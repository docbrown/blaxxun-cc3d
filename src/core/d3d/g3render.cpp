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

#include "stdafx.h"
/******************************************************************************
@doc

@module G3Render.cpp - GLView RenderState classes for Direct 3D |

Copyright (c) 1996-1999	by Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:

Changes:
	12.10.99 HG updated OnTextureMemoryOverflow scale down logic
	28.03.97 added STARTFLAT Macro to PUT_POLYGON

$Revision: 1.27 $
$Log: $

Todo :

	lighted color per vertex


******************************************************************************/


//#include <math.h>

#ifdef _D3D
//#include "d3d.h"
//#include "d3dmacs.h"



#include "g3render.h"
#include "gd3device.h"


#include "gcamera.h"
#include "greporter.h"

#include "glutils.h"



HRESULT D3DError_(const char *s,HRESULT error, const char *file, int line)
{ 
   if (error != D3D_OK) {
      TRACE("(%s:%d)D3DError : %s (%d)  \n",file,line,s, error);
      Reporter.Error("(%s:%d)D3DError : %s (%d %x) %s  \n",file,line,s, error,error,GD3Device::ErrorToString(error));
      return(error);
   }
   return(error);
}


D3DMATRIX G3Matrix::identity = {
    D3DVAL(1.0), D3DVAL(0.0), D3DVAL(0.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(1.0), D3DVAL(0.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(0.0), D3DVAL(1.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(0.0), D3DVAL(0.0), D3DVAL(1.0)
};


// global error variable 
//HRESULT GLastError=D3D_OK;



/*
   set complete geometry execute buffer
   given vertices and triangles
*/
gbool 
G3ExBufPtr::Set(int nV, D3DVERTEX *v,int nTri, D3DTRIANGLE *tri)
{
    /*
     * Copy vertices to execute buffer
     */
    VERTEX_DATA(v, nV, lpPointer);

	offset = Length();

    /*
     * Setup instructions in execute buffer
     */
/*
    OP_MATRIX_MULTIPLY(3, lpPointer);
        MATRIX_MULTIPLY_DATA(hViewRot, hDViewRot, hViewRot, lpPointer);
        MATRIX_MULTIPLY_DATA(hViewRot, hViewPos, hView, lpPointer);
        MATRIX_MULTIPLY_DATA(hWorld, hDWorld, hWorld, lpPointer);
    OP_STATE_LIGHT(1, lpPointer);
        STATE_DATA(D3DLIGHTSTATE_MATERIAL, hMat2, lpPointer);
    // flags for updating status flag status extent
    OP_SET_STATUS(D3DSETSTATUS_ALL, D3DSTATUS_DEFAULT, 2048, 2048, 0, 0, lpPointer);
*/


/*
    OP_STATE_LIGHT(1, lpPointer);
        STATE_DATA(D3DLIGHTSTATE_MATERIAL, hMat2, lpPointer);
*/
    OP_PROCESS_VERTICES(1, lpPointer);
        PROCESSVERTICES_DATA(D3DPROCESSVERTICES_TRANSFORMLIGHT, 0, nV, lpPointer);
/*
    OP_STATE_RENDER(3, lpPointer);
        STATE_DATA(D3DRENDERSTATE_TEXTUREHANDLE, bTex, lpPointer);
        STATE_DATA(D3DRENDERSTATE_WRAPU, FALSE, lpPointer);
        STATE_DATA(D3DRENDERSTATE_WRAPV, FALSE, lpPointer);
*/
    /*
     * Make sure that the triangle data (not OP) will be QWORD aligned
     */
    if (QWORD_ALIGNED(lpPointer)) {
        OP_NOP(lpPointer);
    }
    OP_TRIANGLE_LIST(nTri, lpPointer);
	   TRIANGLE_LIST_DATA(tri, nTri, lpPointer);

/*
        lpTri = (LPD3DTRIANGLE)lpPointer;
        for (i = 0; i < NUM_TRIANGLES; i++) {
            lpTri->v1 = t[i][0];
            lpTri->v2 = t[i][1];
            lpTri->v3 = t[i][2];
            lpTri->wFlags = D3DTRIFLAG_EDGEENABLETRIANGLE;
            lpTri++;
        }
        lpPointer = (void*)lpTri;

*/
    OP_EXIT(lpPointer);
	return(TRUE);
}

//
// 	G3LightHandle
// 

G3LightHandle::G3LightHandle() 
{
		
		refCnt = 0; next = prev = NULL; 	

		enabled = FALSE;
		lpD3DLight = NULL;
		lpView = NULL;

        dwSize = sizeof(D3DLIGHT);
        dltType = D3DLIGHT_DIRECTIONAL;
        dcvColor.r = D3DVAL(0.9);
        dcvColor.g = D3DVAL(0.9);
        dcvColor.b = D3DVAL(0.9);
        dcvColor.a = D3DVAL(1.0);
        dvDirection.x = D3DVAL(0.0);
        dvDirection.y = D3DVAL(0.0);
        dvDirection.z = D3DVAL(-1.0);
        dvAttenuation0 = (float)1.0;
        dvAttenuation1 = (float)0.0;
        dvAttenuation2 = (float)0.0;
		dvRange = 0;

		ambientIntensity = 0;		// new for custom lighting 
		ambientColor.r = 0;
		ambientColor.g = 0;
		ambientColor.b = 0;
		ambientColor.a = 0;

#if 0
		D3DLIGHTTYPE    dltType;            /* Type of light source */
		D3DCOLORVALUE   dcvColor;           /* Color of light */
		D3DVECTOR       dvPosition;         /* Position in world space */
		D3DVECTOR       dvDirection;        /* Direction in world space */
		D3DVALUE        dvRange;            /* Cutoff range */
		D3DVALUE        dvFalloff;          /* Falloff */
		D3DVALUE        dvAttenuation0;     /* Constant attenuation */
		D3DVALUE        dvAttenuation1;     /* Linear attenuation */
		D3DVALUE        dvAttenuation2;     /* Quadratic attenuation */
		D3DVALUE        dvTheta;            /* Inner angle of spotlight cone */
		D3DVALUE        dvPhi;              /* Outer angle of spotlight cone */


		ambient.Set(0,0,0, 1.0);
		diffuse.Set(1,1,1, 1.0);// gl default is ::	diffuse.Set(0,0,0, 1.0);
		specular.Set(0,0,0, 1.0);
		directional = 1;  
		spot=0;
		position.Set(0,0,1);
		direction.Set(0,0,-1);;
		SpotExponent = 0;
		SpotCutoff = 180;
		Attenuation.Set(1,0,0);
#endif

};
	

G3LightHandle::~G3LightHandle() 
{ 
	Release(); 
	if (lpD3DLight) 
		lpD3DLight->Release();  
	lpD3DLight = NULL;
	lpView = NULL;
}


void G3LightHandle::Release(LPDIRECT3DVIEWPORT theView) 
{ 
	if (lpD3DLight) { 
		if (enabled) 
			if (theView->DeleteLight(lpD3DLight) != D3D_OK) { }
		enabled = FALSE;
		lpD3DLight->Release();  
		lpD3DLight = NULL; 
		lpView = NULL;
	}
}



//
// RenderState
// 

RenderState::~RenderState()
{
	backgroundTexture.Release();
	defaultTexture.Release();

	// all ref cnts should be 0000, other wise leak in VRML scene graph

	DeleteUnusedExBufs();	 // DeleteExBufs();
    DeleteUnusedMaterials(); // DeleteMaterials();
    DeleteUnusedLights();	 // DeleteLights();
	DeleteUnusedTextures();  // DeleteTextures();
	DeleteUnusedVBBufs();

	ASSERT(textures == NULL); // if this fires an external app has handle to texture
	ASSERT(materials == NULL); // if this fires an external app has handle to texture
	ASSERT(lights == NULL); // if this fires an external app has handle to texture
	ASSERT(exBufs == NULL); // if this fires an external app has handle to texture
	ASSERT(VBBufs == NULL);
}


// apply any state changes

void RenderState::FlushState_() 
{
		 if (stateBuf.Length()>0) {
			 ASSERT(stateBuf.Length() < (stateBuf.MaxLength()-2*G3ExBufPtr::OP_SIZE()));
			 stateBuf.PUT_OP_EXIT();
			 stateBuf.Unlock();
			 stateBuf.Set();
                if (beginSceneCnt == 0) {
                    BeginScene();
				        Execute(stateBuf);
                    EndScene();
                }
				else Execute(stateBuf);
			 stateBuf.Clear();
			 stateBuf.Lock();
		 }
}


//
// set the rendering device context
//


void RenderState::SetDevice(GD3Device *newDevice) 
{
	device = newDevice;

	ASSERT(device != NULL);

    lpDD = device->lpDD;
    lpD3D = device->lpD3D;
    lpDev = device->lpD3DDevice;
    lpView = device->lpD3DViewport;

	// dx 5
    dx5 =  !!device->dx5;
    lpD3D2 =  device->lpD3D2;
    lpD3DDevice2 =  device->lpD3DDevice2;
    lpD3DViewport2=  device->lpD3DViewport2;

	// dx 6
    dx6 =  !!device->dx6;
    lpD3D3 =  device->lpD3D3;
    lpD3DDevice3 =  device->lpD3DDevice3;
    lpD3DViewport3=  device->lpD3DViewport3;

	dxLevel = device->dxLevel;

}

/*

   init of global state attributes
   once d3 basic setup is done

*/


void RenderState::Init()
{
	// reset state of texture memory handling 
	allTexturesInVideo = TRUE;
	
	textureParameters.square_size = FALSE; 	

	// find the texture formats 
	palette8Format = -1;
	palette4Format = -1;
	rgbFormat = -1;
	rgb24Format = -1;
	rgb16Format = -1;

	rgbaFormat = -1;
	rgba32Format = -1;
	rgba16Format = -1;
	rgba16_1Format = -1;
	gray_format = -1;
	grayalpha_format = -1;

	QueryCaps();

	InitState(512);

    //material;
	//backgroundMaterial;
    SetBackgroundColor();

	currentCullMode = D3DCULL_NONE;

	if (enableOwnLighting && canDoRgbLighting && (dx5 || dx6) ) { // && useOwnLighting
		
		doOwnLighting = gtrue;
        SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE); // turn off culling 
		doD3DCulling = enableD3DCulling;

		//viewMatrix.Set(lpDev,G3Matrix::identity);
		//projMatrix.Set(lpDev,G3Matrix::identity);

		viewMatrix.Set(lpDev,view);
		projMatrix.Set(lpDev,projection);
		worldMatrix.Set(lpDev,G3Matrix::identity);
		matrixChanged = gtrue;

	}
	else {
		doOwnLighting = gfalse;
		viewMatrix.Set(lpDev,view);
		projMatrix.Set(lpDev,projection);
		worldMatrix.Set(lpDev,G3Matrix::identity);
	}
	//branch to vertex buffer
	if (!doOwnLighting && dx6 && canDoRgbLighting) {
		// useVertexBuffer = gtrue; // in development
	}

	// put state attributes
	SetTransforms(projMatrix,viewMatrix,worldMatrix);
    
	SetMaterial(defaultMaterial,defaultMaterialValue); // new 2.0.6.99 hg 
    // set default texture
	
	//SetFillMode();
	//SetLightModel();
	//SetZBuffer();
	//SetDither();
	 // set SetState
   
	//xx FlushState();

	textureParameters.supported_image_fmts = 0;

	int anyRgbaFormat=-1;
	int anyRgbFormat=-1;

	{	

		// select a texture format 
		for(int i=0; i<device->NumTextureFormats; i++) {
			if ((device->TextureFormat[i].ddsd.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8) != 0) 
				palette8Format = i;
			else if ((device->TextureFormat[i].ddsd.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED4) != 0) 
				palette4Format = i;
			else if ((!device->TextureFormat[i].bPalettized)) {
				
				anyRgbFormat = i;

				if ( (device->TextureFormat[i].ddsd.ddpfPixelFormat.dwRGBBitCount==24) && (device->TextureFormat[i].RedBPP==8) && (device->TextureFormat[i].AlphaBPP==0)) 
					rgbFormat=rgb24Format = i;
				
				if ( (device->TextureFormat[i].RedBPP<8) && (device->TextureFormat[i].RedBPP>=5) && (device->TextureFormat[i].AlphaBPP==0)) 
					rgb16Format = i;

				if ( (device->TextureFormat[i].ddsd.ddpfPixelFormat.dwRGBBitCount == 32) && (device->TextureFormat[i].RedBPP==8) && (device->TextureFormat[i].AlphaBPP==8)) 
					rgbaFormat=rgba32Format = i;
				else
					if ((device->TextureFormat[i].RedBPP>0) && (device->TextureFormat[i].AlphaBPP>0)) {
						if (device->TextureFormat[i].AlphaBPP == 1)
							rgba16_1Format = i;
						else 
						    rgba16Format = i;
					}

			}
		}	

	}
	if (rgbFormat <0) rgbFormat = rgb16Format;
	if (rgbaFormat <0) rgbaFormat = rgba16Format;
	if (rgbaFormat <0) rgbaFormat = rgba16_1Format;
	if (rgbFormat <0) rgbFormat = anyRgbFormat;

	if (palette4Format >=0)
		textureParameters.supported_image_fmts |= (1<<IM_MAPPED4);

	if (palette8Format >=0)
		textureParameters.supported_image_fmts |= (1<<IM_MAPPED8);

	if (rgbFormat >=0)
		textureParameters.supported_image_fmts |= (1<<IM_RGB);

	if (rgbaFormat >=0)
		textureParameters.supported_image_fmts |= (1<<IM_RGBA);

}

// find a texture format matching srcPixelFormat
int RenderState::FindTextureFormat(DDPIXELFORMAT &srcPixelFormat,DDPIXELFORMAT &texFormat)
{
	int format = -1;
		// select a texture format 
		for(int i=0; i<device->NumTextureFormats; i++) {
			DDPIXELFORMAT  &d = device->TextureFormat[i].ddsd.ddpfPixelFormat;
			if (( d.dwFlags == srcPixelFormat.dwFlags)
					&& d.dwRGBBitCount == srcPixelFormat.dwRGBBitCount
					&& d.dwRBitMask == srcPixelFormat.dwRBitMask
					&& d.dwGBitMask == srcPixelFormat.dwGBitMask
					&& d.dwBBitMask == srcPixelFormat.dwBBitMask
					&& d.dwRGBAlphaBitMask == srcPixelFormat.dwRGBAlphaBitMask

				) {
					texFormat = d;
					return i;
			}
		}
	format = rgbFormat;
	if (format <0) format = palette8Format;
	
	if (format >=0) {
		texFormat = device->TextureFormat[format].ddsd.ddpfPixelFormat;
	}
	return format;
	
}


typedef struct _DX3_D3DDeviceDesc {
    DWORD            dwSize;                 /* Size of D3DDEVICEDESC structure */
    DWORD            dwFlags;                /* Indicates which fields have valid data */
    D3DCOLORMODEL    dcmColorModel;          /* Color model of device */
    DWORD            dwDevCaps;              /* Capabilities of device */
    D3DTRANSFORMCAPS dtcTransformCaps;       /* Capabilities of transform */
    BOOL             bClipping;              /* Device can do 3D clipping */
    D3DLIGHTINGCAPS  dlcLightingCaps;        /* Capabilities of lighting */
    D3DPRIMCAPS      dpcLineCaps;
    D3DPRIMCAPS      dpcTriCaps;
    DWORD            dwDeviceRenderBitDepth; /* One of DDBB_8, 16, etc.. */
    DWORD            dwDeviceZBufferBitDepth;/* One of DDBD_16, 32, etc.. */
    DWORD            dwMaxBufferSize;        /* Maximum execute buffer size */
    DWORD            dwMaxVertexCount;       /* Maximum vertex count */
} DX3_D3DDEVICEDESC;

/*
 * Description for a device.
 * This is used to describe a device that is to be created or to query
 * the current device.
 */
typedef struct _DX5_D3DDeviceDesc {
    DWORD	     dwSize;		     /* Size of D3DDEVICEDESC structure */
    DWORD	     dwFlags;	             /* Indicates which fields have valid data */
    D3DCOLORMODEL    dcmColorModel;	     /* Color model of device */
    DWORD	     dwDevCaps;	             /* Capabilities of device */
    D3DTRANSFORMCAPS dtcTransformCaps;       /* Capabilities of transform */
    BOOL	     bClipping;	             /* Device can do 3D clipping */
    D3DLIGHTINGCAPS  dlcLightingCaps;        /* Capabilities of lighting */
    D3DPRIMCAPS      dpcLineCaps;
    D3DPRIMCAPS      dpcTriCaps;
    DWORD	     dwDeviceRenderBitDepth; /* One of DDBB_8, 16, etc.. */
    DWORD	     dwDeviceZBufferBitDepth;/* One of DDBD_16, 32, etc.. */
    DWORD	     dwMaxBufferSize;        /* Maximum execute buffer size */
    DWORD	     dwMaxVertexCount;       /* Maximum vertex count */
    // *** New fields for DX5 *** //

    // Width and height caps are 0 for legacy HALs.
    DWORD        dwMinTextureWidth, dwMinTextureHeight;
    DWORD        dwMaxTextureWidth, dwMaxTextureHeight;
    DWORD        dwMinStippleWidth, dwMaxStippleWidth;
    DWORD        dwMinStippleHeight, dwMaxStippleHeight;
} DX5_D3DDEVICEDESC;




// query 
gbool RenderState::QueryCaps(D3DDEVICEDESC *pDesc)
{

	D3DDEVICEDESC desc;
	D3DDEVICEDESC hwDesc;


	needBlendForCKey = gfalse;
	
	if (!(dx5 || dx6))  needBlendForCKey=gtrue; // DX3


	int descSize = sizeof(D3DDEVICEDESC);

	if(dx6) {
	}
	else if ((dx5  && lpD3DDevice2))
		descSize = min(descSize,(int) sizeof(DX5_D3DDEVICEDESC)); // dx 5 back compatibility
	else 
		descSize = min(descSize,(int) sizeof(DX3_D3DDEVICEDESC)); // dx 3 back compatibility

	memset(&desc, 0, sizeof(D3DDEVICEDESC));
	desc.dwSize = descSize;
	desc.dwFlags = 0;

	memset(&hwDesc, 0, sizeof(D3DDEVICEDESC));
	hwDesc.dwSize = descSize;
	hwDesc.dwFlags = 0;

	if (dx6 && lpD3DDevice3) 
		GLastError = lpD3DDevice3->GetCaps(&hwDesc, &desc);
	else
	if (dx5 && lpD3DDevice2) 
		GLastError = lpD3DDevice2->GetCaps(&hwDesc, &desc);
	else GLastError = lpDev->GetCaps(&hwDesc, &desc);

    if (GLastError!=D3D_OK) 
		D3DError("GetCaps failed",GLastError);

	
	if (hwDesc.dwFlags !=0) desc = hwDesc; // assume hardware

	maxBufferSize=0;
	maxVertexCount=0;

	if (desc.dwFlags & D3DDD_MAXVERTEXCOUNT && (desc.dwMaxVertexCount>0)) 
		maxVertexCount = desc.dwMaxVertexCount ; 
	else
		maxVertexCount = 1024; // hw board seems to have often 1024

	if (desc.dwFlags & D3DDD_MAXBUFFERSIZE) 
		maxBufferSize = desc.dwMaxBufferSize;

	DWORD cullMask= D3DPMISCCAPS_CULLNONE | D3DPMISCCAPS_CULLCW | D3DPMISCCAPS_CULLCCW;
	
	canCull = (desc.dpcTriCaps.dwMiscCaps & cullMask) == cullMask;
	
	// canCull = 0; // to do: need to test with hardware board first 

	if (desc.dwFlags & D3DDD_COLORMODEL) {
		canDoRgbLighting = (desc.dcmColorModel & D3DCOLOR_RGB ) !=0;
	}
	else if (desc.dwFlags & D3DDD_LIGHTINGCAPS)  // HAL doesnt set this 
		canDoRgbLighting = (desc.dlcLightingCaps.dwLightingModel & D3DLIGHTINGMODEL_RGB) !=0;
	else canDoRgbLighting = 1;
	
	caps = desc;

	canDoFog=canDoRgbLighting;
	canDoAlphaBlend=canDoRgbLighting;

	/*	if (g_DriverCaps.ddsCaps.dwCaps & DDSCAPS_MIPMAP)
	{
        canDoMipMap = 1;
	} */

	if (desc.dpcTriCaps.dwTextureFilterCaps & D3DPTFILTERCAPS_LINEARMIPLINEAR) 
	{
		canDoMipMap = 1;
		maxMipfilter = D3DFILTER_LINEARMIPLINEAR;
	}
	else if (desc.dpcTriCaps.dwTextureFilterCaps & D3DPTFILTERCAPS_MIPLINEAR) 
	{
		canDoMipMap = 1;
		maxMipfilter = D3DFILTER_MIPLINEAR;
	}
	else if (desc.dpcTriCaps.dwTextureFilterCaps & D3DPTFILTERCAPS_MIPNEAREST) 
	{
		canDoMipMap = 1;
		maxMipfilter = D3DFILTER_MIPNEAREST;
	}
	else {
		canDoMipMap = 0;
		if (desc.dpcTriCaps.dwTextureFilterCaps & D3DPTFILTERCAPS_LINEAR) 
			maxMipfilter = D3DFILTER_LINEAR;
		else maxMipfilter = D3DFILTER_NEAREST;
	}
	// new 22.10.99 hg 
    textureParameters.defaultBlend = D3DTBLEND_MODULATE;
	textureParameters.unlitBlend = D3DTBLEND_DECAL;

	if (desc.dpcTriCaps.dwTextureBlendCaps & D3DPTBLENDCAPS_MODULATEALPHA) {
		textureParameters.defaultBlend = D3DTBLEND_MODULATEALPHA;
	}		
	if (desc.dpcTriCaps.dwTextureBlendCaps & D3DPTBLENDCAPS_DECALALPHA ) {
		//textureParameters.unlitBlend = D3DTBLEND_DECALALPHA ;
	}	
/* test for 4.3, does not seem to work properly	
	else if (desc.dpcTriCaps.dwTextureBlendCaps & D3DPTBLENDCAPS_COPY ) {
		textureParameters.unlitBlend = D3DTBLEND_COPY ; // for sw rasterizers 
	}		
*/

	// can do fog ? 
	if (desc.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_FOGVERTEX) {
		canDoFog = 1;
	}
	else canDoFog = 0;


	// can do alpha ? 
	if (desc.dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_ALPHA) {
		alphaStipple = 0;
	}
	else alphaStipple = 1;

	if( desc.dpcTriCaps.dwAlphaCmpCaps & D3DPCMPCAPS_GREATER )
        canDoAlphaTest = 1;
    else
        canDoAlphaTest = 0;


	// can do color key ? 
	if (desc.dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_TRANSPARENCY) { // is this colorKey ? 
	}

	if (desc.dwMaxTextureWidth>0 &&  desc.dwMaxTextureHeight>0) {
		textureParameters.SetMaxSize(desc.dwMaxTextureWidth,desc.dwMaxTextureWidth);
	}
	else textureParameters.SetMaxSize(1024,1024);

	if (desc.dwMinTextureWidth>0 &&  desc.dwMinTextureHeight>0) {
		textureParameters.SetMinSize(desc.dwMinTextureWidth,desc.dwMinTextureHeight);
	}
	else textureParameters.SetMinSize(1,1);

	textureParameters.square_size = (desc.dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_SQUAREONLY) !=0; 	
	textureParameters.border = (desc.dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_BORDER) !=0; 	

	
	return (GLastError==D3D_OK);

}

void RenderState::SetWorldMatrix(Matrix &m)
{  
	if (doOwnLighting) {
		lightsTransformed=gfalse;
		matrixChanged=gtrue;
		return;
	}
    D3DMATRIX *dm = (D3DMATRIX *) &m.x[0][0];
	worldMatrix.Set(lpDev,*dm);
	// if world matrix handle is current
   // no add to buffer is necessary 
}


    // set texture wrapping 
void RenderState::SetTextureWrap(gbool wrap_s, gbool wrap_t)
{

	if (dx5) 
        if ( wrap_s != textureParameters.wrap_s  || wrap_t != textureParameters.wrap_t) 
		{

			textureParameters.wrap_s = wrap_s;
			textureParameters.wrap_t = wrap_t;

			if (dx5) {
				lpD3DDevice2->SetRenderState(D3DRENDERSTATE_TEXTUREADDRESSU,wrap_s ? D3DTADDRESS_WRAP  : D3DTADDRESS_CLAMP);
				lpD3DDevice2->SetRenderState(D3DRENDERSTATE_TEXTUREADDRESSV,wrap_t ? D3DTADDRESS_WRAP  : D3DTADDRESS_CLAMP);

			} 


	}
		
#if 0
        if ( wrap_s != textureParameters.wrap_s 
			 || wrap_t != textureParameters.wrap_t) 
		{

			textureParameters.wrap_s = wrap_s;
			textureParameters.wrap_t = wrap_t;

			if (dx5) {
				lpD3DDevice2->SetRenderState(D3DRENDERSTATE_WRAPU,wrap_s);
				lpD3DDevice2->SetRenderState(D3DRENDERSTATE_WRAPV,wrap_t);

			} else {
			OP_STATE_RENDER(2, stateBuf.lpPointer);
				STATE_DATA(D3DRENDERSTATE_WRAPU, wrap_s, stateBuf.lpPointer);
				STATE_DATA(D3DRENDERSTATE_WRAPV, wrap_t, stateBuf.lpPointer);
			}
		}
#endif
}

void RenderState::SetTextureParameters() 
{  
    // textureParameters.glSet();
	textureParameters.putall(stateBuf.lpPointer);
}



void RenderState::SetFog_()
{
	gbool vfog =  TRUE; // ! (caps.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_FOGTABLE);

	// ignore in ramp driver for now 
	if (!canDoRgbLighting) {
		SetFogEnable(FALSE);
		return;
	}


	//for now
	if (vfog &&  (caps.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_FOGVERTEX)) {

			if (dx5) {
				D3DCheck(lpD3DDevice2->SetRenderState(D3DRENDERSTATE_FOGENABLE, fog.enable));

				if (fog.useBackgroundColor)
					{D3DCheck(lpD3DDevice2->SetRenderState(D3DRENDERSTATE_FOGCOLOR, getD3DColor(background))); }
				else { D3DCheck(lpD3DDevice2->SetRenderState(D3DRENDERSTATE_FOGCOLOR, getD3DColor(fog.color))); }
				
				D3DCheck(lpD3DDevice2->SetRenderState(D3DRENDERSTATE_FOGTABLEMODE, D3DFOG_NONE));

				/*
				* Set light state
				*/
				D3DCheck(lpD3DDevice2->SetLightState(D3DLIGHTSTATE_FOGMODE, fog.enable ? fog.mode : D3DFOG_NONE));
				D3DCheck(lpD3DDevice2->SetLightState(D3DLIGHTSTATE_FOGSTART, *(unsigned long*)&fog.start));
				D3DCheck(lpD3DDevice2->SetLightState(D3DLIGHTSTATE_FOGEND, *(unsigned long*)&fog.end));
				D3DCheck(lpD3DDevice2->SetLightState(D3DLIGHTSTATE_FOGDENSITY, *(unsigned long*)&fog.density));
			} else
			{
		
			FlushState(sizeof(D3DINSTRUCTION) * 2 + sizeof(D3DSTATE) * 8);

			OP_STATE_RENDER(3, stateBuf.lpPointer);
				STATE_DATA(D3DRENDERSTATE_FOGENABLE, fog.enable, stateBuf.lpPointer);
				if (fog.useBackgroundColor)
				{ STATE_DATA(D3DRENDERSTATE_FOGCOLOR, getD3DColor(background), stateBuf.lpPointer); }
				else { STATE_DATA(D3DRENDERSTATE_FOGCOLOR, getD3DColor(fog.color), stateBuf.lpPointer); }
				
				STATE_DATA(D3DRENDERSTATE_FOGTABLEMODE, D3DFOG_NONE, stateBuf.lpPointer);

			/*
			 * Set light state
			*/
			OP_STATE_LIGHT(4, stateBuf.lpPointer);
				STATE_DATA(D3DLIGHTSTATE_FOGMODE, fog.enable ? fog.mode : D3DFOG_NONE, stateBuf.lpPointer);
				STATE_DATA(D3DLIGHTSTATE_FOGSTART, *(unsigned long*)&fog.start,stateBuf.lpPointer);
				STATE_DATA(D3DLIGHTSTATE_FOGEND, *(unsigned long*)&fog.end,stateBuf.lpPointer);
				STATE_DATA(D3DLIGHTSTATE_FOGDENSITY, *(unsigned long*)&fog.density,stateBuf.lpPointer);
		}
		fogEnabled = fog.enable;
		return;
	} 

	if ((caps.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_FOGTABLE))
	{
			OP_STATE_RENDER(6, stateBuf.lpPointer);
				STATE_DATA(D3DRENDERSTATE_FOGENABLE, fog.enable, stateBuf.lpPointer);
				if (fog.useBackgroundColor)
				{ STATE_DATA(D3DRENDERSTATE_FOGCOLOR, getD3DColor(background), stateBuf.lpPointer); }
				else { STATE_DATA(D3DRENDERSTATE_FOGCOLOR, getD3DColor(fog.color), stateBuf.lpPointer); }

				STATE_DATA(D3DRENDERSTATE_FOGTABLEMODE, fog.enable ? fog.mode : D3DFOG_NONE, stateBuf.lpPointer);
				// 0 .. 1 
				STATE_DATA(D3DRENDERSTATE_FOGTABLESTART, *(unsigned long*)&fog.start,stateBuf.lpPointer);
				STATE_DATA(D3DRENDERSTATE_FOGTABLEEND, *(unsigned long*)&fog.end,stateBuf.lpPointer);
				STATE_DATA(D3DRENDERSTATE_FOGTABLEDENSITY, *(unsigned long*)&fog.density,stateBuf.lpPointer);

				//D3DRENDERSTATE_RANGEFOGENABLE
				fogEnabled = fog.enable;
				return;
	}

	// turn fog off 
	SetFogEnable(FALSE);

}



void RenderState::SetState()
{

	TRACE("RenderState::SetState() \n");

	// put state attributes
	SetTransforms(projMatrix,viewMatrix,worldMatrix);

	SetTextureOff();  // should be texture of defaultMaterial
    
	SetBackgroundColor();
    //SetLightModel();
    //SetFillMode();
    // set default texture
	FlushState();

    /*
     * Set render state
     */
    OP_STATE_RENDER(24, stateBuf.lpPointer);
      STATE_DATA(D3DRENDERSTATE_SHADEMODE, lightmodel, stateBuf.lpPointer);
      STATE_DATA(D3DRENDERSTATE_TEXTUREPERSPECTIVE, (perspectiveCorrectionHint ? TRUE:FALSE),stateBuf.lpPointer);
      STATE_DATA(D3DRENDERSTATE_ZENABLE, GetZBufferType() /* d3dappi.ThisDriver.bDoesZBuffer*/,stateBuf.lpPointer);
      STATE_DATA(D3DRENDERSTATE_STIPPLEDALPHA, alphaStipple ,stateBuf.lpPointer);
	  STATE_DATA(D3DRENDERSTATE_STIPPLEENABLE, alphaStipple ,stateBuf.lpPointer);
      STATE_DATA(D3DRENDERSTATE_ZWRITEENABLE, zwrite, stateBuf.lpPointer);
      STATE_DATA(D3DRENDERSTATE_ZFUNC, depthFunc, stateBuf.lpPointer);
      STATE_DATA(D3DRENDERSTATE_BLENDENABLE, blend, stateBuf.lpPointer);
      STATE_DATA(D3DRENDERSTATE_ALPHATESTENABLE , alphaTest, stateBuf.lpPointer);
      STATE_DATA(D3DRENDERSTATE_SRCBLEND  , blendSrcFunc, stateBuf.lpPointer);
      STATE_DATA(D3DRENDERSTATE_DESTBLEND , blendDestFunc, stateBuf.lpPointer);
      STATE_DATA(D3DRENDERSTATE_ALPHAFUNC  , alphaFunc, stateBuf.lpPointer);
      STATE_DATA(D3DRENDERSTATE_TEXTUREADDRESS, D3DTADDRESS_WRAP , stateBuf.lpPointer);
      STATE_DATA(D3DRENDERSTATE_WRAPU, FALSE, stateBuf.lpPointer);
      STATE_DATA(D3DRENDERSTATE_WRAPV, FALSE, stateBuf.lpPointer);
      STATE_DATA(D3DRENDERSTATE_TEXTUREMAG, textureParameters.mag_filter,stateBuf.lpPointer);
      STATE_DATA(D3DRENDERSTATE_TEXTUREMIN, textureParameters.min_filter,stateBuf.lpPointer);
      STATE_DATA(D3DRENDERSTATE_TEXTUREMAPBLEND, textureParameters.blend,stateBuf.lpPointer);
      STATE_DATA(D3DRENDERSTATE_FILLMODE, fillMode, stateBuf.lpPointer);
      STATE_DATA(D3DRENDERSTATE_DITHERENABLE, dither, stateBuf.lpPointer);
      STATE_DATA(D3DRENDERSTATE_SPECULARENABLE, specular,stateBuf.lpPointer);
      STATE_DATA(D3DRENDERSTATE_ANTIALIAS, lineSmooth, stateBuf.lpPointer); // ??
      STATE_DATA(D3DRENDERSTATE_FOGENABLE, fog.enable, stateBuf.lpPointer);
      STATE_DATA(D3DRENDERSTATE_FOGCOLOR, getD3DColor(fog.color), stateBuf.lpPointer);


	  /*
		RGBAColor ambient;
		ambient.Gray(0.25);
		ambient.a = 0.25;
	  */
 	 
	 //xxxxxxx ambient.a = ambient.r; // very important for D3D ramp mode !!!!!!!!!!!1

//	FlushState();


	/*
     * Set light state
     */
    OP_STATE_LIGHT(5, stateBuf.lpPointer);
//	  STATE_DATA(D3DLIGHTSTATE_AMBIENT, RGBA_MAKE(40, 40, 40, 40), stateBuf.lpPointer);
	  STATE_DATA(D3DLIGHTSTATE_AMBIENT, getD3DColor(ambient), stateBuf.lpPointer);
      STATE_DATA(D3DLIGHTSTATE_FOGMODE, fog.enable ? fog.mode : D3DFOG_NONE, stateBuf.lpPointer);
      STATE_DATA(D3DLIGHTSTATE_FOGSTART, *(unsigned long*)&fog.start,stateBuf.lpPointer);
      STATE_DATA(D3DLIGHTSTATE_FOGEND, *(unsigned long*)&fog.end,stateBuf.lpPointer);
      STATE_DATA(D3DLIGHTSTATE_FOGDENSITY, *(unsigned long*)&fog.density,stateBuf.lpPointer);

	  
    FlushState();

	if (dx5) {
		D3DCheck(lpD3DDevice2->SetRenderState(D3DRENDERSTATE_COLORKEYENABLE, colorKeyEnable)); // dx5+ only
		D3DCheck(lpD3DDevice2->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend));
		D3DCheck(lpD3DDevice2->SetRenderState(D3DRENDERSTATE_TEXTUREPERSPECTIVE, perspectiveCorrectionHint ? TRUE:FALSE));
		D3DCheck(lpD3DDevice2->SetRenderState(D3DRENDERSTATE_ANTIALIAS, antialisMode));
		D3DCheck(lpD3DDevice2->SetRenderState(D3DRENDERSTATE_TEXTUREADDRESSU,textureParameters.wrap_s ? D3DTADDRESS_WRAP  : D3DTADDRESS_CLAMP));
		D3DCheck(lpD3DDevice2->SetRenderState(D3DRENDERSTATE_TEXTUREADDRESSV,textureParameters.wrap_t ? D3DTADDRESS_WRAP  : D3DTADDRESS_CLAMP));

		D3DCheck(lpD3DDevice2->SetRenderState(D3DRENDERSTATE_ALPHAREF,  alphaRefI));
        D3DCheck(lpD3DDevice2->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, alphaFunc));
	}
    else {
		OP_STATE_RENDER(1, stateBuf.lpPointer);
			STATE_DATA(D3DRENDERSTATE_TEXTUREPERSPECTIVE, perspectiveCorrectionHint ? TRUE:FALSE,stateBuf.lpPointer);
	}
 }



