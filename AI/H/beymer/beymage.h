/*****************************************************************************
 beymage.h    David Beymer, S.R.Lines    93.5.22
 --contents below this header are copied from David Beymer's image.h, with
   beymage globally substituted for image.
 *****************************************************************************/
#ifndef	_libvision_beymage_h
#define	_libvision_beymage_h

#include <sys/types.h>

/*
 * beymage types
 */

#define IMG_1BIT		0
#define IMG_U8BIT		1
#define IMG_S8BIT		2
#define IMG_S32BIT		3
#define IMG_FLOAT		4

/*
 * boundary connditions for beymages
 */

#define BOUNDARY_REFLECT			0
#define BOUNDARY_WRAP				1

/*
 * basic beymage representation
 */

typedef struct beymage {
  int width, height;
  int type;			/* one of the above types */
  int bytes_per_line;		/* bytes to next 32-bit padded line. */
  char **beymage;		/* points to an array of row pointers */
  char *mem;			/* pointer to raw memory block */
  void *extra1, *extra2;	/* extra vars for non library routines */
} beymage_t;

/* for accessing binary beymages
 */

#define getBit(line,col)	(((*(line+((col)>>3)))>>(7-((col)&7)))&1)
#define setBit(line,col)	*(line+((col)>>3)) = *(line+((col)>>3))|(1<<(7-((col)&7)))
#define clrBit(line,col)	*(line+((col)>>3)) = *(line+((col)>>3))&~(1<<(7-((col)&7)))

/* function declarations */
#ifndef _NO_PROTO
beymage_t *allocate_beymage();
beymage_t *allocate_beymage_like(beymage_t *img);
beymage_t *copy_beymage();
beymage_t *read_beymage();
beymage_t *read_grok();
beymage_t *read_dump();
beymage_t *read_raw();
beymage_t *read_rast();
beymage_t *read_vis();
beymage_t *read_jlm();
beymage_t *read_pix (char *filename);
beymage_t *extract_center();
void	allocate_line_pointers (beymage_t *beymage);
float	bilinear_interpolate ( beymage_t *beymage, double x, double y );
#else
beymage_t *allocate_beymage();
beymage_t *allocate_beymage_like();
beymage_t *copy_beymage();
beymage_t *read_beymage();
beymage_t *read_grok();
beymage_t *read_dump();
beymage_t *read_raw();
beymage_t *read_rast();
beymage_t *read_vis();
beymage_t *read_jlm();
beymage_t *extract_center();
beymage_t *read_pix ();
void	allocate_line_pointers ();
float	bilinear_interpolate ();
#endif


char *read_beymage_dir();
double beymage_percentile();

#endif	/* !_libvision_beymage_h */


