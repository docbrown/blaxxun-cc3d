/* ----   file information  -------------------------------------------------
   Revison:     $Revision: 1.7 $
   Date:        $Date: 1999/05/10 11:57:53 $
   Modification History:
   $Log: eventin.cpp,v $
   Revision 1.7  1999/05/10 11:57:53  kristof
   set mfstring corrected

   Revision 1.6  1999/03/25 16:09:12  kristof
   *** empty log message ***

   Revision 1.5  1998/11/12 17:23:43  kristof
   mffields corrected

   Revision 1.4  1998/10/07 17:16:54  kristof
   use void *

   Revision 1.3  1998/03/17 14:05:04  holger
   empty message

   Revision 1.2  1997/12/15 12:44:48  kristof
   *** empty log message ***

   ----   file information  ---------------------------------------------- */

#include "stdafx.h"

#define IMPLEMENT_vrml_external_field_EventIn
#define IMPLEMENT_vrml_external_Node

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

/*** protected native setString (I[Ljava/lang/String;)V ***/
extern JRI_PUBLIC_API(void)
native_vrml_external_field_EventIn_setString(JRIEnv* env, struct vrml_external_field_EventIn* self, jint len, jstringArray jArray)
{
    // check whether control is still active
    CJavaCtrl* ctrl = (CJavaCtrl*)get_vrml_external_field_EventIn_cBrowser(env, self);
    if (!CJavaCtrl::checkController(ctrl))
        return;

    void * pEvent = (void *)get_vrml_external_field_EventIn_cEvent(env, self);
    jint type          = get_vrml_external_field_EventIn_type(env, self);
	struct java_lang_String * jString; 

    if ( len > 64 )
    {
	    for (int i = 0 ; i < len ; i++)
	    {
		    jString = (struct java_lang_String *)JRI_GetObjectArrayElement(env, jArray, i);
		    const char * string = JRI_GetStringUTFChars( env , jString );
            ctrl->setString(pEvent, string, i, type);
        }
    }
    else
    {
        CComBSTR tmpBstr[64];
	    for (int i = 0 ; i < len ; i++)
	    {
		    jString = (struct java_lang_String *)JRI_GetObjectArrayElement(env, jArray, i);
		    const char * string = JRI_GetStringUTFChars( env , jString );
            tmpBstr[i] = string; 
        }
        ((EventInMFString *)pEvent)->setValue(len, (BSTR *)tmpBstr);
    }
}

/*** protected native set1String (ILjava/lang/String;)V ***/
extern JRI_PUBLIC_API(void)
native_vrml_external_field_EventIn_set1String(JRIEnv* env, struct vrml_external_field_EventIn* self, jint index, struct java_lang_String *jString)
{
    // check whether control is still active
    CJavaCtrl* ctrl = (CJavaCtrl*)get_vrml_external_field_EventIn_cBrowser(env, self);
    if (!CJavaCtrl::checkController(ctrl))
        return;

    const char * string = JRI_GetStringUTFChars( env , jString );
    void * pEvent = (void *)get_vrml_external_field_EventIn_cEvent(env, self);
    jint type = get_vrml_external_field_EventIn_type(env, self);
    ctrl->setString(pEvent, string, index, type);
}

/*** protected native setNode (I[Lvrml/external/Node;)V ***/
extern JRI_PUBLIC_API(void)
native_vrml_external_field_EventIn_setNode(JRIEnv* env, struct vrml_external_field_EventIn* self, jint len, jobjectArray jArray)
{
    // check whether control is still active
    CJavaCtrl* ctrl = (CJavaCtrl*)get_vrml_external_field_EventIn_cBrowser(env, self);
    if (!CJavaCtrl::checkController(ctrl))
        return;

    /*void * pNode[512];
    if ( len > 512 )
        len = 512;*/
    void * pNode;
    void * pEvent = (void *)get_vrml_external_field_EventIn_cEvent(env, self);
    jint type        = get_vrml_external_field_EventIn_type(env, self);
	struct vrml_external_Node * jNode; 
	for (int i = 0 ; i < len ; i++)
	{
		jNode  = (struct vrml_external_Node *)JRI_GetObjectArrayElement(env, jArray, i);
        //pNode[i]  = (void *)get_vrml_external_Node_cNode(env, jNode);
        pNode  = (void *)get_vrml_external_Node_cNode(env, jNode);
        ctrl->setNode(pEvent, pNode, i, type);
    }
    //ctrl->setNodeArray(pEvent, pNode, len);
}

