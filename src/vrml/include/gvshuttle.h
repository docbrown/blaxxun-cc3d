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
#ifndef  _GvShuttle_
#define  _GvShuttle_

#include <gvsfvec3f.h>
#include <gvsffloat.h>
#include <gvsfbool.h>
#include <gvtranslation.h>
#include <gvsubnode.h>

#ifdef _G_VRML1
class GvShuttle : public GvTranslation {

    GV_NODE_HEADER(GvShuttle);

  public:
    // Fields from GvTranslation
    // GvSFVec3f		translation;	// Translation vector

    // Fields
    GvSFVec3f		translation0;	// Translation vector 0
    GvSFVec3f		translation1;	// Translation vector 1
	GvSFFloat		speed;
	GvSFBool		on;

	void Get(Matrix &m);
	void Get(Matrix &m,float t);

	// multiply matrix with top matrix in current state
	int Do(GTraversal &state);   

};

#endif _G_VRML1

#endif /* _GvShuttle_ */
