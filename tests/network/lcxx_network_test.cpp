// #include <gtest/gtest.h>

#include <lcxx/client.hpp>
#include <lcxx/server.hpp>

using namespace lcxx;

// TEST( Test_Suite_Network, Roundtrip )
int main()
{
    server server( "0.0.0.0", 8080, "certificate.pem", "key.pem" );

    server.on_endpoint( "test_endpoints/*", []( auto const & req ) {
        net::string_response resp;
        resp.result( net::http::status::ok );
        return resp;
    } );

    server.run( server::run_option::async );

    net::string_request req;
    req.method( net::verb::get );
    auto resp = client::request( "https://localhost:8080/test_endpoints/some_test", req );

    assert( resp.result() == net::http::status::ok );
    // EXPECT_EQ( resp.result(), net::http::status::ok );
}