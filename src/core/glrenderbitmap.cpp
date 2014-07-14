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


#ifdef _OGL
#include "COpenGL.h"
#endif
//#include "dib.h"
//#include "dibsurf.h"

#include "greporter.h"

#include "gcamera.h"
#include "grender.h"

#include "gclass.h"

#include "gmodel.h"

#include "glrenderbitmap.h"


// CSimpleDIB -
//
// Implements a simple encapsulation of a DIB section and a DC.
//
class CSimpleDIB
{
	public:
		// Construction
		CSimpleDIB() ;
		virtual ~CSimpleDIB() ;

      // Create a CSimpleDIB object. Create can be called multiple times.
		BOOL Create(int cx, int cy, int ibitcount ) ;

      // Draw the DIB onto the destination DC. 
		void Draw(CDC* pdcDest, int x, int y) ;

      // Set the palette used in the DIB.
		void SetPalette(CPalette* pPal) ;

		CDC* GetDC() {return m_pdc;}
		HBITMAP GetHandle() {return m_hbmp;}

        virtual BOOL Save(const char* pszFileName = NULL);// save DIB to disk file
        virtual BOOL Save(CFile *fp);               // save to file

#if 0
       // BITMAPINFO *m_pBMI;

edef struct tagDIBSECTION { 
    BITMAP              dsBm; 
    BITMAPINFOHEADER    dsBmih; 
    DWORD               dsBitfields[3]; 
    HANDLE              dshSection; 
    DWORD               dsOffset; 
} DI
#endif
        int DibHeight() 
            {return m_dibsection.dsBmih.biHeight;}
        
        int DibWidth()
            {return m_dibsection.dsBmih.biWidth;}
        
        int StorageWidth()
        {return (((m_dibsection.dsBmih.biWidth * m_dibsection.dsBmih.biBitCount)
                     + 31) & ~31) >> 3;}




	protected:
		HBITMAP m_hbmp ;
		HBITMAP m_hbmOld;

        DIBSECTION m_dibsection; // old object information about m_hbmp, set on create 

        void* m_pBits ;		// Pointer to the bits.
		CSize m_sizeDIB ;
		CDC* m_pdc ;
	
		void destroy() ; 	// Destruction helper.
	
};


// Construction
//
CSimpleDIB::CSimpleDIB()
	: m_sizeDIB(0,0) 
{
	m_hbmp = NULL ;
	m_pBits = NULL ;
	m_pdc = NULL ;
}

//
// Destruction.
//
CSimpleDIB::~CSimpleDIB()
{
	destroy() ;
}


//
//	Destruction helper.
//
void CSimpleDIB::destroy()
{
    // Select the old bitmap back into the buffer DC.
    if (m_hbmOld)
    {
        ::SelectObject(m_pdc->GetSafeHdc(), m_hbmOld);
    }

	// Delete bitmap.
	if (m_hbmp)  {
        DeleteObject(m_hbmp);
        m_hbmp = NULL;
    }

	// Delete DC.
	if (m_pdc) { 
        delete m_pdc ;
        m_pdc = NULL;
    }
}

