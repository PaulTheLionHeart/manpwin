/*
 * DL1 Quantization
 * ================
 *
 * File: dl1quant.cpp
 * Author: Dennis Lee   E-mail: denlee@ecf.utoronto.ca
 *
 * Copyright (C) 1993-1997 Dennis Lee
 *
 * C implementation of DL1 Quantization.
 * DL1 Quantization is a 2-pass color quantizer optimized for speed.
 * The method was designed around the steps required by a 2-pass
 * quantizer and constructing a model that would require the least
 * amount of extra work.  The resulting method is extremely fast --
 * about half the speed of a memcpy.  That should make DL1 Quant the
 * fastest 2-pass color quantizer.
 *
 * This quantizer's quality is also among the best, slightly
 * better than Wan et al's marginal variance based quantizer.  For
 * more on DL1 Quant's performance and other related information,
 * see DLQUANT.TXT included in this distribution.
 *
 *
 * NOTES
 * =====
 *
 * The dithering code is based on code from the IJG's jpeg library.
 *
 * This source code may be freely copied, modified, and redistributed,
 * provided this copyright notice is attached.
 * Compiled versions of this code, modified or not, are free for
 * personal use.  Compiled versions used in distributed software
 * is also free, but a notification must be sent to the author.
 * An e-mail to denlee@ecf.utoronto.ca will do.
 *
 */

#define DL1SRC

//#include <stdlib.h>
#include "dl1quant.h"
//#include <windows.h>
//#include "quanterr.h"

/********** The following are options **********/

//#define FAST		// improves speed but uses a lot of memory
//#define QUAL1		// improves quality slightly (slower)
#define QUAL2		// improves quality slightly (slower)

/* define *one* of the following dither options */
//#define DITHER1	// 1-val error diffusion dither
//#define DITHER2	// 2-val error diffusion dither
#define DITHER4		// 4-val error diffusion dither (Floyd-Steinberg)

/********** End of options **********/
#ifdef	FRED
//////////////////////////////////////////////////////////////////////////////////////////////////////
extern	HWND	GlobalHwnd;			// This is the main windows handle
//////////////////////////////////////////////////////////////////////////////////////////////////////

#define DITHER_MAX  20

static uchar palette[3][VGA_COLOURS];
static CUBE *rgb_table[6];
static ushort r_offset[VGA_COLOURS], g_offset[VGA_COLOURS], b_offset[VGA_COLOURS];
static CLOSEST_INFO c_info;
static int tot_colors, pal_index, did_init = 0;
static ulong *squares;
static FCUBE *heap = NULL;
static short *dl_image = NULL;

// returns 1 on success, 0 on failure
// This has ben changed to allow many error reports to be returned as negative numbers

//dl1quant(uchar *inbuf, uchar *outbuf, int width, int height, int quant_to, int dither, uchar userpal[VGA_COLOURS][3]) 
int	dl1quant(uchar *inbuf, uchar *outbuf, int width, int height, int quant_to, int dither, uchar *userpal) 
    {
    // Show formatted text in the caption bar
//    SetWindowText (GlobalHwnd, "Quantisation Initialisation");
    if (!did_init) {
	did_init = 1;
	dlq_init();
    }
    if (dlq_start() == 0) {
	dlq_finish();
	return ERR_DCQ_START;
    }
//    SetWindowText (GlobalHwnd, "Building Quantisation Table");
    if (build_table(inbuf, (ulong)width * (ulong)height) == 0) {
	dlq_finish();
	return ERR_BUILD_TABLE;
    }

    reduce_table(GlobalHwnd, quant_to);
    set_palette(0, 0);
//    SetWindowText (GlobalHwnd, "Quantising Image");
    if (quantize_image(inbuf, outbuf, width, height, dither) == 0) {
	dlq_finish();
	return ERR_QUANTISE_IMAGE;
    }
    dlq_finish();
    copy_pal(userpal);

    return 0;			// normal exit
}

static void	copy_pal(uchar *userpal) 
//copy_pal(uchar userpal[VGA_COLOURS][3]) 
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

