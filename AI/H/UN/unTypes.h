/*****************************************************************************
 unTypes.h    S.R.Lines  (spraxlo@{ai|media}, srl@athena)    93.6.25-7.17
 To ensure that an int is 32-bits, #include "int32bit.h"
 As of 93.6.5, this file is transitional: Equivalent typedefs with different
 names will evenually disappear; preferred identifiers precede future losers.
 Strict left-branchng will probably win over 4-lwd convention, but the newer
 3-lwd's will probably stay for convenience.
 The main thing is to delete the BAD and MISNOMER.
 *****************************************************************************/
#ifndef  unTypes_h__
#define  unTypes_h__

#ifndef	MAC
#include	<sys/types.h>	/* Avoid conflicts & redefs */
#endif

#if 0     /* Some misnomers: these will be the first to go */
typedef        unsigned char                          *ucp; /* MISNOMER */
typedef  const unsigned char                         *cucp; /* MISNOMER */
#define  rpuc  register puc                                 /* MISNOMER */
#endif

/*NAMES:                FULL  GOOD    BAD     SHORT    LONG     */
typedef                 char  chr,           *pch,   *pchr;
typedef        unsigned char  unc,   uch,    *puc,   *punc, *puch, **ppu;
typedef  const unsigned char  cunc,  cuch,   *pcuc,  *pcunc;
typedef  unsigned char *const cpuc,   cpunc;

typedef                 short shr,   sht,    *psh,   *psht, **pps;
typedef  const          short cshr,  csht,   *pcsh,  *pcshr, *pcshrt;
typedef        unsigned short ush,  ushr,    *pus,   pushr, **ppus, **pph;

typedef                 int                  *pint,     pin;
typedef  const          int    cint,         *pcint,   *pcnt;
typedef        unsigned int    uni,   unt,   *puni,   *punt;

typedef                 long   lng,          *plng;
typedef        unsigned long   unl,          *punl;

typedef                 float  flt,          *pfl,   *pflt, **ppf;
typedef  const          float  cflt,         *pcfl,  *pcflt;

typedef                 double dbl,          *pdbl,    *pdl;

typedef                 void                 *pvd,    **ppv;

#define  rgst   register
#define  rgs    register
#define  regint register int
#define  regflt register flt
#define  rflt	register flt
#define  rpuc   register puc
#define  rpfl   register pfl

#if 00
#define  rgsint register int
#define  rpcuc  register pcuc       /* (register const unsigned char *) */
#define  rpcflt register pcflt      /* (register const unsigned char *) */
#endif


#ifdef	stdin                /* infer that <stdio.h> has been included */
typedef FILE *pFILE;
#endif

        /*_____________ TYPES: */
#define UNSIGNED_        1
#define POSITIVE_        2       /* Strictly > 0 */
#define PTR_	         4
#define PTR_PTR_         8
#define  CHAR_           16
#define	 CHAR_PTR_       (CHAR_ | PTR_ )
#define UCHAR_           (UNSIGNED | CHAR)
#define  SHORT_          32
#define USHORT_          (UNSIGNED | SHORT)
#define  INT_            64
#define UINT_            (UNSIGNED | INT)
#define  LONG_           128
#define ULONG_           (UNSIGNED | LONG)

#define FLOAT_           256
#define DOUBLE_          512
#define STRING_          1024
#define STR256_          2048
#define OPTSTR_          4096
#define LDOUBLE_         8192
#define COMPLEX_         16384

#endif  /* unTypes_h__ */


