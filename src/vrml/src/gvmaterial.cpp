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
#include "stdafx.h"

/******************************************************************************
@doc

@module GvMaterial.cpp - Implementation of VRML 1.0 +20.0 Material Nodes|

Copyright (c) 1995- 1998	by Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:


Changes:

$Revision: $
$Log: $


Todo :

******************************************************************************/


#include "grender.h"


#include <gvtraverse.h>
#include <gvmaterial.h>
#include <gvtexture2.h>

#ifdef _G_VRML1



//
// GvMaterial
//
GV_NODE_SOURCE_IMP(GvMaterial,GvNode);

GvMaterial::GvMaterial()
{
    GV_NODE_CONSTRUCTOR(GvMaterial);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(ambientColor);
    GV_NODE_ADD_FIELD(diffuseColor);
    GV_NODE_ADD_FIELD(specularColor);
    GV_NODE_ADD_FIELD(emissiveColor);
    GV_NODE_ADD_FIELD(shininess);
    GV_NODE_ADD_FIELD(transparency);

    ambientColor.values[0]=ambientColor.values[1]=ambientColor.values[2] = 0.2;
    diffuseColor.values[0]=diffuseColor.values[1]=diffuseColor.values[2] = 0.2;
    specularColor.values[0]=specularColor.values[1]=specularColor.values[2]=0.;
    emissiveColor.values[0]=emissiveColor.values[1]=emissiveColor.values[2]=0.;
    shininess.values[0] = 0.2;
    transparency.values[0] = 0.0;
    
    maxNumValues = -1;  // maximum lenght of one of the fields
	hasTransparency = -1;
	isEmissiveColorOnly = -1;
    changed = 1;

}

GvMaterial::~GvMaterial()
{					 
}

int GvMaterial::HasTransparency()
{
 	if (hasTransparency <0) {
		hasTransparency = (transparency.Length() >0 )&&  (transparency.values[0] > 0.0);
	}
 	return(hasTransparency);
}

int GvMaterial::ComputeIsEmissiveColorOnly()
{
 	if ( isEmissiveColorOnly<0) {
		  isEmissiveColorOnly=0;
		  
		  if (emissiveColor.Length() > 0) {
			 if ((emissiveColor.Length() == 1) && emissiveColor.values[0]>=0.99 && 
		 		  emissiveColor.values[1]>=0.99 && 
				  emissiveColor.values[2]>=0.99) {
		 			isEmissiveColorOnly=1;

			 } 

			if ((ambientColor.Length() == 0) || (ambientColor.Length() == 1 && ambientColor.values[0]==0.0 && ambientColor.values[1]==0.0 &&
	      		ambientColor.values[2]== 0.0)) 
			{
			if ((diffuseColor.Length() == 0) || (diffuseColor.Length()  == 1 && diffuseColor.values[0]==0.0 && diffuseColor.values[1]==0.0 &&
	      		diffuseColor.values[2]== 0.0)) 

		 		isEmissiveColorOnly=1;
			}
		  }
	}
 	return(isEmissiveColorOnly);

}

int GvMaterial::ComputeMaxNumValues()
{ int m=0;

  m = max(m,ambientColor.Length());
  m = max(m,diffuseColor.Length());
  m = max(m,specularColor.Length());
  m = max(m,emissiveColor.Length());
  m = max(m,shininess.Length());
  m = max(m,transparency.Length());
  maxNumValues = m;
  return(m);
}

int GvMaterial::OnFieldChanged(GvField *field)
{
	isEmissiveColorOnly = -1;
	hasTransparency = -1;
	maxNumValues = -1;

	OnChanged(); // ??

	return(1);
}


int GvMaterial::OnChanged()
{

  ComputeMaxNumValues();
  changed = 1;

  return(1);
}


#ifdef _OGL

void GvMaterial::glSet()
{
	float v[4];
	float a = 1.0-transparency[0];

	GLenum face = GL_FRONT_AND_BACK;

	glMaterialfv(face,GL_AMBIENT,ambientColor.GetRGBA(v,a));
	glMaterialfv(face,GL_DIFFUSE,diffuseColor.GetRGBA(v,a));
	glMaterialfv(face,GL_SPECULAR,specularColor.GetRGBA(v,a));
	glMaterialfv(face,GL_EMISSION,emissiveColor.GetRGBA(v,a));
	if (shininess.Length()>0) // HG 15.05.95
        glMaterialf(face,GL_SHININESS,shininess[0]*GV_SHININESS_GL_SCALE);
    else ; // else what 
}

