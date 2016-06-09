/******************************************************************************
 SGI/movie.h < movie.c by Paul Haeberli - 1985,
 hacked by spraxlo@ai.mit.edu, 93.7.10
 ******************************************************************************/
#ifndef	sgi_movie_h__
#define	sgi_movie_h__

#define XBORDER         8
#define TOPBORDER       8
#define BOTBORDER       8

#define STOPPED         1
#define LOOPING         2
#define SWINGING        3
#define FORBACK         4
#define SLIDERS         6
#define MOUSEPOS        8

#define FORWARD         0
#define BACK            1

void	back(void);
void	forward(void);
void	step(void);
void	swing(void);
void	gotoframe(int n);
void	addframe(char *filename);
void	makeframe(int frameno);
DISPIMAGE *myreadimage(char *name);

extern	void	percentdone(float);
extern	float	fgetmousex(void);
extern	float	fgetmousey(void);
extern	long	grey(float);		/* libgutil: vcolor.c */
extern	int	findname(char *name, char *fullname,char *GFXpath);
extern	DISPIMAGE	*makedisp(IMAGE *image);

#endif	/* sgi_movie_h__ */


