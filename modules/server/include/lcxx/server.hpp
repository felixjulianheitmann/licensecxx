#ifndef LCXX__LCXX_SERVER_HPP__
#define LCXX__LCXX_SERVER_HPP__

namespace lcxx {

    class server {
    private:
    public:
        server();
        ~server();

        // Desired feature set:
        // push new license
        // Receive license
        // authenticate
        // Save licenses to persistant storage
    };

}  // namespace lcxx

#endif  // LCXX__LCXX_SERVER_HPP__