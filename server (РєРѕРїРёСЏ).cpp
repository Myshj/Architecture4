
#define DEFAULT_PORT 4242
#define THREADING_FORK 1
#define THREADING_PTHREAD 2
#define ASIO_ASYNC 3

#ifndef SERVER_MODEL
#define SERVER_MODEL ASIO_ASYNC
#endif

#include <iostream>
#include <boost/serialization/vector.hpp>
#include <boost/program_options.hpp>
#include "connection/connection.h"

#if (SERVER_MODEL == THREADING_FORK || SERVER_MODEL == THREADING_PTHREAD)

#include "socket/TCPSocket.h"
#include "thread/myThread.h"

using namespace concurrent;

#elif (SERVER_MODEL == ASIO_ASYNC)
#include "tcp_server/asioTcpServer.h"
#endif

using namespace std;
using namespace boost::program_options;

template <class Socket>
void handleRequest(Socket &socket) {
    vector<double> provided_vector = serializedRead< vector<double> >(socket);
    sort(begin(provided_vector), end(provided_vector));
    serializedWrite(socket, provided_vector);
}

int main(int argc, char *argv[]) {
    unsigned short port;

    options_description desc("Client options");
    desc.add_options()
            ("help,h", "produce help message")
            ("port,p", value<unsigned short>(&port)->default_value(DEFAULT_PORT), "server port");
    variables_map args;
    try {
        parsed_options parsed = command_line_parser(argc, argv).options(desc).run();
        store(parsed, args);
        notify(args);
    } catch(std::exception& ex) {
        cerr << desc << endl;
        exit(EXIT_FAILURE);
    }

#if (SERVER_MODEL == THREADING_FORK || SERVER_MODEL == THREADING_PTHREAD)
    InTCPSocket server_socket(port);

    while (true) {
        auto new_socket = server_socket.accept_connection();

#if (SERVER_MODEL == THREADING_FORK)
        if ( !fork() ) {
            handleRequest(*new_socket.get());
            break;
        }
#elif (SERVER_MODEL == THREADING_PTHREAD)
        auto socket_ptr = new unique_ptr<TCPSocket>(std::move(new_socket));
        Thread task_thread([](void* socket)->void* {
            unique_ptr<TCPSocket>* data_socket = (unique_ptr<TCPSocket>*)socket;
            handleRequest(*(*data_socket).get());
            delete data_socket;
            return nullptr;
        }, socket_ptr);
        task_thread.deatch();
#endif
    }
#elif (SERVER_MODEL == ASIO_ASYNC)

    AsioTCP::TCPServer server(port, [] (AsioTCP::TCPServer::socket_ptr socket)->void {
        handleRequest(*socket);
    });
    server.run();

#endif

    return 0;
}
