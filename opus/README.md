# Opus Player Example

This example uses PortAudio to play an Opus file that is decoded with a
sandboxed Opus decoder.

Download and build all dependencies (`libopus`, `libogg`, `libopusfile`, `liblfi`):

```
export LFI_CC=...
export ARCH=...

cd deps
./setup.sh
```

Build the player (`opusplay.c`):

```
make
```

Run:

```
./opusplay samples/music.opus
```

The current version (`opusplay.c`) uses `lfi-bind` to call sandboxed functions.
A future version using RLBox is in-progress.
