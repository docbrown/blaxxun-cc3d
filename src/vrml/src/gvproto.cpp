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

@module GvProto.cpp - VRML 2.0 PROTO handling|

Copyright (c) 1996 - 1999 by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GvProto>
<c GvProtoInstance>
<c GvExternProto>
<c GvRoute>


Notes:

   contains code to create a copy of a sceneGraph, needed for PROTO instancing

Changes:

$Revision: 1.37 $
$Log: gvproto.cpp,v $
Revision 1.37  1999/07/06 16:46:10  tom
*** empty log message ***



Todo :

******************************************************************************/

#include "stdafx.h"

#include "gversion.h"

#include <gvproto.h>

#include <gvfield.h>
#include <gvconnection.h>
#include <gvscene.h>
#include <gvinput.h>
#include <gvreaderror.h>

#include <gvkeyword.h>

#include <gvsensors.h>
#include <gvelement.h>

#include "grender.h"
#include "gmodel.h"

#include "greadgv.h"

// for load after instance creation

#include <gvscript.h>

#include <greporter.h>



GV_NODE_SOURCE_IMP(GvProto,GvNode);


GvProto::GvProto() : children(0), routes(0)
{
    GV_NODE_CONSTRUCTOR(GvProto);
	isBuiltIn = FALSE;
	//isVrml2 = TRUE;
}

void
GvProto::setClassName(const char *name)
{
    className = GvName(name);
}


const char *GvProto::getClassName() const
{
	return(className.getString());
}


// get VRML Type name for node 
const char * GvProto::GetType()
{ const char *n;
  n= getClassName();
  if (n) return(n);
  n = ClassName();

  if (strncmp(n,"Gv",2) == 0) n +=2;
  return(n);
}


GvFieldData *GvProto::getFieldData() const 
{
	return(& ((GvProto *)this)->protoFieldData);
}

// get pointer to field at index 
// returns NULL, because proto has no fields
// only a field description

GvField *
GvProto::getField(int index) const
{
    return(NULL);
}


GvProto::~GvProto()
{
    // TRACE("~Proto (%s) \n",ClassName());
}

// create a new instance of proto, copy classname & fieldData
GvNode * GvProto::newInstance()
{
	GvProto *n = (GvProto *) GvNode::newInstance();
	n->className = className;
	n->protoFieldData.set(protoFieldData);
	// to do: is this sufficient to create a clone ? 

    TRACE("Warning GvProto::newInstance called (%s) \n",ClassName());

	return(n);
}



// create a prototype instance of this prototype
GvNode *GvProto::createInstance()
{
    return(new GvProtoInstance(this));
}

// add route to route list
 
GvBool 
GvProto::addRoute(GvNode * from, int fromIndex, GvNode * to,int toIndex)
{
    GvRoute *route = new GvRoute();
    route->set(from,fromIndex,to,toIndex);
    routes.append(route);
    return (TRUE);
}



// reads name fieldList { graph }
GvNode*
GvProto::read(GvInput *in)
{   GvProto *node= NULL;
	GvBool ret,gotChar;
	GvName name;
	char c;

    // name
    if (! in->read(name, TRUE)) {
	    GvReadError::post(in,GV_RE_SYNTAX_ERROR, "PROTO name expected");
		return(node);

    }
	node = new GvProto();

	if (node == NULL ) {
	    GvReadError::post(in,GV_RE_OUT_OF_MEMORY, "out of memory");
		return(node);
    }
	
	node->setClassName(name);
	// for savety set browser member 
	if (in->getScene()) node->setBrowser(in->getScene()->getBrowser());


    // fields
    if (! node->protoFieldData.readProtoInterface(in, node)) {
		GvReadError::post(in, GV_RE_SYNTAX_ERROR,"Bad interface specifications for PROTO %s",name.getString());
		delete node;
		return NULL;
	}
#ifdef _GFULL
	GvNameSpace &nameSpace=node->nameSpace; 
#else
    GvNameSpace nameSpace; 
#endif
	
	in->pushNameSpace(&nameSpace);

    // update currentProto to resolve IS connections
    GvProto *oldProto = in->currentProto;
    in->currentProto = node;
    {
    // nodes

	if (!(gotChar = in->read(c)) || c != OPEN_BRACE) {
		GvReadError::post(in,GV_RE_SYNTAX_ERROR, "(PROTO %s) Expected '%c'; got '%c' ",
				name.getString(), OPEN_BRACE, c);
	    ret = FALSE;
	}
	else {
//	    ret = node->readChildren(in);


        ret =   ((GvMFNode *) &node->children)->readValueList(in,CLOSE_BRACE);
        
        node->children.setDefault(FALSE);


	    if (! ret)
			;
			//flush = TRUE;

	    else if (! (gotChar = in->read(c)) || c != CLOSE_BRACE) {
			ret = FALSE;
			GvReadError::post(in,GV_RE_SYNTAX_ERROR, "(PROTO %s) Expected '%c'; got '%c'",
				      name.getString(),
				      CLOSE_BRACE, c);
		}
		}

    in->currentProto = oldProto;
    in->popNameSpace();

    }


    if (!ret) {
		if (node->refCnt>0) {
			// Problem here: connections keep refcnts !!!!
			node->children.deleteAll();
			node->routes.deleteAll();
		//	node->protoFieldData.deleteAll();
		}
		if (node->refCnt>0) {
			TRACE("Dangling pointer to proto node !!!!!!!!!!!!!!\n");
		}
		else delete node;
		node = NULL;
        return(NULL);
	}

    // compute copyOnInstance flag
    // traverse all children nodes

	{ GvCopyContext context(NULL);
		node->copyOnInstance = node->children.computeCopyOnInstance(context);	
		// propagate flag from from to to routes
		if (node->routes.computeCopyOnInstance(context))  {
			node->copyOnInstance = TRUE;	
		} 
	}

	{ GvCopyContext context(NULL);
    
		node->computeCopyOnInstance(context);

		// propagate any changes, resulting from routes || directOutput scripts
		if (node->children.computeCopyOnInstance(context)) {
			node->copyOnInstance = TRUE;	
		}    
	}


    // TRACE("Proto %s need to be instanced :%d \n",name.getString(),(int) (node->copyOnInstance != 0));

    // TO DO: what if duplicate protos
    if (node) { 
        in->addProto(name,node); // add proto to current namspace
        node->setName(name);
		if (in->currentProto == NULL) { // its a top-level proto in this scene  
			GvScene *scene = in->getScene(); // store it for EXTERNPROTO lookup
			if (scene) scene->addProto(node);
			else {
				TRACE("GvProto::read XXXXXXXXXXXXXXXXXX: No current scene to store proto ????\n");

			}
		}	
    }
  
    // TRACE("Proto %s read \n",name.getString());
	return(node);
}

