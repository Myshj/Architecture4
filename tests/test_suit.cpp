
#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>

#define BOOST_TEST_MAIN

boost::unit_test::test_suite* init_unit_test_suite(int argc, char* argv[]) {
    boost::unit_test::test_suite* test = BOOST_TEST_SUITE("Master test suite");
    return test;
}/

