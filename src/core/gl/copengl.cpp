// implementation 
//
//
#include "stdafx.h"

#ifdef _OGL

//#include "GOpenglDeviceWGL.h"
#include "COpengl.h"

#define MAX_GENERIC_PFD (1*8+4*4)
#define MIN_GENERIC_PFD 1

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Static member data initialization

  unsigned char GOpenglDeviceWGL::m_threeto8[8] = {
    0, 0111>>1, 0222>>1, 0333>>1, 0444>>1, 0555>>1, 0666>>1, 0377
  };
  unsigned char GOpenglDeviceWGL::m_twoto8[4] = {
    0, 0x55, 0xaa, 0xff
  };
  unsigned char GOpenglDeviceWGL::m_oneto8[2] = {
    0, 255
  };
  int GOpenglDeviceWGL::m_defaultOverride[13] = {
    0, 3, 24, 27, 64, 67, 88, 173, 181, 236, 247, 164, 91
  };

  PALETTEENTRY GOpenglDeviceWGL::m_defaultPalEntry[20] = {
    { 0,   0,   0,    0 },
    { 0x80,0,   0,    0 },
    { 0,   0x80,0,    0 },
    { 0x80,0x80,0,    0 },
    { 0,   0,   0x80, 0 },
    { 0x80,0,   0x80, 0 },
    { 0,   0x80,0x80, 0 },
    { 0xC0,0xC0,0xC0, 0 },

    { 192, 220, 192,  0 },
    { 166, 202, 240,  0 },
    { 255, 251, 240,  0 },
    { 160, 160, 164,  0 },

    { 0x80,0x80,0x80, 0 },
    { 0xFF,0,   0,    0 },
    { 0,   0xFF,0,    0 },
    { 0xFF,0xFF,0,    0 },
    { 0,   0,   0xFF, 0 },
    { 0xFF,0,   0xFF, 0 },
    { 0,   0xFF,0xFF, 0 },
    { 0xFF,0xFF,0xFF, 0 }
  };
  
/////////////////////////////////////////////////////////////////////////////
// Constructor

GOpenglDeviceWGL::GOpenglDeviceWGL(HDC hdc, PIXELFORMATDESCRIPTOR *pfd)
{
  
  type = GR_OGL_DEVICE;

  m_hdc = hdc;
  m_hglrc = NULL;
  m_fIsInitialized = FALSE;
  m_hPalette = 0; // HG
  m_hOldPalette = 0; // HG
  
  m_bDoubleBuffer = FALSE ;
  m_bDrawToBitmap = FALSE ;

  if (pfd != NULL)
    Init(pfd);
}

void GOpenglDeviceWGL::Init(PIXELFORMATDESCRIPTOR *pfd)
{
	memcpy(&m_PixFmtDesc, pfd, sizeof(PIXELFORMATDESCRIPTOR));
}

/////////////////////////////////////////////////////////////////////////////
// Destructor

GOpenglDeviceWGL::~GOpenglDeviceWGL()
{
}



int GOpenglDeviceWGL::SetDefaultPixelFormat(BOOL DoubleBuffer,int depthBits, BOOL softwareRendering, BOOL Rgb)
{
	int nNewIdx=0;

	  DWORD dwFlags = (DWORD)0;

	  DWORD type = (::GetObjectType(m_hdc));

#ifdef _DEBUG
	  if (type != OBJ_DC) {
		  CString x;
		  x.Format("DC type = %d ",type);
		  AfxMessageBox(x);
	  }
#endif

	  memset(&m_PixFmtDesc, 0, sizeof(PIXELFORMATDESCRIPTOR));
	  dwFlags = 0;
	  if (type == OBJ_DC) dwFlags |=  PFD_DRAW_TO_WINDOW;
	  dwFlags |=  PFD_SUPPORT_OPENGL;
	  if (type == OBJ_DC) dwFlags |= (DoubleBuffer) ? PFD_DOUBLEBUFFER : 0;
	  
	  if (softwareRendering) { 
		  dwFlags |= PFD_GENERIC_FORMAT | PFD_SUPPORT_GDI;
	  } 	
	  m_PixFmtDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	  m_PixFmtDesc.nVersion = 1;
	  m_PixFmtDesc.dwFlags = dwFlags;
	  m_PixFmtDesc.iPixelType = (Rgb) ? PFD_TYPE_RGBA : PFD_TYPE_COLORINDEX;
	  m_PixFmtDesc.cDepthBits = depthBits;

	  nNewIdx = ChoosePixelFormat(m_hdc, &m_PixFmtDesc);

	  if ((IsNativeIndex(m_hdc, nNewIdx)) || (IsDeviceIndex(m_hdc, nNewIdx))) {

		if (GetGLRC(m_hdc)) {
			SetSurfaceNotInitialized();
		} else {
		  //MessageBox("Sorry, OpenGL Pixel Format can not be changed", NULL, MB_OK | MB_ICONEXCLAMATION);
		  nNewIdx = -1;
		}
	  }
	  else {
		 //MessageBox("Unsupported OpenGL Pixel Format", NULL, MB_OK | MB_ICONEXCLAMATION);
		 nNewIdx = -1;
	  }	
	 return(nNewIdx);
}

   
/////////////////////////////////////////////////////////////////////////////
// Utility functions

