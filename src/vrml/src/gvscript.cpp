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

@module GvProto.cpp - VRML 2.0 Script node handling|

Copyright (c) 1996 - 1998 by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GvScriptFunctionConnection>
<c GvScript>


Notes:

   contains code for COM object script nodes

Changes:

$Revision: $
$Log: $


Todo :

******************************************************************************/

#include <gvscript.h>

#include <gvfield.h>
#include <gvconnection.h>
#include <gvinput.h>
#include <gvreaderror.h>
#include <gvsensors.h>

#include <gvkeyword.h>

#include <greporter.h>

#include "grender.h"
#include "gmodel.h"

#include <gvtraverse.h>


#ifdef _VS

#include "vsparse.h"
#include "vsfields.h"
#include "vsfunction.h"

extern int yydebug;   /* Set to 1 to debug YACC */
extern int lldebug;   /* Set to 1 to debug lexer */


/*
    trigger does calls script function 
*/

#if 1
class GvScriptFunctionConnection : public GvConnection {

    RT(GvScriptFunctionConnection);

public:


	
	VsFunctionDef *f;
	VsExprList arguments;
	VsField *eventInArg;
	VsField *timestampArg;
	VsFunctionCall *f_call; // = new VsFunctionCall(f_main, arguments);
	GvScript *script;
    
    GvScriptFunctionConnection(GvScript *script_=NULL, VsFunctionDef *f_=NULL,GvField *field=NULL);
    ~GvScriptFunctionConnection();
    
    int OnTrigger(GvField *from);

};

RTIMP(GvScriptFunctionConnection,0,GvConnection);

GvScriptFunctionConnection::GvScriptFunctionConnection(GvScript *script_,VsFunctionDef *f_,GvField *field): script(script_),f(f_) 
{
	eventInArg = NULL;
	timestampArg = NULL;

	ASSERT(script != NULL);
	ASSERT(f != NULL);
	ASSERT(field != NULL);

	
	if (f->getNumArgs() > 0) {
	// Create an argument corresponding to the eventIn value
		eventInArg = VsField::create(field->getFieldType(), VsField::RO,NULL,1);
		ASSERT(eventInArg != NULL);
		arguments.append(eventInArg);
		if (f->getNumArgs() > 1) {
			// Create an argument corresponding to the eventIn time stamp
			timestampArg = VsField::create(SFTIME, VsField::RO,NULL,1);
			ASSERT(timestampArg != NULL);
			arguments.append(timestampArg);
			
			// set point to global time variable 
			GView *view = (GView *) script_->getBrowser();
			timestampArg->setField(&view->time_changed);


		}
	}


	// create a function call instance with the function and
	// its arguments

	f_call = new VsFunctionCall(f, arguments);
	f_call->ref();

}

GvScriptFunctionConnection::~GvScriptFunctionConnection()
{

	if (f_call) f_call->unref();
#ifdef _DEBUG
	f_call = NULL;
#endif
}


int
GvScriptFunctionConnection::OnTrigger(GvField *from)
{
	//TRACE("Calling script function \n");
	//xx GvSFTime timeField;

	if (eventInArg) {
		eventInArg->setField(from);
		// time stap set at init time 
	} 
	//TRACE("GvScriptFunctionConnection::OnTrigger %s %d \n",from->ClassName(),connectionsTriggeredCnt);

    VsValue v;
	f_call->evaluate(v,NULL);

#ifdef _DEBUG
	    // print the return value for testing purposed
	    switch (v.type) {
	      case VsValue::vNUMBER:
			TRACE("script returned %g\n",v.number);
			break;
	      case VsValue::vSTRING:
			TRACE("script returned \"%s\"\n", v.string);
			break;
	      default:
		break;
	    }
#endif


	// schedule eventsProcessed function !!!!!!
	if (script->vsEventsProcessedCall && !script->mustCallEventsProcessed) {
		GView * view = (GView*) script->getBrowser();
		view->pendingScripts.add(script);
	    script->mustCallEventsProcessed = TRUE;
	}
	return(1);
}

#endif



#endif


#ifdef _COM


/*
    trigger does calls script com object 
*/

class GvScriptObjectConnection : public GvConnection {

    RT(GvScriptObjectConnection);

public:


	GvScript *script; // the script node containing the script object 	
	CComBSTR eventName;
	int eventId;
    
    GvScriptObjectConnection(GvScript *s=NULL,const char *eventName_=NULL,int eventId_=-1);
    ~GvScriptObjectConnection();
    
