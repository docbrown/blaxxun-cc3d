#ifndef __CC3DAPI_H
#define __CC3DAPI_H
//
// blaxxun CC3D EAI wrapper classes
// holger@blaxxun.com
// 03/15/98

// get the interface etc definition generated from blaxxunVRML.idl
// the IID ins blaxxunVRML_i.c must be defined once in the application
#include "blaxxunVRML.h"




// basic classes created by ATL proxy generator for blaxxunVRML.tlb
// added constructors and more inline functions for easier access to values



//////////////////////////////////////////////////////////////////////////////
// CNode


class CNode : public CComQIPtr<Node, &IID_Node>
{


public:
	
	// constructor, use parent forms 
	CNode(Node* lp=NULL) :CComQIPtr<Node, &IID_Node>(lp) { }
	CNode(const CComQIPtr<Node,&IID_Node>& lp): CComQIPtr<Node, &IID_Node>(lp) { }
	CNode(IUnknown* lp):CComQIPtr<Node, &IID_Node>(lp) { }

//Node : IDispatch

	HRESULT getType(
		BSTR * type)
	{
		return p->getType(type);
	}

	// example get Cstring 
	CString getType() 
	{
        BSTR v=NULL;
        
		HRESULT res= p->getType(&v);

        CString result(v);
        ::SysFreeString(v);// BSTR allocated by CC3D
		return result;
	}


	HRESULT getEventIn(
		BSTR name,
		EventIn * * result)
	{
		ASSERT(p);
		return p->getEventIn(name, result);
	}

	EventIn* getEventIn(LPCTSTR lpszName)
	{
		ASSERT(p);

		CComBSTR name(lpszName); // convert to BSTR 

		EventIn *result=NULL;
		if (p->getEventIn(name, &result) == S_OK) 	return result;

		TRACE("Node:getEventIn(%s) failed \n",lpszName);

		return NULL;
	}


	HRESULT getEventOut(
		BSTR name,
		EventOut * * result)
	{
		ASSERT(p);
		return p->getEventOut(name, result);
	}

	
	EventOut* getEventOut(LPCTSTR lpszName)
	{
		ASSERT(p);

		CComBSTR name(lpszName); // convert to BSTR 

		EventOut *result=NULL;
		if (p->getEventOut(name, &result) == S_OK) 	return result;
		
		TRACE("Node:getEventOut(%s) failed \n",lpszName);
		return NULL;
	}


	HRESULT getField(
		BSTR name,
		Field * * result)
	{
		return p->getField(name, result);
	}

	HRESULT getExposedField(
		BSTR name,
		Field * * result)
	{
		return p->getExposedField(name, result);
	}

	HRESULT getBrowser(
		Browser * * result)
	{
		return p->getBrowser(result);
	}
	
	HRESULT getName(
		BSTR * name)
	{
		return p->getName(name);
	}
	
	HRESULT setName(
		BSTR name)
	{
		return p->setName(name);
	}
	HRESULT getNumInterfaceElements(
		int * result)
	{
		return p->getNumInterfaceElements(result);
	}
	HRESULT getInterfaceElement(
		int index,
		int * elementType,
		int * fieldType,
		BSTR * name)
	{
		return p->getInterfaceElement(index, elementType, fieldType, name);
	}
	HRESULT getValueNode(
		Node * * result)
	{
		return p->getValueNode(result);
	}
	HRESULT getNative(
		long * result)
	{
		return p->getNative(result);
	}
	HRESULT getRefCnt(
		long * result)
	{
		return p->getRefCnt(result);
	}
};

// helper macros to savee typing 
// C++  constructors must be explicetly defined
#define CSTANDARD_METHODS(CLASSNAME,INTERFACE,IIDNAME) \
public: \
	CLASSNAME() : CComQIPtr<INTERFACE, &IIDNAME>() { } \
	CLASSNAME(INTERFACE* lp) : CComQIPtr<INTERFACE, &IIDNAME>(lp) { } \
	CLASSNAME(const CComQIPtr<INTERFACE,&IIDNAME>& lp): CComQIPtr<INTERFACE, &IIDNAME>(lp) { }\
	CLASSNAME(IUnknown* lp):CComQIPtr<INTERFACE, &IIDNAME>(lp) { } 


//////////////////////////////////////////////////////////////////////////////
// CEventIn
class CEventIn : public CComQIPtr<EventIn, &IID_EventIn>
{

public:
	// constructor, use parent forms 
	CEventIn(EventIn* lp=NULL) :CComQIPtr<EventIn, &IID_EventIn>(lp) { }
	CEventIn(const CComQIPtr<EventIn,&IID_EventIn>& lp): CComQIPtr<EventIn, &IID_EventIn>(lp) { }
	CEventIn(IUnknown* lp):CComQIPtr<EventIn, &IID_EventIn>(lp) { }

//EventIn : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT setValueFromString(
		BSTR value)
	{
		return p->setValueFromString(value);
	}
};


//////////////////////////////////////////////////////////////////////////////
// CEventOut
class CEventOut : public CComQIPtr<EventOut, &IID_EventOut>
{

	CSTANDARD_METHODS(CEventOut,EventOut,IID_EventOut)

//EventOut : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		ASSERT(p);
		return p->getType(type);
	}
	HRESULT advise(
		EventOutObserver * observer,
		IDispatch * userData)
	{
		ASSERT(p);
		return p->advise(observer, userData);
	}
	HRESULT unAdvise(
		EventOutObserver * observer)
	{
		ASSERT(p);
		return p->unAdvise(observer);
	}
	HRESULT toString(
		BSTR * value)
	{
		ASSERT(p);
		return p->toString(value);
	}
};


//////////////////////////////////////////////////////////////////////////////
// CEventOutObserver
#if 0

