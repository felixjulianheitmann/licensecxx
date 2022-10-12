#include <lcxx/server.hpp>

#include <ranges>

#include <lcxx/lcxx.hpp>

#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace beast = boost::beast;
namespace http  = beast::http;
namespace asio  = boost::asio;
namespace ssl   = boost::asio::ssl;
using tcp       = boost::asio::ip::tcp;

namespace lcxx {

    server::server( std::string const & ip, uint16_t port, std::string const & certificate_path,
                    std::string const & key_path ) :
        ioc_(),
        acceptor_( ioc_, { asio::ip::make_address( ip ), port } ),
        ctx_( ssl::context::tlsv12 ),
        stream_( asio::ip::tcp::socket( ioc_ ), ctx_ ),
        ioc_thread_(),
        map_mutex_(),
        cb_map_()
    {
        ctx_.use_certificate_file( certificate_path, ssl::context_base::file_format::pem );
    }

    server::~server() { stop(); }

    void server::on_endpoint( std::string const & endpoint, request_cb callback )
    {
        auto l = std::scoped_lock{ map_mutex_ };
        cb_map_.insert_or_assign( endpoint, callback );
    }

    void server::on_default( request_cb callback )
    {
        auto l      = std::scoped_lock{ default_cb_mutex_ };
        default_cb_ = callback;
    }

    void server::run( run_option ro )
    {
        handle_loop();

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

    auto server::handle_loop() -> void
    {
        asio::co_spawn( ioc_, handle_loop_coro(), [this]( std::exception_ptr e ) {
            if ( e )
                std::rethrow_exception( e );
        } );
    }

    asio::awaitable< void > server::handle_loop_coro()
    {
        boost::system::error_code ec;
        auto                      error = asio::redirect_error( asio::use_awaitable, ec );

        auto shutdown = [&]() {
            boost::beast::get_lowest_layer( stream_ ).socket().shutdown( tcp::socket::shutdown_send );
            boost::beast::get_lowest_layer( stream_ ).socket().close();
        };

        while ( true ) {
            ec = {};

            co_await acceptor_.async_accept( beast::get_lowest_layer( stream_ ).socket(), error );
            if ( ec ) {
                shutdown();
                continue;
            }

            co_await stream_.async_handshake( ssl::stream_base::server, error );
            if ( ec ) {
                shutdown();
                continue;
            }

            beast::flat_buffer   buffer{ 1024 * 1024 };
            net::dynamic_request req;

            auto bytes_transferred = co_await http::async_read( stream_, buffer, req, error );
            if ( ec || bytes_transferred == 0 ) {
                shutdown();
                continue;
            }

            auto target = req.target().to_string();

            {
                std::optional< net::response > resp;

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

                if ( resp ) {
                    auto bytes_written = co_await std::visit(
                        [&]( auto && resp_ ) { return http::async_write( stream_, resp_, error ); }, *resp );

                    if ( ec || bytes_written == 0 ) {
                        shutdown();
                        continue;
                    }
                }
            }

            try {
                shutdown();
            }
            catch ( ... ) {
            }
        }
    }

    auto server::is_pattern( std::string_view const target ) -> bool { return target.ends_with( "/*" ); }

    auto server::pattern_match( std::string_view const target ) -> std::string
    {
        namespace views = std::ranges::views;
        std::vector< std::string > path;
        for ( auto const sec : views::split( target, std::string_view{ "/" } ) ) {
            std::string section = std::string{ sec.begin(), sec.end() };
            if ( !section.empty() )
                path.push_back( std::string{ "/" } + section );
        }

        // remove last section for a wildcard match
        path.pop_back();

        while ( !path.empty() ) {
            auto sub_target = path | views::join;
            auto pattern    = std::string{ sub_target.begin(), sub_target.end() } + std::string{ "/*" };
            if ( cb_map_.contains( pattern ) ) {
                return pattern;
            }
            else {
                path.pop_back();
            }
        };

        return {};
    }

}  // namespace lcxx