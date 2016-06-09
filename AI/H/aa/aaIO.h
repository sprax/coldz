/*****  aaIO.h -- prototypes & macros for functions in aaIO.c.
        S.R. Lines  (srl)
        5.9.92  (Orig. 9.92) *****/
        
#ifndef _aaIO_h_  /* Should be included after aa.h */
#define _aaIO_h_

#ifndef  _NO_PROTO

extern  int  Print3x3 (puc pc, int cols, int w);
extern  int  Print5x5 (puc pc, int cols, int w);
extern  void MoreF (float *src, size_t size, int nLines, int nPerLine, int space, int prec, char *label);
extern  void MoreU (puc src, size_t size, int nLines, int nPerLine, int space, char *label);
extern  int  LoadBitmapRGBI (char *path, int wide, int tall, unc **iptr);
extern  int  LoadImagesDaveG (char *path,int *maxw,int *maxt,puc cip24,puc gip8,puc red8,puc grn8, puc blu8);
extern  int  LoadImagesGRGBB (FILE *fp,int wide,int tall,puc rgbi,puc gray,puc red,puc grn, puc blu);
extern  int  LoadImagesRGBB (FILE *fp, int wide, int tall, puc clr24, puc red, puc grn, puc blu);
extern  int  LoadRGBImagesDave (char *path,int *maxw,int *maxt,puc cip24,puc red8,puc grn8,puc blu8);
extern  int  WriteImageDave (rgst puc  source, char *name, char *ext, int cols, int rows);
extern  int  WriteImageDave3 (puc red,puc grn,puc blu,char *name,char *ext,int cols,int rows);
extern  int  WriteImageRaw (puc src, char *name, char *ext, int wide, int  tall, int nChan);
extern  size_t  Header2LGet (FILE *fp, int *maxw, int *maxt);
extern  size_t  Header2LPut (FILE *fp, int wide, int tall);
extern  puc  GetGrayRaw (void);
extern  puc  LoadImageDave (char *path, int *wide, int *tall);
extern  puc  LoadImageRaw (char *path, int wide, int tall, int nChan, long headerBytes); 
extern  puc  LoadImageRGBB (FILE *fp, int wide, int tall);
extern  puc  LoadImageRGBBasGray (char *path, int *maxw, int *maxt);

#else   /*   ifndef _NO_PROTO */

extern  int  Print3x3 ();
extern  int  Print5x5 ();
extern  void MoreF ();
extern  void MoreU ();
extern  int  LoadBitmapRGBI ();
extern  int  LoadImagesDaveG ();
extern  int  LoadImagesGRGBB ();
extern  int  LoadImagesRGBB ();
extern  int  LoadRGBImagesDave ();
extern  int  WriteImageDave ();
extern  int  WriteImageDave3 ();
extern  int  WriteImageRaw ();
extern  size_t  Header2LGet ();
extern  size_t  Header2LPut ();
extern  puc  GetGrayRaw ();
extern  puc  LoadImageDave ();
extern  puc  LoadImageRaw ();
extern  puc  LoadImageRGBB ();
extern  puc  LoadImageRGBBasGray ();

#endif  /* _NO_PROTO */

#endif  /* _aaIO_h_ */


