.PHONY: build-all
build-all:
	( rm -rf build ; mkdir build ; cd build ; cmake -D CMAKE_C_COMPILER=gcc .. ; cmake -D CMAKE_C_COMPILER=gcc -build  . ; make )


.PHONY: build-all-debug
build-all-debug:
	( rm -rf build ; mkdir build ; cd build ; cmake -D CMAKE_C_COMPILER=gcc -D CMAKE_BUILD_TYPE=Debug .. ; cmake -D CMAKE_C_COMPILER=gcc -D CMAKE_BUILD_TYPE=Debug -build  . ; make )


.PHONY: valgrind-all
valgrind-all: build-all
	bash ./scripts/valgrind.sh