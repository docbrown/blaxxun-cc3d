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

@module GPanel.cpp - |

Copyright (c) 1998	blaxxun interactive, Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:

Changes:

$Revision: 1.28 $
$Log: gpanel.cpp,v $
Revision 1.28  1999/07/06 16:54:43  tom
*** empty log message ***

Revision 1.15  1998/10/30 16:46:10  tom
8bit transparenz

Revision 1.14  1998/10/29 18:02:44  holger
empty message

Revision 1.13  1998/10/28 14:15:18  tom
*** empty log message ***

Revision 1.12  1998/10/22 16:32:54  holger
empty message

Revision 1.9  1998/10/06 10:30:44  tom
ogl fertig

Revision 1.6  1998/09/29 11:48:33  holger
empty message

Revision 1.5  1998/09/28 17:19:00  tom
*** empty log message ***

Revision 1.4  1998/09/22 16:37:34  holger
empty message

Revision 1.3  1998/09/21 16:50:26  tom
ogl-version

Revision 1.1  1998/08/20 17:22:09  holger
empty message



Todo :

******************************************************************************/

#include "stdafx.h"

#ifndef _GLVIEW_DLL
	#include "glviewhdr.h"
	#include "Mainfrm.h"
#else

#if !defined(Linux)

	#include "blaxxunCC3dCtl.h"
	#define CMyglView CGLViewCtrlCtrl


#endif
#endif

#include "../../blaxxuncc3d/resource.h"

#include "grender.h"
#include "gmodel.h"
#include "gpanel.h"
#include "dibio.h"


GPanel::GPanel(CMyglView *newpView) : 
	enabled(gtrue),
	screenToSmall(gfalse),
	hBitmap(NULL),
	hBitmap1(NULL)
{
    BITMAP bm;
	navigationMode = navigationDirection = 0;
	minimized = FALSE;
	isActive = FALSE;
	pView = newpView;

	ASSERT(pView != NULL);

	bmNumber = oldbmNumber = -1;
	int i;


#ifdef _D3D
	panelSurf = NULL;
	device = NULL;
	for(i=0; i<43; i++) 
		panelSurfSphere[i]=NULL;
#endif
#ifdef _OGL
	cache = NULL;
	for(i=0; i<43; i++) {
		m_OGLBitmapArray[i]=NULL; // hg
	}
#endif

	
	m_BitmapArray = (CBitmap**)malloc(43*sizeof(CBitmap*));
	for(i=0; i<43; i++) 
		m_BitmapArray[i]= new CBitmap;
	 m_BitmapArray[0]->LoadBitmap(IDB_SPHERE_DOWN_1);
	 m_BitmapArray[1]->LoadBitmap(IDB_SPHERE_OVER_1);


	 m_BitmapArray[2]->LoadBitmap(IDB_BITMAP10);
	 m_BitmapArray[3]->LoadBitmap(IDB_BITMAP2);

	 m_BitmapArray[4]->LoadBitmap(IDB_BITMAP11);
	 m_BitmapArray[5]->LoadBitmap(IDB_BITMAP3);

	 m_BitmapArray[6]->LoadBitmap(IDB_BITMAP12);
	 m_BitmapArray[7]->LoadBitmap(IDB_BITMAP4);

	 m_BitmapArray[8]->LoadBitmap(IDB_BITMAP13);
	 m_BitmapArray[9]->LoadBitmap(IDB_BITMAP5);

	 m_BitmapArray[10]->LoadBitmap(IDB_BITMAP14);
	 m_BitmapArray[11]->LoadBitmap(IDB_BITMAP6);

	 m_BitmapArray[12]->LoadBitmap(IDB_BITMAP15);
	 m_BitmapArray[13]->LoadBitmap(IDB_BITMAP7);

	 m_BitmapArray[14]->LoadBitmap(IDB_BITMAP16);
	 m_BitmapArray[15]->LoadBitmap(IDB_BITMAP8);

	 //whole panel
	 m_BitmapArray[16]->LoadBitmap(IDB_BITMAP32);
	 m_BitmapArray[16]->GetBitmap(&bm);
	 srcRect.SetRect(0,0,bm.bmWidth,bm.bmHeight);

	 m_BitmapArray[17]->LoadBitmap(IDB_EXAMINE);
	 m_BitmapArray[18]->LoadBitmap(IDB_SLIDE);
	 m_BitmapArray[19]->LoadBitmap(IDB_ROTATE);

	 m_BitmapArray[20]->LoadBitmap(IDB_FLY0);
	 m_BitmapArray[21]->LoadBitmap(IDB_FLY1);
	 m_BitmapArray[22]->LoadBitmap(IDB_FLY2);
	 m_BitmapArray[23]->LoadBitmap(IDB_FLY3);
	 m_BitmapArray[24]->LoadBitmap(IDB_FLY4);
	 m_BitmapArray[25]->LoadBitmap(IDB_FLY5);
	 m_BitmapArray[26]->LoadBitmap(IDB_FLY6);
	 m_BitmapArray[27]->LoadBitmap(IDB_FLY7);
	 m_BitmapArray[28]->LoadBitmap(IDB_FLY);
	 m_BitmapArray[0]->GetBitmap(&bm);
	 rectSphere.SetRect(88, 0, 88+bm.bmWidth, bm.bmHeight);
	 //the position is set in Init() and setScreenRect()

	//viewpoint's
	 m_BitmapArray[29]->LoadBitmap(IDB_VI1_D);
	 m_BitmapArray[30]->LoadBitmap(IDB_VI1_F);
	 m_BitmapArray[30]->GetBitmap(&bm);	 
	 rectViewP.SetRect(10, 16, 10+bm.bmWidth, 16+bm.bmHeight);
	 
	 m_BitmapArray[31]->LoadBitmap(IDB_VI2_D);
	 m_BitmapArray[32]->LoadBitmap(IDB_VI2_F);
	 m_BitmapArray[32]->GetBitmap(&bm);
	 rectViewR.SetRect(27, 16, 27+bm.bmWidth, 16+bm.bmHeight);
	 
	 m_BitmapArray[33]->LoadBitmap(IDB_VI3_D);
	 m_BitmapArray[34]->LoadBitmap(IDB_VI3_F);
     m_BitmapArray[34]->GetBitmap(&bm);
	 rectViewN.SetRect(42, 16, 42+bm.bmWidth, 16+bm.bmHeight);

	 //close-button
	 m_BitmapArray[35]->LoadBitmap(IDB_CLOSE_D);
	 m_BitmapArray[36]->LoadBitmap(IDB_CLOSE_F);
     m_BitmapArray[36]->GetBitmap(&bm);
	 rectClose.SetRect(63, 29, 63+bm.bmWidth, 29+bm.bmHeight);

	 //slide
	 m_BitmapArray[37]->LoadBitmap(IDB_SLIDE1_D);
	 m_BitmapArray[38]->LoadBitmap(IDB_SLIDE1_F);
     m_BitmapArray[38]->GetBitmap(&bm);
	 rectSlideL.SetRect(158, 12, 158+bm.bmWidth, 12+bm.bmHeight);

	 
	 m_BitmapArray[39]->LoadBitmap(IDB_SLIDE2_D);
	 m_BitmapArray[40]->LoadBitmap(IDB_SLIDE2_F);
     m_BitmapArray[40]->GetBitmap(&bm);
	 rectSlideR.SetRect(188, 12, 188+bm.bmWidth, 12+bm.bmHeight);

	 //help
	 m_BitmapArray[41]->LoadBitmap(IDB_HELP_D);
	 m_BitmapArray[42]->LoadBitmap(IDB_HELP_F);
     m_BitmapArray[42]->GetBitmap(&bm);
	 rectHelp.SetRect(226, 15, 226+bm.bmWidth, 15+bm.bmHeight);
}



