#include <stdio.h>
#include <stdlib.h>
#include <expat.h>

#define RLBOX_SINGLE_THREADED_INVOCATIONS

#include "rlbox.hpp"
#include "rlbox_lfi_sandbox.hpp"

extern "C" {
    extern uint8_t expat_start[];
    extern uint8_t expat_end[];
};

RLBOX_DEFINE_BASE_TYPES_FOR(expat, lfi);

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

static void XMLCALL startElement(rlbox_sandbox_expat& sandbox, tainted_expat<void*> userData, tainted_expat<const XML_Char*> name, tainted_expat<const XML_Char**> atts) {
    int i;
    int* const depthPtr = (int*) userData.unverified_safe_pointer_because(sizeof(int), "depthPtr");
    (void) atts;

    for (i = 0; i < *depthPtr; i++)
        putchar('\t');

    std::unique_ptr<XML_Char[]> checked_name = name.copy_and_verify_string([](std::unique_ptr<XML_Char[]> val) {
        return move(val);
    });

    printf("%" XML_FMT_STR "\n", checked_name.get());
    *depthPtr += 1;
}

static void XMLCALL endElement(rlbox_sandbox_expat& sandbox, tainted_expat<void*> userData, tainted_expat<const XML_Char*> name) {
    int* const depthPtr = (int*) userData.unverified_safe_pointer_because(sizeof(int), "depthPtr");
    (void) name;

    *depthPtr -= 1;
}

int main(int argc, char const* argv[]) {
    rlbox_sandbox_expat sandbox;
    sandbox.create_sandbox(expat_start, expat_end);

    tainted_expat<XML_Parser> parser = sandbox.invoke_sandbox_function(XML_ParserCreate, nullptr);

    int done;
    tainted_expat<int*> depth = sandbox.malloc_in_sandbox<int>(sizeof(int));
    *depth = 0;

    if (!parser)
        ERROR("error: XML_ParserCreate\n");

    sandbox.invoke_sandbox_function(XML_SetUserData, parser, depth);

    auto box_startElement = sandbox.register_callback(startElement);
    auto box_endElement = sandbox.register_callback(endElement);

    sandbox.invoke_sandbox_function(XML_SetElementHandler, parser, box_startElement, box_endElement);

    do {
        tainted_expat<void*> buf = sandbox.invoke_sandbox_function(XML_GetBuffer, parser, BUFSIZ);
        if (!buf)
            ERROR("error: XML_GetBuffer\n");
        const size_t len = fread(buf.unverified_safe_pointer_because(BUFSIZ, "fread"), 1, BUFSIZ, stdin);
        if (ferror(stdin))
            ERROR("error: fread\n");

        done = feof(stdin);

        enum XML_Status status = sandbox.invoke_sandbox_function(XML_ParseBuffer, parser, (int) len, done)
            .unverified_safe_because("error status");

        if (status == XML_STATUS_ERROR) {
            XML_Size line = sandbox.invoke_sandbox_function(XML_GetCurrentLineNumber, parser)
                .unverified_safe_because("line number");
            enum XML_Error code = sandbox.invoke_sandbox_function(XML_GetErrorCode, parser)
                .unverified_safe_because("error code");
            std::unique_ptr<XML_LChar[]> err = sandbox.invoke_sandbox_function(XML_ErrorString, code)
                .copy_and_verify_string([](std::unique_ptr<XML_LChar[]> val) {
                    return move(val);
                });
            ERROR("parse error at line %" XML_FMT_INT_MOD "u:\n%" XML_FMT_STR "\n",
                    line, err.get());
        }
    } while (!done);

    sandbox.invoke_sandbox_function(XML_ParserFree, parser);

    sandbox.destroy_sandbox();
}
