
#include "stdafx.h"
#include "blaxxunCC3dCtl.h"

#include "GPanel.h"
#include "GModel.h"
#include "grayselect.h"

//speed of navigation is affected by the rendering time of the last frame
// this variables are limiting the effect
#define NAV_MIN_TIME 0.001 // was 0.005
#define NAV_MAX_TIME 0.5


// walk in horizontal mode
#define NAV_PAN_SCALE (1.0/20.0)
#define NAV_ORBIT_SCALE (1.0/10.0)


/**-------------------------------------------------------------------------
  CGLViewCtrlCtrl::NavStart

  Called by:	Called by OnLButtonDown, OnKeyDown
  
  Description:	Initializes all Navigation stuff
				Called if arrow keys and / or left mouse button pressed
--------------------------------------------------------------------------*/

void CGLViewCtrlCtrl::NavStart() 
{
	POINT		mousepos;
	RECT	    client_rect;
	int			nFlags;
	int			steps;

	nFlags = 0;
	double ftime = (double)view->lastFrameTime / 1000.0f;

	// ensure steps are not too big if long render time
	ftime = min(ftime, NAV_MAX_TIME);
	// ensure steps are not to small if fast render time
	ftime = max(ftime, NAV_MIN_TIME);

		

	switch (m_cameraSpeedFac)
		{
		case 2:
			m_cameraSpeed = view->viewStepSpeed / 5.0;	
			m_cameraSpeedRot = 1.0 / 5.0;	
			break;
		case 5:
			m_cameraSpeed = view->viewStepSpeed / 2.0;	
			m_cameraSpeedRot = 1.0 / 2.0;	
			break;
		case 10:
			m_cameraSpeed = view->viewStepSpeed;	
			m_cameraSpeedRot = 1.0;	
			break;
		case 20:
			m_cameraSpeed = view->viewStepSpeed * 2.0;	
			m_cameraSpeedRot = 1.0 * 2.0;	
			break;
		case 60:
			m_cameraSpeed = view->viewStepSpeed * 5;	
			m_cameraSpeedRot = 1.0 * 5.0;	
			break;
		default:
			m_cameraSpeed = view->viewStepSpeed;	
			m_cameraSpeedRot = 1.0;	
		}		


	if (GetKeyState(VK_BACK) &  0x8000)
		{
		m_NavMotionIP.StopWhere = 0;	// go to the end

		// may last 2 seconds
		steps = (int)(2.0 / ftime);
		// start interpolation (rotate to hit point)					
		NavStartMotionIP(m_NavCamera.DefPosition, steps);
		NavStartRotPIP(m_NavCamera.DefLookAt, steps);
		return;
		}
	else
		{
		// store current camera position and orientation	
		m_NavCamera.DefPosition = view->GetCamera().position;
		m_NavCamera.DefLookAt = view->GetCamera().target;
		}

//	m_currentMode = NULL_MODE;
		


		// get screen position of cursor
		// and convert to client area coordinates
		GetCursorPos(&mousepos);
		ScreenToClient( &mousepos);

		m_firstPoint = mousepos;


		
		if (view->panel && m_navPanelDrag)
			//fake m_firstPoint
		{
			POINT newFirstPoint;
			view->panel->GetCenterPos(&newFirstPoint);
			m_firstPoint = newFirstPoint;
		}

		// if called from device MOUSE (left mouse button down)
		if (m_NavLBUTState)
 			{
			// If TAB is pressed -> Jump to Object
			if (m_NavPointState) 
				{
				NavBeamTo(mousepos);
				}
			else
			// If SPACE is pressed -> Follow Object
			if (m_NavFollowState) 
				{
				if (m_NavFollowObject)
					{
					NavResetFollowObject();
					}
				if (view->Select(mousepos.x,mousepos.y,0,GView::FOLLOW_OBJECT)) 
					m_NavFollowObject = TRUE;
				}
			else
				{
				switch (GetCameraMode()) {
					case JUMP_TO_MODE : 
						if (m_NavSpeedState) {	// shift pressed 		
							if (m_NavFollowObject)
							{
								NavResetFollowObject();
							}
							if (view->Select(mousepos.x,mousepos.y,0,GView::FOLLOW_OBJECT)) {
								m_NavFollowObject = TRUE;
							}
						} else NavBeamTo(mousepos); 
						break;

					case WALK_MODE:
					case PAN_MODE:
					case DOLLY_XY_MODE:
						if (!m_NavRotState)
							break;
					case ORBIT_MODE:
					case ROTATE_MODE:
						{
						// dont animate to clickpoint if on sensor or anchor
						if (!m_wasDrag && (m_currentTSensor || m_currentAnchor))
							break;

						if (view->Select(mousepos.x,mousepos.y,nFlags,GView::INFO)) 
							{
							// may last 3 seconds
							steps = (int)(3.0 / ftime);
							// start interpolation (camera rotation towards hit point)
							
							NavStartRotPIP(view->lastHit->pw,steps);
							m_NavCamera.FixPoint = view->lastHit->pw;
							m_NavCamera.FixPointLocal = view->lastHit->p;
							m_currentMode = ROTATE_MODE;
							m_NavCamera.FixPointValid = TRUE;
							}
						}
					}
				}
			}


		// get window rectangle 	
		GetClientRect( &client_rect);
			
		// centre mouse to the centre of client rectangle
		mousepos.x = client_rect.right / 2;
		mousepos.y = client_rect.bottom / 2;
		
		
		m_NavMouseDiff.x = m_firstPoint.x - mousepos.x;
		m_NavMouseDiff.y = m_firstPoint.y - mousepos.y;
		

		// get screen coordinates
		//ClientToScreen( &mousepos);
		
		// center the cursor
		//SetCursorPos(mousepos.x, mousepos.y);

		// CONTROL key is unpressed
		//m_NavSlideState  = FALSE;
//		m_NavSlideState  = ::GetKeyState(VK_MENU) &  0x8000;				
//		m_NavSlideState  = ::GetKeyState(VK_MENU) &  0x8000;				

		// set navigation cursor
		//SetCursorMode(NAVIGATION_CURSOR);
	//	SetCursorMode(MOVING_CURSOR);

//		SetCursorMode(GetCameraMode());


//		view->BeginDegrade();

	// start navigation timer which calls NavGo every tick
	if (!m_NavTimerRunning) 
		NavStartTimer();


}

void CGLViewCtrlCtrl::NavBeamTo(POINT mousepos) 
{
	int			steps;

	double ftime = (double)view->lastFrameTime / 1000.0f;
	// ensure steps are not too big if long render time
	ftime = min(ftime, NAV_MAX_TIME);

	// ensure steps are not to small if fast render time
	ftime = max(ftime, NAV_MIN_TIME);

	// may last 2 seconds
	steps = (int)(5.0 / ftime);
	// how to move out ?
	if (view->Select(mousepos.x,mousepos.y,0,GView::INFO)) 
	{
		// start interpolation (rotate to hit point)					
		m_NavMotionIP.StopWhere = 1;
		m_NavMotionIP.StopPercentage = 80;
		Point pw= view->cameraMatrix.Inverse() * view->lastHit->pw; // transform to camera system 19.07.99 hg
		
		NavStartMotionIP(pw, steps);
		NavStartRotPIP(pw, (steps * 8) / 10);
	}

}



/**-------------------------------------------------------------------------
  CGLViewCtrlCtrl::NavEnd

  Called by:	Called if no more navigation action
  
  Description:	Ends all Navigation stuff
				Kills Timer / Reset Cursor 
--------------------------------------------------------------------------*/

void CGLViewCtrlCtrl::NavEnd() 
{

			// stop navigation timer if running
			if (m_NavTimerRunning) 
				NavStopTimer();
			
//			SetCursorMode(STANDARD_CURSOR);

//			view->EndDegrade();

			lastCursorMode = -1;
			ReleaseCapture();

			m_currentMode = NULL_MODE;
			m_NavRunning = 0;
			m_NavIPActive = 0;
			m_NavCamera.FixPointValid = FALSE;

}




/**-------------------------------------------------------------------------
  CGLViewCtrlCtrl::NavGo

  Called by:	Called by Navigation Timer tick
  
  Description:	Checks navigation preconditions (keys or mouse button down)
				Condition TRUE -> call NavMain
				Condition FALSE -> kill Timer
--------------------------------------------------------------------------*/