GPanel::~GPanel() 
{
	Term();
	int i;

	for(i=0; i<43; i++) 
		delete m_BitmapArray[i];//delete the CBitmaps

	if (hBitmap) 
		DeleteObject( hBitmap );
	hBitmap = NULL;
}



void GPanel::SetScreenRect(int width,int height)
{
	//sets  screenRect  destRect  screenToSmall
	
	screenRect.SetRect(0,0,width,height);

	//width and height of the panel
	int w = srcRect.Width();
	int h = srcRect.Height();

	//topLeft of complete panel
	int x = (screenRect.Width() +1) / 2 - w /2; // center
	int y = screenRect.bottom - h - GAP;

	destRect.SetRect(x,y, x+w,y+h);
	screenToSmall = (x<0) || (y <h );
}


void GPanel::setBitmap(int mode, int newDirection)
{
	//feedback
#ifdef _DEBUG_PANEL
	TRACE("GPanel::setBitmap...........................SETBITMAP MODE: %i\n", mode);
#endif _DEBUG_PANEL

	if (mode == -1)//no navigation action
		bmNumber=-1;	
	else if (mode == 0 || mode == 1 || mode == 5)
	{
		if (newDirection == 0) bmNumber = -1;
		else if ((newDirection & NAV_PANEL_UP) && (newDirection & NAV_PANEL_RIGHT)) bmNumber=2;
		else if ((newDirection & NAV_PANEL_RIGHT) && (newDirection & NAV_PANEL_DOWN)) bmNumber=6;
		else if ((newDirection & NAV_PANEL_DOWN) && (newDirection & NAV_PANEL_LEFT)) bmNumber=10;
		else if ((newDirection & NAV_PANEL_LEFT) && (newDirection & NAV_PANEL_UP)) bmNumber=14;
		else if (newDirection & NAV_PANEL_UP) bmNumber=0;
		else if (newDirection & NAV_PANEL_RIGHT) bmNumber=4;
		else if (newDirection & NAV_PANEL_DOWN) bmNumber=8;
		else if (newDirection & NAV_PANEL_LEFT) bmNumber=12;

		if (mode == 5) bmNumber = (bmNumber+2)/2+19;//fly
		if (newDirection == 0 && mode == 5) bmNumber = 28;
	}
	else if (mode == 2 || mode ==9 || mode == 7) bmNumber = 18;//slide
	else if (mode == 3 || mode == 6 || mode == 10) bmNumber = 19;//rot
	else if (mode == 4) bmNumber = 17;//examine
	else bmNumber =-1;

	//called by the navigation-module
	if (oldbmNumber != bmNumber) {
		oldbmNumber = bmNumber;
		UpdatePanel(0,0,0,0,0);//blit into the front-buffer immediatly
	}
}


