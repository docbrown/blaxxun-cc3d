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
#ifndef _GCLASS_H
#define _GCLASS_H

/******************************************************************************
@doc

@module GClass.h -  BasicNode, Node Traversal,  Traversal State classes|

Copyright (c) 1995 - 1998 by Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:

  The traversal code is based on an abstract GNode class, 
not requiring any VRML specific classes.
The Traversal objects are referring in the Do(Node) Method the virtual function invoked
on the nodes.

GvNode is the root class for VRML nodes

Changes:

$Revision: 1.18 $
$Log: gclass.h,v $
Revision 1.18  1998/08/20 17:21:54  holger
empty message



Todo :

******************************************************************************/


#define _GDEBUG

// non VRML scene tree stuff
#ifdef _GLVIEW_IE
	#undef _GFULL
#else
	#define _GFULL
#endif



class Point;

class GNode;

#ifdef _GFULL
class 	GAttributeNode;
class 		GGroup;
class 	GGeometry;
class		GInstance;

class 	GAttribute;
class		GStyle;
class 		GMatrix;
class		GBBox;
#endif

class   GStackAttribute;

class 	GTraversal;
class 		GGeometryTraversal;
class 			GPathTraversal;
class				GRaySelectTraversal;	// implemented in GRaySelect.cpp
class				GFindNodeTraversal;		// implemented in GFindNode.cpp
class			GBBoxTraversal;
class			GglRenderTraversal;
class				GglTransparencyRenderTraversal;
class       GD3DRenderTraversal;

class		GioTraversal;
class			GPovIoTraversal;

// some more
class		GBuildShellTraversal;	// implemented in GBuildShell.cpp
class		GOptimizeTraversal;	// implemented in GOptimizeTraversal.cpp
class		GVrml2ConvertTraversal;	// implemented in GvVrml2Convert.cpp

class		GLoadInlinesTraversal;	// implemented in qvTraverse.cpp

class		GDisplayTreeTraversal;	// implemented in GDisplayTree.cpp

//class		GStatisticsTraversal;


class	GProperty;
class		GFloatProperty;

#ifdef _GFULL
class GWorld;
#endif


class GCamera;
class GViewInfo;

// for one level qv attributes
class GvNode;
class GvField;

class GvShape;
class GvAppearance;
class Gv2Material;
class GvMaterial;
class GvTexture;
class Gv2TextureTransform;

class GvDict;
class GvSFVec3f;





#ifdef _GFULL

// Activity Bits, to tell system what to do on next update cycle
enum GActivity {
		GRedraw=1,			// Redraw this element
		GRedrawSome=2,		// There are childs, to redraw
		GEvaluate=4,		// Evaluate this element
		GEvaluateSome=8,	// There are childs to evaluate
		GMarked=16			// A Marker
		// GEvaluateBBox

	};
/*
  GRedraw at group means, redraw all childs
*/


// Action indicators, so that object can react
enum GAction {
//	GGeometryAdded,
//	GGeometryDeleted,
	GGeometryChanged,
	GGeometryMovedOut,
	GGeometryMovedIn,
	GGroupAdded,
	GGroupDeleted,
	GAttributeAdded,
	GAttributeDeleted,
	GAttributeChanged
	};

// object type id´s 
// to do: use RTTI
enum GId {
	GIdGroup=1,
	GIdInstance,
	GIdStyle,
	GIdMatrix,	/* Modelling Matrix Attribute */ 
	GIdBBox,	/* Boundingbox Attribute */

};
#endif

//
// GStackableAttribute
// traversal state attribute, to support push and pop of states

class GStackableAttribute : public RTRoot
{
public:
  	RT(GStackableAttribute);
	int level; // the tree-level this attribute was pushed
	
	GStackableAttribute *prev;  // prev element in States attribute stack
	GStackableAttribute *prevThisAttr;  // prev element in States attribute stack of same class
	
	static int attrIdCounter; // Global for assigning new attributed id to StackableAttributes
	
	// allocate a new attribute id
	static int GetNewAttrId() { return(attrIdCounter++); }

#ifdef WIN32

	static CRITICAL_SECTION section;  
	static void Lock() { EnterCriticalSection(&section); }
	static void Unlock() { LeaveCriticalSection(&section); }

	static void InitClass() { InitializeCriticalSection(&section); }

#else
    static void Lock() {}
    static void Unlock() {}

    static void InitClass() {}

#endif

	static void TermClass() { }

	// get attribute id number
	virtual int AttrId() = 0;
	
	// assign attribute id number (only one time initialization)
	virtual void SetAttrId(int newAttrId) = 0;

	// this attribute instance is made current == top level value
	virtual void Set(GTraversal & /* state*/ ) {};
	
	// allocate a new instance of this attribute
	virtual GStackableAttribute* NewInstance() = 0;

	// function to construct/reinit  element from freelist
	void Init() { }


	// free current instance
	virtual void  FreeInstance() = 0;

	friend class GTraversal;

};


/*
   each attribute Stack element has a unique identification
   id, maintained as a static element
   each element has a freelist, 
   CLASS::NewInstance allocates, FreeInstance "frees"
*/
#define GSTACKABLE_ATTRIBUTE(CLASS) \
		RT(CLASS);	\
		static int attrId;\
		static CLASS *freeList;	\
		virtual int AttrId(); \
		virtual void SetAttrId(int newAttrId);  \
		virtual GStackableAttribute* NewInstance(); \
		static CLASS* NewElement(); \
		virtual void FreeInstance(); \
		static void FreeClass();

// TBD: Rethink freelist could cause problems in MT environment,

