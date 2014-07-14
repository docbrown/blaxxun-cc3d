/* ----   file information  -------------------------------------------------
   Revison:     $Revision: 1.10 $
   Date:        $Date: 1999/03/25 16:09:10 $
   Modification History:
   $Log: browser.cpp,v $
   Revision 1.10  1999/03/25 16:09:10  kristof
   *** empty log message ***

   Revision 1.9  1998/10/07 17:16:47  kristof
   use void *

   Revision 1.8  1998/09/28 15:53:10  tom
   *** empty log message ***

   Revision 1.7  1998/07/14 18:08:37  kristof
   *** empty log message ***

   Revision 1.6  1998/07/13 16:40:17  kristof
   new methods

   Revision 1.5  1998/03/10 18:09:10  kristof
   *** empty log message ***

   Revision 1.4  1998/03/10 17:53:36  bernd
   implement getWorld

   Revision 1.3  1997/12/16 18:09:32  kristof
   createNocde corrcted

   Revision 1.2  1997/12/15 12:44:45  kristof
   *** empty log message ***

   Revision 1.1  1997/12/10 10:33:45  kristof
   Initial revision

   ----   file information  ---------------------------------------------- */

#include "stdafx.h"

#define IMPLEMENT_vrml_external_Browser
#define IMPLEMENT_vrml_external_Node
#define IMPLEMENT_vrml_external_field_EventOut
#define IMPLEMENT_vrml_external_field_EventIn
//#define IMPLEMENT_vrml_external_exception_InvalidNodeException

#ifndef _vrml_external_Browser_H_
#include "vrml_external_Browser.h"
#endif
#ifndef _vrml_external_Node_H_
#include "vrml_external_Node.h"
#endif
#ifndef _java_lang_IllegalArgumentException_H_
#include "java_lang_IllegalArgumentException.h"
#endif
#ifndef _vrml_external_field_EventOut_H_
#include "vrml_external_field_EventOut.h"
#endif
#ifndef _vrml_external_field_EventIn_H_
#include "vrml_external_field_EventIn.h"
#endif
#ifndef _vrml_external_exception_InvalidVrmlException_H_
#include "vrml_external_exception_InvalidVrmlException.h"
#endif
#ifndef _vrml_external_exception_InvalidNodeException_H_
#include "vrml_external_exception_InvalidNodeException.h"
#endif

#ifndef _NPBX3D_H_
#include "npbx3d.h"
#endif

#ifndef _JAVACTRL_H_
#include "javactrl.h"
#endif

extern "C" CJavaCtrl * getController(JRIEnv* env, struct vrml_external_Browser* self)
{
    return CJavaCtrl::getController((jref)self);
	/*NPP instance = (NPP)self->getPeer(env);
    if (!instance)
        return NULL;
    CNpbx3d* plugin = (CNpbx3d*)instance->pdata;
    if (!plugin)
        return NULL;
    return plugin->getJavaCtrl();
    */
}

/*** private native getString (I)Ljava/lang/String; ***/
extern "C" JRI_PUBLIC_API(struct java_lang_String *)
native_vrml_external_Browser_getString(JRIEnv* env, struct vrml_external_Browser* self, jint opcode)
{
    CJavaCtrl* ctrl = getController(env, self);
    if (!ctrl)
        return NULL;

    struct java_lang_String * retStr = NULL;
    CString cStr;


	switch (opcode)
	{
	    case vrml_external_Browser_OP_NAME:
            cStr = ctrl->getCC3D()->getName();
		    break;
	    case vrml_external_Browser_OP_VERSION:
            cStr = ctrl->getCC3D()->getVersion();
		    break;
	    case vrml_external_Browser_OP_WORLDURL:
            cStr = ctrl->getCC3D()->getWorldURL();
		    break;
	    case vrml_external_Browser_OP_VIEWERMODE:
            cStr = ctrl->getCC3D()->GetNavigationMode();
		    break;
	    case vrml_external_Browser_OP_VIEWPOINT:
            cStr = ctrl->getCC3D()->GetViewpoint();
		    break;
	    case vrml_external_Browser_OP_DESCRIPTION:
            cStr = ctrl->getCC3D()->GetDescription();
		    break;
	    case vrml_external_Browser_OP_AVATARURL:
            cStr = ctrl->getCC3D()->getMyAvatarURL();
		    break;
	}

    retStr = JRI_NewStringUTF ( env , cStr , cStr.GetLength() );
    return retStr;
}

/*** private native getBoolean (I)Z ***/
extern "C" JRI_PUBLIC_API(jbool)
native_vrml_external_Browser_getBoolean(JRIEnv* env, struct vrml_external_Browser* self, jint opcode)
{
    CJavaCtrl* ctrl = getController(env, self);
    if (!ctrl)
        return false;

    bool ret = false;
    switch (opcode)
	{
	    case vrml_external_Browser_OP_COLLISION:
            if (ctrl->getCC3D()->GetCollisionDetection())
                ret = true;
		    break;
	    case vrml_external_Browser_OP_HEADLIGHT:
            if (ctrl->getCC3D()->GetHeadlight())
                ret = true;
		    break;
	    case vrml_external_Browser_OP_ANIMATE:
            if (ctrl->getCC3D()->GetAnimateAllViewpoints())
                ret = true;
		    break;
	    case vrml_external_Browser_OP_ISLOADED:
            if (ctrl->getCC3D()->isSceneLoaded())
                ret = true;
		    break;
	    case vrml_external_Browser_OP_3RDPERSON:
            if (ctrl->getCC3D()->getThirdPersonView())
                ret = true;
		    break;
    }
    return ret;
}

