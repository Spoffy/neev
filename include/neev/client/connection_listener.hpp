#ifndef CONNECTION_LISTENER_HEADER
#define CONNECTION_LISTENER_HEADER

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

namespace neev {

class connection_listener {
  public:
    using socket_type = boost::asio::ip::tcp::socket;
    using socket_ptr = std::shared_ptr<socket_type>;

    virtual void on_connection_success(const socket_ptr&) {
      std::cout << "Fire on success call" << std::endl;
    };
    virtual void on_connection_failure(const boost::system::error_code& error) {};
};

}

#endif
