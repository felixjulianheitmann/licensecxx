#ifndef LCXX__LCXX_SERVER_HPP__
#define LCXX__LCXX_SERVER_HPP__

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast/core/buffers_to_string.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/ssl.hpp>

#include <lcxx/net_common.hpp>

namespace lcxx::client {

    using response_cb = std::function< void( net::dynamic_response const & ) >;

    void request( std::string const & endpoint, net::request const & req, response_cb const & on_response );

}  // namespace lcxx::client

#endif  // LCXX__LCXX_SERVER_HPP__