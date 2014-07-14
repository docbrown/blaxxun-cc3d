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
/******************************************************************************
@doc

@module GV2Geometry.cpp - VRML 2.0 geometry nodes |

Copyright (c) 1995-1999	by Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:

Changes:


10.08.98 HG added culling check at Shell Level

23.02.98 HG RaySelectTraversal BuildShell added for Collsion node proxy field
		 HG added Material_FULLY_TRANSPARENT materialStatus bit, geometry not drawn if set

$Revision: 1.38 $
$Log: gv2geometry.cpp,v $
Revision 1.38  1998/10/09 16:38:51  holger
empty message

Revision 1.32  1998/08/20 17:24:57  holger
empty message

Revision 1.31  1998/07/09 15:50:06  kristof
*** empty log message ***

Revision 1.30  1998/07/06 17:53:51  holger
empty message





Todo :

******************************************************************************/

#include "stdafx.h"

#define _GX
// katmai, decompose to gshell


#include "greporter.h"

#include <gvtraverse.h>

#include <gvmaterial.h>
#include <gvtexture2.h>
#include <gvtexture2transform.h>
#include <gv2geometry.h>
#include <gvfontstyle.h>

#include <gvextrusion.h>

#include "gvisibility.h"

#include "gshell.h"
#include "gshelli.h"
#include "ggeom.h"

#include "grender.h"
#include "gmodel.h"

// ?? #include "d3dutil.h"

#include <grayselect.h>

// Font support (True Type)
#ifdef WIN32
#include "gfont.h"
#endif


#include "goptimizetraversal.h"



// cull check 
static inline gbool CULLCHECK(GShell *theShell,GglRenderTraversal &state)
{
#if 1
	if (state.doViewCulling /* && state.doClipping */ )
	{
		GMatrixStack *mstack = state.GetCurrentMatrix();	
		if (mstack->m.contents & ( Matrix::HAS_NEGATIVE_SCALE) ) {
		    state.doClipping = gtrue;
			return gtrue; // not working with neg scale 
		}


		if (!mstack->viewpointLocalOk) { // need to transform it 
			mstack->SetViewpointLocal(state.physicalCamera->position);	
		}  

		if (!mstack->viewVolumeLocalOk) {
			mstack->SetViewVolumeLocal(state.viewVolume); 
		}

		//if (BoxViewVolumeOutside(theShell->GetBBox(),mstack->viewpointLocal,mstack->viewVolumeLocal)) { 
		//	return gfalse;
		//}
		
		if (BoxViewVolumeOutside(theShell->GetBBox(),mstack->viewpointLocal,mstack->viewVolumeLocal,state.doClipping)) { 
			return gfalse;
		}

	}
#endif
	return gtrue;
}

// get the error reporting object 
static GReporter* getReporter(GTraversal &state) 
{
	GView * view = state.getBrowser(); 
	ASSERT(view != NULL);
	if (!view) return GReporter::current; // return NULL;
	return view->GetReporter();
}

//
// GvColor
//


GV_NODE_SOURCE_IMP(GvColor,GvNode);

GvColor::GvColor() : color(0)
{
    GV_NODE_CONSTRUCTOR(GvColor);
    isBuiltIn = TRUE;
    GV_NODE_ADD_FIELD(color);
}

GvColor::~GvColor()
{
}

//
// GvCoordinate
//


GV_NODE_SOURCE_IMP(GvCoordinate,GvNode);

GvCoordinate::GvCoordinate() : point(0)
{
    GV_NODE_CONSTRUCTOR(GvCoordinate);
    isBuiltIn = TRUE;
    GV_NODE_ADD_FIELD(point);
}

GvCoordinate::~GvCoordinate()
{
}

// compute boundingbox based on all points 
int GvCoordinate::Do(GBBoxTraversal &state)
{

//  return Do(* ((GTraversal *) &state));

// a shape nodes might use a subset of coordinates

  BBox box;
  const Point *vp,*vpend;
  Point mi,ma;

  if (point.Length()<=0) return(0);

  vp= (const Point *) point.values;
  vpend=&vp[point.Length()];

  mi= ma= *vp++;
  for (; vp<vpend;vp++) {
		vp->MinMax(mi,ma);
  }

  box.mi = mi;
  box.ma = ma;
  
  state.Union(box);
  //  anyway traverse / register attribute ?? 
  // Do(* ((GTraversal *) &state));

  return(1);

}


//
// Gv2Normal
//


GV_NODE_SOURCE_IMP(Gv2Normal,GvNode);

Gv2Normal::Gv2Normal() : vector(0)
{
    GV_NODE_CONSTRUCTOR(Gv2Normal);
    isBuiltIn = TRUE;
    GV_NODE_ADD_FIELD(vector);
}

Gv2Normal::~Gv2Normal()
{
}

//
// GvTextureCoordinate
//


GV_NODE_SOURCE_IMP(GvTextureCoordinate,GvNode);

GvTextureCoordinate::GvTextureCoordinate() : point(0)
{
    GV_NODE_CONSTRUCTOR(GvTextureCoordinate);
    isBuiltIn = TRUE;
    GV_NODE_ADD_FIELD(point);
}

GvTextureCoordinate::~GvTextureCoordinate()
{
}



//
// GvAppearance
//


GV_NODE_SOURCE_IMP(GvAppearance,GvNode);

GvAppearance::GvAppearance()
{
    GV_NODE_CONSTRUCTOR(GvAppearance);
    isBuiltIn = TRUE;
    GV_NODE_ADD_FIELD(material);
    GV_NODE_ADD_FIELD(texture);
    GV_NODE_ADD_FIELD(textureTransform);

	changed = 0;

}

GvAppearance::~GvAppearance()
{
}

int GvAppearance::checkForDefaultValues(int mode)
{
	if (material.get() == (GvNode *) NULL) material.setDefault(TRUE);
	if (texture.get() == (GvNode *) NULL) texture.setDefault(TRUE);
	if (textureTransform.get() == (GvNode *) NULL) textureTransform.setDefault(TRUE);
	return 0;
}


// some child node has been changed, set redraw bit 
int GvAppearance::OnChildChanged(GvNode *child)
{
   setRedraw();
#ifdef _D3D
   if (!(child == textureTransform)) {
	   if (handle.Ok()) {
		   TRACE("GvAppearance::OnChildChanged releasing material handle \n");
		   handle.Release();
	   }	
   }		
#endif
   return GvNode::OnChildChanged(child); // this doesn't notify owner do this ?
}


int GvAppearance::OnFieldChanged(GvField *field)
{
	OnChanged(); // ??
	return(1);
}


int GvAppearance::OnChanged()
{
  changed = 1;
  setRedraw(); // temp
  return(GvNode::OnChanged());
}




int GvAppearance:: Do(GTraversal &state)
{	int ret = 0;
	if (state.appearance != this) {
		state.appearance = this;
		state.appearanceApplied = 0;	
	}
   	ret = material.Traverse(state);
   	ret = texture.Traverse(state);
   	ret = textureTransform.Traverse(state);
	return(ret);
}

int GvAppearance:: Do(GPathTraversal &state)
{	int ret = 0;

	GPUSH_PATH(state,this);

	if (state.appearance != this) {
		state.appearance = this;
		state.appearanceApplied = 0;	
	}
   	ret = material.Traverse(state);
   	ret = texture.Traverse(state);
   	ret = textureTransform.Traverse(state);

	GPOP_PATH(state);

	return(ret);
}


int GvAppearance::Do(GglRenderTraversal &state)
{  	int ret = 1;
 	if (state.appearance != this) {
		state.appearance = this;
		state.appearanceApplied = 0;	
		// shape.Do() will check for appearance applied
	//ret = material.Traverse(state);
   	//ret = texture.Traverse(state);
   	//ret = textureTransform.Traverse(state);

	}
	// will be done from the geometry node via Apply
	return(ret);
}
// this is the current appearance node in state
// apply sub properties 
/*

  if texturing

*/

#ifdef _D3D

// get the D3D Material handle strucutre 
G3MaterialRef* GvAppearance::GetMaterialHandle()
{
	if (handle.Ok()) return &handle;
	Gv2Material *newMat= (Gv2Material*) material.value.Eval();
	if (!newMat) return (NULL);
	return newMat->GetMaterialHandle();
}

#endif

/*

  apply the effect of the appearance node to the current rendering state
  Problem in Direct 3D Ramp Mode,
  if texture has changed, the material has to be redefined

*/

#if 1

int GvAppearance::Apply(GglRenderTraversal &state)
{  	int ret = 0;
    GvBool matHasAlpha; // has the material alpha
	GvBool resetDiffuse = FALSE;
	GvBool whiteDiffuse = FALSE;
	GvBool setMaterial = FALSE;

	int texStatus=0;
	int matStatus=0;

#ifdef _D3D

	// 22.03.98 HG flush state if less then 128 bytes left 
	if (!state.glState->dx6) state.glState->FlushState(128);  // got assertion in G3RenderState.FlushState_() !!
#endif

	Gv2Material *newMat= (Gv2Material*) material.value.Eval(state);

	if (newMat) {
	    // do an early exit if Material is transparent and we are just collecting transparent shapes for sorting
		if (state.material == newMat) {	
			matStatus = state.materialStatus;
			matHasAlpha = (matStatus & MATERIAL_HAS_ALPHA) != 0;
		}
		else {

	
			matHasAlpha = newMat->getAlphaStatus(matStatus);
			state.materialStatus = matStatus;
		}

		if ( matHasAlpha && ((GGvglRenderTraversal *) &state)->transparencyMode > GTRANSPARENCY_NONE) {
               	
				state.material = newMat;	
				return(0);
         }

	}


    GvTexture *t = (GvTexture*) texture.value.Eval(state);

	// TRACE("Applying appearance %p...\n",this);

	if (state.useTextures) 
	{
		if (state.texture != t) {	// **********  new texture
		if ( t == NULL)  // no texture, turn off
        {
			if (state.glState->texturing) {  // are we in texturing mode ?
				resetDiffuse = state.glState->textureParameters.set_diffuse_white;
				//
				// set texturing off
				state.glState->SetTexturing(0);
  				state.glState->textureDefined = 0;
			}
			state.texture = NULL;
		} 
		else	// set texture 
		{
			int ret;
			gbool newDefined = FALSE;
			if (!t->Defined()) {
				ret = t->Define(state);
				newDefined = TRUE;
				setMaterial = TRUE; // for ramp mode
			}

			state.texture = t ; // the current texture node in effect

            if ( t->HasTransparency() && !t->HasColorKey() && ((GGvglRenderTraversal *) &state)->transparencyMode > GTRANSPARENCY_NONE) {
                texStatus |= TEXTURE_HAS_ALPHA;
            }
            else
			if ( (texStatus = state.texture->glSet(state))>0) {
				if (newDefined) texStatus |= TEXTURE_CHANGED;

				if (!t->IsGrayscale() && state.glState->textureParameters.set_diffuse_white) {
					matStatus |= MATERIAL_SET_DIFFUSE_WHITE;
					whiteDiffuse = 1;
				}
			}
			else { // disable texturing 
				if (state.glState->texturing) { 
					resetDiffuse = state.glState->textureParameters.set_diffuse_white;
					// set texturing off
					state.glState->SetTexturing(0);
  					state.glState->textureDefined = 0;
				}
			}	
		}
	
		setMaterial = TRUE; // force reload of materials because it contains textureHandle
		state.textureStatus = texStatus;

	} else {  // same texture node 
		if (t && !t->IsGrayscale() && state.glState->texturing && state.glState->textureParameters.set_diffuse_white) {
			whiteDiffuse = 1 ;
 			matStatus |= MATERIAL_SET_DIFFUSE_WHITE;
		}
		texStatus = state.textureStatus;

		setMaterial = TRUE; // force reload of materials because it contains textureHandle

    }

	} // if (state.useTextures) 
	//TRACE("Texturing : %d texstatus %d \n",state.glState->texturing,texStatus);

	// FOR D3D we should have the current texture matrix for execute buffer construction
	if (/*state.glState->textureDefined &&  */
		state.textureTransform != textureTransform.value) {

        GTextureMatrixStack *current;
		current = (GTextureMatrixStack*) state.Get(GTextureMatrixStack::attrId);

        Gv2TextureTransform *tt = (Gv2TextureTransform*) textureTransform.value.Eval(state);

       // TRACE("new texture Transform %p \n",tt);

		if (tt == NULL) {
			// set any state transform ? no, we do not support VRML 1 mixing
			state.glState->SetTextureMatrixIdentity();	
			state.textureTransform = NULL;
		}
	    else {
			tt->Apply(state);
			state.textureTransform =  tt;
		}
	}



	if (setMaterial || state.material != newMat) {	//******** need a new material setting 
		
		// if it is all too bright, or texture not loaded yet, take a lighted material 
		if (t != NULL && newMat == NULL && !state.glState->textureDefined) {
			newMat = state.defaultMaterial;
		}


		//TRACE("new material %p \n",newMat);
		state.material = newMat;

		
		if (state.material == NULL ) { // use default material
			// VRML 2.0 : no material ==> UNLIT 
			matStatus |= MATERIAL_IS_UNLIT|MATERIAL_SET_DIFFUSE_WHITE; 

			// could do a short code here 
#ifdef _D3D
			// FOR D3D we need a separate Material because of the texture befor 
			if (changed || (texStatus & TEXTURE_CHANGED) || !state.glState->SetMaterial(handle)) {
				RGBAColor c(1,1,1,1);
				state.glState->SetMaterial(handle,c);
				//handle.handle->whiteDiffuse = TRUE;
			}
			else 
#endif
				state.glState->SetCurrentColor(1,1,1,1);

			if (texStatus & TEXTURE_HAS_ALPHA)
				state.glState->SetBlendFromMaterial(1);
			else 
				state.glState->SetMaterialAlphaOff();

			//  DECAL with ALPHA doesn't work
			if (state.glState->textureDefined) {
				if (matHasAlpha || ((texStatus & (TEXTURE_HAS_ALPHA | TEXTURE_HAS_COLORKEY | TEXTURE_MODULATE )) !=0))
				state.glState->SetTextureBlend(state.glState->textureParameters.defaultBlend);
				else state.glState->SetTextureBlend(state.glState->textureParameters.unlitBlend); 
			}

		} else {

			if (newMat->IsEmissiveColorOnly()) {  
				matStatus |= MATERIAL_IS_UNLIT; 
				// will this be overridden ??
				state.glState->SetCurrentColor(newMat->emissiveColor[0],newMat->emissiveColor[1],newMat->emissiveColor[2],1.0f- newMat->transparency);

			}

			if (state.glState->textureDefined) { // use handle of appearance
				if (matStatus & MATERIAL_IS_UNLIT && !matHasAlpha 
					&& (((texStatus & (TEXTURE_HAS_ALPHA | TEXTURE_HAS_COLORKEY | TEXTURE_MODULATE )) ==0)) // 11.07.99
					) {
					state.glState->SetTextureBlend(state.glState->textureParameters.unlitBlend); // D3DTBLEND_DECAL
				}
				else // if (state.glState->lighting) // 11.07.99
					state.glState->SetTextureBlend(state.glState->textureParameters.defaultBlend); 

#ifdef _D3D

				/* use extra handle, combined Texture & Material,
					but could result in consitency problems if material is animated
					and used  several times with different textures in appearance
					new solution in GvAppearance::OnChildChanged()
				*/
				if (state.glState->canDoRgbLighting)
					matStatus |= newMat->Apply(state,matStatus,texStatus);

				else matStatus |= newMat->Apply(state,handle,matStatus,texStatus);
#else
					matStatus |= newMat->Apply(state,matStatus,texStatus);

#endif
					
			}
			else matStatus |= newMat->Apply(state,matStatus,texStatus);
		}

		state.glState->SetLitMaterial(!(matStatus & MATERIAL_IS_UNLIT)); // new 8.09.98 hg 
		// already done by material apply ? 

		state.materialStatus = matStatus;
	}
	else { // same material 

        if (resetDiffuse) { // reset diffuse color, could be white from before 
#ifdef _OGL
		    if (newMat != NULL)
                 newMat->glSetDiffuse(whiteDiffuse);
#endif

        }
	}
	state.appearanceApplied = 1;
	changed = 0;
	return(ret);
}
#else


int GvAppearance::Apply(GglRenderTraversal &state)
{  	int ret = 0;
	GvBool resetDiffuse = 0;
	GvBool whiteDiffuse = 0;
	GvBool setMaterial = 0;

	int texStatus=0;
	int matStatus=0;

	if (state.useTextures) 
	{

        GvTexture *t =(GvTexture*) texture.value.Eval(state);
		if (state.texture != t) {	// **********  new texture

		if ( t == NULL)  // no texture, turn off
        {
			if (state.glState->texturing) { 
				resetDiffuse = state.glState->textureParameters.set_diffuse_white;
				// set texturing off
				state.glState->SetTexturing(0);
  				state.glState->textureDefined = 0;
			}
			state.texture = NULL;
		} 
		else			// set texture 
		{
			int ret;
			if (!t->Defined()) {
				ret = t->Define(state);
			}
		    //////////t->Ok())  
			state.texture = t ;
            if ( t->HasTransparency() && ((GGvglRenderTraversal *) &state)->transparencyMode > GTRANSPARENCY_NONE) {
                texStatus |= TEXTURE_HAS_ALPHA;
            }
            else
				if ((texStatus = state.texture->glSet(state))>0) {
					if (!t->IsGrayscale() && state.glState->textureParameters.set_diffuse_white) {
						matStatus |= MATERIAL_SET_DIFFUSE_WHITE;
						whiteDiffuse = 1;
					}
				}
				else { // disable 
					if (state.glState->texturing) { 
						resetDiffuse = state.glState->textureParameters.set_diffuse_white;
						// set texturing off
						state.glState->SetTexturing(0);
  						state.glState->textureDefined = 0;
						// state.texture = NULL;
					}
				}	

		}
	
	#ifdef _D3D
		setMaterial = TRUE; // force reload of materials because it contains textureHandle
	#endif

		if (state.materialStatus != matStatus) {
			setMaterial = TRUE; // reset material because of diffuseWhite change ! new for OGL 24.06.98
		}
		state.textureStatus = texStatus;

	} else { // old texture 
		if (t && !t->IsGrayscale() && state.glState->texturing && state.glState->textureParameters.set_diffuse_white) {
			whiteDiffuse = 1 ;
 			matStatus |= MATERIAL_SET_DIFFUSE_WHITE;
		}
		if (state.materialStatus != matStatus) {
			setMaterial = TRUE; // reset material because of diffuseWhite change ! new for OGL 24.06.98
		}
		
		texStatus = state.textureStatus;
    }

	} // if (state.useTextures) 

#ifdef _OGL
	if (state.useTextures) 
#endif

// FOR D3D we should have the current matrix for execute buffer constructions
	if (/*state.glState->textureDefined &&  */
		state.textureTransform != textureTransform.value) {

        GTextureMatrixStack *current;
		current = (GTextureMatrixStack*) state.Get(GTextureMatrixStack::attrId);

        Gv2TextureTransform *tt = (Gv2TextureTransform*) textureTransform.value.Eval(state);

       // TRACE("new texture Transform %p \n",tt);

		if (tt == NULL) {
			// to do:set any state transform
			state.glState->SetTextureMatrixIdentity();	
			state.textureTransform = NULL;
		}
	    else {
			state.textureTransform =  tt;

			// to do: set any state transform
#ifdef _OGL
			state.textureTransform->Apply(state);
#endif

#ifdef _D3D
			// to do
			Matrix m;
			tt->Get(m);
			if (tt->changed || tt->mVersion==0) {
				tt->mVersion= state.glState->GetNewTextureMatrixVersion();
				tt->changed = 0;
			}

			state.glState->SetTextureMatrix(m,tt->mVersion);	
			TRACE("to do d3d:new texture Transform \n");
#endif

		}
	}

	Gv2Material *newMat= (Gv2Material*) material.value.Eval(state);

	if (setMaterial || state.material != newMat) {	//******** need a new material setting 
//		TRACE("new material \n");
		state.material = newMat;
		
		if (state.material == NULL) { // 5.10.96 use default material
			// VRML 2.0 : no material ==> UNLIT 
			matStatus |= MATERIAL_IS_UNLIT|MATERIAL_SET_DIFFUSE_WHITE; 
			whiteDiffuse = 1; //

			if (texStatus & TEXTURE_HAS_ALPHA)
				state.glState->SetBlendFromMaterial(1);
			else 
				state.glState->SetMaterialAlphaOff();

			// could do a short code here 
#ifdef _D3D
			// FOR D3D we need a separate Material because of the texture before 
			if (changed || !state.glState->SetMaterial(handle)) {
				RGBAColor c(1,1,1,1);
				state.glState->SetMaterial(handle,c);
			}

#else
			newMat = state.defaultMaterial;

#endif
			}

		if (newMat != NULL) 
			matStatus |= newMat->Apply(state,matStatus,texStatus);

		state.materialStatus = matStatus;
	}
	else {

        if (resetDiffuse) { // reset diffuse color, could be white from before 
#ifdef _OGL
		    if (newMat != NULL)
                 newMat->glSetDiffuse(whiteDiffuse);
#endif
        }
	}
	changed = 0;
	state.appearanceApplied = 1;
	return(ret);
}
#endif

// add child node , for easier editing
int GvAppearance::addChild(GvNode *child)
{ 
	// to do: check types GvSFNode
  if (RTISA(child,Gv2Material)) { material.set(child); 	OnMaterialChanged(); return(1); }
  if (RTISA(child,GvTexture)) { texture.set(child);  OnTextureChanged(); return(1); }
  if (RTISA(child,Gv2TextureTransform)) { textureTransform.set(child);  OnTextureTransformChanged(); return(1); }
  return(0);
}  