void GvMaterial::glSet(int i)
{
	float v[4];

	if (maxNumValues <0) ComputeMaxNumValues();
	i = i %	maxNumValues;
	
	GLenum face = GL_FRONT_AND_BACK;
	float a = 1.0-transparency.get1(i);

	glMaterialfv(face,GL_AMBIENT,ambientColor.GetRGBA(i,v,a));
	glMaterialfv(face,GL_DIFFUSE,diffuseColor.GetRGBA(i,v,a));
	glMaterialfv(face,GL_SPECULAR,specularColor.GetRGBA(i,v,a));
	glMaterialfv(face,GL_EMISSION,emissiveColor.GetRGBA(i,v,a));
	glMaterialf(face,GL_SHININESS,shininess.get1(i)*GV_SHININESS_GL_SCALE);
}

void GvMaterial::glSetColor(int i)
{
	float v[4];

	if (maxNumValues <0) ComputeMaxNumValues();
	i = i %	maxNumValues;
	
	float a = 1.0-transparency.get1(i);
	//Point color;
	//diffuseColor.Get(i,color,a);
   	//glColor(color);

    glColor4fv(diffuseColor.GetRGBA(i,v,a));
}

void GvMaterial::glSetColor()
{
   glSetColor(0);
}

void GvMaterial::glSetDiffuseWhite()
{
	static float v[4] = { 1.0,1.0,1.0,0.0 };
	v[3] = 1.0-transparency.get1(0);
    glColor4fv(v);
    // assume material diffuse tracked by color
	//GLenum face = GL_FRONT_AND_BACK;
	//glMaterialfv(face,GL_DIFFUSE,v);
}

#endif

int GvMaterial::Apply(GglRenderTraversal &state,int matStatus,int texStatus)
{	int ret=0;

	gbool whiteDiffuse = (matStatus & MATERIAL_SET_DIFFUSE_WHITE) !=0;


#ifdef _OGL
		glSet();
		changed=0;
#endif

#ifdef _D3D
   float a;

   if (changed || !state.glState->SetMaterial(handle,whiteDiffuse)) {
	float am[4];
	float di[4];
	float sp[4];
	float em[4];
	if (transparency.Length()>0) {
		a = 1.0-transparency[0];
		if (a<1.0) ret |= MATERIAL_HAS_ALPHA;
	} else a = 1.0;

	ambientColor.GetRGBA(am,a);
	diffuseColor.GetRGBA(di,a);
	specularColor.GetRGBA(sp,a);
	emissiveColor.GetRGBA(em,a);

	if (IsEmissiveColorOnly()) {
		ret = matStatus |=  MATERIAL_IS_UNLIT;
	}

	if ( (matStatus & MATERIAL_IS_UNLIT) || state.glState->unlit) { // new 
			RGBAColor c;
			c.a = a;
			c.r = max(di[0],em[0]);
			c.g = max(di[1],em[1]);
			c.b = max(di[2],em[2]);
			// redefine handle
			state.glState->SetMaterial(handle,c);
		} else {


	
	float power;
	if (shininess.Length()>0) 
        power = shininess[0]*GV_SHININESS_GL_SCALE;
    else power = 3;

	TRACE("Redefining VRML 1.0 Material : texturing = %d  htex = %d White = %d \n",state.glState->texturing,state.glState->hTex,whiteDiffuse);

	// redefine handle 
    state.glState->SetMaterial(handle,am,di,sp,em,a,power,whiteDiffuse);
	}
	
	changed=0;
   } else { // 9.07.97 
	   // check alpha 
	   if (HasTransparency()) {
			if (transparency.Length()>0) {
				a = 1.0-transparency[0];
				if (a<1.0) ret |= MATERIAL_HAS_ALPHA;
			} else a = 1.0;
	   }

   }	

   // 28.03.97 same as Gv2Material::Apply
   if (ret & MATERIAL_HAS_ALPHA) {
			state.glState->SetMaterialAlpha(a);
   } 
   else  {
		if (texStatus & TEXTURE_HAS_ALPHA)
			state.glState->SetBlendFromMaterial(1);
		else 
			state.glState->SetMaterialAlphaOff();
   }	


#endif
   return(ret);
}


