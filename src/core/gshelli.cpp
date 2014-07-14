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

@module GShellI.cpp - GLView GShellI graphical object |

Copyright (c) 1995 - 1999 by Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:

Changes:

$Revision: 1.28 $
$Log: gshelli.cpp,v $
Revision 1.28  1999/07/06 16:54:54  tom
*** empty log message ***


Todo :

******************************************************************************/

 #include "stdafx.h"
//#include <math.h>
//#include <string.h>
//#include "array.h"
//#include <stdlib.h>
//#include "point.h"

//#include "glh.h"
//#include "gutils.h"

#include "gclass.h"
#include "gshell.h"
#include "gshellop.h"
#include "gshelli.h"

#ifdef _OGL
// for glRender
#include "glrender.h"
#endif



// for	ComputePrimitiveList
#include "tomesh.h"


#include "gkatmai.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// parse RGB color value from a string
int GetColor(const char *c,Point &color);

/******************************************************************************/
// crease Angle support

//
// GVertexFaceEntry
// store information for a face vertex

class GVertexFaceEntry {
public :
		int fi;			// the face index
		int e[2];		// two edges 

		int ni;			// the normal index <0 if not allocated yet
		int ioffset;	// offset for normalIndex list 
		
		GVertexFaceEntry *next; // any other face using the same vertex
		
		GVertexFaceEntry(int faceIndex,int e0, int e1,int indexOffset) : 
		fi(faceIndex),next(NULL),ni(-1),ioffset(indexOffset)  {e[0]=e0,e[1]=e1; }

		gbool NormalAllocated() const { return ni>=0; }

};	

//
// GVertexFace
// helperclass for computing creaseAngle normal indices 
//
class GVertexFace {

public :
	
	int nv; // number of vertices

	float creaseAngleCos;	// the crease angle

	int normalIndex;	// for allocation new normal indices
	
	IntArray faceNormalIndex;	// array for each face, index of facet normal if allocated

	// vertex dictonary, indexed by vertex index
	// gives the list of facets using that vertex

	Array<GVertexFaceEntry*> v;
	
	GVertexFace(int numVerts) {
		initV(numVerts);
		normalIndex = -1;
	}

	~GVertexFace() {
		deleteV();
	}

	// allocate a new vertex normal index
	int NewNormalIndex() { 
		normalIndex++;
		return normalIndex;
	}

	// allocate a new vertex normal index, for face
	int NewFaceNormalIndex(int fi) { 
		if (faceNormalIndex[fi]>=0)
			return faceNormalIndex[fi];
		// allocate
		
		normalIndex++;
		faceNormalIndex[fi] = normalIndex;

		return normalIndex;
	}


	// init v table 
	void initV(int numVerts) 
	{
		nv = numVerts;
		v.SetLength(nv);
		for(int i=0; i<nv;i++) v[i]=NULL;	

	}


	void deleteV() 
	{
	
		  for(int i=0; i<nv;i++)
		  { 	GVertexFaceEntry *lnext;
				for( GVertexFaceEntry *l=v[i]; l != NULL; l = lnext) {
				   lnext = l->next;
				   delete l;
				}
				v[i]=NULL;
		  }
		  v.Flush();
	}

	// add face to vertex
	void addFaceVertex(int vi,int fi,int e0, int e1, int foffset)
	{
		ASSERT(vi<nv);
		ASSERT(vi>=0);

		if (vi>= nv) return;
/* this happens !!! bad IFS ? 
		ASSERT(vi != e0);
		ASSERT(vi != e1);

		ASSERT(e0 != e1);
*/


		GVertexFaceEntry *e= new GVertexFaceEntry(fi,e0, e1, foffset);
		e->next = v[vi];
		v[vi]=e;
	}
	
	// facets a & b share a common vertex
	// do the facets have a common edge too ?
	gbool CommonEdge(GVertexFaceEntry &a,GVertexFaceEntry &b)
	{
		return a.e[0] == b.e[1] ||  a.e[0] == b.e[0]
			   ||  a.e[1] == b.e[1] ||  a.e[1] == b.e[0];
	}

	
	// should we smooth along facet a, b ?
	gbool Smooth(GVertexFaceEntry &a,GVertexFaceEntry &b,const PointArray &fn);
/*
	{
		if (!CommonEdge(a,b)) return gfalse;

		float dot = DotProd(fn[a.fi],fn[b.fi]);
		if (dot >= creaseAngleCos) return gtrue;
		return gfalse;

	}
*/

	// share a normal index between the two facets at that vertex
	gbool ShareIndex(GVertexFaceEntry &a,GVertexFaceEntry &b)
	{
		
		if (a.ni>=0) {
			if (b.ni <0) {
				b.ni = a.ni;
				return gtrue;
			}
			else return gfalse;
		}
		if (b.ni>=0) {
			if (a.ni <0) {
				a.ni = b.ni;
				return gtrue;
			}
			else return gfalse;
		}
		a.ni = b.ni = NewNormalIndex();
		return gtrue;

	}

	// compute the normalindices,
	// fn are the facet normals
	// indices are returned in normalIndex
	int computeVNIndices(int nFaces,const PointArray &fn,IntArray &normalIndex); 


} ; // class 

	gbool GVertexFace::Smooth(GVertexFaceEntry &a,GVertexFaceEntry &b,const PointArray &fn)
	{
		// for some unknow reason we get bad results
		//if (!CommonEdge(a,b))  return gfalse;

		float dot = DotProd(fn[a.fi],fn[b.fi]);
		if (dot >= creaseAngleCos) return gtrue;
		return gfalse;

	}



int GVertexFace::computeVNIndices(int nFaces,const PointArray &fn,IntArray &normalIndex) 
	{
		  faceNormalIndex.Set(nFaces,-1); // mark face normals not allocated 

		  gbool smoothed=gfalse;
		  gbool allFlat = gtrue;
		  gbool allSmoothted = gtrue;
		  
		  for(int i=0; i<nv;i++)
		  { 	
			GVertexFaceEntry *a;
			a=v[i];
			
			if (a == NULL) continue; // unused vertex 
		

			// find sharing pairs 
			for( a=v[i]; a != NULL; a = a->next) {
				for( GVertexFaceEntry *b= a->next; b != NULL; b = b->next) 
				{
					if ( /* a!= b && */ Smooth(*a,*b,fn)) { // a,b should be smooth at vertex i
						ShareIndex(*a,*b);
						smoothed=gtrue;
						allFlat = gfalse;
					}

				}

			}
			// if facet not smoothed 
			for (a=v[i]; a != NULL; a = a->next) {
				if (a->ni <0) { // no sharing, use face normal 
					a->ni = NewFaceNormalIndex(a->fi);
					if (a->next == NULL && a==v[i]) { // border vertex not shared
					}
					else allSmoothted=gfalse;
				}					
				normalIndex[a->ioffset]=a->ni;
			}

		} // nv 

		if (allFlat) return -1;

		if (!allSmoothted) {
			TRACE("crease all smooth %d all flat %d \n",allSmoothted,allFlat);
		}
	
		//if (allSmoothted) 
			//return 0;

		return 1;
	};



// compute the face normal 
inline // test 

int compute_polygon_plane_index1(int nverts, const long vindex[],
						const Point *verts, Plane *plane)
{
		int i;
		Point refpt;
		Point normal;

		const Point *u, *v;
		float /*double*/ len;

		/* compute the polygon normal and a reference Point on
		   the plane. Note that the actual reference Point is
		   refpt / nverts
		*/
		normal.Zero();
		refpt.Zero();

		for(i = 0; i < nverts; i++) {
			u = &verts[vindex[i]];
			v = &verts[vindex[(i + 1) % nverts]];
			normal.x += (u->y - v->y) * (u->z + v->z);
			normal.y += (u->z - v->z) * (u->x + v->x);
			normal.z += (u->x - v->x) * (u->y + v->y);
			refpt+= *u;
		}
		/* normalize the polygon normal to obtain the first
		   three coefficients of the plane equation
		*/
		len = normal.Len();
		if (len == 0.0) len = 1.0f;
		else len = 1.0f / len;
		plane->n.x = normal.x * len;
		plane->n.y = normal.y * len;
		plane->n.z = normal.z * len;
		/* compute the last coefficient of the plane equation */
		len *= nverts;
		plane->d = -DotProd (refpt, normal) * len;  // <<<< check this wrong !!!!!
		return(1);
}

/*
	computeNormalIndices
	compute crease angle based vertexNormal indices
	giving as input
	the coordIndex , coord, creaseAngle Data from a VRML IndexedFaceSet
	Indices will be returned in normalIndex.
	fn will contain computed face normals 

*/

int computeNormalIndices(int nci, const long *cind,
						 int ncoords, const Point *coords,
						 float creaseAngle,
						 gbool ccw,
						 PointArray &fn,IntArray &normalIndex)
{
	const long *ciend = cind + nci;
	const long *ci; // coordIndex stepping point 

	const long *fstart; // where last face has started 
	int fi;		// face index
	int flen;	// length of face

	Plane plane;
	
	GVertexFace vtable(ncoords);
	
	vtable.creaseAngleCos = cos(creaseAngle);

	// setup vertex table 	
	ci = cind;
	fstart = ci;
	fi =0; // current face index
	flen = 0;


	while (ci < ciend) { // for all faces 
		int vi= *ci++;

		if ((vi<0) || (ci>=ciend)) { // new face

			if (flen>=2) {

				int e0 = fstart[flen-1]; // last vertex 
			
				if (e0 == fstart[0]) { // duplicated end vertex ?
					flen--;
					e0 = fstart[flen-1];
					// ASSERT(0); // diagnostic only
				}

			}

			
			if (flen>2) {

			// compute face normal
			compute_polygon_plane_index1(flen,fstart,coords,&plane);
			
			if (fi >= fn.Length()) fn.SetLength(fi+1);
	
			if (ccw)
				fn[fi]=plane.n;
			else fn[fi]=-plane.n;


			

			// create all vertex-face table entrys
			int e0 = fstart[flen-1]; // last vertex 

			for(int i = 0; i < flen; i++) {
				int e1 = fstart[(i + 1) % flen];
				vtable.addFaceVertex(fstart[i],fi,e0,e1,fstart-cind+i);
				e0 = fstart[i];
			}
			} // flen > 2
			else {
				// add dummy to allocate face normal 
				for(int i = 0; i < flen; i++) {
					vtable.addFaceVertex(fstart[i],fi,-1,-1,fstart-cind+i);
				}

			}


			// prepare for next face 
			fi++;
			flen = 0;
			fstart = ci;
		}
		else {
			flen++;

		}

	}

	normalIndex.Set(nci,-1);

	// pass 2 look at faces at vertex & allocate normal indices
	int ret = vtable.computeVNIndices(fi,fn,normalIndex);

	return ret;
}


/******************************************************************************/







//
// GFaceGroup
//


#ifdef _3DV
// IO : input/output
int GFaceGroup::io(AStream &a)
{
		a.iostr(name);
		//if (a.tok("flags")
		a.io(flags);
		//if (a.tok("color")
		if (a.isInput()) {

		  color.x=-1.0;

		  if (a.Skip() == '"') {
			  char tok[256];
				  a.iostr(tok);
				  GetColor(tok,color);
		  }
	  else	color.io(a);
		  if (color.x>=0.0) flags |= COLOR;
		 }
		else	color.io(a);
		a.nl();
		return(a.stat);
}
#endif _3DV


//
// GShellI
//
// to do: faceinfo fi &holes, glPrimitives..

RTIMP(GShellI,0,GShell);

// constructor
GShellI::GShellI() 
{
}

GShellI::GShellI(int new_nv,const Point *new_v,
				int new_nf,const int *new_f,gbool normalsPerFace)
				: GShell(new_nv,new_v,new_nf,new_f)
{

   if (!normalsPerFace) {
		verts.SetLength(new_nv);

		for(int i=0; i< new_nv; i++) {
			GVertexI &v=verts[i];
			v.v = v.n = v.p = v.c = i;
		}
		
   } else {
	int *fp=f.DataRW();
	int fi=-1;

	int *fpend = fp + f.Length();
	GVertexI vert;
	verts.SetMaxLength(f.Length() *0.7);

	while (fp<fpend) {
	  int n = *fp++;
	  if (n<0) n = -n;
	  else	fi++;				  /* Face index  to do: count holes ? */
  	  while (n>0) {
			vert.v = *fp;
			vert.n = fi; // vertex normal = face index
			vert.c = vert.p = vert.v;
			*fp = verts.Length();
			verts.Append(vert);
			fp++;
			n--;
	  }		
	}
   }
   SetEditFlag(SHELL_VERTICES_CHANGED | SHELL_VERTS_CHANGED);
}

	 
 // construct by mesh 
GShellI::GShellI(int cols,int rows,const Point *new_v,gbool normalsPerFace) :
		GShell(cols,rows,new_v)
{
   int new_nv = v.Length();
   if (!normalsPerFace) {
		verts.SetLength(new_nv);

		for(int i=0; i< new_nv; i++) {
			GVertexI &v=verts[i];
			v.v = v.n = v.p = v.c = i;
		}
		
   } else {
	int *fp=f.DataRW();
	int fi=-1;

	int *fpend = fp + f.Length();
	GVertexI vert;

	verts.SetMaxLength(f.Length() *0.7);

	while (fp<fpend) {
	  int n = *fp++;
	  if (n<0) n = -n;
	  else	fi++;				  /* Face index  to do: count holes ? */
  	  while (n>0) {
			vert.v = *fp;
			vert.n = fi; // vertex normal = face index
			vert.c = vert.p = vert.v;
			*fp = verts.Length();
			verts.Append(vert);
			fp++;
			n--;
	  }		
	}
   }
   SetEditFlag(SHELL_VERTICES_CHANGED | SHELL_VERTS_CHANGED);

}


// destructor
GShellI::~GShellI()
{

}


// Check for Recomputation of cached values (e.g. Normals), 1 if redraw
int GShellI::Update() 
{
  if (edit_flags & SHELL_VERTS_CHANGED) 
     SetAction(SHELL_COMPUTE_MAXI);

  int ret = GShell::Update();
  return(ret);
}

