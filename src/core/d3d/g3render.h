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
#ifndef _G3Render_
#define _G3Render_





/******************************************************************************
@doc

@module G3Render.h - GLView RenderState classes for Direct 3D |

Copyright (c) 1996-1998	by Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:
	global Macro define _D3D for Direct 3D Support 

Changes:

$Revision: $
$Log: $

	28.03.92 added STARTFLAT Macro to PUT_POLYGON
	02.10.98 Texture & Material handle changes in DX5 done with state buffer, not device2.setRenderState

Todo :

******************************************************************************/



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
#define G3RIGHT_HANDED

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
#define glMaterial G3Material


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

// standard version
// slow
/*
#define D3DRGB(r, g, b) \
    (0xff000000L | ( ((long)((r) * 255)) << 16) | (((long)((g) * 255)) << 8) | (long)((b) * 255))
#define D3DRGBA(r, g, b, a) \
    (   (((long)((a) * 255)) << 24) | (((long)((r) * 255)) << 16) \
    |   (((long)((g) * 255)) << 8) | (long)((b) * 255) \
    )
*/




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
class RenderState;





// Last D3D error code 
// extern HRESULT GLastError;

class G3ExBufPtr {
public:
	LPVOID lpPointer; // write position in current buffer
	LPVOID lpBuffer;  // buffer start 
	size_t size;
	DWORD offset; // instruction start / vertices before
	
	// return lpPointer 
	operator LPVOID &() {return (lpPointer); }	
	
	// return actual written bytes to exbuf
	ULONG Length() const { return (ULONG) ((char *)lpPointer - (char*)lpBuffer); }


	// return allocated size 
	size_t MaxLength() const { return(size); }
	size_t Size() const { return size; }
	
	G3ExBufPtr(size_t Size=0,LPVOID buffer = NULL) {
				size = Size; lpBuffer = lpPointer = buffer;
				offset = 0;
			}

	void SetSize(size_t Size) { size = Size; }
	
	void PUT_OP(BYTE op,BYTE sz, WORD cnt=1) {
		    PUTD3DINSTRUCTION(op, sz, cnt, lpPointer);
	}
	static size_t OP_SIZE()  { return sizeof(D3DINSTRUCTION); }
	
	// store vertices // only at begin of buffer !!!
	void PUT_VERTEX(int nVert, const D3DVERTEX *vert) { VERTEX_DATA((const void *) vert,nVert,lpPointer); offset = Length(); }
	static size_t VERTEX_SIZE(int nVertex)  { return nVertex * sizeof(D3DVERTEX); }

	void PUT_OP_EXIT() { OP_EXIT(lpPointer); }
	void PUT_ALIGN() {
	    if (QWORD_ALIGNED(lpPointer)) {
			OP_NOP(lpPointer);
		}
	}

	// render points from start to nPoints
	void PUT_OP_POINT_LIST(int start, int nPoints) { OP_POINT_LIST(start, nPoints,lpPointer); }


	void PUT_OP_LINE_LIST(int nLines) { OP_LINE_LIST(nLines,lpPointer); }
	// must be followed by nLines PUT_LINE

	// size req for OP_TRIANGLE_LIST with n Triangles
	size_t LINE_LIST_SIZE(int nLines) {  return sizeof(D3DINSTRUCTION) + nLines * sizeof(D3DLINE); }

    // actual index type is WORD
	void PUT_LINE(int p1,int p2)
	{
			LPD3DLINE t = (LPD3DLINE) lpPointer;

			//TRACE("IN %d %d  %d  \n",p1,p2);
			t->v1 = p1; 
			t->v2 = p2;

			t++;
			lpPointer = (LPVOID) t;

	}


	void PUT_OP_TRIANGLE_LIST(int nTriangles) { OP_TRIANGLE_LIST(nTriangles,lpPointer); }
	// must be followed by nTriangles PUT_TRIANGLE

	// size req for OP_TRIANGLE_LIST with n Triangles
	size_t TRIANGLE_LIST_SIZE(int nTriangles) {  return sizeof(D3DINSTRUCTION) + nTriangles * sizeof(D3DTRIANGLE); }

    // actual index type is WORD
	void PUT_TRIANGLE(int p1,int p2, int p3,gbool flip= FALSE)
	{
			LPD3DTRIANGLE t = (LPD3DTRIANGLE) lpPointer;
            t->wFlags = D3DTRIFLAG_START | D3DTRIFLAG_EDGEENABLETRIANGLE;

			//TRACE("IN %d %d  %d  \n",p1,p2,p3);
			t->v1 = p1; 
			if (flip) 
			{ t->v2 = p3;	t->v3 = p2; }
			else { t->v2 = p2;	t->v3 = p3; }

			t++;
			lpPointer = (LPVOID) t;
			
	}

	// clear the prev triangle buffer 
	void SETUP_PREV(D3DTRIANGLE &prev) 
	{
		prev.v1=prev.v2=prev.v3 =(WORD) -1;
		prev.wFlags = 0;
	}

	// try to build strips with prev triangle
	void PUT_TRIANGLE(int p1,int p2, int p3,gbool flip,D3DTRIANGLE &prev)
	{
			LPD3DTRIANGLE t = (LPD3DTRIANGLE) lpPointer;

			//TRACE("IN %d %d  %d  \n",p1,p2,p3);
			t->v1 = p1; 
			if (flip) 
			{ 
				//t->v2 = p3;	t->v3 = p2; 
				t->v1 = p3; t->v2 = p2;	t->v3 = p1; // new test  
			}
			else { t->v2 = p2;	t->v3 = p3; }

			if (t->v1 == prev.v1 && t->v2 == prev.v3)
	            t->wFlags = D3DTRIFLAG_EVEN | D3DTRIFLAG_EDGEENABLETRIANGLE;
			else if (t->v1 == prev.v3 && t->v2 == prev.v2)
	            t->wFlags = D3DTRIFLAG_ODD | D3DTRIFLAG_EDGEENABLETRIANGLE;
            else t->wFlags = D3DTRIFLAG_START | D3DTRIFLAG_EDGEENABLETRIANGLE;
			
			prev = *t; // save this as current triangle 

			t++;
			lpPointer = (LPVOID) t;
			
	}

    // with flags 
	void PUT_TRIANGLE_FLAG(int p1,int p2, int p3,WORD flags /* =D3DTRIFLAG_START | D3DTRIFLAG_EDGEENABLETRIANGLE */)
	{
			LPD3DTRIANGLE t = (LPD3DTRIANGLE) lpPointer;
            t->wFlags = flags;
			//TRACE("IN %d %d  %d  \n",p1,p2,p3);
			t->v1 = p1;
			t->v2 = p2;
			t->v3 = p3;
			t++;
			lpPointer = (LPVOID) t;
			
	}

	void PUT_QUAD(int p1,int p2, int p3, int p4,gbool flip= FALSE)
	{

			if (flip) {
				PUT_QUAD(p1,p4,p3,p2,FALSE);
				return;
			}
			LPD3DTRIANGLE t = (LPD3DTRIANGLE) lpPointer;

            //only need two edges for wireframe, cull next triangle too
            t->wFlags =   D3DTRIFLAG_START | D3DTRIFLAG_STARTFLAT(1)
						| D3DTRIFLAG_EDGEENABLE1 | D3DTRIFLAG_EDGEENABLE2;
        
			//TRACE("IN %d %d  %d  \n",p1,p2,p3);
			t->v1 = p1;
			t->v2 = p2;
			t->v3 = p3;
			t++;
            t->wFlags =		D3DTRIFLAG_EVEN | 
						D3DTRIFLAG_EDGEENABLE2 | D3DTRIFLAG_EDGEENABLE3;
			t->v1 = p1;
			t->v2 = p3;
			t->v3 = p4;
			t++;
			lpPointer = (LPVOID) t;
	}

	// PUT a quad part of a quadrilateral mesh, optimizable as tristrip
	void PUT_QUAD_STRIP(gbool start,int p1,int p2, int p3, int p4,gbool flip= FALSE,gbool bottomEdge = TRUE)
	{

			if (flip) {
				PUT_QUAD_STRIP(start,p1,p4,p3,p2,FALSE,bottomEdge);
				return;
			}
			LPD3DTRIANGLE t = (LPD3DTRIANGLE) lpPointer;

            //only need two edges for wireframe, cull next triangle too
			if (start) 
            t->wFlags =    D3DTRIFLAG_START // FLAT(1)
						| D3DTRIFLAG_EDGEENABLE1 | D3DTRIFLAG_EDGEENABLE2;
			else 
            t->wFlags =    D3DTRIFLAG_ODD
						 | D3DTRIFLAG_EDGEENABLE2;
        
			//TRACE("IN %d %d  %d  \n",p1,p2,p3);
			t->v1 = p1;
			t->v2 = p2;
			t->v3 = p3;
			t++;
            t->wFlags =	D3DTRIFLAG_EVEN | D3DTRIFLAG_EDGEENABLE2;

			if (bottomEdge) t->wFlags |= D3DTRIFLAG_EDGEENABLE3;

			t->v1 = p1;
			t->v2 = p3;
			t->v3 = p4;
			t++;
			lpPointer = (LPVOID) t;
	}



	// puts n-2 TRIANGLES
	void PUT_POLYGON(int n, const int* fp,gbool flip= FALSE)
	{
			LPD3DTRIANGLE t = (LPD3DTRIANGLE) lpPointer;

            WORD flags = D3DTRIFLAG_START | D3DTRIFLAG_EDGEENABLE1 | D3DTRIFLAG_EDGEENABLE2;


			if (n<32)
			  flags |= D3DTRIFLAG_STARTFLAT(n-3);
			else 
			  flags |= D3DTRIFLAG_STARTFLAT(32-3); // could optimize in between

			// start flat n
			if (flip) {
			fp+=n-1;
			int p1=*fp--, p2=*fp--;
			n-=2;
			while (n>0) {
				int p3 = *fp--;
				n--;
			    if (n==0) flags |= D3DTRIFLAG_EDGEENABLE3;
				//TRACE("IN %d %d  %d  \n",p1,p2,p3);
				t->wFlags = flags;
				t->v1 = p1; t->v2 = p2; t->v3 = p3;
				p2=p3;
				t++;
				flags = D3DTRIFLAG_EVEN |  D3DTRIFLAG_EDGEENABLE2 ;
			}
			}
			else  {  // no flip
			int p1=*fp++, p2=*fp++;
			n-=2;
			while (n>0) {
				int p3 = *fp++;
				n--;
			    if (n==0) flags |= D3DTRIFLAG_EDGEENABLE3;
				//TRACE("IN %d %d  %d  \n",p1,p2,p3);
				t->wFlags = flags;
				t->v1 = p1; t->v2 = p2; t->v3 = p3;
				p2=p3;
				t++;
	           
				flags =  D3DTRIFLAG_EVEN  | D3DTRIFLAG_EDGEENABLE2 ;

			}
			} // no flip

			lpPointer = (LPVOID) t;
	}

