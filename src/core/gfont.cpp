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
/******************************************************************************
@doc

@module GFont.cpp - GLView TrueType font support	|

Copyright (c) 1995	by Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:

Changes:

  mbcs support

$Revision: 1.9 $
$Log: gfont.cpp,v $
Revision 1.9  1999/07/06 16:54:25  tom
..

Revision 1.8  1999/03/10 10:21:34  tom
*** empty log message ***


Todo :

******************************************************************************/


#include "stdafx.h"

#if defined(_WIN32)

//#include <tchar.h>
#include <mbstring.h>


//#include <stdlib.h>
//#include <string.h>

#include "gutils.h"

#include "gfont.h"
//#include "astream.h"
//#include "point.h"
//#include "pointa.h"
#include "gclass.h"
#include "gshell.h"
#include "gshelli.h"

#pragma optimize( "ag", off)
#pragma optimize( "pty", on)



// Convert degrees to radians for math functions.
#define RAD(x) ((x) * 3.1415927 / 180)

/****************************************************************************
 *  FUNCTION   : FixedFromDouble
 *  RETURNS    : FIXED value representing the given double.
 ****************************************************************************/
FIXED/* PASCAL NEAR */ FixedFromDouble(double d)
{
    long l;

    l = (long) (d * 65536L);
    return *(FIXED *)&l;
}

/****************************************************************************
 *  FUNCTION   : IntFromFixed
 *  RETURNS    : int value approximating the FIXED value.
 ****************************************************************************/
int/* PASCAL NEAR */ IntFromFixed(FIXED f)
{
    if (f.fract >= 0x8000)
	return(f.value + 1);
    else
	return(f.value);
}

/****************************************************************************
 *  FUNCTION   : FloatFromFixed
 *  RETURNS    : float value approximating the FIXED value.
 * HG 
 ****************************************************************************/
float/* PASCAL NEAR */ FloatFromFixed(FIXED f)
{
   return ((float) f.value + ((float) f.fract / (float)65536));
}

/****************************************************************************
 *  FUNCTION   : fxDiv2
 *  RETURNS    : (val1 + val2)/2 for FIXED values
 ****************************************************************************/
FIXED/* PASCAL NEAR */ fxDiv2(FIXED fxVal1, FIXED fxVal2)
{
    long l;

    l = (*((long far *)&(fxVal1)) + *((long far *)&(fxVal2)))/2;
    return(*(FIXED *)&l);
}

/****************************************************************************
 *  FUNCTION   : MakeRotationMat
 *  PURPOSE    : Fill in a rotation matrix based on the given angle.
 *  RETURNS    : none.
 ****************************************************************************/
void MakeRotationMat(LPMAT2 lpMat, double dAngle)
{
    lpMat->eM11 = FixedFromDouble(cos(RAD(dAngle)));
    lpMat->eM12 = FixedFromDouble(sin(RAD(dAngle)));
    lpMat->eM21 = FixedFromDouble(-sin(RAD(dAngle)));
    lpMat->eM22 = FixedFromDouble(cos(RAD(dAngle)));
}

/****************************************************************************
 *  FUNCTION   : ShearMat
 *  PURPOSE    : Fill in a 0.25 horizontal shear matrix.
 *  RETURNS    : none.
 ****************************************************************************/
void ShearMat(LPMAT2 lpMat)
{
    lpMat->eM11 = FixedFromDouble(1);
    lpMat->eM12 = FixedFromDouble(0);
    lpMat->eM21 = FixedFromDouble(0.25);
    lpMat->eM22 = FixedFromDouble(1);
}
/****************************************************************************
 *  FUNCTION   : IdentityMat
 *  PURPOSE    : Fill in matrix to be the identity matrix.
 *  RETURNS    : none.
 ****************************************************************************/
void IdentityMat(LPMAT2 lpMat)
{
    lpMat->eM11 = FixedFromDouble(1);
    lpMat->eM12 = FixedFromDouble(0);
    lpMat->eM21 = FixedFromDouble(0);
    lpMat->eM22 = FixedFromDouble(1);
}

/****************************************************************************
 *
 *  FUNCTION   : BitmapFromT2Bitmap
 *
 *  PURPOSE    : Create a Windows bitmap from the GGO_BITMAP format.
 *               Two methods are provided:
 *                 1) create a DWORD-aligned bitmap by setting WidthBytes.
 *                 2) WORD-align the bits and create conventional bitmap.
 *
 *  RETURNS    : Handle to a Windows bitmap object
 *
 ****************************************************************************/
HBITMAP BitmapFromT2Bitmap(void FAR *lpBits, WORD width, WORD height)
{
#if 1
{
// Create a DWORD-aligned Windows bitmap.  By building up a BITMAP structure
// with the bmWidthBytes field corresponding to DWORD-alignment and using
// CreateBitmapIndirect, a bitmap that is DWORD-aligned can be read by 
// the device driver.
    BITMAP bm;

    bm.bmType = 0;
    bm.bmWidth = width;
    bm.bmHeight = height;
    bm.bmWidthBytes = ((width + 31) >> 5) << 2;
    bm.bmPlanes = 1;
    bm.bmBitsPixel = 1;
    bm.bmBits = lpBits;
    return (CreateBitmapIndirect(&bm));
}
#else
{
// Create a WORD-aligned (conventional alignment) bitmap by WORD-aligning
// the GGO_BITMAP format bits.  The bits are not touched if the bitmap
// is already WORD-aligned.

    WORD WordWidth;

    // if word align is same as dword align, bitmap is ready.
    // if not, eliminate the extra word buffer.
    WordWidth = (width + 15) >> 4;
    if (WordWidth != ((width + 31) >> 5) << 1)
    {
	_asm 
	{
	push	ds
	les	di,lpBits;
	lds	si,lpBits;
	mov	dx,height

copy_row:
	mov	cx,WordWidth
	rep	movsw		; copy relevant words
	inc	si
	inc	si		; skip over excess WORD
	dec	dx		; one more scanline compacted
	jnz	copy_row
	pop	ds
	}	       
    }
    return(CreateBitmap(width, height, 1, 1, lpBits));
}
#endif
}



