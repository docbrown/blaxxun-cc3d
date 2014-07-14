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

@module GEvent.cpp - GLView camera event handler classes implementation|

Copyright (c) 1996	by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GCameraEventHandler>

Notes:

Changes:

Todo :

******************************************************************************/
#include "stdafx.h"


#include "gcamera.h"
#include "quaternation.h"

#include "grender.h"
#include "greporter.h"

#include "gmodel.h"

#ifdef _GV
// VRML 
#include <gvscene.h>

#include <gvtraverse.h>


#endif

#include "grayselect.h"
#include "gfindnode.h"


#include "gevent.h"
#include "gcameraevent.h"

//
// GEventHandler 
//
RTIMP(GCameraEventHandler,0,GEventHandler);

GCameraEventHandler::~GCameraEventHandler()
{
}

int GCameraEventHandler::OnLButtonDown(UINT nFlags, Point &point)
{
	TRACE("%s:OnLButtonDown (%f %f %f) %x \n",this->ClassName(),point.x,point.y,point.z,nFlags);
	firstPoint = point;
	firstFlags = nFlags;

	m_currentMode = m_lButtonMode;

	if (m_moveRepeat) { 
//		float cycle = max(LastFrameTime,m_moveRepeatCycleTime);
		float cycle = m_moveRepeatCycleTime;
		m_moveTimerId = SetTimer(2, int (cycle * 1000), NULL);
	}


	lastWasTimer = 0;
	lbutton=1;
	lbuttonP= point;
	buttonFlags = nFlags;


	Mode = m_currentMode; // mode is now the current (viewer) mode


	if (TimerId !=0 ) {
	   KillTimer(TimerId);
	   TimerId = 0;
	}

	// disable the auto animate feature 
	if (Animating) SetAnimateOff();

	if (Mode == LOOKAT_OBJECT_MODE) {
        if (view->Select(point.x,point.y,nFlags,GView::JUMP_OBJECT))
			// camera changed
			Mode = NULL_MODE;
			m_currentMode = NULL_MODE;

            SetMode(GetCameraMode());
			Redraw();
	    return;	
	}
    if (!view->GetFullScreen()) {
	switch (Mode) {
	case NULL_MODE: Message("Hold and drag to activate sensors"); break;
	case ORBIT_MODE: Message("Hold and drag to orbit around the object, SHIFT Rolls, CONTROL zooms"); break;
	case WALK_MODE: Message("Hold and drag horizontal to look left/right, vertical to walk into or outof scene, SHIFT pans, CONTROL translates in xy"); break;
	case FLY_MODE: Message("Move the mouse horizontal to look left/right, vertical to walk into or outof scene, SHIFT pans, CONTROL translates in xy"); break;
	case ZOOM_MODE: Message("Hold and drag horizontal to change zoom scale"); break;
	case ROLL_MODE: Message("Hold and drag horizontal to change up direction"); break;
	default : 	
			Message("Drag with the mouse");
	break;
	}
	}

	SetCapture();


	if (Mode == NULL_MODE) {
        if (view->Select(point.x,point.y,nFlags,GView::DRAG_SENSORS)) {
		}
		return;
   }


	// check for sensor hit, even if a camera walk mode 
   if (m_sensorTest && (Mode>= DOLLY_XY_MODE && Mode <= ZOOM_LENS_MODE)) {
        if (view->Select(point.x,point.y,nFlags,GView::DRAG_SENSORS)) {
			Mode = NULL_MODE;
			return;
		}
   }
 
   if (Mode == ZOOM_MODE) SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
   else SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZE)); // CROSS

//	SetCursor(AfxGetApp()->LoadCursor(IDC_PENCIL));

	if (Mode == FLY_MODE) { 
		fly_state ++;
		if (fly_state == 1) SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
		if (fly_state>1) fly_state = 0;
		if (fly_state >0)  view->BeginDegrade();
	}
	else view->BeginDegrade();

	return(EV_OK);
}

