
~spraxlo/c/h/README.h    S.R.Lines    93.5.22

Most of the .h files here are from either David Beymer
or myself.  As conflicts arise, David's files will migrate
to ./beymer, and a few of mine to ./sprax.

For instance, Paul Haeberli's file
iris:/usr/people/4Dgifts/iristools/include/image.h conflicts with
David's image.h, but in name only.

Just use qualified (or even full) path names, e.g.:
#include "beymer/image.h"
#include "SGI/image.h"
and then compile with IFLAGS = -I${HOME}/c/h ...

For now, the image.h here in h is David's.
But of course, there are a lot of other image.h's out there!

For sources which define another struct image, at least one must change.
I sometimes copy David's image.h to beymage.h, wherein I globally
substitute beymage for image.