// subdivide quadratic Bezier
// result is number of points stored in out 

int SubdivideSpline(const Point &a, const Point &b, const Point &c, int steps, Point *&out)
{
   Point p1 =  a + b;
   p1 *= 0.5;
   Point p2 =  b + c;
   p2 *= 0.5;
   Point m =  p1 + p2;
   m *= 0.5;
   
   steps--;
   if (steps<=0) {
	   *out++= m;
	   *out++= c;
	   return(2);
   }	
   else 
	   return
		SubdivideSpline(a,p1,m,steps,out)
		+ SubdivideSpline(m,p2,c,steps,out);


}





/****************************************************************************
 *
 *  FUNCTION   : QSpline2Polyline
 *
 *  PURPOSE    : Fake spline cracker.  All it really does is take 
 *               the A, B, and C points and pretend they are a polyline,
 *               creating a very unsmooth curve.
 *
 *               In real life, the spline would be digitized.
 *
 *               The calculated points are stored in the given array.
 *
 *  RETURNS    : Number of points added to lpptPolygon.
 *
 *  BONUS INFO : Here is a description of an algorithm to implement
 *               this functionality.  
 *
 *** To break up a parabola defined by three points (A,B,C) into straight 
 *** line vectors given a maximium error. The maximum error is
 *** 1/resolution * 1/ERRDIV.
 ***
 ***           
 ***         B *-_
 ***          /   `-_
 ***         /       `-_
 ***        /           `-_ 
 ***       /               `-_
 ***      /                   `* C
 ***   A *
 ***
 *** PARAMETERS:
 ***
 *** Ax, Ay contains the x and y coordinates for point A. 
 *** Bx, By contains the x and y coordinates for point B. 
 *** Cx, Cy contains the x and y coordinates for point C.
 *** hX, hY are handles to the areas where the straight line vectors are going to be put.
 *** count is pointer to a count of how much data has been put into *hX, and *hY.
 ***
 *** F (t) = (1-t)^2 * A + 2 * t * (1-t) * B + t * t * C, t = 0... 1 =>
 *** F (t) = t * t * (A - 2B + C) + t * (2B - 2A) + A  =>
 *** F (t) = alfa * t * t + beta * t + A
 *** Now say that s goes from 0...N, => t = s/N
 *** set: G (s) = N * N * F (s/N)
 *** G (s) = s * s * (A - 2B + C) + s * N * 2 * (B - A) + N * N * A
 *** => G (0) = N * N * A
 *** => G (1) = (A - 2B + C) + N * 2 * (B - A) + G (0)
 *** => G (2) = 4 * (A - 2B + C) + N * 4 * (B - A) + G (0) =
 ***           3 * (A - 2B + C) + 2 * N * (B - A) + G (1)
 ***
 *** D (G (0)) = G (1) - G (0) = (A - 2B + C) + 2 * N * (B - A)
 *** D (G (1)) = G (2) - G (1) = 3 * (A - 2B + C) + 2 * N * (B - A)
 *** DD (G)   = D (G (1)) - D (G (0)) = 2 * (A - 2B + C)
 *** Also, error = DD (G) / 8 .
 *** Also, a subdivided DD = old DD/4.
 ***
 ****************************************************************************/
int NEAR PASCAL QSpline2Polyline(LPPOINT lpptPolygon, LPPOINTFX lppfSpline)
{
    // Skip over A point.  It is already in the polygon.
    lppfSpline++;

    // Store the B point.
    lpptPolygon->x = IntFromFixed(lppfSpline->x);
    lpptPolygon->y = IntFromFixed(lppfSpline->y);
    lppfSpline++;
    lpptPolygon++;

    // Store the C point.
    lpptPolygon->x = IntFromFixed(lppfSpline->x);
    lpptPolygon->y = IntFromFixed(lppfSpline->y);

    return(2);	// Two points added to polygon.
}

int NEAR PASCAL QSpline2Polyline(Point * lpptPolygon, LPPOINTFX lppfSpline)
{
    // Skip over A point.  It is already in the polygon.
    lppfSpline++;

    // Store the B point.
    lpptPolygon->x = FloatFromFixed(lppfSpline->x);
    lpptPolygon->y = FloatFromFixed(lppfSpline->y);
    lppfSpline++;
    lpptPolygon++;

    // Store the C point.
    lpptPolygon->x = FloatFromFixed(lppfSpline->x);
    lpptPolygon->y = FloatFromFixed(lppfSpline->y);

    return(2);	// Two points added to polygon.
}

int NEAR PASCAL QSpline2PolylineSubdiv(Point *lpptPolygon, LPPOINTFX lppfSpline, int steps)
{
	Point a,b,c;
    a.Set(FloatFromFixed(lppfSpline->x),FloatFromFixed(lppfSpline->y),0);
    lppfSpline++;

    b.Set(FloatFromFixed(lppfSpline->x),FloatFromFixed(lppfSpline->y),0);
    lppfSpline++;

    c.Set(FloatFromFixed(lppfSpline->x),FloatFromFixed(lppfSpline->y),0);
    lppfSpline++;

    return SubdivideSpline(a,b,c,steps,lpptPolygon);	//  points added to polygon.
}