void GPanel::UpdatePanel(int srcBitmap, int xstart,int ystart,int width,int height)
{
//
//the new picture is blitted directly into the front-buffer
#ifdef _OGL
	glDrawBuffer(GL_FRONT);
	Blt();	
	glDrawBuffer(GL_BACK);

#endif _OGL

#ifdef _D3D

	if (!device->lpFrontBuffer) return;

	//reset position on screen, necessary for d3d because onmove not called
#ifdef _GLVIEW_DLL
	pView->view->OnMove(0,0);
#endif

	//the destrect-coords must be adapted to the front-buffer coords
	CRect rect2;
	rect2 = destRect;
	destRect += device->pClientOnPrimary;
	Blt(device->lpFrontBuffer);
	destRect = rect2;

#endif _D3D
}


void GPanel::maximize()
{
	minimized = FALSE;
}

void GPanel::minimize()
{
	 minimized = TRUE;
}

void GPanel::enable()
{
	enabled = TRUE;
}

void GPanel::disable()
{
	 enabled = FALSE;
}

void GPanel::setNavigationMode(int mode)
{
	if (mode != navigationMode)
	{
		navigationDirection = 0;//reset
		navigationMode = mode;
		setBitmap(mode, navigationDirection);
		//TRACE("GPanel::setNavigationMode   mode %i\n", mode);
	}
}

void GPanel::setNavigationDirection(float tileX, float tileY)
{
	// tile -> directionFlag

	
	int directionFlag;
	directionFlag = 0;
	float delta;

	if (tileX == 0) {
		if (tileY > 0) directionFlag= NAV_PANEL_DOWN;
		else if (tileY < 0) directionFlag= NAV_PANEL_UP;

	} else {
	delta = (float)(-1*tileY/tileX);
	
	if (tileX == 0 && tileY == 0)	
		directionFlag = 0;
	else if (tileX>=0)
	{
		if (delta>=tan60) directionFlag= NAV_PANEL_UP;
		else if (delta<tan60 && delta>=tan30) directionFlag= NAV_PANEL_UP|NAV_PANEL_RIGHT;
		else if (delta<tan30 && delta>=tanMin30) directionFlag= NAV_PANEL_RIGHT;
		else if (delta<tanMin30 && delta>tanMin60) directionFlag= NAV_PANEL_RIGHT|NAV_PANEL_DOWN;
		else directionFlag= NAV_PANEL_DOWN;
	}
	else {
		if (delta>=tan60) directionFlag= NAV_PANEL_DOWN;
		else if (delta<tan60 && delta>=tan30) directionFlag= NAV_PANEL_DOWN|NAV_PANEL_LEFT;
		else if (delta<tan30 && delta>=tanMin30) directionFlag= NAV_PANEL_LEFT;
		else if (delta<tanMin30 && delta>tanMin60) directionFlag= NAV_PANEL_LEFT|NAV_PANEL_UP;
		else directionFlag= NAV_PANEL_UP;
	}
	}
#ifdef _DEBUG_PANEL
	TRACE("GPanel::setNavigationDirection   navigationDirection %i\n", navigationDirection);
	TRACE("GPanel::setNavigationDirection   directionFlag %i\n", directionFlag);
#endif _DEBUG_PANEL

		navigationDirection = directionFlag;
		setBitmap(navigationMode, navigationDirection);
}



////////////////////////////////////////////////////////////////////////////////7


