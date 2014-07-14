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

#ifndef _GRenderBase_
#define _GRenderBase_

/******************************************************************************
@doc

@module GRenderBase.h - GLView RenderState base classes  |

Copyright (c) 1996-1999	by Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:

Changes:

$Revision: 1.2 $
$Log: grenderbase.h,v $
Revision 1.2  1999/07/06 16:54:45  tom
*** empty log message ***


Todo :

******************************************************************************/


// forwards 
class GRenderStateBase;
class GTextureHandle;
class GRenderer2D;

typedef struct image_ image;



//@class a simple transform stored component wise
class Transform3 {
public :

	Point scale;
	Point rotate;
	Point translate;

	Transform3() : scale(1.0),rotate(0.0), translate(0.0) {};

	Transform3(Point Scale,Point Rotate, Point Translate) :
		scale(Scale),rotate(Rotate),translate(Translate) {} ;
	
	void Identity() { scale.Set(1.0,1.0,1.0);
					  rotate.Set(0.0,0.0,0.0);
					  translate.Set(0.0,0.0,0.0);
					 };

	gbool IsIdentity() const { return scale == Point(1.0,1.0,1.0) && 
							   rotate == Point(0.0,0.0,0.0) &&
							   translate == Point(0.0,0.0,0.0);
					  }
	
	Matrix Eval() const
	{  Matrix m;
		m=ScaleMatrix(scale);
		m*=RotationMatrix(rotate);
		m*=TranslationMatrix(translate);
		return(m);
	}


};



//@class Red Green Blue Alpha color in float
class RGBAColor 

#ifdef _D3D
: public D3DCOLORVALUE 
#endif

{
public:


#ifndef _D3D
	//! the color components (range 0..1)
	float r,g,b,a;
#endif


  

	RGBAColor(float R=0.0,float G=0.0, float B=0.0,float A=1.0) { r=R,g=G,b=B,a=A; };
	RGBAColor(const Point &p,float A=1.0) { r=p.x,g=p.y,b=p.z,a=A; };

#ifdef _D3D
	RGBAColor(const D3DCOLORVALUE &c) { * ((D3DCOLORVALUE*) this) = c; };
#endif

	RGBAColor& Set(float R,float G, float B,float A=1.0) { r=R,g=G,b=B,a=A; return(*this); };

	RGBAColor& Set(const Point &p,float A=1.0) { r=p.x,g=p.y,b=p.z,a=A; return(*this); };
	RGBAColor& Get(Point &p) { p.Set(r,g,b); return(*this); };
	RGBAColor& SetAlpha(float A=1.0) { a=A; return(*this); };

	RGBAColor& White() { r=g=b=1.0; a=1.0; return (*this); };
	RGBAColor& Black() { r=g=b=0.0; a=1.0; return (*this); };
	RGBAColor& Gray(float gray) { r=g=b=gray; a=1.0; return (*this);};

#ifdef _D3D
	// convert to packed color format
	//operator D3DCOLOR() const { return(D3DRGBA_X(r, g, b,a )); } 
#endif


	operator float* () {return &r;};		// type convert to float * (float[4]) 
	
	// input/ouput
	int io(AStream &a,int fmt=0);

	// Unary operators returning a new RGBAColor
	RGBAColor operator- () const;
	RGBAColor operator+ () const;


	// Assignment operators
	RGBAColor& operator+= (const RGBAColor& A);
	RGBAColor& operator-= (const RGBAColor& A);
	RGBAColor& operator*= (const RGBAColor& A);
	RGBAColor& operator/= (const RGBAColor& A);
	RGBAColor& operator+= (float A);
	RGBAColor& operator-= (float A);
	RGBAColor& operator*= (float A);
	RGBAColor& operator/= (float A);

	// Binary operators returning a new RGBAColor
	friend RGBAColor operator+ (const RGBAColor& A, const RGBAColor& B);
	friend RGBAColor operator- (const RGBAColor& A, const RGBAColor& B);
	friend RGBAColor operator* (const RGBAColor& A, const RGBAColor& B);
	friend RGBAColor operator* (const RGBAColor& A, float B);
	friend RGBAColor operator* (float A, const RGBAColor& B);
	friend RGBAColor operator* (const Matrix&, const RGBAColor&);
	friend RGBAColor operator/ (const RGBAColor& A, const RGBAColor& B);
	friend RGBAColor operator/ (const RGBAColor& A, float B);

	friend int operator< (const RGBAColor& A, const RGBAColor& B);



};

