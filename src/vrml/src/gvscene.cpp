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

@module GvScene.cpp - Implementation of VRML Scene meta node|

Copyright (c) 1995- 1998	by Holger Grahn
All rights reserved

Purpose:

Scene stores a scene graph and scene proto's and routes

Classes:

Notes:


Changes:

$Revision: 1.22 $
$Log: gvscene.cpp,v $
Revision 1.22  1999/07/06 16:46:11  tom
*** empty log message ***

Revision 1.21  1999/05/31 13:15:41  tom
empty message



Todo :

******************************************************************************/


#include <gvscene.h>
#include <gvproto.h>
#include <gvconnection.h>
#include <gvsensors.h>
// viewpoint
#include <gvperspectivecamera.h>
#include <gvinfo.h>
#include <gvelement.h>
#include <gvwwwinline.h>

#include "grayselect.h"


//
// GvScene
//

//GvScene * GvScene::current = NULL;

GV_NODE_SOURCE_IMP(GvScene,Gv2Group);

GvScene::GvScene() : 
    protos(0),
    routes(0),
    timeSensors(0),
    viewpoints(0),
	createVrmlFromUrls(0),
    ownerScenes(0),
    childScenes(0)

{
    GV_NODE_CONSTRUCTOR(GvScene);

    isBuiltIn = FALSE;

    GV_NODE_ADD_FIELD(bboxSize);
    GV_NODE_ADD_FIELD(bboxCenter);
    GV_NODE_ADD_FIELD(children);
    GV_NODE_ADD_EVENT_IN(addChildren,GvMFNode);
    GV_NODE_ADD_EVENT_IN(removeChildren,GvMFNode);


    GV_NODE_ADD_FIELD(protos);
    GV_NODE_ADD_FIELD(routes);
    GV_NODE_ADD_FIELD(timeSensors);
    GV_NODE_ADD_FIELD(viewpoints);
    GV_NODE_ADD_FIELD(createVrmlFromUrls);
    GV_NODE_ADD_FIELD(worldUrl);
    GV_NODE_ADD_FIELD(baseUrl);
    GV_NODE_ADD_FIELD(worldInfo);
    GV_NODE_ADD_FIELD(worldStartTime);
    GV_NODE_ADD_FIELD(worldEndTime);
    GV_NODE_ADD_FIELD(vrml2);

    //GV_NODE_ADD_FIELD(ownerScenes);
    //GV_NODE_ADD_FIELD(childScenes);

    bboxCenter.value[0] = bboxCenter.value[1] = bboxCenter.value[2] = 0.0;
    bboxSize.value[0] = bboxSize.value[1] = bboxSize.value[2] = -1.0;
	TRACE("GvScene() %p\n",this);

}


// delete all contents of scene
void GvScene::deleteAll() {
		// do not trigger events 
		timeSensors.deleteAll();
		viewpoints.deleteAll();
		int old = worldInfo.enableConnections(0);
			worldInfo.set((GvNode *) NULL);
		worldInfo.enableConnections(old);
		protos.deleteAll();
		createVrmlFromUrls.deleteAll();
		children.deleteAll();
		routes.deleteAll();

}

// delete all contents except proto's of scene
void GvScene::deleteSceneGraph() 
{
	timeSensors.deleteAll();
	viewpoints.deleteAll();

	//worldInfo.set((GvNode*)NULL);
	//protos.deleteAll();

	createVrmlFromUrls.deleteAll();
	children.deleteAll();
	routes.deleteAll();
}


GvScene::~GvScene()
{
	TRACE("::~GvScene() %p\n",this);

   	// give proper ordering of deletes
	deleteAll();
/*
	if (GvScene::getCurrent() == this) 
		GvScene::setCurrent(NULL);
*/
}


// get absolute home url
const char * GvScene::GetHomeUrl() const
{
	return baseUrl;
}

// get the absolute url
const char * GvScene::GetUrl() const
{
	return worldUrl;
}


// define a new proto
GvBool GvScene::addProto(GvProto *proto)
{
    if (protos.add(proto)) {
		// url of Externproto must be resolved to scene contained in
		if (RTISA(proto,GvExternProto)){
			GvExternProto *p = (GvExternProto *) proto;
			p->baseUrl = baseUrl.get();

		}
        protos.setDefault(FALSE);
        protos.OnChanged();
        return(TRUE);
    } 
    else return(FALSE);;
}


// define a new proto
GvBool GvScene::addProto(const GvName &name,GvProto *proto)
{
    nameSpace.addProto(name,proto);
    if (protos.add(proto)) {
		if (RTISA(proto,GvExternProto)){
			GvExternProto *p = (GvExternProto *) proto;
			p->baseUrl = baseUrl.get();

		}
        protos.setDefault(FALSE);
        protos.OnChanged();
        return(TRUE);
    } 
    else return(FALSE);;
}

