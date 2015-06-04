
#ifndef COMPUTERARCHITECTURE4_HEADER_FILE_H
#define COMPUTERARCHITECTURE4_HEADER_FILE_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <functional>


/*
 * Клас-асинхронний сервер.
 */
namespace AsioTCP{
    class TCPServer {
    public:
        typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;
        typedef std::function<void(socket_ptr)> CallBackFunction;

        TCPServer(unsigned short port, CallBackFunction func)
                : service(),
                  ep(boost::asio::ip::tcp::v4(), port),
                  acc(service, ep),
                  handle_func(func) {
            socket_ptr sock(new boost::asio::ip::tcp::socket(service));
            start_accept(sock);
        }

        void run() {
            service.run();
        }


    private:
        boost::asio::io_service service;
        boost::asio::ip::tcp::endpoint ep;
        boost::asio::ip::tcp::acceptor acc;

        CallBackFunction handle_func;

        void start_accept(socket_ptr sock) {
            acc.async_accept(*sock,
                             boost::bind(&TCPServer::handle_accept, this, sock, boost::asio::placeholders::error));
        }

        void handle_accept(socket_ptr sock, const boost::system::error_code &err) {
            if (!err) {
                handle_func(sock);
                socket_ptr next_sock(new boost::asio::ip::tcp::socket(service));
                start_accept(next_sock);
            }
        }

    };
}

#endif //COMPUTERARCHITECTURE4_HEADER_FILE_H
