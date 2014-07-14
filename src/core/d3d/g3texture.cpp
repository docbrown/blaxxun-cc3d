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

@module G3Texture.cpp - GLView Texture support for Direct 3D |

Copyright (c) 1996-1998	by Holger Grahn
All rights reserved

Purpose:

Classes:


Notes:
	DX 3 + color key requires alpha blending
	DX 5 not
	DX5 + color key + alpha blending on RIVA 28 need sorted transparency

    alternativ check out in additon alpha compare with >0, even if no Transparency support in caps
	

Changes:

$Revision: 1.29 $
$Log: g3texture.cpp,v $
Revision 1.29  1998/07/09 15:48:25  kristof
*** empty log message ***

Revision 1.28  1998/07/06 17:51:06  holger
empty message


Todo :


******************************************************************************/



#ifdef _D3D
#include "g3render.h"
#include "gd3device.h"

#include "Image.h"
#include "IDither.h"

#include "glutils.h"


// test
//#define GAMMA(X) (X)

#define GAMMA(X) (gammatable[1][X])

byte gammatable[5][256] =
{
    {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
     17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,
     33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,
     49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,
     65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,
     81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,
     97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,
     113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,
     128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
     144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,
     160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,
     176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,
     192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,
     208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,
     224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,
     240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255},

    {2,4,5,7,8,10,11,12,14,15,16,18,19,20,21,23,24,25,26,27,29,30,31,
     32,33,34,36,37,38,39,40,41,42,44,45,46,47,48,49,50,51,52,54,55,
     56,57,58,59,60,61,62,63,64,65,66,67,69,70,71,72,73,74,75,76,77,
     78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,
     99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,
     115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,129,
     130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,
     146,147,148,148,149,150,151,152,153,154,155,156,157,158,159,160,
     161,162,163,163,164,165,166,167,168,169,170,171,172,173,174,175,
     175,176,177,178,179,180,181,182,183,184,185,186,186,187,188,189,
     190,191,192,193,194,195,196,196,197,198,199,200,201,202,203,204,
     205,205,206,207,208,209,210,211,212,213,214,214,215,216,217,218,
     219,220,221,222,222,223,224,225,226,227,228,229,230,230,231,232,
     233,234,235,236,237,237,238,239,240,241,242,243,244,245,245,246,
     247,248,249,250,251,252,252,253,254,255},

    {4,7,9,11,13,15,17,19,21,22,24,26,27,29,30,32,33,35,36,38,39,40,42,
     43,45,46,47,48,50,51,52,54,55,56,57,59,60,61,62,63,65,66,67,68,69,
     70,72,73,74,75,76,77,78,79,80,82,83,84,85,86,87,88,89,90,91,92,93,
     94,95,96,97,98,100,101,102,103,104,105,106,107,108,109,110,111,112,
     113,114,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,
     129,130,131,132,133,133,134,135,136,137,138,139,140,141,142,143,144,
     144,145,146,147,148,149,150,151,152,153,153,154,155,156,157,158,159,
     160,160,161,162,163,164,165,166,166,167,168,169,170,171,172,172,173,
     174,175,176,177,178,178,179,180,181,182,183,183,184,185,186,187,188,
     188,189,190,191,192,193,193,194,195,196,197,197,198,199,200,201,201,
     202,203,204,205,206,206,207,208,209,210,210,211,212,213,213,214,215,
     216,217,217,218,219,220,221,221,222,223,224,224,225,226,227,228,228,
     229,230,231,231,232,233,234,235,235,236,237,238,238,239,240,241,241,
     242,243,244,244,245,246,247,247,248,249,250,251,251,252,253,254,254,
     255},

    {8,12,16,19,22,24,27,29,31,34,36,38,40,41,43,45,47,49,50,52,53,55,
     57,58,60,61,63,64,65,67,68,70,71,72,74,75,76,77,79,80,81,82,84,85,
     86,87,88,90,91,92,93,94,95,96,98,99,100,101,102,103,104,105,106,107,
     108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,
     125,126,127,128,129,130,131,132,133,134,135,135,136,137,138,139,140,
     141,142,143,143,144,145,146,147,148,149,150,150,151,152,153,154,155,
     155,156,157,158,159,160,160,161,162,163,164,165,165,166,167,168,169,
     169,170,171,172,173,173,174,175,176,176,177,178,179,180,180,181,182,
     183,183,184,185,186,186,187,188,189,189,190,191,192,192,193,194,195,
     195,196,197,197,198,199,200,200,201,202,202,203,204,205,205,206,207,
     207,208,209,210,210,211,212,212,213,214,214,215,216,216,217,218,219,
     219,220,221,221,222,223,223,224,225,225,226,227,227,228,229,229,230,
     231,231,232,233,233,234,235,235,236,237,237,238,238,239,240,240,241,
     242,242,243,244,244,245,246,246,247,247,248,249,249,250,251,251,252,
     253,253,254,254,255},

    {16,23,28,32,36,39,42,45,48,50,53,55,57,60,62,64,66,68,69,71,73,75,76,
     78,80,81,83,84,86,87,89,90,92,93,94,96,97,98,100,101,102,103,105,106,
     107,108,109,110,112,113,114,115,116,117,118,119,120,121,122,123,124,
     125,126,128,128,129,130,131,132,133,134,135,136,137,138,139,140,141,
     142,143,143,144,145,146,147,148,149,150,150,151,152,153,154,155,155,
     156,157,158,159,159,160,161,162,163,163,164,165,166,166,167,168,169,
     169,170,171,172,172,173,174,175,175,176,177,177,178,179,180,180,181,
     182,182,183,184,184,185,186,187,187,188,189,189,190,191,191,192,193,
     193,194,195,195,196,196,197,198,198,199,200,200,201,202,202,203,203,
     204,205,205,206,207,207,208,208,209,210,210,211,211,212,213,213,214,
     214,215,216,216,217,217,218,219,219,220,220,221,221,222,223,223,224,
     224,225,225,226,227,227,228,228,229,229,230,230,231,232,232,233,233,
     234,234,235,235,236,236,237,237,238,239,239,240,240,241,241,242,242,
     243,243,244,244,245,245,246,246,247,247,248,248,249,249,250,250,251,
     251,252,252,253,254,254,255,255}
};

// compute the number of mip-levels required
DWORD  CommputeMipLevels(DWORD min_sizex, DWORD min_sizey, DWORD sizex, DWORD sizey)
{
	int level = 1;
	while ( (sizex > min_sizex) && (sizey > min_sizey) ) {
		sizex  = sizex / 2;
		sizey  = sizey / 2;
		level++;
	}
	return level;
}

typedef struct tagPixelConvertInfo {

	BYTE * lpStartPos;		// Start of raw pixels
	BYTE * lpCurrLine;		// Current line start pos
	BYTE * lpCurrPos;		// Current pixel pos 

	DWORD x;			// current x position
	DWORD y;			// current y position
	DWORD w;			// Width
	DWORD h;			// Height
	long  pitch;		// Actual width in video memory
	
	// Red channel Info
	DWORD rMask;		// bit mask
	DWORD rBits;		// # of bits in mask
	DWORD rShift;		// # of bits to shift down to canonical position

	// Green channel info
	DWORD gMask;		// bit mask
	DWORD gBits;		// # of bits in mask
	DWORD gShift;		// # of bits to shift down to canonical position

	// Blue channel Info
	DWORD bMask;		// bit mask
	DWORD bBits;		// # of bits in mask
	DWORD bShift;		// # of bits to shift down to canonical position

	// Alpha channel info
	DWORD aMask;		// bit mask
	DWORD aBits;		// # of bits in mask
	DWORD aShift;		// # of bits to shift down to canonical position

} PixelConvertInfo;
typedef PixelConvertInfo * LPPixelConvertInfo;

typedef void (PASCAL * LPWRITECALLBACK)(LPPixelConvertInfo ppci, DWORD toWrite);

void PASCAL Write8 (LPPixelConvertInfo ppci, DWORD dwColor);
void PASCAL Write16 (LPPixelConvertInfo ppci, DWORD dwColor);
void PASCAL Write24 (LPPixelConvertInfo ppci, DWORD dwColor);
void PASCAL Write32 (LPPixelConvertInfo ppci, DWORD dwColor);

BOOL  InitMaskInfo (LPDDSURFACEDESC pddsd, LPPixelConvertInfo ppci);
DWORD ConvertColor (LPPixelConvertInfo ppci, DWORD color);

BOOL InitMaskInfo (LPDDSURFACEDESC pddsd, LPPixelConvertInfo ppci)
{
	DWORD dwMask;
	DWORD dwBitMask;
	DWORD dwShift;
	DWORD dwBits;

	// Check Parameters
	if ((! pddsd) || (! ppci))
		return FALSE;

	// Get Red Mask info
	dwMask = pddsd->ddpfPixelFormat.dwRBitMask;
	if (0L == dwMask)
	{
		dwBits  = 0;
		dwShift = 0;
	}
	else
	{
		// Count number of bits til first bit in mask
		dwBitMask = 1;
		dwShift = 0;
		while (! (dwMask & dwBitMask))
		{
			dwBitMask <<= 1;
			dwShift++;
		}

		// Count number of bits in mask
		dwBits = 0;
		while (dwMask & dwBitMask)
		{
			dwBitMask <<= 1;
			dwBits++;
		}

		if (dwBits > 8)
			return FALSE;
	}

	ppci->rMask  = dwMask;
	ppci->rBits  = dwBits;
	ppci->rShift = dwShift;
	

	// Get Green Mask info
	dwMask = pddsd->ddpfPixelFormat.dwGBitMask;
	if (0L == dwMask)
	{
		dwBits  = 0;
		dwShift = 0;
	}
	else
	{
		// Count number of bits til first bit in mask
		dwBitMask = 1;
		dwShift = 0;
		while (! (dwMask & dwBitMask))
		{
			dwBitMask <<= 1;
			dwShift++;
		}

		// Count number of bits in mask
		dwBits = 0;
		while (dwMask & dwBitMask)
		{
			dwBitMask <<= 1;
			dwBits++;
		}

		if (dwBits > 8)
			return FALSE;
	}

	ppci->gMask  = dwMask;
	ppci->gBits  = dwBits;
	ppci->gShift = dwShift;


	// Get Blue Mask info
	dwMask = pddsd->ddpfPixelFormat.dwBBitMask;
	if (0L == dwMask)
	{
		dwBits  = 0;
		dwShift = 0;
	}
	else
	{
		// Count number of bits til first bit in mask
		dwBitMask = 1;
		dwShift = 0;
		while (! (dwMask & dwBitMask))
		{
			dwBitMask <<= 1;
			dwShift++;
		}

		// Count number of bits in mask
		dwBits = 0;
		while (dwMask & dwBitMask)
		{
			dwBitMask <<= 1;
			dwBits++;
		}

		if (dwBits > 8)
			return FALSE;
	}

	ppci->bMask  = dwMask;
	ppci->bBits  = dwBits;
	ppci->bShift = dwShift;


	// Get Alpha Mask info
	dwMask = pddsd->ddpfPixelFormat.dwRGBAlphaBitMask;
	if (0L == dwMask)
	{
		dwBits  = 0;
		dwShift = 0;
	}
	else
	{
		// Count number of bits til first bit in mask
		dwBitMask = 1;
		dwShift = 0;
		while (! (dwMask & dwBitMask))
		{
			dwBitMask <<= 1;
			dwShift++;
		}

		// Count number of bits in mask
		dwBits = 0;
		while (dwMask & dwBitMask)
		{
			dwBitMask <<= 1;
			dwBits++;
		}

		if (dwBits > 8)
			return FALSE;
	}

	ppci->aMask  = dwMask;
	ppci->aBits  = dwBits;
	ppci->aShift = dwShift;

	// Success
	return TRUE;
} // End InitConvertInfo


DWORD ConvertColor (LPPixelConvertInfo ppci, DWORD dwColor)
{
	DWORD r, g, b, a;

	// Convert Red component
	r = (dwColor >> 16) & 0xFF;			// Convert to Byte
	r >>= (DWORD)(8 - ppci->rBits);		// throw away low precision bits
	r <<= ppci->rShift;					// move to new position

	// Convert Green component
	g = (dwColor >> 8) & 0xFF;
	g >>= (DWORD)(8 - ppci->gBits);
	g <<= ppci->gShift;

	// Convert Blue component
	b = dwColor & 0xFF;
	b >>= (DWORD)(8 - ppci->bBits);
	b <<= ppci->bShift;

	// Convert Alpha component
	a = (dwColor >> 24) & 0xFF;
	a >>= (DWORD)(8 - ppci->aBits);
	a <<= ppci->aShift;

	// Return converted color
	return (r | g | b | a);
} // End ConvertColor


void PASCAL Write8 (LPPixelConvertInfo ppci, DWORD dwColor)
{
	dwColor &= 0xFF;
	*(ppci->lpCurrPos) = (BYTE)dwColor;
	ppci->lpCurrPos++;
}

void PASCAL Write16 (LPPixelConvertInfo ppci, DWORD dwColor)
{
	dwColor &= 0xFFFF;
	*((WORD *)ppci->lpCurrPos) = (WORD)dwColor;
	ppci->lpCurrPos += 2;
}

void PASCAL Write24 (LPPixelConvertInfo ppci, DWORD dwColor)
{
	BYTE * lpColor = (BYTE *)&dwColor;

	dwColor &= 0xFFFFFF;
	*(ppci->lpCurrPos++) = *lpColor++;
	*(ppci->lpCurrPos++) = *lpColor++;
	*(ppci->lpCurrPos++) = *lpColor++;
}

void PASCAL Write32 (LPPixelConvertInfo ppci, DWORD dwColor)
{
	*((DWORD *)ppci->lpCurrPos) = (WORD)dwColor;
	ppci->lpCurrPos += 4;
}



  


/*

  load a PPM RGB file into memory
  ret = 1 if ok
  <=0 on error
*/