#define GSTACKABLE_ATTRIBUTE_IMP(CLASS,ROOT) \
		RTIMP(CLASS,0,ROOT);	\
		int CLASS::attrId = 0; \
		CLASS * CLASS::freeList = NULL; \
		int CLASS::AttrId() { return(CLASS::attrId); } \
		void CLASS::SetAttrId(int newAttrId) { CLASS::attrId = newAttrId; } \
		GStackableAttribute* CLASS::NewInstance() {  \
				Lock();  \
				if (freeList) {CLASS *f= freeList; freeList = (CLASS *) f->prev; f->Init(); Unlock(); return(f); } \
				Unlock(); \
				return new CLASS ;  \
		}; \
		CLASS* CLASS::NewElement() {  \
				Lock();  \
				if (freeList) { CLASS *f= freeList; freeList = (CLASS *) f->prev; f->Init(); Unlock(); return(f); } \
				Unlock(); \
				return new CLASS ;  \
		}; \
		void CLASS::FreeInstance() { Lock(); this->prev = freeList; freeList = this; Unlock();}; \
		void CLASS::FreeClass() { Lock();   while (freeList) { CLASS * n= (CLASS *) freeList->prev; delete freeList; freeList=n; } Unlock(); };




#define GSTACKABLE_DERIVED_ATTRIBUTE(CLASS) \
		RT(CLASS);	\
		static CLASS *freeList;	\
		virtual GStackableAttribute* NewInstance(); \
		static CLASS* NewElement(); \
		virtual void FreeInstance(); \
		static void FreeClass();

#define GSTACKABLE_DERIVED_ATTRIBUTE_IMP(CLASS,ROOT) \
		RTIMP(CLASS,0,ROOT);	\
		CLASS * CLASS::freeList = NULL; \
		GStackableAttribute* CLASS::NewInstance() {  \
				Lock();  \
				if (freeList) {CLASS *f= freeList; freeList = (CLASS *) f->prev; f->Init(); Unlock(); return(f); } \
				Unlock(); \
				return new CLASS ;  \
		}; \
		CLASS* CLASS::NewElement() {  \
				Lock();  \
				if (freeList) {  CLASS *f=freeList; freeList = (CLASS *) f->prev; f->Init(); Unlock(); return(f); } \
				Unlock(); \
				return new CLASS;  \
		}; \
		void CLASS::FreeInstance() { Lock(); this->prev = freeList; freeList = this; Unlock(); }; \
		void CLASS::FreeClass() { Lock(); while (freeList) { CLASS * n= (CLASS *) freeList->prev; delete freeList; freeList=n; } Unlock(); };




class GView;
class CMyglView;

//
// GPathElement
// a path store a path through a scene tree to a specific node
//
class GPathElement {
public:
	GNode *node;		// node
	GPathElement *next;	 // linked list, for easy push

	GvField *field;		// field of node 
	int index;			// optional index in field
	void *data;			// optional  user data 

	GPathElement(GNode *Node,GPathElement *Next, GvField *Field=NULL,int Index=-1,void *Data=NULL) :
		node(Node), next(Next), field(Field),index(Index),data(Data)
		{ }

	// is this node in path ?
	// gbool IsInPath(GvNode *n);
};

//
//
//


class GPath : public Stack<GPathElement>
{
public :
//	GPath(int initSíze=30) : Stack<GPathElement>(initSize) { };
//	~GPath() {} ;

};


// special result codes from Traverse()
#define GV_NODE_ALREADY_SEEN 2
#define GV_NODE_CULLED -1

//
// GTraversal
// A Traversal applies action to whole tree
// root for all specialised traversal actions
//

class GTraversal : public RTRoot {

    // state for all attributes
public:


	RT(GTraversal);

	// Top attribute pointer, index by AttrId
	Array<GStackableAttribute*> current;

	// the attribute stack, elements linked together by prev field to form stack
	GStackableAttribute *top;

	GvAppearance *defaultAppearance; // appearance if appearance field is NULL
	Gv2Material *defaultMaterial; // material if apperance material is NULL	(part of defaultAppearance)
	Gv2TextureTransform *defaultTextureTransform; // textureTransform if apperance textureTransform is NULL	(part of defaultAppearance)

	GvMaterial *defaultMaterial10; // default / top-level VRML 1.0 material 

	gbool globalPointLights; // treat Point & Spotlight global, has been set up at higher level
	gbool needShapes;		// do we need the expanded sehll geometry 

	gbool appearanceApplied; // 1 : apperance has been applied to rendering pipeline
	gbool initScripts;		// init any scripts not yet initialized	

	//static, one level VRML 2.0 attributes
	GvAppearance *appearance; // current appearance

	GvShape *shape; // current leaf shape
	
	Gv2Material *material; // current applied appearance material

	int materialStatus;	// MATERIAL_STATUS_FLAGS 
	GvTexture *texture; // current applied appearance texture

	int textureStatus;	// TEXTURE_STATUS_FLAGS 
	Gv2TextureTransform *textureTransform; // current applied appearance textureTransform

	// ShapeHints / culling params, needed for GRaySelectTraversal
	gbool solid;		// true: do culling, false no culling 
	gbool cw;		// true: faces are cw, false faces are ccw	
	
	// set static culling attributes 
	void SetCulling(gbool Solid, gbool Cw) { solid = Solid; cw = Cw; }

	
	GvNode *owner; // misc

	GvDict *nodeDict;  // helper dict to mark nodes etc 

	void ClearStaticAttributes() {

		appearance =  NULL; // (GvAppearance *) -1; // really set to invalid value 
		appearanceApplied = gfalse;
		initScripts = gtrue;
		shape = (GvShape*) -1;
		material = (Gv2Material*) -1;
		materialStatus = 0;
		texture = (GvTexture*) -1;
		textureTransform = (Gv2TextureTransform*) -1;
		textureStatus = 0;
		owner = NULL;
		solid=gtrue; 
		cw=gfalse;

	}

	int level;			// current tree traversal level
	int maxLevel;		// max traveral depth 

	int inlineLevel;	 // current loading level
	int maxInlineLevel;  // max depth to load
	int inlineWwwFlags;	 // flags for WWW Operation 
	int stop;			// abort traveral


