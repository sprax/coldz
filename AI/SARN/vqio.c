/* VQ I/O routines */

#include <stdio.h>

get_vqt_header(stream,nsym,dim,xsize,ysize)
FILE *stream;
int *nsym,*dim,*xsize,*ysize;
{
   fread(nsym,sizeof(nsym),1,stream);
   fread(dim,sizeof(dim),1,stream);
   fread(xsize,sizeof(xsize),1,stream);
   fread(ysize,sizeof(ysize),1,stream);
}

put_vqt_header(stream,nsym,dim,xsize,ysize)
FILE *stream;
int nsym,dim,xsize,ysize;
{
   fwrite(&nsym,sizeof(nsym),1,stream);
   fwrite(&dim,sizeof(dim),1,stream);
   fwrite(&xsize,sizeof(xsize),1,stream);
   fwrite(&ysize,sizeof(ysize),1,stream);
}



put_vqc_header(stream,nsym,dim,xsize,ysize)
FILE *stream;
int nsym,dim,xsize,ysize;
{
   fprintf(stream,"nsym=%d  dim=%d  xsize=%d  ysize=%d\n",nsym,dim,xsize,ysize);
}

get_vqc_header(stream,nsym,dim,xsize,ysize)
FILE *stream;
int *nsym,*dim,*xsize,*ysize;
{
   fscanf(stream,"nsym=%d  dim=%d  xsize=%d  ysize=%d",nsym,dim,xsize,ysize);
}

get_codebook(stream,codebook,nsym,dim)
FILE *stream;
float **codebook;
int nsym,dim;
{
   float **fin;
   for (fin = codebook+nsym; codebook < fin; codebook++)
     get_fcodeword(stream,*codebook,dim);
}

put_codebook(stream,codebook,nsym,dim)
FILE *stream;
float **codebook;
int nsym,dim;
{
   float **fin;
   for (fin = codebook+nsym; codebook < fin; codebook++)
     put_fcodeword(stream,*codebook,dim);
}




get_fcodeword(stream,x,dim)
FILE *stream;
float *x;
int dim;
{
  register float *fin;

  for (fin=x+dim; x < fin; x++)
    fscanf(stream,"%f",x);
}

put_fcodeword(stream,x,dim) 
FILE *stream;
float *x;
int dim;
{
  register float *fin;

  while (dim > 8) {
    for (fin=x+8; x < fin; x++)
      fprintf(stream,"%9.3f ",*x);
    fprintf(stream,"\n");
    dim -= 8;
  }
  for (fin=x+dim; x < fin; x++)
    fprintf(stream,"%9.3f ",*x);
  fprintf(stream,"\n");
}

get_scodeword(stream,x,dim)
FILE *stream;
short *x;
int dim;
{
  register short *fin;

  for (fin=x+dim; x < fin; x++)
    fscanf(stream,"%f",x);
}

put_scodeword(stream,x,dim) 
FILE *stream;
short *x;
int dim;
{
  register short *fin;

  while (dim > 8) {
    for (fin=x+8; x < fin; x++)
      fprintf(stream,"%9.3f ",*x);
    fprintf(stream,"\n");
    dim -= 8;
  }
  for (fin=x+dim; x < fin; x++)
    fprintf(stream,"%9.3f ",*x);
  fprintf(stream,"\n");
}

