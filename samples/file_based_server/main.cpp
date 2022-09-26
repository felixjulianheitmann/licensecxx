#include <lcxx/server.hpp>

using namespace lcxx;
namespace http = boost::beast::http;

auto main() -> int
{

    server server( "0.0.0.0", 8080 );
    server.on_get_endpoint( "/", [&]( server::request const & req, server::response & resp ) {
        resp.set( http::field::content_type, "text/html; charset=utf-8" );
        resp.insert( "Some string", "some vlaue" );
    } );

    server.run( server::run_option::sync );

    return 0;
}