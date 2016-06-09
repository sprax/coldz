/*****************************************************************************
 winDraw.h    S.R.Lines    94.8.29
#include        "mj.h"	-- for Dmj struct (dimensions of an mj image) 
 *****************************************************************************/
#ifndef winDraw_h__
#define winDraw_h__

#define MAXCNT   1024
#define MDRAW      17      /* arbitrary nonsense */
#define SLEN      256
#define MAXTRI     64
#define MAXPRS     (6*MAXTRI)
#define SQRT2   (1.414213562373095049)

typedef struct  GridV4 { /* Vertex in a 4-connected grid */
int	stat;   /* Current state or whatever; init to 0 (calloc)  */
flt	xy[2];	/* coords of this vertex */
int	nn[4];	/* indices of connected vertices */
unt	clr;	/* use for indexed or packed color */
}   GridV4;


void readICCVparams(void);

double polyDiff(float pA[][2], float pB[][2], int nvt);
int reorderPoly ( float rord[][2], float poln[][2], int nvt);
int reordPoly2 ( flt rA[][2], flt rB[][2], flt oA[][2], flt oB[][2], int nvt);

double rectBoundsFromPoly (float rectp[4], float polyp[][2], int npts);
void scalePoly2 (flt dstp[][2], flt srcp[][2], unt npts, dbl xfac, dbl yfac);
void scaleOffsetPoly2 (float dstp[][2], float srcp[][2], unt npts
         , double xfac, double yfac, double xoff, double yoff);

void cutItTet (FloXY *flow, float poln[][2], int num);
void drawExCtr (long xx, long yy, long rad);
void drawCrossCtr (double xx, double yy, double rad);
void drawTet (float vtx[][2]);

extern void markFlowTri  (void *v, int device, int flags);
extern void moveFlowTri  (void *v, int device, int flags);
extern void markFlowTet2 (void *v, int device, int flags);
extern void moveFlowTet2 (void *v, int device, int flags);
extern void markFlowTet3 (void *v, int device, int flags);
extern void moveFlowTet3 (void *v, int device, int flags);
extern void moveFlowTetC (void *v, int device, int flags);
extern void markCrsTet2  (void *v, int device, int flags);
extern void moveCrsTet2  (void *v, int device, int flags);
extern void markCrs2     (void *v, int device, int flags);
extern void moveCrs2     (void *v, int device, int flags);
extern void testWarpTet  (void *v, int device, int flags);
extern void testChain    (void *v, int device, int flags);
extern void backTrack    (void *v, int device, int flags);
extern void rubberRectZoid (void *v, int device, int flags);
extern void rubberRectMag2 (void *v, int device, int flags);
extern void markCrsTri2    (void *v, int device, int flags);
extern void moveCrsTri2    (void *v, int device, int flags);
extern void rubberRectMag3 (void *v, int device, int flags);
extern void markChain3     (void *v, int device, int flags);
extern void moveChain3     (void *v, int device, int flags);
extern void zoomRectTri2   (void *v, int device, int flags);
extern void markSub2       (void *v, int device, int flags);
extern void moveSub2       (void *v, int device, int flags);
extern void winEditMenu2   (void *v, int device, int flags);


#endif	/*winDraw_h__*/