    int OnTrigger(GvField *from);

};

RTIMP(GvScriptObjectConnection,0,GvConnection);

GvScriptObjectConnection::GvScriptObjectConnection(GvScript *s,const char *eventName_,int eventId_):script(s),eventName(eventName_),eventId(eventId) 
{

	ASSERT(s != NULL);

	// no additional addRef on script !?
}

GvScriptObjectConnection::~GvScriptObjectConnection()
{
	script = NULL;
}



int
GvScriptObjectConnection::OnTrigger(GvField *from)
{

	TRACE("Calling script com function \n");
	
	if (script->script == NULL) return(0); /// 


	HRESULT res;
	GView * view = (GView*) script->getBrowser();

	ASSERT(view != NULL);
	
	res = script->script->processEvent(eventName,eventId,(EventOut *) from,view->getTime());

	TRACE("Returning from Calling script com function  %d\n",res);

	// schedule eventsProcessed function !!!!!!
	if (!script->mustCallEventsProcessed) {
		view->pendingScripts.add(script);
	    script->mustCallEventsProcessed = TRUE;
	}

	return(1);
}

#endif



//
// GvScript
//

GV_NODE_SOURCE_IMP(GvScript,GvNode);

#if 0

GvMFNode *GvScript::pendingScripts = NULL;
void  GvScript::initPendingScripts()
{
	pendingScripts = new GvMFNode(0);
}

void  GvScript::termPendingScripts()
{
	if (pendingScripts) delete pendingScripts;
	pendingScripts = NULL;
}

GvBool GvScript::callPendingScripts()
{
// 	for (int i= 0; i<pendingScripts) ...
	return FALSE;
}
#endif



GvScript::GvScript() : url(0),directOutput(FALSE),mustEvaluate(FALSE),isRegistered(FALSE)
{
#ifdef _VS
	vsTree = NULL;
	vsEventsProcessedCall = NULL;
	mustCallEventsProcessed = FALSE;
#endif

#ifdef _COM
	script = NULL;
#endif 
	state = SCRIPT_NOT_LOADED;
	permission = SCRIPT_PERMISSION_ALL;

    GV_NODE_CONSTRUCTOR(GvScript);

    GV_NODE_ADD_FIELD(url);
    GV_NODE_ADD_FIELD(directOutput);
    GV_NODE_ADD_FIELD(mustEvaluate);


    // problem internal added fields, how to manage ?????
    // add internal fields to field data
    scriptFieldData.addField(this, GV__QUOTE(url), &url); 
    scriptFieldData.addField(this, GV__QUOTE(directOutput), &directOutput); 
    scriptFieldData.addField(this, GV__QUOTE(mustEvaluate), &mustEvaluate); 
 
#if 0
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
#endif
}



GvScript::~GvScript()
{
    // fields will be deleted
	refCnt++;
#ifdef _VS
		ReleaseScript();
#endif
#ifdef _COM
		ReleaseObject();
#endif 
	fields.DeleteFields();
	refCnt--;

}

GvFieldData *GvScript::getFieldData() const 
{
	return (& ((GvScript *)this)->scriptFieldData);
}


// create a new instance of script, copy script interface
GvNode * GvScript::newInstance()
{
	GvScript *s = (GvScript *) GvNode::newInstance();
	//s->scriptFieldData.set(scriptFieldData);

    GvFieldEntry *tmpField;
    GvFieldEntry *newField;

	// skip first 3 built in fields
    for (int i=3; i<scriptFieldData.getNumFields(); i++) {
        tmpField = scriptFieldData.getFieldEntry(i);
        newField = new GvFieldEntry(*tmpField); 
		s->scriptFieldData.addField(newField); // could ref to field data by ref !!!!!!!!!!!!!!
    }
	// make room for fields 
	s->instanceFields();
	return(s);
}

// a field has been changed
int GvScript::OnFieldChanged(GvField *field)
{
	if (field == &url) {
		TRACE("GvScript::OnFieldChanged(&url), shuting down script\n");
		shutdown();
		loader.SetNotLoaded();
		return(1);
	} 
	else return(OnChanged());
}


GvField *
GvScript::getField(int index) const
{
    switch(index) {
    case 0 : return (GvField*)&url;
    case 1 : return (GvField*)&directOutput;
    case 2 : return (GvField*)&mustEvaluate;

    default : 
      // ASSERT(index>=0 && index < fields.getLength()); // during parsing, this is used to check for builtin fields of script
      if (index>=0 && index < fields.getLength())
		return(fields[index]);
	  return NULL;
    }
}