// object must be created by user 
class CEventOutObserver : public CComQIPtr<EventOutObserver, &IID_EventOutObserver>
{

//EventOutObserver : IDispatch
public:
	HRESULT callback(
		EventOut * value,
		double timeStamp,
		IDispatch * userData)
	{
		ASSERT(p);
		return p->callback(value, timeStamp, userData);
	}
};
#endif


//////////////////////////////////////////////////////////////////////////////
// CField
// can query for eventInXX eventOutXX
class CField : public CComQIPtr<Field, &IID_Field>
{
	CSTANDARD_METHODS(CField,Field,IID_Field)

//Field : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		ASSERT(p);
		return p->getType(type);
	}
};


//////////////////////////////////////////////////////////////////////////////
// CBrowser
// not used yet, user functions on cblaxxuncc3d
class CBrowser : public CComQIPtr<Browser, &IID_Browser>
{
	CSTANDARD_METHODS(CBrowser,Browser,IID_Browser)

//Browser : IDispatch
public:
	HRESULT getName(
		BSTR * value)
	{
		return p->getName(value);
	}
	HRESULT getVersion(
		BSTR * value)
	{
		return p->getVersion(value);
	}
	HRESULT getCurrentSpeed(
		float * value)
	{
		return p->getCurrentSpeed(value);
	}
	HRESULT getCurrentFrameRate(
		float * value)
	{
		return p->getCurrentFrameRate(value);
	}
	HRESULT getWorldURL(
		BSTR * value)
	{
		return p->getWorldURL(value);
	}
	HRESULT replaceWorld(
		Node * value)
	{
		return p->replaceWorld(value);
	}
	HRESULT loadURL(
		int cntUrl,
		BSTR * url,
		int cntParameter,
		BSTR * parameter)
	{
		return p->loadURL(cntUrl, url, cntParameter, parameter);
	}
	HRESULT setDescription(
		BSTR description)
	{
		return p->setDescription(description);
	}
	HRESULT createVrmlFromString(
		BSTR vrmlSyntax,
		Node * * value)
	{
		return p->createVrmlFromString(vrmlSyntax, value);
	}
	HRESULT createVrmlFromURL(
		BSTR url,
		Node * Node,
		BSTR event)
	{
		return p->createVrmlFromURL(url, Node, event);
	}
	HRESULT getNode(
		BSTR name,
		Node * * value)
	{
		return p->getNode(name, value);
	}
	HRESULT addRoute(
		Node * fromNode,
		BSTR fromEventOut,
		Node * toNode,
		BSTR toEventIn)
	{
		return p->addRoute(fromNode, fromEventOut, toNode, toEventIn);
	}
	HRESULT deleteRoute(
		Node * fromNode,
		BSTR fromEventOut,
		Node * toNode,
		BSTR toEventIn)
	{
		return p->deleteRoute(fromNode, fromEventOut, toNode, toEventIn);
	}
	HRESULT createNode(
		BSTR nodeclass,
		Node * * value)
	{
		return p->createNode(nodeclass, value);
	}
	HRESULT getWorld(
		Node * * value)
	{
		return p->getWorld(value);
	}
	HRESULT beginUpdate()
	{
		return p->beginUpdate();
	}
	HRESULT endUpdate()
	{
		return p->endUpdate();
	}
	HRESULT addNode(
		Node * Node)
	{
		return p->addNode(Node);
	}
	HRESULT removeNode(
		Node * Node,
		VARIANT_BOOL * value)
	{
		return p->removeNode(Node, value);
	}
	HRESULT setNodeName(
		Node * Node,
		BSTR name)
	{
		return p->setNodeName(Node, name);
	}
	HRESULT getNodeName(
		Node * Node,
		BSTR * name)
	{
		return p->getNodeName(Node, name);
	}
	HRESULT getEventOut(
		BSTR eventOutName,
		EventOut * * value)
	{
		return p->getEventOut(eventOutName, value);
	}
	HRESULT getEventIn(
		BSTR eventInName,
		EventIn * * value)
	{
		return p->getEventIn(eventInName, value);
	}
	HRESULT getControlPathname(
		BSTR * directoryName)
	{
		return p->getControlPathname(directoryName);
	}
	HRESULT saveWorld(
		BSTR fileName,
		VARIANT_BOOL * value)
	{
		return p->saveWorld(fileName, value);
	}
	HRESULT saveViewpoint(
		BSTR viewpointName)
	{
		return p->saveViewpoint(viewpointName);
	}
	HRESULT setObserver(
		IUnknown * observer,
		int flags,
		VARIANT_BOOL * value)
	{
		return p->setObserver(observer, flags, value);
	}
	HRESULT setViewpointByValue(
		float * position,
		float * orientation,
		float fov,
		VARIANT_BOOL animate)
	{
		return p->setViewpointByValue(position, orientation, fov, animate);
	}
	HRESULT setFullscreen(
		VARIANT_BOOL mode,
		VARIANT_BOOL * value)
	{
		return p->setFullscreen(mode, value);
	}
	HRESULT setCollisionDetection(
		VARIANT_BOOL mode)
	{
		return p->setCollisionDetection(mode);
	}
	HRESULT setRenderMode(
		BSTR mode)
	{
		return p->setRenderMode(mode);
	}
	HRESULT setNavigationMode(
		BSTR mode)
	{
		return p->setNavigationMode(mode);
	}
	HRESULT setViewpoint(
		BSTR viewpointName)
	{
		return p->setViewpoint(viewpointName);
	}
	HRESULT setHeadlight(
		VARIANT_BOOL mode)
	{
		return p->setHeadlight(mode);
	}
	HRESULT setAvatarHeight(
		float value)
	{
		return p->setAvatarHeight(value);
	}
	HRESULT setCollisionDistance(
		float value)
	{
		return p->setCollisionDistance(value);
	}
	HRESULT setVisibilityLimit(
		float value)
	{
		return p->setVisibilityLimit(value);
	}
	HRESULT setWalkSpeed(
		float value)
	{
		return p->setWalkSpeed(value);
	}
};


