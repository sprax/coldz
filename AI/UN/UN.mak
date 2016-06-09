# Microsoft Developer Studio Generated NMAKE File, Based on UN.dsp
!IF "$(CFG)" == ""
CFG=UN - Win32 Debug
!MESSAGE No configuration specified. Defaulting to UN - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "UN - Win32 Release" && "$(CFG)" != "UN - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "UN.mak" CFG="UN - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "UN - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "UN - Win32 Debug" (based on "Win32 (x86) Static Library")
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

!IF  "$(CFG)" == "UN - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\UN.lib"


CLEAN :
	-@erase "$(INTDIR)\unBug.obj"
	-@erase "$(INTDIR)\unFlags.obj"
	-@erase "$(INTDIR)\unMath.obj"
	-@erase "$(INTDIR)\unMem.obj"
	-@erase "$(INTDIR)\unParse.obj"
	-@erase "$(INTDIR)\unString.obj"
	-@erase "$(INTDIR)\unSys.obj"
	-@erase "$(INTDIR)\unVarg.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\UN.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\UN.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\UN.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\UN.lib" 
LIB32_OBJS= \
	"$(INTDIR)\unBug.obj" \
	"$(INTDIR)\unFlags.obj" \
	"$(INTDIR)\unMath.obj" \
	"$(INTDIR)\unMem.obj" \
	"$(INTDIR)\unParse.obj" \
	"$(INTDIR)\unString.obj" \
	"$(INTDIR)\unSys.obj" \
	"$(INTDIR)\unVarg.obj"

"$(OUTDIR)\UN.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "UN - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\UN.lib" "$(OUTDIR)\UN.bsc"


CLEAN :
	-@erase "$(INTDIR)\unBug.obj"
	-@erase "$(INTDIR)\unBug.sbr"
	-@erase "$(INTDIR)\unFlags.obj"
	-@erase "$(INTDIR)\unFlags.sbr"
	-@erase "$(INTDIR)\unMath.obj"
	-@erase "$(INTDIR)\unMath.sbr"
	-@erase "$(INTDIR)\unMem.obj"
	-@erase "$(INTDIR)\unMem.sbr"
	-@erase "$(INTDIR)\unParse.obj"
	-@erase "$(INTDIR)\unParse.sbr"
	-@erase "$(INTDIR)\unString.obj"
	-@erase "$(INTDIR)\unString.sbr"
	-@erase "$(INTDIR)\unSys.obj"
	-@erase "$(INTDIR)\unSys.sbr"
	-@erase "$(INTDIR)\unVarg.obj"
	-@erase "$(INTDIR)\unVarg.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\UN.bsc"
	-@erase "$(OUTDIR)\UN.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /WX /Gm /GX /ZI /Od /I "..\h\un" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\UN.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\UN.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\unBug.sbr" \
	"$(INTDIR)\unFlags.sbr" \
	"$(INTDIR)\unMath.sbr" \
	"$(INTDIR)\unMem.sbr" \
	"$(INTDIR)\unParse.sbr" \
	"$(INTDIR)\unString.sbr" \
	"$(INTDIR)\unSys.sbr" \
	"$(INTDIR)\unVarg.sbr"

"$(OUTDIR)\UN.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\UN.lib" 
LIB32_OBJS= \
	"$(INTDIR)\unBug.obj" \
	"$(INTDIR)\unFlags.obj" \
	"$(INTDIR)\unMath.obj" \
	"$(INTDIR)\unMem.obj" \
	"$(INTDIR)\unParse.obj" \
	"$(INTDIR)\unString.obj" \
	"$(INTDIR)\unSys.obj" \
	"$(INTDIR)\unVarg.obj"

"$(OUTDIR)\UN.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
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
!IF EXISTS("UN.dep")
!INCLUDE "UN.dep"
!ELSE 
!MESSAGE Warning: cannot find "UN.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "UN - Win32 Release" || "$(CFG)" == "UN - Win32 Debug"
SOURCE=.\unBug.c

!IF  "$(CFG)" == "UN - Win32 Release"


"$(INTDIR)\unBug.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "UN - Win32 Debug"


"$(INTDIR)\unBug.obj"	"$(INTDIR)\unBug.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\unFlags.c

!IF  "$(CFG)" == "UN - Win32 Release"


"$(INTDIR)\unFlags.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "UN - Win32 Debug"


"$(INTDIR)\unFlags.obj"	"$(INTDIR)\unFlags.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\unMath.c

!IF  "$(CFG)" == "UN - Win32 Release"


"$(INTDIR)\unMath.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "UN - Win32 Debug"


"$(INTDIR)\unMath.obj"	"$(INTDIR)\unMath.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\unMem.c

!IF  "$(CFG)" == "UN - Win32 Release"


"$(INTDIR)\unMem.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "UN - Win32 Debug"


"$(INTDIR)\unMem.obj"	"$(INTDIR)\unMem.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\unParse.c

!IF  "$(CFG)" == "UN - Win32 Release"


"$(INTDIR)\unParse.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "UN - Win32 Debug"


"$(INTDIR)\unParse.obj"	"$(INTDIR)\unParse.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\unString.c

!IF  "$(CFG)" == "UN - Win32 Release"


"$(INTDIR)\unString.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "UN - Win32 Debug"


"$(INTDIR)\unString.obj"	"$(INTDIR)\unString.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\unSys.c

!IF  "$(CFG)" == "UN - Win32 Release"


"$(INTDIR)\unSys.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "UN - Win32 Debug"


"$(INTDIR)\unSys.obj"	"$(INTDIR)\unSys.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\unVarg.c

!IF  "$(CFG)" == "UN - Win32 Release"


"$(INTDIR)\unVarg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "UN - Win32 Debug"


"$(INTDIR)\unVarg.obj"	"$(INTDIR)\unVarg.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

