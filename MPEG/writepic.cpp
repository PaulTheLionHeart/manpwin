/* writepic.cpp, write reconstructed pictures                                 */

/* Copyright (C) 1996, MPEG Software Simulation Group. All Rights Reserved. */

/*
 * Disclaimer of Warranty
 *
 * These software programs are available to the user without any license fee or
 * royalty on an "as is" basis.  The MPEG Software Simulation Group disclaims
 * any and all warranties, whether express, implied, or statuary, including any
 * implied warranties or merchantability or of fitness for a particular
 * purpose.  In no event shall the copyright-holder be liable for any
 * incidental, punitive, or consequential damages of any kind whatsoever
 * arising from the use of these programs.
 *
 * This disclaimer of warranty extends to the user of these programs and user's
 * customers, employees, agents, transferees, successors, and assigns.
 *
 * The MPEG Software Simulation Group does not represent or warrant that the
 * programs furnished hereunder are free of infringement of any third-party
 * patents.
 *
 * Commercial implementations of MPEG-1 and MPEG-2 video, including shareware,
 * are subject to royalty fees to patent holders.  Many of these patents are
 * general enough such that they are unavoidable regardless of implementation
 * design.
 *
 */

 // let's face it, this is legacy code, so let's shut up the silly warnings
#pragma warning(push)
#pragma warning(disable: 4701 4706)

#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "global.h"
#include "..\ManpWIN64\SafeStrings.h"

void writeframe(char *fname, unsigned char *frame[])

{
  int chrom_hsize, chrom_vsize;
  char name[128];
  FILE *fd;

  chrom_hsize = (chroma_format==CHROMA444) ? horizontal_size
                                           : horizontal_size>>1;

  chrom_vsize = (chroma_format!=CHROMA420) ? vertical_size
                                           : vertical_size>>1;

  if (fname[0]=='-')
    return;

  /* Y */
  _snprintf_s(name, 128, _TRUNCATE,"%s.Y",fname);
  if (!(fd = fopen(name,"wb")))
    {
    _snprintf_s(errortext, 256, _TRUNCATE,"Couldn't create %s\n",name);
    error(errortext);
    }
  fwrite(frame[0],1,horizontal_size*vertical_size,fd);
  fclose(fd);

  /* Cb */
  SAFE_SPRINTF(name,"%s.U",fname);
  if (!(fd = fopen(name,"wb")))
  {
    _snprintf_s(errortext, 256, _TRUNCATE,"Couldn't create %s\n",name);
    error(errortext);
  }
  fwrite(frame[1],1,chrom_hsize*chrom_vsize,fd);
  fclose(fd);

  /* Cr */
  SAFE_SPRINTF(name,"%s.V",fname);
  if (!(fd = fopen(name,"wb")))
  {
    _snprintf_s(errortext, 256, _TRUNCATE,"Couldn't create %s\n",name);
    error(errortext);
  }
  fwrite(frame[2],1,chrom_hsize*chrom_vsize,fd);
  fclose(fd);
}

#pragma warning(pop)