//
// Create a DIB section cx X cy X ibitcount.
//
BOOL CSimpleDIB::Create(int cx, int cy, int ibitcount) 
{
   ASSERT((ibitcount == 8) || (ibitcount == 16) || (ibitcount = 24))	;
   ASSERT(cx > 0);
   ASSERT(cy > 0);

   // Destroy parts of objects if we are recreating it.
   if ((m_pdc != NULL) || (m_hbmp != NULL)) destroy() ;

   // Save size for drawing later.
   m_sizeDIB.cx = cx ;
   m_sizeDIB.cy = cy ;

   // Create a BITMAPINFOHEADER structure to describe the DIB
   BITMAPINFOHEADER BIH ;
   int iSize = sizeof(BITMAPINFOHEADER) ;
   memset(&BIH, 0, iSize);

   // Fill in the header info.
   BIH.biSize = iSize;
   BIH.biWidth = cx;
   BIH.biHeight = cy;
   BIH.biPlanes = 1;
   BIH.biBitCount = ibitcount;
   BIH.biCompression = BI_RGB;

   // Create a new DC.
   m_pdc = new CDC ;
   m_pdc->CreateCompatibleDC(NULL);

   //{ return Attach(::CreateCompatibleDC(pDC->GetSafeHdc())); }

   // Create the DIB section.
   m_hbmp = CreateDIBSection( m_pdc->GetSafeHdc(),
                           	(BITMAPINFO*)&BIH,
                           	DIB_PAL_COLORS,    //	DIB_RGB_COLORS,
                            	&m_pBits,
                           	NULL,
                           	0);

    ASSERT(m_hbmp);
    ASSERT(m_pBits);

    // Select the new bitmap into the buffer DC
    if (m_hbmp)
    {
        GetObject(m_hbmp,sizeof(DIBSECTION), &m_dibsection);

        m_hbmOld = (HBITMAP)::SelectObject(m_pdc->GetSafeHdc(),m_hbmp);
        return(TRUE);
    
    } else
      return(FALSE);
     
}

//
// BitBlt DIB section to the screen. Does not mess with the palette.
//
void CSimpleDIB::Draw(CDC* pdcDest, int x, int y) 
{
    // Note: you do not need to select the palette into
    // the memory DC because the DIB section is using palette
    // index values not colors
    pdcDest->BitBlt( x, y,
                     m_sizeDIB.cx, m_sizeDIB.cy,
                     m_pdc,
                     0, 0,
                     SRCCOPY);

}

//
// Set the color table in the DIB section.
//
void CSimpleDIB::SetPalette(CPalette* pPal)
{
    ASSERT(pPal);

    // get the colors from the palette
    int iColors = 0;
    pPal->GetObject(sizeof(iColors), &iColors);
    ASSERT(iColors > 0);
    PALETTEENTRY* pPE = new PALETTEENTRY[iColors];
    pPal->GetPaletteEntries(0, iColors, pPE);

    // Build a table of RGBQUADS
    RGBQUAD* pRGB = new RGBQUAD[iColors];
    ASSERT(pRGB);
    for (int i = 0; i < iColors; i++) {
        pRGB[i].rgbRed = pPE[i].peRed;
        pRGB[i].rgbGreen = pPE[i].peGreen;
        pRGB[i].rgbBlue = pPE[i].peBlue;
        pRGB[i].rgbReserved = 0;
    }
    
    ::SetDIBColorTable(m_pdc->GetSafeHdc(),
                       0, iColors,
                       pRGB);

    delete [] pRGB;
    delete [] pPE;
}