// set vertices
void
GShellI::SetVertices(int new_nv,const GVertexI *new_v)
{
   if (new_nv != verts.Length()) edit_flags |= SHELL_VERTS_LENGTH_CHANGED;
   verts.Set(new_nv,new_v);
   SetEditFlag(SHELL_VERTICES_CHANGED | SHELL_VERTS_CHANGED);
}

void
GShellI::SetVertices(const Array<GVertexI> &new_v)
{
   if (new_v.Length() != verts.Length()) edit_flags |= SHELL_VERTS_LENGTH_CHANGED;
   verts.Set(new_v);
   SetEditFlag(SHELL_VERTICES_CHANGED | SHELL_VERTS_CHANGED);
}

// Get Vertex Data by Value
// if value arrays are not present they will not be stored
int GShellI::GetVertex(int i, GVertex &vert)
{
	if (i >= verts.Length())  {
		TRACE("Warning GShellI::GetVertex(%d ) out of range ! \n",i);
	   return(-1);
	}

	const GVertexI &vi = verts[i];

	if (vi.v < v.Length())	vert.v = v[vi.v];	// the vertex coordinates
	if (vi.n < vn.Length()) vert.n = vn[vi.n];	// the vertex normal
	if (vi.c < vc.Length()) vert.c = vc[vi.c];	// the vertex color
	if (vi.p < vp.Length()) vert.p = vp[vi.p];	// the vertex parameter
	return(0);
}


// copy go GShell version
void GShellI::UpdateSubMeshVertices(int what)
{
	GSubMesh *m=subMeshes;

	 if (!Flat()) {
			const PointArray& tmp = GetVN(); // make shure vertex normals are up to date
	 }

	GVertex vv;

	while (m) {
		int l = m->vmap.Length();
		Point *mv = m->s->v.DataRW();
		Point *mvn = m->s->vn.DataRW();
		Point *mvc = m->s->vc.DataRW();
		Point *mvp = m->s->vp.DataRW();
		ASSERT(l == m->s->v.Length());   // for gshell

		for(int i=0; i<l;i++) {
#if 0
			this->GetVertex(m->vmap[i],vv);
			
			ASSERT(i < m->s->v.Length());   // for gshell

			m->s->SetVertex(i,vv);
#else
			// optimized case 
			int vii=m->vmap[i];
			const GVertexI &vi = verts[vii];

			ASSERT(i < m->s->v.Length());   // for gshell
/*
			if (vi.v < v.Length())	vv.v = v[vi.v];	// the vertex coordinates
			if (vi.n < vn.Length()) vv.n = vn[vi.n];	// the vertex normal
			if (vi.c < vc.Length()) vv.c = vc[vi.c];	// the vertex color
			if (vi.p < vp.Length()) vv.p = vp[vi.p];	// the vertex parameter
			//m->s->SetVertex(i,vv);
*/			
/*
			if (i < m->s->v.Length())  m->s->v[i] = v[vi.v];			// the vertex coordinates
			if (i < m->s->vn.Length()) m->s->vn[i] = vn[vi.n];	// the vertex normal
			if (i < m->s->vc.Length()) m->s->vc[i] = vc[vi.c];	// the vertex color
			if (i < m->s->vp.Length()) m->s->vp[i] = vp[vi.p];  // the vertex parameter
*/
			*mv++ = v[vi.v];		// the vertex coordinates
			if (mvn) *mvn++ = vn[vi.n];	// the vertex normal
			if (mvc) *mvc++ = vc[vi.c];	// the vertex color
			if (mvp) *mvp++ = vp[vi.p]; // the vertex parameter

#endif

		}
		m->s->SetEditFlag(SHELL_VERTICES_CHANGED);


		// face colors
		if (fc.Length()>0) {
		 
			int i=m->faceEnd-m->faceStart +1;
		 
			if (i >0) {
				m->s->fc.SetLength(i);
				Point *mfc = m->s->fc.DataRW();
		 
				if (mfc)
				for(int i=m->faceStart; i<=m->faceEnd;i++) {
					 *mfc++=fc[i];
				}
				m->s->SetEditFlag(SHELL_FACE_ATTRIBUTES_CHANGED);
			}

		}


		m->s->Update();

		/* if (vn.Length()>0)*/  
		m->s->actions &= ~(SHELL_COMPUTE_VNORMALS);

		m = m->next;
	}
}



// compute required length of v,p,n c arrays

int GShellI::ComputeMaxVertexI( GVertexI &vert)
{

	if (verts.Length() == 0) {
		vert.v = 0;
		vert.n = 0;
		vert.c = 0;
		vert.p = 0;
		return(0);
	}
	const GVertexI *vp = &verts[0];
	const GVertexI *vpEnd = &verts[0] + verts.Length();

	vert.v = vp->v;
	vert.n = vp->n;
	vert.c = vp->c;
	vert.p = vp->p;
	vp++;
	while (vp <vpEnd) {
		if (vert.v < vp->v) vert.v = vp->v;
		if (vert.n < vp->n)	vert.n = vp->n;
		if (vert.c < vp->c)	vert.c = vp->c;
		if (vert.p < vp->p)	vert.p = vp->p;
		vp++;
	}
	vert.v +=1;
	vert.n +=1;
	vert.c +=1;
	vert.p +=1; 
	return(0);
}

// result 0 not planar, 1: shell is planar, plane computed, 2: plane of first poly
int GShellI::isPlanar(Plane &plane)
{
    if (f.Length() <= 3) return 0;
	int vi=f[1];

	const PointArray& fn = GetFN();
	if (fn.Length()==0) return (0);

	plane.Set(v[verts[vi].v],fn[0]);
    
	if (nfaces == 1 || ((flags & SHELL_PLANAR) !=0) ) return(1);

	return(nfaces);

}





// to do : inline
int GShellI::ExtractFace(int flen, const int *fp, int *outfp)
{
  while (flen >0) {
		*outfp ++ = verts[*fp++].v;
		flen--;
  }
  return(0);
}


////////////////////////////////////////////////////////////////////////////////

// class  GVertexITable 

// initialize vertex table
GVertexITable::GVertexITable(Array<GVertexI> &Verts,
			int NV /* max vert.v used as v table key */)
			: verts(Verts) 
		{
			nv = NV;
			v = new GVertexIEntry *[nv];
			for (int i=0; i<nv; i++) v[i] = NULL;
			nverts=0;
			ASSERT(verts.Length() == 0);
			// to do:Enter all inital verts elements to table ?
}

GVertexITable::~GVertexITable() 
{
		  for(int i=0; i<nv;i++)
		  { 	GVertexIEntry *lnext;
				for( GVertexIEntry *l=v[i]; l != NULL; l = lnext) {
				   lnext = l->next;
				   delete l;
				   l = lnext; // hg 20.09.96
				}
				v[i]=NULL;
		  }
		  delete [] v;
		  v=NULL;
		}

// Enter Vertex vert into verts table, 
// Return index of vert if inserted,
// or -1 if enter == 0 and vert not found
int GVertexITable::Lookup(GVertexI &vert,int enter)
		{
				GVertexIEntry *l=v[vert.v];				 // get edgelist for vertex e->v1
				if (l == NULL) { 
					if (!enter) return -1;
					GVertexIEntry *vi = new GVertexIEntry(verts.Length());
					verts.Append(vert);	
					v[vert.v] = vi; nverts++;  // 15.09.96
					return(vi->vi); 
				}
				else {
						while (l != NULL) {
						  // exact match ? return index
						  if (verts[l->vi] == vert) return(l->vi);		// edge already in

						  if (l->next == NULL) {
							   // not found
							if (!enter) return -1;
							GVertexIEntry *vi = new GVertexIEntry(verts.Length());
							verts.Append(vert);	
							l->next = vi; nverts++; 
							return(vi->vi); 

						  }
						  else l=l->next;

						} // while
				} // else
		   return(-1); // should not happen
}



// append a single GVertexI, optionally with lookup
int GShellI::AppendVertex(const GVertexI &p,int doLookup)
{	int newVi=-1;
	if (doLookup) {
	   //newVi = verts.Search(p);

		newVi = -1;
		register const GVertexI* src = verts ;

		int i= verts.Length();
		if (i>0)
		do {
			if ( src->v == p.v && src->n == p.n)
				{ newVi = src-verts; break; }
				  src ++;
				  i--;
		} while ( i > 0);

	}

	if (newVi == -1) { // Append new
	   newVi = verts.Length();
	   verts.Append(p);
	   SetEditFlag(SHELL_VERTICES_CHANGED | SHELL_VERTS_LENGTH_CHANGED);
	}
	return (newVi);
}


// append a single vertex given by a single point, optionally with lookup
int GShellI::AppendVertex(const Point &p,int doLookup)
{  GVertexI v;
   v.v = AppendV(p,doLookup);
   // assign an index, and resize arrays
   v.n = v.v; if ( v.n >= vn.Length() && vn.Length()>0) vn.SetLength(v.n+1);
   v.p= v.v; if ( v.p >= vp.Length() && vp.Length()>0) vp.SetLength(v.p+1);
   v.c = v.v; if ( v.c >= vc.Length() && vc.Length()>0) vc.SetLength(v.c+1);

   return this->AppendVertex(v,doLookup);
}
int GShellI::AppendVertex(const GVertex &vert) 
{
   GVertexI vi;
   vi.v=v.Length();
   v.Append(vert.v);

   if (vn.Length()>0) { vi.n=vn.Length(); vn.Append(vert.n); } else vi.n = vi.v; // correct ?? 
   if (vc.Length()>0) { vi.c=vc.Length(); vc.Append(vert.c); } else vi.c = vi.v; // ??
   if (vp.Length()>0) { vi.p=vp.Length(); vp.Append(vert.p); } else vi.p = vi.v; //
   
   SetEditFlag(SHELL_VERTICES_CHANGED | SHELL_VERTS_LENGTH_CHANGED );
   return this->AppendVertex(vi,0);
}


// compute vertex colors for each vertex from attached faces face color 
int
GShellI::ComputeVertexColorsFromFaceColors()
{
   if (f.Length() <= 0) return(0);

   if (nfaces <=0) {
	   nfaces= GetFaceCount();
   }

   if (nfaces <=0) return(-1);

   // need face normals for computation
   const PointArray &fc = GetFC();
   
   if (fc.Length() == 0) return(-1);


   if (vc.Length() < verts.Length()) vc.SetLength(verts.Length()); 
   if (vc.Length() == 0) return(-1);
   vc.UnRef();


   IntArray vcnt(verts.Length()); // counters of color

   for(int i=0; i<verts.Length();i++) {
	   verts[i].c=i;
   }	

   // Zero vc array
   { Point *p = vc.DataRW(),*pend = &p[vc.Length()];
     int *pcnt = vcnt;
   	 while (p < pend) {	p->Zero();	p++; *pcnt++=0; }
   }


   const int *fp = f;	  // face list stepping pointers
   const int *fpend= fp+f.Length();
   int fi=-1;	// face index
   int vLen = verts.Length();


   do { // for all faces
		 int flen = *fp ++; // length of this face
		 if (flen<0) flen=-flen;   
		 else fi++;

		 int i;
		 for (i=0; i< flen; i++) {
				int vi = *fp++; // vertex index
				if (vi<vLen) {  // if to large ignore
					vc[vi] += fc[fi];	// add face color to vc slot
					vcnt[vi]++;
				}
		 }
   } while (fp <fpend);

   // Normalize vc array
   { Point *p = vc.DataRW(),*pend = &p[vc.Length()];
     const int *pcnt = vcnt;
     while (p < pend) {
				if (*pcnt>0) {
					*p *= 1.0 / (float) *pcnt; 
				}
				p++;
				pcnt++;
    }
   }
   SetEditFlag(SHELL_VERTEX_ATTRIBUTES_CHANGED);
   return(1);
}

// compute face colors for each face from average of all face vertex colors 
int
GShellI::ComputeFaceColorsFromVertexColors()
{
   if (f.Length() <= 0) return(0);

   if (nfaces <=0) {
	   nfaces= GetFaceCount();
   }

   if (nfaces <=0) return(-1);

   // need vertex colors for computation
   const PointArray &vc = GetVC();
   
   if (vc.Length() == 0) return(-1);


   if (fc.Length() < nfaces) fc.SetLength(nfaces); 
   if (fc.Length() == 0) return(-1);
   fc.UnRef();

   Point fcsum;
   int fccnt;		// counters of color

   const int *fp = f;	  // face list stepping pointers
   const int *fpend= fp+f.Length();
   int fi=-1;	// face index
   int vLen = verts.Length();


   do { // for all faces
		 int flen = *fp ++; // length of this face
		 if (flen<0) flen=-flen;   
		 else { fi++;
			fccnt=0;
			fcsum.Zero();
		 }

		 int i;
		 for (i=0; i< flen; i++) {
				int vi = *fp++; // vertex index
				if (vi<vLen) {  // if to large ignore
					fcsum += vc[verts[vi].c];	// add vertex color to sum 
					fccnt++;
				}
		 }
		 if (fccnt>0) {
			fc[fi] = fcsum * 1.0 / (float) fccnt;
		 }
		 else fc[fi].Set(1,1,1);

   } while (fp <fpend);
 
   SetEditFlag(SHELL_FACE_ATTRIBUTES_CHANGED);
   return(1);
}




////////////////////////////////////////////////////////////////////////////////
// Helping class for computing set of edges for shell
class Edge {
public:
		int v1,v2;				// ref to vertex 1 and vertex 2, v1 <= v2
//        int f;                  // ref to face index for color 
		Edge *next;

		Edge (int V1,int V2,Edge *NEXT=NULL) {
						if (V1 < V2) {v1 = V1; v2 =V2;}
						else { v2 = V1; v1 = V2; }
						next = NEXT; }

};


class EdgeTable {
public :
		int nv;
		Edge * *v;
		int nedges;

		// initialize edge table, table size is number of vertices
		EdgeTable(int NV) { nv = NV;
												v = new Edge * [nv];
												for (int i=0; i<nv; i++) v[i] = NULL;
												nedges=0;
										   }

