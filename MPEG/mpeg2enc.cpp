/* mpeg2enc.cpp, main() and parameter file reading                            */

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

#define GLOBAL /* used by global.h */
#include "config.h"
#include "global.h"

/* private prototypes */
static void init _ANSI_ARGS_((void));
// static void readparmfile _ANSI_ARGS_((char *fname)); 		PHD 2012/04/24
static void readparmfile (int, int, int);
static void readquantmat _ANSI_ARGS_((void));
	
////////////////////////////////////////////////////////////////	PHD 2012/04/24
	jmp_buf	mark;				// Address for long jump to jump to when an error occurs
	char	ErrorMessage[240];		// capture the error message for ron
	void	CloseMPEGPtrs(void);		// give the MPEG memory back
extern	void	WarningMPEG(char *);
	enum	encoding  {PAL, NTSC, MPEG1};
static	encoding    format = PAL;
extern	char	MPGPath[];
////////////////////////////////////////////////////////////////	PHD 2012/04/24

		// replace main with DoMPEG()	PHD 2012/04/24
int	DoMPEG(char *MPEGFile, int NumFrames, int width, int height)
    {
    quiet = 1;						// suppress warnings = 1 else = 0

  // read parameter file
    readparmfile(NumFrames, width, height);		// note that we have taken the parameters from the PAL encoding scripts

  // read quantization matrices
    readquantmat();

    outfile = NULL;
    statfile = NULL;
  // open output file 
    if (!(outfile=fopen(MPEGFile,"wb")))
	{
	sprintf(errortext,"Couldn't create output file %s",MPEGFile);
	error(errortext);
	}

    init();
    putseq();

    CloseMPEGPtrs();
    fclose(outfile);
    fclose(statfile);

    return 0;
    }

void	CloseMPEGPtrs(void)		// give the MPEG memory back
    {
    int	i;

    if (blocks)
	{
	delete [] blocks;
	blocks = NULL;
	}
    if (mbinfo)
	{
	delete [] mbinfo;
	mbinfo = NULL;
	}
    for (i = 0; i < 3; i++)
	{
	if(newrefframe[i])
	    {
	    delete [] newrefframe[i];
	    newrefframe[i] = NULL;
	    }
	if(oldrefframe[i])
	    {
	    delete [] oldrefframe[i];
	    oldrefframe[i] = NULL;
	    }
	if(auxframe[i])
	    {
	    delete [] auxframe[i];
	    auxframe[i] = NULL;
	    }
	if(neworgframe[i])
	    {
	    delete [] neworgframe[i];
	    neworgframe[i] = NULL;
	    }
	if(oldorgframe[i])
	    {
	    delete [] oldorgframe[i];
	    oldorgframe[i] = NULL;
	    }
	if(auxorgframe[i])
	    {
	    delete [] auxorgframe[i];
	    auxorgframe[i] = NULL;
	    }
	if(predframe[i])
	    {
	    delete [] predframe[i];
	    predframe[i] = NULL;
	    }
	}
   }

