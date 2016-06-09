# Microsoft Developer Studio Generated NMAKE File, Based on SARN.dsp
!IF "$(CFG)" == ""
CFG=SARN - Win32 Debug
!MESSAGE No configuration specified. Defaulting to SARN - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "SARN - Win32 Release" && "$(CFG)" != "SARN - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SARN - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\SARN.lib"


CLEAN :
	-@erase "$(INTDIR)\0decode.obj"
	-@erase "$(INTDIR)\0encode.obj"
	-@erase "$(INTDIR)\affine_est.obj"
	-@erase "$(INTDIR)\arralloc.obj"
	-@erase "$(INTDIR)\barralloc.obj"
	-@erase "$(INTDIR)\bimgalloc.obj"
	-@erase "$(INTDIR)\bimgio.obj"
	-@erase "$(INTDIR)\blkmot.obj"
	-@erase "$(INTDIR)\bnzrhist.obj"
	-@erase "$(INTDIR)\bnzrstats.obj"
	-@erase "$(INTDIR)\bpyralloc.obj"
	-@erase "$(INTDIR)\bqmfpyralloc.obj"
	-@erase "$(INTDIR)\bredexp.obj"
	-@erase "$(INTDIR)\breflect.obj"
	-@erase "$(INTDIR)\bset.obj"
	-@erase "$(INTDIR)\bstats.obj"
	-@erase "$(INTDIR)\btoflib.obj"
	-@erase "$(INTDIR)\btol.obj"
	-@erase "$(INTDIR)\btos.obj"
	-@erase "$(INTDIR)\bvhist.obj"
	-@erase "$(INTDIR)\bvstats.obj"
	-@erase "$(INTDIR)\bzrstats.obj"
	-@erase "$(INTDIR)\darralloc.obj"
	-@erase "$(INTDIR)\dblredexp.obj"
	-@erase "$(INTDIR)\dctimg.obj"
	-@erase "$(INTDIR)\die.obj"
	-@erase "$(INTDIR)\dimgalloc.obj"
	-@erase "$(INTDIR)\eigens2x2.obj"
	-@erase "$(INTDIR)\eigsrt.obj"
	-@erase "$(INTDIR)\farralloc.obj"
	-@erase "$(INTDIR)\fclip.obj"
	-@erase "$(INTDIR)\fcopy.obj"
	-@erase "$(INTDIR)\fdcts.obj"
	-@erase "$(INTDIR)\fdctsub.obj"
	-@erase "$(INTDIR)\ffts.obj"
	-@erase "$(INTDIR)\fileutils.obj"
	-@erase "$(INTDIR)\filtio.obj"
	-@erase "$(INTDIR)\filtsub.obj"
	-@erase "$(INTDIR)\fimgalloc.obj"
	-@erase "$(INTDIR)\fimgimg.obj"
	-@erase "$(INTDIR)\fimgio.obj"
	-@erase "$(INTDIR)\fimgop.obj"
	-@erase "$(INTDIR)\fminmax.obj"
	-@erase "$(INTDIR)\fmse.obj"
	-@erase "$(INTDIR)\fmsv.obj"
	-@erase "$(INTDIR)\fnzrhist.obj"
	-@erase "$(INTDIR)\fnzrstats.obj"
	-@erase "$(INTDIR)\fpyralloc.obj"
	-@erase "$(INTDIR)\fredexp.obj"
	-@erase "$(INTDIR)\fsdlap.obj"
	-@erase "$(INTDIR)\fset.obj"
	-@erase "$(INTDIR)\fstats.obj"
	-@erase "$(INTDIR)\ftoblib.obj"
	-@erase "$(INTDIR)\ftol.obj"
	-@erase "$(INTDIR)\ftos.obj"
	-@erase "$(INTDIR)\fvhist.obj"
	-@erase "$(INTDIR)\fvstats.obj"
	-@erase "$(INTDIR)\fzrstats.obj"
	-@erase "$(INTDIR)\getblock.obj"
	-@erase "$(INTDIR)\getnzrstats.obj"
	-@erase "$(INTDIR)\getvector.obj"
	-@erase "$(INTDIR)\getvstats.obj"
	-@erase "$(INTDIR)\getzrstats.obj"
	-@erase "$(INTDIR)\huffman.obj"
	-@erase "$(INTDIR)\hvqmfpyralloc.obj"
	-@erase "$(INTDIR)\hvqmfredexp.obj"
	-@erase "$(INTDIR)\img_util.obj"
	-@erase "$(INTDIR)\imgalloc.obj"
	-@erase "$(INTDIR)\jacobi.obj"
	-@erase "$(INTDIR)\kernel.obj"
	-@erase "$(INTDIR)\laputq.obj"
	-@erase "$(INTDIR)\larralloc.obj"
	-@erase "$(INTDIR)\limgalloc.obj"
	-@erase "$(INTDIR)\limgio.obj"
	-@erase "$(INTDIR)\lkflow.obj"
	-@erase "$(INTDIR)\lsqsub.obj"
	-@erase "$(INTDIR)\lsqsub_fwd.obj"
	-@erase "$(INTDIR)\ltob.obj"
	-@erase "$(INTDIR)\ltof.obj"
	-@erase "$(INTDIR)\ltos.obj"
	-@erase "$(INTDIR)\lubksb.obj"
	-@erase "$(INTDIR)\ludcmp.obj"
	-@erase "$(INTDIR)\mathtls.obj"
	-@erase "$(INTDIR)\mmath.obj"
	-@erase "$(INTDIR)\nzrhist.obj"
	-@erase "$(INTDIR)\profile.obj"
	-@erase "$(INTDIR)\putblock.obj"
	-@erase "$(INTDIR)\putvector.obj"
	-@erase "$(INTDIR)\qmfpyralloc.obj"
	-@erase "$(INTDIR)\qmfredexp.obj"
	-@erase "$(INTDIR)\qmfsub.obj"
	-@erase "$(INTDIR)\ratesub.obj"
	-@erase "$(INTDIR)\redexp.obj"
	-@erase "$(INTDIR)\redexp_fwd.obj"
	-@erase "$(INTDIR)\redirected.obj"
	-@erase "$(INTDIR)\reflect.obj"
	-@erase "$(INTDIR)\roi.obj"
	-@erase "$(INTDIR)\sarralloc.obj"
	-@erase "$(INTDIR)\seqio.obj"
	-@erase "$(INTDIR)\sim_est.obj"
	-@erase "$(INTDIR)\simgalloc.obj"
	-@erase "$(INTDIR)\simgimg.obj"
	-@erase "$(INTDIR)\simgio.obj"
	-@erase "$(INTDIR)\snzrhist.obj"
	-@erase "$(INTDIR)\snzrstats.obj"
	-@erase "$(INTDIR)\spyralloc.obj"
	-@erase "$(INTDIR)\sqmfpyralloc.obj"
	-@erase "$(INTDIR)\squeezer.obj"
	-@erase "$(INTDIR)\sstats.obj"
	-@erase "$(INTDIR)\stob.obj"
	-@erase "$(INTDIR)\stof.obj"
	-@erase "$(INTDIR)\stol.obj"
	-@erase "$(INTDIR)\svdlib.obj"
	-@erase "$(INTDIR)\svhist.obj"
	-@erase "$(INTDIR)\svstats.obj"
	-@erase "$(INTDIR)\szrhist.obj"
	-@erase "$(INTDIR)\szrstats.obj"
	-@erase "$(INTDIR)\trans_est.obj"
	-@erase "$(INTDIR)\triangulate.obj"
	-@erase "$(INTDIR)\ucoders.obj"
	-@erase "$(INTDIR)\uqnt.obj"
	-@erase "$(INTDIR)\utcoders.obj"
	-@erase "$(INTDIR)\utqnt.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vhist.obj"
	-@erase "$(INTDIR)\vqio.obj"
	-@erase "$(INTDIR)\warpers.obj"
	-@erase "$(INTDIR)\warps.obj"
	-@erase "$(INTDIR)\zrhist.obj"
	-@erase "$(OUTDIR)\SARN.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\SARN.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SARN.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\SARN.lib" 
