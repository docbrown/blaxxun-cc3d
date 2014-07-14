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

@module GvTraverse.cpp - GLView VRML 1.0 traversal code	|

Copyright (c) 1995 - 1998	by Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:

generic traversal


Changes:
	30.03.98 HG fixed at light source handling in Direct 3D

$Revision: 1.28 $
$Log: gvtraverse.cpp,v $
Revision 1.28  1998/10/07 13:23:09  holger
empty message

Revision 1.25  1998/07/06 17:54:20  holger
empty message

Revision 1.15  1998/04/20 10:18:21  bernd
hg

Revision 1.14  1998/04/14 21:41:55  holger
empty message



Todo :
   some time remove the VRML 1.0 garbage

******************************************************************************/
#include "stdafx.h"

/*
#ifndef _GLVIEW_DLL
#include "glviewhdr.h"
#endif
*/


#include <gvelement.h>
#include <gvnodes.h>
#include <gvsfnode.h>
#include <gvmfnode.h>

#include <gvwebfx.h>

#include <gvunknownnode.h>
#include <gvscript.h>

#include <grayselect.h>

#include "gclass.h"
#include "gshell.h"
#include "gshelli.h"
#include "gfont.h"
#include "ggeom.h"

#include "gcamera.h"

#include "grender.h"

#include "glutils.h"

#include "gbuildshell.h"

#include "www.h"
#include "greadgv.h"
#include "gmodel.h"
#include "greporter.h"

#include <gvmodifier.h>
#include <gvproto.h>

#include <gvtraverse.h>

#include "gtransparency.h"
#include "gvisibility.h"
#include "gvnurbsinventor.h"

#include "goptimizetraversal.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//extern int BeginIncrementalUpdates(GView *view);



// Declared in Gclass.h

// Retreive current scene url or url home directory
const char* GTraversal::GetUrl(CTime &childrenRefreshTime)
{
 	//query state for nearest WWWInline ...
	GvWWWInlineElement *current;
	current = (GvWWWInlineElement*) Get(GvWWWInlineElement::attrId);
	if (current && current->data) {
		GvNode *n= current->data; 
		//if (RTISA(n,GvWWWInline)) 
		// 	return( ((GvWWWInline *) n)->GetUrl());
		//else 

		if (RTISA(n,Gv2Inline)) {
			Gv2Inline *nn = (Gv2Inline *) n;
			childrenRefreshTime = nn->GetChildrenRefreshTime();
			return nn->GetUrl();
		}
		else if (RTISA(n,GvScene)) 
			return( ((GvScene *)n)->GetUrl());

	}

	// get home from view
 	if (view) {
	   return(view->GetUrl());
 	}
 	return(NULL);

}

const char* GTraversal::GetHomeUrl()
{
 	//query state for nearest WWWInline ...

	GvWWWInlineElement *current;
	current = (GvWWWInlineElement*) Get(GvWWWInlineElement::attrId);
	if (current && current->data) {
#ifdef _G_VRML1
		if (RTISA(current->data,GvWWWInline)) 
			return( ((GvWWWInline *) current->data)->GetHomeUrl());
		else 
#endif _G_VRML1
		if (RTISA(current->data,Gv2Inline)) 
			return( ((Gv2Inline *) current->data)->GetHomeUrl());
		else if (RTISA(current->data,GvScene)) 
			return( ((GvScene *) current->data)->GetHomeUrl());
		// should not happen, ExternProto are not creating context for url's
		else if (RTISA(current->data,GvExternProto)) 
			return( ((GvExternProto *) current->data)->GetHomeUrl());

	}
	// should not happend
	// get home from view
 	if (view) {
	   return(view->GetHomeUrl());
 	}
 	return(NULL);
}



#define TRY_CLASS(CLASSNAME) if (!str(first,CLASSNAME)) if (RTISA(n,str(Gv,CLASSNAME))) str(first,CLASSNAME) = RTCAST(n,str(Gv,CLASSNAME));

// update scene info with the node 
int GvSceneInfo::Do(GPathTraversal &state,GNode *n) 
{
		if (!RTISA(n,GvNode)) 
			return(0);

		GvNode *nn= (GvNode *) n;
		nn->m_browser = state.getBrowser();
		
		ASSERT(nn->m_browser != NULL);

		nodeCnt ++;


		if (full) {
        if (!top) top = nn;
		
		// VRML 2 bindable nodes
		TRY_CLASS(Background);
		TRY_CLASS(Fog);
		TRY_CLASS(NavigationInfo);
		TRY_CLASS(WorldInfo);
		}
#ifdef _G_VRML1

		if (full) {
		if (!firstAnimated) {
			if ( RTISA(n,GvTextureRotor)  
			     || RTISA(n,GvRotor)
			     || RTISA(n,GvPendulum)
			     || RTISA(n,GvShuttle)
			     || RTISA(n,GvTextureShuttle)
			     || RTISA(n,GvMorph)
			     || RTISA(n,GvBlinker)
			     // || RTISA(n,GvSpin)
			     || RTISA(n,GvSpinGroup)
				) firstAnimated = RTCAST(n,GvNode);
		}
		
		TRY_CLASS(Material);
		TRY_CLASS(Texture);  // Any texture node 

		TRY_CLASS(WWWInline);
		TRY_CLASS(WWWAnchor);
		TRY_CLASS(WWWRegionAnchor);
		TRY_CLASS(NodeShell);
		TRY_CLASS(Morph);



		if (!firstShell) 
			if (RTISA(n,GShell)) firstShell = RTCAST(n,GShell);

		if (!firstCameraSwitch) 
			if (RTISA(n,GvSwitch))  {
				firstCameraSwitch = RTCAST(n,GvSwitch);
				if (firstCameraSwitch->objName && "Cameras" == *firstCameraSwitch->objName) ;
				else firstCameraSwitch = 0;
			}
		}
#endif _G_VRML1

		if ( (state.notInTransformHierarchy == 0) && RTISA(n,GvLight)) {
			// lights ...
				BOOL add=TRUE;
			
			    GvLight *nn= (GvLight *) n;
				nn->isRegistered = TRUE;

				if (state.topPath) {
					GvNode *parent = (GvNode*) state.topPath->node; 
					if (parent) { // only add lights in proper scope // better find abound tranform hierachy
						if ( RTISA(parent,Gv2Group) || RTISA(parent,Gv2Switch)
							|| RTISA(parent,GvProtoInstance) 
					#ifdef _G_VRML1
							|| RTISA(parent,GvGroup)
					#endif
							) 
							add = TRUE;
						else add = FALSE; 

					}
				}

				if (add) {
					// vrml 2 pointlights are global ! need them before rendering
					lights.Append(nn);
			
					GvPath path; 
					path.SetByList(state.topPath);
					lightPath.Append(path);

					TRY_CLASS(PointLight);
					TRY_CLASS(SpotLight);
					if (!firstLight) {
						if ( RTISA(n,GvPointLight)  
						 || RTISA(n,GvDirectionalLight)
						 || RTISA(n,GvSpotLight)
						) firstLight = nn;
					}

				}
		}
		else 
		if ( (state.notInTransformHierarchy == 0) && 
				(	RTISA(n,GvPerspectiveCamera)  
			     || RTISA(n,GvOrthographicCamera)
                 || RTISA(n,GvViewpoint))
				) 
        {
				BOOL add=TRUE;
				
				// don't add viewpoints without descriptions
				if (RTISA(n,GvViewpoint)) {
					GvViewpoint *vp = RTCAST(n,GvViewpoint);
					((GvViewpoint *) vp)->isRegistered = TRUE;
					// we need it for first viewpoint
					if (firstCamera != NULL) {
						if (vp->getName().getLength() == 0) 
							if (vp->description.get().getLength() == 0) 
								add = FALSE;
					}
				}


				if (state.topPath) {
					GvNode *parent = (GvNode*) state.topPath->node; 
					if (parent) { // only add viewpoints in proper scope // better find abound tranform hierachy
						if ( RTISA(parent,Gv2Group) || RTISA(parent,Gv2Switch)
							|| RTISA(parent,GvProtoInstance) 
							#ifdef _G_VRML1
							|| RTISA(parent,GvGroup)
							#endif _G_VRML1
							) 
						{	// add = TRUE; 
						}
						else 
							add = FALSE; 

					}
				}
				if (add) {
					cameras.Append(RTCAST(n,GvNode));
					cameraTransforms.Append(state.GetMatrixRef());
				}
				if (!firstCamera && add) {
					firstCamera = RTCAST(n,GvNode);
				}
		}
		else 
        // collect time sensors
        if (RTISA(n,GvTimeSensor))
        {
                timeSensors.add((GvNode*) n);
        }
		else 
        if (RTISA(n,GvAudioClip))
        {
                audioClips.add((GvNode*) n);
        }
        else if (RTISA(n,GvSound))
        {
                sounds.add((GvNode*) n);
        }
		else 
        if (RTISA(n,GvMovieTexture))
        {
                movieTextures.add((GvNode*) n);
        }
		else 
        if (RTISA(n,GvMenuSensor))
        {
                menueSensors.add((GvNode*) n);
        }
		else 
        if (RTISA(n,GvScript))
        {
                scripts.add((GvNode*) n);
        }
		else 
        if (RTISA(n,Gv2Inline))
        {
                inlines.add((GvNode*) n);
				((Gv2Inline *) n)->isRegistered = TRUE;
        }
#ifdef _G_VRML1
		else 
		if (full && RTISA(n,GvInfo)) {
			GvInfo *info = (GvInfo *) n;
			if (info->objName) {
				if (*info->objName == "Viewer") { if (!viewerInfo) viewerInfo = info; } // "Walk", "walk" "examine"
				else if (*info->objName == "ViewerSpeed") { if (!viewerSpeed) viewerSpeed = info; } // "10"
				else if (*info->objName == "BackgroundColor") {
						if (!backgroundColor) backgroundColor = info;

				} //backgroundColor = ".4 .66 1"
				else if (*info->objName == "BackgroundImage") { // a name of an image file
						if (!backgroundImage) backgroundImage = info;

				} 
				else if (*info->objName == "SceneInfo") { if (!sceneInfo) sceneInfo = info; }
				else if (*info->objName == "Title") { if (!title) title = info; }
				else if (*info->objName == "RenderMode") { if (!renderMode) renderMode = info; }
				else if (*info->objName == "AnimateViewpoints") { if (!animateViewpoints) animateViewpoints = info; }
				//CollisionDetection		 TRUE /FALSE

			}
		}  

#endif

		return(1);
};

GvSceneInfo::GvSceneInfo() :timeSensors(0),audioClips(0),sounds(0),specialMedia(0),
			movieTextures(0),
			menueSensors(0),scripts(0),inlines(0),full(TRUE)
{
		Zero();
}


GvSceneInfo::~GvSceneInfo()
{
}


void GvSceneInfo::Zero() 
{
		nodeCnt=0;
        top = NULL;
		firstBackground = NULL;
		firstFog = NULL;
		firstNavigationInfo = NULL;
		firstWorldInfo = NULL;

		firstMaterial = NULL;
		firstTexture = NULL;
		firstAnimated = NULL;
		firstGeometrySensor = NULL;
		firstCamera = NULL;
		firstLight = NULL;
		firstPointLight = NULL;
		firstSpotLight = NULL;
		firstNodeShell = NULL;
		firstShell = NULL;
		firstWWWInline = NULL;
		firstWWWAnchor = NULL;
		firstWWWRegionAnchor = NULL;
		firstCameraSwitch = NULL;
		firstCameraKeyframe = NULL;
		firstMorph = NULL;

		// info nodes
		viewerInfo = NULL;
		viewerSpeed = NULL;
		sceneInfo = NULL;
		title=NULL;
		backgroundColor=NULL;
		backgroundImage=NULL;
        renderMode = NULL;
        animateViewpoints = NULL;

}

void GvSceneInfo::Flush() 
{

		Zero();
		cameras.Flush();
		cameraTransforms.SetLength(0);
		lights.Flush();
		lightPath.Flush();

		timeSensors.flush();
		audioClips.flush();
		sounds.flush();
		specialMedia.flush();
		movieTextures.flush();
		menueSensors.flush();
		scripts.flush();
		inlines.flush();
}




int GvSceneInfo::TriggerTimeSensors(double t)
{ int ret=0;
  BOOL  hasUnused=FALSE;
  //  TRACE("Triggering time sensors %f\n",t);
  for(int i=0; i<timeSensors.Length(); i++) {
	  if (timeSensors[i]->refCnt <= 1) {
		    TRACE("TimeSensor with no ref %p \n",(GvNode*)timeSensors[i]);
			hasUnused = TRUE;
	  }	
      else ret =((GvTimeSensor *) (GvNode*) timeSensors[i])->OnTimeChange(t);
  }  
  if (hasUnused) {
	timeSensors.removeUnused();
  }	
  return(ret);
}

int GvSceneInfo::TriggerMovieTextures(double t)
{ int ret=0;
  BOOL hasUnused=FALSE;
  //  TRACE("Triggering movie textures %f\n",t);
  for(int i=0; i<movieTextures.Length(); i++) {
	  if (movieTextures[i]->refCnt <= 1) {
		    TRACE("MovieTexture with no ref %p \n",(GvNode*)movieTextures[i]);
			hasUnused = TRUE;
	  }	
      else ret |=((GvMovieTexture *) (GvNode*) movieTextures[i])->OnTimeChange(t);
  }  
  if (hasUnused) {
	movieTextures.removeUnused();
  }	
  return(ret);
}

int GvSceneInfo::TriggerMedia(double t)
{ int ret=0;
  BOOL hasUnused=FALSE;

  int i;

  //  TRACE("Triggering sounds %f\n",t);
  for(i=0; i<sounds.Length(); i++) {
	  if (sounds[i]->refCnt <= 1) {
		    TRACE("sound with no ref %p \n",(GvNode*)sounds[i]);
			hasUnused = TRUE;
	  }	
      else ret |=((GvSound *) (GvNode*) sounds[i])->OnTimeChange(t);
  }  
  if (hasUnused) {
	sounds.removeUnused();
  }	

  hasUnused=FALSE;
  //  TRACE("Triggering audioClips %f\n",t);
  for(i=0; i<audioClips.Length(); i++) {
	  if (audioClips[i]->refCnt <= 1) {
		    TRACE("audioClips with no ref %p \n",(GvNode*)audioClips[i]);
			hasUnused = TRUE;
	  }	
      //  AudioClip is triggered via Sound, else ret |=((GvAudioClip *) (GvNode*) audioClips[i])->OnTimeChange(t);
  }  
  if (hasUnused) {
	audioClips.removeUnused();
  }	
  return(ret);
}

//! shutdown Media
int GvSceneInfo::TermMedia()
{ int ret=0;
  BOOL hasUnused=FALSE;

  int i;

  //  TRACE("Triggering sounds %f\n",t);
  for(i=0; i<sounds.Length(); i++) {
	  if (sounds[i]->refCnt <= 1) {
		    TRACE("sound with no ref %p \n",(GvNode*)sounds[i]);
			hasUnused = TRUE;
	  }	
      ((GvSound *) (GvNode*) sounds[i])->Term();
  }  
  if (hasUnused) {
	sounds.removeUnused();
  }	

  hasUnused=FALSE;
  return ret;
}

/*
 * Compare inline render time 
 */
static int
G__cdecl CompareInlines(const void* element1, const void* element2) {
    Gv2Inline *e1, *e2;

    e1 = *((Gv2Inline**)element1);
    e2 = *((Gv2Inline**)element2);

	double l1 = e1->lastRenderTime;
	double l2 = e2->lastRenderTime;

	if (l1<l2) return -1;
	if (l1>l2) return 1;

	return 0;

}

// unload some old unaccessed inlines 
int GvSceneInfo::UnloadSomeInlines(double currentT,int activeLimit,int numToUnload)

{ int ret=0;
  BOOL  hasUnused=FALSE;
  int loadedCnt=0;

  //  TRACE("Triggering time sensors %f\n",t);
  Array<Gv2Inline*> active;

  for(int i=0; i<inlines.Length(); i++) {
	  Gv2Inline *n= (Gv2Inline *) (GvNode*) inlines[i];
	 	
	  if (n->refCnt <= 1) {
		    TRACE("Inline with no ref %p \n",n);
			hasUnused = TRUE;
	  }	
      else {
		  if (n->loader.Ok()) {
			  loadedCnt++;
			  if ((n->lastRenderTime < currentT)) { // found a canditate to purge
					active.Append(n);
			  }	
		  }	
	  }			
  }  
  if ( (active.Length() >=activeLimit)) {  // sort by age
		qsort((void *)&active[0], (size_t)active.Length(), sizeof(Gv2Inline*),CompareInlines);
		
		int l=active.Length();
		
		if (numToUnload<0) 
			l=min(l,-numToUnload / 1000);
		else if (numToUnload > 0) { // percentage 
			numToUnload = (l * (numToUnload) / 1000);
			l=min(l,numToUnload);
		}
		
		for(int j=0; j< l; j++) { // and unload them 
			active[j]->Unload(); 
		}

		if (!hasUnused) { // and child inlines now obsolete ?
			for (int j=0; j< l; j++) { 
				if (active[j]->refCnt <=1) { hasUnused = TRUE; break; }
			}
		}
		ret = l;
  }	

  if (hasUnused) {
	inlines.removeUnused();
  }	

  return(ret);
}



RTIMP(GGvSceneInfoTraversal,0,GPathTraversal);

int GGvSceneInfoTraversal::Do(GNode *n) 
{	
	if (sceneInfo.Do(*this,n)) {
//	   return GGeometryTraversal::Do(n);

	   return n->Do(*this); // do default traversal for Groups, Attributes ...

	}	
	return(0);
}

RTIMP(GGvBBoxTraversal,0,GBBoxTraversal);
RTIMP(GGvglRenderTraversal,0,GglRenderTraversal);

GGvglRenderTraversal::~GGvglRenderTraversal()
{
	DeleteTransparentElements();
}

//
// initialize gl rendering specific attributes 
//

void GGvglRenderTraversal::InitializeAttributes() 
{
#ifdef _G_VRML1
  
 if (view && view->useMaterials)	{
	 // push top level material on stack 
	 GvglMaterialElement *mat= 	GvglMaterialElement::NewElement();
	 mat->data = defaultMaterial10;
	 mat->baseColor = NULL;
	 Set(mat);
  }
  
  if (useTextures)	{
   	GvglTexture2Element *t= GvglTexture2Element::NewElement();
  	t->data = NULL;		  // or set global render state texture 
  	Set(t);
  }

	// glShapeHintsElement
	GvglShapeHintsElement *t= GvglShapeHintsElement::NewElement();
  	t->data = NULL;
  	Set(t);

	Set(GvglLightModelElement::NewElement());

#endif _G_VRML1


  if (view && view->useSceneLights) {
  		GvLightElement *le = GvglLightElement::NewElement();
  		le->data = NULL;
		le->index = firstLightIndex -1;	// next 0 is start light index 
  		Set(le);
  }
  

  InitializeGvAttributes(*this);
  GglRenderTraversal::InitializeAttributes();
}

