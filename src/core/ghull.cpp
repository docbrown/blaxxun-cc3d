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
#include "stdafx.h"

// The construction of a convex hull in 3D is a modification of code
// written by Dave Watson.  It uses an algorithm described in
//
//     Watson, D.F., 1981, Computing the n-dimensional Delaunay 
//     tessellation with application to Voronoi polytopes: 
//     The Computer J., 24(2), p. 167-172.

#include <stdlib.h>
#include <float.h>
#include <math.h>

#include "ghull.h"
//typedef int Triangle[3];


//---------------------------------------------------------------------------
int ConvexHull3D (int N, const Point pts[], 
				  int& numTriangles, Triangle*& triangle)
{
	int result;

	double** point = new double*[N+4];
	int i;
	for (i = 0; i < N+4; i++)
		point[i] = new double[3];
	for (i = 0; i < N; i++)
	{
		point[i][0] = pts[i].x;
		point[i][1] = pts[i].y;
		point[i][2] = pts[i].z;
	}

	const double EPSILON = 0.00001;
	const int TSIZE = 75;
	const double RANGE = 10.0;

	double xmin = point[0][0], xmax = xmin;
	double ymin = point[0][1], ymax = ymin;
	double zmin = point[0][2], zmax = zmin;

	for (i = 1; i < N; i++)
	{
		double value = point[i][0];
		if ( xmax < value )
			xmax = value;
		if ( xmin > value )
			xmin = value;

		value = point[i][1];
		if ( ymax < value )
			ymax = value;
		if ( ymin > value )
			ymin = value;

		value = point[i][2];
		if ( zmax < value )
			zmax = value;
		if ( zmin > value )
			zmin = value;
	}

	double xrange = xmax-xmin, yrange = ymax-ymin, zrange = zmax-zmin;
	double maxrange = xrange;
	if ( maxrange < yrange )
		maxrange = yrange;
	if ( maxrange < zrange )
		maxrange = zrange;

	// need to scale the data later to do a correct tetrahedron count
	double maxrange3 = maxrange*maxrange*maxrange;

	// tweak the points by very small random numbers
	double bgs = EPSILON*maxrange;
	srand(367);
	for (i = 0; i < N; i++)
	{
		point[i][0] += bgs*(0.5-rand()/double(RAND_MAX));
		point[i][1] += bgs*(0.5-rand()/double(RAND_MAX));
		point[i][2] += bgs*(0.5-rand()/double(RAND_MAX));
	}

	double wrk[3][4] =
	{
		{ 8*RANGE, -RANGE, -RANGE, -RANGE },
		{ -RANGE, 8*RANGE, -RANGE, -RANGE },
		{ -RANGE, -RANGE, 8*RANGE, -RANGE }
	};
	for (i = 0; i < 4; i++) 
	{
		point[N+i][0] = xmin+xrange*wrk[0][i];
		point[N+i][1] = ymin+yrange*wrk[1][i];
		point[N+i][2] = zmin+zrange*wrk[2][i];
	}

	int i0, i1, i2, i3, i4, i5, i6, i7, i8, i9, i11;
	int nts, ii[3];
	double xx;

	int tsz = 6*TSIZE;
	int** tmp = new int*[tsz+1];
	tmp[0] = new int[3*(tsz+1)];
	for (i0 = 1; i0 < tsz+1; i0++)
		tmp[i0] = tmp[0] + 3*i0;

	// Estimate of how many tetrahedrons there can be.  Since theoretically
	// the number is O(N^2), this could be quite large.  You may need to
	// increase i1 if a call to this function fails...
	i1 = 6*(N+6);
	// i1 = 16*N;  // Had to increase i1 in the case of N=3000 randomly generated
	               // points in [0,10]^3

	int* id = new int[i1];

	for (i0 = 0; i0 < i1; i0++)
		id[i0] = i0;

	int** a3s = new int*[i1];
	a3s[0] = new int[4*i1];
	for (i0 = 1; i0 < i1; i0++)
		a3s[i0] = a3s[0] + 4*i0;
	a3s[0][0] = N;
	a3s[0][1] = N+1;
	a3s[0][2] = N+2;
	a3s[0][3] = N+3;

	double** ccr = new double*[i1];  // circumscribed centers and radii
	ccr[0] = new double[4*i1];
	for (i0 = 1; i0 < i1; i0++)
		ccr[i0] = ccr[0] + 4*i0;
	ccr[0][0] = 0.0;
	ccr[0][1] = 0.0;
	ccr[0][2] = 0.0;
	ccr[0][3] = FLT_MAX;

	nts = 1;  // number of tetrahedra
	i4 = 1;

	// compute tetrahedralization
	for (i0 = 0; i0 < N; i0++)
	{  
		i1 = i7 = -1;
		i9 = 0;
		for (i11 = 0; i11 < nts; i11++)
		{  
			i1++;
			while ( a3s[i1][0] < 0 ) 
				i1++;
			xx = ccr[i1][3];
			for (i2 = 0; i2 < 3; i2++)
			{
				double z = point[i0][i2]-ccr[i1][i2];
				xx -= z*z;
				if ( xx < 0 ) 
					goto Corner3;
			}
			i9--;
			i4--;
			id[i4] = i1;
			for (i2 = 0; i2 < 4; i2++)
			{
				ii[0] = 0;
				if ( ii[0] == i2 ) 
					ii[0]++;
				for (i3 = 1; i3 < 3; i3++)
				{
					ii[i3] = ii[i3-1] + 1;
					if ( ii[i3] == i2 ) 
						ii[i3]++;
				}
				if ( i7 > 2 )
				{
					i8 = i7;
					for (i3 = 0; i3 <= i8; i3++)
					{
						for (i5 = 0; i5 < 3; i5++) 
							if ( a3s[i1][ii[i5]] != tmp[i3][i5] ) 
								goto Corner1;
						for (i6 = 0; i6 < 3; i6++) 
							tmp[i3][i6] = tmp[i8][i6];
						i7--;
						goto Corner2;
Corner1:;
					}
				}
	            if ( ++i7 > tsz )
				{
					// temporary storage exceeded, increase TSIZE
					result = 0;
					TRACE("Hull storage overflow : %s \n",tsz);
					goto ExitConvexHull;
				}
				for (i3 = 0; i3 < 3; i3++) 
					tmp[i7][i3] = a3s[i1][ii[i3]];
Corner2:;
			}
			a3s[i1][0] = -1;
Corner3:;
		}

		for (i1 = 0; i1 <= i7; i1++)
		{
			if ( tmp[i1][0] >= N )
			{
				for (i2 = 0; i2 < 3; i2++)
				{
					for (wrk[i2][3] = 0, i3 = 0; i3 < 3; i3++)
					{
						wrk[i2][i3] = point[tmp[i1][i2]][i3] - point[i0][i3];
						wrk[i2][3] += 0.5*wrk[i2][i3]*
							(point[tmp[i1][i2]][i3]+point[i0][i3]);
					}
				}
				
				xx = 
					(wrk[0][0]*(wrk[1][1]*wrk[2][2]-wrk[2][1]*wrk[1][2])) -
					(wrk[0][1]*(wrk[1][0]*wrk[2][2]-wrk[2][0]*wrk[1][2])) +
					(wrk[0][2]*(wrk[1][0]*wrk[2][1]-wrk[2][0]*wrk[1][1]));
				ccr[id[i4]][0] = 
					((wrk[0][3]*(wrk[1][1]*wrk[2][2]-wrk[2][1]*wrk[1][2])) -
					(wrk[0][1]*(wrk[1][3]*wrk[2][2]-wrk[2][3]*wrk[1][2])) +
					(wrk[0][2]*(wrk[1][3]*wrk[2][1]-wrk[2][3]* wrk[1][1])))/xx;
				ccr[id[i4]][1] = 
					((wrk[0][0]*(wrk[1][3]*wrk[2][2]-wrk[2][3]*wrk[1][2])) -
					(wrk[0][3]*(wrk[1][0]*wrk[2][2]-wrk[2][0]*wrk[1][2])) +
					(wrk[0][2]*(wrk[1][0]*wrk[2][3]-wrk[2][0]*wrk[1][3])))/xx;
				ccr[id[i4]][2] = 
					((wrk[0][0]*(wrk[1][1]*wrk[2][3]-wrk[2][1]*wrk[1][3])) -
					(wrk[0][1]*(wrk[1][0]*wrk[2][3]-wrk[2][0]*wrk[1][3])) +
					(wrk[0][3]*(wrk[1][0]*wrk[2][1]-wrk[2][0]*wrk[1][1])))/xx;
				
				for (ccr[id[i4]][3] = 0, i2 = 0; i2 < 3; i2++) 
				{
					double z = point[i0][i2] - ccr[id[i4]][i2];
					ccr[id[i4]][3] += z*z;
					a3s[id[i4]][i2] = tmp[i1][i2];
				}
				
				a3s[id[i4]][3] = i0;
				i4++;
				i9++;
			}
		}
		nts += i9;
	}

	// count the number of triangular faces
	numTriangles = 0;
	i0 = -1;
	for (i11 = 0; i11 < nts; i11++)
	{
		i0++;
		while ( a3s[i0][0] < 0 )
			i0++;

		if ( a3s[i0][1] < N )
			numTriangles++;
	}

	// create the triangles
	triangle = new Triangle[numTriangles];

	numTriangles = 0;
	i0 = -1;
	for (i11 = 0; i11 < nts; i11++)
	{
		i0++;
		while ( a3s[i0][0] < 0 )
			i0++;

		if ( a3s[i0][1] < N )
		{
			Triangle& tri = triangle[numTriangles];
			tri[0] = a3s[i0][1];
			tri[1] = a3s[i0][2];
			tri[2] = a3s[i0][3];
			numTriangles++;
		}
	}

	result = 1;

ExitConvexHull:
	delete[] tmp[0];
	delete[] tmp;
	delete[] id;
	delete[] a3s[0];
	delete[] a3s;
	delete[] ccr[0];
	delete[] ccr;
	for (i = 0; i < N+4; i++)
		delete[] point[i];
	delete[] point;

	return result;
}
//---------------------------------------------------------------------------
const double epsilon = 1e-08;

