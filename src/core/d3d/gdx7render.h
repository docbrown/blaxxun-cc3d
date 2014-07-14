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
#ifndef _GDX7Render_
#define _GDX7Render_



/******************************************************************************
@doc

@module GDX7Render.h - GLView GDX7RenderState classes for Direct 3D |

Copyright (c) 1999-1999	by blaxxun interactive - Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:
	global Macro define _D3D for Direct 3D Support 

Changes:

$Revision: $
$Log: $


Todo :

******************************************************************************/



#if(DIRECT3D_VERSION >= 0x0700) 


#define _DX7

#endif

#ifdef _DX7

// check for error code and TRACE a message
HRESULT D3DError_(const char *s,HRESULT error, const char *file, int line);


#ifdef _DEBUG

#define	   D3DLASTERROR() HRESULT GLastError=D3D_OK;
#define    D3DError(MSG,RESULT) D3DError_(MSG,RESULT,__FILE__,__LINE__)
#define    D3DCheck(CALL) (((GLastError = CALL) == D3D_OK) ? D3D_OK :D3DError("Call failed",GLastError))

#else

#define	    D3DLASTERROR()
#define     D3DError(MSG,RESULT) 0
#define     D3DCheck(CALL) CALL

#endif


// want a right handed coordinate system like ogl
// : projection matrix x-flip and faces are flipped 
#define GDX7RIGHT_HANDED

// forward 
class GD3Device;

//typedef struct _image image;

#include "image.h"

#include "grenderbase.h"


/*
   attribute to be handled
   material 
   texture
   culling
   transform / mult / set

  add instructions to attribute buffer

  flush on draw 


    global
	   lights
	   fog
	   view
	   renderModes ..

*/


// convienience 
#define glMaterial GDX7Material


#define GL_FLAT D3DSHADE_FLAT
#define GL_SMOOTH D3DSHADE_GOURAUD

#define GL_POINT D3DFILL_POINT
#define GL_LINE D3DFILL_WIREFRAME
#define GL_FILL D3DFILL_SOLID

#define GL_MODULATE  D3DTBLEND_MODULATE    
#define GL_DECAL  D3DTBLEND_DECAL      


#define GL_NEVER                          D3DCMP_NEVER
#define GL_LESS                           D3DCMP_LESS
#define GL_EQUAL                          D3DCMP_EQUAL
#define GL_LEQUAL                         D3DCMP_LESSEQUAL
#define GL_GREATER                        D3DCMP_GREATER
#define GL_NOTEQUAL                       D3DCMP_NOTEQUAL
#define GL_GEQUAL                         D3DCMP_GREATEREQUAL
#define GL_ALWAYS                         D3DCMP_ALWAYS



/* PixelFormat */
#define GL_COLOR_INDEX                    0x1900
#define GL_STENCIL_INDEX                  0x1901
#define GL_DEPTH_COMPONENT                0x1902
#define GL_RED                            0x1903
#define GL_GREEN                          0x1904
#define GL_BLUE                           0x1905
#define GL_ALPHA                          0x1906
#define GL_RGB                            0x1907
#define GL_RGBA                           0x1908
#define GL_LUMINANCE                      0x1909
#define GL_LUMINANCE_ALPHA                0x190A

/* BlendingFactorDest */
#define GL_ZERO                           D3DBLEND_ZERO
#define GL_ONE                            D3DBLEND_ONE
#define GL_SRC_COLOR                      D3DBLEND_SRCCOLOR
#define GL_ONE_MINUS_SRC_COLOR            D3DBLEND_INVSRCCOLOR
#define GL_SRC_ALPHA                      D3DBLEND_SRCALPHA
#define GL_ONE_MINUS_SRC_ALPHA            D3DBLEND_INVSRCALPHA
#define GL_DST_ALPHA                      D3DBLEND_DESTALPHA
#define GL_ONE_MINUS_DST_ALPHA            D3DBLEND_INVDESTALPHA

/* BlendingFactorSrc */
/*      GL_ZERO */
/*      GL_ONE */
#define GL_DST_COLOR                      D3DBLEND_DESTCOLOR
#define GL_ONE_MINUS_DST_COLOR            D3DBLEND_INVDESTCOLOR
#define GL_SRC_ALPHA_SATURATE             D3DBLEND_SRCALPHASAT
/*      GL_SRC_ALPHA */
/*      GL_ONE_MINUS_SRC_ALPHA */
/*      GL_DST_ALPHA */
/*      GL_ONE_MINUS_DST_ALPHA */
//    D3DBLEND_BOTHSRCALPHA      = 12,
//    D3DBLEND_BOTHINVSRCALPHA   = 13,

#define GL_DEPTH_BUFFER_BIT               0x00000100
#define GL_ACCUM_BUFFER_BIT               0x00000200
#define GL_STENCIL_BUFFER_BIT             0x00000400
#define GL_COLOR_BUFFER_BIT               0x00004000


#define GLint int
#define GLenum int
#define GLfloat D3DVALUE
#define GLdouble double


// e.g. persp. correction 
#define GL_FASTEST FALSE
#define GL_NICEST TRUE


/*
 * Untransformed/unlit vertices with Vertexe color ++> DX6 !
 */
#define  D3DFVF_CVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX0)

typedef struct _D3DCVERTEX 
{    float x, y, z;        // position        
	 float nx, ny, nz;     // normal
     DWORD color;  // diffuse color 
     float tu,    // texture coordinates          tv1;     float tu2,
           tv;
} D3DCVERTEX, *LPD3DCVERTEX;


// Forward

class GCamera;
class Matrix;
class GDX7RenderState;

class GDX7VBHandle  {
public: 

	//! the DX7 VertexBuffer 
	LPDIRECT3DVERTEXBUFFER7 pVB;

    int refCnt;				// how many scene nodes are using this buffer
  
	GDX7VBHandle *next;    // Double link to from list of all VB in use
    GDX7VBHandle *prev;

	// only set size 
	GDX7VBHandle(size_t Size=0) : pVB(NULL) { refCnt = 0; next = prev = NULL;}

/*
	// set size and create 
	GDX7VBHandle(LPDIRECT3DDEVICE lpD3DDevice, size_t Size) : G3ExBuf(lpD3DDevice,Size) 
	{	
		refCnt = 0; next = prev = NULL;
	}
*/
	
	~GDX7VBHandle() { Release(); }

    gbool Ok() { return (pVB != NULL); } 

    //! release buffer (After Unlock)
	void Release() 
	{
		HRESULT GLastError;
		if (pVB)
		   GLastError = pVB->Release();
		pVB = NULL;
	}



    void Ref() { refCnt++; }
    void UnRef() { refCnt--;  if (refCnt == 0) Release(); }


};

//
// GDX7VBRef
//
// holds a pointer to a GDX7VBHandle

class GDX7VBRef
{

protected:

public:

    GDX7VBHandle *handle;

public:

    GDX7VBRef() : handle(NULL) { };
    
	// is ExBuf handle valid or need to redefine ?
    gbool Ok() { return (handle && handle->Ok()); }
    
    void Release() {
        if (handle) handle->UnRef();
        handle = NULL;
    }

	operator GDX7VBHandle& () { return *handle; }
/*
    // execute the buffer  release buffer (After Create + Lock/Unlock)
	gbool Execute(LPDIRECT3DDEVICE lpDev, LPDIRECT3DVIEWPORT lpView, DWORD mode = D3DEXECUTE_UNCLIPPED)
	{ return handle->Execute(lpDev,lpView,mode); }
*/

    // Notify handle (owned by renderstate) that this ref doesn´t need the handle any more
    ~GDX7VBRef() {
        if (handle) handle->UnRef();
        handle = NULL;
    }

    friend class GDX7RenderState;
};




//
// GDX7Material
//

   

//@class GL-Style Material
class GDX7Material : public  D3DMATERIAL7 // D3DMATERIAL 
{
public:

/*
	RGBAColor ambient;
	RGBAColor diffuse;
	RGBAColor specular;
	RGBAColor emission;

	float shininess;
	float alpha;
*/
	gbool doSpecular; // flag if specular is not black 

    
	GDX7Material() { }

    RGBAColor& Ambient() { return *((RGBAColor*) &ambient); }
    RGBAColor& Diffuse() { return *((RGBAColor*) &diffuse); }
    RGBAColor& Specular() { return *((RGBAColor*) &specular); }
    RGBAColor& Emissive() { return *((RGBAColor*) &emissive); }
    RGBAColor& Emission() { return *((RGBAColor*) &emissive); }

    GDX7Material & Get(Point &Ambient, Point &Diffuse, Point &Specular,Point &Emissive,float &Shininess,float &Alpha) 
    {
		 Ambient.Set(ambient);
		 Diffuse.Set(diffuse);
		 Specular.Set(specular);
		 Emissive.Set(emissive);
		 Shininess = power;
		 Alpha = diffuse.a;
		 return(*this);
	};


    // set material component wise as "Points"
	GDX7Material & Set(const Point &Ambient, const Point &Diffuse,
				     const Point &Specular,const Point &Emissive,
				     float Shininess,float Alpha) {
		 Ambient.Get(ambient,Alpha);
		 Diffuse.Get(diffuse,Alpha);
		 Specular.Get(specular,Alpha);
		 Emissive.Get(emissive,Alpha);
		 power  = Shininess;
		 doSpecular = (specular.r>0) || (specular.g>0) || (specular.b>0);
		 return(*this);
		};

	GDX7Material &Set(
			// material parameters
			const float ambient[3],const float diffuse[3],const float specular[3],const float emissive[3],
			float alpha,float power) 
	{

    this->diffuse.r = (D3DVALUE)diffuse[0];
    this->diffuse.g = (D3DVALUE)diffuse[1];
    this->diffuse.b = (D3DVALUE)diffuse[2];
    this->diffuse.a = (D3DVALUE)alpha;
    this->ambient.r = (D3DVALUE)ambient[0];
    this->ambient.g = (D3DVALUE)ambient[1];
    this->ambient.b = (D3DVALUE)ambient[2];
    this->ambient.a = (D3DVALUE)alpha;
    this->specular.r = (D3DVALUE)specular[0];
    this->specular.g = (D3DVALUE)specular[1];
    this->specular.b = (D3DVALUE)specular[2];
    this->specular.a = (D3DVALUE)alpha;
    this->emissive.r = (D3DVALUE)emissive[0];
    this->emissive.g = (D3DVALUE)emissive[1];
    this->emissive.b = (D3DVALUE)emissive[2];
    this->emissive.a = (D3DVALUE)alpha;
    this->power = power;
	doSpecular = (this->specular.r>0) || (this->specular.g>0) || (this->specular.b>0);
	return(*this);
	}


	GDX7Material &Set ( // material parameters
				const D3DMATERIAL &mat
				)

	{
		this->diffuse = mat.diffuse;
		this->ambient = mat.ambient;
		this->specular = mat.specular;
		this->emissive = mat.emissive;
		this->power = power;
		doSpecular = (this->specular.r>0) || (this->specular.g>0) || (this->specular.b>0);
		// hTexture / hSpecular

		return(*this);

	}
	
	GDX7Material &Set ( // material parameters
				const D3DMATERIAL7 &mat
				)

	{
		this->diffuse = mat.diffuse;
		this->ambient = mat.ambient;
		this->specular = mat.specular;
		this->emissive = mat.emissive;
		this->power = power;
		doSpecular = (this->specular.r>0) || (this->specular.g>0) || (this->specular.b>0);
		// hTexture / hSpecular

		return(*this);

	}

	GDX7Material &Set ( // material parameters
			const RGBAColor &color
				)

