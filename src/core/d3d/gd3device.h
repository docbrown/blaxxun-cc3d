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

#ifndef _GD3Device_h
#define _GD3Device_h

#include "grenderbase.h"


#include <ddraw.h>
#include <d3d.h>

#include "d3dmacs.h"



/*
 * MACROS
 */
#undef ATTEMPT
#define ATTEMPT(x) if (!(x)) goto exit_with_error
#undef RELEASE
#define RELEASE(x) if (x) { x->Release(); x = NULL; }

#undef MAX
#define MAX(x, y) ((x) > (y)) ? (x) : (y)
#undef MIN

#define MIN(x, y) ((x) > (y)) ? (y) : (x)

#undef ZEROMEM
#define ZEROMEM(x) memset(&x, 0, sizeof(x))

/*
 * DEFINES
 */
#define D3DAPP_WINDOWMINIMUM 50     /* smallest buffer size allowed */
#define D3DAPP_DEFAULTWINDOWDIM 320 /* replaces window size if invalid */
#define D3DAPP_MINBUFFERSIZE 15360  /* minimum "maximum buffer size" for a
                                       D3D driver to be accepted */
#define D3DAPP_MINVERTEXCOUNT 320   /* minimum "maximum vertex count" for a
                                       D3D driver to be accepted */
#define D3DAPP_MAXDDDEVICES 10		/* max number of DD devices to find */ 

#define D3DAPP_MAXD3DDRIVERS 15      /* maximum Direct3D drivers ever expected
                                       to find */
#define D3DAPP_MAXTEXTUREFORMATS 20 /* maximum texture formats ever expected
                                       to be reported by a D3D driver */
#define D3DAPP_MAXMODES 60          /* maximum display modes ever expected to
                                       be reported by DirectDraw */
#define D3DAPP_MAXTEXTURES 15       /* maximum number of textures that wil be
                                       loaded and managed */
#define D3DAPP_MAXCLEARRECTS 30     /* maximum num. rectangles (ie extents)
                                       for clearing */
#define D3DAPP_BOGUS -100           /* unused parameters accept this */
#define D3DAPP_YOUDECIDE -25        /* Use this for certain parameters to
                                       have D3DApp decide an appropriate
                                       value for you */
#define  D3DAPP_USEWINDOW -24        /* Used in place of fullscreen mode */

									   /* CreateFromHWnd : Valid flags:
 *    D3DAPP_ONLYSYSTEMMEMORY  Force all surfaces into system memory.  Also
 *                             disables hardware DD and D3D drivers.
 *    D3DAPP_ONLYD3DEMULATION  Disable D3D hardware
 *    D3DAPP_ONLYDDEMULATION   Disable DD hardware
 */
#define D3DAPP_ONLYSYSTEMMEMORY 0x00000001
#define D3DAPP_ONLYD3DEMULATION 0x00000002
#define D3DAPP_ONLYDDEMULATION  0x00000004
#define D3DAPP_ONLYWINDOW		0x00000008  // HG force a DD device with Window support 

#define D3DAPP_ONLY_HW			0x00000010  // choose HW 
#define D3DAPP_ONLY_RGB			0x00000020  // choose RGB

#define D3DAPP_DXLEVEL3 3
#define D3DAPP_DXLEVEL5 5
#define D3DAPP_DXLEVEL6 6


/*
 * DATA STRUCTURES
 */

/*
 * D3DAppD3DDriver structure
 * Describes a D3D driver
 */
typedef struct tagD3DAppDDDevice {
    CString Name;      /* short name of the driver */
    CString NameUi;    /* short name of the driver  used for User Interface*/
    CString About;     /* short string about the driver */
	DDCAPS DriverCaps, HELCaps;
    GUID Guid;          /* it's GUID */
    BOOL bIsHardware;   /* does this driver represent a hardware device? */
    BOOL bCanDoWindow;  /* can it render to Window's display depth? */
    BOOL bIsPrimary;  
} D3DAppDDDevice;

/*
 * D3DAppD3DDriver structure
 * Describes a D3D driver
 */