static void init()
{
  int i, size;
  static int block_count_tab[3] = {6,8,12};

  initbits();
  init_fdct();
  init_idct();

  /* round picture dimensions to nearest multiple of 16 or 32 */
  mb_width = (horizontal_size+15)/16;
  mb_height = prog_seq ? (vertical_size+15)/16 : 2*((vertical_size+31)/32);
  mb_height2 = fieldpic ? mb_height>>1 : mb_height; /* for field pictures */
  FrameWidth = 16*mb_width;
  FrameHeight = 16*mb_height;

  chrom_width = (chroma_format==CHROMA444) ? FrameWidth : FrameWidth>>1;
  chrom_height = (chroma_format!=CHROMA420) ? FrameHeight : FrameHeight>>1;

  height2 = fieldpic ? FrameHeight>>1 : FrameHeight;
  width2 = fieldpic ? FrameWidth<<1 : FrameWidth;
  chrom_width2 = fieldpic ? chrom_width<<1 : chrom_width;
  
  block_count = block_count_tab[chroma_format-1];

  // set all the pointers to NULL to help releasing memory later
  //clp	 = NULL;
  blocks = NULL;
  mbinfo = NULL;
  for (i=0; i<3; i++)
      {
      newrefframe[i] = NULL;
      oldrefframe[i] = NULL;
      auxframe[i]    = NULL;
      neworgframe[i] = NULL;
      oldorgframe[i] = NULL;
      auxorgframe[i] = NULL;
      predframe[i]   = NULL;
      }

  /* clip table */
//  clp = new unsigned char[1024];
//  if(clp == NULL)
//  if (!(clp = (unsigned char *)malloc(1024)))
//    error("malloc failed\n");
  clp = ClippingTable + 384;
//  clp+= 384;
  for (i=-384; i<640; i++)
    clp[i] = (i<0) ? 0 : ((i>255) ? 255 : i);

  for (i=0; i<3; i++)
  {
    size = (i==0) ? FrameWidth*FrameHeight : chrom_width*chrom_height;

    newrefframe[i] = new unsigned char[size];
    if(newrefframe[i] == NULL)
//    if (!(newrefframe[i] = (unsigned char *)malloc(size)))
      error("malloc failed\n");
    oldrefframe[i] = new unsigned char[size];
    if(oldrefframe[i] == NULL)
//    if (!(oldrefframe[i] = (unsigned char *)malloc(size)))
      error("malloc failed\n");
    auxframe[i] = new unsigned char[size];
    if(auxframe[i] == NULL)
//    if (!(auxframe[i] = (unsigned char *)malloc(size)))
      error("malloc failed\n");
    neworgframe[i] = new unsigned char[size];
    if(neworgframe[i] == NULL)
//    if (!(neworgframe[i] = (unsigned char *)malloc(size)))
      error("malloc failed\n");
    oldorgframe[i] = new unsigned char[size];
    if(oldorgframe[i] == NULL)
//    if (!(oldorgframe[i] = (unsigned char *)malloc(size)))
      error("malloc failed\n");
    auxorgframe[i] = new unsigned char[size];
    if(auxorgframe[i] == NULL)
//    if (!(auxorgframe[i] = (unsigned char *)malloc(size)))
      error("malloc failed\n");
    predframe[i] = new unsigned char[size];
    if(predframe[i] == NULL)
//    if (!(predframe[i] = (unsigned char *)malloc(size)))
      error("malloc failed\n");
  }

  mbinfo = (struct mbinfo *)new unsigned char[mb_width*mb_height2*sizeof(struct mbinfo)];
//  mbinfo = (struct mbinfo *)malloc(mb_width*mb_height2*sizeof(struct mbinfo));

  if (!mbinfo)
    error("malloc failed\n");

  blocks = (short (*)[64])new unsigned char[mb_width*mb_height2*block_count*sizeof(short [64])];
//  blocks =
//    (short (*)[64])malloc(mb_width*mb_height2*block_count*sizeof(short [64]));

  if (!blocks)
    error("malloc failed\n");

  /* open statistics output file */
  if (statname[0]=='-')
    statfile = stdout;
  else if (!(statfile = fopen(statname,"w")))
  {
    sprintf(errortext,"Couldn't create statistics output file %s",statname);
    error(errortext);
  }
}

void error(char *text)			// modified to jump back to MPEGWrite.cpp
{
  sprintf(ErrorMessage,text);
//  putc('\n',stderr);
  longjmp(mark, 1);			// Return control to the setjmp point
//  exit(1);
}


