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
#ifndef  _GvBsp_h_
#define  _GvBsp_h_


#include <gvsubnode.h>

#include <gvsfrotation.h>
#include <gvsfnode.h>

#include <gvmfnode.h>

#include <gvsfbool.h>
#include <gvsftime.h>
#include <gvsfvec3f.h>
#include <gvmfstring.h>

#include <gv2group.h>


/* 
   Build a BSP Tree 
   out of a set of nodes

*/

GvNode *BuildBspTree(GvMFNode &shapes); 



#if 0

class GvBspShape : public GvNode {

    GV_NODE_HEADER(GvBspShape);

public:
    // Fields
   //GvMatrixTransform transform; 

   GvSFNode appearance; // Default NULL
   GvSFNode geometry;  // Default NULL 

public:

	BBox bbox; // bounding box in GLOBAL coordinates 

	// get matrix of node , FALSE if node has no matrix 
	GvBool getMatrix(Matrix &m) {	get(m); return(TRUE); }
	void get(Matrix &m) { matrix.Get(m); }

	// apply transform to state 
	int setMatrix(GTraversal &state);

	BBox &getBoundingBox() { return bbox; }



    int Do(GTraversal &state);   	
    int Do(GglRenderTraversal &state);
    int Do(GRaySelectTraversal &state);  

    int Do(GBBoxTraversal &state);


};

#endif


#define TRAVERSE_NORMAL	0
#define TRAVERSE_BACK_TO_FRONT 1
#define TRAVERSE_FRONT_TO_BACK 2
#define TRAVERSE_FRONT_TO_BACK_VISIBILITY 3


enum BSP_HEURISTIC {
	USE_FIRST_PLANAR_OBJECT,
	USE_LARGEST_PLANAR_OBJECT,
	SPLIT_ALONG_BBOX,
	COMPUTE_SEPARATION_PLANE
};

//
// GvBspTree
//


class GvBspTree : public GvNode {

    GV_NODE_HEADER(GvBspTree);

public:
    // Fields

   GvSFRotation plane;	// the separation plane 
   GvSFNode front; // Default NULL
   GvSFNode overlap; // Default NULL
   GvSFNode back;  // Default NULL 

   // set the plane 
   void setPlane(const Plane &p) { plane.set(p.n.x,p.n.y,p.n.z,p.d); }
   void getPlane(Plane &p) { p.Set(plane.value[0],plane.value[1],plane.value[2],plane.value[3]); }

#ifdef _DEBUG
   // additional BBox check 
   GvBool bboxOk;	
   BBox bbox;

#endif

   // global builder flags
   static BSP_HEURISTIC bspHeuristic;	// bsp split heuristic
   static BOOL	bspSplit;	// split of polygons / facesets allowed 
   static int   maxBspSplit; // max allowed bsp-tree depth 

   // statistic
   static int bspLevel;
   static int bspSearchLimit; // limitation of the number of candidates search
   static int bspMaxLevel; // maximum allowed tree level
   static int bspOverlapMax;
   static int bspOverlapMean;
   static int bspOverlapMeanCnt;
   static int bspNumNodes; // num BspTree Nodes
   static int bspNumGeomNodes; // num Geometry Nodes

   static void ClearStats();

   //#ifdef _DEBUG   
   // render stats
	static int boxCullsSucceeded;
	static int viewerPlaneCullsSucceeded;
	static int checkedNodes;

	static int cullChecks;
	static int cullChecksSucceeded;

	static void ClearBspStats();
	static void TraceBspStats();
//#endif


public:

   int Do(GTraversal &state);   	
   int Do(GPathTraversal &state);   	
   int Do(GglRenderTraversal &state);
   
   int Do(GLoadInlinesTraversal &state);

   int Do(GRaySelectTraversal &state);  
   int Do(GBBoxTraversal &state);
   int Do(GOptimizeTraversal &state); 

};

//
// BspGroup - automatically build BspTree
//


class GvBspGroup : public Gv2Group {

    GV_NODE_HEADER(GvBspGroup);

public:
    // Fields :

    // bboxCenter
    // bboxSize

	GvBool		 buildBspTree;  // need to rebuild bsp Tree
    GvSFNode     bspTree;      // NULL,



	// helpers 
	GvBool isOutside(GTraversal &state, const Point &p);
	GvBool isOutside(GTraversal &state, GvNode* proxy,const Point &p) const;
	GvBool isOutside(GTraversal &state, GvMFNode &proxy,const Point &p) const;

	// build the bsp tree
	GvBool BuildBspTree();

	// children

    // call by subfield on touch
	int OnFieldChanged(GvField *field);


    int Do(GTraversal &state);
    int Do(GPathTraversal &state);
    int Do(GglRenderTraversal &state);

//	int Do(GRaySelectTraversal &state);   // in GRaySelect.cpp
//	int Do(GOptimizeTraversal &state);	// implemented in GOptimizeTraversal.cpp


};


//
// Occlusion is a Group node
// that traverses the children only if the viewpoint is outside of  all of the proxy volumes 
// isActive enterTime and exitTimer are notifying similar to a ProximtySensor
// if the viewer enters and exits the proxy Space 

class GvOcclusion : public Gv2Group {

    GV_NODE_HEADER(GvOcclusion);

public:
    // Fields :

    // bboxCenter
    // bboxSize
	GvSFBool	 enabled;	  // TRUE, true : enable processing,  behave as Group 
    GvSFNode     proxy;      // NULL, if viewpoint outside proxy, chilren are not rendered

    // EventOut
    GvSFBool	 isActive;	    // are the children currently visible  ?
    GvSFTime     enterTime;		// when children becomes traversed
    GvSFTime     exitTime;		// when children are no longer traversed 


