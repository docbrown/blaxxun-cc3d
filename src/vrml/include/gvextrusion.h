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
#ifndef  _GV_Extrusion_
#define  _GV_Extrusion_


#include <gvsfbool.h>
#include <gvmflong.h>
#include <gvmfvec2f.h>
#include <gvmfvec3f.h>
#include <gvmfrotation.h>
#include <gvnodeshell.h>

class GvExtrusion : public GvNodeShell

{

  GV_NODE_HEADER(GvExtrusion);

  public:
  // Fields:
    GvSFBool     beginCap;          //  TRUE
    GvSFBool     ccw;               //  TRUE
    GvSFBool     convex;            //  TRUE
    GvSFFloat    creaseAngle;       //      0

    GvMFVec2f    crossSection;     //  [ 1 1, 1 -1, -1 -1, -1 1, 1 1 ]
    GvSFBool     endCap;           //  TRUE

    GvMFRotation orientation;      //  []
    GvMFVec2f    scale;            //  [1 1]
 
    GvMFVec3f    spine;            //  [ 0 0 0, 0 1 0 ]
    GvSFBool     solid;            //  TRUE

	// is this a solid shape ?
	GvBool Solid() const { return (solid.value); }
	
    // build shell for current state 
    int BuildShell(GTraversal &state);  
	
	int Do(GBBoxTraversal &state);

    int Do(GglRenderTraversal &state);
	int Do(GGeometryTraversal &state);
	int Do(GRaySelectTraversal &state);  

};


#endif /* _GV_Extrusion_ */
