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
/******************************************************************************
@doc

@module GClass.cpp - GLView graphical object classes implementation|

Copyright (c) 1995 -1998	by Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:

Changes:

Todo :

******************************************************************************/

#include "stdafx.h"

// all in stdafx.h

//#include <string.h>
//#include <stdarg.h>
//#include <stdio.h>

// For actual nodes
//#include "rt.h"

//#include "point.h"
//#include "pointa.h"			
//#include "Stack.h"

#include "gclass.h"


// only for GglRenderTraversal
#include "grender.h"

#include <sys/types.h> 
#include <sys/timeb.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


gbool PCInit = gfalse;
gbool usePC = gfalse;


LARGE_INTEGER PCFrequency;

LARGE_INTEGER PCStart;
double PCStartTime;


double GTime::GetGlobalTime() 
{
	 struct _timeb t;
	 double tt;

     if (!PCInit) {
		if (QueryPerformanceFrequency(&PCFrequency)) {
			  _ftime( &t );
			PCStartTime =(double ) t.time + ((double) t.millitm) / 1000.0;
			if (QueryPerformanceCounter(&PCStart))
				usePC=gtrue;
		}
		PCInit = gtrue;
	 }

	 if (usePC) {
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		tt = PCStartTime + ((double ) (now.QuadPart-PCStart.QuadPart) / (double ) PCFrequency.QuadPart);
	 }
	 else {
		 _ftime( &t );
		 tt=(double ) t.time + ((double) t.millitm) / 1000.0;
		 TRACE("Time %d %d \n",t.time,(int) t.millitm); 
	 }
	 return tt;
  
}





// Property describe Propertys of objects
// Intended use :
//	Class independent query functions, user interface functions
// evaluation/rule functions
enum GPropertyTypes {
		GPTboolean,
		GPTint,
		GPTfloat,
		GPTstring,
		GPTpoint,
		GPTvector,
		GPTmatrix,
		GPTpointarray
};

class GProperty :public RTRoot {
public :
//	RT(RTRoot);

	GProperty * next;    // linked together in list
	GNode *owner;		// The node having the property
	char *name;
	int namedid;		// nameidentifier for name

	char *prompt;		// optinal prompt for user interface
	int typedid;		// typeindentifier

};

class GFloatProperty : public GProperty {
public :
//	RT(GProperty);

	float *v;
	float vmin,vmax;
	float vdefault;
	
	GFloatProperty(char *Name,float *V,float Min,float Max,char *Prompt = NULL,float Default = 0.0)
			{ name = Name; v=V; vmin=Min; vmax = Max, prompt = Prompt;
			  typedid = GPTfloat;
			  next = NULL;
			};

	int GNode::*Get (float &v);		// member function of Gnode setting variable
	int GNode::*Set (float &v);
};

// property construct functions
void Property(GProperty * &prev,char *Name,float *V,float Min,float Max,char *Prompt = NULL,float Default=0.0)
{ GFloatProperty *p;
  p = new GFloatProperty(Name,V,Min,Max,Prompt,Default);
  prev = p;
}

#if 0
class GACamera : public GAttribute {
public:
   RT(GAttribute);

   Point position;
   Point target;
   Point up;
   float field_width,field_height;
   int perspective;

   float Near,Far;
   float aspect; // Aspect ratio of screen window
   float fovy;	// Field of view
};
#endif

#ifdef _GFULL
class GAMaterial : public GAttribute {
      // GL
};

#endif




// ****************************************************************************
//
// class GNode
//
RTIMP_ABSTRACT(GNode,0,RTRoot);



int GNode::Traverse(GTraversal &state)
{
	return  state.Do(this); // call virtual Do invoker on state 
}


// predefined Traversal actions
int GNode::Do(GTraversal &state)   
{ 
	//TRACE("%s::Do(GTraversal),no action for %s\n",this->ClassName(), state.ClassName());
	return(-1); 
} 
#if 0
static int level=0;

int GNode::Do(GglRenderTraversal &state)   { 

	level++;
	int ret=Do(*(GTraversal *) & state); 
	if (level > 60) {
		state.level++;
		TRACE("Very deep stack \n");
	}		
	level--;
	return ret;


} 
#else
int GNode::Do(GglRenderTraversal &state)
{ 
	return Do(*(GTraversal *) & state); 
} 

#endif



// IO
int GNode:: io(AStream &a)
{
	if (a.isOutput()) {
		a.Sep('(');
		a.io("Gnode");
		//a.io("Id = ");
		//a.io(id);
		a.Sep(')');

	}
	else {
	  a.Error("GNode::Can´t read unknow object type");
	}
	return(a.stat);
}