/*** private native setString (ILjava/lang/String;)V ***/
extern "C" JRI_PUBLIC_API(void)
native_vrml_external_Browser_setString(JRIEnv* env, struct vrml_external_Browser* self, jint opcode, struct java_lang_String *jStr)
{
    CJavaCtrl* ctrl = getController(env, self);
    if (!ctrl)
        return;

    const char * stringUTF = JRI_GetStringUTFChars( env , jStr );
	switch (opcode)
	{
	    case vrml_external_Browser_OP_VIEWERMODE:
            ctrl->getCC3D()->SetNavigationMode(stringUTF);
		    break;
	    case vrml_external_Browser_OP_VIEWPOINT:
            ctrl->getCC3D()->SetViewpoint(stringUTF);
		    break;
	    case vrml_external_Browser_OP_DESCRIPTION:
            ctrl->getCC3D()->SetDescription(stringUTF);
		    break;
	    case vrml_external_Browser_OP_SAVEVIEW:
            ctrl->getCC3D()->saveViewpoint(stringUTF);
		    break;
	    case vrml_external_Browser_OP_PLAYSOUND:
            ctrl->getCC3D()->playSound(stringUTF);
		    break;
	    case vrml_external_Browser_OP_PRINT:
            ctrl->getCC3D()->print(stringUTF);
		    break;
	    case vrml_external_Browser_OP_AVATARURL:
            ctrl->getCC3D()->setMyAvatarURL(stringUTF);
		    break;
	}
    return;
}

/*** private native setBoolean (IZ)V ***/
extern "C" JRI_PUBLIC_API(void)
native_vrml_external_Browser_setBoolean(JRIEnv* env, struct vrml_external_Browser* self, jint opcode, jbool jBool)
{
    CJavaCtrl* ctrl = getController(env, self);
    if (!ctrl)
        return;

    switch (opcode)
	{
	    case vrml_external_Browser_OP_COLLISION:
            ctrl->getCC3D()->SetCollisionDetection(jBool);
		    break;
	    case vrml_external_Browser_OP_HEADLIGHT:
            ctrl->getCC3D()->SetHeadlight((short)jBool);
		    break;
	    case vrml_external_Browser_OP_ANIMATE:
            ctrl->getCC3D()->SetAnimateAllViewpoints(jBool);
		    break;
	    case vrml_external_Browser_OP_NEXTVIEW:
            ctrl->getCC3D()->OnNextViewpoint();
		    break;
	    case vrml_external_Browser_OP_PREVVIEW:
            ctrl->getCC3D()->OnPrevViewpoint();
		    break;
	    case vrml_external_Browser_OP_BEGINUPDATE:
            ctrl->getCC3D()->beginUpdate();
		    break;
	    case vrml_external_Browser_OP_ENDUPDATE:
            ctrl->getCC3D()->endUpdate();
		    break;
	    case vrml_external_Browser_OP_3RDPERSON:
            ctrl->getCC3D()->setThirdPersonView(jBool);
		    break;
    }
    return;
}

/*** public native getCurrentSpeed ()F ***/
extern "C" JRI_PUBLIC_API(jfloat)
native_vrml_external_Browser_getCurrentSpeed(JRIEnv* env, struct vrml_external_Browser* self)
{
    CJavaCtrl* ctrl = getController(env, self);
    if (!ctrl)
        return 0.0F;

    return ctrl->getCC3D()->getCurrentSpeed();
}

/*** public native getCurrentFrameRate ()F ***/
extern "C" JRI_PUBLIC_API(jfloat)
native_vrml_external_Browser_getCurrentFrameRate(JRIEnv* env, struct vrml_external_Browser* self)
{
    CJavaCtrl* ctrl = getController(env, self);
    if (!ctrl)
        return 0.0F;

    return ctrl->getCC3D()->getCurrentFrameRate();
}

/*** public native getWorld ()Lvrml/external/Node; ***/
extern "C" JRI_PUBLIC_API(struct vrml_external_Node *)
native_vrml_external_Browser_getWorld(JRIEnv* env, struct vrml_external_Browser* self)
{
    CJavaCtrl* ctrl = getController(env, self);
    if (!ctrl)
        return NULL;
//!!! which method to use?
    LPUNKNOWN pUnknown = ctrl->getCC3D()->getWorld();
    if (!pUnknown) // invalid vrml
    {
        struct java_lang_Class * exClass = class_vrml_external_exception_InvalidNodeException( env );
        JRI_ThrowNew ( env , exClass , "Invalid VRML" );
        return NULL;
    }
    ctrl->addRef(pUnknown, TypeNode);

    // convert to java
    struct java_lang_Class * nodeClass = class_vrml_external_Node( env );
    struct vrml_external_Node * node;
    node = vrml_external_Node_new(env, nodeClass, (jint)ctrl, (jint)pUnknown );
    
    return node;
}

