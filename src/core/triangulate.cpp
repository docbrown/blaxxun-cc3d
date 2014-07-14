/*=============================================================================

This code is licensed under the Web3D-blaxxun Community Source License,
provided in distribution file LICENSE.TXT and available online at
http://www.web3D.org/TaskGroups/x3d/blaxxun/Web3D-blaxxunCommunitySourceAgreement.html
and may only be used for non-commercial use as specified in that license.

THE WEB3D CONSORTIUM AND BLAXXUN DO NOT MAKE AND HEREBY DISCLAIM ANY EXPRESS
OR IMPLIED WARRANTIES RELATING TO THIS CODE, INCLUDING BUT NOT LIMITED TO,
WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR
PURPOSE, OR ANY WARRANTIES THAT MIGHT ARISE FROM A COURSE OF DEALING, USAGE
OR TRADE PRACTICE.  THE COMMUNITY SOURCE CODE IS PROVIDED UNDER THIS
AGREEMENT "AS IS," WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COMMUNITY SOURCE CODE ARE
FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR
NON-INFRINGING OR IN ANY WAY CONSTITUTE THE COMPLETE PRODUCT MARKETED UNDER
THE NAMES GIVEN SAID CODE.

==============================================================================*/
/******************************************************************************
@doc

@module Triangulate.cpp - GLView triangulator for Direct3D	|


Purpose:
triangulate a shell, including holes

Classes:

Notes:
Based on:
  http://www.cs.unc.edu/~manocha/CODE/GEM/chapter.html

output adapted to GFaceList format , fixed crash problems with extreme data

originial code has problems with polygon ordering
and static arrays, slow performance using memset

For OpenGL glu triangulator is beeing used


Changes:

Todo :

******************************************************************************/

#include "stdafx.h"
//#include <math.h>
#include <float.h>
#include <time.h>

#include "grender.h"

#include "gshell.h"

//#ifdef _G_USE_TRIANGULATE
#ifndef _OGL

#ifdef DEBUG
#undef DEBUG
#endif


template<class T>
class ArrayDbg : public Array<T>
{
public:
	ArrayDbg() : Array<T>()	{};
#ifdef _DEBUG
	T&		operator[](int i);
#else
	T&		operator[](int i) {
	   	return p[i]; 
	}
#endif


};

#ifdef _DEBUG
void Break() 
{
}

template<class T>
T&		ArrayDbg<T>::operator[](int i)
{		if (i<0 || i>=np) {
			TRACE("Array index out of range %d (%d) \n",i,np);
			Break();
		}
		return p[i]; 
};
#endif



#pragma optimize( "ag", off)
#pragma optimize( "pty", on)

/*
This program is an implementation of a fast polygon
triangulation algorithm based on the paper "A simple and fast
incremental randomized algorithm for computing trapezoidal
decompositions and for triangulating polygons" by Raimund Seidel.


	The algorithm handles simple polygons with holes. The input is
specified as contours. The outermost contour is anti-clockwise, while
all the inner contours must be clockwise. No point should be repeated
in the input. A sample input file 'data_1' is provided.


	The output is a list of triangles. Each triangle gives a pair
(i, j, k) where i, j, and k are indices of the vertices specified in
the input array. (The index numbering starts from 1, since the first
location v[0] in the input array of vertices is unused). The number of
output triangles produced for a polygon with n points is,
	(n - 2) + 2*(#holes)


	The algorithm also generates a qyery structure which can be
used to answer point-location queries very fast.

	int triangulate_polygon(...)
Time for triangulation: O(n log*n)
		
	int is_point_inside_polygon(...)	
Time for query        : O(log n)

	Both the routines are defined in 'tri.c'. See that file for
interfacing details.  If not used stand_alone, include the header file
"interface.h" which contains the declarations for these
functions. Inclusion of "triangulation.h" is not necessary.


	The implementation uses statically allocated arrays. Choose
appropriate value for SEGSIZE / in triangulate.h / depending on
input size.


	There sould not be any compilation problem. If log2() is not
defined in your math library, you will have to supply the definition.

	
USAGE:
	triangulate <filename> // For standalone 


------------------------------------------------------------------
Bibliography:


@article{Sei91,
  AUTHOR = "R. Seidel",
  TITLE = "A simple and Fast Randomized Algorithm for Computing Trapezoidal Decompositions and for Triangulating Polygons",
  JOURNAL = "Computational Geometry Theory \& Applications",
  PAGES = "51-64",
  NUMBER = 1,    
  YEAR = 1991,
  VOLUME = 1 }


@book{o-cgc-94
, author =      "J. O'Rourke"
, title =       "Computational Geometry in {C}"
, publisher =   "Cambridge University Press"
, year =        1994
, note =        "ISBN 0-521-44592-2/Pb \$24.95,
                ISBN 0-521-44034-3/Hc \$49.95.
                Cambridge University Press
                40 West 20th Street
                New York, NY 10011-4211
                1-800-872-7423
                346+xi pages, 228 exercises, 200 figures, 219 references"
, update =      "94.05 orourke, 94.01 orourke"
, annote =      "Textbook"
}



Implementation report: Narkhede A. and Manocha D., Fast polygon
 triangulation algorithm based on Seidel's Algorithm, UNC-CH, 1994.

-------------------------------------------------------------------

This code is in the public domain. Specifically, we give to the public
domain all rights for future licensing of the source code, all resale
rights, and all publishing rights.

UNC-CH GIVES NO WARRANTY, EXPRESSED OR IMPLIED, FOR THE SOFTWARE
AND/OR DOCUMENTATION PROVIDED, INCLUDING, WITHOUT LIMITATION, WARRANTY
OF MERCHANTABILITY AND WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE.


				- Atul Narkhede (narkhede@cs.unc.edu)



*/



double log2(double x) 
{
	return log(x) / log(2.0);
}


#if 0

#define  point_t Point
#define  vector_t Point

#define OUT_INDEX(vnum) (vnum)

#else
	typedef struct {
		double x, y;
		int vindex; // hg , index of original vertex 

	} point_t, vector_t;

#define OUT_INDEX(vnum) (seg[vnum].v0.vindex)

#endif



/* Segment attributes */

typedef struct {	
  point_t v0, v1;		/* two endpoints */
  int is_inserted;		/* inserted in trapezoidation yet ? */
  int root0, root1;		/* root nodes in Q */
  int next;			/* Next logical segment */
  int prev;			/* Previous segment */
} segment_t;


/* Trapezoid attributes */

typedef struct {
  int lseg, rseg;		/* two adjoining segments */
  point_t hi, lo;		/* max/min y-values */
  int u0, u1;
  int d0, d1;
  int sink;			/* pointer to corresponding in Q */
  int usave, uside;		/* I forgot what this means */
  int state;
} trap_t;


/* Node attributes for every node in the query structure */

typedef struct {
  int nodetype;			/* Y-node or S-node */
  int segnum;
  point_t yval;
  int trnum;
  int parent;			/* doubly linked DAG */
  int left, right;		/* children */
} node_t;


typedef struct {
  int vnum;
  int next;			/* Circularly linked list  */
  int prev;			/* describing the monotone */
  int marked;			/* polygon */
} monchain_t;			


typedef struct {
  point_t pt;
  int vnext[4];			/* next vertices for the 4 chains */
  int vpos[4];			/* position of v in the 4 chains */
  int nextfree;
} vertexchain_t;


/* Node types */

#define T_X     1
#define T_Y     2
#define T_SINK  3


#define SEGSIZE 2000	/* max# of segments. Determines how */
				/* many points can be specified as */
				/* input. If your datasets have large */
				/* number of points, increase this */
				/* value accordingly. */

#define QSIZE   8*SEGSIZE	/* maximum table sizes */
#define TRSIZE  4*SEGSIZE	/* max# trapezoids */


#define TRUE  1
#define FALSE 0


#define FIRSTPT 1		/* checking whether pt. is inserted */ 
#define LASTPT  2


#define INFINITY  1<<30
// DBL_MAX

//#define C_EPS 1.0e-7		
#define C_EPS 1.0e-7		

				/* tolerance value: Used for making */
				/* all decisions about collinearity or */
				/* left/right of segment. Decrease */
				/* this value if the input points are */
				/* spaced very close together */


#define S_LEFT 1		/* for merge-direction */
#define S_RIGHT 2


#define ST_VALID 1		/* for trapezium state */
#define ST_INVALID 2


#define SP_SIMPLE_LRUP 1	/* for splitting trapezoids */
#define SP_SIMPLE_LRDN 2
#define SP_2UP_2DN     3
#define SP_2UP_LEFT    4
#define SP_2UP_RIGHT   5
#define SP_2DN_LEFT    6
#define SP_2DN_RIGHT   7
#define SP_NOSPLIT    -1	

#define TR_FROM_UP 1		/* for traverse-direction */
#define TR_FROM_DN 2

#define TRI_LHS 1
#define TRI_RHS 2

#ifndef MAX
	#define MAX(a, b) (((a) > (b)) ? (a) : (b))
	#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#define CROSS(v0, v1, v2) (((v1).x - (v0).x)*((v2).y - (v0).y) - \
			   ((v1).y - (v0).y)*((v2).x - (v0).x))

#define DOT(v0, v1) ((v0).x * (v1).x + (v0).y * (v1).y)

#define FP_EQUAL(s, t) (fabs((s) - (t)) <= C_EPS)



/* Global variables */
//extern node_t qs[QSIZE];		/* Query structure */
//extern trap_t tr[TRSIZE];		/* Trapezoid structure */
//extern segment_t seg[SEGSIZE];		/* Segment table */



/* Functions */
/*
extern int monotonate_trapezoids(int);

//extern int triangulate_monotone_polygons(int, int, int (*)[3]);


extern int _greater_than(point_t *, point_t *);
extern int _equal_to(point_t *, point_t *);
extern int _greater_than_equal_to(point_t *, point_t *);
extern int _less_than(point_t *, point_t *);
extern int locate_endpoint(point_t *, point_t *, int);
extern int construct_trapezoids(int);

extern int generate_random_ordering(int);
extern int choose_segment(void);
extern int read_segments(char *, int *);
extern int math_logstar_n(int);
extern int math_N(int, int);
*/






#if 1
ArrayDbg<node_t> qs;
ArrayDbg<trap_t> tr;
ArrayDbg<segment_t> seg;


ArrayDbg<monchain_t> mchain;
ArrayDbg<vertexchain_t> vert;
ArrayDbg<int> mon;
ArrayDbg<int> visited;
ArrayDbg<int> permute;

#else
static node_t qs[QSIZE];		/* Query structure */
static trap_t tr[TRSIZE];		/* Trapezoid structure */
static segment_t seg[SEGSIZE];		/* Segment table */

// monotone 
static monchain_t mchain[TRSIZE]; /* Table to hold all the monotone */
				  /* polygons . Each monotone polygon */
				  /* is a circularly linked list */

static vertexchain_t vert[SEGSIZE]; /* chain init. information. This */
				    /* is used to decide which */
				    /* monotone polygon to split if */
				    /* there are several other */
				    /* polygons touching at the same */
				    /* vertex  */

static int mon[SEGSIZE];	/* contains position of any vertex in */
				/* the monotone chain for the polygon */
static int visited[TRSIZE];
static int permute[SEGSIZE];


#endif

static int q_idx;
static int tr_idx;
static int seg_idx; // HG 

static BOOL mustFlip; // must flip output triangle 


static int chain_idx, op_idx, mon_idx;

static int choose_idx;

void resetTables() 
{
	qs.SetLength(1);
	tr.SetLength(1);
	seg.SetLength(1);
	mchain.SetLength(0);
	vert.SetLength(0);
	mon.SetLength(0);
	visited.SetLength(0);
	permute.SetLength(0);
}


