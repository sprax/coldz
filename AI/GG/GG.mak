# Microsoft Developer Studio Generated NMAKE File, Based on GG.dsp
!IF "$(CFG)" == ""
CFG=GG - Win32 Debug
!MESSAGE No configuration specified. Defaulting to GG - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "GG - Win32 Release" && "$(CFG)" != "GG - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GG.mak" CFG="GG - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GG - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "GG - Win32 Debug" (based on "Win32 (x86) Static Library")
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

!IF  "$(CFG)" == "GG - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\GG.lib"


CLEAN :
	-@erase "$(INTDIR)\blerpFlowTet.obj"
	-@erase "$(INTDIR)\blipTri.obj"
	-@erase "$(INTDIR)\bnipTri.obj"
	-@erase "$(INTDIR)\copyTet.obj"
	-@erase "$(INTDIR)\copyTri.obj"
	-@erase "$(INTDIR)\gaussFlow.obj"
	-@erase "$(INTDIR)\in3.obj"
	-@erase "$(INTDIR)\inCrsTet.obj"
	-@erase "$(INTDIR)\inCrsTri.obj"
	-@erase "$(INTDIR)\inFloTet.obj"
	-@erase "$(INTDIR)\inFloTri.obj"
	-@erase "$(INTDIR)\inGrid.obj"
	-@erase "$(INTDIR)\inMapOld.obj"
	-@erase "$(INTDIR)\inMapTri.obj"
	-@erase "$(INTDIR)\inParm.obj"
	-@erase "$(INTDIR)\inR.obj"
	-@erase "$(INTDIR)\inWarpBil.obj"
	-@erase "$(INTDIR)\inWarpBilin.obj"
	-@erase "$(INTDIR)\inWarpPersp.obj"
	-@erase "$(INTDIR)\jkCan.obj"
	-@erase "$(INTDIR)\mainN.obj"
	-@erase "$(INTDIR)\mapFwdTri.obj"
	-@erase "$(INTDIR)\matrixOps.obj"
	-@erase "$(INTDIR)\modFwdTri.obj"
	-@erase "$(INTDIR)\nin.obj"
	-@erase "$(INTDIR)\rowFimg.obj"
	-@erase "$(INTDIR)\surfWLSO.obj"
	-@erase "$(INTDIR)\unFlags.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\warpPersp.obj"
	-@erase "$(INTDIR)\warpPerspFwd.obj"
	-@erase "$(INTDIR)\warpPerspI.obj"
	-@erase "$(INTDIR)\warpPerspInv.obj"
	-@erase "$(INTDIR)\warpTetBil.obj"
	-@erase "$(INTDIR)\warpTetBox.obj"
	-@erase "$(INTDIR)\warpTetBoxNen.obj"
	-@erase "$(INTDIR)\warpTetBoxy.obj"
	-@erase "$(INTDIR)\winDraw.obj"
	-@erase "$(INTDIR)\winLooper.obj"
	-@erase "$(INTDIR)\winNew.obj"
	-@erase "$(INTDIR)\winSliders.obj"
	-@erase "$(INTDIR)\winWarp.obj"
	-@erase "$(INTDIR)\winZoom.obj"
	-@erase "$(OUTDIR)\GG.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\GG.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\GG.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\GG.lib" 
