LFI_CC ?= x86_64-lfi-linux-musl-clang
LFI_BIND ?= lfi-bind

RLBOX_INCLUDE = -I$(ROOT)/deps/rlbox/code/include -I$(ROOT)/deps/rlbox_lfi_sandbox/include
LIBLFI = $(ROOT)/deps/install/lib/liblfi.a