// read the event IS protoEvent part
// event == fieldName


GvBool GvProto::readEventTranslation(
			GvInput *in, GvNode *node,
			const GvName &fieldName,
			int fieldIndex)
{
        GvFieldData *nodeFields = node->getFieldData();
        GvFieldEntry *nodeFieldEntry = nodeFields->getFieldEntry(fieldIndex);


		GvName is;
        if (!in->read(is, TRUE)) {
	          GvReadError::post(in,GV_RE_SYNTAX_ERROR, "Couldn't read IS source field/event field \"%s\"",fieldName.getString());
	          return FALSE;
        }


		GvName event;
            if (in->read(event, TRUE)) {
                // to do: event translation with IS
                GvProto *proto = in->currentProto;

                if (!proto) {
	                GvReadError::post(in, GV_RE_SYNTAX_ERROR, "IS %s only allowed in PROTO definition \"%s\"",event.getString());
	                //return FALSE;
	                return TRUE;
                }


				
                GvFieldData *protoFields = proto->getFieldData();
//                int srcFieldIndex= protoFields->getEventOutIndex(proto,event);
                int srcFieldIndex= protoFields->getFieldIndex(event);

                if (srcFieldIndex<0) {
	                GvReadError::post(in,GV_RE_SYNTAX_ERROR, "%s IS %s ,no such Event in PROTO interface",nodeFieldEntry->getName().getString(),event.getString());
	                //return FALSE;
	                return TRUE;
                }
            
                GvFieldEntry *srcFieldEntry = protoFields->getFieldEntry(srcFieldIndex);
                //if (!srcFieldEntry->isInput())) {
                //}
                if (!srcFieldEntry->matchFieldType(nodeFieldEntry->eventTypeId)) {
	                GvReadError::post(in, GV_RE_TYPE_ERROR, "%s IS %s ,type mismatch ",nodeFieldEntry->getName().getString(),event.getString());
	                //return FALSE;
	                return TRUE;
                }

				if(srcFieldEntry->type != GV_FIELD) {
					GvBool eventIn=srcFieldEntry->isInput() && nodeFieldEntry->isInput();	
					GvBool eventOut=srcFieldEntry->isOutput() && nodeFieldEntry->isOutput(); //xx 
					GvBool copyEventOut = srcFieldEntry->type == GV_FIELD_EVENT_OUT;
					// route 
					GvProtoEventIsConnection *is = new GvProtoEventIsConnection
						(proto,srcFieldIndex,fieldIndex,eventIn,eventOut,copyEventOut);
					node->addConnection(is);
				}

                // this node need to be copied on instancing, set copyOnInstance flag 
                node->copyOnInstance = TRUE;

            }
            else {
	            GvReadError::post(in,GV_RE_SYNTAX_ERROR, "Couldn't read IS source field/event field \"%s\"",nodeFieldEntry->getName().getString());
	            return FALSE;
            }
            return(TRUE);
}

/*
  process IS
	node fieldName IS protoEventName
*/
GvBool GvProto::DoIs(GvInput *in,GvNode *node,GvField *fieldxx,const GvName &fieldName,GvName &event)
{
		GvFieldData *nodeFields = node->getFieldData();
		int fieldIndex = nodeFields->getFieldIndex(fieldName);
        GvFieldEntry *nodeFieldEntry = nodeFields->getFieldEntry(fieldIndex);

		GvProto *proto = this;
				
        GvFieldData *protoFields = proto->getFieldData();
				//  int srcFieldIndex= protoFields->getEventOutIndex(proto,event);
                int srcFieldIndex= protoFields->getFieldIndex(event);

                if (srcFieldIndex<0) {
	                GvReadError::post(in,GV_RE_SYNTAX_ERROR, "%s IS %s ,no such Event in PROTO interface",nodeFieldEntry->getName().getString(),event.getString());
	                //return FALSE;
	                return TRUE;
                }
            
                GvFieldEntry *srcFieldEntry = protoFields->getFieldEntry(srcFieldIndex);
                //if (!srcFieldEntry->isInput())) {
                //}
                if (!srcFieldEntry->matchFieldType(nodeFieldEntry->eventTypeId)) {
	                GvReadError::post(in, GV_RE_TYPE_ERROR, "%s IS %s ,type mismatch ",nodeFieldEntry->getName().getString(),event.getString());
	                //return FALSE;
	                return TRUE;
                }

				// src FIELD/exposedField destination field/exposedField
				// to think use already sent values ?
				// 
				if ((nodeFieldEntry->type == GV_FIELD || nodeFieldEntry->type == GV_EXPOSED_FIELD)
					&& (srcFieldEntry->type == GV_FIELD || srcFieldEntry->type == GV_EXPOSED_FIELD) //xx
					)  {
					// need to copy initial value on INSTANCING 
					GvProtoFieldIsConnection *is = new GvProtoFieldIsConnection
					(proto,srcFieldIndex,fieldIndex,srcFieldEntry->isInput());
					// this node need to be copied on instancing, set copyOnInstance flag 
					node->addConnection(is);

				}
				if(srcFieldEntry->type != GV_FIELD) {
					GvBool eventIn=srcFieldEntry->isInput() && nodeFieldEntry->isInput();	
					GvBool eventOut=srcFieldEntry->isOutput() && nodeFieldEntry->isOutput(); //xx 
					GvBool copyEventOut = srcFieldEntry->type == GV_FIELD_EVENT_OUT;
					// route 
					GvProtoEventIsConnection *is = new GvProtoEventIsConnection
						(proto,srcFieldIndex,fieldIndex,eventIn,eventOut,copyEventOut);
					node->addConnection(is);
				}
				// this node need to be copied on instancing, set copyOnInstance flag 
                node->copyOnInstance = TRUE;

            return(TRUE);

}




