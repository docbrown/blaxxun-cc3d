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
#ifndef  _GvScript_
#define  _GvScript_

/******************************************************************************
@doc

@module GvScript.h - VRML 2.0 Script node implementation|

Copyright (c) 1996 - 1998	by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GvScript>

Notes:

Changes:

Todo :

******************************************************************************/

#include <gvnode.h>
#include <gvsubnode.h>

#include <gvfielddata.h>
#include <gvmfstring.h>
#include <gvsfbool.h>
#include <gvmfnode.h>


class GFile;
#include "www.h"



// VRML Script support
#define _VS

class VsParseTree;
class VsFunctionDef;
class VsFunctionCall;






/*
  exposedField MFString url           [] 
  field        SFBool   directOutput  FALSE
  field        SFBool   mustEvaluate  FALSE

  # And any number of:
  eventIn      eventTypeName eventName
  field        fieldTypeName fieldName initialValue
  eventOut     eventTypeName eventName
*/

// protocol prefix string for VRML Script
#define VS_SCRIPT_PROTO "vrmlscript:"
#define JS_SCRIPT_PROTO "javascript:"

class GvScript : public GvNode 

#ifdef _COM
	,public CComDualImpl<ScriptContainer, &IID_ScriptContainer, &LIBID_GLViewVrmlLib>
#endif

{

    GV_NODE_HEADER_UNKNOWN_NODE(GvScript);


  public:
    // fields
    
    GvMFString url;         //[]
    GvSFBool   directOutput; // FALSE;
    GvSFBool   mustEvaluate; //  FALSE;


	GvBool	   isRegistered; // TRUE if node registered in Scene information

	// the execution state of the script 
	enum ScriptState {
		SCRIPT_NOT_LOADED,	// not loaded yet
		SCRIPT_ERROR,		// script has a error
		SCRIPT_LOADED,		// script has been sucessfully loaded, but not initialized yet 
		SCRIPT_INITIALIZING, // script is in the initialize() call 
		SCRIPT_LOADED_AND_INITIALIZED,	// script has been sucessfully loaded and initialize function has been called 
		SCRIPT_TERMINATING, // script is in the shutdwon() call 
	};


	ScriptState state;		// current script state


	// the execution state of the script 
	enum ScriptPermission {
		SCRIPT_PERMISSION_RESTRICTED,		// 
		SCRIPT_PERMISSION_NO_GET_WORLD,		// 
		SCRIPT_PERMISSION_NO_REPLACE_WORLD,	// 
		SCRIPT_PERMISSION_ALL,				// script can do all
	};


	ScriptPermission permission;		// the current permissions 

	GvBool		mustCallEventsProcessed; // true if eventsProcessed must be called 


	
    // interface
    GvFieldData  scriptFieldData; // returned by getFieldData

    // list of pointers to fields
    GvFieldList fields;

    // overrides
    // get pointer to field at index 
    GvField *getField(int index) const;

    // computes copyOnInstance flag
    GvBool computeCopyOnInstance(GvCopyContext &context);

	// set all SF/MFNode fields to NULL
	GvBool flushNodeFields(); 


	// a field has been changed, check for url changes 
	int OnFieldChanged(GvField *field);



    // create fields according to scriptFieldData
    void instanceFields();

    GvBool readInstance(GvInput *in);

	// create a new instance of script, copy script interface
	GvNode *newInstance();



#ifdef _VS
	VsParseTree *vsTree; // VRML script parse tree

	VsFunctionCall *vsEventsProcessedCall;

	
	// Parse the script 
	int ParseScript(const char *script,GView *theBrowser,GvInput *in=NULL);
	int ParseScriptFile(const char *fileName,int fileFmt,GView *theBrowser);



	// release current script and all resources 
	void ReleaseScript();
#endif

	GFileRef        loader;			// url loader

	// get absolute home url
	const char * GetHomeUrl() const;

	// get the absolute url
	const char * GetUrl() const;

	// get base  url of VRML file, which contains the script node
	const char * GetBaseUrl() const;

	// try to load url & parse 
	int Load(GTraversal &state);

	// load the script directly during parse
	int Load(GvInput *in=NULL);



#ifdef _COM
	Script *script; // pointer to com object supporting the script interface 
	// parse and create the COM object from the url 
	int  ParseObject(const char *script);

	// release the object 
	void ReleaseObject();
#endif


	

	// call initialize function of the script, but must be loaded first 
	int initialize();

	// call shutdown function of the script, but must be initialized first 
	int shutdown();

	// call the eventsProcessed call
	void callEventsProcessed(); 

	int OnFieldChanged();

    

	int Do(GTraversal &state);   	
	int Do(GPathTraversal &state);   	
	
	int Do(GLoadInlinesTraversal &state);

	
	// check for registration
	int Do(GglRenderTraversal &state);

    int Do(GioTraversal &state);   	// in GioTraversal.cpp

	//int Do(GDisplayTreeTraversal &state); // implemented in GDisplayTree.cpp

#ifdef _COM
	BEGIN_COM_MAP(GvScript)
		COM_INTERFACE_ENTRY(ScriptContainer)
		COM_INTERFACE_ENTRY_CHAIN(GvNode)
	END_COM_MAP()

	DECLARE_NOT_AGGREGATABLE(GvScript)

    /* IUnknown methods */ \
	STDMETHOD_(ULONG, AddRef)() { return GvNode::AddRef();  } 
	STDMETHOD_(ULONG, Release)() { return GvNode::Release();  } 
	STDMETHOD(QueryInterface)(REFIID iid, void ** ppvObject) {return _InternalQueryInterface(iid, ppvObject);} 


#endif

   
};


#endif /* _GVScript_ */