int LoadPPMFile(const char *fileName,int &width, int &height, unsigned char * &data)
{
	char buf[256];
	int ret=0;
	size_t cnt;

    /*
     * Find the image file and open it
     */
	data = NULL;
	FILE *fp = fopen(fileName,"rb");


    if (fp == NULL) {
        TRACE("Cannot find %s.\n", fileName);
        return 0;
    }
    /*
     * Is it a PPM file?
     */
    fgets(buf, sizeof buf, fp);
    if (lstrcmp(buf, "P6\n")) {
        fclose(fp);
        TRACE("%s is not a PPM file.\n", fileName);
        return 0;
    }
    /*
     * Skip any comments
     */
    do {
        fgets(buf, sizeof buf, fp);
    } while (buf[0] == '#');
    /*
     * Read the width and height
     */
    sscanf(buf, "%d %d\n", &width, &height);
    fgets(buf, sizeof buf, fp); /* skip next line */
	if (width<1 || height <1) goto error_exit;

	cnt = width*height * 3;
	data = (unsigned char *) malloc(cnt);

	if (!data) goto error_exit;
	
	if (fread(data,1,cnt,fp) != cnt) {
		free(data);
		data = NULL;
		goto error_exit;
	}


	ret = 1;



error_exit:

	fclose(fp);
	return(ret);

}


/***************************************************************************/
/*                    Loading a PPM file into a surface                    */
/***************************************************************************/
/*
 * LoadSurface
 * Loads a ppm file into a texture map DD surface of the given format.  The
 * memory flag specifies DDSCAPS_SYSTEMMEMORY or DDSCAPS_VIDEOMEMORY.
 */
LPDIRECTDRAWSURFACE
CreateSurfaceFromImage(LPDIRECTDRAW lpDD, LPCSTR lpName, 
			LPDDSURFACEDESC lpFormat, DWORD memoryflag)
{
    LPDIRECTDRAWSURFACE lpDDS;
    DDSURFACEDESC ddsd, format;
    D3DCOLOR colors[256];
    D3DCOLOR c;
    DWORD dwWidth, dwHeight;
    int i, j;
    FILE *fp;
    char *lpC;
    CHAR buf[100];
    LPDIRECTDRAWPALETTE lpDDPal;
    PALETTEENTRY ppe[256];
    int psize;
    DWORD pcaps;
    int color_count;
    BOOL bQuant = FALSE;
    HRESULT ddrval;

    /*
     * Find the image file and open it
     */
//    fp = D3DAppIFindFile(lpName, "rb");
	fp = fopen(lpName,"rb");

    if (fp == NULL) {
        TRACE("Cannot find %s.\n", lpName);
        return NULL;
    }
    /*
     * Is it a PPM file?
     */
    fgets(buf, sizeof buf, fp);
    if (lstrcmp(buf, "P6\n")) {
        fclose(fp);
        TRACE("%s is not a PPM file.\n", lpName);
        return NULL;
    }
    /*
     * Skip any comments
     */
    do {
        fgets(buf, sizeof buf, fp);
    } while (buf[0] == '#');
    /*
     * Read the width and height
     */
    sscanf(buf, "%d %d\n", &dwWidth, &dwHeight);
    fgets(buf, sizeof buf, fp); /* skip next line */
    /*
     * Create a surface of the given format using the dimensions of the PPM
     * file.
     */
    memcpy(&format, lpFormat, sizeof(DDSURFACEDESC));
    if (format.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8) {
        bQuant = TRUE;
        psize = 256;
        pcaps = DDPCAPS_8BIT | DDPCAPS_ALLOW256;
    } else if (format.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED4) {
        bQuant = TRUE;
        psize = 16;
        pcaps = DDPCAPS_4BIT;
    }
    memcpy(&ddsd, &format, sizeof(DDSURFACEDESC));
    ddsd.dwSize = sizeof(DDSURFACEDESC);
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | memoryflag;
    ddsd.dwHeight = dwHeight;
    ddsd.dwWidth = dwWidth;

    ddrval = lpDD->CreateSurface(&ddsd, &lpDDS, NULL);
    if (ddrval != DD_OK) {
       D3DError("CreateSurface for texture failed (loadtex)",ddrval);
        return NULL;
    }
    /*
     * Lock the surface so it can be filled with the PPM file
     */
    memset(&ddsd, 0, sizeof(DDSURFACEDESC));
    ddsd.dwSize = sizeof(DDSURFACEDESC);
    ddrval = lpDDS->Lock(NULL, &ddsd, 0, NULL);
    if (ddrval != DD_OK) {
        lpDDS->Release();
       D3DError("Lock failed while loading surface (loadtex)",ddrval);
        return NULL;
    }
    /*
     * The method of loading depends on the pixel format of the dest surface
     */
    if (!bQuant) {
        /*
         * The texture surface is not palettized
         */
        //unsigned long* lpLP;
        unsigned short* lpSP;
        unsigned char* lpCP;
        unsigned long m;
        int s;
        int red_shift, red_scale;
        int green_shift, green_scale;
        int blue_shift, blue_scale;
        /*
         * Determine the red, green and blue masks' shift and scale.
         */
        for (s = 0, m = format.ddpfPixelFormat.dwRBitMask; !(m & 1);
                                                               s++, m >>= 1);
        red_shift = s;
        red_scale = 255 / (format.ddpfPixelFormat.dwRBitMask >> s);
        for (s = 0, m = format.ddpfPixelFormat.dwGBitMask; !(m & 1);
                                                               s++, m >>= 1);
        green_shift = s;
        green_scale = 255 / (format.ddpfPixelFormat.dwGBitMask >> s);
        for (s = 0, m = format.ddpfPixelFormat.dwBBitMask; !(m & 1);
                                                               s++, m >>= 1);
        blue_shift = s;
        blue_scale = 255 / (format.ddpfPixelFormat.dwBBitMask >> s);
        /*
         * Each RGB bit count requires different pointers
         */
        switch (format.ddpfPixelFormat.dwRGBBitCount) {
            case 32 :
                for (j = 0; j < (int)dwHeight; j++) {
                    /*
                     * Point to next row in texture surface
                     */
                    unsigned long* lpLP = (unsigned long*)(((char*)ddsd.lpSurface) +
                                                            ddsd.lPitch * j);
                    for (i = 0; i < (int)dwWidth; i++) {
                        int r, g, b;
                        /*
                         * Read each value, scale it and shift it into position
                         */
                        r = getc(fp) / red_scale;
                        g = getc(fp) / green_scale;
                        b = getc(fp) / blue_scale;
                        *lpLP = (r << red_shift) | (g << green_shift) |
                                (b << blue_shift);
                        lpLP++;
                    }
                }
                break;
            case 16 :
                for (j = 0; j < (int)dwHeight; j++) {
                    lpSP = (unsigned short*)(((char*)ddsd.lpSurface) +
                                                            ddsd.lPitch * j);
                    for (i = 0; i < (int)dwWidth; i++) {
                        int r, g, b;
                        r = getc(fp) / red_scale;
                        g = getc(fp) / green_scale;
                        b = getc(fp) / blue_scale;
                        *lpSP = (r << red_shift) | (g << green_shift) |
                                (b << blue_shift);
                        lpSP++;
                    }
                }
                break;
            case 8:
                for (j = 0; j < (int)dwHeight; j++) {
                    lpCP = (unsigned char*)(((char*)ddsd.lpSurface) +
                                                            ddsd.lPitch * j);
                    for (i = 0; i < (int)dwWidth; i++) {
                        int r, g, b;
                        r = getc(fp) / red_scale;
                        g = getc(fp) / green_scale;
                        b = getc(fp) / blue_scale;
                        *lpCP = (r << red_shift) | (g << green_shift) | 
                                (b << blue_shift);
                        lpCP++;
                    }
                }
                break;
            default:
                /*
                 * This wasn't a format I recognize
                 */
                lpDDS->Unlock(NULL);
                fclose(fp);
                lpDDS->Release();
                D3DError("Unknown pixel format (loadtex).",1);
                return NULL;
        }
        /*
         * Unlock the texture and return the surface pointer
         */
        lpDDS->Unlock(NULL);
        fclose(fp);
        return (lpDDS);
    }

    /*
     * We assume the 8-bit palettized case
     */
    color_count = 0;    /* number of colors in the texture */
    for (j = 0; j < (int)dwHeight; j++) {
        /*
         * Point to next row in surface
         */
        lpC = ((char*)ddsd.lpSurface) + ddsd.lPitch * j;
        for (i = 0; i < (int)dwWidth; i++) {
            int r, g, b, k;
            /*
             * Get the next red, green and blue values and turn them into a
             * D3DCOLOR
             */
            r = getc(fp);
            g = getc(fp);
            b = getc(fp);
            c = RGB_MAKE(r, g, b);
            /*
             * Search for this color in a table of colors in this texture
             */
            for (k = 0; k < color_count; k++)
                if (c == colors[k]) break;
            if (k == color_count) {
                /*
                 * This is a new color, so add it to the list
                 */
                color_count++;
                /*
                 * More than 256 and we fail (8-bit) 
                 */
                if (color_count > psize) {
                    color_count--;
                    k = color_count - 1;
                    //goto burst_colors;
                }
                colors[k] = c;
            }
            /*
             * Set the "pixel" value on the surface to be the index into the
             * color table
             */
            if (psize == 16) {
                if ((i & 1) == 0)
                    *lpC = k & 0xf;
                else {
                    *lpC |= (k & 0xf) << 4;
                    lpC++;
                }
            } else {
                *lpC = (char)k;
                lpC++;
            }
        }
    }
    /*
     * Close the file and unlock the surface
     */
    fclose(fp);
    lpDDS->Unlock(NULL);

//burst_colors:
    if (color_count > psize) {
        /*
         * If there are more than 256 colors, we overran our palette
         */
        lpDDS->Unlock(NULL);
        lpDDS->Release();
        D3DError("Palette burst. (loadtex)",1);
        return (NULL);
    }

    /*
     * Create a palette with the colors in our color table
     */
    memset(ppe, 0, sizeof(PALETTEENTRY) * 256);
    for (i = 0; i < color_count; i++) {
        ppe[i].peRed = GAMMA((unsigned char)RGB_GETRED(colors[i]));
        ppe[i].peGreen = GAMMA((unsigned char)RGB_GETGREEN(colors[i]));
        ppe[i].peBlue = GAMMA((unsigned char)RGB_GETBLUE(colors[i]));
    }
    /*
     * Set all remaining entry flags to D3DPAL_RESERVED, which are ignored by
     * the renderer.
     */
    for (; i < 256; i++)
        ppe[i].peFlags = D3DPAL_RESERVED;
    /*
     * Create the palette with the DDPCAPS_ALLOW256 flag because we want to
     * have access to all entries.
     */
    ddrval = lpDD->CreatePalette(        DDPCAPS_INITIALIZE | pcaps,
                                         ppe, &lpDDPal, NULL);
    if (ddrval != DD_OK) {
        lpDDS->Release();
       D3DError("Create palette failed while loading surface (loadtex)",(ddrval));
        return (NULL);
    }
    /*
     * Finally, bind the palette to the surface
     */
    ddrval = lpDDS->SetPalette(lpDDPal);
    if (ddrval != DD_OK) {
        lpDDS->Release();
        lpDDPal->Release();
       D3DError("SetPalette failed while loading surface (loadtex).",(ddrval));
        return (NULL);
    }

    lpDDPal->Release();

    return lpDDS;
}

/***************************************************************************/
/*                    Loading a PPM file into a surface                    */
/***************************************************************************/