/* Return a new segment */
static int newseg()
{
  seg_idx++;

  int idx = seg.Length();
  seg.SetLength(idx+1);
  return idx;
}


/* Return a new node to be added into the query tree */
static int newnode()
{
/* zero out  ?? 
  int nodetype;			
  int segnum;
  point_t yval;
  int trnum;
  int parent;			
  int left, right;		
*/ 
  q_idx++;

  int idx = qs.Length();
  qs.SetLength(idx+1);
  memset((void *)&qs[idx], 0, sizeof(node_t)); // hg 
  return idx;



#if 0
  if (q_idx < QSIZE) {
	memset((void *)&qs[q_idx], 0, sizeof(qs[0])); // hg 
    return q_idx++;
  }
  else
    {
      TRACE("newnode: Query-table overflow\n");
      return -1;
    }
#endif
}

/* Return a free trapezoid */
static int newtrap()
{
  tr_idx++;

  int idx = tr.Length();
  tr.SetLength(idx+1);
  memset((void *)&tr[idx], 0, sizeof(trap_t)); // hg 
  tr[idx].lseg = -1;
  tr[idx].rseg = -1;
  tr[idx].state = ST_VALID;

  return idx;

#if 0

	if (tr_idx < TRSIZE)
    {
	  memset((void *)&tr[tr_idx], 0, sizeof(tr[0])); // hg 

      tr[tr_idx].lseg = -1;
      tr[tr_idx].rseg = -1;
      tr[tr_idx].state = ST_VALID;
      return tr_idx++;
    }
  else
    {
      TRACE("newtrap: Trapezoid-table overflow\n");
      return -1;
    }
#endif
}


/* return a new mon structure from the table */
static int newmon()
{
  mon_idx++;

  int idx = mon.Length();
  mon.SetLength(idx+1);
  mon[idx]=0; // hg 07.07.98

  return idx;

  //return ++mon_idx;
}


/* return a new chain element from the table */
static int new_chain_element()
{
  chain_idx++;

  int i = mchain.Length();
  mchain.SetLength(i+1);
  mchain[i].marked = 0; // hg 
  mchain[i].vnum = -1; // hg 
  mchain[i].next = -1; // hg 
  mchain[i].prev = -1; // hg 

  return i;

#if 0
  int i = ++chain_idx;
  if (i>= TRSIZE) {
	  TRACE("Triangulate, mchain table overflow \n");
	  return -1;

  }
  mchain[i].marked = 0; // hg 
  return(i);
#endif

}



/* Return the maximum of the two points into the yval structure */
static int _max(point_t *yval, point_t *v0, point_t *v1)
{
  if (v0->y > v1->y + C_EPS)
    *yval = *v0;
  else if (FP_EQUAL(v0->y, v1->y))
    {
      if (v0->x > v1->x + C_EPS)
	*yval = *v0;
      else
	*yval = *v1;
    }
  else
    *yval = *v1;
  
  return 0;
}


/* Return the minimum of the two points into the yval structure */
static int _min( point_t *yval,  point_t *v0,  point_t *v1)
{
  if (v0->y < v1->y - C_EPS)
    *yval = *v0;
  else if (FP_EQUAL(v0->y, v1->y))
    {
      if (v0->x < v1->x)
		*yval = *v0;
      else
		*yval = *v1;
    }
  else
    *yval = *v1;
  
  return 0;
}


static int _greater_than( point_t *v0, point_t *v1)
{
  if (v0->y > v1->y + C_EPS)
    return TRUE;
  else if (v0->y < v1->y - C_EPS)
    return FALSE;
  else
    return (v0->x > v1->x);
}


static int _equal_to(point_t *v0,  point_t *v1)
{
  return (FP_EQUAL(v0->y, v1->y) && FP_EQUAL(v0->x, v1->x));
}

static int _greater_than_equal_to( point_t *v0, point_t *v1)
{
  if (v0->y > v1->y + C_EPS)
    return TRUE;
  else if (v0->y < v1->y - C_EPS)
    return FALSE;
  else
    return (v0->x >= v1->x);
}

static int _less_than( point_t *v0,  point_t * v1)
{
  if (v0->y < v1->y - C_EPS)
    return TRUE;
  else if (v0->y > v1->y + C_EPS)
    return FALSE;
  else
    return (v0->x < v1->x);
}



// misc.c

/* Generate a random permutation of the segments 1..n */
static int generate_random_ordering(int n)
{
  register int i;
  int m, *p;

  //int st[SEGSIZE]; 

  ArrayDbg<int> st;

  st.SetLength(n+1);
  permute.SetLength(n+1);

  
#ifndef WIN32
  struct timeval tval;
  struct timezone tzone;

  gettimeofday(&tval, &tzone);
  srand48(tval.tv_sec);
#else
  
  srand(clock());
#endif


  choose_idx = 1;

  for (i = 0; i <= n; i++)
    st[i] = i;

  p = st;
  for (i = 1; i <= n; i++, p++)
    {
#ifndef WIN32
      m = lrand48() % (n + 1 - i) + 1;
#else
      m = rand() % (n + 1 - i) + 1;
#endif
      permute[i] = p[m];
      if (m != 1)
		p[m] = p[1];
    }
  return 0;
}

  
/* Return the next segment in the generated random ordering of all the */
/* segments in S */
static int choose_segment()
{
  //int i;

#ifdef DEBUG
  fprintf(stderr, "choose_segment: %d\n", permute[choose_idx]);
#endif 
  return permute[choose_idx++];
}


#ifdef STANDALONE

/* Read in the list of vertices from infile */
int read_segments(filename, genus)
     char *filename;
     int *genus;
{
  FILE *infile;
  int ccount;
  register int i;
  int ncontours, npoints, first, last;

  if ((infile = fopen(filename, "r")) == NULL)
    {
      perror(filename);
      return -1;
    }

  fscanf(infile, "%d", &ncontours);
  if (ncontours <= 0)
    return -1;
  
  /* For every contour, read in all the points for the contour. The */
  /* outer-most contour is read in first (points specified in */
  /* anti-clockwise order). Next, the inner contours are input in */
  /* clockwise order */

  ccount = 0;
  i = 1;
  
  while (ccount < ncontours)
    {
      int j;

      fscanf(infile, "%d", &npoints);
      first = i;
      last = first + npoints - 1;
      for (j = 0; j < npoints; j++, i++)
	{
	  fscanf(infile, "%lf%lf", &seg[i].v0.x, &seg[i].v0.y);
	  if (i == last)
	    {
	      seg[i].next = first;
	      seg[i].prev = i-1;
	      seg[i-1].v1 = seg[i].v0;
	    }
	  else if (i == first)
	    {
	      seg[i].next = i+1;
	      seg[i].prev = last;
	      seg[last].v1 = seg[i].v0;
	    }
	  else
	    {
	      seg[i].prev = i-1;
	      seg[i].next = i+1;
	      seg[i-1].v1 = seg[i].v0;
	    }
	  
	  seg[i].is_inserted = FALSE;
	}

      ccount++;
    }

  *genus = ncontours - 1;
  return i-1;
}

#endif


/* Get log*n for given n */
static int math_logstar_n(int n)
{
  register int i;
  double v;
  
  for (i = 0, v = (double) n; v >= 1; i++)
    v = log2(v);
  
  return (i - 1);
}
  

static int math_N(int n, int h)
{
  register int i;
  double v;

  for (i = 0, v = (int) n; i < h; i++)
    v = log2(v);
  
  return (int) ceil((double) 1.0*n/v);
}


// monotone.c
#define CROSS_SINE(v0, v1) ((v0).x * (v1).y - (v1).x * (v0).y)
#define LENGTH(v0) (sqrt((v0).x * (v0).x + (v0).y * (v0).y))



//static int triangulate_single_polygon(int, int, int, int (*)[3]);
static int triangulate_single_polygon(int nvert, int posmax, int side, GFaceList &op);

static int traverse_polygon(int, int, int, int);

/* Function returns TRUE if the trapezoid lies inside the polygon */
static int inside_polygon(trap_t *t)
{
  int rseg = t->rseg;

  if (t->state == ST_INVALID)
    return 0;

  if ((t->lseg <= 0) || (t->rseg <= 0))
    return 0;
  
  if (((t->u0 <= 0) && (t->u1 <= 0)) || 
      ((t->d0 <= 0) && (t->d1 <= 0))) /* triangle */
    return (_greater_than(&seg[rseg].v1, &seg[rseg].v0));
  
  return 0;
}




static double get_angle(point_t *vp0, point_t *vpnext,  point_t *vp1)
{
  point_t v0, v1;
  
  v0.x = vpnext->x - vp0->x;
  v0.y = vpnext->y - vp0->y;

  v1.x = vp1->x - vp0->x;
  v1.y = vp1->y - vp0->y;

  if (CROSS_SINE(v0, v1) >= 0)	/* sine is positive */
    return DOT(v0, v1)/LENGTH(v0)/LENGTH(v1);
  else
    return (-1.0 * DOT(v0, v1)/LENGTH(v0)/LENGTH(v1) - 2);
}


/* (v0, v1) is the new diagonal to be added to the polygon. Find which */
/* chain to use and return the positions of v0 and v1 in p and q */ 
static int get_vertex_positions(int v0, int v1, int *ip, int *iq)
{
  vertexchain_t *vp0, *vp1;
  register int i;
  double angle, temp;
  int tp=-1, tq=-1; // HG -1

  vp0 = &vert[v0];
  vp1 = &vert[v1];
  
  /* p is identified as follows. Scan from (v0, v1) rightwards till */
  /* you hit the first segment starting from v0. That chain is the */
  /* chain of our interest */
  
  angle = -4.0;
  for (i = 0; i < 4; i++)
    {
      if ((vp0->vnext[i] <= 0) || (vp0->vnext[i] >= vert.Length())) 
		continue;
      if ((temp = get_angle(&vp0->pt, &(vert[vp0->vnext[i]].pt), 
			    &vp1->pt)) > angle)
	{
	  angle = temp;
	  tp = i;
	}
    }

  *ip = tp;

  /* Do similar actions for q */

  angle = -4.0;
  for (i = 0; i < 4; i++)
    {
      if ((vp1->vnext[i] <= 0) || (vp1->vnext[i] >= vert.Length()))
		continue;      
      if ((temp = get_angle(&vp1->pt, &(vert[vp1->vnext[i]].pt), 
			    &vp0->pt)) > angle)
	{
	  angle = temp;
	  tq = i;
	}
    }

  *iq = tq;

  return 0;
}

  
/* v0 and v1 are specified in anti-clockwise order with respect to 
 * the current monotone polygon mcur. Split the current polygon into 
 * two polygons using the diagonal (v0, v1) 
 */
