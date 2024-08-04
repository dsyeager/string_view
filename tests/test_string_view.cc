#include "string_view.h"

#include <source_location>
#include <iostream>

using dsy::string_view;
using namespace std::literals;

class tester
{
public:
    tester()
    : test_cnt(0), fail_cnt(0)
    {}
    ~tester()
    {
        std::cout << "Test cnt: " << test_cnt << std::endl;
        std::cout << "Failure cnt: " << fail_cnt << std::endl;
    }

    int test_cnt;
    int fail_cnt;
};
static tester test_obj; 

void check_equals(auto left, auto right, const std::source_location loc = std::source_location::current())
{
    test_obj.test_cnt++;
    if (left != right)
    {
        std::cerr << "FAILED, " << loc.function_name() << ", line: " << loc.line() << ", " << left << " != " << right << std::endl;
        test_obj.fail_cnt++;
    }
}

void test_trim()
{
    string_view val("marry.had.a.little.lamb");
    // ltrim
    check_equals(val.ltrim(' '), "marry.had.a.little.lamb"sv);
    check_equals(val.ltrim('m'), "arry.had.a.little.lamb"sv);

    // rtrim
    check_equals(val.rtrim(' '), "arry.had.a.little.lamb"sv);
    check_equals(val.rtrim('b'), "arry.had.a.little.lam"sv);
}

void test_before()
{
    string_view val("marry.had.a.little.lamb");
    // char
    check_equals(val.before('.'), "marry"sv);
    check_equals(val.before('y'), "marr"sv);
    check_equals(val.before(':'), val);

    // string_view
    check_equals(val.before("had"sv), "marry."sv);
    check_equals(val.before("bob"sv), val);
}

void test_after()
{
    string_view val("marry.had.a.little.lamb");
    // char
    check_equals(val.after('.'), "had.a.little.lamb"sv);
    check_equals(val.after(':'), ""sv);

    // string_view
    check_equals(val.after("little"sv), ".lamb"sv);
}

int main (int argc, char **argv)
{
    test_trim();
    test_before();
    test_after();
}
