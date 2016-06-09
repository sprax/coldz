/****************************************************************************
aa.cmd.c -- shell invocation
****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
/* #include <ctype.h> #include <MacUnix.h> */
void testMgVarg(void);

#define block 1024




/*
USAGE:
Suppose froob(void *varg, ...) can handle 0,1,2,3, or 5 (but not 4) args
(not counting the initial va_list void* count arg),
and that they are bar (a typedef'd struct), float, int, char and short, resp.
Noting first that chars (and shorts) may be promoted to ints, and floats to
doubles, froob could then be written as:
*/
#include "unTypes.h"
#include "unArgs.h"
#include <stdarg.h>
#define SZ(a)	stack_sizeof(a)


static void print_dbl(const char *fmt, ...);
static void vadbl(void *varg, ...);

typedef struct bar_s { int rot[4][4]; unsigned stat; } bar;
static bar DefBar;	/* will be initialized to all 0's in BSS */

static void print_dbl(const char *fmt, ...)
{
    va_list  args;
    va_start(args,fmt);
    vfprintf(stderr,fmt,args);
    va_end(args);
}

#define NARG 6
#define BUFLEN 256

static double dd[NARG];   /* All initialized to 0 in BSS. */

static void vadbl(void *varg, ...)
{
    char fmt[BUFLEN] = "";
    int j, narg;
    va_last(varg,narg,double);
    /*{static tc = 0; fprintf(stderr,"vadbl call %d, narg: %d\n",tc++,narg);}*/

    /* Now create a format for narg doubles, setting dd & j as side effects: */
    if (narg > NARG)  narg = NARG;           /* cap narg */
    for (j = 0; narg; j++) {
        dd[--narg] = va_dec(varg,double);
        strcat_s(fmt,BUFLEN,"%g ");
    }
    strcat_s(fmt,BUFLEN,"\n");

    /* The following calls to vfprintf and print_dbl should all produce
    exactly the same results.  Stupid, but this is only a test. */
    vfprintf(stderr,fmt,(va_list)((double *)varg + 1));
    vfprintf(stderr,fmt,(va_list)((unsigned long)varg+sizeof(double)));
    switch(j) {
    case  0: break;
    case  1: print_dbl(fmt,dd[0]); break;
    case  2: print_dbl(fmt,dd[0], dd[1]); break;
    case  3: print_dbl(fmt,dd[0], dd[1], dd[2]); break;
    case  4: print_dbl(fmt,dd[0], dd[1], dd[2], dd[3]); break;
    case  5: print_dbl(fmt,dd[0], dd[1], dd[2], dd[3], dd[4]); break;
    default: fprintf(stderr,"vadbl can't handle %d args!\n",j);
    }
}


#if 1
char *join(char *dst, const char *src) // deprecated 
{
    strcat_s(dst, BUFLEN, src);
    return dst;
}

void main(void)
{
#if 1
    static int yoyo = block * 3;
    char *p,*q,*r="string";
    static char a[] = "0123456789";
    int i = 5;

    /* for (i = 0; i < NARG; i++)
    fprintf(stderr,"dd[%d] is %g\n",i,dd[i]);
    */
    vadbl(0);
    vadbl((void*) 1, 3.1459);
    vadbl((void*) 2, 3.1459, 2.71);
    vadbl((void*) 3, 3.1459, 2.71, 3.45);
    vadbl((void*) 4, 3.1459, 2.71, 3.45, 4.56);
    vadbl((void*) 5, 3.1459, 2.71, 3.45, 4.56, 5.67);
    exit(0);

    p = a;
    printf("a+3 = %x, &*(a+3) = %x, *(a+3) = %x, a[3] = %x\n", a+3, &*(a+3), *(a+3), a[3]);
    printf("p+3 = %x, &*(p+3) = %x, *(p+3) = %x, p[3] = %x\n", p+3, &*(p+3), *(p+3), p[3]);

#define GOOF 5
    assert(GOOF < 3);


    printf("a[i] = %c\n", a[i]);
    printf("i[a] = %c\n", i[a]);

    p = "string";
    q = "string";
    p[3] = 'o';
    printf("p & q: %x<%s> %x<%s>  %x<%s>  %c\n",&p,p,&q,q,&r,r,"string"[3]);
    p = join("donald"," trumpola");
    printf("p before is %s\n",p);fflush(stdout);
    printf("\np=%s, join returns %s, now p=%s\n",p,join("duckling ","soup"),p);
    return;
#else
    static char **envp=NULL;
    static char *argv[MAX_ARGS];
    static int argc=1;
    argv[argc++] = "ddd";
    argv[argc]	 = NULL;
    Main(argc,argv,envp);
#endif
}



#endif

