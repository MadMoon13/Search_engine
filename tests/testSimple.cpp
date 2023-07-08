#include <iostream>
#include "gtest/gtest.h"

void multiplyTest(int a, int b) {
    a *= 2;
    ASSERT_EQ(a, b);
}

TEST(simple_case_test, check_eq) {
    EXPECT_EQ(2*2, 4);
}
TEST(simple_case_test, check_str_eq) {
    EXPECT_STREQ("hello", "hello");
}
TEST(simple_case_test, check_function) {
    multiplyTest(2, 4);
}