/*** protected native set1Node (ILvrml/external/Node;)V ***/
extern JRI_PUBLIC_API(void)
native_vrml_external_field_EventIn_set1Node(JRIEnv* env, struct vrml_external_field_EventIn* self, jint index, struct vrml_external_Node *jNode)
{
    // check whether control is still active
    CJavaCtrl* ctrl = (CJavaCtrl*)get_vrml_external_field_EventIn_cBrowser(env, self);
    if (!CJavaCtrl::checkController(ctrl))
        return;
    
    void * pEvent = (void *)get_vrml_external_field_EventIn_cEvent(env, self);
    void * pNode  = (void *)get_vrml_external_Node_cNode(env, jNode);
    jint type        = get_vrml_external_field_EventIn_type(env, self);
    ctrl->setNode(pEvent, pNode, index, type);
}

/*** protected native setInt (I[I)V ***/
extern JRI_PUBLIC_API(void)
native_vrml_external_field_EventIn_setInt(JRIEnv* env, struct vrml_external_field_EventIn* self, jint len, jintArray jArray)
{
    // check whether control is still active
    CJavaCtrl* ctrl = (CJavaCtrl*)get_vrml_external_field_EventIn_cBrowser(env, self);
    if (!CJavaCtrl::checkController(ctrl))
        return;
    
    void * pEvent = (void *)get_vrml_external_field_EventIn_cEvent(env, self);
    jint type        = get_vrml_external_field_EventIn_type(env, self);
    long *val        = JRI_GetIntArrayElements(env, jArray);
    ctrl->setInt(pEvent, (int *)val, -1, len, type);
}

/*** protected native set1Int (II)V ***/
extern JRI_PUBLIC_API(void)
native_vrml_external_field_EventIn_set1Int(JRIEnv* env, struct vrml_external_field_EventIn* self, jint index, jint jVal)
{
    // check whether control is still active
    CJavaCtrl* ctrl = (CJavaCtrl*)get_vrml_external_field_EventIn_cBrowser(env, self);
    if (!CJavaCtrl::checkController(ctrl))
        return;

    void * pEvent = (void *)get_vrml_external_field_EventIn_cEvent(env, self);
    jint type        = get_vrml_external_field_EventIn_type(env, self);
    int val = (int)jVal;
    ctrl->setInt(pEvent, &val, index, -1, type);
}

/*** protected native setFloat (IF)V ***/
extern JRI_PUBLIC_API(void)
native_vrml_external_field_EventIn_setFloat(JRIEnv* env, struct vrml_external_field_EventIn* self, jint index, jfloat jVal)
{
    // check whether control is still active
    CJavaCtrl* ctrl = (CJavaCtrl*)get_vrml_external_field_EventIn_cBrowser(env, self);
    if (!CJavaCtrl::checkController(ctrl))
        return;

    void * pEvent = (void *)get_vrml_external_field_EventIn_cEvent(env, self);
    jint type        = get_vrml_external_field_EventIn_type(env, self);
    ctrl->setFloat(pEvent, jVal, index, type);
}

/*** protected native setFloatArray (I[F)V ***/
extern JRI_PUBLIC_API(void)
native_vrml_external_field_EventIn_setFloatArray(JRIEnv* env, struct vrml_external_field_EventIn* self, jint len, jfloatArray jArray)
{
    // check whether control is still active
    CJavaCtrl* ctrl = (CJavaCtrl*)get_vrml_external_field_EventIn_cBrowser(env, self);
    if (!CJavaCtrl::checkController(ctrl))
        return;

    void * pEvent = (void *)get_vrml_external_field_EventIn_cEvent(env, self);
    jint type        = get_vrml_external_field_EventIn_type(env, self);
    float *val       = JRI_GetFloatArrayElements(env, jArray);
    ctrl->setFloat(pEvent, val, len, -1, type);
}