BOOL
LoadSurfaceImageData(LPDIRECTDRAWSURFACE lpDDS,
					 LPDIRECTDRAWSURFACE4 lpDDS4,
 							int fmt,
							int width, int height, const unsigned char * data,
							int ncolors, const irgb_color *colorTable,
							DDSURFACEDESC &format, int colorKeyIndex,BOOL updatePalette = FALSE,LPDIRECTDRAWPALETTE lpDDPal=NULL)
{


    D3DCOLOR colors[256];
    D3DCOLOR c;
    DWORD dwWidth, dwHeight;
    int i, j;
    char *lpC;

    
    PALETTEENTRY ppe[256];
    int psize;
    DWORD pcaps;
    int color_count;
    BOOL bQuant = FALSE;
	BOOL bAlpha = FALSE;

    HRESULT ddrval;


	dwWidth = width;
	dwHeight = height;

    LPVOID		lpSurface;		// pointer to the associated surface memory
    LONG		lPitch;			// distance to start of next line (return value only)



	if (lpDDS4) { 
		DDSURFACEDESC2 ddsd2;
		ddsd2.dwSize = sizeof(DDSURFACEDESC2);
		ddrval = lpDDS4->Lock(NULL, &ddsd2, DDLOCK_WRITEONLY , NULL); 
		lpSurface = ddsd2.lpSurface; 
		lPitch = ddsd2.lPitch; 
		
	}
	else  {
		DDSURFACEDESC ddsd;
		ddsd.dwSize = sizeof(DDSURFACEDESC);
		ddrval = lpDDS->Lock(NULL, &ddsd, 0, NULL);
		lpSurface = ddsd.lpSurface; 
		lPitch = ddsd.lPitch; 

	}		
    
	if (ddrval != DD_OK) {

        D3DError("Lock failed while locking surface .",(ddrval));
        return FALSE;
    }
    if (format.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8) {
        bQuant = TRUE;
        psize = 256;
        pcaps = DDPCAPS_8BIT | DDPCAPS_ALLOW256;
    } else if (format.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED4) {
        bQuant = TRUE;
        psize = 16;
        pcaps = DDPCAPS_4BIT;
    }


	const unsigned char* datap = data; // step along pixels 


    /*
     * The method of loading depends on the pixel format of the dest surface
     */
    if (!bQuant) {

        /*
         * The texture surface is not palettized
         */
        unsigned long* lpLP;
        unsigned short* lpSP;
        unsigned char* lpCP;
        unsigned long m;

        int s;
        int red_shift, red_scale;
        int green_shift, green_scale;
        int blue_shift, blue_scale;
        int alpha_shift=0, alpha_scale=1;
        /*
         * Determine the red, green and blue masks' shift and scale.
         */
        for (s = 0, m = format.ddpfPixelFormat.dwRBitMask; !(m & 1);  s++, m >>= 1);
        red_shift = s;
        red_scale = 255 / (format.ddpfPixelFormat.dwRBitMask >> s);
        for (s = 0, m = format.ddpfPixelFormat.dwGBitMask; !(m & 1);
                                                               s++, m >>= 1);
        green_shift = s;
        green_scale = 255 / (format.ddpfPixelFormat.dwGBitMask >> s);
        for (s = 0, m = format.ddpfPixelFormat.dwBBitMask; !(m & 1);
                                                               s++, m >>= 1);
        blue_shift = s;
        blue_scale = 255 / (format.ddpfPixelFormat.dwBBitMask >> s);

		if (fmt == IM_RGBA) {		// IMAGE assume is RGBA, texture has also a
			if (format.ddpfPixelFormat.dwRGBAlphaBitMask !=0) {
				for (s = 0, m = format.ddpfPixelFormat.dwRGBAlphaBitMask; !(m & 1);  s++, m >>= 1);
				alpha_shift = s;
				alpha_scale = 255 / (format.ddpfPixelFormat.dwRGBAlphaBitMask >> s);
			}
		#define GETPIX(r,g,b,a)  { r = *datap++; g = *datap++; b = *datap++; a = *datap++; }

        /*
         * Each RGB bit count requires different pointers
         */
        switch (format.ddpfPixelFormat.dwRGBBitCount) {
            case 32 :
                for (j = 0; j < (int)dwHeight; j++) {
                    /*
                     * Point to next row in texture surface
                     */
                    lpLP = (unsigned long*)(((char*)lpSurface) +  lPitch * j);
                    for (i = 0; i < (int)dwWidth; i++) {
                        int r, g, b,a;
                        /*
                         * Read each value, scale it and shift it into position
                         */
						GETPIX(r,g,b,a);
                        r /= red_scale,  g /= green_scale,  b /= blue_scale,  a /= alpha_scale;
                        *lpLP = (r << red_shift) | (g << green_shift) | (b << blue_shift) | (a << alpha_shift);
                        lpLP++;
                    }
                }
                break;
            case 16 :
                for (j = 0; j < (int)dwHeight; j++) {
                    lpSP = (unsigned short*)(((char*)lpSurface) + lPitch * j);
                    for (i = 0; i < (int)dwWidth; i++) {
                        int r, g, b, a;
						GETPIX(r,g,b,a);
                        r /= red_scale, g /= green_scale, b /= blue_scale,  a /= alpha_scale;

                        *lpSP = (r << red_shift) | (g << green_shift) | (b << blue_shift) | (a << alpha_shift);
                        lpSP++;
                    }
                }
                break;
            case 8:
                for (j = 0; j < (int)dwHeight; j++) {
                    lpCP = (unsigned char*)(((char*)lpSurface) + lPitch * j);
                    for (i = 0; i < (int)dwWidth; i++) {
                        int r, g, b, a;
						GETPIX(r,g,b,a);
                        r /= red_scale, g /= green_scale, b /= blue_scale,  a /= alpha_scale;
                        *lpCP = (r << red_shift) | (g << green_shift) | (b << blue_shift) | (a << alpha_shift);
                        lpCP++;
                    }
                }
                break;
            default:
                /*
                 * This wasn't a format I recognize
                 */
				if (lpDDS4) lpDDS4->Unlock(NULL); else lpDDS->Unlock(NULL);
                D3DError("Unknown pixel format (loadtex).",1);
                return FALSE;
        }
#undef GETPIX


		}

		else { // RGB 

		
		ASSERT(fmt == IM_RGB24);

#define GETPIX(r,g,b)  { r = *datap++; g = *datap++; b = *datap++; }

        /*
         * Each RGB bit count requires different pointers
         */
        switch (format.ddpfPixelFormat.dwRGBBitCount) {
            case 32 :
                for (j = 0; j < (int)dwHeight; j++) {
                    /*
                     * Point to next row in texture surface
                     */
                    lpLP = (unsigned long*)(((char*)lpSurface) +  lPitch * j);
                    for (i = 0; i < (int)dwWidth; i++) {
                        int r, g, b;
                        /*
                         * Read each value, scale it and shift it into position
                         */
						GETPIX(r,g,b);
                        r /= red_scale,  g /= green_scale,  b /= blue_scale;
                        *lpLP = (r << red_shift) | (g << green_shift) | (b << blue_shift);
                        lpLP++;
                    }
                }
                break;
			case 24 :	// new 20.06.98
                for (j = 0; j < (int)dwHeight; j++) {
                    /*
                     * Point to next row in texture surface
                     */
                    IBYTE *lpLP = (IBYTE *)(((char*)lpSurface) +  lPitch * j);
                    for (i = 0; i < (int)dwWidth; i++) {
                        int r, g, b;
                        /*
                         * Read each value, scale it and shift it into position
                         */
						GETPIX(r,g,b);
                        r /= red_scale,  g /= green_scale,  b /= blue_scale;
                        *lpLP++=r; // assume RGB 
                        *lpLP++=g;
                        *lpLP++=b;
                    }
                }
				break;

            case 16 :
                for (j = 0; j < (int)dwHeight; j++) {
                    lpSP = (unsigned short*)(((char*)lpSurface) +  lPitch * j);
                    for (i = 0; i < (int)dwWidth; i++) {
                        int r, g, b;
						GETPIX(r,g,b);
                        r /= red_scale, g /= green_scale, b /= blue_scale;
                        *lpSP = (r << red_shift) | (g << green_shift) | (b << blue_shift);
                        lpSP++;
                    }
                }
                break;
            case 8:
                for (j = 0; j < (int)dwHeight; j++) {
                    lpCP = (unsigned char*)(((char*)lpSurface) + lPitch * j);
                    for (i = 0; i < (int)dwWidth; i++) {
                        int r, g, b;
						GETPIX(r,g,b);
                        r /= red_scale, g /= green_scale, b /= blue_scale;
                        *lpCP = (r << red_shift) | (g << green_shift) | (b << blue_shift);
                        lpCP++;
                    }
                }
                break;
            default:
                /*
                 * This wasn't a format I recognize
                 */
				if (lpDDS4) lpDDS4->Unlock(NULL); else lpDDS->Unlock(NULL);
                D3DError("Unknown pixel format (loadtex).",1);
                return FALSE;
        }
#undef GETPIX
		} // RGB
        /*
         * Unlock the texture and return the surface pointer
         */
		if (lpDDS4) lpDDS4->Unlock(NULL); else lpDDS->Unlock(NULL);
        return (TRUE);
    }

    /*
     * We assume the 8-bit palettized case
     */


	if ((fmt == IM_MAPPED8) || (fmt == IM_GRAY)) { // we already have a palette !!!!!!!!!!!!!!
		if (psize == 256) { // 8-Bit case
			for (j = 0; j < (int)dwHeight; j++) {
				/*
				* Point to next row in surface
				*/
				lpC = ((char*)lpSurface) + lPitch * j;
				memcpy(lpC,datap,dwWidth);
				datap += dwWidth;
			}
		} else 
		for (j = 0; j < (int)dwHeight; j++) {
			/*
			* Point to next row in surface
			*/
			lpC = ((char*)lpSurface) + lPitch * j;
			for (i = 0; i < (int)dwWidth; i++) {

			// get source pixel
			unsigned char k =  *datap ++;
            /*
             * Set the "pixel" value on the surface to be the index into the
             * color table
             */
            if (psize == 16) {
                if ((i & 1) == 0)
                    *lpC = k & 0xf;
                else {
                    *lpC |= (k & 0xf) << 4;
                    lpC++;
                }
            } else {
                *lpC = (char)k;
                lpC++;
            }


			} // col
		} // row

		/*
		* Unlock the surface
		*/
		if (lpDDS4) lpDDS4->Unlock(NULL); else lpDDS->Unlock(NULL);

		color_count = min(256,ncolors);

		if (updatePalette) {
    /*
     * Create a palette with the colors in our color table	  image
     */										
    memset(ppe, 0, sizeof(PALETTEENTRY) * 256);
	if (colorTable) {
		for (i = 0; i < color_count; i++) {
			ppe[i].peRed = GAMMA((unsigned char)(colorTable[i].r));
			ppe[i].peGreen = GAMMA((unsigned char)(colorTable[i].g));
			ppe[i].peBlue = GAMMA((unsigned char)(colorTable[i].b));
		}
	} else {
		color_count = 256;
		for (i = 0; i < color_count; i++) {
			ppe[i].peRed = GAMMA((unsigned char) i);
			ppe[i].peGreen = GAMMA((unsigned char) i);
			ppe[i].peBlue = GAMMA((unsigned char) i);
		}

	}
    /*
     * Set all remaining entry flags to D3DPAL_RESERVED, which are ignored by
     * the renderer.
     */
    for (; i < 256; i++)
      ppe[i].peFlags = D3DPAL_RESERVED;

	if (colorKeyIndex >=0) {
		//ppe[colorKeyIndex].peFlags = D3DPAL_READONLY;
	}
    /*
     * Create the palette with the DDPCAPS_ALLOW256 flag because we want to
     * have access to all entries.
     */
    //XXXX ddrval = lpDD->CreatePalette(        DDPCAPS_INITIALIZE | pcaps,
    //                                     ppe, &lpDDPal, NULL);
	ddrval = DD_OK;    
	if (ddrval != DD_OK) {
        D3DError("Create palette failed while loading surface (loadtex).",(ddrval));
        return (FALSE);
    }
    /*
     * Finally, bind the palette to the surface
     */
	if (lpDDS4)  ddrval = lpDDS4->SetPalette(lpDDPal);
    else ddrval = lpDDS->SetPalette(lpDDPal);

    if (ddrval != DD_OK) {
        // lpDDPal->Release();
        D3DError("SetPalette failed while loading surface (loadtex).",(ddrval));
        return (FALSE);
    }

    //xx lpDDPal->Release();
	
		
	} // palette
    return TRUE;



	} // 8-Bit

	// last change RGB ==> 8
	if (updatePalette) {
        D3DError("Can't burst here.",(ddrval));
 
		return FALSE;
	}	
	// format =RGB, quantize
	ASSERT(fmt == IM_RGB24);
	#define GETPIX(r,g,b)  { r = *datap++; g = *datap++; b = *datap++; }


    color_count = 0;    /* number of colors in the texture */
    for (j = 0; j < (int)dwHeight; j++) {
        /*
         * Point to next row in surface
         */
        lpC = ((char*)lpSurface) + lPitch * j;
        for (i = 0; i < (int)dwWidth; i++) {
            int r, g, b, k;
            /*
             * Get the next red, green and blue values and turn them into a
             * D3DCOLOR
             */
			GETPIX(r,g,b);

            c = RGB_MAKE(r, g, b);
            /*
             * Search for this color in a table of colors in this texture
             */
            for (k = 0; k < color_count; k++)
                if (c == colors[k]) break; // bad performance hg ????

            if (k == color_count) {
                /*
                 * This is a new color, so add it to the list
                 */
                color_count++;
                /*
                 * More than 256 and we fail (8-bit) 
                 */
                if (color_count > psize) {
                    color_count--;
                    k = color_count - 1;  // or find nearest color 
                    //goto burst_colors;
                }
                colors[k] = c;
            }
            /*
             * Set the "pixel" value on the surface to be the index into the
             * color table
             */
            if (psize == 16) {
                if ((i & 1) == 0)
                    *lpC = k & 0xf;
                else {
                    *lpC |= (k & 0xf) << 4;
                    lpC++;
                }
            } else {
                *lpC = (char)k;
                lpC++;
            }
        }
    }
	#undef GETPIX

    /*
     * Unlock the surface
     */
    lpDDS->Unlock(NULL);

//burst_colors:
    if (color_count > psize) {
        /*
         * If there are more than 256 colors, we overran our palette
         */
        lpDDS->Unlock(NULL);
        D3DError("Palette burst. (loadtex).",1);
        return (FALSE);
    }

    /*
     * Create a palette with the colors in our color table
     */
    memset(ppe, 0, sizeof(PALETTEENTRY) * 256);
    for (i = 0; i < color_count; i++) {
        ppe[i].peRed = GAMMA((unsigned char)RGB_GETRED(colors[i]));
        ppe[i].peGreen = GAMMA((unsigned char)RGB_GETGREEN(colors[i]));
        ppe[i].peBlue = GAMMA((unsigned char)RGB_GETBLUE(colors[i]));
    }
    /*
     * Set all remaining entry flags to D3DPAL_RESERVED, which are ignored by
     * the renderer.
     */
    for (; i < 256; i++)
        ppe[i].peFlags = D3DPAL_RESERVED;
    /*
     * Create the palette with the DDPCAPS_ALLOW256 flag because we want to
     * have access to all entries.
     */
    //xxxx ddrval = lpDD->CreatePalette(        DDPCAPS_INITIALIZE | pcaps,
    //                                     ppe, &lpDDPal, NULL);
	ddrval = DD_OK;
    if (ddrval != DD_OK) {
        D3DError("Create palette failed while loading surface (loadtex).",(ddrval));
        return (FALSE);
    }
    /*
     * Finally, bind the palette to the surface
     */
    ddrval = lpDDS->SetPalette(lpDDPal);
    if (ddrval != DD_OK) {
        //xxx lpDDPal->Release();
        D3DError("SetPalette failed while loading surface (loadtex).",(ddrval));
        return (FALSE);
    }

    // lpDDPal->Release();

    return TRUE;

}


/*
 * CreateSurfaceFromImageData
 * Loads image data into a texture map DD surface of the given format.  The
 * memory flag specifies DDSCAPS_SYSTEMMEMORY or DDSCAPS_VIDEOMEMORY.
 */
