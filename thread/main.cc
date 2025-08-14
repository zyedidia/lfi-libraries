#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "lib.h"

#include "rlbox.hpp"
#include "rlbox_lfi_sandbox.hpp"

extern "C" {
    extern uint8_t lib_start[];
    extern uint8_t lib_end[];
};

RLBOX_DEFINE_BASE_TYPES_FOR(lib, lfi);

static rlbox_sandbox_lib sandbox;

static void* threadfn(void* arg) {
    sandbox.invoke_sandbox_function(foo);
    return NULL;
}

int main(int argc, char const *argv[]) {
    sandbox.create_sandbox(lib_start, lib_end);

    pthread_t t1, t2;
    pthread_create(&t1, NULL, threadfn, NULL);
    pthread_create(&t2, NULL, threadfn, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sandbox.destroy_sandbox();

    return 0;
}
