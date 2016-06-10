/* Motion estimation */

#include <stdlib.h>

#define  OLD_ANSI_MATHERR /* SGI only: defines struct exception, matherr(); */
#include "sarnoff/imgdec.h"
#include "sarnoff/imgmacro.h"
#undef   OLD_ANSI_MATHERR /* SGI only: see /usr/include/math.h */

#include "fimgop.h"

#include <un/unMath.h>


#define trunc(A) ((A) > 0 ? (floor(A)) : (ceil(A)))

extern int Image_border;
extern FILTER rf, ef;

#define LIMIT		/* limit residual velocities (only gradient method) */
#define EXTEND

#define TWOPI 6.28318530717958647692528
#define PI 3.14159265358979323846
#define HALF_WINDOW (2)
#define WINDOW (2*HALF_WINDOW+1)

static int CSinit = 0;
static float *Sin, *Cos;


void lsq_opt(FIMAGE image1, FIMAGE image2, FIMAGE vx, FIMAGE vy)	/* < Haim */
{
    register float **img1 = image1.ptr, **img2 = image2.ptr;
    float **dx = vx.ptr, **dy = vy.ptr, S, C, A, B;
    register float *s1x, *s2x, *s1y, *s2y;
    float *alloc_fvector();
    int w = image1.w, h = image1.h, x, y, left, right;
    register int xx, yy, i, j, mj;
    struct ffcomplex f1_10, f1_01, f2_10, f2_01;
    void SinCos(void);

    /* Init Cos and Sin tables if necessary */
    if (!CSinit) {
        SinCos();
        CSinit++;
    }
    /* Reflect images */
    /*   reflect(image1,Image_border,3);*/
    /*   reflect(image2,Image_border,3);*/
    /* Zero out border on vx and vy */
    fset(vx, 0.0);
    fset(vy, 0.0);

    /* Allocate needed vectors and matrices */
    s1x = alloc_fvector(-HALF_WINDOW, HALF_WINDOW);
    s2x = alloc_fvector(-HALF_WINDOW, HALF_WINDOW);
    s1y = alloc_fvector(-HALF_WINDOW, HALF_WINDOW);
    s2y = alloc_fvector(-HALF_WINDOW, HALF_WINDOW);

    /* Start motion estimation */

    for (y = HALF_WINDOW; y < h - HALF_WINDOW; y++) {
        for (x = HALF_WINDOW; x < w - HALF_WINDOW; x++) {
#ifdef DEBUG 
            printf("\n**** Starting computation for Point: %d,%d ****\n", x, y);
#endif

            /* Get sum over rows and cols for 1st sum of FFT */
            if (x == HALF_WINDOW) {
                for (j = -HALF_WINDOW; j <= HALF_WINDOW; j++) {
                    s1x[j] = 0.0;  s2x[j] = 0.0;  s1y[j] = 0.0;  s2y[j] = 0.0;
                }
                for (j = -HALF_WINDOW, yy = y + j; j <= HALF_WINDOW; j++, yy++)
                    for (i = -HALF_WINDOW, xx = x + i; i <= HALF_WINDOW; i++, xx++) {
                        A = img1[yy][xx];   B = img2[yy][xx];
                        s1x[i] += A;  s1y[j] += A;
                        s2x[i] += B;  s2y[j] += B;
                    }
                left = 0; right = left + WINDOW;
            }
            else {		/* only do update to fill up s1x,s2x,s1y,s2y */
                for (i = -HALF_WINDOW; i < HALF_WINDOW; i++) {
                    s1x[i] = s1x[i + 1]; s2x[i] = s2x[i + 1];
                }
                s1x[HALF_WINDOW] = 0.0;  s2x[HALF_WINDOW] = 0.0;
                for (j = -HALF_WINDOW, yy = y + j; j <= HALF_WINDOW; j++, yy++) {
                    A = img1[yy][right];   B = img2[yy][right];
                    s1x[HALF_WINDOW] += A; s2x[HALF_WINDOW] += B;
                    s1y[j] += (A - img1[yy][left]);
                    s2y[j] += (B - img2[yy][left]);
                }
                left++; right++;
            }

            /* Compute f10,f01 (using symmetry of sin and cos) */
            f1_10.r = s1x[0];    f1_01.r = s1y[0];
            f2_10.r = s2x[0];    f2_01.r = s2y[0];
            f1_10.i = f1_01.i = f2_10.i = f2_01.i = 0.0;
            for (j = 1, mj = -j; j <= HALF_WINDOW; j++, mj = -j) {
                S = Sin[j];  C = Cos[j];
                f1_10.r += (s1x[j] + s1x[mj])*C;
                f1_10.i -= (s1x[j] - s1x[mj])*S;
                f1_01.r += (s1y[j] + s1y[mj])*C;
                f1_01.i -= (s1y[j] - s1y[mj])*S;
                f2_10.r += (s2x[j] + s2x[mj])*C;
                f2_10.i -= (s2x[j] - s2x[mj])*S;
                f2_01.r += (s2y[j] + s2y[mj])*C;
                f2_01.i -= (s2y[j] - s2y[mj])*S;
            }
#ifdef DEBUG
            printf("f1_10 = (%f, %f)\n", f1_10.r, f1_10.i);
            printf("f1_01 = (%f, %f)\n", f1_01.r, f1_01.i);
            printf("f2_10 = (%f, %f)\n", f2_10.r, f2_10.i);
            printf("f2_01 = (%f, %f)\n", f2_01.r, f2_01.i);
#endif

            /* Compute velocities */
            S = f1_10.i*f2_10.r - f1_10.r*f2_10.i;
            C = f1_10.r*f2_10.r + f1_10.i*f2_10.i;
#ifdef DEBUG
            printf("X-->   S=%f  C=%f\n", S, C);
#endif
            if (S != 0.0 || C != 0.0)
                dx[y][x] = atan2(S, C)*WINDOW / PI;
            else
                dx[y][x] = 0.0;
            S = f1_01.i*f2_01.r - f1_01.r*f2_01.i;
            C = f1_01.r*f2_01.r + f1_01.i*f2_01.i;
#ifdef DEBUG
            printf("Y-->   S=%f  C=%f\n", S, C);
#endif
            if (S != 0.0 || C != 0.0)
                dy[y][x] = atan2(S, C)*WINDOW / PI;
            else
                dy[y][x] = 0.0;
#ifdef DEBUG
            printf("Velocity vector: %8.4f  %8.4f\n", dx[y][x], dy[y][x]);
#endif
        }
    }

    /* Free vectors and matrices */
    free_fvector(s2y, -HALF_WINDOW, HALF_WINDOW);
    free_fvector(s1y, -HALF_WINDOW, HALF_WINDOW);
    free_fvector(s2x, -HALF_WINDOW, HALF_WINDOW);
    free_fvector(s1x, -HALF_WINDOW, HALF_WINDOW);

#ifdef EXTEND
    /* Extend velocity fields */
    extend(vx, HALF_WINDOW);
    extend(vy, HALF_WINDOW);
#endif
}




