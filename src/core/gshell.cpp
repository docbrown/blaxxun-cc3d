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

@module GShell.cpp - GLView core graphical primitves |

Copyright (c) 1995 - 1999 by Holger Grahn
All rights reserved

Purpose:
Implementation of graphical primitives GShell, GMesh ...
																	  
Classes:

Notes:

Changes:
HG 22.10.98 - switch OGL twosided lighting if solid FALSE & invert VN & FN if CCW = FALSE

HG 05.03.98 use smooth shading if vertex colors or fog for D3D 
But GShell IsFlat could be wrong on some shapes, can't pass face normal to D3D

$Revision: 1.42 $
$Log: gshell.cpp,v $
Revision 1.42  1999/07/06 16:54:53  tom
*** empty log message ***

Revision 1.17  1998/08/20 17:22:12  holger
empty message



Todo :

******************************************************************************/

#include "stdafx.h"

#include "gversion.h"
#include "gkatmai.h"
#include "collision/rapid/rapid.h"


//#include <math.h>
//#include <string.h>
//#include <stdlib.h>
//#include "point.h"
//#include "array.h"
//#include "glh.h"
#include "gutils.h"

#include "greporter.h"

#include "gclass.h"
#include "gshell.h"

#include "gshelli.h" // needed for RTISA()

#include "gshelltriang.h"



// for	ComputePrimitiveList
#include "tomesh.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//

#define COPYPOINTS(DEST,SRC,cnt) for(int i=0; i<cnt; i++) DEST[i]=SRC[i];


//
// GFaceList
//
void GFaceList::SetPolygon(int n) {

 Resize(n+1);
 int *fp = p;
 *fp++ = n;
 for(int i=0; i<n;i++) *fp++ = i;
}


//@cmember  Compute a Facelist for a point set 
int GFaceList::SetPoints(int nPoints,int start)
{
 Resize((nPoints-start)*2);
 int *fp = p;
 for(int i=start; i<nPoints;i++) {
	 *fp++ = 1; // face length of 1 
	 *fp++ = i;
 }
 return Length();

}

//@cmember  Compute a Facelist for a polyline index list 
int GFaceList::SetPolyline(GFaceList &f,int start)
{

  const int *fp = f.p+start,*fpend=f.p+f.Length();
  int fi=-1;

  while ( fp < fpend ) {
	 int n = *fp++;
	 if (n>=0) {
		 fi++;
	 } else n=-n;
	 
	 if (n>1) {
		int i1=*fp++;
		n--;
		while (n>0) {
			int i2 = *fp++;
			AppendEdge(i1,i2);
			i1=i2;
			n--;
		}
	 }	else if (n == 1) AppendPoint(*fp++);

  }
  return fi;
	// to do:
}






//@cmember  Set Faces list to inventor format  (-1 is separator)
int GFaceList::SetFaceListInventor(int lenf,const int *fp)
{
  int fi=-1;
  const int *fpend = fp +lenf;
  int *dest;

  Resize(lenf+2);
  dest = DataRW();
  int *destN;

  destN = dest;
  *dest++=0;
  if (*fp <0) fp++; // ignore leading -1 
  int cnt=0;

  while ( fp < fpend ) {
	  int n=*fp++;
	  if (n<0) {
		  *destN = cnt;
		  cnt = 0;
		  destN = dest;
		  *dest++ = 0;
	  } else {
		*dest++ = n;
		cnt++;
	  }
  }
  if (cnt>0) {
		  *destN = cnt;
  } 
  SetLength(dest-Data());

  return 0;
}



// Compute a Facelist for a Grid like topology
int GFaceList::SetGrid(int rows,int cols,gbool quads,gbool closeCols,gbool closeRows)
{  int w,h,i;
   int cnt;

   // compute size for facelist
   if (quads) {
	 cnt= (cols-1+closeCols) * (rows-1+closeRows) * 5;
   } else
	 cnt=((rows-1)*(cols-1)*2*4);

   Resize(cnt);
   int *fp = DataRW(); // stepping pointer


   i=0;
   if (!fp) return (-1);
   if (quads) {
	 for(h=0;h<(rows-1); h++,i+=cols) {
	 for(w=0;w<(cols-1);w++) {
		*fp++ = 4;
		*fp++=i+w;		 /* x,y   */
		*fp++=i+w+cols; /* x,y+1 */
		*fp++=i+w+1+cols; /* x+1,y+1 */
		*fp++=i+w+1;	 /* x+1,y	*/
	 }
	 if (closeCols) {
		w=cols-1;
		*fp++ = 4;
		*fp++=i+w;		 /* x,y   */
		*fp++=i+w+cols; /* x,y+1 */
		*fp++=i+0+cols; /* x+1,y+1 */
		*fp++=i+0;		 /* x+1,y	*/
	 }
	 }
	 if (closeRows) {
	 i= (rows-1) * cols;
	 for(w=0;w<(cols-1);w++) {
		*fp++ = 4;
		*fp++=i+w;		 /* x,y   */
		*fp++= w; /* x,y+1 */
		*fp++= w+1; /* x+1,y+1 */
		*fp++=i+w+1;	 /* x+1,y	*/
	 }
	 if (closeCols) {
		w=cols-1;
		*fp++ = 4;
		*fp++=i+w;		 /* x,y   */
		*fp++=w; /* x,y+1 */
		*fp++=0; /* x+1,y+1 */
		*fp++=i+0;		 /* x+1,y	*/
	 }

	 }

   } else {
	 /* triangles, like a mesh */
	 for(h=0;h<(rows-1); h++,i+=cols)
	 for(w=0;w<(cols-1);w++) {
		*fp++ = 3;
		*fp++=i+w;		 /* x,y   */
		*fp++=i+w+cols; /* x,y+1 */
		*fp++=i+w+1;	 /* x+1,y	*/
		*fp++ = 3;
		*fp++=i+w+1;	 /* x+1,y	*/
		*fp++=i+w+cols; /* x,y+1 */
		*fp++=i+w+1+cols; /* x+1,y+1 */
	 }
   }
   return (fp-Data()); // # of elements
}

/****************************************************************************/
/*
   find offset of "nth" face in facelist
   subtracted polyons do not count
*/

int GFaceList::GetFaceOffset(int nth)
{
  int *fp = p,*fpend=p+Length();
  int fi=-1;
  while ( fp < fpend ) {
	 int n = *fp;
	 if (n>=0) {
		fi ++;
		if (fi == nth) return(fp-p);
	 } else n=-n;
	 fp+=n+1;
  }
  return -1;
}

// Compute # faces for facelist, starting with an optional offset
int GFaceList::GetFaceCount(int offset)
{
  int *fp = p+offset,*fpend=p+Length();
  int fi=-1;
  while ( fp < fpend ) {
	 int n = *fp;
	 if (n>=0) {
		fi ++;
	 } else n=-n;
	 fp+=n+1;
  }
  return fi+1;
}

// Compute # triangles for facelist, starting with an optional offset (not correct for holes & nonconvex polys)
int GFaceList::GetTriangleCount(int offset)
{
  int *fp = p+offset,*fpend=p+Length();
  int fi=0;
  while ( fp < fpend ) {
	 int n = *fp;
	 if (n>=0) {
	 } else n=-n;
	 if (n>=2) fi+= (n-2);
	 fp+=n+1;
  }
  return fi;
}

// Compute # lines for facelist, starting with an optional offset 
// closed = 1, assume extra closing edge 
int GFaceList::GetLineCount(int offset,gbool closed_)
{
  int *fp = p+offset,*fpend=p+Length();
  int fi=0;
  int closed = (closed_ ? 1 : 0);

  while ( fp < fpend ) {
	 int n = *fp;
	 if (n>=0) {
	 } else n=-n;
	 if (n>1) fi+= (n-1)+closed;
	 fp+=n+1;
  }
  return fi;
}

// Compute # triangles for facelist, starting with an optional offset (not correct for holes & nonconvex polys)
int GFaceList::GetTriangleCountReal(int offset)
{
  int *fp = p+offset,*fpend=p+Length();
  int fi=0;
  while ( fp < fpend ) {
	 int n = *fp;
	 if (n>=0) {
	 } else n=-n;
	 if (n>2) fi+= (n-2);
	 fp+=n+1;
  }
  return fi;
}
// Compute # triangles for facelist, starting with an optional offset (not correct for holes & nonconvex polys)
int GFaceList::GetLineCountReal(int offset)
{
  int *fp = p+offset,*fpend=p+Length();
  int fi=0;
  while ( fp < fpend ) {
	 int n = *fp;
	 if (n>=0) {
	 } else n=-n;
	 if (n>1) fi+= (n-1);
	 fp+=n+1;
  }
  return fi;
}



// reverse the direction of individual facest
int GFaceList::ReverseFaces(int offset)
{
  //to do: implement
  int *fp = p+offset,*fpend=p+Length();
  int fi=-1;
  while ( fp < fpend ) {
	 int n = *fp;
	 if (n>=0) {
		fi ++;
	 } else n=-n;
	 fp+=n+1;
  }
  return fi+1;
}

#define ISSTRIPPED(a1,a2,a3,b1,b2,b3) ( ((b1 == a1) && (b2 == a3)) || ((b1 == a3) && (b2 == a2)))
#define ISDEGENERATED(a1,a2,a3) ((a1 == a2) || (a1 == a3) || (a2 == a3))

// optimize face list (for triangles, and strip building)
// remove degenerated triangles
int GFaceList::Optimize(gbool flip)
{
  int *fp = p,*fpend=p+Length();
  int *fout = fp;

  int fi=-1;
  int p1,p2,p3;
  p1=p2=p3=-1;

  int degenerated=0;

  while ( fp < fpend ) {
	
	
	int v1,v2,v3;
	int n = *fp++;
	if (n>=0) {
		fi ++;
	} else n=-n;
	 if (n<3)
		  fp+=n;
	 else 
	 if (n == 3) {
		 v1=*fp++;	
		 v2=*fp++;	
		 v3=*fp++;	
	    if (ISDEGENERATED(v1,v2,v3)) {
			TRACE("Degenerated Triangle found (%d %d %d) \n",v1,v2,v3);
			degenerated++;
		} else {
		 *fout++=3;

		 if (ISSTRIPPED(p1,p2,p3,v3,v1,v2)) {
			 *fout++=v3; *fout++=v1; *fout++=v2;
			 p1=v3,p2=v1,p3=v2;
			 TRACE("Strippable Triangle found (%d %d %d) \n",v1,v2,v3);
		 }	
		 else if (ISSTRIPPED(p1,p2,p3,v2,v3,v1)) {
			 *fout++=v2; *fout++=v3; *fout++=v1;
			 p1=v2,p2=v3,p3=v1;
			 TRACE("Strippable Triangle found (%d %d %d) \n",v1,v2,v3);
		 }	
		 else if (1 || ISSTRIPPED(p1,p2,p3,v1,v2,v3)) {
			 *fout++=v1; *fout++=v2; *fout++=v3;
			 p1=v1,p2=v2,p3=v3;
		 }	
		}
	 }  // triangle 		
	 else { 
		 int* foutstart = fout;

		 int first=1;
		 
		 int v0;
		 
		 v0 = *fp++;n--;
		 v2=-1;

		 int newn=0;
		 
		 while (n>0) {
			 v3=*fp++;
			 n--;
			 if ((v2 == v3) || ISDEGENERATED(v0,v2,v3)) {
				TRACE("Degenerated Triangle found (%d %d %d) \n",v0,v2,v3);
				degenerated++;
			 } else { 	
				 if (first && v2>=0) { // 
					 *fout++= n;
					 *fout++=v0;
					 *fout++=v2;
					 newn=2;
					 first=0;
				}
				v2=v3;
				if (!first) {
					*fout++ = v3;	newn++;
				}
			
			 }

		 }
		 *foutstart = newn;
		 if (newn<3) // total crap poly
			 fout = foutstart;
	 } // polygon case 
  }

  np = fout-p;

  TRACE("Degenerated triangles removed %d \n",degenerated);

  return fi+1;
}


// Compute # faces for facelist, starting with an optional offset + hole + triangle counters
int GFaceList::GetFaceCount(int offset,int *holecnt,int * tricnt)
{
  int *fp = p+offset,*fpend=p+Length();
  int fi=-1;
  *holecnt = 0;
  *tricnt=0;
  while ( fp < fpend ) {
	 int n = *fp;
	 if (n>=0) {
		fi ++;
		if (n == 3) (*tricnt)++;
	 } else { n=-n,(*holecnt)++; }
	 fp+=n+1;
  }
  return fi+1;
}

// compute size of cnt faces starting at offset in array elements
int GFaceList::GetFaceSize(int offset,int cnt)
{
  int *fp = p+offset,*fpend=p+Length();
  int fi=-1;

   if (fp>= fpend) return(-1);
   do {
   int n = *fp;
   if (n>=0) {
	   fi ++;
	   cnt--;
   } else n=-n;
   fp+=n+1;

   while ( (fp < fpend) && (*fp < 0) ) {
	 n = *fp;
	 if (n>=0) {
		fi ++;
	 } else n=-n;
	 fp+=n+1;
   }
   } while( (cnt>0) && (fp < fpend));
   return fp-(p+offset);
}

// Delete cnt faces starting a face number #nth
int GFaceList::DeleteNth(int nth,int cnt)
{
  int offset = GetFaceOffset(nth);
  if (offset<0) return(offset);
  int size = GetFaceSize(offset,cnt);
  if (size <0) return(size);
  Delete(offset,size);
  return(0);
}



/*

//triangulate a facelist into triangles 
int tesselateFaceList( 
						GFaceList &f, //@parm input facelist
						PointArray &v, //@parm corresponding vertices for indices in f
						IntArray *vmap, // @parm optional vertex map list
						GFaceList &newf, //@parm output: triangulated facelist
						IntArray *new2old,	//@parm optional mapping from new triangles to old face indices
                        GShell * theShell  //@parm optional shell to append vertices too
					  )
*/


// GVertex
// interpolate between two GVertex A + B
GVertex& Interpolate (const GVertex& A,float ta, const GVertex& B,float tb,
					GVertex &Result)
{
	Interpolate(A.v,ta,B.v,tb,Result.v);
	Interpolate(A.n,ta,B.n,tb,Result.n); Result.n.Normalize();
	Interpolate(A.c,ta,B.c,tb,Result.c);
	Interpolate(A.p,ta,B.p,tb,Result.p);
	return (Result);
}

#ifdef _3DV

int GVertex::io(AStream &a)
{
	v.io(a); n.io(a); c.io(a); p.io(a);
	if (a.IsOutput()) a.nl(); // to prevent to long lines !

   	return(a.stat);
}

// GVertexI
int GVertexI::io(AStream &a)
{  if (a.BeginBlock()) {
	  a.io(v); a.io(n); a.io(c); a.io(p);
	  a.EndBlock();
		  if (a.IsOutput()) a.nl(); // to prevent to long lines !
		}
	return(a.stat);
}

#endif




//
// GObject
//
#ifdef _GV
GV_NODE_SOURCE_IMP(GObject,GvNode);

#else

RTIMP(GObject,0,GGeometry)

#endif

GObject::GObject()
{ 
}

GObject::~GObject()
{
}

//
// GSubMesh - store a subset of shell faces & references vertices 
//

GSubMesh::~GSubMesh()
{
	if (s) delete s;
	if (next) delete next;
}



//
// GShell
//
#ifdef _GV
GV_NODE_SOURCE_IMP(GShell,GObject);
#else
RTIMP(GShell,0,GObject);
#endif

int GShell::initial_flags = 0;
int GShell::initial_edit_flags= SHELL_CREATED;
int GShell::maxSubMeshVertices=1020; // 256; // depends on Graphicscard & cache size


#ifdef _GSTATS
	// information for profiling
int  GShell::verticesProcessed;	// number of vertics processed
int  GShell::facesProcessed;		// number of faces processed
int  GShell::primsProcessed;		//
int	 GShell::trianglesDrawn;		// number of triangles sent to render layer
int	 GShell::facesDrawn;			// number of faces sent to render layer
int  GShell::maxPrimVertexCnt;	// size of largest vertex array
int  GShell::maxPrimTriCnt;		// size of largest vertex array

void GShell::ClearStats()
{
	verticesProcessed=0;
	facesProcessed = 0;
	primsProcessed = 0;
	trianglesDrawn=0;
	facesDrawn = 0;

	maxPrimVertexCnt = -1;
	maxPrimTriCnt = -1;

}

#endif



GShell::GShell() : GObject()
{
	Zero();
}

GShell::GShell( int new_nv,const Point *new_v,
				int new_nf,const int *new_f)  : GObject()
{
		Zero();
		SetV(new_nv,new_v);
		SetFaceList(new_nf,new_f);
};

// construct a grid like shell
GShell::GShell(int cols,int rows,const Point *new_v)
{
		Zero();
		SetV(cols*rows,new_v);
        f.SetGrid(rows,cols,/*quads*/ 1);
}




void GShell::Zero()
{
//  flags=0;
//  edit_flags = 0;
  has=0;
  flags= initial_flags;
  edit_flags = initial_edit_flags;
  actions=0;
  rapidModel = NULL;	

  maxI.v = maxI.n = maxI.p = maxI.c = 0;


//	v = NULL;
//	vn = NULL;
//	vc = NULL;
//	vp = NULL;

//	f = NULL;

  nfaces =0;
//	prims = NULL;

//	fn = NULL;
//	fc = NULL;

  finfo = NULL;
  e=NULL;
  ne=0;
  subMeshes = NULL;
}
#if 0
void GShell::Flush()
{
  flags=0;
  edit_flags = 0;
  has=0;

  v.Flush();
//	vn = NULL;
//	vc = NULL;
//	vp = NULL;

//	f = NULL;

  nfaces =0;
//	prims = NULL;
  prims.Flush();
//	fn = NULL;
//	fc = NULL;

  finfo = NULL;
  e=NULL;
  ne=0;
}
#endif

GShell::~GShell()
{

	DeleteSubMeshes();

/*
  delete fc;
  delete fn;

  delete vn;
  delete vp;
  delete vc;
  delete v;
*/
  delete finfo;

  delete e;

  delete rapidModel;
  

//	delete f;
//	delete prims;
}


#ifdef _3DV

// ouput array
int ioa(AStream &a,int &np, Point *&p)
{
  Point newpoint;
  int i;
  a.BeginBlock();

   if (a.isOutput()) {
		 if (p != NULL) {
		 for(i=0;i<np;i++) {
				if (i>1) a.Sep(' ');
				if ((i%4) == 3) a.nl();
				p[i].io(a);
		 }
		 }
   } else {
#if 0
		 PointArray x;
		 np=0;
		 /* read while ( until closing paren */
		 while (a.Skip() == '(') {
		   newpoint.io(a);
		   if (x.np == x.maxp) {
				 // make bigger
				 int oldnp = x.np;
				 x.Resize((x.np+10) + (x.np/2));
				 x.np=oldnp;
		   }
		   x.Append(newpoint);
		 }
		 p = x.p;
		 x.p= NULL; x.maxp = 0;
		 np=x.np; x.np=0;
#endif
  }

  a.EndBlock();
  return (a.stat);

}

#if 0
		 // read face list ( i i i i)
		 { int *flist = NULL;
		   int nflist=0,flist_max=0;
		   /* read until closing paren */
		   while (a.Skip() != ')') {
				  int i;
				  a.r(i);
				  append(flist,nflist,flist_max,&i,1);
		   }
		   s->SetFaceList(nflist,flist);
		   if (flist) FREE(flist);
		 }
		 a.Sep(')');

#endif

// IO : output shell attribute
int GShell::ioOutputAttributes(AStream &a)
{
		if (flags & SHELL_CONVEX) a.tok("convex");
		if (flags & SHELL_PLANAR) a.tok("planar");
		if (flags & SHELL_CLOSED) a.tok("closed");
		if (flags & SHELL_FLIP_NORMALS) a.tok("flip_normals");
		if (flags & SHELL_HAS_CONVEX_FACES) a.tok("convex_faces");

		if (v.Length()) {  a.tok("vertex");  v.io(a); }
		if (vn.Length() && ! (flags & SHELL_VNORMALS_COMPUTED)) {	a.nl(); a.tok("vertex_normals"); vn.io(a); }
		if (vc.Length()) {	a.nl(); a.tok("vertex_colors"); vc.io(a); }
		if (vp.Length()) {	a.nl(); a.tok("vertex_parameters"); vp.io(a); }


		if (f.Length()) {	a.nl(); a.tok("faces");
						  a.BeginBlock();
						  int *fp=f,*fpend=fp+f.Length();

						  while (fp<fpend) {
								int n=*fp++;

								a.nl(); // Each facet on new line ?
								a.io(n);
								if (n<0) n=-n;

								while (n>0) { a.io(*fp); fp++; n--; }
						  }

						  a.EndBlock();
				   }
		if ((fn.Length() >0) && ! (flags & SHELL_FNORMALS_COMPUTED)) {	a.nl(); a.tok("face_normals"); fn.io(a); }
		if ((fc.Length() >0) && ! (flags & SHELL_FCOLORS_COMPUTED)) {	a.nl(); a.tok("face_colors"); fc.io(a); }
		if (prims.Length()) { a.tok("primitives"); prims.io(a); } // to do: really save ?
		return(0);
};



// IO : check current token and input attribute
int GShell::ioInputOneAttribute(AStream &a)
{		  PointArray p(2048);
		  if (a.tok("vertex")) { p.io(a); SetV(p); }
		  else	if (a.tok("vertex_normals")) { p.io(a); SetVN(p); }
		  else	if (a.tok("vertex_parameters")) { p.io(a); SetVP(p); }
		  else	if (a.tok("vertex_colors")) { p.io(a); SetVC(p); }
		  else	if (a.tok("faces")) {
						IntArray fl(4000); fl.io(a);
						SetFaceList(fl.Length(),fl);
						GetFaceCount();
		  }
		  else	if (a.tok("primitives")) {
						IntArray fl(4000); fl.io(a);
						SetPrimitiveList(fl.Length(),fl);
		  }
		  else	if (a.tok("face_normals")) { p.io(a); SetFN(p); }
		  else	if (a.tok("face_colors")) { p.io(a); SetFC(p); }

		  // flags
		  else	if (a.tok("concave"))  ; //  SetFlag(SHELL_CONVEX);
		  else	if (a.tok("convex"))  SetFlag(SHELL_CONVEX);
		  else	if (a.tok("planar"))  SetFlag(SHELL_PLANAR);
		  else	if (a.tok("closed"))  SetFlag(SHELL_CLOSED);
		  else	if (a.tok("flip_normals"))  SetFlag(SHELL_FLIP_NORMALS);
		  else	if (a.tok("convex_faces"))  SetFlag(SHELL_HAS_CONVEX_FACES);

		  else return(0);
		  return(1);
};


// io a shell from a AStream

int GShell::io(AStream &a)
{
  int polyline = RTISA(this,GPolyline);

  if (polyline) { 	
	  if (!a.tok("polyline")) return(0);
  }
  else if (!a.tok("shell")) return(0);

  a.BeginBlock();

  if (a.isOutput()) {
		char buf[512];
		sprintf(buf,"Vertices = %d, Faces = %d",v.Length(),nfaces);
		a.comment(buf);
		BBox &bbox = GetBBox();

		if (!bbox.IsEmpty())
		{ Point center(bbox.Center()), size(bbox.Size());
		  sprintf(buf,"bboxCenter %f %f %f bboxSize %f %f %f",center.x,center.y,center.z,size.x,size.y,size.z);
		  a.comment(buf);
		}

		a.BeginTok();
		   ioOutputAttributes(a);
		a.EndTok();
  } else {
		  if (a.BeginTok())
		  do {
		  if (ioInputOneAttribute(a)) {}
		  else {
				TRACE("Shell::IO Token %s ??? \n",a.curtok);
		  }

		  } while (a.EndTok());

		  SetEditFlag(SHELL_CHANGED);
		  UnSetEditFlag(SHELL_VERTICES_CHANGED | SHELL_FACELIST_CHANGED);
  }

  a.EndBlock();

  return(a.stat);

}
#endif _3DV


// Copy only raw geometry part
//

GShell * GShell::CopyGeometry(GShell *dest) const
{  GShell *g;

   if (!dest) { 		// allocate new
		g=new GShell(v.Length(),v,f.Length(),f);
		return(g);
   }
   else {				// copy
		 dest->SetV(v.Length(),v);
		 dest->SetFaceList(f.Length(),f);
		 return(dest);
   }
}

GShell*  GShell::Copy(GShell *dest) const
{ GShell *g;
  g = CopyGeometry(dest);
  if (fn.Length()>0) g->SetFN(fn);
  if (fc.Length()) g->SetFC(fc);
  if (vn.Length()) g->SetVN(vn);
  if (vc.Length()) g->SetVC(vc);
  if (vp.Length()) g->SetVP(vp);
//		g->UnSetEdit(flag,SHELL_VERTICES_CHANGED);
  return(g);
}

int GShell::UpdateActions()
{
  if (edit_flags == 0) return(0);
  has = 0;
  if (v)  has |= SHELL_VERTEX;
  if (vn) has |= SHELL_VERTEX_NORMAL;
  if (vc) has |= SHELL_VERTEX_COLOR;
  if (vp) has |= SHELL_VERTEX_PARAMETER;
  if (fn) has |= SHELL_FACE_NORMAL;
  if (fc) has |= SHELL_FACE_COLOR;

  if ((edit_flags & SHELL_VERTICES_CHANGED) ||
	 (edit_flags & SHELL_CREATED) )
  { 
  	SetAction(SHELL_COMPUTE_BBOX|SHELL_COMPUTE_MAXI|SHELL_UPDATE_RAPID_MODEL);
  }

  if ( (edit_flags & SHELL_VERTICES_CHANGED) || (edit_flags & SHELL_VERTEX_ATTRIBUTES_CHANGED)) {
	  if (! (edit_flags  & SHELL_VERTS_LENGTH_CHANGED))
		SetAction(SHELL_UPDATE_EXBUF_VERT);
      else SetAction(SHELL_COMPUTE_EXBUF);
  }
  if ( (edit_flags & SHELL_FACE_ATTRIBUTES_CHANGED)) {
      SetAction(SHELL_COMPUTE_EXBUF);
  }


  if ((edit_flags & SHELL_VERTICES_CHANGED) ||
		  (edit_flags & SHELL_FACELIST_CHANGED) )
  { 	int flag=0;
		
		SetAction(SHELL_COMPUTE_FNORMALS);
		SetAction(SHELL_COMPUTE_VNORMALS);
  		SetAction(SHELL_COMPUTE_MAXI);


  }
  if (edit_flags & SHELL_FACELIST_CHANGED||
	 (edit_flags & SHELL_CREATED) ) {
         SetAction(SHELL_COMPUTE_EXBUF);
  		 SetAction(SHELL_UPDATE_RAPID_MODEL);

   		 UnSetFlag(SHELL_TRIANGULATED);
   		 UnSetFlag(SHELL_HAS_HOLE_FACES);
		 nfaces=0; 
		 nfaces = GetFaceCount();
		 if (flags & SHELL_EDGES_OK) {
		   	if (e) delete e;
		   	e=NULL;
		   	ne=0;
		   	flags &= ~ SHELL_EDGES_OK;
		 }
		 if (flags & SHELL_COMPUTE_PRIMLIST)
				SetAction(SHELL_COMPUTE_PRIMITIVELIST);
		 else
		   	if (prims.Length() >0) prims.Flush();		// Flush prims because facelist has changed
       SetAction(SHELL_COMPUTE_EXBUF); // new 19.09.
  }

  edit_flags = SHELL_CHANGED;
  return(1);
}


/*
  update internal state

*/

