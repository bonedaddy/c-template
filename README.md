# c-template

A template repository to help make C development easier and safer. Includes a ready-to-go CMake integration, with linking to cmocka. Additionally includes a few WIP utility functions to assist development

# Utility Functions

* Array length checking
* Random string generation
* Safe Math (WIP)
* Safe Memory
    * Wraps pointer objects to prevent double free 
* Logging

# Logging

A wrapper around [`zlog`](https://github.com/HardySimpson/zlog)

# build

```shell
$> mkdir build && cd build
$> cmake -D CMAKE_C_COMPILER=gcc ..
$> cmake -D CMAKE_C_COMPILER=gcc -build  .
$> make
$> ctest # runs tests
```