void lsq_fast(FIMAGE image1, FIMAGE image2, FIMAGE vx, FIMAGE vy)
{
    register float **img1 = image1.ptr, **img2 = image2.ptr;
    float **dx = vx.ptr, **dy = vy.ptr, S, C, A, B;
    register float *s1x, *s2x, *s1y, *s2y;
    float *alloc_fvector();
    int w = image1.w, h = image1.h, x, y;
    register int xx, yy, i, j, mj;
    struct ffcomplex f1_10, f1_01, f2_10, f2_01;

    /* Init Cos and Sin tables if necessary */
    if (!CSinit) {
        SinCos();
        CSinit++;
    }
    /* Reflect images */
    /*   reflect(image1,Image_border,3);*/
    /*   reflect(image2,Image_border,3);*/
    /* Zero out border on vx and vy */
    fset(vx, 0.0);
    fset(vy, 0.0);

    /* Allocate needed vectors and matrices */
    s1x = alloc_fvector(-HALF_WINDOW, HALF_WINDOW);
    s2x = alloc_fvector(-HALF_WINDOW, HALF_WINDOW);
    s1y = alloc_fvector(-HALF_WINDOW, HALF_WINDOW);
    s2y = alloc_fvector(-HALF_WINDOW, HALF_WINDOW);

    /* Start motion estimation */

    for (y = HALF_WINDOW; y < h - HALF_WINDOW; y++) {
        for (x = HALF_WINDOW; x < w - HALF_WINDOW; x++) {
#ifdef DEBUG
            printf("\n**** Starting computation for Point: %d,%d ****\n", x, y);
#endif

            /* Get sum over rows and cols for 1st sum of FFT */
            for (j = -HALF_WINDOW; j <= HALF_WINDOW; j++) {
                s1x[j] = 0.0;  s2x[j] = 0.0;  s1y[j] = 0.0;  s2y[j] = 0.0;
            }
            for (j = -HALF_WINDOW, yy = y + j; j <= HALF_WINDOW; j++, yy++)
                for (i = -HALF_WINDOW, xx = x + i; i <= HALF_WINDOW; i++, xx++) {
                    A = img1[yy][xx];   B = img2[yy][xx];
                    s1x[i] += A;  s1y[j] += A;
                    s2x[i] += B;  s2y[j] += B;
                }

            /* Compute f10,f01 (using symmetry of sin and cos) */
            f1_10.r = s1x[0];    f1_01.r = s1y[0];
            f2_10.r = s2x[0];    f2_01.r = s2y[0];
            f1_10.i = f1_01.i = f2_10.i = f2_01.i = 0.0;
            for (j = 1, mj = -j; j <= HALF_WINDOW; j++, mj = -j) {
                S = Sin[j];  C = Cos[j];
                f1_10.r += (s1x[j] + s1x[mj])*C;
                f1_10.i -= (s1x[j] - s1x[mj])*S;
                f1_01.r += (s1y[j] + s1y[mj])*C;
                f1_01.i -= (s1y[j] - s1y[mj])*S;
                f2_10.r += (s2x[j] + s2x[mj])*C;
                f2_10.i -= (s2x[j] - s2x[mj])*S;
                f2_01.r += (s2y[j] + s2y[mj])*C;
                f2_01.i -= (s2y[j] - s2y[mj])*S;
            }
#ifdef DEBUG
            printf("f1_10 = (%f, %f)\n", f1_10.r, f1_10.i);
            printf("f1_01 = (%f, %f)\n", f1_01.r, f1_01.i);
            printf("f2_10 = (%f, %f)\n", f2_10.r, f2_10.i);
            printf("f2_01 = (%f, %f)\n", f2_01.r, f2_01.i);
#endif

            /* Compute velocities */
            S = f1_10.i*f2_10.r - f1_10.r*f2_10.i;
            C = f1_10.r*f2_10.r + f1_10.i*f2_10.i;
#ifdef DEBUG
            printf("X-->   S=%f  C=%f\n", S, C);
#endif
            if (S != 0.0 || C != 0.0)
                dx[y][x] = atan2(S, C)*WINDOW / PI;
            else
                dx[y][x] = 0.0;
            S = f1_01.i*f2_01.r - f1_01.r*f2_01.i;
            C = f1_01.r*f2_01.r + f1_01.i*f2_01.i;
#ifdef DEBUG
            printf("Y-->   S=%f  C=%f\n", S, C);
#endif
            if (S != 0.0 || C != 0.0)
                dy[y][x] = atan2(S, C)*WINDOW / PI;
            else
                dy[y][x] = 0.0;
#ifdef DEBUG
            printf("Velocity vector: %8.4f  %8.4f\n", dx[y][x], dy[y][x]);
#endif
        }
    }

    /* Free vectors and matrices */
    free_fvector(s2y, -HALF_WINDOW, HALF_WINDOW);
    free_fvector(s1y, -HALF_WINDOW, HALF_WINDOW);
    free_fvector(s2x, -HALF_WINDOW, HALF_WINDOW);
    free_fvector(s1x, -HALF_WINDOW, HALF_WINDOW);

#ifdef EXTEND
    /* Extend velocity fields */
    extend(vx, HALF_WINDOW);
    extend(vy, HALF_WINDOW);
#endif
}

#ifdef USE_MATHERR
matherr(struct exception *exc)
{
    fprintf(stderr, "%s: ", exc->name);
    switch (exc->type) {
        case DOMAIN:
            fprintf(stderr, "argument domain exception\n");
            break;
        case SING:
            fprintf(stderr, "argument singularity\n");
            break;
        case OVERFLOW:
            fprintf(stderr, "Overflow range exception\n");
            break;
        case UNDERFLOW:
            fprintf(stderr, "Underflow range exception\n");
            break;
        default:
            fprintf(stderr, "Unknown Exception\n");
    }
    fprintf(stderr, "\tinput arguments: %f\t%f\n", exc->arg1, exc->arg2);
    fprintf(stderr, "\treturn Value: %f\n", exc->retval);
    return(0);
}
#endif

void lsq_fourier(FIMAGE image1, FIMAGE image2, FIMAGE vx, FIMAGE vy)
{
    float **img1 = image1.ptr, **img2 = image2.ptr, **dx = vx.ptr, **dy = vy.ptr;
    float *s1x, *s2x, *s1y, *s2y, S, C;
    float *alloc_fvector();
    int w = image1.w, h = image1.h, x, y, i, j;
    struct ffcomplex f1_10, f1_01, f2_10, f2_01;

    /* Init Cos and Sin tables if necessary */
    if (!CSinit) {
        SinCos();
        CSinit++;
    }
    /* Reflect images */
    reflect(image1, Image_border, 3);
    reflect(image2, Image_border, 3);
    /* Zero out border on vx and vy */
    fset(vx, 0.0);
    fset(vy, 0.0);

    /* Allocate needed vectors and matrices */
    s1x = alloc_fvector(-HALF_WINDOW, HALF_WINDOW);
    s2x = alloc_fvector(-HALF_WINDOW, HALF_WINDOW);
    s1y = alloc_fvector(-HALF_WINDOW, HALF_WINDOW);
    s2y = alloc_fvector(-HALF_WINDOW, HALF_WINDOW);

    /* Start motion estimation */

    for (y = 2; y < h - 2; y++) {
        for (x = 2; x < w - 2; x++) {
#ifdef DEBUG
            printf("\n**** Starting computation for Point: %d,%d ****\n", x, y);
#endif

            /* Get sum over rows and cols for 1st sum of FFT */
            for (j = -HALF_WINDOW; j <= HALF_WINDOW; j++) {
                s1x[j] = 0.0;  s2x[j] = 0.0;  s1y[j] = 0.0;  s2y[j] = 0.0;
            }
            for (j = -HALF_WINDOW; j <= HALF_WINDOW; j++)
                for (i = -HALF_WINDOW; i <= HALF_WINDOW; i++) {
                    s1x[i] += img1[y + j][x + i];
                    s1y[j] += img1[y + j][x + i];
                    s2x[i] += img2[y + j][x + i];
                    s2y[j] += img2[y + j][x + i];
                }

            /* Compute f10,f01 */
            f1_10.r = f1_10.i = f1_01.r = f1_01.i = 0.0;
            f2_10.r = f2_10.i = f2_01.r = f2_01.i = 0.0;
            for (j = -HALF_WINDOW; j <= HALF_WINDOW; j++) {
                f1_10.r += s1x[j] * Cos[j];   f1_10.i -= s1x[j] * Sin[j];
                f1_01.r += s1y[j] * Cos[j];   f1_01.i -= s1y[j] * Sin[j];
                f2_10.r += s2x[j] * Cos[j];   f2_10.i -= s2x[j] * Sin[j];
                f2_01.r += s2y[j] * Cos[j];   f2_01.i -= s2y[j] * Sin[j];
            }
#ifdef DEBUG
            printf("f1_10 = (%f, %f)\n", f1_10.r, f1_10.i);
            printf("f1_01 = (%f, %f)\n", f1_01.r, f1_01.i);
            printf("f2_10 = (%f, %f)\n", f2_10.r, f2_10.i);
            printf("f2_01 = (%f, %f)\n", f2_01.r, f2_01.i);
#endif

            /* Compute velocities */
            S = f1_10.i*f2_10.r - f1_10.r*f2_10.i;
            C = f1_10.r*f2_10.r + f1_10.i*f2_10.i;
            dx[y][x] = atan2(S, C)*WINDOW / PI;
            S = f1_01.i*f2_01.r - f1_01.r*f2_01.i;
            C = f1_01.r*f2_01.r + f1_01.i*f2_01.i;
            dy[y][x] = atan2(S, C)*WINDOW / PI;
#ifdef DEBUG
            printf("Velocity vector: %8.4f  %8.4f\n", dx[y][x], dy[y][x]);
#endif
        }
    }

    /* Free vectors and matrices */
    free_fvector(s2y, -HALF_WINDOW, HALF_WINDOW);
    free_fvector(s1y, -HALF_WINDOW, HALF_WINDOW);
    free_fvector(s2x, -HALF_WINDOW, HALF_WINDOW);
    free_fvector(s1x, -HALF_WINDOW, HALF_WINDOW);

#ifdef EXTEND
    /* Extend velocity fields */
    extend(vx, HALF_WINDOW);
    extend(vy, HALF_WINDOW);
#endif
}


