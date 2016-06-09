/*****************************************************************************
 triangulate.c -- Mike Jones.  mods by S.R.Lines    95.07.10
 *****************************************************************************/
#include "triangulate.h"
#include <stdio.h>
#include <malloc.h>
#include <sarnoff/imgmacro.h>
#include <sarnoff/imgdec.h>

extern EdgeNode *edge_list_top, *edge_list_bot;
extern TriNode *tri_list_top, *tri_list_bot;
extern Point2D *src_pts, *dest_pts;
extern int no_points;
extern FIMAGE dx, dy;

EdgeNode *MakeEdge(p1, p2)
int p1, p2;
{
  if (edge_list_top == NULL)  {
    edge_list_top = (EdgeNode *)malloc(sizeof(EdgeNode));
    edge_list_bot = edge_list_top;
  }
  else  {
    edge_list_bot->next = (EdgeNode *)malloc(sizeof(EdgeNode));
    edge_list_bot = edge_list_bot->next;
  }
  edge_list_bot->pt1 = p1;
  edge_list_bot->pt2 = p2;
  edge_list_bot->tri1 = NULL;
  edge_list_bot->tri2 = NULL;
  edge_list_bot->next = NULL;

  return edge_list_bot;
}

TriNode *MakeTriangle(edge1, edge2, edge3)
EdgeNode *edge1, *edge2, *edge3;
{
  if (tri_list_top == NULL)  {
    tri_list_top = (TriNode *)malloc(sizeof(TriNode));
    tri_list_bot = tri_list_top;
  }
  else {
    tri_list_bot->next = (TriNode *)malloc(sizeof(TriNode));
    tri_list_bot = tri_list_bot->next;
  }
  tri_list_bot->e1 = edge1;
  tri_list_bot->e2 = edge2;
  tri_list_bot->e3 = edge3;
  tri_list_bot->next = NULL;

  return tri_list_bot;
}

void eqn_of_plane(a, b, c, d, x1, y1, z1, x2, y2, z2, x3, y3, z3)
int *a, *b, *c, *d, x1, y1, z1, x2, y2, z2, x3, y3, z3;
{
  int v1, v2, v3, w1, w2, w3;

  v1 = x1-x2;
  v2 = y1-y2;
  v3 = z1-z2;
  w1 = x3-x2;
  w2 = y3-y2;
  w3 = z3-z2;

  *a = v2*w3 - v3*w2;
  *b = v3*w1 - v1*w3;
  *c = v1*w2 - v2*w1;

  *d = -(*a)*x1-(*b)*y1-(*c)*z1;
}   

/* returns twice the area of the oriented triangle (a, b, c), ie the
   area is positive if the triangle is oriented counterclockwise */
int TriArea(a, b, c)
Point2D *a, *b, *c;
{
  return (b->x - a->x)*(c->y - a->y) - (b->y - a->y)*(c->x - a->x);
}

/* returns true if the point d is inside the circle defined by the points
   a, b, c. */
int InCircle(a, b, c, d)
Point2D *a, *b, *c, *d;
{
  int ans;

  ans =  (a->x * a->x + a->y * a->y) * TriArea(b, c, d) -
         (b->x * b->x + b->y * b->y) * TriArea(a, c, d) +
	 (c->x * c->x + c->y * c->y) * TriArea(a, b, d) -
	 (d->x * d->x + d->y * d->y) * TriArea(a, b, c) > 0;

  return ans;
}

int InCircle2(a, b, c, d)
Point2D *a, *b, *c, *d;
{
  float cx, cy, r2, l2, f;
  float x1, y1, x2, y2, x3, y3;

  if ((a->x == c->x) && (a->x == b->x))  {
    return 0;
  }
  else if (a->x == c->x)  {
    x1 = (float)b->x;  y1 = (float)b->y;
    x2 = (float)a->x;  y2 = (float)a->y;
    x3 = (float)c->x;  y3 = (float)c->y;
  }
  else if (a->x == b->x)  {
    x1 = (float)c->x;  y1 = (float)c->y;
    x2 = (float)b->x;  y2 = (float)b->y;
    x3 = (float)a->x;  y3 = (float)a->y;
  }
  else  {
    x1 = (float)a->x;  y1 = (float)a->y;
    x2 = (float)b->x;  y2 = (float)b->y;
    x3 = (float)c->x;  y3 = (float)c->y;
  }
  f = (x2-x1)/(x3-x1);

  cy = (1.0/(2.0*(y1-y2) + 2.0*f*(y3-y1))) * (y1*y1 - y2*y2 + x1*x1 - x2*x2 + f*(x3*x3 - x1*x1 + y3*y3 - y1*y1));
  cx = 0.5*(x2*x2 - x1*x1 + (y2-cy)*(y2-cy) - (y1-cy)*(y1-cy))/(x2-x1);
  r2 = (x1-cx)*(x1-cx) + (y1-cy)*(y1-cy);
  l2 = (cx - (float)d->x)*(cx - (float)d->x) + (cy - (float)d->y)*(cy - (float)d->y);

  return l2 < r2;
}