/****************************************************************************
 *  FUNCTION   : DrawT2Outline
 *
 *  PURPOSE    : Decode the GGO_NATIVE outline, create a polypolygon from it,
 *               and draw it using PolyPolygon.  Color and relative 
 *               positioning provided by caller.
 *
 *               Polygon is not actually returned as would be more common
 *               in real usage.  Also, an arbitrary size for the polygon
 *               is specified instead of actually expanding on a need-to-
 *               grow basis.
 *
 *               Error conditions are not handled.
 *
 *  RETURNS    : none.
 ****************************************************************************/
void DrawT2Outline(HDC hDC, LPTTPOLYGONHEADER lpHeader, DWORD size) 
{
    LPTTPOLYGONHEADER lpStart;
    LPTTPOLYCURVE lpCurve;
    POINT pt[1000];
    /* WORD*/   int count[50];
    WORD cTotal = 0;	// Total number of points in polypolygon.
    WORD cInCurve; 	// Number of points in current curve.
    WORD cCurves = 0;	// Number of curves in polypolygon.
    WORD cInSpline;	// Number of points in digitized spline curve.
    WORD iFirstCurve;	// Index to start point of first curve.
    WORD i;
    POINTFX spline[3];

    lpStart = lpHeader;
    while ((DWORD)lpHeader < (DWORD)(((LPSTR)lpStart) + size))
    {
	if (lpHeader->dwType == TT_POLYGON_TYPE)
	{
	    cInCurve = 0;

	    // Get to first curve.
	    lpCurve = (LPTTPOLYCURVE) (lpHeader + 1);
	    iFirstCurve = cTotal;

	    while ((DWORD)lpCurve < (DWORD)(((LPSTR)lpHeader) + lpHeader->cb))
	    {
		if (lpCurve->wType == TT_PRIM_LINE)
		{
		    for (i = 0; i < lpCurve->cpfx; i++)
		    {
			pt[cTotal].x = IntFromFixed(lpCurve->apfx[i].x);
			pt[cTotal].y = IntFromFixed(lpCurve->apfx[i].y);
			cTotal++;
			cInCurve++;
		    }
		}
		else if (lpCurve->wType == TT_PRIM_QSPLINE)
		{
		    //**********************************************
		    // Format assumption:
		    //   The bytes immediately preceding a POLYCURVE
		    //   structure contain a valid POINTFX.
		    //
		    //   If this is first curve, this points to the 
		    //      pfxStart of the POLYGONHEADER.
		    //   Otherwise, this points to the last point of
		    //      the previous POLYCURVE.
		    //
		    //	 In either case, this is representative of the
		    //      previous curve's last point.
		    //**********************************************
		    spline[0] = *(LPPOINTFX)((LPSTR)lpCurve - sizeof(POINTFX));
	
		    for (i = 0; i < lpCurve->cpfx;)
		    {
		        // The B point.
			spline[1] = lpCurve->apfx[i++];

			// Calculate the C point.
			if (i == (lpCurve->cpfx - 1))
			{
			    spline[2] = lpCurve->apfx[i++];
			}     
			else
			{
			    // C is midpoint between B and next point.
			    spline[2].x = fxDiv2(lpCurve->apfx[i-1].x,
			    				lpCurve->apfx[i].x);
			    spline[2].y = fxDiv2(lpCurve->apfx[i-1].y,
			    				lpCurve->apfx[i].y);
			}

			cInSpline = QSpline2Polyline(&(pt[cTotal]), spline);
			cTotal += cInSpline;
			cInCurve += cInSpline;

			// New A point for next slice of spline.
			spline[0] = spline[2];
		    }
		}
		else
		; // error, error, error

		// Move on to next curve.
		lpCurve = (LPTTPOLYCURVE)&(lpCurve->apfx[i]);
	    }

	    // Add points to close curve.
	    // Depending on the specific font and glyph being used, these
	    // may not always be needed, but it never hurts.
	    pt[cTotal].x = lpHeader->pfxStart.x.value;
	    pt[cTotal].y = lpHeader->pfxStart.y.value;
	    cInCurve++;
	    cTotal++;
	    pt[cTotal].x = pt[iFirstCurve].x;
	    pt[cTotal].y = pt[iFirstCurve].y;
	    cInCurve++;
	    cTotal++;
	    count[cCurves++] = cInCurve;

	    // Move on to next polygon.
	    lpHeader = (LPTTPOLYGONHEADER)(((LPSTR)lpHeader) + lpHeader->cb);
	}
	else
	; // error, error, error
    }

    // flip coordinates to get glyph right side up (Windows coordinates)
    for (i = 0; i < cTotal; i++)
	pt[i].y = 0 - pt[i].y;

    PolyPolygon(hDC, pt, count, cCurves);
}

#if 0
/****************************************************************************
 *  FUNCTION   : OutputGlyph
 *
 *  PURPOSE    : Output a character glyph at a given position.
 *
 *               Depending on user choices, the glyph is either retrieved
 *               as a bitmap or an outline or is output with TextOut.
 *
 *  RETURNS    : none.
 ****************************************************************************/