// remove proto
GvBool GvScene::removeProto(GvProto *proto)
{   // note: proto may still be refed by instances of this proto
    // to do: nameSpace.removeProto(proto);
    if (protos.remove(proto)) {
        protos.setDefault(FALSE);
        protos.OnChanged();
        return(TRUE);
    } 
    else return(FALSE);;
}

// get named proto or first if name == NULL
GvProto* GvScene::getProto(const char *name)
{
	if (name == NULL || name[0] == 0) {
		if (protos.getNum() > 0) 
			return((GvProto*) protos.get1(0));
		return (NULL);
	}
    return(nameSpace.findProto(name,FALSE));
}

// try to create a ProtoInstance node of scene Proto with name classname
GvNode *
GvScene::createProtoInstance(const GvName &className)
{
    GvProto* proto = nameSpace.findProto(className);
    if (!proto) {	 // no proto found
        return(NULL);
    }
    GvNode		*instance;
	instance = proto->createInstance();
	if (instance) instance->setBrowser(getBrowser());
	// TRACE("GvScene::CreateProtoInstance : %s \n",className.getString());
	return(instance);
}

// create a NodeInstace given classname and using protos classes
GvNode* GvScene::
createNodeInstance(const GvName &className)
{
	GvNode* instance;

	// is there a proto defined for this name ? 
	instance = createProtoInstance(className);
	if (instance) return(instance);

	instance = createInstanceFromName(className,isVrml2());
	if (instance) instance->setBrowser(getBrowser());
	return(instance);
}




GvBool
GvScene::instanceRoute(GvNode * from, int fromIndex, GvNode * to,int toIndex)
{


	return GvNode::addRoute(from,fromIndex,to,toIndex);

}


// add route to route list
 
GvBool 
GvScene::addRoute(GvNode * from, int fromIndex, GvNode * to,int toIndex)
{
    GvRoute *route = new GvRoute();
    route->set(from,fromIndex,to,toIndex);
    return addRoute(route,FALSE);
}

// define route
GvBool GvScene::addRoute(GvRoute *route,GvBool alreadyInstanced)
{
	GvBool ret = TRUE;
    //  instance
    if (!alreadyInstanced) 
        ret=instanceRoute(route->from,route->fromEventIndex,route->to,route->toEventIndex);

    if (routes.add(route)) {
        routes.setDefault(FALSE);
        routes.OnChanged();
    } 
    return(ret);
}

// delete route
GvBool GvScene::deleteRoute(GvRoute *route)
{
    //  deinstance route 
	GvBool ret = GvNode::deleteRoute(route->from,route->fromEventIndex,route->to,route->toEventIndex);

    if (routes.remove(route)) {
        routes.setDefault(FALSE);
        routes.OnChanged();
    } 
    return(ret);
}

// find a route by VALUE ! NULL if not found 
GvRoute* GvScene::findRoute(GvRoute *route)
{

	for (int i=0; i<routes.getNum(); i++) {
		GvRoute *src = (GvRoute *) routes.get1(i);
		if (*src == *route) return src;
	}
    return(NULL);
}

// remove any routes, considered to be unused 
GvBool GvScene::removeUnusedRoutes()
{
	GvBool found=FALSE;
	for (int i=routes.getNum()-1; i>=0; i--) {
		GvRoute *route = (GvRoute *) routes.get1(i);
		if (route->isUnused()) {
		    //  deinstance route 
			int ret = GvNode::deleteRoute(route->from,route->fromEventIndex,route->to,route->toEventIndex);
			routes.deleteElement(i);
			found = TRUE;
		}
	}
	if (found) {
        routes.OnChanged();
	}
	return found;
}



// define a new timesensor
GvBool GvScene::addTimeSensor(GvTimeSensor *node)
{
    if (timeSensors.add(node)) {
        timeSensors.setDefault(FALSE);
        timeSensors.OnChanged();
        return(TRUE);
    } 
    else return(FALSE);
}

// remove a timesensor
GvBool GvScene::removeTimeSensor(GvTimeSensor *node)
{
    if (timeSensors.remove(node)) {
        timeSensors.setDefault(FALSE);
        timeSensors.OnChanged();
        return(TRUE);
    } 
    else return(FALSE);;
}

// define a new viewpoint
GvBool GvScene::addViewpoint(GvViewpoint *node)
{
    if (viewpoints.add(node)) {
        viewpoints.setDefault(FALSE);
        viewpoints.OnChanged();
        return(TRUE);
    } 
    else return(FALSE);;
}