#ifdef _D3D
static DWORD DDColorMatch(IDirectDrawSurface *pdds, COLORREF rgb)
{
    COLORREF rgbT;
    HDC hdc;
    DWORD dw = CLR_INVALID;
    DDSURFACEDESC ddsd;
    HRESULT hres;

    //
    //  use GDI SetPixel to color match for us
    //
    if (rgb != CLR_INVALID && pdds->GetDC(&hdc) == DD_OK)
    {
		rgbT = GetPixel(hdc, 0, 0);             // save current pixel value
		SetPixel(hdc, 0, 0, rgb);               // set our value
		pdds->ReleaseDC(hdc);
    }

    //
    // now lock the surface so we can read back the converted color
    //
    ddsd.dwSize = sizeof(ddsd);
    while ((hres = pdds->Lock(NULL, &ddsd, 0, NULL)) == DDERR_WASSTILLDRAWING);

    if (hres == DD_OK)
    {
		dw  = *(DWORD *)ddsd.lpSurface;                     // get DWORD
        if(ddsd.ddpfPixelFormat.dwRGBBitCount < 32)
            dw &= (1 << ddsd.ddpfPixelFormat.dwRGBBitCount)-1;  // mask it to bpp
		pdds->Unlock(NULL);
    }

    //
    //  now put the color that was there back.
    //
    if (rgb != CLR_INVALID && pdds->GetDC(&hdc) == DD_OK)
    {
		SetPixel(hdc, 0, 0, rgbT);
		pdds->ReleaseDC(hdc);
    }

    return dw;
}
//-----------------------------------------------------------------------------
// Name: LoadTextureImage()
// Desc: Loads a texture map file into a BITMAP surface.
//-----------------------------------------------------------------------------
static HRESULT LoadBmpImage(HBITMAP &hbmBitmap, LPCTSTR strName )
{
    LPCTSTR strFilename = strName;
    LPCTSTR strExtension;
    LPCTSTR  strPathname= strName;

    // Get the filename extension
    if( NULL == ( strExtension = _tcsrchr( strFilename, TEXT('.') ) ) )
        return S_FALSE; // DDERR_UNSUPPORTED;

    // Check the executable's resource. If it's there, we're done!
    if( NULL != ( hbmBitmap = (HBITMAP)LoadImage(
		                                    GetModuleHandle(NULL),
                                            strFilename, IMAGE_BITMAP,
                                            0, 0, LR_CREATEDIBSECTION) ) )
        return S_OK;


    if( !lstrcmpi( strExtension, ".bmp" ) )
    {
        // Try to load the bitmap as a resource.
        hbmBitmap = (HBITMAP)LoadImage( GetModuleHandle(NULL),
			                            strPathname, IMAGE_BITMAP, 0, 0, 
								        LR_CREATEDIBSECTION );

        // If the bitmap wasn't a resource, try it as a file.
        if( NULL == hbmBitmap )
            hbmBitmap = (HBITMAP)LoadImage( GetModuleHandle(NULL), strPathname,
			                            IMAGE_BITMAP, 0, 0, 
										LR_LOADFROMFILE|LR_CREATEDIBSECTION );

        return (hbmBitmap) ? S_OK : S_FALSE;
    }

	// Can check for other file formats here

    return S_FALSE;
}
#endif _D3D






#ifdef _D3D


//-----------------------------------------------------------------------------
// Name: CopyBitmapToSurface()
// Desc: Copies the image of a bitmap into a surface
//-----------------------------------------------------------------------------
static HRESULT CreateSurfaceFromBitmap(LPDIRECTDRAW pDD, LPDIRECTDRAWSURFACE pddsTarget,
                                    HBITMAP hbmBitmap, DWORD dwFlags, LPDIRECTDRAWSURFACE &result)
{
	HRESULT hr;

	if (hbmBitmap == NULL) {
		return S_FALSE;
	}

    // Get the bitmap structure (to extract width, height, and bpp)
    BITMAP bm;
    GetObject( hbmBitmap, sizeof(BITMAP), &bm );

    // Setup the new surface desc
    DDSURFACEDESC ddsd;

	memset(&ddsd, 0, sizeof(DDSURFACEDESC));
    ddsd.dwSize = sizeof(DDSURFACEDESC);


    if (FAILED( hr = pddsTarget->GetSurfaceDesc( &ddsd ))) 
		return hr;
	   

    ddsd.dwFlags          = DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH|DDSD_PIXELFORMAT;
    ddsd.ddsCaps.dwCaps   = /* DDSCAPS_TEXTURE|*/ DDSCAPS_SYSTEMMEMORY;
   
    ddsd.dwWidth          = bm.bmWidth;
    ddsd.dwHeight         = bm.bmHeight;

    // Create a new surface for the texture
    LPDIRECTDRAWSURFACE pddsTempSurface;

	//a temporary surface in the same format as the destination surface is created
    if (FAILED( hr = hr = pDD->CreateSurface( &ddsd, &pddsTempSurface, NULL )))
        return hr;
	   

    // Get a DC for the bitmap
    HDC hdcBitmap = CreateCompatibleDC( NULL );

    if( NULL == hdcBitmap )
    {
        pddsTempSurface->Release();
        return S_FALSE;
    }

    HGDIOBJ oldBitmap = SelectObject( hdcBitmap, hbmBitmap );

    // Handle palettized textures. Need to attach a palette
	/*
    if( ddsd.ddpfPixelFormat.dwRGBBitCount == 8 )
    {
        // Create the color table and parse in the palette
        DWORD pe[256];
        WORD  wNumColors = GetDIBColorTable( hdcBitmap, 0, 256,
                                             (RGBQUAD*)pe );
        for( WORD i=0; i<wNumColors; i++ )
        {
            pe[i] = 0xff000000 + RGB( GetBValue(pe[i]), GetGValue(pe[i]),
                                      GetRValue(pe[i]) );

			// Set alpha for transparent pixels
			if( dwFlags & D3DTEXTR_TRANSPARENTBLACK )
            {
                if( (pe[i]&0x00ffffff) == 0x00000000 )
                    pe[i] &= 0x00ffffff;
            }
            else if( dwFlags & D3DTEXTR_TRANSPARENTWHITE )
            {
                if( (pe[i]&0x00ffffff) == 0x00ffffff )
                    pe[i] &= 0x00ffffff;
            }
		}
        // Create & attach a palette with the bitmap's colors
        LPDIRECTDRAWPALETTE  pPalette;
		if( dwFlags & (D3DTEXTR_TRANSPARENTWHITE|D3DTEXTR_TRANSPARENTBLACK) )
            pDD->CreatePalette( DDPCAPS_8BIT|DDPCAPS_ALPHA, (PALETTEENTRY*)pe, &pPalette, NULL );
        else
            pDD->CreatePalette( DDPCAPS_8BIT, (PALETTEENTRY*)pe, &pPalette, NULL );
        pddsTempSurface->SetPalette( pPalette );
        pddsTarget->SetPalette( pPalette );
        RELEASE( pPalette );
		
    }
*/
    // Copy the bitmap image to the surface.
    HDC hdcSurface;
    if( SUCCEEDED( pddsTempSurface->GetDC( &hdcSurface ) ) )
    {
        BitBlt( hdcSurface, 0, 0, bm.bmWidth, bm.bmHeight, hdcBitmap, 0, 0,
                SRCCOPY );
	    pddsTempSurface->ReleaseDC( hdcSurface );

    }
    


	//set the color key
	DDCOLORKEY          ddck;
    ddck.dwColorSpaceLowValue  = DDColorMatch(pddsTempSurface /*pddsTarget*/ /* ,hdcSurface */,  RGB(0,0,255));//CLR_INVALID
    ddck.dwColorSpaceHighValue = ddck.dwColorSpaceLowValue;
    
	pddsTempSurface->SetColorKey(DDCKEY_SRCBLT, &ddck);


	SelectObject( hdcBitmap, oldBitmap ); // restore
    DeleteDC( hdcBitmap );
	
    result = pddsTempSurface;

    return S_OK;;
}