// reads the field + interface list of node
GvBool
GvScript::readInstance(GvInput *in)
{

    if (!scriptFieldData.readScriptInterface(in, this)) {
		instanceFields();

		GvReadError::post(in,GV_RE_SYNTAX_ERROR, "Bad interface specification for Script %s",this->getName().getString());
        
		return (FALSE);
	}

    instanceFields();
	if (!in->currentProto) { // tbd: delete field default values to clean up memory
		// instance script if a standard script 
		if (Load(in)>=0) {
			// == 0 url loaded later 
		} else {
			GvReadError::post(in,GV_RE_SYNTAX_ERROR, "Error in Script %s",this->getName().getString());
		}
	} else {

	}

    return TRUE;
}

GvBool
GvScript::computeCopyOnInstance(GvCopyContext &context) 
{
	if (context.alreadySeen(this)) 
		return copyOnInstance;
	context.setSeen(this);

	GvFieldData *fieldData = getFieldData();

	if (directOutput) {
		copyOnInstance = TRUE;
		// for all fields , mark point to nodes with setCopyOnInstance 
		// if directOuput mark nodes hold by SF /MFNode fields, for need to be copied !
		for (int i = 0; i < fieldData->getNumFields(); i++) {
        
        GvFieldEntry *entry= fieldData->getFieldEntry(i);
        GvField *field = fieldData->getField(this, i);
        
        if (!field) continue;

        if (RTISA(field,GvMFNode)) {
			GvMFNode *f =((GvMFNode*) field);

			for(int i=0; i<f->getNum(); i++) { // alternative, mark scene graph recursive
												// to resolve Cosmo inmplementation inompabilities
				GvNode *n = f->values[i];
				if (n) n->setCopyOnInstance(TRUE);
			}

        }
        else if (RTISA(field,GvSFNode)) {
			GvNode *n =((GvSFNode*) field)->value;
            if (n) n->setCopyOnInstance(TRUE);
        }
		}
	}

	if (copyOnInstance) return copyOnInstance; // already computed 

	copyOnInstance = fieldData->computeCopyOnInstance(this,context) || copyOnInstance;

    return(copyOnInstance);
}


// set all SF/MFNode fields to NULL
GvBool
GvScript::flushNodeFields() 
{
	GvBool ret = FALSE;

	GvFieldData *fieldData = getFieldData();

	// for all fields , mark point to nodes with setCopyOnInstance 
	// if directOuput mark nodes hold by SF /MFNode fields, for need to be copied !
	for (int i = 0; i < fieldData->getNumFields(); i++) {
        
        GvFieldEntry *entry= fieldData->getFieldEntry(i);
        GvField *field = fieldData->getField(this, i);
        
        if (!field) continue;

        if (RTISA(field,GvMFNode)) {
			GvMFNode *f =((GvMFNode*) field);

			int old = f->enableConnections(0);

			f->flush();

			f->enableConnections(old);

			ret = TRUE;

        }
        else if (RTISA(field,GvSFNode)) {
			GvSFNode *f =((GvSFNode*) field);

			int old = f->enableConnections(0);
			f->set((GvNode *) NULL);

			f->enableConnections(old);

			ret = TRUE;
        }
	}
	return ret;
}


// create fields according to scriptFieldData
void GvScript::instanceFields()
{
    // instance all fields / events and assign default valus
    int numFields;

	ASSERT(fields.getLength() == 0); // HG diag should only be called one time

    fields.DeleteFields();
    fields.setSize(numFields = scriptFieldData.getNumFields());
    for (int i=0; i<numFields; i++) 
    {
        GvField *field;
        if (i>=3) { // not a built-in field
			GvFieldEntry * entry = scriptFieldData.getFieldEntry(i);
			
			if (entry ->defaultValue) { // reclaim memory from already parsed Script interface
				field = entry ->defaultValue; // new 26.05.99
				entry->defaultValue = NULL;
			} else 
				field = entry->createInstance(); // create an new instance of field

            fields[i]= field; 

            if (field) { 
				field->setContainer(this); field->setDefault(FALSE); 
			}
        }
        else fields[i]=NULL;
    }
}

// is this connection an internal script eventIn connection 

GvBool PredIsScriptConnection(GvConnection *c, void *data){
#ifdef _VS
	if (RTISA(c,GvScriptFunctionConnection)) {
		return TRUE;
	}
#endif
#ifdef _COM
	if (RTISA(c,GvScriptObjectConnection)) {
		return TRUE;
	}
#endif
	return FALSE;
}




