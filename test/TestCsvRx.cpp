#include <boost/test/unit_test.hpp>

#include "serialcsvplugin/regex_util.h"
#include <regex>
#include <string>




BOOST_AUTO_TEST_CASE(one_double_value_regex_test)
{
    const std::regex value_re(R"([+-]?[0-9]+\.?[0-9]*)");

    std::string input("1.4");
    std::smatch mo;
    bool success = std::regex_match(input, mo, value_re);
    BOOST_REQUIRE(success);
    BOOST_CHECK_EQUAL(mo.size(), 1);

    auto m = mo[0];
    BOOST_CHECK_EQUAL(m.str(), "1.4");
}

BOOST_AUTO_TEST_CASE(two_double_value_regex_test)
{
    const std::regex value_re(R"(([+-]?[0-9]+\.?[0-9]*)[,;\t]?\s?)");

    std::string input("1.4, 2.8");
    std::smatch mo;
    bool success;

    success = std::regex_search(input, mo, value_re);
    BOOST_REQUIRE(success);
    BOOST_CHECK_EQUAL(mo.size(), 2);

    BOOST_TEST_MESSAGE("m[0] = " << mo[0].str());
    BOOST_TEST_MESSAGE("m[1] = " << mo[1].str());

    auto m = mo[1];
    BOOST_CHECK_EQUAL(m.str(), "1.4");

    BOOST_TEST_MESSAGE("pos[0] = " << mo.position(0));
    BOOST_TEST_MESSAGE("pos[1] = " << mo.position(1));
    BOOST_TEST_MESSAGE("pos[2] = " << mo.position(2));

    BOOST_TEST_MESSAGE("len[0] = " << mo.length(0));
    BOOST_TEST_MESSAGE("len[1] = " << mo.length(1));
    BOOST_TEST_MESSAGE("len[2] = " << mo.length(2));

    BOOST_TEST_MESSAGE("prefix = " << mo.prefix().str());
    BOOST_TEST_MESSAGE("suffix = " << mo.suffix().str());


    // 2nd match:
    std::string input2 = mo.suffix().str();
    success = std::regex_search(input2, mo, value_re);
    BOOST_REQUIRE(success);
    BOOST_CHECK_EQUAL(mo.size(), 2);

    BOOST_TEST_MESSAGE("m[0] = " << mo[0].str());
    BOOST_TEST_MESSAGE("m[1] = " << mo[1].str());

    m = mo[1];
    BOOST_CHECK_EQUAL(m.str(), "2.8");

    BOOST_TEST_MESSAGE("pos[0] = " << mo.position(0));
    BOOST_TEST_MESSAGE("pos[1] = " << mo.position(1));
    BOOST_TEST_MESSAGE("pos[2] = " << mo.position(2));

    BOOST_TEST_MESSAGE("len[0] = " << mo.length(0));
    BOOST_TEST_MESSAGE("len[1] = " << mo.length(1));
    BOOST_TEST_MESSAGE("len[2] = " << mo.length(2));

    BOOST_TEST_MESSAGE("prefix = " << mo.prefix().str());
    BOOST_TEST_MESSAGE("suffix = " << mo.suffix().str());

}

BOOST_AUTO_TEST_CASE(two_double_value_regex_consume_test)
{
    const std::regex value_re(R"(([+-]?[0-9]+\.?[0-9]*)[,;\t]?\s?)");

    std::string input("1.4, 2.8");
    std::smatch mo;
    bool success;
    std::string value;

    success = re::searchAndConsume(&input, value_re, &value);
    BOOST_REQUIRE(success);
    BOOST_TEST_MESSAGE("value = " << value);
    BOOST_CHECK_EQUAL(value, "1.4");

    success = re::searchAndConsume(&input, value_re, &value);
    BOOST_REQUIRE(success);
    BOOST_TEST_MESSAGE("value = " << value);
    BOOST_CHECK_EQUAL(value, "2.8");

}


BOOST_AUTO_TEST_CASE(timestamp_two_double_value_regex_consume_test)
{
    const std::regex timestamp_re("(#t:([0-9]*)[,;\t]?)");
    const std::regex value_re(R"(([+-]?[0-9]+\.?[0-9]*)[,;\t]?\s?)");

    std::string input("#t:1500, 14.5, -32.4, 78.8, 79.5 \n");
    std::smatch mo;
    bool success;
    std::string value;
    unsigned long ts;

    // timestamp
    success = re::searchAndConsume(&input, timestamp_re, &ts);
    BOOST_REQUIRE(success);
    BOOST_TEST_MESSAGE("ts = " << ts);
    BOOST_CHECK_EQUAL(ts, 1500);

    // value 1
    success = re::searchAndConsume(&input, value_re, &value);
    BOOST_REQUIRE(success);
    BOOST_TEST_MESSAGE("value = " << value);
    BOOST_CHECK_EQUAL(value, "14.5");

    // value 2
    success = re::searchAndConsume(&input, value_re, &value);
    BOOST_REQUIRE(success);
    BOOST_TEST_MESSAGE("value = " << value);
    BOOST_CHECK_EQUAL(value, "-32.4");

}
