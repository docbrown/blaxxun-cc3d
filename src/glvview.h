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

@module GLViewh.h - GLView MFC View class |

Copyright (c) 1995	by Holger Grahn
All rights reserved

Purpose:
Main class for all user interface interaction & drawing intialization ...

Classes:
<c CMyglView>
 
Notes:

Changes:

Todo :

******************************************************************************/
#ifndef _GLVView_h_
#define _GLVView_h_

#ifdef _OGL
#include "COpenGL.h"
#endif

// Utilities

int CstringToPoint(const CString &s,Point &p);
int CstringFromPoint(CString &s,const Point &p);

class GView;		// Forward
class GvMFString;
class GvNode;

class DMaterial;	// forward
class DTexture;		// forward
class DParameter;		// forward
class DBackground;	// forward
class Dfog;		// forward
class DStyle;
class DDrawStyle;

class DStylePage;

class DObjNew;
class DNewFontLogo;
class DTime;

class DDeformSphere;

class DMorph;
// www
class DOpenUrl;
class DViewpoint;
class DWWWOptions;

class DTreeEditor;

class DTransformSheet;
class DConsole;


class DDialogControl {
public :

	// member dialogs (to keep dialog settings and to allow non-modal dialogs)
	DDialogControl();
	~DDialogControl();


	DMaterial   *m_dMaterial;
	DTexture 	*m_dTexture;
	DParameter  *m_dParameter;
	DBackground *m_dBackground;
	Dfog		*m_dFog;
	DDrawStyle  *m_dDrawStyle;

	// @cmember DObjNew dialog
	DObjNew		*m_dObjectNew;
	
	// @cmember DNewFontLogo dialog
	DNewFontLogo *m_dNewFontLogo;

	// @cmember DTime dialog
	DTime		*m_dTime;

	// @cmember DMorph dialog
	DMorph		*m_dMorph;

   // @cmember DDeformSphere dialog
	DDeformSphere *m_dDeformSphere;


	// @cmember DOpenUrl dialog
	DOpenUrl *m_dOpenUrl;
	
	// @cmember DViewpoint dialog
	DViewpoint *m_dViewpoint;

	// @cmember DTreeEditor dialog
	DTreeEditor *m_dTreeEditor;

	DTreeEditor *m_dLibrary;

	// @cmember DTransform dialog
	DTransformSheet *m_dTransform;

	// @cmember Console dialog

	DConsole	*m_dConsole;

	// close all open modeless dialogs
	void CloseAllDialogs();
	void UpdateAllDialogs();

	void UpdateTimeDialog();
	
	void DestroyAllDialogs();


};




class GReporter;

class GFile;

class GEditController;



// message types for Message() display
#define HIT_MESSAGE 1
#define NAVIGATION_MESSAGE 2
#define COLLISION_MESSAGE 3
#define PROGRESS_MESSAGE 4


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


//
// CTranslator
// translates messages 
//

class CTranslator {
public:
	CTranslator();
	~CTranslator();
	// translate unique string into language denpendent string, entryDefault is the default english string
	BOOL Translate(LPCTSTR key,LPCTSTR valueDefault, CString& value);
	BOOL Lookup(LPCTSTR key,CString& value);
	
protected:
	CWinApp*			m_pApp;			// app, for GetProfileString
	CString				m_section;		// section where strings are stored
	CMapStringToString  m_hashTable;	/// mapping keys to strings 
};



//
// Message to post to view, when a modeless dialog is closed
//
#define WM_DIALOG_CLOSED (WM_USER + 101)


#include "gviewcontrol.h"

//@class MFC GLView view class
class CMyglView : 
	 public CView
	, public GViewController