static void readparmfile(int NumFrames, int width, int height)
//char *fname;
{
  int i;
  int h,m,s,f;
//  FILE *fd;
//  char line[256];
  static double ratetab[8]=
    {24000.0/1001.0,24.0,25.0,30000.0/1001.0,30.0,50.0,60000.0/1001.0,60.0};
  char	t[240];					    // for warnings
  extern int /*r,*/ReactionParameter, Xi,Xb,Xp,d0i,d0p,d0b; // rate control 
  extern double avg_act; // rate control 

		// replace some of readparmfile() to incorprate defaults for PAL	PHD 2012/04/24

  strcpy(id_string, ((format == MPEG1) ? "MPEG-1 ManpWIN, 25 frames/sec" : "MPEG-2 ManpWIN, 25 frames/sec"));
  strcpy(tplorg,"name of source files");
  strcpy(tplref,"-");					// default
  strcpy(iqname,"-");					// default
  strcpy(niqname,"-");					// default
  sprintf(statname,"%s\\Stats.txt", MPGPath);		// somewhere to dump stats
  inputtype = 3;					// Type = DIB. PPM but not actually used. Somehow we try to copy in the ANIM frame DIB array
  nframes = NumFrames;					// supplied by user
  frame0 = 0;						// number of first frame
  h = 0; m = 0; s = 0; f = 0;				// timecode of first frame
  N = (format == NTSC) ? 15 : 12;			// N (# of frames in GOP)
//  M = 3;						// M (I/P frame distance)
  M = 1;						// M (I/P frame distance)   changed to 1 as the reference frames seem to cause trouble.
  mpeg1 = (format == MPEG1) ? 1 : 0;			// ISO/IEC 11172-2 stream
  fieldpic = 0;						// 0:frame pictures, 1:field pictures 
  horizontal_size = width;
  vertical_size = height;
  aspectratio = (format == MPEG1) ? 8 : 3;		// aspect_ratio_information 1=square pel, 2=4:3, 3=16:9, 4=2.11:1, 8=CCIR601 625 line, 9=CCIR601 525 line
  frame_rate_code = (format == NTSC) ? 5 : 3;		// frame_rate_code 1=23.976, 2=24, 3=25, 4=29.97, 5=30 frames/sec.
  bit_rate = (format == MPEG1) ? 1152000.0 : 5000000.0;	// bit_rate (bits/s) 
  vbv_buffer_size = (format == MPEG1) ? 20 : 112;	// vbv_buffer_size (in multiples of 16 kbit) 
  low_delay = 0;					// low_delay 
  constrparms = (format == MPEG1) ? 1 : 0;		// constrained_parameters_flag
  profile = 4;						// Profile ID: Simple = 5, Main = 4, SNR = 3, Spatial = 2, High = 1
  level = 4;						// Level ID:   Low = 10, Main = 8, High 1440 = 6, High = 4    
  prog_seq = (format == MPEG1) ? 1 : 0;			// progressive_sequence
  chroma_format = 1;					// chroma_format: 1=4:2:0, 2=4:2:2, 3=4:4:4
  video_format = (format == NTSC) ? 2 : 1;		// video_format: 0=comp., 1=PAL, 2=NTSC, 3=SECAM, 4=MAC, 5=unspec.
  color_primaries = 5;					// color_primaries
  transfer_characteristics = 5;				// transfer_characteristics
  matrix_coefficients = (format == NTSC) ? 4 : 5;	// matrix_coefficients
  display_horizontal_size = width;			// display_horizontal_size
  display_vertical_size = height;			// display_vertical_size
  dc_prec = 0;						// intra_dc_precision (0: 8 bit, 1: 9 bit, 2: 10 bit, 3: 11 bit
  topfirst = (format == MPEG1) ? 0 : 1;			// top_field_first
  frame_pred_dct_tab[0] = (format == MPEG1) ? 1 : 0;	// frame_pred_frame_dct (I P B)
  frame_pred_dct_tab[1] = (format == MPEG1) ? 1 : 0;
  frame_pred_dct_tab[2] = (format == MPEG1) ? 1 : 0;
  conceal_tab[0] = 0;	    				// concealment_motion_vectors (I P B) 
  conceal_tab[1] = 0;
  conceal_tab[2] = 0;
  qscale_tab[0] = (format == MPEG1) ? 0 : 1;		// q_scale_type  (I P B)
  qscale_tab[1] = (format == MPEG1) ? 0 : 1;
  qscale_tab[2] = (format == MPEG1) ? 0 : 1;
  intravlc_tab[0] = (format == MPEG1) ? 0 : 1;		// intra_vlc_format (I P B)
  intravlc_tab[1] = 0;
  intravlc_tab[2] = 0;
  altscan_tab[0] = 0;	    				// alternate_scan (I P B)
  altscan_tab[1] = 0;
  altscan_tab[2] = 0;
  repeatfirst = 0;					// repeat_first_field 
  prog_frame = (format == MPEG1) ? 1 : 0;		// progressive_frame
// intra slice interval refresh period 
  P = 0;						// P distance between complete intra slice refresh
  ReactionParameter = 0;				// rate control: r (reaction parameter)	renamed to ReactionParameter to avoid global conflict
  avg_act = 0;						// rate control: avg_act (initial average activity)
  Xi = 0;						// rate control: Xi (initial I frame global complexity measure)
  Xp = 0;						// rate control: Xp (initial P frame global complexity measure)
  Xb = 0;						// rate control: Xb (initial B frame global complexity measure)
  d0i = 0;						// rate control: d0i (initial I frame virtual buffer fullness)
  d0p = 0;						// rate control: d0p (initial P frame virtual buffer fullness)
  d0b = 0;						// rate control: d0b (initial B frame virtual buffer fullness)

  if (N<1)
    error("N must be positive");
  if (M<1)
    error("M must be positive");
  if (N%M != 0)
    error("N must be an integer multiple of M");

  // set all the pointers to NULL to help releasing memory later
  motion_data = NULL;

  motion_data = (struct motion_data *)malloc(M*sizeof(struct motion_data));
  if (!motion_data)
    error("malloc failed\n");

							// P:  forw_hor_f_code forw_vert_f_code search_width/height
  motion_data[0].forw_hor_f_code = 2; 
  motion_data[0].forw_vert_f_code = 2;
  motion_data[0].sxf = 11; 
  motion_data[0].syf = 11;

  if (M > 2)						// we use reference frames
      {
							// B1: forw_hor_f_code forw_vert_f_code search_width/height
      motion_data[1].forw_hor_f_code = 1; 
      motion_data[1].forw_vert_f_code = 1;
      motion_data[1].sxf = 3;
      motion_data[1].syf = 3;

							// B1: back_hor_f_code back_vert_f_code search_width/height
      motion_data[1].back_hor_f_code = 1; 
      motion_data[1].back_vert_f_code = 1;
      motion_data[1].sxb = 7;
      motion_data[1].syb = 7;

							// B2: forw_hor_f_code forw_vert_f_code search_width/height
      motion_data[2].forw_hor_f_code = 1;
      motion_data[2].forw_vert_f_code = 1;
      motion_data[2].sxf = 7; 
      motion_data[2].syf = 7;

							// B2: back_hor_f_code back_vert_f_code search_width/height
      motion_data[2].back_hor_f_code = 1; 
      motion_data[2].back_vert_f_code = 1;
      motion_data[2].sxb = 3; 
      motion_data[2].syb = 3;
      }
  
/*
  if (!(fd = fopen(fname,"r")))
  {
    sprintf(errortext,"Couldn't open parameter file %s",fname);
    error(errortext);
  }

  fgets(id_string,254,fd);
  fgets(line,254,fd); sscanf(line,"%s",tplorg);
  fgets(line,254,fd); sscanf(line,"%s",tplref);
  fgets(line,254,fd); sscanf(line,"%s",iqname);
  fgets(line,254,fd); sscanf(line,"%s",niqname);
  fgets(line,254,fd); sscanf(line,"%s",statname);
  fgets(line,254,fd); sscanf(line,"%d",&inputtype);
  fgets(line,254,fd); sscanf(line,"%d",&nframes);
  fgets(line,254,fd); sscanf(line,"%d",&frame0);
  fgets(line,254,fd); sscanf(line,"%d:%d:%d:%d",&h,&m,&s,&f);
  fgets(line,254,fd); sscanf(line,"%d",&N);
  fgets(line,254,fd); sscanf(line,"%d",&M);
  fgets(line,254,fd); sscanf(line,"%d",&mpeg1);
  fgets(line,254,fd); sscanf(line,"%d",&fieldpic);
  fgets(line,254,fd); sscanf(line,"%d",&horizontal_size);
  fgets(line,254,fd); sscanf(line,"%d",&vertical_size);
  fgets(line,254,fd); sscanf(line,"%d",&aspectratio);
  fgets(line,254,fd); sscanf(line,"%d",&frame_rate_code);
  fgets(line,254,fd); sscanf(line,"%lf",&bit_rate);
  fgets(line,254,fd); sscanf(line,"%d",&vbv_buffer_size);   
  fgets(line,254,fd); sscanf(line,"%d",&low_delay);     
  fgets(line,254,fd); sscanf(line,"%d",&constrparms);
  fgets(line,254,fd); sscanf(line,"%d",&profile);
  fgets(line,254,fd); sscanf(line,"%d",&level);
  fgets(line,254,fd); sscanf(line,"%d",&prog_seq);
  fgets(line,254,fd); sscanf(line,"%d",&chroma_format);
  fgets(line,254,fd); sscanf(line,"%d",&video_format);
  fgets(line,254,fd); sscanf(line,"%d",&color_primaries);
  fgets(line,254,fd); sscanf(line,"%d",&transfer_characteristics);
  fgets(line,254,fd); sscanf(line,"%d",&matrix_coefficients);
  fgets(line,254,fd); sscanf(line,"%d",&display_horizontal_size);
  fgets(line,254,fd); sscanf(line,"%d",&display_vertical_size);
  fgets(line,254,fd); sscanf(line,"%d",&dc_prec);
  fgets(line,254,fd); sscanf(line,"%d",&topfirst);
  fgets(line,254,fd); sscanf(line,"%d %d %d",
    frame_pred_dct_tab,frame_pred_dct_tab+1,frame_pred_dct_tab+2);
  
  fgets(line,254,fd); sscanf(line,"%d %d %d",
    conceal_tab,conceal_tab+1,conceal_tab+2);
  
  fgets(line,254,fd); sscanf(line,"%d %d %d",
    qscale_tab,qscale_tab+1,qscale_tab+2);

  fgets(line,254,fd); sscanf(line,"%d %d %d",
    intravlc_tab,intravlc_tab+1,intravlc_tab+2);
  fgets(line,254,fd); sscanf(line,"%d %d %d",
    altscan_tab,altscan_tab+1,altscan_tab+2);
  fgets(line,254,fd); sscanf(line,"%d",&repeatfirst);
  fgets(line,254,fd); sscanf(line,"%d",&prog_frame);
// intra slice interval refresh period 
  fgets(line,254,fd); sscanf(line,"%d",&P);
  fgets(line,254,fd); sscanf(line,"%d",&r);
  fgets(line,254,fd); sscanf(line,"%lf",&avg_act);
  fgets(line,254,fd); sscanf(line,"%d",&Xi);
  fgets(line,254,fd); sscanf(line,"%d",&Xp);
  fgets(line,254,fd); sscanf(line,"%d",&Xb);
  fgets(line,254,fd); sscanf(line,"%d",&d0i);
  fgets(line,254,fd); sscanf(line,"%d",&d0p);
  fgets(line,254,fd); sscanf(line,"%d",&d0b);

  if (N<1)
    error("N must be positive");
  if (M<1)
    error("M must be positive");
  if (N%M != 0)
    error("N must be an integer multiple of M");

  motion_data = (struct motion_data *)malloc(M*sizeof(struct motion_data));
  if (!motion_data)
    error("malloc failed\n");

  for (i=0; i<M; i++)
  {
    fgets(line,254,fd);
    sscanf(line,"%d %d %d %d",
      &motion_data[i].forw_hor_f_code, &motion_data[i].forw_vert_f_code,
      &motion_data[i].sxf, &motion_data[i].syf);

    if (i!=0)
    {
      fgets(line,254,fd);
      sscanf(line,"%d %d %d %d",
        &motion_data[i].back_hor_f_code, &motion_data[i].back_vert_f_code,
        &motion_data[i].sxb, &motion_data[i].syb);
    }
  }

  fclose(fd);
*/


  // make flags boolean (x!=0 -> x=1)
  mpeg1 = !!mpeg1;
  fieldpic = !!fieldpic;
  low_delay = !!low_delay;
  constrparms = !!constrparms;
  prog_seq = !!prog_seq;
  topfirst = !!topfirst;

  for (i=0; i<3; i++)
  {
    frame_pred_dct_tab[i] = !!frame_pred_dct_tab[i];
    conceal_tab[i] = !!conceal_tab[i];
    qscale_tab[i] = !!qscale_tab[i];
    intravlc_tab[i] = !!intravlc_tab[i];
    altscan_tab[i] = !!altscan_tab[i];
  }
  repeatfirst = !!repeatfirst;
  prog_frame = !!prog_frame;

  // make sure MPEG specific parameters are valid
  range_checks();

  frame_rate = ratetab[frame_rate_code-1];

  // timecode -> frame number
  tc0 = h;
  tc0 = 60*tc0 + m;
  tc0 = 60*tc0 + s;
  tc0 = (int)(frame_rate+0.5)*tc0 + f;

  if (!mpeg1)
  {
    profile_and_level_checks();
  }
  else
  {
    // MPEG-1 
    if (constrparms)
    {
      if (horizontal_size>768
          || vertical_size>576
          || ((horizontal_size+15)/16)*((vertical_size+15)/16)>396
          || ((horizontal_size+15)/16)*((vertical_size+15)/16)*frame_rate>396*25.0
          || frame_rate>30.0)
      {
        if (!quiet)
          WarningMPEG("Warning: setting constrained_parameters_flag = 0\n");
        constrparms = 0;
      }
    }

    if (constrparms)
    {
      for (i=0; i<M; i++)
      {
        if (motion_data[i].forw_hor_f_code>4)
        {
          if (!quiet)
            WarningMPEG("Warning: setting constrained_parameters_flag = 0\n");
          constrparms = 0;
          break;
        }

        if (motion_data[i].forw_vert_f_code>4)
        {
          if (!quiet)
            WarningMPEG("Warning: setting constrained_parameters_flag = 0\n");
          constrparms = 0;
          break;
        }

        if (i!=0)
        {
          if (motion_data[i].back_hor_f_code>4)
          {
            if (!quiet)
              WarningMPEG("Warning: setting constrained_parameters_flag = 0\n");
            constrparms = 0;
            break;
          }

          if (motion_data[i].back_vert_f_code>4)
          {
            if (!quiet)
              WarningMPEG("Warning: setting constrained_parameters_flag = 0\n");
            constrparms = 0;
            break;
          }
        }
      }
    }
  }

  // relational checks

  if (mpeg1)
  {
    if (!prog_seq)
    {
      if (!quiet)
        WarningMPEG("Warning: setting progressive_sequence = 1\n");
      prog_seq = 1;
    }

    if (chroma_format!=CHROMA420)
    {
      if (!quiet)
        WarningMPEG("Warning: setting chroma_format = 1 (4:2:0)\n");
      chroma_format = CHROMA420;
    }

    if (dc_prec!=0)
    {
      if (!quiet)
        WarningMPEG("Warning: setting intra_dc_precision = 0\n");
      dc_prec = 0;
    }

    for (i=0; i<3; i++)
      if (qscale_tab[i])
      {
        if (!quiet)
	    {
	    sprintf(t, "Warning: setting qscale_tab[%d] = 0\n",i);
	    WarningMPEG(t);
	    }
        qscale_tab[i] = 0;
      }

    for (i=0; i<3; i++)
      if (intravlc_tab[i])
      {
        if (!quiet)
	    {
	    sprintf(t, "Warning: setting intravlc_tab[%d] = 0\n",i);
	    WarningMPEG(t);
	    }
        intravlc_tab[i] = 0;
      }

    for (i=0; i<3; i++)
      if (altscan_tab[i])
      {
        if (!quiet)
	    {
	    sprintf(t, "Warning: setting altscan_tab[%d] = 0\n",i);
	    WarningMPEG(t);
	    }
        altscan_tab[i] = 0;
      }
  }

  if (!mpeg1 && constrparms)
  {
    if (!quiet)
      WarningMPEG("Warning: setting constrained_parameters_flag = 0\n");
    constrparms = 0;
  }

  if (prog_seq && !prog_frame)
  {
    if (!quiet)
      WarningMPEG("Warning: setting progressive_frame = 1\n");
    prog_frame = 1;
  }

  if (prog_frame && fieldpic)
  {
    if (!quiet)
      WarningMPEG("Warning: setting field_pictures = 0\n");
    fieldpic = 0;
  }

  if (!prog_frame && repeatfirst)
  {
    if (!quiet)
      WarningMPEG("Warning: setting repeat_first_field = 0\n");
    repeatfirst = 0;
  }

  if (prog_frame)
  {
    for (i=0; i<3; i++)
      if (!frame_pred_dct_tab[i])
      {
        if (!quiet)
	    {
	    sprintf(t, "Warning: setting frame_pred_frame_dct[%d] = 0\n",i);
	    WarningMPEG(t);
	    }
        frame_pred_dct_tab[i] = 1;
      }
  }

  if (prog_seq && !repeatfirst && topfirst)
  {
    if (!quiet)
      WarningMPEG("Warning: setting top_field_first = 0\n");
    topfirst = 0;
  }

  // search windows
  for (i=0; i<M; i++)
  {
    if (motion_data[i].sxf > (4<<motion_data[i].forw_hor_f_code)-1)
    {
      if (!quiet)
	    {
	    sprintf(t, "Warning: reducing forward horizontal search width to %d\n", (4<<motion_data[i].forw_hor_f_code)-1);
	    WarningMPEG(t);
	    }
      motion_data[i].sxf = (4<<motion_data[i].forw_hor_f_code)-1;
    }

    if (motion_data[i].syf > (4<<motion_data[i].forw_vert_f_code)-1)
    {
      if (!quiet)
	    {
	    sprintf(t, "Warning: reducing forward vertical search width to %d\n", (4<<motion_data[i].forw_vert_f_code)-1);
	    WarningMPEG(t);
	    }
      motion_data[i].syf = (4<<motion_data[i].forw_vert_f_code)-1;
    }

    if (i!=0)
    {
      if (motion_data[i].sxb > (4<<motion_data[i].back_hor_f_code)-1)
      {
        if (!quiet)
	    {
	    sprintf(t, "Warning: reducing backward horizontal search width to %d\n", (4<<motion_data[i].back_hor_f_code)-1);
	    WarningMPEG(t);
	    }
        motion_data[i].sxb = (4<<motion_data[i].back_hor_f_code)-1;
      }

      if (motion_data[i].syb > (4<<motion_data[i].back_vert_f_code)-1)
      {
        if (!quiet)
	    {
	    sprintf(t, "Warning: reducing backward vertical search width to %d\n", (4<<motion_data[i].back_vert_f_code)-1);
	    WarningMPEG(t);
	    }
        motion_data[i].syb = (4<<motion_data[i].back_vert_f_code)-1;
      }
    }
  }

}


