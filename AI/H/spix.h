/*********************************************************************
 spix.h    S.R.Lines    93-04-23
 To replace some functionality of Sun libpixrect.a on SGI.
 *********************************************************************/
 
#ifndef  spix_h__
#define  spix_h__

#include <pixrect/pixrect.h>	/* find conflicts right away */
#include <pixrect/pr_io.h>	/* find conflicts right away */
#include <pixrect/pixfont.h>	/* find conflicts right away */

#ifndef	_NO_PROTO

	/* types of functions pointed to by members of pixrectops: */
typedef int pr_rop_fn ( Pixrect *dpr, int dx, int dy, int w, int h, int op,
		       Pixrect *spr, int sx, int sy );
typedef int pr_stencil_fn ( Pixrect *dpr, int dx, int dy, int w, int h, int op,
               Pixrect *stpr, int stx, int sty, Pixrect *spr, int sx, int sy );
typedef int pr_batchrop_fn ( Pixrect *dpr, int x, int y,int op,int sbp,int n );
typedef int pr_nop_fn ( Pixrect *pr, int cmd, int data );
typedef int pr_destroy_fn ( Pixrect *pr );
typedef int pr_get_fn ( Pixrect *pr, int x, int y );
typedef int pr_put_fn ( Pixrect *pr, int x, int y, int val );
typedef int pr_vector_fn ( Pixrect *pr, int x0, int y0,
		      int x1, int y1, int op, int color );
typedef Pixrect *pr_region_fn ( Pixrect *pr, int x, int y, int w, int h );
typedef int pr_putcolormap_fn ( Pixrect *pr, int ind, int cnt,
                                int red, int grn, int blu );
typedef int pr_getcolormap_fn ( Pixrect *pr, int ind, int cnt,
                                int red, int grn, int blu );
typedef int pr_putattributes_fn ( Pixrect *pr, int planes );
typedef int pr_getattributes_fn ( Pixrect *pr, int planes );
typedef int pr_ioctl_fn ( Pixrect *pr, int cmd, int data );

	/* Functions defined in spix.c */
extern int	pr_dump ( Pixrect *input_pr, FILE *output, colormap_t *colormap,
                     int type, int copy_flag );
extern Pixrect*	pr_load ( FILE *input, colormap_t *colormap );
extern int	pf_close ( Pixfont *pf );
extern Pixfont*	pf_open ( char *name );
extern int	pf_textbound ( struct pr_subregion *bound, int len,
                          Pixfont *font, char *text );
extern int	pr_text ( Pixrect *pr, int x, int y, int op,
                     Pixfont *font, char *text );
extern int	pr_ttext (Pixrect *pr, int x, int y, int op,
                     Pixfont *font, char *text );
extern Pixrect*	mem_create(int w, int h, int depth);
extern void	pr_polypoint(Pixrect *dpr, int dx, int dy, int npts, struct pr_pos *ptlist, int op);
extern int	pr_dump(Pixrect *input_pr, FILE *output, colormap_t *colormap, int type, int copy_flag);
extern void	print_rasterfile_h(struct rasterfile *rfhp);


/***********************************
 NB: pr_close is #defined as pr_destroy, and pr_destroy(pr) is #defined as:
	(*(pr)->pr_ops->pro_destroy)(pr)
void pr_destroy(Pixrect *pr);
************************************/


#else	/* _NO_PROTO is defined */


typedef int pr_rop_fn ();
typedef int pr_stencil_fn ();
typedef int pr_batchrop_fn ();
typedef int pr_nop_fn ();
typedef int pr_destroy_fn ();
typedef int pr_get_fn ();
typedef int pr_put_fn ();
typedef int pr_vector_fn ();
typedef Pixrect *pr_region_fn ();
typedef int pr_putcolormap_fn ();
typedef int pr_getcolormap_fn ();
typedef int pr_putattributes_fn ();
typedef int pr_getattributes_fn ();
typedef int pr_ioctl_fn ();

#endif	/* _NO_PROTO */

#endif	/* spix_h__ */



