/*****************************************************************************
 imagmem.h    S.R.Lines    93.6.10
 "Types" representing attributes of image data in memory structs.
 For image file format and raw memory dump types, see imagic.h.
 
 For all types, memory blocks should be 32-bit aligned, but not necessarily
 32-bit-padded at "row"-boundaries.  (malloc should give this automatically).
 To use "borders", just use a subimage within a larger one (EG: Pc.c).
 The first two bits determine whether the image is stored as:
 0: Full interleaving, consecutive pixels in one memory stream (1D-array).
    EG: 8-bit pixels in consecutive bytes, RGBA pixels in consec. ints.
 1: Row interleaving: consecutive channels for each row in one mem. stream.
    EG: 1 row red, 1 row grn, 1 row blu, 1 row alp, then next row red, etc.
 2: Channels separate but consecutive (AKA planes, panes, banks, etc.)
    Rows are separated by a constant offset (nChan * BytesPerChanInPrimary).
    EG: 1 channel of gray or 8-bit color, followed by 1 channel graded mask.
 3: Channels separate and non-consecutive: each channel stored in its own
    memory stream.  Still will be constant offset between rows for each chan,
    unless memory is distributed among different address spaces--not likely).
 Scheme 0 is most efficient for image processing, scheme 1 is best most device
 I/O tasks, e.g. reading or writing to framebuffer, or converting file formats,
 gives slightly faster I/O for some hardware (HP framebuffers?), and 3 is most
 convenient for many editing and processing tasks, despite 0 often being more
 efficient.  Scheme 0 is the default because it makes single pixel operations
 easy.  
 The overall default -- single channel unsigned char -- is consistent with
 almost everything else, and thus could be assigned numerical value 0, not 512.
 To avoid redundant checking, bits 1 & 2 should always be clear for single
 channel images, even though they are trivially both row and channel separated.
 *****************************************************************************/
#ifndef	imagmem_h__
#define	imagmem_h__

#include <limits.h>	/* CHAR_BIT, INT_MAX, etc. */

#define MM_UNKNOWN  0xF0000000	/* 2^31 == UINT_MAX */

#define MM_CONPXL   0UL /* Contig. Pixels (one interleaved stream, UL to BR) */
#define MM_CONROW   1   /* Contig. Rows (Band structure, eg: R0,G0,B0,R1...) */
#define MM_CONCHN   2   /* Contig. Channels (eg: Consec. Byte-Planes) */
#define MM_SEPCHN   3   /* Separate and non-consecutive planes */ 
#define MM_SEPROW   4   /* Non-consecutive rows: must use row pointers. */ 
#define MM_INTERL   MM_CONPXL	/* synonym */
#define MM_BANDED   MM_CONROW	/* synonym */
#define MM_PLANAR   MM_CONCHN	/* synonym */
#define MM_PLANES   MM_CONCHN	/* synonym */
#define MM_BANKED   MM_CONCHN	/* synonym */
#define MM_BANKS    MM_CONCHN	/* Banks, Panes, Planes, Trains... */

#define MM_CLUT	    8   /* Color Look Up Table(s) present and/or required */
#define MM_SUB      16  /* (pixels wide)*pixelsize != bytes per row, etc */
#define MM_PACK     32  /* Bitmaps tightly packed, etc. */


#define MM_NN       64	/* Non-Negative, usually [0.0,1.0] or [0,INF]*/
#define MM_PS       128	/* Postive Signed, strictly > EPSILON > 0.0   */
#define MM_UU       256	/* (Signed) unit interval [-1,1] (Neg: use -Pos) */
#define MM_UN      (MM_UU | MM_NN)	/* Non-neg Unit interval  [  0,1] */
#define MM_UP      (MM_UU | MM_PS)	/* Positive Unit interval (eps,1] */

#define MM_TYPE     0x00FFFF00UL /* Bits 9-20 store the underlying C type. */
#define MM_BIT      0x00F00000UL /* = 1048576: 1-bit int IS THE LAST TYPE.   */
                                 /* Actual storage is packed unc, on 32-bit  */
                                 /* int bdry; can address as unt & bit shift */

#define MM_UNC      512	/* type UNC: 8-bit unsigned integer (sys. indep.) */
#define MM_DEFAULT  MM_UNC	/* UNsigned Char is the default (32-bit aligned) */
#define MM_UNC_NN  (MM_UNC | MM_NN )	/* [ 0, 255] */
#define MM_UNC_PS  (MM_UNC | MM_PS )	/* [ 1, 255] */
#define MM_UNC_UN  (MM_UNC | MM_PS )	/* {0,1} -- Boolean UNC, Bitmap */


#define MM_CHR     1024 /* type CHR: 8-bit (signed) integer [-128,127] */
#define MM_CHR_NN  (MM_CHR | MM_NN )	/* [ 0, 127]*/
#define MM_CHR_PS  (MM_CHR | MM_PS )	/* [ 1, 127] */
#define MM_CHR_UN  (MM_CHR | MM_PS )	/* {0,1} -- Boolean CHR, Bitmap */

