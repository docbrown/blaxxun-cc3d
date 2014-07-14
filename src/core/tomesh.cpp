/* @doc
 * @module tomesh.cpp - Convert independent triangles to large triangle meshes |
 *
 *							Kurt Akeley - 24 March 1988
 *								Paul Haeberli - 1990
 *						   Derrick Burns - September 1990
 *
 *
 *		The algorithm attempts to avoid leaving isolated triangles by
 *		choosing as the next triangle in a growing mesh that adjacent
 *		triangle with the minimum number of neighbors.	It will frequently
 *		produce a single, large mesh for a well behaved set of triangles.
 *
 *		To use, include the file mesh.h.
 *
 *		exports four routines:
 *
 *				Meshobj *newMeshobj( ... )
 *				void in_ambegin( Meshobj * )
 *				void in_amnewtri( Meshobj * )
 *				void in_amvert( Meshobj *, long )
 *				void in_amend( Meshobj * )
 *				void freeMeshobj( Meshobj * )
 *
 *		calls back these routines:
 *				void out_ambegin( long nverts, long ntris )
 *				void out_amend( void );
 *				int out_amhashvert( long v )
 *				int out_amvertsame( long v1, long v2 )
 *				void out_amvertdata( long fptr )
 *				void out_ambgntmesh( void )
 *				void out_amendtmesh( void )
 *				void out_amswaptmesh( void )
 *				void out_amvert( long index )
 *
 *		on output the package calls:
 *
 *		void out_ambegin( long nverts, long ntris )
 *		calls repeatedly:
 *				int out_amhashvert( long v )
 *				int out_amvertsame( long v1, long v2 )
 *		calls nverts times:
 *				void out_amvertdata( long fptr )
 *		calls these in mixed sequence:
 *				void out_ambgntmesh( void )
 *				  void out_amvert( long index )
 *				  void out_amswaptmesh( void )
 *				void out_amendtmesh( void )
 *		void out_amend()
 */

/*
HG ported to NT
Modifications :
		OUTFIELD Macro

		Swaptmesh not used because not supported in OPENGL

Optimize ?
   more inlines ?
   immediate delete tri after output
   no error checking int outmesh
   no extra function call
   extra begin triangles, end triangles call ?
*/

#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>

#include "gutils.h"


//#include "mesh.h"
#include "tomesh.h"

#ifdef TEST
#define dprintf 		if( 1 ) (void) printf
#else
#define dprintf 		if( 0 ) (void) printf
#endif

#define FALSE			0
#define TRUE			1
#define VERTHASHSIZE	9991
#define TRIHASHSIZE 	9991
#define EDGEHASHSIZE	9991
#define INITMESH		replaceB = FALSE
#define SWAPMESH		replaceB = !replaceB
#define REPLACEVERT(v)	{vert[replaceB] = (v); SWAPMESH;}

typedef struct Vert {
	struct Vert *next,*prev;
	struct Vert *hnext;
	long id, index;
} Vert;

typedef struct Vertlist {
	Vert *first,*last;
} Vertlist;

typedef struct Edge {
	struct Edge *hnext;
	struct Tri *tri;
	long index0, index1;
} Edge;

typedef struct Tri {
	struct Tri *next,*prev;
	struct Tri *hnext;
	struct Tri *(adj[3]);

//   struct Tri * adj[3];

   Vert *vert[3];
	int drawvert[3];
	int adjcount;
	int used;
	int data; /* HG tri belongs to face # data (User Data) */
} Tri;

typedef struct Trilist {
	Tri *first,*last;
} Trilist;

typedef struct {

	void		(*ambegin)( int, int );
	void		(*amend)( void );
	int 		(*amhashvert)( long );
	int 		(*amvertsame)( long, long );
	void		(*amvertdata)( long );
	void		(*amtridata)( int);  /* HG output Triangle data */
	void		(*ambgntmesh)( void );
	void		(*amendtmesh)( void );
	void		(*amswaptmesh)( void );
	void		(*amvert)( long ) ;

	int connectcount;
	int independentcount;

		Vert **verthashlist;
		Tri **trihashlist;
		Edge **edgehashlist;
		Edge *edgearray;
		Edge *freeedges;

		Vertlist *vertlist;
		Trilist *trilist;
		Trilist *newtrilist;
		Trilist *donetrilist;
		Trilist *adjtrilist[4];
		int vertcount;
		Vert *tmpvert;
		int npolys;
		int vertno;
		Tri *curtri;


} Meshobj;


