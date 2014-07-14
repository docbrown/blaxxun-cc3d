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


#include <gvwwwanchor.h>

#include <gvelement.h>

#include "gvtraverse.h"

#ifdef _G_VRML1

//
// GvWWWAnchor
//

GV_NODE_SOURCE_IMP(GvWWWAnchor,GvSeparator);

GvWWWAnchor::GvWWWAnchor()
{
    GV_NODE_CONSTRUCTOR(GvWWWAnchor);
    isBuiltIn = TRUE;

	// from separator
	GV_NODE_ADD_FIELD(renderCulling);

    renderCulling.value = AUTO;

    GV_NODE_DEFINE_ENUM_VALUE(CullEnabled, ON);
    GV_NODE_DEFINE_ENUM_VALUE(CullEnabled, OFF);
    GV_NODE_DEFINE_ENUM_VALUE(CullEnabled, AUTO);

    GV_NODE_SET_SF_ENUM_TYPE(renderCulling, CullEnabled);
	//


    GV_NODE_ADD_FIELD(name);
    GV_NODE_ADD_FIELD(description);
    GV_NODE_ADD_FIELD(map);

    GV_NODE_ADD_FIELD(target); // target frame ?? Live3d hack ? "topFrame"
    GV_NODE_ADD_FIELD(mode); //	OFF

    name.value = "";
    description.value = "";
    map.value = NONE;
    target.value = "";
    mode.value = "";

    GV_NODE_DEFINE_ENUM_VALUE(Map, NONE);
    GV_NODE_DEFINE_ENUM_VALUE(Map, POINT);

    GV_NODE_SET_SF_ENUM_TYPE(map, Map);

	homeUrlInit = FALSE;
}

GvWWWAnchor::~GvWWWAnchor()
{
}


int GvWWWAnchor::Do(GTraversal &state)
{

    state.Push();

	// to do:any GvSeparator culling 

    // put WWWAnchor node in state to resolve anchor picks (deepest first)
	GvWWWAnchorElement *current,*prev;
	prev = (GvWWWAnchorElement*) state.Get(GvWWWAnchorElement::attrId);
	if (prev) {
		current = (GvWWWAnchorElement *) prev->NewInstance();								
		current->data = this;
		state.Push(GvWWWAnchorElement::attrId,current);			
		if (!homeUrlInit) { // get the URL of current scene / inline 
			homeUrl = state.GetHomeUrl();
			homeUrlInit = TRUE;
		}
	}

	int ret = TraverseChildren(state);

	state.Pop();
	return(ret);
}

int GvWWWAnchor::Do(GLoadInlinesTraversal &state)
{

	if (state.resolveRelativeUrls && !homeUrlInit) {
		homeUrl = state.GetHomeUrl();
		homeUrlInit = TRUE;
	}

    state.Push();

	// to do:any GvSeparator culling 

    // put WWWAnchor node in state to resolve anchor picks (deepest first)
	GvWWWAnchorElement *current,*prev;
	prev = (GvWWWAnchorElement*) state.Get(GvWWWAnchorElement::attrId);
	if (prev) {
		current = (GvWWWAnchorElement *) prev->NewInstance();								
		current->data = this;
		state.Push(GvWWWAnchorElement::attrId,current);			
		if (!homeUrlInit) { // get the URL of current scene / inline 
			homeUrl = state.GetHomeUrl();
			homeUrlInit = TRUE;
		}
	}

	int ret = TraverseChildren(state);

	state.Pop();
	return(ret);
}


int GvWWWAnchor:: Do(GPathTraversal &state)
{	int ret = 0;
    state.Push();

	GPUSH_PATH(state,this);

      // put WWWAnchor node in state to resolve anchor picks (deepest first)
	  GvWWWAnchorElement *current,*prev;
	  prev = (GvWWWAnchorElement*) state.Get(GvWWWAnchorElement::attrId);
	  if (prev) {
		current = (GvWWWAnchorElement *) prev->NewInstance();								
		current->data = this;
		state.Push(GvWWWAnchorElement::attrId,current);			
		if (!homeUrlInit) { // get the URL of current scene / inline 
			homeUrl = state.GetHomeUrl();
			homeUrlInit = TRUE;
		}
	  }

	  ret = TraverseChildren(state);
	GPOP_PATH(state);
	
	state.Pop();
	return(ret);
}


int GvWWWAnchor::Do(GglRenderTraversal &state)
{
	return GvSeparator::Do(state);
}


//
// GvWWWAnchor
//

GV_NODE_SOURCE_IMP(GvWWWRegionAnchor,GvWWWAnchor);