HRESULT GPanel::Blt(LPDIRECTDRAWSURFACE targetSurf)
{
	HRESULT hr;

	if (!panelSurf) return S_FALSE;
	if (!targetSurf) return S_FALSE;
	
	// Copy the panel to target
	// this call fails if rects on screen !!!!
	if (!Minimized()) {
		//hr=targetSurf->BltFast(destRect.left,destRect.top, panelSurf, NULL , DDBLTFAST_WAIT  | DDBLTFAST_SRCCOLORKEY );
		hr=targetSurf->Blt(&destRect, panelSurf, NULL , DDBLT_WAIT | DDBLT_KEYSRC, NULL );
		//
	} else //not enabeled
	    bmNumber = 35;//only the close button is shown

	
	CRect rect;

	switch (bmNumber) {
		case -1:
			return hr;
			break;
		case 29:
		case 30:
			rect = rectViewP;
			break;
		case 31:
		case 32:
			rect = rectViewR;
			break;
		case 33:
		case 34:
			rect = rectViewN;
			break;
		case 35:
		case 36:
			rect = rectClose;
			break;
		case 37:
		case 38:
			rect = rectSlideL;
			break;
		case 39:
		case 40:
			rect = rectSlideR;
			break;
		case 41:
		case 42:
			rect = rectHelp;
			break;

		default:
			rect = rectSphere;
			break;
	}
	rect += destRect.TopLeft();
	
	//hr=targetSurf->BltFast(rect.left,rect.top, panelSurfSphere[bmNumber], NULL , DDBLTFAST_WAIT  | DDBLTFAST_SRCCOLORKEY );

	hr=targetSurf->Blt( &rect, panelSurfSphere[bmNumber], NULL, DDBLT_WAIT | DDBLT_KEYSRC, NULL );

	return hr;
}


// initiallize the DD panel surface for BLT 
HRESULT GPanel::Init(GD3Device *device_)
{

	device = device_;

	HRESULT hr;

	ASSERT(device != NULL);
	ASSERT(panelSurf == NULL);

	LPDIRECTDRAW pDD = device->lpDD;

	if (!device->lpDD) return E_FAIL;
	if (!device->lpBackBuffer) return E_FAIL;

	hr =CreateSurfaceFromBitmap(device->lpDD, device->lpBackBuffer, HBITMAP(*m_BitmapArray[16]) ,D3DTEXTR_TRANSPARENTBLACK, panelSurf);

	if (FAILED(hr))
	{
		TRACE("Couldn't load panel main bitmap \n");
		enabled = gfalse;
		return hr;
	}

	//and now the smaller img
	int i;
	for(i=0; i<43; i++) 
	{
		hr =CreateSurfaceFromBitmap(device->lpDD, device->lpBackBuffer, HBITMAP(*m_BitmapArray[i]), D3DTEXTR_TRANSPARENTBLACK, panelSurfSphere[i]);
		if (FAILED(hr))
		{
			TRACE("Couldn't load panel bitmap \n");
			enabled = gfalse;
			return E_FAIL;
		}
	}
	return hr;
}


// release DD resources
void GPanel::Term()
{
	RELEASE(panelSurf);
	int i;
	for(i=0; i<43; i++) 
		RELEASE(panelSurfSphere[i]);

}

#endif // _D3D



// OGL OGL OGL OGL OGL OGL OGL OGL OGL OGL OGL OGL OGL OGL OGL OGL OGL OGL OGL
#ifdef _OGL

GPanel();
	