/*** protected native set1Float (I[F)V ***/
extern JRI_PUBLIC_API(void)
native_vrml_external_field_EventIn_set1Float(JRIEnv* env, struct vrml_external_field_EventIn* self, jint index, jfloatArray jArray)
{
    // check whether control is still active
    CJavaCtrl* ctrl = (CJavaCtrl*)get_vrml_external_field_EventIn_cBrowser(env, self);
    if (!CJavaCtrl::checkController(ctrl))
        return;

    void * pEvent = (void *)get_vrml_external_field_EventIn_cEvent(env, self);
    jint type        = get_vrml_external_field_EventIn_type(env, self);
    float *val       = JRI_GetFloatArrayElements(env, jArray);
    int   size       = JRI_GetFloatArrayLength(env, jArray);
    ctrl->setFloat(pEvent, val, size, index, type);
}

/*** protected native setTime (D)V ***/
extern JRI_PUBLIC_API(void)
native_vrml_external_field_EventIn_setTime(JRIEnv* env, struct vrml_external_field_EventIn* self, jdouble jVal)
{
    // check whether control is still active
    CJavaCtrl* ctrl = (CJavaCtrl*)get_vrml_external_field_EventIn_cBrowser(env, self);
    if (!CJavaCtrl::checkController(ctrl))
        return;

    void * pEvent = (void *)get_vrml_external_field_EventIn_cEvent(env, self);
    jint type        = get_vrml_external_field_EventIn_type(env, self);
    ctrl->setDouble(pEvent, jVal, type);
}

/*** protected native setBoolean (Z)V ***/
extern JRI_PUBLIC_API(void)
native_vrml_external_field_EventIn_setBoolean(JRIEnv* env, struct vrml_external_field_EventIn* self, jbool jVal)
{
    // check whether control is still active
    CJavaCtrl* ctrl = (CJavaCtrl*)get_vrml_external_field_EventIn_cBrowser(env, self);
    if (!CJavaCtrl::checkController(ctrl))
        return;

    void * pEvent = (void *)get_vrml_external_field_EventIn_cEvent(env, self);
    jint type        = get_vrml_external_field_EventIn_type(env, self);
    bool b = (jVal ? true : false);
    ctrl->setBool(pEvent, b, type);
}

/*** protected native setImage (IIII[B)V ***/
extern JRI_PUBLIC_API(void)
native_vrml_external_field_EventIn_setImage(JRIEnv* env, struct vrml_external_field_EventIn* self, jint width, jint height, jint num, jint len, jbyteArray jArray)
{
    // check whether control is still active
    CJavaCtrl* ctrl = (CJavaCtrl*)get_vrml_external_field_EventIn_cBrowser(env, self);
    if (!CJavaCtrl::checkController(ctrl))
        return;

    void * pEvent = (void *)get_vrml_external_field_EventIn_cEvent(env, self);
    jint type        = get_vrml_external_field_EventIn_type(env, self);
    unsigned char *pixels    = (unsigned char *)JRI_GetByteArrayElements(env, jArray);
    ctrl->setImage(pEvent, width, height, num, len, pixels, type);
}

/*** private native nFinalize ()V ***/
extern JRI_PUBLIC_API(void)
native_vrml_external_field_EventIn_nFinalize(JRIEnv* env, struct vrml_external_field_EventIn* self)
{
    // check whether control is still active
    CJavaCtrl* ctrl = (CJavaCtrl*)get_vrml_external_field_EventIn_cBrowser(env, self);
    if (!CJavaCtrl::checkController(ctrl))
        return;

    void * pEvent = (void *)get_vrml_external_field_EventIn_cEvent(env, self);
	ctrl->release(pEvent);
}
