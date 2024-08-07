#include "string_view.h"

#include <charconv>
#include <climits>
#include <locale>
#include <source_location>
#include <iostream>
#include <sstream>

using dsy::string_view;
using namespace std::literals;

struct my_nump : std::numpunct<char>
{
    std::string do_grouping() const { return "\3"; }
};

std::string commas(uint64_t n)
{
    std::ostringstream s;
    s.imbue(std::locale(s.getloc(), new my_nump));
    s << n;
    return s.str();
}

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

void check_equals(auto left, auto right, std::string_view desc, const std::source_location loc = std::source_location::current())
{
    test_obj.test_cnt++;
    if (left != right)
    {
        std::cerr << "FAILED, " << loc.function_name() << ", line: " << loc.line() << ", " << left << " != " << right << ", " << desc << std::endl;
        test_obj.fail_cnt++;
    }
}

void test_trim()
{
    string_view val("marry.had.a.little.lamb");
    // ltrim char
    check_equals(val.ltrim(' '), "marry.had.a.little.lamb"sv, "ltrim not found"sv);
    check_equals(val.ltrim('m'), "arry.had.a.little.lamb"sv, "ltrim found"sv);
    check_equals(val.ltrim('a'), "rry.had.a.little.lamb"sv, "ltrim found"sv);
    check_equals(val.ltrim('r'), "y.had.a.little.lamb"sv, "ltrim found double"sv);
    // ltrim sv
    val = "marry.had.a.little.lamb"sv;
    check_equals(val.ltrim("1234"sv), "marry.had.a.little.lamb"sv, "ltrim sv not found"sv);
    check_equals(val.ltrim("rry"sv), "marry.had.a.little.lamb"sv, "ltrim sv not found"sv);
    check_equals(val.ltrim("marry."sv), "had.a.little.lamb"sv, "ltrim sv found"sv);

    val = "marry.had.a.little.lamb"sv;
    // rtrim char
    check_equals(val.rtrim(' '), "marry.had.a.little.lamb"sv, "rtrim not found"sv);
    check_equals(val.rtrim('b'), "marry.had.a.little.lam"sv, "rtrim found"sv);
    val = "marry.hadttttt"sv;
    check_equals(val.rtrim('t'), "marry.had"sv, "rtrim found multiple"sv);

    val = "marry.had.a.little.lamb"sv;
    // rtrim sv
    check_equals(val.rtrim(" !:="sv), "marry.had.a.little.lamb"sv, "rtrim sv not found"sv);
    check_equals(val.rtrim("lam"sv), "marry.had.a.little.lamb"sv, "rtrim sv not found"sv);
    check_equals(val.rtrim("lamb."sv), "marry.had.a.little"sv, "rtrim sv found"sv);
    check_equals(val.rtrim("leti"sv), "marry.had.a."sv, "rtrim sv found"sv);
}

void test_before()
{
    string_view val("marry.had.a.little.lamb");
    // char
    check_equals(val.before('.'), "marry"sv, "before found"sv);
    check_equals(val.before('y'), "marr"sv, "before found"sv);
    check_equals(val.before(':'), val, "before not found"sv);

    // string_view
    check_equals(val.before("had"sv), "marry."sv, "before sv found"sv);
    check_equals(val.before("bob"sv), val, "before sv not found"sv);
}

void test_after()
{
    string_view val("marry.had.a.little.lamb");
    // char
    check_equals(val.after('.'), "had.a.little.lamb"sv, "after found"sv);
    check_equals(val.after(':'), ""sv, "after not found"sv);

    // string_view
    check_equals(val.after("little"sv), ".lamb"sv, "after sv found"sv);
}

void test_split()
{
    string_view val("marry.had.a.little.lamb");
    string_view sub = val.split('.');
    check_equals(val, "had.a.little.lamb"sv, "split remainder"sv);
    check_equals(sub, "marry"sv, "split sub"sv);
    check_equals(val.split('.'), "had"sv, "split found"sv); 
    check_equals(val.split('.'), "a"sv, "split found"sv); 
    check_equals(val.split('.'), "little"sv, "split found"sv); 
    check_equals(val.split('.'), "lamb"sv, "split found last"sv); 
    check_equals(val.split('.'), ""sv, "split not found empty"sv); 

    val = "marry.had.a.little.lamb"sv;
    std::vector<string_view> subs;
    size_t cnt = val.split('.', subs);
    check_equals(cnt, 5, "split vec cnt"sv);
    auto iter = subs.begin();
    check_equals(*iter++, "marry"sv, "split vec 0"sv);
    check_equals(*iter++, "had"sv, "split vec 1"sv);
    check_equals(*iter++, "a"sv, "split vec 2"sv);
    check_equals(*iter++, "little"sv, "split vec 3"sv);
    check_equals(*iter++, "lamb"sv, "split vec 4"sv);
}

void test_rsplit()
{
    string_view val("marry.had.a.little.lamb");
    string_view sub = val.rsplit('.');
    check_equals(val, "lamb"sv, "split remainder"sv);
    check_equals(sub, "marry.had.a.little"sv, "split sub"sv);
}

template<typename NUMB> void test_aton_type(NUMB min_val, NUMB max_val, NUMB incr, string_view desc)
{
    clock_t start = clock();
    NUMB numb = min_val;
    uint64_t cnt = 0;
    std::array<char, 50> str;

    for (NUMB i = min_val; i < max_val; i += incr)
    {
        auto [ptr, ec] = std::to_chars(str.data(), str.data() + str.size(), i);
        string_view sv(str.data(), ptr - str.data());
        sv.aton(numb);
        if (numb != i)
        {
            check_equals(numb, i, desc);
            std::cerr << "i: " << i << ", str: " << sv << ", numb: " << numb << std::endl; 
            break;
        }
        cnt++;
    }
    clock_t end = clock();
    clock_t clock_ticks = end - start;
    uint64_t ns = clock_ticks * (1000000000 / CLOCKS_PER_SEC);
    
    std::cerr << "tested " << desc 
              << ", " << commas(min_val) << " - " << commas(max_val - 1) 
              << ", inc: " << incr 
              << ", cnt: " << commas(cnt) 
              << ", ns: " << commas(ns) 
              << ", ns each : " << commas(ns / cnt) << std::endl;
}

void test_aton(uint64_t extra_min, uint64_t extra_max, uint64_t extra_incr)
{
    string_view val("1234");
    uint32_t n;
    check_equals(val.aton(n), true, "aton succeeded"sv); 
    check_equals(n, 1234, "aton correct value"sv);

    test_aton_type<uint32_t>(0, 10000, 1, "aton(uint32_t)"sv);
    test_aton_type<uint64_t>(0, 100000000, 1, "aton(uint64_t)"sv);
    if (extra_max)
    {
        test_aton_type<uint64_t>(extra_min, extra_max, extra_incr, "aton(uint64_t)"sv);
    }
}

int main (int argc, char **argv)
{
    uint64_t aton_extra_min = 0;
    uint64_t aton_extra_max = 0;
    uint64_t aton_extra_incr = 0;

    for (int i = 1; i < argc; i++)
    {
        string_view val(argv[i]);
        string_view arg = val.split('=');
        if (arg == "--aton-extra-min"sv)
            val.aton(aton_extra_min);
        else if (arg == "--aton-extra-max"sv)
            val.aton(aton_extra_max);
        else if (arg == "--aton-extra-incr"sv)
            val.aton(aton_extra_incr);
    }
    test_trim();
    test_before();
    test_after();
    test_split();
    test_rsplit();
    test_aton(aton_extra_min, aton_extra_max, aton_extra_incr);
}