		~EdgeTable() {
		  // printf("Destructing Edgetable \n");
		  for(int i=0; i<nv;i++)
		  { 	Edge *lnext;
				for( Edge *l=v[i]; l != NULL; l = lnext) {
				   lnext = l->next;
				   delete l;
				}
				v[i]=NULL;
		  }
		  delete [] v;
		  v=NULL;
		}

		// Enter Edge e into Edgetable, Return 1 if inserted, 0 if already inside
		int Enter(Edge *e)
		{

				Edge * l=v[e->v1];				 // get edgelist for vertex e->v1

				if (l == NULL) { v[e->v1] = e; nedges++; return(1); }
				else {
						while (l != NULL) {
						  if (l->v2 == e->v2) return(0);		// edge already in

						  if (l->next == NULL) {
								  l->next = e;
								  nedges++;
								  return(1);
						  }
						  else l=l->next;

						} // while
						}
		   return(0);
		}
		int Enter(int v1,int v2) {
				int ret;
				Edge *e= new Edge(v1,v2);
				if ( (ret=Enter(e)) == 0) delete e;
				return(ret);
		}

		// allocate and assign list int-array with edges
		int *GetEdgeList(int *Nedges)
		{ int *e,*ep;

		  e = ep = new int [nedges*2];
		  for(int i=0; i<nv;i++)
		  {
				for( Edge *l=v[i]; l != NULL; l = l->next) {
				   *ep++ = l->v1;
				   *ep++ = l->v2;

		  }
		}
		*Nedges = (ep -e ) / 2;
		return(e);
   }
};

// Problem using logical edges, or geometry edges ??????????????????????
// compute the list of edges for all polygons and store them into e
int
GShellI::ComputeEdgeList()
{
   const int *fp =f;			   // face list stepping pointers
   const int *fpend= fp+f.Length();
   int fi=0;	// face index


//	 printf("Computing edge list \n");
   if (f.Length() <= 0) return(0);

   EdgeTable t(verts.Length()); 		// use length of verts array, not v


   do { 		// for all faces
		 int flen = * fp ++;	// length of this face
		 if (flen<0) flen=-flen;

		 int i,v0;
		 v0 = fp[flen-1];				// last vertex

		 for (i=0; i< flen; i++) {
				int vi = *fp++; // vertex index
				t.Enter(v0,vi); 		// enter edge into edgetable
				v0=vi;
		 }

		 fi++ ;
   }
   while (fp < fpend);

   { int *elist;
		 int nedges;

		 elist = t.GetEdgeList(&nedges);
		 SetEdgeListByRef(nedges,elist);
   }

   return(1);
}




// compute face normals for each polygon and store them into fn
void
GShellI::ComputeFaceNormals(int allocFaceinfo)
{
   const int *fp =f;			   // face list stepping pointers
   const int *fpend= fp+f.Length();
   int fi=0;	// face index

   int fbuffer[2048];  // to do: max or dynamic

   Plane plane;

   UnSetAction(SHELL_COMPUTE_FNORMALS);

   if (f.Length() <= 0) return;

   if (nfaces <=0) {
		   nfaces= GetFaceCount();
   }

   if (nfaces <=0) return;

   if (fn.Length() < nfaces) fn.Resize(nfaces);
   fn.UnRef();

   if (allocFaceinfo && (finfo == NULL))
		  finfo = new shell_face_info [nfaces];

   do { 		// for all faces
		 int flen = * fp ++; // length of this face
		 int ishole=0;
		 if (flen<0) { ishole=1; flen=-flen; }

	 	 // expanded with check ExtractFace(flen,fp,fbuffer); 17.05.96
         for (int i= 0; i<flen; i++) {
            int o = fp[i];
            if (o>=verts.Length()) {
                TRACE("GShellI::ComputeFaceNormals bad vertex number %d >= %d \n",o,verts.Length());       
                o=0;
            }    
		    fbuffer[i]= verts[o].v;
         }


		 compute_polygon_plane_index(flen,fbuffer,v,&plane);

		 if (fn.Length()) {
				fn[fi]=plane.n;
		 }
		 if (finfo != NULL) {
				int poly_u,poly_v;
				finfo[fi].plane_d = plane.d;
				// find principal axes of face
				if (fabs(plane.n.x) > fabs(plane.n.y)
						&& fabs(plane.n.x) > fabs(plane.n.z)) {
						poly_u = 1 ;
						poly_v = 2 ;
				} else if (fabs(plane.n.y) > fabs(plane.n.x)
						&& fabs(plane.n.y) > fabs(plane.n.z)) {
						poly_u = 0 ;
						poly_v = 2 ;
				} else {
						poly_u = 0 ;
						poly_v = 1 ;
		}
		finfo[fi].poly_u=poly_u;
		finfo[fi].poly_v=poly_v;
		 }

		 fp += flen;
		 fi++ ;
   }
   while (fp < fpend);
   SetFlag(SHELL_FNORMALS_COMPUTED);
   if (NormalsFlipped()) fn *= Point(-1.0,-1.0,-1.0);  // flip normals
}

// compute vertex normals for each face vertex and store them into vn
int
GShellI::ComputeVertexNormals()
{

   //printf("Computing vertex normals \n");
   
   UnSetAction(SHELL_COMPUTE_VNORMALS);
   
   if (f.Length() <= 0) return(0);

   if (nfaces <=0) {
		   GetFaceCount();
   }

   if (nfaces <=0) return(-1);

   // need face normals for computation
   const PointArray &fnormals = GetFN();
   
   if (fnormals.Length() == 0) return(-1);

   const GVertexI &maxIndices = GetMaxI();

   if (vn.Length() < maxIndices.n) vn.SetLength(maxIndices.n);
   if (vn.Length() == 0) return(-1);
   vn.UnRef();

#ifdef _KATMAI_xx

   if (useKatmai) {
	   ASSERT(sizeof(GVertexI) == sizeof(KGVertexI));

	KatmaiComputeVertexNormals(f,f.Length(),(KGVertexI *)verts.Data(),fn,vn.Length(),vn);
   } 
   else
#endif 
   {	
   vn.Set(vn.Length(),Point(0.0,0.0,0.0)); // zero out Vertex normals

   const int *fp = f;			   // face list stepping pointers
   const int *fpend= fp + f.Length();

   int fi=0;	// face index
   
   do { 		// for all faces
		 int flen = *fp ++; // length of this face
		 if (flen<0) flen=-flen;
		 const Point &fn_ = fn[fi];	

		 for (int i=0; i< flen; i++) {
				
				int vi = *fp++; // vertex index
				
				ASSERT(vi < verts.Length());
				ASSERT(verts[vi].n < vn.Length());
				// crash here, vi to large ?????? 

				vn[verts[vi].n] += fn_;	// add face normal to vn slot
		 }

		 fi++ ;
   } while (fp <fpend);

   // Normalize vn array
   { Point *p = vn.DataRW(),*pend = &p[vn.Length()];
		while (p < pend) {
				p->Normalize();
				p++;
		}
   }
   }
   SetFlag(SHELL_VNORMALS_COMPUTED);
   return(1);
}

#ifndef _OGL
//triangulate a facelist into triangles using
extern int tesselateFaceList( GFaceList &f, //@parm input facelist
						PointArray &v, //@parm corresponding vertices for indices in f
						IntArray *vmap, // @parm optional vertex map list
						GFaceList &triangles, //@parm output: triangulated facelist
						IntArray *new2old,	//@parm optional mapping from new triangles to old face indices
                        GShell * theShell  //@parm optional shell to append vertices too
					  );
#endif


int
GShellI::ComputeTriangles(GFaceList &tri,IntArray &tri2face,int useTriangulator)
{ // same code as in GShell, ony map argument for triangulator
  // already triangulated
  if (flags & SHELL_TRIANGULATED) {
	 tri = f;
	 tri2face.SetLength(GetFaceCount());
	 for (int i=0; i <tri2face.Length(); i++) tri2face[i]=i;
	 return 0;
  }

#ifdef _OGL
  if (useTriangulator || (flags & SHELL_HAS_HOLE_FACES) || (flags & SHELL_HAS_CONVEX_FACES)) {
  
	IntArray vmap(verts.Length());
    int voldLen = v.Length();
	for (int i=0; i<verts.Length(); i++) vmap[i] = verts[i].v;
  	tesselateFaceList(f,v,&vmap,tri,&tri2face,this);
    int vnewLen = v.Length();
    if (voldLen < vnewLen) {
            TRACE("V list expanded \n");

    }
	return (tri.Length() / 4); // return # of triangles
  }
#else 
  if (useTriangulator || (flags & SHELL_HAS_HOLE_FACES) || (flags & SHELL_HAS_CONVEX_FACES)) {
  
	IntArray vmap(verts.Length());
    int voldLen = v.Length();
	for (int i=0; i<verts.Length(); i++) vmap[i] = verts[i].v;
  	tesselateFaceList(f,v,&vmap,tri,&tri2face,this);
    int vnewLen = v.Length();
    if (voldLen < vnewLen) {
          TRACE("V list expanded \n");

    }
	return (tri.Length() / 4); // return # of triangles
  }
#endif


  const int *fp=f;
  int fi;

  const int *fpend = fp + f.Length();

  tri.SetMaxLength(f.Length()*3);
  tri.SetLength(0);
  tri2face.SetMaxLength(f.Length());
  tri2face.SetLength(0);


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

				//TRACE("IN %d %d  %d (Face %d) \n",p1,p2,p3,fi);
				tri.AppendTri(p1,p2,p3);
				tri2face.Append(fi);
				p2=p3;
		 }
	   } else fp+=n;

		
	}
	return (tri.Length() / 4); // return # of triangles
}




// Evaluate object as object of type "type"
// if return = this, don´t destroy result, otherwise destroy
GEvalAs_TYPE*
GShellI::EvalAs(TypeId type,int copyAttributes) 
{
  if (type == Id()) return(this); // already indentical
#if 0
  if (IsA(type,this.Id())) {
	  // is of derived types
	  return(this); // or copy ??
  }
#endif  
  if (type == GShell::ClassId()) {
	 GShell *s = new GShell();
	 ComputeShell(s,copyAttributes);
	 return(s);
  }
  // else cannot convert
  return(NULL);
}



//////////////////////////////////////////////////////////////////////////////

#ifdef _OGL
inline void glNormalFlip(const Point &p) { glNormal3f(-p.x,-p.y,-p.z); }

#define OUTVERT(I) { \
				   GVertexI &vert = verts[I]; \
				   if (VC) glColor(vc[vert.c]); \
				   if (VP) glTexCoord(vp[vert.p]); \
				   if (VN) if (flipNormals) glNormalFlip(vn[vert.n]); else glNormal(vn[vert.n]);		 \
				   glVertex(v[vert.v]); }

const  int GL_MAX_LINES=256;	// Windows crashes if to many lines send

void GShellI::RenderGlEdges(RenderState &state,int VN,int VC,int VP)
{
   gbool flipNormals = state.frontFaceCW && !state.twosidedLighting;

   if (VN) if (vn.Length() == 0)		VN =0;
   if (VC) if (vc.Length() == 0)		VC =0;
   if (VP) if (vp.Length() == 0)		VP =0;

   int FC = VC ? 0 : fc.Length() != 0;	// face colors if no vertex colors

   if (FC) { // edgetable doesn´t support FACE colors
       // render as outline polygon 16.06.96
       // to do: better put edge face color table index into edge table
       // same for GShellI
       glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
            RenderGlFaces(state,0 /*FN*/ ,VN,1,VP);
       glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
       return; 
   }


   if (!(flags & SHELL_EDGES_OK)) {
		  ComputeEdgeList();
   }
   if (ne == 0) return;

   int *ep=e;
   int *epend = e + ne*2;

#if 0
   // Render as special objects ?
   if (state.edgeDrawStyle && state.edgeDrawStyle->enabled) {

				Point *p1,*p2,*n1=NULL,*n2=NULL;
				Point z(0,0,1);

				if (vn.Length()==0)  n1,n2= &z;

				 while (ep <epend)
				{
						int i;
						// 1st vertex of edge
						i= *ep++;
						p1= &v[i];
						//if (VC) c1 = &vc[i];
						if (VN) n1 = &vn[i];

						// 2nd vertex of edge
						i= *ep++;
						p2= &v[i];
						//if (VC) c2 = &vc[i];
						if (VN) n2 = &vn[i];

						state.edgeDrawStyle->RenderEdge(*p1,*n1,*p2,*n2);
						if (state.Abort()) break;
				}
				return;
   }

#endif


   glBegin(GL_LINES);
   int cnt=GL_MAX_LINES;
   while (ep <epend)
   {
		 int i;
		 i= *ep++;
		 OUTVERT(i);
		 i= *ep++;
		 OUTVERT(i);
		 cnt --;
		 if (cnt <=0) {
				cnt = GL_MAX_LINES;
				glEnd();
				glBegin(GL_LINES);
		 }

  }
  glEnd();
}



