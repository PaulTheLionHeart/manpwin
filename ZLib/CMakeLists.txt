# ManpWIN CMake script
# Copyright 2026 Paul de Leeuw
#

add_library(zlib
    adler32.c
    compress.c
    crc32.c
    deflate.c
    gzclose.c
    gzlib.c
    gzread.c
    gzwrite.c
    infback.c
    inffas8664.c
    inffast.c
    inflate.c
    inftrees.c
    trees.c
    uncompr.c
    zutil.c
)

target_include_directories(zlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(zlib PROPERTIES LINKER_LANGUAGE C)