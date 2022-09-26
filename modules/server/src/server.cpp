#include <lcxx/server.hpp>

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
        get_cb_map_(),
        put_cb_map_()
    {
    }

    void server::on_get_endpoint( std::string const & endpoint, get_cb callback )
    {
        auto l = std::scoped_lock{ map_mutex_ };
        get_cb_map_.insert_or_assign( endpoint, callback );
    }

    void server::on_put_endpoint( std::string const & endpoint, put_cb callback )
    {
        auto l = std::scoped_lock{ map_mutex_ };
        put_cb_map_.insert_or_assign( endpoint, callback );
    }

    auto server::read_license( request const & req, crypto::rsa_key_t private_key ) -> license { return {}; }

    void server::write_license( license const & lic, response & req, crypto::rsa_key_t private_key ) {}

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

            response resp;
            resp.result( http::status::ok );

            auto exec_cb = [&]( auto map ) {
                if ( auto target = req.target().to_string(); map.contains( target ) ) {
                    map.at( target )( req, resp );
                }
            };

            switch ( req.method() ) {
            case http::verb::get:
                exec_cb( get_cb_map_ );
                break;
            case http::verb::put:
                exec_cb( put_cb_map_ );
                break;

            default:
                resp.result( http::status::bad_request );
                resp.set( http::field::content_type, "text/plain" );
                beast::ostream( resp.body() ) << "Invalid request method - expected 'get' or 'put', but got "
                                              << std::string( req.method_string() );
                break;
            }

            auto bytes_written = co_await http::async_write( socket_, resp, error );
            if ( ec || bytes_written == 0 )
                continue;
        }
    }

}  // namespace lcxx