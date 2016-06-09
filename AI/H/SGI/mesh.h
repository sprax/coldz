#ifndef MESHDEF
#define MESHDEF

typedef struct Meshobj {
    int			connectcount;
    int			independentcount;
    int			npolys;
    int			vertcount;
    int			vertno;
    struct Edge *	edgearray;
    struct Edge *	freeedges;
    struct Edge **	edgehashlist;
    struct Tri *	curtri;
    struct Tri **	trihashlist;
    struct Trilist *	adjtrilist[4];
    struct Trilist *	donetrilist;
    struct Trilist *	newtrilist;
    struct Trilist *	trilist;
    struct Vert *	tmpvert;
    struct Vert **	verthashlist;
    struct Vertlist *	vertlist;
    void		(*ambegin)( int, int );
    void		(*amend)( void );
    int			(*amhashvert)( long );
    int			(*amvertsame)( long, long );
    void		(*amvertdata)( long );
    void		(*ambgntmesh)( void );
    void		(*amendtmesh)( void );
    void		(*amswaptmesh)( void );
    void		(*amvert)( long );
} Meshobj;

extern	Meshobj * 	newMeshobj ( 
    void (*)( int, int ), void (*)( void ),
    int	 (*)( long ),     int  (*)( long, long ),
    void (*)( long ),     void (*)( void ),
    void (*)( void ),     void (*)( void ),
    void (*)( long ) );
extern	void		freeMeshobj( Meshobj * );
extern	void		in_ambegin( Meshobj * );
extern	void		in_amnewtri( Meshobj * );
extern	void		in_amvert( Meshobj *, long );
extern	void		in_amend( Meshobj * );


/* tomesh.c */
Meshobj *newMeshobj ( void (*out_ambegin )(int, int )
                    , void (*out_amend )(void )
                    , int (*out_amhashvert )(long )
                    , int (*out_amvertsame )(long, long )
                    , void (*out_amvertdata )(long )
                    , void (*out_ambgntmesh )(void )
                    , void (*out_amendtmesh )(void )
                    , void (*out_amswaptmesh )(void )
                    , void (*out_amvert )(long ));

void freeMeshobj(Meshobj *m);
void in_ambegin(Meshobj *m);
void in_amnewtri(Meshobj *m);
void in_amvert(Meshobj *m, long fptr);
void in_amend(Meshobj *m);
#endif