static void readquantmat()
{
  int i,v;
  FILE *fd;

  if (iqname[0]=='-')
  {
    /* use default intra matrix */
    load_iquant = 0;
    for (i=0; i<64; i++)
      intra_q[i] = default_intra_quantizer_matrix[i];
  }
  else
  {
    /* read customized intra matrix */
    load_iquant = 1;
    if (!(fd = fopen(iqname,"r")))
    {
      sprintf(errortext,"Couldn't open quant matrix file %s",iqname);
      error(errortext);
    }

    for (i=0; i<64; i++)
    {
      fscanf(fd,"%d",&v);
      if (v<1 || v>255)
        error("invalid value in quant matrix");
      intra_q[i] = v;
    }

    fclose(fd);
  }

  if (niqname[0]=='-')
  {
    /* use default non-intra matrix */
    load_niquant = 0;
    for (i=0; i<64; i++)
      inter_q[i] = 16;
  }
  else
  {
    /* read customized non-intra matrix */
    load_niquant = 1;
    if (!(fd = fopen(niqname,"r")))
    {
      sprintf(errortext,"Couldn't open quant matrix file %s",niqname);
      error(errortext);
    }

    for (i=0; i<64; i++)
    {
      fscanf(fd,"%d",&v);
      if (v<1 || v>255)
        error("invalid value in quant matrix");
      inter_q[i] = v;
    }

    fclose(fd);
  }
}
