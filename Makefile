ENV := release
GENERATOR := Ninja
GENERATOR_OPTIONS := -k 0

ifeq ($(shell uname -m),$(filter $(shell uname -m), arm64 aarch64))
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
ARCH=asmjs
CMAKE_OPTIONS=-DCMAKE_TOOLCHAIN_FILE=$(EMSDK)/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake
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
ARCH=aarch64
CMAKE_OPTIONS=-DDEVKITPRO=$(DEVKITPRO) -DCMAKE_TOOLCHAIN_FILE=$(DEVKITPRO)/cmake/Switch.cmake
endif

PROJECT_TO_BUILD := all

BUILD_DIR=build/$(ARCH)-$(PLATFORM)-$(ENV)

host:
	cmake -G $(GENERATOR) -H. -B$(BUILD_DIR) -DOUTPUT_PATH=$(OUTPUT_PATH) -DBB_PLATFORM=$(PLATFORM) -DBB_ENV=$(ENV) -DARCH=$(ARCH) $(CMAKE_OPTIONS) && (cd $(BUILD_DIR) && cmake --build . -j $(NUMBER_OF_CORES) --target $(PROJECT_TO_BUILD) -- $(GENERATOR_OPTIONS))

all: host ios android ovr emscripten nx help

compiler:
	make host PROJECT_TO_BUILD=blitzcc

ios: compiler
	make PLATFORM=ios
	make PLATFORM=ios-sim

android: compiler
	make PLATFORM=android ARCH=arm64-v8a
	make PLATFORM=android ARCH=armeabi-v7a
	make PLATFORM=android ARCH=x86_64
	make PLATFORM=android ARCH=x86

ovr: ovr-sdk compiler
	make PLATFORM=ovr

ovr-sdk:
	wget -O ovr-sdk.zip https://securecdn.oculus.com/binaries/download/?id=4643347799061523 # 1.50.0
	mkdir ovr-sdk
	cd ovr-sdk && unzip ../ovr-sdk.zip
	rm ovr-sdk.zip

emscripten: compiler
	make PLATFORM=emscripten

nx: compiler
	make PLATFORM=nx DEVKITPRO=$(DEVKITPRO)

llvm:
	./deps/llvm/build-llvm.sh build/llvm llvm

install-unit-test:
	cp _release/toolchains/mingw32/bin/unit_test.dll ~/.wine/drive_c/Program\ Files/Blitz3D/userlibs/
	cp src/modules/bb/unit-test/unit_test.decls ~/.wine/drive_c/Program\ Files/Blitz3D/userlibs/

help:
	(cd src/help && bundle && ./build.rb)

dist-pkg:
	mv _release blitz3d-ng
	zip -r -x".DS_Store" ./package.zip blitz3d-ng
	mv blitz3d-ng _release

dist-toolchain:
	(cd _release/bin && zip -r -x".DS_Store" ../../toolchain.zip .)

clean:
	rm -rf build
	rm -rf _release/bin
	rm -rf _release/help/assets
	rm -rf _release/help/language
	rm -rf _release/help/reference
	rm -f _release/help/*.html
	rm -rf _release/*.exe
	rm -rf _release/*.app
	rm -rf coverage
	rm -rf tmp

.PHONY: build llvm install-unit-test help clean emscripten
