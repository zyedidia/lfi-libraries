DIR := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
ROOT := $(DIR)/..

include ../common.mk

thread: main.cc lib.lfi lib.inc.s
	$(CXX) -std=gnu++20 $(RLBOX_INCLUDE) -o $@ $< lib.inc.s $(LIBLFI) -O2

lib.lfi: lib.c
	$(LFI_CC) -static-pie -Wl,--export-dynamic $< -o $@ -lboxrt

clean:
	rm -f basic lib.lfi

.PHONY: clean
