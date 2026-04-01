#include "unit_tester.h"

TEST_CASE(test_addition) {
    ASSERT_EQ(1 + 1, 2);
}

TEST_CASE(test_string_equal) {
    ASSERT_STR_EQ("hello", "hello");
}

TEST_CASE(test_failure_example) {
    int x = 5;
    ASSERT(x == 5);
    ASSERT_EQ(x * 2, 10);
}

int main(void) {
    REGISTER_TEST(test_addition);
    REGISTER_TEST(test_string_equal);
    REGISTER_TEST(test_failure_example);
    return RUN_ALL_TESTS();
}
