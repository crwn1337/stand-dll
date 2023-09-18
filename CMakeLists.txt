cmake_minimum_required(VERSION 3.19)
project(stand_dll)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_SHARED_LIBRARY_SUFFIX ".dll")
set(CMAKE_SHARED_LIBRARY_PREFIX "")

add_library(stand_dll SHARED
    dll.cpp
    defs.cpp
)

target_link_libraries(stand_dll PUBLIC
    # c++ stuff
    stdc++

    # win32 stuff
    ole32
    uuid
    urlmon

    # link statically, needed
    "-static"

    # strip symbols
    "-s"
)

set_target_properties(stand_dll PROPERTIES
    OUTPUT_NAME "d3dcompiler_43"
    LINK_FLAGS ${CMAKE_SOURCE_DIR}/defs.def
)