	{
		this->diffuse = color;

		this->ambient.r = 0.0;
		this->ambient.g = 0.0;
		this->ambient.b = 0.0;
		this->ambient.a = 0.0;
		this->specular.r = 0.0;
		this->specular.g = 0.0;
		this->specular.b = 0.0;
		this->specular.a = 0.0;
		this->emissive = color;
		this->power = 0;
		doSpecular = gfalse;

		return(*this);

	}


    // set to a default 
	GDX7Material & Default() {
		ambient.r=ambient.g=ambient.b =0.2f;ambient.a = 1.0f;
		diffuse.r=diffuse.g=diffuse.b =0.8f;diffuse.a = 1.0f;
		specular.r=specular.g=specular.b =0.6f;specular.a = 1.0f;
		emissive.r=emissive.g=emissive.b =0.0f;emissive.a = 1.0f;
		power = 3.0;
		doSpecular = gtrue;
		return(*this);
   }

   // set alpha value 
   GDX7Material & SetAlpha(float a) {
		ambient.a = a;
		diffuse.a = a;
		specular.a = a;
		emissive.a = a;
		return(*this);
   }

   float GetAlpha(float a) {
		return (diffuse.a);
   }

   int io(AStream &a);

   // Read Material
   int ReadFromInventorFile(const char *FileName);

};


//
// @class maintains a handle to a D3D Material
//
class GDX7MaterialHandle 
{
public: 

/////////////////////////////////

	GDX7Material material; // new for custom lighting 

	// custom lighting stuff
	// set material properties
	gbool Set(float ambient[3],float diffuse[3],float specular[3],float emissive[3],float alpha,float power)
	{
		material.Set(ambient,diffuse,specular,emissive,alpha,power);
		defined = TRUE;
		return gtrue;
	}

	gbool SetSolid(const RGBAColor &color)
	{
		material.Set(color);
		defined = TRUE;
		return gtrue;
	}

/////////////////////////////////

    int refCnt; // how many scene nodes are using this material 
    GDX7MaterialHandle *next;    // Double link to from list of all materials in use
    GDX7MaterialHandle *prev;



	GDX7MaterialHandle(): hTexture(0),whiteDiffuse(FALSE), defined(FALSE) { refCnt = 0; next = prev = NULL; }
	~GDX7MaterialHandle() {  }
    
    // release (free) material object
    void Release() { hTexture = 0; defined = 0; }

    D3DTEXTUREHANDLE  hTexture;   // handle for texture, only for ramp

	gbool	whiteDiffuse;	// is the diffuse color white ? 
	gbool   defined;		// was the material defined ??

    // Is definition valid ???
    gbool Ok() { return (defined); }
    void Ref() { refCnt++; }
    void UnRef() { refCnt--;  if (refCnt == 0) Release(); }

	
	D3DTEXTUREHANDLE GetTextureHandle() const { return hTexture;}

	// set material properties
	gbool Set(LPDIRECT3D7 lpD3D,
			LPDIRECT3DDEVICE7 lpDev, 
			// material parameters
			float ambient[3],float diffuse[3],float specular[3],float emissive[3],float alpha,float power,
			
			// D3D specific obsolete in DX7
			DWORD rampSize = 16, 
			D3DTEXTUREHANDLE hTex = NULL  /* Handle for the texture on the material.*/
			) { 

				material.Set(ambient,diffuse,specular,emissive,alpha,power);
				defined = TRUE;

                hTexture = hTex;
				D3DLASTERROR()

				D3DCheck(lpDev->SetMaterial(&material));
				return TRUE;

			}
	

	// set material properties
	gbool Set(LPDIRECT3D7 lpD3D,
			 LPDIRECT3DDEVICE7 lpDev, 
			 // material parameters
             D3DMATERIAL7 &mat) 
          {
			 D3DLASTERROR()

             D3DCheck(lpDev->SetMaterial(&mat));
	         return TRUE;

          }    
	


	// set material to solid single color
	gbool SetSolid(LPDIRECT3D7 lpD3D,
			LPDIRECT3DDEVICE7 lpDev, 
			// material parameters
			//float diffuse[3]
            const RGBAColor &color,
			D3DTEXTUREHANDLE hTex = 0
			) { 
                hTexture = hTex;
				material.Set(color);
				defined = TRUE;
				D3DLASTERROR()
				D3DCheck(lpDev->SetMaterial(&material));
				return TRUE;
			}

};




//
// GDX7MaterialRef
//
// holds a pointer to a GDX7MaterialHandle

class GDX7MaterialRef
{

/* protected: */
public: 

    GDX7MaterialHandle *handle;

public:

    GDX7MaterialRef() : handle(NULL) { };
    // is material handle valide or need to redefine ?

    gbool Ok() { return (handle && handle->Ok()); }
    
    void Release() {
        if (handle) handle->UnRef();
        handle = NULL;
    }

    // Notify handle (owned by renderstate) that this ref doesn´t need the handle any more
    ~GDX7MaterialRef() {
        if (handle) handle->UnRef();
        handle = NULL;
    }

    friend class GDX7RenderState;
};

//
// GDX7TextureHandle
// @class maintains a handle to a D3D Texture
//
class GDX7TextureHandle : public GTextureHandle 
{
public: 

    D3DTEXTUREHANDLE        hTexture;            /* handle */
    LPDIRECTDRAWSURFACE     lpTextureSurf;       /* surface */
    LPDIRECT3DTEXTURE       lpTexture;           /* texture obj */


	GDX7TextureHandle *Next() { return (GDX7TextureHandle *) next; }

	GDX7TextureHandle(): lpTexture(NULL),lpTextureSurf(NULL),hTexture(0) {  
	}

	~GDX7TextureHandle() { Release(); }
    
    // release (free)  object
    virtual void Release() { 
		D3DLASTERROR()
		if (lpTexture) { 
			/*D3DCheck(*/ 
				lpTexture->Unload(); // error here
			//D3DCheck(
				lpTexture->Release(); 
			lpTexture = NULL; 
		}
		if (lpTextureSurf) 
			D3DCheck(lpTextureSurf->Release()); 
		lpTextureSurf = NULL; 
		hTexture = 0; 
		inVideoMem = TRUE;
	}

	// is the surface lost 
	gbool CheckSurfaceLost() 
	{
		HRESULT GLastError;
		if (lpTextureSurf && lpTextureSurf->IsLost() == DDERR_SURFACELOST) { 
             GLastError = lpTextureSurf->Restore(); 
             if (GLastError != DD_OK) return(FALSE);
			 //ReloadTextureSurf();
             return(TRUE);

		}
        else return(FALSE);
	}
	
	inline gbool CheckInVideoMem()  { return inVideoMem; }

	
	// load surface from fileName
	gbool LoadTextureSurf(GD3Device *device,GDX7RenderState &state);  // in GDX7Texture
	gbool LoadTextureSurf(GD3Device *device,GDX7RenderState &state,image &im);
	gbool LoadTextureSurf(GD3Device *device,GDX7RenderState &state,
		LPDIRECTDRAWSURFACE srcSurface,IDirectDrawPalette *srcPalette,
		DDSURFACEDESC &srcDdsd, RECT &srcRect,RECT *targetSizeRect=NULL);
	

	gbool ReloadTextureSurf(GD3Device *device,GDX7RenderState &state);
	gbool UnloadTextureSurf(GD3Device *device,GDX7RenderState &state);


	// get the current texuture handle of the texture object 
    void GetHandle(LPDIRECT3DDEVICE lpDev) {
		D3DLASTERROR()
        D3DCheck(lpTexture->GetHandle(lpDev, &hTexture));
    }

    // Is definition valid ???
    inline gbool Ok() { return !inVideoMem || (lpTextureSurf != NULL && hTexture != 0); }

    void Ref() { refCnt++; }

    void UnRef() { refCnt--;  if (refCnt == 0) Release(); }

	inline operator D3DTEXTUREHANDLE() const { return hTexture; }

	inline D3DTEXTUREHANDLE GetTextureHandle() const { return hTexture;}


	// set texture by texture filename 
	gbool Set(GDX7RenderState &state, LPCTSTR fileName,int format=0, LPCTSTR fileUrl=NULL);

	// set texture from image data
	gbool Set(GDX7RenderState &state, int format,int width,int height,int numComponents,unsigned char *data, char *colorTable=NULL);

	// set texture surface from image data
	// gbool SetTexture(GDX7RenderState &state,
	//		int format,int width,int height,
	//		unsigned char *data, char *colorTable=NULL);

	// set by image 
	gbool Set(GDX7RenderState &state,image *im,gbool checkScale=TRUE,gbool checkFormat=TRUE,gbool filteredScale=TRUE);
	gbool Set(GDX7RenderState &state,LPDIRECTDRAWSURFACE srcSurface,IDirectDrawPalette *srcPalette,DDSURFACEDESC &srcDdsd, RECT &srcRect,gbool checkScale=TRUE,gbool checkFormat=TRUE,gbool filteredScale=TRUE);


    // set texture handle 
    static void put(D3DTEXTUREHANDLE hTex,LPVOID &lpPointer) {
		OP_STATE_RENDER(1, lpPointer);
            STATE_DATA(D3DRENDERSTATE_TEXTUREHANDLE, hTex, lpPointer);
	}

    // set texture handle 
    void put(LPVOID &lpPointer) {
		rendered = TRUE;	
		OP_STATE_RENDER(1, lpPointer);
            STATE_DATA(D3DRENDERSTATE_TEXTUREHANDLE, hTexture, lpPointer);
	}

	// helper for optimizer 
	// return image 
	gbool GetImage(image &im);

};


//
// GDX7TextureHandle
// @class maintains a handle to a D3D Texture // Direct X 6 Version 
//
class GDX7TextureHandle2 : public GTextureHandle 
{
public: 

    LPDIRECTDRAWSURFACE     lpTextureSurf;       /* surface */
    LPDIRECT3DTEXTURE2      lpTexture;           /* texture obj */

	GDX7TextureHandle2 *Next() { return (GDX7TextureHandle2 *) next; }

	GDX7TextureHandle2(): lpTexture(NULL),lpTextureSurf(NULL) {  
	}

	~GDX7TextureHandle2() { Release(); }
    
    // release (free)  object
    virtual void Release() { 
		D3DLASTERROR()
		if (lpTexture) { 
			/*D3DCheck(*/ 
			//	lpTexture->Unload(); // error here
			//D3DCheck(
			lpTexture->Release(); 
			lpTexture = NULL; 
		}
		if (lpTextureSurf) 
			D3DCheck(lpTextureSurf->Release()); 
		lpTextureSurf = NULL; 
		inVideoMem = TRUE;
	}

	// is the surface lost 
	gbool CheckSurfaceLost() 
	{
		HRESULT GLastError;
		if (lpTextureSurf && lpTextureSurf->IsLost() == DDERR_SURFACELOST) { 
             GLastError = lpTextureSurf->Restore(); 
             if (GLastError != DD_OK) return(FALSE);
			 //ReloadTextureSurf();
             return(TRUE);

		}
        else return(FALSE);
	}
	
	inline gbool CheckInVideoMem()  { return inVideoMem; }

	
	// load surface from fileName
	gbool LoadTextureSurf(GD3Device *device,GDX7RenderState &state);  // in GDX7Texture
	gbool LoadTextureSurf(GD3Device *device,GDX7RenderState &state,image &im);
	gbool LoadTextureSurf(GD3Device *device,GDX7RenderState &state,
		LPDIRECTDRAWSURFACE srcSurface,IDirectDrawPalette *srcPalette,
		DDSURFACEDESC &srcDdsd, RECT &srcRect,RECT *targetSizeRect=NULL);
	

