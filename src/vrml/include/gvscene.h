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
#ifndef  _GvScene_
#define  _GvScene_

/******************************************************************************
@doc

@module GvScene.h - VRML Scene Meta node|

Copyright (c) 1996 - 1998 by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GvScene>

Notes:

Changes:
$Revision: 1.19 $
$Log: gvscene.h,v $
Revision 1.19  1999/07/06 16:35:17  tom
*** empty log message ***

Revision 1.18  1999/05/10 13:54:09  tom
*** empty log message ***


Todo :

******************************************************************************/


#include <gv2group.h>

#include <gvsfint32.h>
#include <gvsftime.h>

#include <gvsfnode.h>
#include <gvmfnode.h>
#include <gvsfstring.h>

#include <gvmfstring.h>
#include <gvsfbool.h>

#include <gvnamespace.h>


// forwards 
class GvProto;
class GvExternProto;
class GvProtoInstance;
class GvRoute;
class GvWorldInfo;
class GvTimeSensor;
class GvViewpoint;
class Gv2CreateVrmlFromUrl;
class GFile;

class GReporter;




/*

  GvScene
  maintains a scenegraph
  together with protos, routes ...
*/



class GvScene : public Gv2Group {

    GV_NODE_HEADER(GvScene);

  public:

    // Derived Fields 
    // GvMFNode children;  // the top level node list of scene

    // Fields 

    GvMFNode protos;    // the protos defined in this scene

    GvMFNode routes;    // the routes defined

    GvMFNode timeSensors; // all time sensors in this context

    GvMFNode viewpoints; // all viewpoint nodes in this context

    GvMFNode createVrmlFromUrls; // all create vrmls from urls in progress

    GvSFNode worldInfo;     // the world info of this scene

    GvSFString worldUrl;         // the url of this scene

    GvSFString baseUrl;     // the base url of this scene
    
    GvSFBool vrml2;         // vrm 2.0 version ? 
	
	GvBool isVrml2() const { return(vrml2.value); }

    GvSFTime worldStartTime;
    GvSFTime worldEndTime;


    GvMFNode  ownerScenes;  // Scene nodes this is a child scene off
    GvMFNode  childScenes;  // child Scene nodes 

    // the name space of this scene
    GvNameSpace nameSpace;


	// get absolute home url
	const char * GetHomeUrl() const;

	// get the absolute url
	const char * GetUrl() const;



	// delete all contents of scene
	void deleteAll();
	
	// delete all contents except proto's of scene
	void deleteSceneGraph();


    // define a new proto
    GvBool addProto(GvProto *proto);

    // define a new proto
    GvBool addProto(const GvName &name,GvProto *proto);

    // remove proto
    GvBool removeProto(GvProto *proto);

    void removeUnusedProtos();


    static GvBool instanceRoute(GvNode * from, int fromIndex, GvNode * to,int toIndex);

    // define route add add Route node to routes
    GvBool addRoute(GvNode *from, int fromIndex, GvNode *to,int toIndex);

    // define route
    GvBool addRoute(GvRoute *route,GvBool alreadyInstaced=TRUE);

    // remove route
    GvBool deleteRoute(GvRoute *route);

	// find a route by VALUE ! NULL if not found 
	GvRoute* findRoute(GvRoute *route);

	// remove any routes, considered to be unused 
    GvBool removeUnusedRoutes();

    // define a new timesensor
    GvBool addTimeSensor(GvTimeSensor *node);

    // remove a timesensor
    GvBool removeTimeSensor(GvTimeSensor *node);

    // define a new viewpoint
    GvBool addViewpoint(GvViewpoint *node);

    // remove a viewpoint
    GvBool removeViewpoint(GvViewpoint *node);

	GvBool addCreateVrmlFromUrl(Gv2CreateVrmlFromUrl *node);
	GvBool removeCreateVrmlFromUrl(Gv2CreateVrmlFromUrl *node);

	// check CreateVrmlFromUrl for loaded url´s and send event
	GvBool checkCreateVrmlFromUrl(GFile *file);



    // set world info 
    GvBool setWorldInfo(GvWorldInfo *node);

    // remove world info 
    GvBool removeWorldInfo(GvWorldInfo *node);


    // add pointer to new child scene
    // automatically call addOwnerScene on scene
    GvBool addChildScene(GvScene *scene);

    // remove pointer to child scene 
    GvBool removeChildScene(GvScene *scene);

    // add pointer to another owner
    GvBool addOwnerScene(GvScene *scene);

    // remove pointer to owner 
    GvBool removeOwnerScene(GvScene *scene);

	// get named proto or first if name == NULL
	GvProto* getProto(const char *name);

	// try to create a ProtoInstance node of scene Proto with name classname
	GvNode *createProtoInstance(const GvName &className);

    // create a NodeInstace given classname 
	// and using proto classes defined in this namespace
    GvNode* createNodeInstance(const GvName &className);


	// read from  (read nodes until EOF) 

    GvBool read(GvInput *in);


    // trigger all time sensors, return TRUE if any fired
    GvBool OnTimeChange(double t); 


    /*

        Interface to bound nodes,
    */
    // viewPoint 


/*
    // define a new node by name 
    GvBool addNode(const GvName &name,GvNode *node);
*/

    // overrides
	int Do(GTraversal &state);   	
	int Do(GPathTraversal &state);
	int Do(GRaySelectTraversal &state);


    int Do(GglRenderTraversal &state);
    int Do(GioTraversal &state);

/*
   
    // static current scene managment 
    static GvScene *current;
    static GvScene* getCurrent() { return(current); }
    static void setCurrent(GvScene *newCurrent) { current = newCurrent; }
    static void push(GvScene *newCurrent);
    static GvScene* pop();
*/


    // a NODE has been edited by UI.
    //void OnNodeEdited(GvNode *node);
    // a node has changed, set redraw bit
    //void OnNodeChanged(GvNode *node);
    // GvBool OnNodeSetName(GvNode *node,const GvName &newName);
    // GvBool redraw;
    //void Redraw();
public:

	// supporting methods for EAI 

	// parse the string and build new scene
	GvScene* createVrmlFromString(char *vrmlSyntax,GReporter *reporter);

	// get a node by name 
	GvNode* getNode(const GvName &name);

	// set new name for node 
	GvBool setNodeName(GvNode *node,const GvName &newName);

	// add node to scene 
	GvBool addNode(GvNode *node);

	// remove node from scene 
	GvBool removeNode(GvNode *node);


};

#endif




