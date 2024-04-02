/*
 * DL3 Quantization
 * ================
 *
 * File: dl3quant.cpp
 * Author: Dennis Lee   E-mail: denlee@ecf.utoronto.ca
 *
 * Copyright (C) 1993-1997 Dennis Lee
 *
 * C implementation of DL3 Quantization.
 * DL3 Quantization is a 2-pass color quantizer that uses an
 * exhaustive search technique to minimize error introduced at
 * each step during palette reduction.
 *
 * I believe DL3 Quant offers the highest quality of all existing
 * color quantizers.  It is truly 'optimal' except for a few provisos.
 * These provisos and other information about DL3 Quant can be found
 * in DLQUANT.TXT, which is included in this distribution.
 *
 *
 * NOTES
 * =====
 *
 * The dithering code is based on code from the IJG's jpeg library.
 *
 * DL3 Quantization can take a long time to reduce a palette.
 * Times can range from seconds to minutes or even hours depending on
 * the image and the computer used.  This eliminates DL3 Quant for
 * typical usage unless the user has a very fast computer and/or has a
 * lot of patience.  However, the reward is a quantized image that is
 * the best currently possible.  The number of colors in the source image,
 * not the image size, determines the time required to quantize it.
 *
 * This source code may be freely copied, modified, and redistributed,
 * provided this copyright notice is attached.
 * Compiled versions of this code, modified or not, are free for
 * personal use.  Compiled versions used in distributed software
 * is also free, but a notification must be sent to the author.
 * An e-mail to denlee@ecf.utoronto.ca will do.
 *
 */

#define DL3SRC

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>
#include "manpwin.h"
#include "dl3quant.h"
#include "quanterr.h"

/********** The following are options **********/

//#define STATUS	// undef STATUS to remove all text output
#undef STATUS		// undef STATUS to remove all text output
//#define BEST_MAPPING	// improves quality (slower)
#define SIXTEEN_BIT	// 16-bit boxes (useful for YCrCb 6-5-5)

/* Bias factor for first color component */
//#define CScale   1	// For RGB: all components equal
#define CScale   2.5	// For YCrCb: Y component favored

/* define *one* of the following dither options */
//#define DITHER1	// 1-val error diffusion dither
//#define DITHER2	// 2-val error diffusion dither
#define DITHER4		// 4-val error diffusion dither (Floyd-Steinberg)

/********** End of options **********/

#define DITHER_MAX 20

//////////////////////////////////////////////////////////////////////////////////////////////////////
extern	HWND	GlobalHwnd;				// This is the main windows handle
//////////////////////////////////////////////////////////////////////////////////////////////////////

//extern	StateType	PGVState;		// what state is pgv in?

typedef struct {
    ulong r, g, b;
    ulong pixel_count;
    ulong err;
    slong cc;
    } CUBE;

LOCAL uchar palette[3][VGA_COLOURS];
LOCAL CUBE *rgb_table;
LOCAL slong tot_colors, sqr_tbl[255+255+1], *squares;

/* returns 1 on success, 0 on failure */
GLOBAL int
//dl3quant(uchar *inbuf, uchar *outbuf, int width, int height, int quant_to, int dither, uchar userpal[VGA_COLOURS][3]) 
dl3quant(uchar *inbuf, uchar *outbuf, int width, int height, int quant_to, int dither, uchar *userpal) 
    {
//    char s[300];

    if (init_table() == 0)
	return ERR_INIT_TABLE;

    build_table(inbuf, width * height);

#ifdef STATUS
    printf("Total colors in image (15-bit): %d\n", tot_colors);
    printf("Reducing palette...\n");
#endif

/*
This seems silly to ask every time. Best to just assume we always want to compress the colours

//    if (tot_colors > 600 && PGVState != BATCH && PGVState != SLIDESHOW && PGVState != ZAAPACTIVE)
    if (tot_colors > 600)
	{
	wsprintf(s, "WARNING!\nThere are %d colours in this file!\nThis will take a lot of time\nDo you wish to continue?", tot_colors - 1);
	if (MessageBox (GlobalHwnd, s, "Quantisation", MB_ICONEXCLAMATION | MB_DEFBUTTON2 | MB_YESNO) == IDNO)
	    return ERR_USER_ABORT;
	}
*/
    reduce_table(GlobalHwnd, quant_to);

    set_palette();

#ifdef STATUS
    printf("\nQuantizing image...\n");
#endif

    if (quantize_image(inbuf, outbuf, width, height, dither) == 0)
	return ERR_QUANTISE_IMAGE;

    copy_pal(userpal);
    return 0;			// normal exit
}

