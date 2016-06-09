/******  aaConvert.h -- prototypes & macros for functions in aaConvert.c.
         S.R. Lines  (srl)
         5.9.92  (Orig. Aug 92) *********/

#ifndef  _aaFFT_h_   /* Should be included after aa.h */
#define  _aaFFT_h_

#ifndef _NO_PROTO

extern  int  ceil_power_2 (int);
extern  int  CopyFFTmagTo8 (float *fftf, puc fft8, int fdd[2]);
extern  int  CopyFFTtoImage8 (rgst float  *fftf, puc fft8, int fdd[2]);
extern  void FFTdataToUCP (float *fft,puc cp,int pwide,int ptall,int fwide,int ftall);
extern  void fourn (rgst float *data, int *nn, int ndim, int isign);
extern  void UCPtoFFTdata (float *fft,puc cp,int pwide,int ptall,int fwide,int ftall);
extern  int  DivSqrtDims2 (rgst float *fftf, int fdd[2]);
extern  int  PrepFFT (puc  gip8, flp fftf, int cols, int rows, int fdd[2]);
extern  int  PrepFFTkeepFloat (puc gip8,flp gipf,flp fftf,int cols,int rows,int fdd[2]);

#else   /* _NO_PROTO */

extern  int  ceil_power_2 ();
extern  int  CopyFFTmagTo8 ();
extern  int  CopyFFTtoImage8 ();
extern  void fourn ();
extern  void FFTdataToUCP ();
extern  void UCPtoFFTdata ();
extern  int  DivSqrtDims2 ();
extern  int  PrepFFT ();
extern  int  PrepFFTkeepFloat ();

#endif  /* _NO_PROTO */

#endif  /* _aaFFT_h_ */


