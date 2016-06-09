/*
 * $Id: bm.h,v 0.87 1993/10/11 00:00:00 zhao Pre-Release $
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
 * Bitmaps for marking
 */
#ifndef BM_ICLUDED
#define BM_ICLUDED

#define BM_W  17
#define BM_H  17

/* a horizontal line */
static char line_bits[] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xfe, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00};

/* A plus (+) sign */
static char plus_bits[] =
{
    0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00,
    0xff, 0xff, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x01, 0x00};


/* Distance indicater |-----| */
static char hdist_bits[] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01,
    0xff, 0xff, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00};

/* an open circle */
static char circ_bits[] =
{
    0xc0, 0x07, 0x00, 0xf0, 0x1f, 0x00, 0x3c, 0x78, 0x00, 0x0c, 0x60, 0x00,
    0x06, 0xc0, 0x00, 0x06, 0xc0, 0x00, 0x03, 0x80, 0x01, 0x03, 0x80, 0x01,
    0x03, 0x80, 0x01, 0x03, 0x80, 0x01, 0x03, 0x80, 0x01, 0x06, 0xc0, 0x00,
    0x06, 0xc0, 0x00, 0x0c, 0x60, 0x00, 0x3c, 0x78, 0x00, 0xf0, 0x1f, 0x00,
    0xc0, 0x07, 0x00};

/* a filled circle */
static char fcirc_bits[] =
{
    0xc0, 0x07, 0x00, 0xf0, 0x1f, 0x00, 0xfc, 0x7f, 0x00, 0xfc, 0x7f, 0x00,
    0xfe, 0xff, 0x00, 0xfe, 0xff, 0x00, 0xff, 0xff, 0x01, 0xff, 0xff, 0x01,
    0xff, 0xff, 0x01, 0xff, 0xff, 0x01, 0xff, 0xff, 0x01, 0xfe, 0xff, 0x00,
    0xfe, 0xff, 0x00, 0xfc, 0x7f, 0x00, 0xfc, 0x7f, 0x00, 0xf0, 0x1f, 0x00,
    0xc0, 0x07, 0x00};

/* a rectangle */
static char rect_bits[] =
{
    0xff, 0xff, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01,
    0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01,
    0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01,
    0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01,
    0xff, 0xff, 0x01};

/* a filled rectangle */
static char frect_bits[] =
{
    0xff, 0xff, 0x01, 0xff, 0xff, 0x01, 0xff, 0xff, 0x01, 0xff, 0xff, 0x01,
    0xff, 0xff, 0x01, 0xff, 0xff, 0x01, 0xff, 0xff, 0x01, 0xff, 0xff, 0x01,
    0xff, 0xff, 0x01, 0xff, 0xff, 0x01, 0xff, 0xff, 0x01, 0xff, 0xff, 0x01,
    0xff, 0xff, 0x01, 0xff, 0xff, 0x01, 0xff, 0xff, 0x01, 0xff, 0xff, 0x01,
    0xff, 0xff, 0x01};

/* a open triangle */
static char tri_bits[] =
{
    0x00, 0x01, 0x00, 0x00, 0x03, 0x00, 0x80, 0x02, 0x00, 0x80, 0x04, 0x00,
    0x40, 0x04, 0x00, 0x40, 0x08, 0x00, 0x20, 0x08, 0x00, 0x20, 0x10, 0x00,
    0x10, 0x10, 0x00, 0x10, 0x20, 0x00, 0x08, 0x20, 0x00, 0x08, 0x40, 0x00,
    0x04, 0x40, 0x00, 0x04, 0x80, 0x00, 0x02, 0x80, 0x00, 0x02, 0x00, 0x01,
    0xff, 0xff, 0x01};

/* a filled triangle */
static char ftri_bits[] =
{
    0x00, 0x01, 0x00, 0x00, 0x03, 0x00, 0x80, 0x03, 0x00, 0x80, 0x07, 0x00,
    0xc0, 0x07, 0x00, 0xc0, 0x0f, 0x00, 0xe0, 0x0f, 0x00, 0xe0, 0x1f, 0x00,
    0xf0, 0x1f, 0x00, 0xf0, 0x3f, 0x00, 0xf8, 0x3f, 0x00, 0xf8, 0x7f, 0x00,
    0xfc, 0x7f, 0x00, 0xfc, 0xff, 0x00, 0xfe, 0xff, 0x00, 0xfe, 0xff, 0x01,
    0xff, 0xff, 0x01};

/* a open 5-sided star */
static char star_bits[] =
{
    0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x80, 0x02, 0x00, 0x80, 0x02, 0x00,
    0x80, 0x02, 0x00, 0x40, 0x04, 0x00, 0x7e, 0xfc, 0x00, 0x04, 0x40, 0x00,
    0x18, 0x30, 0x00, 0x20, 0x08, 0x00, 0x20, 0x09, 0x00, 0x80, 0x12, 0x00,
    0x50, 0x14, 0x00, 0x30, 0x38, 0x00, 0x18, 0x20, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00};

/* a filled 5-sided star */
static char fstar_bits[] =
{
    0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x80, 0x03, 0x00, 0x80, 0x03, 0x00,
    0x80, 0x03, 0x00, 0xc0, 0x07, 0x00, 0xfe, 0xff, 0x00, 0xfc, 0x7f, 0x00,
    0xf8, 0x3f, 0x00, 0xf0, 0x0f, 0x00, 0xe0, 0x0f, 0x00, 0xe0, 0x1f, 0x00,
    0x70, 0x1c, 0x00, 0x38, 0x38, 0x00, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00};

/* a line with a small triangle on on end */
static char arrow_bits[] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0xe0, 0x00,
    0xff, 0xff, 0x01, 0x00, 0xe0, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00};
#endif
