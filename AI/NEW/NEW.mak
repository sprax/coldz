# Microsoft Developer Studio Generated NMAKE File, Based on NEW.dsp
!IF "$(CFG)" == ""
CFG=NEW - Win32 Debug
!MESSAGE No configuration specified. Defaulting to NEW - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "NEW - Win32 Release" && "$(CFG)" != "NEW - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NEW.mak" CFG="NEW - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NEW - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "NEW - Win32 Debug" (based on "Win32 (x86) Static Library")
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

!IF  "$(CFG)" == "NEW - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\NEW.lib"


CLEAN :
	-@erase "$(INTDIR)\blerpTri.obj"
	-@erase "$(INTDIR)\extractFace.obj"
	-@erase "$(INTDIR)\ggOF.obj"
	-@erase "$(INTDIR)\ggParse.obj"
	-@erase "$(INTDIR)\ggUtil.obj"
	-@erase "$(INTDIR)\jk.obj"
	-@erase "$(INTDIR)\jkRead.obj"
	-@erase "$(INTDIR)\labelBase.obj"
	-@erase "$(INTDIR)\manCor.obj"
	-@erase "$(INTDIR)\modFnTri.obj"
	-@erase "$(INTDIR)\morphBase.obj"
	-@erase "$(INTDIR)\rpBase.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\NEW.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\NEW.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\NEW.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\NEW.lib" 
LIB32_OBJS= \
	"$(INTDIR)\blerpTri.obj" \
	"$(INTDIR)\extractFace.obj" \
	"$(INTDIR)\ggOF.obj" \
	"$(INTDIR)\ggParse.obj" \
	"$(INTDIR)\ggUtil.obj" \
	"$(INTDIR)\jk.obj" \
	"$(INTDIR)\jkRead.obj" \
	"$(INTDIR)\labelBase.obj" \
	"$(INTDIR)\manCor.obj" \
	"$(INTDIR)\modFnTri.obj" \
	"$(INTDIR)\morphBase.obj" \
	"$(INTDIR)\rpBase.obj"

"$(OUTDIR)\NEW.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "NEW - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\NEW.lib" "$(OUTDIR)\NEW.bsc"


CLEAN :
	-@erase "$(INTDIR)\blerpTri.obj"
	-@erase "$(INTDIR)\blerpTri.sbr"
	-@erase "$(INTDIR)\extractFace.obj"
	-@erase "$(INTDIR)\extractFace.sbr"
	-@erase "$(INTDIR)\ggOF.obj"
	-@erase "$(INTDIR)\ggOF.sbr"
	-@erase "$(INTDIR)\ggParse.obj"
	-@erase "$(INTDIR)\ggParse.sbr"
	-@erase "$(INTDIR)\ggUtil.obj"
	-@erase "$(INTDIR)\ggUtil.sbr"
	-@erase "$(INTDIR)\jk.obj"
	-@erase "$(INTDIR)\jk.sbr"
	-@erase "$(INTDIR)\jkRead.obj"
	-@erase "$(INTDIR)\jkRead.sbr"
	-@erase "$(INTDIR)\labelBase.obj"
	-@erase "$(INTDIR)\labelBase.sbr"
	-@erase "$(INTDIR)\manCor.obj"
	-@erase "$(INTDIR)\manCor.sbr"
	-@erase "$(INTDIR)\modFnTri.obj"
	-@erase "$(INTDIR)\modFnTri.sbr"
	-@erase "$(INTDIR)\morphBase.obj"
	-@erase "$(INTDIR)\morphBase.sbr"
	-@erase "$(INTDIR)\rpBase.obj"
	-@erase "$(INTDIR)\rpBase.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\NEW.bsc"
	-@erase "$(OUTDIR)\NEW.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /WX /Gm /GX /ZI /Od /I "..\h" /I "..\h\m" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\NEW.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\NEW.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\blerpTri.sbr" \
	"$(INTDIR)\extractFace.sbr" \
	"$(INTDIR)\ggOF.sbr" \
	"$(INTDIR)\ggParse.sbr" \
	"$(INTDIR)\ggUtil.sbr" \
	"$(INTDIR)\jk.sbr" \
	"$(INTDIR)\jkRead.sbr" \
	"$(INTDIR)\labelBase.sbr" \
	"$(INTDIR)\manCor.sbr" \
	"$(INTDIR)\modFnTri.sbr" \
	"$(INTDIR)\morphBase.sbr" \
	"$(INTDIR)\rpBase.sbr"