int GCameraEventHandler::OnLButtonDblClk(UINT nFlags, Point &point)
{
	TRACE("%s:OnLButtonDblClk (%f %f %f) %x \n",this->ClassName(),point.x,point.y,point.z,nFlags);
	return(EV_DONE);
}

int GCameraEventHandler::OnMouseMove(UINT nFlags, Point &point)
{
	TRACE("%s:OnMouseMove (%f %f %f) %x \n",this->ClassName(),point.x,point.y,point.z,nFlags);

	return(EV_OK);
}

int GCameraEventHandler::OnLButtonUp(UINT nFlags,  Point &point)
{
	TRACE("%s:OnLButtonUp (%f %f %f) %x \n",this->ClassName(),point.x,point.y,point.z,nFlags);

	if (Mode == FLY_MODE && fly_state == 1) {  // terminate on next click 
		return;
	}
	
	if (m_moveTimerId) {
	   KillTimer(m_moveTimerId);
	   m_moveTimerId = 0;
	}


	ReleaseCapture();

	view->EndDegrade();

	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	
	lbutton=0;
	Mode = NULL_MODE;

	//CView::OnLButtonUp(nFlags, point);

	return(EV_DONE);
}


int GCameraEventHandler::OnTimer()
{
	TRACE("%s:OnTimer %s \n",this->ClassName());
	return(EV_DONE);
}

#define OFS2FAC(ofs) (((ofs)<0.0) ? 1.0 / (-(ofs)+1.0) : (ofs)+1.0)
#define SQ1(F) (((F) < 0.0) ? - ((F)*(F)) : (F)*(F))

#define SQ(F) ((F))


CPoint operator+(CPoint a,CPoint b) { return CPoint(a.x+b.x,a.y+b.y);}
CPoint operator-(CPoint a,CPoint b) { return CPoint(a.x-b.x,a.y-b.y);}

void GCameraEventHandler::OnMouseMove(UINT nFlags, CPoint point)
{

	
	if (view->eventHandler) { // send event to current eventHandler
		Point screen;
		view->Pixel2Screen(point,screen);
		int ret = view->eventHandler->OnMouseMove(nFlags,screen);
		if (ret == GEventHandler::EV_POP) view->PopEventHandler()->unref();

		return;
	}

#if 0
	if (lastWasTimer) {
	  	lbuttonP = m_lastPoint; // reset lButtonP because incremented by timer move
	  	lbuttonP=point;
	  	lastWasTimer = 0;
	  // m_firstPoint =  m_lastPoint; // test
		// Reset move distance
	  	m_firstPoint = point;
		m_firstFlags = nFlags;
		m_firstTime = GetTime();

		m_lastFlags = nFlags;
		m_lastPoint = point;
		m_lastTime = GetTime();
		return; // no update now 

	}
#endif
	m_lastFlags = nFlags;
	m_lastPoint = point;
	m_lastTime = GetTime();
	
	if (Mode == NULL_MODE) {	// Default mouse move action, // display WWW anchors 
	   CPoint lastDelta = point - lbuttonP;

	   if ((abs(lastDelta.x) >=2) || (abs(lastDelta.y) >=2)) {
	      view->Select(point.x,point.y,nFlags,GView::INFO_WWWAnchor);
		  lbuttonP = point;
	   }
	} else {
	   DoMove(nFlags,point);
	}
}

/*

   Repeat last move 

*/

#define INTEPS 3


