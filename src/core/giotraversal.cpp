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

@module GioTraversal.cpp - GLView ouput traversal routines	|

Copyright (c) 1995,1996	by Holger Grahn
All rights reserved

Purpose:
implementation of all 
	int **NODE**::Do(GioTraversal &state) 
methods

Classes:
GioTraversal
GPovIoTraversal

Notes:

Changes:

Todo :

******************************************************************************/

#include "stdafx.h"

#include "gclass.h"
#include "gshell.h"
#include "ggeom.h"

#include "greporter.h"
#include "gversion.h"


#include <gvnodes.h>
#include <gvunknownnode.h>
#include <gvproto.h>
#include <gvscript.h>
#include <gvscene.h>

#include <gvmfnode.h>
#include <gvelement.h>
#include <gvkeyword.h>


#include "www.h"

#include "giotraversal.h"


//
// class GioTraversal
//
RTIMP(GioTraversal,0,GTraversal);


GioTraversal::GioTraversal(const char *FileName,const char *mode)
{
	writeInlines=0;
	updateInlineBbox=1;
	writeAllFields = 0;
	format = 0;
	version = 1.0;
	vrml2 = 0;
	minimizeSize=0;

 	astream = new AStream(FileName,mode);
	refDict = new GvDict();
	protoDict = new GvDict();
	currentProto = NULL;


	// put VRML defaults
 	format = VRML_FMT;

	astream->fmt = IV_FMT; // for point format
    astream->SetBlockSep('{','}');
	astream->SetListSep('[',']',',');
	astream->SetComment('#');

	// need to traverse all nodes
	traverseAll= 1;

};

GioTraversal::~GioTraversal()
{
	 if (astream) delete astream;
	 astream = 0;
	 
	 if (refDict) delete refDict;
	 refDict = 0;
	 if (protoDict) delete protoDict;
	 protoDict = 0;

};


int GNode::Do(GioTraversal &state) 
{ // to do:
  return io(*state.astream); 
} 

int GioTraversal::DoFileHeader()
{
 	if (astream->isOutput()) {
		int ret;
		if (vrml2) 
				ret = astream->comment("VRML V2.0 utf8");
		else ret = astream->comment("VRML V1.0 ascii");
		//astream->comment("written by " _PROGRAM _PROGRAM_TYPE " " _VERSION);

		return ret;
 	}	
	else  return(0);
}
// write out node class of node
int GioTraversal::DoNodeName(GvNode * node)
{   // to do:
	return(0);
}


int GioTraversal::Do(const GvString &name)
{
 	if (astream->isOutput()) {
		return(astream->w(name.getString()));
 	}	
	else  return(0);
}

int GioTraversal::Do(const char * name)
{
 	if (astream->isOutput()) {
		return(astream->w(name));
 	}	
	else  return(0);
}

int GioTraversal::DoString(const GvString &name)
{
 	if (astream->isOutput()) {
		return(astream->wstr(name.getString()));
 	}	
	else  return(0);
}




int GioTraversal::Do(const GvName &name)
{
 	if (astream->isOutput()) {
		return(astream->w(name.getString()));
 	}	
	else  return(0);
}


// GioTraversal

// write out a "fields" field with all fieldnames and values
int GvFieldData::DoFieldTypes(GioTraversal &state,const GvNode *object)
{
	
	//if (state.astream->iotok("fields")) 
    state.astream->nl();

	if (state.astream->tok("fields")) 
	if (state.astream->BeginListTab()) {

    for (int i = 0; i < fields.getLength(); i++) {
	
		if (i>0) {
			state.astream->ListSep();
		}

		if ( (i % 5) == 4) // to do: do proper line wrap
			state.astream->nl();

		// write fieldType

	    const char *type = getFieldType(object, i);
	    state.astream->w(type);

		// write fieldName
		const GvName & fni = getFieldName(i);
		state.Do(fni);

		
	}
	return (state.astream->EndListTab());

	}

	return(0);
}