int GShell::Update()
{

  int ret = UpdateActions();
#if 0
#ifdef _KATMAI
  if (   (actions & SHELL_UPDATE_EXBUF_VERT) 
	  || (actions & SHELL_COMPUTE_EXBUF)
	  || (edit_flags & SHELL_VERTICES_CHANGED) || (edit_flags & SHELL_VERTEX_ATTRIBUTES_CHANGED)) {
	  kv .Set(v);
	  if (vn.Length()>0)   kvn.Set(vn);
	  if (vp.Length()>0)   kvp.Set(vp);
	  if (vc.Length()>0)   kvc.Set(vc);
  }
#endif
#endif



  if (actions & SHELL_COMPUTE_PRIMITIVELIST) {
  		ComputePrimitiveList();
  }
  return(ret);
}

// set vertices
void
GShell::SetV(int new_nv,const Point *new_v)
{
   if (new_nv !=v.Length()) edit_flags |= SHELL_VERTS_LENGTH_CHANGED;
   v.Set(new_nv,new_v);
   edit_flags |= SHELL_VERTICES_CHANGED;
   SetAction(SHELL_COMPUTE_BBOX);

}

void
GShell::SetV(const PointArray &new_v)
{
   if (new_v.Length() !=v.Length()) edit_flags |= SHELL_VERTS_LENGTH_CHANGED;
   v.Set(new_v);
   edit_flags |= SHELL_VERTICES_CHANGED;
   SetAction(SHELL_COMPUTE_BBOX);
}

// use reference to memory pointer only
void 
GShell::SetVByRef(int new_nv,const Point *new_v)
{
   if (new_nv !=v.Length()) edit_flags |= SHELL_VERTS_LENGTH_CHANGED;
   v.SetByRef(new_nv,new_v);
   edit_flags |= SHELL_VERTICES_CHANGED;
   SetAction(SHELL_COMPUTE_BBOX);
}


// set vertex normals
void
GShell::SetVN(int n_vn,const Point *new_vn,int normalize)
{
   vn.Set(n_vn,new_vn);
   if (normalize) vn.Normalize();
   //???????????? vn.Resize(max(n_vn,v.Length()));
   edit_flags |= SHELL_VERTEX_ATTRIBUTES_CHANGED;
   UnSetAction(SHELL_COMPUTE_VNORMALS);
}
void
GShell::SetVNByRef(int n_vn,const Point *new_vn,int normalize)
{
   if (!normalize) {
		vn.SetByRef(n_vn,new_vn);
   } else {
	   vn.Set(n_vn,new_vn);
		if (normalize) vn.Normalize();
		// ??????????? vn.Resize(max(n_vn,v.Length()));
   }	
   edit_flags |= SHELL_VERTEX_ATTRIBUTES_CHANGED;
   UnSetAction(SHELL_COMPUTE_VNORMALS);
}


void
GShell::SetVN(const PointArray &new_vn,int normalize)
{
   vn.Set(new_vn);
   if (normalize) vn.Normalize();
   //?????????? vn.Resize(max(new_vn.Length(),v.Length()));
   edit_flags |= SHELL_VERTEX_ATTRIBUTES_CHANGED;
   UnSetAction(SHELL_COMPUTE_VNORMALS);
}

// set vertex rgb colors
void
GShell::SetVC(int n_vc,const Point *new_vc)
{
   vc.Set(n_vc,new_vc);
   // ????????? vc.Resize(max(n_vc,v.Length()));
   edit_flags |= SHELL_VERTEX_ATTRIBUTES_CHANGED;
}

void
GShell::SetVCByRef(int n_vc,const Point *new_vc)
{
   vc.SetByRef(n_vc,new_vc);
   // ????????? vc.Resize(max(n_vc,v.Length()));
   edit_flags |= SHELL_VERTEX_ATTRIBUTES_CHANGED;
}

void
GShell::SetVC(const PointArray	&new_vc)
{
   vc.Set(new_vc);
   //???????????vc.Resize(max(new_vc.Length(),v.Length()));
   edit_flags |= SHELL_VERTEX_ATTRIBUTES_CHANGED;
}

// set vertex parameter for texture mapping
void
GShell::SetVP(int n_vp,const Point *new_vp)
{
   vp.Set(n_vp,new_vp);
   // ???????????? vp.Resize(max(n_vp,v.Length()));
   edit_flags |= SHELL_VERTEX_ATTRIBUTES_CHANGED;
}

void
GShell::SetVPByRef(int n_vp,const Point *new_vp)
{
   vp.SetByRef(n_vp,new_vp);
   // ???????????? vp.Resize(max(n_vp,v.Length()));
   edit_flags |= SHELL_VERTEX_ATTRIBUTES_CHANGED;
}

void
GShell::SetVP(const PointArray &new_vp)
{
   vp.Set(new_vp);
   //vp.Resize(max(new_vp.Length(),v.Length()));
   edit_flags |= SHELL_VERTEX_ATTRIBUTES_CHANGED;
}

// set face normals
void
GShell::SetFN(int n_fn,const Point *new_fn,int normalize)
{
   fn.Set(n_fn,new_fn);

   if (normalize) fn.Normalize();

   fn.Resize(max(n_fn,GetFaceCount()));
   
   UpdateActions(); 
   
   SetEditFlag(SHELL_FACE_ATTRIBUTES_CHANGED);
   UnSetAction(SHELL_COMPUTE_FNORMALS);
}


void
GShell::SetFN(const PointArray &new_fn,int normalize)
{
   fn.Set(new_fn);
   if (normalize) fn.Normalize();
   fn.Resize(max(new_fn.Length(),GetFaceCount()));
   UpdateActions(); 

   SetEditFlag(SHELL_FACE_ATTRIBUTES_CHANGED);
   UnSetAction(SHELL_COMPUTE_FNORMALS);
}

// set face colors
void
GShell::SetFC(int n_fc,const Point *new_fc)
{
   fc.Set(n_fc,new_fc);
   fc.Resize(max(n_fc,GetFaceCount()));

   edit_flags |= SHELL_FACE_ATTRIBUTES_CHANGED;
}

void
GShell::SetFC(const PointArray &new_fc)
{
   fc.Set(new_fc);
   fc.Resize(max(new_fc.Length(),GetFaceCount()));

   edit_flags |= SHELL_FACE_ATTRIBUTES_CHANGED;
}

//set face colors by index map 
void GShell::SetFCByIndex(int n_fci,const int *new_fci,int n_fc,const Point *new_fc)
{
	// if bad data, set flush FC
   	if ( (n_fci<=0) || (new_fci==NULL) || (n_fc<=0) || (new_fc==NULL) ) { 
		fc.Flush();
	    edit_flags |= SHELL_FACE_ATTRIBUTES_CHANGED;
		return;
	}

	fc.Resize(max(n_fci,GetFaceCount()));
    
	const int *fci_end = new_fci+n_fci;
	const int *fci = new_fci;
	while (fci < fci_end) {
		if (*fci>=0 && *fci < n_fc)
		   fc[fci-new_fci]= new_fc[*fci];
		else fc[fci-new_fci].Set(1,1,1);
		fci++;
	}

    edit_flags |= SHELL_FACE_ATTRIBUTES_CHANGED;

}


// set face color indices 
void
GShell::SetFCIndex(int n_fci,const int *new_fci)
{
   fcI.Set(n_fci,new_fci);
   fcI.Resize(max(n_fci,GetFaceCount()));

   edit_flags |= SHELL_FACE_ATTRIBUTES_CHANGED;
}

void
GShell::SetFCIndex(const IntArray &new_fci)
{
   fcI.Set(new_fci);
   fcI.Resize(max(new_fci.Length(),GetFaceCount()));

   edit_flags |= SHELL_FACE_ATTRIBUTES_CHANGED;
}



// SetFaceList
void
GShell::SetFaceList(int new_nf,const int *new_f)
{
   f.Set(new_nf,new_f);

   edit_flags |= SHELL_FACELIST_CHANGED;

   UnSetFlag(SHELL_TRIANGULATED);
   UnSetFlag(SHELL_HAS_HOLE_FACES);

   nfaces = 0; // trigger recompute
}


void
GShell::SetFaceList(const GFaceList &new_f)
{
   f.Set(new_f);
   edit_flags |= SHELL_FACELIST_CHANGED;

   UnSetFlag(SHELL_TRIANGULATED);
   UnSetFlag(SHELL_HAS_HOLE_FACES);

   nfaces = 0; // trigger recompute
}


void GShell::SetPrimitiveList(int new_nprims,const int *new_prims)
{
   prims.Set(new_nprims,new_prims);
   edit_flags |= SHELL_PRIMLIST_CHANGED;
}

void GShell::SetPrimitiveList(const IntArray & new_prims)
{
   prims.Set(new_prims);
   edit_flags |= SHELL_PRIMLIST_CHANGED;
}

// SetEdgeList
void
GShell::SetEdgeList(int new_ne,const int *new_e)
{
   if (e != NULL) delete e;
   ne = new_ne;
   e = new int [ne*2];
   int i;
   for (i=0; i < ne*2; i++) e[i] = new_e[i];
   flags |=SHELL_EDGES_OK;
}

// SetEdgeList
void
GShell::SetEdgeListByRef(int new_ne,int *new_e)
{
   if (e != NULL) delete e;
   ne = new_ne;
   e = new_e;
   flags |=SHELL_EDGES_OK;
}


// get functions, recompute values if necessary
const PointArray& GShell::GetFN() 
{
	if ((fn.Length() == 0) || (actions & SHELL_COMPUTE_FNORMALS)) {
		ComputeFaceNormals();
	}
	return (fn);

}
const PointArray& GShell::GetVN()
{
	if ((vn.Length() == 0) || (actions & SHELL_COMPUTE_VNORMALS)) {
		ComputeVertexNormals();

	}
	return (vn);

}

const PointArray& GShell::GetVP()
{
	return(vp);
}

const PointArray& GShell::GetVC()
{
	return(vc);
}

const PointArray& GShell::GetV()
{
	return(v);
}

const PointArray& GShell::GetFC()
{
	return(fc);
}


// Editing functions

// Find nearest points
int GShell::FindNearestV(const Point &p,float &distance2)
{
  return v.FindNearest(p,distance2);
}

// if sqr of distance of two pts are less than this, they are
// considered equal
float GShell::maxLookupDistance2= 1.E-6;

// append a single point, optionally with lookup
int GShell::AppendV(const Point &p,int doLookup)
{		int newVi=-1;
		if (doLookup) {
		   float d2;
		   newVi = FindNearestV(p,d2);
		   if (d2 > maxLookupDistance2) newVi = -1;
		}
		if (newVi == -1) {

		   newVi = v.Length();

		   v.Append(p);

		   SetEditFlag(SHELL_VERTICES_CHANGED);
		}
		return (newVi);
}

int GShell::AppendVertex(const GVertex &vert) 
{
   int i;
   i=v.Length();
   v.Append(vert.v);
   if (vn.Length()>0) { if (i>=vn.Length()) vn.SetLength(i+1); vn[i] = vert.n; }
   if (vc.Length()>0) { if (i>=vc.Length()) vc.SetLength(i+1); vc[i] = vert.c; }
   if (vp.Length()>0) { if (i>=vp.Length()) vp.SetLength(i+1); vp[i] = vert.p; }

   SetEditFlag(SHELL_VERTICES_CHANGED | SHELL_VERTS_LENGTH_CHANGED | SHELL_VERTEX_ATTRIBUTES_CHANGED);

   return i;

}


// append a single vertex given by a single point, optionally with lookup
// you must manually update the other attriburtes !
int GShell::AppendVertex(const Point &p,int doLookup)
{
  return(AppendV(p,doLookup));
}
// append a list of points, optionally lookup points and return vertex indices in old2new
int GShell::AppendVertex(const PointArray &p,int doLookup,IntArray *old2new)
{
  return(AppendV(p,doLookup,old2new));
}

// append a list of points, optionally lookup points and return vertex indices in old2new
int GShell::AppendV(const PointArray &p,int doLookup,IntArray *old2new, const Matrix *m)
{
		
		if (m) { // translate points if necessary 
			PointArray pm(p);
//			pm *= *m;	// Transform p by m, and append translated coordinates
			pm.Mult(*m);	// Transform p by m, and append translated coordinates
			return(AppendV(pm,doLookup,old2new,NULL));
		}
		
		if (old2new) old2new->SetLength(p.Length());

		if (doLookup) {
		   int i;
		   // Lookup each point
		   for (i=0; i<p.Length(); i++) {

				int newVi;
				float d2;
				newVi = FindNearestV(p[i],d2);

				if ((newVi == -1) || (d2 > maxLookupDistance2)) {
						 newVi = v.Length();
						 v.Append(p[i]);
				}

				if (old2new) (*old2new)[i]=newVi;

			} // for i
			SetEditFlag(SHELL_VERTICES_CHANGED);

		} else {
		   v.Append(p);
		   SetEditFlag(SHELL_VERTICES_CHANGED);
		   if (old2new) {
				   int i;
				   int newVi= v.Length()-p.Length();
				   for(i=0; i<p.Length(); i++)
						(*old2new)[i]=newVi+i;;
		   }

		}
		return(0);
}


// append facelist, optionally map vertex indices by old2new
int GShell::AppendFacelist(IntArray &p,int doLookup,IntArray *old2new)
{
// to to doLookup
		f.Append(p);
		SetEditFlag(SHELL_FACELIST_CHANGED);
		nfaces = 0;
		return(0);
}

//append a single edge
int GShell::AppendEdge(Point &p1,Point &p2,int doLookup)
{
  IntArray tri(3);
  tri[0]=2;
  tri[1]=AppendV(p1,doLookup);
  tri[2]=AppendV(p2,doLookup);
  return(AppendFacelist(tri));
}

//append a single triangle
int GShell::AppendTriangle(Point &p1,Point &p2, Point &p3,int doLookup)
{
  IntArray tri(4);
  tri[0]=3;
  tri[1]=AppendV(p1,doLookup);
  tri[2]=AppendV(p2,doLookup);
  tri[3]=AppendV(p3,doLookup);
  return(AppendFacelist(tri));
}

// append a single polygon
int GShell::AppendPolygon(const PointArray &pts,int doLookup)
{ IntArray old2new;

  int oldLength = v.Length();

  AppendV(pts,doLookup,&old2new);

  // create face list

  IntArray flist(pts.Length()+1);
  { 	int *fp=flist.DataRW();
  	    int n;
		*fp++ = pts.Length();
		for (n=0; n< pts.Length(); n++)
				 *fp++ = old2new[n];
  }

  // append face list using remapping
  return(AppendFacelist(flist, 0));
}


// append whole shell
int GShell::Append(GShell &s,int doLookup, const Matrix *m)
{ IntArray old2new;

  int oldLength = v.Length();
  
  int oldLengthF = GetFaceCount();

  AppendV(s.v,doLookup,&old2new,m);

   // translate vertex indices from s.f into vertex indices in *this
  GFaceList mappedFlist;
  s.GetFaceList(mappedFlist);

  { int *fp=mappedFlist.DataRW(),*fpend=fp+mappedFlist.Length();
		while (fp < fpend){
		  int n = *fp++;
		  if (n<0) n=-n;
		  while (n>0) {
				 *fp = old2new[*fp];
				 fp++;
				 n--;
		  }
		}
  }

  // append face list using remapping
  AppendFacelist(mappedFlist); // to do: do mapping in append 
//  AppendFacelist(mappedFlist, 1,&old2new);

  // Copy attributes ?
  if (!doLookup) {
		 // vertex attributes
		 if (!m && vn.Length() == oldLength && s.vn.Length()>0) {
				vn.Append(s.vn);
		 } else vn.Flush();
		 if ((vc.Length() == oldLength) && (s.vc.Length()>0)) {
				vc.Append(s.vc);
		 } else if (vc.Length() > 0) {
		 	vc.SetLength(v.Length());
			// assume caller with assign new vertex colors !!!!!!!!!!!!!!!!!!!!
		 } else vc.Flush();
		 if ((vp.Length() == oldLength) && (s.vp.Length()>0)) {
				vp.Append(s.vp);
		 } else vp.Flush();
		 // face attributes
		 
		 if (!m && (fn.Length() == oldLengthF) && (s.fn.Length()>0)) {
				fn.Append(s.fn);
		 } else fn.Flush();

		 if ((fc.Length() == oldLengthF) && (s.fc.Length()>0)) {
				fc.Append(s.fc);
		 } else if (fc.Length() > 0) {
		 	fc.SetLength(GetFaceCount());
			// assume caller with assign new face colors !!!!!!!!!!!!!!!!!!!!
		 }
		 else fc.Flush();
  }

  return(0);
}



void GShell::ComputeBBox(BBox &box)
{
  const Point *vp,*vpend;
  Point mi,ma;

  if (v.Length()<=0) return;

  vp= v;
  vpend=&v[v.Length()];

  mi=ma= *vp;

  *vp++;

  for (; vp<vpend;vp++) {
		vp->MinMax(mi,ma);
  }

  box.mi = mi;
  box.ma = ma;

}


// get bounding box from all vertices, will be recomputed if not up to date
BBox& GShell::GetBBox() 
{

  if ((actions & SHELL_COMPUTE_BBOX) || (edit_flags & SHELL_VERTICES_CHANGED)) {
  	ComputeBBox(bbox);
	
	UnSetAction(SHELL_COMPUTE_BBOX);

  }
  return(bbox);
}

// get bounding box of node / Gv Virtual function 
GvBool GShell::getBBox(BBox &bbox)
{
	bbox = GetBBox();
	return !bbox.IsEmpty();
}


/* Compute the center of all points by averaging */
void GShell::ComputeCenter(Point &center)
{
  const Point *vp,*vpend;
  double X,Y,Z;
  if (v.Length()<=0) return;

  vp= v;
  vpend=&vp[v.Length()];

  X=Y=Z=0.0;
  for (; vp<vpend;vp++) {
		X+=vp->x;
		Y+=vp->y;
		Z+=vp->z;
  }

  double f= 1.0/ (double) v.Length();
  center.Set(X*f,Y*f,Z*f);
}


int GShell::ComputeMaxVertexI( GVertexI &vert)
{

	vert.v = v.Length();		// the vertex coordinates
	vert.n = vn.Length();		// the vertex normal
	vert.c = vc.Length();		// the vertex color
	vert.p = vp.Length();	// the vertex parameter
	return(0);
}


GVertexI& GShell::GetMaxI() 
{
	if (actions & SHELL_COMPUTE_MAXI) {
		ComputeMaxVertexI(maxI);
  		UnSetAction(SHELL_COMPUTE_MAXI);
	}
	return(maxI);
}





