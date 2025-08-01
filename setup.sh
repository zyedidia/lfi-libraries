set -ex

mkdir -p deps
cd deps

if [ ! -d lfi-runtime ]; then
    git clone https://github.com/lfi-project/lfi-runtime
fi

if [ ! -d rlbox ]; then
    git clone https://github.com/PLSysSec/rlbox
fi

if [ ! -d rlbox_lfi_sandbox ]; then
    git clone https://github.com/PLSysSec/rlbox_lfi_sandbox
fi

cd lfi-runtime
meson setup build --libdir=lib --prefix=$PWD/../install
cd build
ninja install