//////////////////////////////////////////////////////////////////////////////
// CEventInSFFloat
class CEventInSFFloat : public CComQIPtr<EventInSFFloat, &IID_EventInSFFloat>
{
	CSTANDARD_METHODS(CEventInSFFloat,EventInSFFloat,IID_EventInSFFloat)

//EventIn : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT setValueFromString(
		BSTR value)
	{
		return p->setValueFromString(value);
	}

//EventInSFFloat : EventIn
public:
	HRESULT setValue(
		float value)
	{
		return p->setValue(value);
	}
};


//////////////////////////////////////////////////////////////////////////////
// CEventInSFBool
class CEventInSFBool : public CComQIPtr<EventInSFBool, &IID_EventInSFBool>
{
	CSTANDARD_METHODS(CEventInSFBool,EventInSFBool,IID_EventInSFBool)

//EventIn : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT setValueFromString(
		BSTR value)
	{
		return p->setValueFromString(value);
	}

//EventInSFBool : EventIn
public:
	HRESULT setValue(
		VARIANT_BOOL value)
	{
		return p->setValue(value);
	}
};


//////////////////////////////////////////////////////////////////////////////
// CEventOutMField
class CEventOutMField : public CComQIPtr<EventOutMField, &IID_EventOutMField>
{
	CSTANDARD_METHODS(CEventOutMField,EventOutMField,IID_EventOutMField)

//EventOut : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT advise(
		EventOutObserver * observer,
		IDispatch * userData)
	{
		return p->advise(observer, userData);
	}
	HRESULT unAdvise(
		EventOutObserver * observer)
	{
		return p->unAdvise(observer);
	}
	HRESULT toString(
		BSTR * value)
	{
		return p->toString(value);
	}

//EventOutMField : EventOut
public:
	HRESULT getSize(
		int * cnt)
	{
		return p->getSize(cnt);
	}
};


//////////////////////////////////////////////////////////////////////////////
// CEventInMFColor
class CEventInMFColor : public CComQIPtr<EventInMFColor, &IID_EventInMFColor>
{
	CSTANDARD_METHODS(CEventInMFColor,EventInMFColor,IID_EventInMFColor)

//EventIn : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT setValueFromString(
		BSTR value)
	{
		return p->setValueFromString(value);
	}

//EventInMFColor : EventIn
public:
	HRESULT setValue(
		int cnt,
		float * value)
	{
		return p->setValue(cnt, value);
	}
	HRESULT set1Value(
		int index,
		float * value)
	{
		return p->set1Value(index, value);
	}

	// extra	
	HRESULT set1Value(int index,float r,float g,float b)
	{
		float v[3];
		
		ASSERT(p);

		v[0]=r,v[1]=g,v[2]=b;
		return p->set1Value(index, v);

	}

};


//////////////////////////////////////////////////////////////////////////////
// CEventInMFFloat
class CEventInMFFloat : public CComQIPtr<EventInMFFloat, &IID_EventInMFFloat>
{
	CSTANDARD_METHODS(CEventInMFFloat,EventInMFFloat,IID_EventInMFFloat)

//EventIn : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT setValueFromString(
		BSTR value)
	{
		return p->setValueFromString(value);
	}

//EventInMFFloat : EventIn
public:
	HRESULT setValue(
		int cnt,
		float * value)
	{
		return p->setValue(cnt, value);
	}
	HRESULT set1Value(
		int index,
		float value)
	{
		return p->set1Value(index, value);
	}
};


//////////////////////////////////////////////////////////////////////////////
// CEventInMFInt32
class CEventInMFInt32 : public CComQIPtr<EventInMFInt32, &IID_EventInMFInt32>
{
	CSTANDARD_METHODS(CEventInMFInt32,EventInMFInt32,IID_EventInMFInt32)

//EventIn : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT setValueFromString(
		BSTR value)
	{
		return p->setValueFromString(value);
	}

//EventInMFInt32 : EventIn
public:
	HRESULT setValue(
		int cnt,
		int * value)
	{
		return p->setValue(cnt, value);
	}
	HRESULT set1Value(
		int index,
		int value)
	{
		return p->set1Value(index, value);
	}
};


//////////////////////////////////////////////////////////////////////////////
// CEventInMFNode
class CEventInMFNode : public CComQIPtr<EventInMFNode, &IID_EventInMFNode>
{
	CSTANDARD_METHODS(CEventInMFNode,EventInMFNode,IID_EventInMFNode)

//EventIn : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT setValueFromString(
		BSTR value)
	{
		return p->setValueFromString(value);
	}

//EventInMFNode : EventIn
public:
	HRESULT setValue(
		int cnt,
		IDispatch * * value)
	{
		return p->setValue(cnt, value);
	}
	HRESULT set1Value(
		int index,
		Node * value)
	{
		return p->set1Value(index, value);
	}
};


//////////////////////////////////////////////////////////////////////////////
// CEventInMFRotation
class CEventInMFRotation : public CComQIPtr<EventInMFRotation, &IID_EventInMFRotation>
{
	CSTANDARD_METHODS(CEventInMFRotation,EventInMFRotation,IID_EventInMFRotation)

//EventIn : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT setValueFromString(
		BSTR value)
	{
		return p->setValueFromString(value);
	}

//EventInMFRotation : EventIn
public:
	HRESULT setValue(
		int cnt,
		float * value)
	{
		return p->setValue(cnt, value);
	}
	HRESULT set1Value(
		int index,
		float * value)
	{
		return p->set1Value(index, value);
	}

	// extra	
	HRESULT set1Value(int index,float x,float y,float z,float a)
	{
		float v[4];
		
		ASSERT(p);

		v[0]=x,v[1]=y,v[2]=z,v[3]=a;
		return p->set1Value(index,v);
	}

};


