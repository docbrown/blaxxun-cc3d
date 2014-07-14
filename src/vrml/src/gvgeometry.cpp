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

@module GvGeometry.cpp - GLView VRML 1.0 Geometry nodes definition|

Copyright (c) 1995 - 1997	by Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:

Changes:

$Revision: 1.19 $
$Log: gvgeometry.cpp,v $
Revision 1.19  1999/07/06 16:45:44  tom
*** empty log message ***



Todo :

******************************************************************************/

#include "stdafx.h"

#include <gvtraverse.h>
// for ElevationGrid
#include <gvmaterial.h>
#include <gv2geometry.h>
#include <grender.h>

#include "gshell.h"

#include "grayselect.h"



// all  VRML 1.0 Geometry nodes together

#ifdef _G_VRML1

//
// GvAsciiText

#include <gvasciitext.h>

GV_NODE_SOURCE_IMP(GvAsciiText,GvNodeShell);

GvAsciiText::GvAsciiText()
{
    GV_NODE_CONSTRUCTOR(GvAsciiText);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(string);
    GV_NODE_ADD_FIELD(spacing);
    GV_NODE_ADD_FIELD(justification);
    GV_NODE_ADD_FIELD(width);
    GV_NODE_ADD_FIELD(extrusion);
    GV_NODE_ADD_FIELD(parts);
    GV_NODE_ADD_FIELD(bevel);
    GV_NODE_ADD_FIELD(invertScale);

    string.values[0] = "";
    spacing.value = 1;
    justification.value = LEFT;
    width.values[0] = 0.0;

    GV_NODE_DEFINE_ENUM_VALUE(Justification, LEFT);
    GV_NODE_DEFINE_ENUM_VALUE(Justification, CENTER);
    GV_NODE_DEFINE_ENUM_VALUE(Justification, RIGHT);

    GV_NODE_SET_SF_ENUM_TYPE(justification, Justification);
	    parts.value = ALL;
    
	invertScale.value[0] = 2.0;
	invertScale.value[1] = 2.0;
	bevel.values[0]=0.0;

    GV_NODE_DEFINE_ENUM_VALUE(Part, SIDES);
    GV_NODE_DEFINE_ENUM_VALUE(Part, TOP);
    GV_NODE_DEFINE_ENUM_VALUE(Part, BOTTOM);
    GV_NODE_DEFINE_ENUM_VALUE(Part, INVERT);
    GV_NODE_DEFINE_ENUM_VALUE(Part, ALL);

    GV_NODE_SET_SF_ENUM_TYPE(parts, Part);


}

GvAsciiText::~GvAsciiText()
{
}


// GvCone.cpp

#include <gvcone.h>

GV_NODE_SOURCE_IMP(GvCone,GvNodeShell);

GvCone::GvCone()
{
    GV_NODE_CONSTRUCTOR(GvCone);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(parts);
    GV_NODE_ADD_FIELD(bottomRadius);
    GV_NODE_ADD_FIELD(height);

    parts.value = ALL;
    bottomRadius.value = 1.0;
    height.value = 2.0;

    GV_NODE_DEFINE_ENUM_VALUE(Part, SIDES);
    GV_NODE_DEFINE_ENUM_VALUE(Part, BOTTOM);
    GV_NODE_DEFINE_ENUM_VALUE(Part, ALL);

    GV_NODE_SET_SF_ENUM_TYPE(parts, Part);
}

GvCone::~GvCone()
{
}

// GvCube.h


#include <gvcube.h>

GV_NODE_SOURCE_IMP(GvCube,GvNodeShell);

GvCube::GvCube()
{
    GV_NODE_CONSTRUCTOR(GvCube);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(width);
    GV_NODE_ADD_FIELD(height);
    GV_NODE_ADD_FIELD(depth);

    width.value = 2.0;
    height.value = 2.0;
    depth.value = 2.0;
}

GvCube::~GvCube()
{
}


#include <gvcylinder.h>

GV_NODE_SOURCE_IMP(GvCylinder,GvNodeShell);

GvCylinder::GvCylinder()
{
    GV_NODE_CONSTRUCTOR(GvCylinder);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(parts);
    GV_NODE_ADD_FIELD(radius);
    GV_NODE_ADD_FIELD(height);

    parts.value = ALL;
    radius.value = 1.0;
    height.value = 2.0;

    GV_NODE_DEFINE_ENUM_VALUE(Part, SIDES);
    GV_NODE_DEFINE_ENUM_VALUE(Part, TOP);
    GV_NODE_DEFINE_ENUM_VALUE(Part, BOTTOM);
    GV_NODE_DEFINE_ENUM_VALUE(Part, ALL);

    GV_NODE_SET_SF_ENUM_TYPE(parts, Part);
}

GvCylinder::~GvCylinder()
{
}

//GvIndexedFaceSet.cpp

#include <gvindexedfaceset.h>

