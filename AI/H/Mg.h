/*****************************************************************************
 Mg.h -- iMaGe.   S.R.Lines  (spraxlo@{ai|media}.mit.edu)    93.6.10-7.30
 *****************************************************************************/
#ifndef  Mg_h__
#define  Mg_h__

#include "unTypes.h"
#include "unLib.h"

typedef struct Mf { 
	pfl	*row;	/* ptr to an array of pointers to each line.	*/
	pfl	 mem;	/* pointer to first byte of 1-D array = *row. 	*/
	int	xloc;	/* coord of  leftmost stored pixel, < 0 if brdr	*/
	int	yloc;	/* coord of rightmost stored pixel, < 0 if brdr	*/
	int	wide;	/* width in pixels ( may != scan )		*/
	int	tall;	/* width & height in pixels			*/
	int	jump;	/* ptr jump to next scan line, usu. row[1]-row[0]*/
} Mf, *pMf;	typedef const Mf *pcMf;


typedef struct Mu { 
	puc	*row;	/* ptr to an array of pointers to each line.	*/
	puc	 mem;	/* pointer to first byte of 1-D array = *row. 	*/
	int	xloc;	/* coord of  leftmost stored pixel, < 0 if brdr	*/
	int	yloc;	/* coord of rightmost stored pixel, < 0 if brdr	*/
	int	wide;	/* width in pixels ( may != scan )		*/
	int	tall;	/* width & height in pixels			*/
	int	jump;	/* ptr jump to next scan line, usu. row[1]-row[0]*/
} Mu, *pMu;	typedef const Mu *pcMu;


typedef struct Mg { 
	puc	pmem;	/* pointer to first byte of 1-D array.  */
	puc    *prow;	/* pointer to an array of pointers to each line.  */
	int	wide;	/* width in pixels ( may != scan )	*/
	int	tall;	/* width & height in pixels     */
	int	chan;	/* usually == bytes per pixel. */
	int	bpch;	/* bits per channel, usually 8. */
	size_t	scan;	/* bytes per scan line. */
	size_t	size;	/* usu. (size_t)wide*tall*nChan         */
	unt	mmss;	/* Memory storage structure */
	unt	ffmt;	/* For file type & status info (ad hoc) */
	char   *path;	/* ptr to path string for fopen (system specific) */
	char   *name;	/* ptr to name for window title, aux file path, etc. */
	FILE   *file;	/* pointer to an (open) file struct. */
        void   *wild;	/* wildcard ptr: for list of points, etc */
} Mg, *pMg;		/* NB: (const Mg *p) != (pcMg p)==(Mg *const p) */
typedef const  Mg *pcMg;	/* ptr to const Mg */
typedef Mg *const  cpMg;	/* const ptr to Mg */
typedef const Mg  *const cpcMg;	/* const ptr to const Mg -- not so useful. */

#define Mg_NEW    1
#define Mg_COPY   2
#define Mg_NULL   4
#define Mg_CLONE  8
#define Mg_DIMS   16
#define Mg_VARG   32
#define Mg_FILE   64
#define Mg_NAME   (Mg_NEW | 128)
#define Mg_FULL   (Mg_NEW | 256)
#define Mg_REAL   512
#define Mg_ZERO   1024


#define Mg_DEFWIDE  512
#define Mg_DEFTALL  512
#define Mg_DEFCHAN    1
#define Mg_DEFBPCH    8
#define Mg_DEFMMSS    0
#define Mg_DEFFFMT    0
#define Mg_DEFPATH    NULL
#define Mg_DEFNAME    NULL
#define Mg_DEFFILE    NULL


#define MgPtr()     ((pMg)malloc(  sizeof(Mg)))   /* parens for func. subs. */
#define MgZero()    ((pMg)calloc(1,sizeof(Mg)))
#define MgCopy(mg)  MgClone(mg,Mg_COPY)
#define MgNull(mg)  MgClone(mg,Mg_NULL)    /* Should rarely be used. */

#define Mg_PRIMARY(mg)  (!(mg->mmss & MM_SUB))

#ifndef	MGERR
#define	MGERR  1
#endif
#if MGERR		/**** Provisional Kludge ****/
#define MgErr(ss,ii) {fprintf(stderr,"MgErr: %s: %d\n",ss,ii);exit(-1);}
#else
#define MgErr(ss,ii) exit(-1);
#endif


#ifndef  _NO_PROTO

pMg MgNew(int wide, int tall, int chan, int bpch, unt mmss);
pMg MgClone(pMg src, unt mode);
pMg MgFull(puc pmem, int wide, int tall, int chan, int bpch, unt mmss, unt ffmt, pch path, char *name, FILE *file);
pMg MgPath(char *path);
pMg MgRealloc(pMg src, int wide, int tall, int chan, int bpch, unt mmss);
void MgFree(pMg mg);
void MgPrint(pcMg q);

#else   /*   ifndef _NO_PROTO */

extern  int  MgMeanValue ();

#endif  /* _NO_PROTO */

#endif  /* Mg_h__ */