void GShellI::RenderGlFaces(RenderState &state,int FN, int VN, int VC, int VP)
{

   int lightModel = state.lightmodel;
   gbool flipNormals = state.frontFaceCW && !state.twosidedLighting;

   // need face normals for computation
   if (FN) {
   		 const PointArray &n=GetFN();
		 if (n.Length() == 0) FN=0; /* return(-1); */
   }

   if (VN) {
    	 const PointArray &n = GetVN();
		 if (n.Length() == 0) VN =0; //return(-1);
   }

   int FC = 0;
   if (VC) {
       if (vc.Length() == 0) {
           VC =0;
           if (fc.Length() > 0) FC = 1;	// face colors if no vertex colors 28.06.96
       } else {
	     state.SetLightModel(GL_SMOOTH); // need smooth lighting to see vertex colors
       } 
   }    

   if (VP) if (vp.Length() == 0) 	VP =0;


#if 0
   if ((prims.Length() > 0) && !FN) {
		  RenderGlPrimitives(state,FN,VN,VC,VP);
		  return;
   }
#endif


   int *fp =f;					// face list stepping pointers

   if (f.Length() ==0) return;	// no faces
   int *fpend= fp+f.Length();	// points after last element of facelist
   int fi=0;					// face index

   int mode=0;	// 1 ==  triangles

   do { 		// for all faces

		 int flen = *fp++; // length of this face
		 if (flen<0) flen=-flen;

		 int i;

		 if (flen==3) { 		// TRIANGLE
				if (mode ==0 ) {
				   glBegin(GL_TRIANGLES);
				   //TRACE("glBegin(GL_TRIANGLES)\n");
				   mode = 1;
				 }
				 {
				   i= *fp++;
				   if (FN) if (flipNormals) glNormalFlip(fn[fi]); else glNormal(fn[fi]);
				   if (FC) glColor(fc[fi]);
				   OUTVERT(i);
				   i= *fp++;
				   OUTVERT(i);
				   i= *fp++;
				   OUTVERT(i);
				 }
		 }
		 else { 				// generic convex polygon
		 if (mode == 1) {		 // close last primitive
				 glEnd();
				 //TRACE("glEnd(GL_TRIANGLES)\n");
				 mode = 0;
		 }
		 glBegin(GL_POLYGON);
		 if (FN) if (flipNormals) glNormalFlip(fn[fi]); else glNormal(fn[fi]);
		 if (FC) glColor(fc[fi]);
		 for (i=0; i< flen; i++) {
				 int vi = *fp++; // vertex index
				 OUTVERT(vi);
		 }
		 glEnd();

		 }								// end convex polygon
		 fi++ ;
   }
   while (fp < fpend);

   if (mode == 1) {
				 glEnd();
				 //TRACE("glEnd(GL_TRIANGLES)\n");
				 mode = 0;
   }
   state.SetLightModel(lightModel);

}

#endif // _OGL


#ifdef _D3D


//-------------------------------------------------------------------------------------------
//		int GShellI::GetVertex(RenderState &state,int flipNormals,DWORD flags, D3DCOLOR diffuse, D3DVALUE *V)
//-------------------------------------------------------------------------------------------
// get vertices in DX flexibile vertexformat
// suported flags D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 

int GShellI::GetVertex(RenderState &state,int flipNormals,DWORD flags,D3DCOLOR diffuse, D3DVALUE *V)
{
    int nv = verts.Length();
	if (nv==0 ) return 0;

	if ((flags & D3DFVF_NORMAL) && (vn.Length() == 0) || (actions & SHELL_COMPUTE_VNORMALS)) {
		ComputeVertexNormals();
	}
 
    D3DVALUE *pV = V;

    const GVertexI* pvi=verts.Data();
    const Point *pc = vc.Data();
    const Point *pv = v.Data();
    const Point *pn = vn.Data();
    const Point *pp = vp.Data();
    if (!pp) pp = pv; // use vertex coordinates

//	TRACE("GShellI:GetVertex identity = %d version %d \n",state.textureMatrixIdentity,state.GetTextureMatrixVersion());


 

 for(int i=0; i < nv; i++) 
 {
    const Point *t;

    // the vertex coordinates
		t = &pv[pvi->v];

		pV[0] = t->x;
		pV[1] = t->y;
		pV[2] = t->z;
		//TRACE("vertex (%f %f %f)\n", pV[0], pV[1], pV[2]);
		pV+=3;

    // the vertex normal
	if (flags & D3DFVF_NORMAL) {
		t = &pn[pvi->n];

		if (flipNormals) 
			{ pV[0] = -t->x; pV[1] = -t->y; pV[2] = -t->z; }
		else 
			{ pV[0] = t->x; pV[1] = t->y; pV[2] = t->z; }
		pV+=3;
	}

	// diffuse color
		if (flags & D3DFVF_DIFFUSE) 
		{
			if (pc) {
				t = &pc[pvi->c];
				((D3DCOLOR *)pV)[0] = D3DRGBA_X(t->x,t->y,t->z,1.0);	 // to do: a * clamp !
			} else ((D3DCOLOR *)pV)[0]= diffuse;			
			pV+=1;
		}
	
	// specular color

	// texture coordinate 1..8
		if (flags & D3DFVF_TEX1) 
		{
		    t = &pp[pvi->p];
			if (!state.textureMatrixIdentity) 
			{
				Point pt= state.textureMatrix * *t;
				pV[0] = pt.x;  pV[1] = pt.y; 
			} 
			else 
				{   pV[0] = t->x; pV[1] = t->y; }
			pV+=2;
		}
    pvi++;
 }//end for

 	return(nv);

}


// Get vertex in D3DVertex format 
int GShellI::GetVertex(RenderState &state,int flipNormals, D3DVERTEX *V)
{
    if ((vn.Length() == 0) || (actions & SHELL_COMPUTE_VNORMALS)) {
		ComputeVertexNormals();
	}

   
    int nv = verts.Length();
 
    D3DVERTEX *pV = V;

    const GVertexI* pvi=verts.Data();
    const Point *pv = v.Data();
    const Point *pn = vn.Data();
    const Point *pp = vp.Data();
    if (!pp) pp = pv; // use vertex coordinates

//	TRACE("GShellI:GetVertex identity = %d version %d \n",state.textureMatrixIdentity,state.GetTextureMatrixVersion());


 

 for(int i=0; i < nv; i++) {
    const Point *t;
    // the vertex coordinates
    t = &pv[pvi->v];

    pV->x = t->x;
    pV->y = t->y;
    pV->z = t->z;

    // the vertex normal
    t = &pn[pvi->n];

	if (flipNormals) 
	{ pV->nx = -t->x; pV->ny = -t->y; pV->nz = -t->z; }
	else { pV->nx = t->x; pV->ny = t->y; pV->nz = t->z; }

    // the vertex parameter
    t = &pp[pvi->p];
	if (!state.textureMatrixIdentity) {
			Point pt= state.textureMatrix * *t;
			pV->tu = pt.x;  pV->tv = pt.y; 
	} else 
	{ 
	    pV->tu = t->x;  pV->tv = t->y;
	}
    pV++; 
    pvi++;
 }
 return(nv);
}

int GShellI::GetVertex(RenderState &state,int flipNormals, D3DCVERTEX *V)
{
    if ((vn.Length() == 0) || (actions & SHELL_COMPUTE_VNORMALS)) {
		ComputeVertexNormals();
	}

   
    int nv = verts.Length();
 
    D3DCVERTEX *pV = V;

    const GVertexI* pvi=verts.Data();
    const Point *pv = v.Data();
    const Point *pn = vn.Data();
    const Point *pc = vc.Data();
    const Point *pp = vp.Data();
    if (!pp) pp = pv; // use vertex coordinates
    if (!pc) pc = pv; // use vertex coordinates

//	TRACE("GShellI:GetVertex identity = %d version %d \n",state.textureMatrixIdentity,state.GetTextureMatrixVersion());


 for(int i=0; i < nv; i++) {
    const Point *t;
    // the vertex coordinates
    t = &pv[pvi->v];

    pV->x = t->x;
    pV->y = t->y;
    pV->z = t->z;

    // the vertex normal
    t = &pn[pvi->n];

	if (flipNormals) 
	{ pV->nx = -t->x; pV->ny = -t->y; pV->nz = -t->z; }
	else { pV->nx = t->x; pV->ny = t->y; pV->nz = t->z; }

	// the vertex color
	t = &pc[pvi->c];
	pV->color = D3DRGB(t->x,t->y, t->z);	 // to do: a * clamp !

    // the vertex parameter
    t = &pp[pvi->p];
	if (!state.textureMatrixIdentity) {
			Point pt= state.textureMatrix * *t;
			pV->tu = pt.x;  pV->tv = pt.y; 
	} else 
	{ 
	    pV->tu = t->x;  pV->tv = t->y;
	}
    pV++; 
    pvi++;
 }
 return(nv);
}

// get pre-lit vertices
int GShellI::GetVertex(RenderState &state,D3DCOLOR color, D3DCOLOR specular,D3DLVERTEX *V)
{
    int nv = verts.Length();
 
    D3DLVERTEX *pV = V;

    const GVertexI* pvi=verts.Data();
    const Point *pv = v.Data();
    const Point *pc = vc.Data();
    const Point *pp = vp.Data();
    if (!pp) pp = pv; // use vertex coordinates

 

 for(int i=0; i < nv; i++) {
    const Point *t;
    // the vertex coordinates
    t = &pv[pvi->v];

    pV->x = t->x;
    pV->y = t->y;
    pV->z = t->z;
	pV->dwReserved = 0;


	if (pc) {
		// the vertex color
		t = &pc[pvi->c];
		pV->color = D3DRGB(t->x,t->y, t->z);	 // to do: a * clamp !
	}
	else pV->color = color;
	pV->specular = specular;


	
    // the vertex parameter
    t = &pp[pvi->p];
	if (!state.textureMatrixIdentity) {
			Point pt= state.textureMatrix * *t;
			pV->tu = pt.x;  pV->tv = pt.y; 
	} else 
	{ 
	    pV->tu = t->x;  pV->tv = t->y;
	}
    pV++; 
    pvi++;
 }
 return(nv);
}

// get vertex mapped 
int GShellI::GetVertex(RenderState &state,int flipNormals,D3DVERTEX *V,IntArray &map)
{

	if ((vn.Length() == 0) || (actions & SHELL_COMPUTE_VNORMALS)) {
		ComputeVertexNormals();
	}

    int nv = map.Length();
    D3DVERTEX *pV = V;

    const Point *pv = v.Data();
    const Point *pn = vn.Data();
    const Point *pp = vp.Data();
    if (!pp) pp = pv; // use vertex coordinates
	int cnt=0;

    for(int i=0; i < nv; i++) {
		int j= map[i];
		if (j < 0) continue; // unused point 

		pV = V + j;	// target 
		const GVertexI* pvi=verts.Data()+i; // source 

		const Point *t;
		
		// the vertex coordinates
		t = &pv[pvi->v];

		pV->x = t->x;
		pV->y = t->y;
		pV->z = t->z;

		// the vertex normal
		t = &pn[pvi->n];
		if (flipNormals) 
		{ pV->nx = -t->x; pV->ny = -t->y; pV->nz = -t->z; }
		else { pV->nx = t->x; pV->ny = t->y; pV->nz = t->z; }


		// the vertex parameter
		t = &pp[pvi->p];
		if (!state.textureMatrixIdentity) {
			Point pt= state.textureMatrix * *t;
			pV->tu = pt.x;  pV->tv = pt.y; 
		} else 
		{ 
			pV->tu = t->x;  pV->tv = t->y;
		}

		cnt ++;

	}
	return(cnt);
}

// get vertex mapped unlit
int GShellI::GetVertex(RenderState &state,D3DCOLOR color, D3DCOLOR specular,D3DLVERTEX *V,IntArray &map)
{

    int nv = map.Length();
    D3DLVERTEX *pV = V;

    const Point *pv = v.Data();
    const Point *pc = vc.Data();
    const Point *pp = vp.Data();
    if (!pp) pp = pv; // use vertex coordinates
	int cnt=0;

    for(int i=0; i < nv; i++) {
		int j= map[i];
		if (j < 0) continue; // unused point 

		pV = V + j;	// target 
		const GVertexI* pvi=verts.Data()+i; // source 

		const Point *t;
		
		// the vertex coordinates
		t = &pv[pvi->v];

		pV->x = t->x;
		pV->y = t->y;
		pV->z = t->z;

		pV->dwReserved = 0;

		// the vertex color
		if (pc) {
			t = &pc[pvi->c];
			pV->color = D3DRGB(t->x,t->y, t->z);	 // to do: a * clamp !
		}
		else pV->color = color;
		pV->specular = specular;

		// the vertex parameter
		t = &pp[pvi->p];
		if (!state.textureMatrixIdentity) {
			Point pt= state.textureMatrix * *t;
			pV->tu = pt.x;  pV->tv = pt.y; 
		} else 
		{ 
			pV->tu = t->x;  pV->tv = t->y;
		}

		cnt ++;

	}
	return(cnt);
}


// Render Shell using D3D IM + custom lighting 
int GShellI::D3dRenderTL(GglRenderTraversal &traversalState)
{
	// shellIs are not yet supported for TL
	// create submehes and delegate

	CreateSubMeshes();

	GSubMesh *m=subMeshes;

	int ret = 0;
	while (m) {
			ret=m->s->Do(traversalState);
			m = m->next;
	}
	return ret;


	//TRACE("GShellI::D3dRenderTL to do \n");
	//return(-1);
}



#endif


#ifdef _3DV


int GShellI::io(AStream &a)
{
  int polyline = RTISA(this,GPolylineI);

  if (polyline) { 	
	  if (!a.tok("polylineindex")) return(0);
  }
  else if (!a.tok("shellindexed")) return(0);



  a.BeginBlock();

  if (a.isOutput()) {			// output
		char buf[256];
		sprintf(buf,"#v = %d, #vn = %d #verts = %d  #Faces = %d (len = %d)",v.Length(),vn.Length(),verts.Length(),nfaces,f.Length());
		a.comment(buf);

		sprintf(buf,"#groups = %d",fgroup.Length());
		a.comment(buf);

		a.BeginTok();

		  ioOutputAttributes(a);		// output shell attributes

		  if (a.tok("verts")) verts.io(a);
		  if (fgroup.Length()>0) if (a.tok("face_groups")) fgroup.io(a);
		  if (fg.Length()>0) if (a.tok("face_to_group")) fg.io(a);

		a.EndTok();
  } else {
		  if (a.BeginTok())
		  do {
		  if (a.tok("verts")) verts.io(a);
		  else if (a.tok("face_groups")) fgroup.io(a);
		  else if (a.tok("face_to_group")) fg.io(a);
		  else if (ioInputOneAttribute(a)) {
		  } else {
				TRACE("ShellI::IO Token %s ??? \n",a.curtok);
		  }

		  } while (a.EndTok());

		  UnSetEditFlag(SHELL_VERTICES_CHANGED | SHELL_FACELIST_CHANGED);

	  // Compute face colors from group tables
	  //if (fc.Length()==0)
	  ComputeFaceColorsFromGroups();


		  // to do : verify indices ?
  }

  a.EndBlock();


  return(a.stat);

}

#endif

//
//
//




