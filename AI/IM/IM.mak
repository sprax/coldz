# Microsoft Developer Studio Generated NMAKE File, Based on IM.dsp
!IF "$(CFG)" == ""
CFG=IM - Win32 Debug
!MESSAGE No configuration specified. Defaulting to IM - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "IM - Win32 Release" && "$(CFG)" != "IM - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "IM.mak" CFG="IM - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IM - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "IM - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "IM - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\IM.lib"


CLEAN :
	-@erase "$(INTDIR)\fxyComb.obj"
	-@erase "$(INTDIR)\fxyQuadFwd.obj"
	-@erase "$(INTDIR)\imagic.obj"
	-@erase "$(INTDIR)\mxyFwd.obj"
	-@erase "$(INTDIR)\rgbMark.obj"
	-@erase "$(INTDIR)\rgbPlugFlt.obj"
	-@erase "$(INTDIR)\rgbSmatAff.obj"
	-@erase "$(INTDIR)\rgbSmatAffBil.obj"
	-@erase "$(INTDIR)\rgbSmatAffNxt.obj"
	-@erase "$(INTDIR)\rgbSmatBil.obj"
	-@erase "$(INTDIR)\rgbSmatBil3.obj"
	-@erase "$(INTDIR)\rgbSmatBil4.obj"
	-@erase "$(INTDIR)\rgbSmatBil8.obj"
	-@erase "$(INTDIR)\rgbSmatWts.obj"
	-@erase "$(INTDIR)\rgbSpatBil.obj"
	-@erase "$(INTDIR)\rowBord.obj"
	-@erase "$(INTDIR)\rowCnvt.obj"
	-@erase "$(INTDIR)\rowComb.obj"
	-@erase "$(INTDIR)\rowCopy.obj"
	-@erase "$(INTDIR)\rowCpack.obj"
	-@erase "$(INTDIR)\rowDiff.obj"
	-@erase "$(INTDIR)\rowFile.obj"
	-@erase "$(INTDIR)\rowFillFlt.obj"
	-@erase "$(INTDIR)\rowFillUnc.obj"
	-@erase "$(INTDIR)\rowFlow.obj"
	-@erase "$(INTDIR)\rowGray.obj"
	-@erase "$(INTDIR)\rowMake.obj"
	-@erase "$(INTDIR)\rowMult.obj"
	-@erase "$(INTDIR)\rowNorm.obj"
	-@erase "$(INTDIR)\rowNwts.obj"
	-@erase "$(INTDIR)\rowPclr.obj"
	-@erase "$(INTDIR)\rowPlugFlt.obj"
	-@erase "$(INTDIR)\rowPost.obj"
	-@erase "$(INTDIR)\rowSGI.obj"
	-@erase "$(INTDIR)\rowSmatBil.obj"
	-@erase "$(INTDIR)\rowSmatBiq.obj"
	-@erase "$(INTDIR)\rowSmatN.obj"
	-@erase "$(INTDIR)\rowSmatSin.obj"
	-@erase "$(INTDIR)\rowSpatBil.obj"
	-@erase "$(INTDIR)\rowStat.obj"
	-@erase "$(INTDIR)\rowWarpFlt.obj"
	-@erase "$(INTDIR)\rowWarpUnc.obj"
	-@erase "$(INTDIR)\rowZero.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\IM.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\IM.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\IM.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\IM.lib" 
