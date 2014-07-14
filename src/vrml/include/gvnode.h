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
#ifndef  _GV_NODE_
#define  _GV_NODE_

/******************************************************************************
@doc

@module GvNode.h - Declaration of VRML Root class GvNode|

Copyright (c) 1995 - 1999 by Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:

Changes:

$Revision: 1.17 $
$Log: gvnode.h,v $
Revision 1.17  1999/07/06 16:35:10  tom
*** empty log message ***



Todo :

******************************************************************************/


#include <gvstring.h>
#include <gvlists.h>


#include "rt.h"
#include "array.h"
#include "stack.h"
#include "point.h"
#include "matrix.h"
#include "gclass.h"


// forwards
class AStream;
class GvChildList;
class GvDict;
class GvFieldData;
class GvFieldEntry;
class GvField;

class GvInput;
class GvNodeList;
class GvState;

class GvConnection;

class GvScene;

class GvCopyContext;

// to use a global, static node dictonary
// dictonarys keept in namespaces & scene
//#define GV_GLOBAL_DICT


#ifdef _COM
// the COM EAI Interface declarations 
#include "glviewvrml.h"
#endif 



#include <gvdict.h>

//
// GvNode
// 

// derive GvNode from GClass node to get RT - Runtime Type information

/*! \class GvNode
 *  \brief ROOT class for all VRML 1.0 & 2.0 scenegraph nodes
 *   
 *  Supports basic mechanism for field information, parsing, routing.

Anatomy of a Node :
A node as a list of fields, a node can describe it self and the fields using a class: GvFieldInfo class.

Fields are numbered using a index.
There is the possibility to get back a pointer to a field given an index.



*/

class GvNode : 
	public GNode

#ifdef _COM
	//,public CComObjectRoot
	,public CComDualImpl<Node, &IID_Node, &LIBID_GLViewVrmlLib>
