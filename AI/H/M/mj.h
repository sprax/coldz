/*****************************************************************************
 mj.h     S.R.Lines    94.6.4
 *****************************************************************************/
#ifndef mj_h
#define mj_h

#define MJ_INVTY	1

typedef struct  Dmj { /* Dims for 2-D image, row ptrs into contig. mem */
unt     w,h;    /* width, height: should be >= 0; replaces sW,sH, etc  */
int     x,y;    /* x,y offsets to physmem origin, replaces sX,sY, etc  */
int     l,r;    /* left & right x-values of a graphics rect, aka x1,x2 */
int     t,b;    /* top & bottom y; rect is {x,y: x1<=x<=x2, y1<=y<=y2} */
unt     d;      /* depth: 1=gry,2=gry+alpha,3=rgb,4=rgba or rgbv       */
/*unt   k; */   /* kind? */
/*unt   c; */   /* color? */
}       Dmj,  *pDmj;

#define TMAX	200


typedef struct  Tetra {
        float   tetA[TMAX][4][2];
        float   tetB[TMAX][4][2];
        float   tetC[TMAX][4][2];
        int     ntr;    /* number of triangles or tetragons */
        int     nvt;    /* number of vertices */
        pDmj    dim;
        ppu    *urgb;   /* gray level (0.0 - 255.0)  */
        ppf    *frgb;   /* interleaved RGB */
        ppf    *fgry;   /* gray level (0.0 - 255.0)  */
        long    wid;    /* id of window to draw in */
        float   nzX;	/* new Zoom X */
        float   nzY;	/* new Zoom Y */
}       Tetra;          /* src & dst relative to inverse warping */

typedef struct  FloXY {
        pDmj    dim;
        pDmj    dmf;
        ppf    *fvX;    /* X vector flow  */
        ppf    *fvY;    /* Y vector flow  */
        ppf    *fvC;    /* confidences */
        ppf    *rgb;    /* interleaved RGB */
        ppf    *gray;   /* gray level (0.0 - 255.0)  */
        ppf     wts;    /* same dims as gray */
        ppf     acx;    /* accumulator for x components  */
        ppf     acy;    /* accumulator for y components  */
        ppf     acz;    /* accumulator for z components  */
        long    wid;    /* id of window to draw flow in */
        Tetra *tetra;   /* tetra-data for drawing back into FloXY windows */
}       FloXY;          /* src & dst relative to inverse warping  */


/* NB: x & y equal the old sX,sY in magnitude AND sign, thus are < 0 for    */
/* left & top margins of size > 0, whereas l,r,t,b are coord values not     */
/* necessarily relating to physical memory; as points (r,t) are part of     */
/* the image, not the limits outside it; thus (l,r,t,b) = (x,w-1,y,h-1)     */
/* is common, but values (x,w,y,h) or (x,y,w,h) are likely a mistake.       */
/* No dogma here about Y increasing upwards or down (right vs left handed   */
/* screen or viewing coord systems).  Sun programmers think down, SGI up.   */

void mjPrint(pDmj dm, char *tag);

typedef struct  Brd {	/* Border specs, usually all >= 0 */
        int l; int t;
        int r; int b;
}       Brd, *pBrd;

typedef struct  Rejy {	/* Rectangular mem region, integer offsets & sizes */
        int x; int y;
        unt w; unt h;
}       Rejy, *pRejy;

typedef struct  Rexy {	/* Rectangular graphics region, float coords */
        flt x1; flt y1;
        flt x2; flt y2;
}       Rexy, *pRexy;

typedef struct  LblFace {  /* Rectangular graphics region, float coords */
        flt lex; flt ley;
        flt rex; flt rey;
	flt nox; flt noy;
	flt mex; flt mey;	/* misc. extra point */
}       LblFace;


typedef struct  AffTrf {
        flt     scale;  /* 1.0 == 100% */
        flt     angle;  /* -M_PI to +M_PI (from acos) */
        flt     cosa;   /* a11 & a22, where a = scale*rotMatrix */
        flt     sina;   /* a21, -a12 */
        flt     xdst;
        flt     ydst;   /* origin in dest. coords, eg, left eye in dst image */
        flt     xsrc;
        flt     ysrc;   /* origin in source coords, eg, left eye in src img  */
}       AffTrf;		/* src & dst relative to inverse warping, ie,        */
			/* xs = xS + scale*((xd - xD)*cos + (yd - yD)*sin)   */


typedef struct  delEyeLoc {
        flt olx; flt orx; flt oly; flt ory;
        flt nlx; flt nrx; flt nly; flt nry;
}       delEyeLoc;

#define DIM_PAR(M,X,Y,W,H,D) {M.x = X, M.y = Y, M.w = W, M.h = H, M.d = D;}


#endif	/* mj_h */