LPDIRECTDRAWSURFACE
CreateSurfaceFromImageData(LPDIRECTDRAW lpDD, 
							int fmt,
							int width, int height, const unsigned char * data,
							int ncolors, const irgb_color *colorTable,
							LPDDSURFACEDESC lpFormat, DWORD memoryflag,
							int colorKeyIndex,irgb_pixel colorKeyRgb,
							DDCOLORKEY &ckey,int mipLevels)
{
    LPDIRECTDRAWSURFACE lpDDS;
    DDSURFACEDESC ddsd, format;
    D3DCOLOR colors[256];
    D3DCOLOR c;
    DWORD dwWidth, dwHeight;
    int i, j;
    char *lpC;
    LPDIRECTDRAWPALETTE lpDDPal;
    PALETTEENTRY ppe[256];
    int psize;
    DWORD pcaps;
    int color_count;
    BOOL bQuant = FALSE;
	BOOL bAlpha = FALSE;

    HRESULT ddrval;


	dwWidth = width;
	dwHeight = height;
    /*
     * Create a surface of the given format using the dimensions of the PPM
     * file.
     */
    memcpy(&format, lpFormat, sizeof(DDSURFACEDESC));
    if (format.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8) {
        bQuant = TRUE;
        psize = 256;
        pcaps = DDPCAPS_8BIT | DDPCAPS_ALLOW256;
    } else if (format.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED4) {
        bQuant = TRUE;
        psize = 16;
        pcaps = DDPCAPS_4BIT;
    }
    memcpy(&ddsd, &format, sizeof(DDSURFACEDESC));
    ddsd.dwSize = sizeof(DDSURFACEDESC);

    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | memoryflag;
    ddsd.dwHeight = dwHeight;
    ddsd.dwWidth = dwWidth;

	if (mipLevels>1) {
			ddsd.dwFlags |= DDSD_MIPMAPCOUNT ;
			ddsd.dwMipMapCount = mipLevels;
			ddsd.ddsCaps.dwCaps |= DDSCAPS_MIPMAP | DDSCAPS_COMPLEX;
	}
    
	ddrval = lpDD->CreateSurface(&ddsd, &lpDDS, NULL);
    if (ddrval != DD_OK) {
		D3DError("CreateSurface for texture failed (loadtex).",(ddrval));

        return NULL;
    }
    /*
     * Lock the surface so it can be filled with the data
     */
    memset(&ddsd, 0, sizeof(DDSURFACEDESC));
    ddsd.dwSize = sizeof(DDSURFACEDESC);

	//LoadSurfaceData

    ddrval = lpDDS->Lock(NULL, &ddsd, 0, NULL);
    if (ddrval != DD_OK) {
        lpDDS->Release();
        D3DError("Lock failed while loading surface (loadtex).",(ddrval));
        return NULL;
    }

	const unsigned char* datap = data; // step along pixels 


    /*
     * The method of loading depends on the pixel format of the dest surface
     */
    if (!bQuant) {

        /*
         * The texture surface is not palettized
         */
        unsigned long* lpLP;
        unsigned short* lpSP;
        unsigned char* lpCP;
        unsigned long m;

        int s;
        int red_shift, red_scale;
        int green_shift, green_scale;
        int blue_shift, blue_scale;
        int alpha_shift=0, alpha_scale=1;
        /*
         * Determine the red, green and blue masks' shift and scale.
         */
        for (s = 0, m = format.ddpfPixelFormat.dwRBitMask; !(m & 1);  s++, m >>= 1);
        red_shift = s;
        red_scale = 255 / (format.ddpfPixelFormat.dwRBitMask >> s);
        for (s = 0, m = format.ddpfPixelFormat.dwGBitMask; !(m & 1);
                                                               s++, m >>= 1);
        green_shift = s;
        green_scale = 255 / (format.ddpfPixelFormat.dwGBitMask >> s);
        for (s = 0, m = format.ddpfPixelFormat.dwBBitMask; !(m & 1);
                                                               s++, m >>= 1);
        blue_shift = s;
        blue_scale = 255 / (format.ddpfPixelFormat.dwBBitMask >> s);

		if (fmt == IM_RGBA) {		// IMAGE assume is RGBA, texture has also a
			if (format.ddpfPixelFormat.dwRGBAlphaBitMask !=0) {
				for (s = 0, m = format.ddpfPixelFormat.dwRGBAlphaBitMask; !(m & 1);  s++, m >>= 1);
				alpha_shift = s;
				alpha_scale = 255 / (format.ddpfPixelFormat.dwRGBAlphaBitMask >> s);
			}
		#define GETPIX(r,g,b,a)  { r = *datap++; g = *datap++; b = *datap++; a = *datap++; }

        /*
         * Each RGB bit count requires different pointers
         */
        switch (format.ddpfPixelFormat.dwRGBBitCount) {
            case 32 :
                for (j = 0; j < (int)dwHeight; j++) {
                    /*
                     * Point to next row in texture surface
                     */
                    unsigned long* lpLP = (unsigned long*)(((char*)ddsd.lpSurface) +  ddsd.lPitch * j);
                    for (i = 0; i < (int)dwWidth; i++) {
                        int r, g, b,a;
                        /*
                         * Read each value, scale it and shift it into position
                         */
						GETPIX(r,g,b,a);
                        r /= red_scale,  g /= green_scale,  b /= blue_scale,  a /= alpha_scale;
                        *lpLP = (r << red_shift) | (g << green_shift) | (b << blue_shift) | (a << alpha_shift);
                        lpLP++;
                    }
                }

				{ int r, g, b;
                r = colorKeyRgb.r / red_scale, g =  colorKeyRgb.g / green_scale, b =  colorKeyRgb.b /blue_scale;
                ckey.dwColorSpaceLowValue = 
				ckey.dwColorSpaceHighValue  = (r << red_shift) | (g << green_shift) | (b << blue_shift);
				}

                break;
            case 16 :
                for (j = 0; j < (int)dwHeight; j++) {
                    lpSP = (unsigned short*)(((char*)ddsd.lpSurface) +  ddsd.lPitch * j);
                    for (i = 0; i < (int)dwWidth; i++) {
                        int r, g, b, a;
						GETPIX(r,g,b,a);
                        r /= red_scale, g /= green_scale, b /= blue_scale,  a /= alpha_scale;

                        *lpSP = (r << red_shift) | (g << green_shift) | (b << blue_shift) | (a << alpha_shift);
                        lpSP++;
                    }
                }

				{ int r, g, b;
                r = colorKeyRgb.r / red_scale, g =  colorKeyRgb.g / green_scale, b =  colorKeyRgb.b /blue_scale;
                ckey.dwColorSpaceLowValue = 
				ckey.dwColorSpaceHighValue  = (r << red_shift) | (g << green_shift) | (b << blue_shift);
				}

                break;
            case 8:
                for (j = 0; j < (int)dwHeight; j++) {
                    lpCP = (unsigned char*)(((char*)ddsd.lpSurface) + ddsd.lPitch * j);
                    for (i = 0; i < (int)dwWidth; i++) {
                        int r, g, b, a;
						GETPIX(r,g,b,a);
                        r /= red_scale, g /= green_scale, b /= blue_scale,  a /= alpha_scale;
                        *lpCP = (r << red_shift) | (g << green_shift) | (b << blue_shift) | (a << alpha_shift);
                        lpCP++;
                    }
                }

				{ int r, g, b;
                r = colorKeyRgb.r / red_scale, g =  colorKeyRgb.g / green_scale, b =  colorKeyRgb.b /blue_scale;
                ckey.dwColorSpaceLowValue = 
				ckey.dwColorSpaceHighValue  = (r << red_shift) | (g << green_shift) | (b << blue_shift);
				}

                break;
            default:
                /*
                 * This wasn't a format I recognize
                 */
                lpDDS->Unlock(NULL);
                lpDDS->Release();
                D3DError("Unknown pixel format (loadtex).",1);
                return NULL;
        }
#undef GETPIX


		}

		else { // RGB 

		
		ASSERT(fmt == IM_RGB24);

#define GETPIX(r,g,b)  { r = *datap++; g = *datap++; b = *datap++; }

        /*
         * Each RGB bit count requires different pointers
         */
        switch (format.ddpfPixelFormat.dwRGBBitCount) {
            case 32 :
                for (j = 0; j < (int)dwHeight; j++) {
                    /*
                     * Point to next row in texture surface
                     */
                    unsigned long* lpLP = (unsigned long*)(((char*)ddsd.lpSurface) +  ddsd.lPitch * j);
                    for (i = 0; i < (int)dwWidth; i++) {
                        int r, g, b;
                        /*
                         * Read each value, scale it and shift it into position
                         */
						GETPIX(r,g,b);
                        r /= red_scale,  g /= green_scale,  b /= blue_scale;
                        *lpLP = (r << red_shift) | (g << green_shift) | (b << blue_shift);
                        lpLP++;
                    }
                }

				{ int r, g, b;
                r = colorKeyRgb.r / red_scale, g =  colorKeyRgb.g / green_scale, b =  colorKeyRgb.b /blue_scale;
                ckey.dwColorSpaceLowValue = 
				ckey.dwColorSpaceHighValue  = (r << red_shift) | (g << green_shift) | (b << blue_shift);
				}

                break;
			case 24 :	// new 20.06.98
                for (j = 0; j < (int)dwHeight; j++) {
                    /*
                     * Point to next row in texture surface
                     */
                    IBYTE *lpLP = (IBYTE *) (((char*)ddsd.lpSurface) +  ddsd.lPitch * j);
                    for (i = 0; i < (int)dwWidth; i++) {
                        int r, g, b;
                        /*
                         * Read each value, scale it and shift it into position
                         */
						GETPIX(r,g,b);
                        r /= red_scale,  g /= green_scale,  b /= blue_scale;
                        *lpLP++=r; // assume RGB 
                        *lpLP++=g;
                        *lpLP++=b;
                    }
                }

				{ int r, g, b;
                r = colorKeyRgb.r / red_scale, g =  colorKeyRgb.g / green_scale, b =  colorKeyRgb.b /blue_scale;
                ckey.dwColorSpaceLowValue = 
				ckey.dwColorSpaceHighValue  = (r << red_shift) | (g << green_shift) | (b << blue_shift);
				}

                break;

            case 16 :
                for (j = 0; j < (int)dwHeight; j++) {
                    lpSP = (unsigned short*)(((char*)ddsd.lpSurface) +  ddsd.lPitch * j);
                    for (i = 0; i < (int)dwWidth; i++) {
                        int r, g, b;
						GETPIX(r,g,b);
                        r /= red_scale, g /= green_scale, b /= blue_scale;
                        *lpSP = (r << red_shift) | (g << green_shift) | (b << blue_shift);
                        lpSP++;
                    }
                }

				{ int r, g, b;
                r = colorKeyRgb.r / red_scale, g =  colorKeyRgb.g / green_scale, b =  colorKeyRgb.b /blue_scale;
                ckey.dwColorSpaceLowValue = 
				ckey.dwColorSpaceHighValue  = (r << red_shift) | (g << green_shift) | (b << blue_shift);
				}

                break;
            case 8:
                for (j = 0; j < (int)dwHeight; j++) {
                    lpCP = (unsigned char*)(((char*)ddsd.lpSurface) +
                                                            ddsd.lPitch * j);
                    for (i = 0; i < (int)dwWidth; i++) {
                        int r, g, b;
						GETPIX(r,g,b);
                        r /= red_scale, g /= green_scale, b /= blue_scale;
                        *lpCP = (r << red_shift) | (g << green_shift) | (b << blue_shift);
                        lpCP++;
                    }
                }
				{ int r, g, b;
                r = colorKeyRgb.r / red_scale, g =  colorKeyRgb.g / green_scale, b =  colorKeyRgb.b /blue_scale;
                ckey.dwColorSpaceLowValue = 
				ckey.dwColorSpaceHighValue  = (r << red_shift) | (g << green_shift) | (b << blue_shift);
				}

                break;
            default:
                /*
                 * This wasn't a format I recognize
                 */
                lpDDS->Unlock(NULL);
                lpDDS->Release();
                D3DError("Unknown pixel format (loadtex).",1);
                return NULL;
        }
#undef GETPIX
		} // RGB
        /*
         * Unlock the texture and return the surface pointer
         */
        lpDDS->Unlock(NULL);
        return (lpDDS);
    }

    /*
     * We assume the
		else {		// scale all other image to sam 8-bit palettized case
     */


	if ((fmt == IM_MAPPED8) || (fmt == IM_GRAY)) { // we already have a palette !!!!!!!!!!!!!!
		if (psize == 256) { // 8-Bit case
			if (ddsd.lPitch == (dwWidth)) {
				lpC = ((char*)ddsd.lpSurface);
				memcpy(lpC,datap,dwWidth*dwHeight*1);
			}
			else for (j = 0; j < (int)dwHeight; j++) {
				/*
				* Point to next row in surface
				*/
				lpC = ((char*)ddsd.lpSurface) + ddsd.lPitch * j;
				memcpy(lpC,datap,dwWidth);
				datap += dwWidth;
			}
		} else 
		for (j = 0; j < (int)dwHeight; j++) {
			/*
			* Point to next row in surface
			*/
			lpC = ((char*)ddsd.lpSurface) + ddsd.lPitch * j;
			for (i = 0; i < (int)dwWidth; i++) {

			// get source pixel

			unsigned char k =  *datap ++;
            /*
             * Set the "pixel" value on the surface to be the index into the
             * color table
             */
            if (psize == 16) {
                if ((i & 1) == 0)
                    *lpC = k & 0xf;
                else {
                    *lpC |= (k & 0xf) << 4;
                    lpC++;
                }
            } else {
                *lpC = (char)k;
                lpC++;
            }


			} // col
		} // row

		/*
		* Unlock the surface
		*/
		lpDDS->Unlock(NULL);

		color_count = min(256,ncolors);
		ckey.dwColorSpaceLowValue = 
		ckey.dwColorSpaceHighValue = colorKeyIndex;

    /*
     * Create a palette with the colors in our color table	  image
     */										
    memset(ppe, 0, sizeof(PALETTEENTRY) * 256);
	if (colorTable) {
		for (i = 0; i < color_count; i++) {
			ppe[i].peRed = GAMMA((unsigned char)(colorTable[i].r));
			ppe[i].peGreen = GAMMA((unsigned char)(colorTable[i].g));
			ppe[i].peBlue = GAMMA((unsigned char)(colorTable[i].b));
		}
	} else {
		color_count = 256;
		for (i = 0; i < color_count; i++) {
			ppe[i].peRed = GAMMA((unsigned char) i);
			ppe[i].peGreen = GAMMA((unsigned char) i);
			ppe[i].peBlue = GAMMA((unsigned char) i);
		}

	}
    /*
     * Set all remaining entry flags to D3DPAL_RESERVED, which are ignored by
     * the renderer.
     */
    for (; i < 256; i++)
      ppe[i].peFlags = D3DPAL_RESERVED;

	if (colorKeyIndex >=0) {
		//ppe[colorKeyIndex].peFlags = D3DPAL_READONLY;
	}
    /*
     * Create the palette with the DDPCAPS_ALLOW256 flag because we want to
     * have access to all entries.
     */
    ddrval = lpDD->CreatePalette(        DDPCAPS_INITIALIZE | pcaps,
                                         ppe, &lpDDPal, NULL);
    if (ddrval != DD_OK) {
        lpDDS->Release();
        D3DError("Create palette failed while loading surface (loadtex).",(ddrval));
        return (NULL);
    }
    /*
     * Finally, bind the palette to the surface
     */
    ddrval = lpDDS->SetPalette(lpDDPal);
    if (ddrval != DD_OK) {
        lpDDS->Release();
        lpDDPal->Release();
        D3DError("SetPalette failed while loading surface (loadtex).",(ddrval));
        return (NULL);
    }

    lpDDPal->Release();

    return lpDDS;



	}

	// format =RGB, quantize
	ASSERT(fmt == IM_RGB24);
	#define GETPIX(r,g,b)  { r = *datap++; g = *datap++; b = *datap++; }


    color_count = 0;    /* number of colors in the texture */
    for (j = 0; j < (int)dwHeight; j++) {
        /*
         * Point to next row in surface
         */
        lpC = ((char*)ddsd.lpSurface) + ddsd.lPitch * j;
        for (i = 0; i < (int)dwWidth; i++) {
            int r, g, b, k;
            /*
             * Get the next red, green and blue values and turn them into a
             * D3DCOLOR
             */
			GETPIX(r,g,b);

            c = RGB_MAKE(r, g, b);
            /*
             * Search for this color in a table of colors in this texture
             */
            for (k = 0; k < color_count; k++)
                if (c == colors[k]) break; // bad performance hg ????

            if (k == color_count) {
                /*
                 * This is a new color, so add it to the list
                 */
                color_count++;
                /*
                 * More than 256 and we fail (8-bit) 
                 */
                if (color_count > psize) {
                    color_count--;
                    k = color_count - 1;  // or find nearest color 
                    //goto burst_colors;
                }
                colors[k] = c;
            }
            /*
             * Set the "pixel" value on the surface to be the index into the
             * color table
             */
            if (psize == 16) {
                if ((i & 1) == 0)
                    *lpC = k & 0xf;
                else {
                    *lpC |= (k & 0xf) << 4;
                    lpC++;
                }
            } else {
                *lpC = (char)k;
                lpC++;
            }
        }
    }
	#undef GETPIX

    /*
     * Unlock the surface
     */
    lpDDS->Unlock(NULL);

//burst_colors:
    if (color_count > psize) {
        /*
         * If there are more than 256 colors, we overran our palette
         */
        lpDDS->Unlock(NULL);
        lpDDS->Release();
        D3DError("Palette burst. (loadtex).",1);
        return (NULL);
    }

    /*
     * Create a palette with the colors in our color table
     */
    memset(ppe, 0, sizeof(PALETTEENTRY) * 256);
    for (i = 0; i < color_count; i++) {
        ppe[i].peRed = GAMMA((unsigned char)RGB_GETRED(colors[i]));
        ppe[i].peGreen = GAMMA((unsigned char)RGB_GETGREEN(colors[i]));
        ppe[i].peBlue = GAMMA((unsigned char)RGB_GETBLUE(colors[i]));
    }
    /*
     * Set all remaining entry flags to D3DPAL_RESERVED, which are ignored by
     * the renderer.
     */
    for (; i < 256; i++)
        ppe[i].peFlags = D3DPAL_RESERVED;
    /*
     * Create the palette with the DDPCAPS_ALLOW256 flag because we want to
     * have access to all entries.
     */
    ddrval = lpDD->CreatePalette(        DDPCAPS_INITIALIZE | pcaps,
                                         ppe, &lpDDPal, NULL);
    if (ddrval != DD_OK) {
        lpDDS->Release();
        D3DError("Create palette failed while loading surface (loadtex).",(ddrval));
        return (NULL);
    }
    /*
     * Finally, bind the palette to the surface
     */
    ddrval = lpDDS->SetPalette(lpDDPal);
    if (ddrval != DD_OK) {
        lpDDS->Release();
        lpDDPal->Release();
        D3DError("SetPalette failed while loading surface (loadtex).",(ddrval));
        return (NULL);
    }

    lpDDPal->Release();

    return lpDDS;
}