LIB32_OBJS= \
	"$(INTDIR)\0decode.obj" \
	"$(INTDIR)\0encode.obj" \
	"$(INTDIR)\affine_est.obj" \
	"$(INTDIR)\arralloc.obj" \
	"$(INTDIR)\barralloc.obj" \
	"$(INTDIR)\bimgalloc.obj" \
	"$(INTDIR)\bimgio.obj" \
	"$(INTDIR)\blkmot.obj" \
	"$(INTDIR)\bnzrhist.obj" \
	"$(INTDIR)\bnzrstats.obj" \
	"$(INTDIR)\bpyralloc.obj" \
	"$(INTDIR)\bqmfpyralloc.obj" \
	"$(INTDIR)\bredexp.obj" \
	"$(INTDIR)\breflect.obj" \
	"$(INTDIR)\bset.obj" \
	"$(INTDIR)\bstats.obj" \
	"$(INTDIR)\btoflib.obj" \
	"$(INTDIR)\btol.obj" \
	"$(INTDIR)\btos.obj" \
	"$(INTDIR)\bvhist.obj" \
	"$(INTDIR)\bvstats.obj" \
	"$(INTDIR)\bzrstats.obj" \
	"$(INTDIR)\darralloc.obj" \
	"$(INTDIR)\dblredexp.obj" \
	"$(INTDIR)\dctimg.obj" \
	"$(INTDIR)\die.obj" \
	"$(INTDIR)\dimgalloc.obj" \
	"$(INTDIR)\eigens2x2.obj" \
	"$(INTDIR)\eigsrt.obj" \
	"$(INTDIR)\farralloc.obj" \
	"$(INTDIR)\fclip.obj" \
	"$(INTDIR)\fcopy.obj" \
	"$(INTDIR)\fdcts.obj" \
	"$(INTDIR)\fdctsub.obj" \
	"$(INTDIR)\ffts.obj" \
	"$(INTDIR)\fileutils.obj" \
	"$(INTDIR)\filtio.obj" \
	"$(INTDIR)\filtsub.obj" \
	"$(INTDIR)\fimgalloc.obj" \
	"$(INTDIR)\fimgimg.obj" \
	"$(INTDIR)\fimgio.obj" \
	"$(INTDIR)\fimgop.obj" \
	"$(INTDIR)\fminmax.obj" \
	"$(INTDIR)\fmse.obj" \
	"$(INTDIR)\fmsv.obj" \
	"$(INTDIR)\fnzrhist.obj" \
	"$(INTDIR)\fnzrstats.obj" \
	"$(INTDIR)\fpyralloc.obj" \
	"$(INTDIR)\fredexp.obj" \
	"$(INTDIR)\fsdlap.obj" \
	"$(INTDIR)\fset.obj" \
	"$(INTDIR)\fstats.obj" \
	"$(INTDIR)\ftoblib.obj" \
	"$(INTDIR)\ftol.obj" \
	"$(INTDIR)\ftos.obj" \
	"$(INTDIR)\fvhist.obj" \
	"$(INTDIR)\fvstats.obj" \
	"$(INTDIR)\fzrstats.obj" \
	"$(INTDIR)\getblock.obj" \
	"$(INTDIR)\getnzrstats.obj" \
	"$(INTDIR)\getvector.obj" \
	"$(INTDIR)\getvstats.obj" \
	"$(INTDIR)\getzrstats.obj" \
	"$(INTDIR)\huffman.obj" \
	"$(INTDIR)\hvqmfpyralloc.obj" \
	"$(INTDIR)\hvqmfredexp.obj" \
	"$(INTDIR)\img_util.obj" \
	"$(INTDIR)\imgalloc.obj" \
	"$(INTDIR)\jacobi.obj" \
	"$(INTDIR)\kernel.obj" \
	"$(INTDIR)\laputq.obj" \
	"$(INTDIR)\larralloc.obj" \
	"$(INTDIR)\limgalloc.obj" \
	"$(INTDIR)\limgio.obj" \
	"$(INTDIR)\lkflow.obj" \
	"$(INTDIR)\lsqsub.obj" \
	"$(INTDIR)\lsqsub_fwd.obj" \
	"$(INTDIR)\ltob.obj" \
	"$(INTDIR)\ltof.obj" \
	"$(INTDIR)\ltos.obj" \
	"$(INTDIR)\lubksb.obj" \
	"$(INTDIR)\ludcmp.obj" \
	"$(INTDIR)\mathtls.obj" \
	"$(INTDIR)\mmath.obj" \
	"$(INTDIR)\nzrhist.obj" \
	"$(INTDIR)\profile.obj" \
	"$(INTDIR)\putblock.obj" \
	"$(INTDIR)\putvector.obj" \
	"$(INTDIR)\qmfpyralloc.obj" \
	"$(INTDIR)\qmfredexp.obj" \
	"$(INTDIR)\qmfsub.obj" \
	"$(INTDIR)\ratesub.obj" \
	"$(INTDIR)\redexp.obj" \
	"$(INTDIR)\redexp_fwd.obj" \
	"$(INTDIR)\redirected.obj" \
	"$(INTDIR)\reflect.obj" \
	"$(INTDIR)\roi.obj" \
	"$(INTDIR)\sarralloc.obj" \
	"$(INTDIR)\seqio.obj" \
	"$(INTDIR)\sim_est.obj" \
	"$(INTDIR)\simgalloc.obj" \
	"$(INTDIR)\simgimg.obj" \
	"$(INTDIR)\simgio.obj" \
	"$(INTDIR)\snzrhist.obj" \
	"$(INTDIR)\snzrstats.obj" \
	"$(INTDIR)\spyralloc.obj" \
	"$(INTDIR)\sqmfpyralloc.obj" \
	"$(INTDIR)\squeezer.obj" \
	"$(INTDIR)\sstats.obj" \
	"$(INTDIR)\stob.obj" \
	"$(INTDIR)\stof.obj" \
	"$(INTDIR)\stol.obj" \
	"$(INTDIR)\svdlib.obj" \
	"$(INTDIR)\svhist.obj" \
	"$(INTDIR)\svstats.obj" \
	"$(INTDIR)\szrhist.obj" \
	"$(INTDIR)\szrstats.obj" \
	"$(INTDIR)\trans_est.obj" \
	"$(INTDIR)\triangulate.obj" \
	"$(INTDIR)\ucoders.obj" \
	"$(INTDIR)\uqnt.obj" \
	"$(INTDIR)\utcoders.obj" \
	"$(INTDIR)\utqnt.obj" \
	"$(INTDIR)\vhist.obj" \
	"$(INTDIR)\vqio.obj" \
	"$(INTDIR)\warpers.obj" \
	"$(INTDIR)\warps.obj" \
	"$(INTDIR)\zrhist.obj"

