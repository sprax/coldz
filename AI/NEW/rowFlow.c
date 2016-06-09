/*****************************************************************************
 rowFlow.c -- Optical Flow.    S.R.Lines    93.6.27-8.16
 Wrap images up in pyramids and call coarseFine to get optical flow fields.
 NB: The image sources, srcA, srcB, etc., must already have borders of size
 >= the formal parameter bord.  If unsure, call the functions here with bord
 == Image_border, the library global, which should be initialized to 5, say,
 and ordinarily never modified.
 
 Laplacian pyramids are the default.
 
 NB: Calling sarnoff routines def_fpyr & alloc_fpyr with sW & sH too small
 will cause an error to be caught in alloc_fimage (fimgalloc.c).  Should this
 be fixed?
 *****************************************************************************/
#define		 UNBUG 0
#include	<stdlib.h>
#include	<string.h>
#include	<un/unBug.h>
#include	<un/unTypes.h>
#include	<un/unFlags.h>
#include	<un/unMacro.h>
#include	<sarnoff/imgdec.h> /* should be changed to get FIMAGE only */
#include	<m/rowCopy.h>
#include	<m/rowMake.h>
#include	<m/rowStat.h>
#include	<m/rowFlow.h>
void coarseFineBac ( FPYR pyr1, FPYR pyr2, FPYR dx, FPYR dy
              , FPYR conf, int botlev, int finlev, int toplev);
extern	int	Image_border;
#define	ImB	Image_border
extern	FILTER	rf, ef;
extern	unl	Options;

#ifndef DEFAULT_PYR_FILTER
#define DEFAULT_PYR_FILTER "/home/vu/spraxlo/c/rh/src/pyrg.fir"
#endif
/************************* "/home/vu/spraxlo/c/rha/src/pyrg.fir"
5 1.0
1   0.0625
2   0.25
3   0.375
4   0.25
5   0.0625
5 1.0
1   0.0625
2   0.25
3   0.375
4   0.25
5   0.0625
*************************************************************/

static flt rf_data[] = { 0.0625, 0.25, 0.375, 0.25, 0.0625 };
static flt ef_data[] = { 0.0625, 0.25, 0.375, 0.25, 0.0625 };
static flt *rf_ptr   = &rf_data[2];
static flt *ef_ptr   = &ef_data[2];


static void
**allocImage(int w, int h, int size, int border)
{
   register char **ptr,**p,**fp;
   register char *area;
   int rsize=(w+2*border)*size;         /* full length of a row */

   ptr = (char **) malloc((h+2*border)*sizeof(*ptr));
   if (!ptr) return (void **) NULL;

   area = (char *) malloc((w+2*border)*(h+2*border)*size);
   if (!area) return (void **) NULL;
   for (p=ptr,fp=p+h+2*border,area+=border*size; p < fp; p++,area += rsize)
     *p = (char *) area;
   return (void **) (ptr+border);
}


static void
allocFimage(FIMAGE *image)
{
int	w=image->w, h=image->h;
   if (!(image->ptr = (float **) allocImage(w,h,sizeof(float),Image_border))) {
     perror("allocFimage");
     die("allocFimage: killed trying to get %d x %d\n",w,h);
   }
}



#if 00	/* simply not used... */
static void
allocFpyr(FPYR pyr, int botlev, int toplev)
{
   int j;

   for (j=botlev; j <= toplev ; j++)
     allocFimage(&(pyr[j]));
}
#endif

/****************************************************************
	This is how it used to be:

   def_fpyr(pyrA,botlev,sW,sH);
   pyrA[botlev].ptr = dupA;
   alloc_fpyr(pyrA,botlev+1,toplev+1);

   def_fpyr  (pyrX,botlev,sW,sH);
   pyrX[botlev].ptr = *floX;
   alloc_fpyr(pyrX,finlev+1,toplev);

*****************************************************************/


void makePyrF(FPYR pyr, int botlev, int toplev, int w, int h, ppf ptr)
{
   int j;

   pyr[botlev].ptr = ptr;
   pyr[botlev].w   = w;
   pyr[botlev].h   = h;
   for (j=botlev-1; j >= 0; j--) {
     pyr[j].w = pyr[j+1].w*2;
     pyr[j].h = pyr[j+1].h*2;
   }
   for (j=botlev+1; MIN(pyr[j-1].w, pyr[j-1].h) > 1; j++) {
     pyr[j].w = (pyr[j-1].w+1)/2;
     pyr[j].h = (pyr[j-1].h+1)/2;
/* @@
     wrn("makePyrF: bot,top:  %d %d  level %d  --   w,h:  %d  %d"
	, botlev, toplev, j, pyr[j].w, pyr[j].h);
*/
     allocFimage(&(pyr[j]));
   }
   for ( ; j < MAXLEV; j++) {
     pyr[j].w   = 0;
     pyr[j].h   = 0;
     pyr[j].ptr = NULL;
   }
}