void CGLViewCtrlCtrl::NavGo() 
{
Point		dir;

 		// get screen position of cursor
		// and convert to client area coordinates
		GetCursorPos(&m_NavMousePos);
		ScreenToClient(&m_NavMousePos);

		// get window rectangle 	
//		GetClientRect( &m_NavClientRect);

//	if ((lastCursorMode >= NAV_ANCHOR) && m_wasDrag)
//		SetCursorMode(GetCameraMode());
		

// calculate the real speed
	    double ftime = (double)view->lastFrameTime / 1000.0; 
		// ensure steps are not too big if long render time
		ftime = min(ftime, NAV_MAX_TIME);
		
		// ensure steps are not to small if fast render time
		ftime = max(ftime, NAV_MIN_TIME);


	    m_NavSpeed = m_cameraSpeed * ftime;
		m_NavSpeedRot = m_cameraSpeedRot * ftime;


	// default no gravity checked	
		m_NavGravityChecked = FALSE;

		// calculate distance camera position - target
		m_NavTargetDist = view->GetCamera().TargetDistance();

		// recalculate fix point for rotate mode  (if object has moved)
		if ((m_currentMode == ROTATE_MODE) && m_NavCamera.FixPointValid && GetCameraMode() != ORBIT_MODE)
			{			
			Matrix		m;

			if (view->lastHit && view->lastHit->path.getMatrix(m)) 
			{ // reevalute matrix of object 
				Point p = m_NavCamera.FixPointLocal;
				p*=m;
			
				if (m_NavCamera.FixPoint != p) // fixpoint no longer the same as on lbuttondown 
				{
					if (m_NavIPActive & NAV_ROT_P_IP)
						NavStopIP(NAV_ROT_P_IP);
				
					m_NavCamera.ForceAction = TRUE;
					m_NavCamera.FixPoint = p; //?? lastHit.pw ?
				}
			}
		}

		// default navigation mode is undefined
		// will be checked by NavMain
		//m_currentMode = NULL_MODE;

		// check if left mouse button is pressed
		if (m_NavLBUTState)
			{
			// was not pressed before
			if (!(m_NavRunning & 0x1))
				{
				// capture the mouse event outside the window 
//				SetCapture();
				m_NavRunning |= 0x1;
				}
			}
		else
			{
			// is now released and was pressed before
			if (m_NavRunning & 0x1)
				{
				lastCursorMode = -1;
//				ReleaseCapture();
				m_NavRunning &= ~0x1;
				}
			}
		
		// check if one of the navigation keys is pressed
		if (m_NavUPState || m_NavDOWNState || m_NavLEFTState || m_NavRIGHTState)
			m_NavRunning |= 0x2;
		else
			m_NavRunning &= ~0x2;

		// any navigation action at all ?
		if (m_NavRunning)
			{
			NavMain();
			}
		else
			{			
			if (m_currentMode == ROTATE_MODE)
				NavStopIP(NAV_ROT_P_IP);
			
			lastCursorMode = -1;
			ReleaseCapture();
			
			m_NavCollision.LastCollision = FALSE;
			}

		// any interpolator active ?
		if (m_NavIPActive)
			{
			if (m_NavIPActive & NAV_MOTION_IP)
				NavStepMotionIP();
	
			if (m_NavIPActive & NAV_ROT_P_IP)
				NavStepRotPIP();
			else
			if (m_NavIPActive & NAV_ROT_V_IP)
				NavStepRotVIP();

			// gravity IP running
			if (m_NavIPActive & NAV_GRAVITY_IP)
				// no gravity check by other modules
				if (!m_NavGravityChecked)
					NavStepGravityIP();
			}
	   
		if (m_NavCoordDspl) { // display coords 
			CString			fname;
			char			str[256];
			Vector axis;
			float angle;
			GCamera &camera = view->GetCamera();

			ConvertCameraToOrientation(camera.position, camera.target,camera.up,axis, angle);


			sprintf(str,"x:%g y:%g z:%g (%g %g %g a=%g)", camera.position.x, camera.position.y, camera.position.z,axis.x,axis.y,axis.z,angle);
			fname = str;
			Message(fname);
		}

		// no navigation and no interpolator -> stop navigation timer
		if (!(m_NavRunning || m_NavIPActive))
			NavEnd();
		else
			{
			if (m_NavIPActive)
				m_wasDrag = TRUE;
			view->GetCamera().OnChanged();
			Redraw();
			}
}

	




/**-------------------------------------------------------------------------
  CGLViewCtrlCtrl::NavMain

  Called by:	Called by NavGo
  
  Description:	Calculates the tile where the mouse cursor is within navigation window.

--------------------------------------------------------------------------*/

void CGLViewCtrlCtrl::NavMain() 
{
	Point		tile;
	Point		key_tile;
	Point		vec3;


	key_tile.x = 0;
	key_tile.y = 0;
   	tile       = key_tile;

	m_NavMotion = TRUE;

	// check if keys are pressed
	if (m_NavUPState) 
		{
		key_tile.y -= 5;
		}
	if (m_NavDOWNState) 
		{
	  	key_tile.y += 5;
		}
	if (m_NavRIGHTState) 
		{
	  	key_tile.x += 5;
		}
	if (m_NavLEFTState) 
		{
	  	key_tile.x -= 5;
		}

 	// if there is some mouse navigation then calculate tile within window
   	if (m_NavRunning & 0x1)
	   	NavCalculateTile(&tile);		

  	// add tiles of keyboard 
  	tile.x += key_tile.x;
  	tile.y += key_tile.y;

	//feedback (navigation)
	if (view->PanelOk() && !view->panel->Active())
	{
		if (m_navPanelDrag) view->panel->setNavigationMode(1);
		//normally the mode is set in navcheckcursor
		//in some cases the value is overridden by navstoptimer
		view->panel->setNavigationDirection(tile.x, tile.y);
	}
			

	if (m_NavSpeedState) {
		m_NavSpeed_step = m_NavSpeed * 5.0;
		m_NavSpeedRot_step = m_NavSpeedRot * 5.0;
	} else {
		m_NavSpeed_step = m_NavSpeed;
		m_NavSpeedRot_step = m_NavSpeedRot;
	}
	// move smaller in panel drag 
	if (view->panel &&  m_navPanelDrag) {
		m_NavSpeed_step = m_NavSpeed_step *.5f; 
		m_NavSpeedRot_step = m_NavSpeedRot_step *.5f;
	}

	// calculate movement vectors
#ifdef _DEBUG_NAVIGATION
	TRACE("CGLViewCtrlCtrl::NavMain:  speed: %3f\n", m_NavSpeed_step);
	TRACE("CGLViewCtrlCtrl::NavMain:  tile.x: %g y%g  moved %g \n", tile.x,tile.y,tile.z);
#endif _DEBUG_NAVIGATION
	vec3.x = (float)-tile.x * m_NavSpeed_step; // compute movement step
	vec3.y = (float)-tile.y * m_NavSpeed_step;
	
	vec3.z = (tile.x !=0)  ||  (tile.y!=0);

	if ((view->panel && view->panel->Active()) || m_navPanelDrag) {
#ifdef _DEBUG_NAVIGATION
		TRACE("CGLViewCtrlCtrl::NavMain:  vec3: %3f %3f \n", vec3.x, vec3.y);
#endif _DEBUG_NAVIGATION

		vec3.x *= 2.0;
		NavWalk(vec3);
	}
	else
	{
		switch (GetCameraMode()) 
			{
			case WALK_MODE:
					vec3.x *= 2;
					NavWalk(vec3);
					break;
			case PAN_MODE:
					NavPan(vec3);
					break;

			case DOLLY_XY_MODE:
					NavMove(vec3);
					break;

			case ORBIT_MODE:
					NavExamine(vec3);
					break;

			case ROTATE_MODE:
					NavRotate(vec3);
					break;
			case FLY_MODE:
					NavFly(vec3);
					break;

			case JUMP_TO_MODE:
					NavJumpTo(vec3);
					break;
			default :
					NavWalk(vec3);
					break;
		}
	}
}