{
public:

#ifdef _OGL
	//@cmember COpenGl object, manages WindowsOpenGl context, pixelformat and palette
    //COpenGL *m_glptr;
#endif


	// @cmember GView objects contains the renderState and model information
	GView	*view;

	CTranslator m_translator;

	// translate a message 
	BOOL Translate(LPCTSTR key,LPCTSTR valueDefault, CString& value);
	BOOL Translate(LPCTSTR valueDefault, CString& value);
	BOOL TranslateMessageString(UINT nID, CString& value);
	CString Translate(LPCTSTR valueDefault) { CString value; Translate(valueDefault,value); return value; }

	
	GEditController *m_controller;


	BOOL	m_displayFrameRate;	 // flag display frame rate 
	DWORD	m_frames;			// num frames since last display
	DWORD   m_startTime;		// last start Time 
	DWORD   m_endTime;			// end Time of last Draw 


	// window is minimized, no activity necessary
	BOOL m_minimized;

    // expired flag 
    BOOL expired;


    // check for expiration
    BOOL Expired(); 

    // last status messge    
    CString lastMessage;

    // type of last status message
    int lastMessageType;


	int lastCursorMode; // the current cursor mode 
	// set the cursor mode depending on action 
	void SetCursorMode(int cursorMode);

	int GetCursorMode()const { return lastCursorMode; }
	
	HCURSOR cursors[ANCHOR_CURSOR+1]; // cursor handles 
	
	HCURSOR hCurrentCursor; // the current cursor handle

	// Stop navigation action, e.g. because new viewpoint was bound 
	void OnStopNavigation(); 


	// dialogs
	BOOL		  m_fullSpeed; // full speed rendering
	BOOL		  m_verbose;   // verbose rendering 

	BOOL		  m_askHardware; 
	// DAskHardware *m_dAskHardware;


	// all open dialogs 
	DDialogControl dialogs;

	// close all open modeless dialogs
	void CloseAllDialogs();
	void UpdateAllDialogs();

	void UpdateTimeDialog();
	
	// update viewpoint dialog if open
	void UpdateViewpointDialog(int all=0);

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


	// for loadURL script call
	BOOL loadURLfromFields(GvMFString *url, GvMFString *parameter,const char *homeUrl);

	// Exectute the Anchor node 
	BOOL ExecuteAnchorNode(GvNode *node);


	//
	// OnDialogClosed   -   Dialogs send this message 
	//                         to this window, when the dialogs has closed
	//
	afx_msg LRESULT OnDialogClosed(WPARAM wParam, LPARAM lParam);

	// WM_MOVING
	afx_msg LRESULT OnMoving(WPARAM wParam, LPARAM lParam);

	//WM_SIZING
	afx_msg LRESULT OnSizing(WPARAM wParam, LPARAM lParam);

	//WM_CAPTURECHANGED
	afx_msg LRESULT OnCaptureChanged(WPARAM wParam, LPARAM lParam);



protected: // create from serialization only
	CMyglView();
	
	DECLARE_DYNCREATE(CMyglView)

// Attributes
public:
	CMyglDoc* GetDocument();

// Operations
public:
	// Apply current material settings to view
	void ApplyMaterial(DMaterial &d);

   // Get Material setting and copy to dialog members
	void GetMaterial(DMaterial &d);

	void ApplyTexture(DTexture &d);
	void GetTexture(DTexture &d);

    void ApplyParameter(DParameter &d);
	void GetParameter(DParameter &d);

	void ApplyBackground(DBackground &d);
	void GetBackground(DBackground &d);

	void ApplyStyle(DStyle &d);
	void GetStyle(DStyle &d);

	void ApplyStyle(DStylePage &d);
	void GetStyle(DStylePage &d);

	void ApplyDrawStyle(DDrawStyle &d);
	void GetDrawStyle(DDrawStyle &d);

	void ApplyObject(DObjNew &d);
	void GetObject(DObjNew &d);

	int Apply(DViewpoint &d);
	int Get(DViewpoint &d);

	int Apply(DWWWOptions &d);
	int Get(DWWWOptions &d);

  	void Apply(DDeformSphere &d);
	void Get(DDeformSphere &d);


	// Load Material from File
	int LoadMaterial();

	//@cmember edit a single color using standard dialog
	int EditColor(const char *prompt, Point &color);

	void ApplyFog(Dfog &d);
	void GetFog(Dfog &d);

	int ReadProfileSetting();
	int WriteProfileSetting(int mode=0);

	bool GetProfile( LPCTSTR lpszSection, LPCTSTR lpszEntry, int &val);
	bool SetProfile( LPCTSTR lpszSection, LPCTSTR lpszEntry, int val);
	bool GetProfile( LPCTSTR lpszSection, LPCTSTR lpszEntry, CString &val); 
	bool SetProfile( LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR val);

	// registry helpers
    BOOL GetProfile(LPCTSTR lpszKey, CString &value);
	BOOL GetProfile(LPCTSTR lpszKey, int &value);

	BOOL SetProfile(LPCTSTR lpszKey, LPCTSTR value);
	BOOL SetProfile(LPCTSTR lpszKey, int value);





// Implementation
public:
	virtual ~CMyglView();
	
    //! displays a message to the status bar with update
    void Message(const char *message,int messageType = 0);

	//! send a message to the console
	void Print(const char *message);

	//! get the type of the last message 
	int GetLastMessageType() { return lastMessageType; }

	//! get the the last message 
	const CString& GetLastMessage() { return lastMessage; }


	// Overidden to change ActiveView in CMyglApp
	virtual void OnActivateView( BOOL bActivate, CView* pActivateView, CView* pDeactiveView );

	// Overidden 
	virtual void OnInitialUpdate();

	virtual void OnDraw(CDC* pDC);	// overridden to draw this view

	static	CMyglView* GetView(void);
	
    BOOL	PreCreateWindow(CREATESTRUCT& cs);

	// Set a Default Pixelformat
	int SetDefaultPixelFormat(int DoubleBuffer=1,int DepthBits = 32, int Rgb=1);

	CMyglApp * GetApp() { return ((CMyglApp *) AfxGetApp());  }

	// flag print timing 
	int doPrintTime;
	float LastFrameTime;   // Time for last frame
	float MinFrameTime;    // Minimum Time a updates should take, to get not to fast
	float LastTime;	       // Last Time Action happend, for AutoAnimate
	float AnimationStartTime; // Start tíme for animation
	float ComplexTime;     // Time, if object is to complex for AutoAnimate
	int isComplex;	       // flag, set OnDraw, to signal object is too complex
			       			// for autoanimation

	int doAutoAnimate;     // Auto Animation after timeout enabled
	float AutoAnimateTimeOut; // Timeout after last action to start animation
	int Animating;		// flag, currently animating ?

    BOOL m_updateColors;	// TRUE use CDC->UpdateColors in OnQueryNewWinPalette
	BOOL m_colorsOk;		// TRUE if our colormap is installed	


	void SetAnimating(int flag) { Animating = flag; }
	void SetDoAutoAnimate(int flag) { doAutoAnimate = flag; }
	
	//@cmember turn of animating
	void SetAnimateOff() {	Animating = 0; /* LastTime = GetTime()); */ }

	virtual BOOL OnIdle(LONG lcount);
	virtual void OnAnimateIdle();

	//@cmember Mark view for redraw, e.g. after settings or model have been changed
	void Redraw();


    void AddUrlToHistory(const CString &url,int isFile = 0);

	//@cmember read model
	int ReadModel(const char *FileName);

	//@cmember read top level url
	int ReadUrl(const char *Url,BOOL reload=FALSE);

	//! Navigate to url 
	HRESULT NavigateToUrl(const char *url, const char *location=NULL, const char *targetFrame=NULL,	DWORD flags = 0);

	//! for loadURL script call / Anchor
	BOOL loadURLfromFields(const char *description,GvMFString *url, GvMFString *parameter,const char *homeUrl);

	
	//@cmember read model
	int WriteModel(const char *FileName);

	// create new reporter for sending status to statusbar 
	GReporter * m_savedReporter;

	GReporter *m_reporter; // the errror / status reporting object

	// get the reporter object 
	GReporter *GetReporter(); 


	/*

	enum MoveMode {
				  NULL_MODE,
				  SELECT_MODE,

				  // viewer modes
				  DOLLY_XY_MODE, // DRAG up/down left/right
				  ORBIT_MODE,	// rotate around 
				  PAN_MODE,		// look left/right up/down 
				  ZOOM_MODE,	// zoom (horizontal)
				  ROLL_MODE,	// roll,
				  WALK_MODE,	// move in/out look left/right
				  FLY_MODE,		// like walk but toggled with 2 clicks 
				  WALK_2CLICK_MODE,
				  ZOOM_LENS_MODE,

				  // other modes
				  TRANSLATE_MODE,
				  SCALE_MODE,
				  ROTATE_MODE,
				  LOOKAT_OBJECT_MODE,
				  DRAG_DEFORMER_MODE,
				  DRAG_VALUE_MODE,
				};
   
	*/

   // current mode for mouse moves
   int m_currentMode;

   // current mode for camera mouse moves
   int m_currentCameraMode;

   int m_cameraRelative; // movement relative to view direction
   int m_cameraConstrain; // constrain to boundingbox
   float m_cameraSpeed;   // walking speed

   float m_cameraDollySpeed;   // walking speed
   float m_cameraAngleSpeed;   // angle for orbit, roll, pan 

   // set the current viewer mode to one of the above move modes
   int SetCameraMode(int newMode);

   // get current viewer mode 
   int GetCameraMode();

   // set the current viewer mode by lower case string 
   int SetCameraMode(const char *mode);
   const char *GetCameraModeString(int mode=-1);

   //! get the current camera navigation mode by string
   const char* GetCameraModeString();


   // set the camera relative move flag, if TRUE walk direction using viewdirection, FALSE relative to worlds up vector
   int SetCameraRelativeMode(int newMode) { m_cameraRelative = newMode; return m_cameraRelative; }
   int GetCameraRelativeMode() {return m_cameraRelative; }


   /* default viewer event handling stuff */
   int SetMode(int newMode);
   int GetMode();
   
   int m_lButtonMode;
   //int m_mButtonMode;
   //int m_rButtonMode;
   //int m_lButtonDClickMode;

   // movement is absolute to first mouse down point
   BOOL m_moveAbs;

   // to anchor, sensor checking on Camera Modes lButtonDown
   BOOL m_sensorTest;
   
   UINT m_moveTimerId;
   int  m_moveRepeat;			// repeat based on timer
   float m_moveRepeatStartTime; // timeout when to start timer
   float m_moveRepeatCycleTime; // how often to repeat

   // Mouse data at Left button down time
   CPoint m_firstPoint;
   UINT	  m_firstFlags;
   float  m_firstTime;

   CPoint m_lastPoint;
   UINT	  m_lastFlags;
   float  m_lastTime;

   // last rbutton point 
   CPoint m_rlastPoint;
   UINT	  m_rlastFlags;


   void ResetMove();

   // do an incremental delta change of parameters
   BOOL DoDeltaMove(UINT nFlags, Point &delta);

   // to move base on point 
   BOOL DoMove(UINT nFlags, CPoint point);

  	// execute move action, on time event 
   BOOL DoMoveTimer();

   void DoRelativeMove(UINT nFlags, float dx, float dy);


   // the currently executed viewer mode (for OnMouseMove,OnTimer etc)
   int Mode; 
   
   // the windows timer ID for the MoveTimer 
   int MOVE_TIMER_ID;

   //CPoint lastDelta;

   int fly_state;

   int lbutton;
   CPoint lbuttonP;
   UINT buttonFlags;
   //int   TimeMode = CMyglView::NULL_MODE;
   UINT TimerId; 
   int lastWasTimer;





#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;

#endif


// Generated message map functions
protected:
	//{{AFX_MSG(CMyglView)
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPixfmtChoose();
	afx_msg void OnPixfmtDescribe();
	afx_msg void OnPixfmtEnum();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCameraDollyXy();
	afx_msg void OnCameraOrbit();
	afx_msg void OnCameraPan();
	afx_msg void OnCameraReset();
	afx_msg void OnCameraWalk();
	afx_msg void OnCameraZoom();
	afx_msg void OnRenderWireframe();
	afx_msg void OnRenderSolid();
	afx_msg void OnRenderFlat();
	afx_msg void OnRenderGouraud();
	afx_msg void OnCameraRoll();
	afx_msg void OnCameraSpinauto();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnFileRead();
	afx_msg void OnRenderVertices();
	afx_msg void OnRenderLighted();
	afx_msg void OnRenderNormals();
	afx_msg void OnRenderTextured();
	afx_msg void OnRenderOptionsBackfacecull();
	afx_msg void OnRenderOptions2sidedlighting();
	afx_msg void OnRenderOptionsLocalviewer();
	afx_msg void OnRenderOptionsAntialiaslines();
	afx_msg void OnRenderOptionsLoadtexture();
	afx_msg void OnRenderColored();
	afx_msg void OnUpdateCameraDollyXy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCameraOrbit(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCameraPan(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCameraZoom(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCameraRoll(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCameraWalk(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCameraSpinauto(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRenderWireframe(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRenderVertices(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRenderSolid(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRenderFlat(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRenderGouraud(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRenderLighted(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRenderColored(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRenderTextured(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRenderNormals(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRenderOptionsBackfacecull(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRenderOptions2sidedlighting(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRenderOptionsLocalviewer(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRenderOptionsAntialiaslines(CCmdUI* pCmdUI);
	afx_msg void OnMaterialLoad();
	afx_msg void OnMaterialEdit();
	afx_msg void OnMaterialBackground();
	afx_msg void OnRenderOptionsRevCulling();
	afx_msg void OnUpdateRenderOptionsRevCulling(CCmdUI* pCmdUI);
	afx_msg void OnMaterialTextureOptions();
	afx_msg void OnMaterialBackgroundOptions();
	afx_msg void OnDeformSphere();
	afx_msg void OnToolsDeformDisable();
	afx_msg void OnFog();
	afx_msg void OnParameter();
	afx_msg void OnOptionsConfig();
	afx_msg void OnRenderStyle();
	afx_msg void OnFileNew();
	afx_msg void OnFileSaveas();
	afx_msg void OnCameraAutospin();
	afx_msg void OnUpdateCameraAutospin(CCmdUI* pCmdUI);
	afx_msg void OnObjectFlipnormals();
	afx_msg void OnUpdateObjectFlipnormals(CCmdUI* pCmdUI);
	afx_msg void OnToolsBuildShell();
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg BOOL OnQueryNewPalette();
	afx_msg void OnToolsTriangulate();
	afx_msg void OnFileNewfontlogo();
	afx_msg void OnToolsObjectInfo();
	afx_msg void OnRenderTime();
	afx_msg void OnUpdateRenderTime(CCmdUI* pCmdUI);
	afx_msg void OnToolsMorph();
	afx_msg void OnRenderBoxes();
	afx_msg void OnUpdateRenderBoxes(CCmdUI* pCmdUI);
	afx_msg void OnWwwOpenUrl();
	afx_msg void OnUpdateWwwOpenUrl(CCmdUI* pCmdUI);
	afx_msg void OnUpdateWwwAbortLoad(CCmdUI* pCmdUI);
	afx_msg void OnWwwAbortLoad();
	afx_msg void OnWwwOptions();
	afx_msg void OnEnterIdle(UINT nWhy, CWnd* pWho);
	afx_msg void OnCameraViewpoints();
	afx_msg void OnUpdateCameraViewpoints(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	afx_msg void OnWwwLoadAllInlines();
	afx_msg void OnUpdateWwwLoadAllInlines(CCmdUI* pCmdUI);
	afx_msg void OnCameraFly();
	afx_msg void OnUpdateCameraFly(CCmdUI* pCmdUI);
	afx_msg void OnRenderTransparency();
	afx_msg void OnUpdateRenderTransparency(CCmdUI* pCmdUI);
	afx_msg void OnRenderNonLighted();
	afx_msg void OnUpdateRenderNonLighted(CCmdUI* pCmdUI);
	afx_msg void OnWwwReload();
	afx_msg void OnRenderAnimate();
	afx_msg void OnUpdateRenderAnimate(CCmdUI* pCmdUI);
	afx_msg void OnToolsTreeEditor();
	afx_msg void OnUpdateToolsTreeEditor(CCmdUI* pCmdUI);
	afx_msg void OnCameraRelative();
	afx_msg void OnUpdateCameraRelative(CCmdUI* pCmdUI);
	afx_msg void OnFileRender();
	afx_msg void OnUpdateFileRender(CCmdUI* pCmdUI);
	afx_msg void OnOptionsGlInfo();
	afx_msg void OnMaterialBackgroundImage();
	afx_msg void OnFileRenderToFile();
	afx_msg void OnMaterialBackgroundReset();
	afx_msg void OnUpdateMaterialBackgroundReset(CCmdUI* pCmdUI);
	afx_msg void OnOptionsDirect3d();
	afx_msg void OnUpdateOptionsDirect3d(CCmdUI* pCmdUI);
	afx_msg void OnOptionsFullscreen();
	afx_msg void OnUpdateOptionsFullscreen(CCmdUI* pCmdUI);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
	afx_msg void OnCameraAnimateViewpoints();
	afx_msg void OnUpdateCameraAnimateViewpoints(CCmdUI* pCmdUI);
	afx_msg void OnCameraViewpointNext();
	afx_msg void OnUpdateCameraViewpointNext(CCmdUI* pCmdUI);
	afx_msg void OnCameraViewpointPrev();
	afx_msg void OnUpdateCameraViewpointPrev(CCmdUI* pCmdUI);
	afx_msg void OnCameraSeekToLast();
	afx_msg void OnUpdateCameraSeekToLast(CCmdUI* pCmdUI);
	afx_msg void OnToolsTransform();
	afx_msg void OnUpdateToolsTransform(CCmdUI* pCmdUI);
	afx_msg void OnCameraNone();
	afx_msg void OnUpdateCameraNone(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolsTriangulate(CCmdUI* pCmdUI);
	afx_msg void OnCameraHeadLight();
	afx_msg void OnUpdateCameraHeadLight(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDeformSphere(CCmdUI* pCmdUI);
	afx_msg void OnCameraFollowObject();
	afx_msg void OnCameraCollision();
	afx_msg void OnUpdateCameraCollision(CCmdUI* pCmdUI);
	afx_msg void OnToolsOptimize();
	afx_msg void OnUpdateToolsOptimize(CCmdUI* pCmdUI);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnCameraJumpto();
	afx_msg void OnUpdateCameraJumpto(CCmdUI* pCmdUI);
	afx_msg void OnRenderSound();
	afx_msg void OnUpdateRenderSound(CCmdUI* pCmdUI);
	afx_msg void OnOptionsShowAvatar();
	afx_msg void OnUpdateOptionsShowAvatar(CCmdUI* pCmdUI);
	afx_msg void OnCameraGravity();
	afx_msg void OnUpdateCameraGravity(CCmdUI* pCmdUI);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnToolsMetaverse();
	afx_msg void OnUpdateToolsMetaverse(CCmdUI* pCmdUI);
	afx_msg void OnToolsLibrary();
	afx_msg void OnUpdateToolsLibrary(CCmdUI* pCmdUI);
	afx_msg void OnOptionsPreferences();
	afx_msg void OnToolsVrml2Convert();
	afx_msg void OnUpdateToolsVrml2Convert(CCmdUI* pCmdUI);
	//}}AFX_MSG

	afx_msg void OnCameraViewpoint(UINT id); 
	afx_msg void OnUpdateCameraViewpoint(CCmdUI* pCmdUI);
	afx_msg void OnOptionsDriver(UINT id); 
	afx_msg void OnUpdateOptionsDriver(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG	// debug version in myglview.cpp
inline CMyglDoc* CMyglView::GetDocument()
   { return (CMyglDoc*) m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
#endif