	gbool ReloadTextureSurf(GD3Device *device,GDX7RenderState &state);
	gbool UnloadTextureSurf(GD3Device *device,GDX7RenderState &state);


	// get the current texuture handle of the texture object 
    void GetHandle(LPDIRECT3DDEVICE lpDev) {
    }

    // Is definition valid ???
    inline gbool Ok() { return !inVideoMem || (lpTextureSurf != NULL); }

    void Ref() { refCnt++; }

    void UnRef() { refCnt--;  if (refCnt == 0) Release(); }

	//inline operator D3DTEXTUREHANDLE() const { return hTexture; }

	//inline D3DTEXTUREHANDLE GetTextureHandle() const { return hTexture;}


	// set texture by texture filename 
	gbool Set(GDX7RenderState &state, LPCTSTR fileName,int format=0, LPCTSTR fileUrl=NULL);

	// set texture from image data
	gbool Set(GDX7RenderState &state, int format,int width,int height,int numComponents,unsigned char *data, char *colorTable=NULL);

	// set texture surface from image data
	// gbool SetTexture(GDX7RenderState &state,
	//		int format,int width,int height,
	//		unsigned char *data, char *colorTable=NULL);

	// set by image 
	gbool Set(GDX7RenderState &state,image *im,gbool checkScale=TRUE,gbool checkFormat=TRUE,gbool filteredScale=TRUE);
	gbool Set(GDX7RenderState &state,LPDIRECTDRAWSURFACE srcSurface,IDirectDrawPalette *srcPalette,DDSURFACEDESC &srcDdsd, RECT &srcRect,gbool checkScale=TRUE,gbool checkFormat=TRUE,gbool filteredScale=TRUE);


	// helper for optimizer 
	// return image 
	gbool GetImage(image &im);

};









//
// @class light information
//
typedef enum 
	{
	GL_AMBIENT=1,  // do ambient
	GL_SPECULAR=2,  // do specular, comming from material
	GL_RANGE=4,		// do range compuation
	GL_DIFFUSE=8,	// do diffuse
	GL_ATTENUATION=16,	// do atten
	GL_FOG=32,		// do diffuse
	}
	GLIGHT_FLAGS;


class GDX7LightHandle : public D3DLIGHT7 {
public:

	RGBAColor	 ambientColor;			/* light color * ambient intensity */

    Point		 positionLocal;			/* Position in local space */
	Point		 directionLocal;		/* Direction in local space */
	float		 rangeLocal;			// range 

    Point		 positionView;			// Position in view coordinates
	Point		 directionView;			// Direction in view coordinates
	float		 rangeView;				// range 

	float		 cosTheta,cosPhi;		// angles in radians

	int			 flags;					// light flags 

    int refCnt;				// how many scene nodes are using this material 

    GDX7LightHandle *next;    // Double link to form list of all lights in use
    GDX7LightHandle *prev;

	// for custom lighting
	float		 ambientIntensity;

	RGBAColor	 kAmbient,kDiffuse,kSpecular; // cached multiplies // material * light	

    
	// is this light enabled
	gbool enabled;


    void Ref() { refCnt++; }
    void UnRef() { refCnt--;  if (refCnt == 0) Release(); }

	// is the light ok ?? 
    gbool Ok() const { return(enabled); } 

    // release (free) light object
    void Release() {  } 


	gbool Set(LPDIRECT3DDEVICE7   lpDev,
					DWORD dwLightIndex 
			) 
	{
		D3DLASTERROR()
		D3DCheck(lpDev->SetLight(dwLightIndex,this));
		D3DCheck(lpDev->LightEnable(dwLightIndex,enabled));

		return(TRUE);

	}
	
	void Enable(LPDIRECT3DDEVICE7   lpDev,
				DWORD dwLightIndex, 
				BOOL bEnable
			) 
	{
		D3DLASTERROR()
		D3DCheck(lpDev->LightEnable(dwLightIndex,bEnable));
		enabled = !!bEnable;

	}



#if 0

    // release (free) light  object and remove from view 
    void Release(LPDIRECT3DVIEWPORT theView) { }
	
	// set the ligth to lpD3DLight 
	gbool Set(LPDIRECT3D lpD3D)
	{
		if (lpD3DLight == NULL)		
			if (lpD3D->CreateLight(&lpD3DLight, NULL) != D3D_OK)
				return FALSE;
		if (lpD3DLight->SetLight(this) != D3D_OK)
			return FALSE;
		return(TRUE);
		
	}

	// add light to viewport
	gbool Add(LPDIRECT3DVIEWPORT theView) {
		if (theView->AddLight(lpD3DLight) != D3D_OK)
			return FALSE;
		enabled = TRUE;
		lpView = theView;
		return TRUE;
	}

	// remove light from viewport 
	gbool Delete(LPDIRECT3DVIEWPORT theView) {
		enabled = FALSE;
		lpView = NULL;
		if (theView->DeleteLight(lpD3DLight) != D3D_OK)
			return FALSE;
		return TRUE;
	}
#endif


	GDX7LightHandle();

	~GDX7LightHandle();
    
	void SetDirection(const Point direction) { dvDirection = direction; }

	// set type of light 
	void SetType(D3DLIGHTTYPE type) { dltType=type; }
    
	gbool IsDirectional() const { return dltType == D3DLIGHT_DIRECTIONAL;}
    gbool IsSpot() const { return dltType == D3DLIGHT_SPOT;}
    gbool IsPoint() const { return dltType == D3DLIGHT_POINT;}
};

//
// GDX7LightRef
//
// holds a pointer to a GDX7Light

class GDX7LightRef
{

//protected:
public:

    GDX7LightHandle *handle;

public:

    GDX7LightRef() : handle(NULL) { };

    // is light handle valid or need to be redefined ?
    gbool Ok() { return (handle && handle->Ok()); }
    
    void Release() {
        if (handle) handle->UnRef();
        handle = NULL;
    }

    // Notify handle (owned by renderstate) that this ref doesn´t need the handle any more
    ~GDX7LightRef() {
        if (handle) handle->UnRef();
        handle = NULL;
    }

    friend class GDX7RenderState;
};


//
// GDX7Matrix
//
// maintains a handle to a D3D Matrix
/*
  bad DeleteMatrix need lpdev !!!!
  A/ store point to lpdev in matrix
  or build a state matrix handle table 
  Matrix 
	operator D3DMATRIX &() {
		ASSERT(sizeof(D3DMATRIX) == sizeof(m));
		return * (( D3DMATRIX *) &m[0][0]); }
		}
*/
class GDX7Matrix {
public: 

	GDX7Matrix() {}
	void Release() { }

	~GDX7Matrix() { Release(); }


	// set matrix 
	gbool Set(LPDIRECT3DDEVICE7 lpDev,D3DMATRIX &data,D3DTRANSFORMSTATETYPE dtstTransformStateType) 
	{ 

		D3DLASTERROR()

		if (D3DCheck(lpDev->SetTransform(dtstTransformStateType, &data)) != D3D_OK) 
			return FALSE;

		return TRUE; 
	}

	gbool Get(LPDIRECT3DDEVICE7 lpDev,D3DMATRIX &data,D3DTRANSFORMSTATETYPE dtstTransformStateType) 
	{ 
		D3DLASTERROR()

		if (D3DCheck(lpDev->GetTransform(dtstTransformStateType, &data)) != D3D_OK) 
			return FALSE;
		return TRUE; 
	}

	gbool Delete(LPDIRECT3DDEVICE7 lpDev) 
	{ 
		return TRUE; 
	}

	// global identity matrix 
	static D3DMATRIX identity;
};

// class Renderflags : contains rendering style flags
//
class RenderFlags {
public:
		int visible:1;		// item is visible
		int lighted:1;		// item is affected by lights (so pass normals to GL)
		int smooth:1;			// item should be smooth (gouaud) shaded
		int textured:1;		// item should be textured, if parameters available
		int colored:1;		// item should use local colors, if available
		int useMaterialColor:1;	// use Material color (instead of item color )

		RenderFlags(int Visible=1,int Lighted=1,int Smooth=1,int Colored=0,int Textured=0,int UseMaterialColor=1) {
		   visible=Visible,lighted=Lighted,smooth=Smooth;
		   colored=Colored;
		   textured=Textured;
		   useMaterialColor=UseMaterialColor;
		}
};

extern const char * GFilterLookup(int v);
extern int GFilterLookup(const char *v);

//@class GL-Texture mapping options
// texture sub options from  D3DRENDERSTATETYPE
class GDX7TextureParameters {
public :

	// renderstate ops
	D3DTEXTUREFILTER	min_filter;
	D3DTEXTUREFILTER	mag_filter;

	// texture state 
	D3DTEXTUREMINFILTER	tssmin_filter;	//! minification filter 

	D3DTEXTUREMAGFILTER	tssmag_filter;	//! magnification filter 

	D3DTEXTUREMIPFILTER tssmip_filter;	//! mip filter 

    gbool wrap_s;
	gbool wrap_t;

    gbool set_diffuse_white;
	int max_size_x,max_size_y; // maximum allow texture size 
	int current_max_size_x,current_max_size_y; // current allowed texture size , idea scale down textures if few texture memory
	int min_size_x,min_size_y; //  min allowed minimum texture size , idea scale down textures if few texture memory
	gbool square_size;			// true, only square texture size allowed
	gbool border;				// can do border 

	//   BOOL perspective; part of RenderState
    D3DTEXTUREBLEND blend;			// blending function

	D3DTEXTUREOP blendOp; // DX7 to Do 

    D3DTEXTUREBLEND defaultBlend;	// default blend function
    D3DTEXTUREBLEND unlitBlend;		// blend function for unlit textures 

    D3DTEXTUREOP defaultBlendOp;	// default blend function
    D3DTEXTUREOP unlitBlendOp;		// blend function for unlit textures 

	gbool gen_mipmaps;
	int supported_image_fmts;		// supported image formts for texture images ( image.h)
	// border color

    static const IS filters[]; // table of filter names 
    static const IS magFilters[]; // table of filter names 
    static const IS minFilters[]; // table of filter names 
    static const IS mipFilters[]; // table of filter names 
    static const IS blends[];  // table of texture blend names 

	//tbd: check this
	void SetMinFilter(int filter) {
        if (filter>=0) min_filter = (D3DTEXTUREFILTER) filter;
		switch (filter) {
		case D3DFILTER_NEAREST : tssmin_filter = D3DTFN_POINT; tssmip_filter=D3DTFP_NONE; break;
		case D3DFILTER_LINEAR :  tssmin_filter = D3DTFN_LINEAR; tssmip_filter=D3DTFP_NONE; break;

		case D3DFILTER_MIPNEAREST : tssmin_filter = D3DTFN_POINT; tssmip_filter=D3DTFP_POINT; break;
		case D3DFILTER_MIPLINEAR :  tssmin_filter = D3DTFN_LINEAR; tssmip_filter=D3DTFP_POINT; break;

		case D3DFILTER_LINEARMIPNEAREST : tssmin_filter = D3DTFN_LINEAR; tssmip_filter=D3DTFP_POINT; break;
		case D3DFILTER_LINEARMIPLINEAR :  tssmin_filter = D3DTFN_LINEAR; tssmip_filter=D3DTFP_LINEAR; break;

		default : break;	
		}
	};
	void SetMagFilter(int filter) {
        if (filter>=0) mag_filter = (D3DTEXTUREFILTER) filter;
		switch (filter) {
		case D3DFILTER_NEAREST : tssmag_filter = D3DTFG_POINT; break;
		case D3DFILTER_LINEAR : tssmag_filter = D3DTFG_LINEAR; break;
		default : break;	
		}
	};


