/******  aaFilter.h
         S.R. Lines  (srl)
         8 Oct 92  (Orig. Aug 92) *********/
         
#ifndef  _aaFilter_h_
#define  _aaFilter_h_

#define  MAX_BANKS 5                 /* As of 25.8.92, used only by BRIM */

typedef  struct Brim {              /* Bank-stored Raster Image Map */
                int wide, tall;     /* width & height in pixels     */
                int nChan;          /* usually == bytes per pixel. */
                int kind;           /* E.g., RGBI, RGBP, RGBIT, Gray256,... */
                unsigned char *cp[MAX_BANKS]; /* Banks need not be contiguously stored.*/
              } Brim ;


typedef  struct Psi1                 /* Point Spread Function, 1d.                  */
              { const int *kern;     /* Convolution kernel = coord. samples of psf. */
                int   denom,len;     /* length in pixels: diameter, not radius.     */
              } Psi1;                /* NB: To normalize, multiply by 1/denom.      */

typedef  struct Filter1d
              { int    len;          /* length in pixels: diameter, not radius */
                const float *kern; /* the x,y coordinate pairs of actual points **/
              } Filter1d;

typedef  struct Filter2d
              { int    wide, tall;   /* width & height in pixels: diameter, not radius */
                const float *kern; /* the x,y coordinate pairs of actual points        */
              } Filter2d;            /* NB: Filter2d assumes rectangular mask; for     */
                                     /* non-const width mask, use an array of Filter1d */
                                       
typedef  struct Filter2dMC           /* Multi-Channel Filter2d, for interleaved images */
              { int    wide, tall;   /* width & height in pixels: diameter, not radius */
                int    nChannels;    /* must be same as number of channels in image    */
                const float *kern; /* the x,y coordinate pairs of actual points      */
              } Filter2dMC;          /* NB: Filter2d assumes rectangular mask; for non-*/
                                     /* const width mask, use an array of Filter1d     */
                                       
#ifndef  _NO_PROTO

extern  puc  LogMagCplx (rgst puc dst, rgst flp fft, unl count, dbl norm);
extern  puc  LogMagCplxEqu (rgst puc dst, flp fft, unl count, flp maxlogcf);

#else   /*  ifndef _NO_PROTO */


#endif  /* _NO_PROTO */

#endif  /* _aaFilter_h_ */



