/* DO NOT EDIT THIS FILE - it is machine generated */
#include "jri.h"

/* Header for class vrml/external/exception/InvalidVrmlException */

#ifndef _vrml_external_exception_InvalidVrmlException_H_
#define _vrml_external_exception_InvalidVrmlException_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct java_lang_String;
struct java_lang_Class;

/*******************************************************************************
 * Class vrml/external/exception/InvalidVrmlException
 ******************************************************************************/

typedef struct vrml_external_exception_InvalidVrmlException vrml_external_exception_InvalidVrmlException;

#define classname_vrml_external_exception_InvalidVrmlException	"vrml/external/exception/InvalidVrmlException"

#define class_vrml_external_exception_InvalidVrmlException(env) \
	((struct java_lang_Class*)JRI_FindClass(env, classname_vrml_external_exception_InvalidVrmlException))

/*******************************************************************************
 * Public Methods
 ******************************************************************************/

#ifdef DEBUG

/*** public <init> ()V ***/
extern JRI_PUBLIC_API(struct vrml_external_exception_InvalidVrmlException*)
vrml_external_exception_InvalidVrmlException_new(JRIEnv* env, struct java_lang_Class* clazz);

/*** public <init> (Ljava/lang/String;)V ***/
extern JRI_PUBLIC_API(struct vrml_external_exception_InvalidVrmlException*)
vrml_external_exception_InvalidVrmlException_new_1(JRIEnv* env, struct java_lang_Class* clazz, struct java_lang_String *a);

#else /* !DEBUG */

/*** public <init> ()V ***/
#define vrml_external_exception_InvalidVrmlException_new(env, clazz)	\
	((struct vrml_external_exception_InvalidVrmlException*)JRI_NewObject(env)(env, JRI_NewObject_op, clazz, methodID_vrml_external_exception_InvalidVrmlException_new))

/*** public <init> (Ljava/lang/String;)V ***/
#define vrml_external_exception_InvalidVrmlException_new_1(env, clazz, a)	\
	((struct vrml_external_exception_InvalidVrmlException*)JRI_NewObject(env)(env, JRI_NewObject_op, clazz, methodID_vrml_external_exception_InvalidVrmlException_new_1, a))

#endif /* !DEBUG*/

/*** public <init> ()V ***/
extern JRIMethodID FAR methodID_vrml_external_exception_InvalidVrmlException_new;
#define name_vrml_external_exception_InvalidVrmlException_new	"<init>"
#define sig_vrml_external_exception_InvalidVrmlException_new 	"()V"
#define use_vrml_external_exception_InvalidVrmlException_new(env, clazz)	\
	(methodID_vrml_external_exception_InvalidVrmlException_new =	\
		JRI_GetMethodID(env, clazz,	\
			name_vrml_external_exception_InvalidVrmlException_new,	\
			sig_vrml_external_exception_InvalidVrmlException_new))
#define unuse_vrml_external_exception_InvalidVrmlException_new(env, clazz)	\
	(methodID_vrml_external_exception_InvalidVrmlException_new = JRIUninitialized)

/*** public <init> (Ljava/lang/String;)V ***/
extern JRIMethodID FAR methodID_vrml_external_exception_InvalidVrmlException_new_1;
#define name_vrml_external_exception_InvalidVrmlException_new_1	"<init>"
#define sig_vrml_external_exception_InvalidVrmlException_new_1 	"(Ljava/lang/String;)V"
#define use_vrml_external_exception_InvalidVrmlException_new_1(env, clazz)	\
	(methodID_vrml_external_exception_InvalidVrmlException_new_1 =	\
		JRI_GetMethodID(env, clazz,	\
			name_vrml_external_exception_InvalidVrmlException_new_1,	\
			sig_vrml_external_exception_InvalidVrmlException_new_1))
#define unuse_vrml_external_exception_InvalidVrmlException_new_1(env, clazz)	\
	(methodID_vrml_external_exception_InvalidVrmlException_new_1 = JRIUninitialized)

/*******************************************************************************
 * IMPLEMENTATION SECTION: 
 * Define the IMPLEMENT_vrml_external_exception_InvalidVrmlException symbol 
 * if you intend to implement the native methods of this class. This 
 * symbol makes the private and protected methods available. You should 
 * also call the register_vrml_external_exception_InvalidVrmlException routine 
 * to make your native methods available.
 ******************************************************************************/

extern JRI_PUBLIC_API(struct java_lang_Class*)
use_vrml_external_exception_InvalidVrmlException(JRIEnv* env);

extern JRI_PUBLIC_API(void)
unuse_vrml_external_exception_InvalidVrmlException(JRIEnv* env);

extern JRI_PUBLIC_API(struct java_lang_Class*)
register_vrml_external_exception_InvalidVrmlException(JRIEnv* env);

extern JRI_PUBLIC_API(void)
unregister_vrml_external_exception_InvalidVrmlException(JRIEnv* env);

#ifdef IMPLEMENT_vrml_external_exception_InvalidVrmlException

#endif /* IMPLEMENT_vrml_external_exception_InvalidVrmlException */

#ifdef __cplusplus
} /* extern "C" */

/*******************************************************************************
 * C++ Definitions
 ******************************************************************************/

#include "java_lang_RuntimeException.h"

struct vrml_external_exception_InvalidVrmlException : virtual public java_lang_RuntimeException {

	static struct java_lang_Class* _use(JRIEnv* env) {
		return use_vrml_external_exception_InvalidVrmlException(env);
	}

	static void _unuse(JRIEnv* env) {
		unuse_vrml_external_exception_InvalidVrmlException(env);
	}

	static struct java_lang_Class* _register(JRIEnv* env) {
		return register_vrml_external_exception_InvalidVrmlException(env);
	}

	static void _unregister(JRIEnv* env) {
		unregister_vrml_external_exception_InvalidVrmlException(env);
	}

	static struct java_lang_Class* _class(JRIEnv* env) {
		return class_vrml_external_exception_InvalidVrmlException(env);
	}

	/* Public Methods */
	/*** public <init> ()V ***/
	static vrml_external_exception_InvalidVrmlException* _new(JRIEnv* env, struct java_lang_Class* clazz) {
		return vrml_external_exception_InvalidVrmlException_new(env, clazz);
	}

	/*** public <init> (Ljava/lang/String;)V ***/
	static vrml_external_exception_InvalidVrmlException* _new(JRIEnv* env, struct java_lang_Class* clazz, struct java_lang_String *a) {
		return vrml_external_exception_InvalidVrmlException_new_1(env, clazz, a);
	}

#ifdef IMPLEMENT_vrml_external_exception_InvalidVrmlException

#endif /* IMPLEMENT_vrml_external_exception_InvalidVrmlException */
};

#endif /* __cplusplus */

#endif /* Class vrml/external/exception/InvalidVrmlException */
