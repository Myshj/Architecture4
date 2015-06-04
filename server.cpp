
/*
 * Ця константа визначає порт (за замовченням), за яким працюватиме сервер.
 */
#define DEFAULT_PORT 4242
/*
 * Ці константивказують на тип серверного ПЗ.
 */
#define THREADING_FORK 1
#define THREADING_PTHREAD 2
#define ASIO_ASYNC 3

/*
 * Модель роботи сервера за замовченням.
 */
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


/*
 * Ця функція виконує знаходження всіх можливих перестановоек вхідних символів.
 * Параметри:
 * symbols - масив вхідних символів
 * 
 * Повертає:
 * Масив всіх можливих комбінацій із вхідних символів.
 * Комбінації розділені '\n'.
 */
vector<char> find_permutations(vector<char> symbols){
    vector<char> ret = vector<char>();

    int pos = 0;
    while(next_permutation(symbols.begin(), symbols.end())){
        for(int i = 0; i < symbols.size(); i++){
            ret.push_back(symbols[i]);
        }
        ret.push_back('\n');
    }

    return ret;

}

template <class Socket>

/*
 * Ця функція виконує обробку запиту, отриманого із сокета.
 * Результат обробки відправляється на сокет.
 */
void handleRequest(Socket &socket) {
    vector<char> provided_vector = serializedRead< vector<char> >(socket);
    vector<char> permutations = find_permutations(provided_vector);
    serializedWrite(socket, permutations);
}


/*
 * Головна функція сервера.
 */
int main(int argc, char *argv[]) {
    unsigned short port;

    /*
     * Розпакування ключів, із якими сервер запускається.
     */
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
    /*
     * Якщо сервер працює багатопоточно, він не використовує asio.
     */
    InTCPSocket server_socket(port);

    /*
     * Основний нескінченний цикл роботи програми.
     */
    while (true) {
        /*
         * Приймання підключення.
         */
        auto new_socket = server_socket.accept_connection();

#if (SERVER_MODEL == THREADING_FORK)
        /*
         * Створення нового процесу для обробки запиту.
         * Цей процес завершиться після обробки запиту.
         */
        if ( !fork() ) {
            handleRequest(*new_socket.get());
            break;
        }
#elif (SERVER_MODEL == THREADING_PTHREAD)
        /*
         * Створення нового потоку для обробки запиту.
         * Цей потік завершиться після обробки запиту і поверне nullptr.
         */
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
    /*
     * Асинхронна обробка запитів.
     */
    AsioTCP::TCPServer server(port, [] (AsioTCP::TCPServer::socket_ptr socket)->void {
        handleRequest(*socket);
    });
    server.run();

#endif

    return 0;
}