	gbool traverseAll;	// uncoditionally traverse ALL nodes
	gbool traverseTransformHierarchyOnly;	// if traverseAll, traverse only all in transform hierarchy 
	gbool loadInThread; // load inlines in thread 
	
	int notInTransformHierarchy; // if (>0) currently traversed scene graph not part of transform hierarchy
	
	
	enum LoadMode {
		DONT_LOAD,	 	//!< don´t load
		LOAD_FIRST,		//!< load first than stop
		LOAD_IF_NEEDED,	//!< lazy, load only if needed
		LOAD_ALL		//!< load all
	};

	LoadMode loadInlines;	// loading flags for diffent sub media
	LoadMode loadTextures;
	LoadMode loadMovies;	//
	LoadMode loadSounds;
	LoadMode loadProtos;	// 

	int pendingLoads;	//!< number of URL nodes encountered, still loading 

	GView *view;		//!< ptr to view if needed
	CMyglView *myglView;// ptr to CMyglView	if needed
	
	//! return the browser for this traversal is beeing called
	GView *getBrowser() { return view; }


	//! Retreive current scene url or url home directory, as well as refresh time for child nodes with URL's
	virtual const char* GetUrl(CTime &childrenRefreshTime);	// implemented in GvTraverse.cpp
	virtual const char* GetHomeUrl();


	gbool attributesInitialized;	//!< has InitializeAttributes been called ?

	//! a linked list allocated on stack 
	GPathElement *topPath;

	//! retreive current attribute for attributeId, or NULL if attribute
	//! not initialized
	GStackableAttribute* Get(int attributeId) const { 
		if (attributeId >= current.Length()) return NULL;
		return(current[attributeId]);
	}
	
	//! Set pointer to current attribute for attributeId
	void SetCurrent(int attributeId,GStackableAttribute* attr) { current[attributeId] = attr; };
	
	//! Return 1 if attribute has been pushed at this level
	int Pushed(int attributeId) const { return current[attributeId]->level >= level; };

	//! initialize initial attribute setting
	void Set(GStackableAttribute* attr);


	//! pop this attribute, must be top !!!!!!!!!!!!!!!!!!!!!!!!!!!
	void Pop(GStackableAttribute *attr) {
			if (!attr) return; // trace should not occure
			top = attr->prev;
			SetCurrent(attr->AttrId(),attr->prevThisAttr);
			if (attr->prevThisAttr) attr->prevThisAttr->Set(*this);
			//delete attr;
			attr->FreeInstance();
	}
	
	//! push this attribute, becomes new top	
	void Push(GStackableAttribute *newTop) {
			 	Push(newTop ->AttrId(),newTop);
			 };

	//! push this attribute, becomes new top	of stack with index id
	void Push(int id,GStackableAttribute *newTop) {
			newTop->prev = top;
			newTop->prevThisAttr = Get(id);
			newTop->level = level;	// store level
			if (newTop->prevThisAttr == 0) {
			  //TRACE("Error: %s::Push attribute %s not initialized \n",this->ClassName(),newTop->ClassName());
			  //delete newTop;
				newTop->FreeInstance();
			} else {
			  
			  //TRACE("%s::Push  %s prev %s  \n",this->ClassName(),newTop->ClassName(),newTop->prevThisAttr->ClassName());
			  top = newTop;
			  SetCurrent(id,newTop);
			  newTop->Set(*this);
			}
	}

	// constructor
	GTraversal();
	~GTraversal();

	//! return current traversal (stack) level
	int Level() const { return (level); }
	
	//! increment current traversal (stack) level
	void Push() { 
			level++; 
	//		TRACE("Push %d (\n",level);
	}

	//! decrement current traversal (stack) level
	//! pop all attributes pushed since Push()

	void Pop() {
//			TRACE("Pop %d )\n",level);
		while (top && (top->level>= level)) { 
					//TRACE("\tpopping attribute %s\n",top->ClassName());
					Pop(top);
			 }
			 level --;
		}


	// virtual ovverrides
	//! overrided implementation will/can specific member function on node
	virtual int Do(GNode *n);


#ifdef _GFULL
	virtual int Do(GGeometry *n);
	
	virtual int Do(GGroup *n);
	virtual int EndDo(GGroup *n);

	virtual int Do(GAttribute *a);
	virtual int EndDo(GAttribute *a);
#endif


	virtual void InitializeAttributes(); 

};

// build a linked list/stack during traversal
// to get the path to the current node 
#define GPUSH_PATH(state,me) \
{\
	GPathElement __newTop(me,state.topPath);\
	state.topPath = &__newTop

#define GPOP_PATH(state) \
	state.topPath = __newTop.next; \
} 

#define GSET_PATH_FIELD(state,Field) \
	state.topPath->field = &Field;

#define GSET_PATH_INDEX(state,Index) \
	state.topPath->index = Index;

#define GSET_PATH_DATA(state,Data) \
	state.topPath->data = Data;

//
// GNode
// The root for all Objects in Scene Tree
// defines a list of virtual functions, which can be overridden for spcecial traversal action
class GNode : public RTRoot {
public :   
	RT(RTRoot);


	GNode() {};

	// define virtual actions
	virtual ~GNode() {};

	int Traverse(GTraversal &state);

	// predefined virtual Traversal actions

	// generic traverseal
	virtual int Do(GTraversal &state);  
	
	// Default action for predefined derived actions 
	// is to pass control to Do(GTraversal&)
	// compute bounding box 
	virtual int Do(GBBoxTraversal &state)       { return Do(*(GTraversal *) & state); } 
#if 0
	virtual int Do(GglRenderTraversal &state)   { return Do(*(GTraversal *) & state); } 
#else
	virtual int Do(GglRenderTraversal &state);
#endif

	virtual int Do(GglTransparencyRenderTraversal &state)  { return Do(*(GglRenderTraversal *) & state); } 

	virtual int Do(GD3DRenderTraversal &state)   { return Do(*(GTraversal *) & state);} 