void SinCos(void)
{
    int j;
    float *alloc_fvector();

    Sin = alloc_fvector(-HALF_WINDOW, HALF_WINDOW);
    Cos = alloc_fvector(-HALF_WINDOW, HALF_WINDOW);
    for (j = -HALF_WINDOW; j <= HALF_WINDOW; j++) {
        Sin[j] = (float)sin(TWOPI*j / (float)WINDOW);
        Cos[j] = (float)cos(TWOPI*j / (float)WINDOW);
    }
}



#define NITERATIONS 3
#define MAX_CHANGE 1.0
void lsq_pr(FIMAGE image1, FIMAGE image2, FIMAGE velx, FIMAGE vely)
{
    register float **img1 = image1.ptr, **img2 = image2.ptr;
    float **dx = velx.ptr, **dy = vely.ptr;
    float vx, vy, delvx, delvy, epsilon, ldif, edif, dfd;
    int w = image1.w, h = image1.h, iter;
    register int x, y, xx, yy;

    /* Reflect images */
    reflect(image1, Image_border, 3);
    reflect(image2, Image_border, 3);
    /* Zero out border on velx and vely */
    fset(velx, 0.0);
    fset(vely, 0.0);

    /* Start motion estimation */

    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
#ifdef DEBUG 
            printf("\n**** Starting computation for Point: %d,%d ****\n", x, y);
#endif

            vx = vy = 0.0;
            for (iter = 1; iter <= NITERATIONS; iter++) {
                dfd = get_dfd(image1, image2, x, y, vx, vy);
                xx = (int)nint(x - vx);   yy = (int)nint(y - vy);
                ldif = (img1[yy + 1][xx] - img1[yy - 1][xx]) / 2.0;
                edif = (img1[yy][xx + 1] - img1[yy][xx - 1]) / 2.0;
                epsilon = 2 * (ldif*ldif + edif*edif);
                if (epsilon != 0.0)
                    epsilon = 1 / epsilon;
                epsilon *= dfd;
                delvx = epsilon*edif;
                delvy = epsilon*ldif;
                if (delvx == 0.0 && delvy == 0.0)
                    break;
                delvx = MAX(-MAX_CHANGE, MIN(delvx, MAX_CHANGE));
                delvy = MAX(-MAX_CHANGE, MIN(delvy, MAX_CHANGE));
                vx -= delvx;   vy -= delvy;
            }
            dx[y][x] = vx;   dy[y][x] = vy;

#ifdef DEBUG
            printf("Velocity vector: %8.4f  %8.4f\n", dx[y][x], dy[y][x]);
#endif
        }
    }
}


float get_dfd(FIMAGE image1, FIMAGE image2, int i, int j, float vx, float vy)
{
    register float **img1 = image1.ptr, **img2 = image2.ptr;
    float x, y, xfrac, yfrac, y1val, y2val;
    int x1, y1, x2, y2, w = image1.w, h = image1.h;

    x = i - vx;
    y = j - vy;
    x1 = (int)floor(x);
    y1 = (int)floor(y);
    xfrac = x - x1;
    yfrac = y - y1;
    x1 = MAX(0, MIN(w - 1, x1));  x2 = x1 + 1;
    y1 = MAX(0, MIN(h - 1, y1));  y2 = y1 + 1;
    y1val = img1[y1][x1] + xfrac*(img1[y1][x2] - img1[y1][x1]);
    y2val = img1[y2][x1] + xfrac*(img1[y2][x2] - img1[y2][x1]);
    return((float)(img2[j][i] - (y1val + yfrac*(y2val - y1val))));
}


static int Qinit = 0;
static float **Q;
static float **W;
static float *gama;
static float *lambda;
static float **phi;

/*___________________________________________________________________________*/
void lsq_poly(FIMAGE image1, FIMAGE image2, FIMAGE vx, FIMAGE vy)
{
    float **img1 = image1.ptr, **img2 = image2.ptr, **dx = vx.ptr, **dy = vy.ptr;
    float *r, *s;
    float **alloc_farray(), *alloc_fvector(), polyfit_error();
    int w = image1.w, h = image1.h, x, y, xx, yy, i, j, n;
    char file[MAX_STR_LEN];

    float conf;	/* just a dummy for now --spraxlo @@ */

 /* Init Q matrix if necessary */
    if (!Qinit) {
        readq();
        Qinit++;
#ifdef DEBUG
        printq();
#endif
    }
    /* Reflect images */
    reflect(image1, Image_border, 3);
    reflect(image2, Image_border, 3);
    /* Zero out border on vx and vy */
    fset(vx, 0.0);
    fset(vy, 0.0);

    /* Allocate needed vectors and matrices */
    r = alloc_fvector(1, 50);
    s = alloc_fvector(1, 9);
    gama = alloc_fvector(1, 2);
    W = alloc_farray(1, 2, 1, 2);
    phi = alloc_farray(1, 2, 1, 2);
    lambda = alloc_fvector(1, 2);

    /* Start motion estimation */

    for (y = 2; y < h - 2; y++) {
        for (x = 2; x < w - 2; x++) {
#ifdef DEBUG
            printf("\n**** Starting computation for Point: %d,%d ****\n", x, y);
#endif

            /* Get observation vector */
            n = 1;
            xx = x;		/* coordinates of center */
            yy = y;		/* of analysis window    */
            for (j = -2; j <= 2; j++)
                for (i = -2; i <= 2; i++)
                    r[n++] = img1[yy + j][xx + i];
            xx = x;		/* coordinates of center */
            yy = y;		/* of analysis window    */
            for (j = -2; j <= 2; j++)
                for (i = -2; i <= 2; i++)
                    r[n++] = img2[yy + j][xx + i];
#ifdef DEBUG
            printf("Observation Vector:\n");
            for (n = 1, n1 = 26, j = -2; j <= 2; j++) {
                for (i = -2; i <= 2; i++)
                    printf("%3.0f ", r[n++]);
                printf("     ");
                for (i = -2; i <= 2; i++)
                    printf("%3.0f ", r[n1++]);
                printf("\n");
            }
#endif

            /* Compute polynomial approximation coefficients */
            for (i = 1; i <= 9; i++) {
                s[i] = 0.0;
                for (j = 1; j <= 50; j++)
                    s[i] += Q[i][j] * r[j];
            }
#ifdef DEBUG
            printf("\nPolynomial Coefficients:\n");
            for (j = 1; j <= 9; j++)
                printf("%8.2f", s[j]);
            printf("\n");
            printf("Error in polynomial fit = %f\n", polyfit_error(r, s));
#endif

            /* Compute W and gama */
            W[1][1] = s[2] * s[2] + (4 * s[5] * s[5] + s[7] * s[7] + s[8] * s[8]) / 3;
            W[2][1] = s[2] * s[3] + (2 * s[5] * s[7] + 2 * s[6] * s[7] + s[8] * s[9]) / 3;
            W[1][2] = W[2][1];
            W[2][2] = s[3] * s[3] + (4 * s[6] * s[6] + s[7] * s[7] + s[9] * s[9]) / 3;

            gama[1] = -s[2] * s[4] - (2 * s[5] * s[8] + s[7] * s[9]) / 3;
            gama[2] = -s[3] * s[4] - (2 * s[6] * s[9] + s[7] * s[8]) / 3;

            compute_velocities(&dx[y][x], &dy[y][x], &conf, W, gama, lambda, phi);
            dx[y][x] *= 2.0;		/* We have estimated half the actual velocity */
            dy[y][x] *= 2.0;		/* since the frames were at t=-1 and t=1      */
        }
    }

    /* Free vectors and matrices */
    free_fvector(lambda, 1, 2);
    free_farray(phi, 1, 2, 1, 2);
    free_farray(W, 1, 2, 1, 2);
    free_fvector(gama, 1, 2);
    free_fvector(s, 1, 9);
    free_fvector(r, 1, 50);

#ifdef EXTEND
    /* Extend velocity fields */
    extend(vx, 2);
    extend(vy, 2);
#endif
}


float polyfit_error(float *r, float *s)
{
    int i, j, k, n;
    float poly, err = 0.0;

    n = 1;
    for (k = -1; k <= 1; k += 2)
        for (j = -2; j <= 2; j++)
            for (i = -2; i <= 2; i++) {
                poly = s[1] + s[2] * i + s[3] * j + s[4] * k + s[5] * i*i + s[6] * j*j
                    + s[7] * i*j + s[8] * i*k + s[9] * j*k;
                err += (r[n] - poly)*(r[n] - poly);
                n++;
            }
    return(err);
}


