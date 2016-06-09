/*****************************************************************************
 inRead.c    S.R.Lines    95.05.27
#define UNTIME 1
 *****************************************************************************/
#include        "gg.h"
#include        "manCor.h"
#include        "inExtern.h"
#include        "inParm.h"
#include        "inLoad.h"
#include        "jk.h"
#include        "mob.h"
#include        <sarnoff/imghdr.h>
#include        <m/rowWarp.h>
#include        <m/rowFile.h>
#include        <m/rowCopy.h>
#include        <m/rowCnvt.h>
#include        <m/rowSGI.h> 
#include        "rowFimg.h"
#include        "winLoop.h"
#include        <UN/unFlags.h>
extern	int	readFaceLbl(LblFace *lbl, char *name);

double CropYF = 0.36;

void writeFlows ( ppf vX, ppf vY, int fX, int fY, unt fW, unt fH
             , const char *path, const char ext, unt flags)
{
  if (Options & O_TIME) {
    unt rw = fW - 2*Image_border, rh = fH - 2*Image_border;
    /* prn("writing %s.Fimg[XY]", path); */
    rowWriteFimg(vX,  0, 0,rw,rh,       path, (ext=='F' ? "sFX":"sBX"), flags|6);
    rowWriteFimg(vY,  0, 0,rw,rh,       path, (ext=='F' ? "sFY":"sBY"), flags|2);
  } else {
    /* prn("writing %s.F[XY]", path); */
    rowWriteFlt( vX, fX,fY,fW,fH,0,NULL,path, (ext=='F' ? "rFX":"rBX"), flags);
    rowWriteFlt( vY, fX,fY,fW,fH,0,NULL,path, (ext=='F' ? "rFY":"rBY"), flags);
  }
}


void readFlows ( ppf *vX, ppf *vY, int fX, int fY, unt fW, unt fH
           , unt hdrBytes, const char *path, const char ext, unt flags)
{
  if (Options & O_TIME) {
    unt sw = fW - 2*Image_border, sh = fH - 2*Image_border;
    /* prn("reading FIMAGE %s.sF[XY]", path); */
    rowReadFimg(vX,fX,fY,fW,fH,sw,sh,0,     path, (ext=='F' ? "sFX":"sBX"), flags|6);
    rowReadFimg(vY,fX,fY,fW,fH,sw,sh,0,     path, (ext=='F' ? "sFY":"sBY"), flags|2);
  } else {
    /* prn("reading RAW BINARY %s.F[XY]", path); */
    rowReadFlt( vX,fX,fY,fW,fH,      0,NULL,path, (ext=='F' ? "rFX":"rBX"), flags);
    rowReadFlt( vY,fX,fY,fW,fH,      0,NULL,path, (ext=='F' ? "rFY":"rBY"), flags);
  }
}



