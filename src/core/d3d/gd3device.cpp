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

@module GD3Device.cpp - Direct 3D device implementation |

Copyright (c) 1996-1998	by Holger Grahn 
All rights reserved

Purpose:

Classes:
<c GD3Device>


Notes:

  need Direct X 5 SDK to compile

Changes:

$Revision: 1.20 $
$Log: gd3device.cpp,v $
Revision 1.20  1998/08/20 17:22:38  holger
empty message


	change VerifyDriverAndMode to choose a driver if  USE_WINDOW failed


Todo :
   
******************************************************************************/




#include "stdafx.h"

#ifdef _D3D

#include "gd3device.h"

#include "d3drm.h"  /* For D3DRM error codes only */


/***************************************************************************/
/*                               FUNCTIONS                                 */
/***************************************************************************/

GD3Device::GD3Device()
{
	hwnd = NULL;
	hwndFrame = NULL;
	hwndParent = NULL;
	hwndFullScreenWnd = NULL;

	lpD3D = NULL;
	lpD3DDevice = NULL;
	lpD3DViewport = NULL;

	dx5 = FALSE;
	lpD3D2 = NULL;
	lpD3DDevice2 = NULL;
	lpD3DViewport2 = NULL;

	dx6 = FALSE;
	lpD3D3 = NULL;
	lpD3DDevice3 = NULL;
	lpD3DViewport3 = NULL;

	dxLevel = 0;

	NumDevices = 0;
	PrimaryDevice=-1;
	FullScreenDevice=-1;

	NumDrivers = 0;
	CurrDriver = 0;
	SoftwareDriver = 0;
	FullscreenDriver = -1;
	coopLevelWasSet = FALSE;


	bBackBufferInVideo = FALSE;
	bZBufferInVideo = FALSE;

	lpDD = NULL;
	lpDD4 = NULL;

	bIsPrimary = FALSE;
	lpFrontBuffer = NULL;
	lpBackBuffer = NULL;
	lpZBuffer = NULL;

	lpFrontBuffer4 = NULL;
	lpBackBuffer4 = NULL;
	lpZBuffer4 = NULL;

	NumModes = 0;
	CurrMode = 0;
	bFullscreen = FALSE;
	bPaused = FALSE;
	bAppActive = FALSE;
	bTexturesDisabled = FALSE;
	bOnlySystemMemory = FALSE;
	bOnlyEmulation = FALSE;
	bMinimized = FALSE;
	bRenderingIsOK = FALSE;

	LastError = 0;	
	D3DDeviceDestroyCallback = NULL;
	D3DDeviceCreateCallback = NULL;

	NumDirtyClientRects = 0;
	NumDirtyBackRects = 0;
	NumDirtyZRects = 0;

	lpClipper = NULL;
	lpPalette = NULL;

    bInitialized = FALSE;
	bPrimaryPalettized = FALSE;
	bPaletteActivate = FALSE;
	bIgnoreWM_SIZE = FALSE;
	CallbackRefCount = 0;

	pausecount = 0;
}

GD3Device::~GD3Device()
{
	if (bInitialized)
		Destroy();
	//any way clean up partial stuff ??? 

}


/***************************************************************************/
/*                               FUNCTIONS                                 */
/***************************************************************************/
/*
 * CreateFromHWND
 */
BOOL 
GD3Device::CreateFromHWND(DWORD flags, 
						  const char* driverName,	// name of driver or ""
					      int driver,	// D3DAPP_YOURDECIDE 
						  int mode,
						  HWND theHwnd,HWND theHwndFrame,
                          BOOL(*DeviceCreateCallback)(int, int,
                                                      LPDIRECT3DVIEWPORT*,
                                                      LPVOID),
                          LPVOID lpCreateContext,
                          BOOL(*DeviceDestroyCallback)(LPVOID),
                          LPVOID lpDestroyContext)
{
    int w, h;
	int requestedMode = mode;

	if (bInitialized)
		Destroy();

	BOOL bUseHw = FALSE;
	BOOL bUseRgb = FALSE;
    /* 
     * Clean the global variables and check the flags
     */
    SetDefaults();

    if (flags & D3DAPP_ONLYSYSTEMMEMORY) {
        bOnlySystemMemory = TRUE;
        // hg bOnlyEmulation = TRUE;
    }
    if (flags & D3DAPP_ONLYD3DEMULATION)
        bOnlyEmulation = TRUE;

    if (flags & D3DAPP_ONLY_HW) {
        bUseHw = TRUE;
    }
    if (flags & D3DAPP_ONLY_RGB) {
        bUseRgb = TRUE;
    }
	// D3DAPP_ONLYDDEMULATION
    /* 
     * Create DirectDraw, remember the Windows display mode and enumerate the
     * display modes
     */
    ATTEMPT(CreateDD( (bOnlyEmulation ? D3DAPP_ONLYDDEMULATION : 0L) | (mode == D3DAPP_USEWINDOW ? D3DAPP_ONLYWINDOW : 0L)));
    ATTEMPT(RememberWindowsMode());
    ATTEMPT(EnumDisplayModes());
    /*
     * Create Direct3D and enumerate the D3D drivers
     */
    ATTEMPT(CreateD3D());
    ATTEMPT(EnumDrivers());
    /*
     * Set the device creation and destroy callback functions
     */
    D3DDeviceDestroyCallback = DeviceDestroyCallback;
    D3DDeviceDestroyCallbackContext = lpDestroyContext;
    D3DDeviceCreateCallback = DeviceCreateCallback;
    D3DDeviceCreateCallbackContext = lpCreateContext;

    hwnd = theHwnd;
    if (theHwndFrame != NULL)
		hwndFrame = theHwndFrame; 
	else hwndFrame = theHwnd;

	// save some window information for fullscreen toggeling 
	if (hwndFrame)  {
		lastFrameMenu = ::GetMenu(hwndFrame);
		lastFrameWinStyle = ::GetWindowLong(hwndFrame,GWL_STYLE);
	}
	lastWinExStyle = ::GetWindowLong(hwnd,GWL_EXSTYLE);


    /*
     * Choose a driver and display mode.  Using the current window is 
     * prefered, but a fullscreen mode may be selected.  Set the cooperative
     * level and create the front and back buffers for this mode.
     */
    //driver = D3DAPP_YOUDECIDE;
    //mode = D3DAPP_YOUDECIDE;
	int i;
	if (driverName[0]!=0) {
		for (i=0; i< NumDrivers;i++) {
			if (strcmp(driverName,Driver[i].Name) == 0) {
				driver = i;
			}
		}
	} 

	if (bUseHw) { // user wants Hardware 
		if ((driver <0) || (!Driver[driver].bIsHardware)) {
		for (i=0; i< NumDrivers;i++) {
			if (Driver[i].bIsHardware) {
				if (mode == D3DAPP_USEWINDOW)
					if (!Driver[i].bCanDoWindow) continue;
				driver = i;
				break;
			}
		}
		}
	}
	
	if (bUseRgb) { // user wants RGB ! how to avoid Reference Rast ??????
		if ((driver <0) || (Driver[driver].Desc.dcmColorModel & D3DCOLOR_MONO)) {
		for (i=NumDrivers-1;i>=0;i--) {
			if (!(Driver[i].Desc.dcmColorModel & D3DCOLOR_MONO)) {
				if (mode == D3DAPP_USEWINDOW)
					if (!Driver[i].bCanDoWindow) continue;
				driver = i;
			}
		}
		}
	}
    ATTEMPT(VerifyDriverAndMode(&driver, &mode));

	// HG turn of system memory flag if hardware is choosen 
	if (Driver[driver].bIsHardware && bOnlySystemMemory) {
		bOnlySystemMemory = FALSE;
	}

	if ( (requestedMode == D3DAPP_USEWINDOW)
		&& (mode  != D3DAPP_USEWINDOW)) 
	{

		// done in verify driver and mode 
/*
		// this driver can't do window rendering in this mode 
		mode = D3DAPP_USEWINDOW;
		driver = D3DAPP_YOUDECIDE;
        
		ATTEMPT(VerifyDriverAndMode(&driver, &mode));
*/
		
		TRACE("We are not in window mode, even if requested \n");

	}

    GetClientWin(hwnd);

    if (mode == D3DAPP_USEWINDOW) {
        w = szClient.cx;
        h = szClient.cy;
        ATTEMPT(SetCoopLevel(hwndFrame, FALSE));
        ATTEMPT(CreateBuffers(hwnd, w, h, D3DAPP_BOGUS, FALSE));

    } else {
        szLastClient = szClient;
        w = Mode[mode].w;
        h = Mode[mode].h;
        szClient.cx = w; szClient.cy = h;
        ATTEMPT(SetCoopLevel(hwndFrame, TRUE));

		TRACE("GD3Device::CreateFromHWND %i %i\n", szClient.cx, szClient.cy);

        ATTEMPT(SetDisplayMode(w, h, Mode[mode].bpp));
        CurrMode = mode;
        ATTEMPT(CreateBuffers(hwnd, w, h, Mode[mode].bpp, TRUE));
    }
    /*
     * If the front buffer is palettized, initialize its palette
     */
    ATTEMPT(CheckForPalettized());
    /*
     * Create the Z-buffer
     */
    ATTEMPT(CreateZBuffer(w, h, driver));
    /*
     * Create the D3D device, load the textures, call the device create
     * callback and set a default render state
     */
    ATTEMPT(CreateDevice(driver));
    ATTEMPT(LoadAllTextures());
    ATTEMPT(FilterDisplayModes(driver));  /* bThisDriverCanDo flags */
    ATTEMPT(CallDeviceCreateCallback(w, h));

#if 0
    ATTEMPT(SetRenderState());
#endif

    /* 
     * Initialize dirty rectangle information
     */
    ValidateDirtyRects();
    /*
     * Ready to render
     */
    bInitialized = TRUE;
    bRenderingIsOK = TRUE;
    return TRUE;

exit_with_error:
    CallDeviceDestroyCallback();

	ReleaseDevice();

    if (bFullscreen) {
        RestoreDispMode();
        SetCoopLevel(hwnd, FALSE);
    }
    RELEASE(lpD3D);
    RELEASE(lpD3D2);
    RELEASE(lpD3D3);
    RELEASE(lpDD4);
    RELEASE(lpDD);
    return FALSE;
}   

/*================================================================
 *								Fullscreen
 *================================================================*/
BOOL 
GD3Device::FullscreenMode(int mode)
{
    int w, h, bpp;
    BOOL b; /* was already in a fullscreen mode? */
    bRenderingIsOK = FALSE;

	
	//
	// save the window size and style for restoring
	//
	szLastClient = szClient;
	if (hwndFrame)  
		{
			GetWindowRect(hwndFrame, &szLastClientFrame); // store frame window size

			lastFrameMenu = ::GetMenu(hwndFrame);
			lastFrameWinStyle = GetWindowLong(hwndFrame,GWL_STYLE);
		}
	lastWinExStyle = GetWindowLong(hwnd,GWL_EXSTYLE);



    /* 
     * Make sure this is a valid request, otherwise doctor mode so it will
     * work with this driver.
     */
    ATTEMPT(VerifyDriverAndMode(&CurrDriver, &mode));
    /* 
     * Release everything
     */
    ATTEMPT(CallDeviceDestroyCallback());
    if (bFullscreen) {
        ATTEMPT(ClearBuffers());
    }
    ReleaseAllTextures();
	
	ReleaseDevice();

    /*
     * Record information about the current status
     */
    b = bFullscreen;
    w = Mode[mode].w;
    h = Mode[mode].h;
    bpp = Mode[mode].bpp;

    szClient.cx = w; szClient.cy = h;
	TRACE("GD3Device::FullscreenMode 0 0 %i %i\n", szClient.cx, szClient.cy);

    /*
     * Set the cooperative level and create front and back buffers
     */
    ATTEMPT(SetCoopLevel(hwndFullScreenWnd, TRUE));

    ATTEMPT(SetDisplayMode(w, h, bpp));

	CurrMode = mode;
    
	ATTEMPT(CreateBuffers(hwnd, w, h, bpp, TRUE));
    /*
     * If the front buffer is palettized, initialize its palette
     */
    ATTEMPT(CheckForPalettized());
    /*
     * Create the Z-buffer
     */
    ATTEMPT(CreateZBuffer(w, h, CurrDriver));
    /*
     * Create the D3D device, load the textures, call the device create
     * callback and set a default render state
     */
    ATTEMPT(CreateDevice(CurrDriver));
    ATTEMPT(LoadAllTextures());
    ATTEMPT(CallDeviceCreateCallback(w, h));

    /* 
     * Set current mode and clear dirty rectangle information
     */
    CurrMode = mode;
    ValidateDirtyRects();
    bRenderingIsOK = TRUE;

    return TRUE;

exit_with_error:
	TRACE("GD3D3Device::FullScreenMode(%d) failed  %d ! \n", mode, LastError);

    CallDeviceDestroyCallback();

	ReleaseDevice();

	if (!b) {
        RestoreDispMode();
        SetCoopLevel(hwndFrame, FALSE);
	    bRenderingIsOK = TRUE; //HG 
    }
    return FALSE;
}


/*
  set the default fullscreen mode
*/
BOOL GD3Device::SetDefaultFullscreenMode(int mode)
{
	if (mode <0  || mode>=NumModes) return FALSE;
	CurrMode = mode;
	return TRUE;
}

BOOL GD3Device::SetDefaultFullscreenMode	( 
			int     w,  
			int     h,  
			int     bpp)
{
	for(int mode=0; mode<NumModes; mode++)
        if (w == Mode[mode].w
			&&   h == Mode[mode].h
			&&   bpp == Mode[mode].bpp
			) 
			if (SetDefaultFullscreenMode(mode)) return TRUE;

	return FALSE;
}

BOOL GD3Device::SetDefaultFullscreenMode(const char *mode)
{
	int     w,h,bpp;  

	if (sscanf(mode,"%dx%dx%d",&w,&h,&bpp) != 3)
		return FALSE;

	return SetDefaultFullscreenMode(w,h,bpp);

};



BOOL GD3Device::GetDefaultFullscreenMode(CString &mode)
{
	int i= CurrMode;
	if (i<0) return FALSE;
	mode.Format("%dx%dx%d",Mode[i].w,Mode[i].h,Mode[i].bpp);
	return TRUE;
}	



/*
 * Window
 */