static int make_new_monotone_poly(int mcur, int v0, int v1)
{
  
  if (v0 <=0 || v1 <= 0) { // HG 
	  return -1;
  }		  
  int p, q, ip, iq;
  // int mnew = 0;  newmon();
  int i, j, nf0, nf1;
  vertexchain_t *vp0, *vp1;
  
  vp0 = &vert[v0];
  vp1 = &vert[v1];

  get_vertex_positions(v0, v1, &ip, &iq);

  if (ip<0 || iq <0)  { // hg 
	  TRACE("triangulate:failed to get vertex positions \n");
	  return -1;
  }

  p = vp0->vpos[ip];
  q = vp1->vpos[iq];

  /* At this stage, we have got the positions of v0 and v1 in the */
  /* desired chain. Now modify the linked lists */

  i = new_chain_element();	/* for the new list */
  j = new_chain_element();

  if (i<0 || j<0) return (-1); // hg a problem with table 

  mchain[i].vnum = v0;
  mchain[i].marked = 0; // HG
  mchain[j].vnum = v1;
  mchain[j].marked = 0; // HG

  mchain[i].next = mchain[p].next;
  mchain[mchain[p].next].prev = i;
  mchain[i].prev = j;
  mchain[j].next = i;
  mchain[j].prev = mchain[q].prev;
  mchain[mchain[q].prev].next = j;

  mchain[p].next = q;
  mchain[q].prev = p;

  nf0 = vp0->nextfree;
  nf1 = vp1->nextfree;

  vp0->vnext[ip] = v1;

  vp0->vpos[nf0] = i;
  vp0->vnext[nf0] = mchain[mchain[i].next].vnum;
  vp1->vpos[nf1] = j;
  vp1->vnext[nf1] = v0;

  vp0->nextfree++;
  vp1->nextfree++;

#ifdef DEBUG
  fprintf(stderr, "make_poly: mcur = %d, (v0, v1) = (%d, %d)\n", 
	  mcur, v0, v1);
  fprintf(stderr, "next posns = (p, q) = (%d, %d)\n", p, q);
#endif

  int mnew=newmon();

  mon[mcur] = p;
  mon[mnew] = i;
  return mnew;
}

/* Main routine to get monotone polygons from the trapezoidation of 
 * the polygon.
 */

static int monotonate_trapezoids(int n)
{
  register int i;
  int tr_start= -1;

#if 0

  memset((void *)vert, 0, sizeof(vert)); //xx must be zeroed only to length of seg ? 

  memset((void *)visited, 0, sizeof(visited));
  memset((void *)mchain, 0, sizeof(mchain));
  memset((void *)mon, 0, sizeof(mon));

#endif 

  
  /* First locate a trapezoid which lies inside the polygon */
  /* and which is triangular */
 

  //for (i = 0; i < TRSIZE; i++)   // hg bad loop, what if no trapezoid found ??????????????
  for (i = 1; i < tr_idx; i++)  // are allocated from 1 ??
  {
    if (inside_polygon(&tr[i])) {
	  tr_start = i;
      break;
    }
  }

  if (tr_start == -1)  // no poly found 
	  return -1;


  
  /* Initialise the mon data-structure and start spanning all the */
  /* trapezoids within the polygon */

#if 0
  for (i = 1; i <= n; i++)
    {
	  visited[i]=0; // HG
      mchain[i].prev = i - 1;
      mchain[i].next = i + 1;
      mchain[i].vnum = i;
      mchain[i].marked = 0; // hg 
      vert[i].pt = seg[i].v0;		    // hg but length is SEGSIZE	  ??? 
      vert[i].vnext[0] = i + 1;	/* next vertex */
      vert[i].vpos[0] = i;	/* locn. of next vertex */
      vert[i].nextfree = 1;
	  for (int j= 1; j<4;j++) { // hg 
		 vert[i].vnext[j]= vert[i].vpos[j] = 0;
	  }
    }
  mchain[1].prev = n;
  mchain[n].next = 1;
  vert[n].vnext[0] = 1;
  vert[n].vpos[0] = n;
  chain_idx = n;
  mon_idx = 0;
  mon[0] = 1;			/* position of any vertex in the first */
				/* chain  */

#else

  mchain.SetLength(n+1);
  vert.SetLength(n+1);
  
  visited.SetLength(tr_idx+1);
  for (i = 0/*1*/; i <= tr_idx; i++)  visited[i]=0; // HG


  for (i = 1; i <= n; i++)
    {
      mchain[i].prev = seg[i].prev;
      mchain[i].next = seg[i].next;
      mchain[i].vnum = i;
      mchain[i].marked = 0; // hg 
      vert[i].pt = seg[i].v0;
      vert[i].vnext[0] = seg[i].next; /* next vertex */
      vert[i].vpos[0] = i;	/* locn. of next vertex */
      vert[i].nextfree = 1;

	  for (int j= 1; j<4;j++) {  
		 vert[i].vnext[j]= vert[i].vpos[j] = -1;
	  }
    }

  chain_idx = n;
  mon_idx = 0;
  mon.SetLength(1);
  mon[0] = 1;			/* position of any vertex in the first */
				/* chain  */

#endif
  
  /* traverse the polygon */
  if (tr[tr_start].u0 > 0)
    traverse_polygon(0, tr_start, tr[tr_start].u0, TR_FROM_UP);
  else if (tr[tr_start].d0 > 0)
    traverse_polygon(0, tr_start, tr[tr_start].d0, TR_FROM_DN);
  
  /* return the number of polygons created */
  return newmon();
}


/* recursively visit all the trapezoids */
static int traverse_polygon(int mcur, int trnum, int from, int dir)
{
  
  if (mcur < 0) { 
	  TRACE("triangulate:traverse_polygon bad polygon \n");
	  if (trnum>0)  visited[trnum] = TRUE;
	  return -1; // hg 
  }
  
  if ((trnum <= 0)) { //HG 
	    TRACE("triangulate:traverse_polygon bad polygon \n");
		return -1;
  }	

  trap_t *t = &tr[trnum];
  int howsplit, mnew;
  int v0, v1, v0next, v1next;
  int retval, tmp;
  int do_switch = FALSE;

  if ((trnum <= 0) || visited[trnum])
    return 0;

  v0 = v1 = -1;

  visited[trnum] = TRUE;
  
  /* We have much more information available here. */
  /* rseg: goes upwards   */
  /* lseg: goes downwards */

  /* Initially assume that dir = TR_FROM_DN (from the left) */
  /* Switch v0 and v1 if necessary afterwards */


  // hg problem : lseg u1 = -1 in processing polygons with holes (big O)

  if (t->lseg <=0 && t->rseg<=0) {
	  retval = SP_NOSPLIT;	/* Just traverse all neighbours */
	  traverse_polygon(mcur, t->u0, trnum, TR_FROM_DN);
	  traverse_polygon(mcur, t->u1, trnum, TR_FROM_DN);
	  traverse_polygon(mcur, t->d0, trnum, TR_FROM_UP);
	  traverse_polygon(mcur, t->d1, trnum, TR_FROM_UP);

	  return -1;	// HG
  }


  /* special cases for triangles with cusps at the opposite ends. */
  /* take care of this first */
  if ((t->u0 <= 0) && (t->u1 <= 0))
    {
      if ((t->d0 > 0) && (t->d1 > 0)) /* downward opening triangle */
	{
	  v0 = tr[t->d1].lseg;
	  v1 = t->lseg;
	  if (from == t->d1)
	    {
	      do_switch = TRUE;
	      mnew = make_new_monotone_poly(mcur, v1, v0);
	      traverse_polygon(mcur, t->d1, trnum, TR_FROM_UP);
	      traverse_polygon(mnew, t->d0, trnum, TR_FROM_UP);	    
	    }
	  else
	    {
	      mnew = make_new_monotone_poly(mcur, v0, v1);
	      traverse_polygon(mcur, t->d0, trnum, TR_FROM_UP);
	      traverse_polygon(mnew, t->d1, trnum, TR_FROM_UP);
	    }
	}
      else
	{
	  retval = SP_NOSPLIT;	/* Just traverse all neighbours */
	  traverse_polygon(mcur, t->u0, trnum, TR_FROM_DN);
	  traverse_polygon(mcur, t->u1, trnum, TR_FROM_DN);
	  traverse_polygon(mcur, t->d0, trnum, TR_FROM_UP);
	  traverse_polygon(mcur, t->d1, trnum, TR_FROM_UP);
	}
    }
  
  else if ((t->d0 <= 0) && (t->d1 <= 0))
    {
      if ((t->u0 > 0) && (t->u1 > 0)) /* upward opening triangle */
	{
	  v0 = t->rseg;
	  v1 = tr[t->u0].rseg;
	  if (from == t->u1)
	    {
	      do_switch = TRUE;
	      mnew = make_new_monotone_poly(mcur, v1, v0);
	      traverse_polygon(mcur, t->u1, trnum, TR_FROM_DN);
	      traverse_polygon(mnew, t->u0, trnum, TR_FROM_DN);	    
	    }
	  else
	    {
	      mnew = make_new_monotone_poly(mcur, v0, v1);
	      traverse_polygon(mcur, t->u0, trnum, TR_FROM_DN);
	      traverse_polygon(mnew, t->u1, trnum, TR_FROM_DN);
	    }
	}
      else
	{
	  retval = SP_NOSPLIT;	/* Just traverse all neighbours */
	  traverse_polygon(mcur, t->u0, trnum, TR_FROM_DN);
	  traverse_polygon(mcur, t->u1, trnum, TR_FROM_DN);
	  traverse_polygon(mcur, t->d0, trnum, TR_FROM_UP);
	  traverse_polygon(mcur, t->d1, trnum, TR_FROM_UP);
	}
    }
  
  else if ((t->u0 > 0) && (t->u1 > 0)) 
    {
      if ((t->d0 > 0) && (t->d1 > 0)) /* downward + upward cusps */
	{
	  v0 = tr[t->d1].lseg;
	  v1 = tr[t->u0].rseg;
	  retval = SP_2UP_2DN;
	  if (((dir == TR_FROM_DN) && (t->d1 == from)) ||
	      ((dir == TR_FROM_UP) && (t->u1 == from)))
	    {
	      do_switch = TRUE;
	      mnew = make_new_monotone_poly(mcur, v1, v0);
	      traverse_polygon(mcur, t->u1, trnum, TR_FROM_DN);
	      traverse_polygon(mcur, t->d1, trnum, TR_FROM_UP);
	      traverse_polygon(mnew, t->u0, trnum, TR_FROM_DN);
	      traverse_polygon(mnew, t->d0, trnum, TR_FROM_UP);
	    }
	  else
	    {
	      mnew = make_new_monotone_poly(mcur, v0, v1);
	      traverse_polygon(mcur, t->u0, trnum, TR_FROM_DN);
	      traverse_polygon(mcur, t->d0, trnum, TR_FROM_UP);
	      traverse_polygon(mnew, t->u1, trnum, TR_FROM_DN);
	      traverse_polygon(mnew, t->d1, trnum, TR_FROM_UP);	      
	    }
	}
      else			/* only downward cusp */
	{
	  if (t->lseg>0 && // HG 
		  _equal_to(&t->lo, &seg[t->lseg].v1))
	    {
	      v0 = tr[t->u0].rseg;
	      v1 = seg[t->lseg].next;

	      retval = SP_2UP_LEFT;
	      if ((dir == TR_FROM_UP) && (t->u0 == from))
		{
		  do_switch = TRUE;
		  mnew = make_new_monotone_poly(mcur, v1, v0);
		  traverse_polygon(mcur, t->u0, trnum, TR_FROM_DN);
		  traverse_polygon(mnew, t->d0, trnum, TR_FROM_UP);
		  traverse_polygon(mnew, t->u1, trnum, TR_FROM_DN);
		  traverse_polygon(mnew, t->d1, trnum, TR_FROM_UP);
		}
	      else
		{
		  mnew = make_new_monotone_poly(mcur, v0, v1);
		  traverse_polygon(mcur, t->u1, trnum, TR_FROM_DN);
		  traverse_polygon(mcur, t->d0, trnum, TR_FROM_UP);
		  traverse_polygon(mcur, t->d1, trnum, TR_FROM_UP);
		  traverse_polygon(mnew, t->u0, trnum, TR_FROM_DN);
		}
	    }
	  else
	    {
	      v0 = t->rseg;
	      v1 = tr[t->u0].rseg;	
	      retval = SP_2UP_RIGHT;
	      if ((dir == TR_FROM_UP) && (t->u1 == from))
		{
		  do_switch = TRUE;
		  mnew = make_new_monotone_poly(mcur, v1, v0);
		  traverse_polygon(mcur, t->u1, trnum, TR_FROM_DN);
		  traverse_polygon(mnew, t->d1, trnum, TR_FROM_UP);
		  traverse_polygon(mnew, t->d0, trnum, TR_FROM_UP);
		  traverse_polygon(mnew, t->u0, trnum, TR_FROM_DN);
		}
	      else
		{
		  mnew = make_new_monotone_poly(mcur, v0, v1);
		  traverse_polygon(mcur, t->u0, trnum, TR_FROM_DN);
		  traverse_polygon(mcur, t->d0, trnum, TR_FROM_UP);
		  traverse_polygon(mcur, t->d1, trnum, TR_FROM_UP);
		  traverse_polygon(mnew, t->u1, trnum, TR_FROM_DN);
		}
	    }
	}
    }
  else if ((t->u0 > 0) || (t->u1 > 0)) /* no downward cusp */
    {
      if ((t->d0 > 0) && (t->d1 > 0)) /* only upward cusp */
	{
	  if ( t->lseg>0 && // HG 
		  _equal_to(&t->hi, &seg[t->lseg].v0))
	    {
	      v0 = tr[t->d1].lseg;
	      v1 = t->lseg;
	      retval = SP_2DN_LEFT;
	      if (!((dir == TR_FROM_DN) && (t->d0 == from)))
		{
		  do_switch = TRUE;
		  mnew = make_new_monotone_poly(mcur, v1, v0);
		  traverse_polygon(mcur, t->u1, trnum, TR_FROM_DN);
		  traverse_polygon(mcur, t->d1, trnum, TR_FROM_UP);
		  traverse_polygon(mcur, t->u0, trnum, TR_FROM_DN);
		  traverse_polygon(mnew, t->d0, trnum, TR_FROM_UP);
		}
	      else
		{
		  mnew = make_new_monotone_poly(mcur, v0, v1);
		  traverse_polygon(mcur, t->d0, trnum, TR_FROM_UP);
		  traverse_polygon(mnew, t->u0, trnum, TR_FROM_DN);
		  traverse_polygon(mnew, t->u1, trnum, TR_FROM_DN);
		  traverse_polygon(mnew, t->d1, trnum, TR_FROM_UP);	      
		}
	    }
	  else
	    {
	      v0 = tr[t->d1].lseg;
		  if (t->rseg <=0) goto NOSPLIT; // HG 

	      v1 = seg[t->rseg].next;

	      retval = SP_2DN_RIGHT;	    
	      if ((dir == TR_FROM_DN) && (t->d1 == from))
		{
		  do_switch = TRUE;
		  mnew = make_new_monotone_poly(mcur, v1, v0);
		  traverse_polygon(mcur, t->d1, trnum, TR_FROM_UP);
		  traverse_polygon(mnew, t->u1, trnum, TR_FROM_DN);
		  traverse_polygon(mnew, t->u0, trnum, TR_FROM_DN);
		  traverse_polygon(mnew, t->d0, trnum, TR_FROM_UP);
		}
	      else
		{
		  mnew = make_new_monotone_poly(mcur, v0, v1);
		  traverse_polygon(mcur, t->u0, trnum, TR_FROM_DN);
		  traverse_polygon(mcur, t->d0, trnum, TR_FROM_UP);
		  traverse_polygon(mcur, t->u1, trnum, TR_FROM_DN);
		  traverse_polygon(mnew, t->d1, trnum, TR_FROM_UP);
		}
	    }
	}
      else			/* no cusp */
	{ //xxx
	  if ( t->lseg >0 && t->rseg >0 &&  //HG
		  _equal_to(&t->hi, &seg[t->lseg].v0) &&
	      _equal_to(&t->lo, &seg[t->rseg].v0))
	    {
	      v0 = t->rseg;
	      v1 = t->lseg;
	      retval = SP_SIMPLE_LRDN;
	      if (dir == TR_FROM_UP)
		{
		  do_switch = TRUE;
		  mnew = make_new_monotone_poly(mcur, v1, v0);
		  traverse_polygon(mcur, t->u0, trnum, TR_FROM_DN);
		  traverse_polygon(mcur, t->u1, trnum, TR_FROM_DN);
		  traverse_polygon(mnew, t->d1, trnum, TR_FROM_UP);
		  traverse_polygon(mnew, t->d0, trnum, TR_FROM_UP);
		}
	      else
		{
		  mnew = make_new_monotone_poly(mcur, v0, v1);
		  traverse_polygon(mcur, t->d1, trnum, TR_FROM_UP);
		  traverse_polygon(mcur, t->d0, trnum, TR_FROM_UP);
		  traverse_polygon(mnew, t->u0, trnum, TR_FROM_DN);
		  traverse_polygon(mnew, t->u1, trnum, TR_FROM_DN);
		}
	    }
	  else if (t->lseg >0 && t->rseg >0 &&  //HG
		  _equal_to(&t->hi, &seg[t->rseg].v1) &&
		   _equal_to(&t->lo, &seg[t->lseg].v1))
	    {
	      v0 = seg[t->rseg].next;
	      v1 = seg[t->lseg].next;

	      retval = SP_SIMPLE_LRUP;
	      if (dir == TR_FROM_UP)
		{
		  do_switch = TRUE;
		  mnew = make_new_monotone_poly(mcur, v1, v0);
		  traverse_polygon(mcur, t->u0, trnum, TR_FROM_DN);
		  traverse_polygon(mcur, t->u1, trnum, TR_FROM_DN);
		  traverse_polygon(mnew, t->d1, trnum, TR_FROM_UP);
		  traverse_polygon(mnew, t->d0, trnum, TR_FROM_UP);
		}
	      else
		{
		  mnew = make_new_monotone_poly(mcur, v0, v1);
		  traverse_polygon(mcur, t->d1, trnum, TR_FROM_UP);
		  traverse_polygon(mcur, t->d0, trnum, TR_FROM_UP);
		  traverse_polygon(mnew, t->u0, trnum, TR_FROM_DN);
		  traverse_polygon(mnew, t->u1, trnum, TR_FROM_DN);
		}
	    }
	  else			/* no split possible */
	    {
NOSPLIT:
	      retval = SP_NOSPLIT;
	      traverse_polygon(mcur, t->u0, trnum, TR_FROM_DN);
	      traverse_polygon(mcur, t->d0, trnum, TR_FROM_UP);
	      traverse_polygon(mcur, t->u1, trnum, TR_FROM_DN);
	      traverse_polygon(mcur, t->d1, trnum, TR_FROM_UP);	      	      
	    }
	}
    }

  return retval;
}


