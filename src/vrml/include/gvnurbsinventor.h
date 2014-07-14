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
#ifndef __GvNurbsSurfaceInventor
#define  __GvNurbsSurfaceInventor

/******************************************************************************
@doc

@module Gvnurbsinventor.h - Inventor NURBS nodes|

Copyright (c) 1999 by blaxxun interactive
All rights reserved

Purpose:

Classes:
<c GvNurbsSurfaceInventor>
	NurbsCurve
	LinearProfile
	NurbsProfile

  states:
	GvProfileCoordinate2
	GvProfileCoordinate3
	GvCoordinate4  
	
...

  
Notes:
	if the parser works on a non-vrml2 file it assumes the nurbssurface to be 
	open inventor (gvnode.cpp: createinstancefromnode())

	gvelement.h: stackableatrribute is extendend to support MFNodeField
	for multiple trimming curves in the state
	
	  two new stacks:
		GvProfileCurveElement: stack for LinearProfile and NurbsProfile
		GvProfileCoordElement: stack for the profile coords (ProfileCoord2/3)

		coordinate4 is pushed on the coordinate3 stack!
		better: rename gvcoordinate3element!
  

Changes:

Todo :

  primitives:
	IndexedNurbsCurve
	IndexedNurbsSurface

  extra:
	complexity

	clean up nodes???
	remove curve/add dummy curve

******************************************************************************/


#include <gvsubnode.h>
#include <gvsfvec3f.h>
#include <gvmfvec3f.h>
#include <gvsfbool.h>

#include <gvsffloat.h>
#include <gvmffloat.h>

#include <gvsfnode.h>
#include <gvmfnode.h>

#include <gvsfint32.h>
#include <gvmfint32.h>
#include <gvmfstring.h>
#include <gvmfvec2f.h>
#include <gvmfcolor.h>

#include <gvnodeshell.h>

// needed for D3D Material Handle
#include "grender.h"
#include "gnurbs.h"

#include "gv2group.h"
#include "gvinterpolators.h"
#include "gnurbsutil.h"

#define GVNURBS_MAX_ORDER 16


#ifdef _G_INVENTOR


class GvNurbsProfile;
class GvNurbsCurve2D;
class GvPolyline2D;
class GvLinearProfile;
class GvNurbsSurface;
class GvContour2D;
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// 
//				T H E   N U R B S S U R F A C E				
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

class GvNurbsSurfaceInventor : public GvNode {

    GV_NODE_HEADER(GvNurbsSurfaceInventor);

  public:
    // Fields
	
	//inventor fields
    GvSFInt32		numUControlPoints;	
    GvSFInt32		numVControlPoints;	

	GvMFFloat		uKnotVector;
	GvMFFloat		vKnotVector;

    //members
	GvSFNode   child;       
	

    int Do(GTraversal &state);
    int Do(GglRenderTraversal &state);

	int Do(GOptimizeTraversal &state);	


	gbool m_bInitialized, m_bBadValues;

	int BuildNode(GTraversal &state);

	//GvNurbsCurve2D*	BuildNurbsCurve2D(GTraversal &state, GvNurbsProfile *mNurbsProfile);
	
	gbool BuildNurbsCurve2D(GTraversal &state, GvContour2D* currentContour, GvNurbsProfile *mNurbsProfile);
	
	GvNurbsSurface*	BuildNurbsSurface(GTraversal &state);
	
	GvPolyline2D* BuildPolyline2D(GTraversal &state, GvLinearProfile *mLinearProfile);

};





//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// 
//					T H E   N U R B S C U R V E					//
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/


class GvNurbsCurveInventor : public GvNode {

    GV_NODE_HEADER(GvNurbsCurveInventor);

  public:
	//inventor fields
	GvSFInt32  numControlPoints;
	GvMFFloat  knotVector;//             []     

    //members
	GvSFNode   child;       
		  	
    int Do(GTraversal &state);

    // build shell for current state 
  	int BuildNode(GTraversal &state);  

	BOOL m_bInitialized, m_bBadValues;
};





//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// 
//				 NurbsProfile	
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/


class GvNurbsProfile : public GvNode {

    GV_NODE_HEADER(GvNurbsProfile);

  public:
    // Fields
	GvMFInt32		index;       
	GvSFEnum		linkage; 
	GvMFFloat		knotVector;//             []     


	enum linkage_type {
		START_FIRST,
		START_NEW,
		ADD_TO_CURRENT
    };


  	//! push attribute on stack
    int Do(GTraversal &state);					     
	int Do(GOptimizeTraversal &state);	// implemented in GOptimizeTraversal.cpp

	GvBool changed;					
	

	BOOL m_bInitialized, m_bBadValues;
};



//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// 
//				LinearProfile	
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/


class GvLinearProfile : public GvNode {

    GV_NODE_HEADER(GvLinearProfile);

  public:
    // Fields
	GvMFInt32		index;       
	GvSFEnum		linkage; 

	enum linkage_type {
		START_FIRST,
		START_NEW,
		ADD_TO_CURRENT
    };


  	//! push attribute on stack
    int Do(GTraversal &state);					     
	int Do(GOptimizeTraversal &state);	// implemented in GOptimizeTraversal.cpp

	GvBool changed;					
	

	BOOL m_bInitialized, m_bBadValues;
};












//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// 
//						states
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/



//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// 
//						ProfileCoordinate2
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

class GvProfileCoordinate2 : public GvNode {

    GV_NODE_HEADER(GvProfileCoordinate2);

  public:
    // Fields
    GvMFVec2f		point;		// Coordinate point(s)

    
  	//@cmember push attribute on stack
    int Do(GTraversal &state);					     
	int Do(GOptimizeTraversal &state);	// implemented in GOptimizeTraversal.cpp

};


//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// 
//						ProfileCoordinate3
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

class GvProfileCoordinate3 : public GvNode {

    GV_NODE_HEADER(GvProfileCoordinate3);

  public:
    // Fields
    GvMFVec3f		point;		// Coordinate point(s)

    
  	//@cmember push attribute on stack
    int Do(GTraversal &state);					     
	int Do(GOptimizeTraversal &state);	// implemented in GOptimizeTraversal.cpp

};




//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// 
//						GvCoordinate4
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

class GvCoordinate4 : public GvNode {

    GV_NODE_HEADER(GvCoordinate4);

  public:
    // Fields
    GvMFVec4f		point;		// Coordinate point(s)
    
  	//! push attribute on stack
    int Do(GTraversal &state);	
	int Do(GOptimizeTraversal &state);	// implemented in GOptimizeTraversal.cpp
	

};




#endif _G_INVENTOR
#endif __GvNurbsSurfaceInventor


