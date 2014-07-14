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
#ifndef _GViewControl_h
#define  _GViewControl_h

// interface
// to the class controlling the GView object
// used to pass back GView requests


class GReporter;
class GvMFString;

// message types for Message() display
#define HIT_MESSAGE 1
#define NAVIGATION_MESSAGE 2
#define COLLISION_MESSAGE 3
#define PROGRESS_MESSAGE 4
#define ERROR_MESSAGE 5


// navigation modes 
	enum GNavigationMode 
	{
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
/*
				  // other modes
				  TRANSLATE_MODE,
				  SCALE_MODE,
				  LOOKAT_OBJECT_MODE,
				  DRAG_DEFORMER_MODE,
				  DRAG_VALUE_MODE,
*/

				  JUMP_TO_MODE,


			};



class GViewController 
{
public:	
	//! display a status message
	virtual void Message(const char *message,int messageType) = 0;

	//! send a message to the console
	virtual void Print(const char *message)=0;

	//! get the type of the last message 
	virtual int GetLastMessageType() = 0; 

	//! get the the last message 
	virtual const CString& GetLastMessage() = 0; 


	//! get the error report object 
	virtual GReporter* GetReporter() = 0;

	//! mark view for redraw 
	virtual void Redraw() = 0;
	
	//! stop navigation state
	virtual void OnStopNavigation() = 0; 

	//! set the camera navigation mode by string 
	virtual int SetCameraMode(const char *mode) = 0;
	
	//! set the camera navigation mode 
	virtual int SetCameraMode(int newMode) = 0;

	//! get the current camera mode as string
	virtual const char * GetCameraModeString() = 0; 


   //! load URL directly from all information of Anchor node / browser loadUrl
	virtual BOOL loadURLfromFields(const char *description,GvMFString *urls, GvMFString *parameter,const char *homeUrl) = 0;

	//virtual void setViewpointByValue_(float positionX, float positionY, float positionZ, float orientationX, float orientationY, float orientationZ, float orientationAngle, long mode) = 0; 

	//!get value from profile as string
	virtual BOOL GetProfile(LPCTSTR lpszKey, CString &value) = 0;
	//!get value from profile as int
	virtual BOOL GetProfile(LPCTSTR lpszKey, int &value) = 0;

};


#endif  _GViewControl_h


