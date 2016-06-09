/*****************************************************************************
 imagic.c    S.R.Lines    93.5.25-7.2
 Tables & functons for data in imagic.h
 *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <un/unString.h>
#include <un/unBug.h>
#include "imagic.h"

const char *const imgTypeNameArray[]= { "Unknown Image Type"
                                      , "Sun Raster"
                                      , "SGI Image (PH)"
                                      , "Beymer Dump"
                                      , "Grok Image"
                                      , "Raw Image"
                                      , "vis Image"
                                      , "PGM Image"
                                      , "PBM Image"
                                      , "PostScript Image"
                                      , "Sarnoff Image"
                                      };

struct	imgMTVN {		/* Magic number, Type, Version, Names */
	int	itype;		/* index type  [0,MAXITYPE) */
	int	otype;		/* other type, not an index into an array */
	int	imagic;		/* integer form of magic number, if extant */
	void	*pmagic;	/* pointer to magic string or other form */
	char	*fileExt;	/* short name, used as file name extension */
	char	*fullName;	/* long name, for textual display */
	char	*version;	/* Distinguish old from new Sun raster, etc.*/
	};

struct	imgMTVN imgmtvn[] = {
/*00*/  { 0, 0, -1, NULL, "dunno", "unknown image (file) type", "" },
/*01*/  { 1, 1, -1, NULL, "sun", "Sun raster image (file) type", "" }
};	


int FFfromString(char *pc)
{
  if (  (!strncmp(pc,	"raster",6))
     || (!strncmp(pc,	"sun"	,3))
     || (!strncmp(pc,	"Sun"	,3))
     || (!strncmp(pc,	"SUN"	,3))
  ) return FF_SUN_RAS;
  if (  (!strcmp(pc,	"sgi"	))
     || (!strcmp(pc,	"iris"	))
     || (!strcmp(pc,	"IRIS"	))
     || (!strcmp(pc,	"SGI"	))
  ) return FF_SGI;
  if (  (!strcmp(pc,	"tiff"	))
     || (!strcmp(pc,	"Tiff"	))
     || (!strcmp(pc,	"TIFF"	))
  ) return FF_TIFF;
  if (  (!strncmp(pc,	"sarn"	,4))
     || (!strcmp(pc,	"pix"	))
  ) return FF_SARNOFF;
  if (  (!strcmp(pc,	"beymer"))
     || (!strcmp(pc,	"dump"	))
  ) return FF_BEYMER;
  if (  (!strcmp(pc,	"ps"	))
     || (!strncmp(pc,	"post"	,4))
  ) return FF_POSTSCRIPT;
  return FF_UNKNOWN;
}

int FFfromShortName(char *name)
{
    char ss[32];
    register char *rp = ss;
     
    (void)strncpylower(rp,name,32);
    if (! strncmp(rp,"aps"   , 3 ))  return FF_APS;
    if (! strncmp(rp,"bw"    , 2 ))  return FF_SGI;
    if (! strncmp(rp,"eps"   , 3 ))  return FF_EPS;
    if (! strncmp(rp,"gif"   , 3 ))  return FF_GIF;
    if (! strncmp(rp,"grok"  , 4 ))  return FF_GIF;
    if (! strncmp(rp,"img"   , 3 ))  return FF_SGI;
    if (! strncmp(rp,"pict2" , 5 ))  return FF_PICT2;
    if (! strncmp(rp,"pict"  , 4 ))  return FF_PICT;
    if (! strncmp(rp,"pct"   , 3 ))  return FF_PICT;
    if (! strncmp(rp,"pic"   , 3 ))  return FF_PIC;
    if (! strncmp(rp,"sgi"   , 3 ))  return FF_SGI;
    if (! strncmp(rp,"sun"   , 3 ))  return FF_SGI;
    if (! strncmp(rp,"tiff"  , 4 ))  return FF_TIFF;
    return FF_UNKNOWN;
}


char *ShortNameFromFF(int *pFF)
{
  switch(*pFF) {
    case FF_SGI:	return("sgi");
    default: warn("ShortNameFromFF: changing unknown FF:%d to FF_RAW",*pFF);
      *pFF = FF_RAW; return("raw");
  }
}


int hasmagic(unsigned int ffmt)
{
    return(1);
}



#if 0
	case	IMAGIC_GROK	1
case	IMAGIC_PBM	"P1"
case	IMAGIC_PGM	"P2"
case	IMAGIC_RAS	0x59a66a95	/* Sun Rasterfile _RT_NORMAL ? */
case	IMAGIC_SGI	0732	   /* SGI rgb (Paul Haeberli's IMAGIC) */
case	IMAGIC_VIS	"VISF="

#endif