#endif

	{
public:

#ifdef _COM
	BEGIN_COM_MAP(GvNode)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(Node)
	END_COM_MAP()

	DECLARE_NOT_AGGREGATABLE(GvNode)

	ULONG InternalAddRef()
	{
		ASSERT(refCnt >= 0 && refCnt <= 64000);

		refCnt ++;
		return refCnt;
	}

	ULONG InternalRelease()
	{
		refCnt--;
		ASSERT(refCnt >= 0 && refCnt <= 64000);
		return refCnt;
	}

	STDMETHOD_(ULONG, AddRef)() {
		//_Module.Lock();	// lock the ATL module 
		
		// how to Lock.m_browser ?
		return InternalAddRef();
	}
	STDMETHOD_(ULONG, Release)()
	{
		ULONG l = InternalRelease(); 
		if (l == 0) delete this; 
		//_Module.Unlock(); // unlock the ATL module 
		return l;
	}

	STDMETHOD(QueryInterface)(REFIID iid, void ** ppvObject)
	{return _InternalQueryInterface(iid, ppvObject);}

	// needed static Methods from CComObjectRoot 
	static HRESULT WINAPI InternalQueryInterface(void* pThis,
		const _ATL_INTMAP_ENTRY* entries, REFIID iid, void** ppvObject) {
		return CComObjectRoot::InternalQueryInterface(pThis,entries,iid,ppvObject);
	}
	static HRESULT WINAPI _Chain(void* pv, REFIID iid, void** ppvObject, DWORD dw)
	{
		return CComObjectRoot::_Chain(pv,iid,ppvObject,dw);
	}


    /* Node methods */
    STDMETHOD(getType)(BSTR * type);

    STDMETHOD(getEventIn)(BSTR name, EventIn* *result);
    STDMETHOD(getEventOut)(BSTR name, EventOut* *result);
    STDMETHOD(getField)(BSTR name, Field ** result);
    STDMETHOD(getExposedField)(BSTR name, Field ** result);

	// needed for Java SI
	STDMETHOD(getBrowser)(Browser **result);

	// name extension
    STDMETHOD(getName) (BSTR *name);
    STDMETHOD(setName) (BSTR name);

	// interface extension 
	STDMETHOD(getNumInterfaceElements)(/* [retval][out] */ int *result);
        
    STDMETHOD(getInterfaceElement)(
			/* [in] */ int index,
            /* [out] */ int *elementType,
            /* [out] */ int *fieldType,
            /* [out] */ BSTR *name);
        
    STDMETHOD(getValueNode)( 
            /* [retval][out] */ Node *  *result);

	//! get back native pointer to node  (internal only) 
	STDMETHOD(getNative)(long *result) { *result = (long) (void *) this; return S_OK; }
 
	//! get current reference counter, for debugging only 
    STDMETHOD(getRefCnt)(long *result) { *result = refCnt; return S_OK; }


	//! get the IUnknown pointer of the node 
	IUnknown* getIUnknown() {
			IUnknown* result=NULL;
			this->QueryInterface(IID_IUnknown, (void **)&result);
			return result;
	}

	// ATL 2.0 
	void Lock() {} // other impl could lock module in memory
	void Unlock() {}

#endif // COM 


  public:
	RT(GGeometry);

	//! construction stage, the first new intialises the fieldInfo structure for builtin nodes
    enum Stage {
	   FIRST_INSTANCE,		//!< First real instance being constructed
	   PROTO_INSTANCE,		//!< Prototype instance being constructed
	   OTHER_INSTANCE,		//!< Subsequent instance being constructed
    };

/*
    enum Type {
       GEOMETRY_NODE=1,
       PROPERTY_NODE=2,
       TIME_DEPENDENT_NODE=4,
       VRML2_NODE=8,
       GLVIEW_NODE=16,
       IV_NODE=32,
       EXTENSION_NODE=64,
       SENSOR_NODE=128,
    };
*/

	GvNodeList owners;		//!< the list of owners of this node (not ref counted)
	GvNode*	   m_browser;	//!< the browser this node is contained in (not ref counted)

	//! get the browser this nodes belongs too 
    inline GvNode* getBrowser() const  { 
		// ASSERT(m_browser != NULL);
		return m_browser; 
	}
    
	//! set the browser this nodes belongs to
	//! set during node construction
	void setBrowser(GvNode * newBrowser) {
		m_browser = newBrowser;
		ASSERT(m_browser != NULL);
	}


    //! the node name
    GvName		*objName;
	
	//	flags;
	BEGIN_PACKED_STRUCT //    struct {
		unsigned int modified       : 1;  //!< node has been modified 
		unsigned int isBuiltIn		: 1;  //!< Node is builtIn (used to decide if EXTERNPROTO need to be written)
		unsigned int copyOnInstance : 1;  //!< Node need to be copied if proto is instanced (used only in PROTO's)		
		// unsigned int isVrml2		: 1;  //!< its a VRML 2 node 
		unsigned int needLightCheck	: 1;  //!< we need to check for lights in children
		unsigned int hasOutRoute	: 1;  //!< this node has out going routes
		unsigned int hasInRoute		: 1;  //!< this node has in routes from other nodes
		unsigned int hasBBox_		: 1;  //!< this node has non empty bbox
    END_PACKED_STRUCT // }; 


    //! Constructor
    GvNode();
    
    //! Destructor
    virtual ~GvNode();

    //! get node name
    const GvName &getName() const;
    
    //! set node name to name
    void setName(const GvName &name,GvBool vrml2=TRUE);

	//! get Type name for node (without Class prefix)
	virtual const char * GetType();
   
    //! get type class of node :: to do
    virtual int GetNodeClassType();

	//! reference counter, used for sharing nodes and memory management
	int refCnt;  
	
    //! increment reference count 
	void ref() { refCnt ++; }

    //! decrement reference counter, delete this if counter gets zero 
	void unref();
    
	//! decrement reference counter, without delete
    void unrefNoDelete() { refCnt--; }
	

    //! sets the modified flag, 
	//! notify owners by OnChildNodeChanged(this) if there are any owner connections for this node
	virtual void touch();

	//! clear the modified flag 
	void untouch() { modified = 0; }

	//! get the modified flag 
	int touched() const  { return(modified);}

	//! has this node any incomming or outgoing routes ? 
	GvBool hasRoutes() const { return hasOutRoute || hasInRoute; }
	//! has this node any outgoing routes ? 
	GvBool hasOutRoutes() const { return hasOutRoute; }
	//! has this node any incomming routes ? 
	GvBool hasInRoutes() const { return hasInRoute; }
	//! has this node a bounding box ? 
	inline GvBool hasBBox() const { return hasBBox_; }


    //! init global node dictonary
    static void	init();
    //! term global node dictonary
    static void	term();
    

    //! get field/event  description data for this node
    virtual GvFieldData *getFieldData() const = 0;

    //! return pointer to field with name "fieldName" into this node
    //! NULL if field not found
    virtual GvField* getField(const char *fieldName) const;

    //! get index of field in node, -1 if not found
    int  getFieldIndex(const GvField *field) const;

    //! get number of fields
    int	getNumFields() const;

    //! get name of field at index 
    const GvName &getFieldName(int index) const;

    //! get name of eventOut for field at index 
	GvName  getEventOutName(int index)  const; 

    //! get name of eventIn for field at index 
	GvName  getEventInName(int index)  const; 

    
    //! get field descriptor for field at index 
    GvFieldEntry* getFieldEntry(int index) const;

    //! get pointer to field at index 
    virtual GvField * getField(int index) const;


    // get scene node, this node belongs to
    ///*static*/ GvScene *getScene() const;

#ifdef WIN32_TLS

	#define TLS_BROWSER 1
	#define TLS_REDRAW 2
	#define TLS_TIME 3

	// use WIN32 Task Local variables to avoid global trashing from differrent instances

    // get (current) browser node, this node belongs to
	// type of node is actually GView 
    //static GvNode* getBrowser()  { return ((GvNode*) TlsGetValue(TLS_BROWSER)); }
    //static void setBrowser(GvNode * newBrowser) { TlsSetValue(TLS_BROWSER,(void *) newBrowser); }

	// Redraw flag 

	// set redraw flag, marks node for redraw e.g. on field changes
	void setRedraw() { TlsSetValue(TLS_REDRAW,(void *) 1); } 
	
	// clear redraw flag
	static void clearRedraw() { TlsSetValue(TLS_REDRAW, (void *) 0); } 
	
	// get redraw flag
	static BOOL getRedraw()   { return (((int)TlsGetValue(TLS_REDRAW)) !=0); } 
	
	// set pointer to threads time variable
	static void setTimePtr(double *timePtr) { TlsSetValue(TLS_TIME,(void *) timePtr); }

	// get the current time 
	static double getTime() { double *t = ((double *) TlsGetValue(TLS_TIME)); return *t; }; 

#else

/*
    // get (current) browser node, this node belongs to
	// type of node is actually GView 
    static GvNode* getBrowser()  { 
					 // ASSERT(browser!=NULL);
					return(browser); 
				}
    static void setBrowser(GvNode * newBrowser) { browser = newBrowser; }
	static GvNode* browser;
*/

#if 1
	// Redraw control, set flag on browser

	//! mark view for redraw
	void setRedraw();
	
	//! get redraw flag
	GvBool getRedraw();

	// design problem, functions implemented in GModel.cpp

	//! get current event simulation time 
	double getTime(); 
	double* getTimePtr();  //!< get pointer to variable where current event simulation time is stored

#else
	// Redraw flag  - static global version - bad for WIN32 OCX

	// global redraw flag
	static GvBool mustRedraw;

	// set redraw flag, marks node for redraw e.g. on field changes
	void setRedraw() { mustRedraw = TRUE; } 
	
	// clear redraw flag
	static void clearRedraw() { mustRedraw = FALSE; } 
	
	// get redraw flag
	static BOOL getRedraw() { return mustRedraw; } 

	static double *theTime;
	static void setTimePtr(double *timePtr) { theTime = timePtr; }

	static double getTime() 
	{ 
		// TBD ASSERT(theTime!=NULL);
		if (theTime == NULL) return GTime::GetGlobalTime();
		return *theTime; 
	}; 
#endif

#endif



    //! computes copyOnInstance flag from copyOnInstance flag or any child flag set
	//! needed for Proto instantiation
    virtual GvBool computeCopyOnInstance(GvCopyContext &context);
   
	//! get the copyOnInstance flag 
    inline GvBool getCopyOnInstance() const {  return(copyOnInstance); }

	//! set the copyOnInstance flag 
    void setCopyOnInstance(GvBool flag) { copyOnInstance = flag; }

	//! get the getNeedLightCheck flag 
    inline GvBool getNeedLightCheck() const {  return(needLightCheck); }
	//! set the getNeedLightCheck flag 
    inline void setNeedLightCheck(GvBool flag) { needLightCheck = flag; }


	//! get/compute bounding box of node , FALSE bbox is empty or node contains no geometry
	virtual GvBool getBBox(BBox &bbox); // in GvTraverse.cpp

	//! return bbox also for nodes having geometric extent (Lights, ProximitySensors etc)
	virtual GvBool getBBoxEx(BBox &bbox);


   	//! get matrix of node , FALSE if node has no matrix 
	virtual GvBool getMatrix(Matrix &m); 

    //return true && fieldName for field "field" in this node
    // or false if field not found

    //GvBool getFieldName(const GvField *field,GvName &fieldName);


 //protected:
  public: 

  // private:

#ifdef GV_GLOBAL_DICT
    // global nameDict
    static GvDict	*nameDict;
#endif 



	// add node by name to global nameDict
    static void		addName(GvNode *, const char *);

  	// remove node by name from global nameDict
    static void		removeName(GvNode *, const char *);
	
	// lookup for node by name in global nameDict
    static GvNode * findName(const char *name);


    // read support

    //! read an instance of this node (after it has been constructed)
    virtual GvBool	readInstance(GvInput *in);

    //! read a node
    static GvBool	read(GvInput *in, GvNode *&node);
   
    // read a node pointer (name)
    static GvNode *	readReference(GvInput *in);

    //! Read any ROUTE, PROTO, EXTERNPROTO until a unknown name 
    static GvBool readRoutesOrProtos(GvInput *in);

    //! Read a single  ROUTE, PROTO, EXTERNPROTO 
    static GvBool readRouteOrProto(GvInput *in,const GvName &name);

    //! is name a ROUTE, PROTO, EXTERNPROTO keyword ?
    static GvBool isRouteOrProto(const GvName &name);

    //! read a ROUTE 
    static GvBool readRoute(GvInput *in);

	//! read node, given classname already
    static GvBool	readNode(GvInput *in, GvName &className,GvNode *&node);

	//! read node instance, given classname already
    static GvBool	readNodeInstance(GvInput *in, const GvName &className,
					      const GvName &refName, GvNode *&node);

	//! create instance of node given by name in Context in
    static GvNode *	createInstance(GvInput *in, const GvName &className);
    static void	flushInput(GvInput *in);
	
	//! try to create a ProtoInstance node of type classname given context in in
	static GvNode *createProtoInstance(GvInput *in, const GvName &className);

	//! create instance of node - global nodes onlz
    static GvNode *	createInstanceFromName(const GvName &className,int vrml2);


	// ROUTE´s 

    //! add a route via connection construction
    static GvBool addRoute(GvNode *from,int fromEventIndex,GvNode * to,int toEventIndex); 
    //! deletea route via connection construction
    static GvBool deleteRoute(GvNode *from,int fromEventIndex,GvNode * to,int toEventIndex); 

	//! add a ROUTE to current pasing PROTO context or SCENE
    static GvBool addRoute(GvInput *in,
                        GvNode *from, const GvName& fromEvent,
                        GvNode *to, const GvName& toEvent);

	//! send event fromField to Node to : event toIndex
	static GvBool sendEventIn(GvField *fromField, GvNode * to,int toIndex);

    


   public:

    //! apply modifiers and call state.Do(this)
    virtual int Traverse(GTraversal &state);
	
	//! traverse all node fields in this node
    int TraverseAllNodeFields(GTraversal &state);
	//! traverse all node fields in this node with PATH construction
    int TraverseAllNodeFields(GPathTraversal &state);

	//! flush all node fields 
    void FlushAllNodeFields();
      

    virtual int Do(GDisplayTreeTraversal &state); // implemented in GDisplayTree.cpp
	
	virtual int Do(GOptimizeTraversal &state);	// implemented in GOptimizeTraversal.cpp


    // for GioTraversal only 
	int Do(GioTraversal &state);
	
	int DoNodeDef(GioTraversal &state,GvBool writeProto=TRUE);	// write out DEF name  or USE refeence if any

	int DoNodeClass(GioTraversal &state);	// write classname 
	
	int DoNodeFields(GioTraversal &state);	// write fields including fields [ ]
	
	int DoNodeBegin(GioTraversal &state);	// start { and indent
	int DoNodeEnd(GioTraversal &state);	// start } and un-indent
	// end gio traversal 
	
	virtual GvBool fieldsAreEqual(const GvNode *n) const;

	//! create a new instance of this node
	virtual GvNode * newInstance();
	
    //! copy all field values  tbd.
	virtual GvBool copyFieldValues(const GvNode *n);
	
	//! copy node recursively
	virtual GvNode * copy(GvNode * resultNode = NULL,GvBool deep=FALSE);

	//virtual void setToDefaults();
	//virtual void hasDefaultValues();

	//! check nodes for it default values, mode = 1 allow clearance of fieldValues
	virtual int checkForDefaultValues(int mode);


    //!eval node to the actual value node of node, overriden by ProtoInstance nodes to return top-level node
    virtual GvNode* Eval(GTraversal &state); // side effect, may load EXTERNPROTO's and instance a ProtoIntace
    //!eval node to the actual value node of node, overriden by ProtoInstance nodes to return top-level node
    virtual GvNode* Eval();	// 


	//! a field has been changed - called by subfields on touch
	virtual int OnFieldChanged(GvField *field);

	//! something in this node changed 
	virtual int OnChanged();

	//! Evaluate object as object of type "type"
	//! if return = this, don´t destroy result, otherwise destroy
	virtual GvNode *EvalAs(TypeId type,int copyAttributes); 

    
    // connection handling

    friend class GvConnection;
    
    //! find input event
    virtual GvField * getEventIn(const char *event);

    //! find output event
    virtual GvField * getEventOut(const char *event);


	// owner connection
	//! add a connection from this node to owner
	void addOwner(GvNode *owner);
	//! remove connection from this node to owner
	void removeOwner(GvNode *owner);

	//! remove all owners
	void removeOwners();

	//! replace a child node by new instance
	GvBool replaceChildNode(GvNode *oldNode,GvNode *newNode);
	GvBool replaceMe(GvNode *newNode);
	
	//! some child node has been changed
	virtual int OnChildChanged(GvNode *child);

	//! notify all registered owners, that this child has been changed
	virtual int NotifiyOwners();

    //! a list of connections implementing ROUTES, IS, Script Function call etc.
    GvConnection* connections;
    
    //! true if node has any outgoing connections
    GvBool hasConnections() const { return(connections != NULL); }

    //! return pointer to first connection
    GvConnection*  getConnections() { return(connections); }
    
    //! delete all connections
    int deleteConnections();
    
    //! add a connection, and optionally trigger the the connection 
    int addConnection(GvConnection *connection,GvBool trigger=FALSE);
    
    //! remove a connection
    int removeConnection(GvConnection *connection);

	//! remove a specific connection filtered by predicate function
	GvBool removeConnection(GvBool (*Pred)(GvConnection *c, void *data),void *data);

	//! find a specific connection filtered by predicate function
	GvConnection* findConnection(GvBool (*Pred)(GvConnection *c, void *data),void *data);


    //! removes connections to a node
    int removeConnections(GvNode *node);

    //! removes connections to a field
    int removeConnections(GvField *field);


};