void GvScript::ReleaseScript()
{

	ref();

#ifdef _VS
	if (vsTree) {
		// call shutdown  function
		if (state == SCRIPT_LOADED_AND_INITIALIZED)  {
			state = SCRIPT_TERMINATING;
			
			VsFunctionDef *f = vsTree->findFunction("shutdown");

			if (f != NULL) {

				VsExprList arguments;
				VsFunctionCall *f_call = new VsFunctionCall(f,arguments);
				VsValue v;
				f_call->ref();
					f_call->evaluate(v,NULL);
				f_call->unref();
			}
		}
	
		// delete script function connections 
		int numFields = scriptFieldData.getNumFields();
		for (int i=0; i<numFields; i++) 
		{
			GvField *field = getField(i);
			GvFieldEntry *entry = scriptFieldData.getFieldEntry(i);
			if (!field) continue;
			field->removeConnection(PredIsScriptConnection,NULL);
		}
		VsSFNode *browser = (VsSFNode *)vsTree->getGlobalVar("Browser");
		if (browser) browser->getField()->set((GvNode *)NULL);



	}
	
	if (vsEventsProcessedCall)	vsEventsProcessedCall->unref();
	vsEventsProcessedCall = NULL;	

	if (vsTree) 
		delete vsTree;

	vsTree = NULL;
	state = SCRIPT_NOT_LOADED;
#endif
	
	unrefNoDelete();
}

#ifdef _COM	

// Parse the object
//object:code=http:-/-/www.foo.com/bar.ocx#Version=a,b,c,d
//		   clsid=


// release the object 
void GvScript::ReleaseObject()
{	
	HRESULT res;
	if (script == NULL) return;
	ref();

	TRACE("Script: Unloading object \n");

	if (state == SCRIPT_LOADED_AND_INITIALIZED)  {
		state = SCRIPT_TERMINATING;


		res = script->shutdown();
		TRACE("Returning from script->shutdown() com function  %d\n",res);
	}

	res = script->setContainer(NULL);
	res = script->Release();
	script = NULL;
	state = SCRIPT_NOT_LOADED;

	unrefNoDelete();

}


BOOL getPart(const char *&p1,const char * &p2)
{
  while (isspace(*p1)) p1++;

  if (*p1 == 0) return FALSE;

  p2=p1;
  BOOL inString = 0;

  while (*p2) {
	  if (inString && *p2 == '\\') { // escape 
		  p2++;
		  if (! *p2) break;

		  p2++;
		  continue;
	  }	

	  if (*p2 == '\"') {
		  inString = !inString;
	  }	

	  // stop on first space 
	  if ( !inString && isspace(*p2)) break;

	  p2++;

  }	
  return TRUE;

}
#define del '='
BOOL getParam(const char *s,const char *param,CString &value)
{
	const char *start=s;
	const char *end;

	int l = strlen(param);
	
	while (getPart(start,end)) {

		if (strncmp(start,param,l) == 0) {
			const char *p=start+l;
			while (isspace(*p)) p++;

			if (*p == del) {
				p++;
				
				while (isspace(*p)) p++;

				value = CString(p,end-p);
				value.TrimRight();
				return TRUE;

			}

		}
		start = end;

	}
	return FALSE;
}

/*
WINOLEAPI GetClassFile( 
LPCWSTR szFileName, 
//Pointer to filename for which you are requesting a CLSID 
CLSID * pclsid 
//Pointer to location for returning the CLSID 
);

  StringFromCLSID
*/ 