void/* PASCAL NEAR */ OutputGlyph(HDC hDC, UINT letter, WORD x, WORD y, WORD ascent)
{
    GLYPHMETRICS gm;
    MAT2 mat;
    DWORD size;
    HANDLE hBits;
    LPSTR lpBits;
    WORD flag;
    HBITMAP hbm;
    HBRUSH hbr;
    DWORD oldOrg;

    // Simply output the character.  The matrix setting does not affect
    // this output.
    if (wGlyph == IDM_TEXTOUT)
    {
	TextOut(hDC, x, y, (LPSTR)&letter, 1);
	return;
    }
	       
    // make the 2x2 matrix
    // these are simply hardcoded examples.
    if (wEffect == IDM_IDENTITY)
	IdentityMat(&mat);
    else if (wEffect == IDM_ROTATE60)
	MakeRotationMat(&mat, 60);
    else if (wEffect == IDM_SHEAR)
	ShearMat(&mat);

    flag = (wGlyph == IDM_BITMAP) ? GGO_BITMAP : GGO_NATIVE;

    // allocate space for the bitmap/outline
    size = GetGlyphOutline(hDC, letter, flag, &gm, 0, NULL, &mat);
    hBits = GlobalAlloc(GHND, size);
    lpBits = GlobalLock(hBits);

    if ((GetGlyphOutline(hDC, letter, flag, &gm, size, lpBits, &mat)) != size)
    {
	MessageBox(hMyWnd, "Won't get it", "foo", MB_OK);
	return;
    }

    // Get the glyph in Windows bitmap format and blt to the screen.
    // Alignment is done in the blting.
    if (wGlyph == IDM_BITMAP)
    {
	hbm = BitmapFromT2Bitmap(lpBits, gm.gmBlackBoxX, gm.gmBlackBoxY);
	SelectObject(hdcMem, hbm);

	BitBlt(hDC, x + gm.gmptGlyphOrigin.x, y + (ascent - gm.gmptGlyphOrigin.y), 
		gm.gmBlackBoxX, gm.gmBlackBoxY, hdcMem, 0, 0, SRCCOPY);
	SelectObject(hdcMem, hbmDefault);
	DeleteObject(hbm);
    }
    // Glyph is in outline format.  Set up viewport origin to align the
    // glyph and draw it.  Actual decoding and drawing is done in 
    // DrawT2Outline.
    else if (wGlyph == IDM_OUTLINE)
    {
	hbr = CreateSolidBrush(RGB(255, 0, 255));
	hbr = SelectObject(hDC, hbr);
	
	oldOrg = GetViewportOrg(hDC);

	// Align based on cell origin.
	OffsetViewportOrg(hDC, x, y + ascent);
	DrawT2Outline(hDC, (LPTTPOLYGONHEADER)lpBits, size);

	SetViewportOrg(hDC, LOWORD(oldOrg), HIWORD(oldOrg));

	hbr = SelectObject(hDC, hbr);
	DeleteObject(hbr);
    }

    GlobalUnlock(hBits);
    GlobalFree(hBits);
}

#endif


// decode  the outline and append to shell