#define MM_USH      2048   /* type USH: 16-bit unsigned integer (sys. dep.) */
#define MM_USHRT    MM_USH /* Tip our hats to the conventions of <limits.h> */
#define MM_USHORT   MM_USH /* synonym */
#define MM_USH_NN  (MM_USH | MM_NN )	/* [ 0, 65535] */
#define MM_USH_PS  (MM_USH | MM_PS )	/* [ 1, 65535] */
#define MM_USH_UN  (MM_USH | MM_PS )	/* {0,1} -- Boolean USH, Bitmap */

#define MM_SHT      4096	/* type SHT: 16-bit (signed) integer (sys. dep.) */
#define MM_SHORT    MM_SHT
#define MM_SHTT     MM_SHT	/* Lip service to the conventions of <limits.h> */
#define MM_SHT_NN (MM_SHT | MM_NN )	/* [ 0, 32767 = SHRT_MAX] */
#define MM_SHT_PS (MM_SHT | MM_PS )	/* [ 1, 32767 = SHRT_MAX] */
#define MM_SHT_UN (MM_SHT | MM_PS )	/* {0,1} -- Boolean SHT, Bitmap */

#define MM_UNT      8192	/* type UNT: 32-bit unsigned int (sys. dep.) */
#define MM_UINT     MM_UNT      /* synonym */
#define MM_UNT_NN  (MM_UNT | MM_NN )	/* [ 0, 4294967295 = 0xFFFFFFFF] */
#define MM_UNT_PS  (MM_UNT | MM_PS )	/* [ 1, 0xFFFFFFFF = UINT_MAX]   */
#define MM_UNT_UN  (MM_UNT | MM_PS )	/* {0,1} -- Boolean UNT, Bitmap  */

#define MM_INT      16384	/* type INT: 32-bit (signed) int (sys. dep.) */
#define MM_INT_NN  (MM_INT | MM_NN )	/* [ 0, 2147483647 = INT_MAX] */
#define MM_INT_PS  (MM_INT | MM_PS )	/* [ 1, 0x0FFFFFFF = INT_MAX] */
#define MM_INT_UN  (MM_INT | MM_PS )	/* {0,1} -- Boolean INT, Bitmap */

#ifdef	INT_NEQ_LONG
#define MM_LNG     (-1)	/* Assume type LNG == type INT (32-bit int) */
#endif

#define MM_FLT      32768	/* type FLT: 32-bit (single precision) float */
#define MM_FLOAT    MM_FLT	/* synonym */
#define MM_FLT_NN  (MM_FLT | MM_NN)         /* [  0,HUGE_VAL] */
#define MM_FLT_PS  (MM_FLT | MM_NN)         /* (eps,HUGE_VAL] */
#define MM_FLT_UN  (MM_FLT | MM_UU | MM_NN) /* [ 0.0,1.0]     */
#define MM_FLT_UP  (MM_FLT | MM_UU | MM_PS) /* ( eps,1.0]     */
#define MM_FLT_UU  (MM_FLT | MM_UU)         /* [-1.0,1.0]     */

#define MM_DBL      65536	/* type DBL: 64-bit (double precision) float */
#define MM_DBL_NN  (MM_DBL | MM_NN)         /* [  0,HUGE_VAL] */
#define MM_DBL_PS  (MM_DBL | MM_NN)         /* (eps,HUGE_VAL] */
#define MM_DBL_UN  (MM_DBL | MM_NN | MM_UU) /* [ 0.0,1.0]     */
#define MM_DBL_UP  (MM_DBL | MM_PS | MM_UU) /* ( eps,1.0]     */
#define MM_DBL_UU  (MM_DBL | MM_UU)         /* [-1.0,1.0]     */

#define MM_CMPLXD   131072	/* type CMPLXF: 2 64-bit (double prec.) floats */
#define MM_CMPLXF   262144	/* type CMPLXD: 2 32-bit (single prec.) floats */

#define MM_8_BIT   (MM_CHR | MM_UNC)	/* Simple tests: if (blah & MM_8_BIT) */
#define MM_16_BIT  (MM_CHR | MM_UNC)
#define MM_24_BIT  (MM_CHR | MM_UNC)
#define MM_32_BIT  (MM_CHR | MM_UNC)

#define MM_RGB_NOT  2097152  /* Not RGB - prob. HSB */
#define MM_HSB      2097152  /* HSB is RGB's only competitor for now.  CIE? */
/**********************************
#define MM_RESERVE_22	4194304
#define MM_RESERVE_23	8388608
#define MM_RESERVE_24	16777216
#define MM_RESERVE_25	33554432
#define MM_RESERVE_26	67108864
#define MM_RESERVE_27	134217728
#define MM_RESERVE_28	268435456
#define MM_RESERVE_29	536870912
#define MM_RESERVE_30	1073741824
#define MM_RESERVE_31	2147483648
#define MM_RESERVE_32	4294967296
**********************************/

#endif /* imagmem_h__ */

