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
#ifndef  _GV_POINT_SET_
#define  _GV_POINT_SET_

#include <gvsfbool.h>
#include <gvsflong.h>
#include <gvsfint32.h>
#include <gvsffloat.h>
#include <gvsfvec2f.h>
#include <gvsfnode.h>
#include <gvmffloat.h>
#include <gvsubnode.h>
#include <gvnodeshell.h>

#define GV_POINT_SET_USE_REST_OF_POINTS	(-1)
#define GV_FACE_SET_USE_REST_OF_VERTICES (-1)
#define GV_TRI_STRIP_SET_USE_REST_OF_VERTICES (-1)

// class GvNonIndexedShapes

#ifdef _G_VRML1

class GvPointSet : public GvNodeShell {

    GV_NODE_HEADER(GvPointSet);

  public:
    // Fields
    GvSFLong		startIndex;	// Index of 1st coordinate of shape
    GvSFLong		numPoints;	// Number of points to draw

  	// build shell for current state 
  	int BuildShell(GTraversal &state);  
  	int Do(GglRenderTraversal &state);

};

class GvFaceSet : public GvNodeShell {

    GV_NODE_HEADER(GvFaceSet);

  public:
    // Fields
    GvSFLong		startIndex;	// Index of 1st coordinate of shape
    GvMFLong		numVertices;	// Number of vertices per face

  	// build shell for current state 
  	int BuildShell(GTraversal &state);  

};

class GvTriangleStripSet : public GvNodeShell {

    GV_NODE_HEADER(GvTriangleStripSet);

  public:
    // Fields
    GvSFLong		startIndex;	// Index of 1st coordinate of shape
    GvMFLong		numVertices;	// Number of vertices in each strip face

  	// build shell for current state 
  	int BuildShell(GTraversal &state);  

};

class GvQuadMesh : public GvNodeShell {

    GV_NODE_HEADER(GvQuadMesh);

  public:
    // Fields
    GvSFLong		startIndex;	// Index of 1st coordinate of shape
    GvSFLong		verticesPerColumn;	
    GvSFLong		verticesPerRow;	

  	// build shell for current state 
  	int BuildShell(GTraversal &state);  

};

#endif _G_VRML1



#endif /* _GV_POINT_SET_ */
