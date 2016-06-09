/*
 * $Id: symbol.h,v 0.87 1993/10/11 00:00:00 zhao Pre-Release $
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
 * Defination for default fonts and symbols. Only need to be
 * included by text.c.
 * This stuff definately should not be hard-coded
 *
 */
#ifndef BIT_SYMBOL_H
#define BIT_SYMBOL_H

/*
 * default fonts.
 * font index 0 is the default font, So it is a good idea to set
 * the first one to be a mono-spaced font. Also, symbol font
 * must present in order for font switching to work
 */
static char *fonts[] =
{
    "Courier",
    "Times-Roman", "Times-Bold", "Times-Italic", "Times-BoldItalic",
    "Helvetica-Oblique", "Helvetica-Bold", "Helvetica",
    "Courier-Bold", "Courier-BoldOblique", "Courier-Oblique",
    "Symbol", "Boston", "Screen", "Screen-Bold",
    "Charter-Black", "Charter-Italic", "Charter-BlackItalic",
    "cursor"
};

static totalfont = sizeof(fonts) / sizeof(fonts[0]);
/*
 * Standard PS encoding for symbol font
 */
typedef struct {
    const char *name;
    const char *code;
} SymCode_t;

static const SymCode_t msymcode[] =
{
    {"forall", "\42"},
    {"universal", "\42"},
    {"Delta", "D"},
    {"Phi", "G"},
    {"Gamma", "G"},
    {"Lambda", "L"},
    {"Pi", "P"},
    {"Theta", "Q"},
    {"Sigma", "S"},
    {"sum", "S"},
    {"Omega", "W"},
    {"Psi", "Y"},
    {"therefore", "\\"},
    {"perp", "^"},
    {"alpha", "a"},
    {"beta", "b"},
    {"chi", "c"},
    {"delta", "d"},
    {"epsilon", "e"},
    {"phi", "j"},
    {"gamma", "g"},
    {"eta", "h"},
    {"psi", "i"},
    {"kappa", "j"},
    {"lambda", "l"},
    {"mu", "m"},
    {"nu", "n"},
    {"pi", "p"},
    {"theta", "q"},
    {"rho", "r"},
    {"sigma", "s"},
    {"tau", "t"},
    {"omega", "w"},
    {"xi", "x"},
    {"Psi", "y"},
    {"le", "\243"},
    {"lessequal", "\243"},
    {"infty", "\244"},
    {"infinity", "\244"},
    {"club", "\247"},
    {"diamond", "\250"},
    {"heart", "\251"},
    {"spade", "\252"},
    {"uparrow", "\255"},
    {"downarrow", "\257"},
    {"leftarrow", "\254"},
    {"rightarrow", "\256"},
    {"pm", "\262"},
    {"plusmqimus", "\262"},
    {"ge", "\263"},
    {"greatequal", "\263"},
    {"times", "\264"},
    {"multiply", "\264"},
    {"prop", "\265"},
    {"partial", "\266"},
    {"bullet", "\267"},
    {"ne", "\271"},
    {"approx", "\273"},
    {"pluscirc", "\305"},
    {"crosscirc", "\304"},
    {"register", "\322"},
    {"copyright", "\323"},
    {"Leftarrow", "\334"},
    {0, 0}                      /* sentinel */
};

/* non-math symbols. ISOLatin1 enconding */
static const SymCode_t symcode[] =
{
    {"AA", "\305"},
    {"aa", "\344"},
    {0, 0}
};

#include <string.h>
#include <stdlib.h>
/* see is a string is a number */
static const char *check_number(const char *name)
{
    static char tmpstr[50];
    char  *where;
    int    code;
    code = strtol(name, &where, 8);
    if (where == name || code == 0)
        return name;
    tmpstr[0] = code;
    tmpstr[1] = '\0';
    return strncat(tmpstr, where, sizeof(tmpstr) - 3);
}
/* given a math symbol name, returns the code or letter */
static const char *mathcode(const char *name)
{
    register const SymCode_t *p;

    for (p = msymcode; p->name; p++)
        if (p->name[0] == *name && strcmp(name, p->name) == 0)
            return p->code;
    /* not a predefined name, check if a number */
    return check_number(name);
}

static const char *spcode(const char *name)
{
    register const SymCode_t *p;

    for (p = symcode; p->name; p++)
        if (p->name[0] == *name && strcmp(name, p->name) == 0)
            return p->code;
    /* not a predefined name, check if a number */
    return check_number(name);
}
#endif