// parse and create the COM object from the url 
int  GvScript::ParseObject(const char *objectUrl)
{
	CString code;
	CString clsids;

	ReleaseObject();

	getParam(objectUrl,"code",code);
	getParam(objectUrl,"clsid",clsids);

	CComBSTR clsidB(clsids);

	CLSID clsid;
	HRESULT res;
	DWORD dwClsContext = CLSCTX_SERVER;
	IUnknown *scriptU=NULL;


	//IIDFromString
	if (clsids[0] == '{')
			res = CLSIDFromString(clsidB, &clsid);
	else
			res = CLSIDFromProgID(clsidB, &clsid);
/*
	HRESULT CreateObject(
		const CLSID& clsid, DWORD dwClsContext=CLSCTX_INPROC_SERVER)
	{
		_Release();
		HRESULT hr = CoCreateInstance(clsid, NULL, dwClsContext,
			GetIID(), reinterpret_cast<void**>(&_pInterface));
		ASSERT(SUCCEEDED(hr));
		return hr;
	}

*/

	TRACE("Script:loading object from %s clsdid %s res = %d\n",(const char *) objectUrl,(const char*)clsids,res);

	CComBSTR codeB(code);
	if (1) {
		res = CoCreateInstance(clsid, NULL, CLSCTX_SERVER, IID_IUnknown, (void **)&scriptU);
	} else {
#ifdef G_URLMON
		if (CoGetClassObjectFromURLH == NULL) res = S_FALSE;
		else res = CoGetClassObjectFromURLH
#else
		res = CoGetClassObjectFromURL
#endif
			( 
				clsid,  //CLSID of the ActiveX object to be installed 
				codeB, //URL pointing to the code for the ActiveX object 
				0,  //DWORD dwFileVersionMS,  //Major version number for object to be installed 
				0,  // DWORD dwFileVersionLS, 	//Minor version number for object to be installed 
				NULL, //Mime type to be understood by the installed ActiveX object 
				NULL, // LPBINDCTX pBindCtx, //Bind context to use for downloading/installing component code 
				dwClsContext, // DWORD dwClsContext,  //Specifies the execution context for the class object 
				NULL,// LPVOID pvReserved, //Reserved, must be set to NULL 

				IID_IUnknown, //REFIID riid,  //Interface to obtain on the factory object 
				(VOID **) &scriptU  //For synchronous calls, pointer to store the interface pointer upon return 
			); 
	};

	if (res == S_OK && scriptU != NULL) {
		if (FAILED(res = scriptU->QueryInterface(IID_Script, (void **)&script)))
		{
			scriptU->Release() ;		// IID_IUnknown
			scriptU= NULL ;

		}
		else {
			scriptU->Release() ;		// IID_IUnknown
			scriptU= NULL ;
		}
	}


	if (res == S_OK && script != NULL) {
		state = SCRIPT_LOADED;
	} else {
		state= SCRIPT_ERROR;
	}
	if (state != SCRIPT_LOADED) return(0);

	script->setContainer((Node *) this);

	// add connections for eventIns to script processEvent
	int numFields = scriptFieldData.getNumFields();
    for (int i=0; i<numFields; i++) 
    {
        GvField *field = getField(i);
		GvFieldEntry *entry = scriptFieldData.getFieldEntry(i);

		switch (entry->type) { 	// type of event/field
		case GV_FIELD_EVENT_IN:
		case GV_EXPOSED_FIELD:
				break;
		default:
			continue;
			break;
		}
        if (i>=3) { // not a built-in field
			GvScriptObjectConnection *c;
			c = new GvScriptObjectConnection(this,entry->name.getString(),i);
			field->addConnection(c,FALSE);
		}
    }

	TRACE("Script:loading refCnt = %d\n",this->refCnt);
	return 1;
}
#endif // _COM	

static CCriticalSection parserLock; // works with Statics !!


