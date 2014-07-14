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
#ifndef _DIBPAL_H
#define _DIBPAL_H

class CDIB;

/////////////////////////////////////////////////////////////////////////////
// @class CDIBPal | This class enhances the CPalette class.
// @base public | CPalette
// @member Create | Create a new palette;
// @member Draw | Draw the palette to a CDC object.
// @member GetNumColors | Get the number of colors in the palette.
// @member SetSysPalColors | Create an identity palette.
// @member Load | Load a palette from a file.
// @member Save | Save the palette to a file.
// @member CreateWash | Create a color cube wash palette.
// @member CreateSystemColorPalette | Create a palette from
//         the 20 system reserved colors.

class CDIBPal : public CPalette
{
public:
    CDIBPal();
    ~CDIBPal();
    BOOL Create(CDIB *pDIB);            // create from a DIB
    BOOL Create(BITMAPINFO* pBMI);      // create from color table
    BOOL Create(RGBQUAD* pRGB, int iClrs); // create from clr table
    void Draw(CDC *pDC, CRect *pRect, BOOL bBkgnd = FALSE); 
    void Draw(HDC hDC, RECT* pRect, BOOL bBkgnd = FALSE); 
    int GetNumColors();                 // get the no. of colors in the pal.
    BOOL SetSysPalColors();

#ifdef _MMIO
    BOOL Load(const char* pszFileName = NULL);
    BOOL Load(CFile* fp);  
    BOOL Load(UINT hFile);
    BOOL Load(HMMIO hmmio);
    BOOL Save(const char* pszFileName = NULL);
    BOOL Save(CFile* fp);  
    BOOL Save(UINT hFile);
    BOOL Save(HMMIO hmmio);
#endif
    BOOL CreateWash();
    BOOL CreateSystemColorPalette();
};

#endif
