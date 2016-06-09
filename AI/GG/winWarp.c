/*****************************************************************************
 winWarp.c    S.R.Lines    95.05.21 - 95.09.10
#include	<SGI/port.h>
#include	"gg.h"
#include	"ggExtern.h"
#define 	UNBUG	0
#define 	UNTIME	0
#include	<stdlib.h>
 *****************************************************************************/
#include	<malloc.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<time.h>
#include	<gl/gl.h>
#include	<gl/device.h>
#include 	<gl/get.h>
#include	<SGI/image.h>
#include	<SGI/dispimg.h>
#include 	<SGI/gfxmach.h>
#include	<SGI/vect.h>
#include	<SGI/vcolor.h>
#if	00
#include	<CC/osfcn.h>	/* actually varies with OS version */
#endif
#include        <un/unFlags.h>
#include        <un/unBug.h>
#include        <un/unMacro.h>
#include        <un/unTypes.h>
#include        <m/mj.h>
#include        <m/fxyComb.h>
#include        <m/rgbMark.h>
#include        <m/rowCopy.h>
#include        <m/rowCpack.h>
#include        <m/rowFile.h>
#include        <m/rowFlow.h>
#include        <m/rowSmat.h>
#include        <m/rowSpat.h>
#include        <m/rowPlug.h>
#include        <m/rowSGI.h>
#include        <m/rowWarp.h>
#include        <winLoop.h>
#include        <winDraw.h>
#include        <winSliders.h>
#include	"inExtern.h"
#include	"inParm.h"
#include	"mob.h"

#ifdef WIN32
#define popen   _popen  /* FILE	*popen(char *, char *); */
#define pclose  _pclose /* void     pclose(FILE *); */
#else 
#include <unistd.h>	/* sleep(uint); */
#endif

long    InSlidersWid = 0;
long    InMainWid    = 0;
long    InShowWid    = 0;
long	InEditIndex  = 0;    /* @@ PROBLEM: winclose won't reset */
int 	MoveGridA    = 0;
int 	MoveGridB    = 0;
int 	ResetGrid    = 0;
int 	UseFlowAB    = 1;
int 	ShowGridA    = 0;
int 	ShowGridB    = 0;

extern 	int 	GrdX, GrdY;
extern  int     findname(char *name, char *fullname,char *GFXpath);
extern	void	unc_to_wp(rgst puc rowU, rgst pus wp, rgst int n, rgst int y);
extern	int	gfxmachine(void);			      /* glstuff.c */
extern	short	**makedittab(int levels, int mult, int add);  /* dispimg.c */
extern	int	prefdrawmode(int type);                       /* dispimg.c */
extern	void	pseudorgb(void);			      /* vcolor.c */

/* kinds of image files */

#define BWIMG           0
#define RGBIMG          1
#define SCREENIMG       2

#define XSIZE   4
#define YSIZE   4
#define TOTAL           (XSIZE*YSIZE)
#define WRAPY(y)        ((y)%YSIZE)

static	DISPIMAGE *movie[MAXFRAMES];
static	DISPIMAGE *movieS[32];
static	char	Ss[256], winTitle[320];
static 	long    orgX, orgY;
static	int	wdMain, htMain, wdShow, htShow;
static	int 	imgmode = RGBIMG;
static  long	curdispmode;
static	int 	nframes=0, nframeS=0;
static	int 	curframe=0, curframeS=0;
static	int 	direction = FORWARD;
static	flt 	zoom=2.0;
static	flt 	Clrs[MAXPARM];



int
copyFlowToUndo( pMD wwD, ppf floX, ppf floY, int nB)
{
pDmj    fD  = wwD->fD;  int  fX = fD->x, fY = fD->y, fW = fD->w, fH = fD->h;

  InUndoIndex = nB;
  rowCopyFF(FeX[2],floX,fX,fY,fW,fH,fX,fY);
  rowCopyFF(FeY[2],floY,fX,fY,fW,fH,fX,fY);
  return InUndoIndex;
}

int
copyFlowFrUndo( pMD wwD, ppf floX, ppf floY, int nB)
{
pDmj    fD  = wwD->fD;  int  fX = fD->x, fY = fD->y, fW = fD->w, fH = fD->h;

  if (InUndoIndex != nB) {
    warn("copyFlowFrUndo: Can't Undo %d",InUndoIndex);
    return 0;
  }
  InRedoIndex = nB;
  rowCopyFF(FeX[0],floX,fX,fY,fW,fH,fX,fY);
  rowCopyFF(FeY[0],floY,fX,fY,fW,fH,fX,fY);
  rowCopyFF(floX,FeX[2],fX,fY,fW,fH,fX,fY);
  rowCopyFF(floY,FeY[2],fX,fY,fW,fH,fX,fY);
  return InRedoIndex;
}


int
copyFlowFrRedo( pMD wwD, ppf floX, ppf floY, int nB)
{
pDmj    fD  = wwD->fD;  int  fX = fD->x, fY = fD->y, fW = fD->w, fH = fD->h;

  if (InRedoIndex != nB) {
    warn("copyFlowFrRedo: Can't Redo %d",InRedoIndex);
    return 0;
  }
  rowCopyFF(floX,FeX[0],fX,fY,fW,fH,fX,fY);
  rowCopyFF(floY,FeY[0],fX,fY,fW,fH,fX,fY);
  return InRedoIndex;
}


#define   UNTIME 1
#include <un/unTime.h>

static void
warpNewFrame ( pMD wwD, flt clrs[MAXPARM], int numSrc)
{
pfl 	parm = wwD->parm[nframes];  int j;
pDmj    fD   = wwD->fD;   int fW = fD->w, fH = fD->h;

  
  for (j = 0; j < MAXPARM; j++)
    parm[j] = clrs[j]; 

#if 1111
  if (Devopts & O_UNCH) {
    int q;
    warn("Going to time rgbFwd/Bak %d:  %d x %d   %d times..."
	,numSrc,fW,fH,Ntweens);
    BEGCLOCK("");
    for (q = 0; q < Ntweens; q++) {

      switch (numSrc) {
        case  2: rgbBil2    (wwD,clrs);     break;
        case  3: rgbFwdBil3 (wwD,clrs);     break;
        case  4: rgbBil4    (wwD,clrs);     break;
        case  6: rgbFwdBil6 (wwD,clrs);     break;
        case  8: rgbBil8    (wwD,clrs);     break;
        case  9: rgbFwdBil9 (wwD,clrs);     break;
        case 12: rgbFwdBil12(wwD,clrs);     break;
        default: die("warpNewFrame [%s]: NOIMP for numSrc: %d",__FILE__,numSrc);
      }
    }
    ENDCLOCK("Done timing");
    die("time was of the essence.");
  }
#endif

  switch (numSrc) {
    case  2: rgbBil2    (wwD,clrs);	break;
    case  3: rgbFwdBil3 (wwD,clrs);	break;
    case  4: rgbBil4    (wwD,clrs);	break;
    case  6: rgbFwdBil6 (wwD,clrs);	break;
    case  8: rgbBil8    (wwD,clrs);	break;
    case  9: rgbFwdBil9 (wwD,clrs);	break;
    case 12: rgbFwdBil12(wwD,clrs);	break;
    default: die("warpNewFrame [%s]: NOIMP for numSrc: %d",__FILE__,numSrc);
  }
}