/*
  release all Direct3D structures
*/

void RenderState::Term()
{

    ReleaseLights(); // free all D3D Light Handles
    ReleaseMaterials(); // free all D3D Material Handles
    ReleaseTextures(); // free all D3D Texture Handles
    ReleaseExBufs(); // free all D3D Execute Buffers
	ReleaseVBBufs(); // free all D3D VB Buffers

    if (lpDev) {
	    viewMatrix.Delete(lpDev);
	    projMatrix.Delete(lpDev);
	    worldMatrix.Delete(lpDev);
    }

	TermState();
}

//
// *********** G3MaterialHandle
//
// create a new material handle with a ref cnt of 1 
G3MaterialHandle* RenderState::CreateMaterial()
{
    G3MaterialHandle *m = new G3MaterialHandle();
    if (!materials) materials = m;
    else  {
      m->next = materials->next;
      m->prev =  materials;
      if (materials->next)
        materials->next->prev = m;
      materials->next = m;
    }
    m->refCnt = 1;
    return(m);
}

// delete an no longer userd material
void RenderState::DeleteMaterial(G3MaterialHandle *m)
{
    if (m == materials) {  // must be first
        materials= m->next;
        if (materials) materials->prev = NULL;
    } else {
      m->prev->next = m->next;
      if (m->next) m->next->prev = m->prev;
    }
	m->Release(); // done by destructor for savety too 
    delete m;
}

