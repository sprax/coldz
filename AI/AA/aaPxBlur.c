/*******  FILE: aaPxBlur.c
          AUTH: S.R. Lines
          DATE: 10.10.92  (Orig. 10.92) 
          FUNC: approximately Gaussian blurring.
          ******/
#define   DEBUG  0
#include <aa/aa.h>
#include <aa/aaConvo.h>
#include <aa/aaGaussians.h>
#include <aa/aaPx.h>

#include <00gg.h>

/*___________________________________________________________________________*
    PxRadialBlur/f, PxGaussianBlur/f: (cf. aaGaussianBlur.c/aaGaussianBlurF.c)
    Give them the blur radius or x and y blur lengths
    and they convolve with the proper mask, output size determined by flags.
    The float versions (suffixed by an "f") are about 3 times slower, but in
    some ways are more accurately gaussian.  It doesn't make a lot of sense
    to use both.  For real improvement, it would pay better to taylor the values
    in aaGaussiansN.c (q.v.) to one's own blurring needs.
 *___________________________________________________________________________*/

Pxp PxGaussianBlur (const Pxp src, int xlen, int ylen, unt flags)
{   
  Pxp dst; puc dp = NULL;
  int wide = src->wide;
  int tall = src->tall;
  if (0 < xlen && xlen < MAX_BLUR_LENGTH &&
      0 < ylen && ylen < MAX_BLUR_LENGTH) {
      if (! (dp = ConvoSep(dp,src->pp,GaussiansN[xlen],GaussiansN[ylen],&wide,&tall
          ,xlen,ylen,src->nChan,src->kind,flags)))  RETNULL("ConvoSep in PxGaussianBlur");
      return( PxNew(wide,tall,src->nChan,src->kind,dp,src->path));
      }
  fprintf(stderr,"PxGaussianBlur: bad x or y blur length\n");
  return(NULL);
}


Pxp PxRadialBlur (const Pxp src, int radius, unt flags)
{   
  Pxp dst; puc dp = NULL;
  int wide = src->wide;
  int tall = src->tall;
  radius = radius*2 + 1;
  if (0 < radius && radius < MAX_BLUR_LENGTH) {
      if (! (dp = ConvoSep(dp,src->pp,GaussiansN[radius],GaussiansN[radius],&wide,&tall
          ,radius,radius,src->nChan,src->kind,flags)))  RETNULL("ConvoSep in PxRadialBlur");
      return( PxNew(wide,tall,src->nChan,src->kind,dp,src->path));
      }
  fprintf(stderr,"PxRadialBlur: bad radius\n");
  return(NULL);
}


