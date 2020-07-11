# to link to use: target_link_libraries(loggerlib ${GLIB_LIBRARIES})
# https://stackoverflow.com/questions/36868143/what-is-the-recommended-way-of-using-glib2-with-cmake
find_package(PkgConfig REQUIRED)
pkg_search_module(GLIB REQUIRED glib-2.0)
include_directories(${GLIB_INCLUDE_DIRS})
link_directories(${GLIB_LIBRARY_DIRS})
add_definitions(${GLIB_CFLAGS_OTHER})