// Compute a facelist containing triangles only in tri
// tri2face maps triangle # i to shells face number #fi
// Return: number of triangles
// this implementation is valid only for concave polygons without holes
int
GShell::ComputeTriangles(GFaceList &tri,IntArray &tri2face,int useTriangulator)
{
  // already triangulated
  if (flags & SHELL_TRIANGULATED) {
	 tri = f;
	 tri2face.SetLength(GetFaceCount());
	 for (int i=0; i <tri2face.Length(); i++) tri2face[i]=i;
	 return 0;
  }


#ifdef _OGL
  if (useTriangulator || (flags & SHELL_HAS_HOLE_FACES) || (flags & SHELL_HAS_CONVEX_FACES)) {
  
  	tesselateFaceList(f,v,NULL,tri,&tri2face,this);
	// new v might be added
	// to do : change layout
	return (tri.Length() / 4); // return # of triangles
  }
#else
#ifdef _MESA_TESS
  if (useTriangulator || (flags & SHELL_HAS_HOLE_FACES) || (flags & SHELL_HAS_CONVEX_FACES)) {
  	mesa_tesselateFaceList(f,v,NULL,tri,&tri2face,this);
	return (tri.Length() / 4); // return # of triangles
  }

#else
  if (useTriangulator || (flags & SHELL_HAS_HOLE_FACES) || (flags & SHELL_HAS_CONVEX_FACES)) {
  
  	tesselateFaceList(f,v,NULL,tri,&tri2face,this);

	// new v might be added
	// to do : change layout
	return (tri.Length() / 4); // return # of triangles
  }
#endif
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

// triangulate the shell using ComputeTriangles with triangulator
int
GShell::Triangulate()
{  GFaceList tri;
   IntArray tri2face;
   gbool keepVn=gfalse;
   
   if (!Flat()) { // && vn.Length()>0)
	   keepVn = gtrue;
	   const PointArray &vnTmp = GetVN(); // update vertexnormals
   }


   ComputeTriangles(tri,tri2face,1);
   
   SetFaceList(tri);
   
   UpdateActions(); 

   if (keepVn) // keep old vertex normals 
	   UnSetAction(SHELL_COMPUTE_VNORMALS);   

   SetFlag(SHELL_TRIANGULATED);
   
   
   if (!Flat()) 
     UnSetAction(SHELL_COMPUTE_VNORMALS);

   // to do, recopy face colors, normals ?????

   if (fc.Length() >0) {
		PointArray newfc(tri2face.Length());
		for (int i=0; i<tri2face.Length(); i++) {
			newfc[i]=fc[tri2face[i]];
		}
		SetFC(newfc);
   }

   if (fn.Length() >0) {
		PointArray newfn(tri2face.Length());
		for (int i=0; i<tri2face.Length(); i++) {
			newfn[i]=fn[tri2face[i]];
		}
		SetFN(newfn);
   }

   return(0);

}


// get "native" vertex parameters
// implementation  simply returns vertex coordinates
void GShell::GetVertexParametersNative(int n,Point *vp)
{
  //vp.Resize(nv);
  int i=0;
  for (i=0; i < v.Length(); i++) {
		 vp[i] = v[i];
		 if (i>=n) break;
  }
}

void
GShell::ComputeBounds()
{
  const Point *vp,*vpend;
  Point mi,ma;

  if (v.Length()<=0) return;

  vp= v;
  vpend=&v[v.Length()];

  mi=ma= *vp;

  *vp++;

  for (; vp<vpend;vp++) {
		vp->MinMax(mi,ma);
  }


  flags |=SHELL_BOUNDS_OK;
}


void
GShell::Cleanup()
{
 if (finfo) delete finfo; finfo = NULL;

}

void
GShell::Make(int new_nv,const Point *new_v,
				int new_nf,const int *new_f)
{
  SetV(new_nv,new_v);
  SetFaceList(new_nf,new_f);
}

// Count Number of Faces in Facelist

int
GShell::CountFaces()
{
   int fi = f.GetFaceCount();
// printf("Count faces = %d \n",fi);

   return(fi);
}

// return number of faces and update triangulated and HOLE flags
int GShell::GetFaceCount()
{
   if (nfaces <=0) {
        int tricnt,holecnt;
	    nfaces= f.GetFaceCount(0,&holecnt,&tricnt);
		if (tricnt == nfaces) SetFlag(SHELL_TRIANGULATED);
		if (holecnt>0) SetFlag(SHELL_HAS_HOLE_FACES);
   }
   return(nfaces);
}

// get a facelist of shell
int GShell::GetFaceList(GFaceList &flist)
{
  flist.Set(f);
  return(flist.Length());
}


// Delete cnt faces starting at face # nthOffset,
// nthOffset <0, start from end
int GShell::DeleteFaces(int nthOffset,int cnt)
{
   if (nthOffset <0) {
	  nthOffset = f.GetFaceCount() + nthOffset; // ?? to do
   }
   if (nthOffset < 0) return -1;
   f.Delete(nthOffset,cnt);
   if (fn.Length() >0) fn.Delete(nthOffset,cnt);
   if (fc.Length() >0) fc.Delete(nthOffset,cnt);
   SetEditFlag(SHELL_FACELIST_CHANGED|SHELL_ATTRIBUTES_CHANGED);
   nfaces = 0;
   return(0);
}
// Delete cnt verts starting at vert # nthOffset,
int GShell::DeleteVerts(int nthOffset,int cnt)
{
   if (nthOffset <0) {
	  nthOffset = v.Length() + nthOffset;
   }
   if (nthOffset < 0) return -1;
   v.Delete(nthOffset,cnt);
   if (vn.Length() >0) vn.Delete(nthOffset,cnt);
   if (vc.Length() >0) vc.Delete(nthOffset,cnt);
   if (vp.Length() >0) vp.Delete(nthOffset,cnt);
   SetEditFlag(SHELL_VERTICES_CHANGED|SHELL_ATTRIBUTES_CHANGED);
   return(0);
}

// compute face normals for each polygon and store them into fn
void
GShell::ComputeFaceNormals(int allocFaceinfo)
{
   const int *fp =f;			   // face list stepping pointers
   const int *fpend= fp+f.Length();
   int fi=0;	// face index
   Plane plane;

   UnSetAction(SHELL_COMPUTE_FNORMALS);

   if (f.Length() <= 0) return;

   if (nfaces <=0) {
		nfaces= CountFaces();
   }

   if (nfaces <=0) return;

   fn.Resize(nfaces);
   fn.UnRef();

   if (allocFaceinfo) {
	   if (finfo != NULL) delete [] finfo; // bad should be handled with edit flags 
	   finfo = new shell_face_info [nfaces];
   }

   fi = -1;
   do { 		// for all faces
		 int flen = *fp++; // length of this face
		 int ishole=0;
		 if (flen<0) { ishole=1; flen=-flen; }
		 else fi++;

		 if ( !ishole) {

		 compute_polygon_plane_index(flen,fp,v,&plane);

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
		 } // !ishole
		 fp += flen;
   }
   while (fp < fpend);
   SetFlag(SHELL_FNORMALS_COMPUTED);

   if (NormalsFlipped()) fn *= Point(-1.0,-1.0,-1.0);  // flip normals
}

// result 0 not planar, 1: shell is planar, plane computed, 2: plane of first poly
int GShell::isPlanar(Plane &plane)
{
    if (f.Length() <= 3) return 0;
	int vi=f[1];

	const PointArray& fn = GetFN();
	if (fn.Length()==0) return (0);
	
	plane.Set(v[vi],fn[0]);

    if (nfaces == 1 || ((flags & SHELL_PLANAR) !=0) ) return(1);
	return(nfaces);

}

// is shell convex 
gbool GShell::isConvex()
{	
	return((flags & SHELL_CONVEX) !=0);
}

// is p outside the object
// may fall back to very simply outside bbox computation
// needed for Occlusion node 

gbool GShell::isOutside(const Point &p) 
{
   return !GetBBox().Inside(p);

}




//////////////////////////////////////////////////////////////////////////////
typedef struct {
		 int vi,fi;
	   } vertex_face;

static int comp_vertex_face(void const *in_a, void const *in_b)
{
   vertex_face *a, *b;

   a = (vertex_face*)in_a;
   b = (vertex_face*)in_b;
   if (a->vi < b->vi) return -1;
   else if (a->vi > b->vi) return 1;
   else return(0);

}

// compute vertex normals for each polygon and vertex and store them into vn
int
GShell::ComputeVertexNormals()
{
   UnSetAction(SHELL_COMPUTE_VNORMALS);

   //TRACE("Computing vertex normals \n");
   if (f.Length() <= 0) return(0);
   if (nfaces <=0) {
	   nfaces= GetFaceCount();
   }

   if (nfaces <=0) return(-1);

   // need face normals for computation
   const PointArray &fnormals = GetFN();
   
   if (fnormals.Length() == 0) return(-1);


   if (vn.Length() < v.Length()) vn.SetLength(v.Length()); 
   if (vn.Length() == 0) return(-1);

   vn.UnRef();


   // Zero vn array
   { Point *p = vn.DataRW(),*pend = &p[vn.Length()];
   	 while (p < pend) { 	p->Zero();	p++; }
   }


   const int *fp = f;	  // face list stepping pointers
   const int *fpend= fp+f.Length();
   int fi=-1;	// face index
   int vnLen = vn.Length();


   do { 		// for all faces
		 int flen = * fp ++; // length of this face
		 if (flen<0) flen=-flen;  
		 else fi++;

		 int i;
		 for (i=0; i< flen; i++) {
				int vi = *fp++; // vertex index
				if (vi<vnLen)  // if to large ignore
					vn[vi] += fnormals[fi];	// add face normal to vn slot
		 }
   } while (fp <fpend);

   // Normalize vn array
   { Point *p = vn.DataRW(),*pend = &p[vn.Length()];
   while (p < pend) {
				p->Normalize();
				p++;
   }
   }
   SetFlag(SHELL_VNORMALS_COMPUTED);



#if 0
   int *fp =f;			   // face list stepping pointers
   int *fpend= fp+f.Length();
   // old 
   vertex_face *vf_list= new vertex_face[f.Length()-nfaces+1]; // ?? to do
   if (vf_list == NULL) return(-1);

   vertex_face *vfp=vf_list,*vf_end;

   do { 		// for all faces
		 int flen = * fp ++; // length of this face
		 if (flen<0) flen=-flen;

		 int i;
		 for (i=0; i< flen; i++) {
		 int vi = *fp++; // vertex index
		 vfp->vi = vi;
		 vfp->fi = fi;
		 vfp++;
		 }

		 fi++ ;
   }
   while (fp < fpend);

   vf_end = vfp;
   vf_end->vi =  -1;

   // sort vf_list
   qsort(vf_list,vf_end-vf_list,sizeof(vertex_face),comp_vertex_face);


   for(vfp = vf_list; vfp<vf_end; ) {

		   /*d*/Point n;
		   int vi,fi;
		   vi = vfp->vi;
		   fi = vfp->fi;
//		 printf("V %d : %d ",vfp->vi,vfp->fi);
		   vfp++;
		   n.Set(fn[fi].x,fn[fi].y,fn[fi].z);

		   while (vfp->vi == vi) {		// for all other faces meeting at the vertex
		  	   fi = vfp->fi;
		  		n+=fn[fi];	// add face normal to vertex normal
			  vfp++;
		   }
		   // vn = |n|
		   double l = n.Len();
		   if (l>0.0) l = 1.0 / l;

		   vn[vi].Set(n.x*l,n.y*l,n.z*l);
   }
   delete vf_list;
   SetFlag(SHELL_VNORMALS_COMPUTED);
#endif
   return(1);
}


// flip normals during computation of faces normals == this propagates to vertexnormals
int GShell::FlipNormals()
{
  if (NormalsFlipped())   UnSetFlag(SHELL_FLIP_NORMALS); else  SetFlag(SHELL_FLIP_NORMALS);

  fn.UnRef();
  vn.UnRef();

  fn *= Point(-1.0,-1.0,-1.0);
  vn *= Point(-1.0,-1.0,-1.0);
  SetEditFlag(SHELL_VERTEX_ATTRIBUTES_CHANGED);
  return (NormalsFlipped());
}

// compute vertex colors for each vertex from attached faces face color 
int
GShell::ComputeVertexColorsFromFaceColors()
{
   if (f.Length() <= 0) return(0);

   if (nfaces <=0) {
	   nfaces= GetFaceCount();
   }

   if (nfaces <=0) return(-1);

   // need face normals for computation
   const PointArray &fc = GetFC();
   
   if (fc.Length() == 0) { 
	   SetVC(0,NULL); // flush vertex colors
	   return(-1);
   }

   if (vc.Length() < v.Length()) vc.SetLength(v.Length()); 
   if (vc.Length() == 0) return(-1);

   vc.UnRef();


   IntArray vcnt(v.Length()); // counters of color

   // Zero vc array
   { Point *p = vc.DataRW(),*pend = &p[vc.Length()];
     int *pcnt = vcnt;
   	 while (p < pend) {	p->Zero();	p++; *pcnt++=0; }
   }


   const int *fp = f;	  // face list stepping pointers
   const int *fpend= fp+f.Length();
   int fi=-1;	// face index
   int vcLen = vc.Length();


   do { // for all faces
		 int flen = *fp ++; // length of this face
		 if (flen<0) flen=-flen;   
		 else fi++;

		 int i;
		 for (i=0; i< flen; i++) {
				int vi = *fp++; // vertex index
				if (vi<vcLen) {  // if to large ignore
					if (fcI.Length() >0) { // indexed 
						vc[vi] += fc[fcI[fi]];	// add face color to vc slot
						vcnt[vi]++;
					}
					else { 
						vc[vi] += fc[fi];	// add face color to vc slot
						vcnt[vi]++;
					}
				}
		 }
   } while (fp <fpend);

   // Normalize vc array
   { Point *p = vc.DataRW(),*pend = &p[vc.Length()];
     const int *pcnt = vcnt;
     while (p < pend) {
				if (*pcnt>0) {
					*p *= (1.0f / (float) *pcnt); 
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
GShell::ComputeFaceColorsFromVertexColors()
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
   int vcLen = vc.Length();


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
				if (vi<vcLen) {  // if to large ignore
					fcsum += vc[vi];	// add vertex color to sum 
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
		EdgeTable(int NV) {
			nv = NV;
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


// compute the list of edges for all polygons and store them into e
int
GShell::ComputeEdgeList()
{
   const int *fp =f;			   // face list stepping pointers
   const int *fpend= fp+f.Length();
   int fi=0;	// face index


//	 printf("Computing edge list \n");
   if (f.Length() <= 0) return(0);

   EdgeTable t(v.Length());


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
//		 t.free();

   return(1);
}

/*  Evaluate object as object of type "type"
  if return = this, don´t destroy result, otherwise destroy */
//GvNode* /*GShell* */  

GEvalAs_TYPE*
GShell::EvalAs(TypeId type,int copyAttributes)
{
  if (type == Id()) return(this); // already indentical
#if 0
  if (IsA(type)) {
	  // is of derived types
	  return(this); // or copy ??
  }
#endif

#if 0  
  if (type == GShellI::ClassId()) {
	 GShellI *s = new GShellI();
	 ComputeShell(s,copyAttributes);
	 return(s);
  }
#endif
  // else cannot convert
  return(NULL);
}







// ********** OpenGL
#ifdef _OGL
/*
void glVertex(const Point &p) { glVertex3fv(&p.x); }
void glNormal(const Point &p) { glNormal3fv(&p.x); }
void glColor(const Point &p)  { glColor3fv(&p.x); }
void glTexCoord(const Point &p) { glTexCoord3fv(&p.x); }
*/
inline void glNormalFlip(const Point &p) { glNormal3f(-p.x,-p.y,-p.z); }

#define OUTVERT(I) \
				   if (VC) glColor(vc[I]); \
				   if (VP) glTexCoord(vp[I]); \
				   if (VN) if (flipNormals) glNormalFlip(vn[I]); else glNormal(vn[I]); 	 \
				   glVertex(v[I]);

#define OUTVERT_VN(I) \
				   glNormal(vn[I]); 	 \
				   glVertex(v[I]);

void GShell::RenderGlFaces(RenderState &state,int FN, int VN, int VC, int VP)
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
       } 
   }    


   if (VP) if (vp.Length() == 0)		VP =0;

   
   if ((prims.Length() > 0) 
        && !FC)     // 16.06. Tristrips doesn´t support Face colors yet
   {  // use precomputed triangle strips
		  RenderGlPrimitives(state,FN,VN,VC,VP);
		  return;
   }


   if (f.Length() == 0) return;	// no faces
   const int *fp =f;					// face list stepping pointers
   const int *fpend= fp+f.Length();   // points after last element of facelist
   int fi=-1;					// face index

   int mode=0;	// 1 triangles



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

		if (state.useCompiledVertexArrayEXT) 
			state.glLockArraysEXT(0,v.Length());


   do { 		// for all faces

		 int flen = * fp ++; 	// length of this face
		 if (flen<0) flen=-flen;   else fi++;

		 int i;

		 if (flen==3) { 		// TRIANGLE
				if (mode ==0 ) {
				   glBegin(GL_TRIANGLES);
				   mode = 1;
				 }
				 {
				   i= *fp++;
				   if (FN) if (flipNormals) glNormalFlip(fn[fi]); else glNormal(fn[fi]);
				   if (FC) glColor(fc[fi]);
				   state.glArrayElementEXT(i);	
				   i= *fp++;
				   state.glArrayElementEXT(i);	
				   i= *fp++;
				   state.glArrayElementEXT(i);	
				 }
		 }
		 else { 				// generic convex polygon
		 if (mode == 1) {		 // close last primitive
				 glEnd();
				 mode = 0;
		 }
		 glBegin(GL_POLYGON);
		 if (FN) if (flipNormals) glNormalFlip(fn[fi]); else glNormal(fn[fi]);
		 if (FC) glColor(fc[fi]);
		 for (i=0; i< flen; i++) {
				 int vi = *fp++; // vertex index
				 state.glArrayElementEXT(vi);	
		 }
		 glEnd();

		 }								// end convex polygon
   }
   while (fp < fpend);

   if (mode == 1) {
				 glEnd();
				 mode = 0;
		}
		glDisable(GL_VERTEX_ARRAY_EXT); // necessary ??

		if (state.useCompiledVertexArrayEXT) 
			state.glUnlockArraysEXT();


   
   		return;
   } // Vertex Array Extension

   do { 		// for all faces

		 int flen = * fp ++; 	// length of this face
		 if (flen<0) flen=-flen;   else fi++;

		 int i;

		 if (flen==3) { 		// TRIANGLE
				if (mode ==0 ) {
				   glBegin(GL_TRIANGLES);
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
   }
   while (fp < fpend);

   if (mode == 1) {
				 glEnd();
				 mode = 0;
   }
   state.SetLightModel(lightModel);

}

// Render primitive list
// stored for support all GL glBegin primitives

// Faceattributes only supported for SHELL_TRISTRIP_FACEINDEX

void GShell::RenderGlPrimitives(RenderState &state,int FN, int VN, int VC, int VP)
{
   int *fp =prims;			// face list stepping pointers
   int fi=0;			  	// face index

   gbool flipNormals = state.frontFaceCW && !state.twosidedLighting;


   // need face normals for computation
   if (FN) {
		  if (fn.Length() == 0) ComputeFaceNormals();
		  if (fn.Length() == 0) FN=0; /* return(-1); */
   }

   if (VN) if (vn.Length() == 0)		VN =0; //return(-1);
   if (VC) if (vc.Length() == 0)		VC =0;
   if (VP) if (vp.Length() == 0)		VP =0;
   int FC = VC ? 0 : fc.Length() != 0;	// face colors if no vertex colors


   int op = - * fp++;			// the opcode
   if (op == SHELL_TRISTRIP_FACEINDEX) {
  	  if (VN && !VP && !VC && !FN) {  // optimized case

	  do {
	  	int i,fi;

	  	glBegin(GL_TRIANGLE_STRIP);
		   i = *fp++; OUTVERT_VN(i);
		   i = *fp++; OUTVERT_VN(i);

		   for(;;) {  // for all vertices, until new opcode == i<0
				fi = *fp++;
				if (fi<0) { glEnd(); op = -fi; break;}
				i = *fp ++;
				OUTVERT_VN(i);
		   }

	  	} while (op == SHELL_TRISTRIP_FACEINDEX);
	  
	  } // OPT
	  else {
	  
	  do {
	  	int i,fi;

	  	glBegin(GL_TRIANGLE_STRIP);
		   i = *fp++; OUTVERT(i);
		   i = *fp++; OUTVERT(i);

		   for(;;) {  // for all vertices, until new opcode == i<0
				fi = *fp++;
				if (fi<0) { glEnd(); op = -fi; break;}
				i = *fp ++;
				if (FN) if (flipNormals) glNormalFlip(fn[fi]); else glNormal(fn[fi]);
				if (FC) glColor(fc[fi]);
				OUTVERT(i);
		   }

	  	} while (op == SHELL_TRISTRIP_FACEINDEX);
	  }

   }
   for(;;) {	// for all primitives

		 if (op == SHELL_END) break;


		 glBegin((GLenum) (op-SHELL_POINTS+GL_POINTS));  // assume ops are numbered same way as in gl.h

//				   if (FN) if (flipNormals) glNormalFlip(fn[fi]); else glNormal(fn[i]);
				for(;;) {						// for all vertices, until new opcode == i<0

				int i= *fp ++;
				if (i<0) {
						glEnd();
						op = -i;
						break;
				}
				OUTVERT(i);
				}
  } 	// for primitives
}


const  int GL_MAX_LINES=256;	// Windows crashes if to many lines send

void GShell::RenderGlEdges(RenderState &state,int VN,int VC,int VP)
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
            RenderGlFaces(state,0 /*FN*/ ,VN, 1,VP);
       glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
       return; 
   }


   if (!(flags & SHELL_EDGES_OK)) {
		  ComputeEdgeList();
   }
   if (ne == 0) return;

   int *ep=e;
   int *epend = e + ne*2;

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

//
// Render the vertics of the object
//
void GShell::RenderGlVertices(RenderState &state,int VN,int VC)
{
   gbool flipNormals = state.frontFaceCW && !state.twosidedLighting;

   if (VN) if (vn.Length() == 0)		VN =0;
   if (VC) if (vc.Length() == 0)		VC =0;

   Point *vp=v;
   Point *vpend = vp + v.Length();
   Point *n= vp;

   if (state.vertexDrawStyle && state.vertexDrawStyle->enabled) {
				// vertexDrawStyle -> SetFlags(VN
				// to do VC
				while (vp < vpend)
				{
						//if (VC) state.SetColor(vc[vp-v]);
						if (VN) n = &vn[vp-v];
						state.vertexDrawStyle->RenderVertex(*vp,*n);
						vp++;
						if (state.Abort()) break;
				}

				return;
   }

   glBegin(GL_POINTS);
   while (vp < vpend)
   {
		 if (VC) glColor(vc[vp-v]);
		 if (VN) if (flipNormals) glNormalFlip(vn[vp-v]); else glNormal(vn[vp-v]);
		 glVertex(*vp);
		 vp++;
  }
  glEnd();
}

//
// Render all vertex normals of object
//
void GShell::RenderGlNormals(RenderState &state,float scale,int VN,int FN)
{
   if (VN) if (vn.Length() == 0) 	VN =0;

   if (VN) {
   Point *vp=v;
   Point *vpend = vp + v.Length();


   if (state.normalDrawStyle && state.normalDrawStyle->enabled) {
				while (vp < vpend)
				{		Point *n;

						//if (VC) state.SetColor(vc[vp-v]);
						if (VN) n = &vn[vp-v];
						state.normalDrawStyle->RenderNormal(*vp,*n);
						vp++;
						if (state.Abort()) break;
				}
   }
   else {
   int cnt=GL_MAX_LINES;

   glBegin(GL_LINES);
   while (vp < vpend)
   {  const Point *n= &vn[vp-v];

		 glNormal(*n);
		 glVertex(*vp);
		 Point p2= *vp + *n * scale;
		 glNormal(*n);
		 glVertex(p2);
		 vp++;

		 cnt --;
		 if (cnt <=0) {
				cnt = GL_MAX_LINES;
				glEnd();
				glBegin(GL_LINES);
		 }
  }
  glEnd();
  }
  } // VN
}

//rendering Bounding box of shell directly to OpenGL
void GShell::RenderGlBBox(RenderState &state)
{ 
   BBox & box = GetBBox();	
   state.RenderCubeEdges(box.mi,box.ma);
}


int GShell::glRender(RenderState &state)
{
#if 0
				 if ((state.renderMode == GRenderSolid) && (state.edges.visible))  {

						 if (state.sbuffer) {
						   // should draw only one poly a time */
						   glColor3f(0.0,0.0,0.0);
						   /* render edges to color & sbuffer */
						   RenderGlEdges(state,0);
						   glStencilFunc(GL_EQUAL,0,1);
						   glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);

				   glColor3f(1.0, 1.0, 0.5);

						    RenderGlFaces(state,0,0);  /* draw if stencil is zero */
						   /* clear stenceil */
						   glStencilFunc(GL_ALWAYS,0,1);
						   glStencilOp(GL_INVERT,GL_INVERT,GL_INVERT);
						   glColor3f(0.0,0.0,0.0);

						   /* render edges to color & sbuffer */
						   RenderGlEdges(state,0);


						 } else
						 {
						 glColor3f(0.0,0.0,0.0);
						 glDepthFunc(GL_LEQUAL);
						 theShell->RenderGlEdges(state,0);

				 glDepthFunc(GL_LESS);

						 state.material.diffuse.glSet(); // non-lighted color
						 if (state.faces.visible)
						 theShell->RenderGlFaces(state,state.faces.smooth == 0,
																  state.faces.smooth>0,
																  state.faces.colored,state.faces.textured);

						 glColor3f(0.0,0.0,0.0);

						 glDepthFunc(GL_LEQUAL);
						 if (state.edges.visible)
						theShell->RenderGlEdges(state,state.edges.lighted,state.edges.colored);


						 glDepthFunc(GL_LESS);
						 }
				 } else
#endif
				 {

					// render faces
					if (state.SetFaceAttributes()) {
						  int doVn = state.faces.smooth && !Flat();	
						  RenderGlFaces(state,!doVn,
										  doVn,
										  state.faces.colored,
										  state.faces.textured);
					}

					if (state.SetEdgeAttributes())
						   RenderGlEdges(state,state.edges.lighted,
								state.edges.colored,
								state.edges.textured);

				 }
				 
				 // render vertices
				 if (state.SetVertexAttributes())
						  RenderGlVertices(state,state.vertices.lighted,
						  state.vertices.colored);
#ifndef _GLVIEW_IE
				 if (state.SetNormalAttributes()) {
						  BBox &box =GetBBox();
						  Point d=box.Size();
						  RenderGlNormals(state,d.Len()*0.1,1);
				 }
				 if (state.SetBBoxAttributes()) {
						  RenderGlBBox(state);
				 } 
#endif

		  return(0);
}
#endif

#ifdef _D3D

#include "gd3device.h"
#include "gcamera.h"


void KatmaiComputeLighting(
					 const Point &n, // normal 
					 
					 const Point &p, // position
					 const Point &viewp, // viewer position

					 const float* diffuse, // 3 floats, diffuse color to use

					 G3Material &material, // current  material 
					 
					 G3LightHandle *lights, // linked list of lights

					 RGBAColor &c // in : set to mat emmissive & alpha
								  // out : resulting color	
					 )
{
	G3LightHandle *l=lights;

	while ( l != NULL ) {	// for all lights 
		// if l->enabled 

		// switch(l->dltType) 					
				float f = n.x * l->dvDirection.x + n.y * l->dvDirection.y + n.z * l->dvDirection.z;
				
				if(f<0.0f) f=-f; // f=0.0f;
				else if(f>1.0f) f=1.0f;

				c.r+=diffuse[0]*l->dcvColor.r *f,
				c.g+=diffuse[1]*l->dcvColor.g *f,
				c.b+=diffuse[2]*l->dcvColor.b *f;

				l = l->next;
			}

}


// approximation for shininess
// strange using x87 ???
#define SPOW(dot,shin) (dot / ((float)shin-(float)shin*dot+dot))
//#define SPOW(dot,shin)  pow(dot,shin)

// Build Line List for DrawPrimitive

int BuildLineList(GFaceList &f,gbool isPolygon,Array<WORD> &prims)
{
   const int *fp = f.Data();  // face list stepping pointers
   const int *fpend= fp+f.Length();
   int fi=-1;	// face index

	// make tri buffer large enough 
	if (prims.Length() <(f.Length()*3))  // account for culling 
		prims.SetLength(f.Length()*3);

	WORD *fpi = prims.DataRW(); // write pointer 


	while (fp < fpend) {	// while more faces
		int flen = *fp++; 	// length of this face
		
		ASSERT(flen>=0);

		//if (flen<0) flen=-flen;   // should not happen at this level
		//else 
		fi++;
		if (flen > 1) {

			int p0 = fp[0]; 
			int p1 = p0;
			int i=1;
			do 
			{
				fpi[0] = p1; 
				p1 = fp[i];
				fpi[1] =p1;
				fpi+=2; 
				i++;
			} while (i<flen);

			// closing line
			if ((p1 != p0) && isPolygon) {
				fpi[0] = p1; 
				fpi[1] = p0;
				fpi+=2; 
			}
		}
		fp+=flen;

	}
	ASSERT(fpi < (prims.Data() + prims.MaxLength()));	
	return (fpi - prims.Data());
}






//===========================================================================================
//void GShell::TransformLightVertices(GglRenderTraversal &traversalState, D3DTLVERTEX *vtlp)
//===========================================================================================




#define D3D_maxVerts 65000


void GShell::TransformLightVertices(GglRenderTraversal &traversalState, D3DTLVERTEX *vtlp)
{

	// Transform & Light
	RenderState &state = *traversalState.glState;
	gbool culling = state.backfaceCulling; // backface culling enabled ?
	gbool singlsidelighting = culling;

	int nv = v.Length();

	int i;

	// matrix 
	GMatrixStack *mstack = traversalState.GetCurrentMatrix();

    if (!mstack->viewpointLocalOk) { // need to transform it
		mstack->SetViewpointLocal(traversalState.physicalCamera->position);
	}  
	const Point &viewpoint = mstack->viewpointLocal;

	// Matrix M = mstack->GetMatrix()* state.view *state.projection;
	
	if (!mstack->model2screenOk) {
		mstack->model2screen = mstack->GetMatrix()* state.view *state.projection;
		mstack->model2screenOk = 1;
		// state.lightsTransformed = 0; Done by SetMatrix(m)

		// KatmaiSetMatrix(mstack->model2screen.m)
	}
	Matrix& M =	mstack->model2screen; // transforming model to screen 

	gbool updateLights= !state.lightsTransformed; // need to transform lights on each matrix change

	// viewport translation & scale

	float cx,w;// = state.device->szClient.cx*0.5f;
	float cy,h;// = state.device->szClient.cy*0.5f;
	RECT & r=state.device->rcViewportRect;

	w=(r.right - r.left)*0.5;
	h=(r.bottom - r.top)*0.5;

	cy= r.bottom - h;
	cx= r.left + w;


	gbool doVp = state.textureDefined && vp.Length()>0;
	gbool doVn =  ! state.Unlit();

	if (!doVn) {
		if (!this->isUnlit || this->currentColorD3D != state.currentColorD3D) // need update colors in vertex buffer 
			actions |= SHELL_UPDATE_EXBUF_VERT;
	}	


	if (
#ifdef _KATMAI		
		kv.Length() ==0 || 
#endif
		(actions & SHELL_UPDATE_EXBUF_VERT) 
		) {

#ifdef _KATMAI
		kv.Set(v); // copy from AOS to SOA 
#endif


		// recreate texture coordinates	
		textureMatrixVersion = state.GetTextureMatrixVersion();

		if (vp.Length()>0) {
			D3DTLVERTEX *vtlp= ( D3DTLVERTEX*)  vtl.Data();
			
			if (!state.textureMatrixIdentity) {
				Matrix &M = state.textureMatrix;
				for(i = 0;  i < nv; i++) {
					const Point &p =vp[i];
					vtlp->tu = p.x * M.x[0][0] + p.y * M.x[1][0] /* + p.z * M.x[2][0] */ + M.x[3][0];
					vtlp->tv=  p.x * M.x[0][1] + p.y * M.x[1][1] /* + p.z * M.x[2][1] */+ M.x[3][1];
					vtlp ++;
				}
			} 
			else 
			{  
				for(i = 0;  i < nv; i++) {
					const Point &p =vp[i];
					vtlp->tu = p.x;
					vtlp->tv = p.y;
					vtlp ++;
				}
			}

		} else {
			//vtlp->tu = 0;
			//vtlp->tv = 0;
		}
		
		if (!doVn) { // no lighting
			
			D3DTLVERTEX *vtlp= ( D3DTLVERTEX*) vtl.Data();

			float a=state.alpha;

			if (vc.Length()>0) { 
				for(i = 0;  i < nv; i++,vtlp++) {
					const Point &c = vc[i]; // we have vertex colors 
					vtlp->color = D3DRGBA_X(c.x,c.y,c.z,a);
					vtlp->specular = 0; // vtlp->color; 
				}
			} else  {
				D3DCOLOR c= state.currentColorD3D;
				for(i = 0;  i < nv; i++,vtlp++) {
					vtlp->color = c; 
					vtlp->specular = 0; 
				}
			}
			this->currentColorD3D = state.currentColorD3D;
			this->isUnlit = gtrue;
		}
	    //if (vp.Length()>0)   kvp.Set(vp);
	    //if (vc.Length()>0)   kvc.Set(vc);

	}


    gbool flipNormals =  state.frontFaceCW; 

	if (doVn) 
	{
		const PointArray &vntmp = GetVN(); // get/update vertex normals 
	
		if (vntmp.Length() == 0) doVn =0; 

		if (doVn && (kvn.Length() == 0) || (actions & SHELL_UPDATE_EXBUF_VERT)) {
		
			if (flipNormals) {
				kvn.Set(vn,-1.0f);

				//int l = kvn.Length()*3;
				//for (int j=0;j<l;j++) kvn.p[j]*= -1.0f;
			} else kvn.Set(vn);

		} 
	}

	gbool doVc = vc.Length()>0; // we have color per vertex, replace diffuseColor
	
	G3Material &mat = state.currentMaterial->material;
	RGBAColor start(mat.emissive); // start with emissive color

	// do we need specular lighting ?
	gbool doSpecular = (mat.specular.r>0) || (mat.specular.g>0) || (mat.specular.b>0);

	gbool doVertexCull = gfalse;

	// IntArray vcull(nv,0);

	// cache lighting values 
	if (doVn ) {

		this->isUnlit = gfalse;

		// transform lights from world to local 
		if (updateLights) {
		G3LightHandle *l=state.lights;

		int numLights=0;

		Matrix& worldInv = mstack->GetMatrixInverse();

		while ( l != NULL ) {	// for all lights 
			
			l->directionLocal = RotateOnly(worldInv, *((Point *)&l->dvDirection));
			l->directionLocal.Normalize();
			l->positionLocal = worldInv * (*((Point *)&l->dvPosition));
			// to do : range , attenuation 

			if (l->enabled) {

				if (numLights == 0) state.firstLight = l;

#ifdef _KATMAI
			if (useKatmai) {
			KSetNumLights(numLights+1);
			switch(l->dltType) { 					
			case  D3DLIGHT_DIRECTIONAL : 
			    KSetLightType(numLights,KDIFFUSE_LIGHT);

				KSetLightDirection(numLights, -l->directionLocal);
				break;

			case  D3DLIGHT_POINT : 
			case  D3DLIGHT_PARALLELPOINT : // actually not used
			    KSetLightType(numLights,KPOINT_LIGHT);
				KSetLightPosition(numLights, l->positionLocal);
			    KSetLightAttenuation(numLights,(l->flags & GL_ATTENUATION),l->dvAttenuation0,l->dvAttenuation1,l->dvAttenuation2);

				break;

			case  D3DLIGHT_SPOT : 
			    KSetLightType(numLights,KSPOT_LIGHT);
				KSetLightDirection(numLights, - l->directionLocal);
				KSetLightPosition(numLights, l->positionLocal);
			    KSetLightAttenuation(numLights,(l->flags & GL_ATTENUATION),l->dvAttenuation0,l->dvAttenuation1,l->dvAttenuation2);
				KSetLightSpotCone(numLights,l->cosPhi,l->cosTheta);
				KSetLightRadius(numLights,l->rangeLocal);
				break;
			}
			}
#endif
			numLights++;
			} // e->enabled 


			l = l->next;

		}
		state.lightsTransformed = gtrue;
		state.numLights = numLights;
		state.materialChanged = gtrue; // already set
		if (state.fogEnabled) { // fog 
#ifdef _KATMAI
			if (useKatmai) {
				KSetLightType(numLights,KFOG_LINEAR);
				float s=state.fog.start;
				float e=state.fog.end;
				s=s*state.projection.x[2][2] + state.projection.x[3][2];
				e=e*state.projection.x[2][2] + state.projection.x[3][2];
			
				TRACE("Katmai Fog %g %g == > %g %g \n",state.fog.start,state.fog.end,s,e);
				KSetFogLinear(state.fog.color,s,e);
				KSetNumLights(numLights+1);
			}
#endif
		}

		} // updateLights

		if (state.materialChanged) { 	// only if material / lights changing

			state.materialChanged=gfalse;
			int numLights=0;
			G3LightHandle *l=state.lights;



		// setup light * material values
		while ( l != NULL ) {	// for all lights 

			l->kAmbient.r =  mat.ambient.r * l->ambientColor.r,	 l->kAmbient.g = mat.ambient.g * l->ambientColor.g, l->kAmbient.b = mat.ambient.b * l->ambientColor.b;

			l->kDiffuse.r =  mat.diffuse.r * l->dcvColor.r,
			l->kDiffuse.g =  mat.diffuse.g * l->dcvColor.g,
			l->kDiffuse.b =  mat.diffuse.b * l->dcvColor.b;

			l->kSpecular.r =  mat.specular.r * l->dcvColor.r,
			l->kSpecular.g =  mat.specular.g * l->dcvColor.g,
			l->kSpecular.b =  mat.specular.b * l->dcvColor.b;

			if (l->enabled) {
				// if (numLights == 0) state.firstLight = l;

#ifdef _KATMAI
/*
				// KSetNumLights(numLights+1);
				if (l->kSpecular.r >0.0f) {
					TRACE("Specular light %g  \n",l->kSpecular.r); 
				}
				if (l->kAmbient.r >0.2f) {
					TRACE("Ambient light %g  \n",l->kAmbient.r); 
				}
*/
				if (useKatmai) {
				KSetLightColors(numLights, l->kAmbient,l->kDiffuse,l->kSpecular,doSpecular);
				}
#endif
				numLights++;
			}

			l = l->next;

		}
		
		state.numLights = numLights;

#ifdef _KATMAI
		if (useKatmai) {
			KSetBasicMaterial(mat.emissive,mat.diffuse.a,mat.power);
		}

#endif
		} // material changed 



	}
	// only if viewer changes
#ifdef _KATMAI
	if (useKatmai) {
		KSetViewerPosition(viewpoint); // for specular lighting, two sided lighting 
		// new also for backface culling 
	}
#endif


#ifdef _KATMAI
	if (useKatmai && doVn && !doVc /* && (nv>6)*/ ) { 


		if ((state.numLights == 1) && (state.firstLight->dltType == D3DLIGHT_DIRECTIONAL)) {
			if (singlsidelighting) {

				if (doSpecular) {
					KatmaiTransformLight_1_dir_spec(nv,kv.Data(), kvn.Data(),M.x, w,cx,h,cy,  vtlp); 
					goto drawIt;
				}
				KatmaiTransformLight_1_dir(nv,kv. Data(),kvn.Data(), M.x, w,cx,h,cy,   vtlp); 
				goto drawIt;
			}
				if (doSpecular) {
					KatmaiTransformLight_1_dir_spec_twosided(nv,kv.Data(), kvn.Data(),M.x, w,cx,h,cy,  vtlp); 
					goto drawIt;
				}
				KatmaiTransformLight_1_dir_twosided(nv,kv. Data(),kvn.Data(), M.x, w,cx,h,cy,   vtlp); 
				goto drawIt;


		} 
		
		// generic lighting 
		if (!singlsidelighting) {
			KatmaiTransformLight_generic_spec_twosided(nv,kv.Data(),kvn.Data(), M.x, w,cx,h,cy,   vtlp); 
			goto drawIt;
		}

		KatmaiTransformLight_generic_spec(nv,kv. Data(),kvn.Data(), M.x, w,cx,h,cy,   vtlp); 
		goto drawIt;

	}

#endif




#ifdef _KATMAI
	if (useKatmai /* && (nv>6)*/ ) {
		if (kv.Length() ==0 || (actions & SHELL_UPDATE_EXBUF_VERT) ) {
			kv.Set(v); // copy from AOS to SOA 
		}
		KatmaiTransformScreen_Int(nv,kv.Data(), M.x, w,cx,h,cy,   vtlp); // SOA 
	} else {
	// don't use Katmai
	for(i=0; i < nv; i++) {

		// vertex 
		const Point &p = v[i];

		float vt[4];

		float tmp = p.x * M.x[0][3] + p.y * M.x[1][3] + p.z * M.x[2][3] + M.x[3][3];
		tmp = 1.0f / tmp;

		vtlp[i].rhw =  tmp; // needed for texture perspective correction
	
		vt[0] = tmp * (p.x * M.x[0][0] + p.y * M.x[1][0] + p.z * M.x[2][0] + M.x[3][0]);
		vt[1] = tmp * (p.x * M.x[0][1] + p.y * M.x[1][1] + p.z * M.x[2][1] + M.x[3][1]);
		vt[2] = tmp * (p.x * M.x[0][2] + p.y * M.x[1][2] + p.z * M.x[2][2] + M.x[3][2]);
		vtlp[i].sx = cx+vt[0]*w;
		vtlp[i].sy = cy-vt[1]*h;
		vtlp[i].sz = vt[2];

	}
	}


	if (!doVn) { 
		goto drawIt;  // color & texcoords are ok in vertex buffer
	}


#endif

	 float zmin,zmax;
	 float wmin,wmax;
	 zmin = 1E37;
	 zmax = -1E37;
	 wmin = 1E37;
	 wmax = -1E37;

	for(i=0; i < nv; i++) {

		// vertex 
		const Point &p = v[i];

#ifndef _KATMAI
		float vt[4];
		vt[0] = p.x * M.x[0][0] + p.y * M.x[1][0] + p.z * M.x[2][0] + M.x[3][0];
		vt[1] = p.x * M.x[0][1] + p.y * M.x[1][1] + p.z * M.x[2][1] + M.x[3][1];
		vt[2] = p.x * M.x[0][2] + p.y * M.x[1][2] + p.z * M.x[2][2] + M.x[3][2];
		vt[3] = p.x * M.x[0][3] + p.y * M.x[1][3] + p.z * M.x[2][3] + M.x[3][3];
		
		float tmp = 1.0f / vt[3];
		vt[0]  *=tmp; 
		vt[1]  *=tmp;
		vt[2]  *=tmp;

 // http://town.hi-ho.ne.jp/cgtech/Contents/Shrine/wshrine3.wrl

		vtlp->sx = cx+vt[0]*w;
		vtlp->sy = cy-vt[1]*h;
		vtlp->sz = vt[2];
/*	for debug :
		zmin = min(vt[2],zmin);
		zmax = max(vt[2],zmax);
		wmin = min(tmp,wmin);
		wmax = max(tmp,wmax);
*/
		vtlp->rhw =  tmp; // needed for texture perspective correction
#endif


		// vertex normal 
		if (doVn) { // light vertices
			// normal 
			//Point n= RotateOnly(worldInv,vn[i]);
			//n.Normalize();

			Point n= vn[i];
			if (flipNormals) { n = -n; }  // FLIP

			G3LightHandle *l=state.lights;	// first light pointer

			// emissive 
			
			RGBAColor c(start) ; // resulting color

			RGBAColor cspec(0,0,0,start.a); 

			// get material diffuse color, 3 floats
			const float* diffuse;

			if (doVc) diffuse = &vc[i].x; // we have vertex colors 
			else diffuse = &mat.diffuse.r;

			Point toViewer; // points from point to viewer 

			doVertexCull = TRUE;
			// if (doSpecular) {
				toViewer= viewpoint - p; // vector to viewer in local space
				toViewer.Normalize();
			//}

			if (!singlsidelighting && (DotProd(toViewer,n) < 0.0f)) {
					n= - n; // flip normal for two sided lighting
			}

			// Light : http://www.vrml.org/Specifications/VRML97/part1/concepts.html			
			// if (DotProd(toViewer,n) >= 0)  // Idea : can cull lighting, if pointing away from viewer
			{ 

			// vcull[i]=1; // reuse info for vertex culling

			while ( l != NULL ) {	// for all lights 
			// diffuse part 	
			RGBAColor c1;
			float f;


			if (l->enabled)

			switch(l->dltType) { 					
			case  D3DLIGHT_DIRECTIONAL : 
				{


				f =	n.x * l->directionLocal.x + n.y * l->directionLocal.y + n.z * l->directionLocal.z ;

				f= -f;

				if(f>0.0f)  {
					c1.r=diffuse[0] * f, c1.g=diffuse[1] * f, c1.b=diffuse[2] * f;
				}
				else c1.r=c1.g=c1.b = 0;



				// specular part 
				if (doSpecular) {
					Point tmp(-l->directionLocal.x + toViewer.x,
							-l->directionLocal.y + toViewer.y,
							-l->directionLocal.z + toViewer.z);
					tmp.Normalize();

					f = n.x * tmp.x + n.y * tmp.y + n.z * tmp.z; // dotprod

					if (f > 0.0f)  {

						f = SPOW ( f , mat.power);
				
						if(f<0.0f) 	
							f=0.0f;	else 
						if (f>1.0) {
							f=1.0f;
						}
					/*
						_asm{
						mov eax,f
						mov ebx,eax
						and eax,0x80000000
						jnz noneg
						mov ebx,0
				noneg :	and ebx,0x3FFFFFFF
						mov f,ebx

						}
*/
						cspec.r += l->kSpecular.r*f,cspec.g += l->kSpecular.g*f,cspec.b += l->kSpecular.b*f;

					}

				}

				// multiply by light color + intensity 
				c.r+=c1.r * l->dcvColor.r,c.g+=c1.g * l->dcvColor.g,c.b+=c1.b * l->dcvColor.b;
				if (l->flags & GL_AMBIENT) { // ambient (Extra to D3D, req. by VRML)
					c.r +=  l->kAmbient.r,	 c.g += l->kAmbient.g, c.b += l->kAmbient.b;
				}

			} 
			break; // case  D3DLIGHT_DIRECTIONAL :

			case  D3DLIGHT_POINT : 
			case  D3DLIGHT_SPOT : 
			case  D3DLIGHT_PARALLELPOINT : // actually not used
			{

				Point toLight(l->positionLocal.x - p.x,
							l->positionLocal.y - p.y,
							l->positionLocal.z - p.z);
					
				float dl=toLight.Length();
				
				if ((l->flags & GL_RANGE) && (dl >l->dvRange)) { // to do: range in  local  coordinates
					break;
				}

				toLight *= 1.0f /dl; // Normalize();

				float scale=1.0f; // attenuation and spotlight scale 

				if ((l->flags & GL_ATTENUATION)) {
				
					// attenuation
					scale = l->dvAttenuation0+dl*(l->dvAttenuation1 + l->dvAttenuation2*dl);

					if (scale>=1.0f) scale = 1.0f;
					else if (scale == 0.0f) break;
					else scale = 1.0/f;
				}



				// spot here
				if (l->dltType == D3DLIGHT_SPOT) { 
					float tmp =	-1.0f * (toLight.x * l->directionLocal.x + toLight.y * l->directionLocal.y + toLight.z * l->directionLocal.z);

#if 1
					float spota = tmp;	
					if ((spota<=0.0f) || (spota < l->cosPhi)) {
						scale=0.0f; // outside
						break;
					} else
					if (spota >= l->cosTheta) {
						// scale*=1.0f;
					}
					else  // check this 
						scale *= ((spota - l->cosPhi ) / (l->cosTheta - l->cosPhi)); // * l->thetaScale

#else
					if (tmp <=0.0f) tmp = 0.0f;
					float spota = acos(tmp);	
					if (spota >= l->dvPhi)
						scale =0.0f;
					else
					if (spota <= l->dvTheta) {
						// f*=1.0f;
					}
					else 
						scale *= ((spota - l->dvPhi ) / (l->dvTheta - l->dvPhi));
#endif

				}

				if (scale >0.0f) {

				// diffuse 

				f =	n.x * toLight.x + n.y * toLight.y + n.z * toLight.z ;

				if(f>0.0f)  {
					f*=scale;
					c1.r=diffuse[0] * f, c1.g=diffuse[1] * f, c1.b=diffuse[2] * f;
				}
				else c1.r=c1.g=c1.b = 0;


				// specular part 
				if (doSpecular) {
					Point tmp(toLight.x + toViewer.x,
							toLight.y + toViewer.y,
							toLight.z + toViewer.z);
					tmp.Normalize();

					f = n.x * tmp.x + n.y * tmp.y + n.z * tmp.z; // dotprod

					if (f > 0.0f)  {

						f = SPOW ( f , mat.power);
				
						if(f<0.0f) 
							f=0.0f;
						else if (f>1.0) {
							f=1.0f;
						}
						f *= scale;
						cspec.r += l->kSpecular.r*f,cspec.g += l->kSpecular.g*f,cspec.b += l->kSpecular.b*f;

					}

				}


				// multiply by light color + intensity 
				c.r+=c1.r * l->dcvColor.r,c.g+=c1.g * l->dcvColor.g,c.b+=c1.b * l->dcvColor.b;

				if (l->flags & GL_AMBIENT) { // ambient (Extra to D3D, req. by VRML)
					c.r +=  scale * l->kAmbient.r,	 c.g += scale * l->kAmbient.g, c.b += scale * l->kAmbient.b;
				}
				
				} // scale > 0

				break;
			}

			} // switch(l->dltType)


			l = l->next;
			}
			
			}
			// clamp or scale by max 
			if (c.r>1.0f) c.r=1.0f;
			if (c.g>1.0f) c.g=1.0f;
			if (c.b>1.0f) c.b=1.0f;

			// fog
			/* to do: Vertex Fog 
				float dV = dist to viewer

				f = 0.0;
				// lin fog	
				if (dV < fogVisibility)
					f= (fogVisibility-dV) / fogVisibility
				
				 // exp fog
				 f = exp(-dV / (fogVisibility-dV ) )
				
                c*=f;
				c += fogCOlor * (1.0-f);

			*/

			vtlp->color = D3DRGBA_X(c.r,c.g,c.b,c.a); // this involves a * 255 mul ==> Katami
			// to do: use Katmai pack to DWORD 

			if (doSpecular) {
				// clamp or scale by max 
				if (cspec.r>1.0f) cspec.r=1.0f;
				if (cspec.g>1.0f) cspec.g=1.0f;
				if (cspec.b>1.0f) cspec.b=1.0f;

				vtlp->specular = D3DRGBA_X(cspec.r,cspec.g,cspec.b,cspec.a);	// to do: use Katmai pack
						// or use D3DRGBA_CLAMP
			} else vtlp->specular = 0;

		}
		else { // unlit 
			if (doVc) {
				const Point &c = vc[i]; // we have vertex colors 
				vtlp->color = D3DRGBA_X(c.x,c.y,c.z,mat.diffuse.a); // costly 
			}
			else vtlp->color = state.currentColorD3D; 
			vtlp->specular = 0; // vtlp->color; 
		}
		
#if 0
		// already setup in buffer 
		// texture coordinates 
		if (doVp) {
			const Point &p =vp[i];
			
			if (!state.textureMatrixIdentity) {
				Matrix &M = state.textureMatrix;
				vtlp->tu = p.x * M.x[0][0] + p.y * M.x[1][0] /* + p.z * M.x[2][0] */ + M.x[3][0];
				vtlp->tv=  p.x * M.x[0][1] + p.y * M.x[1][1] /* + p.z * M.x[2][1] */+ M.x[3][1];

			} 
			else 
			{  
				vtlp->tu = p.x;
				vtlp->tv = p.y;
			}

		} else {
			vtlp->tu = 0;
			vtlp->tv = 0;
		}

#endif

		vtlp++;

	}

//	TRACE("zmin %g zmax %g  wmin %g wmax %g \n",zmin,zmax,wmin,wmax);

drawIt : return;


}


//===========================================================================================
//					int GShell::D3dRenderTL(GglRenderTraversal &traversalState)
//===========================================================================================

//Render using ownLighting/Transform 
int GShell::D3dRenderTL(GglRenderTraversal &traversalState)
{
	RenderState &state = *traversalState.glState;
	gbool culling = state.backfaceCulling; // backface culling enabled ?
	gbool singlsidelighting = culling;

	DWORD primMask;
	if (traversalState.doClipping || 1 )
		primMask =  D3DDP_DONOTUPDATEEXTENTS; // | D3DDP_DONOTLIGHT 
	else  
		primMask = D3DDP_DONOTCLIP |  D3DDP_DONOTUPDATEEXTENTS;


//	TRACE("Draw \n");
    //state.SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE); // turn off culling 
	// state.SetRenderState(D3DRENDERSTATE_TEXTUREPERSPECTIVE,TRUE); 
//	state.backfaceCulling = 0;

	if (state.enableD3DCulling) { // hw culling
		state.SetCullMode(culling  ? (state.frontFaceCW ? D3DCULL_CCW: D3DCULL_CW) : D3DCULL_NONE);
		culling = 0;
	} else state.SetCullMode(D3DCULL_NONE);


	int nv= v.Length();

/*
	if (nv >= MAXWORD) {
		TRACE("D3dRenderTL too many vertices %d \n",nv);
		return(-1);
	}
*/
	if (nv == 0 || (f.Length()<4)) 
		return -1;

	// TLVERTEX
	// Array<D3DTLVERTEX> &vtl= this->vtl; // state.vtlBuffer; // use global buffer
	FloatArrayAligned &vtl= this->vtl;

	// DTLVERTEX = 8 floats x y z w color spec tu tv 
	vtl.SetLength( (sizeof(D3DTLVERTEX) / sizeof(float)) * ((nv+3) & ~3)); // katmai uses at least multiple of 4 
	D3DTLVERTEX *vtlp=   ( D3DTLVERTEX*) vtl.Data();

	// Transform & Light
	int i;

	// matrix 
	GMatrixStack *mstack = traversalState.GetCurrentMatrix();

    if (!mstack->viewpointLocalOk) { // need to transform it
		mstack->SetViewpointLocal(traversalState.physicalCamera->position);
	}  
	const Point &viewpoint = mstack->viewpointLocal;

	// backface culling check for planar things 
	if (culling && ((flags & SHELL_PLANAR) !=0)) {
		// Culltest 
		const PointArray &n=GetFN();
		Point toViewer; // points from point to viewer 

		toViewer= viewpoint - v[0]; // vector to viewer in local space
		toViewer.Normalize();

		float tmp=DotProd(toViewer,n[0]); 
		if (state.frontFaceCW) tmp = -tmp;

		if ( tmp <= 0.0f) // can cull 
			return -1;


	}


	// Matrix M = mstack->GetMatrix()* state.view *state.projection;
	
	if (!mstack->model2screenOk) {
		// mstack->model2screen = mstack->GetMatrix()* state.view *state.projection;
		Multiply(mstack->GetMatrix(),state.model2screen,mstack->model2screen);

		mstack->model2screenOk = 1;
		// state.lightsTransformed = 0; Done by SetMatrix(m)

		// KatmaiSetMatrix(mstack->model2screen.m)
	}
	Matrix& M =	mstack->model2screen; // transforming model to screen 

	gbool updateLights= !state.lightsTransformed; // need to transform lights on each matrix change

	float cx,w;// = state.device->szClient.cx*0.5f;
	float cy,h;// = state.device->szClient.cy*0.5f;
	RECT & r=state.device->rcViewportRect;

	w=(r.right - r.left)*0.5;
	h=(r.bottom - r.top)*0.5;

	cy= r.bottom - h;
	cx= r.left + w;

	gbool doVp = state.textureDefined && vp.Length()>0;
	gbool doVn =  ! state.Unlit();

	if (!doVn) {
		if (!this->isUnlit || this->currentColorD3D != state.currentColorD3D) // need update colors in vertex buffer 
			actions |= SHELL_UPDATE_EXBUF_VERT;
	}	


	if (
#ifdef _KATMAI		
		kv.Length() ==0 || 
#endif
		(actions & SHELL_UPDATE_EXBUF_VERT) 
		) {

#ifdef _KATMAI
		kv.Set(v); // copy from AOS to SOA 
#endif


		// recreate texture coordinates	
		textureMatrixVersion = state.GetTextureMatrixVersion();

		if (vp.Length()>0) {
			D3DTLVERTEX *vtlp= ( D3DTLVERTEX*)  vtl.Data();
			
			if (!state.textureMatrixIdentity) {
				Matrix &M = state.textureMatrix;
				for(i = 0;  i < nv; i++) {
					const Point &p =vp[i];
					vtlp->tu = p.x * M.x[0][0] + p.y * M.x[1][0] /* + p.z * M.x[2][0] */ + M.x[3][0];
					vtlp->tv=  p.x * M.x[0][1] + p.y * M.x[1][1] /* + p.z * M.x[2][1] */+ M.x[3][1];
					vtlp ++;
				}
			} 
			else 
			{  
				for(i = 0;  i < nv; i++) {
					const Point &p =vp[i];
					vtlp->tu = p.x;
					vtlp->tv = p.y;
					vtlp ++;
				}
			}

		} else {
			//vtlp->tu = 0;
			//vtlp->tv = 0;
		}
		
		if (!doVn) { // no lighting
			
			D3DTLVERTEX *vtlp= ( D3DTLVERTEX*) vtl.Data();

			float a=state.alpha;

			if (vc.Length()>0) { 
				for(i = 0;  i < nv; i++,vtlp++) {
					const Point &c = vc[i]; // we have vertex colors 
					vtlp->color = D3DRGBA_X(c.x,c.y,c.z,a);
					vtlp->specular = 0; // vtlp->color; 
				}
			} else  {
				D3DCOLOR c= state.currentColorD3D;
				for(i = 0;  i < nv; i++,vtlp++) {
					vtlp->color = c; 
					vtlp->specular = 0; 
				}
			}
			this->currentColorD3D = state.currentColorD3D;
			this->isUnlit = gtrue;
		}
	    //if (vp.Length()>0)   kvp.Set(vp);
	    //if (vc.Length()>0)   kvc.Set(vc);

	}


    gbool flipNormals =  state.frontFaceCW; 

	if (doVn) 
	{
		const PointArray &vntmp = GetVN(); // get/update vertex normals 
	
		if (vntmp.Length() == 0) doVn =0; 
		// update Vertex Normals in Katmai form
		if (doVn && (kvn.Length() == 0) || (actions & SHELL_UPDATE_EXBUF_VERT)) {
		
			if (flipNormals) {
				kvn.Set(vn,-1.0f);

				//int l = kvn.Length()*3;
				//for (int j=0;j<l;j++) kvn.p[j]*= -1.0f;
			} else kvn.Set(vn);

		} 
	}

	gbool doVc = vc.Length()>0; // we have color per vertex, replace diffuseColor
	
	G3Material &mat = state.currentMaterial->material;
	RGBAColor start(mat.emissive); // start with emissive color

	// do we need specular lighting ?
	gbool doSpecular = (mat.specular.r>0) || (mat.specular.g>0) || (mat.specular.b>0);

	gbool doVertexCull = gfalse;

	// IntArray vcull(nv,0);

	// cache lighting values 
	if (doVn ) {

		this->isUnlit = gfalse;

		// transform lights from world to local 
		if (updateLights) {
		G3LightHandle *l=state.lights;

		int numLights=0;

		Matrix& worldInv = mstack->GetMatrixInverse();

		while ( l != NULL ) {	// for all lights 
			
			l->directionLocal = RotateOnly(worldInv, *((Point *)&l->dvDirection));
			l->directionLocal.Normalize();
			l->positionLocal = worldInv * (*((Point *)&l->dvPosition));
			// to do : range , attenuation 

			if (l->enabled) {

				if (numLights == 0) state.firstLight = l;

#ifdef _KATMAI
			if (useKatmai) {
			KSetNumLights(numLights+1);
			switch(l->dltType) { 					
			case  D3DLIGHT_DIRECTIONAL : 
			    KSetLightType(numLights,KDIFFUSE_LIGHT);

				KSetLightDirection(numLights, -l->directionLocal);
				break;

			case  D3DLIGHT_POINT : 
			case  D3DLIGHT_PARALLELPOINT : // actually not used
			    KSetLightType(numLights,KPOINT_LIGHT);
				KSetLightPosition(numLights, l->positionLocal);
			    KSetLightAttenuation(numLights,(l->flags & GL_ATTENUATION),l->dvAttenuation0,l->dvAttenuation1,l->dvAttenuation2);

				break;

			case  D3DLIGHT_SPOT : 
			    KSetLightType(numLights,KSPOT_LIGHT);
				KSetLightDirection(numLights, - l->directionLocal);
				KSetLightPosition(numLights, l->positionLocal);
			    KSetLightAttenuation(numLights,(l->flags & GL_ATTENUATION),l->dvAttenuation0,l->dvAttenuation1,l->dvAttenuation2);
				KSetLightSpotCone(numLights,l->cosPhi,l->cosTheta);
				KSetLightRadius(numLights,l->rangeLocal);
				break;
			}
			}
#endif
			numLights++;
			} // e->enabled 


			l = l->next;

		}
		state.lightsTransformed = gtrue;
		state.numLights = numLights;
		state.materialChanged = gtrue; // already set

		if (state.fogEnabled) { // fog 
#ifdef _KATMAI
			if (useKatmai) {
				KSetLightType(numLights,KFOG_LINEAR);
				float s=state.fog.start;
				float e=state.fog.end;
				s=s*state.projection.x[2][2] + state.projection.x[3][2];
				e=e*state.projection.x[2][2] + state.projection.x[3][2];
			
				TRACE("Katmai Fog %g %g == > %g %g \n",state.fog.start,state.fog.end,s,e);
				KSetFogLinear(state.fog.color,s,e);
				KSetNumLights(numLights+1);
			}
#endif
		}


		} // updateLights

		if (state.materialChanged) { 	// only if material / lights changing

			state.materialChanged=gfalse;
			int numLights=0;
			G3LightHandle *l=state.lights;



		// setup light * material values
		while ( l != NULL ) {	// for all lights 

			l->kAmbient.r =  mat.ambient.r * l->ambientColor.r,	 l->kAmbient.g = mat.ambient.g * l->ambientColor.g, l->kAmbient.b = mat.ambient.b * l->ambientColor.b;

			l->kDiffuse.r =  mat.diffuse.r * l->dcvColor.r,
			l->kDiffuse.g =  mat.diffuse.g * l->dcvColor.g,
			l->kDiffuse.b =  mat.diffuse.b * l->dcvColor.b;

			l->kSpecular.r =  mat.specular.r * l->dcvColor.r,
			l->kSpecular.g =  mat.specular.g * l->dcvColor.g,
			l->kSpecular.b =  mat.specular.b * l->dcvColor.b;

			if (l->enabled) {
				// if (numLights == 0) state.firstLight = l;

#ifdef _KATMAI
				if (useKatmai) {
					KSetLightColors(numLights, l->kAmbient,l->kDiffuse,l->kSpecular,doSpecular);
				}
#endif
				numLights++;
			}

			l = l->next;

		}
		
		state.numLights = numLights;

#ifdef _KATMAI
		if (useKatmai) {
			KSetBasicMaterial(mat.emissive,mat.diffuse.a,mat.power);
		}	
#endif
		} // material changed 



	}
	// only if viewer changes
#ifdef _KATMAI
	if (useKatmai) {
		KSetViewerPosition(viewpoint); // for specular lighting, two sided lighting 
	}
	// new also for backface culling 
#endif


#ifdef _KATMAI
	if (useKatmai && doVn && !doVc /* && (nv>6)*/ ) { 


		if ((state.numLights == 1) && (state.firstLight->dltType == D3DLIGHT_DIRECTIONAL)) {
			if (singlsidelighting) {

				if (doSpecular) {
					KatmaiTransformLight_1_dir_spec(nv,kv.Data(), kvn.Data(),M.x, w,cx,h,cy,  vtlp); 
					goto drawIt;
				}
				KatmaiTransformLight_1_dir(nv,kv. Data(),kvn.Data(), M.x, w,cx,h,cy,   vtlp); 
				goto drawIt;
			}
				if (doSpecular) {
					KatmaiTransformLight_1_dir_spec_twosided(nv,kv.Data(), kvn.Data(),M.x, w,cx,h,cy, vtlp); 
					goto drawIt;
				}
				KatmaiTransformLight_1_dir_twosided(nv,kv. Data(),kvn.Data(), M.x, w,cx,h,cy,  vtlp); 
				goto drawIt;


		} 
		
		// generic lighting 
		if (!singlsidelighting) {
			KatmaiTransformLight_generic_spec_twosided(nv,kv.Data(),kvn.Data(), M.x, w,cx,h,cy,   vtlp); 
			goto drawIt;
		}

		KatmaiTransformLight_generic_spec(nv,kv. Data(),kvn.Data(), M.x, w,cx,h,cy,  vtlp); 
		goto drawIt;

	}

#endif




#ifdef _KATMAI
	if (useKatmai /* && (nv>6)*/ ) {
		if (kv.Length() ==0 || (actions & SHELL_UPDATE_EXBUF_VERT) ) {
			kv.Set(v); // copy from AOS to SOA 
		}
		KatmaiTransformScreen_Int(nv,kv.Data(), M.x,  w,cx,h,cy,  vtlp); // SOA 
	} else {
	// don't use Katmai
	for(i=0; i < nv; i++) {

		// vertex 
		const Point &p = v[i];

		float vt[4];

		float tmp = p.x * M.x[0][3] + p.y * M.x[1][3] + p.z * M.x[2][3] + M.x[3][3];
		tmp = 1.0f / tmp;

		vtlp[i].rhw =  tmp; // needed for texture perspective correction
	
		vt[0] = tmp * (p.x * M.x[0][0] + p.y * M.x[1][0] + p.z * M.x[2][0] + M.x[3][0]);
		vt[1] = tmp * (p.x * M.x[0][1] + p.y * M.x[1][1] + p.z * M.x[2][1] + M.x[3][1]);
		vt[2] = tmp * (p.x * M.x[0][2] + p.y * M.x[1][2] + p.z * M.x[2][2] + M.x[3][2]);
		vtlp[i].sx = cx+vt[0]*w;
		vtlp[i].sy = cy-vt[1]*h;
		vtlp[i].sz = vt[2];

	}
	}


	if (!doVn) { 
		goto drawIt;  // color & texcoords are ok in vertex buffer
	}


#endif

	 float zmin,zmax;
	 float wmin,wmax;
	 zmin = 1E37;
	 zmax = -1E37;
	 wmin = 1E37;
	 wmax = -1E37;

	for(i=0; i < nv; i++) {

		// vertex 
		const Point &p = v[i];

#ifndef _KATMAI
		float vt[4];
		vt[0] = p.x * M.x[0][0] + p.y * M.x[1][0] + p.z * M.x[2][0] + M.x[3][0];
		vt[1] = p.x * M.x[0][1] + p.y * M.x[1][1] + p.z * M.x[2][1] + M.x[3][1];
		vt[2] = p.x * M.x[0][2] + p.y * M.x[1][2] + p.z * M.x[2][2] + M.x[3][2];
		vt[3] = p.x * M.x[0][3] + p.y * M.x[1][3] + p.z * M.x[2][3] + M.x[3][3];
		
		float tmp = 1.0f / vt[3];
		vt[0]  *=tmp; 
		vt[1]  *=tmp;
		vt[2]  *=tmp;

 // http://town.hi-ho.ne.jp/cgtech/Contents/Shrine/wshrine3.wrl

		vtlp->sx = cx+vt[0]*w;
		vtlp->sy = cy-vt[1]*h;
		
		//if (vt[2] <0) vt[2] = 0.0f;
		//else if (vt[2] >1) vt[2] = 1.0f;
		vtlp->sz = vt[2];
/*	for debug :
		zmin = min(vt[2],zmin);
		zmax = max(vt[2],zmax);
		wmin = min(tmp,wmin);
		wmax = max(tmp,wmax);
*/
		vtlp->rhw =  tmp; // needed for texture perspective correction
#endif


		// vertex normal 
		if (doVn) { // light vertices
			// normal 
			//Point n= RotateOnly(worldInv,vn[i]);
			//n.Normalize();

			Point n= vn[i];
			if (flipNormals) { n = -n; }  // FLIP

			G3LightHandle *l=state.lights;	// first light pointer

			// emissive 
			
			RGBAColor c(start) ; // resulting color

			RGBAColor cspec(0,0,0,0); // start.a); 

			// get material diffuse color, 3 floats
			const float* diffuse;

			if (doVc) diffuse = &vc[i].x; // we have vertex colors 
			else diffuse = &mat.diffuse.r;

			Point toViewer; // points from point to viewer 

			doVertexCull = TRUE;
			// if (doSpecular) {
				toViewer= viewpoint - p; // vector to viewer in local space
				toViewer.Normalize();
			//}

			if (!singlsidelighting && (DotProd(toViewer,n) < 0.0f)) {
					n= - n; // flip normal for two sided lighting
			}

			// Light : http://www.vrml.org/Specifications/VRML97/part1/concepts.html			
			// if (DotProd(toViewer,n) >= 0)  // Idea : can cull lighting, if pointing away from viewer
			{ 

			// vcull[i]=1; // reuse info for vertex culling

			while ( l != NULL ) {	// for all lights 
			// diffuse part 	
			RGBAColor c1;
			float f;


			if (l->enabled)

			switch(l->dltType) { 					
			case  D3DLIGHT_DIRECTIONAL : 
				{


				f =	n.x * l->directionLocal.x + n.y * l->directionLocal.y + n.z * l->directionLocal.z ;

				f= -f;

				if(f>0.0f)  { // add diffuse part
					// use direct material color, because of VC support 
					c1.r=diffuse[0] * f, c1.g=diffuse[1] * f, c1.b=diffuse[2] * f;
				}
				else c1.r=c1.g=c1.b = 0;



				// specular part 
				if (doSpecular) {
					Point tmp(-l->directionLocal.x + toViewer.x,
							-l->directionLocal.y + toViewer.y,
							-l->directionLocal.z + toViewer.z);
					tmp.Normalize();

					f = n.x * tmp.x + n.y * tmp.y + n.z * tmp.z; // dotprod

					if (f > 0.0f)  {  // there is a specular contribution

						f = SPOW ( f , mat.power);
				
						// if(f<0.0f) 	f=0.0f;	else 
						if (f>1.0) {
							f=1.0f;
						}
					/*
						_asm{
						mov eax,f
						mov ebx,eax
						and eax,0x80000000
						jnz noneg
						mov ebx,0
				noneg :	and ebx,0x3FFFFFFF
						mov f,ebx

						}
*/
						cspec.r += l->kSpecular.r*f,cspec.g += l->kSpecular.g*f,cspec.b += l->kSpecular.b*f;

					}

				}

				// multiply by light color + intensity 
				c.r+=c1.r * l->dcvColor.r,c.g+=c1.g * l->dcvColor.g,c.b+=c1.b * l->dcvColor.b;
				if (l->flags & GL_AMBIENT) { // ambient (Extra to D3D, req. by VRML)
					c.r +=  l->kAmbient.r,	 c.g += l->kAmbient.g, c.b += l->kAmbient.b;
				}

			} 
			break; // case  D3DLIGHT_DIRECTIONAL :

			case  D3DLIGHT_POINT : 
			case  D3DLIGHT_SPOT : 
			case  D3DLIGHT_PARALLELPOINT : // actually not used
			{

				Point toLight(l->positionLocal.x - p.x,
							l->positionLocal.y - p.y,
							l->positionLocal.z - p.z);
					
				float dl=toLight.Length();
				
				if ((l->flags & GL_RANGE) && (dl >l->dvRange)) { // to do: range in  local  coordinates
					break;
				}

				toLight *= 1.0f /dl; // Normalize();

				float scale=1.0f; // attenuation and spotlight scale 

				if ((l->flags & GL_ATTENUATION)) {
				
					// attenuation
					scale = l->dvAttenuation0+dl*(l->dvAttenuation1 + l->dvAttenuation2*dl);

					if (scale>=1.0f) scale = 1.0f;
					else if (scale == 0.0f) break;
					else scale = 1.0/f;
				}



				// spot here
				if (l->dltType == D3DLIGHT_SPOT) { 
					float tmp =	-1.0f * (toLight.x * l->directionLocal.x + toLight.y * l->directionLocal.y + toLight.z * l->directionLocal.z);

#if 1
					float spota = tmp;	
					if ((spota<=0.0f) || (spota < l->cosPhi)) {
						scale=0.0f; // outside
						break;
					} else
					if (spota >= l->cosTheta) {
						// scale*=1.0f;
					}
					else  // check this 
						scale *= ((spota - l->cosPhi ) / (l->cosTheta - l->cosPhi)); // * l->thetaScale

#else
					if (tmp <=0.0f) tmp = 0.0f;
					float spota = acos(tmp);	
					if (spota >= l->dvPhi)
						scale =0.0f;
					else
					if (spota <= l->dvTheta) {
						// f*=1.0f;
					}
					else 
						scale *= ((spota - l->dvPhi ) / (l->dvTheta - l->dvPhi));
#endif

				}

				if (scale >0.0f) {

				// diffuse 

				f =	n.x * toLight.x + n.y * toLight.y + n.z * toLight.z ;

				if(f>0.0f)  {
					f*=scale;
					c1.r=diffuse[0] * f, c1.g=diffuse[1] * f, c1.b=diffuse[2] * f;
				}
				else c1.r=c1.g=c1.b = 0;


				// specular part 
				if (doSpecular) {
					Point tmp(toLight.x + toViewer.x,
							toLight.y + toViewer.y,
							toLight.z + toViewer.z);
					tmp.Normalize();

					f = n.x * tmp.x + n.y * tmp.y + n.z * tmp.z; // dotprod

					if (f > 0.0f)  {

						f = SPOW ( f , mat.power);
				
						if(f<0.0f) 
							f=0.0f;
						else if (f>1.0) {
							f=1.0f;
						}
						f *= scale;
						cspec.r += l->kSpecular.r*f,cspec.g += l->kSpecular.g*f,cspec.b += l->kSpecular.b*f;

					}

				}


				// multiply by light color + intensity 
				c.r+=c1.r * l->dcvColor.r,c.g+=c1.g * l->dcvColor.g,c.b+=c1.b * l->dcvColor.b;

				if (l->flags & GL_AMBIENT) { // ambient (Extra to D3D, req. by VRML)
					c.r +=  scale * l->kAmbient.r,	 c.g += scale * l->kAmbient.g, c.b += scale * l->kAmbient.b;
				}
				
				} // scale > 0

				break;
			}

			} // switch(l->dltType)


			l = l->next;
			}
			
			}
			// clamp or scale by max 
			if (c.r>1.0f) c.r=1.0f;
			if (c.g>1.0f) c.g=1.0f;
			if (c.b>1.0f) c.b=1.0f;

			// fog
			/* to do: Vertex Fog 
				float dV = dist to viewer

				f = 0.0;
				// lin fog	
				if (dV < fogVisibility)
					f= (fogVisibility-dV) / fogVisibility
				
				 // exp fog
				 f = exp(-dV / (fogVisibility-dV ) )
				
                c*=f;
				c += fogCOlor * (1.0-f);

			*/

			vtlp->color = D3DRGBA_X(c.r,c.g,c.b,c.a); // this involves a * 255 mul ==> Katami
			// to do: use Katmai pack to DWORD 

			if (doSpecular) {
				// clamp or scale by max 
				if (cspec.r>1.0f) cspec.r=1.0f;
				if (cspec.g>1.0f) cspec.g=1.0f;
				if (cspec.b>1.0f) cspec.b=1.0f;

				vtlp->specular = D3DRGBA_X(cspec.r,cspec.g,cspec.b,cspec.a);	// to do: use Katmai pack

			} else vtlp->specular = 0;

		}
		else { // unlit 
			if (doVc) {
				const Point &c = vc[i]; // we have vertex colors 
				vtlp->color = D3DRGBA_X(c.x,c.y,c.z,mat.diffuse.a); // costly 
			}
			else vtlp->color = state.currentColorD3D; 
			vtlp->specular = 0; // vtlp->color; 
		}
		
#if 0
		// already setup in buffer 
		// texture coordinates 
		if (doVp) {
			const Point &p =vp[i];
			
			if (!state.textureMatrixIdentity) {
				Matrix &M = state.textureMatrix;
				vtlp->tu = p.x * M.x[0][0] + p.y * M.x[1][0] /* + p.z * M.x[2][0] */ + M.x[3][0];
				vtlp->tv=  p.x * M.x[0][1] + p.y * M.x[1][1] /* + p.z * M.x[2][1] */+ M.x[3][1];

			} 
			else 
			{  
				vtlp->tu = p.x;
				vtlp->tv = p.y;
			}

		} else {
			vtlp->tu = 0;
			vtlp->tv = 0;
		}

#endif

		vtlp++;

	}

//	TRACE("zmin %g zmax %g  wmin %g wmax %g \n",zmin,zmax,wmin,wmax);
	// end transform & light 
drawIt: 

	// triangles // faces
	if (state.faces.visible) {

	// Index buffer 
	Array<WORD> &tri=this->primIndex; // state.indexBuffer;
	int nPrim; // number of primitives


	if (!culling && (tri.Length() > 0) && ~ (actions & SHELL_COMPUTE_EXBUF) && (primType == D3DPT_TRIANGLELIST)) {
		// index buffer is ok
		nPrim = this->nPrim;
	}
	else 
	{

	// make tri buffer large enough 
	if (tri.Length() <(f.Length()*3))  // account for culling 
		tri.SetLength(f.Length()*3);

	WORD *fpi = tri.DataRW();
	int ntris=0;



	const int *fp = f.Data();				   // face list stepping pointers
	const int *fpend= fp+f.Length();   // points after last element of facelist
	int fi=-1;					// face index

	const int maxVerts = 20000;


	if (nv>maxVerts) { // new pass, pass smaller strips

		FloatArrayAligned &vtlout= state.vtlBuffer; // use global buffer

		// vtlout.SetLength(maxVerts+2);

		vtlout.SetLength((sizeof(D3DTLVERTEX) / sizeof(float)) * (maxVerts+2));
		const D3DTLVERTEX *vltp= (D3DTLVERTEX*) vtl.Data();

		//IntArray vertMap(nv);	


		while (fp <fpend) {		// while face list 

		// for (i=0; i<nv; i++) vertMap[i]=-1; // not used

		// memset(vertMap.Data(),0xFF,nv*sizeof(int)); very very costly
		

		int newNv = 0;
		int newNTri = 0;
		
		D3DTLVERTEX *vtloutp= (D3DTLVERTEX*) vtlout.Data();

		const int *fpThisStart = f.Data();
		const int *fpThisEnd = fp;
		const int thisFi = fi;

		// pass 1: check how many faces are fitting
		// use only vertices refed by those faces 
		fpi = tri.DataRW();
		ntris=0;


		int p1,p2,p3;
		int p1n,p2n,p3n;


		while (fp<fpend) {
  			int flen = *fp++; 	// length of this face
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
				case 2:
					fp += flen;
					break;
			    default :
					
					p1 = *fp++;
					p2 = *fp++;
					flen -=2 ;
					p3 = *fp;


					// newNTri += flen -2;
					// if (((vltp[p1].sz <0) && (vltp[p2].sz <0) && (vltp[p3].sz <0)) ) goto skip;


				float tmp;
				if (culling) 
					// tmp= (vltp[p2].sx - vltp[p1].sx) * (vltp[p3].sy - vltp[p2].sy) - (vltp[p2].sy - vltp[p1].sy) * (vltp[p3].sx - vltp[p2].sx);
				    tmp = (vltp[p1].sx * vltp[p2].sy) - (vltp[p2].sx * vltp[p1].sy) +
							(vltp[p2].sx * vltp[p3].sy) - (vltp[p3].sx * vltp[p2].sy) +
							(vltp[p3].sx * vltp[p1].sy) - (vltp[p1].sx * vltp[p3].sy);

				else  tmp = -1.0f;
				if (tmp <= 0.0f)
				{

						// if (vertMap[p1]  <0 ) 
						{ // need a new vertex
						//	vertMap[p1]= newNv;
							*vtloutp++ = vltp[p1]; // xxxx
							p1n=newNv;
							newNv ++;

						}
						//if (vertMap[p2] <0) 
						{ // need a new vertex
						//	vertMap[p2]= newNv;
							
							*vtloutp++ = vltp[p2]; // xxxx

							p2n=newNv;
							newNv ++;
						}


					while (flen>0) {


						p3 = *fp++;
#if 0
						if (vertMap[p3] <0 ) 
						{ // need a new vertex
							vertMap[p3]= newNv;
							*vtloutp++ = vltp[p3]; // xxxx
							newNv ++;
						}
						// store triangle 
						{ *fpi++ = vertMap[p1]; *fpi++ = vertMap[p2]; *fpi++ = vertMap[p3]; ntris++; }
#else
							p3n=newNv;
							*vtloutp++ = vltp[p3]; // xxxx
							newNv ++;
							// store triangle 
							{ *fpi++ = p1n; *fpi++ = p2n; *fpi++ = p3n; ntris++; }
							p2n=p3n;

#endif
						
						p2=p3;

						flen--;
					}
				} else skip : fp += flen;
				
				break;
			}
           
		} // while (fp<fpend)
		// fpThisEnd = fp;

		nPrim = ntris*3;

		// draw this part 
#if 1
		if (state.dx6) state.GLastError = state.lpD3DDevice3->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, D3DFVF_TLVERTEX,
		(LPVOID) vtlout.Data(),newNv, (LPWORD) tri.Data(), nPrim, primMask);
		else state.GLastError = state.lpD3DDevice2->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,D3DVT_TLVERTEX,
		(LPVOID) vtlout.Data(),newNv, (LPWORD) tri.Data(), nPrim, primMask);
		if (state.GLastError != D3D_OK) { D3DError("Error DrawIndexedPrimitive",state.GLastError); }
#else
		if (state.dx6) state.GLastError = state.lpD3DDevice3->DrawPrimitive(D3DPT_TRIANGLELIST, D3DFVF_TLVERTEX,
		(LPVOID) vtlout.Data(),newNv, nPrim, primMask);
		else state.GLastError = state.lpD3DDevice2->DrawPrimitive(D3DPT_TRIANGLELIST,D3DVT_TLVERTEX,
		(LPVOID) vtlout.Data(),newNv, nPrim, primMask);
		if (state.GLastError != D3D_OK) { D3DError("Error DrawIndexedPrimitive",state.GLastError); }

#endif


		// pass 2 
		// fp = fpThisStart;
		}

		return(0);


	} else

	if (!culling) { // noop , no culling test done 
	while (fp<fpend)
	{
		int flen = *fp++;
		int p1,p2,p3;

		if (flen==3)
		{
			p1=*fp++; p2=*fp++; p3=*fp++;
			//if (!doVertexCull || (vcull[p1] || vcull[p2] || vcull[p3]))
			{	
				*fpi++=p1; *fpi++=p2; 	*fpi++=p3;
				ntris++;
			}
		}
		else if (flen > 3) 
		{
			p1=*fp++;
			p2=*fp++;
			flen-=2;
			while (flen) 
			{
				flen--;
				p3 = *fp++;
				
				//if (!doVertexCull || (vcull[p1] || vcull[p2] || vcull[p3]))
				{	
					*fpi++ = p1; *fpi++ = p2; *fpi++ = p3;
					ntris++;
				}

				p2=p3;

			}

		}
		else {
		  if (flen<0) flen = -flen;	
		  fp+=flen;
		}
	} // while 

	}
	else if (1) { // Katmai backface culling 


#ifdef _KATMAI	

		if (useKatmai) {
		if ( (kfn.Length() ==0) || (actions & SHELL_UPDATE_EXBUF_VERT) ) {
			// need to update Face info for Katmai

			if (!state.frontFaceCW) 
				kfn.Set(GetFN(),-1.0f); // copy from AOS to SOA 
			else kfn.Set(GetFN(),1.0f); // copy from AOS to SOA 

			kfv.SetLength(kfn.Length());
			fi=-1;
			while (fp < fpend) {		// while more faces
  				int flen = *fp++; 	// length of this face
				
				ASSERT(flen>=3);

				//if (flen<0) flen=-flen;   // should not happend at this level
				//else 

				fi++;

				ASSERT(fi<kfv.Length());

				
				//int index =  (fi >>2)*P4_STRIDE + (fi & 3);

				//kfv.x[index] = v[*fp].x,	kfv.x[index+4] = v[*fp].y,	kfv.x[index+8] = v[*fp].z;

				kfv.Set1(fi,v[*fp]); // use first face vertex
				fp+=flen;
			}
			fp = f.Data();
			fi = -1;

		}

		fp = f.Data();
		int nprims =  KatmaiBuildFaceList(fp,fpend, 						
						kfn.Data(), kfv.Data(),
						fpi);
	
		fpi += nprims;

		ntris = nprims / 3;
		}
		else
#endif
		  // software culling with FN
		{
			fi=-1;
			GetFN();

			Point toViewer;			// points from point to viewer 

			while (fp < fpend) {		// while more faces
  				int flen = *fp++; 	// length of this face
				
				ASSERT(flen>=0);

				//if (flen<0) flen=-flen;   // should not happend at this level
				//else 

				fi++;
				if (flen > 2) {
				ASSERT(flen>=3);


				toViewer= viewpoint - v[*fp]; // vector to viewer in local space

				// toViewer.Normalize();
				float tmp=DotProd(toViewer,fn[fi]); 
				if (state.frontFaceCW) tmp = -tmp;

				if ( tmp >= 0.0f) { 
					int p1= fp[0]; int p2= fp[1];
					int i=2;
					//for (i=2; i<flen; i++)
					do 
					{
						fpi[0] = p1; fpi[1] = p2; 
						p2 = fp[i];
						fpi[2] =p2;
						fpi+=3; 
						i++;
					} while (i<flen);
				}
				}
			
				fp+=flen;

			}
			ntris = (fpi - tri.Data()) / 3;
		} // FN culling

	}
	else { 
	// screen space culling doesn"t work always ??
	const D3DTLVERTEX *vltp= (D3DTLVERTEX*) vtl.Data();

	// version with culling test, but didn"t work always properly 

	while (fp < fpend) {		// while more faces
  			int flen = *fp++; 	// length of this face
			
			if (flen<0) flen=-flen;   // should not happend at this level
			else fi++;

			//Point &n = fn[fi];
			int p1,p2,p3;
#if 0
			if (flen==3) { 		// TRIANGLE

				p1=  *fp++;
				p2 = *fp++;
				p3 = *fp++;

				if (!culling) {
				 	*fpi++ = p3; *fpi++ = p2; *fpi++ = p1;
					ntris++;

					*fpi++ = p1; *fpi++ = p2; *fpi++ = p3;
					ntris++;
					continue;
				}

				if (state.frontFaceCW) { // FLIP
					int tmp=p1;
					p1=p3; p3 = tmp;
				}
				

				//Point tmp =  CrossProd(Point(vltp[p2].sx - vltp[p1].sx,1.0 *(vltp[p2].sy - vltp[p1].sy),vltp[p2].sz - vltp[p1].sz),
				//				Point(vltp[p3].sx - vltp[p2].sx,1.0*(vltp[p3].sy - vltp[p2].sy),vltp[p3].sz - vltp[p2].sz));

				//if (tmp.z < 0)
				float tmp = (vltp[p1].sx * vltp[p2].sy) - (vltp[p2].sx * vltp[p1].sy) +
							(vltp[p2].sx * vltp[p3].sy) - (vltp[p3].sx * vltp[p2].sy) +
							(vltp[p3].sx * vltp[p1].sy) - (vltp[p1].sx * vltp[p3].sy);

				//float tmp = (vltp[p2].sx - vltp[p1].sx) * (vltp[p3].sy - vltp[p2].sy) - (vltp[p2].sy - vltp[p1].sy) * (vltp[p3].sx - vltp[p2].sx);
				if (tmp < 0.0f)
				{
				 	*fpi++ = p1; *fpi++ = p2; *fpi++ = p3;
					ntris++;

					if (!culling) {
				 		*fpi++ = p1; *fpi++ = p3; *fpi++ = p2;
						ntris++;
					}
				}


			} 
			else 
#endif
			if (flen >= 3) {
				p1 = *fp++;
				p2 = *fp++;
				flen-=2;

				p3 = *fp;

				float tmp;

				bool flip = false;

				if (culling) {
					// tmp= (vltp[p2].sx - vltp[p1].sx) * (vltp[p3].sy - vltp[p2].sy) - (vltp[p2].sy - vltp[p1].sy) * (vltp[p3].sx - vltp[p2].sx);
				    tmp = (vltp[p1].sx * vltp[p2].sy) - (vltp[p2].sx * vltp[p1].sy) +
							(vltp[p2].sx * vltp[p3].sy) - (vltp[p3].sx * vltp[p2].sy) +
							(vltp[p3].sx * vltp[p1].sy) - (vltp[p1].sx * vltp[p3].sy);
					if (state.frontFaceCW) tmp = -tmp;
				}
				else tmp = -1.0;
			
				
				if (tmp < 0.0f)
				{
					while (flen>0) {
						p3 = *fp++;
						ASSERT(p3 != p1);
							// TRACE("toc! \n %d", flen);

						flen--;


						//TRACE("IN %d %d  %d (Face %d) \n",p1,p2,p3,fi);
						// add triangle
						// tmp= (vltp[p2].sx - vltp[p1].sx) * (vltp[p3].sy - vltp[p2].sy) - (vltp[p2].sy - vltp[p1].sy) * (vltp[p3].sx - vltp[p2].sx);
						
						// xxxx if (!((vltp[p1].sz <0) && (vltp[p2].sz <0) && (vltp[p3].sz <0)) )
							
						// if (tmp <0) 
						{	*fpi++ = p1; *fpi++ = p3; *fpi++ = p2; }

						// else { *fpi++ = p1; *fpi++ = p2; *fpi++ = p3; }
						ASSERT((p1>=0) && (p2>=0) && (p3>=0) );
						ASSERT((p1<nv) && (p2<nv) && (p3<nv) );


						ntris++;
/*
						if (!culling) {
							*fpi++ = p1; *fpi++ = p3; *fpi++ = p2;
							ntris++;
						} 
*/
						p2=p3;
					}
				
				}
				else fp+=flen;
			}
			else fp+=flen;
			
			
			ASSERT(fpi < (tri.Data() + tri.MaxLength()));

    }	// while fp
	}

	ASSERT(fpi < (tri.Data() + tri.MaxLength()));
	// TRACE("Tris %d %d  %d \n",fpi-tri.Data(),ntris*3,tri.MaxLength());
	nPrim = ntris*3;
	this->nPrim = ntris*3;
	primType = D3DPT_TRIANGLELIST;
	}


#ifdef _GSTATS
	verticesProcessed +=nv;
	facesProcessed+= nfaces;
	trianglesDrawn += (nPrim / 3);
	if ((nPrim / 3)> maxPrimTriCnt) maxPrimTriCnt= (nPrim / 3);
	if (nv> maxPrimVertexCnt) maxPrimVertexCnt= nv;

#endif

	if (nPrim <=0) return -1; // if all culled 

	state.primsDrawn++;
	state.trisDrawn += (nPrim / 3); 

	// D3DFVF_TLVERTEX 
	if (state.dx6) state.GLastError = state.lpD3DDevice3->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, D3DFVF_TLVERTEX,
		(LPVOID) vtl.Data(),nv, (LPWORD) tri.Data(), nPrim, primMask);
	else state.GLastError = state.lpD3DDevice2->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,D3DVT_TLVERTEX,
		(LPVOID) vtl.Data(),nv, (LPWORD) tri.Data(), nPrim, primMask);
	if (state.GLastError != D3D_OK) { D3DError("Error DrawIndexedPrimitive",state.GLastError); }
	} // faces
	else	

	// edges
	if (state.edges.visible) {

	// to do :
	if (nv>D3D_maxVerts) {
		return -1;
	}
	else {
		
		if ((primIndex.Length() > 0) && ~ (actions & SHELL_COMPUTE_EXBUF)  && (primType == D3DPT_LINELIST)) {
			// index buffer is ok
		}
		else 
		{
			nPrim = BuildLineList(f,gtrue,primIndex);
			primType = D3DPT_LINELIST;
		}

		if (nPrim<=0) return -1; // if all culled 

		if (state.dx6) state.GLastError = state.lpD3DDevice3->DrawIndexedPrimitive(D3DPT_LINELIST, D3DFVF_TLVERTEX,
			(LPVOID) vtl.Data(),nv, (LPWORD) primIndex.Data(), nPrim, primMask);
		else state.GLastError = state.lpD3DDevice2->DrawIndexedPrimitive(D3DPT_LINELIST,D3DVT_TLVERTEX,
			(LPVOID) vtl.Data(),nv, (LPWORD) primIndex.Data(), nPrim, primMask);
		if (state.GLastError != D3D_OK) { D3DError("Error DrawIndexedPrimitive",state.GLastError); }
	}

	}	// edges
	// vertices
	else if (state.vertices.visible) 
	{
		if (state.dx6) state.GLastError = state.lpD3DDevice3->DrawPrimitive(D3DPT_POINTLIST, D3DFVF_TLVERTEX,
			(LPVOID) vtl.Data(), nv, primMask);
		else state.GLastError = state.lpD3DDevice2->DrawPrimitive(D3DPT_POINTLIST,D3DVT_TLVERTEX,(LPVOID) vtl.Data(), nv, primMask);
		if (state.GLastError != D3D_OK) { D3DError("Error DrawPrimitive",state.GLastError); }

	}
	return(0);

}


