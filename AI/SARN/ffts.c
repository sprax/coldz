/****************************************************************************/
/* FFTS --- file containing 2d fft subroutines				    */
/*          all routines call fourt - multidimensional fft routine 	    */
/*				     in file : fourt.for                    */
/****************************************************************************/

struct complex {
  float r;           /* real part       */
  float i;           /* imaginary part  */
};



/* 2d forward fftroutine
     set iform = 0 for real fft
     work array needed if dimensions are not powers of 2. (size max(nx,ny)) */


fft2d(cpic,nx,ny,iform,work)
struct complex *cpic;		/* input array */
int nx,ny;			/* x and y dimensions */
int iform;			/* flag : 0 for real fft */
struct complex *work;		/* work array */
{
	int dim[2];			/* dimension array */
	int ndim;			/* number of dimensions */
	int sign;			/* -1 for forward transform 1 for reverse */


	dim[0] = nx;  dim[1] = ny;
	sign = -1;		/* forward transform */
	ndim = 2;		/* number of dimensions */
	fourt_(cpic,dim,&ndim,&sign,&iform,work);
}





/* 2d inverse fftroutine (unscaled)
     set iform = 0 for real fft
     work array needed if dimensions are not powers of 2. (dim max(nx,ny)) */


ifft2d(cpic,nx,ny,iform,work)
struct complex *cpic;		/* input array */
int nx,ny;			/* x and y dimensions */
int iform;			/* flag : 0 for real fft */
struct complex *work;		/* work array */
{
	struct complex *row;		/* holds one row or column of array */
	int dim[2];			/* dimension array */
	int ndim;			/* number of dimensions */
	int sign;			/* -1 for forward transform 1 for reverse */
	int form;			/* form = 0 for real, 1 for complex */
	register struct complex *r,*c;	/* pointers used in loops */
	register struct complex *rl,*rr;
	int i,j;

	dim[0] = nx;  dim[1] = ny;
	sign = 1;		/* reverse transform */
	if (iform == 1) {

	   ndim = 2; form = 1;
	   fourt_(cpic,dim,&ndim,&sign,&form,work);
	}

        else {

	   row = (struct complex *) (malloc(ny*sizeof(struct complex)));
	   ndim = 1; form = 1;
	   for (j=0; j <= ny/2; j++)
	      fourt_((struct complex *) (cpic+j*nx),&nx,&ndim,&sign,&form,work);
	   for (i=0, c=cpic+i; i < nx; i += 2, c=cpic+i) {
	      row->r = c->r - (c+1)->i;	     /* row[0] = cpic[i,0] +       */
	      row->i = c->i + (c+1)->r;      /* cmplx(0.0,1.0)*cpic[i+1,0] */
	      for (j=1, c+=nx; j <= ny/2; j++, c += nx) {
	         rl = row+j;  rr = row+ny-j;
	         rl->r = c->r - (c+1)->i;    /* row[j] = cpic[i,j] +        */
	         rl->i = c->i + (c+1)->r;    /* cmplx(0.0,1.0)*cpic[i+1,j]  */
		 rr->r = c->r + (c+1)->i;    /* row[ny-j] = conjg(cpic[i,j])*/
	         rr->i = -c->i + (c+1)->r;   /* +cmplx(0.0,1.0)*conjg(cpic[i+1,j])*/
	      }
	      fourt_(row,&ny,&ndim,&sign,&form,work);
	      for (j=0, c=cpic+i, r=row; j < ny; j++, r++, c += nx) {
	         c->r = r->r; c->i = 0.0;
	         (c+1)->r = r->i; (c+1)->i = 0.0;
	      }
	   }
	   free(row);
	}
}



/* 2d forward fftroutine
     set iform = 0 for real fft
     work array needed if dimensions are not powers of 2. (size max(nx,ny)) 
     uses 1d transforms only                                               */


fft_2d(cpic,nx,ny,iform,work)
struct complex *cpic;		/* input array */
int nx,ny;			/* x and y dimensions */
int iform;			/* flag : 0 for real fft */
struct complex *work;		/* work array */
{
	struct complex *row;		/* holds one row or column of array */
	int dim[2];			/* dimension array */
	int ndim;			/* number of dimensions */
	int sign;			/* -1 for forward transform 1 for reverse */
	int form;			/* form = 0 for real, 1 for complex */
	register struct complex *r,*c;	/* pointers used in loops */
	int i,j;
	int size;

