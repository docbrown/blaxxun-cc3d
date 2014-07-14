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
#ifndef  _GvIndexedTriangleStripSet_
#define  _GvIndexedTriangleStripSet_

class GShell;


#include <gvmflong.h>
#include <gvsubnode.h>

#include <gvnodeshell.h>

//#define GV_END_FACE_INDEX	(-1)

#ifdef _G_VRML1


class GvIndexedTriangleStripSet : public GvNodeShell
//GvNode 
{

    GV_NODE_HEADER(GvIndexedTriangleStripSet);

  public:
    // Fields:
    GvMFLong		coordIndex;		// Coordinate indices
    GvMFLong		materialIndex;		// Material indices
    GvMFLong		normalIndex;		// Surface normal indices
    GvMFLong		textureCoordIndex;	// Texture Coordinate indices



  	// build shell for current state 
  	int BuildShell(GTraversal &state);  

};

#endif _G_VRML1

#endif /* _GvIndexedTriangleStripSet_ */