#ifdef _GFULL
// ****************************************************************************
//
// class GBaseNode
// 
RTIMP_ABSTRACT(GBaseNode,0,GNode);

int GBaseNode::IsAttribute() { return(0); }
int GBaseNode::IsGeometry()  { return(0); }
int GBaseNode::IsGroup()	 { return(0); }

int GBaseNode::Propagate(unsigned int newActivity)
{
	AddActivity(newActivity);
	return(0);
}

// ****************************************************************************
//
// class GAttributeNode
// 

RTIMP(GAttributeNode,0,GBaseNode);

GAttributeNode::~GAttributeNode()
{
	DeleteAttributes();
}


// Delete Attribute list of group

void GAttributeNode::DeleteAttributes()
{  GAttribute *node,*next_node;
   for (node = attributes; node != NULL; node=next_node) {
	next_node = node->next;
	delete node;
   }
   attributes = NULL;
}

int GAttributeNode:: Traverse(GTraversal &state,int mode)
{ 
   for (GAttribute * a = attributes; a != NULL; a=a->next) { a->Traverse(state,mode); }
   return(1);
}
// Set attribute a on a node, Control over object a is taken
// If attribute is present assign it , else link in

void GAttributeNode :: Set(GAttribute *a)
{ GAttribute *i, **prev;
   
   for (	i= attributes,prev = &attributes; 
   			i != NULL; 
   			prev = &i->next, i=i->next) 
   { 
   	  if (i->id == a->id) {
	  	i->Set(a);		// Asign
		delete a;
		Notify(GAttributeChanged,i);
		return;
   	  }	
	  else if (i->id > a->id) {		// link in if next is greater
	    a->next = i;
		*prev= a;
		Notify(GAttributeAdded,a);
		return;
	  }
   }
   *prev = a;
   a->next = NULL;
   Notify(GAttributeAdded,a);
}

// UnSet attribute with id Id
// if found delete it and notify and return 1
// else return 0

int GAttributeNode::UnSet(GId Id)
{ GAttribute *i, **prev;
   
   for (	i= attributes,prev = &attributes; 
   			i != NULL; 
   			prev = &i->next, i=i->next) 
   { 
   	  if (i->id == Id) {
		*prev = i->next;
		Notify(GAttributeDeleted,i);
		delete i;
		return(1);
   	  }	
	  else if (i->id > Id) {		// break if next is greater
		return(0);
	  }
   }
   return(0);
}

// Search for a attribute with id Id
// If found return pointer to it

GAttribute * GAttributeNode::FindAttribute(GId Id)
{
   GAttribute *i;
   
   for (i= attributes; i != NULL; i=i->next) 
   { 
   	  if (i->id == Id)	return(i);
	  if (i->id > Id) break;
   }	
   return(NULL);
}

int GAttributeNode::Notify(GAction action, GAttribute *n)
{  unsigned int a;
	a = n->activity;
	if (a & GRedraw) {		// If Attributes affects whole group redraw
		AddActivity(a);
		Propagate(a & ~(GRedraw) | GRedrawSome); // Propagate only a RedrawSome	
	}
	else if (a !=0) Propagate(a);
	return(0);
}


//  IO
int GAttributeNode::io( AStream &a)
{
	if (a.isOutput()) {
		    for (GAttribute *n= attributes; n != NULL; n=n->next) { n->io(a); }
	}
	return(a.stat);
}

// ****************************************************************************
//
// class GGroup
// 

RTIMP(GGroup,0,GAttributeNode);

GGroup::~GGroup()
{
  DeleteSubGroups();
  DeleteGeometry();
  DeleteAttributes();
  SetName(NULL,0);
}

int GGroup::IsGroup()   { return(1); }

void GGroup::SetName(const char *Name,int len)
{
   if (len == -1) len= strlen(Name);
   if (name) delete name;
   if (Name) {
      name = new char [	len+1];
      strncpy(name,Name,len); name[len]=0;
   } else name = NULL;
}


int GGroup::Traverse(GTraversal &state)
{

   if (state.Do(this))  {

//   for (  GAttribute *a= attributes; a != NULL; a=a->next) { a->Traverse(state,0); }
   	GAttributeNode::Traverse(state,0);  

   	for (GGeometry *g= geometry; g != NULL; g=g->next) { g->Traverse(state); }

   	for (GGroup *s= subgroups; s != NULL; s=s->next) { s->Traverse(state); }

   	//for (   GAttribute *a = attributes; a != NULL; a=a->next) { a->Traverse(state,1); }
   	// not needed GAttributeNode::Traverse(state,1);  
   
   state.EndDo(this);
   return(1);
   }
   return(0);
}

