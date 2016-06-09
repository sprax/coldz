/*****************************************************************************
 unTime.h    S.R.Lines  (spraxlo@{ai|media}, srl@athena)    93.6.27
 *****************************************************************************/
#ifndef	unTime_h_inc
#define	unTime_h_inc

#define STAMP_FTD(ss)   fprintf(stderr,ss,__FILE__,__TIME__,__DATE__)


#ifndef	UNTIME
#define	UNTIME 0
#endif
#if	UNTIME

#include <time.h>

#ifndef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC 60
#endif

static	time_t unTold,unTnew,unTdif;

#define	BEGCLOCK(zz) { if(zz[0]) fprintf(stderr,"_____BEGIN %s\n",zz); unTold = clock(); }
#define	ENDCLOCK(zz) { unTnew = clock();\
                       unTdif = unTnew - unTold;\
                       fprintf(stderr,"_____END__ %s: %.4f\n",(zz ? zz:"clock sec"),\
                       (double)unTdif/CLOCKS_PER_SEC); }

#else

#define	BEGCLOCK(zz)  ;
#define	ENDCLOCK(zz)  ;

#endif	/* TIMING */

#endif	/* unTime_h_inc */


