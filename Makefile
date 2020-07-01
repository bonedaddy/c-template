.PHONY: build-all
build-all:
	( rm -rf build ; mkdir build ; cd build ; cmake -D CMAKE_C_COMPILER=gcc .. ; cmake -D CMAKE_C_COMPILER=gcc -build  . ; make )