#include <lcxx/server.hpp>

#include <ranges>

#include <lcxx/lcxx.hpp>

#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace beast = boost::beast;
namespace http  = beast::http;
namespace net   = boost::asio;
using tcp       = boost::asio::ip::tcp;

namespace lcxx {

    server::server( std::string const & ip, uint16_t port ) :
        ioc_(),
        acceptor_( ioc_, { net::ip::make_address( ip ), port } ),
        socket_( ioc_ ),
        ioc_thread_(),
        map_mutex_(),
        cb_map_()
    {
    }

    void server::on_endpoint( std::string const & endpoint, request_cb callback )
    {
        auto l = std::scoped_lock{ map_mutex_ };
        cb_map_.insert_or_assign( endpoint, callback );
    }

    auto server::read_license( request const & req, crypto::rsa_key_t private_key ) -> license { return {}; }

    void server::write_license( license const & lic, file_response & req, crypto::rsa_key_t private_key ) {}
    void server::write_license( license const & lic, string_response & req, crypto::rsa_key_t private_key ) {}

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

    void server::stop() { ioc_.stop(); }

    auto server::handle_loop() -> void
    {
        net::co_spawn( ioc_, handle_loop_coro(), [this]( std::exception_ptr e ) {
            if ( e )
                std::rethrow_exception( e );
        } );
    }

    net::awaitable< void > server::handle_loop_coro()
    {
        boost::system::error_code ec;
        auto                      error = net::redirect_error( net::use_awaitable, ec );

        auto shutdown = [&]() {
            socket_.shutdown( tcp::socket::shutdown_send );
            socket_.close();
        };

        while ( true ) {
            ec = {};

            co_await acceptor_.async_accept( socket_, error );
            if ( ec )
                continue;

            beast::flat_buffer buffer{ 1024 * 1024 };
            request            req;

            auto bytes_transferred = co_await http::async_read( socket_, buffer, req, error );
            if ( ec || bytes_transferred == 0 )
                continue;

            auto target = req.target().to_string();

            {
                // I would like to reduce the code duplication, but I don't know how to write a lambda/function that
                // uses co_await
                auto l = std::scoped_lock{ map_mutex_ };
                if ( cb_map_.contains( target ) ) {
                    auto resp          = cb_map_.at( target )( req );
                    auto bytes_written = co_await std::visit(
                        [&]( auto && resp ) { return http::async_write( socket_, resp, error ); }, resp );

                    if ( ec || bytes_written == 0 ) {
                        shutdown();
                        continue;
                    }
                }
                else if ( auto match = pattern_match( target ); !match.empty() ) {
                    auto resp          = cb_map_.at( match )( req );
                    auto bytes_written = co_await std::visit(
                        [&]( auto && resp ) { return http::async_write( socket_, resp, error ); }, resp );

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