int
inReadAll ( pMOB mob[], MobData *wwD
	  , pDmj dD, pDmj sD, pDmj iD, pDmj fD, char **argStr)
{
static	 flt	 triP[MAXTRI][12];
static	 ppf	  fvX[MAXSRC],  fvY[MAXSRC],  bvX[MAXSRC], bvY[MAXSRC];
static	 ppf	 dRGB[MAXSRC], sRGB[MAXSRC], sPre[MAXSRC], srcG[MAXSRC];
static	 ppu	 inpU[MAXSRC][4];
static	 LblFace dLbl[MAXSRC], sLbl[MAXSRC];
static	 AffTrf  aTrf[MAXSRC];
static	 pGlst	  pGi[MAXSRC],  pGs[MAXSRC];

char     path[128];
char    *iname[MAXSRC];
ppu      srcU[MAXSRC][4]; 
int      j,pose=3, doExtraction=1;
int 	 tL,tT,tR,tB, sL,sT,sR,sB, cX,cY, numTri=0;
int      dX,dY, sX,sY, fX,fY, iX=0, iY=0;
unt      dW,dH, sW,sH, fW,fH, iW=0, iH=0;
unt  	 w,h, ch=4, chan=4;
dbl      dd,fracAngle=1.0;
ppf	 ffC;
unt     z;

  for (j = 0; j < NumSrc; j++) {
    mob[j]          =  (MOB *)calloc(1,sizeof(MOB));
    mob[j]->name    =  iname[j] = ArgStr[j+1];
    ch = sgiReadUn(inpU[j],&w,&h,0,0,2048,2048,iname[j],0);    /** READ IMAGES **/
    if (j) {
      if (ch != chan) {
        warn("sgiReadUn sez %s is %d chan, but %s is %d chan",iname[0],chan,iname[j],ch);
        chan = MIN(chan,ch);
      }
      if (iW != w || iH != h) {
        warn("Size of input image %d is %d x %d, tho image 0 was %d x %d",j,w,h,w,h);
        iW = MIN(iW,w),  iH =  MIN(iH,h);
      }
    }
    else {
      iW = w,  iH = h,  chan = ch;
    }

    if ( ! (readFaceLbl(&sLbl[j],iname[j]))) { 
      wrn("readFaceLbl (in inReadAll, inR.c): returned 0"); 
      doExtraction = 0;
    }
    else
      prn("inReadAll: sLbl[%d]:  l_eye: %.5f  %.5f    r_eye: %.5f %.5f   cntr: %.5f %.5f"
        ,j,sLbl[j].lex,sLbl[j].ley,sLbl[j].rex,sLbl[j].rey,sLbl[j].nox,sLbl[j].noy);

  }
  iD->x = iX, iD->y = iY, iD->w = iW, iD->h = iH, iD->d = chan;
  *fD = *dD = *sD = *iD;


#if  0  /* @@ DEBUG FOR INDY DISPLAY */
  if (1) {
    pWmj   wp;
    double zz;
    zz = Zoom;
    Zoom = 1.0;
    wp   = showMjU(inpU[0],iD,NULL,"INDY DISP. DEBUG KLUDGE",4);
    wrn("DEBUG FOR INDY DISPLAY: did it show first image?");
    Zoom = zz;
    /* winLoopSgi(0); */
  }
#endif


/*
wrn("inReadAll AFT  read: iXYWH:  %d  %d    %u  %u", iX,iY,iW,iH);
wrn("AFTER *STRUCT ASSGN: iXYWH:  %d  %d    %u  %u", iD->x,iD->y,iD->w,iD->h);
wrn("AFTER *STRUCT ASSGN: fXYWH:  %d  %d    %u  %u", fD->x,fD->y,fD->w,fD->h);
*/

/** Three affine trf's: in0 to ex0, in1 to ex1, and in0 to in1 (ex0 to ex1 is 0) **/

  if (Devopts & O_STAT)  doExtraction = 0;

  if (doExtraction) {
    if (Devopts & O_TEST)  {
      sD->x = sD->y = sX = sY = 0;
      sD->w = sD->h = sW = sH = 272;
      for (j = 0; j < NumSrc; j++) {
        canonCropFace(srcU[j],inpU[j],sD,iD,&dLbl[j],&sLbl[j]);
      }
    }
    else {
      sD->x = sD->y = sX = sY = 0;
      sD->w = sD->h = sW = sH = 204;
  
      sL = sT = INT_MIN; sR = sB = INT_MAX;
      for (j = 0; j < NumSrc; j++) {
  
        if (ICCV) {
          if (iname[j][0] == 'c')  pose = 2;
          if (iname[j][0] == 'b')  pose = 4;
        }
  
        canonEyeLocs(&aTrf[j],&dLbl[j],&sLbl[j],sD,iD,pose,fracAngle,          1);
        canonWarpExtract(srcU[j],inpU[j],       sD,iD,&aTrf[j],         &sLbl[j],1);
        if (ICCV) {
          sX = sY = 0;  sW = sD->w; sH = sD->h;
        }
        else {
          canonExtractFace(srcU[j],srcU[j],       sD,iD,&aTrf[j],&dLbl[j],&sLbl[j]
                                     ,&cX,&cY,&fX,&fY,&fW,&fH,&tL,&tT,&tR,&tB, 1, pose);
  
          wrd("FROM canonExtractFace: cXY: %3d %3d  tLTRB:  %3d %3d %3d %3d",cX,cY,tL,tT,tR,tB);
          if (j)  {
            if (sX != fX || sY != fY)  die("AF canon, sX:%d  fX:%d", sX,fX);
            if (sW != fW || sH != fH)  die("AF canon, sW:%d  fW:%d", sW,fW);
          }
          else {          sX =  fX,   sY =  fY,   sW =  fW,   sH =  fH ;
            if (tL > sL)  sL = tL;
            if (tT > sT)  sT = tT;
            if (tR < sR)  sR = tR;
            if (tB < sB)  sB = tB;
          }
        }
      }
    }

#if 00
    if (Devopts & O_WARN) {
      dd = Zoom; Zoom = 2.0;
      for (j = 0; j < NumSrc; j++) {
        sprintf(path, "srcU.%s.%d.%d.%u.%u",iname[j],sD->x,sD->y,sD->w,sD->h);
        showItWithDots(srcU[j],&dLbl[j],sD,path,NULL,&pGs[j]);
      }
      Zoom = dd;
    }
#endif

    if (NumSrc == 2) {
      sprintf(path,"%s+%s.tris",ArgStr[1],ArgStr[2]);
      numTri = jkReadTriPairs(triP,MAXPRS,path,iH-1,MJ_INVTY);
      /*
      prn("jkReadTriPairs(%s) found this many TRIANGLES %d",ArgStr[NumSrc+1],numTri);
      */
    }

    if ((Options & O_VERB) && (Devopts & O_WARN)) {
      dd = Zoom; Zoom = 1.0;
      for (j = 0; j < NumSrc; j++) {
        sprintf(path, "inpU.%s.%dx%d",iname[j],iW,iH);
        showItWithTri(inpU[j],&sLbl[j],iD,path,NULL,&pGi[j],triP,numTri,(j%2));
      }
      Zoom = (flt)dd;
    }

    jkTrfTriPairs(triP,numTri,aTrf);
    if ((Options & O_VERB) && (Devopts & O_WARN)) {
      dd = Zoom; Zoom = 2.0;
      for (j = 0; j < NumSrc; j++) {
        sprintf(path, "srcU.%s.%dx%d TRF",iname[j],sW,sH);
        showItWithTri(srcU[j],&sLbl[j],sD,path,NULL,&pGs[j],triP,numTri,(j%2));
      }
      Zoom = (flt)dd;
    }
    if ((Devopts & O_WARN) && Options & O_KLUG)  winLoop(0);
  }
  else {
    wrn("NO EXTRACTION: iXYWH:  %d  %d    %u  %u", iX,iY,iW,iH);
    iX = iD->x, iY = iD->y, iW = iD->w, iH = iD->h, ch = iD->d;
    *fD = *dD = *sD = *iD;

    for (j = 0; j < NumSrc; j++)
      for (z = 0; z < ch; z++)
        srcU[j][z] = inpU[j][z];
    sX = sD->x;
    sY = sD->y;
    sW = sD->w;
    sH = sD->h;
  }





  dX =  sX;  /* sX + 6; */
  dY =  sY;  /* sY + 6; */
  dW =  sW;  /* sW - MIN(12,sR); */
  dH =  sH;  /* sH - MIN(12,sB); */
  fX =  sX - ImB;
  fW =  sW + ImB*2;
  fY =  sY - ImB;
  fH =  sH + ImB*2;

#define PDMJ_SET(M,X,Y,W,H,D) {M->x = X, M->y = Y, M->w = W, M->h = H, M->d = D;}

  PDMJ_SET(dD,dX,dY,dW,dH,ch);
  PDMJ_SET(sD,sX,sY,sW,sH,ch);
  PDMJ_SET(fD,fX,fY,fW,fH,1);


  for (j = 0; j < NumSrc; j++) {
    srcG[j] =  rowMalFlt(fX,fY,fW,fH);  /* Must have border >= Image_border for pyramids */
    if (ch >= 3) {
      sRGB[j] =  rowMalFlt(sX*3,sY,sW*3,sH);
      rgbGrayUnc3(sRGB[j],srcG[j],srcU[j],sX,sY,sX,sY,sW,sH,sX,sY);
      if (Options & O_LBLS) {   /** -l **/
        sprintf(path,"sRGB.%s.%d.%d.%u.%u",iname[j],sX,sY,sW,sH);
        rgbWriteSGIn( sRGB[j],sX,sY,sW,sH,path);
        /*showF(      sRGB[j],sX,sY,sW,sH,3,NULL,path,0, Zoom); */
      }
    }
    else {
      if (!j)   warn("inLoadAll: ch:%d < 3, WILL PROBABLY CRASH!", ch);
      sRGB[j] = NULL;
      rowCopyUF(srcG[j],srcU[j][0],sX,sY,sW,sH,sX,sY);
    }
    rowBordFlt( srcG[j],sX,sY,sW,sH,ImB,BD_COPY);
  }

  mob[0]->foX = fvX[0] = bvX[0]    = rowCalFlt(fX,fY,fW,fH);
  mob[0]->foY = fvY[0] = bvY[0]    = rowCalFlt(fX,fY,fW,fH);
  mob[0]->foC = mob[0]->boC = ffC  = rowCalFlt(fX,fY,fW,fH);

  sPre[0] = sRGB[0];




#if 00

if (NumSrc == 12)  {
  Options &= ~O_BACK;
  for (j = 1; j < 9; j++) {        /* Fwd should be optional, too */
    sprintf(path,"%s-%s",mob[0]->name,mob[j]->name);
    if (! (Options & O_BACK) || Options & O_FWRD || Devopts & O_BOTH || Devopts & O_EXTR) {
      if (Options & O_READ) {
        readFlows(&fvX[j],&fvY[j],fX,fY,fW,fH,0,path,'F',1);
      }
      if ( ! fvX[j] || ! fvY[j]) {
        fvX[j] = rowCalFlt(fX,fY,fW,fH);
        fvY[j] = rowCalFlt(fX,fY,fW,fH);
        prn("flowing %s", path);
        rowFlowBac(fvX[j],fvY[j],ffC,srcG[j],srcG[0],sX,sY,sW,sH,fX,fY,fW,fH,PyrTop,PyrBot,0);
        if (Options & O_WRIT) {
          writeFlows(fvX[j],fvY[j], fX,fY,fW,fH, path, 'F', 1);
        }
      }
      if (Devopts & O_PTRS) {
        sPre[j] = rowMalFlt(sX*3,sY,sW*3,sH);
        rgbWarpBilFlt(sPre[j],sRGB[j],fvX[j],fvY[j],sX,sY,sW,sH,sX,sY,sW,sH,1.0);
      }
    }
    if (Options & O_BACK || Devopts & O_BOTH) {
      if (Options & O_BACK) dRGB[j] =  rowCalFlt(dX*3,dY,dW*3,dH);
      if (Options & O_READ) {
        readFlows(&bvX[j],&bvY[j],fX,fY,fW,fH,0,path,'B',1);
      }
      if ( ! bvX[j] || ! bvY[j]) {
        bvX[j] = rowMalFlt(fX,fY,fW,fH);
        bvY[j] = rowMalFlt(fX,fY,fW,fH);
        prn("flowing %s B", path);
        rowFlowBac(bvX[j],bvY[j],ffC,srcG[0],srcG[j],sX,sY,sW,sH,fX,fY,fW,fH,PyrTop,PyrBot,0);
        if (Options & O_WRIT) {
          writeFlows(bvX[j],bvY[j], fX,fY,fW,fH, path, 'B', 1);
        }
      }
    }
  }
  j = 12;
  sprintf(path,"%s-%s",mob[3]->name,mob[11]->name);
  prn("reading %s.F[XY] for flow %d", path, j);
  rowReadFlt(&fvX[j],fX,fY,fW,fH,0,NULL,path,".FX",1);
  rowReadFlt(&fvY[j],fX,fY,fW,fH,0,NULL,path,".FY",1);
  if ( ! fvX[j] || ! fvY[j]) 
    die("inR.c: inReadAll: ERROR reading %s", path);

  j = 13;
  sprintf(path,"%s-%s",mob[3]->name,mob[10]->name);
  prn("reading %s.F[XY] for flow %d", path, j);
  rowReadFlt(&fvX[j],fX,fY,fW,fH,0,NULL,path,".FX",1);
  rowReadFlt(&fvY[j],fX,fY,fW,fH,0,NULL,path,".FY",1);
  if ( ! fvX[j] || ! fvY[j]) 
    die("inR.c: inReadAll: ERROR reading %s", path);

  j = 14;
  sprintf(path,"%s-%s",mob[3]->name,mob[9]->name);
  prn("reading %s.F[XY] for flow %d", path, j);
  rowReadFlt(&fvX[j],fX,fY,fW,fH,0,NULL,path,".FX",1);
  rowReadFlt(&fvY[j],fX,fY,fW,fH,0,NULL,path,".FY",1);
  if ( ! fvX[j] || ! fvY[j]) 
    die("inR.c: inReadAll: ERROR reading %s", path);

  j = 15;
  sprintf(path,"%s-%s",mob[3]->name,mob[4]->name);
  prn("reading %s.F[XY] for flow %d", path, j);
  rowReadFlt(&fvX[j],fX,fY,fW,fH,0,NULL,path,".FX",1);
  rowReadFlt(&fvY[j],fX,fY,fW,fH,0,NULL,path,".FY",1);
  if ( ! fvX[j] || ! fvY[j]) 
    die("inR.c: inReadAll: ERROR reading %s", path);

} else {

#endif

#if 0000
  if (Devopts & O_UNCH) {
    warn("Going to time rowFlowBac: %d %d  %d times.........",fW,fH,Ntweens);
    BEGCLOCK("")
    for (j = 0; j < Ntweens; j++) {
      rowFlowBac(fvX[0],fvY[0],ffC,srcG[0],srcG[1],sX,sY,sW,sH,fX,fY,fW,fH,PyrTop,PyrBot,0);
    }
    ENDCLOCK("Done timing")
    die("time was of the essence.");
  }
#endif




  for (j = 1; j < NumSrc; j++) {	/* Fwd should be optional, too */
    sprintf(path,"%s-%s",mob[0]->name,mob[j]->name);
    if (! (Options & O_BACK) || Options & O_FWRD || Devopts & O_BOTH || Devopts & O_EXTR) {
      if (Options & O_READ) { 
        readFlows(&fvX[j],&fvY[j],fX,fY,fW,fH,0,path,'F',1);
      }
      if ( ! fvX[j] || ! fvY[j]) {
        fvX[j] = rowMalFlt(fX,fY,fW,fH);
        fvY[j] = rowMalFlt(fX,fY,fW,fH);
        prn("flowing %s", path);

/* @@ */
#if 0000
        if (Options & O_GRAY)
          gaussFlow( fvX[j],fvY[j],ffC,srcG[0],srcG[j],fX,fY,fW,fH,PyrBot,PyrBot,PyrTop);
        else
#endif
          rowFlowBac(fvX[j],fvY[j],ffC,srcG[j],srcG[0],sX,sY,sW,sH,fX,fY,fW,fH,PyrTop,PyrBot,0);

        if (Options & O_WRIT) {
          writeFlows(fvX[j],fvY[j], fX,fY,fW,fH, path, 'F', 1);
        }
      }
      if (Devopts & O_PTRS) {
        sPre[j] = rowMalFlt(sX*3,sY,sW*3,sH);
        rgbWarpBilFlt(sPre[j],sRGB[j],fvX[j],fvY[j],sX,sY,sW,sH,sX,sY,sW,sH,1.0);
      }
    }

    if (Options & O_BACK || Devopts & O_BOTH || Devopts & O_EXTR) {
      if (Options & O_BACK) dRGB[j] =  rowCalFlt(dX*3,dY,dW*3,dH);
      if (Options & O_READ) {
        readFlows(&bvX[j],&bvY[j],fX,fY,fW,fH,0,path,'B',1);
      }
      if ( ! bvX[j] || ! bvY[j]) {
        bvX[j] = rowMalFlt(fX,fY,fW,fH);
        bvY[j] = rowMalFlt(fX,fY,fW,fH);
        prn("flowing %s B", path);

/* @@ */
#if 0000
        if (Options & O_GRAY)
          gaussFlow( bvX[j],bvY[j],ffC,srcG[j],srcG[0],fX,fY,fW,fH,PyrBot,PyrBot,PyrTop);
        else 
#endif
          rowFlowBac(bvX[j],bvY[j],ffC,srcG[0],srcG[j],sX,sY,sW,sH,fX,fY,fW,fH,PyrTop,PyrBot,0);
        if (Options & O_WRIT) {
          writeFlows(bvX[j],bvY[j], fX,fY,fW,fH, path, 'B', 1);
        }
      }
    }
  }
#if 00
}
#endif

  if (Devopts & O_BQDR) {       /* -Q */
    warn("Outputting %d src & pre-warped images as src|pre.[A-D]",NumSrc);
    for (j = 0; j < NumSrc; j++) {
      sprintf(path,"%s.%c","pre",('A'+j));
      rgbWriteSGIn(sPre[j],dX,dY,dW,dH,path);
      sprintf(path,"%s.%c","src",('A'+j));
      rgbWriteSGIn(sRGB[j],dX,dY,dW,dH,path);
    }
  }

  for (j = 0; j < NumSrc; j++) {
    mob[j]->dD      =  dD;
    mob[j]->sD      =  sD;
    mob[j]->fD      =  fD;
    mob[j]->iD      =  iD;
    mob[j]->atrf    = &aTrf[j];
    mob[j]->sLbl    = &sLbl[j];
    mob[j]->dLbl    = &dLbl[j];
    mob[j]->inpu[0] =  inpU[j][0];
    mob[j]->inpu[1] =  inpU[j][1];
    mob[j]->inpu[2] =  inpU[j][2];
    mob[j]->inpu[3] =  inpU[j][3];
    mob[j]->rgbi    =  sRGB[j];
    mob[j]->prew    =  sPre[j];
    mob[j]->gray    =  srcG[j];
    mob[j]->foX     =  fvX[j];
    mob[j]->foY     =  fvY[j];
    mob[j]->boX     =  bvX[j];
    mob[j]->boY     =  bvY[j];
    mob[j]->nzX     =  5.0;
    mob[j]->nzY     =  5.0;
  }

  wwD->mob  = mob;
  wwD->ndst = 0;
  wwD->nsrc = NumSrc;
  wwD->stat = 0;
  wwD->dWin = NULL;
  wwD->dD   = dD;
  wwD->sD   = sD;
  wwD->fD   = fD;
  wwD->iD   = iD;
  wwD->sLbl = sLbl;
  wwD->dLbl = dLbl;
  wwD->aTrf = aTrf;
  wwD->dst  = dRGB[0] = rowMalFlt(dX*3,dY,dW*3,dH);
  wwD->wts  = rowCalFlt(sX,  sY,sW,  sH);  /* changed from d to s, 7/15/95 */
  wwD->dsrc = rowCalFlt(sX*3,sY,sW*3,sH);
  wwD->dRGB = dRGB;
  wwD->sRGB = sRGB;
  wwD->sPre = sPre;
  wwD->srcG = srcG;
  wwD->fvX  = fvX;
  wwD->fvY  = fvY;
  wwD->bvX  = bvX;
  wwD->bvY  = bvY;
  wwD->ffC  = ffC;
  wwD->accW = rowMalFlt(fX,fY,fW,fH);
  wwD->accX = rowMalFlt(fX,fY,fW,fH);
  wwD->accY = rowMalFlt(fX,fY,fW,fH);
  wwD->func = NULL;   /* what should go here? */

  fprintf(stderr,
"inR: iS: %d %d %u %u  dS: %d %d %u %u  sS: %d %d %u %u  fS: %d %d %u %u  ImB:%d\n"
	,iX,iY,iW,iH, dX,dY,dW,dH, sX,sY,sW,sH, fX,fY,fW,fH,Image_border);

  return NumSrc;
}