	void SetWrap(gbool s,gbool t) {
	      wrap_s = s ?  TRUE : FALSE;
	      wrap_t = t ?  TRUE : FALSE;
	} ;

	void GetWrap(gbool &s,gbool &t) {
	      s = wrap_s;
	      t = wrap_t;
	};

	void SetMinFilter(const char *filter) {
        int v = Lookup(filters,filter);
        if (v>=0) SetMinFilter(v);
	};

	void SetMagFilter(const char *filter) {
        int v = Lookup(filters,filter);
        if (v>=0) SetMagFilter(v);
	};

	void SetMipFilter(const char *filter) {
        int v = Lookup(mipFilters,filter);
        if (v>=0) tssmip_filter = (D3DTEXTUREMIPFILTER) v;
	};


	const char * GetMinFilter() { return Lookup(filters,min_filter); }
	const char * GetMagFilter() { return Lookup(filters,mag_filter); }

	void SetBlend(const char *filter) {
        int v = Lookup(blends,filter);
        if (v>=0) blend = defaultBlend = (D3DTEXTUREBLEND) v;
	};
	
	const char * GetBlend(){ return Lookup(blends,blend); }

	void SetUnlitBlend(const char *filter) {
        int v = Lookup(blends,filter);
        if (v>=0) unlitBlend = (D3DTEXTUREBLEND) v;
	};

	const char * GetUnlitBlend(){ return Lookup(blends,unlitBlend); }

	int GetMaxBlend() { return D3DTBLEND_COPY; }

	GDX7TextureParameters();

	// Are mipmap definitions required
	gbool NeedMipmaps() { return( (min_filter >= D3DFILTER_MIPNEAREST) || (gen_mipmaps)); }

	int SetMaxSize(int size_x,int size_y);
	int SetCurrentMaxSize(int size_x,int size_y);

	void GetMaxSize(int &size_x,int &size_y);

	int SetMinSize(int size_x,int size_y);
	void GetMinSize(int &size_x,int &size_y);

};

const char * GFogModeLookup(int v);
int GFogModeLookup(const char *v);

//@class  Fog/Depth Cueing options (glFog)

#define GDX7FOG_VERTEX 1
#define GDX7FOG_TABLE 2
#define GDX7FOG_RANGE 4

class GDX7Fog {
public :
	gbool enable;  // glEnable

    D3DFOGMODE mode;	

	float start,end;
	float density;

	float index;

	gbool useBackgroundColor;

	RGBAColor color; // current color of fog 

	int fogHint;	// vertex, table, range 

    static const IS modes[]; // table of filter names 


	void SetMode(const char *Mode) {
        int v = Lookup(modes,Mode);
        if (v>=0) mode  = (D3DFOGMODE) v;
	};
	const char * GetMode() { return GFogModeLookup(mode); }



    GDX7Fog() {
			enable = 0;
			mode = D3DFOG_EXP ;
			density = 1.0;
			start= 0.0;
			end = 1.0;
			index = 0.0;
			useBackgroundColor = 0;
			color.Gray(0.0);	// Gl Default is Black();
			fogHint = 0;
			};

//	void SetQuality(int i) { if (i) fogHint = GL_NICEST; else fogHint = GL_FASTEST; }
//	int  GetQuality() { return (fogHint == GL_NICEST) ? 1 : 0; }

       	
       	// Evaluate float
       	float Eval(float z);
};



//@enum GRenderMode | predefined render modes
enum GRenderMode { 
			       GRenderVertices,		//@emem render only the vertices
			       GRenderBoundingBoxes,//@emem render bounding box of object only
				   GRenderWireframe, 	//@emem render only the edges
			       GRenderSolid,		//@emem render faces and edges with no lighting
				   GRenderNonLighted,	//@emem render faces with no lighting
				   GRenderFlat,			//@emem render faces with flag lighting
				   GRenderGouraud,		//@emem render faces with smooth lightings
				   GRenderBBox,			//@emem render only bounding box of object 
				   GRenderCustom 		//@emem mode is set to a custom mode
	};

const char * GRenderModeLookup(int v);
int GRenderModeLookup(const char *v);


//
// GDX7RenderState
// 


class GDX7RenderState : public GRenderState 
{

public:

	
	 HRESULT				GLastError; // error variable 

     HWND                   hWnd;       /* handle of window being managed */

	 // a frame counter to measure texture oldiness

	 DWORD					currentFrame; // the current frame number
	 
	 LARGE_INTEGER			currentFrameStartCounter; // QueryPerformanceCounter

	 void IncrementFrame()   { 
					currentFrame++;
					lastFramePrimsDrawn = primsDrawn;
					primsDrawn = 0;
					lastFrameTrisDrawn=trisDrawn;
					trisDrawn = 0;
					}

	 inline DWORD GetCurrentFrame() { return currentFrame; }
	 
	 void ResetFrame()		 {
		 lastFramePrimsDrawn = primsDrawn = 0;
		 lastFrameTrisDrawn=trisDrawn = 0;
		 currentFrame = 2; 
	 }

	 int primsDrawn;			// number of geometric primitves
	 int trisDrawn;				// number of triangles drawn
	 int lastFramePrimsDrawn;
	 int lastFrameTrisDrawn;

	 int currentTriBudget; // estimated target triangle budget for scene


	 gbool IsOldFrame(DWORD nr,DWORD delta) const { return (nr<(currentFrame-delta)); }

	 GD3Device				*device;	/* point to GD3Device needed for device information */
     
     // DD Variables
     LPDIRECTDRAW           lpDD;       /* DirectDraw object */

#if 0

// begin obsolete for DX7
     // D3D Variables

	 LPDIRECT3D             lpD3D;      /* D3D object */
	 LPDIRECT3DDEVICE       lpDev;      /* D3D device */
	 LPDIRECT3DVIEWPORT     lpView;     /* D3D viewport */

#endif

	 // DX 5 Support
	 gbool dx5;
     
	 LPDIRECT3D2              lpD3D2;         /* D3D 2 object */
     LPDIRECT3DDEVICE2        lpD3DDevice2;   /* D3D 2 device */
     LPDIRECT3DVIEWPORT2      lpD3DViewport2; /* D3D 2 viewport, created by application */

	 // DX 6 Support
	 gbool dx6;
     LPDIRECT3D3              lpD3D3;         /* D3D 3 object */
     LPDIRECT3DDEVICE3        lpD3DDevice3;   /* D3D 3 device */
     LPDIRECT3DVIEWPORT3      lpD3DViewport3; /* D3D 3 viewport, created by application */
// end obsolete for DX7

	 // DX 7 Support
	 gbool dx7;
     LPDIRECT3D7              lpD3D7;         /* D3D 7 object */
     LPDIRECT3DDEVICE7        lpD3DDevice7;   /* D3D 7 device */


	 int					  dxLevel;		  // level of direct X supported

	 gbool					  dx6tex;		   // use DX6 Texture interfaces

	 D3DDEVICEDESC7			  caps;			  /* D3D 7 caps */ 


	 DWORD maxBufferSize;		// if >0 max allowed size of execute buffer
	 int maxVertexCount;		// if >0 max allowed nr of points per execute buffer

	 gbool canCull;				// is culling supported (otherwise assume CCW culling done)
	 gbool canDoRgbLighting;	// RGB Mode lighting ? 
	 gbool canDoFog;			// fog ??
	 gbool canDoAlphaBlend;		// ALphablending of Materials & Textues
	 gbool canDoMipMap;			// any mim-map formt ?
	 gbool canDoAlphaStipple;
	 gbool canDoAlphaTest;		// alpha testing working ?
	 
	 D3DTEXTUREFILTER maxMipfilter; // highest level of mip-map min filter supported

	 // copy both faces to execute buffer ?
	 inline gbool NeedBothFaces() { return(!canCull && !backfaceCulling); }

#ifdef GDX7RIGHT_HANDED
	 gbool FlipFaces() { return(!canCull && !frontFaceCW); }
#else
	 gbool FlipFaces() { return(!canCull && frontFaceCW); }
#endif

	 //! do we need to flip normals filling the exbuf ?
	 inline gbool FlipNormals() { return( /* !canCull &&  */frontFaceCW); }


	 //! query D3D capabilitis from lpDev 
	 gbool QueryCaps(D3DDEVICEDESC *pDesc=NULL);

	 //! convert RGBAColor to packed color format
	 static inline 
	 D3DCOLOR getD3DColor(const RGBAColor & color){ return D3DRGBA_X(color );} 
    

	 //! keep track on how many times BeginScene called 
	 int beginSceneCnt;

	 //! D3D BeginScene 
	 HRESULT BeginScene() {
          ASSERT(beginSceneCnt == 0);
		  beginSceneCnt++;
		  HRESULT res=lpD3DDevice7->BeginScene();
          if (res!=D3D_OK) 
                D3DError("BeginScene failed",res);
        return(res);
	 }

	 HRESULT EndScene() {
            ASSERT(beginSceneCnt == 1);

			HRESULT res=lpD3DDevice7->EndScene();
            if (res!=D3D_OK) 
                D3DError("EndScene failed",res);

		beginSceneCnt--;
        return(res);
     }
    
	 //! apply any state changes // DX3
	 void FlushState_() {
		// obsolete in DX 7
	 }

	 
	 inline void FlushState() {
		// obsolete
	 }

	 inline void FlushState(size_t space) {
		// obsolete

	 }
	 
	 // init state buf 
	 void InitState(size_t size=1024) {
	 }
	
	 void TermState() {
	 }

	 // set DX RenderState setting for DX3 .. DX5 
	inline void SetRenderState(D3DRENDERSTATETYPE dwRenderStateType,DWORD dwRenderState) {
		if (dx7) { 
#ifdef _DEBUG
				GLastError = lpD3DDevice7->SetRenderState(dwRenderStateType,dwRenderState);
		        if (GLastError != D3D_OK)  D3DError("SetRenderState() failed",GLastError);

#else
				lpD3DDevice7->SetRenderState(dwRenderStateType,dwRenderState);
#endif
				return;
		}

		if (dx6) { 
#ifdef _DEBUG
				GLastError = lpD3DDevice3->SetRenderState(dwRenderStateType,dwRenderState);
		        if (GLastError != D3D_OK)  D3DError("SetRenderState() failed",GLastError);

#else
				lpD3DDevice3->SetRenderState(dwRenderStateType,dwRenderState);
#endif
				return;
		}
		if (dx5) { 
#ifdef _DEBUG
				GLastError = lpD3DDevice2->SetRenderState(dwRenderStateType,dwRenderState);
		        if (GLastError != D3D_OK)  D3DError("SetRenderState() failed",GLastError);

#else
				lpD3DDevice2->SetRenderState(dwRenderStateType,dwRenderState);
#endif
				return;
		}
	}
	

	// problem with DX5 & Exectue buffers, textures are not pers. correct 
	inline void SetRenderStateTex(D3DRENDERSTATETYPE dwRenderStateType,DWORD dwRenderState) {
		if (dx7) { 
#ifdef _DEBUG
				GLastError = lpD3DDevice7->SetRenderState(dwRenderStateType,dwRenderState);
		        if (GLastError != D3D_OK)  D3DError("SetRenderState() failed",GLastError);

#else
				lpD3DDevice7->SetRenderState(dwRenderStateType,dwRenderState);
#endif
				return;
		}
		if (dx6) { 
#ifdef _DEBUG
				GLastError = lpD3DDevice3->SetRenderState(dwRenderStateType,dwRenderState);
		        if (GLastError != D3D_OK)  D3DError("SetRenderState() failed",GLastError);

#else
				lpD3DDevice3->SetRenderState(dwRenderStateType,dwRenderState);
#endif
				return;
		}
	}

#if 0

// obsolete in DX7

