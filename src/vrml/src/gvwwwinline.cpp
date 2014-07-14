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
#include <gvsfstring.h>
#include <gvwwwinline.h>
#include <gvbsp.h>

#include <gvavatar.h>

#include "www.h"

#include "grender.h"
#include "gmodel.h"
#include "greadgv.h"

#include "gvelement.h"
#include "gvtraverse.h"

#include "gvmediathread.h"

#include "greporter.h"


#ifdef _G_VRML1

//
// GvWWWInline
//

GV_NODE_SOURCE_IMP(GvWWWInline,GvGroup);

GvWWWInline::GvWWWInline() : 
isLoaded(FALSE)
{
    GV_NODE_CONSTRUCTOR(GvWWWInline);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(name);
    GV_NODE_ADD_FIELD(bboxSize);
    GV_NODE_ADD_FIELD(bboxCenter);

    GV_NODE_ADD_EVENT_OUT(isLoaded);


    name.value = "";
    bboxSize.value[0] = bboxSize.value[1] = bboxSize.value[2] = 0.0;
    bboxCenter.value[0] = bboxCenter.value[1] = bboxCenter.value[2] = 0.0;
/*
	loaded = 0;
	cannotLoad = 0;
	loader = NULL;
*/
}

GvWWWInline::~GvWWWInline()
{
/*
   if (loader) {
      //delete loader;
	  loader->unref();
	  loader = NULL;
   }
*/
}

// a field has been changed
int GvWWWInline::OnFieldChanged(GvField *field)
{
	if (field == &name) {
		TRACE("GvWWWInline::OnFieldChanged(&name) to %s \n",(const char *)name);
		deleteChildren();
		loader.SetNotLoaded();

		return(1);

	}
	else return(OnChanged());
}

// something  has been changed
int GvWWWInline::OnChanged()
{
	return(GvGroup::OnChanged());
}


// get absolute home url
const char *GvWWWInline::GetHomeUrl() const
{
	// has GetHome just been setup ?????
	return(loader.GetHomeUrl());
}


int 			
GvWWWInline::Load(GTraversal &state)
{
	// whats the current status
	GFileState status = loader.GetState();

Redo:
	switch(status) {
	case S_BAD_DATA:
	case S_BAD_URL:
		status = loader.state = S_ALL_BAD; // can´t load any URL 
		isLoaded.set(FALSE);
	    break;
	case S_NOT_LOADED: // starting state : NOT loaded 
	     if (name.value.getLength() == 0) { 
			 loader.SetNoUrl();		
			 isLoaded.set(FALSE);
			 return(0);
		 }
		 
		 {
		 CTime refreshTime(0);
		 const char *refedFromUrl= state.GetUrl(refreshTime);	 

		 // start processing 
		 status = loader.Load(state.GetHomeUrl(),name.value.getString(),
			 refedFromUrl,"WWWInline",state.inlineWwwFlags,state.view->hWnd,loader.urlI,refreshTime,state.view->GetReporter(),state.view->file);

		 }

 		 // set url parameters
		 //xx worldUrl.set(loader.GetUrl());
		 //xx baseUrl.set(loader.GetHomeUrl());

		 if (status != S_NOT_LOADED) goto Redo;

		 break;
	case S_LOADED: {
				int ret;
				GvNode *root = NULL;

		 		if (state.view) {
					state.view->lastFrameLoadCnt++;
		 		    state.view->lastFrameInlineLoadCnt++;
				}

				deleteChildren();

				// xx worldUrl.set(loader.GetUrl());
				// xx baseUrl.set(loader.GetHomeUrl());

				ret = ReadFile(loader.GetLocalFile(),state.view->GetReporter(),root);

				if (ret>=0) {  // fine, we could read it 
					loader.SetProcessed();
					addChild(root);
					
					// xxchildren.OnChanged(); // so could advise on children_changed 

					isLoaded.set(TRUE);
				}
				else { 
					loader.SetBadData(); // bad, try next
					if (state.view) state.view->lastFrameLoadErrorCnt++;
					status = loader.GetState();
					if (status != S_LOADED) {
						deleteChildren(); // delete all contents 
						goto Redo; // new 15.06.98
					}

				}

				return(1);
			   }
	case S_PROCESSED:
				return(1);
	default :
		break;

	}
	return(0);
}