int 
GvField::Do(GioTraversal &state)
{
TRACE("%s::Do(GioTraversal &state) not implemented \n");
return(0);
}

// get IS connection for field 
static GvProtoIsConnection* getIsConnection(GvField *f)
{
   GvConnection *c = f->getConnections();
   while (c!= NULL) {
	   if (RTISA(c,GvProtoIsConnection)) {
			  return (GvProtoIsConnection *) c;
			  break;
	   }
	   c= c->next;
	}
   return NULL;
}

// get ProtoEvent IS connection for field 
static GvProtoEventIsConnection* getIsEventConnection(GvNode *object,int index)
{
   GvConnection *c = object->getConnections();
   while (c!= NULL) {
	   if (RTISA(c,GvProtoEventIsConnection)) {
		  GvProtoEventIsConnection *is = (GvProtoEventIsConnection *) c;
		  if (index == is->nodeIndex) return(is);
	   }
	   c= c->next;
   }
   return NULL;
}


// write all IsConnections of node 
int GvFieldData::DoIsConnections(GioTraversal &state, GvNode *object)
{
   GvConnection *c = object->getConnections();
   while (c!= NULL) {
	   if (RTISA(c,GvProtoEventIsConnection)) {
		  GvProtoEventIsConnection *is = (GvProtoEventIsConnection *) c;
		  state.Do(object->getFieldName(is->nodeIndex));
 		  state.Do(GvKeyword::is);	 
		  state.Do(is->proto->getFieldName(is->protoIndex));
		  state.astream->SetNl();
	   }
	   c= c->next;
   }
   return 1;
}

// write a single field including PROTO IS connections 
int 
GvFieldData::Do(GioTraversal &state, GvNode *object,int index)
{
	    GvFieldEntry *entry = getFieldEntry(index);
	    GvField *f = getField(object, index);
		GvProtoIsConnection *is = NULL;

		// check for IS connection for PROTOS 
		is = getIsConnection(f);
        
        switch (entry->type) {
        case GV_FIELD_EVENT_IN: // Don´t write these
        case GV_FIELD_EVENT_OUT:
        case GV_NODE_EVENT_IN:
				if (is == NULL) return 0 ;
                
        default : break;
        }


        if (f == NULL) {
            TRACE("GvFieldData::Do() Unexpected emtpy field pointer  ");    
            return 0;
        }    

		
		
		if (1 /*!f->isDefault()*/ || is ) {

            // write field name
			const GvName &fni = getFieldName(index);
			state.Do(fni);
	    
			if (is == NULL) {
				// write field value
	    		f->Do(state);
			} else {
				state.Do(GvKeyword::is);	 
				state.Do(is->proto->getFieldName(is->index));
			}
		
		}

	return(1);
}


