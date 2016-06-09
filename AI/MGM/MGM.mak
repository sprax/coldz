# Microsoft Developer Studio Generated NMAKE File, Based on MGM.dsp
!IF "$(CFG)" == ""
CFG=MGM - Win32 Debug
!MESSAGE No configuration specified. Defaulting to MGM - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "MGM - Win32 Release" && "$(CFG)" != "MGM - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MGM.mak" CFG="MGM - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MGM - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "MGM - Win32 Debug" (based on "Win32 (x86) Console Application")
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

!IF  "$(CFG)" == "MGM - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\MGM.exe"


CLEAN :
	-@erase "$(INTDIR)\jj.obj"
	-@erase "$(INTDIR)\matrixOps.obj"
	-@erase "$(INTDIR)\rowCopy.obj"
	-@erase "$(INTDIR)\rowMake.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\MGM.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\MGM.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MGM.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\MGM.pdb" /machine:I386 /out:"$(OUTDIR)\MGM.exe" 
LINK32_OBJS= \
	"$(INTDIR)\jj.obj" \
	"$(INTDIR)\matrixOps.obj" \
	"$(INTDIR)\rowMake.obj" \
	"$(INTDIR)\rowCopy.obj"

"$(OUTDIR)\MGM.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "MGM - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\MGM.exe" "$(OUTDIR)\MGM.bsc"


CLEAN :
	-@erase "$(INTDIR)\jj.obj"
	-@erase "$(INTDIR)\jj.sbr"
	-@erase "$(INTDIR)\matrixOps.obj"
	-@erase "$(INTDIR)\matrixOps.sbr"
	-@erase "$(INTDIR)\rowCopy.obj"
	-@erase "$(INTDIR)\rowCopy.sbr"
	-@erase "$(INTDIR)\rowMake.obj"
	-@erase "$(INTDIR)\rowMake.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\MGM.bsc"
	-@erase "$(OUTDIR)\MGM.exe"
	-@erase "$(OUTDIR)\MGM.ilk"
	-@erase "$(OUTDIR)\MGM.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /I "..\h" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\MGM.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MGM.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\jj.sbr" \
	"$(INTDIR)\matrixOps.sbr" \
	"$(INTDIR)\rowMake.sbr" \
	"$(INTDIR)\rowCopy.sbr"

"$(OUTDIR)\MGM.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\MGM.pdb" /debug /machine:I386 /out:"$(OUTDIR)\MGM.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\jj.obj" \
	"$(INTDIR)\matrixOps.obj" \
	"$(INTDIR)\rowMake.obj" \
	"$(INTDIR)\rowCopy.obj"

"$(OUTDIR)\MGM.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
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
!IF EXISTS("MGM.dep")
!INCLUDE "MGM.dep"
!ELSE 
!MESSAGE Warning: cannot find "MGM.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "MGM - Win32 Release" || "$(CFG)" == "MGM - Win32 Debug"
SOURCE=.\jj.cpp

!IF  "$(CFG)" == "MGM - Win32 Release"


"$(INTDIR)\jj.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MGM - Win32 Debug"


"$(INTDIR)\jj.obj"	"$(INTDIR)\jj.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\matrixOps.cpp

!IF  "$(CFG)" == "MGM - Win32 Release"


"$(INTDIR)\matrixOps.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MGM - Win32 Debug"


"$(INTDIR)\matrixOps.obj"	"$(INTDIR)\matrixOps.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rowCopy.cpp

!IF  "$(CFG)" == "MGM - Win32 Release"


"$(INTDIR)\rowCopy.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MGM - Win32 Debug"


"$(INTDIR)\rowCopy.obj"	"$(INTDIR)\rowCopy.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rowMake.cpp

!IF  "$(CFG)" == "MGM - Win32 Release"


"$(INTDIR)\rowMake.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MGM - Win32 Debug"


"$(INTDIR)\rowMake.obj"	"$(INTDIR)\rowMake.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

