/*
 * $Id: uproto.h,v 0.87 1993/10/11 00:00:00 zhao Pre-Release $
 *
 *. This file is part of BIT package.
 *
 *
 *  Copyright(c) 1993 by T.C. Zhao.  All rights reserved.
 *
 *  Permission to do anything with this software non-commercial and
 *  without fee is hereby granted, provided that the copyright notice
 *  and disclaimer appear in all copies and their documentation.
 *
 *  This software is provided "as is" without expressed or implied
 *  warranty of any kind.
 *.
 *
 *  Prototypes for the utilities
 */

#ifndef BIT_UPROTO_H
#define BIT_UPROTO_H

#include "utype.h"

/*********** matrix operations  **************/

extern void set_use_calloc(int);
extern void *dbg_getmat(int, int, size_t, const char *, int);

#ifndef M_DBG
extern void *get_mat(int, int, size_t);
#else
#ifndef get_mat
#define get_mat(a,b,c)   dbg_getmat(a,b,c,__FILE__,__LINE__)
#endif
#endif

extern void free_mat(void *);
extern void *get_3d(int, int, int, size_t);
extern void free_3d(void *);
extern void *get_submat(void *, int, int, int, int, int, int, size_t);
extern int put_submat(void *, int, int, void *, int, int, int, int, size_t);
extern int flip_mat(void *, int, int, int, size_t);
extern void *rotate_mat(void *, int, int, int, size_t);
extern void set_matop_quiet(int);
extern void fill_submat(void *, int, int, int, int, rgba_t, size_t);
extern void set_mat_op(Matop_t);
extern Matop_t get_mat_op(void);
extern void set_magic_pix(rgba_t);
extern rgba_t get_magic_pix(void);
extern void pack_mat(rgba_t *, pc_t *, pc_t *, pc_t *, long);
extern void unpack_mat(rgba_t *, pc_t *, pc_t *, pc_t *, long);
extern void init_mat(void *, int, int, size_t, unsigned long);

/******* cmaps *********/
extern void set_colmap(int, pc_t *, pc_t *, pc_t *);
extern void set_cmap(CMPTR);
extern void get_colmap(int, pc_t *, pc_t *, pc_t *);
extern void modify_cmap(CMPTR, pc_t *, pc_t *, pc_t *);
extern void pack_cmap(CMPTR);
extern void clean_up(void);
extern int cmap_ucolors(CMPTR, pc_t *);

#include "mac.h"
#endif /* !U_PROTO_H_ */