	virtual int Do(GioTraversal &state);		// implemented in GioTraversal.cpp 
	virtual int Do(GPovIoTraversal &state);		// implemented in GioTraversal.cpp 
	virtual int Do(GBuildShellTraversal &state);// implemented in GBuildShell.cpp
	virtual int Do(GOptimizeTraversal &state);	// implemented in GOptimizeTraversal.cpp

	virtual int Do(GPathTraversal &state); // implemented in GRaySelect.cpp

	virtual int Do(GRaySelectTraversal &state); // implemented in GRaySelect.cpp

	virtual int Do(GLoadInlinesTraversal &state);// implemented in qvTraverse.cpp

	virtual int Do(GDisplayTreeTraversal &state); // implemented in GDisplayTree.cpp
	
	virtual int Do(GVrml2ConvertTraversal &state)   { return Do(*(GTraversal *) & state);} 

	// IO 
	virtual int io(AStream &a);

};

#ifdef _GFULL
//
// ROOT for non VRML based scene graph
//

class GBaseNode : public GNode 
{
public :

	RT(GBaseNode);
	GAttribute *attributes;		// Points to first attribute

	GBaseNode() : GNode()  { };


	// stuff not needed for VRML nodes
	GNode *next;
	int id;		// Object Id: to do use RTTI

	virtual int IsAttribute() ;
	virtual int IsGeometry() ;
	virtual int IsGroup() ;
	
	// what actions need to be performed on next update
	unsigned int activity;
	
	void AddActivity(unsigned int newActivity) { activity |= newActivity; }
	void ClearActivity() { activity = 0; }

	int  IsMarked() { return(activity & GMarked); }
	int  Mark() { return(activity |= GMarked); }
	int  UnMark() { return(activity &= ~GMarked); }

	void SetId(int Id) { id = Id; }

	virtual int Propagate(unsigned int newActivity);




};
//
// GAttributeNode
// a node with a optioanal list of attributes
// traverse would push and pop the attributes
// not used for VRML 

class GAttributeNode : public GBaseNode  {
public :
	RT(GNode);
	GAttribute *attributes;		// Points to first attribute

	GAttributeNode() : GBaseNode() 
				{ 
				 attributes = NULL; 
			   	};
	~GAttributeNode();
	int HasAttributes() { return (attributes != NULL); }

	// set a Attribute and take control over node
	void Set(GAttribute  *AttributeNode);
	
	// UnSet attribute with id Id
	// if found delete it and notify and return 1
	// else return 0

	int  UnSet(GId Id);

	GAttribute * FindAttribute(GId Id);
	void DeleteAttributes();
	virtual int Notify(GAction action, GAttribute *n);

	int Traverse(GTraversal &state,int mode);

	// IO 
	virtual int io(AStream &a);
};

//
// not used for VRML 
//

class GGroup : public GAttributeNode  {
public :
	RT(GAttributeNode);
	GGroup *next;			// Next brother to form list of subgroups
	GGroup *subgroups;	// Points to first subgroup
	GGroup *owner;			// Points to father
	
	char *name;				// name, if any
	
	GGeometry *geometry;	// Points to first geometry

	GGroup() : GAttributeNode() { next = NULL; subgroups = NULL; owner = NULL;
				 name = NULL; attributes = NULL; geometry = NULL; 
			   };
	~GGroup();
	
	int IsGroup();
	
	void SetName(const char *Name,int len = -1);
	
	int HasGeometry() { return (geometry != NULL); }
	int HasSubGroups() { return (subgroups != NULL); }

	// Traversal
	int Traverse(GTraversal &state);
	int TraverseAttributes(GTraversal &state,int mode);

	// insert a Geometry node and take control
	void Insert(GGeometry *GeometryNode);

	// link out the the geometry node and call destructor for node
	int  Delete(GGeometry *GeometryNode);

	// link in the the geometry node
	int  LinkIn(GGeometry *GeometryNode);

	// link out the the geometry node
	int  LinkOut(GGeometry *GeometryNode);

	// link in the subgroup node
	int  LinkIn(GGroup *Node);

	// link out the subgroup node
	int  LinkOut(GGroup *Node);


	GGeometry * FindGeometry(GId Id);

	void DeleteGeometry();
	void DeleteSubGroups();
	
	// Empty Group
	void Empty() { 
		DeleteGeometry(); 
		DeleteSubGroups(); 
		DeleteAttributes(); 
	}


	// delete types
	// Find a group by pathname, starting by this
	GGroup* FindSubGroup(const char *name,int name_len = -1,int create = 0);

    // move node to new group
    int Move(GGroup *target);
	
	// Add Activity, and propagate to owners
	int Propagate(unsigned int Activity);

	// Notify Group that action has/(will ?) bee performed
	int Notify(GAction action, GGroup *n);
	int Notify(GAction action, GGeometry *n);

	// IO 
	virtual int io(AStream &a);

};


#define ForGroups(LIST,ELEMENT) for(GGroup *ELEMENT=LIST; ELEMENT != NULL; ELEMENT = ELEMENT->next)
#define ForAttributes(LIST,ELEMENT) for(GAttribute *ELEMENT=LIST; ELEMENT != NULL; ELEMENT = ELEMENT->next)
#define ForGeometry(LIST,ELEMENT) for(GGeometry *ELEMENT=LIST; ELEMENT != NULL; ELEMENT = ELEMENT->next)

//
// Root for all Attribute nodes
// not used for VRML

class GAttribute : public GBaseNode {
public :
	RT(GNode);

	GAttribute *next;

	GAttribute() : GBaseNode() { next = 0; }
	int IsAttribute();

	virtual int Traverse(GTraversal &state,int mode);
	// Assign Attribute (only if same type)
	virtual int Set(GAttribute *a);
};

// Root for all geometry nodes
//
class GGeometry : public GAttributeNode {
public:
	RT(GGeometry);