/* local functions */
static Tri *	hashtrifind( Meshobj *, Tri * );
static Vert *	hashvertfind( Meshobj *, Vert * );
static void 	beginoutputobj( Meshobj *, Vert *, Tri * );
static void 	freelists( Meshobj * );
static void 	hashedgebegin( Meshobj *, int );
static void 	hashedgeadd( Meshobj *, Tri *, Vert *, Vert * );
static Edge *	hashedgefind( Meshobj *, Vert *, Vert * );
static void 	hashedgeend( Meshobj * );
static void 	hashtriadd( Meshobj *, Tri * );
static void 	hashtribegin( Meshobj * );
static void 	hashtriend( Meshobj * );
static int		hashvert( Meshobj *, Vert * );
static void 	hashvertadd( Meshobj *, Vert* );
static void 	hashvertbegin( Meshobj * );
static void 	hashvertend( Meshobj * );
static void 	makelists( Meshobj * );
static void 	outmesh( Meshobj *, Trilist * );
static void 	removeadjacencies( Meshobj *, Tri * );

static Tri *	maketri( void ) ;
static Tri *	minadj( Tri * );
static Trilist *maketrilist( void ) ;
static Vert *	makevert( void ) ;
static Vertlist *makevertlist( void );
static int		hashedge( long, long );
static int		hashtri( Tri * );
//static int	ismember( Vert *, Tri * );
//static int	notcommon( Tri *, Tri * );
static int		triequal( Tri *, Tri * ) ;
static void 	deletetri( Trilist *, Tri * );
static void 	freetri( Tri * );
static void 	freetrilist( Trilist * );
static void 	freevert( Vert * );
static void 	freevertlist( Vertlist * );
static void 	inserttri( Trilist *, Tri * );
static void 	insertvert( Vertlist *, Vert * );

inline static int ismember( Vert *vert, Tri *tri )
{
	/*** return TRUE if vert is one of the vertexes in tri, otherwise FALSE ***/
	register int i;

	for (i=0; i<3; i++)
		if (vert == tri->vert[i])
			return TRUE;
	return FALSE;
}

inline static int notcommon( Tri *tri, Tri *tri2 )
{
	/*** returns the index of the vertex in tri that is not in tri2 ***/
	int i;

	for (i=0; i<3; i++)
		if (!ismember(tri->vert[i],tri2))
			return i;
	return -1;
}

Meshobj *
newMeshobj (
	void		(*out_ambegin)( int, int ),
	void		(*out_amend)( void ),
	int 		(*out_amhashvert)( long ),
	int 		(*out_amvertsame)( long, long ),
	void		(*out_amvertdata)( long ),
	void		(*out_amtridata)( int),
	void		(*out_ambgntmesh)( void ),
	void		(*out_amendtmesh)( void ),
	void		(*out_amswaptmesh)( void ),
	void		(*out_amvert)( long ) )
{
	Meshobj *m;

	m					= (Meshobj *) MALLOC( sizeof(Meshobj) );
	m->connectcount 	= 0;
	m->independentcount = 0;
	m->ambegin			= out_ambegin;
	m->amend			= out_amend;
	m->amhashvert		= out_amhashvert;
	m->amvertsame		= out_amvertsame;
	m->amvertdata		= out_amvertdata;
	m->amtridata		= out_amtridata;
	m->ambgntmesh		= out_ambgntmesh;
	m->amendtmesh		= out_amendtmesh;
	m->amswaptmesh		= out_amswaptmesh;
	m->amvert			= out_amvert;
	return m;
}

void
freeMeshobj( Meshobj *m )
{
	FREE( m );
}

/*
 *		Vertex hashing
 *
 */
static void hashvertbegin( Meshobj *m )
{
	int i;

	m->verthashlist = (Vert**)MALLOC(VERTHASHSIZE*sizeof(Vert*));
	for(i=0; i<VERTHASHSIZE; i++)
		m->verthashlist[i] = 0;
}

static int hashvert( Meshobj *m, Vert *vert )
{
	long val;

	val = (*m->amhashvert)(vert->id);
	return val%VERTHASHSIZE;
}

static Vert *hashvertfind( Meshobj *m, Vert *vert )
{
	int pos;
	Vert *vptr;

	pos = hashvert(m,vert);
	for( vptr = m->verthashlist[pos]; vptr; vptr = vptr->hnext )
		if((*m->amvertsame)(vert->id,vptr->id))
			return vptr;
	return 0;
}

