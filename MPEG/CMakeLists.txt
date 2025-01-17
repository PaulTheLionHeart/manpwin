# ManpWIN CMake script
# Copyright 2025 Paul de Leeuw
#

add_library(mpeg 
config.h
conform.cpp
fdctref.cpp
global.h
idct.cpp
motion.cpp
mpeg2enc.cpp
mpeg2enc.h
predict.cpp
putbits.cpp
puthdr.cpp
putmpg.cpp
putpic.cpp
putseq.cpp
putvlc.cpp
quantize.cpp
ratectl.cpp
readpic.cpp
stats.cpp
transfrm.cpp
vlc.h
writepic.cpp
)
