/* ----   file information  -------------------------------------------------
   Revison:     $Revision: 1.3 $
   Date:        $Date: 1998/10/07 17:17:00 $
   Modification History:
   $Log: eventout.cpp,v $
   Revision 1.3  1998/10/07 17:17:00  kristof
   use void *

   Revision 1.2  1997/12/15 12:44:49  kristof
   *** empty log message ***

   Revision 1.1  1997/12/10 10:33:50  kristof
   Initial revision

   ----   file information  ---------------------------------------------- */

#include "stdafx.h"

#define IMPLEMENT_vrml_external_field_EventOut
#define IMPLEMENT_vrml_external_Node
#define IMPLEMENT_vrml_external_Browser

#ifndef _vrml_external_Browser_H_
#include "vrml_external_Browser.h"
#endif
#ifndef _vrml_external_Node_H_
#include "vrml_external_Node.h"
#endif
#ifndef _java_lang_IllegalArgumentException_H_
#include "java_lang_IllegalArgumentException.h"
#endif
#ifndef _vrml_external_field_EventIn_H_
#include "vrml_external_field_EventIn.h"
#endif
#ifndef _vrml_external_field_EventOut_H_
#include "vrml_external_field_EventOut.h"
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

/*** private native advise ()V ***/
extern JRI_PUBLIC_API(void)
native_vrml_external_field_EventOut_advise_1(JRIEnv* env, struct vrml_external_field_EventOut* self)
{
    // check whether control is still active
    CJavaCtrl* ctrl = (CJavaCtrl*)get_vrml_external_field_EventOut_cBrowser(env, self);
    if (!CJavaCtrl::checkController(ctrl))
        return;

	vrml_external_Browser_addObserver(env, (vrml_external_Browser *)ctrl->getBrowser(), self);
    void * pEvent = (void *)get_vrml_external_field_EventOut_cEvent(env, self);
	ctrl->advise(pEvent, (jref)self);
}

/*** protected native getString ([Ljava/lang/String;)V ***/
extern JRI_PUBLIC_API(void)
native_vrml_external_field_EventOut_getString(JRIEnv* env, struct vrml_external_field_EventOut* self, jstringArray jArray)
{
    // check whether control is still active
    CJavaCtrl* ctrl = (CJavaCtrl*)get_vrml_external_field_EventOut_cBrowser(env, self);
    if (!CJavaCtrl::checkController(ctrl))
        return;

    void * pEvent = (void *)get_vrml_external_field_EventOut_cEvent(env, self);
    jint type        = get_vrml_external_field_EventOut_type(env, self);
    int  size        = JRI_GetObjectArrayLength(env, jArray);
	CString cStr;
    struct java_lang_String * jStr = NULL;
	for (int i = 0 ; i < size ; i++)
	{
		cStr = ctrl->getString(pEvent, i, type);
		jStr = JRI_NewStringUTF ( env , cStr , cStr.GetLength() );
		JRI_SetObjectArrayElement(env, jArray, i, jStr);
	}
}

/*** protected native get1String (I)Ljava/lang/String; ***/
extern JRI_PUBLIC_API(struct java_lang_String *)
native_vrml_external_field_EventOut_get1String(JRIEnv* env, struct vrml_external_field_EventOut* self, jint index)
{
    struct java_lang_String * ret = NULL;
    // check whether control is still active
    CJavaCtrl* ctrl = (CJavaCtrl*)get_vrml_external_field_EventOut_cBrowser(env, self);
    if (!CJavaCtrl::checkController(ctrl))
        return ret;

    void * pEvent = (void *)get_vrml_external_field_EventOut_cEvent(env, self);
    jint type        = get_vrml_external_field_EventOut_type(env, self);
    CString cStr = ctrl->getString(pEvent, index, type);
    ret = JRI_NewStringUTF ( env , cStr , cStr.GetLength() );
    return ret;
}