/*
 * Compare Elements by distance 
 */
static int
G__cdecl CompareGTransparentElement(const void* element1, const void* element2) {
    GTransparentElement *e1, *e2;

    e1 = *((GTransparentElement**)element1);
    e2 = *((GTransparentElement**)element2);

	if (e1->key>e2->key) return -1;
	if (e1->key<e2->key) return 1;

	return 0;

}

	// delete all transparent elements
void GGvglRenderTraversal::DeleteTransparentElements()
{
	for (int i= 0; i<transparentElements.Length(); i++) {
		GTransparentElement *e = transparentElements[i];
		transparentElements[i]=NULL;
		delete e;

	}
	transparentElements.SetLength(0);

}

GTransparentElement::GTransparentElement(GvNode *appearance_,GvNode *geometry_,const BBox& bbox,const Matrix& m_):
appearance(appearance_),geometry(geometry_),m(m_)
{
	center = bbox.Center();
	center *= m;
}

// draw the element
int GTransparentElement::Do(GglRenderTraversal &state) 
{
	int ret;
	state.Push();

	state.AppendMatrix(m);

	ret = ((GNode*)appearance)->Do(state);
	ret = ((GNode*)geometry)->Do(state);

	state.Pop();
	return ret;
}

// add element to transparent list
void GGvglRenderTraversal::AddTransparentElement(GTransparentElement *element)
{
	if (!element) return;
	// compute sort key 
	Point dist = physicalCamera->position - element->center;
	element->key= dist.Len2();
	transparentElements.Append(element);

}


/*
void GGvglRenderTraversal::AddTransparentElement(GvNode *geometry,const BBox& bbox)
{
	GTransparentElement * e= new GTransparentElement(appearance,geometry,bbox,GetMatrixRef());
	((GGvglRenderTraversal *) & state)->AddTransparentElement(e);
}
*/

int GGvglRenderTraversal::DoTransparentElements()
{
	int ret;

	// sort from back to front
	qsort((void *)&transparentElements[0], (size_t)transparentElements.Length(), sizeof(GTransparentElement*),CompareGTransparentElement);

	transparencyMode = GTRANSPARENCY_NONE; // don't sort again into list 
	appearanceApplied = gfalse;

	// reforce to apply all appearances, otherwise an unset texture is cached
	shape = (GvShape*) -1;
	material = (Gv2Material*) -1;
	materialStatus = 0;
	texture = (GvTexture*) -1;
	textureTransform = (Gv2TextureTransform*) -1;
	textureStatus = 0;

	// and draw 
	for (int i=0; i<transparentElements.Length();i++) {
		ret=transparentElements[i]->Do(*this);
	}
	return ret;
}

// process the transprent elements
int GGvglRenderTraversal::RenderTransparentElements()
{
	int ret=-1;

		 if (HasTransparentElements()) { // draw transparent elements

			RenderState &renderState = *glState;


			// TRACE("Drawing transparent elements \n");
			gbool zwrite = renderState.zwrite;
			gbool blend = renderState.blend;

			renderState.SetBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
			renderState.SetBlend(TRUE);
			renderState.SetZWrite(FALSE);
#ifdef _OGL
			renderState.SetDepthFunc(GL_LEQUAL); // test hg ogl
#endif
				DoTransparentElements();

			renderState.SetBlend(blend);
#ifdef _OGL
			renderState.SetDepthFunc(GL_LESS); // test hg ogl
#endif

			//renderState.SetZBuffer(zbuffer);
			renderState.SetZWrite(zwrite);

		}
		
	DeleteTransparentElements(); // ready with transparent elements, clean list

	return ret;
}



#undef TRY_CLASS



RTIMP(GLoadInlinesTraversal,0,GGeometryTraversal);


int GNode::Do(GLoadInlinesTraversal &state) // declared in GClass.h
{
	return Do(*((GTraversal *) &state));		
}






//////////////////////////////////////////////////////////////////////////////
//
// Traversal code for all nodes. The default method (in GvNode) does
// nothing. Because traverse() is defined in the header for ALL node
// classes, each one has an implementation here.
//
//////////////////////////////////////////////////////////////////////////////

// For debugging
static int indent = 0;
static void
announce(const char *className)
{
    for (int i = 0; i < indent; i++)
	printf("\t");
    printf("Traversing a %s\n", className);
}
#define ANNOUNCE(className) announce(GV__QUOTE(className))



//////////////////////////////////////////////////////////////////////////////
//
// Node fields
//
//////////////////////////////////////////////////////////////////////////////

/* now inline 
int GvSFNode::Traverse(GTraversal &state) 
{
   	if (value != NULL) {
	    int ret;
   		ret = value->Traverse(state);
   		return(ret);
   } 
   return(0) ; 
}

int GvSFNode::Traverse(GPathTraversal &state) 
{
   	if (value != NULL) {
	    int ret;
		GSET_PATH_FIELD(state,*this);
   		ret = value->Traverse(state);
   		return(ret);
   } 
   return(0) ; 
}
*/

/* now inline 
int GvMFNode::Traverse(GTraversal &state) 
{
  int ret=0;
		int curIndex=0;
		while (curIndex < Length()) {
			GvNode *n = get1(curIndex); // for savety ignore NULL nodes 
			if (n) ret=n->Traverse(state);
			curIndex ++;
		}
	return ret;
}

int GvMFNode::Traverse(GPathTraversal &state) 
{
  int ret=0;
		GSET_PATH_FIELD(state,*this);
		int curIndex=0;
		while (curIndex < Length()) {
			GvNode *n = get1(curIndex); // for savety ignore NULL nodes 
			
			GSET_PATH_INDEX(state,curIndex);

			if (n) ret=n->Traverse(state);
			curIndex ++;
		}
	return ret;
}
*/


#ifdef _G_VRML1
//////////////////////////////////////////////////////////////////////////////
//
// Groups.
//
//////////////////////////////////////////////////////////////////////////////


int GvGroup::TraverseChildren(GTraversal &state)
{	int ret;
    for (int i = 0; i < getNumChildren(); i++)
	 	ret = getChild(i)->Traverse(state);
	return(ret);

}

int GvGroup::TraverseChildren(GPathTraversal &state)
{	int ret;
    for (int i = 0; i < getNumChildren(); i++) {
		GSET_PATH_INDEX(state,i);
	 	ret = getChild(i)->Traverse(state);
	}
	return(ret);

}

// get the effective matrix up to child node #i (not included)
GvBool GvGroup::getChildrenMatrix(Matrix &m, int upToChild)
{	GvBool ret = FALSE;
	int maxi = getNumChildren();
	if (upToChild<maxi) maxi = upToChild;
	
	Matrix m1;

    for (int i = 0; i < maxi; i++) {
	 	if (getChild(i)->getMatrix(m1)) {
			if (!ret) { m = m1; ret = TRUE; }
			else {
				m= m1 * m;
			}
		}
	}
	return(ret);
}


int GvGroup::Do(GTraversal &state) 
{
	return(TraverseChildren(state));
}   	


int GvGroup::Do(GPathTraversal &state) 
{
	int ret;
	GPUSH_PATH(state,this);
		ret =TraverseChildren(state);
	GPOP_PATH(state);
	return(ret);
}   	


int GvLevelOfDetail::Do(GTraversal &state)
{
	if (state.traverseAll) { return(GvGroup::Do(state)); }
    // ??? In a real implementation, this would choose a child based
    // ??? on the projected screen areas.
    if (getNumChildren() > 0)
	   return getChild(0)->Traverse(state);
	return(0);
}

int GvLOD:: Do(GTraversal &state)
{

	if (state.traverseAll) { return(GvGroup::Do(state)); }

    // ??? In a real implementation, this would choose a child 
    int nChilds = getNumChildren();
	if (nChilds == 0) return(0);
	
	return(getChild(nChilds-1)->Traverse(state));
}

/*
  25.01.96 dynamic load switch,
      if select WWWInline child is not loaded, display previous level
	  if (rangeEmpty) use highest + previous rule

*/
int GvLOD::Do(GglRenderTraversal &state)
{
    int nChilds = getNumChildren();
	if (nChilds == 0) return(0);
	int i=0;

	if (range.Length() == 0) {
		i=nChilds -1;	// try highest level
	} else {
		Point eye = state.camera->position;
		Point pos = center.value;
		//pos *= *state.GetMatrix();
		//eye *= Invert(*state.GetMatrix());
	
		eye *= state.GetMatrixInverse();
	
		Point d= pos-eye;
		float dist = d.Len();
		for(i=0; i<range.Length(); i++) {
			if (dist <= range.values[i]) {
				break;
			}
		}

	}
    
   if (i>= nChilds)  i = nChilds-1;
   


    GvNode * child;
	int ret;

	// if child is busy loading, try previous LOD level 
	do {

		child = getChild(i);
        lastRenderedChild = i; // the child that was rendered the last time 

		if (RTISA(child,GvWWWInline)) {
	    	//GvWWWInline *inlineChild = (GvWWWInline *) child;
			//state.renderInlineBox = 0
			ret = child->Traverse(state);

			if (ret<0) {   // Inline is busy
			  i++;      // try next one
			}
			else break;
		} else {
			ret = child->Traverse(state);
			break;
		}

	} while (i<nChilds);

	return (ret);
}

#endif // _G_VRML1



#ifdef _G_VRML1


int GvSeparator:: Do(GTraversal &state)
{	int ret = 0;

    state.Push();

    for (int i = 0; i < getNumChildren(); i++) {
		if (state.stop) break;
		ret = getChild(i)->Traverse(state);
	}
	
	state.Pop();
	return(ret);
}

int GvSeparator:: Do(GPathTraversal &state)
{	int ret = 0;
    state.Push();

	GPUSH_PATH(state,this);
		ret = TraverseChildren(state);
	GPOP_PATH(state);
	
	state.Pop();
	return(ret);
}


int GvSeparator::Do(GglRenderTraversal &state)
{  	int ret = 0;

// to do: do render culling here
    state.Push();

    for (int i = 0; i < getNumChildren(); i++) {
		if (state.stop) break;
		ret = getChild(i)->Traverse(state);
	}
	
	state.Pop();

	return(ret);

}




int GvSwitch::Do(GTraversal &state)
{
	if (state.traverseAll) { return(GvGroup::Do(state)); }

	int ret;
	GPUSH_PATH(state,this);


    int which = whichChild.value;

    if (which == GV_SWITCH_NONE)
	;

    else if (which == GV_SWITCH_ALL)
	for (int i = 0; i < getNumChildren(); i++)
	    ret= getChild(i)->Traverse(state);

    else if (which <0) {
        TRACE("Bad switch value : %d \n",which);
		ret = 0;
    }
	else if (which < getNumChildren())
	    ret = getChild(which)->Traverse(state);
	else 
		ret = 0;

	GPOP_PATH(state);

	return(ret);

}



int 
GvBlinker::Do(GTraversal &state)
{
	if (state.traverseAll) { return(GvGroup::Do(state)); }

	GTimeStack  *ts = (GTimeStack*) state.Get(GTimeStack::attrId);

	if (on && ts && ts->time.On()) {
		float t=ts->time.t;
		int nChilds = getNumChildren();

		t = offset + t *speed;
		if (t<0.0) t = -t;

		if (nChilds == 0) {
			whichChild.value=GV_SWITCH_NONE;
		} else 
		if (nChilds == 1) {
			t = fmod(t,1.0f);
			if (t<0.5f) whichChild.value=GV_SWITCH_NONE;
			else whichChild.value = 0;

		} else {

		  //t = fmod(t,1.0); // only up to .9999999999, so trucation goes downward
		  whichChild.value = fmod(t*nChilds,nChilds);
		
		  if (whichChild.value >= nChilds) whichChild.value = nChilds-1;
		}

	}
	

    
    
    int which = whichChild.value;
	int ret=0;

    if (which == GV_SWITCH_NONE)
	;

    else if (which == GV_SWITCH_ALL)
	for (int i = 0; i < getNumChildren(); i++)
	    ret=getChild(i)->Traverse(state);

    else
	if (which < getNumChildren())
	{
		if (upto)
			for (int i = 0; i <= which; i++)
	    		ret=getChild(i)->Traverse(state);

	    else ret=getChild(which)->Traverse(state);
	 }
	 return(ret);
}


int 
GvTransformSeparator::Do(GTraversal &state)
{
    int ret=0;
    // We need to "push" just the transformation stack. We'll
    // accomplish this by just pushing a no-op transformation onto
    // that stack. When we "pop", we'll restore that stack to its
    // previous state.

	GMatrixStack *current,*prev;
	
    // get top marker or matrix stack
    prev = (GMatrixStack*) state.Get(GMatrixStack::attrId);

    if (!prev) {
		TRACE("Matrix stack Attribute not initialized \n");
	}

    // push a new instance, 

	current = (GMatrixStack*) prev->NewInstance();
	current->SetMatrix(prev->m);
	state.Push(GMatrixStack::attrId,current);

    for (int i = 0; i < getNumChildren(); i++)
		ret = getChild(i)->Traverse(state);

    // Now do the "pop"
	// can´t simply pop, because all attributes are linked together
	// could do a find & linkout 

    // 13.04.96. hg newly pushed matrices wasn´t restored
	
	GMatrixStack* p;
    p = (GMatrixStack*) state.Get(GMatrixStack::attrId);

    // pop all top level state elements of type matrix
    while (state.top == p && p != prev) {
        state.Pop(p);
        p = (GMatrixStack*) state.Get(GMatrixStack::attrId);

    }
    if (p != prev) {
    // set all other lately pushed matrices to prev    
	while (p != NULL && p!=prev) {
	    p->SetMatrix(prev->m);
		p = (GMatrixStack *) p->prevThisAttr;
		 
	} 
    // apply changes
    p = (GMatrixStack*) state.Get(GMatrixStack::attrId);
   	p->Set(state);
    }


#if 0
	while (p != NULL && p ->prevThisAttr != prev) {
		p = (GMatrixStack *) p->prevThisAttr;
	    p->SetMatrix(prev->m);
		 
	}

	current->SetMatrix(prev->m); // could pop if top
	current->Set(state);
#endif    

   //  while (state->getTopElement(GvState::TransformationIndex) != markerElt)
   //	state->popElement(GvState::TransformationIndex);
    return(ret);
}

//////////////////////////////////////////////////////////////////////////////
//
// Properties.
//
//////////////////////////////////////////////////////////////////////////////

// create 

#define G_DO_PROPERTY(className, stackElement)				      \
int									     \
className::Do(GTraversal &state)		 \
{									     \
	/* TRACE("%s push %s \n",state.ClassName(),this->ClassName());*/	\
	stackElement *current,*prev;									 \
																  \
	/* get current */												  \
	prev = (stackElement*) state.Get(stackElement::attrId); \
	if (!prev) {															\
		/* ignore it, because not registerd (initialized)	*/				\
		/* TRACE("Attribute not registered \n");	*/						\
		return(0);															\
	 }																		\
																			\
	current = (stackElement *) prev->NewInstance();							\
	current->data = this;													 \
																			 \
	state.Push(stackElement::attrId,current);	    \
	return(1);																 \
}																			 




int GvCoordinate3::Do(GTraversal &state)			
{									      
//	TRACE("%s push %s \n",state.ClassName(),this->ClassName());
	GvCoordinate3Element *current,*prev;									
	/* get current */												
	prev = (GvCoordinate3Element*) state.Get(GvCoordinate3Element::attrId); 
	if (!prev) { /* ignore it, because not registerd (initialized)	*/				
		return(0);															
	}	
	if (prev->level == state.level) { // already pushed
	   prev->data = this;
	   return(1);
	}																	
																			
	current = (GvCoordinate3Element *) prev->NewInstance();								
	current->data = this;													

	state.Push(GvCoordinate3Element::attrId,current);			
	return(1);																
}




#if 1
//G_DO_PROPERTY(GvCoordinate3,		Coordinate3Index)
//G_DO_PROPERTY(GvMaterial,			GvMaterialElement)
G_DO_PROPERTY(GvMaterialBinding,	GvMaterialBindingElement)
G_DO_PROPERTY(GvNormal,				GvNormalElement)
G_DO_PROPERTY(GvNormalBinding,		GvNormalBindingElement)
G_DO_PROPERTY(GvShapeHints,			GvShapeHintsElement)
G_DO_PROPERTY(GvTextureCoordinate2,	GvTextureCoordinate2Element)
G_DO_PROPERTY(GvTexture2,			GvTexture2Element)
G_DO_PROPERTY(GvComplexity,			GvComplexityElement)
G_DO_PROPERTY(GvTextureCoordinateBinding,		GvTextureCoordinateBindingElement)
G_DO_PROPERTY(GvFontStyle,			GvFontStyleElement)
//G_DO_PROPERTY(GvTexture2Transform,	Texture2TransformationElement)
#endif




// GvMaterial
int GvMaterial::Do(GTraversal &state) 
{
#if 0
	GFaceColorStack *current,*prev;
	
	// get current
	prev = (GFaceColorStack*) state.Get(GFaceColorStack::attrId);
	if (!prev) {
		return(0);
	 }
	Point c;
	GetColor(0,c);

	current = (GFaceColorStack*) prev->NewInstance();
	current->color = c;
	state.Push(current);
#endif
	
	{
		GvMaterialElement *current,*prev;

		// get current
		prev = (GvMaterialElement*) state.Get(GvMaterialElement::attrId);
		if (!prev) {
			return(0);
	 	} else {
			if (prev->level == state.level) { // already pushed
			 	prev->data = this;
	   			prev->baseColor = NULL;
				prev->Set(state);
			} else {																	
				current = (GvMaterialElement *) prev->NewInstance();								
				current->data = this;
				current->baseColor = NULL;
				state.Push(GvMaterialElement::attrId,current);			
			}
			state.appearance = NULL; // this is a vrml 1.0 file, set appearance to 0

			return(1);																
		}
	 }
	return(1);
}



//
//  GvBaseColor
//
GvMFColor* GvMaterialElement::GetDiffuse() 
{
		if (baseColor) return(&baseColor->rgb);
		else  
		if (data) return &((GvMaterial *) data)->diffuseColor;
		else return (NULL);
}

int  GvMaterialElement::GetDiffuse(int i,Point &color) 
{
		if (baseColor) { baseColor->GetColor(i,color); return(1); }
		else  
		if (data) {
			 &((GvMaterial *) data)->diffuseColor.get1(i,color);
			 return(1);
		}
		else return (0);
}


// Get local colors with cycling

int 
GvMaterialElement::GetLocalColors(PointArray &colors,int period)
{
	GvMFColor* c=NULL;
	if (baseColor) {c = &baseColor->rgb; }
	else  
	if (data)  {
		GvMaterial *m= (GvMaterial *) data;
	  	
	  	c = &m->diffuseColor;
	  	if (m->emissiveColor.num > c->num)
			c=&m->emissiveColor;

	}
	if (!c) return(0);
	
	if (c->num <= 1) return(0);	// one color doesn´t make sense

	if (period<=0) period = c->num;

	
	for(int i=0;i<colors.Length(); i++) {
		c->get1( (i>=period) ? i % period:i,colors[i]);	
	}
	return(1);
}


