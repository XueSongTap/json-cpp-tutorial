#include <iostream>
#include "leptjson.h"

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

#define EXPECT_EQ_BASE(equality, expect, actual)                                         \
    do {                                                                                 \
        test_count++;                                                                    \
        if (equality)                                                                    \
            test_pass++;                                                                 \
        else {                                                                           \
            std::cerr << __FILE__ << ":" << __LINE__ << ": expect: " << expect           \
                      << " actual: " << actual << std::endl;                             \
            main_ret = 1;                                                                \
        }                                                                                \
    } while (0)

#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual)

static void test_parse_null() {
    lept::LeptValue v;
    v.setType(lept::LeptType::False);
    try {
        lept::parse(v, "null");
        EXPECT_EQ_INT(static_cast<int>(lept::LeptType::Null), static_cast<int>(v.getType()));
    } catch (const lept::ParseException& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        main_ret = 1;
    }
}

static void test_parse_expect_value() {
    lept::LeptValue v;

    v.setType(lept::LeptType::False);
    try {
        lept::parse(v, "");
        main_ret = 1; // Should not reach here
    } catch (const lept::ParseException& e) {
        
        EXPECT_EQ_INT(static_cast<int>(lept::ErrorCode::ExpectValue), static_cast<int>(e.code()));
        EXPECT_EQ_INT(static_cast<int>(lept::LeptType::Null), static_cast<int>(v.getType()));
    }

    v.setType(lept::LeptType::False);
    try {
        lept::parse(v, " ");
        main_ret = 1; // Should not reach here
    } catch (const lept::ParseException& e) {
        EXPECT_EQ_INT(static_cast<int>(lept::ErrorCode::ExpectValue), static_cast<int>(e.code()));
        EXPECT_EQ_INT(static_cast<int>(lept::LeptType::Null), static_cast<int>(v.getType()));
    }
}

static void test_parse_invalid_value() {
    lept::LeptValue v;

    v.setType(lept::LeptType::False);
    try {
        lept::parse(v, "nul");
        main_ret = 1; // Should not reach here
    } catch (const lept::ParseException& e) {
        EXPECT_EQ_INT(static_cast<int>(lept::ErrorCode::InvalidValue), static_cast<int>(e.code()));
        EXPECT_EQ_INT(static_cast<int>(lept::LeptType::Null), static_cast<int>(v.getType()));
    }

    v.setType(lept::LeptType::False);
    try {
        lept::parse(v, "?");
        main_ret = 1; // Should not reach here
    } catch (const lept::ParseException& e) {
        EXPECT_EQ_INT(static_cast<int>(lept::ErrorCode::InvalidValue), static_cast<int>(e.code()));
        EXPECT_EQ_INT(static_cast<int>(lept::LeptType::Null), static_cast<int>(v.getType()));
    }
}

static void test_parse_root_not_singular() {
    lept::LeptValue v;
    v.setType(lept::LeptType::False);
    try {
        lept::parse(v, "null x");
        main_ret = 1; // Should not reach here
    } catch (const lept::ParseException& e) {
        EXPECT_EQ_INT(static_cast<int>(lept::ErrorCode::RootNotSingular), static_cast<int>(e.code()));
        EXPECT_EQ_INT(static_cast<int>(lept::LeptType::Null), static_cast<int>(v.getType()));
    }
}

static void test_parse() {
    test_parse_null();
    test_parse_expect_value();
    test_parse_invalid_value();
    test_parse_root_not_singular();
}

int main() {
    test_parse();
    std::cout << test_pass << "/" << test_count << " (" << (test_pass * 100.0 / test_count) << "%) passed\n";
    return main_ret;
}