// execute move action, on time event 
void GCameraEventHandler::DoMoveTimer()
{
	Point delta;


	delta.x = (m_lastPoint.x-m_firstPoint.x);
	delta.y = -(m_lastPoint.y-m_firstPoint.y);
	delta.z = 0;

	if (fabs(delta.x) > fabs(delta.y)) {
			if (fabs(delta.x) < INTEPS) delta.x = 0;
	} else {
			if (fabs(delta.y) < INTEPS) delta.y = 0;
	}

    
	if ((abs((int)delta.x) + (abs((int)delta.y))) <=7) return;

	delta.x *= 0.1;
	delta.y *= 0.1;

	DoDeltaMove(m_lastFlags, delta);
		
	lastWasTimer = 1;



#if 0
   if ((GetTime() - m_lastTime ) > m_moveRepeatStartTime) {
//		DoMove(m_lastFlags,lbuttonP+lastDelta);
		CPoint delta;
		delta = (m_lastPoint-m_firstPoint);
		//delta.x *= 0.25;
		//delta.y *= 0.25;
		delta.x *=0.5;
		delta.y *=0.5;

		DoMove(m_lastFlags,lbuttonP + delta);
		
		lastWasTimer = 1;
   }
#endif 
}


// do the mouse move, based on current mouse position in point 
void GCameraEventHandler::DoMove(UINT nFlags, CPoint point)
{
	Point delta;	
	if (Mode > NULL_MODE) {

	   DoMoveTimer();
	   return;

	   // delta check with origin 
	   delta.x = (point.x-m_firstPoint.x);
	   delta.y = -(point.y-m_firstPoint.y);
	   delta.z = 0;


	   if ((abs((int)delta.x) + (abs((int)delta.y))) <=7) return;

	   delta.x =  (point.x - lbuttonP.x);
	   delta.y = -(point.y - lbuttonP.y);
	   delta.z = 0;
       
//	   if ((abs((int)delta.x) <=2) && (abs((int)delta.y) <=2)) return;



	   DoDeltaMove(nFlags,delta);

	   lbuttonP = point; // incremental movement because camera functions are incremental
	}
	else CView::OnMouseMove(nFlags, point);

}