"$(OUTDIR)\NEW.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\NEW.lib" 
LIB32_OBJS= \
	"$(INTDIR)\blerpTri.obj" \
	"$(INTDIR)\extractFace.obj" \
	"$(INTDIR)\ggOF.obj" \
	"$(INTDIR)\ggParse.obj" \
	"$(INTDIR)\ggUtil.obj" \
	"$(INTDIR)\jk.obj" \
	"$(INTDIR)\jkRead.obj" \
	"$(INTDIR)\labelBase.obj" \
	"$(INTDIR)\manCor.obj" \
	"$(INTDIR)\modFnTri.obj" \
	"$(INTDIR)\morphBase.obj" \
	"$(INTDIR)\rpBase.obj"

"$(OUTDIR)\NEW.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
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
!IF EXISTS("NEW.dep")
!INCLUDE "NEW.dep"
!ELSE 
!MESSAGE Warning: cannot find "NEW.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "NEW - Win32 Release" || "$(CFG)" == "NEW - Win32 Debug"
SOURCE=.\blerpTri.c

!IF  "$(CFG)" == "NEW - Win32 Release"


"$(INTDIR)\blerpTri.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "NEW - Win32 Debug"


"$(INTDIR)\blerpTri.obj"	"$(INTDIR)\blerpTri.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\extractFace.c

!IF  "$(CFG)" == "NEW - Win32 Release"


"$(INTDIR)\extractFace.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "NEW - Win32 Debug"


"$(INTDIR)\extractFace.obj"	"$(INTDIR)\extractFace.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ggOF.c

!IF  "$(CFG)" == "NEW - Win32 Release"


"$(INTDIR)\ggOF.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "NEW - Win32 Debug"


"$(INTDIR)\ggOF.obj"	"$(INTDIR)\ggOF.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ggParse.c

!IF  "$(CFG)" == "NEW - Win32 Release"


"$(INTDIR)\ggParse.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "NEW - Win32 Debug"


"$(INTDIR)\ggParse.obj"	"$(INTDIR)\ggParse.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ggUtil.c

!IF  "$(CFG)" == "NEW - Win32 Release"


"$(INTDIR)\ggUtil.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "NEW - Win32 Debug"


"$(INTDIR)\ggUtil.obj"	"$(INTDIR)\ggUtil.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jk.c

!IF  "$(CFG)" == "NEW - Win32 Release"


"$(INTDIR)\jk.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "NEW - Win32 Debug"


"$(INTDIR)\jk.obj"	"$(INTDIR)\jk.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jkRead.c

!IF  "$(CFG)" == "NEW - Win32 Release"


"$(INTDIR)\jkRead.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "NEW - Win32 Debug"


"$(INTDIR)\jkRead.obj"	"$(INTDIR)\jkRead.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\labelBase.c

!IF  "$(CFG)" == "NEW - Win32 Release"


"$(INTDIR)\labelBase.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "NEW - Win32 Debug"


"$(INTDIR)\labelBase.obj"	"$(INTDIR)\labelBase.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\manCor.c

!IF  "$(CFG)" == "NEW - Win32 Release"


"$(INTDIR)\manCor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "NEW - Win32 Debug"


"$(INTDIR)\manCor.obj"	"$(INTDIR)\manCor.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\modFnTri.c

!IF  "$(CFG)" == "NEW - Win32 Release"


"$(INTDIR)\modFnTri.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "NEW - Win32 Debug"


"$(INTDIR)\modFnTri.obj"	"$(INTDIR)\modFnTri.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\morphBase.c

!IF  "$(CFG)" == "NEW - Win32 Release"


"$(INTDIR)\morphBase.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "NEW - Win32 Debug"


"$(INTDIR)\morphBase.obj"	"$(INTDIR)\morphBase.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rpBase.c

!IF  "$(CFG)" == "NEW - Win32 Release"


"$(INTDIR)\rpBase.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "NEW - Win32 Debug"


"$(INTDIR)\rpBase.obj"	"$(INTDIR)\rpBase.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