	dim[0] = nx;  dim[1] = ny;
	sign = -1;		/* forward transform */
	if (iform == 1) {

	   row = (struct complex *) (malloc(ny*sizeof(struct complex)));
           ndim = 1; form = 1;
	   for (j=0; j < ny; j++)
	      fourt_((struct complex *) (cpic+j*nx),&nx,&ndim,&sign,&form,work);
	   for (i=0; i < nx; i++) {
	      for (j=0, r=row, c=cpic+i; j < ny; j++, c += nx)
	         *r++ = *c;
	      fourt_(row,&ny,&ndim,&sign,&form,work);
              for (j=0, r=row, c=cpic+i; j < ny; j++, c += nx)
	         *c = *r++;
           }
	   free(row);
        }

	else {
	   row = (struct complex *) (malloc(ny*sizeof(struct complex)));
           ndim = 1; form = 0;
	   for (j=0; j < ny; j++)
	      fourt_((struct complex *) (cpic+j*nx),&nx,&ndim,&sign,&form,work);

	   form = 1;
           for (i=0; i <= nx/2; i++) {
	      for (j=0, r=row, c=cpic+i; j < ny; j++, c += nx)
	         *r++ = *c;
	      fourt_(row,&ny,&ndim,&sign,&form,work);
	      for (j=0, r=row, c=cpic+i; j < ny; j++, c += nx)
	         *c = *r++;
	      if (i != 0 && i != nx/2) {
                 c = cpic+nx-i; r = row;
	         c->r = r->r; c->i = -(r->i);
	         for (j=1,r=row+ny-1,c+=nx; j < ny; j++,c += nx,r--) {
	            c->r = r->r; c->i = -(r->i);
                 }
              }
           }
           free(row);
        }
}





/* 2d inverse fftroutine (unscaled)
     set iform = 0 for real fft
     work array needed if dimensions are not powers of 2. (dim max(nx,ny)) 
     uses 1d transforms only                                              */


ifft_2d(cpic,nx,ny,iform,work)
struct complex *cpic;		/* input array */
int nx,ny;			/* x and y dimensions */
int iform;			/* flag : 0 for real fft */
struct complex *work;		/* work array */
{
	struct complex *row;		/* holds one row or column of array */
	int dim[2];			/* dimension array */
	int ndim;			/* number of dimensions */
	int sign;			/* -1 for forward transform 1 for reverse */
	int form;			/* form = 0 for real, 1 for complex */
	register struct complex *r,*c;	/* pointers used in loops */
	register struct complex *rl,*rr;
	int i,j;

	dim[0] = nx;  dim[1] = ny;
	sign = 1;		/* reverse transform */
	if (iform == 1) {

	   row = (struct complex *) (malloc(ny*sizeof(struct complex)));
           ndim = 1; form = 1;
	   for (j=0; j < ny; j++)
	      fourt_((struct complex *) (cpic+j*nx),&nx,&ndim,&sign,&form,work);
	   for (i=0; i < nx; i++) {
	      for (j=0, r=row, c=cpic+i; j < ny; j++, c += nx)
	         *r++ = *c;
	      fourt_(row,&ny,&ndim,&sign,&form,work);
              for (j=0, r=row, c=cpic+i; j < ny; j++, c += nx)
	         *c = *r++;
           }
	   free(row);
	}

        else {

	   row = (struct complex *) (malloc(ny*sizeof(struct complex)));
	   ndim = 1; form = 1;
	   for (j=0; j <= ny/2; j++)
	      fourt_((struct complex *) (cpic+j*nx),&nx,&ndim,&sign,&form,work);
	   for (i=0, c=cpic+i; i < nx; i += 2, c=cpic+i) {
	      row->r = c->r - (c+1)->i;	     /* row[0] = cpic[i,0] +       */
	      row->i = c->i + (c+1)->r;      /* cmplx(0.0,1.0)*cpic[i+1,0] */
	      for (j=1, c+=nx; j <= ny/2; j++, c += nx) {
	         rl = row+j;  rr = row+ny-j;
	         rl->r = c->r - (c+1)->i;    /* row[j] = cpic[i,j] +        */
	         rl->i = c->i + (c+1)->r;    /* cmplx(0.0,1.0)*cpic[i+1,j]  */
		 rr->r = c->r + (c+1)->i;    /* row[ny-j] = conjg(cpic[i,j])*/
	         rr->i = -c->i + (c+1)->r;   /* +cmplx(0.0,1.0)*conjg(cpic[i+1,j])*/
	      }
	      fourt_(row,&ny,&ndim,&sign,&form,work);
	      for (j=0, c=cpic+i, r=row; j < ny; j++, r++, c += nx) {
	         c->r = r->r; c->i = 0.0;
	         (c+1)->r = r->i; (c+1)->i = 0.0;
	      }
	   }
	   free(row);
	}
}

