
#include "../connection/connection.h"
#include <boost/test/unit_test.hpp>
#include <boost/serialization/vector.hpp>
#include <vector>
#include <string>

#define  BOOST_TEST_DYN_LINK

BOOST_AUTO_TEST_SUITE(packing)

BOOST_AUTO_TEST_CASE(string_packing)
    {
        string msg = "hello";
        BOOST_CHECK_EQUAL( msg, unpackFromString<string>(packToString(msg)));
    }

BOOST_AUTO_TEST_CASE(vector_packing)
    {
        vector<double> vector_to_pack(20);
        int i = 0;
        for (double& el: vector_to_pack) {
            el = ++i;
        }
        vector<double> unpacked_vector = unpackFromString<vector<double>>(packToString(vector_to_pack));
        BOOST_CHECK_EQUAL(vector_to_pack.size(), unpacked_vector.size());
        if( !std::equal(begin(vector_to_pack), end(vector_to_pack), begin(unpacked_vector)))
            BOOST_FAIL("Unpacked vector is not equal wiht packed.");
    }


BOOST_AUTO_TEST_SUITE_END()