typedef struct tagD3DAppD3DDriver {
    CString Name;      /* short name of the driver */
    CString NameUi;    /* short name of the driver  used for User Interface*/
    CString About;     /* short string about the driver */
    D3DDEVICEDESC Desc; /* D3DDEVICEDESC for complete information */
    GUID Guid;          /* it's GUID */

    BOOL bIsHardware;   /* does this driver represent a hardware device? */
	BOOL bIsMmx;		// is this the mmx driver ? 
    BOOL bDoesTextures; /* does this driver do texture mapping? */
    BOOL bDoesZBuffer;  /* can this driver use a z-buffer? */
    BOOL bCanDoWindow;  /* can it render to Window's display depth? */

	BOOL bUseSystemMemory; // this driver works best with system memory (e.g. MMX)
	BOOL bUseTextureFiltering; // this driver might do fast bilinear filtering

} D3DAppD3DDriver;

/*
 * D3DAppTextureFormat stucture
 * Describes a texture format
 */
typedef struct tagD3DAppTextureFormat {
    DDSURFACEDESC ddsd; /* DDSURFACEDESC for complete information */
    BOOL bPalettized;   /* is this format palettized */
    int RedBPP;         /* number of red, */
    int BlueBPP;        /*           blue, */
    int GreenBPP;       /*           green,  */
    int AlphaBPP;       /*           and alpha bits per pixel */
    int IndexBPP;       /* number of bits in palette index */
} D3DAppTextureFormat;

/*
 * D3DAppMode structure
 * Describes a display mode
 */
typedef struct tagD3DAppMode {
    int     w;                /* width */
    int     h;                /* height */
    int     bpp;              /* bits per pixel */
    BOOL    bThisDriverCanDo; /*can current D3D driver render in this mode?*/
} D3DAppMode;



class GD3Device  : public GRenderDevice {

public:

	// constructor 
	GD3Device();
	~GD3Device();



    HWND                    hwnd;          /*handle of window being managed*/
	HWND					hwndFrame;     /* hg frame window */ 
	HWND					hwndParent;     /* parent of hwnd  */ 
	HWND					hwndFullScreenWnd;

	HWND GetHWnd() {return (hwnd); }

    /*
     * Direct3D objects and information
     */
    LPDIRECT3D              lpD3D;         /* D3D object */
    LPDIRECT3DDEVICE        lpD3DDevice;   /* D3D device */
    LPDIRECT3DVIEWPORT      lpD3DViewport; /* D3D viewport, created by application */

	// DX 5 Support
	BOOL dx5;
    LPDIRECT3D2              lpD3D2;         /* D3D 2 object */
    LPDIRECT3DDEVICE2        lpD3DDevice2;   /* D3D 2 device */
    LPDIRECT3DVIEWPORT2      lpD3DViewport2; /* D3D 2 viewport, created by application */


	// DX 6 Support
	BOOL dx6;
    LPDIRECT3D3              lpD3D3;         /* D3D 3 object */
    LPDIRECT3DDEVICE3        lpD3DDevice3;   /* D3D 3 device */
    LPDIRECT3DVIEWPORT3      lpD3DViewport3; /* D3D 3 viewport, created by application */

	// DX 7 Support
	BOOL dx7;
    LPDIRECT3D7              lpD3D7;         /* D3D 7 object */
    LPDIRECT3DDEVICE7        lpD3DDevice7;   /* D3D 7 device */

	int						dxLevel; // the Direct X Level to use, 0 highest


	D3DAppDDDevice			Devices[D3DAPP_MAXDDDEVICES]; /* avail. devices */
    
	int                     NumDevices;    /* number of DD devices avail. */
	int						PrimaryDevice;
	int						FullScreenDevice;
	int						CurrDevice;


    int                     NumDrivers;    /* number of D3D drivers avail. */
    int                     CurrDriver;    /* number of curr. D3D driver */
    
	D3DAppD3DDriver         Driver[D3DAPP_MAXD3DDRIVERS]; /* avail. drivers*/
    D3DAppD3DDriver         ThisDriver;    /* description of this driver,
                                           identical to Driver[CurrDriver] */

    int                     SoftwareDriver;    /* number of software driver in case of problems*/
    int                     FullscreenDriver;    /* number of fullscreen driver or -1 if using default */
    int                     NumTextureFormats; /* num texture formats avail*/
    int                     StartTextureFormat; /* texture format choosen at start */
    int                     CurrTextureFormat; /* current texture format
								will only change if driver changes or when app changes it*/
    D3DAppTextureFormat     TextureFormat[D3DAPP_MAXTEXTUREFORMATS];
                                      /* description of all avail. formats */
    D3DAppTextureFormat     ThisTextureFormat; /* description of this format,
                             identical to TextureFormat[CurrTextureFormat] */

