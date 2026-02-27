# =========================================================
# ManpWIN Root CMake Script
# Copyright 2026 Paul de Leeuw
# =========================================================

cmake_minimum_required(VERSION 3.23)

project(ManpWIN LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

file(GLOB MANPWIN64_SOURCES
    ManpWIN64/*.cpp
    ManpWIN64/*.c
    ManpWIN64/*.rc
)

add_executable(ManpWIN64 WIN32 ${MANPWIN64_SOURCES})

target_include_directories(ManpWIN64 PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/ManpWIN64
    ${CMAKE_SOURCE_DIR}/parser
)

add_subdirectory(parser)
add_subdirectory(qdlib)
add_subdirectory(pnglib)
add_subdirectory(ZLib)
add_subdirectory(MPEG)

# MPFR
add_library(mpfr STATIC IMPORTED)
set_target_properties(mpfr PROPERTIES
    IMPORTED_LOCATION_DEBUG
    "C:/Github/manpwin/mpfr/build.vc15/lib/x64/Debug/mpfr.lib"
    IMPORTED_LOCATION_RELEASE
    "C:/Github/manpwin/mpfr/build.vc15/lib/x64/Release/mpfr.lib"
    IMPORTED_LOCATION_RELWITHDEBINFO
    "C:/Github/manpwin/mpfr/build.vc15/lib/x64/Release/mpfr.lib"
    IMPORTED_LOCATION_MINSIZEREL
    "C:/Github/manpwin/mpfr/build.vc15/lib/x64/Release/mpfr.lib"
)

target_link_libraries(ManpWIN64 PRIVATE
    pnglib
    zlib
    parser
    mpeg
    qdlib
    mpfr
    htmlhelp
)