LIB32_OBJS= \
	"$(INTDIR)\fxyComb.obj" \
	"$(INTDIR)\fxyQuadFwd.obj" \
	"$(INTDIR)\imagic.obj" \
	"$(INTDIR)\mxyFwd.obj" \
	"$(INTDIR)\rgbMark.obj" \
	"$(INTDIR)\rgbPlugFlt.obj" \
	"$(INTDIR)\rgbSmatAff.obj" \
	"$(INTDIR)\rgbSmatAffBil.obj" \
	"$(INTDIR)\rgbSmatAffNxt.obj" \
	"$(INTDIR)\rgbSmatBil.obj" \
	"$(INTDIR)\rgbSmatBil3.obj" \
	"$(INTDIR)\rgbSmatBil4.obj" \
	"$(INTDIR)\rgbSmatBil8.obj" \
	"$(INTDIR)\rgbSmatWts.obj" \
	"$(INTDIR)\rgbSpatBil.obj" \
	"$(INTDIR)\rowBord.obj" \
	"$(INTDIR)\rowCnvt.obj" \
	"$(INTDIR)\rowComb.obj" \
	"$(INTDIR)\rowCopy.obj" \
	"$(INTDIR)\rowCpack.obj" \
	"$(INTDIR)\rowDiff.obj" \
	"$(INTDIR)\rowFile.obj" \
	"$(INTDIR)\rowFillFlt.obj" \
	"$(INTDIR)\rowFillUnc.obj" \
	"$(INTDIR)\rowFlow.obj" \
	"$(INTDIR)\rowGray.obj" \
	"$(INTDIR)\rowMake.obj" \
	"$(INTDIR)\rowMult.obj" \
	"$(INTDIR)\rowNorm.obj" \
	"$(INTDIR)\rowNwts.obj" \
	"$(INTDIR)\rowPclr.obj" \
	"$(INTDIR)\rowPlugFlt.obj" \
	"$(INTDIR)\rowPost.obj" \
	"$(INTDIR)\rowSGI.obj" \
	"$(INTDIR)\rowSmatBil.obj" \
	"$(INTDIR)\rowSmatBiq.obj" \
	"$(INTDIR)\rowSmatN.obj" \
	"$(INTDIR)\rowSmatSin.obj" \
	"$(INTDIR)\rowSpatBil.obj" \
	"$(INTDIR)\rowStat.obj" \
	"$(INTDIR)\rowWarpFlt.obj" \
	"$(INTDIR)\rowWarpUnc.obj" \
	"$(INTDIR)\rowZero.obj"

"$(OUTDIR)\IM.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "IM - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\IM.lib" "$(OUTDIR)\IM.bsc"


