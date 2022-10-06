#include <lcxx/client.hpp>

#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace lcxx::client {

    namespace asio  = boost::asio;
    namespace ssl   = asio::ssl;
    namespace beast = boost::beast;
    namespace http  = beast::http;

    void request( std::string const & host, net::request const & req, response_cb const & on_response )
    {
        net::dynamic_response resp;
        asio::io_context      io;

        asio::co_spawn(
            io,
            [&]() -> asio::awaitable< void > {
                net::error_code ec;
                auto            error = asio::redirect_error( asio::use_awaitable, ec );

                asio::ip::tcp::resolver resolver( io );
                auto                    results = co_await resolver.async_resolve( host, error );
                if ( ec )
                    co_return;
                for ( auto const & endpoint : results ) {
                    auto                                   ctx_ = ssl::context( ssl::context::tlsv12 );
                    beast::ssl_stream< beast::tcp_stream > stream( asio::ip::tcp::socket( io, endpoint ), ctx_ );
                    co_await stream.async_handshake( ssl::stream_base::client, error );
                    if ( ec )
                        co_return;

                    auto bytes_sent = co_await std::visit(
                        [&]( auto && req_ ) { return http::async_write( stream, req_, error ); }, req );
                    if ( ec || bytes_sent == 0 )
                        co_return;

                    auto buffer = beast::flat_buffer{ 1024 * 1024 };
                    co_await net::http::async_read( stream, buffer, resp, error );
                    if ( ec )
                        co_return;

                    co_await stream.async_shutdown( error );
                    co_return;
                }
            },
            [&]( std::exception_ptr e ) {
                io.stop();
                if ( e )
                    std::rethrow_exception( e );
            } );

        auto l = asio::make_work_guard( io );
        io.run();

        if ( on_response )
            on_response( resp );
    }

}  // namespace lcxx::client