/*
 * File: dl3quant.h
 *
 * Header file for dl3quant.c (DL3 Quantization)
 *
 * Copyright (C) 1993-1997 Dennis Lee
 */

#ifndef DL3QUANT_H
#define DL3QUANT_H

#include <windows.h>
#include "quanterr.h"


#define schar               signed char
#define sshort              signed short
#define slong               signed long
#define uchar               unsigned char
#define ushort              unsigned short
#define ulong               unsigned long

#ifndef NULL
#define NULL                0
#endif
#ifndef SEEK_SET
#define SEEK_SET            0
#endif
#ifndef SEEK_CUR
#define SEEK_CUR            1
#endif
#ifndef SEEK_END
#define SEEK_END            2
#endif
#define IMPORT              extern
#define EXPORT
#define LOCAL               static
#define GLOBAL


extern	GLOBAL int   dl3quant(uchar *inbuf, uchar *outbuf, int width, int height,
		      int quant_to, int dither, uchar userpal[VGA_COLOURS][3]);

#ifdef DL3SRC
LOCAL  void copy_pal(uchar *userpal);
//LOCAL  void  copy_pal(uchar userpal[VGA_COLOURS][3]);
LOCAL  int   init_table(void);
LOCAL  void  build_table(uchar *image, int size);
LOCAL  ulong calc_err(int, int);
LOCAL  void  reduce_table(HWND hwnd, int num_colors);
LOCAL  void  set_palette(void);
LOCAL  int   quantize_image(uchar *inbuf, uchar *outbuf, int width,
			    int height, int dither);
LOCAL  int   bestcolor(int r, int g, int b);
#endif

#endif
