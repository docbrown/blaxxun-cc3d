/* ----   file information  -------------------------------------------------
   Revison:     $Revision: 1.4 $
   Date:        $Date: 1998/10/07 17:17:12 $
   Modification History:
   $Log: node.cpp,v $
   Revision 1.4  1998/10/07 17:17:12  kristof
   use void *

   Revision 1.3  1998/05/05 16:23:37  kristof
   gettype added

   Revision 1.2  1997/12/15 12:44:54  kristof
   *** empty log message ***

   Revision 1.1  1997/12/10 10:33:57  kristof
   Initial revision

   ----   file information  ---------------------------------------------- */

#include "stdafx.h"

#define IMPLEMENT_vrml_external_Node
#define IMPLEMENT_vrml_external_field_EventIn
#define IMPLEMENT_vrml_external_field_EventOut

//#define IMPLEMENT_vrml_external_exception_InvalidNodeException

#ifndef _vrml_external_Node_H_
#include "vrml_external_Node.h"
#endif
#ifndef _vrml_external_field_EventIn_H_
#include "vrml_external_field_EventIn.h"
#endif
#ifndef _vrml_external_field_EventOut_H_
#include "vrml_external_field_EventOut.h"
#endif
#ifndef _vrml_external_exception_InvalidEventInException_H_
#include "vrml_external_exception_InvalidEventInException.h"
#endif
#ifndef _vrml_external_exception_InvalidEventOutException_H_
#include "vrml_external_exception_InvalidEventOutException.h"
#endif

#ifndef _NPBX3D_H_
#include "npbx3d.h"
#endif

#ifndef _JAVACTRL_H_
#include "javactrl.h"
#endif

/*** public native getType ()Ljava/lang/String; ***/
extern JRI_PUBLIC_API(struct java_lang_String *)
native_vrml_external_Node_getType(JRIEnv* env, struct vrml_external_Node* self)
{
    struct java_lang_String * retStr = NULL;
    CString cStr;

    // check whether control is still active
    CJavaCtrl* ctrl = (CJavaCtrl*)get_vrml_external_Node_cBrowser(env, self);
    if (!CJavaCtrl::checkController(ctrl))
        return retStr;

    // check whether node handle is correct
    void * nodeHdl = (void *)get_vrml_external_Node_cNode(env, self);
    if (!nodeHdl || !ctrl->checkRef(nodeHdl, TypeNode)) // invalid handle
        return retStr;

    cStr = ctrl->getNodeType(nodeHdl);

    retStr = JRI_NewStringUTF ( env , cStr , cStr.GetLength() );
    return retStr;
}

/*** public native getEventIn (Ljava/lang/String;)Lvrml/external/field/EventIn; ***/
extern JRI_PUBLIC_API(struct vrml_external_field_EventIn *)
native_vrml_external_Node_getEventIn(JRIEnv* env, struct vrml_external_Node* self, struct java_lang_String *jEvent)
{
    // check whether control is still active
    CJavaCtrl* ctrl = (CJavaCtrl*)get_vrml_external_Node_cBrowser(env, self);
    if (!CJavaCtrl::checkController(ctrl))
        return NULL;

    // check whether node handle is correct
    void * nodeHdl = (void *)get_vrml_external_Node_cNode(env, self);
    if (!nodeHdl || !ctrl->checkRef(nodeHdl, TypeNode)) // invalid handle
        return NULL;

    const char * eventStr = JRI_GetStringUTFChars( env , jEvent );
    int type;
    void * pUnknown = ctrl->getEventIn(nodeHdl, eventStr, &type);
    if (!pUnknown)
    {
        struct java_lang_Class * exClass = class_vrml_external_exception_InvalidEventInException( env );
        JRI_ThrowNew ( env , exClass , "Invalid EventIn" );
        return NULL;
    }

    // convert to java
    struct java_lang_Class * eventClass = class_vrml_external_field_EventIn( env );
    struct vrml_external_field_EventIn * event;
    event = vrml_external_field_EventIn_newEventIn(env, eventClass, (jint)type, (jint)ctrl, (jint)pUnknown );
    
    return event;
}

/*** public native getEventOut (Ljava/lang/String;)Lvrml/external/field/EventOut; ***/
extern JRI_PUBLIC_API(struct vrml_external_field_EventOut *)
native_vrml_external_Node_getEventOut(JRIEnv* env, struct vrml_external_Node* self, struct java_lang_String *jEvent)
{
    // check whether control is still active
    CJavaCtrl* ctrl = (CJavaCtrl*)get_vrml_external_Node_cBrowser(env, self);
    if (!CJavaCtrl::checkController(ctrl))
        return NULL;

    // check whether node handle is correct
    void * nodeHdl = (void *)get_vrml_external_Node_cNode(env, self);
    if (!nodeHdl || !ctrl->checkRef(nodeHdl, TypeNode)) // invalid handle
        return NULL;

    const char * eventStr = JRI_GetStringUTFChars( env , jEvent );
    int type;
    void * pUnknown = ctrl->getEventOut(nodeHdl, eventStr, &type);
    if (!pUnknown)
    {
        struct java_lang_Class * exClass = class_vrml_external_exception_InvalidEventOutException( env );
        JRI_ThrowNew ( env , exClass , "Invalid EventOut" );
        return NULL;
    }

    // convert to java
    struct java_lang_Class * eventClass = class_vrml_external_field_EventOut( env );
    struct vrml_external_field_EventOut * event;
    event = vrml_external_field_EventOut_newEventOut(env, eventClass, (jint)type, (jint)ctrl, (jint)pUnknown );
    
    return event;
}

/*** private native nFinalize ()V ***/
extern JRI_PUBLIC_API(void)
native_vrml_external_Node_nFinalize(JRIEnv* env, struct vrml_external_Node* self)
{
    // check whether control is still active
    CJavaCtrl* ctrl = (CJavaCtrl*)get_vrml_external_Node_cBrowser(env, self);
    if (!CJavaCtrl::checkController(ctrl))
        return;

    void * nodeHdl = (void *)get_vrml_external_Node_cNode(env, self);
	ctrl->release(nodeHdl);
}