int GvWWWInline::Do(GTraversal &state)
{
  if (!loader.Ok()) {
	if ((state.inlineLevel <= state.maxInlineLevel)
  		&& state.loadInlines >= GTraversal::LOAD_IF_NEEDED)
		Load(state);
  }	
  
  else 

  if (loader.Ok()) { 
		//  pass back to WWWInline::TraverseChildren for proper stack handling
		return(GvWWWInline::TraverseChildren(state));
  }

  return(0);
}

int GvWWWInline::Do(GBBoxTraversal &state)
{
  if (!bboxIsEmpty() && (state.useBBoxes || !loader.Ok())) {  // use bbox 
		BBox box;
		Point s = bboxSize * 0.5;
		box.mi = bboxCenter-s;
		box.ma = bboxCenter+s;
 
		state.Union(box);
		return(1);
  }	


  if (loader.Ok()) {
  		return(GvWWWInline::TraverseChildren(state));
  }
  else 	return(0);

}
  

int GvWWWInline::Do(GLoadInlinesTraversal &state)
{
/*
  if (state.resolveRelativeUrls) {
		homeUrl = state.GetHomeUrl();
		homeUrlInit = TRUE;
  }
*/
  if (!loader.Ok()) {
	if ((state.inlineLevel <= state.maxInlineLevel)
  		&& state.loadInlines >= GTraversal::LOAD_IF_NEEDED)
		Load(state);
  }	
  
  else 

  if (loader.Ok()) { 
		//  pass back to WWWInline::TraverseChildren for proper stack handling
		return(GvWWWInline::TraverseChildren(state));
  }

  return(0);
}

int 			
GvWWWInline::Do(GglRenderTraversal &state)
{


  if (!loader.Ok()) { // we need to check the Loader state 
	if ((state.inlineLevel <= state.maxInlineLevel)
  		&& state.loadInlines >= GTraversal::LOAD_IF_NEEDED)
		Load(state);
  }	

  if (loader.Ok()) { 
		// pass back to WWWInline::TraverseChildren for proper stack handling
  		return(GvWWWInline::TraverseChildren(state));
  }


  if (bboxIsEmpty()) return(-1);
  // to do state: if(renderInlineBox)
  Point s = bboxSize * 0.5;
  state.glState->RenderCubeEdges(bboxCenter-s,bboxCenter+s);

  return(-2);
}

#endif



//
// Gv2Inline
//

GV_NODE_SOURCE_IMP(Gv2Inline,GvScene);

Gv2Inline::Gv2Inline() : url(0),isLoaded(FALSE),lastRenderTime(0),parser(NULL),isRegistered(FALSE)
{
    GV_NODE_CONSTRUCTOR(Gv2Inline);
    isBuiltIn = TRUE;

	// from Gv2group
	GV_NODE_ADD_FIELD(bboxCenter);
	GV_NODE_ADD_FIELD(bboxSize);

	GV_NODE_ADD_FIELD(url);

    // normally hidden
    //from Gv2group
	GV_NODE_ADD_FIELD(children);
    GV_NODE_ADD_EVENT_IN(addChildren,GvMFNode);
    GV_NODE_ADD_EVENT_IN(removeChildren,GvMFNode);

	// extra
    GV_NODE_ADD_EVENT_IN(set_unload,GvSFBool);
    GV_NODE_ADD_EVENT_OUT(isLoaded);

}

