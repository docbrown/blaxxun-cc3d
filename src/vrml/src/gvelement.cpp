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
#include "stdafx.h"
/******************************************************************************
@doc

@module GvElement.cpp - VRML 1.0 / Inventor state attributes|

Copyright (c) 1996 - 1998 by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GvElement>
   ...
  
Notes:

Changes:

$Revision: 1.8 $
$Log: gvelement.cpp,v $
Revision 1.8  1999/07/06 16:45:40  tom
*** empty log message ***


Todo :

******************************************************************************/

#include <gvelement.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


GSTACKABLE_ATTRIBUTE_IMP(GvElement,GStackableAttribute);  //HG




#ifdef _G_INVENTOR

  GSTACKABLE_ATTRIBUTE_IMP(GvProfileCurveElement,GvElement);
  GSTACKABLE_ATTRIBUTE_IMP(GvProfileCoordElement,GvElement);
//  GSTACKABLE_ATTRIBUTE_IMP(GvCoordinate4Element,GvElement);

#endif _G_INVENTOR

// implementation all elements with specific stack indices
#ifdef _G_VRML1

GSTACKABLE_ATTRIBUTE_IMP(GvCoordinate3Element,GvElement);  
GSTACKABLE_ATTRIBUTE_IMP(GvMaterialElement,GvElement);  
GSTACKABLE_DERIVED_ATTRIBUTE_IMP(GvglMaterialElement,GvMaterialElement);  
GSTACKABLE_ATTRIBUTE_IMP(GvMaterialBindingElement,GvElement);  
GSTACKABLE_ATTRIBUTE_IMP(GvTextureCoordinateBindingElement,GvElement);  
GSTACKABLE_ATTRIBUTE_IMP(GvNormalBindingElement,GvElement);  
GSTACKABLE_ATTRIBUTE_IMP(GvNormalElement,GvElement);  
GSTACKABLE_ATTRIBUTE_IMP(GvShapeHintsElement,GvElement);  
GSTACKABLE_DERIVED_ATTRIBUTE_IMP(GvglShapeHintsElement,GvShapeHintsElement);  
GSTACKABLE_ATTRIBUTE_IMP(GvTexture2Element,GvElement);  
GSTACKABLE_DERIVED_ATTRIBUTE_IMP(GvglTexture2Element,GvTexture2Element);  
GSTACKABLE_ATTRIBUTE_IMP(GvTextureCoordinate2Element,GvElement);  
GSTACKABLE_ATTRIBUTE_IMP(GvComplexityElement,GvElement);  
GSTACKABLE_ATTRIBUTE_IMP(GvFontStyleElement,GvElement);  


//GSTACKABLE_ATTRIBUTE_IMP(GvWWWPathElement,GvElement);  

GSTACKABLE_ATTRIBUTE_IMP(GvLightModelElement,GvElement);  
GSTACKABLE_DERIVED_ATTRIBUTE_IMP(GvglLightModelElement,GvLightModelElement);  

#endif _G_VRML1

GSTACKABLE_ATTRIBUTE_IMP(GvLightElement,GvElement);  
GSTACKABLE_DERIVED_ATTRIBUTE_IMP(GvglLightElement,GvLightElement);  

GSTACKABLE_ATTRIBUTE_IMP(GvWWWAnchorElement,GvElement);  
GSTACKABLE_ATTRIBUTE_IMP(GvWWWInlineElement,GvElement);  





void GvElement::termClass() 
{


#ifdef _G_INVENTOR
   GvProfileCurveElement::FreeClass();  
   GvProfileCoordElement::FreeClass();  
//   GvCoordinate4Element::FreeClass();  
#endif _G_INVENTOR

#ifdef _G_VRML1
   GvCoordinate3Element::FreeClass();  
   GvMaterialElement::FreeClass();  
   GvglMaterialElement::FreeClass();
   GvMaterialBindingElement::FreeClass();  
   GvTextureCoordinateBindingElement::FreeClass();  
   GvNormalBindingElement::FreeClass();  
   GvNormalElement::FreeClass();  
   GvShapeHintsElement::FreeClass();  
   GvglShapeHintsElement::FreeClass();
   GvTexture2Element::FreeClass();  
   GvglTexture2Element::FreeClass(); 
   GvTextureCoordinate2Element::FreeClass();  
   GvComplexityElement::FreeClass();  
   GvFontStyleElement::FreeClass();  

//   GvWWWPathElement::FreeClass();  

   GvLightModelElement::FreeClass();  
   GvglLightModelElement::FreeClass();  
#endif

   GvLightElement::FreeClass();  
   GvglLightElement::FreeClass(); 

   GvWWWAnchorElement::FreeClass();  
   GvWWWInlineElement::FreeClass();  

}

