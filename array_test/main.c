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
    assert(array != NULL);                      // Array allocation must succeed
    assert(ARRAY_SIZE(array) == 0);             // Array size should be 0 initially
    assert(ARRAY_CAPACITY(array) == 5);         // Initial capacity should be 5

    // === ARRAY_PUSH_BACK ===
    for (int i = 0; i < 5; i++) {
        int* element = &ARRAY_PUSH_BACK(array);
        *element = i + 1;                       // Adding values 1 to 5
    }
    assert(ARRAY_SIZE(array) == 5);             // Size should match the number of pushes
    assert(ARRAY_CAPACITY(array) >= 5);         // Capacity should remain sufficient
    assert(ARRAY_LAST(array) == 5);             // Last element is 5

    // === ARRAY_ENSURE_CAPACITY ===
    ARRAY_ENSURE_CAPACITY(array, 20);           // Increase capacity to 20
    assert(ARRAY_CAPACITY(array) >= 20);        // Verify capacity is at least 20

    // === ARRAY_TRY_ENSURE_CAPACITY ===
    bool success = ARRAY_TRY_ENSURE_CAPACITY(array, 40);
    assert(success);                            // Should succeed
    assert(ARRAY_CAPACITY(array) >= 40);        // At least a capacity of 40

    // === ARRAY_RESIZE ===
    ARRAY_RESIZE(array, 10);                    // Resize to exactly 10 elements
    assert(ARRAY_SIZE(array) == 10);            // New size should be 10
    assert(ARRAY_CAPACITY(array) >= 10);        // Capacity should remain sufficient

    // === ARRAY_TRY_RESIZE ===
    success = ARRAY_TRY_RESIZE(array, 15);      // Try resizing to 15 elements
    assert(success);                            // Should succeed
    assert(ARRAY_SIZE(array) == 15);            // Size should now be 15

    // === ARRAY_SHRINK_TO_FIT ===
    success = ARRAY_SHRINK_TO_FIT(array);       // Shrinking capacity to match size
    assert(success);                            // Shrink should succeed
    assert(ARRAY_CAPACITY(array) == 15);        // Capacity must equal size

    // === ARRAY_APPLY ===
    ARRAY_APPLY(array, increment);              // Increment each element by 1
    for (int i = 0; i < ARRAY_SIZE(array); i++) {
        assert(array[i] == i + 2);              // Elements must match expected values
    }

    // === ARRAY_APPLY_LEADING ===
    ARRAY_APPLY_LEADING(array, multiply_by_and_add_leading, 2, 3); // Each element *= 2, add 3
    for (int i = 0; i < ARRAY_SIZE(array); i++) {
        assert(array[i] == (i + 2) * 2 + 3);    // Validate transformed elements
    }

    // === ARRAY_APPLY_TRAILING ===
    ARRAY_APPLY_TRAILING(array, multiply_by_and_add_trailing, 3, 1); // *= 3, add 1
    for (int i = 0; i < ARRAY_SIZE(array); i++) {
        assert(array[i] == ((i + 2) * 2 + 3) * 3 + 1); // Validate transformation
    }

    // === ARRAY_FREE ===
    ARRAY_FREE(array);                          // Free the array memory
    assert(array == NULL);                      // Pointer should now be NULL


    // === LAZY_INIT ===
    ARRAY(float) float_array = NULL;

    printf("All tests passed successfully!\n");
    return 0;
}