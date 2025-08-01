set -ex

mkdir -p deps
cd deps

if [ ! -d expat ]; then
    wget https://github.com/libexpat/libexpat/releases/download/R_2_7_1/expat-2.7.1.tar.gz
    tar -xf expat-2.7.1.tar.gz
    rm -f expat-2.7.1.tar.gz
    mv expat-2.7.1 expat
fi

cd expat
mkdir -p build-lfi && cd build-lfi
../configure --prefix=$PWD/../../install CC=$LFI_CC --host $ARCH CPPFLAGS="-DXML_ENABLE_VISIBILITY=1"
make install
cd ../..

cd expat
mkdir -p build-native && cd build-native
../configure --prefix=$PWD/../../install-native --host $ARCH
make install
cd ../..