	GGeometry *next;
	GGroup *owner;

	GGeometry(GGeometry * Next = NULL, GGroup *Owner=NULL) : GAttributeNode()
				{ next = Next; owner = Owner; }

	void SetOwner(GGroup *Owner) { owner = Owner; }

	int IsGeometry();
	virtual int Traverse(GTraversal &state);


	// move node to new group
	int Move(GGroup *target);

	int Notify(GAction action) { if (owner) owner->Notify(action,this); return(0); };

	// Call this if node contents has been changed
	int NotifyChanged() { AddActivity(GRedraw);
						  return (Notify(GGeometryChanged));
						}; 	

};

class GInstance : public GGeometry
{
public :
	RT(GGeometry);

	GGroup *target;		// Group to be included


	GInstance(GGroup *Target=NULL) : GGeometry() { SetId(GIdInstance); target = Target; }

	int Traverse(GTraversal &state);
	// IO 
	int io(AStream &a);
};

// Attribute GStyle : a refrence to target group style
class GStyle : public GAttribute
{
public :
	RT(GAttribute);

	GGroup *target;
	GStyle(GGroup *Target=0) : GAttribute() { SetId(GIdStyle); target = Target; }

	int Traverse(GTraversal &state,int mode);
	// IO
	int io(AStream &a);
};

#endif


/*! \class GMatrixStack
 *  \brief stackable attribute for implementing a Matrix stack
 *   
 */

class GMatrixStack : public GStackableAttribute
{
   
   public :
   GSTACKABLE_ATTRIBUTE(GMatrixStack)

   GMatrixStack() : GStackableAttribute() { 
	   mInverseOk = viewpointLocalOk= viewVolumeLocalOk=0;  
	   model2screenOk=0;
	   viewPlaneLocalOk=0;
   }

   //! zero the attribute
   void Init() { mInverseOk = viewpointLocalOk = viewVolumeLocalOk=viewPlaneLocalOk=0; model2screenOk=0;  }

   Matrix m;			//!< matrix 
   Matrix mInverse;		//!< inverse of m, valid if mInverseOk
   
   Matrix model2screen; //!< matrix transforms from model to screen 
						// for custom lighting 

   BEGIN_PACKED_STRUCT //struct  { // GCC does not like this ?
		int mInverseOk:1;		//!< flag if mInverse is valid ?
		int viewpointLocalOk:1; //!< flag if viewpointLocal is valid ?
		int viewVolumeLocalOk:1;//!< flag if viewVolumeLocal is valid ?
		int model2screenOk:1;	//!< flag if model2screen is valid ?
		int viewPlaneLocalOk:1; //!< flag if viewPlaneLocal is valid ?
   END_PACKED_STRUCT //};


   Point viewpointLocal;		//!< viewpoint in local coordinate system
   
   PointArray viewVolumeLocal;  //!< 5 points of viewfrustrum, pyramid with viewPoint

   PlaneArray viewPlaneLocal;  //!< viewfrustrum planes in local coordinate system

   //! set to identity 	  
   void Identity() { m.Identity(); mInverse.Identity(); mInverseOk = 1;viewpointLocalOk= viewVolumeLocalOk=viewPlaneLocalOk=0; }

   //! append matrix
   void AppendLeft(const Matrix &left) { 
	   m= left * m; mInverseOk= 0;viewpointLocalOk= viewVolumeLocalOk=viewPlaneLocalOk=0; 
	   model2screenOk=0;
   }

   //! get matrix
   Matrix& GetMatrix() { return(m); }
   //! get matrix inverse
   Matrix& GetMatrixInverse() { 
	   if (!mInverseOk) 
	   { mInverse.SetToInverse(m); // mInverse = Invert(m); 
	     mInverseOk= 1;
	   } 
	   return(mInverse);
   }

   //! set matrix absolute
   void SetMatrix(const Matrix &newM) { 
	   m=newM; 
	   mInverseOk = viewpointLocalOk= viewVolumeLocalOk=viewPlaneLocalOk=0;  
	   model2screenOk=0;
   }

   //! set matrix & inverse absolute 
   void SetMatrix(const Matrix &newM,const Matrix &newInverse) { 
	   m=newM; mInverse = newInverse; 
	   mInverseOk= 1; viewpointLocalOk= viewVolumeLocalOk=viewPlaneLocalOk=0;  
   	   model2screenOk=0;

   }

   //! set the viewPoint and transform to local space
   void SetViewpointLocal(const Point &global) {
		viewpointLocal =  GetMatrixInverse() * global;
		viewpointLocalOk=1;	 
   }

   //! set the viewVolume and transform to local space
   void SetViewVolumeLocal(const PointArray &global) 
   {
		const Matrix &m=GetMatrixInverse();

		int l= global.Length();
		
		viewVolumeLocal.SetLength(l);
		for(int i=0; i< l; i++) 
			viewVolumeLocal[i] = m * global[i];

		viewVolumeLocalOk=1;	 
	}

   //! update the viewPlanes and transform to local space
   void SetViewPlanesLocal(const PlaneArray &global) 
   {
		const Matrix &m=GetMatrixInverse();

		int l= global.Length();
		
		viewPlaneLocal.SetLength(l);
		for(int i=0; i< l; i++) 
			viewPlaneLocal[i] = m * global[i];

		viewPlaneLocalOk=1;	 
	}

   
   
};

//
//  glVersion  of state Matrix attibute
//  implementation will set the Matrix in rendering layer 
class GglMatrixStack : public GMatrixStack {

public :
    GSTACKABLE_DERIVED_ATTRIBUTE(GglMatrixStack)
    GglMatrixStack() : GMatrixStack() { }
	
	void Init() { GMatrixStack::Init(); }

	// overwritten virtual set function to set opengl state variables
	void Set(GTraversal &state);
};


// GMatrixStack	a matrix entry, pushable on traversal stack
class GTextureMatrixStack : public GStackableAttribute
{
   