/*
@mfunc BOOL | CDIB | Save | Save a DIB to a file.
@syntax BOOL Save(CFile *fp); 
@syntax BOOL Save(const char* pszFileName);
@parm CFile* | fp | Pointer to an open CFile object.
@parm LPSTR | pszFileName | The name of the file to save the DIB to.
@rdesc The return value is TRUE if the DIB is saved, FALSE if not.
@comm The file  in always created in the Windows DIB format.
@xref <c CDIB>
*/
// Save a DIB to a disk file
// This is somewhat simplistic because we only deal with 256 color DIBs
// and we always write a 256 color table
BOOL CSimpleDIB::Save(CFile *fp)
{
    BITMAPFILEHEADER bfh;
    int nColors = 256;
    
    RGBQUAD colors[256];


    // construct the file header
    bfh.bfType = 0x4D42; // 'BM'
    bfh.bfSize = 
        sizeof(BITMAPFILEHEADER) +
        sizeof(BITMAPINFOHEADER) +
        nColors * sizeof(RGBQUAD) +
        StorageWidth() * DibHeight();
    bfh.bfReserved1 = 0;
    bfh.bfReserved2 = 0;
    bfh.bfOffBits =
        sizeof(BITMAPFILEHEADER) +
        sizeof(BITMAPINFOHEADER) +
        nColors * sizeof(RGBQUAD);

    // write the file header
    int iSize = sizeof(bfh);
    TRY {
        fp->Write(&bfh, iSize);
    } CATCH(CFileException, e) {
        TRACE("Failed to write file header");
        return FALSE;
    } END_CATCH


#if 1
    int curColors = GetDIBColorTable(*m_pdc,0,nColors,&colors[0]);
  

    iSize =   sizeof(BITMAPINFOHEADER) ; 
    TRY {
        fp->Write(&m_dibsection.dsBmih, iSize);
    } CATCH(CFileException, e) {
        TRACE("Failed to write BITMAPINFO HEADER ");
        return FALSE;
    } END_CATCH

    iSize = nColors * sizeof(RGBQUAD) ; 
    TRY {
        fp->Write(&colors, iSize);
    } CATCH(CFileException, e) {
        TRACE("Failed to write color Table ");
        return FALSE;
    } END_CATCH


#else
    // write the BITMAPINFO
    // Note: we assume there are always 256 colors in the
    // color table
    ASSERT(m_pBMI);
    iSize = 
        sizeof(BITMAPINFOHEADER) +
        256 * sizeof(RGBQUAD);
    TRY {
        fp->Write(m_pBMI, iSize);
    } CATCH(CFileException, e) {
        TRACE("Failed to write BITMAPINFO");
        return FALSE;
    } END_CATCH
#endif


    // write the bits
    iSize = StorageWidth() * DibHeight();
    TRY {
        fp->Write(m_pBits, iSize);
    } CATCH(CFileException, e) {
        TRACE("Failed to write bits");
        return FALSE;
    } END_CATCH

    return TRUE;
}

// Save a DIB to a disk file.  If no file name is given, show
// a save file dialog to get one.
BOOL CSimpleDIB::Save(const char* pszFileName)
{
    CString strFile;    

    if ((pszFileName == NULL) 
    ||  (strlen(pszFileName) == 0)) {

        // Show a save file dialog to get the name
        CFileDialog dlg   (FALSE,   // save
                           NULL,    // no default extension
                           NULL,    // no initial file name
                           OFN_OVERWRITEPROMPT
                             | OFN_HIDEREADONLY,
                           "Image files (*.DIB, *.BMP)|*.DIB;*.BMP|All files (*.*)|*.*||");
        if (dlg.DoModal() == IDOK) {
            strFile = dlg.GetPathName();
        } else {
            return FALSE;
        }
    } else {
    
        // copy the supplied file path
        strFile = pszFileName;                    
    }

    // Try to open the file for write access
    CFile file;
    if (! file.Open(strFile,
                    CFile::modeReadWrite
                     | CFile::modeCreate
                     | CFile::shareExclusive)) {
        Reporter.Error("Failed to open bitmap file %s",(const char *)strFile);
        return FALSE;
    }

    BOOL bResult = Save(&file);
    file.Close();
    if (!bResult) Reporter.Error("Could not save bitmpa file %s",(const char *)strFile);
    return bResult;
}



// not declared in 4.1 headers ? 

// WINGDIAPI BOOL  WINAPI wglCopyContext(HGLRC,HGLRC,UINT);

