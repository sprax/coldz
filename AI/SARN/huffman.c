/* Huffman functions */
#define TRUE 1
#define FALSE 0
#include <limits.h>
#define NOCHILD	((short) 0x8000)	/* smallest signed short integer */
#define MAXCOUNT ((unsigned) UINT_MAX)	/* biggest unsigned integer (~0)? */

#ifdef SUN	/* hack @@ kludge */
#define _NO_PROTO
#endif

#ifndef _NO_PROTO
static short make_table(void);
static void scale(unsigned int ceil);
#else
static short make_table();
static void scale();
#endif
static void heap(), adjust(); 
static int cmptrees(), bld_tree(), maxchar(), init_enc(), buildenc();

/*
 * The following array of structures are the nodes of the
 * binary trees. The first "nsym" nodes become the leaves of the
 * final tree and represent the symbols being encoded.
 * The remaining nodes become the internal nodes of the final tree.
 */

typedef struct {
   unsigned int freq;		/* number of appearances */
   unsigned char depth;		/* length on longest path in tree */
   short lchild, rchild;	/* indices to next level */
} NODE;				/* NODE Structure */
static NODE *node;
typedef struct {
   int sym;			/* index into h array */
   unsigned char len;		/* number of bits in code */
   unsigned short code;		/* code itself, right adjusted */
} CODETABLE;
static CODETABLE *ct;
static nsym;			/* number of symbols with non-zero counts */
static unsigned short tcode;	/* temporary code value */

#ifdef TABLES
double fasthuff(h,code,len,lb,ub)
#else
double fasthuff(h,lb,ub)
#endif
int *h,lb,ub;
#ifdef TABLES
unsigned short *code;		/* code, right adjusted */
unsigned char *len;		/* number of bits in code */
#endif
{
  int k,nk,sym;
  double rate;
  
  nsym = 0;
  for (k=lb; k <= ub; k++) {
    if (h[k] > 0)
      nsym++;
#ifdef TABLES
   code[k] = len[k] = 0;
#endif
  }
  if (nsym <= 1)
    return(0.0);

  node = (NODE *) malloc((2*nsym-1)*sizeof(NODE));
  ct = (CODETABLE *) malloc(nsym*sizeof(CODETABLE));
  for (k=lb, nsym=0; k <= ub; k++)
    if ((nk = h[k]) > 0) {
      node[nsym].freq = nk;
      node[nsym].depth = 0;
      node[nsym].lchild = node[nsym].rchild = NOCHILD;
      ct[nsym].sym = k;
      nsym++;
    }

  make_table();
  free(node);

  for (rate=0.0, k=0; k < nsym; k++) {
    sym = ct[k].sym;
#ifdef TABLES
    code[sym] = ct[k].code;
    len[sym] = ct[k].len;
#endif
    rate += (double)h[sym]*ct[k].len;
  }
  free(ct);
  return(rate);
}



static short make_table(/* void */)  /* Sun's /bin/cc loses */
{
  unsigned int ceiling;		/* limit for scaling */
  register short i;
  static short *sym;	/* list of intermediate binary trees */
  short tree;		/* root node of final tree */

  sym = (short *) malloc(nsym*sizeof(*sym));
  ceiling = MAXCOUNT;

  do {	/* Keep trying to scale and encode */
    scale(ceiling);
    ceiling /= 2;	/* in case we rescale */

    /* Build list of single node binary trees having
       leaves for the input values with non-zero counts */
    for (i=0; i < nsym; ++i) {
      sym[i] = i;
      node[i].depth = 0;
    }
    heap(sym, nsym);
    /* Convert the list of trees to a single decoding tree */
    bld_tree(&tree, sym, nsym);

    /* Initialize the encoding table */
    init_enc();
    /* Try to build encoding table. Fail if any code is > 16 bits long. */
  } while (buildenc(0, tree));
  free(sym);
  return(tree);
}

 
/* The count of number of occurrences of each input value
 * have already been prevented from exceeding MAXCOUNT.
 * Now we must scale them so that their sum doesn't exceed
 * ceiling and yet no non-zero count can become zero.
 * This scaling prevents errors in the freqs of the
 * interior nodes of the Huffman tree and also ensures that
 * the codes will fit in an unsigned integer. Rescaling is
 * used if necessary to limit the code length.
 */

static void scale(ceil)
unsigned int ceil;	/* upper limit on total freq */
{
  register short i;
  register unsigned int w, sum;
  register short ovflw, divisor;
  char increased;		/* flag */

  do {
    for (i = 0, sum = ovflw = 0; i < nsym; ++i) {
      if (node[i].freq > (ceil - sum))
	++ovflw;
      sum += node[i].freq;
    }

    divisor = ovflw + 1;

    /* Ensure no non-zero values are lost */
    increased = FALSE;
    for (i = 0; i < nsym; ++i) {
      w = node[i].freq;
      /* Don't fail to provide a code if it's used at all */
      if (w < divisor && w != 0) {
        node[i].freq = divisor;
        increased = TRUE;
      }
    }
  } while(increased);

  /* Scaling factor choosen, now scale */
  if (divisor > 1)
  for (i = 0; i < nsym; ++i)
    node[i].freq /= divisor;
  return;
}
    