//////////////////////////////////////////////////////////////////////////////
// CEventInMFString
class CEventInMFString : public CComQIPtr<EventInMFString, &IID_EventInMFString>
{
	CSTANDARD_METHODS(CEventInMFString,EventInMFString,IID_EventInMFString)

//EventIn : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT setValueFromString(
		BSTR value)
	{
		return p->setValueFromString(value);
	}

//EventInMFString : EventIn
public:
	HRESULT setValue(
		int cnt,
		BSTR * value)
	{
		return p->setValue(cnt, value);
	}
	HRESULT set1Value(
		int index,
		BSTR value)
	{
		return p->set1Value(index, value);
	}
};


//////////////////////////////////////////////////////////////////////////////
// CEventInMFVec2f
class CEventInMFVec2f : public CComQIPtr<EventInMFVec2f, &IID_EventInMFVec2f>
{
	CSTANDARD_METHODS(CEventInMFVec2f,EventInMFVec2f,IID_EventInMFVec2f)

//EventIn : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT setValueFromString(
		BSTR value)
	{
		return p->setValueFromString(value);
	}

//EventInMFVec2f : EventIn
public:
	HRESULT setValue(
		int cnt,
		float * value)
	{
		return p->setValue(cnt, value);
	}
	HRESULT set1Value(
		int index,
		float * value)
	{
		return p->set1Value(index, value);
	}
};


//////////////////////////////////////////////////////////////////////////////
// CEventInMFVec3f
class CEventInMFVec3f : public CComQIPtr<EventInMFVec3f, &IID_EventInMFVec3f>
{
	CSTANDARD_METHODS(CEventInMFVec3f,EventInMFVec3f,IID_EventInMFVec3f)

//EventIn : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT setValueFromString(
		BSTR value)
	{
		return p->setValueFromString(value);
	}

//EventInMFVec3f : EventIn
public:
	HRESULT setValue(
		int cnt,
		float * value)
	{
		return p->setValue(cnt, value);
	}
	HRESULT set1Value(
		int index,
		float * value)
	{
		return p->set1Value(index, value);
	}

	// extra	
	HRESULT set1Value(int index,float x,float y,float z)
	{
		float v[3];
		
		ASSERT(p);

		v[0]=x,v[1]=y,v[2]=z;
		return p->set1Value(index, v);
	}

};


//////////////////////////////////////////////////////////////////////////////
// CEventInSFColor
class CEventInSFColor : public CComQIPtr<EventInSFColor, &IID_EventInSFColor>
{
	CSTANDARD_METHODS(CEventInSFColor,EventInSFColor,IID_EventInSFColor)

//EventIn : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT setValueFromString(
		BSTR value)
	{
		return p->setValueFromString(value);
	}

//EventInSFColor : EventIn
public:
	HRESULT setValue(
		float * value)
	{
		return p->setValue(value);
	}

	// extra	
	HRESULT setValue(float r,float g,float b)
	{
		float v[3];
		
		ASSERT(p);

		v[0]=r,v[1]=g,v[2]=b;
		return p->setValue(v);
	}
};


//////////////////////////////////////////////////////////////////////////////
// CEventInSFImage
class CEventInSFImage : public CComQIPtr<EventInSFImage, &IID_EventInSFImage>
{
	CSTANDARD_METHODS(CEventInSFImage,EventInSFImage,IID_EventInSFImage)

//EventIn : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT setValueFromString(
		BSTR value)
	{
		return p->setValueFromString(value);
	}

//EventInSFImage : EventIn
public:
	HRESULT setValue(
		int width,
		int height,
		int numComponents,
		int cnt,
		unsigned char * pixels)
	{
		return p->setValue(width, height, numComponents, cnt, pixels);
	}
};


//////////////////////////////////////////////////////////////////////////////
// CEventInSFInt32
class CEventInSFInt32 : public CComQIPtr<EventInSFInt32, &IID_EventInSFInt32>
{
	CSTANDARD_METHODS(CEventInSFInt32,EventInSFInt32,IID_EventInSFInt32)

//EventIn : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT setValueFromString(
		BSTR value)
	{
		return p->setValueFromString(value);
	}

//EventInSFInt32 : EventIn
public:
	HRESULT setValue(
		int value)
	{
		return p->setValue(value);
	}
};


//////////////////////////////////////////////////////////////////////////////
// CEventInSFNode
class CEventInSFNode : public CComQIPtr<EventInSFNode, &IID_EventInSFNode>
{
	CSTANDARD_METHODS(CEventInSFNode,EventInSFNode,IID_EventInSFNode)

//EventIn : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT setValueFromString(
		BSTR value)
	{
		return p->setValueFromString(value);
	}

//EventInSFNode : EventIn
public:
	HRESULT setValue(
		Node * value)
	{
		return p->setValue(value);
	}
};


//////////////////////////////////////////////////////////////////////////////
// CEventInSFRotation
class CEventInSFRotation : public CComQIPtr<EventInSFRotation, &IID_EventInSFRotation>
{
	CSTANDARD_METHODS(CEventInSFRotation,EventInSFRotation,IID_EventInSFRotation)

//EventIn : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT setValueFromString(
		BSTR value)
	{
		return p->setValueFromString(value);
	}

//EventInSFRotation : EventIn
public:
	HRESULT setValue(
		float * value)
	{
		return p->setValue(value);
	}

	// extra	
	HRESULT setValue(float x,float y,float z,float a)
	{
		float v[4];
		
		ASSERT(p);

		v[0]=x,v[1]=y,v[2]=z,v[3]=a;
		return p->setValue(v);
	}

};