void CGLViewCtrlCtrl::NavWalk(Point vec3)
{
	Point		oldPosition;
	float		old_dist, new_dist;
	Point		dir;
	Point		old_LookAt;
	int			steps;

	// dont do something until drag 
	if (!m_wasDrag && (m_currentTSensor || m_currentAnchor))
		return;

	if (m_currentMode == ROTATE_MODE)
		{
 		// no longer TAB pressed
		if (!m_NavRotState) 
		// release rotate mode
			{
			// is interpolator still active ?
			if (m_NavIPActive & NAV_ROT_P_IP)
				// stop interpolation
				NavStopIP(NAV_ROT_P_IP);
			
			m_currentMode = NULL_MODE;
			m_NavCamera.FixPointValid = FALSE;		
			}
		}
	else
	//if (m_currentMode == WALK_MODE)	
		{
 		//  TAB now pressed
		if (m_NavRotState) 
		// set rotate mode
			{
			// is interpolator still active ?
			if (!(m_NavIPActive & NAV_ROT_P_IP))
				{
				if (view->Select(m_NavMousePos.x,m_NavMousePos.y,0,GView::INFO)) 
					{
					// animation may last 3 seconds
					steps = (int)(3000.0 / view->lastFrameTime);
					// start interpolation (camera rotation towards hit point)
					//NavStartRotPIP(view->lastHit->pw, 50);
					NavStartRotPIP(view->lastHit->pw, steps);
					m_NavCamera.FixPoint = view->lastHit->pw;
					m_NavCamera.FixPointLocal = view->lastHit->p;
					m_NavCamera.ForceAction = TRUE;
					m_currentMode = ROTATE_MODE;
					m_NavCamera.FixPointValid = TRUE;
					}
				}
			}
		}
	
	if (vec3.z)
		{
		oldPosition = view->GetCamera().position;
		old_LookAt = view->GetCamera().target - view->GetCamera().position;
		//targetDist = old_LookAt.Length();
		old_LookAt.Normalize();

		switch (m_currentMode) 
			{
			case NULL_MODE :
				{		
				// CTRL is pressed -> PAN
				if (m_NavSlideState)
					{
					view->GetCamera().DollyX(-vec3.x,!GetCameraRelativeMode());
					NavCheckCollision(oldPosition,FALSE,FALSE,FALSE);			
					view->GetCamera().DollyY(vec3.y,!GetCameraRelativeMode());
					NavCollisionGround(oldPosition);					
					}
				else
					{
					vec3.x *= m_NavSpeedRot_step / m_NavSpeed_step; // correct rotation

					view->GetCamera().PanX(vec3.x *NAV_PAN_SCALE,!GetCameraRelativeMode()); 
					view->GetCamera().DollyZ(vec3.y,!GetCameraRelativeMode());

					// check stay on ground
					NavCheckCollision(oldPosition,TRUE,TRUE,TRUE);			
					NavCheckStayOnGround();
					}
				}
				break;
						
			case ROTATE_MODE: 
				{		
				if (!m_NavCamera.FixPointValid)
					return;
				// get vector camera - fix point
				dir = 	m_NavCamera.FixPoint - view->GetCamera().position;
				// get length of vector
				old_dist = dir.Length();
				
				// CTRL is pressed -> PAN-ROTATE
				if (m_NavSlideState)
					{
					// ensure look at angle is not bigger than 60 degrees
					if (((old_LookAt.y <= -0.8f && vec3.y > 0.0f) ||
						 (old_LookAt.y >=  0.8f && vec3.y < 0.0f)))
						vec3.y = 0.0f;
					// move camera up / right
					view->GetCamera().DollyX(-vec3.x,!GetCameraRelativeMode());
					NavCheckCollision(oldPosition,FALSE,FALSE,FALSE);			
					view->GetCamera().DollyY(vec3.y,!GetCameraRelativeMode());
					NavCollisionGround(oldPosition);					

					// now let camera look to fix point
					dir =  m_NavCamera.FixPoint - view->GetCamera().position;
					new_dist = dir.Length();
					// normalize new direction vector
					dir.Normalize();
					// set new target 
					view->GetCamera().target = view->GetCamera().position + dir * m_NavTargetDist;
					// correct distance
					view->GetCamera().DollyZ(new_dist-old_dist,!GetCameraRelativeMode());
					}
				else
					{
					// move camera towards fixpoint
					if (vec3.y)	
						// ensure look at angle is not bigger than 60 degrees
						if (!(old_LookAt.y >=  0.8f && vec3.y > 0.0f))
							{
							dir = 	m_NavCamera.FixPoint - view->GetCamera().position;
							// get length of vector
							old_dist = dir.Length();
							if ((old_dist - vec3.y) < 3.0) 
								vec3.y = old_dist - 3.0;
							view->GetCamera().DollyZ(vec3.y,!GetCameraRelativeMode());
							dir = 	m_NavCamera.FixPoint - view->GetCamera().position;
							// get length of vector
							old_dist = dir.Length();
							}

					// move camera left / right						
					if (vec3.x)
						{
							// move camera left / right
						view->GetCamera().DollyX(-vec3.x, !GetCameraRelativeMode());
						// get new vector camera - fix point
						dir = 	m_NavCamera.FixPoint - view->GetCamera().position;
						new_dist = dir.Length();
						// correct distance
						view->GetCamera().DollyZ(new_dist-old_dist,!GetCameraRelativeMode());
						}
					// check for collision detection and correct camera position
					NavCheckCollision(oldPosition, FALSE,TRUE,TRUE);			
					// check stay on ground
					NavCheckStayOnGround();
					// now let camera look to fix point
					NavCameraFixToPoint();	
					}
				}
				break;

			default :
				break;
			}
		}
	else		
		{
		if (!m_NavSlideState)
			// perform stay on ground check
			NavCheckStayOnGround();
		
		if (m_NavCamera.ForceAction)
			{		
			NavCameraFixToPoint();	
			m_NavCamera.ForceAction = FALSE;
			}
		}
}

void CGLViewCtrlCtrl::NavPan(Point vec3)
{
	Point		oldPosition;
	float		old_dist, new_dist;
	Point		dir;
	Point		old_LookAt;
	int			steps;

	// dont do something until drag 
	if (!m_wasDrag && (m_currentTSensor || m_currentAnchor))
		return;
	
	
	if (m_currentMode == ROTATE_MODE)
		{
 		// no longer TAB pressed
		if (!m_NavRotState) 
		// release rotate mode
			{
			// is interpolator still active ?
			if (m_NavIPActive & NAV_ROT_P_IP)
				// stop interpolation
				NavStopIP(NAV_ROT_P_IP);
			
			m_currentMode = NULL_MODE;
			m_NavCamera.FixPointValid = FALSE;		
			}
		}
	else
	//if (m_currentMode == WALK_MODE)	
		{
 		//  TAB now pressed
		if (m_NavRotState) 
		// set rotate mode
			{
			// is interpolator still active ?
			if (!(m_NavIPActive & NAV_ROT_P_IP))
				{
				if (view->Select(m_NavMousePos.x,m_NavMousePos.y,0,GView::INFO)) 
					{
					// animation may last 3 seconds
					steps = (int)(3000.0 / view->lastFrameTime);
					// start interpolation (camera rotation towards hit point)
					//NavStartRotPIP(view->lastHit->pw, 50);
					NavStartRotPIP(view->lastHit->pw, steps);
					m_NavCamera.FixPoint = view->lastHit->pw;
					m_NavCamera.FixPointLocal = view->lastHit->p;
					m_NavCamera.ForceAction = TRUE;
					m_currentMode = ROTATE_MODE;
					m_NavCamera.FixPointValid = TRUE;
					}
				}
			}
		}
	
	if (vec3.z)
		{
		oldPosition = view->GetCamera().position;
		old_LookAt = view->GetCamera().target - view->GetCamera().position;
		//targetDist = old_LookAt.Length();
		old_LookAt.Normalize();

		switch (m_currentMode) 
			{
			case NULL_MODE :
				{		
				// CTRL is pressed -> PAN
				if (m_NavSlideState)
					{
					view->GetCamera().DollyX(-vec3.x,!GetCameraRelativeMode());
					NavCheckCollision(oldPosition,FALSE,FALSE,FALSE);			
					view->GetCamera().DollyY(vec3.y,!GetCameraRelativeMode());
					NavCollisionGround(oldPosition);					
					}
				else
					{
					vec3.x *= m_NavSpeedRot_step / m_NavSpeed_step; // correct rotation
					vec3.y *= m_NavSpeedRot_step / m_NavSpeed_step; // correct rotation
					view->GetCamera().Pan(vec3.x *NAV_PAN_SCALE,vec3.y *NAV_PAN_SCALE,!GetCameraRelativeMode()); 

					//view->GetCamera().PanX(vec3.x *NAV_PAN_SCALE,!GetCameraRelativeMode()); 
					//view->GetCamera().PanY(vec3.y *NAV_PAN_SCALE,!GetCameraRelativeMode()); 

					// check stay on ground
					// NavCheckCollision(oldPosition,TRUE,TRUE,TRUE);			
					// NavCheckStayOnGround();
					}
				}
				break;
						
			case ROTATE_MODE: 
				{		
				if (!m_NavCamera.FixPointValid)
					return;

				// get vector camera - fix point
				dir = 	m_NavCamera.FixPoint - view->GetCamera().position;
				// get length of vector
				old_dist = dir.Length();
				
				// CTRL is pressed -> PAN-ROTATE
				if (m_NavSlideState)
					{
					// ensure look at angle is not bigger than 60 degrees
					if (((old_LookAt.y <= -0.8f && vec3.y > 0.0f) ||
						 (old_LookAt.y >=  0.8f && vec3.y < 0.0f)))
						vec3.y = 0.0f;
					// move camera up / right
					view->GetCamera().DollyX(-vec3.x,!GetCameraRelativeMode());
					NavCheckCollision(oldPosition,FALSE,FALSE,FALSE);			
					view->GetCamera().DollyY(vec3.y,!GetCameraRelativeMode());
					NavCollisionGround(oldPosition);					

					// now let camera look to fix point
					dir =  m_NavCamera.FixPoint - view->GetCamera().position;
					new_dist = dir.Length();
					// normalize new direction vector
					dir.Normalize();
					// set new target 
					view->GetCamera().target = view->GetCamera().position + dir * m_NavTargetDist;
					// correct distance
					view->GetCamera().DollyZ(new_dist-old_dist,!GetCameraRelativeMode());
					}
				else
					{
					// move camera towards fixpoint
					if (vec3.y)	
						// ensure look at angle is not bigger than 60 degrees
						if (!(old_LookAt.y >=  0.8f && vec3.y > 0.0f))
							{
							dir = 	m_NavCamera.FixPoint - view->GetCamera().position;
							// get length of vector
							old_dist = dir.Length();
							if ((old_dist - vec3.y) < 3.0) 
								vec3.y = old_dist - 3.0;
							view->GetCamera().DollyZ(vec3.y,!GetCameraRelativeMode());
							dir = 	m_NavCamera.FixPoint - view->GetCamera().position;
							// get length of vector
							old_dist = dir.Length();
							}

					// move camera left / right						
					if (vec3.x)
						{
							// move camera left / right
						view->GetCamera().DollyX(-vec3.x, !GetCameraRelativeMode());
						// get new vector camera - fix point
						dir = 	m_NavCamera.FixPoint - view->GetCamera().position;
						new_dist = dir.Length();
						// correct distance
						view->GetCamera().DollyZ(new_dist-old_dist,!GetCameraRelativeMode());
						}
					// check for collision detection and correct camera position
					NavCheckCollision(oldPosition, FALSE,TRUE,TRUE);			
					// check stay on ground
					NavCheckStayOnGround();
					// now let camera look to fix point
					NavCameraFixToPoint();	
					}
				}
				break;

			default :
				break;
			}
		}
	else		
		{
		if (!m_NavSlideState)
			// perform stay on ground check
			NavCheckStayOnGround();
		
		if (m_NavCamera.ForceAction)
			{		
			NavCameraFixToPoint();	
			m_NavCamera.ForceAction = FALSE;
			}
		}
}