// write all fields of node 
int 
GvFieldData::Do(GioTraversal &state, GvNode *object)
{
	int i;

	// count how many fields are written 
	int nFieldsToWrite=0;
	int nl=0;

    for (i = 0; i < fields.getLength(); i++) {
	    GvFieldEntry *entry = getFieldEntry(i);
	    GvField *f = getField(object, i);
        
        switch (entry->type) {
        case GV_FIELD_EVENT_IN: // Don´t write these
        case GV_FIELD_EVENT_OUT:
        case GV_NODE_EVENT_IN:
                continue;
        default : break;
        }


        if (f == NULL) {
            TRACE("GvFieldData::Do() Unexpected emtpy field pointer  ");    
            continue;
        }    
		
		if (!f->isDefault()) {
			nFieldsToWrite++;
			// nl=	 type == GvMFNode::ClassId()
		}
	}

	nl = nl || (nFieldsToWrite > 1);
	
	if (RTISA(object,Gv2Group) && (nFieldsToWrite>0) ) nl = 1;
	


	// now write values
    for (i = 0; i < fields.getLength(); i++) {
	
	    GvFieldEntry *entry = getFieldEntry(i);
	    GvField *f = getField(object, i);
		GvProtoIsConnection *is = NULL;
        
        switch (entry->type) {
        case GV_FIELD_EVENT_IN: // Don´t write these
        case GV_FIELD_EVENT_OUT:
        case GV_NODE_EVENT_IN:
                continue;
        default : break;
        }


        if (f == NULL) {
            TRACE("GvFieldData::Do() Unexpected emtpy field pointer  ");    
            continue;
        }    

		// check for IS connection for PROTOS 
		{
		   GvConnection *c = f->getConnections();
		   while (c!= NULL) {
			   if (RTISA(c,GvProtoIsConnection)) {
				  is=	(GvProtoIsConnection *) c;
				  break;
			   }
			   c= c->next;
			}
		}
		

		
		if (!f->isDefault() || is ) {

			if (nl) {
				state.astream->nl();
			}

            // write field name
			const GvName &fni = getFieldName(i);
			state.Do(fni);
	    
			if (is == NULL) {
				// write field value
	    		f->Do(state);
			} else {
				state.Do(GvKeyword::is);	 
				state.Do(is->proto->getFieldName(is->index));
			}
		
		}
	}

	if (nl) {
		state.astream->SetNl();
	}
	
	return(1);
}


// write interface for Proto, Externproto etc.
int 
GvFieldData::DoInterface(GioTraversal &state,const GvNode *object,GvBool writeDefaults,int offset)
{
    
    for (int i = offset; i < fields.getLength(); i++) {
	
	    GvFieldEntry *fieldEntry = getFieldEntry(i);
        const GvName &fni = fieldEntry->getName();
        const char *type = fieldEntry->getType();
        const char *fieldType = fieldEntry->getFieldType();

   		state.Do(fieldType);
   		state.Do(type);
   		state.Do(fni);
        GvField *fieldDefault = fieldEntry->getDefault();
        if ( /* writeDefaults &&*/ fieldDefault) { 
			// write field value
	    	fieldDefault->Do(state);
        }
		state.astream->nl();

	}
	return(1);
}

// write interface for Script 
// cloned from DoInterface 
int 
GvFieldData::DoScriptInterface(GioTraversal &state, GvNode *object,int offset)
{
    for (int i = offset; i < fields.getLength(); i++) {
	
	    GvFieldEntry *fieldEntry = getFieldEntry(i);
        const GvName &fni = fieldEntry->getName();
        const char *type = fieldEntry->getType();
        const char *fieldType = fieldEntry->getFieldType();
	    GvField *f = getField(object, i);

   		state.Do(fieldType); //EventIn
   		state.Do(type);		// SFBool 

		GvProtoEventIsConnection* is = getIsEventConnection(object,i);


        GvField *fieldDefault = fieldEntry->getDefault();
		if (is) {
	   	  state.Do(fni);				// Name 
 		  state.Do(GvKeyword::is);	    // IS 
		  state.Do(is->proto->getFieldName(is->protoIndex));
		} else 
        switch (fieldEntry->type) {
        case GV_FIELD: 	   // write field 
        case GV_EXPOSED_FIELD: 	   // write field 
			Do(state,object,i);
			break;
		default : 
			if (getIsConnection(f) != NULL) 
				Do(state,object,i); // write IS ..
			else 
	   		  state.Do(fni);
			break;
		}

		state.astream->SetNl();
	}
	return(1);
}

