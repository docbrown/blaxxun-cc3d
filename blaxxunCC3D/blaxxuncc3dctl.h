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
// GLViewCtrlCtl.h : Declaration of the CGLViewCtrlCtrl OLE control class.
// $Revision: 1.42 $
// $Log: blaxxuncc3dctl.h,v $
// Revision 1.42  1998/10/30 13:40:25  tom
// shift-taste
//
// Revision 1.41  1998/10/29 18:07:42  holger
// empty message
//
// Revision 1.40  1998/10/07 13:38:55  tom
// *** empty log message ***
//
// Revision 1.39  1998/09/28 15:20:37  tom
// support of embed and param tags for navpanel
//
// Revision 1.38  1998/09/22 16:45:50  holger
// empty message
//
// Revision 1.37  1998/09/21 16:44:45  tom
// *** empty log message ***
//
// Revision 1.35  1998/08/12 13:17:54  tom
// *** empty log message ***
//
// Revision 1.34  1998/08/11 09:44:57  tom
// *** empty log message ***
//
// Revision 1.33  1998/07/30 16:32:42  thilo
// *** empty log message ***
//
// Revision 1.32  1998/07/30 12:37:31  tom
// erste versuche mit navpanel.
//
// Revision 1.31  1998/07/14 17:25:26  kristof
// *** empty log message ***
//
// Revision 1.30  1998/07/06 17:42:15  holger
// empty message
//
// Revision 1.8  1997/11/24 14:00:26  rainer
// Holgers changes
//



#ifdef _OGL
#include "COpenGL.h"
#endif

#include "dispids.h"
#include "ArrayNew.h"


// OLE_DATAPATH + GUID for interesting MIME-Types
// #include "datapath.h"
typedef BSTR OLE_DATAPATH;


// Forward classes
class GView;		
class CGLViewCtrlApp; 
class CRenderThread;
class COpenGL;
class GFile;
class GReporter;

class GDriverHints;

// dialog classes
class	DAskHardware;
class	DConsole;


// redefine VIEW class for GLView core compatibility
#define CMyglView CGLViewCtrlCtrl

class CGLViewCtrlCtrl;
class CCtrlReporter;

class GvMFString;
class GvNode;


/*

class CGlViewDataPathProperty : public CDataPathProperty
{
	void OnStartBinding( );
	void OnStopBinding( HRESULT hresult, LPCTSTR szError );

	void OnDataAvailable( DWORD dwsize, DWORD bscfFlag );
  

};

*/



// Cursor modes 
enum CursorMode {
	STANDARD_CURSOR,
	STANDARD_LOADING_CURSOR,
	NAVIGATION_CURSOR,
	COLLISION_CURSOR,
	TRANSFORM_CURSOR,
	MOVING_CURSOR, // for fly state 2 
	ZOOM_CURSOR, // for fly state 2 

	SENSOR_CURSOR,
	ANCHOR_CURSOR,
};

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
	NAV_NO_CURSOR,
	NAV_CURSOR_MAX,
};

//
enum UIMode {
		UI_NONE,
		UI_SIMPLE,
//		UI_STANDARD,
		UI_EXPERT
};	


//
// CTranslator
// translates messages 
//
#include <fstream.h>

class CTranslator {
public:
	CTranslator();
	~CTranslator();
	// translate unique string into language denpendent string, entryDefault is the default english string
	BOOL Translate(LPCTSTR key,LPCTSTR valueDefault, CString& value);
	BOOL Lookup(LPCTSTR key,CString& value);
	
protected:
#ifdef _DEBUG
	//ofstream out;
#endif

	CWinApp*			m_pApp;			//!> app, for GetProfileString
	CString				m_section;		//!> section where strings are stored
	CMapStringToString  m_hashTable;	//!>/ mapping keys to strings 
};

#include "gviewcontrol.h"

/////////////////////////////////////////////////////////////////////////////
// CGLViewCtrlCtrl : See GLViewCtrlCtl.cpp for implementation.

class CGLViewCtrlCtrl : 
	
	public COleControl
	, public GViewController