void GvBaseColor::GetColor(int i,Point &color)
{
	i = i %	rgb.Length();
	rgb.get1(i,color);

}

// clean up !!!!!!!!!!!!!!!!!!!!!!1
int GvBaseColor::Do(GTraversal &state) 
{
#if 0
	GFaceColorStack *current,*prev;
	
	// get current
	prev = (GFaceColorStack*) state.Get(GFaceColorStack::attrId);
	if (!prev) {
		return(0);
	 }
	Point c;
	GetColor(0,c);

	current = (GFaceColorStack*) prev->NewInstance();
	current->color = c;
	state.Push(current);
#endif
	
	// set baseColor ptr in current material !!!!!!!
	{
		GvMaterialElement *current,*prev;
	
		// get current
		prev = (GvMaterialElement*) state.Get(GvMaterialElement::attrId);
		if (!prev) {
			return(0);
	 	} else {
			if (prev->level == state.level) { // already pushed
	   			prev->baseColor = this;
				prev->Set(state);
			} else {																	
				current = (GvMaterialElement *) prev->NewInstance();								
				current->data = prev->data;
				current->baseColor = this;
				state.Push(GvMaterialElement::attrId,current);			
			}
			return(1);																
		}
	 }
	//Do(* ((GTraversal *) &state));
	return(1);
}

// 

void GvglMaterialElement::Set(GTraversal &state) 
{
	GglRenderTraversal *s= (GglRenderTraversal *) &state;
	GvMaterial *material = (GvMaterial *) data;;
					  
  	if (material ) {
			///////&&  (state.glState->currentMaterialNode != material)
 		  	
 		  	// if material->IsEmmissiveOnly()
			//	s->SetLighting(0);
			//
			//else  s->SetLighting(1);

 		  	// if not glState.lighted 
#ifdef _OGL
 		  	material->glSet();
		  	material->glSetColor();	// could switch if lighted ??? 
#endif

  	}

  	s->glState->currentMaterialNode = material;
  	s->glState->currentBaseColorNode = baseColor;
  	s->glState->currentMaterialIndex = 0;

					  
	//if (!glState.lighting)  mat->glSetColor();
#ifdef _OGL
	if (baseColor) {
			Point c;
//?			s->glState->SetLocalColors(1); // hack
			baseColor->GetColor(0,c);

			//GetRGBA(int i,float *rgba,float a=1.0)
			// set diffuse 
			glColor(c);
	}
#endif
}

void GvShapeHintsElement::Set(GTraversal &state) 
{
	GvShapeHints *n = (GvShapeHints *) data;
					  
  	if (n) {
		// apply shape hints 
		state.SetCulling(n->CanCull(),n->vertexOrdering == (int)GvShapeHints::CLOCKWISE );
	}

}


void GvglShapeHintsElement::Set(GTraversal &state) 
{
	GglRenderTraversal *s= (GglRenderTraversal *) &state;
	GvShapeHints *n = (GvShapeHints *) data;
					  
  	if (n) {
		// apply shape hints 
		s->glState->SetCulling(n->CanCull(),n->vertexOrdering == (int)GvShapeHints::CLOCKWISE );

/*
	   s->glState->SetBackfaceCulling(n->CanCull());
	   if (n->vertexOrdering != GvShapeHints::UNKNOWN_ORDERING) {
	   	 s->glState->SetFrontFaceCW(n->vertexOrdering == (int)GvShapeHints::CLOCKWISE );
	   }
*/
  	}
}



//
// GvTexture2
//

int 			
GvTexture2::Do(GglRenderTraversal &state)
{
  // if no texture mapping ignore node  ! delete ? 15.09
  if (!state.useTextures) return(0);

  if (!Defined() && !state.stop ) {
	   Define(state);
  }

#if 0
// push element
  if (!loaded && !cannotLoad && !state.stop ) {
#if 0
#ifndef _GLVIEW_DLL
   	 // if (!busy && state.view && state.view->incrementalUpdates) state.glState->UpdateFrontBuffer();
     if (!busy && state.view && state.view->incrementalUpdates)  state.view->BeginIncrementalUpdates(state.view);
#endif
#endif


   	 // if (!busy && state.view && state.view->incrementalUpdates) state.glState->UpdateFrontBuffer();
     if (!busy && state.view && state.view->incrementalUpdates) state.view->BeginIncrementalUpdates();
     Load(state);
   }
  
#endif


    GvTexture2Element *current,*prev;
    prev = (GvTexture2Element*) state.Get(GvTexture2Element::attrId); 
	if (!prev) { /* ignore it, because not registerd (initialized)	*/				
		return(0);															
	}	
	if (prev->level == state.level) { // already pushed
	   prev->data = this;
	   prev->Set(state);
	   return(1);
	}																	
																			
	current = (GvTexture2Element *) prev->NewInstance();								
	current->data = this;													
	state.Push(GvTexture2Element::attrId,current);			
	return(1);																
}


void GvglTexture2Element::Set(GTraversal &state) 
{
	GglRenderTraversal *s= (GglRenderTraversal *) &state;
	GvTexture2 *texture2 = (GvTexture2 *) data;

	state.appearanceApplied=0;
	state.texture = NULL; 

	// to do:alpha func changed ??? 

  	if (texture2) {

		/// currently done by GvNodeShell: draw 
		//texture2->glSet(*s);
 	}
	else { 

	  // to do : only if no texture2 node ?
	  int wasTexturing = s->glState->texturing;
	  if (wasTexturing) {

#ifdef _D3D
		s->glState->SetTexturing(0);
#else
		s->glState->SetAnyTexturing(0);
#endif
		s->glState->textureDefined = 0;
      
		GvMaterial *material=NULL; 
		GvMaterialElement *materialTop = (GvMaterialElement*) state.Get(GvMaterialElement::attrId); 

	  
		// reset material color
		if (materialTop) {
#ifdef _OGL
   	  		Point c;
   	  		if (materialTop->GetDiffuse(0,c))
	  			glColor(c);
			else 
				state.defaultMaterial->glSetColor();

#endif
#ifdef _D3D
			
#endif
	  }
	  }
	}

}

#endif _G_VRML1



// Apply Alpha rule onto image 
int GvImage::ApplyAlpha()
{  int ret=1;
   // compute alpha channel ?
   //if (alpha.value < 1.0) 
   if (!alpha.isDefault())
   {
       if (alphaColor.isDefault()) 
       {   
            ret = image.setAlpha(alpha.value);
       }
       else 
           ret = image.setAlpha(alphaColor.value, alpha.value);
   }
   else ret=0;
   return(ret);
}


int 			
GvImage::Define(GglRenderTraversal &state,LPCTSTR fileName, LPCTSTR fileUrl,int fileFmt,GvBool reload)
{
	// pass to generic functions
	return Define(*((GTraversal *) &state), fileName, fileUrl, fileFmt);
}

// Read image file + store into image field 
//CreateTextureFromFile
int 			
GvImage::Define(GTraversal &state,LPCTSTR fileName, LPCTSTR fileUrl,int fileFmt,GvBool reload)
{

    int ret=0;
	ret = ReadImageFile(fileName,fileFmt,state,"Image",image);
    //if (ret>=0) image.OnChanged();
    if (ret>=0) { 
        // compute alpha channel ?
        if (ApplyAlpha()) 
            image.touch();
	    //loaded = 1; // check 
		SetDefined(1);

    }
	else {
	}
	defined = 1;
    return(ret);
}


//CreateTextureFromImage

int 			
GvImage::Define(GglRenderTraversal &state,GvSFImage *theImage)
{
	ASSERT(&image == theImage);
    int ret = 0;
    if (ApplyAlpha()) 
         image.touch();
	defined = 1;
    return(ret);
}


// get pointer to image,
// load it if need
// null if image not loaded or loader still busy

GvSFImage* GvImage::getImage(GglRenderTraversal &state)
{
	if (!Defined() && !state.stop ) {
	   Load(state);
	   if (!Defined()) return(NULL);
	}
#if 0
	if (!loaded && !cannotLoad && !state.stop ) {
   	 // if (!busy && state.view && state.view->incrementalUpdates) state.glState->UpdateFrontBuffer();
     if (!busy && state.view && state.view->incrementalUpdates)  state.view->BeginIncrementalUpdates();
     Load(state);
   }
   if (!loaded) return(NULL);
#endif

   return( &image);
}

GvSFImage* GvImage::getImage(GTraversal &state)
{
	if (!Defined() && !state.stop) {
		Load(state);
	}

	if (!Defined()) return(NULL);

#if 0
   if (!loaded && !cannotLoad && !state.stop ) {
       /// to do : problem here  Load(state);
   }
   if (!loaded) return(NULL);
#endif
   return( &image);
}

int 			
GvImage::Do(GglRenderTraversal &state)
{

  // if no image enabled ignore
	if (!Defined() && !state.stop) {
		Load(state);
	}
    if (!Defined()) return(0);

	//  if (state.view && !state.view->TextureMode) return(0);

#if 0
   if (!loaded && !cannotLoad && !state.stop ) {
   	 // if (!busy && state.view && state.view->incrementalUpdates) state.glState->UpdateFrontBuffer();
     if (!busy && state.view && state.view->incrementalUpdates)  state.view->BeginIncrementalUpdates();
     Load(state);
   }
   if (!loaded) return(0);
#endif

#if 0
	// don"t draw it
#ifdef _OGL

   if ((image.size[0]>0) && (image.size[1]>0) 
		&& image.numComponents >0 && image.bytes
		) {

            int format;
			switch(image.numComponents) {
			default :
			case 1 : format = GL_LUMINANCE; break;
			case 2 : format = GL_LUMINANCE_ALPHA; break;
			case 3 : format = GL_RGB;	break;
			case 4 : format = GL_RGBA; break;
			}

            glRasterPos3f(0.0,0.0,0.0);
            // move in bitmap keeps rasterpos valid
            glBitmap (0, 0, 0, 0, 0.0,0.0, NULL);


            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            if (format == GL_RGBA || format == GL_LUMINANCE_ALPHA) {
                // enable alphafunc
                int _alpha = glIsEnabled(GL_ALPHA_TEST);
                int _blend = glIsEnabled(GL_BLEND);
                if (!_alpha || !_blend) {
                    // no blending done, enable alpha func skipping pixels with alpha = 0.0    
                    GLint _alphaFunc; glGetIntegerv(GL_ALPHA_TEST_FUNC,&_alphaFunc);
                    GLfloat _alphaRef; glGetFloatv(GL_ALPHA_TEST_REF,&_alphaRef);
                    // change state
                    glAlphaFunc(GL_GREATER,0.0);
                    glEnable(GL_ALPHA_TEST);


                    glDrawPixels(image.size[0],image.size[1],format, GL_UNSIGNED_BYTE,image.bytes);
                    
                    // undo state change    
                    glAlphaFunc(_alphaFunc,_alphaRef);
                    if (_alpha) glEnable(GL_ALPHA_TEST); else  glDisable(GL_ALPHA_TEST);
                } 
                else 
                  glDrawPixels(image.size[0],image.size[1],format, GL_UNSIGNED_BYTE,image.bytes);

            } else 
                glDrawPixels(image.size[0],image.size[1],format, GL_UNSIGNED_BYTE,image.bytes);
    }

    return(1);
#else 
    return(0);
#endif
#else
    return(1);
#endif
      
}


#if 0
int GvScaledImage::set(GvSFImage *im)
{

}
#endif


int 			
GvScaledImage::Define(GglRenderTraversal &state,LPCTSTR fileName, LPCTSTR fileUrl,int fileFmt,GvBool reload)
{
    int ret = GvImage::Define(state,fileName,fileUrl,fileFmt);
    ApplyScale(state);
    return(ret);
}

int 			
GvScaledImage::Define(GTraversal &state,LPCTSTR fileName, LPCTSTR fileUrl,int fileFmt,GvBool reload)
{
    int ret = GvImage::Define(state,fileName,fileUrl,fileFmt);
    ApplyScale(state);
    return(ret);
}


int 			
GvScaledImage::Define(GglRenderTraversal &state,GvSFImage *theImage)
{
    int ret = GvImage::Define(state,theImage);
    ApplyScale(state);
    return(ret);
}

int 			
GvScaledImage::ApplyScale(GTraversal &state)
{
    // problem for scale == TO_WINDOW render context needed 
    //return(0);

    int newWidth,newHeight;
    int checkOnly = 0; // to a size check, because of window size dependency
    
    // check if we need a rescale 
//    if (loaded && scale.value == TO_WINDOW) checkOnly = 1; 
    if (Defined() && scale.value == TO_WINDOW) checkOnly = 1; 

    // check if we need a rescale 
    if ( (!Defined() || checkOnly) && srcImage.value) 
	{

		GvSFImage *im = NULL;	
		if (RTISA(srcImage.value,GvImage)) {
			GvImage *s = (GvImage *) (GvNode *) srcImage;
			im = s->getImage(state);
		} 
		else if (RTISA(srcImage.value,GvScaledImage)) {
			GvScaledImage *s = (GvScaledImage *) (GvNode *) srcImage;
			im = s->getImage(state);
		} 
		else if (RTISA(srcImage.value,GvImageTexture)) {
			GvPixelTexture *s = (GvPixelTexture *) (GvNode *) srcImage;
			im = s->getImage(state);
		} 
        
        // image not ready yet 
        if (!im) return(0);
        if (!im->bytes) return(0);

        switch (scale.value) {
        default :
        case NONE:
                newWidth = im->size[0];
                newHeight = im->size[1];
                break;
        case TO_SIZE:
                newWidth = scaleFactor.value[0];
                newHeight = scaleFactor.value[1];
                break;

        case BY_FACTOR:
                newWidth = scaleFactor.value[0] * im->size[0];
                newHeight = scaleFactor.value[1] * im->size[1];
                break;

        case TO_WINDOW: 
				return(-1);
                
                break;
        }

        if (newWidth == 0) { // scale isotropic 
            float r= (float) im->size[0] / im->size[1];
            newWidth = newHeight * r;
        } else  if (newHeight == 0) { // scale isotropic 
             float r= (float) im->size[1] / im->size[0];
             newHeight = newWidth * r;
        }
        
        if (checkOnly && image.size[0] == newWidth && image.size[1] == newHeight) {
            // do nothing 
        }
        else {
            TRACE("%s::Scaling from (%d %d) to (%d %d) ",ClassName(),im->size[0],im->size[1],newWidth,newHeight);
            int ret = image.setScaled(newWidth,newHeight,im->size[0],im->size[1],im->numComponents,im->bytes);
            if (ret>0) {
                ApplyAlpha();
           
            image.touch(); // trigger routes
        }  
        
//        loaded = 1;
		SetDefined(1);
        }

    }

	return(1);
}

#if 0
int 			
GvScaledImage::ApplyScale(GvSFImage *im)
{
    // problem for scale == TO_WINDOW render context needed 

    return(0);
}
#endif


int 			
GvScaledImage::Do(GglRenderTraversal &state)
{
    int newWidth,newHeight;
    int checkOnly = 0; // to a size check, because of window size dependency
    
    // check if we need a rescale 
    if (Defined() && scale.value == TO_WINDOW) checkOnly = 1; 
  


    // check if we need a rescale 
    if ( (!Defined() || checkOnly)  
            && srcImage.value) {

		GvSFImage *im = NULL;	
		if (RTISA(srcImage.value,GvImage)) {
			GvImage *s = (GvImage *) (GvNode *) srcImage;
			im = s->getImage(state);
		} 
		else if (RTISA(srcImage.value,GvScaledImage)) {
			GvScaledImage *s = (GvScaledImage *) (GvNode *) srcImage;
			im = s->getImage(state);
		} 
		else if (RTISA(srcImage.value,GvImageTexture)) {
			GvPixelTexture *s = (GvPixelTexture *) (GvNode *) srcImage;
			im = s->getImage(state);
		} 
        
        // image not ready yet 
        if (!im) return(0);
        if (!im->bytes) return(0);

        switch (scale.value) {
        default :
        case NONE:
                newWidth = im->size[0];
                newHeight = im->size[1];
                break;
        case TO_SIZE:
                newWidth = scaleFactor.value[0];
                newHeight = scaleFactor.value[1];
                break;

        case BY_FACTOR:
                newWidth = scaleFactor.value[0] * im->size[0];
                newHeight = scaleFactor.value[1] * im->size[1];
                break;

        case TO_WINDOW: {
                int windowWidth = state.glState->renderSizex; //640;
                int windowHeight = state.glState->renderSizey; // 480 

                newWidth = scaleFactor.value[0] * windowWidth;
                newHeight = scaleFactor.value[1] * windowHeight;
                }
                
                break;
        }

        if (newWidth == 0) { // scale isotropic 
            float r= (float) im->size[0] / im->size[1];
            newWidth = newHeight * r;
        } else  if (newHeight == 0) { // scale isotropic 
             float r= (float) im->size[1] / im->size[0];
             newHeight = newWidth * r;
        }
        
        if (checkOnly && image.size[0] == newWidth && image.size[1] == newHeight) {
            // do nothing 
        }
        else {
            TRACE("%s::Scaling from (%d %d) to (%d %d) ",ClassName(),im->size[0],im->size[1],newWidth,newHeight);
            int ret = image.setScaled(newWidth,newHeight,im->size[0],im->size[1],im->numComponents,im->bytes);
            if (ret>0) {
                ApplyAlpha();
           
            image.touch(); // trigger routes
        }  
        
        // loaded = 1;
        SetDefined(1);
        }

    }

    return(GvImage::Do(state));
}



// Light

void GvglLightElement::Set(GTraversal &state) 
{
  GglRenderTraversal *s= (GglRenderTraversal *) &state;

  GvLight * light = (GvLight *) data;
  
  // special case encountered light == NULL, index == -1

//  TRACE("light elementl %d %d  \n",index ,s->glState->currentMaxLight);

#ifdef _OGL  	
  if (light && index >= s->glState->currentMaxLight ) {
  	light->glSet(index,*(GglRenderTraversal *)&state);
	glEnable((GLenum) (GL_LIGHT0+index));
	s->glState->currentMaxLight = index;
	// TRACE("Enable l %d \n",index);
  }	else {
	// Disable all lights above
	// TRACE("Disable l %d %d  \n",index+1,s->glState->currentMaxLight);

	for (int i=index+1; i<=s->glState->currentMaxLight; i++)
		glDisable((GLenum) (GL_LIGHT0+i));
	s->glState->currentMaxLight  = index;
  }
#endif

#ifdef _D3D
  if (light && index >= s->glState->currentMaxLight ) { // light beeing pushed
  	light->glSet(index,*(GglRenderTraversal *)&state);

	s->glState->currentMaxLight = index;
	TRACE("Enable l %d \n",index);

  }	else { // light beeing popped, disable all lights above
	 //TRACE("Disable l %d %d  \n",index+1,s->glState->currentMaxLight);

	 
	 for (int i=index+1; i<=s->glState->currentMaxLight; i++)
		s->glState->DisableLight(i);

	 s->glState->currentMaxLight  = index;
  }


#endif

}
// generic traversal, push light into state 