void CGLViewCtrlCtrl::NavMove(Point vec3)
{
float		old_dist, new_dist;
Point		dir;
Point		oldPosition;
Point		old_LookAt;
int			steps;

	// dont do something until drag 
	if (!m_wasDrag && (m_currentTSensor || m_currentAnchor))
		return;

	
	if (m_currentMode == ROTATE_MODE)
		{
 		// no longer TAB pressed
		if (!m_NavRotState) 
		// release rotate mode
			{
			// is interpolator still active ?
			if (m_NavIPActive & NAV_ROT_P_IP)
				// stop interpolation
				NavStopIP(NAV_ROT_P_IP);
			
			m_currentMode = NULL_MODE;
			m_NavCamera.FixPointValid = FALSE;		
			}
		}
	else
		{
 		//  TAB now pressed
		if (m_NavRotState) 
		// set rotate mode
			{
			// is interpolator still active ?
			if (!(m_NavIPActive & NAV_ROT_P_IP))
				{
				
				if (view->Select(m_NavMousePos.x,m_NavMousePos.y,0,GView::INFO)) 
					{
					// animation may last 3 seconds
					steps = (int)(3000.0 / view->lastFrameTime);
					// start interpolation (camera rotation towards hit point)
					//NavStartRotPIP(view->lastHit->pw, 50);
					NavStartRotPIP(view->lastHit->pw, steps);
					m_NavCamera.FixPoint = view->lastHit->pw;
					m_NavCamera.FixPointLocal = view->lastHit->p;
					m_NavCamera.ForceAction = TRUE;
					m_currentMode = ROTATE_MODE;
					}
				}
			}
		}
	
	if (vec3.z || m_NavCamera.ForceAction)
		{
		m_NavCamera.ForceAction = FALSE;

		oldPosition = view->GetCamera().position;

		old_LookAt = view->GetCamera().target - view->GetCamera().position;
		//targetDist = old_LookAt.Length();
		old_LookAt.Normalize();

		switch (m_currentMode) 
			{
			case NULL_MODE :
				{		
				view->GetCamera().DollyX(-vec3.x,!GetCameraRelativeMode());
				// check for collision detection and correct camera position
				NavCheckCollision(oldPosition,FALSE,FALSE,FALSE);			
				view->GetCamera().DollyY(vec3.y,!GetCameraRelativeMode());
				NavCollisionGround(oldPosition);					
				}
				break;
						
			case ROTATE_MODE: 
				{		
				// get vector camera - fix point
				dir = 	m_NavCamera.FixPoint - view->GetCamera().position;
				// get length of vector
				old_dist = dir.Length();
				
				//  PAN-ROTATE
				// ensure look at angle is not bigger than 60 degrees
				if (((old_LookAt.y <= -0.8f && vec3.y > 0.0f) ||
					 (old_LookAt.y >=  0.8f && vec3.y < 0.0f)))
					vec3.y = 0.0f;
				
				// move camera up / right
				view->GetCamera().DollyX(-vec3.x,!GetCameraRelativeMode());
				NavCheckCollision(oldPosition,FALSE,FALSE,FALSE);			
				view->GetCamera().DollyY(vec3.y,!GetCameraRelativeMode());
				NavCollisionGround(oldPosition);					
				// now let camera look to fix point
				dir =  m_NavCamera.FixPoint - view->GetCamera().position;
				new_dist = dir.Length();
				// normalize new direction vector
				dir.Normalize();
				// set new target 
				view->GetCamera().target = view->GetCamera().position + dir * m_NavTargetDist;
				// correct distance
				view->GetCamera().DollyZ(new_dist-old_dist,!GetCameraRelativeMode());
				// now let camera look to fix point
				NavCameraFixToPoint();	
				}
				break;

			default :
				break;
			}
		

		}
}



void CGLViewCtrlCtrl::NavRotate(Point vec3)
{
	Point		oldPosition;
	float		old_dist, new_dist;
	Point		dir;
	Point		old_LookAt;
	int			steps;

	// dont animate to fixpoint if on valid sensor or anchor
	if (!m_wasDrag && (m_currentTSensor || m_currentAnchor))
		return;

	GCamera &camera = view->GetCamera();

	// no valid fix point -> search for one
	if (!m_NavCamera.FixPointValid)	
		if (view->Select(m_NavMousePos.x,m_NavMousePos.y,0,GView::INFO)) 
		{
			// animation may last 3 seconds
			steps = (int)(3000.0 / view->lastFrameTime);
			// start interpolation (camera rotation towards hit point)
			//NavStartRotPIP(view->lastHit->pw,50);
			NavStartRotPIP(view->lastHit->pw,steps);
			m_NavCamera.FixPoint = view->lastHit->pw;
			m_NavCamera.FixPointLocal = view->lastHit->p;
			m_currentMode = ROTATE_MODE;
			m_NavCamera.FixPointValid = TRUE;
		} else {
			// hg 
			view->UpdateBoundingBox();
			BBox &bbox = view->bbox;

			if (!bbox.IsEmpty()) {
				m_NavCamera.FixPoint = bbox.Center();
			} else 	m_NavCamera.FixPoint = camera.target;

			m_NavCamera.FixPointLocal = m_NavCamera.FixPoint;
			m_NavCamera.FixPointValid = TRUE;

			// animation may last 3 seconds
			steps = (int)(3000.0 / view->lastFrameTime);
			NavStartRotPIP(m_NavCamera.FixPoint,steps);
			m_currentMode = ROTATE_MODE;
			m_NavCamera.FixPointValid = TRUE;
	
		}

		if (vec3.z || m_NavCamera.ForceAction)
		{
		m_NavCamera.ForceAction = FALSE;

		oldPosition = view->GetCamera().position;

		old_LookAt = view->GetCamera().target - view->GetCamera().position;
		//targetDist = old_LookAt.Length();
		old_LookAt.Normalize();

		switch (m_currentMode) 
			{
			case ROTATE_MODE: 
				{		
				// get vector camera - fix point
				dir = 	m_NavCamera.FixPoint - view->GetCamera().position;
				// get length of vector
				old_dist = dir.Length();
				
				// CTRL is pressed -> PAN-ROTATE
				if (m_NavSlideState)
				{
					// ensure look at angle is not bigger than 60 degrees
					if (((old_LookAt.y <= -0.8f && vec3.y > 0.0f) ||
						 (old_LookAt.y >=  0.8f && vec3.y < 0.0f)))
						vec3.y = 0.0f;
					
					// move camera up / right
					view->GetCamera().DollyX(-vec3.x,!GetCameraRelativeMode());
					NavCheckCollision(oldPosition,FALSE,FALSE,FALSE);			
					view->GetCamera().DollyY(vec3.y,!GetCameraRelativeMode());
					NavCollisionGround(oldPosition);					

					// now let camera look to fix point
					dir =  m_NavCamera.FixPoint - view->GetCamera().position;
					new_dist = dir.Length();

					// normalize new direction vector
					dir.Normalize();
					
					// set new target 
					view->GetCamera().target = view->GetCamera().position + dir * m_NavTargetDist;
					// correct distance
					view->GetCamera().DollyZ(new_dist-old_dist,!GetCameraRelativeMode());
				}
				else
				{
					// move camera towards fixpoint
					if (vec3.y)	
						// ensure look at angle is not bigger than 60 degrees
						if (!(old_LookAt.y >=  0.8f && vec3.y > 0.0f))
							{
							// get vector camera - fix point
							dir = 	m_NavCamera.FixPoint - view->GetCamera().position;
							// get length of vector
							old_dist = dir.Length();
							
							float minDist = view->collisionDistanceLocal*2.0f;
							if (minDist <=0.0f) minDist = 1.0;
							
							//float minDist = 5.0 * view->visibilityLimitNearLocal;
							// if (minDist <0.5) minDist = 0.5;

							if ((old_dist - vec3.y) < minDist)  // clamp to minDist
								vec3.y = old_dist - minDist;

							view->GetCamera().DollyZ(vec3.y,!GetCameraRelativeMode());
							dir = 	m_NavCamera.FixPoint - view->GetCamera().position;
							// get length of vector
							old_dist = dir.Length();
							}
					// move camera left / right						
					if (vec3.x)
					{
						// move camera left / right
						view->GetCamera().DollyX(-vec3.x, !GetCameraRelativeMode());
						// get new vector camera - fix point
						dir = 	m_NavCamera.FixPoint - view->GetCamera().position;
						new_dist = dir.Length();
						// correct distance
						view->GetCamera().DollyZ(new_dist-old_dist,!GetCameraRelativeMode());
					}
					// check for collision detection and correct camera position
					NavCheckCollision(oldPosition,FALSE,TRUE,TRUE);			
					// check stay on ground
					NavCheckStayOnGround();
					// now let camera look to fix point
					NavCameraFixToPoint();	
					}
				}
				break;

			default :
				break;
			}
		}
	else		
		{
		if (!m_NavSlideState)
			// perform stay on ground check
			NavCheckStayOnGround();
		}
}



