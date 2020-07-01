# c-template

A template repository to help make C development easier and safer. Includes a ready-to-go CMake integration, with linking to cmocka. Additionally includes a few WIP utility functions to assist development. It also includes a CLI builder tool, with a basic implementation to generat a zlog configuration file.

# using in your own projects

The first thing you'll want to do is change the project name to yours. To do so run the following sed statements which will replace the template name with yours

```shell
$> sed -i 's/c-template-REPLACEME/yourProjectNameHere/g' CMakeLists.txt
$> sed -i 's/c-template-REPLACEME/yourProjectNameHere/g' cmake/*.cmake
```

After that you'll want to decide what macros to include. The default `CMakeLists.txt` file is setup to build all included libraries, and their associated tests. This may not be needed for your environment.


# Utility Functions

* Array length checking
* Random string generation
* Command line builder
* Basic cli implementation
* Safe Math (WIP)
* Safe Memory
    * Wraps pointer objects to prevent double free 
* Logging
    * wrapper around [`zlog`](https://github.com/HardySimpson/zlog)

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