{
	DECLARE_DYNCREATE(CGLViewCtrlCtrl)

// Constructor
public:
	//CGLViewCtrlCtrl();
	CGLViewCtrlCtrl();


	// Netscape thread dialog handling
	UINT DoThreadModal(CDialog* dialog, UINT timeout=60000, BOOL menu=FALSE);
	UINT DoThreadModal(CMenu* menu, POINT* menuPos, CWnd* parent);
	void DoThreadModalUpdateMenu(CMenu* menu); 
	BOOL m_threadFinished;
	CWinThread* m_pt;
	CDialog*	m_pd;
	
	BOOL IsModalThreadActive() { return (m_threadFinished)? FALSE: TRUE; }


	// overrides 
	virtual void SetInitialDataFormats();

	// For customizing the default messages on the status bar
	virtual void GetMessageString(UINT nID, CString& rMessage) const;


	// @cmember GView objects contains the renderState and model information
	GView	*view;

	CTranslator m_translator;

	// translate a message 
	BOOL Translate(LPCTSTR key,LPCTSTR valueDefault, CString& value);
	BOOL Translate(LPCTSTR valueDefault, CString& value);
	BOOL TranslateMessageString(UINT nID, CString& value);
	CString Translate(LPCTSTR valueDefault) { CString value; Translate(valueDefault,value); return value; }
	
	// Translate a CMenu
	void Translate(CMenu &m); 
	void Translate(CPropertySheet &ps);



	// the initial context url to be loaded 
	CString	m_initialUrl;	// the initial URL

	CString	m_initialAvatarUrl;	//!> the initial Avatar URL
	BOOL    m_initialThirdPersonView; 

	CString	m_messageUrl;
	CString	m_messageTargetFrame;


	//!> user interface level 
	UIMode m_uiMode;

	BOOL	m_fullscreenOnStartup;//!> set by embed param
	CString m_fullscreenModeOnStartup;//!> set by embed param

	BOOL	m_hideCursor;		  //!> do not show cursor in fullscreen
	BOOL	m_displayFrameRate;	 //!> flag display frame rate 
	BOOL	m_displayFrameStats;
	DWORD	m_frames;			//!> num frames since last display
	DWORD	m_checkFrames;		//!> num frames since driver change
	DWORD   m_startTime;		//!> last start Time 
	DWORD   m_endTime;			//!> end Time of last Draw 


	BOOL	m_mustRedraw;   //!> redraw flag 
	CRect	m_clientRect;	//!> client rectangle 
	BOOL	m_useRenderThread;  //!> TRUE using a separate thread for Timer & Rendering 

	CRenderThread * m_renderThread; //!> the render thread handling redraw etc.

	int		m_requestedState;
	int		m_currentState;

//#define TLS_BROWSER 1
//#define TLS_REDRAW 2

#define	RUN_STATE 1
#define STOP_STATE 2

	CRITICAL_SECTION m_sceneSection;  
	
	void LockScene();
	void UnlockScene();
	UINT RenderWorker();
	BOOL StartRenderThread();
	void StopRenderThread();







	// window is minimized, no activity necessary
	BOOL m_minimized;

	BOOL m_menuActive; //!> menu is active ?
    // expired flag 
    BOOL expired;

    // check for expiration
    BOOL Expired() { return(FALSE); }
	
	void SetAnimateOff() {}

    // last status messge    
    CString lastMessage;

    // type of last status message
    int lastMessageType;

    // counter to skip some onIdles
    int m_idleCounter;

    //@cmember displays a message to the status bar with update
    void Message(const char *message,int messageType = 0);


	//! send a message to the console
	void Print(const char *message);

	//! get the type of the last message 
	int GetLastMessageType() { return lastMessageType; }

	//! get the the last message 
	const CString& GetLastMessage() { return lastMessage; }


	// registry helpers
    BOOL GetProfile(LPCTSTR lpszKey, CString &value);
	BOOL GetProfile(LPCTSTR lpszKey, int &value);

	BOOL SetProfile(LPCTSTR lpszKey, LPCTSTR value);
	BOOL SetProfile(LPCTSTR lpszKey, int value);


	int lastCursorMode; //!> the current cursor mode 

	// set the cursor mode depending on action 
	void SetCursorMode(int cursorMode);

	int GetCursorMode()const { return lastCursorMode; }

	BOOL m_collisionState; 

	// set state, if user currently is in collision during movement
	void SetCollisionState(BOOL newState);

	
	HCURSOR NavCursors[NAV_CURSOR_MAX]; //!> cursor handles 
	
	HCURSOR hCurrentCursor; //!> the current cursor handle



	//! Mark view for redraw, e.g. after settings or model have been changed
	void Redraw();


	//! update lock counter
	int updateLock;

	void BeginUpdate();
	void EndUpdate();


	// initiialized ?? 
	
	BOOL m_initialized;
	CDC	*m_pDC;

	BOOL m_enableInteraction; //!> state of OLECMDID_ENABLE_INTERACTION

	
	// initialize associated view & graphics 

	BOOL Initialize(HDC hDC);

	// flags 
	BOOL		  m_fullSpeed; //!> full speed rendering
	BOOL		  m_verbose;   //!> verbose rendering 


	BOOL		  m_askHardware; //!> popup dialog on init to aks if Hardware should be used
	BOOL		  m_saveDriverSettings;		//!> save driver settings on exit

	BOOL		  m_forceHwProblemCheck; // do a check on crash for switch back to software rendering
	GDriverHints  *m_driverHints; // driver hints for device selection
	
	// dialogs 
	DAskHardware *m_dAskHardware;
	DConsole	 *m_dConsole;
	CPropertySheet	 *m_dPad; //!> navigation pad

	// close all the opened dialogs
	void CloseAllDialogs();


	//! save D3D driver setting to registry
	void SaveDriverSettings();

	CCtrlReporter *m_reporter; //!> the errror / status reporting object

	//! get the reporter object 
	GReporter *GetReporter(); 

	//! Get the application object 
	CGLViewCtrlApp * GetApp() { return ((CGLViewCtrlApp *) AfxGetApp());  }

	
	//! set the current viewer mode to one of the above move modes
	int SetCameraMode(int newMode);

	// get current viewer mode 
	int GetCameraMode();

	//! set the current viewer mode by lower case string 
	int SetCameraMode(const char *mode);

	//! get the camera navigation mode by string for mode
	const char *GetCameraModeString(int mode);
	
	//! get the current camera navigation mode by string
	const char* GetCameraModeString();


	//! reset the navigation parameters 
	void ResetMove();



	// do animation 
	BOOL OnIdle(LONG lcount);

	// timer is running 
	BOOL m_timerRunning;

	// interval for SetTimer 
	long m_timerInterval;

	int m_timerCnt; //!> timer interrupt counter
	int m_timerTimeSensorCycle; //!> trigger time sensor every x cycles


	//! start animation timer
	void StartTimer();
	
	//! re-start animation timer if temporary disabled 
	void RestartTimer();

	//! stop animation timer
	void StopTimer();

	// navigation state variables 

	// mode started at LButton Down
	int m_lButtonMode;


	// current mode for mouse moves
    int		m_currentMode;

	// current mode for camera mouse moves
	int		m_currentCameraMode;
	int		m_cameraModeOld;

	float	m_cameraSpeed;   //!> walking speed
	float	m_cameraSpeedRot;   //!> rotation walking speed
	int		m_cameraSpeedFac;   //!> walking speed factor

	// set the camera relative move flag, if TRUE walk direction using viewdirection, FALSE relative to worlds up vector
	BOOL	SetCameraRelativeMode(BOOL newMode) { m_cameraRelative = newMode; return m_cameraRelative; }
	BOOL	GetCameraRelativeMode() {return m_cameraRelative; }

	BOOL	m_cameraRelative; //!> movement relative to view direction

	// do anchor, sensor checking on Camera Modes lButtonDown
	BOOL	m_sensorTest;
	BOOL	m_anchorSingleClick; //!> execute anchors on a single click
	GvNode* m_currentAnchor; //!> anchor found at mouse down
	BOOL	m_currentTSensor; //!> click started on a Sensor node
	BOOL	m_currentTDragSensor; //!> click started on a Sensor which needs mouse drags

	BOOL	m_handleWrlIntern; //!> handle wrl anchors internall

   
	UINT	m_moveTimerId;

	// the windows timer ID for the MoveTimer 
	int MOVE_TIMER_ID;

	int		fly_state;			//!> current state for FLY_MODE

	BOOL	m_moveRepeat;			//!> repeat based on timer
	float	m_moveRepeatStartTime; //!> timeout when to start timer
	float	m_moveRepeatCycleTime; //!> how often to repeat

	// Mouse data at left button down time
	CPoint	m_firstPoint;
	UINT	m_firstFlags;
	float	m_firstTime;

	// Mouse data at last mouse move 

	CPoint	m_lastPoint;
	UINT	m_lastFlags;
	float	m_lastTime;
	BOOL	m_wasDrag; //!> Up-Down sequence was a drag
	int		m_dragEps; //!>  drag epsilon  distance in pixels


	CPoint	m_prevLastPoint;


    // last rbutton point for SeekTo in Popup-menu
    CPoint	m_rlastPoint;
    UINT	m_rlastFlags;


//----------------------------------------------------------------------------------
	
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

//----------------------------------------------------------------------------------

	// Navigation control variables
//----------------------------------------------------------------------------
#define	NAV_MOTION_IP  1
#define	NAV_ROT_P_IP   2
#define	NAV_ROT_V_IP   4
#define	NAV_GRAVITY_IP 8

SYSTEMTIME		m_NavTime;			//!> navigation time structure

float			m_NavSpeed;			//!> real navigation speed for movement
float			m_NavSpeedRot;		//!> real navigation speed for rotation angles

float			m_NavSpeed_step;		//!> including shift keys etc real navigation speed for movement
float			m_NavSpeedRot_step;		//!> real navigation speed for rotation angles

BOOL			m_NavFocus;

BOOL 			m_NavOldSlideState;	//!> Global State of last Slide key 
BOOL 			m_NavCNTLState;		//!> Global State of CONTROL key 
BOOL 			m_NavSlideState;	//!> Global State of Slide key 
BOOL 			m_NavFollowState;	//!> Global State of Follow key 
BOOL 			m_NavSpeedState;	//!> Global State of Speed key 
BOOL 			m_NavRotState;		//!> Global State of Rotation key 
BOOL 			m_NavPointState;	//!> Global State of Point key 
BOOL 			m_NavUPState;		//!> Global State of Cursor Up key 
BOOL 			m_NavDOWNState;		//!> Global State of Cursor DOWN key 
BOOL 			m_NavLEFTState;		//!> Global State of Cursor Left key 
BOOL 			m_NavRIGHTState;	//!> Global State of Cursor RIGHT key

BOOL 			m_NavSlideStatePanel;
BOOL 			m_NavUPStatePanel;		
BOOL 			m_NavDOWNStatePanel;	
BOOL 			m_NavLEFTStatePanel;		
BOOL 			m_NavRIGHTStatePanel;

BOOL 			m_NavLBUTState;		//!> Global State of left Mouse Button 

BOOL			m_NavGravityChecked;	//!> check modus of gravity

int				m_NavRunning;		//!> 0= no Navigation activity
									//!> 1= mouse down
									//!> 2= navigation key down
BOOL			m_NavFollowObject;

BOOL			m_NavCoordDspl;		//!> coordinate display ON/OFF
BOOL			m_NavNotActive;

BOOL			m_NavMotion;		//!> any motion while waiting for anything
BOOL			m_NavTimerRunning;	//!> is Navigation Timer running ?
int				m_NavTimerCnt;		//!> timer interrupt counter
int				m_NavTimerCnt_1;		//!> timer interrupt counter
int				m_NavCursorBlkCount;	//!> cursor blink counter for Follow mode-cursor	
BOOL			m_NavCursorShow;		//!> toggle for cursor blink
int				m_NavCursorMode;
int				m_NavCursorModeOld;

BOOL			m_NavIPTimerRunning;	//!> is Navigation Interpolator Timer running ?
long			m_NavIPTimerInterval;	//!> interval for Interpolator Navigation timer

POINT			m_NavMousePos;			//!> current client mouse position
RECT			m_NavClientRect;		//!> current client rectangle
BOOL			m_NavIsWithinRect;		//!> cursor is in client rectangle

struct Vec3l
	{
	long		x;
	long		y;
	long		z;
	};																												


struct Camera
	{
	Point	FixPoint;			//!> global fix point for rotation, look at
	Point	FixPointLocal;		//!> local fix point for rotation, look at
	BOOL	FixPointValid;		//!> indicates wether fix point is valid
	Point	DefLookAt;			//!> default direction of navigation
	Point	DefLookUp;			//!> at / up / right vectors
	Point	DefLookRight;
	Point	DefPosition;		//!> default camera position
	Point	LookAt;				//!> direction of navigation
	Point	LookUp;
	Point	LookRight;
	float	LASpeed;			//!> Camera Speed forwards / back
	float	LUSpeed;			//!> Camera speed up / down
	float	LRSpeed;			//!> Camera speed left / right
	float	CRotSpeed;			//!> Camera rotation speed 
	Point	HeadRot;			//!> values for head rotation (degrees)
	float	HRotSpeed;			//!> speed of head rotation
	float	SpeedDouble;		//!> double speed
	float	DoubleSpeed;		//!> double speed
	float	SpeedAcc;			//!> speed accelerator factor (determined by size of world)
	BOOL	BackTurn;			//!> if TRUE then turn head automaticly back
	BOOL	ForceAction;		//!> check even if in ground zero
	};

Camera			m_NavCamera;	//!> Front View Camera structure


// structure for Motion Interpolator
struct  m_NavMotIP
	{
	Point	StartPoint;			//!> Start point of motion (Start camera position)
	Point	EndPoint;			//!> End point of motion (Hit Point)
	int		Steps;				//!> number of steps between start and end
	int		CurStep;			//!> number of current step
	int		StopWhere;			//!> where to stop
								//!> 0 = at the end
								//!> 1 = at x-percent of the distance
								//!> 2 = at the bounding box
	int		StopPercentage;		//!> 0% = dont move, 100% = go whole distance
	};


m_NavMotIP		m_NavMotionIP;

// structure for Gravity Interpolator
struct  m_NavGravIP
	{
	Point	StartPoint;			// Start point of motion (Start camera position)
	Point	EndPoint;			//!> End point of motion (Hit Point)
	int		Steps;				//!> number of steps between start and end
	int		CurStep;			//!> number of current step
	};


m_NavGravIP		m_NavGravityIP;


// structure for Rotation to a Point Interpolator
struct  m_NavRotPIP
	{
	Point	StartPoint;			//!> Start point of motion (Start camera position)
	Point	EndPoint;			//!> End point of motion (Hit Point)
	int		Steps;				//!> number of steps between start and end
	int		CurStep;			//!> number of current step
	};

m_NavRotPIP		m_NavRotPointIP;



// structure for Rotation to a Stand up position Interpolator
struct  m_NavRotVIP
	{
	int		Start;				//!> Vorzeichen of lookAt Normvector when starting
	float	Angle;				//!> angle of rotation (default 0.1 Rad)
	int		Steps;				//!> number of steps between start and end
	int		CurStep;			//!> number of current step
	};

m_NavRotVIP		m_NavRotVecIP;


	int		m_NavIPActive;		//!> 0=no interpolation
								//!> 1=motion interpolation active
								//!> 2=rotation to point interpolation active			
								//!> 4=rotation to vector interpolation active			
	float	m_NavTargetDist;	//!> Distance to target point (target vector length)


struct	NavCollision
	{
	BOOL	LastCollision;		//!> last frame there was a collision
	Point	LastNormal;			//!> normal vector of last collision
	Point	LastUp;				//!> up vector of last collision
	};

	NavCollision	m_NavCollision;


	POINT	m_NavMouseDiff;		//!> mouse offset to window center



//----------------------------------------------------------------------------



	// do the mouse move, based on current mouse position in point 
	BOOL DoMove(UINT nFlags, CPoint point);

	// do an incremental delta change of parameters
	BOOL DoDeltaMove(UINT nFlags, Point &delta);

	// execute navigation / sensor move action, on time event 
	// return TRUE if redraw / false if not 
	BOOL DoMoveTimer();


	// Stop navigation action, e.g. because new viewpoint was bound 
	void OnStopNavigation();

	// do the timer action 
	void OnDoTimer(UINT nIDEvent);


	//
	// OnReadFileCompleted -   GFile Thread sends a user defined WM_READFILECOMPLETED
	//                         message to this window, when the thread has finished.
	//
	afx_msg LRESULT OnReadFileCompleted(WPARAM wParam, LPARAM lParam);

	#define WM_HARDWARECHECK WM_USER +2 

	//
	// OnHardwareCheck -   Hardware check dialog done
	//
	afx_msg LRESULT OnHardwareCheck(WPARAM wParam, LPARAM lParam);

	#define WM_STATUSMESSAGE WM_USER + 3
	#define WM_CONSOLE_MESSAGE WM_USER + 4

	//
	// OnStatusMessage -   Print a status line message
	//
	afx_msg LRESULT OnStatusMessage(WPARAM wParam, LPARAM lParam);

	// write actual console buffer to console
	afx_msg LRESULT OnConsoleMessage(WPARAM wParam, LPARAM lParam);

	// main loader thread for URL scene + WWWAnchors
	GFile *mainLoader;


	// kill all download activities for this window 
	void KillDownloads(); 


	// add url to history 
	void AddUrlToHistory(const char *url,const char *description=NULL, BOOL isFile=FALSE);

	ArrayNew<CString>	m_history;
	ArrayNew<CString>	m_historyDescription;
	int					m_historyIndex;

	BOOL GoBack();

	BOOL GoForward();


	// read top level url, download if needed 
	int ReadUrl(const char *url,const char *homeUrl,gbool reload=FALSE,time_t lastModified=0); 

	//! Navigate to url 
	HRESULT NavigateToUrl(const char *url, const char *location=NULL, const char *targetFrame=NULL,	DWORD flags = 0);

	//!for loadURL script call / Anchor
	BOOL loadURLfromFields(const char *description,GvMFString *url, GvMFString *parameter,const char *homeUrl);

	// Exectute the Anchor node 
	BOOL ExecuteAnchorNode(GvNode *node);
	
	// public version
	void setViewpointByValue_(float positionX, float positionY, float positionZ, float orientationX, float orientationY, float orientationZ, float orientationAngle, long mode);




// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGLViewCtrlCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual BOOL OnSetExtent(LPSIZEL lpSizeL);
	virtual void OnSetClientSite();
	virtual DWORD GetControlFlags();
	virtual BOOL OnSetObjectRects(LPCRECT lpRectPos, LPCRECT lpRectClip);
	virtual BOOL OnSetData(LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium,BOOL bRelease);
	virtual BOOL OnRenderFileData(LPFORMATETC lpFormatEtc, CFile* pFile);
	virtual BOOL OnRenderData(LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium);
	virtual BOOL OnRenderGlobalData(LPFORMATETC lpFormatEtc, HGLOBAL* phGlobal);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnMnemonic(LPMSG pMsg);
	virtual void OnKeyDownEvent(USHORT nChar, USHORT nShiftState);
	virtual void OnAmbientPropertyChange(DISPID dispid);
	virtual BOOL OnEdit(LPMSG lpMsg, HWND hWndParent, LPCRECT lpRect);
	virtual void OnGetControlInfo(LPCONTROLINFO pControlInfo);
	virtual HMENU OnGetInPlaceMenu();
	virtual BOOL OnGetNaturalExtent(DWORD dwAspect, LONG lindex, DVTARGETDEVICE* ptd, HDC hicTargetDev, DVEXTENTINFO* pExtentInfo, LPSIZEL psizel);
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnProperties(LPMSG lpMsg, HWND hWndParent, LPCRECT lpRect);
	virtual void OnFinalRelease();
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	// Non-in-place activation
	virtual HRESULT OnOpen(BOOL bTryInPlace, LPMSG pMsg);
	virtual HRESULT OnHide();


// Implementation
protected:
	~CGLViewCtrlCtrl();

	DECLARE_OLECREATE_EX(CGLViewCtrlCtrl)    //!> Class factory and guid
	DECLARE_OLETYPELIB(CGLViewCtrlCtrl)      //!> GetTypeInfo
	DECLARE_PROPPAGEIDS(CGLViewCtrlCtrl)     //!> Property page IDs
	DECLARE_OLECTLTYPE(CGLViewCtrlCtrl)		//!> Type name and misc status

public:
	// update registry settings 
	BOOL UpdateRegistry() {
		BOOL ret = factory.UpdateRegistry(TRUE);
		return ret;
	}

	// get the progID 
//	LPCTSTR GetProgID() {
//		return factory.m_lpszProgID;
//	}
protected:	

// Message maps


	//{{AFX_MSG(CGLViewCtrlCtrl)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnCameraNone();
	afx_msg void OnUpdateCameraNone(CCmdUI* pCmdUI);
	afx_msg void OnCameraOrbit();
	afx_msg void OnUpdateCameraOrbit(CCmdUI* pCmdUI);
	afx_msg void OnCameraPan();
	afx_msg void OnUpdateCameraPan(CCmdUI* pCmdUI);
	afx_msg void OnCameraRoll();
	afx_msg void OnUpdateCameraRoll(CCmdUI* pCmdUI);
	afx_msg void OnCameraDollyXy();
	afx_msg void OnUpdateCameraDollyXy(CCmdUI* pCmdUI);
	afx_msg void OnCameraFly();
	afx_msg void OnUpdateCameraFly(CCmdUI* pCmdUI);
	afx_msg void OnCameraAnimateViewpoints();
	afx_msg void OnUpdateCameraAnimateViewpoints(CCmdUI* pCmdUI);
	afx_msg void OnCameraReset();
	afx_msg void OnCameraSeekto();
	afx_msg void OnUpdateCameraSeekto(CCmdUI* pCmdUI);
	afx_msg void OnCameraViewpointNext();
	afx_msg void OnUpdateCameraViewpointNext(CCmdUI* pCmdUI);
	afx_msg void OnCameraViewpointPrev();
	afx_msg void OnUpdateCameraViewpointPrev(CCmdUI* pCmdUI);
	afx_msg void OnCameraWalk();
	afx_msg void OnUpdateCameraWalk(CCmdUI* pCmdUI);
	afx_msg void OnCameraZoom();
	afx_msg void OnUpdateCameraZoom(CCmdUI* pCmdUI);
	afx_msg void OnRenderFlat();
	afx_msg void OnUpdateRenderFlat(CCmdUI* pCmdUI);
	afx_msg void OnRenderGouraud();
	afx_msg void OnUpdateRenderGouraud(CCmdUI* pCmdUI);
	afx_msg void OnRenderTextured();
	afx_msg void OnUpdateRenderTextured(CCmdUI* pCmdUI);
	afx_msg void OnRenderWireframe();
	afx_msg void OnUpdateRenderWireframe(CCmdUI* pCmdUI);
	afx_msg void OnCameraHeadlight();
	afx_msg void OnUpdateCameraHeadlight(CCmdUI* pCmdUI);
	afx_msg void OnRenderNonlighted();
	afx_msg void OnUpdateRenderNonlighted(CCmdUI* pCmdUI);
	afx_msg void OnRenderVertices();
	afx_msg void OnUpdateRenderVertices(CCmdUI* pCmdUI);
	afx_msg void OnCameraRelative();
	afx_msg void OnUpdateCameraRelative(CCmdUI* pCmdUI);
	afx_msg void OnCameraFollowObject();
	afx_msg void OnCameraCollision();
	afx_msg void OnUpdateCameraCollision(CCmdUI* pCmdUI);
	afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg void OnPaletteIsChanging(CWnd* pRealizeWnd);
	afx_msg BOOL OnQueryNewPalette();
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnPopupHelpManual();
	afx_msg void OnPopupHelpVisit();
	afx_msg void OnPopupHelpAbout();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnCameraJump();
	afx_msg void OnUpdateCameraJump(CCmdUI* pCmdUI);
	afx_msg void OnSettingsStopLoading();
	afx_msg void OnUpdateSettingsStopLoading(CCmdUI* pCmdUI);
	afx_msg void OnSettingsReload();
	afx_msg void OnUpdateSettingsReload(CCmdUI* pCmdUI);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSettingsSound();
	afx_msg void OnUpdateSettingsSound(CCmdUI* pCmdUI);
	afx_msg void OnPopupHelpWorldinfo();
	afx_msg void OnSettingsStayOnGround();
	afx_msg void OnUpdateSettingsStayOnGround(CCmdUI* pCmdUI);
	afx_msg void OnRenderDither();
	afx_msg void OnUpdateRenderDither(CCmdUI* pCmdUI);
	afx_msg void OnRenderTextureSmooth();
	afx_msg void OnUpdateRenderTextureSmooth(CCmdUI* pCmdUI);
	afx_msg void OnSettingsPreferences();
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnPopupHelpCheckversion();
	afx_msg void OnCameraRotate();
	afx_msg void OnUpdateCameraRotate(CCmdUI* pCmdUI);
	afx_msg void OnPopupSpeedVeryslow();
	afx_msg void OnUpdatePopupSpeedVeryslow(CCmdUI* pCmdUI);
	afx_msg void OnPopupSpeedSlow();
	afx_msg void OnUpdatePopupSpeedSlow(CCmdUI* pCmdUI);
	afx_msg void OnPopupSpeedMedium();
	afx_msg void OnUpdatePopupSpeedMedium(CCmdUI* pCmdUI);
	afx_msg void OnPopupSpeedFast();
	afx_msg void OnUpdatePopupSpeedFast(CCmdUI* pCmdUI);
	afx_msg void OnPopupSpeedVeryfast();
	afx_msg void OnUpdatePopupSpeedVeryfast(CCmdUI* pCmdUI);
	afx_msg void OnCameraZoomout();
	afx_msg void OnCameraStraightenUp();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDevModeChange(LPTSTR lpDeviceName);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnGoBack();
	afx_msg void OnUpdateGoBack(CCmdUI* pCmdUI);
	afx_msg void OnCameraGravity();
	afx_msg void OnUpdateCameraGravity(CCmdUI* pCmdUI);
	afx_msg void OnSettingsConsole();
	afx_msg void OnUpdateSettingsConsole(CCmdUI* pCmdUI);
	afx_msg void OnEnterIdle(UINT nWhy, CWnd* pWho);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnView3rdPerson();
	afx_msg void OnUpdateView3rdPerson(CCmdUI* pCmdUI);
	afx_msg void OnViewPad();
	afx_msg void OnUpdateViewPad(CCmdUI* pCmdUI);
	afx_msg void OnPopupPanel();
	afx_msg void OnUpdatePopupPanel(CCmdUI* pCmdUI);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnInitMenu(CMenu* pMenu);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG

	// standard OnClose is without dwSaveOption

	afx_msg void OnClose(DWORD dwSaveOption); 
	
	//  command id range handlers
	afx_msg void OnCameraViewpoint(UINT id); 
	afx_msg void OnUpdateCameraViewpoint(CCmdUI* pCmdUI);
	afx_msg void OnUserMenu(UINT id); 
	afx_msg void OnOptionsDriver(UINT id); 
	afx_msg void OnUpdateOptionsDriver(CCmdUI* pCmdUI);
	afx_msg void OnPaint();

	// WM_MOVING
	afx_msg LRESULT OnMoving(WPARAM wParam, LPARAM lParam);

	//WM_SIZING
	afx_msg LRESULT OnSizing(WPARAM wParam, LPARAM lParam);

	//WM_CAPTURECHANGED
	afx_msg LRESULT OnCaptureChanged(WPARAM wParam, LPARAM lParam);
	
	
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CGLViewCtrlCtrl)
	afx_msg BSTR GetUrl();
	afx_msg void SetUrl(LPCTSTR lpszNewValue);
	afx_msg BSTR GetRenderMode();
	afx_msg void SetRenderMode(LPCTSTR lpszNewValue);
	afx_msg BSTR GetNavigationMode();
	afx_msg void SetNavigationMode(LPCTSTR lpszNewValue);
	afx_msg short GetHeadlight();
	afx_msg void SetHeadlight(short nNewValue);
	afx_msg float GetWalkSpeed();
	afx_msg void SetWalkSpeed(float newValue);
	afx_msg BOOL GetAnimateAllViewpoints();
	afx_msg void SetAnimateAllViewpoints(BOOL bNewValue);
	afx_msg BSTR GetViewpoint();
	afx_msg void SetViewpoint(LPCTSTR lpszNewValue);
	afx_msg BSTR GetDescription();
	afx_msg void SetDescription(LPCTSTR lpszNewValue);
	afx_msg BSTR GetWorld();
	afx_msg void SetWorld(LPCTSTR lpszNewValue);
	afx_msg BOOL GetCollisionDetection();
	afx_msg void SetCollisionDetection(BOOL bNewValue);
	afx_msg float GetAvatarHeight();
	afx_msg void SetAvatarHeight(float newValue);
	afx_msg float GetCollisionDistance();
	afx_msg void SetCollisionDistance(float newValue);
	afx_msg float GetVisibilityLimit();
	afx_msg void SetVisibilityLimit(float newValue);
	afx_msg BOOL GetGravity();
	afx_msg void SetGravity(BOOL bNewValue);
	afx_msg void OnNextViewpoint();
	afx_msg void OnPrevViewpoint();
	afx_msg BSTR getName();
	afx_msg BSTR getVersion();
	afx_msg float getCurrentSpeed();
	afx_msg float getCurrentFrameRate();
	afx_msg BSTR getWorldURL();
	afx_msg void replaceWorld(LPUNKNOWN nodes);
	afx_msg void loadURL(LPCTSTR url, LPCTSTR parameter);
	afx_msg void setDescription(LPCTSTR description);
	afx_msg LPUNKNOWN createVrmlFromString(LPCTSTR vrmlSyntax);
	afx_msg void createVrmlFromURL(LPCTSTR url, LPDISPATCH node, LPCTSTR event);
	afx_msg LPUNKNOWN getNode(LPCTSTR name);
	afx_msg void addRoute(LPUNKNOWN fromNode, LPCTSTR fromEventOut, LPUNKNOWN toNode, LPCTSTR toEventIn);
	afx_msg void deleteRoute(LPUNKNOWN fromNode, LPCTSTR fromEventOut, LPUNKNOWN toNode, LPCTSTR toEventIn);
	afx_msg LPUNKNOWN createNode(LPCTSTR nodeclass);
	afx_msg void addNode(LPUNKNOWN node);
	afx_msg LPDISPATCH getWorld();
	afx_msg void beginUpdate();
	afx_msg void endUpdate();
	afx_msg long setTimerInterval(long interval);
	afx_msg BOOL isSceneLoaded();
	afx_msg void setNodeName(LPUNKNOWN node, LPCTSTR name);
	afx_msg BSTR getNodeName(LPUNKNOWN node);
	afx_msg LPUNKNOWN getEventOut(LPCTSTR eventOutName);
	afx_msg LPUNKNOWN getEventIn(LPCTSTR eventInName);
	afx_msg BSTR getControlPathname();
	afx_msg BOOL saveWorld(LPCTSTR fileName);
	afx_msg void saveViewpoint(LPCTSTR viewpointName);
	afx_msg BOOL setFullscreen(BOOL mode);
	afx_msg BOOL removeNode(LPUNKNOWN node);
	afx_msg BOOL setObserver(LPUNKNOWN observer, long flags);
	afx_msg BOOL loadURLfromFile(LPCTSTR url, LPCTSTR mimeType, LPCTSTR fileName);
	afx_msg void onUrlNotify(LPCTSTR url, LPCTSTR mimeType, LPCTSTR fileName, long reason);
	afx_msg BOOL setObserverWnd(long hwndHandle);
	afx_msg long getUiMode();
	afx_msg void setUiMode(long newMode);
	afx_msg void openPreferences(long flags);
	afx_msg void playSound(LPCTSTR soundFilename);
	afx_msg BOOL loadURLfromFile2(LPCTSTR url, LPCTSTR mimeType, long lastModified, long size, LPCTSTR fileName);
	afx_msg void onUrlNotify2(LPCTSTR url, LPCTSTR mimeType, long lastModified, long size, LPCTSTR fileName, long reason);
	afx_msg void printMessage(LPCTSTR message);
	afx_msg void setViewpointByValue(float positionX, float positionY, float positionZ, float orientationX, float orientationY, float orientationZ, float orientationAngle, long mode);
	afx_msg void getViewpointByValue(float FAR* positionX, float FAR* positionY, float FAR* positionZ, float FAR* orientationX, float FAR* orientationY, float FAR* orientationZ, float FAR* orientationAngle, long mode);
	afx_msg BOOL getNodeBoundingBox(LPUNKNOWN node, long mode, float FAR* xmin, float FAR* ymin, float FAR* zmin, float FAR* xmax, float FAR* ymax, float FAR* zmax);
	afx_msg void setUrl2(LPCTSTR url, long lastModified, long mode);
	afx_msg long getInterfaceVersion();
	afx_msg void setThirdPersonView(BOOL mode);
	afx_msg BOOL getThirdPersonView();
	afx_msg LPUNKNOWN computeRayHit(LPUNKNOWN startNode, float startx, float starty, float startz, float endx, float endy, float endz, long mode);
	afx_msg void setMyAvatarNode(LPUNKNOWN node);
	afx_msg LPUNKNOWN getMyAvatarNode();
	afx_msg void setMyAvatarURL(LPCTSTR url);
	afx_msg BSTR getMyAvatarURL();
	afx_msg BOOL setViewpointFollow(LPUNKNOWN node, float refX, float refY, float refZ, long mode);
	afx_msg BOOL setNodeEventIn(LPCTSTR nodeName, LPCTSTR eventInName, LPCTSTR value);
	afx_msg BSTR getNodeEventOut(LPCTSTR nodeName, LPCTSTR eventOutName);
	afx_msg LPUNKNOWN getObject(long objectId);
	afx_msg void setNavigationPanel(BOOL mode);
	afx_msg BOOL getNavigationPanel();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