HRESULT GPanel::Init()
{
	if (InitPanelBitmaps())
		return S_OK;
	else {
		enabled = gfalse;
		return S_FALSE;

	}
}

void GPanel::Term()
{
	int i;
	for(i=0; i<43; i++) {
		delete m_OGLBitmapArray[i];
		m_OGLBitmapArray[i]=NULL;
	}
}

//currently only used by OGL

// read panel bitmaps
gbool GPanel::InitPanelBitmaps()
{
	int ret;
	int i;
	for(i=0; i<43; i++) 
	{
		ASSERT(m_OGLBitmapArray[i]==NULL);
		m_OGLBitmapArray[i] = TranslateByGDI(m_BitmapArray[i]);
		if (m_OGLBitmapArray[i] == NULL) goto exit_error;
	}

	return gtrue;

exit_error:

	TRACE("Init panel bitmaps error : %d \n",ret);

	return gfalse;

}


BYTE* GPanel::TranslateByGDI(CBitmap *newCBitmap)//CDIB& aDib, CDIBPal& aPalSrc)
{
	//convert a 16-bit pix to a 32-bit rgba needed for ogl's alpha-blending
	

	//		CBitmap -> dcImage
	//      dcImage  -bitblt-> 32-bit Dib section
	//		bgr -> rgba

	BYTE *pBits=NULL, *m_pBits=NULL;

	
	BITMAP bm;
	newCBitmap->GetBitmap(&bm);
    //   
	// Allocate memory for new DIB section.
	// No color table is needed.
	const int BITMAPINFOHEADER_SIZE = sizeof(BITMAPINFOHEADER) ;

	BITMAPINFOHEADER abBitmapInfo;
	BITMAPINFOHEADER* pBMIH = &abBitmapInfo;

	memset(pBMIH, 0, BITMAPINFOHEADER_SIZE);   // Fill in the header info.
   
	pBMIH->biSize         = sizeof(BITMAPINFOHEADER);
	pBMIH->biWidth        = bm.bmWidth;
	pBMIH->biHeight       = bm.bmHeight;
	pBMIH->biPlanes       = 1;
	pBMIH->biBitCount     = 32 ; 
	pBMIH->biCompression  = BI_RGB ;
    //
	// Create the new 32-bpp DIB section.
	//
	CDC dc;
	if (!dc.CreateCompatibleDC(NULL)) {
		return NULL;
	}


	HBITMAP hbmBuffer = CreateDIBSection( dc.GetSafeHdc(),
                           (BITMAPINFO*) pBMIH,
                                         DIB_RGB_COLORS,
                               (VOID **) &pBits,
                                         NULL,
	                                         0);
	
	// CreateDIBSection failed pBits NULL ==> crash

	ASSERT(pBits != NULL);
	ASSERT(hbmBuffer != (HBITMAP) NULL);

	if (!hbmBuffer) {
		return NULL;
	}	

	// Select DIB into DC.
//	HBITMAP hbmOld = (HBITMAP)::SelectObject(dc.GetSafeHdc(), hbmBuffer);
	//HBITMAP hbmOld;

	HBITMAP hbmOld = (HBITMAP) dc.SelectObject(hbmBuffer);

	CDC dcImage;

	if (!dcImage.CreateCompatibleDC(NULL)) {
	   DeleteObject(hbmBuffer);
	   return NULL;
    }	

	//Paint the image.
	CBitmap* pOldBitmap = dcImage.SelectObject(newCBitmap);
	dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dcImage, 0, 0, SRCCOPY);
	dcImage.SelectObject(pOldBitmap);
	
	// Blt the Windows DIB into our new DIB.
	//dc.SelectPalette(&aPalSrc,0) ;
	//dc.RealizePalette() ;
	//aDib.Draw(&dc,0,0) ;
	//GdiFlush() ;
   

	//
   // Copy the bits out of the DIB section.
   // Change from BGR to RGB.   
	//
	
   int iImageSize = bm.bmWidth * bm.bmHeight * sizeof(RGBQUAD) ;
   
   //m_pBits = (BYTE*)malloc(iImageSize) ;
   m_pBits = new BYTE[iImageSize]; // hg 

   if (m_pBits == NULL) {
	   DeleteObject(hbmBuffer);
	   return NULL;
   }	

   GdiFlush(); 


   RGBQUAD* pSrc = (RGBQUAD*)pBits ;
   RGBQUAD* pDest = (RGBQUAD*) m_pBits ;

   ASSERT(pSrc  != NULL);
   ASSERT(pDest != NULL);

   int cnt= bm.bmWidth*bm.bmHeight;

   for (int i = 0 ; i < cnt  ; i++)
   {
      pDest->rgbRed = pSrc->rgbBlue ;
      pDest->rgbGreen = pSrc->rgbGreen ;
      pDest->rgbBlue = pSrc->rgbRed ;
	  
	  if (pSrc->rgbBlue == 0xff && pSrc->rgbRed == 0x00 && pSrc->rgbGreen == 0x00) 
		  pDest->rgbReserved=0;//transparent
      else 
		  pDest->rgbReserved=1;//opaque
	  
	  pDest++ ;
      pSrc++ ;
   }
   //
   // Get rid of all the GDI stuff.
   //
	if (hbmOld) 
		dc.SelectObject(hbmOld); // hg 
   //if (hbmOld)
     //   (HBITMAP)::SelectObject(dc.GetSafeHdc(), hbmOld);

