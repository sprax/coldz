#if 0	/******************** 00gg.c *****************************************/

Mon Oct 16 16:19:54 EDT 1995

o	fix inn for 8 & 12 examples
o	make mpegs
o	write proposal for thesis
o 	composite image layers
o	walking person
o	'camera' zooming


________done:
o	4-image f+b
o	variable # of pyramid levels
o 	Gaussian instead of Laplacian Pyramids
o	try grid between 0 & 2 in inn4.
o	hack extraction: just crop around center point 
o	option: showGrid shows grid at interpolated points!

COMPOSED FLOW, LARGE ROTATION WARPING CHARACTERISTICS:
Forward:  A:aliasing, ear-compression   
	  B-pixels: aliasing, ear-fade
Backward: A:stretching (middle 1/3), ear-compression (end)
	  B-pixels: face recession all the way to A.

OUTLINE of PROPOSAL
  The Problem: Given N images, choose M, etc.
  Various solutions, and what could be done using them.
  The Difficulties (for various solutions)
    How others have tried to overcome the difficulties.
      Classify, outline strengths & limitations.
    General form of a solution.
    My attempt.

o	extraction, once again, and this time save it.
o   manual correctons with 1-way texture mapping (no blending)
o   inverse warping + manual corrections
o	backward and forward warping
o	forward from both ways warping
7.5	inverse warping
o   



o   save the grid!
o   immediate update in special (frame-storing) window?  YES!
o   Enforce convexity?
o   Reverse the arrows for forward final warping?
o   implement orthopoly warping from Wolberg?
o   implement/borrow Beier & Neely?
o   Prevent initial foldover from flow, setting to minSep
o   Break ties for closest grid point, where "tie" means less than
	minSep
o   INVERSE vs. COMPOSITION (is the tet-warp itself or its inverse
	a component of the final corres/warping field?)

o   ICCV DEMO:
	PARTITION, 6 -> 9
 	BACKUPS
	EXTRACTION ?  (including save extracted -- YES?)
        SAVE EDITED FIELDS
o   REALLY COMBINE O-FLOW WITH MANUAL CORRES: RELAXATION/LEAST SQUARES, ETC.
	register labeled pts w/ bilinear stretching warps, not affine
            registration
o   save extracted faces? (optional, if not using inpU...)
o   label file functions
o   Seam-Stitching/Smoothing:
	1.  blurring
	2.  splining
o   N = 3, 5, 6, 9
o   some version of TP/MJones triangulation
o   flow within trianges
o   allow grayscale input to inn?
o   rewrite inCrsTet based on inCrsTri
o   program that inputs every img file in a directory & does something...
o   images into directories...
o   Redraw existing triangles/erase them
o   aliases for using GG instead of gg ??

o   sub-pixel sample on triangle texture mapping (read more first)
+   Fix innMapTri & lower-levels (leave for now)
+   Reflow on triangle chain
+   Read flow if already saved.
+   Undo - manage "global" variables, ie edit one pair at a time, etc.
o   CONTROL PANELS, incl. SLIDERS:
	are not Wimj's, so don't inherit saveRGB, etc.
	must have own menus
	may have Wj[] in scope, but preferrably call winLoop
	should be updated together, sharing data & avoiding multiples
	interface to forms library?
+   sliders rejoin winLooper w/ own menu
o   X-invert-offset flow for symmetric pseudo head-turning



o   compute flow & stitch segments of vector field together
    -	with & without border: the aperture problem
o   make the patchwork comprehensive/global
o   store identified regions in a growable, storable structure
o   generalize to N-D.

o   manCorTri was busted by y-inversion, core dumps on jkRead...flags=1


6.04	inn accepts N = 2,4,8
6.03	Sliders with different semantics for N = 2,3,...


ISSUES:
    REPRESENTATION OF WARPING FIELDS:
        Dense vector fields: cost of storage is quadratic in precision
	Splines: storage is linear (?) in order of knots.








Some Sj pairs: A-B, C-E(manfix), 
_______________________________________________________
The case of 3 images A, B, C, with B "central":

1.  Use F:AC to morph A torward C to make D, then find flow F:BD
    and morph between B & D.
2.  Use F:AB to morph A torward B to make D, and at the same time
    warp F:AC into F:DC, which can be used to warp D to C.
2a. Use R:BA to inverse warp A torward B to make D, and at the same time
    warp F:AC into F:DC, which can be used to warp D to C.
3.  Combine F:AB and F:BC quadratically to go parabolically from 
    A to C thru B, and combine u*F:AB & v*F:BC, where 0.0 < u,v < 1.0
    to go from A to C without actually going thru B. [[ EXPECT WRONG ]]
