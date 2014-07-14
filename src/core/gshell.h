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
#ifndef _GSHELL_H
#define _GSHELL_H
/******************************************************************************
@doc

@module GShell.h - GLView graphical objects	|

GShell.h
Copyright (c) 1993 - 1999 by Holger Grahn
All rights reserved

Purpose:

Classes:

GFaceList		IntArray for storing face information

GEdgeList		IntArray for storing edge
									   
GVertex			structure storing complete vertex information
GVertexI		like GVertex, but information stored by index (ref. to table)

GObject			root for Graphical objects
	GShell		polyhedral object
		GMesh	regular quad mesh
		GPolyggon	

Notes:

Changes:

$Revision: 1.19 $
$Log: gshell.h,v $
Revision 1.19  1999/07/06 16:54:53  tom
*** empty log message ***



Todo :

******************************************************************************/
#ifdef _GV

#include <gvsubnode.h>

#endif

#include "grender.h"


// Forward
class PointArray;

class RenderState;

class GShell;

class GSubMesh;

class GHitInfo;

class RAPID_model;
class GConvexVolume;
class GCollisionResponse;




//@class FaceList array storing a set of faces by vertex index
class GFaceList : public IntArray {
public:
		GFaceList() : IntArray()		{};
		GFaceList(int n) : IntArray(n) {};
		GFaceList(int n, const int *iT): IntArray(n,iT) {};
		GFaceList(const IntArray & arg): IntArray(arg) {};

		//!  Compute a facelist for a single polygon of n vertices
		void SetPolygon(int n);
		
		//!  Compute a Facelist for a Grid like topology
		int SetGrid(int rows,int cols,gbool quads=gtrue,gbool closeCols=gfalse,gbool closeRows=gfalse);

		//!  Compute a Facelist for a point set 
		int SetPoints(int nPoints,int start=0);

		//!  Compute a Facelist for a polyline index list 
		int SetPolyline(GFaceList &f,int start=0);


		//!  Set Faces list to inventor format  (-1 is separator)
		int SetFaceListInventor(int nf,const int *fp);

		//! append a point  given the  vertex index
		void AppendPoint(int i) { int e = Length();
												Resize(e+2);
												int *fp = DataRW() + e;
												*fp++=1; *fp++=i;
												};


		//! append a edge given two vertex indices
		void AppendEdge(int i1, int i2) { int e = Length();
												Resize(e+3);
												int *fp = DataRW() + e;
												*fp++=2; *fp++=i1;*fp++ = i2;
												};

		//! append a triangle given 3 indices
		void AppendTri(int i1, int i2,int i3) { int e = Length();
												Resize(e+4);
												int *fp = DataRW() + e;
												*fp++=3; *fp++=i1;*fp++ = i2; *fp++=i3;
												};

		//! append a quadrangle given 4 indices
		void AppendQuad(int i1, int i2,int i3,int i4) { int e = Length();
												Resize(e+5);
												int *fp = DataRW() + e;
												*fp++=4; *fp++=i1;*fp++ = i2; *fp++=i3; *fp++=i4;
												};
		//! find/return offset of "nth" face in facelist
		int GetFaceOffset(int nth);

		//! compute # faces for facelist, starting with an optional offset
		int GetFaceCount(int offset=0);

		//! compute # faces for facelist, starting with an optional offset + hole + triangle counters
		int GetFaceCount(int offset,int *holecnt,int *tricnt);


		//! compute size of cnt faces starting at offset in array elements
		int GetFaceSize(int offset=0,int cnt=1);
		
		//! compute # triangles for facelist, starting with an optional offset (not correct for holes & nonconvex polys)

		int GetTriangleCount(int offset=0);


		//! compute # triangles for facelist, starting with an optional offset (not correct for holes & nonconvex polys), ignoring line len <=2
		int GetTriangleCountReal(int offset=0);

		//! compute # lines for facelist, starting with an optional offset (not correct for holes & nonconvex polys), ignoring line len <=1
		int GetLineCountReal(int offset=0);

		// Compute # lines for facelist, starting with an optional offset 
		// closed = 1, assume extra closing edge 
		int GetLineCount(int offset=0,gbool closed=gfalse);


		//! reverse the direction of individual facest
		int ReverseFaces(int offset=0);


		//! Delete cnt faces starting a face number #nth
		int DeleteNth(int nth=0,int cnt=1);

		// optimize face list (for triangles, and strip building)
		// remove degenerated triangles
		// resulting face might be shorter
		int Optimize(gbool flip=gfalse);



};


#ifdef _OGL
//\@func triangulate a facelist into triangles using the GLU triangulator
int tesselateFaceList ( GFaceList &f, //@parm input facelist
						PointArray &v, //@parm corresponding vertices for indices in f
						IntArray *vmap, // @parm optional vertex map table
						GFaceList &newf, //@parm output: triangulated facelist
						IntArray *new2old,	//@parm output : optional mapping from new triangles to old face indices
                        GShell * theShell);  //@parm optional shell to append vertices too
#endif