// do an incremental delta change of parameters
void GCameraEventHandler::DoDeltaMove(UINT nFlags, Point &delta)
{
	if (Mode > NULL_MODE) {
	   float wscale = PI*0.5*0.5*0.5;

	   float x, y,z,dxpix,dypix;

//	   lastDelta = point - lbuttonP;

	   dxpix = x =  delta.x;
	   dypix = y =	delta.y;

#if 0
	   float INTEPS = 4.0-0.0;	// was 2.0 min move epsilon
	   if (fabs(y) > fabs(x)) {
			if (fabs(x) < INTEPS) x = 0;
	   } else {
			if (fabs(y) < INTEPS) y = 0;
	   }
#endif


/*
   too fast 
	   x = SQ1(x); // 16.11.96
	   y = SQ1(y);

*/
	   x *= 2.0;
	   y *= 2.0;



	   x *= 0.005;
	   y *= 0.005;

	// 4.11.95
	   //float scale = LastFrameTime;
	   //if (scale <0.05) scale = 0.05;
	   //x *= 0.005*scale;
	   //y *= 0.005*scale;

	   switch (Mode) {
	   case DOLLY_XY_MODE :
		  if (nFlags & MK_SHIFT) {
		    view->GetCamera().DollyMax(m_cameraSpeed *  - SQ(x) *view->GetCamera().width,0.0, m_cameraSpeed * SQ(y) * 0.5 * view->GetCamera().TargetDistance(),!GetCameraRelativeMode());
		  } else {
		    view->GetCamera().DollyMax(m_cameraSpeed *  - SQ(x) *view->GetCamera().width,
		  						m_cameraSpeed * - SQ(y) *view->GetCamera().width,0.0,!GetCameraRelativeMode());
		  }
		  break;
	   
      case ORBIT_MODE :
		  if (nFlags & MK_CONTROL) {		// ZOOM
		  		float f=x*0.1;
		   		x*=-1.0;
		   		if (x<-0.5) x = -0.5;else if (x>0.5) x = 0.5;
		   		view->GetCamera().Zoom(OFS2FAC(x));
/*		  } else  if (nFlags & MK_SHIFT) {	 // ROLL 
			   view->GetCamera().RollMax(wscale*SQ(x));
*/
		  } else {
#if 0
           // try relative 
           GCamera &c = view->GetCamera();

           Point oldTarget(c.target);
           c += (- c.target + c.worldCenter);
           c.OrbitMax(-wscale*SQ(x),wscale*SQ(y),0 && !GetCameraRelativeMode());
           c += ( oldTarget - c.worldCenter);
#endif

			if (nFlags & MK_SHIFT) 
				view->GetCamera().OrbitCenterMax(-wscale*SQ(x),wscale*SQ(y),0 && !GetCameraRelativeMode());
		    else view->GetCamera().OrbitMax(-wscale*SQ(x),wscale*SQ(y),0 && !GetCameraRelativeMode());
		  }
		  break;

	   case PAN_MODE :
		  view->GetCamera().PanMax(wscale*SQ(x),wscale*SQ(y),!GetCameraRelativeMode());
		  break;

	   case ZOOM_MODE :
		   { float f=x*0.1;
		   x*=-1.0;
		   if (x<-0.5) x = -0.5;else if (x>0.5) x = 0.5;
		   view->GetCamera().Zoom(OFS2FAC(x));

		   if (0) { 
				y*=-1.0;
				if (y<-0.5) y = -0.5;else if (y>0.5) y = 0.5;
				view->GetCamera().ZoomDistance(OFS2FAC(y));
		   }

		   }
		  break;
	   case ZOOM_LENS_MODE :
		   { float f=x*0.1;
		   x*=-1.0;
		   if (x<-0.5) x = -0.5;else if (x>0.5) x = 0.5;
		   view->GetCamera().ZoomDistance(OFS2FAC(x));
		   }
		  break;
	   case ROLL_MODE :
		   view->GetCamera().RollMax(wscale*SQ(x));
		   break;

	   case WALK_MODE :
	   case FLY_MODE :
		  if (nFlags & MK_CONTROL) {
            x = limit(x,-0.5f,0.5f);
            y = limit(y,-0.5f,0.5f);

		    view->GetCamera().Dolly(m_cameraSpeed *  - SQ(x) *view->GetCamera().width,
		  						m_cameraSpeed * - SQ(y) *view->GetCamera().width,
                                0.0,
                                !GetCameraRelativeMode());
		  } else 
		  if (nFlags & MK_SHIFT) {
		  	view->GetCamera().PanMax(0.5f * -wscale*SQ(x),0.5*wscale*SQ(y),!GetCameraRelativeMode()); // new 0.5 new 9.11.
		  } else {
		    // x:pan x + y :dolly z
		    view->GetCamera().PanMax(0.5f * -wscale*SQ(x),0.0,!GetCameraRelativeMode()); 
			// was 0.25 without SQ
            y = limit(y,-0.5f,0.5f);
		    view->GetCamera().Dolly(0.0,0.0, m_cameraSpeed * SQ(y) * 0.5 * view->GetCamera().TargetDistance(),!GetCameraRelativeMode());
		  }

		  break;


//	   default :
//		  break;

	   }
	   Redraw();
	}
}