// release all D3D Material handles
void RenderState::ReleaseMaterials()
{
    G3MaterialHandle *i= materials;
	int cnt=0;
    
    while(i != NULL) 
    {
        i->Release();
        i= i->next;
		cnt++;
    }
	TRACE("%d materials in use \n",cnt);
	backgroundMaterial.Release();
	defaultMaterial.Release();
	currentMaterial = NULL;
	materialChanged =gtrue;
}

void RenderState::ReleaseTexturedMaterials()
{
    G3MaterialHandle *i= materials;
    
    while(i != NULL) 
    {
        if (i->GetTextureHandle() !=0)
            i->Release();
        i= i->next;
    }
	backgroundTexture.Release();
	currentMaterial = NULL;
	materialChanged =gtrue;

}

// Delete all D3D Material handles
// should only be called if all references deleted !!!!!!!!!!!!1
void RenderState::DeleteMaterials()
{
    G3MaterialHandle *i = materials,*next;
    
    while(i != NULL) 
    {
		ASSERT(i->refCnt>=0);
        next = i->next;
        delete i;
        i = next;
    }
    materials = NULL;
}

// Delete all D3D Material handles
// should only be called if all references deleted !!!!!!!!!!!!1
void RenderState::DeleteUnusedMaterials()
{
    G3MaterialHandle *i= materials,*next;
    
    while(i != NULL) 
    {
        next = i->next;
		ASSERT(i->refCnt>=0);

        if (i->refCnt==0) {
            //DeleteMaterial(i);
			if (i == materials) {  // must be first
				materials = next;
				if (materials) materials->prev = NULL;
			} else {
				i->prev->next = next;
				if (next) i->next->prev = i->prev;
			}
			i->Release(); // done by destructor for savety too 
			delete i;
		}
        i = next;
    }
	currentMaterial = NULL;
	materialChanged =gtrue;
}


 static float white[3]= { 1.0,1.0,1.0 } ;

 // Set Material
 gbool RenderState::SetMaterial(G3MaterialRef &ref,			
            // material parameters
			float ambient[3],float diffuseCur[3],float specular[3],float emissive[3],
			float alpha,float power,gbool whiteDiffuse)
 {

	 float *diffuse = diffuseCur;
	if (whiteDiffuse) diffuse= white;
    currentColor.Set(diffuse[0],diffuse[1],diffuse[2],alpha);


	// xxxxxxxxxxxxxxxxx
	 if (doOwnLighting) {
        if (ref.handle == NULL)
            ref.handle = CreateMaterial();

		SetLitMaterial(TRUE);

	    if (ref.handle->Set(ambient,diffuse,specular,emissive,alpha,power))
		{
                // put handle to material in state buffer
				//TRACE("Set Material Mat = %d  Tex = %d W= %d \n",ref.handle->hMat,ref.handle->hTexture,whiteDiffuse);
				ref.handle->whiteDiffuse = whiteDiffuse;
				currentMaterial = ref.handle;
				materialChanged=gtrue;

		}
		return gtrue;

	 }		

	 // use D3D Material 
	    float *specularNew = specular;

		float specularRamp[3];

        if (ref.handle == NULL)
            ref.handle = CreateMaterial();

	   

#if 1
		// 25.07.97

// problem lit textured faces are bright
// D3 Ramp seems to recognize if specular is all black
// to turn off specular lighting
// otherwise specular shading dosent include the diffuse base 
		if (!canDoRgbLighting && (specular[0]>0.0f || specular[1]>0.0f || specular[2]>0.0f) ) {
			// ramp driver using special model for specular
			// standard = additive, make shure specular doesn´t fall below diffuse & emissive

			specularRamp[0] = max(diffuse[0],specular[0]);
			specularRamp[1] = max(diffuse[1],specular[1]);
			specularRamp[2] = max(diffuse[2],specular[2]);
			specularRamp[0] = max(emissive[0],specularRamp[0]);
			specularRamp[1] = max(emissive[1],specularRamp[1]);
			specularRamp[2] = max(emissive[2],specularRamp[2]);

//			specularRamp[0] = diffuse[0]+specular[0];
			if (specularRamp[0]>=1.0f) specularRamp[0]=1.0f; 

//			specularRamp[1] = diffuse[1]+specular[1];
			if (specularRamp[1]>=1.0f) specularRamp[1]=1.0f; 
			
//			specularRamp[2] = diffuse[2]+specular[2];

			if (specularRamp[2]>=1.0f) specularRamp[2]=1.0f;

			specularNew = specularRamp;
	   }
#endif


   	   SetLitMaterial(TRUE);

       // set material properties
	   if (ref.handle && ref.handle->Set(lpD3D,lpDev,ambient,diffuse,specularNew,emissive,alpha,power,
			materialRampSize,hTex)
			) {
                // put handle to material in state buffer
				//TRACE("Set Material Mat = %d  Tex = %d W= %d \n",ref.handle->hMat,ref.handle->hTexture,whiteDiffuse);
				ref.handle->whiteDiffuse = whiteDiffuse;
				if (dx6) lpD3DDevice3->SetLightState(D3DLIGHTSTATE_MATERIAL, ref.handle->hMat);
                else ref.handle->put(stateBuf.lpPointer);
                return(TRUE);
       } else 
		 return(FALSE);
  }

// Set Material to solid color
// set flag, so that we can create an unlit execute buffer 
gbool RenderState::SetMaterial(G3MaterialRef &ref,const RGBAColor &color)
{
    if (ref.handle == NULL)
        ref.handle = CreateMaterial();

    SetCurrentColor(color);

	SetLitMaterial(FALSE);

	// xxxxxxxxxxxxxxxxx
	 if (doOwnLighting) {
		ref.handle->SetSolid(color);
		ref.handle->whiteDiffuse = 0;
		currentMaterial = ref.handle;
		materialChanged=gtrue;
		return gtrue;
	 }		


       // set material properties
	   if (ref.handle && ref.handle->SetSolid(lpD3D,lpDev,color,hTex)) {
              // put handle to material in state buffer
			  // TRACE("Set Material solid  comp %d %d \n",ref.handle->hMat,ref.handle->hTexture);
              if (dx6) lpD3DDevice3->SetLightState(D3DLIGHTSTATE_MATERIAL, ref.handle->hMat);
              else ref.handle->put(stateBuf.lpPointer);
              return(TRUE);
       } else 
		 return(FALSE);
	}

// set material properties
gbool RenderState::SetMaterial(G3MaterialRef &ref,D3DMATERIAL &mat) 
{
    if (ref.handle == NULL)
       ref.handle = CreateMaterial();

	 gbool w = texturing && (hTex !=0) && textureParameters.set_diffuse_white;

	 // xxxxxxxxxxxxxxxxx
	 if (doOwnLighting) {
	    SetCurrentColor(mat.diffuse); // ??
		ref.handle->material.Set(mat);
		// ref.handle->material.diffuse = mat.diffuse;
		ref.handle->whiteDiffuse = 0;
		currentMaterial = ref.handle;
		materialChanged=gtrue;
		return gtrue;
	 }		

	   
	   // mat.hTexture = hTex;

	   if (w) return(SetMaterialW(ref,mat));
	
       // set material properties
	   if (ref.handle && ref.handle->Set(lpD3D,lpDev,mat)) {
           // put handle to material in state buffer
           		if (dx6) lpD3DDevice3->SetLightState(D3DLIGHTSTATE_MATERIAL, ref.handle->hMat);
                else ref.handle->put(stateBuf.lpPointer);
		   //TRACE("Set Material  %p %d %d \n",ref.handle, ref.handle->hMat,ref.handle->hTexture);
           return(TRUE);
       } else 
		return(FALSE);
    }

	// set material, but diffuse is white 
	gbool RenderState::SetMaterialW(G3MaterialRef &ref,D3DMATERIAL &mat) 
    {
        if (ref.handle == NULL)
            ref.handle = CreateMaterial();

	   D3DCOLORVALUE save = mat.diffuse;

	   mat.diffuse.r = 1.0;
	   mat.diffuse.g = 1.0;
	   mat.diffuse.b = 1.0;

	 // xxxxxxxxxxxxxxxxx
	 if (doOwnLighting) {
	    SetCurrentColor(mat.diffuse); // ??
		ref.handle->material.Set(mat);
		ref.handle->whiteDiffuse = 1;
		currentMaterial = ref.handle;
		materialChanged=gtrue;
	    mat.diffuse = save;
		return gtrue;
	 }		



       // set material properties
	   if (ref.handle && ref.handle->Set(lpD3D,lpDev,mat)) {
           	if (dx6) lpD3DDevice3->SetLightState(D3DLIGHTSTATE_MATERIAL, ref.handle->hMat);
            else            // put handle to material in state buffer
				ref.handle->put(stateBuf.lpPointer);
		   mat.diffuse = save;
           return(TRUE);
       } else { 
		   mat.diffuse = save;
		   return(FALSE);
	   }
    }