//---------------------------------------------------------------------------

typedef struct tagEdgeNode
{
	int v0, v1;
	struct tagEdgeNode* next;
}
EdgeNode;

void AddEdge (EdgeNode*& list, int v0, int v1)
{
	EdgeNode* p;

	for (p = list; p; p = p->next)
	{
		if ( (p->v0 == v0 && p->v1 == v1) || (p->v0 == v1 && p->v1 == v0) )
			return;
	}

	p = new EdgeNode;
	if (!p) return;
	p->v0 = v0;
	p->v1 = v1;
	p->next = list;
	list = p;
}

void DeleteList (EdgeNode* list)
{
	while ( list )
	{
		EdgeNode* save = list->next;
		delete list;
		list = save;
	}
}

//---------------------------------------------------------------------------
// hg was double

static int OnSameSide (
	const float nx, const float ny, const float nz, const float c,
	int fCount, const Triangle* face, const Point pts[])
{
	// test if all points on same side of line (nx,ny,nz)*(x,y,z) = c

	int posSide = 0, negSide = 0;

	for (int i = 0; i < fCount; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int k = face[i][j];
			float c0 = nx*pts[k].x+ny*pts[k].y+nz*pts[k].z;
			if ( c0 > c + epsilon )
				posSide++;
			else if ( c0 < c - epsilon )
				negSide++;
			
			if ( posSide && negSide )
			{
				// plane splits point set
				return 0;
			}
		}
	}

	return ( posSide ? +1 : -1 );
}
//---------------------------------------------------------------------------
static int WhichSide (
	const float nx, const float ny, const float nz, const float c,
	int fCount, const Triangle* face, const Point pts[])
{
	// establish which side of line hull is on
	for (int i = 0; i < fCount; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int k = face[i][0];
			double c0 = nx*pts[k].x+ny*pts[k].y+nz*pts[k].z;
			if ( c0 > c + epsilon )  // hull on positive side
				return +1;
			if ( c0 < c - epsilon )  // hull on negative side
				return -1;
		}
	}

	// hull is effectively collinear
	return 0;
}