void readq(void)		/* Read in Q matrix */
{
    int i, j;
    FILE *qstream;

    Q = alloc_farray(1, 9, 1, 50);
    if (!(qstream = fopen("/home/grumpy/rh/ip/Q.mat", "r"))) {
        fprintf(stderr, "Couldn't open Q matrix file\n");
        exit(-1);
    }
    for (i = 1; i <= 9; i++)
        for (j = 1; j <= 50; j++) {
            fread(&Q[i][j], sizeof(Q[i][j]), 1, qstream);
        }
    fclose(qstream);
}

void printq(void)	/* Write Q matrix to stdout */
{
    int i, j, k;

    for (i = 1; i <= 9; i++) {
        printf("Row %d of Q matrix\n", i);
        for (j = 1; j <= 50;) {
            printf("   ");
            for (k = j + 5; j < k; j++)
                printf("%10.6f", Q[i][j]);
            printf("\n");
        }
    }
}



#define HALF_WINDOW (2)
#define WINDOW (2*HALF_WINDOW+1)

void lsq_grad(FIMAGE image1, FIMAGE image2, FIMAGE vx, FIMAGE vy)
{
    float **img1 = image1.ptr, **img2 = image2.ptr, **dx = vx.ptr, **dy = vy.ptr;
    float Fx, Fy, Ft, Fxl, Fyl, Ftl, Fxr, Fyr, Ftr, Fxx, Fxy, Fyy, Fxt, Fyt;
    float alpha_max, alpha_min;
    float **alloc_farray(), *alloc_fvector();
    int left, right;
    int w = image1.w, h = image1.h, x, y, xx, yy, i, j, k, n, n1, nrot;
    char file[MAX_STR_LEN];

    float conf;	/* just a dummy for now --spraxlo @@ */

 /* Reflect images */
    reflect(image1, Image_border, 3);
    reflect(image2, Image_border, 3);
    /* Zero out border on vx and vy */
    fset(vx, 0.0);
    fset(vy, 0.0);

    /* Allocate needed vectors and matrices */
    gama = alloc_fvector(1, 2);
    W = alloc_farray(1, 2, 1, 2);
    phi = alloc_farray(1, 2, 1, 2);
    lambda = alloc_fvector(1, 2);

    /* Start motion estimation */

    for (y = HALF_WINDOW; y < h - HALF_WINDOW; y++) {
        for (x = HALF_WINDOW; x < w - HALF_WINDOW; x++) {
#ifdef DEBUG 
            printf("\n**** Starting computation for Point: %d,%d ****\n", x, y);
#endif

            /* Get sum over window of derivatives */
            if (x == HALF_WINDOW) {
                Fxx = Fxy = Fyy = Fxt = Fyt = 0.0;
                for (j = -HALF_WINDOW, yy = y + j; j <= HALF_WINDOW; j++, yy++)
                    for (i = -HALF_WINDOW, xx = x + i; i <= HALF_WINDOW; i++, xx++) {
                        Fx = (img2[yy][xx + 1] - img2[yy][xx - 1]) / 2.0;
                        Fy = (img2[yy + 1][xx] - img2[yy - 1][xx]) / 2.0;
                        Ft = img1[yy][xx] - img2[yy][xx];
                        Fxx += Fx*Fx;
                        Fxy += Fx*Fy;
                        Fyy += Fy*Fy;
                        Fxt += Fx*Ft;
                        Fyt += Fy*Ft;
                    }
                left = 0; right = left + WINDOW;
            }
            else {
                for (j = -HALF_WINDOW, yy = y + j; j <= HALF_WINDOW; j++, yy++) {
                    Fxl = (img2[yy][left + 1] - img2[yy][left - 1]) / 2.0;
                    Fyl = (img2[yy + 1][left] - img2[yy - 1][left]) / 2.0;
                    Ftl = img1[yy][left] - img2[yy][left];
                    Fxr = (img2[yy][right + 1] - img2[yy][right - 1]) / 2.0;
                    Fyr = (img2[yy + 1][right] - img2[yy - 1][right]) / 2.0;
                    Ftr = img1[yy][right] - img2[yy][right];
                    Fxx += (Fxr*Fxr - Fxl*Fxl);
                    Fxy += (Fxr*Fyr - Fxl*Fyl);
                    Fyy += (Fyr*Fyr - Fyl*Fyl);
                    Fxt += (Fxr*Ftr - Fxl*Ftl);
                    Fyt += (Fyr*Ftr - Fyl*Ftl);
                }
                left++; right++;
            }

            /* Compute W and gama */
            W[1][1] = Fxx;
            W[2][1] = Fxy;
            W[1][2] = W[2][1];
            W[2][2] = Fyy;

            gama[1] = Fxt;
            gama[2] = Fyt;

            compute_velocities(&dx[y][x], &dy[y][x], &conf, W, gama, lambda, phi);

#ifdef LIMIT
            if (ABS(dx[y][x]) > HALF_WINDOW) {
                dx[y][x] = SIGN(HALF_WINDOW, dx[y][x]);
            }
            if (ABS(dy[y][x]) > HALF_WINDOW) {
                dy[y][x] = SIGN(HALF_WINDOW, dy[y][x]);
            }
#endif

        }
    }

    /* Free vectors and matrices */
    free_fvector(lambda, 1, 2);
    free_farray(phi, 1, 2, 1, 2);
    free_farray(W, 1, 2, 1, 2);
    free_fvector(gama, 1, 2);

#ifdef EXTEND
    /* Extend velocity fields */
    extend(vx, HALF_WINDOW);
    extend(vy, HALF_WINDOW);
#endif
}


void lsq_grad_fast(FIMAGE image1, FIMAGE image2, FIMAGE vx, FIMAGE vy)
{
    float **dx = vx.ptr, **dy = vy.ptr;
    float *fxx[WINDOW], *fxy[WINDOW], *fyy[WINDOW], *fxt[WINDOW], *fyt[WINDOW];
    float *oldxx, *oldxy, *oldyy, *oldxt, *oldyt;
    float *newxx, *newxy, *newyy, *newxt, *newyt, *Fxx, *Fxy, *Fyy, *Fxt, *Fyt;
    float alpha_max, alpha_min;
    float **alloc_farray(), *alloc_fvector(), *_alloc_fvector();
    int row;
    int w = image1.w, h = image1.h, x, y, xx, yy, i, j, k, n, n1, nrot;
    char file[MAX_STR_LEN];

    float conf;	/* just a dummy for now --spraxlo @@ */

 /* Reflect images */
    reflect(image1, Image_border, 3);
    reflect(image2, Image_border, 3);
    /* Zero out border on vx and vy */
    fset(vx, 0.0);
    fset(vy, 0.0);

    /* Allocate needed vectors and matrices */
    gama = alloc_fvector(1, 2);
    W = alloc_farray(1, 2, 1, 2);
    phi = alloc_farray(1, 2, 1, 2);
    lambda = alloc_fvector(1, 2);
    for (j = 0; j < WINDOW; j++) {
        fxx[j] = _alloc_fvector(w);
        fxy[j] = _alloc_fvector(w);
        fyy[j] = _alloc_fvector(w);
        fxt[j] = _alloc_fvector(w);
        fyt[j] = _alloc_fvector(w);
    }
    newxx = _alloc_fvector(w);   Fxx = _alloc_fvector(w);
    newxy = _alloc_fvector(w);   Fxy = _alloc_fvector(w);
    newyy = _alloc_fvector(w);   Fyy = _alloc_fvector(w);
    newxt = _alloc_fvector(w);   Fxt = _alloc_fvector(w);
    newyt = _alloc_fvector(w);   Fyt = _alloc_fvector(w);

    /* Start motion estimation */

    for (i = 0; i < w; i++)
        Fxx[i] = Fxy[i] = Fyy[i] = Fxt[i] = Fyt[i] = 0.0;
    for (j = 0; j < WINDOW - 1; j++) {
        row_grad_sum(image1, image2, j, fxx[j], fxy[j], fyy[j], fxt[j], fyt[j]);
        for (i = HALF_WINDOW; i < w - HALF_WINDOW; i++) {
            Fxx[i] += fxx[j][i];
            Fxy[i] += fxy[j][i];
            Fyy[i] += fyy[j][i];
            Fxt[i] += fxt[j][i];
            Fyt[i] += fyt[j][i];
        }
    }
    row = WINDOW - 1;
    for (i = 0; i < w; i++)
        fxx[row][i] = fxy[row][i] = fyy[row][i] = fxt[row][i] = fyt[row][i] = 0.0;

    for (y = HALF_WINDOW; y < h - HALF_WINDOW; y++) {
        row_grad_sum(image1, image2, y + HALF_WINDOW, newxx, newxy, newyy, newxt, newyt);
        oldxx = fxx[row];  oldxy = fxy[row];  oldyy = fyy[row];
        oldxt = fxt[row];  oldyt = fyt[row];
        for (x = HALF_WINDOW; x < w - HALF_WINDOW; x++) {
#ifdef DEBUG 
            printf("\n**** Starting computation for Point: %d,%d ****\n", x, y);
#endif

            /* Compute W and gama */
            W[1][1] = Fxx[x] += (newxx[x] - oldxx[x]);
            W[2][1] = Fxy[x] += (newxy[x] - oldxy[x]);
            W[1][2] = W[2][1];
            W[2][2] = Fyy[x] += (newyy[x] - oldyy[x]);

            gama[1] = Fxt[x] += (newxt[x] - oldxt[x]);
            gama[2] = Fyt[x] += (newyt[x] - oldyt[x]);

            compute_velocities(&dx[y][x], &dy[y][x], &conf, W, gama, lambda, phi);

#ifdef LIMIT
            if (ABS(dx[y][x]) > HALF_WINDOW) {
                dx[y][x] = SIGN(HALF_WINDOW, dx[y][x]);
            }
            if (ABS(dy[y][x]) > HALF_WINDOW) {
                dy[y][x] = SIGN(HALF_WINDOW, dy[y][x]);
            }
#endif

        }
        fxx[row] = newxx; newxx = oldxx;
        fxy[row] = newxy; newxy = oldxy;
        fyy[row] = newyy; newyy = oldyy;
        fxt[row] = newxt; newxt = oldxt;
        fyt[row] = newyt; newyt = oldyt;
        row++;  row %= WINDOW;
    }

    /* Free vectors and matrices */
    _free_fvector(Fyt);   _free_fvector(newyt);
    _free_fvector(Fxt);   _free_fvector(newxt);
    _free_fvector(Fyy);   _free_fvector(newyy);
    _free_fvector(Fxy);   _free_fvector(newxy);
    _free_fvector(Fxx);   _free_fvector(newxx);
    for (j = 0; j < WINDOW; j++) {
        _free_fvector(fyt[j]);
        _free_fvector(fxt[j]);
        _free_fvector(fyy[j]);
        _free_fvector(fxy[j]);
        _free_fvector(fxx[j]);
    }
    free_fvector(lambda, 1, 2);
    free_farray(phi, 1, 2, 1, 2);
    free_farray(W, 1, 2, 1, 2);
    free_fvector(gama, 1, 2);

#ifdef EXTEND
    /* Extend velocity fields */
    extend(vx, HALF_WINDOW);
    extend(vy, HALF_WINDOW);
#endif
}