//////////////////////////////////////////////////////////////////////////////
// CEventInSFString
class CEventInSFString : public CComQIPtr<EventInSFString, &IID_EventInSFString>
{
	CSTANDARD_METHODS(CEventInSFString,EventInSFString,IID_EventInSFString)

//EventIn : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT setValueFromString(
		BSTR value)
	{
		return p->setValueFromString(value);
	}

//EventInSFString : EventIn
public:
	HRESULT setValue(
		BSTR value)
	{
		return p->setValue(value);
	}
};


//////////////////////////////////////////////////////////////////////////////
// CEventInSFTime
class CEventInSFTime : public CComQIPtr<EventInSFTime, &IID_EventInSFTime>
{
	CSTANDARD_METHODS(CEventInSFTime,EventInSFTime,IID_EventInSFTime)

//EventIn : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT setValueFromString(
		BSTR value)
	{
		return p->setValueFromString(value);
	}

//EventInSFTime : EventIn
public:
	HRESULT setValue(
		double value)
	{
		return p->setValue(value);
	}
};


//////////////////////////////////////////////////////////////////////////////
// CEventInSFVec2f
class CEventInSFVec2f : public CComQIPtr<EventInSFVec2f, &IID_EventInSFVec2f>
{
	CSTANDARD_METHODS(CEventInSFVec2f,EventInSFVec2f,IID_EventInSFVec2f)

//EventIn : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT setValueFromString(
		BSTR value)
	{
		return p->setValueFromString(value);
	}

//EventInSFVec2f : EventIn
public:
	HRESULT setValue(
		float * value)
	{
		return p->setValue(value);
	}
};


//////////////////////////////////////////////////////////////////////////////
// CEventInSFVec3f
class CEventInSFVec3f : public CComQIPtr<EventInSFVec3f, &IID_EventInSFVec3f>
{
	CSTANDARD_METHODS(CEventInSFVec3f,EventInSFVec3f,IID_EventInSFVec3f)

//EventIn : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT setValueFromString(
		BSTR value)
	{
		return p->setValueFromString(value);
	}

//EventInSFVec3f : EventIn
public:
	HRESULT setValue(
		float * value)
	{
		ASSERT(p);
		return p->setValue(value);
	}

// extra	
	HRESULT setValue(float x,float y,float z)
	{
		float v[3];
		
		ASSERT(p);

		v[0]=x,v[1]=y,v[2]=z;
		return p->setValue(v);
	}

};


//////////////////////////////////////////////////////////////////////////////
// CEventOutMFColor
class CEventOutMFColor : public CComQIPtr<EventOutMFColor, &IID_EventOutMFColor>
{
	CSTANDARD_METHODS(CEventOutMFColor,EventOutMFColor,IID_EventOutMFColor)

//EventOut : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT advise(
		EventOutObserver * observer,
		IDispatch * userData)
	{
		return p->advise(observer, userData);
	}
	HRESULT unAdvise(
		EventOutObserver * observer)
	{
		return p->unAdvise(observer);
	}
	HRESULT toString(
		BSTR * value)
	{
		return p->toString(value);
	}

//EventOutMField : EventOut
public:
	HRESULT getSize(
		int * cnt)
	{
		return p->getSize(cnt);
	}

//EventOutMFColor : EventOutMField
public:
	HRESULT getValue(
		int cnt,
		float * value)
	{
		return p->getValue(cnt, value);
	}
	HRESULT get1Value(
		int index,
		float * value)
	{
		return p->get1Value(index, value);
	}
};


//////////////////////////////////////////////////////////////////////////////
// CEventOutMFFloat
class CEventOutMFFloat : public CComQIPtr<EventOutMFFloat, &IID_EventOutMFFloat>
{
	CSTANDARD_METHODS(CEventOutMFFloat,EventOutMFFloat,IID_EventOutMFFloat)

//EventOut : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT advise(
		EventOutObserver * observer,
		IDispatch * userData)
	{
		return p->advise(observer, userData);
	}
	HRESULT unAdvise(
		EventOutObserver * observer)
	{
		return p->unAdvise(observer);
	}
	HRESULT toString(
		BSTR * value)
	{
		return p->toString(value);
	}

//EventOutMField : EventOut
public:
	HRESULT getSize(
		int * cnt)
	{
		return p->getSize(cnt);
	}

//EventOutMFFloat : EventOutMField
public:
	HRESULT getValue(
		int cnt,
		float * value)
	{
		return p->getValue(cnt, value);
	}
	HRESULT get1Value(
		int index,
		float * value)
	{
		return p->get1Value(index, value);
	}
};


//////////////////////////////////////////////////////////////////////////////
// CEventOutMFInt32
class CEventOutMFInt32 : public CComQIPtr<EventOutMFInt32, &IID_EventOutMFInt32>
{
	CSTANDARD_METHODS(CEventOutMFInt32,EventOutMFInt32,IID_EventOutMFInt32)

//EventOut : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT advise(
		EventOutObserver * observer,
		IDispatch * userData)
	{
		return p->advise(observer, userData);
	}
	HRESULT unAdvise(
		EventOutObserver * observer)
	{
		return p->unAdvise(observer);
	}
	HRESULT toString(
		BSTR * value)
	{
		return p->toString(value);
	}

//EventOutMField : EventOut
public:
	HRESULT getSize(
		int * cnt)
	{
		return p->getSize(cnt);
	}

//EventOutMFInt32 : EventOutMField
public:
	HRESULT getValue(
		int cnt,
		int * value)
	{
		return p->getValue(cnt, value);
	}
	HRESULT get1Value(
		int index,
		int * value)
	{
		return p->get1Value(index, value);
	}
};