// Parse the script 
int 
GvScript::ParseScript(const char *script,GView *theBrowser, GvInput *in)
{
    int i;
    int numFields;
	GvName s;

	s = getName();

    ReleaseScript();

	parserLock.Lock();

	vsTree = new VsParseTree(this);

	if (theBrowser)
		vsTree->reporter = theBrowser->GetReporter();

	if (in) {
		//vsTree->lineNumber
		// in->getLocationString(s);
	}

	// add fields / eventOuts to script

	numFields = scriptFieldData.getNumFields();
    for (i=0; i<numFields; i++) 
    {
        GvField *field = getField(i);
		GvFieldEntry *entry = scriptFieldData.getFieldEntry(i);
	    VsField *vsField;

		if (!field) continue;

		//switch (entry->type) { 	// type of event/field
		//default: // all are fields 
		//	break;
		//}
	    vsField = VsField::create(field->getFieldType(), entry->type == GV_FIELD_EVENT_IN ? VsField::RO :VsField::RW,field);
		if (!vsField) continue;

//	    vsField->setField(field);
	    vsTree->addGlobalVar(entry->name, vsField);
    }


	VsSFNode *browser = (VsSFNode *) VsSFNode::newField(NULL,VsField::RO,0);

	if (theBrowser)
		browser->getField()->set(theBrowser);
	else browser->getField()->set(getBrowser());

	if (browser) 
		vsTree->addGlobalVar("Browser", browser);


    // parse the vrmlscript in the buffer
    int result = vsTree->parse(script, s);

	parserLock.Unlock();
	

	if (!result) {
		state = SCRIPT_ERROR;
		return(0);
	}
	// resolve function calls 
	vsTree->resolveFunctionCalls();

	// add connections for eventIns to script functions 
	numFields = scriptFieldData.getNumFields();
    for (i=0; i<numFields; i++) 
    {
        GvField *field = getField(i);
		GvFieldEntry *entry = scriptFieldData.getFieldEntry(i);

		switch (entry->type) { 	// type of event/field
		case GV_FIELD_EVENT_IN:
		case GV_EXPOSED_FIELD:
				break;
		default:
			continue;
			break;
		}
		// find functions for eventIns 
		VsFunctionDef *f = vsTree->findFunction(entry->name);
		if (f == NULL) {
	        if (i>=3) { // not a built-in field
				// could have an ROUTE or IS 
				//Reporter.Warning("Couldn't find script function %s\n",entry->name.getString());
			}
			continue;
		}
		GvScriptFunctionConnection *c;
		c = new GvScriptFunctionConnection(this,f,field);
		field->addConnection(c,FALSE);
    }

	// First find the function with a given name (this is the
	// name of the eventIn being handled or the name 
	// "eventsProcessed", "initialize", or "shutdown")


	// find eventsProcessed function
	VsFunctionDef *f = vsTree->findFunction("eventsProcessed");
	if (f != NULL) {
		VsExprList arguments;
		vsEventsProcessedCall = new VsFunctionCall(f,arguments);
		vsEventsProcessedCall->ref();
	}


	state = SCRIPT_LOADED;
	return(1);
}


// Parse the script 
int 
GvScript::ParseScriptFile(const char *fileName,int fileFmt,GView *browser )
{	char *buffer;

	FILE *f = NULL;
	long size;

	int ret=-1;

	f = fopen(fileName,"rb");
	
	if (f == NULL) return(-1);
	
	fseek(f,0,SEEK_END);
	size = ftell(f);
	fseek(f,0,SEEK_SET );
	if (size <=0) goto error;

	buffer = (char *) malloc(12+size * sizeof(char));
	if (buffer ==  NULL ) goto error;
	
	size= fread(buffer,1,size,f);

	if (size <=0 ) goto error;
	
	buffer[size]=0;

	fclose(f);


	ret = ParseScript(buffer,browser);
	
	free(buffer);

	return ret;
error:
	if (f) fclose(f);
	return -1;
}



// call initialize function of the script, but be loaded first 
int GvScript::initialize() {

	if (state == SCRIPT_LOADED) {

		ref();
		state = SCRIPT_INITIALIZING;

		if (vsTree) {
			VsSFNode *browser = (VsSFNode *)vsTree->getGlobalVar("Browser");
			if (browser) 
				browser->getField()->set(getBrowser());

			// call initialize function
			VsFunctionDef *f = vsTree->findFunction("initialize");
			if (f != NULL) {
				VsExprList arguments;
				VsFunctionCall *f_call = new VsFunctionCall(f,arguments);
				VsValue v;
				f_call->ref();
					f_call->evaluate(v,NULL);
				f_call->unref();
			}
		}
#ifdef _COM
		else if (script) {
			
			HRESULT res = script->initialize();
			TRACE("Returning from script->initialize() com function  %d\n",res);

		}
#endif 
		state = SCRIPT_LOADED_AND_INITIALIZED;
		
		unref();
		return(1);
	}
	
	return (0);

}

// call shutdown function of the script, but must be initialized first 
int GvScript::shutdown()
{
#ifdef _VS
		ReleaseScript();
#endif
#ifdef _COM
		ReleaseObject();
#endif 

	return 0;
}



// call the eventsProcessed call
void GvScript::callEventsProcessed() 
{
	mustCallEventsProcessed = FALSE;

	if (state == SCRIPT_LOADED_AND_INITIALIZED) {

#ifdef _VS
		if (vsEventsProcessedCall) {
			VsValue v;
			vsEventsProcessedCall->evaluate(v,NULL);
		}

#endif

	}
	return;
}



// try to load any vrmlscript
// 1 ok if found and no syntax error
// 0 no usable script or error
#define OBJECT_SCRIPT_PROTO "object:"

