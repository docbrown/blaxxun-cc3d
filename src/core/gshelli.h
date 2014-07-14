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
#ifndef _GSHELLI_H
#define _GSHELLI_H
/******************************************************************************
@doc

@module GShellI.h - GLView GShellI object	|

Copyright (c) 1995	by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GFaceGroup>
<c GShellI>
Notes:

Changes:

$Revision: 1.26 $
$Log: gshelli.h,v $
Revision 1.26  1999/07/06 16:54:55  tom
*** empty log message ***

Revision 1.10  1998/08/20 17:22:16  holger
empty message



Todo :

******************************************************************************/

class GFaceGroup;

class GShellI;

/*!
	computeNormalIndices
	compute crease angle based vertexNormal indices
	giving as input
	the coordIndex , coord, creaseAngle Data from a VRML IndexedFaceSet
	Indices will be returned in normalIndex.
	fn will contain computed face normals 

*/

int computeNormalIndices(int nci, const long *cind,
						 int ncoords, const Point *coords,
						 float creaseAngle, gbool ccw,
						 PointArray &fn,IntArray &normalIndex);

int computeNormalIndices (const GFaceList &f,
						 Array<GVertexI> &verts,
						 int ncoords, const Point *coords,
						 float creaseAngle, gbool ccw,
						 PointArray &fn);


/*! 
GFaceGroup

Information on a set of faces
(only for obj reader)
*/
 
class GFaceGroup {
public:

	int flags;
	enum {
		COLOR=1,	// color attribute set
	};

	Point color;
	char name[60];	// name of this group

	int offset; // face offset (not saved )
	int cnt;	// face count

	GFaceGroup() { flags = 0; name[0]=0; }

	GFaceGroup(const GFaceGroup &src) { flags = src.flags; strcpy(name,src.name); color = src.color; }

	// material
	void SetColor(const Point &NewColor) { color = NewColor; flags |= COLOR; }
	int HasColor() { return (flags & COLOR)!=0; }
	void SetName(const char *NewName) { strcpy(name,NewName); }
	int MatchName(const char *SearchName) { return(streq(SearchName,name)); }

#ifdef _3DV
	// IO : input/output
	int io(AStream &a);
#endif _3DV

};



/*!
@class a GShell derived object
Each (logical) vertex can share individually v,p,c,n vertex attributes.
So a vertex coordinate can be defined once, different vertices can share
this coordinate using the coordinate index, but each vertex can have a
different color / normal / texture index.
Usefull for objects like cylinders, sweeps, objects with "hard"
edges.

*/

class GShellI : public GShell  {
public:
	RT(GShellI);

	//! array of vertices
	//! each vertex can have independent attribute indices
	Array<GVertexI> verts;

	//! constructor
	GShellI();
    GShellI(	int new_nv,const Point *new_v,
				int new_nf=0,const int *new_f=NULL,gbool normalsPerFace=gfalse);

	 
    //! construct by mesh 
    GShellI(int cols,int rows,const Point *new_v,gbool normalsPerFace=gfalse);


	//! destructor
	~GShellI();

	//! Check for Recomputation of cached values (e.g. Normals), 1 if redraw
	virtual int Update(); 

	virtual void UpdateSubMeshVertices(int what=0);

	//! result 0 not planar, 1: shell is planar, plane computed, 2: plane of first poly
	int isPlanar(Plane &plane);

    virtual int ComputeMaxVertexI( GVertexI &vert);

	//! get number of vertices
	virtual int GetVertexCnt() { return (verts.Length());}


	//! set vertices
	void SetVertices(int new_nv,const GVertexI *new_v);
	void SetVertices(const Array<GVertexI> &new_v);

	//! Get Vertex Data by Value
	//! if value arrays are not present they will not be stored
	virtual int GetVertex(int i, GVertex &vert);

	//! set all vertex data for vertex i,
	//! if value arrays are not present or to short, values will not be stored
	virtual int SetVertex(int i, const GVertex &vert)
	{

		if (i < v.Length())   v[i] = vert.v;	// the vertex coordinates
		if (i < vn.Length()) vn[i] = vert.n;	// the vertex normal
		if (i < vc.Length()) vc[i] = vert.c;	// the vertex color
		if (i < vp.Length()) vp[i] = vert.p;	// the vertex parameter
		return(0);
	}

