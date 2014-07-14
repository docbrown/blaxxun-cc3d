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

@module mainfrm.cpp - GLView MFC CMainFrame class|

Copyright (c) 1995	by Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:
GLView toolbars are initialized here.
OnQueryNewPalette & OnPaletteChanged are handled here and are passed to active view.

Changes:

Todo :

******************************************************************************/


#include "stdafx.h"
#include "glviewhdr.h"


#include "mainfrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

//Updating the Application Status Bar on Demand
//Article ID: Q98473

void Message(const char *message)
{
//  CFrameWnd *pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);
  CMainFrame *pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
  if (pFrame)  {
     pFrame->SetMessageText(message);
	 //pFrame->m_wndStatusBar.SendMessage(WM_IDLEUPDATECMDUI);
   	 pFrame->m_wndStatusBar.UpdateWindow();
	 //pFrame->UpdateWindow();
  }
}

void Error(const char *message)
{
  Message(message);
}



/////////////////////////////////////////////////////////////////////////////
// CMainFrame
// HG Can replace CFrameWnd to CMDIFramwWnd
IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_PALETTECHANGED()
	ON_WM_QUERYNEWPALETTE()
	ON_WM_CLOSE()
    ON_CBN_SELCHANGE(IDW_URL, OnSelChangeUrl)
	ON_COMMAND(ID_VIEW_URL_BAR, OnViewUrlBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_URL_BAR, OnUpdateViewUrlBar)
	ON_WM_ACTIVATE()
	ON_WM_ACTIVATEAPP()
	ON_WM_MOVE()
	ON_WM_SIZE()
	ON_COMMAND(ID_TOOLS_LIBRARY, OnToolsLibrary)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_LIBRARY, OnUpdateToolsLibrary)
	//}}AFX_MSG_MAP

	ON_MESSAGE(WM_SIZING, OnSizing) 
	ON_MESSAGE(WM_MOVING, OnMoving) 

	// Global help commands
	ON_COMMAND(ID_HELP_INDEX, CFrameWnd::OnHelpIndex)
	ON_COMMAND(ID_HELP_USING, CFrameWnd::OnHelpUsing)
	ON_COMMAND(ID_HELP, CFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CFrameWnd::OnHelpIndex)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// arrays of IDs used to initialize control bars

// toolbar buttons - IDs are command buttons
static UINT BASED_CODE buttons[] =
{
	// same order as in the bitmap 'toolbar.bmp'
	ID_FILE_READ,
	ID_APP_ABOUT,
		ID_SEPARATOR,
	ID_RENDER_WIREFRAME,
	ID_RENDER_FLAT,
	ID_RENDER_GOURAUD,
	ID_RENDER_TEXTURED,
		ID_SEPARATOR,
	ID_CAMERA_ORBIT,
	ID_CAMERA_ZOOM,
	ID_CAMERA_DOLLY_XY,
	ID_CAMERA_PAN,
	ID_CAMERA_WALK,
	ID_CAMERA_RESET,
		ID_SEPARATOR,
	ID_RENDER_ANIMATE,
		ID_SEPARATOR,
	ID_WWW_ABORTLOAD,
		ID_SEPARATOR,
	ID_TOOLS_TREE_EDITOR,
	ID_TOOLS_TRANSFORM

};

static UINT BASED_CODE indicators[] =
{
	ID_SEPARATOR,			// status line indicator
/*	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL, */
};

static UINT BASED_CODE urlButtons[] =
{
	// same order as in the bitmap 'styles.bmp'
	ID_SEPARATOR,           // for combo box (placeholder)
	ID_SEPARATOR,
	ID_WWW_ABORTLOAD
};

/////////////////////////////////////////////////////////////////////////////
// Helpers for saving/restoring window state

static TCHAR BASED_CODE szSection[] = _T("Settings");
static TCHAR BASED_CODE szWindowPos[] = _T("WindowPos");
static TCHAR szFormat[] = _T("%u,%u,%d,%d,%d,%d,%d,%d,%d,%d");

static BOOL PASCAL NEAR ReadWindowPlacement(LPWINDOWPLACEMENT pwp)
{
	CString strBuffer = AfxGetApp()->GetProfileString(szSection, szWindowPos);
	if (strBuffer.IsEmpty())
		return FALSE;

	WINDOWPLACEMENT wp;
	int nRead = _stscanf(strBuffer, szFormat,
		&wp.flags, &wp.showCmd,
		&wp.ptMinPosition.x, &wp.ptMinPosition.y,
		&wp.ptMaxPosition.x, &wp.ptMaxPosition.y,
		&wp.rcNormalPosition.left, &wp.rcNormalPosition.top,
		&wp.rcNormalPosition.right, &wp.rcNormalPosition.bottom);

	if (nRead != 10)
		return FALSE;

	wp.length = sizeof wp;
	*pwp = wp;
	return TRUE;
}

