
#include "stdafx.h"

#ifdef _D3D

//-----------------------------------------------------------------------------
// File: D3DUtil.cpp
//
// Desc: Shortcut macros and functions for using DX objects
//
//
// Copyright (c) 1997-1998 Microsoft Corporation. All rights reserved
//-----------------------------------------------------------------------------

//#define D3D_OVERLOADS
//#define STRICT
#include <math.h>
#include <stdio.h>
#include "d3dutil.h"



//-----------------------------------------------------------------------------
// Name: InitDeviceDesc()
// Desc: Helper function called to initialize a D3DDEVICEDESC structure,
//-----------------------------------------------------------------------------
VOID InitDeviceDesc( D3DDEVICEDESC& ddDevDesc )
{
    ZeroMemory( &ddDevDesc, sizeof(D3DDEVICEDESC) );
    ddDevDesc.dwSize                  = sizeof(D3DDEVICEDESC);
    ddDevDesc.dtcTransformCaps.dwSize = sizeof(D3DTRANSFORMCAPS);
    ddDevDesc.dlcLightingCaps.dwSize  = sizeof(D3DLIGHTINGCAPS);
    ddDevDesc.dpcLineCaps.dwSize      = sizeof(D3DPRIMCAPS);
    ddDevDesc.dpcTriCaps.dwSize       = sizeof(D3DPRIMCAPS);
}




//-----------------------------------------------------------------------------
// Name: InitSurfaceDesc()
// Desc: Helper function called to build a DDSURFACEDESC2 structure,
//       typically before calling CreateSurface() or GetSurfaceDesc()
//-----------------------------------------------------------------------------
VOID InitSurfaceDesc( DDSURFACEDESC2& ddsd, DWORD dwFlags,
                              DWORD dwCaps )
{
    ZeroMemory( &ddsd, sizeof(DDSURFACEDESC2) );
    ddsd.dwSize                 = sizeof(DDSURFACEDESC2);
    ddsd.dwFlags                = dwFlags;
    ddsd.ddsCaps.dwCaps         = dwCaps;
    ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
}




//-----------------------------------------------------------------------------
// Name: InitViewport()
// Desc: Helper function called to build a D3DVIEWPORT3 structure
//-----------------------------------------------------------------------------
VOID InitViewport( D3DVIEWPORT2& vp, DWORD dwWidth, DWORD dwHeight )
{
    ZeroMemory( &vp, sizeof(D3DVIEWPORT2) );
    vp.dwSize   = sizeof(D3DVIEWPORT2);
    vp.dwWidth  = dwWidth;
    vp.dwHeight = dwHeight;
    vp.dvMaxZ   = 1.0f;

    vp.dvClipX      = -1.0f;
    vp.dvClipWidth  = 2.0f;
    vp.dvClipY      = 1.0f;
    vp.dvClipHeight = 2.0f;
}




//-----------------------------------------------------------------------------
// Name: InitMaterial()
// Desc: Helper function called to build a D3DMATERIAL structure
//-----------------------------------------------------------------------------
VOID InitMaterial( D3DMATERIAL& mtrl, FLOAT r, FLOAT g, FLOAT b )
{
    ZeroMemory( &mtrl, sizeof(D3DMATERIAL) );
    mtrl.dwSize       = sizeof(D3DMATERIAL);
    mtrl.dcvDiffuse.r = mtrl.dcvAmbient.r = r;
    mtrl.dcvDiffuse.g = mtrl.dcvAmbient.g = g;
    mtrl.dcvDiffuse.b = mtrl.dcvAmbient.b = b;
    mtrl.dwRampSize   = 16L; // A default ramp size
}




