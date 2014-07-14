/* DO NOT EDIT THIS FILE - it is machine generated */
#include "jri.h"

/* Header for class vrml/external/Node */

#ifndef _vrml_external_Node_H_
#define _vrml_external_Node_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct java_lang_String;
struct vrml_external_field_EventIn;
struct vrml_external_field_EventOut;
struct java_lang_Class;

/*******************************************************************************
 * Class vrml/external/Node
 ******************************************************************************/

typedef struct vrml_external_Node vrml_external_Node;

#define classname_vrml_external_Node	"vrml/external/Node"

#define class_vrml_external_Node(env) \
	((struct java_lang_Class*)JRI_FindClass(env, classname_vrml_external_Node))

/*******************************************************************************
 * Public Methods
 ******************************************************************************/

#ifdef DEBUG

/*** public native getType ()Ljava/lang/String; ***/
extern JRI_PUBLIC_API(struct java_lang_String *)
vrml_external_Node_getType(JRIEnv* env, struct vrml_external_Node* self);

/*** public native getEventIn (Ljava/lang/String;)Lvrml/external/field/EventIn; ***/
extern JRI_PUBLIC_API(struct vrml_external_field_EventIn *)
vrml_external_Node_getEventIn(JRIEnv* env, struct vrml_external_Node* self, struct java_lang_String *a);

/*** public native getEventOut (Ljava/lang/String;)Lvrml/external/field/EventOut; ***/
extern JRI_PUBLIC_API(struct vrml_external_field_EventOut *)
vrml_external_Node_getEventOut(JRIEnv* env, struct vrml_external_Node* self, struct java_lang_String *a);

/*** public finalize ()V ***/
extern JRI_PUBLIC_API(void)
vrml_external_Node_finalize(JRIEnv* env, struct vrml_external_Node* self);

#else /* !DEBUG */

/*** public native getType ()Ljava/lang/String; ***/
#define vrml_external_Node_getType(env, obj) \
	((struct java_lang_String *)JRI_CallMethod(env)(env, JRI_CallMethod_op, obj, methodID_vrml_external_Node_getType))

/*** public native getEventIn (Ljava/lang/String;)Lvrml/external/field/EventIn; ***/
#define vrml_external_Node_getEventIn(env, obj, a) \
	((struct vrml_external_field_EventIn *)JRI_CallMethod(env)(env, JRI_CallMethod_op, obj, methodID_vrml_external_Node_getEventIn, a))

/*** public native getEventOut (Ljava/lang/String;)Lvrml/external/field/EventOut; ***/
#define vrml_external_Node_getEventOut(env, obj, a) \
	((struct vrml_external_field_EventOut *)JRI_CallMethod(env)(env, JRI_CallMethod_op, obj, methodID_vrml_external_Node_getEventOut, a))

/*** public finalize ()V ***/
#define vrml_external_Node_finalize(env, obj) \
	((void)JRI_CallMethod(env)(env, JRI_CallMethod_op, obj, methodID_vrml_external_Node_finalize))

#endif /* !DEBUG*/

/*** public native getType ()Ljava/lang/String; ***/
extern JRIMethodID FAR methodID_vrml_external_Node_getType;
#define name_vrml_external_Node_getType	"getType"
#define sig_vrml_external_Node_getType 	"()Ljava/lang/String;"
#define use_vrml_external_Node_getType(env, clazz)	\
	(methodID_vrml_external_Node_getType =	\
		JRI_GetMethodID(env, clazz,	\
			name_vrml_external_Node_getType,	\
			sig_vrml_external_Node_getType))
#define unuse_vrml_external_Node_getType(env, clazz)	\
	(methodID_vrml_external_Node_getType = JRIUninitialized)

/*** public native getEventIn (Ljava/lang/String;)Lvrml/external/field/EventIn; ***/
extern JRIMethodID FAR methodID_vrml_external_Node_getEventIn;
#define name_vrml_external_Node_getEventIn	"getEventIn"
#define sig_vrml_external_Node_getEventIn 	"(Ljava/lang/String;)Lvrml/external/field/EventIn;"
#define use_vrml_external_Node_getEventIn(env, clazz)	\
	(methodID_vrml_external_Node_getEventIn =	\
		JRI_GetMethodID(env, clazz,	\
			name_vrml_external_Node_getEventIn,	\
			sig_vrml_external_Node_getEventIn))