/* For each monotone polygon, find the ymax and ymin (to determine the */
/* two y-monotone chains) and pass on this monotone polygon for greedy */
/* triangulation. */
/* Take care not to triangulate duplicate monotone polygons */

int triangulate_monotone_polygons(int nvert, int nmonpoly, GFaceList &op)
{
  register int i;
  point_t ymax, ymin;
  int p, vfirst, posmax, posmin, v;
  int vcount, processed;

#ifdef DEBUG
  for (i = 0; i < nmonpoly; i++)
    {
      fprintf(stderr, "\n\nPolygon %d: ", i);
      vfirst = mchain[mon[i]].vnum;
      p = mchain[mon[i]].next;
      fprintf (stderr, "%d ", mchain[mon[i]].vnum);
      while (mchain[p].vnum != vfirst)
	{
	  fprintf(stderr, "%d ", mchain[p].vnum);
	  p = mchain[p].next;
	}
    }
  fprintf(stderr, "\n");
#endif

  op_idx = 0;
  for (i = 0; i < nmonpoly; i++)
    {
      vcount = 1;
      processed = FALSE;
	  if (mon[i]<=0) continue; // HG 
      vfirst = mchain[mon[i]].vnum;
      ymax = ymin = vert[vfirst].pt;
      posmax = posmin = mon[i];
      mchain[mon[i]].marked = TRUE;
      p = mchain[mon[i]].next;
      while ((v = mchain[p].vnum) != vfirst)
	{
	 if (mchain[p].marked)
	   {
	     processed = TRUE;
	     break;		/* break from while */
	   }
	 else
	   mchain[p].marked = TRUE;

	  if (_greater_than(&vert[v].pt, &ymax))
	    {
	      ymax = vert[v].pt;
	      posmax = p;
	    }
	  if (_less_than(&vert[v].pt, &ymin))
	    {
	      ymin = vert[v].pt;
	      posmin = p;
	    }
	  p = mchain[p].next;
	  vcount++;
       }

      if (processed)		/* Go to next polygon */
	continue;
      
    if (vcount == 3)		/* already a triangle */
	{
	  if (mustFlip) 
		  op.AppendTri(OUT_INDEX(mchain[p].vnum), OUT_INDEX(mchain[mchain[p].prev].vnum), OUT_INDEX(mchain[mchain[p].next].vnum));
	  else op.AppendTri(OUT_INDEX(mchain[p].vnum), OUT_INDEX(mchain[mchain[p].next].vnum), OUT_INDEX(mchain[mchain[p].prev].vnum));

	  /*	
	  op[op_idx][0] = mchain[p].vnum;
	  op[op_idx][1] = mchain[mchain[p].next].vnum;
	  op[op_idx][2] = mchain[mchain[p].prev].vnum;
	  */
	  op_idx++;
	}
      else			/* triangulate the polygon */
	{
	  v = mchain[mchain[posmax].next].vnum;
	  if (_equal_to(&vert[v].pt, &ymin))
	    {			/* LHS is a single line */
	      triangulate_single_polygon(nvert, posmax, TRI_LHS, op);
	    }
	  else
	    triangulate_single_polygon(nvert, posmax, TRI_RHS, op);
	}
    }
  
#ifdef DEBUG
  for (i = 0; i < op_idx; i++)
    fprintf(stderr, "tri #%d: (%d, %d, %d)\n", i, op[i][0], op[i][1],
	   op[i][2]);
#endif
  return op_idx;
}


/* A greedy corner-cutting algorithm to triangulate a y-monotone 
 * polygon in O(n) time.
 * Joseph O-Rourke, Computational Geometry in C.
 */
static int triangulate_single_polygon(int nvert, int posmax, int side, GFaceList &op)
{
  register int v;
  //int rc[SEGSIZE]; 

  ArrayDbg<int> rc;
  rc.SetLength(seg.Length());


  int ri = 0;	/* reflex chain */
  int endv, tmp, vpos;
  
  if (side == TRI_RHS)		/* RHS segment is a single segment */
    {
      rc[0] = mchain[posmax].vnum;
      tmp = mchain[posmax].next;
      rc[1] = mchain[tmp].vnum;
      ri = 1;
      
      vpos = mchain[tmp].next;
      v = mchain[vpos].vnum;
      
      if ((endv = mchain[mchain[posmax].prev].vnum) == 0)
		endv = nvert;
    }
  else				/* LHS is a single segment */
    {
      tmp = mchain[posmax].next;
      rc[0] = mchain[tmp].vnum;
      tmp = mchain[tmp].next;
      rc[1] = mchain[tmp].vnum;
      ri = 1;

      vpos = mchain[tmp].next;
      v = mchain[vpos].vnum;

      endv = mchain[posmax].vnum;
    }
  
  while ((v != endv) || (ri > 1))
    {
/*
	  if (ri >= (SEGSIZE -1)) { // HG 
		  TRACE("triangulate ::   Bad Chain \n ");
		  return -1;
	  }
*/
      if (ri > 0)		/* reflex chain is non-empty */
	{
	  if (CROSS(vert[v].pt, vert[rc[ri - 1]].pt, 
		    vert[rc[ri]].pt) > 0)
	    {			/* convex corner: cut if off */
		  if (mustFlip) 
		  op.AppendTri(OUT_INDEX(rc[ri - 1]), OUT_INDEX(v), OUT_INDEX(rc[ri]) );
		  else op.AppendTri(OUT_INDEX(rc[ri - 1]), OUT_INDEX(rc[ri]), OUT_INDEX(v) );
/*

	      op[op_idx][0] = rc[ri - 1];
	      op[op_idx][1] = rc[ri];
	      op[op_idx][2] = v;
*/
	      op_idx++;	     
	      ri--;
	    }
	  else		/* non-convex */
	    {		/* add v to the chain */
	      ri++;
		  if (ri>= rc.Length()) return(-1); // hg rc.SetLength(ri+1);
	      rc[ri] = v;
	      vpos = mchain[vpos].next;
	      v = mchain[vpos].vnum;
	    }
	}
      else			/* reflex-chain empty: add v to the */
	{			/* reflex chain and advance it  */
	  ri++;
	  if (ri>= rc.Length()) return(-1); // HG  rc.SetLength(ri+1);
	  rc[ri] = v;

	  //rc[++ri] = v;
	  vpos = mchain[vpos].next;
	  v = mchain[vpos].vnum;
	}
    } /* end-while */
  
  /* reached the bottom vertex. Add in the triangle formed */
  if (mustFlip) op.AppendTri(OUT_INDEX(rc[ri - 1]), OUT_INDEX(v),OUT_INDEX(rc[ri]));
  else op.AppendTri(OUT_INDEX(rc[ri - 1]), OUT_INDEX(rc[ri]),OUT_INDEX(v));
/*
  op[op_idx][0] = rc[ri - 1];
  op[op_idx][1] = rc[ri];
  op[op_idx][2] = v;
*/
  op_idx++;	     
  ri--;
  
  return 0;
}