// generate a proto for node
int GioTraversal::DoProtoForNode(GvNode *n)
{
	GvName className(n->GetType());
	void *node;

	// check if PROTO with this name has been written
    if (protoDict->find((u_long) className.getString(), node)) {
  	    return(GV_NODE_ALREADY_SEEN);
    }

#if 0	
	// check if proto with nodes class name has been written
    if (refDict->find((u_long) className.getString(), node)) {
		return(GV_NODE_ALREADY_SEEN);
	}
#endif

	//EXTERNPROTO
    Do(GvKeyword::externProto); // n->DoNodeClass(state);
  	
	// NAME 
	Do(className); // n->DoNodeClass(state);
    
	// [ INTERFACE ]
	astream->BeginListTab();
		 astream->SetNl();
         n->getFieldData()->DoInterface(*this,n,FALSE); // write interface 
	astream->EndListTab();

	astream->SetNl();

	// URL [ ... ]
	GvString url;
	url = URN_NODE;
	url += className.getString();
	astream->Sep('[');
	astream->wstr(url);
	astream->Sep(',');

	url = URL_PROTOS;
	url += className.getString();
	astream->wstr(url);

	astream->Sep(',');
	url = "nodes.wrl#";
	url += className.getString();
	astream->wstr(url);

	astream->Sep(']');

	astream->SetNl();

  	//n->DoNodeBegin(*this);
            //ret = children.Traverse(state);
            //ret = routes.Traverse(state);
  	//n->DoNodeEnd(*this);

   // put written proto in dictonary 
    protoDict->enter((u_long) className.getString(), (void *) n);


	return(1);
}


int GvSField::Do(GioTraversal &state)
{
  ioValue(*state.astream);
  return(1);
}


int GvSFNode::Do(GioTraversal &state)
{
	// is this ok ????? fieldname. default  ??
	if (value) 
	{
		return(value->Traverse(state));
	}
	else 
	if (state.astream->IsOutput()) { return(state.astream->tok("NULL")); }
	
	return(0);

}


int GvMField::Do(GioTraversal &state)
{
 if (state.astream->IsInput()) {
 } else {
 		if (Length() == 0 ) {
		  state.astream->Sep(' ');
		  state.astream->BeginList();
		  state.astream->EndList();

		} else 
		if (Length() == 1 ) {
		   io1Value(*state.astream,0);
		} else {
		
		int curIndex=0;
		
		state.astream->Sep(' ');
		state.astream->BeginListTab();
		
		int elPerLine = 4;
		if (RTISA(this,GvMFNode)) elPerLine = 1;
		
		if ((Length() > elPerLine) || (elPerLine == 1)) state.astream->SetNl();

		while (curIndex < Length()) {
		 	if (curIndex>0) {
				state.astream->ListSep();
				if (curIndex % elPerLine == 0) state.astream->SetNl();
			}
		 	io1Value(*state.astream,curIndex);
			// to do: node dependent
			curIndex ++;
		}
		state.astream->EndListTab();
		}

 	return(1);

 }	// output

 
 return(1);
}


int GvMFNode::Do(GioTraversal &state)
{
 int ret=0;

 if (state.astream->IsInput()) {
 } else {
 		if (Length() == 0 ) {
		  state.astream->BeginList();
		  state.astream->EndList();
		  ret = 1;

		} else 
		if (Length() == 1 ) {
			GvNode *n = get1(0);
			if (n)
  				ret = n->Traverse(state);
			else 
				if (state.astream->IsOutput()) { ret = state.astream->tok("NULL"); }

		} else {
		
		int curIndex=0;
		state.astream->Sep(' ');

		state.astream->BeginListTab();

		
		while (curIndex < Length()) {
		 	if (curIndex>0) state.astream->ListSep();
			state.astream->SetNl();

			// state.astream->nl();
			GvNode *n = get1(curIndex);
			if (n) ret= n->Traverse(state);
			else  {
				if (state.astream->IsOutput()) { ret = state.astream->tok("NULL"); }
			}
			curIndex ++;
		}
		state.astream->SetNl();
		state.astream->EndListTab();

		}
	}
 	return(ret);
}

int GvMFNode::TraverseIo(GioTraversal &state)
{	int ret=0;
    for (int i = 0; i < getNum(); i++) {
		GvNode *n = get1(i);
		if (n) ret=n->Traverse(state);
		state.astream->SetNl();
	}
	return(ret);
}




