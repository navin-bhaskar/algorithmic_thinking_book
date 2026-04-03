#include <stdlib.h>
#include <stdio.h>

#ifdef TESTS
#include "../../common/unit_tester.h"
#endif

#define SNOW_FLAKES_SIZE 6

// payload on the hashmap -> a hashmap of snowflakes 
typedef struct SnowflakeNode { 
    int snowflake[SNOW_FLAKES_SIZE];
    struct SnowflakeNode* next;
} SnowflakeNode;

/**
 * @brief This is solution to DMOJ problem cco07p2 from [[https://dmoj.ca/problem/cco07p2]]
 * 
 * The solution uses HashMap to find matching snowflake.
 * First we need to implement a bunch of functions to chheck if two snowflakes are identical. 
 * We can check if two snowflakes are identical by checking all possible offsets and comparing the snowflakes 
 * from right to left and left to right.
 * Basically we need a function that checks if two snowflakes are identical from both clockwise and anti-clockwise direction.
 * To achive that, we can implement two functions, one that checks if two snowflakes are identical from right to left and 
 * another that checks if two snowflakes are identical from left to right. Combining these two functions, we can check if 
 * two snowflakes are identical by checking all possible offsets.
 */


/**
 * @brief Checks if two snowflakes are identical at given offset while comparing from right to left.
 * 
 * @param snow1 First snowflake represented as an array of 6 integers.
 * @param snow2 Second snowflake represented as an array of 6 integers.
 * @param start_offset The offset to compare the snowflakes from.
 * 
 * @return 1 if the snowflakes are identical at the given offset, 0 otherwise.
 */

int are_identical_right(int snow1[], int snow2[], int start_offset) {
     for(int i=0; i<SNOW_FLAKES_SIZE; i++) {
        if(snow1[i] != snow2[(start_offset + i) % SNOW_FLAKES_SIZE]) {
            return 0;
        }
    }
    return 1;
 }


/**
  * @brief Checks if two snowflakes are identical at given offset while comparing from left to right.
  *
  * @param snow1 First snowflake represented as an array of 6 integers.
  * @param snow2 Second snowflake represented as an array of 6 integers.
  * @param start_offset The offset to compare the snowflakes from.
  * 
  * @return 1 if the snowflakes are identical at the given offset, 0 otherwise.
  */
int are_identical_left(int snow1[], int snow2[], int start_offset) {
    for (int i=0; i<SNOW_FLAKES_SIZE; i++) {
        int idx = start_offset - i;
        if (idx < 0) {
            idx += SNOW_FLAKES_SIZE;
        }
        if (snow1[i] != snow2[idx]) {
            return 0;
        }
    }
    return 1;
}

/**
 * @brief Checks if two snowflakes are identical by checking all possible offsets.
 * 
 * @param snow1 First snowflake represented as an array of 6 integers.
 * @param snow2 Second snowflake represented as an array of 6 integers.
 * 
 * @return 1 if the snowflakes are identical, 0 otherwise.
 *
 */
int snowflakes_are_identical(int snow1[], int snow2[]) {
    for (int offset=0; offset<SNOW_FLAKES_SIZE; offset++) {
        if (are_identical_right(snow1, snow2, offset) || are_identical_left(snow1, snow2, offset)) {
            return 1;
        }
    }
    return 0;
}

/**
 * With basics of checking if two snowflakes are identical done, we can now implement the hashmap.
 * So we need first the hash function will be a simple hash function.
*/


/**
 * @brief Simple hash function for snowflakes.
 * 
 * @param snowflake The snowflake represented as an array of 6 integers.
 * @param hash_map_size The size of the hash map.
 * 
 * @return The hash value for the given snowflake.
 */
int compute_hash(int snowflake[], int hash_map_size) {
    int hash_value = 0;
    for(int i=0; i<SNOW_FLAKES_SIZE; i++) {
        hash_value = (hash_value + snowflake[i]) % hash_map_size;
    }
    return hash_value;
}

/**
 * @brief Adds a snowflake to the hash map.
 * 
 * @param hash_map The hash map to add the snowflake to.
 * @param snow_flake The snowflake to add.
 * @param hash_map_size The size of the hash map.
 * 
 */