void row_grad_sum(FIMAGE image1, FIMAGE image2, int row, float *fxx, float *fxy, float *fyy, float *fxt, float *fyt)
{
    float **img1 = image1.ptr, **img2 = image2.ptr;
    int x, xx, col;
    int w = image2.w, h = image2.h;
    float gxx[WINDOW], gxy[WINDOW], gyy[WINDOW], gxt[WINDOW], gyt[WINDOW];
    float Fx, Fy, Ft;

    fxx += HALF_WINDOW;  fxy += HALF_WINDOW;  fyy += HALF_WINDOW;
    fxt += HALF_WINDOW;  fyt += HALF_WINDOW;
    *fxx = *fxy = *fyy = *fxt = *fyt = 0.0;
    for (x = 0; x < WINDOW; x++) {
        Fx = (img2[row][x + 1] - img2[row][x - 1]) / 2.0;
        Fy = (img2[row + 1][x] - img2[row - 1][x]) / 2.0;
        Ft = img1[row][x] - img2[row][x];
        *fxx += gxx[x] = Fx*Fx;
        *fxy += gxy[x] = Fx*Fy;
        *fyy += gyy[x] = Fy*Fy;
        *fxt += gxt[x] = Fx*Ft;
        *fyt += gyt[x] = Fy*Ft;
    }
    col = 0;
    fxx++; fxy++; fyy++; fxt++; fyt++;
    for (x = HALF_WINDOW + 1, xx = x + HALF_WINDOW; x < w - HALF_WINDOW; x++, xx++) {
        *fxx = *(fxx - 1) - gxx[col];
        *fxy = *(fxy - 1) - gxy[col];
        *fyy = *(fyy - 1) - gyy[col];
        *fxt = *(fxt - 1) - gxt[col];
        *fyt = *(fyt - 1) - gyt[col];
        Fx = (img2[row][xx + 1] - img2[row][xx - 1]) / 2.0;
        Fy = (img2[row + 1][xx] - img2[row - 1][xx]) / 2.0;
        Ft = img1[row][xx] - img2[row][xx];
        *fxx++ += gxx[col] = Fx*Fx;
        *fxy++ += gxy[col] = Fx*Fy;
        *fyy++ += gyy[col] = Fy*Fy;
        *fxt++ += gxt[col] = Fx*Ft;
        *fyt++ += gyt[col] = Fy*Ft;
        col++;  col %= WINDOW;
    }
}



void lsq_lsqgrad(FIMAGE image1, FIMAGE image2, FIMAGE vx, FIMAGE vy)
{
    FIMAGE Gx, Gy, Gt;
    float **gx, **gy, **gt;
    float **img1 = image1.ptr, **img2 = image2.ptr, **dx = vx.ptr, **dy = vy.ptr;
    float Fx, Fy, Ft, Fxl, Fyl, Ftl, Fxr, Fyr, Ftr, Fxx, Fxy, Fyy, Fxt, Fyt;
    float alpha_max, alpha_min;
    float **alloc_farray(), *alloc_fvector();
    int left, right;
    int w = image1.w, h = image1.h, x, y, xx, yy, i, j, k, n, n1, nrot;
    char file[MAX_STR_LEN];

    float conf;	/* just a dummy for now --spraxlo @@ */

 /* Reflect images */
    reflect(image1, Image_border, 3);
    reflect(image2, Image_border, 3);
    /* Zero out border on vx and vy */
    fset(vx, 0.0);
    fset(vy, 0.0);

    /* Allocate needed vectors and matrices */
    gama = alloc_fvector(1, 2);
    W = alloc_farray(1, 2, 1, 2);
    phi = alloc_farray(1, 2, 1, 2);
    lambda = alloc_fvector(1, 2);

    /* Gradient Estimation */
    Gx.w = Gy.w = Gt.w = w;  Gx.h = Gy.h = Gt.h = h;
    alloc_fimage(&Gx);  alloc_fimage(&Gy);  alloc_fimage(&Gt);
    gradient_estimation(image1, image2, Gx, Gy, Gt);
    gx = Gx.ptr;  gy = Gy.ptr;  gt = Gt.ptr;

    /* Start motion estimation */

    for (y = HALF_WINDOW; y < h - HALF_WINDOW; y++) {
        for (x = HALF_WINDOW; x < w - HALF_WINDOW; x++) {
#ifdef DEBUG 
            printf("\n**** Starting computation for Point: %d,%d ****\n", x, y);
#endif

            /* Get sum over window of derivatives */
            if (x == HALF_WINDOW) {
                Fxx = Fxy = Fyy = Fxt = Fyt = 0.0;
                for (j = -HALF_WINDOW, yy = y + j; j <= HALF_WINDOW; j++, yy++)
                    for (i = -HALF_WINDOW, xx = x + i; i <= HALF_WINDOW; i++, xx++) {
                        Fx = gx[yy][xx];
                        Fy = gy[yy][xx];
                        Ft = gt[yy][xx];
                        Fxx += Fx*Fx;
                        Fxy += Fx*Fy;
                        Fyy += Fy*Fy;
                        Fxt += Fx*Ft;
                        Fyt += Fy*Ft;
                    }
                left = 0; right = left + WINDOW;
            }
            else {
                for (j = -HALF_WINDOW, yy = y + j; j <= HALF_WINDOW; j++, yy++) {
                    Fxl = gx[yy][left];
                    Fyl = gy[yy][left];
                    Ftl = gt[yy][left];
                    Fxr = gx[yy][right];
                    Fyr = gy[yy][right];
                    Ftr = gt[yy][right];
                    Fxx += (Fxr*Fxr - Fxl*Fxl);
                    Fxy += (Fxr*Fyr - Fxl*Fyl);
                    Fyy += (Fyr*Fyr - Fyl*Fyl);
                    Fxt += (Fxr*Ftr - Fxl*Ftl);
                    Fyt += (Fyr*Ftr - Fyl*Ftl);
                }
                left++; right++;
            }

            /* Compute W and gama */
            W[1][1] = Fxx;
            W[2][1] = Fxy;
            W[1][2] = W[2][1];
            W[2][2] = Fyy;

            gama[1] = Fxt;
            gama[2] = Fyt;

            compute_velocities(&dx[y][x], &dy[y][x], &conf, W, gama, lambda, phi);

#ifdef LIMIT
            if (ABS(dx[y][x]) > HALF_WINDOW) {
                dx[y][x] = SIGN(HALF_WINDOW, dx[y][x]);
            }
            if (ABS(dy[y][x]) > HALF_WINDOW) {
                dy[y][x] = SIGN(HALF_WINDOW, dy[y][x]);
            }
#endif

        }
    }

    /* Free up gradient images */
    free_fimage(Gt);  free_fimage(Gy);  free_fimage(Gx);

    /* Free vectors and matrices */
    free_fvector(lambda, 1, 2);
    free_farray(phi, 1, 2, 1, 2);
    free_farray(W, 1, 2, 1, 2);
    free_fvector(gama, 1, 2);

#ifdef EXTEND
    /* Extend velocity fields */
    extend(vx, HALF_WINDOW);
    extend(vy, HALF_WINDOW);
#endif
}