int GvNode::Do(GioTraversal &state)
{
  if (DoNodeDef(state) == GV_NODE_ALREADY_SEEN) {
  	return(GV_NODE_ALREADY_SEEN);
  } else {

  	DoNodeClass(state);
  	DoNodeBegin(state);

  	DoNodeFields(state);
  
  	DoNodeEnd(state);
  }

  return(1);
}


// check if node is instanced multiple times (refCnt > 1)
// and create a name if need 

int GvNode::DoNodeDef(GioTraversal &state,GvBool writeProto)
{
  if (state.astream->IsOutput()) {

	  
	if (state.vrml2 &&  writeProto && !isBuiltIn) {
		// check if proto declaration needs to be written 
		state.DoProtoForNode(this);
	}


  if (!objName) { 
		return(0);

  }
  

  if ((refCnt >1) && (getName().getLength() == 0)) {
		// Reporter.Warning("Node Reference counter >1, but no name, generating a default name");
		char buf[255];
		sprintf(buf,"_%s",this->GetType());
		GvName newName(buf);
		this->setName(newName);
  }	


  const GvName &name = getName();
  if (name.getLength()>0) {

    void	*node;

	// check if node with this name has been written
    if (state.refDict->find((u_long) name.getString(), node)) {

		if (((GvNode *) node) == this) { // write out a reference
  			state.astream->tok("USE");
		  	state.Do(name);
			// state.astream->nl();			
			return(GV_NODE_ALREADY_SEEN);

		} else {
			//Reporter.Warning("Warning duplicate definition of node name '%s'",name.getString());
			int suffix = 1;
			char buf[260];
			// create a unique name by adding a suffix _###
			while (1) {
				sprintf(buf,"%s_%d",name.getString(),suffix);
				GvName newName(buf);
				if (!state.refDict->find((u_long) newName.getString(), node)) {
					TRACE("Created unique name %s \n",newName.getString());
					this->setName(newName);
					break; // so it will be defe´d 
				}
				suffix++;
			}
		}
	}



    const GvName &name1 = getName();

  	state.astream->tok("DEF");
  	//state.astream->tok(name1.getString());  	
  	state.Do(name1);

	// put written nodes in dictonary 
    state.refDict->enter((u_long) name1.getString(), (void *) this);

	return(1);

  }
  } // IsOuput
  return(0);
}


int GvNode::DoNodeFields(GioTraversal &state)
{
  GvFieldData *fields = getFieldData();
  
  if (!state.vrml2 && !isBuiltIn) {
	 fields->DoFieldTypes(state,this);  	
	 state.astream->SetNl();
  }
  fields->DoIsConnections(state,this); // new   
  return(fields->Do(state,this));
}

int GvNode::DoNodeClass(GioTraversal &state)
{ const char *n;
  n = GetType();
  return(state.astream->tok(n));
  return(0);
}



int GvNode::DoNodeBegin(GioTraversal &state)
{
	return(state.astream->BeginBlockTab());
}

int GvNode::DoNodeEnd(GioTraversal &state)
{
	int ret = state.astream->EndBlockTab();
	// return(state.astream->SetNl()); 
	return(ret); // no new line 

}


int GvNodeShell::Do(GioTraversal &state) 
{
  // don´t traverse builded shell !
  if (Id() == GvNodeShell::ClassId()) { // abstract type
      if (!theShell) BuildShell(state);
	  if (theShell)  return theShell->Do(state);

  }
  return (GvNode::Do(state));
}	

#ifdef _G_VRML1
int GvGroup:: TraverseChildrenIo(GioTraversal &state)
{	int ret;
    for (int i = 0; i < getNumChildren(); i++) {
	 	ret = getChild(i)->Traverse(state);
		state.astream->SetNl();
	}
	return(ret);
}
				      

int GvGroup::Do(GioTraversal &state)
{ int ret=0;

  if (DoNodeDef(state) == GV_NODE_ALREADY_SEEN) {
  	return(GV_NODE_ALREADY_SEEN);
  } else {
  	DoNodeClass(state);
  	DoNodeBegin(state);

  		DoNodeFields(state);
        state.astream->SetNl();

  		ret=TraverseChildrenIo(state);
  
  	DoNodeEnd(state);
  }
  return(ret);
}


