ENV := release

ifeq ($(shell uname), Darwin)
NUMBER_OF_CORES=$(shell sysctl -n hw.ncpu)
PLATFORM := macos
else
NUMBER_OF_CORES=$(shell grep -c ^processor /proc/cpuinfo)
PLATFORM := linux
endif

IOS_VERSION=11

ifeq ($(PLATFORM), ios)
CMAKE_OPTIONS=-DCMAKE_TOOLCHAIN_FILE=src/ios.toolchain.cmake -DIOS_PLATFORM=SIMULATOR64 -DIOS_DEPLOYMENT_TARGET=$(IOS_VERSION)
endif

ifeq ($(PLATFORM), emscripten)
CMAKE_OPTIONS=-DCMAKE_TOOLCHAIN_FILE=/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake
endif

ifeq ($(PLATFORM), mingw32)
CMAKE_OPTIONS=-DCMAKE_TOOLCHAIN_FILE=src/mingw-w64.toolchain.cmake
endif

ifeq ($(PLATFORM), android)
CMAKE_OPTIONS=-DCMAKE_TOOLCHAIN_FILE=/opt/android-ndk/build/cmake/android.toolchain.cmake -DANDROID_ABI="armeabi-v7a"
endif

MAKE=make -j$(NUMBER_OF_CORES)

build:
	cmake -H. -Bbuild/$(PLATFORM)/$(ENV) -DCMAKE_TOOLCHAIN_FILE=src/llvm.cmake -DBB_PLATFORM=$(PLATFORM) -DBB_ENV=$(ENV) $(CMAKE_OPTIONS) && (cd build/$(PLATFORM)/$(ENV) && $(MAKE))

install-unit-test:
	cp _release/toolchains/mingw32/bin/unit_test.dll ~/.wine/drive_c/Program\ Files/Blitz3D/userlibs/
	cp src/modules/bb/unit-test/unit_test.decls ~/.wine/drive_c/Program\ Files/Blitz3D/userlibs/

.PHONY: build
