#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "../lib/CArrayUtil.h"

// Increment function to test ARRAY_APPLY
void increment(int* element) {
    (*element)++;
}

// Multiply and add function for ARRAY_APPLY_LEADING
void multiply_by_and_add_leading(int* element, int multiplier, int addition) {
    *element = (*element) * multiplier + addition;
}

// Multiply and add function for ARRAY_APPLY_TRAILING
void multiply_by_and_add_trailing(int multiplier, int addition, int* element) {
    *element = (*element) * multiplier + addition;
}

int main() {
    printf("Running unit tests for ARRAY macros...\n");

    // === ARRAY_ALLOC ===
    ARRAY(int) array = ARRAY_ALLOC(int, 5);
    assert(array != NULL);
    assert(ARRAY_SIZE(array) == 0);
    assert(ARRAY_CAPACITY(array) == 5);

    // === ARRAY_PUSH_BACK ===
    for (int i = 0; i < 200; i++) {
        int* element = &ARRAY_PUSH_BACK(array);
        *element = i + 1;
    }
    assert(ARRAY_SIZE(array) == 200);
    assert(ARRAY_CAPACITY(array) >= 200);
    assert(ARRAY_LAST(array) == 200);

    // === ARRAY_ENSURE_CAPACITY ===
    ARRAY_ENSURE_CAPACITY(array, 2000);
    assert(ARRAY_CAPACITY(array) >= 2000);
    assert(ARRAY_SIZE(array) == 200);

    // === ARRAY_TRY_ENSURE_CAPACITY ===
    bool success = ARRAY_TRY_ENSURE_CAPACITY(array, 4000);
    assert(success);
    assert(ARRAY_CAPACITY(array) >= 4000);
    assert(ARRAY_SIZE(array) == 200);

    // === ARRAY_APPLY ===
    ARRAY_APPLY(array, increment);
    size_t size = ARRAY_SIZE(array);
    for (int i = 0; i < size; i++) {
        int val = array[i];
        assert(val == i + 2);
    }

    // === ARRAY_APPLY_LEADING ===
    ARRAY_APPLY_LEADING(array, multiply_by_and_add_leading, 2, 3); // Each element *= 2, add 3
    for (int i = 0; i < ARRAY_SIZE(array); i++) {
        assert(array[i] == (i + 2) * 2 + 3);
    }

    // === ARRAY_APPLY_TRAILING ===
    ARRAY_APPLY_TRAILING(array, multiply_by_and_add_trailing, 3, 1); // *= 3, add 1
    for (int i = 0; i < ARRAY_SIZE(array); i++) {
        assert(array[i] == ((i + 2) * 2 + 3) * 3 + 1);
    }

    // === ARRAY_RESIZE ===
    ARRAY_RESIZE(array, 10);
    assert(ARRAY_SIZE(array) == 10);
    assert(ARRAY_CAPACITY(array) >= 10);

    // === ARRAY_TRY_RESIZE ===
    success = ARRAY_TRY_RESIZE(array, 15);
    assert(success);
    assert(ARRAY_SIZE(array) == 15);

    // === ARRAY_SHRINK_TO_FIT ===
    success = ARRAY_SHRINK_TO_FIT(array);
    assert(success);
    assert(ARRAY_CAPACITY(array) == 15);

    // === ARRAY_FREE ===
    ARRAY_FREE(array);
    assert(array == NULL);

    // === LAZY_INIT ===
    ARRAY_PUSH_BACK(array) = 10;
    ARRAY_PUSH_BACK(array) = 20;
    assert(ARRAY_SIZE(array) == 2);
    assert(array[0] == 10 && array[1] == 20);

    printf("All tests passed successfully!\n");
    return 0;
}