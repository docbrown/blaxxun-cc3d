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
#ifndef  _GvWebFx_
#define  _GvWebFx_

#include <gvsfenum.h>
#include <gvsfbitmask.h>
#include <gvsffloat.h>
#include <gvsfvec2f.h>

#include <gvsfstring.h>
#include <gvmfstring.h>
#include <gvmffloat.h>
#include <gvmfvec3f.h>
#include <gvmfnode.h>
#include <gvsubnode.h>
#include <gvnodeshell.h>

#include <gvmodifier.h>

#include "gcamera.h"

#ifdef _G_VRML1
// all WebFX / Live 3d extension nodes


class GvAxisAlignment : public GvNode 
{

    GV_NODE_HEADER(GvAxisAlignment);

  public:

    enum Alignment {
   	ALIGNAXISXYZ = 0,
    };

    // Fields
    GvSFEnum		alignment;	

  	//
//  	int Do(GTraversal &state);
//  	int Do(GBBoxTraversal &state);
//  	int Do(GglRenderTraversal &state);

};


/*
    BrowserHints {
            fields [SFBool headlight]
            headlight       FALSE
            } 
*/
class GvBrowserHints : public GvNode 
{

    GV_NODE_HEADER(GvBrowserHints);

  public:


    // Fields
    GvSFBool		headlight;	

	// 	int Do(GTraversal &state);

};

/*
    Usage:
        Animator {
            fields [ SFString filename, SFBool loop, SFBool reverse,
                     SFLong startframe, SFLong endframe ]
            filename   "some.vue"
            loop       TRUE
            reverse    FALSE
            startframe -1
            endframe   -1
        }
*/

class GvAnimator : public GvNode 
{

    GV_NODE_HEADER(GvAnimator);

  public:


    // Fields
	GvSFString filename;
	GvSFBool loop;
	GvSFBool reverse;
    GvSFLong startframe;
	GvSFLong endframe;

	// 	int Do(GTraversal &state);
};



/*
   CollideStyle {collide TRUE}
*/

class GvCollideStyle : public GvNode 
{

    GV_NODE_HEADER(GvCollideStyle);

  public:


    // Fields
    GvSFBool		collide;	

	// 	int Do(GTraversal &state);

};

class GvCollisionDetection : public GvNode 
{

    GV_NODE_HEADER(GvCollisionDetection);

  public:


    // Fields
    GvSFBool		collision;	

	// 	int Do(GTraversal &state);

};


class GvSprite : public GvNode //Shell 
{

    GV_NODE_HEADER(GvSprite);

  public:


    // Fields
    GvSFString		texture;

	// QvTexture2 

  	//
//  	int Do(GTraversal &state);
//  	int Do(GBBoxTraversal &state);
//  	int Do(GglRenderTraversal &state);

};




/*

    SpinGroup: a Group Node to spin an object(s) around a specified axis at a given angle. The local field
    allows you to specify whether the center of rotation occurs around the 'SpinGroups' or entire world's
    center. 


    Usage:

        SpinGroup {
            fields [SFRotation rotation, SFBool local]
            rotation 0 0 1  0   # SFRotation
            local    TRUE       # SFBool
        }
*/


class GvSpinGroup : public GvGroup 
{

    GV_NODE_HEADER(GvSpinGroup);

  public:

    // Fields
    GvSFRotation	rotation;	// Rotation
    GvSFBool		local;	

	void Get(Matrix &m,float t) {
	       m = RotationAxisMatrix(Point(rotation.value.axis[0],rotation.value.axis[1],rotation.value.axis[2]), 
	       rotation.value.angle*t);
	}

  	//
	// multiply matrix with top matrix in current state
	int Do(GTraversal &state);

};

class GvSpin : public GvNode 
{

    GV_NODE_HEADER(GvSpin);

  public:

	enum Axes {
		LOCAL = 0,
		GLOBAL = 1
	};

    // Fields
    GvSFString	objectname;	// Rotation
	GvSFFloat xangle;
	GvSFFloat yangle;
	GvSFFloat zangle;

    GvSFEnum	axes;	

  	//
	// multiply matrix with top matrix in current state
	//int Do(GTraversal &state);

};

/*
        DirectedSound {
            fields [MFString name, SFString description, 
                   SFFloat intensity, SFVec3f location, SFVec3f direction, 
                   SFFloat minFrontRange, SFFloat maxFrontRange, 
                   SFFloat minBackRange,  SFFloat maxBackRange,
                   SFBool loop, SFFloat start, SFFloat pause ]
            name          "somefile.wav"
            description   "Directed Sound"
            intensity     1.0
            location      0.0 0.0 0.0
            direction     0.0 0.0 1.0
            minFrontRange 1.0
            maxFrontRange 0.0
            minBackRange  1.0
            maxBackRange  0.0
            loop          TRUE
            start         0.0
            pause         0.0
        }

*/


class GvDirectedSound : public GvNode {

    GV_NODE_HEADER(GvDirectedSound);

  public:
    // Fields
    GvMFString name;
	GvSFString description;
    GvSFFloat intensity; 
	GvSFVec3f location;
	GvSFVec3f direction;
    GvSFFloat minFrontRange;
	GvSFFloat maxFrontRange;
    GvSFFloat minBackRange;
	GvSFFloat maxBackRange;
    GvSFBool loop; 
	GvSFFloat start; 
	GvSFFloat pause;


};


/*

        PointSound {
            fields [MFString name, SFString description, 
                   SFFloat intensity, SFVec3f location,
                   SFFloat minRange, SFFloat maxRange, 
                   SFBool loop, SFFloat start, SFFloat pause ]
            name         "somefile.wav"
            description  "Point Sound"
            intensity    1.0
            location     0 0 0
            minRange     1.0
            maxRange     0.0
            loop         TRUE
            start        0.0
            pause        0.0
        }
*/


class GvPointSound : public GvNode {

    GV_NODE_HEADER(GvPointSound);

  public:
    // Fields
    GvMFString name;
	GvSFString description;
    GvSFFloat intensity; 
	GvSFVec3f location;
    GvSFFloat minRange;
	GvSFFloat maxRange;
    GvSFBool loop; 
	GvSFFloat start; 
	GvSFFloat pause;


};


class GvColorGradient : public GvNode, public GvModifier {

   GV_NODE_HEADER(GvColorGradient);

  public:
    enum Axis {
   		X = 0,
   		Y = 1,
   		Z = 2,
         RADIAL2 =3,
         RADIAL = 4,
         SINXY = 5
    };

	GvMFColor colors;    // a table of colors for cycle through
	GvMFFloat keys;      // optional key frame values, default []
	GvSFEnum  axis;      // the axis or coordinate to use as source parameter, default Y
   
   GvMFNode transform;  // an optional list of transform nodes coordinate
                        // will be transformed first, before color is computed
                        // note: use animated tranform nodes to animate through mapping space

   GvSFBool cycle;      // if true, parametrizations is cylcled by the nodes current time frame. default : false
   GvSFBool global;     // if true, parametrizations is done in global coordinates
	
   GvSFFloat scaleFactor;  // a optional scale factor for input value, default: 1.0
	

	// get color for parameter t
	Point Get(float t); 

   
	int Do(GTraversal &state);
  	int Do(GglRenderTraversal &state);

	virtual GvNode *Apply(GTraversal &state, GvNode * node);


};

#endif


#endif /* _GvWebFx_ */