void lsq_lsqgrad_fast(FIMAGE image1, FIMAGE image2, FIMAGE vx, FIMAGE vy, FIMAGE conf)
{
    FIMAGE Gx, Gy, Gt;
    float **dx = vx.ptr, **dy = vy.ptr, **c = conf.ptr;
    float *fxx[WINDOW], *fxy[WINDOW], *fyy[WINDOW], *fxt[WINDOW], *fyt[WINDOW];
    float *oldxx, *oldxy, *oldyy, *oldxt, *oldyt;
    float *newxx, *newxy, *newyy, *newxt, *newyt, *Fxx, *Fxy, *Fyy, *Fxt, *Fyt;
    float alpha_max, alpha_min;
    float **alloc_farray(), *alloc_fvector(), *_alloc_fvector();
    int row;
    int w = image1.w, h = image1.h, x, y, xx, yy, i, j, k, n, n1, nrot;
    char file[MAX_STR_LEN];

    /* Reflect images */
    reflect(image1, Image_border, 3);
    reflect(image2, Image_border, 3);
    /* Zero out border on vx and vy */
    fset(vx, 0.0);
    fset(vy, 0.0);
    fset(conf, 0.0);

    /* Allocate needed vectors and matrices */
    gama = alloc_fvector(1, 2);
    W = alloc_farray(1, 2, 1, 2);
    phi = alloc_farray(1, 2, 1, 2);
    lambda = alloc_fvector(1, 2);
    for (j = 0; j < WINDOW; j++) {
        fxx[j] = _alloc_fvector(w);
        fxy[j] = _alloc_fvector(w);
        fyy[j] = _alloc_fvector(w);
        fxt[j] = _alloc_fvector(w);
        fyt[j] = _alloc_fvector(w);
    }
    newxx = _alloc_fvector(w);   Fxx = _alloc_fvector(w);
    newxy = _alloc_fvector(w);   Fxy = _alloc_fvector(w);
    newyy = _alloc_fvector(w);   Fyy = _alloc_fvector(w);
    newxt = _alloc_fvector(w);   Fxt = _alloc_fvector(w);
    newyt = _alloc_fvector(w);   Fyt = _alloc_fvector(w);

    /* Gradient Estimation */
    Gx.w = Gy.w = Gt.w = w;  Gx.h = Gy.h = Gt.h = h;
    alloc_fimage(&Gx);  alloc_fimage(&Gy);  alloc_fimage(&Gt);
    gradient_estimation(image1, image2, Gx, Gy, Gt);

    /* Start motion estimation */

    for (i = 0; i < w; i++)
        Fxx[i] = Fxy[i] = Fyy[i] = Fxt[i] = Fyt[i] = 0.0;
    for (j = 0; j < WINDOW - 1; j++) {
        row_lsqgrad_sum(Gx, Gy, Gt, j, fxx[j], fxy[j], fyy[j], fxt[j], fyt[j]);
        for (i = HALF_WINDOW; i < w - HALF_WINDOW; i++) {
            Fxx[i] += fxx[j][i];
            Fxy[i] += fxy[j][i];
            Fyy[i] += fyy[j][i];
            Fxt[i] += fxt[j][i];
            Fyt[i] += fyt[j][i];
        }
    }
    row = WINDOW - 1;
    for (i = 0; i < w; i++)
        fxx[row][i] = fxy[row][i] = fyy[row][i] = fxt[row][i] = fyt[row][i] = 0.0;

    for (y = HALF_WINDOW; y < h - HALF_WINDOW; y++) {
        row_lsqgrad_sum(Gx, Gy, Gt, y + HALF_WINDOW, newxx, newxy, newyy, newxt, newyt);
        oldxx = fxx[row];  oldxy = fxy[row];  oldyy = fyy[row];
        oldxt = fxt[row];  oldyt = fyt[row];
        for (x = HALF_WINDOW; x < w - HALF_WINDOW; x++) {
#ifdef DEBUG 
            printf("\n**** Starting computation for Point: %d,%d ****\n", x, y);
#endif

            /* Compute W and gama */
            W[1][1] = Fxx[x] += (newxx[x] - oldxx[x]);
            W[2][1] = Fxy[x] += (newxy[x] - oldxy[x]);
            W[1][2] = W[2][1];
            W[2][2] = Fyy[x] += (newyy[x] - oldyy[x]);

            gama[1] = Fxt[x] += (newxt[x] - oldxt[x]);
            gama[2] = Fyt[x] += (newyt[x] - oldyt[x]);

            compute_velocities(&dx[y][x], &dy[y][x], &c[y][x], W, gama, lambda, phi);

#ifdef LIMIT
            if (ABS(dx[y][x]) > HALF_WINDOW) {
                dx[y][x] = SIGN(HALF_WINDOW, dx[y][x]);
            }
            if (ABS(dy[y][x]) > HALF_WINDOW) {
                dy[y][x] = SIGN(HALF_WINDOW, dy[y][x]);
            }
#endif

        }
        fxx[row] = newxx; newxx = oldxx;
        fxy[row] = newxy; newxy = oldxy;
        fyy[row] = newyy; newyy = oldyy;
        fxt[row] = newxt; newxt = oldxt;
        fyt[row] = newyt; newyt = oldyt;
        row++;  row %= WINDOW;
    }

    /* Free up gradient images */
    free_fimage(Gt);  free_fimage(Gy);  free_fimage(Gx);

    /* Free vectors and matrices */
    _free_fvector(Fyt);   _free_fvector(newyt);
    _free_fvector(Fxt);   _free_fvector(newxt);
    _free_fvector(Fyy);   _free_fvector(newyy);
    _free_fvector(Fxy);   _free_fvector(newxy);
    _free_fvector(Fxx);   _free_fvector(newxx);
    for (j = 0; j < WINDOW; j++) {
        _free_fvector(fyt[j]);
        _free_fvector(fxt[j]);
        _free_fvector(fyy[j]);
        _free_fvector(fxy[j]);
        _free_fvector(fxx[j]);
    }
    free_fvector(lambda, 1, 2);
    free_farray(phi, 1, 2, 1, 2);
    free_farray(W, 1, 2, 1, 2);
    free_fvector(gama, 1, 2);

#ifdef EXTEND
    /* Extend velocity fields */
    extend(vx, HALF_WINDOW);
    extend(vy, HALF_WINDOW);
    extend(conf, HALF_WINDOW);
#endif
    fdivide(conf, (WINDOW*WINDOW) + 0.0 /* don't cast to float or double */);
}


void row_lsqgrad_sum(FIMAGE Gx, FIMAGE Gy, FIMAGE Gt, int row, float *fxx, float *fxy, float *fyy, float *fxt, float *fyt)
{
    float **gx = Gx.ptr, **gy = Gy.ptr, **gt = Gt.ptr;
    int x, xx, col;
    int w = Gx.w, h = Gx.h;
    float gxx[WINDOW], gxy[WINDOW], gyy[WINDOW], gxt[WINDOW], gyt[WINDOW];
    float Fx, Fy, Ft;

    fxx += HALF_WINDOW;  fxy += HALF_WINDOW;  fyy += HALF_WINDOW;
    fxt += HALF_WINDOW;  fyt += HALF_WINDOW;
    *fxx = *fxy = *fyy = *fxt = *fyt = 0.0;
    for (x = 0; x < WINDOW; x++) {
        Fx = gx[row][x];
        Fy = gy[row][x];
        Ft = gt[row][x];
        *fxx += gxx[x] = Fx*Fx;
        *fxy += gxy[x] = Fx*Fy;
        *fyy += gyy[x] = Fy*Fy;
        *fxt += gxt[x] = Fx*Ft;
        *fyt += gyt[x] = Fy*Ft;
    }
    col = 0;
    fxx++; fxy++; fyy++; fxt++; fyt++;
    for (x = HALF_WINDOW + 1, xx = x + HALF_WINDOW; x < w - HALF_WINDOW; x++, xx++) {
        *fxx = *(fxx - 1) - gxx[col];
        *fxy = *(fxy - 1) - gxy[col];
        *fyy = *(fyy - 1) - gyy[col];
        *fxt = *(fxt - 1) - gxt[col];
        *fyt = *(fyt - 1) - gyt[col];
        Fx = gx[row][xx];
        Fy = gy[row][xx];
        Ft = gt[row][xx];
        *fxx++ += gxx[col] = Fx*Fx;
        *fxy++ += gxy[col] = Fx*Fy;
        *fyy++ += gyy[col] = Fy*Fy;
        *fxt++ += gxt[col] = Fx*Ft;
        *fyt++ += gyt[col] = Fy*Ft;
        col++;  col %= WINDOW;
    }
}

