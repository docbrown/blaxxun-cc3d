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
#ifndef  _GV_ELEMENT_
#define  _GV_ELEMENT_

/******************************************************************************
@doc

@module GvElement.h - VRML 1.0 / Inventor state attributes|

Copyright (c) 1996 - 1998 by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GvElement>

  
Notes:

Changes:

$Revision: 1.7 $
$Log: gvelement.h,v $
Revision 1.7  1999/07/06 16:34:40  tom
*** empty log message ***


Todo :

******************************************************************************/


#include <gvbasic.h>


#include "rt.h"
#include "array.h"
#include "stack.h"
#include "point.h"
#include "matrix.h"
#include "gclass.h"
#include "gconfig.h"

class AStream;


class GvNode;	   
class GvBaseColor;
class GvMFColor;
class Point;


//
// GvElement
// 

class GvElement:
	public GStackableAttribute  // so it can be pushed and poped to state
{

  public:
    GSTACKABLE_ATTRIBUTE(GvElement);  //HG

    GvElement() : data(NULL), GStackableAttribute() 
    { 
    	// Presumably, the caller will set these
    }

	void Init() { data = NULL; GStackableAttribute::Init(); }

    GvNode	*data;		// Pointer to node containing data

	void setData(GvNode	*data) { this->data = data;} 
	GvNode	* getData() const  { return data; } 

	static void termClass(); // cleanup free lists  


};

// helper macro to easily get VRML1/Inventor state attributes
#define G_GETSTATE(state,varName,className, stackElement) \
	className *varName; \
	{ GvElement *top=  \
		 (GvElement*) state.Get(stackElement::attrId); \
	  if (top) varName= (className *) top->data; else varName = 0; } 

// e.g. in a Do action 
//	G_GETSTATE(state,shapeHints,GvShapeHints,	GvShapeHintsElement)



// contains the current WWWAnchor, Anchor or TouchSensor node
// needed for VRML 2.0
class GvWWWAnchorElement : public GvElement
{
  public:
    GSTACKABLE_ATTRIBUTE(GvWWWAnchorElement);  
	GvWWWAnchorElement() : parent(NULL) {};
	GvNode *parent;

	void Init() { parent = NULL; GvElement::Init(); }

};





//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// 
//		open inventor trimmed nurbs specific extensions
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/




#ifdef _G_INVENTOR


class GvProfileCurveElement : public GvElement
{
  public:
   GSTACKABLE_ATTRIBUTE(GvProfileCurveElement);  
   GvProfileCurveElement() {};
};


//
//coords
//
class GvProfileCoordElement : public GvElement
{
  public:
   GSTACKABLE_ATTRIBUTE(GvProfileCoordElement);  
   GvProfileCoordElement() {};
};

/*
class GvCoordinate4Element : public GvElement
{
  public:
   GSTACKABLE_ATTRIBUTE(GvCoordinate4Element);  
   GvCoordinate4Element() {};
};
*/
#endif



//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// 
//							VRML 1					
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/




#ifdef _G_VRML1

class GvCoordinate3Element : public GvElement
{
  public:
   GSTACKABLE_ATTRIBUTE(GvCoordinate3Element);  
   GvCoordinate3Element() {};


};

class GvMaterialElement : public GvElement
{
  public:
    GSTACKABLE_ATTRIBUTE(GvMaterialElement);  
	
	GvMaterialElement() { baseColor = NULL; }
	void Init() { baseColor = NULL; GvElement::Init(); }

	GvBaseColor *baseColor;		// the current base color if any



	// get current diffuse color
	GvMFColor* GetDiffuse(); // in GvTraverse 
	
	int GetLocalColors(PointArray &colors,int period=0);
	

	// get current diffuse color
	int  GetDiffuse(int i,Point &color); 
};


class GvglMaterialElement : public GvMaterialElement
{
  public:
    GSTACKABLE_DERIVED_ATTRIBUTE(GvglMaterialElement);  
	
