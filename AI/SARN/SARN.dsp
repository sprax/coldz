# Microsoft Developer Studio Project File - Name="SARN" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=SARN - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SARN.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SARN.mak" CFG="SARN - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SARN - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "SARN - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SARN - Win32 Release"

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

!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\h" /I "..\h\sarnoff" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
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

# Name "SARN - Win32 Release"
# Name "SARN - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\0decode.c
# End Source File
# Begin Source File

SOURCE=.\0encode.c
# End Source File
# Begin Source File

SOURCE=.\affine_est.c
# End Source File
# Begin Source File

SOURCE=.\arralloc.c
# End Source File
# Begin Source File

SOURCE=.\barralloc.c
# End Source File
# Begin Source File

SOURCE=.\bimgalloc.c
# End Source File
# Begin Source File

SOURCE=.\bimgio.c
# End Source File
# Begin Source File

SOURCE=.\blkmot.c
# End Source File
# Begin Source File

SOURCE=.\bnzrhist.c
# End Source File
# Begin Source File

SOURCE=.\bnzrstats.c
# End Source File
# Begin Source File

SOURCE=.\bpyralloc.c
# End Source File
# Begin Source File

SOURCE=.\bqmfpyralloc.c
# End Source File
# Begin Source File

SOURCE=.\bredexp.c
# End Source File
# Begin Source File

SOURCE=.\breflect.c
# End Source File
# Begin Source File

SOURCE=.\bset.c
# End Source File
# Begin Source File

SOURCE=.\bstats.c
# End Source File
# Begin Source File

SOURCE=.\btoflib.c
# End Source File
# Begin Source File

SOURCE=.\btol.c
# End Source File
# Begin Source File

SOURCE=.\btos.c
# End Source File
# Begin Source File

SOURCE=.\bvhist.c
# End Source File
# Begin Source File

SOURCE=.\bvstats.c
# End Source File
# Begin Source File

SOURCE=.\bzrstats.c
# End Source File
# Begin Source File

SOURCE=.\darralloc.c
# End Source File
# Begin Source File

SOURCE=.\dblredexp.c
# End Source File
# Begin Source File

SOURCE=.\dctimg.c
# End Source File
# Begin Source File

SOURCE=.\die.c
# End Source File
# Begin Source File

SOURCE=.\dimgalloc.c
# End Source File
# Begin Source File

SOURCE=.\eigens2x2.c
# End Source File
# Begin Source File

SOURCE=.\eigsrt.c
# End Source File
# Begin Source File

SOURCE=.\farralloc.c
# End Source File
# Begin Source File

SOURCE=.\fclip.c
# End Source File
# Begin Source File

SOURCE=.\fcopy.c
# End Source File
# Begin Source File

SOURCE=.\fdcts.c
# End Source File
# Begin Source File

SOURCE=.\fdctsub.c
# End Source File
# Begin Source File

SOURCE=.\ffts.c
# End Source File
# Begin Source File

SOURCE=.\fileutils.c
# End Source File
# Begin Source File

SOURCE=.\filtio.c
# End Source File
# Begin Source File

SOURCE=.\filtsub.c
# End Source File
# Begin Source File

SOURCE=.\fimgalloc.c
# End Source File
# Begin Source File

SOURCE=.\fimgimg.c
# End Source File
# Begin Source File

SOURCE=.\fimgio.c
# End Source File
# Begin Source File

SOURCE=.\fimgop.c
# End Source File
# Begin Source File

SOURCE=.\fminmax.c
# End Source File
# Begin Source File

SOURCE=.\fmse.c
# End Source File
# Begin Source File

SOURCE=.\fmsv.c
# End Source File
# Begin Source File

SOURCE=.\fnzrhist.c
# End Source File
# Begin Source File

SOURCE=.\fnzrstats.c
# End Source File
# Begin Source File

SOURCE=.\fpyralloc.c
# End Source File
# Begin Source File

SOURCE=.\fredexp.c
# End Source File
# Begin Source File

SOURCE=.\fsdlap.c
# End Source File
# Begin Source File

SOURCE=.\fset.c
# End Source File
# Begin Source File

SOURCE=.\fstats.c
# End Source File
# Begin Source File

SOURCE=.\ftoblib.c
# End Source File
# Begin Source File

SOURCE=.\ftol.c
# End Source File
# Begin Source File

SOURCE=.\ftos.c
# End Source File
# Begin Source File

SOURCE=.\fvhist.c
# End Source File
# Begin Source File

SOURCE=.\fvstats.c
# End Source File
# Begin Source File

SOURCE=.\fzrstats.c
# End Source File
# Begin Source File

SOURCE=.\getblock.c
# End Source File
# Begin Source File

SOURCE=.\getnzrstats.c
# End Source File
# Begin Source File

SOURCE=.\getvector.c
# End Source File
# Begin Source File