// link in the the geometry node
int  GGroup::LinkIn(GGeometry *GeometryNode)
{
  if (!GeometryNode) return(0);

  GeometryNode->next = geometry;
  GeometryNode->owner = this;

  geometry=GeometryNode;
  Notify(GGeometryMovedIn,GeometryNode);

  //?? sort in Node depending on TypeID
  return(1);
}


// link out the the geometry node from the group without destroying it
// Return 1 if the node was found and removed

int  GGroup::LinkOut(GGeometry *GeometryNode)
{  GGeometry *node,**prev_node;

   for (	node = geometry,prev_node=&geometry;
			node != NULL; 
			node=node->next)
   {
      if (node == GeometryNode) {
	  *prev_node=node->next;
	  node->next = NULL;
	  
	  Notify(GGeometryMovedOut,GeometryNode);
	  
	  return(1);
      }
      prev_node = &node->next;
   }
   return(0);
}

// link in the subgroup node
int  GGroup::LinkIn(GGroup *Node)
{
  if (!Node) return(0);

  Node->next = subgroups;
  Node->owner = this;

  subgroups=Node;

  //?? sort in Node depending on name ??
  return(1);
}


// link out the subgroup node
// return 0 if GroupNode not found in list

int  GGroup :: LinkOut(GGroup *GroupNode)
{  GGroup *node,**prev_node;

   for (	node = subgroups,prev_node=&subgroups;
			node != NULL; 
			node=node->next)
   {
      if (node == GroupNode) {
	  *prev_node=node->next;
	  node->next = NULL;
	  return(1);
      }
      prev_node = &node->next;
   }
   return(0);
}

	// insert a Geometry node and take control
void GGroup::Insert(GGeometry *GeometryNode)
{
	LinkIn(GeometryNode);
}


// link out the the geometry node and call destructor for node
int  GGroup::Delete(GGeometry *GeometryNode)
{   int ret;
	ret= LinkOut(GeometryNode);
	delete GeometryNode;
	return(ret);
}


// Delete all Subgroups of group
void GGroup::DeleteSubGroups()
{  GGroup *node,*next_node;
   for (node = subgroups; node != NULL; node=next_node) {
	next_node = node->next;
	node->next = NULL;
	delete node;
   }
   subgroups = NULL;
}

// Delete all gGeometry of group

void GGroup::DeleteGeometry()
{  GGeometry *node,*next_node;
   for (node = geometry; node != NULL; node=next_node) {
	next_node = node->next;
	delete node;
   }
   geometry = NULL;
}

// traverse attributes of group
int GGroup:: TraverseAttributes(GTraversal &state,int mode)
{
   for (GAttribute *i= attributes; i != NULL; i=i->next) { i->Traverse(state,mode); }
   return(1);
}


// Search for geometry with id Id
// If found return pointer to it
// NULL otherwise

GGeometry * GGroup::FindGeometry(GId Id)
{
   for (GGeometry *node = geometry; node != NULL; node=node->next) {
   	  if (node->id == Id) return(node);
   }
   return(NULL);
}

// find next name component until string end or delimiter /
int find_path_del(const char *name,int len)
{  int i=0;

   while ( (i<len) && (name[i] != '/')) i++;
//   if (name[i] == '/')  ;
   return (i);
}

// Starting from group, find a group according to pathname name
// syntax supported
// .	^  name/rest

GGroup* GGroup::FindSubGroup(const char *name,int name_len,int create)
{   GGroup * result=NULL;
    GGroup *curnode = this;
    int len;

    if (name_len == -1)	name_len = strlen(name);

    len=find_path_del(name,name_len);

    while ( (len > 0) && (curnode != NULL)) {

    int found=1;
     // parse name from [0 to len -1 ]

    if ((len == 1)  && name[0]=='.') result = curnode;
    else if ((len == 1)  && name[0]=='^') result = curnode->owner;
    else {
       found = 0;

       ForGroups(curnode->subgroups,s)
       { if (s->name && strncmp(name,s->name,len) == 0)	// to do unnamed segments
	    { result = s;  found = 1; break; }
       }

      if (!found) {
	  if (create) {			// create new group
	     result = new GGroup();
		 result->SetName(name,len);
	     curnode->LinkIn(result);
	     found = 1;
	  }
	  else return(NULL);
      }
   }

   if (name[len] == '/') {	// Proceed with next groupname in path
       curnode = result;
       name += len +1;
	   name_len -= (len +1);
       len=find_path_del(name,name_len);

   } else 
   if (name[len] == 0) {		// eof, search completed 
       if (!found) {
	  		result = NULL;
       }
	   len = 0;
   }
   else {		// ?? something wrong 
     len=0;
	// ASSERT(1);
   }
   }

  return(result);
}