/* heap() and adjust() maintain a list of binary trees as a
 * heap with the top indexing the binary tree on the list
 * which has the least freq or, in case of equal freqs,
 * least depth in its longest path. The depth part is not
 * strictly necessary, but tends to avoid long codes which
 * might provoke rescaling.
 */

static void heap(list, length)
short list[];
short length;
{
  register short i;

  for (i = (length-2) / 2;   i >= 0;   --i)
    adjust(list, i, length-1);
  return;
}

/* Make a heap from a heap with a new top */

static void adjust(list, top, bottom)
short list[];
register short top, bottom;
{
  register short j, k, temp;
  short minleaf = (bottom + 1)/2;

  j = top;
  while (j < minleaf) {
    k = (j<<1)+1;	/* left child of j = 2*j+1 */
    temp = list[j];
    if ( k < bottom && cmptrees(list[k], list[k+1]))
      ++k;

    /* k indexes "smaller" child (in heap of trees) of j */
    /* now make top index "smaller" of old top and smallest child */
    if (!cmptrees(temp, list[k]))
      break;
    list[j] = list[k];
    list[k] = temp;
    j = k;
  }
return;
}


/*
 * Compare two trees, if a > b return true, else return false
 * note comparison rules in previous comments.
 */

static int cmptrees(a, b)
register short a, b;	/* root nodes of trees */
{
  if (node[a].freq > node[b].freq)
    return TRUE;
  if (node[a].freq == node[b].freq)
    if (node[a].depth > node[b].depth)
      return TRUE;
  return FALSE;
}


/* HUFFMAN ALGORITHM: develops the single element trees
 * into a single binary tree by forming subtrees rooted in
 * interior nodes having freqs equal to the sum of freqs of all
 * their descendents and having depth counts indicating the
 * depth of their longest paths.
 *
 * When all trees have been formed into a single tree satisfying
 * the heap property (on freq, with depth as a tie breaker)
 * then the binary code assigned to a leaf (value to be encoded)
 * is then the series of left (0) and right (1)
 * paths leading from the root to the leaf.
 * Note that trees are removed from the heaped list by
 * moving the last element over the top element and
 * reheaping the smaller list.
 */

static bld_tree(tree, list, len)
short *tree;
short list[];
register short len;
{
  register short freenode;	/* next free node in tree */
  register NODE *frnp;		/* free node pointer */
  register short lch, rch;	/* temps for left, right children */

  /* Initialize index to next available (non-leaf) node.
     Lower numbered nodes correspond to leaves (data values) */
  freenode = nsym;
  while(len > 1) {
   /* Take from list two btrees with least freq and build an interior 
      node pointing to them. This forms a new tree. */
   lch = list[0];	/* This one will be left child */

    /* delete top (least) tree from the list of trees */
    list[0] = list[--len];
    adjust(list, 0, len - 1);

    /* Take new top (least) tree. Reuse list slot later */
    rch = list[0];	/* This one will be right child */

    /* Form new tree from the two least trees using a free node as root. 
       Put the new tree in the list. */
    frnp = &(node[freenode]);	/* address of next free node */
    list[0] = freenode++;	/* put at top for now */
    frnp->lchild = lch;
    frnp->rchild = rch;
    frnp->freq = node[lch].freq + node[rch].freq;
    frnp->depth = 1 + maxchar(node[lch].depth, node[rch].depth);
    /* reheap list  to get least tree at top*/
    adjust(list, 0, len - 1);
  }
  *tree = list[0];	/* head of final tree */
}


static int maxchar(a, b)
int a,b;
{
  return ((a > b) ? a : b);
}


static init_enc()
{
  register short i;

  /* Initialize encoding table */
  for (i = 0; i < nsym; ++i) {
    ct[i].len = 0;
  }
}


/* Recursive routine to walk the indicated subtree and level
 * and maintain the current path code in bstree. When a leaf
 * is found the entire code string and length are put into
 * the encoding table entry for the leaf's data value.
 *
 * Returns TRUE if codes are too long.
 */
static 
unsigned mask[] = { 00,01,03,07,017,037,077,0177,0377,0777,01777,03777,07777,
	017777,037777,077777,0177777,0177777,0177777,0177777,0177777,0177777,
	0177777,0177777,0177777,0177777,0177777,0177777,0177777,
	0177777,0177777,0177777,0177777,0177777,0177777,0177777,
	0177777,0177777,0177777,0177777,0177777,0177777,0177777 };