/* Initilialise the query structure (Q) and the trapezoid table (T) 
 * when the first segment is added to start the trapezoidation. The
 * query-tree starts out with 4 trapezoids, one S-node and 2 Y-nodes
 *    
 *                4
 *   -----------------------------------
 *  		  \
 *  	1	   \        2
 *  		    \
 *   -----------------------------------
 *                3
 */

static int init_query_structure(int segnum)
{
  int i1, i2, i3, i4, i5, i6, i7, root;
  int t1, t2, t3, t4;
  segment_t *s = &seg[segnum];

  q_idx = tr_idx = 1;

/* seems to be very slow 
  memset((void *)tr, 0, sizeof(tr));
  memset((void *)qs, 0, sizeof(qs));
*/

  i1 = newnode();

  qs[i1].nodetype = T_Y;
  _max(&qs[i1].yval, &s->v0, &s->v1); /* root */
  root = i1;

  qs[i1].right = i2 = newnode();
  qs[i2].nodetype = T_SINK;
  qs[i2].parent = i1;

  qs[i1].left = i3 = newnode();
  qs[i3].nodetype = T_Y;
  _min(&qs[i3].yval, &s->v0, &s->v1); /* root */
  qs[i3].parent = i1;
  
  qs[i3].left = i4 = newnode();
  qs[i4].nodetype = T_SINK;
  qs[i4].parent = i3;
  
  qs[i3].right = i5 = newnode();
  qs[i5].nodetype = T_X;
  qs[i5].segnum = segnum;
  qs[i5].parent = i3;
  
  qs[i5].left = i6 = newnode();
  qs[i6].nodetype = T_SINK;
  qs[i6].parent = i5;

  qs[i5].right = i7 = newnode();
  qs[i7].nodetype = T_SINK;
  qs[i7].parent = i5;

  t1 = newtrap();		/* middle left */
  t2 = newtrap();		/* middle right */
  t3 = newtrap();		/* bottom-most */
  t4 = newtrap();		/* topmost */

  tr[t1].hi = tr[t2].hi = tr[t4].lo = qs[i1].yval;
  tr[t1].lo = tr[t2].lo = tr[t3].hi = qs[i3].yval;
  tr[t4].hi.y = (double) (INFINITY);
  tr[t4].hi.x = (double) (INFINITY);
  tr[t3].lo.y = (double) -1* (INFINITY);
  tr[t3].lo.x = (double) -1* (INFINITY);
  tr[t1].rseg = tr[t2].lseg = segnum;
  tr[t1].u0 = tr[t2].u0 = t4;
  tr[t1].d0 = tr[t2].d0 = t3;
  tr[t4].d0 = tr[t3].u0 = t1;
  tr[t4].d1 = tr[t3].u1 = t2;
  
  tr[t1].sink = i6;
  tr[t2].sink = i7;
  tr[t3].sink = i4;
  tr[t4].sink = i2;

  tr[t1].state = tr[t2].state = ST_VALID;
  tr[t3].state = tr[t4].state = ST_VALID;

  qs[i2].trnum = t4;
  qs[i4].trnum = t3;
  qs[i6].trnum = t1;
  qs[i7].trnum = t2;

  s->is_inserted = TRUE;
  return root;
}


/* Retun TRUE if the vertex v is to the left of line segment no.
 * segnum. Takes care of the degenerate cases when both the vertices
 * have the same y--cood, etc.
 */

static int is_left_of(int segnum, point_t *v)
{
  segment_t *s = &seg[segnum];
  double area;
  
  if (_greater_than(&s->v1, &s->v0)) /* seg. going upwards */
    {
      if (FP_EQUAL(s->v1.y, v->y))
	{
	  if (v->x < s->v1.x)
	    area = 1.0;
	  else
	    area = -1.0;
	}
      else if (FP_EQUAL(s->v0.y, v->y))
	{
	  if (v->x < s->v0.x)
	    area = 1.0;
	  else
	    area = -1.0;
	}
      else
	area = CROSS(s->v0, s->v1, (*v));
    }
  else				/* v0 > v1 */
    {
      if (FP_EQUAL(s->v1.y, v->y))
	{
	  if (v->x < s->v1.x)
	    area = 1.0;
	  else
	    area = -1.0;
	}
      else if (FP_EQUAL(s->v0.y, v->y))
	{
	  if (v->x < s->v0.x)
	    area = 1.0;
	  else
	    area = -1.0;
	}
      else
	area = CROSS(s->v1, s->v0, (*v));
    }
  
  if (area > 0.0)
    return TRUE;
  else 
    return FALSE;
}



/* Returns true if the corresponding endpoint of the given segment is */
/* already inserted into the segment tree. Use the simple test of */
/* whether the segment which shares this endpoint is already inserted */

static int inserted(int segnum, int whichpt)
{
  if (whichpt == FIRSTPT)
    return seg[seg[segnum].prev].is_inserted;
  else
    return seg[seg[segnum].next].is_inserted;
}

/* This is query routine which determines which trapezoid does the 
 * point v lie in. The return value is the trapezoid number. 
 */

int locate_endpoint(point_t *v, point_t *vo, int r)
{
  if (r <=0) {
	  return (-1);
  }
  node_t *rptr = &qs[r];
  
  switch (rptr->nodetype)
    {
    case T_SINK:
      return rptr->trnum;
      
    case T_Y:
      if (_greater_than(v, &rptr->yval)) /* above */
		  return locate_endpoint(v, vo, rptr->right);
      else if (_equal_to(v, &rptr->yval)) /* the point is already */
	  {			          /* inserted. */
		if (_greater_than(vo, &rptr->yval)) /* above */
			return locate_endpoint(v, vo, rptr->right);
		else 
			return locate_endpoint(v, vo, rptr->left); /* below */	    
	  }
      else
		return locate_endpoint(v, vo, rptr->left); /* below */

    case T_X:
      if (_equal_to(v, &seg[rptr->segnum].v0) || 
	       _equal_to(v, &seg[rptr->segnum].v1))
	  {
	  if (FP_EQUAL(v->y, vo->y)) /* horizontal segment */
	    {
	      if (vo->x < v->x)
			return locate_endpoint(v, vo, rptr->left); /* left */
	      else
			return locate_endpoint(v, vo, rptr->right); /* right */
	    }

	  else if (is_left_of(rptr->segnum, vo))
	    return locate_endpoint(v, vo, rptr->left); /* left */
	  else
	    return locate_endpoint(v, vo, rptr->right); /* right */
	  }
      else if (is_left_of(rptr->segnum, v))
		return locate_endpoint(v, vo, rptr->left); /* left */
      else
		return locate_endpoint(v, vo, rptr->right); /* right */	

    default:
	  TRACE("triangulate :  locate endpoint Haggu !!!!!\n");
	  return(-1); // hg 
      break;
    }
}


/* Thread in the segment into the existing trapezoidation. The 
 * limiting trapezoids are given by tfirst and tlast (which are the
 * trapezoids containing the two endpoints of the segment. Merges all
 * possible trapezoids which flank this segment and have been recently
 * divided because of its insertion
 */

static int merge_trapezoids(int segnum, int tfirst, int tlast, int side)
{
  int t, tnext, cond;
  int ptnext;

  if ((tfirst <0) || (tlast <0)) {	 // HG 
      TRACE("triangulate :  merge_trapezoids Haggu !!!!!\n");
	  return -1; 
  }	

  /* First merge polys on the LHS */
  t = tfirst;
  while ((t > 0) && _greater_than_equal_to(&tr[t].lo, &tr[tlast].lo))
    {
      if (side == S_LEFT)
	cond = ((((tnext = tr[t].d0) > 0) && (tr[tnext].rseg == segnum)) ||
		(((tnext = tr[t].d1) > 0) && (tr[tnext].rseg == segnum)));
      else
	cond = ((((tnext = tr[t].d0) > 0) && (tr[tnext].lseg == segnum)) ||
		(((tnext = tr[t].d1) > 0) && (tr[tnext].lseg == segnum)));
      
      if (cond)
	{
	  if ((tr[t].lseg == tr[tnext].lseg) &&
	      (tr[t].rseg == tr[tnext].rseg)) /* good neighbours */
	    {			              /* merge them */
	      /* Use the upper node as the new node i.e. t */
	      
	      ptnext = qs[tr[tnext].sink].parent;
	      
	      if (qs[ptnext].left == tr[tnext].sink)
		qs[ptnext].left = tr[t].sink;
	      else
		qs[ptnext].right = tr[t].sink;	/* redirect parent */
	      
	      
	      /* Change the upper neighbours of the lower trapezoids */
	      
	      if ((tr[t].d0 = tr[tnext].d0) > 0)
		if (tr[tr[t].d0].u0 == tnext)
		  tr[tr[t].d0].u0 = t;
		else if (tr[tr[t].d0].u1 == tnext)
		  tr[tr[t].d0].u1 = t;
	      
	      if ((tr[t].d1 = tr[tnext].d1) > 0)
		if (tr[tr[t].d1].u0 == tnext)
		  tr[tr[t].d1].u0 = t;
		else if (tr[tr[t].d1].u1 == tnext)
		  tr[tr[t].d1].u1 = t;
	      
	      tr[t].lo = tr[tnext].lo;
	      tr[tnext].state = ST_INVALID; /* invalidate the lower */
				            /* trapezium */
	    }
	  else		    /* not good neighbours */
	    t = tnext;
	}
      else		    /* do not satisfy the outer if */
	t = tnext;
      
    } /* end-while */
       
  return 0;
}


/* Add in the new segment into the trapezoidation and update Q and T
 * structures. First locate the two endpoints of the segment in the
 * Q-structure. Then start from the topmost trapezoid and go down to
 * the  lower trapezoid dividing all the trapezoids in between .
 */

