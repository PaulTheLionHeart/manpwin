# ManpWIN CMake script
# Copyright 2026 Paul de Leeuw
#

add_library(pnglib STATIC
    png.c
    pngerror.c
    pngget.c
    pngmem.c
    pngpread.c
    pngread.c
    pngrio.c
    pngrtran.c
    pngrutil.c
    pngset.c
    pngtrans.c
    pngwio.c
    pngwrite.c
    pngwtran.c
    pngwutil.c
)

target_include_directories(pnglib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

