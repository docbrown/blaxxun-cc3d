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

@module GModel.cpp - GLView data model (document) class implementation|

Copyright (c) 1995-1999	by Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:

Changes:
	physicalCamera == physical camera used for rendering
	Sound abstraction RSX/DirectSound

$Revision: 1.60 $
$Log: gmodel.cpp,v $
Revision 1.60  1998/10/12 12:52:55  holger
empty message

Revision 1.55  1998/09/28 16:04:00  tom
*** empty log message ***

Revision 1.54  1998/09/22 16:37:32  holger
empty message

Revision 1.51  1998/08/20 17:22:05  holger
empty message

Revision 1.50  1998/07/17 12:59:11  kristof
set myavatarurl also when not shown

Revision 1.49  1998/07/14 14:01:09  kristof
removed wait cursor

Revision 1.48  1998/07/06 17:50:30  holger
empty message

Revision 1.32  1998/04/21 08:28:06  bernd
hg

Revision 1.31  1998/04/15 18:49:49  holger
*** empty log message ***


Todo :

******************************************************************************/

#include "stdafx.h"


#ifdef _MFC

#ifndef _GLVIEW_DLL
#include "glviewhdr.h"
#include "Mainfrm.h"
#else

#include "blaxxunCC3dCtl.h"
#define CMyglView CGLViewCtrlCtrl

#endif

#endif


#include "gversion.h"

#include "gviewcontrol.h"

#include "grender.h"

#include "gkatmai.h"


#if defined(_OGL) && defined(WIN32)
#include "COpenGL.h"
#endif

#if defined(_OGL) && defined(_GLMESA)
#include "xmesagl.h"
#endif

#include "gsound.h"

#include "grsxsound.h"
#include "gdssound.h"


#include "glutils.h"

#include "gpanel.h"


#include "quaternation.h"
#include "gcamerautil.h"


#include "gclass.h"
#include "gshell.h"
#include "gshellop.h"
#include "gshelli.h"
#include "gnurbsutil.h"
#include "gnurbs.h"
#include "gcamera.h"
#include "gvisibility.h"

#include "collision/rapid/rapid.h"

#ifdef Linux
#include "ronny.h"
#endif

#ifndef _GLVIEW_DLL
#include "ronny.h"
#endif


#include "greporter.h"

#include "gmodel.h"

#ifdef _DXF
#include "gdxfin.h"
#endif

#ifndef  _GV_MINIMAL
#include "gdeform.h"
#endif

#include "gvbsp.h"

#ifdef _GV

// VRML 
#include <gvscene.h>
#include "greadgv.h"
#include <gvnodes.h>
#include <gvtraverse.h>
// #include <gvwebfx.h>
#include <gvlayer.h>
#include <gvsensors.h>
#include <gvconnection.h>
#include <gvproto.h>

#include <gvscript.h>
#include <gvnurbs.h>

#endif

#include "www.h"
#include "gurlcache.h"


#ifndef  _GV_MINIMAL
// for BuildShell operation
#include "gbuildshell.h"
#include "goptimizetraversal.h"
#endif


// for TransparencyTraversal
#include "gtransparency.h"


// for Select operation
#include "grayselect.h"

#include "gfindnode.h"

// Event handlers 
#include "gevent.h"



#include "gvhandle.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define T1  (511.0f/512.0f)
#define TXEPS (1.0f/512.0f)
/* geometry construction helpers */
static point_ params[] = { {0.0,0.0, 0.0 }, { T1,0.0, 0.0 }, { T1, T1, 0.0 }, { 0.0, T1, 0.0 } };

static point_ front[] = { {-1.0,-1.0, -1.0 }, { 1.0,-1.0, -1.0 }, { 1.0, 1.0, -1.0 }, { -1.0,1.0, -1.0 } };

static point_ center[] = { {-1.0,-1.0, 0.0 }, { 1.0,-1.0, 0.0 }, { 1.0, 1.0, 0.0 }, { -1.0,1.0, 0.0 } };

static point_ back[] = {{ 1.0,-1.0, 1.0 }, {-1.0,-1.0, 1.0 }, { -1.0,1.0, 1.0 }, { 1.0, 1.0, 1.0 }  };

static point_ bottom[] = { {-1.0,-1.0, 1.0 }, { 1.0,-1.0, 1.0 }, { 1.0, -1.0, -1.0 }, { -1.0,-1.0, -1.0 } };

static point_ top[] = { { -1.0,1.0, -1.0 }, { 1.0, 1.0, -1.0 }, { 1.0,1.0, 1.0 }, {-1.0,1.0, 1.0 },  };

static point_ left[] = { {-1.0,-1.0, 1.0 }, { -1.0,-1.0, -1.0 }, { -1.0, 1.0, -1.0 }, { -1.0, 1.0, 1.0 } };

static point_ right[] = { { 1.0,-1.0, -1.0 }, {1.0,-1.0, 1.0 },  { 1.0,1.0, 1.0 },  { 1.0, 1.0, -1.0 } };

static point_ white4[] = { { 1.0,1.0, 1.0 }, {1.0,1.0, 1.0 },  { 1.0,1.0, 1.0 },  { 1.0, 1.0, 1.0 } };



/* Create a single textured quad */ 
GvNode *newTFace(const Point *coords,GvMFString &url)
{
	GvShape *shape = new GvShape();
	GShell *shell = new GPolygon(4, coords);
	shell->SetVP(4,(const Point *) params);
	shell->SetFlat();

	GvNodeShell *node = new GvNodeShell();
	node->SetShell(shell);
	GvImageTexture *t = new GvImageTexture();
	GvAppearance *a = new GvAppearance();


	t->url.set(url);
	t->repeatS.set(FALSE);
	t->repeatT.set(FALSE);

	a->texture.set(t);

	shape->appearance.set(a);
	shape->geometry.set(node);
	return(shape);
}

/* Create a single movie textured quad */ 
GvNode *newMovieTFace(const Point *coords,GvMFString &url)
{
	GvShape *shape = new GvShape();
	GShell *shell = new GPolygon(4, coords);
	shell->SetVP(4,(const Point *) params);
	shell->SetFlat();


	GvNodeShell *node = new GvNodeShell();
	node->SetShell(shell);
	GvMovieTexture *t = new GvMovieTexture();
	GvAppearance *a = new GvAppearance();


	t->url.set(url);
	t->loop.set(TRUE);
	a->texture.set(t);
	shape->appearance.set(a);
	shape->geometry.set(node);
	return(shape);
}


// is position
gbool Outside(GCamera &camera,const Point position)
{

	PointArray viewVolume;
	
	BBox box(position);


	float zfar = camera.zfar;

	Point dir =camera.target - camera.position;
	float d= dir.Length();

	Point right = camera.NormRight();
	Point up = camera.up;


	dir *= zfar / d;

	right *= zfar*0.5*camera.widthWorld / d;
	up *= zfar*0.5*camera.heightWorld / d;

	Point p = camera.position + dir; // point on far plane 

	// compute 4 points on far plane 
	viewVolume.SetLength(4);
	viewVolume[0]=p+right+up;
	viewVolume[1]=p+right-up;
	viewVolume[2]=p-right-up;
	viewVolume[3]=p-right+up;

	return BoxViewVolumeOutside(box,camera.position,viewVolume);


}

// set pointer to physical camera
void GGeometryTraversal::SetPhysicalCamera(GCamera *camera)
{
	this->physicalCamera = camera;

	// update viewvolume 
	
	float zfar = camera->zfar;

	viewpoint = camera->position;
	Point dir =camera->target - camera->position;
	float d= dir.Length();

	Point right = camera->NormRight();
	Point up = camera->up;


	dir *= zfar / d;

	right *= zfar*0.5*camera->widthWorld / d;
	up *= zfar*0.5*camera->heightWorld / d;

	Point p = viewpoint + dir; // point on far plane 

	// compute 4 points on far plane 
	viewVolume.SetLength(4);
	viewVolume[0]=p+right+up;
	viewVolume[1]=p+right-up;
	viewVolume[2]=p-right-up;
	viewVolume[3]=p-right+up;

	viewPlanes.SetLength(5);
	viewPlanes[0]=Plane3(viewpoint,viewVolume[1],viewVolume[0]);
	viewPlanes[1]=Plane3(viewpoint,viewVolume[2],viewVolume[1]);
	viewPlanes[2]=Plane3(viewpoint,viewVolume[3],viewVolume[2]);
	viewPlanes[3]=Plane3(viewpoint,viewVolume[0],viewVolume[3]);
	viewPlanes[4]=Plane3(viewVolume[0],viewVolume[1],viewVolume[2]);


}



#ifdef _D3D 

/*
 * AfterDeviceCreated
 * D3DApp will call this function immediately after the D3D device has been
 * created (or re-created).  D3DApp expects the D3D viewport to be created and
 * returned.  The sample's execute buffers are also created (or re-created)
 * here.
 */

BOOL
GView::AfterDeviceCreated(int w, int h, LPDIRECT3DVIEWPORT* lplpViewport, LPVOID lpContext)
{ 
	GView * view = (GView *) lpContext;
	return view->AfterDeviceCreated(w,h,lplpViewport);
}

BOOL
GView::AfterDeviceCreated(int w, int h, LPDIRECT3DVIEWPORT* lplpViewport)
{
    LPDIRECT3DVIEWPORT lpD3DViewport=NULL;


    HRESULT rval=0;

	if (!device->CreateViewport(w,h)) {
		ReportD3DError();
        // GetReporter()->Error("D3D device not created.\n");
		return FALSE;
	}

	lpD3DViewport = device->lpD3DViewport;

    /*
     * Create the sample's execute buffers via InitView
     */
    if (!InitView()) {
        GetReporter()->Error("InitView failed.\n");
        // HG CleanUpAndPostQuit();
        return FALSE;
    }

    /*
     * Create and initialize the surfaces containing the frame rate and
     * window information
     */
    // fails on laptop InitFontAndTextBuffers();

	updateIsComplex = TRUE;

    return TRUE;
}

/*
 * BeforeDeviceDestroyed
 * D3DApp will call this function before the current D3D device is destroyed
 * to give the app the opportunity to destroy objects it has created with the
 * DD or D3D objects.
 */
BOOL
GView::BeforeDeviceDestroyed(LPVOID lpContext)
{ 
	GView * view = (GView *) lpContext;
	return view->BeforeDeviceDestroyed();
}

BOOL
GView::BeforeDeviceDestroyed()
{
	
#ifdef G_PANEL

	if (panel)
		panel->Term();

#endif

	/*
     * Release all objects (ie execute buffers) created by InitView
     */
    ReleaseView();

    /*
     * Since we created the viewport it is our responsibility to release
     * it. 
     */
    device->ReleaseViewport();

    return TRUE;
}

/*
 * Create the D3D Device
 * Create all DirectDraw and Direct3D objects necessary to begin rendering.
 */
BOOL
GView::CreateD3DDevice( HWND hWnd,	// handle of window
			 HWND hWndFrame,		// handle of app window for exclusive mode 
			 const char *driver,	// preferenced Driver, default "Ramp Driver"
			 GDriverHints *driverHints 
			 )
{


    BOOL bOnlySystemMemory , bOnlyEmulation;
	bOnlySystemMemory = driverHints->m_onlySystemMemory;
	bOnlyEmulation = driverHints->m_onlyEmulation;

    DWORD flags;

	int x;
	x=0;

#ifdef _KATMAI
	if (isKatmaiProcessor) 
		x = 1;
#endif


#ifdef _GLVIEW_IE

	if (viewController && viewController->GetProfile(_T("Direct3D.useSoftwareLighting"),x)) {}

	renderState.enableOwnLighting = (x>0); 

	TRACE(" own lighting set to %d \n",x);
	if (x>0) x = 1;
	if (viewController && viewController->GetProfile(_T("Direct3D.useD3DCulling"),x)) renderState.enableD3DCulling = (x>0); 

	x = 0;
	if (viewController && viewController->GetProfile(_T("Direct3D.dxlevel"),x)) device->dxLevel = x; 
	x=0;
	if (viewController && viewController->GetProfile(_T("Direct3D.maxverts"),x)) if (x>8) GShell::maxSubMeshVertices = x;  
#else
		
	x = AfxGetApp()-> GetProfileInt("Direct3D","dxlevel",x); // get the level of direct x support 
	device->dxLevel = x; 
	// number of vertices for submesh creation 
	x = AfxGetApp()-> GetProfileInt("Direct3D","maxverts",0); // get max number of vertices
	if (x>8) GShell::maxSubMeshVertices = x; 

	if (pView->GetProfile("Direct3D","useSoftwareLighting",x)) {}

	renderState.enableOwnLighting = (x>0); 
	TRACE(" own lighting set to %d \n",x);
	if (x>0) x = 1;
	if (pView && pView->GetProfile("Direct3D","useD3DCulling",x)) renderState.enableD3DCulling = (x>0); 

	x = 0;
	if (pView && pView->GetProfile("Direct3D","dxlevel",x)) device->dxLevel = x; 
	x=0;
	if (pView && pView->GetProfile("Direct3D","maxverts",x)) if (x>8) GShell::maxSubMeshVertices = x;  

#endif 

	CString theDriver("Ramp Emulation");

	if (driverHints->m_useRgb)
		theDriver = "RGB Emulation";

	if (driver != NULL && strlen(driver)>0)
		theDriver = driver;

 
    /*
     * Set the flags to pass to the D3DApp creation based on command line
     */
    flags = ((bOnlySystemMemory) ? D3DAPP_ONLYSYSTEMMEMORY : 0) | 
            ((bOnlyEmulation) ? (D3DAPP_ONLYD3DEMULATION |
                                 D3DAPP_ONLYDDEMULATION) : 0);
	if (driverHints->m_useHW) 
		flags |= D3DAPP_ONLY_HW;
	if (driverHints->m_useRgb) 
		flags |= D3DAPP_ONLY_RGB;
	/*
     * Create all the DirectDraw and D3D objects neccesary to render.  The
     * AfterDeviceCreated callback function is called by D3DApp to create the
     * viewport and the example's execute buffers.
     */
	// m_useFullscreen

    if (!device->CreateFromHWND(flags, 
							  theDriver,
							  /* driver */ D3DAPP_YOUDECIDE,
							  /* mode */ (driverHints->m_useFullscreen  ? D3DAPP_YOUDECIDE :   D3DAPP_USEWINDOW, D3DAPP_YOUDECIDE),
							  hWnd,hWndFrame, 
							  AfterDeviceCreated, this, 
							  BeforeDeviceDestroyed, this 
							  ) || device->bFullscreen) {

		TRACE("Init D3D Error %s", device->GetLastErrorString());
        
		if (device->LastError != DDERR_OUTOFVIDEOMEMORY)
			ReportD3DError();

			// fall back to RampEmulation Driver 
		//if (theDriver != "Ramp Emulation")	
		{
			Message("Falling back to Direct 3D Software driver ");

			theDriver = "Ramp Emulation";
			if (driverHints->m_useRgb)
				theDriver = "RGB Emulation";

			flags |= D3DAPP_ONLYSYSTEMMEMORY;
			flags &= ~D3DAPP_ONLY_HW;

			if (device->CreateFromHWND(flags, 
						theDriver, 
						/* driver */ D3DAPP_YOUDECIDE,
						/* mode */ D3DAPP_USEWINDOW,
							hWnd,hWndFrame, 
							AfterDeviceCreated,this, 
							BeforeDeviceDestroyed, this)) {

				
			} else {
				flags |= D3DAPP_ONLYD3DEMULATION | D3DAPP_ONLYDDEMULATION;
				if (!device->CreateFromHWND(flags, 
						theDriver, 
						/* driver */ D3DAPP_YOUDECIDE,
						/* mode */ D3DAPP_USEWINDOW,
							hWnd,hWndFrame, 
							AfterDeviceCreated,this, 
							BeforeDeviceDestroyed, this)) 
				{
				theDriver = "RGB Emulation";

				if (!device->CreateFromHWND(flags, 
						theDriver, 
						/* driver */ D3DAPP_YOUDECIDE,
						/* mode */ D3DAPP_USEWINDOW,
							hWnd,hWndFrame, 
							AfterDeviceCreated,this, 
							BeforeDeviceDestroyed, this)) 
				{

								return FALSE;
				}
				}
			}
		}
		//else return FALSE;
    }
  
    bResizingDisabled = FALSE;
    bClearsOn = TRUE;
    bShowFrameRate = FALSE;
    bShowInfo = FALSE;
    bQuit = FALSE;


    return TRUE;
}

#if 0

/****************************************************************************/
/*                            Rendering loop                                */
/****************************************************************************/
/*
 * RenderLoop
 * Render the next frame and update the window
 */
static BOOL
RenderLoop()
{
    D3DRECT extents[D3DAPP_MAXCLEARRECTS];
    int count;
    /*
     * If all the DD and D3D objects have been initialized we can render
     */
    if (d3dapp->bRenderingIsOK) {
        /*
         * Restore any lost surfaces
         */
        if (!RestoreSurfaces()) {
            /*
             * Restoring surfaces sometimes fails because the surfaces cannot
             * yet be restored.  If this is the case, the error will show up
             * somewhere else and we should return success here to prevent
             * unnecessary error's being reported.
             */
            return TRUE;
        }
        /*
         * Calculate the frame rate
         */
        if (!CalculateFrameRate())
            return FALSE;

        /*
         * Clear the back buffer and Z buffer if enabled.  If bResized is set,
         * clear the entire window.
         */
        if (myglobs.bClearsOn) {
            if (!D3DAppClearBackBuffer(myglobs.bResized ?
                                       D3DAPP_CLEARALL : NULL)) {
                ReportD3DError();
                return FALSE;
            }
        }
        /*
         * Call the sample's RenderScene to render this frame
         */
        {
            if (!RenderScene(d3dapp->lpD3DDevice, d3dapp->lpD3DViewport,
                             &extents[0])) {
                GetReporter()->Error("RenderScene failed.\n");
                return FALSE;
            }
            count = 1;
        }
        /*
         * Blt the frame rate and window stat text to the back buffer
         */
/*
        if (!DisplayFrameRate(&count, &extents[1]))
            return FALSE;
*/
        /*
         * Give D3DApp the extents so it can keep track of dirty sections of
         * the back and front buffers
         */
        if (!D3DAppRenderExtents(count, extents, myglobs.bResized ?
                                 D3DAPP_CLEARALL : NULL)) {
            ReportD3DError();
            return FALSE;
        }
        /*
         * Blt or flip the back buffer to the front buffer.  Don't report an
         * error if this fails.
         */
        D3DAppShowBackBuffer(myglobs.bResized ? D3DAPP_SHOWALL : NULL);

        /*
         * Reset the resize flag
         */
        myglobs.bResized = FALSE;
    }
    return TRUE;
}

/*
 * AppPause
 * Pause and unpause the application
 */
static BOOL
AppPause(BOOL f)
{
    /*
     * Flip to the GDI surface and halt rendering
     */
    if (!D3DAppPause(f))
        return FALSE;
    /*
     * When returning from a pause, reset the frame rate count
     */
    if (!f) {
        ResetFrameRate();
    }
    return TRUE;
}


#endif

/*
 * RestoreSurfaces
 * Restores any lost surfaces.  Returns TRUE if all surfaces are not lost and
 * FALSE if one or more surfaces is lost and can not be restored at the
 * moment.
 */
BOOL
GView:: RestoreSurfaces()
{
	BOOL someLost=FALSE;
    /*
     * check all the surfaces it's in charge of
     */
    if (!device->CheckForLostSurfaces(someLost)) {
            return FALSE;
    }
	if (someLost) {
		ReleaseTextures();
	}

#if 0
    HRESULT d3drval;

    /*
     * Check frame rate and info surfaces and re-write them if they
     * were lost.
     */
    if (lpFrameRateBuffer && lpFrameRateBuffer->IsLost() == DDERR_SURFACELOST) {
        d3drval = lpFrameRateBuffer->Restore();
        if (d3drval != DD_OK) {
            return FALSE;
        }
        if (!WriteFrameRateBuffer(0.0f, 0))
            return FALSE;
    }
    if (lpInfoBuffer && lpInfoBuffer->IsLost() == DDERR_SURFACELOST) {
        d3drval = lpInfoBuffer->Restore();
        if (d3drval != DD_OK) {
            return FALSE;
        }
        if (!WriteInfoBuffer())
            return FALSE;
    }
#endif

    return TRUE;
}


/*
 * CleanUpAndPostQuit
 * Release all D3D objects, post a quit message and set the bQuit flag
 */
void
GView::CleanUpAndPostQuit(void)
{
    if (bQuit)
        return;
    if (!device->Destroy())
        ReportD3DError();
    ReleaseScene();
    bQuit = TRUE;
    PostQuitMessage( 0 );
}

/*
 * ReportD3DError
 * Reports an error during a d3d app call.
 */
void
GView::ReportD3DError()
{
    Error("%s", device->GetLastErrorString());
}

/* Msg
 * Message output for error notification.
 */
void G__cdecl GView::Error( LPSTR fmt, ... )
{
    char buff[256*4];

    wvsprintf(buff, fmt, (char *)(&fmt+1));
    strcat(buff, "\r\n");

    device->Pause(TRUE);

    if (device->bFullscreen)
        SetWindowPos(device->GetHWnd(), HWND_NOTOPMOST, 0, 0, 0, 0, 
					SWP_NOSIZE | SWP_NOMOVE);
    
	// pView->MessageBox(buff, _PROGRAM " Direct 3D Message", MB_OK );
	//AfxMessageBox(buff /*, _PROGRAM " Direct 3D Message" */, MB_OK );
	
	HWND hWndFrame =  hWnd;

	hWndFrame = AfxGetMainWnd()->GetSafeHwnd();



	::MessageBox(hWndFrame, buff , _PROGRAM " Direct 3D Message" , MB_OK | MB_ICONINFORMATION | MB_SYSTEMMODAL | MB_TOPMOST);

    
	if (device->bFullscreen)
        SetWindowPos(device->GetHWnd(), HWND_TOPMOST, 0, 0, 0, 0,
                     SWP_NOSIZE | SWP_NOMOVE);

    device->Pause(FALSE);
}





/*
 * Builds the scene
 */
BOOL
InitScene(void)
{
   return TRUE; 
}


/*
 * Initializes the execute buffer for rendering.

  called even from resizing !!!!!!!!!!!!!!!!!!!!!
 */

BOOL
GView::InitView()
{

	int x;

	renderState.SetDevice(device);
    renderState.Init();

//#ifndef _GLVIEW_IE
    //renderState().SetStateAll();

#ifdef _GLVIEW_IE
	if (viewController)
	{
	// test for software renderers 
	if (viewController->GetProfile(_T("Direct3D.perspectiveCorrection"),x)) 
			renderState.SetPerspectiveCorrectionHint((x>0));

	if (viewController->GetProfile(_T("Direct3D.specular"),x)) 
			renderState.SetSpecular((x>0));

	if (viewController->GetProfile(_T("Direct3D.rampSize"),x)) 
   		if (x>0) renderState.materialRampSize = x;

	GetProfileSettings();


	}
#endif

	// limit vertex count 
	x = AfxGetApp()-> GetProfileInt("Direct3D","MaxVertexCount",0);
	if (x>8 && ((renderState.maxVertexCount > x) || (renderState.maxVertexCount == 0))) {
   	  renderState.maxVertexCount = x;
	}
	x = AfxGetApp()->GetProfileInt("Direct3D","CanCull",0);
	if (x == 0 && renderState.canCull) {
   	  renderState.canCull = 0; 
	}
	x = AfxGetApp()->GetProfileInt("Direct3D","RampSize",0);
	if (x >0 ) {
   	  renderState.materialRampSize = x;
	}
//#endif
	if (maxUserTextureSize>0) {
		int sx,sy;
		int msx,msy;
		int maxs = 1<<(maxUserTextureSize-1);
		
//		renderState.textureParameters.GetCurrentMaxSize(sx,sy);
		renderState.textureParameters.GetMaxSize(msx,msy);

		sx = min(msx,maxs); 		sy = min(msy,maxs);
		renderState.textureParameters.SetCurrentMaxSize(sx,sy);
	}


    renderState.SetState();

	renderState.SetCamera(GetPhysicalCamera());  

#ifdef G_PANEL
	if (panel)
		panel->Init(device);

#endif

	// reapply background & fog 
	{
		boundBackgroundChanged = FALSE; 
		GvBackground *b = (GvBackground *) (GvNode*) boundBackground;
		if (b) b->Apply(this);
	}
	{
		GvFog *b = (GvFog *) (GvNode*) boundFog;
		if (b) b->Apply(this);
		boundFogChanged=FALSE;
	}




    return TRUE;
}

void
GView::ReleaseScene()
{


}

void
GView::ReleaseView()
{
	TRACE("GView::Releasing View\n");
	
	if (renderState.lpDev != NULL) {  // we had a device before 
		
		// tbd: but only on driver change ??
		ReleaseTextures();	// new 19.03.99 release MovieTextures ?? 
		// renderState.DeleteUnusedTextures();
	
		renderState.DeleteUnusedMaterials(); 
		renderState.DeleteLight(headLight);
		
		renderState.Term();

	}
	renderState.lpDD = NULL;
	renderState.lpD3D = NULL;
	renderState.lpDev = NULL;
	renderState.lpView = NULL;

	renderState.lpD3D2 = NULL;
	renderState.lpD3DDevice2 = NULL;
	renderState.lpD3DViewport2 = NULL;

	renderState.lpD3D3 = NULL;
	renderState.lpD3DDevice3 = NULL;
	renderState.lpD3DViewport3 = NULL;

#ifdef G_PANEL
	if (panel)
		panel->Term();
#endif

}
#endif



// release all handles to textures 
void GView::ReleaseTextures()
{
	int i;
	TRACE("GView::ReleaseTextures \n");

#ifdef _D3D
	renderState.ReleaseTexturedMaterials();
#endif

	renderState.ReleaseTextures();
	renderState.DeleteUnusedTextures();

	if (sceneInfo)
	for(i=0; i < sceneInfo->movieTextures.Length(); i++) {
		 GvMovieTexture *s = (GvMovieTexture *) (GvNode*) sceneInfo->movieTextures[i];
		 // if (s) s->Term(); // ReleaseSurface();
		 if (s) s->ReleaseSurface();
	}	

}




// get pointer to D3D object 
#ifdef _D3D

LPDIRECTDRAW GView::GetDirectDraw()
{
	if (!device) return NULL;
	return device->lpDD;
}

#endif


#if defined(WIN32)

#if defined(_OGL)

LPDIRECTDRAW GView::GetDirectDraw()
{
	if (lpDD == NULL) {
	/*
     * create the main DirectDraw object
     */
    HRESULT ddrval = DirectDrawCreate( NULL, &lpDD, NULL );
    if( ddrval == DD_OK )
    {
		CWinApp *a = AfxGetApp();
		HWND hWndFrame =  hWnd;

		hWndFrame = AfxGetMainWnd()->GetSafeHwnd();

		ddrval = lpDD->SetCooperativeLevel(hWndFrame, DDSCL_NORMAL);

	}		
	else {
		TRACE("OGL DirectDrawCreate failed \n");
	}
	}
	return lpDD;
}

#endif
#endif



#ifdef _GFULL


//static GLfloat radius;
//static RECT	oldrect;


static int wheelpart_facelist [] =
{ 3, 0,3,1, 3,0,1,4,   3, 4,1,2,   3, 2,1,3 };

static void WheelPartCoordinates(PointArray &p,float r=1.0,float w=0.4,float h=0.2,float r2scale=0.8) 
{
  p.SetLength(5);
  p[0].Set(0.0,0.0,0.0);
  p[1].Set(r*r2scale,0.0,h);
  p[2].Set(r,0.0,0.0);
  p[3].Set(r*r2scale,-w*0.5,0.0);
  p[4].Set(r*r2scale,+w*0.5,0.0);
}

// create a colored wheel
//GShell *NewWheel(float r=1.0,int parts = 6,float h=0.2,float r2factor=0.8) 

GShell *NewWheel(float r,int parts,float h,float r2factor) 
{
int i;

if (parts<=2) parts=3;

float w= TWOPI / (float) parts;
float width = 2.0 * tan(w*0.5) * r * r2factor;


GShell *s=new GShell();
PointArray PartPts;

WheelPartCoordinates(PartPts,r,width,h,r2factor);

GShell Part(PartPts.Length(),PartPts,ELEMENTS(wheelpart_facelist),wheelpart_facelist);
Point colors[2];
colors[0] = Point (1.0,0.0,0.0);
colors[1] = Point (0.0,0.0,1.0);
PointArray WheelColors(Part.GetFaceCount());

for (i= 0; i< Part.GetFaceCount();i ++) {
  WheelColors[i]= colors[ i%2 ];
}

Part.SetFC(WheelColors.Length(),WheelColors);

s->Append(Part,0);
for (i= 1; i< parts;i ++) {
   float w= i *TWOPI / (float) parts;
   Matrix m = RotationZMatrix(w);
   s->Append(Part,0,&m);
}
return(s);
}


GGroup *NewFractalTower(int level, GGroup * object,float w=1.0,float scale = 0.5)
{  GGroup *g;
   GGroup *sub;
   Matrix m;
   g = new GGroup();
     g->Insert(new GInstance(object));
	 if (level>0) {
        g->LinkIn(sub=NewFractalTower(level-1,object));
		m = ScaleMatrix(scale);
		m *= TranslationMatrix(-w * 0.5,0.0,0.0);
		g->Set(new GMatrix(m));
		// ...
	 }
   return(g);
}

#endif


/*

   create a grid of sphere objects 


*/


int rnd(int maxVal) {  return rand()  % maxVal; } 

float f_rnd() { return((float)rand() / (float)RAND_MAX); }
float f_rnds() {  return (f_rnd() * 2.0-1.0); } 

Point jitter(const Point &r)
{

	Point p( r.x * f_rnds(), r.y * f_rnds(), r.z * f_rnds());
	return(p);	
}


Point p_sphere(float u,float v,float r=1.0)
{
	Point p;

	u-=0.25; // start -y 

	u*= TWOPI;
	v *= PI;

	float cosv=cos(v);
	float sinv=sin(v);
    float cosu=cos(u);  // VC 4.1 bad value if optimized
	float sinu=sin(u);



	p.x= r*cosu*sinv;
	p.z= r*sinu*sinv;
	p.y= r*cosv;
	return(p);
}

// compute point on circle u 0 .. 1 
Point p_circle(float u, float r=1.0,float z=0.0)
{
	Point p;
	u-=0.25; // start -y 
	u*= TWOPI;
    float cosu=cos(u);  
	float sinu=sin(u);

	p.x= r*cosu;
	p.y= r*sinu;
	p.z= z;
	return(p);
}

// compute point on quadrangle  u 0 .. 1 
Point p_quad(float u, float r=1.0,float z=0.0)
{
	Point p(0,0,z);

	u -=0.125; // start -y 

	u*=4.0;
redo:
	if (u<0) u+=4;

	if (u < 1) {
		p.x=r;
		p.y= (2.0*(u-0.0)-1.0) *r;
	} else
	if (u <2) {
		p.y=r;
		p.x= - (2.0*(u-1.0)-1.0) *r;
	} else
	if (u < 3) {
		p.x=-r;
		p.y= -(2.0*(u-2.0)-1.0) *r;
	} else
	if (u <4) {
		p.y=-r;
		p.x=  (2.0*(u-3.0)-1.0) *r;
	} 
	else {
		u-=4.0; goto redo;
    }




	return(p);
}

// tangent 
Point t_circle(float u, float r=1.0)
{
	Point p;

	u*= TWOPI;
    float cosu=cos(u);  
	float sinu=sin(u);

	p.x= r*cosu;
	p.y= r*sinu;
	p.z= 0;
	p.Normalize();
	return(p);
}



void ComputeParticleMovement(int part,int totalPart,
						  PointArray &position,
						  PointArray &scale,
						  Array<Rotation> &rotate)
{

	float w= (float) part / (float) (totalPart); // don´t close 
	w*= TWOPI;
	w+= PI/2.0;

	float dw = DEG2RAD(15.0);
	int steps = TWOPI / dw;

	position.SetLength(steps);
	for(int i=0;i<steps;i++) {
		position[i]=p_circle((w+(float) i *dw) / TWOPI ,5.0);
	}
	rotate.SetLength(3);
	rotate[0].set(0,0,1,0);
	rotate[1].set(0,1,0,PI);
	rotate[2].set(0,0,1,0);

}

void ComputeKeys(int num,FloatArray &keys)
{
   float d= 1.0 / float (num-1);
   keys.SetLength(num);

   keys[0]=0.0;
   for (int i=1; i< (num-1); i++) {
	   keys[i] = float(i)*d;
   }	
   keys[num-1] = 1.0;
}

/*

  Create a new particle

*/


Gv2Transform *NewParticle(GvNode *object, 
						  PointArray &position,
						  PointArray &scale,
						  Array<Rotation> &rotate,
						  GvTimeSensor *timeSensor,
						  GvScene *scene
						  )
{
	Gv2Transform *t = new Gv2Transform();
	FloatArray keys;

	if (!object) return(NULL);
	ASSERT(object != NULL);


	t->addChild(object);
    
	if (position.Length()>0) {
		GvPositionInterpolator *pi = new GvPositionInterpolator();
		ComputeKeys(position.Length(),keys);
		pi->key.set(keys);
		pi->keyValue.set(position);
		t->addChild(pi);
		
		// add route timeSensor ==> interpolator ==> transform.
		scene->addRoute(pi,pi->getFieldIndex(&pi->value),t,t->getFieldIndex(&t->translation));
		scene->addRoute(timeSensor,timeSensor->getFieldIndex(&timeSensor->fraction),pi,pi->getFieldIndex(&pi->fraction));

	}

	if (scale.Length()>0) {
		GvPositionInterpolator *pi = new GvPositionInterpolator();
		ComputeKeys(scale.Length(),keys);
		pi->key.set(keys);
		pi->keyValue.set(scale);
		t->addChild(pi);
		// add route timeSensor ==> interpolator ==> transform.
		scene->addRoute(pi,pi->getFieldIndex(&pi->value),t,t->getFieldIndex(&t->scale));
		scene->addRoute(timeSensor,timeSensor->getFieldIndex(&timeSensor->fraction),pi,pi->getFieldIndex(&pi->fraction));

	}

	if (rotate.Length()>0) {
		GvOrientationInterpolator *pi = new GvOrientationInterpolator();
		ComputeKeys(rotate.Length(),keys);
		//pi->key.set(keys);
		pi->keyValue.set(rotate.Length(),rotate.Data());
		t->addChild(pi);
		// add route timeSensor ==> interpolator ==> transform.
		scene->addRoute(pi,pi->getFieldIndex(&pi->value),t,t->getFieldIndex(&t->rotation));
		scene->addRoute(timeSensor,timeSensor->getFieldIndex(&timeSensor->fraction),pi,pi->getFieldIndex(&pi->fraction));

	}

	return t;
}


Gv2Group* ParticleAnimation(GvMFNode *particles,
				  GvTimeSensor *timeSensor,
				  GvScene *scene
				  )
{
	Gv2Group *group;
    PointArray p;
    PointArray s;
    Array<Rotation> r;


	group = new Gv2Group();

	for(int i=0; i<particles->getNum(); i++) {
		Gv2Transform *partTransform;
		
		ComputeParticleMovement(i,particles->getNum(),p,s,r);

		partTransform = NewParticle(particles->get1(i),p,s,r,timeSensor,scene);

		if (partTransform) group->addChild(partTransform);

	}

	return(group);
}


GvScene* NewPartAnimation()
{

	int nparts = 4*3;

	
	GvScene *scene = new GvScene;
	GvNode *object = new GvBox();

	float duration = 20.0;

	GvTimeSensor *timeSensor = new GvTimeSensor();
	timeSensor->loop.set(TRUE);
	timeSensor->cycleInterval.set(duration);
	timeSensor->startTime.set(1);


	int ncolors=3;
	Point colors[3];
	int ntextures = 3;

	char *textures[] = {
						 "mandrill.bmp",
						 "brick.rgb",
						 ""
						};


	colors[0].Set(0.9, 0.4, 0.3);
    colors[1].Set(0.372549, 0.247059, 0.878431);
	colors[2].Set(0.882353, 0.439216, 0.000000);
	

	Gv2Material * material[3];

	int i;

	for (i=0; i<ncolors; i++) {
		material[i] = new Gv2Material();
		material[i]->diffuseColor.set(colors[i]);
#ifdef _OGL
		material[i]->specularColor.set(Point(0.9,0.9,0.9));
#endif
	}

	GvImageTexture *texture[3];

	for (i=0; i<ntextures; i++) {
		texture[i]= new GvImageTexture();
		//texture[i]->url.set(textures[i]);
	}

	int napps = ncolors; 
	GvAppearance *appearance[3];

	for (i=0; i<ncolors; i++) {
		appearance[i]= new GvAppearance();
		appearance[i]->texture.set(texture[1]);
		appearance[i]->material.set(material[i]);

	}
	

	scene->vrml2.set(TRUE);
	
	scene->addChild(timeSensor);


	GvMFNode parts(0);


	for(i=0; i<nparts; i++) {
		GvShape *s= new GvShape();
		s->geometry.set(object);
		s->appearance.set(appearance[rnd(napps)]);
		parts.append(s);
	}

	scene->addChild(ParticleAnimation(&parts,timeSensor,scene));

	return scene;

}


/*

  create a VRML 2.0 scene containing a grid of moving spheres
*/


GvScene *newSphereGrid()
{

	//if (f_rnd() <=0.1) return (NewPartAnimation());

	// size of grid 
	int xcnt=5; // 7 
	int ycnt=xcnt;

	float dx = 2.0 / xcnt;
	float dy = 2.0 / xcnt;

	int ncolors=3;
	Point colors[3];


	int ntextures = 3;

	char *textures[] = {
						 "mandrill.bmp",
						 "brick.rgb",
						 ""
						};


	colors[0].Set(0.9, 0.4, 0.3);
    colors[1].Set(0.372549, 0.247059, 0.878431);
	colors[2].Set(0.882353, 0.439216, 0.000000);
	
	Point pmin(-1,-1,0);
	Point pmax(1,1,0);
	Point jitterR;
	Point p;
	int i;
	
	jitterR.Set(dx * 0.5, dy * 0.5, 2*dx); // the amount each particles move about center

	// create top leve scene object 
	GvScene *scene = new GvScene;
	Gv2Sphere *object = new Gv2Sphere();
	GvTimeSensor *timeSensor = new GvTimeSensor();

	// create 3 VRML 2.0 materials for the 3 colors above
	Gv2Material * material[3];
	for (i=0; i<ncolors; i++) {
		material[i] = new Gv2Material();
		material[i]->diffuseColor.set(colors[i]);
		material[i]->specularColor.set(Point(1.0,1.0,1.0));
	}

	// create 3 VRML 2.0 image textures
	GvImageTexture *texture[3];

	for (i=0; i<ntextures; i++) {
		texture[i]= NULL;
		// new GvImageTexture();
		//texture[i]->url.set(textures[i]);
	}

	// create VRML 2.0 appearance nodes using the above materials 
	int napps = ncolors; 
	GvAppearance *appearance[3];

	for (i=0; i<ncolors; i++) {
		appearance[i]= new GvAppearance();
		appearance[i]->texture.set(texture[1]);
		appearance[i]->material.set(material[i]);

	}
	
 
	object->radius.set(dx *0.6) ; // * 0.77); // 1.11);

	scene->vrml2.set(TRUE);


	// create a grid of objects 
	int x,y;
	for (y=0; y<ycnt; y++) {
	for (x=0; x<xcnt; x++) {
		p= pmin;
		p.x += x * dx;
		p.y += y * dy;

		// create a transform node containing the moving shape
		Gv2Transform *t = new Gv2Transform();

		// vrml 2.0 needs a shape node containing the geometry and appearance nodes
		GvShape *s= new GvShape();
		// geometry is the reference to the sphere
		s->geometry.set(object);
		// choose among the appearances
		s->appearance.set(appearance[rnd(napps)]);
		// set the translation of the object
		t->translation.set(p);
		// add the shape
		t->addChild(s);

		// create a position interpolator
		GvPositionInterpolator *pi = new GvPositionInterpolator();
		PointArray vals;
		FloatArray keys;
		int njumps = 30+10;

		// compute keyframes for some movement
		int period = 20;
		for (int i=0; i<= njumps; i++) {
			Point px;
			float t = (float) (y * (xcnt-1) + x) / ((xcnt-1) * (ycnt -1));

			//if (i %6 ==0) px= p; 	else 
			if (i == njumps) px= p;
			else if (i%period == 2) px = p*1.5; 
			else if (i%period == 3) px = p*0.5; 
			else if (i%period == 4) px = p_quad(  t ,0.8);
			else if (i%period == 5) px = p_quad(  t ,1.2);
//			else if (i%period == 0) px = p_sphere(f_rnd(),f_rnd(),1);
			else if (i%period == 6) px = p_sphere( (float) x / (xcnt-1),(float) y / (ycnt-1),1.3);
			else if (i%period == 8) px = p_circle(  t ,1.1);

			else if (i%period == 9) px = p_circle(  t+0.2,1.1);
			else if (i%period == 10) px = p_circle(  t+0.4,1.1);
//rnd			else if (i%period == 11) px = p_circle(  t+0.6,1.2);
			else if (i%period == 12) px = p_circle(  t+0.8,1.1);
			else if (i%period >= 13) px = p_circle(  t *  (1.0+(i%period)-13)*0.2  ,1.2-((i%period)-13)*0.1, -((i%period)-13)*0.3);

			else px = p + jitter(jitterR);
			// add a keyframe
			vals.Append(px);
			keys.Append( (float) i /  njumps);
		}
		pi->keyValue.set(vals);
		pi->key.set(keys.Length(),keys.Data());

		if (0) {  
#if 0
				GvNurbsPositionInterpolator *nip =new GvNurbsPositionInterpolator();
				float scaleFactor = 1.0;
				int dimension, order;

				ComputeNurbsCurve (
					   pi->keyValue.getNum(),
					   pi->key,pi->keyValue,
					   scaleFactor,
					   dimension,order,
					   nip->knot,
					   nip->keyValue
					   );
				//nip->dimension.set(dimension);
				nip->order.set(order);
			t->addChild(nip);

			// connect the output from the position interpolator to the translation field of the transform		
			scene->addRoute(nip,nip->getFieldIndex(&nip->value),t,t->getFieldIndex(&t->translation));
		
			// connect the output from the timer to the fraction_changed innput of the timesensor
			scene->addRoute(timeSensor,timeSensor->getFieldIndex(&timeSensor->fraction),nip,nip->getFieldIndex(&nip->fraction));
			delete pi;
			pi=NULL;
#endif
		} else {
			t->addChild(pi);

			// connect the output from the position interpolator to the translation field of the transform		
			scene->addRoute(pi,pi->getFieldIndex(&pi->value),t,t->getFieldIndex(&t->translation));
		
			// connect the output from the timer to the fraction_changed innput of the timesensor
			scene->addRoute(timeSensor,timeSensor->getFieldIndex(&timeSensor->fraction),
				pi,pi->getFieldIndex(&pi->fraction));
		}
		// add the prepard transform to the scene
		scene->addChild(t);

	}
	}

	// set timesensor parameters
	timeSensor->loop.set(TRUE);		// loop for ever
	timeSensor->cycleInterval.set(25);	// 25 secs is one interval
	timeSensor->startTime.set(1);	// start time at beginning of scene
	scene->addChild(timeSensor);	
	
	// thats it, scene references all created nodes

	return scene;

}


void drawCone();
void drawPyramid();


GV_NODE_SOURCE_IMP(GView,GvNode);

GView::GView(/* GModel *Model */) : 
	locked(0),
	device(NULL),
	viewpoints(0),
	boundViewpointStack(0),
	boundNavigationInfoStack(0), 
	boundFogStack(0),
	boundBackgroundStack(0),
	overTouchSensors(0),
	activeTouchSensors(0),
	hasCustomSensorMessage(gfalse),
	activeVisibilitySensors(0),
	activeZones(0),
	protoScenes(0),
	pendingExternProtos(0),
	hasNewScene(FALSE),hasNewUrl(FALSE),
	newUrl(0),newUrlParams(0),
	pendingScripts(0),
	myAvatarAvatarSize(0),
	eventMask(0), // for now as SFInt32
	foreground(0)


#if defined(WIN32) && defined(_OGL)
	, lpDD(NULL)
#endif

{

#ifdef _GV
    GV_NODE_CONSTRUCTOR(GView);
    //GV_NODE_ADD_EVENT_IN(addChildren,GvMFNode);
    //GV_NODE_ADD_EVENT_IN(removeChildren,GvMFNode);

	setBrowser(this);

   // GV_NODE_ADD_FIELD(theScene);

    GV_NODE_ADD_FIELD(viewpoints);
    GV_NODE_ADD_FIELD(boundViewpoint);
    GV_NODE_ADD_FIELD(boundViewpointStack);
    GV_NODE_ADD_FIELD(boundNavigationInfo);
    GV_NODE_ADD_FIELD(boundNavigationInfoStack);
    GV_NODE_ADD_FIELD(boundFog);
    GV_NODE_ADD_FIELD(boundFogStack);
    GV_NODE_ADD_FIELD(boundBackground);
    GV_NODE_ADD_FIELD(boundBackgroundStack);
    GV_NODE_ADD_FIELD(name);
    GV_NODE_ADD_FIELD(version);
    GV_NODE_ADD_FIELD(worldUrl);
    GV_NODE_ADD_FIELD(overTouchSensors);
    GV_NODE_ADD_FIELD(activeTouchSensors);
    GV_NODE_ADD_FIELD(activeVisibilitySensors);
    GV_NODE_ADD_FIELD(activeZones);
    GV_NODE_ADD_FIELD(protoScenes);

    GV_NODE_ADD_FIELD(sharedZone);

    GV_NODE_ADD_EVENT_IN(set_myAvatarNodes,GvMFNode);
    GV_NODE_ADD_EVENT_IN(set_myAvatarURL,GvSFString);
    GV_NODE_ADD_EVENT_IN(set_myAvatarName,GvSFString);
    GV_NODE_ADD_EVENT_IN(set_showMyAvatar,GvSFBool);
    GV_NODE_ADD_EVENT_IN(set_myAvatarGesture,GvSFInt32);
    GV_NODE_ADD_EVENT_OUT(myAvatarGesture);

    GV_NODE_ADD_FIELD(myAvatarURL);
    GV_NODE_ADD_FIELD(boundAvatar);
    GV_NODE_ADD_FIELD(myAvatar);
    GV_NODE_ADD_FIELD(myAvatarName);
    GV_NODE_ADD_FIELD(myAvatarAvatarSize);

    GV_NODE_ADD_FIELD(followDolly);
    GV_NODE_ADD_FIELD(followOrbit);
    GV_NODE_ADD_FIELD(followPan);
    GV_NODE_ADD_FIELD(thirdPersonMode);


    GV_NODE_ADD_EVENT_OUT(time_changed);
    
	GV_NODE_ADD_EVENT_OUT(viewpointPosition);
	GV_NODE_ADD_EVENT_OUT(viewpointOrientation);
	GV_NODE_ADD_EVENT_OUT(windowSize);
	GV_NODE_ADD_EVENT_OUT(windowAspect);

    GV_NODE_ADD_FIELD(event);
    GV_NODE_ADD_FIELD(eventMask);

    GV_NODE_ADD_FIELD(foreground); // 4.001


	setName("Browser");
	name.set(_PROGRAM);
	version.set(_VERSION);

	boundViewpointChanged = 0;
	boundNavigationInfoChanged =0;
	boundFogChanged = 0;
	boundBackgroundChanged = 0;

	showAvatar = FALSE;
	libraryLoaded = FALSE;

	m_bTestFullScreen = FALSE;


#endif
#ifdef _COM
	observerFlags = 0;
	observerWnd = NULL;
#endif


	model = NULL;

	viewController = NULL; 

	hWnd  = NULL;

#ifdef _MFC
	pView = NULL;
	fullScreenWnd = NULL;
#endif

	
	doUpdateSceneInfo=0;
	doUpdateBBox=0; 

	//
	//the panel
	//
	panel = NULL;
	m_navPanelOn = TRUE;
	m_navPanelOn_Prop = TRUE;
	m_navPanelOn_Reg = TRUE;
	m_navPanelOn_NavInf = TRUE;

	//eventMask = GEV_ALL;


	deformer = 0; parameter = 0; 
	theWorld = 0;
	// theScene = NULL; //automatic
	camera = NULL;
	camera = new GCamera;
	globalCamera = new GCamera;

	thirdPersonView = FALSE;
	physicalCamera = new GCamera;
	physicalCameraInterpolator.alpha = 1.0f;

	//followDolly.set(0,0.5,-3);
	//followPan.set(0, -0.2,0);
	followOrbit.set(0,0,0);
	thirdPersonMode.set(1);

	followPan.set(0, -0.1,0);
	followDolly.set(0,0.3,-3);


	eventHandler = NULL;
	firstUpdate = 1;
	frameCnt = 0;
	lastFrameTime = 0;
	lastFrameTimesI=0;

	for(int i=0; i<NUM_LAST_FRAME_TIMES;i++) {
		lastFrameTimes[i]=0;
		lastFrameTris[i]=0;
		lastFramePrims[i]=0;
	}
	lastFrameTimesSum = 0;
	
	nurbsTessellationScale = 1.0f;
	targetFrameRate=20.0f;
	targetFrameRateDegrade=30.0f;
	autoTesselation = gtrue;
	nurbsTessellationMode = GNURBS_DYNAMIC_FRAMERATE_TESSELATION;
	lodScale = 1.0f;

#ifdef _OGL
	fullScreen = FALSE;
#endif

	SetCameraMatrixIdentity();

    cameraAnimate = 1; // use animate to viewpoint
    cameraInterpolator = NULL;
    cameraInterpolateAll = 0;
    cameraInterpolateAllCycle = 0;
    cameraInterpolateCurrent=-1;
    cameraInterpolateEnd= -1;
    viewpointIndex = -1;

	followObject = FALSE;
   
	bbox.SetEmpty();
	
	file = new GFile();
	file->ref();
	file->hPostMsgWnd = hWnd;


	soundDevice = NULL;
	m_playSoundStarted = FALSE;



	theShell = NULL;
	theWorld = NULL;
	theScene = NULL;
	sceneInfo = NULL;
	

	LightMode = 0;
	NormalMode = 0;
	NormalScale = 0.9;
	TextureMode = 0;
	TextureFilteringMode = 0;
	initialTextureFilteringMode = 0;
	TransparencyMode = 0;

	bspMode = FALSE;
	traversalOrder = TRAVERSE_FRONT_TO_BACK;
	traversalInlineLoadingOrder = TRAVERSE_FRONT_TO_BACK;

	viewCulling = TRUE;

	activeInlinesLimit = 200;
	activeInlinesPurge = 0.01; // percent 


	backupShell = 0;

	useGlCache=0;

	headLightOn= TRUE;

	useSceneLights=TRUE; // use lights in scene
	useSceneSpotLights=TRUE; // use spot lights
	useInitialSceneCamera=1; // use initial scene camera

	allowAnyNavigation = TRUE;
	
	enableCollisionDetection= TRUE; 
	enableStayOnGround = FALSE; 

	lightScale = 1.0;

	//RAPID
	useRapid = gtrue;

	speedBox = NULL;
	speedBoxGlobal = NULL;
	myRapid = NULL;
	speedBoxVolume = NULL;



	vrml2 = 0; // to do: clean meaning of this flag

	useTextures = 1;
	useMaterials = TRUE;
	useMovies = TRUE;
	useShapeHints = TRUE;
	useBackground = TRUE;
	hasSoundDevice = TRUE;
	useSound = TRUE;
	
	// sound defaults 
	useRSXSound = FALSE;
	// useRSXSound = TRUE; // could check for DSound D3DSound 

	hasRSXSound = TRUE; // assume for now 
	soundQuality = GS_QUALITY_MAX;
	maxNumSounds = 8;
	soundDoppler = FALSE;

	maxUserTextureSize=0;

	reportErrors = TRUE;
#ifdef _GLVIEW_IE
	//reportErrors = FALSE;
#endif

	maxInlineDepth = 16;
	incrementalUpdates = 1;
	
	inlineLoadMode=GTraversal::LOAD_IF_NEEDED;
	textureLoadMode=GTraversal::LOAD_IF_NEEDED;
	movieLoadMode=GTraversal::LOAD_IF_NEEDED;
	soundLoadMode=GTraversal::LOAD_IF_NEEDED;

	inlineWwwFlags = WWW_REPORT_ERRORS;

#ifdef _GLVIEW_IE
#ifndef _DEBUG

	inlineWwwFlags  = 0; // don`t report errors 
#endif

#endif
	
	ClearCounts();
	maxFrameInlineLoadCnt = 10;
	maxFrameScriptLoadCnt = 100;
	maxFrameTextureLoadCnt = 50;
	maxFrameSoundLoadCnt = 100;

	anchorPreloadMode=GTraversal::DONT_LOAD; // not implemented yet
	initialTextureMode=GTraversal::LOAD_IF_NEEDED;
	initalRenderingMode=GRenderGouraud;
	

	resetZoomFactor = 1.2;
	ignoreAllInlines = 0;
	viewStepSpeed = 1.0;
	viewAngleSpeed = 1.0;

	playRealtime = 1;
	time.enabled = 0;
	thisFrameTimeSensorsTriggered = FALSE;

	//	time.now = GTime::GetGlobalTime();
	loadTime = GTime::GetGlobalTime();
	autoPlay = 1;

    doBackground = 1;
	currentTraversal = NULL;
	lastHit = NULL;
	executeNode = NULL;

    needUpdate = 0;
	triggerUrlLoading = FALSE;
	updateIsComplex = TRUE;
	moving = 0;
	startRenderTime = 0.0;

	autoDegrade=0;
	SetDegradeDefaults();
	degradeDuringMove=1;
	degraded = 0;
	degrade = 0;
	degradeLastFrameTime = 0.0;
	degradeMaxFrameTime = 1.0;	// for interactive work one second

	collisionDetection = enableCollisionDetection;
	stayOnGround = enableStayOnGround;
	downDirection.Set(0,-1,0);
	collisionDistance = 0.25;
	heightOverGround = 1.6;
	stepOverSize = 0.75; 
	visibilityLimit = 0.0;
	viewStepSpeed = 1.0;

	// set browser event object

	GvEvent* ev= new GvEvent();
	ev->setBrowser(this);
	event.set(ev);

    tasks = NULL;
};

#ifdef _DEBUG

BOOL GView::Lock() {
	//TRACE("GView::Lock Locked = %d  TId is %X \n",locked,GetCurrentThreadId());
	locked++;  
	return locker.Lock(); 
}
BOOL GView::Unlock() {
	// TRACE("GView::Unlocking \n");
	locked-- ; 
	return locker.Unlock(); 
}
#endif

/*

  Terminate view, restart with Initialize
  free up major resources, including D3D device and scene


*/
void GView::Term()
{
	TRACE("GView::Term() refCnt = %d\n",refCnt);

#ifdef _COM
	observer.Release();
	observerFlags = 0;
	observerWnd = NULL;
#endif


	BindAvatar(NULL);

	worldUrl.deleteConnections();
	viewpointPosition.deleteConnections();
	viewpointOrientation.deleteConnections();
	windowSize.deleteConnections();
	windowAspect.deleteConnections();
	time_changed.deleteConnections();


	// if (GvScene::getCurrent() == theScene) GvScene::setCurrent(NULL);
	//GvScene::setCurrent(NULL); // for now, what if multiple instances ?

	// to do :
	// check all scripts, delete Browser vars, clean up recursive USE 
	SetScene(NULL); // set to dummy scene 

	ReleaseSound();

#if defined(WIN32) && defined(_OGL)
	if (lpDD) lpDD->Release();
	lpDD = NULL;
#endif


	//RAPID
	if (speedBox) {
#ifndef _DEBUG
		// speedbox already inserted into myavatar for debug purposes (see BindAvatar) TBD: may clean this up
		delete speedBox;
#endif
		speedBox = NULL;
	}
	if (speedBoxGlobal) {
		delete speedBoxGlobal;
		speedBoxGlobal = NULL;
	}
	if (myRapid ){
		delete myRapid;
		myRapid = NULL;
	}

	if (speedBoxVolume ){
		delete speedBoxVolume;
		speedBoxVolume = NULL;
	}


	FlushEventHandlers();
	FlushBoundedNodes();
	myAvatar.set((GvNode *) NULL);
	FlushViewpoints();

#ifdef G_PANEL

	if (panel) {
		delete panel;
	}
	panel = NULL;

#endif

    if (cameraInterpolator) delete cameraInterpolator;
	cameraInterpolator = NULL;
    if (sceneInfo) delete sceneInfo; 
	sceneInfo = NULL;

	FlushProtoScenes();

	
	ClearTasks();

	if (lastHit) delete lastHit;
	lastHit = NULL;

#ifdef _D3D
	if (device) {
		// destroy D3D device
		device->Destroy();
		delete device;
		device = NULL;
	}
#endif

#ifdef _OGL
	if (device) {
		device->Destroy();

		delete device;
		device = NULL;
	}
#endif


};


// Destructor
GView::~GView () 
{

	TRACE("~GView\n");
	refCnt++; // prevent recursive delete requests

#ifdef _COM
	observer.Release();
	observerFlags = 0;
	observerWnd = NULL;
#endif

//	if (fullScreenWnd)
//		delete fullScreenWnd;

	worldUrl.deleteConnections();
	viewpointPosition.deleteConnections();
	viewpointOrientation.deleteConnections();
	windowSize.deleteConnections();
	windowAspect.deleteConnections();
	time_changed.deleteConnections();
	
	boundViewpoint.deleteConnections();

	ReleaseSound();

	FlushEventHandlers();
	FlushBoundedNodes();
	FlushViewpoints();

    if (cameraInterpolator) delete cameraInterpolator;
	cameraInterpolator = NULL;
    if (sceneInfo) delete sceneInfo;
	sceneInfo = NULL;

	FlushProtoScenes();

	ClearTasks();

#ifndef  _GV_MINIMAL
	if (deformer) delete deformer;
	if (parameter) delete parameter;
#endif

	if (theShell) delete theShell;
	if (backupShell) delete backupShell;
	if (camera) delete camera;
	if (globalCamera) delete globalCamera;
	if (physicalCamera) delete physicalCamera;

#ifdef _GFULL
	if (theWorld) delete theWorld;
#endif
	
	theScene = NULL;

	if (file) {
		file->unref();
		file=NULL;
	}

//	if (backgroundShell) delete backgroundShell;

	if (lastHit) delete lastHit;
	lastHit = NULL;

	refCnt--; // prevent recursive delete requests

	// Automatic renderState, modelCache
 };


  
//displays a message to the status bar with update
void GView::Message(const char *message,int messageType)
{ 
	if (viewController) viewController->Message(message,messageType); 
}

//print a message to the console
void GView::Print(const char *message)
{ 
	if (viewController) viewController->Print(message); 
}


// get the type of the last message 
int GView::GetLastMessageType() 
{ 
   if (!viewController) return(0);
   return viewController->GetLastMessageType(); 
} 


// get the error report object 
GReporter* GView::GetReporter()
{
  if (viewController) return viewController->GetReporter();
  return GReporter::current;
}


// mark view for redraw 
int GView::Redraw()
{
  if (viewController) 
	  viewController->Redraw();

  return (0);
}

// mark view for redraw
void GvNode::setRedraw()
{
	GView * view = (GView*) getBrowser();
	if (view) view->setRedraw();
}

	
// get redraw flag
GvBool GvNode::getRedraw()
{
	GView * view = (GView*) getBrowser();
	if (!view) return FALSE;
	return view->getRedraw();
}


// get current event simulation time 
double GvNode::getTime()
{
	GView * view = (GView*) getBrowser();
	if (!view) return 0;
	return view->getTime();
}	
 
// get pointer to variable where current event simulation time is stored
double* GvNode::getTimePtr() 
{
	GView * view = (GView*) getBrowser();
	if (!view) return NULL;
	return view->getTimePtr();
}	



int GView::CheckEvents()
{
  return(0);
}


// query functions for button checking
gbool GView::GetDithering() { return(renderState.dither);}
gbool GView::GetLightMode() { return (LightMode); }
gbool GView::GetNormalMode() { return (renderState.normals.visible); }
gbool GView::GetBackfaceCulling() {return(renderState.backfaceCulling); }
gbool GView::GetFrontFaceCW() {return(renderState.frontFaceCW); }
gbool GView::GetLocalViewer() { return(renderState.localViewer);}
gbool GView::GetLocalColors() {	return(renderState.localColors); }
gbool GView::GetTexturing() {  return(TextureMode); }
gbool GView::GetTransparency() {  return(TransparencyMode); }
gbool GView::GetLineAntialiasing() { return(renderState.lineSmooth);}


gbool GView::GetAntiAliasing() 
{ 
#ifdef _D3D
	return renderState.GetAntialiasing();	
#else 
	return(renderState.lineSmooth);
#endif

}

int   GView::GetRenderMode() { return(renderState.renderMode); }
gbool GView::GetTwoSidedLighting() {return(renderState.twosidedLighting);}
gbool GView::GetComputePrimitiveList() {return(renderState.computePrimitiveList);}


#ifndef _GV_MINIMAL

// Interface for Material edit

int GView::SetMaterial(Point &Ambient, Point &Diffuse, Point &Specular,Point &Emission,float &Shininess,float &Alpha)
{
#ifdef _D3D
renderState.defaultMaterialValue.Set(Ambient,Diffuse,Specular,Emission,Shininess,Alpha);
renderState.SetMaterial();
#else
  renderState.material.Set(Ambient,Diffuse,Specular,Emission,Shininess,Alpha);
#endif
UpdateDefaultNodes();
  return(1);
}

int GView::GetMaterial(Point &Ambient, Point &Diffuse, Point &Specular,Point &Emission,float &Shininess,float &Alpha)
{
#ifdef _D3D
  renderState.defaultMaterialValue.Get(Ambient,Diffuse,Specular,Emission,Shininess,Alpha);
#else
  renderState.material.Get(Ambient,Diffuse,Specular,Emission,Shininess,Alpha);
#endif
  return(1);
}

int GView::SetBackMaterial(Point &Ambient, Point &Diffuse, Point &Specular,Point &Emission,float &Shininess,float &Alpha)
{
  renderState.backMaterial.Set(Ambient,Diffuse,Specular,Emission,Shininess,Alpha);
  return(1);
}

int GView::GetBackMaterial(Point &Ambient, Point &Diffuse, Point &Specular,Point &Emission,float &Shininess,float &Alpha)
{
  renderState.backMaterial.Get(Ambient,Diffuse,Specular,Emission,Shininess,Alpha);
  return(1);
}

int GView::ReadMaterial(const char * filename, int format)
{
  return(0);
}

int GView::EditBackgroundColor(const Point &c)
{
  int ret = SetBackgroundColor(c);
  if (ret>0) {
      char s[120];
  	  sprintf(s,"%f %f %f", c.x,c.y,c.z);
      if (sceneInfo)       
        if (sceneInfo->backgroundColor) {
            sceneInfo->backgroundColor->string= s;
        } else {
           sceneInfo->backgroundColor = new GvInfo();
           sceneInfo->backgroundColor->string= s;
           sceneInfo->backgroundColor->setName(GvName("BackgroundColor"));
           if (sceneInfo->top && RTISA(sceneInfo->top,GvGroup)) RTCAST(sceneInfo->top,GvGroup) ->addChild(sceneInfo->backgroundImage);
        }
  }  
  return(1);
}
#endif

int GView::SetBackgroundColor(const Point &c)
{
  renderState.SetBackgroundColor(c);
  return(1);
}

int GView::GetBackgroundColor(Point &c)
{
  //renderState.GetBackgroundColor(c);
  renderState.background.Get(c);
  return(1);
}

// Ambient Light 
int GView::GetAmbientLight(CString &c)
{
  RGBAColor &a = renderState.ambient;
  char s[120];
  sprintf(s,"%g %g %g %g",a.r,a.g,a.b,a.a);	
  c=s;
  return(1);
}

int GView::SetAmbientLight(const char *s)
{
	RGBAColor a; // = renderState.ambient;
	sscanf(s,"%g %g %g %g",&a.r,&a.g,&a.b,&a.a);	
	renderState.SetAmbientColor(a);

	return(1);
}


gbool GView::SetNormalMode(gbool mode )
{
	NormalMode = mode;
	renderState.normals.visible = NormalMode;
	return(NormalMode);
}

gbool GView::SetDithering(gbool mode)
{
	renderState.SetDither(mode);
	return(renderState.dither);
}

gbool GView::SetLightMode(gbool mode)
{
	LightMode = mode;
	renderState.LightMode = LightMode;
	renderState.SetRenderMode(renderState.renderMode);
	return(LightMode);
}

gbool GView::SetBackfaceCulling(gbool mode)
{
#ifdef _D3D
	if (renderState.backfaceCulling != mode && !renderState.canCull) {
		renderState.ReleaseExBufs();
	    updateIsComplex = TRUE;
	}
#endif

  renderState.SetBackfaceCulling(mode);
  return(renderState.backfaceCulling);
}


gbool GView::SetFrontFaceCW(gbool mode)
{
#ifdef _D3D
	if (renderState.frontFaceCW != mode && !renderState.canCull) {
	    updateIsComplex = TRUE;
		renderState.ReleaseExBufs();
	}
#endif
  renderState.SetFrontFaceCW(mode);
  return(renderState.frontFaceCW);
}

gbool GView::SetLocalViewer(gbool mode)
{
  renderState.SetLocalViewer(mode);
  return(renderState.localViewer);
}

gbool GView::SetTwoSidedLighting(gbool mode)
{
  renderState.SetTwoSidedLighting(mode);
  return(renderState.twosidedLighting);
}

void GView::SetTargetFrameRate(float targetFR)
{
	targetFrameRate = FuzzyLogic.Initialize(targetFR);
}

gbool GView::SetLocalColors(gbool mode)
{
  renderState.SetLocalColors(mode);
  renderState.faces.colored = renderState.localColors;
  renderState.edges.colored = renderState.localColors;
  renderState.vertices.colored = renderState.localColors;
  
  // otherwise	a line is not smooth color interpolated without lighting ???
  if (renderState.localColors) {
	   renderState.SetLightModel(GL_SMOOTH);
  }
  return(renderState.localColors);
}

gbool GView::SetTexturing(gbool mode)
{
  TextureMode = mode;

  renderState.SetTexturing(mode);
  renderState.faces.textured = renderState.texturing;
  renderState.edges.textured = renderState.texturing;
  renderState.vertices.textured = renderState.texturing;
  if (renderState.faces.visible)
      renderState.edges.textured = 0;
  return(TextureMode);
}
// set texture filtering 0 : nearest : 1 : bilinear
int GView::SetTextureFiltering(int mode)
{
  TextureFilteringMode = mode;

  if (mode ==1) {
	renderState.textureParameters.SetMinFilter("Linear");
	renderState.textureParameters.SetMagFilter("Linear");
  } else {
  	renderState.textureParameters.SetMinFilter("Nearest");
	renderState.textureParameters.SetMagFilter("Nearest");
  }

#ifdef _OGL
   // texture filtering goes into texture object
   // renderState.ReleaseTextures(); this would make alle textures invalid and thez are not reloaded
  renderState.SetTextureParameters();
  renderState.FlushState();
  renderState.UpdateTextures();

#endif
#ifdef _D3D
	if (renderState.lpD3D) {
		renderState.SetTextureParameters();
		renderState.FlushState();
	}

#endif


  return TextureFilteringMode;
}

int GView::SetTextureMipMap(int mode) 
{

#ifdef _OGL
      // texture filtering goes into texture object
	  //if (!renderState.textureParameters.IsMipmaped() && mode)
	  //	 renderState.ReleaseTextures(); // this would make alle textures invalid and thez are not reloaded
	  
	  if (mode) {
		if (TextureFilteringMode) {
				renderState.textureParameters.min_filter = renderState.maxMipfilter; // (D3DFILTER_LINEARMIPLINEAR);
				renderState.textureParameters.mag_filter=(GL_LINEAR);
		}	
		else {
			if (renderState.canDoMipMap) 			
				renderState.textureParameters.min_filter=(GL_NEAREST_MIPMAP_NEAREST);
			else renderState.textureParameters.min_filter=(GL_NEAREST);
			renderState.textureParameters.mag_filter=(GL_LINEAR);
		}
		

	  } else {
		if (TextureFilteringMode) {
				renderState.textureParameters.min_filter=(GL_LINEAR);
				renderState.textureParameters.mag_filter=(GL_LINEAR);
		}	
		else { 
			renderState.textureParameters.min_filter=(GL_NEAREST);
			renderState.textureParameters.mag_filter=(GL_NEAREST);
		}
	  }

  	  renderState.SetTextureParameters();
	  renderState.FlushState();
	  renderState.UpdateTextures();

#endif
#ifdef _D3D
	if (renderState.lpD3D) {
	  if (mode) {
		if (TextureFilteringMode) {
				renderState.textureParameters.min_filter = renderState.maxMipfilter; // (D3DFILTER_LINEARMIPLINEAR);
				renderState.textureParameters.mag_filter=(D3DFILTER_LINEAR);
		}	
		else {
			if (renderState.canDoMipMap) 			
				renderState.textureParameters.min_filter=(D3DFILTER_MIPNEAREST);
			else renderState.textureParameters.min_filter=(D3DFILTER_NEAREST);
			renderState.textureParameters.mag_filter=(D3DFILTER_LINEAR);
		}
		

	  } else {
		if (TextureFilteringMode) {
				renderState.textureParameters.min_filter=(D3DFILTER_LINEAR);
				renderState.textureParameters.mag_filter=(D3DFILTER_LINEAR);
		}	
		else { 
			renderState.textureParameters.min_filter=(D3DFILTER_NEAREST);
			renderState.textureParameters.mag_filter=(D3DFILTER_NEAREST);
		}
	  }

  	  renderState.SetTextureParameters();
	  renderState.FlushState();
	}

#endif
	return mode;

}
int GView::GetTextureMipMap()
{
#ifdef _OGL
	if (renderState.textureParameters.IsMipmaped())
		return 1;
#endif

#ifdef _D3D
	if (renderState.textureParameters.min_filter >= D3DFILTER_MIPNEAREST)
		return 1;
#endif
	return 0;
}


gbool GView::SetTransparency(gbool mode)
{
  TransparencyMode = mode;
  return(TransparencyMode);
}



gbool GView::SetLineAntialiasing(gbool mode)
{
  renderState.SetLineAntialiasing(mode);
  return(renderState.lineSmooth);
}


gbool GView::SetAntiAliasing(gbool mode)
{
#ifdef _D3D	
	 
	renderState.SetAntialiasing(mode);
	return renderState.GetAntialiasing();
	 // if (mode)  renderState.SetRenderState(D3DRENDERSTATE_ANTIALIAS,D3DANTIALIAS_SORTINDEPENDENT);
	 // else renderState.SetRenderState(D3DRENDERSTATE_ANTIALIAS,D3DANTIALIAS_NONE);
#endif

#ifdef _OGL
	renderState.SetLineAntialiasing(mode);
#endif

  return(renderState.lineSmooth);
}


gbool GView::SetComputePrimitiveList(gbool mode)
{
  renderState.SetComputePrimitiveList(mode);
  if (GetComputePrimitiveList())
	  GShell::initial_flags  |= SHELL_COMPUTE_PRIMLIST;
  else GShell::initial_flags  &= ~SHELL_COMPUTE_PRIMLIST;
  return(renderState.computePrimitiveList);
}


// set / get lightScale factor
int GView::SetLightScale(float factor) 
{
  lightScale = factor;
  renderState.SetLightScale(factor);
  return(1);
}

float GView::GetLightScale() 
{
   return(lightScale);
}

void  GView::SetUseTextures(gbool flag)
{
	if (flag == useTextures) return;
	useTextures = flag;
	if (!flag) {
#ifdef _OGL
		renderState.ReleaseTextures();
#else
		ReleaseTextures();
#endif
		textureLoadMode=GTraversal::DONT_LOAD;
	} else {
		textureLoadMode=GTraversal::LOAD_IF_NEEDED;
	}

}

void  GView::SetUseMovies(gbool flag)
{
	if (flag == useMovies) return;
	useMovies = flag;
	if (!flag) {
		//ReleaseTextures();
		setRedraw();
		movieLoadMode=GTraversal::DONT_LOAD;
	}
	else {
		movieLoadMode=GTraversal::LOAD_IF_NEEDED;
	}

}

//! set user lod scale 
void GView::SetLodScale(float factor) 
{
	if (lodScale == factor) return;
	if (factor <= 0.001f) return;

	lodScale = factor;
	if (camera && visibilityLimit >0.0f) {
		camera->zfar = visibilityLimit*lodScale;
	}
	setRedraw();
}


/*
void  GView::SetUseSound(gbool flag)
{
	if (flag == useSound) return;
	useSound= flag;
	// if (!flag) 		ReleaseTextures();
}

*/


void  GView::SetMaxUserTextureSize(int mode)
{
	if (mode == maxUserTextureSize) return;
	maxUserTextureSize = mode;
	

	int sx,sy;
	int msx,msy;
	int maxs = 1<<(maxUserTextureSize-1);
		
//	renderState.textureParameters.GetCurrentMaxSize(sx,sy);
	renderState.GetTextureMaxSize(msx,msy);
	if (maxUserTextureSize<=0) // use hw max
		maxs=msx;

	sx = min(msx,maxs); 		sy = min(msy,maxs);
	renderState.SetTextureCurrentMaxSize(sx,sy);

	renderState.ReleaseLargeTextures(999);

}



// simple toggle funcations for user interface callbacks
int GView::ToggleLightMode() {
  LightMode = ! LightMode;
  renderState.LightMode = LightMode;
  renderState.SetRenderMode(renderState.renderMode);

  return(LightMode);
}

int GView::ToggleNormalMode() {
  NormalMode = ! NormalMode;
  renderState.normals.visible = NormalMode;
  return(NormalMode);
}

int GView::ToggleBackfaceCulling() {
  SetBackfaceCulling(!renderState.backfaceCulling);
  return(renderState.backfaceCulling);
}

int GView::ToggleFrontFaceCW() {
  SetFrontFaceCW(!renderState.frontFaceCW);
  return(renderState.frontFaceCW);
}

int GView::ToggleLocalViewer() {
  SetLocalViewer(!renderState.localViewer);
  return(renderState.localViewer);
}

int GView::ToggleTwoSidedLighting() {
  SetTwoSidedLighting(!renderState.twosidedLighting);
  return(renderState.twosidedLighting);
}

int GView::ToggleLocalColors() {
  renderState.SetLocalColors(!renderState.localColors);
  renderState.faces.colored = renderState.localColors;
  renderState.edges.colored = renderState.localColors;
  renderState.vertices.colored = renderState.localColors;
  // otherwise	a line is not smooth color interpolated without lighting ???
  if (renderState.localColors) {
	   renderState.SetLightModel(GL_SMOOTH);
  }
  return(renderState.localColors);
}

int GView::ToggleTexturing() {
  TextureMode = !TextureMode;
  SetTexturing(TextureMode);
  return(TextureMode);
}


int GView::ToggleTransparency()
{
  TransparencyMode = !TransparencyMode;
  return(TransparencyMode);
}


int GView::ToggleLineAntialiasing() 
{
  renderState.SetLineAntialiasing(!renderState.lineSmooth);
  return(renderState.lineSmooth);
}

int GView::ToggleComputePrimitveList() {
  renderState.SetComputePrimitiveList(!renderState.computePrimitiveList);
  return(renderState.computePrimitiveList);
}

// set global render mode to mode (enum)
int GView::SetRenderMode(GRenderMode newMode)
{
 
 InvalidateModelCache();

#ifdef _D3D
 gbool oldUnlit = renderState.unlit;

 renderState.SetRenderMode(newMode);
 
 if (renderState.unlit != oldUnlit) {	// need to rebuild execute buffers 

	 renderState.FlushState();

	 renderState.ReleaseLights();
	 renderState.ReleaseMaterials();
	 renderState.ReleaseExBufs(); // was uncommented 
	 renderState.ReleaseVBBufs(); // was uncommented 
	 //renderState.ReleaseTextures();
	 updateIsComplex = TRUE;
 }	
#else 
 renderState.SetRenderMode(newMode);
#endif

 return(0);
}

// set global render mode to mode
int GView::SetRenderMode(const char * newMode)
{
	CString tmp(newMode);
	tmp.MakeLower();
    GRenderMode mode = (GRenderMode) GRenderModeLookup(tmp);
	SetRenderMode(mode);
	Redraw();
	return mode;
}	

void GView::SetBspMode(gbool mode)
{
	bspMode = mode;
	if (bspMode) {
		traversalOrder = TRAVERSE_BACK_TO_FRONT;
		renderState.SetZBuffer(0);
		renderState.SetZWrite(0);

	} else 
	{
		renderState.SetZBuffer(1);
		renderState.SetZWrite(1);
		traversalOrder = TRAVERSE_FRONT_TO_BACK;
	}
}

void GView::SetDefaultTextureParameters()
{
#ifdef _OGL
	   if (theShell && theShell->vp.Length() == 0 ) {
		  if (renderState.textureGeneration.isOff)
			  renderState.textureGeneration.Set("xyz");
	   } else {
		  if (!renderState.textureGeneration.isOff)
			  renderState.textureGeneration.Set("off");
	   }
	   renderState.textureGeneration.glSet();
#endif

#ifdef _D3D
	   if (theShell && theShell->vp.Length() == 0 ) {
            PointArray vp;
            // compute default texture coordinates based on Bounding Box for a set of points
            ComputeBoxParams(theShell->GetBBox(),theShell->GetV(),vp);
            theShell->SetVP(vp);
       }
#endif


}



void GView::FlushBackupShell()
{
  if (backupShell) {
    delete backupShell;
	backupShell = 0;
  }
}

// set camera transform
void GView::SetCameraMatrix(const Matrix &m)
{
	cameraMatrix.Set(m);
}

void GView::SetCameraMatrixIdentity()
{
	cameraMatrix.SetIdentity();
}

// update global camera from camera + cameraMatrix
void GView::UpdateGlobalCamera()
{

	// ******* compute global camera 
	*globalCamera = *camera;
	
	if (!cameraMatrix.IsIdentity()) {
		globalCamera->Transform(cameraMatrix.M());
		// ??? UpdateGlobalNavigationValues();
	}
}

// update default z-range of global camera 
void GView::UpdateDefaultZRange()
{
	GCamera *camera = GetPhysicalCamera();

	if (!autoZRange) return; // Navigation info specified a visibiliy limit 
	
	if (doUpdateBBox)
	  UpdateBoundingBox();

	if (bbox.IsEmpty()) return;

	float znear = this->camera->znear;
	float zfar = this->camera->zfar;


	Point size = bbox.Size();
  	
	float field = max(max(fabs(size.x),fabs(size.y)),fabs(size.z));
    if (field <= EPS) return;

 	gbool positionInBox = bbox.Inside(camera->position);

	if (positionInBox) {
		Point d = bbox.Center() - camera->position ;

		// distance to center + radius of bbox 
		zfar = d.Length() + size.Length()*0.5;

		if ((visibilityLimit > 0.0) && (visibilityLimit > (zfar *0.001f))) {
			zfar = visibilityLimit * lodScale;  // take users value from navigation Info
		}
		znear = zfar * camera->znearFactor;
		if (visibilityLimitNear > 0) {
			//znear =0.5f * collisionDistance
			znear = visibilityLimitNear;
		}
	} 
	else { // viewer outside, adjust so that it can be viewed from far away
		Point d = bbox.Center() - camera->position;
		float dist = d.Length(); // distance viewer center
		float r =  size.Length(); // radius of bounding sphere 
		zfar = dist + r*0.7;
#if 0 //changed at 19.01.2000 due to problems with cybelius objects	
		// problem BBox not up tp date , using old code 
		znear = zfar * camera->znearFactor;

		if (visibilityLimitNear > 0) {
			//znear = min(0.5f * collisionDistance,znear);
			//znear = 0.5f * collisionDistance;
			// if (observer != NULL) == > MU
			znear = max(znear,visibilityLimitNear);
			// znear = visibilityLimitNear;
		}
#else
		znear = dist -r *0.5;
		if (znear <= 0) znear = zfar * camera->znearFactor;
#endif
	}
	camera->zfar = zfar;
	camera->znear = znear;
	camera->OnChanged();
	if (camera != globalCamera) {
		globalCamera->zfar = zfar;
		globalCamera->znear = znear;
		globalCamera->OnChanged();
	}
	//TRACE("Auto z %g - %g %g\n",znear,zfar, znear/zfar);
}

// update the camera used for VRML 2.0 Background Node 
void GView::UpdateBackgroundCamera(Vector axis,float angle)
{
	// realign current transformation of VRML 2.0 background node with new camera 
	if ( backgroundRotation != (GvNode*) NULL) {

		if (thirdPersonView) { // need physical camera rotation
			ConvertCameraToOrientation(physicalCamera->position, physicalCamera->target,physicalCamera->up,axis, angle);
		}
		
		GvRotation *r = (GvRotation*) (GvNode*) backgroundRotation;

		Rotation rot(axis.x,axis.y,axis.z,-angle); // must use inverted rotation 5.05.97
		
		r->rotation.set(rot);
		
		GvLayer *layer = (GvLayer *) (GvNode *) background;
		
		GCamera *theCamera = &layer->layerCamera;
		
		GCamera *c = GetPhysicalCamera();
		// take view angle constant 
		float field = 2.0;
		float targetDistance = field / (2.0f * tan(c->GetFovY()*0.5f));
		Point dir ( 0,0,-1);

		theCamera->position.Set(0,0,0);
		theCamera->height = field;
		theCamera->width = field;
		//theCamera->width = theCamera->height * c->aspectRatio;
		theCamera->target = theCamera->position + targetDistance*dir;
		theCamera->SetAspect(c->aspect);
		theCamera->SetFieldPixel(renderState.renderSizex,renderState.renderSizey );

	    theCamera->OnChanged();
	}
}

// transform / scale a value by transform matrix
// is this good ?
float TransformValue(const Matrix &m, float value)
{

	Point v(value,0,0);

	v = RotateOnly(m,v);
	return v.Length();
}

// update navigations values from  cameraMatrix
void GView::UpdateGlobalNavigationValues()
{
	if (!cameraMatrix.IsIdentity()) { 

	   visibilityLimit = TransformValue(cameraMatrix.M(),visibilityLimitLocal);
	   visibilityLimitNear = TransformValue(cameraMatrix.M(),visibilityLimitNearLocal);
	   collisionDistance = TransformValue(cameraMatrix.M(),collisionDistanceLocal);
	   heightOverGround	= TransformValue(cameraMatrix.M(),heightOverGroundLocal);
	   stepOverSize= TransformValue(cameraMatrix.M(),stepOverSizeLocal);
/*

	   visibilityLimit = visibilityLimitLocal;
	   collisionDistance = collisionDistanceLocal;
	   heightOverGround	= heightOverGroundLocal;
	   stepOverSize= stepOverSizeLocal;
*/

	   downDirection = RotateOnly(cameraMatrix.M(),downDirectionLocal);

	   downDirection.Normalize();
	} else {
	   visibilityLimit = visibilityLimitLocal;
	   visibilityLimitNear = visibilityLimitNearLocal;
	   collisionDistance = collisionDistanceLocal;
	   heightOverGround	= heightOverGroundLocal;
	   stepOverSize= stepOverSizeLocal;
	   downDirection = downDirectionLocal;
	}
	
	autoZRange = (visibilityLimit <= 0.0f);

}


void GView::CameraReset()
{
    BindViewpoint(NULL);

	camera->SetByVolume(-1.0,1.0,-1.0,1.0);
	
	SetCameraMatrixIdentity();

	if (theShell) {
	   BBox box;
	   theShell->ComputeBBox(box);
	   bbox = box;
	   if (!box.IsEmpty()) {
	   	 camera->SetByVolume(box.mi,box.ma);
	   	 camera->Zoom(1.2);
	   }
	   camera->SetWorldReference(bbox);
       camera->isDefault = 1;
	} else 
	if (theWorld || theScene) {
	  GGvBBoxTraversal bboxTraversal;
 	  bboxTraversal.view = this;
	  bboxTraversal.SetCamera(globalCamera);
 	  bboxTraversal.SetPhysicalCamera(GetPhysicalCamera());

	  bboxTraversal.traverseAll = gtrue;
	  bboxTraversal.traverseTransformHierarchyOnly=gtrue;

   	  bboxTraversal.InitializeAttributes();

	  // set current time
	  GTimeStack *ts= GTimeStack::NewElement();
	  ts->time = time; 
	  bboxTraversal.Set(ts); 


	  Traverse(bboxTraversal);

	  bbox = bboxTraversal.bbox;
	  
	  	  
	  if (!bbox.IsEmpty()) {
	  	camera->SetByVolume(bboxTraversal.bbox.mi,bboxTraversal.bbox.ma);
	  	camera->Zoom(1.2);
		camera->SetWorldReference(bbox);
        camera->isDefault = 1;
	  }
	}
}

int  GCamera::Set(GvPerspectiveCamera *c,const Matrix *cameraTransform)
{ 
 return(0);
}

int  GView::AnimateToCamera(GvNode *cameraNode,const Matrix *cameraTransform)
{ 
	BindViewpoint(NULL);
    if (!cameraInterpolator) {
        cameraInterpolator = new GCameraInterpolator();
    }

    cameraInterpolator->a.Set(*globalCamera); // current camera
    cameraInterpolator->b.Set(cameraNode,cameraTransform); 
	cameraInterpolator->tstart=Now();
	cameraInterpolator->tend=cameraInterpolator->tstart+1.2;
	cameraInterpolator->tstart-=min(1.0f,(float) lastFrameTime / 1000.0f);
    cameraInterpolator->Reset();
    return(1);
}

int  GView::AnimateToCamera(GvNode *cameraNode,GvPath &cameraPath)
{ 
	BindViewpoint(NULL);
    if (!cameraInterpolator) {
        cameraInterpolator = new GCameraInterpolator();
    }
	cameraAnimateTargetViewpoint.set(cameraNode);
	cameraAnimateTargetViewpointPath = cameraPath;
    Matrix m;
	m.Identity();
	if (cameraAnimateTargetViewpointPath.getMatrix(m)) {
	}


    cameraInterpolator->a.Set(*globalCamera); // current camera
    cameraInterpolator->b.Set(cameraNode,&m); 
	cameraInterpolator->tstart=Now();
	cameraInterpolator->tend=cameraInterpolator->tstart+1.2;
	cameraInterpolator->tstart-=min(1.0f,(float) lastFrameTime / 1000.0f);
    cameraInterpolator->Reset();
    return(1);
}



// do next step of camera animation
//
int GView::AnimateCameraStep()
{   int ret=1;
//    float t=cameraInterpolator->alpha + 0.1;
	// float animationSpeed = 1.0f/ 1.2f; // 1.2sec
	// step forward the interrpolation based on animationSpeed
    // float t=cameraInterpolator->alpha + animationSpeed * (float) lastFrameTime / 1000.0f;

	double t= Now();
	t = (t - cameraInterpolator->tstart) / (cameraInterpolator->tend-cameraInterpolator->tstart);


	// target viewpoint might have been animated, reevaluate matrix & viewpoint 
	if (cameraAnimateTargetViewpoint.get() != (GvNode*) NULL) {
	    Matrix m;
		m.Identity();
		if (cameraAnimateTargetViewpointPath.getMatrix(m)) {
		}
		cameraInterpolator->b.Set(cameraAnimateTargetViewpoint,&m); 
	}

    if (t>= (1.0 - 1E-8)) { // last step 
        ::SetCamera(camera,cameraInterpolator->b);
		*globalCamera = *camera;
        
		// go to next viewpoint if there is one 
		if (cameraInterpolateAll && viewpointIndex < cameraInterpolateEnd ) {
            viewpointIndex++;
            if (viewpointIndex >=0 && sceneInfo && viewpointIndex <sceneInfo->cameras.Length()) {
                return SetCamera(sceneInfo->cameras[viewpointIndex],&sceneInfo->cameraTransforms[viewpointIndex],1);
             } else viewpointIndex = 0;

        } else {

         AnimateCameraStop();

		 // this will bind the  final viewpoint 
         if (viewpointIndex >=0 && sceneInfo && viewpointIndex <sceneInfo->cameras.Length()) {
               SetCamera(sceneInfo->cameras[viewpointIndex],&sceneInfo->cameraTransforms[viewpointIndex],0);
         } else viewpointIndex = 0;
		}
        
		ret = 2;

    } else {
      cameraInterpolator->Interpolate(t);
      ::SetCamera(camera,cameraInterpolator->current); // will be global
	  *globalCamera = *camera;

    }
    // renderState.SetCamera(GetPhysicalCamera()); done at render time 
    return(ret);
}
    
// stop camera animation
void GView::AnimateCameraStop()
{
    if (cameraInterpolator)
        delete cameraInterpolator;
    cameraInterpolator = NULL;
    cameraInterpolateAll=0;

	cameraAnimateTargetViewpoint.set((GvNode*) NULL);
	cameraAnimateTargetViewpointPath.Flush();

}


// animate through all camera 
void GView::AnimateAllCameras()
{
    viewpointIndex = 0;
    cameraInterpolateAll=1;
    cameraInterpolateCurrent = viewpointIndex;
    if (sceneInfo) {
        cameraInterpolateEnd = sceneInfo->cameras.Length()-1;
    }
    else cameraInterpolateEnd = 0;

    SetNextViewpoint(1);
}

// set viewpoint viewpoint index
int GView::SetViewpoint(int index,int animate)
{   viewpointIndex = index;
    if (viewpointIndex >=0 && sceneInfo && viewpointIndex <sceneInfo->cameras.Length()) {
        return SetCamera(sceneInfo->cameras[viewpointIndex],&sceneInfo->cameraTransforms[viewpointIndex],animate);
    } else viewpointIndex = 0;

    return(0);
}

// return name of current viewpoint 
const char * GView::GetViewpointName()
{
    if (viewpointIndex >=0 && sceneInfo && viewpointIndex <sceneInfo->cameras.Length()) {
        return sceneInfo->cameras[viewpointIndex]->getName();
    } else return NULL;
}


//check viewpoint node, for non-empty description 
BOOL isUiViewpoint(GvNode *n)
{
	BOOL ok=TRUE;

	if (n == NULL) return FALSE;
	if (n->refCnt <=1)  // assume one extra ref from scene info
		return FALSE;

	if (RTISA(n,GvViewpoint)) {
		GvViewpoint *vp = RTCAST(n,GvViewpoint);
		if (vp->description.get().getLength() == 0) 
			ok = FALSE;
	}
	return ok;
}
 
// goto prev/next viewpoint
int GView::SetNextViewpoint(int animate)
{
    if (!sceneInfo) return(0);

	if (viewpointIndex <0) {
		viewpointIndex = 0;
	} else {

		viewpointIndex ++;
		// take only VP with non-empty descriptions
		while (viewpointIndex >=0 &&  viewpointIndex <sceneInfo->cameras.Length()) {
			if (isUiViewpoint(sceneInfo->cameras[viewpointIndex]))
				break;
			viewpointIndex ++;
		}
	}
	
	if (viewpointIndex >= sceneInfo->cameras.Length()) viewpointIndex = 0; // wrap

    if (viewpointIndex >=0 &&  viewpointIndex <sceneInfo->cameras.Length()) {
        return SetCamera(sceneInfo->cameras[viewpointIndex],&sceneInfo->cameraTransforms[viewpointIndex],animate);
    } else viewpointIndex = 0;
    return(0);
}

// goto prev/next viewpoint
int GView::SetPrevViewpoint(int animate)
{
    if (!sceneInfo) return(0);

    viewpointIndex --;
	if (viewpointIndex <0) viewpointIndex = sceneInfo->cameras.Length()-1;

	while (viewpointIndex >=0 &&  viewpointIndex <sceneInfo->cameras.Length()) {
			if (isUiViewpoint(sceneInfo->cameras[viewpointIndex]))
				break;
			viewpointIndex --;
	}
	
	if (viewpointIndex <0) viewpointIndex = sceneInfo->cameras.Length()-1;

    if (viewpointIndex >=0 && viewpointIndex <sceneInfo->cameras.Length()) {
        return SetCamera(sceneInfo->cameras[viewpointIndex],&sceneInfo->cameraTransforms[viewpointIndex],animate);
    } else viewpointIndex = 0;
    return(0);
}

    
// set viewpoint index without change in camera
int GView::SetViewpointIndex(int index)
{
    viewpointIndex = index;
    if (viewpointIndex >=0 && sceneInfo && viewpointIndex <sceneInfo->cameras.Length()) {
        return(1);
    } else viewpointIndex = 0;
    return(0);
}

// flush the viewpoints 
void GView::FlushViewpoints()
{
   viewpoints.deleteAll();
   BindViewpoint(NULL);
}

// empty all VRML 2 bounded nodes
void GView::FlushBoundedNodes()
{
	termPendingScripts();

	sharedZone.set((GvNode*)NULL);

	BindBackground(NULL);
	
	SetFollowObjectOff();
	// BindAvatar(NULL); // keep Avatar 

    viewpoints.deleteAll();
	BindViewpoint(NULL);
	BindNavigationInfo(NULL);
	BindFog(NULL);
	boundViewpointStack.deleteAll();	
	boundNavigationInfoStack.deleteAll();
	boundFogStack.deleteAll();
	boundBackgroundStack.deleteAll();

	overTouchSensors.deleteAll();
	activeTouchSensors.deleteAll();
	activeVisibilitySensors.deleteAll();
	activeZones.deleteAll();

	editNode.set((GvNode *) NULL);
	editNodePath.Flush();

}

void GView::SetFollowObjectOff()
{
	followObject = FALSE;
	followObjectPath.Flush();
}

/*
   set follow object mode given a node and a reference point 
   result FALSE if node null or not found in tranform hierarchy
*/

gbool GView::SetFollowObject(GvNode *node,const Point &p)
{

	if (node == NULL) {
		SetFollowObjectOff();
		return FALSE;
	}
	Matrix m;

	// need to find path to node
	GFindNodeTraversal traversal(node);
	
	traversal.InitializeAttributes();
	traversal.SetCamera(globalCamera);
	traversal.SetPhysicalCamera(GetPhysicalCamera());

	Traverse(traversal);

	if (traversal.found) {
		//traversal.foundPath.Trace();
		followObjectPath = traversal.foundPath;
		Matrix m;

		followObjectPosition = p;

		if (traversal.foundPath.getMatrix(m)) { // get the global position 
			followObjectPositionGlobal = m * followObjectPosition;
		} 
	
	
		BindViewpoint(NULL);
				
		followObject = TRUE;
		//followObjectNormal = hit->vn;
		followObjectCameraPosition =  camera->position;
		return TRUE;

	}
	else {
		SetFollowObjectOff();
		return FALSE;
	}
}

/*
avatar is NULL or the avatar
used for third person mode
scann the avatar for eventIn's, add wrapper nodes
and add to scene

*/

void GView::BindAvatar(GvNode *avatar)
{
	GvNode *currentAvatar = boundAvatar;
	GvNode *wrappedAvatar = avatar;

	// turn of old 
	if (boundAvatar != NULL /*&& avatar != currentAvatar*/) {
		SetAvatarGesture(avatarInfo.nodeEvents,avatarInfo.avatarLeaveWorld);
		GetScene()->removeNode(currentAvatar);
		GvRoute *route= (GvRoute*) (GvNode *) avatarInfo.avatarGestureChangedRoute;
		if (route) GetScene()->deleteRoute(route);
		avatarInfo.avatarGestureChangedRoute = NULL;
		setRedraw();

	}
	
	if (avatar == NULL) { // no new one, clean up 
		boundAvatar.set((GvNode*)NULL);
		avatarInfo.nodeTransform = NULL;
		avatarInfo.nodeChoice = NULL;
		avatarInfo.nodeGesture = NULL;
		avatarInfo.nodeEvents = NULL;
		showAvatar = FALSE;
		// clear events 
		return;
	}
	//  scan for extended interface 
	GvFieldData *fields=avatar->getFieldData();


	avatarInfo.avatarSetTranslation=fields->getEventInIndex(avatar,"set_translation",GvSFVec3f::ClassId());
	if (avatarInfo.avatarSetTranslation<0)
		avatarInfo.avatarSetTranslation=fields->getEventInIndex(avatar,"set_position",GvSFVec3f::ClassId());

	avatarInfo.avatarSetScale=fields->getEventInIndex(avatar,"set_scale",GvSFVec3f::ClassId());
	avatarInfo.avatarSetRotation=fields->getEventInIndex(avatar,"set_rotation",GvSFRotation::ClassId());

	avatarInfo.nodeTransform = avatar;
	gbool hasCustomTranslation= avatarInfo.avatarSetTranslation>=0;

	// add at least a tranform node 
	if (avatarInfo.avatarSetTranslation <0 || avatarInfo.avatarSetRotation <0) {
			Gv2Transform *tr = new Gv2Transform();
			tr->setBrowser(this);
	  	    GvFieldData *fields=tr->getFieldData();

            tr->children.add(wrappedAvatar);
			GetScene()->removeNode(avatar);
			TRACE("Bind Avatar :: wrapping into Transform ");
			avatarInfo.nodeTransform = tr;
			avatarInfo.avatarSetTranslation=fields->getEventInIndex(tr,"set_translation",GvSFVec3f::ClassId());
			avatarInfo.avatarSetScale=fields->getEventInIndex(tr,"set_scale",GvSFVec3f::ClassId());
			avatarInfo.avatarSetRotation=fields->getEventInIndex(tr,"set_rotation",GvSFRotation::ClassId());
			wrappedAvatar = tr;
			
#ifdef _DEBUG
// for CD test
			GvNodeShell *s = new GvNodeShell;
			s->SetShell(speedBox);
			Gv2Collision *c = new Gv2Collision;
			c->collide.set(FALSE);
			c->children.add(s);
			GetScene()->addNode(c);
#endif _DEBUG

//			BindAvatar(tr);
//			return;
	}

	avatarInfo.nodeChoice = avatar;
	avatarInfo.avatarSetChoice=fields->getEventInIndex(avatar,"set_whichChoice",GvSFInt32::ClassId());

	// need a switch node 
	if (avatarInfo.avatarSetChoice < 0) {
			Gv2Switch *tr = new Gv2Switch();
			tr->setBrowser(this);
	  	    GvFieldData *fields=tr->getFieldData();
			tr->whichChoice.set((int)0);
            tr->choice.add(wrappedAvatar);

			GetScene()->removeNode(avatar);
			TRACE("Bind Avatar :: wrapping into Switch");
			avatarInfo.avatarSetChoice=fields->getEventInIndex(avatar,"set_whichChoice",GvSFInt32::ClassId());

	  		avatarInfo.nodeChoice = tr;
			wrappedAvatar = tr;

	}

	// scan special navigation events 
	avatarInfo.nodeEvents = avatar;

	avatarInfo.avatarEnterWorld=fields->getEventInIndex(avatar,"enterWorld",GvSFTime::ClassId());
	avatarInfo.avatarLeaveWorld=fields->getEventInIndex(avatar,"leaveWorld",GvSFTime::ClassId());

	avatarInfo.avatarBeginMove=fields->getEventInIndex(avatar,"beginMove",GvSFTime::ClassId());
	avatarInfo.avatarEndMove=fields->getEventInIndex(avatar,"endMove",GvSFTime::ClassId());

	avatarInfo.avatarBeginCollision=fields->getEventInIndex(avatar,"beginCollision",GvSFTime::ClassId());
	avatarInfo.avatarEndCollision=fields->getEventInIndex(avatar,"endCollision",GvSFTime::ClassId());

	avatarInfo.avatarBeginFly=fields->getEventInIndex(avatar,"beginFly",GvSFTime::ClassId());
	avatarInfo.avatarEndFly=fields->getEventInIndex(avatar,"endFly",GvSFTime::ClassId());

	avatarInfo.avatarBeginJump=fields->getEventInIndex(avatar,"beginJump",GvSFTime::ClassId());
	avatarInfo.avatarEndJump=fields->getEventInIndex(avatar,"endJump",GvSFTime::ClassId());

	avatarInfo.avatarBeginClick=fields->getEventInIndex(avatar,"beginClick",GvSFTime::ClassId());

	{ // get guestures 
		BBox bbox;
		avatarInfo.gestures.SetLength(0);
		avatarInfo.nodeGesture = avatar;
		// generic eventIn SFInt32 set_gesture
		avatarInfo.avatarSetGesture=fields->getEventInIndex(avatar,"set_gesture",GvSFInt32::ClassId());

		avatarInfo.avatarAvatarSize=fields->getEventOutIndex(avatar,"avatarSize" /*,GvMFFloat::ClassId()*/);
		if (avatarInfo.avatarAvatarSize>=0) {
			GvField *f = avatar->getField("avatarSize");
			if (f && RTISA(f,GvMFFloat))
				myAvatarAvatarSize = *f;
			else myAvatarAvatarSize.makeRoom(0);
			if (myAvatarAvatarSize.Length()>0) collisionDistance = collisionDistanceLocal = myAvatarAvatarSize[0];
			if (myAvatarAvatarSize.Length()>1) heightOverGround = heightOverGroundLocal = myAvatarAvatarSize[1];
			if (myAvatarAvatarSize.Length()>2) stepOverSize = stepOverSizeLocal = myAvatarAvatarSize[2];

		} else {
			// if avatar has translation any existing translation state would be inlcuded !!!
			if (!hasCustomTranslation) {
				if (avatar->getBBox(bbox)) { // test compute avatarsize from bbox 
					// assume eye at 0 0 0;
					TRACE_BBOX(bbox);
					if (bbox.mi.y <0) {
						heightOverGround = heightOverGroundLocal = -bbox.mi.y; 

					}
					float r = 0.55f * sqrt( sqr(bbox.ma.x-bbox.mi.x) + sqr(bbox.ma.z-bbox.mi.z)); 

					if (collisionDistance > r) r = collisionDistance;
						collisionDistance = collisionDistanceLocal = r;
				
					TRACE("Setting avatar height to %f, coll distance %f ", -bbox.mi.y,r);

				}
		  }

		}


		int cnt = 0;
		while (true) {
			char buf[60];
			sprintf(buf,"set_gesture%d",cnt+1);
			int gesture=fields->getEventInIndex(avatar,buf,GvSFTime::ClassId());
			if (gesture>=0) {
				avatarInfo.gestures.Append(gesture);
			}
			else break;
			cnt++;
		}

	}


//	avatarSetBind=fields->getEventInIndex(avatar,"set_bind",GvSFBool::ClassId());
//	avatarSetJump=fields->getEventInIndex(avatar,"set_jumping",GvSFTime::ClassId());
//	avatarSetCollision=fields->getEventInIndex(avatar,"set_collision",GvSFTime::ClassId());

//	avatarStartMove=fields->getEventInIndex(avatar,"startMove",GvSFTime::ClassId());
//	avatarTranslationKeys=fields->getEventInIndex(avatar,"translationKeys",GvMFVec3f::ClassId());
//	avatarRotationKeys=fields->getEventInIndex(avatar,"rotationKeys",GvMFRotation::ClassId());
/*
	avatarTranslation.makeRoom(2);
	avatarRotation.makeRoom(2);

	if (avatarStartMove >=0) {  // keyframe animation 
		if (avatarTranslationKeys < 0) avatarStartMove = -1;
		if (avatarRotationKeys < 0) avatarStartMove = -1;
	}
*/
	//avatar = wrappedAvatar;

	showAvatar = TRUE;

	if (avatarInfo.avatarSetTranslation <0 /*|| avatarSetMoving <0*/ ) {
		// bad avatar 
		showAvatar=FALSE;
	} else {
		// send avatar point to shared Zone : eventIn SFNode set_sharedZone
		int set_sharedZone=fields->getEventInIndex(avatar,"set_sharedZone",GvSFNode::ClassId());
		if (set_sharedZone>=0) {
			GvNode::sendEventIn(&sharedZone,avatar,set_sharedZone);
		}
		// set url nickname isPilot propertys
		int url=fields->getEventInIndex(avatar,"set_url",GvSFString::ClassId());
		if (url>=0) {
			GvNode::sendEventIn(&myAvatarURL,avatar,url);
		}
		int nickName=fields->getEventInIndex(avatar,"set_nickname",GvSFString::ClassId());
		if (nickName>=0) {
			GvNode::sendEventIn(&myAvatarName,avatar,nickName);
		}
		int isPilot=fields->getEventInIndex(avatar,"set_isPilot",GvSFBool::ClassId());
		if (isPilot>=0) {
			GvSFBool val(TRUE);
			GvNode::sendEventIn(&val,avatar,isPilot);
		}
		{   // send intial position for animated avatars 
			GCamera &c = GetGlobalCamera();
			GvSFVec3f p(c.position);
			GvNode::sendEventIn(&p,avatarInfo.nodeTransform,avatarInfo.avatarSetTranslation);
		}


		// check for eventOut SFInt32 gesture_changed and ROUTE this to Browser eventOut myAvatarGesture	
		avatarInfo.avatarGestureChanged=fields->getEventOutIndex(avatar,"gesture_changed" /*,GvSFInt32::ClassId() */);

		if (avatarInfo.avatarGestureChanged>=0) {
			GvRoute *route = new GvRoute();
			route->set(avatar,avatarInfo.avatarGestureChanged,this,this->getFieldIndex(&myAvatarGesture)); 
		    GetScene()->addRoute(route,FALSE);
			avatarInfo.avatarGestureChangedRoute = route;

		}
		// to do : zone.setMyAvatar
		if (getThirdPersonMode()==4) { // avatar forground mode

	        GvLayer *layer= new GvLayer();

			Gv2Transform *tr = new Gv2Transform();
			tr->setBrowser(this);
			
			tr->scale.set(Point(1, 1, 1));
			
			//tr->translation.set(Point(0.6, 0.8, -0.5));
			tr->translation.set(Point(0 ,0.8, -0.5));
	
			
	  	    GvFieldData *fields=tr->getFieldData();

            tr->children.add(wrappedAvatar);

			TRACE("Bind Avatar :: wrapping into Transform ");
			// reset old values if any 
			{
				GvSFRotation r;
				GvSFVec3f p;
				if (avatarInfo.avatarSetRotation >= 0) GvNode::sendEventIn(&r,avatarInfo.nodeTransform,avatarInfo.avatarSetRotation);
				if (avatarInfo.avatarSetTranslation>=0) GvNode::sendEventIn(&p,avatarInfo.nodeTransform,avatarInfo.avatarSetTranslation);
			}	

			//avatarInfo.nodeTransform = tr;
			
			avatarInfo.avatarSetTranslation= -1;
			avatarInfo.avatarSetRotation = -1;

			//fields->getEventInIndex(tr,"set_translation",GvSFVec3f::ClassId());
			//avatarInfo.avatarSetScale=fields->getEventInIndex(tr,"set_scale",GvSFVec3f::ClassId());
			//avatarInfo.avatarSetRotation=fields->getEventInIndex(tr,"set_rotation",GvSFRotation::ClassId());
			
			layer->addChild(tr);

			layer->setBrowser(this);
			layer->fog.set(FALSE);
			layer->pick.set(FALSE);
			
			layer->clear.set(GvLayer::DEPTH | GvLayer::COLOR );
			
			layer->translation.set(0.7,0.2);
			layer->size.set(0.3,0.4);

			layer->depthWrite.set(TRUE);
			layer->viewpointStretch.set(FALSE);


			// add a background rectangle
			// layer->setViewport()
			
			//foreground = layer;
			GetScene()->addNode(layer);
			wrappedAvatar = layer;


		} 	
		else { // part of scene

			// add the wrapped avatar to scene at toplevel
			GetScene()->addNode(wrappedAvatar);

		}

		// send world enter guesture 
		SetAvatarGesture(avatarInfo.nodeEvents,avatarInfo.avatarEnterWorld);
	}
	
	boundAvatar.set(wrappedAvatar);

	camera->OnChanged(); // force refresh of avatar position 
	setRedraw();
}

// load avatar from file 
int GView::LoadAvatar(const char *fileName)
{  int ret=-1;

	  if (!GetScene()) return -1;

	  if (true) { // assume url
		GvMFString url;
		url.makeRoom(2);
		url.set1(0,fileName);
		url.set1(1,"http://www.blaxxun.com/vrml/avatars/default.wrl");
		createVrmlFromURL(&url,this,"set_myAvatarNodes",NULL);
		return (1);

	  } else {		

	  	GvScene *scene = new GvScene();
	  	scene->setBrowser(this);
	  	scene->ref();
	 	scene->worldUrl.set(fileName);

	  	ret =	  ReadVrmlFile(fileName,GetReporter(),scene);

	  	if (ret<0) {
		  scene->unref();
		  return ret;
	  	}

	  	// bind 3D person - Avatar node if found in file 
	  	GvNode *avatar;
	  	if ((avatar=scene->getNode("Avatar")) != NULL) {
			GetScene()->addNode(avatar);
		  	myAvatar.set(avatar);
		  	BindAvatar(avatar);
	  	}	
	  	else {
		  Gv2Transform *t= new Gv2Transform();
		  t->children.set(scene->children);
		  myAvatar.set(t);

		  GetScene()->addNode(t);
		  BindAvatar(t);
	  	}

	  	scene->unref();
	  }
	  
	  return(1);

}

// eventIn , set URL of my avatar for third person view
void GView::set_myAvatarURL(GvSFString *event)
{
	myAvatarURL.set(*event); // save url
	// to think : save in registry ?

    if (1) { // showAvatar) { // currently active, reload new 
		// `viewController->print(CString("Set my avatar url"));

		GvMFString url;
		url.makeRoom(2);
		url.set1(0,event->get());
		url.set1(1,"http://www.blaxxun.com/vrml/avatars/default.wrl");
		createVrmlFromURL(&url,this,"set_myAvatarNodes",NULL);
	}
	
}

// eventIn set my avatar name
void GView::set_myAvatarName(GvSFString *event)
{
	myAvatarName.set(*event);
}

// eventIn , callback for createVrmlFromURL
void GView::set_myAvatarNodes(GvMFNode *event)
{
	  GvNode *avatar = NULL;
	  
	  int cnt= event->getNum();
	  int current = 0;
	  if (cnt == 0) return;
	  
	  // get first node "real" node
	  while (current <cnt) {

		avatar = event->get1(current);
		if (avatar == NULL 
			|| RTISA(avatar,GvWorldInfo)
			|| RTISA(avatar,GvNavigationInfo)
			|| RTISA(avatar,GvDirectionalLight)
			|| RTISA(avatar,GvViewpoint)
			) {
			// ignore
			avatar = NULL;
		}
		else break;
		current ++;
	  }	

	  if (!avatar) return;

	  myAvatar.set(avatar);

      if (showAvatar || thirdPersonView) {
	      BindAvatar(avatar);	//  bind it 
      }
}

// issue guesture number 1.. nn
void GView::set_myAvatarGesture(GvSFInt32 *event)
{
	int val = event->get();
	if (avatarInfo.nodeGesture == NULL) return;

	if (val>=0 && val < avatarInfo.gestures.Length() && avatarInfo.gestures[val]>=0) {
		SetAvatarGesture(avatarInfo.nodeGesture,avatarInfo.gestures[val]);
	}
	else if (avatarInfo.avatarSetGesture>=0) // we have eventIn SFInt32 set_gesture
		GvNode::sendEventIn(event,avatarInfo.nodeGesture,avatarInfo.avatarSetGesture);


}

// show avatar, load if needed
gbool GView::ShowAvatar(gbool mode)
{
	if (!mode) {
		if (showAvatar) {
			showAvatar = FALSE;
			BindAvatar(NULL);
		}
	} else {
/*
		if (boundAvatar != (GvNode*) NULL) { // we already have our avatar loaded
			BindAvatar(boundAvatar);
		} 
		else  */

		if (myAvatar != (GvNode*) NULL) {
			BindAvatar(myAvatar);
		} else {
			LoadAvatar(myAvatarURL);
		}
	}
	return showAvatar;
}

void GView::set_showMyAvatar(GvSFBool *event)
{
	ShowAvatar(event->get());
    SetThirdPersonView(event->get());
}


// is avatar shown
gbool GView::IsAvatarShown()
{
	return showAvatar;
}

// set avatar gesture, event must be of sftime
void GView::SetAvatarGesture(GvNode* node,int eventId)
{
	if (node && (eventId>=0)) {
		GvNode::sendEventIn(&time_changed,node,eventId);
	}
}


// set third person mode on 
gbool GView::SetThirdPersonView(gbool flag)
{
	thirdPersonView = flag;
	if (flag) {
		UpdateGlobalCamera();
		*physicalCamera = *globalCamera;
		physicalCameraInterpolator.a.Set(*globalCamera);
		physicalCameraInterpolator.b = physicalCameraInterpolator.a;
		physicalCameraInterpolator.alpha = 1.0;
		UpdatePhysicalCamera(gtrue);
	}
	return thirdPersonView;
}

// update the physical camera for a changed globalCamera
gbool GView::UpdatePhysicalCamera(gbool cameraChanged)
{
#if 1    
	GCamera target;

	float animationSpeed = 1.0f/ 0.5f; // 1.2sec
    float t=physicalCameraInterpolator.alpha;
	float dt= animationSpeed * (float) lastFrameTime / 1000.0f;
	
	t+= dt;
	if (t>=1.0) 
		t = 1.0f;

	physicalCameraInterpolator.Interpolate(t);
	
	int mode = getThirdPersonMode(); // ==4 { // avatar forground mode

	gbool useGlobalCamera= mode>=2 && mode <4;
	
	gbool fixedOrientation= mode==2;
	gbool fixedPosition= mode==3;

	*physicalCamera = *globalCamera; // copy other settings znear/zfar etc 

	::SetCamera(physicalCamera,physicalCameraInterpolator.current);

	if (useGlobalCamera) {
		target = thirdPersonFollowCamera; // use initial viewpoint camera 
		if (!cameraMatrix.IsIdentity()) {
			target.Transform(cameraMatrix.M());
		}
		Point oldUp(target.up);

		target.orbitCenter = globalCamera->position;
		target.Dolly(followDolly.x(),followDolly.y(),followDolly.z(),1);
		target.OrbitCenter(followOrbit.x(),followOrbit.y(),1);
		
		target.Pan(followPan.x(),followPan.y(),1);

		target.CheckUpVector();

	} 
	else target = *globalCamera; // use current camera 

	if (cameraChanged) {

		Point targetUp(target.up);

		if (t<0.25) {
			physicalCameraInterpolator.Interpolate(0.25);
			::SetCamera(physicalCamera,physicalCameraInterpolator.current);
		}
		physicalCameraInterpolator.a=physicalCameraInterpolator.current;
		physicalCameraInterpolator.alpha = 0;// dt;
		//::SetCamera(physicalCamera,physicalCameraInterpolator.b);
		//physicalCameraInterpolator.a=physicalCameraInterpolator.b;

		if (fixedOrientation) {
			Point delta;
			delta = globalCamera->position - thirdPersonFollowCamera.position;
			target += delta;
		}
		else if (fixedPosition) {
			Point dir;
			dir = globalCamera->position - target.position;
			float dirLen = dir.Len();
			if (dirLen >0) {  // lookat avatar center 
				target.SetTargetDirection(dir);
			}


		} else {
	
			target.orbitCenter = globalCamera->position;
			target.Dolly(followDolly.x(),followDolly.y(),followDolly.z(),1); 
			float d = target.TargetDistance();

			{
				target.target = globalCamera->position;
				target.Orbit(followOrbit.x(),followOrbit.y(),1);
				target.target = target.position + target.NormTargetDirection() *d;
			}
			target.Pan(followPan.x(),followPan.y(),1);

			if (Outside(target,globalCamera->position)) {
				Point dir;
				dir = globalCamera->position - target.position;
				float dirLen = dir.Len();
				if (dirLen >0) {  // lookat avatar center 
					target.SetTargetDirection(dir);
					// Up vector
				}
			}
		}
		Point d = target.position - globalCamera->position;
		float dist = d.Length();
		float minDist = (2.0f*collisionDistance);
#if 0
#if 0 
		if (dist< minDist) { // too near ?, move away it is jumpy
			float amount = - (minDist -dist);
			float f=  (minDist - dist) / minDist;

			target.Dolly(0,0,amount *f );
		}
		target.CheckUpVector();
#else 
		if (dist< minDist) { // too near ?, move away it is jumpy
			Point dir;
			dir = globalCamera->position - target.position;
			float dirLen = dir.Len();
			if (dirLen >0) {  // lookat avatar center 
				target.SetTargetDirection(dir);
			}
		}

#endif
#endif

	// fix up vector

	// calculate new camera up vector
	Point right = NormCrossProd(targetUp, target.NormDirection());
	// right.y = 0.0f; // hg ????
	// calculate new camera up vector
	Point up = NormCrossProd(target.NormDirection(), right);


	// normalize up vector
	up.Normalize();

		if (up.Length() > 0.9) // hg
			target.up = up;
	
	physicalCameraInterpolator.b.Set(target);
	
	}
	


	Point d = physicalCamera->position - globalCamera->position;
	float dist = d.Length();

	if (dist<collisionDistance) {
#if 0
		*physicalCamera = *globalCamera;
		physicalCamera->orbitCenter = globalCamera->position;
		physicalCamera->Dolly(followDolly.x,followDolly.y,followDolly.z,1);
		physicalCamera->OrbitCenter(followOrbit.x,followOrbit.y,1);
		physicalCamera->Pan(followPan.x,followPan.y,1);
#endif
	}
	
#else
	*physicalCamera = *globalCamera;
	physicalCamera->orbitCenter = globalCamera->position;
	physicalCamera->Dolly(followDolly.x(),followDolly.y(),followDolly.z(),1);
	physicalCamera->OrbitCenter(followOrbit.x(),followOrbit.y(),1);
	physicalCamera->Pan(followPan.x(),followPan.y());
#endif
	return TRUE;
}





/*
  Bind a new viepoint
  camera already set

  will search VP in transform hierarchy and will evaluate path

*/

void GView::BindViewpoint(GvViewpoint *newViewpoint)
{			 
	SetFollowObjectOff();

	if (boundViewpoint != (GvNode *)NULL) {
		((GvViewpoint *) boundViewpoint.get())->isBound.set(gfalse); 
		((GvViewpoint *) boundViewpoint.get())->bindTime.set(Now()); 
	}
	boundViewpointChanged = 0;

	SetCameraMatrixIdentity();	boundViewpointPath.Flush();


    boundViewpoint.set(newViewpoint);
	boundViewpointChanged = 0; // for now, viewpoint has been set elsewhere 
	if (!newViewpoint) {
		
		camera->position = globalCamera->position; // new 28.08
		camera->target = globalCamera->target; // new 28.08
		camera->up = globalCamera->up; // new 28.08
		camera->width = globalCamera->width; // new 28.08
		camera->height = globalCamera->height; // new 28.08

		UpdateGlobalNavigationValues();
		return;
	}
	
	boundViewpointStack.remove(newViewpoint); // remove from stack, because node now top

	// get the path to the viewpoint to be able to check for changed transformations 
	GFindNodeTraversal traversal(newViewpoint);
	
	traversal.InitializeAttributes();
	traversal.SetCamera(globalCamera);
	traversal.SetPhysicalCamera(GetPhysicalCamera());

	Traverse(traversal);

	if (traversal.found) {
		// traversal.foundPath.Trace();
		boundViewpointPath = traversal.foundPath;
		Matrix m;
		if (traversal.foundPath.getMatrix(m)) {
			SetCameraMatrix(m); // why not ?
		} else
			SetCameraMatrixIdentity();
	} else {
		TRACE("Bound viewpoint not in transform hierarchy !!!\n");
		ASSERT(0);
		boundViewpointPath.Flush();
		SetCameraMatrixIdentity();
	}

	newViewpoint->isBound.set(TRUE);
	newViewpoint->bindTime.set(Now()); 

	// display viewpoint description message 
	if (!firstUpdate && strlen(newViewpoint->description.get()) >0) {
		CString msg = "Viewpoint:";
		msg += newViewpoint->description.get();
		Message(msg);
	}
	UpdateGlobalNavigationValues();
	doUpdateBBox=1; // new 22.06.99 hg

}

void GView::BindViewpoint(GvViewpoint *newViewpoint,GvBool bind)
{
    if (bind) {
		if (viewController) viewController->OnStopNavigation();  // we need to stop navigation
		// otherwise confusion
		if (boundViewpoint != (GvNode *)NULL  && this != boundViewpoint)
			boundViewpointStack.append(boundViewpoint); // push current on stack

		//boundViewpointStack.push(newViewpoint);
		GvBool animate =   !newViewpoint->jump.get();
		if (animate  && cameraAnimate) {
			// find current matrix of viewpoint node
			Matrix m;
			GFindNodeTraversal traversal(newViewpoint);
			traversal.InitializeAttributes();
			traversal.SetCamera(globalCamera);
			traversal.SetPhysicalCamera(GetPhysicalCamera());

			Traverse(traversal);
			if (traversal.found) {
	//			traversal.foundPath.Trace();
				Matrix m;
				m.Identity();
				if (traversal.foundPath.getMatrix(m)) {
				}

				viewpointIndex = -1; 
				if (sceneInfo ) { // update the corresponding viewpoint index
					 for (int i= 0; i< sceneInfo->cameras.Length(); i++) {
						 if (((GvNode*)sceneInfo->cameras[i]) == newViewpoint) {
							 viewpointIndex = i;
							 break;
						 } 
					 }
				}


				// AnimateToCamera(newViewpoint,&m);
				AnimateToCamera(newViewpoint,traversal.foundPath); // reevaulate target viewpoint

				boundViewpointChanged=0; 
				return;
			}
		} // animate 

				viewpointIndex = -1; 
				if (sceneInfo ) { // update the corresponding viewpoint index
					 for (int i= 0; i< sceneInfo->cameras.Length(); i++) {
						 if (((GvNode*)sceneInfo->cameras[i]) == newViewpoint) {
							 viewpointIndex = i;
							 break;
						 } 
					 }
				}

		BindViewpoint(newViewpoint);
		boundViewpointChanged=1; // change camera
	    setRedraw();

	} else { // set_bind FALSE 
		if (newViewpoint->isBound) {
			if (boundViewpointStack.getNum() >0) // bind previous on stack
				// recursive call, but now with TRUE
				BindViewpoint((GvViewpoint* ) boundViewpointStack.get1(boundViewpointStack.getNum()-1),TRUE);
			else 
				BindViewpoint(NULL);
		}
		else 
			boundViewpointStack.remove(newViewpoint); // remove from stack
	}
}


void GView::BindNavigationInfo(GvNavigationInfo *newNavigationInfo)
{
	if (boundNavigationInfo != (GvNode *) NULL) ((GvNavigationInfo *) boundNavigationInfo.get())->isBound.set(gfalse); 
	boundNavigationInfoPath.Flush();
    boundNavigationInfo.set(newNavigationInfo);

	if (!newNavigationInfo) return;

	boundNavigationInfoStack.remove(newNavigationInfo); // remove from stack, because node now top

	newNavigationInfo->Apply(this);
	boundNavigationInfoChanged = 0;
	newNavigationInfo->isBound.set(TRUE);

	UpdateGlobalNavigationValues();

}

void GView::BindNavigationInfo(GvNavigationInfo *newNavigationInfo,GvBool bind)
{
    if (bind) {
		if (boundNavigationInfo != (GvNode *)NULL && this != boundNavigationInfo)
			boundNavigationInfoStack.append(boundNavigationInfo); // push current on stack

		BindNavigationInfo(newNavigationInfo);
		boundNavigationInfoChanged=TRUE;
	    setRedraw();
	} else {  // set_bind = FALSE
		if (newNavigationInfo->isBound) {
			if (boundNavigationInfoStack.getNum() >0) // bind previous on stack
				BindNavigationInfo((GvNavigationInfo* ) boundNavigationInfoStack.get1(boundNavigationInfoStack.getNum()-1));
			else BindNavigationInfo(NULL);
		} else
			boundNavigationInfoStack.remove(newNavigationInfo); // remove from stack
	}
}

int GvNavigationInfo::Apply(GView *view)
{
	// transform speed, avatarSize and visibilityLimit, downDirection
	view->downDirection.Set(0,-1,0);
	view->downDirectionLocal.Set(0,-1,0);	

	view->visibilityLimit = view->visibilityLimitLocal =visibilityLimit.value;

	if (avatarSize.Length()>0) view->collisionDistance = view->collisionDistanceLocal = avatarSize[0];
	if (avatarSize.Length()>1) view->heightOverGround = view->heightOverGroundLocal = avatarSize[1];
	if (avatarSize.Length()>2) view->stepOverSize = view->stepOverSizeLocal = avatarSize[2];

	view->visibilityLimitNear = view->visibilityLimitNearLocal = 0.5f * view->collisionDistance;



	//zFar = visibilityLimit 
	//zNear = 0.5 * collision

	view->allowAnyNavigation = TRUE;

	// set viewer type 

	if (type.Length() > 0 ) {
	  	CString tmp(type[0]);
        tmp.MakeLower();
	    if (view->viewController) view->viewController->SetCameraMode(tmp);
		if (tmp == "none")
			view->allowAnyNavigation = FALSE;

		for (int i=0; i<type.Length();i++) {
		  	tmp = (type[i]);
		    tmp.MakeLower();
			if (tmp == "any") {
				view->allowAnyNavigation = TRUE;
			}

		}

	}
	if (speed.value > 0.0f) 
		view->viewStepSpeed = view->viewStepSpeedLocal = speed.value;
	
	view->SetHeadLight(headlight.value);

	view->UpdateGlobalNavigationValues();

	return 1;
}



// apply the fog node to the render state

int GvFog::Apply(GView *view)
{

	if (
#ifdef _D3D
		view->renderState.canDoRgbLighting 
		&& 
#endif
		(visibilityRange >0)) {
		// float t =  TransformValue(view->cameraMatrix.M(),c;
		// to do: transform from local coordinate system

		view->renderState.fog.color.Set(color[0],color[1],color[2]);
		view->renderState.fog.useBackgroundColor=0;
		view->renderState.fog.SetMode(fogType);

		float t = visibilityRange; // in world ??? TBD check 
		// per VRML spec true viewer distance
		view->renderState.fog.end = t;
		view->renderState.fog.density = 1.0f/t;

	
		view->renderState.fog.start = 0;
		view->renderState.fog.enable = 1;

		if (RTISA(this,GvFog2)) { // fog 2 extension node 
			GvFog2 *fog2 = (GvFog2 *) this;
			view->renderState.fog.start = fog2->visibilityStart.get();
			view->renderState.fog.density = fog2->density.get();
		}
	}
	else view->renderState.fog.enable = 0;

	view->renderState.SetFog();
	
	return 1;
}


void GView::BindFog(GvFog *newFog)
{			 
	if (boundFog != (GvNode *)NULL) ((GvFog *) boundFog.get())->isBound.set(FALSE); 
	boundFogPath.Flush();
    boundFog.set(newFog);
	
	boundFogChanged = 0;
	
	if (!newFog) {
		// turn off 
		if (renderState.fog.enable) {
			renderState.fog.enable = 0;
			renderState.SetFog();
		}
		return;
	}

	boundFogStack.remove(newFog); // remove from stack, because node now top

	newFog->Apply(this);

	boundFogChanged = 0;

	newFog->isBound.set(TRUE);
}


void GView::BindFog(GvFog *newFog,GvBool bind)
{
    if (bind) {
		if (boundFog != (GvNode *)NULL && this != boundFog)
			boundFogStack.append(boundFog); // push current on stack
		boundFogChanged=TRUE; // change fog 
		BindFog(newFog);
	    setRedraw();
	} else {
		if (newFog->isBound) {
			boundFogChanged = TRUE; 
			if (boundFogStack.getNum() >0) // bind previous on stack
				BindFog((GvFog* ) boundFogStack.get1(boundFogStack.getNum()-1));
			else BindFog(NULL);
			setRedraw();
		}
		else 
			boundFogStack.remove(newFog); // remove from stack
	}
}

void GView::BindBackground(GvBackground *newBackground)
{			 
	if (boundBackground != (GvNode *)NULL) 
		((GvBackground *) boundBackground.get())->isBound.set(FALSE); 

	ClearBackground();

	boundBackgroundPath.Flush();
    boundBackground.set(newBackground);
	boundBackgroundChanged = 0;


	if (!newBackground) return;
	
	boundBackgroundStack.remove(newBackground); // remove from stack, because node now top

	newBackground->Apply(this);

	newBackground->isBound.set(TRUE);

	boundBackgroundChanged = 0;

}

void GView::BindBackground(GvBackground *newBackground,GvBool bind)
{
    if (bind) {
		if (boundBackground != (GvNode *)NULL && this != boundBackground)
			boundBackgroundStack.append(boundBackground); // push current on stack
		BindBackground(newBackground);
		boundBackgroundChanged = TRUE; 
		boundBackgroundChanged=0; // xx 
	    setRedraw();
	} else { // set_bind = FALSE
		if (newBackground->isBound) {
			if (boundBackgroundStack.getNum() >0) // bind previous on stack
				BindBackground((GvBackground* ) boundBackgroundStack.get1(boundBackgroundStack.getNum()-1));
			else BindBackground(NULL);
			boundBackgroundChanged = TRUE; 
			boundBackgroundChanged=0; // xx 

			setRedraw();
		}
		else 
			boundBackgroundStack.remove(newBackground); // remove from stack
	}
}



int GView::SetCamera(GvNode *cameraNode,const Matrix *cameraTransform,int animate)
{ 
 
 if (!cameraNode) return(0);

 if (animate) {
	// find current matrix of camera 
	Matrix m;
	GFindNodeTraversal traversal(cameraNode);
	traversal.InitializeAttributes();
	traversal.SetCamera(globalCamera);
	traversal.SetPhysicalCamera(GetPhysicalCamera());

	Traverse(traversal);
	if (traversal.found) {
		traversal.foundPath.Trace();
		Matrix m;
		m.Identity();
/*
		if (traversal.foundPath.getMatrix(m)) {
			 return AnimateToCamera(cameraNode,&m);
		}
	    return AnimateToCamera(cameraNode,NULL);
*/
	    return AnimateToCamera(cameraNode,traversal.foundPath);
	}
    else return AnimateToCamera(cameraNode,cameraTransform);
 }

 int ret=-1;
 GvPerspectiveCamera *c=NULL;
 GvViewpoint *cv=NULL;

 
 if (RTISA(cameraNode,GvPerspectiveCamera)) 
 {
	c=RTCAST(cameraNode,GvPerspectiveCamera);

 }
 else if (RTISA(cameraNode,GvViewpoint)) 
 {
	cv =RTCAST(cameraNode,GvViewpoint);

	if (cv != (GvNode*) boundViewpoint)
		BindViewpoint(cv); 

	if (cameraTransform) // temp 
		boundViewpointPath.getMatrix(*((Matrix *)cameraTransform));

	if (cv == boundViewpoint) { // will be transformed at start of rendering cylce
		cameraTransform = NULL;
	}

 }

 if (c || cv)
 {
 			Matrix m;
            float viewAngle = 0.785398;
			float aspectRatio = 1.0f;
            if (c) {
			    c->orientation.get(m);
			    camera->position=c->position;
                viewAngle = (float) c-> heightAngle;
				aspectRatio =  c->aspectRatio.value;
            } else 
            if (cv) {
			    cv->orientation.get(m);
			    camera->position=cv->position;
                viewAngle = (float) cv->fieldOfView;
            }

			Point dir(0,0,-1.0);
			Point up(0,1.0,0);
			
            // apply orientation to standard dir and up vectors
            dir *= m;	
			up *= m;	up.Normalize();
			
            if (cameraTransform) {
                camera->position *= *cameraTransform;
                dir = RotateOnly(*cameraTransform,dir);
                up = RotateOnly(*cameraTransform,up);
                // near far focalDistance ??????????
            }
            dir.Normalize();
            up.Normalize();

			Point size = bbox.Size(); // computed bounding box
		    
		    float field = max(max(fabs(size.x),fabs(size.y)),fabs(size.z));
			
			int positionInBox = bbox.Inside(camera->position);
  			
  			if (bbox.IsEmpty() || (field<=1E-20f)) 
				field = 2.0f; // no bounding box yet, bad 
  			
            // compute distance to target point

  			//xx float targetDistance = field*2.0f;
  			float targetDistance = field*1.0f;
			
			// viepoint inside scene 
			if (positionInBox) targetDistance = 0.2 * field; // new 2.3.96

            camera->targetDistanceIsDefault=1;

            if (c) {        // C has focalDistance value

#if 1
			if (!c->focalDistance.isDefault() 
				&& (c->focalDistance.value >= (targetDistance *0.01))  // should no be to small
				)
			/* && (c->focalDistance.value != 5.0f )*/  
			{
				targetDistance =  c->focalDistance.value;
                camera->targetDistanceIsDefault=0;
			}
#else 
			targetDistance =  c->focalDistance.value;

#endif

#if 0
			GetReporter()->Error("field %f foc Dist %f targ dist %f isDef %d \n(%f %f %f) (%f %f %f) %d %f",
			field,c->focalDistance.value,targetDistance,(int)c->focalDistance.isDefault(),
			bbox.mi.x,bbox.mi.y,bbox.mi.z,bbox.ma.x,bbox.ma.y,bbox.ma.z,bbox.IsEmpty(),max(fabs(size.x),fabs(size.y)));
#endif
            } // if(c)

            camera->zrangeIsDefault=1;

            // compute a reasonable z-range 

			if (cv) { // VRML 2.0 viewpoint 
				if (visibilityLimit >0.0f) {
					camera->zfar = visibilityLimit*lodScale;
					camera->zrangeIsDefault=0;
				}
				else {
					if (positionInBox) 
						camera->zfar = field*1.5;
					else camera->zfar = field*3.0f;
					
					Point center = bbox.Center();
					Point d = camera->position - center;
					float dist = d.Length();
					
					// make shure object is visible from viewpoint 
					if ((dist+field) > camera->zfar)
						camera->zfar = dist + field;

				}

				if (visibilityLimitNear > 0.0f)
				    camera->znear = visibilityLimitNear;
				else 
					 camera->znear = camera->zfar * camera->znearFactor;

			} else {
				
				if (positionInBox) 
					camera->zfar = field*1.5;
				else camera->zfar = field*3.0f;

					Point center = bbox.Center();
					Point d = camera->position - center;
					
					// make shure object is visible from viewpoint 
					if ((d.Length()+field *0.5) > camera->zfar)
						camera->zfar = d.Length() + field *0.5;


	            camera->znear = camera->zfar * camera->znearFactor;
			}


			// specified in VRML 1.0 node ? 
			if (c && !c->nearDistance.isDefault()) 
			{
				camera->znear = max(0.000001f, c->nearDistance.value);		 // is this ok ?
			}
			if (c && !c->farDistance.isDefault()) 
			{
				camera->zfar = max(targetDistance*2.0f, c->farDistance.value);  // is this ok
                camera->zrangeIsDefault=0;
			}

            // compute target 
			camera->target = camera->position + targetDistance*dir;
			camera->up = up;

            // field of view 
			camera->height = 2.0 * tan(viewAngle * 0.5)*targetDistance;
			camera->width = camera->height *  aspectRatio;

			if (!bbox.IsEmpty())
				camera->SetWorldReference(bbox);

			camera->ComputeWorldUpFromUp(); 

            camera->OnChanged();

			// set autoanimate off
			ret=0;
 }
 else if (RTISA(cameraNode,GvOrthographicCamera)) 
 {

	  		GvOrthographicCamera *c=RTCAST(cameraNode,GvOrthographicCamera);
			Matrix m;
			c->orientation.get(m);
			Point dir(0,0,-1);
			Point up(0,1,0);
			dir *= m;	dir.Normalize();
			up *= m;	up.Normalize();
			camera->position=c->position;

			Point size = bbox.Size();
		    float field = max(fabs(size.x),fabs(size.y),fabs(size.z));
  			
  			if (bbox.IsEmpty()) field = 1.0;
  			
  			float targetDistance = field*2.0;

			if (!c->focalDistance.isDefault() && c->focalDistance != 5.0) 
			{
				targetDistance =  (float) c->focalDistance;
			}

  			
  			camera->zfar = targetDistance*4.0;
		    // camera->znear = min(targetDistance*0.001,1.0);

            camera->znear = camera->zfar * camera->znearFactor;

			camera->target = camera->position + targetDistance*dir;
			
			camera->up = up;
			if (!c->nearDistance.isDefault()) 
			{
				camera->znear = max(0.0000001f,(float) c->nearDistance);
			}
			if (!c->farDistance.isDefault()) 
			{
				camera->zfar = max(targetDistance,(float) c->farDistance.value);
			}
			camera->height = c->height.value;
			camera->width = camera->height *  c->aspectRatio;

			if (!bbox.IsEmpty())
				camera->SetWorldReference(bbox);
			camera->ComputeWorldUpFromUp(); // new 8.12.96
            camera->OnChanged();

			// set autoanimate off
			ret=0;
 }
// tmp
	*globalCamera = *camera;

	thirdPersonFollowCamera = *globalCamera;

 return(ret);
}


//@cmember set the camera to the Camera node 
int  GView::SetCamera(const char *cameraName,int animate)
{ 
	GvNode *n;
	if (sceneInfo) {
	for (int i = 0; i< sceneInfo->cameras.Length(); i++) {
		n = sceneInfo->cameras[i];
		gbool  matchName = (n->objName) && (*n->objName == cameraName);
		if (!matchName) { // find matching on description GvSFString 
			GvViewpoint*v = RTCAST(n,GvViewpoint);
			if (v && v->description.get() == cameraName) matchName = TRUE;
		}
		if (matchName) {
            SetViewpointIndex(i);
			SetCamera(n,&sceneInfo->cameraTransforms[i],animate);
			return(1);
		}
	}
	}
	return 0; 
}


// save current camera into camera node 
int  GView::SaveCamera(GvNode *cameraNode)
{
  Vector axis;  
  float angle;
  ConvertCameraToOrientation(camera->position, camera->target,camera->up,axis, angle);

  if (RTISA(cameraNode,GvPerspectiveCamera)) 
  {
	    GvPerspectiveCamera *n=RTCAST(cameraNode,GvPerspectiveCamera);
        n->position.set(camera->position);
        n->orientation.set(Rotation(axis.x,axis.y,axis.z,angle));
        n->focalDistance.set(camera->TargetDistance());
        n->nearDistance.set(camera->znear);
        n->farDistance.set(camera->zfar);
        n->heightAngle.set(camera->GetFovY());
        return(1);
  }
  if (RTISA(cameraNode,GvOrthographicCamera)) 
  {
	    GvOrthographicCamera *n=RTCAST(cameraNode,GvOrthographicCamera);
        n->position.set(camera->position);
        n->orientation.set(Rotation(axis.x,axis.y,axis.z,angle));
        n->focalDistance.set(camera->TargetDistance());
        n->nearDistance.set(camera->znear);
        n->farDistance.set(camera->zfar);
        return(1);
  }
  else if (RTISA(cameraNode,GvViewpoint)) 
  {
	    GvViewpoint *n=RTCAST(cameraNode,GvViewpoint);
        n->position.set(camera->position);
        n->orientation.set(Rotation(axis.x,axis.y,axis.z,angle));
        n->fieldOfView.set(camera->GetFovY());
        return(1);
  }

  return(0);	
}

// save current camera as new viewpoint and add to scene
int  GView::SaveCamera(const char *cameraName)
{ 
  char buf[60];
  if (!sceneInfo) 
      UpdateSceneInfo();

  
  if (cameraName == NULL) {
      sprintf(buf,"Viewpoint%d",sceneInfo->cameras.Length()+1);
      cameraName = buf;
  }        
  
  Vector axis;  float angle;
  ConvertCameraToOrientation(camera->position, camera->target,camera->up,axis, angle);

  int vrml2 = IsVrml2();

  GvNode *c = NULL;

  if (vrml2) {
	GvViewpoint *n;
	n = new GvViewpoint();
	n->position.set(camera->position);
	n->orientation.set(Rotation(axis.x,axis.y,axis.z,angle));
	n->fieldOfView.set(camera->GetFovY());
	n->setName(cameraName);
	n->description.set(cameraName);
	c = n;

  } else {	
	GvPerspectiveCamera *n;
	n = new GvPerspectiveCamera();
	n->position.set(camera->position);

	n->orientation.set(Rotation(axis.x,axis.y,axis.z,angle));
  
	n->focalDistance.set(camera->TargetDistance());
	n->nearDistance.set(camera->znear);
	n->farDistance.set(camera->zfar);
	n->heightAngle.set(camera->GetFovY());
	n->setName(cameraName);
	c = n;
  }

  SetViewpointIndex(sceneInfo->cameras.Length());

  // put into scene info 
  sceneInfo->cameras.Append(c);
  sceneInfo->cameraTransforms.Append(IdentityMatrix());

  viewpoints.append(c);

  if (theScene)
	  GetScene()->children.add(c);

  return 1; 
}
	
int  GView::SaveCamera(const char *cameraName,float t)
{ 
    return -1; 

}

int  GView::SetCamera(float t)
{ 
    return -1; 
}



// terminate current scene 
int  GView::TermScene()
{
   int i;

   // to think :
   // send final events to  BoundNodes and active nodes like activeTouchSensors ?
	
   FlushEventHandlers();
	// remove avatar 
	if (boundAvatar != NULL) {
		SetAvatarGesture(avatarInfo.nodeEvents,avatarInfo.avatarLeaveWorld);
		GetScene()->removeNode(boundAvatar);
		GvRoute *route= (GvRoute*) (GvNode *) avatarInfo.avatarGestureChangedRoute;
		if (route) GetScene()->deleteRoute(route);
		avatarInfo.avatarGestureChangedRoute = NULL;
		boundAvatar = NULL; 
	}
    
	if (lastHit) 
		delete lastHit;  

	lastHit = NULL;

   // flush any bound nodes 
   FlushBoundedNodes();
   FlushViewpoints();


   if (sceneInfo) {
	  // call shutdown functions on loaded but unitialized scripts
	  for(i=0; i < sceneInfo->scripts.Length(); i++) {
		  GvScript *s = (GvScript *) (GvNode*) sceneInfo->scripts[i];
		  if (!s) continue;

		  s->ref();
		  if (s->state == GvScript::SCRIPT_LOADED_AND_INITIALIZED)
			  s->shutdown();
/*
		  if (s->refCnt <=3) {
			TRACE("xxx \n");	
		  }	
*/
		  s->unref();
	  }	
   }
   
   theScene = NULL;

   if (sceneInfo) {
	  // call shutdown functions on loaded and initialized scripts
	  for(i=0; i < sceneInfo->scripts.Length(); i++) {
		  GvScript *s = (GvScript *) (GvNode*) sceneInfo->scripts[i];
		  
		  if (!s) continue;

		  s->ref();

		  if (s->state == GvScript::SCRIPT_LOADED_AND_INITIALIZED)
			  s->shutdown();
		  // 3rd person mode !!!! if (s->refCnt <= 3)  // assume self ref, 1 from above, 1 from scene info & 1 self ref
		  
		  if (s->refCnt <= 2) { // assume self ref, 1 from above, 1 from scene info 
				TRACE("Scene term : flushing script node fields xxx %d \n",s->refCnt);
				// set Node fields to NULL inorder to delete self references 
				s->flushNodeFields();
		  }	

		  s->unref();
	  }	

	// term movie textures
	  for(i=0; i < sceneInfo->movieTextures.Length(); i++) {
		 GvMovieTexture *s = (GvMovieTexture *) (GvNode*) sceneInfo->movieTextures[i];
		 if (s) s->Term();
	  }	
	  sceneInfo->TermMedia();

   }


   if (sceneInfo) 
	   delete sceneInfo;
   sceneInfo = NULL;


   return 1;
   
}


// Set current displayed shell object
// also resets many internal state
// so SetShell(NULL) is used to flush the current scene 
int GView::SetShell(GShell *shell,int resetCamera)
{

  // flush any bound nodes 
  FlushBoundedNodes();
  
  // reset some attributes
  // stop animation 
  time.Reset();
  time.enabled = 0;

  AnimateCameraStop();

  loadTime = GTime::GetGlobalTime();
  //time.now

  lightInit = 0;	 // reset standard lights
  
  SetHeadLight(TRUE);
  
  firstUpdate = 1;

  bbox.SetEmpty();
  
  renderState.SetNormalize(0);
  
  ClearTasks();

  if (sceneInfo) sceneInfo->Flush();
  FlushViewpoints();

  executeNode = NULL;
  if (lastHit) delete lastHit;  lastHit = NULL;

  InvalidateModelCache();

#ifndef  _GV_MINIMAL
  if (deformer) {
      delete deformer;
      deformer = NULL;
  }  
  if (parameter && parameter->enabled)
      parameter->enabled = FALSE;
#endif

  FlushBackupShell();

  if (shell != theShell) {
  	if (theShell) delete theShell;
  	theShell = NULL;
  }

#ifdef _GFULL  
  if (theWorld) { 
  		delete theWorld; 
		theWorld = NULL;
  }
#endif

  theScene = NULL; 

  theShell = shell;
  if (!theShell) return(-1);

  if (theShell) {
       if (renderState.computePrimitiveList) {
	       theShell->SetFlag(SHELL_COMPUTE_PRIMLIST); // Build tristrips
	       theShell->SetEditFlag(SHELL_FACELIST_CHANGED); // Build tristrips
		}
  }

  if (resetCamera)
  if (theShell) {
	   BBox box;

	   autoZRange = TRUE;
	   theShell->SetEditFlag(SHELL_CHANGED);
	   theShell->ComputeBBox(box);
	   camera->SetByVolume(box.mi,box.ma);
	   camera->Zoom(1.2);
	   bbox = box;
	   camera->SetWorldReference(bbox);
	   renderState.SetCamera(camera);
  }
  SetDefaultTextureParameters();

  return(0);

}

// set the current model to <c GvNode> object 
int GView::SetNode(GvNode *root,int resetCamera)
{

	GvNodeHandle oldScene = GetScene(); // keep scene alive some where 
	GvScene *current = GetScene();
	
	if (root == NULL) {
		SetScene(NULL);
		return 0;
	}


	root->setBrowser(this);

	if (RTISA(root,GvScene)) {
		GvScene *s = (GvScene *)root;
		if (current && (s->worldUrl.get().getLength() == 0)) {
			s->worldUrl.set(current->worldUrl.get());
			s->baseUrl.set(current->baseUrl.get());
		}	

		return SetScene(s,TRUE,resetCamera);

	} else {

		GvScene *s = new GvScene();
		s->setBrowser(this);

		s->addChild(root);
		if (current) {
			s->worldUrl.set(current->worldUrl.get());
			s->baseUrl.set(current->baseUrl.get());
		}	


		return SetScene(s,TRUE,resetCamera);
	}
}

// is the current scene a VRML 20 scene 
gbool GView::IsVrml2() 
{ 
	return  theScene && GetScene()->vrml2; 
}


// get path to installation directory 
CString GView::GetInstallDirectory()
{
	// get directory name of program 
	CString strPathName;
#ifdef WIN32
	GetModuleFileName(AfxGetInstanceHandle( ), strPathName.GetBuffer(_MAX_PATH),_MAX_PATH);
	strPathName.ReleaseBuffer();
	int iBackslash = strPathName.ReverseFind('\\');
	if (iBackslash != -1) 	strPathName = strPathName.Left(iBackslash+1);
#else
#pragma message( "GetInstallDirectory: TO DO")
#endif
	return(strPathName);

}



// reset all settings to the VRML 2 defaults 
void GView::ResetSceneDefaults()
{
	allowAnyNavigation = TRUE;
	collisionDetection = enableCollisionDetection;

	stayOnGround = enableStayOnGround;
	downDirection.Set(0,-1,0);
	downDirectionLocal.Set(0,-1,0);

	collisionDistance = collisionDistanceLocal = 0.25;
	heightOverGround = heightOverGroundLocal = 1.6;
	stepOverSize = stepOverSizeLocal = 0.75; 
	visibilityLimitNear = visibilityLimitNearLocal = 0.5f * collisionDistance;
	visibilityLimit = visibilityLimitLocal = 0.0;
	viewStepSpeed = viewStepSpeedLocal = 1.0;
	
	autoZRange = TRUE;

	SetHeadLight(TRUE);

	lastFrameTimesI=0;

	for(int i=0; i<NUM_LAST_FRAME_TIMES;i++) {
		lastFrameTimes[i]=0;
		lastFrameTris[i]=0;
		lastFramePrims[i]=0;
	}
	lastFrameTimesSum = 0;
	// ??? SetThirdPersonView(FALSE);

	if (nurbsTessellationMode != GNURBS_STATIC_TESSELATION)
		nurbsTessellationScale = min(1.0f,max(0.00001f,nurbsTessellationScale)); // reset on intial loading 

	lastFrameStats.Zero();
	hasCustomSensorMessage=gfalse;
	
}



// set the current model to <c GvScene> object 
int GView::SetScene(GvScene *scene,gbool bindNodes, int resetCamera)
{
#ifdef _D3D
	// AfxMessageBox("Releasing Lights");
	renderState.ReleaseMaterials(); 
	renderState.ReleaseLights(); 
	//renderState.Term();
#endif

	renderState.ResetFrame();

   updateIsComplex = TRUE;
   
   TermScene();
  
   SetShell(NULL);

#ifdef _GFULL
   if (theWorld) { 
	   delete theWorld;
	   theWorld = NULL; 
   }
#endif

   ReleaseSound();

   FlushProtoScenes(); // test: delete all EXTERNPROTOS 
   FlushPendingExternProtos();
   
   ResetSceneDefaults();


   if (scene) 
	   scene->setBrowser(this);

   // assign scene, this will unref the old scene graph
   theScene = scene;

   renderState.SetNormalize(1);

   firstUpdate = 1;
   lightInit = 0;	 // reset standard lights


   // trigger PROTO instancing and loading	
   if (scene)
	   LoadAllInlines(0,gfalse,gfalse);

   if (pendingExternProtos.getNum()>0) {
	   TRACE("We have unloaded EXTERNPROTO's %d \n",pendingExternProtos.getNum());
   }	

   UpdateSceneInfo(TRUE); // will call initialize scripts, was before loadAllInlines


   UpdateBoundingBox();

   if (!scene) 
	   return(-1);

  
   if (resetCamera) 
	   CameraReset();


   if (bindNodes) {
	   BindScene();
   }
   
   if (doUpdateSceneInfo >1) { // new 
	   UpdateSceneInfo();
	   UpdateBoundingBox();

   }

	
   // trigger world url event out
   worldUrl.set(GetUrl());
   
   Redraw();

   return(0);
}


// traverse scene graph and trigger loads 
int GView::LoadAllInlines(int oneOnly,gbool inlines,gbool textures)
{

	  TRACE("GView::LoadAllInlines %d %d %d \n", oneOnly,inlines,textures);

	  GLoadInlinesTraversal traversal;
	  traversal.view = this;
	  // traversal.traversalOrder = TRAVERSE_FRONT_TO_BACK;
	  traversal.traversalOrder = traversalInlineLoadingOrder;

	  // traversal.doViewCulling= TRUE;
	  traversal.doViewCulling= FALSE;
	  traversal.loadProtos = GTraversal::LOAD_ALL; 
	  traversal.initScripts = gfalse;

	  if (inlines)
		 traversal.loadInlines = GTraversal::LOAD_ALL;
	  else { 
		traversal.loadInlines = GTraversal::DONT_LOAD;
	  }	

	  traversal.loadSounds = GTraversal::DONT_LOAD;

	  if (textures)  { 
		    traversal.loadTextures = GTraversal::LOAD_ALL;
		    traversal.loadMovies = GTraversal::LOAD_ALL;
			if (useSound ) traversal.loadSounds = GTraversal::LOAD_ALL;
	  }
	  
	  traversal.maxInlineLevel = maxInlineDepth;

	  traversal.InitializeAttributes();

	  traversal.SetCamera(globalCamera);
	  traversal.SetPhysicalCamera(GetPhysicalCamera());

	  traversal.traverseAll = 1;
	  traversal.traverseTransformHierarchyOnly=1;

	  if (inlines || textures) traversal.stopOnFirst = oneOnly;
	  else traversal.resolveRelativeUrls = gtrue;	  

	  Traverse(traversal);

	return(1);
}
// traverse nodes and trigger loads 
int GView::PrefetchNodes(GvMFNode &nodes, int oneOnly,gbool inlines,gbool textures)
{

	  TRACE("GView::PrefetchNodes %d %d %d \n", oneOnly,inlines,textures);

	  GLoadInlinesTraversal traversal;
	  traversal.view = this;
	  // traversal.traversalOrder = TRAVERSE_FRONT_TO_BACK;
	  traversal.traversalOrder = traversalInlineLoadingOrder;

	  traversal.doViewCulling= FALSE;
	  traversal.loadProtos = GTraversal::LOAD_ALL; 
	  traversal.initScripts = gfalse;

	  if (inlines)
		 traversal.loadInlines = GTraversal::LOAD_ALL;
	  else { 
		traversal.loadInlines = GTraversal::DONT_LOAD;
	  }	

	  traversal.loadSounds = GTraversal::DONT_LOAD;

	  if (textures)  { 
		    traversal.loadTextures = GTraversal::LOAD_ALL;
		    traversal.loadMovies = GTraversal::LOAD_ALL;
			if (useSound ) traversal.loadSounds = GTraversal::LOAD_ALL;
	  }
	  
	  traversal.maxInlineLevel = maxInlineDepth;

	  traversal.InitializeAttributes();

	  traversal.SetCamera(globalCamera);
	  traversal.SetPhysicalCamera(GetPhysicalCamera());

	  traversal.traverseAll = 1;
	  traversal.traverseTransformHierarchyOnly=1;

	  if (inlines || textures) traversal.stopOnFirst = oneOnly;
	  //else traversal.resolveRelativeUrls = gtrue;	  

	  nodes.Traverse(traversal);
	  GFileState state=S_NOT_LOADED;
	  for (int i=0;i<nodes.getNum();i++) {
		  GvNode *n= nodes.get1(i);
		  if (!n) continue;
		  if (RTISA(n,GvImageTexture)) { GvImageTexture *v= (GvImageTexture*) n; state=max(state,v->loader.GetState()); }
		  else if (RTISA(n,Gv2Inline)) { Gv2Inline *v= (Gv2Inline*) n; state=max(state,v->loader.GetState()); }
		  else if (RTISA(n,GvAudioClip)) { GvAudioClip *v= (GvAudioClip*) n; state=max(state,v->loader.GetState()); }
		  else if (RTISA(n,GvExternProto)) { GvExternProto *v= (GvExternProto*) n; state=max(state,v->loader.GetState()); }
		  else if (RTISA(n,GvMovieTexture)) { GvMovieTexture *v= (GvMovieTexture*) n; state=max(state,v->loader.GetState()); }
		  else if (RTISA(n,GvScript)) { GvScript *v= (GvScript*) n; state=max(state,v->loader.GetState()); }

	  }	

	  return(state);
}

// evaluate & set the permission level for the script
	
void GView::GetScriptPermissions(GvScript *script, const char *url)
{
	script->permission = GvScript::SCRIPT_PERMISSION_ALL;
}



//@cmember do we have texture nodes ?
gbool GView::SceneHasTextures() 
{
	  if (sceneInfo && sceneInfo->HasTextures())
		  return TRUE;

	  if (background) return (TRUE);
	  return FALSE;
}

// check heuristic, if some inlines should be deleted 
int GView::CheckInlineUnload()
{
	  int num=0;
	  if (sceneInfo) {
		  int l = sceneInfo->inlines.Length();
		  if (l> activeInlinesLimit) {
			  //int num =  0.1f*l; // % to unload 
			  num = activeInlinesPurge * 1000;
			  num=sceneInfo->UnloadSomeInlines(theTime - ( 4.0*(float) lastFrameTime*0.001f),activeInlinesLimit,num);

			  if (num >0) {
			  TRACE("%d inline nodes purged from memory \n",num);
		  
			  //GetReporter()->Warning("%d from %d inline nodes purged from memory \n",num,l);
	
#ifdef _D3D
			  if (num>0) {
				renderState.DeleteUnusedExBufs();
    		 	renderState.DeleteUnusedTextures();
				renderState.DeleteUnusedMaterials();
				renderState.DeleteUnusedLights();	 
			  }  	
#endif
			  
			  }

		  }
	  }	
	  return(num);

}

// update (refresh) scene info
int GView::UpdateSceneInfo(gbool fullInfo)
{
	
	int i;
	TRACE("Updating scene info \n");

	if (!fullInfo) 
	   CheckInlineUnload();

	if (sceneInfo) {
	  // call shutdown functions on scripts with only 1 ref
	  for(int i=0; i < sceneInfo->scripts.Length(); i++) {
		  GvScript *s = (GvScript *) (GvNode*) sceneInfo->scripts[i];
		  if (!s) continue;
		  
		  if (s->refCnt >1) continue;

		  s->ref();

		  if (s->state == GvScript::SCRIPT_LOADED_AND_INITIALIZED)
			  s->shutdown();

  		  s->flushNodeFields();

		  s->unref();
	  }	
    }


	  GGvSceneInfoTraversal sceneInfoT;

	  sceneInfoT.view = this;
	  sceneInfoT.sceneInfo.full = fullInfo;

	  sceneInfoT.InitializeAttributes();
	  sceneInfoT.traverseAll = 1;
	  sceneInfoT.traverseTransformHierarchyOnly=0;	
	  
	  sceneInfoT.nodeDict = new GvDict();
	  sceneInfoT.initScripts = gfalse; // do it later

      TRACE("Updating Scene info \n");

	  Traverse(sceneInfoT);

	  delete sceneInfoT.nodeDict;
	  sceneInfoT.nodeDict = NULL;

	  
	  gbool wasAnimated = FALSE;
	  if (!sceneInfo) sceneInfo = new GvSceneInfo();
	  else {
		  wasAnimated = time.Enabled() && !time.Paused();
		  sceneInfo->Flush();
	  }	

	  
	  *sceneInfo = sceneInfoT.sceneInfo;

	  if (!wasAnimated && (sceneInfo->IsAnimated() || sceneInfo->HasTimeSensors() || sceneInfo->HasMedia())) {
		  // now we have timers, enable animation
		  if (autoPlay) {
			time.Enable();
			time.pause = 0;
		  }
      } 
	  doUpdateSceneInfo = 0;

	  if (GetScene()) {
		  GetScene()->removeUnusedRoutes();
	  }

	  // call initialize functions on loaded but unitialized scripts
	  for(i=0; i < sceneInfo->scripts.Length(); i++) {
		  GvScript *s = (GvScript *) (GvNode*) sceneInfo->scripts[i];
		  s->isRegistered = TRUE;
		  if (s->state == GvScript::SCRIPT_LOADED)
			  s->initialize();
	  }	

	  for(i=0; i < sceneInfo->timeSensors.Length(); i++) {
		  GvTimeSensor *s = (GvTimeSensor *) (GvNode*) sceneInfo->timeSensors[i];
		  s->isRegistered = TRUE;
	  }	
	  for(i=0; i < sceneInfo->menueSensors.Length(); i++) {
		  GvMenuSensor *s = (GvMenuSensor *) (GvNode*) sceneInfo->menueSensors[i];
		  s->isRegistered = TRUE;
	  }	
	  for(i=0; i < sceneInfo->movieTextures.Length(); i++) {
		  GvMovieTexture *s = (GvMovieTexture *) (GvNode*) sceneInfo->movieTextures[i];
		  s->isRegistered = TRUE;
	  }	
	  for(i=0; i < sceneInfo->audioClips.Length(); i++) {
		  GvAudioClip *s = (GvAudioClip *) (GvNode*) sceneInfo->audioClips[i];
		  s->isRegistered = TRUE;
	  }	
	  // check if current bound viepoint has changed position in list 
	  //
	  if (viewpointIndex >=0 && sceneInfo && viewpointIndex <sceneInfo->cameras.Length()) {
		  if (boundViewpoint.get() != (GvNode*) sceneInfo->cameras[viewpointIndex]) {
			  for(i=0; i < sceneInfo->cameras.Length(); i++) {
				  if (boundViewpoint.get() == (GvNode*) sceneInfo->cameras[i]) {
					  viewpointIndex = i;
					  break;
				  }	
			  }	
		  }	
	  } else viewpointIndex = -1;

	  callPendingScripts();

	  return(1);
}

// update scene info incrementally 
int GView::UpdateSceneInfo(GvNode *node)
{
	doUpdateSceneInfo = 1;
	// to do : incrementally update scene info
	return 0;
}


// get the scene info 
GvSceneInfo *GView::GetSceneInfo()
{

	if (( doUpdateSceneInfo>0) || sceneInfo == NULL) 
		UpdateSceneInfo();
	return (sceneInfo);
}

// register a new TimeSensor
void GView::Register(GvTimeSensor *s)
{
	if (sceneInfo) {
		sceneInfo->timeSensors.add(s);
		s->isRegistered = TRUE;
	    if (!time.Enabled() && autoPlay) {
			time.Enable();
			time.pause = 0;
		}
		// might be bad idea to trigger during rendering 
		// s->OnTimeChange(theTime); // trigger it anyway 
	}
}

// register a new MovieTexture
void GView::Register(GvMovieTexture *s)
{
	if (sceneInfo) {
		sceneInfo->movieTextures.add(s);
		s->isRegistered = TRUE;
	    if (!time.Enabled() && autoPlay) {
			time.Enable();
			time.pause = 0;
		}
		s->OnTimeChange(theTime); // trigger it anyway 
	}
}

// register a new AudioClip
void GView::Register(GvAudioClip *s)
{
	if (sceneInfo) {
		sceneInfo->audioClips.add(s);
		s->isRegistered = TRUE;
	    if (!time.Enabled() && autoPlay) {
			time.Enable();
			time.pause = 0;
		}
		// might be bad idea to trigger during rendering 
		// s->OnTimeChange(theTime); // trigger it anyway 
	}
}

void GView::Register(GvSound *s)
{
	if (sceneInfo) {
		sceneInfo->sounds.add(s);
		s->isRegistered = TRUE;
		if (!time.Enabled() && autoPlay) {
			time.Enable();
			time.pause = 0;
		}
	}
}
void GView::RegisterSpecialMedia(GvNode *s)
{
	if (sceneInfo) {
		sceneInfo->specialMedia.add(s);
	}
}

// register a new MenueSensor
void GView::Register(GvMenuSensor *s)
{
	if (sceneInfo) {
		sceneInfo->menueSensors.add(s);
		s->isRegistered = TRUE;
	}
}

// register a new Script
void GView::Register(GvScript *s)
{
	if (sceneInfo) {
		sceneInfo->scripts.add(s);
		s->isRegistered = TRUE;
		// might be bad idea to trigger during rendering 
		if (s->state == GvScript::SCRIPT_LOADED)
			//	 s->initialize();  // ??? 
			doUpdateSceneInfo = 2; // will call the initialize functions

	}
}

// register a new Viewpoint
void GView::Register(GvViewpoint *s)
{
	if (sceneInfo) {
	}
	if (s->description.get().getLength()>0)
		doUpdateSceneInfo = 1; // we need the path
	s->isRegistered = TRUE;

}

// register a new Light
void GView::Register(GvLight *s)
{
	if (sceneInfo) {
	}
	if (!RTISA(s,GvDirectionalLight)) // these are local 
		doUpdateSceneInfo = 1; // we need the path
	s->isRegistered = TRUE;
}

// register a new Gv2Inline
void GView::Register(Gv2Inline *s)
{
	if (sceneInfo) {
		sceneInfo->inlines.add(s);
		s->isRegistered = TRUE;
	}
}


// Register special nodes 
void GView::RegisterNode(GvNode *n)
{
	 if (n == NULL) return;  
	 if (RTISA(n,GvScript)) {
			Register((GvScript *) n); return;
	  } 
	  if (RTISA(n,GvTimeSensor)) {
			 Register((GvTimeSensor *) n); return;
	  } 
	  if (RTISA(n,GvMovieTexture)) {
			 Register((GvMovieTexture *) n); return;
	  } 
	  if (RTISA(n,GvAudioClip)) {
			 Register((GvAudioClip *) n); return;
	  } 
	  if (RTISA(n,GvLight)) {
			Register((GvLight *) n); return;
	  } 
	  if (RTISA(n,GvViewpoint)) {
			Register((GvViewpoint *) n); return; 
	  } 
	  if (RTISA(n,GvMenuSensor)) {
			Register((GvMenuSensor *) n); return; 
	  } 
	  if (RTISA(n,Gv2Inline)) {
			Register((Gv2Inline *) n); return; 
	  } 

}
void GView::RegisterNode(GvMFNode &nodes)
{
	for(int i=0; i<nodes.getNum();i++) RegisterNode(nodes.values[i]);
}


// update scene bounding Box
int GView::UpdateBoundingBox()
{

	  doUpdateBBox=0;

	  GGvBBoxTraversal traversal;

      TRACE("Updating Scene bounding box\n");
	  traversal.view = this;
	  traversal.SetCamera(globalCamera);
	  traversal.SetPhysicalCamera(GetPhysicalCamera());

	  //traversal.useBBoxes = FALSE; // default
	  //traversal.doViewCulling = FALSE; // default

	  traversal.traverseAll = gtrue;
	  traversal.traverseTransformHierarchyOnly=gtrue;


      traversal.InitializeAttributes();

	  // set current time
	  GTimeStack *ts= GTimeStack::NewElement();
	  ts->time = time; 
	  traversal.Set(ts); 


	  int ret = Traverse(traversal);
	  
      bbox = traversal.bbox;
	  
	  return(ret);
}

// get absolute home url
const char * GView::GetHomeUrl() const
{
	if (theScene) return GetScene()->GetHomeUrl();
	if (file) return file->GetHomeUrl();
	return NULL;
}

// get the absolute url
const char * GView::GetUrl() const
{
	if (theScene) return GetScene()->GetUrl();
	if (file) return file->GetUrl();
	return NULL;
}


// bind initial scene nodes 
void GView::BindScene()
{
	SetTheTime();
	
	loadTime = GTime::GetGlobalTime();

#ifdef _D3D
	renderState.ReleaseLights(); 
	renderState.DeleteUnusedLights();
#endif
	  // already done ?????

/*	  
	  GGvBBoxTraversal bboxTraversal;
	  state.view = this;
	  bboxTraversal.SetCamera(camera);
	  bboxTraversal.SetPhysicalCamera(GetPhysicalCamera());

	  bboxTraversal.InitializeAttributes();

	  int ret = Traverse(bboxTraversal,scene);
	  
	  bbox = bboxTraversal.bbox;

	  if (!bbox.IsEmpty()) {
		camera->SetByVolume(bbox.mi,bbox.ma);
		camera->Zoom(1.2);
		camera->SetWorldReference(bbox);
	  } else {
		camera->SetByVolume(-1.5,1.5,-1.5,1.5);
	  }
*/
      renderState.SetCamera(camera);
	  
	  if (!bbox.IsEmpty()) 
         GetScene()->setBBox(bbox); // test 

	  SetHeadLight(TRUE);

	  renderState.SetBackfaceCulling(0); // seems to be better

#ifdef _OGL
	  renderState.textureGeneration.Set("off"); // vrml has texture coordinates
	  renderState.textureGeneration.glSet();
#endif
	  
	  renderState.SetNormalize(1);

	  //renderState.SetTexturing(1);
	  //renderState.faces.textured = 1;
	  SetRenderMode(GRenderGouraud);
  	  
  	  // use local coloring 
	  SetLocalColors(1);
	  SetBackgroundColor(Point(0,0,0)); // black

      SetViewpointIndex(-1);

	  if (!sceneInfo) return;

	  // VRML 1.0 style viewer settings by parsing strings of info nodes
#ifdef _G_VRML1

	  if (sceneInfo->backgroundColor) {
	  	Point color;
		int cnt = sscanf(sceneInfo->backgroundColor->string.value,"%f %f %f", &color.x,&color.y,&color.z);
		if (cnt == 3) {
			SetBackgroundColor(color);
		}
	  }
	  
      if (useBackground && sceneInfo->backgroundImage) {
            SetBackgroundImage(sceneInfo->backgroundImage->string.value);
      }
	  if (viewController) {
	  if (sceneInfo->viewerInfo) {
	  	CString tmp(sceneInfo->viewerInfo->string.value);
		
        tmp.MakeLower();
		if (tmp == "walk") { 
				viewController->SetCameraMode(WALK_MODE);
		}
		else if (tmp == "plane") { 
				viewController->SetCameraMode(WALK_MODE);
		}
		else if (tmp == "fly") { 
		//		viewController->SetCameraMode(FLY_MODE);
				viewController->SetCameraMode(WALK_MODE);
		}
		else if (tmp == "examine") viewController->SetCameraMode(ORBIT_MODE);
        else viewController->SetCameraMode(tmp);
	  }
	  else 	viewController->SetCameraMode(WALK_MODE);
		  //viewController->SetCameraMode(ORBIT_MODE); // or WALK default ???
	  }


      if (sceneInfo->renderMode) {
	  	CString tmp(sceneInfo->renderMode->string.value);
		tmp.MakeLower();
        GRenderMode mode = (GRenderMode) GRenderModeLookup(tmp);
	    SetRenderMode(mode);
      }
#endif _G_VRML1


	  // VRML 2 scene hints 
	  if (sceneInfo->firstBackground)
		  BindBackground(sceneInfo->firstBackground);

	  if (sceneInfo->firstFog)
		  BindFog(sceneInfo->firstFog);

	  if (sceneInfo->firstNavigationInfo)
		  BindNavigationInfo(sceneInfo->firstNavigationInfo);

	  gbool topLevelLight = FALSE;

	  GvScene *s = GetScene();
	  if (s)
	  for (int i=0; i< s->children.getNum(); i++) {
		  GvNode *n = s->children.get1(i);
		  if (n && RTISA(n,GvLight)) {
				topLevelLight = TRUE;
				GvLight *l = (GvLight *) n;
			 	l->global = TRUE; // its a global light
		  }		
	  }	
	  // temp workaround for MU-Worlds, to make sure Avatar's get light
	  if (!topLevelLight )
      //    && (observer != NULL)
      {
			for (int i=0; i< sceneInfo->lightPath.Length(); i++) {
		 		TRACE("Making light global for MU world ");	
			 	GvLight *l = (GvLight *) (GvNode*) sceneInfo->lights[i];
			 	l->global = TRUE;
			}
	  }
	  




      // get any camera path animation 

      // set first encountered camera  
      if (useInitialSceneCamera 
	  	  && (sceneInfo->cameras.Length()>0)) {
            SetViewpointIndex(0);
            SetCamera(sceneInfo->cameras[0],&sceneInfo->cameraTransforms[0]);
			// set autoanimate off
	  	    //if (viewController) viewController->SetAutoAnimate();
	  } else {
					 // no viewpoint 
/*
		  if (scene.vrml2) {
			  // test 
			  GvViewpoint *viewpoint = new GvViewpoint();
			  SetCamera(viewpoint,NULL,FALSE);
		  }
*/
	  }

#ifdef _G_VRML1

      if (sceneInfo->animateViewpoints) {
	  	CString tmp(sceneInfo->animateViewpoints->string.value);
		tmp.MakeLower();
		if (tmp == "true") { 
            AnimateAllCameras();
        }
      }  
#endif _G_VRML1

	  GvNode *zone;
	  if ((zone=GetScene()->getNode("SharedZone")) != NULL) {
		  sharedZone.set(zone);
	  }	
	  else sharedZone.set((GvNode*)NULL);


	  // bind 3rd person avatar 
#if 0
		// 22.08, sportsline had Texture named "Avatar" grr....
	  GvNode *avatar;
	  if ((avatar=GetScene()->getNode("Avatar")) != NULL) {
		  myAvatar.set(avatar);
		  BindAvatar(avatar);
		  SetThirdPersonView(IsAvatarShown());

	  }	
	  else 
#endif
	  if (thirdPersonView) { // reenable myAvatar
		  BindAvatar(myAvatar);
		  SetThirdPersonView(IsAvatarShown());
	  }	
   
	  // get viewer info  // top camera ....
	  // textured ?
}



// read a model from a 3d file
int GView::ReadModel(const char *FileName,int fmt,GFile *theFile)
{  int ret=0;

#ifdef _D3D
	if (!device->bRenderingIsOK) {
		TRACE("Rendering device not ok !\n");
		ASSERT(0);
		return ret;
	}
#endif
   gbool isAudio = (fmt == WAV_FMT) || (fmt == MIDI_FMT) || (fmt == AU_FMT);

   if (!isAudio) {

   //SetShell(NULL);

    // why here ?????? 

   // reset some basic poperties
   ClearBackground();
   ClearTexture();
   // to do: material ???
   // rendermode ??
   renderState.SetMaterialAlphaOff();
   renderState.SetLitMaterial(TRUE);
   renderState.SetAlphaTestFromTexture(FALSE);
   
   // renderState.SetTexturing(0);
   SetTexturing(0);

   SetTexturing(1); // Test 


   // fog
   // material 

   time.Reset();
   time.enabled= 0;
   loadTime = GTime::GetGlobalTime();
   
   }


   switch (fmt) {
   default :
#ifdef _VRML

   case  VRML_FMT :
   {
	  // create new scene	
	  GvScene *scene = new GvScene();
	  scene->setBrowser(this);

	  scene->ref();
	  if (theFile) {
		  // the url's	
		  scene->worldUrl.set(theFile->GetUrl());
		  scene->baseUrl.set(theFile->GetHomeUrl());
	  }  	

	  ret =	  ReadVrmlFile(FileName,GetReporter(),scene);

      SetScene(scene,TRUE);   // calls updateSceneInfo && bbox 

	  scene->unref();

   }
   break;
#endif


#ifdef _DXF
   case  DXF_FMT:
   {
	  SetScene(NULL);

	  if (theWorld) { theWorld->Empty(); }
	  else theWorld = new GWorld;
	  ret =	  ReadDxfFile(FileName,theWorld);
	  
	  GBBoxTraversal bboxTraversal;
	  bboxTraversal.view = this;
   	  bboxTraversal.InitializeAttributes();
	  InitializeGvAttributes(bboxTraversal);


	  theWorld->Traverse(bboxTraversal);

  	  bbox = bboxTraversal.bbox;

	  camera->SetByVolume(bboxTraversal.bbox.mi,bboxTraversal.bbox.ma);
	  camera->Zoom(1.2);
 	  
 	  bbox = bboxTraversal.bbox;
 	  camera->SetWorldReference(bbox);

      renderState.SetCamera(camera);
	  CameraReset(); // strange on first time airplane.dxf ???

	  //SetDefaultTextureParameters();

	  renderState.SetTexturing(0);
	  renderState.SetBackfaceCulling(0);
	  renderState.SetNormalize(1);

   }
   break;
#endif
   case HTML_FMT:
   		ret = -1;
   		break;

   case AVI_FMT: 
   case MPG_FMT: 
   case QT_FMT: 
   case MPS_FMT: 
   case RMA_FMT:

   case GIF_FMT: 
			{
				/* create rect with MovieTexture */ 
				int width=1,height=1;
				float aspect = 1.0;
				BOOL isAnimated = TRUE;

				GvScene *scene = new GvScene();
				scene->setBrowser(this);
				scene->ref();

				if (theFile) {
					scene->worldUrl.set(theFile->GetUrl());
					scene->baseUrl.set(theFile->GetHomeUrl());
				}  	


				GvNodeHandle n;
				GvMFString url(0);
				url.makeRoom(1);
				url.set1(0,FileName);
				if (theFile && (theFile->url.GetLength()>0) ) {
					url.makeRoom(2);
					url.set1(1,(const char *)theFile->url);
				}

				n = newMovieTFace((const Point *) center,url);
				scene->addChild(n);
			
				SetScene(scene,TRUE);
				scene->unref();

			}
			break;
   // case GIF_FMT:
   case RGB_FMT:
   case TGA_FMT:
   case JPEG_FMT:
   case PPM_FMT:
   case PNG_FMT:
   case PCX_FMT:
   case DIB_FMT:
	  ret = ReadTexturedPolygon(FileName,fmt);
	  break;

   case WAV_FMT:
   case AU_FMT:
   case MIDI_FMT:		   

	        if (PlaySoundFile(FileName)) {
					ret=0;
			} else ret = -1;
			break;
   

   case D3V_FMT :
   case GEO_FMT :
   case RAW_FMT :
   case OBJ_FMT :
   case STL_FMT :
   {

	  ret= ReadShell(FileName,fmt);
	  break;
   }

   } // switch 

   return(ret);
}


// set scene to the initial default state
// bind initial nodes etc.
int GView::SetSceneDefaults()
{

	return(0);
}


// read a shell from a 3d file
int GView::ReadShell(const char *FileName,int fmt)
{  int ret=-1;

   GShell * newShell = NULL;

#ifndef _GV_MINIMAL
   ret= ::ReadShell(FileName,fmt,newShell);
#endif

   SetShell(newShell);

   if (theShell) {
	   renderState.SetTexturing(0);
	   if ((fmt == GEO_FMT) || (fmt == RAW_FMT))
	        renderState.SetBackfaceCulling(0);
	   else renderState.SetBackfaceCulling(1);

	   if (theShell->vc.Length() || theShell->fc.Length()) renderState.faces.colored = 1 ;

	   // trigger world url event out
	   worldUrl.set(GetUrl());

   }
   return(theShell != NULL);
}

// read a model from any known file types
//
int GView::ReadModel(const char *FileName) 
{
 int ret = -1;

 GetReporter()->Status("Reading file '%s' ...",FileName);

 int isZipped = IsZippedFile(FileName);
 int fmt = GetFileType(FileName); 

 if (isZipped) {
 	char unzippedFile[_MAX_PATH];
	ret = UnZipFile(FileName,unzippedFile,fmt,TRUE,TRUE);
	if (ret >=0) {
	  ret = ReadModel(unzippedFile);
	  return(ret);
	}

 } 
 
 if (fmt != UNKNOWN_FMT) {
 	  ret = ReadModel(FileName,fmt);
 } else 
 {
 	GetReporter()->Error("Read model : unknown file extension for file '%s'",FileName);
	ret = -1;
 }
 return(ret);
}

// read a model from a initilialized and preprocessed GFile structure 
// this view will onw newFile
int GView::ReadModel(GFile *newFile) 
{ int ret = -1;

#ifdef _D3D
	if (!device->bRenderingIsOK) {
		TRACE("Rendering device not ok !\n");
		ASSERT(0);
		return ret;
	}
#endif

  if (newFile->localFileUnzipped.GetLength()>0) {
	    newFile->ref();
		
		ret = ReadModel(newFile->localFileUnzipped,newFile->localFileFmt,newFile);
		
		if (ret >=0) {
			if (file) {
				file->unref();
			}
   			file = newFile;
			file->ref();   // add a extra control
#ifdef _DEBUG
			// file->lastModified = CTime::GetCurrentTime(); // for refresh testing 
#endif

#ifdef G_CACHE
			
			if (file->lastModified != file->cacheLastModified) {
				if (file->cacheLastModified > 0) {

					// the URL was retreived and had diffent modifiedDate then file in cache
					file->ifModifiedSince = CTime::GetCurrentTime(); // refresh 
					TRACE("TOP-Level url changed, refreshing children \n");
#ifdef _DEBUG
					GetReporter()->Trace("TOP-Level %s url changed, refreshing children",(const char *) file->url);
#endif	
				}
			}
			// set the verify date for child downloads
			else { 
				CTime refresh= theCache.getRefreshTime();
				file->ifModifiedSince = file->lastModified;
				if (theCache.cacheMode == GCACHE_DAYS) { 
					if (refresh < file->ifModifiedSince) file->ifModifiedSince = refresh;
				}
			}

#ifdef _DEBUG
			time_t t= file->ifModifiedSince.GetTime();
			TRACE("GView::ReadModel:Setting top level ifModifiedSince to %d  %s \n",file->ifModifiedSince.GetTime() ,ctime(&t));
			t= file->lastModified.GetTime();
			TRACE("GView:: last modified %s \n",ctime(&t));
#endif
			if (newFile->urlExtraArgs.GetLength()>0) { 
				SetCamera(newFile->urlExtraArgs,FALSE);
			}
#endif	
		}
		else {
			if (newFile->DeleteCachedLocalFile()) {
				GetReporter()->Warning("Deleted probably corrupt world from cache");
			}

		}
		newFile->unref();
  } 
  else ret=-1;
 
  return(ret); 
}



// build a shell by assigning a image texture to a rectangle
int GView::ReadTexturedPolygon(const char *FileName,int format)
{	// create new scene	
	  GvScene *scene = new GvScene();
	  scene->setBrowser(this);
	  scene->ref();
	  if (file) {
		  // the url's	
		  scene->worldUrl.set(file->GetUrl());
		  scene->baseUrl.set(file->GetHomeUrl());
	  }  	
	  GvNode *n = TextureToNode(format,FileName,((format == GIF_FMT) ? OP_MOVIE_RECT : OP_TEXTURED_RECT));
	  if (n) scene->addNode(n);
	  
      SetScene(scene,TRUE);   // calls updateSceneInfo && bbox 
	  scene->unref();

	if (viewController) viewController->SetCameraMode(ORBIT_MODE);

   return(n != NULL);
}

// load a global texture
int GView::SetTexture(const char *FileName,int format)
{
    int ret;
	
	BOOL hasAlpha=FALSE;
	BOOL isGrayscale=FALSE;
	BOOL hasColorKey=FALSE;
	unsigned char	components;
	short			sizeX,sizeY;
	short			textureFormat;


    if (FileName[0] == 0) {
            ClearTexture();
            return(0);
    }

#ifdef _OGL
	renderState.textureCache.Begin(GL_COMPILE_AND_EXECUTE);
	ret = InitImageTexture(format, FileName,0,
				renderState.textureParameters.NeedMipmaps(),
				renderState.textureParameters.max_size_x,renderState.textureParameters.max_size_y,hasAlpha,hasColorKey,isGrayscale,components,sizeX,sizeY,textureFormat);
	renderState.textureCache.End();
    // to do error code:
    if (ret>=0) renderState.textureDefined = 1;
#endif

#ifdef _D3D
    ret = renderState.SetDefaultTexture(FileName,format);
    renderState.textureDefined = ret>=0;
    hasAlpha = renderState.defaultTexture.HasAlpha();

#endif

#ifdef _OGL
	   if (hasAlpha) {  // change state
           renderState.SetAlphaFunc(GL_GREATER,0.0);
           renderState.SetAlphaTestFromTexture(TRUE);
	   } else {
           renderState.SetAlphaTestFromTexture(FALSE);
	   }
#endif

#ifdef _D3D
	   if (hasAlpha) {  // change state
		  renderState.SetBlendFromMaterial(1);
	   } else {
		   renderState.SetBlendFromMaterial(0);
	   }
#endif
	   

    
    textureFile = FileName;

	return(ret);
}


// load a global texture
int GView::EditTexture(const char *FileName,int format)
{
    int ret = SetTexture(FileName,format);
    if (ret) {
    }

    return(ret);

}


//unload the global texture
int GView::ClearTexture()
{
#ifdef _OGL
	renderState.textureCache.Delete();
    renderState.SetAlphaTestFromTexture(FALSE);
	renderState.SetTextureMatrixIdentity();

#endif
#ifdef _D3D
     renderState.ReleaseDefaultTexture();
	 renderState.SetTextureMatrixIdentity();
	 renderState.SetBlendFromMaterial(0);
#endif
    renderState.textureDefined = 0;

    //xxx for now renderState.SetTextureTransform(Transform3()); // set to identity 
    textureFile = "";
	return(0);
}


#ifndef _GV_MINIMAL

// Create a VRML scene out of single object model
GvNode* GView::CreateVrmlScene(gbool vrml2)
{
	GvScene *top = new GvScene();

	if (vrml2) {	// VRML 2 Mode 

		// to do:
		top->vrml2.set(TRUE);
		
    
    if (1) {
        Point c;
        GetBackgroundColor(c);
        GvBackground *n = new GvBackground();

        n->skyColor.setLength(1);
		n->skyColor.set1(0,&c.x);
		if (backgroundImage.GetLength()>0) {
           // to do: relative filename 
           n->backUrl.set_(backgroundImage);
           top->addChild(n);
		}


        n->setName(GvName("Background"));
        top->addChild(n);
    }

    // texture2transform

	GvShape *theShape = new GvShape();
	GvAppearance *theAppearance = new GvAppearance();
	theShape->appearance.set(theAppearance);

    if (textureFile.GetLength()>0) {
           GvImageTexture *n = new GvImageTexture();
           // to do: relative filename 
           n->url.set_(textureFile);
           n->setName(GvName("Texture"));
           theAppearance->texture.set(n);

           //if (!renderState.textureTransform3.IsIdentity()) 
           Gv2TextureTransform *t= new Gv2TextureTransform();
           t->translation.set(renderState.textureTransform3.translate);
           t->scale.set(renderState.textureTransform3.scale);
           t->rotation.set(renderState.textureTransform3.rotate.z);
           t->setName(GvName("TextureTransform"));
           theAppearance->textureTransform.set(t);

    }

    if (1) {
           Point Ambient, Diffuse, Specular,Emission;
           float Shininess,Alpha;

           if (GetMaterial(Ambient,Diffuse,Specular,Emission,Shininess,Alpha)) {
                Gv2Material *n = new Gv2Material();

                n->ambientIntensity.set(Ambient.x); // gray ??
                n->diffuseColor.set(Diffuse);
                n->specularColor.set(Specular);
                n->emissiveColor.set(Emission);
                
                n->shininess.set(Shininess / GV_SHININESS_GL_SCALE);
                n->transparency.set(1.0 - Alpha);
				n->checkForDefaultValues(1);
                n->setName(GvName("Material"));
				theAppearance->material.set(n);
           } 
    }
	gbool solid = GetBackfaceCulling();
	gbool ccw = !GetFrontFaceCW();

    if (1) {
        GvViewpoint *c = new GvViewpoint();
        Vector axis;
        float angle;
        ConvertCameraToOrientation(camera->position, camera->target, camera->up, axis, angle);

        c->position.set(camera->position);
        c->orientation.set(Rotation(axis.x,axis.y,axis.z,angle));
        //c->focalDistance.set(camera->TargetDistance());
        c->fieldOfView.set(camera->GetFovY());
		c->description.set("Start");
        top->addChild(c);
    }

  
    // camera

    if (theShell) {

		Gv2IndexedFaceSet *result=NULL; // resulting node 

		Matrix *m= NULL;
		Matrix *tm= NULL;

        //GvNodeShell *shellNode = new GvNodeShell;
		//shellNode->SetShell(theShell); // new 4.06.96

		//top->addChild(shellNode);
        //top->addChild(theShell);

		if (RTISA(theShell,GShellI)) {
			GShellI *shellI = (GShellI*) theShell;

			result = CreateIndexedFaceSet((GShellI*) theShell,!ccw, m,tm,solid);
		}
		else if (RTISA(theShell,GShell)) {
			GShell *s = (GShell*) theShell;
			result = CreateIndexedFaceSet(s,!ccw, m,tm);
		}

		if (result) {
			if (!solid) result->solid.set(FALSE);
			// if (!ccw) result->ccw.set(FALSE); was wrapped above 
			// shell is triangulated so not convex 
			// creaseAngle 

			theShape->geometry.set(result);

		}

    }
    
	top->addChild(theShape);

    // add all other cameras 
    for (int i=0; i<viewpoints.Length(); i++) {
        top->addChild(viewpoints.get1(i));
    }


	}
#ifdef _G_VRML1
	else {	// VRML 1.0 
	top->vrml2.set(FALSE);
    
    if (backgroundImage.GetLength()>0) {
           GvInfo *n = new GvInfo();
           // to do: relative filename 
           n->string.set_(backgroundImage);
           n->setName(GvName("BackgroundImage"));
           top->addChild(n);
    }
    
    if (1) {
        Point c;
        GetBackgroundColor(c);

        char s[120];
  	    sprintf(s,"%f %f %f", c.x,c.y,c.z);
        GvInfo *n = new GvInfo();
        n->string.set_(s);
        n->setName(GvName("BackgroundColor"));
        top->addChild(n);
    }

    // texture2transform

    if (textureFile.GetLength()>0) {
           GvTexture2 *n = new GvTexture2();
           // to do: relative filename 
           n->filename.set_(textureFile);
           n->setName(GvName("Texture"));
           top->addChild(n);

           //if (!renderState.textureTransform3.IsIdentity()) 
           GvTexture2Transform *t= new GvTexture2Transform();
           t->translation.set(renderState.textureTransform3.translate);
           t->scaleFactor.set(renderState.textureTransform3.scale);
           t->rotation.set(renderState.textureTransform3.rotate.z);
           t->setName(GvName("TextureTransform"));
           top->addChild(t);
    }

    if (1) {
           Point Ambient, Diffuse, Specular,Emission;
           float Shininess,Alpha;

           if (GetMaterial(Ambient,Diffuse,Specular,Emission,Shininess,Alpha)) {
                GvMaterial *n = new GvMaterial();

                n->ambientColor.set(Ambient);
                n->diffuseColor.set(Diffuse);
                n->specularColor.set(Specular);
                n->emissiveColor.set(Emission);
                
                n->shininess.set(Shininess / GV_SHININESS_GL_SCALE);
                n->transparency.set(1.0 - Alpha);

                n->setName(GvName("Material"));
                top->addChild(n);
           } 
    }
    if (1) {
        GvShapeHints *h = new GvShapeHints();
        h->shapeType.set(GetBackfaceCulling() ? GvShapeHints::SOLID : GvShapeHints::UNKNOWN_SHAPE_TYPE);
        h->vertexOrdering.set(!GetFrontFaceCW() ? GvShapeHints::COUNTERCLOCKWISE : GvShapeHints::CLOCKWISE);
        h->faceType.set(GvShapeHints::CONVEX);
        top->addChild(h);
    }
    if (1) {
        GvPerspectiveCamera *c = new GvPerspectiveCamera();
        Vector axis;
        float angle;
        ConvertCameraToOrientation(camera->position, camera->target, camera->up, axis, angle);

        c->position.set(camera->position);
        c->orientation.set(Rotation(axis.x,axis.y,axis.z,angle));
        c->focalDistance.set(camera->TargetDistance());
        c->heightAngle.set(camera->GetFovY());
        top->addChild(c);
    }

  
    // camera

    if (theShell) {
        GvNodeShell *shellNode = new GvNodeShell;
		shellNode->SetShell(theShell); // new 4.06.96

		top->addChild(shellNode);
        //top->addChild(theShell);
    }

    // add all other cameras 
    for (int i=0; i<viewpoints.Length(); i++) {
        top->addChild(viewpoints.get1(i));
    }
	} // VRML 1.0 
#endif 
   
    return(top);
}

#endif



#if 0

int GView::SetBackgroundShell(GShell *shell)
{
 if (backgroundShell) delete backgroundShell;
 backgroundShell = shell;
 return(0);
}

int GView::ClearBackgroundImage()
{
	backgroundTextureCache.Flush();
	SetBackgroundShell(NULL);
	doBackground = 0;
	return(0);
}


int GView::SetBackgroundImage(const char *FileName,int format)
{  Point p[4],params[4];
   int face[5]= { 4, 0, 1, 2, 3};
   float ratio=1.0;

   backgroundImage = LoadImage(format,FileName,0,1)
   
   SetBackgroundShell(NULL);

   GShell *theShell = new GShell;
   if (theShell) {
	   BBox box;
	   p[0].Set(0.0,0.0,0.0);
	   p[1].Set(0.0,1.0,0.0);
	   p[2].Set(ratio,1.0,0.0);
	   p[3].Set(ratio,0.0,0.0);

	   params[0].Set(0.0,0.0,0.0);
	   params[1].Set(0.0,1.0,0.0);
	   params[2].Set(1.0,1.0,0.0);
	   params[3].Set(1.0,0.0,0.0);

	   theShell->SetV(4,p);
	   theShell->SetVP(4,params);
	   theShell->SetFaceList(5,face);
	   theShell->Update();
	   theShell->ComputeBBox(box);

	   camera->SetByVolume(box.mi,box.ma);
	   bbox = box;
 	   camera->SetWorldReference(bbox);

       renderState.SetCamera(camera);
	   renderState.material.Default();
	   renderState.material.diffuse.Gray(1.0);
	   renderState.material.specular.Black();
	   renderState.material.ambient.Gray(0.5);
	   renderState.material.glSet();
	   renderState.textureGeneration.Set("off");
	   renderState.textureGeneration.glSet();

	   backgroundTextureCache.Begin(GL_COMPILE);
	  
	   InitImageTexture(format, FileName,0, (renderState.textureParameters.NeedMipmaps() ? TEX_HIGH: TEX_LOW));

	   backgroundTextureCache.End();

	   renderState.SetTexturing(1);
	   renderState.faces.textured = 1;
	   SetRenderMode(GRenderSolid);
	   renderState.faces.visible = 1;
	   renderState.edges.visible = 0;
	   renderState.normals.visible = 0;
	   renderState.SetZBuffer(0);
	   renderState.SetBackfaceCulling(0);
   }
   return(theShell != NULL);
}

int GView::RenderBackground()
{  Point p[4],params[4];
   int face[5]= { 4, 0, 1, 2, 3};
   float ratio=1.0;

   GShell * theShell;

   theShell = backgroundShell;
   if (theShell) {
	   BBox box;
	   p[0].Set(0.0,0.0,0.0);
	   p[1].Set(0.0,1.0,0.0);
	   p[2].Set(ratio,1.0,0.0);
	   p[3].Set(ratio,0.0,0.0);

	   params[0].Set(0.0,0.0,0.0);
	   params[1].Set(0.0,1.0,0.0);
	   params[2].Set(1.0,1.0,0.0);
	   params[3].Set(1.0,0.0,0.0);

	   theShell->SetV(4,p);
	   theShell->SetVP(4,params);
	   theShell->SetFaceList(5,face);
	   theShell->Update();
	   theShell->ComputeBBox(box);
	   camera->SetByVolume(box.mi,box.ma);
	   bbox = box;

	   camera->SetWorldReference(bbox);

       renderState.SetCamera(camera);

   	   renderState.material.Default();

	   renderState.material.diffuse.Gray(1.0);
	   renderState.material.specular.Black();
	   renderState.material.ambient.Gray(0.5);
	   renderState.material.glSet();
	   renderState.textureGeneration.Set("off");
	   renderState.textureGeneration.glSet();

	   InitImageTexture(format, FileName,0, (renderState.textureParameters.NeedMipmaps() ? TEX_HIGH: TEX_LOW));

	   renderState.SetTexturing(1);
	   renderState.faces.textured = 1;
	   SetRenderMode(GRenderSolid);
	   renderState.faces.visible = 1;
	   renderState.edges.visible = 0;
	   renderState.normals.visible = 0;
	   renderState.bbox.visible = 0;
	   renderState.SetZBuffer(0);
	   renderState.SetBackfaceCulling(0);
	   
	   theShell->RenderGlFaces(state);
   }
} 
#endif



// define default lights
int GView::DefineLights(const Point &dirx,int headLight, int &firstLightIndex)
{	 int usedLights=0;

#if 1
	// define a light
	  GLfloat direction[4]= { 1.0, 1.0, 1.0, 0.0 };
	  GLfloat color[4] = { .8,.8,.8,1 };

	  Point dir(1.0,1.0,2.0);

	  //if (headLight)
   	      dir = dirx;

	  dir.Normalize();
  
	  direction[0] = dir.x;
	  direction[1] = dir.y;
	  direction[2] = dir.z;

#ifdef _OGL

	  glDisable(GL_LIGHT0);
	  glLightfv(GL_LIGHT0,GL_DIFFUSE,color);
	  glLightfv(GL_LIGHT0,GL_SPECULAR,color);
	  glLightfv(GL_LIGHT0,GL_POSITION,direction);
	  glLighti(GL_LIGHT0,GL_SPOT_CUTOFF,180);
	  glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,1.0);
	  glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,0.0);
      glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,0.0);

      glCheckError();


	  glEnable(GL_LIGHT0);
	  usedLights++;
	  

#if 1
	  glDisable(GL_LIGHT1);
	  glLightfv(GL_LIGHT1,GL_DIFFUSE,color);

	  direction[0]*=-1.0;
	  direction[1]*=-1.0;
	  direction[2]*=-1.0;
	  glLightfv(GL_LIGHT1,GL_DIFFUSE,color);
	  glLightfv(GL_LIGHT1,GL_SPECULAR,color);
	  glLightfv(GL_LIGHT1,GL_POSITION,direction);
	  glLighti(GL_LIGHT1,GL_SPOT_CUTOFF,180);
	  glLightf(GL_LIGHT1,GL_CONSTANT_ATTENUATION,1.0);
	  glLightf(GL_LIGHT1,GL_LINEAR_ATTENUATION,0.0);
      glLightf(GL_LIGHT1,GL_QUADRATIC_ATTENUATION,0.0);
      glCheckError();

	  glEnable(GL_LIGHT1);
	  usedLights++;

#endif
	  firstLightIndex = usedLights;

	  for (int i=usedLights; i<8;i++) {
	  	 glDisable((GLenum)(GL_LIGHT0+i));
	  }	
      glCheckError();

#endif
#endif

	  return(1);
}

void GView::SetDegradeDefaults()
{
	degradeRenderMode = GRenderBoundingBoxes;
	degradeTextured = 0;
	degradeLightMode = 0;
	degradeTransparency = 0;
	degradeLastFrameTime = 0.0;

	degradeSaveRenderMode= GRenderWireframe;
	degradeSaveRenderTextured=0;;
	degradeSaveRenderTransparency = 0;
	degradeSaveRenderLightMode = 0;

}

// start degrading on next update
int GView::BeginDegrade() 
{ 
  SetMoving(1); // assume for now BeginDegrade EndDegrade is valid for updates during mouse move
  if (degradeDuringMove) degrade = 1;  
  return(degrade);
}

// end degrading 
int GView::EndDegrade() { 
   if (degraded) {
      needUpdate++; 
	  UnSetDegradeMode();
	  degraded = 0;
    }   
    degrade = 0;
    SetMoving(0); //
    return(degraded); 
 }


void GView::SetDegradeMode()
{
	GRenderMode mode;
	mode = degradeRenderMode;
	if (GetRenderMode() < (int)mode) mode = (GRenderMode) GetRenderMode();
	degradeSaveRenderMode= (GRenderMode) GetRenderMode();
	degradeSaveRenderTextured=GetTexturing();
	degradeSaveRenderTransparency = GetTransparency();
	degradeSaveRenderLightMode = GetLightMode();

	// any OGL customized drawing mode
	degradeSaveVertices=renderState.vertices;
	degradeSaveNormals=renderState.normals;
	degradeSaveEdges=renderState.edges;  
	degradeSaveFaces=renderState.faces;  
	degradeSaveBboxes=renderState.bboxes; 


	// set degraded rendering mode 
	SetRenderMode(mode);
	SetTexturing(degradeTextured);
	SetTransparency(degradeTransparency);
	SetLightMode(degradeLightMode);
	degraded = 1;
}


void GView::UnSetDegradeMode()
{
	SetRenderMode(degradeSaveRenderMode);
	SetTexturing(degradeSaveRenderTextured);
	SetTransparency(degradeSaveRenderTransparency);
	SetLightMode(degradeSaveRenderLightMode);
	
	// any OGL customized drawing mode
	renderState.vertices= degradeSaveVertices;
	renderState.normals=degradeSaveNormals;
	renderState.edges=degradeSaveEdges;  
	renderState.faces=degradeSaveFaces;  
	renderState.bboxes=degradeSaveBboxes; 

}

// set moving mode, tell avatar about it 
int GView::SetMoving(gbool mode)
{ 
   if (showAvatar && moving != mode ) {
	   SetAvatarGesture(avatarInfo.nodeEvents,mode ? avatarInfo.avatarBeginMove : avatarInfo.avatarEndMove);
   }
   moving = mode; 
   return(moving); 
}

int GView::SetColliding(gbool mode)
{ 
   if (showAvatar && colliding != mode ) {
	   SetAvatarGesture(avatarInfo.nodeEvents,mode ? avatarInfo.avatarBeginCollision : avatarInfo.avatarEndCollision);
   }
   colliding = mode; 
   return(colliding); 
}

int GView::SetFlying(gbool mode)
{ 
   if (showAvatar && flying != mode ) {
	   SetAvatarGesture(avatarInfo.nodeEvents,mode ? avatarInfo.avatarBeginFly : avatarInfo.avatarEndFly);
   }
   flying = mode; 
   return(flying); 
}






// is this bounding box visible with respect to current view volume
// ????
gbool GGeometryTraversal::IsBoxVisible(const BBox &box) const
{
  return(1);
}

 int GViewInfo::View2World(const Point &p, Point &result)
 {
    result = view.Inverse() * p;
    return(1);
 }

 int GViewInfo::World2View(const Point &p, Point &result)
 {
    result = view.M() * p;
    return(1);
 }


 int GViewInfo::World2Cam(const Point &p, Point &result)
 {
    result = cam.M() * p;
	result.x *= -1.f; // ??
	result.y *= -1.f;

    return(1);
 }
 int GViewInfo::Cam2World(const Point &p, Point &result)
 {
    result = cam.Inverse() * p;
    return(1);
 }


#ifdef _OGL
int GViewInfo::glGet()
{

    glMatrix gm;
	Matrix m1, m2;
   	gm.glGet(GL_MODELVIEW_MATRIX);
    gm.Get(m1);

	m1.contents = Matrix::HAS_ANY_TRANSFORM;
    SetObject(m1);

   	gm.glGet(GL_PROJECTION_MATRIX);
    gm.Get(m2);
	m2.contents = Matrix::HAS_TRANSLATION|Matrix::HAS_SCALE|Matrix::HAS_NON_UNIFORM_SCALE|Matrix::HAS_PERSPECTIVE;
//tom:
	m2 = m1 * m2;
//end tom
    SetView(m2);
    viewVolume.mi.Set(-1.0,-1.0,0.0);
    viewVolume.ma.Set(1.0,1.0,0.0);
    return(1);
}    

#endif

// set the global animation time
double GView::SetTheTime()
{
	// setBrowser(this); // set the current global browser variable 
	// GvNode::setTimePtr(&this->theTime);

	double t= Now();
	
	theTime = t;
	time_changed.set(t);

	//XX GvSetTheTime(t);

	thisFrameTimeSensorsTriggered = FALSE; // new TS have not been triggered at this timestamp
	GReporter::current = GetReporter(); // set the global reporter for vrmlscript
	return t;
}



// trigger time sensors
gbool GView::TriggerTimeSensors()
{
	// setBrowser(this); // set the current global browser variable 

	double t= Now(); // get the time 

	theTime = t;
	time_changed.set(t);

	//XX GvSetTheTime(t);  // set the global time

	thisFrameTimeSensorsTriggered = TRUE; // don't want a retrigger in DrawScene
	gbool ret=FALSE;

	if (!sceneInfo) return FALSE;

   	if (sceneInfo->HasTimeSensors()) { 
	//	TRACE("Trigger Time Sensors for t %g \n",t);
        sceneInfo->TriggerTimeSensors(t);
		ret = TRUE;
    }
   	if (sceneInfo->HasMovieTextures()) { 
	//	TRACE("Trigger Time Sensors for t %g \n",t);
        if (sceneInfo->TriggerMovieTextures(t) !=0) {
			setRedraw();
			ret = TRUE;
		}
    }
   	if (sceneInfo->HasMedia()) { 
	//	TRACE("Trigger AudioClips for t %g \n",t);
        if (sceneInfo->TriggerMedia(t) !=0) {
			//setRedraw();
			//ret = TRUE;
		}
    }
	// events processed 
	callPendingScripts();
	
	if (boundNavigationInfoChanged) { // any changes on GvNavigationInfo?
		TRACE("NavigationInfo has been changed \n");
		
		GvNavigationInfo *b = (GvNavigationInfo *) (GvNode*) boundNavigationInfo;
		if (b) b->Apply(this);
		boundNavigationInfoChanged=0;
	}


	if (soundDevice)  // send defferend settings to 3D sound system
		soundDevice->CheckCommit(); 

	return ret;
}

void  GView::initPendingScripts()
{
	pendingScripts.flush(); 
}

void  GView::termPendingScripts()
{
	pendingScripts.flush();
}

GvBool GView::callPendingScripts()
{
	for (int i= 0; i<pendingScripts.getNum(); i++) 
	{
		GvScript *s = (GvScript *) pendingScripts.get1(i);
		if (s) s->callEventsProcessed();
	}

	pendingScripts.flush();
	return FALSE;
}



void GGeometryTraversal::SetCamera(GCamera *camera)
{
	this->camera = camera;
	//viewLocalValid = 0;

	float zfar = camera->zfar;

	viewpoint = camera->position;
	Point dir =camera->target - camera->position;
	float d= dir.Length();

	Point right = camera->NormRight();
	Point up = camera->up;


	dir *= zfar / d;

	right *= zfar*0.5*camera->widthWorld / d;
	up *= zfar*0.5*camera->heightWorld / d;

	Point p = viewpoint + dir; // point on far plane 

	// compute 4 points on far plane 
	viewVolume.SetLength(4);
	viewVolume[0]=p+right+up;
	viewVolume[1]=p+right-up;
	viewVolume[2]=p-right-up;
	viewVolume[3]=p-right+up;

	viewPlanes.SetLength(5);
	viewPlanes[0]=Plane3(viewpoint,viewVolume[1],viewVolume[0]);
	viewPlanes[1]=Plane3(viewpoint,viewVolume[2],viewVolume[1]);
	viewPlanes[2]=Plane3(viewpoint,viewVolume[3],viewVolume[2]);
	viewPlanes[3]=Plane3(viewpoint,viewVolume[0],viewVolume[3]);
	viewPlanes[4]=Plane3(viewVolume[0],viewVolume[1],viewVolume[2]);

}


//
// draw the scene 
//

void GView::DrawScene(RECT &rc)
{	int ret;
	gbool cameraChanged = FALSE;

	//setBrowser(this); // set the current global browser variable 
#ifdef _OGL
	device->MakeCurrent();
#endif 

#ifdef _D3D
    if (!device->bRenderingIsOK) return;
	
	if (device->bPaused) return; 

	// probably in nested dialog, 
	if (renderState.beginSceneCnt >0) return;

		
		/*
         * Restore any lost surfaces
         */
        if (!RestoreSurfaces()) {
            /*
             * Restoring surfaces sometimes fails because the surfaces cannot
             * yet be restored.  If this is the case, the error will show up
             * somewhere else and we should return success here to prevent
             * unnecessary error's being reported.
             */
			if (device->LastError == DDERR_WRONGMODE) {
/*
				CreateD3DDevice( HWND hWnd,	// handle of window
					HWND hWndFrame,		// handle of app window for exclusive mode 
						LPSTR lpCmdLine,		// commandline
						const char *driver,	// preferenced Driver, default "Ramp Driver"
						BOOL  bOnlySystemMemory,  // use only system memory for all surfaces
 						BOOL  bOnlyEmulation		// use no hardware 
*/
				
				// reinit driver
				TRACE("Reinit driver \n");
				CString driverName;
				
				driverName = device->Driver[device->CurrDriver].Name;
				device->Destroy();

				GDriverHints driverHints; // keep a global ???

				// initialize D3D device 
				CWinApp *a = AfxGetApp();
				HWND hWndFrame =  hWnd;
				if (a  && (a->m_pMainWnd)) hWndFrame = (a->m_pMainWnd)->m_hWnd;
				else hWndFrame = AfxGetMainWnd()->GetSafeHwnd();

				if (!CreateD3DDevice(hWnd,hWndFrame,driverName,&driverHints))
					return ;

				if (!device->bRenderingIsOK) return;


			}
            else return; // TRUE;
        }

#endif
	lastFrameStats = frameStats;

	renderState.IncrementFrame(); // for marking texture "age"

	DWORD startTime= timeGetTime(); 

	needUpdate = 0;

#ifdef _GSTATS
	GShell::ClearStats();
#endif


	if (degrade) {  // switch to degrade mode 
	   if (!degraded) {
	   		SetDegradeMode();
	   }
	}

	int savedBufferMode;
	savedBufferMode = renderState.GetBufferMode();

	abortAllLoads = 0;

	ClearCounts();

    // ********** trigger time sensors and check for other changes

   	if (!thisFrameTimeSensorsTriggered /* && time.Enabled() */ && !time.Paused()) 
	{ 
	    TriggerTimeSensors();

    } else {
		callPendingScripts();
	}


	thisFrameTimeSensorsTriggered = FALSE; // rest for next time 



	// ********** for VRML 2.0 :  check for changes in viepoint's transform hierarchy 
	if (boundViewpoint != (GvNode*) NULL) {
		Matrix m;
		if (boundViewpointPath.getMatrix(m)) { // re-evalute matrix of viewpoint
			cameraMatrix.Set(m);
		}
		else {
			if (!cameraMatrix.IsIdentity()) cameraMatrix.SetIdentity();
		}

		// recompute NavigationInfo values based on cameraMatrix ! 
		UpdateGlobalNavigationValues();

		if (boundViewpointChanged) {
			// re-evalute bound viewpoint, fields of viewpoint has been animated 
			SetCamera(boundViewpoint,NULL,0); 
			boundViewpointChanged=0;
		}

	} else 

	if (followObject)  // shift camera by movement of object 
	{	Matrix m;
		Point p=followObjectPosition;
		if (followObjectPath.getMatrix(m)) { // reevalute matrix of object 
			p*= m;
		}
		{
			Point delta = p - followObjectPositionGlobal; // the distance object has moved
			camera->position += delta;
			camera->target += delta;
			camera->OnChanged();
			followObjectPositionGlobal = p;
		}

	}

	cameraChanged = camera->Changed();

	// ******* compute global camera 
	*globalCamera = *camera;
	
	if (!cameraMatrix.IsIdentity()) {
		globalCamera->Transform(cameraMatrix.M());
		// ??? UpdateGlobalNavigationValues();
	}
	

	// compute orientation from camera 
	Vector axis;
	float angle;
	ConvertCameraToOrientation(globalCamera->position, globalCamera->target,globalCamera->up,axis, angle);

	if (!cameraChanged) {
		cameraChanged  = firstUpdate || (globalCamera->position.x != viewpointPosition.value[0]) ||
			 (globalCamera->position.y != viewpointPosition.value[1]) ||
		     (globalCamera->position.z != viewpointPosition.value[2]) ||
			 (axis.x != viewpointOrientation.value[0]) ||
			 (axis.y != viewpointOrientation.value[1]) ||
		     (axis.z != viewpointOrientation.value[2]) ||
			 (angle != viewpointOrientation.value[3]);

    }

	if (thirdPersonView) { // update the phyiscal camera following globalCamera
		UpdatePhysicalCamera(cameraChanged);
	}
	
	// adapt znear / zfar 
	UpdateDefaultZRange();

	if (cameraChanged) {
		// global browser fields
		viewpointPosition.set(globalCamera->Position());
		viewpointOrientation.set(axis.x,axis.y,axis.z,angle);

#ifdef _COM
		// check if observer wants viewpoint messages 
		if (observerFlags & GOBSERVE_VIEWPOINT) { 
			observer->OnViewpointChanged((float *) viewpointPosition.get(),(float *)viewpointOrientation.get().get());
		}
#endif

    
		// update sound listeners position
		if (soundDevice) {
			GCamera *camera =  GetPhysicalCamera();

			soundDevice->SetViewpoint(camera->Position(),
				camera->NormTargetDirection(),camera->Up(),startTime);
			soundDevice->PrioritizeSounds();

		}
	} // camera changed

	if (showAvatar) {
		
		Vector dir = globalCamera->NormTargetDirection();
		static Vector oldDir;
		static Point oldPos;

		Vector dir1 = globalCamera->position - oldPos;
		float dirLen = dir1.Length();

		gbool moved = dirLen >= EPS;
		
		dir1.Normalize();

#if 1
		Vector rotAxis = NormCrossProd(dir,oldDir);
		//Vector rotAxis = NormCrossProd(oldDir,dir1);

		Quaternation q1,q2,q3,q4;

		q1.SetAxisRotation(rotAxis,-1.0 * acos(DotProd(dir,oldDir))*10.0);
//		q1.SetAxisRotation(rotAxis,-1.0 * acos(DotProd(oldDir,dir1))*1.0);
		q2.SetAxisRotation(axis,angle);
		q4.SetAxisRotation(Vector(0,1,0),PI);
		q3=q2;
// strange 		q3 = q2 * q1;
		q3 = q3 * q4; // 180.0

		// add 180 deg rotation 
		oldDir = dir;

#else
		Quaternation q3;		
		oldDir = dir;
		dir = globalCamera->position - oldPos;
		dir.Normalize();

		if (dir.Length() <= 0.1) {
			ConvertCameraToOrientation(globalCamera->position, globalCamera->target,globalCamera->up,axis, angle);
			q3.SetAxisRotation(axis,angle);
		} else {
			Vector rotAxis = NormCrossProd(Point(0,0,-1),dir);
			q3.SetAxisRotation(rotAxis,DotProd(Point(0,0,-1),dir));
		}
#endif
		oldPos = globalCamera->position;


		

		Rotation rot(axis.x,axis.y,axis.z,angle); 

		Point pos;
		if (thirdPersonView) 
		   pos = globalCamera->position;
		else pos = globalCamera->position + dir * fabs(followDolly.z());

		{
			GvSFRotation r;
			GvSFVec3f p;
			// r.set(rot);
			r.set(q3);
			p.set(pos);
			if (avatarInfo.avatarSetRotation >= 0) GvNode::sendEventIn(&r,avatarInfo.nodeTransform,avatarInfo.avatarSetRotation);
			if (avatarInfo.avatarSetTranslation>=0) GvNode::sendEventIn(&p,avatarInfo.nodeTransform,avatarInfo.avatarSetTranslation);
		}
	}

#ifdef _DEBUG
	GvBspTree::ClearBspStats();
#endif

	// clear the screen 
    renderState.Clear();



#if 0
    if (renderMode == RENDER_SOLID) {
			 if (renderState.sbuffer) {
				 glClear( GL_STENCIL_BUFFER_BIT);
				 glEnable(GL_STENCIL_TEST);
				 glStencilFunc(GL_ALWAYS,0,1);
			     glStencilOp(GL_INVERT,GL_INVERT,GL_INVERT);
			 }
	}
#endif

    // ********** set camera
	renderState.SetCamera(GetPhysicalCamera());
    globalCamera->changed = camera->changed = 0; // unset changed flag 

	// unset counter of triggered ROUTES 
	//GvConnection::clearTriggeredConnectionsCnt();
	/*xx GvNode::*/ clearRedraw();


	if (boundFogChanged) { // any changes on Fog?
		TRACE("Fog has been changed \n");
		
		GvFog *b = (GvFog *) (GvNode*) boundFog;
		if (b) b->Apply(this);
		boundFogChanged=0;
	}


#ifdef _D3D
    HRESULT ddrval;

//    renderState.SetMaterial(renderState.defaultMaterial,renderState.defaultMaterialValue);

    renderState.SetWorldMatrixIdentity();

#if 1
	if (headLightOn) {

    /*
     * Update head light 
     */
	GCamera *camera =  GetPhysicalCamera();

    Point ldir =camera->position - camera->target;
  
	ldir.Normalize();
    ldir *= -1;

	// get light data structure 
	G3LightHandle &light = *renderState.GetLight(headLight); 
	
    //memset(&light, 0, sizeof(D3DLIGHT));
    //light.dwSize = sizeof(D3DLIGHT);
    
	light.dltType = D3DLIGHT_DIRECTIONAL;

    light.dcvColor.r = 1.0f; 
    light.dcvColor.g = 1.0f;
    light.dcvColor.b = 1.0f;
   
	light.dcvColor.a = 1.0f;  // important for ramp mode ??? 

    
    light.dvDirection.x = ldir.x;
    light.dvDirection.y = ldir.y;
    light.dvDirection.z = ldir.z;
    light.dvAttenuation0 = 1.0f;
    light.dvAttenuation1 = 0.0f;
    light.dvAttenuation2 = 0.0f;
    light.dvRange = 0.0f;
	
	light.ambientIntensity=0;

    light.ambientColor.r = 0.0f;
    light.ambientColor.g = 0.0f;
    light.ambientColor.b = 0.0f;
    light.ambientColor.a = 0.0f;

	renderState.SetLight(headLight);

	} // light 
	else {
		renderState.DeleteLight(headLight);
	}

#endif


     
#endif

 GCamera *cam=GetPhysicalCamera();

// save current view matrix 		
#ifdef _D3D
	viewInfo.SetView(cam->World2p());
	viewInfo.SetCam(cam->viewMatrix);
#endif

#ifdef _OGL    
    viewInfo.glGet();
#endif

	// Light source update 

	int firstLightIndex=0;

	if (useSceneLights && !headLightOn) {
#ifdef _OGL
	  for (int i=0; i<renderState.maxLights;i++) {
	  	 glDisable((GLenum)(GL_LIGHT0+i));
	  }	
      glCheckError();
#endif
#ifdef _D3D
#endif

	} else {
	    // init default lights 
		if (!lightInit) {
			DefineLights(cam->NormDirection(),headLightOn,firstLightIndex);
			lightInit = 0;
			// new enabled lightInit = 1; // ? HG
		}
	}

	if (useSceneLights && sceneInfo) {
		// GvSceneInfo GvLight, where to switch of Deleted / Unused Lights ??
		for (int i=0; i< sceneInfo->lightPath.Length(); i++) {
			 GvLight *l = (GvLight *) (GvNode*) sceneInfo->lights[i];
			 
			 if (!l->on) {
				 l->glSetOff(&renderState);
				 continue; // switched off
			 }
			 if (RTISA(l,GvDirectionalLight) && !l->global) 
			 	continue; // these are local in VRML 2.0

			 GvPath &p=sceneInfo->lightPath[i];

			 Matrix m;
			 if (p.getMatrix(m)) { // reevalute matrix of light

			 }

#ifdef _OGL
  		     if (firstLightIndex>=renderState.maxLights) break; // too many lights 

			 glPushMatrix(); // we need the matrix to transfom 
			 renderState.modelMatrix.glSet();  
			 glMultMatrixf(&m.x[0][0]);
			 //glLoadMatrixf(&m.x[0][0]);

			 if (l->glSet(&renderState,firstLightIndex,globalCamera,m))
				firstLightIndex++;
			 glPopMatrix();
#else
			 // Direct 3D
			 if (l->glSet(&renderState,firstLightIndex,globalCamera,m))
				firstLightIndex++;
#endif

		}
	}
	
	//
	renderState.currentMaxLight = firstLightIndex-1;

	// OpenGL caching of whole model possible ?
    BOOL canUseGlCache = useGlCache & !time.enabled;

#ifdef _D3D
    /*
     * Execute all geometry
     */
    ddrval = renderState.BeginScene();

    renderState.FlushState();
#endif


    // render 
	if (theWorld || GetScene()) {

		renderState.FlushCurrent();

		if (firstUpdate) { 
			maxFrameInlineLoadCnt = 30;
			maxFrameTextureLoadCnt = 100;
		}
		else {
			if (Moving()) {
				maxFrameInlineLoadCnt = 5;
				maxFrameTextureLoadCnt = 5;
			}
			else {
				maxFrameInlineLoadCnt = 10;
				maxFrameTextureLoadCnt = 10;
			}
		}


		if (!TransparencyMode) {

#ifdef _GV
		GGvglRenderTraversal traversal(&renderState);
#else 
		GglRenderTraversal traversal(&renderState);
#endif
		traversal.view = this;
		traversal.useTextures = TextureMode && useTextures;
		traversal.traversalOrder = traversalOrder;
		
		traversal.doViewCulling = viewCulling;
		traversal.lodScale = lodScale;
		traversal.doLodScale = lodScale!=1.0f;

		if (firstUpdate) {
			// try to load as much inlines & textures 
			traversal.doViewCulling = gfalse;// first update load everything
			traversal.loadInThread = gfalse;
			// to think: browser.setPreload(TRUE)
		}


		traversal.firstLightIndex = firstLightIndex;
		traversal.globalPointLights = IsVrml2();
		traversal.defaultMaterial10 = (GvMaterial *) defaultMaterial10.get();
		traversal.defaultAppearance = (GvAppearance *) defaultAppearance.get();
		traversal.defaultMaterial = (Gv2Material *) defaultMaterial.get();
		traversal.defaultTextureTransform = (Gv2TextureTransform *)defaultTextureTransform.get();

		traversal.doProximitySensors = 1; // tested for BMW project !AnimatingCamera(); // don't trigger proxie during animated move
		// if devCanDo
		traversal.transparencyMode = GTRANSPARENCY_DELAYED;

		
		// Don´t load files during mouse move, 
		if (0 && Moving()) {
			traversal.loadInlines = GTraversal::DONT_LOAD;
			traversal.loadTextures = GTraversal::DONT_LOAD;
			traversal.loadMovies = GTraversal::DONT_LOAD;
			traversal.loadSounds = GTraversal::DONT_LOAD;
		} else {
		    traversal.loadInlines = (GTraversal::LoadMode) inlineLoadMode;
		    traversal.loadTextures = (GTraversal::LoadMode) textureLoadMode;
		    traversal.loadMovies = (GTraversal::LoadMode) movieLoadMode;
			traversal.loadProtos =  (GTraversal::LoadMode) inlineLoadMode;
		    
			if (useSound) traversal.loadSounds = (GTraversal::LoadMode) soundLoadMode;
			else traversal.loadSounds = GTraversal::DONT_LOAD;

/* hg disabled for IBM canada project 
			if (firstUpdate) { 
				traversal.loadTextures = GTraversal::DONT_LOAD;
				traversal.loadMovies = GTraversal::DONT_LOAD;
				traversal.loadSounds = GTraversal::DONT_LOAD;
			}
*/
		}
		traversal.maxInlineLevel = maxInlineDepth;
		traversal.inlineWwwFlags = inlineWwwFlags;

		if (file->Flag(WWW_RELOAD)) // propagate RELOAD to inlines, textures ...
			traversal.inlineWwwFlags |= WWW_RELOAD;

		traversal.InitializeAttributes();

		traversal.SetCamera(globalCamera);
		traversal.SetPhysicalCamera(GetPhysicalCamera());

        traversal.viewInfo = &viewInfo;

		traversal.lastFrameStats = lastFrameStats;
		traversal.nurbsTessellationScale = nurbsTessellationScale;
		traversal.nurbsTessellationMode=nurbsTessellationMode;



		GTimeStack *ts= GTimeStack::NewElement();
		ts->time = time; 
	    traversal.Set(ts); // set current time


        // ********** render background Layer 
        if (useBackground /*&& doBackground*/ )	{

			if (boundBackgroundChanged) { // any changes on Background	?
				GvBackground *b = (GvBackground *) (GvNode*) boundBackground;
				if (b) b->Apply(this);
				boundBackgroundChanged=0;
			}

			if (background != NULL) {
				int fogWasEnabled = renderState.fog.enable;
				if (fogWasEnabled) {
					renderState.fog.enable=0;
					renderState.SetFog();
				}

				UpdateBackgroundCamera(axis,angle);
				GTransparencyMode _transparencyMode = traversal.transparencyMode;				

				traversal.transparencyMode = GTRANSPARENCY_NONE; // don't sort into list 

				ret= background->Traverse(traversal);

				traversal.transparencyMode = _transparencyMode;				


				if (fogWasEnabled) 
					renderState.SetFogEnable(TRUE); // settings already there 

#ifdef _D3D
				renderState.SetTextureOff(); // set NULL texture handle 
#endif
			}
        } 

		// ********** set default texture
#ifdef _OGL
		if (TextureMode && renderState.textureCache.IsValid()) {
			renderState.textureCache.Call();
			renderState.SetFaceTexturing(1);
			// white & trans % etc 
		} else {
			renderState.SetTexturing(0);
			renderState.SetAnyTexturing(0);
		}
#endif
#ifdef _D3D
//    renderState.SetTextureBlend(D3DTBLEND_MODULATE);        
		if (TextureMode) {
			if (!renderState.SetTexture(renderState.defaultTexture)) {
				if (!renderState.ReloadTexture(renderState.defaultTexture))
					renderState.SetTexturing(0); // no texture

			}
		}
		else 
			renderState.SetTexturing(0); // set texture
#endif
		// ********** set default material

		renderState.SetMaterial(); // set material
		renderState.SetBlendFromMaterial(FALSE); // disable any enabled blending
		renderState.SetAlphaTestFromTexture(FALSE); // disable any enabled alpha test from texture
	    renderState.SetLitMaterial(TRUE);



#ifdef _OGL
		if (canUseGlCache) {		// use OpenGL call lists !!
			if (modelCache.IsValid())		// call list ok
			    modelCache.Call();
			else
			   {
				modelCache.Begin(GL_COMPILE_AND_EXECUTE);
					ret = Traverse(traversal);
			     modelCache.End();
			   }
		 } else
#endif

		 ret = Traverse(traversal); // traverse the scene 

		 traversal.RenderTransparentElements(); // draw transparent elements

		 if (foreground.Length()>0) {
			ret= foreground.Traverse(traversal);
			traversal.RenderTransparentElements();
		 }


	    frameStats = traversal.frameStats;
	  
	  } else { //transparency
		
		GglTransparencyRenderTraversal traversal(&renderState);

		gbool zbuffer=renderState.zbuffer;

		traversal.view = this;
		traversal.traversalOrder = traversalOrder;

		traversal.doViewCulling = viewCulling;

		traversal.lodScale = lodScale;
		traversal.doLodScale = lodScale!=1.0f;


		if (firstUpdate) {
			// try to laod as much inlines & textures 
			traversal.doViewCulling = gfalse;
			traversal.loadInThread = gfalse;
		}

		traversal.useTextures = TextureMode && useTextures;
		traversal.firstLightIndex = firstLightIndex;

		traversal.loadInlines = (GTraversal::LoadMode) inlineLoadMode;
		traversal.loadProtos = (GTraversal::LoadMode) inlineLoadMode;
	    traversal.loadTextures = (GTraversal::LoadMode) textureLoadMode;
	    traversal.loadMovies = (GTraversal::LoadMode) movieLoadMode;
		if (useSound) traversal.loadSounds = (GTraversal::LoadMode) soundLoadMode;
		else traversal.loadSounds = GTraversal::DONT_LOAD;
/* hg disabled for IBM-Canada 
		if (firstUpdate) { 
				traversal.loadTextures = GTraversal::DONT_LOAD;
				traversal.loadMovies = GTraversal::DONT_LOAD;
				traversal.loadSounds = GTraversal::DONT_LOAD;
		}
*/


		traversal.maxInlineLevel = maxInlineDepth;
		traversal.inlineWwwFlags = inlineWwwFlags;

		
		traversal.InitializeAttributes();
		traversal.SetCamera(globalCamera);
        traversal.SetPhysicalCamera(GetPhysicalCamera());
		traversal.viewInfo = &viewInfo;

		traversal.defaultMaterial10 = (GvMaterial *) defaultMaterial10.get();
		traversal.defaultAppearance = (GvAppearance *) defaultAppearance.get();
		traversal.defaultMaterial = (Gv2Material *) defaultMaterial.get();
		traversal.defaultTextureTransform = (Gv2TextureTransform *)defaultTextureTransform.get();

		traversal.doProximitySensors = 1; // tested for BM proj !AnimatingCamera(); // don't trigger proxie during animated move

		// set current time
		GTimeStack *ts= GTimeStack::NewElement();
		ts->time = time; 
	    traversal.Set(ts); 


        // ********** render background Layer 
        if (/* doBackground && */ useBackground && background != NULL) {
			if (boundBackgroundChanged) { // any changes on Background	?
				GvBackground *b = (GvBackground *) (GvNode*) boundBackground;
				if (b) b->Apply(this);
				boundBackgroundChanged=0;
			}
            ret= background->Traverse(traversal);
        } 

		// ********** set default texture
#ifdef _OGL
		if (TextureMode && renderState.textureCache.IsValid()) {
			renderState.textureCache.Call();
			renderState.SetFaceTexturing(1);
			// white & trans % etc 
		} else {
			renderState.SetTexturing(0);
			renderState.SetAnyTexturing(0);
		}
#endif
#ifdef _D3D
//    renderState.SetTextureBlend(D3DTBLEND_MODULATE);        
		if (TextureMode) {
			if (!renderState.SetTexture(renderState.defaultTexture)) {
				if (!renderState.ReloadTexture(renderState.defaultTexture))
					renderState.SetTexturing(0); // no texture
			}
		}
		else 
			renderState.SetTexturing(0); // set texture
#endif
		// ********** set default material

		renderState.SetMaterial(); // set material
		renderState.SetBlendFromMaterial(FALSE); // disable any enabled blending
		renderState.SetAlphaTestFromTexture(FALSE); // disable any enabled alpha test from texture
	    renderState.SetLitMaterial(TRUE);

		
        
		// ********** transparency first path
		traversal.action = GglTransparencyRenderTraversal::RENDER_OPAQUE_OBJECTS; 
		ret = Traverse(traversal);


		// second path

		renderState.SetBlend(TRUE);
		renderState.SetBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
		renderState.SetZWrite(FALSE);

		traversal.traversalOrder = TRAVERSE_BACK_TO_FRONT; 

		
		//renderState.SetZBuffer(0);
		
		traversal.action = GglTransparencyRenderTraversal::RENDER_TRANSPARENT_OBJECTS; 

		ret = Traverse(traversal);

		renderState.SetBlend(FALSE);
		//renderState.SetZBuffer(zbuffer);
		renderState.SetZWrite(TRUE);

		if (foreground.Length()>0) {
			ret= foreground.Traverse(traversal);
		}

	    frameStats = traversal.frameStats;

	  }


#if 1      
	  // ******** after frame actions 
	  // some inlines loaded ?
      if (lastFrameInlineLoadCnt >0  /* || lastFrameProtoInstanceCnt>0 */ ) { 

         // update scene info ?
         UpdateSceneInfo(); // ?? do we need this, or are all registered 

         // update depth range or default camera
         // optimize if !oldBbox.IsIn(newBbox)
         

         if (camera->IsDefault() || camera->zrangeIsDefault) {

			 BBox oldBbox(bbox);
             UpdateBoundingBox();

             if (camera->zrangeIsDefault && !bbox.IsEmpty()) {
                 if (oldBbox.IsEmpty() || !bbox.Inside(oldBbox)) { // new box is larger
                    TRACE("Updating Z-Range ...");

                    //camera->SetZRange(bbox);
                    globalCamera->SetZRange(bbox); // to do: avatarDistance 

					camera->zfar = globalCamera->zfar;
					camera->znear = globalCamera->znear;
					camera->OnChanged();

                    TRACE("Updating Z-Range ...%f %f \n",camera->znear,camera->zfar);
                    if (camera->Changed()) {
                        needUpdate++; 
                    }
                 }
             }   
         } else {
             UpdateBoundingBox();
         }


	  } // last Frame Inline load Cnt 
#endif

      if (lastFrameInlineLoadCnt >= maxFrameInlineLoadCnt || lastFrameTextureLoadCnt >= maxFrameTextureLoadCnt  ) { 
		setRedraw(); // bunch of stuff loaded, schedule redraw 	
	  }	


	  // deactive vis-sensors
	  if (activeVisibilitySensors.getNum()>0) 
		 InActivateVisibilitySensors();

	  // deactive zones 
	  if (activeZones.isModified()) 
		 InActivateZones();

	} // theWorld
	else 
    if (theShell) {

		 PointArray *savedPoints=NULL;

        // Background
        if ( /*doBackground %% */ useBackground && background != NULL) {
            #ifdef _GV
		    GGvglRenderTraversal traversal(&renderState);
            #else 
		    GglRenderTraversal traversal(&renderState);
            #endif
		    traversal.view = this;
			traversal.defaultMaterial10 = (GvMaterial *) defaultMaterial10.get();
			traversal.defaultAppearance = (GvAppearance *) defaultAppearance.get();
			traversal.defaultMaterial = (Gv2Material *) defaultMaterial.get();
			traversal.defaultTextureTransform = (Gv2TextureTransform *)defaultTextureTransform.get();

		    // Don´t load files during mouse move, 
		    if (Moving()) {
			    traversal.loadInlines = GTraversal::DONT_LOAD;
			    traversal.loadTextures = GTraversal::DONT_LOAD;
			    traversal.loadMovies = GTraversal::DONT_LOAD;
		    } else {
		        traversal.loadInlines = (GTraversal::LoadMode) inlineLoadMode;
		    }
		    traversal.maxInlineLevel = maxInlineDepth;
			traversal.inlineWwwFlags = inlineWwwFlags;

		    traversal.InitializeAttributes();
		    traversal.SetCamera(globalCamera);
			traversal.SetPhysicalCamera(GetPhysicalCamera());

			// set current time
			GTimeStack *ts= GTimeStack::NewElement();
			ts->time = time; 
			traversal.Set(ts); 

            background->Traverse(traversal);
        } 



		 if (theShell->edit_flags !=0 ) InvalidateModelCache();

		 // update internal state of shell 
		 theShell->Update();


		// ********** set default texture
#ifdef _OGL
		if (TextureMode && renderState.textureCache.IsValid()) {
			renderState.textureCache.Call();
			renderState.SetFaceTexturing(1);
			// white & trans % etc 
		} else {
			renderState.SetTexturing(0);
			renderState.SetAnyTexturing(0);
		}
#endif

#ifdef _D3D
//    renderState.SetTextureBlend(D3DTBLEND_MODULATE);        
		if (TextureMode) {
			if (!renderState.SetTexture(renderState.defaultTexture)) {
				if (!renderState.ReloadTexture(renderState.defaultTexture))
					renderState.SetTexturing(0); // no texture
		}
		}
		else 
			renderState.SetTexturing(0); // no texture
#endif
		// ********** set default material

		 renderState.SetMaterial(); // set material
	     renderState.SetLitMaterial(TRUE);

		 //renderState.material.diffuse.glSet(); // non-lighted color
		 renderState.SetTexturingDiffuseColor(); // if textured optionally set diffuse to white

#ifndef  _GV_MINIMAL
		 if (deformer) {
		   savedPoints = new PointArray(theShell->v);

		   deformer->Eval(theShell->v.Length(),savedPoints->Data(),theShell->v);
		   if (deformer->id == GIdSphereDeform) {
				GSphereDeform * d = (GSphereDeform *) deformer;
				
				if (d->display) renderState.RenderSphere(d->center,d->r); 
		   }

		   theShell->SetEditFlag(SHELL_VERTICES_CHANGED);

		   theShell->Update();
		   canUseGlCache  = 0;
		   InvalidateModelCache();
		 }

#if 1
		 if (parameter && parameter->enabled && theShell) {
			if (parameter->changed || parameter->NeedsViewpoint() )
			{
			  GShell *apply;

			  if (backupShell) apply = backupShell;
			  else apply = theShell;

			  parameter->viewpoint= camera->position;

		      parameter->ApplyShell(*apply);

			  parameter->changed = 0; // Notify is applied
			  if (parameter->showSurface) {}
			  if (parameter->showMappedObject && (apply->vp.Length())) {
				  if (!backupShell) {
				  GShell *g;
				  g=theShell->CopyGeometry();
				  g->SetV(apply->v.Length(),apply->vp);
				  g->SetVN(apply->v.Length(),apply->vn);
				  g->SetVP(apply->v.Length(),apply->vp);
//				  g->editFlags = 0;
				  g->UnSetEditFlag(-1);

				  g->Update();
				  backupShell = theShell;
				  theShell = NULL;
				  SetShell(g);
				  } else {
				    theShell->SetV(apply->v.Length(),apply->vp);
				    theShell->SetVN(apply->v.Length(),apply->vn);
				    theShell->SetVP(apply->v.Length(),apply->vp);
				    theShell->UnSetEditFlag(-1); // use my normals
				  }
			  } else {

				if (backupShell) {		// Restore to previous version
				   if (theShell) delete theShell;
				   theShell = 0;
				   SetShell(backupShell);
				   backupShell = NULL;
				}
			  }
			  canUseGlCache  = 0;
		      InvalidateModelCache();

			}
		 }
#endif
#endif

#ifdef _OGL
         if (canUseGlCache) {		// use call lists !!!!!!!!!!!!!!!!!
			if (modelCache.IsValid())		// call list ok
			    modelCache.Call();
			else
			   {
				modelCache.Begin(GL_COMPILE_AND_EXECUTE);
					theShell->glRender(renderState);
			     modelCache.End();
			   }
		 } else
 		   theShell->glRender(renderState);
#else
#ifdef _D3D
		{
		renderState.SetMaterial(); // ??
		GglRenderTraversal traversal(&renderState);
		traversal.view = this;
		traversal.useTextures = TextureMode && useTextures;
		traversal.traversalOrder = traversalOrder;
		
		traversal.doViewCulling = viewCulling;
		traversal.InitializeAttributes();

		traversal.SetCamera(globalCamera);
		traversal.SetPhysicalCamera(GetPhysicalCamera());

        traversal.viewInfo = &viewInfo;



	    //theShell->D3dRender(renderState);
	    theShell->Do(traversal);

		}
#endif
#endif

		 if (savedPoints) {
		   theShell->SetV(*savedPoints);
		   delete savedPoints; savedPoints = NULL;
		 }

		if (foreground != NULL) {
            #ifdef _GV
		    GGvglRenderTraversal traversal(&renderState);
            #else 
		    GglRenderTraversal traversal(&renderState);
            #endif
		    traversal.view = this;
			traversal.defaultMaterial10 = (GvMaterial *) defaultMaterial10.get();
			traversal.defaultAppearance = (GvAppearance *) defaultAppearance.get();
			traversal.defaultMaterial = (Gv2Material *) defaultMaterial.get();
			traversal.defaultTextureTransform = (Gv2TextureTransform *)defaultTextureTransform.get();

			if (foreground.Length()>0) {
				ret= foreground.Traverse(traversal);
			}

		}


	} else {
        // nothing to trender 
//		drawCone();
//		drawPyramid();
	}
#ifdef _D3D
    //ddrval = 
	renderState.EndScene();
#endif

    
    renderState.Finish();

#ifdef _DEBUG
//	if (GetReporter()->verbose) GvBspTree::TraceBspStats();
#endif


#ifdef _D3D
	
		if (panel && panel->Ok())
			panel->Blt(device->lpBackBuffer);

		/*
         * Give D3DApp the extents so it can keep track of dirty sections of
         * the back and front buffers
         */
        //if (!device->RenderExtents(count, extents, myglobs.bResized ?  D3DAPP_CLEARALL : NULL)) 

        if (!device->RenderExtents(0, NULL,D3DAPP_CLEARALL)) 
		{
            ReportD3DError();
            return; // FALSE;
        }
        /*
         * Blt or flip the back buffer to the front buffer.  Don't report an
         * error if this fails.
         */
//        device->ShowBackBuffer(myglobs.bResized ? D3DAPP_SHOWALL : NULL);
         device->ShowBackBuffer(D3DAPP_SHOWALL);

        /*
         * Reset the resize flag
         */
        bResized = FALSE;
#endif

#ifdef _OGL
#ifdef G_PANEL
		if (panel && panel->Ok())
			panel->Blt();
#endif
#endif



	if (renderState.SwapBuffers()) {
	}

	if (renderState.GetBufferMode() != savedBufferMode) {
	   renderState.SetBufferMode(savedBufferMode);
	}


	DWORD endTime= timeGetTime(); 

	if (endTime>startTime) {
	  lastFrameTime = endTime-startTime; // DELTA in milliseconds, could wrap every 49 Days

	  lastFrameTimesSum += lastFrameTime;
	  lastFrameTimesSum -= lastFrameTimes[lastFrameTimesI];

	  lastFrameTimes[lastFrameTimesI]= lastFrameTime;
	  lastFrameTris[lastFrameTimesI] = renderState.trisDrawn;
	  lastFramePrims[lastFrameTimesI] = renderState.primsDrawn;

	  lastFrameTimesI = (lastFrameTimesI+1) % NUM_LAST_FRAME_TIMES;
	}

	frameStats.frameTime = (float) lastFrameTime / 1000.0;
	frameStats.frame = renderState.GetCurrentFrame();

	if (frameStats.prims>0) { // we have nurbs 
		TRACE("Frame Stats : Prims %d  Tris %d  time %g \n",
			frameStats.prims,frameStats.tris,frameStats.frameTime);

		if ((nurbsTessellationMode & GNURBS_FRAMERATE_TESSELATION) && targetFrameRate>=1.0f) {
			float f= 1.0f/frameStats.frameTime;
			float target = f / targetFrameRate;
			TRACE("C FPS %g  target factor %g  tess factor %g \n",f,target,nurbsTessellationScale);
			
			//float feedBackFactor = 0.25;
			//if (target <0.2) feedBackFactor = 1.0f; // if very bad frame rate, high feedback
			
			// feedback 
			nurbsTessellationScale += .2f*FuzzyLogic.applyLogic(f);
			//nurbsTessellationScale = (nurbsTessellationScale*(1.0f - feedBackFactor) + (feedBackFactor)*nurbsTessellationScale*sqrt(target));

/*			if ( target > 1.5) {
				nurbsTessellationScale += 0.2f;
			}
			else if ( target < 0.5) {
				nurbsTessellationScale -= 0.2f;
			}
*/	
			//keine ahnung warum bei .1 immer noch haengt
			if (nurbsTessellationScale < 0.01f) {
				nurbsTessellationScale = 0.01f;
#ifdef _DEBUG
				//MessageBeep(MB_OK);
#endif
			}
			//katmai langweilt sich bei zu kleinen Werten
			else if (nurbsTessellationScale > 50.f) {
				nurbsTessellationScale = 50.f;
#ifdef _DEBUG
				//MessageBeep(MB_OK);
#endif
#ifdef _FC_LOG
			frameRate << timeGetTime()/1000 << f << endl;
			budget << timeGetTime()/1000 << nurbsTessellationScale << endl;
#endif
			}
		}
	}


	frameCnt++;


#ifdef _D3D
	if ((frameCnt % 1024) == 0) {
		if ((frameCnt % 4096) == 0) renderState.DeleteUnusedExBufs();
		else  {
			renderState.DeleteUnusedMaterials();
			renderState.DeleteUnusedTextures();
			renderState.DeleteUnusedLights();

			if (!renderState.allTexturesInVideo) {
				renderState.ReleaseUnrenderedTextures();
				// renderState.allTexturesInVideo=TRUE; // assume for now
			}
		}
	}
#endif

	if (!moving && triggerUrlLoading) {
		TRACE("Triggering an URL loading \n");
		triggerUrlLoading = FALSE;
		LoadAllInlines(1,1,1);
	}
	if (doUpdateSceneInfo >1) {
		UpdateSceneInfo();
	}
}


// update front buffer and switch to front buffer rendering
//
int BeginIncrementalUpdates(GView *view)
{
	if (view->renderState.GetBufferMode() != 1) {
    	// update front buffer
    	view->renderState.Finish();
		if (view->renderState.SwapBuffers()) {
		}
		// switch to foreground rendering 
		view->renderState.SetBufferMode(1);
		return(1);
	 }
	 return(0);
}

int GView::BeginIncrementalUpdates()
{
   return(::BeginIncrementalUpdates(this));
}

// Transform a point by a matrix.
Point
Mult3 (const Matrix& M, const Point& v)
{
    Point ret;
    // float denom;
	float w = 0.0;

    ret.x = v.x * M.x[0][0] + v.y * M.x[1][0] + v.z * M.x[2][0] + w*M.x[3][0];
    ret.y = v.x * M.x[0][1] + v.y * M.x[1][1] + v.z * M.x[2][1] + w*M.x[3][1];
    ret.z = v.x * M.x[0][2] + v.y * M.x[1][2] + v.z * M.x[2][2] + w*M.x[3][2];
 // denom = v.x * M.x[0][3] + v.y * M.x[1][3] + v.z *M.x[2][3] + w *M.x[3][3];
//if (denom != 1.0)
//		ret /= denom;
    return ret;
}

/*
  Process a single HIT element,
  switch depending on action and type of hit node 

*/


int GView::DoHitElement(SelectionAction action, int x,int y,int flags,  GHitInfo &hit,GvNode *hitNode,CString &msg,GMultitSensorHandler* & sensors)
{
	int ret=1;

#ifdef _G_VRML1
	if (RTISA(hitNode,GvWWWAnchor)) {
	  GvWWWAnchor * anchor = (GvWWWAnchor *) hitNode;
	  if (!anchor->description.isDefault()) {
			// msg+="Anchor: ";
	  		msg += anchor->description;
			msg += "  ";
	  }
	  //if (!anchor->name.isDefault()) 
	  {
	  		msg += anchor->name;
			msg += " ";
			if (action == JUMP_WWWAnchor || action == DRAG_SENSORS) { 
				// could also check myScene.url#cameraname
			 	if (anchor->name[0]=='#') {	 // execute jump to camera
					if (SetCamera(&anchor->name[1]))
					  needUpdate ++;
				}
			    else executeNode = anchor;
		    }

	  }
	  ret |= SELECT_ANCHOR_BIT;
	}
	else 
#endif

	if (RTISA(hitNode,Gv2Anchor)) {
	  Gv2Anchor * anchor = (Gv2Anchor *) hitNode;
	  if (!anchor->description.isDefault()) {
	  		msg += CString(anchor->description);
			msg += "  ";
	  }
	  if (anchor->url.Length()>0) 
	  {
	  		msg += CString(anchor->url[0]);
			msg += " ";

			if (action == JUMP_WWWAnchor  || action == DRAG_SENSORS) {
			 	if (anchor->url[0][0]=='#') { // execute jump to camera
					if (SetCamera(&anchor->url[0][1]))
					  needUpdate ++;
				}
			    else executeNode = anchor;
		    }
	  }
	  ret |= SELECT_ANCHOR_BIT;
	}
	else if (action == DRAG_SENSORS && RTISA(hitNode,GvSensor)) 
	{	// Mouse down, ativate sensor processing 
/*
		if (RTISA(hitNode,GvTouchSensor)) {
			GvTouchSensor *s = (GvTouchSensor*) hitNode;
			//s->touchTime.set(Now()); at button up
			if (!s->isOver) {
				s->isOver.set(TRUE); 
				overTouchSensors.append(s);
			}
			if (!s->isActive) {
				s->isActive.set(TRUE); 
				activeTouchSensors.append(s);
			}
			if (s->NeedMouseDrag()) 
				ret |= SELECT_DRAG_SENSOR_BIT;

		} else 
*/		
		{


		CPoint ppix(x,y);
		Point ps;
		Pixel2Screen(ppix,ps);
/*
xxxx
		if (msg != "Drag ") msg += "Drag ";
*/

		GSensorHandler *s = new GSensorHandler(this);
		s->lastHit = hit;
		s->SetSensor(hitNode);
		s->InitPath();
		if (RTISA(hitNode,GvTouchSensor)) {
			GvTouchSensor *s = (GvTouchSensor*) hitNode;
			//s->touchTime.set(Now()); done at button up
			if (!s->isOver) {
				s->isOver.set(TRUE); 
				overTouchSensors.append(s);

				if (RTISA(hitNode,GvMouseSensor)) {
					 GvMouseSensor *s = (GvMouseSensor*) hitNode;
					 GvEvent* e=getEvent();

					 if (e) {
						 e->type.set_("mouseover");
						 GvNodeHandle n;
						 MatrixAndInverse m;
						 hit.GetTargetNode(s,m,n);
						 e->target.set(n);
						 s->event.set(e);
					 }

				}
			}
		}
		// we need to combine several sensors in one 
		if (sensors == NULL )
			sensors = new GMultitSensorHandler(this);

		// do the intial Lbuton Down action on sensor
		s->OnLButtonDown(flags,ps);
		sensors->add(s);

	    ret |= SELECT_SENSOR_BIT;

		// check if we really need a drag 
		if (RTISA(hitNode,GvGeometrySensor)) {
			GvGeometrySensor *s = (GvGeometrySensor*) hitNode;
			if (s->NeedMouseDrag()) 
				ret |= SELECT_DRAG_SENSOR_BIT;
		}
		}

	} else 
	if (RTISA(hitNode,GvSensor)) {
		
		if (RTISA(hitNode,GvTouchSensor)) {
			GvTouchSensor *s = (GvTouchSensor*) hitNode;
			/* if (!s->isOver)  ??? */ 
			if (msg != "Touch ") msg += "Touch ";
			// Event outs
			// only if move && status changed
			// if (old && old != s) old->isOver.set(FALSE);
			if (!s->isOver) {
				s->isOver.set(TRUE); 
				overTouchSensors.append(s);
			}

			if (action == JUMP_WWWAnchor) { // double click 
				activeTouchSensors.append(s);
				s->isActive.set(TRUE); // GRAB until mouse up 
			}

			// for each movement 
			if (true /* s->hitPoint.hasConnections() || s->hitNormal.hasConnections() */) {
				MatrixAndInverse m;

				hit.GetSensorMatrix(s,m);
				if (s->hitNormal.hasConnections() &&  hit.vn.Length()>0) { 
					Point v=RotateOnly(hit.m,hit.vn); // transform global 
					v=RotateOnly(m.Inverse(),v); // transform to sensor matrix 
					v.Normalize();
					s->hitNormal.set(v);
				}

				s->hitPoint.set(m.Inverse()*hit.pw);	// to do : in what coordinate sys

			}
			s->hitTexCoord.set(hit.vp);

			if (action == JUMP_WWWAnchor || action == RELEASE) {
				s->isActive.set(FALSE); //  simulate release 
				s->touchTime.set(Now());
				activeTouchSensors.remove(s);
			}

	}
	else { 
		msg += hitNode->GetType();
		msg += " ";
	}
    ret |= SELECT_SENSOR_BIT;

	} // GvSensor
	else {
		ret=0;
	}
	return(ret);
}

// flush the current event handler of the view
void GView::FlushEventHandlers()
{
	if (eventHandler) {
		eventHandler->unref();
		eventHandler = NULL;
	}
}

// set new top event handler of the view
void GView::SetEventHandler(GEventHandler *newEventHandler)
{
	if (eventHandler) {
		eventHandler->unref();
		eventHandler = NULL;
	}
	eventHandler = newEventHandler;
	if (eventHandler)
		eventHandler->ref();
}

// push a new event handler to the view
void GView::PushEventHandler(GEventHandler *newEventHandler)
{
	SetEventHandler(newEventHandler);

}

// pop event handler from the view
GEventHandler *GView::PopEventHandler()
{
	GEventHandler * ret = GetEventHandler();
	eventHandler = NULL;
	return(ret);
}

// inactive visibility sensors, not visible any more 
void GView::InActivateVisibilitySensors()
{
	for (int ti=0; ti<activeVisibilitySensors.getNum(); ti++) {
		GvVisibilitySensor *t = (GvVisibilitySensor*) 	activeVisibilitySensors.get1(ti);
		BOOL purge= FALSE;

		if (t->refCnt <=1) {
			purge = TRUE;
		} else if (!t->isActive) {
				purge = TRUE;		
		} else if (!t->wasRendered && t->isActive) {
			t->isActive.set(FALSE);			  // keep it in list,
			t->exitTime.set(theTime);
			purge = TRUE;
		}
		t->wasRendered = FALSE; // clear flag for next cylce 
		
		if (purge) { // from mem
			activeVisibilitySensors.deleteElementSwap(ti);
			ti--;
		}
	}
	if (activeVisibilitySensors.isModified()) {
		activeVisibilitySensors.touch();
		activeVisibilitySensors.setModified(FALSE);
	}
}

// inactive zone  not visible any more 
void GView::InActivateZones()
{
	for (int ti=0; ti<activeZones.getNum(); ti++) {
		GvNode *n = activeZones.get1(ti);
		BOOL purge= FALSE;

		if (RTISA(n,GvOcclusion)) {
			GvOcclusion *t = (GvOcclusion*)	n;

			if (t->refCnt <=1) {
				purge = TRUE;
			} else if (!t->isActive) {
				purge = TRUE;
			} else if (!t->wasRendered && t->isActive) {
				t->isActive.set(FALSE);			  // keep it in list,
				t->exitTime.set(theTime);
				purge = TRUE;
			}
			t->wasRendered = FALSE; // clear flag for next cylce 
		}
		else if (RTISA(n,GvInclusion)) {
			GvInclusion *t = (GvInclusion*)	n;


			if (t->refCnt <=1) {
				purge = TRUE;
			} else if (!t->isActive) {
				purge = TRUE;
			} else if (!t->wasRendered && t->isActive) {
				t->isActive.set(FALSE);			  // keep it in list,
				t->exitTime.set(theTime);
				purge = TRUE;
			}
			t->wasRendered = FALSE; // clear flag for next cylce 
		}
		
		if (purge) { // from mem
				activeZones.deleteElementSwap(ti);
				ti--;
		}
	} // for

	if (activeZones.isModified()) {
		activeZones.touch();
		activeZones.setModified(FALSE);
	}

}


// convert a pixel coordinate to normalized screen coordinate (-1..1) (-1 ..1)
void GView::Pixel2Screen(const CPoint &pixel,Point &screen,float screenZ)
{

   screen.Set((float) pixel.x / (float) (renderState.sizex-1), (float) (renderState.sizey-pixel.y) / (float)(renderState.sizey-1),0); 
   // x now in 0..1
   screen.Set(screen.x * 2.0 -1.0, screen.y *2.0 -1.0,screenZ);
}

// convert a screen coordinate to pixel 
void GView::Screen2Pixel(const Point &screen,Point &pixel)
{

   float x = (screen.x +1.0) / 2.0;
   float y = (screen.y +1.0) / 2.0;
   int yy  = y * (float)(renderState.sizey-1);
   pixel.Set(x * (float) (renderState.sizex-1), (renderState.sizey-yy) ,screen.z); 
}

// compute world ray for screen coordinate 
void GView::ComputeWorldRay(const Point &screen,RayRange &ray)
{

   Point objP;
   Point objP1;


#ifdef _D3D

   Point po=screen;

   float znear = 0.0; // -1.0;
   float zfar = 1.0;
   po.z = znear; 

   const Matrix& p2w = GetPhysicalCamera()->P2World();
   objP = p2w * po;
   
   po.z = zfar; 
   objP1 = p2w * po;

#endif


#ifdef _OGL
   GLdouble modelM[16];
   GLdouble projectionM[16];
   GLint viewport[4];
   GLdouble obj[3];

   GLdouble obj1[3];
   Point ps;

   int ret;

   Screen2Pixel(screen,ps);
   float x = ps.x;
   float y = ps.y;

   // to do cache these ?
   glGetDoublev(GL_MODELVIEW_MATRIX,modelM);
   glGetDoublev(GL_PROJECTION_MATRIX,projectionM);
   glGetIntegerv(GL_VIEWPORT,viewport);


   ret = gluUnProject(x,renderState.sizey-y,0.0,modelM,projectionM,viewport,&obj[0],&obj[1],&obj[2]);
   ret = gluUnProject(x,renderState.sizey-y,1.0,modelM,projectionM,viewport,&obj1[0],&obj1[1],&obj1[2]);
   glCheckError();


   
   objP.Set(obj[0],obj[1],obj[2]);
   objP1.Set(obj1[0],obj1[1],obj1[2]);
#endif

   ray.SetByPoints(objP,objP1);

}





/* do selection processing
  x,y are the mouse coordinates in window coordinates (pixel)
  flags are the associated mouse button flags
  action is the requested action
*/

float yz =0;

int GView::Select(int x, int y, int flags, int action)
{  int ret;


   Point mouseNormalized;
   
   Point objP;
   Point objP1;

   CString msg; 

   CString oldMessage; // the last message
 
   GFPSetControl(); // mask FP exceptions 

   if (viewController) oldMessage = viewController->GetLastMessage();
   gbool hitMessage = gfalse; // display hit messages


   GHitInfo *hit = NULL;
   GRaySelectTraversal traversal;

   // use physicalCamera for computing ray hit
   GCamera *theCamera = GetPhysicalCamera();

   mouseNormalized.Set((float) x / (float) (renderState.sizex-1),
					   (float) (renderState.sizey-y) / (float)(renderState.sizey-1),0);

   
   traversal.pscreen.Set((float) x , (float) (renderState.sizey-y) ,0);
   
   GRectInt &v=traversal.viewport;
   renderState.GetViewport(v.x,v.y,v.dx,v.dy);


#ifdef _D3D

   Point dir = theCamera->position- theCamera->target;
   float znear = 0.0; 
   float zfar = 1.0;

#if 0
   TRACE(" znear %f %f \n",theCamera->znear,theCamera->zfar);
   dir.Normalize();
   Point px = theCamera->position + dir * theCamera->znear;
   objP = theCamera->World2p() * px;
   TRACE("(%f %f %f) ==> (%f %f %f), %d\n",px.x,px.y,px.z,objP.x,objP.y,objP.z,ret);
   znear = objP.z;

   px = objP;
   objP = theCamera->P2World() * px;
   TRACE("(%f %f %f) inv ==> (%f %f %f), %d\n",px.x,px.y,px.z,objP.x,objP.y,objP.z,ret);

   px = theCamera->position + dir * theCamera->zfar;
   objP1 = theCamera->World2p() * px;
   TRACE(" (%f %f %f ) ==> (%f %f %f), %d\n",px.x,px.y,px.z,objP1.x,objP1.y,objP1.z,ret);
   zfar = objP1.z;

   px = objP1;
   objP1 = theCamera->P2World() * px;
   TRACE(" (%f %f %f ) inv==> (%f %f %f), %d\n",px.x,px.y,px.z,objP1.x,objP1.y,objP1.z,ret);
#endif


  theCamera->ComputeWorldRay(mouseNormalized.x,mouseNormalized.y,
							  objP,objP1, znear,zfar);

/*
   Point po;
   po.Set((float) x / (float) (renderState.sizex-1), (float) (renderState.sizey-y) / (float)(renderState.sizey-1),0); 
   // x now in 0..1
   po.Set(po.x * 2.0 -1.0, po.y *2.0 -1.0,0.0);
   po.z = znear; // 0.0; // znear;

   const Matrix& p2w = theCamera->P2World();
   objP = p2w * po;
   
   po.z = zfar; // 1.0; // zfar 
   objP1 = p2w * po;
   ret = 0;
*/

#endif


#ifdef _OGL
   GLdouble modelM[16];
   GLdouble projectionM[16];
   GLint viewport[4];
   GLdouble obj[3];

   GLdouble obj1[3];

   // to do cache these ?
   glGetDoublev(GL_MODELVIEW_MATRIX,modelM);
   glGetDoublev(GL_PROJECTION_MATRIX,projectionM);
   glGetIntegerv(GL_VIEWPORT,viewport);


//   ret = gluUnProject(x,renderState.sizey-y,-camera->TargetDistance()/camera->zfar,modelM,projectionM,viewport,&obj[0],&obj[1],&obj[2]);
   ret = gluUnProject(x,renderState.sizey-y,0.0,modelM,projectionM,viewport,&obj[0],&obj[1],&obj[2]);
   if (!ret) return(0);
   ret = gluUnProject(x,renderState.sizey-y,1.0,modelM,projectionM,viewport,&obj1[0],&obj1[1],&obj1[2]);
   glCheckError();

   
   objP.Set(obj[0],obj[1],obj[2]);
   objP1.Set(obj1[0],obj1[1],obj1[2]);
#endif

//   TRACE(" Pix (%d %d) ==> (%f %f %f), %d\n",x,y,objP.x,objP.y,objP.z,ret);
//   TRACE(" Pix 1(%d %d) ==> (%f %f %f), %d\n",x,y,objP1.x,objP1.y,objP1.z,ret);

#if 0
   {
       Point p0,p1;
       Point p0w,p1w;
       float fx = 1.0f / (renderState.sizex-1);
       float fy = 1.0f / (renderState.sizey-1);

       p0.Set(fx*x,fy*(renderState.sizey-y),0.0);
       p1.Set(fx*x,fy*(renderState.sizey-y),1.0);

       viewInfo.View2World(p0,p0w);
       viewInfo.View2World(p1,p1w);
       TRACE("nn Pix (%d %d) ==> (%f %f %f) %d\n",x,y,p0w.x,p0w.y,p0w.z,ret);
       TRACE("nn Pix 1(%d %d) ==> (%f %f %f) %d\n",x,y,p1w.x,p1w.y,p1w.z,ret);
   }

#endif    	   	

        traversal.method = GRaySelectTraversal::BY_OBJECT; 
//      traversal.method = GRaySelectTraversal::BY_BBOX; // quicker but incorrect 
		
		traversal.needShapes = FALSE; // need no shapes which aren't rendered

		switch (action) {
		case  JUMP_WWWAnchor:
		case  DRAG_SENSORS:
				traversal.storeFullHitInfo = 1;
				traversal.storePath = 1;
				break;
		case INFO:
		case SELECT:
		case FOLLOW_OBJECT:
				traversal.storePath = 1;
				traversal.storeFullHitInfo = 1;
				break;


		case  INFO_WWWAnchor: 
				traversal.storePath = 1; 
				traversal.storeFullHitInfo = 1;
				hitMessage = gtrue;

				break;
		default : 
			traversal.storePath = 1;
			traversal.storeFullHitInfo = 1;
			break;
		}

		InitializeGvAttributes(traversal);
		traversal.InitializeAttributes();
        
		// traversal should work like a rendering traversal
        // what is visible on the screen
        // or cache is what is selectable 

        traversal.SetCamera(globalCamera);
        traversal.SetPhysicalCamera(GetPhysicalCamera());
        traversal.viewInfo = &viewInfo;
		// set current time
		GTimeStack *ts= GTimeStack::NewElement();
		ts->time = time; 
		traversal.Set(ts); 

       
        traversal.ray.SetByPoints(objP,objP1);
		
		ASSERT(traversal.ray.d.Len()>0.0f);


		//if (orthographic) ...
		//double dist =  traversal.ray.d.Len();
		//traversal.ray.d *= camera->zfar/dist;
/*
		TRACE("Ray (%f %f %f) (%f %f %f) (%f- %f)  \n",traversal.ray.p.x,traversal.ray.p.y,traversal.ray.p.z,
                    traversal.ray.d.x,traversal.ray.d.y,traversal.ray.d.z,
                    traversal.ray.tmin,traversal.ray.tmax);
*/	    
	    Traverse(traversal);

		hit = traversal.GetHit();

		if (hit ) {	// ****** got a hit 

		ret = 1;

		switch (action) {
		case  JUMP_WWWAnchor:
				TRACE("Jump");
		case  INFO_WWWAnchor:
		case  DRAG_SENSORS:
		case  DRAG:
		case  RELEASE:

					{
					GvNode *hitNode;
					
					int ti;

#if 0
		// done from eventHandler at button up
					// deactivate old active TouchSensors
					for (ti=0; ti<activeTouchSensors.getNum(); ti++) {
						GvTouchSensor *t = (GvTouchSensor*) activeTouchSensors.get1(ti);

						for(int j=0; j<hit->sensors.Length(); j++) {
							if (hit->sensors[j] == t) {
								t = NULL; // keep this sensor 
								break;
							}
						}
						if (t)  { // t is no longer active 
							t->isActive.set(FALSE);
							activeTouchSensors.remove(t);
						}
					}

#endif
					// deactivate old over TouchSensors
					for (ti=0; ti<overTouchSensors.getNum(); ti++) {
						GvTouchSensor *t = (GvTouchSensor*) overTouchSensors.get1(ti);
						// is t in set of new sensors ??
						for(int j=0; j<hit->sensors.Length(); j++) {
							if (hit->sensors[j] == t) {
								t = NULL; // keep this sensor 
								break;
							}
						}
						if (t)  { // t is no longer active 
							t->isOver.set(FALSE);
							if (RTISA(t,GvMouseSensor)) {
								GvMouseSensor *s = (GvMouseSensor*) t;
								GvEvent* e=getEvent();
								e->type.set_("mouseout");
								// target node ???
								s->event.set(e);

							}
							overTouchSensors.remove(t);
						}
					}

					if (hit->sensors.Length() == 0) {
						// no sensor or anchor hit 
						if (hitMessage && GetLastMessageType() == HIT_MESSAGE) // clear message
							if (oldMessage.GetLength()>0)
								Message("",HIT_MESSAGE);
						  
						if (eventHandler == NULL) hasCustomSensorMessage=gfalse;

						ret = 0; // NO HIT 
					} else {	
						GMultitSensorHandler* sensorHandler=NULL;

						if (hit->sensors.Length() >1) {
							TRACE("Multi hit %d \n",hit->sensors.Length());

						}
						// do action on sensors 
						for(int i=0; i<hit->sensors.Length(); i++) {
							hitNode = hit->sensors[i];
							ret |= DoHitElement((SelectionAction) action,x,y,flags, *hit,hitNode,msg,sensorHandler);
						}
						

						if (hitMessage && viewController)  {
							if (viewController->GetLastMessage() != oldMessage)  { 
								hitMessage = gfalse;
								hasCustomSensorMessage=gtrue;
							}
							if (!hasCustomSensorMessage)
								if (hitMessage || (GetLastMessageType() == HIT_MESSAGE))
				                   if (oldMessage != msg)
									  Message(msg,HIT_MESSAGE);
						}

						// must install handler as event handler for sensors
						if (sensorHandler) {
							SetEventHandler(sensorHandler);
						}
					}
				}
				break;
		case INFO:
		case SELECT:
			{
			 if (!hit) {
               if (hitMessage && GetLastMessageType() == HIT_MESSAGE)
                  if (oldMessage.GetLength()>0)
					 Message("",HIT_MESSAGE);
             }
			 else {							  
                //HG GetReporter()->Status("Hit a %s at (%f /! %f) (t=%f)",
				//     hit->node->ClassName(),hit->p.x,hit->p.y,hit->p.z,hit->t);
                //pView->lastMessageType = HIT_MESSAGE; // ??
            }
			}

			break;


		case JUMP_OBJECT:
			{
			  if (hit) { // GCamera
			  	Point position;

                BBox bw;
                bw = hit->bbox;
                bw *= hit->m; // world bounding box

				BindViewpoint(NULL);

				position= bw.Center();
				//position= hit->pw;
				Point size = bw.Size();

				Point dir = theCamera->NormDirection();

		    	float field = max(max(fabs(size.x),fabs(size.y)),fabs(size.z));
				
				// prevent zoom out 
				if (field> (theCamera->height*.25f)) field = theCamera->height *0.25f;

                field = max (theCamera->height *0.25f,field);

				// take view angle constant 
				float targetDistance = field / (2.0f * tan(theCamera->GetFovY()*0.5f));

			    if (!cameraInterpolator) {
					cameraInterpolator = new GCameraInterpolator();
				}
				// copied from AnimateToCamera
				cameraInterpolator->a.Set(*theCamera); // current camera


				theCamera->target = hit->pw; // or center of bbox ?
				theCamera->height = field;
				theCamera->width = field;
				//theCamera->width = theCamera->height * theCamera->aspectRatio;
				theCamera->position = theCamera->target + targetDistance*dir;
		        theCamera->OnChanged();
				
				cameraInterpolator->b.Set(*theCamera);   // target camera
				cameraInterpolator->Reset();
				cameraInterpolateAll = 0;
				viewpointIndex = -1; // workaround to not go to last viewpoint
				AnimateCameraStep();

			  }
			}
			break;
		case FOLLOW_OBJECT:
			  if (hit) { // GCamera
				BindViewpoint(NULL);
				
				followObject = TRUE;
				followObjectPath = hit->path;
				followObjectPosition = hit->p;
				followObjectPositionGlobal = hit->pw;
				// transfrom this by inverse camera Matrix 
				followObjectNormal = hit->vn;
				followObjectCameraPosition =  camera->position;
					  	
			}
			break;

		default : break;

		}

		if (!lastHit) lastHit = new GHitInfo();

		*lastHit = *hit;

		} // hit 
		else  { // no hit 

			ret = 0;
			// deactivate old TouchSensors
			if (overTouchSensors.getNum() >0) {
				for (int ti=0; ti<overTouchSensors.getNum(); ti++) {
					GvTouchSensor *t = (GvTouchSensor*) overTouchSensors.get1(ti);
					t->isOver.set(FALSE);
				}
				overTouchSensors.deleteAll();

			}	

			if (eventHandler == NULL) hasCustomSensorMessage=gfalse;
					

			if (hitMessage && GetLastMessageType() == HIT_MESSAGE)
				 if (oldMessage.GetLength()>0)
					 Message("",HIT_MESSAGE);


     } // no hit  

	 // check for any redraw events

 	 // test, done in onIdle if (GvNode::getRedraw())  Redraw();
 	 return(ret);

}

#include "ghull.h"

// detect a Ray-based collision
// result 1: collisionPosition contains new position 
int GView::CollisionDetect(const Point &oldPosition, const Point &newPosition,  Point &collisionPosition,Point &collisionNormal)
{
  
	if (collisionDistance == 0.0) return 0; // new, bad value 

    GFPSetControl(); // mask FP exceptions 

	// compute collision vector 
	Point oldPos = oldPosition;
	Point newPos = newPosition;
	Point normal;
	
	if (!cameraMatrix.IsIdentity()) { // transform to global viewpoint coordinates
		oldPos = cameraMatrix.M() * oldPosition;
		newPos = cameraMatrix.M() * newPosition;
	}	

	Point dir= newPos - oldPos;
	float l = dir.Length();
	
	if (l==0.0) return 0;

	float newL = l+collisionDistance;

	dir *= (newL / l);

	GHitInfo *hit = NULL;
    GRaySelectTraversal traversal;
	traversal.method = GRaySelectTraversal::BY_OBJECT; 
	traversal.mode= GRaySelectTraversal::MODE_COLLISION;
	traversal.storeFullHitInfo = 1;
	traversal.storePath = 0;
	InitializeGvAttributes(traversal);
	traversal.InitializeAttributes();

	traversal.SetCamera(globalCamera);
    traversal.SetPhysicalCamera(GetPhysicalCamera());
	traversal.viewInfo = &viewInfo;

	// set current time
	GTimeStack *ts= GTimeStack::NewElement();
	ts->time = time; 
	traversal.Set(ts); 

       
    traversal.ray.SetByPoints(oldPos,oldPos+dir);

	Point v[8];
	Point dir90, up, down, diff;
	if (useRapid)
	{
		if (myRapid == NULL) {
			myRapid = new rapidCollide();
			myRapid->RAPID_contact_flag = FIRST_CONTACT;
		}

		traversal.mode= GRaySelectTraversal::MODE_RAPID;
		traversal.myRapid = myRapid;

		traversal.colCyclesPerFrame = 0;
		traversal.speedBoxMat = cameraMatrix.M();
		traversal.collision = FALSE;
		
		if (speedBox == NULL) {
					speedBox = new GShell;
					GFaceList flist;
					flist.SetLength(6*5);
					int *f = flist.DataRW();
					
					*f++=4;
					*f++=0;
					*f++=2;
					*f++=3;
					*f++=1;
					
					*f++=4;
					*f++=0;
					*f++=1;
					*f++=5;
					*f++=4;
					
					*f++=4;
					*f++=5;
					*f++=1;
					*f++=3;
					*f++=7;
					
					*f++=4;
					*f++=3;
					*f++=2;
					*f++=6;
					*f++=7;
					
					*f++=4;
					*f++=0;
					*f++=4;
					*f++=6;
					*f++=2;
					
					*f++=4;
					*f++=4;
					*f++=5;
					*f++=7;
					*f++=6;
					
					speedBox->SetFaceList(flist);
		}
		if (speedBoxGlobal == NULL) {
			speedBoxGlobal = new GShell;
		
			speedBoxGlobal->SetFaceList(speedBox->f);
		}


		traversal.speedBox = speedBox;
		traversal.speedBoxGlobal = speedBoxGlobal;

		//compute the speedBox

		//the direction to go
		dir = newPosition - oldPosition;//local coords
		dir.Normalize();

		//up vector
		up = camera->up;
		up.Normalize();	
		
		//vector showing to the side
		dir90 = dir;
		dir90.Rotate(up, PI*.5);

		//length of speedbox
		dir *= collisionDistanceLocal;

		//width of speedbox
		dir90 *= collisionDistanceLocal*0.6;

		if (stepOverSizeLocal>heightOverGroundLocal)//is not allowed, grounddetect fails!
			stepOverSizeLocal= heightOverGroundLocal;

		down = (-1*heightOverGroundLocal+stepOverSizeLocal)*up;
		up *= .1;

		v[0] = newPosition + dir + dir90 + up;
		v[1] =  newPosition + dir - dir90 + up;
		v[2] =  newPosition + dir + dir90 + down;
		v[3] =  newPosition + dir - dir90 + down;
	
		v[4] =  oldPosition + dir + dir90 + up; 
		v[5] =  oldPosition + dir - dir90 + up; 
		v[6] =  oldPosition + dir + dir90 + down; 
		v[7] =  oldPosition + dir - dir90 + down;
	
		speedBox->SetV(8, v);
		speedBoxGlobal->SetV(8,v);

		speedBox->ComputeRapidModel();
#if 0
			int numTriangles;
			Triangle *tri = NULL;

			if (ConvexHull3D (8, v, 
				numTriangles, tri) >0) {
				GFaceList f;
			
				for(int i=0; i<numTriangles; i++)
					f.AppendTri(tri[i][0],tri[i][2],tri[i][1]);

				delete [] tri;
				speedBox->SetFaceList(f);
				speedBox->SetFlag(SHELL_CONVEX | SHELL_CLOSED);

			}
#endif

		// orient planes of convex volume
		// so that center is inside

		Array<Plane> planes;
		speedBox->ComputeFaceNormals(1);

		planes.SetLength(speedBox->nfaces);

		for(int i=0; i<speedBox->nfaces;i++) {
			planes[i].n = speedBox->fn[i];
			planes[i].d = speedBox->finfo[i].plane_d;
			if (PlaneStatus(1, &newPosition, planes[i]) != INSIDE) {
				planes[i].n = -planes[i].n; 
				planes[i].d = -planes[i].d; 
			}
		}


		if (speedBoxVolume == NULL)
			speedBoxVolume = new GConvexVolume(cameraMatrix.M(),Transpose(cameraMatrix.Inverse()),speedBox->v,planes);
		else 
			speedBoxVolume ->Set(cameraMatrix.M(),Transpose(cameraMatrix.Inverse()),speedBox->v,planes);

		if (speedBoxVolume->Outside(speedBox->GetBBox())) {
			TRACE("Error");
		}

		traversal.speedBoxVolume = speedBoxVolume;

	}

    TraverseWithoutAvatar(traversal);

	//*******************************evaluate the results*********************
	hit = traversal.GetHit();
	if (!hit)
		return(0);
	else
	{
	if (useRapid)
	{
	
		traversal.mode = GRaySelectTraversal::MODE_RAPID_RECURSIVE;
		collisionPosition = oldPos + hit->p;


		TRACE("GView::CollisionDetect: traversal.colCyclesPerFrame: %i\n", traversal.colCyclesPerFrame);
#ifdef _DEBUG
		//MessageBeep(MB_OK);
#endif _DEBUG_CD	
		
		TRACE("hit.p       %f %f %f\n", hit->p.x, hit->p.y, hit->p.z);
		TRACE("oldPos      %f %f %f oldPosition %f %f %f\n", oldPos.x, oldPos.y, oldPos.z, oldPosition.x, oldPosition.y, oldPosition.z);
		TRACE("oldPos+hit.p%f %f %f newPosition %f %f %f\n", collisionPosition.x, collisionPosition.y, collisionPosition.z);
		TRACE("newPos      %f %f %f\n", newPos.x, newPos.y, newPos.z, newPosition.x, newPosition.y, newPosition.z);			collisionPosition = oldPos + hit->p;
	
		TRACE("GView::CollisionDetect:................................Rapid.HIT (end of traversal)\n\n");

	}//end useRapid

	//
	////ray collision
	//
	//
	else
	{
		
		collisionPosition = hit->pw; // global
		collisionNormal = hit->vn;
		
		// hit->vn is in local coordinate system, transform to global 
		collisionNormal = RotateOnly(hit->m,collisionNormal);  // <==== neu
		collisionNormal.Normalize();
		
		// make collisionNormal pointing do ray p // side of plane (hitPosition,hitNormal)
		
		if (DotProd(collisionNormal,oldPos) - DotProd(collisionPosition,collisionNormal) <0) {
			collisionNormal = - collisionNormal; // *flip*
		}			

		// must be at least collisionDistance units away
		Point dirNew= collisionPosition - oldPos;
		
		l = dirNew.Length();
		newL = l-collisionDistance;
		// if newL <=0 ==>  moved back 
		
		dirNew *= (newL / l);
		
		collisionPosition =  oldPos + dirNew;
	}//end raycollision


	if (!cameraMatrix.IsIdentity()) { // transform out value back to local viewpoint coordinates
		collisionPosition = cameraMatrix.Inverse() * collisionPosition;
		collisionNormal = RotateOnly(cameraMatrix.Inverse(),collisionNormal);
	}
	// send collideTime events to ALL nested collision nodes 
	for(int i=0; i<hit->sensors.Length(); i++) {
		GvNode *hitNode = hit->sensors[i];

		if (RTISA(hitNode,Gv2Collision)) {
			((Gv2Collision *) hitNode)->collideTime.set(theTime); 
		}
	}
	return(1);
	
	}//end hit
}



// result 1: newPosition contains new position 2:gravity, falling down
int GView::GroundDetect(const Point &oldPosition,  Point &newPosition,Point &hitPosition,Point &hitNormal) 
{
	int ret=1;

/*	if (GDCache.oldPosition == oldPosition) {
		newPosition = GDCache.newPosition;
		hitPosition = GDCache.hitPosition;
		hitNormal = GDCache.hitNormal;
		ret = GDCache.ret;
		return ret;
	}
*/	
	if (heightOverGround == 0.0) return 0; // if bad value 

	// compute collision vector 
	Point oldPos = oldPosition;

	if (!cameraMatrix.IsIdentity()) { // transform to global viewpoint coordinates
		oldPos = cameraMatrix.M() * oldPosition;
	}	
	
	Point downDirection;
    downDirection = RotateOnly(cameraMatrix.M(),downDirectionLocal);
    downDirection.Normalize();

	
	Vector dir = downDirection;

	GHitInfo *hit = NULL;
    GRaySelectTraversal traversal;
	traversal.method = GRaySelectTraversal::BY_OBJECT; 
	traversal.mode= GRaySelectTraversal::MODE_COLLISION;
	traversal.storeFullHitInfo = 1;
	traversal.storePath = 0;
	InitializeGvAttributes(traversal);
	traversal.InitializeAttributes();

	traversal.SetCamera(globalCamera);
    traversal.SetPhysicalCamera(GetPhysicalCamera());
    traversal.viewInfo = &viewInfo;

	// set current time
	GTimeStack *ts= GTimeStack::NewElement();
	ts->time = time; 
	traversal.Set(ts); 

	//we do a ray hit test from the cam position down to the ground 
    traversal.ray.SetByPoints( oldPos, (oldPos + dir*heightOverGround) );
	traversal.ray.tmax = 1e5f;

    TraverseWithoutAvatar(traversal);

	hit = traversal.GetHit();

	/////////////////////
	//get the results
	/////////////////////
	


	// no collision with this ray 
	if (!hit) {
		TRACE("No ground hit \n");
#ifdef _DEBUG
			//MessageBeep(MB_ICONASTERISK);
#endif _DEBUG
		return(0);
	}


	hitPosition = hit->pw;
	hitNormal = hit->vn;
	// hit->vn is in local coordinate system, transform to global 
	hitNormal = RotateOnly(hit->m,hitNormal);  

	// make hitNormal pointing do ray p // side of plane (hitPosition,hitNormal)

	if ((DotProd(hitNormal,oldPos) - DotProd(hitPosition,hitNormal)) <0) {
		hitNormal = - hitNormal; // *flip*
	}

	
	
	dir = hitPosition - oldPos;

	float dist = dir.Length();

	Point offset;
	
	heightOverGround	= TransformValue(cameraMatrix.M(),heightOverGroundLocal);

	TRACE("GView::GroundDetect: hitPos %g %g %g dist %g dir %g %g %g\n",hitPosition.x,hitPosition.y,hitPosition.z,dist,dir.x,dir.y,dir.z);
	
	if (dist == 0.0f) {
		dir = - downDirection;
		dist = 1;
		offset = dir * heightOverGround;
	} else {
	dir  *= 1.0/ dist;
	if (dist <=0) {
		dir = - downDirection;
		dist = 1;
		offset = dir * heightOverGround;
	}
	else if (dist <= heightOverGround)
		offset = (- dir) * (heightOverGround - dist);
	else {
		//offset = dir * (heightOverGround * 0.25); // fall down
		offset = (- dir) * (heightOverGround - dist);

		ret = 2;
	}
	}

	newPosition = oldPos + offset;
	// newPosition  = hitPosition + downDirection * -heightOverGround;


	if (!cameraMatrix.IsIdentity()) { // transform out value back to local viewpoint coordinates
		newPosition = cameraMatrix.Inverse() * newPosition;
	}


	return(ret);

}





// set all store GL context values 
//

int GView::SetContext(int canDisplayShareLists)
{
   renderState.SetContextAll();
   lightInit = 0;

#ifdef _OGL
   renderState.textureGeneration.glSet();
   renderState.textureParameters.glSet();

   if (renderState.textureDefined  && renderState.textureCache.IsValid())
       renderState.textureCache.Call();
#endif

   return(1);
}




// Traverse world 
int GView::Traverse(GTraversal &traversal) 
{ int ret;
  traversal.view = this;
  traversal.defaultMaterial10 = (GvMaterial *) defaultMaterial10.get();
  traversal.defaultAppearance = (GvAppearance *) defaultAppearance.get();
  traversal.defaultMaterial = (Gv2Material *) defaultMaterial.get();
  // traversal.appearance = traversal.defaultAppearance;  // appearance must never be NULL for glRenderTraversal
  traversal.defaultTextureTransform = (Gv2TextureTransform *)defaultTextureTransform.get();
  //traversal.defaultTexture = defaultTexture.get();

  currentTraversal = &traversal;

  //setBrowser(this); // set global browser variable

  //  ret = traversal.defaultAppearance->Traverse(traversal);
 
  if (theScene) {
	  theScene->ref();
		ret = theScene->Traverse(traversal);
	  theScene->unref();
  } 		
  else 
#ifdef _GFULL
  if (theWorld) {
	  ret = theWorld->Traverse(traversal); 
  } 	
  else 
#endif
  if (theShell) {
		ret = theShell->Traverse(traversal);
  }
  else ret = 0;


  currentTraversal = NULL;

  return(ret); 
}


// Traverse a specific node 
int GView::Traverse(GTraversal &traversal,GvNode *theNode) 
{ int ret;
  traversal.view = this;
  traversal.defaultMaterial10 = (GvMaterial *) defaultMaterial10.get();
  traversal.defaultAppearance = (GvAppearance *) defaultAppearance.get();
  traversal.defaultMaterial = (Gv2Material *) defaultMaterial.get();
  // traversal.appearance = traversal.defaultAppearance;  // appearance must never be NULL for glRenderTraversal
  traversal.defaultTextureTransform = (Gv2TextureTransform *)defaultTextureTransform.get();
  //traversal.defaultTexture = defaultTexture.get();

  currentTraversal = &traversal;

  //setBrowser(this); // set global browser variable

  //  ret = traversal.defaultAppearance->Traverse(traversal);
 
  theNode->ref();
  ret = theNode->Traverse(traversal);
  theNode->unref();

  currentTraversal = NULL;

  return(ret); 
}

// Traverse world without the avatar 
int GView::TraverseWithoutAvatar(GTraversal &traversal) 
{

    if (!showAvatar || avatarInfo.avatarSetChoice<0)
		return Traverse(traversal);

	GvSFInt32 val(-1);

	GvNode::sendEventIn(&val,avatarInfo.nodeChoice,avatarInfo.avatarSetChoice);

    int ret = Traverse(traversal);

	val.set((int) 0);

	GvNode::sendEventIn(&val,avatarInfo.nodeChoice,avatarInfo.avatarSetChoice);

	return ret;
}


void GView::InitDefaultNodes()
{
   	GvAppearance *a = new GvAppearance();
	Gv2Material *m= new Gv2Material();
	//a->material.set(m);
    // default appearance is simply empty, in VRML 2.0 no appearance in shape ==> UNLIT
	// == apperance.material == NULL
	defaultAppearance.set(a); 
	defaultMaterial.set(m);
	defaultTextureTransform.set((GvNode*)NULL);
	defaultTexture.set((GvNode*)NULL);

#ifdef _G_VRML1

	// VRML 1.0 top level material 
	GvMaterial *m1= new GvMaterial();
	defaultMaterial10.set(m1);

#endif _G_VRML1


}

void GView::UpdateDefaultNodes()
{
#ifndef _GV_MINIMAL

  	Point c;

	GvMaterial	*m10 = (GvMaterial *) defaultMaterial10.get();
	Gv2Material *m = (Gv2Material *) defaultMaterial.get();
#ifdef _OGL

	if (m10) {
  	     glMaterial &glm = renderState.material;
  		 glm.ambient.Get(c);
		 m10->ambientColor.set(c);
		 glm.diffuse.Get(c);
		 m10->diffuseColor.set(c);
		 glm.specular.Get(c);
		 m10->specularColor.set(c);
		 glm.emission.Get(c);
		 m10->emissiveColor.set(c);
		 m10->transparency.values[0]=(1.0-glm.alpha);
		 m10->shininess.values[0]=(glm.shininess/GV_SHININESS_GL_SCALE );
		 
	}
	if (m) {
  	     glMaterial &glm = renderState.material;
		 //  		 glm.ambient.Get(c); m->ambientColor.set(c);  // to do : ?? 
  		 glm.Ambient().Get(c); m->ambientIntensity.set(c.x);
		 glm.Diffuse().Get(c); m->diffuseColor.set(c);
		 glm.Specular().Get(c); m->specularColor.set(c);
		 glm.Emission().Get(c); m->emissiveColor.set(c);
		 m->transparency.set(1.0-glm.alpha);
		 m->shininess.set(glm.shininess/GV_SHININESS_GL_SCALE);
	}
#endif
#ifdef _D3D
	G3Material &glm = renderState.material;

	TRACE("to do d3d %s \n",__FILE__);
#endif
#endif

}

void GView::GetProfileSettings()
{
	int x;

	if (viewController)
	{
/*
	// test for software renderers 
	if (viewController->GetProfile(_T("Direct3D.perspectiveCorrection"),x)) 
			renderState.SetPerspectiveCorrectionHint((x>0));

	if (viewController->GetProfile(_T("Direct3D.specular"),x)) 
			renderState.SetSpecular((x>0));

	if (viewController->GetProfile(_T("Direct3D.rampSize"),x)) 
   		if (x>0) renderState.materialRampSize = x;
*/

	if (viewController->GetProfile(_T("Performance.useTextures"),x)) 
   		SetUseTextures(x>0); //useTextures = (x>0);

	if (viewController->GetProfile(_T("Performance.useMovies"),x)) 
   		SetUseMovies(x>0); //   		useMovies = (x>0);

	if (viewController->GetProfile(_T("Performance.useBackground"),x)) 
   		useBackground = (x>0);

	if (viewController->GetProfile(_T("Performance.maxTextureSize"),x)) 
   		maxUserTextureSize = x;
	}
}


//
// first time initialization
//
int GView::Initialize(
    HWND hWnd,
    HDC hDC,
	GDriverHints *driverHints

#ifdef _D3D__XXXX
    ,
	const char *driver,	// preferenced Driver, default "Ramp Driver"
	BOOL  bOnlySystemMemory,  // use only system memory for all surfaces
 	BOOL  bOnlyEmulation		// use no hardware 
#endif
)
{
    GLfloat	aspect;
	RECT	rect;
	CString fileName;
	CString strPathName;

	this->hWnd = hWnd;

#ifdef _FC_LOG
	frameRate.open("c:\\cc3d\\frameRate.log");
	budget.open("c:\\cc3d\\budget.log");
#endif

#ifdef WIN32
    GetClientRect(hWnd, &rect);
#else

    rect.left = 0;
    rect.right = 300;

    rect.top = 0;
    rect.bottom = 300;

#pragma message( "Initialize: HWND")

#endif

	if (!camera)
        camera = new GCamera();

	int haveModel = (theShell) || (theWorld) || (theScene);

    Resize(rect); // set dimension 

    GLint param; // zbuffer depth 

#ifdef _OGL
	int x;

    int depthBits=16;
	int rsoftware=0;

#ifdef _GLMESA
	device = new GXMesaDevice();
#endif

#ifdef _WIN32
	device = new GOpenglDeviceWGL(hDC);

	if (viewController && viewController->GetProfile(_T("OpenGL.zdepth"),x))
    { depthBits = x; }

	if (viewController && viewController->GetProfile(_T("OpenGL.software"),x))
    { rsoftware = x; }

	if (device->SetDefaultPixelFormat(1,depthBits,rsoftware)<=0) {
	   return (-1);
	}

	if (!device->IsSurfaceInitialized())
	{
		device->SetSurfaceInitialized();
	}

#endif


#endif


#ifdef _D3D

	param = 16; 
    /*
     * Initialize the global variables
     */
  
    bClearsOn = TRUE;
    bShowFrameRate = FALSE;
    bShowInfo = FALSE;

	// initialize D3D device 
	CWinApp *a = AfxGetApp();
	HWND hWndFrame =  hWnd;
	if (a  && (a->m_pMainWnd)) hWndFrame = (a->m_pMainWnd)->m_hWnd;
	else hWndFrame = AfxGetMainWnd()->GetSafeHwnd();

	device = new GD3Device();

    if (!CreateD3DDevice(hWnd,hWndFrame,driverHints->m_driver,driverHints))
		return -1;

	// turn on bilinear filtering
	if (device->Driver[device->CurrDriver].bUseTextureFiltering) {
		SetTextureFiltering(1);
	}
	else SetTextureFiltering(0);
	
	if ((device->WindowsDisplay.bpp <24) && device->Driver[device->CurrDriver].bIsHardware)
		SetDithering(1); // assume HW can do good dithering

	if ((device->WindowsDisplay.bpp <=8))
		SetDithering(1); // need dithering else bad !

#endif


#ifdef _OGL
    glGetIntegerv(GL_DEPTH_BITS,&param);
#else
    param = 16;
#endif

    camera->SetZNearFactor(param);
    globalCamera->SetZNearFactor(param);
    physicalCamera->SetZNearFactor(param);

	renderState.InitializeExtensions();

	RGBAColor bg;
	bg.Gray(0.0);

#ifdef _OGL
    glClearDepth(1.0 );
    renderState.SetNormalize(); // normalize normals after transformation only if transformation ???
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, renderState.perspectiveCorrectionHint=GL_NICEST); //  GL_FASTEST);
	glCheckError();
	renderState.GetLimits();
#endif

	renderState.SetBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 


    aspect = (GLfloat) rect.right / rect.bottom;
	if (!haveModel)
        camera->SetByVolume(-1.0,1.0,-1.0,1.0);

	camera->SetAspect(aspect);
	camera->SetFieldPixel(rect.right, rect.bottom);
	renderState.SetCamera(camera);
	renderState.SetCamera(GetPhysicalCamera());

#ifdef _OGL
    glMatrixMode(GL_MODELVIEW);
    glCheckError();
#endif

	renderState.SetBackfaceCulling(1);
    renderState.SetTextureParameters();

	SetRenderMode(GRenderGouraud);

#if _OGL
 	renderState.material.Default();

	renderState.material.diffuse.Set(1.0, 0.5, 1.0);
	renderState.material.specular.Gray(0.8);
	renderState.material.ambient.Gray(0.4);
	renderState.material.shininess = 80;
#endif

	renderState.SetMaterial();
	renderState.SetLitMaterial(TRUE);

#ifdef _OGL
 	renderState.backMaterial.Default();
	renderState.backMaterial.diffuse.Set(1.0, 0.0, 0.0);
	renderState.backMaterial.specular.Gray(0.8);
	renderState.backMaterial.ambient.Gray(0.4);
	renderState.backMaterial.shininess = 80;
	renderState.backMaterial.glSet(GL_BACK);
    glCheckError();

#endif

	renderState.normals.visible = 0;
	renderState.vertices.visible = 0;

	GetProfileSettings();

	// create default appearance nodes 
	InitDefaultNodes();

#ifdef _D3D
    renderState.SetState();
#endif 

	if (maxUserTextureSize>0) { // limit texture size below device limit
		int sx,sy;
		int msx,msy;
		int maxs = 1<<(maxUserTextureSize-1);
		
//		renderState.GetTextureCurrentMaxSize(sx,sy);
		renderState.GetTextureMaxSize(msx,msy);

		sx = min(msx,maxs); 		sy = min(msy,maxs);
		renderState.SetTextureCurrentMaxSize(sx,sy);
	}

#ifdef _OGL
#ifdef G_PANEL
	if (panel) panel->Init();
#endif
#endif

	// get directory name of program 
	strPathName = GetInstallDirectory();
	
	// get URL to default 3red person avatar url
	if (myAvatarURL.get().getLength() == 0)
		myAvatarURL.set(strPathName + "avatar.wrl");

#ifndef _GV_MINIMAL
    // read overlay scene , should contain LayerNode 
	fileName = strPathName;
	fileName += "interface";
	fileName += ".wrl";

	if (ExistsFile(fileName)) {
	    GvNode *root = NULL;
		if (ReadVrmlFile(fileName,GetReporter(),root)>=0) {
   		    foreground.set(root);
		}
	}
#endif

#if defined(WIN32) || defined(Linux)

	if (!haveModel)  
    {
		// try to read startup scene in prog directory with .wrl
		fileName = strPathName;
		fileName += _PROGRAM;
		fileName += ".wrl";

		if (ExistsFile(fileName)) {
			file->Set(fileName,0,0); 

			if (ReadModel(file) >= 0) {
				return 0;
			}
	    }

	    fileName += _PROGRAM;
	    fileName += ".bmp";

	    if (ExistsFile(fileName)) {
		    if (ReadTexturedPolygon(fileName,DIB_FMT) >0) {
			    return 0;
		    }
	    }

#ifndef _GLVIEW_DLL
	    // some startup scene 
	    SetScene(newSphereGrid());
	    return 0 ;
#endif

#if  defined(Linux)
		// display something 
        theShell = new_ronny();
#else 

#ifndef _GLVIEW_DLL
	    // some startup scene 
	    SetScene(newSphereGrid());
	    return 0 ;
#endif

#endif
	    if (theShell)
        {
	       BBox box;

	       theShell->ComputeBBox(box);

	       camera->SetByVolume(box.mi,box.ma);
	       camera->Zoom(1.1);
	       bbox = box;
	       camera->SetWorldReference(bbox);

           renderState.SetCamera(camera);
    
	       SetDefaultTextureParameters();

	       renderState.SetTexturing(0);
	    }
	} // haveModel

#else
    theShell = new_ronny();
    if (theShell)
    {
        BBox box;

        theShell->ComputeBBox(box);

        camera->SetByVolume(box.mi,box.ma);
        camera->Zoom(1.1);
        bbox = box;
        camera->SetWorldReference(bbox);

        renderState.SetCamera(camera);

        SetDefaultTextureParameters();

        renderState.SetTexturing(0);
    }

#endif

	return(0);
}


// resize rendering to rectangle 
void GView::Resize(int width,int height)
{
	TRACE("GView::Resize %i %i\n", width, height);

	float aspect;
	
	if ((height <=0) || (width <=0)) return;

	renderState.SetSize(width,height);

#ifdef G_PANEL

	if (panel)
		panel->SetScreenRect(width,height);

#endif

    aspect = (float) width / height;	// window ratio

	windowSize.set(width,height);
	windowAspect.set(aspect);

	// inform cameras of window size to adapt field of fiew 
	camera->SetAspect(aspect);
	camera->SetFieldPixel(renderState.renderSizex,renderState.renderSizey);

	if (globalCamera) {
		globalCamera->SetAspect(aspect);
		globalCamera->SetFieldPixel(renderState.renderSizex,renderState.renderSizey );
	}
	
	if (physicalCamera) {
		physicalCamera->SetAspect(aspect);
		physicalCamera->SetFieldPixel(renderState.renderSizex,renderState.renderSizey );
	}

}

// resize using rectangle
void GView::Resize(RECT &rect)
{
    Resize(rect.right,rect.bottom);
}


// resize using client rect of window 
void GView::Resize(HWND hWnd)
{
    RECT	rect;
#ifdef WIN32
    GetClientRect(hWnd, &rect);
#else
	// to do:
#endif 
    Resize(rect);
}

// get window properties
int GView::getWindowSizeX()
{
	return windowSize[0];
}
int GView::getWindowSizeY()
{
	return windowSize[1];
}


float GView::getWindowAspect()
{
	return windowAspect;
}




// the window has been resized moved 

void GView::OnSize(UINT nType, int cx, int cy)
{
#ifdef _D3D
	if (device && device->bInitialized)
       if (!device->bIgnoreWM_SIZE) {

		   if (!device->HandleWM_SIZE(device->GetHWnd(), nType, cx, cy)) {
			
		   	// couldn´t handle, fallback to software 			
			if (device->CurrDriver != device->SoftwareDriver)		
            if (!device->ChangeDriver(device->SoftwareDriver,NULL)) {
				ReportD3DError();
			} else {
				ReportD3DError();
			}
		  }
	   }
	updateIsComplex = TRUE;

#endif
	
	Resize(cx,cy);
}

// the window has moved 
void GView::OnMove(int cx, int cy)
{

#ifdef _D3D
	if (device) device->OnMove(cx,cy);
#endif 
}

void GView::OnActivate()
{
#ifdef _D3D
	if (device) device->OnActivate();
#endif
}

const IS GView::GEV_table[] = {
			{-1,"<Invalid>"},
			{GEV_mousedown,"mousedown"},
			{GEV_mousemove,"mousemove"},
			{GEV_mouseout, "mouseout"},
			{GEV_mouseover,"mouseover"},
			{GEV_mouseup,	"mouseup"},
			{GEV_keydown,"mousedown"},
			{GEV_keyup,"keyup"},
			{GEV_keypress,"keypress"},
			{GEV_click,"click"},
			{GEV_dblclick,"dblclick"},
			{GEV_joystick,"joystick"},
			{GEV_MOUSE,"mouse"},
			{GEV_KEY,"key"},
			{GEV_CLICK,"click"},
			{0,NULL},
			};

// parse an given filter string and compute an event mask
int GView::parseEventMask(const char *filter)
{
	return 0;
}



// get pressed status of VIRTUAL key code
// TBD: where are UNICODE keycode defines ??

gbool GView::isKeyPressed(int key)
{
#ifdef WIN32
	// win32
	return ((::GetKeyState(key) & 0x8000) !=0); 
#else
	return gfalse;
#endif

}


gbool  GView::triggerEvent(const char *type)
{
  GvEvent* e=getEvent();
  if (e== NULL) return FALSE;

  e->returnValue.set(TRUE);
  e->type.set_(type);
  event.touch();
#ifdef _DEBUG
  if (!e->returnValue) {
	  TRACE("Gview :: event was handled %s \n", type);
  }	

#endif
  // if script modified returnValue to FALSE, event was handled
  return !e->returnValue;

}

gbool GView::triggerMouseEvent(GBrowserInputEventFlags mask, const char *type,float x,float y, unsigned int flags)
{
  if ((eventMask.get() & mask) == 0) return FALSE;

  GvEvent* e=getEvent();
  if (e== NULL) return FALSE;

  e->setMouseStatus(x,y,flags);

#ifdef WIN32
  e->altKey.set((GetKeyState(VK_MENU) & 0x8000) !=0); 
  e->ctrlKey.set((GetKeyState(VK_CONTROL) & 0x8000) !=0); 
  e->shiftKey.set((GetKeyState(VK_SHIFT) & 0x8000) !=0); 
#endif



  Point p;
  Pixel2Screen(CPoint(x,y),p);

  e->position.set(p);	

  return triggerEvent(type);
}

gbool GView::triggerMouseEvent(GBrowserInputEventFlags mask, const char *type,POINT client, POINT screen, unsigned int flags)
{
  if ((eventMask.get() & mask) == 0) return FALSE;

  GvEvent* e=getEvent();
  if (e== NULL) return FALSE;

#ifdef WIN32
  e->altKey.set((GetKeyState(VK_MENU) & 0x8000) !=0); 
  e->ctrlKey.set((GetKeyState(VK_CONTROL) & 0x8000) !=0); 
  e->shiftKey.set((GetKeyState(VK_SHIFT) & 0x8000) !=0); 
#endif


  e->screen.set(screen.x,screen.y);
  Point p;

  Pixel2Screen(client,p);
  e->position.set(p);	
  e->setMouseStatus(client.x,client.y,flags);

  return triggerEvent(type);

}


gbool GView::triggerKeyboardEvent(GBrowserInputEventFlags mask, const char *type,unsigned int code, unsigned int flags)
{
  if ((eventMask & mask) == 0) return FALSE;

  GvEvent* e=getEvent();
  if (e== NULL) return FALSE;

#ifdef WIN32
  e->altKey.set((GetKeyState(VK_MENU) & 0x8000) !=0); 
  e->ctrlKey.set((GetKeyState(VK_CONTROL) & 0x8000) !=0); 
  e->shiftKey.set((GetKeyState(VK_SHIFT) & 0x8000) !=0); 
#endif


  e->keyCode.set((int)code);

  return triggerEvent(type);
}

gbool GView::triggerSizeEvent(GBrowserInputEventFlags mask, const char *type,float x,float y, unsigned int flags)
{
  //if ((eventMask & mask) == 0) return FALSE;
  GvEvent* e=getEvent();
  if (e== NULL) return FALSE;
  e->position.set(x,y);

  return triggerEvent(type);
}


// fullscreen support 
int GView::GetFullScreen()
{

#ifdef _D3D
//	return m_bTestFullScreen;
	if (!device) return 0;

	return device->bFullscreen;
#endif
#ifdef _OGL
	// ChangeDisplaySettings ??? 
	//WM_DISPLAYCHANGE message is sent to all windows when the display resolution has changed.
	//WM_DISPLAYCHANGE  
	//cBitsPerPixel = wParam; 
	//cxScreen = LOWORD(lParam); 
	//cyScreen = HIWORD(lParam); 
    return(fullScreen);
#endif

}






//////////////////////////////////////////////////////////
//														//
//														//
//				GView::SetFullScreen					//
//														//
//////////////////////////////////////////////////////////

// netscape hierarchie: 
// netscape Afx:400000:b:1466:6:2317 -> Afx:400000:3028 -> aPluginWinClass -> AfxOleControl42d

// IEserver->afxolecontrol->wnd(=device->hwnd)
//
//tbd:
//does not work in Netscape, maybe due to different types of leaf windows?
//problems with chat
//catch errors
//better to use olecontrol?


int GView::SetFullScreen(int mode)
{

	TRACE ("============================Gview:: SetFullScreen : %d ====================================\n",mode);

#ifdef _D3D
 	NavigationPanelDestroy();

		/*
         * Return to a windowed mode.  Let D3DApp decide which
         * D3D driver to use in case this one cannot render to
         * the Windows display depth
         */
		
	if (mode == 0) //////////////////////go back to windowed mode\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/
	{
		//
		// delete the full screen window
		//
		if (!SetParent(device->hwnd, device->hwndParent)) 
		{
			TRACE("GView::SetFullScreen: Could not set back window handle!\n");
		}

		fullScreenWnd->DestroyWindow();
		device->hwndFullScreenWnd = NULL;

		m_bTestFullScreen = FALSE;


		SetForegroundWindow(device->hwndFrame);
		ShowWindow(device->hwndFrame, SW_SHOW);

        if (!device->WindowMode(D3DAPP_YOUDECIDE, D3DAPP_YOUDECIDE)) 
		{
			if (device->CurrDriver != device->SoftwareDriver)	{	
				if (!device->ChangeDriver(device->SoftwareDriver,NULL)) {
					ReportD3DError();
				} 
			} else {
				ReportD3DError();
			}

        }

#ifndef _GLVIEW_DLL
		// new CFrameWnd
		CMainFrame *pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		if (pFrame) {
			 //pFrame->m_wndStatusBar.UpdateWindow();
		     pFrame->RecalcLayout(TRUE);
		}
#endif _GLVIEW_DLL


	} 
	
	else //////////////////////let's go fullscreen\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/
	{		
	
		//
		// create a new top level window (top level == frame window or dialog window!)
		//
		fullScreenWnd = new CFrameWnd();

		if (!fullScreenWnd->Create(NULL, "BLAXXUN FULLSCREEN", WS_OVERLAPPEDWINDOW | WS_MAXIMIZE & ~(WS_BORDER | WS_CAPTION  | WS_DLGFRAME | WS_THICKFRAME), CRect(0, 0, 20, 20), NULL, NULL, 0, NULL ))
			return FALSE;
		device->hwndFullScreenWnd = fullScreenWnd->GetSafeHwnd();

		//
		//switch off unnecessary window decoration of the frame window
		//
		LONG s = GetWindowLong(device->hwndFullScreenWnd,GWL_STYLE);		
		s &= ~(WS_BORDER | WS_CAPTION  | WS_DLGFRAME | WS_THICKFRAME);
		if (::SetWindowLong(device->hwndFullScreenWnd,GWL_STYLE,s) ==0) 
		{
			TRACE("GView::SetFullScreen: set window long  failed \n");
		}
		
		//
		// disable client borders 
		//
		long sEx = GetWindowLong(device->hwnd,GWL_EXSTYLE);
		sEx &= ~(WS_EX_CLIENTEDGE | WS_EX_WINDOWEDGE );
		SetWindowLong(device->hwnd, GWL_EXSTYLE, sEx);
		
			
		fullScreenWnd->ShowWindow(SW_SHOW);
		fullScreenWnd->SetForegroundWindow();

		//
		//set new parent window
		//
		device->hwndParent = SetParent(device->hwnd, device->hwndFullScreenWnd);
		if (device->hwndParent)
		{
			TRACE("GView::SetFullScreen: SetParent(device->hwnd, device->hwndFullScreenWnd) suceeded!\n");
		}
		else
		{
			TRACE("GView::SetFullScreen: SetParent(device->hwnd, device->hwndFullScreenWnd)   F A I L E D !\n");
			return FALSE;
		}

		
		//
		//move the client window	
		//CAUTION: after succesfull mode change move window is forbidden in ctrl!
		//
		int w = device->Mode[device->CurrMode].w;
		int h = device->Mode[device->CurrMode].h;

		//////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
		//change the device (setcooplevel, set up new device...)
		//////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\	
		//if (1) //test
		if (device->FullscreenMode(device->CurrMode)) 
		{
			//adapt to the new screen resolution
			MoveWindow(device->hwndFullScreenWnd, 0, 0, w, h, FALSE);

			RECT clientRect;
			fullScreenWnd->GetClientRect(&clientRect);
			TRACE("hwndFullScreenWnd %d %d %d %d \n",clientRect.left,clientRect.top,clientRect.right,clientRect.bottom);
			

			if (MoveWindow(device->hwnd,0,0,clientRect.right,clientRect.bottom,TRUE)) 
			{
				TRACE("GView::SetFullScreen: width (%i) height (%i)\n", w, h);
			}
			else
			{
				TRACE("GView::SetFullScreen: Move window failed ");
			}

			//
			//resize the camera
			//
			Resize(w,h);

#ifdef _DEBUG	
					m_bTestFullScreen = TRUE;
					RECT fr,wr;
					GetWindowRect(device->hwndFullScreenWnd,&fr);
					GetWindowRect(device->hwnd,&wr);
					TRACE("hwndFullScreenWnd %d %d %d %d hwnd %d %d %d %d \n",fr.left,fr.top,fr.right,fr.bottom,wr.left,wr.top,wr.right,wr.bottom);
#endif
		}

		else //fullscreen failed
		{
            ReportD3DError();
			goto exit_with_error;
        } 
	
		
	}//end fullscreen

	//if (panel)
	//	panel->Init(&device);

	
	return(GetFullScreen());

exit_with_error:
	TRACE("=========================GView::SetFullScreen F A I L E D !===========================\n");

	// fall back to window mode 
	if (!device->WindowMode(D3DAPP_YOUDECIDE, D3DAPP_YOUDECIDE)) 
	{
		//fatal error
		//tbd
	}
	return FALSE;

#endif _D3D



#if defined(_OGL) && defined(_MFC)									 

	if (mode) {
		CFrameWnd *pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);
		if (!fullScreen) {
		   if (pFrame) {
			   savedFrameWinStyle= pFrame->GetStyle();
			   CMenu *m = pFrame->GetMenu();
			   if (m) savedFrameMenu =  (HMENU) (*m); else savedFrameMenu = NULL;
		   }
		   savedWinStyle = pView->GetExStyle();
		}
		pView->ModifyStyleEx(WS_EX_CLIENTEDGE | WS_EX_WINDOWEDGE,0);
		if (pFrame) {
			pFrame->SetMenu(NULL);
			pFrame->ModifyStyle(WS_BORDER | WS_CAPTION  | WS_DLGFRAME | WS_THICKFRAME,0); // WS_MAXIMIZE);
			pFrame->ShowWindow(SW_SHOWMAXIMIZED); 
		}
		RECT r;
		if (pFrame) {
			pFrame->GetClientRect(&r);
			pView->MoveWindow(0,0,r.right,r.bottom,TRUE);
		}
		fullScreen = TRUE;
	} else {
		CFrameWnd *pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);
		if (fullScreen) {
			CMenu mnu;
			mnu.Attach(savedFrameMenu);

		   pView->ModifyStyleEx(0,savedWinStyle);

		   if (pFrame) {
			   pFrame->ModifyStyle(0,savedFrameWinStyle);
			   pFrame->ShowWindow(SW_RESTORE); 
			   pFrame->SetMenu(&mnu);

		   }
		}
		fullScreen = FALSE;
	}

	
	return(GetFullScreen());

#endif

	return FALSE;

}

int GView::ToggleFullScreen()
{
	return (SetFullScreen(!GetFullScreen()));
}



#ifdef _OGL
GLvoid drawCone(GLvoid)
{
    GLUquadricObj *quadObj;

    glColor3f(0.0, 1.0, 0.0);

    glPushMatrix();
	glTranslatef(-1.0, 0.0, 0.0);
	quadObj = gluNewQuadric();
	gluQuadricDrawStyle(quadObj, (GLenum) GLU_FILL);
	gluQuadricNormals(quadObj, (GLenum) GLU_SMOOTH);
	gluCylinder(quadObj, 1.0, 0.0, 1.0, 20, 10);
	gluDeleteQuadric(quadObj);
    glPopMatrix();

}

GLvoid drawPyramid(GLvoid)
{

    glPushMatrix();

	glTranslatef(1.0, 0.0, 0.0);
	glBegin(GL_TRIANGLE_FAN);
	    glColor3f(1.0, 0.0, 0.0);
	    glVertex3f(0.0, 1.0, 0.0);
	    glColor3f(0.0, 1.0, 0.0);
	    glVertex3f(-1.0, 0.0, 0.0);
	    glColor3f(0.0, 0.0, 1.0);
	    glVertex3f(0.0, 0.0, 1.0);
	    glColor3f(0.0, 1.0, 0.0);
	    glVertex3f(1.0, 0.0, 0.0);
	glEnd();
    glPopMatrix();
}
#endif 

#ifndef  _GV_MINIMAL

int GView::ReadMaterial(int fmt,const char *FileName)
{ int ret;

#ifdef _OGL
  glMaterial m;
#endif
#ifdef _D3D
  G3Material m;
#endif

  ///ret = renderState.material.
  ret=m.ReadFromInventorFile(FileName);
  renderState.SetMaterial(m);
  renderState.SetLitMaterial(TRUE);
  return(ret);
}



// Build a shell for the world
int GView::BuildShell()
{
	if (theWorld || theScene) {
	  GBuildShellTraversal t;
   	  t.InitializeAttributes();
  	  t.view = this;

	  InitializeGvAttributes(t);

	  Traverse(t);

	  SetShell(t.GetShell());
	  renderState.faces.colored = 1; // to see effect
	  t.shell = 0;
	  return(1);
	
	} else 
	if (theShell) {
	 	if (RTISA(theShell,GShell)) {
		GShell *s = (GShell *) theShell;
		
		// convert to this type 
		s = (GShell*) s->EvalAs(GShell::ClassId(),1);
		
		if (s != theShell) { 
			SetShell(s,0);
		}
		return (1);
	}

	}
	return(-1);
 }

// Compute a shell for the world

GShell* GView::ComputeShell()
{  GShell *result;
	if (theWorld || theScene) {
	  GBuildShellTraversal t;
 
      t.InitializeAttributes();
  	  t.view = this;

	  InitializeGvAttributes(t);

	  Traverse(t);
	  result = t.GetShell();
	  //renderState.faces.colored = 1; // to see effect
	  t.shell = 0;
      if (result) result->UpdateActions();

	  return(result);
	
	} else 
	if (theShell) {
	 	if (RTISA(theShell,GShell)) {
		GShell *s = (GShell *) theShell;
		
		// convert to this type 
		s = (GShell*) s->EvalAs(GShell::ClassId(),1);
        result = s;
		
		//if (s != theShell) { 
		//	SetShell(s,0);
		//}
        if (result) result->UpdateActions();
		return (s);
	}

	}

	return(NULL);
}

#endif


GLoadTask *tasks;

int GView::AddTask(GLoadTask *newTask,int where)
{
  newTask->next = tasks;
  tasks = newTask;
  return(1);
}

void GView::ClearTasks()
{
/*
 GLoadTask *t;
 while(tasks) {
 	t=tasks->next;
	tasks->next = NULL;
	delete tasks;
	tasks = t;
 }
*/
}


int GView::CheckTasks()
{
   return(0);
}

// some child node has been changed
// triggered if a node receives an eventIn
int GView::OnChildChanged(GvNode *child)
{
   	//TRACE("GView::OnChildChanged(%s)\n",child->ClassName());

	if (child == (GvNode*) boundViewpoint) {
		boundViewpointChanged = 1;
	} else 
	if (child == (GvNode*) boundBackground) {
		boundBackgroundChanged = 1;
	} else 
	if (child == (GvNode*) boundFog) {
		boundFogChanged = 1;
	} else 
	if (child == (GvNode*) boundNavigationInfo) {
		boundNavigationInfoChanged = 1;
	}
	return(0);
}



// sphere with parameters arround y
GShellI *NewSkySphere(int sky,float cx,float cy,float cz,float r, 
                       int n1,int n2,const float *angles,
					   int nColors, const PointArray &colors,
                       GShellI *o=NULL)
{

		int 	i,j;

		if (n1<=0) n1=12;
		if (n2<=0) n2=n1 / 2;

		n1+=1;
		n2+=1;

		PointArray poly(n1*n2);
		PointArray params(n1*n2);
		PointArray norms(n1*n2);

		if (!o)
			  o = new GShellI;

		
		GVertexI *verts = new GVertexI[n1*n2];


		float du = TWOPI / (float) (n1-1);
		float dv = PI / (float) (n2-1);

        float dpu = 1.0f / (float) (n1-1);
        float dpv = 1.0f / (float) (n2-1);

        float c0 = -PI*0.5;
		
		for (i=0; i<n1; i++) {

          		int offset = i * n2;

				float u = c0 +  du * i;
        		float cosu=cos(u);  // VC 4.1 bad value if optimized
				float sinu=sin(u);

				float pu= 1.0 - ((float) (i) * dpu );
				
				for (j=0; j<n2; j++) {
					float v;
					if (j == 0) v= 0; 
					else v = angles[j-1];
					if (sky) {

					}
					else v = PI - v;

					float cosv=cos(v);
					float sinv=sin(v);

					norms[offset+j].x= cosu*sinv;
					norms[offset+j].z= sinu*sinv;
					norms[offset+j].y= cosv;
					norms[offset+j].Normalize();

					poly[offset+j].x= cx+r*cosu*sinv;
					poly[offset+j].z= cz+r*sinu*sinv;
					poly[offset+j].y= cy=r*cosv;

					params[offset+j].Set(pu,(float) (j) *dpv ,0.0);

					verts[i*n2+j].v = offset+j;
					verts[i*n2+j].c =  min(nColors-1,j);
					verts[i*n2+j].n = offset+j;
					verts[i*n2+j].p = offset+j;

				}
		}
/*
		j=0;
		for(i=1; i<n1; i ++) { verts[i*n2+j].v = verts[0*n2+j].v;verts[i*n2+j].n = verts[0*n2+j].n; }

		j=n2-1;
		for(i=1; i<n2; i ++) { verts[i*n2+j].v = verts[0*n2+j].v;verts[i*n2+j].n = verts[0*n2+j].n; }

		
		i=n1-1;
		for(j=0; j<n2; j ++) { verts[i*n2+j].v = verts[0*n2+j].v;verts[i*n2+j].n = verts[0*n2+j].n; }
*/



		o->SetV(n1*n2,poly);


		GFaceList flist((n1-1)*(n2-1)*5);

		o->SetVN(norms);
		o->SetVP(params);
		o->SetVC(colors);

		o->SetVertices(n1*n2,verts);
		delete verts;

		flist.SetLength(0);

		for(i=0; i< (n1-1); i++) {
			int voffset = i*n2;
			for(j=0; j<(n2-1); j++) {
				if (!sky)
				flist.AppendQuad(voffset + (j+ 1),voffset+j,voffset+j+n2,voffset+ n2 + (j+ 1) );
				else flist.AppendQuad(voffset + (j+ 1),voffset+ n2 + (j+ 1),voffset+j+n2,voffset+j ); 
				//TRACE(" %d %d %d %d \n",voffset + (j+ 1),voffset+ n2 + (j+ 1),voffset+j+n2,voffset+j ); 
			}
		}

		o->SetFaceList(flist);
		
		o->UpdateActions();
		o->UnSetAction(SHELL_COMPUTE_VNORMALS);

		return(o);
}



int GvBackground::Apply(GView *view)
{
	int numSkyColors = skyColor.getNum();
	int numSkyAngles = skyAngle.getNum();
	int numGroundColors = groundColor.getNum();
	int numGroundAngles = groundAngle.getNum();

	view->SetBackground(NULL);

	GShellI *sky = NULL;
	GShellI *ground = NULL;

#ifdef _D3D
	if (!view->renderState.canDoRgbLighting) {
		// we don´t have color per vertex 
		if (numSkyColors > 1) numSkyColors = 1;
		numGroundColors = 0;
	}

#endif


#ifdef XXXXXXXXXXXXXXXXXXXXXXXXxxx
	if  (numSkyColors > 0) { // no background 
		Point color;
		skyColor.get1(0,color);
		view->SetBackgroundColor(color);

	} 

#else 

	if  (numSkyColors == 1) {
		Point color;
		skyColor.get1(0,color);
		view->SetBackgroundColor(color);

	} else {

		if  (numSkyColors >= 2 && (numSkyAngles>0) ) {
		
			PointArray colors;
			skyColor.get(colors);
			sky = NewSkySphere(1,0,0,0,2.0, 
                       4*3,numSkyAngles,skyAngle.get(),
					   numSkyColors,colors);
		}
		// band from  last sky angle to PI (y down) should be clampe to last color
		if  (numSkyColors > 0) {
			Point color;
			skyColor.get1(numSkyColors-1,color);
			view->SetBackgroundColor(color);
		}
		// could find out last ground groundAngle and limit sky to that 
		// to minimize overdraw 

	}

	if  (numGroundColors >= 2 && (numGroundAngles>0) ) {
			PointArray colors;
			groundColor.get(colors);
			ground = NewSkySphere(0,0,0,0,2.0, 
                       4*3,numGroundAngles,groundAngle.get(),
					   numGroundColors,colors);
	}
#endif


	if (sky == NULL && ground == NULL   
	    && (backUrl.getNum()==0) 
	    && (frontUrl.getNum()==0) 
	    && (bottomUrl.getNum()==0) 
	    && (topUrl.getNum()==0) 
	    && (leftUrl.getNum()==0) 
	    && (rightUrl.getNum()==0)) 
	{
		return (0);
	}

	BOOL onlyFront = 
			(backUrl.getNum()==0) 
			&& (frontUrl.getNum()>0) 
			&& (bottomUrl.getNum()==0) 
			&& (topUrl.getNum()==0) 
			&& (leftUrl.getNum()==0) 
			&& (rightUrl.getNum()==0) ;

	if (onlyFront) { // keep as backgroundImage 
	}


    GvLayer *layer = new GvLayer();
	layer->setBrowser(view);
	
    layer->fog.set(FALSE);
    layer->pick.set(FALSE);

	layer->clear.set(GvLayer::NONE);
    layer->depthWrite.set(FALSE);
    layer->depthFunc.set(GvLayer::ALWAYS);
    layer->lighted.set(FALSE);
	
    layer->depthFunc.set(GvLayer::ALWAYS);

	///xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	GvRotation *layerRotation = new GvRotation();
	layerRotation->setBrowser(view);

	// add shape Hints 
	{
        GvShapeHints *h = new GvShapeHints();
		h->setBrowser(view);
        h->shapeType.set(GvShapeHints::SOLID);
        h->vertexOrdering.set(GvShapeHints::COUNTERCLOCKWISE);
        h->faceType.set(GvShapeHints::CONVEX);
		layer->addChild(h);
	}
    // hg test why ? layer->addChild(new GvMaterial());

	if (onlyFront) {
		if (frontUrl.getNum()>0) {
			layer->addChild( newTFace((const Point *) front,frontUrl));
		}
	}

	// rotation 
    layer->addChild(layerRotation);

	// to do: d3d && ! rampMode  ??
	if (sky) {
		GvNodeShell *node = new GvNodeShell();
		node->setBrowser(view);
		node->SetShell(sky);
		GvShape *shape = new GvShape();
		shape->setBrowser(view);
		shape->geometry.set(node);
		layer->addChild(shape);
	}
	if (ground) {
		GvNodeShell *node = new GvNodeShell();
		node->setBrowser(view);

		node->SetShell(ground);
		GvShape *shape = new GvShape();
		shape->setBrowser(view);
		shape->geometry.set(node);
		layer->addChild(shape);
	}

	if (backUrl.getNum()>0) {
		layer->addChild( newTFace((const Point *) back,backUrl));
	}
	if (!onlyFront && frontUrl.getNum()>0) {
		layer->addChild( newTFace((const Point *) front,frontUrl));
	}

	if (bottomUrl.getNum()>0) {
		layer->addChild( newTFace((const Point *) bottom,bottomUrl));
	}

	if (topUrl.getNum()>0) {
		layer->addChild( newTFace((const Point *) top,topUrl));
	}
	
	if (leftUrl.getNum()>0) {
		layer->addChild( newTFace((const Point *) left,leftUrl));
	}

	if (rightUrl.getNum()>0) {
		layer->addChild( newTFace((const Point *) right,rightUrl));
	}

	view->SetBackground(layerRotation,layer);

	return(1);

}



// Background

int GView::SetBackground(GvNode *node)
{
    background = node;
    backgroundRotation = NULL;
    backgroundImage = "";
    return(node != NULL);
}

int GView::SetBackground(GvRotation *rotation,GvNode *node)
{
    background = node;
    backgroundRotation = rotation;
    backgroundImage = "";
    return(node != NULL);
}


void GView::ClearBackground()
{
    SetBackground(NULL);
    backgroundImage = "";
}


int GView::SetBackgroundImage(const char *FileName,int format)
{
    SetBackground(NULL); // recover space
	return(0);

#if 1 //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
    
    backgroundImage = FileName;
	// obsolete ?? !!!!!!!!!!!!!!!

    if (FileName[0] == 0) return(0);

    if (format == UNKNOWN_FMT)
       format = GetFileType(FileName); 

    GvLayer *layer = new GvLayer();   
    layer->fog.set(FALSE);
    layer->pick.set(FALSE);
	layer->clear.set(GvLayer::NONE);


    if (format <= VRML_FMT) { // assume a 3d scene
        layer->clear.value = GvLayer::NONE;
        Gv2Inline *WWWInline = new Gv2Inline();
        WWWInline->url.makeRoom(1);
        WWWInline->url.set1(0,FileName);
        layer->addChild(WWWInline);
    } else { // assume image

		layer->depthWrite.set(FALSE);
		layer->depthFunc.set(GvLayer::ALWAYS);
		layer->lighted.set(FALSE);

		{
			GvShapeHints *h = new GvShapeHints();
			h->shapeType.set(GvShapeHints::SOLID);
			h->vertexOrdering.set(GvShapeHints::COUNTERCLOCKWISE);
			h->faceType.set(GvShapeHints::CONVEX);
			layer->addChild(h);
		}
		//layer->addChild(new GvMaterial());
		GvMFString url(1);
		url.set1(0,FileName);
		layer->addChild( newTFace((const Point *) ::center,url));
    }
    
    int ret = SetBackground(layer);
    backgroundImage = FileName;
	return(ret);
#endif

}

int GView::EditBackgroundImage(const char *FileName,int format)
{
    int ret = SetBackgroundImage(FileName,format);
#ifdef _G_VRML1

    if (ret) {
      if (sceneInfo)       
        if (sceneInfo->backgroundImage) {
           // to do: relative filename 
            sceneInfo->backgroundImage->string= FileName;
        } else {
           sceneInfo->backgroundImage = new GvInfo();
           // to do: relative filename 
           sceneInfo->backgroundImage->string= FileName;
           sceneInfo->backgroundImage->setName(GvName("BackgroundImage"));
           //addTopLevelNode(sceneInfo->backgroundImage);
        }
    }
#endif _G_VRML1

    return(ret);

}

// find proto for extern proto with url "url", and optional name "name"
GvProto* 
GView::getProto(const char *url,const char *name)
{
	if (!!*url) 
	for(int i=0; i< protoScenes.getNum(); i++) {
		GvScene* s = (GvScene *) protoScenes.get1(i);
		if (s->worldUrl == url) 
			return s->getProto(name);
	}
	// find also in MainScene (e.g. for createVrmlFromString() 17.04.97 
	GvScene* s = GetScene();
	if (s->worldUrl == url || !*url) 
			return s->getProto(name);

	return(NULL);
}

// add externproto to list of unloaded externprotos
void GView::RegisterPending(GvExternProto *proto)
{
	pendingExternProtos.add(proto);
}
// remove externproto to list of unloaded externprotos
void GView::UnRegisterPending(GvExternProto *proto)
{
	pendingExternProtos.remove(proto);
}
// empty pending externproto's
void GView::FlushPendingExternProtos()
{
	pendingExternProtos.deleteAll();
}


// empty proto scenes 
void GView::FlushProtoScenes()
{
	protoScenes.deleteAll();
}
// add scene to proto scenes
void GView::addProtoScene(GvScene *protoScene)
{
	TRACE("Register proto Scene : %s \n",(const char *) protoScene->worldUrl);

#ifdef _GV_MINIMAL
	protoScene->deleteSceneGraph(); // no need to store scene graph new 20.04
#endif

	protoScenes.add(protoScene);
}



// library managment

//GvSFNode 
//GvNodeHandle 
//GView::library; // a GvScene node

//GvBool GView::libraryLoaded=FALSE;

GvBool GView::LoadLibrary(const char *fileName) 
{

	GvScene *scene = new GvScene();
    scene->setBrowser(this);

    int ret =	ReadVrmlFile(fileName,GReporter::current,scene);
	library = scene;
	libraryLoaded = TRUE;

	if (ret <0) return(FALSE);
	return(TRUE);
}


GvScene *GView::GetLibrary()
{
	if (!libraryLoaded) {
		if (!LoadLibrary( _PROGRAM "Lib.wrl")) return NULL;
	}
	return(GvScene *) (GvNode*) library;
}

GvMFNode *GView::GetLibraryObjects() 
{
	GvScene *s = GetLibrary();
	if (!s) return(NULL);
	GvNode * o= s->children.find(GvName("Objects"));
	if (!o || !RTISA(o,Gv2Group)) return(NULL);
	return  & ((Gv2Group *) o)->children;
}
//GvMFNode *GView::GetLibraryMaterials();
//GvMFNode *GView::GetLibraryAppearances();


// notification  functions
void GView::OnNodeAdded(GvNode *node)
{
	if (RTISA(node,GvTimeSensor)
			|| RTISA(node,GvLight)
			|| RTISA(node,GvViewpoint)
			|| RTISA(node,GvScript)
	) {
		doUpdateSceneInfo = 1;
	}
}

void GView::OnNodeDeleted(GvNode *node)
{
	if (RTISA(node,GvTimeSensor)
			|| RTISA(node,GvLight)
			|| RTISA(node,GvViewpoint)
	) {
		doUpdateSceneInfo = 1;
	}
}
void GView::OnNodeChanged(GvNode *node)
{
}


//
// script browser  methods 
//

const char * GView::getName() 
{
	return name;
}

const char* GView::getVersion() 
{
	return version;
}

float GView::getCurrentSpeed() 
{
	return viewStepSpeed;
}

float GView::getCurrentFrameRate() 
{
	if (lastFrameTime == 0) return 0.0;

	return 1000.0f / (float) lastFrameTime; 
}

const char *GView::getWorldURL() 
{
	return worldUrl;
}

void GView::replaceWorld(GvMFNode *nodes) 
{


	GvNodeHandle oldScene = GetScene(); // keep scene alive some where 
	GvScene *current = GetScene();
	
	GvScene *scene = new GvScene();
    scene->setBrowser(this);

	scene->ref();

	if (current) {
		scene->worldUrl.set(current->worldUrl.get());
		scene->baseUrl.set(current->baseUrl.get());
	}	


	scene->children.set(*nodes);
	
	hasNewScene = TRUE;
	newScene.set(scene);

	// delayed 		
	// SetScene(scene,TRUE);

	scene->unref();
}

int PrepareNodes(GView *view, GvScene *node)
{
	// for resolving relative urls 
	GLoadInlinesTraversal traversal;
	traversal.view = view;			
	traversal.resolveRelativeUrls = gtrue;	  

			traversal.traversalOrder = view->traversalInlineLoadingOrder;

			traversal.doViewCulling= FALSE;
			traversal.loadProtos = GTraversal::LOAD_ALL;
			traversal.loadInlines = GTraversal::DONT_LOAD;
			traversal.loadSounds = GTraversal::DONT_LOAD;
			traversal.loadTextures = GTraversal::DONT_LOAD;
			traversal.loadMovies = GTraversal::DONT_LOAD;
  
			traversal.maxInlineLevel = view->maxInlineDepth;
			traversal.InitializeAttributes();

			traversal.SetCamera(view->globalCamera);
			traversal.SetPhysicalCamera(view->GetPhysicalCamera());

			// do we need matrix etc ?

			traversal.traverseAll = 1;
			traversal.traverseTransformHierarchyOnly=1;
		
		int ret = node->Traverse(traversal);
		return ret;
}



GvMFNode* GView::createVrmlFromString(const char* vrmlSyntax,GvScript *scriptNode)
{
	GvScene * scene =  GetScene();
	if (!scene) return NULL;
    
	const char *baseUrl = GetHomeUrl();
    if (scriptNode) baseUrl = scriptNode->GetBaseUrl();
	ASSERT(baseUrl != NULL);

	//scene->homeUrl.set_(baseUrl); // not exactly correct but ....

	// create a new scene from string 
	GvScene* newScene = scene->createVrmlFromString((char *)vrmlSyntax,GetReporter());

	if (!newScene) return NULL;
	newScene->ref();
	newScene->baseUrl.set_(baseUrl);

	PrepareNodes(this,newScene);


	GvMFNode *ret = new GvMFNode(0);
	ret->set(newScene->children);
	
	newScene->unref();

	return ret;

}

void GView::createVrmlFromURL(GvMFString *url, GvNode *node, const char *event,GvScript *scriptNode)
{
//	CWaitCursor wait;


	GvScene * scene =  GetScene();
	if (!scene || url == NULL) return;

	// setup createVrmlFromUrl pseudo node 
	Gv2CreateVrmlFromUrl *vrmlFromUrl = new Gv2CreateVrmlFromUrl();
	vrmlFromUrl->ref();
	vrmlFromUrl->setBrowser(this);

	vrmlFromUrl->url.set(*url);

	const char *baseUrl = GetHomeUrl();
    if (scriptNode) baseUrl = scriptNode->GetBaseUrl();
	vrmlFromUrl->baseUrl.set_(baseUrl);

	if (node) { // set node and get eventIn Index 
		vrmlFromUrl->node.set(node);
		vrmlFromUrl->nodeEvent.set(node->getFieldData()->getEventInIndex(node,event));
	}

	// to do : get base Url e.g. from File where scripts resides

	// for resolving relative urls 
	GLoadInlinesTraversal traversal;
	traversal.view = this;
	traversal.resolveRelativeUrls = gtrue;
	
	// process load request 
	vrmlFromUrl->Load(traversal);
	
	// get status 
	GFileState status = vrmlFromUrl->loader.GetState();

	switch (status) {
	case S_NOT_LOADED :	 // to many threads, wait 
	case S_LOADING :
	case S_PROCESSING :
		 // will be processed when URL ready 
		TRACE("CreateVrmlFromUrl: %s started %d \n",(const char *) url->get1(0),status);
		scene->addCreateVrmlFromUrl(vrmlFromUrl); 

		break; 
	// case S_ALL_BAD :
		// all URL's not found or bad 

	default : 		
		// data loaded or error, send event 
		TRACE("CreateVrmlFromUrl: %s done %d \n",(const char *) url->get1(0),status);
		// status == S_PROCESSED if all went well, anyway send NULL children event even if error
	
	    traversal.traversalOrder = traversalInlineLoadingOrder;
	    traversal.doViewCulling= FALSE;
	    traversal.loadProtos = GTraversal::LOAD_ALL;
	    traversal.loadInlines = GTraversal::DONT_LOAD;
	    traversal.loadSounds = GTraversal::DONT_LOAD;
  
	    traversal.maxInlineLevel = maxInlineDepth;
	    traversal.InitializeAttributes();

	    traversal.SetCamera(globalCamera);
	    traversal.SetPhysicalCamera(GetPhysicalCamera());

	    traversal.traverseAll = 1;
	    traversal.traverseTransformHierarchyOnly=1;

	    vrmlFromUrl->Traverse(traversal);

		vrmlFromUrl->SendEvent();

		UpdateSceneInfo(); // probably sensor stuff added 
		break;

	}

	vrmlFromUrl->unref();
	vrmlFromUrl = NULL;

}
#if 0
void GView::CheckCreateVrmlFromURL(Gv2CreateVrmlFromUrl *src)
{
		// for resolving relative urls 
		GLoadInlinesTraversal traversal;
		traversal.view = view;

		// check & process load request 
		src->Load(traversal);

		// get current status 
			GFileState status = src->loader.GetState();

			switch (status) {
			case S_NOT_LOADED :	 // to many threads, wait 
			case S_LOADING :
			case S_PROCESSING :
				break; // continue to work on this 

			default : 		
				{ // no longer loading 
				TRACE("OnReadFileCompleted :: CreateVrmlFromUrl: %s done %d \n",src->GetUrl(),status);
				// instance PROTO's 
				traversal.loadProtos = GTraversal::LOAD_ALL;
				traversal.loadInlines = GTraversal::DONT_LOAD;
				traversal.loadSounds = GTraversal::DONT_LOAD;
  
				traversal.maxInlineLevel = maxInlineDepth;
				traversal.InitializeAttributes();

				traversal.SetCamera(globalCamera);
				traversal.SetPhysicalCamera(GetPhysicalCamera());

				traversal.traverseAll = 1;
				traversal.traverseTransformHierarchyOnly=1;

			    src->Traverse(traversal);

				// send the event
				src->SendEvent();
				return TRUE;
				

}

#endif

void GView::addRoute(GvNode *fromNode, const char *fromEventOut, GvNode *toNode, const char *toEventIn)
{
	GvScene * scene =  GetScene();
	if (!scene) return;

	if (!fromNode || !toNode) return; //ERROR

	// setup GvRoute pseudo node 
	GvRoute *route = new GvRoute();

	route->ref();

	if (route->set(fromNode,fromEventOut,toNode,toEventIn,TRUE)) {
		if (!scene->addRoute(route,0)) { 
			// error 
		}
	}
	
	route->unref();

}

void GView::deleteRoute(GvNode *fromNode, const char *fromEventOut, GvNode *toNode, const char *toEventIn)
{
	GvScene * scene =  GetScene();
	if (!scene) return;

	if (!fromNode || !toNode) return; //ERROR

	// setup GvRoute pseudo node 
	GvRoute *route = new GvRoute();

	route->ref();

	if (route->set(fromNode,fromEventOut,toNode,toEventIn,TRUE)) {
		GvRoute *theRoute = scene->findRoute(route); // find route by value 
		if (theRoute && scene->deleteRoute(theRoute)) { 
			// ok
		}
		else {
			// error 
			if (!theRoute) { 
				// route wasn't part of this scene
				// assume route is part of other scene or proto instance 
				GvNode::deleteRoute(route->from,route->fromEventIndex,route->to,route->toEventIndex);
			}

		}
	}
	
	route->unref();

}

void GView::loadURL(GvMFString *url, GvMFString *parameter,GvScript *scriptNode)
{

   if (!url || url->getNum() == 0) return;
   
   BOOL startNow= FALSE;
   const char *target = NULL;
   const char *location = NULL;

   const char *baseUrl = GetHomeUrl();
   if (scriptNode) baseUrl = scriptNode->GetBaseUrl();

    // check for FRAME = argument
	if (parameter && parameter->Length()>0) {
				const char *targetPrefix = "target=";
				const int targetPrefixLen = strlen(targetPrefix);

				int i;
				for (i=0;i< parameter->Length(); i++) {
					// check for target option 
					const char *opt = parameter->get1(i).getString();
					if (strncmp(opt, targetPrefix,targetPrefixLen) == 0) {
						target = &opt[targetPrefixLen];
						startNow = TRUE;
					}
				}
			}

   const char* url0 = url->get1(0).getString();	// url to execute

   if (!strncmp(url0, "OnEvent", 7)) // call vbscript/javascript
		startNow = TRUE;

   if (startNow && !hasNewUrl) {

	   if (scriptNode) scriptNode->ref(); // timesenor chain might still be active 
	#ifdef _GLVIEW_IE

		if (viewController) viewController->loadURLfromFields("loadUrl",url,parameter,baseUrl);
	#else 
		// to do:  pView->ReadUrl(url->get1(0));
		// to do: it is probably frame replacement stuff
		// ignore because recursive entry to ReadModel ==> crash 
	#endif
	   if (scriptNode) scriptNode->unref();
   
   } else {		   
		// save request until event cascade is done
	    hasNewUrl = TRUE;
		newUrl.set(*url);
		newUrlBase= baseUrl;
		if (parameter) newUrlParams.set(*parameter);
		else newUrlParams.setLength(0);
   }

}

// set the viewpoint depending on mode

int GView::setViewpointByValue(Point position,Rotation orientation,int mode)
{
#ifdef _GLVIEW_IE
	// viewController
	pView->setViewpointByValue_(
				position.x,position.y,position.z,
				orientation.axis[0],orientation.axis[1],orientation.axis[2],orientation.angle,mode);

	return 1;
#else

	return 0;
#endif
}


int GView::getViewpointByValue(Point &position,Rotation &orientation, int mode)
{
	Vector axis;
	float angle;
	
	GCamera *camera = &GetCamera();

	if (mode == 1)
		camera = &GetGlobalCamera();
	else if (mode == 2)
		camera = GetPhysicalCamera();

	ConvertCameraToOrientation(camera->position, camera->target,camera->up,axis, angle);

	position = camera->position;
	orientation.set(axis.x,axis.y,axis.z,angle);

	return 1;
}


//////////////////////////////////////////////
//											//
//		navigation-panel stuff				//
//											//
//////////////////////////////////////////////

// is panel ok, (visible, not to small */ 
gbool GView::PanelOk() const
{
#ifdef G_PANEL
	if (!panel) return gfalse;
	return panel->Ok();
#endif
	return gfalse;
}

void GView::NavigationPanelCreate(int flush)
{
#ifdef G_PANEL
	m_navPanelOn = TRUE;
	ASSERT(panel == NULL);

#ifdef WIN32
	HRESULT res;

	panel = new GPanel(pView);
	
	CRect rect;

	pView->GetClientRect(&rect);
	panel->SetScreenRect(rect.Width(), rect.Height());

#if _D3D
	res = panel->Init(device); 
#endif //_D3D


#ifdef _OGL
	res = panel->Init();
#endif

	if (res != S_OK) {
		TRACE("Panel init error %x \n",res);
	}
	if (flush)
		Redraw();
#else
#pragma message( "Panel: TO DO")
#endif

#endif
}

void GView::NavigationPanelDestroy()
{
#ifdef G_PANEL
	if (panel) { 
		delete panel;//calls panel::term()
		panel = NULL;
		Redraw();
	}
#endif
}

void GView::setNavigationPanel(gbool mode)
{
#ifdef G_PANEL
	if (mode) 
		NavigationPanelCreate(TRUE);

	else
		NavigationPanelDestroy();
	
#endif
}

gbool GView::getNavigationPanel()
{
#ifdef G_PANEL
	return panel != NULL;
#else
    return gfalse;
#endif
}

void GView::setNavigationPanelProp(gbool mode)
{
#ifdef G_PANEL
    m_navPanelOn_Prop = mode;
	m_navPanelOn = m_navPanelOn_Prop & m_navPanelOn_NavInf;

	TRACE("GView::setNavigationPanelProp: m_navPanelOn = m_navPanelOn_Prop & m_navPanelOn_NavInf %i %i %i\n", 
    m_navPanelOn, m_navPanelOn_Prop, m_navPanelOn_NavInf);
#endif
}

void GView::setNavigationPanelNavInf(gbool mode)
{
#ifdef G_PANEL
    m_navPanelOn_NavInf = mode;

	//ON only if user wants it, i.e. flag in settings and current status
    m_navPanelOn = m_navPanelOn_NavInf && m_navPanelOn_Prop;
	TRACE("GView::setNavigationPanelNavInf: m_navPanelOn = m_navPanelOn_NavInf: %i %i \n", m_navPanelOn, m_navPanelOn_NavInf);
#endif
}

////////////////////////////////////////////////////////////////////////



gbool GView::CheckForSceneChange()
{
	gbool ret = FALSE;

	if (hasNewScene) {
		hasNewScene = FALSE;
		SetScene((GvScene*) newScene.get(),TRUE);
		newScene.set((GvNode*) NULL);
		ret = TRUE;
	}

	if (hasNewUrl) {
 	    hasNewUrl = FALSE;
		ret = TRUE;

//#ifdef _GLVIEW_IE
		if (viewController) 
			viewController->loadURLfromFields(NULL,&newUrl,&newUrlParams,GetHomeUrl());
//#else 
//		pView->ReadUrl(newUrl.get1(0));
//#endif
	}
	return ret;
}


void GView::setDescription(const char *description)
{
  //TRACE("Description %s refCnt = %d \n",description,this->refCnt);
  Message(description);
}

void GView::notifyWhenLoaded(GvMFNode *nodesToLoad, GvNode *nodeToNotify, const char *eventInToNotify)
{
}


//
// Sound stuff
//


// initialize the sound device
gbool GView::InitializeSound()
{

	if (!hasSoundDevice) 
		return FALSE; // no sound device 

#ifdef _MFC

	CWinApp *a = AfxGetApp();
	HWND hWndFrame =  hWnd;

	if (a  && (a->m_pMainWnd)) hWndFrame = (a->m_pMainWnd)->GetSafeHwnd();
	else 
		hWndFrame = AfxGetMainWnd()->GetSafeHwnd();
	if (hWndFrame == NULL) 
		hWndFrame = hWnd;
#endif



	if (soundDevice) {
		delete soundDevice;
		soundDevice = NULL;
	}

	//if (m_playSoundStarted) 
#ifdef WIN32	
	// Purge always, CCpro may have started a sound 
	{
		::PlaySound(NULL,NULL,SND_PURGE  );
		m_playSoundStarted = FALSE;
	}
#endif

#ifdef G_RSX
	// user want rsx
	if (useRSXSound) {
		soundDevice = new GRSXSoundDevice();
		if (!soundDevice) return FALSE;
		soundDevice->SetQuality(soundQuality);
		if (soundDevice->Initialize(hWndFrame)) {
			//soundDevice->setCPUBudget(cpuBudget);
			soundDevice->SetViewpoint(globalCamera->Position(),globalCamera->NormTargetDirection(),globalCamera->Up(),timeGetTime());
			soundDevice->SetNumActiveSounds(maxNumSounds);
			TRACE("RSX Sound initialized \n");
			hasRSXSound = TRUE;
			return TRUE;
		}
		TRACE("RSX Sound initialize failed \n");
		delete soundDevice;
		soundDevice = NULL;
	}
#endif G_RSX

	hasRSXSound = FALSE;

#ifdef G_DSOUND
	soundDevice = new GDSSoundDevice();
	soundDevice->SetQuality(soundQuality);

	if (soundDevice->Initialize(hWndFrame)) {
		soundDevice->SetViewpoint(globalCamera->Position(),globalCamera->NormTargetDirection(),globalCamera->Up(),timeGetTime());
		soundDevice->SetNumActiveSounds(maxNumSounds);
		return TRUE;
	}
	else {
		delete soundDevice;
		soundDevice = NULL;
		hasSoundDevice = FALSE; // assume no sound device there 

		//useSound = FALSE; // turn of sounds, no sound device 
		TRACE("Initialization of Sound device failed \n");
		
		if (GetReporter()->verbose) 
			GetReporter()->Warning("Initialization of Sound device failed");

		return FALSE;
	}
#endif 

	hasSoundDevice = FALSE; // assume no sound device there 
	TRACE("No Sound support \n");
	return FALSE;
}

// get sound device, initialize if needed 
// if NULL cannot initialize sound device 
GSoundDevice *GView::GetSoundDevice()
{
	if (soundDevice == NULL) {
		if (!useSound) return NULL; // we shouldn't use sound 
		InitializeSound();
	}
	return soundDevice;
}


// release the sound device and associated resources
void GView::ReleaseSound()
{
	if (soundDevice) {
		soundDevice->Term();

		delete soundDevice;
		soundDevice = NULL;
	}
}

// get sound mode flag 
gbool GView::GetUseSound()
{
	return useSound; //  && soundDevice != NULL;
}

gbool GView::HasSoundDevice()
{
	return hasSoundDevice;
}

// set sound mode flag 
gbool GView::SetUseSound(gbool flag)
{
	if (!flag && soundDevice != NULL) {
		ReleaseSound();
	}
	useSound = flag;

	if (flag) {
		// would need to call Term() on each Sound node to get a reload
		// we need to start play now ?? 
		soundLoadMode=GTraversal::LOAD_IF_NEEDED;
		Redraw();
	}
	else soundLoadMode=GTraversal::DONT_LOAD;

	return useSound;
}


gbool GView::PlaySoundFile(const char *fileName)
{
	if (soundDevice) { // use the sound device, to mix in sound
		  // the current scene has sounds playing
		// ignore CCpro sounds for security reasons
		// return FALSE;
		return !!soundDevice->PlaySound(fileName); // back enabled

	} else {
#ifdef WIN32
		// win mm function
	    if (m_playSoundStarted)::PlaySound(NULL,NULL, SND_PURGE);
		
		m_playSoundStarted = TRUE;
		/* , SND_FILENAME | SND_ASYNC  */
		BOOL ret = ::PlaySound(fileName,AfxGetInstanceHandle(),SND_ASYNC|SND_FILENAME /* |SND_NOWAIT */ );
		return !!ret;
#else
		return gfalse;
#endif

	}
}


#include "gcaps.h"
// query for capabilities
int GView::GetCap(int cap)
{

	switch (cap) {
#ifdef WIN32
	case GC_FREE_PHYSICAL_MEMORY : {
				MEMORYSTATUS stat;
				::GlobalMemoryStatus (&stat);
				return (stat.dwAvailPhys / 1024);
			}
	case GC_TOTAL_PHYSICAL_MEMORY : {
				MEMORYSTATUS stat;
				::GlobalMemoryStatus (&stat);
				return (stat.dwTotalPhys / 1024);
			}
	case GC_USED_PHYSICAL_MEMORY : {
				MEMORYSTATUS stat;
				::GlobalMemoryStatus (&stat);
				return (stat.dwMemoryLoad);
			}
#endif

#ifdef _OGL
	case GC_OPENGL : return 1;

	case GC_RGB_LIGHTING : 
	case GC_ALPHA_BLENDING :
	case GC_MIPMAPPING : 
				return 1;

	case GC_MAX_LIGHTS : return renderState.maxLights;

	case GC_MAX_TEXTURESIZE_X : return renderState.textureParameters.max_size_x;
	case GC_MAX_TEXTURESIZE_Y : return renderState.textureParameters.max_size_y;
	case GC_CURRENT_MAX_TEXTURESIZE_X : return renderState.textureParameters.current_max_size_x;
	case GC_CURRENT_MAX_TEXTURESIZE_Y : return renderState.textureParameters.current_max_size_y;
	case GC_TEXTURE_SQUARE : return renderState.textureParameters.square_size;
	case GC_IS_HARDWARE : 
	case GC_TOTAL_VIDEO_MEMORY :
	case GC_FREE_VIDEO_MEMORY :
				return -1;

#endif

#ifdef _D3D
	case GC_DIRECT3D : return 1;
	case GC_RGB_LIGHTING : return renderState.canDoRgbLighting;
	case GC_ALPHA_BLENDING : return renderState.canDoAlphaBlend;
	case GC_MIPMAPPING : return renderState.canDoMipMap;
	case GC_MAX_LIGHTS : return renderState.maxLights;
	case GC_MAX_TEXTURESIZE_X : return renderState.textureParameters.max_size_x;
	case GC_MAX_TEXTURESIZE_Y : return renderState.textureParameters.max_size_y;
	case GC_CURRENT_MAX_TEXTURESIZE_X : return renderState.textureParameters.current_max_size_x;
	case GC_CURRENT_MAX_TEXTURESIZE_Y : return renderState.textureParameters.current_max_size_y;
	case GC_TEXTURE_SQUARE : return renderState.textureParameters.square_size;
	case GC_IS_HARDWARE : 
	case GC_TOTAL_VIDEO_MEMORY :
	case GC_FREE_VIDEO_MEMORY :
	case GC_FREE_TEXTURE_MEMORY :
	case GC_TOTAL_TEXTURE_MEMORY :
				if (device) return device->GetCap(cap);
				break;

#endif
	case GC_USING_TEXTURES : return useTextures;
	case GC_USING_MOVIES : return useMovies;
	case GC_USING_SOUND : return useSound;
	case GC_HAS_SOUND : return hasSoundDevice;
	case GC_USING_BACKGROUND : return useBackground;


	default : return 0;
	}

	return 0;
}

gbool GView::SetOption(const char *option,const char *value)
{
	if (!option) return gfalse;
	if (!value) return gfalse;

	gbool f=parseBool(value);

	switch(option[0]) {
	case 'd' : if (streq(option,"dithering")) return SetDithering(f);
			   break;	
	case 'I' :

#ifdef _KATMAI	
			if (streq(option,"ISSE")) {
					KatmaiEnable(f); 
					Redraw();
					return gtrue;
			}

#endif
	case 'N' : if (streq(option,"NurbsTesselationScale")) {
				   float f=0.0;
				   if (sscanf(value,"%g",&f) == 1 && f>0.0f) {
					    nurbsTessellationScale =f;
						Redraw();
						return gtrue;
				   }
				}
				else if (streq(option,"NurbsTargetFps")) {
				   float f=0.0;
				   if (sscanf(value,"%g",&f) == 1 && f>0.0f) {
					    SetTargetFrameRate(f);
						Redraw();
						return gtrue;
				   }
				}			   
				else if (streq(option,"NurbsTessellationMode")) {
				   int f;
				   if (sscanf(value,"%d",&f) == 1 && f>=0) {
					    nurbsTessellationMode = f;
						Redraw();
						return gtrue;
				   }
				}			   
				break;	

	default : break;
	}
	return gfalse;
}

static gbool SetBool(GvString &value,gbool b) 
{
   if (b) value = "TRUE";
   else value = "FALSE";
   return b;
}

gbool GView::GetOption(const char *option,GvString &value)
{

	switch(option[0]) {
	case 'd' : if (streq(option,"dithering")) return SetBool(value,GetDithering());
			   break; 	
	case 'I' :
#ifdef _KATMAI	
			if (streq(option,"ISSE")) {return SetBool(value,useKatmai);
			}
#endif
	default : break;
	}
	return gfalse;
}

void GView::SetNavigationMode(const char* mode)
{
    if (viewController) viewController->SetCameraMode(mode);
}

void GView::GetNavigationMode(CString &mode)
{
    if (viewController) 
		mode= viewController->GetCameraModeString();
}


#ifdef _COM
// Browser Interface methods

STDMETHODIMP GView::getName(BSTR *value)
{
	USES_CONVERSION;
	const char *v = getName(); 
	*value = SysAllocString(T2COLE(v));
	return S_OK;
}

STDMETHODIMP GView::getVersion(BSTR *value)
{
	USES_CONVERSION;
	const char *v = getVersion(); 
	*value = SysAllocString(T2COLE(v));
	return S_OK;
}

STDMETHODIMP GView::getCurrentSpeed(float *value)
{
	*value = getCurrentSpeed();
	return S_OK;
}

STDMETHODIMP GView::getCurrentFrameRate(float *value)
{
	*value = getCurrentFrameRate();
	return S_OK;
}


STDMETHODIMP GView::getWorldURL(BSTR *value)
{
	CString strResult;

	strResult = GetUrl();

	*value = strResult.AllocSysString();

	return S_OK;
}


STDMETHODIMP GView::replaceWorld(Node *value)
{
	if (value == NULL) return S_OK; // ignore

	GvNode *root = NULL;
	value->getNative( (long *)  &root);

	if (root == NULL) return S_OK; // ignore


	GvNodeHandle oldScene = GetScene(); // keep scene alive some where 
	GvScene *current = GetScene();
	GvScene *scene = NULL;

	if (RTISA(root,GvScene)) {
		GvScene *s = (GvScene *)root;
		if (current && (s->worldUrl.get().getLength() == 0)) {
			s->worldUrl.set(current->worldUrl.get());
			s->baseUrl.set(current->baseUrl.get());
		}	

		scene = s;

	} else {

		GvScene *s = new GvScene();
		s->setBrowser(this);

		s->addChild(root);
		if (current) {
			s->worldUrl.set(current->worldUrl.get());
			s->baseUrl.set(current->baseUrl.get());
		}	


		scene = s;
	}

	// delayed url
	hasNewScene = TRUE;
	newScene.set(scene);

	return S_OK;
}


STDMETHODIMP GView::loadURL(int cntUrl,
    /* [size_is][in] */ BSTR *url,
    int cntParameter,
    /* [size_is][in] */ BSTR *parameter)
{
	return S_OK;
}


STDMETHODIMP GView::setDescription(BSTR description)
{
	return S_OK;
}


STDMETHODIMP GView::createVrmlFromString(BSTR vrmlSyntax,Node **value)
{
	return S_OK;
}


STDMETHODIMP GView::createVrmlFromURL(BSTR url, Node *node, BSTR event)
{
	return S_OK;
}


STDMETHODIMP GView::getNode(BSTR name,Node **value)
{
	return S_OK;
}


STDMETHODIMP GView::addRoute(Node *fromNode, BSTR fromEventOut, Node *toNode, BSTR toEventIn)
{
	return S_OK;
}


STDMETHODIMP GView::deleteRoute(Node *fromNode,BSTR fromEventOut,Node *toNode,BSTR toEventIn)
{
	return S_OK;
}


STDMETHODIMP GView::createNode(BSTR nodeclass,Node **value)
{
	return S_OK;
}


STDMETHODIMP GView::getWorld(Node **value)
{
	return S_OK;
}


STDMETHODIMP GView::beginUpdate(void)
{
	return S_OK;
}


STDMETHODIMP GView::endUpdate(void)
{
	return S_OK;
}


STDMETHODIMP GView::addNode(Node *node)
{
	return S_OK;
}


STDMETHODIMP GView::removeNode(Node *node,VARIANT_BOOL *value)
{
	return S_OK;
}



STDMETHODIMP GView::setNodeName(Node *node, BSTR name)
{
	return S_OK;
}


STDMETHODIMP GView::getNodeName(Node *node, BSTR *name)
{
	return S_OK;
}


STDMETHODIMP GView::getEventOut(BSTR eventOutName, EventOut **value)
{
	return GvNode::getEventOut(eventOutName,value);
}


STDMETHODIMP GView::getEventIn(BSTR eventInName, EventIn **value)
{
	return GvNode::getEventIn(eventInName,value);
}


STDMETHODIMP GView::getControlPathname(BSTR *directoryName)
{
	return S_OK;
}


STDMETHODIMP GView::saveWorld(BSTR fileName, VARIANT_BOOL *value)
{
	return S_OK;
}


STDMETHODIMP GView::saveViewpoint( BSTR viewpointName)
{
	return S_OK;
}


STDMETHODIMP GView::setObserver(IUnknown *observerUnk,int flags,VARIANT_BOOL *value)
{
	CComQIPtr<BrowserObserver,&IID_BrowserObserver> tmp(observerUnk);

	if (tmp) {
		observer = tmp;
		observerFlags = flags |  (observerFlags & ~GOBSERVER_OBSERVE_MASK);
		reportErrors = FALSE; // tmp
		*value = TRUE;
	}
	else {
		observer.Release();
		observerFlags = 0 |  (observerFlags & ~GOBSERVER_OBSERVE_MASK);
		*value = (observerUnk == NULL);
	}
	return S_OK;
}


STDMETHODIMP GView::setViewpointByValue(float *position, float *orientation,float fov, VARIANT_BOOL animate)
{
	return S_OK;
}


STDMETHODIMP GView::setFullscreen(VARIANT_BOOL mode,VARIANT_BOOL *value)
{
	SetFullScreen(mode!=0);
	*value = GetFullScreen();
	return S_OK;
}


STDMETHODIMP GView::setCollisionDetection(VARIANT_BOOL mode)
{
	collisionDetection = mode!=0;
	return S_OK;
}


STDMETHODIMP GView::setRenderMode(BSTR modeStr)
{
	USES_CONVERSION;
	CString tmp;
	tmp = OLE2CT(modeStr);
	tmp.MakeLower();
    GRenderMode mode = (GRenderMode) GRenderModeLookup(tmp);
	SetRenderMode(mode);
	setRedraw();
	return S_OK;
}


STDMETHODIMP GView::setNavigationMode(BSTR mode)
{
	USES_CONVERSION;
	CString tmp;

	tmp = OLE2CT(mode);
    tmp.MakeLower();
    if (pView) pView->SetCameraMode(tmp);
	return S_OK;
}


STDMETHODIMP GView::setViewpoint(BSTR viewpointName)
{
	USES_CONVERSION;
	const char * name = OLE2CT(viewpointName);
	SetCamera(name);
	return S_OK;
}


STDMETHODIMP GView::setHeadlight(VARIANT_BOOL mode)
{
	SetHeadLight (mode ? TRUE : FALSE);
	return S_OK;
}


STDMETHODIMP GView::setAvatarHeight(float value)
{
	heightOverGround = heightOverGroundLocal=value;
	return S_OK;
}


STDMETHODIMP GView::setCollisionDistance(float value)
{
	collisionDistance = collisionDistanceLocal = value;	
	return S_OK;
}


STDMETHODIMP GView::setVisibilityLimit(float value)
{
	if (value <= 0.0f ) return S_OK; // BAD 
	visibilityLimit = visibilityLimitLocal = value;
	camera->zfar = value;
	setRedraw();
	return S_OK;
}


STDMETHODIMP GView::setWalkSpeed(float value)
{
	if (value <0) return S_OK;
	viewStepSpeedLocal=value;
	UpdateGlobalNavigationValues();
	return S_OK;
}

STDMETHODIMP GView::setNavigationPanel(VARIANT_BOOL mode)
{
	setNavigationPanel(!!mode);
	return S_OK;
}
        
STDMETHODIMP GView::getNavigationPanel(VARIANT_BOOL *value)
{
	*value = getNavigationPanel();
	return S_OK;
}


#else


void GView::setHeadlight(gbool mode)
{
	SetHeadLight (mode ? TRUE : FALSE);
}


void GView::setAvatarHeight(float value)
{
	heightOverGround = heightOverGroundLocal=value;
}


void GView::setCollisionDistance(float value)
{
	collisionDistance = collisionDistanceLocal = value;	
}


void GView::setVisibilityLimit(float value)
{
	if (value <= 0.0f ) return; // BAD 
	visibilityLimit = visibilityLimitLocal = value;
	camera->zfar = value;
	setRedraw();
}


void GView::setWalkSpeed(float value)
{
	if (value <0) return;
	viewStepSpeedLocal=value;
	UpdateGlobalNavigationValues();
}


#endif