	void SetLightState(D3DLIGHTSTATETYPE dwLightStateType,DWORD dwLightState) {
		if (dx5) { 
#ifdef _DEBUG
				GLastError = lpD3DDevice2->SetLightState(dwLightStateType,dwLightState);
		        if (GLastError != D3D_OK)  D3DError("SetLightState() failed",GLastError);
#else
				lpD3DDevice2->SetLightState(dwLightStateType,dwLightState);
#endif
				return;
		}
	}

	void SetLightStateMat(D3DLIGHTSTATETYPE dwLightStateType,DWORD dwLightState) {
		if (dx6) { 
#ifdef _DEBUG
				GLastError = lpD3DDevice3->SetLightState(dwLightStateType,dwLightState);
		        if (GLastError != D3D_OK)  D3DError("SetLightState() failed",GLastError);
#else
				lpD3DDevice3->SetLightState(dwLightStateType,dwLightState);
#endif
				return;
		}
	}

#endif


     GDX7Material    defaultMaterialValue; // value for defaul material 
     GDX7MaterialRef defaultMaterial;


 	 //GDX7MaterialRef backgroundMaterial;
 	 //GTextureRef  backgroundTexture;


	 DWORD materialRampSize;	// default 16

	 gbool enableOwnLighting;	// global flag to enable custom & transform lighting 
	 gbool enableD3DCulling;	// global flag to enable custom & transform lighting 
	 gbool doOwnLighting;		// do our own lighting and transform
	 gbool doD3DCulling;		// use D3D culling instead of our backface culling

	 gbool lightsTransformed;   // lights already transformed to model space ?
	 gbool lightsTransformedView; // lights already transformed to view space ?
	 
	 gbool materialChanged;		// material changed 
	 gbool matrixChanged;		// model to world matrix changed

	 gbool do2Dclip;			// do we need clipping
	 gbool devNeed2DClip;		// device needs clipping
	 gbool useKatmai;			// use Katmai code	

	 // global D3D D3DTLVERTEX buffer 
	
	 // Array<D3DTLVERTEX> vtlBuffer;
	 FloatArrayAligned  vtlBuffer;

	 // global D3D index buffer 
	 Array<WORD> indexBuffer; // for DrawPrimitiveIndexed
		
	 void *		katmaiState;	// Katmai Transform & Light state data 	

	 GDX7MaterialHandle *currentMaterial; // the current material 
	 
	 // to do: 
	 // void ComputeLighting(Point &p,Point &n,D3DCCOLOR &result);


	 D3DTEXTUREHANDLE hTex;  /* Handle for the texture on the material.*/
     GTextureRef  defaultTexture;


	 Matrix view;
	 Matrix projection;


	 Matrix model2screen; // new 


	 // emulation of texture matrix
	 Matrix textureMatrix;
	 
	 // DX7 has now a texture matrix !!!!!!!!!!!!!!!!!


	 // set texture matrix, version Id is incremented
	 void SetTextureMatrix(const Matrix &m) { 
		 textureMatrix = m; textureMatrixVersion++; textureMatrixIdentity = FALSE; 

		 D3DMATRIX *dm = (D3DMATRIX *) &m.x[0][0];
		 D3DLASTERROR()
		 D3DCheck(lpD3DDevice7->SetTransform(D3DTRANSFORMSTATE_TEXTURE0, dm));
 
	 }

	 // set texture matrix, together with version ID, ID = 0, matrix is identity
	 void SetTextureMatrix(const Matrix &m, unsigned long version) { 
		 textureMatrix = m; textureMatrixVersion=version; textureMatrixIdentity = (version == 0);
		 D3DMATRIX *dm = (D3DMATRIX *) &m.x[0][0];
		 D3DLASTERROR()
		 D3DCheck(lpD3DDevice7->SetTransform(D3DTRANSFORMSTATE_TEXTURE0, dm));
	 
	 }

	 // set texture matrix to identity, together with version ID, ID = 0, matrix is identity
	 void SetTextureMatrixIdentity() { 
		 if (!textureMatrixIdentity) { 
			 textureMatrix.Identity();  textureMatrixIdentity = TRUE; 
			 //D3DMATRIX *dm = (D3DMATRIX *) &textureMatrix.x[0][0];
			 D3DLASTERROR()
			 D3DCheck(lpD3DDevice7->SetTransform(D3DTRANSFORMSTATE_TEXTURE0, NULL));
		} 
	 }


	 // D3D Versions of these matrices
	 GDX7Matrix viewMatrix;
	 GDX7Matrix projMatrix;
	 GDX7Matrix worldMatrix;


    //! set world modelling matrix 
    void SetWorldMatrix(D3DMATRIX &m)
    {
		if (doOwnLighting) {
			lightsTransformed=gfalse;
			matrixChanged=gtrue;
			return;
		}

		worldMatrix.Set(lpD3DDevice7,m,D3DTRANSFORMSTATE_WORLD);
	}

	//! set matrix  to identity
	void SetWorldMatrixIdentity() {
		SetWorldMatrix(GDX7Matrix::identity);
	}

	//! set matrix 
	void SetWorldMatrix(Matrix &m);

    
	//! set view and projection matrix from camera 
    int SetCamera(GCamera *camera);


	//! set the ambient light color 
	void SetAmbientLight() {
		//SetLightState(D3DLIGHTSTATE_AMBIENT,getD3DColor(ambient));
		SetRenderState(D3DRENDERSTATE_AMBIENT,getD3DColor(ambient));
	}

	//
	// set the rendering device context
	//


	void SetDevice(GD3Device *newDevice);

	// one time initialization
	void Init();
	void Term();

    // at beginning of frame
    void SetState();



    GDX7MaterialHandle* materials; // list of all materials in use 
    GDX7TextureHandle* textures; // list of all textures in use 
    
	GDX7LightHandle* lights; // list of all lights in use 

    // Set Material, if result FALSE, need definition of material 
    gbool SetMaterial(GDX7MaterialRef &ref)
    {
		if (doOwnLighting) {
			if (!ref.handle) return FALSE;
			currentMaterial = ref.handle;
			materialChanged=gtrue;
			return TRUE;
		
		}
        if (!ref.Ok()) return(FALSE);
		//SetLightStateMat(D3DLIGHTSTATE_MATERIAL,ref.handle->hMat);
		D3DCheck(lpD3DDevice7->SetMaterial(&ref.handle->material));
        return(TRUE);
    }
    
	gbool SetMaterial(GDX7MaterialRef &ref,gbool whiteDiffuse)
    {
		if (doOwnLighting) {
			if (!ref.handle) return FALSE;
			
			if (!ref.handle->defined) return (FALSE);
			
			if (whiteDiffuse != ref.handle->whiteDiffuse) return (FALSE); 
			
			currentMaterial = ref.handle;

			materialChanged=gtrue;

			return TRUE;
		
		}

        if (!ref.Ok()) return(FALSE);
		if (!canDoRgbLighting && (hTex != ref.handle->hTexture)) return (FALSE); // textures doesn´t match
		if (whiteDiffuse != ref.handle->whiteDiffuse) return (FALSE); 
		D3DCheck(lpD3DDevice7->SetMaterial(&ref.handle->material));
		//SetLightStateMat(D3DLIGHTSTATE_MATERIAL,ref.handle->hMat);
        return(TRUE);
    }

    // Set Material
    gbool SetMaterial(GDX7MaterialRef &ref,			
            // material parameters
			float ambient[3],float diffuse[3],float specular[3],float emissive[3],
			float alpha,float power,gbool whiteDiffuse);

    // Set Material solid 
    gbool SetMaterial(GDX7MaterialRef &ref,const RGBAColor &color);

	// set material properties
	gbool SetMaterial(GDX7MaterialRef &ref,D3DMATERIAL7 &mat);
	
	// set material, but diffuse is white 
	gbool SetMaterialW(GDX7MaterialRef &ref,D3DMATERIAL7 &mat); 
    
    // create a new material handle with a ref cnt of 1 
    GDX7MaterialHandle* CreateMaterial();

    // delete an no longer userd material
    void DeleteMaterial(GDX7MaterialHandle *m);

    // release all D3D Material handles
    void ReleaseMaterials();

    // release all D3D Material handles using textures
    void ReleaseTexturedMaterials();

    // Delete all D3D Material handles
    // should only be called if all references deleted !!!!!!!!!!!!1
    void DeleteMaterials();

    // Delete all D3D Material handles
    // should only be called if all references deleted !!!!!!!!!!!!1
    void DeleteUnusedMaterials();


    // Texture 
	gbool allTexturesInVideo; // all textures are fitting in video

	// D3DDevice texture formats of interest
	int palette8Format;
	int palette4Format;

	int rgbFormat;
	int rgb24Format;
	int rgb16Format;

	int rgbaFormat;
	int rgba32Format;
	int rgba16Format;
	int rgba16_1Format; // 1 bit alpha 

	int gray_format;		// Luminance
	int grayalpha_format;	// Lunminace alpha

	// find a texture format matching srcPixelFormat
	int FindTextureFormat(DDPIXELFORMAT &srcPixelFormat,DDPIXELFORMAT &texFormat);


	//! get allowable image format mask
	int GetSupportedImageFormats() { return textureParameters.supported_image_fmts; }


    //! Set Texture, if result FALSE, need definition of Texture 
    gbool SetTexture(GTextureRef &ref)
    {
		if (dx6tex) 
			return SetTexture((GDX7TextureHandle2 *) ref.handle);

		GDX7TextureHandle *h = (GDX7TextureHandle *) ref.handle; // downcast  
        if (!ref.Ok()) return(FALSE);
		
		if (hTex == h->hTexture) {
			//TRACE("Same texture handle %d \n",hTex);
			return gtrue;
		}

        hTex = h->hTexture;
		h->rendered = TRUE;
		h->lastRenderedFrame = currentFrame;
		SetRenderStateTex(D3DRENDERSTATE_TEXTUREHANDLE,hTex);
        return(TRUE);
    }

	// Direct X 3 Version
    gbool SetTexture(GDX7TextureHandle *h)
    {
		if (h == NULL) return FALSE;
        if (!h->Ok()) return(FALSE);
		if (hTex == h->hTexture) {
			//TRACE("Same texture handle %d \n",hTex);
			return gtrue;
		}

        hTex = h->hTexture;
		h->rendered = TRUE;
		h->lastRenderedFrame = currentFrame;
		SetRenderStateTex(D3DRENDERSTATE_TEXTUREHANDLE,hTex);
        return(TRUE);

	}

	// Direct X 6 Version
    gbool SetTexture(GDX7TextureHandle2 *h)
    {
		if (h == NULL) return FALSE;
        if (!h->Ok()) return(FALSE);
/*		
		if (hTex == h->hTexture) {
			//TRACE("Same texture handle %d \n",hTex);
			return gtrue;
		}

        hTex = h->hTexture;
*/
		h->rendered = TRUE;
		h->lastRenderedFrame = currentFrame;

		D3DCheck(lpD3DDevice3->SetTexture( 0, h->lpTexture));

        return(TRUE);
    }


