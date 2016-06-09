/*****************************************************************************
 aa.h    S.R.Lines  (spraxlo@{ai|media}, srl@athena)    92.2-93.6.20
 aa as a project is dead, but its src lives on.
 ANSI C features are #ifdef'd for portability, but ANSI name space is invaded.
 All Px stuff should migrate to aaPx.h, which should become ~/c/Px/src/Px.h.
 *****************************************************************************/
#ifndef  _aa_h_
#define  _aa_h_

         /*_______SYSTEMS: Hardware & Compilers____________*/ 
#ifdef   SUN
#define  _NO_CONST
#endif
#ifdef   _NO_CONST      /* Some non-ANSI compilers permit const decls */
#define  const          /* Define const as nothing (anti-ANSI)  */
#endif

#include <stdio.h>      /* FILE, fopen, ferror, printf, perror ... */
#include <stdlib.h>     /* exit, malloc, ... */
#include <stddef.h>     /* NULL, size_t, ... */
#include <string.h>     /* memcpy, memset, strcpy, strcoll, strerror ... */ 
#include <math.h>       /* cosh, frexp, ... */
#include <time.h>       /* time_t, CLOCKS_PER_SEC */
#ifndef  CLOCKS_PER_SEC
#define  CLOCKS_PER_SEC 60
#endif

#ifndef  _int32bit_h_
#include "int32bit.h"
#endif
#ifndef   unTypes_h__
#include <un/unTypes.h>
#endif

         /*_____ GLOBAL VARIABLES______________*/
/* extern  int    Verbo, Debug; */

         /*______ MACRO CONSTANTS __________________________________*/
         /*______ Debugging:  Use these predefined values as (loose) guides: */
#define  DEBUG_OFF 0     /* No debugging (except the kluge #if (DEBUG==0))   */
#define  BUG_LOCAL 1     /* Do only local, temporarily defined bug checking. */
#define  BUG_CHECK 2     /* Check data, don't necessarily print anything. */
#define  BUG_EMESG 3     /* Print messages only on failure */
#define  BUG_EDATA 4     /* Print data and message only on failure */
#define  BUG_SMESG 5     /* Print message even on success */
#define  BUG_SDATA 6     /* Print data and message even on success */

                         /* _______Used as values of Px.ffmt: __________*/
#define  PX_BANKS     1  /* Channel order: (contiguous) banks in memory */
#define  PX_REVERSE   2  /* Pixel order: interleaved bytes, or tuples */
#define  PX_CTRANS    4  /* Constant Transparency/Opacity (@@ why?) */
#define  PX_CLUT      8  /* Color Look Up Table necessary */
/*
#define  PX_RESERVE  16
#define  PX_RESERVE  32
#define  PX_RESERVE  64
#define  PX_RESERVE 128
#define  PX_RESERVE 256
*/
                                       /* ____ 24-bits per pixel: ____ */
#define  RGBI        512               /* RGB Interleaved */ 
#define  RGBP    (  RGBI | PX_BANKS )  /* RGB Banked ("Dave format"). */ 

                                       /* ____ 32-bits per pixel: ____ */
#define  RGBTI      1024               /* RGB + Transparency, Interleaved. */ 
#define  RGBTP   ( RGBTI | PX_BANKS )  /* RGB + Transparency, Banked.      */ 
#define  RGBTCI  ( RGBTI | PX_CTRANS)  /* RGB + Transparency, Interleaved, Const. Trans.    */ 
#define  RGBTCP  ( RGBTI | PX_BANKS | CTRANS)  /* RGB+ConstTrans, Banked */ 
#define  CMYKI      2048               /* CMYK Interleaved (Swizzled) */ 
#define  CMYKP   ( CMYKI | PX_BANKS )  /* CMYK in 8-bit Panes (Banks) */ 

                                       /* _____ 8-bits per pixel: ____ */
#define  GRAY    (  4096 | PX_BANKS )  /* 8-bit GrayScale */ 
#define  CLUT8   (  8192 | PX_BANKS )  /* 8-bit Color Look Up Table. */ 

                                       /* ____ 16-bits per pixel: ____ */
#define  GRAYTB  ( 16384 | PX_BANKS )  /* GrayScale + Transparency */ 
#define  CLUT8TB ( 32768 | PX_BANKS )  /* Color LookUp + Transparency: */ 

                                       /* ____ 40-bits (5 bytes) per pixel:  */
#define  CHAN5   (   8192| PX_BANKS )  /* RGB + T + Alpha, 5 ubytes in Panes.*/

         /* ____________ Alignment flag values: _______________ */
#define  A_LEFT        1
#define  A_HCENTER     2
#define  A_RIGHT       4
#define  A_HORIZONTAL (A_LEFT | A_HCENTER | A_RIGHT)
#define  A_TOP         8
#define  A_VCENTER    16
#define  A_BOTTOM     32
#define  A_VERTICAL   (A_TOP  | A_VCENTER | A_BOTTOM)
#define  A_CENTER     (A_HCENTER | A_VCENTER)

#define  CONTRACT      1
#define  SAMESIZE      2
#define  EXPAND        4
#define  ZEROPAD       8
#define  MEANPAD      16
#define  DONTPAD      32
#define  COPYPAD      64
#define  CTRANSP     128  /* constant transparency: don't convolve T channel */

         /*______ MACRO FUNCTIONS __________________________________*/
#define  RETNULL(zz) {fprintf(stderr,"NULL returned by %s\n",zz);return(NULL);}
         /*______ BASICS ___________*/
#ifndef  ABS
#define  ABS(a)       (((a) < 0) ? -(a):(a))
#endif
#ifndef  MAX
#define  MAX(a,b)     (((a) > (b)) ? (a):(b))
#define  MIN(a,b)     (((a) < (b)) ? (a):(b))
#endif
         /*______ TIMING ___________*/
#ifdef   TIMING
static   time_t Told,Tnew;
#define  BEGCLOCK(zz) { fprintf(stderr,"_____BEGIN %s\n",zz); Told = clock(); }
#define  ENDCLOCK(zz) { Tnew = clock();\
                        fprintf(stderr,"_____END__ %s: %.3f\n",zz,\
                        (double)(Tnew-Told)/CLOCKS_PER_SEC); }
#else
#define  BEGCLOCK(zz)  ;
#define  ENDCLOCK(zz)  ;
#endif   /* TIMING */

#endif   /*  _aa_h_: eof  */


