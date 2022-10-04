#ifndef LCXX__LCXX_SERVER_HPP__
#define LCXX__LCXX_SERVER_HPP__

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/ssl.hpp>

#include <lcxx/net_common.hpp>

namespace lcxx::client {
    using string_request  = detail::networking::string_request;
    using dynamic_request = detail::networking::dynamic_request;
    using request_t       = std::variant< string_request, dynamic_request >;
    using response        = detail::networking::dynamic_response;
    using verb            = detail::networking::verb;
    using error_code      = detail::networking::error_code;

    using response_cb = std::function< void( response const & ) >;

    void request( std::string const & endpoint, request_t const & req, response_cb const & on_response );

}  // namespace lcxx::client

#endif  // LCXX__LCXX_SERVER_HPP__