    D3DAppTextureFormat     RgbTextureFormat; /*  prefered rgb texture format  HG  */ 

#if 0

    int                     NumTextures;    /* number of textures loaded */
    char                    ImageFile[D3DAPP_MAXTEXTURES][50]; /* files */
    D3DTEXTUREHANDLE        TextureHandle[D3DAPP_MAXTEXTURES]; /* handles */
    LPDIRECTDRAWSURFACE     lpTextureSurf[D3DAPP_MAXTEXTURES]; /* surfaces */
    LPDIRECT3DTEXTURE       lpTexture[D3DAPP_MAXTEXTURES]; /* texture objs */
    BOOL                    bTexturesInVideo; /* are textures in video
                                                 memory? */
#endif

	BOOL					coopLevelWasSet; // HG coop level was already set on device    

	/*
     * DirectDraw objects and information
     */
    LPDIRECTDRAW            lpDD;          /* DirectDraw object */
    LPDIRECTDRAW4			lpDD4;         /* The DirectDraw object */
	DDDEVICEIDENTIFIER		deviceId;	   // Direct Draw device id


    BOOL                    bIsPrimary;    /* Is this the primary DD device?
               If FALSE, we're using a hardware DD device that cannot
               display a window and so only fullscreen modes are available */

    LPDIRECTDRAWSURFACE     lpFrontBuffer; /* front buffer surface */
    LPDIRECTDRAWSURFACE     lpBackBuffer;  /* back buffer surface */
    LPDIRECTDRAWSURFACE     lpZBuffer;     /* z-buffer surface */

	LPDIRECTDRAWSURFACE4     lpFrontBuffer4; /* front buffer surface */
	LPDIRECTDRAWSURFACE4     lpBackBuffer4;  /* back buffer surface */
	LPDIRECTDRAWSURFACE4     lpZBuffer4;     /* z-buffer surface */



    BOOL                    bBackBufferInVideo; /* back buf in video mem? */
    BOOL                    bZBufferInVideo;    /* is Z-buf in video mem? */

    int                     NumModes; /* number of available display modes */
    int                     CurrMode; /* number of current display mode (only
                                         when fullscreen) */
    D3DAppMode              Mode[D3DAPP_MAXMODES]; /* desc avail modes */
    D3DAppMode              ThisMode; /* description of this mode, identical
                                         to Mode[CurrMode] */
    BOOL                    bFullscreen; /* in fullscreen exclusive mode? */
    D3DAppMode              WindowsDisplay; /* current Windows disply mode */

    SIZE                    szClient;         /* dimensions of client win */
    POINT                   pClientOnPrimary; /* position of client win */

	RECT					rcScreenRect;     // Screen rect for window
    RECT					rcViewportRect;   // Offscreen rect for VPort