//
// GPolyline
//
RTIMP(GPolylineI,0,GShellI);

GPolylineI::GPolylineI() 
{
};




// get a facelist in shell format
int GPolylineI::GetFaceList(GFaceList &flist)
{
   flist.SetPolyline(f);
   return (flist.Length());
}



#if 0
void GPolylineI::RenderGlEdges(RenderState &state,int VN,int VC,int VP)
{

   if (VN) if (vn.Length() == 0)		VN =0;
   if (VC) if (vc.Length() == 0)		VC =0;
   if (VP) if (vp.Length() == 0)		VP =0;

   if (!(flags & SHELL_EDGES_OK)) {
		  ComputeEdgeList();
   }

   
   if (v.Length() <= 1) return;


   // Render as special objects ?
   if (state.edgeDrawStyle && state.edgeDrawStyle->enabled) {

				Point *p1,*p2,*n1=NULL,*n2=NULL;
				Point z(0,0,1);

				if (vn.Length()==0)  n1,n2= &z;
				 int i=0;
				 while (i < (nv-1))
				{
						// 1st vertex of edge
						p1= &v[i];
						//if (VC) c1 = &vc[i];
						if (VN) n1 = &vn[i];
						i++

						// 2nd vertex of edge
						p2= &v[i];
						//if (VC) c2 = &vc[i];
						if (VN) n2 = &vn[i];


						state.edgeDrawStyle->RenderEdge(*p1,*n1,*p2,*n2);
						if (state.Abort()) break;
				}
				return;
   }




   glBegin(GL_LINE_STRIP);
   int cnt=GL_MAX_LINES;
   int i=0;

   while (i  < nv)
   {
		 OUTVERT(i);
		 cnt --; i++;
		 if (cnt <=0) {
				cnt = GL_MAX_LINES;
				glEnd();
				glBegin(GL_LINE_STRIP);
				i--;
		 }

  }
  glEnd();
}

#endif



#ifdef _OGL 
void GPolylineI::RenderGlFaces(RenderState &state,int FN, int VN, int VC, int VP)
{

   int lightModel = state.lightmodel;
   gbool flipNormals = state.frontFaceCW && !state.twosidedLighting;

   // need face normals for computation
   if (FN) {
		 const PointArray &n=GetFN();
		 if (n.Length() == 0) FN=0; /* return(-1); */
   }

   if (VN) {
	 	const PointArray &n=GetVN();
   		if (n.Length() == 0) VN =0; //return(-1);
   }

   int FC = 0;
   if (VC) {
       if (vc.Length() == 0) {
           VC =0;
           if (fc.Length() > 0) FC = 1;	// face colors if no vertex colors 28.06.96
       } else {
	       state.SetLightModel(GL_SMOOTH); // need smooth lighting to see vertex colors
		   //state.SetLocalColors(gtrue);   
		   if (!state.litMaterial) {
			   TRACE("unlit material \n");
		   }		
	   } 
   }    


   if (VP) if (vp.Length() == 0)		VP =0;

#if 0
   //not impl. yet 
   
   if ((prims.Length() > 0) 
        && !FC)     // 16.06. Tristrips doesn´t support Face colors yet
   {  // use precomputed triangle strips
		  RenderGlPrimitives(state,FN,VN,VC,VP);
		  return;
   }
#endif



   if (f.Length() == 0) {// no faces, simply draw polyline 
		glBegin(GL_LINE_STRIP);
		int cnt=GL_MAX_LINES;
		int i=0;
		int nv = v.Length();

		while (i  < nv)
		{
			OUTVERT(i);
			cnt --; i++;
			if (cnt <=0) {
				cnt = GL_MAX_LINES;
				glEnd();
				glBegin(GL_LINE_STRIP);
				i--;
			}

		}
		glEnd();


	   return;

   }	
   const int *fp =f;					// face list stepping pointers
   const int *fpend= fp+f.Length();   // points after last element of facelist
   int fi=-1;					// face index


   if (state.useVertexArrayEXT && state.hasVertexArrayEXT) {
        
        state.glVertexPointerEXT(3,GL_FLOAT,0,v.Length(),v.Data());
		glEnable(GL_VERTEX_ARRAY_EXT);

        if (VN) { state.glNormalPointerEXT(GL_FLOAT,0,vn.Length(),vn.Data());
				  glEnable(GL_NORMAL_ARRAY_EXT);
		}  else  glDisable(GL_NORMAL_ARRAY_EXT);
        if (VC) { state.glColorPointerEXT(3,GL_FLOAT,0,vc.Length(),vc.Data());
				  glEnable(GL_COLOR_ARRAY_EXT);
		}  else  glDisable(GL_COLOR_ARRAY_EXT);
        if (VP) { state.glTexCoordPointerEXT(3,GL_FLOAT,0,vp.Length(),vp.Data());
				  glEnable(GL_TEXTURE_COORD_ARRAY_EXT);
		}  else  glDisable(GL_TEXTURE_COORD_ARRAY_EXT);

   do { 		// for all faces

		 int flen = * fp ++; 	// length of this face
		 if (flen<0) flen=-flen;   else fi++;

		 int i;

		 glBegin(GL_LINE_STRIP);
		 if (FN) if (flipNormals) glNormalFlip(fn[fi]); else glNormal(fn[fi]);
		 if (FC) glColor(fc[fi]);
		 for (i=0; i< flen; i++) {
				 int vi = *fp++; // vertex index
				 state.glArrayElementEXT(vi);	
		 }
		 glEnd();

   }
   while (fp < fpend);

  	return;
   } // Vertex Array Extension

   do { 		// for all faces

		 int flen = * fp ++; 	// length of this face
		 if (flen<0) flen=-flen;   else fi++;

		 int i;

		 glBegin(GL_LINE_STRIP);
		 if (FN) if (flipNormals) glNormalFlip(fn[fi]); else glNormal(fn[fi]);
		 if (FC) glColor(fc[fi]);
		 for (i=0; i< flen; i++) {
				 int vi = *fp++; // vertex index
				 OUTVERT(vi);
		 }
		 glEnd();

   }
   while (fp < fpend);

   state.SetLightModel(lightModel);

}


int GPolylineI::glRender(RenderState &state)
{	
	 int l = state.faces.lighted;

	 if (state.faces.lighted) {
		if (state.faces.smooth) {
			if (vn.Length() == 0) state.faces.lighted=0;
		} else if (fn.Length() ==0) 
			state.faces.lighted=0;
	 }
			

				// render faces
				if (state.SetFaceAttributes()) {
						  int doVn = state.faces.smooth && !Flat();	
						  RenderGlFaces(state,!doVn,
										  doVn,
										  state.faces.colored,
										  state.faces.textured);
				} else {
					if (state.SetEdgeAttributes()) {

						int doVn = state.edges.smooth && !Flat();	
						RenderGlFaces(state,!doVn,
										  doVn,
										  state.edges.colored,
										  state.edges.textured);
					}

				}
				state.faces.lighted = l;

								 
				 // render vertices
				 if (state.SetVertexAttributes())
						  RenderGlVertices(state,state.vertices.lighted,
						  state.vertices.colored);

				 if (state.SetNormalAttributes()) {
						  BBox &box =GetBBox();
						  Point d=box.Size();
						  RenderGlNormals(state,d.Len()*0.1,1);
				 }
				 if (state.SetBBoxAttributes()) {
						  RenderGlBBox(state);
				 }
		  return(0);
}
#endif

#ifdef _D3D

/*
Create Direct 3D Execute Buffer
*/
int GPolylineI::CreateExBuf(RenderState &state,G3ObjRef &objBuf)
{
    
	size_t size;
    int nv=GetVertexCnt(); 
    int nTri = f.GetLineCount();

    if (nv == 0 || nTri == 0) return(0);

	gbool flip = state.FlipFaces();
	gbool flipNormals =  state.FlipNormals();
	gbool bothSides = 0; // state.NeedBothFaces();
	// to do: unlit, colored 
	gbool unlit= (state.canDoRgbLighting && state.faces.colored && (vc.Length()>0)) 
		|| state.Unlit() || vn.Length() == 0; 

	gbool hasvc= (vc.Length()>0);
	D3DCOLOR color, specular;
	
	if (unlit) {
		color = state.getCurrentColorD3D();
		specular = RGBA_MAKE(0,0,0,255);
	}



	gbool tooManyVerts = 0;

	if ((state.maxVertexCount>0) && (nv > state.maxVertexCount)) 
		tooManyVerts = 1;

	// tooManyVerts = 1; //debug


	int bufi=0;	// the current execute buffer created 


	while(1) {  // two passes if bothSides 
	
	if (tooManyVerts)	{
		IntArray vertMap(nv);	// table for mapping used vertices 
		IntArray fmap;		// termporay to remap a face 
		int i;

		int maxVerts = state.maxVertexCount;
		// maxVerts = 32; // debug

		const int *fp =f;					// face list stepping pointers
		const int *fpend= fp+f.Length();   // points after last element of facelist
		int fi=-1;					// face index

		while (fp <fpend) {		// while face list 

		for (i=0; i<nv; i++) vertMap[i]=-1; // not used
		int newNv = 0;
		int newNTri = 0;

		const int *fpThisStart = fp;
		const int *fpThisEnd = fp;
		const int thisFi = fi;

		// pass 1: check how many faces are fitting
		// use only vertices refed by those faces 

		while (fp<fpend) {
  			int flen = * fp ++; 	// length of this face
			if (flen<0) flen=-flen;   else fi++;

			// do we have enough vertices now ? 
			if ((flen + newNv) > maxVerts) {
				fp--;
				if (newNv == 0) {
					TRACE("to big face len = %d \n",flen);
					return(0);
				}
				break; // while 
			}

			switch (flen) {
				case 0:
				case 1:
					fp += flen;
					break;
			    default :
					newNTri += flen -1;

					while (flen>0) {
						int vi = *fp++;
						if (vertMap[vi] == -1) { // need a new vertex
							vertMap[vi]= newNv;
							newNv ++;
						}
						flen--;
					}
				break;
			}
           
		} // while (fp<fpend)
		fpThisEnd = fp;


		// pass 2 
		fp = fpThisStart;

	    if (objBuf.exBuf.Length() <= bufi) {
			objBuf.Append(state.CreateExBuf());
		}
		
		{
		G3ExBuf &buf = *objBuf.exBuf[bufi];
		// Compute size of execute buffer
		size = buf.VERTEX_SIZE(newNv);
		size += buf.LINE_LIST_SIZE(newNTri)+30*buf.OP_SIZE();

		//TRACE("Creating Ex buf v=%d tri=%d size=%d \n",newNv,newNTri,size);

		// create execute buffer
		if (!buf.Create (state.lpDev,size)) {
			TRACE("%s:Could not create execute buffer of size &d (verts=%d tris=%d)\n",ClassName(),size,nv,nTri);
			return(0);
		}
		if (!buf.Lock()) {
			TRACE("%s:Could not lock execute buffer \n",ClassName());
			return(0);
		}

		// store vertex data
		int cnt;
		if (unlit) {
			LPD3DLVERTEX vp = (LPD3DLVERTEX) buf.lpPointer;
			cnt = GetVertex(state,color, specular,vp,vertMap);
			buf.lpPointer = (void *) &vp[cnt];
		} else {
			LPD3DVERTEX vp = (LPD3DVERTEX) buf.lpPointer;
			cnt= GetVertex(state,flipNormals,vp,vertMap);
			buf.lpPointer = (void *) &vp[cnt];
		}
		buf.offset = buf.Length();


		ASSERT(cnt == newNv);

		// store misc instructions

		OP_PROCESS_VERTICES(1, buf.lpPointer);
		if (unlit) {
			PROCESSVERTICES_DATA(D3DPROCESSVERTICES_TRANSFORM |(!hasvc ? D3DPROCESSVERTICES_NOCOLOR : 0) , 0, cnt, buf.lpPointer);
		} else {
			PROCESSVERTICES_DATA(D3DPROCESSVERTICES_TRANSFORMLIGHT, 0, cnt, buf.lpPointer);
		}


		buf.PUT_ALIGN();

		// store a line list

		buf.PUT_OP_LINE_LIST(newNTri);
  
		fi=thisFi;					// face index

		while (fp<fpThisEnd) {
  			int flen = * fp ++; 	// length of this face
			if (flen<0) flen=-flen;   else fi++;
			switch (flen) {
			case 0:
			case 1:
			default :
				{
					fmap.SetLength(flen);
					for (int vi=0; vi<flen; vi++) fmap[vi] = vertMap[fp[vi]];
					buf.PUT_POLYLINE(flen,fmap.Data(),flip);
				}
				break;

			} // switch
			fp += flen;
          
		}
		buf.PUT_OP_EXIT();
		// Unlock and set 
		buf.Unlock();
    
		ASSERT(buf.Length()<=buf.MaxLength());
		//TRACE("Stored  Ex buf v=%d  len %d size=%d \n",newNv,buf.Length(),buf.MaxLength());

		if (!buf.Set()) {
			TRACE("%s:Could not set execute buffer of size &d (verts=%d tris=%d)\n",ClassName(),size,nv,nTri);
			return(0);
		}
		}  // create new buffer 

		
		//
		bufi++; // need a new exbuf
		fp = fpThisEnd;

		} // for all faces

	}	 // too mayVerts
	else 
	{
    if (objBuf.exBuf.Length() <= bufi) {
		objBuf.Append(state.CreateExBuf());

	}
		

	G3ExBuf &buf = *objBuf.exBuf[bufi];

    // Compute size of execute buffer
    size = buf.VERTEX_SIZE(nv);
    size += buf.TRIANGLE_LIST_SIZE(nTri)+30*buf.OP_SIZE();

	//TRACE("Creating Ex buf v=%d tri=%d size=%d \n",nv,nTri,size);

    // create execute buffer
	if (!buf.Create (state.lpDev,size)) {
        TRACE("%s:Could not create execute buffer of size &d (verts=%d tris=%d)\n",ClassName(),size,nv,nTri);
        return(0);
    }
    if (!buf.Lock()) {
        TRACE("%s:Could not lock execute buffer \n",ClassName());
        return(0);
    }

    // store vertex data

	int cnt;

	if (unlit) {
		LPD3DLVERTEX vp = (LPD3DLVERTEX) buf.lpPointer;
		cnt = GetVertex(state,color, specular,vp);
		buf.lpPointer = (void *) &vp[cnt];
	}
	else {
		LPD3DVERTEX vp = (LPD3DVERTEX) buf.lpPointer;
		cnt= GetVertex(state,flipNormals,vp);
		buf.lpPointer = (void *) &vp[cnt];
	}

    buf.offset = buf.Length();

    
    // store misc instructions

    OP_PROCESS_VERTICES(1, buf.lpPointer);
		if (unlit) {
			PROCESSVERTICES_DATA(D3DPROCESSVERTICES_TRANSFORM |(!hasvc ? D3DPROCESSVERTICES_NOCOLOR : 0) , 0, nv, buf.lpPointer);
		} else {
			PROCESSVERTICES_DATA(D3DPROCESSVERTICES_TRANSFORMLIGHT, 0, nv, buf.lpPointer);
		}


    buf.PUT_ALIGN();

    // store a triangle list

    buf.PUT_OP_LINE_LIST(nTri);
  
    const int *fp =f;					// face list stepping pointers
    const int *fpend= fp+f.Length();   // points after last element of facelist
    int fi=-1;					// face index

    while (fp<fpend) {
  	   int flen = * fp ++; 	// length of this face
	   if (flen<0) flen=-flen;   else fi++;
       switch (flen) {
       case 0:
       case 1:
       default :
           buf.PUT_POLYLINE(flen,fp,flip);
           break;

       }
       fp += flen;
           
    }
    buf.PUT_OP_EXIT();
    // Unlock and set 
    buf.Unlock();
    
    ASSERT(buf.Length()<=buf.MaxLength());
	//TRACE("Stored  Ex buf v=%d tri=%d size=%d \n",nv,buf.Length(),buf.MaxLength());

    if (!buf.Set()) {
        TRACE("%s:Could not set execute buffer of size &d (verts=%d tris=%d)\n",ClassName(),size,nv,nTri);
        return(0);
    }
    bufi++; 
	}

	if (bothSides) {  // second path 
		bothSides = 0;
		flip = !flip;
	    flipNormals = !flipNormals;
		continue;
	}

	break;

	} 
    
	if (objBuf.exBuf.Length() > bufi) {
		objBuf.Truncate(bufi);
	}
		
	objBuf.textureMatrixVersion = state.GetTextureMatrixVersion();
	objBuf.unlit = unlit;

    return(1);
}