void ComputeT2Outline(LPTTPOLYGONHEADER lpHeader, DWORD size,float scaleFac, float x, float y, int duplicateHardPts, 
				GShell &s) 
{
    LPTTPOLYGONHEADER lpStart;
    LPTTPOLYCURVE lpCurve;
    //POINT 
    Point pt[4500];
//	char  ptHard[1500];
    /*WORD*/ 
    INT count[150];
    WORD cTotal = 0;	// Total number of points in polypolygon.
    WORD cInCurve; 	// Number of points in current curve.
    WORD cCurves = 0;	// Number of curves in polypolygon.
    WORD cInSpline;	// Number of points in digitized spline curve.
    WORD iFirstCurve;	// Index to start point of first curve.
    WORD i;
    POINTFX spline[3];
	
	if (!lpHeader) return;
	int splineSubdiv = 2; // number of spline subdivisions

	if (scaleFac >= 0.1) splineSubdiv = 2; // hack for now
	else if (scaleFac >= 0.01) splineSubdiv = 1; // hack for now
	else splineSubdiv = 0;

	gbool isLine = RTISA(&s,GPolyline);

	
    lpStart = lpHeader;
    while ((DWORD)lpHeader < (DWORD)(((LPSTR)lpStart) + size))
    {
	if (lpHeader->dwType == TT_POLYGON_TYPE)
	{
	    cInCurve = 0;

	    // Get to first curve.
	    lpCurve = (LPTTPOLYCURVE) (lpHeader + 1);
	    iFirstCurve = cTotal;

#if 1
		// TTPLOT does this at start
	   // unclear why need, where is doc for this 
	    pt[cTotal].x = lpHeader->pfxStart.x.value;
	    pt[cTotal].y = lpHeader->pfxStart.y.value;
	    cInCurve++;
	    cTotal++;
#endif


	    while ((DWORD)lpCurve < (DWORD)(((LPSTR)lpHeader) + lpHeader->cb))
	    {
		if (lpCurve->wType == TT_PRIM_LINE)
		{
		    for (i = 0; i < lpCurve->cpfx; i++)
		    {
				pt[cTotal].x = FloatFromFixed(lpCurve->apfx[i].x);
				pt[cTotal].y = FloatFromFixed(lpCurve->apfx[i].y);
				//ptHard[cTotal]=1;	// hard edge 
				cTotal++;
				cInCurve++;

				if (duplicateHardPts) // to get hard edge in sweep code
				{	pt[cTotal].x=pt[cTotal-1].x;
					pt[cTotal].y=pt[cTotal-1].y;
					cTotal++;
					cInCurve++;
				}
		    }
		}
		else if (lpCurve->wType == TT_PRIM_QSPLINE)
		{
		    //**********************************************
		    // Format assumption:
		    //   The bytes immediately preceding a POLYCURVE
		    //   structure contain a valid POINTFX.
		    //
		    //   If this is first curve, this points to the 
		    //      pfxStart of the POLYGONHEADER.
		    //   Otherwise, this points to the last point of
		    //      the previous POLYCURVE.
		    //
		    //	 In either case, this is representative of the
		    //      previous curve's last point.
		    //**********************************************
		    spline[0] = *(LPPOINTFX)((LPSTR)lpCurve - sizeof(POINTFX));
	
		    for (i = 0; i < lpCurve->cpfx;)
		    {
		        // The B point.
				spline[1] = lpCurve->apfx[i++];

				// Calculate the C point.
				if (i == (lpCurve->cpfx - 1))
				{
					spline[2] = lpCurve->apfx[i++];
				}     
				else
				{
					// C is midpoint between B and next point.
					spline[2].x = fxDiv2(lpCurve->apfx[i-1].x,
			    					lpCurve->apfx[i].x);
					spline[2].y = fxDiv2(lpCurve->apfx[i-1].y,
			    					lpCurve->apfx[i].y);
				}

				// cInSpline = QSpline2Polyline(/*(LPPOINT)*/ &(pt[cTotal]), spline);
				cInSpline = QSpline2PolylineSubdiv((Point *) &pt[cTotal], spline,splineSubdiv);
	
				//for(int j=0;j<cInSpline;j++) ptHard[j+cTotal]=0;
	
				cTotal += cInSpline;
				cInCurve += cInSpline;

				// New A point for next slice of spline.
				spline[0] = spline[2];
		    }  // i
		}
		else
			TRACE("ComputeT2Outline: unknow stroke type");
		; // error, error, error

		// Move on to next curve.
		lpCurve = (LPTTPOLYCURVE)&(lpCurve->apfx[i]);
	    }

	    // Add points to close curve.
	    // Depending on the specific font and glyph being used, these
	    // may not always be needed, but it never hurts.
#if 0
	   // unclear why need, where is doc for this 
	    pt[cTotal].x = lpHeader->pfxStart.x.value;
	    pt[cTotal].y = lpHeader->pfxStart.y.value;
	    cInCurve++;
	    cTotal++;
#endif

	if (isLine) { // close curve 
	    pt[cTotal].x = pt[iFirstCurve].x;
	    pt[cTotal].y = pt[iFirstCurve].y;
	    cInCurve++;
	    cTotal++;
	}
	    count[cCurves++] = cInCurve;

	    // Move on to next polygon.
	    lpHeader = (LPTTPOLYGONHEADER)(((LPSTR)lpHeader) + lpHeader->cb);
	}
	else
	; // error, error, error
    }


#if 0
// hg
    // flip coordinates to get glyph right side up (Windows coordinates)
    for (i = 0; i < cTotal; i++)
	pt[i].y = 0 - pt[i].y;
#endif


	// Append vertices to shell
	int pstart = s.v.Length();

	s.v.SetLength(pstart+cTotal);
    for (i = 0; i < cTotal; i++) s.v[pstart + i].Set(x+ pt[i].x*scaleFac, y+ pt[i].y*scaleFac);

	int ptOffset=0;

	// Append polygons to shell
	for(i=0; i<cCurves;i++) {
		int cnt = count[i];

		int cntOffset = s.f.Length();
		int newCnt= 0;
		BOOL isHole;

		// compute handedness of polygon 

		float area=0.0;
		for (int ii=0;ii<cnt;ii++) {
			int j = (ii + 1) % cnt;
			area += pt[ptOffset+ii].x * pt[ptOffset+j].y;
			area -= pt[ptOffset+ii].y * pt[ptOffset+j].x;
		}

		area /= 2;

		TRACE("Contour %d, cnt = %d, area = %g \n",i,cnt,area);
	    ptOffset +=cnt;
		
		if (i>0 && (area > 0.0) ) isHole = TRUE;
		else isHole = FALSE;

		if (!isHole || isLine) s.f.Append(cnt);
		else s.f.Append(-cnt); // Assume Hole, but better check cw / ccw orientation
		

		// create the face llist 
		while(cnt>0) {
		 	s.f.Append(pstart);
			newCnt ++;
		 	pstart++;
		 	cnt --;
		 }
		 
		 if (!isHole || isLine) s.f[cntOffset] = newCnt;
		 else s.f[cntOffset]=-newCnt; // Assume Hole, but better check cw / ccw orientation
		 

	}
	s.SetFlag(SHELL_HAS_CONVEX_FACES);
	s.SetEditFlag(SHELL_FACELIST_CHANGED||SHELL_VERTICES_CHANGED);
	s.nfaces=0;
//  PolyPolygon(hDC, pt, count, cCurves);
}


//@func compute a polygonal outline for character letter

