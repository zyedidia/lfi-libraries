# Expat Example

This example uses Expat to parse an XML file and list elements.

Download and build `libexpat`:

```
export LFI_CC=...
export ARCH=...

./setup.sh
```

Build the tool (`elements_lfi.c`):

```
make
```

Run:

```
./elements_lfi < books.xml
```
