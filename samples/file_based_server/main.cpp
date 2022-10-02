#include <filesystem>
#include <fstream>
#include <iostream>

#include <lcxx/server.hpp>

using namespace lcxx;

void on_get_license( server::request const & req, server::string_response & resp, std::string const & path );
void on_post_license( server::request const & req, server::string_response & resp, std::string const & path );
void on_delete_license( server::request const & req, server::string_response & resp, std::string const & path );

auto main() -> int
{

    server server( "0.0.0.0", 8080 );

    // Read/Store licenses from/to file system
    server.on_endpoint( "/licenses/*", [&]( server::request const & req ) {
        server::string_response resp;
        resp.set( boost::beast::http::field::content_type, "application/json" );
        std::string path = { req.target().begin() + 1, req.target().end() };

        try {
            switch ( req.method() ) {
            case server::verb::get:
                on_get_license( req, resp, path );
                break;
            case server::verb::post:
                on_post_license( req, resp, path );
                break;
            case server::verb::delete_:
                on_delete_license( req, resp, path );
                break;
            default:
                resp.body() = "Given verb not supported";
                break;
            }
        }
        catch ( std::exception & e ) {
            resp.set( boost::beast::http::field::content_type, "text/html" );
            resp.body() = std::string{ "Could not load license, error: " } + e.what();
        }

        return resp;
    } );

    try {
        server.run( server::run_option::sync );
    }
    catch ( std::exception & e ) {
        server.stop();
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}

void on_get_license( server::request const & req, server::string_response & resp, std::string const & path )
{
    if ( !std::filesystem::exists( path ) )
        throw std::runtime_error( "File not found" );
    std::ifstream ifs{ path };
    auto          license = std::string{ std::istreambuf_iterator< char >{ ifs }, std::istreambuf_iterator< char >{} };
    resp.body()           = license;
}

void on_post_license( server::request const & req, server::string_response & resp, std::string const & path )
{
    std::ofstream ofs{ path };
    ofs << boost::beast::buffers_to_string( req.body().data() );
    resp.body() = "Successfully saved\n";
}

void on_delete_license( server::request const & req, server::string_response & resp, std::string const & path )
{
    if ( !std::filesystem::exists( path ) )
        throw std::runtime_error( "File not found" );
    std::filesystem::remove( path );
    resp.body() = "Successfully deleted\n";
}
