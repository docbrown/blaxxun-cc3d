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
#ifndef _DIBSURF_
#define _DIBSURF_

/////////////////////////////////////////////////////////////////////////////
// @class CDIBSurface | This class provides a drawing surface based on
//  a CDIB object which can be drawn to using GDI operations and can also 
//  be manipulated directly. Note that this object requires the target system
// to be Windows NT, Windows 95 or Win 3.1 with Win32s and WinG installed.
// @base public | CDIB
// @member Create | Create a new surface.
// @member GetDC | Get a CDC for the surface.
// @member BitBlt | Blt a section of the surface to a CDC.
// @member StretchBlt | Stretch a section of the surface to a CDC.
// @member Draw | Draw the entire surface to a DC.
// @member SetPalette | Set a new palette.

/*
@mfunc CDC* | CDIBSurface | GetDC | Get a CDC for the surface.
@syntax CDC* GetDC();
@rdesc The return value is a pointer to a CDC object which can be
used for drawing to the surface.
@xref <c CDIBSurface>
*/

// define the WinG functions
#define WINGAPI WINAPI
typedef HDC (WINGAPI WinGCreateDCProc)(void);
typedef HBITMAP (WINGAPI WinGCreateBitmapProc)(HDC WinGDC,
                                                 const BITMAPINFO* pHeader,
                                                 void** ppBits);
typedef UINT (WINGAPI WinGSetDIBColorTableProc)(HDC WinGDC,
                                                 UINT StartIndex,
                                                 UINT NumberOfEntries,
                                                 const RGBQUAD* pColors );
typedef BOOL (WINGAPI WinGBitBltProc)(HDC hdcDest,
                                       int nXOriginDest,
                                       int nYOriginDest,
                                       int nWidthDest,
                                       int nHeightDest,
                                       HDC hdcSrc,
                                       int nXOriginSrc,
                                       int nYOriginSrc );

typedef BOOL (WINGAPI WinGStretchBltProc)(HDC hdcDest,
                                       int nXOriginDest,
                                       int nYOriginDest,
                                       int nWidthDest,
                                       int nHeightDest,
                                       HDC hdcSrc,
                                       int nXOriginSrc,
                                       int nYOriginSrc,
                                       int nWidthSrc,
                                       int nHeightSrc );


class CDIBSurface : public CDIB
{
    DECLARE_SERIAL(CDIBSurface)
public:
    CDIBSurface();
    ~CDIBSurface();

// Attributes
public:

// Operations
public:
    BOOL Create(int cx, int cy,int ibitcount, CPalette* pPal = NULL);
    BOOL Create(int width, int height,int ibitcount, HPALETTE hPal); 
    void BitBlt(CDC* pDC, int xd, int yd, int w, int h,
                int xs, int ys);  
    void BitBlt(HDC hDC, int xd, int yd, int w, int h,
                int xs, int ys);  
    void StretchBlt(CDC* pDC, int xd, int yd, int wd, int hd,
                    int xs, int ys, int ws, int hs);  
    void Draw(CDC* pDC);
    void Draw(HDC hDC, int x, int y);
    void SetPalette(CPalette* pPal);
    void SetPalette(HPALETTE hPal);
    BOOL Load(const char* pszFileName = NULL); // load DIB from disk file
    BOOL MapColorsToPalette(CPalette *pPal);
    BOOL MapColorsToPalette(HPALETTE hPal);

// Implementation
protected:
#if 0
    CPalette* m_pPal;       // Palette for drawing
#else
    HPALETTE m_hPal;
#endif

private:
    static BOOL m_bUseCreateDIBSection;        // flag
    static BOOL m_bUseWinG;                    // flag
    static WinGCreateDCProc* m_pWinGCDCProc;   // ptr to WinGCreateDC()
    static WinGCreateBitmapProc* m_pWinGCBProc;// ptr to WinGCreateBitmap()
    static WinGSetDIBColorTableProc* m_pWinGSDCTProc;  // ptr to WinGSetDIBColorTable()
    static WinGBitBltProc* m_pWinGBBProc;      // ptr to WinGBitBlt()
    static WinGStretchBltProc* m_pWinGSBProc;  // ptr to WinGStretchBlt()
    HBITMAP m_hbmBuffer;        // buffer DDB
    HBITMAP m_hbmOld;           // used for storing old bitmap

    static BOOL CDIBSurface::TestPlatform();
};

#endif
