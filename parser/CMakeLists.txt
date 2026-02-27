# ManpWIN CMake script
# Copyright 2026 Paul de Leeuw
#

# Parser library

add_library(parser STATIC
    addstuff.cpp
    DDTrigFn.cpp
    fpu087.cpp
    parser.cpp
    ParserFn.cpp
    ParserCtx.cpp
    QDTrigFn.cpp
    TrigFn.cpp
)

target_include_directories(parser PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/ManpWIN64
)