//////////////////////////////////////////////////////////////////////////////
// CEventOutMFNode
class CEventOutMFNode : public CComQIPtr<EventOutMFNode, &IID_EventOutMFNode>
{
	CSTANDARD_METHODS(CEventOutMFNode,EventOutMFNode,IID_EventOutMFNode)

//EventOut : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT advise(
		EventOutObserver * observer,
		IDispatch * userData)
	{
		return p->advise(observer, userData);
	}
	HRESULT unAdvise(
		EventOutObserver * observer)
	{
		return p->unAdvise(observer);
	}
	HRESULT toString(
		BSTR * value)
	{
		return p->toString(value);
	}

//EventOutMField : EventOut
public:
	HRESULT getSize(
		int * cnt)
	{
		return p->getSize(cnt);
	}

//EventOutMFNode : EventOutMField
public:
	HRESULT getValue(
		int cnt,
		IDispatch * * value)
	{
		return p->getValue(cnt, value);
	}
	HRESULT get1Value(
		int index,
		Node * * value)
	{
		return p->get1Value(index, value);
	}
};


//////////////////////////////////////////////////////////////////////////////
// CEventOutMFRotation
class CEventOutMFRotation : public CComQIPtr<EventOutMFRotation, &IID_EventOutMFRotation>
{
	CSTANDARD_METHODS(CEventOutMFRotation,EventOutMFRotation,IID_EventOutMFRotation)

//EventOut : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT advise(
		EventOutObserver * observer,
		IDispatch * userData)
	{
		return p->advise(observer, userData);
	}
	HRESULT unAdvise(
		EventOutObserver * observer)
	{
		return p->unAdvise(observer);
	}
	HRESULT toString(
		BSTR * value)
	{
		return p->toString(value);
	}

//EventOutMField : EventOut
public:
	HRESULT getSize(
		int * cnt)
	{
		return p->getSize(cnt);
	}

//EventOutMFRotation : EventOutMField
public:
	HRESULT getValue(
		int cnt,
		float * value)
	{
		return p->getValue(cnt, value);
	}
	HRESULT get1Value(
		int index,
		float * value)
	{
		return p->get1Value(index, value);
	}
};


//////////////////////////////////////////////////////////////////////////////
// CEventOutMFString
class CEventOutMFString : public CComQIPtr<EventOutMFString, &IID_EventOutMFString>
{
	CSTANDARD_METHODS(CEventOutMFString,EventOutMFString,IID_EventOutMFString)

//EventOut : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT advise(
		EventOutObserver * observer,
		IDispatch * userData)
	{
		return p->advise(observer, userData);
	}
	HRESULT unAdvise(
		EventOutObserver * observer)
	{
		return p->unAdvise(observer);
	}
	HRESULT toString(
		BSTR * value)
	{
		return p->toString(value);
	}

//EventOutMField : EventOut
public:
	HRESULT getSize(
		int * cnt)
	{
		return p->getSize(cnt);
	}

//EventOutMFString : EventOutMField
public:
	HRESULT getValue(
		int cnt,
		BSTR * value)
	{
		return p->getValue(cnt, value);
	}
	HRESULT get1Value(
		int index,
		BSTR * value)
	{
		return p->get1Value(index, value);
	}
};


//////////////////////////////////////////////////////////////////////////////
// CEventOutMFVec2f
class CEventOutMFVec2f : public CComQIPtr<EventOutMFVec2f, &IID_EventOutMFVec2f>
{
	CSTANDARD_METHODS(CEventOutMFVec2f,EventOutMFVec2f,IID_EventOutMFVec2f)

//EventOut : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT advise(
		EventOutObserver * observer,
		IDispatch * userData)
	{
		return p->advise(observer, userData);
	}
	HRESULT unAdvise(
		EventOutObserver * observer)
	{
		return p->unAdvise(observer);
	}
	HRESULT toString(
		BSTR * value)
	{
		return p->toString(value);
	}

//EventOutMField : EventOut
public:
	HRESULT getSize(
		int * cnt)
	{
		return p->getSize(cnt);
	}

//EventOutMFVec2f : EventOutMField
public:
	HRESULT getValue(
		int cnt,
		float * value)
	{
		return p->getValue(cnt, value);
	}
	HRESULT get1Value(
		int index,
		float * value)
	{
		return p->get1Value(index, value);
	}
};


//////////////////////////////////////////////////////////////////////////////
// CEventOutMFVec3f
class CEventOutMFVec3f : public CComQIPtr<EventOutMFVec3f, &IID_EventOutMFVec3f>
{
	CSTANDARD_METHODS(CEventOutMFVec3f,EventOutMFVec3f,IID_EventOutMFVec3f)

//EventOut : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT advise(
		EventOutObserver * observer,
		IDispatch * userData)
	{
		return p->advise(observer, userData);
	}
	HRESULT unAdvise(
		EventOutObserver * observer)
	{
		return p->unAdvise(observer);
	}
	HRESULT toString(
		BSTR * value)
	{
		return p->toString(value);
	}

//EventOutMField : EventOut
public:
	HRESULT getSize(
		int * cnt)
	{
		return p->getSize(cnt);
	}

//EventOutMFVec3f : EventOutMField
public:
	HRESULT getValue(
		int cnt,
		float * value)
	{
		return p->getValue(cnt, value);
	}
	HRESULT get1Value(
		int index,
		float * value)
	{
		return p->get1Value(index, value);
	}
};


//////////////////////////////////////////////////////////////////////////////
// CEventOutSFBool
class CEventOutSFBool : public CComQIPtr<EventOutSFBool, &IID_EventOutSFBool>
{
	CSTANDARD_METHODS(CEventOutSFBool,EventOutSFBool,IID_EventOutSFBool)

//EventOut : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT advise(
		EventOutObserver * observer,
		IDispatch * userData)
	{
		return p->advise(observer, userData);
	}
	HRESULT unAdvise(
		EventOutObserver * observer)
	{
		return p->unAdvise(observer);
	}
	HRESULT toString(
		BSTR * value)
	{
		return p->toString(value);
	}

