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

#ifndef _GPanel_h
#define _GPanel_h


#ifdef _D3D

class GD3Device;

#endif



#include "image.h"
#include "grender.h"

#define tan30 .577
#define tan60  1.732
#define tanMin30 -.577
#define tanMin60 -1.732

#define GAP 3

#define NAV_PANEL_HEIGHT 48 
#define NAV_PANEL_MIN_X 600
#define NAV_PANEL_UP	0x1
#define NAV_PANEL_DOWN	0x2
#define NAV_PANEL_LEFT	0x4
#define NAV_PANEL_RIGHT 0x8
#define NAV_PANEL_SLIDE 0x10

#define PANEL_SPHERE0 0 
#define PANEL_SPHERE1 1 
#define PANEL_SPHERE2 2 
#define PANEL_SPHERE3 3 
#define PANEL_SPHERE4 4 
#define PANEL_SPHERE5 5 
#define PANEL_SPHERE6 6 
#define PANEL_SPHERE7 7
#define PANEL_SLIDE_RIGHT 8
#define PANEL_SLIDE_LEFT 9
#define PANEL_HELP 10
#define PANEL_CLOSE 11
#define PANEL_VIEW_RESET 12
#define PANEL_VIEW_PREV 13
#define PANEL_VIEW_NEXT 14

#define PANEL_LBUTTONDOWN 1
#define PANEL_MOUSEMOVE_DOWN 2 
#define PANEL_MOUSEMOVE_OVER 3
#define PANEL_LBUTTONUP 4

#define D3DTEXTR_TRANSPARENTWHITE 0x00000001
#define D3DTEXTR_TRANSPARENTBLACK 0x00000002
#define D3DTEXTR_32BITSPERPIXEL   0x00000004


// id for the different panel bitmaps 
typedef enum {
	GPANEL_DEFAULT	= 0 ,
	GPANEL_OVER		= 1 ,
	GPANEL_PRESSED	= 2,

	GPANEL_CURRENT	= 3
} GPanelState;


//
// GPanel -- 
//
class CMyglView;

class GPanel {

public : 	
	void GetCenterPos(POINT *point);

	BOOL InsideSphere(CPoint point);
	GPanel(CMyglView *newpView);

	~GPanel();


	void GetHelpButPos(CPoint *point);
	//the ctrl needs this position in order to place the popup menu


	int OnMouseAction(CPoint point, int actionFlag);
	//implements the button behaviour
	//according to the point and the actionFlag
	//buttons are highlighted etc
	//as a result the method changes the state-variable bmNumber
	//of the panel	

	//called by NavCheckCursor
	//used for feedback
	void setNavigationMode(int mode);

	//called by NavMain()
	//used for feedback
	void setNavigationDirection(float tileX, float tileY);

	//only if the panel is not active, the ctrl asks the
	//panel to give feedback
	//is set in onMouseAction and reseted in Inside()
	//isActive means that the bmNumber was by onMouseAction and
	//not by the feedback-module
	BOOL Active() const {return isActive;};


	// read panel bitmaps
	gbool InitPanelBitmaps();

	// is point inside rectangle of panel ?
	BOOL Inside(POINT point);

	// set the new dimension of the screen, call at resize
	//sets  screenRect  destRect  screenToSmall
	void SetScreenRect(int width,int height);

	// is panel currently visible and enabled ?
	gbool Enabled() const { return enabled;}
	
	//gbool ScreenToSmall() const { return screenToSmall;}

	gbool Ok() const { return !screenToSmall;}
		
	void enable();
	void disable();//puts the panel in a passiv state, i.e.
	//the panel gives only feedback

	BOOL Minimized() const { return minimized ;}
	
	void maximize();
	void minimize();
	//called by the closeButton and the ctrl in case of fly-mode etc



#ifdef _D3D
	HRESULT Blt(LPDIRECTDRAWSURFACE targetSurf);

	// initiallize the DD panel surfaces for BLT 
	HRESULT Init(GD3Device *device);

	void Term();

#endif

#ifdef _OGL
	
	HRESULT Blt();
	HRESULT Init();

	glCache *cache;
	void Term();

#endif

private:

#ifdef _MFC
	CBitmap **m_BitmapArray;
	HBITMAP hBitmap, hBitmap1;
#endif


	// panel is enabled
	gbool enabled;	
	
	BOOL isActive;//if a button is hit
	//the panel gives only feedback if it is inactive

	BOOL minimized;

	//BOOL initialized;
	//in the first loop no blt is done in order to avoid the panel in the default.wrl

	void UpdatePanel(int srcBitmap, int xstart,int ystart,int width,int height);

	// current screen size is to small for panel
	gbool screenToSmall;	
	
	CRect screenRect; // rect of rendering screen
	CRect srcRect;	// src rect of bitmap
	CRect destRect; // dest rect on target surface (screen buffer)


#ifdef _MFC
	CMyglView *pView;
#endif


	int navigationDirection, navigationMode;
	void setBitmap(int mode, int newDirection);
	
	//****************************************************
	int bmNumber, oldbmNumber;//represents the state of the whole panel

	CRect rectSphere, rectViewP, rectViewR, rectViewN, rectClose, rectSlideL, rectSlideR, rectHelp;

#ifdef _OGL

#ifdef _MFC
	BYTE* TranslateByGDI(CBitmap *newCBitmap);
#endif

	BYTE *m_OGLBitmapArray[43];
#endif _OGL


#ifdef _D3D
	GD3Device *device;		// Backpointer to device to get DD & D3D surfaces

	LPDIRECTDRAWSURFACE     panelSurf;
	// LPDIRECTDRAWSURFACE     pddsFrontBuffer;
	LPDIRECTDRAWSURFACE     panelSurfSphere[43];

#endif _D3D
};



#endif // _GPanel_h