#define unuse_vrml_external_Node_getEventIn(env, clazz)	\
	(methodID_vrml_external_Node_getEventIn = JRIUninitialized)

/*** public native getEventOut (Ljava/lang/String;)Lvrml/external/field/EventOut; ***/
extern JRIMethodID FAR methodID_vrml_external_Node_getEventOut;
#define name_vrml_external_Node_getEventOut	"getEventOut"
#define sig_vrml_external_Node_getEventOut 	"(Ljava/lang/String;)Lvrml/external/field/EventOut;"
#define use_vrml_external_Node_getEventOut(env, clazz)	\
	(methodID_vrml_external_Node_getEventOut =	\
		JRI_GetMethodID(env, clazz,	\
			name_vrml_external_Node_getEventOut,	\
			sig_vrml_external_Node_getEventOut))
#define unuse_vrml_external_Node_getEventOut(env, clazz)	\
	(methodID_vrml_external_Node_getEventOut = JRIUninitialized)

/*** public finalize ()V ***/
extern JRIMethodID FAR methodID_vrml_external_Node_finalize;
#define name_vrml_external_Node_finalize	"finalize"
#define sig_vrml_external_Node_finalize 	"()V"
#define use_vrml_external_Node_finalize(env, clazz)	\
	(methodID_vrml_external_Node_finalize =	\
		JRI_GetMethodID(env, clazz,	\
			name_vrml_external_Node_finalize,	\
			sig_vrml_external_Node_finalize))
#define unuse_vrml_external_Node_finalize(env, clazz)	\
	(methodID_vrml_external_Node_finalize = JRIUninitialized)

/*******************************************************************************
 * IMPLEMENTATION SECTION: 
 * Define the IMPLEMENT_vrml_external_Node symbol 
 * if you intend to implement the native methods of this class. This 
 * symbol makes the private and protected methods available. You should 
 * also call the register_vrml_external_Node routine 
 * to make your native methods available.
 ******************************************************************************/

extern JRI_PUBLIC_API(struct java_lang_Class*)
use_vrml_external_Node(JRIEnv* env);

extern JRI_PUBLIC_API(void)
unuse_vrml_external_Node(JRIEnv* env);

extern JRI_PUBLIC_API(struct java_lang_Class*)
register_vrml_external_Node(JRIEnv* env);

extern JRI_PUBLIC_API(void)
unregister_vrml_external_Node(JRIEnv* env);

#ifdef IMPLEMENT_vrml_external_Node

/*******************************************************************************
 * Native Methods: 
 * These are the signatures of the native methods which you must implement.
 ******************************************************************************/

/*** public native getType ()Ljava/lang/String; ***/
extern JRI_PUBLIC_API(struct java_lang_String *)
native_vrml_external_Node_getType(JRIEnv* env, struct vrml_external_Node* self);

/*** public native getEventIn (Ljava/lang/String;)Lvrml/external/field/EventIn; ***/
extern JRI_PUBLIC_API(struct vrml_external_field_EventIn *)
native_vrml_external_Node_getEventIn(JRIEnv* env, struct vrml_external_Node* self, struct java_lang_String *a);

/*** public native getEventOut (Ljava/lang/String;)Lvrml/external/field/EventOut; ***/
extern JRI_PUBLIC_API(struct vrml_external_field_EventOut *)
native_vrml_external_Node_getEventOut(JRIEnv* env, struct vrml_external_Node* self, struct java_lang_String *a);

/*** private native nFinalize ()V ***/
extern JRI_PUBLIC_API(void)
native_vrml_external_Node_nFinalize(JRIEnv* env, struct vrml_external_Node* self);

/*******************************************************************************
 * Implementation Field Accessors: 
 * You should only use these from within the native method definitions.
 ******************************************************************************/