   public :
   GSTACKABLE_ATTRIBUTE(GTextureMatrixStack)
   GTextureMatrixStack() : GStackableAttribute() { }

   Matrix m;

   void Identity() { m.Identity();  }

   void AppendLeft(const Matrix &left) { m= left * m; }
   Matrix& GetMatrix() { return(m); }
   
   void SetMatrix(const Matrix &newM) { m=newM;  }
};

//  glVersion 
class GglTextureMatrixStack : public GTextureMatrixStack {

public :
    GSTACKABLE_DERIVED_ATTRIBUTE(GglTextureMatrixStack)
    
    GglTextureMatrixStack() : GTextureMatrixStack() 
	{ 
		mVersion = 0; // needed for D3D
	}

	void Init() { 
		mVersion = 0;
		GTextureMatrixStack::Init(); 
	}

	// overwritten virtual set function to set opengl state variables
	void Set(GTraversal &state);

	long mVersion; // need to give each Matrix a distinctive version
				   // because matrix need to applied directly to objects 	
};


#ifdef _GFULL

class GMatrix : public GAttribute
{
public :
	RT(GAttribute);
	Matrix m;
	GMatrix () { SetId(GIdMatrix); m=IdentityMatrix(); }
	GMatrix(const Matrix &M) { SetId(GIdMatrix); m=M; AddActivity(GRedraw); }
	//GMatrix(const float M[4][4]) : m(M) { SetId(GIdMatrix);  AddActivity(GRedraw); }

	void Append(const Matrix &M) { m*=M; AddActivity(GRedraw); }
	void Set(const Matrix &M) { m=M; AddActivity(GRedraw); }

	int Set(GAttribute *a);

	int Traverse(GGeometryTraversal &state,int mode);
	
	int Do(GTraversal &state);   

#if 0	
	int Do(GglRenderTraversal &state);   
	int EndDo(GglRenderTraversal &state);
#endif

	//virtual int Do(GBBoxTraversal &state)       { return(-1); } 

	// IO
	int io(AStream &a);
};
#endif


//
// GFaceColor
//
class GFaceColorStack : public GStackableAttribute
{
public :
   GSTACKABLE_ATTRIBUTE(GFaceColorStack)
   GFaceColorStack() : GStackableAttribute() { }
   GFaceColorStack(const Point &Color) : color(Color) { }

   Point color;
   
};


// 
class GglFaceColorStack : public GFaceColorStack {

public :
    GSTACKABLE_DERIVED_ATTRIBUTE(GglFaceColorStack)

	// overwritten virutal set function to set opengl state variables
	GglFaceColorStack() : GFaceColorStack() { }
	GglFaceColorStack(const Point &Color) : GFaceColorStack(Color) { }
	void Set(GTraversal &state);
};


#ifdef _GFULL

class GFaceColor : public GAttribute
{
public :
	RT(GAttribute);
	Point color;
	GFaceColor () { }
	GFaceColor(const Point &Color) { color = Color; AddActivity(GRedraw); }
	void Set(const Point &Color) { color = Color; AddActivity(GRedraw); }

//	int Set(GAttribute *a);
	int Do(GTraversal &state);   

	// IO
	int io(AStream &a);
};

class GBBox : public GAttribute
{
public :
	RT(GAttribute);

	BBox m;
	GBBox () { SetId(GIdBBox); }
	
	GBBox(const BBox &M) { SetId(GIdBBox); m=M; /*AddActivity(GRedraw);*/ }

	void Append(const Matrix &M) { m*=M; /* AddActivity(GRedraw); */ }
	void Set(const BBox &M) { m=M; /* AddActivity(GRedraw);*/ }

	int Set(GAttribute *a) { if (a->id  == id) {
								Set(((GBBox *) a) -> m); 
								return(1);
							  }
							  return(0);
							} 	

	int Traverse(GGeometryTraversal &state,int mode);

	// IO
	int io(AStream &a);

};
#endif


//@class time status for animation
class GTime {
public :
	//@cmember the current time
	float t;

	//@cmember the current time in global time frame 
	//double now; // == start time + t 

	//@cmember the current time step from previous time
	float dt;

	//@cmember the current time range 
	float t0,t1;

	//@cmember if animation is enabled
	int enabled;

	//@cmember if animation is paused
	gbool pause;

	//@cmember if animation repeats
	gbool wrap;
	
	//@cmember current frame number 
	int frame;
	
	GTime() { t=0.0; t0 = 0.0; t1= 10.0; dt = 0.1; 
			  enabled = 1; pause=0; wrap = 1; 
			  frame = 0;  
				// now = 0.0; 
			}

	//@cmember step to next time frame
	void Step(float DT) 
		{ 
		t+=DT; 
		//now += DT; 
		
		if (DT >0) {
			if (t>t1)  { 
		   		if (wrap) { t=t0; frame++;} 
		   		else enabled = 0;
		 	}
		} else {
			if (t<t0)  { 
		   		if (wrap) { t=t1; frame++;} 
		   		else enabled = 0;
		 	}
		}
		
		}

	//@cmember step to next time frame
	void Step() { Step(dt); }

	//@cmember reset local time 
	void Reset() { t = t0; frame=0; }

	//@cmember reverse direction 
	void Reverse() { dt = -dt; }

	//@cmember set local time range 
	void SetRange(float T0, float T1) { t0 = T0, t1 = T1;
					if (T1 < T0) { t0 = T1; t1 = T0; }
	}

	//@cmember gett local time range 
	void GetRange(float &T0, float &T1) { T0 = t0, T1 = t1; }

	//@cmember animation enabled ? 
	int On() { return ( enabled && (t>=t0) && (t<=t1)); }

	//@cmember timer enabled ? 
	int Enabled() { return(enabled); }

	void Enable() { enabled =1; }

	int Paused() { return(pause); }


	//@cmember get current system time in secs from 1970 GMT
	static double GetGlobalTime(); 

};
	 