static void hashvertadd( Meshobj *m, Vert* vert )
{
	int pos;

	pos = hashvert(m,vert);
	vert->hnext = m->verthashlist[pos];
	m->verthashlist[pos] = vert;
}

static void hashvertend( Meshobj *m )
{
	FREE(m->verthashlist);
}

/*
 *		Triangle hashing
 *
 */
static void hashtribegin( Meshobj *m )
{
	int i;

	m->trihashlist = (Tri**)MALLOC(TRIHASHSIZE*sizeof(Tri*));
	for(i=0; i<TRIHASHSIZE; i++)
		m->trihashlist[i] = 0;
}

static int hashtri( Tri *tri )
{
	long val, l;

	l = (long)tri->vert[0];
	val = l;
	l = (long)tri->vert[1];
	val = val^l;
	l = (long)tri->vert[2];
	val = val^l;
	return val%VERTHASHSIZE;
}

static Tri *hashtrifind( Meshobj *m, Tri *tri )
{
	int pos;
	Tri *tptr;

	pos = hashtri(tri);
	for( tptr = m->trihashlist[pos]; tptr; tptr = tptr->hnext )
		if(triequal(tri,tptr))
			return tptr;
	return 0;
}

static void hashtriadd( Meshobj *m, Tri *tri )
{
	int pos;

	pos = hashtri(tri);
	tri->hnext = m->trihashlist[pos];
	m->trihashlist[pos] = tri;
}

static void hashtriend( Meshobj *m )
{
	FREE(m->trihashlist);
}

/*
 *		Edge hashing
 *
 */
static void hashedgebegin( Meshobj *m, int np )
{
	int i;

	m->edgehashlist = (Edge**)MALLOC(EDGEHASHSIZE*sizeof(Edge*));
	m->edgearray = m->freeedges = (Edge*)MALLOC(3*np*sizeof(Edge));
	for(i=0; i<EDGEHASHSIZE; i++)
		m->edgehashlist[i] = 0;
}

static int hashedge( long index0, long index1 )
{
	long val;

	val = index0*index1;
	val = val&0x7fffffff;
	return val%EDGEHASHSIZE;
}

static Edge *hashedgefind( Meshobj *m, Vert *v0, Vert *v1 )
{
	int pos;
	long index0, index1;
	Edge *tptr;

	index0 = v0->index;
	index1 = v1->index;
	pos = hashedge(index0,index1);
	tptr = m->edgehashlist[pos];
	return tptr;
}

static void hashedgeadd( Meshobj *m, Tri *tri, Vert *v0, Vert *v1 )
{
	int pos;
	long index0, index1;
	Edge *edge;

	index0 = v0->index;
	index1 = v1->index;
	pos = hashedge(index0,index1);
	edge = m->freeedges++;
	edge->index0 = index0;
	edge->index1 = index1;
	edge->tri = tri;
	edge->hnext = m->edgehashlist[pos];
	m->edgehashlist[pos] = edge;
}

static void hashedgeend( Meshobj *m )
{
	FREE(m->edgehashlist);
	FREE(m->edgearray);
}

static Vertlist *makevertlist( void )
{
	/* allocate space for and initialize a vert list */
	Vertlist *tmp;

	if ((tmp=(Vertlist*)MALLOC(sizeof(Vertlist)))==0) {
		TRACE("makevertlist: out of memory.  abort.\n");
		return(tmp);
	//		exit(1);
	}
	tmp->first = tmp->last = 0;
	return tmp;
}

static void freevertlist( Vertlist *vl )
{
	Vert *vert, *nvert;

	for( vert = vl->first; vert; vert = nvert ) {
		nvert = vert->next;
		freevert(vert);
	}
	FREE(vl);
}

static Trilist *maketrilist( void )
{
	/* allocate space for and initialize a tri list */
	Trilist *tmp;

	if ((tmp=(Trilist*)MALLOC(sizeof(Trilist)))==0) {
		TRACE("maketrilist: out of memory.  abort.\n");
		return tmp;
		//exit(1);
	}
	tmp->first = tmp->last = 0;
	return tmp;
}

static void freetrilist( Trilist *tl )
{
	Tri *tri, *ntri;

	for( tri = tl->first; tri; tri = ntri ) {
		ntri = tri->next;
		freetri(tri);
	}
	FREE(tl);
}