/*** public native getNode (Ljava/lang/String;)Lvrml/external/Node; ***/
extern "C" JRI_PUBLIC_API(struct vrml_external_Node *)
native_vrml_external_Browser_getNode(JRIEnv* env, struct vrml_external_Browser* self, struct java_lang_String *jString)
{
    CJavaCtrl* ctrl = getController(env, self);
    if (!ctrl)
        return NULL;

    const char * nodeName = JRI_GetStringUTFChars( env , jString );
    LPUNKNOWN pNode = ctrl->getCC3D()->getNode(nodeName);
    void * pUnknown = ctrl->getNodeQIPtr(pNode);

    if (!pUnknown) // invalid vrml
    {
        struct java_lang_Class * exClass = class_vrml_external_exception_InvalidNodeException( env );
        JRI_ThrowNew ( env , exClass , "Invalid VRML" );
        return NULL;
    }

    // convert to java
    struct java_lang_Class * nodeClass = class_vrml_external_Node( env );
    struct vrml_external_Node * node;
    node = vrml_external_Node_new(env, nodeClass, (jint)ctrl, (jint)pUnknown );
    
    return node;
}

/*** public native loadURL ([Ljava/lang/String;[Ljava/lang/String;)V ***/
extern "C" JRI_PUBLIC_API(void)
native_vrml_external_Browser_nLoadURL(JRIEnv* env, struct vrml_external_Browser* self, struct java_lang_String *jUrl, struct java_lang_String *jPar)
{
    CJavaCtrl* ctrl = getController(env, self);
    if (!ctrl)
        return;

    const char * url = JRI_GetStringUTFChars( env , jUrl );
    const char * par = JRI_GetStringUTFChars( env , jPar );
    ctrl->getCC3D()->loadURL(url, par);
}

/*** public native createVrmlFromString (Ljava/lang/String;)[Lvrml/external/Node; ***/
extern "C" JRI_PUBLIC_API(jref)
native_vrml_external_Browser_createVrmlFromString(JRIEnv* env, struct vrml_external_Browser* self, struct java_lang_String *jString)
{
    CJavaCtrl* ctrl = getController(env, self);
    if (!ctrl)
        return NULL;

    // get c handle
    const char * str = JRI_GetStringUTFChars( env , jString );
    LPUNKNOWN pVrml = ctrl->getCC3D()->createVrmlFromString(str);
    void * pScene = ctrl->getNodeQIPtr(pVrml);

    if (!pScene) // invalid vrml
    {
        struct java_lang_Class * exClass = class_vrml_external_exception_InvalidVrmlException( env );
        JRI_ThrowNew ( env , exClass , "Invalid VRML" );
        return NULL;
    }

    // cc3d provides a group node instead of a mfnode so we have to read all nodes by ourself
    int type;
    void * pUnknown = ctrl->getEventOut(pScene, "children_changed", &type);
    ctrl->release( pScene );
    pScene = NULL;

    if (!pUnknown) // no children
    {
        struct java_lang_Class * exClass = class_vrml_external_exception_InvalidVrmlException( env );
        JRI_ThrowNew ( env , exClass , "Invalid VRML" );
        return NULL;
    }
	
    // get the size first and create java array
    struct java_lang_Class * nodeClass = class_vrml_external_Node( env );
    int size = ctrl->getSize(pUnknown, type);
	jobjectArray jArray = (jobjectArray)JRI_NewObjectArray(env, size, nodeClass, NULL);

    struct vrml_external_Node * node;
    void * pNode;
    for (int i = 0 ; i < size ; i++)
	{
		pNode = ctrl->getNode(pUnknown, i, type);

		if (!pNode) // shouldn't happen
			break;

		node = vrml_external_Node_new(env, nodeClass, (jint)ctrl, (jint)pNode );
		JRI_SetObjectArrayElement(env, jArray, i, node);
	}

    ctrl->release( pUnknown );
    pUnknown = NULL;

    return jArray;
}

/*** public native createVrmlFromURL ([Ljava/lang/String;Lvrml/external/Node;Ljava/lang/String;)V ***/
extern "C" JRI_PUBLIC_API(void)
native_vrml_external_Browser_nCreateVrmlFromURL(JRIEnv* env, struct vrml_external_Browser* self, struct java_lang_String *jUrl, struct vrml_external_Node *node, struct java_lang_String *jEvent)
{
    CJavaCtrl* ctrl = getController(env, self);
    if (!ctrl)
        return;

    LPUNKNOWN nodeHdl = 0;
    if (node)
    {
        nodeHdl = (LPUNKNOWN)get_vrml_external_Node_cNode(env, node);
        if (!ctrl->checkRef((void *)nodeHdl, TypeNode)) // invalid handle
            nodeHdl = 0;
    }
    const char * event = JRI_GetStringUTFChars( env , jEvent );
    const char * url   = JRI_GetStringUTFChars( env , jUrl );

    ctrl->getCC3D()->createVrmlFromURL(url, (LPDISPATCH)nodeHdl, event);
    return;
}