// io to AStream 
//int io(D3DCOLORVALUE &this_,AStream &a,int fmt=0);

// set color to a gray value
//void SetGray(D3DCOLORVALUE &v, float gray,float alpha=1.0);

inline RGBAColor RGBAColor::operator+ () const
{
	return *this;
}

inline RGBAColor RGBAColor::operator- () const
{
	return RGBAColor(-r, -g, -b, -a);
}

inline RGBAColor& RGBAColor::operator+= (const RGBAColor& A)
{
	r += A.r;  g += A.g; b += A.b; a += A.a;
	return *this;
}

inline RGBAColor& RGBAColor::operator-= (const RGBAColor& A)
{
	r -= A.r;  g -= A.g; b -= A.b; a -= A.a;
	return *this;
}

inline RGBAColor& RGBAColor::operator+= (float A)
{
	r += A;  g += A;  b += A;  a += A;
	return *this;
}

inline RGBAColor& RGBAColor::operator-= (float A)
{
	r -= A;  g -= A;  b -= A;  a -= A;
	return *this;
}

inline RGBAColor& RGBAColor::operator*= (float A)
{
	r *= A;  g *= A;  b *= A; a *= A;
	return *this;
}

inline RGBAColor& RGBAColor::operator/= (float A)
{
	r /= A;  g /= A;  b /= A; a /= A;
	return *this;
}

inline RGBAColor& RGBAColor::operator*= (const RGBAColor& A)
{
	r *= A.r;  g *= A.g;  b *= A.b; a *= A.a;
	return *this;
}

inline RGBAColor& RGBAColor::operator/= (const RGBAColor& A)
{
	r /= A.r;  g /= A.g;  b /= A.b;  a /= A.a;
	return *this;
}

inline RGBAColor operator+ (const RGBAColor& A, const RGBAColor& B)
{
	return RGBAColor(A.r + B.r, A.g + B.g, A.b + B.b, A.a + B.a);
}

inline RGBAColor operator- (const RGBAColor& A, const RGBAColor& B)
{
	return RGBAColor(A.r - B.r, A.g - B.g, A.b - B.b , A.a - B.a);
}

inline RGBAColor operator* (const RGBAColor& A, const RGBAColor& B)
{
	return RGBAColor(A.r * B.r, A.g * B.g, A.b * B.b, A.a * B.a);
}

inline RGBAColor operator* (const RGBAColor& A, float B)
{
	return RGBAColor(A.r * B, A.g * B, A.b * B, A.a * B);
}

inline RGBAColor operator* (float A, const RGBAColor& B)
{
	return B * A;
}

inline RGBAColor operator/ (const RGBAColor& A, const RGBAColor& B)
{
	return RGBAColor(A.r / B.r, A.g / B.g, A.b / B.b, A.a / B.a);
}

inline RGBAColor operator/ (const RGBAColor& A, float B)
{
	return RGBAColor(A.r / B, A.g / B, A.b / B, A.a / B);
}

inline int operator< (const RGBAColor& A, const RGBAColor& B)
{
	return A.r < B.r && A.g < B.g && A.b < B.b && A.a < B.a;
}

inline int operator== (const RGBAColor& A, const RGBAColor& B)
{
	return (A.r == B.r) && (A.g == B.g) && (A.b == B.b) && (A.a == B.a);
}

inline int operator!= (const RGBAColor& A, const RGBAColor& B)
{
	return !(A == B);
}


#ifdef _D3D

// standard versions
// slow
/*
#define D3DRGB(r, g, b) \
    (0xff000000L | ( ((long)((r) * 255)) << 16) | (((long)((g) * 255)) << 8) | (long)((b) * 255))

#define D3DRGBA(r, g, b, a) \
    (   (((long)((a) * 255)) << 24) | (((long)((r) * 255)) << 16) \
    |   (((long)((g) * 255)) << 8) | (long)((b) * 255) \
    )
*/