    BOOL                    bPaused;           /* the app is paused */
    BOOL                    bAppActive;        /* the app is active */
    BOOL                    bTexturesDisabled; /* textures are disabled */
    BOOL                    bOnlySystemMemory; /* all surfaces forced into
                                                  system memory */
    BOOL                    bOnlyEmulation;    /* no hardware DD or D3D
                                                  devices allowed */
    BOOL                    bMinimized;        /* app window is minimized */
    BOOL                    bRenderingIsOK;    /* All objects etc. necessary
                                                  for rendering are in ok */

public:

/*
 * FUNCTION PROTOTYPES
 */

/*
 * D3DAppCreateFromHWND
 *
 * Call this before all other D3DApp functions (except AddTexture).  
 * Initializes all DD and D3D objects necessary for rendering, enumerates the
 * available display modes and drivers and loads textures specified by prior
 * AddTexture() calls.  Caller passes the handle of the window to be manged
 * and callback functions to execute for device creation and destruction.
 * 
 * DeviceCreateCallback is executed AFTER the creation of D3D device and all
 * objects D3DApp created using the device.  This allows an application to
 * reconstruct the scene and create any additional objects.  The callback
 * must create and return (in the variable provided) the DIRECT3DVIEWPORT
 * from the given width and height.  The returned pointer is stored in the
 * D3DAppInfo structure and used for clearing and setting the render state.
 * A NULL pointer is fine if D3DApp is not used for either of these
 * functions. The create callback will always be called before any calls to
 * the destroy callback.  The boolean returned indicates success or failure.
 *
 * DeviceDestroyCallback is executed BEFORE the D3D device and objects
 * created by D3DApp using the device are released.  This allows an
 * application to save data regarding the scene or release any objects
 * created from the device before it is destroyed.  The DIRECT3DVIEWPORT
 * should be released in this callback.  The boolean returned indicates the
 * success or failure.
 *
 * A pointer to the internal D3DAppInfo data structure is returned.  This
 * should be READ ONLY!
 *
 * The DirectDraw device, Direct3D driver, display mode and texture format
 * will all be chosen by D3DApp.  Hardware DD and D3D devices are prefered.
 * Mono lighting D3D drivers are prefered.  Paletted texture formats are
 * prefered.  If possible, the current window size will be used, otherwise
 * a fullscreen mode will be selected.
 *
 * Call AddTexture() to add textures to be loaded upon initialization.
 *
 * Valid flags:
 *    D3DAPP_ONLYSYSTEMMEMORY  Force all surfaces into system memory.  Also
 *                             disables hardware DD and D3D drivers.
 *    D3DAPP_ONLYD3DEMULATION  Disable D3D hardware
 *    D3DAPP_ONLYDDEMULATION   Disable DD hardware
 */
	BOOL CreateFromHWND(DWORD flags, 
						  const char* driverName,	// name of driver or ""
					      int driver,
						  int mode,
						  HWND hwnd,HWND hwndFrame,
                          BOOL(*DeviceCreateCallback)(int, int,
                                                      LPDIRECT3DVIEWPORT*,
                                                      LPVOID),
                          LPVOID lpCreateContext,
                          BOOL (*DeviceDestroyCallback)(LPVOID),
                          LPVOID lpDestroyContext
						  );

/*
 * D3DAppWindowProc
 * To be truly effective, D3DApp should be allowed to trap incoming window
 * messages such as WM_SIZE.  Call D3DAppWindowProc at the begining of the
 * application's main window WindowProc with the message information.  If
 * bStopProcessing is set to TRUE, stop processing the message and return
 * lresult.
 */
BOOL WindowProc(BOOL* bStopProcessing, LRESULT* lresult, HWND hwnd,
                      UINT message, WPARAM wParam, LPARAM lParam);

	/*
	* D3DAppFullscreen
	* Places the app in a fullscreen mode using the current driver.
	*/
	BOOL FullscreenMode(int mode);


	/*
	set the default fullscreen mode
	*/
	BOOL SetDefaultFullscreenMode(int mode);

	BOOL SetDefaultFullscreenMode( 
						int     w,  
						int     h,  
						int     bpp);

	BOOL SetDefaultFullscreenMode(const char *mode); 
	BOOL GetDefaultFullscreenMode(CString &mode); 

					




	/*
	* D3DAppWindow
	* Places the application in windowed mode at the given client size.  If w
	* and h are D3DAPP_YOUDECIDE, D3DApp will decide on a suitable client size.
	* If called while in fullscreen, restores the display mode and returns the
	* hooked window to the size it was before a call to D3DAppFullscreen or to
	* the size specified.
	*/
	BOOL WindowMode(int w, int h);

	/*
	* ChangeDevice 
	* Changes the  DD device.  If the current display mode is incompatible with the
	* driver, a new one will be selected and employed.  A new texture format is
	* selected and textures are reloaded, hence their handles may change.  By
	* default, paletted formats are prefered.
	*/
	BOOL ChangeDevice(int device, DWORD flags);


	/*
	* ChangeDriver 
	* Changes the driver.  If the current display mode is incompatible with the
	* driver, a new one will be selected and employed.  A new texture format is
	* selected and textures are reloaded, hence their handles may change.  By
	* default, paletted formats are prefered.
	*/
	BOOL ChangeDriver(int driver, DWORD flags);

	// is current driver hardware driver ?
	BOOL IsHardwareDriver();

	// has device hardware driver ?
	BOOL HasHardwareDriver(DWORD flags);

	// change to first hardware driver 
	BOOL ChangeToHardwareDriver(DWORD flags);

	// get MMX or ramp driver 
	int GetBestSoftwareDriver(DWORD flags);

	// get HAL- MMX or ramp driver 
	int GetBestDriver(DWORD flags);

