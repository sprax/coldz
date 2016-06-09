# Microsoft Developer Studio Generated NMAKE File, Based on AA.dsp
!IF "$(CFG)" == ""
CFG=AA - Win32 Debug
!MESSAGE No configuration specified. Defaulting to AA - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "AA - Win32 Release" && "$(CFG)" != "AA - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AA.mak" CFG="AA - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AA - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "AA - Win32 Debug" (based on "Win32 (x86) Static Library")
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

!IF  "$(CFG)" == "AA - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\AA.lib"


CLEAN :
	-@erase "$(INTDIR)\a.obj"
	-@erase "$(INTDIR)\aaConvo.obj"
	-@erase "$(INTDIR)\aaConvoClip.obj"
	-@erase "$(INTDIR)\aaConvoF.obj"
	-@erase "$(INTDIR)\aaConvoN.obj"
	-@erase "$(INTDIR)\aaGaussBlur.obj"
	-@erase "$(INTDIR)\aaGaussBlurF.obj"
	-@erase "$(INTDIR)\aaPxBlur.obj"
	-@erase "$(INTDIR)\addflow.obj"
	-@erase "$(INTDIR)\c.obj"
	-@erase "$(INTDIR)\canny_edges.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\AA.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\AA.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\AA.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\AA.lib" 
LIB32_OBJS= \
	"$(INTDIR)\aaConvoN.obj" \
	"$(INTDIR)\canny_edges.obj" \
	"$(INTDIR)\a.obj" \
	"$(INTDIR)\aaConvo.obj" \
	"$(INTDIR)\aaConvoClip.obj" \
	"$(INTDIR)\aaConvoF.obj" \
	"$(INTDIR)\aaGaussBlur.obj" \
	"$(INTDIR)\aaGaussBlurF.obj" \
	"$(INTDIR)\aaPxBlur.obj" \
	"$(INTDIR)\c.obj" \
	"$(INTDIR)\addflow.obj"

"$(OUTDIR)\AA.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "AA - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\AA.lib" "$(OUTDIR)\AA.bsc"


CLEAN :
	-@erase "$(INTDIR)\a.obj"
	-@erase "$(INTDIR)\a.sbr"
	-@erase "$(INTDIR)\aaConvo.obj"
	-@erase "$(INTDIR)\aaConvo.sbr"
	-@erase "$(INTDIR)\aaConvoClip.obj"
	-@erase "$(INTDIR)\aaConvoClip.sbr"
	-@erase "$(INTDIR)\aaConvoF.obj"
	-@erase "$(INTDIR)\aaConvoF.sbr"
	-@erase "$(INTDIR)\aaConvoN.obj"
	-@erase "$(INTDIR)\aaConvoN.sbr"
	-@erase "$(INTDIR)\aaGaussBlur.obj"
	-@erase "$(INTDIR)\aaGaussBlur.sbr"
	-@erase "$(INTDIR)\aaGaussBlurF.obj"
	-@erase "$(INTDIR)\aaGaussBlurF.sbr"
	-@erase "$(INTDIR)\aaPxBlur.obj"
	-@erase "$(INTDIR)\aaPxBlur.sbr"
	-@erase "$(INTDIR)\addflow.obj"
	-@erase "$(INTDIR)\addflow.sbr"
	-@erase "$(INTDIR)\c.obj"
	-@erase "$(INTDIR)\c.sbr"
	-@erase "$(INTDIR)\canny_edges.obj"
	-@erase "$(INTDIR)\canny_edges.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\AA.bsc"
	-@erase "$(OUTDIR)\AA.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /I "..\h" /I "..\h\m" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\AA.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\AA.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\aaConvoN.sbr" \
	"$(INTDIR)\canny_edges.sbr" \
	"$(INTDIR)\a.sbr" \
	"$(INTDIR)\aaConvo.sbr" \
	"$(INTDIR)\aaConvoClip.sbr" \
	"$(INTDIR)\aaConvoF.sbr" \
	"$(INTDIR)\aaGaussBlur.sbr" \
	"$(INTDIR)\aaGaussBlurF.sbr" \
	"$(INTDIR)\aaPxBlur.sbr" \
	"$(INTDIR)\c.sbr" \
	"$(INTDIR)\addflow.sbr"

