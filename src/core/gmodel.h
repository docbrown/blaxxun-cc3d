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
#ifndef _GMODEL_H
#define _GMODEL_H
/******************************************************************************
@doc

@module gmodel.h - GLView data model (document) class|

Copyright (c) 1995 - 1998 by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GModel>
<c GView>

Notes:

Changes:

$Revision: 1.33 $
$Log: gmodel.h,v $
Revision 1.33  1998/09/28 15:44:33  tom
*** empty log message ***

Revision 1.32  1998/09/22 16:37:33  holger
empty message


Todo :

******************************************************************************/

class RenderState;
class RGBAColor;

#ifdef _OGL
#ifdef _GLMESA
	class GXMesaDevice;
#endif

#ifdef _WIN32
	class GOpenglDeviceWGL;
#endif
#endif



class GShell;
class GWorld;
class GTraversal;
class GCamera;


class GDeform;
class GParameter;

class GvNode;
class GvScene;

class GvScript;

class GvViewpoint;
class GvTimeSensor;
class GvMovieTexture;
class GvAudioClip;
class GvSound;

class GvMenuSensor;
class GvLight;
class GvFog;
class GvBackground;
class GvRotation;
class GvNavigationInfo;
class Gv2Inline;

class GHitInfo;
class GvEvent;

class GConvexVolume;
class rapidCollide;

class GEventHandler;


class GCameraInterpolator;

class GSoundDevice;

#include "www.h"


#ifdef _GV

#include <gvsubnode.h>

#include <gvnode.h>

#include <gvsfnode.h>
#include <gvmfnode.h>
#include <gvsfbool.h>

#include <gvsffloat.h>
#include <gvsfint32.h>
#include <gvsftime.h>
#include <gvsfstring.h>
#include <gvsfrotation.h>
#include <gvsfvec2f.h>
#include <gvsfvec3f.h>
#include <gvmfstring.h>
#include <gvmfrotation.h>
#include <gvmfvec3f.h>
#include <gvmffloat.h>

#include <gvpath.h>


#endif

#include "quaternation.h"

#include "gcamera.h"
#include "gcamerautil.h"
#include "gfuzzy.h"

class GMultitSensorHandler;

#ifdef WIN32
// for _OGL Amovie support
#ifndef __DDRAW_INCLUDED__
struct IDirectDraw;
typedef struct IDirectDraw			FAR *LPDIRECTDRAW;
#endif
#endif


// create a colored wheel

GShell *NewWheel(float r=1.0,int parts = 6,float h=0.2,float r2factor=0.7);


//@class a 3d model
class GModel  {
public :
};


class GFile;
class GPanel;


class GvSceneInfo;
class GvNode;
class GvInfo;
class GvExternProto;


// set of flags, for choosing the "right" output device 
class GDriverHints 
{
public  :


	CString m_driver;			// preferenced Driver, default "Ramp Driver"
	CString m_fullScreenMode;	// preferenced full screen mode 

	BOOL  m_useHW;				// use a HARDWARE driver if present
	BOOL  m_useFullscreen;		// switch to fullscreen !

	// D3D specific crap 
	BOOL  m_useRgb;				// use a RGB capable driver if present

	BOOL  m_onlySystemMemory;	// use only system memory for all surfaces
 	BOOL  m_onlyEmulation;		// use no hardware 

	GDriverHints() 
	{

			m_useHW = FALSE; // TRUE;
			m_useFullscreen = FALSE;

		    m_useRgb = FALSE;	

			m_onlySystemMemory = FALSE;
			m_onlyEmulation = FALSE;
	}

};




class GLoadTask { // not used 
public:
	int status;

	GFile file;

	GvNode *node;

	GLoadTask * next;

	GLoadTask() { next = NULL;}
};


class CMyglView;
struct _TK_RGBImageRec;

class GViewController;

// callback flags if running in "observered" enviroments 

typedef enum {
		// callback via observer interface // must match flags in blaxxunCC3D.odl
		GOBSERVE_MESSAGE = 0x01,		// observer want's all status messages
		GOBSERVE_ANCHOR  = 0x02,		// execution of anchors
		GOBSERVE_VIEWPOINT = 0x04,		// viewpoint message
		GOBSERVE_URLLOADING = 0x08,		// observers loads URL's to file for us
		GOBSERVE_URLERRORS = 0x10,		// observer wants URL errors

		// send message via sendMessage to observer window
		GOBSERVE_WM_MESSAGE = 64,		// same but using window messages
		GOBSERVE_WM_ANCHOR  = 128,
		GOBSERVE_WM_URLLOADING = 256,

} GBrowserObserverFlags;

#define GOBSERVER_WM_MASK		 0x0000FFC0
#define GOBSERVER_OBSERVE_MASK	 0x0000003F


//! flags returned by selection processing 
typedef enum {

		//! found a sensor
		SELECT_SENSOR_BIT = 2,

		//! found a anchor
		SELECT_ANCHOR_BIT = 4,

		//! found some collision
		SELECT_COLLISION_BIT = 8,

		//! found a sensor which needs a mouse drag
		SELECT_DRAG_SENSOR_BIT = 16

} GBrowserSelectionFlags;


//! event mask for VRML input handling

typedef enum {

	GEV_mousedown	= 1,
	GEV_mousemove	= 1<<1, 
	GEV_mouseout	= 1<<2,  
	GEV_mouseover	= 1<<3, 
	GEV_mouseup		= 1<<4, 
	
	GEV_keydown		= 1<<5, 
	GEV_keyup		= 1<<6, 
	GEV_keypress	= 1<<7, 
	GEV_click		= 1<<8, 
	GEV_dblclick	= 1<<9, 

	GEV_joystick	= 1<<10, 

	GEV_ALL			= 0x0FFFF,
	GEV_MOUSE		= GEV_mousedown | GEV_mousemove | GEV_mouseout | GEV_mouseover | GEV_mouseup,
	GEV_KEY			= GEV_keydown | GEV_keyup | GEV_keypress,
	GEV_CLICK		= GEV_click | GEV_dblclick ,
	GEV_NONE		= 0

} GBrowserInputEventFlags;




/*!a view of a 3D model
   derived from GvNode for browser interface !

*/

class GView : public GvNode 

#ifdef _COM
	// implements the Browser methods 
	,public CComDualImpl<Browser, &IID_Browser, &LIBID_GLViewVrmlLib>
#endif