	// Reload Texture, if it has been flushed 
    gbool ReloadTexture(GTextureRef &ref)
    {	gbool ret = FALSE;
		if (ref.handle == NULL) return (FALSE); // never defined
		
		GDX7TextureHandle *h = (GDX7TextureHandle *) ref.handle; // downcast  

        if (!ref.handle->Ok()) {
		   if (ref.handle->CantLoad()) 
			   return (FALSE); // file lost !!!

		   ret = h->LoadTextureSurf(device,*this);
		}

        hTex = h->hTexture;
		h->rendered = TRUE;
		h->lastRenderedFrame = currentFrame;
		SetRenderStateTex(D3DRENDERSTATE_TEXTUREHANDLE,hTex);
        return(ret);
    }


	// set Texture properties
	gbool SetTexture(GTextureRef &ref,
								LPCTSTR fileName,
								LPCTSTR fileUrl,
								int format = 0,gbool reload=gfalse); 
	// set Texture properties
	// find if a texture with this URL already loaded 
	gbool FindTextureByUrl(GTextureRef &ref,LPCTSTR fileUrl); 

	// set by image data 
	gbool SetTexture(GTextureRef &ref,int glFormat,int width,int height,int numComponents,unsigned char *data);

	// set texture by image structure 
	gbool SetTexture(GTextureRef &ref,image *im,gbool checkScale=TRUE);

	// set texture by DD surface 
	gbool SetTexture(GTextureRef &ref,LPDIRECTDRAWSURFACE srcSurface,IDirectDrawPalette *srcPalette,DDSURFACEDESC &srcDdsd, RECT &srcRect,gbool checkScale=TRUE,RECT *targetSizeRect=NULL);


	gbool SetDefaultTexture(const char *fileName,int format = 0) 
    {
        return(SetTexture(defaultTexture,fileName,NULL,format));
    }

    void ReleaseDefaultTexture() 
    {
      defaultTexture.Release();
    }

/*
	gbool SetBackgroundTexture(const char *fileName,int format = 0) 
    {
        if (SetTexture(backgroundTexture,fileName,NULL,format)) {
			SetBackgroundColor();
			SetTextureOff();
			return(TRUE);
		}
		else return (FALSE);

    }

    void ReleaseBackgroundTexture() 
    {
		backgroundTexture.Release();
    }
*/

    //! create a new Texture handle with a ref cnt of 1 
    GDX7TextureHandle* CreateTexture();

    //! delete an no longer used Texture
    void DeleteTexture(GDX7TextureHandle *m);
    void DeleteTexture(GTextureHandle *m) { DeleteTexture((GDX7TextureHandle *)m); }


	//! compute target texture format size 
	void GetTextureTargetSize(int &width,int &height,int roundUpLimit=128);
    

    //! release all Texture handles
    void ReleaseTextures();

    //! release textures not rendered in the last frame
    gbool ReleaseUnrenderedTextures(int max=1);

    //! release large textures 
    gbool ReleaseLargeTextures(int max=1);

	//! compute the size of the largest textures 
	gbool GetMaxUsedTextureSize(int &sizeX,int &sizeY, int &bytesPerPix);

    //! Delete all Texture handles
    //! should only be called if all references deleted !!!!!!!!!!!!1
    void DeleteTextures();

    //! Delete all unused D3D Texture handles
    gbool DeleteUnusedTextures();

	//! texture memory overflow 
	//! if result TRUE, try to reload texture in progress
	gbool OnTextureMemoryOverflow(GTextureHandle &overflowHandle);


	// lights

	// cache for custom lighting 
	int numLights;
	GDX7LightHandle *firstLight;

	// create a new light handle with a ref cnt of 1 
	GDX7LightHandle* CreateLight();

	// delete a no longer used light
	void DeleteLight(GDX7LightHandle *m);

	// release all D3D Light handles and remove from view 
	void ReleaseLights();

#if 0
	// Delete all D3D Light handles
	// should only be called if all references deleted !!!!!!!!!!!!
	void DeleteLights();

	// Delete all unused D3D Light handles
	void DeleteUnusedLights();

	 // Set Light, define data and add to viewport 
	gbool SetLight(GDX7LightRef &ref); 

	// Remove Light from viewport , but keep structure intact 
	void RemoveLight(GDX7LightRef &ref)
	{
		if (ref.handle != NULL) {
			if (ref.handle->enabled) ref.handle->Delete(lpView);
		}
	}

	// delete a no longer used light, use also to disable light 
	void DeleteLight(GDX7LightRef &ref){
		if (ref.handle != NULL) {
            DeleteLight(ref.handle);
			ref.handle = NULL;
		}
	}


	// get the light handle info structure for light reference 
	// fill in data and call SetLight(ref)
	GDX7LightHandle* GetLight(GDX7LightRef &ref) 
	{
		if (ref.handle == NULL)
            ref.handle = CreateLight();
		return ref.handle;
	}
		


	gbool SetLight(GDX7LightHandle *light)
	{	
		return light->Set(lpD3DDevice7);
	}

	gbool AddLight(GDX7LightHandle *light)
	{	
		return light->Add(lpView);
	}
#endif



	void UpdateAmbientLight(); // recompute ambient light from all lights

	// light stack
	Array<GDX7LightHandle *> lightStack;

	// add/Enable  light at stack index I 
	gbool EnableLight(int index, GDX7LightRef &ref);
	
	// remove light at stack index I 
	gbool DisableLight(int index);

     // Texture options 
	 void SetTextureParameters();
	 

	 // flags keeping track of current render state
	 gbool zbuffer;          // zbuffer enabled ?
	 gbool wbuffer;          // wbuffer used instead of zbuffer ?

	 D3DCMPFUNC depthFunc;  // depth func 
	 
	 gbool zwrite;			// zbuffer write enabled ?

	 gbool dither;			// dithering enabled ?
	 gbool lastPixel;			// TRUE do not render last pixel 

	 gbool alphaStipple;		// alpha stipple pattern enabled ? 
	 gbool noDitherIfTextured;  // dithering enabled in texture mode  ?
	 gbool specular;			// sepcular hightlights
	 int sbuffer;
	 gbool lighting;			// lighting needed
	 int lightmodel;			// light model FLAT/GOURAUD
	 int fillMode;				// POINT LINE FILL 
	 gbool localColors;			// use local face/vertex colors of object
	 gbool backfaceCulling;
	 int cullFront;
	 gbool frontFaceCW;			// glFrontFace


	 gbool texturing;         // using texturing 
	 gbool textureDefined;	// texture has been set
     int textureEnvMode;    //
	 gbool twosidedLighting;
	 gbool localViewer;       // gl local viewer

	 gbool lineSmooth;        // line smooth
	 int colorMaterial;		// glColorMaterial
	 int colorMaterialBack;
	 gbool computePrimitiveList;	// compute tristrips for shells

	 gbool blend;				// blend enabled
	 gbool blendFromMaterial;	// blend test enabled from material

	 D3DBLEND blendSrcFunc,blendDestFunc;	// Alpha blend function

	 gbool alphaTest;			// alpha test enabled
	 gbool alphaTestFromTexture;// alpha test enabled from texture

	 gbool colorKeyEnable;		// DX5 
	 gbool alphaBlend;			// DX5 

	 gbool needBlendForCKey;	// device needs alpha blending for Texture with color key, true for DX3


	 D3DCMPFUNC alphaFunc;		// ALPHA Comparison Function
	 float alphaRef;		
	 DWORD alphaRefI;			// DX integer 0..255

	 float alpha;				// alpha value from material

	 int normalize;				// need normalization of normals
	 float lightScale;			// global light scale factor 

     // Hints 
     int perspectiveCorrectionHint;
     int pointSmoothHint;
     int lineSmoothHint;
     int polygonSmoothHint;
     
     // int fogHint; part of glFog 

	 gbool fogEnabled;				// fog  enabled

	 GDX7Fog fog;			// fog settings
	 GDX7TextureParameters textureParameters;

	 D3DCOLORMODEL colorModel;


#if 0

	 glBackground backgroundOptions;	// background settings

	 glTextureGeneration textureGeneration;

	 glMatrix modelMatrix;
	 glMatrix projectionMatrix;
	 glMatrix textureMatrix;
#endif

	 Transform3 textureTransform3;



	 void SetTextureTransform(const Transform3 &t) 
            {
				textureTransform3 = t;
				if (textureTransform3.IsIdentity())
					SetTextureMatrixIdentity();
				else 
					SetTextureMatrix(textureTransform3.Eval());
			}

     void SetTextureTransform() {
			//glMatrixMode(GL_TEXTURE);
				//glLoadIdentity();
				//textureTransform3.glSet();
			//glMatrixMode(GL_MODELVIEW);
			}


	 void UnSetTextureTransform() {
			textureTransform3.Identity();
			SetTextureMatrixIdentity();

			//glMatrixMode(GL_TEXTURE);
				//glLoadIdentity();
			//glMatrixMode(GL_MODELVIEW);
			}




	 // limits
	 int maxMmatrixStack;
	 int maxLights;
	 int maxTmatrixStack;

	 RGBAColor background;
	 D3DCOLOR  backgroundD3D; // and shadowed in DWORD

	 RGBAColor ambient;		// ambient light color

	 GDX7Material material;
	 GDX7Material backMaterial;


	 RenderFlags vertices;
	 RenderFlags normals;
	 RenderFlags edges;
	 RenderFlags faces;
	 RenderFlags bboxes;

	 RGBAColor currentColor; // current unlit color
	 D3DCOLOR  currentColorD3D; // and shadowed in DWORD

	 
	 RGBAColor vertexColor;
	 RGBAColor edgeColor;
	 RGBAColor faceColor;
	 RGBAColor normalColor;
	 RGBAColor bboxColor;

	 float pointSize;
	 float lineWidth;
     gbool   lineStipple; // glenable();   
     int   lineFactor;
     WORD  linePattern; 


	 gbool	unlit; // flag TRUE create unlit ex.buffers false lit:
	 gbool	litMaterial; //flag TRUE if current material is lighted

	 // create unlit execute buffers 
	 gbool Unlit() { return(!litMaterial || unlit); } 



	 GRenderMode renderMode;


	 gbool LightMode;  // lighted wireframe ...
/*
 	 // Drawing extensions ......
 	 GExtendedDrawStyle *vertexDrawStyle,
	 					*edgeDrawStyle,
	 					*normalDrawStyle,
						*bboxDrawStyle;
*/


	 GDX7RenderState();
	 ~GDX7RenderState();

	 // clear buffers
	 void Clear();
	 
	 // clear buffers
	 void Clear(gbool colorBuffer,gbool zBuffer,gbool stencilBuffer=gfalse,gbool accumBuffer=gfalse); 

	 // set current viewport rectangle
	 void SetViewport(int x,int y,int width, int height);

	 // set current viewport to max rectangle
	 void SetViewport();

	 // get current viewport
	 void GetViewport(int &x,int &y,int &width, int &height);

	 // set global render mode to mode (enum)
     int SetRenderMode(GRenderMode newMode);

	 int SetRenderFlags(RenderFlags &flags);

	 // Set current color, also change material color
	 void SetColor(RGBAColor &c);
	 
	 // Set Rendering Attributes to vertex
	 int SetVertexAttributes();

	 // Set Rendering Attributes to edge
	 int SetEdgeAttributes();

	 // Set Rendering Attributes for faces
	 int SetFaceAttributes(); 

	 // Set Rendering Attributes for normals
	 int SetNormalAttributes(); 

	 // Set Rendering Attributes for bounding boxes
	 int SetBBoxAttributes(); 

