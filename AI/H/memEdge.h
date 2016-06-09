/*****************************************************************************
memEdge.h    S.R.Lines (spraxlo@ai.mit.edu)    93.7.11-22
Doesn't yet handle interleaving of different types.
*****************************************************************************/
         
#ifndef  memEdge_h__
#define  memEdge_h__

/* nearest.c */
int NearestEdgePixelGray(unc *bitmap, int thresh, int maxdist, int wide, int tall, int locX, int locY, int *newX, int *newY);
int NearestEdgePixelRGBI(unc *pixmap, int thresh, int maxdist, int wide, int tall, int locX, int locY, int *newX, int *newY);

#define  REDI (0.30)   /* Factory calibration values for phosphors in HP */
#define  GRNI (0.59)   /* bobcat monitors (98721 or whatever).  All monitors */
#define  BLUI (0.11)   /* are different; see Foley & van Dam for typical values. */
         /* Use other values for non-standard conversions/color edge tracing, etc.  */

                    /*____ RGB to Gray Conversion ****/
#define  RGBI2G(p)  ((int)(REDI*(*(p)) + GRNI*(*((p)+1)) + BLUI*(*((p)+2))))

                     /*____ NEIGHBORS AND EDGES ____*/
                /**** Light neighbors && dark interiors --> edge ****/
#define  LNEIGHBOR_GRAY(p,t,c)  (*((p)+1) > t||*((p)-1) > t||*((p)-c) > t||*((p)+c) > t)
#define  THIN_EDGE_GRAY(p,t,c)   (*(p) <= t && LNEIGHBOR_GRAY(p,t,c))

#define  LNEIGHBOR_RGBI(p,t,c3)                   /* check only on 4 sides. */\
         (RGBI2G(p+3) > t || RGBI2G(p-3) > t || RGBI2G(p+c3) > t || RGBI2G(p-c3) > t)  
#define  THIN_EDGE_RGBI(p,t,c3)   ((RGBI2G(p) <= t) && LNEIGHBOR_RGBI(p,t,c3))

#endif   /* memEdge_h__ */