void  ComputeGlyphOutline(HDC hDC, UINT letter, 
						  float scaleFac, float &x, float &y, 
						  float dirstepx, // 1.0
						  float dirstepy,// -1.0
						  int duplicateHardPts, GShell &s)
{
    GLYPHMETRICS gm;
    MAT2 mat;
    DWORD size;
    HANDLE hBits;
    LPSTR lpBits;
    WORD flag;
    //DWORD oldOrg;

	       
    // make the 2x2 matrix
    // these are simply hardcoded examples.
//    if (wEffect == IDM_IDENTITY)
	IdentityMat(&mat);
  //  else if (wEffect == IDM_ROTATE60)
//	MakeRotationMat(&mat, 60);
//    else if (wEffect == IDM_SHEAR)
//	ShearMat(&mat);

    flag = GGO_NATIVE;

    // allocate space for the bitmap/outline
    size = GetGlyphOutline(hDC, letter, flag, &gm, 0, NULL, &mat);

	if (size >0) 
	{
  
		hBits = GlobalAlloc(GHND, size);
		lpBits =(LPSTR) GlobalLock(hBits);

		if ((GetGlyphOutline(hDC, letter, flag, &gm, size, lpBits, &mat)) != size)
		{
			TRACE("GFont:Can´t get TrueType Glph outline \n");
		}
		else
		{

			// Glyph is in outline format.
			ComputeT2Outline((LPTTPOLYGONHEADER)lpBits, size,scaleFac,x,y,duplicateHardPts,s);
		}

		GlobalUnlock(hBits);
		GlobalFree(hBits);
	}
	else
	{
		TRACE("GFont:Can´t allocate space for %c \n", letter);
	}

	// todo : prop spacing ? 


	x +=  dirstepx* gm.gmCellIncX * scaleFac;
	if (letter == 10)	
		y +=  dirstepy* gm.gmCellIncY*scaleFac;

}


float ComputeGlyphLineHeight(
	HDC hDC	// a Windows DC with a TrueType font selected
)
{
   return(1.0);
}


/*
 @func compute a polygonal outline for string text
 from a true tpye font

*/


int ComputeGlyphOutline(
	HDC hDC,			// a Windows DC with a TrueType font selected
 	
	const  _TCHAR *text, // the input text, to compute a polygon outline for

 	float &x, float &y, // starting x,y position, will be updated after the call to new position
	float spacing, // 0.0 extra character spacing
	float height,	// = 1.0,

	float dirstepx, // 1.0
	float dirstepy,// -1.0

	float lineSpacing,	// = 1.0,
	int align, 
	float length,		// 0.0 
 	gbool duplicateHardPts,	// if true all vertices of line segments will be duplicated, to create hard edges for 3d extrsuion
 	GShell &s,		// the output <c GShell> ("2d"), will contain faces with holes
	float &resultMaxLength	// max length of the line 
	)
{ 
	
  float scaleFac = 0.1*height;
  int len;
  
 // #include <tchar.h>
  unsigned char * mbsp;

#ifndef _UNICODE   

  gbool mbcs=gfalse;
  gbool utf8=gtrue;
  
  mbcs = gtrue; utf8=gfalse; 	
  if (utf8) {
	 mbsp = (unsigned char *) text;
	 len = utf8strlen(mbsp);
  } else 	
  if (mbcs) {
	 mbsp = (unsigned char *) text;
	 len = _mbstrlen(text);
  } else 
	  len=_tclen(text);
#else
  gbool mbcs=gfalse;
  gbool utf8=gfalse;
  len=_tclen(text);
#endif

  // float spacing = 1.0;
  float xstart = x;
  
  TEXTMETRIC  tm;
  GetTextMetrics(hDC,&tm);
  //OUTLINETEXTMETRIC otm;
  //GetOutlineTextMetrics(hDC,sizeof(otm),&otm);
  // otmLineGap

  int tstart = 0;

  int vstart = s.v.Length();

  int i=0;
  while (i<len) { 

  // to do : alignment left / right / center
  while (i < len) {

	unsigned int c;
	// get next character 
	if (utf8) {
		c = utf8nextcinc(mbsp);
		if (c == UTF_EOF) break;
		if (c == UTF_INVALID) break;
	} else 
	if (mbcs) {
		c = _mbsnextc(mbsp);
		mbsp= _mbsinc(mbsp);
	} else {
		c =_tcsnextc(text);
	    text =_tcsinc(text);
	}

    if (c == 13) {
		unsigned int cnext;
		if (utf8) {
			c = utf8nextcinc(mbsp);
		} else 
		if (mbcs) {
			cnext = _mbsnextc(mbsp);
		} else {
			cnext =_tcsnextc(text);
		}

		if (cnext == '\n') {} // ignore and process ass /n
		else x = xstart;

		i++; 
		continue; 
	}		
   	else 
	if (c == '\n') {
		i++;
		y+=dirstepy*tm.tmHeight*scaleFac*lineSpacing;
		break;

	}
	else 
#if 0    
    if (c == 10) 
    { x=xstart;	tstart = i + 1; i++; 
    }
    else if (c  == 13) 
    { y-=tm.tmHeight*scaleFac; }
  	else 
#endif

  	{ 
	  //SIZE size;
	  //GetTextExtentPoint32(hDC,&text[i],1,&size);
	  //TRACE("Text extent reports %d %d \n",size.cx,size.cy); is the same as in Gm

  	  ComputeGlyphOutline(hDC,c,scaleFac,x,y,dirstepx,dirstepy,duplicateHardPts,s);

	  i++;
	}
	


  }
  // do after line processing 
  float width = x-xstart;
  float offset;

  if (length > 0.0 && width >0) {
	  
	  float fac = length/width;
	  for (int vi=vstart ; vi<s.v.Length(); vi++) s.v[vi].x = xstart+(s.v[vi].x-xstart)*fac;
	  width = length;
  }	

  if (align <0) offset = 0;
  else if (align >0) offset = - width;
  else offset = - width * 0.5;

  for (int vi=vstart ; vi<s.v.Length(); vi++) s.v[vi].x += offset;
  
  x=xstart;	
  tstart = i;
  vstart = s.v.Length();
  resultMaxLength=max(resultMaxLength,width);

  }
  return(0);
};


