/*****************************************************************************
 unFlags.h    S.R.Lines  (spraxlo@{ai|media}, srl@athena)    93.7.19-8.18
 *****************************************************************************/
#ifndef unFlags_h__
#define unFlags_h__

extern unsigned long Options, Devopts;

void addUnFlag (unsigned long *options, int letter);
void addUnFlags (unsigned long *opts, unsigned long *devo, char *argstr);

#define O_NONE  0

#define O_VERB  1                /* 0x000000001 */
#define O_WARN  2                /* 0x000000002 */
#define O_HELP  4                /* 0x000000004 */
#define O_TEST  8                /* 0x000000008 */

#define O_XGUI  16               /* 0x000000010 */
#define O_BACK  32               /* 0x000000020 */
#define O_CENT  64               /* 0x000000040 */
#define O_FWRD	128              /* 0x000000080 */

#define O_NERN  256              /* 0x000000100 Nearest Neighbor */
#define O_SLIN	O_NERN           /* synonym */
#define O_BLNR  512              /* 0x000000200 */
#define O_BQDR  1024             /* 0x000000400 */
#define O_BCBC	2048             /* 0x000000800 */

#define O_LBLS	4096             /* 0x000001000 */

#define O_FLOT	4096             /* 0x000001000 */
#define O_UNCH	8192             /* 0x000002000 */
#define	O_HITS	16384            /* 0x000004000 */
#define	O_HOLE	32768            /* 0x000008000 */

#define O_CLIP  65536            /* 0x000010000 */
#define O_STAT  131072           /* 0x000020000 */
#define O_COMB  262144           /* 0x000040000 */
#define O_EXTR  524288           /* 0x000080000 */

#define O_NONO  1048576          /* 0x000100000 */
#define O_GRAY  2097152          /* 0x000200000 */
#define O_GREY  O_GRAY
#define O_WIDE  4194304          /* 0x000400000 */
#define O_MORE  8388608          /* 0x000800000 */

#define O_PTRS  16777216         /* 0x001000000 */
#define O_READ  33554432         /* 0x002000000 */
#define O_WRIT  67108864         /* 0x004000000 */
#define O_TIME	134217728        /* 0x008000000 */

#define O_ONLY	268435456        /* 0x010000000 */
#define O_BOTH	536870912        /* 0x020000000 */
#define O_QUIE	1073741824       /* 0x040000000 */
#define O_KLUG	2147483648U      /* 0x080000000 */
#define O_KLUDGE O_KLUG

#define ICCV     (Devopts & O_UNCH)

#if 0				 /* Too Big:	*/
#define O_LAST	4294967296U      /* 0x100000000 2^32*/
#endif	/* Value does not fit in 32 bits: truncated */

#define O_HORZ	O_WIDE

#endif	/* unFlags_h__ */

/************
2^32:	4294967296
2^33:	8589934592
2^34:	17179869184
2^35:	34359738368
2^36:	68719476736
2^37:	137438953472
2^38:	274877906944
2^39:	549755813888
2^40:	1099511627776
2^41:	2199023255552
2^42:	4398046511104
2^43:	8796093022208
2^44:	17592186044416
2^45:	35184372088832
2^46:	70368744177664
2^47:	140737488355328
2^48:	281474976710656
2^49:	562949953421312
2^50:	1125899906842624
2^51:	2251799813685248
2^52:	4503599627370496
2^53:	9007199254740992
2^54:	18014398509481984
2^55:	36028797018963968
2^56:	72057594037927936
2^57:	144115188075855872
2^58:	288230376151711744
2^59:	576460752303423488
2^60:	1152921504606846976
2^61:	2305843009213693952
2^62:	4611686018427387904
2^63:	9223372036854775808
2^64:	18446744073709551620 WRONG!
*****************************************/