void CGLViewCtrlCtrl::NavExamine(Point vec3)
{
	Point		dir;
	Point		up;
	Point		right;
	float		dist;
	int			steps;

	// dont animate to fixpoint if on valid sensor or anchor
	if (!m_wasDrag && (m_currentTSensor || m_currentAnchor))
		return;
	GCamera &camera = view->GetCamera();

	if (!m_NavCamera.FixPointValid)	
	{
		view->UpdateBoundingBox();
		BBox &bbox = view->bbox;

		if (!bbox.IsEmpty()) {
			m_NavCamera.FixPoint = bbox.Center();
		} else 	m_NavCamera.FixPoint = camera.target;

		m_NavCamera.FixPointLocal = m_NavCamera.FixPoint;
		m_NavCamera.FixPointValid = TRUE;

		// animation may last 3 seconds
		steps = (int)(3000.0 / view->lastFrameTime);
		//NavStartRotPIP(m_NavCamera.FixPoint,50);
		NavStartRotPIP(m_NavCamera.FixPoint,steps);
		m_currentMode = ROTATE_MODE;
	}

	if (vec3.z)
		{
		switch (m_currentMode) 
			{
			case ROTATE_MODE: 
				{		
				if (vec3.x)
				{
				
					vec3.x *= m_NavSpeedRot_step / m_NavSpeed_step; // correct rotation

					// get vector fix point - camera
					dir = camera.position - m_NavCamera.FixPoint;
					up = camera.up;
					up.Normalize();
					dir.Rotate(up,vec3.x * NAV_ORBIT_SCALE);
					
					camera.position = m_NavCamera.FixPoint + dir;

					dir.Normalize();

					camera.target = camera.position - dir * m_NavTargetDist;
					camera.OnChanged();
				}

				if (vec3.y)
				{
					// CTRL is pressed -> PAN-ROTATE
					if (m_NavRotState)
					{
						vec3.y *= 5.0;

						float minDist = view->collisionDistanceLocal*2.0;
						if (minDist <=0.0f) minDist = 1.0;

						// get vector camera - fix point
						dir = 	m_NavCamera.FixPoint - camera.position;
						// get length of vector
						dist = dir.Length();
						if ((dist - vec3.y) < minDist) 
							vec3.y = dist - minDist;
						// move camera towards fixpoint
						camera.DollyZ(vec3.y,!GetCameraRelativeMode());
					}
					else
					{
						vec3.y *= m_NavSpeedRot_step / m_NavSpeed_step; // correct rotation

						// get vector fix point - camera
						dir = camera.position - m_NavCamera.FixPoint;

						right = camera.NormRight();
						
						dir.Rotate(right,vec3.y * NAV_ORBIT_SCALE);
						camera.position = m_NavCamera.FixPoint + dir;
						dir.Normalize();
						camera.target = view->GetCamera().position - dir * m_NavTargetDist;

						//up=NormCrossProd(camera.NormDirection(),right);	// fix up vector 

					}
					camera.OnChanged();
				}
				camera.CheckUpVector();
				// NavBalanceCamera(); hg does an align check
				}
				break;

			default :
				break;
			}
		}
}



void CGLViewCtrlCtrl::NavFly(Point vec3)
{
	Point		dir;
	float		speed;
	Point		oldPosition;
	
	oldPosition = view->GetCamera().position;
	
	// balance camera
	NavBalanceCamera();

	vec3.x *= m_NavSpeedRot_step / m_NavSpeed_step; // correct rotation
	vec3.y *= m_NavSpeedRot_step / m_NavSpeed_step; // correct rotation

	view->GetCamera().PanX(vec3.x * NAV_PAN_SCALE,!GetCameraRelativeMode()); 
	// up / down
	view->GetCamera().PanY(-vec3.y * NAV_PAN_SCALE,!GetCameraRelativeMode()); 


	speed = m_NavSpeed_step;

	speed *= 10.0;	


	// if down then faster, up slower
//	dir = view->GetCamera().NormTargetDirection();
//	speed -= dir.y / 2.0f;	

	// automatic forward
	view->GetCamera().DollyZ(speed,!GetCameraRelativeMode());

	NavCheckCollision(oldPosition,TRUE,TRUE,FALSE);

	// roll camera
	if (vec3.x)
		view->GetCamera().Roll(vec3.x / 60.0f); 
}


void CGLViewCtrlCtrl::NavJumpTo(Point vec3)
{
	POINT	mousepos;

 	// get screen position of cursor
	// and convert to client area coordinates
	GetCursorPos(&mousepos);
	ScreenToClient( &mousepos);
	NavBeamTo(mousepos);

}

void CGLViewCtrlCtrl::NavCheckCollision(Point oldPosition, BOOL rotate, BOOL slide, BOOL ground)
{
	Point newPosition;
	Point p,n;
	

	// check for collision detection 
	// collision detection ON ?

	if (view->collisionDetection) {
		newPosition = view->GetCamera().position;
		// p = schnittpunkt - distance to object, n = normalenvektor
		if (view->CollisionDetect(oldPosition,newPosition,p,n)) 
		{
			view->GetCamera() += (p-newPosition);
			// could optiomize, no redraw if camera didn't change 
			// Message("Collision !",COLLISION_MESSAGE);
			SetCollisionState(TRUE);
		}
		else 
			SetCollisionState(FALSE);

	} else 
	  SetCollisionState(FALSE);

}




/*

// variante 3

void CGLViewCtrlCtrl::NavCheckCollision(Point oldPosition, BOOL rotate, BOOL slide, BOOL ground)
{
Point	newPosition;
Point	p,n, p1, n1, newp;
Point	dir, dirn;
float	angle;	
Point	move_dir;
float	move_dist;
Point	up;
float	dist;
float	cDistNew;
Point	new_dir;
Point	calcPosition;
Point	hvec;
Point	vec1, vec2, vec3;
float	rotation;
float	coll_dist;


CString			fname;
CString			fext;
char			str[256];


	// check for collision detection 
	// collision detection ON ?
	if (view->collisionDetection && !m_NavFollowObject) 
		{
		
		// this is current camera position
		newPosition = view->GetCamera().position;
		
		// no movement
		if (newPosition == oldPosition)
			return;



		// current camera direction
		dir =  newPosition - oldPosition;
		
		// length of the distance we wanted to go
		dist = dir.Length();
		
		// normalized camera direction vector
		dir.Normalize();
		
		
		// strech distance for collision detection
		calcPosition = newPosition + dir * 10.0;
//		calcPosition = newPosition;
		
		coll_dist = GetCollisionDistance();

		SetCollisionDistance(0.0f);
		
		// p = schnittpunkt - distance to object, n = normalenvektor
		if (view->CollisionDetect(oldPosition,calcPosition,p,n)) 
			{

				n.x = -n.x;
				n.y = -n.y;
				n.z = -n.z;


			// reset collision distance
			SetCollisionDistance(coll_dist);

			// opposite direction vector... looks to the camera position
			dirn.x = -dir.x;
			dirn.y = -dir.y;
			dirn.z = -dir.z;

			// angle between opp. direction vector and normal vector of polygon.
			// arccos(angle) is real angle. 	
			// angle is between 0.0 and 1.0	
			
			// senkrechtes Auftreffen -> vermeide Division durch Null
			if (dirn == n)
				angle = 0.01;
			else
				angle = DotProd(dirn, n);
/-		
			if (abs(angle) > 1.57)
				{
				n.x = -n.x;
				n.y = -n.y;
				n.z = -n.z;
				if (dirn == n)
					angle = 0.01;
				else
					angle = DotProd(dirn, n);
				}
-/
 
			// calculate intersection distance with collision sector
			// collision Abstand ist 3.0
//			cDistNew = 3.0 / angle;		
			//cDistNew = 2.0 / angle;		
			cDistNew = coll_dist / angle;		

			// intersection point - collision distance = new intersection point
			newp = p - dir * cDistNew;

			// now check wether current camera position is behind collision sector
			// get distance old position - intersection point
			
		


			// vector polygon intersection to collision zone intersection
			vec1 = newp - p;
			vec1.Normalize();

			// vector camera position to collision zone intersection
			vec2 = newp - newPosition;
			vec2.Normalize();


			// vector visible (old) camera position to collision zone intersection
			//vec3 = newp - oldPosition;
			//vec3.Normalize();
			
			// camera destination is not behind collision zone
			if (!Equal(vec1, vec2, 0.1))
				{
/-
				//m_NavCollision.LastCollision = FALSE;
			fname = "No Collision !!"; 

			Message(fname);
-/
				return;
				}

//			SetCursorMode(COLLISION_CURSOR);

			// calculate intersection distance with collision sector
			//	cDistNew = 3.1 / angle;		

			// intersection point - collision distance = new intersection point
			//	newp = p - dir * cDistNew;

			// normal vector of polygon-normal and moving direction
			up = NormCrossProd(n, dirn);
			
			// if still in collision then keep rotation to solve corner problem
			if (m_NavCollision.LastCollision)
				{
				// angle between old and new rotation axis > 90 degrees
				if (DotProd(m_NavCollision.LastUp, up) < 0.0)
					up = m_NavCollision.LastUp;
				}


			
			// moving direction is parallel to polygon
			// builds one plane together with camera direction and polygon normal
			move_dir = NormCrossProd(n, up);
			

			rotation = 0.1;

			if (ground)
				move_dir.y = 0.0;

			move_dir.Normalize();

			// check if angle > 60 degrees
			// move camera away
			if (angle < 0.5)
				{
				newp = oldPosition;
				rotation = 0.1;
//				rotation = 0.02;
				move_dir.Rotate(up, rotation);
				move_dist = dist;
				}
			else
				{
				// in dependence of length we want to go and intersection angle
				// we calculate a moving distance (slide along) 0.0*dist to 1.0*dist
				//move_dist = ((1.0f / angle) / 5.0f) * dist;
				move_dist = (1.0f / angle) * dist;
				}
		
			if (!slide)
				move_dist = 0.0;


			// give a proposal for a rotation angle
			// rotate dir-vector arount up vector with 5.7 degrees		

			dirn = view->GetCamera().NormTargetDirection();
/-
			if (rotate)
				{
				dirn.Rotate(up, rotation);
				}
-/
			if (ground)
				dirn.y = 0.0;
			
			dirn.Normalize();

			// move camera back to intersection point + distance and 
			// slide camera along polygon
			// 'view->collisionDistance' is distance to intersection point

			//view->GetCamera() += (p-newPosition + move_dir * move_dist);
			view->GetCamera().position  = (newp + move_dir * move_dist);

			view->GetCamera().target = view->GetCamera().position + dirn * m_NavTargetDist;

			if (view->stayOnGround)
				NavBalanceCamera();


			m_NavCollision.LastCollision = TRUE;
			m_NavCollision.LastNormal    = n;
			m_NavCollision.LastUp        = up;

			}
		else 
			// no collision
			{
			m_NavCollision.LastCollision = FALSE;

			SetCollisionDistance(coll_dist);

			if (lastMessageType == COLLISION_MESSAGE) 
				Message("");
//			if (lastCursorMode == COLLISION_CURSOR)
//				SetCursorMode(GetCameraMode());
				//SetCursorMode(MOVING_CURSOR);
			}
		}
}

*/



