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
#ifndef  _GV_MATERIAL_
#define  _GV_MATERIAL_
/******************************************************************************
@doc

@module GvMaterial.h - VRML 1.0 & 2.0 Material nodes|

Copyright (c) 1996 - 1998 by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GvMaterial>
<c Gv2Material>

Notes:

Changes:
$Revision: 1.17 $
$Log: gvmaterial.h,v $
Revision 1.17  1999/07/06 16:35:03  tom
*** empty log message ***



Todo :

******************************************************************************/


#include <gvsfcolor.h>
#include <gvmfcolor.h>

#include <gvsffloat.h>
#include <gvmffloat.h>

#include <gvsubnode.h>

// needed for D3D Material Handle
#include "grender.h"

#define GV_SHININESS_GL_SCALE 128.0

#ifdef _G_VRML1


// VRML 1.0
class GvMaterial : public GvNode {

    GV_NODE_HEADER(GvMaterial);

  public:
    // Fields
    GvMFColor		ambientColor;	// Ambient color
    GvMFColor		diffuseColor;	// Diffuse color
    GvMFColor		specularColor;	// Specular color
    GvMFColor		emissiveColor;	// Emissive color
    GvMFFloat		shininess;	// Shininess
    GvMFFloat		transparency;	// Transparency
	
	int maxNumValues;  // maximum lenght of one of the fields

	signed char hasTransparency;
	signed char isEmissiveColorOnly;
    signed char changed;    // set to 1 if any field has changed 

	// material has transparency ??? 
	int HasTransparency();

	// Material has only emissive color channel
	int ComputeIsEmissiveColorOnly();
	
	int IsEmissiveColorOnly() {
 		 if ( isEmissiveColorOnly<0) return (ComputeIsEmissiveColorOnly());
		 else return(isEmissiveColorOnly);
	}

	int ComputeMaxNumValues();
	
	// get length of largest material channel

	int GetMaxNumValues() { if (maxNumValues <0) return ComputeMaxNumValues(); else return(maxNumValues); }
	
	//@cmember push attribute on stack
    int Do(GTraversal &state);					     
	int Do(GBuildShellTraversal &state); 
	int Do(GOptimizeTraversal &state);	// implemented in GOptimizeTraversal.cpp


	// a field has been changed
	int OnFieldChanged(GvField *field);

	// something changed 
	int OnChanged();

#ifdef _OGL

	// set first material to Open GL
	void glSet();
	
	// set nth material 
	void glSet(int i);

	void glSetColor();
	void glSetColor(int i);

	// set diffuse to white
	void glSetDiffuseWhite();
#endif

#ifdef _D3D
     G3MaterialRef handle;
#endif


	void GetColor(int i,Point &color);
	// return color channel for local colors / diffuse / emissive
	GvMFColor * GetLocalColors(); 

	// apply setting to renderstate
	int Apply(GglRenderTraversal &state,int matStatus,int texStatus);

};

#endif _G_VRML1


//
// Gv2Material
//


class Gv2Material : public GvNode {

    GV_NODE_HEADER(Gv2Material);

  public:
    // Fields
    GvSFFloat		ambientIntensity;	// Ambient intensity
    GvSFColor		diffuseColor;	// Diffuse color
    GvSFColor		specularColor;	// Specular color
    GvSFColor		emissiveColor;	// Emissive color
    GvSFFloat		shininess;	// Shininess
    GvSFFloat		transparency;	// Transparency
	
	signed char hasTransparency;
	signed char isEmissiveColorOnly;
    signed char changed;    // set to 1 if any field has changed 

	// material has transparency ??? 
	int HasTransparency();

    // Material has only emissive color channel
	int ComputeIsEmissiveColorOnly();
	
	int IsEmissiveColorOnly() {
 		 if ( isEmissiveColorOnly<0) return (ComputeIsEmissiveColorOnly());
		 else return(isEmissiveColorOnly);
	}

	int checkForDefaultValues(int mode);


//    int Do(GTraversal &state);					     

	// push face color
	int Do(GBuildShellTraversal &state); 

	int Do(GOptimizeTraversal &state);	// implemented in GOptimizeTraversal.cpp


	// a field has been changed
	int OnFieldChanged(GvField *field);

	// something changed 
	int OnChanged();

#ifdef _OGL
	// set material to Open GL
	void glSet(GvBool whiteDiffuse= TRUE);

    // set only diffuse part
	void glSetDiffuse(GvBool whiteDiffuse= TRUE);
	
    // set diffuse to white
	void glSetDiffuseWhite();

    // set diffuse color as current color
	void glSetColor();
#endif

#ifdef _D3D
	G3MaterialRef handle;

	// get the D3D Material handle strucutre 
	G3MaterialRef* GetMaterialHandle() { return(&handle); }

#endif

	//! query if material has any alpha
    GvBool getAlphaStatus(int &matStatus);

    //! get diffuse color 
	void GetColor(Point &color);

	//! apply setting to renderstate
	int Apply(GglRenderTraversal &state,int matStatus,int texStatus);

#ifdef _D3D
	// apply setting to renderstate
	int Apply(GglRenderTraversal &state,G3MaterialRef &handle,int matStatus,int texStatus);
#endif

};


#endif /* _GV_MATERIAL_ */