#define BOTH_GRADIENTS
void gradient_estimation(FIMAGE image1, FIMAGE image2, FIMAGE Gx, FIMAGE Gy, FIMAGE Gt)
{
    float **img1 = image1.ptr, **img2 = image2.ptr;
    float **gx = Gx.ptr, **gy = Gy.ptr, **gt = Gt.ptr;
    int w = image1.w, h = image2.h, x, y;
    FIMAGE Scr;
    float **scr;

    Scr.w = w;  Scr.h = h;  alloc_fimage(&Scr);  scr = Scr.ptr;
    fsubtract_img(Scr, image1, image2);
    reflect(Scr, 3, 3);
    for (y = 0; y < h; y++)
        for (x = 0; x < w; x++) {
#ifdef BOTH_GRADIENTS
            gx[y][x] = (img2[y][x + 1] - img2[y][x - 1] +
                img1[y][x + 1] - img1[y][x - 1]) / 4.0;
            gy[y][x] = (img2[y + 1][x] - img2[y - 1][x] +
                img1[y + 1][x] - img1[y - 1][x]) / 4.0;
            gt[y][x] = (scr[y][x - 1] + scr[y][x + 1] + scr[y][x] +
                scr[y - 1][x] + scr[y + 1][x]) / 5.0;
#endif
#ifdef FIVE_GRADIENT
            gx[y][x] = (img2[y][x - 2] - 8.0*img2[y][x - 1] + 8.0*img2[y][x + 1]
                - img2[y][x + 2]) / 12.0;
            gy[y][x] = (img2[y - 2][x] - 8.0*img2[y - 1][x] + 8.0*img2[y + 1][x]
                - img2[y + 2][x]) / 12.0;
            gt[y][x] = (scr[y][x - 1] + scr[y][x + 1] + scr[y][x] +
                scr[y - 1][x] + scr[y + 1][x]) / 5.0;
#endif

#ifdef BOTH_FIVE_GRADIENT
            gx[y][x] = (img2[y][x - 2] - 8.0*img2[y][x - 1] + 8.0*img2[y][x + 1]
                - img2[y][x + 2] +
                img1[y][x - 2] - 8.0*img1[y][x - 1] + 8.0*img1[y][x + 1]
                - img1[y][x + 2]) / 24.0;
            gy[y][x] = (img2[y - 2][x] - 8.0*img2[y - 1][x] + 8.0*img2[y + 1][x]
                - img2[y + 2][x] +
                img1[y - 2][x] - 8.0*img1[y - 1][x] + 8.0*img1[y + 1][x]
                - img1[y + 2][x]) / 24.0;
            gt[y][x] = (scr[y][x - 1] + scr[y][x + 1] + scr[y][x] +
                scr[y - 1][x] + scr[y + 1][x]) / 5.0;
#endif

#ifdef THREE_GRADIENT
            gx[y][x] = (img2[y][x + 1] - img2[y][x - 1]) / 2.0;
            gy[y][x] = (img2[y + 1][x] - img2[y - 1][x]) / 2.0;
            gt[y][x] = scr[y][x];
#endif

        }
    free_fimage(Scr);
}

void gradient_mag(FIMAGE Gx, FIMAGE Gy, FIMAGE G)
{
    int x, y;

    for (y = 0; y < G.h; y++)
        for (x = 0; x < G.w; x++) {
            G.ptr[y][x] = (float)sqrt((double)(Gx.ptr[y][x] * Gx.ptr[y][x] +
                Gy.ptr[y][x] * Gy.ptr[y][x]));
        }
}




/*

#define LSQ_GRADIENTS
#define CENTERED_DIFFERENCE
gradient_estimation(image1,image2,Gx,Gy,Gt)
FIMAGE image1,image2;
FIMAGE Gx,Gy,Gt;
{
   float **img1=image1.ptr,**img2=image2.ptr;
   float **gx=Gx.ptr, **gy=Gy.ptr, **gt=Gt.ptr;
   int w=image1.w, h=image2.h, x,y;
   FIMAGE Scr;
   float **scr;

   Scr.w = w;  Scr.h = h;  alloc_fimage(&Scr);  scr = Scr.ptr;
   fsubtract_img(Scr,image1,image2);
   reflect(Scr,3,3);
   for (y=0; y < h; y++)
     for (x=0; x < w; x++) {
#ifdef LSQ_GRADIENTS
#ifdef CENTERED_DIFFERENCE
       gx[y][x] = (img2[y][x+1] - img2[y][x-1] +
                   img1[y][x+1] - img1[y][x-1])/4.0;
       gy[y][x] = (img2[y+1][x] - img2[y-1][x] +
                   img1[y+1][x] - img1[y-1][x])/4.0;
       gt[y][x] = (scr[y][x-1] + scr[y][x+1] + scr[y][x] +
                   scr[y-1][x] + scr[y+1][x])/5.0;
#else
       gx[y][x] = (img2[y][x] - img2[y][x-1] + img2[y-1][x] - img2[y-1][x-1] +
                  img1[y][x] - img1[y][x-1] + img1[y-1][x] - img1[y-1][x-1])/4.0;
       gy[y][x] = (img2[y][x] - img2[y-1][x] + img2[y][x-1] - img2[y-1][x-1] +
                  img1[y][x] - img1[y-1][x] + img1[y][x-1] - img1[y-1][x-1])/4.0;
       gt[y][x] = (scr[y][x] + scr[y-1][x] + scr[y][x-1] + scr[y-1][x-1])/4.0;
#endif
#else
#ifdef CENTERED_DIFFERENCE
       gx[y][x] = (img2[y][x+1] - img2[y][x-1])/2.0;
       gy[y][x] = (img2[y+1][x] - img2[y-1][x])/2.0;
       gt[y][x] = scr[y][x];
#else
       gx[y][x] = img2[y][x] - img2[y][x-1];
       gy[y][x] = img2[y][x] - img2[y-1][x];
       gt[y][x] = scr[y][x];
#endif
#endif
     }
   free_fimage(Scr);
}

*/


void blur(FIMAGE img, int nlev)
{
    FPYR pyr;
    int botlev = 0, toplev = nlev;
    extern FILTER rf, ef;

    def_fpyr(pyr, botlev, img.w, img.h);
    alloc_fpyr(pyr, botlev + 1, toplev);
    pyr[botlev].ptr = img.ptr;
    reduce(pyr, botlev, toplev, rf);
    expand(pyr, botlev, toplev, ef, 0);
    free_fpyr(pyr, botlev + 1, toplev);
}


void extend(FIMAGE img, int n)
{
    register float **p = img.ptr;
    register int l = 0, r = img.w - 1, b = 0, t = img.h - 1;
    register int i, j;

    for (j = 0; j < img.h; j++)
        for (i = 0; i < n; i++) {
            p[j][i] = p[j][n];
            p[j][r - i] = p[j][r - n];
        }
    for (j = 0; j < n; j++)
        for (i = 0; i < img.w; i++) {
            p[j][i] = p[n][i];
            p[t - j][i] = p[t - n][i];
        }
}