	// puts n-2 TRIANGLES stripped 
	void PUT_POLYGON(int n, const int* fp,gbool flip,D3DTRIANGLE &prev)
	{
			LPD3DTRIANGLE t = (LPD3DTRIANGLE) lpPointer;

            WORD flags = D3DTRIFLAG_START | D3DTRIFLAG_EDGEENABLE1 | D3DTRIFLAG_EDGEENABLE2;


			if (n<32)
			  flags |= D3DTRIFLAG_STARTFLAT(n-3);
			else 
			  flags |= D3DTRIFLAG_STARTFLAT(32-3); // could optimize in between

			// start flat n
			if (flip) {
				fp+=n-1;
				int p1=*fp--, p2=*fp--;
				n-=2;
				
				if (p1 == prev.v1 && p2 == prev.v3)
					flags |= D3DTRIFLAG_EVEN ;
				else if (p1 == prev.v3 && p2 == prev.v2)
					flags |= D3DTRIFLAG_ODD ;

			while (n>0) {
				int p3 = *fp--;
				n--;
			    if (n==0) flags |= D3DTRIFLAG_EDGEENABLE3;
				//TRACE("IN %d %d  %d  \n",p1,p2,p3);
				t->wFlags = flags;
				t->v1 = p1; t->v2 = p2; t->v3 = p3;
				p2=p3;
				t++;
				flags = D3DTRIFLAG_EVEN |  D3DTRIFLAG_EDGEENABLE2 ;
			}
			}
			else  {  // no flip
				int p1=*fp++, p2=*fp++;
				n-=2;

				if (p1 == prev.v1 && p2 == prev.v3)
					flags |= D3DTRIFLAG_EVEN ;
				else if (p1 == prev.v3 && p2 == prev.v2)
					flags |= D3DTRIFLAG_ODD ;

			while (n>0) {
				int p3 = *fp++;
				n--;
			    if (n==0) flags |= D3DTRIFLAG_EDGEENABLE3;
				//TRACE("IN %d %d  %d  \n",p1,p2,p3);
				t->wFlags = flags;
				t->v1 = p1; t->v2 = p2; t->v3 = p3;
				p2=p3;
				t++;
	           
				flags =  D3DTRIFLAG_EVEN  | D3DTRIFLAG_EDGEENABLE2 ;

			}
			} // no flip
			
			prev = t[-1];

			lpPointer = (LPVOID) t;
	}


	// puts n-1 LINES
	void PUT_POLYLINE(int n, const int* fp,gbool flip= FALSE)
	{
			LPD3DLINE t = (LPD3DLINE) lpPointer;

			if (flip) {
			fp+=n-1;
			int p1=*fp--;
			n-=1;
			while (n>0) {
				int p2 = *fp--;
				n--;
				//TRACE("IN %d %d  %d  \n",p1,p2);
				t->v1 = p1; t->v2 = p2; 
				p1=p2;
				t++;
			}
			}
			else  {  // no flip
			int p1=*fp++;
			n-=1;
			while (n>0) {
				int p2 = *fp++;
				n--;
				//TRACE("IN %d %d  %d  \n",p1,p2,p3);
				t->v1 = p1; t->v2 = p2;
				p1=p2;
				t++;
			}
			} // no flip

			lpPointer = (LPVOID) t;
	}


	gbool Set(int nV, D3DVERTEX *v,int nTri, D3DTRIANGLE *tri);

    // set texture wrapping mode
	void put_texture_wrap(gbool wrap_s,gbool wrap_t) {
		OP_STATE_RENDER(2, lpPointer);
            STATE_DATA(D3DRENDERSTATE_WRAPU, wrap_s, lpPointer);
            STATE_DATA(D3DRENDERSTATE_WRAPV, wrap_t, lpPointer);
	}
	
    // set texture handle 
    void put_texture_handle(D3DTEXTUREHANDLE hTex) {
		OP_STATE_RENDER(1, lpPointer);
            STATE_DATA(D3DRENDERSTATE_TEXTUREHANDLE, hTex, lpPointer);
	}

};


/*
  G3ExBuf

  G3ExBuf b;
  b.Create(lpD3DDevice, mysize);
  b.Lock()
     // write data
  b.Unlock();
  b.Set();
  b.Execute();

*/

class G3ExBuf : public G3ExBufPtr {
public:
    LPDIRECT3DEXECUTEBUFFER lpD3DExBuf;  

	// only set size 
	G3ExBuf(size_t Size=0) : G3ExBufPtr(Size) { lpD3DExBuf = NULL; }
	
	// set size and create 
	G3ExBuf(LPDIRECT3DDEVICE lpD3DDevice, size_t Size) : G3ExBufPtr(Size) 
	{	lpD3DExBuf = NULL; 
		Create(lpD3DDevice); 
	}
	~G3ExBuf() { Release(); }

    gbool Ok() { return(lpD3DExBuf != NULL); } 

    // create the buffer using the current size 
	gbool Create (LPDIRECT3DDEVICE lpD3DDevice) {
        Release();
		
		HRESULT GLastError;

        D3DEXECUTEBUFFERDESC debDesc;
		memset(&debDesc, 0, sizeof(D3DEXECUTEBUFFERDESC));
		debDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
		debDesc.dwFlags = D3DDEB_BUFSIZE;
		// caps : video / system
		debDesc.dwBufferSize = size;
	  	
		GLastError = lpD3DDevice->CreateExecuteBuffer(&debDesc, &lpD3DExBuf, NULL);
        if (GLastError != D3D_OK)  D3DError("CreateExecuteBuffer failed",GLastError);

        return (GLastError == D3D_OK);
	}

    // create the buffer using the  new size 

	gbool Create (LPDIRECT3DDEVICE lpD3DDevice, size_t newSize) {
        Release();
		SetSize(newSize);
		return Create(lpD3DDevice);
	}

	// lock the buffer, to enable writing
	gbool Lock() {
		HRESULT GLastError;
		D3DEXECUTEBUFFERDESC debDesc;
		memset(&debDesc, 0, sizeof(D3DEXECUTEBUFFERDESC));
		debDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
		GLastError = lpD3DExBuf->Lock(&debDesc);
        if (GLastError == D3D_OK) {
		    lpPointer = lpBuffer = debDesc.lpData;
            size = debDesc.dwBufferSize;
        } else {
          D3DError("LockExecuteBuffer failed",GLastError);
        }
        return (GLastError == D3D_OK);
	}

	// unlock buffer 
	gbool Unlock() {
		HRESULT GLastError;
		GLastError = lpD3DExBuf->Unlock();
        if (GLastError != D3D_OK)  D3DError("UnlockExecuteBuffer failed",GLastError);
        return (GLastError == D3D_OK);
	}
	

	// set the execute buffer data
    // length is current lpPointer-
    // execute the buffer  release buffer (After Create + Lock/Unlock)

	gbool Set() {
		HRESULT GLastError;
		D3DEXECUTEDATA d3dExData;
		// memset(&d3dExData, 0, sizeof(D3DEXECUTEDATA));
		d3dExData.dwSize = sizeof(D3DEXECUTEDATA);
		d3dExData.dwVertexOffset = 0;
		d3dExData.dwVertexCount = offset / sizeof(D3DVERTEX);
		d3dExData.dwInstructionOffset = offset;
		d3dExData.dwInstructionLength = Length() - offset;
		d3dExData.dwHVertexOffset=
		d3dExData.dsStatus.dwFlags=d3dExData.dsStatus.dwStatus =0; // new 

		GLastError = lpD3DExBuf->SetExecuteData(&d3dExData);
        if (GLastError != D3D_OK)  D3DError("SetExecuteData failed",GLastError);
        return (GLastError == D3D_OK);
	}

    //! execute the buffer  release buffer (After Create + Lock/Unlock)
	gbool Execute(LPDIRECT3DDEVICE lpDev, LPDIRECT3DVIEWPORT lpView, DWORD mode = D3DEXECUTE_UNCLIPPED)
	{
		HRESULT GLastError;
		GLastError = lpDev->Execute(lpD3DExBuf, lpView, mode);
        if (GLastError != D3D_OK)  D3DError("ExecuteData failed",GLastError);
        return (GLastError == D3D_OK);
	}
	
    //! release buffer (After Unlock)
	void Release() 
	{
		HRESULT GLastError;
		if (lpD3DExBuf)
		   GLastError = lpD3DExBuf->Release();
		lpPointer = lpBuffer = NULL;
		lpD3DExBuf = NULL;
	}


    // clear buffer to zero (After Lock)
    void ClearAndZero() {
		memset(lpBuffer, 0, size);
		lpPointer = lpBuffer;
	}

    void Clear() {
		lpPointer = lpBuffer;
	}

};

class G3ExBufDesc : public G3ExBuf {
public:

    D3DEXECUTEBUFFERDESC debDesc;

