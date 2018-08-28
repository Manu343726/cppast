#/bin/sh
set -e

if [[ -z "$SOURCE_DIR" ]]; then
    SOURCE_DIR=`pwd`
fi

if [[ -z "$LLVM_VERSION" ]]; then
    LLVM_VERSION="4.0.0"
fi

if [[ -z "$GENERATOR" ]]; then
    GENERATOR=Ninja
fi

if [[ -z "$BUILD_DIR" ]]; then
    BUILD_DIR=$SOURCE_DIR/build_${CI_JOB_NAME}_${HOSTNAME}
fi

echo Cross building: $CROSS_BUILDING
echo Source dir: $SOURCE_DIR
echo LLVM version: $LLVM_VERSION
echo Generator: $GENERATOR
echo Build directory: $BUILD_DIR

if [[ "$CLEAN_BUILD" == "YES" ]]; then
    echo Cleaning up old build tree...
    rm -rf $BUILD_DIR
fi

mkdir -p $BUILD_DIR && cd $BUILD_DIR

if [ "$CROSS_BUILDING" == "YES" ]; then
    toolchain="-DCMAKE_TOOLCHAIN_FILE=/usr/share/toolchain.cmake"
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