static Vert *makevert( void )
{
	/* allocate space for and initialize a vert */
	Vert *tmp;

	if ((tmp=(Vert*)MALLOC(sizeof(Vert)))==0) {
		TRACE("makevert: out of memory.  abort.\n");
		return tmp;
		//exit(1);
	}
	tmp->prev = tmp->next = 0;
	tmp->index = 0;
	return tmp;
}

static void freevert( Vert *v )
{
	FREE(v);
}

/* allocate space for and initialize a tri */

static Tri *maketri( void )
{

	Tri *tmp;

	register int i;
	if ((tmp=(Tri*)MALLOC(sizeof(Tri)))==0) {
		TRACE("maketri: out of memory.  abort.\n");
		return tmp;
	}
	tmp->prev = tmp->next = 0;
    tmp->hnext = 0; // HG 15.03.96
	for (i=0; i<3; i++) {
		tmp->adj[i] = 0;
		tmp->vert[i] = 0;
	}
	tmp->drawvert[0] = 0;
	tmp->drawvert[1] = 1;
	tmp->drawvert[2] = 2;
	tmp->adjcount = 0;
	
    tmp->used = FALSE;
    tmp->data = 0;

	return tmp;
}

static void freetri( Tri *tri )
{
	FREE(tri);
}

static void insertvert( Vertlist *list, Vert *item )
{
	/* insert the new item at the top of the list */
	if (list->first) {
		item->next = list->first;
		item->prev = 0;
		item->next->prev = item;
		list->first = item;
	} else {
		list->first = list->last = item;
		item->prev = item->next = 0;
	}
}

static void inserttri( Trilist *list, Tri *item )
{
	/* insert the new item at the top of the list */
   ASSERT(item);
   ASSERT(list);

	if (list->first) {
		item->next = list->first;
		item->prev = 0;
		item->next->prev = item;
		list->first = item;
	} else {
		list->first = list->last = item;
		item->prev = item->next = 0;
	}
}

static void deletetri( Trilist *list, Tri *item )
{
   ASSERT(item);
   ASSERT(list);

//   TRACE("Delete tri from %p %p \n",list,item);

   /* delete the item from the list */
	if (list->first == item) {
		if (list->last == item) {
			/* this is the only item in the list */
			list->first = list->last = 0;
		} else {
			/* this is the first item in the list */
			list->first = item->next;
			list->first->prev = 0;
		}
	} else if (list->last == item) {
		/* this is the last item in the list */
		list->last = item->prev;
		list->last->next = 0;
	} else {
		item->prev->next = item->next;
		item->next->prev = item->prev;
	}
	item->prev = item->next = 0;
}

static int triequal( Tri *tri1, Tri *tri2 )
{
	int i, j, k;

	k = 0;
	for (i=0; i<3; i++) {
		for (j=0; j<3; j++) {
			if (tri1->vert[i] == tri2->vert[j])
				k += 1;
		}
	}
	if (k == 3)
		return 1;
	else
		return 0;
}

static void makelists( Meshobj *m )
{
	int i;

	m->vertlist = makevertlist();
	m->trilist = maketrilist();
	m->newtrilist = maketrilist();
	m->donetrilist = maketrilist();
	for (i=0; i<4; i++)
		m->adjtrilist[i] = maketrilist();
}

static void freelists( Meshobj *m )
{
	int i;

	freevertlist(m->vertlist);
	freetrilist(m->trilist);
	freetrilist(m->newtrilist);
	freetrilist(m->donetrilist);
	for (i=0; i<4; i++)
		freetrilist(m->adjtrilist[i]);
}

/*
 *		actual exported routines
 *
 *
 */
void in_ambegin( Meshobj *m )
{

	makelists(m);

	m->vertcount = 0;
	hashvertbegin( m );
	m->tmpvert = makevert();
	m->npolys = 0;
	m->vertno = 0;
}


void in_amnewtri( Meshobj *m ,int data )
{
	m->vertno = 0;
	m->curtri = maketri();
	if (m->curtri == NULL) return; //FALSE

	m->curtri->data = data; /* hg */
	inserttri(m->trilist,m->curtri);
	m->npolys++;
}

void in_amvert( Meshobj *m, long fptr )
{
	Vert *vert;

	if(m->vertno > 2) {
		TRACE("in_amvert: can't have more than 3 verts in triangle\n");
		return;
	}
	if (m->curtri == NULL) return; //FALSE

	m->curtri->vert[m->vertno] = 0;
	m->tmpvert->id = fptr;
	vert = hashvertfind(m,m->tmpvert);
	if (vert)
		m->curtri->vert[m->vertno] = vert;
	else  {
		hashvertadd(m,m->tmpvert);
		/* add a new vertex to the list */
		m->tmpvert->index = m->vertcount;
		m->vertcount++;
		m->curtri->vert[m->vertno] = m->tmpvert;
		insertvert(m->vertlist,m->tmpvert);
		m->tmpvert = makevert();
	}
	m->vertno++;
}

