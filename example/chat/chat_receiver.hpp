#ifndef CHAT_RECEIVER_HEADER
#define CHAT_RECEIVER_HEADER

#include "chat_client.hpp"
#include <neev/client/client.hpp>
#include <neev/transfer_listener.hpp>
#include <neev/fixed_mutable_buffer.hpp>
#include <boost/system/error_code.hpp>

using namespace neev;

class chat_client;

class chat_receiver : public neev::transfer_listener, public std::enable_shared_from_this<chat_receiver> {
 public:
  using socket_ptr = typename neev::client::socket_ptr;

  chat_receiver(chat_client *const client);

  void async_wait_message(const socket_ptr&);

  void on_transfer_complete() override;
  void on_transfer_error(const boost::system::error_code& code) override;

 private:
  chat_client *const client_;
  fixed_receiver_ptr<no_timer, std::uint32_t> receiver_;
};

#endif