CLEAN :
	-@erase "$(INTDIR)\fxyComb.obj"
	-@erase "$(INTDIR)\fxyComb.sbr"
	-@erase "$(INTDIR)\fxyQuadFwd.obj"
	-@erase "$(INTDIR)\fxyQuadFwd.sbr"
	-@erase "$(INTDIR)\imagic.obj"
	-@erase "$(INTDIR)\imagic.sbr"
	-@erase "$(INTDIR)\mxyFwd.obj"
	-@erase "$(INTDIR)\mxyFwd.sbr"
	-@erase "$(INTDIR)\rgbMark.obj"
	-@erase "$(INTDIR)\rgbMark.sbr"
	-@erase "$(INTDIR)\rgbPlugFlt.obj"
	-@erase "$(INTDIR)\rgbPlugFlt.sbr"
	-@erase "$(INTDIR)\rgbSmatAff.obj"
	-@erase "$(INTDIR)\rgbSmatAff.sbr"
	-@erase "$(INTDIR)\rgbSmatAffBil.obj"
	-@erase "$(INTDIR)\rgbSmatAffBil.sbr"
	-@erase "$(INTDIR)\rgbSmatAffNxt.obj"
	-@erase "$(INTDIR)\rgbSmatAffNxt.sbr"
	-@erase "$(INTDIR)\rgbSmatBil.obj"
	-@erase "$(INTDIR)\rgbSmatBil.sbr"
	-@erase "$(INTDIR)\rgbSmatBil3.obj"
	-@erase "$(INTDIR)\rgbSmatBil3.sbr"
	-@erase "$(INTDIR)\rgbSmatBil4.obj"
	-@erase "$(INTDIR)\rgbSmatBil4.sbr"
	-@erase "$(INTDIR)\rgbSmatBil8.obj"
	-@erase "$(INTDIR)\rgbSmatBil8.sbr"
	-@erase "$(INTDIR)\rgbSmatWts.obj"
	-@erase "$(INTDIR)\rgbSmatWts.sbr"
	-@erase "$(INTDIR)\rgbSpatBil.obj"
	-@erase "$(INTDIR)\rgbSpatBil.sbr"
	-@erase "$(INTDIR)\rowBord.obj"
	-@erase "$(INTDIR)\rowBord.sbr"
	-@erase "$(INTDIR)\rowCnvt.obj"
	-@erase "$(INTDIR)\rowCnvt.sbr"
	-@erase "$(INTDIR)\rowComb.obj"
	-@erase "$(INTDIR)\rowComb.sbr"
	-@erase "$(INTDIR)\rowCopy.obj"
	-@erase "$(INTDIR)\rowCopy.sbr"
	-@erase "$(INTDIR)\rowCpack.obj"
	-@erase "$(INTDIR)\rowCpack.sbr"
	-@erase "$(INTDIR)\rowDiff.obj"
	-@erase "$(INTDIR)\rowDiff.sbr"
	-@erase "$(INTDIR)\rowFile.obj"
	-@erase "$(INTDIR)\rowFile.sbr"
	-@erase "$(INTDIR)\rowFillFlt.obj"
	-@erase "$(INTDIR)\rowFillFlt.sbr"
	-@erase "$(INTDIR)\rowFillUnc.obj"
	-@erase "$(INTDIR)\rowFillUnc.sbr"
	-@erase "$(INTDIR)\rowFlow.obj"
	-@erase "$(INTDIR)\rowFlow.sbr"
	-@erase "$(INTDIR)\rowGray.obj"
	-@erase "$(INTDIR)\rowGray.sbr"
	-@erase "$(INTDIR)\rowMake.obj"
	-@erase "$(INTDIR)\rowMake.sbr"
	-@erase "$(INTDIR)\rowMult.obj"
	-@erase "$(INTDIR)\rowMult.sbr"
	-@erase "$(INTDIR)\rowNorm.obj"
	-@erase "$(INTDIR)\rowNorm.sbr"
	-@erase "$(INTDIR)\rowNwts.obj"
	-@erase "$(INTDIR)\rowNwts.sbr"
	-@erase "$(INTDIR)\rowPclr.obj"
	-@erase "$(INTDIR)\rowPclr.sbr"
	-@erase "$(INTDIR)\rowPlugFlt.obj"
	-@erase "$(INTDIR)\rowPlugFlt.sbr"
	-@erase "$(INTDIR)\rowPost.obj"
	-@erase "$(INTDIR)\rowPost.sbr"
	-@erase "$(INTDIR)\rowSGI.obj"
	-@erase "$(INTDIR)\rowSGI.sbr"
	-@erase "$(INTDIR)\rowSmatBil.obj"
	-@erase "$(INTDIR)\rowSmatBil.sbr"
	-@erase "$(INTDIR)\rowSmatBiq.obj"
	-@erase "$(INTDIR)\rowSmatBiq.sbr"
	-@erase "$(INTDIR)\rowSmatN.obj"
	-@erase "$(INTDIR)\rowSmatN.sbr"
	-@erase "$(INTDIR)\rowSmatSin.obj"
	-@erase "$(INTDIR)\rowSmatSin.sbr"
	-@erase "$(INTDIR)\rowSpatBil.obj"
	-@erase "$(INTDIR)\rowSpatBil.sbr"
	-@erase "$(INTDIR)\rowStat.obj"
	-@erase "$(INTDIR)\rowStat.sbr"
	-@erase "$(INTDIR)\rowWarpFlt.obj"
	-@erase "$(INTDIR)\rowWarpFlt.sbr"
	-@erase "$(INTDIR)\rowWarpUnc.obj"
	-@erase "$(INTDIR)\rowWarpUnc.sbr"
	-@erase "$(INTDIR)\rowZero.obj"
	-@erase "$(INTDIR)\rowZero.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\IM.bsc"
	-@erase "$(OUTDIR)\IM.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /WX /Gm /GX /ZI /Od /I "..\h" /I "..\h\m" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\IM.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\IM.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\fxyComb.sbr" \
	"$(INTDIR)\fxyQuadFwd.sbr" \
	"$(INTDIR)\imagic.sbr" \
	"$(INTDIR)\mxyFwd.sbr" \
	"$(INTDIR)\rgbMark.sbr" \
	"$(INTDIR)\rgbPlugFlt.sbr" \
	"$(INTDIR)\rgbSmatAff.sbr" \
	"$(INTDIR)\rgbSmatAffBil.sbr" \
	"$(INTDIR)\rgbSmatAffNxt.sbr" \
	"$(INTDIR)\rgbSmatBil.sbr" \
	"$(INTDIR)\rgbSmatBil3.sbr" \
	"$(INTDIR)\rgbSmatBil4.sbr" \
	"$(INTDIR)\rgbSmatBil8.sbr" \
	"$(INTDIR)\rgbSmatWts.sbr" \
	"$(INTDIR)\rgbSpatBil.sbr" \
	"$(INTDIR)\rowBord.sbr" \
	"$(INTDIR)\rowCnvt.sbr" \
	"$(INTDIR)\rowComb.sbr" \
	"$(INTDIR)\rowCopy.sbr" \
	"$(INTDIR)\rowCpack.sbr" \
	"$(INTDIR)\rowDiff.sbr" \
	"$(INTDIR)\rowFile.sbr" \
	"$(INTDIR)\rowFillFlt.sbr" \
	"$(INTDIR)\rowFillUnc.sbr" \
	"$(INTDIR)\rowFlow.sbr" \
	"$(INTDIR)\rowGray.sbr" \
	"$(INTDIR)\rowMake.sbr" \
	"$(INTDIR)\rowMult.sbr" \
	"$(INTDIR)\rowNorm.sbr" \
	"$(INTDIR)\rowNwts.sbr" \
	"$(INTDIR)\rowPclr.sbr" \
	"$(INTDIR)\rowPlugFlt.sbr" \
	"$(INTDIR)\rowPost.sbr" \
	"$(INTDIR)\rowSGI.sbr" \
	"$(INTDIR)\rowSmatBil.sbr" \
	"$(INTDIR)\rowSmatBiq.sbr" \
	"$(INTDIR)\rowSmatN.sbr" \
	"$(INTDIR)\rowSmatSin.sbr" \
	"$(INTDIR)\rowSpatBil.sbr" \
	"$(INTDIR)\rowStat.sbr" \
	"$(INTDIR)\rowWarpFlt.sbr" \
	"$(INTDIR)\rowWarpUnc.sbr" \
	"$(INTDIR)\rowZero.sbr"