int 
GvLight::Do(GTraversal &state)
{
	GvLightElement *current,*prev;

	if (!on) return(0);

	// get current light element 
	prev = (GvLightElement*) state.Get(GvLightElement::attrId);
	if (!prev) {
		return(0);
 	} else {
		// create new instance
		current = (GvLightElement *) prev->NewInstance();								
		current->data = this;
		current->index = prev->index + 1;
		// and push, this will apply the light 
		state.Push(GvLightElement::attrId,current);			
	 }
	return(1);
}

void GvLight::glSetOff(RenderState *glState)
{
#ifdef _D3D
	   G3LightRef &l = getLight();
	   // remove light from view	
	   glState->RemoveLight(l);

#endif 
}

// set as light i in glState
// overwritten by specialized versions
GvBool GvLight::glSet(RenderState *glState,int i,GCamera *camera,const Matrix &m)
{
	return FALSE;
}

int 
GvDirectionalLight::Do(GglRenderTraversal &state)
{
  if (!on) { 
	  glSetOff(state.glState); 
	  return(0);
  }	
  if (global) // already made global before rendering 
  	return(0);
  if (state.view && !state.view->useSceneLights) return(0);
  return(GvLight::Do(state));
}

int 
GvPointLight::Do(GglRenderTraversal &state)
{
  if (!on) { 
	  glSetOff(state.glState); 
	  return(0);
  }	
  if (state.globalPointLights) return(0);
  if (state.view && !state.view->useSceneLights) return(0);

  if (!isRegistered) {
		state.getBrowser()->Register(this);
  }
  return(GvLight::Do(state));
}

int 
GvSpotLight::Do(GglRenderTraversal &state)
{
  if (!on) { 
	  glSetOff(state.glState); 
	  return(0);
  }	
  if (state.globalPointLights) return(0);
  if (state.view && !state.view->useSceneSpotLights) return(0);

  if (!isRegistered) {
		state.getBrowser()->Register(this);
  }

  return(GvLight::Do(state));
}

// set as light i in glState
void GvDirectionalLight::glSet(int i,GglRenderTraversal &state)
{
  glSet(state.glState,i,state.physicalCamera,*state.GetMatrix());
}

// set as light i in glState
//void GvDirectionalLight::glSet(int i,GglRenderTraversal &state)
GvBool GvDirectionalLight::glSet(RenderState *glState,int i,GCamera *camera,const Matrix &m)
{
  
  GLfloat c4[4];
  GLfloat ac4[4];
  float factor;

  color.GetRGBA(c4);


  factor = ambientIntensity.get();

  ac4[0] = c4[0] * factor;
  ac4[1] = c4[1] * factor;
  ac4[2] = c4[2] * factor;
  ac4[3] = 1.0f;	
  
  factor = intensity * glState->lightScale;

  c4[0] *= factor;
  c4[1] *= factor;
  c4[2] *= factor;
  
  // TRACE("set Directional light %d %f\n",i,factor);


#ifdef _OGL
  GLfloat position4[4];

  glDisable((GLenum)(GL_LIGHT0+i));

  glLightfv((GLenum)(GL_LIGHT0+i),GL_DIFFUSE,c4);
  glLightfv((GLenum)(GL_LIGHT0+i),GL_SPECULAR,c4);
  glLightfv((GLenum)(GL_LIGHT0+i),GL_AMBIENT,ac4);

  Point dir;
  direction.get(dir);
  dir.Normalize();

  position4[0]=-dir.x;
  position4[1]=-dir.y;
  position4[2]=-dir.z;
  position4[3]=0;
 
  glLightfv((GLenum)(GL_LIGHT0+i),GL_POSITION,position4);
  glLighti((GLenum)(GL_LIGHT0+i),GL_SPOT_CUTOFF,180);

  glLightf((GLenum)(GL_LIGHT0+i),GL_CONSTANT_ATTENUATION,attenuation[0]);
  glLightf((GLenum)(GL_LIGHT0+i),GL_LINEAR_ATTENUATION,attenuation[1]);
  glLightf((GLenum)(GL_LIGHT0+i),GL_QUADRATIC_ATTENUATION,attenuation[2]);

  glEnable((GLenum)(GL_LIGHT0+i));

#endif 

#ifdef _D3D
   changed = changed || ! getLight().Ok();
   
   G3LightHandle *l = glState->GetLight(getLight()); // get pointer to light structure

	   
   Point dir;
   direction.get(dir);
   dir = RotateOnly(m,dir);
   dir.Normalize();

   //dir *= -1; // need flip because of handedness change

   // check if matrix above light changed	
   if (!changed &&  ! (dir == currentDirection)) {
	   changed = 1;
   }	


   if (changed ) {

	   l->SetType(D3DLIGHT_DIRECTIONAL);

	   if (glState->canDoRgbLighting) {
			l->dcvColor.r = c4[0];
			l->dcvColor.g = c4[1];
			l->dcvColor.b = c4[2];
			l->dcvColor.a = c4[3];
	   } else {
			l->dcvColor.r = factor; // 1.0;
			l->dcvColor.g = factor; // 1.0;
			l->dcvColor.b = factor; //1.0;

			l->dcvColor.r = c4[0];
			l->dcvColor.g = c4[1];
			l->dcvColor.b = c4[2];

			l->dcvColor.a = factor;
	   }	

	   l->dvDirection.x = dir.x;
	   l->dvDirection.y = dir.y;
	   l->dvDirection.z = dir.z;
	   l->dvRange = 0.0f;

	   currentDirection = dir;

	   l->ambientColor.r = ac4[0];
	   l->ambientColor.g = ac4[1];
	   l->ambientColor.b = ac4[2];
	   l->ambientColor.a = ac4[3];

	   l->ambientIntensity = ambientIntensity.get();

	   // set the light 
	   glState->SetLight(getLight());
	   glState->EnableLight(i, getLight());


	   changed = 0;
   }	
   else {
	   glState->EnableLight(i, getLight());
   }	

#endif
    return (TRUE);

}

// set as light i in glState
void GvPointLight::glSet(int i,GglRenderTraversal &state)
{
  glSet(state.glState,i,state.physicalCamera,*state.GetMatrix());
}

// set as light i in glState
GvBool GvPointLight::glSet(RenderState *glState,int i,GCamera *camera,const Matrix &m)
{
  GLfloat c4[4];
  GLfloat ac4[4];

  float factor;


  color.GetRGBA(c4);

  factor = ambientIntensity.get();

  ac4[0] = c4[0] * factor;
  ac4[1] = c4[1] * factor;
  ac4[2] = c4[2] * factor;
  ac4[3] = 1.0f;	

  factor = intensity * glState->lightScale;

  c4[0] *= factor;
  c4[1] *= factor;
  c4[2] *= factor;

  // TRACE("Point light %d\n",i);

#ifdef _OGL

  GLfloat position4[4];

  if (i>=glState->maxLights) return FALSE; // too many lights 

  glDisable((GLenum)(GL_LIGHT0+i));

  glLightfv((GLenum)(GL_LIGHT0+i),GL_DIFFUSE,c4);
  glLightfv((GLenum)(GL_LIGHT0+i),GL_SPECULAR,c4);
  glLightfv((GLenum)(GL_LIGHT0+i),GL_AMBIENT,ac4);

  position4[0]=location[0];
  position4[1]=location[1];
  position4[2]=location[2];
  position4[3]=1.0;

  glLightfv((GLenum)(GL_LIGHT0+i),GL_POSITION,position4);
  glLightf((GLenum)(GL_LIGHT0+i),GL_SPOT_CUTOFF,180);
  glLightf((GLenum)(GL_LIGHT0+i),GL_SPOT_EXPONENT,0);

  glCheckError();

  glLightf((GLenum)(GL_LIGHT0+i),GL_CONSTANT_ATTENUATION,(attenuation[0] == 0.0 ? 1.0:attenuation[0]));
  glLightf((GLenum)(GL_LIGHT0+i),GL_LINEAR_ATTENUATION,attenuation[1]);
  glLightf((GLenum)(GL_LIGHT0+i),GL_QUADRATIC_ATTENUATION,attenuation[2]);
  glCheckError();


  glEnable((GLenum)(GL_LIGHT0+i));
  //glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,1);

#endif

#ifdef _D3D

   if (!changed && !getLight().Ok()) {
	   changed = 1;
   }	

   G3LightHandle *l = glState->GetLight(getLight()); // get pointer to light structure

   Point pos;
   location.get(pos);
   pos *= m;

   if (!changed &&  ! (pos == currentPosition)) {
	   changed = 1;
   }	

   if ( changed) {

	   l->SetType(D3DLIGHT_POINT);
	   

	   if (glState->canDoRgbLighting) {
			l->dcvColor.r = c4[0];
			l->dcvColor.g = c4[1];
			l->dcvColor.b = c4[2];
			l->dcvColor.a = c4[3];
	   } else {
			l->dcvColor.r = 1.0;
			l->dcvColor.g = 1.0;
			l->dcvColor.b = 1.0;

			l->dcvColor.r = c4[0];
			l->dcvColor.g = c4[1];
			l->dcvColor.b = c4[2];

			l->dcvColor.a = factor;
	   }	

	   l->dvPosition.x = pos.x;
	   l->dvPosition.y = pos.y;
	   l->dvPosition.z = pos.z;

	   currentPosition = pos;

	   l->dvAttenuation0 = attenuation[0];
	   l->dvAttenuation1 = attenuation[1];
	   l->dvAttenuation2 = attenuation[2];

	   l->dvRange = (float) radius; 

	   l->ambientColor.r = ac4[0];
	   l->ambientColor.g = ac4[1];
	   l->ambientColor.b = ac4[2];
	   l->ambientColor.a = ac4[3];
	   l->ambientIntensity = ambientIntensity.get();


	   // set the light 
	   glState->SetLight(getLight());

	   changed = 0;
   }	

#endif
   return (TRUE);
}


// set as light i in glState
void GvSpotLight::glSet(int i,GglRenderTraversal &state)
{
  glSet(state.glState,i,state.physicalCamera,*state.GetMatrix());
}

// set as light i in glState
GvBool GvSpotLight::glSet(RenderState *glState,int i,GCamera *camera,const Matrix &m)
{
  GLfloat c4[4];
  GLfloat ac4[4];
  float factor;


  color.GetRGBA(c4);

  factor = ambientIntensity.get();

  ac4[0] = c4[0] * factor;
  ac4[1] = c4[1] * factor;
  ac4[2] = c4[2] * factor;
  ac4[3] = 1.0f;	

  factor = intensity * glState->lightScale;

  c4[0] *= factor;
  c4[1] *= factor;
  c4[2] *= factor;

  // TRACE("Spot light %d\n",i);

#ifdef _OGL

  GLfloat position4[4];
  GLfloat direction4[4];

  if (i>=glState->maxLights) return FALSE; // too many lights 

  glDisable((GLenum)(GL_LIGHT0+i));

  glLightfv((GLenum)(GL_LIGHT0+i),GL_DIFFUSE,c4);
  glLightfv((GLenum)(GL_LIGHT0+i),GL_SPECULAR,c4);
  glLightfv((GLenum)(GL_LIGHT0+i),GL_AMBIENT,ac4);

  position4[0]=location[0];
  position4[1]=location[1];
  position4[2]=location[2];
  position4[3]=1.0;
  glLightfv((GLenum)(GL_LIGHT0+i),GL_POSITION,position4);

  Point dir;
  direction.get(dir);
  dir.Normalize();

  
  direction4[0]=dir.x;
  direction4[1]=dir.y;
  direction4[2]=dir.z;
  direction4[3]=0;

  glLightfv((GLenum)(GL_LIGHT0+i),GL_SPOT_DIRECTION,direction4);
  glLightf((GLenum)(GL_LIGHT0+i),GL_SPOT_CUTOFF,RAD2DEG(cutOffAngle));
  float exponent = 0.5 * log(0.5) / log(cos(beamWidth));
  float beam = beamWidth;

  //if (cutOffAngle > beam) beam = cutOffAngle;
  if (1) //vrml 2
     glLightf((GLenum)(GL_LIGHT0+i),GL_SPOT_EXPONENT,exponent);
  else 
     glLightf((GLenum)(GL_LIGHT0+i),GL_SPOT_EXPONENT,dropOffRate * 128.0);
  
  glLightf((GLenum)(GL_LIGHT0+i),GL_CONSTANT_ATTENUATION,attenuation[0]);
  glLightf((GLenum)(GL_LIGHT0+i),GL_LINEAR_ATTENUATION,attenuation[1]);
  glLightf((GLenum)(GL_LIGHT0+i),GL_QUADRATIC_ATTENUATION,attenuation[2]);

  glEnable((GLenum)(GL_LIGHT0+i));
#endif

#ifdef _D3D

  if (!changed && !getLight().Ok()) {
	   changed = 1;
   }	

   G3LightHandle *l = glState->GetLight(getLight()); // get pointer to light structure

   Point dir;
   direction.get(dir);
   dir = RotateOnly(m,dir);
   dir.Normalize();

   //dir *= -1; // need flip because of handedness change

   Point pos;
   location.get(pos);
   pos *= m;


   // check if matrix above light changed	
   if (!changed &&  ( !(pos == currentPosition) || !(dir == currentDirection))) {
	   changed = 1;
   }	

   if (changed) {

	   l->SetType(D3DLIGHT_SPOT);
	   
	   if (glState->canDoRgbLighting) {
			l->dcvColor.r = c4[0];
			l->dcvColor.g = c4[1];
			l->dcvColor.b = c4[2];
			l->dcvColor.a = c4[3];
	   } else {
			l->dcvColor.r = 1.0;
			l->dcvColor.g = 1.0;
			l->dcvColor.b = 1.0;

			l->dcvColor.r = c4[0];
			l->dcvColor.g = c4[1];
			l->dcvColor.b = c4[2];

			l->dcvColor.a = factor;
	   }	

	   l->dvDirection.x = dir.x;
	   l->dvDirection.y = dir.y;
	   l->dvDirection.z = dir.z;

	   l->dvPosition.x = pos.x;
	   l->dvPosition.y = pos.y;
	   l->dvPosition.z = pos.z;

	   currentPosition = pos;
	   currentDirection = dir;

	   l->dvPhi = (float) cutOffAngle; /* Angle, in radians, defining the outer edge of the spotlight's penumbra. Points outside this cone are not lit by the spotlight.  */

	   l->dvRange = (float) radius; 

	   float beam = beamWidth;
	   if (beam > cutOffAngle ) beam = cutOffAngle.value;
	   l->dvTheta = beam;  /* Angle, in radians, of the spotlight's umbra—that is, the fully illuminated spotlight cone. */
	 
	   //float exponent = 0.5 * log(0.5) / log(cos(beamWidth));
	   // l->dvFalloff = dropOffRate * 128.0;
	   l->dvFalloff = 1.0; // dropOffRate;

	   l->dvAttenuation0 = attenuation[0];
	   l->dvAttenuation1 = attenuation[1];
	   l->dvAttenuation2 = attenuation[2];

	   l->ambientColor.r = ac4[0];
	   l->ambientColor.g = ac4[1];
	   l->ambientColor.b = ac4[2];
	   l->ambientColor.a = ac4[3];

	   l->ambientIntensity = ambientIntensity.get();

	   // set the light 
	   glState->SetLight(getLight());

	   changed = 0;
   }	

#endif
   return TRUE;

}

#ifdef _G_VRML1

int 
GvLightModel::Do(GTraversal &state)
{
	GvLightModelElement *current,*prev;
	// get current
	prev = (GvLightModelElement*) state.Get(GvLightModelElement::attrId);
	if (!prev) {
		return(0);
 	} else {
	   // to do: replace
		current = (GvLightModelElement *) prev->NewInstance();								
		state.Push(GvLightModelElement::attrId,current);			
	 }
	return(1);
}

int 
GvLightModel::Do(GglRenderTraversal &state)
{
	GvLightModelElement *current,*prev;
	// get current
	prev = (GvLightModelElement*) state.Get(GvLightModelElement::attrId);
	if (!prev) {
		return(0);
 	} else {
	   // to do: replace
		current = (GvLightModelElement *) prev->NewInstance();								
		state.Push(GvLightModelElement::attrId,current);			
	 }
	return(1);
}


void GvglLightModelElement::Set(GTraversal &state) 
{
	GglRenderTraversal *s= (GglRenderTraversal *) &state;
	GvLightModel *model = (GvLightModel *) data;;
					  
  	if (model ) {
// to do:
	}

}


// do boundingbox 
int GvCoordinate3::Do(GBBoxTraversal &state)
{

  // if (state.view && state.view->computeExtendedBBox) 
  
  return Do(* ((GTraversal *) &state));

#if 0
// a shape nodes might use coordinates, but here can
// be a matrix in between  !!!!!!!!!!!!!

  BBox box;
  const Point *vp,*vpend;
  Point mi,ma;

  if (point.Length()<=0) return(0);

  vp= (Point *) point.values;
  vpend=&vp[point.Length()];

  mi= ma= *vp++;
  for (; vp<vpend;vp++) {
		vp->MinMax(mi,ma);
  }

  box.mi = mi;
  box.ma = ma;
  
  state.Union(box);
  // to do ? anyway register attribute
  Do(* ((GTraversal *) &state));
  return(1);
#endif
}

#endif _G_VRML1


/* register all qv property elements for  nodes to state */
void InitializeGvAttributes(GTraversal &state,int all)
{	

   state.Set(GvWWWAnchorElement::NewElement());
   state.Set(GvWWWInlineElement::NewElement()); // needed ?


#ifdef _G_INVENTOR

   state.Set(GvProfileCurveElement::NewElement());
  state.Set(GvProfileCoordElement::NewElement());
//  state.Set(GvCoordinate4Element::NewElement());

#endif _G_INVENTOR

#ifdef _G_VRML1

  state.Set(GvCoordinate3Element::NewElement());
  state.Set(GvMaterialBindingElement::NewElement());
  state.Set(GvNormalBindingElement::NewElement());
  state.Set(GvNormalElement::NewElement());

  
  state.Set(GvTextureCoordinate2Element::NewElement());
  state.Set(GvTextureCoordinateBindingElement::NewElement());
  state.Set(GvFontStyleElement::NewElement());

  state.Set(GvComplexityElement::NewElement());
  
//  state.Set(new GvWWWPathElement());

  if (state.view) {
  	if (state.view->useMaterials)	{
  		GvMaterialElement *mat= GvMaterialElement::NewElement();
  		mat->baseColor = NULL;
  		state.Set(mat);
	}
  	if (state.view->useShapeHints)   state.Set(GvShapeHintsElement::NewElement());
	if (state.view->useTextures) state.Set(GvTexture2Element::NewElement());
	

  } else {
    
    GvMaterialElement *mat= GvMaterialElement::NewElement();
    mat->baseColor = NULL;
    state.Set(mat);
  	state.Set(GvShapeHintsElement::NewElement());
	state.Set(GvTexture2Element::NewElement());
  }
#endif // _G_VRML1
}

