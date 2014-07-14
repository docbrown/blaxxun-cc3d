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

// GglTransparencyRenderTraversal.h
#ifndef _GglTransparencyRenderTraversal_H
#define _GglTransparencyRenderTraversal_H


//
//	GglTransparencyRenderTraversal
//
// traverse graph to do selection for all geometry nodes
class GglTransparencyRenderTraversal : public GGvglRenderTraversal 
{
public :
	RT(GglTransparencyRenderTraversal);

	GglTransparencyRenderTraversal(RenderState * GlState=0) : GGvglRenderTraversal(GlState) 
		{  
			method = DELAYED_BLEND; 
			numTransparentElements = 0;
		}
	
	~GglTransparencyRenderTraversal();
	

	enum Method {
		DELAYED_BLEND,
		SORTED_BBOX_CENTER_BLEND,
		SORTED_BLEND,
	};

	Method method;
	
	enum Action {
		RENDER_ALL_OBJECTS,				// render all objects independent of transparency
		RENDER_OPAQUE_OBJECTS,			// render only opaque objects
		RENDER_TRANSPARENT_OBJECTS,		// render only transparent	
		RENDER_OPAQUE_OBJECTS_COLLECT_TRANSPARENT, // render opaque and collect transparent
	};
	
	Action action;
	
	int numTransparentElements;
	
	Array<GTransparentElement*> elements;

	// add element to transparent list
	int AddTransparentElement(GTransparentElement *element);

	// delete all elements
	int DeleteTransparentElements();

	// sort & render the transparent elements
	int DoTransparentElements(GglTransparencyRenderTraversal &state);

    // fully process the transparent elements, and clean list at end
	virtual int RenderTransparentElements();

	
	int Do(GNode *n) {	return (n->Do(*this)); }
	
   	void InitializeAttributes(); 

};



#endif