	// check if device is proabably ok 
	BOOL IsDriverOkForWindowMode(int driver,int w,int h, DWORD extraTextureMem); 


 
/*
 * D3DAppShowBackBuffer
 * Blts or flips the back buffer to the primary surface.  In the windowed
 * case, only the dirty portion of the front buffer is blt'ed over.  The
 * dirty region of front and back buffers is maintained by calls to
 * D3DAppRenderExtents(). D3DAPP_SHOWALL will force the entire front buffer
 * to be updated.
 */
#define D3DAPP_SHOWALL 0x00000001
BOOL ShowBackBuffer(DWORD flags);

/*
 * D3DAppRenderExtents
 * Tells D3DApp the extents of all regions updated on the back buffer as a
 * list of D3DRECTs (no more than D3DAPP_MAXCLEARRECTS).  Call this before
 * clearing the back buffer.  If the D3DAPP_CLEARALL flag is set, the extents
 * are ignored and the entire back buffer is assumed to have changed.
 */
#define D3DAPP_CLEARALL 0x00000001
BOOL RenderExtents(DWORD dwCount, LPD3DRECT extent, DWORD flags);

/*
 * D3DAppClearBackBuffer
 * Clears the back buffer and Z-buffer (if enabled).  D3DAPP_CLEARALL can be
 * used to clear the entire back buffer.
 */
#define D3DAPP_CLEARALL	 0x00000001
#define D3DAPP_CLEARBACK 0x00000002
#define D3DAPP_CLEARZ    0x00000004

	BOOL ClearBackBuffer(DWORD flags);

/*
 * D3DAppCheckForLostSurfaces
 * Checks all surfaces D3DApp has allocated and restores them if necessary.
 * An error is returned on any type of failure, but it may be best to ignore
 * it since restoring surface can fail for non-fatal reasons and the app may
 * just want to spin.
 */
BOOL CheckForLostSurfaces(BOOL &someLost);

/*
 * D3DAppPause
 * Use D3DAppPause(TRUE) to pause the app and D3DAppPause(FALSE) to unpause.
 * When fullscreen, the menu bar is redrawn.  bPaused is updated to reflect
 * the current status.
 */
int pausecount;
BOOL Pause(BOOL flag);

/*
 * D3DAppErrorToString
 * Converts a DirectDraw, Direct3D or Direct3D RM error code to a string.
 */
static char* ErrorToString(HRESULT error);

/*
 * D3DAppCreateSurface
 * Creates a surface described by ddsd.  Will force the surface into
 * systemmemory if D3DApp was initialized with D3DAPP_ONLYSYSTEMMEMORY.
 */
//BOOL 
HRESULT CreateSurface(DDSURFACEDESC *ddsd, LPDIRECTDRAWSURFACE *lplpSurf);
/*
 * D3DAppLastError
 * D3DAppLastErrorString
 * Returns the last D3DApp error as a string and HRESULT.
 */
HRESULT GetLastError();
char* GetLastErrorString();


/*
 * D3DAppDestroy
 * Destroys all objects including Direct Draw.  Call before program
 * termination.
 */
BOOL Destroy();

/*
 * Release all D3D Device and related objects
 */


void ReleaseDevice();



/*
 * GLOBAL VARIABLES
 *    see d3dapp.c for descriptions
 */


/* 
 * Callback functions for D3D device creation and destruction
 */
BOOL(*D3DDeviceDestroyCallback)(LPVOID);
LPVOID D3DDeviceDestroyCallbackContext;
BOOL(*D3DDeviceCreateCallback)(int, int, LPDIRECT3DVIEWPORT*, LPVOID);
LPVOID D3DDeviceCreateCallbackContext;
/*
 * The last error code and string
 */
HRESULT LastError;
char LastErrorString[256];
/*
 * List of dirty rectangles on back buffer and client area
 */
int NumDirtyClientRects, NumDirtyBackRects, NumDirtyZRects;
D3DRECT DirtyClient[D3DAPP_MAXCLEARRECTS];
D3DRECT DirtyBack[D3DAPP_MAXCLEARRECTS];
D3DRECT DirtyZ[D3DAPP_MAXCLEARRECTS];


LPDIRECTDRAWCLIPPER lpClipper; /* Clipper in windowed case */
LPDIRECTDRAWPALETTE lpPalette; /* Front buffer's palette */
PALETTEENTRY ppe[256];         /* Current palette entries */
PALETTEENTRY Originalppe[256]; /* Windows palette entries at startup */
BOOL bInitialized;				/* Is D3DApp initialized? */
BOOL bPrimaryPalettized;       /* Is the front buffer palettized? */
BOOL bPaletteActivate;         /* Is the front buffer's palette valid? */
BOOL bIgnoreWM_SIZE;           /* Ignore this WM_SIZE messages */
SIZE szLastClient;             /* Dimensions of the last window */
RECT szLastClientFrame;        /* Dimensions of the last mainFrame window */

// needed to backup window state before full screen  
HMENU lastFrameMenu;
LONG  lastFrameWinStyle;
LONG  lastWinExStyle;

SIZE szBuffers;                /* Current buffer dimensions, not necessarily
                                  the same as the client window */
int CallbackRefCount;          /* How many times DeviceCreateCallback has
                                  been called in a row */


BOOL CreateViewport(int w,int h);
void ReleaseViewport();

// set current viewport rectangle
void SetViewport(int x,int y,int width, int height);
// set current viewport to max rectangle
void SetViewport();

// get current viewport
void GetViewport(int &x,int &y,int &width, int &height);
 

 





private:
/*
 * INTERNAL FUNCTION PROTOTYPES
 */
BOOL SetRenderState();
BOOL EnumDrivers();
BOOL PickDriver(int* driver, DWORD depths);
BOOL CreateD3D();
BOOL EnumTextureFormats();
BOOL CreateZBuffer(int w, int h, int driver);
BOOL CreateDevice(int driver);




BOOL LoadAllTextures();
void ReleaseAllTextures();


BOOL SetCoopLevel(HWND hwnd, BOOL bFullscreen);
BOOL SetDisplayMode(int w, int h, int bpp);
BOOL CheckForPalettized();

BOOL RestoreDispMode();
BOOL VerifyDriverAndMode(int* lpdriver, int* lpmode);
BOOL FilterDrivers(int mode);

BOOL EnumDisplayModes();
BOOL PickDisplayMode(int* mode, DWORD depths);
BOOL SetDispMode(int w, int h, int bpp);
BOOL CreateDD(DWORD flags);
BOOL FilterDisplayModes(int driver);

BOOL CreateBuffers(HWND hwnd, int w, int h, int bpp,BOOL bFullscreen);

BOOL RememberWindowsMode();

public:
	BOOL SetPalette();