static buildenc(level, root)
unsigned char level;	/* level of tree being examined, from zero */
short root;		/* root of subtree is also data value if leaf */
{
  register short l, r;

  l = node[root].lchild;
  r = node[root].rchild;


  if ( l == NOCHILD && r == NOCHILD) {
    /* Leaf. Previous path determines bit string code of length level 
       (bits 0 to level - 1). Ensures unused code bits are zero. */
    ct[root].len = level;
    ct[root].code = tcode & mask[level];
    return (level > 16);
  } 
  else {
    if ( l != NOCHILD) {
      /* Clear path bit and continue deeper */
      tcode &= ~(1 << level);
      /* NOTE RECURSION */
      if (buildenc(level + 1, l))
        return TRUE;
    }
    if (r != NOCHILD) {
      /* Set path bit and continue deeper */
      tcode |= (1 << level);
      /* NOTE RECURSION */
      if (buildenc(level + 1, r))
        return TRUE;
    }
  }
  return FALSE;	/* if we got here we're ok so far */
}




double quickhuff(h,lb,ub)
int *h,lb,ub;
{
  struct symbol {
    int pos;
    int prob;
    int leftson;
    int rightson;
    short len;
  } *sym;
  int *symptr,nsym,topsym,newsym;
  int lo,mid,hi,bot,top;
  int i,j,k,nk,kptr;
  double rate;

  nsym = 0;
  for (k=lb; k <= ub; k++)
    if (h[k] > 0)
      nsym++;
  if (nsym <= 1)
    return(0.0);

  sym = (struct symbol *) malloc((2*nsym+1)*sizeof(struct symbol));
  symptr = (int *) malloc((2*nsym+1)*sizeof(int));

  for (k=lb, nsym=0; k <= ub; k++)
    if ((nk = h[k]) > 0) {
      nsym++;
      sym[nsym].pos = k;
      sym[nsym].prob = nk;
      symptr[nsym] = nsym;
    }

/* Heapsort the symbol table.  Rearrange symptr array only. */

  for (k=2; k <= nsym; k++) {    /* creating initial heap */
    i = k;
    kptr = symptr[k];
    j = i/2;
    while (j > 0) {
      if (sym[kptr].prob <= sym[symptr[j]].prob)
        break;
      symptr[i] = symptr[j];
      i = j;
      j = i/2;
    }
    symptr[i] = kptr;
  }
  for (k=nsym; k >= 2; k--) {
    kptr = symptr[k];
    symptr[k] = symptr[1];
    i = 1;  j = 2;
    if ((sym[symptr[3]].prob > sym[symptr[2]].prob) && (k-1 >= 3))
      j = 3;
    while (j <= k-1) {
      if (sym[symptr[j]].prob <= sym[kptr].prob) 
        break;
      symptr[i] = symptr[j];
      i = j;  j = 2*i;
      if (j+1 <= k-1) 
        if (sym[symptr[j+1]].prob > sym[symptr[j]].prob)
          j = j + 1;
    }
    symptr[i] = kptr;
  }

/* HUFFMAN CODING ALGORITHM --- requires nsym-1 iterations.  In each iteration
   we create a new symbol which replaces the two symbols with the lowest
   probabilities in the old set of symbols.  Pointers are kept to lead back to
   the replaced symbols.  The new symbol is assigned a probability equal to the
   sum of the probabilities of the two it replaced.  The procedure is repeated
   on the new set of nsym-1 symbols.
*/
  newsym = nsym;
  bot = 1;		/* pointer to lowest valid pointer in symptr */
  top = nsym;		/* pointer to highest valid pointer in symptr */
  for (k=1; k < nsym; k++) {
    newsym++;
    sym[newsym].prob = sym[symptr[bot]].prob + sym[symptr[bot+1]].prob;
    sym[newsym].leftson = symptr[bot];		/* leftson has lowest prob */
    sym[newsym].rightson = symptr[bot+1];
    bot += 2;  lo = bot;  hi = top;
    while (lo <= hi) {
      mid = (lo+hi)/2;
      if (sym[newsym].prob >= sym[symptr[mid]].prob) 
        lo = mid + 1;
      else
        hi = mid - 1;
    }
    if (lo-bot > top-lo) {		/* shift symptr to right */
      for (j=top; j >= lo; j--)
        symptr[j+1] = symptr[j];
      top++;
      symptr[lo] = newsym;
    } else {				/* shift symptr to left */
      for (j=bot-1; j <= lo-2; j++) 
        symptr[j] = symptr[j+1];
      bot--; 
      symptr[lo-1] = newsym;
    }
  }

/* Code assignment - Done by tracing back through the code tree */
  topsym = 2*nsym - 1;
  sym[sym[topsym].leftson].len = 1;
  sym[sym[topsym].rightson].len = 1;
  for (k=topsym-1; k > nsym; k--) {
    sym[sym[k].rightson].len = sym[k].len + 1;
    sym[sym[k].leftson].len  = sym[k].len + 1;
  }

  for (k=1, rate=0; k <= nsym; k++)
    rate += sym[k].len*sym[k].prob;

  free(sym); free(symptr);
  return(rate);

}
 
    
   

