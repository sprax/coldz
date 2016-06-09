/*****************************************************************************
 winSliders.h    S.R.Lines    93.11.10 - 95.06.03
 *****************************************************************************/
#ifndef winSliders_h__
#define winSliders_h__

extern	long	InMainWid;
extern	long	InSlidersWid;

void winSlidersUpdate (double red, double grn, double blu, int mark, int olds);

void
winSlidersRgbO( double *pR, double *pG, double *pB, char *wTitle
              , void (*rgbCallBack)(double R, double G, double B)
              , void (*redrawMain)(long wid), long widMain, int get_rgb);

void
winSlidersRgb ( double *pR, double *pG, double *pB, char *wTitle
              , void (*rgbCallBack)(double R, double G, double B)
              , void (*redrawMain)(long wid), long widMain, int get_rgb);

int
xy_from_curframe(double *xp, double *yp);

#endif	/* winSliders_h__ */