/*! \class GvNodeHandle
 *  \brief  a handle & smartpointer to a GvNode
 *   
on a assignment node->ref is called
on destructin node->unref is called
handles allso assignment with NULL or same node
used also for GvSFNode + GvMFNode
*/

class GvNodeHandle
{
//protected :

public:
	//! pointer to the node
	GvNode *node;

public:

	//! get the node
	operator GvNode*() const { return node;}

	//! constructor given node handle
	GvNodeHandle(const GvNodeHandle &newNode) 
		{
		   node = newNode.node;
           if (node) node->ref();
		}

#ifdef _DEBUG_NODE_HANDLE
    
	//! init to NULL
	GvNodeHandle();

    //! init with node
	GvNodeHandle(GvNode * newNode); 

	//! unrefs node ,set node to NULL 
	void destruct();

	//! destructor, unrefs node 
	~GvNodeHandle();

	//! assginment
	GvNodeHandle& operator=(GvNode * newNode); 

#else

	//! init to NULL
	GvNodeHandle() { node = NULL; }

	//! unrefs node ,set node to NULL 
	inline void destruct() { if (node) node->unref(); node = NULL; }

	//! destructor, unrefs node 
	~GvNodeHandle() { if (node) node->unref(); /* node = NULL; */ }

    //! init with node
	inline GvNodeHandle(GvNode *newNode) 
		{
		   node = newNode;
           if (node) node->ref();
		}
    