int GvWWWInline::Do(GioTraversal &state)
{
  // to do:   update bboxSize bboxCenter
  //

  if (DoNodeDef(state) == GV_NODE_ALREADY_SEEN) {
  	return(GV_NODE_ALREADY_SEEN);
  } else {
  	DoNodeClass(state);
  	DoNodeBegin(state);
  		DoNodeFields(state);
  	DoNodeEnd(state);
  }

  
  if (state.writeInlines) {
    // to do:

  }
  return(1);
}
#endif _G_VRML1

int Gv2Inline::Do(GioTraversal &state)
{
  // to do:   update bboxSize bboxCenter
  //

  if (DoNodeDef(state) == GV_NODE_ALREADY_SEEN) {
  	return(GV_NODE_ALREADY_SEEN);
  } else {
	//if (state.writeInlines)
	//	 return(state.astream->tok("Group"));
	//}
  	//else 

	DoNodeClass(state);

	DoNodeBegin(state);

	if (state.updateInlineBbox) {
		BBox bbox;
		if (((GvNode*) this)->getBBox(bbox))
			this->setBBox(bbox);
	}
	// should not write children 
	GvBool def = children.isDefault();

	//if (state.writeInlines)
	//	children.setDefault(FALSE);
	//else 
		children.setDefault(TRUE);


  		DoNodeFields(state);

	children.setDefault(def);

	DoNodeEnd(state);
  }

  
  return(1);
}

#ifdef _G_VRML1

int GvTexture2::Do(GioTraversal &state)
{

#if 0
  if (DoNodeDef(state) == GV_NODE_ALREADY_SEEN) {
  	return(GV_NODE_ALREADY_SEEN);
  } else {
  	DoNodeClass(state);
  	DoNodeBegin(state);

  		DoNodeFields(state);
  	DoNodeEnd(state);
  }

  
  if (state.writeTextures) {
    // to do:

  }
  return(1);
#else
   return(GvNode::Do(state));

#endif
}


int GvUnknownNode::Do(GioTraversal &state)
{

  if (DoNodeDef(state) == GV_NODE_ALREADY_SEEN) {
  	return(GV_NODE_ALREADY_SEEN);
  } else {
  	
	state.astream->tok(className); // DoNodeClass(state);

  	DoNodeBegin(state);
  		DoNodeFields(state);
  	DoNodeEnd(state);
  }
  return(1);
}

#endif


int GvProto::Do(GioTraversal &state)
{	int ret = 0;

    void	*node;

	// check if node with this name has been written
    if (state.protoDict->find((u_long) className.getString(), node)) {
  	    return(GV_NODE_ALREADY_SEEN);
    } else 
    {

		GvProto *oldProto = state.currentProto;
		state.currentProto = this;

        state.Do(GvKeyword::proto); // DoNodeClass(state);
  	
	    state.Do(className); // DoNodeClass(state);

	    // put written proto in dictonary 
	    state.protoDict->enter((u_long) className.getString(), (void *) this);

		// new namespace ????


	    state.astream->BeginListTab();
            protoFieldData.DoInterface(state,this); // write interface 
	    state.astream->EndListTab();
		state.astream->SetNl();

  	    DoNodeBegin(state);
            ret = children.TraverseIo(state);
			// protos ??? 
            ret = routes.Traverse(state);
  	    
		DoNodeEnd(state);

		state.currentProto = oldProto;
		ret = 1;
  	    state.astream->SetNl();


    }
	return(ret);
}