int GPolylineI::D3dRenderTL(GglRenderTraversal &traversalState)
{
	// GPolylineI are not yet supported for TL
	// create submehes and delegate

	CreateSubMeshes();

	GSubMesh *m=subMeshes;

	int ret = 0;
	while (m) {
			ret=m->s->Do(traversalState);
			m = m->next;
	}
	return ret;
}

#endif









//
//
//








#define MIN3INDEX(a,b,c) (((a)<=(b)) ? ( ((a)<=(c)) ?  0 : 2 ) : (((b)<=(c)) ?  1	: 2 ))
#define MAX3INDEX(a,b,c) (((a)>=(b)) ? ( ((a)>=(c)) ?  0 : 2 ) : (((b)>=(c)) ?  1	: 2 ))


// compute default texture coordinates based on Bounding Box for a set of points
void ComputeBoxParams(const BBox &bbox,const PointArray &p, PointArray &params)
{
  Point size = bbox.Size();
  int s=0;
  int t=1;

  // precedence for lower axes x y z 
  
  if (params.Length() < p.Length()) params.SetLength(p.Length());

  // find u,v axis of bounding box for planar mapping
  s = MAX3INDEX(size.x,size.y,size.z);
  switch (s) {
  case 0:  t = (size.y>=size.z) ? 1 : 2; break;
  case 1:  t = (size.x>=size.z) ? 0 : 2; break;
  case 2:  t = (size.x>=size.y) ? 0 : 1; break;
  }

  if (size[s] == 0.0) size[s] = 1.0;

  // compute scaling / translation params
  float soffset = -bbox.mi[s];
  float sscale = 1.0/ size[s];

  float toffset = -bbox.mi[t];
  float tscale = 1.0/ size[s]; // from s !

  // map points
  for(int i=0; i<p.Length(); i++) {
  	float fs = (p[i][s] + soffset) * sscale;
  	float ft = (p[i][t] + toffset) * tscale ;

	params[i].Set( fs, ft ,0.0);
  }

}

// compute default texture coordinates based on Bounding Box for a set of points
// params.Length must have been set properly 
void ComputeBoxParams(const BBox &bbox, const Array<GVertexI> &verts, const PointArray &p, PointArray &params)
{
  Point size = bbox.Size();
  int s=0;
  int t=1;

  if (verts.Length() == 0) return;
  if (params.Length() == 0) return;
  
  //if (params.Length() < p.Length()) params.SetLength(p.Length());

  // find s,t axis of bounding box for planar mapping
  s = MAX3INDEX(size.x,size.y,size.z);
  switch (s) {
  case 0:  t = (size.y>=size.z) ? 1 : 2; break;
  case 1:  t = (size.x>=size.z) ? 0 : 2; break;
  case 2:  t = (size.x>=size.y) ? 0 : 1; break;
  }

  if (size[s] == 0.0) size[s] = 1.0;

  // compute scaling / translation params
  float soffset = -bbox.mi[s];
  float sscale = 1.0/ size[s];

  float toffset = -bbox.mi[t];
  float tscale = 1.0/ size[s]; // from s !

  // map points
  const GVertexI *vp = verts.Data();
  const GVertexI *vpEnd = vp +  verts.Length();

  while (vp<vpEnd) {
  	int i=vp->v;
	float fs = (p[i][s] + soffset) * sscale;
  	float ft = (p[i][t] + toffset) * tscale ;

	params[vp->p].Set( fs, ft ,0.0);
	vp++;
  }
}




static int cube_connectivity[] = {

/*		4,0,1,2,3,
	4,4,5,0,3,
	4,6,1,0,5,
	4,6,7,2,1,
	4,3,2,7,4,
	4,4,7,6,5 */
// modified for VRML  x left y up z out
		4,0,2,3,1,
		4,1,3,7,5,
		4,5,7,6,4,
		4,4,6,2,0, // wrong 4,6,2,0,4,
		4,7,3,2,6,
		4,1,5,4,0 };


static int cube_flags = SHELL_CONVEX|SHELL_CLOSED;





// Make a box
// Each face shares vertices, but has
// separate normals
// and is indepently textured to unit square

GShellI * NewBox(float atx, float aty,float atz,
						float dx,float dy,float dz,GShellI *cube)
{
   Point _p[8];
   Point params[4];
   int p,i,j,k;
   float x,y,z;

   p= 0;
   for (k=0;k<=1;k++) {
		 z=atz+dz*float(k);
		 for (i=0;i<=1;i++) {
				y=aty+float(i)*dy;
				for (j=0;j<=1;j++) {
				  x=atx+float(j)*dx;
				  {
						_p[p].Set(x,y,z);
				  }
				  p++;
				}
		 } /*  i */
   }
   if (!cube)
		  cube = new GShellI;

   GVertexI *v = new GVertexI[4*6];
   int *flist = new int [5*6];

   int *flistp=flist;
   const int *fp =cube_connectivity;
   int f,vi=0;
   for (f=0; f<6; f++) {

		 *flistp++ = *fp++;
		 for(i=0; i<4;i++) {
				v[vi].v = *fp++;		// share v
				v[vi].n = vi;			// separate vn
				v[vi].p = i;			// separate parameter
				v[vi].c = v[vi].v;		// color
				*flistp ++ = vi;
				vi++;
		 }
   }
/*
   params[0].Set(0.0,0.0,0.0);
   params[1].Set(0.0,1.0,0.0);
   params[2].Set(1.0,1.0,0.0);
   params[3].Set(1.0,0.0,0.0);
*/

   params[0].Set(1.0,0.0,0.0);
   params[1].Set(1.0,1.0,0.0);
   params[2].Set(0.0,1.0,0.0);
   params[3].Set(0.0,0.0,0.0);

   cube->SetV(8,_p);
   cube->SetVN(4*6,(Point *)NULL);
   cube->SetVP(4,params);
   cube->SetVertices(vi,v);
   cube->SetFN(6,(Point *)NULL);
   delete v;


   cube->SetFaceList(flistp-flist,flist);
//   cube->ComputeFaceNormals();
//   cube->ComputeVertexNormals();

   cube->SetFlag(cube_flags);
   delete flist;

   return (cube);
}

// Create a cylinder
// with top + bottom face (hard)
// to do cl
// to do texture pararmeter needs seems == duplicate vertex
//GShellI *NewCylinder(float cx=0.0,float cy=0.0,float cz=0.0,float r=1.0,float h=2.0,int n=12,GShellI *o=0)
GShellI *NewCylinder(float cx,float cy,float cz,float r,float h,int n,GShellI *o)
{

		int 	i;
		float	div, phi;


		if (n<=0) n=12;

		PointArray poly(n*2);
		PointArray params(n*2+n*2);

		if (!o)
			  o = new GShellI;

		div = TWOPI / (float) n;
		for (i=0; i<n; i++) {
				phi = div * i;
				poly[i].x= cx+r* (cos(phi));
				poly[i].y= cy+r* (sin(phi));
				poly[i].z= cz;
				poly[i+n]= poly[i];
				poly[i+n].z += h;
				params[i].Set((float) (i) / (float) (n-1),0.0,0.0);
				params[i+n].Set((float) (i) / (float) (n-1),1.0,0.0);
		}
		o->SetV(n*2,poly);

		GVertexI *verts = new GVertexI[n*2 + n*2];

		GFaceList flist((n+1)*2 + (n+1) * 5);

		for (i=0; i<n; i++) {
				verts[i].v = i; 		// Bottom face
				verts[i+n].v = i;
				verts[i+2*n].v = i+n;
				verts[i+3*n].v = i+n; // top face

				verts[i].p = n*2+i;
				verts[i+n].p = i;
				verts[i+2*n].p = i+n;
				verts[i+3*n].p = n*3+i;
		}
		// verts[n+n].
		// each vert a normal
		for (i=0; i<4*n; i++) verts[i].n = i;

		// each vert a color
		for (i=0; i<4*n; i++) verts[i].c = i;

		// special xy parameters for top & bottom face
		// to do:scale somehow ??
		for (i=0; i<n; i++) {
				  params[2*n+i].Set(-poly[i].x/r,poly[i].y/r,0.0);
				  params[3*n+i].Set(poly[i].x/r,poly[i].y/r,0.0);
				}


		o->SetVN(4*n,(Point *)NULL);
		o->SetVP(4*n,params);

		o->SetVertices(4*n,verts);
		delete verts;


		flist.SetLength(0);

		// Bottom Face
		int voffset=0;
		flist.Append(n);
		for(i=n-1; i>=0; i--) { flist.Append(i+voffset);  }

		voffset=n;
		for(i=0; i<n; i++) {
			flist.AppendQuad(voffset+i,voffset + (i+ 1) % n,
				voffset+ n + (i+ 1) % n,voffset+i+n);
		}


		// Top Face
		voffset=3*n;
		flist.Append(n);
		for(i=0; i<n; i++) flist.Append(i+voffset);

		o->SetFaceList(flist);
		o->SetFlag(SHELL_CONVEX);

		o->UpdateActions();

//		o->ComputeFaceNormals();
//		o->ComputeVertexNormals();

//		o->UnSetFlag(SHELL_FNORMALS_COMPUTED);
//		o->UnSetFlag(SHELL_VNORMALS_COMPUTED);
		
//		o->UnSetEditFlag(SHELL_FACELIST_CHANGED | SHELL_VERTICES_CHANGED);

		////o->edit_flags = SHELL_CHANGED;

		return(o);
}


// capped cylinder around y-axis  

GShellI *NewCylinderVrml(float cx,float cy,float cz,float r,float h,int n,int parts,Point *fcolors,GShellI *o)
{

		int 	i;
		float	div, phi;


		if (n<=0) n=12;

		n+=1;

		PointArray poly(n*2);
		PointArray params((n)*2+n*2);

		if (!o)
			  o = new GShellI;

		div = TWOPI / (float) (n-1);
		for (i=0; i<n; i++) {
				if (i== (n-1)) {
				   poly[i]= poly[0]; // new 02.03.96
				} else {
					phi = -PI*0.5+  div * i;   // - new
					poly[i].x= cx+r* (cos(phi));
					poly[i].z= cz+r* (sin(phi)); // was cy
					poly[i].y= cy-h*0.5;
				}
				poly[i+n]= poly[i];
				
				poly[i+n].y = cy+h*0.5;
				float u= (float) (i) / (float) (n-1);

				params[i].Set(1.0-u,0.0,0.0);
				params[i+n].Set(1.0-u,1.0,0.0);
		}
		o->SetV(n*2,poly);

		GVertexI *verts = new GVertexI[n*2 + n*2];

		GFaceList flist((n+1)*2 + (n+1-1) * 5);

	
		for (i=0; i<n; i++) {
				verts[i].v = i; 		// Bottom face
				verts[i+n].v = i;
				verts[i+2*n].v = i+n;
				verts[i+3*n].v = i+n; // top face

				verts[i].p = n*2+i;
				verts[i+n].p = i;
				verts[i+2*n].p = i+n;
				verts[i+3*n].p = n*3+i;
		}


		// each vert a normal
		for (i=0; i<4*n; i++) verts[i].n = i;

		// each vert a color
		for (i=0; i<4*n; i++) verts[i].c = i;
		
		// share vertices & normals
		verts[n-1].v = verts[0].v;
		verts[n-1].n = verts[0].n;
		verts[n-1+n].v = verts[0+n].v;
		verts[n-1+n].n = verts[0+n].n;

		verts[n-1+2*n].v = verts[0+2*n].v;
		verts[n-1+2*n].n = verts[0+2*n].n;
		verts[n-1+3*n].v = verts[0+3*n].v;
		verts[n-1+3*n].n = verts[0+3*n].n;

		// verts[n+n].

		// special xz parameters for top & bottom face
		// to do:scale somehow ??
		for (i=0; i<n; i++) {
				  float u = (r+poly[i].x-cx) / 2.0*r;
				  float v = (r+poly[i].z-cz) / 2.0*r;

				  params[2*n+i].Set(1.0-u,v,0.0);
				  params[3*n+i].Set(u,1.0-v,0.0); // new 1.0-v new 29.02.96
				}


		o->SetVN(4*n,(Point *)NULL);
		o->SetVP(4*n,params);

		o->SetVertices(4*n,verts);
		delete verts;


		flist.SetLength(0);

		// side Faces
		int voffset=0;
		if (parts & 1) {
		voffset=n;
		for(i=0; i<(n-1); i++) {
//			flist.AppendQuad(voffset+i,voffset + (i+ 1) % n,
//				voffset+ n + (i+ 1) % n,voffset+i+n);
			flist.AppendQuad(voffset + (i+ 1) % n,voffset+i,
				voffset+i+n,voffset+ n + (i+ 1) % n);

		}
		}


		// Top Face
		if (parts & 2) {
			voffset=3*n;
			flist.Append(n);
			for(i=n-1; i>=0; i--) { flist.Append(i+voffset);  }
		}

		if (parts & 4) { 	// Bottom Face
			voffset=0;
  	    	flist.Append(n);
			for(i=0; i<n; i++) flist.Append(i+voffset);
		}


		o->SetFaceList(flist);
		o->SetFlag(SHELL_CONVEX);

		o->UpdateActions();

		return(o);
}
#if 0
// capped cylinder around y-axis  