//
// GvExternProto
//

GV_NODE_SOURCE_IMP(GvExternProto,GvProto);

GvExternProto::GvExternProto()
{
    GV_NODE_CONSTRUCTOR(GvExternProto);
}

GvExternProto::~GvExternProto()
{
    // proto will be unrefed
    // fields will be deleted
}

// create a prototype instance of this prototype, check for native node URN's
GvNode *GvExternProto::createInstance()
{
	// check for special URN's first to create native nodes 
	for (int i=0; i<url.getNum(); i++) {
		const char *s = url.get1(i);

		// list of supported hardcoded URN's 

		int l=strlen(URN_NODE);
		if (strncmp(s,URN_NODE,l) == 0) {// we found our URN for nodes 
			const char *s1=	&s[l];
			int l1	=6;
			int vrml2 = 1;
			if (strncmp(s1,"vrml1:",l1) == 0) {
				s1 += 6;
				vrml2=0;
			}

			GvName nodeName(s1);
			// TRACE("Externproto is native node %s \n",nodeName.getString());
			GvNode *n= GvNode::createInstanceFromName(nodeName,vrml2);
			if (n) return n;
		}

		l=strlen(URN_NODE1);
		if (strncmp(s,URN_NODE1,l) == 0) {// we found our URN for nodes 
			const char *s1=	&s[l];
			int l1	=6;
			int vrml2 = 1;
			if (strncmp(s1,"vrml1:",l1) == 0) {
				s1 += 6;
				vrml2=0;
			}

			GvName nodeName(s1);
			// TRACE("Externproto is native node %s \n",nodeName.getString());
			GvNode *n= GvNode::createInstanceFromName(nodeName,vrml2);
			if (n) return n;
		}
	}

    return(new GvProtoInstance(this));
}



// get absolute home url
const char *GvExternProto::GetHomeUrl() const
{
	// has GetHome just been setup ?????
	return(loader.GetHomeUrl());
}


int 			
GvExternProto::Load(GTraversal &state)
{
	// whats the current status
	GFileState status = loader.GetState();

Redo:
	switch(status) {
	case S_BAD_DATA:
	case S_BAD_URL:
		if (loader.urlI< (url.Length() -1) ) {
			loader.urlI ++;
			// fall through and load  next 
		} else {
			status = loader.state = S_ALL_BAD; // can´t load any URL 
			GView *browser = (GView *) getBrowser();
			if (browser) browser->UnRegisterPending(this);
		    break;
		}
	case S_NOT_LOADED:
	     if (url.Length() == 0) { 
			 loader.SetNoUrl();		
			 GView *browser = (GView *) getBrowser();
			 if (browser) browser->UnRegisterPending(this);
			 return(0);
		 }
		 {
		 CString extraArg;
		 CTime refreshTime(0);
		 const char *refedFromUrl= state.GetUrl(refreshTime);	 
		 
		 status = loader.SetupUrl(baseUrl /* state.GetHomeUrl()*/,url[loader.urlI].getString(),refedFromUrl,"EXTERNPROTO",state.inlineWwwFlags,state.view->hWnd,loader.urlI,state.view->GetReporter(),state.view->file,extraArg);
		 
		 externProtoName = extraArg;

			 
		 if (( status <= S_LOADED) && (status != S_NOT_LOADED)) goto Redo; // download request already in progress

		 if (status == S_NOT_LOADED)
		 {  // url resolved to absolute // check in proto cache
				GFile *theFile = loader.loader;
				
				theFile->ref();

				// check if Extern Proto already loaded
				GView *browser = (GView *) getBrowser();
				if (state.view) browser = state.view;
				if (browser) { 
					this->proto = browser->getProto(theFile->GetUrl(),externProtoName);


					if (this->proto) {
						TRACE("Found Proto already in cache %s %s ",theFile->GetUrl(),(LPCTSTR) externProtoName);
						theFile->unref();
						loader.SetProcessed();
						browser->UnRegisterPending(this);
						return(1);
					}
				}
				theFile->unref();

				loader.Release();

		 }  	
		 status = loader.Load(baseUrl /* state.GetHomeUrl()*/,url[loader.urlI].getString(),refedFromUrl,"EXTERNPROTO",state.inlineWwwFlags,state.view->hWnd,loader.urlI,refreshTime,state.view->GetReporter(),state.view->file);

		 {
		 GView *browser = (GView *) getBrowser();
		 if (state.view) browser = state.view;

		 if (browser) browser->RegisterPending(this);
		 }

		 if (status != S_NOT_LOADED) goto Redo;

		 }

		 break;

	case S_LOADING:
	case S_PROCESSING:
				state.pendingLoads ++;
				break;

	case S_LOADED: {
				int ret;

				GFile *theFile = loader.loader;
				theFile->ref();

				// check if Extern Proto already loaded
				GView *browser = (GView *) getBrowser();
				if (state.view) browser = state.view;
				if (browser) { 
					this->proto = browser->getProto(theFile->GetUrl(),externProtoName);

					if (this->proto) {
						TRACE("Parsing Proto already in cache %s %s ",theFile->GetUrl(),(LPCTSTR)externProtoName);

						loader.SetProcessed();
						theFile->unref();
						browser->UnRegisterPending(this);
						return(1);
					}
				}


		 		if (state.view) {
					state.view->lastFrameLoadCnt++;
					state.view->lastFrameInlineLoadCnt++;
				}


				GvScene *scene = new GvScene();
				scene->setBrowser(browser);
				
				scene->ref();
				scene->worldUrl.set(theFile->GetUrl());
	 		    scene->baseUrl.set(theFile->GetHomeUrl());


				//this->deleteAll(); // delete all contents 
				ret = ReadVrmlFile(loader.GetLocalFile(),state.view->GetReporter(),scene);

				if (ret>=0) { 
				
					// add loaded scene to browsers proto scene 
					//GView *browser = (GView *) getBrowser();
					if (browser) { 
						browser->addProtoScene(scene);
						this->proto = browser->getProto(theFile->GetUrl(),externProtoName);
					}
					if (!this->proto) {
						state.view->GetReporter()->Error("EXTERNPROTO:Can´t find proto '%s' in %s \n", (const char *)externProtoName,(const char *)loader.loader->GetUrl());
					}
					loader.SetProcessed();
					state.view->lastFrameLoadCnt++;
					state.view->lastFrameInlineLoadCnt++; // new 16.11.99 hg

				}
				else { 
					// try to delete 
					loader.DeleteCachedLocalFile();

					loader.SetBadData();
					if (state.view) state.view->lastFrameLoadErrorCnt++;
				}
				
				scene->unref();
				theFile->unref();
				browser->UnRegisterPending(this);

				return(1);
			   }
	case S_PROCESSED:
				return(1);
	default :
		break;

	}
	return(0);
}


 // get pointer to implementing proto
 GvProto *GvExternProto::getProto()  
 { 
	GvProto *p = (GvProto*) (GvNode *) proto;
	if (!p) return NULL;
	if (p == this) {
		ASSERT(0);
		return NULL;
	}
    return p->getProto(); 
}

 // get pointer to implementing proto, check for read of externProto
 GvProto *GvExternProto::getProto(GTraversal &state)  
 { 
	if (!loader.Ok()) {
		// if ((state.inlineLevel <= state.maxInlineLevel)	
		
		// we need the inline element to resolve URL´s !!!!!!!!!!!!!!!
		GvWWWInlineElement *current;
		current = (GvWWWInlineElement*) state.Get(GvWWWInlineElement::attrId);

		if (current && (state.loadProtos >= GTraversal::LOAD_IF_NEEDED)) {
			Load(state);
		}
		else {
		}
	}	

	if (loader.Ok()) { 
	}
	GvProto *p = (GvProto*) (GvNode *) proto;
	if (!p) return NULL;

	if (p == this) {
		ASSERT(0);
		return NULL;
	}
	// chaing down to linked EXTERNPROTOS
    return p->getProto(state);  
}