LIB32_OBJS= \
	"$(INTDIR)\blerpFlowTet.obj" \
	"$(INTDIR)\blipTri.obj" \
	"$(INTDIR)\bnipTri.obj" \
	"$(INTDIR)\copyTet.obj" \
	"$(INTDIR)\copyTri.obj" \
	"$(INTDIR)\gaussFlow.obj" \
	"$(INTDIR)\in3.obj" \
	"$(INTDIR)\inCrsTet.obj" \
	"$(INTDIR)\inCrsTri.obj" \
	"$(INTDIR)\inFloTet.obj" \
	"$(INTDIR)\inFloTri.obj" \
	"$(INTDIR)\inGrid.obj" \
	"$(INTDIR)\inMapOld.obj" \
	"$(INTDIR)\inMapTri.obj" \
	"$(INTDIR)\inParm.obj" \
	"$(INTDIR)\inR.obj" \
	"$(INTDIR)\inWarpBil.obj" \
	"$(INTDIR)\inWarpBilin.obj" \
	"$(INTDIR)\inWarpPersp.obj" \
	"$(INTDIR)\jkCan.obj" \
	"$(INTDIR)\mainN.obj" \
	"$(INTDIR)\mapFwdTri.obj" \
	"$(INTDIR)\matrixOps.obj" \
	"$(INTDIR)\modFwdTri.obj" \
	"$(INTDIR)\nin.obj" \
	"$(INTDIR)\rowFimg.obj" \
	"$(INTDIR)\surfWLSO.obj" \
	"$(INTDIR)\unFlags.obj" \
	"$(INTDIR)\warpPersp.obj" \
	"$(INTDIR)\warpPerspFwd.obj" \
	"$(INTDIR)\warpPerspI.obj" \
	"$(INTDIR)\warpPerspInv.obj" \
	"$(INTDIR)\warpTetBil.obj" \
	"$(INTDIR)\warpTetBox.obj" \
	"$(INTDIR)\warpTetBoxNen.obj" \
	"$(INTDIR)\warpTetBoxy.obj" \
	"$(INTDIR)\winDraw.obj" \
	"$(INTDIR)\winLooper.obj" \
	"$(INTDIR)\winNew.obj" \
	"$(INTDIR)\winSliders.obj" \
	"$(INTDIR)\winWarp.obj" \
	"$(INTDIR)\winZoom.obj"

"$(OUTDIR)\GG.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "GG - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\GG.lib" "$(OUTDIR)\GG.bsc"