	 // set texture flags for faces edges vertices
	 int SetAnyTexturing(int state)
	 {	
		faces.textured = state;
		edges.textured = state;
		vertices.textured = state;

		// disable alpha test if enabled only for texture
		if (!state) {
			if (alphaTestFromTexture) {
				SetAlphaTestFromTexture(FALSE);
			}
			SetTexturing(0);
		}
		return(state);
	 }

	 int SetAnyLighting(int state)
	 {	
		faces.lighted = state;
		edges.lighted = state;
		vertices.lighted = state;
		return(state);
	 }


	 
	 int SetFaceTexturing(int state)
	 {	
		faces.textured = state;
		if (state && faces.visible)
			edges.textured = 0;
		return(state);
	 }

	 int SetEdgeTexturing(int state)
	 {	
		edges.textured = state;
		if (state && edges.visible)
			vertices.textured = 0;
		return(state);
	 }

	 int SetEdgeLighting(int state)
	 {	
		edges.lighted = state;
		return(state);
	 }


     // set all cached glstate parameters 
     void SetContextAll();
	 
	 // Set Z-Buffer Mode  on/off
	void SetZBuffer(gbool Zbuffer) {
		if (zbuffer == Zbuffer) return;
		zbuffer = Zbuffer;
        SetZBuffer();
    }

    void SetZBuffer() {
		//if (zbuffer) glEnable(GL_DEPTH_TEST);
		//else glDisable(GL_DEPTH_TEST);
        SetRenderState(D3DRENDERSTATE_ZENABLE, (zbuffer ? (wbuffer ? D3DZB_USEW: D3DZB_TRUE) : D3DZB_FALSE));
         //SetRenderState(D3DRENDERSTATE_ZVISIBLE, zbuffer);

	};
	
	D3DZBUFFERTYPE	GetZBufferType() { return (zbuffer ? (wbuffer ? D3DZB_USEW: D3DZB_TRUE) : D3DZB_FALSE); }

	 // Set Z-Buffer Mode  on/off
	void SetZWrite(gbool ZWrite) {
		if (zwrite == ZWrite) return;
		zwrite = ZWrite;
        SetZWrite();
    }

    void SetZWrite() {
        SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, zwrite);
	};

    void SetDepthFunc() {
		SetRenderState(D3DRENDERSTATE_ZFUNC, depthFunc); 
	};

    void SetDepthFunc(int /*D3DCMPFUNC*/ DepthFunc) {
		depthFunc= (D3DCMPFUNC) DepthFunc;
		SetRenderState(D3DRENDERSTATE_ZFUNC, depthFunc); // 29.05.99
	};

	//! Set Dither Mode  on/off
	void SetDither(gbool Dither) {
		if (dither == Dither) return;
		dither = Dither;
        SetDither();
    }

    void SetDither() {
        SetRenderState(D3DRENDERSTATE_DITHERENABLE, dither);
	};

	//! Set Specular Mode  on/off
	void SetSpecular(gbool Specular) {
		if (specular == Specular) return;
		specular = Specular;
        SetSpecular();
    }

    void SetSpecular() {
        SetRenderState(D3DRENDERSTATE_SPECULARENABLE, specular);
	};

	//! Set SetLastPixelMode on/off
	void SetLastPixelMode(gbool LastPixel) {
		if (lastPixel == LastPixel) return;
		lastPixel = LastPixel;
        SetLastPixelMode();
    }

    void SetLastPixelMode() {
		//if (dither) glEnable(GL_DEPTH_TEST);
		//else glDisable(GL_DEPTH_TEST);
        SetRenderState(D3DRENDERSTATE_LASTPIXEL, lastPixel);
	};



	// Set Alpha Stipple  on/off
	void SetAlphaStippleDither(gbool AlphaStipple) {
		if (alphaStipple == AlphaStipple) return;
		alphaStipple = AlphaStipple;
        SetAlphaStipple();
    }

    void SetAlphaStipple() {
         SetRenderState(D3DRENDERSTATE_STIPPLEDALPHA, alphaStipple);
	};


	void SetBlend(gbool Blend) {
		if (blend == Blend) return;
		blend = Blend;
        SetBlend();
    }

    void SetBlend() {
		//if (blend) glEnable(GL_BLEND);
		//else glDisable(GL_BLEND);
		SetRenderState(D3DRENDERSTATE_BLENDENABLE, blend);
	};


	void SetAlphaBlend(gbool Blend) {
		if (alphaBlend == Blend) return;
		alphaBlend = Blend;
        SetAlphaBlend();
    }

    void SetAlphaBlend() {
		if (!dx5) return;
		SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend);

	};

	void SetColorKeyEnable(gbool Blend) {
		if (colorKeyEnable == Blend) return;
		colorKeyEnable = Blend;
        SetColorKeyEnable();
    }

    void SetColorKeyEnable() {
		if (!dx5) return;
		SetRenderState(D3DRENDERSTATE_COLORKEYENABLE, colorKeyEnable);

	};



	void SetBlendFromMaterial(gbool Blend) {
		if (blend == Blend) return;
		blend = Blend;
		blendFromMaterial = blend;
        SetBlend();
    }

	void SetUnlit();

	void SetUnlit(gbool Unlit) {
		if (Unlit == unlit) return;
		unlit = Unlit;
        SetUnlit();
    }

	void SetLitMaterial(gbool Lit) {
		if (Lit == litMaterial) return;
		litMaterial = Lit;
    }


    void SetBlendFunc(int BlendSrcFunc,int BlendDestFunc) 
	{
		blendSrcFunc=(D3DBLEND)BlendSrcFunc,blendDestFunc=(D3DBLEND)BlendDestFunc; 
		//glBlendFunc(blendSrcFunc,blendDestFunc);
		if (dx5) {
	      lpD3DDevice2->SetRenderState(D3DRENDERSTATE_SRCBLEND  , blendSrcFunc);
		  lpD3DDevice2->SetRenderState(D3DRENDERSTATE_DESTBLEND , blendDestFunc);
		  return ;
		}

	}


	void SetMaterialAlpha(float Alpha) 
	{
		alpha=Alpha;

		if (alpha < 1.0) {
			SetBlendFromMaterial(TRUE);
		}
		else SetBlendFromMaterial(FALSE);
	}

	void SetMaterialAlphaOff() 
	{
		alpha=1.0;
		SetBlendFromMaterial(FALSE);
	}

	// enable alpha test, needed for transparent textures 
	void SetAlphaTest(gbool AlphaTest) {
		if (alphaTest == AlphaTest) return;
		alphaTest = AlphaTest;
        SetAlphaTest();
    }

    void SetAlphaTest() {
		//if (alphaTest) glEnable(GL_ALPHA_TEST);
		//else glDisable(GL_ALPHA_TEST);
	    SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE , alphaTest);

	};


	void SetAlphaTestFromTexture(gbool AlphaTest) {
		if (alphaTest == AlphaTest) return;
		alphaTest = AlphaTest;
		alphaTestFromTexture = alphaTest;
        SetAlphaTest();
    }

    void SetAlphaFunc(int AlphaFunc,float AlphaRef) 
	{
		if (alphaFunc !=(D3DCMPFUNC) AlphaFunc) {
			alphaFunc=(D3DCMPFUNC) AlphaFunc;
			SetRenderState(D3DRENDERSTATE_ALPHAFUNC, alphaFunc);
		}

		if (alphaRef != AlphaRef) {
			alphaRef=AlphaRef;
			// glAlphaFunc(alphaFunc,alphaRef);
			alphaRefI = 255.0*alphaRef;
			if (alphaRefI<0)  alphaRefI = 0;
			else if (alphaRefI>255)  alphaRefI = 255;
			SetRenderState(D3DRENDERSTATE_ALPHAREF, alphaRefI);
		}

	}
    void SetAlphaFunc() 
	{
		SetRenderState(D3DRENDERSTATE_ALPHAFUNC, alphaFunc);
		SetRenderState(D3DRENDERSTATE_ALPHAREF, alphaRefI);

	}


	void SetPointSize(float PointSize) {
		pointSize = PointSize;
		//glPointSize(pointSize);
	}
	void SetPointSize() {
		//glPointSize(pointSize);
	}

    void SetLineWidth(float width) {
		lineWidth = width;
		//glLineWidth(lineWidth);
	}
    void SetLineWidth() {
		//glLineWidth(lineWidth);
	}

    void SetLineStipple()
    {
        if (lineStipple) {
            //glLineStipple(lineFactor,linePattern);
            //glEnable(GL_LINE_STIPPLE);
        }
        else {
            //glDisable(GL_LINE_STIPPLE);
        }
    }

	 // Set Auto normalize  on/off
	 void SetNormalize(int Normalize) {
			if (normalize == Normalize) return;
			normalize = Normalize;
			//if (normalize) glEnable(GL_NORMALIZE);
			//else glDisable(GL_NORMALIZE);
	};

	 void SetNormalize() {
			//if (normalize) glEnable(GL_NORMALIZE);
			//else glDisable(GL_NORMALIZE);
	 };

	 // Set Light Mode on/off
	 void SetLighting(gbool Lighting) 
	 {
			if (lighting == Lighting) return;
			lighting = Lighting;
			SetLighting();
	 };
	 
     void SetLighting() 
	 {
			//if (lighting) glEnable(GL_LIGHTING);
			//else glDisable(GL_LIGHTING);
			// ownlighting ??
			SetRenderState(D3DRENDERSTATE_LIGHTING,lighting); // new in DX7             
	 };

	 void SetLightModel(int Lightmodel) {
			if (lightmodel == Lightmodel) return;
			lightmodel = Lightmodel;
            SetLightModel();
	 };
	 
	 void SetLightModel() {
            SetRenderState(D3DRENDERSTATE_SHADEMODE, lightmodel);
	 };

     void SetFillMode(int mode) {
			if (fillMode == mode) return;
			fillMode = mode;
            SetFillMode();
	 };
	 
	 void SetFillMode() {
			//glShadeModel(lightmodel);
            SetRenderState(D3DRENDERSTATE_FILLMODE, fillMode);
	 };

	
	// set / get lightScale factor
	
	void SetLightScale(float factor) { lightScale = factor; }
	
	float GetLightScale() { return(lightScale); }


	 void SetTwoSidedLighting(gbool Lighting) {
			if (Lighting == twosidedLighting) return;
			twosidedLighting = Lighting;
			//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,twosidedLighting);
	 };

     void SetTwoSidedLighting() {
			//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,twosidedLighting);
	 };

	 // mode = 0 specular reflection is computed fom the origin of eye coordinate sys.
	 // otherwise simply in direction of - z
	 void SetLocalViewer(gbool mode) {
			if (mode == localViewer) return;
			localViewer = mode;
			//glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,mode);
	 };
	 
     void SetLocalViewer() {
			//glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,localViewer);
	 };

	 gbool SetMaterial(const GDX7Material &Material) {
			//material = Material;
			//material.glSet();
			//if (localColors) material.diffuse.glSet();
            defaultMaterialValue = Material;
            return SetMaterial();


	 };
	 gbool SetMaterial() {
		//material.glSet();
		//if (localColors) material.diffuse.glSet();
         return SetMaterial(defaultMaterial,defaultMaterialValue);
	 };

	 // set background color = gl clear color
	 void SetBackgroundColor(const RGBAColor &Color) {
	     background = Color;
		 backgroundD3D = getD3DColor(background);

         SetBackgroundColor();
	 };

     void SetBackgroundColor() { 
		 // new using clear in DX7
	 };

	 //! set current color // e.g. material diffuse, needed for unlit execute buffers
	 void SetCurrentColor(const RGBAColor &Color) {
	     currentColor = Color;
		 currentColorD3D = getD3DColor(currentColor);
	 };

	 void SetCurrentColor(const Point &Color) {
	     currentColor = Color;
		 currentColorD3D = getD3DColor(currentColor);
	 };

	 void SetCurrentColor(float r,float g,float b, float a=1.0f) {
	     currentColor.Set(r,g,b,a);
		 currentColorD3D = getD3DColor(currentColor);
	 };

	 void SetFogEnable(gbool flag) 
	 {
		 if (fogEnabled == flag) return;
		 fogEnabled = flag;
		 SetRenderState(D3DRENDERSTATE_FOGENABLE, fogEnabled);
	 }		 

	 
	 void SetFog_(); // lowlevel set 
	

	 // apply the fogsettings described in the Fog structure
	 void SetFog() {
		 if (fog.enable) {
			 SetFog_();
		 }
		 else {
			SetFogEnable(FALSE);
		 }
	};

	 // set global ambient light color
	 void SetAmbientColor(const RGBAColor &Color) {
	     ambient = Color;
		 ambient.a = Color.r;
		 SetAmbientColor();
	 };

	 void SetAmbientColor() {
		//SetLightState(D3DLIGHTSTATE_AMBIENT, getD3DColor(ambient));
		SetRenderState(D3DRENDERSTATE_AMBIENT,getD3DColor(ambient));
	 };

	 void SetColorModel(D3DCOLORMODEL model)
	 {
		colorModel = model;
		//SetLightState(D3DLIGHTSTATE_COLORMODEL, model);
	 }

	 //! Enable display of local vertex colors in lighted mode
	 void SetLocalColors(gbool mode) {
		  if (mode == localColors) return;
		  localColors = mode;
          SetLocalColors();
     }

     void SetLocalColors() {
		SetRenderState(D3DRENDERSTATE_COLORVERTEX,localColors); // new in DX 7
	 };

     // disable / enable backface culling 
	 inline void SetBackfaceCulling(gbool mode) {
		  if (mode == backfaceCulling) return;
		  backfaceCulling = mode;

          SetCulling(backfaceCulling,frontFaceCW);
	 };
	 
     void SetBackfaceCulling() {
         SetCulling();
	 };

	 // mode 0 Front Facing polyongs are oriented counter clock wise
	 // mode 1   " 	   "						  clock wise
	 inline void SetFrontFaceCW(gbool mode) {
		  if (mode == frontFaceCW) return;
		  //glFrontFace(mode ? GL_CW : GL_CCW);
		  frontFaceCW = mode;
          SetCulling_(backfaceCulling,frontFaceCW);
	 };

	 inline void SetFrontFaceCW() {
		  //glFrontFace(frontFaceCW ? GL_CW : GL_CCW);
         SetCulling();
	 };

	 // set both culling params in one call 
	 inline void SetCulling(gbool cull, gbool modeCW) {
		 backfaceCulling = cull;
		 frontFaceCW = modeCW;
         SetCulling_(backfaceCulling,frontFaceCW);

	 }

	 // do render state settings if supported 
	 inline void SetCulling_(gbool cull, gbool modeCW) {
			if (!canCull) return;

            D3DCULL c;
            if (cull) {
                if (modeCW) c= D3DCULL_CW;
                else c = D3DCULL_CCW;
            }    
            else c = D3DCULL_NONE;
            SetCullMode(c);
     }

	 D3DCULL currentCullMode;

 	 inline void SetCullMode(D3DCULL cullMode) {
		if (cullMode != currentCullMode) {
			currentCullMode = cullMode;
			SetRenderState(D3DRENDERSTATE_CULLMODE,cullMode);
		}
	}

     
     void SetCulling() {
        SetCulling(backfaceCulling,frontFaceCW);
     }



     // disable/enable texturing 
	 void SetTexturing(gbool mode) {
		  if (mode == texturing) return;
		  if (mode ) {
			 //glEnable(GL_TEXTURE_2D);
			 //textureGeneration.glEnable_();
		  }
		  else {
		    //glDisable(GL_TEXTURE_2D);
			//textureGeneration.glDisable_();
			SetTextureOff();
		  };
		  texturing = mode;
	 };

	 void SetTexturing() {
		  if (texturing) {
			 //glEnable(GL_TEXTURE_2D);
			 //textureGeneration.glEnable_();
		  }
		  else {
		    //glDisable(GL_TEXTURE_2D);
			//textureGeneration.glDisable_();
			SetTextureOff();
		  };
	 };

	 // if flag set_diffuse_white is set, set WHite on Texturing
	 void SetTexturingDiffuseColor(int setBorder=1) {
		if (texturing && textureParameters.set_diffuse_white)
		{   //RGBAColor w;
			//w.White();
			//glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,&w.r);
			//w.glSet();
			//if (setBorder) glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,&material.diffuse.r);
		}
	};

    // set texture wrapping 
	void SetTextureWrap(gbool wrap_s, gbool wrap_t);
