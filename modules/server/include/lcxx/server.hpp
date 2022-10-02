#ifndef LCXX__LCXX_SERVER_HPP__
#define LCXX__LCXX_SERVER_HPP__

#include <functional>
#include <string>
#include <thread>
#include <unordered_map>
#include <variant>

#include <boost/asio.hpp>
#include <boost/beast.hpp>

#include <lcxx/lcxx.hpp>

namespace lcxx {

    class server {

    public:
        using file_response   = boost::beast::http::response< boost::beast::http::file_body >;
        using string_response = boost::beast::http::response< boost::beast::http::string_body >;
        using response        = std::variant< file_response, string_response >;
        using request         = boost::beast::http::request< boost::beast::http::dynamic_body >;

        using request_cb = std::function< response( request const & ) >;

        using verb       = boost::beast::http::verb;
        using error_code = boost::system::error_code;

        enum class run_option {
            async,
            sync,
        };

        server( std::string const & ip, uint16_t port );

        void on_endpoint( std::string const & endpoint, request_cb callback );

        auto read_license( request const & req, crypto::rsa_key_t private_key ) -> license;
        void write_license( license const &, file_response & resp, crypto::rsa_key_t private_key );
        void write_license( license const &, string_response & resp, crypto::rsa_key_t private_key );

        void run( run_option ro );
        void stop();

    private:
        using cb_map = std::unordered_map< std::string, request_cb >;

        void handle_loop();
        auto handle_loop_coro() -> boost::asio::awaitable< void >;

        auto is_pattern( std::string_view const target ) -> bool;
        auto pattern_match( std::string_view const target ) -> std::string;

        boost::asio::io_context        ioc_;
        boost::asio::ip::tcp::acceptor acceptor_;
        boost::asio::ip::tcp::socket   socket_;

        run_option  ro_;
        std::thread ioc_thread_;

        std::mutex map_mutex_;
        cb_map     cb_map_;
    };

}  // namespace lcxx

#endif  // LCXX__LCXX_SERVER_HPP__