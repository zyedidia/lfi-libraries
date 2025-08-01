#include <stdio.h>
#include <stdlib.h>
#include <expat.h>

#include "expat_box.h"

#define ERROR(fmt, ...)                                 \
    do {                                                \
        fprintf(stderr, fmt __VA_OPT__(,) __VA_ARGS__); \
        exit(1);                                        \
    } while (0)

#ifdef XML_LARGE_SIZE
# define XML_FMT_INT_MOD "ll"
#else
# define XML_FMT_INT_MOD "l"
#endif

#ifdef XML_UNICODE_WCHAR_T
# define XML_FMT_STR "ls"
#else
# define XML_FMT_STR "s"
#endif

static void XMLCALL startElement(void *userData, const XML_Char *name, const XML_Char **atts) {
    int i;
    int *const depthPtr = (int *) userData;
    (void) atts;

    if (!lfi_box_ptrvalid(expat_box_box, (lfiptr) userData))
        ERROR("invalid pointer: %p\n", userData);
    if (!lfi_box_ptrvalid(expat_box_box, (lfiptr) name))
        ERROR("invalid pointer: %p\n", name);

    for (i = 0; i < *depthPtr; i++)
        putchar('\t');
    printf("%" XML_FMT_STR "\n", name);
    *depthPtr += 1;
}

static void XMLCALL endElement(void *userData, const XML_Char *name) {
    int *const depthPtr = (int *) userData;
    (void) name;

    if (!lfi_box_ptrvalid(expat_box_box, (lfiptr) depthPtr))
        ERROR("invalid pointer: %p\n", depthPtr);

    *depthPtr -= 1;
}

int main(void) {
    XML_Parser parser = XML_ParserCreate(NULL);
    int done;
    int *depth = expat_box_calloc(1, sizeof(int));

    if (!parser)
        ERROR("error: XML_ParserCreate\n");

    XML_SetUserData(parser, depth);

    void *box_startElement = expat_box_register_cb(startElement);
    void *box_endElement = expat_box_register_cb(endElement);

    XML_SetElementHandler(parser, box_startElement, box_endElement);

    do {
        void *const buf = XML_GetBuffer(parser, BUFSIZ);
        if (!buf)
            ERROR("error: XML_GetBuffer\n");

        const size_t len = fread(buf, 1, BUFSIZ, stdin);
        if (ferror(stdin))
            ERROR("error: fread\n");

        done = feof(stdin);

        if (XML_ParseBuffer(parser, (int) len, done) == XML_STATUS_ERROR)
            ERROR("parse error at line %" XML_FMT_INT_MOD "u:\n%" XML_FMT_STR "\n",
                    XML_GetCurrentLineNumber(parser),
                    XML_ErrorString(XML_GetErrorCode(parser)));
    } while (!done);

    XML_ParserFree(parser);

    return 0;
}