static void PASCAL NEAR WriteWindowPlacement(LPWINDOWPLACEMENT pwp)
	// write a window placement to settings section of app's ini file
{
	TCHAR szBuffer[sizeof("-32767")*8 + sizeof("65535")*2];

	wsprintf(szBuffer, szFormat,
		pwp->flags, pwp->showCmd,
		pwp->ptMinPosition.x, pwp->ptMinPosition.y,
		pwp->ptMaxPosition.x, pwp->ptMaxPosition.y,
		pwp->rcNormalPosition.left, pwp->rcNormalPosition.top,
		pwp->rcNormalPosition.right, pwp->rcNormalPosition.bottom);
	AfxGetApp()->WriteProfileString(szSection, szWindowPos, szBuffer);
}



/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
    m_saveWindowSize = TRUE;
    m_active = FALSE; // frame is active
    m_minimized = FALSE; // frame is minimized 
}

CMainFrame::~CMainFrame()
{
}

// add url to combo list box
int CMainFrame::AddUrl(const char *url)
{
		m_wndUrlBar.m_url.AddString(url);
      return(0);
}


BOOL CMainFrame::CreateUrlBar()
{
	const int nDropHeight = 100;


	if (!m_wndUrlBar.Create(this,IDD_URL_BAR, WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_SIZE_DYNAMIC |
			CBRS_TOOLTIPS|CBRS_FLYBY, ID_VIEW_URL_BAR))
	{
		TRACE0("Failed to create UrlBar\n");
		return FALSE;       // fail to create
	}

	m_wndUrlBar.UpdateData(FALSE);   // initialize controls (DDX)

   
   // open the glview animation
   CString aviPath;
   aviPath = AfxGetApp()->m_pszHelpFilePath;
   aviPath = aviPath.Left(aviPath.ReverseFind('.'));
   aviPath += ".avi";



   if (!m_wndUrlBar.m_animate.Open(aviPath)) 
   if (!m_wndUrlBar.m_animate.Open(IDR_GLVIEW_AVI)) 
   {
      TRACE("Couldn´t open the glview avi file \n");

   }


	return TRUE;
}

void CMainFrame::OnSelChangeUrl()
{
	CString strText;
	CString strItem;
	CComboBox* pCBox = (CComboBox*)m_wndUrlBar.GetDlgItem(IDW_URL);
	int nIndex = pCBox->GetCurSel();
	if (nIndex == CB_ERR)
		return;
	pCBox->GetLBText(nIndex, strItem);

   //AfxFormatString1(strText, IDS_SELECTED_PROMPT, (LPCTSTR)strItem);
   
   // TRACE("On Sel Change URL %s \n",(const char *) strItem);

	SetMessageText(strItem);
}



int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
      return -1;

#if 0
   
   m_saveWindowSize = AfxGetApp()-> GetProfileInt("GLView","saveWindowSize",1);

   if (m_saveWindowSize) {
      WINDOWPLACEMENT wp;
	   if (ReadWindowPlacement(&wp)) {

         SetWindowPlacement(&wp);
      }  
    
   }
#endif



  	EnableDocking(CBRS_ALIGN_ANY); // 4.0


	if (!m_wndToolBar.Create(this,CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY|WS_VISIBLE) ||
		!m_wndToolBar.LoadBitmap(IDR_MAINFRAME) ||
		!m_wndToolBar.SetButtons(buttons,
		  sizeof(buttons)/sizeof(UINT)))
	{
		TRACE("Failed to create toolbar\n");
		return -1;		// fail to create
	}

   if (!CreateUrlBar()) {
	   TRACE("Failed to create url \n");
		return -1;		// fail to create

   }


	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE("Failed to create status bar\n");
		return -1;		// fail to create
	}
   // new 4.0
   UINT nID, nStyle;
	int cxWidth;

	m_wndStatusBar.GetPaneInfo( 0, nID, nStyle, cxWidth);
	m_wndStatusBar.SetPaneInfo( 0, nID, SBPS_STRETCH|SBPS_NORMAL, cxWidth);
   //
	// New VC 2.0
	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);

	// TODO: Remove this if you don't want tool tips
	//m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
	//	CBRS_TOOLTIPS | CBRS_FLYBY);
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers




void CMainFrame::OnPaletteChanged(CWnd* pFocusWnd) 
{
//CFrameWnd::OnPaletteChanged(pFocusWnd);
// HG 10.08.95 from easygl MSDN 07/95	
    CView* pView = GetActiveView();
    if (pView) {
        // OnPaletteChanged is not public, so send a message.
        pView->SendMessage(WM_PALETTECHANGED,
                           (WPARAM)(pFocusWnd->GetSafeHwnd()),
                           (LPARAM)0);
    }

}

BOOL CMainFrame::OnQueryNewPalette() 
{
// HG 10.08.95 from easygl MSDN 07/95	
    CView* pView = GetActiveView();
    if (pView) {
        // OnQueryNewPalette is not public, so send a message.
        return pView->SendMessage(WM_QUERYNEWPALETTE,
                                  (WPARAM)0,
                                  (LPARAM)0);
    }
    return FALSE; 
	
//	return CFrameWnd::OnQueryNewPalette();
}

/*	to change window size */
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs) 
{	// to do: add border size 

	int wx = AfxGetApp()->GetProfileInt("GLView","WinSizeX",0);
	int wy = AfxGetApp()->GetProfileInt("GLView","WinSizeY",0);

	if (wx > 2048) cs.style |= WS_MAXIMIZE;
	else {		  // cx cy normaly 0 on entry 
		if (wx>=320) cs.cx = wx;
		if (wy>=240) cs.cy = wy;	
	}
	return CFrameWnd::PreCreateWindow(cs);
}


void CMainFrame::OnClose() 
{

	// TO DO : kill all Threads ???
   if (m_saveWindowSize) {
   
   WINDOWPLACEMENT wp;
   wp.length = sizeof wp;

   if (!IsIconic() && !IsZoomed()) 
   if (GetWindowPlacement(&wp))
	{
		wp.flags = 0;

   	int wx = wp.rcNormalPosition.right-wp.rcNormalPosition.left+1;
    AfxGetApp()->WriteProfileInt("GLView","WinSizeX",wx);
   	int wy = wp.rcNormalPosition.bottom-wp.rcNormalPosition.top+1;
	   
      AfxGetApp()->WriteProfileInt("GLView","WinSizeY",wy);

#if 0

		// HG  ?? if (IsZoomed())		wp.flags |=  WPF_RESTORETOMAXIMIZED;
		// and write it to the .INI file
      if (wp.ptMaxPosition.x == wp.rcNormalPosition.top) {
         // is really maximized 
         wp.showCmd = SW_SHOWMAXIMIZED;
      }
       
      //GetWindowRect
      else 
         WriteWindowPlacement(&wp);
#endif

   }
   }
	
   // really exectuted on exit 
	
	CFrameWnd::OnClose();
}

// WM_MOVING
afx_msg LRESULT CMainFrame::OnMoving(WPARAM wParam, LPARAM lParam)
{
	return(TRUE);
}

//WM_SIZING
afx_msg LRESULT CMainFrame::OnSizing(WPARAM wParam, LPARAM lParam)
{
	return(TRUE);
}


/////////////////////////////////////////////////////////////////////////////
// CUrlBox

CUrlBox::CUrlBox()
{
}

CUrlBox::~CUrlBox()
{
}

BEGIN_MESSAGE_MAP(CUrlBox, CComboBox)
	//{{AFX_MSG_MAP(CUrlBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUrlBox message handlers