// Helpers

// Create a material
gbool CreateMaterial(LPDIRECT3D lpD3D,
			LPDIRECT3DDEVICE lpDev, 
			// material parameters
			float ambient[3],float diffuse[3],float specular[3],float emissive[3],float alpha,float power,
			// D3D specific
			DWORD rampSize /* = 16 */, 
			D3DTEXTUREHANDLE hTex,  /* Handle for the texture on the material.*/
			/* out / in  must be released at end */
			LPDIRECT3DMATERIAL &lpmat,   /* Material object */
			/* out */
			D3DMATERIALHANDLE &hMat /* Handle for the material  */
			)

{
	D3DLASTERROR()

	// create material if not allocated    
	if (!lpmat) {
		 if (lpD3D->CreateMaterial(&lpmat, NULL) != D3D_OK)
			return FALSE;
		D3DCheck(lpmat->GetHandle(lpDev, &hMat));
	}

	
	D3DMATERIAL mat;

    mat.dwSize = sizeof(D3DMATERIAL);
    mat.diffuse.r = (D3DVALUE)diffuse[0];
    mat.diffuse.g = (D3DVALUE)diffuse[1];
    mat.diffuse.b = (D3DVALUE)diffuse[2];
    mat.diffuse.a = (D3DVALUE)alpha;
    mat.ambient.r = (D3DVALUE)ambient[0];
    mat.ambient.g = (D3DVALUE)ambient[1];
    mat.ambient.b = (D3DVALUE)ambient[2];
    mat.ambient.a = (D3DVALUE)alpha;
    mat.specular.r = (D3DVALUE)specular[0];
    mat.specular.g = (D3DVALUE)specular[1];
    mat.specular.b = (D3DVALUE)specular[2];
    mat.specular.a = (D3DVALUE)alpha;
    mat.emissive.r = (D3DVALUE)emissive[0];
    mat.emissive.g = (D3DVALUE)emissive[1];
    mat.emissive.b = (D3DVALUE)emissive[2];
    mat.emissive.a = (D3DVALUE)alpha;
    mat.power = power;
    mat.dwRampSize = rampSize;
    mat.hTexture = hTex;
    D3DCheck(lpmat->SetMaterial(&mat));
    //D3DCheck(lpmat->GetHandle(lpDev, &hMat));
	return TRUE;
}

gbool CreateSolidMaterial(LPDIRECT3D lpD3D,
			LPDIRECT3DDEVICE lpDev, 

			// material parameters
            const RGBAColor &color,
			// texture handle
			D3DTEXTUREHANDLE hTex,

			/* out / in  must be released at end */
			LPDIRECT3DMATERIAL &lpmat,   /* Material object */
			/* out */
			D3DMATERIALHANDLE &hMat /* Handle for the material  */
			)

{
   	D3DLASTERROR()
   // create material if not allocated    
	if (!lpmat) {
		if (lpD3D->CreateMaterial(&lpmat, NULL) != D3D_OK) 
			return FALSE;
		D3DCheck(lpmat->GetHandle(lpDev, &hMat));
	}
	
    D3DMATERIAL mat;

    memset(&mat, 0, sizeof(D3DMATERIAL));
    mat.dwSize = sizeof(D3DMATERIAL);
/*
    mat.diffuse.r = (D3DVALUE)diffuse[0];
    mat.diffuse.g = (D3DVALUE)diffuse[1];
    mat.diffuse.b = (D3DVALUE)diffuse[2];
*/
    mat.ambient = mat.diffuse = color;
    mat.dwRampSize = 1;
	mat.hTexture = hTex;

    D3DCheck(lpmat->SetMaterial(&mat));
    // D3DCheck(lpmat->GetHandle(lpDev, &hMat)); get each time a new material ?  // 10.05.972 removed
	return TRUE;
}


//
// *********** G3LightHandle
//
// create a new light handle with a ref cnt of 1 
G3LightHandle* RenderState::CreateLight()
{
    G3LightHandle *m = new G3LightHandle();
    if (!lights) lights = m;
    else  {
      m->next = lights->next;
      m->prev =  lights;
      if (lights->next)
        lights->next->prev = m;
      lights->next = m;
    }
    m->refCnt = 1;
    return(m);
}

// delete a no longer used light
void RenderState::DeleteLight(G3LightHandle *m)
{
    if (m == lights) {  // must be first
        lights= m->next;
        if (lights) lights->prev = NULL;
    } else {
      m->prev->next = m->next;
      if (m->next) m->next->prev = m->prev;
    }

	TRACE("del light %d refCnt %d %p %p \n",m->enabled,m->refCnt,m->lpD3DLight,m->lpView); // debug

    if (lpView) m->Release(lpView);
    delete m;
}

// release all D3D Light handles and remove from view 
void RenderState::ReleaseLights()
{
    G3LightHandle *i= lights;
	int cnt=0;
    
    while(i != NULL) 
    {	
		TRACE("Rel light %d refCnt %d %p %p \n",i->enabled,i->refCnt,i->lpD3DLight,lpView); // debug
        i->Release(lpView);
        i= i->next;
		cnt++;
    }
	TRACE("%D3D Release lights %d lights in use \n",cnt);
}

// Delete all D3D Light handles
// should only be called if all references deleted !!!!!!!!!!!!
void RenderState::DeleteLights()
{
    G3LightHandle *i = lights,*next;
    
    while(i != NULL) 
    {
        next = i->next;
        if (lpView) i->Release(lpView);
        delete i;
        i = next;
    }
    lights = NULL;
}

// Delete all unused D3D Light handles
void RenderState::DeleteUnusedLights()
{
    G3LightHandle *i= lights,*next;
    while(i != NULL) 
    {
        next = i->next;
        if (i->refCnt==0)
            DeleteLight(i);
        i = next;
    }
}

// Set Light, define data and add to viewport 
gbool RenderState::SetLight(G3LightRef &ref) 
{	gbool ret = FALSE;
    if (ref.handle == NULL) return FALSE; // not defined

	if (doOwnLighting) {
		int f=0;

		if (ref.handle->ambientIntensity > 0.0f)
			f |= GL_AMBIENT;
		if (ref.handle->dvRange > 0.0f)
			f |= GL_RANGE;
		if ((ref.handle->dvAttenuation0 != 1.0f) && (ref.handle->dvAttenuation1 != 0.0f) && (ref.handle->dvAttenuation2 != 0.0f))
			f |= GL_ATTENUATION;

		ref.handle->flags =f;
		ref.handle->enabled = gtrue;
		lightsTransformed=gfalse;
		lightsTransformedView=gfalse;
		materialChanged=gtrue;

		ref.handle->cosTheta = cos(ref.handle->dvTheta);
		ref.handle->cosPhi = cos(ref.handle->dvPhi);
		float x = (ref.handle->cosTheta - ref.handle->cosPhi);
		if (x!= 0.0f) x = 1.0f / x;
		// ref.handle->thetaScale = x;

		return TRUE;
	}

	if (ret = ref.handle->Set(lpD3D)) {
	   if (!ref.handle->enabled) 
		   ret = ref.handle->Add(lpView);
	}
	UpdateAmbientLight();
	return ret; 
}

// add/Enable  light at stack index I 
gbool RenderState::EnableLight(int index, G3LightRef &ref)
{	
	gbool ret = FALSE;
    if (ref.handle == NULL) return FALSE; // not defined

	if (doOwnLighting) {
		lightsTransformed=gfalse;
		lightsTransformedView=gfalse;

		materialChanged=gtrue;

		if (lightStack.Length() <= index) lightStack.SetLength(index+1);
		lightStack[index]=ref.handle;
		ref.handle->enabled = TRUE;

		return TRUE;
	}

//	if (ret = ref.handle->Set(lpD3D)) {
	   if (!ref.handle->enabled) 
		   ret = ref.handle->Add(lpView);
//	}
    
	if (lightStack.Length() <= index) lightStack.SetLength(index+1);

	lightStack[index]=ref.handle;
	
	UpdateAmbientLight();


	return ret; 
}
	
// delete light at stack index I 
gbool RenderState::DisableLight(int index)
{
	gbool ret = FALSE;

	if (index >= lightStack.Length()) return ret; // safety, happened with VRML 1.0 files

	G3LightHandle *light = lightStack[index];
	if (doOwnLighting) {
	    if (light && light->enabled) 
			light->enabled = FALSE;

		lightStack[index]=NULL;
		return TRUE;
	}


    if (light && light->enabled) {
	    ret = light->Delete(lpView);
		UpdateAmbientLight();
	}

	lightStack[index]=NULL;

	return ret; 
}

// recompute ambient light from all lights

void RenderState::UpdateAmbientLight()
{
	
	if (!canDoRgbLighting)  { 
		ambient.r = ambient.g = ambient.b = ambient.a = 0.0f;
		SetAmbientColor();
		return; // bad in Ramp Mode
	}

	G3LightHandle *l=lights;
	ambient.Black();

	while (l) {
		if (l->enabled) {
			ambient += l->ambientColor;
		}
		l=l->next;
	}
	ambient.r = min(1.0f,ambient.r);
	ambient.g = min(1.0f,ambient.g);
	ambient.b = min(1.0f,ambient.b);
	ambient.a = min(1.0f,ambient.a);
	ambient.a =0.0f;

	SetAmbientColor();
}




//
//	 G3TextureHandle
//

//
// compute target texture format size 
//
void RenderState::GetTextureTargetSize(int &width,int &height,int roundUpLimit)
{
	get_image_2power(width,height,
		textureParameters.min_size_x,textureParameters.min_size_y,
		textureParameters.current_max_size_x,textureParameters.current_max_size_y,
		textureParameters.square_size,roundUpLimit);

}



// create a new texture handle with a ref cnt of 1 
G3TextureHandle* RenderState::CreateTexture()
{
    G3TextureHandle *m = new G3TextureHandle();
    if (!textures) textures = m;
    else  {
      m->next = textures->next;
      m->prev =  textures;
      if (textures->next)
          textures->next->prev = m;
      textures->next = m;
    }
    m->refCnt = 1;
    return(m);
}

// delete an no longer used Texture
void RenderState::DeleteTexture(G3TextureHandle *m)
{
    if (m == textures) {  // must be first
        textures= m->Next();
        if (textures) textures->prev = NULL;
    } else {
      m->prev->next = m->next;
      if (m->next) m->next->prev = m->prev;
    }
	m->Release(); // done by destructor for savety too 
    delete m;
}

// release all Texture handles
void RenderState::ReleaseTextures()
{
    G3TextureHandle *i= textures;
    
    while(i != NULL) 
    {
        i->Release();
		i->rendered = FALSE;
        i= i->Next();
    }
	allTexturesInVideo = TRUE;
	hTex = 0; 
	backgroundTexture.Release(); // new 
	defaultTexture.Release();

}

// release textures not rendered in the last frame
gbool RenderState::ReleaseUnrenderedTextures(int max)
{
	gbool ret = FALSE;
    G3TextureHandle *i= textures;
    
    while(i != NULL) 
    {
		if (!i->rendered)
		{
			if (i->Ok() && i->CheckInVideoMem()) {
				i->Release();
				ret = TRUE;
				max--;
				if (max <0) return ret;
			}
			else i->Release();
		}
		i->rendered = FALSE;
        i= i->Next();
    }

	hTex = 0;  
	return ret;
}

// release large textures 
gbool RenderState::ReleaseLargeTextures(int max)
{
	gbool ret = FALSE;

    G3TextureHandle *i= textures;
    
    while(i != NULL) 
    {
		if (i->textureFormat == rgb24Format) { // 24 ==> 16
			if (i->Ok() && i->CheckInVideoMem()) {
				i->Release();
				ret = TRUE;
				max--;
				if (max <0) return ret;
			}
			else i->Release();
		}
		else 
		if ((i->sizeX > textureParameters.current_max_size_x)
		 || (i->sizeY > textureParameters.current_max_size_y))
		{
			if (i->Ok() && i->CheckInVideoMem()) {
				i->Release();
				ret = TRUE;
				max--;
				if (max <0) return ret;
			}
			else i->Release();
		}
		else if ((i->textureFormat == rgba32Format) && (rgbaFormat == rgba16Format)) {
			if (i->Ok() && i->CheckInVideoMem()) {
				i->Release();
				ret = TRUE;
				max--;
				if (max <0) return ret;
			}
			else i->Release();
		}
	 	else if ((i->textureFormat == rgb16Format) && (rgbFormat == -1)) {
			if (i->Ok() && i->CheckInVideoMem()) {
				i->Release();
				ret = TRUE;
				max--;
				if (max <0) return ret;
			}
			else i->Release();
		}
	 

        i= i->Next();
    }

	hTex = 0; 
	return ret;
}


// Delete all Texture handles
// should only be called if all references deleted !!!!!!!!!!!!1
void RenderState::DeleteTextures()
{
    G3TextureHandle *i = textures,*next;
    
    while(i != NULL) 
    {
        next = i->Next();
        delete i;
        i = next;
    }
    textures = NULL;
}

// Delete all unused Texture handles
gbool RenderState::DeleteUnusedTextures()
{
    G3TextureHandle *i= textures,*next;
	gbool ret = FALSE;
    
    while(i != NULL) 
    {
        next = i->Next();
        if (i->refCnt==0) {
			if (i->Ok() && i->CheckInVideoMem()) {
				ret = TRUE;
			}

            DeleteTexture(i);
		}
        i = next;
    }
	return ret;
}

// texture memory overflow 
// if result TRUE, try to reload texture in progress
gbool RenderState::OnTextureMemoryOverflow(GTextureHandle &overflowHandle)
{	int sx=0,sy=0,bpp;
	
	TRACE(" RenderState::OnTextureMemoryOverflow \n");

	if (DeleteUnusedTextures()) {
		TRACE("Found unused texture \n");
		return TRUE;
	}
	
	
	if ( (rgbFormat == rgb24Format) && (rgb24Format>=0) && (rgb16Format >= 0)) {
		// lets go to a lowres Texture Format
		rgbFormat = rgb16Format;
		TRACE("D3D reducing RGB texture quality to 16 bit. \n");
	}
	else if ( (rgbaFormat == rgba32Format) && (rgba32Format>=0) && (rgba16Format >= 0)) {
		// lets go to a lowres Texture Format
		rgbaFormat = rgba16Format;
		TRACE("D3D reducing RGBA texture quality to 16 bit. \n");
	}
		
	GetMaxUsedTextureSize(sx,sy,bpp);

	sx = max((short)sx,overflowHandle.sizeX);
	sy = max((short)sy,overflowHandle.sizeY);

	if (ReleaseLargeTextures(2)) 
		return TRUE;


	if (textureParameters.current_max_size_x <256 && palette8Format>=0 && rgbFormat >=0) {
		rgbFormat = -1;
		TRACE("D3D reducing RGB texture quality to 8 bit. \n");
		if (ReleaseLargeTextures(1)) 
			return TRUE;

		// mm driver could emulate 8-Bit by 16 bit, so no memory save ???

	}

	if (!doTextureDownscale) 
		return FALSE;


	// reduce texture size 
	if ((sx>textureParameters.min_size_x) || (sy>textureParameters.min_size_y)) {
		int s=max(sx,sy);
		s = s/2;
		textureParameters.SetCurrentMaxSize(min(s,textureParameters.max_size_x),min(s,textureParameters.max_size_y));
		TRACE("New max texture size is %d %d \n",textureParameters.current_max_size_x,textureParameters.current_max_size_y);
		//if (overflowHandle.sizeX > textureParameters.current_max_size_x) return TRUE; // try with new size 
		if (ReleaseLargeTextures(1)) // was 2
			return TRUE;
	}
	allTexturesInVideo = FALSE;
	// to think: need to reinit texture formats after scene change ?????
	return FALSE;

}

