#include <stdio.h>
#include <stdlib.h>

#include "lib.h"

// We're going to use RLBox in a single-threaded environment.
#define RLBOX_SINGLE_THREADED_INVOCATIONS

#include "rlbox.hpp"
#include "rlbox_lfi_sandbox.hpp"

extern "C" {
    extern uint8_t lib_start[];
    extern uint8_t lib_end[];
};

RLBOX_DEFINE_BASE_TYPES_FOR(lib, lfi);

int main() {
    rlbox_sandbox_lib sandbox;
    sandbox.create_sandbox(lib_start, lib_end);

    sandbox.invoke_sandbox_function(hello);

    auto ret = sandbox.invoke_sandbox_function(add, 3, 4)
        .copy_and_verify([](unsigned val) {
            return val;
        });
    printf("add(3, 4) = %d\n", ret);

    sandbox.destroy_sandbox();

    return 0;
}
