#include <stdio.h>
#include <stdlib.h>
#include <expat.h>

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

    for (i = 0; i < *depthPtr; i++)
        putchar('\t');
    printf("%" XML_FMT_STR "\n", name);
    *depthPtr += 1;
}

static void XMLCALL endElement(void *userData, const XML_Char *name) {
    int *const depthPtr = (int *) userData;
    (void) name;

    *depthPtr -= 1;
}

int main(void) {
    XML_Parser parser = XML_ParserCreate(NULL);
    int done;
    int depth = 0;

    if (!parser)
        ERROR("error: XML_ParserCreate\n");

    XML_SetUserData(parser, &depth);
    XML_SetElementHandler(parser, startElement, endElement);

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
