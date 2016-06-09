/*
 * $Id: curdef.h,v 0.87 1993/10/11 00:00:00 zhao Pre-Release $
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
 * Definations for all cursors used.
 *
 */
#ifndef BIT_CURDEF_H
#define BIT_CURDEF_H


/*
 * A 16X16 with one or two colors. Used to indicate short busy.
 */
static unsigned short hourglass[] =
{
    0xFFFF, 0x47E2, 0x23C4, 0x1188, 0x0890, 0x0420, 0x02C0, 0x02C0,
    0x02C0, 0x02C0, 0x06E0, 0x0810, 0x1008, 0x2004, 0x4002, 0xFFFF
#ifndef MONO_CURSOR
    ,0x0000, 0X381C, 0X1C38, 0X0E70, 0X0760, 0X03C0, 0X0100, 0X0100,
    0X0100, 0X0100, 0X0100, 0X07E0, 0X0FF0, 0X1FF8, 0X3FFC, 0x0000
#endif
};

/*
 * A 16X16 square cursor ( with 8X8 solid cell )
 */
static unsigned short SmallSquare[] =
{
    0x0000, 0x0000, 0x0000, 0x0000, 0x0FF0, 0x0FF0, 0x0FF0, 0x0FF0,
    0x0FF0, 0x0FF0, 0x0FF0, 0x0FF0, 0x0000, 0x0000, 0x0000, 0x0000
};

/*
 * A 16X16 two-colored hand
 */
static unsigned short hand_cur[] =
{
/* outline */
    0x0000, 0x0040, 0x00A0, 0x0110, 0x0248, 0x0304, 0x0C82, 0x1044,
    0x0E28, 0x1010, 0x0E10, 0x1010, 0x7E20, 0x8040, 0x7F80, 0x0000
#ifndef MONO_CURSOR
    ,0x0000, 0x0000, 0x0040, 0x00E0, 0x01B0, 0x00F8, 0x037C, 0x0FB8,
    0x01D0, 0x0FE0, 0x01E0, 0x0FE0, 0x01C0, 0x7F80, 0x0000, 0x0000
#endif
};

/*
 * A 32X32 monocolored cross
 */
static unsigned short MedCross[] =
{
    0x0001, 0x0000, 0x0001, 0x0000, 0x0001, 0x0000, 0x0001, 0x0000,
    0x0001, 0x0000, 0x0001, 0x0000, 0x0001, 0x0000, 0x0001, 0x0000,
    0x0001, 0x0000, 0x0001, 0x0000, 0x0001, 0x0000, 0x0001, 0x0000,
    0x0001, 0x0000, 0x0001, 0x0000, 0x0001, 0x0000, 0xFFFF, 0xFFFF,
    0x0001, 0x0000, 0x0001, 0x0000, 0x0001, 0x0000, 0x0001, 0x0000,
    0x0001, 0x0000, 0x0001, 0x0000, 0x0001, 0x0000, 0x0001, 0x0000,
    0x0001, 0x0000, 0x0001, 0x0000, 0x0001, 0x0000, 0x0001, 0x0000,
    0x0001, 0x0000, 0x0001, 0x0000, 0x0001, 0x0000, 0x0001, 0x0000
};
/*
 * A 16X16 monocolored cross
 */
static unsigned short SmallCross[] =
{
    0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0xFFFE,
    0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100
};


/*
 * the following 4 cursors should be displayed in sequence to get a rotating
 * circle
 */
static unsigned short tl1[] =
{
    0x03C0, 0x0CF0, 0x10F8, 0x20FC, 0x40FE, 0x40FE, 0x80FF, 0x80FF,
    0xFF01, 0xFF01, 0x7F02, 0x7F02, 0x3F04, 0x1F08, 0x0F30, 0x03C0
#ifndef MONO_CURSOR
    ,0X0000, 0X0300, 0X0F00, 0X1F00, 0X3F00, 0X3F00, 0X7F00, 0X7F00,
    0X00FE, 0X00FE, 0X00FC, 0X00FC, 0X00F8, 0X00F0, 0X00C0, 0X0000
#endif
};
static unsigned short tl2[] =
{
    0x03C0, 0x0FF0, 0x1FF8, 0x3FFC, 0x4FF2, 0x47E2, 0x83C1, 0x8181,
    0x8181, 0x83C1, 0x47E2, 0x4FF2, 0x3FFC, 0x1FF8, 0x0FF0, 0x03C0
#ifndef MONO_CURSOR
    ,0X0000, 0X0000, 0X0000, 0X0000, 0X300C, 0X381C, 0X7C3E, 0X7E7E,
    0X7E7E, 0X7C3E, 0X381C, 0X300C, 0X0000, 0X0000, 0X0000, 0X0000
#endif
};
static unsigned short tl3[] =
{
    0x03C0, 0x0F30, 0x1F08, 0x2F04, 0x7F02, 0x7F02, 0xFF01, 0xFF01,
    0x80FF, 0x80FF, 0x40FE, 0x40FE, 0x20FC, 0x10F8, 0x0CF0, 0x03C0
#ifndef MONO_CURSOR
    ,0X0000, 0X00C0, 0X00F0, 0X00F8, 0X00FC, 0X00FC, 0X00FE, 0X00FE,
    0X7F00, 0X7F00, 0X3F00, 0X3F00, 0X1F00, 0X0F00, 0X0300, 0X0000
#endif
};
static unsigned short tl4[] =
{
    0x03C0, 0x0C30, 0x1008, 0x300C, 0x781E, 0x7C3E, 0xFE7F, 0xFFFF,
    0xFFFF, 0xFE7F, 0x7C3E, 0x781E, 0x300C, 0x1008, 0x0C30, 0x03C0
#ifndef MONO_CURSOR
    ,0X0000, 0X03C0, 0X0FF0, 0X0FF0, 0X07E0, 0X03C0, 0X0180, 0X0000,
    0X0000, 0X0180, 0X03C0, 0X07E0, 0X0FF0, 0X0FF0, 0X03C0, 0X0000
#endif
};
#endif
