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
#ifndef  _GV_TRANSFORM_
#define  _GV_TRANSFORM_

#include <gvsfrotation.h>
#include <gvsfvec3f.h>
#include <gvsubnode.h>

#include <gvmfstring.h>


#ifdef _G_VRML1

class GvTransform : public GvNode {

    GV_NODE_HEADER(GvTransform);

  public:
    // Fields
    GvSFVec3f		translation;	 // Translation vector
    GvSFRotation	rotation;	 // Rotation
    GvSFVec3f		scaleFactor;	 // Scale factors
    GvSFRotation	scaleOrientation;// Defines rotational space for scale
    GvSFVec3f		center;	         // Center point for scale and rotate


	Matrix 	cache;
	int cached; // if 1 cache matrix has been computed from above values


	int Do(GTraversal &state);
	int Do(GOptimizeTraversal &state);	// implemented in GOptimizeTraversal.cpp

	
	int isDefault() { return(0); }

	void Get(Matrix &m);

	virtual int OnChanged();

	// get matrix of node , FALSE if node has no matrix 
	GvBool getMatrix(Matrix &m);


};
#endif


#endif /* _GV_TRANSFORM_ */