Gv2Inline::~Gv2Inline()
{
#ifdef WIN32
	if (parser) {
		parser->KillThread();
		parser = NULL;
	}
#endif

	const char *s="";
	if (url.getNum()>0)
		s=url.get1(0).getString();

	// HG DEBUG
	TRACE("Inline %p %s is now destructed \n",this,s);

    //GView *view = (GView*) getBrowser();	
	//ASSERT(view != NULL);
	//view->GetReporter()->Warning("Inline %p %s is now destructed \n",this,s);



}


int Gv2Inline::Unload()
{	int ret=0;

#ifdef WIN32
	if (parser) {
		parser->KillThread();
		parser = NULL;
	}
#endif

	loader.SetNotLoaded();
	if (children.getNum()>0) {
		GvScene::deleteAll();
		children.OnChanged(); // notify observers / ROUTES
		isLoaded.set(FALSE);
		GView *view = (GView*) getBrowser();

		ASSERT(view != NULL);

		if (view)
			view->doUpdateSceneInfo = 2;
		ret=1;

	}
	return ret;

}


void Gv2Inline::set_unload(GvSFBool *mode)
{
    TRACE("%s::set_unload \n",ClassName());

	if (! mode->get()) return;

	Unload();

	
}

// a field has been changed
int Gv2Inline::OnFieldChanged(GvField *field)
{
	if (field == &url) {
		TRACE("Gv2Inline::OnFieldChanged(&url)\n");
		Unload();
		//return(1);
	} else 	if (field == &isLoaded) {
		return(0);
    } 

	return(Gv2Group::OnFieldChanged(field));
}


// get absolute home url
const char *Gv2Inline::GetHomeUrl() const
{
	// has GetHome just been setup ?????
	return(loader.GetHomeUrl());
}


// get the caching check time for children 
CTime Gv2Inline::GetChildrenRefreshTime() const
{
	return(loader.checkTime);
}

const char *Gv2Inline::GetUrl() const
{
	return(GvScene::GetUrl());
}


// traverse and instace proto's trigger extern proto load 
// prepare scene graph, instance protos load externprotos

int Gv2Inline::PrepareChildren(GView *view)
{
	// for resolving relative urls 
	GLoadInlinesTraversal traversal;
	traversal.view = view;			
	traversal.resolveRelativeUrls = gtrue;	  

			traversal.traversalOrder = view->traversalInlineLoadingOrder;

			traversal.doViewCulling= FALSE;
			traversal.loadProtos = GTraversal::LOAD_ALL;
			traversal.loadInlines = GTraversal::DONT_LOAD;
			traversal.loadSounds = GTraversal::DONT_LOAD;
			traversal.loadTextures = GTraversal::DONT_LOAD;
  
			traversal.maxInlineLevel = view->maxInlineDepth;
			traversal.InitializeAttributes();

			traversal.SetCamera(view->globalCamera);
			traversal.SetPhysicalCamera(view->GetPhysicalCamera());

			// do we need matrix etc ?

			traversal.traverseAll = 1;
			traversal.traverseTransformHierarchyOnly=1;

		int ret = TraverseChildren(traversal);
		return ret;
}

void GetProps(GFile *file,BBox &bbox) 
{
	TRACE("fileSize=%d\n",file->size);
	TRACE("isZipped=%d\n",file->fileIsZipped);

	TRACE("bboxMin=%g %g %g\n",bbox.mi.x,bbox.mi.y,bbox.mi.z );
	TRACE("bboxMax=%g %g %g\n",bbox.ma.x,bbox.ma.y,bbox.ma.z );

	///urlExtraArgs
	// contentLength
	// contentType
	// contentEncoding


}