	//! Set Length of verts array
	int SetVertsLength(int newLen)
	{
       if (newLen != verts.Length()) edit_flags |= SHELL_VERTS_LENGTH_CHANGED;
       verts.SetLength(newLen);
		 return (verts.Length());
	};

	//! get all vertex data for vertex i by index,
	virtual int GetVertexI(int i, GVertexI &vert)
	{

		if (i < verts.Length())  {
		   vert = verts[i]; // all vert information
		   return(0);
		} else return(-1);
	}

	//! set all vertex data for vertex i by index,
	virtual int SetVertexI(int i, const GVertexI &vert)
	{

		if (i < verts.Length())  {
		   verts[i] = vert; // all vert information
		   return(0);
		} else return(-1);
	}

	// !append a single GVertexI, optionally with lookup
	int AppendVertex(const GVertexI &p,int doLookup=1);

	virtual int AppendVertex(const GVertex &vert);


	//! append a single vertex given by a single point, optionally with lookup
	virtual int AppendVertex(const Point &p,int doLookup=1);
	
    // append a list of points, optionally lookup points and return vertex indices in old2new
	//virtual int AppendVertex(const PointArray &p,int doLookup,IntArray *old2new)

	//! Translate vertex indices into direct v indices
	int ExtractFace(int flen, const int *fp, int *outfp);

	//! compute face normals from geometry
	void ComputeFaceNormals(int allocFaceinfo=0);

	//! compute vertex normals from geometry
	//! this normally includes calling compute face normals
	int ComputeVertexNormals();


	//! compute vertex colors for each vertex from attached faces face color 
	int ComputeVertexColorsFromFaceColors();

	//! compute face colors for each face from average of all face vertex colors 
	int ComputeFaceColorsFromVertexColors();


	//! compute edge list (e.g. from facelist)
	int ComputeEdgeList();


	//! Facegroup support
	IntArray fg;	// mapping face index to fgroup

	//! list of face groups
	Array<GFaceGroup> fgroup;

	//! Lookup Group with name name, return index or -1 if not found
	int FindFaceGroup(const char *name) {
			int i; for(i=0; i<fgroup.Length(); i++) {
				if (fgroup[i].MatchName(name)) return(i); }
			 return(-1);
	   }

	
	//! append a new face group
	int AppendFaceGroup(GFaceGroup &NewGroup)
	   {
			fgroup.Append(NewGroup);
			return(fgroup.Length()) -1;
	   }

	
	//! Compute face colors from group tables
	int ComputeFaceColorsFromGroups();
	
	//! compute a new shell object from a GShellI object
	GShell *ComputeShell(GShell *s=0,int copyAttributes=1);

	/*! Evaluate object as object of type "type"
	 if return = this, don´t destroy result, otherwise destroy */

    GEvalAs_TYPE* EvalAs(TypeId type,int copyAttributes=1); 
	
	int ComputeTriangles(GFaceList &tri,IntArray &tri2face,int useTriangulator);




#ifdef _OGL
	// OPENGL Rendering
	void RenderGlEdges(RenderState &state,int VN=0,int VC=0,int VP=0);
	void RenderGlFaces(RenderState &state,int FN=1, int VN=0, int VC=0, int VP=0);
#endif

#ifdef _D3D
    //! Get all vertices in D3DVertex format 
    //! result = number of vertices stored at *V 
    //! Normals must have been compute, V must be the right size 
    virtual int GetVertex(RenderState &state,int flipNormals,D3DVERTEX *V);

	
	//! get vertices in DX flexible vertexformat
	//! suported flags D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 
	virtual int GetVertex(RenderState &state,int flipNormals,DWORD flags, D3DCOLOR diffuse, D3DVALUE *V);

    //!get CVERTEX v,n,c,p
	virtual int GetVertex(RenderState &state,int flipNormals, D3DCVERTEX *V);


    //! get pre-lit vertices
	virtual int GetVertex(RenderState &state,D3DCOLOR color, D3DCOLOR specular,D3DLVERTEX *V);
	
	//! get vertex mapped 
	virtual int GetVertex(RenderState &state,int flipNormals,D3DVERTEX *V,IntArray &map);

	//! get vertex mapped unlit
	virtual int GetVertex(RenderState &state,D3DCOLOR color, D3DCOLOR specular,D3DLVERTEX *V,IntArray &map);