static int add_segment(int segnum)
{
  segment_t s;
  segment_t *so = &seg[segnum];
  int tu, tl, sk, tfirst, tlast, tnext;
  int tfirstr, tlastr, tfirstl, tlastl;
  int i1, i2, t, t1, t2, tn;
  point_t tpt;
  int tritop = 0, tribot = 0, is_swapped = 0;
  int tmptriseg;

  tfirstr=tlastr= -1; // hg 

  s = seg[segnum];

  if (_equal_to(&s.v1,&s.v0)) {	 //HG 
	  TRACE("Ingnoring zero length segment");
	  seg[segnum].is_inserted = FALSE;
	  return 0;
  }

  if (_greater_than(&s.v1, &s.v0)) /* Get higher vertex in v0 */
    {
      int tmp;
      tpt = s.v0;
      s.v0 = s.v1;
      s.v1 = tpt;
      tmp = s.root0;
      s.root0 = s.root1;
      s.root1 = tmp;
      is_swapped = TRUE;
    }

  if ((is_swapped) ? !inserted(segnum, LASTPT) :
       !inserted(segnum, FIRSTPT))     /* insert v0 in the tree */
    {
      int tmp_d;

      tu = locate_endpoint(&s.v0, &s.v1, s.root0);
	  if (tu<=0) return -1; // HG find can't locate
      tl = newtrap();		/* tl is the new lower trapezoid */
      tr[tl].state = ST_VALID;
      tr[tl] = tr[tu];
      tr[tu].lo.y = tr[tl].hi.y = s.v0.y;
      tr[tu].lo.x = tr[tl].hi.x = s.v0.x;
      tr[tu].d0 = tl;      
      tr[tu].d1 = 0;
      tr[tl].u0 = tu;
      tr[tl].u1 = 0;

      if (((tmp_d = tr[tl].d0) > 0) && (tr[tmp_d].u0 == tu))
		tr[tmp_d].u0 = tl;
      if (((tmp_d = tr[tl].d0) > 0) && (tr[tmp_d].u1 == tu))
		tr[tmp_d].u1 = tl;

      if (((tmp_d = tr[tl].d1) > 0) && (tr[tmp_d].u0 == tu))
		tr[tmp_d].u0 = tl;
      if (((tmp_d = tr[tl].d1) > 0) && (tr[tmp_d].u1 == tu))
		tr[tmp_d].u1 = tl;

      /* Now update the query structure and obtain the sinks for the */
      /* two trapezoids */ 
      
      i1 = newnode();		/* Upper trapezoid sink */
      i2 = newnode();		/* Lower trapezoid sink */
      sk = tr[tu].sink;
      
      qs[sk].nodetype = T_Y;
      qs[sk].yval = s.v0;
      qs[sk].segnum = segnum;	/* not really reqd ... maybe later */
      qs[sk].left = i2;
      qs[sk].right = i1;

      qs[i1].nodetype = T_SINK;
      qs[i1].trnum = tu;
      qs[i1].parent = sk;

      qs[i2].nodetype = T_SINK;
      qs[i2].trnum = tl;
      qs[i2].parent = sk;

      tr[tu].sink = i1;
      tr[tl].sink = i2;
      tfirst = tl;
    }
  else				/* v0 already present */
    {       /* Get the topmost intersecting trapezoid */
      tfirst = locate_endpoint(&s.v0, &s.v1, s.root0);
      tritop = 1;
    }


  if ((is_swapped) ? !inserted(segnum, FIRSTPT) :
       !inserted(segnum, LASTPT))     /* insert v1 in the tree */
    {
      int tmp_d;

      tu = locate_endpoint(&s.v1, &s.v0, s.root1);

      tl = newtrap();		/* tl is the new lower trapezoid */
      tr[tl].state = ST_VALID;
      tr[tl] = tr[tu];
      tr[tu].lo.y = tr[tl].hi.y = s.v1.y;
      tr[tu].lo.x = tr[tl].hi.x = s.v1.x;
      tr[tu].d0 = tl;      
      tr[tu].d1 = 0;
      tr[tl].u0 = tu;
      tr[tl].u1 = 0;

      if (((tmp_d = tr[tl].d0) > 0) && (tr[tmp_d].u0 == tu))
	tr[tmp_d].u0 = tl;
      if (((tmp_d = tr[tl].d0) > 0) && (tr[tmp_d].u1 == tu))
	tr[tmp_d].u1 = tl;

      if (((tmp_d = tr[tl].d1) > 0) && (tr[tmp_d].u0 == tu))
	tr[tmp_d].u0 = tl;
      if (((tmp_d = tr[tl].d1) > 0) && (tr[tmp_d].u1 == tu))
	tr[tmp_d].u1 = tl;
      
      /* Now update the query structure and obtain the sinks for the */
      /* two trapezoids */ 
      
      i1 = newnode();		/* Upper trapezoid sink */
      i2 = newnode();		/* Lower trapezoid sink */
      sk = tr[tu].sink;
      
      qs[sk].nodetype = T_Y;
      qs[sk].yval = s.v1;
      qs[sk].segnum = segnum;	/* not really reqd ... maybe later */
      qs[sk].left = i2;
      qs[sk].right = i1;

      qs[i1].nodetype = T_SINK;
      qs[i1].trnum = tu;
      qs[i1].parent = sk;

      qs[i2].nodetype = T_SINK;
      qs[i2].trnum = tl;
      qs[i2].parent = sk;

      tr[tu].sink = i1;
      tr[tl].sink = i2;
      tlast = tu;
    }
  else				/* v1 already present */
    {       /* Get the lowermost intersecting trapezoid */
      tlast = locate_endpoint(&s.v1, &s.v0, s.root1);
      tribot = 1;
    }
  
  /* Thread the segment into the query tree creating a new X-node */
  /* First, split all the trapezoids which are intersected by s into */
  /* two */

  t = tfirst;			/* topmost trapezoid */
  
  while ((t > 0) && 
	 _greater_than_equal_to(&tr[t].lo, &tr[tlast].lo))
				/* traverse from top to bot */
    {
      int t_sav, tn_sav;
      sk = tr[t].sink;
      i1 = newnode();		/* left trapezoid sink */
      i2 = newnode();		/* right trapezoid sink */
      
      qs[sk].nodetype = T_X;
      qs[sk].segnum = segnum;
      qs[sk].left = i1;
      qs[sk].right = i2;

      qs[i1].nodetype = T_SINK;	/* left trapezoid (use existing one) */
      qs[i1].trnum = t;
      qs[i1].parent = sk;

      qs[i2].nodetype = T_SINK;	/* right trapezoid (allocate new) */
      qs[i2].trnum = tn = newtrap();
      tr[tn].state = ST_VALID;
      qs[i2].parent = sk;

      if (t == tfirst)
		tfirstr = tn;
      if (_equal_to(&tr[t].lo, &tr[tlast].lo))
		tlastr = tn;

      tr[tn] = tr[t];
      tr[t].sink = i1;
      tr[tn].sink = i2;
      t_sav = t;
      tn_sav = tn;

      /* error */

      if ((tr[t].d0 <= 0) && (tr[t].d1 <= 0)) /* case cannot arise */  // HG but arises
	  {
		TRACE("add_segment: error\n");
		break;
	  }
      
      /* only one trapezoid below. partition t into two and make the */
      /* two resulting trapezoids t and tn as the upper neighbours of */
      /* the sole lower trapezoid */
      
      else if ((tr[t].d0 > 0) && (tr[t].d1 <= 0))
	  {			/* Only one trapezoid below */
	  if ((tr[t].u0 > 0) && (tr[t].u1 > 0))
	    {			/* continuation of a chain from abv. */
	      if (tr[t].usave > 0) /* three upper neighbours */
		{
		  if (tr[t].uside == S_LEFT)
		    {
		      tr[tn].u0 = tr[t].u1;
		      tr[t].u1 = -1;
		      tr[tn].u1 = tr[t].usave;
		      
		      tr[tr[t].u0].d0 = t;
		      tr[tr[tn].u0].d0 = tn;
		      tr[tr[tn].u1].d0 = tn;
		    }
		  else		/* intersects in the right */
		    {
		      tr[tn].u1 = -1;
		      tr[tn].u0 = tr[t].u1;
		      tr[t].u1 = tr[t].u0;
		      tr[t].u0 = tr[t].usave;

		      tr[tr[t].u0].d0 = t;
		      tr[tr[t].u1].d0 = t;
		      tr[tr[tn].u0].d0 = tn;		      
		    }
		  
		  tr[t].usave = tr[tn].usave = 0;
		}
	      else		/* No usave.... simple case */
		{
		  tr[tn].u0 = tr[t].u1;
		  tr[t].u1 = tr[tn].u1 = -1;
		  tr[tr[tn].u0].d0 = tn;
		}
	    }
	  else 
	    {			/* fresh seg. or upward cusp */
	      int tmp_u = tr[t].u0;

		  if (tmp_u<=0) { // HG 
				TRACE("add_segment: u error\n");
				break;
		  }
	      int td0, td1;
	      if (((td0 = tr[tmp_u].d0) > 0) && 
		  ((td1 = tr[tmp_u].d1) > 0))
		{		/* upward cusp */
		  if ((tr[td0].rseg > 0) &&
		      !is_left_of(tr[td0].rseg, &s.v1))
		    {
		      tr[t].u0 = tr[t].u1 = tr[tn].u1 = -1;
		      tr[tr[tn].u0].d1 = tn;
		    }
		  else		/* cusp going leftwards */
		    { 
		      tr[tn].u0 = tr[tn].u1 = tr[t].u1 = -1;
		      tr[tr[t].u0].d0 = t;
		    }
		}
	      else		/* fresh segment */
		{
		  tr[tr[t].u0].d0 = t;
		  tr[tr[t].u0].d1 = tn;
		}	      
	    }
	  
	  if (FP_EQUAL(tr[t].lo.y, tr[tlast].lo.y) && 
	      FP_EQUAL(tr[t].lo.x, tr[tlast].lo.x) && tribot)
	    {		/* bottom forms a triangle */

	      if (is_swapped)	
			tmptriseg = seg[segnum].prev;
	      else
			tmptriseg = seg[segnum].next;
	      
	      if ((tmptriseg > 0) && is_left_of(tmptriseg, &s.v0))
		{
				/* L-R downward cusp */
		  tr[tr[t].d0].u0 = t;
		  tr[tn].d0 = tr[tn].d1 = -1;
		}
	      else
		{
				/* R-L downward cusp */
		  tr[tr[tn].d0].u1 = tn;
		  tr[t].d0 = tr[t].d1 = -1;
		}
	    }
	  else
	    {
	      if ((tr[tr[t].d0].u0 > 0) && (tr[tr[t].d0].u1 > 0))
		{
		  if (tr[tr[t].d0].u0 == t) /* passes thru LHS */
		    {
		      tr[tr[t].d0].usave = tr[tr[t].d0].u1;
		      tr[tr[t].d0].uside = S_LEFT;
		    }
		  else
		    {
		      tr[tr[t].d0].usave = tr[tr[t].d0].u0;
		      tr[tr[t].d0].uside = S_RIGHT;
		    }		    
		}
	      tr[tr[t].d0].u0 = t;
	      tr[tr[t].d0].u1 = tn;
	    }
	  
	  t = tr[t].d0;
	}


      else if ((tr[t].d0 <= 0) && (tr[t].d1 > 0))
	{			/* Only one trapezoid below */
	  if ((tr[t].u0 > 0) && (tr[t].u1 > 0))
	    {			/* continuation of a chain from abv. */
	      if (tr[t].usave > 0) /* three upper neighbours */
		{
		  if (tr[t].uside == S_LEFT)
		    {
		      tr[tn].u0 = tr[t].u1;
		      tr[t].u1 = -1;
		      tr[tn].u1 = tr[t].usave;
		      
		      tr[tr[t].u0].d0 = t;
		      tr[tr[tn].u0].d0 = tn;
		      tr[tr[tn].u1].d0 = tn;
		    }
		  else		/* intersects in the right */
		    {
		      tr[tn].u1 = -1;
		      tr[tn].u0 = tr[t].u1;
		      tr[t].u1 = tr[t].u0;
		      tr[t].u0 = tr[t].usave;

		      tr[tr[t].u0].d0 = t;
		      tr[tr[t].u1].d0 = t;
		      tr[tr[tn].u0].d0 = tn;		      
		    }
		  
		  tr[t].usave = tr[tn].usave = 0;
		}
	      else		/* No usave.... simple case */
		{
		  tr[tn].u0 = tr[t].u1;
		  tr[t].u1 = tr[tn].u1 = -1;
		  tr[tr[tn].u0].d0 = tn;
		}
	    }
	  else 
	    {			/* fresh seg. or upward cusp */
	      int tmp_u = tr[t].u0;
	      int td0, td1;
	      if (((td0 = tr[tmp_u].d0) > 0) && 
		  ((td1 = tr[tmp_u].d1) > 0))
		{		/* upward cusp */
		  if ((tr[td0].rseg > 0) &&
		      !is_left_of(tr[td0].rseg, &s.v1))
		    {
		      tr[t].u0 = tr[t].u1 = tr[tn].u1 = -1;
		      tr[tr[tn].u0].d1 = tn;
		    }
		  else 
		    {
		      tr[tn].u0 = tr[tn].u1 = tr[t].u1 = -1;
		      tr[tr[t].u0].d0 = t;
		    }
		}
	      else		/* fresh segment */
		{
		  tr[tr[t].u0].d0 = t;
		  tr[tr[t].u0].d1 = tn;
		}
	    }
	  
	  if (FP_EQUAL(tr[t].lo.y, tr[tlast].lo.y) && 
	      FP_EQUAL(tr[t].lo.x, tr[tlast].lo.x) && tribot)
	    {		/* bottom forms a triangle */
	      int tmpseg; /// ??? to do, not initialized 

	      if (is_swapped)	
			tmptriseg = seg[segnum].prev;
	      else
			tmptriseg = seg[segnum].next;

		  tmpseg = tmptriseg ; /// hg XXXXXXXXXXXXXXXX ???????????? 

	      if ((tmpseg > 0) && is_left_of(tmpseg, &s.v0))
		{
		  /* L-R downward cusp */
		  tr[tr[t].d1].u0 = t;
		  tr[tn].d0 = tr[tn].d1 = -1;
		}
	      else
		{
		  /* R-L downward cusp */
		  tr[tr[tn].d1].u1 = tn;
		  tr[t].d0 = tr[t].d1 = -1;
		}
	    }		
	  else
	    {
	      if ((tr[tr[t].d1].u0 > 0) && (tr[tr[t].d1].u1 > 0))
		{
		  if (tr[tr[t].d1].u0 == t) /* passes thru LHS */
		    {
		      tr[tr[t].d1].usave = tr[tr[t].d1].u1;
		      tr[tr[t].d1].uside = S_LEFT;
		    }
		  else
		    {
		      tr[tr[t].d1].usave = tr[tr[t].d1].u0;
		      tr[tr[t].d1].uside = S_RIGHT;
		    }		    
		}
	      tr[tr[t].d1].u0 = t;
	      tr[tr[t].d1].u1 = tn;
	    }
	  
	  t = tr[t].d1;
	}

      /* two trapezoids below. Find out which one is intersected by */
      /* this segment and proceed down that one */
      
      else
	{
	  int tmpseg = tr[tr[t].d0].rseg;
	  double y0, yt;
	  point_t tmppt;
	  int tnext, i_d0, i_d1;

	  i_d0 = i_d1 = FALSE;
	  if (FP_EQUAL(tr[t].lo.y, s.v0.y))
	    {
	      if (tr[t].lo.x > s.v0.x)
		i_d0 = TRUE;
	      else
		i_d1 = TRUE;
	    }
	  else
	    {
	      tmppt.y = y0 = tr[t].lo.y;
	      yt = (y0 - s.v0.y)/(s.v1.y - s.v0.y);
	      tmppt.x = s.v0.x + yt * (s.v1.x - s.v0.x);
	      
	      if (_less_than(&tmppt, &tr[t].lo))
		i_d0 = TRUE;
	      else
		i_d1 = TRUE;
	    }
	  
	  /* check continuity from the top so that the lower-neighbour */
	  /* values are properly filled for the upper trapezoid */

	  if ((tr[t].u0 > 0) && (tr[t].u1 > 0))
	    {			/* continuation of a chain from abv. */
	      if (tr[t].usave > 0) /* three upper neighbours */
		{
		  if (tr[t].uside == S_LEFT)
		    {
		      tr[tn].u0 = tr[t].u1;
		      tr[t].u1 = -1;
		      tr[tn].u1 = tr[t].usave;
		      
		      tr[tr[t].u0].d0 = t;
		      tr[tr[tn].u0].d0 = tn;
		      tr[tr[tn].u1].d0 = tn;
		    }
		  else		/* intersects in the right */
		    {
		      tr[tn].u1 = -1;
		      tr[tn].u0 = tr[t].u1;
		      tr[t].u1 = tr[t].u0;
		      tr[t].u0 = tr[t].usave;

		      tr[tr[t].u0].d0 = t;
		      tr[tr[t].u1].d0 = t;
		      tr[tr[tn].u0].d0 = tn;		      
		    }
		  
		  tr[t].usave = tr[tn].usave = 0;
		}
	      else		/* No usave.... simple case */
		{
		  tr[tn].u0 = tr[t].u1;
		  tr[tn].u1 = -1;
		  tr[t].u1 = -1;
		  tr[tr[tn].u0].d0 = tn;
		}
	    }
	  else 
	    {			/* fresh seg. or upward cusp */
	      int tmp_u = tr[t].u0;
	      int td0, td1;
		  if (tmp_u <= 0) { // HG
			  TRACE("add_segment bad tr %d \n",t);
		  } else 
		  {

			if (((td0 = tr[tmp_u].d0) > 0) && 
		  ((td1 = tr[tmp_u].d1) > 0))
		{		/* upward cusp */
		  if ((tr[td0].rseg > 0) &&
		      !is_left_of(tr[td0].rseg, &s.v1))
		    {
		      tr[t].u0 = tr[t].u1 = tr[tn].u1 = -1;
		      tr[tr[tn].u0].d1 = tn;
		    }
		  else 
		    {
		      tr[tn].u0 = tr[tn].u1 = tr[t].u1 = -1;
		      tr[tr[t].u0].d0 = t;
		    }
		}
	      else		/* fresh segment */
		{
		  tr[tr[t].u0].d0 = t;
		  tr[tr[t].u0].d1 = tn;
		}
	  }  
	  }
	  
	  if (FP_EQUAL(tr[t].lo.y, tr[tlast].lo.y) && 
	      FP_EQUAL(tr[t].lo.x, tr[tlast].lo.x) && tribot)
	    {
	      /* this case arises only at the lowest trapezoid.. i.e.
		 tlast, if the lower endpoint of the segment is
		 already inserted in the structure */
	      
	      tr[tr[t].d0].u0 = t;
	      tr[tr[t].d0].u1 = -1;
	      tr[tr[t].d1].u0 = tn;
	      tr[tr[t].d1].u1 = -1;

	      tr[tn].d0 = tr[t].d1;
	      tr[t].d1 = tr[tn].d1 = -1;
	      
	      tnext = tr[t].d1;	      
	    }
	  else if (i_d0)
				/* intersecting d0 */
	    {
	      tr[tr[t].d0].u0 = t;
	      tr[tr[t].d0].u1 = tn;
	      tr[tr[t].d1].u0 = tn;
	      tr[tr[t].d1].u1 = -1;
	      
	      /* new code to determine the bottom neighbours of the */
	      /* newly partitioned trapezoid */
	      
	      tr[t].d1 = -1;

	      tnext = tr[t].d0;
	    }
	  else			/* intersecting d1 */
	    {
	      tr[tr[t].d0].u0 = t;
	      tr[tr[t].d0].u1 = -1;
	      tr[tr[t].d1].u0 = t;
	      tr[tr[t].d1].u1 = tn;

	      /* new code to determine the bottom neighbours of the */
	      /* newly partitioned trapezoid */
	      
	      tr[tn].d0 = tr[t].d1;
	      tr[tn].d1 = -1;
	      
	      tnext = tr[t].d1;
	    }	    
	  
	  t = tnext;
	}
      
      tr[t_sav].rseg = tr[tn_sav].lseg  = segnum;
    } /* end-while */
  
  /* Now combine those trapezoids which share common segments. We can */
  /* use the pointers to the parent to connect these together. This */
  /* works only because all these new trapezoids have been formed */
  /* due to splitting by the segment, and hence have only one parent */

  tfirstl = tfirst; 
  tlastl = tlast;
  merge_trapezoids(segnum, tfirstl, tlastl, S_LEFT);
  merge_trapezoids(segnum, tfirstr, tlastr, S_RIGHT);

  seg[segnum].is_inserted = TRUE;
  return 0;
}


