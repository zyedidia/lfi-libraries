# LFI Libraries

Example applications with libraries sandboxed with LFI.

First, set up libraries that are used in all the examples (`liblfi` and RLBox):

```
./setup.sh
```

Current examples:

* `basic`: example integration showing RLBox+LFI.
* `opus`: example Opus audio player using a sandboxed Opus decoder.
* `expat`: example XML parser using a sandboxed Expat XML parser.