int
GvScript::Load(GvInput *in)
{

	if (url.Length() == 0) {
		loader.SetNoUrl();
		state = SCRIPT_ERROR;
		return 0;
	}

	// to do: check for vs files etc
	int len,i;
#ifdef _COM
	len	 = strlen(OBJECT_SCRIPT_PROTO);
	for(i=0; i<url.Length();i++) {
		if (strncmp(url[i].getString(),OBJECT_SCRIPT_PROTO,len)==0) {
			if (ParseObject(url[i].getString()+len)) {
				loader.urlI=i;
				loader.SetProcessed();
				return(1);
			}
		}
	}

#endif

	
	len	 = strlen(VS_SCRIPT_PROTO);

	for(i=0; i<url.Length();i++) {
		if (strncmp(url[i].getString(),VS_SCRIPT_PROTO,len)==0) {
			if (ParseScript(url[i].getString()+len, (GView*) getBrowser(), in)) {
				loader.urlI=i;

				loader.SetProcessed();
				return(1);
			}
		}
	}

	len	 = strlen(JS_SCRIPT_PROTO);

	for(i=0; i<url.Length();i++) {
		if (strncmp(url[i].getString(),JS_SCRIPT_PROTO,len)==0) {
			if (ParseScript(url[i].getString()+len,(GView*) getBrowser(),in)) {
				loader.urlI=i;
				loader.SetProcessed();
				return(1);
			}
		}
	}
	len	 = strlen(OBJECT_SCRIPT_PROTO);

	// new 10.07.99
	//?? state=SCRIPT_ERROR;
	//?? loader.SetBadData();
	
	// might be URL 
	state=SCRIPT_NOT_LOADED;
	loader.SetNotLoaded();

	return(0);
}

// get absolute home url
const char *GvScript::GetHomeUrl() const
{
	// has GetHome just been setup ?????
	return(loader.GetHomeUrl());
}

// get base  url of VRML file, which contains the script node
const char * GvScript::GetBaseUrl() const
{
	return(loader.GetBaseUrl());
}


int GvScript::Do(GLoadInlinesTraversal &state)
{
	if (state.resolveRelativeUrls && !loader.HasBaseUrl()) {
		loader.SetBaseUrl(state.GetHomeUrl());
	}

	if (this->state == SCRIPT_NOT_LOADED && !loader.Ok() && state.loadProtos >= GTraversal::LOAD_IF_NEEDED) {
		  Load(state);
	}	

	return(0);
}



// based on Gv2Inline::Load 