int 			
Gv2Inline::Load(GTraversal &state)
{
	// whats the current status
	GFileState status = loader.GetState();

Redo:
	switch(status) {
	case S_BAD_DATA:
	case S_BAD_URL:
		if (loader.urlI< (url.Length() -1) ) {
			loader.urlI ++;
			// fall through and load next url 
		} else {
			status = loader.state = S_ALL_BAD; // can´t load any URL 
			isLoaded.set(FALSE);
			children.OnChanged(); // so could advise on children_changed 
		    break;
		}
	case S_NOT_LOADED: // starting state : NOT loaded 
	     if (url.Length() == 0) { 
			 loader.SetNoUrl();		
			 isLoaded.set(FALSE);
			 children.OnChanged(); // so could advise on children_changed 
			 return(0);
		 }
		 if (!loader.HasBaseUrl())
			loader.SetBaseUrl(state.GetHomeUrl());

		 // start processing 
		 {
		 CTime refreshTime(0);
		 const char *refedFromUrl= state.GetUrl(refreshTime);	 

		 status = loader.Load(loader.GetBaseUrl(),url[loader.urlI].getString(),
			 refedFromUrl,"Inline",state.inlineWwwFlags,state.view->hWnd,loader.urlI,refreshTime,state.view->GetReporter(),state.view->file);
		 }

 		 // set url parameters
		 worldUrl.set(loader.GetUrl());
		 baseUrl.set(loader.GetHomeUrl());

		 if (status != S_NOT_LOADED) goto Redo;

		 break;

	case S_LOADING:
				state.pendingLoads ++;
				break;

	case S_LOADED: {
				if (!state.view) {
					ASSERT(0);
					break;
				}

				int ret;
				GvBool useThread =  state.loadInThread && !RTISA(this,Gv2CreateVrmlFromUrl);

				if (loader.loader) {
					GFile *file = loader.loader;
					
					if (file->lastModified != file->cacheLastModified && file->cacheLastModified > 0) {

					// the URL was retreived and had diffent modifiedDate then file in cache
					file->ifModifiedSince = CTime::GetCurrentTime(); // refresh 
					TRACE("Inline url changed, refreshing children \n");
					loader.checkTime = file->ifModifiedSince ;
#ifdef _DEBUG
					state.view->GetReporter()->Trace("INLINE  %s url changed, refreshing children",(const char *) file->url);
#endif	
				}
				}


				this->deleteAll(); // delete all contents 



				worldUrl.set(loader.GetUrl());
				baseUrl.set(loader.GetHomeUrl());
#ifdef WIN32
				if (useThread) {	
					   // hg 21.07.99 for savety set module state, if call results from COM/CALL/ BeginThread use AfxGetApp
					#ifdef _AFXDLL
						AFX_MANAGE_STATE(AfxGetStaticModuleState());
					#endif

					if (parser) {
						parser->KillThread();
						parser = NULL;
					}
					parser = new GvMediaThread(GV_VRML_FILE,loader.GetLocalFile());

					parser->m_reporter = state.view->GetReporter();
					parser->m_flags = (state.view->reportErrors?0:GV_READ_NOERROR);
					parser->m_scene = this;
					if (parser->CreateThread())
					{
						loader.Release();
						loader.state = S_PROCESSING;
					}
					else { 
						delete parser; parser = NULL;
					}

					break;

				}
#endif

				state.view->lastFrameLoadCnt++;
		 		state.view->lastFrameInlineLoadCnt++;

				ret = ReadVrmlFile(loader.GetLocalFile(),state.view->GetReporter(),this, (state.view->reportErrors?0:GV_READ_NOERROR));

				if (ret>=0) {  // fine, we could read it 
					loader.SetProcessed();
					
					children.CheckLights();
					setNeedLightCheck(FALSE);

					// traverse and instace proto's trigger extern proto load 
					PrepareChildren(state.view);

					children.OnChanged(); // so could advise on children_changed 
					isLoaded.set(TRUE);
					return(1);
				}
				else { 
					loader.SetBadData(); // bad, try next
					state.view->lastFrameLoadErrorCnt++;
					status = loader.GetState();
					if (status != S_LOADED) {
						this->deleteAll(); // delete all contents 
						goto Redo; // new 15.06.98
					}
				}

				return(1);
			   }
	case S_PROCESSING : 
				{
#ifdef WIN32
				ASSERT_VALID(parser);
				if (parser) {
				if (parser->m_status == GV_READY) {
				
				int ret = parser->m_ret;
				TRACE("Inline parser ready %d \n",ret);
				
				parser->KillThread();
				parser = NULL;


				if (ret>=0) {  // fine, we could read it 
					loader.SetProcessed();

					children.CheckLights();
					setNeedLightCheck(FALSE);

					// traverse and instace proto's trigger extern proto load 
					PrepareChildren(state.view);
/*
					if (RTISA(this,GvAvatar)) { // update bounding box 
						BBox bbox;
						if (!getBBox(bbox))
							setBBox(bbox);
					}
*/

					children.OnChanged(); // so could advise on children_changed 
					isLoaded.set(TRUE);
					state.view->lastFrameLoadCnt++;
			 		state.view->lastFrameInlineLoadCnt++;
					return(1);

				}
				else { 

					// try to delete 
					loader.DeleteCachedLocalFile();

					loader.SetBadData(); // bad, try next
					state.view->lastFrameLoadErrorCnt++;
					status = loader.GetState();
					this->deleteAll(); // delete all contents 


					if (status != S_LOADED) {
						this->deleteAll(); // delete all contents 
						goto Redo; 
					}
				}


				}
				else {
					state.pendingLoads ++;
					return 0;
				}
				}
#endif

				};
				break;
	case S_PROCESSED:
				return(1);
	default :
		break;

	}
	return(0);
}