SOURCE=.\getvstats.c
# End Source File
# Begin Source File

SOURCE=.\getzrstats.c
# End Source File
# Begin Source File

SOURCE=.\huffman.c
# End Source File
# Begin Source File

SOURCE=.\hvqmfpyralloc.c
# End Source File
# Begin Source File

SOURCE=.\hvqmfredexp.c
# End Source File
# Begin Source File

SOURCE=.\img_util.c
# End Source File
# Begin Source File

SOURCE=.\imgalloc.c
# End Source File
# Begin Source File

SOURCE=.\jacobi.c
# End Source File
# Begin Source File

SOURCE=.\kernel.c
# End Source File
# Begin Source File

SOURCE=.\laputq.c
# End Source File
# Begin Source File

SOURCE=.\larralloc.c
# End Source File
# Begin Source File

SOURCE=.\limgalloc.c
# End Source File
# Begin Source File

SOURCE=.\limgio.c
# End Source File
# Begin Source File

SOURCE=.\lkflow.c
# End Source File
# Begin Source File

SOURCE=.\lsqsub.c
# End Source File
# Begin Source File

SOURCE=.\lsqsub_fwd.c
# End Source File
# Begin Source File

SOURCE=.\ltob.c
# End Source File
# Begin Source File

SOURCE=.\ltof.c
# End Source File
# Begin Source File

SOURCE=.\ltos.c
# End Source File
# Begin Source File

SOURCE=.\lubksb.c
# End Source File
# Begin Source File

SOURCE=.\ludcmp.c
# End Source File
# Begin Source File

SOURCE=.\mathtls.c
# End Source File
# Begin Source File

SOURCE=.\mmath.c
# End Source File
# Begin Source File

SOURCE=.\nzrhist.c
# End Source File
# Begin Source File

SOURCE=.\profile.c
# End Source File
# Begin Source File

SOURCE=.\putblock.c
# End Source File
# Begin Source File

SOURCE=.\putvector.c
# End Source File
# Begin Source File

SOURCE=.\qmfpyralloc.c
# End Source File
# Begin Source File

SOURCE=.\qmfredexp.c
# End Source File
# Begin Source File

SOURCE=.\qmfsub.c
# End Source File
# Begin Source File

SOURCE=.\ratesub.c
# End Source File
# Begin Source File

SOURCE=.\redexp.c
# End Source File
# Begin Source File

SOURCE=.\redexp_fwd.c
# End Source File
# Begin Source File

SOURCE=.\redirected.c
# End Source File
# Begin Source File

SOURCE=.\reflect.c
# End Source File
# Begin Source File

SOURCE=.\roi.c
# End Source File
# Begin Source File

SOURCE=.\sarralloc.c
# End Source File
# Begin Source File

SOURCE=.\seqio.c
# End Source File
# Begin Source File

SOURCE=.\sim_est.c
# End Source File
# Begin Source File

SOURCE=.\simgalloc.c
# End Source File
# Begin Source File

SOURCE=.\simgimg.c
# End Source File
# Begin Source File

SOURCE=.\simgio.c
# End Source File
# Begin Source File

SOURCE=.\snzrhist.c
# End Source File
# Begin Source File

SOURCE=.\snzrstats.c
# End Source File
# Begin Source File

SOURCE=.\spyralloc.c
# End Source File
# Begin Source File

SOURCE=.\sqmfpyralloc.c
# End Source File
# Begin Source File

SOURCE=.\squeezer.c
# End Source File
# Begin Source File

SOURCE=.\sstats.c
# End Source File
# Begin Source File

SOURCE=.\stob.c
# End Source File
# Begin Source File

SOURCE=.\stof.c
# End Source File
# Begin Source File

SOURCE=.\stol.c
# End Source File
# Begin Source File

SOURCE=.\svdlib.c
# End Source File
# Begin Source File

SOURCE=.\svhist.c
# End Source File
# Begin Source File

SOURCE=.\svstats.c
# End Source File
# Begin Source File

SOURCE=.\szrhist.c
# End Source File
# Begin Source File

SOURCE=.\szrstats.c
# End Source File
# Begin Source File

SOURCE=.\trans_est.c
# End Source File
# Begin Source File

SOURCE=.\triangulate.c
# End Source File
# Begin Source File

SOURCE=.\ucoders.c
# End Source File
# Begin Source File

SOURCE=.\uqnt.c
# End Source File
# Begin Source File

SOURCE=.\utcoders.c
# End Source File
# Begin Source File

SOURCE=.\utqnt.c
# End Source File
# Begin Source File

SOURCE=.\vhist.c
# End Source File
# Begin Source File

SOURCE=.\vqio.c
# End Source File
# Begin Source File

SOURCE=.\warpers.c
# End Source File
# Begin Source File

SOURCE=.\warps.c
# End Source File
# Begin Source File

SOURCE=.\zrhist.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# End Target
# End Project