GvWWWRegionAnchor::GvWWWRegionAnchor()
{
    GV_NODE_CONSTRUCTOR(GvWWWRegionAnchor);
    isBuiltIn = TRUE;

	// from separator
	GV_NODE_ADD_FIELD(renderCulling);

    renderCulling.value = AUTO;

    GV_NODE_DEFINE_ENUM_VALUE(CullEnabled, ON);
    GV_NODE_DEFINE_ENUM_VALUE(CullEnabled, OFF);
    GV_NODE_DEFINE_ENUM_VALUE(CullEnabled, AUTO);

    GV_NODE_SET_SF_ENUM_TYPE(renderCulling, CullEnabled);
	//  from WWWAnchor


    GV_NODE_ADD_FIELD(name);
    GV_NODE_ADD_FIELD(description);
    GV_NODE_ADD_FIELD(map);

    name.value = "";
    description.value = "";
    map.value = NONE;

    GV_NODE_DEFINE_ENUM_VALUE(Map, NONE);
    GV_NODE_DEFINE_ENUM_VALUE(Map, POINT);

    GV_NODE_SET_SF_ENUM_TYPE(map, Map);
	//
    GV_NODE_ADD_FIELD(bboxSize);
    GV_NODE_ADD_FIELD(bboxCenter);

    name.value = "";
    bboxSize.value[0] = bboxSize.value[1] = bboxSize.value[2] = 0.0;
    bboxCenter.value[0] = bboxCenter.value[1] = bboxCenter.value[2] = 0.0;
	loaded = 0;
	cannotLoad = 0;

}

GvWWWRegionAnchor::~GvWWWRegionAnchor()
{
}

#endif _G_VRML1



//
// Gv2Anchor
//

GV_NODE_SOURCE_IMP(Gv2Anchor,Gv2Group);

Gv2Anchor::Gv2Anchor() : parameter(0),url(0)
{
    GV_NODE_CONSTRUCTOR(Gv2Anchor);
    isBuiltIn = TRUE;

	// from Gv2group
	GV_NODE_ADD_FIELD(bboxCenter);
	GV_NODE_ADD_FIELD(bboxSize);

    GV_NODE_ADD_FIELD(description);
	GV_NODE_ADD_FIELD(parameter);
	GV_NODE_ADD_FIELD(url);

    // from Gv2group
	GV_NODE_ADD_FIELD(children);
    GV_NODE_ADD_EVENT_IN(addChildren,GvMFNode);
    GV_NODE_ADD_EVENT_IN(removeChildren,GvMFNode);

	homeUrlInit = FALSE;

}

Gv2Anchor::~Gv2Anchor()
{
}


int Gv2Anchor::Do(GTraversal &state)
{
    // put Anchor node in state to resolve anchor picks (deepest first)
    state.Push();

	GvWWWAnchorElement *current,*prev;
	prev = (GvWWWAnchorElement*) state.Get(GvWWWAnchorElement::attrId);
	if (!prev) {
	} else {
		current = (GvWWWAnchorElement *) prev->NewInstance();								
		current->data = this;
		state.Push(GvWWWAnchorElement::attrId,current);			
	}

		int ret = children.Traverse(state);

	state.Pop();
	return(ret);
}

int Gv2Anchor::Do(GPathTraversal &state)
{
    // put Anchor node in state to resolve anchor picks (deepest first)
    state.Push();

	GvWWWAnchorElement *current,*prev;
	prev = (GvWWWAnchorElement*) state.Get(GvWWWAnchorElement::attrId);
	if (!prev) {
	} else {
		current = (GvWWWAnchorElement *) prev->NewInstance();								
		current->data = this;
		state.Push(GvWWWAnchorElement::attrId,current);			
	}

		int ret = children.Traverse(state);

	state.Pop();
	return(ret);
}

int Gv2Anchor::Do(GLoadInlinesTraversal &state)
{

	if (state.resolveRelativeUrls && !homeUrlInit) {
		homeUrl = state.GetHomeUrl();
		homeUrlInit = TRUE;
	}

    // put Anchor node in state to resolve anchor picks (deepest first)
    state.Push();

	GvWWWAnchorElement *current,*prev;
	prev = (GvWWWAnchorElement*) state.Get(GvWWWAnchorElement::attrId);
	if (!prev) {
	} else {
		current = (GvWWWAnchorElement *) prev->NewInstance();								
		current->data = this;
		state.Push(GvWWWAnchorElement::attrId,current);			
	}

		int ret = children.Traverse(state);

	state.Pop();
	return(ret);

}



int Gv2Anchor::Do(GglRenderTraversal &state)
{
	// no need to push anchor  && do bbox check
	return(Gv2Group::Do(state));
}


