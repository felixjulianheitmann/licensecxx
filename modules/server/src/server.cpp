#include <lcxx/server.hpp>

#include <lcxx/lcxx.hpp>

#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace beast = boost::beast;
namespace http  = beast::http;
namespace net   = boost::asio;
using tcp       = boost::asio::ip::tcp;

namespace lcxx {

    server::server( std::string const & ip, uint16_t port ) {}

    void server::on_get_endpoint( std::string const & endpoint, std::function< void() > callback ) {}

    void server::on_put_endpoint( std::string const & endpoint, std::function< void() > callback ) {}

    license server::read_license( request const & req, crypto::rsa_key_t private_key ) { return {}; }

    license server::write_license( request & req, crypto::rsa_key_t private_key ) { return {}; }

}  // namespace lcxx