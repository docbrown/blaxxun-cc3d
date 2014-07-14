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
#ifndef  _GV_WWW_ANCHOR_
#define  _GV_WWW_ANCHOR_


#include <gvsfstring.h>
#include <gvmfstring.h>
#include <gv2group.h>

#ifdef _G_VRML1


#include <gvsfenum.h>
#include <gvsfvec3f.h>

#include <gvgroup.h>
#include <gvseparator.h>


// derivation from Separator, was Group before
class GvWWWAnchor : public GvSeparator {

    GV_NODE_HEADER(GvWWWAnchor);

  public:

    enum Map {			// Map types:
	NONE,				// Leave URL name alone
	POINT,				// Add object coords to URL name
    };

    // Fields
    GvSFString		name;		// URL name
    GvSFString		description;// URL description // extension to 1.0
    GvSFEnum		map;		// How to map pick to URL name

    // non standard
    GvSFString      target; // target frame ?? Live3d hack ? "topFrame"
    GvSFString      mode; // Inventor 2.1 field ???? e.g. OFF

	GvString homeUrl; // the url of parent, used for resolving anchors 
	GvBool homeUrlInit; // is the home url setup ?
	const char * getHomeUrl()  { return homeUrl; }

	
    int Do(GTraversal &state);  
	int Do(GLoadInlinesTraversal &state);
	
	int Do(GPathTraversal &state);
	int Do(GglRenderTraversal &state);
	int Do(GRaySelectTraversal &state); // implemented in GRaySelect.cpp
	//int Do(GioTraversal &state);
	int Do(GDisplayTreeTraversal &state); // implemented in GDisplayTree.cpp
	int Do(GOptimizeTraversal &state);	// implemented in GOptimizeTraversal.cpp


};

/* HG extension
  Automatically load Anchor if camera is in box
*/

class GvWWWRegionAnchor : public GvWWWAnchor {

    GV_NODE_HEADER(GvWWWRegionAnchor);

  public:


    //Derived  Fields
    //GvSFString		name;		// URL name
    //GvSFString		description;// URL description // extension to 1.0
    //GvSFEnum		map;		// How to map pick to URL name

    // Fields

    GvSFVec3f		bboxSize;	// Size of 3D bounding box
    GvSFVec3f		bboxCenter;	// Center of 3D bounding box
	
	// is the bounding box empty ????
	int bboxIsEmpty() { return ((bboxSize[0]<=0.0) && (bboxSize[1]<=0.0) && (bboxSize[2]<=0.0)); }


  	int Do(GTraversal &state);
	
	// do boundingbox 
	int Do(GBBoxTraversal &state);
	int Do(GglRenderTraversal &state);

	//void Traverse(GTraversal &state);

	int loaded;
	int cannotLoad;
	
	GvString absName; // url in absolute form


};


#endif _G_VRML1


// VRML 2
/*
Anchor {
  eventIn      MFNode   addChildren
  eventIn      MFNode   removeChildren
  exposedField MFNode   children        []
  exposedField SFString description     "" 
  exposedField MFString parameter       []
  exposedField MFString url             []
  field        SFVec3f  bboxCenter      0 0 0
  field        SFVec3f  bboxSize        -1 -1 -1
}
*/

class Gv2Anchor : public Gv2Group {

    GV_NODE_HEADER(Gv2Anchor);

public:
    // Fields :

    // bboxCenter
    // bboxSize
    GvSFString		description;    // ""
    GvMFString		parameter;      // []
    GvMFString		url;            // []
    // children

	GvString homeUrl; // the url of parent, used for resolving anchors 
	GvBool homeUrlInit; // is the home url setup ?
	const char * getHomeUrl()  { return homeUrl; }

	// put node in state  GvWWWAnchorElement
    int Do(GTraversal &state);   	
    int Do(GPathTraversal &state);   	
	int Do(GLoadInlinesTraversal &state);

	int Do(GRaySelectTraversal &state);
	int Do(GDisplayTreeTraversal &state); 
	int Do(GOptimizeTraversal &state);	// implemented in GOptimizeTraversal.cpp



	// don´t put node in state, call Gv2Group::Do(GglRenderTraversal)
	int Do(GglRenderTraversal &state);

};


#endif /* _GV_WWW_ANCHOR_ */