//
// GvShape
//


GV_NODE_SOURCE_IMP(GvShape,GvNode);

GvShape::GvShape()
{
    GV_NODE_CONSTRUCTOR(GvShape);
    isBuiltIn = TRUE;
    GV_NODE_ADD_FIELD(appearance);
    GV_NODE_ADD_FIELD(geometry);

}

GvShape::~GvShape()
{
}

int GvShape::checkForDefaultValues(int mode)
{
	if (appearance.get() == (GvNode *) NULL) appearance.setDefault(TRUE);
	if (geometry.get() == (GvNode *) NULL) geometry.setDefault(TRUE);
	return 0;
}


int GvShape:: Do(GTraversal &state)
{	int ret = 0;
    
	state.shape = this;
	ret = appearance.Traverse(state);
   	ret = geometry.Traverse(state);
	state.shape = NULL;
	return(ret);
}


// appearance should not have a extent
// but problem BuildShell should have all local information
int GvShape:: Do(GBBoxTraversal &state)
{	int ret = 0; 
	state.shape = this;
	ret = geometry.Traverse(state);
	state.shape = NULL;
	return(ret);
}

// get bounding box 
GvBool GvShape::getBBox(BBox &bbox)
{
    GvNode *n= geometry;
	if (n == NULL) return FALSE;
	return  n->getBBox(bbox);
}



int GvShape::Do(GglRenderTraversal &state)
{  	int ret = 0;
	state.shape = this;

	// process apperance
	if (appearance.value == (GvNode*) NULL) { // use default 
		ret = state.defaultAppearance->Do(state);
	}
	else {
		ret = appearance.Traverse(state);
	}

	// traverse geometry
	ret = geometry.Traverse(state);

	state.shape = NULL;
	return(ret);
}


int GvShape::Do(GRaySelectTraversal &state)
{  	int ret = 0;
	GPUSH_PATH(state,this);

	state.shape = this;
#if 1
	// but needed now because shells build during rayselect,
	// xxx not needed until transparency check 
	if (appearance.value == (GvNode*) NULL) { // use default 
		ret = state.defaultAppearance->Do(state);
	}
	else {
		ret = appearance.Traverse(state);
	}
#endif
    ret = geometry.Traverse(state);

	state.shape = NULL;	
	GPOP_PATH(state);

	return(ret);
}

// set redraw bit if a field changed 
int GvShape::OnFieldChanged(GvField *field)
{
	setRedraw();
	return GvNode::OnFieldChanged(field);
}

// some child node has been changed, set redraw bit 
int GvShape::OnChildChanged(GvNode *child)
{
   setRedraw();
   return GvNode::OnChildChanged(child);
}


// add child node , for easier editing
int GvShape::addChild(GvNode *child)
{ 
  if (RTISA(child,GvAppearance)) { appearance.set(child); 	OnAppearanceChanged(); return(1); }
  // any valid geometry node here ?
  if (RTISA(child,GvNodeShell)) { geometry.set(child); 	OnGeometryChanged(); return(1); }

  // support for appearance sub types
  // to do: check types
  if (RTISA(child,Gv2Material) ||
	  RTISA(child,GvTexture) ||
      RTISA(child,Gv2TextureTransform)) 
  { 
	  if (appearance.value.node == NULL) 
		  appearance.set(new GvAppearance());

	  if (!RTISA(appearance.value.node,GvAppearance)) return(0);
	  int ret = ((GvAppearance *) appearance.value.node)->addChild(child);  
	  
	  OnAppearanceChanged(); // to be removed

	  return(ret); 
  }
  return(0);
}  



//
// GvBox
//


GV_NODE_SOURCE_IMP(GvBox,GvNodeShell);

GvBox::GvBox()
{
    GV_NODE_CONSTRUCTOR(GvBox);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(size);

    size.value[0]=size.value[1]=size.value[2]=2;

}

GvBox::~GvBox()
{
}

// is p outside the box ? 
GvBool GvBox::isOutside(const Point &p) const 
{
	return ( (p.x < -size.value[0]*0.5) || (p.x > size.value[0]*0.5) 
			 || (p.y < -size.value[1]*0.5) || (p.y > size.value[1]*0.5) 		
			 || (p.z < -size.value[2]*0.5) || (p.z > size.value[2]*0.5) 		
		); 
}


int			
GvBox::BuildShell(GTraversal &state)
{
   SetShell(NewBox(-size.value[0]*0.5,-size.value[1]*0.5,-size.value[2]*0.5,
					size.value[0],size.value[1],size.value[2]));

#ifdef _GX
    if (RTISA(theShell, GShellI)) {
  		GShellI *shellI = (GShellI *) theShell;
		if (1) {
			GShell *newS = new GShell();
			const PointArray &vn = shellI->GetVN();
			shellI->ComputeShell(newS,1);
			SetShell(newS);
		}
	}
#endif


   return(1);
}

GvBool GvBox::getBBox(BBox &bbox)
{
	bbox.mi.Set(-size.value[0]*0.5,-size.value[1]*0.5,-size.value[2]*0.5);
	bbox.ma.Set(size.value[0]*0.5,size.value[1]*0.5,size.value[2]*0.5);
	return TRUE;
}

int GvBox::Do(GBBoxTraversal &state)
{
  BBox box;

  box.mi.Set(-size.value[0]*0.5,-size.value[1]*0.5,-size.value[2]*0.5);
  box.ma.Set(size.value[0]*0.5,size.value[1]*0.5,size.value[2]*0.5);
  state.Union(box);
  return(1);
}

int GvBox::Do(GglRenderTraversal &state)
{  	
	if (!theShell) BuildShell(state);
	
	if (!theShell) return (/*GvNode*/ GNode::Do(state));

	if (!CULLCHECK(theShell,state)) return TRAVERSE_CULLED;


	if (!state.appearanceApplied) {
		if (state.appearance)
			state.appearance->Apply(state);
	}

	// don't draw fully transparent geometry
	if (state.materialStatus & MATERIAL_FULLY_TRANSPARENT) {
		return TRAVERSE_CULLED;
    }

	// collect transparent shapes  ?
	if (((GGvglRenderTraversal *) & state)->IsDelayed())
	{
		if (!theShell) { // need shell for Bounding Box
		    state.glState->SetCulling(Solid(),FALSE);
			BuildShell(state);
		}
		if (theShell) {
			GTransparentElement * e= new GTransparentElement(state.appearance,this,theShell->GetBBox(),state.GetMatrixRef());
			((GGvglRenderTraversal *) & state)->AddTransparentElement(e);
		}
		return TRAVERSE_DELAYED;
	}


	// apply shape hints 
    state.glState->SetCulling(Solid(),FALSE);

	// apply traversal 
	int ret = theShell->Traverse(state);

	return(ret);
}

int GvBox::Do(GGeometryTraversal &state)
{ 	int ret;
 	
	// apply shape hints 
    state.SetCulling(Solid(),FALSE);

	if (!theShell) 	BuildShell(state); 

	if (!theShell) return (0);

	ret = theShell->Traverse(state);

	return ret;
}


int GvBox::Do(GRaySelectTraversal &state)
{  	
	if (!theShell) 	BuildShell(state); 

	if (!theShell) return (0);

	// apply shape hints 
    state.SetCulling(Solid(),FALSE);

	int ret;

	// apply traversal 
	GPUSH_PATH(state,this);
		ret = theShell->Traverse(state);
	GPOP_PATH(state);
	return ret;

}


//
// Gv2Cone
//


GV_NODE_SOURCE_IMP(Gv2Cone,GvNodeShell);

Gv2Cone::Gv2Cone() :
    bottomRadius(1),
    height(2),
    side(TRUE),
    bottom(TRUE)

{
    GV_NODE_CONSTRUCTOR(Gv2Cone);
    isBuiltIn = TRUE;
    GV_NODE_ADD_FIELD(bottomRadius);
    GV_NODE_ADD_FIELD(height);
    GV_NODE_ADD_FIELD(side);
    GV_NODE_ADD_FIELD(bottom);

}

Gv2Cone::~Gv2Cone()
{
}

int			
Gv2Cone::BuildShell(GTraversal &state)
{
	int parts = (side ? 1 : 0) | (bottom ? 2 : 0);
	SetShell(NewConeVrml(0,0,0,bottomRadius,height,16,parts)); 
#ifdef _GX
    if (RTISA(theShell, GShellI)) {
  		GShellI *shellI = (GShellI *) theShell;
		if (1) {
			GShell *newS = new GShell();
			const PointArray &vn = shellI->GetVN();
			shellI->ComputeShell(newS,1);
			SetShell(newS);
		}
	}
#endif


	return(1);
}

GvBool Gv2Cone::getBBox(BBox &bbox)
{
	bbox.mi.Set(-bottomRadius,-height*0.5,-bottomRadius);
	bbox.ma.Set(bottomRadius,height*0.5,bottomRadius);
	return TRUE;
}

int Gv2Cone::Do(GBBoxTraversal &state)
{
	BBox box;

	box.mi.Set(-bottomRadius,-height*0.5,-bottomRadius);
	box.ma.Set(bottomRadius,height*0.5,bottomRadius);
	state.Union(box);
	return(1);
}

int Gv2Cone::Do(GglRenderTraversal &state)
{  	
	if (!theShell) BuildShell(state);
	
	if (!theShell) return (/*GvNode*/ GNode::Do(state));

	if (!CULLCHECK(theShell,state)) return TRAVERSE_CULLED;

	if (!state.appearanceApplied) {
		if (state.appearance)
			state.appearance->Apply(state);
	}

	// don't draw fully transparent geometry
	if (state.materialStatus & MATERIAL_FULLY_TRANSPARENT) {
		return 0;
    }

	// collect transparent shapes  ?
	if ( ((GGvglRenderTraversal *) & state)->IsDelayed())
	{
		if (!theShell) { // need shell for Bounding Box
		    state.glState->SetCulling(Solid(),FALSE);
			BuildShell(state);
		}
		if (theShell) {
			GTransparentElement * e= new GTransparentElement(state.appearance,this,theShell->GetBBox(),state.GetMatrixRef());
			((GGvglRenderTraversal *) & state)->AddTransparentElement(e);
		}
		return TRAVERSE_DELAYED;
	}

	// apply shape hints 
    state.glState->SetCulling(Solid(),FALSE);

	// apply traversal 
	int ret = theShell->Traverse(state);

	return(ret);
}

int Gv2Cone::Do(GGeometryTraversal &state)
{ 	int ret;
 	
    state.SetCulling(Solid(),FALSE);

	if (!theShell) 	BuildShell(state); 

	if (!theShell) return (0);

	ret = theShell->Traverse(state);

	return ret;
}


int Gv2Cone::Do(GRaySelectTraversal &state)
{  	
	if (!theShell) 	BuildShell(state); 

	if (!theShell) return (0);

	// apply shape hints 
    state.SetCulling(Solid(),FALSE);

	int ret;

	// apply traversal 
	GPUSH_PATH(state,this);
		ret = theShell->Traverse(state);
	GPOP_PATH(state);
	return ret;
}


//
// Gv2Cylinder
//


GV_NODE_SOURCE_IMP(Gv2Cylinder,GvNodeShell);

Gv2Cylinder::Gv2Cylinder() :
    bottom(TRUE),
    height(2),
    radius(1),
    side(TRUE),
    top(TRUE)

{
    GV_NODE_CONSTRUCTOR(Gv2Cylinder);
    isBuiltIn = TRUE;
    GV_NODE_ADD_FIELD(bottom);
    GV_NODE_ADD_FIELD(height);
    GV_NODE_ADD_FIELD(radius);
    GV_NODE_ADD_FIELD(side);
    GV_NODE_ADD_FIELD(top);

}

Gv2Cylinder::~Gv2Cylinder()
{
}

// is p outside ? 
GvBool Gv2Cylinder::isOutside(const Point &p) const 
{
	if ((p.y < (-height.value *0.5)) || (p.y > (height.value *0.5))) return TRUE; 
	float dist = sqr(p.x) + sqr(p.z);
	return dist> sqr(radius.value);
}


int			
Gv2Cylinder::BuildShell(GTraversal &state)
{
	int parts = (side ? 1 : 0) | (bottom ?  4 : 0) | (top ? 2 : 0);
	SetShell(NewCylinderVrml(0,0,0,radius,height,16,parts)); 
#ifdef _GX
    if (RTISA(theShell, GShellI)) {
  		GShellI *shellI = (GShellI *) theShell;
		if (1) {
			GShell *newS = new GShell();
			const PointArray &vn = shellI->GetVN();
			shellI->ComputeShell(newS,1);
			SetShell(newS);
		}
	}
#endif


	return(1);
}

GvBool Gv2Cylinder::getBBox(BBox &bbox)
{
	bbox.mi.Set(-radius,-height*0.5,-radius);
	bbox.ma.Set(radius,height*0.5,radius);
	return TRUE;
}


int Gv2Cylinder::Do(GBBoxTraversal &state)
{
	BBox box;

	box.mi.Set(-radius,-height*0.5,-radius);
	box.ma.Set(radius,height*0.5,radius);
	state.Union(box);
	return(1);
}

int Gv2Cylinder::Do(GglRenderTraversal &state)
{  	
	if (!theShell) BuildShell(state);
	
	if (!theShell) return (/*GvNode*/ GNode::Do(state));

	if (!CULLCHECK(theShell,state)) return TRAVERSE_CULLED;

	if (!state.appearanceApplied) {
		if (state.appearance)
			state.appearance->Apply(state);
	}

	// don't draw fully transparent geometry
	if (state.materialStatus & MATERIAL_FULLY_TRANSPARENT) {
		return 0;
    }

	// collect transparent shapes  ?
	if ( ((GGvglRenderTraversal *) & state)->IsDelayed())
	{
		if (!theShell) { // need shell for Bounding Box
		    state.glState->SetCulling(Solid(),FALSE);
			BuildShell(state);
		}
		if (theShell) {
			GTransparentElement * e= new GTransparentElement(state.appearance,this,theShell->GetBBox(),state.GetMatrixRef());
			((GGvglRenderTraversal *) & state)->AddTransparentElement(e);
		}
		return TRAVERSE_DELAYED;
	}

	// apply shape hints 
    state.glState->SetCulling(Solid(),FALSE);

	// apply traversal 
	int ret = theShell->Traverse(state);

	return(ret);
}

int Gv2Cylinder::Do(GGeometryTraversal &state)
{ 	int ret;
 	
    state.SetCulling(Solid(),FALSE);

	if (!theShell) 	BuildShell(state); 

	if (!theShell) return (0);

	ret = theShell->Traverse(state);

	return ret;
}

int Gv2Cylinder::Do(GRaySelectTraversal &state)
{  	
	if (!theShell) 	BuildShell(state); 

	if (!theShell) return (0);

	// apply shape hints 
    state.SetCulling(Solid(),FALSE);

	int ret;

	// apply traversal 
	GPUSH_PATH(state,this);
		ret = theShell->Traverse(state);
	GPOP_PATH(state);
	return ret;
}


//
// Gv2IndexedFaceSet
//


GV_NODE_SOURCE_IMP(Gv2IndexedFaceSet,GvNodeShell);


Gv2IndexedFaceSet::Gv2IndexedFaceSet() :
        ccw(TRUE),
        colorIndex(0),
        colorPerVertex(TRUE),
        convex(TRUE),
        coordIndex(0),
        creaseAngle(0),
        normalIndex(0),
        normalPerVertex(TRUE),
        solid(TRUE),
        texCoordIndex(0)
{
    GV_NODE_CONSTRUCTOR(Gv2IndexedFaceSet);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(color);
    GV_NODE_ADD_FIELD(coord);
    GV_NODE_ADD_FIELD(normal);
    GV_NODE_ADD_FIELD(texCoord);
    GV_NODE_ADD_FIELD(ccw);
    GV_NODE_ADD_FIELD(colorIndex);
    GV_NODE_ADD_FIELD(colorPerVertex);
    GV_NODE_ADD_FIELD(convex);
    GV_NODE_ADD_FIELD(coordIndex);
    GV_NODE_ADD_FIELD(creaseAngle);
    GV_NODE_ADD_FIELD(normalIndex);
    GV_NODE_ADD_FIELD(normalPerVertex);
    GV_NODE_ADD_FIELD(solid);
    GV_NODE_ADD_FIELD(texCoordIndex);

        
}

Gv2IndexedFaceSet::~Gv2IndexedFaceSet()
{
}

int Gv2IndexedFaceSet::Do(GBBoxTraversal &state)
{
	if (theShell) return GvNodeShell::Do(state);
	// compute bbox from coordinates
	return (coord.Traverse(state));
}


int Gv2IndexedFaceSet::Do(GglRenderTraversal &state)
{  	
	if (!theShell) 
		BuildShell(state);
	

	if (!theShell) return (/*GvNode*/ GNode::Do(state));

	if (!CULLCHECK(theShell,state)) 
		return TRAVERSE_CULLED;

	
	if (!state.appearanceApplied) {
		if (state.appearance)
			state.appearance->Apply(state);
	}

	// don't draw fully transparent geometry
	if (state.materialStatus & MATERIAL_FULLY_TRANSPARENT) {
		return TRAVERSE_CULLED;
    }


	// collect transparent shapes  ?
	if ( ((GGvglRenderTraversal *) & state)->IsDelayed())
	{
		if (!theShell) { // need shell for Bounding Box
		    state.glState->SetCulling(Solid(),!ccw.value);
			BuildShell(state);
		}
		if (theShell) {
			GTransparentElement * e= new GTransparentElement(state.appearance,this,theShell->GetBBox(),state.GetMatrixRef());
			((GGvglRenderTraversal *) & state)->AddTransparentElement(e);
		}
		return TRAVERSE_DELAYED;
	}

	// apply shape hints 
    state.glState->SetCulling(Solid(),!ccw.value);


	if (theShapes.getNum() >0 ) {		// traverse the expanded shapes
		return theShapes.Traverse(state);
	}


	// apply traversal 
	int ret = theShell->Traverse(state);

#ifdef _OGL
	if (theShell->vc.Length()>0 || theShell->fc.Length()>0) {
		state.material = NULL; // force reload of materials  // glcolor
		state.appearanceApplied = 0;
	}
#endif

	return(ret);
}

int Gv2IndexedFaceSet::Do(GRaySelectTraversal &state)
{  	
	// apply shape hints 
    state.SetCulling(Solid(),!ccw.value);

	// we need the shell, even if not rendered eg for Collision.proxy field
	if (!theShell) 	BuildShell(state); 

	if (!theShell) return (0);


	int ret;
	// to think: use theShapes ?

	// apply traversal 
	GPUSH_PATH(state,this);
		ret = theShell->Traverse(state);
	GPOP_PATH(state);
	return ret;
}

int Gv2IndexedFaceSet::Do(GGeometryTraversal &state)
{ 	int ret;
 	
	// apply shape hints 
    state.SetCulling(Solid(),!ccw.value);

	if (!theShell) 	BuildShell(state); 

	if (!theShell) return (0);

	ret = theShell->Traverse(state);

	return ret;
}



// try to update internal shell
// by changing internal shell properties
//
int Gv2IndexedFaceSet::OnColorChanged()
{
	if (theShell == NULL) return(1); // not build yet

	if (theShapes.getNum()>0 ) {		// bad, expanded shapes
		SetShell(NULL);
		setRedraw();
	    return(0);
	}


	GvNode * tmp = this->color.Eval();
	GvColor *color = tmp ? (RTCAST(tmp,GvColor)) : NULL;

	if (!color) {
		// clear values
		theShell->SetVC(0,NULL);
		theShell->SetFC(0,NULL);
	    setRedraw();
		return(1);
	}
	if (colorPerVertex) {
	   //if (color->color.num < (maxI.c+1)) { 
	   //	   TRACE("to few vertex colors %d %d \n",color->color.num , maxI.c+1);
	   //}
	   //else
       
		// to do: what if wrong length ??
		
	   theShell->SetVC(color->color.num,(Point*) color->color.values);
	   theShell->SetFC(0,NULL);
	   setRedraw();
	
	   return(1);

	} else { 
      //TRACE("OnColor per face changed not implemented \n");
	  //SetShell(NULL);
		int fcnt = theShell->GetFaceCount();
#if 1
		if (colorIndex.num >0)  {
			ASSERT(sizeof(Point) == (3*sizeof(color->color.values[0])));

			theShell->SetFCByIndex(colorIndex.num,(const int *) colorIndex.values,color->color.num,(Point *)color->color.values);
		}
		else 
			theShell->SetFC(color->color.num,(Point *)color->color.values);

#ifdef _D3D
		 // katmai for now 
		theShell->ComputeVertexColorsFromFaceColors();
#endif

#else
  	    PointArray fc(fcnt);

		if (colorIndex.num >0) {
						const long *vp,*vpStart = colorIndex.values;
		   				const long *vpEnd = colorIndex.values+colorIndex.num;
						int f=0;
						vp=vpStart;

						while (vp<vpEnd) {
							int ofs = *vp++;
							if (ofs<0) ofs = 0;
							if (f>=fcnt) break;
							color->color.get1(ofs,fc[f]);
					 		f++;
						}
					 } else { // colorIndex values are default 
						 int n = min(fcnt,color->color.num);
						 for (int i=0; i<n; i++) {
							color->color.get1(i,fc[i]);
						 }	
					 } 	
   	        		theShell->SetFC(fc);
#endif
      setRedraw();
	  return(0);
    }
}