inline long D3DRGBA_X(float r,float g,float b, float a)
{
  return float2int(a*255.0f)<<24 | float2int(r*255.0f)<<16 | float2int(g*255.0f)<< 8 | float2int(b*255.0f);
}

inline long D3DRGBA_X(const RGBAColor &color)
{
	return D3DRGBA_X(color.r,color.g,color.b,color.a);
}

//inline long operator D3DCOLOR(RGBAColor &color) { return D3DRGBA_X(color); } 
inline long D3DRGBA_CLAMPED(float r,float g,float b, float a)
{
  return limit(float2int(a*255.0f),0,255)<<24 | limit(float2int(r*255.0f),0,255)<<16 | limit(float2int(g*255.0f),0,255)<< 8 | limit(float2int(b*255.0f),0,255);
}


#endif



//
// GTextureHandle
// @class maintains a handle to a texture
//
class GTextureHandle {
public: 

	//! how many scene nodes are using this texture
    int refCnt;			
    
	//! Double link to form list of all textures in use
	GTextureHandle *next;    
    GTextureHandle *prev;

	GTextureHandle() {  
			fileFormat = 0; 
			hasAlpha = 0; 
			hasColorKey = 0; 
			isGrayscale = 0; 
			rendered = 0;
			refCnt = 0; next = prev = NULL; 
			cannotLoad = 0;  
			inVideoMem = 1; 
			rendered = 0;
			lastRenderedFrame = 0;
			textureFormat = -1;
			components=sizeX=sizeY=bytesPerPix = 0;
	}

	virtual ~GTextureHandle() { Release(); }

	GTextureHandle * Next() { return  next; }

    
    //! release (free)  object
    virtual void Release() { 
		inVideoMem = TRUE;
	}

	//! is the surface lost (D3D specific) ?
	gbool CheckSurfaceLost() 
	{
        return(FALSE);
	}

	//! is the surface in Video Memory (D3D specific) ?
	inline gbool CheckInVideoMem()  { return inVideoMem; }

  
	CString				    fileName;			// file name
	CString				    fileUrl;			// file name url, in order to early lookup textures

	int						fileFormat;			/* type of file */	
	
	BEGIN_PACKED_STRUCT
	
	BOOL					cannotLoad:1;		/* no success loading the file  */
	BOOL					hasAlpha:1;			/* has color key or alpha channel */
	BOOL					hasColorKey:1;		/* has color key or alpha channel */
	BOOL					isGrayscale:1;		/* is grayscale image */
	BOOL					inVideoMem:1;       /* texture is in video memory, false it  doesn't fit  */
	BOOL					rendered:1;			/* texture was rendered */ 
	unsigned char			components;			// number of components 
	unsigned char			bytesPerPix;
	short					sizeX,sizeY;		// current size 
	short					textureFormat;		// index into texture format // D3D
	
	END_PACKED_STRUCT


	DWORD					lastRenderedFrame;	// frame counter of frame this texture has been used

	// texture can't be loaded due to some problem ?
	inline gbool CantLoad() { return (cannotLoad); }	

	// alpha component ?
	inline gbool HasAlpha() { return (hasAlpha); }	

	// color key ?
	inline gbool HasColorKey() { return (hasColorKey); }	
    
	// is texture a grayscale image ?
	inline gbool IsGrayscale() { return (isGrayscale); }

	inline gbool Rendered() { return (rendered); }

	gbool MatchFileName(LPCTSTR name) const { return fileName.Compare(name) == 0; }
	gbool MatchFileUrl(LPCTSTR name) const { return fileUrl.Compare(name) == 0; }


    //! Is definition of object valid ???
    virtual gbool Ok() = 0;

    //! increment reference count
    void Ref() { refCnt++; }

    //! decrement reference count, release if going to 0
    void UnRef() { refCnt--;  if (refCnt == 0) Release(); }