/*____________________________________________________________________________*
rowFlowBac: Takes only row pointers for images and flow fields, no struct ptrs.
floX,floY should be the flow in srcB's frame from srcB back to srcA;
conf (the confidences) are actually ignored.
flags isn't used now, but was to choose LAPLACIAN or not.
 *____________________________________________________________________________*/

void
rowFlowBac ( ppf floX, ppf floY, ppf cnfd, ppf srcA, ppf srcB
           , int sX, int sY, unt sW, unt sH	
           , int fX, int fY, unt fW, unt fH
           , int toplev, int botlev, int flags)
{
/* static int	virgin=0; */
FPYR	pyrA,pyrB,pyrX,pyrY,pyrC;
int	finlev=0;

#if 0
unt	mW = sW + 2*ImB, mH = sH + 2*ImB;
ppf   dupA = rowMalFlt(   -ImB, -ImB,mW,mH);
ppf   dupB = rowMalFlt(   -ImB, -ImB,mW,mH);
      rowCopyFF(dupA,srcA,-ImB, -ImB,mW,mH,sX-ImB,sY-ImB);
      rowCopyFF(dupB,srcB,-ImB, -ImB,mW,mH,sX-ImB,sY-ImB);
#else
ppf 	dupA = rowDupFlt(srcA,fX,fY,fW,fH);
ppf 	dupB = rowDupFlt(srcB,fX,fY,fW,fH);
#endif

  /****
  if (!virgin++)  wrn("rowFlowBac: botlev %d,  toplev %d", botlev,toplev);
  ****/

#if 00
  if (virgin) {
    char *filtfile = DEFAULT_PYR_FILTER;
    warn("rowFlowBac [%s]: getpyrfilters(DEFAULT_PYR_FILTER= %s)\n",__FILE__,filtfile);
    getpyrfilters(filtfile,&rf,&ef);
    virgin = 0;
  }
#endif
  /* Replace W,H real by nominal values for Sarnoff (librhs)"): */
  sW += 2*sX;
  sH += 2*sY;

/* Set up pyramids for coarse to fine estimation */

   makePyrF(pyrA,botlev,toplev+1,sW,sH,dupA);
   makePyrF(pyrB,botlev,toplev+1,sW,sH,dupB);
   makePyrF(pyrX,botlev,toplev,  sW,sH,floX);	/* for flows, botlev = finlev = 0 */
   makePyrF(pyrY,botlev,toplev,  sW,sH,floY);	/* why is toplev less than for imgs? */
   makePyrF(pyrC,botlev,toplev,  sW,sH,cnfd);	/* why is toplev less than for imgs? */
   
   coarseFineBac(pyrA,pyrB,pyrX,pyrY,pyrC,botlev,finlev,toplev);

#if	UNBUG > 6
  { int p,q,z;
    fprintf(stderr,"DX AFTER COARSE_FINE:::::::::::::::::::::::::::\n");
    for (z = 0; z < 25; z++) {
      p = rand() % sH;
      q = rand() % sW;
      fprintf(stderr,"floX[%d][%d]=%.2g    ",p,q,floX[p][q]);
    }
    fprintf(stderr,"\n");
  }
#endif

  free_fpyr(pyrA,botlev+1,toplev+1); /* should free bottom level, cuz it's now */
  free_fpyr(pyrB,botlev+1,toplev+1); /* the Laplacian image.  But now it's FUDGE */
  free_fpyr(pyrX,finlev+1,toplev);	/* don't free fine level (0) */
  free_fpyr(pyrY,finlev+1,toplev);
  free_fpyr(pyrC,finlev+1,toplev);

  /* warn("rowFlowBac: killing dupA,..."); */
  rowKillFlt(dupA,fX,fY);
  rowKillFlt(dupB,fX,fY);
}

/*****************************************************************************
 newFlow.c -- Optical Flow.    S.R.Lines    93.6.27-8.16
 Wrap images up in pyramids and call coarseFine to get optical flow fields.
 NB: The image sources, srcA, srcB, etc., must already have borders of size
 >= the formal parameter bord.  If unsure, call the functions here with bord
 == Image_border, the library global, which should be initialized to 5, say,
 and ordinarily never modified.
 
 Laplacian pyramids are the default.
 
 NB: Calling sarnoff routines def_fpyr & alloc_fpyr with sW & sH too small
 will cause an error to be caught in alloc_fimage (fimgalloc.c).  Should this
 be fixed?
 *****************************************************************************/