int Gv2IndexedFaceSet::OnCoordChanged()
{
	if (theShell == NULL) return(1); // not build yet

	if (theShapes.getNum()>0 ) {		// bad, expanded shapes
		SetShell(NULL);
		setRedraw();
	    return(0);
	}

	GvNode * tmp = this->coord.Eval();
	GvCoordinate *coord = tmp ? (RTCAST(tmp,GvCoordinate)) : NULL;

	if (!coord) {
	    SetShell(NULL); // no good coord, no shell
	    setRedraw();

		if (this->coord.value.node == NULL) {
			TRACE("Try to set coord to 0, bad idea\n");
			return(1);
		}
		else return(0);
   }
   // to do: what if wrong length ??
   theShell->SetVByRef(coord->point.num,(Point*) coord->point.values);
   theShell->UpdateActions(); 

   tmp = this->normal.Eval();
   if (tmp) { // explicit normals, user must also animate normals !
		//theShell->UnSetAction(SHELL_COMPUTE_FNORMALS);
		theShell->UnSetAction(SHELL_COMPUTE_VNORMALS);
   }

   setRedraw();

   return(1);
}

int Gv2IndexedFaceSet::OnNormalChanged()
{
	if (theShell == NULL) return(1); // not build yet
	if (theShapes.getNum()>0 ) {		// bad, expanded shapes
		SetShell(NULL);
		setRedraw();
	    return(0);
	}

	GvNode * tmp = this->normal.Eval();
	Gv2Normal *normal = tmp ? (RTCAST(tmp,Gv2Normal)) : NULL;
	if (!normal) {
		return(0);
	}
	if (normalPerVertex) {
       // to do: what if wrong length ??
	   // speed penalty, need to re-normalize	
	   theShell->SetVNByRef(normal->vector.num,(Point*) normal->vector.values,1);

	   // will ActionFlag be cleared ?

	   setRedraw();

	   return(1);
	} else { 
      TRACE("OnNormal per face changed not implemented \n");
	  // could be ignored SetShell(NULL);
	  return(0);
    }
}

int Gv2IndexedFaceSet::OnTexCoordChanged()
{
	if (theShell == NULL) return(1); // not build yet
	if (theShapes.getNum()>0 ) {		// bad, expanded shapes
		SetShell(NULL);
		setRedraw();
	    return(0);
	}

	GvNode * tmp = this->texCoord.Eval();
	GvTextureCoordinate *coord = tmp ? (RTCAST(tmp,GvTextureCoordinate)) : NULL;

	if (!coord) {
		if (this->texCoord.value.node == NULL) {
			// clear values
			theShell->SetVP(0,NULL);
			// to do : compute default params ?
			setRedraw();

			return(1);
		}
		else return(0);
   }
   //PointArray vp;
   PointArray &vp=theShell->vp; // avoid a memcopy
   // convert from float2 to float3
   coord->point.get(vp);
   // to do: what if wrong length ??
   theShell->SetVP(vp);
   setRedraw();

   return(1);

}


// a child node changed
int Gv2IndexedFaceSet::OnChildNodeChanged(GvNode *child)
{ 
  if (child == color.value.node) return OnColorChanged();
  if (child == coord.value.node) return OnCoordChanged();
  if (child == normal.value.node) return OnNormalChanged();
  if (child == texCoord.value.node) return OnTexCoordChanged();
  return(0);
}  

// add child node , for easier editing
int Gv2IndexedFaceSet::addChild(GvNode *child)
{ 
  if (RTISA(child,GvColor)) { color.set(child);  OnColorChanged(); return(1); }
  if (RTISA(child,GvCoordinate)) { coord.set(child);  OnCoordChanged(); return(1); }
  if (RTISA(child,Gv2Normal)) { normal.set(child);  OnNormalChanged(); return(1); }
  if (RTISA(child,GvTextureCoordinate)) { texCoord.set(child);  OnTexCoordChanged(); return(1); }
  return(0);
}  

// some child node has been changed
int Gv2IndexedFaceSet::OnChildChanged(GvNode *child)
{
   return(OnChildNodeChanged(child));
}

int Gv2IndexedFaceSet::OnFieldChanged(GvField *field)
{
	if (field == &coord) 
			return OnCoordChanged();
	if (field == &color) 
			return OnColorChanged();
	if (field == &normal) 
			return OnNormalChanged();
	if (field == &texCoord) 
			return OnTexCoordChanged();

	return GvNodeShell::OnFieldChanged(field);
}



/*

  Create internal GShell or GShellI  representation for node


*/

int			
Gv2IndexedFaceSet::BuildShell(GTraversal &state)
{
	GReporter* reporter = getReporter(state);
	
	// delete any old internal shell version
	SetShell(NULL);

	// get property child nodes as proper typed nodes
    GvNode *tmp = this->coord.Eval(state);
	GvCoordinate *coord = tmp ? (RTCAST(tmp,GvCoordinate)) : NULL;
    
	tmp = this->color.Eval(state);
	GvColor *color = tmp ? (RTCAST(tmp,GvColor)) : NULL;
    
	tmp = this->normal.Eval(state);
	Gv2Normal *normal = tmp ? (RTCAST(tmp,Gv2Normal)) : NULL;
    
	tmp = this->texCoord.Eval(state);
	GvTextureCoordinate *texCoord = tmp ? (RTCAST(tmp,GvTextureCoordinate)) : NULL;

	// max number of values, -1 no values 
	int coordMax = -1;
	int normalMax = -1;
	int colorMax = -1;
	int texCoordMax = -1;

	// ignore empty nodes 
	if (coord && (coordMax = coord->point.getNum()) == 0) coord = NULL;
	if (normal && (normalMax = normal->vector.getNum()) == 0) normal = NULL;
	if (color && (colorMax=color->color.getNum()) == 0) color = NULL;
	if (texCoord && (texCoordMax=texCoord->point.getNum()) == 0) texCoord = NULL;


	// it´s in a very bad condition, no coordinates yet
	if (coord==NULL || (coordIndex.getNum() < 3)) {
		return(0);
	}

   // check for explicit default values

   if (!colorIndex.isDefault() && (colorIndex.num == 0))
   {  colorIndex.setDefault(TRUE);}
   
   if (!normalIndex.isDefault() && (normalIndex.num == 0))
   {  normalIndex.setDefault(TRUE);}

   if (!texCoordIndex.isDefault() && (texCoordIndex.num == 0))
   {  texCoordIndex.setDefault(TRUE);}
		   

	
	// gbool isFlat = (!normalPerVertex.value || (GvCreaseAngleIsFlat(creaseAngle.value) && normal == NULL)); // ccpro 3.08
	gbool isFlat = (!normalPerVertex.value || ((creaseAngle.value < 0.0001) && normal == NULL));

	// should we compute vertex normal indices based on creaseAngle value ?
	gbool doCreaseIndex = !isFlat && (normal == NULL) && normalIndex.isDefault() && (creaseAngle.value < GvAlwaysSmoothCreaseAngle);

	// color per face required  ?
	gbool colorPerFace =  (!colorPerVertex)  && color; // new 


	{
		   int doIndexed = 0; // flag if build index shell 
		   GVertexI maxI;	// maximum number of indices for each parameter
		   
		   maxI.Zero();
		   
		   
		   // check if a GShellI indexed primitive is required
		   if ((normalPerVertex) && !normalIndex.isDefault())
		   {
		   	  doIndexed = 1;
		   }
		   
		   if ((colorPerVertex)  && !colorIndex.isDefault())
		   {
		   	 doIndexed = 1;
		   }

		   
		   if (!texCoordIndex.isDefault())
		   {
		   	 doIndexed = 1;
		   }

		   IntArray creasedNormalIndex; // computed normal indices

		   // compute vertexNormalIndices based on creaseAngle
		   if (doCreaseIndex) {
			   PointArray fn;
			   int ret=computeNormalIndices(coordIndex.num,coordIndex.values,
					coord->point.num,(const Point*) coord->point.values,
					creaseAngle,ccw,
					fn,creasedNormalIndex);

			   if (ret <=0) {
				   doCreaseIndex = gfalse;
				   if (ret == -1) isFlat = gtrue;
			   } else doIndexed = 1;

		   }	


		   // pointers for stepping the coordIndex list 
		   const long *vp,*vpStart = coordIndex.values;
		   const long *vpEnd = coordIndex.values+coordIndex.num;
		   vp = vpStart;
		   
		   GFaceList f;
		   f.SetMaxLength(coordIndex.num*2);
		   
		   int fcnt=0;	// how many vertices for this face
		   int fstart=0; // where to put fcnt in f
		   int fi=0;	// the current face we are working on 
		   f.Append(0);	// room for fcnt


#ifdef _D3D
		   if (isFlat || colorPerFace) { 
			   // expand to unique normal per face at vertex
			   // we can't send a normal per face in Direct 3D
		   	   doIndexed = 1;
		   }	
#endif

		   if (doIndexed) {
			  GShellI *theShellI;

			  GVertexI vert;
			  int donormalIndex = 0;
			  int doTextureIndex = 0;
			  int docolorIndex = 0;
			  int maxColors = 0;

		      SetShell(theShellI = new GShellI());

			  ASSERT(theShellI);  	
			  
			  if (theShellI ==0) return -1;


		      theShellI->SetVByRef(coord->point.num,(Point*) coord->point.values);
			  
			  theShellI->verts.SetMaxLength(coord->point.num);
			  
			  GVertexITable vertsTable(theShellI->verts,coord->point.num);
			  
		      if (/*normal &&*/ (normalPerVertex)  &&
		         ! normalIndex.isDefault())
		      {
		   	      donormalIndex = 1;
		      }				
		      
		      if (/*texCoord  &&*/
		         !texCoordIndex.isDefault())
		      {
		   	      doTextureIndex = 1;
		      }				

		      if ((colorPerVertex)  &&
		         ! colorIndex.isDefault())
		      {
		   	      docolorIndex = 1;
		      }				

			  // theShellI->BeginVertexLookup();
			  	

		   	  while (vp<vpEnd) {
		      	long v= *vp++;
			  	if (v <0) {
					if (vp >= vpEnd) break;	// last -1 
				
					f[fstart]=fcnt;	// cnt for last face 

					if (fcnt < 3) {
					   TRACE("Bad face length  %d at offset %d \n",fcnt,vp-1-vpStart);	
					   if (reporter->verbose) 
						   reporter->Warning("IndexedFaceSet: face %d has only %d vertices at list positon %d number of coordinates %d",fi,fcnt,vp-1-vpStart,coordMax);
					   f.SetLength(fstart+1);
					} else {
		   				fstart = f.Length(); // new xx
		   				f.Append(0);	// room for fcnt
					}
					fcnt=0;

					fi++;
			  	} 
			  	else {
					// checked also at the end
					if (v >= coordMax) {
					   TRACE("Bad coord index %d at offset %d \n",v,vp-1-vpStart);	
					   if (reporter->verbose) 
						   reporter->Warning("IndexedFaceSet: coordIndex value %d at list positon %d exceedes max number of coordinates %d",v,vp-1-vpStart,coordMax);
					   
					   continue;
					}
			  		vert.v = v;
					vert.n = v;
					
					if (isFlat) vert.n = fi;

					if (doCreaseIndex) {
					   // use computed vertex normal index 
					   int ofs = vp-1- vpStart;

				       ofs = creasedNormalIndex[ofs];

					   if (ofs>=0)
						   vert.n = ofs;
					   else {
						   // happens if face has too few vertices
						   // ASSERT(0);
						   // could assign face normal index etc
					   }	

					} else if (donormalIndex) {
					   int ofs = vp-1 - vpStart;

					   if ((ofs>=0) && (ofs <normalIndex.num)) {
					       ofs = normalIndex.values[ofs];
						   if (normalMax >=0) if (ofs>=normalMax) ofs = -1;	

						   if (ofs>=0)
							   vert.n = ofs;
						   //else garbage=1;	
					   }
					} 

					vert.p = v;
					if (doTextureIndex) {
					   int ofs = vp-1 - vpStart;
					   if ((ofs>=0) && (ofs <texCoordIndex.num)) {
					      ofs = texCoordIndex.values[ofs];
						  if (texCoordMax >=0) if (ofs>=texCoordMax) ofs = -1;	

						  if (ofs>=0) 
							vert.p = ofs;
						  //else garbage=1;	
					   }
					}

					vert.c = v;
					if (docolorIndex) {
					   int ofs = vp-1 - vpStart;
					   if ((ofs>=0) && (ofs <colorIndex.num)) {
					      ofs = colorIndex.values[ofs];
						  if (colorMax >=0) if (ofs>=colorMax) ofs = -1;	
						  if (ofs>=0) 
							vert.c = ofs;
						  //else garbage=1;	
					   }
					} //else
					if (colorPerFace) 	vert.c = fi; // new : to do: could use color index

					maxI.v = max(maxI.v,vert.v);
					maxI.n = max(maxI.n,vert.n);
					maxI.c = max(maxI.c,vert.c);
					maxI.p = max(maxI.p,vert.p);

					int vi = vertsTable.Lookup(vert,1);
					
					// unoptimized function : int vi = theShellI->AppendVertex(vert,1);
			  		f.Append(vi);
			  		fcnt++; 
			  	}
			  } // while 

			   f[fstart]=fcnt; // just for savty , if not -1 terminated
			   if (fcnt < 3) {
					   TRACE("Bad face length  %d at offset %d \n",fcnt,vp-1-vpStart);	
					   if (reporter->verbose) 
						   reporter->Warning("IndexedFaceSet: face %d has only %d vertices at list positon %d number of coordinates %d",fi,fcnt,vp-1-vpStart,coordMax);
					f.SetLength(fstart); // new xx 
			   }
			   
			   //if (1) FaceListClean(f); // could remove degenerated triangles, but face information lost
			   
			   theShellI->SetEditFlag(SHELL_VERTICES_CHANGED);

			   theShellI->SetFaceList(f);

			   fcnt = theShellI->GetFaceCount();

  			   if (theShellI->v.Length() < (maxI.v+1)) { 
  			   		reporter->Error("IndexedFaceSet: to few coordinates %d Required : %d \n",theShellI->v.Length() , maxI.v+1);
					theShellI->v.SetLength(maxI.v+1);
			   }
					
			   theShellI->vn.SetLength(maxI.n+1);

		   } else {

			 // facelist constructed directly from coordIndex
			 // output is GShell
		     while (vp<vpEnd) {
		     
			  long v= *vp++; // get index 

			  if (v <0) {	// face end 
				if (vp >= vpEnd) break;	// its the last -1 
				
				f[fstart]=fcnt;

					if (fcnt < 3) {
					   TRACE("Bad face length  %d at offset %d \n",fcnt,vp-1-vpStart);	
					   if (reporter->verbose) 
						   reporter->Warning("IndexedFaceSet: face has only %d vertices at list positon %d number of coordinates %d",fcnt,vp-1-vpStart,coordMax);
					   f.SetLength(fstart+1); // new xx
					}
					else {
		   				fstart = f.Length();
		   				f.Append(0);	// room for fcnt

					}

				fcnt=0;
			  } 
			  else { 
					// checked also at the end
					if (v >= coordMax) {
					   TRACE("Bad coord index %d at offset %d \n",v,vp-1-vpStart);							
					   if (reporter->verbose) 
						   reporter->Warning("IndexedFaceSet: coordIndex value %d  at list positon %d exceedes max number of coordinates %d",v,vp-1-vpStart,coordMax);
					} else {
						if (fcnt>0 && (v == f[fstart+1])) {
							TRACE("Duplicated face vertex %d length  %d at offset %d \n",v,fcnt,vp-1-vpStart);	
						} else {
							f.Append(v);	// append new face index
			  				fcnt++; 
						}
					}
			  }
		   	}
		   	f[fstart]=fcnt; // just for savety , if not -1 terminated
			if (fcnt < 3) {
				   TRACE("Bad face length  %d at offset %d \n",fcnt,vp-1-vpStart);	
				   if (reporter->verbose) 
						   reporter->Warning("IndexedFaceSet: face has only %d vertices at list positon %d number of coordinates %d",fcnt,vp-1-vpStart,coordMax);
					f.SetLength(fstart); // new xx 
			}
#if 1
			  GShell *theShell= new GShell();

		      SetShell(theShell);

			  ASSERT(theShell);  	

			  if (theShell ==0) return -1;
			  // set data by reference !	
		   	  theShell->SetVByRef(coord->point.num,(Point*) coord->point.values);	// save cast if values = float
			  theShell->SetFaceList(f);

#else


		   	SetShell(new GShell(
		   			coord->point.num,
		   			(Point*) coord->point.values,	// save cast if values = float
		   			f.Length(),f));
#endif

			maxI.v=maxI.c=maxI.p=maxI.n=coord->point.num-1;
			
			} // !do indexed
			
			ASSERT(theShell); 
			if (!theShell) return -1;

			theShell->UpdateActions();

			// check and set additional attributres 

			if (normal) {
                if (normalPerVertex) {
				  if (normal->vector.num < (maxI.n+1)) { 
					  TRACE("to few normals %d %d \n",normal->vector.num , maxI.n+1);
					  if (reporter->verbose) reporter->Warning("IndexedFaceSet: to few vertex normals %d  Required : %d",normal->vector.num , maxI.n+1);
				  }
	  		      else	// speed penalty : author normals may be not normalized ! 
					  theShell->SetVNByRef(normal->vector.num,(Point*) normal->vector.values,1);
				} else {
					if (theShell->GetFaceCount() <= normal->vector.num) {
#ifdef _D3D
						// we had allocated a vertex normal for each face 
						theShell->SetVN(normal->vector.num, (Point *) normal->vector.values,1);
#endif

#ifndef _D3D
						// the face normal currently need to be the true geometric normal
						// used for ray-intersection etc.
						// don't trust the world builders face normal
						// can´t pass normals to D3D

//						theShell->SetFN(normal->vector.num, (Point *) normal->vector.values,1);
#endif
					}
					else {
					  TRACE("to few face normals %d %d \n",normal->vector.num , theShell->GetFaceCount());
// #ifndef _GLVIEW_IE
					 if (reporter->verbose) reporter->Warning("IndexedFaceSet: to few face normals %d  Required : %d",normal->vector.num , theShell->GetFaceCount());
// #endif
					}
					theShell->SetFlat();
				}	
					
			} // Normal

			if (color) {
				if (colorPerVertex) {
				  if (color->color.num < (maxI.c+1)) { 
					  TRACE("to few vertex colors %d  Required : %d \n",color->color.num , maxI.c+1);
//#ifndef _GLVIEW_IE
					  if (reporter->verbose) reporter->Warning("IndexedFaceSet: to few vertex colors %d  Required : %d",color->color.num , maxI.c+1);
//#endif				  
				  }
	  		      else theShell->SetVCByRef(color->color.num,(Point*) color->color.values);
				} else {
					// unindex FACE colors 
					GvMFColor *c =  &color->color;
					if (!c || (c->num <= 0)) {
						TRACE("Bad color node \n");
					} else {
#if 1
						ASSERT(sizeof(Point) == (3*sizeof(color->color.values[0])));
						if (colorIndex.num >0)  {

							theShell->SetFCByIndex(colorIndex.num,(const int *) colorIndex.values,color->color.num,(Point *)color->color.values);
					}
					else 
						theShell->SetFC(color->color.num,(Point *)color->color.values);
#else						
					 fcnt = theShell->GetFaceCount();


					 PointArray fc(fcnt);

					 if (colorIndex.num >0) {
						const long *vp,*vpStart = colorIndex.values;
		   				const long *vpEnd = colorIndex.values+colorIndex.num;
						int f=0;
						vp=vpStart;

						while (vp<vpEnd) {
							int ofs = *vp++;
							if (ofs<0) ofs = 0;
							if (f>=fcnt) break;
							c->get1(ofs,fc[f]);
					 		f++;
						}
					 } else { // colorIndex values are default 
						 int n = min(fcnt,c->num /* buuug colorIndex.num*/);
						 for (int i=0; i<n; i++) {
							c->get1(i,fc[i]);
						 }	
					 } 	
   	        		theShell->SetFC(fc);
#endif
					}
				}
			} // color 
			
			if (texCoord)
		    {
				  // PointArray vp;
 				  PointArray &vp=theShell->vp; // avoid memcopy
				  // convert from float2 to float3
				  texCoord->point.get(vp);
  				  if (vp.Length() < (maxI.p+1)) { 
  				  	TRACE("to few parameters #params %d max index %d %d \n",
  				  		vp.Length() , maxI.p+1,texCoordIndex.isDefault());
//#ifndef _GLVIEW_IE

					if (reporter->verbose) reporter->Warning("IndexedFaceSet: to few texture coordinates %d  Required : %d",texCoord->point.num , maxI.p+1);
//#endif
					// bad VRML 1.0 => 2.0 conversion or alias error 
				  	PointArray vpNew;
				    while (vpNew.Length() <  (maxI.p+1)) {
						vpNew.Append(vp);
					}
					theShell->SetVP(vpNew);

  				  }
	  		      else theShell->SetVP(vp);
		    } else {		// compute a default mapping 
 			    //PointArray vp(maxI.p+1);
 			    PointArray &vp=theShell->vp; // avoid memcopy
				vp.SetLength(maxI.p+1);

				int indexed=0;

				if (RTISA(theShell, GShellI)) {
					// ?? hg if (!texCoordIndex.isDefault()) 
					indexed = 1;
				}
				if (indexed) {
				  GShellI *s = (GShellI *) theShell;	
				  s->UpdateActions();  // to do, GetBBox should update 
				  ComputeBoxParams(s->GetBBox(),s->verts,s->v, vp);
				  s->SetVP(vp);
				} else {
				  theShell->UpdateActions();  // to do, GetBBox should update 
				  ComputeBoxParams(theShell->GetBBox(),theShell->v, vp);
				  theShell->SetVP(vp);
				}
			}				

 	};
 
  if (isFlat) 
	  theShell->SetFlat();

	if (!ccw.value) {
	/// theShell->flags |= SHELL_FLIP_NORMALS;
	}	

	if (!convex.value) {
 		// TRACE("Triangulating IndexedFaceSet\n");
		// this may add new vertices !!!!!!
 		theShell->Triangulate();
	}

 // heuristic, detect a one coordinate3 many IndexFaceSets with few faces only case, compress
 // alternative , implement a GShell by Reference 
 // TO DO : what about GShell ???? or refcnt > 0

	gbool computeSubMeshes = (theShell->f.Length() < theShell->v.Length())
		||  (theShell->v.Length() > GShell::maxSubMeshVertices);


#ifdef _D3D
    if (RTISA(theShell, GShellI)) {

		GShellI *shellI = (GShellI *) theShell;
		// computeSubMeshes=gtrue;
		
		if (shellI->fc.Length() > 0) { // we have face colors 
			// build a separate object for each color
			theShapes.flush();
			if (state.appearance != NULL)
				ExpandFaceColors(shellI, (Gv2Material*) state.appearance->material.Eval(state),state.appearance->texture.get(),state.appearance->textureTransform.get(),solid,!ccw,RTISA(shellI,GPolylineI),theShapes);
			else ExpandFaceColors(shellI, state.material,state.texture,state.textureTransform,solid,!ccw,RTISA(shellI,GPolylineI),theShapes);
			computeSubMeshes = gfalse;
		}

	}
	else {
		if (colorPerFace) // katmai for now 
			theShell->ComputeVertexColorsFromFaceColors();
	}

#endif 


	// split a big shell into smaller parts ?
	if (computeSubMeshes && theShell->subMeshes == NULL)
		theShell->CreateSubMeshes();
	
#if 0
    if (RTISA(theShell, GShellI)) {
  		GShellI *shellI = (GShellI *) theShell;

#ifdef _GX
		if (1) {	// doOwnLighting tmp new lighting 
			theShell->ComputeSubMeshes();
			/* 
			GShell *newS = new GShell();
			const PointArray &vn = shellI->GetVN();
			shellI->ComputeShell(newS,1);
			SetShell(newS);
			*/
		} else 
#endif

		
		{



#ifdef _D3D
		if (shellI->fc.Length() > 0) {
			theShapes.flush();
			if (state.appearance != NULL)
				ExpandFaceColors(shellI, (Gv2Material*) state.appearance->material.Eval(state),state.appearance->texture.get(),state.appearance->textureTransform.get(),solid,!ccw,RTISA(shellI,GPolylineI),theShapes);
			else ExpandFaceColors(shellI, state.material,state.texture,state.textureTransform,solid,!ccw,RTISA(shellI,GPolylineI),theShapes);
		}
		else 
#endif

		if (shellI->f.Length() < shellI->v.Length()) { 
			TRACE("Compressing shell : facelist length = %d, verts %d v %d n %d p%d c%d \n",
				shellI->f.Length(),shellI->verts.Length(),shellI->v.Length(),shellI->vn.Length(),shellI->vp.Length(),shellI->vc.Length());
			GShellI *newS = NULL;
			newS = CompressShell(shellI,NULL,1);
			SetShell(newS);
		}
		}
	}
#endif

 
	return(1);
}