#if 0
// qspline.c



// This routine contains the code to convert a quadratic spline into a
// polyline.  This code is taken from our TT rasterizer source code and
// was origially written by the folks at APPLE.
// Modified for the use in GDI by Gunter Zieber [gunterz]   11/30/90


// DO NOT change these constants without understanding implications:
// overflow, out of range, out of memory, quality considerations, etc...
// COPYRIGHT:
//
//   (C) Copyright Microsoft Corp. 1993.  All rights reserved.
//
//   You have a royalty-free right to use, modify, reproduce and
//   distribute the Sample Files (and/or any modified version) in
//   any way you find useful, provided that you agree that
//   Microsoft has no warranty obligations or liability for any
//   Sample Application Files which are modified.
//

#define PIXELSIZE 64 /* number of units per pixel. It has to be a power of two */
#define PIXSHIFT   6 /* should be 2log of PIXELSIZE */
#define ERRDIV     16 /* maximum error is  (pixel/ERRDIV) */
#define ERRSHIFT 4  /* = 2log(ERRDIV), define only if ERRDIV is a power of 2 */
#define ONE 0x40            /* constants for 26.6 arithmetic */
#define HALF 0x20
#define HALFM 0x1f

// The maximum number of vectors a spline segment is broken down into
// is 2 ^ MAXGY
// MAXGY can at most be:
// (31 - (input range to sc_DrawParabola 15 + PIXSHIFT = 21)) / 2

#define MAXGY 5
#define MAXMAXGY 8 /* related to MAXVECTORS */

// RULE OF THUMB: xPoint and yPoints will run out of space when
//		  MAXVECTORS = 176 + ppem/4 (ppem = pixels per EM)
#define MAXVECTORS 257  /* must be at least 257  = (2 ^ MAXMAXGY) + 1  */

typedef signed long	int32;

#if 0
typedef struct _POINTFX {		// definition of 16.16 point structure
    int32   x;
    int32   y;
} POINTFX;
#endif

typedef struct _QS {			// definition of QSpline data points
    POINTFX ptfxStartPt;		// structure
    POINTFX ptfxCntlPt;
    POINTFX ptfxEndPt;
} QS;

typedef QS  far *LPQS;

#if 0
typedef struct	_POINT {
    int x;
    int y;
} POINT;
typedef POINT	far *LPPOINT;

#endif


// int32 _myshr(int32 lval, unsigned int cnt) {
//     while(cnt) {
//	   lval>>=1;
//	   cnt--;
//     }
//     return(lval);
// }
//
// int _ShrAndRound(int32 lval, unsigned int cnt) {
//     lval+=HALF;			   // perform the rounding
//     while(cnt) {
//	   lval>>=1;
//	   cnt--;
//     }
//     return(lval);
// }

/*
 * This function break up a parabola defined by three points (A,B,C) and breaks it
 * up into straight line vectors given a maximium error. The maximum error is
 * 1/resolution * 1/ERRDIV. ERRDIV is defined in sc.h.
 *
 *           
 *         B *-_
 *          /   `-_
 *         /       `-_
 *        /           `-_ 
 *       /               `-_
 *      /                   `* C
 *   A *
 *
 * PARAMETERS:
 *
 * Ax, Ay contains the x and y coordinates for point A. 
 * Bx, By contains the x and y coordinates for point B. 
 * Cx, Cy contains the x and y coordinates for point C.
 * hX, hY are handles to the areas where the straight line vectors are going to be put.
 * count is pointer to a count of how much data has been put into *hX, and *hY.
 *
 * F (t) = (1-t)^2 * A + 2 * t * (1-t) * B + t * t * C, t = 0... 1 =>
 * F (t) = t * t * (A - 2B + C) + t * (2B - 2A) + A  =>
 * F (t) = alfa * t * t + beta * t + A
 * Now say that s goes from 0...N, => t = s/N
 * set: G (s) = N * N * F (s/N)
 * G (s) = s * s * (A - 2B + C) + s * N * 2 * (B - A) + N * N * A
 * => G (0) = N * N * A
 * => G (1) = (A - 2B + C) + N * 2 * (B - A) + G (0)
 * => G (2) = 4 * (A - 2B + C) + N * 4 * (B - A) + G (0) =
 *           3 * (A - 2B + C) + 2 * N * (B - A) + G (1)
 *
 * D (G (0)) = G (1) - G (0) = (A - 2B + C) + 2 * N * (B - A)
 * D (G (1)) = G (2) - G (1) = 3 * (A - 2B + C) + 2 * N * (B - A)
 * DD (G)   = D (G (1)) - D (G (0)) = 2 * (A - 2B + C)
 * Also, error = DD (G) / 8 .
 * Also, a subdivided DD = old DD/4.
 */

// int sc_DrawParabola (F26Dot6 Ax,F26Dot6 Ay,F26Dot6 Bx,F26Dot6 By,F26Dot6 Cx,F26Dot6 Cy,F26Dot6 **hX,F26Dot6 **hY,unsigned *count,int inGY)

//#define NEAR		    _near
//#define PASCAL		    _pascal

