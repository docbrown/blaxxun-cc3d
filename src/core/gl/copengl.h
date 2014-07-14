// COpenGL.h : header file
//
// COpenGL class
//

#ifndef __WGL__
#define __WGL__

#ifdef WIN32

#include "grenderbase.h"

class GOpenglDeviceWGL : public GRenderDevice
{

public://functions
	HGLRC                  m_hglrc;	  //OpenGL Rendering Context
	HDC                    m_hdc; // MFC can use CDC

	int                    m_maxpfd;
   
	PIXELFORMATDESCRIPTOR  m_PixFmtDesc;
	
	//Palette
	HPALETTE 			   m_hOldPalette, m_hPalette; // HG
   
   //
	//constructor
	//
	GOpenglDeviceWGL(HDC hdc = NULL, PIXELFORMATDESCRIPTOR *pfd = NULL);
	//
	//destructor
	//
    ~GOpenglDeviceWGL();


	// destroy device
	void Destroy() {	ReleaseGLRC(); }


	// set a default OpenGL pixel fromat
	int SetDefaultPixelFormat(BOOL DoubleBuffer=TRUE,int depthBits=16, BOOL softwareRendering=FALSE, BOOL Rgb=TRUE);



    BOOL Create(CDC* pdcMemory,
 				   int iPixelType 	= 	PFD_TYPE_RGBA,
 				   DWORD dwFlags	   =	PFD_SUPPORT_OPENGL |     // Use OpenGL
									         PFD_SUPPORT_GDI |	 
                                    PFD_GENERIC_FORMAT |     // software format
									         PFD_DRAW_TO_BITMAP 
                                    );    // Pixel format is for a bitmap.

 	// Helpful functions
	void OutputGlError(char* label); 
	void MakeCurrent();

	
   // Attributes
	
   HPALETTE GetPalette() {return m_hPalette;}

   
   BOOL m_bDoubleBuffer ; // Controls calling SwapBuffers

	BOOL m_bDrawToBitmap ;

	//
	//utility functions
	//
	void  Init(PIXELFORMATDESCRIPTOR *pfd);
	BOOL  GetGLRC(HDC hdc);
	
	BOOL  ReleaseGLRC();
	BOOL  SetupPixelFormat(HDC hdc, PIXELFORMATDESCRIPTOR *pPFD);

	BOOL SetPalette();

	int   GetCurPFDIndex();
	int   GetMaxPFIndex(HDC hdc);
	BOOL  IsNativeIndex(HDC hdc, int nIdx);
	BOOL  IsDeviceIndex(HDC hdc, int idx);
	BOOL  IsSurfaceInitialized() {return m_fIsInitialized;}
	void  SetSurfaceInitialized() {m_fIsInitialized = TRUE;}
	void  SetSurfaceNotInitialized() {m_fIsInitialized = FALSE;}  // HG
	//
	//class wrappers for win32 pix format and buffering api
	//and WGL functions
	//
    int   ChoosePixelFormat(HDC hdc, PIXELFORMATDESCRIPTOR *ppfd);
    int   DescribePixelFormat(HDC hdc, int iPixelFormat, 
                              UINT nBytes, 
                              PIXELFORMATDESCRIPTOR *ppfd);
    int   GetPixelFormat(HDC hdc);
    BOOL  SetPixelFormat(HDC hdc, int iPixelFormat, PIXELFORMATDESCRIPTOR *ppfd);
    BOOL  SwapBuffers(HDC hdc);
    HGLRC wglCreateContext(HDC hdc);
    BOOL  wglDeleteContext(HGLRC hglrc);
    HGLRC wglGetCurrentContext(void);
    HDC   wglGetCurrentDC(void);
    BOOL  wglMakeCurrent(HDC hdc, HGLRC hglrc);
    BOOL  wglUseFontBitmaps(HDC hdc, DWORD first, DWORD count, DWORD listbase);

    // HG Is current pixelformat of type color index ?
	 BOOL IsColorIndex();
    void SetOneColor(int index, float r, float g, float b);


private://data
	 BOOL m_fIsInitialized;
    static unsigned char   m_oneto8[2];
    static unsigned char   m_twoto8[4];
    static unsigned char   m_threeto8[8];
    static int             m_defaultOverride[13];
    static PALETTEENTRY    m_defaultPalEntry[20];

    void  CreateRGBPalette(HDC hDC);
    UCHAR ComponentFromIndex(int i, UINT nbits, UINT shift);

};					   
#endif


#endif // __WGL__