/* Update the roots stored for each of the endpoints of the segment.
 * This is done to speed up the location-query for the endpoint when
 * the segment is inserted into the trapezoidation subsequently
 */
static int find_new_roots(int segnum)
{
  segment_t *s = &seg[segnum];
  
  if (s->is_inserted)
    return 0;

  s->root0 = locate_endpoint(&s->v0, &s->v1, s->root0);
  s->root0 = tr[s->root0].sink;

  s->root1 = locate_endpoint(&s->v1, &s->v0, s->root1);
  s->root1 = tr[s->root1].sink;  
  return 0;
}


/* Main routine to perform trapezoidation */
int construct_trapezoids(int nseg)
{
  register int i;
  int root, h;
  
  /* Add the first segment and get the query structure and trapezoid */
  /* list initialised */

  root = init_query_structure(choose_segment());

  // prealloc structures 
  if ((tr.Length()+nseg) < tr.MaxLength()) tr.SetMaxLength(tr.Length()+nseg);


  for (i = 1; i <= nseg; i++)
    seg[i].root0 = seg[i].root1 = root;
  
  for (h = 1; h <= math_logstar_n(nseg); h++)
    {
      for (i = math_N(nseg, h -1) + 1; i <= math_N(nseg, h); i++)
		if (add_segment(choose_segment()) <0) return -1; // HG
      
      /* Find a new root for each of the segment endpoints */
      for (i = 1; i <= nseg; i++)
		find_new_roots(i);
    }
  
  for (i = math_N(nseg, math_logstar_n(nseg)) + 1; i <= nseg; i++)
		if (add_segment(choose_segment()) <0) return -1; // HG

  return 0;
}


// from tri.c 
static int initialise(int n)
{
  register int i;

  for (i = 1; i <= n; i++)
    seg[i].is_inserted = FALSE;

  generate_random_ordering(n);
  
  return 0;
}


// triangulate.c


/* Input specified as contours.
 * Outer contour must be anti-clockwise.
 * All inner contours must be clockwise.
 *  
 * Every contour is specified by giving all its points in order. No
 * point shoud be repeated. i.e. if the outer contour is a square,
 * only the four distinct endpoints shopudl be specified in order.
 *  
 * ncontours: #contours
 * cntr: An array describing the number of points in each
 *	 contour. Thus, cntr[i] = #points in the i'th contour.
 * vertices: Input array of vertices. Vertices for each contour
 *           immediately follow those for previous one. Array location
 *           vertices[0] must NOT be used (i.e. i/p starts from
 *           vertices[1] instead. The output triangles are
 *	     specified  w.r.t. the indices of these vertices.
 * triangles: Output array to hold triangles.
 *  
 * Enough space must be allocated for all the arrays before calling
 * this routine
 */


