ENV := release
GENERATOR := Ninja
GENERATOR_OPTIONS := -k 0

LLVM_VERSION=14.0.4

ifeq ($(shell uname -m), arm64)
ARCH := arm64
else
ARCH := x86_64
endif

ifeq ($(shell uname), Darwin)
NUMBER_OF_CORES=$(shell sysctl -n hw.ncpu)
PLATFORM := macos
else
NUMBER_OF_CORES=$(shell grep -c ^processor /proc/cpuinfo)
PLATFORM := linux
endif

IOS_VERSION:=15.4
IOS_OPTIONS=-DCMAKE_TOOLCHAIN_FILE=src/ios.toolchain.cmake -DENABLE_BITCODE=OFF -DDEPLOYMENT_TARGET=$(IOS_VERSION)

ANDROID_PLATFORM:=30

ifeq ($(PLATFORM), ios)
ARCH=arm64
CMAKE_OPTIONS=$(IOS_OPTIONS) -DARCHS=arm64 -DPLATFORM=OS64
endif

ifeq ($(PLATFORM), ios-sim)
ifeq ($(ARCH), arm64)
IOS_PLATFORM:=SIMULATORARM64
else
IOS_PLATFORM:=SIMULATOR64
endif
CMAKE_OPTIONS=$(IOS_OPTIONS) -DPLATFORM=$(IOS_PLATFORM)
endif

ifeq ($(PLATFORM), emscripten)
CMAKE_OPTIONS=-DCMAKE_TOOLCHAIN_FILE=/opt/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake
endif

ifeq ($(PLATFORM), mingw32)
ARCH=i686
CMAKE_OPTIONS=-DCMAKE_TOOLCHAIN_FILE=src/mingw-w64.toolchain.cmake
endif

ifeq ($(PLATFORM), mingw64)
ARCH=x86_64
CMAKE_OPTIONS=-DCMAKE_TOOLCHAIN_FILE=src/mingw-w64.toolchain.cmake
endif

ifeq ($(PLATFORM), android)
ARCH:=arm64-v8a# armeabi-v7a x86_64 x86
CMAKE_OPTIONS=-DCMAKE_TOOLCHAIN_FILE=$(ANDROID_HOME)/ndk-bundle/build/cmake/android.toolchain.cmake -DANDROID_NATIVE_API_LEVEL=$(ANDROID_PLATFORM) -DANDROID_PLATFORM=$(ANDROID_PLATFORM) -DANDROID_ABI="$(ARCH)" -DANDROID_LD=deprecated -DARCH=$(ARCH)
endif

ifeq ($(PLATFORM), ovr)
ANDROID_PLATFORM=24
ARCH=arm64-v8a
CMAKE_OPTIONS=-DCMAKE_TOOLCHAIN_FILE=$(ANDROID_HOME)/ndk-bundle/build/cmake/android.toolchain.cmake -DANDROID_NATIVE_API_LEVEL=$(ANDROID_PLATFORM) -DANDROID_PLATFORM=$(ANDROID_PLATFORM) -DANDROID_ABI="$(ARCH)" -DANDROID_LD=deprecated -DARCH=$(ARCH)
endif

ifeq ($(PLATFORM), nx)
CMAKE_OPTIONS=-DCMAKE_TOOLCHAIN_FILE=src/devkita64.toolchain.cmake
endif

BUILD_DIR=build/$(ARCH)-$(PLATFORM)-$(ENV)

host:
	cmake -G $(GENERATOR) -H. -B$(BUILD_DIR) -DOUTPUT_PATH=$(OUTPUT_PATH) -DBB_PLATFORM=$(PLATFORM) -DBB_ENV=$(ENV) -DARCH=$(ARCH) $(CMAKE_OPTIONS) && (cd $(BUILD_DIR) && cmake --build . -j $(NUMBER_OF_CORES) -- $(GENERATOR_OPTIONS))

ios:
	make PLATFORM=ios
	make PLATFORM=ios-sim

android:
	make PLATFORM=android ARCH=arm64-v8a
	make PLATFORM=android ARCH=armeabi-v7a
	make PLATFORM=android ARCH=x86_64
	make PLATFORM=android ARCH=x86

llvm:
	test -d deps/llvm/tree || git clone -b llvmorg-$(LLVM_VERSION) --recursive https://github.com/llvm/llvm-project.git deps/llvm/tree
	cmake -S deps/llvm -B build/llvm -G $(GENERATOR)
	(cd build/llvm && cmake --build . -j $(NUMBER_OF_CORES) -- $(GENERATOR_OPTIONS))
	(cd build/llvm && cmake --install .)

install-unit-test:
	cp _release/toolchains/mingw32/bin/unit_test.dll ~/.wine/drive_c/Program\ Files/Blitz3D/userlibs/
	cp src/modules/bb/unit-test/unit_test.decls ~/.wine/drive_c/Program\ Files/Blitz3D/userlibs/

dist-pkg:
	(cd _release/ && zip -r -x".DS_Store" ../package.zip .)

dist-toolchain:
	(cd _release/bin && zip -r -x".DS_Store" ../../toolchain.zip .)

clean:
	rm -rf build
	rm -rf _release/bin

.PHONY: build llvm install-unit-test clean