void CGLViewCtrlCtrl::NavCheckStayOnGround()
{
	Point	newPosition,hitPosition,hitNormal;

	// stay on ground ON ?	
	if (view->stayOnGround && !m_NavFollowObject) 
		{
		// gravity IP running
		if (m_NavIPActive & NAV_GRAVITY_IP)
		{
			NavStepGravityIP();
		}
		else
		// IP not running
			{
			// hitPosition = point of intersection with ground
			// newPosition = hitPosition + avatar height
		//	if (view->GroundDetect(view->GetCamera().position,
		//		newPosition,hitPosition,hitNormal)  >0) 
				// there is a ground below
		//		{
				// start gravity IP
				NavStartGravityIP(50);
				// gravity IP running
				if (m_NavIPActive & NAV_GRAVITY_IP)
					// do the first step
					NavStepGravityIP();
		//		}
			}
/*
		if (view->GroundDetect(view->GetCamera().position,
			newPosition,hitPosition,hitNormal)  >0) 
			{
			if ((view->GetCamera().position.y - newPosition.y) > 10.0f)
				NavStartGravityIP(newPosition);
			else
				view->GetCamera() += (newPosition-view->GetCamera().position);

			}
*/
		}
}

void CGLViewCtrlCtrl::NavCollisionGround(Point old_position)
{
	Point	newPosition,hitPosition,hitNormal;
	Point	dir;
		
	if (view->collisionDetection) 
	{
		if (view->GroundDetect(old_position, newPosition,hitPosition,hitNormal)  >0) 
			if (view->GetCamera().position.y < newPosition.y)
			{
				dir = view->GetCamera().NormTargetDirection();
				view->GetCamera().position.y = newPosition.y;
				view->GetCamera().target = view->GetCamera().position + dir * m_NavTargetDist;
			}
	}
}




void CGLViewCtrlCtrl::NavForceStayOnGround()
{
Point		dir;
Point		newPosition,hitPosition,hitNormal;
Point		camPos;

	// stay on ground ON ?	
	if (view->stayOnGround) 
		{
		camPos = view->GetCamera().position;
	
		// hitPosition = point of intersection with ground
		// newPosition = hitPosition + avatar height
		// if hit
		if (view->GroundDetect(camPos, newPosition,hitPosition,hitNormal)  >0) 
			{
			// camera down to ground
			//view->GetCamera().DollyY(newPosition.y - camPos.y, !GetCameraRelativeMode());
			view->GetCamera() += (newPosition-view->GetCamera().position);

			}
		}
}


void CGLViewCtrlCtrl::NavCameraFixToPoint()
{
Point		dir;

	if (m_NavCamera.FixPointValid)
		{
		// now let camera look to fix point
		dir =  m_NavCamera.FixPoint - view->GetCamera().position;
		// normalize new direction vector
		dir.Normalize();
		// set new target 
		view->GetCamera().target = view->GetCamera().position + dir * m_NavTargetDist;
		}
}

void CGLViewCtrlCtrl::NavStraightenUp()
{

	NavStartRotVIP();
}


int CGLViewCtrlCtrl::NavCheckKeyState(BOOL checkAnchor)
{

	m_NavOldSlideState  = m_NavSlideState;				

	// force navigation	
	m_NavCNTLState  = ::GetKeyState(VK_CONTROL) &  0x8000;

	//if 	(m_NavCNTLState)
		//TRACE("............................................m_NavCNTLState\n");
	// slide (pan)
	//alt
		
	//m_NavSlideState	= ::GetKeyState(VK_MENU) &  0x8000;		
	if (view->allowAnyNavigation) 
	m_NavSlideState	= ::GetKeyState((UINT)'S') &  0x8000 |
		::GetKeyState(VK_MENU) &  0x8000 | m_NavSlideStatePanel;				
	else m_NavSlideState = FALSE;

	// speed up
	m_NavSpeedState = ::GetKeyState(VK_SHIFT) & 0x8000;

	// get fixpoint for rotation
	//	m_NavRotState	= ::GetKeyState(VK_TAB) & 0x8000;	

	if (view->allowAnyNavigation) {
		m_NavFollowState = ::GetKeyState(VK_SPACE) & 0x8000;	
		m_NavRotState	= ::GetKeyState((UINT)'R') & 0x8000;	
		m_NavPointState = ::GetKeyState((UINT)'J') & 0x8000;
	} else {
		m_NavFollowState = FALSE;
		m_NavRotState = FALSE;
		m_NavPointState = FALSE;
	}
	
	m_NavUPState	= ::GetKeyState(VK_UP) & 0x8000 | m_NavUPStatePanel;
	m_NavDOWNState	= ::GetKeyState(VK_DOWN) & 0x8000 | m_NavDOWNStatePanel;
	m_NavLEFTState  = ::GetKeyState(VK_LEFT) & 0x8000 | m_NavLEFTStatePanel;
	m_NavRIGHTState = ::GetKeyState(VK_RIGHT) & 0x8000 | m_NavRIGHTStatePanel;

	if (m_NavCNTLState && (m_NavUPState || m_NavDOWNState) ) {
		m_cameraModeOld = GetCameraMode();
		SetCameraMode(PAN_MODE);
	}

	//	m_NavLBUTState  = ::GetKeyState(VK_LBUTTON) & 0x8000;	

	NavCheckCursor(checkAnchor);

	return 0;

}






void CGLViewCtrlCtrl::NavCheckCursor(BOOL checkAnchor)
{
	int		mode = -1;
	int		ret = 0;
	POINT	point;
	BOOL	forcedNav;

	// get screen position of cursor
	// and convert to client area coordinates
	GetCursorPos(&point);

	ScreenToClient(&point);
	
	// get window rectangle 	
	GetClientRect( &m_NavClientRect);
	
	//tv: switch aus if raus
	//brauche mode fuer nav-feedback
	forcedNav = (m_NavPointState || m_NavCNTLState || m_NavFollowState || (m_NavTimerRunning && m_wasDrag));

	if (forcedNav)
		m_wasDrag = TRUE;
			
	switch (GetCameraMode()) 
			{
			case WALK_MODE:
					{
					if (m_NavPointState)
						mode = NAV_JUMP_TO;
					else
					if (m_NavFollowState)
						mode = NAV_FOLLOW;
					else
					if (m_NavSlideState && m_NavRotState)
						mode = NAV_WALK_ROT_SLIDE;
					else
					if (m_NavSlideState)
						mode = NAV_WALK_SLIDE;
					else
					if (m_NavRotState)
						mode = NAV_WALK_ROT;
					else
					if (forcedNav)
						mode = NAV_WALK;
					}
					break;
			case PAN_MODE:
					{
					if (m_NavPointState)
						mode = NAV_JUMP_TO;
					else
					if (m_NavFollowState)
						mode = NAV_FOLLOW;
					else
					if (m_NavSlideState && m_NavRotState)
						mode = NAV_WALK_ROT_SLIDE;
					else
					if (m_NavSlideState)
						mode = NAV_WALK_SLIDE;
					else
					if (m_NavRotState)
						mode = NAV_WALK_ROT;
					else
					if (forcedNav)
						mode = NAV_PAN;
					}
					break;
			case DOLLY_XY_MODE:
					{
					if (m_NavPointState)
						mode = NAV_JUMP_TO;
					else
					if (m_NavFollowState)
						mode = NAV_FOLLOW;
					else
					if (m_NavRotState)
						mode = NAV_SLIDE_ROT;
					else
					if (forcedNav)
						mode = NAV_SLIDE;
					}
					break;
			case ORBIT_MODE:
					{
					//hg
					if (m_NavPointState)
						mode = NAV_JUMP_TO;
					else
					if (m_NavFollowState)
						mode = NAV_FOLLOW;
					else
					// hg 

					if (m_NavRotState)
						mode = NAV_EXA_SLIDE;
					else
					if (forcedNav)
						mode = NAV_EXAMINE;
					}
					
					break;
			case ROTATE_MODE:
					{
					if (m_NavPointState)
						mode = NAV_JUMP_TO;
					else
					if (m_NavFollowState)
						mode = NAV_FOLLOW;
					else
					if (m_NavSlideState)
						mode = NAV_ROT_SLIDE;
					else
					if (forcedNav)
						mode = NAV_ROTATE;
					}
					break;
			case FLY_MODE:
					// hg 
					if (m_NavPointState)
						mode = NAV_JUMP_TO;
					else
					if (m_NavFollowState)
						mode = NAV_FOLLOW;
					else
					if (m_NavSlideState && m_NavRotState)
						mode = NAV_WALK_ROT_SLIDE;
					else
					if (m_NavSlideState)
						mode = NAV_WALK_SLIDE;
					else
					if (m_NavRotState)
						mode = NAV_WALK_ROT;
					// hg

					if (forcedNav)
						mode = NAV_FLY;
					break;
			case JUMP_TO_MODE:
					mode = NAV_JUMP_TO;
					break;
			default :
					break;
			}


	// 8.12.98 hg display cant walk cursor 
	if (m_collisionState && forcedNav )
		mode = NAV_COLLISION;


	// within client area ?
	if (point.x > 0 && point.y > 0 && point.x < m_NavClientRect.right && point.y < m_NavClientRect.bottom)
	{
		m_NavIsWithinRect = TRUE;


		//tv: switch ist hochgewandert

		//lastMessage = ""; // clear last message to detect messages sent by scripts etc.

		// if not in navigation then check for anchor
		if ( mode == -1  && checkAnchor)
		{
			
			ret=view->Select(point.x,point.y,0,GView::INFO_WWWAnchor);

			if (ret & SELECT_ANCHOR_BIT) 
			{
				mode = NAV_ANCHOR;  // 
				if (ret & SELECT_SENSOR_BIT) 
					mode = NAV_ANCHOR_SENSOR;
			} 	
			else { 
				mode = GetCameraMode();
				if (mode == PAN_MODE) 
					mode = NAV_PAN;

				if (ret & SELECT_SENSOR_BIT)
					mode += NAV_NONE_SENSOR;
				}
			}

/*
			if (ret & SELECT_ANCHOR_BIT)
				mode = NAV_ANCHOR;
			else
				mode = GetCameraMode();


			if (ret & SELECT_SENSOR_BIT) {
				mode += NAV_NONE_SENSOR;
			}
*/
//		if (mode != m_NavCursorModeOld)
		if (view->PanelOk() && view->panel->Inside(point))
		{
			mode = NAV_PANEL_CURSOR;
			m_NavCursorShow = TRUE;
		}



		if (GetCursor() != NavCursors[mode] && m_NavCursorShow)
		{
			SetCursorMode(mode);
//			if (mode != NAV_FOLLOW) 
			m_NavCursorModeOld = mode;
		}
	}
	else
		{
		m_NavIsWithinRect = FALSE;
		m_NavCursorModeOld = -2;
		}


	if (view->PanelOk())
		if (!view->panel->Active() && !view->panel->Inside(point) && !m_navPanelDrag)
			//panel gives feedback
			view->panel->setNavigationMode(mode);
}