void add_snowflake(SnowflakeNode* hash_map[], int snow_flake[], int hash_map_size) {
    int hash_value = compute_hash(snow_flake, hash_map_size);
    SnowflakeNode* current_snow_flake_node = hash_map[hash_value];

    SnowflakeNode* new_snow_flake_node = (SnowflakeNode*)malloc(sizeof(SnowflakeNode));
    if(new_snow_flake_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // copy the snowflake to the new node
    for(int i=0; i<SNOW_FLAKES_SIZE; i++) {
        new_snow_flake_node->snowflake[i] = snow_flake[i];
    }

    new_snow_flake_node->next = current_snow_flake_node;
    hash_map[hash_value] = new_snow_flake_node;
}

/**
 * @brief Checks if a snowflake is already in the hash map.
 * 
 * @param hash_map The hash map to check.
 * @param snow_flake The snowflake to check for.
 * @param hash_map_size The size of the hash map.
 * 
 * @return 1 if the snowflake is already in the hash map, 0 otherwise.
 */
int is_snowflake_in_hash_map(SnowflakeNode* hash_map[], int snow_flake[], int hash_map_size) {
    int hash_value = compute_hash(snow_flake, hash_map_size);
    if (hash_map[hash_value] == NULL) {
        return 0;
    }

    SnowflakeNode* current_snow_flake_node = hash_map[hash_value];
    while (current_snow_flake_node != NULL) { 
        if (snowflakes_are_identical(current_snow_flake_node->snowflake, snow_flake)) {
            return 1;
        }
        current_snow_flake_node = current_snow_flake_node->next;
    }

    return 0;
}

/**
 * @brief Checks if a snowflake is already in the hash map and adds it if it is not.
 * 
 * @param hash_map The hash map to check and add to.
 * @param snow_flake The snowflake to check and add.    
 * @param hash_map_size The size of the hash map.
 */
int check_and_add_snowflake(SnowflakeNode* hash_map[], int snow_flake[], int hash_map_size) {
    if (is_snowflake_in_hash_map(hash_map, snow_flake, hash_map_size)) {
        return 1;
    }
    add_snowflake(hash_map, snow_flake, hash_map_size);
    return 0;
}

#ifdef TESTS
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

    int snow3[] = {1, 2, 3, 4, 5, 6};
    int snow4[] = {4, 3, 2, 1, 6, 5};
    ASSERT(snowflakes_are_identical(snow3, snow4));
}

TEST_CASE(test_snow_flakes_are_not_identical) {
    int snow1[] = {1, 2, 3, 4, 5, 6};
    int snow2[] = {7, 8, 9, 10, 11, 12};

    ASSERT(!snowflakes_are_identical(snow1, snow2));
}

TEST_CASE(test_compute_hash_returns_same_hash_for_identical_snowflakes) {
    int snow1[] = {1, 2, 3, 4, 5, 6};
    int snow2[] = {4, 5, 6, 1, 2, 3};

    int hash_map_size = 10;
    ASSERT(compute_hash(snow1, hash_map_size) == compute_hash(snow2, hash_map_size));
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
        {1, 2, 3, 4, 5, 6},
        {4, 3, 2, 1, 6, 5}
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
    REGISTER_TEST(test_compute_hash_returns_same_hash_for_identical_snowflakes);

    return RUN_ALL_TESTS();
}
#else
int main(void) {
    SnowflakeNode* hash_map[100000] = {0}; // initialize hash map with NULL
    int total_snow_flakes;
    int snow_flake[SNOW_FLAKES_SIZE];
    scanf("%d", &total_snow_flakes);

    for (int i=0; i<total_snow_flakes; i++) {
        for (int j=0; j<SNOW_FLAKES_SIZE; j++) {
            scanf("%d", &snow_flake[j]);
        }
        if (check_and_add_snowflake(hash_map, snow_flake, 100000)) {
            printf("Twin snowflakes found.\n");
            return 0;
        }
    }
    printf("No two snowflakes are alike.\n");
    return 0;
}
#endif