"$(OUTDIR)\AA.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\AA.lib" 
LIB32_OBJS= \
	"$(INTDIR)\aaConvoN.obj" \
	"$(INTDIR)\canny_edges.obj" \
	"$(INTDIR)\a.obj" \
	"$(INTDIR)\aaConvo.obj" \
	"$(INTDIR)\aaConvoClip.obj" \
	"$(INTDIR)\aaConvoF.obj" \
	"$(INTDIR)\aaGaussBlur.obj" \
	"$(INTDIR)\aaGaussBlurF.obj" \
	"$(INTDIR)\aaPxBlur.obj" \
	"$(INTDIR)\c.obj" \
	"$(INTDIR)\addflow.obj"

"$(OUTDIR)\AA.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
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
!IF EXISTS("AA.dep")
!INCLUDE "AA.dep"
!ELSE 
!MESSAGE Warning: cannot find "AA.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "AA - Win32 Release" || "$(CFG)" == "AA - Win32 Debug"
SOURCE=.\a.c

!IF  "$(CFG)" == "AA - Win32 Release"


"$(INTDIR)\a.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AA - Win32 Debug"


"$(INTDIR)\a.obj"	"$(INTDIR)\a.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\aaConvo.c

!IF  "$(CFG)" == "AA - Win32 Release"


"$(INTDIR)\aaConvo.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AA - Win32 Debug"


"$(INTDIR)\aaConvo.obj"	"$(INTDIR)\aaConvo.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\aaConvoClip.c

!IF  "$(CFG)" == "AA - Win32 Release"


"$(INTDIR)\aaConvoClip.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AA - Win32 Debug"


"$(INTDIR)\aaConvoClip.obj"	"$(INTDIR)\aaConvoClip.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\aaConvoF.c

!IF  "$(CFG)" == "AA - Win32 Release"


"$(INTDIR)\aaConvoF.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AA - Win32 Debug"


"$(INTDIR)\aaConvoF.obj"	"$(INTDIR)\aaConvoF.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\aaConvoN.c

!IF  "$(CFG)" == "AA - Win32 Release"


"$(INTDIR)\aaConvoN.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AA - Win32 Debug"


"$(INTDIR)\aaConvoN.obj"	"$(INTDIR)\aaConvoN.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\aaGaussBlur.c

!IF  "$(CFG)" == "AA - Win32 Release"


"$(INTDIR)\aaGaussBlur.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AA - Win32 Debug"


"$(INTDIR)\aaGaussBlur.obj"	"$(INTDIR)\aaGaussBlur.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\aaGaussBlurF.c

!IF  "$(CFG)" == "AA - Win32 Release"


"$(INTDIR)\aaGaussBlurF.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AA - Win32 Debug"


"$(INTDIR)\aaGaussBlurF.obj"	"$(INTDIR)\aaGaussBlurF.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\aaPxBlur.c

!IF  "$(CFG)" == "AA - Win32 Release"


"$(INTDIR)\aaPxBlur.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AA - Win32 Debug"


"$(INTDIR)\aaPxBlur.obj"	"$(INTDIR)\aaPxBlur.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\addflow.c

!IF  "$(CFG)" == "AA - Win32 Release"


"$(INTDIR)\addflow.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AA - Win32 Debug"


"$(INTDIR)\addflow.obj"	"$(INTDIR)\addflow.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\c.c

!IF  "$(CFG)" == "AA - Win32 Release"


"$(INTDIR)\c.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AA - Win32 Debug"


"$(INTDIR)\c.obj"	"$(INTDIR)\c.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\canny_edges.c

!IF  "$(CFG)" == "AA - Win32 Release"


"$(INTDIR)\canny_edges.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "AA - Win32 Debug"


"$(INTDIR)\canny_edges.obj"	"$(INTDIR)\canny_edges.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

