#ifndef DITHERDEF
#define DITHERDEF

typedef struct dither {
    short *mat;
    int xsize, ysize;
    int nmat, nlevels, nshades;
    unsigned char *lut;
} dither;

#ifdef DEFDITHER
short dithbayer44[4][4] = {		/* Bayer matrix */
	0, 8, 2, 10,
	12, 4, 14, 6,
	3, 11, 1, 9,
	15, 7, 13, 5,
};

short dithmatdash44[4][4] = {
	0, 4, 2, 6,
	8, 12, 10, 14,
	3, 7, 1, 5,
	11, 15, 9, 13,
};

short dithlines44[4][4] = {
	0, 1, 2, 3,
	8, 9, 10, 11,
	4, 5, 6, 7,
	12, 13, 14, 15,
};

short dithbayer88[8][8] = {		/* Bayer matrix */
	0,	8,	36,	44,	2,	10,	38,	46,
	16,	24,	52,	60,	18,	26,	54,	62,
	32,	40, 	4,	12,	34,	42,	6,	14,
	48,	56,	20,	28,	50,	58,	22,	30,
	3,	11,	39,	47,	1,	9,	37,	45,
	19,	27,	55,	63,	17,	25,	53,	61,
	35,	43,	7,	15,	33,	41,	5,	13,
	51,	59,	23,	31,	49,	57,	21,	29,
};

short dithdots88[8][8] = {	/* halftone dots */
	3,	17,	55,     63,	61,     47,	9,	1,
	15, 	29,	39,	51,	49,	35,	25,	13,
	40,	32,	26,	20,	22,	30,	36,	42,
	56,	44,	10,	4,	6,	18,	52,	58,
	60,	46,	8,	0,	2,	16,	54,	62,
	48,	34,	24,	12,	14,	28,	38,	50,
	23,	31,	37,	43,	41,	33,	27,	21,
	7,	19,	53,	59,	57,	45,	11,	5,
};

short dithfloyd88[8][8] = {	
	0,	32,	8,	40,	2,	34,	10,	42,
	48,	16,	56,	24,	50,	18,	58,	26,
	12,	44, 	4,	36,	14,	46,	6,	38,
	60,	28,	52,	20,	62,	30,	54,	22,
	3,	35,	11,	43,	1,	33,	9,	41,
	51,	19,	59,	27,	49,	17,	57,	25,
	15,	47,	7,	39,	13,	45,	5,	37,
	63,	31,	55,	23,	61,	29,	53,	21,
};

short dithfax88[8][8] = {	/* FAX dither */
	0,	8,	36,	44,	2,	10,	38,	46,
	16,	24,	52,	60,	18,	26,	54,	62,
	32,	40, 	4,	12,	34,	42,	6,	14,
	48,	56,	20,	28,	50,	58,	22,	30,
	3,	11,	39,	47,	1,	9,	37,	45,
	19,	27,	55,	63,	17,	25,	53,	61,
	35,	43,	7,	15,	33,	41,	5,	13,
	51,	59,	23,	31,	49,	57,	21,	29,
};
#else
extern short dithbayer44[4][4];
extern short dithmatdash44[4][4];
extern short dithlines44[4][4];
extern short dithbayer88[8][8];
extern short dithdots88[8][8];
extern short dithfloyd88[8][8];
extern short dithfax88[8][8];
#endif

dither *newdither();

#endif

