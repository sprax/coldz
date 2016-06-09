# Microsoft Developer Studio Project File - Name="IM" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=IM - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "IM.mak".
!MESSAGE 
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

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "IM - Win32 Release"

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

!ELSEIF  "$(CFG)" == "IM - Win32 Debug"

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
# ADD CPP /nologo /W3 /WX /Gm /GX /ZI /Od /I "..\h" /I "..\h\m" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
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

# Name "IM - Win32 Release"
# Name "IM - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\fxyComb.c
# End Source File
# Begin Source File

SOURCE=.\fxyQuadFwd.c
# End Source File
# Begin Source File

SOURCE=.\imagic.c
# End Source File
# Begin Source File

SOURCE=.\mxyFwd.c
# End Source File
# Begin Source File

SOURCE=.\rgbMark.c
# End Source File
# Begin Source File

SOURCE=.\rgbPlugFlt.c
# End Source File
# Begin Source File

SOURCE=.\rgbSmatAff.c
# End Source File
# Begin Source File

SOURCE=.\rgbSmatAffBil.c
# End Source File
# Begin Source File

SOURCE=.\rgbSmatAffNxt.c
# End Source File
# Begin Source File

SOURCE=.\rgbSmatBil.c
# End Source File
# Begin Source File

SOURCE=.\rgbSmatBil3.c
# End Source File
# Begin Source File

SOURCE=.\rgbSmatBil4.c
# End Source File
# Begin Source File

SOURCE=.\rgbSmatBil8.c
# End Source File
# Begin Source File

SOURCE=.\rgbSmatWts.c
# End Source File
# Begin Source File

SOURCE=.\rgbSpatBil.c
# End Source File
# Begin Source File

SOURCE=.\rowBord.c
# End Source File
# Begin Source File

SOURCE=.\rowCnvt.c
# End Source File
# Begin Source File

SOURCE=.\rowComb.c
# End Source File
# Begin Source File

SOURCE=.\rowCopy.c
# End Source File
# Begin Source File

SOURCE=.\rowCpack.c
# End Source File
# Begin Source File

SOURCE=.\rowDiff.c
# End Source File
# Begin Source File

SOURCE=.\rowFile.c
# End Source File
# Begin Source File

SOURCE=.\rowFillFlt.c
# End Source File
# Begin Source File

SOURCE=.\rowFillUnc.c
# End Source File
# Begin Source File

SOURCE=..\NEW\rowFlow.c
# End Source File
# Begin Source File

SOURCE=.\rowGray.c
# End Source File
# Begin Source File

SOURCE=.\rowMake.c
# End Source File
# Begin Source File

SOURCE=.\rowMult.c
# End Source File
# Begin Source File

SOURCE=.\rowNorm.c
# End Source File
# Begin Source File

SOURCE=.\rowNwts.c
# End Source File
# Begin Source File

SOURCE=.\rowPclr.c
# End Source File
# Begin Source File

SOURCE=.\rowPlugFlt.c
# End Source File
# Begin Source File

SOURCE=.\rowPost.c
# End Source File
# Begin Source File

SOURCE=.\rowSGI.c
# End Source File
# Begin Source File

SOURCE=.\rowSmatBil.c
# End Source File
# Begin Source File

SOURCE=.\rowSmatBiq.c
# End Source File
# Begin Source File

SOURCE=.\rowSmatN.c
# End Source File
# Begin Source File

SOURCE=.\rowSmatSin.c
# End Source File
# Begin Source File

SOURCE=.\rowSpatBil.c
# End Source File
# Begin Source File

SOURCE=.\rowStat.c
# End Source File
# Begin Source File

SOURCE=.\rowWarpFlt.c
# End Source File
# Begin Source File

SOURCE=.\rowWarpUnc.c
# End Source File
# Begin Source File

SOURCE=.\rowZero.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\mxyFwd.h
# End Source File
# End Group
# End Target
# End Project
