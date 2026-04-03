#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef TESTS
#include "../../common/unit_tester.h"
#endif


#define MAX_PWD_SIZE 10U
#define HASH_SIZE 10000U
#define NUM_OF_BITS 20

typedef struct PasswordNode {
    char password[MAX_PWD_SIZE+1];
    int total_occurance;
    struct PasswordNode* next;
} PasswordNode;

int compute_hash(char password[], int hash_map_size) {
    unsigned long hash = 5381;
    int c;
    while ((c = *password++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash % hash_map_size;
}

PasswordNode *lookup_password(PasswordNode* hash_map[], char password[], int hash_map_size) {
    int hash_value = compute_hash(password, hash_map_size);
    PasswordNode *current_node = hash_map[hash_value];
    while (current_node != NULL){
        if (strncmp(current_node->password, password, MAX_PWD_SIZE) == 0){
            return current_node;
        }
        current_node = current_node->next;
    }
    return NULL;
}

void add_password(PasswordNode* hash_map[], char password[], int hash_map_size) {
    int hash_value = compute_hash(password, hash_map_size);
    PasswordNode* found_password_node = lookup_password(hash_map, password, hash_map_size);
    if (found_password_node != NULL) {
        found_password_node->total_occurance++;
    } else {
        PasswordNode *new_password_node = (PasswordNode*)malloc(sizeof(PasswordNode));
        if (new_password_node == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        strncpy(new_password_node->password, password, MAX_PWD_SIZE);
        new_password_node->password[MAX_PWD_SIZE] = '\0'; // ensure null termination
        new_password_node->total_occurance = 1;
        new_password_node->next = hash_map[hash_value];
        hash_map[hash_value] = new_password_node;
    }
    //printf("Added password: %s, total occurrence: %d\n", password, lookup_password(hash_map, password, hash_map_size)->total_occurance);
}

typedef struct PasswordSubStrNode {
    char sub_password[MAX_PWD_SIZE+1];
    struct PasswordSubStrNode* next;
} PasswordSubStrNode;

int is_password_sub_str_seen(PasswordSubStrNode* hash_map[], char sub_password[], int hash_map_size) {
   int hash_value = compute_hash(sub_password, hash_map_size);
   PasswordSubStrNode* current_node = hash_map[hash_value];
    while (current_node != NULL) {
         if (strncmp(current_node->sub_password, sub_password, MAX_PWD_SIZE) == 0) {
              return 1; // seen
         }
         current_node = current_node->next;
    } 
    return 0; // not seen
}

void add_password_sub_str(PasswordSubStrNode* hash_map[], char sub_password[], int hash_map_size) {
    int hash_value = compute_hash(sub_password, hash_map_size);
    PasswordSubStrNode* new_sub_str_node = (PasswordSubStrNode*)malloc(sizeof(PasswordSubStrNode));
    if (new_sub_str_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strncpy(new_sub_str_node->sub_password, sub_password, MAX_PWD_SIZE);
    new_sub_str_node->sub_password[MAX_PWD_SIZE] = '\0'; // ensure null termination
    new_sub_str_node->next = hash_map[hash_value];
    hash_map[hash_value] = new_sub_str_node;
}

#ifdef TESTS

TEST_CASE(test_compute_hash_returns_same_hash_for_passwords) {
    char password1[] = "password123";
    char password2[] = "password123";
    int hash_map_size = 10000;
    ASSERT(compute_hash(password1, hash_map_size) == compute_hash(password2, hash_map_size));
}

TEST_CASE(test_add_password_adds_a_password_to_hash_map) {
    PasswordNode* hash_map[HASH_SIZE] = {0}; // initialize hash map with NULL
    char password[] = "password123";
    add_password(hash_map, password, HASH_SIZE);
    PasswordNode* found_password_node = lookup_password(hash_map, password, HASH_SIZE);
    ASSERT(found_password_node != NULL);
    ASSERT(strncmp(found_password_node->password, password, MAX_PWD_SIZE) == 0);
    ASSERT(found_password_node->total_occurance == 1);
}

TEST_CASE(test_add_password_increments_occurrence_count) {
    PasswordNode* hash_map[HASH_SIZE] = {0}; // initialize hash map with NULL
    char password[] = "password123";
    add_password(hash_map, password, HASH_SIZE);
    add_password(hash_map, password, HASH_SIZE);
    PasswordNode* found_password_node = lookup_password(hash_map, password, HASH_SIZE);
    ASSERT(found_password_node != NULL);
    ASSERT(strncmp(found_password_node->password, password, MAX_PWD_SIZE) == 0);
    ASSERT(found_password_node->total_occurance == 2);
}

TEST_CASE(test_check_if_non_existent_password_is_in_hash_map) {
    PasswordNode* hash_map[HASH_SIZE] = {0}; // initialize hash map with NULL
    char password[] = "password123";
    PasswordNode* found_password_node = lookup_password(hash_map, password, HASH_SIZE);
    ASSERT(found_password_node == NULL);
}

TEST_CASE(test_in_lookup_password_different_passwords_with_same_hash) {
    PasswordNode* hash_map[HASH_SIZE] = {0}; // initialize hash map with NULL
    char password1[] = "password123";
    char password2[] = "passw0rd123"; // different password but might have same hash value as password1
    add_password(hash_map, password1, HASH_SIZE);
    add_password(hash_map, password2, HASH_SIZE);
    PasswordNode* found_password_node1 = lookup_password(hash_map, password1, HASH_SIZE);
    PasswordNode* found_password_node2 = lookup_password(hash_map, password2, HASH_SIZE);
    ASSERT(found_password_node1 != NULL);
    ASSERT(found_password_node2 != NULL);
    ASSERT(strncmp(found_password_node1->password, password1, MAX_PWD_SIZE) == 0);
    ASSERT(strncmp(found_password_node2->password, password2, MAX_PWD_SIZE) == 0);
}

TEST_CASE(test_add_password_sub_str_adds_a_sub_password_to_hash_map) {
    PasswordSubStrNode* hash_map[HASH_SIZE] = {0}; // initialize hash map with NULL
    char sub_password[] = "pass";
    add_password_sub_str(hash_map, sub_password, HASH_SIZE);
    ASSERT(is_password_sub_str_seen(hash_map, sub_password, HASH_SIZE));
}

int main(void) {
    REGISTER_TEST(test_compute_hash_returns_same_hash_for_passwords);
    REGISTER_TEST(test_add_password_adds_a_password_to_hash_map);
    REGISTER_TEST(test_add_password_increments_occurrence_count);
    REGISTER_TEST(test_check_if_non_existent_password_is_in_hash_map);
    REGISTER_TEST(test_in_lookup_password_different_passwords_with_same_hash);
    REGISTER_TEST(test_add_password_sub_str_adds_a_sub_password_to_hash_map);

    return RUN_ALL_TESTS();
}
#else

#define ADD 1
#define CHECK 2

int main(void) {
    static PasswordNode* hash_map[HASH_SIZE] = {0}; // initialize hash map with NULL
    char password[MAX_PWD_SIZE+1], sub_password[MAX_PWD_SIZE+1];
    int total_operations;
    int operation;
    PasswordSubStrNode *password_lookup_buffer[1000] = {0}; // initialize lookup buffer with NULL
    scanf("%d", &total_operations);

    for(int i=0; i<total_operations; i++) {
        scanf("%d %s", &operation, password);
        if (operation == ADD) {
            // note: This will cause memory leak on the allocated memeory for the password substrings in the lookup buffer hashmap, 
            // but since we are only interested in the final output and we are not running this code for a long time,
            // we can ignore this memory leak for now and not risk running into TLE in dmoj
            memset(password_lookup_buffer, 0, 1000 * sizeof(PasswordSubStrNode*)); // reset the lookup buffer for each new password
            
            // generate all possible sub passwords and add them to the hash map
            for(int i=0; i<strlen(password); i++) {
                for(int j=i; j<strlen(password); j++) {
                    int sub_password_length = j - i + 1;
                    strncpy(sub_password, &password[i], sub_password_length);
                    sub_password[sub_password_length] = '\0'; 
                    // we cannot add blindly a new substring password to hashmap 
                    // we need to first check if the substring already exists in hashmap
                    // Take this example: add -> 'aaa' check -> 'a'
                    // now 'a' as a substring will appear thrice in 'aaa' so each addition will increase the count of 'a' in the hash map by 3, 
                    // but we only want to increase it by 1 since we are only adding one password 'aaa' to the hash map, s
                    // so we need to check if the substring already exists in the hash map before adding it.


                    // is the substring in lookupbuffer?
                    if (!is_password_sub_str_seen(password_lookup_buffer, sub_password, 1000)) {
                        //printf("Adding substring: %s\n", sub_password);
                        add_password_sub_str(password_lookup_buffer, sub_password, 1000);
                        add_password(hash_map, sub_password, HASH_SIZE);
                    }
                    
                }
            }
        } else if (operation == CHECK) {
            PasswordNode* found_password_node = lookup_password(hash_map, password, HASH_SIZE);
            if (found_password_node != NULL) {
                printf("%d\n", found_password_node->total_occurance);
            } else {
                printf("0\n");
            }
        } else {
            fprintf(stderr, "Invalid operation: %d\n", operation);
            exit(EXIT_FAILURE);
        }
    }
    
    return 0;
}
#endif