GvFieldData *GvExternProto::getFieldData() const 
{
    // check first if proto available
   // GvProto *proto= (GvProto*) (GvNode *) this->proto; // getProto();
   // if (proto) return proto->getFieldData();
   // no no, this causes problems in instancing 
	return(& ((GvExternProto *)this)->protoFieldData);
}

// reads name fieldList url 
GvNode*
GvExternProto::read(GvInput *in)
{   GvExternProto *node= NULL;
	GvBool ret;
	GvName name;

    // name
    if (! in->read(name, TRUE)) {
	    GvReadError::post(in,GV_RE_SYNTAX_ERROR, "EXTERNPROTO name expected");
		return(node);

    }
	node = new GvExternProto();

	if (node == NULL ) {
	    GvReadError::post(in,GV_RE_OUT_OF_MEMORY, "out of memory");
		return(node);
    }
	
	node->setClassName(name);

	// for savety set browser member 
	if (in->getScene()) node->setBrowser(in->getScene()->getBrowser());

	
	node->setClassName(name);

    // fields
    if (! node->protoFieldData.readProtoInterface(in, node,TRUE)) {
		GvReadError::post(in,GV_RE_SYNTAX_ERROR, "Bad interface specifications for PROTO %s",name.getString());
		delete node;
		return NULL;
	}


	// read url 
	ret = node->url.read(in,"url");

    if (!ret) {
		delete node;
		node = NULL;
        return(NULL);
	}


    // TO DO: what if duplicate protos
    if (node) { 
        if (!in->addProto(name,node)) { // add proto to current namspace
			GvReadError::post(in, GV_RE_SYNTAX_ERROR,"Warning: duplicate EXTERNPROTO in context %s",name.getString());

		}
        node->setName(name);

		GvScene *scene = in->getScene();
		if (scene) 	node->baseUrl = scene->baseUrl.get(); // set the baseUrl for resolving the EXTERNPROTO url's

		if (in->currentProto == NULL) { // its a top-level proto in this scene  
			if (scene) scene->addProto(node);
			else {
				TRACE("No current scene to store proto ????\n");
			}
		}
    }
  
    TRACE("Extern Proto %s read \n",name.getString());
	return(node);
}






//
// GvProtoInstance
//

GV_NODE_SOURCE_IMP(GvProtoInstance,GvNode);

GvProtoInstance::GvProtoInstance() : children(0)
{
    GV_NODE_CONSTRUCTOR(GvProtoInstance);
    instanced = FALSE;
	fieldMap = NULL;

}

GvProtoInstance::GvProtoInstance(GvProto *proto) : children(0)
{
    GV_NODE_CONSTRUCTOR(GvProtoInstance);
    instanced = FALSE;
	fieldMap = NULL;
  
    ASSERT(proto != NULL);
    
	this->proto = proto;

    instanceFields();

}


// get node Proto evals to

GvNode*
GvProtoInstance::Eval()
{
    // ???? if (!instanced) Instance();
    if (!instanced || children.Length() == 0) return NULL;
	GvNode *n = children[0];
	if (!n) return NULL;
    return(n->Eval()); // could be proto again 
}

