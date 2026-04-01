#include <stdlib.h>
#include <stdio.h>

#include <utils.h>

#include "unique_snow_flakes.h"

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
 * @return true if the snowflakes are identical at the given offset, false otherwise.
 */

bool are_identical_right(int snow1[], int snow2[], int start_offset) {
     for(int i=0; i<SNOW_FLAKES_SIZE; i++) {
        if(snow1[i] != snow2[(start_offset + i) % SNOW_FLAKES_SIZE]) {
            return false;
        }
    }
    return true;
 }


/**
  * @brief Checks if two snowflakes are identical at given offset while comparing from left to right.
  *
  * @param snow1 First snowflake represented as an array of 6 integers.
  * @param snow2 Second snowflake represented as an array of 6 integers.
  * @param start_offset The offset to compare the snowflakes from.
  * 
  * @return true if the snowflakes are identical at the given offset, false otherwise.
  */
bool are_identical_left(int snow1[], int snow2[], int start_offset) {
    for (int i=0; i<SNOW_FLAKES_SIZE; i++) {
        int idx = start_offset - i;
        if (idx < 0) {
            idx += SNOW_FLAKES_SIZE;
        }
        if (snow1[i] != snow2[idx]) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Checks if two snowflakes are identical by checking all possible offsets.
 * 
 * @param snow1 First snowflake represented as an array of 6 integers.
 * @param snow2 Second snowflake represented as an array of 6 integers.
 * 
 * @return true if the snowflakes are identical, false otherwise.
 *
 */
bool snowflakes_are_identical(int snow1[], int snow2[]) {
    for (int offset=0; offset<SNOW_FLAKES_SIZE; offset++) {
        if (are_identical_right(snow1, snow2, offset) || are_identical_left(snow1, snow2, offset)) {
            return true;
        }
    }
    return false;
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
        hash_value = (hash_value * 31 + snowflake[i]) % hash_map_size;
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
 * @return true if the snowflake is already in the hash map, false otherwise.
 */
bool is_snowflake_in_hash_map(SnowflakeNode* hash_map[], int snow_flake[], int hash_map_size) {
    int hash_value = compute_hash(snow_flake, hash_map_size);
    if (hash_map[hash_value] == NULL) {
        return false;
    }

    SnowflakeNode* current_snow_flake_node = hash_map[hash_value];
    while (current_snow_flake_node != NULL) { 
        if (snowflakes_are_identical(current_snow_flake_node->snowflake, snow_flake)) {
            return true;
        }
        current_snow_flake_node = current_snow_flake_node->next;
    }

    return false;
}

/**
 * @brief Checks if a snowflake is already in the hash map and adds it if it is not.
 * 
 * @param hash_map The hash map to check and add to.
 * @param snow_flake The snowflake to check and add.    
 * @param hash_map_size The size of the hash map.
 */
bool check_and_add_snowflake(SnowflakeNode* hash_map[], int snow_flake[], int hash_map_size) {
    if (is_snowflake_in_hash_map(hash_map, snow_flake, hash_map_size)) {
        return true;
    }
    add_snowflake(hash_map, snow_flake, hash_map_size);
    return false;
}

