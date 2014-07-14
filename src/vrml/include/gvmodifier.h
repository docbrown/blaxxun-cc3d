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
#ifndef _GvModifier_h
#define _GvModifier_h

#include <gvnode.h>
#include <gvelement.h>



class GvModifier
{

public :

   virtual GvNode *Apply(GTraversal &state, GvNode * node);

};

class GvModifierElement : public GvElement
{
  public:
   GSTACKABLE_ATTRIBUTE(GvModifierElement);  
	GvModifierElement() {};

   GvModifier *modifier;   // the modifier part of the node
   GvNode *node;  // the node representing the modifier

   // matrix m;   // matrix at the time the modifier was pushed
   float t;       // the animation time, at the place the modifier was pushed



};


// Apply all Modifier in turn

#define ApplyModifiers(state,node) \
	GvModifierElement  *e = (GvModifierElement) state.Get(GvModifierElement::attrId); \
   for (; e != NULL; e = e->prevThisAttr) { \
      node = e->Apply(state,node); \
   } \

#endif