/*** public native createNode (Ljava/lang/String;)Lvrml/external/Node; ***/
extern "C" JRI_PUBLIC_API(struct vrml_external_Node *)
native_vrml_external_Browser_createNode(JRIEnv* env, struct vrml_external_Browser* self, struct java_lang_String *jString)
{
    CJavaCtrl* ctrl = getController(env, self);
    if (!ctrl)
        return NULL;

    // get c handle
    const char * str = JRI_GetStringUTFChars( env , jString );
    LPUNKNOWN pNode = ctrl->getCC3D()->createNode(str);
    void * pUnknown = ctrl->getNodeQIPtr(pNode);

    if (!pUnknown) // invalid vrml
    {
        struct java_lang_Class * exClass = class_vrml_external_exception_InvalidVrmlException( env );
        JRI_ThrowNew ( env , exClass , "Invalid VRML" );
        return NULL;
    }

    // convert to java
    struct java_lang_Class * nodeClass = class_vrml_external_Node( env );
    struct vrml_external_Node * node;
    node = vrml_external_Node_new(env, nodeClass, (jint)ctrl, (jint)pUnknown );
    
    return node;
}

/*** public native addRoute (Lvrml/external/Node;Ljava/lang/String;Lvrml/external/Node;Ljava/lang/String;)V ***/
extern "C" JRI_PUBLIC_API(void)
native_vrml_external_Browser_addRoute(JRIEnv* env, struct vrml_external_Browser* self, struct vrml_external_Node *jFromNode, struct java_lang_String *jFromEvent, struct vrml_external_Node *jToNode, struct java_lang_String *jToEvent)
{
    CJavaCtrl* ctrl = getController(env, self);
    if (!ctrl)
        return;

    // check parameters
    LPUNKNOWN fromNode, toNode;
    bool wrongPar = false;
    if (!jFromNode || !jToNode || !jFromEvent || !jToEvent )
    {
        wrongPar = true;
    }
    else
    {
        fromNode = (LPUNKNOWN)get_vrml_external_Node_cNode(env, jFromNode);
        toNode   = (LPUNKNOWN)get_vrml_external_Node_cNode(env, jToNode);
        if (!ctrl->checkRef((void *)fromNode, TypeNode) || !ctrl->checkRef((void *)toNode, TypeNode)) // invalid handle
            wrongPar = true;
    }
    if (wrongPar)
    {
        struct java_lang_Class * exClass = class_java_lang_IllegalArgumentException( env );
        JRI_ThrowNew ( env , exClass , "Invalid Parameter" );
        return;
    }
    const char * fromEvent = JRI_GetStringUTFChars( env , jFromEvent );
    const char * toEvent   = JRI_GetStringUTFChars( env , jToEvent );

    // create route
    ctrl->getCC3D()->addRoute(fromNode, fromEvent, toNode, toEvent);
    return;
}

/*** public native deleteRoute (Lvrml/external/Node;Ljava/lang/String;Lvrml/external/Node;Ljava/lang/String;)V ***/
extern "C" JRI_PUBLIC_API(void)
native_vrml_external_Browser_deleteRoute(JRIEnv* env, struct vrml_external_Browser* self, struct vrml_external_Node *jFromNode, struct java_lang_String *jFromEvent, struct vrml_external_Node *jToNode, struct java_lang_String *jToEvent)
{
    CJavaCtrl* ctrl = getController(env, self);
    if (!ctrl)
        return;

    // check parameters
    LPUNKNOWN fromNode, toNode;
    bool wrongPar = false;
    if (!jFromNode || !jToNode || !jFromEvent || !jToEvent )
    {
        wrongPar = true;
    }
    else
    {
        fromNode = (LPUNKNOWN)get_vrml_external_Node_cNode(env, jFromNode);
        toNode   = (LPUNKNOWN)get_vrml_external_Node_cNode(env, jToNode);
        if (!ctrl->checkRef((void *)fromNode, TypeNode) || !ctrl->checkRef((void *)toNode, TypeNode)) // invalid handle
            wrongPar = true;
    }
    if (wrongPar)
    {
        struct java_lang_Class * exClass = class_java_lang_IllegalArgumentException( env );
        JRI_ThrowNew ( env , exClass , "Invalid Parameter" );
        return;
    }
    const char * fromEvent = JRI_GetStringUTFChars( env , jFromEvent );
    const char * toEvent   = JRI_GetStringUTFChars( env , jToEvent );

    // create route
    ctrl->getCC3D()->deleteRoute(fromNode, fromEvent, toNode, toEvent);
    return;
}

/*** public native replaceWorld ([Lvrml/external/Node;)V ***/
extern "C" JRI_PUBLIC_API(void)
native_vrml_external_Browser_replaceWorld(JRIEnv* env, struct vrml_external_Browser* self, struct vrml_external_Node *jNode)
{
    CJavaCtrl* ctrl = getController(env, self);
    if (!ctrl)
        return;

    LPUNKNOWN pNode;
    pNode  = (LPUNKNOWN)get_vrml_external_Node_cNode(env, jNode);
    if (ctrl->checkRef((void *)pNode, TypeNode)) // correct handle
        ctrl->getCC3D()->replaceWorld(pNode);
}

