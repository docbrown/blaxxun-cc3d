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
#ifndef  _GvAsciiText_
#define  _GvAsciiText_

#include <gvsfenum.h>
#include <gvsfbitmask.h>
#include <gvsffloat.h>
#include <gvsfvec2f.h>

#include <gvsfstring.h>
#include <gvmfstring.h>
#include <gvmffloat.h>
#include <gvmfvec3f.h>
#include <gvsubnode.h>
#include <gvnodeshell.h>


#ifdef _G_VRML1

class GvAsciiText : public GvNodeShell 
{

    GV_NODE_HEADER(GvAsciiText);

  public:

    enum Justification {
	LEFT	= 0,
	CENTER	= 1,
	RIGHT	= 2	
    };

    enum Part {			// Cylinder parts
	SIDES	= 0x01,			// The tubular part
	TOP	= 0x02,			// The top circular face
	BOTTOM	= 0x04,			// The bottom circular face
	INVERT	= 0x08,			// inverted 
	ALL	= 0x07,			// All parts
    };

    // Fields
    GvMFString		string;		
    GvSFFloat		spacing;
    GvSFEnum		justification;	
    GvMFFloat		width;
	
	GvMFVec3f		extrusion;
    GvSFBitMask		parts;		// Visible parts of extrusion
	GvMFFloat		bevel;
	GvSFVec2f		invertScale;	// size of invertion


	int BuildShell(GTraversal &state);
	int Do(GBBoxTraversal &state);


};

#endif

#endif /* _GvAsciiText_ */