#ifdef DEBUG

/*** private cBrowser I ***/
extern JRI_PUBLIC_API(jint)
get_vrml_external_Node_cBrowser(JRIEnv* env, vrml_external_Node* obj);
extern JRI_PUBLIC_API(void)
set_vrml_external_Node_cBrowser(JRIEnv* env, vrml_external_Node* obj, jint v);

/*** private cNode I ***/
extern JRI_PUBLIC_API(jint)
get_vrml_external_Node_cNode(JRIEnv* env, vrml_external_Node* obj);
extern JRI_PUBLIC_API(void)
set_vrml_external_Node_cNode(JRIEnv* env, vrml_external_Node* obj, jint v);

#else /* !DEBUG */

/*** private cBrowser I ***/
#define get_vrml_external_Node_cBrowser(env, obj) \
	(JRI_GetFieldInt(env, obj, fieldID_vrml_external_Node_cBrowser))
#define set_vrml_external_Node_cBrowser(env, obj, v) \
	JRI_SetFieldInt(env, obj, fieldID_vrml_external_Node_cBrowser, v)

/*** private cNode I ***/
#define get_vrml_external_Node_cNode(env, obj) \
	(JRI_GetFieldInt(env, obj, fieldID_vrml_external_Node_cNode))
#define set_vrml_external_Node_cNode(env, obj, v) \
	JRI_SetFieldInt(env, obj, fieldID_vrml_external_Node_cNode, v)

#endif /* !DEBUG*/

extern JRIFieldID FAR fieldID_vrml_external_Node_cBrowser;
#define name_vrml_external_Node_cBrowser	"cBrowser"
#define sig_vrml_external_Node_cBrowser 	"I"
#define use_vrml_external_Node_cBrowser(env, clazz)	\
	(fieldID_vrml_external_Node_cBrowser =	\
		JRI_GetFieldID(env, clazz,	\
			name_vrml_external_Node_cBrowser,	\
			sig_vrml_external_Node_cBrowser))
#define unuse_vrml_external_Node_cBrowser(env, clazz)	\
		(fieldID_vrml_external_Node_cBrowser = JRIUninitialized)

extern JRIFieldID FAR fieldID_vrml_external_Node_cNode;
#define name_vrml_external_Node_cNode	"cNode"
#define sig_vrml_external_Node_cNode 	"I"
#define use_vrml_external_Node_cNode(env, clazz)	\
	(fieldID_vrml_external_Node_cNode =	\
		JRI_GetFieldID(env, clazz,	\
			name_vrml_external_Node_cNode,	\
			sig_vrml_external_Node_cNode))
#define unuse_vrml_external_Node_cNode(env, clazz)	\
		(fieldID_vrml_external_Node_cNode = JRIUninitialized)

/*******************************************************************************
 * Implementation Methods: 
 * You should only use these from within the native method definitions.
 ******************************************************************************/

#ifdef DEBUG

/*** protected <init> (II)V ***/
extern JRI_PUBLIC_API(struct vrml_external_Node*)
vrml_external_Node_new(JRIEnv* env, struct java_lang_Class* clazz, jint a, jint b);

/*** private native nFinalize ()V ***/
extern JRI_PUBLIC_API(void)
vrml_external_Node_nFinalize(JRIEnv* env, struct vrml_external_Node* self);

#else /* !DEBUG */

/*** protected <init> (II)V ***/
#define vrml_external_Node_new(env, clazz, a, b)	\
	((struct vrml_external_Node*)JRI_NewObject(env)(env, JRI_NewObject_op, clazz, methodID_vrml_external_Node_new, a, b))

/*** private native nFinalize ()V ***/
#define vrml_external_Node_nFinalize(env, obj) \
	((void)JRI_CallMethod(env)(env, JRI_CallMethod_op, obj, methodID_vrml_external_Node_nFinalize))

#endif /* !DEBUG*/

