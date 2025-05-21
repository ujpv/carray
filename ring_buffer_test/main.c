#include "../lib/CArrayUtil.h"
#include <assert.h>
#include <stdio.h>

int main() {
    printf("Running ring buffer tests...\n");

    RING_BUFFER(int) buffer = RING_BUFFER_INIT(int);

    // === Test RING_BUFFER_TRY_ENSURE_CAN_PUSH & RING_BUFFER_ENSURE_CAN_PUSH ===
    bool success = RING_BUFFER_TRY_ENSURE_CAN_PUSH(buffer);
    assert(success);
    assert(RING_BUFFER_SIZE(buffer) == 0);
    assert(RING_BUFFER_CAPACITY(buffer) >= 1);

    RING_BUFFER_ENSURE_CAN_PUSH(buffer);
    assert(RING_BUFFER_CAPACITY(buffer) >= 1);
    assert(RING_BUFFER_SIZE(buffer) == 0);

    // === Test RING_BUFFER_PUSH_BACK ===
    RING_BUFFER_PUSH_BACK(buffer) = 10;
    assert(RING_BUFFER_SIZE(buffer) == 1);
    assert(*RING_BUFFER_FRONT(buffer) == 10);
    assert(*RING_BUFFER_BACK(buffer) == 10);

    RING_BUFFER_PUSH_BACK(buffer) = 20;
    assert(RING_BUFFER_SIZE(buffer) == 2);
    assert(*RING_BUFFER_FRONT(buffer) == 10);
    assert(*RING_BUFFER_BACK(buffer) == 20);

    // === Test RING_BUFFER_PUSH_FRONT ===
    RING_BUFFER_PUSH_FRONT(buffer) = 5;
    assert(RING_BUFFER_SIZE(buffer) == 3);
    assert(*RING_BUFFER_FRONT(buffer) == 5);
    assert(*RING_BUFFER_BACK(buffer) == 20);

    // === Test RING_BUFFER_POP_FRONT ===
    RING_BUFFER_POP_FRONT(buffer);
    assert(RING_BUFFER_SIZE(buffer) == 2);
    assert(*RING_BUFFER_FRONT(buffer) == 10);

    // === Test RING_BUFFER_POP_BACK ===
    RING_BUFFER_POP_BACK(buffer);
    assert(RING_BUFFER_SIZE(buffer) == 1);
    assert(*RING_BUFFER_FRONT(buffer) == 10);
    assert(*RING_BUFFER_BACK(buffer) == 10);

    // === Test RING_BUFFER_FREE ===
    RING_BUFFER_FREE(buffer);

    printf("All ring buffer tests passed successfully!\n");
    return 0;
}