#ifdef _G_VRML1

// GvTransform
int GvTransform::Do(GTraversal &state)   
{ 
	GMatrixStack *current,*prev;
	prev = (GMatrixStack*) state.Get(GMatrixStack::attrId);

	if (!cached) { 
	   Get(cache);
	   cached = 1;
	}

    const Matrix &m = cache;

	//if (!prev) return(0);
	
	if (prev->level == state.level) { // already pushed
	   // new prev->SetMatrix(m * prev->m);
	   prev->AppendLeft(m);
	   prev->Set(state);
	   return(1);
	}																	

	current = (GMatrixStack*) prev->NewInstance();
	current->m = m * prev->m;
	state.Push(current);
	return(1);
}

// get matrix of node , FALSE if node has no matrix 
GvBool GvTransform::getMatrix(Matrix &m)
{
	if (!cached) { 
	   Get(cache);
	   cached = 1;
	}

	m = cache;
	return(TRUE);
}


int GvRotation::Do(GTraversal &state)   
{ 
	GMatrixStack *current,*prev;
	prev = (GMatrixStack*) state.Get(GMatrixStack::attrId);
	if (!prev) return(0);

	Matrix m;
	get(m);

	if (prev->level == state.level) { // already pushed
      prev->AppendLeft(m);
	   prev->Set(state);
	   return(1);
	}																	

	current = (GMatrixStack*) prev->NewInstance();
	current->m = m * prev->m;
	state.Push(current);
	return(1);
}

// get matrix of node , FALSE if node has no matrix 
GvBool GvRotation::getMatrix(Matrix &m)
{
	get(m);
	return(TRUE);
}


int GvRotationXYZ::Do(GTraversal &state)   
{ 
	GMatrixStack *current,*prev;
	prev = (GMatrixStack*) state.Get(GMatrixStack::attrId);
	if (!prev) return(0);

	Matrix m;
	get(m);

	if (prev->level == state.level) { // already pushed
      prev->AppendLeft(m);
	   prev->Set(state);
	   return(1);
	}																	

	current = (GMatrixStack*) prev->NewInstance();
	current->m = m * prev->m;
	state.Push(current);
	return(1);
}

// get matrix of node , FALSE if node has no matrix 
GvBool GvRotationXYZ::getMatrix(Matrix &m)
{
	get(m);
	return(TRUE);
}


int GvRotor::Do(GTraversal &state)   
{ 
	GMatrixStack *current,*prev;
	prev = (GMatrixStack*) state.Get(GMatrixStack::attrId);
	if (!prev) return(0);
	GTimeStack  *ts = (GTimeStack*) state.Get(GTimeStack::attrId);

	if (!on || !ts || !ts->time.On()) return(0);
	
	float t=ts->time.t;
	
	Matrix m;
	Get(m,t);

	if (prev->level == state.level) { // already pushed
      prev->AppendLeft(m);
	   prev->Set(state);
	   return(1);
	}																	
	current = (GMatrixStack*) prev->NewInstance();
	current->m = m * prev->m;
	state.Push(current);
	return(1);
}

int GvPendulum::Do(GTraversal &state)   
{ 
	GMatrixStack *current,*prev;
	prev = (GMatrixStack*) state.Get(GMatrixStack::attrId);
	if (!prev) return(0);
	GTimeStack  *ts = (GTimeStack*) state.Get(GTimeStack::attrId);

	if (!on || !ts || !ts->time.On()) return(0);
	
	float t=ts->time.t;
	
	Matrix m;
	Get(m,t);

	if (prev->level == state.level) { // already pushed
       prev->AppendLeft(m);
	   prev->Set(state);
	   return(1);
	}																	
	current = (GMatrixStack*) prev->NewInstance();
	current->m = m * prev->m;
	state.Push(current);
	return(1);
}

int GvShuttle::Do(GTraversal &state)   
{ 
	GMatrixStack *current,*prev;
	prev = (GMatrixStack*) state.Get(GMatrixStack::attrId);
	if (!prev) return(0);
	GTimeStack  *ts = (GTimeStack*) state.Get(GTimeStack::attrId);

	if (!on || !ts || !ts->time.On()) return(0);
	
	float t=ts->time.t;
	
	Matrix m;
	Get(m,t);

	if (prev->level == state.level) { // already pushed
       prev->AppendLeft(m);
	   prev->Set(state);
	   return(1);
	}																	
	current = (GMatrixStack*) prev->NewInstance();
	current->m = m * prev->m;
	state.Push(current);
	return(1);
}



int GvScale::Do(GTraversal &state)   
{ 
	GMatrixStack *current,*prev;
	prev = (GMatrixStack*) state.Get(GMatrixStack::attrId);

	Matrix m;
	Get(m);

	if (prev->level == state.level) { // already pushed
	   prev->AppendLeft(m);
	   prev->Set(state);
	   return(1);
	}																	

	current = (GMatrixStack*) prev->NewInstance();
	current->m = m * prev->m;
	state.Push(current);
	return(1);
}

// get matrix of node , FALSE if node has no matrix 
GvBool GvScale::getMatrix(Matrix &m)
{
	Get(m);
	return(TRUE);
}



int GvTranslation::Do(GTraversal &state)   
{ 
	GMatrixStack *current,*prev;
	prev = (GMatrixStack*) state.Get(GMatrixStack::attrId);

	Matrix m;
	Get(m);

	if (prev->level == state.level) { // already pushed
       prev->AppendLeft(m);
	   prev->Set(state);
	   return(1);
	}																	

	current = (GMatrixStack*) prev->NewInstance();
	current->m = m * prev->m;
	state.Push(current);
	return(1);
}

// get matrix of node , FALSE if node has no matrix 
GvBool GvTranslation::getMatrix(Matrix &m)
{
	Get(m);
	return(TRUE);
}



int GvMatrixTransform::Do(GTraversal &state)   
{ 
	GMatrixStack *current,*prev;
	prev = (GMatrixStack*) state.Get(GMatrixStack::attrId);

	Matrix m;
	get(m);

	if (prev->level == state.level) { // already pushed
       prev->AppendLeft(m);
	   prev->Set(state);
	   return(1);
	}																	

	current = (GMatrixStack*) prev->NewInstance();
	current->m = m * prev->m;
	state.Push(current);
	return(1);
}

// get matrix of node , FALSE if node has no matrix 
GvBool GvMatrixTransform::getMatrix(Matrix &m)
{
	get(m);
	return(TRUE);
}


// GvTexture2Transform
int GvTexture2Transform::Do(GTraversal &state)   
{ 
	GTextureMatrixStack *current,*prev;
	prev = (GTextureMatrixStack*) state.Get(GTextureMatrixStack::attrId);
	if (!prev) return(0);

    Matrix m;
	Get(m);

	if (prev->level == state.level) { // already pushed
       prev->AppendLeft(m);
	   prev->Set(state);
	   return(1);
	}																	
	current = (GTextureMatrixStack*) prev->NewInstance();
	current->m = m* prev->m;

	state.Push(current);
	return(1);
}

int GvTexture2Transform::Do(GglRenderTraversal &state)   
{ 
	GglTextureMatrixStack *current,*prev;
	prev = (GglTextureMatrixStack*) state.Get(GTextureMatrixStack::attrId);
	if (!prev) return(0);

    Matrix m;


#ifdef _D3D
	if (changed || mVersion == 0) {
		// create new texture matrix version 
		mVersion = state.glState->GetNewTextureMatrixVersion();
		changed = 0;
	}
#endif

	Get(m);

	if (prev->level == state.level) { // already pushed
       prev->AppendLeft(m);

#ifdef _D3D
	   prev->mVersion= mVersion;
#endif
	   prev->Set(state);
	   return(1);
	}																	
	current = (GglTextureMatrixStack*) prev->NewInstance();

#ifdef _D3D
	current->mVersion= mVersion;
#endif

	current->m = m* prev->m;
	state.Push(current);
	return(1);
}



int GvTextureRotor::Do(GTraversal &state)   
{ 
	GTextureMatrixStack *current,*prev;
	prev = (GTextureMatrixStack*) state.Get(GTextureMatrixStack::attrId);
	GTimeStack  *ts = (GTimeStack*) state.Get(GTimeStack::attrId);

	if (!on || !ts || !ts->time.On()) return(0);
	
	float t=ts->time.t;
	
	Matrix m;
	Get(m,t);

	if (prev->level == state.level) { // already pushed
      prev->AppendLeft(m);
	   prev->Set(state);
	   return(1);
	}																	
	current = (GTextureMatrixStack*) prev->NewInstance();
	current->m = m* prev->m;
	state.Push(current);
	return(1);
}

int GvTextureShuttle::Do(GTraversal &state)   
{ 
	GTextureMatrixStack *current,*prev;
	prev = (GTextureMatrixStack*) state.Get(GTextureMatrixStack::attrId);
	if (!prev) return(0);
	GTimeStack  *ts = (GTimeStack*) state.Get(GTimeStack::attrId);

	if (!on || !ts || !ts->time.On()) return(0);
	
	float t=ts->time.t;
	
	Matrix m;
	Get(m,t);

	if (prev->level == state.level) { // already pushed
      prev->AppendLeft(m);
	   prev->Set(state);
	   return(1);
	}																	
	current = (GTextureMatrixStack*) prev->NewInstance();
	current->m = m * prev->m;
	state.Push(current);
	return(1);
}


int GvTimeTransform::Do(GTraversal &state)   
{ 
	GTimeStack *current,*prev;

	prev = (GTimeStack*) state.Get(GTimeStack::attrId);

	if (!prev) return(0);

//	if (!on || !ts || !ts->time.On()) return(0);
	
	float t=prev->time.t;
	// do do: transform dt t0 t1 also ???????????

	t = Transform(state,t);

	if (prev->level == state.level) { // already pushed
	   prev->time.t = t;
	   prev->Set(state);
	   return(1);
	}																	
	current = (GTimeStack*) prev->NewInstance();
	
	current->time = prev->time;
	current->time.t = t;
	state.Push(current);
	return(1);
}


#endif // _G_VRML1


//////////////////////////////////////////////////////////////////////////////
//
// Shapes.
//
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//
// specific nodes.
//
//////////////////////////////////////////////////////////////////////////////

int GvNodeShell::Do(GTraversal &state)
{
  if (!theShell && state.needShapes) 
	  BuildShell(state);
//  if (theShell) return( ((GGeometry *) theShell)->Do(state));
  if (theShell) return( theShell->Traverse(state));
  else  return (/*GvNode*/GNode::Do(state));
  return(0);
}	

// get bounding box if shell built in
GvBool GvNodeShell::getBBox(BBox &bbox)
{
    if (theShell) return(theShell->getBBox(bbox));
	return GvNode::getBBox(bbox);

}
				      

int GvNodeShell::Do(GBBoxTraversal &state)
{
  if (!theShell) BuildShell(state);
  if (theShell) return( theShell->Traverse(state));
  //if (theShell) return(theShell->Do(state));
  else  return (/*GvNode*/ GNode::Do(state));
  return(0);
}					      

int GvNodeShell::Do(GRaySelectTraversal &state)
{
	int ret=0;

	GPUSH_PATH(state,this);


#ifdef _G_VRML1
	// apply shape hints 
	G_GETSTATE(state,shapeHints,GvShapeHints,	GvShapeHintsElement)
	if (shapeHints) {
	   state.SetCulling(shapeHints->CanCull(),shapeHints->vertexOrdering == (int)GvShapeHints::CLOCKWISE );
	}
#endif _G_VRML1

	if (theShell) ret = theShell->Traverse(state);
	else  ret =  (/*GvNode*/ GNode::Do(state));
	
	GPOP_PATH(state);

	return(ret);
}					      


int GvNodeShell::Do(GBuildShellTraversal &state)
{
  if (!theShell) {
     BuildShell(*((GTraversal *) &state));	  //VC  error 2664 without cast 
  }
  if (theShell) 
    // C2664: 'int GShell::Do(class GglRenderTraversal&)' : cannot convert parameter 1 from 'class GBuildShellTraversal' to 'class GglRenderTraversal&'
     return( theShell->Traverse(state));

     //return( ((GGeometry *) theShell)->Do(state));
  else 
    return (/*GvNode*/ GNode::Do(state));
}					      

int 			
GvNodeShell::Do(GglRenderTraversal &state)
{ int ret;

  if (!theShell) {
     BuildShell(state);
  }

  if (theShapes.getNum() >0 ) {		// traverse the expanded shapes
	return theShapes.Traverse(state);
  }

  if (theShell)  {


 	// cullcheck here :
	if (!CullCheck(theShell,state)) 
		return TRAVERSE_CULLED;

	// end cullcheck 

	if (state.appearance) { // if ! NULL node was inside shape 
		// normally DO still overwritten 
		if (!state.appearanceApplied) {
			state.appearance->Apply(state);
		}
		// don't draw fully transparent geometry
		if (state.materialStatus & MATERIAL_FULLY_TRANSPARENT) {
			return TRAVERSE_CULLED;
		}
		// collect transparent shapes ?
		if (((GGvglRenderTraversal *) &state)->IsDelayed())
		{
			GTransparentElement * e= new GTransparentElement(state.appearance,this,theShell->GetBBox(),state.GetMatrixRef());
			((GGvglRenderTraversal *) &state)->AddTransparentElement(e);
			return TRAVERSE_DELAYED;
		}

		int ret = theShell->Traverse(state);

		return(ret);
    } 

#ifdef _G_VRML1
	
	else { // VRML 1.0 
	G_GETSTATE(state,matB,GvMaterialBinding,	GvMaterialBindingElement)
	G_GETSTATE(state,shapeHints,GvShapeHints,	GvShapeHintsElement)
	G_GETSTATE(state,texture2,GvTexture2,	GvTexture2Element)
	
	GvMaterial *material=NULL; 
	GvMaterialElement *materialTop = (GvMaterialElement*) state.Get(GvMaterialElement::attrId); 
	if (materialTop) material = (GvMaterial *) materialTop->data;

	int matStatus = 0;
	int texStatus =  0;

	if (texture2) {  // may change color !!
		if ( (texStatus = texture2->glSet(state))>0) {
		   // force reload color, if texture switch off
	       state.glState->currentBaseColorNode = (GvNode *) -1;
		   if (state.glState->texturing && state.glState->textureParameters.set_diffuse_white) {
 		 	   matStatus |= MATERIAL_SET_DIFFUSE_WHITE;
		   }
		}
	 }
										
	if (material)			   // GvMaterial
		material->Apply(state,matStatus,texStatus);


	 // if normalbinding per face requested switch to flat shading 
	 if (theShell->Flat() && state.glState->faces.visible 
	     && state.glState->faces.lighted && state.glState->faces.smooth) {
	   	  state.glState->faces.smooth = 0;
		  ret= theShell->Traverse(state);
	   	  state.glState->faces.smooth = 1;
	 }
	 else 
       //ret = theShell->Do(state);
       ret = theShell->Traverse(state); // 22.03.96 do full traverse, e.g. for modifiers

#ifdef _OGL	 
	 if (materialTop) { // reset diffuse, because local colors may have changed current color
		Point c;
		// what if textured && white diffuse ??? 
	 	if (materialTop->GetDiffuse(0,c))
	 		glColor(c);
		else 
			state.defaultMaterial->glSetColor();

	 }
#endif
	 return ret;
	} // appearance
#endif
    return ret;

  }
  else return (/*GvNode*/ GNode::Do(state));
}		


// obsolete, replaced by sorted transparency

int 			
GvNodeShell::Do(GglTransparencyRenderTraversal &state)
{

	if (state.appearance) { // if ! NULL node was inside shape 
		if (!state.appearanceApplied) {
			state.appearance->Apply(state);
		}
   }
   switch (state.action) {
   case GglTransparencyRenderTraversal::RENDER_ALL_OBJECTS: 
   		return Do(*((GglRenderTraversal *) &state));
   
   case GglTransparencyRenderTraversal::RENDER_TRANSPARENT_OBJECTS:  
   		{
        int transparent = 0;
		if (state.material && (state.material != (Gv2Material*) -1)) {
			transparent = state.material->HasTransparency() || (state.materialStatus & MATERIAL_HAS_ALPHA);
        } else {
#ifdef _G_VRML1
	  		G_GETSTATE(state,material,GvMaterial,GvMaterialElement);
			if (material)
				transparent = material->HasTransparency();
#endif
		// texture && texture.HasTransparency();
        }
		if (!transparent && state.texture && (state.texture != (GvTexture*) -1))
			transparent = state.texture->HasTransparency();

		if (transparent) {
			TRACE("Transparent %s \n",this->ClassName());
   			return Do(*((GglRenderTraversal *) &state));
		}
		else return(0);
		}
   
   case GglTransparencyRenderTraversal::RENDER_OPAQUE_OBJECTS:  
   		{
        int transparent = 0;
		if (state.material && (state.material != (Gv2Material*)-1)) {
			transparent = state.material->HasTransparency();
        } 
#ifdef _G_VRML1
		else {
	  		G_GETSTATE(state,material,GvMaterial,GvMaterialElement);
			if (material)
				transparent = material->HasTransparency();
        }
#endif

		if (!transparent && state.texture && (state.texture != (GvTexture*) -1))
			transparent = state.texture->HasTransparency();

		if (!transparent) {
   			return Do(*((GglRenderTraversal *) &state)); // draw it now
		}
		else { 
			state.numTransparentElements++;			
			return(0);
		}
		}

  }
  return(0);
}



#ifdef _G_VRML1

int 			
GvIndexedLineSet::Do(GglRenderTraversal &state)
{	int ret=0;
	if (state.glState->faces.visible) {

	  int edgeVis= state.glState->edges.visible;
	  
	  state.glState->faces.visible = 0;
	  
	  state.glState->edges.visible = 1;

	  ret = GvNodeShell::Do(state);

	  state.glState->faces.visible = 1;
	  state.glState->edges.visible = edgeVis;

	} else  ret = GvNodeShell::Do(state);
	return(ret);
}

int 			
GvPointSet::Do(GglRenderTraversal &state)
{	int ret=0;
	if (state.glState->faces.visible) {

	  int markerVis= state.glState->vertices.visible;
	  
	  state.glState->faces.visible = 0;
	  
	  state.glState->vertices.visible = 1;

	  ret = GvNodeShell::Do(state);

	  state.glState->faces.visible = 1;
	  state.glState->vertices.visible = markerVis;

	} else  ret = GvNodeShell::Do(state);
	return(ret);
}






