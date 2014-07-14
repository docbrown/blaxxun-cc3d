//-----------------------------------------------------------------------------
// File: D3DUtil.h
//
// Desc: Helper functions and typing shortcuts for Direct3D programming.
//
//
// Copyright (C) 1997 Microsoft Corporation. All rights reserved
//-----------------------------------------------------------------------------

#ifndef _D3DUtil_H
#define _D3DUtil_H

#include <ddraw.h>
#include <d3d.h>


//-----------------------------------------------------------------------------
// Typing shortcuts for deleting and freeing objects.
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }


VOID AppOutputText( LPDIRECT3DDEVICE3 pd3dDevice, DWORD x, DWORD y, CHAR* str );


//-----------------------------------------------------------------------------
// Short cut functions for creating and using DX structures
//-----------------------------------------------------------------------------
VOID InitDeviceDesc( D3DDEVICEDESC& ddDevDesc );
VOID InitSurfaceDesc( DDSURFACEDESC2& ddsd, DWORD dwFlags=0,
                              DWORD dwCaps=0 );
VOID InitViewport( D3DVIEWPORT2& vdViewData, DWORD dwWidth=0,
                           DWORD dwHeight=0 );
VOID InitMaterial( D3DMATERIAL& mdMtrlData, FLOAT r=0.0f, FLOAT g=0.0f,
                           FLOAT b=0.0f );
VOID InitLight( D3DLIGHT& ldLightData, D3DLIGHTTYPE ltType, 
                        FLOAT x=0.0f, FLOAT y=0.0f, FLOAT z=0.0f );




//-----------------------------------------------------------------------------
// Miscellaneous helper functions
//-----------------------------------------------------------------------------
LPDIRECTDRAW4 GetDirectDrawFromDevice( LPDIRECT3DDEVICE3 pd3dDevice );
DWORD         GetDeviceMemoryType( LPDIRECT3DDEVICE3 pd3dDevice );
DWORD         GetDisplayDepth( LPDIRECTDRAW4 pDD4=NULL );



//-----------------------------------------------------------------------------
// Debug printing support
//-----------------------------------------------------------------------------

HRESULT _DbgOut( TCHAR*, DWORD, HRESULT, TCHAR* );

#if defined(DEBUG) | defined(_DEBUG)
    #define DEBUG_MSG(str)    _DbgOut( __FILE__, (DWORD)__LINE__, 0, str )
    #define DEBUG_ERR(hr,str) _DbgOut( __FILE__, (DWORD)__LINE__, hr, str )
#else
    #define DEBUG_MSG(str)    (0L)
    #define DEBUG_ERR(hr,str) (hr)
#endif




#endif  _D3DUtil_H


