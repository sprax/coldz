/****** aaCoords.h -- prototypes & macros for functions in aaCoords.c.
        S.R. Lines  (srl)
        12.9.92  (Orig. 9.92) *********/

#ifndef _aaCoords_h_   /* Should be included after aa.h */
#define _aaCoords_h_

/*
KEYS: w = wide, t = tall, d = deep, rh = rho (radius), ph = phi, th = theta
      L = MAX(w,t), W = MIN(w,t), D = Depth or min{w,t,d}
      R = max{rh} or max{r} = sqrt(w*w + t*t)/2.0 for code C.
      Types: N = 32-bit int, F = float.
      Ranges: Cartesian products of intervals.  P = PI, x0 & x1 = 1st & 2nd coords. 
CODE  MNEMONIC         TYPE RANGES/SCALES/DOMAINS               NOTES/USAGE
   A  Absolute   Pixel  NN  [0,w) x [0,t) = [0,w-1] x [0,t-1]   default
   B  StdBuffer  Pixel  NN  [0,512) x [0,512)                   prototyping
   C  Centered   Pixel  NN  [-w/2,w-w/2) x [-t/2,t-t/2)         cartesian geometry
   D  Device     Pixel  NN  [x0,x1)x[y0,y1) = [0,w-1]x[0,t-1]   buffer/device relative
   E  Absolute   Float  FF  [0,wide) x [0,tall)                 just cast A
   F  Normal     Float  FF  [0,w/L) x [0,t/L)                  "device independent"
   G  Cent. Norm Float  FF  [-w/L,w/L) x [-t/L,t/L)             cartesian geometry
   H  Unit Square Cent. FF  [-0.5,0.5) x [-0.5,0.5)           ? Use J instead?
   I  Unit Square       FF  [0,1 = w/w) x [0,1 = t/t)           abstract
   J  2-Unit Sqr. Cent. FF  [-1,1) x [-1,1)                     singular functions
   K  Complex Unit Disk FF  [0,1) x [0,2*P)     (??)            planar mappings
   L  LogPolar   Float  FF  [0,ln(1+R)) x [-P,P)                strange max rh.
   M  LogPolar Lnormal  FF  [0,1=ln(1+R/R)/ln(2.0))x[-P,P)      code O with A=1/ln(2), B=1
   N  LogPolar Enormal  FF  [0,1=ln(e))) x [-P,P)               code 0 with A=1, B=(e-1)/R
   O  LogPolar  Params  FF  [0,1=A*ln(1 + B*R)) x [-P,P)        A & B satisfy (1 + B*R)^A = e
   P  Polar Pixel-Deg.  NN  [0,0.5*sqrt(w*w+t*t)) x [-180,180)  can offset origin
   Q  Polar     Normal  FF  [0,1=R/R) x [-P,P)                ? Same use as K?
   R  Polar Sqr Normal  FF  [0,1=R^2/R^2) x [-P,P)              Efficiency shortcut/physics
   S
   T  Triangular Index  NN  [0,(L+1)/2) x [0,x1)                For radial symmetry
   U,V
      _________  the rest are probably nonsense __________
   W  Spiral Outward    NN  [0,L/2) x [0,4 times first coord) ? Ever used to index?
   X  Spiral Inward     NN  [0,L/2) x [0,4 times (L-x0))      ? Ever used to index?
   Y  Log TwoPi-disk    FF  [0,ln(2*P)) x [0,2*P)               log spirals, etc.          
   Z  TwoPi-disk        FF  [0,2*P) x [0,2*P)                   linear spirals, etc.                 
*/
        /* MATHEMATICAL PRELIMINARIES */
#define ATAN2(y,x)        ((x||y) ? atan2((double)y,(double)x) : 0.0) /* Use 0.0L instead? */
#define MAXABS(a,b)       

        /* COORDINATE CONVERSION */
#define Cxy2abT(x,y,A,B,t)  (A=x;if(A<0) A = -A; B=y; if(B<0) B = -B; if (B>A) SWAP(A,B,t))

#define Cxy2rpP(x,y,r,h)  (r = sqrt((double)(x*x+y*y)), h = ATAN2(y,x))
#define Cxy2rpL(x,y,r,h)  (r = 0.5*log(1.0L+(x*x+y*y)), h = ATAN2(y,x))
#define Prh2xyC(r,h,x,y)  (x = (int)r*cos(h), y = (int)r*sin(h))

        /* SYMMETRICAL ASSIGNMENTS */
#define CxySYM_MULT(x,y,pc,wd,v)  (*(pc-y*wd+x)=v,*(pc-y*wd-x)=v,*(pc+y*wd-x)=v,*(pc+y*wd+x)=v)

        /* GET & PUT VALUES FROM FLOATING-POINT LOOKUP TABLE */
#define GET_I_FLUT(x,y,tp,XS,YS)  (*(tp+((long)(XS*(x + y*YS)))))


#define OOB(x0,x,x1) ((x) < (x0) || (x) >= (x1))

#if     (DEBUG >= BUG_SDATA)   /* Print data even if good */
#define IBOUND(x0,x,x1)\
        fprintf(stderr,"%s:(%4d <= %4d < %4d)?\n"\
        ,__FILE__,x0,x,x1);\
        if (     OOB(x0,x,x1)) {\
        aaFail(0);\
        }
#else
#if    (DEBUG >= BUG_EDATA)   /* Print data (and message) only on error */
#define IBOUND(x0,x,x1,y0,y,y1)\
        if (     OOB(x0,x,x1)) {\
        fprintf(stderr,"%s:(%4d <= %4d < %4d)?\n"\
        ,__FILE__,x0,x,x1);\
        aaFail(0);\
        }
#else   /* default: DEBUG_OFF: if no higher level applies, do nothing. */
#define IBOUND(x0,x,x1)
#endif
#endif


#define OOBS(x0,x,x1,y0,y,y1) ((x)<(x0)||(x)>=(x1)||(y)<(y0)||(y)>=(y1))

#if     (DEBUG >= BUG_SDATA)   /* Print data even if good */
#define IBOUNDS(xd,yd,x0,x,x1,y0,y,y1,eno)\
        fprintf(stderr,"%s: %d,%d (%4d <= %4d < %4d && %4d <= %4d < %4d)?\n"\
        ,__FILE__,xd,yd,x0,x,x1,y0,y,y1);\
        if (     OOBS(x0,x,x1,y0,y,y1)) {\
        aaFail((eno));\
        }
#else
#if    (DEBUG >= BUG_EDATA)   /* Print data (and message) only on error */
#define IBOUNDS(xd,yd,x0,x,x1,y0,y,y1,eno)\
        if (     OOBS(x0,x,x1,y0,y,y1)) {\
        fprintf(stderr,"%s: %d,%d (%4d <= %4d < %4d && %4d <= %4d < %4d)?\n"\
        ,__FILE__,xd,yd,x0,x,x1,y0,y,y1);\
        aaFail((eno));\
        }
#else   /* default: DEBUG_OFF: if no higher level applies, do nothing. */
#define IBOUNDS(xd,yd,x0,x,x1,y0,y,y1,eno)
#endif
#endif

#endif  /* _aaCoords_h_ */