//
// Gv2IndexedLineSet
//


GV_NODE_SOURCE_IMP(Gv2IndexedLineSet,GvNodeShell);


Gv2IndexedLineSet::Gv2IndexedLineSet() :
        colorIndex(0),
        colorPerVertex(TRUE),
        coordIndex(0)
{
    GV_NODE_CONSTRUCTOR(Gv2IndexedLineSet);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(color);
    GV_NODE_ADD_FIELD(coord);
    GV_NODE_ADD_FIELD(colorIndex);
    GV_NODE_ADD_FIELD(colorPerVertex);
    GV_NODE_ADD_FIELD(coordIndex);
        
}

Gv2IndexedLineSet::~Gv2IndexedLineSet()
{
}

int Gv2IndexedLineSet::Do(GBBoxTraversal &state)
{
	if (theShell) return GvNodeShell::Do(state);
	// compute bbox from coordinates
	return (coord.Traverse(state));
}


int Gv2IndexedLineSet::Do(GglRenderTraversal &state)
{  	
	if (!theShell) BuildShell(state);
	
	if (!theShell) return (/*GvNode*/ GNode::Do(state));

	if (!CULLCHECK(theShell,state)) return TRAVERSE_CULLED;

	if (!state.appearanceApplied) {
		if (state.appearance)
			state.appearance->Apply(state);
	}

	// don't draw fully transparent geometry
	if (state.materialStatus & MATERIAL_FULLY_TRANSPARENT) {
		return 0;
    }


	// collect transparent shapes  ?
	if ( ((GGvglRenderTraversal *) & state)->IsDelayed())
	{
		if (!theShell) { // need shell for Bounding Box
			BuildShell(state);
		}
		if (theShell) {
			GTransparentElement * e= new GTransparentElement(state.appearance,this,theShell->GetBBox(),state.GetMatrixRef());
			((GGvglRenderTraversal *) & state)->AddTransparentElement(e);
		}
		return TRAVERSE_DELAYED;
	}



	// apply shape hints 
    //state.glState->SetCulling(Solid(),!ccw.value);
	// disable lighting

	// apply traversal 
	int ret = theShell->Traverse(state);

#ifdef _OGL
	if (theShell->vc.Length()>0 || theShell->fc.Length()>0) {
		state.material = NULL; // force reload of materials  // glcolor
		state.appearanceApplied = 0;
	}
#endif

	return(ret);
}


// try to update internal shell
// by changing internal shell properties
//
int Gv2IndexedLineSet::OnColorChanged()
{
	if (theShell == NULL) return(1); // not build yet

	if (theShapes.getNum()>0 ) {		// bad, expanded shapes
		SetShell(NULL);
		setRedraw();
	    return(0);
	}

	GvNode * tmp = this->color.Eval();
	GvColor *color = tmp ? (RTCAST(tmp,GvColor)) : NULL;

	if (!color) {
		if (this->color.value.node == NULL) { /// ????
			// clear values
			theShell->SetVC(0,NULL);
			theShell->SetFC(0,NULL);
		    setRedraw();
			return(1);
		}
		else return(0);
	}
	if (colorPerVertex) {
	   //if (color->color.num < (maxI.c+1)) { 
	   //	   TRACE("to few vertex colors %d %d \n",color->color.num , maxI.c+1);
	   //}
	   //else
       
		// to do: what if wrong length ??
		
	   theShell->SetVCByRef(color->color.num,(Point*) color->color.values);
	   theShell->SetFC(0,NULL);
	   setRedraw();
	   return(1);
	} else { 
      TRACE("OnColor per face changed not implemented \n");
	  SetShell(NULL);
	  setRedraw();
	  return(0);
    }
}

int Gv2IndexedLineSet::OnCoordChanged()
{
	if (theShell == NULL) return(1); // not build yet

	if (theShapes.getNum()>0 ) {		// bad, expanded shapes
		SetShell(NULL);
		setRedraw();
	    return(0);
	}

	GvNode * tmp = this->coord.Eval();
	GvCoordinate *coord = tmp ? (RTCAST(tmp,GvCoordinate)) : NULL;

	if (!coord) {
	    SetShell(NULL); // no good coord, no shell
	    setRedraw();
		if (this->coord.value.node == NULL) {
			TRACE("Try to set coord to 0, bad idea\n");
			return(1);
		}
		else return(0);
   }
   // to do: what if wrong length ??
   theShell->SetVByRef(coord->point.num,(Point*) coord->point.values);
   theShell->UpdateActions(); 
   setRedraw();
   return(1);
}


// a child node changed
int Gv2IndexedLineSet::OnChildNodeChanged(GvNode *child)
{ 
  if (child == color.value.node) return OnColorChanged();
  if (child == coord.value.node) return OnCoordChanged();
  return(0);
}  

int Gv2IndexedLineSet::OnFieldChanged(GvField *field)
{
	if (field == &coord) 
			return OnCoordChanged();
	if (field == &color) 
			return OnColorChanged();

	return GvNodeShell::OnFieldChanged(field);
}


// add child node , for easier editing
int Gv2IndexedLineSet::addChild(GvNode *child)
{ 
  if (RTISA(child,GvColor)) { color.set(child);  OnColorChanged(); return(1); }
  if (RTISA(child,GvCoordinate)) { coord.set(child);  OnCoordChanged(); return(1); }
  return(0);
}  

// some child node has been changed
int Gv2IndexedLineSet::OnChildChanged(GvNode *child)
{
   return(OnChildNodeChanged(child));
}


int			
Gv2IndexedLineSet::BuildShell(GTraversal &state)
{
	GReporter* reporter = getReporter(state);
	
	// delete any old internal shell version
	SetShell(NULL);

	// get property child nodes as proper typed nodes
    GvNode *tmp = this->coord.Eval(state);
	GvCoordinate *coord = tmp ? (RTCAST(tmp,GvCoordinate)) : NULL;
    
	tmp = this->color.Eval(state);
	GvColor *color = tmp ? (RTCAST(tmp,GvColor)) : NULL;
    

	// ignore empty nodes 
	if (coord && coord->point.getNum() == 0) coord = NULL;
	if (color && color->color.getNum() == 0) color = NULL;

	// it´s in a very bad condition, no coordinates yet
 	if (coord==NULL || coordIndex.isDefault()) return(0);

	// color per face required  ?
	gbool colorPerFace =  (!colorPerVertex)  && color; // new 
	
	{
		   int doIndexed = 0; // flag if build index shell 
		   GVertexI maxI;
		   
		   maxI.Zero();
		   
		   // check for explicit default values

		   if (!colorIndex.isDefault() && (colorIndex.num == 0))
		   {  colorIndex.setDefault(TRUE);}

		   
		   if ((colorPerVertex)  &&
		      !colorIndex.isDefault())
		   {
		   	 doIndexed = 1;
		   }
		   


		   const long *vp,*vpStart = coordIndex.values;
		   const long *vpEnd = coordIndex.values+coordIndex.num;
		   vp = vpStart;
		   
		   GFaceList f;
		   f.SetMaxLength(coordIndex.num*2);
		   
		   int fcnt=0;	// how many vertices for this face
		   int fstart=0; // where to put fcnt in f
		   f.Append(0);	// room for fcnt

#if 1
		   if (doIndexed) {
			  GPolylineI *theShellI;

			  GVertexI vert;
			  int donormalIndex = 0;
			  int doTextureIndex = 0;
			  int docolorIndex = 0;
			  int maxColors = 0;

		      SetShell(theShellI = new GPolylineI());
			  if (theShellI == NULL) return (0);

		      theShellI->SetVByRef(coord->point.num,(Point*) coord->point.values);
			  
			  theShellI->verts.SetMaxLength(coord->point.num);
			  
			  GVertexITable vertsTable(theShellI->verts,coord->point.num);
			  

		      if ((colorPerVertex)  &&
		         ! colorIndex.isDefault())
		      {
		   	      docolorIndex = 1;
		      }				

			  // theShellI->BeginVertexLookup();
			  	
		      
		   	  while (vp<vpEnd) {
		      	long v= *vp++;
			  	if (v <0) {
					if (vp >= vpEnd) break;	// last -1 
				
					f[fstart]=fcnt;
					fcnt=0;
		   			fstart = f.Length();
		   			f.Append(0);	// room for fcnt
			  	} 
			  	else {
					// checked also at the end
					if (v>= coord->point.num) {
					   TRACE("Bad coord index %d at offset %d \n",v,vp-1-vpStart);							
					   continue;
					}
			  		vert.v = v;
					vert.n = v;
					vert.p = v;
					vert.c = v;
					if (docolorIndex) {
					   int ofs = vp-1 - vpStart;
					   if ((ofs>=0) && (ofs <colorIndex.num))
					      ofs = colorIndex.values[ofs];
						  vert.c = ofs;
					}
					maxI.v = max(maxI.v,vert.v);
					maxI.n = max(maxI.n,vert.n);
					maxI.c = max(maxI.c,vert.c);
					maxI.p = max(maxI.p,vert.p);

					int vi = vertsTable.Lookup(vert,1);
					// unoptimized : int vi = theShellI->AppendVertex(vert,1);
			  		f.Append(vi);
			  		fcnt++; 
			  	}
		   	   }
		   	   f[fstart]=fcnt; // just for savty , if not -1 terminated

			   theShellI->SetEditFlag(SHELL_VERTICES_CHANGED);

			   theShellI->SetFaceList(f);

			   fcnt = theShellI->GetFaceCount();

  			   if (theShellI->v.Length() < (maxI.v+1)) { 
  			   		reporter->Error("to few coordinates %d %d \n",theShellI->v.Length() , maxI.c+1);
					theShellI->v.SetLength(maxI.v+1);
			   }

			   theShellI->vn.SetLength(maxI.n+1);

		   } else 
#endif

		   {
	         // facelist constructed directly from coordIndex
		     while (vp<vpEnd) {
		      long v= *vp++;
			  if (v <0) {
				if (vp >= vpEnd) break;	// last -1 
				
				f[fstart]=fcnt;
				fcnt=0;
		   		fstart = f.Length();
		   		f.Append(0);	// room for fcnt
			  } 
			  else { f.Append(v);
			  		 fcnt++; 
			  }
		   	}

			f[fstart]=fcnt; // just for savety , if not -1 terminated
#if 0
		   	SetShell(new GPolyline(
		   			coord->point.num,
		   			(Point*) coord->point.values,	// save cast if values = float
		   			f.Length(),f));
#endif
			SetShell(new GPolyline ());
			if (theShell == NULL) return 0;

			theShell->SetVByRef(coord->point.num,(Point*) coord->point.values);	// save cast if values = float
		   	theShell->SetFaceList(f);

			maxI.v=maxI.c=maxI.p=maxI.n=coord->point.num-1;
			
			} // !do indexed
			

			theShell->UpdateActions();
	

			if (color) {
				if (colorPerVertex) {
					// to do : unindex 
/*
					if (colorIndex.num >= coordIndex.num) {
						GvMFColor *c =  &color->color;
						PointArray vc(coord->point.num);
						for (int i=0;

					} else
*/
					if (color->color.num < (maxI.c+1)) { 
						TRACE("to few vertex colors %d %d \n",color->color.num , maxI.c+1);
					    if (reporter->verbose) reporter->Warning("IndexedLineSet: to few vertex colors %d  Required : %d",color->color.num , maxI.c+1);
					}
	  				else theShell->SetVCByRef(color->color.num,(Point*) color->color.values);

				} else {
					// unindex FACE colors 
					GvMFColor *c =  &color->color;
					if (!c || (c->num <= 0)) {
						TRACE("Bad color node \n");
					} else {
					 
					 fcnt = theShell->GetFaceCount();
					 PointArray fc(fcnt);
					 const long *vp,*vpStart = colorIndex.values;
		   			 const long *vpEnd = colorIndex.values+colorIndex.num;
					 int f=0;
					 vp=vpStart;
					 while (vp<vpEnd) {
					    int ofs = *vp++;
						if (ofs<0) ofs = 0;
						if (f>=fcnt) break;
						c->get1(ofs,fc[f]);
					 	f++;
					 }
   	        		 theShell->SetFC(fc);
					}
				}
			} // color 

 	};

#ifdef _D3D
	{
		if (colorPerFace) // katmai for now 
			theShell->ComputeVertexColorsFromFaceColors();
	}
#endif

 // heuristic, detect a one coordinate3 many IndexFaceSets with few faces only case, compress
 // alternative , implement a GShell by Reference 
 // TO DO : what about GShell ???? or refcnt > 0
 if (RTISA(theShell, GShellI)) {
  	GShellI *shellI = (GShellI *) theShell;

	if (shellI->f.Length() < shellI->v.Length()) { 
		TRACE("Compressing shell : facelist length = %d, verts %d v %d n %d p%d c%d \n",
				shellI->f.Length(),shellI->verts.Length(),shellI->v.Length(),shellI->vn.Length(),shellI->vp.Length(),shellI->vc.Length());
		GShellI *newS = NULL;
		newS = CompressShell(shellI,NULL,1,0);
		SetShell(newS);
	}
 }

 return(1);
}



//
// Gv2PointSet
//


GV_NODE_SOURCE_IMP(Gv2PointSet,GvNodeShell);


Gv2PointSet::Gv2PointSet() 
{
    GV_NODE_CONSTRUCTOR(Gv2IndexedPointSet);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(color);
    GV_NODE_ADD_FIELD(coord);
        
}

Gv2PointSet::~Gv2PointSet()
{
}



int Gv2PointSet::Do(GBBoxTraversal &state)
{
	if (theShell) return GvNodeShell::Do(state);
	// compute bbox from coordinates
	return (coord.Traverse(state));
}


int Gv2PointSet::Do(GglRenderTraversal &state)
{  	
	if (!theShell) BuildShell(state);
	
	if (!theShell) return (/*GvNode*/ GNode::Do(state));

	if (!CULLCHECK(theShell,state)) return TRAVERSE_CULLED;


	if (!state.appearanceApplied) {
		if (state.appearance)
			state.appearance->Apply(state);
	}

	// don't draw fully transparent geometry
	if (state.materialStatus & MATERIAL_FULLY_TRANSPARENT) {
		return 0;
    }

	// apply shape hints 
    //state.glState->SetCulling(Solid(),!ccw.value);
	// disable lighting

	// apply traversal 
	int ret = theShell->Traverse(state);

#ifdef _OGL
	if (theShell->vc.Length()>0 || theShell->fc.Length()>0) {
		state.material = NULL; // force reload of materials  // glcolor
		state.appearanceApplied = 0;
	}
#endif

	return(ret);
}


// try to update internal shell
// by changing internal shell properties
//
int Gv2PointSet::OnColorChanged()
{
	if (theShell == NULL) return(1); // not build yet

	if (theShapes.getNum()>0 ) {		// bad, expanded shapes
		SetShell(NULL);
		setRedraw();
	    return(0);
	}

	GvNode * tmp = this->color.Eval();
	GvColor *color = tmp ? (RTCAST(tmp,GvColor)) : NULL;

	if (!color) {
		if (this->color.value.node == NULL) { /// ????
			// clear values
			theShell->SetVC(0,NULL);
			theShell->SetFC(0,NULL);
		    setRedraw();
			return(1);
		}
		else return(0);
	}
	if (TRUE) { // colorPerVertex
	   //if (color->color.num < (maxI.c+1)) { 
	   //	   TRACE("to few vertex colors %d %d \n",color->color.num , maxI.c+1);
	   //}
	   //else
       
		// to do: what if wrong length ??
		
	   theShell->SetVCByRef(color->color.num,(Point*) color->color.values);
	   theShell->SetFC(0,NULL);
	   setRedraw();
	   return(1);
	} else { 
      TRACE("OnColor per face changed not implemented \n");
	  SetShell(NULL);
      setRedraw();
	  return(0);
    }
}

int Gv2PointSet::OnCoordChanged()
{
	if (theShell == NULL) return(1); // not build yet
	if (theShapes.getNum()>0 ) {		// bad, expanded shapes
		SetShell(NULL);
		setRedraw();
	    return(0);
	}

	
	GvNode * tmp = this->coord.Eval();
	GvCoordinate *coord = tmp ? (RTCAST(tmp,GvCoordinate)) : NULL;

	if (!coord) {
	    SetShell(NULL); // no good coord, no shell
		setRedraw();
		if (this->coord.value.node == NULL) {
			TRACE("Try to set coord to 0, bad idea\n");
			return(1);
		}
		else return(0);
   }
   // to do: what if wrong length ??
   theShell->SetVByRef(coord->point.num,(Point*) coord->point.values);
   theShell->UpdateActions(); 
   setRedraw();
   return(1);
}


// a child node changed
int Gv2PointSet::OnChildNodeChanged(GvNode *child)
{ 
  if (child == color.value.node) return OnColorChanged();
  if (child == coord.value.node) return OnCoordChanged();
  return(0);
}  

// add child node , for easier editing
int Gv2PointSet::addChild(GvNode *child)
{ 
  if (RTISA(child,GvColor)) { color.set(child);  OnColorChanged(); return(1); }
  if (RTISA(child,GvCoordinate)) { coord.set(child);  OnCoordChanged(); return(1); }
  return(0);
}  

// some child node has been changed
int Gv2PointSet::OnChildChanged(GvNode *child)
{
   return(OnChildNodeChanged(child));
}


int			
Gv2PointSet::BuildShell(GTraversal &state)
{
	// delete any old internal shell version
	SetShell(NULL);

	// get property child nodes as proper typed nodes
    GvNode *tmp = this->coord.Eval(state);
	GvCoordinate *coord = tmp ? (RTCAST(tmp,GvCoordinate)) : NULL;
    
	tmp = this->color.Eval(state);
	GvColor *color = tmp ? (RTCAST(tmp,GvColor)) : NULL;
    

	// ignore empty nodes 
	if (coord && coord->point.getNum() == 0) coord = NULL;
	if (color && color->color.getNum() == 0) color = NULL;


	// it´s in a very bad condition, no coordinates yet

 	if (coord == NULL) return(0);
	
   	SetShell(new GPoints(coord->point.num,
		   			(Point*) coord->point.values	// save cast if values = float
		   	)		);

	if (color) {
		if (color->color.num < coord->point.num) { 
			TRACE("to few vertex colors %d %d \n",color->color.num , coord->point.num);
		}
 		else theShell->SetVC(color->color.num,(Point*) color->color.values);

	}
 return(1);
}



