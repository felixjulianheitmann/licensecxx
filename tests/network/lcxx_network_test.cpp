#include <gtest/gtest.h>

#include <lcxx/client.hpp>
#include <lcxx/server.hpp>

using namespace lcxx;

class network_fixture : public ::testing::Test {
protected:
    server server_;
    network_fixture() : server_( server( "127.0.0.1", 8080, "certificate.pem", "key.pem" ) )
    {
        server_.on_endpoint( "/test_endpoints/*", []( auto const & req ) {
            net::string_response resp;
            resp.result( net::http::status::ok );
            if ( req.method() == net::verb::get )
                resp.body() = "Pattern_GET";
            if ( req.method() == net::verb::post )
                resp.body() = "Pattern_POST";
            return resp;
        } );

        server_.on_default( []( auto const & req ) {
            net::string_response resp;
            resp.result( net::http::status::ok );
            if ( req.method() == net::verb::get )
                resp.body() = "Default_GET";
            if ( req.method() == net::verb::post )
                resp.body() = "Default_POST";
            return resp;
        } );

        server_.on_endpoint( "/test_endpoints/specialized/endpoint", []( auto const & req ) {
            net::string_response resp;
            resp.result( net::http::status::ok );
            if ( req.method() == net::verb::get )
                resp.body() = "Specialized_GET";
            if ( req.method() == net::verb::post )
                resp.body() = "Specialized_POST";
            return resp;
        } );
    }
};

TEST_F( network_fixture, Roundtrip_Pattern )
{

    server_.run( server::run_option::async );

    net::string_request req;
    req.method( net::verb::get );
    req.target( "/test_endpoints/some_test" );
    auto const resp = client::request( "https://localhost:8080", req );

    EXPECT_TRUE( resp );
    if ( resp ) {
        EXPECT_EQ( resp->result(), net::http::status::ok );
        EXPECT_EQ( boost::beast::buffers_to_string( resp->body().data() ), "Pattern_GET" );
    }
}

TEST_F( network_fixture, Roundtrip_Specialized )
{

    server_.run( server::run_option::async );

    net::string_request req;
    req.method( net::verb::get );
    req.target( "/test_endpoints/specialized/endpoint" );
    auto const resp = client::request( "https://localhost:8080", req );

    EXPECT_TRUE( resp );
    if ( resp ) {
        EXPECT_EQ( resp->result(), net::http::status::ok );
        EXPECT_EQ( boost::beast::buffers_to_string( resp->body().data() ), "Specialized_GET" );
    }
}

TEST_F( network_fixture, Roundtrip_Default )
{

    server_.run( server::run_option::async );

    net::string_request req;
    req.method( net::verb::get );
    req.target( "/trigger/default" );
    auto const resp = client::request( "https://localhost:8080", req );

    EXPECT_TRUE( resp );
    if ( resp ) {
        EXPECT_EQ( resp->result(), net::http::status::ok );
        EXPECT_EQ( boost::beast::buffers_to_string( resp->body().data() ), "Default_GET" );
    }
}

TEST_F( network_fixture, Roundtrip_Default_Post )
{

    server_.run( server::run_option::async );

    net::string_request req;
    req.method( net::verb::post );
    req.target( "/trigger/default" );
    auto const resp = client::request( "https://localhost:8080", req );

    EXPECT_TRUE( resp );
    if ( resp ) {
        EXPECT_EQ( resp->result(), net::http::status::ok );
        EXPECT_EQ( boost::beast::buffers_to_string( resp->body().data() ), "Default_POST" );
    }
}

TEST_F( network_fixture, Roundtrip_Multiple )
{

    server_.run( server::run_option::async );

    net::string_request req;
    req.method( net::verb::post );
    req.target( "/trigger/default" );
    auto resp = client::request( "https://localhost:8080", req );

    EXPECT_TRUE( resp );
    if ( resp ) {
        EXPECT_EQ( resp->result(), net::http::status::ok );
        EXPECT_EQ( boost::beast::buffers_to_string( resp->body().data() ), "Default_POST" );
    }

    req.method( net::verb::get );
    req.target( "/trigger/default" );
    resp = client::request( "https://localhost:8080", req );

    EXPECT_TRUE( resp );
    if ( resp ) {
        EXPECT_EQ( resp->result(), net::http::status::ok );
        EXPECT_EQ( boost::beast::buffers_to_string( resp->body().data() ), "Default_GET" );
    }

    req.method( net::verb::get );
    req.target( "/test_endpoints/pattern" );
    resp = client::request( "https://localhost:8080", req );

    EXPECT_TRUE( resp );
    if ( resp ) {
        EXPECT_EQ( resp->result(), net::http::status::ok );
        EXPECT_EQ( boost::beast::buffers_to_string( resp->body().data() ), "Pattern_GET" );
    }
}