/*****************************************************************************
 SgTg.h -- Images & vector correspondences.    S.R.Lines    93.6.27
 *****************************************************************************/
#ifndef SgTg_h_
#define SgTg_h_

#include	"unTypes.h"

typedef	struct	Sg {	/* Two images and one-way vector deltas.        */
	int 	 wide;	/* pixels wide = number of elts in a row        */
	int	 tall;	/* pixels tall = number of row pointers         */
	int	 marg;	/* margin or border: may only exist for vx & vy */
	puc	*srcA;	/* src image A (stored as tall row ptrs)        */
	puc	*srcB;	/* src image B (stored as tall row ptrs)        */
	pfl	*vdxA;	/* X-delta vector from src A to src B           */
	pfl	*vdyA;	/* Y-delta vector from A to B                   */
	pfl	*vcfA;	/* XY-Confidences in coord frame of A           */
} Sg, *pSg;

typedef	struct	Tg {	/* Two images and two-way vector deltas. */
	int 	 wide;	/* pixels wide = number of elts in a row */
	int	 tall;	/* pixels tall = number of row pointers  */
	int	 marg;	/* margin or border: may only exist for vx & vy */
	puc	*srcA;	/* src image A stored as tall ptrs to rows */
	puc	*srcB;	/* src image B stored as tall ptrs to rows */
	pfl	*vdxA;  /* X-vector delta from srcA to srcB (row ptrs) */
	pfl	*vdyA;  /* Y-vector delta from srcA to srcB */
	pfl	*vcfA;  /* XY-Confidences from srcA to srcB */
	pfl	*vdxB;  /* X-vector delta from srcB to srcA */
	pfl	*vdyB;  /* Y-vector delta from srcB to srcA */
	pfl	*vcfB;  /* XY-Confidences from srcB to srcA */
} Tg, *pTg;


typedef	struct	SSg {	/* Multiple im srcs with one-way deltas from srcs[0] */
	int	   num;	/* Number of srcs, etc. */
	int 	  wide;	/* pixels wide = number of elts in a row */
	int	  tall;	/* pixels tall = number of row pointers  */
	int	  marg;	/* margin or border: may only exist for vx & vy */
	puc	**srcs;	/* Ptr to array of n src images stored as rows */
	pfl	**vdxA;	/* Ptr to n X-deltas from src[0] to src[j] as rows */
	pfl	**vdyA;	/* Ptr to n Y-deltas from src[0] to src[j] as rows */
	pfl	**vcfA;	/* Ptr to n Confidences from src[0] to src[j] as rows */
} SSg, *pSSg;

typedef	struct	TTg {	/* Multiple im srcs with two-way deltas from srcs[0] */
	int	   num;	/* Number of srcs, etc. */
	int 	  wide;	/* pixels wide = number of elts in a row */
	int	  tall;	/* pixels tall = number of row pointers  */
	int	  marg;	/* margin or border: may only exist for vx & vy */
	puc	**srcs;	/* Ptr to array of n src images stored as rows */
	pfl	**vdxA;	/* Ptr to n X-deltas from src[0] to src[j] as rows */
	pfl	**vdyA;	/* Ptr to n Y-deltas from src[0] to src[j] as rows */
	pfl	**vcfA;	/* Ptr to n Confidences from src[0] to src[j] as rows */
	pfl	**vdxB;	/* Ptr to n X-deltas from src[j] to src[0] as rows */
	pfl	**vdyB;	/* Ptr to n Y-deltas from src[j] to src[0] as rows */
	pfl	**vcfB;	/* Ptr to n Confidences from src[j] to src[0] as rows */
} TTg, *pTTg;



#if 0000	/**** Vector fields w/o src images: Never used. ****/

typedef	struct	Sxyc {	/* Just one-way vector deltas. */
	int 	 wide;	/* pixels wide = number of elts in a row        */
	int	 tall;	/* pixels tall = number of row pointers         */
	int	 marg;	/* margin or border: may only exist for vx & vy */
	pfl	*vdxA;	/* X-delta vector from some src A to some dst B */
	pfl	*vdyA;	/* Y-delta vector from A to B                   */
	pfl	*vcfA;	/* XY-Confidences in coord frame of A           */
} Sxyc, *pSxyc;

typedef	struct	Txyc {	/* Just two-way vector deltas. */
	int 	 wide;	/* pixels wide = number of elts in a row        */
	int	 tall;	/* pixels tall = number of row pointers         */
	int	 marg;	/* margin or border: may only exist for vx & vy */
	pfl	*vdxA;	/* X-delta vector from some src A to some dst B */
	pfl	*vdyA;	/* Y-delta vector from A to B                   */
	pfl	*vcfA;	/* XY-Confidences in coord frame of A           */
	pfl	*vdxB;	/* X-delta vector from some src B to some dst A */
	pfl	*vdyB;	/* Y-delta vector from B to A                   */
	pfl	*vcfB;	/* XY-Confidences in coord frame of B           */
} Txyc, *pTxyc;

#endif	/* 0000 */

#endif	/* SgTg_h_ */