int GvExternProto::Do(GioTraversal &state)
{	int ret = 0;

    void	*node;

	// check if node with this name has been written
    if (state.protoDict->find((u_long) className.getString(), node)) {
  	    return(GV_NODE_ALREADY_SEEN);
    } else 
    {


        state.Do(GvKeyword::externProto); // DoNodeClass(state);
  	

	    // put written proto in dictonary 
	    state.protoDict->enter((u_long) className.getString(), (void *) this);
		
		state.Do(className); // DoNodeClass(state);

		state.astream->BeginListTab();
		   state.astream->SetNl();
           getFieldData()->DoInterface(state,this,FALSE); // write interface 
		state.astream->EndListTab();

		state.astream->SetNl();


		url.Do(state);

		state.astream->SetNl();

		ret = 1;

    }
	return(ret);
}

int GvProtoInstance::Do(GioTraversal &state)
{
	GvProto *p = getProto(state);
	if (p) p->Do(state); // check if proto is written  

	if (DoNodeDef(state,FALSE) == GV_NODE_ALREADY_SEEN) {
  		return(GV_NODE_ALREADY_SEEN);
	} else {
  		DoNodeClass(state);
  		DoNodeBegin(state);

  			DoNodeFields(state);
  
  		DoNodeEnd(state);

	}

	return(1);

}


int GvScript::Do(GioTraversal &state)
{  int ret=0;

  if (DoNodeDef(state,FALSE) == GV_NODE_ALREADY_SEEN) {
  	return(GV_NODE_ALREADY_SEEN);
  } else {
  	DoNodeClass(state);
  		DoNodeBegin(state);

		state.astream->SetNl();
		
		// write interface skipping internal script fields 
		scriptFieldData.DoScriptInterface(state,this,3); 
		// scriptFieldData.DoIsConnections(state,this); // done in scriptInterface

  		//DoNodeFields(state); 

		// internal fields 
		GvFieldData *fields = getFieldData();
		// fields->Do(state,this);
		
		// write internal fields 
		for(int i=0; i<3; i++) {
			GvField *f = getField(i);
			if (f->isDefault() &&  getIsConnection(f) == NULL) continue;

			fields->Do(state, this,i);
			state.astream->SetNl();
		}
		// write internal IS fields 


		// fields->DoIsConnections(state,this); // new   

  	DoNodeEnd(state);
  }


  return(ret);
}



int GvRoute:: Do(GioTraversal &state)
{	int ret = 0;

/*
    if (DoNodeDef(state) == GV_NODE_ALREADY_SEEN) {
  	    return(GV_NODE_ALREADY_SEEN);
    } else 
*/
    {

        state.Do(GvKeyword::route); 

      	state.Do(from.value->getName());

        state.astream->Sep('.');

      	state.Do(getFromEventName());

        state.Do(GvKeyword::to); 
      	
        state.Do(to.value->getName());

        state.astream->Sep('.');

        state.Do(getToEventName());
        state.astream->nl();
        
    }
	return(ret);
}

int GvScene:: Do(GioTraversal &state)
{  int ret;

    // push scene
    //GvScene *current = getCurrent();
    //setCurrent(this);

    state.Push();
		{	GvWWWInlineElement *current,*prev;
		prev = (GvWWWInlineElement*) state.Get(GvWWWInlineElement::attrId);
		if (!prev) {
		} else {
			current = (GvWWWInlineElement *) prev->NewInstance();								
			current->data = this;
			state.Push(GvWWWInlineElement::attrId,current);			
		}
		}
   
// to do: write protos on demand
   ret = protos.Traverse(state);
   //ret = children.Traverse(state);
   ret=children.TraverseIo(state);


   routes.Traverse(state);

	state.Pop();
    
    // pop scene
    //setCurrent(current);


   return(ret);

}






//
// class GPovIoTraversal
//
RTIMP(GPovIoTraversal,0,GioTraversal);


GPovIoTraversal::GPovIoTraversal(const char *FileName,const char *mode) : GioTraversal(FileName,mode)
{

};

GPovIoTraversal::~GPovIoTraversal()
{

}

int GNode::Do(GPovIoTraversal &state) 
{ 
  return 0;
} 