/*** protected native getNode ([Lvrml/external/Node;)V ***/
extern JRI_PUBLIC_API(void)
native_vrml_external_field_EventOut_getNode(JRIEnv* env, struct vrml_external_field_EventOut* self, jobjectArray jArray)
{
    // check whether control is still active
    CJavaCtrl* ctrl = (CJavaCtrl*)get_vrml_external_field_EventOut_cBrowser(env, self);
    if (!CJavaCtrl::checkController(ctrl))
        return;

    void * pEvent = (void *)get_vrml_external_field_EventOut_cEvent(env, self);
    jint type        = get_vrml_external_field_EventOut_type(env, self);
    int  size        = JRI_GetObjectArrayLength(env, jArray);
	
	struct java_lang_Class * nodeClass = class_vrml_external_Node( env );
    struct vrml_external_Node * node;
	void * pUnknown;
	for (int i = 0 ; i < size ; i++)
	{
		pUnknown = ctrl->getNode(pEvent, i, type);

		if (!pUnknown) // array finished
			break;

		node = vrml_external_Node_new(env, nodeClass, (jint)ctrl, (jint)pUnknown );
		JRI_SetObjectArrayElement(env, jArray, i, node);
	}
}

/*** protected native get1Node (I)Lvrml/external/Node; ***/
extern JRI_PUBLIC_API(struct vrml_external_Node *)
native_vrml_external_field_EventOut_get1Node(JRIEnv* env, struct vrml_external_field_EventOut* self, jint index)
{
    struct vrml_external_Node * node = NULL;
    // check whether control is still active
    CJavaCtrl* ctrl = (CJavaCtrl*)get_vrml_external_field_EventOut_cBrowser(env, self);
    if (!CJavaCtrl::checkController(ctrl))
        return node;

    void * pEvent   = (void *)get_vrml_external_field_EventOut_cEvent(env, self);
    jint type          = get_vrml_external_field_EventOut_type(env, self);
    void * pUnknown = ctrl->getNode(pEvent, index, type);

    if (!pUnknown) // invalid node
        return NULL;

    // convert to java
    struct java_lang_Class * nodeClass = class_vrml_external_Node( env );
    node = vrml_external_Node_new(env, nodeClass, (jint)ctrl, (jint)pUnknown );
    
    return node;
}

/*** protected native getInt ([I)V ***/
extern JRI_PUBLIC_API(void)
native_vrml_external_field_EventOut_getInt(JRIEnv* env, struct vrml_external_field_EventOut* self, jintArray jArray)
{
    // check whether control is still active
    CJavaCtrl* ctrl = (CJavaCtrl*)get_vrml_external_field_EventOut_cBrowser(env, self);
    if (!CJavaCtrl::checkController(ctrl))
        return;

    void * pEvent = (void *)get_vrml_external_field_EventOut_cEvent(env, self);
    jint type        = get_vrml_external_field_EventOut_type(env, self);
    int  size        = JRI_GetIntArrayLength(env, jArray);
	long * ret		 = JRI_GetIntArrayElements(env, jArray);
	int  * val       = (int *)malloc ( size * sizeof(int) );

	ctrl->getInt(pEvent, val, size, 0, type);
	for (int i = 0 ; i < size ; i++)
		ret[i] = (long)val[i];
    free(val);
}

/*** protected native get1Int (I)I ***/
extern JRI_PUBLIC_API(jint)
native_vrml_external_field_EventOut_get1Int(JRIEnv* env, struct vrml_external_field_EventOut* self, jint index)
{
    int ret = 0;
    // check whether control is still active
    CJavaCtrl* ctrl = (CJavaCtrl*)get_vrml_external_field_EventOut_cBrowser(env, self);
    if (!CJavaCtrl::checkController(ctrl))
        return ret;

    void * pEvent = (void *)get_vrml_external_field_EventOut_cEvent(env, self);
    jint type        = get_vrml_external_field_EventOut_type(env, self);
	ctrl->getInt(pEvent, &ret, 1, index, type);
    return ret;
}

/*** protected native getFloat (I[F)V ***/
extern JRI_PUBLIC_API(void)
native_vrml_external_field_EventOut_getFloat(JRIEnv* env, struct vrml_external_field_EventOut* self, jint index, jfloatArray jArray)
{
    // check whether control is still active
    CJavaCtrl* ctrl = (CJavaCtrl*)get_vrml_external_field_EventOut_cBrowser(env, self);
    if (!CJavaCtrl::checkController(ctrl))
        return;

    void * pEvent = (void *)get_vrml_external_field_EventOut_cEvent(env, self);
    jint type        = get_vrml_external_field_EventOut_type(env, self);
    int  size        = JRI_GetFloatArrayLength(env, jArray);
	float * ret		 = JRI_GetFloatArrayElements(env, jArray);

	ctrl->getFloat(pEvent, ret, size, index, type);
}

