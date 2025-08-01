LFI_CC=x86_64-lfi-linux-musl-clang
RLBOX_INCLUDE=-Ideps/rlbox/code/include -Ideps/rlbox_lfi_sandbox/include
OPUS_INCLUDE=-Ideps/install/include -Ideps/install/include/opus
LIBLFI=deps/install/lib/liblfi.a

opusplay: opusplay.c gen/lib_trampolines.S gen/lib_init.c
	$(CC) $^ -O2 -o $@ $(OPUS_INCLUDE) -Igen $(LIBLFI) -lportaudio

# In-progress
opusplay_rlbox: opusplay_rlbox.cc libopusfile.inc.s libopusfile.lfi
	$(CXX) -std=gnu++20 $(RLBOX_INCLUDE) $(OPUS_INCLUDE) -o $@ $< libopusfile.inc.s $(LIBLFI) -lportaudio -O2

libopusfile.lfi: deps/install/lib/libopusfile.a deps/install/lib/libopus.a deps/install/lib/libogg.a
	$(LFI_CC) -static-pie -Wl,--export-dynamic -Wl,--whole-archive $^ -Wl,--no-whole-archive -lboxrt -o $@

gen/lib_trampolines.S gen/lib_init.c &: libopusfile.lfi libopusfile.symbols
	mkdir -p gen
	lfi-bind -no-verify -gen-trampolines gen/lib_trampolines.S -gen-init gen/lib_init.c -lib opusfile_box -symbols-file libopusfile.symbols $<

clean:
	rm -f libopusfile.lfi opusplay
	rm -rf gen

.PHONY: clean