	//! assginment
	inline GvNodeHandle& operator=(GvNode * newNode) 
		{
			if (node != newNode) {
				if (node) node->unref();			
				node = newNode;
				if (node) node->ref();			
			}

			return(*this);
		}
#endif

	//! set, including maintaining owner connections from newNode to ownerNode
	GvNodeHandle& set(GvNode *newNode,GvNode* ownerNode);



    GvNode* operator->() { return (node); }
    const GvNode* operator->() const { return (node); }

	GvNodeHandle& operator=(const GvNodeHandle &newNode) { return(*this = newNode.node); } 
  
    GvBool operator==(const GvNode * value) const { return(node == value);}
    
    //! get value node of node
    GvNode * Eval(GTraversal &state) { if (!node) return(NULL); return node->Eval(state); }
    //! get value node of node
    GvNode * Eval() { if (!node) return(NULL); return node->Eval(); }
};


// helper class for copying node trees 
// and avoiding recursive operations with self references scene graph
class GvCopyContext {

private :

    GvDict nodeMap;	//!< maps from old node pointers to new node pointers
	
	GvNode *browser; //!< the browser object for newly created nodes 

public :

    GvCopyContext(GvNode *theBrowser) : browser(theBrowser) {};
    ~GvCopyContext() {};

	//! is node already marked ?
	GvBool alreadySeen(GvNode * node);
	
	//! mark node seen
	void setSeen(GvNode * node);


    //! define  that oldnode has been  mapped to a newNode
    void put(GvNode * oldNode, GvNode *newNode);

    //! check if oldnode has already been mapped to a new
    //! NULL if not already mapped
    GvNode * get(GvNode *oldNode);

    //! get the browser context
    GvNode * getBrowser() { return browser; }


};




#endif /* _GV_NODE_ */

