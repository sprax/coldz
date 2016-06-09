/******************************************************************************
 imagic.h    S.R.Lines    93.5.25-7.15
 All the image file format magic numbers I know, even the ones I made up myself
 For "types" attributes of image data in memory structs, see imagmem.h.
******************************************************************************/
#ifndef imagic_h__
#define imagic_h__

#define	IMAGIC_BEYMER	32		/* David Beymer's homebrew */ 
#define	IMAGIC_GROK	1
#define	IMAGIC_PBM	"P1"
#define	IMAGIC_PGM	"P2"
#define	IMAGIC_RAS	0x59a66a95	/* Sun Rasterfile _RT_NORMAL ? */
#define	IMAGIC_SGI	0732	   /* SGI rgb (Paul Haeberli's IMAGIC) */
#define	IMAGIC_VIS	"VISF="

/******************************************
The following synonyms are here for compatibilty with code based on David
Beymer's image_t struct.  They are commented out of my version of his image.h.
*******************************************/
#define DUMP_MAGIC_NUMBER	IMAGIC_BEYMER
#define GROK_MAGIC_NUMBER	IMAGIC_GROK
#define PBM_MAGIC_NUMBER        IMAGIC_PBM
#define PGM_MAGIC_NUMBER        IMAGIC_PGM
#define RAS_MAGIC_NUMBER        IMAGIC_RAS
#define SGI_MAGIC_NUMBER        IMAGIC_SGI
#define VIS_MAGIC_NUMBER        IMAGIC_VIS


/****** FF-Numbers are historical accidents, bothersome to change
        because they index various arrays. ********/
#define FF_UNKNOWN       0
#define FF_A_RESERVE     1
#define FF_B_RESERVE     1
#define FF_C_RESERVE     1
#define FF_E_RESERVE     1
#define FF_G_RESERVE     1
#define FF_H_RESERVE     1
#define FF_I_RESERVE     1
#define FF_J_RESERVE     1
#define FF_K_RESERVE     1
#define FF_L_RESERVE     1
#define FF_ALIAS         1
#define FF_ALIAS         1
#define FF_ARC           2
#define FF_APS           3
#define FF_ASCII         4	/* That's right, ASCII-as-B&W */
#define FF_BEYMER        5
#define FF_BITPACKED     6
#define FF_CANVAS        7
#define FF_CMAP          8
#define FF_COREL         9
#define FF_D_RESERVE     10	/* @@ breakdown */
#define FF_CUBICOMP      10	/* Cubicomp/Vertigo, aka cube */
#define FF_DATACUBE      11
#define FF_EPS	         12
#define FF_F_RESERVE     13	/* UNIX faceserver */
#define FF_FACE	         14	/* UNIX faceserver */
#define FF_GHOSTSCRIPT   15
#define FF_GIF           16
#define FF_GROK          17
#define FF_IRIS          18
#define FF_JPEG          19
#define FF_M_RESERVE     20
#define FF_JPEG2         21
#define FF_MACDRAW       22
#define FF_MACPAINT      23
#define FF_Z_RESERVE     24
#define FF_MPEG          25
#define FF_NEXTICON      26
#define FF_NEXTPS        27
#define FF_NEXTCOMP      28
#define FF_PBM           29
#define FF_N_RESERVE     30
#define FF_PDI           31
#define FF_PCM           32
#define FF_PGM           33
#define FF_PORTABLE      200		/* @@ KLUDGE temp */
#define FF_PHOTOSHOP     FF_APS		/* Adobe PhotoShop */
#define FF_P_RESERVE     34
#define FF_PIC           35
#define FF_PICS          36
#define FF_PICT          37
#define FF_PICT2         38
#define FF_PIXAR         39
#define FF_Q_RESERVE     40
#define FF_PIXELPAINT    41
#define FF_POSTSCRIPT    42
#define FF_PPM           FF_PBM		/* PBMPLUS, by Jef Poskanzer */
#define FF_RAW           43
#define FF_RGB           FF_IRIS	/* rather presumptuous */
#define FF_RASTER        44		/* RenderMan */
#define FF_RENDERMAN     45		/* RenderMan */
#define FF_RIB           FF_RENDERMAN	/* RenderMan */
#define FF_RLA           46		/* Wavefront RLA */
#define FF_RLE           47
#define FF_SARNOFF       48
#define FF_STUDIO8       49
#define FF_SGI           FF_IRIS
#define FF_S_RESERVE     50
#define FF_STUDIO32      51
#define FF_SGI_DITHER    52
#define FF_SUN           FF_RASTER
#define FF_SUN_RAS       FF_RASTER
#define FF_SUN_RASTER    FF_RASTER
#define FF_TARGA         53
#define FF_TIFF          54
#define FF_VIS           55		/* So & So's "vision" format */
#define FF_WAVE          FF_RLA		/* Wavefront RLA */
#define FF_XBM           56		/* X BitMap */
#define FF_XWD           57		/* Not sure */
#define FF_YUV           58	/* Abekas yuv format - 720x486 non-square */


/****** Functions in imagic.c ******/

int FFfromString(char *pc);
int FFfromShortName(char *name);
char *ShortNameFromFF(int *pFF);
int hasmagic(unsigned int ffmt);


#endif	/* imagic_h__ */


