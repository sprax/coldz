/******  aaConvert.h -- prototypes & macros for functions in aaConvert.c.
         S.R. Lines  (srl)
         5.9.92  (Orig. Aug 92) *********/

#ifndef _aaConvert_h_   /* Should be included after aa.h */
#define _aaConvert_h_

/*___________ Weights for converting RGB to 8-bit gray.  */
#define  REDI (0.30)    /* Factory calibration values for phosphors in HP */
#define  GRNI (0.59)    /* bobcat monitors (98721 or whatever).  All monitors */
#define  BLUI (0.11)    /* are different; see Foley & van Dam for typical values. */
/*___________ Use other values for non-standard conversions/color edge tracing, etc. */
/*___________ RGB to Gray Conversion:  */ 
#define  RGBI2G(p)  ((int)(REDI*(*(p)) + GRNI*(*((p)+1)) + BLUI*(*((p)+2))))

#ifndef _NO_PROTO

extern  int  AveRGBHSV(puc ucip, size_t pixels, float rgb[3 ], float hsv[3 ]);
extern  int  AveHSV(puc ucip, size_t pixels, float *ah, float *as, float *av);
extern  int  oldHSV(puc ucip, size_t pixels, float *ah, float *as, float *av);
extern  int  rgbhsvf(dbl r, dbl g, dbl b, dbl *h, dbl *s, dbl *v);
extern  int  rgbhsvu(puc rgb, puc h, puc s, puc v);
extern  int  RGBtoHSV(puc imp, int wide, int tall);
extern  dbl  AveIntensityRGB(puc src, int wide, int tall, int iave[3 ], flt fave[3]);
extern  dbl  AveIntensity(puc src, int wide, int tall);
extern  int  RGBtoConstBright(puc cip, int cw, int ch, int bright);
extern  int  RGBtoRelAbsRGB(puc cip, int cw, int ch, int bright);
extern  int  RGBtoRelTwoRGB(puc cip, int cw, int ch, int bright);
extern  int  RGBtoConstMin(puc cip, int cw, int ch, int minbrt);
extern  int  RelativeRGB(unc rgb[3 ], float iqf[3 ]);
extern  int  QuotientRGB(unc rgb[3 ], float iqf[3 ]);
extern  int  rgbcie(int r, int g, int b, float *x, float *y, float *Y);
extern  int  compute_i_coef(void);
extern  puc  IntToUncInvertClip (puc dst, pcint src, int wide, int tall);
extern  puc  IntToUnc (puc dst, pcint src, int wide, int tall);
extern  puc  RGBIfromRGBTI (puc dst, rpuc src, int wide, int tall);
extern  int  BanksToInterleaved (puc *src, int wide, int tall, int nChan);
extern  int  Color24toGray8 (rgst puc src,rgst puc dest8,rgst size_t pixels);
extern  int  Color24toGray8and24 (rgst puc source, rgst puc dest8, rgst puc dest24, rgst size_t pixels);
extern  int  Copy8to24 (rgst puc src8,rgst puc dest24, int cols, int rows);
extern  int  CopyFFTmagTo8 (float *fftf, puc fft8, int fdd[2]);
extern  int  CopyFFTtoImage8 (rgst float  *fftf, puc fft8, int fdd[2]);
extern  int  CopyFto24 (float *gipf, puc dest24, int cols, int rows);
extern  int  CopyFtoUNC (float *gipf, unc *gip8, int cols, int rows);
extern  int  CopyMagTo8and24 (float *fftf, puc fft8, puc fft24, int cols, int rows);
extern  int  CplxMagTo24 (float *fftf, puc xip24, int cols, int rows);
extern  int  CplxMagTo8 (float *fftf, puc xip8, int cols, int rows);
extern  int  DataTo8and24 (float *fftf, puc fft8, puc fft24, int cols, int rows,int fdd[2]);
extern  int  GrayTo24 (rgst puc src8,rgst puc dest24, rgst size_t pixels);
extern  int  GrayToBinary (rgst unc *gray8bit, int cols, int rows, rgst int thresh);
extern  int  GrayToRGBI (rgst puc gip, rgst puc rgbi, int cols, int rows);
extern  int  RealTo24 (float *fftf, puc xip24, int cols,int rows, int fdd[2]);
extern  int  RealTo8 (float *fftf, puc gip8, int cols,int rows, int fdd[2]);
extern  int  RescaleBinIm (rgst unc *bin8bit, int cols, int rows, int posval);
extern  int  RescaleBinTo24 (rgst puc *bin8bit,rgst puc cip24, int cols, int rows, int posval);
extern  int  RGBIintoGray8 (rgst puc src, rgst puc dst, rgst size_t pixels);
extern  int  RGBItoChannels (puc src24,puc gray,puc red,puc grn,puc blu,int cols,int rows);
extern  int  ScaleFltToUnc (float *gipf, puc up, int cols, int rows, dbl range);
extern  int  ScaleUncToFlt (puc up, float *gipf, int cols, int rows, dbl range);
extern  puc  GrayFromRGBI (rgst puc rgbi, int wide, int tall);
extern  puc  RGBIfromGray (rgst puc src8, int wide, int tall);
extern  puc  RGBItoGray (rgst puc src24, int wide, int tall);
extern  puc  RGBItoGray5x5p (puc rip, puc gip, int wide, int x, int y);
extern  void FFTdataToUCP (float *fft,puc cp,int pwide,int ptall,int fwide,int ftall);
extern  void RGBItoGray5x5 (puc rip, puc gip, int wide, int x, int y);
extern  void UCPtoFFTdata (float *fft,puc cp,int pwide,int ptall,int fwide,int ftall);

#else   /*  ifdef _NO_PROTO */

extern  puc  RGBIfromRGBTI ();
extern  int  BanksToInterleaved ();
extern  int  Color24toGray8 ();
extern  int  Color24toGray8and24 ();
extern  int  Copy8to24 ();
extern  int  CopyFFTmagTo8 ();
extern  int  CopyFFTtoImage8 ();
extern  int  CopyFto24 ();
extern  int  CopyFtoUNC ();
extern  int  CopyMagTo8and24 ();
extern  int  CplxMagTo24 ();
extern  int  CplxMagTo8 ();
extern  int  DataTo8and24 ();
extern  int  GrayTo24 ();
extern  int  GrayToBinary ();
extern  int  GrayToRGBI ();
extern  int  RealTo24 ();
extern  int  RealTo8 ();
extern  int  RescaleBinIm ();
extern  int  RescaleBinTo24 ();
extern  int  RGBIintoGray8 ();
extern  int  RGBItoChannels ();
extern  int  ScaleFltToUnc ();
extern  int  ScaleUncToFlt ();
extern  puc  GrayFromRGBI ();
extern  puc  RGBIfromGray ();
extern  puc  RGBItoGray ();
extern  puc  RGBItoGray5x5p ();
extern  void FFTdataToUCP ();
extern  void RGBItoGray5x5 ();
extern  void UCPtoFFTdata ();

#endif  /* _NO_PROTO */

#endif  /* _aaConvert_h_ */


