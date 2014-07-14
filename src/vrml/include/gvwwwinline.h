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
#ifndef  _GV_WWW_INLINE_
#define  _GV_WWW_INLINE_

/******************************************************************************
@doc

@module GvWWWInline.h - VRML Inline nodes|

Copyright (c) 1996 - 1998 by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GvWWWInline>
<c Gv2Inline>
<c Gv2CreateVrmlFromUrl>

  
Notes:

Changes:

$Revision: 1.17 $
$Log: gvwwwinline.h,v $
Revision 1.17  1999/07/06 16:36:02  tom
*** empty log message ***


Todo :

******************************************************************************/


#include <gvsfvec3f.h>
#include <gvmfstring.h>

#include <gv2group.h>

#include <gvscene.h>

class GFile;
class GvMediaThread;


#include "www.h"


#ifdef _G_VRML1

#include <gvsfenum.h>
#include <gvgroup.h>


///WWWInline
// WWWInlines are also pushed onto state stack, for resolving relative inlines

class GvWWWInline : public GvGroup {

    GV_NODE_HEADER(GvWWWInline);

  public:
    // Fields
    GvSFString		name;		// URL name
    GvSFVec3f		bboxSize;	// Size of 3D bounding box
    GvSFVec3f		bboxCenter;	// Center of 3D bounding box

	
	// EventOut
	GvSFBool		isLoaded;		// FALSE,
	// TRUE, url is loaded, FALSE no url could be loaded 

	GFileRef        loader;			// url loader

	// is the bounding box empty ????
	int bboxIsEmpty() { return ((bboxSize[0]<=0.0) && (bboxSize[1]<=0.0) && (bboxSize[2]<=0.0)); }

	//void Traverse(GTraversal &state);

	// pushes WWWInlineElement in stack to resolve relative urls
	int TraverseChildren(GTraversal &state);  	

	int Do(GTraversal &state);
	int Do(GBBoxTraversal &state);

	// check if inline can be visible with respect to camera
	int CanBeCulled(GglRenderTraversal &state);
	int Do(GglRenderTraversal &state);
	int Do(GRaySelectTraversal &state); // implemented in GRaySelect.cpp

	int Do(GLoadInlinesTraversal &state);
	int Do(GioTraversal &state);
	int Do(GDisplayTreeTraversal &state); // implemented in GDisplayTree.cpp

	int bboxEmpty;

/*	
	int loaded;		// inlines has been retreived, children are containing the inline data
	int cannotLoad;	// inline has been tried to load, but there was an error
	GFile *loader;	// loader object / thread

	int LoaderBusy() { return(!loaded && loader != NULL); }
    
    // private field
    GvSFString		nameHome;		// URL home name
*/

	// get absolute home url
	const char *GetHomeUrl() const;

	// try to load url / file 
	int Load(GTraversal &state); 

	// a field has been changed
	int OnFieldChanged(GvField *field);

	// something changed 
	int OnChanged();

};

#endif


//
// VRML 2.0 Inline
//
class Gv2Inline : public GvScene {

    GV_NODE_HEADER(Gv2Inline);

public:
    // Fields :
	// from group 
    // bboxCenter
    // bboxSize
    GvMFString		url;            // []

	// EventIn
    
	//GvSFBool     set_unload
    void set_unload(GvSFBool *mode);

	// EventOut
	GvSFBool		isLoaded;		// FALSE,
	// TRUE, url is loaded, FALSE no url could be loaded 
	
	GvBool			isRegistered;	//!< TRUE if node registered in Scene information

	GFileRef        loader;			//!< url loader

	GvMediaThread	*parser;		//!< parser thread

    // children from Scene 

	double			lastRenderTime; //!< time this inline was rendered


	//! get absolute home url
	const char * GetHomeUrl() const;

	//! get the absolute url
	const char * GetUrl() const;

	//! get the caching check time for children 
	CTime GetChildrenRefreshTime() const;



	//! try to load url
	int Load(GTraversal &state);
	
	//! unload the inline
	int Unload();
	
	//! prepare scene graph, instance protos load externprotos
	int PrepareChildren(GView *view);

	// pushes WWWInlineElement in stack to resolve relative urls
	int TraverseChildren(GTraversal &state); 
	int TraverseChildren(GPathTraversal &state);
	

	int Do(GTraversal &state);

	int Do(GPathTraversal &state);

	int Do(GBBoxTraversal &state);

	int Do(GglRenderTraversal &state);

	int Do(GLoadInlinesTraversal &state);
	int Do(GioTraversal &state);
	int Do(GDisplayTreeTraversal &state); // implemented in GDisplayTree.cpp
	int Do(GOptimizeTraversal &state);	// implemented in GOptimizeTraversal.cpp


	// a field has been changed
	int OnFieldChanged(GvField *field);

};

//
// VRML 2.0 CreateVrmlFromUrl Pseudo node 
//
class Gv2CreateVrmlFromUrl : public Gv2Inline {

    GV_NODE_HEADER(Gv2CreateVrmlFromUrl);

public:
    // Fields :

	GvSFNode	node;	// node with addChildren event
	GvSFInt32   nodeEvent; // event of node to be called 

	// check the Loading state for a completed file task
	// return TRUE if done 
	GvBool CheckForLoad(GFile *file);
	
	// send the event out 
	GvBool SendEvent();


};



#endif /* _GV_WWW_INLINE_ */
