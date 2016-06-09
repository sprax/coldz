/*
 * $Id: mac.h,v 0.87 1993/10/11 00:00:00 zhao Pre-Release $
 *
 *. This file is part of BIT package.
 *
 *
 *  Copyright(c) 1993 by T.C. Zhao.  All rights reserved.
 *
 *  Permission to do anything with this software non-commercial and
 *  without fee is hereby granted, provided that the copyright notice
 *  and disclaimer appear in all copies and their documentation.
 *
 *  This software is provided "as is" without expressed or implied
 *  warranty of any kind.
 *.
 * Some useful macros.  Usual restrictions apply.
 * This file is #include'd by uproto.h as well as bit.h and
 * maybe by others
 */

#ifndef BIT_MAC_H
#define BIT_MAC_H

#ifdef lint
  extern const int ZERO;
#else
#  ifdef ZERO
#     undef ZERO
#  endif
#  define ZERO    0
#endif

/* General utility macros */

#ifndef Abs
#define Abs(a)      (((a) > 0) ? (a) : -(a))
#endif   /* Abs */

#ifndef Sgn
#define Sgn(a)      ((a < 0) ? -1: 1)
#endif

#ifndef  Min
#define  Min(a,b)   (((a) < (b)) ? (a):(b))
#endif

#ifndef Max
#define  Max(a,b)   (((a) > (b)) ? (a):(b))
#endif

/* might speed things up a bit */
#ifndef Streq
#define Streq(a,b)  (*(a) == *(b) && strcmp((a),(b)) == 0)
#endif

/* terminates strncpy properly, but failed the original return */
#ifndef Strncpy
#define Strncpy(s1,s2,n)    sprintf(s1, "%.*s", (int)n,  s2)
#endif

#ifndef Badfread
#define Badfread(buf, es, n, fp)   (fread(buf, es, n, fp) != (n))
#define Badfwrite(buf, es, n, fp)  (fwrite(buf, es, n, fp) != (n))
#endif

#ifndef Badfgets
#define Badfgets(buf, n, fp)       (fgets(buf, n, fp) == 0)
#define Badfputs(buf, n, fp)       (fputs(buf, n, fp) == 0)
#endif

/* Just make a safe free  */
#ifndef Free
#define Free(p)     do { if(p) free(p); (p) = 0;} while(ZERO)
#endif

#ifndef Range
#define Range(x, xmin, xmax)             \
        do {                             \
          if( x < (xmin)) x = (xmin);    \
          else if(x > (xmax)) x = (xmax);\
        } while (ZERO)
#endif

#ifndef Swap
#define Swap(type, a, b )                   \
            do {                            \
              register type tmp__ = (a);    \
              (a) = (b); (b) = tmp__;       \
            } while (ZERO)
#endif

#ifndef Lintp  /* linear interpolation with delta = 1 */
#define Lintp(a,b,dx)  (((dx) < 0.004) ? (a) : ((a) + ((b) - (a))*(dx)))
#endif

/* check if the point is within a rectangular region */
#ifndef Inside
#define Inside(x,y,x1,y1,x2,y2)    \
          ((x) >= (x1) && (x) <= (x2)  && (y) >= (y1) && (y) <= (y2))
#endif

/* macros specific to BIT program */

#define REPORT(j,t) if(((j)+1)%t==0) update_progress_report((j)+1)

#define SET_NICE(a)          \
         do {                \
            if((a) > 1000) (a) = ((a)/1000) * 1000;  \
            else if((a) > 100) (a) = ((a)/100) * 100;\
            else if((a) > 10 ) (a) = ((a)/10)  * 10; \
            else if((a) > 5  ) (a) = 5;              \
            else a = 1;                              \
         } while (ZERO)
/*
 * A bi-linear interpolation: get the value at (r1+dx,c1+dy).
 * r,c indicates the row and column
 * po     : the output
 * (r1,c1): the approximate grid point           O(r2,c1)     O(r2,c2)
 * (r2,c2): the approximate grid point
 * (dr,dc): delta-x and -y from the grid point           o(r1+dr,c1+dc)
 * drc    : dr*dc;                               O(r1,c1)     O(r1,c2)
 * pc     : source
 *
 */
#define GETFRAC(po,r1,c1,r2,c2,dr,dc,drc,pc,min,max)         \
       do {                                                  \
          register float pdc_, pdr_, pf_, f_;                \
          register int itmp_;                                \
          f_   = (float) pc[r1][c1];                         \
          pdr_ = (float) pc[r2][c1] - f_;                    \
          pdc_ = (float) pc[r1][c2] - f_;                    \
          pf_  = (float) pc[r2][c2] - f_;                    \
          f_ += pdc_ * (dc) + pdr_ * (dr) +                  \
                (pf_ - pdc_ - pdr_) * (drc);                 \
          itmp_ = (f_ + 0.5);                                \
          if(itmp_ > max) itmp_ = max;                       \
          if(itmp_ < min) itmp_ = min;                       \
          (po) = itmp_;                                      \
        } while (ZERO)

#endif

