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
#ifndef  _GV_DIRECTIONAL_LIGHT_
#define  _GV_DIRECTIONAL_LIGHT_

#include <gvsfbool.h>
#include <gvsfenum.h>

#include <gvsfcolor.h>
#include <gvsffloat.h>
#include <gvsfvec3f.h>
#include <gvsubnode.h>

#ifdef _D3D
	#include "grender.h"
#endif


#ifdef _G_VRML1

// IV Node 
class GvLightModel : public GvNode  {
    GV_NODE_HEADER(GvLightModel);

public:
 	enum LightModel {
			BASE_COLOR, // use only the base (diffuse) object color
			PHONG,		// use Phong lighting model
			
	};
	// Fields
 	GvSFEnum	model;	// light model

	int Do(GTraversal &state);
	int Do(GglRenderTraversal &state);
};

#endif


//
// GvLight - abstract parent node for lights 
// 


class GvLight : public GvNode  {
    GV_NODE_HEADER(GvLight);

public:
    
	// Fields

	GvSFBool		on;			// Default TRUE Whether light is on
	// VRML 2
	GvSFFloat		ambientIntensity; // Default  0 

    GvSFFloat		intensity;	// Default  1.0 intensity (0 to 1)
    GvSFColor		color;		// Default 1 1 1 RGB source color

	//  for PointLight + SpotLight  VRML 
	GvSFVec3f		attenuation;	// 1 0 0 : GL attenuation factors

	GvBool			global;			// flag for treating the light global

	// some settings was changed 
	int changed;

#ifdef _D3D
	// D3D light handle 

	G3LightRef light;
	
	G3LightRef& getLight() { return light; }

#endif

	// set redraw bit if a field changed 
	int OnFieldChanged(GvField *field);


    int Do(GTraversal &state);					     

	// set as light i in glState
	virtual void glSet(int /* i */ ,GglRenderTraversal & /* state */ ) { }

	// set as light i in glState
	virtual GvBool glSet(RenderState *glState,int i,GCamera *camera,const Matrix &m);

	void glSetOff(RenderState *glState);
	
	GvBool			isRegistered; // TRUE if node registered in Scene information


};

//
// GvDirectionalLight
// 


class GvDirectionalLight : public GvLight {

    GV_NODE_HEADER(GvDirectionalLight);

  public:
    // Fields
	GvSFVec3f		direction;	// Default : 0 0 -1 Illumination direction vector

	Point			currentDirection; // internal : last global direction

	//
	int Do(GglRenderTraversal &state); 

	//! set as light i in glState
	virtual void glSet(int i,GglRenderTraversal &state);

	//! set as light i in glState
	virtual GvBool glSet(RenderState *glState,int i,GCamera *camera,const Matrix &m);



};

#endif /* _GV_DIRECTIONAL_LIGHT_ */