// remove a viewpoint
GvBool GvScene::removeViewpoint(GvViewpoint *node)
{
    if (viewpoints.remove(node)) {
        viewpoints.setDefault(FALSE);
        viewpoints.OnChanged();
        return(TRUE);
    } 
    else return(FALSE);;
}

GvBool GvScene::addCreateVrmlFromUrl(Gv2CreateVrmlFromUrl *node)
{
    if (createVrmlFromUrls.add(node)) {
        createVrmlFromUrls.setDefault(FALSE);
        createVrmlFromUrls.OnChanged();
        return(TRUE);
    } 
    else return(FALSE);;
}

// remove a CreateVrmlFromUrl
GvBool GvScene::removeCreateVrmlFromUrl(Gv2CreateVrmlFromUrl *node)
{
    if (createVrmlFromUrls.remove(node)) {
        createVrmlFromUrls.setDefault(FALSE);
        createVrmlFromUrls.OnChanged();
        return(TRUE);
    } 
    else return(FALSE);;
}

// check CreateVrmlFromUrl for loaded url´s and send event
GvBool GvScene::checkCreateVrmlFromUrl(GFile *file)
{

	GvBool ret = FALSE;
	for (int i=0; i<createVrmlFromUrls.getNum(); i++) {
		Gv2CreateVrmlFromUrl *src = (Gv2CreateVrmlFromUrl *) createVrmlFromUrls.get1(i);
		if (src->CheckForLoad(file)) {
			src->SendEvent();
			removeCreateVrmlFromUrl(src); // will unref it !
			ret = TRUE;
			i--; // continue check others 
		}
	}
    return(ret);
}


// set world info 
GvBool GvScene::setWorldInfo(GvWorldInfo *node)
{
    worldInfo.set(node);
    return(TRUE);
}

// remove world info 
GvBool GvScene::removeWorldInfo(GvWorldInfo *node)
{
    worldInfo.set((GvNode*) NULL);
    return(TRUE);
}
   


// add pointer to new child scene
GvBool GvScene::addChildScene(GvScene *scene)
{
    childScenes.add(scene);
    return scene->addOwnerScene(this);
}


// remove pointer to child scene 
GvBool GvScene::removeChildScene(GvScene *scene)
{
    scene->removeOwnerScene(this);

    return childScenes.remove(scene);
}

// add pointer to another owner
GvBool GvScene::addOwnerScene(GvScene *scene)
{
    return ownerScenes.add(scene);
}


// remove pointer to owner 
GvBool GvScene::removeOwnerScene(GvScene *scene)
{
    return ownerScenes.remove(scene);
}



// read
// push namescape and current scene

GvBool
GvScene::read(GvInput *in)
{
    // push scene
    GvScene *current = in->getScene();
   
	in->setScene(this);  // set the current scene 
	
	ASSERT(getBrowser() != NULL);


	// in->setUrl(GetUrl());
	
    in->pushNameSpace(&nameSpace);

    
    GvBool ret = children.readValueList(in,EOF); // EOF
    
    children.setDefault(FALSE);

    in->popNameSpace();


    // pop scene
    //setCurrent(current);
	
	in->setScene(current);

    vrml2.set(in->isVrml2());



    return(ret);

}


/*
   trigger all time sensors
*/



GvBool GvScene::OnTimeChange(double t) 
{   
/* HG removed 25.10.97, TimeSensor not added automatically to scene 
*/
    int n = timeSensors.Length();
    int i;
    GvBool ret = FALSE;
    for(i=0; i<n; i++) {
        GvTimeSensor *s;
        s = (GvTimeSensor*) (GvNode *) timeSensors.values[i];
        if (s->OnTimeChange(t) >0) ret = TRUE;
    }
    
    n = childScenes.Length();
    for(i=0; i<n; i++) {
        GvScene *s;
        s = (GvScene*) (GvNode *) childScenes.values[i];
        if (s->OnTimeChange(t)) ret = TRUE;
    }


    return(ret);
}



int GvScene:: Do(GTraversal &state)
{	int ret = 0;

    // push scene
    //GvScene *current = getCurrent();
    //setCurrent(this);

    state.Push();
		{GvWWWInlineElement *current,*prev;

		prev = (GvWWWInlineElement*) state.Get(GvWWWInlineElement::attrId);
		if (!prev) {
		} else {
			current = (GvWWWInlineElement *) prev->NewInstance();								
			current->data = this;
			state.Push(GvWWWInlineElement::attrId,current);			
		}
		}

    	ret = children.Traverse(state);
	state.Pop();
    
    // pop scene
    //setCurrent(current);

	return(ret);
}

