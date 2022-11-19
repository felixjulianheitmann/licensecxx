#include <lcxx/server.hpp>

#include <ranges>

#include <lcxx/lcxx.hpp>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <iostream>

namespace beast = boost::beast;
namespace http  = beast::http;
namespace asio  = boost::asio;
namespace ssl   = boost::asio::ssl;
using tcp       = boost::asio::ip::tcp;

namespace lcxx {

    server::server( std::string const & ip, uint16_t port, std::string const & certificate_path,
                    std::string const & key_path, std::size_t receive_buffer_size ) :
        ioc_(),
        local_endpoint_( { asio::ip::make_address( ip ), port } ),
        ctx_( ssl::context::tlsv12 ),
        ioc_thread_(),
        map_mutex_(),
        cb_map_(),
        recv_buf_size_( receive_buffer_size )
    {

        ctx_.use_certificate_file( certificate_path, ssl::context_base::pem );
        ctx_.use_rsa_private_key_file( key_path, ssl::context::pem );
    }

    server::~server() { stop(); }

    void server::on_endpoint( std::string const & endpoint, request_cb callback )
    {
        if ( endpoint == "/*" ) {
            on_default( callback );
        }
        else {
            auto l = std::scoped_lock{ map_mutex_ };
            cb_map_.insert_or_assign( endpoint, callback );
        }
    }

    void server::on_default( request_cb callback )
    {
        auto l      = std::scoped_lock{ default_cb_mutex_ };
        default_cb_ = callback;
    }

    void server::run( run_option ro )
    {
        asio::co_spawn( ioc_, handle_loop_coro(), [this]( std::exception_ptr e ) {
            if ( e )
                std::rethrow_exception( e );
        } );

        auto run_ioc = [this] {
            auto work = boost::asio::make_work_guard( ioc_ );
            ioc_.run();
        };

        if ( ro == run_option::sync )
            run_ioc();
        else
            ioc_thread_ = std::thread( run_ioc );
    }

    void server::stop()
    {
        ioc_.stop();

        if ( ioc_thread_.joinable() ) {
            ioc_thread_.join();
        }
    }

    auto server::handle_loop_coro() -> asio::awaitable< void >
    {
        boost::system::error_code ec;
        auto                      error = asio::redirect_error( asio::use_awaitable, ec );

        asio::ip::tcp::acceptor acceptor( ioc_ );
        acceptor.open( local_endpoint_.protocol() );
        acceptor.set_option( asio::socket_base::reuse_address( true ) );
        acceptor.bind( local_endpoint_ );
        acceptor.listen( asio::socket_base::max_listen_connections );

        while ( true ) {
            ec = {};

            asio::ip::tcp::socket socket( ioc_ );
            co_await acceptor.async_accept( socket, error );
            if ( ec ) {
                throw ec;
            }
            else {
                asio::co_spawn( ioc_,
                                run_session( beast::ssl_stream< beast::tcp_stream >( std::move( socket ), ctx_ ) ),
                                [&]( std::exception_ptr e ) {
                                    if ( e )
                                        std::rethrow_exception( e );
                                } );
            }
        }
    }

    auto server::run_session( beast::ssl_stream< beast::tcp_stream > stream ) -> asio::awaitable< void >
    {

        boost::system::error_code ec;
        auto                      error = asio::redirect_error( asio::use_awaitable, ec );
        auto set_timeout = [&] { beast::get_lowest_layer( stream ).expires_after( std::chrono::seconds( 30 ) ); };
        auto shutdown    = [&]() { return stream.async_shutdown( error ); };

        set_timeout();
        co_await stream.async_handshake( ssl::stream_base::server, error );
        if ( ec ) {
            co_await shutdown();
            co_return;
        }

        beast::flat_buffer   buffer{ recv_buf_size_ };
        net::dynamic_request req;

        set_timeout();
        auto bytes_transferred = co_await http::async_read( stream, buffer, req, error );
        if ( ec || bytes_transferred == 0 ) {
            co_await shutdown();
            co_return;
        }

        auto target = req.target().to_string();

        std::optional< net::response > resp;
        {
            auto l = std::scoped_lock{ map_mutex_ };
            if ( cb_map_.contains( target ) ) {
                resp = cb_map_.at( target )( req );
            }
            else if ( auto match = pattern_match( target ); !match.empty() ) {
                resp = cb_map_.at( match )( req );
            }
            else {
                auto l = std::scoped_lock{ default_cb_mutex_ };
                if ( default_cb_ )
                    resp = default_cb_->operator()( req );
            }
        }

        if ( resp ) {
            auto bytes_written = co_await std::visit(
                [&]( auto && resp_ ) { return http::async_write( stream, resp_, error ); }, *resp );

            if ( ec || bytes_written == 0 ) {
                co_await shutdown();
                co_return;
            }
        }
        co_await shutdown();
    }

    auto server::is_pattern( std::string_view const target ) -> bool { return target.ends_with( "/*" ); }

    auto server::pattern_match( std::string_view const target ) -> std::string
    {
        auto remove_one = []( std::string const & path ) -> std::string {
            if ( auto pos = path.find_last_of( "/" ); pos != std::string::npos ) {
                return path.substr( 0, pos );
            }
            return {};
        };

        std::string path( target.begin(), target.end() );

        while ( !path.empty() ) {
            if ( cb_map_.contains( path + "/*" ) ) {
                return path + "/*";
            }
            path = remove_one( path );
        };

        return {};
    }

}  // namespace lcxx