static DISPIMAGE *
dispimgRgb (pWmj dWin, ppf srcf, pDmj dD)
{
DISPIMAGE	*di;
register unl	*lptr;
int 		y;

  di = (DISPIMAGE *)calloc(1,sizeof(DISPIMAGE));	/* size: 20 bytes */
  di->xsize = dD->w;
  di->ysize = dD->h;
  di->type  = prefdrawmode(imgmode);
  /* warn("di->type is %d",di->type); */
  /* Assuming di->type is DI_LRECTWRITE: */
#ifdef INDY
  switch(di->type) {
    case DI_WRITERGB:
    case DI_LRECTWRITE:
      if(curdispmode != DMRGB && curdispmode != DMRGBDOUBLE)
        pseudorgb();
      break;
    case DI_WRITEPIXELS:
    case DI_RECTWRITE:
      if(curdispmode != DMSINGLE && curdispmode != DMDOUBLE) {
        singlebuffer();
        gconfig();
      }
    break;
  }
#else
  if (di->type != DI_LRECTWRITE) {
    warn("dispimgRgb: di->type:%d != DI_LRECTWRITE:%d",di->type,DI_LRECTWRITE);
  }
#endif


  y = di->xsize * di->ysize * sizeof(long);
  if ( ! (di->data = (puc)malloc(y)))
    die("dispimgRgb: malloc(%d)",y);

  dWin->P = lptr = (unsigned long *)di->data;

#if 1
  mjPackFltI(lptr,srcf,dD);
#else
  for (y = 0; y < di->ysize; y++, lptr += di->xsize)
    cpackFltRGBI(lptr,srcf[y],di->xsize);
#endif

  return di;
}



static void
makeframeS(int frameno)
{
    curframeS = frameno;
    viewport(0,wdShow-1,0,htShow-1);
    ortho2(-0.5F, wdShow-0.5F, -0.5F, htShow-0.5F);
    drawimage(movieS[frameno],0,0);
}


static void
makeframe(int frameno)
{
    curframe = frameno;
    viewport((short)0,wdMain-1,0,htMain-1);
    ortho2(-0.5F, wdMain-0.5F, -0.5F, htMain-0.5F);
    drawimage(movie[frameno],0,0);
}


static void
gotoframeS (int n)
{
  if (n >= nframeS)	n = nframeS-1;
  if (n < 0)		n = 0;
  if (n != curframeS) {
    curframeS = n;
    makeframeS(curframeS);
    swapbuffers();
  }
}


static void
gotoframe (int n)
{
  if (n >= nframes)	n = nframes-1;
  if (n < 0)		n = 0;
  if (n != curframe) {
    curframe = n;
    makeframe(curframe);
    swapbuffers();
  }
}


static void
reframe (long wid)
{
  long tmp =  winget();
  if  (tmp != wid)  winset(wid);
  reshapeviewport();
  frontbuffer(1);
  makeframe(curframe);
  frontbuffer(0);
  winset(tmp);
}

static void
back(void)
{
    curframe--;
    if(curframe<0)
	curframe = nframes-1;
    makeframe(curframe);
    swapbuffers();
}

static void
forward(void)
{
    curframe++;
    if(curframe >= nframes)
	curframe = 0;
    makeframe(curframe);
    swapbuffers();
}

static void
winWarpStep(void)
{
    if(direction == FORWARD)
	forward();
    else
	back();
}

static void
winWarpSwing(void)
{
    if(curframe == 0)
	direction = FORWARD;
    if(curframe == nframes-1)
	direction = BACK;
    winWarpStep();
}



#if 000
#define outCn(frp,x,y,w,h,dir,name,num,tot)     \
        sprintf(path,"%s/%s.%0*d",dir,name,DECIMALS(tot),num);\
        rgbWriteSGIn(frp,x,y,w,h,path);
#endif

extern FILE *LogFile;
double GloX,GloY,GloZ;

int showNewFrameMain(pMD wwD)
{
pDmj    dD  = wwD->dD;  int  dX = dD->x, dY = dD->y, dW = dD->w, dH = dD->h;
long	wid = winget();
ppf 	dst = wwD->dst;
int 	j;

  winset(wwD->dWin->id);
  movie[nframes] = dispimgRgb(wwD->dWin,dst,dD);
  wwD->frames[nframes] = wwD->dWin->P;
  makeframe(nframes);
  swapbuffers();
  
  if (Devopts & O_WRIT) {
    sprintf(Ss,"%s/v.%0*d",ArgStr[NumSrc+2],3,nframes);
    rgbWriteSGIn(dst,dX,dY,dW,dH,Ss);
    sprintf(Ss,"%s/v.%0*d",ArgStr[NumSrc+2],3,nframes);
    /****
    fprintf(LogFile,"XYZ %3d: %.2lf %.2lf %.2lf  " ,nframes,GloX,GloY,GloZ);
    fprintf(stderr ,"XYZ %3d: %.2lf %.2lf %.2lf\n" ,nframes,GloX,GloY,GloZ);
    *****/
    for (j = 0; j < NumSrc; j++)
      fprintf(LogFile," %.2f",Clrs[j]);
    fprintf(LogFile,"\n");
    fflush(LogFile);
  }

  winset(wid);
return ++nframes;
}


int showNewFrameShow (pMD wwD)
{
long wid = winget();
  winset(wwD->sWin->id);
  movieS[nframeS] = dispimgRgb(wwD->sWin,wwD->dsrc,wwD->sD);
  makeframeS(nframeS);
  swapbuffers();
  if (ShowGridA)  drawGridTet1(wwD, 1);
  if (ShowGridB)  drawGridTet1(wwD, 2);
  winset(wid);
return ++nframeS;
}





