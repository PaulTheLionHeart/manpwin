# =========================================================
# ManpWIN Root CMake Script
# Copyright 2026 Paul de Leeuw
# =========================================================

cmake_minimum_required(VERSION 3.23)

project(ManpWIN LANGUAGES C CXX)

# CMake helper modules
list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/cmake")

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

# GMP and MPFR use custom Find modules in /cmake because the
# current vcpkg ports do not provide CMake package configuration files.
find_package(GMP REQUIRED)
find_package(MPFR REQUIRED)

target_link_libraries(parser PUBLIC
    GMP::GMP
    MPFR::MPFR
)

target_link_libraries(ManpWIN64 PRIVATE
    pnglib
    zlib
    parser
    mpeg
    qdlib
    htmlhelp
)