GvNode*
GvProtoInstance::Eval(GTraversal &state)
{
    if (!instanced) Instance(state);
    if (!instanced || children.Length() == 0) return NULL;
	GvNode *n = children[0];
	if (!n) return NULL;
    return(n->Eval(state)); // could be proto again 
}

// create fields according to protoFieldData
// see also GvUnknownNode
void GvProtoInstance::instanceFields()
{
    fields.DeleteFields();
    GvFieldData *fieldData = getFieldData();

    // instance all fields / events and assign default valus
    int numFields;
    fields.setSize(numFields = fieldData->getNumFields());
    for (int i=0; i<numFields; i++) 
    {
        GvField *field;
        fields[i]= field = fieldData->getFieldEntry(i)->createInstance();
        if (field) field->setContainer(this); // will set setDefault(TRUE);
    }

}


const char *GvProtoInstance::getClassName() const
{
	GvProto *p= (GvProto *) (GvNode *) proto;
	return(p->getClassName());
}


GvProtoInstance::~GvProtoInstance()
{
    // proto will be unrefed
    // fields will be deleted
	if (fieldMap) delete []fieldMap;
	fieldMap = NULL;
}


// delete children
void GvProtoInstance::deleteAll()
{
	children.deleteAll();
	instanced = FALSE;
}

GvFieldData *GvProtoInstance::getFieldData() const 
{
    GvProto *p=((GvProto*) (GvNode *) proto); 
	return p->getFieldData(); // would be fieldData of externproto 

//	return getProto()->getFieldData();
}

// create a new instance of instance, copy proto field and instance fields
GvNode * GvProtoInstance::newInstance()
{
	GvProtoInstance *n = (GvProtoInstance *) GvNode::newInstance();
	n->proto = proto;
    n->instanceFields();
	return(n);
}



GvField *
GvProtoInstance::getField(int index) const
{
    ASSERT(index>=0 && index < fields.getLength());
	if (index >= fields.getLength()) return (NULL);
    return(fields[index]);
}

// get the fieldIndex in ProtoInstance fieldlist 
int GvProtoInstance::MapProtoField(int protoFieldIndex)
{
    if (!fieldMap) return(protoFieldIndex);
	return(fieldMap[protoFieldIndex]);
}



#if 0

GvNode *GvProto::CopyAndInstanceIsFields(node)
{ GvNode *newNode = NULL;
  //check all fields for IS keyword
  //call instanceNod on SFNode MFNode fields
  return(NULL);  
}

#endif

// Instance an MFNode field
void GvProtoInstance::Instance(GvCopyContext &context,GvMFNode *src,GvMFNode *dest)
{   int n= src->Length();

	int old = dest->enableConnections(0);

    dest->makeRoom(n);
    for(int i=0; i<n;i++) 
        dest->values[i]= InstanceNode(context, src->values[i]);
	
	if (src->isDefault()) // propagate default bit 10.11.99 hg
		dest->setDefault(TRUE);

	if (n>0) { 
		// dest->setDefault(FALSE); // del 10.11.99 hg

		dest->OnChanged(); // do we want events now = 4.0.97
    }
	dest->enableConnections(old);

}

// Instance an SFNode field
void GvProtoInstance::Instance(GvCopyContext &context,GvSFNode *src,GvSFNode *dest)
{   
	int old = dest->enableConnections(0);
    dest->set(InstanceNode(context, src->value));
	//if (dest->value != (GvNode*) NULL) dest->setDefault(FALSE); del 10.11.99 hg
	if (src->isDefault()) // propagate default bit 10.11/99 hg
		dest->setDefault(TRUE);
	dest->enableConnections(old);

}


// instance proto node tree
GvNode*
GvProtoInstance::InstanceNode(GvCopyContext &context,GvNode *node)
{   GvNode *newNode = NULL;
    
    if (node == NULL) return(NULL);

    // node already translated ????, return the already translated instance
    if ( (newNode = context.get(node)) != NULL) {
        return(newNode);
    }

	/*
	if (RTISA(node,GvScript)) {
		TRACE("Instancing script %s \n",node->getName().getString());
	}
*/
/*

	if (RTISA(node,GvProtoInstance)) {
		TRACE("Instancing proto instance  %s \n",node->getName().getString());
	}
*/

    if (!node->copyOnInstance) 
        return(node);

/*
	if (RTISA(node,GvProtoInstance)) {
		TRACE("Creating copy of proto instance  %s \n",node->getName().getString());
	}

*/
    // if node has any IS expression
    // create a copy with IS expanded

	
	newNode = node->newInstance(); // create instance

    ASSERT(newNode);
    if (!newNode) return(newNode);

	node->ref();
	newNode->ref(); // keep a ref 

	newNode->setBrowser(context.getBrowser());
	
	ASSERT(newNode->getBrowser() != NULL);


    // define translated node in context 
    context.put(node,newNode);


    GvFieldData *fields = node->getFieldData();

    // for all fields 
    for (int i = 0; i < fields->getNumFields(); i++) {
        GvField *field = node->getField(i);
        
        if (!field) continue;

        // get and evaluate IS field connection

        GvConnection*c=  field->getConnections();
        GvBool isConnected = FALSE;
        while (c && !isConnected) {

            if (RTISA(c,GvProtoIsConnection)) {
                // assign field with argument 
                GvProtoIsConnection *cIs = (GvProtoIsConnection *) c;
                GvField *newField = newNode->getField(i);
				GvField* srcField  = cIs->eval(this);
				if (srcField) {
					
					int old = newField->enableConnections(0);
						newField->set(srcField);
					newField->enableConnections(old);

					isConnected = TRUE;
				}
				else {
					// srcField is not defined in ProtoInstance,
					// get default value from proto field data
				}

            }    
            c=c->next;
        }
        
        if (!isConnected) { // copy original field value from node to newNode

        if (RTISA(field,GvMFNode)) {
            Instance(context,(GvMFNode*) field, (GvMFNode*) newNode->getField(i));
        }
        else if (RTISA(field,GvSFNode)) {
            Instance(context,(GvSFNode*) field, (GvSFNode*) newNode->getField(i));
        }
        else {
            GvField *newField = newNode->getField(i);
			ASSERT(newField != NULL);
			if (newField) {
				int old = newField->enableConnections(0);
					newField->set(field);
				newField->enableConnections(old);

			}
		}
        } // (!isConnected)

    } // for fields

		// connections of originial Node
        // add routes for IS event connections

        GvConnection*c=  node->getConnections();
        GvBool isConnected = FALSE;
        while (c  /* && !isConnected why */ ) {

            if (RTISA(c,GvProtoEventIsConnection)) {
                GvProtoEventIsConnection *cIs = (GvProtoEventIsConnection *) c;
				cIs->instanceRoute(this,newNode);
                isConnected = TRUE;
            }   
            else if (RTISA(c,GvProtoFieldIsConnection)) {
                GvProtoFieldIsConnection *cIs = (GvProtoFieldIsConnection *) c;
				cIs->copyValue(this,newNode);
            }   
            c=c->next;
        }


#if 0
    
    // recursively expand children
    if (node->HasChilds()) {
        int needCopy = 0;
        GvChildList newChilds;

        for(i=0; i<childs.Length() i++)) {
            GvNode * n=childs[i];
            GvNode newN = InstanceNode(n);
            if (n != newN) needCopy = 1;
            newChilds.Append(newN);
        }

        if (needCopy && node == newNode) {
            newNode = CopyNode(node);
        }
        newNode->SetChilds(newChilds);
    }

    // for beeing able to create instanced routes
    if (node!=newNode) AddToNodeTable(node,newNode);