//===========================================================================================
//					int GShell::D3dRenderVB(GglRenderTraversal &traversalState)
//===========================================================================================

//! Render Shell using D3D IM + vertex buffers 

int GShell::D3dRenderVB(GglRenderTraversal &traversalState)
{
	RenderState &state = *traversalState.glState;
	gbool culling = state.backfaceCulling; // backface culling enabled ?
	gbool singlsidelighting = culling;
	int ret=0;

	DWORD primMask;

	if (traversalState.doClipping || 1 ) // 1 debug only, check culling
		primMask =  D3DDP_DONOTUPDATEEXTENTS; // | D3DDP_DONOTLIGHT 
	else  
		primMask = D3DDP_DONOTCLIP |  D3DDP_DONOTUPDATEEXTENTS;

	state.SetCullMode(culling  ? (state.frontFaceCW ? D3DCULL_CCW: D3DCULL_CW) : D3DCULL_NONE);
	culling =0; // d3d does the culling 
	int nv= v.Length();
	if (nv >= D3DMAXNUMVERTICES ) { // MAXWORD
		TRACE("D3dRenderVB too many vertices %d \n",nv);
		return(-1);
	}
	
	if (nv == 0 || (f.Length()<4)) 
		return -1;

	gbool doVn = !state.Unlit();

	if (!doVn) {
		if (!this->isUnlit) { // we are in lit mode but need unlit
			actions |= SHELL_COMPUTE_EXBUF;
		}
		else if (this->currentColorD3D != state.currentColorD3D) // need update colors in vertex buffer 
			actions |= SHELL_UPDATE_EXBUF_VERT;
	} else {

	}		

	//
	// create / update vertex buffer 
	//
	// do we need to (re) create the D3D ExecuteBuffers ?
	//tbd think about ok(): does renderstate destroy handle? ->crash
		if ( (actions & SHELL_COMPUTE_EXBUF) || !hVBBuf.Ok()	)// Exbuf destroyed, eg. Window resized 
		{
			// temp workaround until face color support in D3D
			//if (fc.Length()>0 && state.faces.colored && state.canDoRgbLighting) 
			//	ComputeVertexColorsFromFaceColors();

			ret = CreateVBBuf(state);

			actions &= ~(SHELL_COMPUTE_EXBUF|SHELL_UPDATE_EXBUF_VERT);
			if (ret<=0) 
				return(ret);
		}
		else if ((actions & SHELL_UPDATE_EXBUF_VERT)) { // only update vertices 

			ret = UpdateVBBuf(state);
			actions &= ~ (SHELL_UPDATE_EXBUF_VERT);
			if (ret<=0) 
				return(ret);
		}
	
	// unlit ?
	if (isUnlit) 
			primMask |= D3DDP_DONOTLIGHT; 


	// triangles + faces (in contrast to wireframe)
	if (state.faces.visible) {

		// Index buffer 
			Array<WORD> &tri=this->primIndex; // state.indexBuffer;
			int nPrim; // number of primitives


			if (!culling && (tri.Length() > 0) && ~ (actions & SHELL_COMPUTE_EXBUF) && (primType == D3DPT_TRIANGLELIST)) {
				// index buffer is ok
				nPrim = this->nPrim;
			}
			else 
			{
				//do culling?????
					// make tri buffer large enough 
					if (tri.Length() <(f.Length()*3))  // account for culling 
						tri.SetLength(f.Length()*3);

					WORD *fpi = tri.DataRW();
					int ntris=0;

					const int *fp = f.Data();				   // face list stepping pointers
					const int *fpend= fp+f.Length();   // points after last element of facelist
					int fi=-1;					// face index


					if (!culling) { // noop , no culling test done 
						while (fp<fpend)
						{
							int flen = *fp++;
							int p1,p2,p3;

							if (flen==3)
							{
								p1=*fp++; p2=*fp++; p3=*fp++;
								{	
									*fpi++=p1; *fpi++=p2; 	*fpi++=p3;
									//TRACE("facelist: (%i %i %i)\n", p1, p2, p3);
									ntris++;
								}
							}
							else if (flen > 3) //split quads(?) to triangles
							{
								p1=*fp++;
								p2=*fp++;
								flen-=2;
								while (flen) 
								{
									flen--;
									p3 = *fp++;
									{	
										*fpi++ = p1; *fpi++ = p2; *fpi++ = p3;
										//TRACE("facelistq: (%i %i %i)\n", p1, p2, p3);
										ntris++;
									}
									p2=p3;
								}
							}
							else {
							  if (flen<0) flen = -flen;	
							  fp+=flen;
							}
						} // while 

					}

				ASSERT(fpi < (tri.Data() + tri.MaxLength()));
				// TRACE("Tris %d %d  %d \n",fpi-tri.Data(),ntris*3,tri.MaxLength());
				nPrim = ntris*3;
				this->nPrim = ntris*3;
				primType = D3DPT_TRIANGLELIST;
			}


#ifdef _GSTATS
			verticesProcessed +=nv;
			facesProcessed+= nfaces;
			trianglesDrawn += (nPrim / 3);
			if ((nPrim / 3)> maxPrimTriCnt) maxPrimTriCnt= (nPrim / 3);
			if (nv> maxPrimVertexCnt) maxPrimVertexCnt= nv;

#endif

			if (nPrim <=0) 
				return -1; // if all culled 

			state.primsDrawn++;
			state.trisDrawn += (nPrim / 3); 

			//
			// draw the stuff
			if (state.dx6 && hVBBuf.Ok()) 	 
				state.GLastError = state.lpD3DDevice3->DrawIndexedPrimitiveVB(D3DPT_TRIANGLELIST,
																			hVBBuf.handle->lpD3DVBBuf,
																			(LPWORD) tri.Data(),
																			nPrim,
																			primMask);

			if (state.GLastError != D3D_OK) 
				{ D3DError("Error DrawIndexedPrimitive",state.GLastError); }
	} // faces
	
	else
		// edges
		if (state.edges.visible) {
			
				if ((primIndex.Length() > 0) && ~ (actions & SHELL_COMPUTE_EXBUF)  && (primType == D3DPT_LINELIST)) {
					// index buffer is ok
				}
				else 
				{
					nPrim = BuildLineList(f,gtrue,primIndex);
					primType = D3DPT_LINELIST;
				}

				if (nPrim<=0) return -1; // if all culled 

				if (state.dx6) 
					state.GLastError = state.lpD3DDevice3->DrawIndexedPrimitiveVB(D3DPT_LINELIST,
																			hVBBuf.handle->lpD3DVBBuf,
																			(LPWORD) primIndex.Data(),
																			nPrim,
																			primMask);
				if (state.GLastError != D3D_OK) { D3DError("Error DrawIndexedPrimitive",state.GLastError); }

			}	// edges
		// vertices
		else if (state.vertices.visible) 
			{
				if (state.dx6) 
					state.GLastError = state.lpD3DDevice3->DrawPrimitiveVB(D3DPT_POINTLIST,
																			hVBBuf.handle->lpD3DVBBuf,
																			0,
																			nPrim,
																			primMask);
				if (state.GLastError != D3D_OK) { D3DError("Error DrawPrimitive",state.GLastError); }
			}
	return(0);

}