int NEAR PASCAL QSpline2Polyline(LPPOINT lpptBuffer, LPQS lpqsPoints, 
                      int inGY, unsigned int far *count, int nAscent)
{
  int32 Ax, Ay, Bx, By, Cx, Cy;
  int32 GX, GY, DX, DY, DDX, DDY, nsqs; 	/*F26Dot6*/
  int32 tmp, i; 				/*F26Dot6*/
  QS	qs;

//  start out by converting our 16.16 numbers to 26.6 numbers.	This can be
//  done safely since the numbers we are converting started out as 26.6
//  values at some point.

    if(inGY < 0) {
	Ax=lpqsPoints->ptfxStartPt.x >> 10;
	Ay=lpqsPoints->ptfxStartPt.y >> 10;
	Bx=lpqsPoints->ptfxCntlPt.x >> 10;
	By=lpqsPoints->ptfxCntlPt.y >> 10;
	Cx=lpqsPoints->ptfxEndPt.x >> 10;
	Cy=lpqsPoints->ptfxEndPt.y >> 10;
    }
    else {
	Ax=lpqsPoints->ptfxStartPt.x;
	Ay=lpqsPoints->ptfxStartPt.y;
	Bx=lpqsPoints->ptfxCntlPt.x;
	By=lpqsPoints->ptfxCntlPt.y;
	Cx=lpqsPoints->ptfxEndPt.x;
	Cy=lpqsPoints->ptfxEndPt.y;
    }

/* Start calculating the first and 2nd order differences */
  GX  = Bx;
  DDX = (DX = (Ax - GX)) - GX + Cx; /* = alfa-x = half of ddx, DX = Ax - Bx */
  GY  = By; /* GY = By */
  DDY = (DY = (Ay - GY)) - GY + Cy; /* = alfa-y = half of ddx, DY = Ay - By */
/* The calculation is not finished but these intermediate results are useful */

  if (inGY < 0) {
/* calculate amount of steps necessary = 1 << GY */
/* calculate the error, GX and GY used a temporaries */
    GX  = DDX < 0 ? -DDX : DDX;
    GY  = DDY < 0 ? -DDY : DDY;
/* approximate GX = sqrt (ddx * ddx + ddy * ddy) = Euclididan distance, DDX = ddx/2 here */
    GX += GX > GY ? GX + GY : GY + GY; /* GX = 2*distance = error = GX/8 */

/* error = GX/8, but since GY = 1 below, error = GX/8/4 = GX >> 5, => GX = error << 5 */

    for (GY=1; GX > (PIXELSIZE << (5-ERRSHIFT)); GX>>=2) // GX = GX >> 2
    {
	GY++; /* GY used for temporary purposes */
    }
/* Now GY contains the amount of subdivisions necessary, number of vectors == (1 << GY)*/
      if (GY > MAXMAXGY) 
	GY = MAXMAXGY; /* Out of range => Set to maximum possible. */
      i = 1 << GY;
      if ((*count = *count + (unsigned int)i)  > MAXVECTORS)
      {
/* Overflow, not enough space => return */
	return (1);
      }
    } 
else {
  GY = inGY;
  i = 1 << GY;
}

if (GY > MAXGY) 
{
  DDX = GY - 1; /* DDX used as a temporary */
/* Subdivide, this is nummerically stable. */
#define MIDX GX
#define MIDY GY
  qs.ptfxStartPt.x=Ax;
  qs.ptfxStartPt.y=Ay;
  qs.ptfxCntlPt.x =((long) Ax + Bx + 1) >> 1;
  qs.ptfxCntlPt.y =((long) Ay + By + 1) >> 1;
  qs.ptfxEndPt.x  =((long) Ax + Bx + Bx + Cx + 2) >> 2;
  qs.ptfxEndPt.y  =((long) Ay + By + By + Cy + 2) >> 2;
  QSpline2Polyline(lpptBuffer, (LPQS)&qs, (int)DDX, count, nAscent);
  qs.ptfxStartPt.x=qs.ptfxEndPt.x;
  qs.ptfxStartPt.y=qs.ptfxEndPt.y;
  qs.ptfxCntlPt.x =((long) Cx + Bx + 1) >> 1;
  qs.ptfxCntlPt.y =((long) Cy + By + 1) >> 1;
  qs.ptfxEndPt.x  =Cx;
  qs.ptfxEndPt.y  =Cy;
  QSpline2Polyline(lpptBuffer, (LPQS)&qs, (int)DDX, count, nAscent);
  return 0;
}

nsqs = GY + GY; /* GY = n shift, nsqs = n*n shift */

/* Finish calculations of 1st and 2nd order differences */
DX   = DDX - (DX << ++GY); /* alfa + beta * n */
DDX += DDX;
DY   = DDY - (DY <<   GY);
DDY += DDY;

GY = (long) Ay << nsqs; /*  Ay * (n*n) */
GX = (long) Ax << nsqs; /*  Ax * (n*n) */
/* GX and GY used for real now */

/* OK, now we have the 1st and 2nd order differences,
       so we go ahead and do the forward differencing loop. */
tmp = 1L << (nsqs-1);
do {
  GX += DX;  /* Add first order difference to x coordinate */
  lpptBuffer->x=(int)((((GX + tmp) >> nsqs)+0x3) >> 3);
  DX += DDX; /* Add 2nd order difference to first order difference. */
  GY += DY;  /* Do the same thing for y. */

#if 1
  lpptBuffer->y=((int)((((GY + tmp) >> nsqs)+0x3) >> 3));
#else
  lpptBuffer->y=nAscent-((int)((((GY + tmp) >> nsqs)+0x3) >> 3));
#endif

  lpptBuffer->y >>= 3;
  lpptBuffer->x >>= 3;

  DY += DDY;
  lpptBuffer++;
} while (--i);
return 0;
  }
#undef MIDX
#undef MIDY

#endif
#endif