	// only set size 
	G3ExBufDesc(size_t Size=0) : G3ExBuf(Size) {  
		memset(&debDesc, 0, sizeof(D3DEXECUTEBUFFERDESC));
		debDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);

	}
	
	// set size and create 
	G3ExBufDesc(LPDIRECT3DDEVICE lpD3DDevice, size_t Size) : G3ExBuf(Size) 
	{	 
		memset(&debDesc, 0, sizeof(D3DEXECUTEBUFFERDESC));
		debDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
		Create(lpD3DDevice); 
	}
	~G3ExBufDesc() { Release(); }

    gbool Ok() { return(lpD3DExBuf != NULL); } 

    // create the buffer using the current size 
	gbool Create(LPDIRECT3DDEVICE lpD3DDevice) {

		HRESULT GLastError;
        Release();

		debDesc.dwFlags = D3DDEB_BUFSIZE;
		// caps : video / system
		debDesc.dwBufferSize = size;
	  	GLastError = lpD3DDevice->CreateExecuteBuffer(&debDesc, &lpD3DExBuf, NULL);
        if (GLastError != D3D_OK)  D3DError("CreateExecuteBuffer failed",GLastError);

        return (GLastError == D3D_OK);
	}

    // create the buffer using the  new size 

	gbool Create (LPDIRECT3DDEVICE lpD3DDevice, size_t newSize) {
        Release();
		SetSize(newSize);
		return Create(lpD3DDevice);
	}

	// lock the buffer, to enable writing
	gbool Lock() {
		HRESULT GLastError;
		GLastError = lpD3DExBuf->Lock(&debDesc);
        if (GLastError == D3D_OK) {
		    lpPointer = lpBuffer = debDesc.lpData;
            size = debDesc.dwBufferSize;
        } else {
          D3DError("LockExecuteBuffer failed",GLastError);
        }
        return (GLastError == D3D_OK);
	}

	// unlock buffer 
	gbool Unlock() {
		HRESULT GLastError;
		GLastError = lpD3DExBuf->Unlock();
        if (GLastError != D3D_OK)  D3DError("UnlockExecuteBuffer failed",GLastError);
        return (GLastError == D3D_OK);
	}
	

	// set the execute buffer data
    // length is current lpPointer-
    // execute the buffer  release buffer (After Create + Lock/Unlock)

	gbool Set() {
		HRESULT GLastError;

		D3DEXECUTEDATA d3dExData;
		// memset(&d3dExData, 0, sizeof(D3DEXECUTEDATA));
		d3dExData.dwSize = sizeof(D3DEXECUTEDATA);
		d3dExData.dwVertexOffset = 0;
		d3dExData.dwVertexCount = offset / sizeof(D3DVERTEX);
		d3dExData.dwInstructionOffset = offset;
		d3dExData.dwInstructionLength = Length() - offset;
		d3dExData.dwHVertexOffset=0;
		GLastError = lpD3DExBuf->SetExecuteData(&d3dExData);
        if (GLastError != D3D_OK)  D3DError("SetExecuteData failed",GLastError);
        return (GLastError == D3D_OK);
	}

    // execute the buffer  release buffer (After Create + Lock/Unlock)
	gbool Execute(LPDIRECT3DDEVICE lpDev, LPDIRECT3DVIEWPORT lpView, DWORD mode = D3DEXECUTE_UNCLIPPED)
	{
		HRESULT GLastError;
		GLastError = lpDev->Execute(lpD3DExBuf, lpView, mode);
        if (GLastError != D3D_OK)  D3DError("ExecuteData failed",GLastError);
        return (GLastError == D3D_OK);
	}
	
    // release buffer (After Unlock)
	void Release() 
	{
		HRESULT GLastError;
		if (lpD3DExBuf)
		   GLastError = lpD3DExBuf->Release();
		lpPointer = lpBuffer = NULL;
		lpD3DExBuf = NULL;
	}


    // clear buffer to zero (After Lock)
    void ClearAndZero() {
		memset(lpBuffer, 0, size);
		lpPointer = lpBuffer;
	}

    void Clear() {
		lpPointer = lpBuffer;
	}

};

//------------------------//------------------------//------------------------
//								G3VBBufHandle
//------------------------//------------------------//------------------------ 
//
//
// @class maintains a device-independend handle to a device dependend G3VBBuf
// 
// 
class G3VBBufHandle 
{
public: 

    int refCnt;				// how many scene nodes are using this buffer
    G3VBBufHandle *next;    // Double link to from list of all materials in use
    G3VBBufHandle *prev;

	// only set size 
//	G3VBBufHandle(size_t Size=0) : G3VBBuf(Size) { refCnt = 0; next = prev = NULL;}
	
	// set size and create 
	G3VBBufHandle() 
	{	
		refCnt = 0; next = prev = NULL; lpD3DVBBuf= NULL;
	}
	
	~G3VBBufHandle() 
	{ 
		HRESULT GLastError;
		if (lpD3DVBBuf)
		   GLastError = lpD3DVBBuf->Release();
	}

	gbool Ok() {return lpD3DVBBuf!=NULL;};

	// release buffer (After Unlock)
	void ReleaseBuffer() 
	{
		HRESULT GLastError;
		TRACE("delete VB\n");
		if (lpD3DVBBuf)
		   GLastError = lpD3DVBBuf->Release();
		lpD3DVBBuf = NULL;
	}

    void AddRef() { refCnt++; }

	// release refCnt, can"t delte here because of the linked list 
    ULONG Release() { refCnt--;  if (refCnt == 0) {ReleaseBuffer(); return 0;} return refCnt;}



    LPDIRECT3DVERTEXBUFFER lpD3DVBBuf;
	D3DVERTEXBUFFERDESC vbdesc;//or use HRESULT GetVertexBufferDesc(LPD3DVERTEXBUFFERDESC lpVBDesc); 

};



//------------------------//------------------------//------------------------
//									G3VBBufRef
//------------------------//------------------------//------------------------
/* 
	abstract handle,
	
	  holds a pointer to a G3VBBufHandle
*/

class G3VBBufRef
{

protected:

public:

	//! the pointer to refcountet handle
    G3VBBufHandle *handle;

public:

    G3VBBufRef(G3VBBufHandle *newHandle=NULL) 
	{ 
		handle = newHandle; 
		if (handle) handle->AddRef();
	};
    
	//! is VBBuf handle valid or need to redefine ?
    gbool Ok() { return (handle && handle->Ok()); }
    
	//!< release handle 
    void Release() {
        if (handle) 
			handle->Release();
        handle = NULL;
    }
    //! attach a new handle, release old, ref new 
	void Attach(G3VBBufHandle *newHandle) 
	{ 
        if (handle) 
			handle->Release();
		handle = newHandle; 
		ASSERT(handle);
		handle->AddRef();
	};

	//operator G3VBBufHandle& () { return *handle; }

    //!< Notify handle (owned by renderstate) that this ref doesn´t need the handle any more
    ~G3VBBufRef() {
        if (handle) 
			handle->Release();
    }

    friend class RenderState;
};






//------------------------//------------------------//------------------------
//									G3ExBufHandle
//------------------------//------------------------//------------------------
//
/*!
	maintains a handle to a G3ExBuf
*/

class G3ExBufHandle :public G3ExBuf {
public: 

    int refCnt;				//!< how many scene nodes are using this buffer
    G3ExBufHandle *next;    // Double link to from list of all materials in use
    G3ExBufHandle *prev;

	// only set size 
	G3ExBufHandle(size_t Size=0) : G3ExBuf(Size) { refCnt = 0; next = prev = NULL;}
	
	// set size and create 
	G3ExBufHandle(LPDIRECT3DDEVICE lpD3DDevice, size_t Size) : G3ExBuf(lpD3DDevice,Size) 
	{	
		refCnt = 0; next = prev = NULL;
	}
	
	~G3ExBufHandle() { Release(); }

    void Ref() { refCnt++; }
    void UnRef() { refCnt--;  if (refCnt == 0) Release(); }

};

//
// G3ExBufRef
//
// holds a pointer to a G3ExBufHandle

class G3ExBufRef
{

protected:

public:

    G3ExBufHandle *handle;

public:

    G3ExBufRef() : handle(NULL) { };
    
	// is ExBuf handle valid or need to redefine ?
    gbool Ok() { return (handle && handle->Ok()); }
    
    void Release() {
        if (handle) handle->UnRef();
        handle = NULL;
    }

	operator G3ExBuf& () { return *handle; }

    // execute the buffer  release buffer (After Create + Lock/Unlock)
	gbool Execute(LPDIRECT3DDEVICE lpDev, LPDIRECT3DVIEWPORT lpView, DWORD mode = D3DEXECUTE_UNCLIPPED)
	{ return handle->Execute(lpDev,lpView,mode); }

    // Notify handle (owned by renderstate) that this ref doesn´t need the handle any more
    ~G3ExBufRef() {
        if (handle) handle->UnRef();
        handle = NULL;
    }

    friend class RenderState;
};


#if 0

//@class D3D Red Green Blue Alpha color
class RGBAColor : public D3DCOLORVALUE {
public:

    //@cmember the color components (range 0..1)

	RGBAColor(float R=0.0,float G=0.0, float B=0.0,float A=1.0) { r=R,g=G,b=B,a=A; };
	RGBAColor(const Point &p,float A=1.0) { r=p.x,g=p.y,b=p.z,a=A; };
	RGBAColor(const D3DCOLORVALUE &c) { * ((D3DCOLORVALUE*) this) = c; };

	RGBAColor& Set(float R,float G, float B,float A=1.0) { r=R,g=G,b=B,a=A; return(*this); };

	RGBAColor& Set(const Point &p,float A=1.0) { r=p.x,g=p.y,b=p.z,a=A; return(*this); };
	RGBAColor& Get(Point &p) { p.Set(r,g,b); return(*this); };
	RGBAColor& SetAlpha(float A=1.0) { a=A; return(*this); };

	RGBAColor& White() { r=g=b=1.0; a=1.0; return (*this); };
	RGBAColor& Black() { r=g=b=0.0; a=1.0; return (*this); };
	RGBAColor& Gray(float gray) { r=g=b=gray; a=1.0; return (*this);};

	// convert to packed color format
	operator D3DCOLOR() const { return(D3DRGBA_X(r, g, b,a )); } 

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
int io(D3DCOLORVALUE &this_,AStream &a,int fmt=0);

// set color to a gray value
void SetGray(D3DCOLORVALUE &v, float gray,float alpha=1.0);

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


#endif




		// create a material and assign values 
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
			);

		// create a material and assign values with a single color 
        gbool CreateSolidMaterial(LPDIRECT3D lpD3D,
			LPDIRECT3DDEVICE lpDev, 
			// material parameters
			//float diffuse[3],
            const RGBAColor &color,
			// texture handle
			D3DTEXTUREHANDLE hTex,
			/* out / in  must be released at end */
			LPDIRECT3DMATERIAL &lpmat,   /* Material object */
			/* out */
			D3DMATERIALHANDLE &hMat /* Handle for the material  */
			);





//
// G3Material
//

   