BOOL GOpenglDeviceWGL::GetGLRC(HDC hdc)
{ 
   BOOL bRet = TRUE;

   m_maxpfd = GetMaxPFIndex(m_hdc);

   if (SetupPixelFormat(hdc, &m_PixFmtDesc))
   {
     if ((m_hglrc = wglCreateContext(hdc)) != NULL)
	 {
	   if (!wglMakeCurrent(hdc, m_hglrc))
	   {
	    wglDeleteContext(m_hglrc);
		 m_hglrc = NULL; /* hg */
		 bRet = FALSE;
	   }
	 }
	 else bRet = FALSE;
   }
   else 
     bRet = FALSE;

   return bRet;
}

BOOL GOpenglDeviceWGL::SetPalette()
{
    if (m_hPalette) {   	
        ::SelectPalette(m_hdc,m_hPalette, FALSE);
        UINT u = ::RealizePalette(m_hdc);
		return TRUE;

    } else return FALSE;
}

BOOL GOpenglDeviceWGL::ReleaseGLRC()
{
  BOOL bRet = TRUE;
  HDC   hdc;
  HGLRC hglrc;
  
  if (hglrc = wglGetCurrentContext())
  {
    //
    //get the DC associated with the rendering context
    //
    hdc = wglGetCurrentDC();
    //
    //make the rendering context not current.
    //
    wglMakeCurrent(NULL, NULL);
    //
    //nuke the DC
    //
  // HG ?????  ::ReleaseDC(hwnd, hdc);
	//
	//nuke the rendering context
	//
	wglDeleteContext(hglrc);

	// HG  Dont understand above code, m_hglrc should be deleted
	
	m_hglrc = NULL;  // HG
  	m_fIsInitialized = FALSE; //HG
  }	 
  else bRet = FALSE;
  return bRet;
}