/*** public native addNode (Lvrml/external/Node;)V ***/
extern "C" JRI_PUBLIC_API(void)
native_vrml_external_Browser_addNode(JRIEnv* env, struct vrml_external_Browser* self, struct vrml_external_Node *node)
{
    CJavaCtrl* ctrl = getController(env, self);
    if (!ctrl)
        return;

    LPUNKNOWN nodeHdl = 0;
    if (node)
    {
        nodeHdl = (LPUNKNOWN)get_vrml_external_Node_cNode(env, node);
        if (!ctrl->checkRef((void *)nodeHdl, TypeNode)) // invalid handle
            nodeHdl = 0;
    }
    if (!nodeHdl)
    {
        struct java_lang_Class * exClass = class_java_lang_IllegalArgumentException( env );
        JRI_ThrowNew ( env , exClass , "Invalid Parameter" );
        return;
    }

    ctrl->getCC3D()->addNode(nodeHdl);
    return;
}

/*** public native removeNode (Lvrml/external/Node;)Z ***/
extern "C" JRI_PUBLIC_API(jbool)
native_vrml_external_Browser_removeNode(JRIEnv* env, struct vrml_external_Browser* self, struct vrml_external_Node *node)
{
    CJavaCtrl* ctrl = getController(env, self);
    if (!ctrl)
        return false;

    LPUNKNOWN nodeHdl = 0;
    if (node)
    {
        nodeHdl = (LPUNKNOWN)get_vrml_external_Node_cNode(env, node);
        if (!ctrl->checkRef((void *)nodeHdl, TypeNode)) // invalid handle
            nodeHdl = 0;
    }
    if (!nodeHdl)
    {
        struct java_lang_Class * exClass = class_java_lang_IllegalArgumentException( env );
        JRI_ThrowNew ( env , exClass , "Invalid Parameter" );
        return false;
    }

    return ctrl->getCC3D()->removeNode(nodeHdl);
}

/*** public native setNodeName (Lvrml/external/Node;Ljava/lang/String;)V ***/
extern "C" JRI_PUBLIC_API(void)
native_vrml_external_Browser_setNodeName(JRIEnv* env, struct vrml_external_Browser* self, struct vrml_external_Node *node, struct java_lang_String *jName)
{
    CJavaCtrl* ctrl = getController(env, self);
    if (!ctrl)
        return;

    LPUNKNOWN nodeHdl = 0;
    if (node)
    {
        nodeHdl = (LPUNKNOWN)get_vrml_external_Node_cNode(env, node);
        if (!ctrl->checkRef((void *)nodeHdl, TypeNode)) // invalid handle
            nodeHdl = 0;
    }
    if (!nodeHdl)
    {
        struct java_lang_Class * exClass = class_java_lang_IllegalArgumentException( env );
        JRI_ThrowNew ( env , exClass , "Invalid Parameter" );
        return;
    }
    const char * name = JRI_GetStringUTFChars( env , jName );

    ctrl->getCC3D()->setNodeName(nodeHdl, name);
    return;
}

/*** public native setTimerInterval (I)I ***/
extern "C" JRI_PUBLIC_API(jint)
native_vrml_external_Browser_setTimerInterval(JRIEnv* env, struct vrml_external_Browser* self, jint interval)
{
    CJavaCtrl* ctrl = getController(env, self);
    if (!ctrl)
        return 0;
    return (jint)ctrl->getCC3D()->setTimerInterval((long) interval);
}

/*** public native adviseOnUrlChanged (Lvrml/external/field/EventOutObserver;Ljava/lang/Object;)Lvrml/external/field/EventOut; ***/
extern "C" JRI_PUBLIC_API(struct vrml_external_field_EventOut *)
native_vrml_external_Browser_adviseOnUrlChanged(JRIEnv* env, struct vrml_external_Browser* self, struct vrml_external_field_EventOutObserver *observer, struct java_lang_Object *data)
{
    CJavaCtrl* ctrl = getController(env, self);
    if (!ctrl)
        return NULL;

    int type;
    void * pUnknown = ctrl->getEventOut(NULL, "worldUrl_changed", &type);
    if (!pUnknown) 
    {
        return NULL;
    }

    // convert to java
    struct java_lang_Class * eventClass = class_vrml_external_field_EventOut( env );
    struct vrml_external_field_EventOut * event;
    event = vrml_external_field_EventOut_newEventOut(env, eventClass, (jint)SFSTRING, (jint)ctrl, (jint)pUnknown );
    set_vrml_external_field_EventOut_userData(env, event, data);
    set_vrml_external_field_EventOut_observer(env, event, observer);
    ctrl->addRef(pUnknown, TypeEventOut);
	vrml_external_Browser_addObserver(env, self, event);
	ctrl->advise(pUnknown, (jref)event);
    return event;
}

