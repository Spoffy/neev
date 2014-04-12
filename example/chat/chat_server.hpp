#ifndef CHAT_SERVER_INCLUDE
#define CHAT_SERVER_INCLUDE

#include <boost/asio.hpp>
#include <neev/server/basic_server.hpp>
#include <vector>
#include "connection.hpp"


//! Basic chat server for inter-user messaging.
class chat_server {
    public:
        //! Opens the server on the specified port.
        /*! Listens for chat messages on the specified port.
            \param port The port number to listen on.
            \return Doesn't return unless the server failed to start.
        */
        void open_on_port(const std::string& port);

        //! Closes the chat server.
        void close();

    private:
        void on_new_client(const boost::shared_ptr<boost::asio::ip::tcp::socket>& socket);
        void on_message_receive(connection&, const std::string&);
        void on_connection_close(connection&);

        std::vector<boost::shared_ptr<connection>> connections_;
        neev::basic_server server_;
};

#endif