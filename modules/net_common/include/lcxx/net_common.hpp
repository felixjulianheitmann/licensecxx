#ifndef LCXX__NET_COMMON_NET_COMMON_HPP__
#define LCXX__NET_COMMON_NET_COMMON_HPP__

#include <variant>

#include <boost/beast/http/dynamic_body.hpp>
#include <boost/beast/http/file_body.hpp>
#include <boost/beast/http/string_body.hpp>

namespace lcxx::detail::networking {
    using file_response    = boost::beast::http::response< boost::beast::http::file_body >;
    using string_response  = boost::beast::http::response< boost::beast::http::string_body >;
    using dynamic_response = boost::beast::http::response< boost::beast::http::dynamic_body >;

    using string_request  = boost::beast::http::request< boost::beast::http::string_body >;
    using dynamic_request = boost::beast::http::request< boost::beast::http::dynamic_body >;

    using verb       = boost::beast::http::verb;
    using error_code = boost::system::error_code;

}  // namespace lcxx::detail::networking

#endif  // LCXX__NET_COMMON_NET_COMMON_HPP__