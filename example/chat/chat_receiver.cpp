#include "chat_receiver.hpp"

chat_receiver::chat_receiver(chat_client *const client) :
client_(client) {
}

void chat_receiver::on_transfer_complete() {
  client_->message_received(receiver_->data());
}

void chat_receiver::on_transfer_error(const boost::system::error_code& code) {
  client_->disconnect(code.message());
}

void chat_receiver::async_wait_message(const socket_ptr& socket) {
  receiver_ = make_fixed32_receiver<no_timer>(socket);
  receiver_->register_transfer_listener(shared_from_this());
  receiver_->async_transfer();
}