int 			
Gv2Inline::Do(GglRenderTraversal &state)
{
  int ret;
  
  if (!state.CullCheck(bboxCenter,bboxSize)) 
	  return GV_NODE_CULLED;

  lastRenderTime=state.view->theTime; // time this inline was rendered


  if (!loader.Ok()) { // we need to check the Loader state 
	if ((state.inlineLevel <= state.maxInlineLevel &&  
		(state.view->lastFrameInlineLoadCnt < state.view->maxFrameInlineLoadCnt)  // make a small pause
		&& state.loadInlines >= GTraversal::LOAD_IF_NEEDED)) {
		if (!isRegistered) state.getBrowser()->Register(this);
		Load(state);
    }
  }	


  if (loader.Ok()) { 

#ifdef WIN32
		TRY	
#endif
			
		{

#ifdef _G_VRML1
		if (!vrml2.value) {	// VRML 1.0 file included in VRML 2 scene, !! BAD 
			state.ClearStaticAttributes();
			GvglShapeHintsElement *shapeHints = (GvglShapeHintsElement*) state.Get(GvglShapeHintsElement::attrId); 
			if (shapeHints)
				shapeHints->Set(state);
			GvglTexture2Element *tex = (GvglTexture2Element*) state.Get(GvglTexture2Element::attrId); 
			if (tex)
				tex->Set(state);

			// set default material ?
			// compare GvNodeShell::DO
		}
#endif _G_VRML1

		// pass to TraverseChildren for proper stack handling
  		ret = TraverseChildren(state);

		}

#ifdef WIN32
		CATCH_ALL(e)
		{
			// Note: DELETE_EXCEPTION(e) not required.
			TRACE0("Gv2Inline:Exception during render \n");
			ASSERT(0);
			Unload();
			loader.SetBadData();

		}
		END_CATCH_ALL
#endif

		
		return ret;

  }

  // not loaded, display bbox 
  if (bboxIsEmpty()) 
	  return(-1);

  // to do state: if(renderInlineBox)
  Point s = bboxSize * 0.5;
  state.glState->RenderCubeEdges(bboxCenter-s,bboxCenter+s);

  return(-2);
}