GShellI *NewCylinderVrml(float cx,float cy,float cz,float r,float h,int n,int parts,Point *fcolors,GShellI *o)
{

		int 	i;
		float	div, phi;


		if (n<=0) n=12;
		int n1= n+1;

		PointArray poly(n*2);
		PointArray params((n)*2+n*2);

		if (!o)
			  o = new GShellI;

		div = TWOPI / (float) n;
		for (i=0; i<n; i++) {
				phi = PI*0.5+  div * i;
				poly[i].x= cx+r* (cos(phi));
				poly[i].z= cy+r* (sin(phi));
				poly[i].y= cy-h*0.5;
				poly[i+n]= poly[i];
				
				poly[i+n].y = cy+h*0.5;

				params[i].Set((float) (i) / (float) (n-1),0.0,0.0);
				params[i+n].Set((float) (i) / (float) (n-1),1.0,0.0);
		}
		o->SetV(n*2,poly);

		GVertexI *verts = new GVertexI[(n+1)*2 + n*2];

		GFaceList flist((n+1)*2 + (n+1) * 5);

	
		for (i=0; i<n; i++) {
				verts[i].v = i; 		// Bottom face
				verts[i+n1].v = i;
				verts[i+2*n].v = i+n;
				verts[i+3*n].v = i+n; // top face

				verts[i].p = n*2+i;
				verts[i+n].p = i;
				verts[i+2*n].p = i+n;
				verts[i+3*n].p = n*3+i;
		}



		// verts[n+n].
		// each vert a normal
		for (i=0; i<4*n; i++) verts[i].n = i;

		// each vert a color
		for (i=0; i<4*n; i++) verts[i].c = i;

		// share vertices & normals
		verts[n-1].v = verts[0].v;
		verts[n-1].n = verts[0].n;
		verts[n-1+n].v = verts[0+n].v;
		verts[n-1+n].n = verts[0+n].n;

		verts[n-1+2*n].v = verts[0+2*n].v;
		verts[n-1+2*n].n = verts[0+2*n].n;
		verts[n-1+3*n].v = verts[0+3*n].v;
		verts[n-1+3*n].n = verts[0+3*n].n;



		// special xz parameters for top & bottom face
		// to do:scale somehow ??
		for (i=0; i<n; i++) {
				  params[2*n+i].Set(-poly[i].x,poly[i].z,0.0);
				  params[3*n+i].Set(poly[i].x,poly[i].z,0.0);
				}


		o->SetVN(4*n,0);
		o->SetVP(4*n,params);

		o->SetVertices(4*n,verts);
		delete verts;


		flist.SetLength(0);

		// side Faces
		int voffset=0;
		if (parts & 1) {
		voffset=n;
		for(i=0; i<n; i++) {
//			flist.AppendQuad(voffset+i,voffset + (i+ 1) % n,
//				voffset+ n + (i+ 1) % n,voffset+i+n);
			flist.AppendQuad(voffset + (i+ 1) % n,voffset+i,
				voffset+i+n,voffset+ n + (i+ 1) % n);

		}
		}


		// Top Face
		if (parts & 2) {
		voffset=3*n;
		flist.Append(n);
		for(i=n-1; i>=0; i--) { flist.Append(i+voffset);  }
		}

		if (parts & 4) {
		// Bottom Face
		voffset=0;
  	    flist.Append(n);
		for(i=0; i<n; i++) flist.Append(i+voffset);
		}


		o->SetFaceList(flist);
		o->SetFlag(SHELL_CONVEX);


		//o->ComputeFaceNormals();
		//o->ComputeVertexNormals();
		//o->edit_flags = SHELL_CHANGED;
	    
	    //o->UnSetEditFlag(SHELL_FACELIST_CHANGED | SHELL_VERTICES_CHANGED);
		o->UpdateActions();

		return(o);
}
#endif

// capped cone arround y
GShellI *NewConeVrml(float cx,float cy,float cz,float r,float h,int n,int parts,Point *fcolors,GShellI *o)
{

		int 	i;
		float	div, phi;


		if (n<=0) n=12;

		n+=1;

		PointArray poly(n*2);
		PointArray params(n*2+n*2);

		if (!o)
			  o = new GShellI;

		div = TWOPI / (float) (n-1);
		for (i=0; i<n; i++) {
				if (i== (n-1)) {
				   poly[i]= poly[0]; // new 02.03.96
				} else {
					phi = -PI*0.5+  div * i;  // - new 29.02.96
					poly[i].x= cx+r* (cos(phi));
					poly[i].z= cz+r* (sin(phi)); // was cz
					poly[i].y= cy-h*0.5;
				}
				float u= (float) (i) / (float) (n-1);
				params[i].Set(1.0-u,0.0,0.0);
				params[i+n].Set(1.0-u,1.0,0.0);
		}

		poly[n].x = cx;
		poly[n].y = cy+h*0.5;
		poly[n].z = cz;

		o->SetV(n+1,poly);

		GVertexI *verts = new GVertexI[n*2 + n*1];

		GFaceList flist((n+1)*2 + (n) * 5);

		// each vert a normal
		for (i=0; i<3*n; i++) verts[i].n = i;
		// each vert a color
		for (i=0; i<3*n; i++) verts[i].c = i;


	
		for (i=0; i<n; i++) {
				verts[i].v = i; 		// Bottom face
				verts[i+n].v = i;
				verts[i+2*n].v = n;	// apex

				verts[i].p = n*2+i;
				verts[i+n].p = i;
				verts[i+2*n].p = i+n; // apex
		}
		// verts[n+n].

		// special xz parameters for  bottom face
		// to do:scale somehow ??
		for (i=0; i<n; i++) {
				  float u = (1.0+(poly[i].x-cx)/r) / 2.0;
				  float v = (1.0+(poly[i].z-cz)/r) / 2.0;

				  params[2*n+i].Set(u,v,0.0);

				
		}


		o->SetVN(3*n,(Point *)NULL);
		o->SetVP(3*n,params);

		o->SetVertices(3*n,verts);

		delete verts;


		flist.SetLength(0);

		// side Faces
		int voffset=0;
		if (parts & 1) {
		voffset=n;
		for(i=0; i<(n-1); i++) {
//			flist.AppendTri(voffset+i,voffset + (i+ 1) % n,voffset+i+n);
			flist.AppendQuad(voffset + (i+ 1) % n,voffset+i,
				voffset+i+n,voffset+ n + (i+ 1) % n);
		}
		}


		if (parts & 2) {
			// Bottom Face
			voffset=0;
  	    	flist.Append(n);
			for(i=0; i<n; i++) flist.Append(i+voffset);
		}


		o->SetFaceList(flist);
		o->SetFlag(SHELL_CONVEX);
		
		o->UpdateActions();

/*
		o->ComputeFaceNormals();
		o->ComputeVertexNormals();
		o->edit_flags = SHELL_CHANGED;
*/

		return(o);
}


// sphere with parameters arround y
GShellI *NewSphereVrml(float cx,float cy,float cz,float r, 
                       int n1,int n2,
                       Point *fcolors,GShellI *o)
{

		int 	i,j;


		if (n1<=0) n1=12;
		if (n2<=0) n2=n1 / 2;

		n1+=1;
		n2+=1;

		PointArray poly(n1*n2);
		PointArray params(n1*n2);
		PointArray norms(n1*n2);

		if (!o)
			  o = new GShellI;

		
		GVertexI *verts = new GVertexI[n1*n2];


		float du = TWOPI / (float) (n1-1);
		float dv = PI / (float) (n2-1);

        float dpu = 1.0f / (float) (n1-1);
        float dpv = 1.0f / (float) (n2-1);

        float c0 = -PI*0.5;
		
        poly[0].x= c0;

		for (i=0; i<n1; i++) {

          		int offset = i * n2;

				float u = c0 +  du * i;
        		float cosu=cos(u);  // VC 4.1 bad value if optimized
				float sinu=sin(u);

				float pu= 1.0 - ((float) (i) * dpu );
				
				for (j=0; j<n2; j++) {
					float v = PI - dv * j; // 29.02 PI -
					float cosv=cos(v);
					float sinv=sin(v);

					norms[offset+j].x= cosu*sinv;
					norms[offset+j].z= sinu*sinv;
					norms[offset+j].y= cosv;
					norms[offset+j].Normalize();

					poly[offset+j].x= cx+r*cosu*sinv;
					poly[offset+j].z= cz+r*sinu*sinv;
					poly[offset+j].y= cy=r*cosv;

					params[offset+j].Set(pu,(float) (j) *dpv ,0.0);

					verts[i*n2+j].v = offset+j;
					verts[i*n2+j].c = offset+j;
					verts[i*n2+j].n = offset+j;
					verts[i*n2+j].p = offset+j;

				}
		}

		j=0;
		for(i=1; i<n1; i ++) { verts[i*n2+j].v = verts[0*n2+j].v;verts[i*n2+j].n = verts[0*n2+j].n; }

		j=n2-1;
		for(i=1; i<n2; i ++) { verts[i*n2+j].v = verts[0*n2+j].v;verts[i*n2+j].n = verts[0*n2+j].n; }

		
		i=n1-1;
		for(j=0; j<n2; j ++) { verts[i*n2+j].v = verts[0*n2+j].v;verts[i*n2+j].n = verts[0*n2+j].n; }



		o->SetV(n1*n2,poly);


		GFaceList flist((n1-1)*(n2-1)*5);

		o->SetVN(norms);
		o->SetVP(params);

		o->SetVertices(n1*n2,verts);

		delete verts;


		flist.SetLength(0);

		for(i=0; i< (n1-1); i++) {
			int voffset = i*n2;

			for(j=0; j<(n2-1); j++) {
//				flist.AppendQuad(voffset + (j+ 1),voffset+j,voffset+j+n2,voffset+ n2 + (j+ 1) );
				flist.AppendQuad(voffset + (j+ 1),voffset+ n2 + (j+ 1),voffset+j+n2,voffset+j ); // 29.06.96
		}
		}

		o->SetFaceList(flist);
		o->SetFlag(SHELL_CONVEX);
		
		o->UpdateActions();
		o->UnSetAction(SHELL_COMPUTE_VNORMALS);


		return(o);
}


struct ColorTableEntry
{
		const char * name;
		float r,g,b;
		void SetColor(const Point &NewColor) { r = NewColor.x; g = NewColor.y; b = NewColor.z;	}
		Point  GetColor() const { return Point(r,g,b); }
		void SetNanme(const char *NewName) { name = NewName; } // by Ref !!!!!!!!!1
		int MatchName(const char *SearchName) const { return(streq(SearchName,name)); }

};

ColorTableEntry ColorTable[] = {
#if 0
"black" ,0,0,0,
"white", 1,1,1,
"gray", 0.5,0.5,0.5,
#else

#include "colors.inc"

#endif


NULL,0,0,0
}; // colortable

// parse RGB color value from a string
int GetColor(const char *c,Point &color)
{ int stat;

 if (isalnum(*c)) {

	 const ColorTableEntry * p=ColorTable;
		 while (p->name != NULL) {
		   if (p->MatchName(c)) {
				 color=p->GetColor();
				 return(0);
		   }
		   p++;
		 }
		 return(-1);
 }
 else  {
		Point tmp;
		stat=sscanf(c,"%f %f %f",&tmp.x,&tmp.y,&tmp.z);
		color =tmp;
		return (stat != 3? -1:0);
 }

}