/***************************************************************************/
/*
 * LoadTextureSurf
 * Creates a texture map surface and texture object from the numbered PPM
 * file.  This is done in a two step process.  A source texture surface and
 * object are created in system memory.  A second, initially empty, texture
 * surface is created (in video memory if hardware is present).  The source
 * texture is loaded into the destination texture surface and then discarded.
 * This process allows a device to compress or reformat a texture map as it
 * enters video memory during the Load call.
 */
gbool
G3TextureHandle::LoadTextureSurf(GD3Device *device,RenderState &state,image &im)
{
    DDSURFACEDESC ddsd;
    LPDIRECTDRAWSURFACE lpSrcTextureSurf = NULL;
    LPDIRECT3DTEXTURE lpSrcTexture = NULL;
    LPDIRECTDRAWPALETTE lpDstPalette = NULL;
    PALETTEENTRY ppe[256];
	DDCOLORKEY ckey;	// Color key, for GIF transparent index 
	BOOL hasCkey = FALSE;
	int targetFormat = -1; // choosen texture format 

    DWORD pcaps;
	gbool mipMap = FALSE;
	DWORD mipLevels = 1;

/*

	if (state.canDoMipMap) { // debug test 
		mipMap = state.textureParameters.NeedMipmaps();
	}
	// for now mip map here not supported, need to load mip levels
*/


	int ret;

Retry:


    /*
     * Release the surface if it is hanging around
     */
	//xx new n ew Release();

    RELEASE(lpTextureSurf);
	hTexture = 0;
	hasAlpha = 0;
	hasColorKey = 0;
	isGrayscale = 0;
	targetFormat = -1; // choosen texture format 

    /*
     * Create a surface in system memory and load the Image file into it.
     * Query for the texture interface.
     */
	{	
		


		BOOL needPalette;

		if (im.image) {
			
			cannotLoad=0;

			//  get best texture format for image 

			if (state.rgbFormat>=0) targetFormat = state.rgbFormat;
			else if (state.palette8Format>=0) targetFormat = state.palette8Format;
			else targetFormat = device->CurrTextureFormat;

			if (!device->TextureFormat[device->CurrTextureFormat].bPalettized) {
				targetFormat = device->CurrTextureFormat; // let user choice ???
			}

			switch (im.fmt) {
			case IM_BW:
			case IM_GRAY:
			case IM_MAPPED4:
			case IM_MAPPED8: // choose a pal 8 format
					if (state.palette8Format>=0) {
						targetFormat = state.palette8Format;
					}
					else if (state.rgba16_1Format>=0 && im.colorKey) {
						ret=image_convert_to_rgba(&im);
						if (ret>=0) {
							targetFormat = state.rgba16_1Format;
							break;
						}
					}


					break;
			case IM_GRAYA:
					if (state.rgbaFormat>=0) {
						ret=image_convert_to_rgba(&im);
						if (ret>=0) {
							targetFormat = state.rgbaFormat;
							break;
						}
					}
					if (state.palette8Format>=0) 
						targetFormat = state.palette8Format;
					break;
			case IM_RGBA:
					if (state.rgbaFormat>=0) 
						targetFormat = state.rgbaFormat;
					break;
			default :
				break;

			}

			// dither RGB to palette 8
			if (device->TextureFormat[targetFormat].bPalettized) {
				needPalette = TRUE;
				/* convert RGB image to FS dither mapped image */
				if (im.fmt != IM_MAPPED8 ) { 
//					if (im.fmt == IM_RGB && (im.width <512))
//						ret = image_scale(&im, min(512,im.width*2),min(512,im.height*2));

					ret = image_convert_image_mapped8(&im);
				}		
				// to do : mapped 4
			} else {
				
				if (im.fmt == IM_RGBA && (device->TextureFormat[targetFormat].AlphaBPP >0)) {
					// Alpha supported
					hasAlpha = TRUE;
				} // for now, convert everything else to RGB 
				else 
				if (im.fmt != IM_RGB) {
						ret=image_convert_to_rgb(&im);
				}
			}

			TRACE("Texture %s %d %d %d  Format = %d \n",fileName,im.width,im.height,im.ncolors,targetFormat);

			if (mipMap) {
				mipLevels = CommputeMipLevels(state.textureParameters.min_size_x,state.textureParameters.min_size_y,
					sizeX,sizeY);

				if (mipLevels <=1) mipMap = FALSE;
			}

		    lpSrcTextureSurf = CreateSurfaceFromImageData(
									device->lpDD, 
									im.fmt,im.width,im.height,im.image,
									im.ncolors,im.cmap,	
                                    &device->TextureFormat[targetFormat].ddsd,
									DDSCAPS_SYSTEMMEMORY,
									im.colorKeyIndex,im.colorKeyRgb,ckey,mipLevels
									);
			
			
			if (lpSrcTextureSurf && im.colorKey) {	
#if 0				
				if (device->TextureFormat[targetFormat].bPalettized) {
					ckey.dwColorSpaceLowValue = 
					ckey.dwColorSpaceHighValue = im.colorKeyIndex;
				} else {
					ckey.dwColorSpaceLowValue = D3DRGB(im.colorKeyRgb.r,im.colorKeyRgb.g,im.colorKeyRgb.b); 
/*
				// to do: get right color 
					if (im.ncolors >0 && im.cmap)  {
						irgb_color &c= im.cmap[im.backgroundIndex];
						ckey.dwColorSpaceLowValue = D3DRGB(c.r,c.g,c.b);
					}
				    else ckey.dwColorSpaceLowValue = D3DRGB(0,0,0);
*/
					ckey.dwColorSpaceHighValue = ckey.dwColorSpaceLowValue;
				}
#endif



				state.GLastError = lpSrcTextureSurf->SetColorKey(DDCKEY_SRCBLT ,&ckey); 
				if (state.GLastError!=D3D_OK) 
					D3DError("SetColorKey failed",state.GLastError);
				else ;;

				
				TRACE("Color key %d  BG index %d \n",im.colorKeyIndex, im.backgroundIndex );

				hasCkey = TRUE;
				hasColorKey = 1;
				if (!state.dx5)  hasAlpha = 1; // DX3 must enable  alpha blending for colorKey 

			}
			isGrayscale = image_is_gray(&im);
			// ?????????????????????????ßß image_free_all(&im);

		}
		else  cannotLoad = 1;

	}

	this->textureFormat = targetFormat; // the texture format

    if (!lpSrcTextureSurf)
        goto exit_with_error;
    
	state.GLastError = lpSrcTextureSurf->QueryInterface(IID_IDirect3DTexture,
                                             (LPVOID*)&lpSrcTexture);
    if (state.GLastError != DD_OK) {
        D3DError("Failed to obtain D3D texture interface for a source texture.", (state.GLastError));
        goto exit_with_error;
    }
    /*
     * Create an empty texture surface to load the source texture into.
     * The DDSCAPS_ALLOCONLOAD flag allows the DD driver to wait until the
     * load call to allocate the texture in memory because at this point,
     * we may not know how much memory the texture will take up (e.g. it
     * could be compressed to an unknown size in video memory).
     */
	state.GLastError = device->GetSurfDesc(&ddsd, lpSrcTextureSurf);

    if (state.GLastError != DD_OK) {
        D3DError("Could not get the surface description of the source texture.",(state.GLastError));
        goto exit_with_error;
    }
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_ALLOCONLOAD;

	// DDSCAPS_MIPMAP 
	// DDSCAPS_MODEX 
	// DDSCAPS_ALPHA 


	if (lpTextureSurf != NULL) { // new 
		state.GLastError = DD_OK;
	}
    else     
		state.GLastError = device->CreateSurface(&ddsd, &lpTextureSurf);
    
	if (state.GLastError != DD_OK) {
        D3DError("Could not create the destination texture surface.",(state.GLastError));
        goto exit_with_error;
    }
    if (ddsd.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8) {
        pcaps = DDPCAPS_8BIT | DDPCAPS_ALLOW256;
    } else if (ddsd.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED4) {
        pcaps = DDPCAPS_4BIT;
    } else {
        pcaps = 0;
    }

    if (pcaps) {
        memset(ppe, 0, sizeof(PALETTEENTRY) * 256);

		//if (hasCkey) 
		//	ppe[ckey.dwColorSpaceLowValue].peFlags = D3DPAL_READONLY;

        state.GLastError = device->lpDD->CreatePalette(pcaps,ppe, &lpDstPalette, NULL);
        if (state.GLastError != DD_OK) {
            D3DError("Failed to create a palette for the destination texture.",(state.GLastError));
            goto exit_with_error;
        }
        state.GLastError = lpTextureSurf->SetPalette(lpDstPalette);

        if (state.GLastError != DD_OK) {
            D3DError("Failed to set the destination texture's palette",(state.GLastError));
            goto exit_with_error;
        }
        
	    RELEASE(lpDstPalette); // hg wasn´t deleted 
    }

	if (hasCkey) {
		state.GLastError = lpTextureSurf->SetColorKey(DDCKEY_SRCBLT ,&ckey); 
		if (state.GLastError!=D3D_OK) {
			D3DError("SetColorKey on Texture failed",state.GLastError);
		}
		hasColorKey = 1;
		if (!state.dx5)  hasAlpha = 1; // DX3 must enable  alpha blending for colorKey 

	}

    /*
     * Query our destination surface for a texture interface
     */
    state.GLastError = lpTextureSurf->QueryInterface(IID_IDirect3DTexture,
                                             (LPVOID*)&lpTexture);
    if (state.GLastError != DD_OK) {
        D3DError("Failed to obtain D3D texture interface for a destination texture.",(state.GLastError));
        goto exit_with_error;
    }
    /*
     * Load the source texture into the destination.  During this call, a
     * driver could compress or reformat the texture surface and put it in
     * video memory.
     */
    state.GLastError = lpTexture->Load(lpSrcTexture);
    if (state.GLastError != DD_OK) {
        D3DError("Could not load a source texture into a destination texture.",(state.GLastError));
        goto exit_with_error;
    }


    /* 
     * Now we are done with the source texture
     */
    RELEASE(lpSrcTexture);
    RELEASE(lpSrcTextureSurf);

    /*
     * Did the texture end up in video memory?
     */
    state.GLastError = device->GetSurfDesc(&ddsd, lpTextureSurf);
    if (state.GLastError != DD_OK) {
        D3DError("Could not get the surface description of the loaded texture surface.",(state.GLastError));
        goto exit_with_error;
    }

	inVideoMem = TRUE;

    if (!(ddsd.ddsCaps.dwCaps & DDSCAPS_VIDEOMEMORY)) {
		//CString msg;
		//msg.Format("2 Im  Texture not in video %X ",ddsd.ddsCaps.dwCaps,device->ThisDriver.bIsHardware); // hg debug
		//D3DError(msg,-1);
		TRACE("2 Im  Texture not in video %X %d",ddsd.ddsCaps.dwCaps,device->ThisDriver.bIsHardware); // hg debug

		if (device->ThisDriver.bIsHardware  && !(ddsd.ddsCaps.dwCaps & DDSCAPS_NONLOCALVIDMEM)) {
			inVideoMem = FALSE;
			state.GLastError = lpTexture->Unload();
		    RELEASE(lpTexture);
			hTexture = 0;
			if (state.OnTextureMemoryOverflow(*this)) {
				goto Retry;
			}
			//device->bTexturesInVideo = FALSE;
			return TRUE;
		}
	}


    state.GLastError = lpTexture->GetHandle(device->lpD3DDevice, &hTexture);

    if (state.GLastError != DD_OK) {
        D3DError("Could not get a handle to loaded texture",(state.GLastError));
        goto exit_with_error;
    }
    
    
    return TRUE;

exit_with_error:
    RELEASE(lpSrcTexture);
    RELEASE(lpSrcTextureSurf);
    RELEASE(lpDstPalette);
    RELEASE(lpTexture);
    RELEASE(lpTextureSurf);
	cannotLoad = 1; // new 18.05.99 hg 

    return FALSE;       
}