gbool RenderState::GetMaxUsedTextureSize(int &sizeX,int &sizeY, int &bytesPerPix)
{
	gbool ret = FALSE;

    G3TextureHandle *i= textures;
    
    while(i != NULL) 
    {
		if (i->Ok()) {
			sizeX = max((short)sizeX,i->sizeX);
			sizeY = max((short)sizeY,i->sizeY);
			ret = TRUE;
		}
        i= i->Next();
    }

	return ret;
}




// set Texture properties
gbool RenderState::SetTexture(GTextureRef &ref,
								LPCTSTR fileName,
								LPCTSTR fileUrl,
							    int format,gbool reload) 
{
	ref.Release(); // new 1.07.99 hg

	if (ref.handle == NULL) {
			// find if a texture with this file name exists 
		    G3TextureHandle *i= textures;
	        G3TextureHandle *found =  NULL;
	        G3TextureHandle *firstFree =  NULL;
    
			while(i != NULL) 
			{
				if (i->MatchFileName(fileName)) {
					found = i;
					break;
				} else
				
				if (i->refCnt == 0)
					firstFree = i;

				i = i->Next();
			}

			if (found) {
				ref.Release();

				ref.handle = found;
				found->Ref();	 // inc ref count 

				//SetTexture(ref.handle->hTexture);
			
				TRACE("Sharing texture %s \n",fileName);

				if (reload) {
					G3TextureHandle *h = (G3TextureHandle *) ref.handle; // downcast  
					if (h->Set(*this,fileName,format,fileUrl)) return TRUE;
					return FALSE;
				}

				return(TRUE);
			}

		    // create a new one 
			if (firstFree) {
				ref.handle = firstFree;
				firstFree->Ref(); 
			}
            else ref.handle = CreateTexture();
		}
	   
	   G3TextureHandle *h = (G3TextureHandle *) ref.handle; // downcast  
       // set Texture properties
	   if (h && h->Set(*this,fileName,format,fileUrl)) {
           //xxx SetTexture(ref.handle->hTexture);
           return(TRUE);
       } else {
		   //xx SetTexture(0);
		   return(FALSE);
	   }
}

// find if a texture with this URL already loaded 
gbool RenderState::FindTextureByUrl(GTextureRef &ref,LPCTSTR fileUrl)
{
    //if (ref.handle == NULL) 

	{
			// find if a texture with this file name exists 
		    G3TextureHandle *i= textures;
	        G3TextureHandle *found =  NULL;
	        G3TextureHandle *firstFree =  NULL;
    
			while(i != NULL) 
			{
				if (i->MatchFileUrl(fileUrl)) {
					found = i;
					break;
				} else
				
				if (i->refCnt == 0)
					firstFree = i;

				i = i->Next();
			}

			if (found) {
				
				ref.Release();

				ref.handle = found;
				found->Ref();	 // inc ref count 
				TRACE("Sharing texture by url %s \n",fileUrl);

				return(TRUE);
			}

	}
	return FALSE;
}



// set by image data 
gbool RenderState::SetTexture(GTextureRef &ref,int glFormat,int width,int height,int numComponents,unsigned char *data)
{

	if (ref.handle) {
		ref.Release(); // unref old 
	}
	// create a new one 
    if (!ref.handle)
	    ref.handle = CreateTexture();
	
	G3TextureHandle *h = (G3TextureHandle *) ref.handle; // downcast  
    // set Texture properties
	if (h && h->Set(*this,glFormat,width,height,numComponents,data)) {
        SetTexture(h->hTexture);
        return(TRUE);
    } else {
	    SetTextureOff();
	    return(FALSE);
    }
}

// set texture by image structure 
gbool RenderState::SetTexture(GTextureRef &ref,image *im,gbool checkScale)
{
	// crash in ramp-driver
	// to do: optimize path
	G3TextureHandle *h = (G3TextureHandle *) ref.handle; // downcast  

	if (h) {
	    if(h->hTexture == hTex) 
			SetTextureOff();
		h->Release();
		//ref.Release(); // unref old  no crash, but mem-leak
	}
	// create a new one 
    if (!ref.handle)
		ref.handle = CreateTexture();

	h = (G3TextureHandle *) ref.handle; // downcast  

    // set Texture properties
	if (h && h->Set(*this,im,checkScale)) {
        SetTexture(h->hTexture);
        return(TRUE);
    } else {
	    SetTextureOff();
	    return(FALSE);
    }
}

// set texture by DD surface 
gbool RenderState::SetTexture(GTextureRef &ref,LPDIRECTDRAWSURFACE srcSurface,IDirectDrawPalette *srcPalette,
							  DDSURFACEDESC &srcDdsd, RECT &srcRect,
							  gbool checkScale,RECT *targetSizeRect
							  )
{
	// crash in ramp-driver
	// to do: optimize path
	G3TextureHandle *h = (G3TextureHandle *) ref.handle; // downcast  

	if (h) {
	    if(h->hTexture == hTex) 
			SetTextureOff();
		ref.handle->Release();
		//ref.Release(); // unref old  no crash, but mem-leak
	}
	// create a new one 
    if (!ref.handle)
		ref.handle = CreateTexture();

	h = (G3TextureHandle *) ref.handle; // downcast  

    // set Texture properties
	if (h  && h->LoadTextureSurf(device,*this,srcSurface,srcPalette,srcDdsd,srcRect,targetSizeRect)) {
		SetTexture(h->hTexture);
        return(TRUE);
    } else {
	    SetTextureOff();
	    return(FALSE);
    }
}


//
// *********** G3ExBufHandle
//

// create a new ExBuf handle with a ref cnt of 1 
G3ExBufHandle* RenderState::CreateExBuf()
{
    G3ExBufHandle *m = new G3ExBufHandle();
    if (!exBufs) exBufs = m;
    else  {
      m->next = exBufs->next;
      m->prev =  exBufs;
      if (exBufs->next)
        exBufs->next->prev = m;
      exBufs->next = m;
    }
    m->refCnt = 1;
    return(m);
}

// delete an no longer used ExBuf
void RenderState::DeleteExBuf(G3ExBufHandle *m)
{
    if (m == exBufs) {  // must be first
        exBufs= m->next;
        if (exBufs) exBufs->prev = NULL;
    } else {
      m->prev->next = m->next;
      if (m->next) m->next->prev = m->prev;
    }
    delete m;
}

// release all D3D ExBuf handles
void RenderState::ReleaseExBufs()
{
    G3ExBufHandle *i= exBufs;
    
    while (i != NULL) 
    {
        i->Release();
        i= i->next;
    }
}

// Delete all D3D ExBuf handles
// should only be called if all references deleted !!!!!!!!!!!!
void RenderState::DeleteExBufs()
{
    G3ExBufHandle *i = exBufs,*next;
    
    while (i != NULL) 
    {
        next = i->next;
        delete i;
        i = next;
    }
    exBufs = NULL;
}

// Delete all D3D ExBuf handles
// should only be called if all references deleted !!!!!!!!!!!!1
void RenderState::DeleteUnusedExBufs()
{
    G3ExBufHandle *i= exBufs,*next;
    
    while(i != NULL) 
    {
        next = i->next;
        if (i->refCnt==0)
            DeleteExBuf(i);
        i = next;
    }
}



//-----------------------//-----------------------//-----------------------
//								vertex buffer
//-----------------------//-----------------------//-----------------------

// create a new VBBufs handle with a ref cnt of 1 and attach to bufRef
gbool RenderState::CreateVBBuf(G3VBBufRef &bufRef,DWORD dwFVF, int iNumVerts)
{	 

	G3VBBufHandle *hVBBuf = new G3VBBufHandle();
	
	ZeroMemory(&hVBBuf->vbdesc, sizeof(D3DVERTEXBUFFERDESC));
	hVBBuf->vbdesc.dwSize= sizeof(D3DVERTEXBUFFERDESC);
	hVBBuf->vbdesc.dwCaps        = 0L;
	hVBBuf->vbdesc.dwFVF         |= dwFVF;
	hVBBuf->vbdesc.dwNumVertices = iNumVerts;

	// If this isn't a hardware device, make sure the 
	// vertex buffer uses system memory.
	//if( !fIsAHardwareDevice )
	hVBBuf->vbdesc.dwCaps |= D3DVBCAPS_SYSTEMMEMORY;
			
	// Create a clipping-capable vertex buffer.
	if(FAILED(lpD3D3->CreateVertexBuffer(&hVBBuf->vbdesc,
										&hVBBuf->lpD3DVBBuf, 0L,
										NULL))) 
	{
		delete hVBBuf;
		return gfalse; 
	}
	//update list
	if (!VBBufs) 
		VBBufs = hVBBuf;
    else  
	{
      hVBBuf->next = VBBufs->next;
      hVBBuf->prev =  VBBufs;
      if (VBBufs->next)
        VBBufs->next->prev = hVBBuf;
      VBBufs->next = hVBBuf;
    }

	// attach to pointer ref
    bufRef.Attach(hVBBuf);

    return(gtrue);
}



// release all D3D VBBuf 
void RenderState::ReleaseVBBufs()
{
    G3VBBufHandle *i= VBBufs;
    
    while (i != NULL) 
    {
        i->ReleaseBuffer();
        i= i->next;
    }
}




// Delete all D3D VBBuf handles
// should only be called if all references deleted !!!!!!!!!!!!
/*
void RenderState::DeleteVBBufs()
{
    G3VBBufHandle *i = VBBufs,*next;
    
    while (i != NULL) 
    {
        next = i->next;
        delete i;
        i = next;
    }
    VBBufs = NULL;
}
*/

// Delete all D3D VBBuf handles
// should only be called if all references deleted !!!!!!!!!!!!
void RenderState::DeleteUnusedVBBufs()
{
    G3VBBufHandle *i= VBBufs;
    G3VBBufHandle *next;

    while(i != NULL) 
    {
        next = i->next;
		
		if (i->refCnt==0)
		{
			//update list
				if (i == VBBufs)// first element 
				{  
					VBBufs= next;
					if (VBBufs) 
						VBBufs->prev = NULL;
				} 
				else 
				{
				  i->prev->next = next;
				  if (i->next) 
					  i->next->prev = i->prev;
				}
				delete i;
		}
        i = next;
    }
}

/*

// delete an no longer used VBBuf
void RenderState::DeleteVBBuf(G3VBBufHandle *m)
{
    if (m == VBBufs) 
	{  // must be first
        VBBufs= m->next;
        if (VBBufs) VBBufs->prev = NULL;
    } 
	else 
	{
      m->prev->next = m->next;
      if (m->next) m->next->prev = m->prev;
    }
    delete m;
}
*/

//-----------------------//-----------------------//-----------------------
//							 g3TextureParameters
//-----------------------//-----------------------//-----------------------

G3TextureParameters::G3TextureParameters() 
{
	min_filter = D3DFILTER_NEAREST;
	mag_filter = D3DFILTER_NEAREST;
	wrap_s = TRUE;
	wrap_t = TRUE;
	gen_mipmaps=0;
	set_diffuse_white = 1;
	max_size_x = 1024, max_size_y= 1024;
	current_max_size_x = 1024, current_max_size_y= 1024;
	min_size_x = 1, min_size_y= 1;
	square_size = FALSE;
	border = FALSE;

    //perspective = TRUE;
    blend = D3DTBLEND_DECAL;
    defaultBlend = D3DTBLEND_MODULATE;
    unlitBlend = D3DTBLEND_DECAL; // copy mode 
	supported_image_fmts = (1<<IM_RGB) | (1<<IM_MAPPED8); // to be initialized depending on texture formats
};

int G3TextureParameters::SetMaxSize(int size_x,int size_y)
{	int iPow2;

	max_size_x = size_x;
	
	iPow2 = log((double)size_x) / log((double)2.0);

    max_size_x = 1 << iPow2;

	max_size_y = size_y;
	iPow2 = log((double)size_y) / log((double)2.0);
    max_size_y = 1 << iPow2;

	current_max_size_x=max_size_x;
	current_max_size_y=max_size_y;

	return(1);
}

int G3TextureParameters::SetCurrentMaxSize(int size_x,int size_y)
{	int iPow2;

	int max_size_x,max_size_y;

	max_size_x = size_x;
	
	iPow2 = log((double)size_x) / log((double)2.0);

    max_size_x = 1 << iPow2;

	max_size_y = size_y;
	iPow2 = log((double)size_y) / log((double)2.0);
    max_size_y = 1 << iPow2;

	current_max_size_x=min(this->max_size_x,max_size_x);
	current_max_size_y=min(this->max_size_y,max_size_y);

	return(1);
}

void G3TextureParameters::GetMaxSize(int &size_x,int &size_y)
{ 
	size_x = max_size_x;size_y = max_size_y;
}

void G3TextureParameters::GetCurrentMaxSize(int &size_x,int &size_y)
{ 
	size_x = current_max_size_x;size_y = current_max_size_y;
}

int G3TextureParameters::SetMinSize(int size_x,int size_y)
{	int iPow2;

	if (size_x > max_size_x) size_x = max_size_x;
	if (size_y > max_size_y) size_y = max_size_y;

	min_size_x = size_x;
	
	iPow2 = log((double)size_x) / log((double)2.0);

    min_size_x = 1 << iPow2;

	min_size_y = size_y;
	iPow2 = log((double)size_y) / log((double)2.0);
    min_size_y = 1 << iPow2;

	return(1);
}

void G3TextureParameters::GetMinSize(int &size_x,int &size_y)
{ 
  size_x = min_size_x;size_y = min_size_y;
}


// Evaluate float
float G3Fog::Eval(float z) {
	   float f= (end - z) /  ( end-start);
	   if (mode == D3DFOG_EXP) f = exp(-density * z);
	   else if (mode == D3DFOG_EXP2) f = exp(sqr(-density * z));
//		   f = clamp(f,1.0,0.0);
	   return f;
	   // return f= blend( f,c, (1-f),color);
}


const char * GFogModeLookup(int v);
int GFogModeLookup(const char *v);


const IS G3TextureParameters::filters[] = {
			{-1,"<Invalid>"},
			{D3DFILTER_NEAREST,"Nearest"},
			{D3DFILTER_LINEAR,"Linear"},
			{D3DFILTER_MIPNEAREST,"Mipmap_Nearest_Nearest"},
			{D3DFILTER_MIPLINEAR,"Mipmap_Linear_Nearest"},
			{D3DFILTER_LINEARMIPNEAREST,"Mipmap_Nearest_Linear"},
			{D3DFILTER_LINEARMIPLINEAR,"Mipmap_Linear_Linear"},
			{0,NULL},
			};

