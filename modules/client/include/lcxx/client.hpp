#ifndef LCXX__LCXX_CLIENT_HPP__
#define LCXX__LCXX_CLIENT_HPP__

#include <optional>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast/core/buffers_to_string.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/ssl.hpp>

#include <lcxx/net_common.hpp>

namespace lcxx::client {

    using response_cb = std::function< void( net::dynamic_response const & ) >;

    std::optional< net::dynamic_response > request( std::string const & endpoint, net::request const & req );

}  // namespace lcxx::client

#endif  // LCXX__LCXX_CLIENT_HPP__