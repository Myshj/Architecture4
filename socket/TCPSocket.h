
#ifndef COMPUTERARCHITECTURE4_TCPSOCKET_H
#define COMPUTERARCHITECTURE4_TCPSOCKET_H

#include <exception>
#include <string>
#include <memory>


using namespace std;

/*
 * Класи, що відповідають за сокети.
 */
class SocketError : public exception {
public:
    SocketError(string msg = string("Something wrong with tcp socket."), int error_number=-1)
            : error_number(error_number), msg(msg) {}
    virtual const char *what() const noexcept (true) override;
    const int error_number;
private:
    string msg;

};

class TCPSocket {
public:
    TCPSocket(int descriptor) : socket_descriptor(descriptor) {}
    TCPSocket(const TCPSocket&) = delete;
    TCPSocket& operator=(const TCPSocket&) = delete;

    virtual ~TCPSocket();

    void writeData(const void *data, size_t size);
    ssize_t readData(void *buffer, size_t size);

protected:
    int getDescriptor();
    TCPSocket();

private:
    int socket_descriptor;
};

class InTCPSocket : public TCPSocket {
public:
    InTCPSocket(unsigned short port, int backlog = 10);
    unique_ptr<TCPSocket> accept_connection();

private:
    unsigned short port;

};

class OutTCPSocket : public TCPSocket {
public:
    OutTCPSocket(string host, unsigned short port);

    unsigned short getPort() const {
        return port;
    }

    string getHost() const {
        return host;
    }

private:
    string host;
    unsigned short port;

};



#endif //COMPUTERARCHITECTURE4_TCPSOCKET_H