/* Does the line segment p0 -> p1 intersect the line segment p2 -> p3? */
int Intersect(p0, p1, p2, p3)
Point2D *p0, *p1, *p2, *p3;
{
  int x1, x2, x3, x4, y1, y2, y3, y4, s1, s2;

  x1 = MIN(p0->x, p1->x);
  x2 = MAX(p0->x, p1->x);
  y1 = MIN(p0->y, p1->y);
  y2 = MAX(p0->y, p1->y);
  x3 = MIN(p2->x, p3->x);
  x4 = MAX(p2->x, p3->x);
  y3 = MIN(p2->y, p3->y);
  y4 = MAX(p2->y, p3->y);

  if (! ((x2 >= x3) && (x4 >= x1) && (y2 >= y3) && (y4 > y1)))
    return 0;
  else if ((p2->x == p3->x) && (p1->x == p2->x))  {
    /* p1 is on line seg p2->p3 */
    return 2;
  }
  else if (fabs((double)p1->y - (((double)(p3->y - p2->y)/((double)p3->x - p2->x))*(double)p1->x + (double)p2->y - ((double)(p3->y - p2->y)/(double)(p3->x - p2->x))*(double)p2->x)) < 0.000001)  {
    /* p1 is on line seg p2->p3 */
    return 2;
  }
  else  {
    x1 = p2->x - p0->x;
    y1 = p2->y - p0->y;
    x2 = p1->x - p0->x;
    y2 = p1->y - p0->y;
    s1 = SGN(x1*y2 - x2*y1);

    x1 = p3->x - p0->x;
    y1 = p3->y - p0->y;
    s2 = SGN(x1*y2 - x2*y1);
    if ((s1 == 0) || (s2 == 0))
      return 1;
    else if (s1 == s2)
      return 0;
    else  {
      x1 = p0->x - p2->x;
      y1 = p0->y - p2->y;
      x2 = p3->x - p2->x;
      y2 = p3->y - p2->y;
      s1 = SGN(x1*y2 - x2*y1);

      x1 = p1->x - p3->x;
      y1 = p1->y - p3->y;
      s2 = SGN(x1*y2 - x2*y1);
      if ((s1 == 0) || (s2 == 0))
	return 1;
      else if (s1 == s2)
	return 0;
      else
	return 1;
    }
  }
}

int InTriangle(tri, p)
TriNode *tri;
Point2D *p;
{
  int intersections = 0;
  Point2D p0;
  int i1, i2, i3;

  /* The code relies on p0 being set this way */
  p0.x = -1;  p0.y = p->y-1;

  if ((i1=Intersect(&p0, p, &(src_pts[tri->e1->pt1]), &(src_pts[tri->e1->pt2]))))
    intersections++;

  if ((i2=Intersect(&p0, p, &(src_pts[tri->e2->pt1]), &(src_pts[tri->e2->pt2]))))
    intersections++;

  if ((i3=Intersect(&p0, p, &(src_pts[tri->e3->pt1]), &(src_pts[tri->e3->pt2]))))
    intersections++;

  if ((i1 == 2) || (i2 == 2) || (i3 == 2))  {
    return 1;
  }
  else if (intersections % 2)
    return 1;
  else
    return 0;
}

EdgeNode *EdgeThrough(p1, p2, tri)
int p1, p2;
TriNode *tri;
{
  if (((tri->e1->pt1 == p1) && (tri->e1->pt2 == p2)) || ((tri->e1->pt1 == p2) && (tri->e1->pt2 == p1)))
    return tri->e1;
  else if (((tri->e2->pt1 == p1) && (tri->e2->pt2 == p2)) || ((tri->e2->pt1 == p2) && (tri->e2->pt2 == p1)))
    return tri->e2;
  else
    return tri->e3;
}