/*** public native getNodeName (Lvrml/external/Node;)Ljava/lang/String; ***/
extern JRI_PUBLIC_API(struct java_lang_String *)
native_vrml_external_Browser_getNodeName(JRIEnv* env, struct vrml_external_Browser* self, struct vrml_external_Node *node)
{
    struct java_lang_String * retStr = NULL;
    CString cStr;

    CJavaCtrl* ctrl = getController(env, self);
    if (!ctrl)
        return retStr;


    LPUNKNOWN nodeHdl = 0;
    if (node)
    {
        nodeHdl = (LPUNKNOWN)get_vrml_external_Node_cNode(env, node);
        if (!ctrl->checkRef(nodeHdl, TypeNode)) // invalid handle
            nodeHdl = 0;
    }
    if (!nodeHdl)
    {
        struct java_lang_Class * exClass = class_vrml_external_exception_InvalidNodeException( env );
        JRI_ThrowNew ( env , exClass , "Invalid Node" );
        return retStr;
    }
    
    cStr = ctrl->getCC3D()->getNodeName(nodeHdl);

    retStr = JRI_NewStringUTF ( env , cStr , cStr.GetLength() );
    return retStr;
}

/*** public native getEventOut (Ljava/lang/String;)Lvrml/external/field/EventOut; ***/
extern JRI_PUBLIC_API(struct vrml_external_field_EventOut *)
native_vrml_external_Browser_getEventOut(JRIEnv* env, struct vrml_external_Browser* self, struct java_lang_String *jName)
{
    CJavaCtrl* ctrl = getController(env, self);
    if (!ctrl)
        return NULL;

    const char * name = JRI_GetStringUTFChars( env , jName );
    int type;
    void * pUnknown = ctrl->getEventOut(NULL, name, &type);
    if (!pUnknown) 
    {
        return NULL;
    }

    // convert to java
    struct java_lang_Class * eventClass = class_vrml_external_field_EventOut( env );
    struct vrml_external_field_EventOut * event;
    event = vrml_external_field_EventOut_newEventOut(env, eventClass, (jint)type, (jint)ctrl, (jint)pUnknown );
    
    return event;
}

/*** public native getEventIn (Ljava/lang/String;)Lvrml/external/field/EventIn; ***/
extern JRI_PUBLIC_API(struct vrml_external_field_EventIn *)
native_vrml_external_Browser_getEventIn(JRIEnv* env, struct vrml_external_Browser* self, struct java_lang_String *jName)
{
    CJavaCtrl* ctrl = getController(env, self);
    if (!ctrl)
        return NULL;

    int type;
    const char * name = JRI_GetStringUTFChars( env , jName );
    void * pUnknown = ctrl->getEventIn(NULL, name, &type);
    if (!pUnknown) 
    {
        return NULL;
    }

    // convert to java
    struct java_lang_Class * eventClass = class_vrml_external_field_EventIn( env );
    struct vrml_external_field_EventIn * event;
    //!!! which type???
    event = vrml_external_field_EventIn_newEventIn(env, eventClass, (jint)type, (jint)ctrl, (jint)pUnknown );
    
    return event;
}

/*** public native saveWorld (Ljava/lang/String;)Z ***/
extern JRI_PUBLIC_API(jbool)
native_vrml_external_Browser_saveWorld(JRIEnv* env, struct vrml_external_Browser* self, struct java_lang_String *jName)
{
    CJavaCtrl* ctrl = getController(env, self);
    if (!ctrl)
        return false;

    const char * name = JRI_GetStringUTFChars( env , jName );
    bool ret = false;
    if (ctrl->getCC3D()->saveWorld(name))
        ret = true;
    return ret;
}

/*** public native setFullscreen (Z)Z ***/
extern JRI_PUBLIC_API(jbool)
native_vrml_external_Browser_setFullscreen(JRIEnv* env, struct vrml_external_Browser* self, jbool jBool)
{
    CJavaCtrl* ctrl = getController(env, self);
    if (!ctrl)
        return false;

    bool ret = false;
    if (ctrl->getCC3D()->setFullscreen(jBool))
        ret = true;
    return ret;
}

/*** public native getUiMode ()I ***/
extern JRI_PUBLIC_API(jint)
native_vrml_external_Browser_getUiMode(JRIEnv* env, struct vrml_external_Browser* self)
{
    CJavaCtrl* ctrl = getController(env, self);
    if (!ctrl)
        return 0;

    jint ret = ctrl->getCC3D()->getUiMode();
    return ret;
}

/*** public native setUiMode (I)V ***/
extern JRI_PUBLIC_API(void)
native_vrml_external_Browser_setUiMode(JRIEnv* env, struct vrml_external_Browser* self, jint mode)
{
    CJavaCtrl* ctrl = getController(env, self);
    if (!ctrl)
        return;

    ctrl->getCC3D()->setUiMode(mode);
}

/*** public native openPreferences (I)V ***/
extern JRI_PUBLIC_API(void)
native_vrml_external_Browser_openPreferences(JRIEnv* env, struct vrml_external_Browser* self, jint flag)
{
    CJavaCtrl* ctrl = getController(env, self);
    if (!ctrl)
        return;

    ctrl->getCC3D()->openPreferences(flag);
}

