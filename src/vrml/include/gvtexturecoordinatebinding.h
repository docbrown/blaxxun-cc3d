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
#ifndef  _GvTextureCoordinateBinding_
#define  _GvTextureCoordinateBinding_

#ifdef _G_VRML1

#include <gvsfenum.h>
#include <gvsubnode.h>

class GvTextureCoordinateBinding : public GvNode {

    GV_NODE_HEADER(GvTextureCoordinateBinding);

  public:
    enum Binding {
	DEFAULT,
	PER_VERTEX,
	PER_VERTEX_INDEXED,
    };

    // Fields:
    GvSFEnum		value;	
    
    //@cmember push attribute on stack
    int Do(GTraversal &state);					     
	int Do(GOptimizeTraversal &state);	// implemented in GOptimizeTraversal.cpp

		
};

#endif _G_VRML1

#endif /* _GvTextureCoordinateBinding_ */