void DeleteEdge(edge)
EdgeNode *edge;
{
  EdgeNode *edge_ptr;

  edge_ptr = edge_list_top;

  if (edge_list_top == edge)  {
    edge_list_top = edge_list_top->next;
    if (edge_list_bot == edge_ptr)
      edge_list_bot = NULL;
    free(edge_ptr);
  }
  else  {
    while ((edge_ptr->next != edge) && (edge_ptr->next->next != NULL))  {
      edge_ptr = edge_ptr->next;
    }

    if (edge_ptr->next == edge)  {
      edge_ptr->next = edge_ptr->next->next;
      if (edge_list_bot == edge)
	edge_list_bot = NULL;
      free(edge);
    }
    else  {
      fprintf(stderr, "Error: tried to delete an edge that didn't exist.\n");
    }
  }
}

void DeleteTriangle(tri)
TriNode *tri;
{
  TriNode *tri_ptr;

  tri_ptr = tri_list_top;

  if (tri_list_top == tri)  {
    tri_list_top = tri_list_top->next;
    if (tri_list_bot == tri_ptr)
      tri_list_bot = NULL;
    free(tri_ptr);
  }
  else  {
    while ((tri_ptr->next != tri) && (tri_ptr->next->next != NULL))  {
      tri_ptr = tri_ptr->next;
    }

    if (tri_ptr->next == tri)  {
      tri_ptr->next = tri_ptr->next->next;
      if (tri_list_bot == tri)
	tri_list_bot = NULL;
      free(tri);
    }
    else  {
      fprintf(stderr, "Error: tried to delete a triangle that didn't exist.\n");
    }
  }
}
void free_tri_list()
{
  EdgeNode *edge;
  TriNode *tri;

  while (tri_list_top != NULL) {
    tri = tri_list_top;
    tri_list_top = tri_list_top->next;
    free(tri);
  }

  while (edge_list_top != NULL)  {
    edge = edge_list_top;
    edge_list_top = edge_list_top->next;
    free(edge);
  }
}

void init_tri_list()
{
  EdgeNode *edge1, *edge2, *edge3;
  TriNode *tri;

  if (tri_list_top != NULL)  {
    free_tri_list();
  }

  edge1 = MakeEdge(0,1);
  edge2 = MakeEdge(1,3);
  edge3 = MakeEdge(0,3);
  tri = MakeTriangle(edge1, edge2, edge3);
  edge1->tri1 = tri;
  edge2->tri1 = tri;
  edge3->tri1 = tri;

  edge1 = MakeEdge(0,2);
  edge2 = MakeEdge(2,3);
  tri = MakeTriangle(edge1, edge2, edge3);
  edge1->tri1 = tri;
  edge2->tri1 = tri;
  edge3->tri2 = tri;
}