// Move node to Group target
int GGroup::Move(GGroup *target)
{
   owner->LinkOut(this);
   return( target->LinkIn(this));
}

// Add Activity, and propagate to owners
int GGroup::Propagate(unsigned int Activity)
{
	AddActivity(Activity);

	if (owner != NULL) owner->Propagate(Activity);
	return(0);
}

// Notify Group that action has/(will ?) bee performed
int GGroup::Notify(GAction action, GGroup *n) 
{
	if (n->HasGeometry() || n->HasSubGroups()) // HasNonEmptySubGroups()
	    Propagate(GRedrawSome);
	return(0);
}

int GGroup::Notify(GAction action, GGeometry *n)
{
	Propagate(GRedrawSome);
	return(0);

}


#if 0
//  IO
int GGroup::io( AStream &a)
{
	if (a.isOutput()) {
		a.nl();
		a.Sep('(');
			a.TabIn();
			a.io("Group");
            a.iostr(name);
			a.Sep('(');

		    for (GAttribute *n= attributes; n != NULL; n=n->next) { n->io(a); }

   			for (GGeometry *g= geometry; g != NULL; g=g->next) { g->io(a); }

   			for (GGroup *s= subgroups; s != NULL; s=s->next) { s->io(a); }
			a.Sep(')');

			a.TabOut();
		a.Sep(')');

	}
	else {
			// to do 
	}
	return a.stat;

}
#endif
//  IO

int GGroup::io( AStream &a)
{
	if (a.isOutput()) {
		a.nl();
			a.io("Group");
            a.iostr(name);

			a.BeginBlock();

		    for (GAttribute *n= attributes; n != NULL; n=n->next) { n->io(a); }

   			for (GGeometry *g= geometry; g != NULL; g=g->next) { g->io(a); }

   			for (GGroup *s= subgroups; s != NULL; s=s->next) { s->io(a); }

			a.EndBlock();

	}
	else {
			// to do 
	}
	return a.stat;

}

// ***************************************************************************
//
//  class GAttribute
//
RTIMP(GAttribute,0,GBaseNode);

int GAttribute::IsAttribute()  { return(1); }

int GAttribute::Set(GAttribute *a)
{ 
  return(0); 
}

// do attribute if mode = 0, undo it if mode = 1
int GAttribute::Traverse(GTraversal &state,int mode)
{
   if (mode == 0) state.Do(this);
   else state.EndDo(this);
   return(1);
}


// ***************************************************************************


// ***************************************************************************
//
//  class GGeometry
//

RTIMP(GGeometry,0,GAttributeNode);

int GGeometry::IsGeometry()  { return(1); }

// Move node to Group target
int GGeometry::Move(GGroup *target)
{
   owner->LinkOut(this);
   return( target->LinkIn(this));
}

int GGeometry::Traverse(GTraversal &state)
{
	int ret;	
	if (HasAttributes()) {  // process Attributes
	    state.Push();
			GAttributeNode::Traverse(state,0);
			ret =state.Do(this);
		state.Pop();
	}
	else
		ret =state.Do(this);
	return(ret);
}

// ***************************************************************************
//
// class GInstance
//
RTIMP(GInstance,0,GGeometry);

int GInstance :: Traverse(GTraversal &state)
{
   return(target -> Traverse(state));
}

// IO
int GInstance :: io(AStream &a)
{
	if (a.isOutput()) {
		a.Sep('(');
		a.io("Include");
		// target.GetPathName(path); a.io(path);
		// to do
		a.Sep(')');

	}
	else {
	  // to do
	}
	return(a.stat);
}

/*
void GInstanceGeometry :: Traverse(GTraversal &state)
{
   target -> TraverseGeometry(state);
}
*/

// ***************************************************************************
//
// class GStyle
//
RTIMP(GStyle,0,GAttribute);

int GStyle::Traverse(GTraversal &state,int mode)
{
   return (target->TraverseAttributes(state,mode));
}

// IO
int GStyle::io(AStream &a)
{
	if (a.isOutput()) {
		a.Sep('(');
		a.io("Style");
		// target.GetPathName(path); a.io(path);
		// to do
		a.Sep(')');

	}
	else {
	  // to do
	}
	return(a.stat);
}
#endif


// *************************************************************************
//

//static

#ifdef WIN32

CRITICAL_SECTION GStackableAttribute::section;

#endif

RTIMP_ABSTRACT(GStackableAttribute,0,RTRoot);

