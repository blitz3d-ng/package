ENV := release

ifeq ($(shell uname), Darwin)
NUMBER_OF_CORES=$(shell sysctl -n hw.ncpu)
PLATFORM := macos
else
NUMBER_OF_CORES=$(shell grep -c ^processor /proc/cpuinfo)
PLATFORM := linux
endif

IOS_VERSION=11

ifeq ($(PLATFORM), linux)
CMAKE_OPTIONS=-DCMAKE_TOOLCHAIN_FILE=src/llvm.cmake
endif

ifeq ($(PLATFORM), macos)
CMAKE_OPTIONS=-DCMAKE_TOOLCHAIN_FILE=src/llvm.cmake
endif

ifeq ($(PLATFORM), ios)
CMAKE_OPTIONS=-DCMAKE_TOOLCHAIN_FILE=src/ios.toolchain.cmake -DIOS_PLATFORM=SIMULATOR64 -DIOS_DEPLOYMENT_TARGET=$(IOS_VERSION)
endif

ifeq ($(PLATFORM), emscripten)
CMAKE_OPTIONS=-DCMAKE_TOOLCHAIN_FILE=/opt/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake
endif

ifeq ($(PLATFORM), mingw32)
CMAKE_OPTIONS=-DCMAKE_TOOLCHAIN_FILE=src/mingw-w64.toolchain.cmake
endif

ifeq ($(PLATFORM), android)
ANDROID_ABI := armeabi-v7a
CMAKE_OPTIONS=-DCMAKE_TOOLCHAIN_FILE=/opt/android-sdk/ndk-bundle/build/cmake/android.toolchain.cmake -DANDROID_ABI="$(ANDROID_ABI)"
endif

ifeq ($(PLATFORM), nx)
CMAKE_OPTIONS=-DCMAKE_TOOLCHAIN_FILE=src/devkita64.toolchain.cmake
endif

build:
	cmake -G Ninja -H. -Bbuild/$(PLATFORM)/$(ENV) -DBB_PLATFORM=$(PLATFORM) -DBB_ENV=$(ENV) $(CMAKE_OPTIONS) && (cd build/$(PLATFORM)/$(ENV) && cmake --build . -j $(NUMBER_OF_CORES) -- -k 0)

install-unit-test:
	cp _release/toolchains/mingw32/bin/unit_test.dll ~/.wine/drive_c/Program\ Files/Blitz3D/userlibs/
	cp src/modules/bb/unit-test/unit_test.decls ~/.wine/drive_c/Program\ Files/Blitz3D/userlibs/

.PHONY: build
