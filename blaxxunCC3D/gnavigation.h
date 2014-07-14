/************************************************************************************************




			inputs 0: keystrokes, mousemovement, mouseclick, joystick, upcoming devices
											I
											I
											I
											V
 
						//////////////////////////////////////////////			//////////////////////////////////////
						//											//			//									//
						//				event handler				//<========>//			mapping table			//
						//											//			//									//
						//////////////////////////////////////////////			//////////////////////////////////////
								I	I			I		I											
								I	I			I		I						
								I	I			I		I						
								I	I			V		I						
								I	//////////////		I
								I	//			//		I
other events (help button)<=====I===//  panel	//		I
								I	//			//		I
								I	//////////////		I												
								I	I			I		I						
								V	V			V		V
		EAI===============>state change		deltaX, deltaY, deltaZ	(mouse + joystick + cursor keys)	
								I	I			I		I
								I	I			I		I
								V	V			V		V
						//////////////////////////////////////////////			///////////////////
						//											//			//
						//				navigator					//			//
	timer ticks	==>		//											//			//sensor handler
						//		state(walk, rotate etc)				//			//	
<==check collision==>	//											//			//
						//											//			//
						//////////////////////////////////////////////			//////////////////
								I						I
								I						I
								V						V
							setNavigationMode	setCameraPosition				
								I						I
								I						I
								V						V

								cursor					camera




/************************************************************************************************/

// Cursor modes 
enum NavCursorMode {
	NAV_NONE,
	NAV_WALK,
	NAV_SLIDE,
	NAV_ROTATE,
	NAV_EXAMINE,
	NAV_FLY,
	NAV_WALK_ROT,
	NAV_WALK_SLIDE,
	NAV_WALK_ROT_SLIDE,
	NAV_ROT_SLIDE,
	NAV_SLIDE_ROT,
	NAV_EXA_SLIDE,
	NAV_JUMP_TO,
	NAV_FOLLOW,
	NAV_ANCHOR,
	NAV_NONE_SENSOR,
	NAV_WALK_SENSOR,
	NAV_SLIDE_SENSOR,
	NAV_ROTATE_SENSOR,
	NAV_EXAMINE_SENSOR,
	NAV_FLY_SENSOR,
	NAV_ANCHOR_SENSOR,
	NAV_PANEL_CURSOR,
	NAV_PAN,
	NAV_COLLISION,
	NAV_CURSOR_MAX,
};



	
// navigation modes 
enum MoveMode {
				  NULL_MODE,

				  // viewer modes
				  WALK_MODE,		// move in/out look left/right
				  DOLLY_XY_MODE,	// DRAG up/down left/right
				  ROTATE_MODE,		// rotate around 
				  ORBIT_MODE,		// examine
				  FLY_MODE,			// like walk but toggled with 2 clicks 

				  ZOOM_LENS_MODE,
				  WALK_2CLICK_MODE,
				  ROLL_MODE,	// roll,
				  ZOOM_MODE,	// zoom (horizontal)
				  PAN_MODE,		// look left/right up/down 
				  SELECT_MODE,
				  // other modes
				  TRANSLATE_MODE,
				  SCALE_MODE,
				  LOOKAT_OBJECT_MODE,
				  DRAG_DEFORMER_MODE,
				  DRAG_VALUE_MODE,
				  JUMP_TO_MODE,

};


//////////////////////////////////////////////////////////////////
//																//
//																//
//																//
//					class CEventHandler							//
//																//
//																//
//																//
//																//
//////////////////////////////////////////////////////////////////
class CEventHandler
{
public:	
	
	//
	//input
	//
	void OnKeyDown(int key);
	void OnMouseMove(Point delta);
	void OnJoystickMove(Point delta);
	void OnButtonDown(int button);


private:
	//
	int functionLUT[100];
}







//////////////////////////////////////////////////////////////////
//																//
//																//
//																//
//					class GNavigator							//
//																//
//																//
//																//
//																//
//////////////////////////////////////////////////////////////////
class GNavigator
{
public:
	//
	//	low level access
	//
	void setDelta(Point delta);
	void setState(int state);
	void setTime(double time);


	//
	// high level access
	//
	void beamTo(Point target);

private:
	//Interpolator motionInterpolator;
	//Interpolator rotationInterpolator;
}











/*


	// start Navigation timer
	void NavStartTimer();
	// stop Navigation timer
	void NavStopTimer();
	

	// navigation controls
	void NavStart();
	void NavBeamTo(POINT mousepos);
	void NavEnd();
	void NavGo();
	void NavMain();
	void NavWalk(Point vec);
	void NavPan(Point vec3);  
	
	void NavMove(Point vec);
	void NavRotate(Point vec);
	void NavExamine(Point vec);
	void NavFly(Point vec);
	void NavJumpTo(Point vec3);


	void NavCheckCollision(Point oldPosition, BOOL rotate, BOOL slide,BOOL ground);
	void NavCheckStayOnGround();
	void NavCollisionGround(Point oldPosition);
	void NavForceStayOnGround();
	void NavCameraFixToPoint();
	void NavStraightenUp();
	int  NavCheckKeyState(BOOL checkAnchor=TRUE);
	void NavCheckCursor(BOOL checkAnchor=TRUE);
	void NavResetFollowObject();
	void NavResetCursorBlinking();

	void NavCalculateTile(Point *ptile);

	// navigation interpolators
	BOOL NavStartMotionIP(Point p, int steps);
	BOOL NavStartGravityIP(int steps);
	BOOL NavStartRotPIP(Point p, int steps);
	BOOL NavStartRotVIP();
	void NavStopIP(int ip);

	void NavStepMotionIP();
	void NavStepGravityIP();
	void NavStepRotPIP();
	void NavStepRotVIP();

	void NavBalanceCamera();
*/