int triangulate_polygon(int ncontours, int cntr[], double (*vertices)[2], GFaceList &triangles)
{
  register int i;
  int nmonpoly, ccount, npoints, genus;
  int n;

  //hg memset((void *)seg, 0, sizeof(seg));
  ccount = 0;
  i = 1;
  
  while (ccount < ncontours)
    {
      int j;
      int first, last;

      npoints = cntr[ccount];
      first = i;
      last = first + npoints - 1;
      for (j = 0; j < npoints; j++, i++)
	  {
	  seg[i].v0.x = vertices[i][0];
	  seg[i].v0.y = vertices[i][1];

	  if (i == last)
	    {
	      seg[i].next = first;
	      seg[i].prev = i-1;
	      seg[i-1].v1 = seg[i].v0;
	    }
	  else if (i == first)
	    {
	      seg[i].next = i+1;
	      seg[i].prev = last;
	      seg[last].v1 = seg[i].v0;
	    }
	  else
	    {
	      seg[i].prev = i-1;
	      seg[i].next = i+1;
	      seg[i-1].v1 = seg[i].v0;
	    }
	  
	  seg[i].is_inserted = FALSE;
	}
      
      ccount++;
    }
  
  genus = ncontours - 1;
  n = i-1;

  initialise(n);
  construct_trapezoids(n);
  nmonpoly = monotonate_trapezoids(n);
  triangulate_monotone_polygons(n, nmonpoly, triangles);
  
  return 0;
}


/* This function returns TRUE or FALSE depending upon whether the 
 * vertex is inside the polygon or not. The polygon must already have
 * been triangulated before this routine is called.
 * This routine will always detect all the points belonging to the 
 * set (polygon-area - polygon-boundary). The return value for points 
 * on the boundary is not consistent!!!
 */

int is_point_inside_polygon(double vertex[2])
{
  point_t v;
  int trnum, rseg;
  trap_t *t;

  v.x = vertex[0];
  v.y = vertex[1];
  
  trnum = locate_endpoint(&v, &v, 1);
  t = &tr[trnum];
  
  if (t->state == ST_INVALID)
    return FALSE;
  
  if ((t->lseg <= 0) || (t->rseg <= 0))
    return FALSE;
  rseg = t->rseg;
  return _greater_than_equal_to(&seg[rseg].v1, &seg[rseg].v0);
}


#if 0

int triangulate_polygon(int ncontours, int cntr[], double (*vertices)[2], int (*triangles)[3])
{
  register int i;
  int nmonpoly, ccount, npoints, genus;
  int n;

  //hg memset((void *)seg, 0, sizeof(seg));
  ccount = 0;
  i = 1;
  
  while (ccount < ncontours)
    {
      int j;
      int first, last;

      npoints = *fp++;
      first = *fp;
      last = *fp[npoints - 1];
      for (j = 0; j < npoints; j++, i++)
	{
	  i= fp[j];
	  seg[i].v0.x = vertices[i][0];
	  seg[i].v0.y = vertices[i][1];

	  if (i == last)
	    {
	      seg[i].next = first;
	      seg[i].prev = i-1;
	      seg[i-1].v1 = seg[i].v0;
	    }
	  else if (i == first)
	    {
	      seg[i].next = i+1;
	      seg[i].prev = last;
	      seg[last].v1 = seg[i].v0;
	    }
	  else
	    {
	      seg[i].prev = i-1;
	      seg[i].next = i+1;
	      seg[i-1].v1 = seg[i].v0;
	    }
	  
	  seg[i].is_inserted = FALSE;
	}
      
      ccount++;
    }
  
  genus = ncontours - 1;
  n = i-1;

  initialise(n);
  construct_trapezoids(n);
  nmonpoly = monotonate_trapezoids(n);
  triangulate_monotone_polygons(n, nmonpoly, triangles);
  
  return 0;
}

#endif


void endPolygon(int i /* num segments,*/, int ncontours,GFaceList &triangles)
{
  int n;

//  mustFlip = FALSE;
  TRACE("Triangulating segments = %d, contours = %d \n",i-1,ncontours);


  n = (i-2) * 4;
  if ((triangles.Length()+n) < triangles.MaxLength()) triangles.SetMaxLength(triangles.Length()+n);


  //genus = ncontours - 1;
  n = i-1;
  seg_idx = n; // hg 

  if (n<3) { 
	  resetTables();
	  return;
  }	

  initialise(n);

  int nmonpoly = -1;
  if (construct_trapezoids(n)<0) {
	  TRACE("Can't construct trapezoides");
  }		 
  else nmonpoly = monotonate_trapezoids(n);

  if (nmonpoly <0) {
	  TRACE("Bad polygon, can't find inside trapezoid \n");
#if 0
	  mustFlip = !mustFlip;
	  // try to flip seg
	  for(int j = 1; j <=n; j++) {
		  point_t s;
		  s = seg[j].v0;
		  seg[j].v0 = seg[j].v1;
		  seg[j].v1 = s;
		  int x = seg[j].prev;
		  seg[j].prev = seg[j].next;
		  seg[j].next = x;
	  }	
	  initialise(n);
	  construct_trapezoids(n);
	  nmonpoly = monotonate_trapezoids(n);
	  triangulate_monotone_polygons(n, nmonpoly, triangles);
#endif
  }  
  else 
	  triangulate_monotone_polygons(n, nmonpoly, triangles);

  resetTables();

}


//triangulate a facelist into triangles using the GLU triangulator
int triangulate_tesselateFaceList( GFaceList &f, //@parm input facelist
						PointArray &v, //@parm corresponding vertices for indices in f
						IntArray *vmap, // @parm optional vertex map list
						GFaceList &triangles, //@parm output: triangulated facelist
						IntArray *new2old,	//@parm optional mapping from new triangles to old face indices
                        GShell * theShell  //@parm optional shell to append vertices too
					  )
{

  const int *fp = f;
  const int *fpend=fp+f.Length();
  int fi=-1;

  int beginPolygon=0;
  int i=1;	// line segment counter 
  int numContours = 0;

  
  int ftricnt= f.GetTriangleCount();
  
//  tesselateF= &newf;
//  tesselateP = &v; // combine might add new vertices !!!!
//  tesselateShell = theShell;

  
	triangles.SetLength(0);
	triangles.SetMaxLength(ftricnt*(4+1));

//  tesselateNew2Old =new2old;

  if (new2old) {
     new2old->SetLength(0);
     new2old->SetMaxLength(ftricnt);
  } 
	 
  int first,last; // segment index 

  Point normal; // of face 
  int dominantAxis;
  int x_axis,y_axis;

  int j;
  int oldTriIndex = 0;	

  resetTables();

  // for each face / face hole 
  while ( fp < fpend ) {
	 int n = *fp++;

	 if (n>0)  { // start of new polygon 

		 if (beginPolygon)  {  // close pending polygon 
			endPolygon(i,numContours,triangles);
			if (new2old) { // store triangle to old mapping 
				for(int triIndex = oldTriIndex; triIndex < triangles.Length(); triIndex+=4) {
					(*new2old).Append(fi);
				}	
				oldTriIndex = triangles.Length();
			}
         }


		// face counter
		fi++;
		// tesselateFi = fi;
		
		if (n <= 2) { // ignore edges
		   fp += n;
		   beginPolygon=0;
		   continue;
		}

		// get normal and choose index 
		normal.Zero();
		
		for(j = 0; j < n; j++) {
			int vi= fp[j]; // get next vertex index 
			int vi1 = fp[j+1];
			if (j == (n-1)) vi1 = fp[0];
			if (vmap)  {
				vi = (*vmap)[vi];
				vi1 = (*vmap)[vi1];
			}

			const Point *uu = &(v[vi]);
			const Point *vv = &(v[vi1]);

			normal.x += (uu->y - vv->y) * (uu->z + vv->z);
			normal.y += (uu->z - vv->z) * (uu->x + vv->x);
			normal.z += (uu->x - vv->x) * (uu->y + vv->y);

		}

		dominantAxis = normal.Dominant();
		normal.ProjectIndex(x_axis,y_axis, dominantAxis);

		TRACE("Normal %g %g %g, x %d y %d \n",normal.x,normal.y,normal.z,x_axis,y_axis);

		// compute handedness of polygon 

		float area=0.0;

		for(j = 0; j < n; j++) {
			int vi= fp[j]; // get next vertex index 
			int vi1 = fp[j+1];
			if (j == (n-1)) vi1 = fp[0];
			if (vmap)  {
				vi = (*vmap)[vi];
				vi1 = (*vmap)[vi1];
			}

			const Point &uu = (v[vi]);
			const Point &vv = (v[vi1]);

			area += uu[x_axis] * vv[y_axis];
			area -= uu[y_axis] * vv[x_axis];


		}


		area /= 2;

		mustFlip = area < 0; 

		TRACE("Area %f \n",area);



		// begin and new polygon 
		//memset((void *)seg, 0, sizeof(seg));
		seg.SetLength(1);
		i = 1;
		beginPolygon=1;
		numContours = 1;

	 }
	 else { // hole 
	     // gluNextContour(tobj,GLU_INTERIOR);
	     // gluNextContour(tobj,GShell_GLUTriangulationHoleCode);
		 numContours ++;
		 n= - n;
	 }

	 int vold = -1;
	 int voldmapped;
	 int vfirstMapped = -1;

	 first = last = i;	// index of first segment 
/*
	 if (i>=SEGSIZE) {
		 TRACE("Triangulate table to small %d \n",i);
		 return(-1);
	 }
*/
	 if ((seg.Length()+n) < seg.MaxLength()) seg.SetMaxLength(seg.Length()+n);

	 for (int jj=0; jj<n; jj++) 
	 //for (j=n-1; j>=0; j--) 
	 {
		int j=jj;
		if (mustFlip) j = (n-1)-jj;

		// int vi= *fp++; // get next vertex index 
		int vi= fp[j]; // get next vertex index 

		if (vi>=v.Length()) { 
			
		}

		int vimapped = vi;
		if (vmap) vimapped = (*vmap)[vi];

		float vertex[3];

		vertex[0]= v[vimapped].x,vertex[1]= v[vimapped].y,vertex[2]= v[vimapped].z;

		if ((vold >=0) && ( (vimapped == voldmapped) ||	v[vimapped] == v[voldmapped])) {  // 15.12.95
	  		// resulting from sweep code to get "hard" edges
			// ignore it 
			TRACE("identical vertex %d %d\n",vold,vi); 
		} 
		else 
		if ( (vfirstMapped >=0) && (v[vimapped] == v[vfirstMapped]))
		{
			// found a closing point 
			break;
		}
		else 
		{
			if (vfirstMapped <0)  vfirstMapped = vimapped;
			
			seg.SetLength(i+1);

			seg[i].v0.x = vertex[x_axis];
			seg[i].v0.y = vertex[y_axis];
			
			seg[i].v0.vindex = vi; // record original vertex index 

			seg[i].prev = i-1;
			seg[i].next = i+1;
			if (i != first) 
				seg[i-1].v1 = seg[i].v0;

			seg[i].root0 = seg[i].root1 = 0; 
			seg[i].is_inserted = FALSE;

			last = i; // index of last segment 
			i++;

			//gluTessVertex(tobj,vertex,(void *)vi);
		}
		
		vold = vi;
		voldmapped = vimapped;
	 } // for j<n
	 
	 fp+=n;
	 seg[first].prev = last;
	 seg[last].next = first;
	 seg[last].v1 = seg[first].v0;

  }

  if (beginPolygon) {
	  endPolygon(i,numContours,triangles);
  
		if (new2old) { // store triangle to old mapping 
			for(int triIndex = oldTriIndex; triIndex < triangles.Length(); triIndex+=4) {
				(*new2old).Append(fi);
			}	
			oldTriIndex = triangles.Length();
		}
  }

  TRACE("triangulation gives %d triangles\n",triangles.Length() / 4);

  return(0);
}



#endif // _OGL
