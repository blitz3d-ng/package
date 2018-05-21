ifeq ($(shell uname), Darwin)
NUMBER_OF_CORES=$(shell sysctl -n hw.ncpu)
PLATFORM := macos
else
NUMBER_OF_CORES=$(shell grep -c ^processor /proc/cpuinfo)
PLATFORM := linux
endif

ENV := release

build:
	cmake -H. -Bbuild/$(PLATFORM)/$(ENV) -DBB_ENV=$(ENV) && (cd build/$(PLATFORM)/$(ENV) && make -j$(NUMBER_OF_CORES))

.PHONY: build