// Render view to BITMAO file 
int RenderOnDib(GView	*view, const char *fileName, int cx, int cy,int depth)
{

   int ret = 0;

#ifdef _OGL
   HGLRC oldGLRC = ::wglGetCurrentContext();
   HDC   oldHDC = ::wglGetCurrentDC();

   
    GOpenglDeviceWGL context;

    CSimpleDIB m_aDIBSurface ;		// CDIBSurface from Nigel's Animation class.

   //	CSimpleDIB m_theSimpleDIB[3] ;	// DIB sections for rendering scenes.

   //	CScene m_aScene ; 				   // Scene which renders in the client area.

		// Create DIB.
		if (!m_aDIBSurface.Create(cx, cy, depth)) {
          Reporter.Error("Could not create bitmap %d * %d  * %d bits(%s) ",cx,cy,depth,fileName);
          return(-1);

        }

		CDC* pdcTemp = m_aDIBSurface.GetDC() ;

		if (!context.Create(pdcTemp)) {
		    TRACE("creating bitmap context Failed \r\n");
            Reporter.Error("Could not create bitmap context %d * %d  * %d bits(%s) ",cx,cy,depth,fileName);
            return(-1);
        }            


		// Not needed DIB is 24 bpp.
		//	pPalTemp = context.GetPalette() ;
		//	if (pPalTemp) m_theSimpleDIB.SetPalette(pPalTemp) ;


		// force a reload of textures, because textureObjects / displaylists are
		// probabybly not shareable across context
		view->renderState.ReleaseTextures();
      
#if 0
      // Function not in MSVC 4.1 Libs + not available in OGL 95  26.01.96 / Size  558 464 Ver 4.0 
        // Fails also in Winnt 3.51 with software rendering
	  if (!wglCopyContext(oldGLRC,context.m_hglrc, GL_ALL_ATTRIB_BITS)) {
		    TRACE("wglCopyContext Failed %x\r\n", GetLastError() ) ;
      }    
#endif

      // NOTE :You can only share display lists with rendering contexts within the same process. However, not all rendering contexts in a process can share display lists. Rendering contexts can share display lists if they use the same implementation of OpenGL functions only. All client rendering contexts of a given pixel format can always share display lists.

      int shareLists;  
	  if (!(shareLists = wglShareLists(oldGLRC,context.m_hglrc))){
			TRACE("wglShareLists Failed %x\r\n", GetLastError() ) ;
            // Fails on Gloria Nt 3.51 SVPCK 2
      }    

      // get old render size ==> camera aspect
      int oldSizex,oldSizey;
      oldSizex = view->renderState.sizex;
      oldSizey = view->renderState.sizey;

      context.MakeCurrent();	


      Reporter.Status("Rendering to bitmap %d * %d (%s) ...",cx,cy,fileName);

		// Set palette in DIB to palette in scene.
   //		pPalTemp = m_aSceneOnDIB.GetPalette() ;
   //		if (pPalTemp) m_aDIBSurface.SetPalette(pPalTemp) ;

		// Render scene onto DIB.
		//m_aSceneOnDIB.Resize(m_sizeBitmap.cx, m_sizeBitmap.cy) ; //Doesn't resize?!!!!
   
      GLdouble gldAspect = (GLdouble) cx/ (GLdouble) cy;
      
      glMatrixMode(GL_PROJECTION); 
      glLoadIdentity();
      glViewport(0, 0, cx, cy);
    
      
      view->SetContext(shareLists); // set all GLState variables

      //
      // Draw 
      //		
      glMatrixMode(GL_MODELVIEW);



      view->InvalidateModelCache();
      view->Resize(cx,cy);
      CRect rc(0,0,cx-1,cy-1);

      view->DrawScene(rc);

      // flush OpenGL 
      glFlush() ;
      // flush GDI
      GdiFlush() ;

      Reporter.Status("Saving bitmap %d * %d (%s) ...",cx,cy,fileName);

      if (m_aDIBSurface.Save(fileName)) {
        Reporter.Status("Bitmap %d * %d (%s) saved ",cx,cy,fileName);
        ret = 0;
      } else ret = -1;  
       



      if (!wglMakeCurrent(oldHDC,oldGLRC)) {
		  TRACE("wglMakeCurrent Failed %x\r\n", GetLastError() ) ;
      }    

  	  view->renderState.ReleaseTextures();

      // set old aspect 
      view->Resize(oldSizex,oldSizey);
      // set gl camera for picking 
#endif

      return(0);
}




