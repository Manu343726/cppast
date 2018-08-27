#/bin/sh
set -e

if [[ -z "$SOURCE_DIR" ]]; then
    SOURCE_DIR=".."
fi

if [[ -z "$LLVM_VERSION" ]]; then
    LLVM_VERSION="4.0.0"
fi

if [[ -z "$GENERATOR" ]]; then
    GENERATOR=Ninja
fi

mkdir build && cd build

echo Cross building: $CROSS_BUILDING
echo Source dir: $SOURCE_DIR
echo LLVM version: $LLVM_VERSION
echo Generator: $GENERATOR

if [ "$CROSS_BUILDING" == "YES" ]; then
    toolchain="-DCMAKE_TOOLCHAIN_FILE=$DCMAKE_TOOLCHAIN_FILE"
else
    toolchain=""
fi

cmake $SOURCE_DIR -G $GENERATOR \
        $toolchain \
        -DLLVM_VERSION="${LLVM_VERSION}" \
        -DLLVM_DOWNLOAD_OS_NAME="x86_64-linux-gnu-ubuntu-14.04" \
        -DCMAKE_VERBOSE_MAKEFILE=ON

cmake --build .

if [ "$CROSS_BUILDING" != "YES" ]; then
    ctest . -V
fi