void GvMaterial::GetColor(int i,Point &color)
{
	if (maxNumValues <0) ComputeMaxNumValues();
	i = i %	maxNumValues;
	diffuseColor.get1(i,color);
}

#endif _G_VRML1



//
// Gv2Material
//
GV_NODE_SOURCE_IMP(Gv2Material,GvNode);

Gv2Material::Gv2Material() : 
		ambientIntensity(0.2),shininess(0.2),transparency(0),
		diffuseColor(0.8,0.8,0.8),
		specularColor(0,0,0),
		emissiveColor(0,0,0)
{
    GV_NODE_CONSTRUCTOR(Gv2Material);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(ambientIntensity);
    GV_NODE_ADD_FIELD(diffuseColor);
    GV_NODE_ADD_FIELD(specularColor);
    GV_NODE_ADD_FIELD(emissiveColor);
    GV_NODE_ADD_FIELD(shininess);
    GV_NODE_ADD_FIELD(transparency);

   
	// mark as invalid, reading a node will not call OnFieldChanged method
	hasTransparency = -1;
	isEmissiveColorOnly = -1;
    changed = -1;

}

Gv2Material::~Gv2Material()
{					 
}

int Gv2Material::checkForDefaultValues(int mode)
{
	const Point zeroPoint(0,0,0);
	const Point gray(0.8,0.8,0.8);

	if (ambientIntensity.get() == 0.2) ambientIntensity.setDefault(TRUE);

	if (diffuseColor.getPoint() == gray) diffuseColor.setDefault(TRUE);
	if (emissiveColor.getPoint() == zeroPoint) emissiveColor.setDefault(TRUE);
	if (specularColor.getPoint() == zeroPoint) specularColor.setDefault(TRUE);

	if (shininess.get() == 0.2) shininess.setDefault(TRUE);
	if (transparency.get() == 0.0) transparency.setDefault(TRUE);
	return 0;
}



int Gv2Material::HasTransparency()
{
 	if (hasTransparency <0) {
		hasTransparency = (transparency.value > 0.0);
	}
 	return(hasTransparency);
}

// test if material is only emmissive color, so we can treat it unlit 
int Gv2Material::ComputeIsEmissiveColorOnly()
{
 	if ( isEmissiveColorOnly<0) {
		  isEmissiveColorOnly=0;
		  if (emissiveColor.value[0]>=0.99 && 
		 	  emissiveColor.value[1]>=0.99 && 
			  emissiveColor.value[2]>=0.99) {
		 			isEmissiveColorOnly=1;

		  } else  	
	      // if (ambientIntensity.value == 0.0) 
		  {
			if (diffuseColor.value[0]==0.0 && diffuseColor.value[1]==0.0 &&
	      		diffuseColor.value[2]== 0.0
				&& specularColor.value[0]==0.0 && specularColor.value[1]==0.0 &&
	      		specularColor.value[2]== 0.0
				) 
		 		isEmissiveColorOnly=1;
		  }
	}
 	return(isEmissiveColorOnly);
}


int Gv2Material::OnFieldChanged(GvField *field)
{
	// recompute 

	isEmissiveColorOnly = -1;
	hasTransparency = -1;
	changed = 1;
	return GvNode::OnFieldChanged(field); 
}


int Gv2Material::OnChanged()
{
  changed = 1;
  setRedraw();	 // done in appearance but not if in Proto :-(
  return GvNode::OnChanged();
}


#ifdef _OGL

/*
   set material as current gl material
   if whiteDiffuse is TRUE, white is used for diffuse color (texture mapping)
*/