// class GMatrixStack

GSTACKABLE_ATTRIBUTE_IMP(GMatrixStack,GStackableAttribute);


GSTACKABLE_DERIVED_ATTRIBUTE_IMP(GglMatrixStack,GMatrixStack);

void GglMatrixStack::Set(GTraversal &state) 
{
#ifdef _OGL

  GglRenderTraversal *s = (GglRenderTraversal *)&state;
  
  s->glState->modelMatrix.glSet();  

  glMultMatrixf(&m.x[0][0]);

  // old glLoadMatrixf(&m.x[0][0]);

// GglRenderTraversal *s = (GglRenderTraversal *)&state;
// s->glState->modelMatrix.Set(m);
// s->glState->modelMatrix.glSet();
#endif

#ifdef _D3D
  GglRenderTraversal *s = (GglRenderTraversal *)&state;
  s->glState->SetWorldMatrix(m);  
#endif

// s->glState->modelMatrix.Trace();
 //TRACE("%s::set\n",this->ClassName());
}


// *************************************************************************
//
// class GTextureMatrixStack

GSTACKABLE_ATTRIBUTE_IMP(GTextureMatrixStack,GStackableAttribute);


GSTACKABLE_DERIVED_ATTRIBUTE_IMP(GglTextureMatrixStack,GTextureMatrixStack);

void GglTextureMatrixStack::Set(GTraversal &state) 
{
 GglRenderTraversal *s = (GglRenderTraversal *)&state;
/*
#ifdef _OGL
 glMatrixMode(GL_TEXTURE);
 s->glState->textureMatrix.Set(m);
 s->glState->textureMatrix.glSet();
 glMatrixMode(GL_MODELVIEW);
#endif
*/

//#ifdef _D3D

 s->glState->SetTextureMatrix(m,mVersion);

 //#endif
 //TRACE("%s::set\n",this->ClassName());
}


#ifdef _GFULL

// *************************************************************************
//
// class GMatrix
//
RTIMP(GMatrix,0,GAttribute);



int GMatrix::Set(GAttribute *a) 
{ if (a->id  == id) {
  	Set(((GMatrix *) a) -> m); 
	return(1);
  }
  return(0);
}

int GMatrix::Do(GTraversal &state)   
{ 

	GMatrixStack *current,*prev;
	
	// get current MatrixStack
	prev = (GMatrixStack*) state.Get(GMatrixStack::attrId);

	// TRACE("%s push %s \n",state.ClassName(),this->ClassName());
	
	current = (GMatrixStack*) prev->NewInstance();
	current->m = prev->m * m;
	state.Push(current);
	return(1);
}

#if 0
int GMatrix::Do(GglRenderTraversal &state)   
{ 

		//TRACE("%s push %s \n",state.ClassName(),this->ClassName());
		state.matrixStack.Push(state.glState->modelMatrix.Save());
		state.glState->modelMatrix.Set(m);

		state.glState->modelMatrix.glMult();

		state.glState->modelMatrix.glGet();
		state.glState->modelMatrix.Trace();
		return(1);
	};

	
int GMatrix::EndDo(GglRenderTraversal &state)   { 
		//TRACE("%s pop %s \n",state.ClassName(),this->ClassName());
		
		glMatrix *m=state.matrixStack.Pop();

		state.glState->modelMatrix = *m;

		state.glState->modelMatrix.glSet();

 		state.glState->modelMatrix.glGet();
		state.glState->modelMatrix.Trace();
 		delete m;
 		return(1);
	};	                                           
#endif
 	 
  
int GMatrix::io(AStream &a)
{
	a.nl();
	a.iotok("Matrix");
	m.io(a);
	return(a.stat);

}
#endif

// *************************************************************************
//
// class GFaceColorStack

GSTACKABLE_ATTRIBUTE_IMP(GFaceColorStack,GStackableAttribute);


GSTACKABLE_DERIVED_ATTRIBUTE_IMP(GglFaceColorStack,GFaceColorStack);

void GglFaceColorStack::Set(GTraversal &state) 
{
 GglRenderTraversal *s = (GglRenderTraversal *)&state;
#ifdef _OGL
 s->glState->faceColor.Set(color);
 s->glState->SetColor(s->glState->faceColor); // test 
 s->glState->faces.useMaterialColor = 0;
#endif
 //TRACE("%s::set\n",this->ClassName());
}

// *************************************************************************
#ifdef _GFULL

//
// class GFaceColor
//
RTIMP(GFaceColor,0,GAttribute);

