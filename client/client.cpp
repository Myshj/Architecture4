
#include "../socket/TCPSocket.h"
#include "../connection/connection.h"
#include <boost/serialization/vector.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <random>
#include <chrono>

/*
 * Оголошення хоста і сервера за замовченням.
 */
#define DEFAULT_PORT 4242
#define DEFAULT_HOST "localhost"

using namespace std;
using namespace boost::program_options;

/*
 * Головна функція роботи клієнта.
 */
int main(int argc, char *argv[]) {
    string hostname;
    unsigned short port;
    int request_length;
    char a, b;

    options_description desc("Client options");
    desc.add_options()
            ("help,h", "produce help message")
            ("port,p", value<unsigned short>(&port)->default_value(DEFAULT_PORT), "specify port")
            ("host", value<string>(&hostname)->default_value(DEFAULT_HOST), "specify host")
            ("lenght,l", value<int>(&request_length)->required(), "set request length")
            ("a", value<char>(&a)->default_value('a'), "inverval begin")
            ("b", value<char>(&b)->default_value('z'), "inverval end");
    variables_map args;
    try {
        parsed_options parsed = command_line_parser(argc, argv).options(desc).run();
        store(parsed, args);
        notify(args);
    } catch(std::exception& ex) {
        cerr << desc << endl;
        exit(EXIT_FAILURE);
    }

    default_random_engine generator(chrono::system_clock::now().time_since_epoch().count());
    uniform_int_distribution<char> distribution(a, b);

    /*
     * Генерування 
     */
    vector<char> symbols(request_length);
    for (char& el: symbols) {
        el = distribution(generator);
        printf("%c", el);
    }
    printf("\n");

    /*
     * Відсилання символів на обробку.
     */
    OutTCPSocket out(hostname, port);
    serializedWrite(out, symbols);


    /*
     * Отримання відповіді.
     */
    vector<char> permutated_symbols = serializedRead<vector <char>>(out);

    for(char& el: permutated_symbols){
        printf("%c",el);
    }

    return 0;
}