static void	dlq_init(void) {
    int i;

    for (i = 0; i < VGA_COLOURS; i++) {
	r_offset[i] = (i & 128) << 7 | (i & 64) << 5 | (i & 32) << 3 |
		      (i & 16)  << 1 | (i & 8)  >> 1;
	g_offset[i] = (i & 128) << 6 | (i & 64) << 4 | (i & 32) << 2 |
		      (i & 16)  << 0 | (i & 8)  >> 2;
	b_offset[i] = (i & 128) << 5 | (i & 64) << 3 | (i & 32) << 1 |
		      (i & 16)  >> 1 | (i & 8)  >> 3;
    }

    for (i = (-255); i <= 255; i++)
	c_info.squares[i+255] = i*i;
    squares = c_info.squares + 255;
}

/* returns 1 on success, 0 on failure */
static int	dlq_start(void) {
    int i;

    rgb_table[0] = (CUBE *) calloc(sizeof(CUBE), 1);
    rgb_table[1] = (CUBE *) calloc(sizeof(CUBE), 8);
    rgb_table[2] = (CUBE *) calloc(sizeof(CUBE), 64);
    rgb_table[3] = (CUBE *) calloc(sizeof(CUBE), 512);
    rgb_table[4] = (CUBE *) calloc(sizeof(CUBE), 4096);
    rgb_table[5] = (CUBE *) calloc(sizeof(CUBE), 32768);

    for (i = 0; i <= 5; i++)
	if (rgb_table[i] == NULL)
	    return 0;

    pal_index = 0;
    return 1;
}

static void	dlq_finish(void) {
    if (rgb_table[0] != NULL)
	free(rgb_table[0]);
    if (rgb_table[1] != NULL)
	free(rgb_table[1]);
    if (rgb_table[2] != NULL)
	free(rgb_table[2]);
    if (rgb_table[3] != NULL)
	free(rgb_table[3]);
    if (rgb_table[4] != NULL)
	free(rgb_table[4]);
    if (rgb_table[5] != NULL)
	free(rgb_table[5]);
    if (heap != NULL)
	free(heap);
    if (dl_image != NULL)
	free(dl_image);
}

/* returns 1 on success, 0 on failure */
static int	build_table(uchar *image, ulong pixels) {
    ulong i, index, cur_count, head, tail;
    slong j;

    heap = (FCUBE *) malloc(sizeof(FCUBE) * 32769);
    if (heap == NULL)
	return 0;

#ifdef FAST
    dl_image = malloc(sizeof(short) * pixels);
    if (dl_image == NULL)
	return 0;
#endif

    for (i = 0; i < pixels; i++) {
#ifdef FAST
	dl_image[i] = index = r_offset[image[0]] + g_offset[image[1]] + b_offset[image[2]];
#else
	index = r_offset[image[0]] + g_offset[image[1]] + b_offset[image[2]];
#endif
#ifdef QUAL1
	rgb_table[5][index].r += image[0];
	rgb_table[5][index].g += image[1];
	rgb_table[5][index].b += image[2];
#endif
	rgb_table[5][index].pixel_count++;
	image += 3;
    }

    tot_colors = 0;
    for (i = 0; i < 32768; i++) {
	cur_count = rgb_table[5][i].pixel_count;
	if (cur_count) {
	    heap[++tot_colors].level = 5;
	    heap[tot_colors].index = (ushort)i;
	    rgb_table[5][i].pixels_in_cube = cur_count;
#ifndef QUAL1
	    rgb_table[5][i].r = cur_count * (((i & 0x4000) >> 7 |
				(i & 0x0800) >> 5 | (i & 0x0100) >> 3 |
				(i & 0x0020) >> 1 | (i & 0x0004) << 1) + 4);
	    rgb_table[5][i].g = cur_count * (((i & 0x2000) >> 6 |
				(i & 0x0400) >> 4 | (i & 0x0080) >> 2 |
				(i & 0x0010) >> 0 | (i & 0x0002) << 2) + 4);
	    rgb_table[5][i].b = cur_count * (((i & 0x1000) >> 5 |
				(i & 0x0200) >> 3 | (i & 0x0040) >> 1 |
				(i & 0x0008) << 1 | (i & 0x0001) << 3) + 4);
#endif
	    head = i;
	    for (j = 4; j >= 0; j--) {
		tail = head & 0x7;
		head >>= 3;
		rgb_table[j][head].pixels_in_cube += cur_count;
		rgb_table[j][head].children |= 1 << tail;
	    }
	}
    }

    for (i = tot_colors; i > 0; i--)
	fixheap(i);

    return 1;
}

