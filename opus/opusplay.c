#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <opus/opusfile.h>
#include <portaudio.h>

#include "opusfile_box.h"

#define ERROR(fmt, ...)                                 \
    do {                                                \
        fprintf(stderr, fmt __VA_OPT__(,) __VA_ARGS__); \
        exit(1);                                        \
    } while (0)

int main(int argc, char **argv) {
    if (argc < 2) {
        ERROR("error: no file provided\n");
    }

    PaStream *stream;
    short *pcmdata = opusfile_box_malloc(960 * sizeof(short));
    if (!pcmdata)
        ERROR("error: sandbox malloc failed\n");

    Pa_Initialize();
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

    void *box_buf = opusfile_box_mmap(NULL, length, LFI_PROT_READ, LFI_MAP_PRIVATE, fileno(f), 0);
    if (box_buf == (void *) -1)
        ERROR("error: opusfile_box_mmap\n");

    fclose(f);

    OggOpusFile *op_parser = LFI_CALL(op_open_memory, box_buf, length, 0);
    if (!op_parser)
        ERROR("error: op_open_memory %s\n", argv[1]);

    printf("%s: %.2fs\n", argv[1], (float) LFI_CALL(op_pcm_total, op_parser, 0) / 48000.0f);

    while ((err = LFI_CALL(op_read_stereo, op_parser, pcmdata, 960)) > 0) {
        if (Pa_WriteStream(stream, pcmdata, 480) != paNoError)
            break;
    }

    opusfile_box_free(pcmdata);
    opusfile_box_munmap(box_buf, length);

    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();

    return 0;
}
