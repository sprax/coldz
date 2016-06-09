# Microsoft Developer Studio Project File - Name="GG" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=GG - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GG.mak".
!MESSAGE 
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

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GG - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "GG - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\h" /I "..\h\m" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# SUBTRACT CPP /WX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "GG - Win32 Release"
# Name "GG - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\blerpFlowTet.c
# End Source File
# Begin Source File

SOURCE=.\blipTri.c
# End Source File
# Begin Source File

SOURCE=.\bnipTri.c
# End Source File
# Begin Source File

SOURCE=.\copyTet.c
# End Source File
# Begin Source File

SOURCE=.\copyTri.c
# End Source File
# Begin Source File

SOURCE=.\gaussFlow.c
# End Source File
# Begin Source File

SOURCE=.\in3.c
# End Source File
# Begin Source File

SOURCE=.\inCrsTet.c
# End Source File
# Begin Source File

SOURCE=.\inCrsTri.c
# End Source File
# Begin Source File

SOURCE=.\inFloTet.c
# End Source File
# Begin Source File

SOURCE=.\inFloTri.c
# End Source File
# Begin Source File

SOURCE=.\inGrid.c
# End Source File
# Begin Source File

SOURCE=.\inMapOld.c
# End Source File
# Begin Source File

SOURCE=.\inMapTri.c
# End Source File
# Begin Source File

SOURCE=.\inParm.c
# End Source File
# Begin Source File

SOURCE=.\inR.c
# End Source File
# Begin Source File

SOURCE=.\inWarpBil.c
# End Source File
# Begin Source File

SOURCE=.\inWarpBilin.c
# End Source File
# Begin Source File

SOURCE=.\inWarpPersp.c
# End Source File
# Begin Source File

SOURCE=.\jkCan.c
# End Source File
# Begin Source File

SOURCE=.\mainN.c
# End Source File
# Begin Source File

SOURCE=.\mapFwdTri.c
# End Source File
# Begin Source File

SOURCE=.\matrixOps.c
# End Source File
# Begin Source File

SOURCE=.\modFwdTri.c
# End Source File
# Begin Source File

SOURCE=.\nin.c
# End Source File
# Begin Source File

SOURCE=.\rowFimg.c
# End Source File
# Begin Source File

SOURCE=.\surfWLSO.c
# End Source File
# Begin Source File

SOURCE=.\unFlags.c
# End Source File
# Begin Source File

SOURCE=.\warpPersp.c
# End Source File
# Begin Source File

SOURCE=.\warpPerspFwd.c
# End Source File
# Begin Source File

SOURCE=.\warpPerspI.c
# End Source File
# Begin Source File

SOURCE=.\warpPerspInv.c
# End Source File
# Begin Source File

SOURCE=.\warpTetBil.c
# End Source File
# Begin Source File

SOURCE=.\warpTetBox.c
# End Source File
# Begin Source File

SOURCE=.\warpTetBoxNen.c
# End Source File
# Begin Source File

SOURCE=.\warpTetBoxy.c
# End Source File
# Begin Source File

SOURCE=.\winDraw.c
# End Source File
# Begin Source File

SOURCE=.\winLooper.c
# End Source File
# Begin Source File

SOURCE=.\winNew.c
# End Source File
# Begin Source File

SOURCE=.\winSliders.c
# End Source File
# Begin Source File

SOURCE=.\winWarp.c
# End Source File
# Begin Source File

SOURCE=.\winZoom.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# End Target
# End Project
