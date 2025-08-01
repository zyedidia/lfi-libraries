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

    tainted_opusfile<short*> pcmdata = sandbox.malloc_in_sandbox<short>(960 * sizeof(short));

    Pa_Initialize();
    PaStream *stream;
    int err = Pa_OpenDefaultStream(&stream, 0, 2, paInt16, 48000.0, 480, NULL, NULL);
    if (err != 0)
        ERROR("error: Pa_OpenDefaultStream\n");
    err = Pa_StartStream(stream);
    if (err != 0)
        ERROR("error: Pa_StartStream\n");

    FILE *f = fopen(argv[1], "r");
    if (!f)
        ERROR("error: fopen %s", argv[1]);

    fseek(f, 0L, SEEK_END);
    size_t length = ftell(f);
    fseek(f, 0L, SEEK_SET);

    tainted_opusfile<unsigned char*> box_buf = sandbox.malloc_in_sandbox<unsigned char>(length);
    if (!box_buf)
        ERROR("error: opusfile_box_malloc\n");

    size_t n = fread(box_buf.unverified_safe_pointer_because(length, "fread"), length, 1, f);
    if (n != 1)
        ERROR("error: fread");

    fclose(f);

    tainted_opusfile<OggOpusFile*> op_parser = sandbox.invoke_sandbox_function(op_open_memory, box_buf, length, nullptr);
    if (!op_parser)
        ERROR("error: op_open_memory %s\n", argv[1]);

    int64_t pcm_total = sandbox.invoke_sandbox_function(op_pcm_total, op_parser, 0)
        .copy_and_verify([](int64_t val) {
            return val;
        });
    printf("%s: %.2fs\n", argv[1], (float) pcm_total / 48000.0f);

    while ((err = sandbox.invoke_sandbox_function(op_read_stereo, op_parser, pcmdata, 960)
                .copy_and_verify([](int val) { return val; })) > 0) {
        if (Pa_WriteStream(stream, pcmdata.unverified_safe_pointer_because(960 * sizeof(short), "Pa_WriteStream"), 480) != paNoError)
            break;
    }

    sandbox.free_in_sandbox(pcmdata);
    sandbox.free_in_sandbox(box_buf);

    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();

    sandbox.destroy_sandbox();
}
