#include "../lib/CArrayUtil.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

bool int_equals(const void* a, const void* b) {
    return *(int*)a == *(int*)b;
}

size_t int_hash(const void* data) {
    return *(int*)data;
}

int compare_ints(const void* a, const void* b) {
    return *(int*)a - *(int*)b;
}

// Function to run all tests
void run_map_tests() {
    printf("Running MAP tests...\n");

    // Test MAP_NEXT_KEY and basic map operations
    {
        MAP(int, char*) map = NULL;
        HASH_MAP_REHASH(map, linear_probing, int_equals, int_hash, 16, 10, 0.75);

        MAP_AT(map, (int){1}) = "One";
        MAP_AT(map, (int){2}) = "Two";
        MAP_AT(map, (int){3}) = "Three";

        const void* key = NULL;
        int found_keys[3];
        size_t found_keys_count = 0;

        while ((key = MAP_NEXT_KEY(map, key)) != NULL) {
            found_keys[found_keys_count++] = *(int*)key;
        }

        assert(found_keys_count == 3);
        qsort(found_keys, 3, sizeof(int), compare_ints);
        assert(found_keys[0] == 1);
        assert(found_keys[1] == 2);
        assert(found_keys[2] == 3);

        MAP_CLEAR(map);
    }

    // Test MAP_FIND and MAP_AT
    {
        MAP(int, char*) map = NULL;
        HASH_MAP_REHASH(map, linear_probing, int_equals, int_hash, 16, 10, 0.75);

        // Insert elements
        MAP_AT(map, (int){5}) = "Five";
        MAP_AT(map, (int){10}) = "Ten";

        char** value = MAP_FIND(map, (int){5});
        assert(value != NULL && strcmp(*value, "Five") == 0);

        value = MAP_FIND(map, (int){10});
        assert(value != NULL && strcmp(*value, "Ten") == 0);

        value = MAP_FIND(map, (int){3});
        assert(value == NULL);

        MAP_FREE(map);
    }

    // Test MAP_REMOVE
    {
        MAP(int, char*) map = NULL;
        HASH_MAP_REHASH(map, linear_probing, int_equals, int_hash, 16, 10, 0.75);

        MAP_AT(map, (int){1}) = "One";
        MAP_AT(map, (int){2}) = "Two";
        MAP_AT(map, (int){3}) = "Three";

        bool removed = MAP_REMOVE(map, (int){2});
        assert(removed);

        char** value = MAP_FIND(map, (int){2});
        assert(value == NULL);

        MAP_FREE(map);
    }

    // Test MAP_ENSURE_EXTRA_CAPACITY and MAP_TRY_ENSURE_EXTRA_CAPACITY
    {
        MAP(int, char*) map = NULL;
        HASH_MAP_REHASH(map, linear_probing, int_equals, int_hash, 16, 10, 0.75);

        MAP_ENSURE_EXTRA_CAPACITY(map, 10);
        assert(MAP_TRY_ENSURE_EXTRA_CAPACITY(map, 100) == true);

        MAP_FREE(map);
    }

    // Test HASH_MAP_REHASH and HASH_MAP_TRY_REHASH
    {
        MAP(int, char*) map = NULL;

        HASH_MAP_REHASH(map, linear_probing, int_equals, int_hash, 32, 10, 0.75);
        bool success = HASH_MAP_TRY_REHASH(map, linear_probing, int_equals, int_hash, 64, 10, 0.75);
        assert(success);

        MAP_FREE(map);
    }

    // Test MAP_CLEAR
    {
        MAP(int, char*) map = NULL;
        HASH_MAP_REHASH(map, linear_probing, int_equals, int_hash, 16, 10, 0.75);

        MAP_AT(map, (int){1}) = strdup("One");
        MAP_AT(map, (int){2}) = strdup("Two");

        MAP_CLEAR(map);

        assert(MAP_FIND(map, (int){1}) == NULL);
        assert(MAP_FIND(map, (int){2}) == NULL);

        MAP_FREE(map);
    }

    // Test MAP_FREE
    {
        MAP(int, char*) map = NULL;
        HASH_MAP_REHASH(map, linear_probing, int_equals, int_hash, 16, 10, 0.75);

        MAP_AT(map, (int){1}) = strdup("One");
        MAP_AT(map, (int){2}) = strdup("Two");

        MAP_FREE(map);
        assert(map == NULL);
    }
}

int main() {
    run_map_tests();
    printf("All MAP tests passed!\n");
    return 0;
}