#define SMALL 5
void compute_velocities(float *vx, float *vy, float *conf, float **W, float *gama, float *lambda, float **phi)
{
    float alpha_max, alpha_min;
    int nrot;

#ifdef DEBUG
    printwgama();
#endif

    /* Get eigenvectors and eigenvalues of W */
    eigens(W, lambda, phi);
    /*  jacobi(W,2,lambda,phi,&nrot);*/
    /*  eigsrt(lambda,phi,2);*/

#ifdef DEBUG
    printeigens();
#endif

    /* Compute confidence */
    *conf = lambda[2];
    /* Compute velocities */
    if (lambda[1] > 100 * lambda[2] && lambda[1] > SMALL) {/* Singular point */
        alpha_max = (phi[1][1] * gama[1] + phi[2][1] * gama[2]) / lambda[1];
        *vx = alpha_max*phi[1][1];
        *vy = alpha_max*phi[2][1];
#ifdef DEBUG
        printf("Singular point: alpha_max=%f\n", alpha_max);
        printf("Velocity vector: %8.4f  %8.4f\n", *vx, *vy);
#endif
    }
    else if (lambda[1] > SMALL) {	/* Non-singular point */
        alpha_max = (phi[1][1] * gama[1] + phi[2][1] * gama[2]) / lambda[1];
        alpha_min = (phi[1][2] * gama[1] + phi[2][2] * gama[2]) / lambda[2];
        *vx = alpha_max*phi[1][1] + alpha_min*phi[1][2];
        *vy = alpha_max*phi[2][1] + alpha_min*phi[2][2];
#ifdef DEBUG
        printf("Non-singular point: alpha_max=%f alpha_min=%f\n",
            alpha_max, alpha_min);
        printf("Velocity vector: %8.4f  %8.4f\n", *vx, *vy);
#endif
    }
    else {
        *vx = 0.0;
        *vy = 0.0;
#ifdef DEBUG
        printf("Singular point: W=0\n");
#endif
    }
}

void printwgama(void)	/* Write W matrix and gama to stdout */
{

    printf("W matrix: %10.4f   %10.4f\n          %10.4f   %10.4f\n",
        W[1][1], W[1][2], W[2][1], W[2][2]);
    printf("Gamma:    %10.4f   %10.4f\n", gama[1], gama[2]);
}

void printeigens(void)	/* Write eigenvalues and eigenvectors to stdout */
{
    printf("Eigenvalues: %10.4f  %10.4f\n", lambda[1], lambda[2]);
    printf("Eigenvectors:\n");
    printf("   %10.4f   %10.4f\n", phi[1][1], phi[2][1]);
    printf("   %10.4f   %10.4f\n", phi[1][2], phi[2][2]);
}


void lsq_lsqgrad_fast_mask(FIMAGE image1, FIMAGE image2, FIMAGE mask, FIMAGE vx, FIMAGE vy, FIMAGE conf)
{
    FIMAGE Gx, Gy, Gt;
    float **msk = mask.ptr, **dx = vx.ptr, **dy = vy.ptr, **c = conf.ptr;
    float *fxx[WINDOW], *fxy[WINDOW], *fyy[WINDOW], *fxt[WINDOW], *fyt[WINDOW];
    float *oldxx, *oldxy, *oldyy, *oldxt, *oldyt;
    float *newxx, *newxy, *newyy, *newxt, *newyt, *Fxx, *Fxy, *Fyy, *Fxt, *Fyt;
    float alpha_max, alpha_min;
    float **alloc_farray(), *alloc_fvector(), *_alloc_fvector();
    int row;
    int w = image1.w, h = image1.h, x, y, xx, yy, i, j, k, n, n1, nrot;
    char file[MAX_STR_LEN];

    /* Reflect images */
    reflect(image1, Image_border, 3);
    reflect(image2, Image_border, 3);
    /* Zero out border on vx and vy */
    fset(vx, 0.0);
    fset(vy, 0.0);
    fset(conf, 0.0);

    /* Allocate needed vectors and matrices */
    gama = alloc_fvector(1, 2);
    W = alloc_farray(1, 2, 1, 2);
    phi = alloc_farray(1, 2, 1, 2);
    lambda = alloc_fvector(1, 2);
    for (j = 0; j < WINDOW; j++) {
        fxx[j] = _alloc_fvector(w);
        fxy[j] = _alloc_fvector(w);
        fyy[j] = _alloc_fvector(w);
        fxt[j] = _alloc_fvector(w);
        fyt[j] = _alloc_fvector(w);
    }
    newxx = _alloc_fvector(w);   Fxx = _alloc_fvector(w);
    newxy = _alloc_fvector(w);   Fxy = _alloc_fvector(w);
    newyy = _alloc_fvector(w);   Fyy = _alloc_fvector(w);
    newxt = _alloc_fvector(w);   Fxt = _alloc_fvector(w);
    newyt = _alloc_fvector(w);   Fyt = _alloc_fvector(w);

    /* Gradient Estimation */
    Gx.w = Gy.w = Gt.w = w;  Gx.h = Gy.h = Gt.h = h;
    alloc_fimage(&Gx);  alloc_fimage(&Gy);  alloc_fimage(&Gt);
    gradient_estimation(image1, image2, Gx, Gy, Gt);

    /* Start motion estimation */

    for (i = 0; i < w; i++)
        Fxx[i] = Fxy[i] = Fyy[i] = Fxt[i] = Fyt[i] = 0.0;
    for (j = 0; j < WINDOW - 1; j++) {
        row_lsqgrad_sum(Gx, Gy, Gt, j, fxx[j], fxy[j], fyy[j], fxt[j], fyt[j]);
        for (i = HALF_WINDOW; i < w - HALF_WINDOW; i++) {
            Fxx[i] += fxx[j][i];
            Fxy[i] += fxy[j][i];
            Fyy[i] += fyy[j][i];
            Fxt[i] += fxt[j][i];
            Fyt[i] += fyt[j][i];
        }
    }
    row = WINDOW - 1;
    for (i = 0; i < w; i++)
        fxx[row][i] = fxy[row][i] = fyy[row][i] = fxt[row][i] = fyt[row][i] = 0.0;

    for (y = HALF_WINDOW; y < h - HALF_WINDOW; y++) {
        row_lsqgrad_sum(Gx, Gy, Gt, y + HALF_WINDOW, newxx, newxy, newyy, newxt, newyt);
        oldxx = fxx[row];  oldxy = fxy[row];  oldyy = fyy[row];
        oldxt = fxt[row];  oldyt = fyt[row];
        for (x = HALF_WINDOW; x < w - HALF_WINDOW; x++) {
#ifdef DEBUG 
            printf("\n**** Starting computation for Point: %d,%d ****\n", x, y);
#endif

            /* Compute W and gama */
            W[1][1] = Fxx[x] += (newxx[x] - oldxx[x]);
            W[2][1] = Fxy[x] += (newxy[x] - oldxy[x]);
            W[1][2] = W[2][1];
            W[2][2] = Fyy[x] += (newyy[x] - oldyy[x]);

            gama[1] = Fxt[x] += (newxt[x] - oldxt[x]);
            gama[2] = Fyt[x] += (newyt[x] - oldyt[x]);

            if (msk[y][x] > 0.5) {
                compute_velocities(&dx[y][x], &dy[y][x], &c[y][x], W, gama, lambda, phi);
            }
            else {
                dx[y][x] = 0.0;
                dy[y][x] = 0.0;
                c[y][x] = 0.0;
            }

#ifdef LIMIT
            if (ABS(dx[y][x]) > HALF_WINDOW) {
                dx[y][x] = SIGN(HALF_WINDOW, dx[y][x]);
            }
            if (ABS(dy[y][x]) > HALF_WINDOW) {
                dy[y][x] = SIGN(HALF_WINDOW, dy[y][x]);
            }
#endif

        }
        fxx[row] = newxx; newxx = oldxx;
        fxy[row] = newxy; newxy = oldxy;
        fyy[row] = newyy; newyy = oldyy;
        fxt[row] = newxt; newxt = oldxt;
        fyt[row] = newyt; newyt = oldyt;
        row++;  row %= WINDOW;
    }

    /* Free up gradient images */
    free_fimage(Gt);  free_fimage(Gy);  free_fimage(Gx);

    /* Free vectors and matrices */
    _free_fvector(Fyt);   _free_fvector(newyt);
    _free_fvector(Fxt);   _free_fvector(newxt);
    _free_fvector(Fyy);   _free_fvector(newyy);
    _free_fvector(Fxy);   _free_fvector(newxy);
    _free_fvector(Fxx);   _free_fvector(newxx);
    for (j = 0; j < WINDOW; j++) {
        _free_fvector(fyt[j]);
        _free_fvector(fxt[j]);
        _free_fvector(fyy[j]);
        _free_fvector(fxy[j]);
        _free_fvector(fxx[j]);
    }
    free_fvector(lambda, 1, 2);
    free_farray(phi, 1, 2, 1, 2);
    free_farray(W, 1, 2, 1, 2);
    free_fvector(gama, 1, 2);

#ifdef EXTEND
    /* Extend velocity fields */
    extend(vx, HALF_WINDOW);
    extend(vy, HALF_WINDOW);
    extend(conf, HALF_WINDOW);
#endif
    fdivide(conf, (float)(WINDOW*WINDOW));
}


