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
#ifndef  _GVTimeTransform_
#define  _GVTimeTransform_

#ifdef _G_VRML1


#include <gvsffloat.h>
#include <gvsfenum.h>

#include <gvsubnode.h>

// provide a remapping of current time in stack
//
class GvTimeTransform : public GvNode {

    GV_NODE_HEADER(GvTimeTransform);

  public:
    enum Function {
	TRANSFORM,
	IDENTITY,
	CLAMP,
	CYCLE,
	WRAP,

	MIRROR,
	SIN,
	SQR,
	CUB,
	RND,
	EASE_IN_OUT
    };

    // Fields
    GvSFEnum		function;		// function mapping 
    GvSFFloat		translation;	// Translation vector
    GvSFFloat		scaleFactor;	// Scale factors

	int Do(GTraversal &state);

    virtual  float Transform(GTraversal &state, float t);   

};
#endif _G_VRML1

#endif /* _GVTimeTransform_ */