CLEAN :
	-@erase "$(INTDIR)\blerpFlowTet.obj"
	-@erase "$(INTDIR)\blerpFlowTet.sbr"
	-@erase "$(INTDIR)\blipTri.obj"
	-@erase "$(INTDIR)\blipTri.sbr"
	-@erase "$(INTDIR)\bnipTri.obj"
	-@erase "$(INTDIR)\bnipTri.sbr"
	-@erase "$(INTDIR)\copyTet.obj"
	-@erase "$(INTDIR)\copyTet.sbr"
	-@erase "$(INTDIR)\copyTri.obj"
	-@erase "$(INTDIR)\copyTri.sbr"
	-@erase "$(INTDIR)\gaussFlow.obj"
	-@erase "$(INTDIR)\gaussFlow.sbr"
	-@erase "$(INTDIR)\in3.obj"
	-@erase "$(INTDIR)\in3.sbr"
	-@erase "$(INTDIR)\inCrsTet.obj"
	-@erase "$(INTDIR)\inCrsTet.sbr"
	-@erase "$(INTDIR)\inCrsTri.obj"
	-@erase "$(INTDIR)\inCrsTri.sbr"
	-@erase "$(INTDIR)\inFloTet.obj"
	-@erase "$(INTDIR)\inFloTet.sbr"
	-@erase "$(INTDIR)\inFloTri.obj"
	-@erase "$(INTDIR)\inFloTri.sbr"
	-@erase "$(INTDIR)\inGrid.obj"
	-@erase "$(INTDIR)\inGrid.sbr"
	-@erase "$(INTDIR)\inMapOld.obj"
	-@erase "$(INTDIR)\inMapOld.sbr"
	-@erase "$(INTDIR)\inMapTri.obj"
	-@erase "$(INTDIR)\inMapTri.sbr"
	-@erase "$(INTDIR)\inParm.obj"
	-@erase "$(INTDIR)\inParm.sbr"
	-@erase "$(INTDIR)\inR.obj"
	-@erase "$(INTDIR)\inR.sbr"
	-@erase "$(INTDIR)\inWarpBil.obj"
	-@erase "$(INTDIR)\inWarpBil.sbr"
	-@erase "$(INTDIR)\inWarpBilin.obj"
	-@erase "$(INTDIR)\inWarpBilin.sbr"
	-@erase "$(INTDIR)\inWarpPersp.obj"
	-@erase "$(INTDIR)\inWarpPersp.sbr"
	-@erase "$(INTDIR)\jkCan.obj"
	-@erase "$(INTDIR)\jkCan.sbr"
	-@erase "$(INTDIR)\mainN.obj"
	-@erase "$(INTDIR)\mainN.sbr"
	-@erase "$(INTDIR)\mapFwdTri.obj"
	-@erase "$(INTDIR)\mapFwdTri.sbr"
	-@erase "$(INTDIR)\matrixOps.obj"
	-@erase "$(INTDIR)\matrixOps.sbr"
	-@erase "$(INTDIR)\modFwdTri.obj"
	-@erase "$(INTDIR)\modFwdTri.sbr"
	-@erase "$(INTDIR)\nin.obj"
	-@erase "$(INTDIR)\nin.sbr"
	-@erase "$(INTDIR)\rowFimg.obj"
	-@erase "$(INTDIR)\rowFimg.sbr"
	-@erase "$(INTDIR)\surfWLSO.obj"
	-@erase "$(INTDIR)\surfWLSO.sbr"
	-@erase "$(INTDIR)\unFlags.obj"
	-@erase "$(INTDIR)\unFlags.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\warpPersp.obj"
	-@erase "$(INTDIR)\warpPersp.sbr"
	-@erase "$(INTDIR)\warpPerspFwd.obj"
	-@erase "$(INTDIR)\warpPerspFwd.sbr"
	-@erase "$(INTDIR)\warpPerspI.obj"
	-@erase "$(INTDIR)\warpPerspI.sbr"
	-@erase "$(INTDIR)\warpPerspInv.obj"
	-@erase "$(INTDIR)\warpPerspInv.sbr"
	-@erase "$(INTDIR)\warpTetBil.obj"
	-@erase "$(INTDIR)\warpTetBil.sbr"
	-@erase "$(INTDIR)\warpTetBox.obj"
	-@erase "$(INTDIR)\warpTetBox.sbr"
	-@erase "$(INTDIR)\warpTetBoxNen.obj"
	-@erase "$(INTDIR)\warpTetBoxNen.sbr"
	-@erase "$(INTDIR)\warpTetBoxy.obj"
	-@erase "$(INTDIR)\warpTetBoxy.sbr"
	-@erase "$(INTDIR)\winDraw.obj"
	-@erase "$(INTDIR)\winDraw.sbr"
	-@erase "$(INTDIR)\winLooper.obj"
	-@erase "$(INTDIR)\winLooper.sbr"
	-@erase "$(INTDIR)\winNew.obj"
	-@erase "$(INTDIR)\winNew.sbr"
	-@erase "$(INTDIR)\winSliders.obj"
	-@erase "$(INTDIR)\winSliders.sbr"
	-@erase "$(INTDIR)\winWarp.obj"
	-@erase "$(INTDIR)\winWarp.sbr"
	-@erase "$(INTDIR)\winZoom.obj"
	-@erase "$(INTDIR)\winZoom.sbr"
	-@erase "$(OUTDIR)\GG.bsc"
	-@erase "$(OUTDIR)\GG.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /I "..\h" /I "..\h\m" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\GG.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\GG.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\blerpFlowTet.sbr" \
	"$(INTDIR)\blipTri.sbr" \
	"$(INTDIR)\bnipTri.sbr" \
	"$(INTDIR)\copyTet.sbr" \
	"$(INTDIR)\copyTri.sbr" \
	"$(INTDIR)\gaussFlow.sbr" \
	"$(INTDIR)\in3.sbr" \
	"$(INTDIR)\inCrsTet.sbr" \
	"$(INTDIR)\inCrsTri.sbr" \
	"$(INTDIR)\inFloTet.sbr" \
	"$(INTDIR)\inFloTri.sbr" \
	"$(INTDIR)\inGrid.sbr" \
	"$(INTDIR)\inMapOld.sbr" \
	"$(INTDIR)\inMapTri.sbr" \
	"$(INTDIR)\inParm.sbr" \
	"$(INTDIR)\inR.sbr" \
	"$(INTDIR)\inWarpBil.sbr" \
	"$(INTDIR)\inWarpBilin.sbr" \
	"$(INTDIR)\inWarpPersp.sbr" \
	"$(INTDIR)\jkCan.sbr" \
	"$(INTDIR)\mainN.sbr" \
	"$(INTDIR)\mapFwdTri.sbr" \
	"$(INTDIR)\matrixOps.sbr" \
	"$(INTDIR)\modFwdTri.sbr" \
	"$(INTDIR)\nin.sbr" \
	"$(INTDIR)\rowFimg.sbr" \
	"$(INTDIR)\surfWLSO.sbr" \
	"$(INTDIR)\unFlags.sbr" \
	"$(INTDIR)\warpPersp.sbr" \
	"$(INTDIR)\warpPerspFwd.sbr" \
	"$(INTDIR)\warpPerspI.sbr" \
	"$(INTDIR)\warpPerspInv.sbr" \
	"$(INTDIR)\warpTetBil.sbr" \
	"$(INTDIR)\warpTetBox.sbr" \
	"$(INTDIR)\warpTetBoxNen.sbr" \
	"$(INTDIR)\warpTetBoxy.sbr" \
	"$(INTDIR)\winDraw.sbr" \
	"$(INTDIR)\winLooper.sbr" \
	"$(INTDIR)\winNew.sbr" \
	"$(INTDIR)\winSliders.sbr" \
	"$(INTDIR)\winWarp.sbr" \
	"$(INTDIR)\winZoom.sbr"