//	afx_msg BSTR GetUrl();
//	afx_msg void SetUrl(LPCTSTR lpszNewValue);

//	afx_msg BSTR GetMedia();
//	afx_msg void SetMedia(LPCTSTR lpszNewValue);
//	afx_msg BSTR GetCfformat();
//	afx_msg void SetCfformat(LPCTSTR lpszNewValue);

	DECLARE_INTERFACE_MAP()

	// IPersistMoniker
	BEGIN_INTERFACE_PART(PersistMoniker, IPersistMoniker)
		INIT_INTERFACE_PART(CGLViewCtrl, PersistMoniker)
		STDMETHOD(GetClassID)(CLSID *clsid);
		STDMETHOD(IsDirty)(void);
		STDMETHOD(Load)(BOOL fFullyAvailable, IMoniker* pmkSrc, IBindCtx* pbc, DWORD grfMode);
		STDMETHOD(Save)(IMoniker* pmkDst, IBindCtx* pbc, BOOL fRemember);
		STDMETHOD(SaveCompleted)(IMoniker* pmkNew, IBindCtx* pbc);
		STDMETHOD(GetCurMoniker)(IMoniker** ppmkCur);
	END_INTERFACE_PART(PersistMoniker)

	// IOleCommandTarget //http://www.microsoft.com/kb/articles/q167/9/56.htm
    BEGIN_INTERFACE_PART(CmdTargetObj, IOleCommandTarget)
         STDMETHOD(QueryStatus)(const GUID*, ULONG, OLECMD[], OLECMDTEXT*);
         STDMETHOD(Exec)(const GUID*, DWORD, DWORD, VARIANTARG*, VARIANTARG*);
     END_INTERFACE_PART(CmdTargetObj)






	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CGLViewCtrlCtrl)
	void FireOnSceneChanged(LPCTSTR url)
		{FireEvent(eventidOnSceneChanged,EVENT_PARAM(VTS_BSTR), url);}
	void FireOnEvent(LPCTSTR name, LPCTSTR value, double timeStamp)
		{FireEvent(eventidOnEvent,EVENT_PARAM(VTS_BSTR  VTS_BSTR  VTS_R8), name, value, timeStamp);}
	void FireOnProgress(long amount)
		{FireEvent(DISPID_ONPROGRESS,EVENT_PARAM(VTS_I4), amount);}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()


	//void FireOnProgress(long amount) {FireEvent(DISPID_PROGRESS,EVENT_PARAM(VTS_I4), amount);}