/*===============================================================================
			int GShell::CreateVBBuf(RenderState &state,G3ExBuf &buf)
===============================================================================*/
//Create Direct 3D Vertex Buffer and fill it with data
//compare with CreateExBuf

int GShell::CreateVBBuf(RenderState &state)
{
	//get prepared

			int nv=GetVertexCnt(); 

			if (nv == 0) return(0);
			gbool flip = state.FlipFaces();
			gbool flipNormals = state.FlipNormals();
			gbool lit = gtrue;
			
			lit = !state.Unlit();


			//TRACE("Creating Vertex buf v=%d tri=%d size=%d \n",nv,nTri,size);
	
	//
    // create vb buffer
	// tbd delegate creation stuff to renderstate


			DWORD dwFVF =0;
			dwFVF = D3DFVF_XYZ;

			if (lit) {  dwFVF |= D3DFVF_NORMAL; }
			else { dwFVF |= D3DFVF_DIFFUSE;}

			if (vc.Length()>0) { dwFVF |= D3DFVF_DIFFUSE;}

			if (state.textureDefined && vp.Length()>0) { dwFVF|=D3DFVF_TEX1;}
			
			if (!state.CreateVBBuf(hVBBuf,dwFVF, nv)) 
			{
				return 0;
			}

			D3DVALUE* pVertices;

			if (lit) {
				this->isUnlit = gfalse;
			} else {
				this->currentColorD3D = state.currentColorD3D;
				this->isUnlit = gtrue;
			}

			if (!state.LockVB(hVBBuf,pVertices)) 
				return 0;

	
			// get the vertices
			GetVertex(state,flipNormals,hVBBuf.handle->vbdesc.dwFVF,state.currentColorD3D,pVertices); 
  
			state.UnlockVB(hVBBuf);
	
			//if (state.dx6) hVBBuf.handle->lpD3DVBBuf->Optimize(state.lpD3DDevice3, 0);
	textureMatrixVersion = state.GetTextureMatrixVersion();
			
	return(1);
}



