/*******************************************************************************
aaGaussians.h    S.R. Lines    5.9.92  (Orig. 9.92)
         FUNC: Should be #included in aaGaussiansBlur.c aaGaussiansFade.c., etc.
         If your compiler can't handle const declarations, hide them using the
         precompiler, e.g.:
#ifdef	_NO_CONST
#define  const
#endif
*********************************************************************************/
                
#ifndef  _aaGaussians_h_
#define  _aaGaussians_h_

#define  MAX_BLUR_RADIUS    21  /* Usage: radius < MAX_BLUR_RADIUS (strictly <, not <=) */
#define  MAX_BLUR_LENGTH    42  /* Usage: length = 2*radius + 1 < MAX_BLUR_LENGTH  */

extern  const float *const GaussiansF [MAX_BLUR_LENGTH];  /* Normalized: should sum to 1.0 */
extern  const int   *const GaussiansN [MAX_BLUR_LENGTH];  /* MIGHT sum to 2 to the (n-1)   */

#endif   /* _aaGaussians_h_ */