//-----------------------------------------------------------------------------
// Name: InitLight()
// Desc: Initializes a D3DLIGHT structure
//-----------------------------------------------------------------------------
VOID InitLight( D3DLIGHT& light, D3DLIGHTTYPE ltType, 
                        FLOAT x, FLOAT y, FLOAT z )
{
    ZeroMemory( &light, sizeof(D3DLIGHT) );
    light.dwSize       = sizeof(D3DLIGHT);
    light.dltType      = ltType;
    light.dcvColor.r   = 1.0f;
    light.dcvColor.g   = 1.0f;
    light.dcvColor.b   = 1.0f;
    light.dvPosition.x = light.dvDirection.x = x;
    light.dvPosition.y = light.dvDirection.y = y;
    light.dvPosition.z = light.dvDirection.z = z;
}




//-----------------------------------------------------------------------------
// Name: GetDirectDrawFromDevice()
// Desc: Get the DDraw interface from a D3DDevice.
//-----------------------------------------------------------------------------
LPDIRECTDRAW4 GetDirectDrawFromDevice( LPDIRECT3DDEVICE3 pd3dDevice )
{
	LPDIRECTDRAW4        pDD = NULL;
	LPDIRECTDRAWSURFACE4 pddsRender;

    if( pd3dDevice )
	{
	    // Get the current render target
		if( SUCCEEDED( pd3dDevice->GetRenderTarget( &pddsRender ) ) )
		{
		    // Get the DDraw4 interface from the render target
			pddsRender->GetDDInterface( (VOID**)&pDD );
			pddsRender->Release();
		}
	}
	return pDD;
}




//-----------------------------------------------------------------------------
// Name: GetDeviceMemoryType()
// Desc: Retreives the default memory type used for the device.
//-----------------------------------------------------------------------------
DWORD GetDeviceMemoryType( LPDIRECT3DDEVICE3 pd3dDevice )
{
	D3DDEVICEDESC ddHwDesc, ddSwDesc;
	ddHwDesc.dwSize = sizeof(D3DDEVICEDESC);
	ddSwDesc.dwSize = sizeof(D3DDEVICEDESC);
	if( FAILED( pd3dDevice->GetCaps( &ddHwDesc, &ddSwDesc ) ) )
		return 0L;

	if( ddHwDesc.dwFlags )
		return DDSCAPS_VIDEOMEMORY;

	return DDSCAPS_SYSTEMMEMORY;
}






//-----------------------------------------------------------------------------
// Name: GetDisplayDepth()
// Desc: Returns the depth of the current display mode.
//-----------------------------------------------------------------------------
DWORD GetDisplayDepth( LPDIRECTDRAW4 pDD4 )
{
	// If the caller did not supply a DDraw object, just create a temp one.
	if( NULL == pDD4 )
	{
		LPDIRECTDRAW pDD1;
		if( FAILED( DirectDrawCreate( NULL, &pDD1, NULL ) ) )
			return 0L;
	
		HRESULT hr = pDD1->QueryInterface( IID_IDirectDraw4, (VOID**)&pDD4 );
		pDD1->Release();
		if( FAILED(hr) )
			return 0L;
	}
	else
		pDD4->AddRef();

	// Get the display mode description
	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd, sizeof(DDSURFACEDESC2) );
	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	pDD4->GetDisplayMode( &ddsd );
	pDD4->Release();

	// Return the display mode's depth
	return ddsd.ddpfPixelFormat.dwRGBBitCount;
}




//-----------------------------------------------------------------------------
// Name: _DbgOut()
// Desc: Outputs a message to the debug stream
//-----------------------------------------------------------------------------
HRESULT _DbgOut( TCHAR* strFile, DWORD dwLine, HRESULT hr, TCHAR* strMsg )
{
    TCHAR buffer[256];
    sprintf( buffer, "%s(%ld): ", strFile, dwLine );
    OutputDebugString( buffer );
    OutputDebugString( strMsg );
    
    if( hr )
    {
        sprintf( buffer, "(hr=%08lx)\n", hr );
        OutputDebugString( buffer );
    }

    OutputDebugString( "\n" );
    
    return hr;
}