#endif

	// extra stuff for some nodes 
	if (RTISA(newNode,GvScript)) {
		GvScript *s = (GvScript *)newNode;

		newNode->setName(node->getName()); // for debug only

		s->Load(); // parse the script now 
	}

	node->unrefNoDelete(); // caller takes care 
	newNode->unrefNoDelete(); // caller takes care 

    return(newNode);

}

#include <gvtexture2.h>

// instance the routes 
GvBool 
GvProtoInstance::InstanceRoute(GvCopyContext &context, GvRoute *route)
{
	ASSERT(route->from.value->refCnt >1);
	ASSERT(route->to.value->refCnt >1);

    GvNodeHandle src = InstanceNode(context, route->from.value);
    GvNodeHandle dest = InstanceNode(context, route->to.value);

	int fromIndex = route->fromEventIndex;

	//if (route->from.value == theProto)  fromIndex=MapEventIndex(fromIndex);
	
	int toIndex = route->toEventIndex;

	ASSERT(src->refCnt >1);
	ASSERT(dest->refCnt >1);
	
	// TRACE("Instancing PROTO Route  %s . %s TO %s %s \n",src->ClassName(), route->getFromEventName().getString(),dest->ClassName(),route->getToEventName().getString());

	// to do:event translation
	return addRoute(src,fromIndex, dest, toIndex);
}

// Instance an MFNode field containing routes
void GvProtoInstance::InstanceRoutes(GvCopyContext &context,GvMFNode *src)
{   int n= src->Length();
    GvBool ret;
    for(int i=0; i<n;i++) {
        GvNode* r= src->values[i];
#ifdef _DEBUG
        if (!RTISA(r,GvRoute)) TRACE("InstanceRoute: node is no route !");
#endif
        ret = InstanceRoute(context,(GvRoute *) r);
    }
}


// get proto this instance is a proto from
GvProto *GvProtoInstance::getProto(GTraversal &state) const 
{
  GvProto *p=((GvProto*) (GvNode *) proto); 
  if (!p) return(p);
 
  return p; 

}

// get proto this instance is a proto from
GvProto *GvProtoInstance::getDefiningProto(GTraversal &state) const 
{
  GvProto *p=((GvProto*) (GvNode *) proto); 
  if (!p) 
	  return(p);
  
  return p->getProto(state);

}

// get proto this instance is a proto from
GvProto *GvProtoInstance::getDefiningProto() const 
{
  GvProto *p=((GvProto*) (GvNode *) proto); 
  if (!p) 
	  return(p);
  
  return p->getProto();

}


// new 02.09.98 

GvBool
GvProtoInstance::computeCopyOnInstance(GvCopyContext &context) 
{
	// hg 12.11.98  Node fields might not be traversed yet
	// if (copyOnInstance) return copyOnInstance; // already computed 
	if (context.alreadySeen(this)) 
		return copyOnInstance;
	context.setSeen(this);


	GvProto *proto = getDefiningProto();
  
	if (proto) {  
		if (proto->copyOnInstance || RTISA(proto,GvExternProto)) // mm could be extern proto which must be instanced
			copyOnInstance = TRUE;
	}	else {
			copyOnInstance = TRUE; // EXTERNPROTO not loaded , copy for savety

	}
	

    // check any SFNode MFNode arguments 
	copyOnInstance = getFieldData()->computeCopyOnInstance(this,context) || copyOnInstance;


    return(copyOnInstance);
}



// Instance, create copies/refs of proto scene graph as needed 

