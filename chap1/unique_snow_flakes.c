#include <stdlib.h>
#include <stdio.h>

/**
 * @brief This is solution to DMOJ problem cco07p2 from [[https://dmoj.ca/problem/cco07p2]]
 * 
 * The solution uses HashMap to find matching snowflake.
 */

#define bool char
#define false 0
#define true 1

#define HASH_SIZE 1000


bool is_identical_right(int snow1[], int snow2[], int start) {
    for(int offset=0; offset <6; offset++) {
        if (snow1[offset] != snow2[(offset+ start)%6]){
            return false;
        }
    }
    return true;
}

bool is_identical_left(int snow1[], int snow2[], int start) {
    for(int offset=0; offset < 6; offset++) {
        int snow2_index = start - offset;
        if (snow2_index < 0) {
            snow2_index += 6;
        }
        if (snow1[offset] != snow2[snow2_index]) {
            return false;
        }
    }
    return true;
}

bool are_identical(int snow1[], int snow2[]) {
    int start;

    for (start=0; start<6; start++) {
        if(is_identical_right(snow1, snow2, start)){
            return true;
        }

        if(is_identical_left(snow1, snow2, start)){
            return true;
        }
    }

    return false;
}


int compute_hash(int snowflake[], int size) {
    int hash=0;
    for (int i=0; i<6; i++) {
        hash += snowflake[i];
    }
    return hash % size;
}

typedef struct PayLoad {
    int data[6];
    struct PayLoad *next;
} PayLoad;


int contains_entry(PayLoad* hash_map[], int snowflake[]) {
    int hash = compute_hash(snowflake, HASH_SIZE);
    if (hash_map[hash] != NULL) {
        PayLoad *cur_flake = hash_map[hash];
        while (cur_flake) {
            if (are_identical(snowflake, cur_flake->data)) {
                return true;
            }
            cur_flake = cur_flake->next;
        }
    }
   return false;
}

void add_entry(PayLoad* hash_map[], PayLoad *payload) {
    int *snow_flake = payload->data;
    int hash = compute_hash(snow_flake, HASH_SIZE);
    if(payload == NULL) {
        return;
    }
    payload->next = hash_map[hash];
    hash_map[hash] = payload;
}

int main() {
   int total_snowflakes = 0;
   static PayLoad* hash_map[HASH_SIZE]; // Init to all NULLs
   scanf("%d", &total_snowflakes);

   for(int i=0; i<total_snowflakes; i++) {
        PayLoad *cur = malloc(sizeof(struct PayLoad));
        
        if (cur == NULL) {
            printf("Error allocating memory");
            exit(1);
        }

        cur->next = NULL;
        for(int j=0; j<6; j++) {
            scanf("%d", &cur->data[j]);
        }
        
        if (contains_entry(hash_map, cur->data)) {
            printf("Twin snowflakes found.\n");
            return 0;
        }
        add_entry(hash_map, cur);
    }
    printf("No two snowflakes are alike.\n");
    
}