// Dispatch and event IDs
public:

	enum {
	//{{AFX_DISP_ID(CGLViewCtrlCtrl)
	dispidUrl = 1L,
	dispidRenderMode = 2L,
	dispidNavigationMode = 3L,
	dispidHeadlight = 4L,
	dispidWalkSpeed = 5L,
	dispidAnimateAllViewpoints = 6L,
	dispidViewpoint = 7L,
	dispidDescription = 8L,
	dispidWorld = 9L,
	dispidCollisionDetection = 10L,
	dispidAvatarHeight = 11L,
	dispidCollisionDistance = 12L,
	dispidVisibilityLimit = 13L,
	dispidGravity = 14L,
	dispidOnNextViewpoint = 15L,
	dispidOnPrevViewpoint = 16L,
	dispidGetName = 17L,
	dispidGetVersion = 18L,
	dispidGetCurrentSpeed = 19L,
	dispidGetCurrentFrameRate = 20L,
	dispidGetWorldURL = 21L,
	dispidReplaceWorld = 22L,
	dispidLoadURL = 23L,
	dispidSetDescription = 24L,
	dispidCreateVrmlFromString = 25L,
	dispidCreateVrmlFromURL = 26L,
	dispidGetNode = 27L,
	dispidAddRoute = 28L,
	dispidDeleteRoute = 29L,
	dispidCreateNode = 30L,
	dispidAddNode = 31L,
	dispidGetWorld = 32L,
	dispidBeginUpdate = 33L,
	dispidEndUpdate = 34L,
	dispidSetTimerInterval = 35L,
	dispidIsSceneLoaded = 36L,
	dispidSetNodeName = 37L,
	dispidGetNodeName = 38L,
	dispidGetEventOut = 39L,
	dispidGetEventIn = 40L,
	dispidGetControlPathname = 41L,
	dispidSaveWorld = 42L,
	dispidSaveViewpoint = 43L,
	dispidSetFullscreen = 44L,
	dispidRemoveNode = 45L,
	dispidSetObserver = 46L,
	dispidLoadURLfromFile = 47L,
	dispidOnUrlNotify = 48L,
	dispidSetObserverWnd = 49L,
	dispidGetUiMode = 50L,
	dispidSetUiMode = 51L,
	dispidOpenPreferences = 52L,
	dispidPlaySound = 53L,
	dispidLoadURLfromFile2 = 54L,
	dispidOnUrlNotify2 = 55L,
	dispidPrint = 56L,
	dispidSetViewpointByValue = 57L,
	dispidGetViewpointByValue = 58L,
	dispidGetNodeBoundingBox = 59L,
	dispidSetUrl2 = 60L,
	dispidGetInterfaceVersion = 61L,
	dispidSetThirdPersonView = 62L,
	dispidGetThirdPersonView = 63L,
	dispidComputeRayHit = 64L,
	dispidSetMyAvatarNode = 65L,
	dispidGetMyAvatarNode = 66L,
	dispidSetMyAvatarURL = 67L,
	dispidGetMyAvatarURL = 68L,
	dispidSetViewpointFollow = 69L,
	dispidSetNodeEventIn = 70L,
	dispidGetNodeEventOut = 71L,
	dispidGetObject = 72L,
	dispidSetNavigationPanel = 73L,
	dispidGetNavigationPanel = 74L,
	eventidOnSceneChanged = 1L,
	eventidOnEvent = 2L,
	//}}AFX_DISP_ID
	};

	//eventidProgress = 3L,


private:
	BOOL InsideSphere;
	BOOL m_navPanelDrag;

	//BOOL m_PanelEnabled;
	BOOL m_navPanelOn_Embed;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

