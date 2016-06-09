/*
 * $Id: tsdef.h,v 0.87 1993/10/11 00:00:00 zhao Pre-Release $
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
 * Structures used by ps.c and text.c
 *
 */
#ifndef TS_DEF_H
#define TS_DEF_H

typedef struct
  {
      char *str;		/* current token   */
      float size;		/* requested size  */
      short isize;		/* index           */
      short math;		/* if symbol fonts */
  }
Token;

#define MAXTOK 30		/* tokens per line */
typedef struct
  {
      int ntokens;
      Token token[MAXTOK];
#if 0
      const char *fontname;
#endif
  }
Line;

/* o left 1 center 2 right */
enum
  {
      TLEFT, TCENTER, TRIGHT
  };

extern Line *get_text_line(int);

#endif

