#include "../lib/CArrayUtil.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

// Hash function for integers
size_t hash_int(const void* data) {
    return *((const int*)data);
}

// Equality check function for integers
bool equals_int(const void* a, const void* b) {
    return *((const int*)a) == *((const int*)b);
}

int main() {
    printf("Running MAP tests...\n");

    // Map declaration
    MAP(int, char*) map = NULL;

    // Test strategy parameters
    size_t initial_capacity = 5;
    uint32_t probing_limit = 10;
    float load_factor = 0.75;

    // === Testing HASH_MAP_REHASH ===
    HASH_MAP_REHASH(map, linear_probing, &equals_int, &hash_int, initial_capacity, probing_limit, load_factor);
    assert(map != NULL);                               // Ensure map is initialized
    assert(CARR_RING_BUFFER_T(map)->capacity >= 5);    // Ensure capacity is set correctly
    assert(CARR_RING_BUFFER_T(map)->size == 0);        // Ensure initial size is 0

    // === Testing HASH_MAP_TRY_REHASH ===
    bool success = HASH_MAP_TRY_REHASH(map, linear_probing, &equals_int, &hash_int, 10, probing_limit, load_factor);
    assert(success);                                   // Ensure TRY_REHASH succeeds
    assert(CARR_RING_BUFFER_T(map)->capacity >= 10);   // Check updated capacity

    // === Testing MAP_AT ===
    MAP_AT(map, (int){1}) = strdup("Value 1");         // Insert key=1, value="Value 1"
    MAP_AT(map, (int){2}) = strdup("Value 2");         // Insert key=2, value="Value 2"
    MAP_AT(map, (int){3}) = strdup("Value 3");         // Insert key=3, value="Value 3"

    assert(strcmp(*MAP_FIND(map, (int){1}), "Value 1") == 0);  // Find key=1
    assert(strcmp(*MAP_FIND(map, (int){2}), "Value 2") == 0);  // Find key=2
    assert(strcmp(*MAP_FIND(map, (int){3}), "Value 3") == 0);  // Find key=3
    assert(MAP_FIND(map, (int){4}) == NULL);                   // Find non-existent key

    // === Testing MAP_NEXT_KEY ===
    const int* key = NULL;
    size_t keys_found = 0;
    while ((key = MAP_NEXT_KEY(map, key)) != NULL) {
        keys_found++;
        assert((*key == 1 && strcmp(*MAP_FIND(map, *key), "Value 1") == 0) ||
               (*key == 2 && strcmp(*MAP_FIND(map, *key), "Value 2") == 0) ||
               (*key == 3 && strcmp(*MAP_FIND(map, *key), "Value 3") == 0));
    }
    assert(keys_found == 3);  // All keys were found

    // === Testing MAP_RESOLVE ===
    const int* resolved_key = NULL;
    char** value = MAP_RESOLVE(map, resolved_key = (const int[]){2});
    assert(value != NULL && strcmp(*value, "Value 2") == 0);

    // === Testing MAP_RESOLVE_OR_INSERT ===
    value = MAP_RESOLVE_OR_INSERT(map, resolved_key = (const int[]){4});
    assert(value == NULL);  // Entry was just inserted; value uninitialized
    *value = strdup("Value 4");
    assert(strcmp(*MAP_FIND(map, (int){4}), "Value 4") == 0);  // Validate insertion

    // === Testing MAP_REMOVE ===
    assert(MAP_REMOVE(map, (int){2}));       // Remove key=2
    assert(MAP_FIND(map, (int){2}) == NULL);  // Validate removal
    assert(MAP_REMOVE(map, (int){5}) == false);  // Removing a non-existent item returns false

    // === Testing MAP_ENSURE_EXTRA_CAPACITY ===
    MAP_ENSURE_EXTRA_CAPACITY(map, 10);
    assert(CARR_RING_BUFFER_T(map)->capacity >= 20);  // Ensure capacity increased

    // === Testing MAP_TRY_ENSURE_EXTRA_CAPACITY ===
    success = MAP_TRY_ENSURE_EXTRA_CAPACITY(map, 30);
    assert(success);
    assert(CARR_RING_BUFFER_T(map)->capacity >= 50);  // Ensure capacity increased

    // === Testing MAP_CLEAR ===
    MAP_CLEAR(map);
    assert(MAP_NEXT_KEY(map, NULL) == NULL);  // Ensure map is empty

    // === Testing MAP_FREE ===
    MAP_FREE(map);
    assert(map == NULL);  // Ensure map is cleared and freed

    printf("All MAP tests passed successfully!\n");
    return 0;
}