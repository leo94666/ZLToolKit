#!/bin/bash

sudo apt install cmake

abi=$1

cd ..
git clone --depth=50 https://github.com/leo94666/ZLToolKit.git
cd ZLToolKit
sudo rm -rf android_build
mkdir -p android_build
rm -rf ./build
ln -s ./android_build ./build
cd android_build 
cmake .. -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK_ROOT/build/cmake/android.toolchain.cmake -DANDROID_NDK=$ANDROID_NDK_ROOT  -DCMAKE_BUILD_TYPE=Release  -DANDROID_ABI=${abi} -DANDROID_NATIVE_API_LEVEL=android-16 -DCMAKE_INSTALL_PREFIX=../out/${abi}/
make -j4
sudo make install
