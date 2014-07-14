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
#ifndef  _GV_SHAPE_HINTS_
#define  _GV_SHAPE_HINTS_

#ifdef _G_VRML1

#include <gvsfenum.h>
#include <gvsffloat.h>
#include <gvsubnode.h>

class GvShapeHints : public GvNode {

    GV_NODE_HEADER(GvShapeHints);

  public:
    enum VertexOrdering {
	UNKNOWN_ORDERING,
	CLOCKWISE,
	COUNTERCLOCKWISE,
    };

    enum ShapeType {
	UNKNOWN_SHAPE_TYPE,
	SOLID,
    };

    enum FaceType {
	UNKNOWN_FACE_TYPE,
	CONVEX,
    };

    // Fields
    GvSFEnum		vertexOrdering;	// Ordering of face vertices
    GvSFEnum		shapeType;	// Info about shape geometry
    GvSFEnum		faceType;	// Info about face geometry
    GvSFFloat		creaseAngle;	// Smallest angle for sharp edge

	// enable backface culling ?
	GvBool CanCull() { return ((vertexOrdering.value != UNKNOWN_ORDERING) && (shapeType.value == SOLID)); }


  	//@cmember push attribute on stack
    int Do(GTraversal &state);					     
	int Do(GOptimizeTraversal &state);	// implemented in GOptimizeTraversal.cpp

};

#endif _G_VRML1

#endif /* _GV_SHAPE_HINTS_ */