void CGLViewCtrlCtrl::NavResetFollowObject()
{
	if (view) 
		view->SetFollowObjectOff();
	m_NavFollowObject = FALSE;

	NavResetCursorBlinking();
}



void CGLViewCtrlCtrl::NavResetCursorBlinking()
{
//int		count;
	m_NavCursorBlkCount = 0;
	m_NavCursorShow = TRUE;


	if (GetCursor() == NavCursors[NAV_FOLLOW])
		SetCursorMode(m_NavCursorModeOld);


/*
	// get internal visibility counter
	count = ShowCursor(TRUE);

	if (count > 0)
		while (ShowCursor(FALSE));
	else
	if (count < 0)
		while (ShowCursor(TRUE) < 0);
*/

}



/**-------------------------------------------------------------------------
  CGLViewCtrlCtrl::NavCalculateTile

  Called by:	Called by NavMain only if left mouse button pressed
  
  Description:	Calculates the tile where the mouse cursor is within navigation window.

--------------------------------------------------------------------------*/
void CGLViewCtrlCtrl::NavCalculateTile(Point *ptile)
{
	Point		point;	   
	Point		tile;
 	
 	

	// check wether <CONTROL> is pressed 
	//cntl_state =  (::GetKeyState(VK_MENU) &  0x8000); 
	//shift_state = (GetKeyState(VK_SHIFT) &  0x8000); 
	
	
	// if state of navigation has changed (CNTL now pressed or depressed)
	// then start with new navigation centre	 

	if (m_NavOldSlideState ^ m_NavSlideState)
		{
		m_NavOldSlideState = m_NavSlideState;		
		// centre mouse to the centre of client rectangle
		point.x = m_NavClientRect.right / 2;
		point.y = m_NavClientRect.bottom / 2;
			
		m_NavMouseDiff.x = m_NavMousePos.x - point.x;		
		m_NavMouseDiff.y = m_NavMousePos.y - point.y;		

		//mousepos = point;
		
		// get screen coordinates
		//ClientToScreen( &mousepos);
			
		// set the cursor to the centre
		//::SetCursorPos(mousepos.x, mousepos.y);
		}
	 else


		{
		// get screen position of cursor
		// and convert to client area coordinates
		// ::GetCursorPos(&point);
		// ScreenToClient(&point);
		

		point.x = m_NavMousePos.x - m_NavMouseDiff.x;
		point.y = m_NavMousePos.y - m_NavMouseDiff.y;
		} 
	
	/*
	// if mouse has moved then reset fixing
	if ((point.x != client_rect.right / 2) || (point.y != client_rect.bottom / 2))
		mCurNavMode &= ~0x2;
	*/
	if (m_navPanelDrag && InsideSphere)
	{
			tile.x = 0;
			tile.y = 0;
			return;

/*		if (point.x > -24 && point.x <24)
			point.x = 0;
		if (point.y > -24 && point.y <24)
			point.y = 0;
*/
  }
	else
	{
	 
		// divide the client area into horizontal and vertical tiles 
		// hg float ?
		tile.x = point.x / ((float) m_NavClientRect.right / (float) 41) - 21;
		tile.y = point.y / ((float) m_NavClientRect.bottom / (float) 41) - 21;
	}
#ifdef _DEBUG_NAVIGATION
	TRACE("CGLViewCtrlCtrl::NavCalculateTile:  tile %g %g \n", tile.x, tile.y);
#endif _DEBUG_NAVIGATION

	float zeroClamp = 3.0;

	// extend ground zero area

	if (m_navPanelDrag) {
		zeroClamp = 1.0;
	}

	{
		if (tile.x < -zeroClamp)
			tile.x += zeroClamp;
		else
			if (tile.x > zeroClamp)
			tile.x -= zeroClamp;
		else
			tile.x = 0;
	
		if (tile.y < -zeroClamp)
			tile.y +=zeroClamp;
		else
			if (tile.y > zeroClamp)
				tile.y -= zeroClamp;
		else
			tile.y = 0;
	}

	if (m_navPanelDrag) {	// scale movement in panel 
		tile.x *=4;
		tile.y *=4;
	}

// 	TRACE("hg tile %g %g %g \n",tile.x,tile.y,tile.z);
	*ptile      = tile;
}




BOOL CGLViewCtrlCtrl::NavStartMotionIP(Point p, int steps)
{
	Point		dir;


	// if no navigation interpolator running then start Nav IP Timer
	if (!m_NavIPActive)
	{
		NavStartTimer();
	}	
	else
	// if already motion interpolator running then return
	if (m_NavIPActive & NAV_MOTION_IP)
		return FALSE;

	
	// motion interpolator now active
	m_NavIPActive |= NAV_MOTION_IP;

	// do 10 steps
	m_NavMotionIP.Steps   = steps;
	// current step is 0
	m_NavMotionIP.CurStep = 0;
	// get start position
	m_NavMotionIP.StartPoint = view->GetCamera().position;
	// get end position
	m_NavMotionIP.EndPoint = p;


	return TRUE;

}


BOOL CGLViewCtrlCtrl::NavStartGravityIP(int steps)
{
	Point		dir;
	Point		newPosition,hitPosition,hitNormal;
	Point		camPos;


	camPos = view->GetCamera().position;
//	camPos.y += 5.0;
	
	// hitPosition = point of intersection with ground
	// newPosition = hitPosition + avatar height
	// if hit
	if (view->GroundDetect(camPos, newPosition,hitPosition,hitNormal)  >0) 
		{
//		camPos.y -= 5.0;


		// if near ground then do not animate but set camera to ground
		if ((camPos.y - newPosition.y) < 5.0f)
			{
			// camera down to ground
			view->GetCamera().DollyY(newPosition.y - camPos.y, !GetCameraRelativeMode());
			// check fixing and fix to point
			if (m_currentMode == ROTATE_MODE)
				{
				if (!(m_NavIPActive & NAV_ROT_P_IP)) 
					NavCameraFixToPoint();
				// balance camera
				NavBalanceCamera();
				}
			else
				// ground is reached so ensure camera stands up		
				{
					// generate look at vector parallel to ground
					NavStartRotVIP();
					//NavStepRotVIP();
				}

			return FALSE;
			}
		else
			{
			// if no navigation interpolator running then start Nav IP Timer
			if (!m_NavIPActive)
				{
				NavStartTimer();
				}	
			else
			// if already gravity interpolator running then return
			if (m_NavIPActive & NAV_GRAVITY_IP)
				return FALSE;

			
			// gravity interpolator now active
			m_NavIPActive |= NAV_GRAVITY_IP;

			// do steps
			m_NavGravityIP.Steps   = steps;
			// current step is 0
			m_NavGravityIP.CurStep = 0;
			// get start position
			m_NavGravityIP.StartPoint = camPos;
			// get end position
			m_NavGravityIP.EndPoint = newPosition;


			return TRUE;
			}
		}
	else
		// no ground below
		return FALSE;
}



