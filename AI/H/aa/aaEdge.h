/****** FILE: aaEdge.h -- prototypes & macros for functions in aaEdge.c.
        AUTH: S.R. Lines  (srl)
        DATE: 15.10.92  (Orig. 9.92) *********/

#ifndef _aaEdge_h_
#define _aaEdge_h_

#ifndef _NO_PROTO

extern  int  ThinEdgesBin (unc *gimp, int wide, int tall);
extern  int  EdgeGradSobel (puc image, puc emag, pfl etheta, int cols, int rows, double scale);
extern  puc  EdgeMagSobel (puc dst,pcuc src,int wide,int tall,int thresh,double scale);
extern  int  ThinEdgeBin (unc *gimp, int wide, int tall);
extern  int  ThinEdgeZero (unc *src, int wide, int tall);
extern  int  ThinEdgeThresh (unc *src, int wide, int tall, int thresh);
extern  int* LaplacianGray (int *dst, pcuc src, int wide, int tall);
extern  int  xor_hvd_edges (register unc  *up, int cols, int rows);
extern  int  ThinEdgeGray (unc *gimp, int wide, int tall);
extern  int  xor_edges (register unc *up, int cols, int rows);
extern  int  xor_hv_edges (register puc image, int cols, int rows);
extern  int  xor_d_edges  (register puc image, int cols, int rows);
extern  int  xor_v_edges  (register puc image, int cols, int rows);
extern  int  xor_h_edges  (rgst puc image, int cols, int rows);
extern  int  srlm_edges_thresh (puc image, puc omage, int cols, int rows, int thresh);

#else   /* ifndef _NO_PROTO */

extern  int  EdgeGradSobel ();
extern  puc  EdgeMagSobel ();
extern  int  ThinEdgesBin ();
extern  int  ThinEdgeZero ();
extern  int  ThinEdgeThresh ();

#endif  /* _NO_PROTO */

#endif  /* _aaEdge_h_ */


