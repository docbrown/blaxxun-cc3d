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
#ifndef _GvTraverse
#define _GvTraverse

#include "arraynew.h"

#include "gshell.h"
#include "gvisibility.h"
#include "gcamera.h"

#include <gvpath.h>
#include <gvmfnode.h>


// scene enable flags 
class GvSceneFlags {
public:
	int light;
	int spotLight;
	int pointLight;
	int camera;
	int texture2;
	int WWWinline;
	GvSceneFlags() {
			light = 1;
			spotLight = 1;
			pointLight = 1;
			camera = 1;
			texture2 = 1;
			WWWinline = 1;
	}
};

/* register all qv property elements for  nodes to state */
void InitializeGvAttributes(GTraversal &state,int all=1);


class GvBackground;
class GvFog;
class GvNavigationInfo;
class GvWorldInfo;

class GvMaterial;
class GvTexture2;
class GvWWWInline;
class GvNode;
class GvNodeShell;
class GShell;
class GvWWWInline;
class GvWWWAnchor;
class GvWWWRegionAnchor;
class GvMorph;
class GvSwitch;
class GvInfo;

class GvTimeSensor;


// flags returned by material apply functions
enum MATERIAL_RETURN_FLAGS {
			MATERIAL_IS_UNLIT  = 1,		// turn of lights
			MATERIAL_HAS_ALPHA = 2,		// has transparency
			MATERIAL_SET_DIFFUSE_WHITE = 4,	// set diffuse color to white (for textures)
			MATERIAL_FULLY_TRANSPARENT = 8	// fully transparent, don't draw

};

// flags from the texture setting functions
enum TEXTURE_RETURN_FLAGS {
			TEXTURE_DEFINED = 1,
			TEXTURE_HAS_ALPHA = 2,		// texture has some alpha channel
			TEXTURE_HAS_COLORKEY = 4,
			TEXTURE_MODULATE = 8,
			TEXTURE_CHANGED = 16	//D3D handle of texture may changed
};



//
// an object collecting information about a VRML scene tree
//
class GvSceneInfo {
public : 
	int nodeCnt;

	// extended 
	int geometryCnt;
	int faceCnt;
	int textureCnt;
	
	GvBool full; // check full 
	
    GvNode *top;

	// VRML 2 bindable nodes
	GvBackground	* firstBackground;
	GvFog		*firstFog;
	GvNavigationInfo *firstNavigationInfo;
	GvWorldInfo	*firstWorldInfo;

    GvMaterial *firstMaterial;
	GvTexture *firstTexture;
	GvNode *firstAnimated;
	GvNode *firstGeometrySensor;
	GvNode *firstCamera;
	GvNode *firstLight;
	GvNode *firstPointLight;
	GvNode *firstSpotLight;
	GvNodeShell *firstNodeShell;
	GShell *firstShell;
	
	GvWWWInline *firstWWWInline;
	GvWWWAnchor *firstWWWAnchor;
	GvWWWRegionAnchor *firstWWWRegionAnchor;

	GvSwitch *firstCameraSwitch;
	GvSwitch *firstCameraKeyframe;
	
	GvMorph *firstMorph;


	// info nodes
	GvInfo *viewerInfo;
	GvInfo *viewerSpeed;
	GvInfo *sceneInfo;
	GvInfo *title;
	GvInfo *backgroundColor;
	GvInfo *backgroundImage;
	GvInfo *renderMode;
    GvInfo *animateViewpoints;	

	// 18.08.97 HG changed from GvNode* to  GvNodeHandle
	ArrayNew<GvNodeHandle> cameras;

	Array<Matrix>  cameraTransforms;

	ArrayNew<GvNodeHandle> lights; 
	ArrayNew<GvPath>  lightPath;	// pathes to lights 

	double nextScheduleTime;

	GvMFNode timeSensors;	//!< list of timeSensor nodes
	GvMFNode audioClips;	//!< list of AudioClip's
	GvMFNode sounds;		//!< list of Sounds's

	GvMFNode specialMedia;	//!< list of special media 

	GvMFNode movieTextures;	//!< list of MovieTextures 

	GvMFNode scripts;	//!< list of scripts, to call initialize function 

	GvMFNode menueSensors; //!< list of menue Sensors

	GvMFNode inlines; //!< list of Gv2Inline nodes 

    //! notify all time sensors with current time t
    int TriggerTimeSensors(double t);

    //! notify all MovieTextures with current time t
    int TriggerMovieTextures(double t);

    //! notify all Media  with current time t
    int TriggerMedia(double t);

    //! shutdown Media
    int TermMedia();

	//! unload some old unaccessed inlines 
	int UnloadSomeInlines(double currentT,int activeLimit, int numToUnload);


	
	GvSceneInfo();
	~GvSceneInfo();


	void Zero();

	// flush all node references
	void Flush(); 

	GvBool HasTextures() const { return(firstTexture != 0); }
	
	GvBool HasLights() const { return(firstLight != 0); }
	
    GvBool HasTimeSensors() const { return(timeSensors.Length() > 0); }

    GvBool HasMovieTextures() const { return(movieTextures.Length() > 0); }

