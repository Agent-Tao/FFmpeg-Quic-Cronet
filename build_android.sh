#!/bin/bash

TOOLCHAIN=/home/hezhen/package/ndk_17c_arch_arm_api_21_toolchain
SYSROOT=$TOOLCHAIN/sysroot
CPU=armv7-a
PREFIX=$(pwd)/android/$CPU
EXTRA_CFLAGS="-Os -fpic -DANDROID -D__thumb__ -mthumb -mfloat-abi=softfp -mfpu=vfp -march=$CPU -D__ANDROID_API__=21 -D__GLIBC_HAVE_LONG_LONG"
EXTRA_LDFLAGS="-march=$CPU "
CRONET_INCLUDE_FLAG="-I/home/hezhen/google/CronetTestAndroid/cronet_c_headers"
CRONET_LIB_DIR="/home/hezhen/google/chromium/src/out/Release/cronet/libs/armeabi-v7a"
CRONET_LIB_FLAG="-L$CRONET_LIB_DIR"

rm -fr build
mkdir build

rm -fr android
mkdir android

cd build

function configure_android {
  ../configure \
    --prefix=$PREFIX \
    --enable-neon \
    --enable-hwaccels \
    --enable-shared \
    --enable-jni \
    --enable-mediacodec \
    --enable-decoder=h264_mediacodec \
    --disable-static \
    --disable-doc \
    --enable-ffmpeg \
    --enable-libcronet \
    --disable-ffplay \
    --disable-ffprobe \
    --enable-avdevice \
    --disable-doc \
    --disable-symver \
    --cross-prefix=$TOOLCHAIN/bin/arm-linux-androideabi- \
    --target-os=android \
    --arch=arm \
    --cpu=armv7-a \
    --enable-cross-compile \
    --sysroot=$SYSROOT \
    --extra-cflags="$EXTRA_CFLAGS $CRONET_INCLUDE_FLAG" \
    --extra-ldflags="$EXTRA_LDFLAGS $CRONET_LIB_FLAG" \
    --extra-libs=-lcronet.73.0.3683.75
}

configure_android
make
make install
cp $CRONET_LIB_DIR/libcronet.73.0.3683.75.so $PREFIX/lib/ 

cd ..

echo Build done.