int GFaceColor::Do(GTraversal &state)   
{ 

	//TRACE("%s push %s \n",state.ClassName(),this->ClassName());
	GFaceColorStack *current,*prev;
	
	// get current
	prev = (GFaceColorStack*) state.Get(GFaceColorStack::attrId);
	if (!prev) {
		// ignore it, because not registerd (initialized)
		// TRACE("Attribute not registered \n");
		return(0);
	 }
	
	current = (GFaceColorStack*) prev->NewInstance();
	current->color = color;
	state.Push(current);
	return(1);
}

int GFaceColor::io(AStream &a)
{
	a.nl();
	a.iotok("FaceColor");
	color.io(a);
	return(a.stat);
}


    
// * ************************************************************************
//	  
// class GBBox
//
RTIMP(GBBox,0,GAttribute);

// do attribute if mode = 0, undo it if mode = 1
int GBBox::Traverse(GGeometryTraversal &state,int mode)
{
/*
	 if (!mode) 
	 	state.PushModellingMatrix(m);
	 else state.PopModellingMatrix();
*/
   return(1);
}


int GBBox::io(AStream &a)
{
	a.Sep('(');
	a.iotok("Bbox");
	m.io(a);
	a.Sep(')');
	return(a.stat);

}
#endif


// *************************************************************************
//
// class GTimeStack

GSTACKABLE_ATTRIBUTE_IMP(GTimeStack,GStackableAttribute);


// *************************************************************************

#ifdef _GFULL

void G__cdecl GWorld::Error(const char * message,...)
{ 

   int ret;
   va_list args;

   va_start(args, message);	/* get variable arg pointer */
   printf("Error:");
   ret=vprintf(message,args);	/* format into buf with variable args */
   va_end(args);		/* finish the arglist */
   // to do trace::
   char buf[4000];
   ret=vsprintf(buf,message,args);	/* format into buf with variable args */
   TRACE("GWorld:Error:%s",buf);
}


int IsGroup(GKey key)
{
  return(key && key->IsGroup());
}

int IsGeometry(GKey key)
{
  return(key && key->IsGeometry());
}

GWorld::~GWorld() {
  	if (root) delete root; root = NULL;
  	if (objects) delete objects; objects = NULL;
  	if (styles) delete styles; styles = NULL;
	// Stack Autmatic
};


// Open a group given a Pathname
// if group dosn´t exist create a new one
// if pathname = "" create a unnamed group in the curreent group
// new group is also beeing pushed on current open group stack
void GWorld::Open(const char *PathName)
{  GGroup *s=NULL;

//   TRACE("Open %s \n",PathName);

   if (PathName[0]==0) {
       if (!current) Error ("No open group for creating new unnamed segment");
       else 
	   {
		 s= new GGroup();
		 current->LinkIn(s);
	   }
   }
   else 
   if (PathName[0] == '/') {
      s = root->FindSubGroup(PathName+1,-1,1);
   }
   else {
       if (!current) Error ("No open group for %s",PathName);
       else s= current->FindSubGroup(PathName,-1,1);
   }

   if (!s) Error("Can't open group %s",PathName);
   else {
      Push(s);
   }
}

void GWorld::Close()
{  GGroup *s=NULL;

   if (!current) Error ("No open group can't close","" );
   //TRACE("Close %s\n",current->name);
   Pop();

}


void GWorld::OpenByKey(GKey key)
{
   if (IsGroup(key)) {
       GGroup *s= (GGroup *) key;
//	   TRACE("OpenByKey %s \n",s->name);
       Push(s);
   } else {
      Error("OpenByKey:Unknown key type (key)","");
   }
}


void GWorld::DeleteByKey(GKey key)
{
   if (IsGroup(key)) {
       GGroup *s= (GGroup *) key;
       s->owner->LinkOut(s);
       delete s;
   }
   else if (IsGeometry(key)) {
       GGeometry *s= (GGeometry *) key;
       s->owner->Delete(s);
       delete s;
   } else {
      Error("Unknown key type in DeleteByKey (key)","");
   }
}

void GWorld::MoveByKey(GKey key,GGroup *target)
{
   if (key->IsGroup()) {
       GGroup *s= (GGroup *) key;
       s->Move(target);
   }
   else if (key->IsGeometry()) {
       GGeometry *s= (GGeometry *) key;
       s->Move(target);
   } else {
      Error("Unknown key type in DeleteByKey (key)","");
   }
}

// write World starting at root to file
int GWorld::Write(const char *PathName)

{ AStream a(PathName,"w");
  a.block_open = '{';
  a.block_close = '}';


  if (!a.atEof()) {
	a.comment("GLView Version 1.1 World");
    Root()->io(a);
  }
  return(a.stat);
}
#endif