const IS G3TextureParameters::blends[] = {
			{-1,"<Invalid>"},
			{D3DTBLEND_DECAL,"Decal"},
			{D3DTBLEND_MODULATE,"Modulate"},
			{D3DTBLEND_DECALALPHA,"DecalAlpha"},
			{D3DTBLEND_MODULATEALPHA,"ModulateAlpha"},
			{D3DTBLEND_DECALMASK,"DecalMask"},
			{D3DTBLEND_MODULATEMASK,"ModulateMask"},
			{D3DTBLEND_COPY,"Copy"},
			{D3DTBLEND_ADD,"Add"},
			{0,NULL},
			};

const char * GFilterLookup(int v)
{ return Lookup(G3TextureParameters::filters,v); }

int GFilterLookup(const char *v) 
{ return Lookup(G3TextureParameters::filters,v); }

const IS G3Fog::modes [] = {
			{-1,"<Invalid>"},
			{D3DFOG_NONE,"none"},
			{D3DFOG_LINEAR,"linear"},
			{D3DFOG_EXP,"exp"},
			{D3DFOG_EXP2,"exp2"},
// vrml types
			{D3DFOG_NONE,"NONE"},
			{D3DFOG_LINEAR,"LINEAR"},
			{D3DFOG_EXP,"EXPONENTIAL"},
			{D3DFOG_EXP2,"EXPONENTIAL2"},
			{0,NULL},
			};

const char * GFogModeLookup(int v)
{ return Lookup(G3Fog::modes,v); }

int GFogModeLookup(const char *v)
{ return Lookup(G3Fog::modes,v); }

static const IS GL_RenderModesS [] = {
			{-1,"<Invalid>"},
			{ GRenderVertices,"Vertices" },
			{ GRenderBoundingBoxes,"BoundingBoxes" },
			{ GRenderWireframe,"Wireframe" },
			{ GRenderSolid,"Solid" },
			{ GRenderNonLighted,"NonLighted" },
			{ GRenderFlat,"Flat" },
			{ GRenderGouraud,"Gouraud" },

            // aliases lowercase 
			{ GRenderVertices,"vertices" },
			{ GRenderBoundingBoxes,"boundingboxes" },
			{ GRenderWireframe,"wireframe" },
			{ GRenderSolid,"solid" },
			{ GRenderNonLighted,"nonlighted" },
			{ GRenderFlat,"flat" },
			{ GRenderGouraud,"gouraud" },

			{0,NULL},
			};


const char * GRenderModeLookup(int v)
{ for (int i=0; GL_RenderModesS[i].s != NULL; i++)
      if (GL_RenderModesS[i].i == v) return(GL_RenderModesS[i].s);
  return 0;
}

int GRenderModeLookup(const char *v)
{ for (int i=0; GL_RenderModesS[i].s != NULL; i++)
      if (streq(GL_RenderModesS[i].s,v)) return(GL_RenderModesS[i].i);
  return -1;
}

#if 0
static const IS GL_DrawStyleS [] = {
			{-1,"<Invalid>"},
			{ GAsDefault,"default" },
			{ GAsSphere,"sphere" },
			{ GAsCube,"cube" },
			{ GAsCylinder,"cylinder" },
			{ GAsCone,"cone" },
			{ GAsDCone,"dcone" },
			{ GAsDisc,"disc" },
			{0,NULL},
			};

const char * GDrawStyleLookup(int v)
{ for (int i=0; GL_DrawStyleS[i].s != NULL; i++)
      if (GL_DrawStyleS[i].i == v) return(GL_DrawStyleS[i].s);
  return 0;
}

int GDrawStyleLookup(const char *v)
{ for (int i=0; GL_DrawStyleS[i].s != NULL; i++)
      if (streq(GL_DrawStyleS[i].s,v)) return(GL_DrawStyleS[i].i);
  return -1;
}
#endif

//
// RGBAColor
//


// Read/Write Material to File

int RGBAColor::io(AStream &a,int fmt)
{
 if (fmt == 1)	{ a.io(r); a.io(g); a.io(b); }
 else { a.Sep('('); a.io(r); a.io(g); a.io(b); a.Sep(')'); }
 return(a.stat);
}

int io(D3DCOLORVALUE &t,AStream &a,int fmt)
{
 if (fmt == 1)	{ a.io(t.r); a.io(t.g); a.io(t.b); }
 else { a.Sep('('); a.io(t.r); a.io(t.g); a.io(t.b); a.Sep(')'); }
 return(a.stat);
}

void SetGray(D3DCOLORVALUE &v, float gray,float alpha) 
{ v.r=v.g=v.b=gray; v.a=alpha;};


#if 0
Example
#
Inventor V1.0 ascii

Material {
    ambientColor	0.0706087 0.0212897 0.0336154
    diffuseColor	0.282435 0.0851587 0.134462
    specularColor	0.276305 0.11431 0.139857
    emissiveColor	0 0 0
    shininess	0.127273
    transparency	0
}
	RGBAColor ambient;
	RGBAColor diffuse;
	RGBAColor specular;
	RGBAColor emission;

	float shininess;
	float alpha;
#endif


//
// glMaterial
//



// Read Material
//

int G3Material::io(AStream &a)
{  int ret;

   char tok[256];
   a.Sep('{');
   if (a.isInput()) {
   while ( (a.Skip() != '}'))  {
	   a.r(tok);	strlwr(tok);
       if (streq(tok,"ambientcolor")) 	  { ret = ::io(ambient,a,1); }
       else if (streq(tok,"diffusecolor")) {ret = ::io(diffuse,a,1); }
       else if (streq(tok,"specularcolor")) {ret = ::io(specular,a,1); }
       else if (streq(tok,"emissivecolor")) {ret = ::io(emissive,a,1); }

       else if (streq(tok,"shininess")) { float r; ret=a.io(r);
										      power=r*128.0;
									     }
	   else if (streq(tok,"alpha") ||
				streq(tok,"transparency")) { float r; ret=a.io(r);
										      SetAlpha(r);
								}
	   else {
			a.Error("Bad keyword in reading material ");
			break;
			}
   }
   } else
   {
		// TBD
   }
   a.Sep('}');
   return(a.stat);
}

#if 0
int glMaterial::io(AStream &a)
{  int ret;

   char tok[256];
   a.Sep('{');
   if (a.isInput()) {
   while ( (a.Skip() != '}'))  {
	   a.r(tok);	strlwr(tok);
	   if (streq(tok,"ambientcolor")) 	{	ret = ambient.io(a,1); }
	   else if (streq(tok,"diffusecolor")) {ret = diffuse.io(a,1); }
	   else if (streq(tok,"specularcolor")) {ret = specular.io(a,1); }
	   else if (streq(tok,"emissivecolor")) {ret = emission.io(a,1); }
	   else if (streq(tok,"shininess")) { float r; ret=a.io(r);
										      shininess=r*128.0;
									     }
	   else if (streq(tok,"alpha") ||
				streq(tok,"transparency")) { float r; ret=a.io(r);
										      SetAlpha(r);
								}
	   else {
			a.error("Bad keyword in reading material ");
			break;
			}
   }
   } else
   {
		// TBD
   }
   a.Sep('}');
   return(a.stat);
}
#endif

int G3Material::ReadFromInventorFile(const char *FileName)
{ int ret;

  AStream a(FileName,"r");

  a.comment_char = '#';
  ret = 0;

  if (!a.atEof()) {
	    a.Skip();
		char tok[256];
		a.r(tok);	strlwr(tok);
		if (streq(tok,"material")) {
			ret=io(a);
		}
  }
  return(ret);
}



//
// class RenderState
//

RenderState::RenderState()
{
    // D3D
	GLastError=D3D_OK;

	hWnd   = NULL;
	currentFrame = 0;
	device = NULL;

    lpDD = NULL;
    lpD3D = NULL;
    lpDev = NULL;
    lpView = NULL;

	dx5 = FALSE;
	lpD3D2 = NULL;
	lpD3DDevice2 = NULL;
	lpD3DViewport2 = NULL;

	dx6 = FALSE;
	lpD3D3 = NULL;
	lpD3DDevice3 = NULL;
	lpD3DViewport3 = NULL;

	dxLevel = 0;

	dx6tex = FALSE; 

    beginSceneCnt = 0;
    hTex = 0;
    
    materials=NULL; // list of all materials in use 
    textures=NULL;	// list of all textures in use 
	lights=NULL;	// list of all lights in use 

    exBufs=NULL;	// list of all execute buffers in use 
	VBBufs=NULL;

	materialRampSize=16; // default 16, optimize on num materials ?
	

	enableOwnLighting = gtrue;
	doOwnLighting = gfalse;
	
	enableD3DCulling = gfalse; // gtrue;
	doD3DCulling = gfalse; // gtrue;
	useVertexBuffer = gfalse;

	maxBufferSize = 0;
	maxVertexCount = 0;
	maxMmatrixStack=0;
	maxTmatrixStack=0;
	maxLights=0;

	canCull	= gfalse;
	canDoRgbLighting = gfalse;
	canDoFog= gfalse;
	canDoAlphaBlend = gfalse;
	canDoMipMap = gfalse;
	canDoAlphaStipple = gfalse;
	canDoAlphaTest = gfalse;
	maxMipfilter = D3DFILTER_NEAREST;


	lightsTransformed = gfalse;
	lightsTransformedView=gfalse;

	materialChanged=gtrue;
	matrixChanged = gtrue;

	do2Dclip = gtrue;
	devNeed2DClip = gtrue;
	
	useKatmai = gfalse;
	katmaiState = NULL;

	currentMaterial = NULL;

	numLights = 0;
	firstLight =0;
   
    
    defaultMaterialValue.Default();
    defaultMaterialValue.diffuse.r=1.0;  // test
    defaultMaterialValue.diffuse.g=0.5;
    defaultMaterialValue.diffuse.b=1.0;
	defaultMaterialValue.specular.r=defaultMaterialValue.specular.g=defaultMaterialValue.specular.b =0.9f;
	defaultMaterialValue.power = 30;

    background.Gray(0.0);

	projection.Identity();
	projection.x[2][3] = 1.0;
	projection.x[3][2] = -1.0;
	projection.x[3][3] = 0.0;

	textureMatrix.Identity();
	textureMatrixVersion=0;
	textureMatrixVersionMax=0;
    textureMatrixIdentity = TRUE;

	view = ::ViewMatrix(Point(0,0,0),Point(0,0,2.5), Point (0, 1, 0));
       

			//zbuffer = 0; 
			zbuffer = gtrue; 
			wbuffer = gfalse; 
			zwrite = 1; 

			depthFunc = D3DCMP_LESSEQUAL;
			dither = 0; 
			//dither = 1;  // looks better !!!!!!!!!!!!!!!
			lastPixel = gtrue;

			alphaStipple = 1;
			// if 0 D3D HAL device with Alpha support don't apply alpha stipple
			// e.g. Matrox Mystiqe, Millenium 2

			noDitherIfTextured = 1;
			specular = 1;
			sbuffer = 0;
			renderMode = GRenderGouraud;
			LightMode = 0;

			unlit =FALSE;
			litMaterial = TRUE;

			vertices.visible = 0;
			normals.visible =0;
			bboxes.visible =0;
			
			lighting=0;
			lightmodel = GL_SMOOTH;
            fillMode = GL_FILL;
			localColors=0;
			backfaceCulling = 0;
			cullFront = 0;
			frontFaceCW=0;
			texturing=0;
			textureDefined = 0;
			twosidedLighting=0;
			localViewer=0;

			//ambient.Gray(0.2);
			//ambient.Gray(0.4); // 1.03.97 HG made everything brighter  but very gray in ramp driver
			ambient.Gray(0.1);
			ambient.a = ambient.r; // very important for D3D ramp mode !!!!!!!!!!!

			lineSmooth=0;
			
			mipMapLodBias =0.0f;
			zBias = 0;

			antialisMode =  D3DANTIALIAS_NONE;

			computePrimitiveList=0;

			blend = FALSE;
			blendFromMaterial = FALSE;

			colorKeyEnable = FALSE; // DX 5

			colorKeyEnable = TRUE; // DX 5, we want it true
			alphaBlend = FALSE; // DX 5

			blendSrcFunc= GL_SRC_ALPHA; // D3DBLEND_ONE;
			blendDestFunc=GL_ONE_MINUS_SRC_ALPHA; // D3DBLEND_ZERO;

			alphaTest = FALSE;
			needBlendForCKey = gfalse;

			alphaTestFromTexture = FALSE;

			alphaFunc = D3DCMP_ALWAYS;

			alphaRef = 1.0;
			alphaRefI = 255;

			alpha = 1.0;

			lightScale = 1.0;

			colorModel = D3DCOLOR_MONO;
            
            textureEnvMode = D3DTBLEND_MODULATE;

			
			vertexColor.Set(1.0,1.0,0.0);
			edgeColor.White();
			faceColor.Gray(0.5);
			normalColor.Set(1.0,0.0,0.0);
			bboxColor.Set(0.0,1.0,0.0);
			
			doAbortCheck=0;
			AbortCheckerData = 0;
			AbortChecker=0;
/*
			vertexDrawStyle = 0;
			edgeDrawStyle = 0;
			normalDrawStyle = 0;
			bboxDrawStyle = 0;
*/
			
			pointSize = 1.0;
            lineWidth= 1.0;
            lineStipple = 0;
            lineFactor = 1;
            linePattern = (unsigned short) -1;


            perspectiveCorrectionHint=1; // def 0 
            pointSmoothHint=0;
            lineSmoothHint=0;
            polygonSmoothHint=0;
			
			fogEnabled = 0;
			bufferMode = 0; // swapbuffers
			FlushCurrent();

	exBufAddMat=FALSE;
	exBufCombine = FALSE;
};





// set all cached glstate parameters 
void RenderState::SetContextAll() 
{
         SetZBuffer();
         SetPointSize();
         SetLineWidth();
         SetLineStipple();

         SetNormalize();
         SetLighting();
         SetLightModel();
         SetFillMode();

         SetTwoSidedLighting();
         SetLocalViewer();
         //SetMaterial();
         SetBackgroundColor();
         SetFog();
         SetAmbientColor();
         SetLocalColors();
         SetBackfaceCulling();
         SetFrontFaceCW();
         SetTexturing();
         // SetTexturingDiffuseColor ????
         SetLineAntialiasing();
         SetTextureTransform();
    	 // glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,textureEnvMode);

    	 // glHint(GL_FOG_HINT,fogHint);
}