static void	fixheap(ulong id) {
    uchar thres_level = heap[id].level;
    ulong thres_index = heap[id].index, index, half_totc = tot_colors >> 1,
	  thres_val = rgb_table[thres_level][thres_index].pixels_in_cube;

    while (id <= half_totc) {
	index = id << 1;

	if (index < (ulong)tot_colors)
	    if (rgb_table[heap[index].level][heap[index].index].pixels_in_cube
	      > rgb_table[heap[index+1].level][heap[index+1].index].pixels_in_cube)
		index++;

	if (thres_val <= rgb_table[heap[index].level][heap[index].index].pixels_in_cube)
	    break;
	else {
	    heap[id] = heap[index];
	    id = index;
	}
    }
    heap[id].level = thres_level;
    heap[id].index = (ushort)thres_index;
}

static void	reduce_table(HWND hwnd, int num_colors) {
    char	s[200];
    short	percent, display_count, OrigColours;

    while (tot_colors > num_colors) {
	uchar tmp_level = heap[1].level, t_level = tmp_level - 1;
	ulong tmp_index = heap[1].index, t_index = tmp_index >> 3;

	percent = 0;
	OrigColours = tot_colors;
	display_count = (10 * (OrigColours - tot_colors)) / OrigColours;
	if (display_count > percent)
	    {
	    percent = display_count;
//	    wsprintf (s, "Quantisation: %d%%", percent * 10);
//	    SetWindowText (hwnd, s);		// Show formatted text in the caption bar
	    }

	if (rgb_table[t_level][t_index].pixel_count)
	    heap[1] = heap[tot_colors--];
	else {
	    heap[1].level = t_level;
	    heap[1].index = (ushort)t_index;
	}
	rgb_table[t_level][t_index].pixel_count += rgb_table[tmp_level][tmp_index].pixel_count;
	rgb_table[t_level][t_index].r += rgb_table[tmp_level][tmp_index].r;
	rgb_table[t_level][t_index].g += rgb_table[tmp_level][tmp_index].g;
	rgb_table[t_level][t_index].b += rgb_table[tmp_level][tmp_index].b;
	rgb_table[t_level][t_index].children &= ~(1 << (tmp_index & 0x7));
	fixheap(1);
    }
}

static void	set_palette(int index, int level) {
    int i;

    if (rgb_table[level][index].children)
	for (i = 7; i >= 0; i--)
	    if (rgb_table[level][index].children & (1 << i))
		set_palette((index << 3) + i, level + 1);

    if (rgb_table[level][index].pixel_count) {
	ulong r_sum, g_sum, b_sum, sum;

	rgb_table[level][index].palette_index = pal_index;
	r_sum = rgb_table[level][index].r;
	g_sum = rgb_table[level][index].g;
	b_sum = rgb_table[level][index].b;
	sum = rgb_table[level][index].pixel_count;
	palette[0][pal_index] = (uchar)((r_sum + (sum >> 1)) / sum);
	palette[1][pal_index] = (uchar)((g_sum + (sum >> 1)) / sum);
	palette[2][pal_index] = (uchar)((b_sum + (sum >> 1)) / sum);
	pal_index++;
    }
}

static void	closest_color(int index, int level) {
    int i;

    if (rgb_table[level][index].children)
	for (i = 7; i >= 0; i--)
	    if (rgb_table[level][index].children & (1 << i))
		closest_color((index << 3) + i, level + 1);

    if (rgb_table[level][index].pixel_count) {
	slong dist, r_dist, g_dist, b_dist;
	uchar pal_num = rgb_table[level][index].palette_index;

	/* Determine if this color is "closest". */
	r_dist = palette[0][pal_num] - c_info.red;
	g_dist = palette[1][pal_num] - c_info.green;
	b_dist = palette[2][pal_num] - c_info.blue;
	dist = squares[r_dist] + squares[g_dist] + squares[b_dist];
	if (dist < (slong)c_info.distance) {
	    c_info.distance = dist;
	    c_info.palette_index = pal_num;
	}
    }
}

