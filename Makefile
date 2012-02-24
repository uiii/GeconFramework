ifndef config
  config=debug
endif
export config

.PHONY: all make clean unit-tests

PREMAKE=../config/premake4/premake4

BUILD_DIR=build
LIB_DIR=${BUILD_DIR}

UNIT_TESTS=${BUILD_DIR}/unit_tests
MANUAL_TESTS=${BUILD_DIR}/manual_tests

SET_LIB_PATH=LD_LIBRARY_PATH=${LIB_DIR}

all: build make

build: premake4.lua src/premake4.lua test/premake4.lua
	rm -rf build
	mkdir -p build
	cd build && ${PREMAKE} --file=../premake4.lua --to gmake

make:
	cd build && ${MAKE} config=$(config)

unit-tests:
	-@${SET_LIB_PATH} ${UNIT_TESTS}

manual-tests:
	-@${SET_LIB_PATH} ${MANUAL_TESTS}

clean:
	rm -rf build