LOCAL void
//copy_pal(uchar userpal[VGA_COLOURS][3]) 
copy_pal(uchar *userpal) 
    {
    int i;

    for (i = 0; i < VGA_COLOURS; i++) {
//	userpal[i][0] = palette[0][i];
//	userpal[i][1] = palette[1][i];
//	userpal[i][2] = palette[2][i];
	*(userpal + i * 3 + 0) = palette[0][i];
	*(userpal + i * 3 + 1) = palette[1][i];
	*(userpal + i * 3 + 2) = palette[2][i];
    }
}

/* returns 1 on success, 0 on failure */
LOCAL int
init_table(void) {
    int i;

#ifdef SIXTEEN_BIT
    rgb_table = (CUBE *) calloc(sizeof(CUBE), 65536);
#else
    rgb_table = (CUBE *) calloc(sizeof(CUBE), 32768);
#endif
    if (rgb_table == NULL)
	return 0;

    for (i = (-255); i <= 255; i++)
	sqr_tbl[i+255] = i*i;
    squares = sqr_tbl + 255;

    return 1;
}

LOCAL void
build_table(uchar *image, int size) {
    int i, index;

    for (i = 0; i < size; i++) {
#ifdef SIXTEEN_BIT
	index = ((image[0]&252)<<8) + ((image[1]&248)<<2) + (image[2]>>3);
#else
	index = ((image[0]&248)<<7) + ((image[1]&248)<<2) + (image[2]>>3);
#endif
	rgb_table[index].r += (long)((double)image[0] * CScale);
	rgb_table[index].g += image[1];
	rgb_table[index].b += image[2];
	rgb_table[index].pixel_count++;
	image += 3;
    }

    tot_colors = 0;
#ifdef SIXTEEN_BIT
    for (i = 0; i < 65536; i++)
#else
    for (i = 0; i < 32768; i++)
#endif
	if (rgb_table[i].pixel_count)
	    rgb_table[tot_colors++] = rgb_table[i];
}

LOCAL ulong
calc_err(int c1, int c2) {
    ulong dist1, dist2, P1, P2, P3, R1, G1, B1, R2, G2, B2, R3, G3, B3;
    slong r_dist, g_dist, b_dist;

    R1 = rgb_table[c1].r;
    G1 = rgb_table[c1].g;
    B1 = rgb_table[c1].b;
    P1 = rgb_table[c1].pixel_count;

    R2 = rgb_table[c2].r;
    G2 = rgb_table[c2].g;
    B2 = rgb_table[c2].b;
    P2 = rgb_table[c2].pixel_count;

    P3 = P1 + P2;
    R3 = (R1 + R2 + (P3 >> 1)) / P3;
    G3 = (G1 + G2 + (P3 >> 1)) / P3;
    B3 = (B1 + B2 + (P3 >> 1)) / P3;

    R1 = (R1 + (P1 >> 1)) / P1;
    G1 = (G1 + (P1 >> 1)) / P1;
    B1 = (B1 + (P1 >> 1)) / P1;

    R2 = (R2 + (P2 >> 1)) / P2;
    G2 = (G2 + (P2 >> 1)) / P2;
    B2 = (B2 + (P2 >> 1)) / P2;

    r_dist = R3 - R1;
    g_dist = G3 - G1;
    b_dist = B3 - B1;
    dist1 = squares[r_dist] + squares[g_dist] + squares[b_dist];
    dist1 = (ulong)(sqrt((double)dist1) * P1);	//IB 2009-04-26 cast argument

    r_dist = R2 - R3;
    g_dist = G2 - G3;
    b_dist = B2 - B3;
    dist2 = squares[r_dist] + squares[g_dist] + squares[b_dist];
    dist2 = (ulong)(sqrt((double)dist2) * P2);	//IB 2009-04-26 cast argument

    return (dist1 + dist2);
}