// Read a shell from a wavefront file
int ReadShellWavefront(const char *FileName,GShellI *&s)
{

  AStream a(FileName,"r");
  char line[256];

  char group[256];				// Groupname
  char material[256];	// materialname

  GFaceList f(128);
  GVertexI vert;

  vert.Zero();

  group[0]=0;
  material[0]=0;
  Point color;

  int newGroup=1,newMaterial=0;
  int group_index=0,face_index=0;

  strcpy(group,"<Default>");
  strcpy(material,"<Default>");
  color.x = -1.0;


  if (!a.atEof()) {
	   if (s == NULL) s = new GShellI();

		s->v.SetMaxLength(2000);
		s->v.SetLength(0);

   while (!a.atEof()) {
		 
		 a.Skip();		// skip comments, nl ...

		 fgets(line,256,a.f);
		 { int l = strlen(line);  // strip nl
		   if ((l>0) && (line[l-1] == '\n') ) line[l-1]= 0;
		 }

		 switch (line[0]) {
		 case '#' : break;
		 case 'f' :
				if (line[1] == ' ') {  // face
				   char *buf;
				   int vi,vni;
				   int verti;

				   if (s->verts.Length() == 0) s->verts.SetMaxLength(max(s->v.Length(),s->vn.Length()));

				   if (newGroup || newMaterial) {
						 // for wavefront allways new group ? group_index = 	s->FindFaceGroup(group);
						 group_index = -1;
						 if (group_index <0) {	// append new group
								GFaceGroup newGroup;
								newGroup.color.x = -1.0;
								newGroup.SetName(group);
								newGroup.offset = face_index;
								if (color.x >= 0.0) newGroup.SetColor(color);

								//newGroup.SetColor();
							group_index =		s->AppendFaceGroup(newGroup);
						 }
						 newGroup=0;
						 newMaterial=0;
				   }
				   f.SetLength(1);

				   f[0]=0;
				   // Get next vertex//normal pair
				   buf = strtok(&line[2]," \t\n");
				   while ( buf != NULL) {
								if (strstr(buf,"//") != NULL) {
								    sscanf(buf,"%d//%d",&vi,&vni);
									vert.v = vi-1;
									vert.n = vni-1;
								} else {
								    sscanf(buf,"%d",&vi);
									vert.v = vi-1;
									vert.n = vi-1;
								}

								vert.c = vi-1;
								vert.p = vi-1;
								verti = s->AppendVertex(vert,1);

								f[0]++;
								f.Append(verti);
								buf = strtok(NULL," \t\n");
				   }
				   s->AppendFacelist(f);
				   s->fg.Append(group_index);			// face_index to group mapping
				   face_index++;
				   break;
				}
				TRACE("Read wavefront : unknown : %s\n",line);
				break;

		 case 'g' :
				if (line[1]==' ') {
				   //sscanf(&line[2],"%s",group); 
				   strcpy(group,&line[2]); // may contain blanks
				   newGroup=1;
				  break;
				}
				// g without name = start group
				TRACE("Read wavefront : unknown : %s\n",line);
				break;

		 case 'v' :
				if (line[1] == ' ') {  // vertex
				   Point p;
				   sscanf(&line[2],"%g %g %g %g",&p.x,&p.y,&p.z);
				   s->v.Append(p);
				   break;
				}
				if ((line[1] == 'n') && (line[2] == ' ')) {  // vertex normal
				   Point p;
				   if(s->vn.Length() == 0) s->vn.SetMaxLength(s->v.Length());
				   sscanf(&line[3],"%g %g %g %g",&p.x,&p.y,&p.z);
				   s->vn.Append(p);
				   break;
				}
				TRACE("Read wavefront : unknown : %s\n",line);
				break;
		 case 'u' :
				if (strncmp(line,"usemtl ",7) == 0) {
				   sscanf(&line[7],"%s",material);
				   if (strncmp(material,"fl",2) == 0) { // delete fl prefix
					  strcpy(material,material+2);
				   }
				   if (GetColor(material,color) !=0) {
						 TRACE("Read wavefront : unknow color : %s\n",material);
						 color.x = -1.0;
				   }
				   newMaterial=1;
				   break;
				}
		 case 's'  :
				//TRACE("Read wavefront : unknown : %s\n",line);
				// smoothing grouup ? to do
				break;
		 default :
				TRACE("Read wavefront : unknown : %s\n",line);
				break;
		 }
  }
  TRACE("V : %d vn %d  verts %d   fl %d \n",s->v.Length(),s->vn.Length(),s->verts.Length(), s->f.Length());
  
  s->UpdateActions();
  
  s->ComputeFaceNormals();
  if (s->vn.Length()>0) s->UnSetAction(SHELL_COMPUTE_VNORMALS);
  else s->ComputeVertexNormals();


  // Expand group colors into face colors
  if ((s->fgroup.Length()>1) && (s->fc.Length() == 0)) {
		 s->ComputeFaceColorsFromGroups();
  }
// ????????  s->edit_flags = 0;
//  s->UnSetEditFlag(SHELL_FACELIST_CHANGED | SHELL_VERTICES_CHANGED);
  }
  return(a.stat);
}


// Compute face colors from group tables
int GShellI::ComputeFaceColorsFromGroups()
{
  fc.Set(fg.Length(),Point(0.5,0.5,0.5));  // default for unassinged
  int i;
  int nok=0;
		  for (i=0; i < fg.Length();i++) {
				 int gi=fg[i];
				 if ((gi<0) || (gi>= fgroup.Length())) {
						TRACE("Bad group index %d for face # %d\n",gi,i);
						gi=0;
						continue;
				 }
				 GFaceGroup &g = fgroup[gi];
				 if (!g.HasColor() || (g.color.x < 0.0)) continue; // invalid
				 fc[i]= g.color;
				 nok++;
		  }
		  if (nok>0) {
//				SetFC(fc); is this ok ?
				SetFlag(SHELL_FCOLORS_COMPUTED);
		  }
		  else fc.Flush();

 return( nok >0 );
}

// compute a new shell object from a GShellI object
GShell *GShellI::ComputeShell(GShell *s,int copyAttributes)
{
  if (!s) {
	if (RTISA(this,GPolylineI))
		s = new GPolylineI();
	else   
	  s = new GShell();
  }	
  
  int vcnt = verts.Length();

  if (RTISA(s,GShellI)) {

	  GShellI *si = (GShellI *) s;
	  si->SetV(v);

	  si->SetVertices(verts);
	  if (copyAttributes) {
  		if (vn.Length() > 0)	si->SetVN(vn);
		if (vc.Length() > 0)	si->SetVC(vc);
		if (vp.Length() > 0)	si->SetVP(vp);
	  }

  } else {	
  
	s->v.SetLength(vcnt);
	if (copyAttributes) {
  		if (vn.Length() > 0)	s->vn.SetLength(vcnt);
		if (vc.Length() > 0)	s->vc.SetLength(vcnt);
		if (vp.Length() > 0)	s->vp.SetLength(vcnt);
	}

	// copy vertices 1-1
	GVertex vi;
	int i;
	for(i=0; i<vcnt; i++) {
		 GetVertex(i,vi);
		 s->SetVertex(i,vi);
	}

  }	

  s->SetFaceList(f);
  s->UpdateActions();

  if (copyAttributes && (s->vn.Length() > 0))
		s->UnSetAction(SHELL_COMPUTE_VNORMALS);
  
  
  if (copyAttributes) {
  	if (fn.Length()>0) s->SetFN(fn);
  	if (fc.Length()>0) s->SetFC(fc);
  	// to do : flags
	if (Flat()) s->SetFlat();
	if (GetFlag(SHELL_TRIANGULATED)) s->SetFlag(SHELL_TRIANGULATED);
  }

  return(s);
}


// compute a new shell object from a GShellI object
GShellI * CompressShell(GShellI *old, GShellI *s,int copyAttributes, int lookupAttributes)
{
	if (!s) { 
		if (RTISA(old,GPolylineI))
		s = new GPolylineI();
		else s = new GShellI();
	}
  
  int vcnt = old->verts.Length();
  int newVertsCnt = vcnt;
  
  int i, newI;

  IntArray usedVerts(vcnt,0);

  const GVertexI &maxIndices = old->GetMaxI();

  IntArray usedV(max(maxIndices.v,(int)old->v.Length()),-1);
  IntArray usedN(max(maxIndices.n,(int)old->vn.Length()),-1);
  IntArray usedP(max(maxIndices.p,(int)old->vp.Length()),-1);
  IntArray usedC(max(maxIndices.c,(int)old->vc.Length()),-1);

  int newVcnt=0,newNcnt=0,newCcnt=0,newPnt=0;

  if (lookupAttributes) {
	for (i=0; i<vcnt; i++) {
  		GVertexI &vert = old->verts[i];
		int oldVi = vert.v;
		Point thePoint = old->v[oldVi];
		int newVi = old->v.FindNearest(thePoint,GShell::maxLookupDistance2);
		if (newVi >=0) vert.v = newVi;
	}
	// color ? // vp ??
  }


  // step face list and mark used vertices
  const int *fp = old->f,*fpend=old->f+old->f.Length();
  
  int fi=-1;
  
  while ( fp < fpend ) {
  
	  const int *faceStart = fp;
	  int n = *fp++;

	  if (n<0) n=-n; // hole
	  else fi++;

	  while (n>0) {
		  int  vi = *fp++;
		  if (vi>= 0 && vi < vcnt)
			usedVerts[vi]=1;
		  n--;
	  }

  }	


  
  for (i=0; i<vcnt; i++) {
  	GVertexI &v = old->verts[i];
	if (!usedVerts[i]) continue;

	if (v.v>=0 && v.v<usedV.Length()) { usedV[v.v]=1; } 
	if (v.n>=0 && v.n<usedN.Length()) { usedN[v.n]=1;	}
	if (v.c>=0 && v.c<usedC.Length()) { usedC[v.c]=1; }
	if (v.p>=0 && v.p<usedP.Length()) { usedP[v.p]=1;	}
  }


  // copy V  ....................................
  newI= 0;
  for(i=0; i<usedV.Length(); i++) {
   if (usedV[i] == 1) {  usedV[i]=newI; newI++; }
  }
  s->SetV(newI,NULL);
  newI= 0;
  for(i=0; i<usedV.Length(); i++) {
   if (usedV[i] >= 0) {  s->v[newI] = old->v[i]; newI++;  }
  }

  // copy N  ....................................
  newI= 0;
  for(i=0; i<usedN.Length(); i++) {
   if (usedN[i] == 1) {  usedN[i]=newI; newI++; }
  }
  if (newI>0 && (old->vn.Length()>0)) {
	s->SetVN(newI,(Point *)NULL);
	newI= 0;
	for(i=0; i<usedN.Length(); i++) {
	if (usedN[i] >= 0) {  s->vn[newI] = old->vn[i]; newI++;  }
	}
  }

  // copy P  ....................................
  newI= 0;
  for(i=0; i<usedP.Length(); i++) {
   if (usedP[i] == 1) {  usedP[i]=newI; newI++; }
  }
  if (newI > 0 && (old->vp.Length()>0) ) {
  	s->SetVP(newI,NULL);
  	newI= 0;

	for(i=0; i<usedP.Length(); i++) {
   		if (usedP[i] >= 0) {  s->vp[newI] = old->vp[i]; newI++;  }
  	}
  }

  // copy C  ....................................
  newI= 0;
  for(i=0; i<usedC.Length(); i++) {
   if (usedC[i] == 1) {  usedC[i]=newI; newI++; }
  }
  if (newI > 0 && (old->vc.Length()>0) ) {
  	s->SetVC(newI,NULL);
  	newI= 0;
  	for(i=0; i<usedC.Length(); i++) {
   		if (usedC[i] >= 0) {  s->vc[newI] = old->vc[i]; newI++;  }
  	}
  }
  
  // count numer of newverts 
  newVertsCnt=0;

  for (i=0; i<vcnt; i++) {
	if (!usedVerts[i]) {
		  continue;	
	}
    newVertsCnt++;
  }	

  Array<GVertexI> newVerts(newVertsCnt);
  newI= 0;

  for (i=0; i<vcnt; i++) {
	if (!usedVerts[i]) {
		  usedVerts[i] = -1;
		  continue;	
	}


  	GVertexI &v = old->verts[i];
  	GVertexI &newV = newVerts[newI];
	
	newV.v=newV.n=newV.c=newV.p = newI;
    
	usedVerts[i] = newI; // vi old maps to this newVert
	newI++;

	if (v.v>=0 && v.v<usedV.Length()) { newV.v = usedV[v.v]; } 
	if (v.n>=0 && v.n<usedN.Length()) { newV.n = usedN[v.n]; } 
	if (v.c>=0 && v.c<usedC.Length()) { newV.c = usedC[v.c]; } 
	if (v.p>=0 && v.p<usedP.Length()) { newV.p = usedP[v.p]; } 

  }
  
  s->SetVertices(newVerts);

  s->SetFaceList(old->f);

  // step new face list and remap  vertices
  {
  int *fp = s->f,*fpend=s->f+s->f.Length();
  
  int fi=-1;
  
  // to think: remove bad triangles, indentical vertices etc.
  //
  while ( fp < fpend ) {
  
	  int *faceStart = fp;
	  int n = *fp++;

	  if (n<0) n=-n; // hole
	  else fi++;

	  while (n>0) {
		  int vi = *fp;
		  
		  if (vi>= 0 && vi < vcnt)
			vi = usedVerts[vi];
		  else vi=-1;

		  if (vi<0) {
			  vi = 0;
			  TRACE("Unexpected bad vertex index in face list \n");
		  }	

		  *fp = vi;
		  
		  fp++;

		  n--;
	  }

  }	
  }


  s->UpdateActions();
  if (!(old->actions & SHELL_COMPUTE_VNORMALS) && (s->vn.Length()>0)) 
      s->UnSetAction(SHELL_COMPUTE_VNORMALS);

  if (copyAttributes) {
  	if (old->fn.Length()>0) s->SetFN(old->fn);
  	if (old->fc.Length()>0) s->SetFC(old->fc);
  	// to do : flags
	if (old->Flat()) s->SetFlat();
  }
  
  return(s);
}


#if 0
int GShellI::Update()
{

  GShell:Update();

  if (edit_flags == 0) return(0);
  if (fn) has |= SHELL_FACE_NORMAL;
}
#endif

#include "collision/rapid/rapid.h"
void GShellI::ComputeRapidModel()
{
	const int *fp = f;
	const int *fpend = f + f.Length();
	int fi = -1;

	if (!rapidModel)
		rapidModel = new RAPID_model;

	  rapidModel->BeginModel();
	  while (fp<fpend) {
		  int n = *fp ++;
		  fi++;
		  if (n>=3) {
			int p0 = *fp++;
			int p1 = *fp++;
			n -=2;
			while (n>0) {
				int p2 = *fp++;
				rapidModel->AddTri(&v[verts[p0].v].x, &v[verts[p1].v].x, &v[verts[p2].v].x, fi);
				p1 = p2;
				n--;
			}
		  }
		  else fp+=n;
	  }

	  rapidModel->EndModel();
  	  UnSetAction(SHELL_UPDATE_RAPID_MODEL);

}