/*____________________________________________________________________________*
newFlowBac: Takes only row pointers for images and flow fields, no struct ptrs.
floX,floY should be the flow in srcB's frame from srcB back to srcA;
conf (the confidences) are actually ignored.
flags isn't used now, but was to choose LAPLACIAN or not.
 *____________________________________________________________________________*/


void
newFlowBac ( ppf *floX, ppf *floY, ppf *conf, ppf srcA, ppf srcB
           , int sX, int sY, unt sW, unt sH	
           , int fX, int fY, unt fW, unt fH
           , int toplev, int botlev, int flags)
{
  FPYR pyrA,pyrB,pyrX,pyrY,pyrC;
  int finlev=0;
  ppf dupA, dupB;

#if 00
  static int virgin=1;
  if (virgin) {
    char *filtfile = DEFAULT_PYR_FILTER;
    warn("newFlowBac [%s]: getpyrfilters(DEFAULT_PYR_FILTER= %s)\n",__FILE__,filtfile);
    getpyrfilters(filtfile,&rf,&ef);
    virgin = 0;
  }
#endif

  if (!flags) {
    dupA = rowMalFlt(   -ImB, -ImB,fW,fH);
    dupB = rowMalFlt(   -ImB, -ImB,fW,fH);
    rowCopyFF(dupA,srcA,-ImB, -ImB,fW,fH,sX-ImB,sY-ImB);
    rowCopyFF(dupB,srcB,-ImB, -ImB,fW,fH,sX-ImB,sY-ImB);
    *floX = rowCalFlt(  -ImB, -ImB,fW,fH);
    *floY = rowCalFlt(  -ImB, -ImB,fW,fH);
    *conf = rowCalFlt(  -ImB, -ImB,fW,fH);
    /*wrn("newFlowBac: NEWK ON:  fXYWH: %d %d %u %u  Nom-sXYWH: %d %d %u %u"
	,fX,fY,fW,fH,sX,sY,sW,sH);*/
  }
  else {
    /* Replace W,H real by nominal values for Sarnoff (librhs)"): */
    sW   += 2*sX;
    sH   += 2*sY;
    dupA  = rowDupFlt(srcA, fX,fY, fW,fH);
    dupB  = rowDupFlt(srcB, fX,fY, fW,fH);
    *floX = rowCalFlt(      fX,fY, fW,fH);
    *floY = rowCalFlt(      fY,fY, fW,fH);
    *conf = rowCalFlt(  -ImB, -ImB,fW,fH);
    wrn("newFlowBac: OLD WAY:  fXYWH: %d %d %u %u  Nom-sXYWH: %d %d %u %u"
	,fX,fY,fW,fH,sX,sY,sW,sH);
  }


/* Set up pyramids for coarse to fine estimation */

   def_fpyr(pyrB,botlev,sW,sH);
   pyrB[botlev].ptr = dupB;

   def_fpyr(pyrA,botlev,sW,sH);
   pyrA[botlev].ptr = dupA;

#ifdef LAPLACIAN	/* undefing LAPLACE gets a seg fault */
   alloc_fpyr(pyrA,botlev+1,toplev+1);
   alloc_fpyr(pyrB,botlev+1,toplev+1);
#else
   alloc_fpyr(pyrA,botlev+1,toplev);
   alloc_fpyr(pyrB,botlev+1,toplev);
#endif
   def_fpyr  (pyrX,botlev,sW,sH);
   pyrX[botlev].ptr = *floX;
   alloc_fpyr(pyrX,finlev+1,toplev);

   def_fpyr  (pyrY,botlev,sW,sH);
   pyrY[botlev].ptr = *floY;
   alloc_fpyr(pyrY,finlev+1,toplev);

   def_fpyr  (pyrC,botlev,sW,sH);
   pyrC[botlev].ptr = *conf;
   alloc_fpyr(pyrC,finlev+1,toplev);

   /* assert(pyrA == NULL); */
   
   coarseFineBac(pyrA,pyrB,pyrX,pyrY,pyrC,botlev,finlev,toplev);

#if	UNBUG > 6
  { int p,q,z;
    fprintf(stderr,"DX AFTER COARSE_FINE:::::::::::::::::::::::::::\n");
    for (z = 0; z < 25; z++) {
      p = rand() % sH;
      q = rand() % sW;
      fprintf(stderr,"*floX[%d][%d]=%.2g    ",p,q,*floX[p][q]);
    }
    fprintf(stderr,"\n");
  }
#endif

#ifdef LAPLACIAN
  free_fpyr(pyrA,botlev+1,toplev+1); /* should free bottom level, cuz it's now */
  free_fpyr(pyrB,botlev+1,toplev+1); /* the Laplacian image.  But now it's FUDGE */
#else
  free_fpyr(pyrA,botlev+1,toplev);
  free_fpyr(pyrB,botlev+1,toplev);
#endif
  free_fpyr(pyrX,finlev+1,toplev);	/* don't free fine level (0) */
  free_fpyr(pyrY,finlev+1,toplev);
  free_fpyr(pyrC,finlev+1,toplev);

  if (!flags) {
    rowKillFlt( dupA,-ImB,-ImB);
    rowKillFlt( dupB,-ImB,-ImB);
    rowKillFlt(*conf,-ImB,-ImB);
    *floX = rowPntFlt(*floX,fX,fY,-ImB,-ImB,fW,fH);
    *floY = rowPntFlt(*floY,fX,fY,-ImB,-ImB,fW,fH);
  } else {
    rowKillFlt( dupA,fX,fY);
    rowKillFlt( dupB,fX,fY);
    rowKillFlt(*conf,fX,fY);
  }
}