BOOL CUrlBox::PreTranslateMessage(MSG* pMsg)
{
	if ((pMsg->message != WM_KEYDOWN))
		return CComboBox::PreTranslateMessage(pMsg);

    if (pMsg->wParam != VK_RETURN)
       return CComboBox::PreTranslateMessage(pMsg);

	// when the enter key is hit in the ComboBox we want to add the string
	// to the top of the list and hilight it.  We also want to limit the
	// list to the last 15 entries.
	if ((pMsg->lParam & 0x40000000) == 0)   // Not a repeat.
	{
		CString strText;
		GetWindowText(strText);

		InsertString(0, strText);
		SetCurSel(0);
      
      m_curSel = 0;

		if (GetCount() > 150)
			DeleteString(GetCount()-1);


      CMyglView* pView = ((CMyglApp *) AfxGetApp())->GetActiveView();
      if (pView) { pView->ReadUrl( strText); }

	}
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CUrlBar message handlers


BEGIN_MESSAGE_MAP(CUrlBar, CDialogBar)
	//{{AFX_MSG_MAP(CUrlBar)
	ON_WM_CHAR()
	ON_WM_CREATE()
	ON_CBN_SELCHANGE(IDC_URL, OnSelchangeUrl)
	ON_CBN_DROPDOWN(IDC_URL, OnDropdownUrl)
	ON_WM_SIZE()
	ON_CBN_DBLCLK(IDC_URL, OnDblclkUrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CEditBar message handlers



void CUrlBar::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialogBar::OnChar(nChar, nRepCnt, nFlags);
}

int CUrlBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;
   
   m_visible = 1;
	
	// TODO: Add your specialized creation code here
   
	return 0;
}

void CUrlBar::OnSelchangeUrl() 
{
	// TODO: Add your control notification handler code here
#if 0
   CString strText;
	CString strItem;
	CComboBox* pCBox = (CComboBox*)GetDlgItem(IDC_URL);
	int nIndex = pCBox->GetCurSel();
	if (nIndex == CB_ERR)
		return;
	pCBox->GetLBText(nIndex, strItem);
	//AfxFormatString1(strText, IDS_SELECTED_PROMPT, (LPCTSTR)strItem);
   
   TRACE("On Sel Change URL %s \n",(const char *) strItem);
#endif
  
   CString url;   

   int nIndex = m_url.GetCurSel();
   if (nIndex == CB_ERR) 
         return;
   
   m_url.m_curSel = nIndex;
   m_url.GetText(nIndex,url);

   CMyglView* pView = ((CMyglApp *) AfxGetApp())->GetActiveView();
   if (pView) { pView->ReadUrl( url); }




//	SetMessageText(strItem);

   
}

void CUrlBar::OnDropdownUrl() 
{
	// TODO: Add your control notification handler code here
	
}

void CUrlBar::OnSize(UINT nType, int cx, int cy) 
{
	CDialogBar::OnSize(nType, cx, cy);
   if ((cx >0) && (cy >0)) {

   // attach controls
   CRect box;
   CRect urlBox;
   CRect animateBox;

   if ( ::IsWindow(m_url.m_hWnd)) { // controls are instanced

      
   GetClientRect(&box);
   m_url.GetWindowRect(&urlBox);
   
   m_url.GetDroppedControlRect( &urlBox);
  

   ScreenToClient(&urlBox);
   m_animate.GetWindowRect(&animateBox);
   
   ScreenToClient(&animateBox);

   // move icon left
   int d = animateBox.right-animateBox.left;

   animateBox.right = box.right-4;
   animateBox.left = animateBox.right - d;

   // stretch URL box
   urlBox.right = animateBox.left - 8;
   
   m_animate.MoveWindow(&animateBox,TRUE);
   m_url.MoveWindow(&urlBox,TRUE);

   }
  
   }

}

void CUrlBar::DoDataExchange(CDataExchange* pDX) 
{
	
	CDialogBar::DoDataExchange(pDX);

   //{{AFX_DATA_MAP(CUrlBar)
	DDX_Control(pDX, IDC_ANIMATE, m_animate);
	DDX_Control(pDX, IDC_URL, m_url);
	//}}AFX_DATA_MAP

}

void CUrlBar::OnDblclkUrl() 
{
	// TODO: Add your control notification handler code here
	
}

// example for poupup

#if 0

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_RBUTTONDOWN)
	{
		CWnd* pWnd = CWnd::FromHandlePermanent(pMsg->hwnd);
		CControlBar* pBar = DYNAMIC_DOWNCAST(CControlBar, pWnd);

		if (pBar != NULL)
		 {
			CMenu Menu;
			CPoint pt;

			pt.x = LOWORD(pMsg->lParam);
			pt.y = HIWORD(pMsg->lParam);
			pBar->ClientToScreen(&pt);

			if (Menu.LoadMenu(IDR_TOOLBAR))
			{
				CMenu* pSubMenu = Menu.GetSubMenu(0);

				if (pSubMenu!=NULL)
				{
					pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,pt.x,pt.y,this);
				}
			}
		}
	}
	return CFrameWnd::PreTranslateMessage(pMsg);
}


#endif