//
// Gv2Sphere
//


GV_NODE_SOURCE_IMP(Gv2Sphere,GvNodeShell);


Gv2Sphere::Gv2Sphere() :
        radius(1)
{
    GV_NODE_CONSTRUCTOR(Gv2Sphere);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(radius);
        
}

Gv2Sphere::~Gv2Sphere()
{
}

// is p outside ? 
GvBool Gv2Sphere::isOutside(const Point &p) const 
{
	float dist = p.Length();
	return dist>radius.value;
}


int			
Gv2Sphere::BuildShell(GTraversal &state)
{
   
//   SetShell(new GSphereSubdiv(Point(0,0,0),radius,1,1));
   SetShell(NewSphereVrml(0,0,0,radius,12,6,NULL,NULL));
#ifdef _GX
    if (RTISA(theShell, GShellI)) {
  		GShellI *shellI = (GShellI *) theShell;
		if (1) {
			GShell *newS = new GShell();
			const PointArray &vn = shellI->GetVN();
			shellI->ComputeShell(newS,1);
			SetShell(newS);
		}
	}
#endif


   return(1);
}


GvBool Gv2Sphere::getBBox(BBox &bbox)
{
	bbox.mi.Set(-radius,-radius,-radius);
	bbox.ma.Set(radius,radius,radius);
	return TRUE;
}

int Gv2Sphere::Do(GBBoxTraversal &state)
{
  BBox box;

  box.mi.Set(-radius,-radius,-radius);
  box.ma.Set(radius,radius,radius);
  state.Union(box);
  return(1);
}


int Gv2Sphere::Do(GglRenderTraversal &state)
{  	
	if (!theShell) BuildShell(state);
	
	if (!theShell) return (/*GvNode*/ GNode::Do(state));

	if (!CULLCHECK(theShell,state)) return TRAVERSE_CULLED;

	if (!state.appearanceApplied) {
		if (state.appearance)
			state.appearance->Apply(state);
	}

	// don't draw fully transparent geometry
	if (state.materialStatus & MATERIAL_FULLY_TRANSPARENT) {
		return 0;
    }

	// collect transparent shapes  ?
	if ( ((GGvglRenderTraversal *) & state)->IsDelayed())
	{
		if (!theShell) { // need shell for Bounding Box
		    state.glState->SetCulling(Solid(),FALSE);
			BuildShell(state);
		}
		if (theShell) {
			GTransparentElement * e= new GTransparentElement(state.appearance,this,theShell->GetBBox(),state.GetMatrixRef());
			((GGvglRenderTraversal *) & state)->AddTransparentElement(e);
		}
		return TRAVERSE_DELAYED;
	}

	// apply shape hints 
    state.glState->SetCulling(Solid(),FALSE);

	// apply traversal 
	int ret = theShell->Traverse(state);

	return(ret);
}


int Gv2Sphere::Do(GGeometryTraversal &state)
{ 	int ret;
 	
    state.SetCulling(Solid(),FALSE);

	if (!theShell) 	BuildShell(state); 

	if (!theShell) return (0);

	ret = theShell->Traverse(state);

	return ret;
}


//
// Gv2Text
//


GV_NODE_SOURCE_IMP(Gv2Text,GvNodeShell);


Gv2Text::Gv2Text() :
        string(0),
        length(0),
        maxExtent(0),
		solid(FALSE)
{
    GV_NODE_CONSTRUCTOR(Gv2Text);
    isBuiltIn = TRUE;
    GV_NODE_ADD_FIELD(string);
    GV_NODE_ADD_FIELD(fontStyle);
    GV_NODE_ADD_FIELD(length);
    GV_NODE_ADD_FIELD(maxExtent);
       
}

Gv2Text::~Gv2Text()
{
}


// a child node changed
int Gv2Text::OnChildNodeChanged(GvNode *child)
{ 
	return OnFieldChanged(&fontStyle); // will flush shell and rebuild
}


extern int GShell_GLUTriangulationHoleCode;



// default 16*16 characters per bitmap 

// bitmap character configuration 
class GBitmapTextConfig {
public :
	int cols, rows; // grid spacing in texture
	int minChar,maxChar; // minimal maximal character (grid starts at minChar)
	float aspect,extraSpace; // width factor, extra spacing factor
	float shift;	// upper italic shift factor

	GBitmapTextConfig() { cols=rows=16;minChar =0; maxChar=255; 
				aspect = 0.8; extraSpace =0.0f; shift =0.0; }
};



void ComputeBitmapTextChar(
	GBitmapTextConfig &config,
	const  _TCHAR c, 
	float x,float y,float w,float h,
	gbool italic,
 	GShell &s)		
{


//set the UV's into the font texture.
		float fCharOffsetx = 1.0/(float)config.cols; //the fraction between 0&1 per char
		float fCharOffsety = 1.0/(float)config.rows; //the fraction between 0&1 per char
		float fUStart = (float)(((int)c) % config.cols)*fCharOffsetx;
		float fVStart = 1.0f-(float)(1+(((int)c) / config.cols))*fCharOffsety;
		float onePix = 1.0f / 511.0f;
		
		fCharOffsetx -= onePix;
		fCharOffsety -= onePix;

		float shift= config.shift*w;
		if (italic) shift += 0.4*w;

		
		PointArray &v = s.v;
		PointArray &vp = s.vp;
		
		int i = v.Length();

		v.SetLength(i+4);
		vp.SetLength(i+4);
		// ccw 
		v[i].Set(x,y,0);	

		v[i+1].Set(x+w,y,0);	
		v[i+2].Set(x+w+shift,y+h,0);	
		v[i+3].Set(x+shift,y+h,0);	

		vp[i].Set(fUStart,fVStart,0);	
		vp[i+1].Set(fUStart+fCharOffsetx,fVStart,0);	
		vp[i+2].Set(fUStart+fCharOffsetx,fVStart+fCharOffsety,0);	
		vp[i+3].Set(fUStart,fVStart+fCharOffsety,0);
		s.f.AppendQuad(i,i+1,i+2,i+3);
		
}


#if defined(_WIN32)

//#include <tchar.h>
#include <mbstring.h>

#else

#define _mbstrlen(xx) strlen(xx)
#define _tclen(xx) strlen(xx)

#define _mbsnextc(xx) *(xx)
#define _mbsinc(xx) ((xx)+1)

#define _tcsnextc(xx) *(xx)
#define _tcsinc(xx) ((xx)+1)

#endif





int ComputeBitmapText(
	GBitmapTextConfig &config, // configuration data

	const  _TCHAR *text, // the input text, to compute a polygon outline for

 	float &x, float &y, // starting x,y position, will be updated after the call to new position
	float spacing, // 0.0 extra character spacing
	float width,  // 0.0
	float height,	// = 1.0,
	float dirstepx, // 1.0
	float dirstepy,// -1.0
	float lineSpacing,	// = 1.0,
	int align, 
	float length,
	gbool italic,
 	GShell &s, // the output <c GShell> ("2d"),
	float &resultMaxLength)		
{ 

  	
  float scaleFac = height;
  
  float charWidth = config.aspect*scaleFac;
  float charHeight = 1.0*scaleFac;

  int len;
  
  unsigned char * mbsp;

#ifndef _UNICODE   
  gbool mbcs=gtrue;

  if (mbcs) {
	 mbsp = (unsigned char *) text;
	 len = _mbstrlen(text);
  } else 
	  len=_tclen(text);
#else
  gbool mbcs=gfalse;
  len=_tclen(text);
#endif

  // float spacing = 1.0;
  float xstart = x;
  

  int tstart = 0;

  int vstart = s.v.Length();

  int i=0;
  while (i<len) { 

  // to do : alignment left / right / center
  while (i < len) {

	unsigned int c;
	// get next character 
	if (mbcs) {
		c = _mbsnextc(mbsp);
		mbsp= _mbsinc(mbsp);
	} else {
		c =_tcsnextc(text);
	    text =_tcsinc(text);
	}

    if (c == 13) {
		unsigned int cnext;

		if (mbcs) {
			cnext = _mbsnextc(mbsp);
		} else {
			cnext =_tcsnextc(text);
		}

		if (cnext == '\n') {} // ignore and process ass /n
		else x = xstart;

		i++; 
		continue; 
	}		
   	else 
	if (c == '\n') {
		i++;
		y+= dirstepy * charHeight*lineSpacing;
		break;

	}
	else 
  	{ 
	  if (c>=config.minChar && c<=config.maxChar)	
  		ComputeBitmapTextChar(config,c-config.minChar,x,y,charWidth,charHeight,italic,s);
	  x+=  dirstepx * charWidth * (1.0+spacing);
	  i++;
	}
	
  }
  // do after line processing 
  float width = x-xstart;
  float offset;

  if (length > 0.0 && width >0) {
	  
	  float fac = length/width;
	  for (int vi=vstart ; vi<s.v.Length(); vi++) s.v[vi].x = xstart+(s.v[vi].x-xstart)*fac;
	  width = length;
  }	

  if (align <0) offset = 0;
  else if (align >0) offset = - width;
  else offset = - width * 0.5;

  for (int vi=vstart ; vi<s.v.Length(); vi++) s.v[vi].x += offset;
  
  x=xstart;	
  tstart = i;
  vstart = s.v.Length();
  resultMaxLength=max(resultMaxLength,width);

  }
  return(0);
};