int			
GvIndexedFaceSet::BuildShell(GTraversal &state)
{

	SetShell(NULL);

	G_GETSTATE(state,coord,GvCoordinate3,		GvCoordinate3Element)
	G_GETSTATE(state,materialB,GvMaterialBinding,	GvMaterialBindingElement)
	G_GETSTATE(state,normal,GvNormal,			GvNormalElement)
	G_GETSTATE(state,normalB,GvNormalBinding,	GvNormalBindingElement)
	G_GETSTATE(state,shapeHints,GvShapeHints,	GvShapeHintsElement)
	G_GETSTATE(state,textureCoord,GvTextureCoordinate2, GvTextureCoordinate2Element)

	GvMaterial *material=NULL; 
	GvMaterialElement *materialTop = (GvMaterialElement*) state.Get(GvMaterialElement::attrId); 
	if (materialTop) material = (GvMaterial *) materialTop->data;

	// ignore empty nodes 
	if (coord && coord->point.getNum() == 0) coord = NULL;
	if (normal && normal->vector.getNum() == 0) normal = NULL;
	if (textureCoord && textureCoord->point.getNum() == 0) textureCoord = NULL;

	gbool isFlat = gfalse;
	gbool doCreaseIndex = gfalse;



 	if (coord && !coordIndex.isDefault() && (coordIndex.num > 1)) {
		   int doIndexed = 0; // flag if build index shell 
		   GVertexI maxI;
		   
		   maxI.Zero();

		   int normalBinding = GvNormalBinding::PER_VERTEX_INDEXED;
		   if (normalB) normalBinding = normalB->value.value;
		   if (normalBinding == GvNormalBinding::DEFAULT) normalBinding = GvNormalBinding::PER_VERTEX_INDEXED; // 10.95

		   int materialBinding = GvMaterialBinding::OVERALL;
		   if (materialB) materialBinding = materialB->value.value;
		   
		   // check for explicit default values

		   if (!materialIndex.isDefault() && (materialIndex.num == 1) && (materialIndex.values[0] == -1))
		   {  materialIndex.setDefault(TRUE);}

		   if (!normalIndex.isDefault() && (normalIndex.num == 1) && (normalIndex.values[0] == -1))
		   {  normalIndex.setDefault(TRUE);}

		   if (!textureCoordIndex.isDefault() && (textureCoordIndex.num == 1) && (textureCoordIndex.values[0] == -1))
		   {  textureCoordIndex.setDefault(TRUE);}

			if (shapeHints) {
				isFlat = (normalBinding <= GvNormalBinding::PER_FACE_INDEXED) || ((shapeHints->creaseAngle.value < 0.0001) && normal == NULL);

				// should we compute vertex normal indices based on creaseAngle value ?
				doCreaseIndex = !isFlat && (normal == NULL) && normalIndex.isDefault() && (shapeHints->creaseAngle.value < GvAlwaysSmoothCreaseAngle);

			}

		   IntArray creasedNormalIndex; // computed normal indices

		   // compute vertexNormalIndices based on creaseAngle
		   if (doCreaseIndex) {
			   PointArray fn;
			   int ret=computeNormalIndices(coordIndex.num,coordIndex.values,
					coord->point.num,(const Point*) coord->point.values,
					shapeHints->creaseAngle,!shapeHints->vertexOrdering.value ==  GvShapeHints::CLOCKWISE
					, fn,creasedNormalIndex);

			   if (ret <=0) {
				   doCreaseIndex = gfalse;
				   if (ret == -1) isFlat = gtrue;
			   } else {
				   doIndexed = 1;
				   normalBinding = GvNormalBinding::PER_VERTEX_INDEXED;
			   }
		   }	

	   
		   
		   // check if a GShellI indexed primitive is required
		   if ((normalBinding == GvNormalBinding::PER_VERTEX_INDEXED)  &&
		      ! normalIndex.isDefault())
		   {
		   	  doIndexed = 1;
		   }
		   
		   if ((materialBinding == GvMaterialBinding::PER_VERTEX_INDEXED)  &&
		      ! materialIndex.isDefault())
		   {
		   	 doIndexed = 1;
		   }
		   
		   if ((textureCoord)  && ! textureCoordIndex.isDefault())
		   {
		   	 doIndexed = 1;
		   }

#ifdef _D3D
		   if ( material && ((materialBinding == GvMaterialBinding::PER_FACE) || (materialBinding == GvMaterialBinding::PER_FACE_INDEXED)) )
		   {
		   	 doIndexed = 1;
		   }
#endif


		   const long *vp,*vpStart = coordIndex.values;
		   const long *vpEnd = coordIndex.values+coordIndex.num;
		   vp = vpStart;
		   
		   GFaceList f;
		   f.SetMaxLength(coordIndex.num*2);
		   
		   int fcnt=0;	// how many vertices for this face
		   int fstart=0; // where to put fcnt in f
		   f.Append(0);	// room for fcnt

		   if (doIndexed) {
			  GShellI *theShellI;

			  GVertexI vert;
			  int doNormalIndex = 0;
			  int doTextureIndex = 0;
			  int doMaterialIndex = 0;
			  int maxColors = 0;

		     SetShell(theShellI = new GShellI());

		     theShellI->SetV(coord->point.num,(Point*) coord->point.values);
			  
			  theShellI->verts.SetMaxLength(coord->point.num);

		      if (/*normal &&*/ (normalBinding == GvNormalBinding::PER_VERTEX_INDEXED)  &&
		         ! normalIndex.isDefault())
		      {
		   	      doNormalIndex = 1;
		      }				
		      
		      if (/*textureCoord  &&*/
		         !textureCoordIndex.isDefault())
		      {
		   	      doTextureIndex = 1;
		      }				


		      if (/*materialTop &&*/ (materialBinding == GvMaterialBinding::PER_VERTEX_INDEXED)  &&
		         ! materialIndex.isDefault())
		      {
		   	      doMaterialIndex = 1;
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
			  		vert.v = v;
					vert.n = v;

					if (doCreaseIndex) {
					   // use computed vertex normal index 
					   int ofs = vp-1- vpStart;
				       ofs = creasedNormalIndex[ofs];
					   if (ofs>=0)
						   vert.n = ofs;
					} else 
					if (doNormalIndex) {
					   int ofs = vp-1 - vpStart;
					   if ((ofs>=0) && (ofs <normalIndex.num))
					      vert.n = normalIndex.values[ofs];
					}
					vert.p = v;

					if (doTextureIndex) {
					   int ofs = vp-1 - vpStart;
					   if ((ofs>=0) && (ofs <textureCoordIndex.num))
					      ofs = textureCoordIndex.values[ofs];
						  vert.p = ofs;
					}

					vert.c = v;
					if (doMaterialIndex) {
					   int ofs = vp-1 - vpStart;
					   if ((ofs>=0) && (ofs <materialIndex.num))
					      ofs = materialIndex.values[ofs];
						  vert.c = ofs;
					}
					maxI.v = max(maxI.v,vert.v);
					maxI.n = max(maxI.n,vert.n);
					maxI.c = max(maxI.c,vert.c);
					maxI.p = max(maxI.p,vert.p);

					// to do:Optimize lookup 
	  			    int vi = theShellI->AppendVertex(vert,1);
			  		f.Append(vi);
			  		fcnt++; 
			  	}
		   	   }
		   	   f[fstart]=fcnt; // just for savty , if not -1 terminated
			  // theShellI->EndVertexLookup();

			   theShellI->SetFaceList(f);

			   fcnt = theShellI->GetFaceCount();

  			   if (theShellI->v.Length() < (maxI.v+1)) { 
  			   		Reporter.Error("VRML1 IndexedFaceSet: to few coordinates %d %d \n",theShellI->v.Length() , maxI.c+1);
					theShellI->v.SetLength(maxI.v+1);
				}

				theShellI->vn.SetLength(maxI.n+1);


		   } else {
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

		   	SetShell(new GShell(
		   			coord->point.num,
		   			(Point*) coord->point.values,	// save cast if values = float
		   			f.Length(),f));

			maxI.v=maxI.c=maxI.p=maxI.n=coord->point.num-1;
			
			} // !do indexed
			

			theShell->UpdateActions();

			

			if (normal) {

			switch (normalBinding) { // Normal binding value

			case GvNormalBinding::DEFAULT:
			case GvNormalBinding::NONE:
							break;

			case GvNormalBinding::OVERALL:
					break;
			case GvNormalBinding::PER_PART_INDEXED:
					theShell->SetFlat();

					break;

			case GvNormalBinding::PER_PART:
			case GvNormalBinding::PER_FACE:
					if (theShell->GetFaceCount() <= normal->vector.num) {
#if 0
					    theShell->SetFN(normal->vector.num, (Point *) normal->vector.values,1);
#endif 
					}
					theShell->SetFlat();

					break;

			case GvNormalBinding::PER_FACE_INDEXED:
					theShell->SetFlat();
					// unindex FN
					break;

			case GvNormalBinding::PER_VERTEX:
			    // to do 
				break;
			case GvNormalBinding::PER_VERTEX_INDEXED:
				  if (normal->vector.num < (maxI.n+1)) { TRACE("to few normals %d %d \n",normal->vector.num , maxI.n+1);}
	  		      else theShell->SetVN(normal->vector.num,(Point*) normal->vector.values,1);

				break;
			};
			} // Normal
			else {	 // facetted if normal binding = per face 
			switch (normalBinding) { // Normal binding value

			case GvNormalBinding::PER_PART_INDEXED:
			case GvNormalBinding::PER_PART:
			case GvNormalBinding::PER_FACE:
			case GvNormalBinding::PER_FACE_INDEXED:
					theShell->SetFlat();
					break;
			default : break;

			};


			}

			if (materialTop) {
			switch (materialBinding) { // Material binding value

			case GvMaterialBinding::DEFAULT:
			case GvMaterialBinding::NONE:
			case GvMaterialBinding::OVERALL:
					break;
			case GvMaterialBinding::PER_PART_INDEXED:
			case GvMaterialBinding::PER_FACE_INDEXED:
					{
					GvMFColor* c=NULL;
					if (materialTop->baseColor) {c = &materialTop->baseColor->rgb; }
					else  
					if (materialTop->data)  {
						GvMaterial *m= (GvMaterial *) materialTop->data;
	  	
	  					c = &m->diffuseColor;
	  					if (m->emissiveColor.num > c->num)
						c=&m->emissiveColor;

					}
					if (!c || (c->num <= 0) ) break;
					 
					 fcnt = theShell->GetFaceCount();
					 PointArray fc(fcnt);
					 const long *vp,*vpStart = materialIndex.values;
		   			 const long *vpEnd = materialIndex.values+materialIndex.num;
					 int f=0;
					 vp=vpStart;
					 while (vp<vpEnd) {
					    int ofs = *vp++;
						if (ofs<0) ofs = 0;
						if (f>=fcnt) break;
						//materialTop->GetDiffuse(ofs,fc[f]);
						c->get1(ofs,fc[f]);
					 	f++;
					 }
   	        		 theShell->SetFC(fc);
					}

					break;

			case GvMaterialBinding::PER_PART:
			case GvMaterialBinding::PER_FACE:
				    {
				    PointArray c(theShell->GetFaceCount());

				    if (materialTop->GetLocalColors(c))
  		        		theShell->SetFC(c);

					}
					break;

//			case GvMaterialBinding::PER_VERTEX:
			case GvMaterialBinding::PER_VERTEX_INDEXED:
				    {
				    PointArray c(maxI.c+1);
				    if (materialTop->GetLocalColors(c))
  		        		theShell->SetVC(c);
					}
				break;
			};
			} // Material
			
			if (textureCoord)
		    {
				  PointArray vp;
				  textureCoord->point.get(vp);
  				  if (vp.Length() < (maxI.p+1)) { 
  				  	TRACE("to few parameters #params %d max index %d %d \n",
  				  		vp.Length() , maxI.p+1,textureCoordIndex.isDefault());
				  	PointArray vpNew;
				    while (vpNew.Length() <  (maxI.p+1)) {
						vpNew.Append(vp);
					}
					theShell->SetVP(vpNew);

  				  }
	  		      else theShell->SetVP(vp);
		    } else {		// compute a default mapping 
 			    PointArray vp(maxI.p+1);
				int indexed=0;

				if (RTISA(theShell, GShellI)) {
		         if (!textureCoordIndex.isDefault()) indexed = 1;
				}
				if (indexed) {
				  GShellI *s = (GShellI *) theShell;	
				  TRACE("Compute default parameters for index texture cords not implemented .\n");
				  s->UpdateActions();  // to do, GetBBox should update 
				  ComputeBoxParams(s->GetBBox(),s->verts,s->v, vp);
				  s->SetVP(vp);
				} else {
				  theShell->UpdateActions();  // to do, GetBBox should update 
				  ComputeBoxParams(theShell->GetBBox(),theShell->v, vp);
				  theShell->SetVP(vp);
				}
		    }				


 if (shapeHints && (shapeHints->faceType == (int)GvShapeHints::UNKNOWN_FACE_TYPE)) {
 	TRACE("Triangulating IndexedFaceSet\n");
 	theShell->Triangulate();
 }

 
 // heuristic, detect a one coordinate3 many IndexFaceSets with few faces only case, compress
 // alternative , implement a GShell by Reference 
 if (RTISA(theShell, GShellI)) {
  	GShellI *shellI = (GShellI *) theShell;

#ifdef _D3D
		if (shellI->fc.Length() > 0) {
			// TO DO : VRML 1 material 
			gbool flip = shapeHints && (shapeHints->vertexOrdering == (int)GvShapeHints::CLOCKWISE);
			gbool solid = shapeHints && (shapeHints->shapeType == (int)GvShapeHints::SOLID);
			theShapes.flush();
			ExpandFaceColors(shellI, state.material,state.texture,state.textureTransform,solid,flip,RTISA(shellI,GPolylineI),theShapes);
		}
		else 
#endif

	if (shellI->f.Length() < shellI->v.Length()) { 
		TRACE("Compressing shell : facelist length = %d, verts %d v %d n %d p%d c%d \n",
				shellI->f.Length(),shellI->verts.Length(),shellI->v.Length(),shellI->vn.Length(),shellI->vp.Length(),shellI->vc.Length());
		GShellI *newS = NULL;
		newS = CompressShell(shellI,NULL,1,0);
		SetShell(newS);

	}
 }

 
};


 return(1);
}

int			
GvIndexedLineSet::BuildShell(GTraversal &state)
{
	SetShell(NULL);

	G_GETSTATE(state,coord,GvCoordinate3,		GvCoordinate3Element)
	G_GETSTATE(state,materialB,GvMaterialBinding,	GvMaterialBindingElement)
	G_GETSTATE(state,normal,GvNormal,			GvNormalElement)
	G_GETSTATE(state,normalB,GvNormalBinding,	GvNormalBindingElement)
	G_GETSTATE(state,shapeHints,GvShapeHints,	GvShapeHintsElement)
	G_GETSTATE(state,textureCoord,GvTextureCoordinate2, GvTextureCoordinate2Element)

	GvMaterial *material=NULL; 
	GvMaterialElement *materialTop = (GvMaterialElement*) state.Get(GvMaterialElement::attrId); 
	if (materialTop) material = (GvMaterial *) materialTop->data;;

 	if (coord && !coordIndex.isDefault()) {
		   int doIndexed = 0; // flag if build index shell 

   		   int normalBinding = GvNormalBinding::PER_VERTEX_INDEXED;
		   if (normalB) normalBinding = normalB->value.value;
		   if (normalBinding == GvNormalBinding::DEFAULT) normalBinding = GvNormalBinding::PER_VERTEX_INDEXED; // 10.95


		   int materialBinding = GvMaterialBinding::OVERALL;
		   if (materialB) materialBinding = materialB->value.value;

		   
		   if ((normalBinding == GvNormalBinding::PER_VERTEX_INDEXED)  &&
		      ! normalIndex.isDefault())
		   {
		   	 doIndexed = 1;
		   }
		   
		   if ((materialBinding == GvMaterialBinding::PER_VERTEX_INDEXED)  &&
		      ! materialIndex.isDefault())
		   {
		   	 doIndexed = 1;
		   }
		   
		   if ((textureCoord)  && ! textureCoordIndex.isDefault())
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

		   if (doIndexed) {
			  GShellI *theShellI;

			  GVertexI vert;
			  int doNormalIndex = 0;
			  int doTextureIndex = 0;
			  int doMaterialIndex = 0;

		      SetShell(theShellI = new GShellI());
		      theShellI->SetV(coord->point.num,(Point*) coord->point.values);
			  
			  theShellI->verts.SetMaxLength(coord->point.num);

		      if (normal && (normalBinding == GvNormalBinding::PER_VERTEX_INDEXED)  &&
		         ! normalIndex.isDefault())
		      {
		   	      doNormalIndex = 1;
		      }				
		      
		      if (textureCoord  &&
		         !textureCoordIndex.isDefault())
		      {
		   	      doTextureIndex = 1;
		      }				


		      if (materialTop && (materialBinding == GvMaterialBinding::PER_VERTEX_INDEXED)  &&
		         ! materialIndex.isDefault())
		      {
		   	      doMaterialIndex = 1;
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
			  		vert.v = v;
					vert.n = v;
					if (doNormalIndex) {
					   int ofs = vp-1 - vpStart;
					   if ((ofs>=0) && (ofs <normalIndex.num))
					      vert.n = normalIndex.values[ofs];
					}
					vert.p = v;
					if (doTextureIndex) {
					   int ofs = vp-1 - vpStart;
					   if ((ofs>=0) && (ofs <textureCoordIndex.num))
					      ofs = textureCoordIndex.values[ofs];
						  vert.p = ofs;
					}

					vert.c = v;
					if (doMaterialIndex) {
					   int ofs = vp-1 - vpStart;
					   if ((ofs>=0) && (ofs <materialIndex.num))
					      ofs = materialIndex.values[ofs];
						  vert.c = ofs;
					}

					// to do:Optimize lookup 
	  			    int vi = theShellI->AppendVertex(vert,1);
			  		f.Append(vi);
			  		fcnt++; 
			  	}
		   	   }
		   	   f[fstart]=fcnt; // just for savty , if not -1 terminated
			  // theShellI->EndVertexLookup();

			   theShellI->SetFaceList(f);
			   fcnt = theShellI->GetFaceCount();

		   } else {
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

		   SetShell(new GPolyline(
		   			coord->point.num,
		   			(Point*) coord->point.values,	// save cast if values = float
		   			f.Length(),f));

			} // !do indexed

			theShell->UpdateActions();

			if (normal) {

			switch (normalBinding) { // Normal binding value

			case GvNormalBinding::DEFAULT:
			case GvNormalBinding::NONE:
							break;

			case GvNormalBinding::OVERALL:
					break;
			case GvNormalBinding::PER_PART_INDEXED:

					break;

			case GvNormalBinding::PER_PART:
			case GvNormalBinding::PER_FACE:
					if (theShell->GetFaceCount() <= normal->vector.num)
					    theShell->SetFN(normal->vector.num, (Point *) normal->vector.values,1);
					theShell->SetFlat();

					break;

			case GvNormalBinding::PER_FACE_INDEXED:
					theShell->SetFlat();
					// unindex FN
					break;

			case GvNormalBinding::PER_VERTEX:
			case GvNormalBinding::PER_VERTEX_INDEXED:
	  		      theShell->SetVN(normal->vector.num,(Point*) normal->vector.values,1);

				break;
			};
			} // Normal

			if (materialTop) {
  		        GvMFColor * rgb = materialTop->GetDiffuse();
			
			if (rgb)
			switch (materialBinding) { // Material binding value

			case GvMaterialBinding::DEFAULT:
			case GvMaterialBinding::NONE:
			case GvMaterialBinding::OVERALL:
					break;
			case GvMaterialBinding::PER_PART_INDEXED:
			case GvMaterialBinding::PER_FACE_INDEXED:
					{
					 fcnt = theShell->GetFaceCount();
					 PointArray fc(fcnt);
					 const long *vp,*vpStart = materialIndex.values;
		   			 const long *vpEnd = materialIndex.values+materialIndex.num;
					 int f=0;
					 vp=vpStart;
					 while (vp<vpEnd) {
					    int ofs = *vp++;
						if (ofs<0) ofs = 0;
						if (f>=fcnt) break;
						rgb->get1(ofs,fc[f]);
					 	f++;
					 }
   	        		 theShell->SetFC(fc);

					}

					break;

			case GvMaterialBinding::PER_PART:
			case GvMaterialBinding::PER_FACE:
					if (theShell->GetFaceCount() <= rgb->num)
  		        		theShell->SetFC(rgb->num,(Point*) rgb->values);

					break;

//			case GvMaterialBinding::PER_VERTEX:
			case GvMaterialBinding::PER_VERTEX_INDEXED:
  		        	theShell->SetVC(rgb->num,(Point*) rgb->values);
				break;
			};
			} // Material
			
			if (textureCoord)
		    {
				  PointArray vp;
				  textureCoord->point.get(vp);
	  		      theShell->SetVP(vp);
		    }				
    };

 return(1);
}