"$(OUTDIR)\IM.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\IM.lib" 
LIB32_OBJS= \
	"$(INTDIR)\fxyComb.obj" \
	"$(INTDIR)\fxyQuadFwd.obj" \
	"$(INTDIR)\imagic.obj" \
	"$(INTDIR)\mxyFwd.obj" \
	"$(INTDIR)\rgbMark.obj" \
	"$(INTDIR)\rgbPlugFlt.obj" \
	"$(INTDIR)\rgbSmatAff.obj" \
	"$(INTDIR)\rgbSmatAffBil.obj" \
	"$(INTDIR)\rgbSmatAffNxt.obj" \
	"$(INTDIR)\rgbSmatBil.obj" \
	"$(INTDIR)\rgbSmatBil3.obj" \
	"$(INTDIR)\rgbSmatBil4.obj" \
	"$(INTDIR)\rgbSmatBil8.obj" \
	"$(INTDIR)\rgbSmatWts.obj" \
	"$(INTDIR)\rgbSpatBil.obj" \
	"$(INTDIR)\rowBord.obj" \
	"$(INTDIR)\rowCnvt.obj" \
	"$(INTDIR)\rowComb.obj" \
	"$(INTDIR)\rowCopy.obj" \
	"$(INTDIR)\rowCpack.obj" \
	"$(INTDIR)\rowDiff.obj" \
	"$(INTDIR)\rowFile.obj" \
	"$(INTDIR)\rowFillFlt.obj" \
	"$(INTDIR)\rowFillUnc.obj" \
	"$(INTDIR)\rowFlow.obj" \
	"$(INTDIR)\rowGray.obj" \
	"$(INTDIR)\rowMake.obj" \
	"$(INTDIR)\rowMult.obj" \
	"$(INTDIR)\rowNorm.obj" \
	"$(INTDIR)\rowNwts.obj" \
	"$(INTDIR)\rowPclr.obj" \
	"$(INTDIR)\rowPlugFlt.obj" \
	"$(INTDIR)\rowPost.obj" \
	"$(INTDIR)\rowSGI.obj" \
	"$(INTDIR)\rowSmatBil.obj" \
	"$(INTDIR)\rowSmatBiq.obj" \
	"$(INTDIR)\rowSmatN.obj" \
	"$(INTDIR)\rowSmatSin.obj" \
	"$(INTDIR)\rowSpatBil.obj" \
	"$(INTDIR)\rowStat.obj" \
	"$(INTDIR)\rowWarpFlt.obj" \
	"$(INTDIR)\rowWarpUnc.obj" \
	"$(INTDIR)\rowZero.obj"