LOCAL void
reduce_table(HWND hwnd, int num_colors) {
    int i, j, c1, c2, c3;
    ulong err, cur_err;
//    char s[200];
//    int	    percent, display_count, OrigColours;

//    percent = 0;
//    SetWindowText (hwnd, "Entering Quantisation Stage 1");	// Show formatted text in the caption bar
    for (i = 0; i < (tot_colors - 1); i++) {
//	display_count = (100 * i) / (tot_colors - 1);
//	if (display_count > percent)
//	    {
//	    percent = display_count;
//	    wsprintf (s, "Quantisation Stage 1: %d%%", percent);
//	    SetWindowText (hwnd, s);		// Show formatted text in the caption bar
//	    }
#ifdef STATUS
	printf("\rPass1: %5d", i + 2);
	fflush(stdout);
#endif

	err = (ulong)(~0L);
	for (j = i + 1; j < tot_colors; j++) {
	    cur_err = calc_err(i, j);
	    if (cur_err < err) {
		err = cur_err;
		c2 = j;
	    }
	}
	rgb_table[i].err = err;
	rgb_table[i].cc = c2;
    }
    rgb_table[i].err = (ulong)(~0L);
    rgb_table[i].cc = tot_colors;

//    OrigColours = tot_colors;
    while (tot_colors > num_colors) {
	err = (ulong)(~0L);

//	    wsprintf (s, "Stage 2: Reducing Colours %d Down to %d", tot_colors, num_colors);
//	    SetWindowText (hwnd, s);		// Show formatted text in the caption bar

	for (i = 0; i < tot_colors; i++) {
	    if (rgb_table[i].err < err) {
		err = rgb_table[i].err;
		c1 = i;
	    }
	}
	c2 = rgb_table[c1].cc;
	rgb_table[c2].r += rgb_table[c1].r;
	rgb_table[c2].g += rgb_table[c1].g;
	rgb_table[c2].b += rgb_table[c1].b;
	rgb_table[c2].pixel_count += rgb_table[c1].pixel_count;
	tot_colors--;

	rgb_table[c1] = rgb_table[tot_colors];
	rgb_table[tot_colors-1].err = (ulong)(~0L);
	rgb_table[tot_colors-1].cc = tot_colors;

	for (i = 0; i < c1; i++)
	    if (rgb_table[i].cc == tot_colors)
		rgb_table[i].cc = c1;
	for (i = c1 + 1; i < tot_colors; i++)
	    if (rgb_table[i].cc == tot_colors) {
		err = (ulong)(~0L);
		for (j = i + 1; j < tot_colors; j++) {
		    cur_err = calc_err(i, j);
		    if (cur_err < err) {
			err = cur_err;
			c3 = j;
		    }
		}
		rgb_table[i].err = err;
		rgb_table[i].cc = c3;
	    }
	err = (ulong)(~0L);
	for (i = c1 + 1; i < tot_colors; i++) {
	    cur_err = calc_err(i, c1);
	    if (cur_err < err) {
		err = cur_err;
		c3 = i;
	    }
	}
	rgb_table[c1].err = err;
	rgb_table[c1].cc = c3;

	for (i = 0; i < c1; i++)
	    if (rgb_table[i].cc == c1) {
		err = (ulong)(~0L);
		for (j = i + 1; j < tot_colors; j++) {
		    cur_err = calc_err(i, j);
		    if (cur_err < err) {
			err = cur_err;
			c3 = j;
		    }
		}
		rgb_table[i].err = err;
		rgb_table[i].cc = c3;
	    }
	for (i = 0; i < c1; i++) {
	    cur_err = calc_err(i, c1);
	    if (cur_err < rgb_table[i].err) {
		rgb_table[i].err = cur_err;
		rgb_table[i].cc = c1;
	    }
	}

	if (c2 != tot_colors) {
	    err = (ulong)(~0L);
	    for (i = c2 + 1; i < tot_colors; i++) {
		cur_err = calc_err(c2, i);
		if (cur_err < err) {
		    err = cur_err;
		    c3 = i;
		}
	    }
	    rgb_table[c2].err = err;
	    rgb_table[c2].cc = c3;
	    for (i = 0; i < c2; i++)
		if (rgb_table[i].cc == c2) {
		    err = (ulong)(~0L);
		    for (j = i + 1; j < tot_colors; j++) {
			cur_err = calc_err(i, j);
			if (cur_err < err) {
			    err = cur_err;
			    c3 = j;
			}
		    }
		    rgb_table[i].err = err;
		    rgb_table[i].cc = c3;
		}
	    for (i = 0; i < c2; i++) {
		cur_err = calc_err(i, c2);
		if (cur_err < rgb_table[i].err) {
		    rgb_table[i].err = cur_err;
		    rgb_table[i].cc = c2;
		}
	    }
	}
#ifdef STATUS
	printf("\rPass2: %5ld", tot_colors);
	fflush(stdout);
#endif
    }
}

LOCAL void
set_palette(void) {
    int i;
    ulong sum;

    for (i = 0; i < tot_colors; i++) {
	sum = rgb_table[i].pixel_count;
	palette[0][i] = (uchar)(((rgb_table[i].r / CScale) + (sum >> 1)) / sum);
	palette[1][i] = (uchar)((rgb_table[i].g + (sum >> 1)) / sum);
	palette[2][i] = (uchar)((rgb_table[i].b + (sum >> 1)) / sum);
    }
    free(rgb_table);
}