#if 0
//-----------------------------------------------------------------------------
// Name: AppShowStats()
// Desc: Shows frame rate and dimensions of the rendering device. Note: a 
//       "real" app wouldn't query the surface dimensions each frame.
//-----------------------------------------------------------------------------
VOID AppShowStats()
{
    static FLOAT fFPS      = 0.0f;
    static FLOAT fLastTime = 0.0f;
    static DWORD dwFrames  = 0L;

	// Keep track of the time lapse and frame count
	FLOAT fTime = timeGetTime() * 0.001f; // Get current time in seconds
	++dwFrames;

	// Update the frame rate once per second
	if( fTime - fLastTime > 1.0f )
    {
        fFPS      = dwFrames / (fTime - fLastTime);
        fLastTime = fTime;
        dwFrames  = 0L;
    }

    // Get dimensions of the render surface 
    DDSURFACEDESC2 ddsd;
    ddsd.dwSize = sizeof(DDSURFACEDESC2);
    g_pFramework->GetRenderSurface()->GetSurfaceDesc(&ddsd);

    // Setup the text buffer to write out
    CHAR buffer[80];
    sprintf( buffer, "%7.02f fps (%dx%dx%d)", fFPS, ddsd.dwWidth,
             ddsd.dwHeight, ddsd.ddpfPixelFormat.dwRGBBitCount );
    AppOutputText( g_pFramework->GetD3DDevice(), 0, 0, buffer );
}

#endif

//-----------------------------------------------------------------------------
// Name: AppOutputText()
// Desc: Draws text on the window.
//-----------------------------------------------------------------------------
VOID AppOutputText( LPDIRECT3DDEVICE3 pd3dDevice, DWORD x, DWORD y, CHAR* str )
{
    LPDIRECTDRAWSURFACE4 pddsRenderSurface;
    if( FAILED( pd3dDevice->GetRenderTarget( &pddsRenderSurface ) ) )
        return;

    // Get a DC for the surface. Then, write out the buffer
    HDC hDC;
    if( SUCCEEDED( pddsRenderSurface->GetDC(&hDC) ) )
    {
        SetTextColor( hDC, RGB(255,255,0) );
        SetBkMode( hDC, TRANSPARENT );
        ExtTextOut( hDC, x, y, 0, NULL, str, strlen(str), NULL );
    
        pddsRenderSurface->ReleaseDC(hDC);
    }
    pddsRenderSurface->Release();
}