BOOL CGLViewCtrlCtrl::NavStartRotPIP(Point p, int steps)
{
Point		dir;
Point		targetP;
//float		len;

	dir = view->GetCamera().target - view->GetCamera().position;
	
	// if no navigation interpolator running then start Nav IP Timer
	if (!m_NavIPActive)
		{
		NavStartTimer();
		// calculate distance camera position - target
		//m_NavTargetDist = dir.Length();
		}	
	else
	// if already rotation to point interpolator running then return
	if (m_NavIPActive & NAV_ROT_P_IP)
		return FALSE;

	
	// rotation to point interpolator now active
	m_NavIPActive |= NAV_ROT_P_IP;

	// do 10 steps
	m_NavRotPointIP.Steps   = steps;
	// current step is 0
	m_NavRotPointIP.CurStep = 0;
	// get start position
	
	
	dir.Normalize();
	targetP = p - view->GetCamera().position;	
	//len = targetP.Length();

	//m_NavRotPointIP.StartPoint = view->GetCamera().target;
	m_NavRotPointIP.StartPoint = view->GetCamera().position + dir * targetP.Length();
	// get end position
	m_NavRotPointIP.EndPoint = p;


	return TRUE;

}



// in this implementation function is a pure camera stant up
// input vector is ignored
// interpolation is between current camera up vector and 0/0/1


BOOL CGLViewCtrlCtrl::NavStartRotVIP()
{
Point		dir;


	// first check if look at vector is nearly parallel to ground
	// if so then dont start interpolation but set look at directly
	// get normalized look at vector
	dir = view->GetCamera().NormTargetDirection();


	// check if end position already reached
	if (dir.y == 0.0f)
		{
		// balance camera
		NavBalanceCamera();

		return TRUE;
		}
/*
	else
	// difference not big enough to start interpolator
	if (dir.y >= -0.05f && dir.y <= 0.05f)
		{
		dir.y = 0.0f;
		dir.Normalize();
		// set new target 
		view->GetCamera().target = view->GetCamera().position + dir * m_NavTargetDist;
		
		// balance camera
		NavBalanceCamera();

		return TRUE;
		}
*/
	else
		// big difference so start interpolator
		{
		// if no navigation interpolator running then start Nav IP Timer
		if (!m_NavIPActive)
			NavStartTimer();
		else
		// if already rotation interpolator running then return
		if (m_NavIPActive & (NAV_ROT_V_IP | NAV_ROT_P_IP))
			return FALSE;
		
		// rotation to vector interpolator now active
		m_NavIPActive |= NAV_ROT_V_IP;

		// current step is 0
		m_NavRotVecIP.CurStep = 0;
		// get normalized camera at vector
		dir = view->GetCamera().target - view->GetCamera().position;
		dir.Normalize();

		if (dir.y >= 0)
			{
			m_NavRotVecIP.Angle = -0.05f;
			m_NavRotVecIP.Start = 1;
			}
		else
			{
			m_NavRotVecIP.Angle = 0.05f;
			m_NavRotVecIP.Start = -1;
			}

		// balance camera
		//NavBalanceCamera();

		return TRUE;
		}









}



void CGLViewCtrlCtrl::NavStopIP(int ip)
{
	// reset ip interpolator
	m_NavIPActive &= ~ip;

	// no more interpolator running
//	if (!m_NavIPActive)
//		NavStopIPTimer();

}



void CGLViewCtrlCtrl::NavStepMotionIP()
{
Point		nextPos;
Point		nextTarget;
Point		dir;
Point		dist;
int			stepOffset;


	m_NavMotionIP.CurStep++;

	switch (m_NavMotionIP.StopWhere)
		{
		case 0:
				stepOffset = m_NavMotionIP.Steps;
				break;
		case 1:
				stepOffset = (m_NavMotionIP.Steps * m_NavMotionIP.StopPercentage) / 100;				
				break;
		default:
				stepOffset = 0;
				break;
		}
		
	// end of steps reached -> reset interpolator
	if (m_NavMotionIP.CurStep > stepOffset)
		{
		m_NavMotionIP.CurStep = 0;
		m_NavIPActive &= ~NAV_MOTION_IP;
		return;
		}

	// get current target direction (Camera Look At vector)
	dir = view->GetCamera().target - view->GetCamera().position;
	// normalize camera look at vector
	dir.Normalize();

	dist = (m_NavMotionIP.EndPoint - m_NavMotionIP.StartPoint) / m_NavMotionIP.Steps;

	// calculate new camera position
	nextPos = m_NavMotionIP.StartPoint + m_NavMotionIP.CurStep * dist;

	// set new camera position
	view->GetCamera().position= nextPos;

	// set new target 
	view->GetCamera().target = nextPos + dir * m_NavTargetDist;

	// check stay on ground and put camera immediately down to ground
	NavForceStayOnGround();
}

void CGLViewCtrlCtrl::NavStepGravityIP()
{
Point		nextPos;
Point		nextTarget;
Point		dir;
float		dist;
Point		newPosition,hitPosition,hitNormal;
Point		curPoint;


	curPoint = view->GetCamera().position;

	
	m_NavGravityIP.CurStep++;


	// end of steps reached -> reset interpolator
	// final y-position reached | no Stay On Ground | no Ground below
	if ((curPoint.y <= m_NavGravityIP.EndPoint.y) ||
		!view->stayOnGround						  ||
		(view->GroundDetect(curPoint, newPosition,hitPosition,hitNormal) == 0))
	{
		m_NavGravityIP.CurStep = 0;
		m_NavIPActive &= ~NAV_GRAVITY_IP;
		return;
	}
	
	dist = (m_NavGravityIP.StartPoint.y - m_NavGravityIP.EndPoint.y) / m_NavGravityIP.Steps;

	dist *= m_NavGravityIP.CurStep;

	// we have reached the ground
	if ((curPoint.y - dist) <= newPosition.y)
		{
		dist = curPoint.y - newPosition.y;
		view->GetCamera().DollyY(-dist, !GetCameraRelativeMode());
		m_NavGravityIP.CurStep = 0;
		m_NavIPActive &= ~NAV_GRAVITY_IP;

		// if not fix to point mode then stand up camera
		if (!(m_currentMode == ROTATE_MODE))
			// ground is reached so ensure camera stands up		
		{
			// generate look at vector parallel to ground
			NavStartRotVIP();
			//NavStepRotVIP();
		}
	}
	else
		// camera down to next point
		view->GetCamera().DollyY(-dist, !GetCameraRelativeMode());
		

	if (m_currentMode == ROTATE_MODE)
		NavCameraFixToPoint();

	// gravity was checked
	m_NavGravityChecked = TRUE;

}

void CGLViewCtrlCtrl::NavStepRotPIP()
{
	Point		nextPos;
	Point		nextTarget;
	Point		dir;
	Point		step;

	
	m_NavRotPointIP.CurStep++;
	
	// end of steps reached -> reset interpolator
	if (m_NavRotPointIP.CurStep > m_NavRotPointIP.Steps)
	{
		m_NavRotPointIP.CurStep = 0;
		m_NavIPActive &= ~NAV_ROT_P_IP;
		return;
	}

	step = (m_NavRotPointIP.EndPoint - m_NavRotPointIP.StartPoint) / m_NavRotPointIP.Steps;

	// calculate new position on line start-hit
	nextPos = m_NavRotPointIP.StartPoint + m_NavRotPointIP.CurStep * step;

	// get new target direction (Camera Look At vector)
	dir = nextPos - view->GetCamera().position;
	// normalize new direction vector
	dir.Normalize();
	
	// set new target 
	view->GetCamera().target = view->GetCamera().position + dir * m_NavTargetDist;
}

// in this implementation function is a pure camera stand up
// input vector is ignored
// interpolation is between current camera up vector and 0/0/1

void CGLViewCtrlCtrl::NavStepRotVIP()
{
	Point		lookAt;
	BOOL		stop;


	// rotate camera around camera right vector
	view->GetCamera().PanY(m_NavRotVecIP.Angle,!GetCameraRelativeMode()); 

	// get normalized look at vector
	lookAt = view->GetCamera().NormTargetDirection();

	stop = FALSE;

	// check if end position already reached
	if (m_NavRotVecIP.Start <= 0)
	{
		if (lookAt.y >= 0.0f)
			stop = TRUE;
	}
	else
	{
		if (lookAt.y < 0.0f)
			stop = TRUE;
	}


	if (stop)
	{
		lookAt.y = 0.0f;
		lookAt.Normalize();
		// set new target 
		view->GetCamera().target = view->GetCamera().position + lookAt * m_NavTargetDist;
		// end of steps reached -> reset interpolator
		m_NavIPActive &= ~NAV_ROT_V_IP;
		
		NavBalanceCamera();
	}
}


void CGLViewCtrlCtrl::NavBalanceCamera()
{
	Point		right;
	Point		at;
	Point		up;

	GCamera & camera = view->GetCamera();

	// get camera right vector
	right = camera.NormRight();

	// camera coplanar to x - z plane
	right.y = 0.0f; // hg ????

	// calculate new camera up vector
	up = CrossProd(camera.NormDirection(), right);

	// normalize up vector
	up.Normalize();
	if (up.Length() > 0.9) // hg
		camera.up = up;
} 

void CGLViewCtrlCtrl::NavStartTimer()
{
	m_NavTimerRunning = TRUE;
}

void CGLViewCtrlCtrl::NavStopTimer()
{
	if (view->PanelOk()) view->panel->setNavigationMode(-1);
	m_NavTimerRunning = FALSE;
}


// Stop navigation action, e.g. because new viewpoint was bound 
void CGLViewCtrlCtrl::OnStopNavigation() 
{
	if (view->AnimatingCamera()) 
		view->AnimateCameraStop();
	// if any navigation interpolator active then stop
	else if (m_NavIPActive)
		m_NavIPActive = 0;
	else if (m_NavFollowObject)
		NavResetFollowObject();

	NavEnd();
}