	GvglMaterialElement() : GvMaterialElement() {};

	void Set(GTraversal &state);

};


class GvMaterialBindingElement : public GvElement
{
  public:
    GSTACKABLE_ATTRIBUTE(GvMaterialBindingElement);  
	GvMaterialBindingElement() {};


};

class GvTextureCoordinateBindingElement : public GvElement
{
  public:
    GSTACKABLE_ATTRIBUTE(GvTextureCoordinateBindingElement);  
	GvTextureCoordinateBindingElement() {};

};

class GvNormalBindingElement : public GvElement
{
  public:
    GSTACKABLE_ATTRIBUTE(GvNormalBindingElement);  
	GvNormalBindingElement() {};

};


class GvNormalElement : public GvElement
{
  public:
    GSTACKABLE_ATTRIBUTE(GvNormalElement);  
	GvNormalElement() {};

};

class GvShapeHintsElement : public GvElement
{
  public:
    GSTACKABLE_ATTRIBUTE(GvShapeHintsElement);  
	GvShapeHintsElement() {};

	void Set(GTraversal &state);

};

class GvglShapeHintsElement : public GvShapeHintsElement
{
  public:
    GSTACKABLE_DERIVED_ATTRIBUTE(GvglShapeHintsElement);  
	
	GvglShapeHintsElement() : GvShapeHintsElement() {};

	void Set(GTraversal &state);

};


class GvTexture2Element : public GvElement
{
  public:
    GSTACKABLE_ATTRIBUTE(GvTexture2Element);  
	GvTexture2Element() {};
};

class GvglTexture2Element : public GvTexture2Element
{
  public:
    GSTACKABLE_DERIVED_ATTRIBUTE(GvglTexture2Element);  
	
	GvglTexture2Element() : GvTexture2Element() {};

	void Set(GTraversal &state);

};


class GvTextureCoordinate2Element : public GvElement
{
  public:
    GSTACKABLE_ATTRIBUTE(GvTextureCoordinate2Element);  
	GvTextureCoordinate2Element() {};

};

class GvComplexityElement : public GvElement
{
  public:
    GSTACKABLE_ATTRIBUTE(GvComplexityElement);  
	GvComplexityElement() {};

};

class GvFontStyleElement : public GvElement
{
  public:
    GSTACKABLE_ATTRIBUTE(GvFontStyleElement);  
	GvFontStyleElement() {};

};


class GvLightModelElement : public GvElement
{
  public:
    GSTACKABLE_ATTRIBUTE(GvLightModelElement);  
};

class GvglLightModelElement : public GvLightModelElement
{
  public:
    GSTACKABLE_DERIVED_ATTRIBUTE(GvglLightModelElement);  
	
	GvglLightModelElement() {};

	void Set(GTraversal &state);
};


#if 0
// store the absolut url / pathname for nested includes
class GvWWWPathElement : public GvElement
{
  public:
    GSTACKABLE_ATTRIBUTE(GvWWWPathElement);  
	GvWWWPathElement() {};
};
#endif

#endif


//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// 
//						
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/




class GvLightElement : public GvElement
{
  public:
    GSTACKABLE_ATTRIBUTE(GvLightElement);  
	
	GvLightElement() { index = -1; }
	void Init() { index = -1; GvElement::Init(); }

	int index;		// light index 

};

class GvglLightElement : public GvLightElement
{
  public:
    GSTACKABLE_DERIVED_ATTRIBUTE(GvglLightElement);  
	
	GvglLightElement() {};
	void Init() {  GvLightElement::Init(); }

	void Set(GTraversal &state);
};


// reference to inlines to retreive absolute URL´s
// contains a WWWInline or Inline node 
class GvWWWInlineElement : public GvElement
{
  public:
    GSTACKABLE_ATTRIBUTE(GvWWWInlineElement);  
	GvWWWInlineElement() {};
};




#endif /* _GV_ELEMENT_ */