int Gv2Inline::Do(GTraversal &state)
{
  if (!loader.Ok()) {
	if ((state.inlineLevel <= state.maxInlineLevel)
  		&& state.loadInlines >= GTraversal::LOAD_IF_NEEDED)
		Load(state);
  }	

  if (loader.Ok()) { 
#ifdef _G_VRML1

		if (!vrml2.value) {	// VRML 1.0 file included in VRML 2 scene, !! BAD 
			state.appearance = NULL;
			state.ClearStaticAttributes();
			GvShapeHintsElement *shapeHints = (GvShapeHintsElement*) state.Get(GvShapeHintsElement::attrId); 
			if (shapeHints) shapeHints->Set(state);
			GvTexture2Element *tex = (GvTexture2Element*) state.Get(GvTexture2Element::attrId); 
			if (tex) tex->Set(state);
		}
#endif

		// pass to TraverseChildren for proper stack handling
  		return(TraverseChildren(state));
  }

  return(0);
}

int Gv2Inline::Do(GPathTraversal &state)
{
  if (!loader.Ok()) {
	if ((state.inlineLevel <= state.maxInlineLevel)
  		&& state.loadInlines >= GTraversal::LOAD_IF_NEEDED)
		Load(state);
  }	

  if (loader.Ok()) { 

#ifdef _G_VRML1

		if (!vrml2.value) {	// VRML 1.0 file included in VRML 2 scene, !! BAD 
			state.appearance = NULL;
			state.ClearStaticAttributes();
			GvShapeHintsElement *shapeHints = (GvShapeHintsElement*) state.Get(GvShapeHintsElement::attrId); 
			if (shapeHints) shapeHints->Set(state);
			GvTexture2Element *tex = (GvTexture2Element*) state.Get(GvTexture2Element::attrId); 
			if (tex) tex->Set(state);
		}
#endif _G_VRML1

		// pass to TraverseChildren for proper stack handling
  		return(TraverseChildren(state));
  }

  return(0);
}


int Gv2Inline::Do(GBBoxTraversal &state)
{
  if (!bboxIsEmpty() && (state.useBBoxes || !loader.Ok())) {  // use bbox 
		BBox box;
		Point s = bboxSize * 0.5;
		box.mi = bboxCenter-s;
		box.ma = bboxCenter+s;
 
		state.Union(box);
		return(1);
  }	


  // if loaded, compute bbox from children
  if (loader.Ok()) {
		// pass to TraverseChildren for proper stack handling
   		return TraverseChildren(state);
  }
  else 	return(0);
}



int Gv2Inline::Do(GLoadInlinesTraversal &state)
{
   if (state.resolveRelativeUrls && !loader.HasBaseUrl()) {
		loader.SetBaseUrl(state.GetHomeUrl());
  }


  if (!loader.Ok()) {
	if ((state.inlineLevel <= state.maxInlineLevel)
		&& state.loadInlines >= GTraversal::LOAD_IF_NEEDED) {
		if (!isRegistered) state.getBrowser()->Register(this);

		Load(state);
		
		if ( !(loader.GetState() <= S_NO_DATA) &&  state.stopOnFirst) { // stop loading
			state.loadInlines = GTraversal::DONT_LOAD;
			return TRAVERSE_STOP;
		}
	}
  }	
  
  else 

  if (loader.Ok()) { 
		// pass to TraverseChildren for proper stack handling
  		return(TraverseChildren(state));
  }

  return(0);
}



// traverse the children && put Inline element into state
int Gv2Inline::TraverseChildren(GTraversal &state)
{
  int ret=0;

  if (loader.Ok()) { 
  	state.inlineLevel++;
    // put Inline node in state to resolve relative includes
    state.Push();

	GvWWWInlineElement *current,*prev;
	prev = (GvWWWInlineElement*) state.Get(GvWWWInlineElement::attrId);
	if (!prev) {
	} else {
		current = (GvWWWInlineElement *) prev->NewInstance();								
		current->data = this;
		state.Push(GvWWWInlineElement::attrId,current);			
	 }

	ret = Gv2Group::TraverseChildren(state);

	state.Pop();

  	state.inlineLevel--;
	return(ret);
  }
  else {

  }
  return(ret);
}