4.  Use F:AB to go from A to B, then F:CB to go in reverse from B to C.
5.  Take F = t*F:BA + (1 - t)*F:BC and pixels base on relative distance
    [Done: doesn't work very well].
6.  Combine F:BA, F:BC, and either F:AC or F:CA or both.
    -------- methods which resample from INPUT sources -------


_______________________________________________________

Multi-dimensional morphing:

Much of it can be understood even from considereing the simplest
case of morphing between 2 images A -> B:

    Transform both images to canonical pose & coord frame: A->a, B->b.
    Use automatic methods to find dense correspondences f: a -> b.
    Use manual methods to assign sparse correspondences F: A -> B.
    For every pixel location p in A, decide whether to use f, F, or
        a combination, and go fetch an appropriate pixel value b' from B, A,
        some combination of A & B, or some database, and put that pixval
        into location p in a new image B'.  B' is a "pre-warped" version of B;
        the inverse transformation from B' to B may also be viewed as a
        virtual correspondence mapping F': A -> B'.  For real images of
        faces, etc., F' is unlikely to be even approximately a homeomorphism.
        NB: The places where there are difficulties in obtaining good pixvals
        for B' are at this point obvious candidates for further automatic
        analysis or hand-picking of corresponding points/regions.
    Now, to synthesize an image S, combine pixvals from same pixlocs in A & B'
        and either send each one to "splatter" onto some location in S
        (forward construction), or, for each pixloc in S, fetch a pixval v
        from the A-B' combination (inverse construction).  This synthesis
        phase may use F', or it may not.  Regarding F as a vector field
        and linearly interpolating along the vectors creates a standard
        morph sequence from A to B, but some other mapping G may be used,
        say, to make smooth transitions between A and B as texture mapped
        onto an arbitrarily shaped surface. 
        
NB: The main advantage of prewarping B to B' is efficiency; it makes the
    construction phase much simpler.  However, image quality is biased 
    toward the A-end of the sequence; quality from A to B will be perfect
    at A (A will be reconstruced exactly), then up to the midpint will
    deteriorate mostly with f not being into (injective), but from the
    midpoint to B will suffer more from f not being onto (surjective);
    the last image will perfectly reconstruct B iff f is onto.

Similar considerations apply to more than 3 or more images.  For instance,
given images A, B, and C, where A is regarded as "central," then a new
image S may be constructed using pixvals from A and the pre-warped images
B' and C' as given by F': A -> B and G': A -> C.  What does "central"
mean?  Formally speaking, A is "central" in the sense that mappings are
known from A to both B and C.  Together, these mappings also give 
correspondences between B' and C' and even B and C, but in general these
correspondences will not be as good, because some parts of the mappings
involved are not strictly invertible.  Informally, or in a natural way of
speaking, saying A is "central" just means that B and C seem more
different from each other than they do from A.  If this is so, then
choosing A as central will tend to give better implied correspondences
between B and C.  If both A and B seem equally central in this natural
sense, then it shouldn't matter which is treated as central from this
formal or computational point of view.  If A, B, and C are completely
different from each other, then any of them may be chosen as central
and synthesis will probably fail anyway.


________STEPS________________:
   COMPOSITION OF partial images
   INTERACTIVE point picking
   FLOW WITHIN Triangles/Quadrilaterals
   COMPOSITION of Flow Fields (dissolving at boundary)

________Priority_Queue_______:
DEMOS FOR TOMMY:			PLUSES			MINUSES
1  Expression independent of pose	Proves sep/1 lbl	1-D (?)
2  Concatenate flow globally in warped	Simple, extends 1	Stuck in dD?
3  Concatenate flow globally in source	See diff		??
   Bilinear triangle warp works:	Proves feasibility	Fine hand tune
4    4 Tri sep nostrils/bulb/stem
5    4 V-Slice sep nostrils/cheeks
6  Concatenate within triangles 	Save space/prove multi	??
7  Interactive vers. of 4		Less tedious/show prob	Imprecise(?)
8  Sep. flow within triangles (nose)	Long time goal done
9  Linear warp to hide teeth		basically works		tedious now
O  Correct InterEye in warped domain?	Shows past mistakes	Not best soln.
      

Some ways to try fixing the nose:
X 1. Simplest bilinear hack
X 2. More triangles
  3. Concatenate flows: C->D->E->F->G into C->G
     A. Globally
X	1. In warped (src) coord system
X       2. In source (inp) coord system [might look ridiculous]
     B. Nose region only: THIS IS DEEP: Where F[C->G] works,
	it should work better than F[C->D->E->F->G]; trade-offs
	mean there is an optical region within which to use the
	composite flow!
  4.
	Separate flow within triangle's bounding box -- this 
		is a control to compare against what follows
	Slightly better affine warp between triangles -- 
		but aren't there arbitrary choices?
	Now run flow between transwarped triangles,
		and synthesize using untranswarped flow

Follow CC's mole to get fwd trf, then relabel E & D?

BUG -- that massive Y offset coming from canonExtractFace(NULL,...)?
	Same problem as the mysterious scale factor?  Prob. not.
	Fix the fwd trf so points for interaction go.  Do this at
	same time as flow on isolated triangles for proof of concept;
	thus, interactive from the start, in ways that are useful.

Sunday (X-MAS):
	* MD, David, Eryk, Lynnette, Sean, Lonnie @ Al's, SMAG @ parents,
	Len?  Tamara?
	
Wed:    9.00 exam, papers, key
Thu:	grade 9.00, work on Rune, draft Progress Report
Fri:	turn in draft to TP; 
Wed: 
	Placement Office:
		Jobs: Viet Nam, Brazil, Japan, Indonesia, China, etc.
		Schools: Iowa, BU, Berkeley; writing programs
		TeraNet
	HMV: Return CD for $$
	Ferranti: pickup camera part, buy film on MIT
	Harvard: AID Project (Indonesia)
	Joiner Center for Viet Nam work/study abroad

Td27:	ROTCH Library: I.D., Art Forum, Art in America, FACE BOOK
	(writing, editing, layout, internet access, world wide web)
Tj3:    Postmark date for applic to Iowa Writer's Workshop: 3 stories,
		3 letters of rec (later).

________Progress_Report_______:
Lucas-Kanade optical flow is best for estimating relative movement
of local features, whereas we want global correspondences.
    PROCEDURE:
	Put all faces in a canonical "pose" for flow-detection --
	    this involves aligning eyes, hoping that the correspondences
	    between other features will be found automatically.
	Compute flow between faces in this "pose."
	Transform this flow into correspondences bewteen the original
	    images using inverse affine (or whatever) transformation.
	But the actual placement of synthesized faces is with the labelled
	    center always in the center.

	Simple transformation affinely 
EXTRACTION:  Just use the self-portraits, change nothing
	RpT: Hetero-inputs are a general problem.  Video is a 
	     practical/partial measure, not a whole solution.
POSE_ESTIM:  Do the algebraTrig of 3 pts; that's all
	RpT: Independence of pose/expression is obviously a 
	     necessary step, but hasn't been done in most (any other?)
	     artifical talking head project.
	RpT: Not clear how much human's compute any 3D model, even though
	     they infer and are consciously aware of 3D info.
	Pro/Con: Demands some labelling, but not necessarily manual, 
	     and you can get a lot back out (deciding which pose of
	     an object is canonical already puts structure/corresp. info there. 
JUSTIFY_3D_USE: If RBF's or other approximation techniques can interpolate
	images in such a was as to simulate 3D transformations, why shouldn't
	they also do the inverse, i.e., infer 3D motion from 2D image
	sequences?  Then we have a loop...   It seems we can safely 
	assume that on some level the brain uses very simple 3D geometry
	"schemes" in relating 2D images to each other.
SIZE_PROBS:  angle and interocular distance
  EYE_VS_CENTER(NOSE): 
HUMAN_GEOMT: Approx as ellipse with spine inserted into one focal pt.
RE-TRF-FLOW: All the way back to the input image 
		* Eliminates 1 or 2 degrading transformations.
		* When examples are far from canonical, the trf to 
		  canonical position may open gaps, holes, etc: these
		  may make for bad flow locally, but at least the
		  content of these glitches won't be transferred.
	RpT: Comparison would be the logical test.	
	RpT: Allows (much?) wider range of input images
MULTI_DIMEN: Make one function for N input/example images
INTERACTIVE: Show an interface
	RpT: Talk about actually useful usage, criticize the usual
		BS if appropriate.
SEMI-AUTOMA: One blend function is enough: This *is* texture mapping.
	RpT: Discuss others in the context of general texture mapping.
RBF_NETWORK: Keep it simple, even if that means switch statements.
	RpT: Of course...
#EXAMPLES>#DIMENSIONS: More than 2 examples in a "line": use splines.
ON-RELATED-RESEARCH: Comment on 2 or 3 papers in book lent by Carey.


morph all the Zappa Heads!

make undo-affine-alignment a clear option
  extract from inputs & then throw them away 
  the always transform flow fields back into correspondences between
    the extracted images -- thus translations must already be done!
    (may use average of flow itself as global translation, then
     subtract it, making average translation 0)


        re-define AffTrf AFTER demos for Tommy!

	put labels in same file as other correspondences
	normalize: W&H included in files...
	Rework the registration/extraction phase
		integrate manual correspondences & labels --
			every corres pair assigned a number AND string
		Use 3 instead of 2 pts to compute affine transf to canon view? 

	integrate the new unFlags.[ch]
	move all arg parsing out of mainG (ggParse?)

	add more colorful GL drawing commands
	unaffine with MC + 2-way
	Fix bug in hole-filler (Apparently rects outside central sqr disabled!)
	Try David's forward warper
	Save from framebuffer (x-capture, snapshot, whatever)
        Go to full triangle with teeth, optional(?) Y-reorient
        Show Tommy teeth demo, try to make work with jj's re-warp
        Two-way jj with MC

        Then go the direct sampling route -- or is this the
                same as previous if done properly?
        put SjrA -> nd2 morph on Web as MPEG

        Make cropper & shell script for multiples
        do simple (SEND instead of GET) form for mc2 AA->BB -
                with & without hand correction

	AFTER_THURS:_______________________________________
        print more .ps & delete
        make form for output of fc4
        install Photoshop & Illustrator
        Here, drink this moth-wing tea.
        redo bottom menus throughout p
        email EIT
        learn PERL
        fix fff
        Find the clues behind the kludges.
        make dream gallery software work
        hack xv2
        mail Ranjit
        read graphics books
        read brain books
        write publishable stories for Karmel & Desai
        collect job, grant, & degree offers
        use Xresource to give Mosaic an icon?
        beymerEyes (like Xeyes)
        write lotto dreams

	Fix hole pluggers?
        keep entire originals as RGBI; just call matchEyeLoc with
		new eyeLoc's to get inv-aff-warpd source (later,
		clean all this up).
	Fwd: go fwd to D, then inv. transf. from D to D', then sample/render
	mv *.ch to old, then retrieve only what's needed...
	File format for flow fields (X & Y in same file)
	Follow coarseFine down, rewriting Sarnoff.
 	Reflection symmetry
	Fix rowPlug (img 1 in `fwd2 b a 10`)
	Warping of Triangular patches (avoid whole rectangle?)
	Triangulation/Tesselation
	A better, normalized biquadratic
	Add ability to read & write (pre)cropped/scaled images.
	Forward Optical Flow corres.
	Fix rowSmat*N
        glc == gls in RGBmode
	Find out if/how much border Sarnoff routines really need.
	Need rowBordUnc and/or rowBordUncRGB
+	Xsliders in mixx -- no examples using Xlib/XCreateWindow proglevel
	movie-buffering in a GLX app. -- probably not posible, but...

________REVERSE CHRONOLOGY________:
7	Put MC-file reader in mainG
6	rowFlowBac -> newFlowBac: input images were duplicated at fXYWH
	regardless of input sizes - turns out that adding real image border
	doesn't necessarily help -- worse for models
5	Reworking the extraction phase: take in much more of orig images
5	Figured out how to set origin & not size in GL resource file.
5	Flow initialization bug fixed (calloc instead of malloc before pyrs)
4	Two way morphing with manual correspondences (mc2 -B)
4	Added functionality to winLoop -- transparent GL triangles
2.Sep	Redid CC-CS.mpg & FlyBiCubic.mpg MPEGS for WWW
31	Auto-orient triangles in list, allowing any order 
29  	CHANNEL ORDER: R,G,B,[Alpha|Gray,[more]] - So always s/th at [0]
28	FlyBiCubic MPEG
28	mpeg_encode & mpeg_play
26	Explored optimization by unrolling, SGI:cc & SUN:gcc
26	How big a border does Sarnoff really need for Flow & Img?  4
25	Showed TP Cindy-Claudia, Tri-Tri working, wholes not filled correctly
24	Tri-Tri reordered per individual triangle	
23.Aug	Trapezoids to triangles on DB pre-ordered
25	Added RGB to jj et al.
25.May	Updated priorities
10	Changing default/optional sizes of warped images.
9	Fixed the subtle bug in b3c8: malloc instead of calloc, + rowComb.
8	grid.c -- display warping fields by warping a graph paper-like image.
7	BUGLIST
6
1.Apr	Trying to fix bug in b3c8
1.Oct	Eliminated GUI's dependency on -ljpeg.
9	(Harvard, sick/headache)
8	Fixed bug (again).
7	Looking for bug (again).
6 S	(Home)	
5	
31	Replaced alloc_image with image_new, removing last ref. to pixrect
31	Replaced libmorf with libnew (again).
31	marked interface between my row-based lib (libim.a) & Sarnoff
  	border conventions (librha.a) in rowFlow.c & moved on.
30	SuperBowl XXVIII.
29	Nothing.
28	rowFlow stuff
27
23	imageGrey2.c morphBase.c, rpBase.c: removed image_t from mainH.
22	
21	Fixed fff to work as "QuadBac3"
20	New OPTIONS: O_COMB, O_BACK, O_CENT, O_FWRD
19	BORDER CONVENTIONS:
DST: dX,dY,dW,dH effectively always pick out a subrect.  Thus,
  if extra space is needed and isn't outside of the rect given by
  dX=0,dY=0, then we need dX>0 and/or dY>0 to avoid a seg-fault.
SRC: There are two cases:
  COPY: Need only sX & sY for start of subrect in Src. 
    Assume that dW <= (sL-sX), where sL is length from Src Org
    to Src End, but sL isn't usually given.  This case is more
    analogous to Dst: sW = sL-sX, and sX is just like dX, etc.
    For an interior subrect, ADD sX>0 to the Ptr to Org.
  PROC: Like to know 4 distances from Orig to Bdry, ie, how far
    we CAN go, not nec. how far we SHOULD go.  Except for purposes
    of (de)alloc & filling, we need to know only a lower bound
    on the border size (eg, for an operator of 5x5 support, we
    need assume all borders >= 2 pixels; this is only 1,2, or 4
    assertions, so doesn't bear on efficiency).  In cases where
    we do have to go to the borders, the clearest, most convenient
    representation still seems to be sX,sY,sW,sH, where sL = sX+sW,
    ie, a Ptr to the X-Org of row j is Src[j]+sX, where sX < 0 for
    a positive-width left border; right and bottom borders are 
    alwasy implicit, ie, have no explicit representation.
  BOTH: If a routine needs both the absolute limits and the
    origin of a subrect, let sX,sY designate the latter, ie
    the "shift" or "start" coords, and either oX,oY or sU,sV
    give the additive offsets to the beginning of the memory
    block, so that free(sP[sV]+sU) would properly deallocate
    the block, and free(sP+sV) would kill the array of row ptrs.

18	Fixed bugs (should've followed dbx all the way back to main!)
17	All night chasing bugs.
16	Using reflected images to increase range in bemo3 spin-off.
16	NAMING: c8i3 inputs 8 (possibly color) images, process them as
	examples in 3 dimensions (was: bemo3); g2f1 replaces fwd2, etc.
16	Restructuring code for bemo3
15	Made video with John Barnett, Marney Smyth, TP, & DB.
14	Working on quadratics: worked out inverse schemes.
13	Working on quadratics: started working out the remaining schemes.
12	iqb3 (Inv Quad Back)
12	fqf3 (Fwd Quad Fwd) fqc3 (Fwd Quad Ctr), 
11	debugged fxyQuad, adding fxyQuadFwdBC, fxyQuadFwdBiq, etc.
11	Equations for fxyQuadCtr___
11	mtg. w/ TP, Marney, DB on video for Japan trip
10	fqd3: foward quadratic warping between 3 images
9 Sun	Demo for NSF Site Visit: bemo3
8	fxyQuad, continued (fxySmatQuadBil, etc.)
8	Worked out how to use cubic splines instead of quadratic poly.
	Probably not worth it for only one tween field (3 images, 2 flows)
7	fxyQuad: flow field combination using quadratic fwd diffs, etc.
7	Showed demo for NSF to tp, girosi, berwick, reza
4.1.94	Returned from Winter break.
14.12	Departed into Winter break.
31	
31.10	clock, timing, & net stuff (ctime, etc.)
======= SWITCHING DATE FORMATS -- ABOVE:dd[.mm[.yy]], BELOW:mm.dd
10.17 U	rgbPlugFlt.c: OPT:tmp vars to avoid repeated ptr indexing.
10.16	rgbPlugFlt.c: Fixed border handling, reworked the concept of holes.
9.18	Revisited by THE BUG!  Now have identical code which breaks on IRIS
	but works for SUN4 isolated to a single file: lkflow_bac.c
	(and lkflow_fwd.c).  For now, won't remake librhs.a, but the key
	to the problem lies in this files in gg/o.iris.
9.06	Stabilized morph_bse.c and interface.c against float vs. double
	proto-no-prototype "bug", linking to all dirs.
9.05	Finally isolated "the bug" in warps.c & warpers.c -- floats in
	prototypes, apparently.
9.03	Verified morphprint, morph2, mimic, morphview on Sun's
8.30 	Fwd4 works but badly.
8.29 	Froze fwd1.c -- preserving example of saturation clipping in raw smat.
8.29 	Fixed rowSmatBiq.c upto 2 images.


8.29 U	Rewrote rowStat.c to libim.a standards, adding new functions.
8.28 	
8.24	Fixed rowFill.c
8.23	Fixed rowFill.c
8.22 M	Argument parsing (unParse, etc.)
8.21	Remodulizing main as a server, warp functions as clients.
8.20	Continued working on fwd1, fwd2, fwd4.
8.19	Demo of my Fwd vs. Sarnoff/Beymer approx. inverse warping.
8.18	Rewrite rowFill.c
8.17	Fixed rowPlug.c
8.16	Pseudo-color & data-bucket (histogram/posterization) functions
8.15	rowSmat*.c	

8.12	Tested new border creation & filling routines.	
8.11	Tested new border creation & filling routines.	

7.29	rowFile.c, rowFill.c
7.28	Higher moments in rowStat.c
7.28	Debugged wxyFill.c
7.27	vxy --> wxy
7.27	ff2.c
7.26	rowPost.c, rowPclr.c, rowMult.c, vxyFillUnc.c, vxyFillFlt.c
7.26	Fixed rowPlug.c
7.25	vxyInvt.c, vxyFill.c  fillvec.c
7.24	rowFile.c, rowFill.c, fillrow.c
7.23	New memFillUnc, better than old memFillUncA.
7.23	Square spiral: for-loop much cleaner than do-loop, and still w/ ptrs.
7.23	rowBord.c, memCrop.c, memEdge.c, memMake.c, memFile.c, memSoak.c
7.22	Hit the wall debugging ffN (rowPlug.c ?).
7.22	ggN debugged & frozen.  Warps an "arbitrary" number of faces in sequence.
7.22	rowInvert.c, rowPlug.c
7.21	rowMake.c
7.21	ggF is not so close to gg4; some level of difference is expected, but
	I'm not sure how much -- may try to analyze the differences and or
	modify parameters in ggF.c (low priority).
7.21	ggT and gg2 give identical output, proving that RH's O-Flow doesn't
	have to be reworked just to get forward mapping.  (gg2 and ggT both
	use Beymer's method, but ggT switches endpoints in the O-Flow call,
	finding imQ<--imP instead of imP<--imQ).
7.21	Random walk on unit sphere, N-gon perimeter walker, etc.
7.21	Utility functions: clipN, unitSpherize, normVectSumFlt, bbGP_ggN, etc.
7.21	Reversed conclusion of 7.18 on sub-image targets in rowWarp*,
	but waiting a few days before implementing all the changes.
7.20	ggN.c -- generalized old Beymer method, arbitrary number of images. 
7.20	Froze gg4.c and ggF.c
7.20	Test harness renamed from ggT to ggO to make room for ggT <--> gg2 (BI & GB
	versions of a 2 src morpher).
7.20	Analyzed the generalized Beymer and "Backwards Inverse" methods for N > 2
	images: pretty bad.  It seems the Beymer method is fundamentally less
	correct than BI, but benefits from offsetting errors (blending cancels
	warping, and fixing eye locations, tho wrong, agrees with translation
	compensation that O-flow was originally designed to do).
7.19	gg4: generalized Beymer method for N images, calling rowWarpBil{Flt|Unc}N.
7.19	Froze gg2, made test harness ggT
7.19	Fixed x5fromAlphaBeta to be intuitive order 1,2,3,4 counterclockwise sqr.
7.19	extractWholeFaceBordLR (morph_base.c)
7.19	reMkdir (unSys.c)
7.18	Started transition from imWarp to rowWarp: all the new conventions
        & optimizations.  Definitely no x0,y0 - rowPtrs must be
	set before hand, no parameters for dealing with various arrays differently.
7.18	Changed sign convention to the intuitive: srcA goes all the way to srcB
        when beta == 1.0
7.18	Debugging & optimization of 2-image interpolators.
7.17	Several ad hoc V-fld inverters.
7.17	memFill.c, memPlug.c: resumed work on smoothing & filling images & v-flds..
7.16	Figured out what causes the edge effects in ggDav.
7.16	New path for fgOldFig: Hourglass & then big egg circumf.
7.16	Debugged fgOldMix, imWarBilMixFlt
7.16	broke RBGg function into 3:
	getting GG, setting bb, and making fwd vX,vY.
7.16	fixed argType, wrote argUnion
7.15	rewrote ggDavF to do one flt blend/clip/conversion to unc in one
	step at the end
	using rowBlnd.c
7.15	created ggDavF.c
7.15	changed ggDav back to warp & blend in unc domain.
7.15	created fgOld, fgMed, fgNew...
7.14	argType: single function for getting int, double, etc. from argv[j].
7.13	remade ggFig w/o all the spaghetti & meatball #ifdefs & branching
7.10	Optimized warp functions.
7.8	USAGE macro
7.7	RBF combining of V-flds, revisited.
7.6	unParse.c: argType, cmd-line parsing.
7.4	unArgs.h, unBug.c, other varargs functions.

DEMOS:
	gg2: DB, but all in float, with options for SLIN, BLNR, BQDR, and UNCHR
	     Show A, B, A+B, and B+A


	hh2: combining P<-Q && Q<-P  --  May use both all the time, or each for 
             half the time.
	gh2: if (beta < .5), used ff2 method; if > .5, use Beymer method
        Do "relaxation" between P<-Q and Q<-P to get P<->Q
        

	test optimal rowWarpBil before changing border handling.
	borders in srcs:
	1.	Warp knows nothing of border, thus must stop at bdry.
	2.	Warp knows size of border, and looks in border only for
		src info; dst size & the basic algorithm unaffected,
                only where the src bdry is. 
 	3.	Warp knows border size and makes dst that much smaller --
		so initial & final j & k change, as well as bdry checks,
                & wide/tall apply to all sources (P & V).  One advantage
		is that then row ptrs don't have to be adjusted for each
                new size, (tho they still can be for effects).

        Arguments: silly for Warp to rely on border of *at least* 1 or 2
        pixels, without knowing exactly how big -- there will alwasy be a
        hard limit, so basic alg. won't change, just use all the info
        available.
		

move spix/libpixrect into its own directory, so not constantly remade
eventually move all mo*.c out, too (back to faces/morph, but with name-suffix



PROGRAM	  NEW	FUNCTIONS	SCHEME/METHODS		STATUS	DATE
-------	  ---	---------	--------------		------	----
ggN	  7.20   rowWarpBilFltN,RBF(N)  Gen. Old Beymer	Testing	7.20
ggO	  7.18   optimization testor cloned from gg2	Keeper	7.20
ggF.c	  6.9   same as gg4	Backward Inv != Fwd	Testing	7.20
gg4.c     6.9   rowWarpBilFltN,RBF4  Gen. Old Beymer	Frozen	7.20
ggT.c     7.20	same as gg2	Backward Inv != Fwd	Testing	7.20
gg2       7.18	rowWarp*2	Old Beymer (one func.)	Frozen	7.20
fgMed	  7.14	To rise from the ashes of fgOldMix
fgOldMix  7.14	?
fgOldF	  7.14	imWarp*Flt, rowClip, imRBFgFwd4 (1-src)	???	7.15
fgOld	  7.14	imWarp*Unc, imRBFgFwd4 (single src)	Keeper	7.15
ggDavF	  7.14	imWarp*Flt, rowBlend2FltToUnc; inverse	Frozen	7.15
ggDav	  7.2	imWarp*Unc, blend2images; std inv warp	Frozen	7.15
ggFig	  7.9	Disso4 (0,0)	naive fwd s-p warp 	Frozen	7.14
ggFigOld  7.11	Disso4 (0,0)	inv warp (just to see)	Frozen	7.14
ggFigBad  7.11	Disso4 (0,0)	inv warp (just to see)	Frozen	7.14
ggFigCol  7.11	Disso4 (0,0)	inv warp (just to see)	Frozen	7.14
ggFigNew  7.11	Disso4 (0,0)	inv warp (just to see)	Frozen	7.15



DELETE		REPLACEMENT
adm	has sigma from cmdline
ammo	

ggBm,ggbm,ggcm	NONE
abLF.c		ggFwd.c
abL.c		ggRes.c
gg00.*		NONE
gga,ggA		NONE (ggDav)
ggB,ggb		ggBac
ggD,ggd		ggDav
ggF,ggf		ggFor (done)
ggC,ggc		ggCrc
ggz		ggNew (from ggDav?)
mof		adm ?

------- JULY 10 -------
ggDav, morph, and mo all do exactly the same thing using
	slightly different functions (ggDav is the most efficient).
	ggDav should soon be fixed to handle boundaries better (Done Jul 10).
	NB: The old ggDav, now called ggDavOld, which called the mixed
	image format warping function bilinInvWarpFimg,
	broke when some change was made to that routine around July 8
	-- it also used to do the same as the others, but is obsolete anyway.
	
ggFwd was the same as ggBac -- now changed to use linForWarpF -> imForWarp*.

Give everything that stays a gg- or mo-prefix, make libgg of stuff that's stable
bic forward & backward, then go for multiples

Should vectorize images from the start...?
Key:
sym	symbols, roughly divided into identifiers and operators
ids	symbols used mainly for distinguishing like objects
ops	symbols which may be combined with other symbols to produce functions, objects,
	relations, values, etc.
obj	objects: things that retain some identity through transformations or in different contexts,
	not (always) derivable from other things.
val	values:	properties, quantities, etc. pertaining to and derivable from things
	via various operations
fnc	functions or mappings: produce values from objects, values, etc.
rel	relations
	
gg	ids	Project name
im	ids	Any array of row ptrs (an image, 2-D vector field, matrix, etc.)
P,Q,R,S	obj	Example images, P usually central (also labelled 0,1,2,3 & A,B,C,D)
Pc,Pf,Pv obj	Same image P stored as unc,flt, and 2-vectors w/ color (x,y,c)
p,q,r	obj	Points in P,Q,R, not necessarily in bounds or with integer coords
Cp,Cq	val	Pixvals at p, q (intensity, hue, etc.)
		(May be thought of as an abbreviation of CPp, CP[x,y], etc.)
L	ops	Locations in some spatial coord frame
U	obj	Absolute or "Universal" Lframe.
LP	obj	Entire Lframe of P = all of P's pixlocs = all p in P
C	ops	Color/Texture-mapping or pixval (re)sampling operation
		(2-slots: C from some P to some LQ, thus CPLQ)
Cx	ops	x is a particular color/texture-mapping mode or procedure
		(For compactness, x is usually dropped from schematics.)
CP	ops	Coloring (in some mode x) using pixval info from P
CPLQ	obj	Lframe of Q recolored (in some way) with pixval info from P
		(Thus P's natural appearance is as CP in LP, or CPLP)
F	ops	Forward Vector Field (from some P "forward" to some Q)
G	ops	Inverse Vector Field (from some Q "backward" to some P)
FPQ	obj	forward vector field from P to Q (usually in Lframe of P, i.e., indexed by LP)
GPQ	obj	inverse vector field from P to Q in Lframe of Q (usually in Lframe of Q)
FPQp	obj	FPQ at p, a vector from p in P to q in Q (usually in Lframe of P) 
Fp	obj	Abbreviation for FPQp when Q is clear.
GPQq,Gq	obj	GPQ at q, a vector from q in Q to p in P (in LQ), Gp is abbreviation
LFPQp	val	The point q in Q to which FPQ maps p (in LP, not frame-independent)
LGPQq	val	The point p in P to which VPQ maps q (generally different in LP & LQ)

CFp	fnc	Given some Q, return CFPQp, the "color" of Q at LFPQp
CGq	fnc	Given some P, return CGPQq, the "color" of P at LGPQq

A,B,C,D	ids	Affine, Bilinear, Bicubic, Biquadratic

testForCorres(P,Q,FPQ)
	output (A): CLFPQP (P recolored from locations in Q to which FPQ warps P): foreach p in P,
	color p (LPp) with CLFPQp, the color from locaton q in Q to which FPQ maps p (no collisions).
	and/or (B): CPLFPQ (P forward warped via FPQ, i.e. P's pixvals placed in warped
	Lframe with holes, aka CFPQ[P]): every pixval Cp from P is placed at LFPQp
	(collisions should be averaged or interpolated via accumulation, etc)
                          
testInvCorres(P,Q,VPQ)
	output (A): CLGPQQ, Q recolored using pixvals from points p in P to which GPQ maps
	points q in Q (no collisions)
	and/or (B): CQLGPQ, color each locations p "in" P to which points q in Q are
	mapped by GPQ with the values Cq (should handle collisions) 

::::::::NOW TRY THIS:::::::::::::::::::::::::::::::::::::

	strictly inverse warping, all pixels from Q (c,f)
	strictly inverse warping, pixels from P & Q (c,f)
	strictly forward warping, all pixels from P (c,f)
	strictly forward warping, pixels from P & Q (c,f)
	

make a FIMG version of best linWarper to use in abLsqcF

decide on Vxyc struct to keep using.

write the 2way prog.

Mg:	image
MMg	multiple images or "multi-image"

Kg
Mkt  mktAB
Mkm  mkm

Tgo tgo
Tgt tgt
Tgm tgm

Sgo sgo
Sgt sgt
Sgm sgm

uv  U v

Ng


Sg  sg:	Images & one-way correspondences
Tg  tg:	Images & two-way correspondences
Ng  ng:	Images & multiple correspondences

Vg:	Mv:	vector image
VVg:	MMv multiple vector images  (or MVg?)

________SUMMARY_OF_CURRENT_PROGRAMS____________________________________________
bemo3 --> c8i3
c8i3  --> b3c8

Mosaic
  resume in HTML
  resume in DVI -> PS
  Art <-> im \ mg \ a
  Bio <-> lif \ lf \ ab \ bg \ b
  Lit <-> lt \ st \ l
    Dream menu
    Story menu
    Poems menu
    Pubs. menu
  Pub <-> p 
  Wrk <-> Sci/Mth \ s \ c for Cmp
    Beymer faces
    morph mpeg

  DIRS: art <-> im\mg\px\a, bio <-> lf\ab\bg\b, lit <-> lt\st\s\l, 

AREAS OF RESEARCH:
@       Blending pixvals from 4 or more images in proper corres. (7.19 DONE?)
@       Inverting a vector flow field.
@       Combining flow corres. in both directions into one tween.
@       Eliminating a central image and O-flow between 2 copies of same image.
@       Warping thru intermediaries to achieve full rotations.
@       Smoothing a flow-corres. and image resampling simultaneously.
@       Use confidence field with U & V.
@       Warping U & V to make W.
@       Finding the kinds of images most suitable for various corres. finders.

CODE DEVELOPMENT TASKS:

O   relax label requirement: affine-warp-align only if labels are there.
O   make size an option
O       Fix rowSmatBil4 and rowSmatBiq4
O   recompile all with gcc -Wall -ansi, then ka/kaw on SGI & SUN.  Even lint.
O   Fix the fill functions to deal with width != height.
O   Finish optimization testing with ggO, & rid rowWarp of
        failed methods if possible.
O   Develop Mo routines to use all the info.

Manual corres/warp:
-------------------
        display two images + a control panel?
        save flow fields as what format?




Structure of d1c2 == d2:

  parse args
  read img's
  try to read labels
  if labels, affine warp to labels
  else       crop/resize to "compromise" size
  finish converting all images to bordered flt & kill the rest
  call function F1 based on number of images & type of corres & warping
    F1 calls:
      corres fn
      one warping/rendering fn, which is either
        X-interface interactive (with option to save one at a time)
        writes a sequence of files, then spawns a movie viewer.

  THUS: new libsgi.a for all SGI depened display code (but not file i/o)



#endif /*0*/