"$(OUTDIR)\SARN.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\SARN.lib" "$(OUTDIR)\SARN.bsc"


CLEAN :
	-@erase "$(INTDIR)\0decode.obj"
	-@erase "$(INTDIR)\0decode.sbr"
	-@erase "$(INTDIR)\0encode.obj"
	-@erase "$(INTDIR)\0encode.sbr"
	-@erase "$(INTDIR)\affine_est.obj"
	-@erase "$(INTDIR)\affine_est.sbr"
	-@erase "$(INTDIR)\arralloc.obj"
	-@erase "$(INTDIR)\arralloc.sbr"
	-@erase "$(INTDIR)\barralloc.obj"
	-@erase "$(INTDIR)\barralloc.sbr"
	-@erase "$(INTDIR)\bimgalloc.obj"
	-@erase "$(INTDIR)\bimgalloc.sbr"
	-@erase "$(INTDIR)\bimgio.obj"
	-@erase "$(INTDIR)\bimgio.sbr"
	-@erase "$(INTDIR)\blkmot.obj"
	-@erase "$(INTDIR)\blkmot.sbr"
	-@erase "$(INTDIR)\bnzrhist.obj"
	-@erase "$(INTDIR)\bnzrhist.sbr"
	-@erase "$(INTDIR)\bnzrstats.obj"
	-@erase "$(INTDIR)\bnzrstats.sbr"
	-@erase "$(INTDIR)\bpyralloc.obj"
	-@erase "$(INTDIR)\bpyralloc.sbr"
	-@erase "$(INTDIR)\bqmfpyralloc.obj"
	-@erase "$(INTDIR)\bqmfpyralloc.sbr"
	-@erase "$(INTDIR)\bredexp.obj"
	-@erase "$(INTDIR)\bredexp.sbr"
	-@erase "$(INTDIR)\breflect.obj"
	-@erase "$(INTDIR)\breflect.sbr"
	-@erase "$(INTDIR)\bset.obj"
	-@erase "$(INTDIR)\bset.sbr"
	-@erase "$(INTDIR)\bstats.obj"
	-@erase "$(INTDIR)\bstats.sbr"
	-@erase "$(INTDIR)\btoflib.obj"
	-@erase "$(INTDIR)\btoflib.sbr"
	-@erase "$(INTDIR)\btol.obj"
	-@erase "$(INTDIR)\btol.sbr"
	-@erase "$(INTDIR)\btos.obj"
	-@erase "$(INTDIR)\btos.sbr"
	-@erase "$(INTDIR)\bvhist.obj"
	-@erase "$(INTDIR)\bvhist.sbr"
	-@erase "$(INTDIR)\bvstats.obj"
	-@erase "$(INTDIR)\bvstats.sbr"
	-@erase "$(INTDIR)\bzrstats.obj"
	-@erase "$(INTDIR)\bzrstats.sbr"
	-@erase "$(INTDIR)\darralloc.obj"
	-@erase "$(INTDIR)\darralloc.sbr"
	-@erase "$(INTDIR)\dblredexp.obj"
	-@erase "$(INTDIR)\dblredexp.sbr"
	-@erase "$(INTDIR)\dctimg.obj"
	-@erase "$(INTDIR)\dctimg.sbr"
	-@erase "$(INTDIR)\die.obj"
	-@erase "$(INTDIR)\die.sbr"
	-@erase "$(INTDIR)\dimgalloc.obj"
	-@erase "$(INTDIR)\dimgalloc.sbr"
	-@erase "$(INTDIR)\eigens2x2.obj"
	-@erase "$(INTDIR)\eigens2x2.sbr"
	-@erase "$(INTDIR)\eigsrt.obj"
	-@erase "$(INTDIR)\eigsrt.sbr"
	-@erase "$(INTDIR)\farralloc.obj"
	-@erase "$(INTDIR)\farralloc.sbr"
	-@erase "$(INTDIR)\fclip.obj"
	-@erase "$(INTDIR)\fclip.sbr"
	-@erase "$(INTDIR)\fcopy.obj"
	-@erase "$(INTDIR)\fcopy.sbr"
	-@erase "$(INTDIR)\fdcts.obj"
	-@erase "$(INTDIR)\fdcts.sbr"
	-@erase "$(INTDIR)\fdctsub.obj"
	-@erase "$(INTDIR)\fdctsub.sbr"
	-@erase "$(INTDIR)\ffts.obj"
	-@erase "$(INTDIR)\ffts.sbr"
	-@erase "$(INTDIR)\fileutils.obj"
	-@erase "$(INTDIR)\fileutils.sbr"
	-@erase "$(INTDIR)\filtio.obj"
	-@erase "$(INTDIR)\filtio.sbr"
	-@erase "$(INTDIR)\filtsub.obj"
	-@erase "$(INTDIR)\filtsub.sbr"
	-@erase "$(INTDIR)\fimgalloc.obj"
	-@erase "$(INTDIR)\fimgalloc.sbr"
	-@erase "$(INTDIR)\fimgimg.obj"
	-@erase "$(INTDIR)\fimgimg.sbr"
	-@erase "$(INTDIR)\fimgio.obj"
	-@erase "$(INTDIR)\fimgio.sbr"
	-@erase "$(INTDIR)\fimgop.obj"
	-@erase "$(INTDIR)\fimgop.sbr"
	-@erase "$(INTDIR)\fminmax.obj"
	-@erase "$(INTDIR)\fminmax.sbr"
	-@erase "$(INTDIR)\fmse.obj"
	-@erase "$(INTDIR)\fmse.sbr"
	-@erase "$(INTDIR)\fmsv.obj"
	-@erase "$(INTDIR)\fmsv.sbr"
	-@erase "$(INTDIR)\fnzrhist.obj"
	-@erase "$(INTDIR)\fnzrhist.sbr"
	-@erase "$(INTDIR)\fnzrstats.obj"
	-@erase "$(INTDIR)\fnzrstats.sbr"
	-@erase "$(INTDIR)\fpyralloc.obj"
	-@erase "$(INTDIR)\fpyralloc.sbr"
	-@erase "$(INTDIR)\fredexp.obj"
	-@erase "$(INTDIR)\fredexp.sbr"
	-@erase "$(INTDIR)\fsdlap.obj"
	-@erase "$(INTDIR)\fsdlap.sbr"
	-@erase "$(INTDIR)\fset.obj"
	-@erase "$(INTDIR)\fset.sbr"
	-@erase "$(INTDIR)\fstats.obj"
	-@erase "$(INTDIR)\fstats.sbr"
	-@erase "$(INTDIR)\ftoblib.obj"
	-@erase "$(INTDIR)\ftoblib.sbr"
	-@erase "$(INTDIR)\ftol.obj"
	-@erase "$(INTDIR)\ftol.sbr"
	-@erase "$(INTDIR)\ftos.obj"
	-@erase "$(INTDIR)\ftos.sbr"
	-@erase "$(INTDIR)\fvhist.obj"
	-@erase "$(INTDIR)\fvhist.sbr"
	-@erase "$(INTDIR)\fvstats.obj"
	-@erase "$(INTDIR)\fvstats.sbr"
	-@erase "$(INTDIR)\fzrstats.obj"
	-@erase "$(INTDIR)\fzrstats.sbr"
	-@erase "$(INTDIR)\getblock.obj"
	-@erase "$(INTDIR)\getblock.sbr"
	-@erase "$(INTDIR)\getnzrstats.obj"
	-@erase "$(INTDIR)\getnzrstats.sbr"
	-@erase "$(INTDIR)\getvector.obj"
	-@erase "$(INTDIR)\getvector.sbr"
	-@erase "$(INTDIR)\getvstats.obj"
	-@erase "$(INTDIR)\getvstats.sbr"
	-@erase "$(INTDIR)\getzrstats.obj"
	-@erase "$(INTDIR)\getzrstats.sbr"
	-@erase "$(INTDIR)\huffman.obj"
	-@erase "$(INTDIR)\huffman.sbr"
	-@erase "$(INTDIR)\hvqmfpyralloc.obj"
	-@erase "$(INTDIR)\hvqmfpyralloc.sbr"
	-@erase "$(INTDIR)\hvqmfredexp.obj"
	-@erase "$(INTDIR)\hvqmfredexp.sbr"
	-@erase "$(INTDIR)\img_util.obj"
	-@erase "$(INTDIR)\img_util.sbr"
	-@erase "$(INTDIR)\imgalloc.obj"
	-@erase "$(INTDIR)\imgalloc.sbr"
	-@erase "$(INTDIR)\jacobi.obj"
	-@erase "$(INTDIR)\jacobi.sbr"
	-@erase "$(INTDIR)\kernel.obj"
	-@erase "$(INTDIR)\kernel.sbr"
	-@erase "$(INTDIR)\laputq.obj"
	-@erase "$(INTDIR)\laputq.sbr"
	-@erase "$(INTDIR)\larralloc.obj"
	-@erase "$(INTDIR)\larralloc.sbr"
	-@erase "$(INTDIR)\limgalloc.obj"
	-@erase "$(INTDIR)\limgalloc.sbr"
	-@erase "$(INTDIR)\limgio.obj"
	-@erase "$(INTDIR)\limgio.sbr"
	-@erase "$(INTDIR)\lkflow.obj"
	-@erase "$(INTDIR)\lkflow.sbr"
	-@erase "$(INTDIR)\lsqsub.obj"
	-@erase "$(INTDIR)\lsqsub.sbr"
	-@erase "$(INTDIR)\lsqsub_fwd.obj"
	-@erase "$(INTDIR)\lsqsub_fwd.sbr"
	-@erase "$(INTDIR)\ltob.obj"
	-@erase "$(INTDIR)\ltob.sbr"
	-@erase "$(INTDIR)\ltof.obj"
	-@erase "$(INTDIR)\ltof.sbr"
	-@erase "$(INTDIR)\ltos.obj"
	-@erase "$(INTDIR)\ltos.sbr"
	-@erase "$(INTDIR)\lubksb.obj"
	-@erase "$(INTDIR)\lubksb.sbr"
	-@erase "$(INTDIR)\ludcmp.obj"
	-@erase "$(INTDIR)\ludcmp.sbr"
	-@erase "$(INTDIR)\mathtls.obj"
	-@erase "$(INTDIR)\mathtls.sbr"
	-@erase "$(INTDIR)\mmath.obj"
	-@erase "$(INTDIR)\mmath.sbr"
	-@erase "$(INTDIR)\nzrhist.obj"
	-@erase "$(INTDIR)\nzrhist.sbr"
	-@erase "$(INTDIR)\profile.obj"
	-@erase "$(INTDIR)\profile.sbr"
	-@erase "$(INTDIR)\putblock.obj"
	-@erase "$(INTDIR)\putblock.sbr"
	-@erase "$(INTDIR)\putvector.obj"
	-@erase "$(INTDIR)\putvector.sbr"
	-@erase "$(INTDIR)\qmfpyralloc.obj"
	-@erase "$(INTDIR)\qmfpyralloc.sbr"
	-@erase "$(INTDIR)\qmfredexp.obj"
	-@erase "$(INTDIR)\qmfredexp.sbr"
	-@erase "$(INTDIR)\qmfsub.obj"
	-@erase "$(INTDIR)\qmfsub.sbr"
	-@erase "$(INTDIR)\ratesub.obj"
	-@erase "$(INTDIR)\ratesub.sbr"
	-@erase "$(INTDIR)\redexp.obj"
	-@erase "$(INTDIR)\redexp.sbr"
	-@erase "$(INTDIR)\redexp_fwd.obj"
	-@erase "$(INTDIR)\redexp_fwd.sbr"
	-@erase "$(INTDIR)\redirected.obj"
	-@erase "$(INTDIR)\redirected.sbr"
	-@erase "$(INTDIR)\reflect.obj"
	-@erase "$(INTDIR)\reflect.sbr"
	-@erase "$(INTDIR)\roi.obj"
	-@erase "$(INTDIR)\roi.sbr"
	-@erase "$(INTDIR)\sarralloc.obj"
	-@erase "$(INTDIR)\sarralloc.sbr"
	-@erase "$(INTDIR)\seqio.obj"
	-@erase "$(INTDIR)\seqio.sbr"
	-@erase "$(INTDIR)\sim_est.obj"
	-@erase "$(INTDIR)\sim_est.sbr"
	-@erase "$(INTDIR)\simgalloc.obj"
	-@erase "$(INTDIR)\simgalloc.sbr"
	-@erase "$(INTDIR)\simgimg.obj"
	-@erase "$(INTDIR)\simgimg.sbr"
	-@erase "$(INTDIR)\simgio.obj"
	-@erase "$(INTDIR)\simgio.sbr"
	-@erase "$(INTDIR)\snzrhist.obj"
	-@erase "$(INTDIR)\snzrhist.sbr"
	-@erase "$(INTDIR)\snzrstats.obj"
	-@erase "$(INTDIR)\snzrstats.sbr"
	-@erase "$(INTDIR)\spyralloc.obj"
	-@erase "$(INTDIR)\spyralloc.sbr"
	-@erase "$(INTDIR)\sqmfpyralloc.obj"
	-@erase "$(INTDIR)\sqmfpyralloc.sbr"
	-@erase "$(INTDIR)\squeezer.obj"
	-@erase "$(INTDIR)\squeezer.sbr"
	-@erase "$(INTDIR)\sstats.obj"
	-@erase "$(INTDIR)\sstats.sbr"
	-@erase "$(INTDIR)\stob.obj"
	-@erase "$(INTDIR)\stob.sbr"
	-@erase "$(INTDIR)\stof.obj"
	-@erase "$(INTDIR)\stof.sbr"
	-@erase "$(INTDIR)\stol.obj"
	-@erase "$(INTDIR)\stol.sbr"
	-@erase "$(INTDIR)\svdlib.obj"
	-@erase "$(INTDIR)\svdlib.sbr"
	-@erase "$(INTDIR)\svhist.obj"
	-@erase "$(INTDIR)\svhist.sbr"
	-@erase "$(INTDIR)\svstats.obj"
	-@erase "$(INTDIR)\svstats.sbr"
	-@erase "$(INTDIR)\szrhist.obj"
	-@erase "$(INTDIR)\szrhist.sbr"
	-@erase "$(INTDIR)\szrstats.obj"
	-@erase "$(INTDIR)\szrstats.sbr"
	-@erase "$(INTDIR)\trans_est.obj"
	-@erase "$(INTDIR)\trans_est.sbr"
	-@erase "$(INTDIR)\triangulate.obj"
	-@erase "$(INTDIR)\triangulate.sbr"
	-@erase "$(INTDIR)\ucoders.obj"
	-@erase "$(INTDIR)\ucoders.sbr"
	-@erase "$(INTDIR)\uqnt.obj"
	-@erase "$(INTDIR)\uqnt.sbr"
	-@erase "$(INTDIR)\utcoders.obj"
	-@erase "$(INTDIR)\utcoders.sbr"
	-@erase "$(INTDIR)\utqnt.obj"
	-@erase "$(INTDIR)\utqnt.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\vhist.obj"
	-@erase "$(INTDIR)\vhist.sbr"
	-@erase "$(INTDIR)\vqio.obj"
	-@erase "$(INTDIR)\vqio.sbr"
	-@erase "$(INTDIR)\warpers.obj"
	-@erase "$(INTDIR)\warpers.sbr"
	-@erase "$(INTDIR)\warps.obj"
	-@erase "$(INTDIR)\warps.sbr"
	-@erase "$(INTDIR)\zrhist.obj"
	-@erase "$(INTDIR)\zrhist.sbr"
	-@erase "$(OUTDIR)\SARN.bsc"
	-@erase "$(OUTDIR)\SARN.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /I "..\h" /I "..\h\sarnoff" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\SARN.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SARN.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\0decode.sbr" \
	"$(INTDIR)\0encode.sbr" \
	"$(INTDIR)\affine_est.sbr" \
	"$(INTDIR)\arralloc.sbr" \
	"$(INTDIR)\barralloc.sbr" \
	"$(INTDIR)\bimgalloc.sbr" \
	"$(INTDIR)\bimgio.sbr" \
	"$(INTDIR)\blkmot.sbr" \
	"$(INTDIR)\bnzrhist.sbr" \
	"$(INTDIR)\bnzrstats.sbr" \
	"$(INTDIR)\bpyralloc.sbr" \
	"$(INTDIR)\bqmfpyralloc.sbr" \
	"$(INTDIR)\bredexp.sbr" \
	"$(INTDIR)\breflect.sbr" \
	"$(INTDIR)\bset.sbr" \
	"$(INTDIR)\bstats.sbr" \
	"$(INTDIR)\btoflib.sbr" \
	"$(INTDIR)\btol.sbr" \
	"$(INTDIR)\btos.sbr" \
	"$(INTDIR)\bvhist.sbr" \
	"$(INTDIR)\bvstats.sbr" \
	"$(INTDIR)\bzrstats.sbr" \
	"$(INTDIR)\darralloc.sbr" \
	"$(INTDIR)\dblredexp.sbr" \
	"$(INTDIR)\dctimg.sbr" \
	"$(INTDIR)\die.sbr" \
	"$(INTDIR)\dimgalloc.sbr" \
	"$(INTDIR)\eigens2x2.sbr" \
	"$(INTDIR)\eigsrt.sbr" \
	"$(INTDIR)\farralloc.sbr" \
	"$(INTDIR)\fclip.sbr" \
	"$(INTDIR)\fcopy.sbr" \
	"$(INTDIR)\fdcts.sbr" \
	"$(INTDIR)\fdctsub.sbr" \
	"$(INTDIR)\ffts.sbr" \
	"$(INTDIR)\fileutils.sbr" \
	"$(INTDIR)\filtio.sbr" \
	"$(INTDIR)\filtsub.sbr" \
	"$(INTDIR)\fimgalloc.sbr" \
	"$(INTDIR)\fimgimg.sbr" \
	"$(INTDIR)\fimgio.sbr" \
	"$(INTDIR)\fimgop.sbr" \
	"$(INTDIR)\fminmax.sbr" \
	"$(INTDIR)\fmse.sbr" \
	"$(INTDIR)\fmsv.sbr" \
	"$(INTDIR)\fnzrhist.sbr" \
	"$(INTDIR)\fnzrstats.sbr" \
	"$(INTDIR)\fpyralloc.sbr" \
	"$(INTDIR)\fredexp.sbr" \
	"$(INTDIR)\fsdlap.sbr" \
	"$(INTDIR)\fset.sbr" \
	"$(INTDIR)\fstats.sbr" \
	"$(INTDIR)\ftoblib.sbr" \
	"$(INTDIR)\ftol.sbr" \
	"$(INTDIR)\ftos.sbr" \
	"$(INTDIR)\fvhist.sbr" \
	"$(INTDIR)\fvstats.sbr" \
	"$(INTDIR)\fzrstats.sbr" \
	"$(INTDIR)\getblock.sbr" \
	"$(INTDIR)\getnzrstats.sbr" \
	"$(INTDIR)\getvector.sbr" \
	"$(INTDIR)\getvstats.sbr" \
	"$(INTDIR)\getzrstats.sbr" \
	"$(INTDIR)\huffman.sbr" \
	"$(INTDIR)\hvqmfpyralloc.sbr" \
	"$(INTDIR)\hvqmfredexp.sbr" \
	"$(INTDIR)\img_util.sbr" \
	"$(INTDIR)\imgalloc.sbr" \
	"$(INTDIR)\jacobi.sbr" \
	"$(INTDIR)\kernel.sbr" \
	"$(INTDIR)\laputq.sbr" \
	"$(INTDIR)\larralloc.sbr" \
	"$(INTDIR)\limgalloc.sbr" \
	"$(INTDIR)\limgio.sbr" \
	"$(INTDIR)\lkflow.sbr" \
	"$(INTDIR)\lsqsub.sbr" \
	"$(INTDIR)\lsqsub_fwd.sbr" \
	"$(INTDIR)\ltob.sbr" \
	"$(INTDIR)\ltof.sbr" \
	"$(INTDIR)\ltos.sbr" \
	"$(INTDIR)\lubksb.sbr" \
	"$(INTDIR)\ludcmp.sbr" \
	"$(INTDIR)\mathtls.sbr" \
	"$(INTDIR)\mmath.sbr" \
	"$(INTDIR)\nzrhist.sbr" \
	"$(INTDIR)\profile.sbr" \
	"$(INTDIR)\putblock.sbr" \
	"$(INTDIR)\putvector.sbr" \
	"$(INTDIR)\qmfpyralloc.sbr" \
	"$(INTDIR)\qmfredexp.sbr" \
	"$(INTDIR)\qmfsub.sbr" \
	"$(INTDIR)\ratesub.sbr" \
	"$(INTDIR)\redexp.sbr" \
	"$(INTDIR)\redexp_fwd.sbr" \
	"$(INTDIR)\redirected.sbr" \
	"$(INTDIR)\reflect.sbr" \
	"$(INTDIR)\roi.sbr" \
	"$(INTDIR)\sarralloc.sbr" \
	"$(INTDIR)\seqio.sbr" \
	"$(INTDIR)\sim_est.sbr" \
	"$(INTDIR)\simgalloc.sbr" \
	"$(INTDIR)\simgimg.sbr" \
	"$(INTDIR)\simgio.sbr" \
	"$(INTDIR)\snzrhist.sbr" \
	"$(INTDIR)\snzrstats.sbr" \
	"$(INTDIR)\spyralloc.sbr" \
	"$(INTDIR)\sqmfpyralloc.sbr" \
	"$(INTDIR)\squeezer.sbr" \
	"$(INTDIR)\sstats.sbr" \
	"$(INTDIR)\stob.sbr" \
	"$(INTDIR)\stof.sbr" \
	"$(INTDIR)\stol.sbr" \
	"$(INTDIR)\svdlib.sbr" \
	"$(INTDIR)\svhist.sbr" \
	"$(INTDIR)\svstats.sbr" \
	"$(INTDIR)\szrhist.sbr" \
	"$(INTDIR)\szrstats.sbr" \
	"$(INTDIR)\trans_est.sbr" \
	"$(INTDIR)\triangulate.sbr" \
	"$(INTDIR)\ucoders.sbr" \
	"$(INTDIR)\uqnt.sbr" \
	"$(INTDIR)\utcoders.sbr" \
	"$(INTDIR)\utqnt.sbr" \
	"$(INTDIR)\vhist.sbr" \
	"$(INTDIR)\vqio.sbr" \
	"$(INTDIR)\warpers.sbr" \
	"$(INTDIR)\warps.sbr" \
	"$(INTDIR)\zrhist.sbr"

