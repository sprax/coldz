/**** aaMacro.h
      S.R. Lines
      25 Mar 92 (Orig. Jan 92) ******/

#ifndef  M_PI
#define  M_PI   (3.14159265358979323846)
#define	 M_PI_2 (1.57079632679489661923)
#define	 M_PI_4 (0.78539816339744830966)
#define	 M_E    (2.71828182845904523536)
#endif 
#define  M_PI_8 (0.39269908169872415480)
#define  TWO_PI  (6.28318530717958647692)
#define  INVROOT2PI  0.398942280401432678   /* 1.0/sqrt(2 * M_PI) */


/*       Directions: When only 2 directions are possible, WEST==NORTH==1 */
#ifndef  EAST        /* These should agree with MoverShaker.h */
#define  EAST  0     /* Left to Right, 1st quadrant, x:y = pos:neg  */
#define  WEST  2     /* Right to Left, 3rd quadrant, x:y = neg:pos  */
#define  SOUTH 1     /* Top to Bottom, 4th quadrant, x:y = pos:pos  */
#define  NORTH 3     /* Bottom to Top, 2nd quadrant, x:y = neg:neg  */
#endif 

/*          8 compass directions returned by DIRECTION */
#define  EE 0    /* Arrays nx[8] hold values for the 8    */
#define  SE 1    /* directions around center pixel c      */
#define  SS 2    /* like:   5 6 7                         */
#define  SW 3    /*         4 c 0                         */
#define  WW 4    /*         3 2 1                         */
#define  NW 5    
#define  NN 6    
#define  NE 7    

#ifndef  SWAP
#define  SWAP(A,B,T) (T = A, A = B, B = T)
#endif
#ifndef  SQR
#define  SQR(A,T)  (T = (A), T*T)
#endif

#define  CUBEROOT_INT(k)  ((int)(pow((double)(k),(double)(1.0/3.0))))
#define  CUBE_INT(k)      (k*k*k)
#define  SQRT_INT(k)      ((int)(sqrt((double)(k))))
#define  SQR_INT(k)       (k*k)


                    /*_____ THRESHOLDING & CLIPPING _____*/
#define  THRESH(a,t)  (((a) > (t)) ? (a) : 0)
#ifndef  RANGE
#define  RANGE(x,lo,hi)  (x=MAX(x,lo),x=MIN(x,hi))
#endif
                    /*___________ SIGNS & DIRECTIONS_____________*/
#define  NONNEG(a)  (((a) < 0) ? 0 : 1)
 
                    /**** QUADRANTS agrees with cardinal directions as follows:
                     W=2  N=3  -- Dividing the 8 compass directions by 2 gives 
                     S=1  E=0     these quadrants.                        ****/ 
#define  QUADRANT(x,y,q)  ({if (y<0) { if (x<0) q=WW;  else q=SS;  }   \
                            else     { if (x<0) q=NN;  else q=EE;  }}, q)
 
 
                    /**** Directions returned by DIRECTION() and direction() are these:
                     5  6  7     Some data arrays & functions depend on this scheme
                     4 -1  0     implicitly, and changing it would wreck array references,
                     3  2  1     etc., which would be hard to debug.  Take care.  ****/
#define  DIRECTION(x,y,d)  /* Same as direction, except for implicit type conv. */ \
                (((x)||(y))?(d=(int)(atan2((double)(y),(double)(x))/M_PI_8),       \
                 (d<0)?((d+16)/2):((d+1)/2)):(-1))

                     /*_____ DISTANCES ________*/
#define  ABS_DIFF(A,B)     ((A > B) ? (A - B) : (B - A))    /* Use if A & B are atomic */
#define  ABS_DIFF_T(A,B,T)  (T = A - B, ABS(T))  /* Use if A or B not atomic, or inside macros */
 
#define  DIST_TAXI(x1,y1,x2,y2)  (DIFF_ABS(x1,x2) + DIFF_ABS(y1,y2))
 
#define  DIST_TILE(x1,y1,x2,y2,T1,T2)  /** T1 & T2 are temp variables */  \
         (T1 = ABS_DIFF_T(x1,x2,T1), T2 = ABS_DIFF_T(y1,y2,T2), MAX(T1,T2))
 
#define  DIFF_SQUARED(A,B,T) (T = A - B, T*=T)  /** T is a temp variable **/
 
#define  DIST_EUCLID(x1,y1,x2,y2,T1,T2)         /** T1 & T2 are temp variables */ \
	 (T1 = x1-x2, T2 = T1*T1, T1 = y1-y2, T2 += T1*T1, (T2 ? sqrt((double)T2) :  0.0)) 
 
                     /*____ NEIGHBORS AND EDGES ____*/
                /**** Light neighbors && dark interiors --> edge ****/
#define  LNEIGHBOR_GRAY(p,t,c)  (*((p)+1) > t||*((p)-1) > t||*((p)-c) > t||*((p)+c) > t)
#define  THIN_EDGE_GRAY(p,t,c)   (*(p) <= t && LNEIGHBOR_GRAY(p,t,c))

#define  LNEIGHBOR_RGBI(p,t,c3)                   /* check only on 4 sides. */\
         (INTENS(p+3) > t || INTENS(p-3) > t || INTENS(p+c3) > t || INTENS(p-c3) > t)  
#define  THIN_EDGE_RGBI(p,t,c3)   ((INTENS(p) <= t) && LNEIGHBOR_RGBI(p,t,c3))



 
#define  LoadImageRGBI(path,wide,tall)  LoadImageRaw(path,wide,tall,3)
#ifndef  HP
#define  Compress_Check(p)  (p)
#endif

