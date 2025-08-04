DIR := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
ROOT := $(DIR)/..

include ../common.mk

all: elements_native elements_lfi elements_rlbox

elements_native: elements_native.c
	$(CC) -O2 $< -o $@ -Ldeps/install-native/lib -Ideps/install-native/include -lexpat

elements_rlbox: elements_rlbox.cc libexpat.lfi libexpat.inc.s
	$(CXX) -std=gnu++20 $(RLBOX_INCLUDE) -Ideps/install/include -o $@ $< libexpat.inc.s $(LIBLFI) -O2

elements_lfi: elements_lfi.c gen/lib_trampolines.S gen/lib_init.c
	$(CC) $^ -O2 -o $@ -Ideps/install/include -Igen -Ldeps/install/lib $(LIBLFI) -lexpat

libexpat.lfi: deps/install/lib/libexpat.a
	$(LFI_CC) -static-pie -Wl,--export-dynamic -Wl,--whole-archive $^ -Wl,--no-whole-archive -lboxrt -o $@

gen/lib_trampolines.S gen/lib_init.c &: libexpat.lfi libexpat.symbols
	mkdir -p gen
	$(LFI_BIND) -no-verify -gen-trampolines gen/lib_trampolines.S -gen-init gen/lib_init.c -lib expat_box -symbols-file libexpat.symbols $<

clean:
	rm -f elements_native elements_lfi elements_rlbox libexpat.lfi
	rm -rf gen

.PHONY: all clean