"$(OUTDIR)\IM.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("IM.dep")
!INCLUDE "IM.dep"
!ELSE 
!MESSAGE Warning: cannot find "IM.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "IM - Win32 Release" || "$(CFG)" == "IM - Win32 Debug"
SOURCE=.\fxyComb.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\fxyComb.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\fxyComb.obj"	"$(INTDIR)\fxyComb.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\fxyQuadFwd.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\fxyQuadFwd.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\fxyQuadFwd.obj"	"$(INTDIR)\fxyQuadFwd.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\imagic.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\imagic.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\imagic.obj"	"$(INTDIR)\imagic.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mxyFwd.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\mxyFwd.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\mxyFwd.obj"	"$(INTDIR)\mxyFwd.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rgbMark.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\rgbMark.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\rgbMark.obj"	"$(INTDIR)\rgbMark.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rgbPlugFlt.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\rgbPlugFlt.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\rgbPlugFlt.obj"	"$(INTDIR)\rgbPlugFlt.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rgbSmatAff.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\rgbSmatAff.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\rgbSmatAff.obj"	"$(INTDIR)\rgbSmatAff.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rgbSmatAffBil.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\rgbSmatAffBil.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\rgbSmatAffBil.obj"	"$(INTDIR)\rgbSmatAffBil.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rgbSmatAffNxt.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\rgbSmatAffNxt.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\rgbSmatAffNxt.obj"	"$(INTDIR)\rgbSmatAffNxt.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rgbSmatBil.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\rgbSmatBil.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\rgbSmatBil.obj"	"$(INTDIR)\rgbSmatBil.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rgbSmatBil3.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\rgbSmatBil3.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\rgbSmatBil3.obj"	"$(INTDIR)\rgbSmatBil3.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rgbSmatBil4.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\rgbSmatBil4.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\rgbSmatBil4.obj"	"$(INTDIR)\rgbSmatBil4.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rgbSmatBil8.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\rgbSmatBil8.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\rgbSmatBil8.obj"	"$(INTDIR)\rgbSmatBil8.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rgbSmatWts.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\rgbSmatWts.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\rgbSmatWts.obj"	"$(INTDIR)\rgbSmatWts.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rgbSpatBil.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\rgbSpatBil.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\rgbSpatBil.obj"	"$(INTDIR)\rgbSpatBil.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rowBord.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\rowBord.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\rowBord.obj"	"$(INTDIR)\rowBord.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rowCnvt.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\rowCnvt.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\rowCnvt.obj"	"$(INTDIR)\rowCnvt.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rowComb.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\rowComb.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\rowComb.obj"	"$(INTDIR)\rowComb.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rowCopy.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\rowCopy.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\rowCopy.obj"	"$(INTDIR)\rowCopy.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rowCpack.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\rowCpack.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\rowCpack.obj"	"$(INTDIR)\rowCpack.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rowDiff.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\rowDiff.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\rowDiff.obj"	"$(INTDIR)\rowDiff.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rowFile.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\rowFile.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\rowFile.obj"	"$(INTDIR)\rowFile.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rowFillFlt.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\rowFillFlt.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\rowFillFlt.obj"	"$(INTDIR)\rowFillFlt.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rowFillUnc.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\rowFillUnc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\rowFillUnc.obj"	"$(INTDIR)\rowFillUnc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=..\NEW\rowFlow.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\rowFlow.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\rowFlow.obj"	"$(INTDIR)\rowFlow.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\rowGray.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\rowGray.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\rowGray.obj"	"$(INTDIR)\rowGray.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rowMake.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\rowMake.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\rowMake.obj"	"$(INTDIR)\rowMake.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rowMult.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\rowMult.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\rowMult.obj"	"$(INTDIR)\rowMult.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rowNorm.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\rowNorm.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\rowNorm.obj"	"$(INTDIR)\rowNorm.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rowNwts.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\rowNwts.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\rowNwts.obj"	"$(INTDIR)\rowNwts.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rowPclr.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\rowPclr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\rowPclr.obj"	"$(INTDIR)\rowPclr.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rowPlugFlt.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\rowPlugFlt.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\rowPlugFlt.obj"	"$(INTDIR)\rowPlugFlt.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rowPost.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\rowPost.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\rowPost.obj"	"$(INTDIR)\rowPost.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rowSGI.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\rowSGI.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\rowSGI.obj"	"$(INTDIR)\rowSGI.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rowSmatBil.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\rowSmatBil.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\rowSmatBil.obj"	"$(INTDIR)\rowSmatBil.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rowSmatBiq.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\rowSmatBiq.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\rowSmatBiq.obj"	"$(INTDIR)\rowSmatBiq.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rowSmatN.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\rowSmatN.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\rowSmatN.obj"	"$(INTDIR)\rowSmatN.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rowSmatSin.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\rowSmatSin.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\rowSmatSin.obj"	"$(INTDIR)\rowSmatSin.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rowSpatBil.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\rowSpatBil.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\rowSpatBil.obj"	"$(INTDIR)\rowSpatBil.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rowStat.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\rowStat.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\rowStat.obj"	"$(INTDIR)\rowStat.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rowWarpFlt.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\rowWarpFlt.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\rowWarpFlt.obj"	"$(INTDIR)\rowWarpFlt.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rowWarpUnc.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\rowWarpUnc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\rowWarpUnc.obj"	"$(INTDIR)\rowWarpUnc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rowZero.c

!IF  "$(CFG)" == "IM - Win32 Release"


"$(INTDIR)\rowZero.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IM - Win32 Debug"


"$(INTDIR)\rowZero.obj"	"$(INTDIR)\rowZero.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