//EventOutSFBool : EventOut
public:
	HRESULT getValue(
		VARIANT_BOOL * value)
	{
		return p->getValue(value);
	}

	bool getValue()
	{
        VARIANT_BOOL v;
		if ((p->getValue(&v) == S_OK) && v) return true;
		else return false;
	}

};


//////////////////////////////////////////////////////////////////////////////
// CEventOutSFColor
class CEventOutSFColor : public CComQIPtr<EventOutSFColor, &IID_EventOutSFColor>
{
	CSTANDARD_METHODS(CEventOutSFColor,EventOutSFColor,IID_EventOutSFColor)

//EventOut : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT advise(
		EventOutObserver * observer,
		IDispatch * userData)
	{
		return p->advise(observer, userData);
	}
	HRESULT unAdvise(
		EventOutObserver * observer)
	{
		return p->unAdvise(observer);
	}
	HRESULT toString(
		BSTR * value)
	{
		return p->toString(value);
	}

//EventOutSFColor : EventOut
public:
	HRESULT getValue(
		float * value)
	{
		return p->getValue(value);
	}
};


//////////////////////////////////////////////////////////////////////////////
// CEventOutSFFloat
class CEventOutSFFloat : public CComQIPtr<EventOutSFFloat, &IID_EventOutSFFloat>
{
	CSTANDARD_METHODS(CEventOutSFFloat,EventOutSFFloat,IID_EventOutSFFloat)

//EventOut : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT advise(
		EventOutObserver * observer,
		IDispatch * userData)
	{
		return p->advise(observer, userData);
	}
	HRESULT unAdvise(
		EventOutObserver * observer)
	{
		return p->unAdvise(observer);
	}
	HRESULT toString(
		BSTR * value)
	{
		return p->toString(value);
	}

//EventOutSFFloat : EventOut
public:
	HRESULT getValue(
		float * value)
	{
		return p->getValue(value);
	}

// extra 
	float getValue() {
		float v=0.0;
		HRESULT res = getValue(&v);
		ASSERT(res == S_OK);
		return v;

	}
};


//////////////////////////////////////////////////////////////////////////////
// CEventOutSFImage
class CEventOutSFImage : public CComQIPtr<EventOutSFImage, &IID_EventOutSFImage>
{
	CSTANDARD_METHODS(CEventOutSFImage,EventOutSFImage,IID_EventOutSFImage)

//EventOut : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT advise(
		EventOutObserver * observer,
		IDispatch * userData)
	{
		return p->advise(observer, userData);
	}
	HRESULT unAdvise(
		EventOutObserver * observer)
	{
		return p->unAdvise(observer);
	}
	HRESULT toString(
		BSTR * value)
	{
		return p->toString(value);
	}

//EventOutSFImage : EventOut
public:
	HRESULT getWidth(
		int * value)
	{
		return p->getWidth(value);
	}
	HRESULT getHeight(
		int * value)
	{
		return p->getHeight(value);
	}
	HRESULT getNumComponents(
		int * value)
	{
		return p->getNumComponents(value);
	}
	HRESULT getPixels(
		int cnt,
		unsigned char * value)
	{
		return p->getPixels(cnt, value);
	}
};


//////////////////////////////////////////////////////////////////////////////
// CEventOutSFInt32
class CEventOutSFInt32 : public CComQIPtr<EventOutSFInt32, &IID_EventOutSFInt32>
{
	CSTANDARD_METHODS(CEventOutSFInt32,EventOutSFInt32,IID_EventOutSFInt32)

//EventOut : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT advise(
		EventOutObserver * observer,
		IDispatch * userData)
	{
		return p->advise(observer, userData);
	}
	HRESULT unAdvise(
		EventOutObserver * observer)
	{
		return p->unAdvise(observer);
	}
	HRESULT toString(
		BSTR * value)
	{
		return p->toString(value);
	}

//EventOutSFInt32 : EventOut
public:
	HRESULT getValue(
		int * value)
	{
		return p->getValue(value);
	}
};


//////////////////////////////////////////////////////////////////////////////
// CEventOutSFNode
class CEventOutSFNode : public CComQIPtr<EventOutSFNode, &IID_EventOutSFNode>
{

//EventOut : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT advise(
		EventOutObserver * observer,
		IDispatch * userData)
	{
		return p->advise(observer, userData);
	}
	HRESULT unAdvise(
		EventOutObserver * observer)
	{
		return p->unAdvise(observer);
	}
	HRESULT toString(
		BSTR * value)
	{
		return p->toString(value);
	}

//EventOutSFNode : EventOut
public:
	HRESULT getValue(
		Node * * value)
	{
		return p->getValue(value);
	}
};


//////////////////////////////////////////////////////////////////////////////
// CEventOutSFRotation
class CEventOutSFRotation : public CComQIPtr<EventOutSFRotation, &IID_EventOutSFRotation>
{
	CSTANDARD_METHODS(CEventOutSFRotation,EventOutSFRotation,IID_EventOutSFRotation)

//EventOut : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT advise(
		EventOutObserver * observer,
		IDispatch * userData)
	{
		return p->advise(observer, userData);
	}
	HRESULT unAdvise(
		EventOutObserver * observer)
	{
		return p->unAdvise(observer);
	}
	HRESULT toString(
		BSTR * value)
	{
		return p->toString(value);
	}

//EventOutSFRotation : EventOut
public:
	HRESULT getValue(
		float * value)
	{
		return p->getValue(value);
	}
};