int GOpenglDeviceWGL::GetMaxPFIndex(HDC hdc)
{
  PIXELFORMATDESCRIPTOR pfd;
  
  int ipfdmax = DescribePixelFormat(hdc, 1, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

  return (ipfdmax);
}

BOOL GOpenglDeviceWGL::IsNativeIndex(HDC hdc, int idx)
{
  ASSERT (hdc);
  ASSERT (idx > 0);

  BOOL bRet = FALSE;
  PIXELFORMATDESCRIPTOR pfd;
  int ipfdmax = DescribePixelFormat(hdc, idx, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

  if (pfd.dwFlags & PFD_DRAW_TO_WINDOW)
    bRet = TRUE;
  return (bRet);
}

BOOL GOpenglDeviceWGL::IsDeviceIndex(HDC hdc, int idx)
{
  ASSERT (hdc);
  ASSERT (idx > 0);

  BOOL bRet = FALSE;
  PIXELFORMATDESCRIPTOR pfd;
  int ipfdmax = DescribePixelFormat(hdc, idx, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

  if (!(pfd.dwFlags & PFD_GENERIC_FORMAT))
    bRet = TRUE;
  return (bRet);
}

// HG Is current pixelformat of type color index ?
BOOL GOpenglDeviceWGL::IsColorIndex()
{
 return (m_PixFmtDesc.iPixelType == PFD_TYPE_COLORINDEX);
}
            
              
BOOL GOpenglDeviceWGL::SetupPixelFormat(HDC hdc, PIXELFORMATDESCRIPTOR *pPFD)
{
   BOOL bRet = TRUE;
   int pixelformat;
	
   if (!(pixelformat = ChoosePixelFormat(hdc, pPFD)))
     bRet = FALSE;

   if (!SetPixelFormat(hdc, pixelformat, pPFD))
     bRet = FALSE;

   if (bRet)
     CreateRGBPalette(hdc);

   return bRet;
}

int GOpenglDeviceWGL::GetCurPFDIndex()
{
  int icuridx = GetPixelFormat(wglGetCurrentDC());
  return (icuridx);
  
}

unsigned char GOpenglDeviceWGL::ComponentFromIndex(int i, UINT nbits, UINT shift)
{
    unsigned char val;

    val = (unsigned char) (i >> shift);
    switch (nbits) {

    case 1:
        val &= 0x1;
        return m_oneto8[val];

    case 2:
        val &= 0x3;
        return m_twoto8[val];

    case 3:
        val &= 0x7;
        return m_threeto8[val];

    default:
        return 0;
    }
}


void GOpenglDeviceWGL::CreateRGBPalette(HDC hDC)
{
    PIXELFORMATDESCRIPTOR pfd;
    LOGPALETTE *pPal;
    int n, i;
	 HPALETTE ghpalOld, ghPalette = (HPALETTE) 0;

    n = GetPixelFormat(hDC);
    DescribePixelFormat(hDC, n, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    if (pfd.dwFlags & PFD_NEED_PALETTE) {
        n = 1 << pfd.cColorBits;
        pPal = (PLOGPALETTE)LocalAlloc(LMEM_FIXED, sizeof(LOGPALETTE) +
                n * sizeof(PALETTEENTRY));
        pPal->palVersion = 0x300;
        pPal->palNumEntries = n;
        for (i=0; i<n; i++) {
            pPal->palPalEntry[i].peRed =
                    ComponentFromIndex(i, pfd.cRedBits, pfd.cRedShift);
            pPal->palPalEntry[i].peGreen =
                    ComponentFromIndex(i, pfd.cGreenBits, pfd.cGreenShift);
            pPal->palPalEntry[i].peBlue =
                    ComponentFromIndex(i, pfd.cBlueBits, pfd.cBlueShift);
            pPal->palPalEntry[i].peFlags = 0;
        }

        /* fix up the palette to include the default GDI palette */	
        if ((pfd.cColorBits == 8)                           &&
            (pfd.cRedBits   == 3) && (pfd.cRedShift   == 0) &&
            (pfd.cGreenBits == 3) && (pfd.cGreenShift == 3) &&
            (pfd.cBlueBits  == 2) && (pfd.cBlueShift  == 6)
           ) {
            for (i = 1 ; i <= 12 ; i++)
                pPal->palPalEntry[m_defaultOverride[i]] = m_defaultPalEntry[i];
        }

        ghPalette = ::CreatePalette(pPal);
        LocalFree(pPal);

        ghpalOld = ::SelectPalette(hDC, ghPalette, FALSE);
        n = RealizePalette(hDC);
		
      m_hPalette = ghPalette; // HG
		m_hOldPalette = ghpalOld; // HG
    }
} 

// HG from tk.c
void GOpenglDeviceWGL::SetOneColor(int index, float r, float g, float b)
{
    PALETTEENTRY PalEntry;
    HPALETTE Palette;
	int tkUseStaticColors = 1;

    if ( NULL != (Palette = m_hPalette) )
    {
        if ( tkUseStaticColors && ( index == 0 || index == 255 ) )
            return;

        PalEntry.peRed   = (BYTE)(r*(float)255.0 + (float)0.5);
        PalEntry.peGreen = (BYTE)(g*(float)255.0 + (float)0.5);
        PalEntry.peBlue  = (BYTE)(b*(float)255.0 + (float)0.5);
        PalEntry.peFlags = ( tkUseStaticColors ) ? PC_NOCOLLAPSE : 0;

        // This is a workaround for a GDI palette "feature".  If any of
        // the static colors are repeated in the palette, those colors
        // will map to the first occurance.  So, for our case where there
        // are only two static colors (black and white), if a white
        // color appears anywhere in the palette other than in the last
        // entry, the static white will remap to the first white.  This
        // destroys the nice one-to-one mapping we are trying to achieve.
        //
        // There are two ways to workaround this.  The first is to
        // simply not allow a pure white anywhere but in the last entry.
        // Such requests are replaced with an attenuated white of
        // (0xFE, 0xFE, 0xFE).
        //
        // The other way is to mark these extra whites with PC_RESERVED
        // which will cause GDI to skip these entries when mapping colors.
        // This way the app gets the actual colors requested, but can
        // have side effects on other apps.
        //
        // Both solutions are included below.  The PC_RESERVED solution is
        // the one currently enabled.  It may have side effects, but taking
        // over the static colors as we are is a really big side effect that
        // should swamp out the effects of using PC_RESERVED.

        if ( tkUseStaticColors )
        {
            if ( PalEntry.peRed   == 0xFF &&
                 PalEntry.peGreen == 0xFF &&
                 PalEntry.peBlue  == 0xFF )
            {
            #define USE_PC_RESERVED_WORKAROUND  1
            #if USE_PC_RESERVED_WORKAROUND
                PalEntry.peFlags |= PC_RESERVED;
            #else
                PalEntry.peRed   =
                PalEntry.peGreen =
                PalEntry.peBlue  = 0xFE;
            #endif
            }
        }

        SetPaletteEntries( Palette, index, 1, &PalEntry);

//        DelayPaletteRealization();
    }
}

///////////////////////////////////////////////////////////////////////////////////
// Win32 pixel format and buffering API wrappers

int GOpenglDeviceWGL::ChoosePixelFormat(HDC hdc, PIXELFORMATDESCRIPTOR *ppfd)
{
  ASSERT (hdc);
  ASSERT (ppfd);

  int iRet = 0;

  DWORD type = (::GetObjectType(hdc));
  if (ppfd)
    iRet = ::ChoosePixelFormat(hdc, ppfd);
  return (iRet);
}

int GOpenglDeviceWGL::DescribePixelFormat(HDC hdc, int iPixelFormat, 
                                 UINT nBytes, 
                                 PIXELFORMATDESCRIPTOR *ppfd)
{
  ASSERT (hdc);

  int iRet = 0;
  //
  //don't bother validating iPixelFormat, nBytes and ppfd as
  //it is OK to have them 0, 0, NULL when trying to get the 
  //maximum PF index
  //
  iRet = ::DescribePixelFormat(hdc, iPixelFormat, nBytes, ppfd);
  return (iRet);
}

int GOpenglDeviceWGL::GetPixelFormat(HDC hdc)
{
  ASSERT (hdc);

  int iRet = 0;

  iRet = ::GetPixelFormat(hdc);
  return (iRet);
}

BOOL GOpenglDeviceWGL::SetPixelFormat(HDC hdc, int iPixelFormat, PIXELFORMATDESCRIPTOR *ppfd)
{
  ASSERT (hdc);
  ASSERT (iPixelFormat);
  ASSERT (ppfd);

  DWORD dwErr;

  BOOL bRet = FALSE;

  if ( /*(::GetObjectType(hdc) == OBJ_DC) && */ iPixelFormat > 0 && ppfd)
    bRet = ::SetPixelFormat(hdc, iPixelFormat, ppfd);

  if (!bRet)
    dwErr = GetLastError();
  return (bRet);
}

BOOL GOpenglDeviceWGL::SwapBuffers(HDC hdc)
{
  ASSERT (hdc);

  BOOL bRet = FALSE;

  if (::GetObjectType(hdc) == OBJ_DC)
    bRet = ::SwapBuffers(hdc);
  return (bRet);
}


///////////////////////////////////////////////////////////////////////////////////
// wgl function wrappers

HGLRC GOpenglDeviceWGL::wglCreateContext(HDC hdc)
{
  ASSERT (hdc);

  HGLRC hglrc = NULL;

  hglrc = ::wglCreateContext(hdc);
  //
  //if rendering context successfully created
  //then assign to the member variable m_hglrc
  //
  if (hglrc)
    m_hglrc = hglrc;

  return (hglrc);
}

BOOL GOpenglDeviceWGL::wglDeleteContext(HGLRC hglrc)
{
  ASSERT (hglrc);

  BOOL bRet = FALSE;

  if (hglrc)
    bRet = ::wglDeleteContext(hglrc);
  return (bRet);
}

HGLRC GOpenglDeviceWGL::wglGetCurrentContext(void)
{
  return(::wglGetCurrentContext());
}

HDC GOpenglDeviceWGL::wglGetCurrentDC(void)
{
  return(::wglGetCurrentDC());
}

BOOL GOpenglDeviceWGL::wglMakeCurrent(HDC hdc, HGLRC hglrc)
{
  ASSERT(hdc >= NULL);
  ASSERT(hglrc >= NULL);

  BOOL bRet = FALSE;
  //
  //hdc and hglrc may be NULL if you are attempting
  //to make the current rc not current
  //
  bRet = ::wglMakeCurrent(hdc, hglrc);
  return (bRet);
}

BOOL GOpenglDeviceWGL::wglUseFontBitmaps(HDC hdc, DWORD first, DWORD count, DWORD listbase)
{
  ASSERT (hdc);
  ASSERT (first >= 0);
  ASSERT (count);
  ASSERT (listbase >= 0);

  BOOL bRet = FALSE;

  if ((::GetObjectType(hdc) == OBJ_DC) && first >= 0 && count > 0 && listbase >= 0)
    bRet = ::wglUseFontBitmaps(hdc, first, count, listbase);
  return (bRet);
}


//
//	Create: use PFD_DRAW_TO_BITMAP
//
BOOL GOpenglDeviceWGL::Create(CDC* m_pdc, int iPixelType, DWORD dwFlags)
{
	TRACE0("GOpenglDeviceWGL::Create (MemoryDC)\r\n") ;

#ifdef _DEBUG
	m_pdc->AssertValid() ;
#endif

   //m_hdc = pdcMemory;
   
    m_hdc = m_pdc->m_hDC;


	CBitmap* pBitmap = m_pdc->GetCurrentBitmap() ;
	BITMAP bmInfo ;
	pBitmap->GetObject(sizeof(BITMAP), &bmInfo) ;

	ASSERT(bmInfo.bmPlanes == 1) ;
	ASSERT((bmInfo.bmBitsPixel == 8) || (bmInfo.bmBitsPixel == 16) || (bmInfo.bmBitsPixel == 24)) ;		

	//
	// Fill in the Pixel Format Descriptor
	//
   PIXELFORMATDESCRIPTOR pfd ;
   memset(&pfd,0, sizeof(PIXELFORMATDESCRIPTOR)) ;
   pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);  
   pfd.nVersion = 1 ;                           // Version number
   pfd.dwFlags =  dwFlags ;
   pfd.iPixelType = iPixelType ;
   pfd.cColorBits = (BYTE)bmInfo.bmBitsPixel ;
   pfd.cDepthBits = 32;						// 32-bit depth buffer
   pfd.iLayerType = PFD_MAIN_PLANE ;           // Layer type

	// Let children change creation.
//	OnCreate(NULL, &pfd) ;

	ASSERT( (dwFlags & PFD_DRAW_TO_BITMAP)) ;
	ASSERT( !(dwFlags & PFD_DOUBLEBUFFER)) ;
	ASSERT( (iPixelType == PFD_TYPE_RGBA) || (iPixelType == PFD_TYPE_COLORINDEX)) ;

	m_bDrawToBitmap = TRUE ;

	// Determine double buffering state.
	m_bDoubleBuffer = FALSE ;


	// Chose the pixel format.
   int nPixelFormat = ::ChoosePixelFormat(m_hdc, &pfd);
   if (nPixelFormat == 0)
	{
		TRACE("ChoosePixelFormat Failed %d\r\n",GetLastError()) ;
		return FALSE ;
	}
	TRACE("Pixel Format %d\r\n", nPixelFormat) ;

	// Set the pixel format.
   BOOL bResult = ::SetPixelFormat(m_hdc, nPixelFormat, &pfd);
   if (!bResult)
	{
		TRACE("SetPixelFormat Failed %d\r\n",GetLastError()) ;
		return FALSE ;
	}

   ::DescribePixelFormat(m_hdc, nPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	// Create the palette
	//CreatePalette() ;	

	CreateRGBPalette(m_hdc);


   //
   // Create a rendering context.
   //
   m_hglrc = wglCreateContext(m_hdc);
	if (!m_hglrc)
	{
		TRACE("wglCreateContext Failed %x\r\n", GetLastError()) ;
		return FALSE;
	}

	// Window size is 0,0 here. Don't wglMakeCurrent.
	// wglMakeCurrent will be called in Resize 
	
	// A dibsection should set the color palette after this call.
	//	m_pDibSurf->SetPalette(m_pPal) ; // DIBSECTION
	//	if (m_pPal) setPaletteDIB() ;

	return TRUE;
		
}

void GOpenglDeviceWGL::MakeCurrent()
{
	ASSERT(m_hglrc) ;
	ASSERT(m_hdc) ;

#if 0
	if (m_pPal && !m_bDrawToBitmap)
	{
   		m_hdc->SelectPalette(m_pPal, 0);
   		m_hdc->RealizePalette();
	}
#endif


	if (m_hglrc != wglGetCurrentContext())
	{
		//
		// Make the rendering context m_hrc current
		//
    	BOOL bResult = wglMakeCurrent(m_hdc, m_hglrc);
		if (!bResult)
		{
			TRACE("wglMakeCurrent Failed %x\r\n", GetLastError() ) ;
			return ;
		}
	}
}

//
//
//
void GOpenglDeviceWGL::OutputGlError(char* label) 
{
	GLenum errorno = glGetError() ;
	if (errorno != GL_NO_ERROR)
		TRACE("%s had error: #(%d) %s\r\n", label, errorno, gluErrorString(errorno)) ;
}

#endif


