/****************************************************************************
 jk.c    for mc2, etc.    S.R.Lines    94.9.19
 *****************************************************************************/
#define		UNTIME  0
#define		UNBUG	0
#include	"gg.h"
#include	"inExtern.h"
#include	"ggParse.h"
#include	"jk.h"

extern double CropYF;

int
jkParse(int argc, char **argv, char **envp)
{
    int 	j, q, z;
    char	dirname[SLEN];
#ifdef MAC
    void MacArg(int *argcp, char ***argvp, char ***envpp);
    argv = &ProgName, argc = 1, envp[0] = NULL;
    MacArg(&argc, &argv, &envp);
#else
    ProgName = argv[0];   /* might want to make this depend on # args */
#endif
    BEGCLOCK("");
    NumSrc = ggParse(&Options, &Devopts, ArgStr, ArgInt, ArgDbl, argc, argv, envp
        , NumSrc, MAXSRC, NumInt + 2, NumDbl);
    /* wrn("jkParse: ggParse returned %d", NumSrc); */
    z = (int)ArgStr[0];
    if (z < NumSrc)
        die("jkParse: Need at least %d string args!", NumSrc);
    if (z < NumSrc + 2) {
        sprintf_s(dirname, SLEN, "%s./%s", ProgName, ArgStr[1]);
        for (j = 2; j <= NumSrc; j++) {
            strcat_s(dirname, SLEN, ".");
            strcat_s(dirname, SLEN, ArgStr[j]);
        }
        ArgStr[NumSrc + 2] = _strdup(dirname);
    }
    sprintf_s(dirname, SLEN, "%s+%s.tris", ArgStr[1], ArgStr[2]);
    ArgStr[NumSrc + 1] = _strdup(dirname);

    if (z < NumSrc + 1)  OutIFF = FF_SGI;
    else if ((q = FFfromString(ArgStr[NumSrc + 1])) == FF_UNKNOWN)
        Usage("Unknown output file format (%d) -- see imagic.h", q);
    else OutIFF = q;

    z = ArgInt[0];
    if (z > 0) {
        if ((q = ArgInt[1]) < 0 || q > MAXTWEENS) {
            Ntweens = 1;
            warn("\tNtweens must be between 0 and %d, not %d -- reset to %d."
                , MAXTWEENS, q, Ntweens);
        }
        else Ntweens = ArgInt[1];
    }
    if (z > 1) { Ytweens = MAX(ArgInt[2], 1); wrn("%s: Ytweens: %d ", __FILE__, Ytweens); }
    if (z > 2) { Ztweens = MAX(ArgInt[3], 1); wrn("%s: Ztweens: %d ", __FILE__, Ztweens); }
    if (z > 3) { ImB = MAX(ArgInt[4], 4); wrn("%s: ImB: %d ", __FILE__, ImB); }
    if (z > 4) { PyrTop = MAX(ArgInt[5], 2); wrn("%s: PyrTop: %d ", __FILE__, PyrTop); }
    if (z > 5) { PyrBot = MIN(ArgInt[6], 0); wrn("%s: PyrBot: %d ", __FILE__, PyrBot); }
    /*****
      if (z > 2)  Ytweens = ArgInt[2];
      if (z > 3)  Ztweens = ArgInt[3];
    *****/
    z = (int)ArgDbl[0];
    if (z > 0)      MvZoom = (flt)ArgDbl[1];
    if (z > 1)      CropYF = ArgDbl[2];
    if (z > 2)      TrsBeg = ArgDbl[3];
    if (z > 3)      TrsEnd = ArgDbl[4];
    if (z > 4)      TrsMed = ArgDbl[5];
    Zoom = MvZoom;
    /* prn("jk.c:CropYF: %f", CropYF); */
    /* prn("jk.c:Zoom=MvZoom: %f", Zoom); */
    /***** Done parsing, tho args actually interpreted by warpG. ****/
    return NumSrc;
}


FILE *jkLog(int argc, char **argv, char **argStr, int ntweens, int pvim
    , dbl trsBeg, dbl trsEnd, unt dW, unt dH)
{
    FILE *fLog;
    char sLog[SLEN], ss[SLEN], *dirname = argStr[NumSrc + 2];
    int j;

    reMkdir(dirname, 0775, 0);
    /* fprintf(stderr,"Output dir: %s\t dW,dH: %d x %d\n",dirname,dW,dH); */
    sprintf_s(ss, SLEN, "%s/Log", dirname);
    errno_t err = fopen_s(&fLog, ss, "a");
    if (err)
        die("Error opening %s for appending\n", ss);

    sprintf_s(ss, SLEN, "%s", argv[0]);
    for (j = 1; j < argc; j++) {
        strcat_s(ss, SLEN, " ");
        strcat_s(ss, SLEN, argv[j]);
    }
    fprintf(fLog, "%s\n", ss);

    sprintf_s(sLog, SLEN, "%s ", ProgName);
    for (j = 1; j <= NumSrc; j++) {
        strcat_s(sLog, SLEN, argStr[j]);
        strcat_s(sLog, SLEN, " ");
    }
    sprintf_s(ss, SLEN, "\tSYS:%s  PVI:%s  DOM:%s  OFF:%s "
#if     defined(SGI)
        , "SGI"
#elif   defined(SUN)
        , "Sun"
#elif   defined(MAC)
        , "Mac"
#else
        , "???"
#endif
        , pvimStr(pvim)
        , (Options & O_UNCH ? "unchr" : "float")
        , "SGI");      /* @ */
    strcat_s(sLog, SLEN, ss);
    sprintf_s(ss, SLEN, "%d %.3f %.3f ", ntweens, trsBeg, trsEnd);
    strcat_s(sLog, SLEN, ss);
    /**
    fprintf(stderr,"%s\n",sLog);
    fflush(stderr);
    **/
    fprintf(fLog, "%s\n", sLog);
    return (fLog);
}