class GTimeStack : public GStackableAttribute
{
public :
   GSTACKABLE_ATTRIBUTE(GTimeStack)
   GTimeStack() : GStackableAttribute() { }
   GTimeStack(const GTime &Time) : GStackableAttribute() { time = Time; }

   GTime time;

   float T() { return(time.t); }
   //double Now() { return(time.now); }
   float Enabled() { return(time.Enabled()); }
   
};

// predefine return codes from Traverse / Do()

// return code to signal traversal stop 
#define TRAVERSE_STOP -10

// object not yet loaded 
#define TRAVERSE_NOT_LOADED -2

// object delayed
#define TRAVERSE_DELAYED -3

// object was culled 
#define TRAVERSE_CULLED -4


// Predefined traversal actions
// register attribute Matrix

class GGeometryTraversal : public GTraversal {
public :

	RT(GGeometryTraversal);


	GGeometryTraversal() : GTraversal() {
		camera = NULL; 
		physicalCamera = NULL;
        viewInfo = NULL; 
        doViewCulling=FALSE;
		traversalOrder = 0;
		//viewLocalValid= 0;
		currentMatrix = NULL;
		 
		doLodScale = gfalse;

		lodScale = 1.0f;
		drawPath = G_DRAW_NORMAL;
	};										

	GCamera *camera;		   //!< pointer to global logical camera in effect
	GCamera *physicalCamera;   //!< pointer to physical rendering camera in effect
    
	GViewInfo *viewInfo;    //!< points to current viewing transformation info
    
	gbool doViewCulling;	//!< check bboxes against view
	gbool doLodScale;		//!< adapt LOD ranges 
	float lodScale;			//!< scale factor for LOD range, default 1.0

	typedef enum {
		G_DRAW_NORMAL,
		G_STEREO_LEFT,
		G_STEREO_RIGHT,
	} GDrawPath;
	
	GDrawPath drawPath;		//!< type of stereo drawing path


    void InitializeAttributes(); 

	GMatrixStack *currentMatrix; // top of matrix stack



	//! append and push a new matrix 
	inline void AppendMatrix(const Matrix &m) 
	{
        GMatrixStack *current,*prev;
	    prev = (GMatrixStack*) Get(GMatrixStack::attrId);
	    current = (GMatrixStack*) prev->NewInstance();
	    current->m.Multiply(m,prev->m);
	    Push(current);
		currentMatrix = current;
	}
	
	
	//! get the current matrix stack entry 
	// could be done, assume currentMatrix always valid to eleminate Get call 
	inline GMatrixStack * GetCurrentMatrix() 
	{
			GMatrixStack *m= (GMatrixStack*)Get(GMatrixStack::attrId);
			currentMatrix = m;
			return m;
	}

	
	// get current modelling matrix
	inline Matrix * GetMatrix()  { 
			GMatrixStack *m= (GMatrixStack*)Get(GMatrixStack::attrId);
			if (m) return &m->m;
			else return NULL;
		}

	//! get current modelling matrix
	inline const Matrix& GetMatrixRef() const { 
			GMatrixStack *m= (GMatrixStack*)Get(GMatrixStack::attrId);
		    // assume always valid !		
			return m->GetMatrix();
		}
	
	//! get inverse of current modelling matrix
	inline const Matrix& GetMatrixInverse() const { 
			GMatrixStack *m= (GMatrixStack*)Get(GMatrixStack::attrId);
		    // assume always valid !		
			return m->GetMatrixInverse();
		}
	
	//! get current texture matrix
	inline Matrix * GetTextureMatrix() const { 
			GTextureMatrixStack *m= (GTextureMatrixStack*)Get(GTextureMatrixStack::attrId);
			if (m) return &m->m;
			else return NULL;
		}

    //! is this bounding box visible with respect to current view volume
    gbool IsBoxVisible(const BBox &box) const;


	// BSP Stuff
	//gbool viewLocalValid;
	//Point viewpointLocal;		// viewpoint in local coordinate system
	//PointArray viewVolumeLocal; //  pyramid with viewPoint in local coordinate system

	Point viewpoint;
	PointArray viewVolume; //  pyramid with viewPoint
	Array<Plane> viewPlanes; // the viewplanes 

	int traversalOrder; // BSP traversal ort 

	// set pointer to camera
	void SetCamera(GCamera *camera);

	// set pointer to physical camera
	void SetPhysicalCamera(GCamera *camera);



};


//
//	GPathTraversal
//
//  abstract base clase for traversal maintaining the current path
class GPathTraversal : public GGeometryTraversal 
{
public :
	RT(GPathTraversal);

	gbool resolveRelativeUrls; // flag for some traversals to resolve local urls now to absolute url

	GPathTraversal() : GGeometryTraversal() {resolveRelativeUrls = FALSE; }


	// get the current path 
	void GetPath(GPath &thePath);


	
	int Do(GNode *n) {	return (n->Do(*this)); }
	//int Do(GAttribute *a)	 { return (a->Do(*this)); }

	// is the node n already in the set of parents ? (recursive link)
	// gbool IsParent(GvNode *n) { if (topPath) return topPath->IsInPath(n); else return 0; }


};


//
//	GBBoxTraversal
//
// traverse graph to find bounding box
class GBBoxTraversal : public GGeometryTraversal 
{
public :
	RT(GBBoxTraversal);
	
	BBox bbox;		// resulting boundingbox in global coordinates

	gbool useBBoxes; // use bbox Information of VRML Nodes


	GBBoxTraversal() : GGeometryTraversal() { 
							useBBoxes = FALSE; 
							// needShapes = TRUE; // to think ??
	
	}

	// return compute bounding box 
	BBox &GetBBox() { return(bbox); }

	// add a local coordinate bounding box to current sum 
	void Union(const BBox &b) {
		
		if (b.IsEmpty()) return; //  ignore if empty 

		// transform b by current matrix 
		// to do optimize ?
		GMatrixStack *m= (GMatrixStack*)Get(GMatrixStack::attrId);
		if (m) bbox.Union(m->m*b);
		else
			bbox.Union(b);
	}
	