/*** protected <init> (II)V ***/
extern JRIMethodID FAR methodID_vrml_external_Node_new;
#define name_vrml_external_Node_new	"<init>"
#define sig_vrml_external_Node_new 	"(II)V"
#define use_vrml_external_Node_new(env, clazz)	\
	(methodID_vrml_external_Node_new =	\
		JRI_GetMethodID(env, clazz,	\
			name_vrml_external_Node_new,	\
			sig_vrml_external_Node_new))
#define unuse_vrml_external_Node_new(env, clazz)	\
	(methodID_vrml_external_Node_new = JRIUninitialized)

/*** private native nFinalize ()V ***/
extern JRIMethodID FAR methodID_vrml_external_Node_nFinalize;
#define name_vrml_external_Node_nFinalize	"nFinalize"
#define sig_vrml_external_Node_nFinalize 	"()V"
#define use_vrml_external_Node_nFinalize(env, clazz)	\
	(methodID_vrml_external_Node_nFinalize =	\
		JRI_GetMethodID(env, clazz,	\
			name_vrml_external_Node_nFinalize,	\
			sig_vrml_external_Node_nFinalize))
#define unuse_vrml_external_Node_nFinalize(env, clazz)	\
	(methodID_vrml_external_Node_nFinalize = JRIUninitialized)

#endif /* IMPLEMENT_vrml_external_Node */

#ifdef __cplusplus
} /* extern "C" */

/*******************************************************************************
 * C++ Definitions
 ******************************************************************************/

#include "java_lang_Object.h"

struct vrml_external_Node : virtual public java_lang_Object {

	static struct java_lang_Class* _use(JRIEnv* env) {
		return use_vrml_external_Node(env);
	}

	static void _unuse(JRIEnv* env) {
		unuse_vrml_external_Node(env);
	}

	static struct java_lang_Class* _register(JRIEnv* env) {
		return register_vrml_external_Node(env);
	}

	static void _unregister(JRIEnv* env) {
		unregister_vrml_external_Node(env);
	}

	static struct java_lang_Class* _class(JRIEnv* env) {
		return class_vrml_external_Node(env);
	}

	/* Public Methods */
	/*** public native getType ()Ljava/lang/String; ***/
	struct java_lang_String * getType(JRIEnv* env) {
		return vrml_external_Node_getType(env, this);
	}

	/*** public native getEventIn (Ljava/lang/String;)Lvrml/external/field/EventIn; ***/
	struct vrml_external_field_EventIn * getEventIn(JRIEnv* env, struct java_lang_String *a) {
		return vrml_external_Node_getEventIn(env, this, a);
	}

	/*** public native getEventOut (Ljava/lang/String;)Lvrml/external/field/EventOut; ***/
	struct vrml_external_field_EventOut * getEventOut(JRIEnv* env, struct java_lang_String *a) {
		return vrml_external_Node_getEventOut(env, this, a);
	}

	/*** public finalize ()V ***/
	void finalize(JRIEnv* env) {
		vrml_external_Node_finalize(env, this);
	}

#ifdef IMPLEMENT_vrml_external_Node

	/* Protected Methods */
	/*** protected <init> (II)V ***/
	static vrml_external_Node* _new(JRIEnv* env, struct java_lang_Class* clazz, jint a, jint b) {
		return vrml_external_Node_new(env, clazz, a, b);
	}

	/* Private Field Accessors */
	/*** private cBrowser I ***/
	jint cBrowser(JRIEnv* env) {
		return get_vrml_external_Node_cBrowser(env, this);
	}
	void cBrowser(JRIEnv* env, jint v) {
		set_vrml_external_Node_cBrowser(env, this, v);
	}

	/*** private cNode I ***/
	jint cNode(JRIEnv* env) {
		return get_vrml_external_Node_cNode(env, this);
	}
	void cNode(JRIEnv* env, jint v) {
		set_vrml_external_Node_cNode(env, this, v);
	}

	/* Private Methods */
	/*** private native nFinalize ()V ***/
	void nFinalize(JRIEnv* env) {
		vrml_external_Node_nFinalize(env, this);
	}

#endif /* IMPLEMENT_vrml_external_Node */
};

#endif /* __cplusplus */

#endif /* Class vrml/external/Node */