void triangulate_flow()
{
  int i,j;
  TriNode *tri_ptr, *prev_tri_ptr;
  TriNode *tri1, *tri2, *tri3, *tri4;
  EdgeNode *edge1, *edge2, *edge3, *edge4;
  EdgeList *checklist, *elist_ptr;
  int p1, p2, p3, pt;
  int tlx, tly, brx, bry;
  int a1, b1, c1, d1, a2, b2, c2, d2;
  Point2D point;
  checklist = NULL;

  init_tri_list();

/*  InCircle2(&src_pts[4], &src_pts[3], &src_pts[5], &src_pts[2]);
  InCircle2(&src_pts[4], &src_pts[5], &src_pts[3], &src_pts[2]);
  InCircle2(&src_pts[3], &src_pts[5], &src_pts[4], &src_pts[2]);
*/
  for (i=4; i<no_points; i++)  {
    /* look through current triangle list to find out which triangle
       this one is in. */
    tri_ptr = tri_list_top;
    prev_tri_ptr = tri_ptr;
    while ((tri_ptr != NULL)  && (! InTriangle(tri_ptr, &src_pts[i])))  {
      if (prev_tri_ptr != tri_ptr)
	prev_tri_ptr = prev_tri_ptr->next;
      tri_ptr = tri_ptr->next;
    }

    if (tri_ptr == NULL)  {
      fprintf(stderr, "Error: new point not in existing triangle.\n");
      exit(-1);
    }

/*printf("adding pt %d, it's in triangle %d %d %d %d\n", i, tri_ptr->e1->pt1, tri_ptr->e1->pt2, tri_ptr->e2->pt1, tri_ptr->e2->pt2);*/

    edge1 = MakeEdge(i, tri_ptr->e1->pt1);
    edge2 = MakeEdge(i, tri_ptr->e1->pt2);
    if ((tri_ptr->e2->pt1 != tri_ptr->e1->pt1) && (tri_ptr->e2->pt1 != tri_ptr->e1->pt2))  {
      edge3 = MakeEdge(i, tri_ptr->e2->pt1);
    }
    else {
      edge3 = MakeEdge(i, tri_ptr->e2->pt2);
    }

    /* add 3 new triangles */
    tri1 = MakeTriangle(edge1, edge2, tri_ptr->e1);
    tri2 = (TriNode *)malloc(sizeof(TriNode));
    tri3 = (TriNode *)malloc(sizeof(TriNode));

    edge1->tri1 = tri1;
    edge2->tri1 = tri1;

    if ((edge2->pt2 == tri_ptr->e2->pt1) || (edge2->pt2 == tri_ptr->e2->pt2)) {
      tri2 = MakeTriangle(edge2, edge3, tri_ptr->e2);
      tri3 = MakeTriangle(edge1, edge3, tri_ptr->e3);
      edge1->tri2 = tri3;
      edge2->tri2 = tri2;
      edge3->tri1 = tri2;
      edge3->tri2 = tri3;
    }
    else  {
      tri2 = MakeTriangle(edge1, edge3, tri_ptr->e2);
      tri3 = MakeTriangle(edge2, edge3, tri_ptr->e3);
      edge1->tri2 = tri2;
      edge2->tri2 = tri3;
      edge3->tri1 = tri2;
      edge3->tri2 = tri3;
    }

    /* reassign old triangle pointers on old edges - put new triangles in tri1 slot */
    if (tri_ptr->e1->tri1 == tri_ptr)
      tri_ptr->e1->tri1 = tri1;
    else  {
      tri_ptr->e1->tri2 = tri_ptr->e1->tri1;
      tri_ptr->e1->tri1 = tri1;
    }

    if (tri_ptr->e2->tri1 == tri_ptr)
      tri_ptr->e2->tri1 = tri2;
    else  {
      tri_ptr->e2->tri2 = tri_ptr->e2->tri1;
      tri_ptr->e2->tri1 = tri2;
    }

    if (tri_ptr->e3->tri1 == tri_ptr)
      tri_ptr->e3->tri1 = tri3;
    else  {
      tri_ptr->e3->tri2 = tri_ptr->e3->tri1;
      tri_ptr->e3->tri1 = tri3;
    }

    /* remove old triangle */
    if (prev_tri_ptr == tri_ptr)  {
      tri_list_top = tri_ptr->next;
      free(tri_ptr);
    }
    else  {
      prev_tri_ptr->next = tri_ptr->next;
      free(tri_ptr);
    }

    /* add old edges to checklist if they are part of 2nd triangle */
    if (tri1->e3->tri2 != NULL)  {
      elist_ptr = (EdgeList *)malloc(sizeof(EdgeList));
      elist_ptr->edge = tri1->e3;
      elist_ptr->next = checklist;
      checklist = elist_ptr;
    }
    if (tri2->e3->tri2 != NULL)  {
      elist_ptr = (EdgeList *)malloc(sizeof(EdgeList));
      elist_ptr->edge = tri2->e3;
      elist_ptr->next = checklist;
      checklist = elist_ptr;
    }
    if (tri3->e3->tri2 != NULL)  {
      elist_ptr = (EdgeList *)malloc(sizeof(EdgeList));
      elist_ptr->edge = tri3->e3;
      elist_ptr->next = checklist;
      checklist = elist_ptr;
    }

    /* check if edges on checklist satisfy Delaunay criterion */
    while (checklist != NULL)  {
      /* determine 3rd point in tri2 for next edge on checklist */
      edge1 = checklist->edge;
      tri1 = edge1->tri1;
      tri2 = edge1->tri2;
      p1 = edge1->pt1;
      p2 = edge1->pt2;

      /* remove top entry on checklist */
      elist_ptr = checklist;
      checklist = checklist->next;
      free(elist_ptr);

      if ((tri2->e1->pt1 != p1) && (tri2->e1->pt1 != p2))
	pt = tri2->e1->pt1;
      else if ((tri2->e1->pt2 != p1) && (tri2->e1->pt2 != p2))
	pt = tri2->e1->pt2;
      else if ((tri2->e2->pt1 != p1) && (tri2->e2->pt1 != p2))
	pt = tri2->e2->pt1;
      else
	pt = tri2->e2->pt2;

      /* see if pt is inside tri1 circumcircle */
      if ((tri1->e1->pt1 != p1) && (tri1->e1->pt1 != p2))
	p3 = tri1->e1->pt1;
      else
	p3 = tri1->e1->pt2;

      if (InCircle2(&src_pts[p1], &src_pts[p2], &src_pts[p3], &src_pts[pt]))  {
	/* bad edge found - swap it */
	edge2 = MakeEdge(p3, pt);
	edge3 = EdgeThrough(p3, p1, tri1);
	edge4 = EdgeThrough(p1, pt, tri2);

	tri3 = MakeTriangle(edge2, edge3, edge4);
	edge2->tri1 = tri3;
	if (edge3->tri1 == tri1)
	  edge3->tri1 = tri3;
	else
	  edge3->tri2 = tri3;

	if (edge4->tri1 == tri2)
	  edge4->tri1 = tri3;
	else  {
	  edge4->tri2 = edge4->tri1;
	  edge4->tri1 = tri3;
	}

/* put edge4 on the checklist if edge4->tri2 != NULL */
        if (edge4->tri2 != NULL)  {
	  elist_ptr = (EdgeList *)malloc(sizeof(EdgeList));
	  elist_ptr->edge = edge4;
	  elist_ptr->next = checklist;
	  checklist = elist_ptr;
	}

	edge3 = EdgeThrough(p3, p2, tri1);
	edge4 = EdgeThrough(p2, pt, tri2);

	tri4 = MakeTriangle(edge2, edge3, edge4);
	edge2->tri2 = tri4;
	if (edge3->tri1 == tri1)
	  edge3->tri1 = tri4;
	else
	  edge3->tri2 = tri4;

	if (edge4->tri1 == tri2)
	  edge4->tri1 = tri4;
	else  {
	  edge4->tri2 = edge4->tri1;
	  edge4->tri1 = tri4;
	}

	/* put edge4 on the checklist if edge4->tri2 != NULL */
        if (edge4->tri2 != NULL)  {
	  elist_ptr = (EdgeList *)malloc(sizeof(EdgeList));
	  elist_ptr->edge = edge4;
	  elist_ptr->next = checklist;
	  checklist = elist_ptr;
	}

	DeleteEdge(edge1);
	DeleteTriangle(tri1);
	DeleteTriangle(tri2);
      }
    }
  }

  /* Done creating triangles.  Now linearly interpolate flow inside
     triangles and fill in dx, dy. */
  tri_ptr = tri_list_top;
  while (tri_ptr != NULL)  {
    p1 = tri_ptr->e1->pt1;
    p2 = tri_ptr->e1->pt2;
    if ((tri_ptr->e2->pt1 != p1) && (tri_ptr->e2->pt1 != p2))
      p3 = tri_ptr->e2->pt1;
    else
      p3 = tri_ptr->e2->pt2;

    tlx = MIN(MIN(src_pts[p1].x, src_pts[p2].x), src_pts[p3].x);
    tly = MIN(MIN(src_pts[p1].y, src_pts[p2].y), src_pts[p3].y);
    brx = MAX(MAX(src_pts[p1].x, src_pts[p2].x), src_pts[p3].x);
    bry = MAX(MAX(src_pts[p1].y, src_pts[p2].y), src_pts[p3].y);

    /* compute eqn of plane through triangle for dx and dy */
    /* 0 = ax + by + cz + d */
    eqn_of_plane(&a1, &b1, &c1, &d1, src_pts[p1].x, src_pts[p1].y, dest_pts[p1].x - src_pts[p1].x, src_pts[p2].x, src_pts[p2].y, dest_pts[p2].x - src_pts[p2].x, src_pts[p3].x, src_pts[p3].y, dest_pts[p3].x - src_pts[p3].x);

    eqn_of_plane(&a2, &b2, &c2, &d2, src_pts[p1].x, src_pts[p1].y, dest_pts[p1].y - src_pts[p1].y, src_pts[p2].x, src_pts[p2].y, dest_pts[p2].y - src_pts[p2].y, src_pts[p3].x, src_pts[p3].y, dest_pts[p3].y - src_pts[p3].y);

    /* now for every pt in bounding box, check if it's in triangle
       and if so, interpolate dx and dy values */
    for (i=tly; i<=bry; i++)  {
      for (j=tlx; j<=brx; j++)  {
	point.x = j;  point.y = i;
	if (InTriangle(tri_ptr, &point))  {
	  if (c1 != 0)
	    dx.ptr[i][j] = (-(float)(a1*j + b1*i + d1))/(float)c1;
	  else
	    printf("Hmmm, c1 is 0 - probably means you have 3 colinear points somewhere\n");
	  if (c2 != 0)
	    dy.ptr[i][j] = (-(float)(a2*j + b2*i + d2))/(float)c2;
	  else
	    printf("Hmmm, c2 is 0 - probably means you have 3 colinear points somewhere\n");
	}
      }
    }

    tri_ptr = tri_ptr->next;
  }
/*
  for (i=0; i<dx.h; i++)
    for (j=0; j<dx.w; j++)
      if (dx.ptr[i][j] >= 200.0)
	printf("hole at (%d, %d)\n", j, i);
*/
}