	BOOL ClearBuffers();

	HRESULT GetSurfDesc(LPDDSURFACEDESC lpDDSurfDesc, LPDIRECTDRAWSURFACE lpDDSurf);

	// called on WM_MOVE message to update primary surface rects
	void OnMove(int cx, int cy);

	// OnActivate :: WM_ACTIVATE, set palette
	void OnActivate();


	static DWORD BPPToDDBD(int bpp);

private:

	void ReleasePathList();

	LPDIRECTDRAWSURFACE LoadSurface(LPDIRECTDRAW lpDD, LPCSTR lpName,
                                       LPDDSURFACEDESC lpFormat,
                                       DWORD memoryflag);

	void SetClientSize(HWND hwnd, int w,int h,BOOL bReturnFromFullscreen);
	void GetClientWin(HWND hwnd);




	void SetDefaults();

	BOOL CallDeviceDestroyCallback();
	BOOL CallDeviceCreateCallback(int w, int h);

public:

	void MergeRectLists(int* dstnum, LPD3DRECT dst, int src1num,
                           LPD3DRECT src1, int src2num, LPD3DRECT src2);
	void CopyRectList(int* dstnum, LPD3DRECT dst, int srcnum,
                         LPD3DRECT src);

	void ValidateDirtyRects();
	
	BOOL HandleWM_SIZE(HWND hwnd, WPARAM wParam, int w, int h);

	// BOOL HandleWM_SIZE(LRESULT* lresult, HWND hwnd, UINT message,  WPARAM wParam, LPARAM lParam);
	void G__cdecl SetErrorString( LPSTR fmt, ... );

	/*
	* TotalVideoMemory
	* Returns the amount of total video memory supported (not free)
	*/
	DWORD TotalVideoMemory();

	/*
	* FreeVideoMemory
	* Returns the amount of total video memory supported (not free)
	*/
	DWORD FreeVideoMemory();

	/*
	* GetTextureMemory
	* Returns the amount of total / free texture memory supported 
	*/
	DWORD GetTextureMemory(BOOL total);

	// query for capabilities, see GCaps.h for definition
	int GetCap(int cap);


};

#endif
