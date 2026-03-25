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

find_path(GMP_INCLUDE_DIR gmp.h)
find_library(GMP_LIBRARY gmp)

find_path(MPFR_INCLUDE_DIR mpfr.h)
find_library(MPFR_LIBRARY mpfr)

target_include_directories(parser PUBLIC
    ${GMP_INCLUDE_DIR}
    ${MPFR_INCLUDE_DIR}
)

target_link_libraries(parser PUBLIC
    ${GMP_LIBRARY}
    ${MPFR_LIBRARY}
)

target_link_libraries(ManpWIN64 PRIVATE
    pnglib
    zlib
    parser
    mpeg
    qdlib
    ${GMP_LIBRARY}
    ${MPFR_LIBRARY}
    htmlhelp
)