void in_amend( Meshobj *m )
{
	Vert *vert0,*vert1;
	register Tri *tri;
	register Tri *tmptri;
	register Tri *nexttri;
	register Edge *edge;
	int i, j, k;
	int degeneratecount;
	int equivalentcount;
	int count;
	int adjcount[4];
	int adjnumber;

	freevert(m->tmpvert);

	if(m->vertno != 3) {
		TRACE("in_amend: incomplete triangle\n");
		// exit(1);
	}
	hashvertend(m);
	TRACE("%d triangles read\n",m->npolys);
	TRACE("%d vertexes created\n",m->vertcount);

	beginoutputobj(m,m->vertlist->first,m->trilist->first);

/*** eliminate degenerate triangles ***/
	TRACE("eliminating degenerate triangles\n");
	degeneratecount = 0;
	for (tri=m->trilist->first; tri;) {
		if ((tri->vert[0] == tri->vert[1]) ||
			(tri->vert[0] == tri->vert[2]) ||
			(tri->vert[1] == tri->vert[2])) {
			degeneratecount += 1;
			tmptri = tri->next;
			deletetri(m->trilist,tri);
			freetri(tri);
			tri = tmptri;
		} else
			tri = tri->next;
	}
	TRACE("%d degenerate triangles eliminated\n",degeneratecount);

		/* Optional ? */
/*** eliminate equivalent triangles ***/
	TRACE("eliminating equivalent triangles\n");
	count = 0;
	equivalentcount = 0;

	hashtribegin(m);
	for (tri=m->trilist->first; tri;) {
		count += 1;
		if(hashtrifind(m,tri)) {
			equivalentcount += 1;
			nexttri = tri->next;
			deletetri(m->trilist,tri);
			freetri(tri);
			tri = nexttri;
		} else {
			hashtriadd(m,tri);
			tri = tri->next;
		}
	}
	hashtriend(m);
	TRACE("%d equivalent triangles eliminated\n",equivalentcount);

/*** compute triangle adjacencies ***/
	TRACE("computing adjacent triangles\n");
	hashedgebegin(m,m->npolys);
	TRACE("adding to hash table . . ");
	for (tri=m->trilist->first; tri; tri=tri->next) {
		for(i=0; i<3; i++) {
			vert0 = tri->vert[(i+0)%3];
			vert1 = tri->vert[(i+1)%3];
			hashedgeadd(m,tri,vert0,vert1);
		}
	}
	TRACE("done\n");
	count = 0;
	for (tri=m->trilist->first; tri; tri=tri->next) {
		count += 1;
		for (i=0; i<3; i++) {
			if (tri->adj[i])
				continue;
			vert0 = tri->vert[i];
			vert1 = tri->vert[(i+1)%3];
			for (edge=hashedgefind(m,vert0,vert1); edge; edge=edge->hnext) {
				nexttri = edge->tri;
				if(nexttri == tri)
					continue;
				for (j=0; j<3; j++) {
					if (vert0 == nexttri->vert[j]) {
						for (k=0; k<3; k++) {
							if (k==j)
								continue;
							if (vert1 == nexttri->vert[k]) {
								switch (j+k) {
									case 1:
										adjnumber = 0;
										break;
									case 2:
										adjnumber = 2;
										break;
									case 3:
										adjnumber = 1;
										break;
									default:
										TRACE("ERROR: bad adjnumber\n");
										break;
								}
								if (tri->adj[i]||nexttri->adj[adjnumber]) {
								} else {
									tri->adj[i] = nexttri;
									nexttri->adj[adjnumber] = tri;
								}
							}
						}
					}
				}
			}
		}
	}
	hashedgeend(m);
	TRACE(" done\n");

/*** test adjacency pointers for consistency ***/
	for (tri=m->trilist->first; tri; tri=tri->next) {
		for (i=0; i<3; i++) {
			if (nexttri = tri->adj[i]) {
				for (j=0,k=0; j<3; j++) {
					if (tri == nexttri->adj[j])
						k += 1;
				}
				if (k != 1) {
					TRACE(" ERROR: %x to %x k = %d\n",tri,nexttri,k);
				}
			}
		}
	}

/*** compute adjacency statistics ***/
	for (i=0; i<4; i++)
		adjcount[i] = 0;
	for (tri=m->trilist->first; tri;) {
		for (i=0,count=0; i<3; i++) {
			if (tri->adj[i])
				count += 1;
		}
		tri->adjcount = count;
		adjcount[count] += 1;
		nexttri = tri->next;
		deletetri(m->trilist,tri);
		inserttri(m->adjtrilist[count],tri);
		tri = nexttri;
	}
	TRACE("adjacencies: 0:%d, 1:%d, 2:%d, 3:%d\n",
		adjcount[0],adjcount[1],adjcount[2],adjcount[3]);

/*** search for connected triangles and output ***/
	while (1) {
	/*** output singular triangles, if any ***/
		while (tri = m->adjtrilist[0]->first) {
			deletetri(m->adjtrilist[0],tri);
			inserttri(m->newtrilist,tri);
			tri->used = TRUE;
			outmesh(m,m->newtrilist);
		}
	/*** choose a seed triangle with the minimum number of adjacencies ***/
		if (tri = m->adjtrilist[1]->first)
			deletetri(m->adjtrilist[1],tri);
		else if (tri = m->adjtrilist[2]->first)
			deletetri(m->adjtrilist[2],tri);
		else if (tri = m->adjtrilist[3]->first)
			deletetri(m->adjtrilist[3],tri);
		else
			break;
		inserttri(m->newtrilist,tri);
		removeadjacencies(m,tri);

	/*** extend in one direction using triangles with min adjacencies ***/
		while (tri = minadj(tri)) {
			deletetri(m->adjtrilist[tri->adjcount],tri);
			inserttri(m->newtrilist,tri);
			removeadjacencies(m,tri);
		}

	/*** if seed has two or more adjacencies, extend in other direction **/
		tri = m->newtrilist->first;
		nexttri = 0;
		for (i=0; i<3; i++) {
			if (tri->adj[i] &&
				(tri->adj[i] != tri->next) &&
				(!(tri->adj[i]->used))) {
				nexttri = tri->adj[i];
				break;
			}
		}
		for( tri = nexttri; tri; tri=minadj(tri) ) {
			deletetri(m->adjtrilist[tri->adjcount],tri);
			inserttri(m->newtrilist,tri);
			removeadjacencies(m,tri);
		}

	/*** output the resulting mesh ***/
		outmesh(m,m->newtrilist);
	}
	if (m->connectcount)
		TRACE("%d triangle mesh%s output\n",
			m->connectcount,m->connectcount==1?"":"es");
	if (m->independentcount)
		TRACE("%d independent triangle%s output\n",
			m->independentcount,m->independentcount==1?"":"s");
	(*m->amend)();
	freelists(m);
}

