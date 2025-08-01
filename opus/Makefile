DIR := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
ROOT := $(DIR)/..

include ../common.mk

OPUS_INCLUDE=-Ideps/install/include -Ideps/install/include/opus

all: opusplay opusplay_rlbox

opusplay: opusplay.c gen/lib_trampolines.S gen/lib_init.c
	$(CC) $^ -O2 -o $@ $(OPUS_INCLUDE) -Igen $(LIBLFI) -lportaudio

opusplay_rlbox: opusplay_rlbox.cc libopusfile.inc.s libopusfile.lfi
	$(CXX) -std=gnu++20 $(RLBOX_INCLUDE) $(OPUS_INCLUDE) -o $@ $< libopusfile.inc.s $(LIBLFI) -lportaudio -O2

libopusfile.lfi: deps/install/lib/libopusfile.a deps/install/lib/libopus.a deps/install/lib/libogg.a
	$(LFI_CC) -static-pie -Wl,--export-dynamic -Wl,--whole-archive $^ -Wl,--no-whole-archive -lboxrt -o $@

gen/lib_trampolines.S gen/lib_init.c &: libopusfile.lfi libopusfile.symbols
	mkdir -p gen
	$(LFI_BIND) -no-verify -gen-trampolines gen/lib_trampolines.S -gen-init gen/lib_init.c -lib opusfile_box -symbols-file libopusfile.symbols $<

clean:
	rm -f libopusfile.lfi opusplay
	rm -rf gen

.PHONY: all clean
