
#include "../connection/connection.h"
#include "../socket/TCPSocket.h"
#include "../thread/myThread.h"
#include <boost/test/unit_test.hpp>
#include <boost/serialization/vector.hpp>
#include <errno.h>

#define  BOOST_TEST_DYN_LINK


BOOST_AUTO_TEST_SUITE(test_suite_name)

    BOOST_AUTO_TEST_CASE(connecting) {
        try {
            OutTCPSocket out("localhost", 4221);
            BOOST_FAIL("Invalid connection didn't throw an exeption.");
        } catch (SocketError& sockerr) {
            BOOST_CHECK_EQUAL( sockerr.error_number, ECONNREFUSED );
        }
    }

    BOOST_AUTO_TEST_CASE(comminication) {
        InTCPSocket in(4242);
        concurrent::Thread outThread([] (void*)->void* {
            OutTCPSocket out("localhost", 4242);
            char msg[3] = "ok";
            out.writeData(msg, strlen(msg) * sizeof(char));
        });
        auto connection = in.accept_connection();
        char buffer[3];
        memset(buffer, 0, strlen(buffer));
        connection.get()->readData(buffer, 3);
        BOOST_CHECK_EQUAL(string(buffer), string("ok"));
    }

BOOST_AUTO_TEST_SUITE_END()