// HG want input vertex index
// #define OUTFIELD index
#define OUTFIELD id

static void outmesh( Meshobj *m, Trilist *tris )
{
	Tri *tri;
	int i;
	Vert *vert[2];
	Vert *nextvert;
	int replaceB;

Restart:

	/*** output trilist - transfer to donelist ***/
	tri = tris->first;
	if (tri == tris->last) {
		/*** there is only one triangle in the mesh - use polygon command ***/
		m->independentcount += 1;
		(*m->ambgntmesh)();
		(*m->amvert)(tri->vert[0]->OUTFIELD);
		(*m->amvert)(tri->vert[1]->OUTFIELD);
		(*m->amtridata)(tri->data) /* hg */;
		(*m->amvert)(tri->vert[2]->OUTFIELD);
		(*m->amendtmesh)();
		deletetri(tris,tri);
		inserttri(m->donetrilist,tri);
	} else {

		/*** a real mesh output is required ***/
		m->connectcount += 1;

		/*** start output with vertex that is not in the second triangle ***/
		i = notcommon(tri,tri->next);
		INITMESH;
		(*m->ambgntmesh)();
		(*m->amvert)(tri->vert[i]->OUTFIELD);
		REPLACEVERT(tri->vert[i]);
		(*m->amvert)(tri->vert[(i+1)%3]->OUTFIELD);
		REPLACEVERT(tri->vert[(i+1)%3]);

		(*m->amtridata)(tri->data) /* hg */;

		(*m->amvert)(tri->vert[(i+2)%3]->OUTFIELD);
		REPLACEVERT(tri->vert[(i+2)%3]);
		/*** compute vertex of second triangle that is not in the first ***/
		i = notcommon(tri->next,tri);
		nextvert = (tri->next)->vert[i];
		/*** transfer triangle to done list ***/
		deletetri(tris,tri);
		inserttri(m->donetrilist,tri);

		for( tri = tris->first; tri->next; tri=tris->first ) {
			/*** check for errors ***/
			if ((!ismember(vert[0],tri)) || (!ismember(vert[1],tri)) ||
				(!ismember(nextvert,tri))) {
						TRACE("ERROR in mesh generation\n");
			}
			if ((vert[0] == vert[1]) || (vert[0] == nextvert)) {
						TRACE("ERROR in mesh generation\n");
			}
			/*** decide whether to swap or not ***/
			if (ismember(vert[replaceB],tri->next)) {
#if 1
				(*m->amtridata)(tri->data) /* hg */;

				(*m->amvert)(nextvert->OUTFIELD);
				REPLACEVERT(nextvert);
				(*m->amendtmesh)();

				deletetri(tris,tri);
				inserttri(m->donetrilist,tri);

						goto Restart;
#else
						(*m->amswaptmesh)(); // no support on OPENGL
						SWAPMESH;
#endif

			}
			/*** output the next vertex ***/
			(*m->amtridata)(tri->data) /* hg */;
			(*m->amvert)(nextvert->OUTFIELD);
			REPLACEVERT(nextvert);
		
			/*** determine the next output vertex ***/
			i = notcommon(tri->next,tri);
			nextvert = (tri->next)->vert[i];
			/*** transfer tri to the done list ***/
			deletetri(tris,tri);
			inserttri(m->donetrilist,tri);
		}
		
		/*** output the last vertex ***/
		(*m->amtridata)(tri->data) /* hg */;
		(*m->amvert)(nextvert->OUTFIELD);
		REPLACEVERT(nextvert);
		deletetri(tris,tri);
		inserttri(m->donetrilist,tri);
		(*m->amendtmesh)();
	}
}

