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
#ifndef _GCameraEvent_h
#define _GCameraEvent_h
/******************************************************************************
@doc

@module GCameraEvent.h - GLView camera event handler classes|

Copyright (c) 1996	by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GCameraEventHandler>

Notes:

Changes:

Todo :

******************************************************************************/

#include "quaternation.h"


class GView;

class GvGroup;

//
// GCameraEventHandler 
// base class for EventHandlers
//

class GCameraEventHandler : public GEventHandler {
public:
	RT(GCameraEventHandler);


	GCameraEventHandler(GView *View =NULL) : GEventHandler(View) { }

	virtual ~GCameraEventHandler();

	// get reference to camera in effect for view 
	GCamera &GetCamera() { return view->GetGlobalCamera(); }

	// mark for redraw
	void Redraw() {view->Redraw(); }
	
	// display message
	void Message(const char* msg)  { /* view->Message(msg); */ }; 

	// nFlags are the OS status flags, point is window pixel position 
	virtual int OnLButtonDown(UINT nFlags, CPoint point);
	virtual int OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual int OnMouseMove(UINT nFlags, CPoint point);
	virtual int OnLButtonUp(UINT nFlags,  CPoint point);
	virtual int OnTimer();
	
	// handle a key, return false if key not handlerd 
	virutal BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);


	// execute move action, on time event 
	void DoMoveTimer();

	// do the mouse move, based on current mouse position in point 
	void DoMove(UINT nFlags, CPoint point);



};





#endif