//exit:

   if (hbmBuffer != NULL ) 
	   DeleteObject(hbmBuffer) ;
   return m_pBits;   
}


HRESULT GPanel::Blt()
{	
	// Copy the panel to target
	// this call fails if rects on screen !!!!
	if (Minimized())
	    bmNumber = 35;//only the close button is shown
	
	CRect rect;

	switch (bmNumber) {
		case -1:
			break;
		case 29:
		case 30:
			rect = rectViewP;
			break;
		case 31:
		case 32:
			rect = rectViewR;
			break;
		case 33:
		case 34:
			rect = rectViewN;
			break;
		case 35:
		case 36:
			rect = rectClose;
			break;
		case 37:
		case 38:
			rect = rectSlideL;
			break;
		case 39:
		case 40:
			rect = rectSlideR;
			break;
		case 41:
		case 42:
			rect = rectHelp;
			break;

		default:
			rect = rectSphere;
			break;
	}
	
	//mapping to ogl-coords
	GLfloat x1, y1, x2, y2;
	
	x1 =  destRect.left;
	y1 = GAP;
	if (bmNumber != -1) {
		x2 = destRect.left + rect.left;
		y2 =  screenRect.Height() - destRect.top - rect.bottom;
	}


	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, screenRect.Width(), 0.0, screenRect.Height());

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	BITMAP bmPanel, bm;
	m_BitmapArray[16]->GetBitmap(&bmPanel);
	if (bmNumber != -1)
		m_BitmapArray[bmNumber]->GetBitmap(&bm);
	
	// enable alphafunc
	int _alpha = glIsEnabled(GL_ALPHA_TEST);
	int _blend = glIsEnabled(GL_BLEND);
	int _fog   = glIsEnabled(GL_FOG);
	int _lighting = glIsEnabled(GL_LIGHTING);
	int _texture_1d = glIsEnabled(GL_TEXTURE_1D);
	int _texture_2d = glIsEnabled(GL_TEXTURE_2D);
	int  _depth= glIsEnabled(GL_DEPTH_TEST);
	GLboolean _depthWrite;

    glGetBooleanv( GL_DEPTH_WRITEMASK ,&_depthWrite); // hg
 

	if (_blend) glDisable(GL_BLEND);
	if (_fog) glDisable(GL_FOG);
	if (_lighting) glDisable(GL_LIGHTING);
	if (_texture_1d) glDisable(GL_TEXTURE_1D);
	if (_texture_2d) glDisable(GL_TEXTURE_2D);
	if (_depth) glDisable(GL_DEPTH_TEST);
	if (_depthWrite) glDepthMask(GL_FALSE);

	GLint _alphaFunc;
	glGetIntegerv(GL_ALPHA_TEST_FUNC,&_alphaFunc);
	GLfloat _alphaRef;
	glGetFloatv(GL_ALPHA_TEST_REF,&_alphaRef);
		// change state
	glAlphaFunc(GL_GREATER,0.0);

	if (!_alpha)
		// no blending done, enable alpha func skipping pixels with alpha = 0.0   
		glEnable(GL_ALPHA_TEST);
		
	glRasterPos2f(x1, y1);
	if (!Minimized()) //blt the panel itself
	{
		if (cache == NULL) {
			cache = new glCache();
			cache->Begin();
			glDrawPixels(bmPanel.bmWidth, bmPanel.bmHeight, GL_RGBA, GL_UNSIGNED_BYTE, m_OGLBitmapArray[16]) ;
			cache->End();
		}
		cache->Call();
	}

	glRasterPos2f(x2, y2);
	if (bmNumber != -1) //blt its elements
		glDrawPixels(bm.bmWidth, bm.bmHeight, GL_RGBA, GL_UNSIGNED_BYTE, m_OGLBitmapArray[bmNumber]) ;
		
	glAlphaFunc(_alphaFunc,_alphaRef);
	if (!_alpha) glDisable(GL_ALPHA_TEST);

	//restore the modelview-matrix
	glPopMatrix();

	//restore the projection
	glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	if (_blend) glEnable(GL_BLEND);
	if (_fog) glEnable(GL_FOG);
	if (_depth) glEnable(GL_DEPTH_TEST);
	if (_texture_1d) glEnable(GL_TEXTURE_1D);
	if (_texture_2d) glEnable(GL_TEXTURE_2D);
	if (_lighting) glEnable(GL_LIGHTING);
	if (_depthWrite) glDepthMask(GL_TRUE);

	return S_OK;

}
#endif _OGL





///////////////////////////////////////////////////////////////////////////////////////////////////

BOOL GPanel::Inside(POINT point) 
{
/*	POINT offset;
	offset.x = point.x;
	offset.y = point.y+10; hg */
	if (!minimized && (destRect.PtInRect(point) /* || destRect.PtInRect(offset) */) )
		return TRUE;
	else if (minimized) {
		point.x -= destRect.left;
		point.y -= destRect.top;
		if (rectClose.PtInRect(point))
			return TRUE;
		else {
			isActive = FALSE;
			return FALSE;
		}
	}
	else {
		isActive = FALSE;
		return FALSE;
	}

}

