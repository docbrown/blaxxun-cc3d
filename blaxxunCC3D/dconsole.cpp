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
// DConsole.cpp : implementation file
//



//RESIZING: 
// Changes I have done to enable resizing:
//
// - inserted members CButton m_BtnCopy, m_BtnClear, m_BtnHide, m_BtnClose
// - renamed Edit box m_text to m_EdtText, to get consistent naming scheme.
// - added DConsole::OnSize(.) to do the resizing.
// - added members m_Dist_*_* to contain the distances between the controlls.
// - added DConsole::InitDistances() to initialize these members.
//   this is called in the first processing of WM_PAINT message, since I don't
//   know any better way. PLEASE tell me one.
// - added member m_DistancesInited;












#include "stdafx.h"

#include "resource.h"

#include "blaxxuncc3dCtl.h"

#include "DConsole.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DConsole dialog


DConsole::DConsole(CWnd* pParent /*=NULL*/)
	: CDialog(DConsole::IDD, pParent)
{
	m_hide = FALSE;
	//{{AFX_DATA_INIT(DConsole)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

    m_DistancesInited= FALSE;
    m_Dist_LeftBorder_BtnCopy=
        m_Dist_BtnCopy_BtnClear=
        m_Dist_BtnHide_BtnClose=
        m_Dist_BtnClose_RightBorder=
        m_Dist_EdtText_Btns=
        m_Dist_EdtText_RightBorder=
        m_Dist_Btns_BottomBorder=     10;  // some default value in case initializing doesn't work.

}


void DConsole::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DConsole)
	DDX_Control(pDX, IDOK, m_BtnClose);
	DDX_Control(pDX, IDHIDE, m_BtnHide);
	DDX_Control(pDX, IDC_COPY, m_BtnCopy);
	DDX_Control(pDX, IDC_CLEAR, m_BtnClear);
	DDX_Control(pDX, IDC_TEXT, m_EdtText);
	//}}AFX_DATA_MAP
   

    if(!pDX->m_bSaveAndValidate && !m_DistancesInited)
    {
        InitDistances();
        m_DistancesInited= TRUE;
    }



}


BEGIN_MESSAGE_MAP(DConsole, CDialog)
	//{{AFX_MSG_MAP(DConsole)
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	ON_BN_CLICKED(IDC_COPY, OnCopy)
	ON_BN_CLICKED(IDHIDE, OnHide)
	ON_EN_ERRSPACE(IDC_TEXT, OnErrspaceText)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DConsole message handlers
void DConsole::AddText(const TCHAR *text) 
{
	if (!::IsWindow(m_hWnd))	// savety
		return;

	if (!text)
		return; 	// savety

	CString newText;

	const TCHAR *s=text;

	while (*s) { // replace \n by \r\n
		TCHAR *e = strchr(s,'\n');
	
		if (e) {
			newText += CString(s,e-s);
			s = e+1;
			newText += "\r\n";
		} else {
			newText += s;
			break;
		}
	}
	newText += "\r\n";
	int l=	m_EdtText.GetWindowTextLength();
	m_EdtText.SetSel( l+1, -1); 
	m_EdtText.ReplaceSel(newText); 
	l=	m_EdtText.GetWindowTextLength();
	m_EdtText.SetSel( l+1, -1); 
}

void DConsole::OnOK() 
{
	// TODO: Add extra validation here
	m_hide = TRUE; // new 
	
	CDialog::OnOK();
}

void DConsole::OnClear() 
{
	int l=	m_EdtText.GetWindowTextLength();

	m_EdtText.SetSel(0,-1); 
	m_EdtText.ReplaceSel(""); 
	m_EdtText.Clear();
	l=	m_EdtText.GetWindowTextLength();
	m_EdtText.SetSel( l+1, -1); 
}

void DConsole::OnCopy() 
{
	int l=	m_EdtText.GetWindowTextLength();

	int nStartChar, nEndChar;

	m_EdtText.GetSel(nStartChar,nEndChar );
	if (nEndChar <= nStartChar) {
		m_EdtText.SetSel(0,-1,TRUE); 
		m_EdtText.Copy();
		m_EdtText.SetSel( l+1, -1); 
	}
	else // use current selection  
		m_EdtText.Copy();
}

void DConsole::OnHide() 
{
	m_hide = TRUE;
	CDialog::OnOK();

}
void DConsole::OnErrspaceText() 
{
	// TODO: Add your control notification handler code here
	OnClear();
	
}


