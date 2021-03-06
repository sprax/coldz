/*****************************************************************************
 ggStruct.h     S.R.Lines    94.6.4
 *****************************************************************************/
#ifndef ggStruct_h__
#define ggStruct_h__

typedef struct  Dim {   /* Dimensions of a 2-D image */
        unt     w,h;    /* width, height: should be >= 0; replaces sW,sH, etc*/
        unt     d;      /* depth: 1=gry,2=gry+alpha,3=rgb,...?*/
      /*unt     k; */   /* kind: ?? */
      /*unt     c; */   /* color? */
        int     l,t;    /* left, top offsets: dist. ptr loc. to mem. origin. */
                        /* Should equal sX,sY, etc (sign AND magnitude)      */
                        /* So l,t < 0 means bordered; l,t > 0 means cropping */
        int     r,b;    /* right, bottom borders; positive border if r,b > 0 */
        int     x,y;    /* some xy-point, e.g. corner of nominal image rect. */
}       Dim,  *pDim;

typedef struct  rexy {
        flt x1; flt y1;
        flt x2; flt y2;
}       rexy, *prxy;

typedef struct  affTrf {
	flt	diag;	/* a11 & a22, where a = scale*rotMatrix */
	flt	sr21;	/* a21, -a12 */
	flt	scale;	/* 1.0 == 100% */
	flt	angle;	/* -M_PI to +M_PI (from acos) */
	flt	xoff;	/* offsets from origin */
	flt	yoff;
	flt	xtrn;	/* translations to dst */
	flt	ytrn;
}       affTrf;

typedef struct  delEyeLoc {
        flt olx; flt orx; flt oly; flt ory;
        flt nlx; flt nrx; flt nly; flt nry;
}       delEyeLoc;

#endif	/* ggStruct_h__ */

