/*****************************************************************************
 winZoom.c    S.R.Lines    95.5.14
 *****************************************************************************/
#include        <un/unBug.h>
#include        <un/unTypes.h>
#include        <gl/gl.h>
#include        <gl/device.h>
#include	<m/mj.h>
#include	<winLoop.h>
#include	<winDraw.h>

void
rubberRectZoid (void *v, int device, int mouseDown)
{
static	WmjAct	backAct;
pWmj	cW = (pWmj)v;
WmjAct *Ap = (device == LEFTMOUSE ? cW->lefMouse : cW->midMouse);
/* Tetra  *trp  = Ap->data; */
flt     zX = cW->zX, zY = cW->zY;
long    bxA, byA, nxA, nyA, oxA,oyA, wx,ww,wy,wh;
long	widA = winget();

  if (! mouseDown)  return;

  getorigin(&wx,&wy);       
  getsize(  &ww,&wh); ww -= 1; wh -= 1;

  oxA = bxA = (getvaluator(MOUSEX) - wx);
  oyA = byA = (getvaluator(MOUSEY) - wy);

  cursoff();
  drawmode(OVERDRAW);

  while(getbutton(device)) {
    nxA  = getvaluator(MOUSEX) - wx; if (nxA < 0)  nxA = 0;  else if (nxA > ww)  nxA = ww;
    nyA  = getvaluator(MOUSEY) - wy; if (nyA < 0)  nyA = 0;  else if (nyA > wh)  nyA = wh;
    if (nxA != oxA || nyA != oyA) {

      winset(widA);
      color(BLACK);                   /* black erases */
      sboxi(bxA,byA,oxA,oyA);
      color(RED);                     /* set color of new circle */
      sboxi(bxA,byA,nxA,nyA);
      oxA = nxA,  oyA = nyA;
    }
  }
  color(BLACK);
  sboxi(bxA,byA,oxA,oyA);
  drawmode(NORMALDRAW);
  cpack(FcGRN);
  sboxi(bxA,byA,nxA,nyA);

  if (bxA > nxA)  nxA = bxA, bxA = oxA;
  if (byA > nyA)  nyA = byA, byA = oyA;
  wrn("rubberRectZ B4 showSub: %4d  %4d  %4d  %4d   zX:%.2f  zY:%.2f",bxA,byA,nxA,nyA, zX, zY);

  backAct = *Ap;
  backAct.func = &testChain;
  cW->next = showSub(cW,   bxA/zX,byA/zY,nxA/zX,nyA/zY,"subA",zX*2.0,zY*2.0,5);
  cW->next->lefMouse = &backAct;
  cW->next->prev = cW;

  winset(widA);
  curson();
}



void
backTrack (void *v, int device, int mouseDown)
{
pWmj    cW = (pWmj)v, pW = cW->prev;
/* WmjAct *Ap = (device == LEFTMOUSE ? cW->lefMouse : cW->midMouse); */
pDmj    cD = &cW->D;
long    bxA, byA, nxA, nyA, oxA,oyA, wx,ww,wy,wh;
long    widA = winget();

  if (! mouseDown)  return;

  getorigin(&wx,&wy);
  getsize(  &ww,&wh); ww -= 1; wh -= 1;

  oxA = bxA = (getvaluator(MOUSEX) - wx);
  oyA = byA = (getvaluator(MOUSEY) - wy);

  cursoff();
  drawmode(OVERDRAW);

  while(getbutton(device)) {
    nxA  = getvaluator(MOUSEX) - wx; if (nxA < 0)  nxA = 0;  else if (nxA > ww)  nxA = ww;
    nyA  = getvaluator(MOUSEY) - wy; if (nyA < 0)  nyA = 0;  else if (nyA > wh)  nyA = wh;
    if (nxA != oxA || nyA != oyA) {
      /* winset(widA); */
      color(BLACK);             /* black erases */
      sboxi(bxA,byA,oxA,oyA);   /* NB: sbox is faster than rect, but less flex */
      color(RED);                     /* set color of new circle */
      sboxi(bxA,byA,nxA,nyA);
      oxA = nxA,  oyA = nyA;
    }
  }
  color(BLACK);
  sboxi(bxA,byA,oxA,oyA);
  drawmode(NORMALDRAW);
  cpack(FcGRN);
  sboxi(bxA,byA,nxA,nyA);

  if (pW) {
    pDmj        pD = &pW->D;
    flt         pzX = pW->zX, zX = pzX/cW->zX;
    flt         pzY = pW->zY, zY = pzY/cW->zY;
    long        L = cD->l*pzX, B = cD->b*pzY;
    wrn("bT: id: %d  pD->lb: %3d %3d   zXY: %.1f %.1f", pW->id, pD->l, pD->b,    zX,    zY);
    wrn("bT: id: %d  cD->lb: %3d %3d  czXY: %.1f %.1f", cW->id, cD->l, cD->b,cW->zX,cW->zY);
    winset(pW->id);
    drawmode(OVERDRAW);
    color(YELLOW);
    sboxi(L + bxA*zX, B + byA*zY, L + oxA*zX, B + oyA*zY);
    drawmode(NORMALDRAW);
  }
  winset(widA);
  curson();
}


