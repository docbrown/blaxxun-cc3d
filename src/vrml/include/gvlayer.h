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
#ifndef  _GvLayer_h_
#define  _GvLayer_h_

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

#include <gvgroup.h>


/* HG extension */
class GvLayer : public Gv2Group {

  GV_NODE_HEADER(GvLayer);

  public:

	enum Clear {
		NONE = 0,
		COLOR = GL_COLOR_BUFFER_BIT,
		DEPTH = GL_DEPTH_BUFFER_BIT,
		ACCUM = GL_ACCUM_BUFFER_BIT,
		STENCIL= GL_STENCIL_BUFFER_BIT,
	};

	enum DepthFunc {
		NEVER	= GL_NEVER,
		LESS	= GL_LESS,
		EQUAL	= GL_EQUAL,
		LEQUAL	= GL_LEQUAL,
		GREATER = GL_GREATER,
		NOTEQUAL = GL_NOTEQUAL,
		GEQUAL 	= GL_GEQUAL,
		ALWAYS 	= GL_ALWAYS,

	};
#if 0
	enum AphaFunc {
		NEVER	= GL_NEVER,
		LESS	= GL_LESS,
		EQUAL	= GL_EQUAL,
		LEQUAL	= GL_LEQUAL,
		GREATER = GL_GREATER,
		NOTEQUAL = GL_NOTEQUAL,
		GEQUAL 	= GL_GEQUAL,
		ALWAYS 	= GL_ALWAYS,
	};
#endif
	enum BlendSrcFunc {
		ZERO = GL_ZERO,
		ONE = GL_ONE,
		DST_COLOR = GL_DST_COLOR,
		ONE_MINUS_DST_COLOR = GL_ONE_MINUS_DST_COLOR,

		SRC_ALPHA = GL_SRC_ALPHA,
		ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,

		DST_ALPHA = GL_DST_ALPHA,
		ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,

		SRC_ALPHA_SATURATE = GL_SRC_ALPHA_SATURATE,

	};

#if 0
	enum BlendDestFunc {
		ZERO = GL_ZERO,
		ONE = GL_ONE,
		
		SRC_COLOR = GL_SRC_COLOR,
		ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,

		SRC_ALPHA = GL_SRC_ALPHA,
		ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,

		DST_ALPHA = GL_DST_ALPHA,
		ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,

	};

#endif

    GvSFNode background;
    GvSFNode fog_;
    GvSFNode navigationInfo;


	GvSFNode viewpoint;
	GvSFBool viewpointStretch; // TRUE stretch camera to viewport area

    GvSFBool pick; 		//glEnable/glDisable Picking ; Default : TRUE (if defaultValue use context setting)
    GvSFBitMask clear;  // glClear(clear); // Default : NONE

    GvSFBool depthWrite;// glDepthMask(depthWrite) // Default : FALSE
    GvSFEnum depthFunc; // glDepthFunc(depthFunc) Default : LESS

    GvSFBool lighted; 	//glEnable/glDisable(GL_LIGHTING); Default : TRUE (if defaultValue use context setting)
    GvSFBool fog; 		//glEnable/glDisable(GL_FOG); Default : TRUE (if defaultValue use context setting)

    
	GvSFEnum alphaFunc; // glAlphaFunc(alphaFunc,alphaRef); Default ALWAYS
    GvSFFloat alphaRef; // Default : 0.0
	GvSFEnum alphaComp;

    GvSFBool blend; 	//glEnable(GL_BLEND); Default : FALSE
    GvSFEnum blendSrcFunc; //glBlendFunc(srcFunc,DestFunc); Default : SRC_ALPHA
	GvSFEnum blendDestFunc;  // Default : ONE_MINUS_SRC_ALPHA
	
	GvSFEnum stencilFunc;
	GvSFLong stencilRef;
	GvSFLong stencilMask;	// glStencilMask

	GvSFEnum stencilFail; 	// glStencilOp
	GvSFEnum stencilZFail;
	GvSFEnum stencilZPass;

	// viewport 
	GvSFVec2f		translation;     // Translation default : 0 0
    GvSFInt32		depth;	         // default : 0
    GvSFVec2f		size;	         // default : -1 -1

	GCamera layerCamera;		//!< the camera to use 
	BBox layerBBox;				//!< the layer bbbox 

	//! compute new viewport rect 
	GvBool getViewportRect(const GRectInt &srcViewport, GRectInt &newViewport); 
	
	// compute new camera
	GvBool getCamera(GCamera &newCamera);

	int OnFieldChanged(GvField *field);

	int Do(GTraversal &state);

//  	int Do(GBBoxTraversal &state);
  	int Do(GglRenderTraversal &state);

	int Do(GRaySelectTraversal &state); 




};


//
// GvLayer3D - MPEG 4 
//


class GvLayer3D : public Gv2Group {

    GV_NODE_HEADER(GvLayer3D);

public:
    // Fields :

    // bboxCenter   not including transform effect 
    // bboxSize

    GvMFNode		childrenLayer;	 
    GvSFVec2f		translation;     // Translation default : 0 0
    GvSFInt32		depth;	         // default : 0

    GvSFVec2f		size;	         // default : -1 -1

    GvSFNode		background;
    GvSFNode		fog;
    GvSFNode		navigationInfo;
    GvSFNode		viewpoint;

	// children

    // Event IN
    void addChildrenLayer(GvMFNode *event);
    void removeChildrenLayer(GvMFNode *event);

	GCamera			layerCamera;		//!< the camera to use 
	BBox			layerBBox;				//!< the layer bbbox 

	gbool childrenLayerChanged;

    GvMFNode		childrenLayerSorted;	 
	void SortChildrenLayer();

	//! compute new viewport rect 
	GvBool getViewportRect(const GRectInt &srcViewport, GRectInt &newViewport); 

private:	

public:

	int Do(GTraversal &state);

	int Do(GPathTraversal &state);   

	// apply touchsensors first 
	int Do(GRaySelectTraversal &state);

	int Do(GBBoxTraversal &state);
    int Do(GglRenderTraversal &state);


    // notification on field change
    int OnChanged();
	int OnChildChanged(GvNode *child);
	int OnFieldChanged(GvField *field);

};


#endif _GvLayer_h_

