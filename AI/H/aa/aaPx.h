/*****************************************************************************
 aaPx.h    S.R.Lines  (spraxlo@{ai|media}.mit.edu)     92.5-93.6.1
 *****************************************************************************/
#ifndef  _aaPx_h_
#define  _aaPx_h_

typedef struct Px { 
               int wide, tall;     /* width & height in pixels     */
               int nChan;          /* usually == bytes per pixel. */
               size_t size;        /* usu. (size_t)wide*tall*nChan         */
               i32 kind;           /* E.g., RGBI, RGBP, RGBIT, Gray256,... */
               unsigned char *pp;  /* pointer to first byte of 1-D array.  */
               char *path;         /* pointer to path string */
             } Px, *pPx;           /* NB: (const *Pf) != (const pPf) == (Pf *const) */

typedef  const Px   cPx, *pcPx;
typedef  Px   *Pxp;


#ifdef   MAC
#define  PxOut(px,name,ext,ix,iy)  PxWrite(px,name,ext,0); //PxFree(px);
#else
#define  PxOut(px,name,ext,ix,iy)  \
         fprintf(stderr,"PxOut: %s.%dx%dx%d\n",name,px->wide,px->tall,px->nChan);
#endif

#ifndef  _NO_PROTO

extern  pPx  PxNew(int wide, int tall, int nChan, i32 kind, puc pp, char *path);
extern  pPx  PxClone(pcPx src);
extern  void PxPrint(pcPx px);
extern  void PxErr(pcPx px);
extern  void PxFree(Px *px);
extern  puc  GetPxChan(pcPx px, int chan);
extern  int  PxMeanValue(pcPx px);
extern  pPx  PxReframe(pcPx src, int padval, int dwide, int dtall, int flags);
extern  int  PxShow (pcPx src, char *title, int x, int y, int flags);
extern  int  PxWrite (pcPx src, char *name, char *ext, int flags);
extern  pPx  PxRead (char *path, int wide, int tall, int nChan, long kind, long headerBytes); 
extern  int  PxWriteDave (pcPx src, char *name, char *ext);
extern  int  PxMeanValue (pcPx px);
extern  int  PxWriteDave (pcPx src, char *name, char *ext);
extern  pPx  PxClone (pcPx src);
extern  pPx  PxGaussianBlur (pcPx src, int xlen, int ylen, int flags);
extern  pPx  PxGaussianBlurf (pcPx src, int xlen, int ylen, int flags);
extern  pPx  PxLog (pcPx src, dbl xfrac, dbl yfrac, dbl xbeta, dbl ybeta);
extern  pPx  PxNew (int wide, int tall, int bpp, i32 kind, puc pp, char *path);
extern  pPx  PxPow (pcPx src, dbl xfrac, dbl yfrac, dbl xexp, dbl yexp);
extern  pPx  PxPxCenter (pcPx src,int mwide,int mtall,int padval);
extern  pPx  PxRadialBlur (pcPx src, int blurRadius, int flags);
extern  pPx  PxRadialBlurf (pcPx src, int blurRadius, int flags);
extern  pPx  PxReframe (pcPx src, int padval, int dwide, int dtall, int flags);
extern  pPx  PxSqr (pcPx src, dbl xfrac, dbl yfrac);
extern  pPx  PxSqrOld (pcPx src, dbl xfrac, dbl yfrac);
extern  pPx  PxSqrDemi (pcPx src, dbl xfrac, dbl yfrac);
extern  pPx  PxSqrSemi (pcPx src, dbl xfrac, dbl yfrac);
extern  pPx  PxSqrPeri (pcPx src, dbl xfrac, dbl yfrac);
extern  pPx  PxSqrPer2 (pcPx src, dbl xfrac, dbl yfrac);
extern  pPx  PxSqrDiag (pcPx src, dbl xfrac, dbl yfrac);
extern  pPx  PxSqrt (pcPx src, dbl xfrac, dbl yfrac);
extern  puc  PxGetChan (pcPx pms, int chan);
extern  void PxErr (pcPx px);
extern  void PxFree (Px *px);
extern  void PxPrint (pcPx px);

#else   /*   ifndef _NO_PROTO */

extern  int  PxMeanValue ();
extern  int  PxWriteDave ();
extern  pPx  PxClone ();
extern  pPx  PxGaussianBlur ();
extern  pPx  PxGaussianBlurf ();
extern  pPx  PxRead ();
extern  pPx  PxLog ();
extern  pPx  PxNew ();
extern  pPx  PxPow ();
extern  pPx  PxPxCenter ();
extern  pPx  PxRadialBlur ();
extern  pPx  PxRadialBlurf ();
extern  pPx  PxReframe ();
extern  pPx  PxSqr ();
extern  pPx  PxSqrOld ();
extern  pPx  PxSqrt ();
extern  puc  GetPxChan ();
extern  void PxErr ();
extern  void PxFree ();
extern  void PxPrint ();

#endif  /* _NO_PROTO */

#endif  /* _aaPx_h_ */