//@class EdgeList array, storing a list of edges by 2 vertex indices
class GEdgeList : public IntArray {
public:
		GEdgeList() : IntArray()		{};
		GEdgeList(int n) : IntArray(n) {};
		GEdgeList(int n, const int *iT): IntArray(n,iT) {};
		GEdgeList(const IntArray & arg): IntArray(arg) {};

};

//@class root object of graphical output objects
class GObject : 

#ifdef _GV

	public GvNode
{
      GV_NODE_HEADER(GObject);
#else   // not GV

	public GGeometry 
{
		RT(GGeometry);
		GObject();
		~GObject();
#endif      

public:
		BBox bbox;      // bounding box for object 

		//! Evaluate current state of object, to process or to render
		// virtual GObject* Eval() { return(this); }

		//! Mark object as changed
		virtual void SetChanged() { };
		
		//! Mark object as unchanged
		virtual void UnSetChanged() { };
		
		//! is object changed =
		virtual int Changed() { return(0); }

		virtual BBox & GetBBox()  { return bbox; }
};

/*
	Bitmask describing state information about shell object
*/

enum
		shell_flags {
		SHELL_CONVEX=1,					// convex polyhedron
		SHELL_CLOSED=2, 				// closed polyhedron
		SHELL_SHADE_FLAT=4,
		SHELL_BOUNDS_OK=8,
		SHELL_VNORMALS_COMPUTED=16, 	// Vertex Normals set via ComputeVertexNormals
		SHELL_FNORMALS_COMPUTED=32, 	// Face Normals set via ComputeFaceNormals
		SHELL_FINFO_OK=64,
		SHELL_TRIANGULATED=128, 		// face list only contains triangles
		SHELL_EDGES_OK=256, 			// edge list has been constructed
		SHELL_COMPUTE_PRIMLIST=512, 	// Build primitive list (tristrips)
		SHELL_FLIP_NORMALS=1024,		// Flip Normals during computation (face normals)
		SHELL_FCOLORS_COMPUTED=2048,	// Face Colors are computed from Group information
		SHELL_PRIMLIST_COMPUTED=4096,	// Primitivlist is computed
		SHELL_HAS_HOLE_FACES=8192,		// face list contains hole faces nr ok ?
		SHELL_HAS_CONVEX_FACES=16384,	// face list contains convex faces
		SHELL_PLANAR=16384*2,			// this is a planar object, all face are in the same plane 
		};
/*
	Bitmask describing actions to be done on shell object
*/

enum shell_actions {
		SHELL_COMPUTE_MAXI=1,				// recompute max indices
		SHELL_COMPUTE_FNORMALS=2,			// recompute face normals
		SHELL_COMPUTE_VNORMALS=4,			// recompute vertex normals
		SHELL_COMPUTE_BBOX=8,				// recompute bounding box
		SHELL_COMPUTE_TRIANGULATION=16,		// triangulate shell
		SHELL_COMPUTE_PRIMITIVELIST=32,		// recompute primitve list 
		SHELL_COMPUTE_EDGELIST=64,			// recompute face colors from group
		SHELL_COMPUTE_FCOLORS_FROM_GROUP=128,	// recompute face colors from group
		SHELL_REDRAW=256,				// recompute face colors from group
        SHELL_COMPUTE_EXBUF=512,          // create D3D EXECUTE Buffer
        SHELL_UPDATE_EXBUF_VERT=1024,    // update vertices in D3D EXECUTE Buffer
        SHELL_UPDATE_SUBMESH_VERT=1024*2,  // update vertices in D3D EXECUTE Buffer
        SHELL_UPDATE_RAPID_MODEL=1024*2*2  // update the RAPID model data structure
		};

enum shell_changed_flags {
		SHELL_VERTICES_CHANGED=1,       // vertex positions changes
		SHELL_FACELIST_CHANGED=2,       // face information changed
		SHELL_PRIMLIST_CHANGED=4,       
		SHELL_ATTRIBUTES_CHANGED=8,
		SHELL_VERTEX_ATTRIBUTES_CHANGED=16, // any vertex attribute changed
		SHELL_FACE_ATTRIBUTES_CHANGED=32,   // any face attribute changed
		SHELL_CHANGED=64,
		SHELL_CREATED=128,
		SHELL_FACE_GROUP_CHANGED=256,
		SHELL_VERTS_CHANGED=512,            // the verts array changed
		SHELL_VERTS_LENGTH_CHANGED=1024,    // GetVertexCount changed 

		};

//xxxx//@typee shell_primitive_op | Op codes for precompiled gl low level primitive list
enum shell_primitive_op {
				SHELL_END=1,			// END OF primitive
				SHELL_POINTS,			// equivalent glBegin(MODE) values
				SHELL_LINES,
				SHELL_LINE_LOOP,
				SHELL_LINE_STRIP,
				SHELL_TRIANGLES,
				SHELL_TRIANGLE_STRIP,
				SHELL_TRIANGLE_FAN,
				SHELL_QUADS,
				SHELL_QUAD_STRIP,
				SHELL_POLYGON,
				SHELL_TRISTRIP_FACEINDEX, 		// v1 v2 fi v3 fi v4
				SHELL_SET_MATERIAL,		// index	set material by index
				SHELL_SET_COLOR,		// index	set color by index
				};


typedef struct {
		float plane_d;			// Plane distance, together with face normals forms Plane EQ
		int poly_u:2;			// major axis 1
		int poly_v:2;			// major axis 2
		} shell_face_info;


enum shell_data {
		SHELL_VERTEX=1,
		SHELL_VERTEX_NORMAL=2,
		SHELL_VERTEX_COLOR=4,
		SHELL_VERTEX_PARAMETER=8,

		SHELL_FACE_NORMAL=512,
		SHELL_FACE_COLOR=1024,

		SHELL_FACE_GROUP=2048,

};

//@class GVertex, all vertex data in one structure,
class GVertex {
public :
		//! the vertex coordinates
		Point v;		
		//! the vertex normal
		Point n;		
		//! the vertex color
		Point c;		
		//! the vertex parameter
		Point p;		

		//! Zero out values
		void Zero() { v.Set(0.0,0.0,0.0); n=c=p=v; }

		//! interpolate two GVertex, Result = A*ta + B * tb
		friend GVertex& Interpolate (const GVertex& A,float ta, const GVertex& B,float tb, GVertex &Result);

#ifdef _3DV
		//! io
		int io(AStream &a);
#endif _3DV

};

//@class GVertexI, all vertex data in one structure, but by index array reference
class GVertexI {
public :
		//! the vertex coordinate index
		int v;	
		//! the vertex normal index
		int n;	
		//! the vertex color index
		int c;	
		//! the vertex parameter index
		int p;	

		//! Zero out values to -1
		void Zero() { v=n=c=p=-1; }

		//! compare for equality
		int operator== ( const GVertexI &b) 
			{
				return(v == b.v && n == b.n && c == b.c && p == b.p); 
			}
#ifdef _3DV
		//! io
		int io(AStream &a);
#endif _3DV

};

//
// GSubMesh - store a subset of shell faces & references vertices 
//

class GSubMesh {
public :

	GShell *s;		// shell for submesh
	IntArray vmap;  // mapping s->v[i] = this->vmap[i], for updating vertex coordinates

	
	RGBAColor diffuseColor; // optional faceColor 
	int faceStart;			// offset of facelist in parent shell
	int faceEnd;			// offset of facelist in parent shell

	GSubMesh *next;

	GSubMesh() : s(NULL),next(NULL),faceStart(-1),faceEnd(-1) { }

	~GSubMesh(); // destruct shell and next 

};


//
// GShell - store vertex list with optional attributes and face information
//

//@class a set of vertices + faces
class GShell : public GObject  
{

public:

#ifdef _GV
      GV_NODE_HEADER(GShell);

#define GEvalAs_TYPE GvNode

#else
      RT(GShell);
      GShell();
	  ~GShell();

#define GEvalAs_TYPE GGeometry

#endif



public:

		PointArray v;	// vertex coordinates
		PointArray vn;	// vertex normals
		PointArray vc;	// vertex colors
		PointArray vp;	// vertex parameters

		// Katmai datastructures 

		PointArray4 kv;  	// vertex coordinates
		PointArray4 kvn;	// vertex normals
		PointArray4 kvc;	// vertex colors
		PointArray4 kvp;	// vertex parameters

		PointArray4  kfn;	// face normals
		PointArray4  kfv;	// representative face vertex 


		GVertexI maxI;	// maximum indices for current lists

		GFaceList f;	// facelist
						// structure :
						// #verts of face
						// #verts vert indices
						// optional nverts <0 hole for preceding face

		int nfaces; 	// number of faces

		IntArray prims; // primitive list with gl Opcodes + primitives (triangle strips)
						// optionally computed from facelist f

		PointArray fn;	// face normals
		PointArray fc;	// face colors
		IntArray   fcI;	// index from face number into fc table if present 

		// int *fgroup; [nfaces]
		// int ngroups;
		// FGroup * groups; 					// ShadeFlat, Color ???

		shell_face_info *finfo; // info per plane for speed of ray-intersect operation

		int *e; 		// edge list (computed from facelist)
		int ne; 		// length of edgelist = number of edges
		// to think about: optional explicit edge table, or table with invisible edges ?


		static int initial_flags;
		static int initial_edit_flags;

		int flags;		// different flags
		int edit_flags; // what has been changed
		int actions;	// type of action to be computed on shell 
		int has;		// type of data present ( computed from update)

public:
	  // constructor
	  // GV_NODE_HEADER GShell();
	  GShell(	int new_nv,const Point *new_v,
				int new_nf=0,const int *new_f=NULL);

	 
	  // construct by mesh 
	  GShell(int cols,int rows,const Point *new_v);


     // destructor
	 //GV_NODE_HEADER ~GShell();
     //GV_NODE_HEADER virtual GvFieldData *getFieldData();


protected :
	  virtual void Zero();			// Null out all members (internal)

public: 
	//! Check for recomputation of cached values (e.g. Normals), 1 if redraw
	virtual int Update(); 

	//! update actions, based on edit flags
	virtual int UpdateActions(); 
	
	//! copy raw geometry
	virtual GShell* CopyGeometry(GShell *dest=0) const;
	
	//! copy including attributes
	virtual GShell*  Copy(GShell *dest=0) const;

	//! Set Vertices
	virtual void SetV(int new_nv,const Point *new_v);
	virtual void SetVByRef(int new_nv,const Point *new_v);
	void SetV(const PointArray &new_v);
	
	//! set vertex normals for vertices
	virtual void SetVN(int n_vn,const Point *new_vn,int normalize=0);
	virtual	void SetVNByRef(int n_vn,const Point *new_vn,int normalize=0);
	void SetVN(const PointArray &new_vn,int normalize=0);
	//void SetVNByRef(Point *new_vn);
	
	//! set vertex colors
	virtual void SetVC(int n_vn,const Point *new_vc);
	void SetVCByRef(int n_vc,const Point *new_vc);
	void SetVC(const PointArray &new_vc);
	//void SetVCByRef(Point *new_vc);

	//! set vertex (texture) parameters
	virtual  void SetVP(int n_vp,const Point *new_vp);
	void SetVPByRef(int n_vp,const Point *new_vp);
	void SetVP(const PointArray &new_vp);
	//void SetVPByRef(Point *new_vp);

	//! set face normals
	virtual void SetFN(int n_fn,const Point *new_fn,int normalize=0);
	void SetFNByRef(int n_fn,const Point *new_fn);
	void SetFN(const PointArray &new_fn,int normalize=0);
	//void SetFNByRef(int n_f,Point *new_vn);

	//! set face colors
	virtual void SetFC(int n_fc,const Point *new_fc);
	void SetFCByRef(int n_fc,const Point *new_fc);
	void SetFC(const PointArray &new_fc);

	//! set face colors by indexmap 
	virtual void SetFCByIndex(int n_fci,const int *new_fci,int n_fc,const Point *new_fc);

	//! set face colors index table 
	void SetFCIndex(int n_fci,const int *new_fci);
	void SetFCIndexByRef(int n_fci,const int *new_fci);
	void SetFCIndex(const IntArray &new_fci);

	//! set face list (list of polygons by vertex indices)
	void SetFaceList(int new_nf,const int *new_f);
	void SetFaceListByRef(int new_nf,const int *new_f);
	void SetFaceList(const GFaceList &new_f);

	//! set primitive list = (optimized opengl primitives)
	void SetPrimitiveList(int new_np,const int *new_prims);
	void SetPrimitiveList(const IntArray &new_p);

	//! set edgelist (format v1 v2 ...) connect v1 v2 with a edge
	void SetEdgeList(int new_ne,const int *new_e);
	void SetEdgeListByRef(int new_ne,int *new_e);

	// get functions, recompute values if necessary
	virtual const PointArray& GetV();
	virtual const PointArray& GetVN();
	virtual const PointArray& GetVP();
	virtual const PointArray& GetVC();

	virtual const PointArray& GetFN();
	virtual const PointArray& GetFC();

	// Editing functions

	// vertex append / lookup functions

	
	//! Find nearest point in vertex table
	virtual int FindNearestV(const Point &p,float &distance2);

	//! distance squared up to that points are considered equal on lookup
	static float maxLookupDistance2;
	
	//! set distance up to that points are considered equal on lookup
	void SetMaxLookupDistance(float r) { maxLookupDistance2 = r*r; }

	//! append a single point to v list, optionally with lookup
	int AppendV(const Point &p,int doLookup=1);
	
	//! append a list of points, optionally lookup points and return vertex indices in old2new
	// transform points, if optional attribute m is present
	int AppendV(const PointArray &p,int doLookup=1,IntArray *old2new=0, const Matrix *m=NULL);

	//! get number of vertices
	virtual int GetVertexCnt() { return (v.Length()); }

	// facelist append functions
	
	//! append a single vertex given by a single point, optionally with lookup
	virtual int AppendVertex(const Point &p,int doLookup);

	virtual int AppendVertex(const GVertex &vert);

	
	//! append a list of points, optionally lookup points and return vertex indices in old2new
	virtual int AppendVertex(const PointArray &p,int doLookup,IntArray *old2new);


	//! append facelist, optionally map vertex indices by old2new
	int AppendFacelist(IntArray &p,int doLookup=1,IntArray *old2new=0);

	//! append a single edge
	int AppendEdge(Point &p1,Point &p2,int doLookup=1);

	//! append a single triangle
	int AppendTriangle(Point &p1,Point &p2, Point &p3,int doLookup=1);

	//! append a single polygon
	int AppendPolygon(const PointArray &pts,int doLookup=1);

	//!  append whole shell
	virtual int Append(GShell &s,int doLookup=1, const Matrix *m=NULL);

	//!  Delete cnt faces starting at face # nthOffset,
	int DeleteFaces(int nthOffset,int cnt=1);

	//!  Delete cnt verts starting at vert # nthOffset,
	int DeleteVerts(int nthOffset,int cnt=1);

	//!  Compute bounding box from all vertices
	virtual void ComputeBBox(BBox &box);

	//!  get bounding box from all vertices, will be recomputed if not up to date
	virtual BBox &GetBBox();


	void ComputeBounds();

	// !  Compute the center of all points by averaging 
	virtual void ComputeCenter(Point &center);

	// get bounding box of node // GvNode virtual function 
	virtual GvBool getBBox(BBox &bbox);

	// result 0 not planar, 1: shell is planar, plane computed, 2: plane of first poly
	virtual int isPlanar(Plane &plane);


	// is shell convex 
	virtual gbool isConvex();

	// is p outside the object
	// may fall back to very simply outside bbox computation
	// needed for Occlusion node 

	virtual gbool isOutside(const Point &p);

	
	//!  Compute a facelist containing triangles only in tri
	// tri2face maps triangle # i to shells face number #fi
	// Return: number of triangles
	virtual int ComputeTriangles(GFaceList &tri,IntArray &tri2face,int useTriangulator=0);

	//!  triangulate shell
	virtual int Triangulate();


	//!  compute native vertex parameters for object
	virtual void GetVertexParametersNative(int n,Point *vp);

	// ! get all vertex data for vertex i,
	// if values are not present they will not be stored
	virtual int GetVertex(int i, GVertex &vert)
	{
			if (i < v.Length())  vert.v = v[i]; 			// the vertex coordinates
			if (i < vn.Length()) vert.n = vn[i];	// the vertex normal
			if (i < vc.Length()) vert.c = vc[i];	// the vertex color
			if (i < vp.Length()) vert.p = vp[i];		// the vertex parameter
			return(0);
	}

	//!  set all vertex data for vertex i,
	// if value arrays are not present or to short, values will not be stored

	virtual int SetVertex(int i, const GVertex &vert)
	{

			if (i < v.Length())   v[i] = vert.v;			// the vertex coordinates
			if (i < vn.Length()) vn[i] = vert.n;	// the vertex normal
			if (i < vc.Length()) vc[i] = vert.c;	// the vertex color
			if (i < vp.Length()) vp[i] = vert.p;		// the vertex parameter
			return(0);
	}

	//!  get all vertex data for vertex i by index,
	// if values are not present they will not be stored
	virtual int GetVertexI(int i, GVertexI &vert)
	{

			if (i < v.Length())  vert.v = i;		// the vertex coordinates
			if (i < vn.Length()) vert.n = i;		// the vertex normal
			if (i < vc.Length()) vert.c = i;		// the vertex color
			if (i < vp.Length()) vert.p = i;	// the vertex parameter
			return(0);
	}

	//!  compute the maximum number / length of vertex data for by index,
	virtual int ComputeMaxVertexI( GVertexI &vert);

    //!  get the maximum number/ length of vertex data by index, recomputed if not up to date
	virtual GVertexI& GetMaxI();


	//! count number of faces in face list
	virtual int CountFaces();

	//!  return number of faces
	virtual int GetFaceCount();

	//!  get a facelist of shell
	virtual int GetFaceList(GFaceList &flist);

	//!  compute face normals from geometry
	virtual void ComputeFaceNormals(int allocFaceinfo=0);

	//!  compute vertex normals from geometry
	// this normally includes calling compute face normals
	virtual int ComputeVertexNormals();

	//!  flip normals during computation of faces normals == this propagates to vertexnormals
	virtual int FlipNormals();

	int NormalsFlipped() { return ( flags & SHELL_FLIP_NORMALS) != 0; }

	// compute vertex colors for each vertex from attached faces face color 
	virtual int ComputeVertexColorsFromFaceColors();

	// compute face colors for each face from average of all face vertex colors 
	virtual int ComputeFaceColorsFromVertexColors();


	//!  compute edge list (e.g. from facelist)
	virtual int ComputeEdgeList();
	
	// Flag handling

	// mark for flat shading 
	void SetFlat() { flags |= SHELL_SHADE_FLAT;}
	gbool Flat() { return( (flags & SHELL_SHADE_FLAT) !=0); }
	
	//! set a shell flag, multiple values can be or-ed together
	int SetFlag(int f) { return(flags |=f); }
	
	//! clear a shell flag, multiple values can be or-ed together
	int UnSetFlag(int f) { return(flags &= ~f); }

	//! get a shell flag, multiple values can be or-ed together
	gbool GetFlag(int f) { return( (flags & f) !=0); }


	//! set a edit flag, multiple values can be or-ed together
	int SetEditFlag(int f) { return(edit_flags |=f); }

	//! clear a edit flag, multiple values can be or-ed together
	int UnSetEditFlag(int f) { return(edit_flags &= ~f); }

	//! set a shell action flag, multiple values can be or-ed together
	int SetAction(int f) { return(actions |=f); }
	
	//! clear a shell action flag, multiple values can be or-ed together
	int UnSetAction(int f) { return(actions &= ~f); }


	//! mark edit flags, that object has been changed
	void SetChanged() {SetEditFlag(SHELL_CHANGED); };

	//! clear all edit flags
	void UnSetChanged() { edit_flags = 0; };
	
	//! has something on object been changed ?
	int Changed() { return(edit_flags != 0); }

	/* ! Evaluate object as object of type "type"
	  if return = this, don´t destroy result, otherwise destroy */
	virtual GEvalAs_TYPE* EvalAs(TypeId type,int copyAttributes=1); 


	  void Cleanup();

	  void Make(int new_nv,const Point *new_v,
				int new_nf,const int *new_f);


#ifdef _OGL
	// OPENGL Rendering
	//! rendering faces of shell directly to OpenGL, either parse f list or prims
	virtual void RenderGlFaces(RenderState &state,int FN=1, int VN=0, int VC=0, int VP=0);
	
	// Render primitive list
	virtual void RenderGlPrimitives(RenderState &state,int FN, int VN, int VC, int VP);

	//! rendering edges / edgetable of shell directly to OpenGL
	virtual void RenderGlEdges(RenderState &state,int VN=0,int VC=0,int VP=0);
	
	//! rendering vertices of shell directly to OpenGL
	
	virtual void RenderGlVertices(RenderState &state,int VN=0,int		VC=0);

	//! rendering normals of shell directly to OpenGL

	virtual void RenderGlNormals(RenderState &state,float scale=0.2,int VN=1,int FN=0);

	//! rendering Bounding box of shell directly to OpenGL
	virtual void RenderGlBBox(RenderState &state);

	//! rendering  shell directly to OpenGL using RenderGL members
	virtual int glRender(RenderState &state);
#endif

#ifdef _D3D
    //! Get all vertices in D3DVertex format 
    //! result = number of vertices stored at *V 
    //! Normals must have been computed, V must be the right size 
    virtual int GetVertex(RenderState &state,int flipNormals, D3DVERTEX *V);

    //! get CVERTEX v,n,c,p
	virtual int GetVertex(RenderState &state,int flipNormals, D3DCVERTEX *V);

	//! get vertices in DX flexibile vertexformat
	//! suported flags D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 
	virtual int GetVertex(RenderState &state,int flipNormals,DWORD flags, D3DCOLOR diffuse, D3DVALUE *V);

    //! get pre-lit vertices
	virtual int GetVertex(RenderState &state,D3DCOLOR color, D3DCOLOR specular,D3DLVERTEX *V);

	//! get vertex mapped 
	virtual int GetVertex(RenderState &state,int flipNormals,D3DVERTEX *V,IntArray &map);

	//! get vertex mapped unlit
	virtual int GetVertex(RenderState &state,D3DCOLOR color, D3DCOLOR specular,D3DLVERTEX *V,IntArray &map);


    //! Create execute buffer for shell,
    //! result 0 error, 1 ok
    virtual int CreateExBuf(RenderState &state,G3ObjRef &objBuf);


	/*!
		Create Direct 3D Execute Buffer, all fits in single buffer
	*/
	virtual int CreateExBuf(RenderState &state,G3ExBuf &buf);


    //! Update vertex data in Execute Buffer
    //! vertex size must be identical !!!!!!!!!!!!!!!!
    int UpdateExBufVerts(RenderState &state,G3ExBuf &buf,gbool unlit);

    //! D3D Execute buffer for Shell
    G3ObjRef objBuf;


    
    //! Release D3D Execute buffer, e.g. if device has changed 
    void ReleaseExBuf() { objBuf.Release(); SetAction(SHELL_COMPUTE_EXBUF); }
    
    //! Render Shell using D3D, build execute buffer if needed 
	int D3dRender(GglRenderTraversal &traversalState, DWORD mode = D3DEXECUTE_CLIPPED);

	//! for TL Rendering Array<D3DTLVERTEX> vtl;
	FloatArrayAligned  vtl;

	// TO DO:
	


	//! triangle list
	Array<WORD> primIndex; // DrawPrimitiveIndexed list 

	int nPrim;			//!< number of primitives
	int primType;		//!< D3D type of primitive 
	int textureMatrixVersion; //!< id of texture matrix (already copied to vtl)

	gbool isUnlit;		//!< vertex buffer currently in unlit mode 
	D3DCOLOR  currentColorD3D; //!< current unlit color in VertexBuffer

	//! transform & light vertices 
	void TransformLightVertices(GglRenderTraversal &traversalState, D3DTLVERTEX *vtlp);

	//! Render Shell using D3D IM + custom lighting 
	virtual int D3dRenderTL(GglRenderTraversal &traversalState);

	//! Create Vertex Buffer
	int CreateVBBuf(RenderState &state/*,G3VBBuf &buf*/);

	//! Update vertex buffer
	int UpdateVBBuf(RenderState &state);

	//! Render Shell using D3D IM + vertex buffers 
	virtual int D3dRenderVB(GglRenderTraversal &traversalState);

	
	//! references handle to DX Vertex Buffer 
	G3VBBufRef hVBBuf;

	//	DWORD  vertexTypeDesc

#endif

	static int maxSubMeshVertices; //!< max number of vertices per submesh

	// submesh support
	GSubMesh *subMeshes;	//!< list of submeshes
	
	//! Create sub meshes, each mesh can hold maxSubMeshVertices vertices
	int CreateSubMeshes() { return CreateSubMeshes(maxSubMeshVertices); }

	//! Create sub meshes, each mesh can hold maxv vertices
	virtual int CreateSubMeshes(int maxv);
	
	//! delete all sub meshes
	void DeleteSubMeshes();

	//! update vertices of submesh
	virtual void UpdateSubMeshVertices(int what=0);

	// 



	//! compute primitive list (= e.g. build triangle strips)
	virtual int ComputePrimitiveList(int flags=0);

#ifdef _3DV

	//! output shell attributes
	virtual int ioOutputAttributes(AStream &a);

	//! IO : check current token and input one attribute
	virtual int ioInputOneAttribute(AStream &a);

	//! input/output to AStream
	virtual int io(AStream &a);

#endif _3DV

	// GClass protocol

	//! do nothing 
	int Do(GTraversal &state);
	
	//! GClass do glRendering traversal
	int Do(GglRenderTraversal &state);

	//! GClass do boundingbox traversal
	int Do(GBBoxTraversal &state);

	//! GClass do ray selection 
	int Do(GRaySelectTraversal &state); // declared in GShell.h implmented in GRaySelect

	RAPID_model* rapidModel;
	
	virtual void ComputeRapidModel();



    //! GClass do ray intersection with ray, store hits in traversal
    virtual int IntersectFaces(GRaySelectTraversal &traversal,const GFaceList &f, RayRange &ray, GHitInfo &info); // declared in GShell.h implemented in GRaySelect

	virtual int CollideFaces(const GFaceList &f, GConvexVolume &volume, GCollisionResponse &response, GHitInfo &hitInfo);


	
	// additional io routines implemented in GShellIO.CPP
#ifdef _HMF
	//! write shell to AStream in HMF format
	virtual int WriteHmf(AStream &a);
#endif

#ifdef _GEO
	//! write to GEO polygon format (optional with normals)
	virtual int WriteGeo(AStream &a,int outputNormals=0);

#endif

#ifdef _STL
	//! write to STL triangle format
	virtual int WriteStl(AStream &a);
#endif

#ifdef _POV
	//! write to POV Ray-tracer format
	virtual int WritePov(AStream &a);
#endif

#ifdef _RAW
	//! write to RAW triangle format
	virtual int WriteRaw(AStream &a);
#endif

#ifdef _VRML
	//! write shell to AStream in VRML format
	virtual int WriteVrml(AStream &a,gbool vrml2);

	//! GClass io Traversal
	int Do(GioTraversal &state); 

#endif

#ifdef _GSTATS
	// information for profiling
	static int  verticesProcessed;	// number of vertics processed
	static int  facesProcessed;		// number of faces processed
	static int  primsProcessed;		//
	static int	trianglesDrawn;		// number of triangles sent to render layer
	static int	facesDrawn;			// number of faces sent to render layer
	static int  maxPrimVertexCnt;	// size of largest vertex array
	static int  maxPrimTriCnt;		// size of largest vertex array
	static void ClearStats();

#endif


};

//
//@class a 2 d Array of vertices
//
class GMesh:public GShell
{
public:
    RT(GShell);
	int ncols /* u */;
    int nrows /* v */;

	int closedU,closedV;	// use only for calculating vertex normals , not implemented 

	GMesh(int Nu=0,int Nv=0,const Point *p=0) : GShell() {
								ncols=Nu; nrows=Nv;
								closedU=0;
								closedV=0;
								SetV(nrows*ncols,p);
								};

    // set mesh to new grid of points
	GMesh& Set(int Nu=0,int Nv=0,const Point *p=0)	{
								ncols=Nu; nrows=Nv;
								SetV(nrows*ncols,p);
								return (*this);
								};

	virtual GShell* CopyGeometry(GShell *dest=0) const;

	//! get size int cols and rows 
    void GetSize(int &Cols,int &Rows) { Cols = ncols; Rows = nrows; }
	void SetClosed(int u=0,int v=0) { closedU=u, closedV=v; }
	void GetClosed(int &u,int &v) { u=closedU, v=closedV; }

    //! map u v to linear offset
	int Map(int u,int v) { return(v*ncols+u); }

    //! get number of faces
	int CountFaces();

	//! compute a GShell compatible face list for the mesh
	void ComputeFaceList(gbool quads=gtrue);
	
	void ComputeRapidModel();

	// overwritten virtuals from GShell
	int GetFaceList(GFaceList &flist);
	int GetFaceCount();

	int ComputeTriangles(GFaceList &tri,IntArray &tri2face,int useTriangulator=0);

	void ComputeFaceNormals(int allocFaceinfo=0);
	
    int ComputeVertexNormals();

	// int ComputeEdgeList();


	//! compute a native parametrization implented by
	//! mapping 2d array to 2d rectangle
	void SetVertexParametersNative(float umin=0.0,float umax=1.0,
														   float vmin=0.0,float vmax=1.0);
	// compute native vertex parameters for object
	void GetVertexParametersNative(int n,Point *vp);

	/*! Evaluate object as object of type "type"
	  if return = this, don´t destroy result, otherwise destroy */
	 GEvalAs_TYPE* EvalAs(TypeId type,int copyAttributes=1);

    //! GClass do ray intersection with ray, store hits in traversal
    virtual int IntersectFaces(GRaySelectTraversal &traversal,const GFaceList &f, RayRange &ray, GHitInfo &info); // declared in GShell.h implemented in GRaySelect



#ifdef _OGL
	void RenderGlEdges(RenderState &state,int VN=0,int VC=0,int VP=0);

	void RenderGlFaces(RenderState &state,int FN=1, int VN=0, int VC=0, int VP=0);
#endif

#ifdef _D3D
    //! Create execute buffer for mesh
    //! result 0 error, 1 ok
    virtual int CreateExBuf(RenderState &state,G3ObjRef &objBuf);

	/*!
		Create Direct 3D Execute Buffer, all fits in single buffer
	*/
    virtual int CreateExBuf(RenderState &state,G3ExBuf &buf);

	// create for a subrange 
	int CreateExBuf(RenderState &state,gbool flip, gbool flipNormals,G3ExBuf &buf,int row0, int row1, int col0,int col1);
    
	// Get all vertices in D3DVertex format 
    // result = number of vertices stored at *V 
    // V must be the right size 
    virtual int GetVertex(RenderState &state,int flipNormals,D3DVERTEX *V,int row0, int row1, int col0,int col1);
	
	virtual int GetVertex(RenderState &state,D3DCOLOR color, D3DCOLOR specular,D3DLVERTEX *V,int row0, int row1, int col0,int col1);

	virtual int D3dRenderTL(GglRenderTraversal &traversalState);



#endif

#ifdef _3DV
	int io(AStream &a);
#endif _3DV

	// additional io routines implemented in GShellIO.CPP

#ifdef _HMF
	// write mesh to AStream in HMF format
	int WriteHmf(AStream &a);
#endif

#ifdef _VRML
	// write to AStream in VRML format
	virtual int WriteVrml(AStream &a,gbool vrml2);
#endif

};


//@class a single face
class GPolygon : public GShell {
public :
		RT(GShell);
		//! constructor given a <c Point> array
		GPolygon(int np=0, const Point *p=0);
};

//@class a polyline with optional facelist 

class GPolyline : public GShell {
public :
		RT(GPolyline);
		
		//! constructor given a <c Point> array
		GPolyline(int np=0, const Point *p=0);

		//! constructor given a list of polylines 
		GPolyline(int new_nv,const Point *new_v,
				int new_nf,const int *new_f);

		//!  get the facelist , 2 element faces
		int GetFaceList(GFaceList &flist);



		int Do(GRaySelectTraversal &state);

#ifdef _OGL
		// render the faces as LINE_STRIP
		void RenderGlFaces(RenderState &state,int FN, int VN, int VC, int VP);
		int glRender(RenderState &state);

#endif
#ifdef _D3D
		//! Create execute buffer for shell,
		//! result 0 error, 1 ok
		virtual int CreateExBuf(RenderState &state,G3ObjRef &objBuf);

		//! Render Shell using D3D IM + custom lighting 
		virtual int D3dRenderTL(GglRenderTraversal &traversalState);

#endif



};

//@class a point set with optional facelist 

class GPoints : public GShell {
public :
		RT(GPoints);
		
		//! constructor given a <c Point> array
		GPoints(int np=0, const Point *p=0);

		//!  get the facelist , 1 element faces
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



GShell * new_box3d(GShell *cube=0,float atx=-0.5, float aty=-0.5,float atz=-0.5,
					float dx=1.0,float dy=1.0,float dz=1.0);

GShell * new_icosahedron(GShell *icosahedron=0,int doComputeNormals=1);

//int compute_polygon_plane_index(int nverts, const int vindex[],
//						const Point *verts, Plane *plane);

inline int compute_polygon_plane_index(int nverts, const int vindex[],
						const Point *verts, Plane *plane)
{
		int i;

		/*d*/ Point refpt;
		/*d*/Point normal;
		
		const Point *u, *v;
		
		float len;

		if (nverts <=2) {
			plane->n.x = 0;
			plane->n.y = 0;
			plane->n.z = 1;
			plane->d = 0;
			return 0;
		}
		

		/* compute the polygon normal and a reference Point on
		   the plane. Note that the actual reference Point is
		   refpt / nverts
		*/
		normal.Zero();
		refpt.Zero();

		v = &verts[vindex[0]];

		for(i = nverts-1; i >=0 ; i--) {
			u = &verts[vindex[i]];

			//v = &verts[vindex[(i + 1) % nverts]];

			normal.x += (u->y - v->y) * (u->z + v->z);
			normal.y += (u->z - v->z) * (u->x + v->x);
			normal.z += (u->x - v->x) * (u->y + v->y);

			refpt+= *u;

			v=u;
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

#endif // _GSHELL_H