// ************************************************************************
//
// class GStackableAttribute
//
//RTIMP_ABSTRACT(GStackableAttribute,0,RTRoot);
int GStackableAttribute::attrIdCounter = 0;	// counter is 0 at start, allocate starting with 1



// ************************************************************************
//
// class GTraversal
//
RTIMP(GTraversal,0,RTRoot);

/*
  default control :

   node.Traverse(state);
 		
   		state.Do(node);  << virtual

			node->Do(state);		// default action
			

*/
GTraversal::GTraversal()
{
	level = 0;
	maxLevel = 128;
	
	top=0;
	stop = 0;
	traverseAll = gfalse;
	traverseTransformHierarchyOnly=gfalse;
	notInTransformHierarchy = 0;

	inlineLevel = 0;

	maxInlineLevel = 32;

	inlineWwwFlags = 0;

	loadInThread = gtrue;

	loadInlines = DONT_LOAD;
	loadTextures = DONT_LOAD;
	loadSounds = DONT_LOAD;
	loadProtos = DONT_LOAD;

	pendingLoads = 0;

	attributesInitialized = 0;
	topPath = NULL;

	view = 0;
	myglView = 0;

	defaultAppearance = NULL;
	defaultMaterial = NULL;
	defaultTextureTransform = NULL;
	defaultMaterial10 = NULL;

	globalPointLights = gfalse;
	needShapes = gfalse;
	nodeDict=NULL; 

	ClearStaticAttributes();
}

GTraversal::~GTraversal()
{
   // remove attributes from stack
   while (top) Pop(top);
   
   // destroy top attributes
   for(int i=0; i<current.Length(); i++)
   		if (current[i]) { 
   			current[i]->FreeInstance(); 
   			current[i]=0; 
   		}
}

	// initialize initial attribute setting
void GTraversal::Set(GStackableAttribute* attr) 
{
			int id = attr->AttrId();
			attr->prev = 0;
			attr->level = 0;
			attr->prevThisAttr = 0;
			if (id == 0) {
				// change in GClass  x++ ==> ++x 
				attr->SetAttrId(id=1+GStackableAttribute::GetNewAttrId());
			}
			if (id >= current.Length()) {	// reallocate current[AttrId]
			   int oldLength = current.Length();
			   current.SetLength(id+1);
			   for(int i=oldLength; i<current.Length();i++) current[i]=0;
			}
			if (current[id] != 0) {
			  // Attribute type already declared (probably derived version)
			  // so ignore and destory attr
			  // TRACE("%s Attribute %s already registered as %s\n",this->ClassName(),attr->ClassName(),current[id]->ClassName());
			  attr->FreeInstance(); 
			}
			else current[id]= attr;
	};

int GTraversal::Do(GNode *n) 
{ 	
	//TRACE("Traverse not implemented for Traversal %s, Argument is %s \n",ClassName(),n->ClassName());
	//return(1);
	return(n->Do(*this));
}

#ifdef _GFULL
int GTraversal::Do(GGeometry *n) 
{ 	
	//TRACE("Traverse not implemented for Traversal %s, Argument is %s \n",ClassName(),n->ClassName());
	//return(1);
	return(n->Do(*this));
}

int GTraversal::Do(GGroup *n)
{
//	TRACE("Do Group not implemented for %s , Argument is %s \n",ClassName(),n->ClassName());
	// return 0 if Group should not be traversed further
	// i.e. if Group is inactive, ivisible ....
	// and/or stack some state, unstack it in EndDo
	Push();
	return(1);
}

int GTraversal::EndDo(GGroup *n)
{
	//TRACE("EndDo Group not implemented for %s, Argument is %s  \n",ClassName(),n->ClassName());
	Pop();
	return(1);
}

int GTraversal::Do(GAttribute *a)
{
//	TRACE("Do Attribute not implemented for %s, Argument is %s  \n",ClassName(),a->ClassName());
//	return(1);
	return(a->Do(*this));
}

int GTraversal::EndDo(GAttribute *a)
{
	TRACE("EndDo Attribute not implemented for %s, Argument is %s  \n",ClassName(),a->ClassName());
	return(1);
}
#endif


void GTraversal::InitializeAttributes()
{
  attributesInitialized = 1;
}

//
// class GGeometryTraversal
//
RTIMP(GGeometryTraversal,0,GTraversal);