	//! set texture by texture filename 
	gbool Set(GRenderStateBase &state, LPCTSTR fileName,int format=0, LPCTSTR fileUrl=NULL);

	// !set texture from image data
	gbool Set(GRenderStateBase &state, int format,int width,int height,int numComponents,unsigned char *data, char *colorTable=NULL);

	// set texture surface from image data
	// gbool SetTexture(RenderState &state,
	//		int format,int width,int height,
	//		unsigned char *data, char *colorTable=NULL);

	// set by image 
	gbool Set(GRenderStateBase &state,image *im,gbool checkScale=TRUE,gbool checkFormat=TRUE,gbool filteredScale=TRUE);

#ifdef _D3D
	// set by DDraw Surface
	gbool Set(GRenderStateBase &state,LPDIRECTDRAWSURFACE srcSurface,IDirectDrawPalette *srcPalette,DDSURFACEDESC &srcDdsd, RECT &srcRect,gbool checkScale=TRUE,gbool checkFormat=TRUE,gbool filteredScale=TRUE);
#endif


	// helper for optimizer 
	// return image 
	// gbool GetImage(image &im);

};

//
// GTextureRef
//
// holds a pointer to a GTextureHandleBase

class GTextureRef
{

/* protected: */
public : 

	// ! pointer to a GTextureHandleBase or derived structure, reference counted !
    GTextureHandle *handle;

public:

    GTextureRef() : handle(NULL) { };

    //! is texture handle valid or need it  be to redefined ?
    gbool Ok() { return (handle && handle->Ok()); }

    //! is texture handle valid or need it  be to redefined ?
    gbool CantLoad() { return (handle && handle->CantLoad()); }

    //! has texture an alpha channel or color key 
	gbool HasAlpha() { return (handle && handle->HasAlpha()); }

    //! has texture a  color key 
	gbool HasColorKey() { return (handle && handle->HasColorKey()); }

    //! is texture a grayscale image 
	gbool IsGrayscale() { return (handle && handle->IsGrayscale()); }

    //! texture no longer needed, release and free up if no more references
    void Release() {
        if (handle) handle->UnRef();
        handle = NULL;
    }

    //! Notify handle (owned by renderstate) that this ref doesn´t need the handle any more
    ~GTextureRef() {
        if (handle) handle->UnRef();
        handle = NULL;
    }

    friend class GRenderState;
};


typedef enum {
	GR_UNKNKOWN_DEVICE,
	GR_NULL_DEVICE,
	GR_OGL_DEVICE,
	GR_D3D_RAMP_DEVICE,
	GR_D3D_DEVICE,
	GR_D3D7_DEVICE

} G_RENDER_DEVICE;



class GRenderDevice 
{
public :

	int type;

	gbool Init() {return gfalse; }
	void Term() {}

};



class GRenderState
{
public:

	GRenderState() {

		textureMatrixIdentity=gtrue;		  
		textureMatrixVersion=0;
		textureMatrixVersionMax=0;
		
		doTextureDownscale = gtrue;

	}	


	//  ********** Texture  support **********


	//! define texture by filename
	virtual gbool SetTexture(GTextureRef &ref,
								LPCTSTR fileName,
								LPCTSTR fileUrl,
								int format = 0,gbool reload=gfalse)=0; 
	
	//! check if a texture with this URL already loaded  and initialize ref
	virtual gbool FindTextureByUrl(GTextureRef &ref,LPCTSTR fileUrl)=0; 

	//! set texture by image data  (VRML SFImage)
	virtual gbool SetTexture(GTextureRef &ref,int glFormat,int width,int height,int numComponents,unsigned char *data)=0;

	//! set texture by image structure 
	virtual gbool SetTexture(GTextureRef &ref,image *im,gbool checkScale=TRUE)=0;


    //! Set Texture, if result FALSE, need definition of Texture with one of the above functions
    virtual gbool SetTexture(GTextureRef &ref) = 0;

	
	// ********** texture matrix support **********
	/*
		Direct 3D has no texture matrix,
		so texture coordinates need to contain effects of the matrix
		for speed reasons lowe level must detect texture matrix changes via the textureMatrixVersion
	*/
	
