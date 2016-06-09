# Microsoft Developer Studio Generated NMAKE File, Based on MJ.dsp
!IF "$(CFG)" == ""
CFG=MJ - Win32 Debug
!MESSAGE No configuration specified. Defaulting to MJ - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "MJ - Win32 Release" && "$(CFG)" != "MJ - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MJ.mak" CFG="MJ - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MJ - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "MJ - Win32 Debug" (based on "Win32 (x86) Static Library")
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

!IF  "$(CFG)" == "MJ - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\MJ.lib"


CLEAN :
	-@erase "$(INTDIR)\mjBord.obj"
	-@erase "$(INTDIR)\mjBordUnc.obj"
	-@erase "$(INTDIR)\mjCopy.obj"
	-@erase "$(INTDIR)\mjMake.obj"
	-@erase "$(INTDIR)\mjPack.obj"
	-@erase "$(INTDIR)\mjRead.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\MJ.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\MJ.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MJ.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\MJ.lib" 
LIB32_OBJS= \
	"$(INTDIR)\mjBord.obj" \
	"$(INTDIR)\mjBordUnc.obj" \
	"$(INTDIR)\mjCopy.obj" \
	"$(INTDIR)\mjMake.obj" \
	"$(INTDIR)\mjPack.obj" \
	"$(INTDIR)\mjRead.obj"

"$(OUTDIR)\MJ.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "MJ - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\MJ.lib" "$(OUTDIR)\MJ.bsc"


CLEAN :
	-@erase "$(INTDIR)\mjBord.obj"
	-@erase "$(INTDIR)\mjBord.sbr"
	-@erase "$(INTDIR)\mjBordUnc.obj"
	-@erase "$(INTDIR)\mjBordUnc.sbr"
	-@erase "$(INTDIR)\mjCopy.obj"
	-@erase "$(INTDIR)\mjCopy.sbr"
	-@erase "$(INTDIR)\mjMake.obj"
	-@erase "$(INTDIR)\mjMake.sbr"
	-@erase "$(INTDIR)\mjPack.obj"
	-@erase "$(INTDIR)\mjPack.sbr"
	-@erase "$(INTDIR)\mjRead.obj"
	-@erase "$(INTDIR)\mjRead.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\MJ.bsc"
	-@erase "$(OUTDIR)\MJ.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /WX /Gm /GX /ZI /Od /I "..\h" /I "..\h\m" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\MJ.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MJ.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\mjBord.sbr" \
	"$(INTDIR)\mjBordUnc.sbr" \
	"$(INTDIR)\mjCopy.sbr" \
	"$(INTDIR)\mjMake.sbr" \
	"$(INTDIR)\mjPack.sbr" \
	"$(INTDIR)\mjRead.sbr"

"$(OUTDIR)\MJ.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\MJ.lib" 
LIB32_OBJS= \
	"$(INTDIR)\mjBord.obj" \
	"$(INTDIR)\mjBordUnc.obj" \
	"$(INTDIR)\mjCopy.obj" \
	"$(INTDIR)\mjMake.obj" \
	"$(INTDIR)\mjPack.obj" \
	"$(INTDIR)\mjRead.obj"

"$(OUTDIR)\MJ.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
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
!IF EXISTS("MJ.dep")
!INCLUDE "MJ.dep"
!ELSE 
!MESSAGE Warning: cannot find "MJ.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "MJ - Win32 Release" || "$(CFG)" == "MJ - Win32 Debug"
SOURCE=.\mjBord.c

!IF  "$(CFG)" == "MJ - Win32 Release"


"$(INTDIR)\mjBord.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MJ - Win32 Debug"


"$(INTDIR)\mjBord.obj"	"$(INTDIR)\mjBord.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mjBordUnc.c

!IF  "$(CFG)" == "MJ - Win32 Release"


"$(INTDIR)\mjBordUnc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MJ - Win32 Debug"


"$(INTDIR)\mjBordUnc.obj"	"$(INTDIR)\mjBordUnc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mjCopy.c

!IF  "$(CFG)" == "MJ - Win32 Release"


"$(INTDIR)\mjCopy.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MJ - Win32 Debug"


"$(INTDIR)\mjCopy.obj"	"$(INTDIR)\mjCopy.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mjMake.c

!IF  "$(CFG)" == "MJ - Win32 Release"


"$(INTDIR)\mjMake.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MJ - Win32 Debug"


"$(INTDIR)\mjMake.obj"	"$(INTDIR)\mjMake.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mjPack.c

!IF  "$(CFG)" == "MJ - Win32 Release"


"$(INTDIR)\mjPack.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MJ - Win32 Debug"


"$(INTDIR)\mjPack.obj"	"$(INTDIR)\mjPack.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mjRead.c

!IF  "$(CFG)" == "MJ - Win32 Release"


"$(INTDIR)\mjRead.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MJ - Win32 Debug"


"$(INTDIR)\mjRead.obj"	"$(INTDIR)\mjRead.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

