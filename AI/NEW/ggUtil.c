/*****************************************************************************
 ggUtil.c    S.R.Lines    93.8.17-9.21
 *****************************************************************************/
#include	"gg.h"	/* includes ggUtil.h */
#include	"ggExtern.h"	/* includes ggUtil.h */

#define SLOG_SIZE 256
#define SS_SIZE   128


FILE *ggLogO ( int dX, int dY, unt dW, unt dH, unt sW, unt sH, unt qW, unt qH
             , char *dir, char **iname, int ntweens, int pvim, dbl trsBeg, dbl trsEnd)
{
  FILE *fLog;
  char sLog[256], ss[128];
  int j;

  sprintf_s(sLog, SLOG_SIZE, "%s ",ProgName);
  for (j = 0; j < NumSrc; j++) {
    strcat_s(sLog, SLOG_SIZE, iname[j]);
    strcat_s(sLog, SLOG_SIZE, " ");
  }
  sprintf_s(ss, SS_SIZE, "%d %.3f %.3f ",ntweens,trsBeg,trsEnd);
  strcat_s(sLog, SLOG_SIZE, ss);
  sprintf_s(ss, SS_SIZE, "\tSYS:%s  PVI:%s  DOM:%s  OFF:%s "
#if     defined(SGI)
  ,"SGI"
#elif   defined(SUN)
  ,"Sun"
#elif   defined(MAC)
  ,"Mac"
#else
  ,"???"
#endif
  ,pvimStr(pvim)
  ,(Options & O_UNCH ? "unchr" : "float")
  ,"SGI");      /* @ */
  strcat_s(sLog, SLOG_SIZE, ss);

  fprintf(stderr,"%s\n",sLog);
  reMkdir(dir,0775,0);
  fprintf(stderr,"Output dir: %s\t dW,dH: %d x %d,      qW,qH: %d x %d\n"
         ,dir,dW,dH,qW,qH);
  sprintf_s(ss, SS_SIZE,"%s/Log",dir);
  errno_t err = fopen_s(&fLog, ss, "a");
  if (err != 0)
    die("Error opening %s for appending\n",ss);
  fprintf(fLog,"%s\n",sLog);
  fflush(stderr);
  return (fLog);
}

FILE *ggLogH ( int *dims, char *dir, int pvim, FILE *pFiles
             , char **argStr, int *argInt, dbl *argDbl)
{
  FILE *fLog = NULL;
  char sLog[SLOG_SIZE], ss[128], **iname = argStr+1;
  int j;

  sprintf_s(sLog, SLOG_SIZE,"%s ",ProgName);
  for (j = 0; j < NumSrc; j++) {
    strcat_s(sLog, 128, iname[j]);
    strcat_s(sLog, 128," ");
  }
  sprintf_s(ss, 128,"%d ",argInt[1]);

  strcat_s(sLog, SLOG_SIZE, ss);
  sprintf_s(ss, 128, "\tSYS:%s  PVI:%s  DOM:%s  OFF:%s "
#if     defined(SGI)
  ,"SGI"
#elif   defined(SUN)
  ,"Sun"
#elif   defined(MAC)
  ,"Mac"
#else
  ,"???"
#endif
  ,pvimStr(pvim)
  ,(Options & O_UNCH ? "unchr" : "float")
  ,"SGI");      /* @ */


  die("ggLogH: not done...");
#if 0
  fprintf(stderr,"%s\n",sLog);
  reMkdir(dir,0775,0);
  fprintf(stderr,"Output dir: %s\t\tdW,dH: %d x %d,      qW,qH: %d x %d\n"
         ,dir,dW,dH,qW,qH);
  sprintf_s(ss,"%s/Log",dir);
  if ( ! (fLog = fopen_s(ss,"a")))
    die("Error opening %s for appending\n",ss);
  fprintf(fLog,"%s\n",sLog);
#endif

  return (fLog);
}

char *pvimStr (unt pvim)
{
  switch (pvim & (O_BLNR|O_BQDR|O_BCBC|O_SLIN)) {
    case O_BLNR: return "BiLinear";
    case O_BQDR: return "BiQuadrat";
    case O_BCBC: return "BiCubic";
    case O_SLIN: return "SingleP";
    default:     return "UNKNOWN";
  }
}





int clipOn4 (float dst[4], float src[4])
{
  int j;
  double dd, dsum = 0.0, ssum = src[0] + src[1] + src[2] + src[3];
  if (ssum < 1.0e-3)
    die("clipOn4: sum of src vector == % .4f is too close to zero\n",ssum);

  for (j = 0; j < 4; j++) {
    dd = src[j] / ssum;
    if (dd > 1.0)  dd = 1.0;  else
    if (dd < 0.0)  dd = 0.0;
    dst[j] = (flt)dd;
    dsum  +=      dd;
  }
  
  if (dsum < 1.0e-3)
    die("clipOn4: near 0 sum of dst vector == % .4f\n",dsum);
  for (j = 0; j < 4; j++)
    dst[j] = (flt)( dst[j] / dsum );

  dsum = dst[0] + dst[1] + dst[2] + dst[3];
  dd = dsum - 1.0;
  dd = ABS(dd);
  assert (dd < 1.0e-3);
  return 0;
}




int clipOnN (float *dst, float *src, int len)
{
  int j;
  double dd, dsum = 0.0, ssum = 0.0;


  for (ssum = 0.0, j = 1; j <= len; j++)
    ssum += src[j];

  assert(ssum > 0.001);

  if (ssum < 0.001)
    die("clipOnN: sum(src) too close to 0 (% .4f)\n",ssum);

  for (j = 1; j <= len; j++) {
    dd = src[j] / ssum;
    if (dd > 1.0)  dd = 1.0;  else
    if (dd < 0.0)  dd = 0.0;
    dst[j] = (flt) dd;
    dsum  +=       dd;
  }
  
  assert(dsum > 0.001);

  if (dsum < 0.001)
    die("clipOnN: sum(dst) too close to 0 (% .4f)\n",dsum);

  if (Options & O_VERB) {
    fprintf(stderr,"clipOnN[0...]: % .4f",dst[0]);
    for (j = 1; j <= len; j++) {
      dst[j] = (flt)( dst[j] / dsum );
      fprintf(stderr," %.3f", dst[j]);
    }
    fprintf(stderr,"\n");
  }
  else
    for (j = 1; j <= len; j++)
      dst[j] = (flt)( dst[j] / dsum );

  return 0;
}


