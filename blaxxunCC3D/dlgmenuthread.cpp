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
#include "DlgMenuThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDlgMenuThread::CDlgMenuThread(CWnd* pParent, CMenu* pMenu, POINT* pPos, CWnd* pTarget)
	: CDialog(CDlgMenuThread::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMenuThread)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pTarget	= pTarget;
	m_pPos		= pPos;
	m_pMenu		= pMenu;
}


void CDlgMenuThread::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMenuThread)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMenuThread, CDialog)
	//{{AFX_MSG_MAP(CDlgMenuThread)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDlgMenuThread::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	if ( bShow )
	{
		MoveWindow(m_pPos->x, m_pPos->y, 1, 1);
		m_timer = SetTimer (1, 100, NULL);
	}
}

void CDlgMenuThread::OnTimer(UINT nIDEvent) 
{
	KillTimer (m_timer);

	// thilo 15.07.99: im moment wird hier das original menü von cc3d angezeigt. sollte
	// in zukunft ein owner draw menü verwendet werden, wie in ccpro, muß die menüstruktur
	// kopiert werden und ein und die kopie angezeigt werden damit der update funktioniert.

	// code für owner draw menüs ...

	//  CMenu* menu = CreateMenu();
	//  BuildMenu(menu,(CMenu*)m_pMenu);
	//  UINT ret = menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_NONOTIFY|TPM_RETURNCMD, m_pPos->x, m_pPos->y, this);
	//  m_pTarget->PostMessage(WM_COMMAND, (int)ret, NULL);
	//  DestroyMenus();
	//  EndDialog(0);

	// code für standard menüs ...

	UINT ret = m_pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_NONOTIFY|TPM_RETURNCMD, m_pPos->x, m_pPos->y, this);
	m_pTarget->PostMessage(WM_COMMAND, (int)ret, NULL);
	EndDialog(0);
}

void CDlgMenuThread::BuildMenu(CMenu* menu, CMenu* pMenu) 
{
	for ( UINT i=0; i< pMenu->GetMenuItemCount(); i++ )
	{
		CMenu* sub = pMenu->GetSubMenu(i);
		
		CString menuText;
		pMenu->GetMenuString(i, menuText, MF_BYPOSITION);
		UINT menuFlag = pMenu->GetMenuState(i, MF_BYPOSITION);
		UINT menuCode = pMenu->GetMenuItemID(i) ;

		if ( sub != NULL )
		{
			CMenu* subMenu = CreateMenu();
			BuildMenu(subMenu,(CMenu*)sub );
	        menu->AppendMenu(MF_POPUP, (UINT)subMenu->GetSafeHmenu(), menuText);
		}

		else
		{
	        menu->AppendMenu(MF_STRING | menuFlag, menuCode, menuText);
			if ( menuFlag & MF_CHECKED ) 
				 menu->CheckMenuItem( menu->GetMenuItemCount() - 1, MF_CHECKED);
		}

	}
}

CMenu* CDlgMenuThread::CreateMenu()
{
		CMenu* menu = new CMenu();
		menu->CreatePopupMenu();
		m_arrayMenuData.Add(menu);
		return menu;
}

void CDlgMenuThread::DestroyMenus()
{
	for ( int i=0; i<m_arrayMenuData.GetSize(); i++ )
	{
		CMenu* menu = m_arrayMenuData[i];
		menu->DestroyMenu();
		delete(menu);
	}
}