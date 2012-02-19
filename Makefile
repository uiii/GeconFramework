ifndef config
  config=debug
endif
export config

.PHONY: all make clean

PREMAKE=../config/premake4/premake4

all: build make

build: premake4.lua src/premake4.lua test/premake4.lua
	rm -rf build
	mkdir -p build
	cd build && ${PREMAKE} --file=../premake4.lua --to gmake

make:
	cd build && ${MAKE} config=$(config)

clean:
	rm -rf build
