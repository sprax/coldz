/*****************************************************************************
 img_util.c -- for Sarnoff libimg.    S.R.Lines    93.6.28
 *****************************************************************************/
#include <stdlib.h>
#include <string.h>
#include "imgdec.h"
//#include "img_util.h"
extern int Image_border;

/*
#include <stdio.h>
#define Err(ss,ii) {fprintf(stderr,"Err: %s %d\n",ss,ii);exit(-1);}
*/

FIMAGE *fimg_new(int wide, int tall)
{
  FIMAGE *new = (FIMAGE*)malloc(sizeof(FIMAGE));
  new->w = wide;
  new->h = tall;
  alloc_fimage(new);
  return new;
}

FIMAGE *fimgdup(FIMAGE *src)	/* duplicate image AND border */
{
  void *sv, *nv;  size_t size;
  FIMAGE  *new = (FIMAGE*)malloc(sizeof(FIMAGE));
  new->w = src->w;
  new->h = src->h;
  alloc_fimage(new);

#if 1
  sv = *(src->ptr - Image_border) - Image_border;
  nv = *(new->ptr - Image_border) - Image_border;
  size = sizeof(float)*(new->w + 2*Image_border)*(new->h + 2*Image_border);
  memcpy(nv,sv,size);
#else
  fcopy_img(*src,*new);
  reflect(*new,Image_border,1);
#endif

  return new; 
}


/*******************
typedef struct {
   float **ptr;
   int w;
   int h;
} FIMAGE;
********************/

void print_fimage(FIMAGE *ip, char *name)
{
  fprintf(stderr,"print_fimage(ip==%ld, name==%s)\n",(long)ip,name);
  fprintf(stderr,"w, h:    %d  %d\n"       ,ip->w,ip->h);
  fprintf(stderr,"  ptr:   %ld\n"          ,(long)  ip->ptr);
  fprintf(stderr," *ptr:   %ld\n"          ,(long) *ip->ptr);
  fprintf(stderr,"**ptr:   %.1g\n"         ,(double) **ip->ptr);
}