GvBool 
GvProtoInstance::Instance(GTraversal &state) 
{
  instanced = TRUE; // 

  // if reinstancing 
  // to do: delete routes from fields into children 
  children.deleteAll();


  if (fieldMap) {
	  delete []fieldMap;
	  fieldMap  = NULL;
  }
  // to do: delete routes from field int0 prot 

  GvProto *proto = getDefiningProto(state);
  
  if (proto) {                  // proto->valid) 

	  // TRACE("%p:%s Instancing Proto %s (%d) \n",this,this->getName().getString(),proto->getName().getString(),state.view->lastFrameProtoInstanceCnt);
	  
	  if (proto != (GvNode *) this->proto) { 
		  // instancing of an externproto

		 ASSERT(!RTISA(proto,GvExternProto));

		 int j;

		 int n = proto->getNumFields();
		 GvFieldData *fieldData = getFieldData(); // this is the fieldData of EXTERN PROTO proto
		 GvFieldData *protoFieldData = proto->getFieldData(); // this is the field data of the defining proto
				
		 int numProtoFields= protoFieldData->getNumFields();
		 int numFields = fieldData->getNumFields();

		 fieldMap = new int[numProtoFields]; // mapping of Proto fields  x to fiekd y 

		 for (j=0; j<numProtoFields; j++) fieldMap[j] = -1; 

		 int fieldi;

		 // for all fields declard in proto
		 for (j=0; j<numProtoFields; j++) {
			GvBool found=FALSE;
			GvFieldEntry *protoEntry =protoFieldData->getFieldEntry(j); 

			// search proto field in extern proto field list 
			for (int i=0; i<numFields; i++) 
			{
				GvFieldEntry *f = fieldData->getFieldEntry(i);

				if (protoEntry->name == f->name) 
				{
					if (protoEntry->type == f->type) {
						if (protoEntry->eventTypeId == f->eventTypeId) {
							found = TRUE;
							fieldi= i;
							break;
						}
					}
					Reporter.Warning("ExternProto %s field type mismatch for %s",proto->getName().getString(),protoEntry->name.getString());
				}
			} 

			if (!found) { // field not found 
				#if 0
				//add to proto Instance, not ok, because there might be several ProtoInstance nodes
				GvField *field;
			    fieldi = fieldData->getNumFields();
				fieldData->addField(new GvFieldEntry(*protoEntry));
				this->fields.append(field = protoEntry->createInstance());
				if (field) field->setContainer(this); // will set setDefault(TRUE);
				numFields ++;
				#else
				fieldi = -1;
				#endif

			}
			else {
				GvField *f = getField(fieldi);
				if (f && f->isDefault() && (protoEntry->defaultValue)) { // value not specified
					// take default value from PROTO spec 
					int old = f->enableConnections(0);
						f->set(protoEntry->defaultValue);	  // should not trigger events !
						f->setDefault(TRUE);
					f->enableConnections(old);
				}
			}
			fieldMap[j] = fieldi;
		 }	
	
	  }	

      GvCopyContext context(state.view);
      
      // Instance the children
      Instance(context,&proto->children,&children);

      // instance the routes 
      InstanceRoutes(context,&proto->routes);
	  
	  if (state.view) { 
		  state.view->lastFrameProtoInstanceCnt++;
		  // register certain new node, to avoid full scene info update
		  for(int i=0; i<children.getNum();i++) {
			  GvNode *n=children.values[i];
			  state.view->RegisterNode(n);
		  }		
	  }
	
      instanced = TRUE;
      return(TRUE);

  } else {
      instanced = FALSE;
      return(FALSE);
  }  
}

int GvProtoInstance::OnChanged()
{
	// instanced = FALSE;
	// this should not happen for normal events
	//  TRACE("Forcing reinstancing of ProtoInstance !!!!!!!!!!!!!!!!!!!!!!");

	// called from  normal events !!!

   return(1);
}



int 
GvProtoInstance::Do(GTraversal &state)
{	int ret = 0;
	if (state.traverseAll) {
		if (state.level >= state.maxLevel) 
			return -1;
		state.Push();
#if 0
		// place externproto in context 

		if (RTISA(((GvNode*) proto),GvExternProto)) {
			GvWWWInlineElement *current,*prev;
			prev = (GvWWWInlineElement*) state.Get(GvWWWInlineElement::attrId);
			if (!prev) {
			} else {
				current = (GvWWWInlineElement *) prev->NewInstance();								
				current->data = proto;
				state.Push(GvWWWInlineElement::attrId,current);			
			}
		}
#endif
		state.level ++;	


		if (!instanced)  
			Instance(state);

		if (!state.traverseTransformHierarchyOnly)
			ret = TraverseAllNodeFields(state);

	    ret = children.Traverse(state);

		state.level --;	

		state.Pop();

	} else {

		if (!instanced)  
			Instance(state);
		ret = children.Traverse(state);
	}
	return(ret);
}

int GvProtoInstance::Do(GPathTraversal &state)
{	int ret = 0;

	if (state.traverseAll) {
		if (state.level >= state.maxLevel) 
			return -1;
		
		if (state.nodeDict) {
			void *node;
			if (state.nodeDict->find((u_long) this, node)) {
  				return(GV_NODE_ALREADY_SEEN);
			}
		    state.nodeDict->enter((u_long) this, (void *) this);
		}



		{ // recursive check 
			GPathElement *p = state.topPath;
			while (p != NULL) {
				if (p->node == this) {
					TRACE("Recursive entry in %s \n",(const char *) this->getName().getString());
					return -1;
				}
				p = p->next;
			}
		}

		GPUSH_PATH(state,this);

		state.Push();
		// place externproto in context 
#if 0
		if (RTISA(((GvNode*) proto),GvExternProto)) {
			GvWWWInlineElement *current,*prev;
			prev = (GvWWWInlineElement*) state.Get(GvWWWInlineElement::attrId);
			if (!prev) {
			} else {
				current = (GvWWWInlineElement *) prev->NewInstance();								
				current->data = proto;
				state.Push(GvWWWInlineElement::attrId,current);			
			}
		}
#endif
		state.level ++;	
//		TRACE("(PROTO Instance %s Level %d \n", (const char *) this->getName().getString(),state.level);

		if (!instanced)  
			Instance(state);

		//fieldGraph++;
		if (!state.traverseTransformHierarchyOnly) {
			state.notInTransformHierarchy++;
			ret = TraverseAllNodeFields(state);
			state.notInTransformHierarchy--;
		}
		//fieldGraph--;

	    ret = children.Traverse(state);

//		TRACE(")");
		state.level--;

		state.Pop();
		GPOP_PATH(state);

	} else {
		GPUSH_PATH(state,this);

		if (!instanced)  
			Instance(state);
		ret = children.Traverse(state);
		
		GPOP_PATH(state);
	}

	return(ret);
}

