set -ex

if [ ! -d opusfile ]; then
    wget https://downloads.xiph.org/releases/opus/opusfile-0.11.tar.gz
    tar -xf opusfile-0.11.tar.gz
    rm -f opusfile-0.11.tar.gz
    mv opusfile-0.11 opusfile
fi

if [ ! -d opus ]; then
    wget https://github.com/xiph/opus/releases/download/v1.5.2/opus-1.5.2.tar.gz
    tar -xf opus-1.5.2.tar.gz
    rm -f opus-1.5.2.tar.gz
    mv opus-1.5.2 opus
fi

if [ ! -d libogg ]; then
    wget https://downloads.xiph.org/releases/ogg/libogg-1.3.6.tar.gz
    tar -xf libogg-1.3.6.tar.gz
    rm -f libogg-1.3.6.tar.gz
    mv libogg-1.3.6 libogg
fi

if [ ! -d lfi-runtime ]; then
    git clone https://github.com/lfi-project/lfi-runtime
fi

if [ ! -d rlbox ]; then
    git clone https://github.com/PLSysSec/rlbox
fi

if [ ! -d rlbox_lfi_sandbox ]; then
    git clone https://github.com/PLSysSec/rlbox_lfi_sandbox
fi

cd opus
./configure --prefix=$PWD/../install CC=$LFI_CC --host $ARCH
make install
cd ..

cd libogg
./configure --prefix=$PWD/../install CC=$LFI_CC --host $ARCH
make install
cd ..

cd opusfile
./configure --prefix=$PWD/../install CC=$LFI_CC --host $ARCH PKG_CONFIG_PATH="$PWD/../install/lib/pkgconfig" --disable-http
make install
cd ..

cd lfi-runtime
meson setup build --libdir=lib --prefix=$PWD/../install
cd build
ninja install
cd ../..