// seppt3
//---------------------------------------------------------------------------
int SeparatePointSets3D (
	// convex hull 0
	int n0, const Point pts0[],	int fCount0, const Triangle* face0,
	// convex hull 1
	int n1, const Point pts1[], int fCount1, const Triangle* face1,
	
	// result plane, if result >0
	Plane &plane)
{
	int i, j0, j1, j2, side0, side1;
	//hg was double 
	float	ux, uy, uz, vx, vy, vz;
	float   xNor,yNor,zNor,c;

	// test faces of hull 0 for possible separation of points
	for (i = 0; i < fCount0; i++)
	{
		// lookup face (assert: j0 != j1 && j0 != j2 && j1 != j2)
		j0 = face0[i][0];
		j1 = face0[i][1];
		j2 = face0[i][2];

		// compute perpendicular to face (assert: (xNor,yNor,zNor) != (0,0,0))
		ux = pts0[j1].x-pts0[j0].x;
		uy = pts0[j1].y-pts0[j0].y;
		uz = pts0[j1].z-pts0[j0].z;
		vx = pts0[j2].x-pts0[j0].x;
		vy = pts0[j2].y-pts0[j0].y;
		vz = pts0[j2].z-pts0[j0].z;

		xNor = uy*vz-uz*vy;
		yNor = uz*vx-ux*vz;
		zNor = ux*vy-uy*vx;

		if (xNor == 0 && yNor == 0 && zNor == 0) {
			TRACE("BAD NORMAL in hull !!!!!!\n");
			continue;
		}

		// compute plane constant
		c = xNor*pts0[j0].x+yNor*pts0[j0].y+zNor*pts0[j0].z;

		// determine if hull 1 is on same side of plane
		side1 = OnSameSide(xNor,yNor,zNor,c,fCount1,face1,pts1);
		if ( side1 )
		{
			// determine which side of plane hull 0 lies
			side0 = WhichSide(xNor,yNor,zNor,c,fCount0,face0,pts0);
			if ( side0*side1 <= 0 ) { // plane separates hulls
				plane.Set(xNor,yNor,zNor,c);
				return 1;
			}
		}
	}

	// test faces of hull 1 for possible separation of points
	for (i = 0; i < fCount1; i++)
	{
		// lookup edge (assert: j0 != j1 && j0 != j2 && j1 != j2)
		j0 = face1[i][0];
		j1 = face1[i][1];
		j2 = face1[i][2];

		// compute perpendicular to face (assert: (xNor,yNor,zNor) != (0,0,0))
		ux = pts1[j1].x-pts1[j0].x;
		uy = pts1[j1].y-pts1[j0].y;
		uz = pts1[j1].z-pts1[j0].z;
		vx = pts1[j2].x-pts1[j0].x;
		vy = pts1[j2].y-pts1[j0].y;
		vz = pts1[j2].z-pts1[j0].z;
		xNor = uy*vz-uz*vy;
		yNor = uz*vx-ux*vz;
		zNor = ux*vy-uy*vx;


		if (xNor == 0 && yNor == 0 && zNor == 0) {
			TRACE("BAD NORMAL in hull !!!!!!\n");
			continue;
		}

		// compute plane constant
		c = xNor*pts1[j0].x+yNor*pts1[j0].y+zNor*pts1[j0].z;

		// determine if hull 0 is on same side of plane
		side0 = OnSameSide(xNor,yNor,zNor,c,fCount0,face0,pts0);
		if ( side0 )
		{
			// determine which side of plane hull 1 lies
			side1 = WhichSide(xNor,yNor,zNor,c,fCount1,face1,pts1);
			if ( side0*side1 <= 0 ) {  // plane separates hulls
/*
				// normalize 
				float l = sqrt(xNor*xNor + yNor*yNor + zNor*zNor);
				
				if (l != 0.0)  {
					l= 1.0/l; 
					xNor *= l; yNor *=l; zNor*=l;
				}
				c = xNor*pts1[j0].x+yNor*pts1[j0].y+zNor*pts1[j0].z;
*/
				plane.Set(xNor,yNor,zNor,c);
				return 1;
			}
		}
	}
#if 0
	// what if two hulls are on same plane ?
	// don't do the full test 
	return (0);
#endif

	
	// build edge list for hull 0
	EdgeNode* list0 = 0;
	for (i = 0; i < fCount0; i++)
	{
		// lookup face (assert: j0 != j1 && j0 != j2 && j1 != j2)
		j0 = face0[i][0];
		j1 = face0[i][1];
		j2 = face0[i][2];
		AddEdge(list0,j0,j1);
		AddEdge(list0,j0,j2);
		AddEdge(list0,j1,j2);
	}

	// build edge list for hull 1
	EdgeNode* list1 = 0;
	for (i = 0; i < fCount1; i++)
	{
		// lookup face (assert: j0 != j1 && j0 != j2 && j1 != j2)
		j0 = face1[i][0];
		j1 = face1[i][1];
		j2 = face1[i][2];
		AddEdge(list1,j0,j1);
		AddEdge(list1,j0,j2);
		AddEdge(list1,j1,j2);
	}

	// Test planes whose normals are cross products of two edges,
	// one from each hull.
	for (EdgeNode* node0 = list0; node0; node0 = node0->next)
	{
		// get edge
		ux = pts0[node0->v1].x-pts0[node0->v0].x;
		uy = pts0[node0->v1].y-pts0[node0->v0].y;
		uz = pts0[node0->v1].z-pts0[node0->v0].z;

		for (EdgeNode* node1 = list1; node1; node1 = node1->next)
		{
			vx = pts1[node1->v1].x-pts1[node1->v0].x;
			vy = pts1[node1->v1].y-pts1[node1->v0].y;
			vz = pts1[node1->v1].z-pts1[node1->v0].z;

			// compute plane normal
			xNor = uy*vz-uz*vy;
			yNor = uz*vx-ux*vz;
			zNor = ux*vy-uy*vx;

			if (xNor == 0 && yNor == 0 && zNor == 0) {
				TRACE("BAD NORMAL in hull !!!!!!\n");
				continue;
			}

			// compute plane constant
			c = xNor*pts0[node0->v0].x+yNor*pts0[node0->v0].y+zNor*pts0[node0->v0].z;

			// determine if hull 0 is on same side of plane
			side0 = OnSameSide(xNor,yNor,zNor,c,fCount0,face0,pts0);
			side1 = OnSameSide(xNor,yNor,zNor,c,fCount1,face1,pts1);
			if ( side0*side1 < 0 ) { // plane separates hulls
				DeleteList(list0); // hg
				DeleteList(list1);
				plane.Set(xNor,yNor,zNor,c);
				return 1;
			}
		}
	}

	DeleteList(list0);
	DeleteList(list1);

	return 0;
}
//---------------------------------------------------------------------------
// seppt3
//---------------------------------------------------------------------------
int SeparatePointSets3D (
	int n0, const Point pts0[],
	int n1, const Point pts1[],
	Plane &plane)
{

	// construct convex hull of point set 0
	int fCount0;
	Triangle* face0 = 0;
	if ( !ConvexHull3D(n0,pts0,fCount0,face0) )
	{
		delete[] face0;
		return 0;
	}

	// construct convex hull of point set 1
	int fCount1;
	Triangle* face1 = 0;
	if ( !ConvexHull3D(n1,pts1,fCount1,face1) )
	{
		delete[] face0;
		delete[] face1;
		return 0;
	}
	
	int ret=SeparatePointSets3D(n0,pts0,fCount0,face0,
						n1,pts1,fCount1,face1,
						plane);
	delete[] face0;
	delete[] face1;
	return ret;

}
