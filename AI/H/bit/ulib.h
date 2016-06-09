/*
 * $Id: ulib.h,v 0.60 1993/09/30 17:54:50 zhao Exp $
 *
 *.  Copyright(c) 1993 by T.C. Zhao
 *   All rights reserved.
 *.
 *    Macros and Prototypes for the utilities routines
 */

#ifndef TC_ULIB_H
#define TC_ULIB_H

#include <stdio.h>		/* for FILE */

/* For lint: _semputc and _semgetc never decleared in stdio.h */
#ifdef lint

extern int _semputc(int, FILE *);
extern int _semgetc(FILE *);
extern const int ZERO;

#else /* !defined lint */

#ifdef ZERO
#undef ZERO
#endif /* def ZERO */

#define ZERO 0
#endif /* lint related */





/********** Rectangle routines ******************/
typedef struct
  {
      int x, y;
      int w, h;
  }
Rect_t;

extern const Rect_t *rect_overlap(Rect_t *, Rect_t *);

/**** for compilation with GCC on ultrix *******/

extern char *strdup(const char *);
extern int strcasecmp(const char *, const char *);
extern char *getenv(const char *);

/****************************************************************
 * Semi-portable directory reader with cache
 ******************************************************************/
#include <limits.h>

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif

enum
  {
      FT_DIR, FT_FILE, FT_OTHER
  };

typedef struct
  {
      char *name;		/* entry name                 */
      short type;		/* one of the enums           */
#if 0
      short misc;		/* other info about the entry */
#endif
  }
Dirlist;

extern void free_all_dirlist(void);
extern Dirlist *get_dir_list(const char *, const char *, int *, int);
extern void rm_all_files(const char *);
extern unsigned long f_mtime(const char *);

/*********** Portable IO operations ***********/

extern int get2LSBF(FILE *);
extern int get2MSBF(FILE *);
extern long get4LSBF(FILE *);
extern long get4MSBF(FILE *);

extern void put2LSBF(int, FILE *);
extern void put2MSBF(int, FILE *);
extern void put4LSBF(long, FILE *);
extern void put4MSBF(long, FILE *);

extern int readint(FILE *);
extern int readpint(FILE *);
extern int readhexint(FILE *);
extern int readstring(FILE *, char[], int);
extern double readfloat(FILE *);
extern double readpfloat(FILE *);
extern void set_read_silent(int);
extern int skip_comment(FILE *);

/************ Some string utilities ****************/

extern char *de_space(char *);
extern char *space_de(char *);
extern char *de_space_de(char *);


/********** A recursive descent parar/evaluator *******/

extern int rd_evaluate(const char *, float *, float *, int);


/******************************************************************
 * convert number to string with limited capability, primarily
 * for speed critical situations, like continuous reporting etc
 ******************************************************************/

extern const char *itoa(int);
extern const char *ftoa(float, int);
extern unsigned int power_of_2(unsigned int);

/********* Variable number arguments strcat ******************/

extern char *vstrcat(const char *,...);
extern void free_vstrcat(void *);

extern int sep_choices(char *, char *[]);
extern void set_sep_letter(int);
extern const char *comb_choices(char *[], int);

/********** Save and restore "current directory" *************/

extern void push_dir(void);
extern void pop_dir(void);

/****** UNIX specific utilities ********/

extern unsigned msleep(unsigned);
extern unsigned malarm(unsigned);
extern int split_fname(char[], char[], const char *);
extern char *fix_dirname(char *);
extern char *fix_dir_tail(char[]);
extern int is_valid_dir(const char *);
extern const char *getext(const char *);
extern char *get_tmpf(const char *);
extern void del_tmpf(char *);
extern void del_all_tmpf(void);

/******** Timing routines *******************/

extern double current_time(void);
extern void reset_time(void);
extern long time_passed(void);

/******** misc. utilities **************/
extern int b_search(float *, int, float);
extern void find_bbox(int, int, int, int, float, int *, int *, int *, int *);

/******** pattern matching (after Rich Salz) ************/
extern int wildmat(const char *, const char *);


/************  misc. macros ***********************/

#ifndef Badfread
#define Badfread(buf, es, n, fp)    (fread(buf, es, n, fp) != n)
#define Badfwrite(buf, es, n, fp)   (fwrite(buf, es, n, fp) != n)
#define Badfgets(buf, l, fp)        (fgets(buf, l, fp) == 0)
#endif

#ifndef Abs
#define Abs(a)      ((a) >= 0 ? (a) : -(a))
#endif

#ifndef Min
#define Min(a, b)   ( ((a) > (b)) ? (b) : (a))
#endif

#ifndef Max
#define Max(a, b)   ( ((a) > (b)) ? (a) : (b))
#endif

#ifndef StrReDup
#define StrReDup(a, b)  do { if(a) free(a); a = strdup(b); } while(ZERO)
#endif

#ifndef Readline
#define Readline(s, fp)                                         \
	do                                                      \
	{   register int c_;                                    \
	    while ((c_ = getc(fp)) != EOF && c_ != '\n')        \
	       *s++ = c_;                                       \
            *s = '\0';                                          \
         } while (ZERO)
#endif

/*********************************************************************
 * Basic error handling routines
 ********************************************************************/

#ifndef TC_ERROR_H
#define TC_ERROR_H

#include <errno.h>

/*
 * message levels(verbosity). Error generating routine should
 * have a (positive) control parameter specifying how loud
 * to bark (i.e., amount of messages generated)
 */
typedef enum
  {
      ML_ERR = -1,		/* always print       */
      ML_WARN,			/* warning            */
      ML_INFO,			/* good to have       */
      ML_DEBUG,			/* many output        */
      ML_TRACE			/* bad program!       */
  } ML_t;

/*
 * actions to take when something is not right.
 * A_WAIT = 0, so that a default action is to wait. Not implemented
 */
typedef enum
  {
      A_WAIT,			/* print msg, demanding a response */
      A_MSG,			/* print msg, do nothing           */
      A_QUIT,			/* fatal, quit gracefully          */
      A_DIE			/* fatal, quit immdediately        */
  } Act_t;

typedef void (*ErrFunc_) (const char *, const char *,...);
extern ErrFunc_ whereError(int, ML_t, const char *, int), efp_;

/*
 * define the actual names that will be used
 */

/* no graphics stuff */
#define M_err       (efp_ = whereError(0, ML_ERR, __FILE__, __LINE__))
#define M_warn      (efp_ = whereError(0, ML_WARN, __FILE__, __LINE__))
#define M_info      (efp_ = whereError(0, ML_INFO, __FILE__, __LINE__))
#define M_debug     (efp_ = whereError(0, ML_DEBUG, __FILE__, __LINE__))
#define M_trace     (efp_ = whereError(0, ML_TRACE, __FILE__, __LINE__))
#define M_msg(a, b, c) (efp_=whereError(0, a, b, c))

/* graphics    */
#define Bark                (efp_= whereError(1, 0, __FILE__, __LINE__))
#define GM_msg(a, b, c)     (efp_= whereError(1, a, b, c))

/*
 * the actual output routines takes three constant strings and an integer
 * in the order title, where, why, 0
 */
typedef void (*Gmsgout_) (const char *, const char *, const char *, int);

/****** Misc. control routines **********/
extern void set_err_msg_func(Gmsgout_);
extern void set_err_msg_fp(FILE *);
extern void set_msg_threshold(ML_t);
extern void set_err_msg_brief(int);

#define to_be_written(a) Bark(a,"To be written");

#endif /* ERROR_H */


#endif /* TC_ULIB_H */