static void removeadjacencies( Meshobj *m, Tri *tri )
{
	register int i,j;
	Tri *adjtri;

	tri->used = TRUE;
	for (i=0; i<3; i++) {
		adjtri = tri->adj[i];
		if (adjtri) {
			deletetri(m->adjtrilist[adjtri->adjcount],adjtri);
			adjtri->adjcount -= 1;
			for (j=0; j<3; j++) {
				if (tri == adjtri->adj[j]) {
					adjtri->adj[j] = 0;
					// hg 20.03 freetri(adjtri);  why is this beeing removed ????????????????????
               // hg 20.03 adjtri = NULL; // HG 15.03.96
					break;
				}
			}
         if (adjtri) {

			   inserttri(m->adjtrilist[adjtri->adjcount],adjtri);
         } else TRACE("remove adja : Tri was freed \n"); // HG 15.03.96


		}
	}
}

static Tri *minadj( Tri *tri )
{
	int min0,min1;
   ASSERT(tri != 0);
   
   ASSERT(tri->adjcount >= 0 && tri->adjcount<=3);



	switch (tri->adjcount) {
		case 0:
			return 0;
		case 1:
			if (tri->adj[0])
				return tri->adj[0];
			else if (tri->adj[1])
				return tri->adj[1];
			else
				return tri->adj[2];
		case 2:
			if (tri->adj[0]) {
				min0 = 0;
				if (tri->adj[1])
					min1 = 1;
				else
					min1 = 2;
			} else {
				min0 = 1;
				min1 = 2;
			}
			if ((tri->adj[min0])->adjcount <= (tri->adj[min1])->adjcount)
				return tri->adj[min0];
			else
				return tri->adj[min1];
		case 3:
		default:
			if ((tri->adj[0])->adjcount <= (tri->adj[1])->adjcount)
				min0 = 0;
			else
				min0 = 1;
			min1 = 2;
			if ((tri->adj[min0])->adjcount <= (tri->adj[min1])->adjcount)
				return tri->adj[min0];
			else
				return tri->adj[min1];
	}
}

static void beginoutputobj( Meshobj *m, Vert *verts, Tri *tris )
{
	Vert *vert;
	Tri *tri;
	int nverts, ntris;
	int i;

/* count verticies and triangles */
	nverts = 0;
	for( vert = verts; vert; vert = vert->next )
		nverts++;

	ntris = 0;
	for( tri = tris; tri; tri = tri->next )
		ntris++;

	TRACE("makeoutput: vertexes in %d out %d\n",m->npolys*3,nverts);
	TRACE("makeoutput: tris     in %d out %d\n",m->npolys,ntris);
	(*m->ambegin)(nverts,ntris);

	for( i=0, vert = verts; vert; vert = vert->next, i++ ) {
		vert->index = i;
		(*m->amvertdata)(vert->id);
	}
}