	int Do(GNode *n) { return (n->Do(*this)); }
	

	//int Do(GAttribute *a)	 { return (a->Do(*this)); }

   	void InitializeAttributes(); 

};

class GFrameStats {
public :
	 DWORD frame;				// the frame counter
	 int prims;					// number of geometric primitves
	 int tris;					// number of triangles drawn
	 
	 float budget;				// the budget used
	 float quality;				// the quality computed	
	 float frameTime;			// time used

	 GFrameStats() { Zero(); }
	 
	 void Zero() {
		 frame = 0;
		 prims=tris=0;
		 budget=0;
		 quality=0;
		 frameTime=0;
	 }

};

//
// GRenderTraversal
//

// register all important rendering attributes
// Register Attributes : TextureMatrix FaceColor

enum GTransparencyMode {
		GTRANSPARENCY_NONE,		// process in normal order during render
		GTRANSPARENCY_DELAYED	// store GTransparenElement objects during render
};

class GRenderTraversal : public GGeometryTraversal {
public :

	RT(GRenderTraversal);
	
	BOOL doClipping;	 // geometry need clipping to view frustrum	
	int firstLightIndex; // index of first free light 

	GTransparencyMode transparencyMode; // the processing mode for transparent shapes 

	gbool useTextures; // flag : use textures 

	gbool doProximitySensors; // process proximity sensors during rendering
	gbool doVisibilitySensors; // process visibility sensors during rendering

	GRenderTraversal() : GGeometryTraversal() { 
		useTextures = TRUE; 
		firstLightIndex = 0;
		doProximitySensors = TRUE;
		doVisibilitySensors = TRUE;
		needShapes = TRUE;
		doViewCulling=TRUE;
		doClipping = TRUE;
		transparencyMode = GTRANSPARENCY_NONE;
		nurbsTessellationScale = 1.0f;
		nurbsTessellationGroupS = 1.0f;
		nurbsTessellationMode=0;
	}

    void InitializeAttributes(); 

    // fully process the transparent elements, and clean list at end
	virtual int RenderTransparentElements();

	GFrameStats lastFrameStats;		// total stats of last frame
	GFrameStats frameStats;			// accumulated frame stats
	float nurbsTessellationScale;	// scale factor for nurbs tesselation, contains all scaling factors (group, fuzzy)
	float nurbsTessellationGroupS;	// scale factor from the nurbsGroup
	int nurbsTessellationMode;		// tessellation / feed back mode 


};


class RenderState;		// implemented in Glrender

class glMatrix;
class GTransparentElement;



// 
// Register gl Attributes : Matrix TextureMatrix FaceColor



class GglRenderTraversal : public GRenderTraversal 
{
public :
    RT(GglRenderTraversal);
    
	RenderState *glState; // Points to the current Render Context in use

	//Stack<glMatrix*> matrixStack;

	GglRenderTraversal(RenderState * GlState=0) : GRenderTraversal() 
	
	{	glState = GlState;
		loadInlines = LOAD_IF_NEEDED;
		loadTextures = LOAD_IF_NEEDED;
		loadMovies = LOAD_IF_NEEDED;
		loadSounds = LOAD_IF_NEEDED;
		loadProtos = LOAD_IF_NEEDED;
	}

	int Do(GNode *n) 
	{
		return (n->Do(*this));	// delegate to virtual function Do(GglRenderTraversal)
	}
	

	// perform check of bounding box against transformed view pyramid
	// Result : true : continue processing, false : box not visible OR bbox empty
	// bbox treated not empty if bboxSize.x >0 
	gbool CullCheck(GvSFVec3f &bboxCenter,GvSFVec3f &bboxSize); // implementend in Gv2Group.cpp
	gbool CullCheck(BBox &bbox);


//	int Do(GAttribute *a)	 { return (a->Do(*this)); }
//	int EndDo(GAttribute *a);
	
   	void InitializeAttributes(); 



};

#ifdef _GFULL


typedef GBaseNode* GKey;



// The World of Groups
class GWorld	{

protected:
public:

	GGroup *root;

	GGroup *objects;	// objects to be instanced
	GGroup *styles;		// styles to be used


	Stack<GGroup*> stack;


	void Push(GGroup *s) { stack.Push(current);  current=s; }
	void Pop() { current = stack.Pop(); }

	int doTrace;	// trace certain operations for debugging

public:

	GGroup *current;

	GWorld() {
		   root = new GGroup();
		   current = NULL;
		   objects = NULL; styles = NULL;
		   doTrace = 1;
		   	

		 };
	~GWorld();


	void G__cdecl Error(const char * message,...);

	GGroup* Current() {return(current); }
	GGroup* Root() { return(root); }

	int SetTrace(int DoTrace) { doTrace = DoTrace; return(doTrace); }

	int Traverse(GTraversal &t) { return(root->Traverse(t));}

	// Open (and Create) group by name 
	void Open(const char *PathName);
	
	// close currently open group, go back to previous
	void Close();

	// Open and return the key 
	GKey KOpen(const char *PathName);

	// Open group given a Key to a group
	void OpenByKey(GKey key);

	// Delete Group or geometry given the key
	void DeleteByKey(GKey key);

	// Open group by node 
	void Open(GGroup *group) { OpenByKey(group); }


	// Move Group or geometry to a group given by key
	void MoveByKey(GKey key,GGroup *target);

	// Empty current world
	void Empty() {
	   
	   if (root) root->Empty();
	   if (objects) objects->Empty();
	   if (styles)  styles->Empty();
	   
	   stack.SetLength(0);
	   current = 0;
	}

	// write World starting at root to file
	int Write(const char *PathName);

};

extern int test(int argc,char *argv[]);

#endif

void GClassFreeAttributeClasses();


#endif

