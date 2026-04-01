#ifndef _UNIQUE_SNOW_FLAKES_H_
#define _UNIQUE_SNOW_FLAKES_H_

#include <utils.h>

#define SNOW_FLAKES_SIZE 6

// payload on the hashmap -> a hashmap of snowflakes 
typedef struct SnowflakeNode { 
    int snowflake[SNOW_FLAKES_SIZE];
    struct SnowflakeNode* next;
} SnowflakeNode;



bool are_identical_right(int snow1[], int snow2[], int start_offset);
bool are_identical_left(int snow1[], int snow2[], int start_offset);
bool snowflakes_are_identical(int snow1[], int snow2[]);
void add_snowflake(SnowflakeNode* hash_map[], int snow_flake[], int hash_map_size);
bool is_snowflake_in_hash_map(SnowflakeNode* hash_map[], int snow_flake[], int hash_map_size);
bool check_and_add_snowflake(SnowflakeNode* hash_map[], int snow_flake[], int hash_map_size);


#endif