static MobData *MobD;	/* __________ STATIC GLOBAL for wwD ____________ */


static int
newFrameFromClrs(flt clrs[], int numSrc)
{
double	cpuT;
time_t	oldT = clock();

  warpNewFrame(MobD,clrs,numSrc);
  cpuT = (double)(clock() - oldT)/CLOCKS_PER_SEC;
  sprintf(Ss,"%s frame %d, time %.3f", winTitle, nframes+1, cpuT);
  wintitle(Ss);
  return showNewFrameMain(MobD);
}




static void
winSlidersCB(double red, double grn, double blu)
{
  GloX = red, GloY = grn, GloZ = blu;
  clrsFromXYZ(Clrs, red, grn, blu, NumSrc);
  newFrameFromClrs(Clrs,NumSrc);
}


void
readICCVparams (void)
{
static	char *ipath = "/home/vu/mjones/images/face5/ICCV-params";
char	sts[256], str1[256], str2[256];
FILE	*fi;  
int 	ch, nr, nbytes;
double	rX, rY, rZ;

#if 11
  fi = popen("file /home/vu/mjones/images/face5/ICCV-params","r");
  fscanf(fi, "%s %s", str1, str2);
  pclose(fi);
  if (strcmp(str2, "No"))  {
    fi = popen("wc -c /home/vu/mjones/images/face5/ICCV-params","r");
    ch = getc(fi);
    if (ch != 'w')  {
      fscanf(fi, "%d", &nbytes);
    }
    pclose(fi);
    if (nbytes > 26)  {
      if ((fi = fopen(ipath,"r")) != NULL) {
        if (fgets(sts,256,fi) != NULL) {
          if (3 == (nr = sscanf(sts," %lf %lf %lf ", &rX, &rY, &rZ))) {
            prn("readICCVparams %d XYZ: %.3f %.3f %.3f", nframes,rX,  rY,  rZ);
            winSlidersCB(rX,rY,rZ);
          }
          else warn("readICCVparams: err scan params from %s in %s",sts,ipath);
          }
        else
          warn("readICCVparams: opened file %s but error from fgets",ipath);
        fclose(fi);  fi = NULL;
        sprintf(Ss,"rm -f %s", ipath);
        system(Ss);
        qenter(WL_NOTHING,(short)InMainWid);
        return;
      }
    }
  }
  sginap(67);	/* hundredths of a second */
  return;
#else
    if ((fi = fopen(ipath,"r")) != NULL) {
      if (fgets(sts,256,fi) != NULL) {
        if (3 == (nr = sscanf(sts," %lf %lf %lf ", &rX, &rY, &rZ))) {
          prn("readICCVparams %d XYZ: %.3f %.3f %.3f", nframes,rX,  rY,  rZ);
          winSlidersCB(rX,rY,rZ);
        }
        else warn("readICCVparams: err scan params from %s in %s",sts,ipath);
        }
      else
        warn("readICCVparams: opened file %s but error from fgets",ipath);
      fclose(fi);  fi = NULL;
      sprintf(Ss,"rm -f %s", ipath);
      system(Ss);
      qenter(WL_NOTHING,InMainWid);
      return;
      sginap(25);	/* quarter of a second */
      return;
#endif
}


void
mouseWarpXYZ (void *v, int device, int mouseDown)
{
static   long   xo,yo;
register long   xn,yn;
dbl        xf = GloX, yf = GloY, zf = GloZ;
/* pWmj    cW = (pWmj)v; */
/* pWmjAct Ap = (device == LEFTMOUSE ? cW->lefMouse : cW->midMouse); */
/* pMD    wwD = (pMD) Ap->data; */
long    wd, ht;
dbl 	dist = 0.0, mindist = 0.00001;

  if (! mouseDown)  return;

  getorigin(&orgX,&orgY);
  getsize(&wd,&ht);     wd -= 1, ht -= 1;

  /*
  cursoff();
  drawmode(OVERDRAW);
  */

  while(getbutton(device)) {
    xn = getvaluator(MOUSEX) - orgX; if (xn < 0) xn=0; else if (xn > wd) xn=wd;
    yn = getvaluator(MOUSEY) - orgY; if (yn < 0) yn=0; else if (yn > ht) yn=ht;
    if (xn != xo || yn != yo) {
      xf = (dbl) xn / wd;
      if (NumSrc > 4 && device != LEFTMOUSE)
        zf = (dbl) yn / ht;
      else
        yf = (dbl) yn / ht;
      dist  = (xf-GloX)*(xf-GloX) + (yf-GloY)*(yf-GloY) + (zf-GloZ)*(zf-GloZ);
      /* if (dist > mindist) change_cursor(); */
      xo = xn, yo = yn;
    }
  }

  if (dist > mindist) {

    if (InSlidersWid)
      winSlidersUpdate( xf, yf, zf, 1, 1);    /* replace with general CP updater */

    clrsFromXYZ(Clrs, xf, yf, zf, NumSrc);
    newFrameFromClrs(Clrs,NumSrc);

    if (Options & O_VERB)
      fprintf(stderr,"%5d: old-xyz: %.3f %.3f %.3f --> new-xyz: %.3f %.3f %.3f\n"
             ,nframes,GloX,GloY,GloZ,xf,yf,zf);

    GloX = xf, GloY = yf, GloZ = zf;
  }
}


void
markSub2 (void *v, int device, int flags)
{
  if (! flags) return;  /* act only on mouseDown */
  switch (device) {
    case  MIDDLEMOUSE:  wrn("markSub2: NOIMP for TETRAGONS");            break;
    case  LEFTMOUSE:    markCrsTri2( v, device, flags );                 break;
    default:            wrn("markSub2: NO ACTION on device %d",device);  break;
  }
}

void
moveSub2 (void *v, int device, int flags)
{
  if (! flags) return;  /* act only on mouseDown */
  switch (device) {
    case  MIDDLEMOUSE:  wrn("moveSub2: NOIMP for TETRAGONS");             break;
    case  LEFTMOUSE:    moveCrsTri2( v, device, flags );                  break;
    default:            wrn("moveSub2: NO ACTION for device %d",device);  break;
  }
}



void
markCrs2 (void *v, int device, int flags)
{
  if (! flags) return;  /* act only on mouseDown */
  switch (device) {
    case  MIDDLEMOUSE:  markCrsTet2( v, device, flags);  		  break;
    case  LEFTMOUSE:    markCrsTri2( v, device, flags);  		  break;
    default:            wrn("markCrs2: NO ACTION on device %d",device);	  break;
  }
}

void
moveCrs2 (void *v, int device, int flags)
{
  if (! flags)  return;  /* act only on mouseDown */
  switch (device) {
    case  MIDDLEMOUSE:  moveCrsTet2( v, device, flags);                   break;
    case  LEFTMOUSE:    moveCrsTri2( v, device, flags);                   break;
    default:            wrn("moveCrs2: NO ACTION for device %d",device);  break;
  }
}


void
reFlowGridWarp (Wimj *wp, int device, int flags)
{
WmjAct *ap   =  wp->act;
pMD     wwD  = (pMD)ap->data;

  warpGridTet(wp,0,0, 0.0);   /* A<-B */
  inFlowDsrc(wwD,1,1);        /* A<-N */
  warpGridTet(wp,0,9, 0.0);   /* N->B */
  warpGridTet(wp,0,1, 0.0);   /* B<-A */
  inFlowDsrc(wwD,1,2);        /* B<-N */
  warpGridTet(wp,0,8, 0.0);   /* N->A */
  if (NumSrc == 2)  { Clrs[0] = 0.5; Clrs[1] = 0.5; }
  newFrameFromClrs(Clrs,NumSrc);
}

int saveFlowFiles ( pMD wwD, int nA, int nB )
{
pMOB   *mob  = wwD->mob;
ppf    *fvX  = wwD->fvX,  *fvY = wwD->fvY;
ppf    *bvX  = wwD->bvX,  *bvY = wwD->bvY;
pDmj    fD   = wwD->fD;   int fX = fD->x, fY = fD->y, fW = fD->w, fH = fD->h;

  if (Options & O_FWRD || Devopts & O_BOTH) {
    sprintf(Ss,"%s-%s",mob[nA]->name,mob[nB]->name);
    writeFlows(fvX[nB],fvY[nB], fX,fY,fW,fH, Ss, 'F', 1);
  }
  if (Options & O_BACK || Devopts & O_BOTH) {
    sprintf(Ss,"%s-%s",mob[nA]->name,mob[nB]->name);
    writeFlows(bvX[nB],bvY[nB], fX,fY,fW,fH, Ss, 'B', 1);
  }
return 0;
}


#define retitle(S)      {sprintf(Ss,"%s: %s",wA->T,(S)); wintitle(Ss);}
#define ModeTil(M,S)    WLmode = M; retitle(S)

static char *msOpts = "OPTS %t|Warn/Show %x101|A-Pixels %x102|B-Pixels %x103|\
	BlendPixels %x104|UseFlow %x105|MoveGridA %x106|MoveGridB %x107|\
	ShowGridA %x108|ShowGridB %x109";

void
winEditMenu2 (void *v, int device, int flags)
{
static  WmjAct  zoomA;  static zoomOn;
pWmj    tW   = (pWmj)v, wA, wB;  /* should be circular A->B->A */
WmjMnu *mn   = tW->mnu;
int 	nA   = mn->nA, nB = mn->nB;
pMD     wwD  = (pMD)mn->data;
pMOB   *mob  = wwD->mob;
/* pMOB	mA   = mob[nA]; */
pMOB 	mB   = mob[nB];
ppf    *fvX  = wwD->fvX,  *fvY = wwD->fvY  /* , *srcG = wwD->srcG */ ;
pDmj	fD   = wwD->fD, sD   = wwD->sD;
int     fX = fD->x, fY = fD->y, fW = fD->w, fH = fD->h;
int     sX = sD->x, sY = sD->y, sW = sD->w, sH = sD->h;
long    pval;
static long	mpOpts, mpGrid, mpTris, mpTets, mpFile;

static char *msGrid = "GRID %t|MakeTet %x201|ReadGridTet %x202|ReadRevTet %x203|\
	WriteGridTet %l %x204|ReFlowGrid %x205|MoveGridA %x206|MoveGridB %x207|\
	ResetGrid %x220|ShowGridA %x208|ShowGridB %x209|\
	GridWarp A<-B %x210|GridWarp A->B %x211|\
	WarpFwd A->B %x212|WarpBac A<-B %x213|UseFlow %l %x214|\
	SurfWLSO Bac %x215|GridMorph Fwd %x221";

static char *msTris = "TRIANGLES %t|New Chain %x31|TxtMap %x32|\
	TxtMapOLD %x33|ReFlow %x34";
static char *msTets = "TETRAGONS %t|New Chain %x41|TxtMap %x42|ReFlow %x43|\
	BL-ReFlow %x44|BoxBIL %x45|BoxNEN %x46|SwapPersp %x47|SwapBilin %x48";
static char *msFile = "FILE %t|SaveSRC %x61|SaveFB+GL %x62|\
	Clone %l %x63|SaveFLOW %x64|Read %x65";
static char *msEdit = "EDIT CRS %t|Opts %m|Grid %m|Tri %m|Tet %m|File %m %l|\
	Return %x1|Zoomer %x2|Undo %x3|Redo %x4|Restart %x5|FlowFWD %x6|\
	FlowBAC %x7|FlowB-B %x8|Copy0 FWD %x9|Copy0 BAC %x10";

  if (tW->in == nA)  wA = tW;  else wA = tW->next; wB = wA->next;

  if (! mn->id)  {
    mpOpts     = defpup(msOpts);
    mpGrid     = defpup(msGrid);
    mpTris     = defpup(msTris);
    mpTets     = defpup(msTets);
    mpFile     = defpup(msFile);
    mn->id     = defpup(msEdit,mpOpts,mpGrid,mpTris,mpTets,mpFile);
    zoomA.data = wwD; 
    zoomA.func = zoomRectTri2; 
    zoomA.nA   = 0; 
  }

  winset(wA->id);
  switch (pval = dopup(mn->id)) {
    
    case 101: if (Devopts & O_WARN) { Devopts &= ~O_WARN; retitle("warn off"); }
              else                  { Devopts |=  O_WARN; retitle("warn on");  }       break;
    case 102: Devopts &= ~O_ONLY; Options |=  O_ONLY; retitle("A-pixels ONLY");        break;
    case 103: Options &= ~O_ONLY; Devopts |=  O_ONLY; retitle("B-pixels ONLY");        break;
    case 104: Options &= ~O_ONLY; Devopts &= ~O_ONLY; retitle("Blend A+B Pixels");     break;
    case 105: UseFlowAB = !UseFlowAB; retitle(UseFlowAB ? "UseFlow ON":"UseFlow OFF"); break;
    case 106: MoveGridA = !MoveGridA; retitle(MoveGridA ? "MoveGridA ON":"MoveGridA OFF"); break;
    case 107: MoveGridB = !MoveGridB; retitle(MoveGridB ? "MoveGridB ON":"MoveGridB OFF"); break;
    case 108: ShowGridA = !ShowGridA; retitle(ShowGridA ? "ShowGridA ON":"ShowGridA OFF"); break;
    case 109: ShowGridB = !ShowGridB; retitle(ShowGridB ? "ShowGridB ON":"ShowGridB OFF"); break;

    case 201: retitle("Grid Tet MADE");   makeGridTet(v,0,0,0);drawGridTet2(v,0,0); break;
    case 202: retitle("Grid Tet READ");   readGridTet(v,GrdY,GrdX,0);   break;
    case 203: retitle("Grid Rev READ");   readGridTet(v,GrdY,GrdX,1);   break;
    case 204: retitle("Grid Tet WRITE");  writGridTet(v,GrdY,GrdX,0);   break;
    case 205: if (! fvX[nB] || ! wwD->bvX[nB]) { 
              retitle("reFlowGrid NOT DONE"); 
              warn("reFlowGrid requires both forward and backward flows."); break; }
              retitle("reFlowGrid A<->B"); reFlowGridWarp(v,0,0);  if (Devopts & O_EXTR) {
              retitle("ReGrid+Save"); writGridTet(v,GrdY,GrdX,0); saveFlowFiles(wwD,nA,nB);} break;
    case 206: MoveGridA = !MoveGridA; retitle(MoveGridA ? "MoveGridA ON":"MoveGridA OFF"); break;
    case 207: MoveGridB = !MoveGridB; retitle(MoveGridB ? "MoveGridB ON":"MoveGridB OFF"); break;

    case 220: ResetGrid = !ResetGrid; retitle(ResetGrid ? "ResetGrid ON":"ResetGrid OFF"); break;

    case 208: ShowGridA = !ShowGridA; retitle(ShowGridA ? "ShowGridA ON":"ShowGridA OFF"); break;
    case 209: ShowGridB = !ShowGridB; retitle(ShowGridB ? "ShowGridB ON":"ShowGridB OFF"); break;
    case 210: retitle("GridWarped A<-B"); warpGridTet(v,0,0, 0.0);           break;
    case 211: retitle("GridWarped A->B"); warpGridTet(v,0,1, 0.0);           break;
    case 212: retitle("GridWarp F A->B"); warpGridTet(v,0,8, 0.0);           break;
    case 213: retitle("GridWarp I A<-B"); warpGridTet(v,0,9, 0.0);           break;
    case 214: UseFlowAB = !UseFlowAB; retitle(UseFlowAB ? "UseFlow ON":"UseFlow OFF"); break;
    case 215: retitle("GridWarp I A<-B"); surfGridWLS(v,0,1);           break;

    case 221: 
              if (NumSrc == 2) {
                int j;
                WLmode = MOUSE_X;
                for (j = 0; j < 10; j++)
                  warpGridTet(v, 0, 2, (dbl)j/(9.0));
              }
              break;


    case   1: retitle("Returning...");  InEditIndex = 0;
	      winclose(wA->id); 
	      winclose(wB->id); 
              return;
    case   2: if ((zoomOn = !zoomOn)) {    zoomA.nB = nB;
                retitle("A->LEFT==ZOOM");  wA->lefMouse = &zoomA; }
              else { retitle("Zoom Off");       wA->lefMouse = NULL;   mB->nvrt = 0; }
              break;
    case   3: if (copyFlowFrUndo(wwD,mB->foX,mB->foY,nB))  retitle("Undid Changes")
              else  warn("EditMenu: Can't Undo %d",InUndoIndex);
              break;
    case   4: if (copyFlowFrRedo(wwD,mB->foX,mB->foY,nB))  retitle("Redid Changes")
              else  warn("EditMenu: Can't Redo %d",InRedoIndex);
              break;
    case   5: retitle("Start Over");  
              mB->ntet = mB->ntri = mB->nvrt = 0;
              /* qenter(REDRAW,wA->id); qenter(REDRAW,wB->id); */
              Wdraw(wA,REDRAW,wA->id);
              Wdraw(wB,REDRAW,wB->id);
              break;
    case   6: retitle("Flow A->NEW FWD");  inFlowDsrc(wwD,nB,0);  	break;
    case   7: retitle("Flow A->NEW BAC");  inFlowDsrc(wwD,nB,1);  	break;
    case   8: retitle("Flow B->NEW BAC");  inFlowDsrc(wwD,nB,2);  	break;
    case   9: retitle("Copied Flow 0 FWD"); copyFlow0(wwD,nB,0);        break;
    case  10: retitle("Copied Flow 0 BAC"); copyFlow0(wwD,nB,1);        break;

    case  31: retitle("New Chain");	    mB->nvrt = 0;		break;
    case  32: retitle("Txt-Map Tris");	    inMapTri2(wwD,nA,nB,0);	break;
    case  33: retitle("Txt-Map Tris OLD");  inMapTri2Old(wwD,nA,nB,0);	break;
    case  34: retitle("OptFlowCutOut");	    inFloTriCut(wwD,nA,nB);	break;

    case  41: retitle("New Chain");	mB->nvrt = 0;			break;
    case  42: retitle("T-Map Tets");	inCrsTetReFlowBil(wwD,nB,0);	break;
    case  43: retitle("Redo OptFlow");	inCrsTetReFlowCut(wwD,nB,0);	break;
    case  44: retitle("WarpBilReFlow");	inCrsTetReFlowBil(wwD,nB,1);	break;
    case  45: retitle("BOX BIL");	inCrsTetBox(wA,0);	 	break;
    case  46: retitle("BOX NEN");	inCrsTetBox(wA,1);		break;
    case  47: retitle("Swap-Persp");	inCrsTetSwapPersp(wA,0);	break;
    case  48: retitle("Swap-Bilin");	inCrsTetSwapBilin(wA,0);	break;

    case  61: ModeTil(STOPPED,"Src RGB Saved");    saveRgbGL(tW,0);	break;
    case  62: ModeTil(STOPPED,"FB RGB+GL Saved");  saveRgbGL(tW,1);	break;
    case  63: ModeTil(STOPPED,"CLONED");           Wclone(tW,0);   	break;
    case  64: retitle("Save Flow Files");  saveFlowFiles(wwD,nA,nB);    break;
    case  65: retitle("Read Flow Files"); 
              sprintf(Ss,"%s-%s",mob[nA]->name,mob[nB]->name);
              readFlows(&fvX[nB],&fvY[nB],fX,fY,fW,fH,0,Ss,'F',1);
              if (Devopts & O_PTRS)
                rgbWarpBilFlt(mB->prew,mB->rgbi,fvX[nB],fvY[nB],sX,sY,sW,sH,sX,sY,sW,sH,1.0);
              break;

    case -1: /* no selection */                         break;
    default: warn("winEditMenu2: Unknown option: pupval:%d",pval);
  }
  winset(tW->id);
}




static pWmj
winEditCrs2 (int nA, int nB)	/* nA has to be 0, the central image */
{
static  WmjMnu  editM;   static first = 1;
static  WmjAct  markA, moveB;
pWmj 	wA,wB;
pMOB	mA,mB;
long	wid;

  if (first) {  first = 0;
    editM.data  =  MobD;
    editM.func  = &winEditMenu2;
    markA.data  =  MobD;
    markA.func  = &markCrs2;
    moveB.data  =  MobD;
    moveB.func  = &moveCrs2;
  }

  if (!(mA = MobD->mob[nA]) || !(mB = MobD->mob[nB])) {
    warn("winWarpEdit2: missing data on src %d or %d",nA,nB);
    return NULL;
  }
  wid = winget();

  sprintf(Ss,"[A] %s",mA->name);
  wA = showFm(mA->rgbi,mA->sD,Ss,Zoom,0);
  wA->in          = nA;
  wA->act         = &markA;
  wA->mnu         = &editM;

  sprintf(Ss,"[B] %s",mB->name);
  wB = showFm(mB->rgbi,mB->sD,Ss,Zoom,0);
  wB->in          = nB;
  wB->act         = &moveB;
  wB->mnu         = &editM;

  markA.nA        = moveB.nA   = editM.nA = nA;
  markA.nB        = moveB.nB   = editM.nB = nB;
  wA->next = wA->prev = wB;
  wB->next = wB->prev = wA;		/* close circle */
 
  InEditIndex = nB;	/* @@ PROBLEM: winclose won't reset */
  InRedoIndex = 0;
  if (InUndoIndex != nB)  InUndoIndex = 0;

  winset(wid);
  return wA;
}

#define EDIT_SM_OFF 100


static void
editSubMenuFunc (int num)
{
  WLmode = STOPPED;
  num -= EDIT_SM_OFF;
  if (num < 1 || num >= NumSrc)
    die("Edit SubMenu: No src %d",num);
  if (InEditIndex)
    wrn("Edit SubMenu: Already editing %d", InEditIndex);
  else
      winEditCrs2(0,num);
}

static void
winWarpMouseX (pWmj v, int device, int flags)
{
pWmjAct act = v->act;
pMD     wwD = (pMD) act->data;

int 	n;
  if ( ! flags)  return;

  /* getsize(&wdMain,&htMain); */
  getorigin(&orgX,&orgY);
  while (getbutton(device)) {
    if      (v == wwD->dWin)  {
      n = (nframes * (getvaluator(MOUSEX) - orgX))/(float)wdMain;
      gotoframe(n);
    }
    else if (v == wwD->sWin)  {
      n = (nframeS * (getvaluator(MOUSEX) - orgX))/(float)wdShow;
      gotoframeS(n);
    }
  }
}

void gridSrc(pMD wwD)
{
int  	j, clr;
flt     rgbp[3];
ppf 	*sRGB = wwD->sRGB;
pDmj 	sD = wwD->sD; int sX = sD->x, sY = sD->y, sW = sD->w, sH = sD->h;

  wrn("gridSrc on NumSrc: %d", NumSrc);

  for (j = 0; j < NumSrc; j++) {
    clr = j % 4; switch (clr) {
      case 0: rgbp[0] = 255.0; rgbp[1] =   0.0; rgbp[2] =   0.0; break;
      case 1: rgbp[0] =   0.0; rgbp[1] = 255.0; rgbp[2] =   0.0; break;
      case 2: rgbp[0] = 255.0; rgbp[1] =   0.0; rgbp[2] = 255.0; break;
      case 3: rgbp[0] = 255.0; rgbp[1] = 255.0; rgbp[2] =   0.0; break;
    }
    rgbMarkGridRect(sRGB[j],sX,sY,sW,sH, 16, 16, rgbp);
  }
}


static void
winWarpMenu (void *v, int device, int flags)
{
static  long    mpMovie, mpSave, mpEdit, mpMark, mpWarp, mpOpts;
static  pchr    msMovie, msSave, msEdit, msMark, msWarp, msMain;
char 	str[256];
pWmj    wA   = (pWmj)v;
pWmjMnu mn   = wA->mnu;
pMD	wwD  = (pMD)mn->data;
long    pval;

  if (! mn->id) {

    msMark  = "Mark %t|example %x31|Grid %x32|What? %x33";
    mpMark  =  defpup(msMark);
    sprintf(Ss,"EDIT CRS %%t %%F");
    for (pval = 1; pval < NumSrc; pval++) {
      sprintf(str,"|0 - %ld %%x%ld",pval, (pval + EDIT_SM_OFF));
      strcat(Ss,str);
    }
    msEdit  =  Ss;
    mpEdit  =  defpup(msEdit,editSubMenuFunc);
    msMovie = "MOVIE %t|Swing %x51|Loop %x52|MouseX %x53|Stop %x54";
    mpMovie =  defpup(msMovie);
    msSave  = "SAVE %t|Clone %x61|FrameSrc %x62|Frame+GL %x63|\
	Each New %x64|No More %x65|Movie %x66";
    mpSave  =  defpup(msSave);
    mpOpts  =  defpup(msOpts);
    msWarp  = "WARP %t|Fwd-Org %x41|Fwd-Both %x42|Fwd-End %x43|\
	Bak-Org %x44|Back-Both %x45|Back-End %x46 %l|\
	A-Pixels %x47|B-Pixels %x48";
    mpWarp  =  defpup(msWarp);
    msMain  = "MODES %t|Opts %m|Warp %m|EditCrs %m|MOVIE %m %l|Save %m %l|\
	Quit %x1|Return %x2|Sliders %x3|Zoomer %x4";
    mn->id    = defpup(msMain,mpOpts,mpWarp,mpEdit,mpMovie,mpSave);
  }

if   (WLmode == SLIDERS) {ModeTil(MOUSEPOS,"MouseXY");}
  switch (pval = dopup(mn->id)) {

    case 101: if (Devopts & O_WARN) { Devopts &= ~O_WARN; retitle("warn off"); }
              else                  { Devopts |=  O_WARN; retitle("warn on");  }       break;
    case 102: Devopts &= ~O_ONLY; Options |=  O_ONLY; retitle("A-pixels ONLY");        break;
    case 103: Options &= ~O_ONLY; Devopts |=  O_ONLY; retitle("B-pixels ONLY");        break;
    case 104: Options &= ~O_ONLY; Devopts &= ~O_ONLY; retitle("Blend A+B Pixels");     break;
    case 105: UseFlowAB = !UseFlowAB; retitle(UseFlowAB ? "UseFlow ON":"UseFlow OFF"); break;
    case 106: MoveGridA = !MoveGridA; retitle(MoveGridA ? "MoveGridA ON":"MoveGridA OFF"); break;
    case 107: MoveGridB = !MoveGridB; retitle(MoveGridB ? "MoveGridB ON":"MoveGridB OFF"); break;
    case 108: ShowGridA = !ShowGridA; retitle(ShowGridA ? "ShowGridA ON":"ShowGridA OFF"); break;
    case 109: ShowGridB = !ShowGridB; retitle(ShowGridB ? "ShowGridB ON":"ShowGridB OFF"); break;

    case  1: gexit(); exit(0); break;
    case  2: qenter(WL_RETURN,0); break;
    case  3: if (InSlidersWid) {
               winclose(InSlidersWid);
               InSlidersWid = 0;
               winset(wwD->dWin->id);
               ModeTil(MOUSEPOS,"MouseXY");
             } else {
               ModeTil(SLIDERS,"Sliders");
               winSlidersRgb(&GloX,&GloY,&GloZ,"XYZ==RGB",winSlidersCB
                          ,reframe,InMainWid,0);
             }
             break;
    case  4: if   (WLmode == ZOOMER) {ModeTil(WLmodeDef,"");}
             else {ModeTil(ZOOMER,"Zoomer");}           	break;

    case 31: ModeTil(STOPPED,"Show GL"); showSomeGL(wA);    	break;
    case 32: ModeTil(STOPPED,"Grid"); gridSrc(wwD);     	break;
    case 33: ModeTil(STOPPED,"What"); wrn("ShowGL what?");  	break;

    case 41: Options  = (Options & ~O_BACK) | O_FWRD;
             Devopts &= ~O_BOTH;
             ModeTil(STOPPED,"WARP Fwd-Orig");                   break;
    case 42: Options  = (Options & ~O_BACK) | O_FWRD;
             Devopts |= O_BOTH;
             ModeTil(STOPPED,"WARP Fwd-Both");                   break;
    case 43: Options  = (Options & ~O_BACK) | O_FWRD | O_EXTR;
             Devopts &= ~O_BOTH;
             ModeTil(STOPPED,"WARP Fwd-End");                   break;
    case 44: Options  = (Options & ~O_FWRD) | O_BACK;
             Devopts &= ~O_BOTH;
             ModeTil(STOPPED,"WARP Bak-Orig");                   break;
    case 45: Options  = (Options & ~O_FWRD) | O_BACK;
             Devopts |= O_BOTH;
             ModeTil(STOPPED,"WARP Fwd-Both");                   break;
    case 46: Options  = (Options & ~O_FWRD) | O_BACK | O_EXTR;
             Devopts &= ~O_BOTH;
             ModeTil(STOPPED,"WARP Fwd-End");                   break;
    case 47: Devopts &= ~O_ONLY; Options |=  O_ONLY; retitle("A-pixels ONLY"); break;
    case 48: Options &= ~O_ONLY; Devopts |=  O_ONLY; retitle("B-pixels ONLY"); break;

    case 51: ModeTil(SWINGING,"SWING");                 	break;
    case 52: ModeTil(LOOPING,"LOOP");                   	break;
    case 53: ModeTil(MOUSE_X,"mouse-X");                	break;
    case 54: ModeTil(STOPPED,"STOP");                   	break;

    case 61: ModeTil(STOPPED,"CLONED");          Wclone(wA,0);	break;
    case 62: ModeTil(STOPPED,"RGB Saved");    saveRgbGL(wA,0);	break;
    case 63: ModeTil(STOPPED,"RGB+GL Saved"); saveRgbGL(wA,1);	break;
    case 64: ModeTil(STOPPED,"SaveEachNew: NOIMP");     	break;
    case 65: ModeTil(STOPPED,"Save NoMore: NOIMP");     	break;
    case 66: ModeTil(STOPPED,"Save Movie: NOIMP");      	break;

    case  -1: /* no selection */                         	break;
    case   0: /* no selection @@ */                         	break;
    default: warn("winWarpMenu: Unknown option: pupval:%d",pval); break;
    /** Menu can pop up and go away again with no selected item. **/
  }
}


long dbWin (long xsize, long ysize, char *wtil, unt flags)
{
long 	wid, xframe, yframe, dispmode;

  xframe = zoom*xsize;
  yframe = zoom*ysize;
  prefsize(xframe,yframe);
  sprintf(winTitle,"%s(%d)",ProgName,NumSrc);
  sprintf(Ss,"%s %s",winTitle,wtil);
  if (flags)
    prefposition(1280 - xframe, 1279, 1024 - yframe, 1023 );
  else
    prefposition( 640 - xframe/2, 639 + xframe/2, 512 - yframe/2, 511 + yframe/2);
  wid = winopen(Ss);
  rectzoom(zoom,zoom);

/*******
#ifdef INDY
  imgmode = SCREENIMG;
  singlebuffer();
#else
  doublebuffer();
#endif
********/

  gconfig();
  dispmode = prefdrawmode(imgmode);
  curdispmode = getdisplaymode();
  switch(dispmode) {
    case DI_WRITERGB:
    case DI_LRECTWRITE:
      if(curdispmode != DMRGB && curdispmode != DMRGBDOUBLE)
        pseudorgb();
      break;
    case DI_WRITEPIXELS:
    case DI_RECTWRITE:
      if(curdispmode != DMSINGLE && curdispmode != DMDOUBLE) {
        singlebuffer();
        gconfig();
      }
    break;
  }
  grey(0.5);
  clear();
  swapbuffers();
  return wid;
}


static  WmjAct  warpAct;

long
makeShowWin (pMD wwD, char *til)
{
pDmj	sD = wwD->sD;

  InShowWid = dbWin(sD->w,sD->h,"ShowWin",1);
  wdShow    = zoom * sD->w;
  htShow    = zoom * sD->h;
  wwD->sWin = Wreg(InShowWid,sD,til,zoom,zoom,0);
  wwD->sWin->act = &warpAct;
/*
  wwD->sWin->mnu = &warpMnu;
  wwD->sWin->F = wwD->dst;
*/
  return InShowWid;
}


int
parmReadXYZ (flt parm[][MAXPARM], char *path, int numSrc)
{
#define SLEN 256
char    q, str[SLEN];
dbl     rX,rY,rZ;
int     j, line, tween;
FILE    *fi;

  if (! (fi = fopen(path,"r"))) {
    warn("parmReadXYZ [%s]: can't open %s, returning 0",__FILE__,path);
    return 0;
  }

  for (line = tween = 0; fgets(str,SLEN,fi) != NULL; line++) {
    if ((j=strlen(str)) < 12 || (q=str[0]) == '#' || q == '/' || q == ';' || q == '*') {
      /* fprintf(stderr,"[%3d]: %s",j,str); */
    }
    else {
      if (3 != (j=sscanf(str, " %lf %lf %lf "         , &rX, &rY, &rZ))
      &&  3 != (j=sscanf(str, " %*[^:]: %lf %lf %lf "  , &rX, &rY, &rZ)))
        die("parmReadXYZ: parse error at tween %d, line %d, sscanf %d:\n%s"
                ,tween,line,j,str);
      else {
/*	
        fprintf(stderr,"[%3d]: % 7.2f  % 7.2f  % 7.2f \n"
                ,tween,rX,rY,rZ);

	ALL CALLS IN winWarp.c should be with XYZ, not clrs! 
        clrsFromXYZ(parm[tween],rX,rY,rZ,numSrc);
*/
        winSlidersCB(rX,rY,rZ);

        tween++;
      }
    }
  }
  fclose(fi);
  return tween;
}




int
winWarpFwdN ( pMOB mob[], pMD wwD, int numSrc)
{
static int	first = 1;
static  WmjMnu  warpMnu;
pWmj	pw;
pDmj 	dD = mob[0]->dD;
long	j;

#if 00
  MvZoom = Zoom = 1.0;
#endif

  zoom = (flt)MvZoom;

  if (first) {	first = 0;
    if (Options & O_BACK || Devopts & O_BOTH)  MoveGridA = 1;
    else                                       MoveGridB = 1;
    MobD = wwD;
    warpAct.data  = (void *) wwD;
    warpAct.func  = &mouseWarpXYZ;
    warpMnu.data  =  wwD;              /* ap->data; */
    warpMnu.func  = &winWarpMenu;
    InMainWid = dbWin(dD->w,dD->h,"Warp Window XGUI",0);
    wdMain    = (int)(zoom * dD->w);
    htMain    = (int)(zoom * dD->h);
    wwD->dWin = Wreg(InMainWid,dD,winTitle,zoom,zoom,0);
    wwD->dWin->act = &warpAct;
    wwD->dWin->mnu = &warpMnu;
    wwD->dWin->F = wwD->dst;
    GloX = GloY = GloZ = 0.0;
    Clrs[0] = 1.0;
  }

  switch (NumSrc) {
    case  2:
    case  3:
             Ntweens = parmFromNtweens2(wwD->parm,Ntweens,NumSrc); break;
    case  4: Ntweens = parmFromNtweens4(wwD->parm,Ntweens,Ytweens,4); break;
    case  6: wrn("N==6; auto-cover doing only half of parm-space..."); 
             Ntweens = parmFromNtweens4(wwD->parm,Ntweens,Ytweens,4); break;
    case  8: Ntweens = parmFromNtweens8(wwD->parm,Ntweens,Ytweens,Ztweens,8);
             break;
    case 12: 
    default: Ntweens = 1; Clrs[0] = 1.0;
             wrn("%s: no auto-cover for NumSrc = %d", ProgName, NumSrc);
  }

  if (Ztweens > 9)  GrdX = MIN(Ztweens,GTX);  else GrdX = GTX;
  if (Ytweens > 9)  GrdY = MIN(Ytweens,GTY);  else GrdY = GTY;
  /* prn("GrdX & GrdY are:  %d  %d", GrdX,GrdY); */


  if (Devopts & O_EXTR)  {
    pw = winEditCrs2(0,1);

    readGridTet(pw,GrdY,GrdX,0);/* A->B */
    readGridTet(pw,GrdY,GrdX,1);/* A<-B */
 
    if (Devopts & O_KLUG)  {
      warpGridTet(pw,0,0, 0.0);	/* A<-B */
      inFlowDsrc(wwD,1,1     );	/* A<-N */
      warpGridTet(pw,0,9, 0.0);	/* N->B */

      warpGridTet(pw,0,1, 0.0);	/* B<-A */
      inFlowDsrc(wwD,1,2     );	/* B<-N */
      warpGridTet(pw,0,8, 0.0);	/* N->A */
      /* surfGridWLS(pw,0,1); */
      if (NumSrc == 2)  { Clrs[0] = 0.5; Clrs[1] = 0.5; }
    }
  }

  if (Devopts & O_WRIT)  sys("rm -fr %s/%s.*", ArgStr[NumSrc+2], "v");

  if (Devopts & O_READ) {
    WLmode = MOUSE_X;
    Ntweens = parmReadXYZ(wwD->parm,"innXYZ",NumSrc);
  }
  else if (Devopts & O_XGUI ) {
    WLmode = MOUSE_X;
    for (j = 0; j < Ntweens; j++)
      newFrameFromClrs(wwD->parm[j],numSrc);
  }
  else {
    Ntweens = 1;
    newFrameFromClrs(Clrs,numSrc);
  }

  WLswing  = &winWarpSwing;
  WLstep   = &winWarpStep;
  WLmouseX = &winWarpMouseX;
  /* winLoop(); -- done in mainN.c */
  return wwD->ndst = nframes;
}