int GPanel::OnMouseAction(CPoint point, int actionFlag)
//is called by OnMouseMove and by OnLButtonDown
//determines according to the flag what has to be done (button pressed
//or mouse over)
//returns a number to the ctrl that represents a button
{
	int result;
	result = -1;

	if (!enabled) {
		isActive = FALSE;
		return result;
	}

	point -= destRect.TopLeft();

	if (rectSphere.PtInRect(point))
	{
		float delta, deltaX, deltaY;
		
		//winkel berechnen
		deltaX = point.x - rectSphere.CenterPoint().x ;
		deltaY = rectSphere.CenterPoint().y - point.y;
		
		if (deltaX == 0) deltaX=.1f;
		delta = deltaY/deltaX;
		
		if (deltaX>=0){
			if (delta>=tan60) {
				bmNumber = 0;
				result =  PANEL_SPHERE0;
			}
			else if (delta<tan60 && delta>=tan30) {
				bmNumber = 2;
				result =  PANEL_SPHERE1;
			}
			else if (delta<tan30 && delta>=tanMin30) {
				bmNumber = 4;
				result =  PANEL_SPHERE2;
			}
			else if (delta<tanMin30 && delta>tanMin60) {
				bmNumber = 6;
				result =  PANEL_SPHERE3;
			}
			else {
				bmNumber = 8;
				result =  PANEL_SPHERE4;
			}
		}
		else {
			if (delta>=tan60) {
				bmNumber = 8;
				result =  PANEL_SPHERE4;
			}
			else if (delta<tan60 && delta>=tan30) {
				bmNumber = 10;
				result =  PANEL_SPHERE5;
			}
			else if (delta<tan30 && delta>=tanMin30) {
				bmNumber = 12;
				result =  PANEL_SPHERE6;
			}
			else if (delta<tanMin30 && delta>tanMin60) {
				bmNumber = 14;
				result =  PANEL_SPHERE7;
			}
			else {
				bmNumber = 0;
				result =  PANEL_SPHERE0;
			}
		}
	}// <- in rectSphere

	else if (rectViewP.PtInRect(point)) 
			{
			bmNumber = 29;
			result =  PANEL_VIEW_PREV;
		}
	else if (rectViewR.PtInRect(point)) 
		{
		bmNumber = 31;
		result =  PANEL_VIEW_RESET;
		}
	else if (rectViewN.PtInRect(point)) 
		{
		bmNumber = 33;
		result =  PANEL_VIEW_NEXT;
		}
	else if (rectClose.PtInRect(point)) 
		{
		bmNumber = 35;
		result =  PANEL_CLOSE;
		}
	else if (rectSlideL.PtInRect(point)) 
		{
		bmNumber = 37;
		result =  PANEL_SLIDE_LEFT;
		}
	else if (rectSlideR.PtInRect(point)) 
		{
		bmNumber = 39;
		result =  PANEL_SLIDE_RIGHT;
		}
	else if (rectHelp.PtInRect(point))
		{
		bmNumber = 41;
		result =  PANEL_HELP;
		}
	else { //no button was hit
		isActive = FALSE;
		bmNumber = -1;
		result = -1;
	}
	

	if (actionFlag == PANEL_LBUTTONDOWN)
		bmNumber = bmNumber;
	else if (actionFlag == PANEL_MOUSEMOVE_DOWN 
			&& oldbmNumber != bmNumber 
			&& result > PANEL_SPHERE7)
			//illegal drag
	{
		bmNumber = -1;
		result = -1;
	}
	else if (actionFlag == PANEL_MOUSEMOVE_OVER && result != -1)
		bmNumber += 1; 
	else if (actionFlag == PANEL_LBUTTONUP && result != -1)
		bmNumber += 1;

	if (bmNumber == -1)
		isActive = FALSE;
	else
		isActive = TRUE;

	if (oldbmNumber != bmNumber && !screenToSmall) {
		oldbmNumber = bmNumber;
		UpdatePanel(0,0,0,0,0);//blit into the front-buffer immediatly
	}

#ifdef _DEBUG_PANEL
	TRACE ("\nGPanel::OnMouseAction:   bmNumber %i\n", bmNumber);
	TRACE ("GPanel::OnMouseAction:   result %i\n", result);
	TRACE ("GPanel::OnMouseAction:   isActive %i\n", isActive);
#endif _DEBUG_PANEL

	return result;
}

void GPanel::GetHelpButPos(CPoint* point)
{
	*point = destRect.TopLeft() + rectHelp.TopLeft();
}

BOOL GPanel::InsideSphere(CPoint point)
{
	point -= destRect.TopLeft();
	if (rectSphere.PtInRect(point))
		return TRUE;
	else return FALSE;
}

void GPanel::GetCenterPos(POINT *point)
{
	point->x = destRect.left + rectSphere.CenterPoint().x;
	point->y = destRect.top + rectSphere.CenterPoint().y;
}