GV_NODE_SOURCE_IMP(GvIndexedFaceSet,GvNodeShell);

GvIndexedFaceSet::GvIndexedFaceSet()
{
    GV_NODE_CONSTRUCTOR(GvIndexedFaceSet);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(coordIndex);
    GV_NODE_ADD_FIELD(materialIndex);
    GV_NODE_ADD_FIELD(normalIndex);
    GV_NODE_ADD_FIELD(textureCoordIndex);

    coordIndex.values[0] = 0;
    materialIndex.values[0] = GV_END_FACE_INDEX;
    normalIndex.values[0] = GV_END_FACE_INDEX;
    textureCoordIndex.values[0] = GV_END_FACE_INDEX;
}

GvIndexedFaceSet::~GvIndexedFaceSet()
{
}


//GvIndexedLineSet.cpp

#include <gvindexedlineset.h>

GV_NODE_SOURCE_IMP(GvIndexedLineSet,GvNodeShell);

GvIndexedLineSet::GvIndexedLineSet()
{
    GV_NODE_CONSTRUCTOR(GvIndexedLineSet);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(coordIndex);
    GV_NODE_ADD_FIELD(materialIndex);
    GV_NODE_ADD_FIELD(normalIndex);
    GV_NODE_ADD_FIELD(textureCoordIndex);

    coordIndex.values[0] = 0;
    materialIndex.values[0] = GV_END_LINE_INDEX;
    normalIndex.values[0] = GV_END_LINE_INDEX;
    textureCoordIndex.values[0] = GV_END_LINE_INDEX;
}

GvIndexedLineSet::~GvIndexedLineSet()
{
}

// IndexedTriangleStripSet.cpp
#include <gvindexedtrianglestripset.h>

GV_NODE_SOURCE_IMP(GvIndexedTriangleStripSet,GvNodeShell);

GvIndexedTriangleStripSet::GvIndexedTriangleStripSet()
{
    GV_NODE_CONSTRUCTOR(GvIndexedTriangleStripSet);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(coordIndex);
    GV_NODE_ADD_FIELD(materialIndex);
    GV_NODE_ADD_FIELD(normalIndex);
    GV_NODE_ADD_FIELD(textureCoordIndex);

    coordIndex.values[0] = 0;
    materialIndex.values[0] = GV_END_FACE_INDEX;
    normalIndex.values[0] = GV_END_FACE_INDEX;
    textureCoordIndex.values[0] = GV_END_FACE_INDEX;
}

GvIndexedTriangleStripSet::~GvIndexedTriangleStripSet()
{
}


//GvPointSet.cpp
#include <gvpointset.h>

GV_NODE_SOURCE_IMP(GvPointSet,GvNodeShell);

GvPointSet::GvPointSet()
{
    GV_NODE_CONSTRUCTOR(GvPointSet);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(startIndex);
    GV_NODE_ADD_FIELD(numPoints);

    startIndex.value = 0;
    numPoints.value = GV_POINT_SET_USE_REST_OF_POINTS;
}

GvPointSet::~GvPointSet()
{
}


GV_NODE_SOURCE_IMP(GvFaceSet,GvNodeShell);

GvFaceSet::GvFaceSet()
{
    GV_NODE_CONSTRUCTOR(GvFaceSet);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(startIndex);
    GV_NODE_ADD_FIELD(numVertices);

    startIndex.value = 0;
    numVertices.values[0] = GV_FACE_SET_USE_REST_OF_VERTICES;
}

GvFaceSet::~GvFaceSet()
{
}


GV_NODE_SOURCE_IMP(GvTriangleStripSet,GvNodeShell);


GvTriangleStripSet::GvTriangleStripSet()
{
    GV_NODE_CONSTRUCTOR(GvTriangleStripSet);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(startIndex);
    GV_NODE_ADD_FIELD(numVertices);

    startIndex.value = 0;
    numVertices.values[0] = GV_TRI_STRIP_SET_USE_REST_OF_VERTICES;
}

GvTriangleStripSet::~GvTriangleStripSet()
{
}


// GvQuadMesh

GV_NODE_SOURCE_IMP(GvQuadMesh,GvNodeShell);

GvQuadMesh::GvQuadMesh()
{
    GV_NODE_CONSTRUCTOR(GvQuadMesh);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(startIndex);
    GV_NODE_ADD_FIELD(verticesPerColumn);
    GV_NODE_ADD_FIELD(verticesPerRow);

    startIndex.value = 0;
	verticesPerColumn.value = 1;
	verticesPerRow.value = 1;

}

GvQuadMesh::~GvQuadMesh()
{
}





#include <gvsphere.h>

GV_NODE_SOURCE_IMP(GvSphere,GvNodeShell);

GvSphere::GvSphere()
{
    GV_NODE_CONSTRUCTOR(GvSphere);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(radius);

    radius.value = 1.0;
}

GvSphere::~GvSphere()
{
}

#endif _G_VRML1

