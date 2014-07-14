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

#ifndef  _GV2Group_
#define  _GV2Group_
/******************************************************************************
@doc

@module Gv2Group.h - VRML 2.0 Group nodes|

Copyright (c) 1996 - 1998 by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GGv2Group>

Notes:

Changes:

$Revision: $
$Log:

Todo :

******************************************************************************/


#include <gvsubnode.h>
#include <gvsfvec3f.h>
#include <gvsfrotation.h>

#include <gvsfnode.h>
#include <gvmfnode.h>

#include <gvsfbool.h>
#include <gvsftime.h>
#include <gvmffloat.h>

// for Gv2MatrixTransform
#include <gvsfmatrix.h>

// for GvHUD
#include <gvmfstring.h>

class BBox;

typedef enum  
{
	GVG_FLAGS_VALID = 1,
	GVG_HAS_GEOMETRY_SENSOR = 2,
	GVG_HAS_VISIBILITY_SENSOR = 4,
	GVG_HAS_PROXIMITY_SENSOR = 8,
	GVG_HAS_DIRECTIONAL_LIGHT = 16,
	GVG_HAS_SOUND = 32,
	GVG_HAS_BBOX = 64
} GvGroupFlags;

//
//  Gv2Group - VRML 2.0 Group node, ROOT for most other Group nodes 
//
class Gv2Group : public GvNode {

    GV_NODE_HEADER(Gv2Group);

public:
    // Fields


    GvSFVec3f  bboxCenter;      // Default :  0 0 0
    GvSFVec3f  bboxSize;        // Default     0 0 0
    
	GvMFNode   children;        // Default  [ ]
    
    
    // Event IN
    void addChildren(GvMFNode *event);
    void removeChildren(GvMFNode *event);
    
    //! set bounding box 
    void setBBox(const BBox& box);

    //! get bounding box , false if empty box 
    GvBool getBBox( BBox& box);

    //! is the bounding box empty ????
	//GvBool bboxIsEmpty() { return ((bboxSize[0]<=0.0) && (bboxSize[1]<=0.0) && (bboxSize[2]<=0.0)); }
	GvBool bboxIsEmpty() { return ((bboxSize[0]<0.0)); }
	
	//! VRML spec : at leat one of size is greater 0
	GvBool hasBBoxEx() { return ( (bboxSize[0]>0.0) || (bboxSize[1]>0.0) || (bboxSize[2]>0.0)); }

	//! status flag mask, internal bits from enum GvGroupFlags
	unsigned char statusFlags;
	//! recompute status flags
	void computeStatusFlags(GTraversal &state);

  public:

    //! add (append) a new child
	void addChild(GvNode *node);


#if 0
	// corresponding member functions for VRML 1 group ?
#if 0
    GvNode *	getChild(int index) const;
    int			getNumChildren() const;
#else
    // HG
    GvNode *		getChild(int index) const { return(children)[index];}
    int			getNumChildren() const { return children.getLength();};
#endif

    virtual GvMFNode *getChildren() { return(&children); }
    

	// delete child at index
	int deleteChild(int index);

	// delete all children
	int deleteChildren();
	
	// insert & ref child at index
	int insertChild(int index,GvNode *node);
	
	// insertChild(index, srcGroup[index])
	int moveChild(int index,GvNode *scrGroup, int srcIndex);
	
	// find a child by index
	GvNode *findChild(int index);

	// find a child by ptr, return index, -1  if not found
	int  findChild(GvNode * node);

	// find a child by name
	GvNode *findChild(const GvName &name);
	// find a child by type
	GvNode *findChild(TypeId type);

	// copy()
	// copyChildrenTo(GvGroup	*dest)
	
	// GLView generic traversal
	// default : pass to Do void Traverse(GTraversal &state);
#endif 

    // called by subfield on touch
	int OnFieldChanged(GvField *field);


	virtual int TraverseChildren(GTraversal &state);
	virtual int TraverseChildren(GPathTraversal &state);

	// put all GeometrySensors in children into state 
	// add call apply to register in state
	int ApplyGeometrySensors(GTraversal &state);

	// GLView generic traversal
	// default :  Push  + call virtual TraverseChildren + pop 