BOOL
GD3Device::WindowMode(int w, int h)
{
    BOOL b; /* changing from a fullscreen mode? */


    if (!bIsPrimary) {
        //SetErrorString("It is not possible to create a D3D window with a hardware DirectDraw device.  Check the bIsPrimary flag before calling Window.");
        //return FALSE;

		return ChangeDevice(PrimaryDevice,D3DAPP_ONLYWINDOW);
    }

    b = bFullscreen;
    /*
     * If asked to set the window size, return it to the last value or use
     * a default value.
     */
    if (w == D3DAPP_YOUDECIDE) {
        w = b ? szLastClient.cx : D3DAPP_DEFAULTWINDOWDIM;
    }
    if (h == D3DAPP_YOUDECIDE) {
        h = b ? szLastClient.cy : D3DAPP_DEFAULTWINDOWDIM;
    }
    /*
     * Release everything
     */
    bRenderingIsOK = FALSE;
    ATTEMPT(CallDeviceDestroyCallback());
    if (b) {
        ATTEMPT(ClearBuffers());
    }
    ReleaseAllTextures();

	ReleaseDevice();

    /* 
     * Restore the display mode if we were in a fullscreen mode
     */
    if (b) {
        RestoreDispMode();
    }
    /* 
     * Set the cooperative level and create front and back buffers
     */
    SetCoopLevel(hwndFrame, FALSE);

    // HG    
	if (b)  // SetClientRect(hwndFrame, &szLastClientFrame); // store frame window size
	{
	    bIgnoreWM_SIZE = TRUE;
		// set size of frame window 
		if (hwndFrame &&  hwndFrame != hwnd) {
			SetWindowPos(hwndFrame, NULL, 0, 0, szLastClientFrame.right-szLastClientFrame.left,
                     szLastClientFrame.bottom-szLastClientFrame.top,
                     SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
			SetWindowPos(hwndFrame, HWND_NOTOPMOST, 0, 0, 0, 0,
                     SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
		}



	    SetClientSize(hwnd, w, h, b); // set size on subwindow 

	    bIgnoreWM_SIZE = FALSE;
	}
    
	ATTEMPT(CreateBuffers(hwnd, w, h, D3DAPP_BOGUS, FALSE));
    /*
     * If the front buffer is palettized, initialize its palette
     */
    ATTEMPT(CheckForPalettized());
    /*
     * Create the Z-buffer
     */
    ATTEMPT(CreateZBuffer(szBuffers.cx, szBuffers.cy,                       CurrDriver));
    /*
     * Create the D3D device, load the textures, call the device create
     * callback and set a default render state
     */
    ATTEMPT(CreateDevice(CurrDriver));
    ATTEMPT(LoadAllTextures());          
    ATTEMPT(CallDeviceCreateCallback(szBuffers.cx, szBuffers.cy));

#if 0
    ATTEMPT(SetRenderState());
#endif

    /* 
     * Clear dirty rectangle information
     */
    ValidateDirtyRects();
    bRenderingIsOK = TRUE;
    
	return TRUE;


exit_with_error:
    CallDeviceDestroyCallback();

	ReleaseDevice();

    return FALSE;  
}

// is current driver hardware driver ?
BOOL GD3Device::IsHardwareDriver() 
{
	if (CurrDriver <0) return FALSE;
	return Driver[CurrDriver].bIsHardware;
}


// has device hardware driver ?
BOOL GD3Device::HasHardwareDriver(DWORD flags)
{
    for (int i = 0; i < NumDrivers; i++) {
		if (Driver[i].bIsHardware) {
			if (flags & D3DAPP_ONLYWINDOW) {
				if (Driver[i].bCanDoWindow) return TRUE;

				else continue;
			}
			return TRUE;
		}
    }
	return FALSE;
}

// change to first hardware driver 
BOOL GD3Device::ChangeToHardwareDriver(DWORD flags)
{
    for (int i = 0; i < NumDrivers; i++) {
		if (Driver[i].bIsHardware) {
			
			if (flags & D3DAPP_ONLYWINDOW) {
				if (!Driver[i].bCanDoWindow) continue;
			}
	
			return ChangeDriver(i,flags);
		}
    }
	return FALSE;
}


/*
 * ChangeDriver 
 */
BOOL
GD3Device::ChangeDriver(int driver, DWORD flags)
{
    int mode;
	
	if (flags & D3DAPP_ONLYWINDOW ) {
        mode = D3DAPP_USEWINDOW;
	}
	else 
    /*
     * Verify the compatibility of this mode with the specified driver.
     * The mode may change.
     */
    if (bFullscreen)
        mode = CurrMode;
    else
        mode = D3DAPP_USEWINDOW;

    ATTEMPT(VerifyDriverAndMode(&driver, &mode));

    if (driver == D3DAPP_BOGUS || mode == D3DAPP_BOGUS)
        goto exit_with_error;

	// HG turn of system memory flag if hardware is choosen 
	if (Driver[driver].bIsHardware && bOnlySystemMemory) {
		bOnlySystemMemory = FALSE;
	}

    /*
     * Update the current driver and set bThisDriverCanDo flags
     */
    CurrDriver = driver;

    ATTEMPT(FilterDisplayModes(driver));
    /*
     * Either call Window or Fullscreen depending on mode
     */
    if (mode == D3DAPP_USEWINDOW) {
        if (bFullscreen) {
            /*
             * We need to switch to a window.   will either use the
             * size of the last window it saw or use a default size.
             */
            ATTEMPT(WindowMode(D3DAPP_YOUDECIDE, D3DAPP_YOUDECIDE));
        } else {
            /*
             * We need to recreate the current window.  Don't let 
             * decide on the size.
             */
            ATTEMPT(WindowMode(szClient.cx, szClient.cy));
        }
        /*
         * Change the currently selected mode if it's not compatible with
         * this driver.  Just to make sure that CurrMode is always a mode the
         * current driver can do.
         */
        if (!(Driver[driver].Desc.dwDeviceRenderBitDepth &
              BPPToDDBD(Mode[CurrMode].bpp))){
            ATTEMPT(PickDisplayMode(&CurrMode,
                        Driver[driver].Desc.dwDeviceRenderBitDepth));
        }
        return TRUE;
    } else {
        /*
         * We need to switch to fullscreen or switch fullscreen modes or stay
         * in the same fullscreen mode.  In any of these cases, we call the
         * same function.
         */
        ATTEMPT(FullscreenMode(mode));
        return TRUE;
    }

exit_with_error:
    /*
     * The failed mode setting call would have released everything
     */
    return FALSE;
}

#if 0
/*
 * WindowProc
 */
BOOL
GD3Device::WindowProc(BOOL* bStopProcessing, LRESULT* lresult, HWND hwnd,
                 UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    *bStopProcessing = FALSE;
    if (!bInitialized)
        return TRUE;
    /*
     * Look for messages which effect rendering.  In some cases, we will not
     * want the app to continue processing the message, so set the flag and
     * provide a return value in lresult.
     */
    switch(message) {
        case WM_SIZE:
            if (!bIgnoreWM_SIZE) {
                /*
                 * Too long to fit here, see ddcalls.c. Updates the buffers
                 * and re-creates the device.
                 */
                ATTEMPT(HandleWM_SIZE(lresult, hwnd, message,
                                             wParam, lParam));
                *bStopProcessing = TRUE;
            }
            break;
        case WM_MOVE:
            /*
             * Update client window position information
             */
            pClientOnPrimary.x = pClientOnPrimary.y = 0;
            ClientToScreen(hwnd, &pClientOnPrimary);
            break;
        case WM_ACTIVATE:
            /*
             * Set the front buffer's palette
             */
            if (bPaletteActivate && bPrimaryPalettized &&
                lpFrontBuffer) {
                lpFrontBuffer->SetPalette(lpPalette);
            }
            break;
        case WM_ACTIVATEAPP:
            bAppActive = (BOOL)wParam;
            break;
        case WM_SETCURSOR:
            /*
             * Prevent the cursor from being shown in fullscreen
             */
            if (bFullscreen && !bPaused) {
                SetCursor(NULL);
                *lresult = 1;
                *bStopProcessing = TRUE;
                return TRUE;
            }
            break;
        case WM_MOVING:
            /*
             * Prevent the window from moving in fullscreen
             */
            if (bFullscreen) {
                GetWindowRect(hwnd, (LPRECT)lParam);
                *lresult = 1;
                *bStopProcessing = TRUE;
                return TRUE;
            }
            break;
        case WM_GETMINMAXINFO:
            /*
             * Ensure the window won't resize in fullscreen
             */
            if (bFullscreen) {
                ((LPMINMAXINFO)lParam)->ptMaxTrackSize.x= ThisMode.w;
                ((LPMINMAXINFO)lParam)->ptMaxTrackSize.y= ThisMode.h;
                ((LPMINMAXINFO)lParam)->ptMinTrackSize.x= ThisMode.w;
                ((LPMINMAXINFO)lParam)->ptMinTrackSize.y= ThisMode.h;
                *lresult = 0;
                *bStopProcessing = TRUE;
                return TRUE;
            } else {
                ((LPMINMAXINFO)lParam)->ptMaxTrackSize.x =
                                                    WindowsDisplay.w;
                ((LPMINMAXINFO)lParam)->ptMaxTrackSize.y =
                                                    WindowsDisplay.h;
                *lresult = 0;
                *bStopProcessing = TRUE;
                return TRUE;
            }
            break;
        case WM_PAINT:
            /*
             * Clear the rectangle and blt the backbuffer image
             */
            BeginPaint(hwnd, &ps);
            if (bRenderingIsOK && !bFullscreen) {
                ShowBackBuffer(D3DAPP_SHOWALL);
            }
            ValidateDirtyRects();
            EndPaint(hwnd, &ps);
            *lresult = 1;
            *bStopProcessing = TRUE;
            return TRUE;
        case WM_NCPAINT:
            /*
             * When in fullscreen mode, don't draw the window frame.
             */
            if (bFullscreen && !bPaused) {
                *lresult = 0;
                *bStopProcessing = TRUE;
                return TRUE;
            }
            break;
    }
    return TRUE;

exit_with_error:
    return FALSE;
}

#endif 



/*
 * ShowBackBuffer
 */
BOOL
GD3Device::ShowBackBuffer(DWORD flags)
{
    if (!bRenderingIsOK) {
        SetErrorString("Cannot call ShowBackBuffer while bRenderingIsOK is FALSE.\n");
        return FALSE;
    }
    if (bPaused)
        return TRUE;

    if (bFullscreen) {  
        int numtemp;
        D3DRECT temp[D3DAPP_MAXCLEARRECTS];
        /*
         * Flip the back and front buffers
         */
        LastError = lpFrontBuffer->Flip(lpBackBuffer,1);
/*
		if (LastError == DDERR_WASSTILLDRAWING) {
				LastError= DD_OK;
				//mustFlip=TRUE;
		}		
*/
        if (LastError == DDERR_SURFACELOST) {
            lpFrontBuffer->Restore();
            lpBackBuffer->Restore();
            ClearBuffers();
        } else if (FAILED(LastError)) {
            SetErrorString("Flipping complex display surface failed.\n%s", ErrorToString(LastError));
            return FALSE;
        }
        if (bBackBufferInVideo) {
            /*
             * This is a real flip, so the client and back buffer dirty
             * rectangles also flip
             */
            CopyRectList(&numtemp, temp, NumDirtyClientRects,
                                DirtyClient);
            CopyRectList(&NumDirtyClientRects, DirtyClient,
                                NumDirtyBackRects, DirtyBack);
            CopyRectList(&NumDirtyBackRects, DirtyBack, numtemp, temp);
        } else {
            /*
             * The flip is being emulated as a blt from a system memory back
             * buffer, so the back buffer's dirty rectangles are now also the
             * client's.
             */
            CopyRectList(&NumDirtyClientRects, DirtyClient,
                                NumDirtyBackRects, DirtyBack);
        }
    } else {
        int NumFrontRects, NumBufferRects, i;
        RECT front[D3DAPP_MAXCLEARRECTS];
        RECT buffer[D3DAPP_MAXCLEARRECTS];
        /*
         * Set the rectangle to blt from the back to front bufer
         */
        if (flags & D3DAPP_SHOWALL) {
            /*
             * Set to entire client window
             */
            NumBufferRects = 1;
			front[0] = rcScreenRect;
			buffer[0] = rcViewportRect;
			int delta=0;
			if (rcScreenRect.top<0) { // out of scroll cases ?? 
				delta = -rcScreenRect.top;
				front[0].top+=delta;
				buffer[0].top+=delta;
			}
			if (rcScreenRect.left<0) {
				delta = -rcScreenRect.left;
				front[0].left+=delta;
				buffer[0].left+=delta;
			}

/*
            SetRect(&buffer[0], 0, 0, szClient.cx,szClient.cy);
            SetRect(&front[0],
                    pClientOnPrimary.x, pClientOnPrimary.y,
                    szClient.cx + pClientOnPrimary.x,szClient.cy + pClientOnPrimary.y);
*/
        } else {
            /*
             * Merge the back and front buffer dirty rectangle lists to get
             * a list of rectangles to blt.  This will simultaneously clear
             * the smallest front buffer region while blt'ing all the back
             * buffer which changed.
             */
            MergeRectLists(&NumBufferRects, (LPD3DRECT)buffer,
                                  NumDirtyClientRects, DirtyClient,
                                  NumDirtyBackRects, DirtyBack);
            CopyRectList(&NumFrontRects, (LPD3DRECT)front,
                                NumBufferRects, (LPD3DRECT)buffer);
            for (i = 0; i < NumFrontRects; i++) {
                front[i].top += pClientOnPrimary.y;
                front[i].left += pClientOnPrimary.x;
                front[i].bottom += pClientOnPrimary.y;
                front[i].right += pClientOnPrimary.x;
            }
        }


		if (NumBufferRects == 1) {
            // don´t wait 

			if (lpFrontBuffer4)
				LastError = lpFrontBuffer4->Blt(&front[0], lpBackBuffer4, &buffer[0], DDBLT_WAIT , NULL);
			else 
				LastError = lpFrontBuffer->Blt(&front[0], lpBackBuffer, &buffer[0], DDBLT_WAIT  , NULL);

			// DDBLT_ASYNC
            if (LastError == DDERR_SURFACELOST) {
                lpFrontBuffer->Restore();
                lpBackBuffer->Restore();
                ClearBuffers();
            } else if (LastError != DD_OK) {
				if (LastError != DDERR_WASSTILLDRAWING) {
					SetErrorString("Blt of back buffer to front buffer failed.\n%s", ErrorToString(LastError));
					return FALSE;
				} 
            }
		} else 
        /*
         * Blt the list of rectangles from the back to front buffer
         */
        for (i = 0; i < NumBufferRects; i++) {
            LastError =
                    lpFrontBuffer->Blt(&front[i], lpBackBuffer,
                                             &buffer[i], DDBLT_WAIT, NULL);
            if (LastError == DDERR_SURFACELOST) {
                lpFrontBuffer->Restore();
                lpBackBuffer->Restore();
                ClearBuffers();
            } else if (LastError != DD_OK) {
				SetErrorString("Blt of back buffer to front buffer failed.\n%s", ErrorToString(LastError));
				return FALSE;
            }
        }
        /*
         * The back buffer's dirty rectangles are now also the client's
         */
        CopyRectList(&NumDirtyClientRects, DirtyClient,
                            NumDirtyBackRects, DirtyBack);
    }
    return TRUE;
}

/*
 * RenderExtents
 */
BOOL
GD3Device::RenderExtents(DWORD dwCount, LPD3DRECT extent, DWORD flags)
{
    if (dwCount > D3DAPP_MAXCLEARRECTS) {
        SetErrorString("The number of clear rectangles exceeded D3DAPP_MAXCLEARRECTS.");
        return FALSE;
    }
    if (flags & D3DAPP_CLEARALL) {
        /*
         * Set the back buffer dirty rectangle to the entire client area
         */
        D3DRECT dummy;
        dummy.x1 = dummy.y1 = 0;
        dummy.x2 = szClient.cx;
        dummy.y2 = szClient.cy;
        CopyRectList(&NumDirtyBackRects, DirtyBack, 1, &dummy);
        CopyRectList(&NumDirtyClientRects, DirtyClient, 1, &dummy);
        CopyRectList(&NumDirtyZRects, DirtyZ, 1, &dummy);
    } else {
        /*
         * Set the back and Z buffer dirty rectangle list as specified
         */
        CopyRectList(&NumDirtyBackRects, DirtyBack, dwCount, extent);
        CopyRectList(&NumDirtyZRects, DirtyZ, dwCount, extent);
    }
    return TRUE;
}

/*
 * ClearBackBuffer
 */
BOOL
GD3Device::ClearBackBuffer(DWORD flags)
{
	if (!lpD3DViewport) 
		return FALSE;

    if (!bRenderingIsOK) {
        SetErrorString("Cannot call ClearBackBuffer while bRenderingIsOK is FALSE.\n");
        return FALSE;
    }
    if (flags & D3DAPP_CLEARALL) {
        /*
         * Clear the entire back buffer
         */
        int clearflags;
        D3DRECT dummy;
        /*
         * Decided wether to clear just back buffer or also z-buffer
         */

        clearflags = 0;
        if (flags & D3DAPP_CLEARBACK)
            clearflags |= D3DCLEAR_TARGET;

        if (flags & D3DAPP_CLEARZ)
            clearflags |= D3DCLEAR_ZBUFFER;
/*
        dummy.x1 = dummy.y1 = 0;
        dummy.x2 = szClient.cx;
        dummy.y2 = szClient.cy;
 */
		
		dummy.x1 = rcViewportRect.left;
		dummy.y1 = rcViewportRect.top;
        dummy.x2 = rcViewportRect.right;
        dummy.y2 = rcViewportRect.bottom;


		LastError = lpD3DViewport->Clear(1, &dummy,clearflags);
        if (LastError != D3D_OK) {
            SetErrorString("Viewport clear failed.\n%s",
                                  ErrorToString(LastError));
            return FALSE;
        }
    } else {
        /*
         * Clear the dirty rectangles on the back buffer
         */
        LastError =	 lpD3DViewport->Clear( NumDirtyBackRects,
                                            DirtyBack, D3DCLEAR_TARGET);
        if (LastError != D3D_OK) {
            SetErrorString("Viewport clear of back buffer failed.\n%s",
                                  ErrorToString(LastError));
            return FALSE;
        }
        
		if (flags & D3DAPP_CLEARZ) {

        /*
         * Clear the dirty rectangles on the Z buffer
         */
        LastError =  lpD3DViewport->Clear( NumDirtyZRects,
                                                      DirtyZ, D3DCLEAR_ZBUFFER);
        if (LastError != D3D_OK) {
            SetErrorString("Viewport clear of Z buffer failed.\n%s",
                                  ErrorToString(LastError));
            return FALSE;
        }
		}
    }
    return TRUE;
}

/*
 * CheckForLostSurfaces
 */
#define CHECKSURF(x) if (x) {                                         \
                        if ((LastError = x->IsLost()) == DDERR_SURFACELOST) {    \
							LastError = x->Restore();              \
                            if (LastError != DD_OK) goto exit_with_error;   \
                            b = TRUE;                                       \
                        }                                                   \
                     }
BOOL
GD3Device::CheckForLostSurfaces(BOOL &someLost)
{
    BOOL b = FALSE;
    /*
     * Check all the surfaces  owns and restore them if lost.
     */
    CHECKSURF(lpFrontBuffer);
	if (LastError != DD_OK)  {
		TRACE("Check Surf failed.\n%s", ErrorToString(LastError));
	}

    CHECKSURF(lpBackBuffer);
    CHECKSURF(lpZBuffer);
    if (b) {
        /*
         * If any of the surfaces were lost and restored, clear all the buffers.
         * If this fails, that's fine, just move on.
         */
        ClearBuffers();
    }
#if 0
    int i;
    for (i = 0; i < NumTextures; i++) {
        b = FALSE;
        CHECKSURF(lpTextureSurf[i]);
        if (b) {
            ATTEMPT(ReloadTextureSurf(i));
        }
    }
#endif 
	someLost = b;
    return TRUE;

exit_with_error:
	someLost = b;
    SetErrorString("Restoring of a lost surface failed.\n%s", ErrorToString(LastError));
    return FALSE;
}

/*
 * Pause
 */
BOOL
GD3Device::Pause(BOOL flag)
{

    /*
     * Keep a count of the number of times Pause has been called to 
     * prevent pausing more than once in a row.
     */
    if (pausecount != 0) {
        if (flag) {
            ++pausecount;
            return TRUE;
        } else {
            --pausecount;
            if (pausecount != 0)
                return TRUE;
        } 
    }

    bPaused = flag;
    if (!flag) {
        /*
         * Returning from a pause
         */
        if (bFullscreen && bPrimaryPalettized && lpPalette) {
            /*
             * Set front buffer's palette back to what it was before pause
             */
            LastError = lpPalette->SetEntries(0, 0, 256, &ppe[0]);
            if (LastError != DD_OK) {
                SetErrorString("Setting palette entries during unpause failed.\n%s", ErrorToString(LastError));
                goto exit_with_error;
            }
        }
        /*
         * Dirty rectangle info is no longer valid
         */
        ValidateDirtyRects();
    }
    if (flag && bFullscreen) {
        /*
         * Pausing in a fullscreen mode
         */
        if (bPrimaryPalettized && lpPalette) {
            /*
             * Save the front buffer's current palette and restore the
             * original Windows palette.
             */
            int i;
            LastError = lpPalette->GetEntries( 0, 0, 256,&ppe[0]);
            if (LastError != DD_OK) {
                SetErrorString("Getting palette entries before a pause failed.\n%s", ErrorToString(LastError));
                goto exit_with_error;
            }
            for (i = 10; i < 246; i++)
                Originalppe[i] = ppe[i];
            LastError = lpPalette->SetEntries(0, 0, 256,
                                                      &Originalppe[0]);
            if (LastError != DD_OK) {
                SetErrorString("Returning palette entries to defaults failed.\n%s", ErrorToString(LastError));
                goto exit_with_error;
            }
        }
        /*
         * Flip to GDI surface (either front or back buffer)
         */
        if (bIsPrimary && lpDD) {
            LastError = lpDD->FlipToGDISurface();
            if (LastError != DD_OK) {
                SetErrorString("Flipping to GDI surface failed.\n%s", ErrorToString(LastError));
                goto exit_with_error;
            }
        }
        /*
         * Draw the menu and frame
         */
        DrawMenuBar(hwnd);
        RedrawWindow(hwnd, NULL, NULL, RDW_FRAME);
    }
    return TRUE;
exit_with_error:
    return FALSE;
}


/*
 * LastError
 */
HRESULT
GD3Device::GetLastError()
{
    return LastError;  
}

/*
 * LastSetErrorString
 */
char*
GD3Device::GetLastErrorString()
{
    return LastErrorString;
}

/*
 * Release all D3D Device and related objects
 */


void 
GD3Device::ReleaseDevice()
{
	RELEASE(lpD3DViewport); // new hg 
    RELEASE(lpD3DViewport2);
    RELEASE(lpD3DViewport3);

	
	RELEASE(lpD3DDevice);
    RELEASE(lpD3DDevice2);
    RELEASE(lpD3DDevice3);

    RELEASE(lpZBuffer);
    RELEASE(lpPalette);
    RELEASE(lpClipper);

    RELEASE(lpBackBuffer);
    RELEASE(lpFrontBuffer);
	RELEASE(lpFrontBuffer4);
	RELEASE(lpBackBuffer4);


}
/*
 * Destroy
 */

BOOL
GD3Device::Destroy()
{
    /*
     * Destroys all objects including Direct Draw.
     */
    bRenderingIsOK = FALSE;
    
	ATTEMPT(CallDeviceDestroyCallback());

    ReleaseAllTextures();
	
    ReleaseDevice();

    if (bFullscreen) {
        RestoreDispMode();
        SetCoopLevel(hwnd, FALSE);
    }

    RELEASE(lpD3D);
    RELEASE(lpD3D2);
    RELEASE(lpD3D3);
    RELEASE(lpDD4);
    RELEASE(lpDD);

    hwnd = NULL;
    hwndFrame = NULL;
	
	bInitialized = FALSE;


    return TRUE;

exit_with_error:
    return FALSE;
}


/*
 * ErrorToString
 */
char*
GD3Device::ErrorToString(HRESULT error)
{
    switch(error) {
#if 1
        case DD_OK:
            return "No error.\0";
        case DDERR_ALREADYINITIALIZED:
            return "This object is already initialized.\0";
        case DDERR_BLTFASTCANTCLIP:
            return "Return if a clipper object is attached to the source surface passed into a BltFast call.\0";
        case DDERR_CANNOTATTACHSURFACE:
            return "This surface can not be attached to the requested surface.\0";
        case DDERR_CANNOTDETACHSURFACE:
            return "This surface can not be detached from the requested surface.\0";
        case DDERR_CANTCREATEDC:
            return "Windows can not create any more DCs.\0";
        case DDERR_CANTDUPLICATE:
            return "Can't duplicate primary & 3D surfaces, or surfaces that are implicitly created.\0";
        case DDERR_CLIPPERISUSINGHWND:
            return "An attempt was made to set a cliplist for a clipper object that is already monitoring an hwnd.\0";
        case DDERR_COLORKEYNOTSET:
            return "No src color key specified for this operation.\0";
        case DDERR_CURRENTLYNOTAVAIL:
            return "Support is currently not available.\0";
        case DDERR_DIRECTDRAWALREADYCREATED:
            return "A DirectDraw object representing this driver has already been created for this process.\0";
        case DDERR_EXCEPTION:
            return "An exception was encountered while performing the requested operation.\0";
        case DDERR_EXCLUSIVEMODEALREADYSET:
            return "An attempt was made to set the cooperative level when it was already set to exclusive.\0";
        case DDERR_GENERIC:
            return "Generic failure.\0";
        case DDERR_HEIGHTALIGN:
            return "Height of rectangle provided is not a multiple of reqd alignment.\0";
        case DDERR_HWNDALREADYSET:
            return "The CooperativeLevel HWND has already been set. It can not be reset while the process has surfaces or palettes created.\0";
        case DDERR_HWNDSUBCLASSED:
            return "HWND used by DirectDraw CooperativeLevel has been subclassed, this prevents DirectDraw from restoring state.\0";
        case DDERR_IMPLICITLYCREATED:
            return "This surface can not be restored because it is an implicitly created surface.\0";
        case DDERR_INCOMPATIBLEPRIMARY:
            return "Unable to match primary surface creation request with existing primary surface.\0";
        case DDERR_INVALIDCAPS:
            return "One or more of the caps bits passed to the callback are incorrect.\0";
        case DDERR_INVALIDCLIPLIST:
            return "DirectDraw does not support the provided cliplist.\0";
        case DDERR_INVALIDDIRECTDRAWGUID:
            return "The GUID passed to DirectDrawCreate is not a valid DirectDraw driver identifier.\0";
        case DDERR_INVALIDMODE:
            return "DirectDraw does not support the requested mode.\0";
        case DDERR_INVALIDOBJECT:
            return "DirectDraw received a pointer that was an invalid DIRECTDRAW object.\0";
        case DDERR_INVALIDPARAMS:
            return "One or more of the parameters passed to the function are incorrect.\0";
        case DDERR_INVALIDPIXELFORMAT:
            return "The pixel format was invalid as specified.\0";
        case DDERR_INVALIDPOSITION:
            return "Returned when the position of the overlay on the destination is no longer legal for that destination.\0";
        case DDERR_INVALIDRECT:
            return "Rectangle provided was invalid.\0";
        case DDERR_LOCKEDSURFACES:
            return "Operation could not be carried out because one or more surfaces are locked.\0";
        case DDERR_NO3D:
            return "There is no 3D present.\0";
        case DDERR_NOALPHAHW:
            return "Operation could not be carried out because there is no alpha accleration hardware present or available.\0";
        case DDERR_NOBLTHW:
            return "No blitter hardware present.\0";
        case DDERR_NOCLIPLIST:
            return "No cliplist available.\0";
        case DDERR_NOCLIPPERATTACHED:
            return "No clipper object attached to surface object.\0";
        case DDERR_NOCOLORCONVHW:
            return "Operation could not be carried out because there is no color conversion hardware present or available.\0";
        case DDERR_NOCOLORKEY:
            return "Surface doesn't currently have a color key\0";
        case DDERR_NOCOLORKEYHW:
            return "Operation could not be carried out because there is no hardware support of the destination color key.\0";
        case DDERR_NOCOOPERATIVELEVELSET:
            return "Create function called without DirectDraw object method SetCooperativeLevel being called.\0";
        case DDERR_NODC:
            return "No DC was ever created for this surface.\0";
        case DDERR_NODDROPSHW:
            return "No DirectDraw ROP hardware.\0";
        case DDERR_NODIRECTDRAWHW:
            return "A hardware-only DirectDraw object creation was attempted but the driver did not support any hardware.\0";
        case DDERR_NOEMULATION:
            return "Software emulation not available.\0";
        case DDERR_NOEXCLUSIVEMODE:
            return "Operation requires the application to have exclusive mode but the application does not have exclusive mode.\0";
        case DDERR_NOFLIPHW:
            return "Flipping visible surfaces is not supported.\0";
        case DDERR_NOGDI:
            return "There is no GDI present.\0";
        case DDERR_NOHWND:
            return "Clipper notification requires an HWND or no HWND has previously been set as the CooperativeLevel HWND.\0";
        case DDERR_NOMIRRORHW:
            return "Operation could not be carried out because there is no hardware present or available.\0";
        case DDERR_NOOVERLAYDEST:
            return "Returned when GetOverlayPosition is called on an overlay that UpdateOverlay has never been called on to establish a destination.\0";
        case DDERR_NOOVERLAYHW:
            return "Operation could not be carried out because there is no overlay hardware present or available.\0";
        case DDERR_NOPALETTEATTACHED:
            return "No palette object attached to this surface.\0";
        case DDERR_NOPALETTEHW:
            return "No hardware support for 16 or 256 color palettes.\0";
        case DDERR_NORASTEROPHW:
            return "Operation could not be carried out because there is no appropriate raster op hardware present or available.\0";
        case DDERR_NOROTATIONHW:
            return "Operation could not be carried out because there is no rotation hardware present or available.\0";
        case DDERR_NOSTRETCHHW:
            return "Operation could not be carried out because there is no hardware support for stretching.\0";
        case DDERR_NOT4BITCOLOR:
            return "DirectDrawSurface is not in 4 bit color palette and the requested operation requires 4 bit color palette.\0";
        case DDERR_NOT4BITCOLORINDEX:
            return "DirectDrawSurface is not in 4 bit color index palette and the requested operation requires 4 bit color index palette.\0";
        case DDERR_NOT8BITCOLOR:
            return "DirectDrawSurface is not in 8 bit color mode and the requested operation requires 8 bit color.\0";
        case DDERR_NOTAOVERLAYSURFACE:
            return "Returned when an overlay member is called for a non-overlay surface.\0";
        case DDERR_NOTEXTUREHW:
            return "Operation could not be carried out because there is no texture mapping hardware present or available.\0";
        case DDERR_NOTFLIPPABLE:
            return "An attempt has been made to flip a surface that is not flippable.\0";
        case DDERR_NOTFOUND:
            return "Requested item was not found.\0";
        case DDERR_NOTLOCKED:
            return "Surface was not locked.  An attempt to unlock a surface that was not locked at all, or by this process, has been attempted.\0";
        case DDERR_NOTPALETTIZED:
            return "The surface being used is not a palette-based surface.\0";
        case DDERR_NOVSYNCHW:
            return "Operation could not be carried out because there is no hardware support for vertical blank synchronized operations.\0";
        case DDERR_NOZBUFFERHW:
            return "Operation could not be carried out because there is no hardware support for zbuffer blitting.\0";
        case DDERR_NOZOVERLAYHW:
            return "Overlay surfaces could not be z layered based on their BltOrder because the hardware does not support z layering of overlays.\0";
        case DDERR_OUTOFCAPS:
            return "The hardware needed for the requested operation has already been allocated.\0";
        
		case DDERR_OUTOFMEMORY:
            return "DirectDraw does not have enough memory to perform the operation.\0";
        
		case DDERR_OUTOFVIDEOMEMORY:
            return "Out of Video Memory\n You may close some other 3D Windows or reduce the window size\nChoosing a software mode now <Enter>...\0";

        case DDERR_OVERLAYCANTCLIP:
            return "The hardware does not support clipped overlays.\0";

        case DDERR_OVERLAYCOLORKEYONLYONEACTIVE:
            return "Can only have one color key active at one time for overlays.\0";

        case DDERR_OVERLAYNOTVISIBLE:
            return "Returned when GetOverlayPosition is called on a hidden overlay.\0";
        case DDERR_PALETTEBUSY:
            return "Access to this palette is being refused because the palette is already locked by another thread.\0";
        case DDERR_PRIMARYSURFACEALREADYEXISTS:
            return "This process already has created a primary surface.\0";
        case DDERR_REGIONTOOSMALL:
            return "Region passed to Clipper::GetClipList is too small.\0";
        case DDERR_SURFACEALREADYATTACHED:
            return "This surface is already attached to the surface it is being attached to.\0";
        case DDERR_SURFACEALREADYDEPENDENT:
            return "This surface is already a dependency of the surface it is being made a dependency of.\0";
        case DDERR_SURFACEBUSY:
            return "Access to this surface is being refused because the surface is already locked by another thread.\0";
        case DDERR_SURFACEISOBSCURED:
            return "Access to surface refused because the surface is obscured.\0";
        case DDERR_SURFACELOST:
            return "Access to this surface is being refused because the surface memory is gone. The DirectDrawSurface object representing this surface should have Restore called on it.\0";
        case DDERR_SURFACENOTATTACHED:
            return "The requested surface is not attached.\0";
        case DDERR_TOOBIGHEIGHT:
            return "Height requested by DirectDraw is too large.\0";
        case DDERR_TOOBIGSIZE:
            return "Size requested by DirectDraw is too large, but the individual height and width are OK.\0";
        case DDERR_TOOBIGWIDTH:
            return "Width requested by DirectDraw is too large.\0";
        case DDERR_UNSUPPORTED:
            return "Action not supported.\0";
        case DDERR_UNSUPPORTEDFORMAT:
            return "FOURCC format requested is unsupported by DirectDraw.\0";
        case DDERR_UNSUPPORTEDMASK:
            return "Bitmask in the pixel format requested is unsupported by DirectDraw.\0";
        case DDERR_VERTICALBLANKINPROGRESS:
            return "Vertical blank is in progress.\0";
        case DDERR_WASSTILLDRAWING:
            return "Informs DirectDraw that the previous Blt which is transfering information to or from this Surface is incomplete.\0";
        case DDERR_WRONGMODE:
            return "This surface can not be restored because it was created in a different mode.\0";
        case DDERR_XALIGN:
            return "Rectangle provided was not horizontally aligned on required boundary.\0";

#endif 
#if 0        
		case D3DERR_BADMAJORVERSION:
            return "D3DERR_BADMAJORVERSION\0";
        case D3DERR_BADMINORVERSION:
            return "D3DERR_BADMINORVERSION\0";
        case D3DERR_EXECUTE_LOCKED:
            return "D3DERR_EXECUTE_LOCKED\0";
        case D3DERR_EXECUTE_NOT_LOCKED:
            return "D3DERR_EXECUTE_NOT_LOCKED\0";
        case D3DERR_EXECUTE_CREATE_FAILED:
            return "D3DERR_EXECUTE_CREATE_FAILED\0";
        case D3DERR_EXECUTE_DESTROY_FAILED:
            return "D3DERR_EXECUTE_DESTROY_FAILED\0";
        case D3DERR_EXECUTE_LOCK_FAILED:
            return "D3DERR_EXECUTE_LOCK_FAILED\0";
        case D3DERR_EXECUTE_UNLOCK_FAILED:
            return "D3DERR_EXECUTE_UNLOCK_FAILED\0";
        case D3DERR_EXECUTE_FAILED:
            return "D3DERR_EXECUTE_FAILED\0";
        case D3DERR_EXECUTE_CLIPPED_FAILED:
            return "D3DERR_EXECUTE_CLIPPED_FAILED\0";
        case D3DERR_TEXTURE_NO_SUPPORT:
            return "D3DERR_TEXTURE_NO_SUPPORT\0";
        case D3DERR_TEXTURE_NOT_LOCKED:
            return "D3DERR_TEXTURE_NOT_LOCKED\0";
        case D3DERR_TEXTURE_LOCKED:
            return "D3DERR_TEXTURELOCKED\0";
        case D3DERR_TEXTURE_CREATE_FAILED:
            return "D3DERR_TEXTURE_CREATE_FAILED\0";
        case D3DERR_TEXTURE_DESTROY_FAILED:
            return "D3DERR_TEXTURE_DESTROY_FAILED\0";
        case D3DERR_TEXTURE_LOCK_FAILED:
            return "D3DERR_TEXTURE_LOCK_FAILED\0";
        case D3DERR_TEXTURE_UNLOCK_FAILED:
            return "D3DERR_TEXTURE_UNLOCK_FAILED\0";
        case D3DERR_TEXTURE_LOAD_FAILED:
            return "D3DERR_TEXTURE_LOAD_FAILED\0";
        case D3DERR_MATRIX_CREATE_FAILED:
            return "D3DERR_MATRIX_CREATE_FAILED\0";
        case D3DERR_MATRIX_DESTROY_FAILED:
            return "D3DERR_MATRIX_DESTROY_FAILED\0";
        case D3DERR_MATRIX_SETDATA_FAILED:
            return "D3DERR_MATRIX_SETDATA_FAILED\0";
        case D3DERR_SETVIEWPORTDATA_FAILED:
            return "D3DERR_SETVIEWPORTDATA_FAILED\0";
        case D3DERR_MATERIAL_CREATE_FAILED:
            return "D3DERR_MATERIAL_CREATE_FAILED\0";
        case D3DERR_MATERIAL_DESTROY_FAILED:
            return "D3DERR_MATERIAL_DESTROY_FAILED\0";
        case D3DERR_MATERIAL_SETDATA_FAILED:
            return "D3DERR_MATERIAL_SETDATA_FAILED\0";
        case D3DERR_LIGHT_SET_FAILED:
            return "D3DERR_LIGHT_SET_FAILED\0";
#endif 

			
// DX5 
#if 0
    case DD_OK:
        // The request completed successfully.
        return TEXT("DD_OK");
        break;

    case DDERR_ALREADYINITIALIZED:
        // The object has already been initialized.
        return TEXT("DDERR_ALREADYINITIALIZED");
        break;

    case DDERR_BLTFASTCANTCLIP:
        // A DirectDrawClipper object is attached to a source surface 
        // that has passed into a call to the IDirectDrawSurface2::BltFast method. 
        return TEXT("DDERR_BLTFASTCANTCLIP");
        break;

    case DDERR_CANNOTATTACHSURFACE:
        // A surface cannot be attached to another requested surface. 
        return TEXT("DDERR_CANNOTATTACHSURFACE");
        break;

    case DDERR_CANNOTDETACHSURFACE:
        // A surface cannot be detached from another requested surface. 
        return TEXT("DDERR_CANNOTDETACHSURFACE");
        break;

    case DDERR_CANTCREATEDC:
        // Windows cannot create any more device contexts (DCs). 
        return TEXT("DDERR_CANTCREATEDC");
        break;

    case DDERR_CANTDUPLICATE:
        // Primary and 3D surfaces, or surfaces that are 
        // implicitly created, cannot be duplicated. 
        return TEXT("DDERR_CANTDUPLICATE");
        break;

    case DDERR_CANTLOCKSURFACE:
        // Access to this surface is refused because an 
        // attempt was made to lock the primary surface without DCI support. 
        return TEXT("DDERR_CANTLOCKSURFACE"); 
        break;

    case DDERR_CANTPAGELOCK:
        // An attempt to page lock a surface failed. 
        // Page lock will not work on a display-memory 
        // surface or an emulated primary surface.
        return TEXT("DDERR_CANTPAGELOCK"); 
        break;

    case DDERR_CANTPAGEUNLOCK:
        // An attempt to page unlock a surface failed. 
        // Page unlock will not work on a display-memory 
        // surface or an emulated primary surface. 
        return TEXT("DDERR_CANTPAGEUNLOCK");
        break;

    case DDERR_CLIPPERISUSINGHWND:
        // An attempt was made to set a clip list for a DirectDrawClipper 
        // object that is already monitoring a window handle. 
        return TEXT("DDERR_CLIPPERISUSINGHWND");
        break;

    case DDERR_COLORKEYNOTSET:
        // No source color key is specified for this operation
        return TEXT("DDERR_COLORKEYNOTSET");
        break;

    case DDERR_CURRENTLYNOTAVAIL:
        // No support is currently available. 
        return TEXT("DDERR_CURRENTLYNOTAVAIL");
        break;

    case DDERR_DCALREADYCREATED:
        // A device context (DC) has already been returned for this surface. 
        // Only one DC can be retrieved for each surface. 
        return TEXT("DDERR_DCALREADYCREATED");
        break;

    case DDERR_DIRECTDRAWALREADYCREATED:
        // A DirectDraw object representing this driver 
        // has already been created for this process. 
        return TEXT("DDERR_DIRECTDRAWALREADYCREATED");
        break;

    case DDERR_EXCEPTION:
        // An exception was encountered while 
        // performing the requested operation. 
        return TEXT("DDERR_EXCEPTION");
        break;

    case DDERR_EXCLUSIVEMODEALREADYSET:
        // An attempt was made to set the cooperative 
        // level when it was already set to exclusive. 
        return TEXT("DDERR_EXCLUSIVEMODEALREADYSET");
        break;

    case DDERR_GENERIC:
        // There is an undefined error condition. 
        return TEXT("DDERR_GENERIC");
        break;

    case DDERR_HEIGHTALIGN:
        // The height of the provided rectangle 
        // is not a multiple of the required alignment. 
        return TEXT("DDERR_HEIGHTALIGN");
        break;

    case DDERR_HWNDALREADYSET:
        // The DirectDraw cooperative level window 
        // handle has already been set. It cannot 
        // be reset while the process has surfaces or palettes created. 
        return TEXT("DDERR_HWNDALREADYSET");
        break;

    case DDERR_HWNDSUBCLASSED:
        // DirectDraw is prevented from restoring state because the
        // DirectDraw cooperative level window handle has been subclassed. 
        return TEXT("DDERR_HWNDSUBCLASSED");
        break;

    case DDERR_IMPLICITLYCREATED:
        // The surface cannot be restored because 
        // it is an implicitly created surface. 
        return TEXT("DDERR_IMPLICITLYCREATED");
        break;
 
    case DDERR_INCOMPATIBLEPRIMARY:
        // The primary surface creation request 
        // does not match with the existing primary surface. 
        return TEXT("DDERR_INCOMPATIBLEPRIMARY");
        break;

    case DDERR_INVALIDCAPS:
        // One or more of the capability bits 
        // passed to the callback function are incorrect. 
        return TEXT("DDERR_INVALIDCAPS");
        break;

    case DDERR_INVALIDCLIPLIST:
        // DirectDraw does not support the provided clip list.  
        return TEXT("DDERR_INVALIDCLIPLIST");
        break;

    case DDERR_INVALIDDIRECTDRAWGUID:
        // The globally unique identifier (GUID) passed to the
        // DirectDrawCreate function is not a valid DirectDraw driver identifier. 
        return TEXT("DDERR_INVALIDDIRECTDRAWGUID");
        break;

    case DDERR_INVALIDMODE:
        // DirectDraw does not support the requested mode. 
        return TEXT("DDERR_INVALIDMODE");
        break;

    case DDERR_INVALIDOBJECT:
        // DirectDraw received a pointer that was an invalid DirectDraw object. 
        return TEXT("DDERR_INVALIDOBJECT");
        break;

    case DDERR_INVALIDPARAMS:
        // One or more of the parameters passed to the method are incorrect. 
        return TEXT("DDERR_INVALIDPARAMS");
        break;

    case DDERR_INVALIDPIXELFORMAT:
        // The pixel format was invalid as specified. 
        return TEXT("DDERR_INVALIDPIXELFORMAT");
        break;

    case DDERR_INVALIDPOSITION:
        // The position of the overlay on the destination is no longer legal. 
        return TEXT("DDERR_INVALIDPOSITION");
        break;

    case DDERR_INVALIDRECT:
        // The provided rectangle was invalid. 
        return TEXT("DDERR_INVALIDRECT");
        break;

    case DDERR_INVALIDSURFACETYPE:
        // The requested operation could not be performed
        // because the surface was of the wrong type. 
        return TEXT("DDERR_INVALIDSURFACETYPE");
        break;

    case DDERR_LOCKEDSURFACES:
        // One or more surfaces are locked, 
        // causing the failure of the requested operation. 
        return TEXT("DDERR_LOCKEDSURFACES");
        break;

    case DDERR_MOREDATA:
        // There is more data available than the specified 
        // buffer size could hold.
        return TEXT("DDERR_MOREDATA");
        break;

    case DDERR_NO3D:
        // No 3D hardware or emulation is present. 
        return TEXT("DDERR_NO3D");
        break;

    case DDERR_NOALPHAHW:
        // No alpha acceleration hardware is present or available, 
        // causing the failure of the requested operation. 
        return TEXT("DDERR_NOALPHAHW");
        break;

    case DDERR_NOBLTHW:
        // No blitter hardware is present. 
        return TEXT("DDERR_NOBLTHW");
        break;

    case DDERR_NOCLIPLIST:
        // No clip list is available. 
        return TEXT("DDERR_NOCLIPLIST");
        break;

    case DDERR_NOCLIPPERATTACHED:
        // No DirectDrawClipper object is attached to the surface object. 
        return TEXT("DDERR_NOCLIPPERATTACHED");
        break;

    case DDERR_NOCOLORCONVHW:
        // The operation cannot be carried out because 
        // no color-conversion hardware is present or available. 
        return TEXT("DDERR_NOCOLORCONVHW");
        break;

    case DDERR_NOCOLORKEY:
        // The surface does not currently have a color key. 
        return TEXT("DDERR_NOCOLORKEY");
        break;

    case DDERR_NOCOLORKEYHW:
        // The operation cannot be carried out because there 
        // is no hardware support for the destination color key. 
        return TEXT("DDERR_NOCOLORKEYHW");
        break;

    case DDERR_NOCOOPERATIVELEVELSET:
        // A create function is called without the 
        // IDirectDraw2::SetCooperativeLevel method being called. 
        return TEXT("DDERR_NOCOOPERATIVELEVELSET");
        break;

    case DDERR_NODC:
        // No DC has ever been created for this surface. 
        return TEXT("DDERR_NODC");
        break;

    case DDERR_NODDROPSHW:
        // No DirectDraw raster operation (ROP) hardware is available. 
        return TEXT("DDERR_NODDROPSHW");
        break;

    case DDERR_NODIRECTDRAWHW:
        // Hardware-only DirectDraw object creation is not possible; 
        // the driver does not support any hardware. 
        return TEXT("DDERR_NODIRECTDRAWHW");
        break;

    case DDERR_NODIRECTDRAWSUPPORT:
        // DirectDraw support is not possible with the current display driver. 
        return TEXT("DDERR_NODIRECTDRAWSUPPORT");
        break;

    case DDERR_NOEMULATION: 
        // Software emulation is not available. 
        return TEXT("DDERR_NOEMULATION");
        break;

    case DDERR_NOEXCLUSIVEMODE:
        // The operation requires the application to have 
        // exclusive mode, but the application does not have exclusive mode. 
        return TEXT("DDERR_NOEXCLUSIVEMODE");
        break;

    case DDERR_NOFLIPHW: 
        // Flipping visible surfaces is not supported. 
        return TEXT("DDERR_NOFLIPHW");
        break;

    case DDERR_NOGDI: 
        // No GDI is present. 
        return TEXT("DDERR_NOGDI");
        break;

    case DDERR_NOHWND: 
        // Clipper notification requires a window handle, 
        // or no window handle has been previously set 
        // as the cooperative level window handle. 
        return TEXT("DDERR_NOHWND");
        break;

    case DDERR_NOMIPMAPHW: 
        // The operation cannot be carried out because no 
        // mipmap texture mapping hardware is present or available. 
        return TEXT("DDERR_NOMIPMAPHW");
        break;

    case DDERR_NOMIRRORHW: 
        // The operation cannot be carried out because 
        // no mirroring hardware is present or available. 
        return TEXT("DDERR_NOMIRRORHW");
        break;

    case DDERR_NONONLOCALVIDMEM: 
        // An attempt was made to allocate non-local video memory
        // from a device that does not support non-local video memory.
        return TEXT("DDERR_NONONLOCALVIDMEM");
        break;
 
    case DDERR_NOOVERLAYDEST: 
        // The IDirectDrawSurface2::GetOverlayPosition method 
        // is called on an overlay that the IDirectDrawSurface2::UpdateOverlay 
        // method has not been called on to establish a destination. 
        return TEXT("DDERR_NOOVERLAYDEST");
        break;

    case DDERR_NOOVERLAYHW: 
        // The operation cannot be carried out because 
        // no overlay hardware is present or available. 
        return TEXT("DDERR_NOOVERLAYHW");
        break;

    case DDERR_NOPALETTEATTACHED: 
        // No palette object is attached to this surface. 
        return TEXT("DDERR_NOPALETTEATTACHED");
        break;

    case DDERR_NOPALETTEHW: 
        // There is no hardware support for 16- or 256-color palettes. 
        return TEXT("DDERR_NOPALETTEHW");
        break;

    case DDERR_NORASTEROPHW: 
        // The operation cannot be carried out because 
        // no appropriate raster operation hardware is present or available. 
        return TEXT("DDERR_NORASTEROPHW");
        break;

    case DDERR_NOROTATIONHW: 
        // The operation cannot be carried out because 
        // no rotation hardware is present or available. 
        return TEXT("DDERR_NOROTATIONHW");
        break;

    case DDERR_NOSTRETCHHW: 
        // The operation cannot be carried out because 
        // there is no hardware support for stretching. 
        return TEXT("DDERR_NOSTRETCHHW");
        break;

    case DDERR_NOT4BITCOLOR: 
        // The DirectDrawSurface object is not using a 
        // 4-bit color palette and the requested operation 
        // requires a 4-bit color palette. 
        return TEXT("DDERR_NOT4BITCOLOR");
        break;

    case DDERR_NOT4BITCOLORINDEX: 
        // The DirectDrawSurface object is not using a 4-bit 
        // color index palette and the requested operation 
        // requires a 4-bit color index palette. 
        return TEXT("DDERR_NOT4BITCOLORINDEX");
        break;

    case DDERR_NOT8BITCOLOR: 
        // The DirectDrawSurface object is not using an 8-bit 
        // color palette and the requested operation requires 
        // an 8-bit color palette. 
        return TEXT("DDERR_NOT8BITCOLOR");
        break;

    case DDERR_NOTAOVERLAYSURFACE: 
        // An overlay component is called for a non-overlay surface. 
        return TEXT("DDERR_NOTAOVERLAYSURFACE");
        break;

    case DDERR_NOTEXTUREHW: 
        // The operation cannot be carried out because no 
        // texture-mapping hardware is present or available. 
        return TEXT("DDERR_NOTEXTUREHW");
        break;

    case DDERR_NOTFLIPPABLE: 
        // An attempt has been made to flip a surface that cannot be flipped. 
        return TEXT("DDERR_NOTFLIPPABLE");
        break;

    case DDERR_NOTFOUND: 
        // The requested item was not found. 
        return TEXT("DDERR_NOTFOUND");
        break;

    case DDERR_NOTINITIALIZED: 
        // An attempt was made to call an interface method of a DirectDraw object 
        // created by CoCreateInstance before the object was initialized. 
        return TEXT("DDERR_NOTINITIALIZED");
        break;

    case DDERR_NOTLOCKED: 
        // An attempt is made to unlock a surface that was not locked. 
        return TEXT("DDERR_NOTLOCKED");
        break;

    case DDERR_NOTPAGELOCKED: 
        // An attempt is made to page unlock a surface 
        // with no outstanding page locks. 
        return TEXT("DDERR_NOTPAGELOCKED");
        break;

    case DDERR_NOTPALETTIZED: 
        // The surface being used is not a palette-based surface. 
        return TEXT("DDERR_NOTPALETTIZED");
        break;

    case DDERR_NOVSYNCHW: 
        // The operation cannot be carried out because 
        // there is no hardware support for vertical blank synchronized operations. 
        return TEXT("DDERR_NOVSYNCHW");
        break;

    case DDERR_NOZBUFFERHW: 
        // The operation to create a z-buffer in display memory 
        // or to perform a blit using a z-buffer cannot be carried 
        // out because there is no hardware support for z-buffers. 
        return TEXT("DDERR_NOZBUFFERHW");
        break;

    case DDERR_NOZOVERLAYHW: 
        // The overlay surfaces cannot be z-layered based 
        // on the z-order because the hardware does not 
        // support z-ordering of overlays. 
        return TEXT("DDERR_NOZOVERLAYHW");
        break;

    case DDERR_OUTOFCAPS: 
        // The hardware needed for the requested operation has already been allocated. 
        return TEXT("DDERR_OUTOFCAPS");
        break;

    case DDERR_OUTOFMEMORY: 
        // DirectDraw does not have enough memory to perform the operation. 
        return TEXT("DDERR_OUTOFMEMORY");
        break;

    case DDERR_OUTOFVIDEOMEMORY: 
        // DirectDraw does not have enough display memory to perform the operation. 
        return TEXT("DDERR_OUTOFVIDEOMEMORY");
        break;

    case DDERR_OVERLAYCANTCLIP: 
        // The hardware does not support clipped overlays. 
        return TEXT("DDERR_OVERLAYCANTCLIP");
        break;

    case DDERR_OVERLAYCOLORKEYONLYONEACTIVE: 
        // An attempt was made to have more than one color key active on an overlay. 
        return TEXT("DDERR_OVERLAYCOLORKEYONLYONEACTIVE");
        break;

    case DDERR_OVERLAYNOTVISIBLE: 
        // The IDirectDrawSurface2::GetOverlayPosition method is called on a hidden overlay. 
        return TEXT("DDERR_OVERLAYNOTVISIBLE");
        break;

    case DDERR_PALETTEBUSY: 
        // Access to this palette is refused 
        // because the palette is locked by another thread. 
        return TEXT("DDERR_PALETTEBUSY");
        break;

    case DDERR_PRIMARYSURFACEALREADYEXISTS: 
        // This process has already created a primary surface. 
        return TEXT("DDERR_PRIMARYSURFACEALREADYEXISTS");
        break;

    case DDERR_REGIONTOOSMALL: 
        // The region passed to the 
        // IDirectDrawClipper::GetClipList method is too small. 
        return TEXT("DDERR_REGIONTOOSMALL");
        break;

    case DDERR_SURFACEALREADYATTACHED: 
        // An attempt was made to attach a surface to 
        // another surface to which it is already attached. 
        return TEXT("DDERR_SURFACEALREADYATTACHED");
        break;

    case DDERR_SURFACEALREADYDEPENDENT: 
        // An attempt was made to make a surface a dependency 
        // of another surface to which it is already dependent. 
        return TEXT("DDERR_SURFACEALREADYDEPENDENT");
        break;

    case DDERR_SURFACEBUSY: 
        // Access to the surface is refused because the 
        // surface is locked by another thread. 
        return TEXT("DDERR_SURFACEBUSY");
        break;

    case DDERR_SURFACEISOBSCURED: 
        // Access to the surface is refused 
        // because the surface is obscured. 
        return TEXT("DDERR_SURFACEISOBSCURED");
        break;

    case DDERR_SURFACELOST: 
        // Access to the surface is refused because the 
        // surface memory is gone. The DirectDrawSurface 
        // object representing this surface should have 
        // the IDirectDrawSurface2::Restore method called on it. 
        return TEXT("DDERR_SURFACELOST");
        break;

    case DDERR_SURFACENOTATTACHED: 
        // The requested surface is not attached. 
        return TEXT("DDERR_SURFACENOTATTACHED");
        break;

    case DDERR_TOOBIGHEIGHT: 
        // The height requested by DirectDraw is too large. 
        return TEXT("DDERR_TOOBIGHEIGHT");
        break;

    case DDERR_TOOBIGSIZE: 
        // The size requested by DirectDraw is too large. 
        // However, the individual height and width are OK.
        return TEXT("DDERR_TOOBIGSIZE");
        break;

    case DDERR_TOOBIGWIDTH: 
        // The width requested by DirectDraw is too large. 
        return TEXT("DDERR_TOOBIGWIDTH");
        break;

    case DDERR_UNSUPPORTED: 
        // The operation is not supported. 
        return TEXT("DDERR_UNSUPPORTED");
        break;

    case DDERR_UNSUPPORTEDFORMAT: 
        // The FourCC format requested is not supported by DirectDraw. 
        return TEXT("DDERR_UNSUPPORTEDFORMAT");
        break;

    case DDERR_UNSUPPORTEDMASK: 
        // The bitmask in the pixel format requested is not supported by DirectDraw. 
        return TEXT("DDERR_UNSUPPORTEDMASK");
        break;

    case DDERR_UNSUPPORTEDMODE: 
        // The display is currently in an unsupported mode. 
        return TEXT("DDERR_UNSUPPORTEDMODE");
        break;

    case DDERR_VERTICALBLANKINPROGRESS: 
        // A vertical blank is in progress. 
        return TEXT("DDERR_VERTICALBLANKINPROGRESS");
        break;

    case DDERR_WASSTILLDRAWING: 
        // The previous blit operation that is transferring 
        // information to or from this surface is incomplete. 
        return TEXT("DDERR_WASSTILLDRAWING");
        break;

    case DDERR_WRONGMODE:
        // This surface cannot be restored because it was created in a different mode. 
        return TEXT("DDERR_WRONGMODE");
        break;

    case DDERR_XALIGN:
        // The provided rectangle was not horizontally aligned on a required boundary. 
        return TEXT("DDERR_XALIGN");
        break;

#endif

    case DDERR_VIDEONOTACTIVE:
        // The video port is not active
        return TEXT("DDERR_VIDEONOTACTIVE");
        break;



        //
        // D3D Immediate Mode Errors
        //

    case D3DERR_BADMAJORVERSION:
        // ???
        return TEXT("D3DERR_BADMAJORVERSION");
        break;

    case D3DERR_BADMINORVERSION:
        // ???
        return TEXT("D3DERR_BADMINORVERSION");
        break;

    case D3DERR_INVALID_DEVICE:
        // ???
        return TEXT("D3DERR_INVALID_DEVICE");
        break;

    case D3DERR_INITFAILED:
        // ???
        return TEXT("D3DERR_INITFAILED");
        break;

    case D3DERR_EXECUTE_CREATE_FAILED:
        // ???
        return TEXT("D3DERR_EXECUTE_CREATE_FAILED");
        break;

    case D3DERR_EXECUTE_DESTROY_FAILED:
        // ???
        return TEXT("D3DERR_EXECUTE_DESTROY_FAILED");
        break;

    case D3DERR_EXECUTE_LOCK_FAILED:
        // ???
        return TEXT("D3DERR_EXECUTE_LOCK_FAILED");
        break;

    case D3DERR_EXECUTE_UNLOCK_FAILED:
        // ???
        return TEXT("D3DERR_EXECUTE_UNLOCK_FAILED");
        break;

    case D3DERR_EXECUTE_LOCKED:
        // ???
        return TEXT("D3DERR_EXECUTE_LOCKED");
        break;

    case D3DERR_EXECUTE_NOT_LOCKED:
        // ???
        return TEXT("D3DERR_EXECUTE_NOT_LOCKED");
        break;

    case D3DERR_EXECUTE_FAILED:
        // ???
        return TEXT("D3DERR_EXECUTE_FAILED");
        break;

    case D3DERR_EXECUTE_CLIPPED_FAILED:
        // ???
        return TEXT("D3DERR_CLIPPED_FAILED");
        break;

    case D3DERR_TEXTURE_NO_SUPPORT:
        // ???
        return TEXT("D3DERR_TEXTURE_NO_SUPPORT");
        break;

    case D3DERR_TEXTURE_CREATE_FAILED:
        // ???
        return TEXT("D3DERR_TEXTURE_CREATE_FAILED");
        break;

    case D3DERR_TEXTURE_DESTROY_FAILED:
        // ???
        return TEXT("D3DERR_TEXTURE_DESTROY_FAILED");
        break;

    case D3DERR_TEXTURE_LOCK_FAILED:
        // ???
        return TEXT("D3DERR_TEXTURE_LOCK_FAILED");
        break;

    case D3DERR_TEXTURE_UNLOCK_FAILED:
        // ???
        return TEXT("D3DERR_TEXTURE_UNLOCK_FAILED");
        break;

    case D3DERR_TEXTURE_LOAD_FAILED:
        // ???
        return TEXT("D3DERR_LOAD_FAILED");
        break;

    case D3DERR_TEXTURE_SWAP_FAILED:
        // ???
        return TEXT("D3DERR_SWAP_FAILED");
        break;

    case D3DERR_TEXTURE_LOCKED:
        // ???
        return TEXT("D3DERR_TEXTURE_LOCKED");
        break;

    case D3DERR_TEXTURE_NOT_LOCKED:
        // ???
        return TEXT("D3DERR_TEXTURE_NOT_LOCKED");
        break;

    case D3DERR_TEXTURE_GETSURF_FAILED:
        // ???
        return TEXT("D3DERR_TEXTURE_GETSURF_FAILED");
        break;

    case D3DERR_MATRIX_CREATE_FAILED:
        // ???
        return TEXT("D3DERR_MATRIX_CREATE_FAILED");
        break;

    case D3DERR_MATRIX_DESTROY_FAILED:
        // ???
        return TEXT("D3DERR_MATRIX_DESTROY_FAILED");
        break;

    case D3DERR_MATRIX_SETDATA_FAILED:
        // ???
        return TEXT("D3DERR_MATRIX_SETDATA_FAILED");
        break;

    case D3DERR_MATRIX_GETDATA_FAILED:
        // ???
        return TEXT("D3DERR_MAXTRIX_GETDATA_FAILED");
        break;

    case D3DERR_SETVIEWPORTDATA_FAILED:
        // ???
        return TEXT("D3DERR_SETVIEWPORTDATA_FAILED");
        break;

    case D3DERR_INVALIDCURRENTVIEWPORT:
        // ???
        return TEXT("D3DERR_INVALIDCURRENTVIEWPORT");
        break;
	
    case D3DERR_INVALIDPRIMITIVETYPE:
        // ???
        return TEXT("D3DERR_INVALIDPRIMITIVETYPE");
        break;

    case D3DERR_INVALIDVERTEXTYPE:
        // ???
        return TEXT("D3DERR_INVALIDVERTEXTYPE");
        break;

    case D3DERR_TEXTURE_BADSIZE:
        // ???
        return TEXT("D3DERR_TEXTURE_BADSIZE");
        break;

    case D3DERR_MATERIAL_CREATE_FAILED:
        // ???
        return TEXT("D3DERR_MATERIAL_CREATE_FAILED");
        break;

    case D3DERR_MATERIAL_DESTROY_FAILED:
        // ???
        return TEXT("D3DERR_MATERIAL_DESTROY_FAILED");
        break;

    case D3DERR_MATERIAL_SETDATA_FAILED:
        // ???
        return TEXT("D3DERR_MATERIAL_SETDATA_FAILED");
        break;

    case D3DERR_MATERIAL_GETDATA_FAILED:
        // ???
        return TEXT("D3DERR_MATERIAL_GETDATA_FAILED");
        break;

    case D3DERR_INVALIDPALETTE:
        // ???
        return TEXT("D3DERR_INVALIDPALETTE");
        break;

    case D3DERR_ZBUFF_NEEDS_SYSTEMMEMORY:
        // ???
        return TEXT("D3DERR_ZBUFF_NEEDS_SYSTEMMEMORY");
        break;

    case D3DERR_ZBUFF_NEEDS_VIDEOMEMORY:
        // ???
        return TEXT("D3DERR_ZBUFF_NEEDS_VIDEOMEMORY");
        break;

    case D3DERR_SURFACENOTINVIDMEM:
        // ???
        return TEXT("D3DERR_SURFACENOTINVIDMEM");
        break;

    case D3DERR_LIGHT_SET_FAILED:
        // ???
        return TEXT("D3DERR_LIGHT_SET_FAILED");
        break;

    case D3DERR_LIGHTHASVIEWPORT:
        // ???
        return TEXT("D3DERR_LIGHTHASVIEWPORT");
        break;

    case D3DERR_LIGHTNOTINTHISVIEWPORT:
        // ???
        return TEXT("D3DERR_LIGHTNOTINTHISVIEWPORT");
        break;

    case D3DERR_SCENE_IN_SCENE:
        // ???
        return TEXT("D3DERR_SCENE_IN_SCENE");
        break;

    case D3DERR_SCENE_NOT_IN_SCENE:
        // ???
        return TEXT("D3DERR_SCENE_NOT_IN_SCENE");
        break;

    case D3DERR_SCENE_BEGIN_FAILED:
        // ???
        return TEXT("D3DERR_SCENE_BEGIN_FAILED");
        break;

    case D3DERR_SCENE_END_FAILED:
        // ???
        return TEXT("D3DERR_SCENE_END_FAILED");
        break;

    case D3DERR_INBEGIN:
        // ???
        return TEXT("D3DERR_INBEGIN");
        break;

    case D3DERR_NOTINBEGIN:
        // ???
        return TEXT("D3DERR_NOTINBEGIN");
        break;

    case D3DERR_NOVIEWPORTS:
        // ???
        return TEXT("D3DERR_NOVIEWPORTS");
        break;

    case D3DERR_VIEWPORTDATANOTSET:
        // ???
        return TEXT("D3DERR_VIEWPORTDATANOTSET");
        break;

    case D3DERR_VIEWPORTHASNODEVICE:
        // ???
        return TEXT("D3DERR_VIEWPORTHASNODEVICE");
        break;


// RM ERRORS        
		case D3DRMERR_BADOBJECT:
            return "D3DRMERR_BADOBJECT\0";
        case D3DRMERR_BADTYPE:
            return "D3DRMERR_BADTYPE\0";
        case D3DRMERR_BADALLOC:
            return "D3DRMERR_BADALLOC\0";
        case D3DRMERR_FACEUSED:
            return "D3DRMERR_FACEUSED\0";
        case D3DRMERR_NOTFOUND:
            return "D3DRMERR_NOTFOUND\0";
        case D3DRMERR_NOTDONEYET:
            return "D3DRMERR_NOTDONEYET\0";
        case D3DRMERR_FILENOTFOUND:
            return "The file was not found.\0";
        case D3DRMERR_BADFILE:
            return "D3DRMERR_BADFILE\0";
        case D3DRMERR_BADDEVICE:
            return "D3DRMERR_BADDEVICE\0";
        case D3DRMERR_BADVALUE:
            return "D3DRMERR_BADVALUE\0";
        case D3DRMERR_BADMAJORVERSION:
            return "D3DRMERR_BADMAJORVERSION\0";
        case D3DRMERR_BADMINORVERSION:
            return "D3DRMERR_BADMINORVERSION\0";
        case D3DRMERR_UNABLETOEXECUTE:
            return "D3DRMERR_UNABLETOEXECUTE\0";
        default:
            return "Unrecognized error value.\0";
    }
}



// d3dcalls.cpp
/***************************************************************************/
/*                            Creation of D3D                              */
/***************************************************************************/
BOOL
GD3Device::CreateD3D()
{
    LastError = lpDD->QueryInterface(IID_IDirect3D, (LPVOID*)&lpD3D);
    if (LastError != DD_OK) {
        SetErrorString("Creation of Interface to Direct3D failed.\n"
			"Please install Direct3D\n"
			"Downloadable from http://www.microsoft.com/directx\n");
        goto exit_with_error;
    }

	dx5=FALSE;
	dx6=FALSE;


	if ((dxLevel == 0) || (dxLevel>= D3DAPP_DXLEVEL5)) {
	
		LastError = lpDD->QueryInterface(IID_IDirect3D2, (LPVOID*)&lpD3D2);
		if (LastError != DD_OK) {
		} else dx5 = TRUE;

		if ((dxLevel == 0) || (dxLevel>= D3DAPP_DXLEVEL6)) {
			LastError = lpDD->QueryInterface(IID_IDirect3D3, (LPVOID*)&lpD3D3);
			if (LastError != DD_OK) {
			} else dx6 = TRUE;
		}
	
	} // DX5 

    return TRUE;

exit_with_error:
    return FALSE;
}

/***************************************************************************/
/*                           D3D Device Enumeration                        */
/***************************************************************************/
/*
 * enumDeviceFunc
 * Device enumeration callback.  Record information about the D3D device
 * reported by D3D.
 */
static HRESULT
WINAPI enumDeviceFunc(LPGUID lpGuid, LPSTR lpDeviceDescription,
                      LPSTR lpDeviceName, LPD3DDEVICEDESC lpHWDesc,
                      LPD3DDEVICEDESC lpHELDesc, LPVOID lpContext)
{
    lpContext = lpContext;

	GD3Device *dev = (GD3Device *) lpContext;

    /*
     * Don't accept any hardware D3D devices if emulation only option is set
     */
    if (lpHWDesc->dcmColorModel && dev->bOnlyEmulation)
        return D3DENUMRET_OK;
	
	D3DAppD3DDriver &driver = dev->Driver[dev->NumDrivers];
    /*
     * Record the D3D driver's information
     */
    memcpy(&dev->Driver[dev->NumDrivers].Guid, lpGuid, sizeof(GUID));
    
	driver.About = lpDeviceDescription;
    driver.Name =  lpDeviceName;
    driver.NameUi =  lpDeviceName;
	
	driver.bIsMmx = FALSE;
	driver.bUseSystemMemory=FALSE;

	if (driver.NameUi == "Ramp Emulation")
		driver.NameUi = "High Speed";
	else if (driver.NameUi == "RGB Emulation")
		driver.NameUi = "High Quality";
	else if (driver.NameUi == "MMX Emulation") {
		driver.NameUi = "MMX Speed";
		driver.bIsMmx = TRUE;
		driver.bUseSystemMemory=TRUE;
	} else if (driver.NameUi == "Direct3D HAL")
		driver.NameUi = "Hardware";


    /*
     * Is this a hardware device or software emulation?  Checking the color
     * model for a valid model works.
     */
    if (lpHWDesc->dcmColorModel) {
        driver.bIsHardware = TRUE;
        memcpy(&dev->Driver[dev->NumDrivers].Desc, lpHWDesc,sizeof(D3DDEVICEDESC));
    } else {
        driver.bIsHardware = FALSE;
        memcpy(&dev->Driver[dev->NumDrivers].Desc, lpHELDesc,sizeof(D3DDEVICEDESC));
    }
    /*
     * Does this driver do texture mapping?
     */
    driver.bDoesTextures =
        (driver.Desc.dpcTriCaps.dwTextureCaps &
         D3DPTEXTURECAPS_PERSPECTIVE) ? TRUE : FALSE;

	if (driver.bIsHardware  /* || driver.bIsMmx */)
		 driver.bUseTextureFiltering =
        (driver.Desc.dpcTriCaps.dwTextureFilterCaps &  D3DPTFILTERCAPS_LINEAR) ? TRUE : FALSE;
    else driver.bUseTextureFiltering = FALSE;

	/*
     * Can this driver use a z-buffer?
     */
    driver.bDoesZBuffer = driver.Desc.dwDeviceZBufferBitDepth
                ? TRUE : FALSE;
    /*
     * Can this driver render to the Windows display depth
     */
    driver.bCanDoWindow =
        (driver.Desc.dwDeviceRenderBitDepth &
         dev->BPPToDDBD(dev->WindowsDisplay.bpp)) ? TRUE : FALSE;

    if (!dev->bIsPrimary)
        driver.bCanDoWindow = FALSE;
	
	if (dev->SoftwareDriver <0) {
		// choose software driver 
		if (driver.bCanDoWindow && !driver.bIsHardware)
			dev->SoftwareDriver = dev->NumDrivers;

	}

    dev->NumDrivers++;
    if (dev->NumDrivers == D3DAPP_MAXD3DDRIVERS)
        return (D3DENUMRET_CANCEL);
    return (D3DENUMRET_OK);
}

/*
 * EnumDrivers
 * Get the available drivers from Direct3D by enumeration.
 */
BOOL
GD3Device::EnumDrivers()
{
    SoftwareDriver = -1;
	NumDrivers = 0;
/*
	// dx6 enumerates only RGB + HAL, no Ramp & MMX 
	if (dx6)
		LastError = lpD3D3->EnumDevices(enumDeviceFunc, this);
	else 
*/
	if (dx5)
		LastError = lpD3D2->EnumDevices(enumDeviceFunc, this);
    else LastError = lpD3D->EnumDevices(enumDeviceFunc, this);

    if (LastError != DD_OK) {
        SetErrorString("Enumeration of drivers failed.\n%s",
                              ErrorToString(LastError));
        return FALSE;
    }
    CurrDriver = 0;
    return TRUE;
}

/***************************************************************************/
/*                    Enumeration of texure format                         */
/***************************************************************************/
/*
 * EnumTextureFormatsCallback
 * Record information about each texture format the current D3D driver can
 * support. Choose one as the default format (paletted formats are prefered)
 * and return it through lpContext.
 */
static HRESULT
CALLBACK EnumTextureFormatsCallback(LPDDSURFACEDESC lpDDSD, LPVOID lpContext)
{
    unsigned long m;
    int r, g, b, a;

	GD3Device *dev = (GD3Device *) lpContext;
    /*
     * Record the DDSURFACEDESC of this texture format
     */
    memset(&dev->TextureFormat[dev->NumTextureFormats], 0, sizeof(D3DAppTextureFormat));
    memcpy(&dev->TextureFormat[dev->NumTextureFormats].ddsd, lpDDSD, sizeof(DDSURFACEDESC));

	DWORD	pixfFlags = lpDDSD->ddpfPixelFormat.dwFlags;
	/*
     * Is this format palettized?  How many bits?  Otherwise, how many RGB
     * bits?
     */

    dev->TextureFormat[dev->NumTextureFormats].bPalettized = FALSE;

    if (pixfFlags & DDPF_PALETTEINDEXED8) {
        dev->TextureFormat[dev->NumTextureFormats].bPalettized = TRUE;
        dev->TextureFormat[dev->NumTextureFormats].IndexBPP = 8;
    } else if (pixfFlags & DDPF_PALETTEINDEXED4) {
        dev->TextureFormat[dev->NumTextureFormats].bPalettized = TRUE;
        dev->TextureFormat[dev->NumTextureFormats].IndexBPP = 4;
    } 
	else
    if (pixfFlags & DDPF_RGB) 
    {
        dev->TextureFormat[dev->NumTextureFormats].IndexBPP = 0;
        for (r = 0, m = lpDDSD->ddpfPixelFormat.dwRBitMask; !(m & 1);
                                                               r++, m >>= 1);
        for (r = 0; m & 1; r++, m >>= 1);
        for (g = 0, m = lpDDSD->ddpfPixelFormat.dwGBitMask; !(m & 1);
                                                               g++, m >>= 1);
        for (g = 0; m & 1; g++, m >>= 1);
        for (b = 0, m = lpDDSD->ddpfPixelFormat.dwBBitMask; !(m & 1);  b++, m >>= 1);
		for (b = 0; m & 1; b++, m >>= 1);


		a=0;
		if (lpDDSD->ddpfPixelFormat.dwFlags & DDPF_ALPHAPIXELS || (lpDDSD->ddpfPixelFormat.dwRGBAlphaBitMask!=0)) { // new
			//TRACE("Direct 3D: Has alpha texture format ");

	        for (a = 0, m = lpDDSD->ddpfPixelFormat.dwRGBAlphaBitMask; !(m & 1); a++, m >>= 1);
			for (a = 0; m & 1; a++, m >>= 1);
		}

        dev->TextureFormat[dev->NumTextureFormats].RedBPP = r;
        dev->TextureFormat[dev->NumTextureFormats].GreenBPP = g;
        dev->TextureFormat[dev->NumTextureFormats].BlueBPP = b;
        dev->TextureFormat[dev->NumTextureFormats].AlphaBPP = a;

    }
	// LUMINANCE interesting 
    else {
		TRACE("Unsupported Texture format found %x \n",pixfFlags);
	    return DDENUMRET_OK;

	}
	
	/*
     * If lpStarFormat is -1, this is the first format.  Select it.
     */
    if (dev->StartTextureFormat == -1)
        dev->StartTextureFormat = dev->NumTextureFormats;

	else {
    /* 
     * If this format is paletted, select it.
     */
/*
    if (dev->TextureFormat[dev->NumTextureFormats].bPalettized) {
        dev->StartTextureFormat = dev->NumTextureFormats;
    }
*/
	// prefer RGB over paletteized
    if (dev->TextureFormat[dev->StartTextureFormat].bPalettized && !(lpDDSD->ddpfPixelFormat.dwFlags & DDPF_ALPHAPIXELS)) {
		if (dev->TextureFormat[dev->NumTextureFormats].bPalettized && dev->TextureFormat[dev->StartTextureFormat].IndexBPP != 8) 
			dev->StartTextureFormat = dev->NumTextureFormats;
        else 
			dev->StartTextureFormat = dev->NumTextureFormats; // take this rgb 
    }
    else 
		; // dev->StartTextureFormat = dev->NumTextureFormats; // take this rgb 

	}
    dev->NumTextureFormats++;
    return DDENUMRET_OK;

}

/*
 * EnumTextureFormats
 * Get a list of available texture map formats from the Direct3D driver by
 * enumeration.  Choose a default format (paletted is prefered).
 */
BOOL
GD3Device::EnumTextureFormats()
{
    /*
     * Set the default format to -1 to let the callback know it's being 
     * called for the first time.
     */
    StartTextureFormat = -1;
    NumTextureFormats = 0;
/*
	to do: 
	if (dx6 && lpD3DDevice3) {
	  LastError = lpD3DDevice3->EnumTextureFormats( EnumTextureFormatsCallback,(LPVOID)this);
	}
	else
*/
	if (dx5 && lpD3DDevice2) {
	  LastError = lpD3DDevice2->EnumTextureFormats( EnumTextureFormatsCallback,(LPVOID)this);
	}
    else 
      LastError = lpD3DDevice->EnumTextureFormats( EnumTextureFormatsCallback,(LPVOID)this);
    if (LastError != DD_OK) {
        SetErrorString("Enumeration of texture formats failed.\n%s",ErrorToString(LastError));
        return FALSE;
    }
    memcpy(&ThisTextureFormat, &TextureFormat[StartTextureFormat],
           sizeof(D3DAppTextureFormat));
    CurrTextureFormat = StartTextureFormat;
    return TRUE;
}

/***************************************************************************/
/*                               Device creation                           */
/***************************************************************************/
/*
 * CreateDevice
 * Create the D3D device and enumerate the texture formats
 */
BOOL
GD3Device::CreateDevice(int driver)
{

    RELEASE(lpD3DDevice);
    RELEASE(lpD3DDevice2);
    RELEASE(lpD3DDevice3);

    if (Driver[driver].bIsHardware && !bBackBufferInVideo) {
        SetErrorString("Could not fit the rendering surfaces in video memory for this hardware device.\n");
        goto exit_with_error;
    }

    CurrDriver = driver;

	//    memcpy(&ThisDriver, &Driver[driver], sizeof(D3DAppD3DDriver));
   
	ThisDriver = Driver[driver]; 

	if (lpD3D3 != NULL) {

 	   LPDIRECTDRAWSURFACE4 lpDS4=NULL;

	   LastError = lpBackBuffer->QueryInterface(IID_IDirectDrawSurface4,(LPVOID*)&lpDS4);

	   if (LastError != DD_OK) {
				TRACE("Can't get Create IDirectDrawSurface4 from backbuffer \n%s",ErrorToString(LastError));
	   }	

	   LastError = lpD3D3->CreateDevice(Driver[driver].Guid, lpDS4, &lpD3DDevice3,NULL);

	   RELEASE(lpDS4);

      if (LastError == DD_OK) {
	      LastError = lpD3DDevice3->QueryInterface(IID_IDirect3DDevice2,(LPVOID*)&lpD3DDevice2);
		  if (LastError != DD_OK) {
				SetErrorString("Creating IDirect3DDevice2 from DX6 device failed.\n%s",ErrorToString(LastError));
			    goto exit_with_error;
		  }	
		  dx6 = TRUE;
		  
	      LastError = lpD3DDevice2->QueryInterface(IID_IDirect3DDevice,(LPVOID*)&lpD3DDevice);
		  if (LastError != DD_OK) {
				SetErrorString("Creating IDirect3DDevice from DX6 device failed.\n%s",ErrorToString(LastError));
			    goto exit_with_error;
		  }	
      }	
	  else {
		dx6 = FALSE;
	  }	

	}

	if (!lpD3DDevice3) { // fall back DX5 & DX 3
	
	if (lpD3D2 !=NULL) {
      
	  LastError = lpD3D2->CreateDevice(Driver[driver].Guid, lpBackBuffer, &lpD3DDevice2);

      if (LastError == DD_OK) {
	      LastError = lpD3DDevice2->QueryInterface(IID_IDirect3DDevice,(LPVOID*)&lpD3DDevice);
		  if (LastError != DD_OK) {
				SetErrorString("Create D3D from DX5 device failed.\n%s",ErrorToString(LastError));
			    goto exit_with_error;
		  }	
		  dx5 = TRUE;
      }	
	  else {
		dx5 = FALSE;
		LastError = lpBackBuffer->QueryInterface(Driver[driver].Guid,(LPVOID*)&lpD3DDevice);
	  }

	}
	else 
		LastError = lpBackBuffer->QueryInterface(Driver[driver].Guid,(LPVOID*)&lpD3DDevice);
	
	}


    if (LastError != DD_OK) {
        SetErrorString("Create D3D device failed.\n%s",ErrorToString(LastError));
        goto exit_with_error;
    }
    CurrDriver = driver;
    NumTextureFormats = 0;
    if (Driver[driver].bDoesTextures) {
        if (!EnumTextureFormats())
            goto exit_with_error;
    }

    return TRUE;
exit_with_error:
    RELEASE(lpD3DDevice);
    RELEASE(lpD3DDevice2);
    RELEASE(lpD3DDevice3);
    return FALSE;
}

/***************************************************************************/
/*                      Setting the render state                           */
/***************************************************************************/
/*
 * SetRenderState
 * Create and execute an execute buffer which will set the render state and
 * light state for the current viewport.
 */
BOOL
GD3Device::SetRenderState()
{

	// done from RenderState	
    return TRUE;

}

/***************************************************************************/
/*                               Device creation                           */
/***************************************************************************/
/*
 * CreateViewpoint
 * Create the D3D viewpoint 
 */
BOOL
GD3Device::CreateViewport(int w,int h)
{

	RELEASE(lpD3DViewport);
    RELEASE(lpD3DViewport2);
    RELEASE(lpD3DViewport3);

    HRESULT rval=0;
	

	if (lpD3DDevice == NULL) {
        SetErrorString("D3D device not created.\n");
		return FALSE;
	}

	if (dx6) { 
		rval = lpD3D3->CreateViewport(&lpD3DViewport3, NULL);
		if (rval != D3D_OK) {
			SetErrorString("Create D3D viewport failed.\n%s", ErrorToString(rval));
			return FALSE;
		}
		/*
		* Add the viewport to the D3D device
		*/
		rval = lpD3DDevice3->AddViewport(lpD3DViewport3);
		if (rval != D3D_OK) {
			SetErrorString("Add D3D viewport failed.\n%s", ErrorToString(rval));
			return FALSE;
		}

		rval = lpD3DViewport3->QueryInterface(IID_IDirect3DViewport, (LPVOID*)&lpD3DViewport);
		if (rval != D3D_OK) {
			SetErrorString("Query IID_IDirect3DViewport failed.\n%s", ErrorToString(rval));
			return FALSE;
		}
	}
	else
	if (dx5) { 
		rval = lpD3D2->CreateViewport(&lpD3DViewport2, NULL);
		if (rval != D3D_OK) {
			SetErrorString("Create D3D viewport failed.\n%s", ErrorToString(rval));
			return FALSE;
		}
		/*
		* Add the viewport to the D3D device
		*/
		rval = lpD3DDevice2->AddViewport(lpD3DViewport2);
		if (rval != D3D_OK) {
			SetErrorString("Add D3D viewport failed.\n%s", ErrorToString(rval));
			return FALSE;
		}

		rval = lpD3DViewport2->QueryInterface(IID_IDirect3DViewport, (LPVOID*)&lpD3DViewport);
		if (rval != D3D_OK) {
			SetErrorString("Query IID_IDirect3DViewport failed.\n%s", ErrorToString(rval));
			return FALSE;
		}
	}

	if (lpD3DViewport == NULL) { // dx3
    /*
     * Create the D3D viewport object
     */
    rval = lpD3D->CreateViewport(&lpD3DViewport, NULL);
    if (rval != D3D_OK) {
        SetErrorString("Create D3D viewport failed.\n%s", ErrorToString(rval));
        //CleanUpAndPostQuit(); // HG 
        return FALSE;
    }
    /*
     * Add the viewport to the D3D device
     */
    rval = lpD3DDevice->AddViewport(lpD3DViewport);
    if (rval != D3D_OK) {
        SetErrorString("Add D3D viewport failed.\n%s", ErrorToString(rval));
        // HG CleanUpAndPostQuit();
        return FALSE;
    }

	} // dx3

    /*
     * Setup the viewport for a reasonable viewing area
     */
	if (lpD3DViewport3) { // DX 6
		D3DVIEWPORT2 viewData;
	    memset(&viewData, 0, sizeof(viewData));
		viewData.dwSize = sizeof(viewData);

		viewData.dwX = viewData.dwY = 0;

		viewData.dwWidth  = max(1,w);
		viewData.dwHeight = max(1,h);
		viewData.dvMinZ   = 0.0f;
		viewData.dvMaxZ   = 1.0f;

		viewData.dvClipX      = -1.0f;
		viewData.dvClipWidth  = 2.0f;
		viewData.dvClipY      = 1.0f;
		viewData.dvClipHeight = 2.0f;
	    rval = lpD3DViewport3->SetViewport2(&viewData);

	} else {
		D3DVIEWPORT viewData;
		memset(&viewData, 0, sizeof(viewData));
		viewData.dwSize = sizeof(viewData);
		viewData.dwX = viewData.dwY = 0;
		viewData.dwWidth = max(1,w);
		viewData.dwHeight = max(1,h);
		viewData.dvScaleX = viewData.dwWidth / (float)2.0;
		viewData.dvScaleY = viewData.dwHeight / (float)2.0;
		viewData.dvMaxX = (float)D3DDivide(D3DVAL(viewData.dwWidth),
                                       D3DVAL(2 * viewData.dvScaleX));
		viewData.dvMaxY = (float)D3DDivide(D3DVAL(viewData.dwHeight),
                                       D3DVAL(2 * viewData.dvScaleY));

		viewData.dvMinZ = 0.0f;	// hg 
		viewData.dvMaxZ = 1.0f;	// hg 
		rval = lpD3DViewport->SetViewport(&viewData);
	}
	

    if (rval != D3D_OK) {
        SetErrorString("SetViewport failed.\n%s", ErrorToString(rval));
        //CleanUpAndPostQuit();
        return FALSE;
    }

    // init viewport to whole area
	SetRect( &rcViewportRect, 0, 0, w, h);

	D3DVIEWPORT viewData;
	memset(&viewData, 0, sizeof(viewData));
	viewData.dwSize = sizeof(viewData);

    // hg 
	if (lpD3DViewport3)
	rval = lpD3DViewport3->GetViewport(&viewData);
	else if (lpD3DViewport2)
	rval = lpD3DViewport2->GetViewport(&viewData);
	else rval = lpD3DViewport->GetViewport(&viewData);

    if (rval != D3D_OK) {
        SetErrorString("GetViewport failed.\n%s", ErrorToString(rval));
        // CleanUpAndPostQuit();
        return FALSE;
    }

	if (lpD3DDevice3 && lpD3DViewport3)
		rval = lpD3DDevice3->SetCurrentViewport(lpD3DViewport3);
	else
	if (lpD3DDevice2 && lpD3DViewport2)
		rval = lpD3DDevice2->SetCurrentViewport(lpD3DViewport2);
	else rval = D3D_OK;

	if (rval != D3D_OK) {
		SetErrorString("SetCurrentViewport failed.\n%s", ErrorToString(rval));
		return FALSE;
	}

	return TRUE;

}

void 
GD3Device::ReleaseViewport() 
{
    /*
     * Since we created the viewport it is our responsibility to release
     * it.  Use D3DApp's pointer to it since we didn't save one.
     */
    RELEASE(lpD3DViewport);
    RELEASE(lpD3DViewport2);
    RELEASE(lpD3DViewport3);
}

// set current viewport rectangle
void GD3Device::SetViewport(int x,int y,int width, int height) 
{

     SetRect( &rcViewportRect, x, y, x+width, y+height);

	//glViewport(x,y,width,height);
    /*
     * Setup the viewport for a reasonable viewing area
     */
	if (lpD3DViewport3) { // DX 6
		D3DVIEWPORT2 viewData;
	    memset(&viewData, 0, sizeof(viewData));
		viewData.dwSize = sizeof(viewData);

		viewData.dwX = x; // top left
		viewData.dwY = y;

		viewData.dwWidth  = max(1,width);
		viewData.dwHeight = max(1,height);
		viewData.dvMinZ   = 0.0f;
		viewData.dvMaxZ   = 1.0f;

		viewData.dvClipX      = -1.0f;
		viewData.dvClipWidth  = 2.0f;
		viewData.dvClipY      = 1.0f;
		viewData.dvClipHeight = 2.0f;
	    LastError = lpD3DViewport3->SetViewport2(&viewData);

	} else {
		D3DVIEWPORT viewData;
		memset(&viewData, 0, sizeof(viewData));
		viewData.dwSize = sizeof(viewData);

		viewData.dwX = x; // top left
		viewData.dwY = y;

		viewData.dwWidth = max(1,width);
		viewData.dwHeight = max(1,height);
		viewData.dvScaleX = viewData.dwWidth / (float)2.0;
		viewData.dvScaleY =viewData.dwHeight / (float)2.0;
		viewData.dvMaxX = (float)D3DDivide(D3DVAL(viewData.dwWidth),
                                       D3DVAL(2 * viewData.dvScaleX));
		viewData.dvMaxY = (float)D3DDivide(D3DVAL(viewData.dwHeight),
                                       D3DVAL(2 * viewData.dvScaleY));

		viewData.dvMinZ = 0.0f;	// hg 
		viewData.dvMaxZ = 1.0f;	// hg 
		LastError = lpD3DViewport->SetViewport(&viewData);
	}
	

    if (LastError != D3D_OK) {
        SetErrorString("SetViewport failed.\n%s", ErrorToString(LastError));
        //CleanUpAndPostQuit();
        //return FALSE;
    }

}

void GD3Device::SetViewport() 

{
	SetViewport(0,0,szClient.cx,szClient.cy);
	TRACE("GD3Device::SetViewport 0 0 %i %i\n", szClient.cx,szClient.cy);

}

 
// get current viewport
void GD3Device::GetViewport(int &x,int &y,int &width, int &height) 
{
	/*
		GLint v[4];
		glViewport(x,y,width,height);
		glGetIntegerv(GL_VIEWPORT,v);
		x=v[0],y=v[1],width=v[2],height=v[3];
	*/

    x=rcViewportRect.left;
	y=rcViewportRect.top;
	
	width=rcViewportRect.right - rcViewportRect.left;
	height=rcViewportRect.bottom - rcViewportRect.top;

}


// ddcalls.cpp

/***************************************************************************/
/*                         Direct Draw Creation                            */
/***************************************************************************/
/*
 * DDEnumCallback
 * Callback function used during enumeration of DirectDraw drivers.
 * During enumeration, if a 3D capable hardware device is found, it is 
 * created and *(LPDIRECTDRAW*)lpContext is set to it.  Otherwise, does
 * nothing.
 */
BOOL FAR PASCAL DDEnumCallback(GUID FAR* lpGUID, LPSTR lpDriverDesc,
                               LPSTR lpDriverName, LPVOID lpContext)
{
    LPDIRECTDRAW lpDD;
    DDCAPS DriverCaps, HELCaps;

	GD3Device *dev = (GD3Device *) lpContext;


    /*
     * Record the D3D driver's information
     */
    if (lpGUID) {
	    memcpy(&dev->Devices[dev->NumDevices].Guid, lpGUID, sizeof(GUID));
		dev->Devices[dev->NumDevices].bIsPrimary = FALSE;
	}
	else {
		dev->Devices[dev->NumDevices].bIsPrimary = TRUE;
		dev->PrimaryDevice=dev->NumDevices;
	}
    dev->Devices[dev->NumDevices].About = lpDriverDesc;
    dev->Devices[dev->NumDevices].Name =  lpDriverName;
	dev->NumDevices++;

#if 0
    /*
     * A NULL GUID* indicates the DirectDraw HEL which we are not interested
     * in at the moment.
     */
    if (lpGUID) {
        /*
         * Create the DirectDraw device using this driver.  If it fails,
         * just move on to the next driver.
         */
        if (FAILED(DirectDrawCreate(lpGUID, &lpDD, NULL))) {
            return DDENUMRET_OK;
        }
        /*
         * Get the capabilities of this DirectDraw driver.  If it fails,
         * just move on to the next driver.
         */
        memset(&DriverCaps, 0, sizeof(DDCAPS));
        DriverCaps.dwSize = sizeof(DDCAPS);
        memset(&HELCaps, 0, sizeof(DDCAPS));
        HELCaps.dwSize = sizeof(DDCAPS);
        if (FAILED(lpDD->GetCaps(&DriverCaps, &HELCaps))) {
            lpDD->Release();
            return DDENUMRET_OK;
        }
        if (DriverCaps.dwCaps & DDCAPS_3D) {
            /*
             * We have found a 3d hardware device.  Return the DD object
             * and stop enumeration.
             */
            dev->bIsPrimary = FALSE;
            // *(LPDIRECTDRAW*)lpContext = lpDD;
			dev->lpDD = lpDD;
            return DDENUMRET_CANCEL;
        }       
        lpDD->Release();
    }
#endif

    return DDENUMRET_OK;
}

/*
 * CreateDD
 * Creates the DirectDraw device and saves the current palette. If a 3D 
 * capable DD driver is available, use it as the DD device, otherwise, use
 * the HEL.  It is assumed that a 3D capable DD hardware driver is not the
 * primary device and hence cannot operate in a window (ie it's a fullscreen
 * only device displaying on a second monitor).  Valid flags:
 *     D3DAPP_ONLYDDEMULATION    Always use the DirectDraw HEL
 */
BOOL
GD3Device::CreateDD(DWORD flags)
{
    HDC hdc;
    int i;

	ASSERT(lpDD == NULL);
	PrimaryDevice=-1;
	FullScreenDevice=-1;
	NumDevices = 0;


    /*
     * If we aren't forced to use the DirectDraw HEL, search for a 3D capable
     * DirectDraw hardware driver and create it.
     */
//    if (!(flags & D3DAPP_ONLYDDEMULATION)) {
        LastError = DirectDrawEnumerate(DDEnumCallback, this);
        if (LastError != DD_OK) {
            SetErrorString("DirectDrawEnumerate failed.\n%s",
                                  ErrorToString(LastError));
            return FALSE;
        }
  //  }

	if (flags & D3DAPP_ONLYWINDOW)
		CurrDevice = PrimaryDevice;
	else
		CurrDevice = PrimaryDevice;

    if (!lpDD) {
        /*
         * If we haven't created a hardware DD device by now, resort to HEL
         */
		if (Devices[CurrDevice].bIsPrimary) {
			bIsPrimary = TRUE;
			LastError = DirectDrawCreate(NULL, &lpDD, NULL);
		} else {
			bIsPrimary = Devices[CurrDevice].bIsPrimary;
			LastError = DirectDrawCreate(&Devices[CurrDevice].Guid, &lpDD, NULL);
		}
        if (LastError != DD_OK) {
            SetErrorString("DirectDrawCreate failed.\n%s",
                                  ErrorToString(LastError));
            return FALSE;
        }
    } else {
        //lpDD = lpDD;
    }

	//check for DDRAW 4 
    if( FAILED( lpDD->QueryInterface( IID_IDirectDraw4, (LPVOID*)&lpDD4)))
    {
		lpDD4= NULL;
    }

	memset(&deviceId, 0, sizeof(deviceId));

    if (lpDD4) {


		lpDD4->GetDeviceIdentifier(&deviceId,0);

		TRACE("Driver:%s\n",deviceId.szDriver);
		TRACE("Description:%s\n",deviceId.szDescription);

		TRACE("VendorId:%d %d\n",deviceId.dwVendorId,deviceId.dwDeviceId);

		// G200:Matrox Millennium G200 AGP
		// vendorId = 4139  dwDeviceId 1313

		// Intel(R) Express 3D Graphics Card
		// vendorId:32902 30720

		// Driver:G400DD32.DLL
		// Description:Matrox Millennium G400 DualHead - English
		// VendorId:4139 1317


	}

    /*
     * Save the original palette for when we are paused.  Just in case we
     * start in a fullscreen mode, put them in ppe.
     */
    hdc = GetDC(NULL);
    GetSystemPaletteEntries(hdc, 0, (1 << 8),
                            (LPPALETTEENTRY)(&Originalppe[0]));
    for (i = 0; i < 256; i++)
        ppe[i] = Originalppe[i];
    ReleaseDC(NULL, hdc);
    return TRUE;
}

/*
 * ChangeDevice 
 * Changes the  DD device.  If the current display mode is incompatible with the
 * driver, a new one will be selected and employed.  A new texture format is
 * selected and textures are reloaded, hence their handles may change.  By
 * default, paletted formats are prefered.
 */
BOOL GD3Device::ChangeDevice(int device, DWORD flags)
{
	int driver = D3DAPP_YOUDECIDE;
    int mode = D3DAPP_YOUDECIDE;
	
	coopLevelWasSet=FALSE;
	
	if (flags & D3DAPP_ONLYWINDOW) 
		mode = D3DAPP_USEWINDOW;



	int w,h;

    /*
     * Destroys all objects including Direct Draw.
     */
    bRenderingIsOK = FALSE;

    ATTEMPT(CallDeviceDestroyCallback());

    ReleaseAllTextures();
	ReleaseDevice();

    if (bFullscreen) {
        RestoreDispMode();
        SetCoopLevel(hwnd, FALSE);
    }

    RELEASE(lpD3D);
    RELEASE(lpD3D2);
    RELEASE(lpD3D3);

    RELEASE(lpDD);
    RELEASE(lpDD4);
	

	if (bInitialized)
		Destroy();

	bInitialized = FALSE;

	/*
     * create new DD device 
     */
	ASSERT(lpDD == NULL);

		if (Devices[device].bIsPrimary) {
			bIsPrimary = TRUE;
			LastError = DirectDrawCreate(NULL, &lpDD, NULL);
		} else {
			bIsPrimary = Devices[device].bIsPrimary;
			LastError = DirectDrawCreate(&Devices[device].Guid, &lpDD, NULL);
		}
        if (LastError != DD_OK) {
            SetErrorString("ChangeDevice::DirectDrawCreate failed.\n%s",
                                  ErrorToString(LastError));
            return FALSE;
        }

		CurrDevice = device;

	    ATTEMPT(EnumDisplayModes());
		// from CreateFromHWND
    /*
     * Create Direct3D and enumerate the D3D drivers
     */
    ATTEMPT(CreateD3D());
    ATTEMPT(EnumDrivers());
    

    ATTEMPT(VerifyDriverAndMode(&driver, &mode));

	if (mode == D3DAPP_USEWINDOW) {
        w = szClient.cx;
        h = szClient.cy;
        ATTEMPT(SetCoopLevel(hwndFrame, FALSE));
        ATTEMPT(CreateBuffers(hwnd, w, h, D3DAPP_BOGUS, FALSE));

    } else {
        szLastClient = szClient;
        w = Mode[mode].w;
        h = Mode[mode].h;
        szClient.cx = w; szClient.cy = h;
        ATTEMPT(SetCoopLevel(hwndFrame, TRUE));
        ATTEMPT(SetDisplayMode(w, h, Mode[mode].bpp));
        CurrMode = mode;
        ATTEMPT(CreateBuffers(hwnd, w, h, Mode[mode].bpp, TRUE));
    }
    /*
     * If the front buffer is palettized, initialize its palette
     */
    ATTEMPT(CheckForPalettized());
    /*
     * Create the Z-buffer
     */
    ATTEMPT(CreateZBuffer(w, h, driver));
    /*
     * Create the D3D device, load the textures, call the device create
     * callback and set a default render state
     */
    ATTEMPT(CreateDevice(driver));
    ATTEMPT(LoadAllTextures());
    ATTEMPT(FilterDisplayModes(driver));  /* bThisDriverCanDo flags */
    ATTEMPT(CallDeviceCreateCallback(w, h));
#if 0
    ATTEMPT(SetRenderState());
#endif

    /* 
     * Initialize dirty rectangle information
     */
    ValidateDirtyRects();
    /*
     * Ready to render
     */
    bInitialized = TRUE;
    bRenderingIsOK = TRUE;
    return TRUE;

exit_with_error:
    CallDeviceDestroyCallback();
	ReleaseDevice();

    if (bFullscreen) {
        RestoreDispMode();
        SetCoopLevel(hwnd, FALSE);
    }

    RELEASE(lpD3D);
    RELEASE(lpD3D2);
    RELEASE(lpD3D3);

    RELEASE(lpDD);
    RELEASE(lpDD4);

    return FALSE;



}


/***************************************************************************/
/*                   Enumerating the display modes                         */
/***************************************************************************/
/*
 * EnumDisplayModesCallback
 * Callback to save the display mode information.
 */
static HRESULT
CALLBACK EnumDisplayModesCallback(LPDDSURFACEDESC pddsd, LPVOID lpContext)
{

	GD3Device *dev = (GD3Device *) lpContext;

#if 0
    /*
     * Very large resolutions cause problems on some hardware.  They are also
     * not very useful for real-time rendering.  We have chosen to disable
     * them by not reporting them as available.
     */
    if (pddsd->dwWidth > 1024 || pddsd->dwHeight > 768)
        return DDENUMRET_OK;
#endif 

    /*
     * Save this mode at the end of the mode array and increment mode count
     */
    dev->Mode[dev->NumModes].w = pddsd->dwWidth;
    dev->Mode[dev->NumModes].h = pddsd->dwHeight;
    dev->Mode[dev->NumModes].bpp = pddsd->ddpfPixelFormat.dwRGBBitCount;
    dev->Mode[dev->NumModes].bThisDriverCanDo = FALSE;
    dev->NumModes++;
    if (dev->NumModes == D3DAPP_MAXMODES)
        return DDENUMRET_CANCEL;
    else
        return DDENUMRET_OK;
}

/*
 * CompareModes
 * Compare two display modes during sorting.  Modes are sorted by depth and
 * then resolution.
 */
static int
G__cdecl CompareModes(const void* element1, const void* element2) {
    D3DAppMode *lpMode1, *lpMode2;

    lpMode1 = (D3DAppMode*)element1;
    lpMode2 = (D3DAppMode*)element2;

    if (lpMode1->bpp > lpMode2->bpp)
        return -1;
    else if (lpMode2->bpp > lpMode1->bpp)
        return 1;
    else if (lpMode1->w > lpMode2->w)
        return -1;
    else if (lpMode2->w > lpMode1->w)
        return 1;
    else if (lpMode1->h > lpMode2->h)
        return -1;
    else if (lpMode2->h > lpMode1->h)
        return 1;
    else
        return 0;
}

/*
 * EnumerateDisplayModes
 * Generates the list of available display modes.
 */
BOOL
GD3Device::EnumDisplayModes()
{
    int i;
    /*
     * Get a list of available display modes from DirectDraw
     */
    NumModes = 0;
    LastError = lpDD->EnumDisplayModes(0, NULL, this, EnumDisplayModesCallback);
    if(LastError != DD_OK ) {
        SetErrorString("EnumDisplayModes failed.\n%s", ErrorToString(LastError));
        NumModes = 0;
        return FALSE;
    }

/*
	// modex tests 
    Mode[NumModes].w = 320;
    Mode[NumModes].h = 200;
    Mode[NumModes].bpp = 8;
    Mode[NumModes].bThisDriverCanDo = FALSE;
    NumModes++;
    Mode[NumModes].w = 320;
    Mode[NumModes].h = 240;
    Mode[NumModes].bpp = 8;
    Mode[NumModes].bThisDriverCanDo = FALSE;
    NumModes++;
*/

    /*
     * Sort the list of display modes
     */
    qsort((void *)&Mode[0], (size_t)NumModes, sizeof(D3DAppMode),CompareModes);
    /*
     * Pick a default display mode.  640x480x16 is a very good mode for
     * rendering, so choose it over all others.  Otherwise, just take the
     * first one.  This selection may be overriden later if a driver is
     * created which cannot render in this mode.
     */
    CurrMode = 0;
    for (i = 0; i < NumModes; i++) {
        if (Mode[i].w == 640 && Mode[i].h == 480 &&
            Mode[i].bpp == 16)
            CurrMode = i;
    }
    //memcpy(&ThisMode, &Mode[CurrMode], sizeof(D3DAppMode));
	ThisMode = Mode[CurrMode];
    return TRUE;
}

/***************************************************************************/
/*               Creating Front and Back Buffers (and misc surf funcs)     */
/***************************************************************************/
/*
 * CreateSurface
 * Create a DirectDraw Surface of the given description.  Using this function
 * ensures that all surfaces end up in system memory if that option was set.
 * Returns the result of the CreateSurface call.
 */
HRESULT
GD3Device::CreateSurface(LPDDSURFACEDESC lpDDSurfDesc,
                LPDIRECTDRAWSURFACE FAR *lpDDSurface) 
{
    HRESULT result;
    if (bOnlySystemMemory)
        lpDDSurfDesc->ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
    result = lpDD->CreateSurface(lpDDSurfDesc, lpDDSurface, NULL);
    return result;
}

/*
 * GetSurfDesc
 * Get the description of the given surface.  Returns the result of the
 * GetSurfaceDesc call.
 */
HRESULT
GD3Device::GetSurfDesc(LPDDSURFACEDESC lpDDSurfDesc,LPDIRECTDRAWSURFACE lpDDSurf)
{
    HRESULT result;
    memset(lpDDSurfDesc, 0, sizeof(DDSURFACEDESC));
    lpDDSurfDesc->dwSize = sizeof(DDSURFACEDESC);
    result = lpDDSurf->GetSurfaceDesc(lpDDSurfDesc);
    return result;
}

/*
 * CreateBuffers
 * Creates the front and back buffers for the window or fullscreen case
 * depending on the bFullscreen flag.  In the window case, bpp is ignored.
 */
BOOL
GD3Device::CreateBuffers(HWND hwnd, int w, int h, int bpp, BOOL bFullscreen)
{
    DDSURFACEDESC ddsd;
    DDSCAPS ddscaps;

    /*
     * Release any old objects that might be lying around.  This should have
     * already been taken care of, but just in case...
     */
    RELEASE(lpClipper);
	RELEASE(lpBackBuffer4);
    RELEASE(lpBackBuffer);

	RELEASE(lpFrontBuffer4);
    RELEASE(lpFrontBuffer);


    /*
     * The size of the buffers is going to be w x h, so record it now
     */
    if (w < D3DAPP_WINDOWMINIMUM)
        w = D3DAPP_WINDOWMINIMUM;
    if (h < D3DAPP_WINDOWMINIMUM)
        h = D3DAPP_WINDOWMINIMUM;
    szBuffers.cx = w;
    szBuffers.cy = h;

    if (bFullscreen) {
        /*
         * Create a complex flipping surface for fullscreen mode with one
         * back buffer.
         */
        memset(&ddsd,0,sizeof(DDSURFACEDESC));
        ddsd.dwSize = sizeof( ddsd );
        ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
        ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP |
            DDSCAPS_3DDEVICE | DDSCAPS_COMPLEX;
        ddsd.dwBackBufferCount = 1;
        LastError = CreateSurface(&ddsd, &lpFrontBuffer);
        if(LastError != DD_OK) {
            SetErrorString("CreateSurface for fullscreen flipping surface failed.\n%s",
                                  ErrorToString(LastError));
            goto exit_with_error;
        }
        /* 
         * Obtain a pointer to the back buffer surface created above so we
         * can use it later.  For now, just check to see if it ended up in
         * video memory (FYI).
         */
        ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
        LastError = lpFrontBuffer->GetAttachedSurface(&ddscaps, &lpBackBuffer);
        if(LastError != DD_OK) {
            SetErrorString("GetAttachedSurface failed to get back buffer.\n%s",
                                  ErrorToString(LastError));
            goto exit_with_error;
        }
        LastError = GetSurfDesc(&ddsd, lpBackBuffer);
        if (LastError != DD_OK) {
            SetErrorString("Failed to get surface description of back buffer.\n%s",
                                  ErrorToString(LastError));
            goto exit_with_error;
        }
        bBackBufferInVideo =
                  (ddsd.ddsCaps.dwCaps & DDSCAPS_VIDEOMEMORY) ? TRUE : FALSE;
    }
    else {


		if (lpDD4) {

	    DDSURFACEDESC2 ddsd;

        /*
         * In the window case, create a front buffer which is the primary
         * surface and a back buffer which is an offscreen plane surface.
         */
        memset(&ddsd,0,sizeof(ddsd));
        ddsd.dwSize = sizeof(ddsd);
        ddsd.dwFlags = DDSD_CAPS;
        ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
        /*
         * If we specify system memory when creating a primary surface, we
         * won't get the actual primary surface in video memory.  So, don't
         * use CreateSurface().
         */
        LastError = lpDD4->CreateSurface(&ddsd, &lpFrontBuffer4, NULL);

        if(LastError != DD_OK ) {
            SetErrorString("CreateSurface for window front buffer failed.\n%s",
                                  ErrorToString(LastError));
            goto exit_with_error;
        }
		LastError = lpFrontBuffer4->QueryInterface(IID_IDirectDrawSurface, (LPVOID*)&lpFrontBuffer);

        if(LastError != DD_OK ) {
            SetErrorString("CreateSurface for window front buffer QI failed.\n%s",
                                  ErrorToString(LastError));
            goto exit_with_error;
        }



        /*
         * Create the DirectDraw Clipper object and attach it to the window
         * and front buffer.
         */
        LastError = lpDD4->CreateClipper(0,&lpClipper, NULL);
        if(LastError != DD_OK ) {
            SetErrorString("CreateClipper failed.\n%s",
                                  ErrorToString(LastError));
            goto exit_with_error;
        }

        LastError = lpClipper->SetHWnd(0, hwnd);
        if(LastError != DD_OK ) {
            SetErrorString("Attaching clipper to window failed.\n%s",
                                  ErrorToString(LastError));
            goto exit_with_error;
        }
        LastError = lpFrontBuffer4->SetClipper(lpClipper);
        if(LastError != DD_OK ) {
            SetErrorString("Attaching clipper to front buffer failed.\n%s",
                                  ErrorToString(LastError));
            goto exit_with_error;
        }


        ddsd.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
        ddsd.dwWidth = w;
        ddsd.dwHeight = h;
        ddsd.ddsCaps.dwCaps= DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE;
		
		    
		if (bOnlySystemMemory)
			ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;        

		// ddsd.ddsCaps.dwCaps |= DDSCAPS_VIDEOMEMORY;
    
		// LastError = CreateSurface(&ddsd, &lpBackBuffer);

		LastError = lpDD4->CreateSurface( &ddsd, &lpBackBuffer4, NULL);
		
		//LastError=DDERR_OUTOFVIDEOMEMORY; // see what happens 
		
		HRESULT saveError = LastError;

		// LastError=DDERR_OUTOFVIDEOMEMORY

		// if failed retry in system memory hg 
		if (LastError != DD_OK && !bOnlySystemMemory) {
			ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
			// LastError = CreateSurface(&ddsd, &lpBackBuffer);
			LastError = lpDD4->CreateSurface( &ddsd, &lpBackBuffer4, NULL );
			// max swith onlySystemMeomry flag ??????
			if (LastError != DD_OK) LastError = saveError;
		}

        if (LastError != DD_OK) {
            SetErrorString("CreateSurface for window back buffer failed.\n%s",
                                  ErrorToString(LastError));
            goto exit_with_error;
        }

        /*
         * Check to see if the back buffer is in video memory (FYI).
         */
        // LastError = GetSurfDesc(&ddsd, lpBackBuffer4);

		memset(&ddsd, 0, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);
		LastError = lpBackBuffer4->GetSurfaceDesc(&ddsd);


        if (LastError != DD_OK) {
            SetErrorString("Failed to get surface description for back buffer.\n%s",
                                  ErrorToString(LastError));
            goto exit_with_error;
        }
        bBackBufferInVideo =
                  (ddsd.ddsCaps.dwCaps & DDSCAPS_VIDEOMEMORY) ? TRUE : FALSE;


		LastError = lpBackBuffer4->QueryInterface(IID_IDirectDrawSurface, (LPVOID*)&lpBackBuffer);
        if (LastError != DD_OK) {
            SetErrorString("Failed to QI for back buffer.\n%s",
                                  ErrorToString(LastError));
            goto exit_with_error;
        }

		} // lpDD4

		else { 
        /*
         * In the window case, create a front buffer which is the primary
         * surface and a back buffer which is an offscreen plane surface.
         */
        memset(&ddsd,0,sizeof(DDSURFACEDESC));
        ddsd.dwSize = sizeof(DDSURFACEDESC);
        ddsd.dwFlags = DDSD_CAPS;
        ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
        /*
         * If we specify system memory when creating a primary surface, we
         * won't get the actual primary surface in video memory.  So, don't
         * use CreateSurface().
         */
        LastError = lpDD->CreateSurface(&ddsd, &lpFrontBuffer, NULL);
        if(LastError != DD_OK ) {
            SetErrorString("CreateSurface for window front buffer failed.\n%s",
                                  ErrorToString(LastError));
            goto exit_with_error;
        }
        ddsd.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
        ddsd.dwWidth = w;
        ddsd.dwHeight = h;
        ddsd.ddsCaps.dwCaps= DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE;
        LastError = CreateSurface(&ddsd, &lpBackBuffer);

		HRESULT saveError = LastError;

		// if failed retry in system memory hg 
		if (LastError != DD_OK && !bOnlySystemMemory) {
			ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
			LastError = CreateSurface(&ddsd, &lpBackBuffer);
			// max swith onlySystemMeomry flag ??????
			if (LastError != DD_OK) LastError = saveError;

		}

        if (LastError != DD_OK) {
            SetErrorString("CreateSurface for window back buffer failed.\n%s",
                                  ErrorToString(LastError));
            goto exit_with_error;
        }
        /*
         * Check to see if the back buffer is in video memory (FYI).
         */
        LastError = GetSurfDesc(&ddsd, lpBackBuffer);
        if (LastError != DD_OK) {
            SetErrorString("Failed to get surface description for back buffer.\n%s",
                                  ErrorToString(LastError));
            goto exit_with_error;
        }
        bBackBufferInVideo =
                  (ddsd.ddsCaps.dwCaps & DDSCAPS_VIDEOMEMORY) ? TRUE : FALSE;
        /*
         * Create the DirectDraw Clipper object and attach it to the window
         * and front buffer.
         */
        LastError = lpDD->CreateClipper(0,&lpClipper, NULL);
        if(LastError != DD_OK ) {
            SetErrorString("CreateClipper failed.\n%s",
                                  ErrorToString(LastError));
            goto exit_with_error;
        }
        LastError = lpClipper->SetHWnd(0, hwnd);
        if(LastError != DD_OK ) {
            SetErrorString("Attaching clipper to window failed.\n%s",
                                  ErrorToString(LastError));
            goto exit_with_error;
        }
        LastError = lpFrontBuffer->SetClipper(lpClipper);
        if(LastError != DD_OK ) {
            SetErrorString("Attaching clipper to front buffer failed.\n%s",
                                  ErrorToString(LastError));
            goto exit_with_error;
        }
    }
	
	} // window case


    ClearBuffers();
    return TRUE;

exit_with_error:

    RELEASE(lpFrontBuffer);
    RELEASE(lpBackBuffer);

	RELEASE(lpFrontBuffer4);
	RELEASE(lpBackBuffer4);


    RELEASE(lpClipper);
    return FALSE;
}

/*
 * CheckForPalettized
 * If the front/back buffer is palettized, we need to create a palette.
 */
BOOL
GD3Device::CheckForPalettized()
{
    DDSURFACEDESC ddsd;
    /*
     * Get the back buffer surface description and check to see if it's
     * palettized
     */
    LastError = GetSurfDesc(&ddsd, lpBackBuffer);
    if (LastError != DD_OK) {
        SetErrorString("Failed to get surface description for back buffer for palettizing.\n%s",
                              ErrorToString(LastError));
        goto exit_with_error;
    }
    bPrimaryPalettized = 
        (ddsd.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8) ? TRUE : FALSE;

    if (bPrimaryPalettized) {
        int i;
        /*
         * Get the current palette.
         */
        HDC hdc = GetDC(NULL);
        GetSystemPaletteEntries(hdc, 0, (1 << 8), ppe);
        ReleaseDC(NULL, hdc);
        /*
         * Change the flags on the palette entries to allow D3D to change
         * some of them.  In the window case, we must not change the top and
         * bottom ten (system colors), but in a fullscreen mode we can have
         * all but the first and last.
         */
        if (!bFullscreen) {
            for (i = 0; i < 10; i++) ppe[i].peFlags = D3DPAL_READONLY;
#ifndef _GLVIEW_IE
            // 2.07. D3PAL_FREE from ATL D3D Example 
			for (i = 10; i < 256 - 10; i++) ppe[i].peFlags = D3DPAL_FREE | PC_RESERVED;
#else
			// 2.07. use static palette 		
			for (i = 10; i < 256 - 10; i++) ppe[i].peFlags = D3DPAL_READONLY;
#endif

            for (i = 256 - 10; i < 256; i++) ppe[i].peFlags = D3DPAL_READONLY;
        } else {
            ppe[0].peFlags = D3DPAL_READONLY;
            for (i = 1; i < 255; i++) ppe[i].peFlags = PC_RESERVED;
            ppe[255].peFlags = D3DPAL_READONLY;
        }
        /*
         * Create a palette using the old colors and new flags
         */
        LastError = lpDD->CreatePalette(DDPCAPS_8BIT | DDPCAPS_INITIALIZE,
                                           ppe, &lpPalette, NULL);
        if (LastError != DD_OK) {
            SetErrorString("CreatePalette failed.\n%s",
                                  ErrorToString(LastError));
            goto exit_with_error;
        }
        /*
         * Set this as the front and back buffers' palette
         */
        LastError = lpBackBuffer->SetPalette(lpPalette);
        if(LastError != DD_OK ) {
            SetErrorString("SetPalette failed on back buffer.\n%s",
                                  ErrorToString(LastError));
            goto exit_with_error;
        }
        LastError = lpFrontBuffer->SetPalette(lpPalette);
        if(LastError != DD_OK ) {
            SetErrorString("SetPalette failed on front buffer.\n%s",
                                  ErrorToString(LastError));
            goto exit_with_error;
        }
        /*
         * The palette is now valid, so set it again on anyt WM_ACTIVATE
         */
        bPaletteActivate = TRUE;
    }
    return TRUE;
exit_with_error:
    RELEASE(lpPalette);
    return FALSE;
}

/*
 * Set the front buffer's palette
 */

BOOL
GD3Device::SetPalette()
{
     if (bPaletteActivate && bPrimaryPalettized && lpFrontBuffer) {
         lpFrontBuffer->SetPalette(lpPalette);
     }
	 return (TRUE);
}

/***************************************************************************/
/*                           Creation of Z-Buffer                          */
/***************************************************************************/
/*
 * CreateZBuffer
 * Create a Z-Buffer of the appropriate depth and attach it to the back
 * buffer.
 */
BOOL
GD3Device::CreateZBuffer(int w, int h, int driver)
{
    DDSURFACEDESC ddsd;
    DWORD devDepth;
    /*
     * Release any Z-Buffer that might be around just in case.
     */
    RELEASE(lpZBuffer);
    
    /*
     * If this driver does not do z-buffering, don't create a z-buffer
     */
    if (!Driver[driver].bDoesZBuffer)
        return TRUE;

    memset(&ddsd, 0 ,sizeof(ddsd));
    ddsd.dwSize = sizeof( ddsd );
    ddsd.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS |  DDSD_ZBUFFERBITDEPTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_ZBUFFER;
	ddsd.ddpfPixelFormat.dwSize = sizeof(ddsd.ddpfPixelFormat);
	//ddsd.ddsCaps.dwSize = sizeof(ddsd.ddsCaps);

    /*
     *  HG CreateSurface fails if w and h is zero 
     */
    if (w < D3DAPP_WINDOWMINIMUM)
        w = D3DAPP_WINDOWMINIMUM;
    if (h < D3DAPP_WINDOWMINIMUM)
        h = D3DAPP_WINDOWMINIMUM;

    ddsd.dwHeight = h;
    ddsd.dwWidth = w;
    /*
     * If this is a hardware D3D driver, the Z-Buffer MUST end up in video
     * memory.  Otherwise, it MUST end up in system memory.
     */
    if (Driver[driver].bIsHardware)
        ddsd.ddsCaps.dwCaps |= DDSCAPS_VIDEOMEMORY;
    else
        ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
    /*
     * Get the Z buffer bit depth from this driver's D3D device description
     */
    devDepth = Driver[driver].Desc.dwDeviceZBufferBitDepth;
    if (devDepth & DDBD_32)
        ddsd.dwZBufferBitDepth = 32;
    else if (devDepth & DDBD_24)
        ddsd.dwZBufferBitDepth = 24;
    else if (devDepth & DDBD_16)
        ddsd.dwZBufferBitDepth = 16;
    else if (devDepth & DDBD_8)
        ddsd.dwZBufferBitDepth = 8;
    else {
        SetErrorString("Unsupported Z-buffer depth requested by device.\n");
        return FALSE;
    }

	if (0 && lpDD4) {

		DDSURFACEDESC2 ddsd2;
	    memset(&ddsd2, 0 ,sizeof(ddsd2));
		ddsd2.dwSize = sizeof( ddsd2 );
		ddsd2.dwFlags = ddsd.dwFlags;
		ddsd2.ddsCaps.dwCaps = ddsd.ddsCaps.dwCaps; 
	    ddsd2.dwHeight= ddsd.dwHeight;
		ddsd2.dwWidth = ddsd.dwWidth;
        // ddsd2.dwZBufferBitDepth = ddsd.dwZBufferBitDepth;
	    ddsd2.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS; //  |  DDSD_ZBUFFERBITDEPTH;
        ddsd2.ddsCaps.dwCaps |= DDSCAPS_VIDEOMEMORY;

	    LastError = lpDD4->CreateSurface(&ddsd2,&lpZBuffer4,NULL);
		if(LastError == DD_OK )
			LastError = lpZBuffer4->QueryInterface(IID_IDirectDrawSurface, (LPVOID*)&lpZBuffer);


	} else 
		LastError = lpDD->CreateSurface(&ddsd,&lpZBuffer,NULL);

    if(LastError != DD_OK) {
        SetErrorString("CreateSurface for Z-buffer failed.\n%s",
                              ErrorToString(LastError));
        goto exit_with_error;
    }
    /*
     * Attach the Z-buffer to the back buffer so D3D will find it
     */
	if (lpBackBuffer4 && lpZBuffer4) 
	LastError = lpBackBuffer4->AddAttachedSurface(lpZBuffer4); 
    else LastError = lpBackBuffer->AddAttachedSurface(lpZBuffer); 

    if(LastError != DD_OK) {
        SetErrorString("AddAttachedBuffer failed for Z-Buffer.\n%s",
                              ErrorToString(LastError));
        goto exit_with_error;
    }
    /*
     * Find out if it ended up in video memory.
     */
    LastError = GetSurfDesc(&ddsd, lpZBuffer);
    if (LastError != DD_OK) {
        SetErrorString("Failed to get surface description of Z buffer.\n%s",
                              ErrorToString(LastError));
        goto exit_with_error;
    }
    bZBufferInVideo = (ddsd.ddsCaps.dwCaps & DDSCAPS_VIDEOMEMORY) ? TRUE : FALSE;
    if (Driver[driver].bIsHardware && !bZBufferInVideo) {
        SetErrorString("Could not fit the Z-buffer in video memory for this hardware device.\n");
        goto exit_with_error;
    }

    return TRUE;

exit_with_error:
    RELEASE(lpZBuffer);
    return FALSE;
}


/***************************************************************************/
/*                             WM_SIZE Handler                             */
/***************************************************************************/
/*
 * HandleWM_SIZE
 * Processes the WM_SIZE message.  Resizes all the buffers and re-creates
 * device if necessary.
 */
BOOL
GD3Device::HandleWM_SIZE(HWND hwnd, WPARAM wParam, int w, int h)
{
    int i;
	BOOL someLost;
    /*
     * If we have minimzied, take note and call the default window proc
     */
    if (wParam == SIZE_MINIMIZED) {
        bMinimized = TRUE;
        //*lresult = DefWindowProc(hwnd, message, wParam, lParam);
        return TRUE;
    }
    /*
     * In fullscreen mode, restore our surfaces and let DDraw take
     * care of the rest.
     */
    if (bFullscreen) {
        ValidateDirtyRects();
        CheckForLostSurfaces(someLost);
        bMinimized = FALSE;
        //*lresult = DefWindowProc(hwnd, message, wParam, lParam);
        return TRUE;
    }
    /*
     * If we are minimized, this is the un-minimized size message.
     */
    if (bMinimized) {
        /*
         * Restore our surfaces and update the dirty rectangle info
         */
        ValidateDirtyRects();
        CheckForLostSurfaces(someLost);
        bMinimized = FALSE;
        //*lresult = DefWindowProc(hwnd, message, wParam, lParam);
        return TRUE;
    }
    /*
     * Since we are still here, this must be a regular, window resize
     * message.  A new viewport will definitely be needed, but the
     * device and buffers will only be re-created if they have gotten bigger
     * or change size by a very large amount.
     */
    GetClientWin(hwnd);
    /*
     * If w and h are under the minimum, create buffers of the minimum size
     */
    if (w < D3DAPP_WINDOWMINIMUM)
        w = D3DAPP_WINDOWMINIMUM;
    if (h < D3DAPP_WINDOWMINIMUM)
        h = D3DAPP_WINDOWMINIMUM;
    /*
     * Destroy the viewport and all execute buffers
     */
    bRenderingIsOK = FALSE;
    ATTEMPT(CallDeviceDestroyCallback());
    /*
     * Only create a new device and buffers if they changed significantly,
     * otherwise just make sure the old buffers aren't lost.
     */
    if ((w > szBuffers.cx || h > szBuffers.cy) ||
        (w < szBuffers.cx / 2 || h < szBuffers.cy / 2)) {
        /*
         * Release the device
         */
		ReleaseDevice();
        /*
         * Create new ones
         */
        ATTEMPT(CreateBuffers(hwnd, w, h, D3DAPP_BOGUS, FALSE));
        ATTEMPT(CheckForPalettized());
        ATTEMPT(CreateZBuffer(w, h, CurrDriver));
        /*
         * Create the driver
         */
        ATTEMPT(CreateDevice(CurrDriver));
#if 0
        /*
         * Since the driver did not change, the texture surfaces are still valid.
         * We just need to get new handles.
         */
        if (ThisDriver.bDoesTextures) {
            for (i = 0; i < NumTextures; i++) {
                GetTextureHandle(i);
            }
        }
#endif

    } else {
        CheckForLostSurfaces(someLost);
    }
    /*
     * Call the device create callback to create the viewport, set the render
     * state and clear the dirty rectangle info
     */
    ATTEMPT(CallDeviceCreateCallback(w, h));
#if 0
	// HG state is managed in RenderState.Init()
    ATTEMPT(SetRenderState());
#endif
    ValidateDirtyRects();
    bRenderingIsOK = TRUE;
    /*
     * Call the default window proc
     */
//    *lresult = DefWindowProc(hwnd, message, wParam, lParam);
    return TRUE;

exit_with_error:
    CallDeviceDestroyCallback();
	ReleaseDevice();

    return FALSE;
}

/***************************************************************************/
/*                             WM_SIZE Handler                             */
/***************************************************************************/

#if 0
/*
 * HandleWM_SIZE
 * Processes the WM_SIZE message.  Resizes all the buffers and re-creates
 * device if necessary.
 */
BOOL
GD3Device::HandleWM_SIZE(LRESULT* lresult, HWND hwnd, UINT message,
                     WPARAM wParam, LPARAM lParam)
{
    int w, h, i;
    /*
     * If we have minimzied, take note and call the default window proc
     */
    if (wParam == SIZE_MINIMIZED) {
        bMinimized = TRUE;
        *lresult = DefWindowProc(hwnd, message, wParam, lParam);
        return TRUE;
    }
    /*
     * In fullscreen mode, restore our surfaces and let DDraw take
     * care of the rest.
     */
    if (bFullscreen) {
        ValidateDirtyRects();
        CheckForLostSurfaces();
        bMinimized = FALSE;
        *lresult = DefWindowProc(hwnd, message, wParam, lParam);
        return TRUE;
    }
    /*
     * If we are minimized, this is the un-minimized size message.
     */
    if (bMinimized) {
        /*
         * Restore our surfaces and update the dirty rectangle info
         */
        ValidateDirtyRects();
        CheckForLostSurfaces();
        bMinimized = FALSE;
        *lresult = DefWindowProc(hwnd, message, wParam, lParam);
        return TRUE;
    }
    /*
     * Since we are still here, this must be a regular, window resize
     * message.  A new viewport will definitely be needed, but the
     * device and buffers will only be re-created if they have gotten bigger
     * or change size by a very large amount.
     */
    GetClientWin(hwnd);
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    /*
     * If w and h are under the minimum, create buffers of the minimum size
     */
    if (w < D3DAPP_WINDOWMINIMUM)
        w = D3DAPP_WINDOWMINIMUM;
    if (h < D3DAPP_WINDOWMINIMUM)
        h = D3DAPP_WINDOWMINIMUM;
    /*
     * Destroy the viewport and all execute buffers
     */
    bRenderingIsOK = FALSE;
    ATTEMPT(CallDeviceDestroyCallback());
    /*
     * Only create a new device and buffers if they changed significantly,
     * otherwise just make sure the old buffers aren't lost.
     */
    if ((w > szBuffers.cx || h > szBuffers.cy) ||
        (w < szBuffers.cx / 2 || h < szBuffers.cy / 2)) {
        /*
         * Release the device
         */
		ReleaseDevice();

	    /*
         * Create new ones
         */
        ATTEMPT(CreateBuffers(hwnd, w, h, D3DAPP_BOGUS, FALSE));
        ATTEMPT(CheckForPalettized());
        ATTEMPT(CreateZBuffer(w, h, CurrDriver));
        /*
         * Create the driver
         */
        ATTEMPT(CreateDevice(CurrDriver));
        /*
         * Since the driver did not change, the texture surfaces are still valid.
         * We just need to get new handles.
         */
        if (ThisDriver.bDoesTextures) {
            for (i = 0; i < NumTextures; i++) {
                GetTextureHandle(i);
            }
        }
    } else {
        CheckForLostSurfaces();
    }
    /*
     * Call the device create callback to create the viewport, set the render
     * state and clear the dirty rectangle info
     */
    ATTEMPT(CallDeviceCreateCallback(w, h));
#if 0
    ATTEMPT(SetRenderState());
#endif 
    ValidateDirtyRects();
    bRenderingIsOK = TRUE;
    /*
     * Call the default window proc
     */
    *lresult = DefWindowProc(hwnd, message, wParam, lParam);
    return TRUE;

exit_with_error:
    CallDeviceDestroyCallback();
	ReleaseDevice();

    return FALSE;
}
#endif 


/***************************************************************************/
/*              Setting the display mode and cooperative level             */
/***************************************************************************/
/*
 * SetCoopLevel
 * Set the cooperative level to exclusive mode for fullscreen and normal for
 * a window.  Set the bIgnoreWM_SIZE flag because SetCooperativeLevel
 * generates a WM_SIZE message you do not have to resize the buffers on.
 */


BOOL
GD3Device::SetCoopLevel(HWND hwnd, BOOL bFullscreen)
{
	if (!lpDD) return FALSE;
    if (bFullscreen) 
	
	{
        bIgnoreWM_SIZE = TRUE;
									//DDSCL_ALLOWMODEX 
        LastError = lpDD->SetCooperativeLevel(hwnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN /*| DDSCL_ALLOWMODEX */);

		if (LastError == DDERR_EXCLUSIVEMODEALREADYSET || LastError == DDERR_HWNDALREADYSET)  {
			LastError = DD_OK;
			
			LastError = lpDD->SetCooperativeLevel(hwnd, DDSCL_NORMAL);
			LastError = lpDD->SetCooperativeLevel(hwnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN  );

			coopLevelWasSet = TRUE;
        }
		else coopLevelWasSet = FALSE;

	//	LastError = DD_OK;

        bIgnoreWM_SIZE = FALSE;
        
		if(LastError != DD_OK ) {
            SetErrorString("SetCooperativeLevel to fullscreen failed.\n%s",
                                  ErrorToString(LastError));
            return FALSE;
        }
        this->bFullscreen = TRUE;

	

    } 
	
	else 
	
	{
        bIgnoreWM_SIZE = TRUE;

		if (!coopLevelWasSet)
			LastError = lpDD->SetCooperativeLevel(hwnd, DDSCL_NORMAL);
		else 
			LastError = lpDD->SetCooperativeLevel(hwnd, DDSCL_EXCLUSIVE);

		//LastError = DD_OK;

        bIgnoreWM_SIZE = FALSE;

		/*
		if (this->bFullscreen ) {
			// we are returning, 
			::SetMenu(hwnd,lastFrameMenu);
			if (::SetWindowLong(hwnd,GWL_STYLE,lastFrameWinStyle) ==0) {
				TRACE("set window long  failed \n");
			}

		}
		*/
        this->bFullscreen = FALSE;

    }
    return TRUE;
}

/*
 * SetDisplayMode
 * Set the display mode to the given dimensions and bits per pixel.  The
 * bIgnoreWM_SIZE message is set because the display change generates a
 * WM_SIZE message which we don't want to resize the buffers on.
 */
BOOL
GD3Device::SetDisplayMode(int w, int h, int bpp)
{
    ThisMode.w = w; ThisMode.h = h;
    ThisMode.bpp = bpp;
    
	bIgnoreWM_SIZE = TRUE;

    LastError = lpDD->SetDisplayMode(w, h, bpp);
	
	TRACE("GD3Device::SetDisplayMode(w %d ,  h %d , bpp %d)==%d\n",w,h,bpp,LastError);
    
	bIgnoreWM_SIZE = FALSE;
    
	if(LastError != DD_OK ) {
        SetErrorString("SetDisplayMode to %dx%dx%d failed\n%s",
                              w, h, bpp, ErrorToString(LastError));
        return FALSE;
    }
    return TRUE;
}

/*
 * RestoreDispMode
 * Restores the display mode to the current windows display mode.  The
 * bIgnoreWM_SIZE message is set because the display change generates a
 * WM_SIZE message which we don't want to resize the buffers on.
 */
BOOL
GD3Device::RestoreDispMode()
{
    bIgnoreWM_SIZE = TRUE;
    LastError = lpDD->RestoreDisplayMode();
    if (FAILED(LastError)) {
        SetErrorString("RestoreDisplayMode failed.\n%s",ErrorToString(LastError));
        return FALSE;
    }
    bIgnoreWM_SIZE = FALSE;
    return TRUE;
}

/*
 * RememberWindowsMode
 * Record the current display mode in WindowsDisplay
 */
BOOL
GD3Device::RememberWindowsMode()
{
    DDSURFACEDESC ddsd;
	// what if not primary 
    memset(&ddsd, 0, sizeof(DDSURFACEDESC));
    ddsd.dwSize = sizeof(DDSURFACEDESC);
	
	if (!lpDD) 
		return FALSE;

    LastError = lpDD->GetDisplayMode(&ddsd);

    if (FAILED(LastError)) {
        SetErrorString("Getting the current display mode failed.\n%s",ErrorToString(LastError));
        return FALSE;
    }
    WindowsDisplay.w = ddsd.dwWidth;
    WindowsDisplay.h = ddsd.dwHeight;
    WindowsDisplay.bpp = ddsd.ddpfPixelFormat.dwRGBBitCount;
    return TRUE;
}

/***************************************************************************/
/*                          Misc DD Utilities                              */
/***************************************************************************/

/*
 * ClearBuffers
 * Clear the front and back buffers to black
 */
BOOL
GD3Device::ClearBuffers()
{
    DDSURFACEDESC ddsd;
    RECT dst;
    DDBLTFX ddbltfx;
    /*
     * Find the width and height of the front buffer by getting its
     * DDSURFACEDESC
     */
    if (lpFrontBuffer) {
        LastError = GetSurfDesc(&ddsd, lpFrontBuffer);
        if (FAILED(LastError)) {
            SetErrorString("Failure getting the surface description of the front buffer before clearing.\n%s",
                                  ErrorToString(LastError));
            return FALSE;
        }
        /*
         * Clear the front buffer to black
         */
        memset(&ddbltfx, 0, sizeof(ddbltfx));
        ddbltfx.dwSize = sizeof(DDBLTFX);
        SetRect(&dst, 0, 0, ddsd.dwWidth, ddsd.dwHeight);
        LastError = lpFrontBuffer->Blt(&dst, NULL, NULL, 
                                                    DDBLT_COLORFILL | DDBLT_WAIT,
                                                    &ddbltfx);
        if (FAILED(LastError)) {
            SetErrorString("Clearing the front buffer failed.\n%s",
                                  ErrorToString(LastError));
            return FALSE;
        }
    }
    if (lpBackBuffer) {
        /*
         * Find the width and height of the back buffer by getting its
         * DDSURFACEDESC
         */
        LastError = GetSurfDesc(&ddsd, lpBackBuffer);
        if (FAILED(LastError)) {
            SetErrorString("Failure while getting the surface description of the back buffer before clearing.\n%s",
                                  ErrorToString(LastError));
            return FALSE;
        }
        /*
         * Clear the back buffer to black
         */
        memset(&ddbltfx, 0, sizeof(ddbltfx));
        ddbltfx.dwSize = sizeof(DDBLTFX);
        SetRect(&dst, 0, 0, ddsd.dwWidth, ddsd.dwHeight);
        LastError = lpBackBuffer->Blt(  &dst,
                                                     NULL, NULL,
                                                     DDBLT_COLORFILL | DDBLT_WAIT,
                                                     &ddbltfx);
        if (FAILED(LastError)) {
            SetErrorString("Clearing the back buffer failed.\n%s",
                                  ErrorToString(LastError));
            return FALSE;
        }
    }
    return TRUE;
}

/*
 * BPPToDDBD
 * Convert an integer bit per pixel number to a DirectDraw bit depth flag
 */
DWORD
GD3Device::BPPToDDBD(int bpp)
{
    switch(bpp) {
        case 1:
            return DDBD_1;
        case 2:
            return DDBD_2;
        case 4:
            return DDBD_4;
        case 8:
            return DDBD_8;
        case 16:
            return DDBD_16;
        case 24:
            return DDBD_24;
        case 32:
            return DDBD_32;
        default:
            return (DWORD)D3DAPP_BOGUS;
    }
}

/*
 * TotalVideoMemory
 * Returns the amount of total video memory supported (not free)
 */
DWORD
GD3Device::TotalVideoMemory()
{
    DDCAPS DriverCaps, HELCaps;
	int capsSize = sizeof(DDCAPS_DX3);
	if (!lpDD) return 0;

    memset (&DriverCaps, 0, sizeof(DDCAPS));
    DriverCaps.dwSize = capsSize;
    memset (&HELCaps, 0, sizeof(DDCAPS));
    HELCaps.dwSize = capsSize;

    LastError = lpDD->GetCaps(&DriverCaps,&HELCaps);
    if (FAILED(LastError)) {
        SetErrorString("Getting DD capabilities failed while checking total video memory.\n%s",
                              ErrorToString(LastError));
        return 0L;
    }
    if (DriverCaps.dwVidMemTotal)
        return DriverCaps.dwVidMemTotal;
    else
        return HELCaps.dwVidMemTotal;
}


DWORD
GD3Device::GetTextureMemory(BOOL total)
{
	DDSCAPS2      ddsCaps2; 
	DWORD         dwTotal; 
	DWORD         dwFree;
 
 
	// Initialize the structure.
	ZeroMemory(&ddsCaps2, sizeof(ddsCaps2));

	if (lpDD4) {  // DX6 !!!!
		ddsCaps2.dwCaps = DDSCAPS_TEXTURE; 
		LastError = lpDD4->GetAvailableVidMem(&ddsCaps2, &dwTotal, &dwFree); 
		if (FAILED(LastError))
			return 0;

		if (total) return dwTotal;
		return dwFree;
	}
	if (total) return TotalVideoMemory();
	return FreeVideoMemory();

}
 

#include "gcaps.h"

int
GD3Device::GetCap(int what)
{

	if (what == GC_IS_HARDWARE) 
		return IsHardwareDriver();

    DDCAPS DriverCaps, HELCaps;

	int capsSize = sizeof(DDCAPS_DX3);

    memset (&DriverCaps, 0, sizeof(DDCAPS));
    DriverCaps.dwSize = capsSize;
    memset (&HELCaps, 0, sizeof(DDCAPS));
    HELCaps.dwSize = capsSize;
    LastError = lpDD->GetCaps(&DriverCaps,&HELCaps);

    if (FAILED(LastError)) {
        SetErrorString("Getting DD capabilities failed while checking total video memory.\n%s",
                              ErrorToString(LastError));
        return 0L;
    }
	if (what == GC_TOTAL_VIDEO_MEMORY)
		if (DriverCaps.dwVidMemTotal)
			return DriverCaps.dwVidMemTotal;
		else
			return HELCaps.dwVidMemTotal;

	if (what == GC_FREE_VIDEO_MEMORY)
	    if (DriverCaps.dwVidMemTotal)
		    return DriverCaps.dwVidMemFree;
		else
			return HELCaps.dwVidMemFree;
	if (what == GC_FREE_TEXTURE_MEMORY)
		return GetTextureMemory(FALSE);
	if (what == GC_TOTAL_TEXTURE_MEMORY)
		return GetTextureMemory(TRUE);

	return 0;
}

// check if device is proabably ok 
BOOL GD3Device::IsDriverOkForWindowMode(int driver,int w,int h, DWORD extraTextureMem) 
{

	DWORD mem = FreeVideoMemory();

	DWORD vsize = w*h* (WindowsDisplay.bpp) / 8;

	if (!Driver[driver].bCanDoWindow) return FALSE;
	if (!Driver[driver].bDoesTextures) return FALSE;

	if (Driver[driver].bIsHardware) {
		if (Driver[driver].bDoesZBuffer) vsize +=w*h*2;
		if (vsize>= mem) {
			TRACE("	IsDriverOkForWindowMode : assuming device has not enought video memory \n");			
			return FALSE;
		}

	}

	return TRUE;
}

 
/*
 * FreeVideoMemory
 * Returns the amount of total video memory supported (not free)
 */
DWORD
GD3Device::FreeVideoMemory()
{
    DDCAPS DriverCaps, HELCaps;

	int capsSize = sizeof(DDCAPS_DX3);

	if (!lpDD) return 0;

    memset (&DriverCaps, 0, sizeof(DDCAPS));
    DriverCaps.dwSize = capsSize;
    memset (&HELCaps, 0, sizeof(DDCAPS));
    HELCaps.dwSize =capsSize;

    LastError = lpDD->GetCaps(&DriverCaps,&HELCaps);

    if (FAILED(LastError)) {
        SetErrorString("Getting DD capabilities failed while checking free video memory.\n%s",
                              ErrorToString(LastError));
        return 0L;
    }
    if (DriverCaps.dwVidMemTotal)
        return DriverCaps.dwVidMemFree;
    else
        return HELCaps.dwVidMemFree;
}

// misc.cpp
/***************************************************************************/
/*                          Setting Defaults                               */
/***************************************************************************/
/*
 * SetDefaults
 * Set all the global variables to their default values.  Do not reset the
 * image files.
 */
void
GD3Device::SetDefaults(void)
{
    int n;
 
    lpClipper = NULL;
    lpPalette = NULL;
    bPrimaryPalettized = FALSE;
    bPaletteActivate = FALSE;
    bIgnoreWM_SIZE = FALSE;
    ZEROMEM(ppe);
    ZEROMEM(Originalppe);
    LastError = DD_OK;
    ZEROMEM(LastErrorString);
    D3DDeviceDestroyCallback = NULL;
    D3DDeviceDestroyCallbackContext = NULL;
    D3DDeviceCreateCallback = NULL;
    D3DDeviceCreateCallbackContext = NULL;
}

/***************************************************************************/
/*                Calling Device Create And Destroy Callbacks              */
/***************************************************************************/
BOOL
GD3Device::CallDeviceDestroyCallback(void)
{
    if (D3DDeviceDestroyCallback) {
        if (CallbackRefCount) {
            --CallbackRefCount;
            return (D3DDeviceDestroyCallback)(D3DDeviceDestroyCallbackContext);
        }
    }
    return TRUE;
}

BOOL
GD3Device::CallDeviceCreateCallback(int w, int h)
{
    if (D3DDeviceCreateCallback) {
        ++CallbackRefCount;
        return (D3DDeviceCreateCallback)(w, h, &lpD3DViewport,
                                         D3DDeviceCreateCallbackContext);
    }
    return TRUE;
}

/***************************************************************************/
/*            Choosing and verifying the driver and display mode           */
/***************************************************************************/
/*
 * PickDriver
 * Choose a driver from the list of available drivers which can render to one
 * of the given depths.  Hardware is prefered.  Mono-lighting drivers are
 * prefered over RGB.
 */
BOOL
GD3Device::PickDriver(int* driver, DWORD depths)
{
    int i, j;
    j = 0;
    for (i = 0; i < NumDrivers; i++)
        if (Driver[i].Desc.dwDeviceRenderBitDepth & depths)
            break;
    if (i >= NumDrivers) {
        *driver = D3DAPP_BOGUS;
        return TRUE;
    }
    j = i;
    for (i = 0; i < NumDrivers; i++) {
        if (Driver[i].Desc.dwDeviceRenderBitDepth & depths) {
            if (Driver[i].bIsHardware && !Driver[j].bIsHardware)
                                                  j = i;
            else if (Driver[i].bIsHardware == Driver[j].bIsHardware) {
                if (Driver[i].Desc.dcmColorModel & D3DCOLOR_MONO &&
                    !(Driver[j].Desc.dcmColorModel & D3DCOLOR_MONO))
                        j = i;
            }
        }
    }
    if (j >= NumDrivers)
        *driver = D3DAPP_BOGUS;
    else
        *driver = j;
    return TRUE;
}

/*
 * FilterDisplayModes
 * Set the bThisDriverCanDo flag for each display mode if the given driver
 * can render in that depth.  Also checks to make sure there is enough
 * total video memory for front/back/z-buffer in video memory if it's a
 * hardware device.
 */
BOOL
GD3Device::FilterDisplayModes(int driver)
{
    int i;
    DWORD depths = Driver[driver].Desc.dwDeviceRenderBitDepth;

    for (i = 0; i < NumModes; i++) {
       Mode[i].bThisDriverCanDo = FALSE;
        if (!(BPPToDDBD(Mode[i].bpp) & depths))
            continue;
       Mode[i].bThisDriverCanDo = TRUE;
            
    }
	if (CurrMode >= 0) 
		ThisMode.bThisDriverCanDo =  Mode[CurrMode].bThisDriverCanDo;
    return TRUE;
}

/*
 * PickDisplayMode
 * Pick a display mode of one of the given depths.  640x480x16 is prefered.
 */
BOOL
GD3Device::PickDisplayMode(int *mode, DWORD depths)
{
    int i, j;
    for (i = 0; i < NumModes; i++)
        if (BPPToDDBD(Mode[i].bpp) & depths)
            break;
    j = i;
    for (; i < NumModes; i++) {
        if (!(BPPToDDBD(Mode[i].bpp) & depths))
            continue;
        if (Mode[i].w == 640 && Mode[i].h == 480 &&  Mode[i].bpp == 16) {
            j = i;
            break;
        }
    }
    if (j >= NumModes)
        *mode = D3DAPP_BOGUS;
    else
        *mode = j;
    return TRUE;
}

/*
 * VerifyDriverAndMode
 * Verifies the selected driver and mode combination.  If the driver is
 * specified, the mode will be changed to accomodate the driver if it's not
 * compatible.  If the driver is not specified, one will be selected which is
 * compatible with the specified mode.  If neither are specified, a suitable
 * pair will be returned.
 */
BOOL
GD3Device::VerifyDriverAndMode(int* lpdriver, int* lpmode)
{
    DWORD depths;
    int driver, mode, i;
    driver = *lpdriver; mode = *lpmode;

    if (mode == D3DAPP_USEWINDOW && !bIsPrimary) {
        SetErrorString("Cannot render to a window when the DirectDraw device is not the primary.\n");
        goto exit_with_error;
    }

    /*
     * If I've been ask to choose a driver, choose one which is compatible
     * with the specified mode.
     */
    if (driver == D3DAPP_YOUDECIDE) {   
        if (mode == D3DAPP_USEWINDOW) {
            /*
             * I must find a driver for this display depth
             */
            depths = BPPToDDBD(WindowsDisplay.bpp);

            ATTEMPT(PickDriver(&driver, depths));

            if (driver == D3DAPP_BOGUS) {
                SetErrorString("Cannot find a D3D device driver which is compatible with the current display depth.\n");
                goto exit_with_error;
            }
            /*
             * I don't need to go through the mode selection since I've
             * verified it here
             */
            goto ret_ok;
        } else if (mode == D3DAPP_YOUDECIDE) {
            /*
             * I'm free to choose any driver which can use even one
             * supported depth
             */
            if (bIsPrimary)
                depths = BPPToDDBD(WindowsDisplay.bpp);
            else
                depths = 0;
            for (i = 0; i < NumModes; i++)
                depths |= BPPToDDBD(Mode[i].bpp);

            ATTEMPT(PickDriver(&driver, depths));

            if (driver == D3DAPP_BOGUS) {
                SetErrorString("Cannot find a D3D device driver which is compatible with the current display depth or any supported fullscreen mode.\n");
                goto exit_with_error;
            }
            /*
             * The mode will be chosen in the next section
             */
        } else {
            /*
             * Must pick a driver which uses the given mode depth
             */
            ATTEMPT(PickDriver(&driver, BPPToDDBD(Mode[mode].bpp)));
            if (driver == D3DAPP_BOGUS) {
                SetErrorString("Cannot find a D3D device driver which is compatible with the specified fullscreen mode.\n");
                goto exit_with_error;
            }
            /*
             * I don't need to go through the mode selection since I've
             * verified it here
             */
            goto ret_ok;
        }
    }

    /* 
     * At this stage, I have a driver I want to match the mode to.
     */
    if (mode == D3DAPP_YOUDECIDE) {
        /*
         * If it's my choice, I prefer windowed over fullscreen
         */
        if (bIsPrimary) {
            if (BPPToDDBD(WindowsDisplay.bpp) & 
                   Driver[driver].Desc.dwDeviceRenderBitDepth) {
                mode = D3DAPP_USEWINDOW;
                goto ret_ok;
            }
        }
        /*
         * Either this is not a primary DD device or the driver cannot use
         * the Windows display depth
         */
        ATTEMPT(PickDisplayMode(&mode,
                       Driver[driver].Desc.dwDeviceRenderBitDepth));
        if (mode == D3DAPP_BOGUS) {
            SetErrorString("The selected D3D device driver is not compatible with the current display depth or any supported fullscreen modes.\n");
            goto exit_with_error;
        }
        goto ret_ok;

    } else if (mode == D3DAPP_USEWINDOW) {
        /*
         * Check to see if this driver can use the Windows display depth
         */
        if (BPPToDDBD(WindowsDisplay.bpp) &
               Driver[driver].Desc.dwDeviceRenderBitDepth) {
            goto ret_ok;
        } else {
		    /*
             * Since it cannot, call this function again to choose any 
             * display mode which is compatible 
             */
            //mode = D3DAPP_YOUDECIDE;
			driver = D3DAPP_YOUDECIDE; // HG choose a driver 
		
            ATTEMPT(VerifyDriverAndMode(&driver, &mode));
            if (driver == D3DAPP_BOGUS)
                goto exit_with_error;
            goto ret_ok;

        }
	} else {
        /*
         * Check to see if this driver can use the specified fullscreen mode
         */
        if (BPPToDDBD(Mode[mode].bpp) & Driver[driver].Desc.dwDeviceRenderBitDepth) {
            goto ret_ok;
        } else {
            /*
             * Since it cannot, call this function again to choose any
             * display mode which is compatible
             */
            mode = D3DAPP_YOUDECIDE;
            ATTEMPT(VerifyDriverAndMode(&driver, &mode));
            if (driver == D3DAPP_BOGUS)
                goto exit_with_error;
            goto ret_ok;
        }
    }

ret_ok:
    *lpdriver = driver; 
	*lpmode = mode;
    return TRUE;
exit_with_error:
    return FALSE;
}

/***************************************************************************/
/*                        Dirty Rectangle Functions                        */
/***************************************************************************/
/*
 * ValidateDirtyRects
 * Set the dirty rectangles for the front and back buffers to the entire
 * client size.
 */
void
GD3Device::ValidateDirtyRects(void)
{
    NumDirtyClientRects = 1; NumDirtyBackRects = 1; NumDirtyZRects = 1;
    SetRect((LPRECT)&DirtyClient[0], 0, 0, szClient.cx, szClient.cy);
    SetRect((LPRECT)&DirtyBack[0], 0, 0, szClient.cx, szClient.cy);
    SetRect((LPRECT)&DirtyZ[0], 0, 0, szClient.cx, szClient.cy);
}

/*
 * CopyRectList
 * Copy a list of rectangles to another
 */
void
GD3Device::CopyRectList(int* dstnum, LPD3DRECT dst, int srcnum, LPD3DRECT src)
{
    int i;
    for (i = 0; i < srcnum; i++)
        dst[i] = src[i];
    *dstnum = srcnum;
}

/*
 * MERGE macro
 * Set first rectangle to be the smallest rectangle containing both rects
 */
#undef MERGE
#define MERGE(rc1, rc2)                         \
    do {                                        \
        if (rc2.x1 < rc1.x1) rc1.x1 = rc2.x1;   \
        if (rc2.y1 < rc1.y1) rc1.y1 = rc2.y1;   \
        if (rc2.x2 > rc1.x2) rc1.x2 = rc2.x2;   \
        if (rc2.y2 > rc1.y2) rc1.y2 = rc2.y2;   \
    } while(0)

/*
 * MergeRectLists
 * Merge two lists of rectangles to create another list of rectangles. The
 * merged list of rectangles covers all the area of the original two with NO
 * OVERLAPPING amongst it's rectangles.
 */
void
GD3Device::MergeRectLists(int* dstnum, LPD3DRECT dst, int src1num, LPD3DRECT src1,
                     int src2num, LPD3DRECT src2)
{
    LPD3DRECT rc;
    int* isvalid;
    int num, i, j, intersect;
    rc = (LPD3DRECT)malloc(sizeof(D3DRECT) * D3DAPP_MAXCLEARRECTS * 2);
    memset(rc, 0, sizeof(D3DRECT) * D3DAPP_MAXCLEARRECTS * 2);
    isvalid = (int*)malloc(sizeof(int) * D3DAPP_MAXCLEARRECTS * 2);
    memset(isvalid, 0, sizeof(int) * D3DAPP_MAXCLEARRECTS * 2);
    for (i = 0; i < src1num; i++) {
        memcpy(&rc[i], &src1[i], sizeof(D3DRECT));
        if ((rc[i].x1 == 0 && rc[i].x2 == 0) ||
            (rc[i].y1 == 0 && rc[i].y2 == 0))
            isvalid[i] = 0;
        else if (rc[i].x1 <= rc[i].x2 && rc[i].y1 <= rc[i].y2)
            isvalid[i] = 1;
        else
            isvalid[i] = 0;
    }
    for (i = 0; i < src2num; i++) {
        memcpy(&rc[i + src1num], &src2[i], sizeof(D3DRECT));
        if (rc[i + src1num].x1 <= rc[i + src1num].x2 &&
            rc[i + src1num].y1 <= rc[i + src1num].y2)
            isvalid[i + src1num] = 1;
        else
            isvalid[i + src1num] = 0;

    }
    num = src1num + src2num;
    for (i = 0; i < num - 1; i++) {
        if (!isvalid[i]) continue;
        j = i + 1;
        do {
            intersect = 0;
            for (; j < num; j++) {
                if (j != i && isvalid[j]) {
                    if (rc[i].x1 < rc[j].x1) {
                        if (rc[i].x2 < rc[j].x1)
                            continue;
                    } else {
                        if (rc[j].x2 < rc[i].x1)
                            continue;
                    }
                    if (rc[i].y1 < rc[j].y1) {
                        if (rc[i].y2 < rc[j].y1)
                            continue;
                    } else {
                        if (rc[j].y2 < rc[i].y1)
                            continue;
                    }
                    MERGE(rc[i], rc[j]);
                    isvalid[j] = 0;
                    j = 0; intersect = 1;
                    break;
                }
            }
        } while(intersect);
    }

    for (i = 0, j = 0; i < num; i++)
        if (isvalid[i]) ++j;
    if (j > D3DAPP_MAXCLEARRECTS) {
        for (i = 0; i < num; i++)
            if (isvalid[i]) break;
        if (i < num) {
            *dstnum = 1;
            dst[0] = rc[i];
            for (; i < num; i++) {
                if (isvalid[i]) {
                    MERGE(dst[0], rc[i]);
                }
            }
        } else {
            *dstnum = 0;
        }
    } else {
        for (i = 0, j = 0; i < num; i++) {
            if (isvalid[i]) {
                memcpy(&dst[j], &rc[i], sizeof(D3DRECT));
                ++j;
            }
        }
        *dstnum = j;
    }
    free(rc);
    free(isvalid);
}

/***************************************************************************/
/*                     Getting and Setting Window Attribs                  */
/***************************************************************************/
/*
 * SetClientSize
 * Set the client size of the given window.  A WM_SIZE message is generated,
 * but ignored.
 */
void
GD3Device::SetClientSize(HWND hwnd, int w, int h, BOOL bReturnFromFullscreen)
{
    RECT rc;

    bIgnoreWM_SIZE = TRUE;
    if (bReturnFromFullscreen) {

		if (::SetWindowLong(hwnd,GWL_EXSTYLE,lastWinExStyle) ==0) {
				TRACE("set window long ex failed ");
		}

        SetRect(&rc, 0, 0, w, h);
        
		AdjustWindowRectEx(&rc, GetWindowLong(hwnd, GWL_STYLE),
                           GetMenu(hwnd) != NULL,
                           GetWindowLong(hwnd, GWL_EXSTYLE));

        SetWindowPos(hwnd, NULL, 0, 0, rc.right-rc.left, rc.bottom-rc.top,
                     SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
        SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0,
                     SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);

    } else {
        /*
         * This is the only way to set the client size correctly if the menu
         * is stacked, so do it unless we are returning from a fullscreen
         * mode.
         */
        SendMessage(hwnd, WM_SIZE, SIZE_RESTORED, w + (h << 16));

        GetWindowRect(hwnd, &rc);
        SetWindowPos(hwnd, NULL, 0, 0, rc.right-rc.left,
                     rc.bottom-rc.top,
                     SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
        SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0,
                     SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
    }
    bIgnoreWM_SIZE = FALSE;
    pClientOnPrimary.x = pClientOnPrimary.y = 0;
    ClientToScreen(hwnd, &pClientOnPrimary);
    szClient.cx = w; szClient.cy = h;

	OnMove(0,0);

    GetWindowRect(hwnd, &rc);
	TRACE("GD3Device::SetClientSize %d %d (%d %d) \n",w,h,	rc.right-rc.left, rc.bottom-rc.top);
}

/*
 * GetClientWin
 * Gets the client window size and sets it in the nfo structure
 */
void
GD3Device::GetClientWin(HWND hwnd)
{
    if (!bFullscreen) {

       pClientOnPrimary.x = pClientOnPrimary.y = 0;
       ClientToScreen(hwnd, &pClientOnPrimary);

	   // Get the dimensions of the viewport and screen bounds

	   GetClientRect( hwnd, &rcViewportRect );
       szClient.cx = rcViewportRect.right;
       szClient.cy = rcViewportRect.bottom;

	   GetClientRect( hwnd, &rcScreenRect );
	   ClientToScreen( hwnd, (POINT*)&rcScreenRect.left );
	   ClientToScreen( hwnd, (POINT*)&rcScreenRect.right );
	   pClientOnPrimary = *((POINT*)&rcScreenRect.left); 

    } else {
        /*
         * In the fullscreen case, we must be careful because if the window
         * frame has been drawn, the client size has shrunk and this can
         * cause problems, so it's best to report the entire screen.
         */
       pClientOnPrimary.x = pClientOnPrimary.y = 0;
       szClient.cx = ThisMode.w;
       szClient.cy = ThisMode.h;
	   TRACE("GD3Device::GetClientWin %i %i\n", szClient.cx, szClient.cy);

	   SetRect( &rcViewportRect, 0, 0, szClient.cx, szClient.cy);
	   memcpy( &rcScreenRect, &rcViewportRect, sizeof(RECT) );
    }
}

// called on WM_MOVE message to update primary surface rects
void
GD3Device::OnMove(int cx, int cy)
{
	if (bInitialized) {
		if (!bFullscreen) {
            /*
             * Update client window position information
             */
			// assume size not changed 

            //pClientOnPrimary.x = pClientOnPrimary.y = 0;
            //ClientToScreen(hwnd, &pClientOnPrimary);

			GetClientRect( hwnd, &rcViewportRect);
			if (rcViewportRect.left >0) {
				ASSERT(0);
			}

			GetClientRect( hwnd, &rcScreenRect );

			MapWindowPoints(hwnd,NULL,(POINT*)&rcScreenRect.left,2); 
			// ClientToScreen( hwnd, (POINT*)&rcScreenRect.left);
			// ClientToScreen( hwnd, (POINT*)&rcScreenRect.right);


			pClientOnPrimary = *((POINT*)&rcScreenRect.left); 

#ifdef _DEBUG_XXX
			TRACE("Client on primary Rect l %d t %d r %d b %d \n",
				rcScreenRect.left,rcScreenRect.top,rcScreenRect.right,rcScreenRect.bottom);

			HWND parent = GetParent(hwnd);
			if (parent) {
				RECT rp,ri;
				GetClientRect( hwnd, &rp );
				ClientToScreen( hwnd, (POINT*)&rp.left);
				ClientToScreen( hwnd, (POINT*)&rp.right);
				IntersectRect(&ri,&rcScreenRect,&rp);
				TRACE("IntersectRect with parent l %d t %d r %d b %d \n",ri.left,ri.top,ri.right,ri.bottom);
			}
			
			/* does not help 
			if (lpClipper) {
				LastError = lpClipper->SetHWnd(0, hwnd);
				if(LastError != DD_OK ) {
					SetErrorString("Attaching clipper to window failed.\n%s",
						              ErrorToString(LastError));
				}
			}
			*/
#endif
		}
	}

}

// OnActivate :: WM_ACTIVATE, set palette
void GD3Device::OnActivate()
{
	if (bInitialized) {
            /*
             * Set the front buffer's palette
             */
            if (bPaletteActivate && bPrimaryPalettized &&
                lpFrontBuffer) {
                lpFrontBuffer->SetPalette(lpPalette);
            }
	}

}



/***************************************************************************/
/*                              Error reporting                            */
/***************************************************************************/

/*
 * SetErrorString
 * Set the global variable which records the last error string.
 */
void
G__cdecl GD3Device::SetErrorString( LPSTR fmt, ... )
{
    char buff[512];

    buff[0] = 0;
    wvsprintf(&buff[0], fmt, (char *)(&fmt+1));
    strcat(buff, "\r\n");
    strcpy(LastErrorString, buff);
	TRACE(buff);


}

/* dpf
 * Debug printf.  Very useful for fullscreen exclusive mode or when surfaces
 * are in video memory.
 */
void G__cdecl dpf( LPSTR fmt, ... )
{
    char buff[256];

    strcpy(buff, "D3D: ");
    wvsprintf(&buff[lstrlen(buff)], fmt, (char *)(&fmt+1));
    strcat(buff, "\r\n");
    OutputDebugString(buff);
}


// texture.cpp
// textures are handled in RenderState
BOOL GD3Device::LoadAllTextures() { return TRUE; }
void GD3Device::ReleaseAllTextures() { }



#endif
