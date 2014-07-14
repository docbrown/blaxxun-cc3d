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
#ifndef  _GV_INFO_
#define  _GV_INFO_

#include <gvsfstring.h>
#include <gvmfstring.h>
#include <gvsffloat.h>
#include <gvsftime.h>
#include <gvsfcolor.h>
#include <gvsfvec3f.h>
#include <gvmffloat.h>
#include <gvsfbool.h>
#include <gvmfcolor.h>
#include <gvsfnode.h>


#include <gvsubnode.h>

class GTraversal;

class GView;

#ifdef _G_VRML1



class GvInfo : public GvNode {

    GV_NODE_HEADER(GvInfo);

  public:
    // Fields
    GvSFString		string;		// Info string
	int Do(GTraversal &state);
	int GvInfo::Do(GOptimizeTraversal &state);


};

#endif _G_VRML1


class GvWorldInfo : public GvNode {

    GV_NODE_HEADER(GvWorldInfo);

  public:
    // Fields
    GvMFString		_info;  // Default [] // to do: info is from RT 
    GvSFString      title; // Default : ""

	// int Do(GTraversal &state);

};


class GvNavigationInfo : public GvNode {

    GV_NODE_HEADER(GvNavigationInfo);

  public:
    // Fields
    //  eventIn      SFBool   set_bind
    void set_bind(GvSFBool *bind);

    GvMFFloat  avatarSize;       // Default :[ 0.25, 1.6, 0.75 ]
    GvSFBool   headlight;        // Default :TRUE
    GvSFFloat  speed;            // Default :1.0 
    GvMFString type;             // Default :"WALK" 
    GvSFFloat  visibilityLimit;  // Default :0.0 
    
    // Event out
    GvSFBool   isBound;

	int Apply(GView *view);

};


class GvBackground : public GvNode {

    GV_NODE_HEADER(GvBackground);
  public:
    // Fields

    // eventIn      SFBool   set_bind
    void set_bind(GvSFBool *bind);

    GvMFFloat  groundAngle;  // Default :[]
    GvMFColor  groundColor;  // Default :[]
    GvMFString backUrl;      // Default :[]
    GvMFString bottomUrl;    // Default :[]
    GvMFString frontUrl;     // Default :[]
    GvMFString leftUrl;      // Default :[]
    GvMFString rightUrl;     // Default :[]
    GvMFString topUrl;       // Default :[]
    GvMFFloat  skyAngle;     // Default :[]
    GvMFColor  skyColor;     // Default :[ 0 0 0 ]

    // Event out
    GvSFBool   isBound;

	// a field has been changed check, for redraw
	int OnFieldChanged(GvField *field);

	
	int Apply(GView *view);

};


class GvFog : public GvNode {

    GV_NODE_HEADER(GvFog);

  public:
    // Fields

      //  eventIn      SFBool   set_bind
    void set_bind(GvSFBool *bind);

    GvSFColor   color;          // Default : 1 1 1
    GvSFString  fogType;        // Default : "LINEAR" 
    GvSFFloat   visibilityRange;  // Default :0 
    
    // Event out
    GvSFBool   isBound;

	// a field has been changed check, for redraw
	int OnFieldChanged(GvField *field);

	int Apply(GView *view);

};

class GvFog2 : public GvFog {

    GV_NODE_HEADER(GvFog2);

  public:
    // Fields

    // inherited from Fog

    //  eventIn      SFBool   set_bind
    //void set_bind(GvSFBool *bind);

    //GvSFColor   color;          // Default : 1 1 1
    //GvSFString  fogType;        // Default : "LINEAR" 
    //GvSFFloat   visibilityRange;  // Default :0 
    
    // Event out
    //GvSFBool   isBound;

	// a field has been changed check, for redraw
	//int OnFieldChanged(GvField *field);

	//int Apply(GView *view);

	 GvSFFloat   visibilityStart;  // Default :0 
	 GvSFFloat	 density;		   // defaul 1.0

};





#endif /* _GV_INFO_ */
