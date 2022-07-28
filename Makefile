ENV := release
GENERATOR := Ninja
GENERATOR_OPTIONS := -k 0

LLVM_VERSION=14.0.4

ifeq ($(shell uname), Darwin)
NUMBER_OF_CORES=$(shell sysctl -n hw.ncpu)
PLATFORM := macos
else
NUMBER_OF_CORES=$(shell grep -c ^processor /proc/cpuinfo)
PLATFORM := linux
endif

IOS_VERSION=15.4
IOS_OPTIONS=-DCMAKE_TOOLCHAIN_FILE=src/ios.toolchain.cmake -DENABLE_BITCODE=OFF -DDEPLOYMENT_TARGET=$(IOS_VERSION)

ifeq ($(PLATFORM), ios)
CMAKE_OPTIONS=$(IOS_OPTIONS) -DARCH=arm64 -DARCHS=arm64 -DPLATFORM=OS64
endif

ifeq ($(PLATFORM), ios-sim)
ifeq ($(shell uname -m), arm64)
IOS_PLATFORM := SIMULATORARM64
else
IOS_PLATFORM := SIMULATOR64
endif
CMAKE_OPTIONS=$(IOS_OPTIONS) -DPLATFORM=$(IOS_PLATFORM)
endif

ifeq ($(PLATFORM), emscripten)
CMAKE_OPTIONS=-DCMAKE_TOOLCHAIN_FILE=/opt/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake
endif

ifeq ($(PLATFORM), mingw32)
CMAKE_OPTIONS=-DCMAKE_TOOLCHAIN_FILE=src/mingw-w64.toolchain.cmake -DARCH=i686
endif

ifeq ($(PLATFORM), mingw64)
CMAKE_OPTIONS=-DCMAKE_TOOLCHAIN_FILE=src/mingw-w64.toolchain.cmake -DARCH=x86_64
endif

ifeq ($(PLATFORM), android)
ANDROID_ABI := armeabi-v7a
CMAKE_OPTIONS=-DCMAKE_TOOLCHAIN_FILE=/opt/android-sdk/ndk-bundle/build/cmake/android.toolchain.cmake -DANDROID_ABI="$(ANDROID_ABI)" -DANDROID_LD=deprecated -DARCH=armeabi-v7a
endif

ifeq ($(PLATFORM), nx)
CMAKE_OPTIONS=-DCMAKE_TOOLCHAIN_FILE=src/devkita64.toolchain.cmake
endif

build:
	cmake -G $(GENERATOR) -H. -Bbuild/$(PLATFORM)/$(ENV) -DOUTPUT_PATH=$(OUTPUT_PATH) -DBB_PLATFORM=$(PLATFORM) -DBB_ENV=$(ENV) $(CMAKE_OPTIONS) && (cd build/$(PLATFORM)/$(ENV) && cmake --build . -j $(NUMBER_OF_CORES) -- $(GENERATOR_OPTIONS))

llvm:
	test -d deps/llvm/tree || git clone -b llvmorg-$(LLVM_VERSION) --recursive https://github.com/llvm/llvm-project.git deps/llvm/tree
	cmake -S deps/llvm -B build/llvm -G $(GENERATOR)
	(cd build/llvm && cmake --build . -j $(NUMBER_OF_CORES) -- $(GENERATOR_OPTIONS))
	(cd build/llvm && cmake --install .)

install-unit-test:
	cp _release/toolchains/mingw32/bin/unit_test.dll ~/.wine/drive_c/Program\ Files/Blitz3D/userlibs/
	cp src/modules/bb/unit-test/unit_test.decls ~/.wine/drive_c/Program\ Files/Blitz3D/userlibs/

clean:
	rm -rf build
	rm -rf _release/toolchains/arm*
	rm -rf _release/toolchains/x84*

.PHONY: build llvm install-unit-test clean