//
// GvRoute
//

GV_NODE_SOURCE_IMP(GvRoute,GvNode);

GvRoute::GvRoute() : fromEventIndex(-1),toEventIndex(-1),toInitialValue(NULL)
{
    GV_NODE_CONSTRUCTOR(GvRoute);
    GV_NODE_ADD_FIELD(from);
    GV_NODE_ADD_FIELD(fromEventIndex);
    GV_NODE_ADD_FIELD(to);
    GV_NODE_ADD_FIELD(toEventIndex);
}

GvRoute::~GvRoute()
{
    //  deinstance route 
	if ((from != (GvNode *) NULL)  && (to != (GvNode * ) NULL)) {

		if ( ( ((GvNode *)from)->refCnt == 1) ||  (((GvNode *)to)->refCnt == 1)) { // only one ref from us
			GvBool ret = GvNode::deleteRoute(from,fromEventIndex,to,toEventIndex);
			if (ret) {
				//TRACE("Deleted route in ~GvRoute \n");
			}	
		}
	}

	if (toInitialValue) delete toInitialValue;
}

void GvRoute::SaveInitialValue() 
{
	TRACE("TO DO GvRoute :\n");
}

void GvRoute::RestoreInitialValue()
{
	TRACE("TO DO GvRoute :\n");
}


GvBool 
GvRoute::set(GvNode * From,int FromIndex,GvNode * To,int ToIndex)
{
    from.set(From);
    to.set(To);
    fromEventIndex.set(FromIndex);
    toEventIndex.set(ToIndex);
    return(TRUE);
}

GvBool 
GvRoute::match(GvNode * From,int FromIndex,GvNode * To,int ToIndex) const
{
    return from == From
		&& to == To
		&& fromEventIndex == FromIndex
		&& toEventIndex == ToIndex;
}

GvBool 
GvRoute::operator ==(const GvRoute &b) const
{
    return from == b.from
		&& to == b.to
		&& fromEventIndex == b.fromEventIndex
		&& toEventIndex == b.toEventIndex;
}

// set by event names, return FALSE on mismatch
GvBool GvRoute::set(GvNode * from, const char* fromEvent, GvNode * to,const char * toEvent,GvBool verbose)
{

	if (!from || !to) return FALSE;

    GvFieldData* fromData = from->getFieldData();
    GvFieldData* toData = to->getFieldData();

    int fromIndex = fromData->getEventOutIndex(from,fromEvent);
    if (fromIndex<0) {
       if (verbose) Reporter.Error("ROUTE FROM %s . %s ,no such eventOut",from->getName().getString(),fromEvent);
	   return FALSE;
    }

    GvFieldEntry *fromFieldEntry = fromData->getFieldEntry(fromIndex);

    int toIndex = toData->getEventInIndex(to,toEvent,fromFieldEntry->eventTypeId);
    if (toIndex<0) {
       if (verbose) Reporter.Error( "ROUTE TO %s . %s, no eventIn of type %s ",to->getName().getString(),toEvent,fromFieldEntry->getType());
	   return FALSE;
    }

    GvFieldEntry *toFieldEntry = toData->getFieldEntry(toIndex);

	if (!(fromFieldEntry->fieldClass == toFieldEntry->fieldClass)) {
       if (verbose) Reporter.Error( "ROUTE TO %s . %s, no eventIn of type %s ",to->getName().getString(),toEvent,fromFieldEntry->getType());
	   return FALSE;
	}


	return(set(from,fromIndex,to,toIndex));
}



GvBool
GvRoute::computeCopyOnInstance(GvCopyContext &context) 
{
    GvNode::computeCopyOnInstance(context);
	GvNode *src = from.value;

	// certain nodes must be copied
	if (RTISA(src,GvSensor)) {
		if (!RTISA(src,GvTimeSensor)) // may have already time
			src->setCopyOnInstance(TRUE); 
	}
	else if (RTISA(src,GvScript)) { // script sends arbitrary outputs 
		src->setCopyOnInstance(TRUE);
	}

	// new 2.09.98
	else if (RTISA(src,Gv2Collision)) { //might be instanced in several transforms ??
		src->setCopyOnInstance(TRUE);
	}

#if 0
	// done in nodes:computeCopyOnInstance directly
	else if (RTISA(src,Gv2Billboard)) { //might be instanced in several transforms ??
		src->setCopyOnInstance(TRUE);
	}
// new 2.09.98
	else if (RTISA(src,GvSpotLight)) { //might be instanced in several transforms ??
		src->setCopyOnInstance(TRUE);
	}
	else if (RTISA(src,GvPointLight)) { //might be instanced in several transforms ??
		src->setCopyOnInstance(TRUE);
	}
	else if (RTISA(src,Gv2LOD)) { //might be instanced in several transforms ??
		src->setCopyOnInstance(TRUE);
	}
#endif

    // if source node, sending events needs a copy, dest node need to be copied too
    if (from.value->getCopyOnInstance()) {
//        to.value->setCopyOnInstance(TRUE);
        to.value->copyOnInstance = TRUE;
        copyOnInstance = TRUE;	 // this route must instanced each time
    }

    if (to.value->getCopyOnInstance()) { // new 4.04.97
        copyOnInstance = TRUE;	 // this route must instanced each time
    }

	// allways 
    //to.value->copyOnInstance = TRUE;
    return(copyOnInstance);
}


// is this route in use  ?
GvBool GvRoute::isUnused()
{
	if (from.value->refCnt == 1) return TRUE;
	if (to.value->refCnt == 1) return TRUE;
	return FALSE;

}

GvName GvRoute::getFromEventName() const
{
    return from.value->getEventOutName(fromEventIndex.value);
}

GvName GvRoute::getToEventName() const 
{
    return to.value->getEventInName(toEventIndex.value);
}