// handle a key, return false if key not handlerd 
BOOL GCameraEventHandler::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	float wstep = PI / 18.0;
	float dstep = 0.1;

	int shift = GetAsyncKeyState(VK_SHIFT);  // also VK_LSHIFT RSHIFT
	int control = GetAsyncKeyState(VK_CONTROL);
	int alt = GetAsyncKeyState(VK_MENU);

	float dist = view->GetCamera().width;

   int walk= 0;
   int orbit =0;
   int pan = 0;

   if (control) {
      pan = 1;
   } else
   
   if (GetCameraMode() == WALK_MODE) {
      if (shift) orbit = 1;
      else walk = 1;
   } 
   else {
      if (!shift) orbit = 1;
      else walk = 1;
   } 


   
	switch (nChar) {
#if 0
	case VK_ESCAPE : 
			   if (view->GetFullScreen()) {
					view->SetFullScreen(0);
			   }
			   //if (view->currentTraversal) view->currentTraversal->stop ++;
			   else if (view->AnimatingCamera()) {
				   view->AnimateCameraStop();
			   }
	           else if (view->time.enabled) OnRenderAnimate(); // switch of animation
               else if (view->GetTexturing()) {
                     OnRenderTextured();
               }
			   else {
					WWWAbortLoad();
					return;
			   }
			break;
	case VK_NEXT: 		// 	22	PAGE DOWN key 
				OnCameraViewpointNext();
				break;
	case VK_PRIOR: 		// PAGE UP key 
				OnCameraViewpointPrev();
				break;
#endif

    case (UINT)'+' :
			if (orbit) 
		    	view->GetCamera().Zoom(1.0/1.14);
			else 
//		    view->GetCamera().Dolly(0.0,0.0, -dstep*nRepCnt * view->GetCamera().TargetDistance());
      		view->GetCamera().Dolly(0.0,dstep*nRepCnt * dist,0.0); 

			break;
    case (UINT)'-' :
			if (orbit) 
		    	view->GetCamera().Zoom(1.14);
			else 
				//view->GetCamera().Dolly(0.0,0.0, dstep*nRepCnt * view->GetCamera().TargetDistance());
      			view->GetCamera().Dolly(0.0,-dstep*nRepCnt * dist,0.0); 
			break;
#if 0
	case VK_END: 		// 	23	END key 
	case VK_HOME: 		// 	24	HOME key 
                OnCameraViewObject();
			break;
#endif 

	case VK_LEFT: 		// 	25	LEFT ARROW key 
			if (walk) 
		        view->GetCamera().Dolly(-dstep*nRepCnt * dist,0.0,0.0); 
			 else if (pan)
			    view->GetCamera().Pan(-wstep*nRepCnt,0.0); 
			 else 
			   view->GetCamera().Orbit(wstep*nRepCnt,0.0); 
			break;
	case VK_UP: 		// 	26	UP ARROW key 
			if (walk) 
              view->GetCamera().Dolly(0.0,0.0, dstep*nRepCnt * view->GetCamera().TargetDistance());
			else if (pan)
			    view->GetCamera().Pan(0.0,wstep*nRepCnt); 
			else 
			    view->GetCamera().Orbit(0.0,wstep*nRepCnt); 
			break;
	case VK_RIGHT: 		// 	27	RIGHT ARROW key 
			 if (walk) 
		        view->GetCamera().Dolly(dstep*nRepCnt * dist,0.0,0.0); 
			 else if (pan)
			    view->GetCamera().Pan(wstep*nRepCnt,0.0); 
			 else 
			    view->GetCamera().Orbit(-wstep*nRepCnt,0.0); 
			    break;
	case VK_DOWN: 		// 	28	DOWN ARROW key 
			 if (walk) 
		        //view->GetCamera().Dolly(0.0,-dstep*nRepCnt * dist,0.0); 
              view->GetCamera().Dolly(0.0,0.0, -dstep*nRepCnt * view->GetCamera().TargetDistance());
			 else if (pan)
			    view->GetCamera().Pan(0.0,-wstep*nRepCnt); 
			 else 
			    view->GetCamera().Orbit(0.0,-wstep*nRepCnt); 
			    break;
/* 
    case (UINT)'f' :
    case (UINT)'F' :
                OnOptionsFullscreen();
                break;

    case (UINT)'j' :
    case (UINT)'J' :
                OnCameraViewObject();
                break;

    case (UINT)'m' :
    case (UINT)'M' :
                OnCameraDollyXy();
                break;


    case (UINT)'w' :
    case (UINT)'W' :
                OnCameraWalk();
                break;

    case (UINT)'o' :
    case (UINT)'O' :
                OnCameraOrbit();
                break;
*/
	default :
			return(FALSE);
		break;
	}
	if (view->GetCamera().changed) {
		Redraw();
	}
}


