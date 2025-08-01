# Opus Player Example

This example uses PortAudio to play an Opus file that is decoded with a
sandboxed Opus decoder. Before continuing, install PortAudio: `apt install
portaudio19-dev`.

Download and build all dependencies (`libopus`, `libogg`, `libopusfile`):

```
export LFI_CC=...
export ARCH=...

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

See `opusplay_rlbox.c` for a version that uses RLBox instead.