	//! Render Shell using D3D IM + custom lighting 
	virtual int D3dRenderTL(GglRenderTraversal &traversalState);


#endif


    //!! GClass do ray intersection with ray, store hits in traversal
    virtual int IntersectFaces(GRaySelectTraversal &traversal,const GFaceList &f,RayRange &ray, GHitInfo &info); // declared in GShell.h implemented in GRaySelect
	virtual void ComputeRapidModel();
	int CollideFaces(const GFaceList &f,GConvexVolume &volume, GCollisionResponse &response, GHitInfo &hitInfo);

#ifdef _3DV
	//! IO : input/output
	int io(AStream &a);
#endif _3DV

	//! additional io routines implemented in GShellIO.CPP

#ifdef _VRML
	//! write to AStream in VRML format
	virtual int WriteVrml(AStream &a,gbool vrml2);
#endif


}; // GShellI


// Helping class for computing set of verts for shelli
class GVertexIEntry {
public:
		int vi; // ref to vertex number in verts table
		GVertexIEntry *next;

		GVertexIEntry(int VI,GVertexIEntry *NEXT=NULL) {
						vi = VI;
						next = NEXT; }

};

// helper class
// to lookup and enter GVertexI elements
// quickly into an Array<GVertexI> 


class GVertexITable {
public :
		GVertexIEntry * *v;
		int nv;
		Array<GVertexI> &verts; // reference to vert table
		int nverts;

		// initialize vertex table
		GVertexITable(Array<GVertexI> &Verts,
			int NV /* max vert.v used as v table key */);


		~GVertexITable();

		// Enter Vertex vert into verts table, 
		// Return index of vert if inserted,
		// or -1 if enter == 0 and vert not found
		int Lookup(GVertexI &vert,int enter=1);
};

//@class a polyline Indexed with optional facelist 

class GPolylineI : public GShellI {
public :
		RT(GPolylineI);
		
		//! constructor given a <c Point> array
		GPolylineI();

		//!  get the facelist , 2 element faces
		int GetFaceList(GFaceList &flist);

		int Do(GRaySelectTraversal &state);

#ifdef _OGL
		// render the faces as LINE_STRIP
		void RenderGlFaces(RenderState &state,int FN, int VN, int VC, int VP);
		int glRender(RenderState &state);
#endif

#ifdef _D3D
		// Create execute buffer for shell,
		// result 0 error, 1 ok
		virtual int CreateExBuf(RenderState &state,G3ObjRef &objBuf);
		virtual int D3dRenderTL(GglRenderTraversal &traversalState);

#endif



};






// compute default VRML texture coordinates based on Bounding Box for a set of points
void ComputeBoxParams(const BBox &bbox, const PointArray &p, PointArray &params);
void ComputeBoxParams(const BBox &bbox, const Array<GVertexI> &verts, const PointArray &p, PointArray &params);


// compute a new Ghell object from a GShellI object with only used V VN VC VP entrys
GShellI * CompressShell(GShellI *old, GShellI *s,int copyAttributes,int lookupAttributes);
GShellI * CompressShell(GShell *old, GShellI *s,int copyAttributes,int lookupAttributes);


int ReadShellWavefront(const char *FileName,GShellI *&s);

GShellI * NewBox(float atx=-0.5, float aty=-0.5,float atz=-0.5,
			float dx=1.0,float dy=1.0,float dz=1.0,GShellI *cube=0);


GShellI *NewCylinder(float cx=0.0,float cy=0.0,float cz=0.0,float r=1.0,float h=2.0,int n=12,GShellI *o=0);

GShellI *NewCylinderVrml(float cx=0.0,float cy=0.0,float cz=0.0,float r=1.0,float h=2.0,int n=12,int parts = -1,Point *facolors=NULL,GShellI *o=0);
GShellI *NewConeVrml(float cx=0.0,float cy=0.0,float cz=0.0,float r=1.0,float h=2.0,int n=12,int parts = -1,Point *facolors=NULL,GShellI *o=0);
GShellI * NewBoxVrml(float atx=-0.5, float aty=-0.5,float atz=-0.5,
			float dx=1.0,float dy=1.0,float dz=1.0,GShellI *cube=0);

// sphere with parameters arround y
GShellI *NewSphereVrml(float cx=0.0,float cy=0.0,float cz=0.0,float r=1.0, int n1=16,int n2=8,Point *folors=NULL,GShellI *o=NULL);



#endif