void Gv2Material::glSet(GvBool whiteDiffuse)
{
	float v[4];
	GLenum face = GL_FRONT_AND_BACK; 
	float a = 1.0-transparency.value;
	v[3]=a;

    //	glMaterialfv(face,GL_AMBIENT,ambientColor.GetRGBA(i,v,a));
    
    if (whiteDiffuse) {
		v[0]=v[1]=v[2]=1.0;
		v[3] = a;
        glColor4fv(v);
    	glMaterialfv(face,GL_DIFFUSE,v);

    } else {
	    glMaterialfv(face,GL_DIFFUSE,diffuseColor.GetRGBA(v,a));
        glColor4fv(v);
    }

	v[0]*=ambientIntensity.value;
	v[1]*=ambientIntensity.value;
	v[2]*=ambientIntensity.value;
	glMaterialfv(face,GL_AMBIENT,v);


	glMaterialfv(face,GL_SPECULAR,specularColor.GetRGBA(v,a));
	glMaterialfv(face,GL_EMISSION,emissiveColor.GetRGBA(v,a));
	glMaterialf (face,GL_SHININESS,shininess.value*GV_SHININESS_GL_SCALE);
}


void Gv2Material::glSetColor()
{
	float v[4];
	float a = 1.0-transparency.value;
    glColor4fv(diffuseColor.GetRGBA(v,a));
}

void Gv2Material::glSetDiffuse(GvBool white)
{
	if (white) {
        glSetDiffuseWhite();
        return;
    }
    float v[4];
	float a = 1.0-transparency.value;
	
    glColor4fv(diffuseColor.GetRGBA(v,a));
    // assume material diffuse tracked by color
	//GLenum face = GL_FRONT_AND_BACK;
	//glMaterialfv(face,GL_DIFFUSE,v);
}
void Gv2Material::glSetDiffuseWhite()
{
	static float v[4] = { 1.0,1.0,1.0,0.0 };
	v[3] = 1.0-transparency.value;
    glColor4fv(v);
    // assume material diffuse tracked by color
	//GLenum face = GL_FRONT_AND_BACK;
	//glMaterialfv(face,GL_DIFFUSE,v);
}
#endif


void Gv2Material::GetColor(Point &color)
{
	diffuseColor.get(color);
}

//! query if material has any alpha
GvBool Gv2Material::getAlphaStatus(int &matStatus) {
	if (transparency.value > 0.0f) {
		
		matStatus |= MATERIAL_HAS_ALPHA;

		if (transparency.value >= 0.999) { 
			matStatus |= MATERIAL_FULLY_TRANSPARENT;
		}	
		return gtrue;

	} 
	return gfalse;
}



int Gv2Material::Apply(GglRenderTraversal &state,int matStatus,int texStatus)
{	int ret=matStatus;

	float a = 1.0f;
	
	// check transparency early 
	if (transparency.value > 0.0f) {
		a = 1.0f-transparency.value;
		ret |= MATERIAL_HAS_ALPHA;
		if (a<=0.001f) {  // new, early exit 
			ret |= MATERIAL_FULLY_TRANSPARENT;
			return ret;
		}	
		state.glState->SetMaterialAlpha(a);
		if (state.glState->alphaTestFromTexture) {
		}
	} 



	gbool whiteDiffuse = (matStatus & MATERIAL_SET_DIFFUSE_WHITE) !=0;

#ifdef _OGL

		if ( (matStatus & MATERIAL_IS_UNLIT) || !state.glState->lighting) { // new 
			
			glSet(whiteDiffuse);	

			RGBAColor c;
			c.a = a;
			c.r = max(diffuseColor.value[0],emissiveColor.value[0]);
			c.g = max(diffuseColor.value[1],emissiveColor.value[1]);
			c.b = max(diffuseColor.value[2],emissiveColor.value[2]);
			
			if (whiteDiffuse) 
				state.glState->SetCurrentColor(1,1,1,a);
			
			else if (matStatus & MATERIAL_IS_UNLIT) { // new 3.06.99
				state.glState->SetCurrentColor(emissiveColor[0],emissiveColor[1],emissiveColor[2],a);
			}
			else state.glState->SetCurrentColor(diffuseColor[0],diffuseColor[1],diffuseColor[2],a);

		} else 
		   glSet(whiteDiffuse);
#endif

#ifdef _D3D
	if (changed || !state.glState->SetMaterial(handle,whiteDiffuse)) {
		if ( (matStatus & MATERIAL_IS_UNLIT) || state.glState->unlit) { // new 
			RGBAColor c;
			c.a = a;
			c.r = max(diffuseColor.value[0],emissiveColor.value[0]);
			c.g = max(diffuseColor.value[1],emissiveColor.value[1]);
			c.b = max(diffuseColor.value[2],emissiveColor.value[2]);
			// redefine handle
			state.glState->SetMaterial(handle,c);

		} else {

		float ambient[3];

		ambient[0]= diffuseColor.value[0]*ambientIntensity.value;
		ambient[1]= diffuseColor.value[1]*ambientIntensity.value;
		ambient[2]= diffuseColor.value[2]*ambientIntensity.value;

		float power = shininess.value*GV_SHININESS_GL_SCALE;

		// redefine handle
		state.glState->SetMaterial(handle,ambient,diffuseColor.value,
			specularColor.value,emissiveColor.value,a,power,whiteDiffuse);
		}

	}
	else {
		// need this for exbuf creation
		if (whiteDiffuse) 
			state.glState->SetCurrentColor(1,1,1,a);
		else if (matStatus & MATERIAL_IS_UNLIT) { // new 3.06.99
			state.glState->SetCurrentColor(emissiveColor[0],emissiveColor[1],emissiveColor[2],a);
		}
	    else state.glState->SetCurrentColor(diffuseColor[0],diffuseColor[1],diffuseColor[2],a);

	}
#endif
	// check transparency
	if (ret & MATERIAL_HAS_ALPHA) {
			// already setup before
	} 
	else  {

		if (texStatus & TEXTURE_HAS_ALPHA)	// turn on blending if texture has alpha 
			state.glState->SetBlendFromMaterial(1);
		else 
			state.glState->SetMaterialAlphaOff();
	}
   
	state.glState->SetLitMaterial(!(ret & MATERIAL_IS_UNLIT) );
	
	changed=0;

	return(ret);
}