// toggle  visibility of UrlBar
void CMainFrame::OnViewUrlBar() 
{
 int m_bUrlBar = ((m_wndUrlBar.GetStyle() & WS_VISIBLE) != 0);
 
 m_bUrlBar = !m_bUrlBar;

 ShowControlBar( &m_wndUrlBar, m_bUrlBar, FALSE );

 m_wndUrlBar.m_visible = ((m_wndUrlBar.GetStyle() & WS_VISIBLE) != 0);

 if (!m_wndUrlBar.m_visible) m_wndUrlBar.StopPlay();
 	
}

void CMainFrame::OnUpdateViewUrlBar(CCmdUI* pCmdUI) 
{

 //m_wndUrlBar.m_visible = ((m_wndUrlBar.GetStyle() & WS_VISIBLE) != 0);
 
 pCmdUI->SetCheck(m_wndUrlBar.m_visible);

}

#if 0
// from speaker

#include <mmsystem.h>

/////////////////////////////////////////////////////////////////////////////
// Sound helpers

static void PlaySound(LPCTSTR lpszSound)
{
	HRSRC hRes; // resource handle to wave file
	HGLOBAL hData;
	BOOL bOk = FALSE;
	if ((hRes = ::FindResource(AfxGetResourceHandle(), lpszSound,
	  _T("sound"))) != NULL &&
	  (hData = ::LoadResource(AfxGetResourceHandle(), hRes)) != NULL)
	{
		// found the resource, play it
		bOk = sndPlaySound((LPCTSTR)::LockResource(hData),
			SND_MEMORY|SND_SYNC|SND_NODEFAULT);
		FreeResource(hData);
	}
	if (!bOk)
	{
		static BOOL bReported = FALSE;
		if (!bReported)
		{
			AfxMessageBox(IDS_CANNOT_PLAY_SOUND);
			bReported = TRUE;       // once please
		}
	}
}

inline static void PlaySound(UINT nIDS)
	{ PlaySound(MAKEINTRESOURCE(nIDS)); }


	// load the bitmaps for bitmap buttons
	VERIFY(m_replayButton.AutoLoad(IDC_REPLAY_SOUND, this));
	InputEdit().ShowWindow(FALSE);      // start with input disabled


   CBitmapButton   m_pictureButton; // main bitmap done as a special button
	
	// load initial picture
	VERIFY(m_pictureButton.SubclassDlgItem(IDC_PICTURE, this));
	VERIFY(m_pictureButton.LoadBitmaps(_T("intro"), NULL, NULL));



	// draw the picture (bitmap with the same name as the target)
	if (!m_pictureButton.LoadBitmaps(m_targetRes))
	{
		AfxMessageBox(IDS_PICTURE_UNAVAILABLE);
		VERIFY(m_pictureButton.LoadBitmaps(_T("intro"), NULL, NULL));
			// go back to the initial bitmap
	}
	m_pictureButton.Invalidate(TRUE);


	HICON hNew = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(nIDI));
	ASSERT(hNew != NULL);
	::DestroyIcon(StatusFace().SetIcon(hNew));
	UpdateWindow();                 // draw everything now

#endif


void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CFrameWnd::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: Add your message handler code here
	
}

void CMainFrame::OnActivateApp(BOOL bActive, HTASK hTask) 
{
	CFrameWnd::OnActivateApp(bActive, hTask);

	m_active = bActive;
	
	// TODO: Add your message handler code here
	
}

void CMainFrame::OnMove(int x, int y) 
{
	CFrameWnd::OnMove(x, y);
	// D3D needs to be informed on moves 
    CView* pView = GetActiveView();
    if (pView) {
        pView->SendMessage(WM_MOVE,(WPARAM)0,(LPARAM)0);
    }
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	
    m_minimized =  (nType == SIZE_MINIMIZED);

	// D3D needs to be informed on moves 
    CView* pView = GetActiveView();
    if (pView) {
//        pView->SendMessage(WM_SIZE,(WPARAM)nType,  (LPARAM)0);
    }
}
/////////////////////////////////////////////////////////////////////////////
// CThanksTo dialog


CThanksTo::CThanksTo(CWnd* pParent /*=NULL*/)
	: CDialog(CThanksTo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CThanksTo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CThanksTo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CThanksTo)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CThanksTo, CDialog)
	//{{AFX_MSG_MAP(CThanksTo)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThanksTo message handlers

void CMainFrame::OnToolsLibrary() 
{
	// TODO: Add your command handler code here
	
}

void CMainFrame::OnUpdateToolsLibrary(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}
