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
#ifndef  _GV_PENDULUM_
#define  _GV_PENDULUM_
// HG 

#include <gvsfrotation.h>
#include <gvsffloat.h>
#include <gvsfbool.h>
#include <gvrotation.h>
#include <gvsubnode.h>

class GvPendulum : public GvRotation {

    GV_NODE_HEADER(GvPendulum);

  public:
    // Fields from GvRotation
    //GvSFRotation	rotation;	// Rotation

    GvSFRotation	rotation0;	// Rotation0
    GvSFRotation	rotation1;	// Rotation1

	GvSFFloat speed;
	GvSFBool  on;
	GvSFFloat offset;

   // get type class of node
   virtual int GetNodeType();

	void Get(Matrix &m);
	void Get(Matrix &m,float t);


	// multiply matrix with top matrix in current state
	int Do(GTraversal &state);   
};

#endif /* _GV_PENDULUM_ */