// traverse the children && put Inline element into state
int Gv2Inline::TraverseChildren(GPathTraversal &state)
{
  int ret=0;

  if (loader.Ok()) { 
  	state.inlineLevel++;
    // put Inline node in state to resolve relative includes
    state.Push();

	GvWWWInlineElement *current,*prev;
	prev = (GvWWWInlineElement*) state.Get(GvWWWInlineElement::attrId);
	if (!prev) {
	} else {
		current = (GvWWWInlineElement *) prev->NewInstance();								
		current->data = this;
		state.Push(GvWWWInlineElement::attrId,current);			
	 }

	ret = Gv2Group::TraverseChildren(state);

	state.Pop();

  	state.inlineLevel--;
	return(ret);
  }
  else {

  }
  return(ret);
}






//
// Gv2CreateVrmlFromUrl
//

GV_NODE_SOURCE_IMP(Gv2CreateVrmlFromUrl,Gv2Inline);

Gv2CreateVrmlFromUrl::Gv2CreateVrmlFromUrl() : nodeEvent(-1)
{
    GV_NODE_CONSTRUCTOR(Gv2CreateVrmlFromUrl);
    isBuiltIn = FALSE;

	// from Gv2group
	GV_NODE_ADD_FIELD(bboxCenter);
	GV_NODE_ADD_FIELD(bboxSize);

	// from Gv2Inline
	GV_NODE_ADD_FIELD(url);
    GV_NODE_ADD_EVENT_OUT(isLoaded);


    //from Gv2group
	GV_NODE_ADD_FIELD(children);
    GV_NODE_ADD_EVENT_IN(addChildren,GvMFNode);
    GV_NODE_ADD_EVENT_IN(removeChildren,GvMFNode);

	GV_NODE_ADD_FIELD(node);
	GV_NODE_ADD_FIELD(nodeEvent);


}

Gv2CreateVrmlFromUrl::~Gv2CreateVrmlFromUrl()
{
}


GvBool Gv2CreateVrmlFromUrl::CheckForLoad(GFile *file)
{

  if (!loader.Ok()) {
//	  Load(State);
  }
  if (loader.Ok()) {
	  return TRUE;
  }	
  else return FALSE;
}


GvBool Gv2CreateVrmlFromUrl::SendEvent()
{
	if (nodeEvent<0) return FALSE;
	return GvNode::sendEventIn(&children,node,nodeEvent);
}





//
// GvAvatar
//

GV_NODE_SOURCE_IMP(GvAvatar,Gv2Inline);

GvAvatar::GvAvatar(): 
	avatarSize(0),
	properties(0)

{
    GV_NODE_CONSTRUCTOR(GvAvatar);
    isBuiltIn = FALSE;

	// from Gv2group
	GV_NODE_ADD_FIELD(bboxCenter);
	GV_NODE_ADD_FIELD(bboxSize);

	// from Gv2Inline
	GV_NODE_ADD_FIELD(url);

    GV_NODE_ADD_EVENT_IN(set_unload,GvSFBool);
    GV_NODE_ADD_EVENT_OUT(isLoaded);


    //from Gv2group
	GV_NODE_ADD_FIELD(children);
    GV_NODE_ADD_EVENT_IN(addChildren,GvMFNode);
    GV_NODE_ADD_EVENT_IN(removeChildren,GvMFNode);

	// avatar 
	GV_NODE_ADD_FIELD(avatarURL);	
	GV_NODE_ADD_FIELD(properties);	


}

GvAvatar::~GvAvatar()
{
}

/*
int 			
GvAvatar::Do(GglRenderTraversal &state) {
	int ret=0;

	int xi = state.view->lastFrameInlineLoadCnt;
	int xp = state.view->lastFrameProtoInstanceCnt;
		
		ret =Gv2Inline::Do(state);

	// invalidate bbox if inline / proto loaded
	if ( (xi != state.view->lastFrameInlineLoadCnt)
		 || (xp != state.view->lastFrameProtoInstanceCnt))
	{
		bboxOk = gfalse;
	}

}

*/