	int Do(GTraversal &state);   
	int Do(GPathTraversal &state);   

	// apply touchsensors first 
	int Do(GRaySelectTraversal &state);


	// use Bbox, if specified 
	int Do(GBBoxTraversal &state);

    int Do(GglRenderTraversal &state);

	//int Do(GioTraversal &state);
	int Do(GDisplayTreeTraversal &state); // implemented in GDisplayTree.cpp

	int Do(GOptimizeTraversal &state);	// implemented in GOptimizeTraversal.cpp


	//! Evaluate object as object of type "type"
	// !if return = this, don´t destroy result, otherwise destroy
	GvNode *EvalAs(TypeId type,int copyAttributes); 
};


//
// CullGroup :: a group with viewFrustrum culling check & LOD
//
class Gv2CullGroup : public Gv2Group {

    GV_NODE_HEADER(Gv2CullGroup);

	GvSFBool		lod;		// FALSE

	GvMFNode		level;      // []	// proxy geometry in -1 1 coordinate range .e.g. a box
    GvMFFloat		range;      // []	// the ranges

	GvBool bboxOk;	
	BBox bbox;

    // notification on field change
    virtual int OnChanged();

public:
    int Do(GglRenderTraversal &state);
	int Do(GRaySelectTraversal &state);


};


//
// Gv2Transform
//


class Gv2Transform : public Gv2Group {

    GV_NODE_HEADER(Gv2Transform);

public:
    // Fields :

    // bboxCenter   not including transform effect 
    // bboxSize
    GvSFVec3f		translation;	 //!< Translation vector default : 0 0 0
    GvSFRotation	rotation;	     //!< Rotation default : 0 0 1  0
    GvSFVec3f		scale;	         //!< Scale factors default : 1 1 1
    GvSFRotation	scaleOrientation;//!< Defines rotational space for scale default : 0 0 1  0
    GvSFVec3f		center;	         //!< Center point for scale and rotate default : 0 0 0
    // children

private:	
	//! computed matrix for above individual fields
    Matrix 	transformCache;         

	//! has the matrix been computed ? 
	GvBool transformCached;         
    //GvBool isIdentity

public:

	//! check field values for defaults, and set default flag on fields 
	int checkForDefaultValues(int mode);

	//! get the transformation matrix 
    void get(Matrix &m);

   	//! get matrix of node , FALSE if node has no matrix 
	GvBool getMatrix(Matrix &m); 

   	//! set transform by matrix 
	void setMatrix(const Matrix &m); 
    
    //! get matrix, recompute if necessary
    inline Matrix& Transform() { 
            if (!transformCached) { get(transformCache); transformCached = 1; } 
            return(transformCache);        
    }


	int Do(GTraversal &state);

	int Do(GPathTraversal &state);   

	// apply touchsensors first 
	int Do(GRaySelectTraversal &state);

	int Do(GBBoxTraversal &state);
    int Do(GglRenderTraversal &state);

	int Do(GOptimizeTraversal &state);	// implemented in GOptimizeTraversal.cpp

	
    // notification on field change
    virtual int OnChanged();
};

class Gv2MatrixTransform : public Gv2Group {

    GV_NODE_HEADER(Gv2MatrixTransform);

public:
    // Fields :

    // bboxCenter   not including transform effect 
    // bboxSize
    GvSFMatrix		matrix;	
    // children

private:	

public:


	//! get the transformation matrix 
    void get(Matrix &m);

   	//! get matrix of node , FALSE if node has no matrix 
	GvBool getMatrix(Matrix &m); 

   	//! set matrix of node
	void setMatrix(const Matrix &m) 
	{
			matrix.set(m);
	}
	
    
    //! get matrix, recompute if necessary
    inline Matrix& Transform() { 
			return matrix.get();
    }


	int Do(GTraversal &state);

	int Do(GPathTraversal &state);   

	// apply touchsensors first 
	int Do(GRaySelectTraversal &state);

	int Do(GBBoxTraversal &state);
    int Do(GglRenderTraversal &state);

	int Do(GOptimizeTraversal &state);	// implemented in GOptimizeTraversal.cpp

	
    // notification on field change
    virtual int OnChanged();

};