int			
Gv2Text::BuildShell(GTraversal &state)
{

	SetShell(NULL);
	if (string.getNum() == 0) {
		// nothing to do 
		return 0;
	}

	// get property child nodes as proper typed nodes
	GvNode *n = this->fontStyle.Eval(state);
	Gv2FontStyle *fontStyle = n ? (RTCAST(this->fontStyle.value.node,Gv2FontStyle)) : NULL;


	int m_invert=0;
	gbool m_extrude=0;
	gbool m_outline=0;
	gbool m_bitmap=0;
	gbool m_italic=0;
	GBitmapTextConfig config; // configuration for textfont 

#ifdef WIN32

		// logical font description
   		LOGFONT m_lf;

		m_lf.lfHeight=75;
		//m_lf.lfHeight=100;

	    m_lf.lfWidth=0;
	    m_lf.lfEscapement=m_lf.lfOrientation=0;
	    m_lf.lfWeight=FW_NORMAL;
		
		if (fontStyle && strstr(fontStyle->style,"BOLD"))  m_lf.lfWeight = 700;
	    
	    m_lf.lfItalic=0;
		if (fontStyle &&  strstr(fontStyle->style,"ITALIC"))  m_lf.lfItalic = m_italic=1;
	    
	    m_lf.lfUnderline=0;
		if (fontStyle &&  strstr(fontStyle->style, "UNDERLINE"))  m_lf.lfUnderline = 1;
	    
	    m_lf.lfStrikeOut=0;
		if (fontStyle &&  strstr(fontStyle->style,"STRIKEOUT"))  m_lf.lfStrikeOut = 1;
		
		if (fontStyle &&  strstr(fontStyle->style,"INVERT"))  m_invert = 1; 

		if (fontStyle &&  strstr(fontStyle->style,"EXTRUDE"))  m_extrude = 1; 
		if (fontStyle &&  strstr(fontStyle->style,"OUTLINE"))  m_outline = 1; 

	    m_lf.lfCharSet=ANSI_CHARSET;
	    m_lf.lfOutPrecision=OUT_TT_ONLY_PRECIS;
	    m_lf.lfClipPrecision=CLIP_DEFAULT_PRECIS;
	    m_lf.lfQuality=PROOF_QUALITY; //DEFAULT_QUALITY;
	    m_lf.lfPitchAndFamily=FF_DONTCARE;
	    
	    lstrcpy(m_lf.lfFaceName,"Arial");
		if (fontStyle) {
			if (fontStyle->family.getNum()>0) {
			const char *fam = fontStyle->family.get1(0);
			if (strprefix(fam,"TEXTURE")) {
	    		m_bitmap =1;
				fam +=strlen("TEXTURE");
				if (strlen(fam) >0) { // rows after RC 1
					sscanf(fam,"%d %d %d %d %f %f %f",&config.minChar,&config.maxChar,&config.cols,&config.rows,&config.aspect,&config.extraSpace,&config.shift);
				}


			} 
			else if (fontStyle->family.get1(0) == "SANS") 
	    		lstrcpy(m_lf.lfFaceName,"Arial");
			else if (fontStyle->family.get1(0) == "SERIF") 
	    		lstrcpy(m_lf.lfFaceName,"Times New Roman");
			else if (fontStyle->family.get1(0) == "TYPEWRITER") 
	    		lstrcpy(m_lf.lfFaceName,"Courier New");
			else 
				lstrcpy(m_lf.lfFaceName,fontStyle->family.get1(0).getString()); // to do unicode
			}
			 {
				const char *lang = fontStyle->language.get();
				if (streq(lang,"SYMBOL"))  m_lf.lfCharSet = SYMBOL_CHARSET; // test
				else if (streq(lang,"OEM"))  m_lf.lfCharSet = OEM_CHARSET ; // test
				else if (streq(lang,"DEFAULT"))  m_lf.lfCharSet = DEFAULT_CHARSET;  // test

			}	
		}
#else
		if (fontStyle &&  strstr(fontStyle->style,"ITALIC"))  m_italic=1;
	    
		if (fontStyle) {
			if (fontStyle->family.getNum()>0) {
			const char *fam = fontStyle->family.get1(0);
			if (strprefix(fam,"TEXTURE")) {
	    		m_bitmap =1;
				fam +=strlen("TEXTURE");
				if (strlen(fam) >0) {
					sscanf(fam,"%d %d %d %d %f %f %f",&config.minChar,&config.maxChar,&config.cols,&config.rows,&config.aspect,&config.extraSpace,&config.shift);
				}
			} 
		}

#endif

//     SERIF       Serif style (such as TimesRoman)
 //    SANS        Sans Serif Style (such as Helvetica)
 //    TYPEWRITER  Fixed pitch style (such as Courier)

		float height = 1.0;
		float spacing = 1.0;

		if (fontStyle)  {
			height = fontStyle->size.value;
			spacing = fontStyle->spacing.value;
		}


		int m_parameters = 1;
		
		
		//int m_top = parts & TOP;
		//int m_bottom= parts & BOTTOM;

		float m_invert_factor = 2.0;

		int align = -1;
		int yalign = -1;

		if (fontStyle && fontStyle->justify.getNum()>0) {
			const GvString &justification = fontStyle->justify.values[0];
			if (justification == "FIRST") align = -1;
			if (justification == "BEGIN") align = -1;
			else if (justification == "MIDDLE") align = 0;
			else if (justification == "END") align = 1;
			if (fontStyle->justify.getNum()>1) {
				const GvString &justification = fontStyle->justify.values[1];
				if (justification == "FIRST") yalign = -2;
				if (justification == "BEGIN") yalign = -1;
				else if (justification == "MIDDLE") yalign = 0;
				else if (justification == "END") yalign = 1;
			}
		}
		float dirstepx =1.0;
		float dirstepy = -1.0;

		if (fontStyle) {
			if (!fontStyle->leftToRight) dirstepx=-1.0;
			if (!fontStyle->topToBottom  ) dirstepy= 1.0;
		}



		if (!state.view) return(0);

		if (m_bitmap) {

			GShell *s;
			float x=0.0,y=0.0;
					
			s = new GShell();

			float widthi=0;
			float maxWidth = maxExtent; // maximum width if present (local coord space)
			float maxLength=0.0;
			

        	
			for(int line=0; line <string.num; line ++) {			
				CString m_text(string[line]);
			 
				if (line < length.getNum()) widthi= length.get1(line);
				else widthi = 0;
				m_text += "\n";

				//to do: kernning // center, multiline
				
				ComputeBitmapText(config,m_text,x,y,config.extraSpace,widthi,height, dirstepx,dirstepy, spacing ,align, widthi, m_italic,*s,maxLength);

			} // for line

			if (maxWidth>0.0 && maxLength >maxExtent) {
				// scale by maxWidth/maxLength
		
				float fac =maxExtent/maxLength;
				for (int vi=0 ; vi<s->v.Length(); vi++) s->v[vi].x *= fac;

			}

			s->SetFlag(SHELL_PLANAR);

		   SetShell(s);


		
		} else {

#ifdef WIN32
		HFONT font;
		//m_lf.lfHeight=80;
		//m_lf.lfHeight = -MulDiv(20, GetDeviceCaps(pDC->m_hDC, LOGPIXELSY), 72);

		height *= 0.133; // font units ??

		if ((font = ::CreateFontIndirect(&m_lf)) != NULL)
		{

			HDC hdc = ::GetDC(NULL);
			HGDIOBJ	oldFont = ::SelectObject(hdc,font);


			GShell *s;
			float x=0.0,y=0.0;

			
			if (m_outline) s = new GPolyline;
			else s = new GShell();

			float widthi=0;
			float maxWidth = maxExtent; // maximum width if present (local coord space)
			float maxLength=0.0;
			
        	
			for(int line=0; line <string.num; line ++) {			
				CString m_text(string[line]);
			 
				if (line < length.getNum()) widthi= length.get1(line);
				else widthi = 0;

				//CSize size = pDC->GetTextExtent(m_text,strlen(m_text));
				//TRACE("Text %s Size %d %d %f %f\n", m_text,(int) size.cx, (int) size.cy,x,y);

				m_text += "\n";

				//to do: kernning // center, multiline

				ComputeGlyphOutline(hdc,m_text,x,y,0.0,height,dirstepx,dirstepy, spacing ,align,widthi,m_extrude,*s,maxLength);
				//maxLength=max(maxLength,thisLength);


			} // for line

			if (maxWidth>0.0 && maxLength >maxExtent) {
				// scale by maxWidth/maxLength
				// scale by maxWidth/maxLength
		
				float fac =maxExtent/maxLength;
				for (int vi=0 ; vi<s->v.Length(); vi++) s->v[vi].x *= fac;

			}

			s->SetFlag(SHELL_PLANAR);
			solid = FALSE;


#if 1
			if (m_invert) {
			   GFaceList fout;
			   BBox box;
			   s->ComputeBBox(box);
			   float scale =m_invert_factor;
			   if (scale != 0.0)
			      box.ScaleCenter(Point(scale,scale,scale));

			   int flist[9];

			   fout.SetMaxLength(s->f.Length()+9);

			   flist[0] = 4;
			   flist[1] = s->AppendV(box.mi);
			   flist[2] = s->AppendV(Point(box.mi.x,box.ma.y,box.mi.z));
			   flist[3] = s->AppendV(box.ma);
			   flist[4] = s->AppendV(Point(box.ma.x,box.mi.y,box.mi.z));
			   fout.Append(5,flist);

			   ComputeInvertedFaceList(s->f,fout);

			   s->SetFaceList(fout);
			}

	 		
			// make 3d 
			if(m_extrude) {
			   PointArray offsets(2);

			   offsets[0] = Point(0.0,0.0,0.0);
			   offsets[1] = Point(0.0,0.0,1.0);


			   //istyle flag = ICLOSE | IHARDTOPBOTTOM | (m_top ? ITOP:0)| (m_bottom ? IBOTTOM:0) | (m_parameters ? IPARAMETERS:0);
			   istyle flag = ICLOSE | IHARDTOPBOTTOM | ITOP | IBOTTOM | IPARAMETERS ;
			   if (m_outline) flag |= IQUAD | ILINES; 
			   GShell* s3d = ComputeExtrudedShell(flag,*s,offsets);
			   // solid = TRUE; tbd back face must be flipped
			   
			   delete s;
			   s = s3d;

			}
#endif
	   		s->UnSetFlag(SHELL_TRIANGULATED);

			if (!m_extrude) {
				s->SetFlat(); // is flat shaded 
			}

			s->SetFlag(SHELL_HAS_CONVEX_FACES);
			if (!m_outline) {
#ifdef _OGL
			GShell_GLUTriangulationHoleCode=GLU_UNKNOWN;

			s->Triangulate(); // temp

			GShell_GLUTriangulationHoleCode=GLU_INTERIOR;
#else
			s->Triangulate(); // temp

#endif
			}

			if (theShell == NULL)
			   SetShell(s);
			else { 
				theShell->Append(*s);
				delete s; s= NULL;
			}

			if (theShell) { // compute texture parameters, mapping is not defined in VRML 2.0 spec
			theShell->UpdateActions();  //  
			PointArray &vp=theShell->vp; // avoid memcopy
			const PointArray &p=theShell->v; 
			vp.SetLength(theShell->v.Length());

			const BBox &bbox = theShell->GetBBox();
			Point size = bbox.Size();

			if (size.x == 0.0) size.x = 1.0;
			
			float xscale= string.num;
			float sdiv=min(size.x,size.y);

			// compute scaling / translation params
			float soffset = -bbox.mi.x;
			float sscale = xscale/ sdiv;

			float toffset = -bbox.mi.y;
			float tscale = xscale / sdiv; // from s !


			// map points
			for(int i=0; i<p.Length(); i++) {
  				float fs = (p[i].x + soffset) * sscale;
  				float ft = (p[i].y + toffset) * tscale ;

				vp[i].Set( fs, ft ,0.0);
			}
			//ComputeBoxParams(theShell->GetBBox(),theShell->v, vp);
			theShell->SetVP(vp);
			}

			if (oldFont) ::SelectObject(hdc, oldFont);
			::ReleaseDC(NULL,hdc);
			::DeleteObject(font);
#else 
    return(0);
#endif	// WIN32	

		} // font
   } //!bitmap 	
   return(1);


}


int Gv2Text::Do(GglRenderTraversal &state)
{
	
//	if (state.glState->dx6)
//		AppOutputText((LPDIRECT3DDEVICE3) state.glState->lpD3DDevice3, 10, 10, "versuch");

	
	if (!theShell) 
		BuildShell(state);

	if (!theShell) return (/*GvNode*/ GNode::Do(state));

	if (!CULLCHECK(theShell,state)) return TRAVERSE_CULLED;

	if (!state.appearanceApplied) {
		if (state.appearance)
			state.appearance->Apply(state);
	}

	// don't draw fully transparent geometry
	if (state.materialStatus & MATERIAL_FULLY_TRANSPARENT) {
		return 0;
    }

	// collect transparent shapes  ?
	if ( ((GGvglRenderTraversal *) & state)->IsDelayed())
	{
		if (theShell) {
			GTransparentElement * e= new GTransparentElement(state.appearance,this,theShell->GetBBox(),state.GetMatrixRef());
			((GGvglRenderTraversal *) & state)->AddTransparentElement(e);
		}
		return TRAVERSE_DELAYED;
	}

	// apply shape hints 
    state.glState->SetCulling(Solid(),TRUE);



	// apply traversal 
	int ret = theShell->Traverse(state);

	return(ret);
}


int Gv2Text::Do(GRaySelectTraversal &state)
{  	
	if (!theShell) BuildShell(state);

	if (!theShell) return (0);

	// apply shape hints 
    state.SetCulling(Solid(),FALSE);

	int ret;

	if (state.mode >= GRaySelectTraversal::MODE_COLLISION)
		ret = 0;
	else {
	// apply traversal 
	GPUSH_PATH(state,this);
		ret = theShell->Traverse(state);
	GPOP_PATH(state);
	}
	return ret;


}



//
// GvExtrusion
//


#include <gvextrusion.h>

#include "gshell.h"


GV_NODE_SOURCE_IMP(GvExtrusion,GvNodeShell);

GvExtrusion::GvExtrusion() :
    beginCap(TRUE),ccw(TRUE),convex(TRUE),creaseAngle(0),
    endCap(TRUE),orientation(0),scale(1),
    solid(TRUE)
{
    GV_NODE_CONSTRUCTOR(GvExtrusion);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(beginCap);
    GV_NODE_ADD_FIELD(ccw);
    GV_NODE_ADD_FIELD(convex);
    GV_NODE_ADD_FIELD(creaseAngle);

    GV_NODE_ADD_FIELD(crossSection);
    GV_NODE_ADD_FIELD(endCap);
    GV_NODE_ADD_FIELD(orientation);
    GV_NODE_ADD_FIELD(scale);
    GV_NODE_ADD_FIELD(spine);
    GV_NODE_ADD_FIELD(solid);


    const float spineDefault[] ={ 0, 0, 0,   0, 1, 0 };
    const float crossSectionDefault[] = { 1, 1,   1, -1,  -1,-1,   -1,1,  1,1 };
    
    spine.set(2, spineDefault);
    crossSection.set(5, crossSectionDefault);
    
    scale.values[0] = 1;
    scale.values[1] = 1;
}


GvExtrusion::~GvExtrusion()
{
}


int GvExtrusion::BuildShell(GTraversal &state)
{


PointArray spine;
this->spine.get(spine);

PointArray crossSection;
this->crossSection.get(crossSection);

int section;  // the current section
int nSections = spine.Length();


if ( (nSections <=0) || (crossSection.Length()<1)) {
   SetShell(NULL);
   return(0);
}

// put crossSection from xy into xz plane
for(int i=0; i<crossSection.Length(); i++) {
    crossSection[i].z = crossSection[i].y;
    crossSection[i].y = 0;
}



GvBool spineClosed = (nSections>2) && Equal(spine[0],spine[spine.Length()-1],EPS);

GvBool sectionClosed = (crossSection.Length()>2) && Equal(crossSection[0],crossSection[crossSection.Length()-1],EPS);

int nScale = scale.Length();
int nOrientation = orientation.Length();

if (nScale >  spine.Length())
{
	// not supported 
}
			  
Point tangent;	// y axis 
Point tangentOld(0,1,0);
Point Z(0,0,1),X(1,0,0);

// total number of points 
int npts = nSections * crossSection.Length();

// closing ?
gbool closeBegin = !spineClosed && beginCap;
gbool closeEnd = !spineClosed && endCap;


PointArray pts(npts);
// tangents (y-Axis) & z-Axis at each spine point
PointArray tangents(nSections);
PointArray zs(nSections);
int firstNonZeroZ=-1;
int firstNonZeroY=-1;

// compute tangents
for(section=0; section<nSections; section++) {
   Point v1,v2;
   Z.Set(0,0,0);	
   if (section == 0)  // first segment 
   {
	  if (nSections <=1) {
		  v1.Set(0,1,0);
		  v2.Set(0,1,0);
		  tangent = v1; 
		  // ???
	  } else {	

	  v2 = (spine[1] - spine[0]);
		  
      if (spineClosed) {  // last == first 
         tangent = spine[1]-spine[nSections-2];
         // v1 = spine[nSections-2] - spine[nSections-1];
         v1 = spine[nSections-2] - spine[0]; // new !!!!!!!!
      }
      else  
      { 
         tangent = spine[1]-spine[0];
         if (nSections >2) {  // was >1
            v1 = (spine[0] - spine[2]);
			//v1 = (spine[1-1] - spine[1]); // new use Z of spine[1]
			//v2 = (spine[1+1] - spine[1]);
			v1 = (spine[0] - spine[1]); // new 
			v2 = (spine[2] - spine[1]);
		 }
		 else  v1=tangent; // new  ???
       
      }
	  }
   }
   else if (section == (nSections -1))  // last section 
   {
      v1 = (spine[section-1] - spine[section]);
      if (spineClosed) {  // last == first 
         tangent = spine[1]-spine[nSections-2];
         v2 = spine[1] - spine[nSections-1];
      }
      else  { 
         tangent = spine[section]-spine[section-1];
         if (nSections >2) {  //  // xxxx >2 was >1
            //v2 = (spine[nSections-1] - spine[nSections-2]);  // fix v2 = (spine[nSections] - spine[nSections-2]);
			v1 = (spine[section-1-1] - spine[section-1]);
			v2 = (spine[section-1+1] - spine[section-1]);
		 } else {
			v2 = (spine[section-1+1] - spine[section-1]);
		 }	
      }
   }
   else  // intermediate section 
   {
      tangent =  (spine[section+1] - spine[section-1]);
      v1 = (spine[section-1] - spine[section]);
      v2 = (spine[section+1] - spine[section]);
  
   }

   float len = tangent.Length();

   if (len >0) {
		if (firstNonZeroY<0) firstNonZeroY=section;
   }	
   tangents[section]=tangent;

   
   v1.Normalize();
   v2.Normalize();
   Point newZ = CrossProd(v2,v1);
   float newZlen = newZ.Length();

   if (newZlen > EPS) {
		Z = newZ * 1.0f/newZlen;
		if (firstNonZeroZ<0) firstNonZeroZ=section;
   }
   zs[section]=Z;

}
// degenerate case ????? 
if (zs[0].Length() < EPS) {
	if (firstNonZeroZ>0) {
		zs[0]=zs[firstNonZeroZ];
	}
	else { // rotate Z to v2, rotate y0 plane 
	   Point v = (spine[nSections-1] - spine[0]);
	   if (firstNonZeroY>=0) v = tangents[firstNonZeroY];
	   Point y(0,1,0);
	   v.Normalize();
	   Point ax = CrossProd(y,v);
	   float axCos = DotProd(y,v);
	   float l=ax.Length();
	   Quaternation q;
	   q.SetAxisRotationCos(ax*1.0/l,axCos);
	   if (l>EPS) {
		   Matrix m;
		   Point z(0,0,1);
		   q.GetMatrix(m);
		   zs[0]= m * z;
	  
	   }
	   else  // v collinear with y
		  //if (axCos> -EPS)
			  zs[0].Set(0,0,1);
		  //else zs[0].Set(0,0,-1);
	   // tangentOld=v;
	}
}



if (firstNonZeroY>=0) {
	tangentOld = tangents[firstNonZeroY];
	tangentOld.Normalize();
}	

//Z = zs[0];
Z.Set(0,0,1); // cosmo implements it this way 

for(section=0; section<nSections; section++) {
   Point v1,v2;

   tangent=tangents[section];
   
   float len = tangent.Length();

   if (len < EPS) {		// length is 0, = coincident spine points 
	   tangent = tangentOld;
   }	
   else  {
	  tangent *= 1.0/ len; // normalize
   }
   Point newZ=zs[section];

   if (newZ.Length() > EPS) { 

	   if (/*(section>0) && */ (DotProd(Z,newZ) < -EPS))
		   Z=-newZ;
   	   else Z = newZ;
   }	
   X = NormCrossProd(tangent,Z);

   if (X.Length() < EPS) { 

      TRACE("Section %d Y (%f %f %f) z= (%f %f %f) X= (%f %f %f) \n",section,tangent.x,tangent.y,tangent.z,Z.x,Z.y,Z.z,X.x,X.y,X.z);
   }

   
   Matrix m;

   // apply scale scale[i].x y
   if (nScale >0) {
      Point s; // scale factor

      scale.get1(min(nScale-1,section),s);
      
      // profile  is in XZ !!
      m = ScaleMatrix(s.x,1.0,s.y);

   }
   else m.Identity();

   // translate to spine point
   //m *= TranslationMatrix(spine[section]);


   // apply orientation  
   if (nOrientation >0 ) {
       Matrix orient;
       orientation.get1(min(nOrientation-1,section),orient);
       m *= orient;
   } 
 
  
   // apply reference frame orientation
   //m *= GenRotation(X,tangent,Z);
    Matrix M; 
	M.Identity();

    M.x[0][0] = X.x;
    M.x[1][0] = X.y;
    M.x[2][0] = X.z;
    M.x[0][1] = tangent.x;
    M.x[1][1] = tangent.y;
    M.x[2][1] = tangent.z;
    M.x[0][2] = Z.x;
    M.x[1][2] = Z.y;
    M.x[2][2] = Z.z;


	M.contents |= Matrix::HAS_ROTATION;
	M.Invert();

	m*= M;

    // translate to spine point
    m *= TranslationMatrix(spine[section]);

  
   // append transformed crossection to points 
   int j= section * crossSection.Length();
   for(int i=0; i<crossSection.Length(); i ++) {
      pts[j+i]= m*crossSection[i];
   }
     
   tangentOld = tangent;


}  // for section

   GvBool isFlat = (GvCreaseAngleIsFlat(creaseAngle.value)) ;

	// special treatment for flat because of D3D no normalPerFace support 
#if 1
// mesh code disabeld 
if (1 || closeBegin || closeEnd || isFlat || !convex.value) {
   
   int h,w;
   int i;

   GFaceList f;
   
   //f.SetGrid(nSections,crossSection.Length()); // wrong orientation 

   int nrows = nSections; 
   int ncols =crossSection.Length();

   Array<GVertexI> verts(0);

   if (!isFlat && ( sectionClosed || spineClosed)) {
	   // create smooth vertex & nomrmal sharing

   verts.SetLength(ncols*nrows);
   i=0;
	
    for(h=0;h<(nrows); h++) {
	 int rowStart = i;
	 for(w=0;w<(ncols);w++) {
		GVertexI &v = verts[i];
		v.v=v.n=v.p=v.c= i;
		i++;

	 }

	 if (sectionClosed) {
		GVertexI &v = verts[i-1];
		v.v= verts[rowStart].v;
		if (!isFlat) v.n= verts[rowStart].n;
	 }	
	}	 

	if (spineClosed) {
	   i-=ncols;	
	   for(w=0;w<(ncols);w++) {
			GVertexI &v = verts[i];
			v.v=verts[w].v;
			if (!isFlat) v.n= verts[w].n;
			i++;
	 }
	}	
   }

   
   // compute vertex parameters
   PointArray vp(ncols*nrows);
   
   float umin=0.0,umax=1.0;
   float vmin=0.0,vmax=1.0;

   float u,du = (umax-umin) / (float) (ncols-1);
   float v,dv = (vmax-vmin) / (float) (nrows-1);


   i=0;
   int row,col;
   for (row = nrows-1,v=vmin; row >=0; row --,v+=dv)
   for (col = ncols-1,u=umin; col >=0; col --,u+=du) {
		 vp[i].Set(u,v);
		 i++;
   }   

   {
	int cnt;
	int cols = ncols;
	int rows=nrows;
	int closeCols = 0;
	int closeRows = 0;

    cnt= (cols-1+closeCols) * (rows-1+closeRows) * 5;

	 f.Resize(cnt);
     int *fp = f.DataRW();
	 i = 0;

	 for(h=0;h<(rows-1); h++,i+=cols) {
	 for(w=0;w<(cols-1);w++) {
		*fp++ = 4;
		*fp++=i+w;		 /* x,y   */
		*fp++=i+w+1;	 /* x+1,y	*/
		*fp++=i+w+1+cols; /* x+1,y+1 */
		*fp++=i+w+cols; /* x,y+1 */
	 }
	}
   }
   
   //
   
   // create caps pts & faces & paramas
   
   // to do: triangulate caps !
	// compute parameters for caps 
    
    PointArray crossParams;
	this->crossSection.get(crossParams);
	BBox bbox;
	crossParams.ComputeBBox(bbox);
    ComputeBoxParams(bbox,crossParams,crossParams);


   if (closeBegin) {
      int ofs = pts.Length();
	  pts.SetMaxLength(pts.Length() + crossSection.Length() +2  );
      pts.Append(crossSection.Length(),&pts[0]); // this could resize pts 
      
      vp.Append(crossSection.Length(),&crossParams[0]);

      f.Append(crossSection.Length());
      for(int i=crossSection.Length()-1; i>=0; i--) {
         f.Append(ofs+i);
      }
   }
   
   if (closeEnd) {
      int ofs = pts.Length();
	  pts.SetMaxLength(pts.Length() + crossSection.Length() +2 );
      pts.Append(crossSection.Length(),&pts[(nSections-1) * crossSection.Length()]);
      vp.Append(crossSection.Length(),&crossParams[0]);
      
      f.Append(crossSection.Length());
      for(int i=0; i<crossSection.Length(); i++) {
         f.Append(ofs+i);
      }
   }

/*  for Fog smooth shading need to be activated
  need to create real GShellI with vertex normal = FaceNormals (like for IndexedFaceSet)
*/


   GShell *s;
   if (isFlat || (verts.Length()>0)) {
	   GShellI *si;
	   // create shell indexed 
	   s= si=  new GShellI(pts.Length(),pts,f.Length(),f,isFlat);

	   if (si && (verts.Length() >0)) {
		   for (i=0;i<verts.Length();i++) {
				si->verts[i].v=verts[i].v; // vertex sharing 
				if (!isFlat) si->verts[i].n=verts[i].n; // normal sharing 
		   }
	   }	

   }	
   else s=  new GShell(pts.Length(),pts,f.Length(),f);
   
   s->SetVP(vp);

   if (isFlat) 
	   s->SetFlat();
   if (!ccw.value) {
	/// s->flags |= SHELL_FLIP_NORMALS;
   }	

   if (!convex.value) { // TBD triangulate only caps
 		TRACE("Triangulating Extrusion\n");
 		s->Triangulate();
   }

   SetShell(s);


} else
#endif

{
   GMesh *mesh = new GMesh(crossSection.Length(),nSections,pts);

	//  mesh->SetVertexParametersNative();

   int nrows = nSections; 
   int ncols = crossSection.Length();
   
   // compute vertex parameters
   PointArray vp(ncols*nrows);
   
   float umin=0.0,umax=1.0;
   float vmin=0.0,vmax=1.0;

   float u,du = (umax-umin) / (float) (ncols-1);
   float v,dv = (vmax-vmin) / (float) (nrows-1);


   int i=0;
   int row,col;
   for (row = nrows-1,v=vmin; row >=0; row --,v+=dv)
   for (col = ncols-1,u=umin; col >=0; col --,u+=du) {
		 vp[i].Set(u,v);
		 i++;
   }   

   mesh->SetVP(vp);

   if (isFlat) mesh->SetFlat();

   SetShell(mesh);

} // mesh case 

return(1);

}


int GvExtrusion::Do(GBBoxTraversal &state)
{
	if (theShell) return GvNodeShell::Do(state);
	// compute bbox from coordinates
	BBox box;

	if (spine.getBBox(box))
	    state.Union(box);

	return 0;

}



int GvExtrusion::Do(GglRenderTraversal &state)
{  	
	if (!theShell) BuildShell(state);
	
	if (!theShell) return (/*GvNode*/ GNode::Do(state));

	if (state.shape == NULL) { // VRML 1.0 
		return(GvNodeShell::Do(state));
	}

	if (!theShell) return (/*GvNode*/ GNode::Do(state));

	if (!CULLCHECK(theShell,state)) return TRAVERSE_CULLED;


	if (!state.appearanceApplied) {
		if (state.appearance)
			state.appearance->Apply(state);
	}

	// don't draw fully transparent geometry
	if (state.materialStatus & MATERIAL_FULLY_TRANSPARENT) {
		return 0;
    }

	// collect transparent shapes  ?
	if ( ((GGvglRenderTraversal *) & state)->IsDelayed())
	{
		if (!theShell) { // need shell for Bounding Box
		    state.glState->SetCulling(Solid(),!ccw.value);
			BuildShell(state);
		}
		if (theShell) {
			GTransparentElement * e= new GTransparentElement(state.appearance,this,theShell->GetBBox(),state.GetMatrixRef());
			((GGvglRenderTraversal *) & state)->AddTransparentElement(e);
		}
		return TRAVERSE_DELAYED;
	}

	// apply shape hints 
    state.glState->SetCulling(Solid(),!ccw.value);


	// apply traversal 
	int ret = theShell->Traverse(state);

	return(ret);
}

int GvExtrusion::Do(GGeometryTraversal &state)
{ 	int ret;
 	
    state.SetCulling(Solid(),!ccw.value);

	if (!theShell) 	BuildShell(state); 

	if (!theShell) return (0);

	ret = theShell->Traverse(state);

	return ret;
}


int GvExtrusion::Do(GRaySelectTraversal &state)
{  	
	if (!theShell) BuildShell(state);

	if (!theShell) return (0);

	// apply shape hints 
    state.SetCulling(Solid(),!ccw.value);

	int ret;

	// apply traversal 
	GPUSH_PATH(state,this);
		ret = theShell->Traverse(state);
	GPOP_PATH(state);
	return ret;
}



#if 0
class GvExtentedExtrusion : public GvNodeShell

{

  GV_NODE_HEADER(GvExtrusion);

  public:
  // Fields:
    GvSFBool     beginCap;          //  TRUE
    GvSFBool     ccw;               //  TRUE
    GvSFBool     convex;            //  TRUE
    GvSFFloat    creaseAngle;       //      0

    GvMFVec2f    crossSection;     //  [ 1 1, 1 -1, -1 -1, -1 1, 1 1 ]
    GvSFBool     endCap;           //  TRUE

    GvMFRotation orientation;      //  []
    GvMFVec2f    scale;            //  [1 1]
 
    GvMFVec3f    spine;            //  [ 0 0 0, 0 1 0 ]
    GvSFBool     solid;            //  TRUE

  	GvSFFloat    spineStart;        // 0.0
  	GvSFFloat    spineEnd;          // 1.0

    // Eventout
    GvSFVec3f    spineStartPosition;
    GvSFVec3f    spineEndPosition;

    // Eventout
    GvSFRotation    spineStartRotation;
    GvSFRotation    spineEndRotation;

    // EvaluateSpine(GvSFFloat *t);

    // build shell for current state 
    int BuildShell(GTraversal &state);  


};

#endif



//
// GvElevationGrid
//

#define EGRID_AS_TRIANGLES

void DuplicatePoints3(int n, const float *v,PointArray &p) 
{
	p.SetLength(n*2);
	Point *pp=p.DataRW();

	while (n-- > 0) {
		pp->Set(v[0],v[1],v[2]);
		v+=3;
		pp[1]=pp[0];
		pp+=2;
	}
}	

GV_NODE_SOURCE_IMP(GvElevationGrid,GvNodeShell);

GvElevationGrid::GvElevationGrid() :
    xDimension(0),xSpacing(1),zDimension(0),zSpacing(1),
    height(0),colorPerVertex(TRUE),
    normalPerVertex(TRUE),ccw(TRUE),solid(TRUE),creaseAngle(0)
{
    GV_NODE_CONSTRUCTOR(GvElevationGrid);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(xDimension);
    GV_NODE_ADD_FIELD(xSpacing);
    GV_NODE_ADD_FIELD(zDimension);
    GV_NODE_ADD_FIELD(zSpacing);
    
    GV_NODE_ADD_FIELD(height);

    GV_NODE_ADD_FIELD(color);
    GV_NODE_ADD_FIELD(colorPerVertex);
    GV_NODE_ADD_FIELD(normal);
    GV_NODE_ADD_FIELD(normalPerVertex);
    GV_NODE_ADD_FIELD(texCoord);
    GV_NODE_ADD_FIELD(ccw);
    GV_NODE_ADD_FIELD(solid);
    GV_NODE_ADD_FIELD(creaseAngle);



}

GvElevationGrid::~GvElevationGrid()
{
}

int			
GvElevationGrid::ComputeVertexColors(GTraversal &state,PointArray &colors)
{

    //if (colorPerVertex && color.value && RTISA(color.value,GvColor)) {

    //}
	return(0);
}

int			
GvElevationGrid::BuildShell(GTraversal &state)
{
	GvNode *n;
  
	SetShell(NULL);
    

	int rows = zDimension;
	int cols = xDimension;

	if ((rows<=0) || (cols<=0)) return(0);


	if (rows*cols > height.getNum()) { return(0); } // bad !!!


	PointArray p(rows*cols);
	//PointArray vp(rows*cols); // vertex parameters

	// extension

	if (xSpacing.value == 0.0) xSpacing.set( 1.0/ (float) (cols));
	if (zSpacing.value == 0.0) zSpacing.set( 1.0/ (float) (rows));
	
	int ofs = 0;
	int i,j;

	for (i=0; i <rows; i++)
		for (j=0; j <cols; j++)
		{
			Get(i,j,p[ofs]);
			ofs++;
		}
	

#ifdef _D3D
		//int rowsPerBuf  = state.state.maxVertexCount / ncols;
		// BuildShell if too large 
		//ASSERT(rowsPerBuf>= 2);
#endif

#if 0

	GMesh *theMesh;
	theMesh = new GMesh(cols,rows,p);
	theMesh->SetVertexParametersNative();
#else
	// treat triangulated 
	GShell *theMesh;

	theMesh = new GShell();
	theMesh->SetV(p);
	//theMesh->SetVP(vp);

#endif

	// use triangles not quads for ray intersection
	GFaceList f;
	f.SetGrid(rows,cols,0);
	theMesh->SetFaceList(f);


	n= this->normal.value.Eval(state);
	Gv2Normal *normal = n ? (RTCAST(this->normal.value.node,Gv2Normal)) : NULL;
	if (normal && normal->vector.getNum() == 0) normal = NULL;
	
	gbool isFlat = (!normalPerVertex.value || 
			(GvCreaseAngleIsFlat(creaseAngle.value) && normal == NULL));

	if (isFlat) theMesh->SetFlat();


	SetShell(theMesh);


// ********* texCoord  attribute

	n= this->texCoord.value.Eval(state);
	GvTextureCoordinate  *texCoord = n ? (RTCAST(this->texCoord.value.node, GvTextureCoordinate )) : NULL;
	if (texCoord && texCoord->point.getNum() == 0) texCoord = NULL;

	gbool computeVp=gtrue;

	if (texCoord) { 
			if (texCoord->point.getNum() < (theShell->v.Length())) { 
				TRACE("to few vertex paramters %d %d \n",texCoord->point.num , (theShell->v.Length()));
			}
			else {
				PointArray &vp=theShell->vp;
				texCoord->point.get(vp);
				theShell->SetVP(vp);
				computeVp=gfalse;
			}
	} 

	if (computeVp) {
		// PointArray vp;
		PointArray &vp=theShell->vp;// avoid a memory copy
		vp.SetLength(rows*cols);
		
		const float umin=0.0,umax=1.0;
		const float vmin=0.0,vmax=1.0;


		float u,du = (umax-umin) / (float) (cols-1);
		float v,dv = (vmax-vmin) / (float) (rows-1);


		int i=0;
		int row,col;
		//for (row = rows-1,v=vmax; row >=0; row --,v-=dv)
		for (row = rows-1,v=vmin; row >=0; row --,v+=dv)
			for (col = cols-1,u=umin; col >=0; col --,u+=du) {
				vp[i].Set(u,v);
				i++;
			}
		theShell->SetVP(vp);
	}

// ********* normal attribute

	if (normal) { 
		if (normalPerVertex) {
			if (normal->vector.getNum() < (theShell->v.Length())) { 
				TRACE("to few vertex normals %d %d \n",normal->vector.num , (theShell->v.Length()));}
			else theShell->SetVNByRef(normal->vector.getNum(),(Point*) normal->vector.values,1);
		} else {
#ifdef EGRID_AS_TRIANGLES
			if ((normal->vector.getNum()*2) < (theShell->GetFaceCount())) { 
				TRACE("to few face normals %d %d \n",normal->vector.num , (theShell->v.Length()));
			} else {
				PointArray fn;
				DuplicatePoints3(normal->vector.getNum(), normal->vector.values,fn); 
				theShell->SetFN(fn);
			}

#else
			if (normal->vector.getNum() < (theShell->GetFaceCount())) { 
				TRACE("to few face normals %d %d \n",normal->vector.num , (theShell->v.Length()));}
			else theShell->SetFN(normal->vector.getNum(),(Point*) normal->vector.values);
#endif
		}
	}

// ********* color attribute

	PointArray vc;
	n= this->color.value.Eval(state);
	GvColor *color = n ? (RTCAST(this->color.value.node,GvColor)) : NULL;
	if (color && color->color.getNum() == 0) color = NULL;

	if (color) { 
		if (colorPerVertex) {
			if (color->color.getNum() < (theShell->v.Length())) { 
				TRACE("to few vertex colors %d %d \n",color->color.num , (theShell->v.Length()));}
			else theShell->SetVCByRef(color->color.getNum(),(Point*) color->color.values);
		} else {

#ifdef EGRID_AS_TRIANGLES
			if ((color->color.getNum()*2) < (theShell->GetFaceCount())) { 
				TRACE("to few face colors %d %d \n",color->color.num , (theShell->v.Length()));
			} else {
				PointArray fc;
				DuplicatePoints3(color->color.getNum(), color->color.values,fc); 
				theShell->SetFC(fc);
			}

#else
			if (color->color.getNum() < (theShell->GetFaceCount())) { 
				TRACE("to few face colors %d %d \n",color->color.num , (theShell->v.Length()));}
			else theShell->SetFC(color->color.getNum(),(Point*) color->color.values);
#endif

		}
	}
	else 
		if (ComputeVertexColors(state,vc)>0) { // virtual function e.g. for ImageElevation Grid
			theShell->SetVC(vc);
		} 
else 
{    

#ifdef _G_VRML1_XXX

// 1.0 style material binding


	G_GETSTATE(state,materialB,GvMaterialBinding,	GvMaterialBindingElement)
	GvMaterial *material=NULL; 
	GvMaterialElement *materialTop = (GvMaterialElement*) state.Get(GvMaterialElement::attrId); 
	if (materialTop) material = (GvMaterial *) materialTop->data;;
	int materialBinding = GvMaterialBinding::OVERALL;
	if (materialB) materialBinding = materialB->value.value;
 
			if (materialTop) {
  		        GvMFColor * rgb = materialTop->GetDiffuse();
			
			if (rgb)
			switch (materialBinding) { // Material binding value

			case GvMaterialBinding::DEFAULT:
			case GvMaterialBinding::NONE:
			case GvMaterialBinding::OVERALL:
					break;

			case GvMaterialBinding::PER_PART:
			case GvMaterialBinding::PER_PART_INDEXED:
					{
						PointArray fc((rows-1) * (cols-1));
						Point *fcp = fc;
						for (int row= 0; row < (rows-1); row ++) {
						    Point partColor;
							rgb->get1(row,partColor);
							for (int col= 0; col < (cols-1); col ++) 
							   {
							   *fcp++= partColor;
							   }

						}
  		        		theShell->SetFC(fc);
					}
					break;


			case GvMaterialBinding::PER_FACE:
			case GvMaterialBinding::PER_FACE_INDEXED:
					if (theShell->GetFaceCount() <= rgb->num)
  		        		theShell->SetFC(rgb->num,(Point*) rgb->values);

					break;

			case GvMaterialBinding::PER_VERTEX:
			case GvMaterialBinding::PER_VERTEX_INDEXED:
				if (rgb->num>1) 
  		        	theShell->SetVC(rgb->num,(Point*) rgb->values);
				break;
			};
			} // Material
#endif // _G_VRML1

	} // compute vertex colors 
return(0);
}




// try to update internal shell
// by changing internal shell properties
//
int GvElevationGrid::OnColorChanged()
{
	if (theShell == NULL) return(1); // not build yet

	GvNode * tmp = this->color.Eval();
	GvColor *color = tmp ? (RTCAST(tmp,GvColor)) : NULL;
	if (color && color->color.getNum() == 0) color = NULL;

	if (!color) {
		// clear values
		theShell->SetVC(0,NULL);
		theShell->SetFC(0,NULL);
	    setRedraw();
		return(1);
	}


	if (color) { 

		if (colorPerVertex) {
			if (color->color.getNum() < (theShell->v.Length())) { 
				TRACE("to few vertex colors %d %d \n",color->color.num , (theShell->v.Length()));}
			else theShell->SetVCByRef(color->color.getNum(),(Point*) color->color.values);
		} else {
#ifdef EGRID_AS_TRIANGLES
			if ((color->color.getNum()*2) < (theShell->GetFaceCount())) { 
				TRACE("to few face colors %d %d \n",color->color.num , (theShell->v.Length()));
			} else {
				PointArray fc;
				DuplicatePoints3(color->color.getNum(), color->color.values,fc); 
				theShell->SetFC(fc);
			}

#else

			if (color->color.getNum() < (theShell->GetFaceCount())) { 
				TRACE("to few face colors %d %d \n",color->color.num , (theShell->v.Length()));}
			else theShell->SetFC(color->color.getNum(),(Point*) color->color.values);
#endif

		}
	}

    setRedraw();
    return(1);
}

int GvElevationGrid::OnHeightChanged()
{
	if (theShell == NULL) return(1); // not build yet

	int rows = zDimension;
	int cols = xDimension;

	//GMesh *theMesh = RTCAST(theShell,GMesh);

	if (!theShell ) return(0);

	if ((rows<=0) || (cols<=0) || (rows*cols > height.getNum())) { 
		SetShell(NULL);
		setRedraw();
		return(1); 
	} // bad !!!


	//PointArray p(rows*cols);
	PointArray &p=theShell->v; // by ref
	p.SetLength(rows*cols);

	if (xSpacing.value == 0.0) xSpacing.set( 1.0/ (float) (cols));
	if (zSpacing.value == 0.0) zSpacing.set( 1.0/ (float) (rows));

	int ofs = 0;
	int i,j;

	for (i=0; i <rows; i++)
    for (j=0; j <cols; j++)
    {
        Get(i,j,p[ofs]);
        ofs++;
    }

	theShell->SetV(p);

	setRedraw();
	return(1);
}

int GvElevationGrid::OnNormalChanged()
{
	if (theShell == NULL) return(1); // not build yet

	GvNode * tmp = this->normal.Eval();
	Gv2Normal *normal = tmp ? (RTCAST(tmp,Gv2Normal)) : NULL;

	if (normal && normal->vector.getNum() == 0) normal = NULL;

	if (!normal) {
		return(0);
	}

	if (normal) { 
		if (normalPerVertex) {
			if (normal->vector.getNum() < (theShell->v.Length())) { 
				TRACE("to few vertex normals %d %d \n",normal->vector.num , (theShell->v.Length()));}
			else theShell->SetVNByRef(normal->vector.getNum(),(Point*) normal->vector.values,1);
		} else {

#ifdef EGRID_AS_TRIANGLES
			if ((normal->vector.getNum()*2) < (theShell->GetFaceCount())) { 
				TRACE("to few face normals %d %d \n",normal->vector.num , (theShell->v.Length()));
			} else {
				PointArray fn;
				DuplicatePoints3(normal->vector.getNum(), normal->vector.values,fn); 
				theShell->SetFN(fn);
			}

#else

			if (normal->vector.getNum() < (theShell->GetFaceCount())) { 
				TRACE("to few face normals %d %d \n",normal->vector.num , (theShell->v.Length()));}
			else theShell->SetFN(normal->vector.getNum(),(Point*) normal->vector.values);
#endif

		}
	}
   setRedraw();
   return(1);
}

int GvElevationGrid::OnTexCoordChanged()
{
	if (theShell == NULL) return(1); // not build yet

	GvNode * tmp = this->texCoord.Eval();
	GvTextureCoordinate *texCoord = tmp ? (RTCAST(tmp,GvTextureCoordinate)) : NULL;

	if (texCoord && texCoord->point.getNum() == 0) texCoord = NULL;

	if (texCoord) { 
			if (texCoord->point.getNum() < (theShell->v.Length())) { 
				TRACE("to few vertex paramters %d %d \n",texCoord->point.num , (theShell->v.Length()));}
			else {
				PointArray &vp=theShell->vp;
				texCoord->point.get(vp);
				theShell->SetVP(vp);
			}
	}


   setRedraw();
   return(1);

}

// a child node changed
int GvElevationGrid::OnChildNodeChanged(GvNode *child)
{ 
  if (child == color.value.node) return OnColorChanged();
  if (child == normal.value.node) return OnNormalChanged();
  if (child == texCoord.value.node) return OnTexCoordChanged();
  return(0);
}  

// some child node has been changed
int GvElevationGrid::OnChildChanged(GvNode *child)
{
   return(OnChildNodeChanged(child));
}

int GvElevationGrid::OnFieldChanged(GvField *field)
{
	if (field == &height) 
			return OnHeightChanged();
	if (field == &color) 
			return OnColorChanged();
	if (field == &normal) 
			return OnNormalChanged();
	if (field == &texCoord) 
			return OnTexCoordChanged();
	if (field == &xSpacing) 
			return OnHeightChanged();
	if (field == &zSpacing) 
			return OnHeightChanged();

	// not an exposed field / could be ignored
	return GvNodeShell::OnFieldChanged(field);
}


// add child node , for easier editing
int GvElevationGrid::addChild(GvNode *child)
{ 
  if (RTISA(child,GvColor)) { color.set(child);  OnColorChanged(); return(1); }
  if (RTISA(child,Gv2Normal)) { normal.set(child);  OnNormalChanged(); return(1); }
  if (RTISA(child,GvTextureCoordinate)) { texCoord.set(child);  OnTexCoordChanged(); return(1); }
  return(0);
}  



int GvElevationGrid::Do(GBBoxTraversal &state)
{
  if (!theShell) BuildShell(state);
  if (theShell) return( theShell->Traverse(state));

  // compute some aproximate / coarse bounding box, until imageloading is done
  // not only in glRenderTraversal
  //
  BBox myBox;
 
  myBox.Union(Point (0,0,0));
  int i= xDimension;
  if (i<=0) i = 100;
  int j= zDimension;
  if (j<=0) j = 100;
  myBox.Union(Point(xSpacing * i, 1.0, zSpacing * j));
  state.Union(myBox);
  return(1);

}				


int GvElevationGrid::Do(GglRenderTraversal &state)
{  	
	if (!theShell) 
		BuildShell(state);

	if (!theShell) return (/*GvNode*/ GNode::Do(state));

	if (!CULLCHECK(theShell,state)) return TRAVERSE_CULLED;

	if (state.shape == NULL) { // VRML 1.0 
		return(GvNodeShell::Do(state));
	}

	if (!state.appearanceApplied) {
		state.appearance->Apply(state);
	}

	// don't draw fully transparent geometry
	if (state.materialStatus & MATERIAL_FULLY_TRANSPARENT) {
		return 0;
    }

	// collect transparent shapes  ?
	if ( ((GGvglRenderTraversal *) & state)->IsDelayed())
	{
		if (!theShell) { // need shell for Bounding Box
		    state.glState->SetCulling(Solid(),!ccw.value);
			BuildShell(state);
		}
		if (theShell) {
			GTransparentElement * e= new GTransparentElement(state.appearance,this,theShell->GetBBox(),state.GetMatrixRef());
			((GGvglRenderTraversal *) & state)->AddTransparentElement(e);
		}
		return TRAVERSE_DELAYED;
	}



	// apply shape hints 
    state.glState->SetCulling(Solid(),!ccw.value); // !!

	// apply traversal 
	int ret = theShell->Traverse(state);

	return(ret);
}

int GvElevationGrid::Do(GGeometryTraversal &state)
{ 	int ret;
 	
	// apply shape hints 
    state.SetCulling(Solid(),!ccw.value);

	if (!theShell) 	BuildShell(state); 

	if (!theShell) return (0);

	ret = theShell->Traverse(state);

	return ret;
}

int GvElevationGrid::Do(GRaySelectTraversal &state)
{  	

	if (!theShell) BuildShell(state);

	if (!theShell) return (0);

	// apply shape hints 
    state.SetCulling(Solid(),!ccw.value); //!!

	int ret;
	// apply traversal 
	GPUSH_PATH(state,this);
		ret = theShell->Traverse(state);
	GPOP_PATH(state);
	return ret;

}



//
// GvImageElevationGrid
//

#include <gvtexture2.h>

GV_NODE_SOURCE_IMP(GvImageElevationGrid,GvElevationGrid);

GvImageElevationGrid::GvImageElevationGrid()
{
    GV_NODE_CONSTRUCTOR(GvImageElevationGrid);
    isBuiltIn = FALSE;
    
    GV_NODE_ADD_FIELD(xDimension);
    GV_NODE_ADD_FIELD(xSpacing);
    GV_NODE_ADD_FIELD(zDimension);
    GV_NODE_ADD_FIELD(zSpacing);

    GV_NODE_ADD_FIELD(color);
    GV_NODE_ADD_FIELD(colorPerVertex);

    GV_NODE_ADD_FIELD(normal);
    GV_NODE_ADD_FIELD(normalPerVertex);
    GV_NODE_ADD_FIELD(texCoord);

    GV_NODE_ADD_FIELD(ccw);
    GV_NODE_ADD_FIELD(solid);
    GV_NODE_ADD_FIELD(creaseAngle);

    
    GV_NODE_ADD_FIELD(height);
    GV_NODE_ADD_FIELD(heightScale);

    GV_NODE_ADD_FIELD(heightImage);
    GV_NODE_ADD_FIELD(colorImage);
    GV_NODE_ADD_FIELD(colorPerHeight);


    xDimension.value = 0;
    xSpacing.value = 0.0;
    zDimension.value = 0;
    zSpacing.value = 0.0;

	heightScale.value= 1.0;
}


GvImageElevationGrid::~GvImageElevationGrid()
{

}



int			
GvImageElevationGrid::ComputeVertexColors(GTraversal &state,PointArray &colors)
{
    int ret=0;

    if ( colorImage.value && RTISA(colorImage.value,GvImage)) {
  
        GvImage *s = (GvImage *) (GvNode *) colorImage;
        GvSFImage *im = s->getImage(state);
        
        // image not ready yet 
        if (!im) return(0);
        if (!im->bytes) return(0);
        if (im->size[0]<=0 || im->size[1]<=0)  return(0);

       
        
        int cols = im->size[0];
        int rows = im->size[1];
        short step = im->numComponents;

                
        const unsigned char *ip = im->bytes;
        float scale = 1.0/255.0;
        Point color;


        if (cols == xDimension  && rows == zDimension) {
            float *zp = height.values;

            colors.SetLength(cols*rows);
            Point *cp = colors.DataRW();


            // get vertex colors from image 

            for(int r = 0; r<rows; r++) {
                for(int c=0; c<cols; c++) {
                    float z = scale *  *ip;
                    switch(step) {
                    case 1:
                    case 2:
                        color.Set(*ip * scale,*ip * scale,*ip * scale); break;
                    case 3:
                    case 4:
                        color.Set(ip[0] * scale,ip[1] * scale,ip[2] * scale); break;
                    default : break;
                    }
                    
                    ip+=step;
                    *cp = color;
                    cp++;
            }
            }
        } else {
            rows = zDimension;
            cols = xDimension;

            int imCols = im->size[0];
            int imRows = im->size[1];


            colors.SetLength(cols*rows); 
            Point *cp = colors.DataRW();


            // find nearest pixel 
            for(int r = 0; r<rows; r++) {
                for(int c=0; c<cols; c++) {
                    float z = scale *  *ip;
                    ip =  &im->bytes[(imCols * (r * (imRows -1)  / (rows-1)) +
                                     (c * (imCols -1)  / (cols-1))) * step];   
                    switch(step) {
                    case 1:
                    case 2:
                        color.Set(*ip * scale,*ip * scale,*ip * scale); break;
                    case 3:
                    case 4:
                        color.Set(ip[0] * scale,ip[1] * scale,ip[2] * scale); break;
                    default : break;
                    }
                    
                    *cp = color;
                    cp++;
            }
            }


        }
        
        return(colors.Length()>0);   

    }
    else return(GvElevationGrid::ComputeVertexColors(state,colors));
}



int			
GvImageElevationGrid::BuildShell(GTraversal &state)
{

    if ( heightImage.value && RTISA(heightImage.value,GvImage)) {
      
        GvImage *s = (GvImage *) (GvNode *) heightImage;
        
        GvSFImage *im = s->getImage(state);
        
        // image not ready yet 
        if (!im) return(0);
        if (!im->bytes) return(0);
        if (im->size[0]<=0 || im->size[1]<=0)  return(0);
        
        int cols = im->size[0];
        int rows = im->size[1];
        int step = im->numComponents;
        
        const unsigned char *ip = im->bytes;
        float scale = heightScale * 1.0/255.0;

        height.makeRoom(cols*rows);
        float *zp = height.values;

        // compute height values from image 

        for(int r = 0; r<rows; r++) {
            for(int c=0; c<cols; c++) {
                float z = scale *  *ip;
                *zp++ = z;
                ip+=step;
            }
        }
        height.OnChanged();

        xDimension.set(cols);
        /* if (xSpacing.value == 0.0) */ 
		xSpacing.set(1.0/(float) cols);
        
		zDimension.set(rows);
        /* if (zSpacing.value == 0.0)*/ 
		
		zSpacing.set(1.0/(float) rows);
        
       
        return(GvElevationGrid::BuildShell(state));

    }
    else return(-1);

}

int GvImageElevationGrid::Do(GglRenderTraversal &state)
{
   // load height image
   if ( heightImage.value && RTISA(heightImage.value,GvImage)) {
        GvImage *s = (GvImage *) (GvNode *) heightImage;
        GvSFImage *im = s->getImage(state);
   }
   // load color image
   if ( colorImage.value && RTISA(colorImage.value,GvImage)) {
        GvImage *s = (GvImage *) (GvNode *) colorImage;
        GvSFImage *im = s->getImage(state);
   }
   return (GvElevationGrid::Do(state));
 
}


//
// GvPolygon
//


GV_NODE_SOURCE_IMP(GvPolygon,GvNodeShell);


GvPolygon::GvPolygon() :
        ccw(TRUE),
        coordIndex(0),
        solid(TRUE),
		texCoord(0)
       
{
    GV_NODE_CONSTRUCTOR(GvPolygon);
    isBuiltIn = FALSE;

    GV_NODE_ADD_FIELD(color);
    GV_NODE_ADD_FIELD(coord);
    GV_NODE_ADD_FIELD(normal);
    GV_NODE_ADD_FIELD(texCoord);
    GV_NODE_ADD_FIELD(ccw);
    GV_NODE_ADD_FIELD(coordIndex);
    GV_NODE_ADD_FIELD(solid);

	bboxOk=planeOk=0;
	bbox.SetEmpty();
        
}

GvPolygon::~GvPolygon()
{
}

// update cached information
void GvPolygon::Update(gbool theBox,gbool thePlane)
{
	bboxOk=planeOk=0;

	GvNode * tmp = this->coord.Eval();
	
	GvCoordinate *coord = tmp ? (RTCAST(tmp,GvCoordinate)) : NULL;
	if (!coord) 
		return;

	int numc= coord->point.Length();
	const Point *c =(const Point *) coord->point.values;

	int nverts=0;

	if (theBox) {


		// pointers for stepping the coordIndex list 
		const long *vp,*vpStart = coordIndex.values;
		const long *vpEnd = coordIndex.values+coordIndex.num;

		vp = vpStart;
		bbox.SetEmpty();


		while (vp<vpEnd) {
		     
			  long v= *vp++; // get index 
			  if (v<0) break; // -1 index 
			  if (v>=numc) break;
			  const Point *cp = &c[(unsigned long)v];
			  cp->MinMax(bbox.mi,bbox.ma);
			  
			  nverts++;
	   }

	   bboxOk=1;
	}

	if (thePlane) {
		int i;

		const long *vindex = coordIndex.values;

		Point refpt;
		Point normal;
		
		const Point *u, *v;
		
		float len;

		if (nverts <=2) {
			plane.n.x = 0;
			plane.n.y = 0;
			plane.n.z = 1;
			plane.d = 0;
			planeOk=1;
			return;
		}
		

		/* compute the polygon normal and a reference Point on
		   the plane. Note that the actual reference Point is
		   refpt / nverts
		*/
		normal.Zero();
		refpt.Zero();

		v = &c[vindex[0]];

		for(i = nverts-1; i >=0 ; i--) {

			u = &c[vindex[i]];

			normal.x += (u->y - v->y) * (u->z + v->z);
			normal.y += (u->z - v->z) * (u->x + v->x);
			normal.z += (u->x - v->x) * (u->y + v->y);

			refpt+= *u;
			v=u;
		}

		/* normalize the polygon normal to obtain the first
		   three coefficients of the plane equation
		*/
		len = normal.Len();
		if (len == 0.0) len = 1.0f;
		else len = 1.0f / len;
		
		plane.n.x = normal.x * len;
		plane.n.y = normal.y * len;
		plane.n.z = normal.z * len;
		
		refpt *= (1.0f / nverts);
		
		/* compute the last coefficient of the plane equation */
		plane.d = -DotProd (refpt, plane.n); 
		
		planeOk=1;
	}

}


int GvPolygon::Do(GBBoxTraversal &state)
{
	if (theShell) return GvNodeShell::Do(state);
	// compute bbox from coordinates
	return (coord.Traverse(state));
}


int GvPolygon::Do(GglRenderTraversal &state)
{  	

	gbool drawIm = gfalse;

#ifdef _OGL
	drawIm = gtrue;
#endif


	if (!bboxOk) {
		Update(gtrue,!planeOk);
		if (!bboxOk)
			return TRAVERSE_CULLED;
	}
	
	if (!state.CullCheck(bbox)) 
		return TRAVERSE_CULLED;

	if (!drawIm) {
		if (!theShell) 
			BuildShell(state);

		if (!theShell) 
			return (/*GvNode*/ GNode::Do(state));

	//	if (!CULLCHECK(theShell,state)) 
	//		return TRAVERSE_CULLED;

	}


	
	if (!state.appearanceApplied) {
		if (state.appearance)
			state.appearance->Apply(state);
	}

	// don't draw fully transparent geometry
	if (state.materialStatus & MATERIAL_FULLY_TRANSPARENT) {
		return TRAVERSE_CULLED;
    }


	// collect transparent shapes  ?
	if ( ((GGvglRenderTraversal *) & state)->IsDelayed())
	{
		if (!theShell) { // need shell for Bounding Box
		    state.glState->SetCulling(Solid(),!ccw.value);
			BuildShell(state);
		}
		if (theShell) {
			GTransparentElement * e= new GTransparentElement(state.appearance,this,theShell->GetBBox(),state.GetMatrixRef());
			((GGvglRenderTraversal *) & state)->AddTransparentElement(e);
		}
		return TRAVERSE_DELAYED;
	}

	// apply shape hints 
    state.glState->SetCulling(Solid(),!ccw.value);

	int ret=0;

	if (!drawIm) {
		// apply traversal 
		int ret = theShell->Traverse(state);

	#ifdef _OGL
		if (theShell->vc.Length()>0 || theShell->fc.Length()>0) {
			state.material = NULL; // force reload of materials  // glcolor
			state.appearanceApplied = 0;
		}
	#endif

	}
#ifdef _OGL
	else 
	{

	const float *v=NULL;
	const float *vp[8];
	const float *vc=NULL;
	const float *vn=NULL;

	int vertexFlags=0;


	// get property child nodes as proper typed nodes
    GvNode *tmp = this->coord.Eval(state);
	GvCoordinate *coord = tmp ? (RTCAST(tmp,GvCoordinate)) : NULL;

	if (coord) { v = coord->point.values; }
	
	if (!v) return 0;

    
	tmp = this->color.Eval(state);
	GvColor *color = tmp ? (RTCAST(tmp,GvColor)) : NULL;

	if (color) { vc = color->color.values; }
    
	tmp = this->normal.Eval(state);
	Gv2Normal *normal = tmp ? (RTCAST(tmp,Gv2Normal)) : NULL;

	if (normal) { vn = normal->vector.values; }

	int nvp=this->texCoord.getNum();

	if (nvp>=8) nvp = 8;

	for (int i=0; i<nvp;i++) {
		tmp = this->texCoord.get1(i);
    	if (tmp) tmp = tmp->Eval(state);
	
		GvTextureCoordinate *texCoord = tmp ? (RTCAST(tmp,GvTextureCoordinate)) : NULL;

		if (texCoord) { vp[i] = texCoord->point.values; }
		else vp[i]=NULL;
	
	}

	state.glState->DrawPolygon3D(vertexFlags,v,vc,vn,nvp,vp,coordIndex.values,coordIndex.num,plane);

	}
#endif


	return(ret);
}

int GvPolygon::Do(GRaySelectTraversal &state)
{  	
	// apply shape hints 
    state.SetCulling(Solid(),!ccw.value);

	// we need the shell, even if not rendered eg for Collision.proxy field
	if (!theShell) 	BuildShell(state); 

	if (!theShell) return (0);


	int ret;

	// apply traversal 
	GPUSH_PATH(state,this);
		ret = theShell->Traverse(state);
	GPOP_PATH(state);
	return ret;
}

int GvPolygon::Do(GGeometryTraversal &state)
{ 	int ret;
 	
	// apply shape hints 
    state.SetCulling(Solid(),!ccw.value);

	if (!theShell) 	BuildShell(state); 

	if (!theShell) return (0);

	ret = theShell->Traverse(state);

	return ret;
}



// try to update internal shell
// by changing internal shell properties
//
int GvPolygon::OnColorChanged()
{
	if (theShell == NULL) return(1); // not build yet

	if (theShapes.getNum()>0 ) {		// bad, expanded shapes
		SetShell(NULL);
		setRedraw();
	    return(0);
	}


	GvNode * tmp = this->color.Eval();
	GvColor *color = tmp ? (RTCAST(tmp,GvColor)) : NULL;

	if (!color) {
		// clear values
		theShell->SetVC(0,NULL);
		theShell->SetFC(0,NULL);
	    setRedraw();
		return(1);
	}

	{
	   //if (color->color.num < (maxI.c+1)) { 
	   //	   TRACE("to few vertex colors %d %d \n",color->color.num , maxI.c+1);
	   //}
	   //else
       
		// to do: what if wrong length ??
		
	   theShell->SetVC(color->color.num,(Point*) color->color.values);
	   theShell->SetFC(0,NULL);
	   setRedraw();
	
	   return(1);

	} 
}

int GvPolygon::OnCoordChanged()
{
	bboxOk=planeOk=0;

	if (theShell == NULL) return(1); // not build yet

	if (theShapes.getNum()>0 ) {		// bad, expanded shapes
		SetShell(NULL);
		setRedraw();
	    return(0);
	}

	GvNode * tmp = this->coord.Eval();
	GvCoordinate *coord = tmp ? (RTCAST(tmp,GvCoordinate)) : NULL;

	if (!coord) {
	    SetShell(NULL); // no good coord, no shell
	    setRedraw();

		if (this->coord.value.node == NULL) {
			TRACE("Try to set coord to 0, bad idea\n");
			return(1);
		}
		else return(0);
   }
   
	// to do: what if wrong length ??
   theShell->SetVByRef(coord->point.num,(Point*) coord->point.values);
   theShell->UpdateActions(); 

   tmp = this->normal.Eval();
   if (tmp) { // explicit normals, user must also animate normals !
		//theShell->UnSetAction(SHELL_COMPUTE_FNORMALS);
		theShell->UnSetAction(SHELL_COMPUTE_VNORMALS);
   }

   setRedraw();

   return(1);
}

int GvPolygon::OnNormalChanged()
{
	if (theShell == NULL) return(1); // not build yet
	if (theShapes.getNum()>0 ) {		// bad, expanded shapes
		SetShell(NULL);
		setRedraw();
	    return(0);
	}

	GvNode * tmp = this->normal.Eval();
	Gv2Normal *normal = tmp ? (RTCAST(tmp,Gv2Normal)) : NULL;
	if (!normal) {
		return(0);
	}
	if (1 /*normalPerVertex */) {
       // to do: what if wrong length ??
	   // speed penalty, need to re-normalize	
	   theShell->SetVNByRef(normal->vector.num,(Point*) normal->vector.values,1);

	   // will ActionFlag be cleared ?

	   setRedraw();

	   return(1);
	} else { 
      TRACE("OnNormal per face changed not implemented \n");
	  // could be ignored SetShell(NULL);
	  return(0);
    }
}

int GvPolygon::OnTexCoordChanged()
{
	if (theShell == NULL) 
		return(1); // not build yet

#if 0
	GvNode * tmp = this->texCoord.get1(0);

	if (tmp)
		tmp = tmp->Eval();

	GvTextureCoordinate *coord = tmp ? (RTCAST(tmp,GvTextureCoordinate)) : NULL;

	if (!coord) {
		if (this->texCoord.value.node == NULL) {
			// clear values
			theShell->SetVP(0,NULL);
			// to do : compute default params ?
			setRedraw();

			return(1);
		}
		else return(0);
   }

	//PointArray vp;
   PointArray &vp=theShell->vp; // avoid a memcopy
   // convert from float2 to float3
   coord->point.get(vp);
   // to do: what if wrong length ??
   theShell->SetVP(vp);
   setRedraw();

   return(1);
#endif
	return 0;

}


// a child node changed
int GvPolygon::OnChildNodeChanged(GvNode *child)
{ 
  if (child == color.value.node) return OnColorChanged();
  if (child == coord.value.node) return OnCoordChanged();
  if (child == normal.value.node) return OnNormalChanged();
  // if (child == texCoord.value.node) return OnTexCoordChanged();
  return(0);
}  

// add child node , for easier editing
int GvPolygon::addChild(GvNode *child)
{ 
  if (RTISA(child,GvColor)) { color.set(child);  OnColorChanged(); return(1); }
  if (RTISA(child,GvCoordinate)) { coord.set(child);  OnCoordChanged(); return(1); }
  if (RTISA(child,Gv2Normal)) { normal.set(child);  OnNormalChanged(); return(1); }
  if (RTISA(child,GvTextureCoordinate)) { texCoord.set(child);  OnTexCoordChanged(); return(1); }
  return(0);
}  

// some child node has been changed
int GvPolygon::OnChildChanged(GvNode *child)
{
   return(OnChildNodeChanged(child));
}

int GvPolygon::OnFieldChanged(GvField *field)
{
	if (field == &coord) 
			return OnCoordChanged();
	if (field == &color) 
			return OnColorChanged();
	if (field == &normal) 
			return OnNormalChanged();
	if (field == &texCoord) 
			return OnTexCoordChanged();

	return GvNodeShell::OnFieldChanged(field);
}



/*

  Create internal GShell representation for node


*/

int			
GvPolygon::BuildShell(GTraversal &state)
{
	GReporter* reporter = getReporter(state);
	
	// delete any old internal shell version
	SetShell(NULL);

	// get property child nodes as proper typed nodes
    GvNode *tmp = this->coord.Eval(state);
	GvCoordinate *coord = tmp ? (RTCAST(tmp,GvCoordinate)) : NULL;
    
	tmp = this->color.Eval(state);
	GvColor *color = tmp ? (RTCAST(tmp,GvColor)) : NULL;
    
	tmp = this->normal.Eval(state);
	Gv2Normal *normal = tmp ? (RTCAST(tmp,Gv2Normal)) : NULL;

	tmp = this->texCoord.get1(0);
    
	if (tmp) tmp = tmp->Eval(state);
	GvTextureCoordinate *texCoord = tmp ? (RTCAST(tmp,GvTextureCoordinate)) : NULL;

	// max number of values, -1 no values 
	int coordMax = -1;
	int normalMax = -1;
	int colorMax = -1;
	int texCoordMax = -1;

	// ignore empty nodes 
	if (coord && (coordMax = coord->point.getNum()) == 0) coord = NULL;
	if (normal && (normalMax = normal->vector.getNum()) == 0) normal = NULL;
	if (color && (colorMax=color->color.getNum()) == 0) color = NULL;
	if (texCoord && (texCoordMax=texCoord->point.getNum()) == 0) texCoord = NULL;


	// it´s in a very bad condition, no coordinates yet
	if (coord==NULL || (coordIndex.getNum() < 3)) {
		return(0);
	}

	
	gbool isFlat = (normal == NULL);


	{
		   int doIndexed = 0; // flag if build index shell 
		   GVertexI maxI;	// maximum number of indices for each parameter
		   
		   maxI.Zero();
		   
		   
		   // pointers for stepping the coordIndex list 
		   const long *vp,*vpStart = coordIndex.values;
		   const long *vpEnd = coordIndex.values+coordIndex.num;
		   vp = vpStart;
		   
		   GFaceList f;
		   f.SetMaxLength(coordIndex.num*2);
		   
		   int fcnt=0;	// how many vertices for this face
		   int fstart=0; // where to put fcnt in f
		   int fi=0;	// the current face we are working on 
		   f.Append(0);	// room for fcnt


		   {

			 // facelist constructed directly from coordIndex
			 // output is GShell
		     while (vp<vpEnd) {
		     
			  long v= *vp++; // get index 

			  if (v <0) {	// face end 
				if (vp >= vpEnd) break;	// its the last -1 
				
				f[fstart]=fcnt;

					if (fcnt < 3) {
					   TRACE("Bad face length  %d at offset %d \n",fcnt,vp-1-vpStart);	
					   if (reporter->verbose) 
						   reporter->Warning("IndexedFaceSet: face has only %d vertices at list positon %d number of coordinates %d",fcnt,vp-1-vpStart,coordMax);
					   f.SetLength(fstart+1); // new xx
					}
					else {
		   				fstart = f.Length();
		   				f.Append(0);	// room for fcnt

					}

				fcnt=0;
			  } 
			  else { 
					// checked also at the end
					if (v >= coordMax) {
					   TRACE("Bad coord index %d at offset %d \n",v,vp-1-vpStart);							
					   if (reporter->verbose) 
						   reporter->Warning("IndexedFaceSet: coordIndex value %d  at list positon %d exceedes max number of coordinates %d",v,vp-1-vpStart,coordMax);
					} else {
						if (fcnt>0 && (v == f[fstart+1])) {
							TRACE("Duplicated face vertex %d length  %d at offset %d \n",v,fcnt,vp-1-vpStart);	
						} else {
							f.Append(v);	// append new face index
			  				fcnt++; 
						}
					}
			  }
		   	}
		   	f[fstart]=fcnt; // just for savety , if not -1 terminated
			if (fcnt < 3) {
				   TRACE("Bad face length  %d at offset %d \n",fcnt,vp-1-vpStart);	
				   if (reporter->verbose) 
						   reporter->Warning("IndexedFaceSet: face has only %d vertices at list positon %d number of coordinates %d",fcnt,vp-1-vpStart,coordMax);
					f.SetLength(fstart); // new xx 
			}
#if 1
			  GShell *theShell= new GShell();

		      SetShell(theShell);

			  ASSERT(theShell);  	

			  if (theShell ==0) return -1;
			  // set data by reference !	
		   	  theShell->SetVByRef(coord->point.num,(Point*) coord->point.values);	// save cast if values = float
			  theShell->SetFaceList(f);

#else


		   	SetShell(new GShell(
		   			coord->point.num,
		   			(Point*) coord->point.values,	// save cast if values = float
		   			f.Length(),f));
#endif

			maxI.v=maxI.c=maxI.p=maxI.n=coord->point.num-1;
			
			} 
			
			ASSERT(theShell); 
			if (!theShell) return -1;

			theShell->UpdateActions();

			// check and set additional attributres 

			if (normal) {
				if (normal->vector.num < (maxI.n+1)) { 
				  TRACE("to few normals %d %d \n",normal->vector.num , maxI.n+1);
				  if (reporter->verbose) reporter->Warning("IndexedFaceSet: to few vertex normals %d  Required : %d",normal->vector.num , maxI.n+1);
				}
	  			else	// speed penalty : author normals may be not normalized ! 
					  theShell->SetVNByRef(normal->vector.num,(Point*) normal->vector.values,1);
					
			} // Normal

			if (color) {
				  if (color->color.num < (maxI.c+1)) { 
					  TRACE("to few vertex colors %d  Required : %d \n",color->color.num , maxI.c+1);
					  if (reporter->verbose) reporter->Warning("IndexedFaceSet: to few vertex colors %d  Required : %d",color->color.num , maxI.c+1);
				  }
	  		      else theShell->SetVCByRef(color->color.num,(Point*) color->color.values);
			} // color 
			
			if (texCoord)
		    {
				  // PointArray vp;
 				  PointArray &vp=theShell->vp; // avoid memcopy
				  // convert from float2 to float3
				  texCoord->point.get(vp);
  				  if (vp.Length() < (maxI.p+1)) { 
  				  	TRACE("to few parameters #params %d max index %d \n",
  				  		vp.Length() , maxI.p+1);
//#ifndef _GLVIEW_IE

					if (reporter->verbose) reporter->Warning("IndexedFaceSet: to few texture coordinates %d  Required : %d",texCoord->point.num , maxI.p+1);
//#endif
					// bad VRML 1.0 => 2.0 conversion or alias error 
				  	PointArray vpNew;
				    while (vpNew.Length() <  (maxI.p+1)) {
						vpNew.Append(vp);
					}
					theShell->SetVP(vpNew);

  				  }
	  		      else theShell->SetVP(vp);
		    } else {		// compute a default mapping 
 			    //PointArray vp(maxI.p+1);
 			    PointArray &vp=theShell->vp; // avoid memcopy
				vp.SetLength(maxI.p+1);

				int indexed=0;

				if (RTISA(theShell, GShellI)) {
					// ?? hg if (!texCoordIndex.isDefault()) 
					indexed = 1;
				}
				if (indexed) {
				  GShellI *s = (GShellI *) theShell;	
				  s->UpdateActions();  // to do, GetBBox should update 
				  ComputeBoxParams(s->GetBBox(),s->verts,s->v, vp);
				  s->SetVP(vp);
				} else {
				  theShell->UpdateActions();  // to do, GetBBox should update 
				  ComputeBoxParams(theShell->GetBBox(),theShell->v, vp);
				  theShell->SetVP(vp);
				}
			}				

 	};
 
	if (isFlat) 
	  theShell->SetFlat();

	if (!ccw.value) {
	/// theShell->flags |= SHELL_FLIP_NORMALS;
	}	


 // heuristic, detect a one coordinate3 many IndexFaceSets with few faces only case, compress
 // alternative , implement a GShell by Reference 
 // TO DO : what about GShell ???? or refcnt > 0

	gbool computeSubMeshes = (theShell->f.Length() < theShell->v.Length())
		||  (theShell->v.Length() > GShell::maxSubMeshVertices);


 
	return(1);
}