/*** public native computeRayHit (Lvrml/external/Node;[F[FI)Lvrml/external/Node; ***/
extern JRI_PUBLIC_API(struct vrml_external_Node *)
native_vrml_external_Browser_computeRayHit(JRIEnv* env, struct vrml_external_Browser* self, struct vrml_external_Node *startNode, jfloatArray start, jfloatArray end, jint mode)
{
    struct vrml_external_Node * retNode = NULL;

    CJavaCtrl* ctrl = getController(env, self);
    if (!ctrl)
        return retNode;


    LPUNKNOWN nodeHdl = 0;
    LPUNKNOWN resHdl = 0;
    if (startNode)
    {
        nodeHdl = (LPUNKNOWN)get_vrml_external_Node_cNode(env, startNode);
        if (!ctrl->checkRef((void *)nodeHdl, TypeNode)) // invalid handle
            nodeHdl = 0;
    }
    if (JRI_GetFloatArrayLength(env, start) != 3
        || JRI_GetFloatArrayLength(env, end) != 3)
        return retNode;

    float *startP = JRI_GetFloatArrayElements(env, start);
    float *endP = JRI_GetFloatArrayElements(env, end);
    if (resHdl = ctrl->getCC3D()->computeRayHit(nodeHdl, startP[0], startP[1], startP[2],  endP[0], endP[1], endP[2], mode))
    {
        ctrl->addRef ( resHdl, TypeNode );
        struct java_lang_Class * nodeClass = class_vrml_external_Node( env );
        retNode = vrml_external_Node_new(env, nodeClass, (jint)ctrl, (jint)resHdl );
    }
    return retNode;
}

/*** public native setViewpointByValue ([F[FI)V ***/
extern JRI_PUBLIC_API(void)
native_vrml_external_Browser_setViewpointByValue(JRIEnv* env, struct vrml_external_Browser* self, jfloatArray pos, jfloatArray orient, jint mode)
{
    CJavaCtrl* ctrl = getController(env, self);
    if (!ctrl)
        return;

    if (JRI_GetFloatArrayLength(env, pos) != 3
        || JRI_GetFloatArrayLength(env, orient) != 4)
        return;
    float *posP = JRI_GetFloatArrayElements(env, pos);
    float *orientP = JRI_GetFloatArrayElements(env, orient);
    ctrl->getCC3D()->setViewpointByValue(posP[0], posP[1], posP[2], orientP[0], orientP[1], orientP[2], orientP[3], mode);
}

/*** public native getViewpointByValue ([F[FI)V ***/
extern JRI_PUBLIC_API(void)
native_vrml_external_Browser_getViewpointByValue(JRIEnv* env, struct vrml_external_Browser* self, jfloatArray pos, jfloatArray orient, jint mode)
{
    CJavaCtrl* ctrl = getController(env, self);
    if (!ctrl)
        return;

    if (JRI_GetFloatArrayLength(env, pos) != 3
        || JRI_GetFloatArrayLength(env, orient) != 4)
        return;
    float *posP = JRI_GetFloatArrayElements(env, pos);
    float *orientP = JRI_GetFloatArrayElements(env, orient);
    ctrl->getCC3D()->getViewpointByValue(&posP[0], &posP[1], &posP[2], &orientP[0], &orientP[1], &orientP[2], &orientP[3], mode);
    return;
}

/*** public native getNodeBoundingBox (Lvrml/external/Node;I[F[F)Z ***/
extern JRI_PUBLIC_API(jbool)
native_vrml_external_Browser_getNodeBoundingBox(JRIEnv* env, struct vrml_external_Browser* self, struct vrml_external_Node *node, jint mode, jfloatArray min, jfloatArray max)
{
    CJavaCtrl* ctrl = getController(env, self);
    if (!ctrl)
        return false;

    if (JRI_GetFloatArrayLength(env, min) != 3
        || JRI_GetFloatArrayLength(env, max) != 3)
        return false;

    LPUNKNOWN nodeHdl = 0;
    if (node)
    {
        nodeHdl = (LPUNKNOWN)get_vrml_external_Node_cNode(env, node);
        if (!ctrl->checkRef((void *)nodeHdl, TypeNode)) // invalid handle
            nodeHdl = 0;
    }
    if (!nodeHdl)
    {
        struct java_lang_Class * exClass = class_vrml_external_exception_InvalidNodeException( env );
        JRI_ThrowNew ( env , exClass , "Invalid Node" );
        return false;
    }

    boolean ret = false;
    float *minP = JRI_GetFloatArrayElements(env, min);
    float *maxP = JRI_GetFloatArrayElements(env, max);
    if (ctrl->getCC3D()->getNodeBoundingBox(nodeHdl, mode, &minP[0], &minP[1], &minP[2], &maxP[0], &maxP[1], &maxP[2]))
        ret = true;
    return ret;
}

