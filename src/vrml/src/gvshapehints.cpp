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

#ifdef _G_VRML1

#include <gvshapehints.h>

GV_NODE_SOURCE_IMP(GvShapeHints,GvNode);

GvShapeHints::GvShapeHints()
{
    GV_NODE_CONSTRUCTOR(GvShapeHints);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(vertexOrdering);
    GV_NODE_ADD_FIELD(shapeType);
    GV_NODE_ADD_FIELD(faceType);
    GV_NODE_ADD_FIELD(creaseAngle);
    
    vertexOrdering.value = UNKNOWN_ORDERING;
    shapeType.value = UNKNOWN_SHAPE_TYPE;
    faceType.value = CONVEX;
    creaseAngle.value = 0.5;
    
    GV_NODE_DEFINE_ENUM_VALUE(VertexOrdering,	UNKNOWN_ORDERING);
    GV_NODE_DEFINE_ENUM_VALUE(VertexOrdering,	CLOCKWISE);
    GV_NODE_DEFINE_ENUM_VALUE(VertexOrdering,	COUNTERCLOCKWISE);

    GV_NODE_DEFINE_ENUM_VALUE(ShapeType,	UNKNOWN_SHAPE_TYPE);
    GV_NODE_DEFINE_ENUM_VALUE(ShapeType,	SOLID);

    GV_NODE_DEFINE_ENUM_VALUE(FaceType,		UNKNOWN_FACE_TYPE);
    GV_NODE_DEFINE_ENUM_VALUE(FaceType,		CONVEX);

    GV_NODE_SET_SF_ENUM_TYPE(vertexOrdering,	VertexOrdering);
    GV_NODE_SET_SF_ENUM_TYPE(shapeType,		ShapeType);
    GV_NODE_SET_SF_ENUM_TYPE(faceType,		FaceType);
}

GvShapeHints::~GvShapeHints()
{
}

#endif _G_VRML1
