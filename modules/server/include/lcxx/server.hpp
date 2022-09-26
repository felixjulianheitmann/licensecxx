#ifndef LCXX__LCXX_SERVER_HPP__
#define LCXX__LCXX_SERVER_HPP__

#include <functional>
#include <string>
#include <thread>
#include <unordered_map>

#include <boost/asio.hpp>
#include <boost/beast.hpp>

#include <lcxx/lcxx.hpp>

namespace lcxx {

    class server {

    public:
        using request  = boost::beast::http::request< boost::beast::http::dynamic_body >;
        using response = boost::beast::http::response< boost::beast::http::dynamic_body >;
        using get_cb   = std::function< void( request const &, response & ) >;
        using put_cb   = std::function< void( request const &, response & ) >;

        enum class run_option {
            async,
            sync,
        };

        server( std::string const & ip, uint16_t port );

        void on_get_endpoint( std::string const & endpoint, get_cb callback );
        void on_put_endpoint( std::string const & endpoint, put_cb callback );

        auto read_license( request const & req, crypto::rsa_key_t private_key ) -> license;
        void write_license( license const &, response & resp, crypto::rsa_key_t private_key );

        void run( run_option ro );
        void stop();

    private:
        using get_cb_map = std::unordered_map< std::string, get_cb >;
        using put_cb_map = std::unordered_map< std::string, put_cb >;

        void handle_loop();
        auto handle_loop_coro() -> boost::asio::awaitable< void >;

        boost::asio::io_context        ioc_;
        boost::asio::ip::tcp::acceptor acceptor_;
        boost::asio::ip::tcp::socket   socket_;

        run_option  ro_;
        std::thread ioc_thread_;

        std::mutex map_mutex_;
        get_cb_map get_cb_map_;
        put_cb_map put_cb_map_;
    };

}  // namespace lcxx

#endif  // LCXX__LCXX_SERVER_HPP__