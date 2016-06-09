typedef struct point {
  int x, y;
} Point2D;

typedef struct ENode {
  int pt1, pt2;
  struct TNode *tri1, *tri2;
  struct ENode *next;
} EdgeNode;

typedef struct TNode {
  EdgeNode *e1, *e2, *e3;
  struct TNode *next;
} TriNode;

typedef struct EList {
  EdgeNode *edge;
  struct EList *next;
} EdgeList;

