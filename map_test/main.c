#include "../lib/CArrayUtil.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int compare_ints(const void* a, const void* b) {
    return *(int*)a - *(int*)b;
}

void run_map_tests() {
    printf("Running MAP tests...\n");

    // Test MAP_NEXT_KEY and basic map operations
    {
        MAP(int, char*) map = NULL;

        int keys[1] = { 3};

        char **at = NULL;
        MAP_AT(map, (int){1}, at);
        assert(at != NULL);
        *at = "One";
        MAP_AT(map, 2, at);
        assert(at != NULL);
        *at = "Two";
        MAP_AT(map, keys[0], at);
        assert(at != NULL);
        *at = "Three";

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
        MAP(int, int) map = NULL;

        int *at = NULL;
        MAP_AT(map, (int){5}, at);
        assert(at != NULL);
        *at = 50;
        MAP_AT(map, (int){10}, at);
        assert(at != NULL);
        *at = 100;

        at = NULL;
        MAP_FIND(map, (int){5}, at);
        assert(at != NULL && *at == 50);

        at = NULL;
        MAP_FIND(map, (int){10}, at);
        assert(at != NULL && *at == 100);

        at = NULL;
        MAP_FIND(map, (int){15}, at);
        assert(at == NULL);

        MAP_FREE(map);
    }

    // Test MAP_PUT
    {
        MAP(char, char *) map = NULL;

        MAP_PUT(map, (int){1}, "First");
        MAP_PUT(map, (int){2}, "Second");
        MAP_PUT(map, (int){3}, "Third");

        char **value_ptr;
        MAP_FIND(map, (int){1}, value_ptr);
        assert(value_ptr != NULL && strcmp(*value_ptr, "First") == 0);

        MAP_FIND(map, (int){2}, value_ptr);
        assert(value_ptr != NULL && strcmp(*value_ptr, "Second") == 0);

        MAP_FIND(map, (int){3}, value_ptr);
        assert(value_ptr != NULL && strcmp(*value_ptr, "Third") == 0);
    }


    // Test MAP_REMOVE
    {
        MAP(int, char*) map = NULL;

        MAP_PUT(map, (int){1}, "One");
        MAP_PUT(map, (int){2}, "Two");
        MAP_PUT(map, (int){3}, "Three");

        int two = 2;
        MAP_REMOVE(map, two);

        char **val_ptr;
        MAP_FIND(map, (int){1}, val_ptr);
        assert(val_ptr != NULL && strcmp(*val_ptr, "One") == 0);
        MAP_FIND(map, (int){2}, val_ptr);
        assert(val_ptr == NULL);
        MAP_FIND(map, (int){3}, val_ptr);
        assert(val_ptr != NULL && strcmp(*val_ptr, "Three") == 0);

        MAP_REMOVE(map, two);
        MAP_FIND(map, (int){1}, val_ptr);
        assert(val_ptr != NULL && strcmp(*val_ptr, "One") == 0);
        MAP_FIND(map, (int){2}, val_ptr);
        assert(val_ptr == NULL);
        MAP_FIND(map, (int){3}, val_ptr);
        assert(val_ptr != NULL && strcmp(*val_ptr, "Three") == 0);

        MAP_FREE(map);
    }


    // Test MAP_CLEAR
    {
        MAP(int, char*) map = NULL;
        int one = 1, two = 2;
        MAP_PUT(map, one, "One");
        MAP_PUT(map, two, "Two");

        MAP_CLEAR(map);

        char **val;
        MAP_FIND(map, one, val);
        assert(val == NULL);
        MAP_FIND(map, two, val);
        assert(val == NULL);

        MAP_FREE(map);
    }

    // Test MAP_FREE
    {
        MAP(int, char*) map = NULL;
        int one = 1, two = 2;
        MAP_PUT(map, one, "One");
        MAP_PUT(map, two, "Two");

        MAP_FREE(map);
        assert(map == NULL);
    }
}

int main() {
    run_map_tests();
    printf("All MAP tests passed!\n");
    return 0;
}