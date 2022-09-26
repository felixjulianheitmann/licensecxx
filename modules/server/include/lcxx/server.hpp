#ifndef LCXX__LCXX_SERVER_HPP__
#define LCXX__LCXX_SERVER_HPP__

#include <functional>
#include <string>

#include <boost/beast.hpp>

#include <lcxx/lcxx.hpp>

namespace lcxx {

    using request = std::string;

    class server {
    public:
        enum class run_option {
            async,
            sync,
        };

        server( std::string const & ip, uint16_t port );

        void on_get_endpoint( std::string const & endpoint, std::function< void() > callback );
        void on_put_endpoint( std::string const & endpoint, std::function< void() > callback );

        license read_license( request const & req, crypto::rsa_key_t private_key );
        license write_license( request & req, crypto::rsa_key_t private_key );

        void run( run_option ro );
    };

}  // namespace lcxx

#endif  // LCXX__LCXX_SERVER_HPP__