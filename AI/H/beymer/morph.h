#ifndef beymer_morph_h_ /*****************************/
#define beymer_morph_h_ /******** random junk ********/

#define INTERSECTION	0
#define COPY			1
#define SUBTRACT		2
#define UNION			3

#define PIXEL_COPY		0
#define PIXEL_OR		1
#define PIXEL_AND		2
#define PIXEL_NOT		3

#ifndef TRUE
#define FALSE			0
#define TRUE			1
#endif

#ifndef MOVIE_DIR
#ifdef	SGI
#define MOVIE_DIR	"/home/vu/spraxlo/c/face/morph/movies/SGI"
#else
#ifdef	SUN
#define MOVIE_DIR	"/home/vu/spraxlo/c/face/morph/movies/SUN"
#else
#define MOVIE_DIR	"movies"
#endif
#endif
#endif

#endif	/* beymer_morph_h_ **************************************************/


