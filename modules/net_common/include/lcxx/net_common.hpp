#ifndef LCXX__NET_COMMON_NET_COMMON_HPP__
#define LCXX__NET_COMMON_NET_COMMON_HPP__

#include <variant>

#include <boost/beast/http/dynamic_body.hpp>
#include <boost/beast/http/file_body.hpp>
#include <boost/beast/http/string_body.hpp>

namespace lcxx::net {
    using file_response    = boost::beast::http::response< boost::beast::http::file_body >;
    using string_response  = boost::beast::http::response< boost::beast::http::string_body >;
    using dynamic_response = boost::beast::http::response< boost::beast::http::dynamic_body >;
    using response         = std::variant< file_response, string_response, dynamic_response >;

    using string_request  = boost::beast::http::request< boost::beast::http::string_body >;
    using dynamic_request = boost::beast::http::request< boost::beast::http::dynamic_body >;
    using request         = std::variant< string_request, dynamic_request >;

    using verb       = boost::beast::http::verb;
    using error_code = boost::system::error_code;
    namespace http   = boost::beast::http;

}  // namespace lcxx::net

#endif  // LCXX__NET_COMMON_NET_COMMON_HPP__