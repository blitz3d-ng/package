ENV := release

ifeq ($(shell uname), Darwin)
NUMBER_OF_CORES=$(shell sysctl -n hw.ncpu)
PLATFORM := macos
else
NUMBER_OF_CORES=$(shell grep -c ^processor /proc/cpuinfo)
PLATFORM := linux
endif

ifeq ($(PLATFORM), ios)
CMAKE_OPTIONS=-DCMAKE_TOOLCHAIN_FILE=src/ios.toolchain.cmake -DIOS_PLATFORM=SIMULATOR64
endif

ifeq ($(PLATFORM), emscripten)
CMAKE_OPTIONS=-DCMAKE_TOOLCHAIN_FILE=/emsdk/emscripten/1.38.12/cmake/Modules/Platform/Emscripten.cmake
endif

ifeq ($(PLATFORM), mingw32)
CMAKE_OPTIONS=-DCMAKE_TOOLCHAIN_FILE=src/mingw-w64.toolchain.cmake
endif

MAKE=make -j$(NUMBER_OF_CORES)

build:
	cmake -H. -Bbuild/$(PLATFORM)/$(ENV) -DBB_PLATFORM=$(PLATFORM) -DBB_ENV=$(ENV) $(CMAKE_OPTIONS) && (cd build/$(PLATFORM)/$(ENV) && $(MAKE))

.PHONY: build
