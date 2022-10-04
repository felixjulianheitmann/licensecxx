#include <lcxx/client.hpp>

#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace lcxx::client {

    namespace net   = boost::asio;
    namespace ssl   = net::ssl;
    namespace beast = boost::beast;
    namespace http  = beast::http;

    void request( std::string const & host, request_t const & req, response_cb const & on_response )
    {
        response        resp;
        net::io_context io;

        net::co_spawn(
            io,
            [&]() -> net::awaitable< void > {
                error_code ec;
                auto       error = net::redirect_error( net::use_awaitable, ec );

                net::ip::tcp::resolver resolver( io );
                auto                   results = co_await resolver.async_resolve( host, error );
                if ( ec )
                    co_return;
                for ( auto const & endpoint : results ) {
                    auto                                   ctx_ = ssl::context( ssl::context::tlsv12 );
                    beast::ssl_stream< beast::tcp_stream > stream( net::ip::tcp::socket( io, endpoint ), ctx_ );
                    co_await stream.async_handshake( ssl::stream_base::client, error );
                    if ( ec )
                        co_return;

                    auto bytes_sent = co_await std::visit(
                        [&]( auto && req_ ) { return http::async_write( stream, req_, error ); }, req );
                    if ( ec || bytes_sent == 0 )
                        co_return;

                    auto buffer = beast::flat_buffer{ 1024 * 1024 };
                    co_await http::async_read( stream, buffer, resp, error );
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

        auto l = net::make_work_guard( io );
        io.run();

        if ( on_response )
            on_response( resp );
    }

}  // namespace lcxx::client