    GvBool HasAudioClips() const { return(audioClips.Length() > 0); }

    GvBool HasMedia() const { return (
							    (audioClips.Length() > 0) 
							 || (sounds.Length() > 0)
							 || (specialMedia.Length() > 0)
							 );
	}

    GvBool HasMenueSensors() const { return(menueSensors.Length() > 0); }
	
    GvBool HasCamera() const  { return(firstCamera != 0); }
	
    GvBool IsAnimated() const  { return(firstAnimated != 0) || HasMovieTextures(); }
	
	GvBool HasBackgroundImage() const { return(backgroundImage != 0); }


	int Do(GPathTraversal &state,GNode *n); 
};

//
// GGvSceneInfoTraversal
// traverse nodes, and gather some information into sceneInfo
//
class GGvSceneInfoTraversal : public GPathTraversal {

public :
	RT(GGvSceneInfoTraversal);

	GGvSceneInfoTraversal() : GPathTraversal() { }

  	GvSceneInfo sceneInfo;


  	void InitializeAttributes() {
  		InitializeGvAttributes(*this);
		GPathTraversal :: InitializeAttributes();
   	}

	int Do(GNode *n);

};


class GGvBBoxTraversal : public GBBoxTraversal
{
public :
	RT(GGvBBoxTraversal);
  	
  	GGvBBoxTraversal() : GBBoxTraversal() {} 
  	void InitializeAttributes() {
  		InitializeGvAttributes(*this);
		GBBoxTraversal :: InitializeAttributes();
   	}

};

class RenderState;

//
// helper structure to collect transparent items
//

class GTransparentElement  {
public :
	float key; // sort key
	
	Point center; // in world space 

	Matrix m; // transformation matrix
	
	GvNode *appearance;	// appearances node
	GvNode *geometry;	// the geometry node
	GTransparentElement(GvNode *appearance,GvNode *geometry,const BBox& bbox,const Matrix& m);

	// draw the element
	int Do(GglRenderTraversal &state); 

};



// do cull check wihth shells bounding box
inline gbool CullCheck(GShell *theShell,GglRenderTraversal &state)
{
#if 1
	// to think : could turn off checking if parent matrices says all inside
	if (state.doViewCulling /* && state.doClipping */ )
	{
		GMatrixStack *mstack = state.GetCurrentMatrix();	
		if (mstack->m.contents & ( Matrix::HAS_NEGATIVE_SCALE) )  {
		    state.doClipping = gtrue;
			return gtrue; // not working with neg scale 
		}

		if (!mstack->viewpointLocalOk) { // need to transform it 
			mstack->SetViewpointLocal(state.physicalCamera->position);	
		}  

		if (!mstack->viewVolumeLocalOk) {
			mstack->SetViewVolumeLocal(state.viewVolume); 
		}
		
		if (BoxViewVolumeOutside(theShell->GetBBox(),mstack->viewpointLocal,mstack->viewVolumeLocal,state.doClipping)) { 
			return gfalse;
		}

	}
#endif
	return gtrue;
}


class GGvglRenderTraversal : public GglRenderTraversal
{
public:
	RT(GGvglRenderTraversal);

	GGvglRenderTraversal(RenderState * GlState=0) : GglRenderTraversal(GlState) { } 
	~GGvglRenderTraversal();

  	void InitializeAttributes();

	// don't draw fully transparent geometry
	gbool IsFullyTransparent() {
		return (materialStatus & MATERIAL_FULLY_TRANSPARENT)!=0;
		}

	// should this geometry node be drawed later ??
	gbool IsDelayed() {
		return ((transparencyMode > GTRANSPARENCY_NONE) 
			&& ((materialStatus & MATERIAL_HAS_ALPHA)
			|| (textureStatus & TEXTURE_HAS_ALPHA)));
	}
	

	// the list of transparent elements, memory owned by this class
	Array<GTransparentElement*> transparentElements;

	// any transparent elements ?
	gbool HasTransparentElements() { return transparentElements.Length()>0; }

	// add element to transparent list
	void AddTransparentElement(GTransparentElement *element);

	// delete all transparent elements
	void DeleteTransparentElements();

	// sort & render the transparent elements
	int DoTransparentElements();

	// fully process the transparent elements, and clean list at end
	virtual int RenderTransparentElements();


};

// traverse nodes, load inlines 
//
class GLoadInlinesTraversal : public GGeometryTraversal 
{

public :
	RT(GLoadInlinesTraversal);

	GLoadInlinesTraversal() : GGeometryTraversal() { 
		stopOnFirst = 0; resolveRelativeUrls = FALSE; 
	}


	// if TRUE, mode is relativeUrl resolution
	GvBool resolveRelativeUrls; 

/*
	// retreived via stack attribute 

	GvString currentHomeUrl;

	const GvString & getHomeUrl() { return currentHomeUrl; }
	void setHomeUrl(const GvString &s) { currentHomeUrl = s; }
*/

	int stopOnFirst;

  	void InitializeAttributes() {
  		InitializeGvAttributes(*this);
		GGeometryTraversal :: InitializeAttributes();
   	}

	int Do(GNode *n) {	return (n->Do(*this)); }

};


#endif