int			
GvIndexedTriangleStripSet::BuildShell(GTraversal &state)
{
//	TRACE("GvIndexedTriangleStripSet::BuildShell not implemented \n");

	SetShell(NULL);

	G_GETSTATE(state,coord,GvCoordinate3,		GvCoordinate3Element)
	G_GETSTATE(state,materialB,GvMaterialBinding,	GvMaterialBindingElement)
	G_GETSTATE(state,normal,GvNormal,			GvNormalElement)
	G_GETSTATE(state,normalB,GvNormalBinding,	GvNormalBindingElement)
	G_GETSTATE(state,shapeHints,GvShapeHints,	GvShapeHintsElement)
	G_GETSTATE(state,textureCoord,GvTextureCoordinate2, GvTextureCoordinate2Element)

	GvMaterial *material=NULL; 
	GvMaterialElement *materialTop = (GvMaterialElement*) state.Get(GvMaterialElement::attrId); 
	if (materialTop) material = (GvMaterial *) materialTop->data;;

 	if (coord && !coordIndex.isDefault()) {
		   int doIndexed = 0; // flag if build index shell 

		   int normalBinding = GvNormalBinding::PER_VERTEX_INDEXED;
		   if (normalB) normalBinding = normalB->value.value;
		   if (normalBinding == GvNormalBinding::DEFAULT) normalBinding = GvNormalBinding::PER_VERTEX_INDEXED; // 10.95

		   int materialBinding = GvMaterialBinding::OVERALL;
		   if (materialB) materialBinding = materialB->value.value;

		   
		   if ((normalBinding == GvNormalBinding::PER_VERTEX_INDEXED)  &&
		      ! normalIndex.isDefault())
		   {
		   	 doIndexed = 1;
		   }
		   
		   if ((materialBinding == GvMaterialBinding::PER_VERTEX_INDEXED)  &&
		      ! materialIndex.isDefault())
		   {
		   	 doIndexed = 1;
		   }
		   
		   if ((textureCoord)  && ! textureCoordIndex.isDefault())
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

		   int vcnt=0;	// for unpacking tristrip to tris
 	   	   int mode = 0;
	       int vi[3];


		   if (doIndexed) {
			  GShellI *theShellI;

			  GVertexI vert;
			  int doNormalIndex = 0;
			  int doTextureIndex = 0;
			  int doMaterialIndex = 0;

		     SetShell(theShellI = new GShellI());
		     
           theShellI->SetV(coord->point.num,(Point*) coord->point.values);
			  theShellI->verts.SetMaxLength(coord->point.num);

		      if (normal && (normalBinding == GvNormalBinding::PER_VERTEX_INDEXED)  &&
		         ! normalIndex.isDefault())
		      {
		   	      doNormalIndex = 1;
		      }				
		      
		      if (textureCoord  &&
		         !textureCoordIndex.isDefault())
		      {
		   	      doTextureIndex = 1;
		      }				


		      if (materialTop && (materialBinding == GvMaterialBinding::PER_VERTEX_INDEXED)  &&
		         ! materialIndex.isDefault())
		      {
		   	      doMaterialIndex = 1;
		      }				

			  // theShellI->BeginVertexLookup();
			  	
		      
		   	  while (vp<vpEnd) {
		      	long v= *vp++;
			  	if (v <0) {
					if (vp >= vpEnd) break;	// last -1 
					vcnt = 0;
					mode = 0;
			  	} 
			  	else { 
			  		vert.v = v;
					vert.n = v;
					if (doNormalIndex) {
					   int ofs = vp-1 - vpStart;
					   if ((ofs>=0) && (ofs <normalIndex.num))
					      vert.n = normalIndex.values[ofs];
					}
					vert.p = v;
					if (doTextureIndex) {
					   int ofs = vp-1 - vpStart;
					   if ((ofs>=0) && (ofs <textureCoordIndex.num))
					      ofs = textureCoordIndex.values[ofs];
						  vert.p = ofs;
					}

					vert.c = v;
					if (doMaterialIndex) {
					   int ofs = vp-1 - vpStart;
					   if ((ofs>=0) && (ofs <materialIndex.num))
					      ofs = materialIndex.values[ofs];
						  vert.c = ofs;
					}

					// to do:Optimize lookup 
	  			    int vertexi = theShellI->AppendVertex(vert,1);
					{
					vi[vcnt]= vertexi;
					vcnt ++;
					if (vcnt >=3) {
			  		if (!mode ) {  
			  			f.AppendTri(vi[0],vi[1],vi[2]);
					 } else {
			  			f.AppendTri(vi[1],vi[0],vi[2]);
					 }
					 mode = !mode; vi[0]=vi[1]; vi[1]=vi[2]; vcnt = 2;
					}

					}
			  		fcnt++; 
			  	}
		   	   }
			  // theShellI->EndVertexLookup();

			   theShellI->SetFaceList(f);
			   fcnt = theShellI->GetFaceCount();

		   } else {
		   while (vp<vpEnd) {
		      long v= *vp++;
			  if (v <0) {
				if (vp >= vpEnd) break;	// last -1 
				vcnt = 0;
				mode = 0;
			  } 
			  else { 
				vi[vcnt]= v;
				vcnt ++;
				if (vcnt >=3) {
			  		if (!mode ) {  
			  			f.AppendTri(vi[0],vi[1],vi[2]);
					 } else {
			  			f.AppendTri(vi[1],vi[0],vi[2]);
					 }
					 mode = !mode; vi[0]=vi[1]; vi[1]=vi[2]; vcnt = 2;
				}

			  }
		   }

		   SetShell(new GShell(
		   			coord->point.num,
		   			(Point*) coord->point.values,	// save cast if values = float
		   			f.Length(),f));

			} // !do indexed

			theShell->UpdateActions();

			if (normal) {

			switch (normalBinding) { // Normal binding value

			case GvNormalBinding::DEFAULT:
			case GvNormalBinding::NONE:
							break;

			case GvNormalBinding::OVERALL:
					break;
			case GvNormalBinding::PER_PART_INDEXED:
					theShell->SetFlat();

					break;

			case GvNormalBinding::PER_PART:
			case GvNormalBinding::PER_FACE:
					if (theShell->GetFaceCount() <= normal->vector.num)
					    theShell->SetFN(normal->vector.num, (Point *) normal->vector.values,1);
					theShell->SetFlat();

					break;

			case GvNormalBinding::PER_FACE_INDEXED:
					theShell->SetFlat();
					// unindex FN
					break;

			case GvNormalBinding::PER_VERTEX:
			case GvNormalBinding::PER_VERTEX_INDEXED:
	  		      theShell->SetVN(normal->vector.num,(Point*) normal->vector.values,1);

				break;
			};
			} // Normal

			if (materialTop) {
  		        GvMFColor * rgb = materialTop->GetDiffuse();
			
			if (rgb)
			switch (materialBinding) { // Material binding value

			case GvMaterialBinding::DEFAULT:
			case GvMaterialBinding::NONE:
			case GvMaterialBinding::OVERALL:
					break;
			case GvMaterialBinding::PER_PART_INDEXED:
			case GvMaterialBinding::PER_FACE_INDEXED:
					{
					 fcnt = theShell->GetFaceCount();
					 PointArray fc(fcnt);
					 const long *vp,*vpStart = materialIndex.values;
		   			 const long *vpEnd = materialIndex.values+materialIndex.num;
					 int f=0;
					 vp=vpStart;
					 while (vp<vpEnd) {
					    int ofs = *vp++;
						if (ofs<0) ofs = 0;
						if (f>=fcnt) break;
						rgb->get1(ofs,fc[f]);
					 	f++;
					 }
   	        		 theShell->SetFC(fc);

					}

					break;

			case GvMaterialBinding::PER_PART:
			case GvMaterialBinding::PER_FACE:
					if (theShell->GetFaceCount() <= rgb->num)
  		        		theShell->SetFC(rgb->num,(Point*) rgb->values);

					break;

//			case GvMaterialBinding::PER_VERTEX:
			case GvMaterialBinding::PER_VERTEX_INDEXED:
  		        	theShell->SetVC(rgb->num,(Point*) rgb->values);
				break;
			};
			} // Material
			
			if (textureCoord)
		    {
				  PointArray vp;
				  textureCoord->point.get(vp);
	  		      theShell->SetVP(vp);
		    }				


    };
 return(1);

}

int			
GvPointSet::BuildShell(GTraversal &state)
{
	SetShell(NULL);

	G_GETSTATE(state,coord,GvCoordinate3,		GvCoordinate3Element)
	if (!coord) return(0);

	G_GETSTATE(state,materialB,GvMaterialBinding,	GvMaterialBindingElement)
	G_GETSTATE(state,normal,GvNormal,			GvNormalElement)
	G_GETSTATE(state,normalB,GvNormalBinding,	GvNormalBindingElement)
	G_GETSTATE(state,shapeHints,GvShapeHints,	GvShapeHintsElement)
	G_GETSTATE(state,textureCoord,GvTextureCoordinate2, GvTextureCoordinate2Element)

	GvMaterial *material=NULL; 
	GvMaterialElement *materialTop = (GvMaterialElement*) state.Get(GvMaterialElement::attrId); 
	if (materialTop) material = (GvMaterial *) materialTop->data;;
 
    int normalBinding = GvNormalBinding::PER_VERTEX;
    if (normalB) normalBinding = normalB->value.value;

    int materialBinding = GvMaterialBinding::OVERALL;
    if (materialB) materialBinding = materialB->value.value;
	

	// to do: check length of coords, startIndex
    if (startIndex >= coord->point.num) return(0);

	GFaceList f;
	int pointLen= numPoints;

	if (pointLen == GV_POINT_SET_USE_REST_OF_POINTS) 
		pointLen = (coord->point.num)-startIndex;
   
    SetShell(new GShell(
		   			pointLen,
		   			((Point*) coord->point.values)+startIndex,	// save cast if values = float
		   			f.Length(),f));


			if (normal) {

			switch (normalBinding) { // Normal binding value

			case GvNormalBinding::DEFAULT:
			case GvNormalBinding::NONE:
			case GvNormalBinding::OVERALL:
					break;

			case GvNormalBinding::PER_PART:
			case GvNormalBinding::PER_PART_INDEXED:
			case GvNormalBinding::PER_FACE:
			case GvNormalBinding::PER_FACE_INDEXED:
			case GvNormalBinding::PER_VERTEX:
			case GvNormalBinding::PER_VERTEX_INDEXED:
	  		      theShell->SetVN(normal->vector.num-startIndex,(Point*) normal->vector.values+startIndex,1);
				break;
			};
			} // Normal


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
			case GvMaterialBinding::PER_FACE:
			case GvMaterialBinding::PER_FACE_INDEXED:
			case GvMaterialBinding::PER_VERTEX:
			case GvMaterialBinding::PER_VERTEX_INDEXED:
  		        	theShell->SetVC(rgb->num-startIndex,(Point*) rgb->values+startIndex);
				break;
			};
			} // Material
			
			if (textureCoord)
		    {
				  PointArray vp;
				  textureCoord->point.get(vp);
	  		      theShell->SetVP(vp.Length()-startIndex,vp.Data() + startIndex);
		    } else {
		    }				

   return(1);
}

int			
GvFaceSet::BuildShell(GTraversal &state)
{
	SetShell(NULL);

	G_GETSTATE(state,coord,GvCoordinate3,		GvCoordinate3Element)
	if (!coord) return(0);

	G_GETSTATE(state,materialB,GvMaterialBinding,	GvMaterialBindingElement)
	G_GETSTATE(state,normal,GvNormal,			GvNormalElement)
	G_GETSTATE(state,normalB,GvNormalBinding,	GvNormalBindingElement)
	G_GETSTATE(state,shapeHints,GvShapeHints,	GvShapeHintsElement)
	G_GETSTATE(state,textureCoord,GvTextureCoordinate2, GvTextureCoordinate2Element)

	GvMaterial *material=NULL; 
	GvMaterialElement *materialTop = (GvMaterialElement*) state.Get(GvMaterialElement::attrId); 
	if (materialTop) material = (GvMaterial *) materialTop->data;;
 
    int normalBinding = GvNormalBinding::PER_VERTEX;
    if (normalB) normalBinding = normalB->value.value;

    int materialBinding = GvMaterialBinding::OVERALL;
    if (materialB) materialBinding = materialB->value.value;
	

	// to do: check length of coords, startIndex
    if (startIndex >= coord->point.num) return(0);

	GFaceList f;
	
	int coordi=0; // startIndex handled later;
	
	for(int face = 0; face<numVertices.Length(); face++) {
		int faceLen = numVertices.values[face];
		if (faceLen == GV_FACE_SET_USE_REST_OF_VERTICES) {
			faceLen =  (coord->point.num-startIndex)-coordi;
		}
		f.Append(faceLen);
		for (int i=0; i<faceLen; i++) {
		  f.Append(coordi);
		  coordi ++;
		}
   }
   
   SetShell(new GShell(
		   			coord->point.num-startIndex,
		   			((Point*) coord->point.values)+startIndex,	// save cast if values = float
		   			f.Length(),f));


			if (normal) {

			switch (normalBinding) { // Normal binding value

			case GvNormalBinding::DEFAULT:
			case GvNormalBinding::NONE:
			case GvNormalBinding::OVERALL:
					break;

			case GvNormalBinding::PER_PART:
			case GvNormalBinding::PER_PART_INDEXED:
			case GvNormalBinding::PER_FACE:
			case GvNormalBinding::PER_FACE_INDEXED:
					if (theShell->GetFaceCount() <= normal->vector.num)
					    theShell->SetFN(normal->vector.num-startIndex, (Point *) normal->vector.values+startIndex,1);
					theShell->SetFlat();

					break;

			case GvNormalBinding::PER_VERTEX:
			case GvNormalBinding::PER_VERTEX_INDEXED:
	  		      theShell->SetVN(normal->vector.num-startIndex,(Point*) normal->vector.values+startIndex,1);
				break;
			};
			} // Normal


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
			case GvMaterialBinding::PER_FACE:
			case GvMaterialBinding::PER_FACE_INDEXED:
					if (theShell->GetFaceCount() <= rgb->num)
  		        		theShell->SetFC(rgb->num,(Point*) rgb->values);

					break;

			case GvMaterialBinding::PER_VERTEX:
			case GvMaterialBinding::PER_VERTEX_INDEXED:
  		        	theShell->SetVC(rgb->num-startIndex,(Point*) rgb->values+startIndex);
				break;
			};
			} // Material
			
			if (textureCoord)
		    {
				  PointArray vp;
				  textureCoord->point.get(vp);
	  		      theShell->SetVP(vp.Length()-startIndex,vp.Data() + startIndex);
		    } else {
		    }				

	  

 return(1);
}

int			
GvTriangleStripSet::BuildShell(GTraversal &state)
{
	SetShell(NULL);

	G_GETSTATE(state,coord,GvCoordinate3,		GvCoordinate3Element)
	if (!coord) return(0);

	G_GETSTATE(state,materialB,GvMaterialBinding,	GvMaterialBindingElement)
	G_GETSTATE(state,normal,GvNormal,			GvNormalElement)
	G_GETSTATE(state,normalB,GvNormalBinding,	GvNormalBindingElement)
	G_GETSTATE(state,shapeHints,GvShapeHints,	GvShapeHintsElement)
	G_GETSTATE(state,textureCoord,GvTextureCoordinate2, GvTextureCoordinate2Element)

	GvMaterial *material=NULL; 
	GvMaterialElement *materialTop = (GvMaterialElement*) state.Get(GvMaterialElement::attrId); 
	if (materialTop) material = (GvMaterial *) materialTop->data;;
 
    int normalBinding = GvNormalBinding::PER_VERTEX;
    if (normalB) normalBinding = normalB->value.value;

    int materialBinding = GvMaterialBinding::OVERALL;
    if (materialB) materialBinding = materialB->value.value;
	

	// to do: check length of coords, startIndex
    if (startIndex >= coord->point.num) return(0);

	GFaceList f;
	
	int coordi=0; // startIndex handled later;
	int coord0=0,coord1=0;

	
	for(int face = 0; face<numVertices.Length(); face++) {
		int faceLen = numVertices.values[face];
		
		if (faceLen == GV_TRI_STRIP_SET_USE_REST_OF_VERTICES) {
			faceLen  =  (coord->point.num-startIndex)-coordi;
		}
		for (int i=0; i<faceLen; i++) {
		  if (i>=2 )  {
		    if (i && 1) f.AppendTri(coord1,coord0,coordi);
		  	else f.AppendTri(coord0,coord1,coordi); 
		  }
		  coord0=coord1;
		  coord1 = coordi;
		  coordi ++;
		}
   }
   
   SetShell(new GShell(
		   			coord->point.num-startIndex,
		   			((Point*) coord->point.values)+startIndex,	// save cast if values = float
		   			f.Length(),f));


			if (normal) {

			switch (normalBinding) { // Normal binding value

			case GvNormalBinding::DEFAULT:
			case GvNormalBinding::NONE:
			case GvNormalBinding::OVERALL:
					break;

			case GvNormalBinding::PER_PART:
			case GvNormalBinding::PER_PART_INDEXED:
			case GvNormalBinding::PER_FACE:
			case GvNormalBinding::PER_FACE_INDEXED:
					if (theShell->GetFaceCount() <= normal->vector.num)
					    theShell->SetFN(normal->vector.num-startIndex, (Point *) normal->vector.values+startIndex,1);
					theShell->SetFlat();

					break;

			case GvNormalBinding::PER_VERTEX:
			case GvNormalBinding::PER_VERTEX_INDEXED:
	  		      theShell->SetVN(normal->vector.num-startIndex,(Point*) normal->vector.values+startIndex,1);
				break;
			};
			} // Normal


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
			case GvMaterialBinding::PER_FACE:
			case GvMaterialBinding::PER_FACE_INDEXED:
					if (theShell->GetFaceCount() <= rgb->num)
  		        		theShell->SetFC(rgb->num,(Point*) rgb->values);

					break;

			case GvMaterialBinding::PER_VERTEX:
			case GvMaterialBinding::PER_VERTEX_INDEXED:
  		        	theShell->SetVC(rgb->num-startIndex,(Point*) rgb->values+startIndex);
				break;
			};
			} // Material
			
			if (textureCoord)
		    {
				  PointArray vp;
				  textureCoord->point.get(vp);
	  		      theShell->SetVP(vp.Length()-startIndex,vp.Data() + startIndex);
		    } else {
		    }				

 return(1);
}