"$(OUTDIR)\GG.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\GG.lib" 
LIB32_OBJS= \
	"$(INTDIR)\blerpFlowTet.obj" \
	"$(INTDIR)\blipTri.obj" \
	"$(INTDIR)\bnipTri.obj" \
	"$(INTDIR)\copyTet.obj" \
	"$(INTDIR)\copyTri.obj" \
	"$(INTDIR)\gaussFlow.obj" \
	"$(INTDIR)\in3.obj" \
	"$(INTDIR)\inCrsTet.obj" \
	"$(INTDIR)\inCrsTri.obj" \
	"$(INTDIR)\inFloTet.obj" \
	"$(INTDIR)\inFloTri.obj" \
	"$(INTDIR)\inGrid.obj" \
	"$(INTDIR)\inMapOld.obj" \
	"$(INTDIR)\inMapTri.obj" \
	"$(INTDIR)\inParm.obj" \
	"$(INTDIR)\inR.obj" \
	"$(INTDIR)\inWarpBil.obj" \
	"$(INTDIR)\inWarpBilin.obj" \
	"$(INTDIR)\inWarpPersp.obj" \
	"$(INTDIR)\jkCan.obj" \
	"$(INTDIR)\mainN.obj" \
	"$(INTDIR)\mapFwdTri.obj" \
	"$(INTDIR)\matrixOps.obj" \
	"$(INTDIR)\modFwdTri.obj" \
	"$(INTDIR)\nin.obj" \
	"$(INTDIR)\rowFimg.obj" \
	"$(INTDIR)\surfWLSO.obj" \
	"$(INTDIR)\unFlags.obj" \
	"$(INTDIR)\warpPersp.obj" \
	"$(INTDIR)\warpPerspFwd.obj" \
	"$(INTDIR)\warpPerspI.obj" \
	"$(INTDIR)\warpPerspInv.obj" \
	"$(INTDIR)\warpTetBil.obj" \
	"$(INTDIR)\warpTetBox.obj" \
	"$(INTDIR)\warpTetBoxNen.obj" \
	"$(INTDIR)\warpTetBoxy.obj" \
	"$(INTDIR)\winDraw.obj" \
	"$(INTDIR)\winLooper.obj" \
	"$(INTDIR)\winNew.obj" \
	"$(INTDIR)\winSliders.obj" \
	"$(INTDIR)\winWarp.obj" \
	"$(INTDIR)\winZoom.obj"

