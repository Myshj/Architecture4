
#include "../socket/TCPSocket.h"
#include "../connection/connection.h"
#include <boost/serialization/vector.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <random>
#include <chrono>

#define DEFAULT_PORT 4242
#define DEFAULT_HOST "localhost"

using namespace std;
using namespace boost::program_options;

int main(int argc, char *argv[]) {
    string hostname;
    unsigned short port;
    int request_length;
    double a, b;

    options_description desc("Client options");
    desc.add_options()
            ("help,h", "produce help message")
            ("port,p", value<unsigned short>(&port)->default_value(DEFAULT_PORT), "specify port")
            ("host", value<string>(&hostname)->default_value(DEFAULT_HOST), "specify host")
            ("lenght,l", value<int>(&request_length)->required(), "set request length")
            ("a", value<double>(&a)->default_value(-10000.0), "inverval begin")
            ("b", value<double>(&b)->default_value(10000.0), "inverval end");
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
    uniform_real_distribution<double> distribution(a, b);

//generate vector
    vector<double> vector_to_sort(request_length);
    for (double& el: vector_to_sort) {
        el = distribution(generator);
    }

// open connection
    OutTCPSocket out(hostname, port);

// send vector tor sort
    serializedWrite(out, vector_to_sort);


//get sorted vector
    vector<double> sorted_vector = serializedRead<vector <double>>(out);

    return 0;
}