int			
GvQuadMesh::BuildShell(GTraversal &state)
{
   SetShell(NULL); 

   G_GETSTATE(state,coord,GvCoordinate3,		GvCoordinate3Element)
	if (!coord) return(0);

	G_GETSTATE(state,materialB,GvMaterialBinding,	GvMaterialBindingElement)
	G_GETSTATE(state,normal,GvNormal,			GvNormalElement)
	G_GETSTATE(state,normalB,GvNormalBinding,	GvNormalBindingElement)
	G_GETSTATE(state,shapeHints,GvShapeHints,	GvShapeHintsElement)
	G_GETSTATE(state,textureCoord,GvTextureCoordinate2, GvTextureCoordinate2Element)

	GvMaterial *material=NULL; 
	GvMaterialElement *materialTop = (GvMaterialElement*) state.Get(GvMaterialElement::attrId); 
	if (materialTop) material = (GvMaterial *) materialTop->data;;
 
    int normalBinding = GvNormalBinding::PER_VERTEX;
    if (normalB) normalBinding = normalB->value.value;

    int materialBinding = GvMaterialBinding::OVERALL;
    if (materialB) materialBinding = materialB->value.value;
	
//	int ncoords =  coord->point.numValues-startIndex;

// to do: check length of coords, startIndex
	  
		   GMesh *theMesh;
		   SetShell(theMesh = new GMesh(
		   			verticesPerColumn,verticesPerRow,
		   			((Point*) coord->point.values) + startIndex	// save cast if values = float
		   			));


			if (normal) {

			switch (normalBinding) { // Normal binding value

			case GvNormalBinding::DEFAULT:
			case GvNormalBinding::NONE:
							break;

			case GvNormalBinding::OVERALL:
					break;

			case GvNormalBinding::PER_PART:
			case GvNormalBinding::PER_PART_INDEXED:
					theShell->SetFlat();

					// to do: use normals

					break;

			case GvNormalBinding::PER_FACE:
			case GvNormalBinding::PER_FACE_INDEXED:
					if (theShell->GetFaceCount() <= normal->vector.num)
					    theShell->SetFN(normal->vector.num-startIndex, (Point *) normal->vector.values+startIndex,1);
					theShell->SetFlat();

					break;

			case GvNormalBinding::PER_VERTEX:
			case GvNormalBinding::PER_VERTEX_INDEXED:
	  		      theShell->SetVN(normal->vector.num-startIndex,(Point*) normal->vector.values+startIndex,1);
				break;
			};
			} // Normal


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
						PointArray fc((verticesPerColumn-1) * (verticesPerRow-1));
						Point *fcp = fc;
						for (int row= 0; row < (verticesPerRow-1); row ++) {
						    Point partColor;
							rgb->get1(row,partColor);
							for (int col= 0; col < (verticesPerColumn-1); col ++) 
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
  		        	theShell->SetVC(rgb->num-startIndex,(Point*) rgb->values+startIndex);
				break;
			};
			} // Material
			
			if (textureCoord)
		    {
				  PointArray vp;
				  textureCoord->point.get(vp);
	  		      theShell->SetVP(vp.Length()-startIndex,vp.Data() + startIndex);
		    } else {
			   //xxx theMesh->SetVertexParametersNative();
		    }				


   return(1);
}

#endif 

#ifdef _G_VRML1

// to do : check for - values eg. radius <0 ...

int			
GvCone::BuildShell(GTraversal &state)
{
   //to do: Material
   SetShell(NewConeVrml(0,0,0,bottomRadius,height,16,parts)); 

   return(1);
}

// do boundingbox 
int GvCone::Do(GBBoxTraversal &state)
{
  BBox box;

  box.mi.Set(-bottomRadius,-height*0.5,-bottomRadius);
  box.ma.Set(bottomRadius,height*0.5,bottomRadius);
  state.Union(box);
  return(1);
}


int			
GvCube::BuildShell(GTraversal &state)
{
   SetShell(NewBox(-width*0.5,-height*0.5,-depth*0.5,width,height,depth));
   return(1);
}
// do boundingbox 
int GvCube::Do(GBBoxTraversal &state)
{
  BBox box;

  box.mi.Set(-width*0.5,-height*0.5,-depth*0.5);
  box.ma.Set(width*0.5,height*0.5,depth*0.5);
  state.Union(box);
  return(1);
}


int			
GvCylinder::BuildShell(GTraversal &state)
{
   //to do: Material
   SetShell(NewCylinderVrml(0,0,0,radius,height,16,parts)); 
   return(1);
}
// do boundingbox 
int GvCylinder::Do(GBBoxTraversal &state)
{
  BBox box;

  box.mi.Set(-radius,-height*0.5,-radius);
  box.ma.Set(radius,height*0.5,radius);
  state.Union(box);
  return(1);
}



int			
GvSphere::BuildShell(GTraversal &state)
{
   
//   SetShell(new GSphereSubdiv(Point(0,0,0),radius,1,1));
   SetShell(NewSphereVrml(0,0,0,radius,12,6,NULL,NULL));
   return(1);
}

// do boundingbox 
int GvSphere::Do(GBBoxTraversal &state)
{
  BBox box;

  box.mi.Set(-radius,-radius,-radius);
  box.ma.Set(radius,radius,radius);
  state.Union(box);
  return(1);
}

// to do : check for - values eg. radius <0 ...



int			
GvSweep::BuildShell(GTraversal &state)
{

	SetShell(NULL);

	return(0);

}


extern int GShell_GLUTriangulationHoleCode;

/*
from d3d examples 
    if (statglobs.hFont != NULL) {
        DeleteObject(statglobs.hFont);
    }
    statglobs.hFont = CreateFont(
        d3dapp->szClient.cx <= 600 ? 12 : 24,
        0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        VARIABLE_PITCH,
        "Arial" );

    hdc = GetDC(NULL);
    SelectObject(hdc, statglobs.hFont);
    GetTextExtentPoint(hdc, dummyfps, strlen(dummyfps), &statglobs.szFrameRate);
    GetTextExtentPoint(hdc, dummyinfo, strlen(dummyinfo), &statglobs.szInfo);
    ReleaseDC(NULL, hdc);
*/



int			
GvAsciiText::BuildShell(GTraversal &state)
{

	SetShell(NULL);
	G_GETSTATE(state,fontStyle,GvFontStyle,	GvFontStyleElement)

	float dirstepx =1.0;
	float dirstepy = -1.0;
#ifdef WIN32

		// logical font description
   		LOGFONT m_lf;

		m_lf.lfHeight=80;
		m_lf.lfHeight=100;
	    m_lf.lfWidth=0;
	    m_lf.lfEscapement=m_lf.lfOrientation=0;
	    m_lf.lfWeight=FW_NORMAL;
		
		if (fontStyle && fontStyle->style & GvFontStyle::BOLD)  m_lf.lfWeight = 700;
	    
	    m_lf.lfItalic=0;
		if (fontStyle && fontStyle->style & GvFontStyle::ITALIC)  m_lf.lfItalic = 1;
	    
	    m_lf.lfUnderline=0;
		if (fontStyle && fontStyle->style & GvFontStyle::UNDERLINE)  m_lf.lfUnderline = 1;
	    
	    m_lf.lfStrikeOut=0;
		if (fontStyle && fontStyle->style & GvFontStyle::STRIKEOUT)  m_lf.lfStrikeOut = 1;

	    m_lf.lfCharSet=ANSI_CHARSET;
	    m_lf.lfOutPrecision=OUT_TT_ONLY_PRECIS;
	    m_lf.lfClipPrecision=CLIP_DEFAULT_PRECIS;
	    m_lf.lfQuality=PROOF_QUALITY; //DEFAULT_QUALITY;
	    m_lf.lfPitchAndFamily=FF_DONTCARE | VARIABLE_PITCH;	//FIXED_PITCH

		lstrcpy(m_lf.lfFaceName,"Arial");
		
		if (fontStyle) {
			if (fontStyle->family.value == "SANS") 
	    		lstrcpy(m_lf.lfFaceName,"Arial");
			else if (fontStyle->family.value == "SERIF") 
	    		lstrcpy(m_lf.lfFaceName,"Times New Roman");
			else if (fontStyle->family.value == "TYPEWRITER") 
	    		lstrcpy(m_lf.lfFaceName,"Courier New");
			else 
				lstrcpy(m_lf.lfFaceName,(const char *)fontStyle->family.value); // to do unicode
		}

//     SERIF       Serif style (such as TimesRoman)
 //    SANS        Sans Serif Style (such as Helvetica)
 //    TYPEWRITER  Fixed pitch style (such as Courier)

		float height = 1.0;
		if (fontStyle) height = fontStyle->size.value;

		height *= 0.2;

		int m_invert = parts & INVERT;

		int m_extrude = extrusion.Length() > 1;
		int m_parameters = 1;
		
		
		int m_top = parts & TOP;
		int m_bottom= parts & BOTTOM;

		float m_invert_factor = 2.0;

		int align = -1;
		if (justification == (int)LEFT) align = -1;
		else if (justification == (int)CENTER) align = 0;
		else if (justification == (int)RIGHT) align = 1;



		if (!state.view) return(0);

		HFONT font;

		if ((font = ::CreateFontIndirect(&m_lf)) != NULL)
		{
			//BeginWaitCursor();
			//Message("Creating Ascii Text...");

        	//XX  CDC* pDC = state.view->pView->GetDC();
			//XX CFont* oldFont = pDC->SelectObject(&font);
			
			HDC hdc = ::GetDC(NULL);
			HGDIOBJ	oldFont = ::SelectObject(hdc,font);


			GShell *s;
			float x=0.0,y=0.0;

						
			s = new GShell();

			float widthi= width.values[0];
			float maxLength = 0.0;			
        	
			for(int line=0; line <string.num; line ++) {			
			 CString m_text(string[line]);
			 
			 //if (line < width.numValues)  widthi= width.values[line];

				//CSize size = pDC->GetTextExtent(m_text,strlen(m_text));
				//TRACE("Text %s Size %d %d %f %f\n", m_text,(int) size.cx, (int) size.cy,x,y);

			 m_text += "\n";

			 //to do: kernning // center, multiline

			 ComputeGlyphOutline(hdc,m_text,x,y,0.0,height,dirstepx,dirstepy,spacing,align,widthi,m_extrude,*s,maxLength);

			} // for line
			
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
			   extrusion.get1(0, offsets[0]);
			   extrusion.get1(1,offsets[1]);

			   istyle flag = ICLOSE | IHARDTOPBOTTOM | 
			   (m_top ? ITOP:0)| (m_bottom ? IBOTTOM:0) | (m_parameters ? IPARAMETERS:0);

			   GShell* s3d = ComputeExtrudedShell(flag,*s,offsets);
			   
			   delete s;
			   s = s3d;

			}
	   		s->UnSetFlag(SHELL_TRIANGULATED);

			s->SetFlag(SHELL_HAS_CONVEX_FACES);
#ifdef _OGL
			GShell_GLUTriangulationHoleCode=GLU_UNKNOWN;

			s->Triangulate(); // temp

			GShell_GLUTriangulationHoleCode=GLU_INTERIOR;
#else
			s->Triangulate(); // temp

#endif
			
			if (theShell == NULL)
			   SetShell(s);
			else { 
				theShell->Append(*s);
				delete s; s= NULL;
			}
		

			if (oldFont) ::SelectObject(hdc, oldFont);
			::ReleaseDC(NULL,hdc);
		
			::DeleteObject(font);

   }
   return(1);

#else // WIN32
    return(0);
#endif // WIN32			


}

// do boundingbox 
int GvAsciiText::Do(GBBoxTraversal &state)
{
   return GvNodeShell::Do(state);

  //BBox box;

//  box.mi.Set(-bottomRadius,-height*0.5,-bottomRadius);
//  box.ma.Set(bottomRadius,height*0.5,bottomRadius);
//  state.Union(box);
  return(1);
}




//////////////////////////////////////////////////////////////////////////////
//
// WWW-specific nodes.
//
//////////////////////////////////////////////////////////////////////////////


int GvWWWInline:: TraverseChildren(GTraversal &state)
{
  int ret=0;

  if (loader.Ok()) { 
  	state.inlineLevel++;

    // put WWWInline node in state to resolve relative includes
    state.Push();

	GvWWWInlineElement *current,*prev;
	prev = (GvWWWInlineElement*) state.Get(GvWWWInlineElement::attrId);
	if (!prev) {
	} else {
		current = (GvWWWInlineElement *) prev->NewInstance();								
		current->data = this;
		state.Push(GvWWWInlineElement::attrId,current);			
	 }


    for (int i = 0; i < getNumChildren(); i++) {
		if (state.stop) break;
		ret = getChild(i)->Traverse(state);
	}
	state.Pop();

  	state.inlineLevel--;
	return(ret);
  }
  else {

/*
  	state.inlineLevel++;
  		state.Do(this);
  	state.inlineLevel--;
*/
  }
  return(ret);
}





//
// class GvWWWRegionAnchor
//
#if 0
void GvWWWRegionAnchor:: Traverse(GTraversal &state)
{
  if (loaded) { 
//  	GvGroup::Traverse(state);
//	return;
  }

  state.Do(this);
}
#endif

int GvWWWRegionAnchor::Do(GBBoxTraversal &state)
{
  if (getNumChildren()> 0) { 
  	GvGroup::Traverse(state);
	return(1);
  }

  if (bboxIsEmpty()) return(0);
  BBox box;
  
  Point s = bboxSize * 0.5;
  box.mi = bboxCenter-s;
  box.ma = bboxCenter+s;
 
  state.Union(box);
  return(1);
}

int GvWWWRegionAnchor::Do(GTraversal &state)
{

  if (getNumChildren()> 0) { 
  	GvGroup::Traverse(state);
	return(1);
  }

  return(0);
}


int 			
GvWWWRegionAnchor::Do(GglRenderTraversal &state)
{ int ret=0;

  if (getNumChildren()> 0) { 
  	GvGroup::Traverse(state);
	ret = 1;
  }	else 

  if (!bboxIsEmpty()) {
  	Point s = bboxSize * 0.5;
    state.glState->RenderCubeEdges(bboxCenter-s,bboxCenter+s);
	ret = 1;
  }
  {
	Point eye = state.camera->position;
	//pos *= *state.GetMatrix();
	eye *= Invert(*state.GetMatrix());
	BBox box;
  
  	Point s = bboxSize * 0.5;
  	box.mi = bboxCenter-s;
  	box.ma = bboxCenter+s;

	if (box.Inside(eye)) {
       // camera in box, load anchor
       // ????? state.selectedNodes.Append(this);
	}

  }


  return(ret);
}


//////////////////////////////////////////////////////////////////////////////
//
// Default traversal methods. These nodes have no effects during traversal.
//
//////////////////////////////////////////////////////////////////////////////


int GvUnknownNode::Do(GTraversal &state)
{
   if (isA) {  isA->Traverse(state); return(1); }
   return(0);
}


int GvInfo::Do(GTraversal &state)
{
	return(0);
}

int GvCallback::Do(GTraversal &state)
{
	return(0);
}

int GvLabel::Do(GTraversal &state)
{
	return(0);
}



//////////////////////////////////////////////////////////////////////////////

// GvMorph

#if 0
// set GMorph.cpp
void GvMorph:: Traverse(GTraversal &state)
{

    for (int i = 0; i < getNumChildren(); i++)
		getChild(i)->Traverse(state);
}


int 			
GvMorph::Traverse(GglRenderTraversal &state)
{
  GGvSceneInfoTraversal info;
  	
  for (int i = 0; i < getNumChildren(); i++) {
		GVNode * child = getChild(i);
		
		child->Traverse(state);
  }

  return(1);
}

#endif

#undef ANNOUNCE
#undef DO_SHAPE

#endif _G_VRML1



// get bounding box of node 
GvBool GvNode::getBBox(BBox &bbox)
{

	  GGvBBoxTraversal traversal;
      traversal.InitializeAttributes();
      //traversal.Set(new GTimeStack(time)); // set current time
	  // traversal.SetCamera(view->globalCamera);
	  // traversal.SetPhysicalCamera(view->GetPhysicalCamera());
	  
	  traversal.useBBoxes = FALSE; // default
	  traversal.doViewCulling = FALSE; // default

	  traversal.traverseAll = gtrue;
	  traversal.traverseTransformHierarchyOnly=gtrue;

	  int ret = Traverse(traversal);
      bbox = traversal.bbox;
	  
	  return(!bbox.IsEmpty());
}

// return bbox
GvBool GvNode::getBBoxEx(BBox &bbox)
{
	return getBBox(bbox);
}


// get matrix of node , FALSE if node has no matrix 
GvBool GvNode::getMatrix(Matrix &m)
{
	return(FALSE);
}


// get bounding box of MFNode
GvBool GvMFNode::getBBox(BBox &bbox)
{
	  GGvBBoxTraversal traversal;
      traversal.InitializeAttributes();
      //traversal.Set(new GTimeStack(time)); // set current time
	  //traversal.SetCamera(view->globalCamera);
	  //traversal.SetPhysicalCamera(view->GetPhysicalCamera());

	  int ret = Traverse(traversal);
      bbox = traversal.bbox;
	  return(!bbox.IsEmpty());
}


// get bounding box of SFNode
GvBool GvSFNode::getBBox(BBox &bbox)
{

	  GvNode *n= value;
	  if (n != NULL) {
		  return n->getBBox(bbox);
	  }
	  else {
		  return FALSE;
	  }
 
}