/*** public native setMyAvatarNode (Lvrml/external/Node;)V ***/
extern JRI_PUBLIC_API(void)
native_vrml_external_Browser_setMyAvatarNode(JRIEnv* env, struct vrml_external_Browser* self, struct vrml_external_Node *node)
{
    CJavaCtrl* ctrl = getController(env, self);
    if (!ctrl)
        return;

    LPUNKNOWN nodeHdl = 0;
    if (node)
    {
        nodeHdl = (LPUNKNOWN)get_vrml_external_Node_cNode(env, node);
        if (!ctrl->checkRef((void *)nodeHdl, TypeNode)) // invalid handle
            nodeHdl = 0;
    }
    if (!nodeHdl)
    {
        struct java_lang_Class * exClass = class_vrml_external_exception_InvalidNodeException( env );
        JRI_ThrowNew ( env , exClass , "Invalid Node" );
        return ;
    }
    ctrl->getCC3D()->setMyAvatarNode(nodeHdl);
}

/*** public native getMyAvatarNode ()Lvrml/external/Node; ***/
extern JRI_PUBLIC_API(struct vrml_external_Node *)
native_vrml_external_Browser_getMyAvatarNode(JRIEnv* env, struct vrml_external_Browser* self)
{
    CJavaCtrl* ctrl = getController(env, self);
    if (!ctrl)
        return NULL;

    LPUNKNOWN pNode = ctrl->getCC3D()->getMyAvatarNode();
    void * pUnknown = ctrl->getNodeQIPtr(pNode);

    if (!pUnknown) // invalid vrml
    {
        struct java_lang_Class * exClass = class_vrml_external_exception_InvalidNodeException( env );
        JRI_ThrowNew ( env , exClass , "Invalid Node" );
        return NULL;
    }

    // convert to java
    struct java_lang_Class * nodeClass = class_vrml_external_Node( env );
    struct vrml_external_Node * node;
    node = vrml_external_Node_new(env, nodeClass, (jint)ctrl, (jint)pUnknown );
    
    return node;
}

/*** public native setViewpointFollow (Lvrml/external/Node;[FI)Z ***/
extern JRI_PUBLIC_API(jbool)
native_vrml_external_Browser_setViewpointFollow(JRIEnv* env, struct vrml_external_Browser* self, struct vrml_external_Node *node, jfloatArray ref, jint mode)
{
    CJavaCtrl* ctrl = getController(env, self);
    if (!ctrl)
        return false;

    if (JRI_GetFloatArrayLength(env, ref) != 3)
        return false;

    LPUNKNOWN nodeHdl = 0;
    if (node)
    {
        nodeHdl = (LPUNKNOWN)get_vrml_external_Node_cNode(env, node);
        if (!ctrl->checkRef((void *)nodeHdl, TypeNode)) // invalid handle
            nodeHdl = 0;
    }
    if (!nodeHdl)
    {
        struct java_lang_Class * exClass = class_vrml_external_exception_InvalidNodeException( env );
        JRI_ThrowNew ( env , exClass , "Invalid Node" );
        return false;
    }

    boolean ret = false;
    float *refP = JRI_GetFloatArrayElements(env, ref);
    if (ctrl->getCC3D()->setViewpointFollow(nodeHdl, refP[0], refP[1], refP[2], mode))
        ret = true;
    return ret;
}

/*** public native setNodeEventIn (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z ***/
extern JRI_PUBLIC_API(jbool)
native_vrml_external_Browser_setNodeEventIn(JRIEnv* env, struct vrml_external_Browser* self, struct java_lang_String *node, struct java_lang_String *event, struct java_lang_String *value)
{
    CJavaCtrl* ctrl = getController(env, self);
    if (!ctrl)
        return false;

    const char * nodeName = JRI_GetStringUTFChars( env , node );
    const char * evName = JRI_GetStringUTFChars( env , event );
    const char * valP = JRI_GetStringUTFChars( env , value );

    boolean ret = false;
    if (ctrl->getCC3D()->setNodeEventIn(nodeName, evName, valP))
        ret = true;
    return ret;
}

/*** public native getNodeEventOut (Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String; ***/
extern JRI_PUBLIC_API(struct java_lang_String *)
native_vrml_external_Browser_getNodeEventOut(JRIEnv* env, struct vrml_external_Browser* self, struct java_lang_String *node, struct java_lang_String *event)
{
    CJavaCtrl* ctrl = getController(env, self);
    if (!ctrl)
        return NULL;

    const char * nodeName = JRI_GetStringUTFChars( env , node );
    const char * evName = JRI_GetStringUTFChars( env , event );

    struct java_lang_String * retStr = NULL;
    CString cStr;

    if (cStr = ctrl->getCC3D()->getNodeEventOut(nodeName, evName)) 
	{
        retStr = JRI_NewStringUTF ( env , cStr , cStr.GetLength() );
    }
    
    return retStr;
}

/*** public native getObject (I)I ***/
extern JRI_PUBLIC_API(jint)
native_vrml_external_Browser_getObject(JRIEnv* env, struct vrml_external_Browser* self, jint id)
{
    CJavaCtrl* ctrl = getController(env, self);
    if (!ctrl)
        return 0;
    LPUNKNOWN pUnknown;
    if (pUnknown = ctrl->getCC3D()->getObject(id))
        return (jint)pUnknown;
    else
        return 0;

}