{

#ifdef _GV

    GV_NODE_HEADER(GView);

#endif


public:

#ifdef _COM
	BEGIN_COM_MAP(GView)
		COM_INTERFACE_ENTRY(Browser)
		COM_INTERFACE_ENTRY_CHAIN(GvNode)
	END_COM_MAP()

	DECLARE_NOT_AGGREGATABLE(GView)

		// Browser Interface methods

		// IUnknown 
		STDMETHOD_(ULONG, AddRef)() {return InternalAddRef();}
		STDMETHOD_(ULONG, Release)()
		{ 
			ULONG l = InternalRelease(); 
			if (l == 0) delete this; 
			return l;
		}

		STDMETHOD(QueryInterface)(REFIID iid, void ** ppvObject)
		{return _InternalQueryInterface(iid, ppvObject);}

		//
        STDMETHOD(getName) (BSTR *value);
        STDMETHOD(getVersion) (BSTR *value);
        STDMETHOD(getCurrentSpeed) (float *value);
        STDMETHOD(getCurrentFrameRate) (float *value);
        
        STDMETHOD(getWorldURL)(BSTR *value);
        
        STDMETHOD(replaceWorld)(Node *value);
        
        STDMETHOD(loadURL)(int cntUrl,
            /* [size_is][in] */ BSTR *url,
            int cntParameter,
            /* [size_is][in] */ BSTR *parameter);
        
        STDMETHOD(setDescription)(BSTR description);
        
        STDMETHOD(createVrmlFromString)(BSTR vrmlSyntax,Node **value);
        
        STDMETHOD(createVrmlFromURL)(BSTR url, Node *node, BSTR event);
        
        STDMETHOD(getNode)(BSTR name,Node **value);
        
        STDMETHOD(addRoute)(Node *fromNode, BSTR fromEventOut, Node *toNode, BSTR toEventIn);
        
        STDMETHOD(deleteRoute)(Node *fromNode,BSTR fromEventOut,Node *toNode,BSTR toEventIn);
        
        STDMETHOD(createNode)(BSTR nodeclass,Node **value);
        
        STDMETHOD(getWorld)(Node **value);
        
        STDMETHOD(beginUpdate)(void);
        
        STDMETHOD(endUpdate)(void);
        
        STDMETHOD(addNode)(Node *node);
        
        STDMETHOD(removeNode)(Node *node,VARIANT_BOOL *value);

        
        STDMETHOD(setNodeName)(Node *node, BSTR name);
        
        STDMETHOD(getNodeName)(Node *node, BSTR *name);
        
        STDMETHOD(getEventOut)(BSTR eventOutName, EventOut **value);
        
        STDMETHOD(getEventIn)(BSTR eventInName, EventIn **value);
        
        STDMETHOD(getControlPathname)(BSTR *directoryName);
        
        STDMETHOD(saveWorld)(BSTR fileName, VARIANT_BOOL *value);
        
        STDMETHOD(saveViewpoint)( BSTR viewpointName);
        
        STDMETHOD(setObserver)(IUnknown *observer,int flags,VARIANT_BOOL *value);
        
        STDMETHOD(setViewpointByValue)(float *position, float *orientation,float fov, VARIANT_BOOL animate);
        
        STDMETHOD(setFullscreen)(VARIANT_BOOL mode,VARIANT_BOOL *value);
        
        STDMETHOD(setCollisionDetection)(VARIANT_BOOL mode);
        
        STDMETHOD(setRenderMode)(BSTR mode);
        
        STDMETHOD(setNavigationMode)(BSTR mode);
        
        STDMETHOD(setViewpoint)(BSTR viewpointName);
        
        STDMETHOD(setHeadlight)(VARIANT_BOOL mode);
        
        STDMETHOD(setAvatarHeight)(float value);
        
        STDMETHOD(setCollisionDistance)(float value);
        
        STDMETHOD(setVisibilityLimit)(float value);
        
        STDMETHOD(setWalkSpeed)(float value);

        STDMETHOD(setNavigationPanel)(VARIANT_BOOL mode);
        
        STDMETHOD(getNavigationPanel)(VARIANT_BOOL *value);



		//! handle of observer 
		CComPtr<BrowserObserver> observer;
		HWND	observerWnd;
		int		observerFlags;

#endif


  public:


	//! exclusive scene access via critical section
	CCriticalSection locker;	

	int locked;
#ifdef _DEBUG
	BOOL Lock();
	BOOL Unlock();
#else 
	BOOL Lock() {locked++;  return locker.Lock(); }
	BOOL Unlock() {locked-- ; return locker.Unlock(); }
#endif

	gbool Locked() { return locked >0; }


	GModel *model;		//!< back pointer to a <c GModel>
	
	GFile *file;		//!< points to file info structure


	HWND hWnd;			//!< hwnd of view if needed (e.g. for file notification)

#ifdef _MFC
	CMyglView *pView;	//!< back pointer to view if needed
	void SetView(CMyglView *view) { pView = view; }

#endif

	//! object for passing back messages from view to controlling context
	GViewController *viewController;

	//! set the viewController
	void SetViewController(GViewController *ViewController)
	{	viewController = ViewController; }

	//! get the error report object 
	GReporter* GetReporter();

#ifdef _MFC	
	//! full screen mode helper window
	CFrameWnd* fullScreenWnd;
#endif



	// Panel 
	//! bitmap UI-Panel 
	GPanel	*panel;		

	//! geth the panel
	GPanel* getPanel() const { return panel;}

	//! is panel ok, (visible, not to small ) 
	gbool PanelOk() const;
	
	// set by the embed in IE
	//if false panel is always off
	BOOL m_navPanelOn_Prop;
	
	//set by registry
	//panel on startup?
	//if false panel is always off
	BOOL m_navPanelOn_Reg;
	
	//the current status
	//m_navPanelOn = m_navPanelOn_Reg & m_navPanelOn_Prop;
	BOOL m_navPanelOn;

	//is valid for one scene, is used for the NONE mode in navigationInfo
	BOOL m_navPanelOn_NavInf;

  	//void GetWindowPos(CPoint point);

	void NavigationPanelDestroy();
	void NavigationPanelCreate(int flush);

	void setNavigationPanel(gbool mode);
    gbool getNavigationPanel();
	void setNavigationPanelProp(gbool mode);
	void setNavigationPanelNavInf(gbool mode);


	GFuzzyLogic FuzzyLogic;

#ifdef _DEBUG
//hg #define _FC_LOG
#ifdef _FC_LOG
	ofstream frameRate, budget;
#endif _FC_LOG
#endif _DEBUG

#if 0
	// idea 

	// caching mode , DONT_CACHE, CACHE_ALL, CACHE_MEDIA
	int defaultUrlCachingMode;
	int urlCachingMode;

	void SetUrlCachingMode(int newMode);
	int  GetUrlCachingMode();

	// load url to cache 
	gbool LoadUrlToCache(const char *url);

	// is the url in cache ?
	gbool IsUrlInCache(const char *url);
	
	// set the last modified date of an url
	void SetUrlModifiedDate(const char *url,time_t date);
	
	void MakeUrlAbsolute(GvString &url);
#endif



    
	//! displays a message to the status bar with update
    void Message(const char *message,int messageType = 0);

	//! get the type of the last message 
	int GetLastMessageType();

	//! print a message to the console
	void Print(const char *message);


	// Redraw control 
	
	//! calls directly Redraw on parent (Invalidate)
	int		Redraw();		


	//! redraw flag for browser 
	GvBool mustRedraw;

	//! set redraw flag, marks node for redraw e.g. on field changes
	inline void setRedraw() { mustRedraw = TRUE; } 
	
	//! clear redraw flag
	void clearRedraw() { mustRedraw = FALSE; } 
	
	//! get redraw flag
	inline GvBool getRedraw() const { return mustRedraw; } 

	//////////////////////////////////////////////


	gbool firstUpdate;	//!< flag for first update 


	// currently dummy, idea check for events during rendering 
	int		CheckEvents();


	gbool	useGlCache;		//!< flag Use GL Cache ()
	gbool	LightMode;		//!< lighted wireframe / vertices mode
	gbool	NormalMode;		//!< display normals
	gbool	TextureMode;	//!< use textures
	int		TextureFilteringMode; //!< texture filtering 0 point 1 linear
	float	NormalScale;	//!< scale factor for displaying normals 
	gbool	TransparencyMode;	//!< use transparency

	gbool	bspMode;		//!< use BSP rendering settings 
	int		traversalOrder;	//!< BSP traversal order 
	int		traversalInlineLoadingOrder;	//!< BSP traversal order for loadinlines
	gbool	viewCulling;  //!< do view culling 

	
	gbool headLightOn;		//!< add a head light 

	gbool useSceneLights;	//!< use lights in VRML scene
	float lightScale;		//!< global light scale factor 
	
	gbool useSceneSpotLights; //!< use spot lights
	gbool useInitialSceneCamera; //!< use initial scene camera

	gbool enableCollisionDetection; //!< enable CD as default 
	gbool enableStayOnGround; //!< enable stay on ground as default 

	//! set / get lightScale factor
	int SetLightScale(float factor);
	float GetLightScale();

	//! set headlight 
	void SetHeadLight(gbool flag) { if (flag != headLightOn) { headLightOn = flag; Redraw(); }}
	//! get headlight status 
	gbool GetHeadLight() { return (headLightOn); }

#ifndef _COM	
	void setHeadlight(gbool flag);
        
    void setAvatarHeight(float value);
        
    void setCollisionDistance(float value);
        
    void setVisibilityLimit(float value);
        
    void setWalkSpeed(float value);

#endif
	


	//! get bspMode 
	gbool	GetBspMode() { return (bspMode); }
	
	//! set bspMode 
	void	SetBspMode(gbool mode);

	//! set new navigation mode 
	void	SetNavigationMode(const char* mode);

	//! get navigation mode 
	void	GetNavigationMode(CString &mode);


	gbool vrml2;	//!< is this a VRML 2 scene flag 

	// GvNode enables
	gbool	useTextures;		//!<use VRML texture nodes	
	gbool	useShapeHints;		//!<use VRML texture nodes	
	gbool	useMaterials;
	gbool	useMovies;			//!<use VRML movie texture nodes	
	
	gbool	useBackground;		//!< use/render background nodes
	gbool	hasSoundDevice;		//!< TRUE until createSoundDevice fails

	gbool	useSound;			//!< use sound 
	gbool	useRSXSound;		//!< use Intel RSX sound 
	gbool	hasRSXSound;		//!<  Intel RSX sound installed ?
	int		soundQuality;		//!< sound quality see GSound.h
	int		maxNumSounds;		//!< max number of parallel playing sounds 
	gbool	soundDoppler;		//!< compute doppler effects 


	int maxUserTextureSize;		//!< max allowed users textureSize


	void SetUseTextures(gbool flag);
	void SetUseMovies(gbool flag);

	void SetMaxUserTextureSize(int mode);



	gbool	reportErrors;		//!< report VRML Errors 
	int		errorLevel;
	int		maxInlineDepth;		//!< maximum nestable inline level
	gbool	incrementalUpdates;	// Experimental : update screen after inline reading

	gbool	updateIsComplex;	//!< flag if update take a long time ==> wait cursor
	gbool	triggerUrlLoading;	//!< flag, if true scedule an url load 

	// load modes for different node types
	int		inlineLoadMode;
	int		textureLoadMode;
	int		movieLoadMode;
	int		soundLoadMode;

	//int inlineLoadFlags;			// loader flags 
	int		inlineWwwFlags;				//!< flags for WWW GFIle loader
	
	int		lastFrameLoadCnt;			//!< inlines /textures loaded on last frame
	int		lastFrameLoadErrorCnt;		//!< how many load errors on last frame 
	int		lastFrameInlineLoadCnt;		//!< inlines loaded on last frame
	int		lastFrameScriptLoadCnt;		//!< scripts loaded on last frame
	int		lastFrameTextureLoadCnt;	//!< textures loaded 
	int		lastFrameSoundLoadCnt;		//!< sounds loaded on last frame
	int		lastFrameProtoInstanceCnt;	//!< protos instanced during last frame

	int		maxFrameInlineLoadCnt;		//!< max # inlines to load per frame 
	int		maxFrameScriptLoadCnt;		//!< scripts loaded on last frame
	int		maxFrameTextureLoadCnt;	    //!< textures loaded 
	int		maxFrameSoundLoadCnt;		//!< sounds loaded on last frame


	//!< clear loader cnts 
	void ClearCounts() {
		lastFrameLoadCnt = 0;
		lastFrameLoadErrorCnt=0;
		lastFrameInlineLoadCnt=0;
		lastFrameScriptLoadCnt=0;	
		lastFrameTextureLoadCnt=0;
		lastFrameSoundLoadCnt=0;
		lastFrameProtoInstanceCnt=0;
		updateIsComplex = FALSE;
	}


	int anchorPreloadMode;	// preload anchors

		
	int initialTextureMode;		//!< start texturing mode for VRML scenes, 0 don´t load 1 ask 2 load
	int initialTextureFilteringMode;//!< start filter mode 
	int initalRenderingMode;  // initial rendering mode for VRML scenes

	float resetZoomFactor;
	float ignoreAllInlines;	// flag , abort any inline loading

	// set useGLCache optimization flag 
	gbool SetUseGlCache(gbool flag) {
					if (useGlCache == flag) return(flag);
					useGlCache = flag;
					if (!flag) InvalidateModelCache();
					return(useGlCache); 
	}



	//! the current animation time
	GTime time;
	gbool playRealtime; // use real time stepping

	double loadTime;	// the global time, the scene was loaded

	// get current VRML2 time (secs from 1970)
	double Now() const {
		if (playRealtime) 
			return(GTime::GetGlobalTime()); // use system time
		else return(loadTime + time.t);
	}
	
	gbool autoPlay;		// start  animations on scene loading

	gbool AutoPlay() { return(autoPlay); }
	
	void SetTargetFrameRate(float targetFR);


	double theTime; // the current VRML simulation time 
	GvSFTime time_changed; // the global time field

	// set the global animation time
	double SetTheTime();

	// get current event timestamp
	inline double getTime() const
	{ 
		return theTime; 
	}; 

	// get pointer to variable where current event simulation time is stor
	inline double* getTimePtr()
	{ 
		return &theTime; 
	}; 



	// timeSensors are triggered at beginning of frame
	// if this flag is true, sensors have been already triggered for this frame
	gbool thisFrameTimeSensorsTriggered;

	// trigger time sensors	  
	// Result : TRUE there are TimeSensors, FALSE, no timeSensors in scene
	gbool TriggerTimeSensors();

	// pending scripts, scripts where the eventsProcessed function need to be called 
	GvMFNode pendingScripts;

	void  initPendingScripts();	
	void  termPendingScripts();
	
	// call the eventsProcessed function and clear pendingScripts
	GvBool callPendingScripts();


	//
	// 3D Device / Renderstate 
	//

	//the gl <c RenderState> object, maintains/reflects current rendering settings
	RenderState renderState;


#ifdef _OGL
#ifdef _GLMESA
    GXMesaDevice *device;
#endif

#ifdef _WIN32
	GOpenglDeviceWGL *device;
#endif
#endif


#ifdef _D3D

	//! the D3D <c GD3Device> object, maintains/reflects current rendering settings
	GD3Device *device;

	// globals
	//HINSTANCE hInstApp;     /* application instance for dialog boxes */

    BOOL bSingleStepMode;        /* render one frame at a time */
    BOOL bDrawAFrame;            /* render on this pass of the main loop */
    BOOL bClearsOn;              /* clear the back and z-buffer each frame */
    BOOL bShowFrameRate;         /* show the frame rate at the top */
    BOOL bShowInfo;              /* show window information at the bottom */
    BOOL bResizingDisabled;      /* do not allow resizing */

    BOOL bResized; /* the window has resized or some other drastic change, the
                      entire client area should be cleared */
    BOOL bQuit;    /* program is about to terminate */

    LPDIRECTDRAWSURFACE lpFrameRateBuffer; /* frame rate surface */
    LPDIRECTDRAWSURFACE lpInfoBuffer;      /* window info surface */


	// callbacks from device 
	static BOOL AfterDeviceCreated(int w, int h, LPDIRECT3DVIEWPORT* lplpViewport, LPVOID lpContext);
	
	BOOL AfterDeviceCreated(int w, int h, LPDIRECT3DVIEWPORT* lplpViewport);
	static BOOL BeforeDeviceDestroyed(LPVOID lpContext);
	BOOL BeforeDeviceDestroyed();


	/* create the D3D device */
	BOOL CreateD3DDevice(
			 HWND hwnd,HWND hwndFrame,
			 const char *driver= NULL,	// preferenced Driver, default "Ramp Driver"
			 GDriverHints *driverHints = NULL);

	/*
     * InitView
     * Builds execute buffer and all components needed to be added to viewport.
     */

	BOOL InitView();


    /*!
     * Release all objects and free all memory allocated in InitView
     */

	void ReleaseView();

    /*!
     * Release all objects and free all memory allocated in InitScene
     */

	void ReleaseScene();



	/*
	* RestoreSurfaces
	* Restores any lost surfaces.  Returns TRUE if all surfaces are not lost and
	* FALSE if one or more surfaces is lost and can not be restored at the
	* moment.
	*/
	BOOL RestoreSurfaces();


	/*
	* CleanUpAndPostQuit
	* Release all D3D objects, post a quit message and set the bQuit flag
	*/
	void CleanUpAndPostQuit();

	void ReportD3DError();
	void G__cdecl Error( LPSTR fmt, ... );

#endif

	//! release all handles to textures 
	void ReleaseTextures();


#ifdef WIN32

#ifdef _OGL
    LPDIRECTDRAW           lpDD;         /* DirectDraw object, needed for Actvie Move */
#endif

	// get pointer to DDRAW object 
	LPDIRECTDRAW GetDirectDraw();

#endif

	//
	// sound stuff
	//
	GSoundDevice *soundDevice; // the sound device 
	gbool m_playSoundStarted;	// we have started some PlaySound stuff in PlaySoundFile

	// get sound device, initialize if needed 
	GSoundDevice *GetSoundDevice();
	
	// initialize the sound device
	// return TRUE if initialized, GetSoundDevice() woud return non NULL soundevice
	gbool InitializeSound();

	// release the sound device
	void ReleaseSound();

	// set sound mode flag 
	// setting to false will kill sounddevice and sounds
	gbool SetUseSound(gbool flag);

	// get sound mode flag 
	gbool GetUseSound();

	// do we have a sound device,
	// detected on first try to initialize
	gbool HasSoundDevice();

	// play a sound file
	gbool PlaySoundFile(const char *fileName);

	// delayed execution of replaceWorld / browser.loadURL

	GvBool	   hasNewScene;	
	GvSFNode   newScene; // the new scene to be loaded

	GvBool	   hasNewUrl;	// we should load a new url

	GvMFString newUrl;
	GvMFString newUrlParams;
	GvString   newUrlBase;	


	//check for replaceWorld loadURL requests 
	gbool CheckForSceneChange();


	//
	// Model functions
	//

	// The Model
	//! the model data set as one single <c GShell> object 
	GShell *theShell;

	//! original version of the model data set, if deformations has been applied
	GShell * backupShell;

	//! the model data set as a scene tree (obsolete, used only for DXF Input)
	GWorld *theWorld;

	//! the VRML scene
	/* GvScene  */ 
	GvNodeHandle theScene;

	//! get top level scene node 
	GvScene* GetScene() const { return (GvScene*) (GvNode*) theScene; };

	//! is  top level scene level VRML 2.0 ? 
	gbool IsVrml2(); 


	// flag that a scene info update is necessary , 1: needed for UI, 2 : after rendering because of lights / scripts
	int doUpdateSceneInfo;
	int doUpdateBBox;

	//! information about the scene
	GvSceneInfo *sceneInfo;

	//! get the scene info 
	GvSceneInfo * GetSceneInfo(); 

	int activeInlinesLimit;		//!< limit of how many inlines should be active
	float activeInlinesPurge;	//!< factor of how much old inlines should be purge

	//! register a new TimeSensor
	void Register(GvTimeSensor *s);

	//! register a new MovieTexture
	void Register(GvMovieTexture *s);

	//! register a new AudioClip
	void Register(GvAudioClip *s);

	//! register a new Sound
	void Register(GvSound *s);

	//! register a special media node 
	void RegisterSpecialMedia(GvNode *s);

	//! register a new MenuSensor
	void Register(GvMenuSensor *s);

	//! register a new Script
	void Register(GvScript *s);

	//! register a new Viewpoint
	void Register(GvViewpoint *s);

	//! register a new Light
	void Register(GvLight *s);

	//! register a new Inline
	void Register(Gv2Inline *s);

	// Register special nodes 
	void RegisterNode(GvNode *n);
	void RegisterNode(GvMFNode &nodes);


	//! do we have texture nodes ?
	gbool SceneHasTextures();
 

	//! get the model scene tree
	GWorld* GetWorld() const { return(theWorld); }

	// terminate current scene
	int  TermScene();
		
	//! set the current model to <c GShell> object shell
	int SetShell(GShell *shell,int resetCamera=1);

	//! set the current model to <c GvNode> object 
	int SetNode(GvNode *root,int resetCamera=1);

    // set the current model to <c GvScene> object 
    int SetScene(GvScene *scene,gbool bindNodes=FALSE,int resetCamera=1);

	// bind initial scene nodes 
	void BindScene();


    // reset all settings to the VRML 2 defaults 
    void ResetSceneDefaults();
	
	// set scene to the initial default state
	// bind initial nodes etc.
	int SetSceneDefaults();


	// get path to installation directory 
	CString GetInstallDirectory();


	//! the current Camera of the view
	GCamera *camera;

	//! return a pointer  to the current Camera of the view
	GCamera &GetCamera() { return (*camera); }

	// additional matrix transforming camera to the global camera
	MatrixAndInverse cameraMatrix;
	
	// camera transformed by camera matrix 
	GCamera *globalCamera; 

	GCamera &GetGlobalCamera() { return (*globalCamera); }

	GViewInfo viewInfo; // current view transformation

	// third person mode active ?
	gbool	thirdPersonView;

	// type of floating camera
	GvSFInt32	thirdPersonMode;

	// otherAvatars view
	gbool ThirdPersonView() const { return thirdPersonView; }
	
	// get type of third person mode
	int getThirdPersonMode() { return thirdPersonMode.get(); }

	GCamera thirdPersonFollowCamera;

	// the camera for 3d person mode
	GCamera *physicalCamera;

	// camera interpolator 
	GCameraInterpolator physicalCameraInterpolator;

	// must phyiscal camera animated ?
	gbool AnimatingPhysicalCamera() { return physicalCameraInterpolator.alpha < 1.0f; }	

	// get the real amera used for 3d display  ( 3dperson view / stereo etc)
	GCamera *GetPhysicalCamera() 
	{
		if (thirdPersonView) return physicalCamera;
		else return globalCamera;
	}

	// 3rd person mode camera offset variables 
	//Point followDolly;
	//Point followOrbit;
	//Point followPan;

	GvSFVec3f followDolly;
	GvSFVec3f followOrbit;
	GvSFVec3f followPan;

	// update the physical camera for a changed globalCamera
	gbool UpdatePhysicalCamera(gbool cameraChanged);

	// set third person mode on (will offset the camera)
	gbool SetThirdPersonView(gbool flag);


	// set camera transform transforming camera to globalCamera 
	void SetCameraMatrix(const Matrix &m);
	void SetCameraMatrixIdentity();

	// update global camera from camera + cameraMatrix
	void UpdateGlobalCamera();

	// update default z-range of global camera 
	void UpdateDefaultZRange();

	// update navigations values from  cameraMatrix
	void UpdateGlobalNavigationValues();

	//! the current event handler of the view
	GEventHandler *eventHandler;
	// stack 

	//! flush the current event handlers of the view
	void FlushEventHandlers();

	//! push a new event handler to the view
	void PushEventHandler(GEventHandler *eventHandler);

	//! pop event handler from the view
	GEventHandler *PopEventHandler();

	//! set new top event handler of the view
	void SetEventHandler(GEventHandler *eventHandler);

	//! get top event handler from the view
	GEventHandler *GetEventHandler() { return eventHandler; }


	// convert a pixel coordinate to normalized screen coordinate (-1..1) (-1 ..1)
	void Pixel2Screen(const CPoint &pixel,Point &screen,float screenZ = 0.0);

	// convert a normalized screen coordinate to pixel 
	void Screen2Pixel(const Point &screen,Point &pixel);

	// convert a normalized screen coordinate to world coordinates
    int Screen2World(const Point &screen, Point &world) {  return viewInfo.View2World(screen,world); }
    int World2Screen(const Point &world, Point &screen) {  return viewInfo.World2View(world,screen); }

	
    int Cam2World(const Point &screen, Point &world) {  return viewInfo.Cam2World(screen,world); }
    int World2Cam(const Point &world, Point &screen) {  return viewInfo.World2Cam(world,screen); }

	// compute world ray for screen coordinate 
	void ComputeWorldRay(const Point &screen,RayRange &ray);

	

	float viewStepSpeed;	// step factor for camera moves
	float viewStepSpeedLocal;	// step factor for camera moves
	float viewAngleSpeed;	// factor for camera rotations 

	// ??Matrix viewpointTransform; // transfrom of viewpoint 
	// from navigation Info
	// local values are transformed by viewpoint transform to global values 

	gbool allowAnyNavigation;	// allow navigation mode switching & viewpoint control 
	gbool collisionDetection; // currently collision detection active
	gbool stayOnGround; // currently stay over ground activated 

	void setCollisionDetection(gbool flag) { collisionDetection = flag; }
	gbool getCollisionDetection() { return collisionDetection; }
	
	void setGravity(gbool flag) { stayOnGround = flag; }
	gbool getGravity() { return stayOnGround; }

	void setViewpointAnimation(gbool flag) { cameraAnimate = flag; }
	gbool getViewpointAnimation() { return cameraAnimate; }


	Point downDirection; // the direction vector point to ground 
	Point downDirectionLocal;

	float visibilityLimitNear;
	float visibilityLimitNearLocal;

	float visibilityLimit;
	float visibilityLimitLocal;
	float collisionDistance;
	float collisionDistanceLocal;
	float heightOverGround;
	float heightOverGroundLocal;
	float stepOverSize;
	float stepOverSizeLocal;

	
	//the rapid-stuff
	gbool useRapid;//use Rapid collision-detection lib

	GShell *speedBox;//the shape that represents the avatar and its movement (local coordinates)
	GShell *speedBox2;
	GShell *speedBoxGlobal;//the shape that represents the avatar and its movement (global coordinates)
	rapidCollide *myRapid;	// rapid helper structure

	GConvexVolume *speedBoxVolume;	// convex volume for BBox test

	float getVisibilityLimit() { return visibilityLimit; }
	float getCollisionDistance() { return collisionDistance; }
	float getAvatarHeight() { return heightOverGround; }
	float getStepOverSize() { return stepOverSize; }
	float getZNear() { return globalCamera->znear; }
	float getZFar() { return globalCamera->zfar; }


	gbool autoZRange;		// automatically recompute z-range 

	gbool collisionPlane;	// enable collision test with ground plane 

	
	BBox	bbox;	// last computed scene bbox, recomputed eg. by UpdateBbox

	// update scene bounding Box
	int UpdateBoundingBox();

	// update scene bounding Box incrementally 
	int UpdateBoundingBox(BBox &box);

	//! load all inlines 
	// traverse scene graph and trigger loads 
	int LoadAllInlines(int oneOnly,gbool inlines,gbool textures=gfalse);

	// traverse nodes and trigger loads 
	int PrefetchNodes(GvMFNode &nodes, int oneOnly=gfalse,gbool inlines=gtrue,gbool textures=gtrue);

	// evaluate & set the permission level for the script
	void GetScriptPermissions(GvScript *script, const char *url);

	int CheckInlineUnload();
	
	//! read a model from a 3d file
	int ReadModel(const char *FileName,int fmt,GFile *theFile=NULL);
	
	// read a model from any known file type
	// including unzip
	int ReadModel(const char *FileName); 

    // Create a VRML scene out of single object model (shell) 
    GvNode* CreateVrmlScene(gbool vrml2=FALSE);
	
	// read a model from any known file type
	// including unzip
	// view will own file structure if sucessufll
	int ReadModel(GFile *file); 

	// get absolute home url
	const char * GetHomeUrl() const;

	// get the absolute url
	const char * GetUrl() const;


	//! read a single shell object
	int ReadShell(const char *FileName,int fmt);

	//! build a shell by assigning a image texture to a rectangle
	int ReadTexturedPolygon(const char *FileName,int format);

	//! load a global texture
	int SetTexture(const char *FileName,int format=0);

    //! load a global texture
	int EditTexture(const char *FileName,int format=0);

	//! unload the global texture
	int ClearTexture();

    CString textureFile;    // name of current texture image file 
    
	const char *GetTexture() const { return(textureFile); }

	//! Reset the camera to the default view (x-y plane)
	void CameraReset();

	//! set the camera to the Camera node , with optional transformation matrix
	int  SetCamera(GvNode *cameraNode,const Matrix *cameraTransform=NULL,int animate=0);
    
    // set viewpoint viewpoint index
    int SetViewpoint(int index,int animate=0);


    //! set the camera to the Camera node , with optional transformation matrix
	int  AnimateToCamera(GvNode *cameraNode,const Matrix *cameraTransform=NULL);
	int  AnimateToCamera(GvNode *cameraNode,GvPath &cameraPath);

	// state variables for camera animation
    GCameraInterpolator *cameraInterpolator;

    GvSFNode cameraAnimateTargetViewpoint;		// target animation viewpint 
	GvPath cameraAnimateTargetViewpointPath;	// target viewpoint path

    
    gbool cameraInterpolateAll; // flag 
    gbool cameraInterpolateAllCycle; // flag 
    int cameraInterpolateCurrent; // current viewpoint index
    int cameraInterpolateEnd; // end viewpoint index

   
    gbool cameraAnimate;  // animate cameras on viewpoint change 
    
    gbool AnimatingCamera() { return cameraInterpolator != NULL; }

    int AnimateCameraStep();
   
    // stop camera animation 
    void AnimateCameraStop();

    // animate through all camera 
    void AnimateAllCameras();

    //  current viewpointIndex
    int viewpointIndex;

    // goto prev/next viewpoint
    int SetNextViewpoint(int animate=0);
    int SetPrevViewpoint(int animate=0);
    
    // set viewpoint viewpoint index without change in camera
    int SetViewpointIndex(int index);
    
    // return current viewpoint index
    int GetViewpointIndex(int &index) { index = viewpointIndex; return(1); }

    // return current viewpoint index
    int GetViewpointIndex() { return viewpointIndex; }

    // return name of current viewpoint 
    const char * GetViewpointName();

	// camera follows picked object 
	gbool followObject;

	// the path to the object
	// checked for changed transformations
	GvPath followObjectPath;
	Point followObjectPosition;	// local position of object to be followed 
	Point followObjectPositionGlobal;	// local position of object to be followed 
	Point followObjectCameraPosition; // camera position in local space 
	Point followObjectNormal;

	void SetFollowObjectOff(); // switch off follow object mode 

							   /*
	set follow object mode given a node and a reference point 
	result FALSE if node null or not found in tranform hierarchy
	*/

	gbool SetFollowObject(GvNode *node,const Point &p);



	GvSFNode sharedZone;		// SharedZone 

	gbool showAvatar;			// show my avatar, i.e. it is part of scene

	GvSFString myAvatarURL;			// the filename for myAvatar 
	GvSFString myAvatarName;		// the name of my avatar

	// 3d person mode bound avatar 
	GvSFNode boundAvatar;		// the avatar bound and added to scene at top-level
	GvSFNode myAvatar;			// node source for my Avatar

	void set_myAvatarURL(GvSFString *event);

	// eventIn , callback for createVrmlFromURL
	void set_myAvatarNodes(GvMFNode *event);
	void set_myAvatarName(GvSFString *event);
	void set_showMyAvatar(GvSFBool *event);
	
	// issue predefined guesture number 1.. nn
	void set_myAvatarGesture(GvSFInt32 *event);
	GvSFInt32 myAvatarGesture; // event out for sending guesture
	
	GvMFFloat myAvatarAvatarSize; // avatar Size info for my avatar  (used instead of NavigationInfo


	// helper class storing avatar eventIn's for 3rd person mode
	
	class GAvatarInfo { 
	public :		
		
		GAvatarInfo() : avatarSetTranslation(-1),avatarSetRotation(-1),avatarSetScale(-1),avatarSetChoice(-1),
			avatarAvatarSize(-1)
		{
		}
		~GAvatarInfo() 
		{
		}


		// default guestures 
		GvNodeHandle nodeGesture;
		int avatarSetGesture; // SFInt32

		IntArray gestures;
		GvMFString *gestureNames;

		int avatarGestureChanged; // SFInt32
		GvNodeHandle avatarGestureChangedRoute; // the route from avtar to browser

		int avatarAvatarSize; // MFFloat
		GvNodeHandle avatarAvatarSizeRoute; // the route from avatar to browser

		// transform

		GvNodeHandle nodeTransform;

		// event codes Ins of avatar
		int avatarSetTranslation; // SFVec3f
		int avatarSetRotation;	  // SFRotation
		int avatarSetScale;		  // SFVec3f

		// switch 
		GvNodeHandle nodeChoice;
		int avatarSetChoice;	  // SFInt32

		// extended guestures 
		GvNodeHandle nodeEvents;

		int avatarEnterWorld;		// SFTime
		int avatarLeaveWorld;		// SFTime

		int avatarBeginMove;		// SFTime
		int avatarEndMove;			// SFTime

		int avatarBeginCollision;		// SFTime
		int avatarEndCollision;			// SFTime

		int avatarBeginFly;			// SFTime
		int avatarEndFly;			// SFTime

		int avatarBeginJump;		// SFTime
		int avatarEndJump;			// SFTime

		int avatarBeginClick;		// SFTime

		// one time handled
		//	eventIn SFString set_url
		//	eventIn SFString set_nickname
		//  eventIn SFBool isPilot
		// e
	
	};
	
	GAvatarInfo avatarInfo;


	GvMFRotation avatarRotation;
	GvMFVec3f avatarTranslation;
	
	// bind new avatar 
	void BindAvatar(GvNode* avatar);

	// load avatar from file 
	int LoadAvatar(const char *avatar);

	// show avatar, load if needed
	gbool ShowAvatar(gbool mode);

	// is avatar shown
	gbool IsAvatarShown();

	// set avatar gesture
	void SetAvatarGesture(GvNode* node,int eventId);


    // a set of created viewpoints
    GvMFNode viewpoints;
    
    // delete list of viewpoints
    void FlushViewpoints();

	// VRML2 bindable nodes 
    GvSFNode boundViewpoint; // the bounded viewpoint node (top of stack)

	// the path to the bound viewpoint
	// checked for changed transformations
	GvPath boundViewpointPath;

    GvMFNode boundViewpointStack;		// list of stacked Viewpoints

	gbool boundViewpointChanged; // flag : 1, viewpoint has been changed 
	
	// the path to the bound viewpoint
	// checked for changed transformations

	void BindViewpoint(GvViewpoint *newViewpoint);
	void BindViewpoint(GvViewpoint *newViewpoint,GvBool bind);
    
	GvSFNode boundNavigationInfo; // the bounded NavigationInfo node (top of stack)
	GvPath boundNavigationInfoPath; 

	GvMFNode boundNavigationInfoStack;	// list of stacked NavigationInfos
	gbool boundNavigationInfoChanged; // flag : 1, fog  has been changed 

	void BindNavigationInfo(GvNavigationInfo *newNavigationInfo);
	void BindNavigationInfo(GvNavigationInfo *newNavigationInfo,GvBool bind);

    GvSFNode boundFog;	  // the bounded fog node (top of stack)
    GvPath boundFogPath;
    GvMFNode boundFogStack; // list of stacked Fog Nodes
	gbool boundFogChanged; // flag : 1, fog  has been changed 

	void BindFog(GvFog *newFog);
	void BindFog(GvFog *newFog,GvBool bind);
    
	GvSFNode boundBackground;
	GvPath boundBackgroundPath;
	GvMFNode boundBackgroundStack;
	gbool boundBackgroundChanged; // flag : 1, background  has been changed 

	void BindBackground(GvBackground *newBackground);
	void BindBackground(GvBackground *newBackground,GvBool bind);
	
	// update the camera used for VRML 2.0 Background Node rendering 
	void UpdateBackgroundCamera(Vector axis,float angle);

	// empty all VRML 2 bounded nodes
	void FlushBoundedNodes();

	// the currently edited node 
	GvSFNode editNode;
	GvPath editNodePath;

	// list of scene nodes with Proto definitions (for EXTERNPROTO)
	GvMFNode protoScenes;


	// find proto for extern proto with url "url", and optional name "name"
	GvProto* getProto(const char *url,const char *name);

	// empty proto scenes 
	void FlushProtoScenes();
	
	// add scene to proto scenes
	void addProtoScene(GvScene *protoScene);


	GvMFNode pendingExternProtos; // list of pending externprotos

	// add externproto to list of unloaded externprotos
	void RegisterPending(GvExternProto *proto);

	// remove externproto to list of unloaded externprotos
	void UnRegisterPending(GvExternProto *proto);

	// empty pending externproto's
	void FlushPendingExternProtos();




	// get node, specified by urn "urn:inet:glview.com: ...."
	GvNode* newUrnInstance(const char *url);


	//! set the camera to the named Camera 
	int  SetCamera(const char *cameraName) { return SetCamera(cameraName,cameraAnimate); }
	int  SetCamera(const char *cameraName,int animate);

	int  SaveCamera(const char *cameraName);
	
	// save current camera into camera node 
	int  SaveCamera(GvNode *cameraNode);
	
	int  SaveCamera(const char *cameraName,float t);
	
    int  SetCamera(float t);

#ifdef _OGL
	//! cached version of model
	glCache modelCache;


	//! invalidate cached version of model, if any
	// necessary if model data have been changed

	void InvalidateModelCache() { modelCache.Invalidate(); }
#else 
    void InvalidateModelCache() { };
#endif


	//! set default gl texture parameter generation, if no
	// texture parameters are present for theShell
	void SetDefaultTextureParameters();

	void FlushBackupShell();


	// query functions for button checking
	gbool GetDithering();
	gbool GetLightMode();
	gbool GetNormalMode();
	gbool GetBackfaceCulling();
	gbool GetFrontFaceCW();
	gbool GetLocalViewer();
	gbool GetLocalColors();
	gbool GetTexturing();
	gbool GetTransparency();
	gbool GetLineAntialiasing();
	gbool GetAntiAliasing();
	int GetRenderMode();
	gbool GetTwoSidedLighting();
	gbool GetComputePrimitiveList();


	//! set the current render mode to a predefined <t GRenderMode> setting
	int SetRenderMode(GRenderMode newMode);
	
	// set global render mode to mode
	int SetRenderMode(const char * newMode);


	gbool SetNormalMode(gbool);
	gbool SetDithering(gbool);
	gbool SetLightMode(gbool mode);
	gbool SetBackfaceCulling(gbool mode);
	gbool SetFrontFaceCW(gbool mode);
	gbool SetLocalViewer(gbool mode);
	gbool SetTwoSidedLighting(gbool mode);
	gbool SetLocalColors(gbool mode);
	gbool SetTexturing(gbool mode);

	// set texture filtering 0 : nearest : 1 : bilinear
	int SetTextureFiltering(int mode);
	int GetTextureFiltering() { return TextureFilteringMode; }

	int SetTextureMipMap(int mode);
	int GetTextureMipMap();


	gbool SetTransparency(gbool mode);
	gbool SetLineAntialiasing(gbool mode);
	
	gbool SetAntiAliasing(gbool mode);

	gbool SetComputePrimitiveList(gbool mode);

	// Toggles for User-Interfaces callbacks

	int ToggleNormalMode();
	int ToggleLightMode();
	int ToggleBackfaceCulling();
	int ToggleFrontFaceCW();
	int ToggleLocalViewer();
	int ToggleTwoSidedLighting();
	int ToggleLocalColors();
	int ToggleTexturing();

	int ToggleTransparency();

	int ToggleLineAntialiasing();
	int ToggleComputePrimitveList();


	// fullscreen support 
	int GetFullScreen();
	int SetFullScreen(int mode);
	BOOL m_bTestFullScreen;

	int ToggleFullScreen();

	// query for capabilities, see GCaps.h for definition
	int GetCap(int cap);

	// set/get a global browser option
	gbool SetOption(const char *option,const char *value);
	gbool GetOption(const char *option,GvString &value);



#ifdef _OGL
	gbool fullScreen;

#ifdef WIN32
	HMENU savedFrameMenu;
	long savedFrameWinStyle;
	long savedWinStyle;
#endif

#endif



	// interface for material edit

	int SetMaterial(Point &Ambient, Point &Diffuse, Point &Specular,Point &Emission,float &Shininess,float &Alpha);
	int GetMaterial(Point &Ambient, Point &Diffuse, Point &Specular,Point &Emission,float &Shininess,float &Alpha);
	int SetBackMaterial(Point &Ambient, Point &Diffuse, Point &Specular,Point &Emission,float &Shininess,float &Alpha);
	int GetBackMaterial(Point &Ambient, Point &Diffuse, Point &Specular,Point &Emission,float &Shininess,float &Alpha);

	//! read the current material from a material file
	CString material;
	const char *GetMaterial() { return(material); }

	int ReadMaterial(const char * filename, int format);
	int ReadMaterial(int fmt,const char *FileName);

	// Background color
	int GetBackgroundColor(Point &c);
	int SetBackgroundColor(const Point &c);
	int EditBackgroundColor(const Point &c);

	// Ambient Light color / intensity 
	int GetAmbientLight(CString &c);
	int SetAmbientLight(const char *c);


	//! set all OpenGL State variables
	// used to render to a bitmap context

	int SetContext(int canDisplayShareLists=0);


	//! start incremental updates
	int BeginIncrementalUpdates();

	//! get some settings from profile 
	void GetProfileSettings();


	//! initialize, given a window handle
	// <0 initialization failed, exit app
	int Initialize(HWND hWnd,HDC hDC,
			GDriverHints *driverHints = NULL
#ifdef _D3D_XXX
			 ,const char *driver= NULL,	// preferenced Driver, default "Ramp Driver"
			 BOOL  bOnlySystemMemory = FALSE,
 			 BOOL  bOnlyEmulation = FALSE
#endif
		);
	
	/*!
		Terminate view, restart with Initialize
		free up major resources, including D3D device and scene
	*/
	void Term();




#ifdef _D3D

	G3LightRef headLight; // headLight

#endif

	gbool lightInit; // need initializations of lights 



	//! define default lights
	int DefineLights(const Point &dirx,int headLight, int &firstLightIndex);

	DWORD frameCnt;		  // FRAME CNT 	

	DWORD lastFrameTime; // in milli secons 

#define NUM_LAST_FRAME_TIMES 8

	//! time in MS of the last N Frames 
	int lastFrameTimes[NUM_LAST_FRAME_TIMES];
	//! number of triangles in the last N Frames 
	int lastFrameTris[NUM_LAST_FRAME_TIMES];
	//! number of drawing primitives  in the last N Frames 
	int lastFramePrims[NUM_LAST_FRAME_TIMES];
	
	//! current write index 
	int lastFrameTimesI;

	//! current sum over the 8 elements 
	int lastFrameTimesSum; 


	// Auto LOD 
	GFrameStats lastFrameStats;		//!< total stats of last frame
	GFrameStats frameStats;			//!< accumulated frame stats
	float nurbsTessellationScale;	//!< scale factor for nurbs tesselation


	float targetFrameRate;			//!< Dynamic Tessellation target frame rate, 30 FPS
	float targetFrameRateDegrade;	//! 30 FPS - during navigation
	int nurbsTessellationMode;		//!< type of nurbs tesselation mode
	gbool autoTesselation;

	void UpdateNurbsTesselation();

	float lodScale;					//!< user scale factor adapting LOD ranges

	//! set user lod scale 
	void SetLodScale(float factor);


	//! render the model
	void DrawScene(RECT &rc);

	//! resize rendering area to rectangle 
	void Resize(int width,int height);

	//! handle a resize message (OnSize)
	void Resize(HWND hWnd);

	//! handle a resize to rectangle 
	void Resize(RECT &rect);

	GvSFFloat windowAspect; //!< window aspect field 
	GvSFVec2f windowSize;	//!< window size field 
	GvSFNode  event;		//!< VRML event structure
	GvSFInt32 eventMask;	//!< for now as SFInt32


	// get window properties
	int getWindowSizeX();
	int getWindowSizeY();

	float getWindowAspect();

	// get pressed status of VIRTUAL key code
	// TBD: where are UNICODE keycode defines ??
	gbool isKeyPressed(int key);

	// the mask of needed input events, avoid e.g. mousemoves
	// GBrowserInputEventFlags eventMask; // for now as SFInt32
	// list of observers / outgoing routes

	
	// parse an given filter string and compute an event mask
	static int parseEventMask(const char *filter);
	static const IS GEV_table[];



	//! get event structure
	GvEvent* getEvent() const { return (GvEvent*) event.get(); };
	void  triggerEvent() { event.touch(); }; // trigger ROUTES
	gbool triggerEvent(const char *type); //

	gbool triggerMouseEvent(GBrowserInputEventFlags mask, const char *type, float x,float y, unsigned int flags);
	
	gbool triggerMouseEvent(GBrowserInputEventFlags mask,const char *type,POINT client, POINT screen, unsigned int flags);
	
	gbool triggerKeyboardEvent(GBrowserInputEventFlags mask,const char *type,unsigned int code, unsigned int flags);
	gbool triggerSizeEvent(GBrowserInputEventFlags mask,const char *type,float x,float y, unsigned int flags);


	// callbacks with MFC Cwindow signature
	void OnSize(UINT nType, int cx, int cy);

	void OnMove(int cx, int cy);

	void OnActivate();



	//! the current deformer object
	GDeform * deformer;

	//! the current shell parametrization object
	GParameter *parameter;

	//! build one shell from the tree model & replace current model
	int BuildShell();

	// Compute a shell for the world
	GShell* ComputeShell();


	// do background node rendering
	gbool doBackground;
	// node (LayerNode) for background rendering 
	GvNodeHandle background;

	// Rotation Node for applying cameraOrientation for VRML 2.0 Background
	GvNodeHandle backgroundRotation;

	// do foreground / overlay node rendering
	gbool doForeground;
	
	// node (LayerNode) for foreground rendering 
	GvMFNode foreground;


	int SetBackground(GvNode *node=NULL);

	int SetBackground(GvRotation *rotation,GvNode *node);

	//int SetForeground(GvNode *node=NULL);
	
	void addForeground(GvNode *node);
	void removeForeground(GvNode *node);
	void ClearForeground();


	// clear any background
	void ClearBackground();

	CString backgroundImage;    // name of current background image 
	const char *GetBackgroundImage() { return(backgroundImage); }

	// Set background by creating a layer node
	int SetBackgroundImage(const char *FileName,int format = UNKNOWN_FMT);

	// Edit background + edit scene BackgroundImage node
	int EditBackgroundImage(const char *FileName,int format = UNKNOWN_FMT);

	int RenderBackground();


	// set viewer (Navigation) mode 
	int SetViewerMode(const char *mode);


	// action on do do on select processing
	enum SelectionAction {
		INFO,		   	// Display info about object

		INFO_WWWAnchor, // display WWW Anchor information // move
		JUMP_WWWAnchor, // execute www anchor (double click)			

		DRAG_SENSORS,	// execute www anchor / handle sensors 	// mouse down		

		JUMP_OBJECT,	// jump to object			
		FOLLOW_OBJECT,	// follow object			
		SELECT,			// select current object 
		DRAG,			// drag current object 
		RELEASE,		// drag current object 
	};

	// do selection processing
	int Select(int x, int y, int flags, int action);

	// execute a hit element
	// x,y,flags are the orignial data from the mouse event
	// msg is the message to be displayed
	// sensors contains the list of sensor eventhandlers to be activated
	int DoHitElement(SelectionAction action, int x, int y, int flags, GHitInfo &hit,GvNode *hitNode,CString &msg,GMultitSensorHandler* & sensors);

	// detect a Ray-based collision
	// result 1: collisionPosition contains new position 
	int CollisionDetect(const Point &oldPosition, const Point &newPosition,  Point &collisionPosition,Point &collisionNormal);

	// result 1: newPosition contains new position 2:gravity, falling down
	int GroundDetect(const Point &oldPosition,  Point &newPosition,Point &hitPosition,Point &hitNormal); 

	// execute the url 
	int ExecuteUrl(const char *url);
	int ExecuteUrl(GvSFString &url);
	int ExecuteUrl(GvMFString &url);



	// the current traversal in progress
	GTraversal *currentTraversal;

	// last hit information stored by Select
	GHitInfo *lastHit;

	// the current set of touchsensors with isOver == TRUE 
	GvMFNode overTouchSensors;

	// the current set of touchsensors with isActive == TRUE 
	GvMFNode activeTouchSensors;

	// the matrix of the over touchSensors
	MatrixAndInverse overTouchSensorsMatrix;

	gbool hasCustomSensorMessage; // setDescription message is sent from VRML via JavaScript

	// node to be executed from Anchor
	GNode *executeNode;

	// the current set of visibility sensors with isActive == TRUE 
	GvMFNode activeVisibilitySensors;

	// the current set of active zones with isActive == TRUE 
	// e.g. Occlusion
	GvMFNode activeZones;

	// inactive visibility sensors, not visible any more 
	void InActivateVisibilitySensors();

	// inactive zone  not visible any more 
	void InActivateZones();



	int needUpdate;		// flag scene need refresh, scanned in CMYglView::OnIdle()


	int abortAllLoads;	// ignore any following inlines / texture load requests

	// set moving mode 
	gbool moving;			// avatar  is moving
	int SetMoving(gbool mode); 
	gbool Moving() const  { return(moving); }

	// set colliding mode 
	gbool colliding;			// avatar is colliding
	int SetColliding(gbool mode); 
	gbool Colliding() const  { return(colliding); }

	// set flying mode 
	gbool flying;			// avatar is flying
	int SetFlying(gbool mode); 
	gbool Flying() const  { return(flying); }

	float startRenderTime;  // time rendering has started

	// Degrading 
	int degradeDuringMove;
	int autoDegrade;	// automatically try to degrade
	int degrade;	// flag, sceen should be rendered degraded
	int degraded;	// current display is rendered degraded

	float degradeLastFrameTime;

	float degradeMaxFrameTime;

	// rendering mode flags for degraded mode 
	GRenderMode degradeRenderMode;
	gbool degradeTextured;
	gbool degradeTransparency;
	gbool degradeLightMode;


	void SetDegradeDefaults();

	// start degrading on next update
	int BeginDegrade();

	// end degrading 
	int EndDegrade();

	// saved rendering mode flags for degraded mode 
	GRenderMode degradeSaveRenderMode;
	gbool degradeSaveRenderTextured;
	gbool degradeSaveRenderTransparency;
	gbool degradeSaveRenderLightMode;

	// extended customized OGL rendering mode
	RenderFlags degradeSaveVertices;
	RenderFlags degradeSaveNormals;
	RenderFlags degradeSaveEdges;
	RenderFlags degradeSaveFaces;
	RenderFlags degradeSaveBboxes;

	// set rendering state to degraded rendering mode
	void SetDegradeMode();

	// restore rendering state to state before degraded rendering mode
	void UnSetDegradeMode();


	GLoadTask *tasks;

	int AddTask(GLoadTask *newTask,int where=0);
	void ClearTasks();
	int CheckTasks();

	// update scene info
	int UpdateSceneInfo(gbool fullInfo=FALSE);

	// update scene info incrementally 
	int UpdateSceneInfo(GvNode *node); 


	// Traverse current scene
	// update pointer to camera and defaults nodes in traversal
	int Traverse(GTraversal &traversal); 

	// Traverse a specific node 
	int Traverse(GTraversal &traversal,GvNode *theNode); 


	// Traverse world without the avatar 
	int TraverseWithoutAvatar(GTraversal &traversal); 


	// default nodes
	GvSFNode defaultAppearance; // appearance if appearance field is NULL
	GvSFNode defaultMaterial; // material if apperance material is NULL	(part of defaultAppearance)
	GvSFNode defaultTextureTransform; // textureTransform if apperance textureTransform is NULL	(part of defaultAppearance)
	GvSFNode defaultTexture; // texture if apperance textureTransform is NULL	(part of defaultAppearance)

	GvSFNode defaultMaterial10; // default material for VRML 1.0 style 

	// create above default nodes
	void InitDefaultNodes();

	// update values from current state
	void UpdateDefaultNodes();


	void ResetDefaults();

	// notification  functions
	// a node has been added to scene
	void OnNodeAdded(GvNode *node);
	
	// node will be deleted to scene
	void OnNodeDeleted(GvNode *node);

	// node has been changed
	void OnNodeChanged(GvNode *node);


#ifdef _GV
// browser fields


// browser methods 
	GvSFString name;
	GvSFString version;
	GvSFString worldUrl;

	// Event Outs in global coordinates 
	GvSFVec3f viewpointPosition;
	GvSFRotation viewpointOrientation;

	//
	// script browser  methods 
	//

    const char * getName(); 

    const char * getVersion(); 

    float getCurrentSpeed(); 

    float getCurrentFrameRate(); 

    const char *getWorldURL(); 

	void replaceWorld(GvMFNode *nodes); 

    GvMFNode* createVrmlFromString(const char* vrmlSyntax,GvScript *scriptNode); 

    void createVrmlFromURL(GvMFString *url, GvNode *node, const char *event,GvScript *scriptNode); 

    void addRoute(GvNode *fromNode, const char *fromEventOut, GvNode *toNode, const char *toEventIn); 

    void deleteRoute(GvNode *fromNode, const char *fromEventOut, GvNode *toNode, const char *toEventIn); 

    void loadURL(GvMFString *url, GvMFString *parameter,GvScript *scriptNode); 

    void setDescription(const char *description); 

    void notifyWhenLoaded(GvMFNode *nodesToLoad, GvNode *nodeToNotify, const char *eventInToNotify); 

	// set the viewpoint depending on mode
	int setViewpointByValue(Point position,Rotation orientation,int mode);
	int getViewpointByValue(Point &position,Rotation &orientation,int mode);

	//
	//
	//


	// some child node has been changed
	int OnChildChanged(GvNode *child);

	// library managment

	GvSFNode		library; // a GvScene node
	GvBool			libraryLoaded; // library loaded ?

	// load global library scene 
	GvBool LoadLibrary(const char *fileName);

	// get global library scene / load if not loaded 

	GvScene *GetLibrary();
	GvMFNode *GetLibraryObjects();
	GvMFNode *GetLibraryMaterials();
	GvMFNode *GetLibraryAppearances();

#endif




};

#endif