/* returns 1 on success, 0 on failure */
static int	quantize_image(uchar *in, uchar *out, int width, int height, int dither) {
    if (!dither) {
	ulong i, pixels = width * height;
#ifndef QUAL2
	ushort level, index;
	uchar cube;
#endif
	uchar tmp_r, tmp_g, tmp_b, *lookup;

	lookup = (BYTE *)malloc(sizeof(char) * 32768);
	if (lookup == NULL)
	    return 0;

	for (i = 0; i < 32768; i++)
	    if (rgb_table[5][i].pixel_count) {
		tmp_r = (uchar)((i & 0x4000) >> 7 | (i & 0x0800) >> 5 |
			(i & 0x0100) >> 3 | (i & 0x0020) >> 1 |
			(i & 0x0004) << 1);
		tmp_g = (uchar)((i & 0x2000) >> 6 | (i & 0x0400) >> 4 |
			(i & 0x0080) >> 2 | (i & 0x0010) >> 0 |
			(i & 0x0002) << 2);
		tmp_b = (uchar)((i & 0x1000) >> 5 | (i & 0x0200) >> 3 |
			(i & 0x0040) >> 1 | (i & 0x0008) << 1 |
			(i & 0x0001) << 3);
#ifdef QUAL2
		lookup[i] = bestcolor(tmp_r, tmp_g, tmp_b);
#else
		c_info.red   = tmp_r + 4;
		c_info.green = tmp_g + 4;
		c_info.blue  = tmp_b + 4;
		level = 0;
		index = 0;
		for (;;) {
		    cube = (tmp_r&128) >> 5 | (tmp_g&128) >> 6 | (tmp_b&128) >> 7;
		    if ((rgb_table[level][index].children & (1 << cube)) == 0) {
			c_info.distance = ~0L;
			closest_color(index, level);
			lookup[i] = c_info.palette_index;
			break;
		    }
		    level++;
		    index = (index << 3) + cube;
		    tmp_r <<= 1;
		    tmp_g <<= 1;
		    tmp_b <<= 1;
		}
#endif
	    }

	for (i = 0; i < pixels; i++) {
#ifdef FAST
	    out[i] = lookup[dl_image[i]];
#else
	    out[i] = lookup[r_offset[in[0]] + g_offset[in[1]] + b_offset[in[2]]];
	    in += 3;
#endif
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
		*out = (uchar)lookup[offset];
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

	if (range_tbl == NULL || lookup == NULL) {
	    if (range_tbl != NULL)
		free(range_tbl);
	    if (lookup != NULL)
		free(lookup);
	    return 0;
	}

	for (i = 0; i < 32768; i++)
	    lookup[i] = -1;

	for (i = 0; i < 256; i++) {
	    range_tbl[i] = 0;
	    range_tbl[i + 256] = (uchar) i;
	    range_tbl[i + 512] = 255;
	}

	for (i = 0; i < height; i++) {
	    r_err = g_err = b_err = 0;
	    for (j = width - 1; j >= 0; j--) {
		r_pix = range[(r_err >> 1) + in[0]];
		g_pix = range[(g_err >> 1) + in[1]];
		b_pix = range[(b_err >> 1) + in[2]];
		offset = (r_pix&248) << 7 | (g_pix&248) << 2 | b_pix >> 3;
		if (lookup[offset] < 0)
		    lookup[offset] = bestcolor(r_pix, g_pix, b_pix);
		*out++ = lookup[offset];
		r_err = r_pix - palette[0][lookup[offset]];
		g_err = g_pix - palette[1][lookup[offset]];
		b_err = b_pix - palette[2][lookup[offset]];
		in += 3;
	    }
	}

	free(range_tbl);
	free(lookup);
#endif
    }
    return 1;
}

static int	bestcolor(int r, int g, int b) {
    ulong i, bestcolor, curdist, mindist;
    slong rdist, gdist, bdist;

    r = (r & 248) + 4;
    g = (g & 248) + 4;
    b = (b & 248) + 4;
    mindist = 200000;
    for (i = 0; i < (ulong)tot_colors; i++) {
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
#endif
