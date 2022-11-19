#include <lcxx/client.hpp>

#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace lcxx::client {

    namespace asio  = boost::asio;
    namespace ssl   = asio::ssl;
    namespace beast = boost::beast;
    namespace http  = beast::http;

    std::optional< net::dynamic_response > request( net::request const & req, std::string const & host,
                                                    std::string_view const port )
    {
        std::optional< net::dynamic_response > resp;
        asio::io_context                       io;

        asio::co_spawn(
            io,
            [&]() -> asio::awaitable< void > {
                ssl::context                           ctx_( ssl::context::tlsv12 );
                beast::ssl_stream< beast::tcp_stream > stream( io, ctx_ );
                ctx_.set_verify_mode( asio::ssl::verify_peer );
                auto set_timeout = [&] {
                    beast::get_lowest_layer( stream ).expires_after( std::chrono::seconds( 30 ) );
                };

                set_timeout();
                auto results = co_await asio::ip::tcp::resolver( io ).async_resolve( host, port, asio::use_awaitable );

                co_await beast::get_lowest_layer( stream ).async_connect( results, asio::use_awaitable );

                set_timeout();
                co_await stream.async_handshake( ssl::stream_base::client, asio::use_awaitable );

                set_timeout();
                auto bytes_sent = co_await std::visit(
                    [&]( auto && req_ ) { return http::async_write( stream, req_, asio::use_awaitable ); }, req );
                if ( bytes_sent == 0 )
                    co_return;

                auto buffer = beast::flat_buffer{ 1024 * 1024 };
                resp        = net::dynamic_response{};
                co_await net::http::async_read( stream, buffer, resp.value(), asio::use_awaitable );

                set_timeout();
                boost::system::error_code ec;
                co_await stream.async_shutdown( asio::redirect_error( asio::use_awaitable, ec ) );
            },
            [&]( std::exception_ptr e ) {
                io.stop();
                if ( e )
                    std::rethrow_exception( e );
            } );

        auto l = asio::make_work_guard( io );

        io.run();

        return resp;
    }

}  // namespace lcxx::client