/*****************************************************************************
 image_new.h    S.R.Lines  (spraxlo@{ai|media}.mit.edu)     93.6.17-7.15
 *****************************************************************************/

image_t *image_new(int width,int height,int type,void *extra1,void *extra2);
image_t	*image_create(int width,int height,int type,char **image,void *extra1,void *extra2);
image_t *image_no_mem(int width,int height,int type);
image_t *image_wrap(int width,int height,int type,char **imp,void *extra1,void *extra2);

void	 image_realloc_mem (image_t *im);  /* void; exits on failure */
void	 image_free ( image_t *image );