/* decode direct Draw surface
   and allocate and copy pixels to image im
*/

gbool ImageSet(image &im, 
			   LPDIRECTDRAWSURFACE lpDDS,
			   IDirectDrawPalette *lpDDP,
			   DDSURFACEDESC &format, 
			   RECT &srcRect)
{
    HRESULT ddrval;
    DWORD i,j;

	gbool ret = FALSE;
	DWORD dwWidth,dwHeight;
	DDSURFACEDESC ddsd;

    memset(&ddsd, 0, sizeof(DDSURFACEDESC));
	ddsd.dwSize = sizeof(DDSURFACEDESC);
    
	ddrval = lpDDS->Lock(NULL, &ddsd, 0, NULL);
    
	if (ddrval != DD_OK) {

        D3DError("Lock failed while locking surface .",(ddrval));
        return FALSE;
    }
	dwWidth = ddsd.dwWidth;
	dwHeight = ddsd.dwHeight;

    if (ddsd.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8 ) {
				IBYTE *dest;

				ret = image_alloc(&im,IM_MAPPED8,dwWidth,dwHeight)>=0;
				if (!ret) goto Exit;
				dest = IROW(&im,0);
				if (lpDDP) {
					PALETTEENTRY ppe[256];
					memset(ppe, 0, sizeof(PALETTEENTRY) * 256);
					int numColors = 256;
					ddrval = lpDDP->GetEntries(0,0,numColors,ppe);
					if (ddrval != DD_OK) {
						D3DError("Can't get palette",ddrval);
					}
					ret= image_alloc_cmap(&im, numColors)>=0;
					for (int i = 0; i < numColors; i++) {
						image_set_cmap(&im,i,ppe[i].peRed,ppe[i].peGreen,ppe[i].peBlue);
					}
				}

                for (j = 0; j < dwHeight; j++) {
                    /*
                     * Point to next row in texture surface
                     */
                    unsigned char* lpSP = (unsigned char*)(((char*)ddsd.lpSurface) +  ddsd.lPitch * j);
					dest = IROW(&im,dwHeight-1-j);
#if 1
					memcpy(dest,lpSP,dwWidth);
#else
                    for (i = 0; i < dwWidth; i++) {
						unsigned char p= *lpSP++;
						*dest++ = p;
                    }
#endif
                }
				ret = TRUE;

	}
	else 


    if (ddsd.ddpfPixelFormat.dwFlags & DDPF_RGB ) {
		IBYTE *dest;
        int red_shift, red_scale;
        int green_shift, green_scale;
        int blue_shift, blue_scale;
        int alpha_shift=0, alpha_scale=1;
		int s,m;
        /*
         * Determine the red, green and blue masks' shift and scale.
         */
        for (s = 0, m = ddsd.ddpfPixelFormat.dwRBitMask; !(m & 1);  s++, m >>= 1);
        red_shift = s;
        red_scale = 255 / (ddsd.ddpfPixelFormat.dwRBitMask >> s);
        for (s = 0, m = ddsd.ddpfPixelFormat.dwGBitMask; !(m & 1);
                                                               s++, m >>= 1);
        green_shift = s;
        green_scale = 255 / (ddsd.ddpfPixelFormat.dwGBitMask >> s);
        for (s = 0, m = ddsd.ddpfPixelFormat.dwBBitMask; !(m & 1);
                                                               s++, m >>= 1);
        blue_shift = s;
        blue_scale = 255 / (ddsd.ddpfPixelFormat.dwBBitMask >> s);

			if (ddsd.ddpfPixelFormat.dwRGBAlphaBitMask !=0) {
				for (s = 0, m = ddsd.ddpfPixelFormat.dwRGBAlphaBitMask; !(m & 1);  s++, m >>= 1);
				alpha_shift = s;
				alpha_scale = 255 / (ddsd.ddpfPixelFormat.dwRGBAlphaBitMask >> s);
			}

        /*
         * Each RGB bit count requires different pointers
         */
        switch (ddsd.ddpfPixelFormat.dwRGBBitCount) {
            case 24 :
				ret = image_alloc(&im,IM_RGB,dwWidth,dwHeight)>=0;
				if (!ret) goto Exit;
				dest = IROW(&im,0);

                for (j = 0; j < (int)dwHeight; j++) {
                    /*
                     * Point to next row in texture surface
                     */
                    IBYTE *lpLP = (((IBYTE*)ddsd.lpSurface) +  ddsd.lPitch * j);
					dest = IROW(&im,dwHeight-1-j);
                    for (i = 0; i < dwWidth; i++) {
						*dest++ = *lpLP++;
						*dest++ = *lpLP++;
						*dest++ = *lpLP++;
                    }
                }
				ret = TRUE;
                break;
            case 32 :
				ret = image_alloc(&im,IM_RGB,dwWidth,dwHeight)>=0;
				if (!ret) goto Exit;
				dest = IROW(&im,0);

                for (j = 0; j < dwHeight; j++) {
                    /*
                     * Point to next row in texture surface
                     */
                    unsigned long* lpSP = (unsigned long*)(((char*)ddsd.lpSurface) +  ddsd.lPitch * j);
					dest = IROW(&im,dwHeight-1-j);
                    for (i = 0; i < dwWidth; i++) {
                        IBYTE r, g, b;
						unsigned long p= *lpSP++;
						r =  ((p & ddsd.ddpfPixelFormat.dwRBitMask) >> red_shift)*red_scale;
						g =  ((p & ddsd.ddpfPixelFormat.dwGBitMask) >> green_shift)*green_scale;
						b =  ((p & ddsd.ddpfPixelFormat.dwBBitMask) >> blue_shift)*blue_scale;

						*dest++ = r;
						*dest++ = g;
						*dest++ = b;
                    }
                }
				ret = TRUE;
                break;
            case 16 :
				ret = image_alloc(&im,IM_RGB,dwWidth,dwHeight)>=0;
				if (!ret) goto Exit;
				dest = IROW(&im,0);

                for (j = 0; j < dwHeight; j++) {
                    /*
                     * Point to next row in texture surface
                     */
                    unsigned short* lpSP = (unsigned short*)(((char*)ddsd.lpSurface) +  ddsd.lPitch * j);
					dest = IROW(&im,dwHeight-1-j);
                    for (i = 0; i < dwWidth; i++) {
                        IBYTE r, g, b;
						unsigned short p= *lpSP++;
						r =  ((p & ddsd.ddpfPixelFormat.dwRBitMask) >> red_shift)*red_scale;
						g =  ((p & ddsd.ddpfPixelFormat.dwGBitMask) >> green_shift)*green_scale;
						b =  ((p & ddsd.ddpfPixelFormat.dwBBitMask) >> blue_shift)*blue_scale;

						*dest++ = r;
						*dest++ = g;
						*dest++ = b;
                    }
                }
				ret = TRUE;
                break;
            case 8 :
				ret = image_alloc(&im,IM_RGB,dwWidth,dwHeight)>=0;
				if (!ret) goto Exit;
				dest = IROW(&im,0);

                for (j = 0; j < dwHeight; j++) {
                    /*
                     * Point to next row in texture surface
                     */
                    unsigned char* lpSP = (unsigned char*)(((char*)ddsd.lpSurface) +  ddsd.lPitch * j);
					dest = IROW(&im,dwHeight-1-j);
                    for (i = 0; i < dwWidth; i++) {
                        IBYTE r, g, b;
						unsigned char p= *lpSP++;
						r =  ((p & ddsd.ddpfPixelFormat.dwRBitMask) >> red_shift)*red_scale;
						g =  ((p & ddsd.ddpfPixelFormat.dwGBitMask) >> green_shift)*green_scale;
						b =  ((p & ddsd.ddpfPixelFormat.dwBBitMask) >> blue_shift)*blue_scale;

						*dest++ = r;
						*dest++ = g;
						*dest++ = b;
                    }
                }
				ret = TRUE;
                break;
            default:
                /*
                 * This wasn't a format I recognize
                 */
                lpDDS->Unlock(NULL);
                D3DError("Unknown pixel format (loadtex).",1);
                return FALSE;
        }

	}
Exit:
    lpDDS->Unlock(NULL);
	return ret;

}