int GvScene:: Do(GPathTraversal &state)
{	int ret = 0;

    // push scene
    //GvScene *current = getCurrent();
    //setCurrent(this);

    state.Push();
	GPUSH_PATH(state,this);
		{GvWWWInlineElement *current,*prev;

		prev = (GvWWWInlineElement*) state.Get(GvWWWInlineElement::attrId);
		if (!prev) {
		} else {
			current = (GvWWWInlineElement *) prev->NewInstance();								
			current->data = this;
			state.Push(GvWWWInlineElement::attrId,current);			
		}
		}

    	ret = children.Traverse(state);

	GPOP_PATH(state);

	state.Pop();
    
    // pop scene
    //setCurrent(current);

	return(ret);
}


int GvScene:: Do(GRaySelectTraversal &state)
{	int ret = 0;

    // push scene
    //GvScene *current = getCurrent();
    //setCurrent(this);

    state.Push();
	GPUSH_PATH(state,this);
		{GvWWWInlineElement *current,*prev;

		prev = (GvWWWInlineElement*) state.Get(GvWWWInlineElement::attrId);
		if (!prev) {
		} else {
			current = (GvWWWInlineElement *) prev->NewInstance();								
			current->data = this;
			state.Push(GvWWWInlineElement::attrId,current);			
		}
		}
		if (state.mode == GRaySelectTraversal::MODE_SELECTION)
			ret = ApplyGeometrySensors(state);

    	ret = children.Traverse(state);

	GPOP_PATH(state);
	state.Pop();
    
    // pop scene
    //setCurrent(current);

	return(ret);
}


int GvScene::Do(GglRenderTraversal &state)
{  	int ret = 0;

    // push scene
    //GvScene *current = getCurrent();
    //setCurrent(this);

	if (getNeedLightCheck()) {	// move lights in Front 
		children.CheckLights();
		setNeedLightCheck(FALSE);
	}

    // to do: do render culling here
    state.Push();
		{ GvWWWInlineElement *current,*prev;
		prev = (GvWWWInlineElement*) state.Get(GvWWWInlineElement::attrId);
		if (!prev) {
		} else {
			current = (GvWWWInlineElement *) prev->NewInstance();								
			current->data = this;
			state.Push(GvWWWInlineElement::attrId,current);			
		}
		}

    	ret = children.Traverse(state);

	state.Pop();

    // pop scene
    //setCurrent(current);

	return(ret);
}




// parse the string and build new scene
GvScene* GvScene::createVrmlFromString(char *vrmlSyntax,GReporter *reporter)
{
	GvScene *scene=new GvScene();

	scene->setBrowser(getBrowser());
	ASSERT(scene->getBrowser() != NULL);	

	scene->vrml2.set(TRUE);
	scene->worldUrl = this->worldUrl;
	scene->baseUrl = this->baseUrl; // wrong get baseUrl from script node 


	GvInput in;
	in.setScene(scene);
	
	in.setSrc("createVrmlFromString");

    in.setString(vrmlSyntax);
	in.version = 2.0;
	in.vrml2 = 1;
	
	if (reporter) {
	   in.setReporter(reporter);
	   in.reportErrors = TRUE;
	}	


#ifndef _DEBUG
	// in.reportErrors = FALSE;
#endif


    in.pushNameSpace(&this->nameSpace); // push this scene on name space to resolve PROTOS 

    in.pushNameSpace(&scene->nameSpace); // push new scene, for new node names

   
    GvBool ret = scene->children.readValueList(&in,EOF); 

    scene->children.setDefault(FALSE);

    in.popNameSpace();
    in.popNameSpace();


	return scene;
}

// get a node by name 
GvNode* GvScene::getNode(const GvName &name)
{
	return nameSpace.findNode(name);
}

// set new name for node 
GvBool GvScene::setNodeName(GvNode *node,const GvName &newName) 
{
	// remove node  from table, if already there
	nameSpace.removeNode(node->getName(),node);

	// set new name 
	node->setName(newName,vrml2);

	// add to namespace 
	return nameSpace.addNode(newName,node);

}

// add node to scene 
GvBool GvScene::addNode(GvNode *node)
{
	if (node == NULL) return FALSE;
	if (children.add(node)) {
		nameSpace.addNode(node->getName(),node);
		OnFieldChanged(&children);
		setRedraw();
		return TRUE;
	}
	return FALSE;
}

// remove node from  scene 
GvBool GvScene::removeNode(GvNode *node)
{
	if (node == NULL) return FALSE;
	if (children.remove(node)) {
		// remove from  namespace 
		nameSpace.removeNode(node->getName(),node);
		OnFieldChanged(&children);
		setRedraw();
		return TRUE;
	}
	return FALSE;
}