//////////////////////////////////////////////////////////////////////////////
// CEventOutSFString
class CEventOutSFString : public CComQIPtr<EventOutSFString, &IID_EventOutSFString>
{
	CSTANDARD_METHODS(CEventOutSFString,EventOutSFString,IID_EventOutSFString)

//EventOut : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT advise(
		EventOutObserver * observer,
		IDispatch * userData)
	{
		return p->advise(observer, userData);
	}
	HRESULT unAdvise(
		EventOutObserver * observer)
	{
		return p->unAdvise(observer);
	}
	HRESULT toString(
		BSTR * value)
	{
		return p->toString(value);
	}

//EventOutSFString : EventOut
public:
	HRESULT getValue(
		BSTR * value)
	{
		return p->getValue(value);
	}
};


//////////////////////////////////////////////////////////////////////////////
// CEventOutSFTime
class CEventOutSFTime : public CComQIPtr<EventOutSFTime, &IID_EventOutSFTime>
{
	CSTANDARD_METHODS(CEventOutSFTime,EventOutSFTime,IID_EventOutSFTime)

//EventOut : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT advise(
		EventOutObserver * observer,
		IDispatch * userData)
	{
		return p->advise(observer, userData);
	}
	HRESULT unAdvise(
		EventOutObserver * observer)
	{
		return p->unAdvise(observer);
	}
	HRESULT toString(
		BSTR * value)
	{
		return p->toString(value);
	}

//EventOutSFTime : EventOut
public:
	HRESULT getValue(
		double * value)
	{
		return p->getValue(value);
	}
};


//////////////////////////////////////////////////////////////////////////////
// CEventOutSFVec2f
class CEventOutSFVec2f : public CComQIPtr<EventOutSFVec2f, &IID_EventOutSFVec2f>
{
	CSTANDARD_METHODS(CEventOutSFVec2f,EventOutSFVec2f,IID_EventOutSFVec2f)

//EventOut : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT advise(
		EventOutObserver * observer,
		IDispatch * userData)
	{
		return p->advise(observer, userData);
	}
	HRESULT unAdvise(
		EventOutObserver * observer)
	{
		return p->unAdvise(observer);
	}
	HRESULT toString(
		BSTR * value)
	{
		return p->toString(value);
	}

//EventOutSFVec2f : EventOut
public:
	HRESULT getValue(
		float * value)
	{
		return p->getValue(value);
	}
};


//////////////////////////////////////////////////////////////////////////////
// CEventOutSFVec3f
class CEventOutSFVec3f : public CComQIPtr<EventOutSFVec3f, &IID_EventOutSFVec3f>
{
	CSTANDARD_METHODS(CEventOutSFVec3f,EventOutSFVec3f,IID_EventOutSFVec3f)

//EventOut : IDispatch
public:
	HRESULT getType(
		int * type)
	{
		return p->getType(type);
	}
	HRESULT advise(
		EventOutObserver * observer,
		IDispatch * userData)
	{
		return p->advise(observer, userData);
	}
	HRESULT unAdvise(
		EventOutObserver * observer)
	{
		return p->unAdvise(observer);
	}
	HRESULT toString(
		BSTR * value)
	{
		return p->toString(value);
	}

//EventOutSFVec3f : EventOut
public:
	HRESULT getValue(
		float * value)
	{
		return p->getValue(value);
	}
};


//////////////////////////////////////////////////////////////////////////////
// CScriptContainer
class CScriptContainer : public CComQIPtr<ScriptContainer, &IID_ScriptContainer>
{
	CSTANDARD_METHODS(CScriptContainer,ScriptContainer,IID_ScriptContainer)

//ScriptContainer : IDispatch
public:
};


//////////////////////////////////////////////////////////////////////////////
// CScript
class CScript : public CComQIPtr<Script, &IID_Script>
{
	CSTANDARD_METHODS(CScript,Script,IID_Script)

//Script : IDispatch
public:
	HRESULT setContainer(
		Node * container)
	{
		return p->setContainer(container);
	}
	HRESULT loadScriptObject(
		BSTR urlData)
	{
		return p->loadScriptObject(urlData);
	}
	HRESULT initialize()
	{
		return p->initialize();
	}
	HRESULT shutdown()
	{
		return p->shutdown();
	}
	HRESULT processEvent(
		BSTR name,
		int eventId,
		EventOut * value,
		double timeStamp)
	{
		return p->processEvent(name, eventId, value, timeStamp);
	}
	HRESULT eventsProcessed()
	{
		return p->eventsProcessed();
	}
};


//////////////////////////////////////////////////////////////////////////////
// CBrowserObserver
// interface must be implemented by user object
class CBrowserObserver : public CComQIPtr<BrowserObserver, &IID_BrowserObserver>
{

//BrowserObserver : IDispatch
public:
	HRESULT OnStatusMessage(
		BSTR message,
		int messageType)
	{
		return p->OnStatusMessage(message, messageType);
	}
	HRESULT OnUrlError(
		BSTR url,
		BSTR message,
		int errorCode)
	{
		return p->OnUrlError(url, message, errorCode);
	}
	HRESULT OnLoadUrl(
		BSTR url,
		BSTR targetFrame)
	{
		return p->OnLoadUrl(url, targetFrame);
	}
	HRESULT OnLoadAnchor(
		BSTR description,
		int numUrl,
		BSTR * url,
		int numParameter,
		BSTR * parameter)
	{
		return p->OnLoadAnchor(description, numUrl, url, numParameter, parameter);
	}
	HRESULT OnViewpointChanged(
		float * position,
		float * orientation)
	{
		return p->OnViewpointChanged(position, orientation);
	}
	HRESULT OnSceneChanged(
		BSTR url)
	{
		return p->OnSceneChanged(url);
	}
	HRESULT LoadUrlToFile(
		BSTR url)
	{
		return p->LoadUrlToFile(url);
	}
};


#endif