// set global render mode to mode (enum)
int RenderState::SetRenderMode(GRenderMode newMode)
{

  FlushState();
  switch (newMode)  {
  case GRenderWireframe:
	faces.visible = 0;
	edges.visible = 1;

	edges.lighted = LightMode;
	edges.smooth = LightMode || edges.colored;
	edges.useMaterialColor = 1;

	vertices.visible = 0;
	bboxes.visible = 0;

	SetZBuffer(LightMode);  /* ligted edges are better with z-buffering */
	SetLighting(LightMode);
	if (edges.smooth)
	   SetLightModel(GL_SMOOTH);
	else SetLightModel(GL_FLAT);
    SetFillMode(GL_LINE);
	unlit = FALSE;

	break;

  case GRenderVertices:
	faces.visible = 0;
	edges.visible = 0;
	bboxes.visible = 0;
	vertices.visible = 1;
	vertices.lighted = LightMode;

	SetZBuffer(0);
	//if (vertexDrawStyle && vertexDrawStyle->enabled) {
	// 	SetZBuffer(1);
	//}
	SetLighting(LightMode);
    SetFillMode(GL_POINT);
	unlit = FALSE;

	break;

  case GRenderBoundingBoxes:
	faces.visible = 0;
	edges.visible = 0;
	vertices.visible = 0;
	bboxes.visible = 1;

	SetZBuffer(0);
/* x
	if (bboxDrawStyle && bboxDrawStyle->enabled) {
	 	SetZBuffer(1);
		SetLighting(LightMode);
	} else 
*/
    SetLighting(0);
	unlit = TRUE;

	break;

  case GRenderSolid:
	faces.visible = 1;
	faces.lighted = 0;
	edges.visible = 1;
	edges.lighted = 0;
	edges.useMaterialColor = 0;
	vertices.visible = 0;
	bboxes.visible = 0;

	SetZBuffer(1);
	SetLighting(0);
//	theRenderState.SetLightModel(GL_FLAT);
//x	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,textureEnvMode = GL_DECAL);
	// theRenderState.sbuffer = 1; /* use stencil planes */
	SetTextureBlend(GL_DECAL);
    SetFillMode(GL_FILL);
	unlit = TRUE;

	break;
  case GRenderNonLighted:
	ReleaseMaterials();
	ReleaseLights();

	faces.visible = 1;
	faces.lighted = 0;
	edges.visible = 0;
	edges.lighted = 0;
	edges.useMaterialColor = 0;
	vertices.visible = 0;
	bboxes.visible = 0;
	SetZBuffer(1);
	SetLighting(0);
	SetTextureBlend(GL_DECAL); // D3DTBLEND_DECAL
	//SetTextureBlend(D3DTBLEND_COPY);
	//x	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,textureEnvMode = GL_DECAL);
    SetFillMode(GL_FILL);
	unlit = TRUE;
	break;

  case GRenderFlat:
	faces.visible = 1;
	faces.lighted = 1;
	faces.smooth = 0;
	edges.visible = 0;
	vertices.visible = 0;
	bboxes.visible = 0;
	SetLightModel(GL_FLAT);
	SetZBuffer(1);
	SetLighting(1);
//x	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,textureEnvMode = GL_MODULATE);
	SetTextureBlend(GL_MODULATE);
    SetFillMode(GL_FILL);
	unlit = FALSE;
	break;
  case GRenderGouraud :
	faces.visible = 1;
	faces.lighted = 1;
	faces.smooth = 1;
	edges.visible = 0;
	vertices.visible = 0;
	bboxes.visible = 0;
	SetLightModel(GL_SMOOTH);
	SetZBuffer(1);
	SetLighting(1);
//x	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,textureEnvMode = GL_MODULATE);
	SetTextureBlend(GL_MODULATE);
    SetFillMode(GL_FILL);
	unlit = FALSE;
	break;

 }
 renderMode = newMode;
 FlushState();
 return(0);
}
int RenderState::SetRenderFlags(RenderFlags &flags)
{
  	if (!flags.visible) return(0);
  	SetLighting(flags.lighted);
	SetTexturing(flags.textured && textureDefined);
	SetLightModel(flags.smooth ? GL_SMOOTH : GL_FLAT);
	SetLocalColors(flags.colored);
	return(1);

}

// Set current color, also change material color
void RenderState::SetColor(RGBAColor &c) {
/*
	  	const int face = GL_FRONT_AND_BACK;
		if (lighting) {
		   //glMaterialfv(face,GL_AMBIENT,&c.r);
		   glMaterialfv(face,GL_DIFFUSE,&c.r);
		}
		c.glSet();
*/
	currentColor = c;
}

// Set Rendering Attributes to vertex
int RenderState::SetVertexAttributes() 
{
	if (SetRenderFlags(vertices)) {
	   if (!vertices.useMaterialColor) SetColor(vertexColor);
	   return(1);
	}
	return(0);
}

// Set Rendering Attributes to edge
int RenderState::SetEdgeAttributes() 
{
	if (SetRenderFlags(edges)) {
	   if (!edges.useMaterialColor) SetColor(edgeColor);
	   return(1);
	}
	return(0);
}

// Set Rendering Attributes to face
int RenderState::SetFaceAttributes() 
{
	if (SetRenderFlags(faces)) {
	   if (!faces.useMaterialColor) SetColor(faceColor);
	   return(1);
	}
	return(0);
}
// Set Rendering Attributes to normal
int RenderState::SetNormalAttributes() 
{
	if (SetRenderFlags(normals)) {
	   if (!normals.useMaterialColor) SetColor(normalColor);
	   return(1);
	}
	return(0);
}

int RenderState::SetBBoxAttributes() 
{
	if (SetRenderFlags(bboxes)) {
	   if (!bboxes.useMaterialColor) SetColor(bboxColor);
	   return(1);
	}
	return(0);
}

void RenderState::Clear() {
   //xglClear (GL_COLOR_BUFFER_BIT | (zbuffer ? GL_DEPTH_BUFFER_BIT : 0));
	
    if (!device->ClearBackBuffer(D3DAPP_CLEARBACK | (zbuffer ? D3DAPP_CLEARZ : 0) | D3DAPP_CLEARALL /* bResized ? D3DAPP_CLEARALL : NULL*/)) {
		//XXXXXXX  ReportD3DError();
		TRACE("RenderState::Clear: D3D Can´t clear\n");
     }
};

// clear buffers
void RenderState::Clear(gbool colorBuffer,gbool zBuffer,gbool stencilBuffer,gbool accumBuffer) 
{
   //xglClear (GL_COLOR_BUFFER_BIT | (zbuffer ? GL_DEPTH_BUFFER_BIT : 0));
	
    if (!device->ClearBackBuffer(D3DAPP_CLEARALL | 
								(zBuffer ? D3DAPP_CLEARZ : 0) 
								| (colorBuffer ? D3DAPP_CLEARBACK : 0))
		
	) {
			//ReportD3DError();
			TRACE("D3D Can´t clear\n");
               
     }
};

// set current viewport
void RenderState::SetViewport(int x,int y,int width, int height) 
{  // lower left to to top left
	if (!device) return;
	device->SetViewport(x,sizey-(y+height),width,height);
}

void RenderState::SetViewport() 
{
	if (!device) return;
	device->SetViewport();
}
 

// get current viewport
void RenderState::GetViewport(int &x,int &y,int &width, int &height) 
{
	if (!device) return;
	device->GetViewport(x,y,width,height);
	y=sizey-(y+height);

}

 

void RenderState::SetBufferMode(int mode)
{
	
	bufferMode = mode;
	switch (mode) {
	case 0:
		/* Draw into the back buffer */
		SetRenderSize(sizex,sizey);
		//glDrawBuffer(GL_BACK);
		break;
	case 1:
		/* Draw into the front buffer */
		SetRenderSize(sizex,sizey);
		//glDrawBuffer(GL_FRONT);
		break;
	case 2:
		/* Draw into the back buffer */
		//glDrawBuffer(GL_BACK);
		//glViewport(0, 0, renderSizex, renderSizey);
		SetRenderSize(sizex/2,sizey/2);
		break;
	}
}

int RenderState::SwapBuffers()
{
	switch (bufferMode) {
	case 0:
	    //HDC hdc = gl.wglGetCurrentDC();
    	//SwapBuffers(hdc);
		return(1);

		break;
	case 1:
		break;
	case 2:
		{

		/* Copy the backbuffer viewport with a 2X zoom.
	 	* Don't forget to disable depth test, blending,lighting, and
	 	* other modes which slow down pixel operations!
	 	*/
		}
		break;

	}
  return(0);
}

// copy back to front buffer // implementation dependent
int RenderState::UpdateFrontBuffer()
{
	switch (bufferMode) {
	case 1:
		break;
	case 0:
	case 2:
		{
		gbool saveZBuffer = zbuffer;
		gbool saveLighting = lighting;

		/* Copy from the back buffer to the front buffer */
		SetZBuffer(saveZBuffer);
		SetLighting(saveLighting);
		}
		break;

	}
  return(0);
}


void RenderState::SetSize(int sizeX,int sizeY)
{
	sizex = sizeX,sizey = sizeY;
	if (bufferMode == 2)
	   SetRenderSize(sizex / 2,sizey / 2);
	else SetRenderSize(sizex,sizey);
}

void RenderState::SetRenderSize(int sizeX,int sizeY)
{
	renderSizex = sizeX,renderSizey = sizeY;
    //todo:	glViewport(0, 0, renderSizex, renderSizey);
}


/*
 * Normalises the vector v
 */
static 
LPD3DVECTOR 
D3DVECTORNormalise(LPD3DVECTOR v)
{
    float vx, vy, vz, inv_mod;
    vx = v->x;
    vy = v->y;
    vz = v->z;
    if ((vx == 0) && (vy == 0) && (vz == 0))
        return v;
    inv_mod = (float)(1.0 / sqrt(vx * vx + vy * vy + vz * vz));
    v->x = vx * inv_mod;
    v->y = vy * inv_mod;
    v->z = vz * inv_mod;
    return v;
}


/*
 * Calculates cross product of a and b.
 */
static 
LPD3DVECTOR 
D3DVECTORCrossProduct(LPD3DVECTOR lpd, LPD3DVECTOR lpa, LPD3DVECTOR lpb)
{

    lpd->x = lpa->y * lpb->z - lpa->z * lpb->y;
    lpd->y = lpa->z * lpb->x - lpa->x * lpb->z;
    lpd->z = lpa->x * lpb->y - lpa->y * lpb->x;
    return lpd;
}



/*
 * -1 d = a
 */
static 
LPD3DMATRIX 
D3DMATRIXInvert(LPD3DMATRIX d, LPD3DMATRIX a)
{
    d->_11 = a->_11;
    d->_12 = a->_21;
    d->_13 = a->_31;
    d->_14 = a->_14;

    d->_21 = a->_12;
    d->_22 = a->_22;
    d->_23 = a->_32;
    d->_24 = a->_24;

    d->_31 = a->_13;
    d->_32 = a->_23;
    d->_33 = a->_33;
    d->_34 = a->_34;

    d->_41 = a->_14;
    d->_42 = a->_24;
    d->_43 = a->_34;
    d->_44 = a->_44;

    return d;
}


/*
 * Set the rotation part of a matrix such that the vector lpD is the new
 * z-axis and lpU is the new y-axis.
 */
static 
LPD3DMATRIX 
D3DMATRIXSetRotation(LPD3DMATRIX lpM, LPD3DVECTOR lpD, LPD3DVECTOR lpU)
{
    float t;
    D3DVECTOR d, u, r;

    /*
     * Normalise the direction vector.
     */
    d.x = lpD->x;
    d.y = lpD->y;
    d.z = lpD->z;
    D3DVECTORNormalise(&d);

    u.x = lpU->x;
    u.y = lpU->y;
    u.z = lpU->z;
    /*
     * Project u into the plane defined by d and normalise.
     */
    t = u.x * d.x + u.y * d.y + u.z * d.z;
    u.x -= d.x * t;
    u.y -= d.y * t;
    u.z -= d.z * t;
    D3DVECTORNormalise(&u);

    /*
     * Calculate the vector pointing along the matrix x axis (in a right
     * handed coordinate system) using cross product.
     */
    D3DVECTORCrossProduct(&r, &u, &d);

    lpM->_11 = r.x;
    lpM->_12 = r.y, lpM->_13 = r.z;
    lpM->_21 = u.x;
    lpM->_22 = u.y, lpM->_23 = u.z;
    lpM->_31 = d.x;
    lpM->_32 = d.y;
    lpM->_33 = d.z;

    return lpM;
}


/*
 * Creates a matrix which is equivalent to having the camera at a
 * specified position. This matrix can be used to convert vertices to
 * camera coordinates. lpP    Position of the camera. lpD    Direction of
 * view. lpN    Up vector. lpM    Matrix to update.
 */

// see also Graphic Gems I, Pg. 778
static 
void 
PositionCamera(LPD3DVECTOR lpP, LPD3DVECTOR lpD, LPD3DVECTOR lpN, 
               LPD3DMATRIX lpM)
{
    D3DMATRIX tmp;

    /*
     * Set the rotation part of the matrix and invert it. Vertices must be
     * inverse rotated to achieve the same result of a corresponding 
     * camera rotation.
     */
    tmp._14 = tmp._24 = tmp._34 = tmp._41 = tmp._42 = tmp._43 = (float)0.0;
    tmp._44 = (float)1.0;

    D3DMATRIXSetRotation(&tmp, lpD, lpN);
    D3DMATRIXInvert(lpM, &tmp);
    //*lpM = tmp;
    /*
     * Multiply the rotation matrix by a translation transform.  The
     * translation matrix must be applied first (left of rotation).
     */
    lpM->_41=-(lpM->_11 * lpP->x + lpM->_21 * lpP->y + lpM->_31 * lpP->z);
    lpM->_42=-(lpM->_12 * lpP->x + lpM->_22 * lpP->y + lpM->_32 * lpP->z);
    lpM->_43=-(lpM->_13 * lpP->x + lpM->_23 * lpP->y + lpM->_33 * lpP->z);

}


//To do this, flip the signs of the _13, _23, _33, and _43 

/*
static D3DMATRIX proj = {
    D3DVAL(2.0), D3DVAL(0.0), D3DVAL(0.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(2.0), D3DVAL(0.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(0.0), D3DVAL(1.0*0.1), D3DVAL(1.0),
    D3DVAL(0.0), D3DVAL(0.0), D3DVAL(-1.0), D3DVAL(0.0)
};

*/

/*

From Direct x help file

Direct3D's Coordinate System 
Direct3D uses the left-handed coordinate system. This means the positive z-axis points away from the viewer, as shown in the following illustration: 
 
In a left-handed coordinate system, rotations occur clockwise around any axis that is pointed at the viewer. 
If you need to work in a right-handed coordinate system—for example, if you are porting an application that relies on right-handedness—you can do so by making two simple changes to the data passed to Direct3D. 

·	Flip the order of triangle vertices so that the system traverses them clockwise from the front. In other words, if the vertices are v0, v1, v2, pass them to Direct3D as v0, v2, v1. 
·	Scale the projection matrix by -1 in the z direction. 
To do this, flip the signs of the _13, _23, _33, and _43 
members of the D3DMATRIX structure. 
*/


