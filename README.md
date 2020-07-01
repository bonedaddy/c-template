# c-template

A template repository to help make C development easier and safer. Includes a ready-to-go CMake integration, with linking to cmocka. Additionally includes a few WIP utility functions to assist development

# first steps

To change the name of the CLI which is default to `c-template-REPLACEME` to your own application cli name run:

```shell
$> sed -i 's/c-template-REPLACEME/yourNameHEre/g' CMakeLists.txt
```

Then to adjust the project to your own run

```shell
$> sed -i 's/c-template/yourNameHEre/g' CMakeLists.txt
```

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

## normal (release)

```shell
$> mkdir build && cd build
$> cmake -D CMAKE_C_COMPILER=gcc ..
$> cmake -D CMAKE_C_COMPILER=gcc -build  .
$> make
$> ctest # runs tests
```

## debug

```shell
$> mkdir build && cd build
$> cmake -D CMAKE_C_COMPILER=gcc -D CMAKE_BUILD_TYPE=Debug ..
$> cmake -D CMAKE_C_COMPILER=gcc -D CMAKE_BUILD_TYPE=Debug -build  .
$> make
$> ctest # runs tests
```