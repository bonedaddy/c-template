# c-template

A template repository to use as a base for new C programs. Includes utility functions, and CMake setup with cmocka for unit testing


# Utility Functions

* Array length checking
* Random string generation
* Safe Math (WIP)
* Safe Memory
    * Wraps pointer objects to prevent double free 

# build

```shell
$> mkdir build && cd build
$> cmake -D CMAKE_C_COMPILER=gcc ..
$> cmake -D CMAKE_C_COMPILER=gcc -build  .
$> make
```