// set view and projection matrix from camera 
// see also http://www.microsoft.com/directx/pavilion/d3d/projmatrix.htm
int RenderState::SetCamera(GCamera *camera)
{

//    D3DMATRIX m1;
    Point dir = camera->TargetDirection();
    float d  = camera->TargetDistance();

	float w,h,r;

	camera->widthWorld = w = camera->width;
	camera->heightWorld = h = camera->height;

	// map to -1,-1 / 1,1, viewport will scale it to window size 

	// Adapt to window aspect
	r = w / h;
	if (r < camera->aspect) {
	   camera->widthWorld = w = h*camera->aspect;
	} else {
       camera->heightWorld = h = w / camera->aspect; // exchanged 
	}


    //PositionCamera((LPD3DVECTOR)&camera->position,(LPD3DVECTOR)&dir,(LPD3DVECTOR)&camera->up,&m1);
/*	Matrix  */ // is global in state 
	view = ViewMatrix(camera->target, camera->position, camera->up);
	//Matrix view = ViewMatrix(camera->position, camera->target, camera->up);
    //GL Like 
/*
    proj._11 = 2.0 * d  / w;
    proj._22 = 2.0 * d / h;
    proj._33 =  - (camera->zfar + camera->znear) / (camera->zfar - camera->znear);
    proj._34 = - (2.0 * camera->zfar * camera->znear) / (camera->zfar - camera->znear);
    proj._43 =  -1 ;
*/

/*
    problem x is mirrored,
	a -1.0 for 11 makes right orientation, *but* culling is reversed 
	and normals ???????????????????

*/

/*  working  */
	float flip = 1.0; // if -1 nothing is visible 
	projection.Identity();

	float zd = (camera->zfar - camera->znear); 

#if 1
	// - x  RIGHT_HANDED 
    //projection.x[0][0]  = 2.0  / w;

    projection.x[0][0] =  -2.0f * d / w  ;  // scale x
    projection.x[1][1] =   2.0f * d / h  ;// scale y 


    projection.x[2][3] = 1.0f;		// perspective divide 
    projection.x[2][2] = (camera->zfar) / zd; // scale z 
    //projection.x[3][2] = (-camera->zfar * camera->znear) /  zd ;  

    projection.x[3][2] = -(camera->zfar / zd) * camera->znear;  

/*
	// intel : 
    projection.x[2][3] = 1.0f;		// perspective divide 
    projection.x[2][2] = (camera->znear+camera->zfar) / zd;
	// scale z 
    projection.x[3][2] = (-2.0*camera->zfar * camera->znear) /  zd ;  
*/


#else 
	// - x  RIGHT_HANDED 
    //proj._11 = 2.0  / w;
    projection.x[0][0] = -2.0f / w ;  // scale x
    projection.x[1][1] =  2.0f / h ;	// scale y 

    projection.x[2][3] = 1.0f / d;		// perspective divide 
    projection.x[2][2] = 1.0* (camera->znear + camera->zfar) * flip * proj._34  / zd; // scale z 

    // proj._33 =  -1.0 * (camera->zfar + camera->znear) / (camera->zfar - camera->znear) ;


//  proj._43 = flip * -1. *0.001 ; // old 
    projection.x[3][2] = -2.0*camera->zfar * camera->znear * proj._34  / zd ;  // z-translation new 21.09.96 

/*
	// GL 
	proj._34 = -1; // xx
	projection.x[2][2] = proj._33  = -1.0 * (camera->zfar + camera->znear) / (zd);
	projection.x[3][2]= proj._43  = -2.0 * (camera->zfar * camera->znear) / (zd);
*/
#endif 

	projection.x[3][3] = 0.0f;
	projection.contents = Matrix::HAS_TRANSLATION|Matrix::HAS_SCALE|Matrix::HAS_NON_UNIFORM_SCALE|Matrix::HAS_PERSPECTIVE;


	// set camera matrices 
	camera->viewMatrix = view;
	camera->projectionMatrix=projection;
    camera->world2pValid=camera->p2worldValid=0;

	Multiply(view,projection,model2screen);
	// viewport scale
	//float w= device->szClient.cx*0.5f;
	//float h= device->szClient.cy*0.5f;


#ifdef _DEBUG
#if 0
	// test computed matrices 
	dir.Normalize();

	Point p= camera->position + dir * camera->znear;
	Point pv= camera->viewMatrix * p;
	pv= camera->projectionMatrix * pv;
	Point pw= camera->World2p() * p;
	TRACE("Translated point znear (%f %f %f) ==> (%f %f %f) (%f %f %f) \n",p.x,p.y,p.z,pw.x,pw.y,pw.z,pv.x,pv.y,pv.z);

	p= camera->P2World() * pw;
	TRACE("Translated point back  (%f %f %f) <== (%f %f %f) \n",p.x,p.y,p.z,pw.x,pw.y,pw.z);

	p= camera->position + dir * camera->zfar;
	pv= camera->viewMatrix * p;
	pv= camera->projectionMatrix * pv;
	pw= camera->World2p() * p;

	TRACE("Translated point zfar (%f %f %f) ==> (%f %f %f) (%f %f %f)\n",p.x,p.y,p.z,pw.x,pw.y,pw.z,pv.x,pv.y,pv.z);

	p= camera->P2World() * pw;
	TRACE("Translated point back  (%f %f %f) <== (%f %f %f) \n",p.x,p.y,p.z,pw.x,pw.y,pw.z);

	pw.Set(0,0,camera->znear);
	p= camera->projectionMatrix * pw;
	TRACE("persp znear  (%f %f %f) <== (%f %f %f) \n",p.x,p.y,p.z,pw.x,pw.y,pw.z);

	pw.Set(0,0,camera->zfar);
	p= camera->projectionMatrix * pw;
	TRACE("persp zfar  (%f %f %f) <== (%f %f %f) \n",p.x,p.y,p.z,pw.x,pw.y,pw.z);
#endif


#endif 

	lightsTransformed = gfalse;
	lightsTransformedView=gfalse;

	matrixChanged = gtrue;

	//if (!doOwnLighting) { // need to set matrix because onTL clip DX may retransform
	if (!lpDev) {
        TRACE("Can´t set projection  matrix zet , no device \n");
		return (0);
	}

    if (!projMatrix.Set(lpDev,projection))  {
        TRACE("Can´t set projection  matrix \n");
    }

    if (!viewMatrix.Set(lpDev, view))  {
        TRACE("Can´t set view matrix \n");
        return(0);
	}
	//} // doOwnLighting


    return(1);
}





// primitive Rendering 
void RenderState::RenderVertex(Point &p){}
void RenderState::RenderLine(Point &p1,Point &p2){}
void RenderState::RenderBBox(Point &p1,Point &p2)
{
  RenderCubeEdges(p1,p2);
}
void RenderState::RenderCylinder(Point &p, float r, float h){}
void RenderState::RenderCone(Point &p, float r, float h){}
//	 void RenderState::RenderSphere(Point &p,float r){}



/* render a wireframe cube */
void RenderState::RenderCubeEdges(double x0, double y0,double z0,
				double x1,double y1,double z1)
{ gbool saveLighting = lighting;
  SetLighting(0);
#if 0
  glBegin (GL_LINES);  /* vertical */
    glVertex3f (x0, y0, z0);  glVertex3f (x0, y0, z1);
    glVertex3f (x0, y1, z0);  glVertex3f (x0, y1, z1);
    glVertex3f (x1, y0, z0);  glVertex3f (x1, y0, z1);
    glVertex3f (x1, y1, z0);  glVertex3f (x1, y1, z1);
  glEnd ();

  glBegin (GL_LINE_LOOP);  /* bottom */
    glVertex3f (x0, y0, z0);
    glVertex3f (x0, y1, z0);
    glVertex3f (x1, y1, z0);
    glVertex3f (x1, y0, z0);
  glEnd ();
  glBegin (GL_LINE_LOOP);  /* top */
    glVertex3f (x0, y0, z1);
    glVertex3f (x0, y1, z1);
    glVertex3f (x1, y1, z1);
    glVertex3f (x1, y0, z1);
  glEnd ();
#endif
  SetLighting(saveLighting);
} 

void RenderState::RenderCubeEdges(const Point &p0,const Point &p1)
{
  RenderCubeEdges(p0.x,p0.y,p0.z,p1.x,p1.y,p1.z);
}

void RenderState::RenderSphere(Point &center, float r,int level, int mask, int VN,int FN)
{

} 


#if 0
static int renderSphereVN = 1;
static int renderSphereFN = 0;

void glRenderSphereSub(int level,const Point &p1,const Point &p2,const Point &p3)
{

	if  (level<=0) { 	
#if 0
		if (renderSphereFN) {
		   glNormal(NormCrossProd(p2-p1,p3-p1));
		}
 		if (renderSphereVN) glNormal(p1);
 		glVertex(p1);
 		if (renderSphereVN) glNormal(p2);
 		glVertex(p2);
 		if (renderSphereVN) glNormal(p3);
 		glVertex(p3);
#endif
		return;
	}

	Point p12((p1+p2)*0.5); p12.Normalize();
	Point p23((p2+p3)*0.5); p23.Normalize();
	Point p31((p3+p1)*0.5);	p31.Normalize();
	{
	  level--;
	  glRenderSphereSub(level,p1,p12,p31);
	  glRenderSphereSub(level,p12,p23,p31);
	  glRenderSphereSub(level,p31,p23,p3);
	  glRenderSphereSub(level,p12,p2,p23);
	}
}

void RenderState::RenderSphere(Point &center, float r,int level, int mask, int VN,int FN)
{
	renderSphereVN=VN;
	renderSphereFN=FN;

/*
   Icosahedron
   from Open Gl programming guide, pg. 56
*/
#define X .525731112119133606
#define Z .850650808352039932
//static Point icosahedron_pts[12]= { {-X,0.0,Z}, {X,0.0,Z},{-X,0.0,-Z}, {X,0.0,-Z},
//						  {0.0,Z,X}, {0.0,Z,-X},{0.0,-Z,X}, {0.0,-Z,-X},
//						  {Z,X,0.0}, {-Z,X,0.0},{Z,-X,0.0}, {-Z,-X,0.0} };

static point icosahedron_ptsp[12]= {
						  {-X,0.0,Z}, {X,0.0,Z},{-X,0.0,-Z}, {X,0.0,-Z},
						  {0.0,Z,X}, {0.0,Z,-X},{0.0,-Z,X}, {0.0,-Z,-X},
						  {Z,X,0.0}, {-Z,X,0.0},{Z,-X,0.0}, {-Z,-X,0.0}
						  };
static int icosahedron_connectivity[] = {
		3,0,4,1,  3,0,9,4,		3,9,5,4,  3,4,5,8,		3,4,8,1,
		3,8,10,1, 3,8,3,10, 3,5,3,8,  3,5,2,3,	3,2,7,3,
		3,7,10,3, 3,7,6,10, 3,7,11,6, 3,11,0,6, 3,0,1,6,
		3,6,1,10, 3,9,0,11, 3,9,11,2, 3,9,2,5,	3,7,2,11
		};

	Point *icosahedron_pts = (Point *) icosahedron_ptsp;
	//glPushMatrix();
	//glScalef(r,r,r);
	//glTranslate(center);
	
	// normalize should be on
	//glBegin(GL_TRIANGLES);
	const int *fp =icosahedron_connectivity; 
	const int *fpEnd =icosahedron_connectivity+ELEMENTS(icosahedron_connectivity);
	
	while (fp<fpEnd) {
		int n=*fp++;
		int i1=*fp++;
		int i2=*fp++;
		int i3=*fp++;

		glRenderSphereSub(level,icosahedron_pts[i1],icosahedron_pts[i2],icosahedron_pts[i3]);
	}
	//glEnd();
	//glPopMatrix();
}

#endif 

#endif //D3D


/*

  http://www.silvercrk.com/directx/dxcaps/rage.txt

  ATI rage
Microsoft Direct3D Hardware acceleration through Direct3D HAL (Direct3D HAL)
        COLORMODEL                              Yes
        DEVCAPS                                 Yes
        LINECAPS                                Yes
        TRICAPS                                 Yes
        DEVICERENDERBITDEPTH                    Yes
ColorModel
        D3DCOLOR_RGB                            Yes
DevCaps
        SORTEXACT                               Yes
TransformCaps
LightingCaps
        dwNumLights                             0
BClipping
LineCaps
        D3DPMISCCAPS_CULLCCW                    Yes
        D3DPRASTERCAPS_DITHER                   Yes
        D3DPRASTERCAPS_SUBPIXEL                 Yes
        D3DPBLENDCAPS_ZERO                      Yes
        D3DPBLENDCAPS_ONE                       Yes
        D3DPBLENDCAPS_SRCALPHA                  Yes
        D3DPBLENDCAPS_INVSRCALPHA               Yes
        D3DPBLENDCAPS_DESTCOLOR                 Yes
        D3DPBLENDCAPS_INVDESTCOLOR              Yes
        D3DPBLENDCAPS_ZERO                      Yes
        D3DPBLENDCAPS_ONE                       Yes
        D3DPBLENDCAPS_SRCCOLOR                  Yes
        D3DPBLENDCAPS_INVSRCCOLOR               Yes
        D3DPBLENDCAPS_SRCALPHA                  Yes
        D3DPBLENDCAPS_INVSRCALPHA               Yes
        D3DPSHADECAPS_COLORFLATRGB              Yes
        D3DPSHADECAPS_COLORGOURAUDRGB           Yes
        D3DPSHADECAPS_ALPHAFLATBLEND            Yes
        D3DPSHADECAPS_ALPHAGOURAUDBLEND         Yes
        D3DPSHADECAPS_FOGFLAT                   Yes
        D3DPSHADECAPS_FOGGOURAUD                Yes
        D3DPTEXTURECAPS_PERSPECTIVE             Yes
        D3DPTEXTURECAPS_POW2                    Yes
        D3DPTFILTERCAPS_NEAREST                 Yes
        D3DPTFILTERCAPS_LINEAR                  Yes
        D3DPTBLENDCAPS_DECAL                    Yes
        D3DPTBLENDCAPS_MODULATE                 Yes
        D3DPTBLENDCAPS_DECALALPHA               Yes
        D3DPTADDRESSCAPS_WRAP                   Yes
TriCaps
        D3DPMISCCAPS_CULLCCW                    Yes
        D3DPRASTERCAPS_DITHER                   Yes
        D3DPRASTERCAPS_SUBPIXEL                 Yes
        D3DPBLENDCAPS_ZERO                      Yes
        D3DPBLENDCAPS_ONE                       Yes
        D3DPBLENDCAPS_SRCALPHA                  Yes
        D3DPBLENDCAPS_INVSRCALPHA               Yes
        D3DPBLENDCAPS_DESTCOLOR                 Yes
        D3DPBLENDCAPS_INVDESTCOLOR              Yes
        D3DPBLENDCAPS_ZERO                      Yes
        D3DPBLENDCAPS_ONE                       Yes
        D3DPBLENDCAPS_SRCCOLOR                  Yes
        D3DPBLENDCAPS_INVSRCCOLOR               Yes
        D3DPBLENDCAPS_SRCALPHA                  Yes
        D3DPBLENDCAPS_INVSRCALPHA               Yes
        D3DPSHADECAPS_COLORFLATRGB              Yes
        D3DPSHADECAPS_COLORGOURAUDRGB           Yes
        D3DPSHADECAPS_ALPHAFLATBLEND            Yes
        D3DPSHADECAPS_ALPHAGOURAUDBLEND         Yes
        D3DPSHADECAPS_FOGFLAT                   Yes
        D3DPSHADECAPS_FOGGOURAUD                Yes
        D3DPTEXTURECAPS_PERSPECTIVE             Yes
        D3DPTEXTURECAPS_POW2                    Yes
        D3DPTFILTERCAPS_NEAREST                 Yes
        D3DPTFILTERCAPS_LINEAR                  Yes
        D3DPTBLENDCAPS_DECAL                    Yes
        D3DPTBLENDCAPS_MODULATE                 Yes
        D3DPTBLENDCAPS_DECALALPHA               Yes
        D3DPTADDRESSCAPS_WRAP                   Yes
Misc
        MaxBufferSize                           0
        MaxVertexCount                          1024
        DeviceRenderBitDepth                    1280
        DeviceZBufferBitDepth                   0

*/