#if 0
	//
	// Use Nigel's Animation Library CDIBSection
	//	
		// Create DIB.
		m_aDIBSurface.Create(m_sizeBitmap.cx, m_sizeBitmap.cy, (HPALETTE)NULL) ;	
		pdcTemp = m_aDIBSurface.GetDC() ;

		// Create Scene.
		m_aSceneOnDIB.Create(pdcTemp) ;									

		// Set palette in DIB to palette in scene.
		pPalTemp = m_aSceneOnDIB.GetPalette() ;
		if (pPalTemp) m_aDIBSurface.SetPalette(pPalTemp) ;

		// Render scene onto DIB.
		m_aSceneOnDIB.Resize(m_sizeBitmap.cx, m_sizeBitmap.cy) ; //Doesn't resize?!!!!
		m_aSceneOnDIB.Init() ;
		m_aSceneOnDIB.Render() ;

		// Draw a border around the DIB.
		DrawBlueBorder(pdcTemp) ;


	//
	// Use my CSimpleDIB to create an 8 bpp DIB section and render a CSceneDodec on it.
	//
		m_theSimpleDIB[0].Create(m_sizeBitmap.cx, m_sizeBitmap.cy, 8) ;
	 	pdcTemp = m_theSimpleDIB[0].GetDC() ;

		CSceneDodec aSceneDodec;
		aSceneDodec.Create(pdcTemp) ;

		pPalTemp = aSceneDodec.GetPalette() ;	
		if (pPalTemp) m_theSimpleDIB[0].SetPalette(pPalTemp) ;

		aSceneDodec.Resize(m_sizeBitmap.cx, m_sizeBitmap.cy) ; //Doesn't resize?!!!!
		aSceneDodec.Init() ;
		aSceneDodec.Render() ;

		DrawBlueBorder(pdcTemp) ;
	   	DrawCaption(pdcTemp, _T("8 bpp")) ;
 
	//
	// Use my CSimpleDIB to create a 16 bpp DIB section and render a Box on it.
	//

		m_theSimpleDIB[1].Create(m_sizeBitmap.cx, m_sizeBitmap.cy, 16) ;
		pdcTemp = m_theSimpleDIB[1].GetDC() ;

		CSceneBox aSceneBox ;
		aSceneBox.Create(pdcTemp) ;

		// Not needed because its a 16 bpp DIB.
		//pPalTemp = aSceneBox.GetPalette() ;
		//if (pPalTemp) m_theSimpleDIB[1].SetPalette(pPalTemp) ;

		aSceneBox.Resize(m_sizeBitmap.cx, m_sizeBitmap.cy) ; //Doesn't resize?!!!!
		aSceneBox.Init() ;
		aSceneBox.Render() ;

		DrawBlueBorder(pdcTemp) ;
	   	DrawCaption(pdcTemp, _T("16 bpp")) ;

	//
	// Use my CSimpleDIB to create a 24 bpp DIB and render a pyramid on it.
	//
		m_theSimpleDIB[2].Create(m_sizeBitmap.cx, m_sizeBitmap.cy, 24) ;
		pdcTemp = m_theSimpleDIB[2].GetDC() ;

		CScenePyramid aScenePyramid ;
		aScenePyramid.Create(pdcTemp) ;

		// Not needed DIB is 24 bpp.
		//	pPalTemp = aScenePyramid.GetPalette() ;
		//	if (pPalTemp) m_theSimpleDIB[2].SetPalette(pPalTemp) ;

		aScenePyramid.Resize(m_sizeBitmap.cx, m_sizeBitmap.cy) ; //Doesn't resize?!!!!
		aScenePyramid.Init() ;
		aScenePyramid.Render() ;

		DrawBlueBorder(pdcTemp) ;
	   	DrawCaption(pdcTemp, _T("24 bpp")) ;
#endif
