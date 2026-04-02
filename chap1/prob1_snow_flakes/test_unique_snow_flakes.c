#include <unit_tester.h>

#include "unique_snow_flakes.h"

TEST_CASE(test_identical_snowflakes_at_given_offset) {
    int snow1[] = {1, 2, 3, 4, 5, 6};
    int snow2[] = {4, 5, 6, 1, 2, 3};

    ASSERT(are_identical_right(snow1, snow2, 3));
}

TEST_CASE(test_non_identical_snowflakes_at_given_offset) {
    int snow1[] = {1, 2, 3, 4, 5, 6};
    int snow2[] = {4, 5, 6, 1, 2, 4};

    ASSERT(!are_identical_right(snow1, snow2, 3));
}

TEST_CASE(test_snow_flakes_are_identical_wrong_offset_given) {
    int snow1[] = {1, 2, 3, 4, 5, 6};
    int snow2[] = {4, 5, 6, 1, 2, 3};

    ASSERT(!are_identical_right(snow1, snow2, 2));
}

TEST_CASE(test_snow_flakes_are_identical_left_at_given_offset) {
    int snow1[] = {1, 2, 3, 4, 5, 6};
    int snow2[] = {2, 1, 6, 5, 4, 3};

    ASSERT(are_identical_left(snow1, snow2, 1));
}

TEST_CASE(test_snow_flakes_are_not_identical_left_at_given_offset) {
    int snow1[] = {1, 2, 3, 4, 5, 6};
    int snow2[] = {2, 1, 6, 5, 4, 4};

    ASSERT(!are_identical_left(snow1, snow2, 1));
}

TEST_CASE(test_snow_flakes_are_identical_left_wrong_offset_given) {
    int snow1[] = {1, 2, 3, 4, 5, 6};
    int snow2[] = {2, 1, 6, 5, 4, 3};

    ASSERT(!are_identical_left(snow1, snow2, 2));
}

TEST_CASE(test_snow_flakes_are_identical_simple_case){
    int snow1[] = {1, 2, 3, 4, 5, 6};
    int snow2[] = {1, 2, 3, 4, 5, 6};

    ASSERT(snowflakes_are_identical(snow1, snow2));
}

TEST_CASE(test_snow_flakes_are_identical_clockwise) {
    int snow1[] = {1, 2, 3, 4, 5, 6};
    int snow2[] = {4, 5, 6, 1, 2, 3};

    ASSERT(snowflakes_are_identical(snow1, snow2));
}

TEST_CASE(test_snow_flakes_are_identical_anti_clockwise) {
    int snow1[] = {1, 2, 3, 4, 5, 6};
    int snow2[] = {2, 1, 6, 5, 4, 3};

    ASSERT(snowflakes_are_identical(snow1, snow2));
}

TEST_CASE(test_snow_flakes_are_not_identical) {
    int snow1[] = {1, 2, 3, 4, 5, 6};
    int snow2[] = {7, 8, 9, 10, 11, 12};

    ASSERT(!snowflakes_are_identical(snow1, snow2));
}

TEST_CASE(test_add_snowflake_and_check_if_it_is_in_hash_map) {
    SnowflakeNode* hash_map[10] = {0}; // initialize hash map with NULL
    int snow_flake[] = {1, 2, 3, 4, 5, 6};

    add_snowflake(hash_map, snow_flake, 10);
    ASSERT(is_snowflake_in_hash_map(hash_map, snow_flake, 10));
}

TEST_CASE(test_check_if_non_existent_snowflake_is_in_hash_map) {
    SnowflakeNode* hash_map[10] = {0}; // initialize hash map with NULL
    int snow_flake[] = {1, 2, 3, 4, 5, 6};

    ASSERT(!is_snowflake_in_hash_map(hash_map, snow_flake, 10));
}

TEST_CASE(test_add_multiple_snowflakes_and_check_if_they_are_in_hash_map) {
    SnowflakeNode* hash_map[10] = {0}; // initialize hash map with NULL
    int snow_flake1[] = {1, 2, 3, 4, 5, 6};
    int snow_flake2[] = {7, 8, 9, 10, 11, 12};

    add_snowflake(hash_map, snow_flake1, 10);
    add_snowflake(hash_map, snow_flake2, 10);

    ASSERT(is_snowflake_in_hash_map(hash_map, snow_flake1, 10));
    ASSERT(is_snowflake_in_hash_map(hash_map, snow_flake2, 10));
}

TEST_CASE(test_check_and_add_snowflake) {
    // This is one of the examples given in the problem statement, 
    // we can use this example to test our implementation of the hashmap and the function that checks 
    // if a snowflake is in the hashmap and adds it if it is not.
    // To make this work on Dmoj, you will have to adapt the code to read from standard input and write to standard output, 
    // but for testing purposes, we can just hardcode the snowflakes and the hash map size.
    SnowflakeNode* hash_map[10] = {0}; // initialize hash map with NULL

    int all_snow_flakes[][SNOW_FLAKES_SIZE] = {
        {3, 4, 5, 6, 1, 2},
        {2, 3, 4, 5, 6, 7},
        {4, 5, 6, 7, 8, 9},
        {1, 2, 3, 4, 5, 6},
    };

    for (int i = 0; i < 4; i++) {
        if (check_and_add_snowflake(hash_map, all_snow_flakes[i], 10)) {
            printf("Snowflake %d is a duplicate.\n", i + 1);
        } else {
            printf("Snowflake %d is unique and added to the hash map.\n", i + 1);
        }
    }
}

int main(void) {
    REGISTER_TEST(test_identical_snowflakes_at_given_offset);
    REGISTER_TEST(test_non_identical_snowflakes_at_given_offset);
    REGISTER_TEST(test_snow_flakes_are_identical_wrong_offset_given);
    REGISTER_TEST(test_snow_flakes_are_identical_left_at_given_offset);
    REGISTER_TEST(test_snow_flakes_are_not_identical_left_at_given_offset);
    REGISTER_TEST(test_snow_flakes_are_identical_left_wrong_offset_given);
    REGISTER_TEST(test_snow_flakes_are_identical_simple_case);
    REGISTER_TEST(test_snow_flakes_are_identical_clockwise);
    REGISTER_TEST(test_snow_flakes_are_identical_anti_clockwise);
    REGISTER_TEST(test_snow_flakes_are_not_identical);
    REGISTER_TEST(test_add_snowflake_and_check_if_it_is_in_hash_map);
    REGISTER_TEST(test_check_if_non_existent_snowflake_is_in_hash_map);
    REGISTER_TEST(test_add_multiple_snowflakes_and_check_if_they_are_in_hash_map);
    REGISTER_TEST(test_check_if_non_existent_snowflake_is_in_hash_map);
    REGISTER_TEST(test_check_and_add_snowflake);

    return RUN_ALL_TESTS();
}