    void setActive(GvBool state) { if (isActive != state) isActive.set(state); }

	Point lastPosition;	   // used to minimize position checks 

	// helpers 
	GvBool isOutside(GTraversal &state, const Point &p);
	GvBool isOutside(GTraversal &state, GvNode* proxy,const Point &p) const;
	GvBool isOutside(GTraversal &state, GvMFNode &proxy,const Point &p) const;


	// children

    int Do(GglRenderTraversal &state);

	int Do(GRaySelectTraversal &state);   // in GRaySelect.cpp

//	int Do(GOptimizeTraversal &state);	// implemented in GOptimizeTraversal.cpp

	GvBool wasRendered; // set to true each time it was reached by  Do(GglRenderTraversal &state);



};

//
// Inclusion is a Group node
// that traverses the children only if the viewpoint is inside of one or more of the proxy volumes 
// isActive enterTime and exitTimer are notifying similar to a ProximitySensor
// if the viewer enters and exits the proxy Space 
// in addition further traversing of the scene graph is aborted, if an inclusion is active

class GvInclusion : public Gv2Group {

    GV_NODE_HEADER(GvInclusion);

public:
    // Fields :

    // bboxCenter
    // bboxSize
	GvSFBool	 enabled;		// TRUE, true : enable processing,  behave as Group 
    GvSFNode     proxy;			// NULL, if viewpoint outside proxy, chilren are not rendered

	GvMFNode	 portals;		// list of portals or doors

    // EventOut
    GvSFBool	 isActive;	    // are the children currently visible  ?
    GvSFTime     enterTime;		// when children becomes traversed
    GvSFTime     exitTime;		// when children are no longer traversed 


    GvSFBool	 isInside;	    // user is inside ?

    void setActive(GvBool state) 
	{ 
		if (isActive != state) isActive.set(state); 
	}

    void setInside(GvBool state) 
	{ 
		if (isInside != state) isInside.set(state); 
	}


	Point lastPosition;	   // used to minimize position checks 

	// helpers 
	GvBool isPointInside(GTraversal &state, const Point &p);
	GvBool isPointInside(GTraversal &state, GvNode* proxy,const Point &p) const;
	GvBool isPointInside(GTraversal &state, GvMFNode &proxy,const Point &p) const;

	GvBool anyPortalOpen(GGeometryTraversal &state);
	GvBool isNodeVisible(GGeometryTraversal &state, GvNode* proxy);
	GvBool isNodeVisible(GGeometryTraversal &state, GvMFNode &proxy);
	GvBool isBBoxVisible(GGeometryTraversal &state, BBox &proxy);


	GvBool wasRendered; // set to true each time it was reached by  Do(GglRenderTraversal &state);

	// children

    int Do(GglRenderTraversal &state);

	int Do(GRaySelectTraversal &state);   // in GRaySelect.cpp

//	int Do(GOptimizeTraversal &state);	// implemented in GOptimizeTraversal.cpp


};

class Pointi 
{
public:
	int x,y,z;
	Pointi(int x_=0,int y_=0,int z_=0) : x(x_),y(y_),z(z_) {}
	Pointi(const Pointi &p) : x(p.x),y(p.y),z(p.z) {}

	friend int operator< (const Pointi& A, const Pointi& B);

	friend int operator== (const Pointi& A, const Pointi& B);
	friend int operator!= (const Pointi& A, const Pointi& B);

};

inline int operator< (const Pointi& A, const Pointi& B)
{
	return A.x < B.x && A.y < B.y && A.z < B.z;
}

inline int operator== (const Pointi& A, const Pointi& B)
{
	return (A.x == B.x) && (A.y == B.y) && (A.z == B.z);
}

inline int operator!= (const Pointi& A, const Pointi& B)
{
	return !(A == B);
}



class GTileEntry {
public : 
	Pointi coord;	// the logical coordinate
	GvNodeHandle node;
	BOOL inScene;  // entry is instanced in SceneGraph
	GTileEntry(const Pointi c) : coord(c), inScene(FALSE) {}

};

class GvMetaverse : public Gv2Group {

    GV_NODE_HEADER(GvMetaverse);

public:
    // Fields :

    // bboxCenter
    // bboxSize
	//GvSFBool	 enabled;	  // TRUE, true : enable processing,  behave as Group 

	GvSFVec3f	 start;
	GvSFVec3f	 end;
	GvSFVec3f	 spacing;
	GvMFString	 urlPrefix;


	GvSFNode     proxy;      

	BOOL		initialized;

	Pointi		currentTile;

	
	Array<GTileEntry *> tileCache;

	// store entry in cache
	void putCache(GTileEntry *entry);
	
	// find entry in cache
	GTileEntry *getCache(Pointi coord);

	// flush 
	void FlushAll();

	// generate the scene graph for coordinate
	GvNode *generateTileNode(Pointi coord);

	// map world position to tile coordinate
	// FALSE if outside the range
	BOOL computeTileCoord(const Point &p,Pointi &coord);

	// get the Inline url field	
	void generateTileUrl(Pointi coord,GvMFString &url);

	// add the tile node to the scene graph
	BOOL addTileNode(GTileEntry *entry);
	
	// remove the tile node to the scene graph
	void removeTileNode(GTileEntry *entry);


    // EventOut
    //GvSFBool	 isActive;	    // are the children currently visible  ?
    //GvSFTime     enterTime;		// when children becomes traversed
    //GvSFTime     exitTime;		// when children are no longer traversed 

    int Do(GglRenderTraversal &state);
    int Do(GRaySelectTraversal &state);  

	//int Do(GLoadInlinesTraversal &state);

};



#endif // _GvBsp_h_