//
// Gv2Billboard
//
  

class Gv2Billboard : public Gv2Group {

    GV_NODE_HEADER(Gv2Billboard);

public:
    // Fields :

    // bboxCenter
    // bboxSize
    GvSFVec3f		axisOfRotation;	 // default : 0 1 0

	Matrix lastMatrix; //!< to simplify some operation, store last matrix 

	GvBool getMatrix(Matrix &m); // overriden  from GvNode 


	//! get the transformation matrix ( need GGeometryTraversal for viewpoint info) 
    void getMatrix(GGeometryTraversal &state, Matrix &m);
	
	//! compute the matrix from camera world2local transform
	void getMatrix(GCamera *camera,const Matrix &world2local, Matrix &m);

	//! push matrix onto state 
	int ApplyMatrix(GTraversal &state);

	inline int ApplyMatrix(GGeometryTraversal &state) 
	{
		getMatrix(state,lastMatrix);
		state.AppendMatrix(lastMatrix);
		return(1);
	}

	
	int Do(GTraversal &state);

	// need to override Gv2Group virtuals 

	int Do(GPathTraversal &state);
	// apply touchsensors first 
	int Do(GRaySelectTraversal &state);
	int Do(GBBoxTraversal &state);

    int Do(GglRenderTraversal &state);

	int Do(GOptimizeTraversal &state);	// implemented in GOptimizeTraversal.cpp

	// copy on proto instancing
    GvBool computeCopyOnInstance(GvCopyContext &context);

};

//
// GvHUD - Extension
//
  

class GvHUD : public Gv2Group {

    GV_NODE_HEADER(GvHUD);

public:
    // Fields :

    // bboxCenter
    // bboxSize
	// children

	GvMFString align;      //Default ["CENTER" "CENTER"]
	//GvSFNode viewpoint;  // viewpoint to use 

	Matrix lastMatrix; // to simplify some operation, store last matrix 

	GvBool getMatrix(Matrix &m); // overriden  from GvNode 

	// get the transformation matrix   ( need GGeometryTraversal) 
    void getMatrix(GGeometryTraversal &state, Matrix &m);
	
	// compute the matrix from camera world2local transform
	void getMatrix(GCamera *camera,const Matrix &world2local, Matrix &m);

	// push matrix onto state 
	int ApplyMatrix(GTraversal &state);

	inline int ApplyMatrix(GGeometryTraversal &state) 
	{
		getMatrix(state,lastMatrix);
		state.AppendMatrix(lastMatrix);
		return(1);
	}


	
	int Do(GTraversal &state);

	// need to override Gv2Group virtuals 

	int Do(GPathTraversal &state);

	int Do(GRaySelectTraversal &state);
	int Do(GBBoxTraversal &state);

    int Do(GglRenderTraversal &state);

	int Do(GOptimizeTraversal &state);	// implemented in GOptimizeTraversal.cpp

	// copy on proto instancing
    GvBool computeCopyOnInstance(GvCopyContext &context);

};


//
// Gv2Collision
//


class Gv2Collision : public Gv2Group {

    GV_NODE_HEADER(Gv2Collision);

public:
    // Fields :

    // bboxCenter
    // bboxSize
    GvSFBool		collide;	// default : TRUE
    GvSFNode        proxy;      // NULL

    // EventOut
    GvSFTime        collideTime;
    
	// children

	int Do(GRaySelectTraversal &state);   // in GRaySelect.cpp

	int Do(GOptimizeTraversal &state);	// implemented in GOptimizeTraversal.cpp


};


class Gv2Selection : public Gv2Group {

    GV_NODE_HEADER(Gv2Selection);

public:
    // Fields :

    // bboxCenter
    // bboxSize
    GvSFBool		collide;	// default : TRUE
    GvSFBool		select;		// default : TRUE
    GvSFNode        proxy;      // NULL

    
	// children

	int Do(GRaySelectTraversal &state);   // in GRaySelect.cpp

	//int Do(GOptimizeTraversal &state);	// implemented in GOptimizeTraversal.cpp


};




#endif /* _GV2Group_ */