/**************************************************************
 Lucas-Kanade optical flow algorithm: Amnon Shashua, March 1993
 From RH.
 **************************************************************/
#define BEFORE		/* smoothing of residual velocity */
#define NBLUR	1	/* Number of smoothing levels */
#define LIMIT		/* limit residual velocities (only gradient method) */
#define EXTEND

void
coarseFineBac ( FPYR pyr1, FPYR pyr2, FPYR dx, FPYR dy
              , FPYR conf, int botlev, int finlev, int toplev)
{
  FIMAGE warped;
  int lev;
  FILTER trF, teF;
 
#if 00
    trF.k = rf.k;  trF.taps = rf.taps;
    teF.k = ef.k;  teF.taps = ef.taps;
#else
    trF.k = rf_ptr;  trF.taps = 5;
    teF.k = ef_ptr;  teF.taps = 5;
    rf.k  = trF.k;   rf.taps  = trF.taps;
    ef.k  = teF.k;   ef.taps  = teF.taps;
#endif

  reduce(pyr1,botlev,toplev+1,trF);
  reduce(pyr2,botlev,toplev+1,trF);
  if ( ! (Options & O_GRAY)) {
    expand(pyr1,botlev,toplev+1,teF,-1);
    expand(pyr2,botlev,toplev+1,teF,-1);
  }

/* Start coarse to fine motion estimation */
   for (lev=toplev; lev >= finlev; lev--) {
     if (lev != toplev) {	/* expand previous levels velocity estimate */
       fmultiply(dx[lev+1],2.0);
       fmultiply(dy[lev+1],2.0);
       expand(dx,lev,lev+1,teF,0);
       expand(dy,lev,lev+1,teF,0);
     } else {
       fset(dx[lev],0.0);
       fset(dy[lev],0.0);
     }
     warped.w = pyr1[lev].w; warped.h = pyr1[lev].h; alloc_fimage(&warped);
     bilinear_warp(pyr1[lev],warped,dx[lev],dy[lev],-1.0);
     lsq_lsqgrad_fast(warped,pyr2[lev],dx[lev],dy[lev],conf[lev]);
#ifdef BEFORE
/* Blur velocity fields */
     blur(dx[lev],NBLUR);
     blur(dy[lev],NBLUR); 
#endif
     if (lev != toplev) {	/* add in previous estimates to current */
       expand(dx,lev,lev+1,teF,1);
       expand(dy,lev,lev+1,teF,1);
     }
#ifdef AFTER
/* Blur velocity fields */
     blur(dx[lev],NBLUR);
     blur(dy[lev],NBLUR);
#endif
     free_fimage(warped);
   }
}



void
fxyFlowBac ( ppf abX, ppf abY, ppf dwT, ppf sgA, ppf sgB
           , int fX, int fY, unt fW, unt fH
           , int sX, int sY, unt sW, unt sH, dbl *stdX, dbl *stdY
	   , int pyrLevs)	/**** pyrLevs usually = 4. ****/
{
  rowFlowBac(abX,abY,dwT,sgA,sgB,sX,sY,sW,sH,fX,fY,fW,fH,pyrLevs,0,0);
  /* pyrLevs usually = 4.  B<-A, dwT disregarded */
  if (stdX && stdY)
    fxyStat(stdX,stdY,abX,abY,sX,sY,sW,sH,"BacInv: B<-A",0);
}