/*** protected native get1Float (I)F ***/
extern JRI_PUBLIC_API(jfloat)
native_vrml_external_field_EventOut_get1Float(JRIEnv* env, struct vrml_external_field_EventOut* self, jint index)
{
    float ret = 0.0f;
    // check whether control is still active
    CJavaCtrl* ctrl = (CJavaCtrl*)get_vrml_external_field_EventOut_cBrowser(env, self);
    if (!CJavaCtrl::checkController(ctrl))
        return ret;

    void * pEvent = (void *)get_vrml_external_field_EventOut_cEvent(env, self);
    jint type        = get_vrml_external_field_EventOut_type(env, self);

	ctrl->getFloat(pEvent, &ret, 1, index, type);
    return ret;
}

/*** protected native getSize ()I ***/
extern JRI_PUBLIC_API(jint)
native_vrml_external_field_EventOut_getSize(JRIEnv* env, struct vrml_external_field_EventOut* self)
{
    int ret = 0;
    // check whether control is still active
    CJavaCtrl* ctrl = (CJavaCtrl*)get_vrml_external_field_EventOut_cBrowser(env, self);
    if (!CJavaCtrl::checkController(ctrl))
        return ret;

    void * pEvent = (void *)get_vrml_external_field_EventOut_cEvent(env, self);
    jint type        = get_vrml_external_field_EventOut_type(env, self);

	ret = ctrl->getSize(pEvent, type);
    return ret;
}

/*** protected native getTime ()D ***/
extern JRI_PUBLIC_API(jdouble)
native_vrml_external_field_EventOut_getTime(JRIEnv* env, struct vrml_external_field_EventOut* self)
{
    // check whether control is still active
    double ret = 0.0f;
    CJavaCtrl* ctrl = (CJavaCtrl*)get_vrml_external_field_EventOut_cBrowser(env, self);
    if (!CJavaCtrl::checkController(ctrl))
        return ret;

    void * pEvent = (void *)get_vrml_external_field_EventOut_cEvent(env, self);
    jint type        = get_vrml_external_field_EventOut_type(env, self);

	ret = ctrl->getDouble(pEvent, type);
    return ret;
}

/*** protected native getBoolean ()Z ***/
extern JRI_PUBLIC_API(jbool)
native_vrml_external_field_EventOut_getBoolean(JRIEnv* env, struct vrml_external_field_EventOut* self)
{
	bool ret = false;
    // check whether control is still active
    CJavaCtrl* ctrl = (CJavaCtrl*)get_vrml_external_field_EventOut_cBrowser(env, self);
    if (!CJavaCtrl::checkController(ctrl))
        return ret;

    void * pEvent = (void *)get_vrml_external_field_EventOut_cEvent(env, self);
    jint type        = get_vrml_external_field_EventOut_type(env, self);

	ret = ctrl->getBool(pEvent, type);
    return ret;
}

/*** protected native getByte ([B)V ***/
extern JRI_PUBLIC_API(void)
native_vrml_external_field_EventOut_getByte(JRIEnv* env, struct vrml_external_field_EventOut* self, jbyteArray jArray)
{
    // check whether control is still active
    CJavaCtrl* ctrl = (CJavaCtrl*)get_vrml_external_field_EventOut_cBrowser(env, self);
    if (!CJavaCtrl::checkController(ctrl))
        return;

    void * pEvent = (void *)get_vrml_external_field_EventOut_cEvent(env, self);
    jint type        = get_vrml_external_field_EventOut_type(env, self);
    int  size        = JRI_GetByteArrayLength(env, jArray);
	char * ret		 = JRI_GetByteArrayElements(env, jArray);

	ctrl->getByte(pEvent, ret, size, type);
}

/*** private native nFinalize ()V ***/
extern JRI_PUBLIC_API(void)
native_vrml_external_field_EventOut_nFinalize(JRIEnv* env, struct vrml_external_field_EventOut* self)
{
    // check whether control is still active
    CJavaCtrl* ctrl = (CJavaCtrl*)get_vrml_external_field_EventOut_cBrowser(env, self);
    if (!CJavaCtrl::checkController(ctrl))
        return;

    void * pEvent = (void *)get_vrml_external_field_EventOut_cEvent(env, self);
	ctrl->release(pEvent);
}

