
/* glstuff.c */
int	greybase(void);
void	savewindow(char *name);
void	printwindow(void);
void	savemovieframe(void);
int 	gfxmachine(void);
void	smartsetdepth(void);
void	fsetdepth(float near, float far);
void	zviewport(long near, long far);
void	getzviewport(long *lnear, long *lfar);
void	drawmeshimage(unsigned long *imgdata, int xsize, int ysize);
void	drawquadlimage(unsigned long *imgdata, int xsize, int ysize);
void	drawtrilimage(unsigned long *imgdata, int xsize, int ysize);
void	drawrectlimage(unsigned long *imgdata, int xsize, int ysize);