void GGeometryTraversal::InitializeAttributes()
{
	GMatrixStack *m = GMatrixStack::NewElement();
	m->m.Identity();
	Set(m);

	GTextureMatrixStack *mt = GTextureMatrixStack::NewElement();
	mt->m.Identity();
	Set(mt);

}
// moved to GModel to setup parameters
#if 0
void GGeometryTraversal::SetCamera(GCamera *camera)
{

	this->camera = camera;

	viewLocalValid = 0;
/*	
	viewpoint = camera->viewpoint;
	Point dir = = camera->target - camera->position;
	Point right = camera->NormRight();
	Point up = camera-up;

	dir *= zfar / dir.Length();
	Point p = viewpoint +dir;
*/
/*
	viewVolume[0]=p+right+up;
	viewVolume[1]=p+right-up;
	viewVolume[2]=p-right+up;
	viewVolume[3]=p-right-up;
*/

}
#endif


//
// class GRenderTraversal
//
RTIMP(GRenderTraversal,0,GGeometryTraversal);


void GRenderTraversal::InitializeAttributes()
{
	GGeometryTraversal::InitializeAttributes();
	GFaceColorStack *c = GFaceColorStack::NewElement();
	c->color.Set(0.5,0.5,0.5);
	Set(c);

}

// fully process the transparent elements, and clean list at end
int GRenderTraversal::RenderTransparentElements()
{
	return -1;
}


//
// class GPathTraversal
//
RTIMP(GPathTraversal,0,GGeometryTraversal);

int GNode::Do(GPathTraversal &state) 
{
   return Do(*((GGeometryTraversal *) &state));		
}




//
// class GBBoxTraversal
//
RTIMP(GBBoxTraversal,0,GGeometryTraversal);

void GBBoxTraversal::InitializeAttributes()
{	
 	GGeometryTraversal::InitializeAttributes();
}

//
// class GglRenderTraversal
//
RTIMP(GglRenderTraversal,0,GRenderTraversal);


void GglRenderTraversal::InitializeAttributes()
{
	// overwrite standard attributes with glSpecific versions
	// which will have set functions keeping track of the glState
	GglMatrixStack *m = GglMatrixStack::NewElement();
	m->m.Identity();
	Set(m);

	GglTextureMatrixStack *mt = GglTextureMatrixStack::NewElement();
	mt->m.Identity();
	Set(mt);

    Point c;
#ifdef _OGL
	GFaceColorStack *fc = GFaceColorStack::NewElement();
	
	glState->faceColor.Get(c);

	if (glState->faces.useMaterialColor) {
		glState->material.diffuse.Get(c);
	}
	
	fc->color = c;
	Set(fc);

#endif

	GRenderTraversal::InitializeAttributes();
}


#ifdef _GFULL
// 
class GPrintTraversal : public GTraversal {

    // state for all attributes
public:
	int level;
	GPrintTraversal() { level = 0; }

	int Do(GGeometry *n);
	
	int Do(GGroup *n);
	int EndDo(GGroup *n);
	
	int Do(GAttribute *a);
	int EndDo(GAttribute *a);
};

int GPrintTraversal::Do(GGeometry *n) 
{ 	
	printf("Geometry %p Class = %s \n",n,n->ClassName());
	return(1);
}

int GPrintTraversal::Do(GGroup *n)
{
	printf("(Group ");
	if (n->name) printf("%s",n->name);
	printf("(%p) \n",n);
	return(1);
}

int GPrintTraversal::EndDo(GGroup *n)
{
	printf(")\n");
	return(1);
}


int GPrintTraversal::Do(GAttribute *a)
{
	printf("Attribute %p %s \n",a,a->ClassName());
	return(1);
}

int GPrintTraversal::EndDo(GAttribute *a)
{
	printf("Attribute pop %p %s \n",a,a->ClassName());
	return(1);
}

int test(int argc,char *argv[])
{
  GWorld w;
  
  w.Open("/test/1 /2 /3");
  	w.Open("a");
	w.Close();

  	w.Open("b");
	w.Close();

  	w.Open("/test/b");
	 w.current->Set(new GMatrix(TranslationMatrix(Point(10,0,0)))) ;	
	w.Close();

  	w.Open("/test/1");
	 w.current->Set(new GBBox()) ;	
	w.Close();

	// w.Apply(*(new GPrintTraversal()));

	{ AStream s(NULL,"w");
	  w.Root()->io(s);
	}

  w.Close();	
  return(0);
}
#endif


void GClassFreeAttributeClasses() 
{
	GMatrixStack::FreeClass();
	GglMatrixStack::FreeClass();
	GTextureMatrixStack::FreeClass();
	GglTextureMatrixStack::FreeClass();

	GMatrixStack::FreeClass();
	GglMatrixStack::FreeClass();

	GFaceColorStack::FreeClass();
	GglFaceColorStack::FreeClass();

	GTimeStack::FreeClass();

}