LRESULT DConsole::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); // hg 11.5.99 but already done in AfxWndProcDllStatic

	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL DConsole::OnInitDialog() 
{
	return CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	// return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void DConsole::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

    // This will rearrange buttons and the text box to the new window size.

    if(!m_DistancesInited)
return;


    RECT rText, rCopy, rClear, rHide, rClose, rThis;
    
    m_EdtText.GetWindowRect(&rText);
    m_BtnCopy.GetWindowRect(&rCopy);
    m_BtnClear.GetWindowRect(&rClear);
    m_BtnHide.GetWindowRect(&rHide);
    m_BtnClose.GetWindowRect(&rClose);
    GetClientRect(&rThis);

    ScreenToClient(&rText);
    ScreenToClient(&rCopy);
    ScreenToClient(&rClear);
    ScreenToClient(&rHide);
    ScreenToClient(&rClose);

    // From now on, .bottom and .right have the meaning of width and height.
    rText.right-=   rText.left;
    rText.bottom-=  rText.top;
    rCopy.right-=   rCopy.left;
    rCopy.bottom-=  rCopy.top;
    rClear.right-=  rClear.left;
    rClear.bottom-= rClear.top;
    rHide.right-=   rHide.left;
    rHide.bottom-=  rHide.top;
    rClose.right-=  rClose.left;
    rClose.bottom-= rClose.top;


    //TBD: Insert here calculations of r*.top and r*.left.

    rClose.left= rThis.right - m_Dist_BtnClose_RightBorder - rClose.right;
    rHide.left=  rClose.left - m_Dist_BtnHide_BtnClose     - rHide.right;

    rCopy.left=  rThis.left   + m_Dist_LeftBorder_BtnCopy;
    rClear.left= rCopy.left + rCopy.right + m_Dist_BtnCopy_BtnClear;

    rCopy.top= rClear.top= rHide.top= rClose.top= 
        rThis.bottom - m_Dist_Btns_BottomBorder - rCopy.bottom;

    rText.bottom= rCopy.top - m_Dist_EdtText_Btns - rText.top;
    rText.right=  rThis.right - rText.left - m_Dist_EdtText_RightBorder;

     m_EdtText.SetWindowPos(NULL,  rText.left,  rText.top,  rText.right,  rText.bottom, SWP_NOZORDER);
     m_BtnCopy.SetWindowPos(NULL,  rCopy.left,  rCopy.top,  rCopy.right,  rCopy.bottom, SWP_NOZORDER);
    m_BtnClear.SetWindowPos(NULL, rClear.left, rClear.top, rClear.right, rClear.bottom, SWP_NOZORDER);
     m_BtnHide.SetWindowPos(NULL,  rHide.left,  rHide.top,  rHide.right,  rHide.bottom, SWP_NOZORDER);
    m_BtnClose.SetWindowPos(NULL, rClose.left, rClose.top, rClose.right, rClose.bottom, SWP_NOZORDER);


    Invalidate(FALSE);

}



void DConsole::InitDistances()
{

    RECT rText, rCopy, rClear, rHide, rClose, rThis;

    m_EdtText.GetWindowRect(&rText);
    m_BtnCopy.GetWindowRect(&rCopy);
    m_BtnClear.GetWindowRect(&rClear);
    m_BtnHide.GetWindowRect(&rHide);
    m_BtnClose.GetWindowRect(&rClose);
    GetClientRect(&rThis);


    ScreenToClient(&rText);
    ScreenToClient(&rCopy);
    ScreenToClient(&rClear);
    ScreenToClient(&rHide);
    ScreenToClient(&rClose);

    m_Dist_EdtText_Btns=         rCopy.top    - rText.bottom;
    m_Dist_LeftBorder_BtnCopy=   rCopy.left   - rThis.left;
    m_Dist_BtnCopy_BtnClear=     rClear.left  - rCopy.right;
    m_Dist_BtnHide_BtnClose=     rClose.left  - rHide.right;
    m_Dist_BtnClose_RightBorder= rThis.right  - rClose.right;
    m_Dist_Btns_BottomBorder=    rThis.bottom - rCopy.bottom;
    m_Dist_EdtText_RightBorder=  rThis.right  - rText.right;
}


void DConsole::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}

void DConsole::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}