#ifdef _D3D
int Gv2Material::Apply(GglRenderTraversal &state,G3MaterialRef &handle,
					   int matStatus,int texStatus)
{	int ret=matStatus;

	float a = 1.0f;
	
	// check transparency early 
	if (transparency.value >0.0f) {
			a = 1.0f-transparency.value;
			ret |= MATERIAL_HAS_ALPHA;
			if (a<=0.01f) {  // new, early exit 
				ret |= MATERIAL_FULLY_TRANSPARENT;
				return ret;
			}	
			state.glState->SetMaterialAlpha(a);
	} 


	gbool whiteDiffuse = (matStatus & MATERIAL_SET_DIFFUSE_WHITE) !=0;

#ifdef _OGL
		glSet(whiteDiffuse);
#endif

#ifdef _D3D
	if (changed || (texStatus & TEXTURE_CHANGED) || !state.glState->SetMaterial(handle,whiteDiffuse)) {

		if (IsEmissiveColorOnly()) {
			ret = matStatus |=  MATERIAL_IS_UNLIT;
		}
		// redefine handle

		if ( (matStatus & MATERIAL_IS_UNLIT) || state.glState->unlit) { // new 
			RGBAColor c;
			c.a = a;
			c.r = max(diffuseColor.value[0],emissiveColor.value[0]);
			c.g = max(diffuseColor.value[1],emissiveColor.value[1]);
			c.b = max(diffuseColor.value[2],emissiveColor.value[2]);
			// redefine handle
			state.glState->SetMaterial(handle,c);

		} else {


		float ambient[3];

		if (whiteDiffuse) {
			ambient[0]= ambientIntensity.value;
			ambient[1]= ambientIntensity.value;
			ambient[2]= ambientIntensity.value;
		} else {
			ambient[0]= diffuseColor.value[0]*ambientIntensity.value;
			ambient[1]= diffuseColor.value[1]*ambientIntensity.value;
			ambient[2]= diffuseColor.value[2]*ambientIntensity.value;
		}

		float power = shininess.value*GV_SHININESS_GL_SCALE;

		// redefine handle
		state.glState->SetMaterial(handle,ambient,diffuseColor.value,
			specularColor.value,emissiveColor.value,a,power, whiteDiffuse);
		}

	}
#endif

	if (ret & MATERIAL_HAS_ALPHA) {
		// already setup before
	} 
	else  {
		if (texStatus & TEXTURE_HAS_ALPHA)
			state.glState->SetBlendFromMaterial(1);
		else 
			state.glState->SetMaterialAlphaOff();
	}

   state.glState->SetLitMaterial(!(ret & MATERIAL_IS_UNLIT) );

   changed=0;

   return(ret);
}
#endif