//@class GL-Style Material
class G3Material : public  D3DMATERIAL 
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

    
	G3Material() { dwSize = sizeof(D3DMATERIAL); hTexture = NULL; dwRampSize = 16; }

    RGBAColor& Ambient() { return *((RGBAColor*) &ambient); }
    RGBAColor& Diffuse() { return *((RGBAColor*) &diffuse); }
    RGBAColor& Specular() { return *((RGBAColor*) &specular); }
    RGBAColor& Emissive() { return *((RGBAColor*) &emissive); }
    RGBAColor& Emission() { return *((RGBAColor*) &emissive); }

    G3Material & Get(Point &Ambient, Point &Diffuse, Point &Specular,Point &Emissive,float &Shininess,float &Alpha) 
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
	G3Material & Set(const Point &Ambient, const Point &Diffuse,
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

	G3Material &Set(
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


	G3Material &Set ( // material parameters
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
	G3Material &Set ( // material parameters
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
	G3Material & Default() {
		ambient.r=ambient.g=ambient.b =0.2f;ambient.a = 1.0f;
		diffuse.r=diffuse.g=diffuse.b =0.8f;diffuse.a = 1.0f;
		specular.r=specular.g=specular.b =0.6f;specular.a = 1.0f;
		emissive.r=emissive.g=emissive.b =0.0f;emissive.a = 1.0f;
		power = 3.0;
		doSpecular = gtrue;
		return(*this);
   }

   // set alpha value 
   G3Material & SetAlpha(float a) {
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
class G3MaterialHandle 
{
public: 

/////////////////////////////////

	G3Material material; // new for custom lighting 
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
    G3MaterialHandle *next;    // Double link to from list of all materials in use
    G3MaterialHandle *prev;



	G3MaterialHandle(): lpMat(NULL),hMat(0),hTexture(0),whiteDiffuse(FALSE), defined(FALSE) { refCnt = 0; next = prev = NULL; }
	~G3MaterialHandle() { if (lpMat) lpMat->Release(); }
    
    // release (free) material object
    void Release() { if (lpMat) lpMat->Release(); lpMat = NULL; hMat = 0; hTexture = 0; defined = 0; }

    LPDIRECT3DMATERIAL lpMat;	/* Material object */
	D3DMATERIALHANDLE hMat;		/* Handle for the material  */

    D3DTEXTUREHANDLE  hTexture;   // handle for texture, only for ramp

	gbool	whiteDiffuse;	// is the diffuse color white ? 
	gbool   defined;		// was the material defined ??

    // Is definition valid ???
    gbool Ok() { return (hMat != 0); }
    void Ref() { refCnt++; }
    void UnRef() { refCnt--;  if (refCnt == 0) Release(); }

	
	operator D3DMATERIALHANDLE() const { return hMat; }
	D3DTEXTUREHANDLE GetTextureHandle() const { return hTexture;}


	// set material properties
	gbool Set(LPDIRECT3D lpD3D,
			LPDIRECT3DDEVICE lpDev, 
			// material parameters
			float ambient[3],float diffuse[3],float specular[3],float emissive[3],float alpha,float power,
			// D3D specific
			DWORD rampSize /* = 16 */, 
			D3DTEXTUREHANDLE hTex  /* Handle for the texture on the material.*/
			) { 
                hTexture = hTex;
                return 
				CreateMaterial(lpD3D,lpDev,
					ambient,diffuse,specular,emissive,alpha,power,
					rampSize,hTex,
					lpMat,hMat); 
			}
	

	// set material properties
	gbool Set(LPDIRECT3D lpD3D,
			 LPDIRECT3DDEVICE lpDev, 
			 // material parameters
             D3DMATERIAL &mat) 
          {
			D3DLASTERROR()

            if (!lpMat) 
	             if ( D3DCheck(lpD3D->CreateMaterial(&lpMat, NULL)) != D3D_OK)
                 return FALSE;

			 hTexture = mat.hTexture;

             D3DCheck(lpMat->SetMaterial(&mat));
             D3DCheck(lpMat->GetHandle(lpDev, &hMat));
	         return TRUE;

          }    
	


	// set material to solid single color
	gbool SetSolid(LPDIRECT3D lpD3D,
			LPDIRECT3DDEVICE lpDev, 
			// material parameters
			//float diffuse[3]
            const RGBAColor &color,
			D3DTEXTUREHANDLE hTex = 0
			) { 
                hTexture = hTex;

				return 	CreateSolidMaterial(lpD3D,lpDev, color, hTex, lpMat,hMat); 
			}

    // apply current material handle as background for viewport
	void SetBackground(LPDIRECT3DVIEWPORT lpView) 
	{ 
		D3DLASTERROR()
		D3DCheck(lpView->SetBackground(hMat)); 
	}

	// add to execute buffer
	void put(LPVOID &lpPointer) const {
		OP_STATE_LIGHT(1, lpPointer);
			STATE_DATA(D3DLIGHTSTATE_MATERIAL, hMat, lpPointer);
	}

	// return instruction size
	DWORD size() const { return sizeof(D3DINSTRUCTION) * 1 + sizeof(D3DSTATE) * 1; }
};




//
// G3MaterialRef
//
// holds a pointer to a G3MaterialHandle

class G3MaterialRef
{

/* protected: */
public: 

    G3MaterialHandle *handle;

public:

    G3MaterialRef() : handle(NULL) { };
    // is material handle valide or need to redefine ?

    gbool Ok() { return (handle && handle->Ok()); }
    
    void Release() {
        if (handle) handle->UnRef();
        handle = NULL;
    }

    // Notify handle (owned by renderstate) that this ref doesn´t need the handle any more
    ~G3MaterialRef() {
        if (handle) handle->UnRef();
        handle = NULL;
    }

    friend class RenderState;
};

//
// G3TextureHandle
// @class maintains a handle to a D3D Texture
//
class G3TextureHandle : public GTextureHandle 
{
public: 

    D3DTEXTUREHANDLE        hTexture;            /* handle */
    LPDIRECTDRAWSURFACE     lpTextureSurf;       /* surface */
    LPDIRECT3DTEXTURE       lpTexture;           /* texture obj */


	G3TextureHandle *Next() { return (G3TextureHandle *) next; }

	G3TextureHandle(): lpTexture(NULL),lpTextureSurf(NULL),hTexture(0) {  
	}

	~G3TextureHandle() { Release(); }
    
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
	gbool LoadTextureSurf(GD3Device *device,RenderState &state);  // in G3Texture
	gbool LoadTextureSurf(GD3Device *device,RenderState &state,image &im);
	gbool LoadTextureSurf(GD3Device *device,RenderState &state,
		LPDIRECTDRAWSURFACE srcSurface,IDirectDrawPalette *srcPalette,
		DDSURFACEDESC &srcDdsd, RECT &srcRect,RECT *targetSizeRect=NULL);
	

	gbool ReloadTextureSurf(GD3Device *device,RenderState &state);
	gbool UnloadTextureSurf(GD3Device *device,RenderState &state);


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
	gbool Set(RenderState &state, LPCTSTR fileName,int format=0, LPCTSTR fileUrl=NULL);

	// set texture from image data
	gbool Set(RenderState &state, int format,int width,int height,int numComponents,unsigned char *data, char *colorTable=NULL);

	// set texture surface from image data
	// gbool SetTexture(RenderState &state,
	//		int format,int width,int height,
	//		unsigned char *data, char *colorTable=NULL);

	// set by image 
	gbool Set(RenderState &state,image *im,gbool checkScale=TRUE,gbool checkFormat=TRUE,gbool filteredScale=TRUE);
	gbool Set(RenderState &state,LPDIRECTDRAWSURFACE srcSurface,IDirectDrawPalette *srcPalette,DDSURFACEDESC &srcDdsd, RECT &srcRect,gbool checkScale=TRUE,gbool checkFormat=TRUE,gbool filteredScale=TRUE);


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
// G3TextureHandle
// @class maintains a handle to a D3D Texture // Direct X 6 Version 
//
class G3TextureHandle2 : public GTextureHandle 
{
public: 

    LPDIRECTDRAWSURFACE     lpTextureSurf;       /* surface */
    LPDIRECT3DTEXTURE2      lpTexture;           /* texture obj */

	G3TextureHandle2 *Next() { return (G3TextureHandle2 *) next; }

	G3TextureHandle2(): lpTexture(NULL),lpTextureSurf(NULL) {  
	}

	~G3TextureHandle2() { Release(); }
    
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
	gbool LoadTextureSurf(GD3Device *device,RenderState &state);  // in G3Texture
	gbool LoadTextureSurf(GD3Device *device,RenderState &state,image &im);
	gbool LoadTextureSurf(GD3Device *device,RenderState &state,
		LPDIRECTDRAWSURFACE srcSurface,IDirectDrawPalette *srcPalette,
		DDSURFACEDESC &srcDdsd, RECT &srcRect,RECT *targetSizeRect=NULL);
	

	gbool ReloadTextureSurf(GD3Device *device,RenderState &state);
	gbool UnloadTextureSurf(GD3Device *device,RenderState &state);


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
	gbool Set(RenderState &state, LPCTSTR fileName,int format=0, LPCTSTR fileUrl=NULL);

	// set texture from image data
	gbool Set(RenderState &state, int format,int width,int height,int numComponents,unsigned char *data, char *colorTable=NULL);

	// set texture surface from image data
	// gbool SetTexture(RenderState &state,
	//		int format,int width,int height,
	//		unsigned char *data, char *colorTable=NULL);

	// set by image 
	gbool Set(RenderState &state,image *im,gbool checkScale=TRUE,gbool checkFormat=TRUE,gbool filteredScale=TRUE);
	gbool Set(RenderState &state,LPDIRECTDRAWSURFACE srcSurface,IDirectDrawPalette *srcPalette,DDSURFACEDESC &srcDdsd, RECT &srcRect,gbool checkScale=TRUE,gbool checkFormat=TRUE,gbool filteredScale=TRUE);


	// helper for optimizer 
	// return image 
	gbool GetImage(image &im);

};






// D3D has a limitation of number of verts per buffer
// so for large objects we need to have a set of buffer
class G3ObjRef
{
protected:

public:

    Array<G3ExBufHandle *> exBuf;	// array of execute buffers
    Array<G3MaterialHandle *> mat;	// array of local materials 
	unsigned long textureMatrixVersion;		// id of current textureMatrix applied, 0 identity
	gbool unlit;		// is this an unlit geometry 

public:

    G3ObjRef() { textureMatrixVersion = 0; unlit = FALSE; };
    
	// is ExBuf handle valid or need to redefine ?
    gbool Ok() const { return (exBuf.Length()>0 && exBuf[0]->Ok()); }

	// is Material handle valid or need to redefine ?
    gbool MatOk() const { return (mat.Length()>0 && mat[0]->Ok()); }

	// is this a multibuffer object ? 
    gbool IsMulti() const { return (exBuf.Length()>1); }

	// append one more material
	void Append(G3MaterialHandle * newMat) { mat.Append(newMat); }

	// append one more buffer 
	void Append(G3ExBufHandle * newBuf) { exBuf.Append(newBuf); }

    // release buffers after i 
    void Truncate(int newLength) {
		for (int i=newLength; i< exBuf.Length(); i++) exBuf[i]->UnRef();
		exBuf.SetLength(newLength);
    }


    // release buffers & materials 
    void Release() {
		for (int i=0; i< exBuf.Length(); i++) exBuf[i]->UnRef();
		exBuf.SetLength(0);
		for (int j=0; j< mat.Length(); j++) mat[j]->UnRef();
		mat.SetLength(0);
    }

    // execute the buffer(s) 
	gbool Execute(LPDIRECT3DDEVICE lpDev, LPDIRECT3DVIEWPORT lpView, DWORD mode = D3DEXECUTE_UNCLIPPED)
	{ 
		for (int i=0; i< exBuf.Length(); i++) {
		    if (!exBuf[i]->Execute(lpDev,lpView,mode)) return(FALSE); 
		}
		return(TRUE);
	}

    // Notify handle (owned by renderstate) that this ref doesn´t need the handle any more
    ~G3ObjRef() {
		Release();
    }

    friend class RenderState;
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


class G3LightHandle : public D3DLIGHT {
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
    G3LightHandle *next;    // Double link to form list of all lights in use
    G3LightHandle *prev;

	// for custom lighting
	float		 ambientIntensity;

	RGBAColor	 kAmbient,kDiffuse,kSpecular; // cached multiplies // material * light	

    
	// is this light enabled, ie added to the viewport ?
	gbool enabled;

	/* D3D light object  */
	LPDIRECT3DLIGHT lpD3DLight; 

	/* we keep a pointer to the viewpoint */
	LPDIRECT3DVIEWPORT lpView;

    void Ref() { refCnt++; }
    void UnRef() { refCnt--;  if (refCnt == 0) Release(); }

	// is the light ok ?? 
    gbool Ok() const { return(lpD3DLight != NULL && enabled); } 

    // release (free) light object
	// we can't do the release directly here, because light has to be removed from viewport
    void Release() { if (lpView) Release(lpView); } 
    //void Release() { if (lpD3DLight) lpD3DLight->Release();  lpD3DLight = NULL; }

    // release (free) light  object and remove from view 
    void Release(LPDIRECT3DVIEWPORT theView);
	
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

	G3LightHandle();

	~G3LightHandle();
    
	void SetDirection(const Point direction) { dvDirection = direction; }

	// set type of light 
	void SetType(D3DLIGHTTYPE type) { dltType=type; }
    
	gbool IsDirectional() const { return dltType == D3DLIGHT_DIRECTIONAL;}
    gbool IsSpot() const { return dltType == D3DLIGHT_SPOT;}
    gbool IsPoint() const { return dltType == D3DLIGHT_POINT;}
};

//
// G3LightRef
//
// holds a pointer to a G3Light

class G3LightRef
{

//protected:
public:

    G3LightHandle *handle;

public:

    G3LightRef() : handle(NULL) { };

    // is light handle valid or need to be redefined ?
    gbool Ok() { return (handle && handle->Ok()); }
    
    void Release() {
        if (handle) handle->UnRef();
        handle = NULL;
    }

    // Notify handle (owned by renderstate) that this ref doesn´t need the handle any more
    ~G3LightRef() {
        if (handle) handle->UnRef();
        handle = NULL;
    }

    friend class RenderState;
};



//
// G3Matrix
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
class G3Matrix {
public: 

	G3Matrix(): hMat(NULL) {}
	void Release() { hMat = NULL; }
	~G3Matrix() { Release(); }

	D3DMATRIXHANDLE hMat; // handle to matrix 


	operator D3DMATRIXHANDLE() const { return hMat; }

	//! set matrix 
	gbool Set(LPDIRECT3DDEVICE lpDev,D3DMATRIX &data) 
	{ 

		D3DLASTERROR()

		if (!hMat)
		if (D3DCheck(lpDev->CreateMatrix(&hMat)) != D3D_OK) 
			return FALSE; 
		if (D3DCheck(lpDev->SetMatrix(hMat, &data)) != D3D_OK) 
			return FALSE;

		return TRUE; 
	}

	gbool Get(LPDIRECT3DDEVICE lpDev,D3DMATRIX &data) 
	{ 
		D3DLASTERROR()

		if (D3DCheck(lpDev->GetMatrix(hMat, &data)) != D3D_OK) 
				return FALSE;
		return TRUE; 
	}

	gbool Delete(LPDIRECT3DDEVICE lpDev) 
	{ 
		D3DLASTERROR()
		if (hMat !=0) {
			if (D3DCheck(lpDev->DeleteMatrix(hMat)) != D3D_OK) 
				return FALSE;
			hMat = 0;
		}
		return TRUE; 
	}


	//! add to execute buffer
	//  if already
	void put(LPVOID &lpPointer,D3DTRANSFORMSTATETYPE what) const {
		 OP_STATE_TRANSFORM(1, lpPointer);
			 STATE_DATA(what, hMat, lpPointer);
	}

	//! return instruction size
	DWORD size() const { return sizeof(D3DINSTRUCTION) * 1 + sizeof(D3DSTATE) * 1; }

	//! global identity matrix 
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
class G3TextureParameters {
public :
	D3DTEXTUREFILTER min_filter;	// minification filter 
	D3DTEXTUREFILTER mag_filter;	// magnification filter 

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

    D3DTEXTUREBLEND defaultBlend;	// default blend function
    D3DTEXTUREBLEND unlitBlend;		// blend function for unlit textures 

	gbool gen_mipmaps;
	int supported_image_fmts;		// supported image formts for texture images ( image.h)
	// border color

    static const IS filters[]; // table of filter names 
    static const IS blends[];  // table of texture blend names 

	void SetMinFilter(int filter) {
        if (filter>=0) min_filter = (D3DTEXTUREFILTER) filter;
	};
	void SetMagFilter(int filter) {
        if (filter>=0) mag_filter = (D3DTEXTUREFILTER) filter;
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
        if (v>=0) min_filter = (D3DTEXTUREFILTER) v;
	};

	void SetMagFilter(const char *filter) {
        int v = Lookup(filters,filter);
        if (v>=0) mag_filter = (D3DTEXTUREFILTER) v;
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

	G3TextureParameters();

	//! Are mipmap definitions required
	gbool NeedMipmaps() { return( (min_filter >= D3DFILTER_MIPNEAREST) || (gen_mipmaps)); }

	//! set the maximum texture size
	int SetMaxSize(int size_x,int size_y);

	//! get max texture size
	void GetMaxSize(int &size_x,int &size_y);

	//! set the current maximal texture size
	int SetCurrentMaxSize(int size_x,int size_y);

	//! get current max texture size
	void GetCurrentMaxSize(int &size_x,int &size_y);

	//! set min texture size
	int SetMinSize(int size_x,int size_y);

	//! get min texture size
	void GetMinSize(int &size_x,int &size_y);


	// add to execute buffer
	void put(LPVOID &lpPointer) const {
        OP_STATE_RENDER(2, lpPointer);
        STATE_DATA(D3DRENDERSTATE_WRAPU, wrap_s, lpPointer);
        STATE_DATA(D3DRENDERSTATE_WRAPV, wrap_t, lpPointer);
	}

	// add to execute buffer
	void putall(LPVOID &lpPointer) const {
        OP_STATE_RENDER(3, lpPointer);
        //STATE_DATA(D3DRENDERSTATE_TEXTUREPERSPECTIVE, perspective, lpPointer);
		// WRAPU /WRAPV has a very different meansing, is set to false in SetState !!
        //STATE_DATA(D3DRENDERSTATE_WRAPU, wrap_s, lpPointer);
        //STATE_DATA(D3DRENDERSTATE_WRAPV, wrap_t, lpPointer);
        STATE_DATA(D3DRENDERSTATE_TEXTUREMAG, mag_filter, lpPointer);
        STATE_DATA(D3DRENDERSTATE_TEXTUREMIN, min_filter, lpPointer);
        STATE_DATA(D3DRENDERSTATE_TEXTUREMAPBLEND, blend, lpPointer);
	}

};

const char * GFogModeLookup(int v);
int GFogModeLookup(const char *v);

//@class  Fog/Depth Cueing options (glFog)

#define G3FOG_VERTEX 1
#define G3FOG_TABLE 2
#define G3FOG_RANGE 4

class G3Fog {
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



    G3Fog() {
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
// RenderState
// 


class RenderState : public GRenderState 
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

     // D3D Variables

	 LPDIRECT3D             lpD3D;      /* D3D object */
	 LPDIRECT3DDEVICE       lpDev;      /* D3D device */
	 LPDIRECT3DVIEWPORT     lpView;     /* D3D viewport */

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

	 int					  dxLevel;		  // level of direct X supported

	 gbool					  dx6tex;		   // use DX6 Texture interfaces

	 D3DDEVICEDESC			  caps;			  /* D3D caps */ 


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

#ifdef G3RIGHT_HANDED
	 gbool FlipFaces() { return(!canCull && !frontFaceCW); }
#else
	 gbool FlipFaces() { return(!canCull && frontFaceCW); }
#endif

	 // do we need to flip normals filling the exbuf ?
	 inline gbool FlipNormals() { return( /* !canCull &&  */frontFaceCW); }


	 //! query D3D capabilitis from lpDev 
	 gbool QueryCaps(D3DDEVICEDESC *pDesc=NULL);

	 //! convert RGBAColor to packed color format
	 static inline 
	 D3DCOLOR getD3DColor(const RGBAColor & color){ return D3DRGBA_X(color );} 



     // Helper functions 
	 gbool Execute (G3ExBuf &exBuf) 
	 { 
		 return exBuf.Execute(lpDev,lpView); 
	 }
	 
	 gbool ExecuteDraw (G3ExBuf &exBuf) { 
				FlushState(); 
				return exBuf.Execute(lpDev,lpView); 
	 }

	 // a global buffer for putting state change instruction
	 // buffer always open, flush with FlushState
	 G3ExBufDesc stateBuf;

	 // keep track on how many times BeginScene called 
	 int beginSceneCnt;

	 //! D3D BeginScene 
	 HRESULT BeginScene() {
          ASSERT(beginSceneCnt == 0);
		  beginSceneCnt++;
		  HRESULT res=lpDev->BeginScene();
          if (res!=D3D_OK) 
                D3DError("BeginScene failed",res);
        return(res);
	 }

	 HRESULT EndScene() {
            ASSERT(beginSceneCnt == 1);

			HRESULT res=lpDev->EndScene();
            if (res!=D3D_OK) 
                D3DError("EndScene failed",res);

		beginSceneCnt--;
        return(res);
     }
    
	 // apply any state changes // DX3
	 void FlushState_();
	 
	 inline void FlushState() {
		 if (stateBuf.Length()>0) FlushState_();
	 }

	 inline void FlushState(size_t space) {
		 if (space >= (stateBuf.Size()-stateBuf.Length())) FlushState_();
	 }
	 
	 // init state buf 
	 void InitState(size_t size=1024) {
		 stateBuf.Create(lpDev,size);
		 stateBuf.Lock();
	 }
	
	 void TermState() {
		 stateBuf.Unlock();
		 stateBuf.Release();
	 }

	 // set DX RenderState setting for DX3 .. DX5 
	inline void SetRenderState(D3DRENDERSTATETYPE dwRenderStateType,DWORD dwRenderState) {
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
        OP_STATE_RENDER(1, stateBuf.lpPointer);
			STATE_DATA(dwRenderStateType, dwRenderState, stateBuf.lpPointer);
	}
	
	inline void SetRenderStateEB(D3DRENDERSTATETYPE dwRenderStateType,DWORD dwRenderState) {
		if (doOwnLighting && dx5) { 
#ifdef _DEBUG
				GLastError = lpD3DDevice2->SetRenderState(dwRenderStateType,dwRenderState);
		        if (GLastError != D3D_OK)  D3DError("SetRenderState() failed",GLastError);

#else
				lpD3DDevice2->SetRenderState(dwRenderStateType,dwRenderState);
#endif
				return;
		}
        OP_STATE_RENDER(1, stateBuf.lpPointer);
			STATE_DATA(dwRenderStateType, dwRenderState, stateBuf.lpPointer);
	}

	// problem with DX5 & Exectue buffers, textures are not pers. correct 
	inline void SetRenderStateTex(D3DRENDERSTATETYPE dwRenderStateType,DWORD dwRenderState) {
		if (dx6) { 
#ifdef _DEBUG
				GLastError = lpD3DDevice3->SetRenderState(dwRenderStateType,dwRenderState);
		        if (GLastError != D3D_OK)  D3DError("SetRenderState() failed",GLastError);

#else
				lpD3DDevice3->SetRenderState(dwRenderStateType,dwRenderState);
#endif
				return;
		}
        OP_STATE_RENDER(1, stateBuf.lpPointer);
			STATE_DATA(dwRenderStateType, dwRenderState, stateBuf.lpPointer);
	}

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
        OP_STATE_LIGHT(1, stateBuf.lpPointer);
			STATE_DATA(dwLightStateType, dwLightState, stateBuf.lpPointer);
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
        OP_STATE_LIGHT(1, stateBuf.lpPointer);
			STATE_DATA(dwLightStateType, dwLightState, stateBuf.lpPointer);
	}



     G3Material    defaultMaterialValue; // value for defaul material 
     G3MaterialRef defaultMaterial;


 	 G3MaterialRef backgroundMaterial;
 	 GTextureRef  backgroundTexture;


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
	 gbool useVertexBuffer;		// use D3D vertex buffer

	 // global D3D D3DTLVERTEX buffer 
	
	 // Array<D3DTLVERTEX> vtlBuffer;
	 FloatArrayAligned  vtlBuffer;

	 // global D3D index buffer 
	 Array<WORD> indexBuffer; // for DrawPrimitiveIndexed
		
	 void *		katmaiState;	// Katmai Transform & Light state data 	

	 G3MaterialHandle *currentMaterial; // the current material 
	 
	 // to do: 
	 // void ComputeLighting(Point &p,Point &n,D3DCCOLOR &result);


	 D3DTEXTUREHANDLE hTex;  /* Handle for the texture on the material.*/
     GTextureRef  defaultTexture;


	 Matrix view;
	 Matrix projection;


	 Matrix model2screen; // new 


	 // emulation of texture matrix
	 Matrix textureMatrix;
	 



	 // set texture matrix, version Id is incremented
	 void SetTextureMatrix(const Matrix &m) { textureMatrix = m; textureMatrixVersion++; textureMatrixIdentity = FALSE; }

	 // set texture matrix, together with version ID, ID = 0, matrix is identity
	 void SetTextureMatrix(const Matrix &m, unsigned long version) { textureMatrix = m; textureMatrixVersion=version; textureMatrixIdentity = (version == 0); }

	 //! set texture matrix to identity, together with version ID, ID = 0, matrix is identity
	 void SetTextureMatrixIdentity() { if (!textureMatrixIdentity) { textureMatrix.Identity();  textureMatrixIdentity = TRUE; } }


	 //! D3D Versions of these matrices
	 G3Matrix viewMatrix;
	 G3Matrix projMatrix;
	 G3Matrix worldMatrix;

	 //! set matrix handles 
	 void SetTransforms(D3DMATRIXHANDLE hProj,
				D3DMATRIXHANDLE hView,
				D3DMATRIXHANDLE hWorld
	) 
	{
     OP_STATE_TRANSFORM(3, stateBuf.lpPointer);
        STATE_DATA(D3DTRANSFORMSTATE_WORLD, hWorld, stateBuf.lpPointer);
        STATE_DATA(D3DTRANSFORMSTATE_VIEW, hView, stateBuf.lpPointer);
        STATE_DATA(D3DTRANSFORMSTATE_PROJECTION, hProj, stateBuf.lpPointer);
	}

    //! set world modelling matrix 
    void SetWorldMatrix(D3DMATRIX &m)
    {
		if (doOwnLighting) {
			lightsTransformed=gfalse;
			matrixChanged=gtrue;
			return;
		}

		worldMatrix.Set(lpDev,m);
		// if world matrix handle is current
		// no add to buffer is necessary 
	}

	//! set matrix  to identity
	void SetWorldMatrixIdentity() {
		SetWorldMatrix(G3Matrix::identity);
	}

	//! set matrix 
	void SetWorldMatrix(Matrix &m);

    
	//! set view and projection matrix from camera 
    int SetCamera(GCamera *camera);


	//! set the ambient light color 
	void SetAmbientLight() {
		SetLightState(D3DLIGHTSTATE_AMBIENT, getD3DColor(ambient));
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



    G3MaterialHandle* materials; // list of all materials in use 
    G3TextureHandle* textures; // list of all textures in use 
    
	G3LightHandle* lights; // list of all lights in use 
	G3ExBufHandle* exBufs; // list of all execute buffers
	G3VBBufHandle* VBBufs; // list of all vertex buffer

    // Set Material, if result FALSE, need definition of material 
    gbool SetMaterial(G3MaterialRef &ref)
    {
		if (doOwnLighting) {
			if (!ref.handle) return FALSE;
			currentMaterial = ref.handle;
			materialChanged=gtrue;
			return TRUE;
		
		}
        if (!ref.Ok()) return(FALSE);
		if (!canDoRgbLighting && (hTex != ref.handle->hTexture)) return (FALSE); // textures doesn´t match
        // ref.handle->put(stateBuf.lpPointer);
		SetLightStateMat(D3DLIGHTSTATE_MATERIAL,ref.handle->hMat);
        return(TRUE);
    }
    
	gbool SetMaterial(G3MaterialRef &ref,gbool whiteDiffuse)
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
		SetLightStateMat(D3DLIGHTSTATE_MATERIAL,ref.handle->hMat);
        return(TRUE);
    }

    // Set Material
    gbool SetMaterial(G3MaterialRef &ref,			
            // material parameters
			float ambient[3],float diffuse[3],float specular[3],float emissive[3],
			float alpha,float power,gbool whiteDiffuse);

    // Set Material solid 
    gbool SetMaterial(G3MaterialRef &ref,const RGBAColor &color);

	// set material properties
	gbool SetMaterial(G3MaterialRef &ref,D3DMATERIAL &mat);
	
	// set material, but diffuse is white 
	gbool SetMaterialW(G3MaterialRef &ref,D3DMATERIAL &mat); 
    
    // create a new material handle with a ref cnt of 1 
    G3MaterialHandle* CreateMaterial();

    // delete an no longer userd material
    void DeleteMaterial(G3MaterialHandle *m);

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
			return SetTexture((G3TextureHandle2 *) ref.handle);

		G3TextureHandle *h = (G3TextureHandle *) ref.handle; // downcast  
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
    gbool SetTexture(G3TextureHandle *h)
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
    gbool SetTexture(G3TextureHandle2 *h)
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
		
		G3TextureHandle *h = (G3TextureHandle *) ref.handle; // downcast  

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

    //! create a new Texture handle with a ref cnt of 1 
    G3TextureHandle* CreateTexture();

    //! delete an no longer used Texture
    void DeleteTexture(G3TextureHandle *m);
    void DeleteTexture(GTextureHandle *m) { DeleteTexture((G3TextureHandle *)m); }


	//! compute target texture format size 
	void GetTextureTargetSize(int &width,int &height,int roundUpLimit=128);

	//! get max texture size
	void GetTextureMaxSize(int &size_x,int &size_y) { textureParameters.GetMaxSize(size_x,size_y); }

	//! get current max texture size
	void GetTextureCurrentMaxSize(int &size_x,int &size_y) { textureParameters.GetCurrentMaxSize(size_x,size_y); }

	//! set current max texture size
	void SetTextureCurrentMaxSize(int size_x,int size_y) { textureParameters.SetCurrentMaxSize(size_x,size_y); }

	//! get min texture size
	void GetTextureMinSize(int &size_x,int &size_y)  { textureParameters.GetMinSize(size_x,size_y); }


  

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
	G3LightHandle *firstLight;

	// create a new light handle with a ref cnt of 1 
	G3LightHandle* CreateLight();

	// delete a no longer used light
	void DeleteLight(G3LightHandle *m);

	// release all D3D Light handles and remove from view 
	void ReleaseLights();

	// Delete all D3D Light handles
	// should only be called if all references deleted !!!!!!!!!!!!
	void DeleteLights();

	// Delete all unused D3D Light handles
	void DeleteUnusedLights();

	 // Set Light, define data and add to viewport 
	gbool SetLight(G3LightRef &ref); 

	// Remove Light from viewport , but keep structure intact 
	void RemoveLight(G3LightRef &ref)
	{
		if (ref.handle != NULL) {
			if (ref.handle->enabled) ref.handle->Delete(lpView);
		}
	}

	// delete a no longer used light, use also to disable light 
	void DeleteLight(G3LightRef &ref){
		if (ref.handle != NULL) {
            DeleteLight(ref.handle);
			ref.handle = NULL;
		}
	}


	// get the light handle info structure for light reference 
	// fill in data and call SetLight(ref)
	G3LightHandle* GetLight(G3LightRef &ref) 
	{
		if (ref.handle == NULL)
            ref.handle = CreateLight();
		return ref.handle;
	}
		


	gbool SetLight(G3LightHandle *light)
	{	
		return light->Set(lpD3D);
	}

	gbool AddLight(G3LightHandle *light)
	{	
		return light->Add(lpView);
	}

	void UpdateAmbientLight(); // recompute ambient light from all lights

	// light stack
	Array<G3LightHandle *> lightStack;

	// add/Enable  light at stack index I 
	gbool EnableLight(int index, G3LightRef &ref);
	
	// remove light at stack index I 
	gbool DisableLight(int index);



	// exBufs
	gbool exBufAddMat; // add material settings to execute buffer 
	gbool exBufCombine; // combine execute buffers 

    // create a new ExBuf handle with a ref cnt of 1 
    G3ExBufHandle* CreateExBuf();

    // delete an no longer userd ExBuf
    void DeleteExBuf(G3ExBufHandle *m);

    // release all D3D ExBuf handles
    void ReleaseExBufs();

    // release all D3D ExBuf handles using textures
    void ReleaseTexturedExBufs();

    // Delete all D3D ExBuf handles
    // should only be called if all references deleted !!!!!!!!!!!!1
    void DeleteExBufs();

    // Delete all D3D ExBuf handles
    // should only be called if all references deleted !!!!!!!!!!!!1
    void DeleteUnusedExBufs();


    //-----------------//-----------------//-----------------
	//						vertex buffer
	//-----------------//-----------------//-----------------
	

	//! create a new VBBufs handle with a ref cnt of 1 and attach to bufRef
	gbool CreateVBBuf(G3VBBufRef &bufRef,DWORD dwFVF, int iNumVerts);

	//! lock a valid vertex buffer for writing and return point
	gbool LockVB(G3VBBufRef &bufRef, D3DVALUE* &pVertices)
	{
		GLastError =  bufRef.handle->lpD3DVBBuf->Lock(DDLOCK_WAIT | DDLOCK_WRITEONLY, (void **) &pVertices, NULL);
		if (FAILED(GLastError)) {
			D3DError("LockVB() failed",GLastError);
			return gfalse;
		}
		return gtrue;
	}

	//! lock a valid vertex buffer for writing and return point
	gbool UnlockVB(G3VBBufRef &bufRef,gbool optimize=gfalse)
	{
		GLastError =  bufRef.handle->lpD3DVBBuf->Unlock();
		if (FAILED(GLastError)) {
			D3DError("UnlockVB() failed",GLastError);
			return gfalse;
		}
		if (optimize) {
			GLastError =  bufRef.handle->lpD3DVBBuf->Optimize(lpD3DDevice3, 0);
			if (GLastError != D3D_OK) D3DError("UnlockVB() Optimize failed",GLastError);
		}
		return gtrue;
	}

    //! release all D3D VBBuf handles
    void ReleaseVBBufs();


    //! Delete all D3D VBBuf handles
    //! should only be called if all references deleted !!!!!!!!!!!!1
    void DeleteVBBufs();

    //! Delete all unused D3D VBBuf handles
    void DeleteUnusedVBBufs();

    //! delete an no longer used VBBuf
    void DeleteVBBuf(G3VBBufHandle *m);

     //! Texture options 
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

	 G3Fog fog;			// fog settings
	 G3TextureParameters textureParameters;

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
	 RGBAColor ambient;		// ambient light color

	 G3Material material;
	 G3Material backMaterial;


	 RenderFlags vertices;
	 RenderFlags normals;
	 RenderFlags edges;
	 RenderFlags faces;
	 RenderFlags bboxes;

	 RGBAColor currentColor; // current unlit color
	 D3DCOLOR  currentColorD3D; // and shadowed in DWORD
	 
	 inline D3DCOLOR getCurrentColorD3D() { return currentColorD3D;} 
	 
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


	 RenderState();
	 ~RenderState();

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
        OP_STATE_RENDER(2, stateBuf.lpPointer);
	      STATE_DATA(D3DRENDERSTATE_SRCBLEND  , blendSrcFunc, stateBuf.lpPointer);
		  STATE_DATA(D3DRENDERSTATE_DESTBLEND , blendDestFunc, stateBuf.lpPointer);

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
	 };
	 
     void SetLighting() 
	 {
			//if (lighting) glEnable(GL_LIGHTING);
			//else glDisable(GL_LIGHTING);
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

	 gbool SetMaterial(const G3Material &Material) {
			//material = Material;
			//material.glSet();
			//if (localColors) material.diffuse.glSet();
            defaultMaterialValue = Material;
            return SetMaterial();


	 };
	 gbool SetMaterial() {
			//material.glSet();
			//if (localColors) material.diffuse.glSet();
		 defaultMaterialValue.hTexture = hTex; // new 17.09
         return SetMaterial(defaultMaterial,defaultMaterialValue);
	 };

	 // set background color = gl clear color
	 void SetBackgroundColor(const RGBAColor &Color) {
	     background = Color;
         SetBackgroundColor();
	 };

     gbool SetBackgroundColor() {  // seems to cause flash if called during each rendering cycle
	    //hBackgroundMaterial.SetSolid (lpD3D,lpDev,background);
	    //hBackgroundMaterial.SetBackground(lpView);
        if (backgroundMaterial.handle == NULL)
            backgroundMaterial.handle = CreateMaterial();
	   
	   G3TextureHandle *h = (G3TextureHandle *) backgroundTexture.handle; // downcast  


       // set material properties
	   if (backgroundMaterial.handle && backgroundMaterial.handle->SetSolid(lpD3D,lpDev,background,(h ? h->GetTextureHandle():NULL))
			) {
                // set as background material
                backgroundMaterial.handle->SetBackground(lpView);
                return(TRUE);
       } else return(FALSE);
	 };

	 // set current color // e.g. material diffuse, needed for unlit execute buffers
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
	

	 //! apply the fogsettings described in the Fog structure
	 void SetFog() {
		 if (fog.enable) {
			 SetFog_();
		 }
		 else {
			SetFogEnable(FALSE);
		 }
	 };

	 //! set global ambient light color
	 void SetAmbientColor(const RGBAColor &Color) {
	     ambient = Color;
		 ambient.a = Color.r;
		 SetAmbientColor();
	 };

	 void SetAmbientColor() {
		SetLightState(D3DLIGHTSTATE_AMBIENT, getD3DColor(ambient));
	 };

	 void SetColorModel(D3DCOLORMODEL model)
	 {
		colorModel = model;
		SetLightState(D3DLIGHTSTATE_COLORMODEL, model);
	 }

	 //! Enable display of local vertex colors in lighted mode
	 void SetLocalColors(gbool mode) {
		  if (mode == localColors) return;
		  localColors = mode;
          SetLocalColors();
     }

     void SetLocalColors() {
          if (localColors ) {
		  }
		  else;
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


	 int (*AbortChecker)(RenderState &s);
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

	//! get a 2D renderer to current state
	GRenderer2D* getRenderer2D()  { return NULL; }

    // ******** end from glRender.h

};

/* from HELP file 

D3DOPCODE 

typedef enum _D3DOPCODE {
    D3DOP_POINT           = 1,
    D3DOP_LINE            = 2,
    D3DOP_TRIANGLE        = 3,
    D3DOP_MATRIXLOAD      = 4,
    D3DOP_MATRIXMULTIPLY  = 5,
    D3DOP_STATETRANSFORM  = 6,
    D3DOP_STATELIGHT      = 7,
    D3DOP_STATERENDER     = 8,
    D3DOP_PROCESSVERTICES = 9,
    D3DOP_TEXTURELOAD     = 10,
    D3DOP_EXIT            = 11,
    D3DOP_BRANCHFORWARD   = 12,
    D3DOP_SPAN            = 13,
    D3DOP_SETSTATUS       = 14,

} D3DOPCODE;


Contains the opcodes for execute buffers. 

D3DOP_POINT 

Sends a point to the renderer. Operand data is described by the D3DPOINT structure. 

D3DOP_LINE 

Sends a line to the renderer. Operand data is described by the D3DLINE structure. 

D3DOP_TRIANGLE 

Sends a triangle to the renderer. Operand data is described by the D3DTRIANGLE structure. 

D3DOP_MATRIXLOAD 

Triggers a data transfer in the rendering engine. Operand data is described by the D3DMATRIXLOAD structure. 

D3DOP_MATRIXMULTIPLY 

Triggers a data transfer in the rendering engine. Operand data is described by the D3DMATRIXMULTIPLY structure. 

D3DOP_STATETRANSFORM 

Sets the value of internal state variables in the rendering engine for the transformation module. Operand data is a variable token and the new value. The token identifies the internal state variable, and the new value is the value to which that variable should be set. For more information about these variables, see the D3DSTATE structure and the D3DTRANSFORMSTATETYPE enumerated type. 

D3DOP_STATELIGHT 

Sets the value of internal state variables in the rendering engine for the lighting module. Operand data is a variable token and the new value. The token identifies the internal state variable, and the new value is the value to which that variable should be set. For more information about these variables, see the D3DSTATE structure and the D3DLIGHTSTATETYPE enumerated type. 

D3DOP_STATERENDER 

Sets the value of internal state variables in the rendering engine for the rendering module. Operand data is a variable token and the new value. The token identifies the internal state variable, and the new value is the value to which that variable should be set. For more information about these variables, see the D3DSTATE structure and the D3DRENDERSTATETYPE enumerated type. 

D3DOP_PROCESSVERTICES 

Sets both lighting and transformations for vertices. Operand data is described by the D3DPROCESSVERTICES structure. 

D3DOP_TEXTURELOAD 

Triggers a data transfer in the rendering engine. Operand data is described by the D3DTEXTURELOAD structure. 

D3DOP_EXIT 

Signals that the end of the list has been reached. 

D3DOP_BRANCHFORWARD 

Enables a branching mechanism within the execute buffer. For more information, see the D3DBRANCH structure. 

D3DOP_SPAN 

Spans a list of points with the same y value. For more information, see the D3DSPAN structure. 

D3DOP_SETSTATUS 

Resets the status of the execute buffer. For more information, see the D3DSTATUS structure. 

An execute buffer has two parts: an array of vertices (each typically with position, normal vector, and texture coordinates) and an array of opcode/operand groups. One opcode can have several operands following it; the system simply performs the relevant operation on each operand. 



D3DRENDERSTATETYPE 
==================
typedef enum _D3DRENDERSTATETYPE {
    D3DRENDERSTATE_TEXTUREHANDLE      = 1,    
    D3DRENDERSTATE_ANTIALIAS          = 2,    
    D3DRENDERSTATE_TEXTUREADDRESS     = 3,    
    D3DRENDERSTATE_TEXTUREPERSPECTIVE = 4,    
    D3DRENDERSTATE_WRAPU              = 5,    
    D3DRENDERSTATE_WRAPV              = 6,    
    D3DRENDERSTATE_ZENABLE            = 7,    
    D3DRENDERSTATE_FILLMODE           = 8,    
    D3DRENDERSTATE_SHADEMODE          = 9,    
    D3DRENDERSTATE_LINEPATTERN        = 10,   

    D3DRENDERSTATE_MONOENABLE         = 11,   
    D3DRENDERSTATE_ROP2               = 12,   
    D3DRENDERSTATE_PLANEMASK          = 13,   
    D3DRENDERSTATE_ZWRITEENABLE       = 14,   
    D3DRENDERSTATE_ALPHATESTENABLE    = 15,   
    D3DRENDERSTATE_LASTPIXEL          = 16,   
    D3DRENDERSTATE_TEXTUREMAG         = 17,   
    D3DRENDERSTATE_TEXTUREMIN         = 18,   
    D3DRENDERSTATE_SRCBLEND           = 19,   
    D3DRENDERSTATE_DESTBLEND          = 20,   

    D3DRENDERSTATE_TEXTUREMAPBLEND    = 21,   
    D3DRENDERSTATE_CULLMODE           = 22,   
    D3DRENDERSTATE_ZFUNC              = 23,   
    D3DRENDERSTATE_ALPHAREF           = 24,   
    D3DRENDERSTATE_ALPHAFUNC          = 25,   
    D3DRENDERSTATE_DITHERENABLE       = 26,   
    D3DRENDERSTATE_BLENDENABLE        = 27,   
    D3DRENDERSTATE_FOGENABLE          = 28,   
    D3DRENDERSTATE_SPECULARENABLE     = 29,   
    D3DRENDERSTATE_ZVISIBLE           = 30,   

    D3DRENDERSTATE_SUBPIXEL           = 31,   
    D3DRENDERSTATE_SUBPIXELX          = 32,   
    D3DRENDERSTATE_STIPPLEDALPHA      = 33,   
    D3DRENDERSTATE_FOGCOLOR           = 34,   
    D3DRENDERSTATE_FOGTABLEMODE       = 35,   
    D3DRENDERSTATE_FOGTABLESTART      = 36,   
    D3DRENDERSTATE_FOGTABLEEND        = 37,   
    D3DRENDERSTATE_FOGTABLEDENSITY    = 38,   
    D3DRENDERSTATE_STIPPLEENABLE      = 39, 
    D3DRENDERSTATE_STIPPLEPATTERN00   = 64,
       // Stipple patterns 01 through 30 omitted here.

    D3DRENDERSTATE_STIPPLEPATTERN31   = 95,
} D3DRENDERSTATETYPE;


Describes the render state for the D3DOP_STATERENDER opcode. This enumerated type is part of the D3DSTATE structure. The values mentioned in the following descriptions are set in the second member of this structure. 

D3DRENDERSTATE_TEXTUREHANDLE 

Texture handle. The default value is NULL. 

D3DRENDERSTATE_ANTIALIAS 

Antialiasing primitive edges. The default value is FALSE. 

D3DRENDERSTATE_TEXTUREADDRESS 

One of the members of the D3DTEXTUREADDRESS enumerated type. The default value is D3DTADDRESS_WRAP. 

D3DRENDERSTATE_TEXTUREPERSPECTIVE 

TRUE for perspective correction. The default value is FALSE. 

D3DRENDERSTATE_WRAPU 

TRUE for wrapping in u direction. The default value is FALSE. 

D3DRENDERSTATE_WRAPV 

TRUE for wrapping in v direction. The default value is FALSE. 

D3DRENDERSTATE_ZENABLE 

TRUE to enable the z-buffer comparison test when writing to the frame buffer. The default value is FALSE. 

D3DRENDERSTATE_FILLMODE 

One or more members of the D3DFILLMODE enumerated type. The default value is D3DFILL_SOLID. 

D3DRENDERSTATE_SHADEMODE 

One or more members of the D3DSHADEMODE enumerated type. The default value is D3DSHADE_GOURAUD. 

D3DRENDERSTATE_LINEPATTERN 

The D3DLINEPATTERN structure. The default values are 0 for wRepeatPattern and 0 for wLinePattern. 

D3DRENDERSTATE_ROP2 

One of the 16 ROP2 binary raster operations specifying how the supplied pixels are combined with the pixels of the display surface. The default value is R2_COPYPEN. Applications can use the D3DPRASTERCAPS_ROP2 flag in the dwRasterCaps member of the D3DPRIMCAPS structure to determine whether additional raster operations are supported. 

D3DRENDERSTATE_PLANEMASK 

Physical plane mask whose type is ULONG. The default value is ~0. 

D3DRENDERSTATE_ZWRITEENABLE 

TRUE to enable z writes. The default value is TRUE. This member enables an application to prevent the system from updating the z-buffer with new z values. 

D3DRENDERSTATE_ALPHATESTENABLE 

TRUE to enable alpha tests. The default value is FALSE. This member enables applications to turn off the tests that otherwise would accept or reject a pixel based on its alpha value. 

D3DRENDERSTATE_LASTPIXEL 

TRUE to prevent drawing the last pixel in a line. The default value is FALSE. 

D3DRENDERSTATE_TEXTUREMAG 

One of the members of the D3DTEXTUREFILTER enumerated type. The default value is D3DFILTER_NEAREST. 

D3DRENDERSTATE_TEXTUREMIN 

One of the members of the D3DTEXTUREFILTER enumerated type. The default value is D3DFILTER_NEAREST. 

D3DRENDERSTATE_SRCBLEND 

One of the members of the D3DBLEND enumerated type. The default value is D3DBLEND_ONE. 

D3DRENDERSTATE_DSTBLEND 

One of the members of the D3DBLEND enumerated type. The default value is D3DBLEND_ZERO. 

D3DRENDERSTATE_TEXTUREMAPBLEND 

One of the members of the D3DTEXTUREBLEND enumerated type. The default value is D3DTBLEND_MODULATE. 

D3DRENDERSTATE_CULLMODE 

One of the members of the D3DCULL enumerated type. The default value is D3DCULL_CCW. Software renderers have a fixed culling order and do not support changing the culling mode. 

D3DRENDERSTATE_ZFUNC 

One of the members of the D3DCMPFUNC enumerated type. The default value is D3DCMP_LESSEQUAL. This member enables an application to accept or reject a pixel based on its distance from the camera. 

D3DRENDERSTATE_ALPHAREF 

Value specifying a reference alpha value against which pixels are tested when alpha-testing is enabled. This value's type is D3DFIXED. The default value is 0. 

D3DRENDERSTATE_ALPHAFUNC 

One of the members of the D3DCMPFUNC enumerated type. The default value is D3DCMP_ALWAYS. This member enables an application to accept or reject a pixel based on its alpha value. 

D3DRENDERSTATE_DITHERENABLE 

TRUE to enable dithering. The default value is FALSE. 

D3DRENDERSTATE_BLENDENABLE 

TRUE to enable alpha blending. The default value is FALSE. 

D3DRENDERSTATE_FOGENABLE 

TRUE to enable fog. The default value is FALSE. 

D3DRENDERSTATE_SPECULARENABLE 

TRUE to enable specular. The default value is TRUE. 

D3DRENDERSTATE_ZVISIBLE 

TRUE to enable z-checking. The default value is FALSE. Z-checking is a culling technique in which a polygon representing the screen space of an entire group of polygons is tested against the z-buffer to discover whether any of the polygons should be drawn. 

D3DRENDERSTATE_SUBPIXEL 

TRUE to enable subpixel correction. The default value is FALSE. 

D3DRENDERSTATE_SUBPIXELX 

TRUE to enable correction in X only. The default value is FALSE. 

D3DRENDERSTATE_STIPPLEDALPHA 

TRUE to enable stippled alpha. The default value is FALSE. 

D3DRENDERSTATE_FOGCOLOR 

Value whose type is D3DCOLOR. The default value is 0. 

D3DRENDERSTATE_FOGTABLEMODE 

One of the members of the D3DFOGMODE enumerated type. The default value is D3DFOG_NONE. 

D3DRENDERSTATE_FOGTABLESTART 

Fog table start. This is the position at which fog effects begin for linear fog mode. 

D3DRENDERSTATE_FOGTABLEEND 

Fog table end. This is the position at which fog effects reach their maximum density for linear fog mode. 

D3DRENDERSTATE_FOGTABLEDENSITY 

Sets the maximum fog density for linear fog mode. This value can range from 0 to 1. 

D3DRENDERSTATE_STIPPLEENABLE 

Enables stippling in the device driver. When stippled alpha is enabled, it must override the current stipple pattern. When stippled alpha is disabled, the stipple pattern must be returned. 

D3DRENDERSTATE_STIPPLEPATTERN00 through D3DRENDERSTATE_STIPPLEPATTERN31 

Stipple pattern. Each render state applies to a separate line of the stipple pattern. 


*/

#endif
