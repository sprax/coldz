/*****************************************************************************
 beymer/image.h -- David Beymer's image structure.    S.R.Lines    -93.5.25
	Note: a copy of all contents below exists in beymage.h,
        albeit with "image" globally replaced by "beymage".
 *****************************************************************************/
#ifndef	beymer_image_h_
#define	beymer_image_h_


typedef struct image_t {	/* spraxlo changed tag from image to image_t */
  int	width, height;		/* @@ padded or pixel width?! */
  int	type;			/* one of the above types */
  int	bytes_per_line;		/* jump to next 32-bit padded line */
  char	**image;		/* points to an array of row pointers */
  char	*mem;			/* pointer to raw memory block */
  void	*extra1, *extra2;	/* extra vars for non library routines */
} image_t;

typedef unsigned char  uchar;
typedef unsigned short ushrt;

	/* types for image_t:   */
#define IMG_1BIT		1
#define IMG_U8BIT		0
#define IMG_S8BIT		2
#define IMG_S32BIT		4
#define IMG_FLOAT		8
#define IMG_RGBFLT		16
#define IMG_RGBUNC		32
#define IMG_TYPE_MASK		63
#define IMG_TYPE_MASK_RGB	48

#define IM_BITS(type) ((type==IMG_FLOAT) ? 32 : (type != IMG_1BIT) ? 8:1)

#define image_ptr() ((image_t *)malloc(sizeof(image_t)))

/*********************************************************
The following magic numbers for foreign image file headers 
were moved to imagic.h, given nicer names, etc.:
#define GROK_MAGIC_NUMBER	1
#define DUMP_MAGIC_NUMBER	32
#define RAS_MAGIC_NUMBER        0x59a66a95
#define PBM_MAGIC_NUMBER        "P1"
#define PGM_MAGIC_NUMBER        "P2"
#define VIS_MAGIC_NUMBER        "VISF="
**********************************************************/
/*
 *	Boundary connditions for images:
 */
#define BOUNDARY_REFLECT			0
#define BOUNDARY_WRAP				1

#include "beymer/alloc.h"           /* kludge? */
#include "beymer/image_new.h"       /* kludge? */

#endif	/* beymer_image_h_ */


