/* Short kernel filters */

#include "imgdec.h"
#include "imgmacro.h"

extern int Image_border;

kernel(in,out,fs)
FIMAGE in,out;
char *fs;
{
  register float **p=in.ptr,**q=out.ptr;
  float val;
  register int i,j,n;

  if (!strcmp(fs,"help")) {			/* help */ 
    fprintf(stderr,
       "Filters are:\n\
          h1    .5  .5\n\
          h2    -1   1 \n\
          h3    .25 .5 .25 \n\
          h4    -1   2  -1 \n\
          h5    -1   0   1 \n\
          h6    .5   0  .5 \n\
        similarly for v1...v5 (vertical); r1...r5 (right diag) \n\
        and l1...l5 (left diag) \n\
        Extra filters are: \n\
          x1  0 -1  0       x2    0 -1  0    x3    1  2  1 \n\
              1  0 -1            -1  0  1          2  4  2 \n\
              0  1  0             0  1  0          1  2  1 \n\
        Note: these show result of applying to impulse. \n\
        (1 centered on impulse)           -1 \n\
          h2:  -1 1       v2: 1        l2:   1       r2:   1 \n\
                             -1                         -1  \n\
         also: h+, h- shift horiz; v+, v- shift vert. \n");
   exit(-1);
  }
  else if (!strcmp(fs,"x1")) {		/* x1 filter */
    for (j=0;  j < in.h; j++)
      for (i=0; i < in.w; i++)
        q[j][i] = p[j][i+1] + p[j-1][i] - p[j][i-1] - p[j+1][i];
  } 
  else if (!strcmp(fs,"x2")) {		/* x2 filter */
    for (j=0;  j < in.h; j++)
      for (i=0; i < in.w; i++)
        q[j][i] = -p[j][i+1] + p[j-1][i] + p[j][i-1] - p[j+1][i];
  }
  else if (!strcmp(fs,"x3")) {		/* x3 filter */
    for (j=0;  j < in.h; j++)
      for (i=0; i < in.w; i++)
        q[j][i] = 4*p[j][i] + (p[j][i+1] + 2*p[j][i-1] + p[j+1][i] + p[j-1][i])
                  + (p[j+1][i+1] + p[j-1][i+1] + p[j+1][i-1] + p[j-1][i-1]);
  }
  else if (!strcmp(fs,"h1")) {		/* h1 filter */
    for (j=0;  j < in.h; j++)
      for (i=0; i < in.w; i++)
        q[j][i] = 0.5*(p[j][i] + p[j][i+1]);
  }
  else if (!strcmp(fs,"h2")) {		/* h2 filter */
    for (j=0;  j < in.h; j++)
      for (i=0; i < in.w; i++)
        q[j][i] = p[j][i] - p[j][i+1];
  }
  else if (!strcmp(fs,"h3")) {		/* h3 filter */
    for (j=0;  j < in.h; j++)
      for (i=0; i < in.w; i++)
        q[j][i] = 0.5*p[j][i] + 0.25*(p[j][i-1] + p[j][i+1]);
  }
  else if (!strcmp(fs,"h4")) {		/* h4 filter */
    for (j=0;  j < in.h; j++)
      for (i=0; i < in.w; i++)
        q[j][i] = 2*p[j][i] - p[j][i-1] - p[j][i+1];
  }
  else if (!strcmp(fs,"h5")) {		/* h5 filter */
    for (j=0;  j < in.h; j++)
      for (i=0; i < in.w; i++)
        q[j][i] = p[j][i-1] - p[j][i+1];
  }
  else if (!strcmp(fs,"h6")) {		/* h6 filter */
    for (j=0;  j < in.h; j++)
      for (i=0; i < in.w; i++)
        q[j][i] = 0.5*(p[j][i-1] + p[j][i+1]);
  }
  else if (!strcmp(fs,"h+")) {		/* hor shift  */
    for (j=0;  j < in.h; j++)
      for (i= -Image_border+1; i < in.w+Image_border; i++)
        q[j][i] = p[j][i-1];
  }
  else if (!strcmp(fs,"h-")) {		/* hor shift  */
    for (j=0;  j < in.h; j++)
      for (i= -Image_border; i < in.w+Image_border-1; i++)
        q[j][i] = p[j][i+1];
  }
  else if (!strcmp(fs,"v1")) {		/* v1 filter */
    for (j=0;  j < in.h; j++)
      for (i=0; i < in.w; i++)
        q[j][i] = 0.5*(p[j][i] + p[j+1][i]);
  }
  else if (!strcmp(fs,"v2")) {		/* v2 filter */
    for (j=0;  j < in.h; j++)
      for (i=0; i < in.w; i++)
        q[j][i] = p[j][i] - p[j+1][i];
  }
  else if (!strcmp(fs,"v3")) {		/* v3 filter */
    for (j=0;  j < in.h; j++)
      for (i=0; i < in.w; i++)
        q[j][i] = 0.5*p[j][i] + 0.25*(p[j-1][i] + p[j+1][i]);
  }
  else if (!strcmp(fs,"v4")) {		/* v4 filter */
    for (j=0;  j < in.h; j++)
      for (i=0; i < in.w; i++)
        q[j][i] = 2*p[j][i] - p[j-1][i] - p[j+1][i];
  }
  else if (!strcmp(fs,"v5")) {		/* v5 filter */
    for (j=0;  j < in.h; j++)
      for (i=0; i < in.w; i++)
        q[j][i] = p[j-1][i] - p[j+1][i];
  }
  else if (!strcmp(fs,"v6")) {		/* v6 filter */
    for (j=0;  j < in.h; j++)
      for (i=0; i < in.w; i++)
        q[j][i] = 0.5*(p[j-1][i] + p[j+1][i]);
  }
  else if (!strcmp(fs,"v+")) {		/* ver shift  */
    for (j= -Image_border+1;  j < in.h+Image_border; j++)
      for (i=0; i < in.w; i++)
        q[j][i] = p[j-1][i];
  }
  else if (!strcmp(fs,"v-")) {		/* ver shift  */
    for (j= -Image_border;  j < in.h+Image_border-1; j++)
      for (i=0; i < in.w; i++)
        q[j][i] = p[j+1][i];
  }
  else if (!strcmp(fs,"l1")) {		/* l1 filter */
    for (j=0;  j < in.h; j++)
      for (i=0; i < in.w; i++)
        q[j][i] = 0.5*(p[j][i] + p[j+1][i+1]);
  }
  else if (!strcmp(fs,"l2")) {		/* l2 filter */
    for (j=0;  j < in.h; j++)
      for (i=0; i < in.w; i++)
        q[j][i] = p[j][i] - p[j+1][i+1];
  }
  else if (!strcmp(fs,"l3")) {		/* l3 filter */
    for (j=0;  j < in.h; j++)
      for (i=0; i < in.w; i++)
        q[j][i] = 0.5*p[j][i] + 0.25*(p[j+1][i+1] + p[j-1][i-1]);
  }
  else if (!strcmp(fs,"l4")) {		/* l4 filter */
    for (j=0;  j < in.h; j++)
      for (i=0; i < in.w; i++)
        q[j][i] = 2*p[j][i] - p[j+1][i+1] - p[j-1][i-1];
  }
  else if (!strcmp(fs,"l5")) {		/* l5 filter */
    for (j=0;  j < in.h; j++)
      for (i=0; i < in.w; i++)
        q[j][i] = p[j-1][i-1] - p[j+1][i+1];
  }
  else if (!strcmp(fs,"l6")) {		/* l6 filter */
    for (j=0;  j < in.h; j++)
      for (i=0; i < in.w; i++)
        q[j][i] = 0.5*(p[j+1][i+1] + p[j-1][i-1]);
  }
  else if (!strcmp(fs,"r1")) {		/* r1 filter */
    for (j=0;  j < in.h; j++)
      for (i=0; i < in.w; i++)
        q[j][i] = 0.5*(p[j][i] + p[j-1][i+1]);
  }
  else if (!strcmp(fs,"r2")) {		/* r2 filter */
    for (j=0;  j < in.h; j++)
      for (i=0; i < in.w; i++)
        q[j][i] = p[j][i] - p[j-1][i+1];
  }
  else if (!strcmp(fs,"r3")) {		/* r3 filter */
    for (j=0;  j < in.h; j++)
      for (i=0; i < in.w; i++)
        q[j][i] = 0.5*p[j][i] + 0.25*(p[j-1][i+1] + p[j+1][i-1]);
  }
  else if (!strcmp(fs,"r4")) {		/* r4 filter */
    for (j=0;  j < in.h; j++)
      for (i=0; i < in.w; i++)
        q[j][i] = 2*p[j][i] - p[j+1][i-1] - p[j-1][i+1];
  }
  else if (!strcmp(fs,"r5")) {		/* r5 filter */
    for (j=0;  j < in.h; j++)
      for (i=0; i < in.w; i++)
        q[j][i] = p[j+1][i-1] - p[j-1][i+1];
  }
  else if (!strcmp(fs,"r6")) {		/* r6 filter */
    for (j=0;  j < in.h; j++)
      for (i=0; i < in.w; i++)
        q[j][i] = 0.5*(p[j+1][i-1] + p[j-1][i+1]);
  }
  else {
    fprintf(stderr,"kernel: Unknown kernel %s\n",fs);
    exit(-1);
  }
}