LOCAL int
quantize_image(uchar *in, uchar *out, int width, int height, int dither) {
    if (!dither) {
	int i, offset;
	sshort *lookup = (short *)malloc(sizeof(short) * 32768);

	if (lookup == NULL)
	    return 0;

	for (i = 0; i < 32768; i++)
	    lookup[i] = -1;

	for (i = 0; i < (width * height); i++) {
#ifdef BEST_MAPPING
	    out[i] = bestcolor(in[0], in[1], in[2]);
#else
	    offset = (in[0]&248) << 7 | (in[1]&248) << 2 | in[2] >> 3;
	    if (lookup[offset] < 0)
	       lookup[offset] = bestcolor(in[0], in[1], in[2]);
	    out[i] = (uchar)(lookup[offset]);
#endif
	    in += 3;
	}

	free(lookup);
    } else {
#if defined(DITHER2) || defined(DITHER4)
	slong i, j, r_pix, g_pix, b_pix, offset, dir, two_val,
	      odd_scanline = 0, err_len = (width + 2) * 3;
	uchar *range_tbl = (BYTE *)malloc(3 * 256), *range = range_tbl + 256;
	sshort *lookup  = (short *)malloc(sizeof(short) * 32768),
	       *erowerr = (short *)malloc(sizeof(short) * err_len),
	       *orowerr = (short *)malloc(sizeof(short) * err_len),
	       *thisrowerr, *nextrowerr;
	schar *dith_max_tbl = (schar *)malloc(512), *dith_max = dith_max_tbl + 256;

	if (range_tbl == NULL || lookup == NULL ||
	    erowerr == NULL || orowerr == NULL || dith_max_tbl == NULL) {
	    if (range_tbl != NULL)
		free(range_tbl);
	    if (lookup != NULL)
		free(lookup);
	    if (erowerr != NULL)
		free(erowerr);
	    if (orowerr != NULL)
		free(orowerr);
	    if (dith_max_tbl != NULL)
		free(dith_max_tbl);
	    return 0;
	}

	for (i = 0; i < err_len; i++)
	    erowerr[i] = 0;

	for (i = 0; i < 32768; i++)
	    lookup[i] = -1;

	for (i = 0; i < 256; i++) {
	    range_tbl[i] = 0;
	    range_tbl[i + 256] = (uchar) i;
	    range_tbl[i + 512] = 255;
	}

	for (i = 0; i < 256; i++) {
	    dith_max_tbl[i] = -DITHER_MAX;
	    dith_max_tbl[i + 256] = DITHER_MAX;
	}
	for (i = -DITHER_MAX; i <= DITHER_MAX; i++)
	    dith_max_tbl[i + 256] = (char)i;

	for (i = 0 ; i < height; i++) {
	    if (odd_scanline) {
		dir = -1;
		in  += (width - 1) * 3;
		out += (width - 1);
		thisrowerr = orowerr + 3;
		nextrowerr = erowerr + width * 3;
	    } else {
		dir = 1;
		thisrowerr = erowerr + 3;
		nextrowerr = orowerr + width * 3;
	    }
	    nextrowerr[0] = nextrowerr[1] = nextrowerr[2] = 0;
	    for (j = 0; j < width; j++) {
#ifdef DITHER2
		r_pix = range[(thisrowerr[0] >> 1) + in[0]];
		g_pix = range[(thisrowerr[1] >> 1) + in[1]];
		b_pix = range[(thisrowerr[2] >> 1) + in[2]];
#else
		r_pix = range[((thisrowerr[0] + 8) >> 4) + in[0]];
		g_pix = range[((thisrowerr[1] + 8) >> 4) + in[1]];
		b_pix = range[((thisrowerr[2] + 8) >> 4) + in[2]];
#endif
		offset = (r_pix&248) << 7 | (g_pix&248) << 2 | b_pix >> 3;
		if (lookup[offset] < 0)
		    lookup[offset] = bestcolor(r_pix, g_pix, b_pix);
		*out = (char)(lookup[offset]);
		r_pix = dith_max[r_pix - palette[0][lookup[offset]]];
		g_pix = dith_max[g_pix - palette[1][lookup[offset]]];
		b_pix = dith_max[b_pix - palette[2][lookup[offset]]];
#ifdef DITHER2
		nextrowerr[0  ]  = r_pix;
		thisrowerr[0+3] += r_pix;
		nextrowerr[1  ]  = g_pix;
		thisrowerr[1+3] += g_pix;
		nextrowerr[2  ]  = b_pix;
		thisrowerr[2+3] += b_pix;
#else
		two_val = r_pix * 2;
		nextrowerr[0-3]  = (short)r_pix;
		r_pix += two_val;
		nextrowerr[0+3] += (short)r_pix;
		r_pix += two_val;
		nextrowerr[0  ] += (short)r_pix;
		r_pix += two_val;
		thisrowerr[0+3] += (short)r_pix;
		two_val = g_pix * 2;
		nextrowerr[1-3]  = (short)g_pix;
		g_pix += two_val;
		nextrowerr[1+3] += (short)g_pix;
		g_pix += two_val;
		nextrowerr[1  ] += (short)g_pix;
		g_pix += two_val;
		thisrowerr[1+3] += (short)g_pix;
		two_val = b_pix * 2;
		nextrowerr[2-3]  = (short)b_pix;
		b_pix += two_val;
		nextrowerr[2+3] += (short)b_pix;
		b_pix += two_val;
		nextrowerr[2  ] += (short)b_pix;
		b_pix += two_val;
		thisrowerr[2+3] += (short)b_pix;
#endif
		thisrowerr += 3;
		nextrowerr -= 3;
		in  += dir * 3;
		out += dir;
	    }
	    if ((i % 2) == 1) {
		in  += (width + 1) * 3;
		out += (width + 1);
	    }
	    odd_scanline = !odd_scanline;
	}

	free(range_tbl);
	free(lookup);
	free(erowerr);
	free(orowerr);
	free(dith_max_tbl);
#else
	slong i, j, r_pix, g_pix, b_pix, r_err, g_err, b_err, offset;
	uchar *range_tbl = malloc(3 * 256), *range = range_tbl + 256;
	sshort *lookup = malloc(sizeof(short) * 32768);
	schar *dith_max_tbl = malloc(512), *dith_max = dith_max_tbl + 256;

	if (range_tbl == NULL || lookup == NULL || dith_max_tbl == NULL) {
	    if (range_tbl != NULL)
		free(range_tbl);
	    if (lookup != NULL)
		free(lookup);
	    if (dith_max_tbl != NULL)
		free(dith_max_tbl);
	    return 0;
	}

	for (i = 0; i < 32768; i++)
	    lookup[i] = -1;

	for (i = 0; i < 256; i++) {
	    range_tbl[i] = 0;
	    range_tbl[i + 256] = (uchar) i;
	    range_tbl[i + 512] = 255;
	}

	for (i = 0; i < 256; i++) {
	    dith_max_tbl[i] = -DITHER_MAX;
	    dith_max_tbl[i + 256] = DITHER_MAX;
	}
	for (i = -DITHER_MAX; i <= DITHER_MAX; i++)
	    dith_max_tbl[i + 256] = (char)i;

	for (i = 0; i < height; i++) {
	    r_err = g_err = b_err = 0;
	    for (j = width - 1; j >= 0; j--) {
		r_pix = range[(r_err >> 1) + in[0]];
		g_pix = range[(g_err >> 1) + in[1]];
		b_pix = range[(b_err >> 1) + in[2]];
		offset = (r_pix&248) << 7 | (g_pix&248) << 2 | b_pix >> 3;
		if (lookup[offset] < 0)
		    lookup[offset] = bestcolor(r_pix, g_pix, b_pix);
		*out++ = (uchar)(lookup[offset]);
		r_err = dith_max[r_pix - palette[0][lookup[offset]]];
		g_err = dith_max[g_pix - palette[1][lookup[offset]]];
		b_err = dith_max[b_pix - palette[2][lookup[offset]]];
		in += 3;
	    }
	}

	free(range_tbl);
	free(lookup);
	free(dith_max_tbl);
#endif
    }
    return 1;
}

LOCAL int
bestcolor(int r, int g, int b) {
    ulong i, bestcolor, curdist, mindist;
    slong rdist, gdist, bdist;

#ifndef BEST_MAPPING
    r = (r & 248) + 4;
    g = (g & 248) + 4;
    b = (b & 248) + 4;
#endif
    mindist = 200000;
    for (i = 0; (long)i < tot_colors; i++) {
	rdist = palette[0][i] - r;
	gdist = palette[1][i] - g;
	bdist = palette[2][i] - b;
	curdist = squares[rdist] + squares[gdist] + squares[bdist];
	if (curdist < mindist) {
	    mindist = curdist;
	    bestcolor = i;
	}
    }
    return bestcolor;
}