/*===============================================================================
			int GShell::UpdateVBBuf(RenderState &state)
===============================================================================*/
//Update a Direct 3D Vertex Buffer and fill it with data in case of vertex animation
//compare with CreateExBuf

int GShell::UpdateVBBuf(RenderState &state)
{
			int nv=GetVertexCnt(); 
		
			if (nv == 0) return(0);
			ASSERT(nv==hVBBuf.handle->vbdesc.dwNumVertices);
			if (nv!=hVBBuf.handle->vbdesc.dwNumVertices) 
				return(0);
			gbool flip = state.FlipFaces();
			gbool flipNormals = state.FlipNormals();


	//
    // update vertices
	//
			D3DVALUE* pVertices;
			if (!state.LockVB(hVBBuf,pVertices)) 
				return 0;

			// get the vertices
			GetVertex(state,flipNormals,hVBBuf.handle->vbdesc.dwFVF,state.currentColorD3D,pVertices); 
  
			state.UnlockVB(hVBBuf);

	this->currentColorD3D = state.currentColorD3D;
	textureMatrixVersion = state.GetTextureMatrixVersion();

	return(nv);
}


//-------------------------------------------------------------------------------------------
//			int GShell::GetVertex(RenderState &state,int flipNormals,D3DVERTEX *V)
//-------------------------------------------------------------------------------------------
// Get vertex in D3DVertex format 
int GShell::GetVertex(RenderState &state,int flipNormals,D3DVERTEX *V)
{

	if ((vn.Length() == 0) || (actions & SHELL_COMPUTE_VNORMALS)) {
		ComputeVertexNormals();
	}

    int nv = v.Length();
    D3DVERTEX *pV = V;

    const Point *pv = v.Data();
    const Point *pn = vn.Data();
    const Point *pp = vp.Data();


	//TRACE("GShell:GetVertex identity = %d version %d \n",state.textureMatrixIdentity,state.GetTextureMatrixVersion());

    if (!pp) pp = pv; // use vertex coordinates

    for(int i=0; i < nv; i++) {
        pV->x = pv->x; pV->y = pv->y; pV->z = pv->z;

#if 0
		float l= pn->Length();
		if (l != 1.0f) {
			TRACE("Not unit normal %d %f %f %f %f \n", i,pn->x,pn->y,pn->z,l);
		}
#endif

		if (flipNormals)
        { pV->nx = -pn->x; pV->ny = -pn->y; pV->nz = -pn->z; }
		else 
        { pV->nx = pn->x; pV->ny = pn->y; pV->nz = pn->z; }

		if (!state.textureMatrixIdentity) {
			Point pt= state.textureMatrix * *pp;
			pV->tu = pt.x;  pV->tv = pt.y; 
		} else 
		{   pV->tu = pp->x; pV->tv = pp->y; }
        pV++; 
        pv++;
        pn++;
        pp++;
 }
 return(nv);
}


//-------------------------------------------------------------------------------------------
//			int GShell::GetVertex(RenderState &state,int flipNormals,D3DCVERTEX *V)
//-------------------------------------------------------------------------------------------
// Get vertex in D3DCVERTEX format 
int GShell::GetVertex(RenderState &state,int flipNormals,D3DCVERTEX *V)
{

	if ((vn.Length() == 0) || (actions & SHELL_COMPUTE_VNORMALS)) {
		ComputeVertexNormals();
	}

    int nv = v.Length();
    D3DCVERTEX *pV = V;

    const Point *pv = v.Data();
    const Point *pc = vc.Data();
    const Point *pn = vn.Data();
    const Point *pp = vp.Data();


	//TRACE("GShell:GetVertex identity = %d version %d \n",state.textureMatrixIdentity,state.GetTextureMatrixVersion());

    if (!pp) pp = pv; // use vertex coordinates
    if (!pp) pc = pv; // use vertex coordinates

    for(int i=0; i < nv; i++) {
        pV->x = pv->x; pV->y = pv->y; pV->z = pv->z;

#if 0
		float l= pn->Length();
		if (l != 1.0f) {
			TRACE("Not unit normal %d %f %f %f %f \n", i,pn->x,pn->y,pn->z,l);
		}
#endif

		if (flipNormals)
        { pV->nx = -pn->x; pV->ny = -pn->y; pV->nz = -pn->z; }
		else 
        { pV->nx = pn->x; pV->ny = pn->y; pV->nz = pn->z; }

		pV->color = D3DRGB(pc->x,pc->y, pc->z);	 // to do: a * clamp !
		pc++;

		if (!state.textureMatrixIdentity) {
			Point pt= state.textureMatrix * *pp;
			pV->tu = pt.x;  pV->tv = pt.y; 
		} else 
		{   pV->tu = pp->x; pV->tv = pp->y; }

        pV++; 
        pv++;
        pn++;
        pp++;
 }
 return(nv);
}

//-------------------------------------------------------------------------------------------
//		int GShell::GetVertex(RenderState &state,int flipNormals,DWORD flags, D3DVALUE *V)
//-------------------------------------------------------------------------------------------
// get vertices in DX flexibile vertexformat
// suported flags D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 

int GShell::GetVertex(RenderState &state,int flipNormals,DWORD flags, D3DCOLOR diffuse, D3DVALUE *V)
{


    int nv = v.Length();

	if (nv==0 ) return 0;

	if ((flags & D3DFVF_NORMAL) && ((vn.Length() == 0) || (actions & SHELL_COMPUTE_VNORMALS))) {
		ComputeVertexNormals();
	}


    D3DVALUE *pV = V;

    const Point *pv = v.Data();
    const Point *pc = vc.Data();
    const Point *pn = vn.Data();
    const Point *pp = vp.Data();


	//TRACE("GShell:GetVertex identity = %d version %d \n",state.textureMatrixIdentity,state.GetTextureMatrixVersion());

    if (!pp) pp = pv; // use vertex coordinates
	int i=nv;
    do	{
		
		// position
        pV[0] = pv->x; pV[1] = pv->y; pV[2] = pv->z;
			//TRACE("vertex (%f %f %f)\n", pV[0], pV[1], pV[2]);
		pV+=3;
        pv++;

		// w
		// weights
		// normal

		if (flags & D3DFVF_NORMAL) {
			if (flipNormals)
			{ pV[0] = -pn->x; pV[1] = -pn->y; pV[2] = -pn->z; }
			else 
			{ pV[0] = pn->x; pV[1] = pn->y; pV[2] = pn->z; }
			//TRACE("normal (%f %f %f)\n", pV[0], pV[1], pV[2]);
			pV+=3;
			pn++;
		}

		// diffuse color
		if (flags & D3DFVF_DIFFUSE) {
			if (pc) {
				((D3DCOLOR *)pV)[0] = D3DRGBA_X(pc->x,pc->y,pc->z, 1);	 // to do: a * clamp !
				pc++;
			} else ((D3DCOLOR *)pV)[0]= diffuse;
			pV+=1;
		}
		// specular color

		// texture coordinate 1..8
		if (flags & D3DFVF_TEX1) {
			if (!state.textureMatrixIdentity) {
				Point pt= state.textureMatrix * *pp;
				pV[0] = pt.x;  pV[1] = pt.y; 
			} else 
			{   pV[0] = pp->x; pV[1] = pp->y; }
			pp++;
			pV+=2;
		}

        //pV+=vExtraStride;

		i--;
	} while (i>0);
	return(nv);
}


// get pre-lit vertices
int GShell::GetVertex(RenderState &state,D3DCOLOR color, D3DCOLOR specular,D3DLVERTEX *V)
{

    int nv = v.Length();
    D3DLVERTEX *pV = V;

    const Point *pv = v.Data();
    const Point *pc = vc.Data();
    const Point *pp = vp.Data();

    if (!pp) pp = pv; // use vertex coordinates

    for(int i=0; i < nv; i++) {
        pV->x = pv->x; pV->y = pv->y; pV->z = pv->z;
		pV->dwReserved = 0;

		if (!state.textureMatrixIdentity) {
			Point pt= state.textureMatrix * *pp;
			pV->tu = pt.x;  pV->tv = pt.y; 
		} else 
		{     pV->tu = pp->x; pV->tv = pp->y; }

		if (pc) {
			pV->color = D3DRGB(pc->x,pc->y, pc->z);	 // to do: a * clamp !
			pc++;
		}
		else pV->color = color;
		pV->specular = specular;
        pV++; 
        pv++;
        pp++;
 }
 return(nv);
}




// get vertex mapped 
int GShell::GetVertex(RenderState &state,int flipNormals,D3DVERTEX *V,IntArray &map)
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
		pV = V + j;
        pV->x = pv[i].x;
        pV->y = pv[i].y;
        pV->z = pv[i].z;
		if (flipNormals)
        { pV->nx = -pn[i].x;  pV->ny = -pn[i].y; pV->nz = -pn[i].z; }
		else { pV->nx = pn[i].x;  pV->ny = pn[i].y; pV->nz = pn[i].z; }
		
		if (!state.textureMatrixIdentity) {
			Point pt= state.textureMatrix * pp[i];
			pV->tu = pt.x;  pV->tv = pt.y; 
		} else 
		{ pV->tu = pp[i].x; pV->tv = pp[i].y; }
		cnt ++;
	}
	return(cnt);
}

// get vertex mapped unlit
int GShell::GetVertex(RenderState &state,D3DCOLOR color, D3DCOLOR specular,D3DLVERTEX *V,IntArray &map)
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
		
		pV = V + j;
        pV->x = pv[i].x;
        pV->y = pv[i].y;
        pV->z = pv[i].z;

		if (pc) {
			pV->color = D3DRGB(pc[i].x,pc[i].y, pc[i].z);	 // to do: a * clamp !
		}
		else pV->color = color;
		pV->specular = specular;

		if (!state.textureMatrixIdentity) {
			Point pt= state.textureMatrix * pp[i];
			pV->tu = pt.x;  pV->tv = pt.y; 
		} else 
		{ pV->tu = pp[i].x; pV->tv = pp[i].y; }

		cnt ++;
	}
	return(cnt);
}