int 			
GvScript::Load(GTraversal &state)
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
			state.view->GetReporter()->Warning("Can't load Script node URL\n");
			this->state = SCRIPT_ERROR;
			//isLoaded.set(FALSE);
		    break;
		}
	case S_NOT_LOADED: // starting state : NOT loaded 
	     if (url.Length() == 0) { 
			 loader.SetNoUrl();		
			 this->state = GvScript::SCRIPT_ERROR;
			 //isLoaded.set(FALSE);
			 return(0);
		 }
		 if (!loader.HasBaseUrl())
			loader.SetBaseUrl(state.GetHomeUrl());

		 int len;
	 	 len = strlen(VS_SCRIPT_PROTO);
		 if (strncmp(url[loader.urlI].getString(),VS_SCRIPT_PROTO,len)==0) {
			if (ParseScript(url[loader.urlI].getString()+len, state.getBrowser(), NULL)) {
				loader.SetProcessed();
				return(1);
			}
			else status = S_BAD_URL;
		 }
	 	 len = strlen(JS_SCRIPT_PROTO);
		 if (strncmp(url[loader.urlI].getString(),JS_SCRIPT_PROTO,len)==0) {
			if (ParseScript(url[loader.urlI].getString()+len, state.getBrowser(), NULL)) {
				loader.SetProcessed();
				return(1);
			}
			else status = S_BAD_URL;
		 }

		 // java not yet supported 
		 if (strsuffix(url[loader.urlI].getString(),".java") || strsuffix(url[loader.urlI].getString(),".class")) {
			if (url.getNum() == 1) 
				state.view->GetReporter()->Warning("Java support for Script node not implemented.");
			status = S_BAD_URL;
		 }
			 

		 // assume url : e.g. .js .vs
		 // start processing 
		 if (status != S_BAD_URL) {
		 CTime refreshTime(0);
		 const char *refedFromUrl= state.GetUrl(refreshTime);	 


		 status = loader.Load(loader.GetBaseUrl(),url[loader.urlI].getString(),
			 refedFromUrl,"Script",state.inlineWwwFlags,state.view->hWnd,loader.urlI,refreshTime,state.view->GetReporter(),state.view->file);
		 }

 		 // set url parameters
		 //worldUrl.set(loader.GetUrl());
		 //baseUrl.set(loader.GetHomeUrl());

		 if (status != S_NOT_LOADED) goto Redo;

		 break;

	case S_LOADING:
	case S_PROCESSING:
				state.pendingLoads ++;
				break;

	case S_LOADED: {
				if (!state.view) break;

				int ret;

				state.view->lastFrameLoadCnt++;
		 		state.view->lastFrameInlineLoadCnt++;
				//
				this->shutdown(); // delete all contents 


				// ret = ReadVrmlFile(loader.GetLocalFile(),state.view->GetReporter(),this, (state.view->reportErrors?0:GV_READ_NOERROR));
				ret = 0;

				if (ret>=0) {  // fine, we could read it 
					switch (loader.GetLocalFileFmt()) {
					case JS_FMT: {


						ret = ParseScriptFile(loader.GetLocalFile(),loader.GetLocalFileFmt(),state.view);
						if (ret >0) {
							state.view->GetScriptPermissions(this,loader.GetUrl());
							loader.SetProcessed();

							//children.OnChanged(); // so could advise on children_changed 
							//isLoaded.set(TRUE);
						}
						else  {
							ret = -1;
							loader.SetBadData(); // bad, try next
						}

						break;
						}

					default:
						ret = -1;
						loader.SetBadData(); // bad, try next

						break;
					}

				}
				else { 
					loader.SetBadData(); // bad, try next
					state.view->lastFrameLoadErrorCnt++;
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


int GvScript::Do(GTraversal &state)
{   int ret=1;

	if (!loader.Ok()) {
		Load(state);

		if (this->state == GvScript::SCRIPT_LOADED)
			  initialize();

	}	

	if (this->state == SCRIPT_NOT_LOADED) { // should be done earlier
		TRACE("Unexpected Script Loading %s\n",this->getName().getString());
		Load();
		if (this->state == GvScript::SCRIPT_LOADED)
			  initialize();

	}

	if (!isRegistered && state.getBrowser()  /* && !RTISA((&state),GvSceneInfoTraversal)  */ )
		state.getBrowser()->Register(this);

	if (state.traverseAll && !state.traverseTransformHierarchyOnly) {
		if (state.level >= state.maxLevel) 
			return -1;

		state.Push();
			// in order to instance Protos refed by SF/MFNode fields 
			ret = TraverseAllNodeFields(state);

		state.Pop();

	} 

	return(ret);
}

int GvScript::Do(GPathTraversal &state)
{   int ret=1;


	if (!loader.Ok()) {
		Load(state);
	}	

	if (this->state == SCRIPT_NOT_LOADED) { // should be done earlier
		TRACE("Unexpected Script Loading %s\n",this->getName().getString());
		Load();
	}


	if (!isRegistered && state.getBrowser()  /* && !RTISA((&state),GvSceneInfoTraversal)  */ )
		state.getBrowser()->Register(this);


	if (state.traverseAll && !state.traverseTransformHierarchyOnly) {
		if (state.level >= state.maxLevel)  {
			TRACE("Script : recurse to deep %p \n",this);
			return -1;
		}

		// avoid recursive scan, same for ProtoInstance 
		if (state.nodeDict) {
			void *node;
			if (state.nodeDict->find((u_long) this, node)) {
				// check if already in path 
				TRACE("Script : already seen %p \n",this);
  				return(GV_NODE_ALREADY_SEEN);
			}
		    state.nodeDict->enter((u_long) this, (void *) this);
		}

		state.Push();
		GPUSH_PATH(state,this);
			state.notInTransformHierarchy++;

			// in order to instance Protos refed by SF/MFNode fields 
			ret = TraverseAllNodeFields(state);

			state.notInTransformHierarchy--;
		
		GPOP_PATH(state);

		state.Pop();

	} 

	// init call 
	if (state.initScripts && this->state == GvScript::SCRIPT_LOADED)
		  initialize();


	return(ret);
}

// check for registration
int GvScript::Do(GglRenderTraversal &state)
{
	if (this->state == SCRIPT_NOT_LOADED) { // should be done earlier
		TRACE("Unexpected Script Loading %s\n",this->getName().getString());
		Load();
		if (this->state == GvScript::SCRIPT_LOADED)
			  initialize();
	}
	if (!isRegistered)
		state.getBrowser()->Register(this);
	return (1);
}

