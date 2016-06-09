# Microsoft Developer Studio Generated NMAKE File, Based on MGG.dsp
!IF "$(CFG)" == ""
CFG=MGG - Win32 Debug
!MESSAGE No configuration specified. Defaulting to MGG - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "MGG - Win32 Release" && "$(CFG)" != "MGG - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MGG.mak" CFG="MGG - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MGG - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "MGG - Win32 Debug" (based on "Win32 (x86) Static Library")
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

!IF  "$(CFG)" == "MGG - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\MGG.lib"


CLEAN :
	-@erase "$(INTDIR)\jj.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\MGG.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\MGG.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MGG.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\MGG.lib" 
LIB32_OBJS= \
	"$(INTDIR)\jj.obj"

"$(OUTDIR)\MGG.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "MGG - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\MGG.lib" "$(OUTDIR)\MGG.bsc"


CLEAN :
	-@erase "$(INTDIR)\jj.obj"
	-@erase "$(INTDIR)\jj.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\MGG.bsc"
	-@erase "$(OUTDIR)\MGG.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /I "..\h" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\MGG.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MGG.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\jj.sbr"

"$(OUTDIR)\MGG.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\MGG.lib" 
LIB32_OBJS= \
	"$(INTDIR)\jj.obj"

"$(OUTDIR)\MGG.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
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
!IF EXISTS("MGG.dep")
!INCLUDE "MGG.dep"
!ELSE 
!MESSAGE Warning: cannot find "MGG.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "MGG - Win32 Release" || "$(CFG)" == "MGG - Win32 Debug"
SOURCE=.\jj.cpp

!IF  "$(CFG)" == "MGG - Win32 Release"


"$(INTDIR)\jj.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MGG - Win32 Debug"


"$(INTDIR)\jj.obj"	"$(INTDIR)\jj.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

