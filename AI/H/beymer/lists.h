/************************************
 beymer/lists.h
 ************************************/
#ifndef beymer_lists_h_
#define beymer_lists_h_

#include "beymer/boolean.h"

typedef struct cell_s {
  void *car;
  void *cdr;
} cellBody,*cell;

typedef cell List;
typedef void *mapFun(List);
typedef void collectFun(List);

#define push(a,l) ((l) = cons((a),(l)))
#define pop(l) (popIntern(&(l)))

#define L_CAR(L)        ((L) ? L->car : NULL)
#define L_CDR(L)        ((L) ? L->cdr : NULL)
#define L_PUSH(A,L)	{cell _C=(cell)malloc(sizeof(cellBody));_C->car=A;_C->cdr=L;L=_C;}

#if 0  /* you've got to be kidding. */
#define nil ((List)NULL)
#endif

#ifndef _NO_PROTO

/* lists.c */
List cdr(List l);
void *car(List l);
void *second(List l);
void *nth(List l, int n);
void *popIntern(List *l);
int  endp(List l);
List cons(void *theCar, List theCdr);
void map(List l, mapFun *f);
List collect(List l, collectFun *c);
int ListLength(List l);
List member(void *item, List l);
void InsertSort(void *item, List *l, int (*f )(void *, List ));
void RemoveElement(void *item, List *l);
List append(List l1, List l2);
List list_reverse(List l);
List list_reverse2(List l1, List l2);
List SortList(List lst, int (*comparator )());

#else

List cdr();
void *car();
void *nth();
void *first();
void *popIntern();
BOOLEAN endp();
List cons();
void map();
List collect();
int ListLength();
List member();
void InsertSort();
void RemoveElement();
List list_append();
List list_reverse();
List SortList();

#endif

#endif /* beymer_lists_h_ */