/*
Create Direct 3D Execute Buffer
*/
int GShell::CreateExBuf(RenderState &state,G3ObjRef &objBuf)
{
    
	size_t size;
    int nv=GetVertexCnt(); 
    int nTri = f.GetTriangleCount();

#ifdef _DEBUG
	int stripCnt=0;
#endif

    if (nv == 0 || nTri == 0) return(0);

	gbool flip = state.FlipFaces();
	// int flipNormals =  state.FlipNormals();
	gbool flipNormals =  state.frontFaceCW;
	//	int flipNormals =  0; // 2.04.97

	gbool bothSides = state.NeedBothFaces();

	gbool unlit= (state.canDoRgbLighting && state.faces.colored && (vc.Length()>0)) 
		|| state.Unlit(); 
	gbool hasvc= (vc.Length()>0);
	
	D3DCOLOR color, specular;
	
	if (unlit) {
		//color = D3DCOLOR(1,1,1);
		//color  = RGBA_MAKE(255,255,255,255);
		color = state.getCurrentColorD3D();
		specular = RGBA_MAKE(0,0,0,255);
	}


	gbool tooManyVerts = FALSE;

	if ((state.maxVertexCount>0) && (nv > state.maxVertexCount)) 
		tooManyVerts = TRUE;

	// tooManyVerts = TRUE; //for testing debug



	int bufi=0;	// the current execute buffer created 


	while(1) {  // two passes if bothSides 
	
	if (tooManyVerts) {
		IntArray vertMap(nv);	// table for mapping used vertices 
		IntArray fmap;		// termporay to remap a face 
		int i;
		// TRACE("to many verts %d \n",nv);
		int maxVerts = state.maxVertexCount;

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
				case 2:
					fp += flen;
					break;
			    default :
					newNTri += flen -2;

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
		size += buf.TRIANGLE_LIST_SIZE(newNTri)+30*buf.OP_SIZE();

		// TRACE("Creating Ex buf  v=%d tri=%d size=%d ccw = %d flip = %d\n",newNv,newNTri,size,state.frontFaceCW,flipNormals);

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

		if (state.maxVertexCount>0) {
			ASSERT(newNv <= state.maxVertexCount);
		}

		// store misc instructions

		OP_PROCESS_VERTICES(1, buf.lpPointer);
			if (unlit) {
				PROCESSVERTICES_DATA(D3DPROCESSVERTICES_TRANSFORM |(!hasvc ? D3DPROCESSVERTICES_NOCOLOR : 0) , 0, cnt, buf.lpPointer);
			} else {
				PROCESSVERTICES_DATA(D3DPROCESSVERTICES_TRANSFORMLIGHT, 0, cnt, buf.lpPointer);
			}


		buf.PUT_ALIGN();

		// store a triangle list

		buf.PUT_OP_TRIANGLE_LIST(newNTri);
  
		fi=thisFi;					// face index

		D3DTRIANGLE currentTri; // the current triangle for strip optimization
		buf.SETUP_PREV(currentTri);

		while (fp<fpThisEnd) {
  			int flen = * fp ++; 	// length of this face
			if (flen<0) flen=-flen;   else fi++;
			switch (flen) {
			case 0:
			case 1:
			case 2:
				break;
			case 3:
				buf.PUT_TRIANGLE(vertMap[fp[0]],vertMap[fp[1]],vertMap[fp[2]],flip); // ,currentTri);
#ifdef _DEBUG
				if (currentTri.wFlags & D3DTRIFLAG_EVEN || currentTri.wFlags & D3DTRIFLAG_ODD) {
					//TRACE("strippable triangle found %d\n",fi);
					stripCnt++;
				}
#endif		   

				break;
/*
			case 4:
				buf.PUT_QUAD(vertMap[fp[0]],vertMap[fp[1]],vertMap[fp[2]],vertMap[fp[3]],flip);
				break;
*/
			default :
				{
					fmap.SetLength(flen);
					for (int vi=0; vi<flen; vi++) fmap[vi] = vertMap[fp[vi]];
					buf.PUT_POLYGON(flen,fmap.Data(),flip); // ,currentTri);
				}
				break;

			} // switch
			fp += flen;
          
		}
		buf.PUT_OP_EXIT();
		// Unlock and set 
		buf.Unlock();
    
		ASSERT(buf.Length()<=buf.MaxLength());
		// TRACE("Stored  Ex buf v=%d  len %d size=%d \n",newNv,buf.Length(),buf.MaxLength());

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

//	TRACE("Creating simple Ex buf v=%d tri=%d size=%d cw = %d flip = %d\n",nv,nTri,size,state.frontFaceCW,flipNormals);

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
			PROCESSVERTICES_DATA(D3DPROCESSVERTICES_TRANSFORM | (!hasvc ? D3DPROCESSVERTICES_NOCOLOR : 0)  , 0, nv, buf.lpPointer);
		} else {
			PROCESSVERTICES_DATA(D3DPROCESSVERTICES_TRANSFORMLIGHT, 0, nv, buf.lpPointer);
		}

    buf.PUT_ALIGN();

    // store a triangle list

    buf.PUT_OP_TRIANGLE_LIST(nTri);
	// strippable optimization turned off, because getting D3D error message using DEBUG DLL's 
  
    const int *fp =f;					// face list stepping pointers
    const int *fpend= fp+f.Length();   // points after last element of facelist
    int fi=-1;	// face index

	D3DTRIANGLE currentTri; // the current triangle for strip optimization
	buf.SETUP_PREV(currentTri);

    while (fp<fpend) {
  	   int flen = * fp ++; 	// length of this face
	   if (flen<0) flen=-flen;   else fi++;
       switch (flen) {
       case 0:
       case 1:
       case 2: // ignore 
           break;
       case 3:
           buf.PUT_TRIANGLE(fp[0],fp[1],fp[2],flip); // ;,currentTri);
#ifdef _DEBUG_X
		   if (currentTri.wFlags & D3DTRIFLAG_EVEN || currentTri.wFlags & D3DTRIFLAG_ODD) {
			   // TRACE("strippable triangle found %d\n",fi);
 			   stripCnt++;

		   }
#endif		   
           break;
/*
       case 4:
           buf.PUT_QUAD(fp[0],fp[1],fp[2],fp[3],flip);
           break;
*/
       default :
           buf.PUT_POLYGON(flen,fp,flip); // ,currentTri);
           // buf.PUT_POLYGON(flen,fp,flip,currentTri);
           break;

       }
       fp += flen;
           
    } // for all faces
    buf.PUT_OP_EXIT();
    // Unlock and set 
    buf.Unlock();
    
    ASSERT(buf.Length()<=buf.MaxLength());
//	TRACE("Stored  Ex buf v=%d tri=%d size=%d \n",nv,buf.Length(),buf.MaxLength());

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

#ifdef _DEBUG
	TRACE(" %d triangles of %d stripped\n",stripCnt,nTri);
#endif

    return(1);
}


/*
Create Direct 3D Execute Buffer, all fits in single buffer

  not unlit ? 
*/
int GShell::CreateExBuf(RenderState &state,G3ExBuf &buf)
{
    size_t size;
    int nv=GetVertexCnt(); 
    int nTri = f.GetTriangleCount();

    if (nv == 0 || nTri == 0) return(0);
	gbool flip = state.FlipFaces();
	gbool flipNormals = state.FlipNormals();


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
    LPD3DVERTEX vp = (LPD3DVERTEX) buf.lpPointer;
    int cnt= GetVertex(state,flipNormals,vp);
    buf.lpPointer = (void *) &vp[cnt];
    buf.offset = buf.Length();

    // store misc instructions

    OP_PROCESS_VERTICES(1, buf.lpPointer);
        PROCESSVERTICES_DATA(D3DPROCESSVERTICES_TRANSFORMLIGHT, 0, nv, buf.lpPointer);

	buf.PUT_ALIGN();

    // store a triangle list

    buf.PUT_OP_TRIANGLE_LIST(nTri);
  
    const int *fp =f;					// face list stepping pointers
    const int *fpend= fp+f.Length();   // points after last element of facelist
    int fi=-1;					// face index

    while (fp<fpend) {
  	   int flen = * fp ++; 	// length of this face
	   if (flen<0) flen=-flen;   else fi++;
       switch (flen) {
       case 0:
       case 1:
       case 2:
           break;
       case 3:
           buf.PUT_TRIANGLE(fp[0],fp[1],fp[2],flip);
           break;
/*
       case 4:
           buf.PUT_QUAD(fp[0],fp[1],fp[2],fp[3],flip);
           break;
*/
       default :
           buf.PUT_POLYGON(flen,fp,flip);
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

	objBuf.textureMatrixVersion = state.GetTextureMatrixVersion();
    return(1);
}

// Update vertex data in Execute Buffer
// vertex size must be identical !!!!!!!!!!!!!!!!
int GShell::UpdateExBufVerts(RenderState &state,G3ExBuf &buf,gbool unlit)
{
    
    if (!buf.Ok()) return CreateExBuf(state,buf);

    int nv=GetVertexCnt(); 

    if (nv == 0) return(1);
	gbool flipNormals= state.FlipNormals();

	D3DCOLOR color, specular;

	if (unlit) {
		//color  = RGBA_MAKE(255,255,255,255);
		color = state.getCurrentColorD3D();

		specular = RGBA_MAKE(0,0,0,255);
	}


    // Compute size of execute buffer
    // size = buf.VERTEX_SIZE(nv);

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

	} else {
		LPD3DVERTEX vp = (LPD3DVERTEX) buf.lpPointer;
	    cnt= GetVertex(state,flipNormals,vp);
		buf.lpPointer = (void *) &vp[cnt];
	}
    buf.offset = buf.Length();

    buf.lpPointer =  (LPVOID)(((char *)buf.lpBuffer) + buf.size) ;

    buf.Unlock();
    
    ASSERT(buf.Length()<=buf.MaxLength());

    if (!buf.Set()) {
        TRACE("%s:Could not set execute buffer \n",ClassName());
        return(0);
    }
    return(1);
}




//===========================================================================================
//		int GShell::D3dRender(GglRenderTraversal &traversalState, DWORD mode)
//===========================================================================================

// Render Shell using D3D, build execute buffer if needed state.glState->texturing
int GShell::D3dRender(GglRenderTraversal &traversalState, DWORD mode)
{   int ret = 0;
	
	RenderState &state = *traversalState.glState;



	//branch to software lighting x86/pIII
	if (state.doOwnLighting) {

		if (state.texturing && !state.TextureMatrixVersionOk(textureMatrixVersion)) {
			//TRACE("GShell::D3dRender textureMatrixChanged vers = %d\n",	state.GetTextureMatrixVersion());
			SetAction(SHELL_UPDATE_EXBUF_VERT);
		}
		/*
		but we could not see vertex lighting, therefor  state.Unlit()
		also too many state changes for HW renderer ? 
		// new 10.06.99 hg 
		if (Flat() && (vc.Length() == 0) && !state.fogEnabled &&  state.Unlit() ) { 
		  // need smooth if color per vertex
		  // if fog lighting on vertex can't use flast
			state.SetLightModel(GL_FLAT);

		}
		else 
		*/
		{
			if (state.faces.smooth)  // undo above FLAT changed !!!
				state.SetLightModel(GL_SMOOTH);
		}

	    // flush any pending state changes 
	    state.FlushState();

		ret = D3dRenderTL(traversalState);
        actions &= ~ (SHELL_COMPUTE_EXBUF|SHELL_UPDATE_EXBUF_VERT);
		return ret;
	}
	

	




	state.trisDrawn +=nfaces;
	state.primsDrawn +=1;

	// texture matrix changed, need new texture coordinates     
	if (state.texturing && !state.TextureMatrixVersionOk(objBuf.textureMatrixVersion)) {
		// TRACE("GShell::D3dRender textureMatrixChanged vers = %d\n",	state.GetTextureMatrixVersion());
		SetAction(SHELL_UPDATE_EXBUF_VERT);
	}

	if (state.useVertexBuffer) // branch to DX6 Vertex Buffer rendering 
	{
		return D3dRenderVB(traversalState);
	}


/*
	if (vc && dx6) {
		D3DCVERTEX ....
	}
*/

	// do we need to (re) create the D3D ExecuteBuffers ?
    if ( (actions & SHELL_COMPUTE_EXBUF) 
		|| !objBuf.Ok()							// Exbuf destroyed, eg. Window resized 
		// || objBuf.unlit != state.Unlit()		// enabled 09.06.98 if material unlit status changed rebuild exbuf
		// need to take into account Vcolors etc 
		)
    {

		// temp workaround until face color support in D3D
		if (fc.Length()>0 && state.faces.colored && state.canDoRgbLighting) 
			ComputeVertexColorsFromFaceColors();

		ret = CreateExBuf(state,objBuf);

        actions &= ~(SHELL_COMPUTE_EXBUF|SHELL_UPDATE_EXBUF_VERT);

        if (ret<=0) return(ret);

    }
    else if ((actions & SHELL_UPDATE_EXBUF_VERT)) { // only update vertices 
		
		if (objBuf.exBuf.Length() == 1 && (objBuf.exBuf[0]->Ok())) {
			// update vertices table in existing execute buffer
			ret = UpdateExBufVerts(state,*objBuf.exBuf[0],objBuf.unlit);
	    	objBuf.textureMatrixVersion = state.GetTextureMatrixVersion();
		} else ret = CreateExBuf(state,objBuf);	// too many verts, slow path 

        actions &= ~ (SHELL_UPDATE_EXBUF_VERT);
        if (ret<=0) return(ret);
    }
    // execute the buffer 
    if (objBuf.Ok()) {   

      if (Flat() && (vc.Length() == 0) && !state.fogEnabled) { 
		  // need smooth if color per vertex
		  // if fog lighting on vertex can't use flast
		state.SetLightModel(GL_FLAT);

	  }
	  else {
	    if (state.faces.smooth)  // undo above FLAT changed !!!
			state.SetLightModel(GL_SMOOTH);
	  }

	  // flush any pending state changes 
	  state.FlushState();

	  // default D3DEXECUTE_CLIPPED
/*
	  if (!traversalState.doClipping )  // new 20.04.99
		  mode = D3DEXECUTE_UNCLIPPED;
*/

	  TRY {
		ret = objBuf.Execute(state.lpDev,state.lpView, mode);
	  }

	  CATCH_ALL(e)
	  {
		TRACE("GShell::D3dRender Fatal exception \n");
		ASSERT(0);
		
	  }
	  END_CATCH_ALL
   
    }
    return(ret);
}


#endif



// GClass protocol



// do nothing 
int GShell::Do(GTraversal &state)
{
  return(0);	
}



// do glRendering
int GShell::Do(GglRenderTraversal &state)   
{ 

	Update();

	if (subMeshes) {
		// e = edit_flags;
/* to do 
		if (state.texturing && !state.TextureMatrixVersionOk(textureMatrixVersion)) {
			TRACE("GShell::D3dRender textureMatrixChanged vers = %d\n",	state.GetTextureMatrixVersion());
			SetAction(SHELL_UPDATE_EXBUF_VERT);
		}
*/


		if (actions & SHELL_COMPUTE_EXBUF) { 
			actions &= ~ (SHELL_COMPUTE_EXBUF|SHELL_UPDATE_EXBUF_VERT);	
			CreateSubMeshes();

		}
		else if (actions & SHELL_UPDATE_EXBUF_VERT) {
			actions &= ~ (SHELL_UPDATE_EXBUF_VERT);	
			UpdateSubMeshVertices();
		}
		edit_flags=0;

		GSubMesh *m=subMeshes;

		int ret = 0;
		while (m) {
			ret=m->s->Do(state);
			m = m->next;
		}
		return ret;
	}

	edit_flags=0;

#ifdef _GSTATS
	primsProcessed++;
#endif

#ifdef _OGL
	state.glState->primsDrawn++;

	state.glState->trisDrawn +=nfaces; // for now 

	return (glRender(*state.glState)); 
#else
#ifdef _D3D
	return (D3dRender(state));
#else
    return(0);
#endif

#endif

}; 

// do boundingbox 
int GShell::Do(GBBoxTraversal &state)       
{ 
	BBox myBox;

	if (v.Length()<=0) return (0); 
 			
	ComputeBBox(myBox);
	state.Union(myBox);
	return(1);
}; 
	


//
// GMesh
//
RTIMP(GMesh,0,GShell);

// Copy or assign
GShell * GMesh::CopyGeometry(GShell *dest) const
{  GMesh *g;

   if (!dest) { 		// allocate new
		g=new GMesh(ncols,nrows,v);
		return(g);
   }
   else {				// copy
		 g = (GMesh *) dest;	// may not be valid
		 g->Set(ncols,nrows,v);
		 // to do: if shell, create facelist
		 return(g);
   }
}

int GMesh::CountFaces()	
{ 
	// count the real face list 
	if (f.Length() >0) return GShell::CountFaces();

	return((ncols-1) * (nrows-1)); 

}

// compute a GShell compatible face list for the mesh
void GMesh::ComputeFaceList(gbool quads)
{

   f.SetGrid(nrows,ncols,quads); // ,int closeCols,int closeRows)
   GetFaceCount();
   if (fn.Length() != nfaces) fn.Flush();

}

// get a facelist in shell format
int GMesh::GetFaceList(GFaceList &flist)
{
   flist.SetGrid(nrows,ncols,gtrue); // ,closeCols,closeRows)
   return (flist.Length());
}

int GMesh::GetFaceCount()
{
  return(nfaces=CountFaces());
}

// compute triangle list for mesh
int GMesh::ComputeTriangles(GFaceList &tri,IntArray &tri2face,int useTriangulator)
{
   tri.SetGrid(nrows,ncols,0); // ,int closeCols,int closeRows);
   int tris = tri.Length() / 4;
   tri2face.SetLength(tris);
   for (int i=0; i < tris; i++)
	  tri2face[i]= i / 2;
   return(tris);
}

#define GETFN(COL,ROW) fn[ROW*(ncols-1) +COL]

// compute face normals for each quad and store them into fn
void
GMesh::ComputeFaceNormals(int allocFaceinfo)
{


   if (f.Length() >0) {
	   // compute true normals from faceslist 
	   GShell::ComputeFaceNormals(allocFaceinfo);
	   return;
   }		

   Plane plane;

   if (nfaces <=0) {
		   nfaces= GetFaceCount();
   }

   UnSetAction(SHELL_COMPUTE_FNORMALS);

   if (nfaces <=0) return;

   if (fn.Length() == 0) fn.SetLength(nfaces);
   
   fn.UnRef();

   if (allocFaceinfo && (finfo == NULL))
		  finfo = new shell_face_info [nfaces];

   int f[4];
   int flen=4;
   int fi;		// Face number
   int i1;		// vertex ptr current row
   int i2;		// vertex ptr nex row
   i1=0;
   i2=i1+ncols;
   fi=0;
   for (int rows = nrows-1; rows>0; rows --) {

		 f[0]=i2;
		 f[1]=i1;
		 i1++;i2++;
		 for (int cols = ncols-1; cols>0; cols --) {
			f[2]=i1;
			f[3]=i2;

			compute_polygon_plane_index(flen,f,v,&plane);

			if (fn.Length()) {
					//plane.n = -plane.n; // hg wrap normal
					fn[fi]=plane.n;
					//GETFN(cols,rows) = plane.n;
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

			f[0]=i2;
			f[1]=i1;


			i1++;i2++;
			fi++;
		 }
   }
   SetFlag(SHELL_FNORMALS_COMPUTED);
   if (NormalsFlipped()) fn *= Point(-1.0,-1.0,-1.0);  // flip normals

}

int
GMesh::ComputeVertexNormals()
{
   if (nfaces <=0) {
 	    nfaces= GetFaceCount();
   }

   UnSetAction(SHELL_COMPUTE_VNORMALS);

   if (nfaces <=0) return(-1);

   // need face normals for computation
   if (fn.Length() == 0) ComputeFaceNormals();
   if (fn.Length() == 0) return(-1);

   if (vn.Length() == 0) vn.SetLength(v.Length());
   if (vn.Length() == 0) return(-1);
   vn.UnRef();

   int f0=0;
   int f1=0;
   int vi=0;

#if 1
   for(int rows=nrows-1; rows >=0; rows --) {
		  int frowoffset = (ncols-1);	// only ncols-1 face normals !
		  // first + last row, use only this
		  if ((rows == (nrows-1))) 
          {
              frowoffset=0;
              f0=0;

          } else
          if (rows == 0) {
              frowoffset=0;
          } 
          else {
    		  if ((rows == (nrows-2))) 
              {
                f0=0;
              } 

              frowoffset = (ncols-1);	// only ncols-1 face normals !
          }  

		  for(int cols=ncols-1; cols >=0; cols --) {
		  Vector n;
		  if (cols == (ncols-1)) f1 = f0;
		  else if (cols == 0) f0 = f1;
		  else f1=f0+1;
		  
		 
		 // GETFN(cols,rows) = plane.n;


		  n = fn[f0];
		  n += fn[f1];
		  n += fn[f0+frowoffset];
		  n += fn[f1+frowoffset];
		  n.Normalize();
		  vn[vi]=n;
		  vi++;

		  f0=f1;
//		  f1 ++;
		  }
		  f0++;
   }
#endif
   SetFlag(SHELL_VNORMALS_COMPUTED);
   return(0);
}

void GMesh::SetVertexParametersNative(float umin,float umax,
									  float vmin,float vmax)
{

  vp.SetLength(v.Length());
  vp.UnRef();

  float u,du = (umax-umin) / (float) (ncols-1);
  float v,dv = (vmax-vmin) / (float) (nrows-1);


  int i=0;
  int row,col;
  for (row = nrows-1,v=vmax; row >=0; row --,v-=dv)
  for (col = ncols-1,u=umin; col >=0; col --,u+=du) {
		 vp[i].Set(u,v);
		 i++;
  }
  SetEditFlag(SHELL_VERTEX_ATTRIBUTES_CHANGED);
}

// compute native vertex parameters for object
void GMesh::GetVertexParametersNative(int n,Point *vp)
{
  float umin=0.0, umax=1.0,vmin=0.0,vmax=1.0;


  float u,du = (umax-umin) / (float) (ncols-1);
  float v,dv = (vmax-vmin) / (float) (nrows-1);

  //vp.Resize(nv);

  int i=0;
  int row,col;
//  for (row = nrows-1,v=vmax; row >=0; row --,v-=dv)
  for (row = nrows-1,v=vmin; row >=0; row --,v+=dv)
  for (col = ncols-1,u=umin; col >=0; col --,u+=du) {
		 vp[i].Set(u,v);
		 i++;
		 if (i>=n) break;
  }
}

/*  Evaluate object as object of type "type"
  if return = this, don´t destroy result, otherwise destroy */
//GvNode * /*GShell**/ 

GEvalAs_TYPE*
GMesh::EvalAs(TypeId type,int copyAttributes)
{
  if (type == Id()) return(this); // already indentical
#if 0
  if (IsA(type)) {
	  // is of derived types
	  return(this); // or copy ??
  }
#endif
  if (type == GShell::ClassId()) {  // convert to shell
	 GShell *s = new GShell();
	 s->f.SetGrid(nrows,ncols,1); // ,int closeCols,int closeRows)
	 s->SetV(v);
	 if (copyAttributes) {
		 if (vn.Length() > 0) s->SetVN(vn);
		 if (vc.Length() > 0) s->SetVC(vc);
		 if (vp.Length() > 0) s->SetVP(vp);
		 if (fn.Length() > 0) s->SetFN(fn);
		 if (fc.Length() > 0) s->SetFC(fc);

	 }
	 return(s);
  }

#if 0  
  if (type == GShellI::ClassId()) {
	 GShellI *s = new GShellI();
	 ComputeShell(s,copyAttributes);
	 return(s);
  }
#endif
  // else cannot convert
  return(NULL);
}


#ifdef _OGL
void GMesh::RenderGlEdges(RenderState &state,int VN,int VC,int VP)
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


   // horizontal
   int i=0;
   for (int rows = nrows; rows>0; rows --) {

		 glBegin(GL_LINE_STRIP);
		 for (int cols = ncols; cols>0; cols --) {
				 OUTVERT(i);
				 i++;
		  }
		  glEnd();
   }
   // vertical
}


void GMesh::RenderGlFaces(RenderState &state,int FN, int VN, int VC, int VP)
{
   int lightModel = state.lightmodel;
   gbool flipNormals = state.frontFaceCW && !state.twosidedLighting;


   // need face normals for computation
   if (FN) {
  		 const PointArray &n=GetFN();
		 if (n.Length() == 0) FN=0; /* return(-1); */
   }

   if (VN)  {
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

   if (!VC  && !VN)
	   state.SetLightModel(GL_FLAT);


   if (VP) if (vp.Length() == 0)		VP =0;

   int fi;		// Face number
   int i1;		// vertex ptr current row
   int i2;		// vertex ptr nex row
   i1=0;
   i2=i1+ncols;
   fi=0;
   for (int rows = nrows-1; rows>0; rows --) {

		 glBegin(GL_QUAD_STRIP);
		 OUTVERT(i2);
		 OUTVERT(i1);
		 i1++;i2++;
		 for (int cols = ncols-1; cols>0; cols --) {
				 OUTVERT(i2);
				 if (FC) glColor(fc[fi]);
				 if (FN) if (flipNormals) glNormalFlip(fn[fi]); else glNormal(fn[fi]);
				 OUTVERT(i1);
				 i1++;i2++;
				 fi++;
		 }
		 glEnd();
   }
   state.SetLightModel(lightModel);
};
#endif

#ifdef _D3D



/*
Create Direct 3D Execute Buffer
*/
int GMesh::CreateExBuf(RenderState &state,G3ObjRef &objBuf)
{
    int nv=GetVertexCnt(); 
	int bufi=0;

	//objBuf.Truncate(0);

	gbool flip = state.FlipFaces();
	gbool flipNormals =  state.FlipNormals();

	gbool bothSides = state.NeedBothFaces();

	gbool unlit = (state.canDoRgbLighting && state.faces.colored && (vc.Length()>0)) 
		|| state.Unlit(); 



    if ((nrows<=1) || (ncols <=1) || nv == 0) return(0);

	gbool tooManyVerts = 0;
	if ((state.maxVertexCount>0) && (nv > state.maxVertexCount)) 
		tooManyVerts = 1;

	if (tooManyVerts || bothSides) {

		int rowsPerBuf  = state.maxVertexCount / ncols;
		
		ASSERT(rowsPerBuf>= 2);
		if (rowsPerBuf<2) {
		   TRACE("mesh to big %d %d \n", ncols,nrows);
		   Reporter.Warning("Direct 3D limitation :mesh (%d  x %d ) max col size :\n", ncols,nrows,state.maxVertexCount / 2);
		   return(0);
		} else {
			//rowsPerBuf--;
		}

		// slice mesh in separate buffers 
		for (int row = 0; row<nrows; row+= (rowsPerBuf-1)) {
			   if (objBuf.exBuf.Length() <= bufi) {
					objBuf.Append(state.CreateExBuf());
			   }
			   G3ExBufHandle *buf= objBuf.exBuf[bufi];
			   bufi++;

			   // TRACE("Creating mesh buffer par %d %d \n", row, min(nrows-1,row+rowsPerBuf-1));
			   int ret = CreateExBuf(state,flip, flipNormals,*buf, row, min(nrows-1,row+rowsPerBuf-1),0,ncols-1);
			   if (ret<=0) return ret;
		}
		
		if (bothSides) {
		// slice mesh in separate buffers 
		for (int row = 0; row<nrows; row+= (rowsPerBuf-1)) {
			   if (objBuf.exBuf.Length() <= bufi) {
					objBuf.Append(state.CreateExBuf());
			   }
			   G3ExBufHandle *buf= objBuf.exBuf[bufi];
			   bufi++;

			   // TRACE("Creating mesh buffer par %d %d \n", row, min(nrows-1,row+rowsPerBuf-1));
			   int ret = CreateExBuf(state,!flip, !flipNormals,*buf,row, min(nrows-1,row+rowsPerBuf-1),0,ncols-1);
			   if (ret<=0) return ret;
		}


		}
		
		if (objBuf.exBuf.Length() > bufi) {
			objBuf.Truncate(bufi);
		}

    	objBuf.textureMatrixVersion = state.GetTextureMatrixVersion();
		return(1);

	}
	else {
		if (objBuf.exBuf.Length() == 0) {
			objBuf.Append(state.CreateExBuf());
		}
		G3ExBuf &buf = *objBuf.exBuf[bufi];
		bufi++;

		int ret = CreateExBuf(state,buf);

		if (objBuf.exBuf.Length() > bufi) {
			objBuf.Truncate(bufi);
		}

    	objBuf.textureMatrixVersion = state.GetTextureMatrixVersion();
		objBuf.unlit = unlit;

		return (ret);
	}
}

/*
Create Direct 3D Execute Buffer
*/
int GMesh::CreateExBuf(RenderState &state,G3ExBuf &buf)
{
    size_t size;
    int nv=GetVertexCnt(); 
    int nTri = ((ncols-1) * (nrows-1)) /* CountFaces()*/	*2;

    if (nv == 0 || nTri == 0) return(0);

	gbool flip = state.FlipFaces();
	gbool flipNormals =  state.FlipNormals();


    // Compute size of execute buffer
    size = buf.VERTEX_SIZE(nv);
    size += buf.TRIANGLE_LIST_SIZE(nTri)+30*buf.OP_SIZE();

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
	gbool unlit= (state.canDoRgbLighting && state.faces.colored && (vc.Length()>0)) || state.Unlit(); 

	if (unlit) {
		D3DCOLOR color, specular;
		//color  = RGBA_MAKE(255,255,255,255);
		color = state.getCurrentColorD3D();

		specular = RGBA_MAKE(0,0,0,255);
		

		LPD3DLVERTEX vp = (LPD3DLVERTEX) buf.lpPointer;
		cnt = GShell::GetVertex(state,color, specular,vp);
		buf.lpPointer = (void *) &vp[cnt];

	}
	else {
		LPD3DVERTEX vp = (LPD3DVERTEX) buf.lpPointer;
		cnt= GShell::GetVertex(state,flipNormals,vp);
		buf.lpPointer = (void *) &vp[cnt];
	}

    buf.offset = buf.Length();

    // store misc instructions

    OP_PROCESS_VERTICES(1, buf.lpPointer);
		if (unlit) {
			PROCESSVERTICES_DATA(D3DPROCESSVERTICES_TRANSFORM, 0, nv, buf.lpPointer);
		} else {
			PROCESSVERTICES_DATA(D3DPROCESSVERTICES_TRANSFORMLIGHT, 0, nv, buf.lpPointer);
		}

    buf.PUT_ALIGN();

    // store a triangle list

    buf.PUT_OP_TRIANGLE_LIST(nTri);

   int fi;		// Face number
   int i1;		// vertex ptr current row
   int i2;		// vertex ptr nex row
   i1=0;
   i2=i1+ncols;
   fi=0;
   // to do: otimize edge visibility, build tri strip 
   gbool bottomEdge = TRUE;

   for (int rows = nrows-1; rows>0; rows --) {
		 i1++;i2++;
		 gbool startStrip = TRUE;
		 for (int cols = ncols-1; cols>0; cols --) {
				 //buf.PUT_TRIANGLE(i2-1,i1-1,i1,flip);
				 //buf.PUT_TRIANGLE(i2-1,i1,i2,flip);
                 //buf.PUT_QUAD(i2-1,i1-1,i1,i2,flip);
                 buf.PUT_QUAD_STRIP(startStrip,i2-1,i1-1,i1,i2,FALSE,bottomEdge);
				 i1++;i2++;
				 fi++;
				 startStrip = FALSE;
		 }
		 bottomEdge = FALSE;
   }
 
    buf.PUT_OP_EXIT();
    // Unlock and set 
    buf.Unlock();

	ASSERT( (fi*2) == (nTri));
    
    ASSERT(buf.Length()<=buf.MaxLength());

    if (!buf.Set()) {
        TRACE("%s:Could not set execute buffer of size &d (verts=%d tris=%d)\n",ClassName(),size,nv,nTri);
        return(0);
    }

    return(1);
}

/*
Create Direct 3D Execute Buffer for sub mesh
*/
int GMesh::CreateExBuf(RenderState &state,gbool flip, gbool flipNormals,G3ExBuf &buf,int row0, int row1, int col0,int col1)
{
	gbool unlit= (state.canDoRgbLighting && state.faces.colored && (vc.Length()>0)) || state.Unlit(); 

    size_t size;

	int nrows = row1-row0+1;
	int ncols = col1-col0+1;

    int nv= nrows*ncols;
    int nTri = (nrows-1) * (ncols -1) *2;

    if (nv == 0 || nTri == 0) return(0);

	if ((state.maxVertexCount>0)) {

		ASSERT(nv <= state.maxVertexCount);

	}


    // Compute size of execute buffer
    size = buf.VERTEX_SIZE(nv);
    size += buf.TRIANGLE_LIST_SIZE(nTri)+30*buf.OP_SIZE();

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
		D3DCOLOR color, specular;
		
		//color  = RGBA_MAKE(255,255,255,255);
		color = state.getCurrentColorD3D();

		specular = RGBA_MAKE(0,0,0,255);


		LPD3DLVERTEX vp = (LPD3DLVERTEX) buf.lpPointer;
		cnt= GetVertex(state,color,specular,vp,row0,row1,col0,col1);
		buf.lpPointer = (void *) &vp[cnt];
	}
	else {
		LPD3DVERTEX vp = (LPD3DVERTEX) buf.lpPointer;
	    cnt= GetVertex(state,flipNormals,vp,row0,row1,col0,col1);
		buf.lpPointer = (void *) &vp[cnt];
	}

    buf.offset = buf.Length();

	ASSERT(cnt == nv);

    // store misc instructions

    OP_PROCESS_VERTICES(1, buf.lpPointer);
		if (unlit) {
			PROCESSVERTICES_DATA(D3DPROCESSVERTICES_TRANSFORM, 0, nv, buf.lpPointer);
		} else {
			PROCESSVERTICES_DATA(D3DPROCESSVERTICES_TRANSFORMLIGHT, 0, nv, buf.lpPointer);
		}

    buf.PUT_ALIGN();

    // store a triangle list

    buf.PUT_OP_TRIANGLE_LIST(nTri);

   int fi;		// Face number
   int i1;		// vertex ptr current row
   int i2;		// vertex ptr nex row
   i1=0;
   i2=i1+ncols;
   fi=0;
   // to do: otimize edge visibility, build tri strip 
   gbool bottomEdge = TRUE;
   for (int rows = nrows-1; rows>0; rows --) {
		 i1++;i2++;
		 gbool startStrip = TRUE;
    
		 for (int cols = ncols-1; cols>0; cols --) {
				// buf.PUT_TRIANGLE(i2-1,i1-1,i1,flip);
				// buf.PUT_TRIANGLE(i2-1,i1,i2,flip);
                //buf.PUT_QUAD(i1-1,i2-1,i2,i1,flip);
				buf.PUT_QUAD_STRIP(startStrip,i2-1,i1-1,i1,i2,flip,bottomEdge);
				startStrip = FALSE;
				i1++;i2++;
				fi++;
		 }
	     bottomEdge = FALSE;
   }
   ASSERT( (fi*2) == nTri);
 
    buf.PUT_OP_EXIT();
    // Unlock and set 
    buf.Unlock();
    
    ASSERT(buf.Length()<=buf.MaxLength());

    if (!buf.Set()) {
        TRACE("%s:Could not set execute buffer of size &d (verts=%d tris=%d)\n",ClassName(),size,nv,nTri);
        return(0);
    }

    return(1);
}


// Get vertex in D3DVertex format 

int GMesh::GetVertex(RenderState &state,int flipNormals,D3DVERTEX *V,int row0, int row1, int col0,int col1)
{

	if ((vn.Length() == 0) || (actions & SHELL_COMPUTE_VNORMALS)) {
		ComputeVertexNormals();
	}

    int nv = 0;
    D3DVERTEX *pV = V;

    const Point *pv = v.Data();
    const Point *pn = vn.Data();
    const Point *pp = vp.Data();
    if (!pp) pp = pv; // use vertex coordinates

	int offset = row0*ncols + col0;
	pv+=offset;
	pn+=offset;
	pp+=offset;

	// offset after col row to position to next row
	offset = (ncols)-  (col1-col0+1);

    for(int r=row0; r <= row1; r++) {
    for(int c=col0; c <= col1; c++) {
        pV->x = pv->x;
        pV->y = pv->y;
        pV->z = pv->z;
		if (flipNormals) 
		{ pV->nx = -pn->x; pV->ny = -pn->y; pV->nz = -pn->z; }
		else 
		{ pV->nx = pn->x; pV->ny = pn->y; pV->nz = pn->z; }
		if (!state.textureMatrixIdentity) {
			Point pt= state.textureMatrix * *pp;
			pV->tu = pt.x;  pV->tv = pt.y; 
		} else 
		{ pV->tu = pp->x;  pV->tv = pp->y; }
        pV++; 
        pv++;
        pn++;
        pp++;
		nv++;	 
	}
	pv+=offset;
	pn+=offset;
	pp+=offset;

	}
 return(nv);
}

int GMesh::GetVertex(RenderState &state,D3DCOLOR color, D3DCOLOR specular,D3DLVERTEX *V,int row0, int row1, int col0,int col1)
{

    int nv = 0;
    D3DLVERTEX *pV = V;

    const Point *pv = v.Data();
    const Point *pc = vc.Data();
    const Point *pp = vp.Data();
    if (!pp) pp = pv; // use vertex coordinates

	int offset = row0*ncols + col0;
	pv+=offset;
	if (pc) pc+=offset;
	pp+=offset;

	// offset after col row to position to next row
	offset = (ncols)-  (col1-col0+1);

    for(int r=row0; r <= row1; r++) {
    for(int c=col0; c <= col1; c++) {
        pV->x = pv->x;
        pV->y = pv->y;
        pV->z = pv->z;
		pV->dwReserved = 0;

		if (!state.textureMatrixIdentity) {
			Point pt= state.textureMatrix * *pp;
			pV->tu = pt.x;  pV->tv = pt.y; 
		} else 
		{ pV->tu = pp->x;  pV->tv = pp->y; }
		
		if (pc) {
			pV->color = D3DRGB(pc->x,pc->y, pc->z);	 // to do: a * clamp !
			pc++;
		}
		else pV->color = color;
		pV->specular = specular;

        pV++; 
        pv++;
        pp++;
		nv++;	 
	}
	pv+=offset;
	if (pc) pc+=offset;
	pp+=offset;

	}
 return(nv);
}


int GMesh::D3dRenderTL(GglRenderTraversal &traversalState)
{
	TRACE("GMesh::D3dRenderTL  \n");

	if (( f.Length() == 0) && (ncols >0) ) {
		ComputeFaceList();
	}
	return GShell::D3dRenderTL(traversalState);
}



#endif

#ifdef _3DV

int GMesh::io(AStream &a)
{

  if (!a.tok("mesh")) return(0);

  a.BeginBlock();

  if (a.isOutput()) {
		a.BeginTok();
		  if (a.tok("rows")) a.io(nrows);
		  if (a.tok("cols")) a.io(ncols);
		  f.Flush(); // don`t write facelist
		  ioOutputAttributes(a);		// output shell attributes

		a.EndTok();
  } else {
		  if (a.BeginTok())
		  do {
		  if (a.tok("rows")) a.io(nrows);
		  else if (a.tok("cols"))  a.io(ncols);
		  else if (ioInputOneAttribute(a)) {
		  } else {
				TRACE("Mesh::IO Token %s ??? \n",a.curtok);
		  }

		  } while (a.EndTok());

		  UnSetEditFlag(SHELL_VERTICES_CHANGED | SHELL_FACELIST_CHANGED);

		  if (v.Length() != (nrows * ncols )) {
				TRACE("Mesh:: Vertexlist length of %d doesnt match cols = %d * rows = %d (`%d )\n",v.Length(), nrows ,ncols,nrows * ncols);
				nrows = v.Length() / ncols;
		  }
  }

  a.EndBlock();

  return(a.stat);

}
#endif _3DV



// Compute primitive list for shell, build tristrips for better gl performance


int GShell::ComputePrimitiveList(int flags)
{
  int nout=0;
  int *out = NULL;
//	compute_facelist_tristrips(nf,f, &nout, &out, - SHELL_TRIANGLE_STRIP, - SHELL_END);
  compute_facelist_tristrips(f.Length(),f.Data(), &nout, &out, - SHELL_TRISTRIP_FACEINDEX, - SHELL_END,1);
  SetPrimitiveList(nout, out);

  FREE(out);
  UnSetAction(SHELL_COMPUTE_PRIMITIVELIST);
  return(0);
}


/***************************************************************************/
/* insert 3d box,
   startPoint is at (atx,aty,atz)
   width is (dx,dy,dz)
*/

static int cube_connectivity[] = {
		4,0,2,3,1,
		4,1,3,7,5,
		4,5,7,6,4,
		4,6,2,0,4,
		4,2,6,7,3,
		4,0,1,5,4 };

static int cube_flags = SHELL_CONVEX|SHELL_CLOSED|SHELL_SHADE_FLAT;


GShell * new_box3d(GShell *cube,float atx, float aty,float atz,
						float dx,float dy,float dz)
{
   Point _p[8];
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
		  cube = new GShell;

   cube->Make(8,_p,ELEMENTS(cube_connectivity),cube_connectivity);
   cube->SetFlag(cube_flags);

   return (cube);
}

/***************************************************************************/

/***************************************************************************/
/*
   Icosahedron
   from Open Gl programming guide, pg. 56
*/
#define X .525731112119133606
#define Z .850650808352039932
//static Point icosahedron_pts[12]= { {-X,0.0,Z}, {X,0.0,Z},{-X,0.0,-Z}, {X,0.0,-Z},
//						  {0.0,Z,X}, {0.0,Z,-X},{0.0,-Z,X}, {0.0,-Z,-X},
//						  {Z,X,0.0}, {-Z,X,0.0},{Z,-X,0.0}, {-Z,-X,0.0} };

static point_ icosahedron_pts[12]= {
						  {-X,0.0,Z}, {X,0.0,Z},{-X,0.0,-Z}, {X,0.0,-Z},
						  {0.0,Z,X}, {0.0,Z,-X},{0.0,-Z,X}, {0.0,-Z,-X},
						  {Z,X,0.0}, {-Z,X,0.0},{Z,-X,0.0}, {-Z,-X,0.0}
						  };
static int icosahedron_connectivity[] = {
		3,0,4,1,  3,0,9,4,		3,9,5,4,  3,4,5,8,		3,4,8,1,
		3,8,10,1, 3,8,3,10, 3,5,3,8,  3,5,2,3,	3,2,7,3,
		3,7,10,3, 3,7,6,10, 3,7,11,6, 3,11,0,6, 3,0,1,6,
		3,6,1,10, 3,9,0,11, 3,9,11,2, 3,9,2,5,	3,7,2,11
		};

static int icosahedron_flags = SHELL_CONVEX|SHELL_CLOSED|SHELL_TRIANGULATED;


GShell * new_icosahedron(GShell *icosahedron,int doComputeNormals)
{  int a,b;
   if (!icosahedron)
		  icosahedron = new GShell;

   a = sizeof(point_);
   b = sizeof(Point);

   icosahedron->Make(12,(Point*) icosahedron_pts,ELEMENTS(icosahedron_connectivity),icosahedron_connectivity);
   icosahedron->flags=icosahedron_flags;
   if (doComputeNormals) {
		   icosahedron->CountFaces();
		   icosahedron->ComputeFaceNormals(1);
		   icosahedron->SetVN(12,(Point*) icosahedron_pts);
   }
   icosahedron->edit_flags=0;
   return (icosahedron);
}



//
// GPolygon
//
RTIMP(GPolygon,0,GShell);

GPolygon::GPolygon(int np, const Point *p) : GShell(np,p,0,0) {
				GFaceList fl(np+1);
				fl.SetPolygon(np);
				SetFaceList(fl);
};

//
// GPolyline
// update GPolylineI in GShellI.cpp on any changes !
//

RTIMP(GPolyline,0,GShell);

GPolyline::GPolyline(int np, const Point *p) : GShell(np,p,0,0) 
{
	GFaceList fl(np+1);
	fl.SetPolygon(np);
	SetFaceList(fl);
};




GPolyline::GPolyline(int new_nv,const Point *new_v,int new_nf,const int *new_f) :
					GShell(new_nv,new_v,new_nf,new_f)
{
}

// get a facelist in shell format
int GPolyline::GetFaceList(GFaceList &flist)
{
   flist.SetPolyline(f);
   return (flist.Length());
}



#if 0
void GPolyline::RenderGlEdges(RenderState &state,int VN,int VC,int VP)
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
void GPolyline::RenderGlFaces(RenderState &state,int FN, int VN, int VC, int VP)
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


int GPolyline::glRender(RenderState &state)
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
int GPolyline::CreateExBuf(RenderState &state,G3ObjRef &objBuf)
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
		//color = D3DCOLOR(1,1,1);
		//color  = RGBA_MAKE(255,255,255,255);
		color = state.getCurrentColorD3D();
		specular = RGBA_MAKE(0,0,0,255);
	}



	int tooManyVerts = 0;

	if ((state.maxVertexCount>0) && (nv > state.maxVertexCount)) 
		tooManyVerts = 1;

	// tooManyVerts = 1; //debug


	int bufi=0;	// the current execute buffer created 


	while(1) {  // two passes if bothSides 
	
	if (tooManyVerts) {
		IntArray vertMap(nv);	// table for mapping used vertices 
		IntArray fmap;		// termporay to remap a face 
		int i;

		// TRACE("to many verts %d \n",nv);
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

		// store a triangle list

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
		// TRACE("Stored  Ex buf v=%d  len %d size=%d \n",newNv,buf.Length(),buf.MaxLength());

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

	// TRACE("Creating Ex buf v=%d tri=%d size=%d \n",nv,nTri,size);

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
	// TRACE("Stored  Ex buf v=%d tri=%d size=%d \n",nv,buf.Length(),buf.MaxLength());

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



int GPolyline::D3dRenderTL(GglRenderTraversal &traversalState)
{
	RenderState &state = *traversalState.glState;

	int nv= v.Length();

	if (nv == 0 || (f.Length()<3)) 
		return -1;

	DWORD primMask;
	if (traversalState.doClipping)
		primMask =  D3DDP_DONOTUPDATEEXTENTS; // | D3DDP_DONOTLIGHT 
	else  
		primMask = D3DDP_DONOTCLIP |  D3DDP_DONOTUPDATEEXTENTS;

	// DTLVERTEX = 8 floats x y z w color spec tu tv 
	vtl.SetLength( (sizeof(D3DTLVERTEX) / sizeof(float)) * ((nv+3) & ~3)); // katmai uses at least multiple of 4 
	D3DTLVERTEX *vtlp=   ( D3DTLVERTEX*) vtl.Data();	

	TransformLightVertices(traversalState, vtlp);

	// vertices
	if (state.vertices.visible) 
	{
		if (state.dx6) state.GLastError = state.lpD3DDevice3->DrawPrimitive(D3DPT_POINTLIST, D3DFVF_TLVERTEX,
			(LPVOID) vtl.Data(), nv, primMask);
		else state.GLastError = state.lpD3DDevice2->DrawPrimitive(D3DPT_POINTLIST,D3DVT_TLVERTEX,(LPVOID) vtl.Data(), nv, primMask);
		if (state.GLastError != D3D_OK) { D3DError("Error DrawPrimitive",state.GLastError); }

	}
	else { // edges
	// Index buffer 
	Array<WORD> &prims=this->primIndex;

	// to do :
	if (nv>D3D_maxVerts) {
		return -1;
	}
	else {
		if ( (prims.Length() > 0) && ~ (actions & SHELL_COMPUTE_EXBUF)) {
			// index buffer is ok
		}
		else 
		{
			nPrim = BuildLineList(f,gfalse,prims);
		}

		if (nPrim<=0) return -1; // if all culled 

		if (state.dx6) state.GLastError = state.lpD3DDevice3->DrawIndexedPrimitive(D3DPT_LINELIST, D3DFVF_TLVERTEX,
			(LPVOID) vtl.Data(),nv, (LPWORD) prims.Data(), nPrim, primMask);
		else state.GLastError = state.lpD3DDevice2->DrawIndexedPrimitive(D3DPT_LINELIST,D3DVT_TLVERTEX,
			(LPVOID) vtl.Data(),nv, (LPWORD) prims.Data(), nPrim, primMask);
		if (state.GLastError != D3D_OK) { D3DError("Error DrawIndexedPrimitive",state.GLastError); }
	}
	}
	return(0);

}

#endif




//
// GPoints
//
RTIMP(GPoints,0,GShell);

GPoints::GPoints(int np, const Point *p) : GShell(np,p,0,0) 
{
};


// get a facelist in shell format
int GPoints::GetFaceList(GFaceList &flist)
{
   flist.SetPoints(v.Length());
   return (flist.Length());
}








#ifdef _OGL 
void GPoints::RenderGlFaces(RenderState &state,int FN, int VN, int VC, int VP)
{
   int lightModel = state.lightmodel;

   if (VC) {
       if (vc.Length() == 0) {
           VC =0;
       } else {
	       state.SetLightModel(GL_SMOOTH); // need smooth lighting to see vertex colors
       } 
   }    

   GShell::RenderGlVertices(state,VN,VC);

   state.SetLightModel(lightModel);

}


int GPoints::glRender(RenderState &state)
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
					if (state.SetVertexAttributes()) {

						int doVn = state.vertices.smooth && !Flat();	
						RenderGlFaces(state,!doVn,
										  doVn,
										  state.vertices.colored,
										  state.vertices.textured);
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
int GPoints::CreateExBuf(RenderState &state,G3ObjRef &objBuf)
{
    
	size_t size;
    int nv=GetVertexCnt(); 

    if (nv == 0 ) return(0);

	gbool flip = state.FlipFaces();
	gbool flipNormals =  state.FlipNormals();
	gbool bothSides = FALSE; // state.NeedBothFaces();
	gbool unlit= (state.canDoRgbLighting && state.faces.colored && (vc.Length()>0)) 
		|| state.Unlit() || vn.Length() == 0; 

	gbool hasvc= (vc.Length()>0);
	D3DCOLOR color, specular;
	
	if (unlit) {
		//color = D3DCOLOR(1,1,1);
		//color  = RGBA_MAKE(255,255,255,255);
		color = state.getCurrentColorD3D();
		specular = RGBA_MAKE(0,0,0,255);
	}



	
	// to do: unlit, colored 

	int tooManyVerts = 0;

	if ((state.maxVertexCount>0) && (nv > state.maxVertexCount)) 
		tooManyVerts = 1;

	// tooManyVerts = 1; //debug


	int bufi=0;	// the current execute buffer created 


	while(1) {  // two passes if bothSides 
	
	if (tooManyVerts) {
		IntArray vertMap(nv);	// table for mapping used vertices 
		int i;
		// TRACE("to many verts %d \n",nv);
		int maxVerts = state.maxVertexCount;

		// maxVerts = 32; // debug

		int vertStart = 0;
		int vertEnd;


		while (vertStart  < nv) {		// while verts left


		vertEnd = vertStart+maxVerts;
		
		if (vertEnd >nv) vertEnd = nv;
		
		int newNv = vertEnd-vertStart;

		// get slice of vertics 
		for (i=0; i<nv; i++) vertMap[i]=-1; // not used

		for (i=vertStart; i<vertEnd; i++) vertMap[i]= i-vertStart; 

		// pass 2 

	    if (objBuf.exBuf.Length() <= bufi) {
			objBuf.Append(state.CreateExBuf());
		}
		
		{
		G3ExBuf &buf = *objBuf.exBuf[bufi];
		// Compute size of execute buffer
		size = buf.VERTEX_SIZE(newNv);
		size += 30*buf.OP_SIZE();

		// TRACE("Creating Ex buf v=%d size=%d \n",newNv,size);

		// create execute buffer
		if (!buf.Create (state.lpDev,size)) {
			TRACE("%s:Could not create execute buffer of size &d (verts=%d )\n",ClassName(),size,nv);
			return(0);
		}
		if (!buf.Lock()) {
			TRACE("%s:Could not lock execute buffer \n",ClassName());
			return(0);
		}



		// store misc instructions

		int  cnt;
		if (unlit) {
			LPD3DLVERTEX vp = (LPD3DLVERTEX) buf.lpPointer;
			cnt = GetVertex(state,color, specular,vp,vertMap);
			buf.lpPointer = (void *) &vp[cnt];
		}
		else {
			LPD3DVERTEX vp = (LPD3DVERTEX) buf.lpPointer;
			cnt= GetVertex(state,flipNormals,vp,vertMap);
			buf.lpPointer = (void *) &vp[cnt];
		}

		buf.offset = buf.Length();

		ASSERT(cnt == newNv);

		// store misc instructions

		OP_PROCESS_VERTICES(1, buf.lpPointer);
		if (unlit) {
			PROCESSVERTICES_DATA(D3DPROCESSVERTICES_TRANSFORM | (!hasvc ? D3DPROCESSVERTICES_NOCOLOR : 0)  , 0, cnt, buf.lpPointer);
		} else {
			PROCESSVERTICES_DATA(D3DPROCESSVERTICES_TRANSFORMLIGHT, 0, cnt, buf.lpPointer);
		}

		buf.PUT_ALIGN();

		// store a POINT LIST 
		OP_POINT_LIST(0,cnt, buf.lpPointer);


		buf.PUT_OP_EXIT();
		// Unlock and set 
		buf.Unlock();
    
		ASSERT(buf.Length()<=buf.MaxLength());
		// TRACE("Stored  Ex buf v=%d  len %d size=%d \n",newNv,buf.Length(),buf.MaxLength());

		if (!buf.Set()) {
			TRACE("%s:Could not set execute buffer of size &d (verts=%d )\n",ClassName(),size,nv);
			return(0);
		}
		}  // create new buffer 

		
		//
		bufi++; // need a new exbuf
		vertStart = vertEnd;

		} // for all vertices

	}	 // too mayVerts
	else 
	{
    if (objBuf.exBuf.Length() <= bufi) {
		objBuf.Append(state.CreateExBuf());

	}
		

	G3ExBuf &buf = *objBuf.exBuf[bufi];

    // Compute size of execute buffer
    size = buf.VERTEX_SIZE(nv);
    size += 15*buf.OP_SIZE();

	// TRACE("Creating Ex buf v=%d  size=%d \n",nv,size);

    // create execute buffer
	if (!buf.Create (state.lpDev,size)) {
        TRACE("%s:Could not create execute buffer of size &d (verts=%d )\n",ClassName(),size,nv);
        return(0);
    }
    if (!buf.Lock()) {
        TRACE("%s:Could not lock execute buffer \n",ClassName());
        return(0);
    }

    // store vertex data

	int  cnt;
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
			PROCESSVERTICES_DATA(D3DPROCESSVERTICES_TRANSFORM | (!hasvc ? D3DPROCESSVERTICES_NOCOLOR : 0)  , 0, nv, buf.lpPointer);
		} else {
			PROCESSVERTICES_DATA(D3DPROCESSVERTICES_TRANSFORMLIGHT, 0, nv, buf.lpPointer);
		}

    buf.PUT_ALIGN();

    // store a triangle list
	OP_POINT_LIST(0,nv, buf.lpPointer);

    buf.PUT_OP_EXIT();
    // Unlock and set 
    buf.Unlock();
    
    ASSERT(buf.Length()<=buf.MaxLength());
	// TRACE("Stored  Ex buf v=%d tri=%d size=%d \n",nv,buf.Length(),buf.MaxLength());

    if (!buf.Set()) {
        TRACE("%s:Could not set execute buffer of size &d (verts=%d)\n",ClassName(),size,nv);
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

int GPoints::D3dRenderTL(GglRenderTraversal &traversalState)
{
	RenderState &state = *traversalState.glState;

	int nv= v.Length();

	if (nv == 0 ) 
		return -1;

	DWORD primMask;
	if (traversalState.doClipping || 1 )
		primMask =  D3DDP_DONOTUPDATEEXTENTS; 
	else  
		primMask = D3DDP_DONOTCLIP | D3DDP_DONOTUPDATEEXTENTS;

	vtl.SetLength( (sizeof(D3DTLVERTEX) / sizeof(float)) * ((nv+3) & ~3)); 
	D3DTLVERTEX *vtlp= (D3DTLVERTEX*) vtl.Data();	

	TransformLightVertices(traversalState, vtlp);

	if (state.dx6) state.GLastError = state.lpD3DDevice3->DrawPrimitive(D3DPT_POINTLIST, D3DFVF_TLVERTEX,
		(LPVOID) vtl.Data(), nv, primMask);
	else state.GLastError = state.lpD3DDevice2->DrawPrimitive(D3DPT_POINTLIST,D3DVT_TLVERTEX,(LPVOID) vtl.Data(), nv, primMask);
	if (state.GLastError != D3D_OK) { D3DError("Error DrawPrimitive",state.GLastError); }

	return(0);

}


#endif

// delete all sub meshes
void GShell::DeleteSubMeshes()
{
	if (subMeshes)
		delete subMeshes;
	subMeshes = NULL;
}


void GShell::UpdateSubMeshVertices(int what)
{
	GSubMesh *m=subMeshes;

	 if (!Flat()) {
			const PointArray& tmp = GetVN(); // make shure vertex normals are up to date
	 }

	GVertex v;

	while (m) {
		int l = m->vmap.Length();
		for(int i=0; i<l;i++) {
			this->GetVertex(m->vmap[i],v);
			
			ASSERT(i < m->s->v.Length());   // for gshell

			m->s->SetVertex(i,v);
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

		/* if (vn.Length()>0)*/  m->s->actions &= ~(SHELL_COMPUTE_VNORMALS);

		m = m->next;
	}
}



int GShell::CreateSubMeshes(
							 int maxv // maximum vertices per submesh
							 ) 
{

   DeleteSubMeshes();
   
   if (f.Length() <= 0) return(0);

   if (nfaces <=0) {
	   nfaces= GetFaceCount();
   }

   if (nfaces <=0) return(-1);

   IntArray processedFaces(nfaces,0);

   int nv = GetVertexCnt();

   IntArray vertMap(nv);	

	// debug
   int newMeshCnt = 0;
   int allocatedNewVerts=0;


   int i;

   const int *fp = f;	  // face list stepping pointers
   const int *fpend= fp+f.Length();
   int fi=-1;	// face index

#if 0
   // pass 1

   do { 		// for all faces
		 int flen = * fp ++; // length of this face
		 if (flen<0) flen=-flen;  
		 else fi++;

		 int i;
		 for (i=0; i< flen; i++) {
				int vi = *fp++; // vertex index
		 }
   } while (fp <fpend);

#endif

   // pass2
   fp = f;	
   fi=-1;	// face index

   while (fp < fpend)  { 

	    // not all faces processed
		// for (i=0; i<nv; i++) vertMap[i]=-1; // not used
		 memset(vertMap.DataRW(),0xFF,nv*sizeof(int)); // very very costly

		 int newNv=0;

		 // create new submesh 
		 
		 GSubMesh *subMesh = new GSubMesh();
		 
		 ASSERT(subMesh != NULL);

		 if (RTISA(this,GPolylineI) || RTISA(this,GPolyline))
			subMesh->s = new GPolyline();
		 else if (RTISA(this,GPoints))
			subMesh->s = new GPoints();
		 else 
			subMesh->s = new GShell();
		 
		 ASSERT(subMesh->s != NULL);

		 if (subMesh->s == NULL) {
			 delete subMesh;
			 break;
		 }  	

		 GShell &s = * subMesh->s;
		 GFaceList &newf= subMesh->s->f;
		 IntArray &new2old= subMesh->vmap;
		 subMesh->next = subMeshes;
		 subMeshes = subMesh;

		 newMeshCnt++;
		 
		 subMesh->faceStart = fi+1;

		 while ( (newNv <= maxv) && (fp<fpend)) 
		 {

		 int flen = *fp ++; // length of this face
		 
		 newf.Append(flen);
		 
		 if (flen<0) flen=-flen;  
		 else fi++;


		 // add this face to outlist 
		 for(i=0;i<flen;i++) {
			 int p;
			 p =  *fp++; // fp[i];
			 int pnew = vertMap[p];
			 if (pnew <0 ) 	{ // need a new vertex
					vertMap[p]= newNv;
			
					new2old.Append(p);

					pnew=newNv;
					newNv ++;
					allocatedNewVerts++;
			 }
			 newf.Append(pnew);

		 } // for face 

		 }

		 subMesh->faceEnd = fi;

		 if (Flat()) {
			 s.SetFlat();
 			 const PointArray& tmp = GetVN(); // make shure vertex normals are  up to date
			// need it for flat shading anyway 

		 } else {
			const PointArray& tmp = GetVN(); // make shure vertex normals are  up to date
		 }

		 if (v.Length() >0) {
			 s.v.SetLength(newNv);
		 }

		 if (vn.Length() >0) {
			 s.vn.SetLength(newNv);
		 }
		 if (vc.Length() >0) {
			 s.vc.SetLength(newNv);
		 }
		 if (vp.Length() >0) {
			 s.vp.SetLength(newNv);
		 }

		 // face colors
		 i=subMesh->faceEnd-subMesh->faceStart +1;
		 if (fc.Length() >0 && i >0) {
			 s.fc.SetLength(i);
		 }
		 // copy vertex data from this to submesh

	}
    TRACE("Create submesh original nv %d nfaces %d, submeshes %d, allocated total vertices %d  f = %g \n",
		nv,nfaces, newMeshCnt, allocatedNewVerts, ((float )allocatedNewVerts/(float) nv));
    UpdateSubMeshVertices();
	return(1);

}

void GShell::ComputeRapidModel()
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
				rapidModel->AddTri(&v[p0].x, &v[p1].x, &v[p2].x, fi);
				p1 = p2;
				n--;
			}
		  }
		  else fp+=n;
	  }

	  rapidModel->EndModel();
  	  UnSetAction(SHELL_UPDATE_RAPID_MODEL);

}

void GMesh::ComputeRapidModel()
{
	if (( f.Length() == 0) && (ncols >0) ) {
		ComputeFaceList();
	}
	GShell::ComputeRapidModel();
}