"$(OUTDIR)\SARN.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\SARN.lib" 
LIB32_OBJS= \
	"$(INTDIR)\0decode.obj" \
	"$(INTDIR)\0encode.obj" \
	"$(INTDIR)\affine_est.obj" \
	"$(INTDIR)\arralloc.obj" \
	"$(INTDIR)\barralloc.obj" \
	"$(INTDIR)\bimgalloc.obj" \
	"$(INTDIR)\bimgio.obj" \
	"$(INTDIR)\blkmot.obj" \
	"$(INTDIR)\bnzrhist.obj" \
	"$(INTDIR)\bnzrstats.obj" \
	"$(INTDIR)\bpyralloc.obj" \
	"$(INTDIR)\bqmfpyralloc.obj" \
	"$(INTDIR)\bredexp.obj" \
	"$(INTDIR)\breflect.obj" \
	"$(INTDIR)\bset.obj" \
	"$(INTDIR)\bstats.obj" \
	"$(INTDIR)\btoflib.obj" \
	"$(INTDIR)\btol.obj" \
	"$(INTDIR)\btos.obj" \
	"$(INTDIR)\bvhist.obj" \
	"$(INTDIR)\bvstats.obj" \
	"$(INTDIR)\bzrstats.obj" \
	"$(INTDIR)\darralloc.obj" \
	"$(INTDIR)\dblredexp.obj" \
	"$(INTDIR)\dctimg.obj" \
	"$(INTDIR)\die.obj" \
	"$(INTDIR)\dimgalloc.obj" \
	"$(INTDIR)\eigens2x2.obj" \
	"$(INTDIR)\eigsrt.obj" \
	"$(INTDIR)\farralloc.obj" \
	"$(INTDIR)\fclip.obj" \
	"$(INTDIR)\fcopy.obj" \
	"$(INTDIR)\fdcts.obj" \
	"$(INTDIR)\fdctsub.obj" \
	"$(INTDIR)\ffts.obj" \
	"$(INTDIR)\fileutils.obj" \
	"$(INTDIR)\filtio.obj" \
	"$(INTDIR)\filtsub.obj" \
	"$(INTDIR)\fimgalloc.obj" \
	"$(INTDIR)\fimgimg.obj" \
	"$(INTDIR)\fimgio.obj" \
	"$(INTDIR)\fimgop.obj" \
	"$(INTDIR)\fminmax.obj" \
	"$(INTDIR)\fmse.obj" \
	"$(INTDIR)\fmsv.obj" \
	"$(INTDIR)\fnzrhist.obj" \
	"$(INTDIR)\fnzrstats.obj" \
	"$(INTDIR)\fpyralloc.obj" \
	"$(INTDIR)\fredexp.obj" \
	"$(INTDIR)\fsdlap.obj" \
	"$(INTDIR)\fset.obj" \
	"$(INTDIR)\fstats.obj" \
	"$(INTDIR)\ftoblib.obj" \
	"$(INTDIR)\ftol.obj" \
	"$(INTDIR)\ftos.obj" \
	"$(INTDIR)\fvhist.obj" \
	"$(INTDIR)\fvstats.obj" \
	"$(INTDIR)\fzrstats.obj" \
	"$(INTDIR)\getblock.obj" \
	"$(INTDIR)\getnzrstats.obj" \
	"$(INTDIR)\getvector.obj" \
	"$(INTDIR)\getvstats.obj" \
	"$(INTDIR)\getzrstats.obj" \
	"$(INTDIR)\huffman.obj" \
	"$(INTDIR)\hvqmfpyralloc.obj" \
	"$(INTDIR)\hvqmfredexp.obj" \
	"$(INTDIR)\img_util.obj" \
	"$(INTDIR)\imgalloc.obj" \
	"$(INTDIR)\jacobi.obj" \
	"$(INTDIR)\kernel.obj" \
	"$(INTDIR)\laputq.obj" \
	"$(INTDIR)\larralloc.obj" \
	"$(INTDIR)\limgalloc.obj" \
	"$(INTDIR)\limgio.obj" \
	"$(INTDIR)\lkflow.obj" \
	"$(INTDIR)\lsqsub.obj" \
	"$(INTDIR)\lsqsub_fwd.obj" \
	"$(INTDIR)\ltob.obj" \
	"$(INTDIR)\ltof.obj" \
	"$(INTDIR)\ltos.obj" \
	"$(INTDIR)\lubksb.obj" \
	"$(INTDIR)\ludcmp.obj" \
	"$(INTDIR)\mathtls.obj" \
	"$(INTDIR)\mmath.obj" \
	"$(INTDIR)\nzrhist.obj" \
	"$(INTDIR)\profile.obj" \
	"$(INTDIR)\putblock.obj" \
	"$(INTDIR)\putvector.obj" \
	"$(INTDIR)\qmfpyralloc.obj" \
	"$(INTDIR)\qmfredexp.obj" \
	"$(INTDIR)\qmfsub.obj" \
	"$(INTDIR)\ratesub.obj" \
	"$(INTDIR)\redexp.obj" \
	"$(INTDIR)\redexp_fwd.obj" \
	"$(INTDIR)\redirected.obj" \
	"$(INTDIR)\reflect.obj" \
	"$(INTDIR)\roi.obj" \
	"$(INTDIR)\sarralloc.obj" \
	"$(INTDIR)\seqio.obj" \
	"$(INTDIR)\sim_est.obj" \
	"$(INTDIR)\simgalloc.obj" \
	"$(INTDIR)\simgimg.obj" \
	"$(INTDIR)\simgio.obj" \
	"$(INTDIR)\snzrhist.obj" \
	"$(INTDIR)\snzrstats.obj" \
	"$(INTDIR)\spyralloc.obj" \
	"$(INTDIR)\sqmfpyralloc.obj" \
	"$(INTDIR)\squeezer.obj" \
	"$(INTDIR)\sstats.obj" \
	"$(INTDIR)\stob.obj" \
	"$(INTDIR)\stof.obj" \
	"$(INTDIR)\stol.obj" \
	"$(INTDIR)\svdlib.obj" \
	"$(INTDIR)\svhist.obj" \
	"$(INTDIR)\svstats.obj" \
	"$(INTDIR)\szrhist.obj" \
	"$(INTDIR)\szrstats.obj" \
	"$(INTDIR)\trans_est.obj" \
	"$(INTDIR)\triangulate.obj" \
	"$(INTDIR)\ucoders.obj" \
	"$(INTDIR)\uqnt.obj" \
	"$(INTDIR)\utcoders.obj" \
	"$(INTDIR)\utqnt.obj" \
	"$(INTDIR)\vhist.obj" \
	"$(INTDIR)\vqio.obj" \
	"$(INTDIR)\warpers.obj" \
	"$(INTDIR)\warps.obj" \
	"$(INTDIR)\zrhist.obj"

