#ifndef LCXX__LCXX_SERVER_HPP__
#define LCXX__LCXX_SERVER_HPP__

#include <functional>
#include <string>
#include <thread>
#include <unordered_map>
#include <variant>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>

#include <lcxx/lcxx.hpp>
#include <lcxx/net_common.hpp>

namespace lcxx {

    class server {

    public:
        using request_cb = std::function< net::response( net::dynamic_request const & ) >;

        enum class run_option {
            async,
            sync,
        };

        /**
         * @brief Construct a new server object
         *
         * @param ip the ip to listen on for incoming connections
         * @param port the port to listen on for incoming connections
         * @param certificate_path the path to the certificate file in .pem format
         * @param key_path the path to the private key file corresponding to the ceritificate in .pem format
         * @param receive_buffer_size optional: the buffer to allocate for incoming requests. Default: 1MB
         */
        server( std::string const & ip, uint16_t port, std::string const & certificate_path,
                std::string const & key_path, std::size_t receive_buffer_size = 1024 * 1024 );
        ~server();

        /**
         * @brief Add callback for a specific endpoint. If a callback already exists for this endpoint it will be
         * overwritten. Ending an endpoint with a wildcard (e.g. '/my_ep_category/*') will have the callback be invoked
         * for any requests matching the pattern. If there is a more specific callback that matches the request first,
         * this wild card handler will not be invoked. It can be used as a default if non of the specific handlers
         * match.
         *
         * @param endpoint an endpoint string optionally ending with a wildcard
         * @param callback the callback to be invoked on a request matching the endpoint
         */
        void on_endpoint( std::string const & endpoint, request_cb callback );

        /**
         * @brief Add a default callback for any request that does not match any registered handler.
         * This is equivalent to `server.on_endpoint("/*", ...);`
         *
         * @param callback the callback to be invoked if none of the specific handlers match the request.
         */
        void on_default( request_cb callback );

        void run( run_option ro );
        void stop();

    private:
        using cb_map = std::unordered_map< std::string, request_cb >;

        auto handle_loop_coro() -> boost::asio::awaitable< void >;
        auto run_session( boost::beast::ssl_stream< boost::beast::tcp_stream > stream )
            -> boost::asio::awaitable< void >;

        auto is_pattern( std::string_view const target ) -> bool;
        auto pattern_match( std::string_view const target ) -> std::string;

        boost::asio::io_context        ioc_;
        boost::asio::ip::tcp::endpoint local_endpoint_;
        boost::asio::ssl::context      ctx_;

        run_option  ro_;
        std::thread ioc_thread_;

        std::mutex                  map_mutex_;
        std::mutex                  default_cb_mutex_;
        cb_map                      cb_map_;
        std::optional< request_cb > default_cb_;
        std::size_t                 recv_buf_size_;
    };

}  // namespace lcxx

#endif  // LCXX__LCXX_SERVER_HPP__