static int *tri=NULL;
static int *trip;
static int ntri,trimax;
static int vi=0;
static int nStrips,curStrip,maxStrip,meanStrip;

static int opBeginTristrip = -2;
static int opEndPrim	= -1;


void	tri_ambegin( int nverts, int ntris)
{
  trimax = ntris*2;
  tri = (int *) CALLOC(trimax,sizeof(int));
  trip = tri;
  ntri=0;
  vi =0;
  nStrips=0,maxStrip=0,meanStrip=0;

  TRACE("Mesh  { nverts=%d, ntris = %d \n",nverts,ntris);
}

void	tri_amend( void )
{
		int op=opEndPrim;

		append(tri,ntri,trimax,&op,1);

		TRACE("}\n");

		TRACE("N Strips : %d, Max len = %d, Mean len = %f\n",nStrips,maxStrip,(float) meanStrip / nStrips);
}

int 	tri_amhashvert( long v) { return(v); }
int 	tri_amvertsame( long v1, long v2 ) { return(v1==v2); }

void	tri_amvertdata ( long fptr)
{
//		TRACE("%d Vert %ld \n",vi,fptr);
		vi++;
}

void	tri_ambgntmesh ( void )
{		int op=opBeginTristrip;

//		TRACE("\nTristrip {");
		append(tri,ntri,trimax,&op,1);
		curStrip=0;

}

void	tri_amendtmesh( void )
{
//		TRACE("}\n");
		 if (curStrip > maxStrip) maxStrip = curStrip;

		 nStrips++;
		 meanStrip += curStrip;

}

void	tri_amswaptmesh( void )
{
		TRACE("\nswaptmesh !!!!!!! \n");
}

void	tri_amvert ( long  index)
{
		int op=index;

		append(tri,ntri,trimax,&op,1);

//		TRACE("%d ",index);
		curStrip++;
}

// Data associated with triangle == face number in GShell
void	tri_amtridata ( int  data)
{
		int op=data;

		append(tri,ntri,trimax,&op,1);

//		TRACE(" Data %d ",data);
}

// Compute trianglestrip representation for face list f of length nf
int compute_facelist_tristrips(int nf,const int *f,
				int  *nout, int **out, 
				int opBeginTriStrip_,int opEndPrim_,
				int triangulate)
{
  Meshobj *m;
  const int *fp=f;
  int fi;

  const int *fpend = fp + nf;

  int first=1;

  opBeginTristrip= opBeginTriStrip_;
  opEndPrim = opEndPrim_;


  m = newMeshobj (
	tri_ambegin, tri_amend,
	tri_amhashvert, tri_amvertsame,tri_amvertdata,
	tri_amtridata,
	tri_ambgntmesh,tri_amendtmesh,
	tri_amswaptmesh,tri_amvert);

  in_ambegin(m);

  fi=-1;
  while (fp<fpend) {
	  int n = *fp++;
	  if (n<0) n = -n;
	  else	fi++;				  /* Face index  to do: count holes ? */
	  if (n>2) {
			int p1 = *fp++;
			int p2 = *fp++;
			n-=2;
			while (n>0) {
				int p3 = *fp++;
				n--;

				in_amnewtri(m,fi); //TRACE("IN %d %d  %d (Face %d) \n",p1,p2,p3,fi);
				in_amvert(m, p1);
				in_amvert(m, p2);
				in_amvert(m, p3);
				p2=p3;
		 }
	 } else {
		  fp+=n;
		  TRACE("Compute tristrips: face list contains edges or points \n");
	 }
 }

 in_amend(m);
 freeMeshobj(m);

 *out = tri; tri=NULL;
 *nout = ntri; ntri=0;
 trimax = 0;
 return(0);
}

#ifdef TEST

int main(int argc,char *argv[])

{
 static int cube_connectivity[] = {
		4,0,2,3,1,
		4,1,3,7,5,
		4,5,7,6,4,
		4,6,2,0,4,
		4,2,6,7,3,
		4,0,1,5,4 };

int *out = NULL;
int nout = 0;

 compute_facelist_tristrips(6*5,cube_connectivity,
				&nout,&out,opBeginTristrip,opEndPrim);
 return(0);
}

#endif