"$(OUTDIR)\SARN.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
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
!IF EXISTS("SARN.dep")
!INCLUDE "SARN.dep"
!ELSE 
!MESSAGE Warning: cannot find "SARN.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "SARN - Win32 Release" || "$(CFG)" == "SARN - Win32 Debug"
SOURCE=.\0decode.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\0decode.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\0decode.obj"	"$(INTDIR)\0decode.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\0encode.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\0encode.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\0encode.obj"	"$(INTDIR)\0encode.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\affine_est.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\affine_est.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\affine_est.obj"	"$(INTDIR)\affine_est.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\arralloc.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\arralloc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\arralloc.obj"	"$(INTDIR)\arralloc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\barralloc.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\barralloc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\barralloc.obj"	"$(INTDIR)\barralloc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\bimgalloc.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\bimgalloc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\bimgalloc.obj"	"$(INTDIR)\bimgalloc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\bimgio.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\bimgio.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\bimgio.obj"	"$(INTDIR)\bimgio.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\blkmot.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\blkmot.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\blkmot.obj"	"$(INTDIR)\blkmot.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\bnzrhist.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\bnzrhist.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\bnzrhist.obj"	"$(INTDIR)\bnzrhist.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\bnzrstats.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\bnzrstats.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\bnzrstats.obj"	"$(INTDIR)\bnzrstats.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\bpyralloc.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\bpyralloc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\bpyralloc.obj"	"$(INTDIR)\bpyralloc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\bqmfpyralloc.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\bqmfpyralloc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\bqmfpyralloc.obj"	"$(INTDIR)\bqmfpyralloc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\bredexp.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\bredexp.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\bredexp.obj"	"$(INTDIR)\bredexp.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\breflect.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\breflect.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\breflect.obj"	"$(INTDIR)\breflect.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\bset.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\bset.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\bset.obj"	"$(INTDIR)\bset.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\bstats.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\bstats.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\bstats.obj"	"$(INTDIR)\bstats.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\btoflib.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\btoflib.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\btoflib.obj"	"$(INTDIR)\btoflib.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\btol.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\btol.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\btol.obj"	"$(INTDIR)\btol.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\btos.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\btos.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\btos.obj"	"$(INTDIR)\btos.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\bvhist.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\bvhist.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\bvhist.obj"	"$(INTDIR)\bvhist.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\bvstats.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\bvstats.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\bvstats.obj"	"$(INTDIR)\bvstats.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\bzrstats.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\bzrstats.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\bzrstats.obj"	"$(INTDIR)\bzrstats.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\darralloc.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\darralloc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\darralloc.obj"	"$(INTDIR)\darralloc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\dblredexp.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\dblredexp.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\dblredexp.obj"	"$(INTDIR)\dblredexp.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\dctimg.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\dctimg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\dctimg.obj"	"$(INTDIR)\dctimg.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\die.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\die.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\die.obj"	"$(INTDIR)\die.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\dimgalloc.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\dimgalloc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\dimgalloc.obj"	"$(INTDIR)\dimgalloc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\eigens2x2.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\eigens2x2.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\eigens2x2.obj"	"$(INTDIR)\eigens2x2.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\eigsrt.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\eigsrt.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\eigsrt.obj"	"$(INTDIR)\eigsrt.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\farralloc.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\farralloc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\farralloc.obj"	"$(INTDIR)\farralloc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\fclip.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\fclip.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\fclip.obj"	"$(INTDIR)\fclip.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\fcopy.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\fcopy.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\fcopy.obj"	"$(INTDIR)\fcopy.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\fdcts.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\fdcts.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\fdcts.obj"	"$(INTDIR)\fdcts.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\fdctsub.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\fdctsub.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\fdctsub.obj"	"$(INTDIR)\fdctsub.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ffts.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\ffts.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\ffts.obj"	"$(INTDIR)\ffts.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\fileutils.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\fileutils.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\fileutils.obj"	"$(INTDIR)\fileutils.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\filtio.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\filtio.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\filtio.obj"	"$(INTDIR)\filtio.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\filtsub.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\filtsub.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\filtsub.obj"	"$(INTDIR)\filtsub.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\fimgalloc.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\fimgalloc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\fimgalloc.obj"	"$(INTDIR)\fimgalloc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\fimgimg.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\fimgimg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\fimgimg.obj"	"$(INTDIR)\fimgimg.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\fimgio.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\fimgio.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\fimgio.obj"	"$(INTDIR)\fimgio.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\fimgop.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\fimgop.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\fimgop.obj"	"$(INTDIR)\fimgop.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\fminmax.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\fminmax.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\fminmax.obj"	"$(INTDIR)\fminmax.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\fmse.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\fmse.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\fmse.obj"	"$(INTDIR)\fmse.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\fmsv.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\fmsv.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\fmsv.obj"	"$(INTDIR)\fmsv.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\fnzrhist.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\fnzrhist.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\fnzrhist.obj"	"$(INTDIR)\fnzrhist.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\fnzrstats.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\fnzrstats.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\fnzrstats.obj"	"$(INTDIR)\fnzrstats.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\fpyralloc.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\fpyralloc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\fpyralloc.obj"	"$(INTDIR)\fpyralloc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\fredexp.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\fredexp.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\fredexp.obj"	"$(INTDIR)\fredexp.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\fsdlap.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\fsdlap.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\fsdlap.obj"	"$(INTDIR)\fsdlap.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\fset.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\fset.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\fset.obj"	"$(INTDIR)\fset.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\fstats.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\fstats.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\fstats.obj"	"$(INTDIR)\fstats.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ftoblib.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\ftoblib.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\ftoblib.obj"	"$(INTDIR)\ftoblib.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ftol.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\ftol.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\ftol.obj"	"$(INTDIR)\ftol.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ftos.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\ftos.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\ftos.obj"	"$(INTDIR)\ftos.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\fvhist.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\fvhist.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\fvhist.obj"	"$(INTDIR)\fvhist.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\fvstats.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\fvstats.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\fvstats.obj"	"$(INTDIR)\fvstats.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\fzrstats.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\fzrstats.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\fzrstats.obj"	"$(INTDIR)\fzrstats.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\getblock.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\getblock.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\getblock.obj"	"$(INTDIR)\getblock.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\getnzrstats.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\getnzrstats.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\getnzrstats.obj"	"$(INTDIR)\getnzrstats.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\getvector.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\getvector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\getvector.obj"	"$(INTDIR)\getvector.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\getvstats.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\getvstats.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\getvstats.obj"	"$(INTDIR)\getvstats.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\getzrstats.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\getzrstats.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\getzrstats.obj"	"$(INTDIR)\getzrstats.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\huffman.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\huffman.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\huffman.obj"	"$(INTDIR)\huffman.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\hvqmfpyralloc.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\hvqmfpyralloc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\hvqmfpyralloc.obj"	"$(INTDIR)\hvqmfpyralloc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\hvqmfredexp.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\hvqmfredexp.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\hvqmfredexp.obj"	"$(INTDIR)\hvqmfredexp.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\img_util.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\img_util.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\img_util.obj"	"$(INTDIR)\img_util.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\imgalloc.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\imgalloc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\imgalloc.obj"	"$(INTDIR)\imgalloc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jacobi.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\jacobi.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\jacobi.obj"	"$(INTDIR)\jacobi.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\kernel.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\kernel.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\kernel.obj"	"$(INTDIR)\kernel.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\laputq.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\laputq.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\laputq.obj"	"$(INTDIR)\laputq.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\larralloc.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\larralloc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\larralloc.obj"	"$(INTDIR)\larralloc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\limgalloc.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\limgalloc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\limgalloc.obj"	"$(INTDIR)\limgalloc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\limgio.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\limgio.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\limgio.obj"	"$(INTDIR)\limgio.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\lkflow.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\lkflow.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\lkflow.obj"	"$(INTDIR)\lkflow.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\lsqsub.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\lsqsub.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\lsqsub.obj"	"$(INTDIR)\lsqsub.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\lsqsub_fwd.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\lsqsub_fwd.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\lsqsub_fwd.obj"	"$(INTDIR)\lsqsub_fwd.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ltob.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\ltob.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\ltob.obj"	"$(INTDIR)\ltob.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ltof.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\ltof.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\ltof.obj"	"$(INTDIR)\ltof.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ltos.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\ltos.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\ltos.obj"	"$(INTDIR)\ltos.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\lubksb.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\lubksb.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\lubksb.obj"	"$(INTDIR)\lubksb.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ludcmp.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\ludcmp.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\ludcmp.obj"	"$(INTDIR)\ludcmp.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mathtls.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\mathtls.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\mathtls.obj"	"$(INTDIR)\mathtls.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mmath.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\mmath.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\mmath.obj"	"$(INTDIR)\mmath.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\nzrhist.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\nzrhist.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\nzrhist.obj"	"$(INTDIR)\nzrhist.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\profile.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\profile.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\profile.obj"	"$(INTDIR)\profile.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\putblock.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\putblock.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\putblock.obj"	"$(INTDIR)\putblock.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\putvector.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\putvector.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\putvector.obj"	"$(INTDIR)\putvector.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\qmfpyralloc.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\qmfpyralloc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\qmfpyralloc.obj"	"$(INTDIR)\qmfpyralloc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\qmfredexp.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\qmfredexp.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\qmfredexp.obj"	"$(INTDIR)\qmfredexp.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\qmfsub.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\qmfsub.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\qmfsub.obj"	"$(INTDIR)\qmfsub.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ratesub.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\ratesub.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\ratesub.obj"	"$(INTDIR)\ratesub.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\redexp.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\redexp.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\redexp.obj"	"$(INTDIR)\redexp.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\redexp_fwd.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\redexp_fwd.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\redexp_fwd.obj"	"$(INTDIR)\redexp_fwd.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\redirected.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\redirected.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\redirected.obj"	"$(INTDIR)\redirected.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\reflect.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\reflect.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\reflect.obj"	"$(INTDIR)\reflect.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\roi.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\roi.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\roi.obj"	"$(INTDIR)\roi.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\sarralloc.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\sarralloc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\sarralloc.obj"	"$(INTDIR)\sarralloc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\seqio.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\seqio.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\seqio.obj"	"$(INTDIR)\seqio.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\sim_est.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\sim_est.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\sim_est.obj"	"$(INTDIR)\sim_est.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\simgalloc.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\simgalloc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\simgalloc.obj"	"$(INTDIR)\simgalloc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\simgimg.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\simgimg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\simgimg.obj"	"$(INTDIR)\simgimg.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\simgio.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\simgio.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\simgio.obj"	"$(INTDIR)\simgio.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\snzrhist.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\snzrhist.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\snzrhist.obj"	"$(INTDIR)\snzrhist.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\snzrstats.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\snzrstats.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\snzrstats.obj"	"$(INTDIR)\snzrstats.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\spyralloc.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\spyralloc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\spyralloc.obj"	"$(INTDIR)\spyralloc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\sqmfpyralloc.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\sqmfpyralloc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\sqmfpyralloc.obj"	"$(INTDIR)\sqmfpyralloc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\squeezer.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\squeezer.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\squeezer.obj"	"$(INTDIR)\squeezer.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\sstats.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\sstats.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\sstats.obj"	"$(INTDIR)\sstats.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\stob.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\stob.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\stob.obj"	"$(INTDIR)\stob.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\stof.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\stof.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\stof.obj"	"$(INTDIR)\stof.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\stol.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\stol.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\stol.obj"	"$(INTDIR)\stol.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\svdlib.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\svdlib.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\svdlib.obj"	"$(INTDIR)\svdlib.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\svhist.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\svhist.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\svhist.obj"	"$(INTDIR)\svhist.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\svstats.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\svstats.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\svstats.obj"	"$(INTDIR)\svstats.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\szrhist.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\szrhist.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\szrhist.obj"	"$(INTDIR)\szrhist.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\szrstats.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\szrstats.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\szrstats.obj"	"$(INTDIR)\szrstats.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\trans_est.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\trans_est.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\trans_est.obj"	"$(INTDIR)\trans_est.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\triangulate.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\triangulate.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\triangulate.obj"	"$(INTDIR)\triangulate.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ucoders.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\ucoders.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\ucoders.obj"	"$(INTDIR)\ucoders.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\uqnt.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\uqnt.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\uqnt.obj"	"$(INTDIR)\uqnt.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\utcoders.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\utcoders.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\utcoders.obj"	"$(INTDIR)\utcoders.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\utqnt.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\utqnt.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\utqnt.obj"	"$(INTDIR)\utqnt.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\vhist.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\vhist.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\vhist.obj"	"$(INTDIR)\vhist.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\vqio.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\vqio.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\vqio.obj"	"$(INTDIR)\vqio.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\warpers.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\warpers.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\warpers.obj"	"$(INTDIR)\warpers.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\warps.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\warps.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\warps.obj"	"$(INTDIR)\warps.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\zrhist.c

!IF  "$(CFG)" == "SARN - Win32 Release"


"$(INTDIR)\zrhist.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SARN - Win32 Debug"


"$(INTDIR)\zrhist.obj"	"$(INTDIR)\zrhist.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

