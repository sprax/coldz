/* Filter I/O routines */

#include <stdlib.h>
#include "sarnoff/imgdec.h"
#ifndef DEFAULT_PYR_FILTER
#define DEFAULT_PYR_FILTER "/home/cbcl/amnon/rh/pyrg.fir"
#define DEFAULT_QMF_FILTER "/home/cbcl/amnon/rh/qmf7.fir"
#endif


/*****************************************************************************/

getpyrfilters(char *filename, FILTER *rf, FILTER *ef)
{
    FILE *stream;

    if ((stream = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "getpyrfilters: Couldn't open filter file %s\n", filename);
        return(0);
    }
    read_1d_sym_filter(rf, stream);
    read_1d_sym_filter(ef, stream);
    fclose(stream);
    return(1);
}

/*****************************************************************************/

getqmffilters(char *filename, FILTER *rlf, FILTER *rhf, FILTER *elf, FILTER *ehf)
{
    FILE *stream;
    int i;

    if ((stream = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "getqmffilters: Couldn't open filter file %s\n", filename);
        return(0);
    }
    read_1d_sym_filter(rlf, stream);
    read_1d_sym_filter(rhf, stream);
    read_1d_sym_filter(elf, stream);
    for (i = -(elf->taps - 1) / 2; i <= elf->taps / 2; i++)
        elf->k[i] *= 2.0;
    read_1d_sym_filter(ehf, stream);
    for (i = -(ehf->taps - 1) / 2; i <= ehf->taps / 2; i++)
        ehf->k[i] *= 2.0;
    fclose(stream);
    return(1);
}

/*****************************************************************************/

get_1d_filter(char *filename, FILT1D *f)
{
    FILE *stream;
    int i;

    if ((stream = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "get_1d_filter: Couldn't open filter file %s\n", filename);
        return(0);
    }
    read_1d_filter(f, stream);
    fclose(stream);
    return(1);
}

/*****************************************************************************/

get_1d_sym_filter(char *filename, FILTER *f)
{
    FILE *stream;
    int i;

    if ((stream = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "get_1d_sym_filter: Couldn't open filter file %s\n", filename);
        return(0);
    }
    read_1d_sym_filter(f, stream);
    fclose(stream);
    return(1);
}

/*****************************************************************************/

get_2d_filter(char *filename, FILT2D *f)
{
    FILE *stream;
    int i;

    if ((stream = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "get_2d_filter: Couldn't open filter file %s\n", filename);
        return(0);
    }
    read_2d_filter(f, stream);
    fclose(stream);
    return(1);
}

/*****************************************************************************/

read_1d_filter(FILT1D *f, FILE *stream)
{
    int i;
    double val, scale;

    fscanf_s(stream, "%d:%d %lf", &(f->l), &(f->r), &scale);
    f->k = alloc_fvector(f->l, f->r);
    for (i = f->l; i <= f->r; i++) {
        fscanf_s(stream, "%lf", &val);
        f->k[i] = (float)(val / scale);
    }
}

/*****************************************************************************/

read_1d_sym_filter(FILTER *f, FILE *stream)
{
    register float *p, *q;
    int ntaps, i, itap;
    double val, scale;

    fscanf_s(stream, "%d %lf", &ntaps, &scale);
    p = (float *)malloc(ntaps * sizeof(*p));
    for (i = 0; i < ntaps; i++) {
        fscanf_s(stream, "%d %lf", &itap, &val);
        p[itap - 1] = (float)(val / scale);
    }
    f->k = p + (ntaps - 1) / 2;
    f->taps = ntaps;
}

/*****************************************************************************/

read_2d_filter(FILT2D *f, FILE *stream)
{
    int i, j;
    double val, scale;

    fscanf_s(stream, "%d:%d %d:%d %lf", &(f->l), &(f->r), &(f->b), &(f->t), &scale);
    f->k = alloc_farray(f->b, f->t, f->l, f->r);
    for (j = f->b; j <= f->t; j++)
        for (i = f->l; i <= f->r; i++) {
            fscanf_s(stream, "%lf", &val);
            f->k[j][i] = (float)(val / scale);
        }
}

/*****************************************************************************/