	//! current texture matrix is identity	
	gbool textureMatrixIdentity;		  
    
	//! the current texture matrix in effect
	unsigned long  textureMatrixVersion; 
	
	//! max texture matrix version / can wrap 
	unsigned long  textureMatrixVersionMax;


	//! set texture matrix, version Id is incremented
	virtual void SetTextureMatrix(const Matrix &m) = 0;

	// set texture matrix, together with version ID, ID = 0, matrix is identity
	virtual void SetTextureMatrix(const Matrix &m, unsigned long version) = 0;

	//! set texture matrix to identity, together with version ID, ID = 0, matrix is identity
	virtual void SetTextureMatrixIdentity() = 0;

 	//! get texture matrix version id, ID = 0, matrix is identity
	unsigned long GetTextureMatrixVersion() { if (textureMatrixIdentity) return(0); else return textureMatrixVersion; }

	//! generate new texture matrix version id
	unsigned long GetNewTextureMatrixVersion() {  
		 textureMatrixVersionMax++; 
		 if (textureMatrixVersionMax == 0) textureMatrixVersionMax=1;  
		 return textureMatrixVersionMax; 
	}

	//! check if current texture matrix ID  matches version
	gbool TextureMatrixVersionOk(unsigned long version) { return(version == GetTextureMatrixVersion()); }


	// ********** texture matrix support **********

	//! downscale textures if they do no fit in memory
	gbool doTextureDownscale;

    //! create a new Texture handle with a ref cnt of 1 
    //virtual GTextureHandle* CreateTexture() = 0;

    //! delete an no longer used Texture
    //virtual void DeleteTexture(GTextureHandle *m)= 0;


	//! compute target texture format size 
	virtual void GetTextureTargetSize(int &width,int &height,int roundUpLimit=128) = 0;
    

    //! release all Texture handles
    virtual void ReleaseTextures() = 0;

    //! release textures not rendered in the last frame
    virtual gbool ReleaseUnrenderedTextures(int max=1) = 0;

    //! release large textures 
    virtual gbool ReleaseLargeTextures(int max=1) = 0;

	//! compute the size of the largest textures 
	virtual gbool GetMaxUsedTextureSize(int &sizeX,int &sizeY, int &bytesPerPix) = 0;

    //! Delete all Texture handles
    //! should only be called if all references deleted !!!!!!!!!!!!1
    virtual void DeleteTextures() = 0;

    //! Delete all unused D3D Texture handles
    virtual gbool DeleteUnusedTextures() = 0;

	//! texture memory overflow 
	//! if result TRUE, try to reload texture in progress
	virtual gbool OnTextureMemoryOverflow(GTextureHandle &overflowHandle) = 0;


	//! get a 2D renderer to current state
	virtual GRenderer2D* getRenderer2D() = 0;


};

enum GRender2DMode 
{
	G_EDGE = 1,
	G_FILL = 2,
};



// abstract interface

class GRenderer2D
{
public :

	//!render 2D polyline,
	virtual void DrawPolyline(int np, const Point *pts) = 0;

	//!render 2D polygon
	virtual void DrawPolygon(int np, const Point *pts) = 0;

	//!render 2D points
	virtual void DrawPoint(int np, const Point *pts) = 0;

	//!render 2D grid 
	virtual void DrawGrid(const Point &pmin,const Point &pmax,int xsteps,int ysteps,gbool lines=gtrue) = 0;


	//!begin a new contour within the current polygon 
	virtual void BeginContour()= 0;
	//!end the current contour within the current polygon 
	virtual void EndContour()= 0;

	//!begin a new polygon 
	virtual void BeginPolygon()= 0;

	//!end the current open polygon 
	virtual void EndPolygon()= 0;

	// attributes
	

	virtual void SetEdgeColor(const RGBAColor &color) = 0;
	virtual void SetColor(const RGBAColor &color) = 0;

	virtual void SetDrawStyle(int style) = 0;

	//! set the point size 
	virtual void SetPointSize(float radius) = 0;

	
};


#endif _GRenderBase_