gbool
G3TextureHandle::LoadTextureSurf(GD3Device *device,RenderState &state,
								 LPDIRECTDRAWSURFACE srcSurface,IDirectDrawPalette *srcPalette,
								 DDSURFACEDESC &srcDdsd, RECT &srcRect,RECT *targetSizeRect)
{
    DDSURFACEDESC ddsd;
    LPDIRECTDRAWSURFACE lpSrcTextureSurf = NULL;
    LPDIRECT3DTEXTURE lpSrcTexture = NULL;
    LPDIRECTDRAWPALETTE lpDstPalette = NULL;
    PALETTEENTRY ppe[256];
	DDCOLORKEY ckey;	// Color key, for GIF transparent index 
	BOOL hasCkey = FALSE;
	int targetFormat = -1; // choosen texture format 

    DWORD pcaps;

	int ret;

	if (0) {
	  ret = 0;	
      image im;
	  image_zero(&im);
	  if (ImageSet(im, srcSurface,srcPalette,srcDdsd,srcRect)) {
		  //ret = LoadTextureSurf(device,state,im);
		 ret =Set(state,&im,TRUE,TRUE,FALSE); // use quick scaling 
	  }	
	  image_free_all(&im);
	  return ret>=0;
	}


Retry:


    /*
     * Release the surface if it is hanging around
     */
	//xx new n ew Release();
/*
    RELEASE(lpTextureSurf);
	hTexture = 0;
	hasAlpha = 0;
	hasColorKey = 0;
	isGrayscale = 0;
	targetFormat = -1; // choosen texture format 
*/
    /*
     * Create a surface in system memory and load the Image file into it.
     * Query for the texture interface.
     */
	{	
		


		gbool needPalette = FALSE;
		int dwWidth;
		int dwHeight;


		if (srcSurface) {
			
			cannotLoad=0;

			if (lpSrcTextureSurf != NULL) { // take old one 
				state.GLastError = DD_OK;
				dwWidth=sizeX;
				dwHeight=sizeY;
			} 
			else 
			{
			dwWidth = srcRect.right-srcRect.left;
			dwHeight= srcRect.bottom-srcRect.top;


			if (targetSizeRect && targetSizeRect->right>0 && targetSizeRect->bottom >0 ) {
				 dwWidth = targetSizeRect->right-targetSizeRect->left;
				 dwHeight= targetSizeRect->bottom-targetSizeRect->top;
			}
			else {
				// map to nearest 2 power 
				get_image_2power(dwWidth,dwHeight,
					state.textureParameters.min_size_x,state.textureParameters.min_size_y,
					state.textureParameters.current_max_size_x,state.textureParameters.current_max_size_y,state.textureParameters.square_size);
			}

			//dwWidth = 128;
			//dwHeight = 128;


			//  get best texture format for image 

			if (state.rgbFormat>=0) targetFormat = state.rgbFormat;
			else if (state.palette8Format>=0) targetFormat = state.palette8Format;
			else targetFormat = device->CurrTextureFormat;

			TRACE("Texture %d %d  Format = %d \n",srcDdsd.dwWidth,srcDdsd.dwHeight,targetFormat);
			
			// start with TextureFormat
			//memcpy(&ddsd, &device->TextureFormat[targetFormat].ddsd, sizeof(DDSURFACEDESC));

			memcpy(&ddsd, &srcDdsd, sizeof(DDSURFACEDESC));
			hasAlpha = (ddsd.ddpfPixelFormat.dwRGBAlphaBitMask != 0);

		    ddsd.dwSize = sizeof(DDSURFACEDESC);
			ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
			ddsd.ddsCaps.dwCaps =  DDSCAPS_TEXTURE | DDSCAPS_SYSTEMMEMORY;
			//ddsd.ddsCaps.dwCaps =  DDSCAPS_OFFSCREENPLAIN;
			ddsd.dwHeight = dwHeight;
			ddsd.dwWidth = dwWidth;

			state.GLastError = device->lpDD->CreateSurface(&ddsd, &lpSrcTextureSurf, NULL);

			if (state.GLastError != DD_OK) {
				D3DError("CreateSurface for texture failed (loadtex).",(state.GLastError));
				cannotLoad = 1;
				// return NULL;
			} else
			if (srcPalette) {
				    state.GLastError =lpSrcTextureSurf->SetPalette(srcPalette);
			        if (state.GLastError != DD_OK) {
						D3DError("Failed to set palette for texture.",(state.GLastError));
						goto exit_with_error;
					}

			}
			sizeX=dwWidth;
			sizeY=dwHeight;

			}

			if (lpSrcTextureSurf) {
				RECT destRect;
				destRect.left = destRect.top = 0;
				destRect.right = dwWidth;
				destRect.bottom = dwHeight;
				if (srcPalette) {
				    state.GLastError =lpSrcTextureSurf->SetPalette(srcPalette);
			        if (state.GLastError != DD_OK) {
						D3DError("Failed to set palette for texture.",(state.GLastError));
						goto exit_with_error;
					}

				}
				DDBLTFX bltFx;
			    memset(&bltFx, 0, sizeof(DDBLTFX));
				bltFx.dwSize = sizeof(DDBLTFX);
				bltFx.dwDDFX = DDBLTFX_MIRRORUPDOWN; 

				state.GLastError = lpSrcTextureSurf->Blt(&destRect, srcSurface, &srcRect, DDBLT_WAIT | DDBLT_DDFX , &bltFx);
				// Action not supported
				if (state.GLastError != DD_OK) {
					D3DError("Blt to texture surface failed",(state.GLastError));
					// return NULL;
					cannotLoad = 1;			
				}
			}


		}
		else  cannotLoad = 1;

	}

	this->textureFormat = targetFormat; // the texture format

    if (!lpSrcTextureSurf)
        goto exit_with_error;
    
	state.GLastError = lpSrcTextureSurf->QueryInterface(IID_IDirect3DTexture,
                                             (LPVOID*)&lpSrcTexture);
    if (state.GLastError != DD_OK) {
        D3DError("Failed to obtain D3D texture interface for a source texture.", (state.GLastError));
        goto exit_with_error;
    }
    /*
     * Create an empty texture surface to load the source texture into.
     * The DDSCAPS_ALLOCONLOAD flag allows the DD driver to wait until the
     * load call to allocate the texture in memory because at this point,
     * we may not know how much memory the texture will take up (e.g. it
     * could be compressed to an unknown size in video memory).
     */
	state.GLastError = device->GetSurfDesc(&ddsd, lpSrcTextureSurf);

    if (state.GLastError != DD_OK) {
        D3DError("Could not get the surface description of the source texture.",(state.GLastError));
        goto exit_with_error;
    }
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_ALLOCONLOAD;

	// DDSCAPS_MIPMAP 
	// DDSCAPS_MODEX 
	// DDSCAPS_ALPHA 


	if (lpTextureSurf != NULL) { // take old one 
		state.GLastError = DD_OK;
	}
    else     
		state.GLastError = device->CreateSurface(&ddsd, &lpTextureSurf);
    
	if (state.GLastError != DD_OK) {
        D3DError("Could not create the destination texture surface.",(state.GLastError));
        goto exit_with_error;
    }
    if (ddsd.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8) {
        pcaps = DDPCAPS_8BIT | DDPCAPS_ALLOW256;
    } else if (ddsd.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED4) {
        pcaps = DDPCAPS_4BIT;
    } else {
        pcaps = 0;
    }

    if (pcaps) {
        memset(ppe, 0, sizeof(PALETTEENTRY) * 256);

		//if (hasCkey) 
		//	ppe[ckey.dwColorSpaceLowValue].peFlags = D3DPAL_READONLY;

        state.GLastError = device->lpDD->CreatePalette(pcaps,ppe, &lpDstPalette, NULL);
        if (state.GLastError != DD_OK) {
            D3DError("Failed to create a palette for the destination texture.",(state.GLastError));
            goto exit_with_error;
        }
        state.GLastError = lpTextureSurf->SetPalette(lpDstPalette);

        if (state.GLastError != DD_OK) {
            D3DError("Failed to set the destination texture's palette",(state.GLastError));
            goto exit_with_error;
        }
        
	    RELEASE(lpDstPalette); // hg wasn´t deleted 
    }

	if (hasCkey) {
		state.GLastError = lpTextureSurf->SetColorKey(DDCKEY_SRCBLT ,&ckey); 
		if (state.GLastError!=D3D_OK) {
			D3DError("SetColorKey on Texture failed",state.GLastError);
		}
		hasColorKey = TRUE;
		if (!state.dx5) hasAlpha = 1; // DX3 must enable  alpha blending for colorKey 

	}

    /*
     * Query our destination surface for a texture interface
     */
    state.GLastError = lpTextureSurf->QueryInterface(IID_IDirect3DTexture,
                                             (LPVOID*)&lpTexture);
    if (state.GLastError != DD_OK) {
        D3DError("Failed to obtain D3D texture interface for a destination texture.",(state.GLastError));
        goto exit_with_error;
    }
    /*
     * Load the source texture into the destination.  During this call, a
     * driver could compress or reformat the texture surface and put it in
     * video memory.
     */
    state.GLastError = lpTexture->Load(lpSrcTexture);
    if (state.GLastError != DD_OK) {
        D3DError("Could not load a source texture into a destination texture.",(state.GLastError));
        goto exit_with_error;
    }


    /* 
     * Now we are done with the source texture
     */
    RELEASE(lpSrcTexture); // keep it  ?
    RELEASE(lpSrcTextureSurf);

    /*
     * Did the texture end up in video memory?
     */
    state.GLastError = device->GetSurfDesc(&ddsd, lpTextureSurf);
    if (state.GLastError != DD_OK) {
        D3DError("Could not get the surface description of the loaded texture surface.",(state.GLastError));
        goto exit_with_error;
    }

	inVideoMem = TRUE;

    if (!(ddsd.ddsCaps.dwCaps & DDSCAPS_VIDEOMEMORY)) {
		//CString msg;
		//msg.Format("2 Im  Texture not in video %X ",ddsd.ddsCaps.dwCaps,device->ThisDriver.bIsHardware); // hg debug
		//D3DError(msg,-1);
		TRACE("2 Im  Texture not in video %X %d",ddsd.ddsCaps.dwCaps,device->ThisDriver.bIsHardware); // hg debug

		if (device->ThisDriver.bIsHardware  && !(ddsd.ddsCaps.dwCaps & DDSCAPS_NONLOCALVIDMEM)) {
			inVideoMem = FALSE;
			state.GLastError = lpTexture->Unload();
		    RELEASE(lpSrcTextureSurf);
		    RELEASE(lpTexture);
		    RELEASE(lpTextureSurf);
			hTexture = 0;
			if (state.OnTextureMemoryOverflow(*this)) {
				goto Retry;
			}
			//device->bTexturesInVideo = FALSE;
			return TRUE;
		}
	}


    state.GLastError = lpTexture->GetHandle(device->lpD3DDevice, &hTexture);

    if (state.GLastError != DD_OK) {
        D3DError("Could not get a handle to loaded texture",(state.GLastError));
        goto exit_with_error;
    }
    
    
    return TRUE;

exit_with_error:
    RELEASE(lpSrcTexture);
    RELEASE(lpSrcTextureSurf);
    RELEASE(lpDstPalette);
    RELEASE(lpTexture);
    RELEASE(lpTextureSurf);
	cannotLoad = 1; // new 18.05.99 hg 

    return FALSE;       
}


gbool
G3TextureHandle::GetImage(image &im)
{
	int ret= ReadTextureImage(fileFormat,fileName, &im,1,1,1024,1024,FALSE,TRUE,
				 (1<<IM_RGBA) | (1<<IM_RGB) | (1<<IM_GRAYA) | (1<<IM_GRAY)| (1<<IM_MAPPED8));
	if (ret<=0) return FALSE;
	else return TRUE;
}

