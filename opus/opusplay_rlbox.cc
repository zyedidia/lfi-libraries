#include <stdio.h>
#include <stdlib.h>
#include <portaudio.h>
#include <opus/opusfile.h>

#define RLBOX_SINGLE_THREADED_INVOCATIONS

#include "rlbox.hpp"
#include "rlbox_lfi_sandbox.hpp"

extern "C" {
    extern uint8_t opusfile_start[];
    extern uint8_t opusfile_end[];
};

RLBOX_DEFINE_BASE_TYPES_FOR(opusfile, lfi);

#define ERROR(fmt, ...)                                 \
    do {                                                \
        fprintf(stderr, fmt __VA_OPT__(,) __VA_ARGS__); \
        exit(1);                                        \
    } while (0)

int main(int argc, char const *argv[]) {
    rlbox_sandbox_opusfile sandbox;
    sandbox.create_sandbox(opusfile_start, opusfile_end);

    sandbox.destroy_sandbox();
}