#if 0
	{
        if ( wrap_s != textureParameters.wrap_s 
			 || wrap_t != textureParameters.wrap_t) 
		{

			textureParameters.wrap_s = wrap_s;
			textureParameters.wrap_t = wrap_t;


			OP_STATE_RENDER(2, stateBuf.lpPointer);
				STATE_DATA(D3DRENDERSTATE_WRAPU, wrap_s, stateBuf.lpPointer);
				STATE_DATA(D3DRENDERSTATE_WRAPV, wrap_t, stateBuf.lpPointer);
		}
	}
#endif

    // set texture filtering
	void SetTextureBlend(D3DTEXTUREBLEND blend) 
	{
		if (blend != textureParameters.blend) {;
			textureParameters.blend = blend;
			SetRenderState(D3DRENDERSTATE_TEXTUREMAPBLEND, blend);
		}

	}

	void SetTextureBlend_(D3DTEXTUREBLEND blend) {
		textureParameters.blend = blend;
	    SetRenderState(D3DRENDERSTATE_TEXTUREMAPBLEND, blend);

	}



    // set texture handle
    void SetTexture(D3DTEXTUREHANDLE hTex) {
		this->hTex = hTex;
		SetRenderStateTex(D3DRENDERSTATE_TEXTUREHANDLE,hTex);
	}

    void SetTextureOff() {
		// if dx6Tex
		if (hTex != 0) {
			hTex = 0;
			SetRenderStateTex(D3DRENDERSTATE_TEXTUREHANDLE,hTex);
		}
	}


	 // miMap LodBias 
	 float mipMapLodBias; // def 0 normally -1 ..1 

	 void SetMipMapLodBias(float offset) {
		 if (offset != mipMapLodBias) {		 
			mipMapLodBias = offset;
			SetMipMapLodBias();
		 }	
     }    
	 
	 float GetMipMapLodBias() {
		 return mipMapLodBias;
	 }
	 
     void SetMipMapLodBias() {
		SetRenderState(D3DRENDERSTATE_MIPMAPLODBIAS,mipMapLodBias);
	 };

	 int zBias; // 0 .. 16, default 0 

	 void SetZBias(int offset) {
		 if (offset != zBias) {		 
			zBias = offset;
			SetZBias();
		 }	
     }    
	 
	 int GetZBias() {
		 return zBias;
	 }
	 
     void SetZBias() {
		SetRenderState(D3DRENDERSTATE_ZBIAS ,zBias);
	 };


	 // antialiasing 
	 
	 D3DANTIALIASMODE antialisMode;

	 void SetAntialiasing(gbool mode) {
		 if (mode )
		    antialisMode =	 D3DANTIALIAS_SORTINDEPENDENT;
		 else antialisMode =  D3DANTIALIAS_NONE;
         SetAntialiasing();
     }    
	 
	 gbool GetAntialiasing() {
		 return antialisMode > D3DANTIALIAS_NONE;
	 }
	 
     void SetAntialiasing() {
		SetRenderState(D3DRENDERSTATE_ANTIALIAS, antialisMode);
	 };



	 void SetLineAntialiasing(gbool mode) {
         lineSmooth = mode;
         SetLineAntialiasing();
     }    
	 
     void SetLineAntialiasing() {
		// SetRenderState(D3DRENDERSTATE_ANTIALIAS, lineSmooth);
	 };
     
     void SetPerspectiveCorrectionHint(int hint) {
         perspectiveCorrectionHint = hint;
    	 //glHint(GL_PERSPECTIVE_CORRECTION_HINT,perspectiveCorrectionHint);
         SetRenderState(D3DRENDERSTATE_TEXTUREPERSPECTIVE, perspectiveCorrectionHint);
     }   

     void SetComputePrimitiveList(gbool flag) { computePrimitiveList = flag; }


	 // primitive Rendering 
	 void RenderVertex(Point &p);
	 void RenderLine(Point &p1,Point &p2);
	 void RenderBBox(Point &p1,Point &p2);
	 void RenderCylinder(Point &p, float r, float h);
	 void RenderCone(Point &p, float r, float h);
	 //void RenderSphere(Point &p,float r);

	 // get GL limits of interes for our renderer
	 void GetLimits() {
		 //GLint i;
		 //glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH,&i); maxMmatrixStack=i;
		 //glGetIntegerv(GL_MAX_TEXTURE_STACK_DEPTH,&i); maxTmatrixStack=i;
		 //glGetIntegerv(GL_MAX_LIGHTS,&i); maxLights=i;
		 maxLights = 128; // some number
	  };

    


	 // End of rendering to this frame
	 void Finish()
	 {
		//glFinish();
	 };

 	 // cache
	 //glCache modelCache;
	 //glCache textureCache;


	 int (*AbortChecker)(GDX7RenderState &s);
	 void * AbortCheckerData;
	 int doAbortCheck;

	 float LastFrameTime; // Time needed to do last update
	 
	 // Check if redraw should be aborted
	 int Abort() {  if (doAbortCheck)  return(AbortChecker(*this));
	 				return(0);
			    }
	// 
	int bufferMode; // 0 swapbuffers, 1 front buffer, 2 scale

	void SetBufferMode(int mode);
	int GetBufferMode() { return(bufferMode); }
	int SwapBuffers();
	
	int UpdateFrontBuffer();


	//@cmember viewport (window) size in pixel
	int sizex,sizey;
	
	//@cmember rendering viewport size in pixel
	int renderSizex,renderSizey;

	void SetSize(int sizeX,int sizeY);
	void SetRenderSize(int sizex,int sizey);

	// Primitives
	void RenderSphere(Point &center, float r=1.0,int level=1, int mask = 1, int VN=1, int FN=0);
	
	// render a wireframe cube 
	void RenderCubeEdges(double x0, double y0,double z0,double x1,double y1,double z1);
	void RenderCubeEdges(const Point &p0,const Point &p1);

	enum RenderElements {
		RVertices = 1,
		REdges = 2,
		RFaces = 4,
		RNormals = 8,
		RBBox	= 16
	};

	// caching help for qv
	// the material node, which recently has been set
	void * currentMaterialNode;
	void * currentBaseColorNode;
	int currentMaterialIndex;
	void *currentTexture2Node;
	void *currentCameraNode;
	int currentMaxLight;

	void FlushCurrent() {
		currentMaterialNode = 0;
		currentBaseColorNode = 0;
		currentMaterialIndex = 0;
		currentTexture2Node = 0;
		currentCameraNode = 0;
		currentMaxLight=0;
	}

    
    // get functions pointers for extensions
    int InitializeExtensions() { return(0); }

    // ******** end from glRender.h

};

#endif _DX7


#endif