gbool
G3TextureHandle::LoadTextureSurf(GD3Device *device,RenderState &state)
{
    DDSURFACEDESC ddsd;
    LPDIRECTDRAWSURFACE lpSrcTextureSurf = NULL;
    LPDIRECT3DTEXTURE lpSrcTexture = NULL;
    LPDIRECTDRAWPALETTE lpDstPalette = NULL;
    PALETTEENTRY ppe[256];
	DDCOLORKEY ckey;	// Color key, for GIF transparent index 
	BOOL hasCkey = FALSE;
	int targetFormat = -1; // choosen texture format 

    DWORD pcaps;
	gbool mipMap = FALSE;
	DWORD mipLevels = 1;

#if 1

	if (state.canDoMipMap) { // debug test 
		// state.SetRenderState(D3DRENDERSTATE_TEXTUREMIN,D3DFILTER_LINEARMIPNEAREST /*D3DFILTER_MIPNEAREST*/);
		// mipMap = TRUE;
		mipMap = state.textureParameters.NeedMipmaps();
		
	}
#endif



Retry:

    /*
     * Release the surface if it is hanging around
     */
    RELEASE(lpTextureSurf);
	hTexture = 0;
	hasAlpha = FALSE;
	isGrayscale = FALSE;
	targetFormat = -1; // choosen texture format 


    /*
     * Create a surface in system memory and load the Image file into it.
     * Query for the texture interface.
     */
#if 1
	{	



		BOOL needPalette;

		int formats =  state.GetSupportedImageFormats();
		formats |= (1<<IM_MAPPED8); // RGBA_1 Test 

		// image library image structure 
		image im;
		image_zero(&im);

		im.supported_fmts = formats ;

		// read the texture file
		int ret= ReadTextureImage(fileFormat,fileName, &im,
			state.textureParameters.min_size_x,state.textureParameters.min_size_y,
			state.textureParameters.current_max_size_x,state.textureParameters.current_max_size_y,
			state.textureParameters.square_size,TRUE,formats);

		if (ret<0) 
			cannotLoad = 1; 

		else 
		if (im.image) {
			
			cannotLoad=0;

			isGrayscale = image_is_gray(&im);

			//  get best texture format for image 

			if (state.rgbFormat>=0) targetFormat = state.rgbFormat;
			else if (state.palette8Format>=0) targetFormat = state.palette8Format;
			else targetFormat = device->CurrTextureFormat;

#ifndef _GLVIEW_IE
			// new 18.01.98
			if (!device->TextureFormat[device->CurrTextureFormat].bPalettized) {
				targetFormat = device->CurrTextureFormat; // let user choice ???
			}
#endif 


			switch (im.fmt) {
			case IM_BW:
			case IM_GRAY:
			case IM_MAPPED4:
			case IM_MAPPED8:
					if (state.palette8Format>=0) {
						targetFormat = state.palette8Format;
						mipMap = FALSE;
					}
					else if (state.rgba16_1Format>=0 && im.colorKey) {
						ret=image_convert_to_rgba(&im);
						if (ret>=0) {
							targetFormat = state.rgba16_1Format;
							break;
						}
					}
			

					break;
			case IM_GRAYA:
					if (state.rgbaFormat>=0) {
						ret=image_convert_to_rgba(&im);
						if (ret>=0) {
							targetFormat = state.rgbaFormat;
							break;
						}
					}
					if (state.palette8Format>=0) 
						targetFormat = state.palette8Format;
					break;
			case IM_RGBA:
					if (state.rgbaFormat>=0) 
						targetFormat = state.rgbaFormat;
					break;
			default :
				break;

			}

			// dither RGB to palette 8
			if (device->TextureFormat[targetFormat].bPalettized) {
				needPalette = TRUE;
				/* convert RGB image to FS dither mapped image */
				if (im.fmt != IM_MAPPED8 ) {
					// beam me up test
//					if (im.fmt == IM_RGB && (im.width <512))
//						ret = image_scale(&im, min(512,im.width*2),min(512,im.height*2));

					ret = image_convert_image_mapped8(&im);
				}
				// to do : mapped 4
			} else {
				
				if (im.fmt == IM_RGBA && (device->TextureFormat[targetFormat].AlphaBPP >0)) {
					// Alpha supported
					hasAlpha = TRUE;
				} // for now, convert everything else to RGB 
				else 
				if (im.fmt != IM_RGB) {
						ret=image_convert_to_rgb(&im);
				}
			}		
	

			TRACE("Texture %s %d %d %d  Format = %d \n",fileName,im.width,im.height,im.ncolors,targetFormat);

			sizeX = im.width,sizeY=im.height;
			memcpy(&ddsd,&device->TextureFormat[targetFormat].ddsd,sizeof(DDSURFACEDESC));
			
			ddsd.dwFlags=0;
			ddsd.ddsCaps.dwCaps=0;

			if (mipMap) {
				mipLevels = CommputeMipLevels(state.textureParameters.min_size_x,state.textureParameters.min_size_y,
					sizeX,sizeY);

				if (mipLevels <=1) mipMap = FALSE;
			}

		    lpSrcTextureSurf = CreateSurfaceFromImageData(
									device->lpDD, 
									im.fmt,im.width,im.height,im.image,
									im.ncolors,im.cmap,	
                                    &ddsd,
									DDSCAPS_SYSTEMMEMORY,
									im.colorKeyIndex,im.colorKeyRgb,ckey,mipLevels);
			

			if (lpSrcTextureSurf) {
				if (im.colorKey &&!hasAlpha) // no set color key if already alpha // hg 25.10.99, texture even wasn't drawn
				{	

				state.GLastError = lpSrcTextureSurf->SetColorKey(DDCKEY_SRCBLT ,&ckey); 
				if (state.GLastError!=D3D_OK) 
					D3DError("SetColorKey failed",state.GLastError);
				else ;;

				TRACE("Color key %d  BG index %d \n",im.colorKeyIndex, im.backgroundIndex );
				
				hasCkey = TRUE;
				hasColorKey = 1;
				if (!state.dx5) hasAlpha = 1; // DX3 must enable  alpha blending for colorKey 

				}
			
				if (mipMap)	{
					DDSCAPS ddsCaps; //ddsd.ddsCaps.
					HRESULT ddres;
					LPDIRECTDRAWSURFACE lpDDLevel, lpDDNextLevel; 

					ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP; 
				
					ddres = lpSrcTextureSurf->GetAttachedSurface(&ddsCaps, &lpDDLevel); 
					if (ddres != DD_OK) {
						D3DError("Loading MipLevels .",(ddres));
					}

					while (ddres == DD_OK) 
					{ 
					// Process this level. 

					image_scale(&im,im.width/2,im.height/2,TRUE); // filter down 

					TRACE("Loading mip level %d %d \n",im.width,im.height);

					LoadSurfaceImageData(
									lpDDLevel, 
									NULL,
									im.fmt,im.width,im.height,im.image,
									im.ncolors,im.cmap,	
                                    ddsd,
                                    im.colorKeyIndex);//,im.colorKeyRgb,ckey);
				//if (im.colorKey) 
				if (im.colorKey &&!hasAlpha) // no set color key if already alpha // hg 25.10.99, texture even wasn't drawn

						state.GLastError = lpDDLevel->SetColorKey(DDCKEY_SRCBLT ,&ckey); 

					ddres = lpDDLevel->GetAttachedSurface(&ddsCaps, &lpDDNextLevel); 
					lpDDLevel->Release(); 
					lpDDLevel = lpDDNextLevel; 
				} 

				if ((ddres != DD_OK) && (ddres != DDERR_NOTFOUND)) 
				{
					// Code to handle the error goes here
					D3DError("Loading MipLevels .",(ddres));
					mipMap = FALSE;
				}
				ASSERT(lpDDLevel == NULL);
				}

			}
			image_free_all(&im);

		}
		else  cannotLoad = 1;

	}
#else
    lpSrcTextureSurf = CreateSurfaceFromImage(device->lpDD, fileName,
                                          &device->ThisTextureFormat.ddsd,
                                          DDSCAPS_SYSTEMMEMORY);
#endif

	this->textureFormat = targetFormat; // the texture format


    if (!lpSrcTextureSurf)
        goto exit_with_error;
    

	state.GLastError = lpSrcTextureSurf->QueryInterface(IID_IDirect3DTexture,
                                             (LPVOID*)&lpSrcTexture);
    if (state.GLastError != DD_OK) {
        D3DError("Failed to obtain D3D texture interface for a source texture.", (state.GLastError));
        goto exit_with_error;
    }

	if (!state.dx5 && (state.caps.dwDevCaps & D3DDEVCAPS_TEXTURESYSTEMMEMORY) && !device->ThisDriver.bIsHardware) {
		// lloks different N4 4.0 & RGB driver ?? 
	    state.GLastError = lpSrcTexture->GetHandle(device->lpD3DDevice, &hTexture);
		if (state.GLastError != DD_OK) {
			D3DError("Could not get a handle to loaded sys texture",-1);
			goto exit_with_error;
		}
		lpTextureSurf = lpSrcTextureSurf; // store the pointers
		lpTexture = lpSrcTexture;

		lpSrcTextureSurf = NULL;
		lpSrcTexture = NULL;
		return TRUE;
	}

    /*
     * Create an empty texture surface to load the source texture into.
     * The DDSCAPS_ALLOCONLOAD flag allows the DD driver to wait until the
     * load call to allocate the texture in memory because at this point,
     * we may not know how much memory the texture will take up (e.g. it
     * could be compressed to an unknown size in video memory).
     */
    state.GLastError = device->GetSurfDesc(&ddsd, lpSrcTextureSurf);
    if (state.GLastError != DD_OK) {
        D3DError("Could not get the surface description of the source texture.",(state.GLastError));
        goto exit_with_error;
    }
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_ALLOCONLOAD;

	if (mipMap) {
		ddsd.dwFlags |= DDSD_MIPMAPCOUNT ;
		ddsd.dwMipMapCount = mipLevels;
		ddsd.ddsCaps.dwCaps |=  DDSCAPS_MIPMAP | DDSCAPS_COMPLEX;
	}



/*	if (device->ThisDriver.bIsHardware)	
		ddsd.ddsCaps.dwCaps |= DDSCAPS_NONLOCALVIDMEM;
*/
	
	// DDSCAPS_MIPMAP 
	// DDSCAPS_MODEX 
	// DDSCAPS_ALPHA 


    
	state.GLastError = device->CreateSurface(&ddsd, &lpTextureSurf);
    
	if (state.GLastError != DD_OK) {
		if (state.GLastError == DDERR_INVALIDPARAMS) {
			if (sizeX > 1024 || sizeY >1024) {
				// got this on Riva  TNT ????
				state.textureParameters.SetCurrentMaxSize(min(1024,state.textureParameters.current_max_size_x),
					min(1024,state.textureParameters.current_max_size_x));
				RELEASE(lpSrcTexture);
				RELEASE(lpSrcTextureSurf);
				RELEASE(lpDstPalette);
				RELEASE(lpTexture);
				RELEASE(lpTextureSurf);
				goto Retry;


			}
		}
        D3DError("Could not create the destination texture surface.",(state.GLastError));
        goto exit_with_error;
    }
    if (ddsd.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8) {
        pcaps = DDPCAPS_8BIT | DDPCAPS_ALLOW256;
    } else if (ddsd.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED4) {
        pcaps = DDPCAPS_4BIT;
    } else {
        pcaps = 0;
    }

    if (pcaps) {
        memset(ppe, 0, sizeof(PALETTEENTRY) * 256);

		//if (hasCkey) 
		//	ppe[ckey.dwColorSpaceLowValue].peFlags = D3DPAL_READONLY;

        state.GLastError = device->lpDD->CreatePalette(pcaps,ppe, &lpDstPalette, NULL);
        if (state.GLastError != DD_OK) {
            D3DError("Failed to create a palette for the destination texture.",(state.GLastError));
            goto exit_with_error;
        }
        state.GLastError = lpTextureSurf->SetPalette(lpDstPalette);

        if (state.GLastError != DD_OK) {
            D3DError("Failed to set the destination texture's palette",(state.GLastError));
            goto exit_with_error;
        }
        
	    RELEASE(lpDstPalette); // hg wasn´t deleted 
    }

	if (hasCkey) {
		state.GLastError = lpTextureSurf->SetColorKey(DDCKEY_SRCBLT ,&ckey); 
		if (state.GLastError!=D3D_OK) {
			D3DError("SetColorKey on Texture failed",state.GLastError);
		}
		hasColorKey = TRUE;
		if (!state.dx5) hasAlpha = 1; // DX3 must enable  alpha blending for colorKey 

	}

    /*
     * Query our destination surface for a texture interface
     */
    state.GLastError = lpTextureSurf->QueryInterface(IID_IDirect3DTexture,
                                             (LPVOID*)&lpTexture);
    if (state.GLastError != DD_OK) {
        D3DError("Failed to obtain D3D texture interface for a destination texture.",(state.GLastError));
        goto exit_with_error;
    }
    /*
     * Load the source texture into the destination.  During this call, a
     * driver could compress or reformat the texture surface and put it in
     * video memory.
     */
    state.GLastError = lpTexture->Load(lpSrcTexture);
    if (state.GLastError != DD_OK) {
        D3DError("Could not load a source texture into a destination texture.",(state.GLastError));
        goto exit_with_error;
    }




    /* 
     * Now we are done with the source texture
     */
    RELEASE(lpSrcTexture);
    RELEASE(lpSrcTextureSurf);


    /*
     * Did the texture end up in video memory?
     */
	inVideoMem = TRUE;
    state.GLastError = device->GetSurfDesc(&ddsd, lpTextureSurf);
    if (state.GLastError != DD_OK) {
        D3DError("Could not get the surface description of the loaded texture surface.",(state.GLastError));
        goto exit_with_error;
    }
    if (!(ddsd.ddsCaps.dwCaps & DDSCAPS_VIDEOMEMORY)) {
		//CString msg;
		//msg.Format("Texture not in video %X ",ddsd.ddsCaps.dwCaps,device->ThisDriver.bIsHardware); // hg debug
		//D3DError(msg,-1);
		TRACE("Im  Texture not in video %X %d",ddsd.ddsCaps.dwCaps,device->ThisDriver.bIsHardware); // hg debug
		if (ddsd.ddsCaps.dwCaps & DDSCAPS_NONLOCALVIDMEM) {
				TRACE("Texture in non local video mem \n");
		}

		if (device->ThisDriver.bIsHardware && !(ddsd.ddsCaps.dwCaps & DDSCAPS_NONLOCALVIDMEM)) {
			inVideoMem = FALSE;
			state.GLastError = lpTexture->Unload();
		    RELEASE(lpTexture);
			hTexture = 0;
			if (state.OnTextureMemoryOverflow(*this)) {
				goto Retry;
			}
			//device->bTexturesInVideo = FALSE;
			return TRUE;
		}
	}

    state.GLastError = lpTexture->GetHandle(device->lpD3DDevice, &hTexture);
    if (state.GLastError != DD_OK) {
        D3DError("Could not get a handle to loaded texture",-1);
        goto exit_with_error;
    }
    
    
    return TRUE;

exit_with_error:
    RELEASE(lpSrcTexture);
    RELEASE(lpSrcTextureSurf);
    RELEASE(lpDstPalette);
    RELEASE(lpTexture);
    RELEASE(lpTextureSurf);
	cannotLoad = 1; // new 18.05.99 hg 
    return FALSE;       
}


/*
 * ReloadTextureSurf
 * Reloads a lost and restored texture surface
 */
gbool
G3TextureHandle::ReloadTextureSurf(GD3Device *device,RenderState &state)
{
    LPDIRECTDRAWSURFACE lpSrcTextureSurf = NULL;
    LPDIRECT3DTEXTURE lpSrcTexture = NULL;

    /*
     * Create a surface in system memory and load the PPM file into it.
     * Query for the texture interface.
     */
    lpSrcTextureSurf = CreateSurfaceFromImage(device->lpDD, fileName,
                                          &device->ThisTextureFormat.ddsd,
                                          DDSCAPS_SYSTEMMEMORY);
    if (!lpSrcTextureSurf)
        goto exit_with_error;

    state.GLastError = lpSrcTextureSurf->QueryInterface(IID_IDirect3DTexture,
                                             (LPVOID*)&lpSrcTexture);
    if (state.GLastError != DD_OK) {

        D3DError("Failed to obtain D3D texture interface for a source texture.", (state.GLastError));
        goto exit_with_error;
    }
    /*
     * Load the source texture into the destination.  During this call, a
     * driver could compress or reformat the texture surface and put it in
     * video memory.
     */
    state.GLastError = lpTexture->Load(lpSrcTexture);
    if (state.GLastError != DD_OK) {
       D3DError("Could not load a source texture into a destination texture.",(state.GLastError));
        goto exit_with_error;
    }

    state.GLastError = lpTexture->GetHandle(device->lpD3DDevice, &hTexture);
    if (state.GLastError != DD_OK) {
        D3DError("Could not get a handle to loaded texture",(state.GLastError));
        goto exit_with_error;
    }

    /* 
     * Now we are done with the source texture
     */
    RELEASE(lpSrcTexture);
    RELEASE(lpSrcTextureSurf);

    return TRUE;

exit_with_error:
    RELEASE(lpSrcTexture);
    RELEASE(lpSrcTextureSurf);
	cannotLoad = 1; // new 18.05.99 hg 

    return FALSE;       
}


// set texture by texture filename 
gbool G3TextureHandle::Set(RenderState &state,
             LPCTSTR fileName,int format,LPCTSTR fileUrl)
{
	
	//strcpy(this->fileName,fileName);
	this->fileName=fileName;
	this->fileUrl=fileUrl;
	fileFormat = format;
	return LoadTextureSurf(state.device,state);
}

// set texture from image data
gbool G3TextureHandle::Set(RenderState &state,
				int format,int width,int height,int numComponents,unsigned char *data, char *colorTable)
{
	fileName.Empty();
	fileUrl.Empty();
	fileFormat = 0;
	int ret;

	// image library image structure 
	image im;
	
	image_zero(&im);

	im.supported_fmts = state.GetSupportedImageFormats();
	
	int fmt = IM_RGB;
	switch(numComponents) {
	case 4 : fmt = IM_RGBA; break;
	case 3 : fmt = IM_RGB; break;
	case 2 : fmt = IM_GRAYA; break;
	case 1 : fmt = IM_GRAY; break;
	default : return FALSE;	
	}

	//image_set_fmt(&im,fmt,width,height);
	ret = image_alloc(&im,fmt,width,height);

	if (ret<0) return FALSE;

	memcpy(im.image,data,width*height*numComponents);

	//image_set_data_by_ref(&im,data);

	ret = image_scale_2power(&im,state.textureParameters.min_size_x,state.textureParameters.min_size_y,state.textureParameters.current_max_size_x,state.textureParameters.current_max_size_y,state.textureParameters.square_size); // would do a free on format

/*
   		if (format == GIF_FMT && (im->fmt == IM_MAPPED8) && (!(supported_fmts & (1<<IM_MAPPED8)))&& ((supported_fmts & (1<<IM_RGBA)))) {
		   ret=image_convert_to_rgba(im);
   		}
		else 
*/
   		if ((im.fmt == IM_MAPPED8) && (!(im.supported_fmts & (1<<IM_MAPPED8)))) {
		   ret=image_convert_to_rgb(&im);
   		}
   		else if ((im.fmt == IM_GRAY) && (!(im.supported_fmts & (1<<IM_GRAY)))) {
		   ret=image_convert_to_rgb(&im);
   		}



	gbool retb=LoadTextureSurf(state.device,state,im);

	image_free_all(&im);

	return retb;

}

// set by image 
gbool G3TextureHandle::Set(RenderState &state,
			image *im,gbool checkScale,gbool checkFormat,gbool filteredScale)
{
	int ret = 0;

	im->supported_fmts = state.GetSupportedImageFormats();

	if (checkScale)
		ret = image_scale_2power(im,state.textureParameters.min_size_x,state.textureParameters.min_size_y,state.textureParameters.current_max_size_x,state.textureParameters.current_max_size_y,state.textureParameters.square_size,filteredScale); // would do a free on format

/*
		// D3D has color key 
   		if (format == GIF_FMT && (im->fmt == IM_MAPPED8) && (!(supported_fmts & (1<<IM_MAPPED8))) && ((supported_fmts & (1<<IM_RGBA)))) {
		   ret=image_convert_to_rgba(im);
   		}
		else 
*/
/*
  done later 
   		if ((im->fmt == IM_MAPPED8) && (!(im->supported_fmts & (1<<IM_MAPPED8)))) {
		   ret=image_convert_to_rgb(im);
   		}
   		else if ((im->fmt == IM_GRAY) && (!(im->supported_fmts & (1<<IM_GRAY)))) {
		   ret=image_convert_to_rgb(im);
   		}
*/

	gbool retb;

	if ( (sizeX == im->width) && (sizeY == im->height) && lpTexture) {
//		retb=ReloadTextureSurf(state.device,state,*im);
		retb=LoadTextureSurf(state.device,state,*im);
	
	}
	else 
		retb=LoadTextureSurf(state.device,state,*im);

	return retb;
}

gbool G3TextureHandle::Set(RenderState &state,
						   LPDIRECTDRAWSURFACE srcSurface,IDirectDrawPalette *srcPalette,DDSURFACEDESC &srcDdsd, RECT &srcRect,gbool checkScale,gbool checkFormat,gbool filteredScale)
{
	gbool retb;
	// TBD: 
//	retb=LoadTextureSurf(state.device,state,srcSurface,srcDdsd);

	return retb;

}



#endif