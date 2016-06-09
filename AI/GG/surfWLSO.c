/*****************************************************************************
 surfWLSO.c    S.R.Lines    95.05.08
 surface interpolation using Weighted Least Squares & Orthogonal Polynomials
#include        <unMath.h>
 *****************************************************************************/
#include        <math.h>
#include        <un/unMacro.h>
#include        <un/unTypes.h>
#include        <un/unBug.h>
#include        <m/mj.h>
#include        "blerp.h"

#define MAXTERMS 10	/* WAS: 10 */
#define MAXCP	512
static float	*Wc, *Xc, *Yc, *Zc;
static float	Ac[MAXTERMS][MAXTERMS];
static int	NumCP;

static float
basis (int n, float x, float y)
{
float     h; 
  switch (n) {
    case  0:	h = 1.0;	break;
    case  1:	h = x;		break;
    case  2:	h = y;		break;
    case  3:	h = x*x;	break;
    case  4:	h = x*y;	break;
    case  5:	h = y*y;	break;
    case  6:	h = x*x*x;	break;
    case  7:	h = x*x*y;	break;
    case  8:	h = x*y*y;	break;
    case  9:	h = y*y*y;	break;
    default:    warn("basis [%s]: no case %d",__FILE__,n);
  }
return h;
}



static float
poly (int k, float x, float y)
{
int     i;
float     p;

  for (p = i = 0; i < k; i++) {
    p += (Ac[k][i] * poly(i,x,y));
  }
  p += (Ac[k][k] * basis(k,x,y));
  return p;
}


static
float coef (int k)
{
int  	i;
float 	p, num, denum;

  num = denum = 0;
  for (i = 0; i < NumCP; i++) {
    p = poly( k,  Xc[i], Yc[i]);
    num   += (Wc[i]*Zc[i]*p);
    denum += (Wc[i]*p*p);
  }
  return (num/denum);
}


static float
init_alpha (int j, int k)
{
int 	i;
float 	a, h, p, num, denum;

  if      (k == 0)  a = 1.0;	 	/* case 0:  a_j0 */
  else if (j == k)  {		 	/* case 1:  a_jj */
    num = denum = 0;
    for (i = 0; i < NumCP; i++) {
      h = basis(j, Xc[i], Yc[i]);
      num   += (Wc[i]  );
      denum += (Wc[i]*h);
    }
    a = -num/denum;
  }
  else {				/* case 2: a_jk, j != k */
    num = denum = 0;
    for (i = 0; i < NumCP; i++) {
      h = basis(j, Xc[i], Yc[i]);
      p = poly( k, Xc[i], Yc[i]);
      num   += (Wc[i]*p*h);
      denum += (Wc[i]*p*p);
    }
    a = -Ac[j][j]*num/denum;
  }
  return a;
}


#if 1

void
surfWLSO (ppf dst, ppf wts, float wc[], float xc[], float yc[], float zc[]
, int sX, int sY, int sW, int sH, int nCP, int maxterms, dbl smoother, flt dQ[4][2])
{
static  tc = 0;
#if 00
float   dx0  = dQ[0][0], dx1  = dQ[1][0],  dx2 = dQ[2][0], dx3 = dQ[3][0];
float   dy0  = dQ[0][1], dy1  = dQ[1][1],  dy2 = dQ[2][1], dy3 = dQ[3][1];
float   dx10 = dx1 - dx0,  dx23 = dx2 - dx3, dy30 = dy3 - dy0,  dy21 = dy2 - dy1;
#endif
int 	i,j,k, x,y, t,terms;
float 	a,f,g, p, uf,um,vf,vm,  dxx,dyy;
double 	dsx,dsy,dss;

  NumCP = nCP;
  Wc    = wc;
  Xc    = xc;
  Yc    = yc;
  Zc    = zc;

  /* determine number of terms necessary for error < 0.5 (optional) */
  if (maxterms > MAXTERMS)  maxterms = MAXTERMS;
  for (terms = 3; terms < maxterms; terms++) {
    for (i = 0; i < NumCP; i++) {
      /* init Wc: the weights of the NumCP control points on Xc,Yc */
      for (j = 0; j < NumCP; j++) {
        dsx = (Xc[i] - Xc[j]);  dsx *= dsx;
        dsy = (Yc[i] - Yc[j]);  dsy *= dsy;
        Wc[j] = 1.0/sqrt(dsx + dsy + smoother);
      }
      /* init Ac: alpha_jk coeffs of the ortho polynomials */
      for (j = 0; j < terms; j++)    Ac[j][j] = init_alpha(j,j);
      for (j = 0; j < terms; j++)
        for (k = 0; k < j; k++)      Ac[j][k] = init_alpha(j,k);
      /* compute error at each control point over all terms */
      for (f = t = 0; t < terms; t++) {
        a  = coef(t);
        p  = poly(t, Xc[i], Yc[i]);
        f += (a*p);
      }
      g = Zc[i] - f;
      if (ABS(g) > 0.5)  break;
    }
    if (i == NumCP)  break;
  }
prn("surfWLSO %3d: sXY:  %3d %3d  %3d %3d   terms: %d",++tc, sX,sY,sW,sH,terms);


  /* perform surface approximation */
  for (  y = sY; y < sY+sH; y++) {
    vf   = (flt)  y / sH;  vm = 1.0F - vf;
    for (x = sX; x < sX+sW; x++) {

#if 00
      uf = (flt) x / sW;   um = 1.0F - uf;
      dxx    = vm*(dx0 + uf*dx10) + vf*(dx3 + uf*dx23);
      dyy    = um*(dy0 + vf*dy30) + uf*(dy1 + vf*dy21);
#endif



      /* init Wc: the weights of the NumCP control pts on x,y */

      for (j = 0; j < NumCP; j++) {
        dsx = (x - Xc[j]);  dsx *= dsx;
        dsy = (y - Yc[j]);  dsy *= dsy;
#if 1
        Wc[j] = 1.0/sqrt(dsx + dsy + smoother);
#else
#define SQR(D,F)  (D = F, D*D)
        Wc[j] = 1.0/ SQR(dss, dsx + dsy + smoother);
#endif
      }
#if 0
      j = tc % nCP;
      f = Zc[j]*Wc[j];
#else
      /* init Ac: alpha_jk coeffs of the ortho polynomials */
      for (j = 0; j < terms; j++)    Ac[j][j] = init_alpha(j,j);
      for (j = 0; j < terms; j++)
        for (k = 0; k < j; k++)      Ac[j][k] = init_alpha(j,k);
      /* evaluate surface at point (x,y) over all terms */
      for (f = t = 0; t < terms; t++) {
        a  = coef(t);
        p  = poly(t, (float)x, (float)y);
        f += (a*p);
      }
#endif
      dst[y][x] = f;
    }
  }
}

#endif