"$(OUTDIR)\GG.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
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
!IF EXISTS("GG.dep")
!INCLUDE "GG.dep"
!ELSE 
!MESSAGE Warning: cannot find "GG.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "GG - Win32 Release" || "$(CFG)" == "GG - Win32 Debug"
SOURCE=.\blerpFlowTet.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\blerpFlowTet.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\blerpFlowTet.obj"	"$(INTDIR)\blerpFlowTet.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\blipTri.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\blipTri.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\blipTri.obj"	"$(INTDIR)\blipTri.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\bnipTri.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\bnipTri.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\bnipTri.obj"	"$(INTDIR)\bnipTri.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\copyTet.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\copyTet.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\copyTet.obj"	"$(INTDIR)\copyTet.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\copyTri.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\copyTri.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\copyTri.obj"	"$(INTDIR)\copyTri.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\gaussFlow.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\gaussFlow.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\gaussFlow.obj"	"$(INTDIR)\gaussFlow.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\in3.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\in3.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\in3.obj"	"$(INTDIR)\in3.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\inCrsTet.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\inCrsTet.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\inCrsTet.obj"	"$(INTDIR)\inCrsTet.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\inCrsTri.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\inCrsTri.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\inCrsTri.obj"	"$(INTDIR)\inCrsTri.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\inFloTet.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\inFloTet.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\inFloTet.obj"	"$(INTDIR)\inFloTet.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\inFloTri.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\inFloTri.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\inFloTri.obj"	"$(INTDIR)\inFloTri.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\inGrid.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\inGrid.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\inGrid.obj"	"$(INTDIR)\inGrid.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\inMapOld.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\inMapOld.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\inMapOld.obj"	"$(INTDIR)\inMapOld.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\inMapTri.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\inMapTri.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\inMapTri.obj"	"$(INTDIR)\inMapTri.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\inParm.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\inParm.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\inParm.obj"	"$(INTDIR)\inParm.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\inR.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\inR.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\inR.obj"	"$(INTDIR)\inR.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\inWarpBil.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\inWarpBil.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\inWarpBil.obj"	"$(INTDIR)\inWarpBil.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\inWarpBilin.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\inWarpBilin.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\inWarpBilin.obj"	"$(INTDIR)\inWarpBilin.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\inWarpPersp.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\inWarpPersp.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\inWarpPersp.obj"	"$(INTDIR)\inWarpPersp.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jkCan.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\jkCan.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\jkCan.obj"	"$(INTDIR)\jkCan.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mainN.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\mainN.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\mainN.obj"	"$(INTDIR)\mainN.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mapFwdTri.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\mapFwdTri.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\mapFwdTri.obj"	"$(INTDIR)\mapFwdTri.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\matrixOps.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\matrixOps.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\matrixOps.obj"	"$(INTDIR)\matrixOps.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\modFwdTri.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\modFwdTri.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\modFwdTri.obj"	"$(INTDIR)\modFwdTri.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\nin.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\nin.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\nin.obj"	"$(INTDIR)\nin.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rowFimg.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\rowFimg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\rowFimg.obj"	"$(INTDIR)\rowFimg.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\surfWLSO.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\surfWLSO.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\surfWLSO.obj"	"$(INTDIR)\surfWLSO.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\unFlags.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\unFlags.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\unFlags.obj"	"$(INTDIR)\unFlags.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\warpPersp.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\warpPersp.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\warpPersp.obj"	"$(INTDIR)\warpPersp.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\warpPerspFwd.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\warpPerspFwd.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\warpPerspFwd.obj"	"$(INTDIR)\warpPerspFwd.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\warpPerspI.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\warpPerspI.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\warpPerspI.obj"	"$(INTDIR)\warpPerspI.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\warpPerspInv.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\warpPerspInv.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\warpPerspInv.obj"	"$(INTDIR)\warpPerspInv.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\warpTetBil.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\warpTetBil.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\warpTetBil.obj"	"$(INTDIR)\warpTetBil.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\warpTetBox.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\warpTetBox.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\warpTetBox.obj"	"$(INTDIR)\warpTetBox.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\warpTetBoxNen.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\warpTetBoxNen.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\warpTetBoxNen.obj"	"$(INTDIR)\warpTetBoxNen.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\warpTetBoxy.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\warpTetBoxy.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\warpTetBoxy.obj"	"$(INTDIR)\warpTetBoxy.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\winDraw.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\winDraw.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\winDraw.obj"	"$(INTDIR)\winDraw.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\winLooper.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\winLooper.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\winLooper.obj"	"$(INTDIR)\winLooper.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\winNew.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\winNew.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\winNew.obj"	"$(INTDIR)\winNew.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\winSliders.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\winSliders.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\winSliders.obj"	"$(INTDIR)\winSliders.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\winWarp.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\winWarp.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\winWarp.obj"	"$(INTDIR)\winWarp.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\winZoom.c

!IF  "$(CFG)" == "GG - Win32 Release"


"$(INTDIR)\winZoom.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GG - Win32 Debug"


"$(INTDIR)\winZoom.obj"	"$(INTDIR)\winZoom.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