#if 0
//-----------------------------------------------------------------------------
// Name: CopyBitmapToSurface()
// Desc: Copies the image of a bitmap into a surface
//-----------------------------------------------------------------------------
static HRESULT CopyBitmapToSurface( LPDIRECTDRAWSURFACE pddsTarget,
                                    HBITMAP hbmBitmap, DWORD dwFlags )
{
    // Get a DDraw object to create a temporary surface
    LPDIRECTDRAW pDD;
    pddsTarget->GetDDInterface( (VOID**)&pDD );
    pDD->Release();

    // Get the bitmap structure (to extract width, height, and bpp)
    BITMAP bm;
    GetObject( hbmBitmap, sizeof(BITMAP), &bm );

    // Setup the new surface desc
    DDSURFACEDESC ddsd;

    D3DUtil_InitSurfaceDesc( ddsd );

    pddsTarget->GetSurfaceDesc( &ddsd );
    ddsd.dwFlags          = DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH|DDSD_PIXELFORMAT|
		                    DDSD_TEXTURESTAGE;
    ddsd.ddsCaps.dwCaps   = DDSCAPS_TEXTURE|DDSCAPS_SYSTEMMEMORY;
    ddsd.ddsCaps.dwCaps2  = 0L;
    ddsd.dwWidth          = bm.bmWidth;
    ddsd.dwHeight         = bm.bmHeight;

    // Create a new surface for the texture
    LPDIRECTDRAWSURFACE pddsTempSurface;
	HRESULT hr;
    if( FAILED( hr = pDD->CreateSurface( &ddsd, &pddsTempSurface, NULL ) ) )
        return NULL;

    // Get a DC for the bitmap
    HDC hdcBitmap = CreateCompatibleDC( NULL );
    if( NULL == hdcBitmap )
    {
        pddsTempSurface->Release();
        return NULL;
    }
    SelectObject( hdcBitmap, hbmBitmap );

    // Handle palettized textures. Need to attach a palette
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
        SAFE_RELEASE( pPalette );
    }

    // Copy the bitmap image to the surface.
    HDC hdcSurface;
    if( SUCCEEDED( pddsTempSurface->GetDC( &hdcSurface ) ) )
    {
        BitBlt( hdcSurface, 0, 0, bm.bmWidth, bm.bmHeight, hdcBitmap, 0, 0,
                SRCCOPY );
        pddsTempSurface->ReleaseDC( hdcSurface );
    }
    DeleteDC( hdcBitmap );

    // Copy the temp surface to the real texture surface
    pddsTarget->Blt( NULL, pddsTempSurface, NULL, DDBLT_WAIT, NULL );

    if( ddsd.ddpfPixelFormat.dwRGBBitCount == 8 )
	{
        LPDIRECTDRAWPALETTE  pPalette;
		DWORD pe[256];
        pddsTempSurface->GetPalette( &pPalette );
		pPalette->GetEntries( 0, 0, 256, (PALETTEENTRY*)&pe );
		pPalette->Release();

		pddsTarget->GetPalette( &pPalette );
		pPalette->GetEntries( 0, 0, 256, (PALETTEENTRY*)&pe );
		pPalette->Release();
	}

    pddsTempSurface->Release();

	// For textures with real alpha (not palettized), set transparent bits
    if( ddsd.ddpfPixelFormat.dwRGBAlphaBitMask )
	{
		if( dwFlags & (D3DTEXTR_TRANSPARENTWHITE|D3DTEXTR_TRANSPARENTBLACK) )
		{
			// Lock the texture surface
			DDSURFACEDESC2 ddsd;
			D3DUtil_InitSurfaceDesc( ddsd );
			while( pddsTarget->Lock( NULL, &ddsd, 0, NULL ) ==
				   DDERR_WASSTILLDRAWING );
			 
		    DWORD dwAlphaMask = ddsd.ddpfPixelFormat.dwRGBAlphaBitMask;
			DWORD dwRGBMask   = ( ddsd.ddpfPixelFormat.dwRBitMask |
							      ddsd.ddpfPixelFormat.dwGBitMask |
							      ddsd.ddpfPixelFormat.dwBBitMask );
			DWORD dwColorkey  = 0x00000000; // Colorkey on black
			if( dwFlags & D3DTEXTR_TRANSPARENTWHITE ) 
				dwColorkey = dwRGBMask;     // Colorkey on white

			// Add an opaque alpha value to each non-colorkeyed pixel
			for( DWORD y=0; y<ddsd.dwHeight; y++ )
			{
				WORD*  p16 =  (WORD*)((BYTE*)ddsd.lpSurface + y*ddsd.lPitch);
				DWORD* p32 = (DWORD*)((BYTE*)ddsd.lpSurface + y*ddsd.lPitch);
        
				for( DWORD x=0; x<ddsd.dwWidth; x++ )
				{
					if( ddsd.ddpfPixelFormat.dwRGBBitCount == 16 )
					{
						if( ( *p16 &= dwRGBMask ) != dwColorkey )
							*p16 |= dwAlphaMask;
						p16++;
					}
					if( ddsd.ddpfPixelFormat.dwRGBBitCount == 32 )
					{
						if( ( *p32 &= dwRGBMask ) != dwColorkey )
							*p32 |= dwAlphaMask;
						p32++;
					}
				}
			}
			pddsTarget->Unlock( NULL );
		}
	}

    return S_OK;;
}


#endif 0

#endif
