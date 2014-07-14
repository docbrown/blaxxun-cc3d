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
#if !defined(AFX_DLGMENUTHREAD_H__91847F00_3471_11D3_822E_00A024A917F6__INCLUDED_)
#define AFX_DLGMENUTHREAD_H__91847F00_3471_11D3_822E_00A024A917F6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "resource.h"
#include <afxtempl.h>    

class CDlgMenuThread : public CDialog
{
// Construction
public:
	CDlgMenuThread(CWnd* pParent = NULL, CMenu* pMenu = NULL, POINT* pPos = NULL, CWnd* pTarget = NULL);

// Dialog Data
	//{{AFX_DATA(CDlgMenuThread)
	enum { IDD = IDD_DIALOG_MENU_THREAD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMenuThread)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void BuildMenu(CMenu* menu, CMenu* pMenu);
	CMenu* CreateMenu();
	void DestroyMenus();

	CArray < CMenu*, CMenu* > m_arrayMenuData;
	
	// CArray<CCustomMenu*,CCustomMenu*> m_arrayMenuData;

	CWnd*	m_pTarget;
	POINT*	m_pPos;
	CMenu*	m_pMenu;
	UINT	m_timer;

	// Generated message map functions
	//{{AFX_MSG(CDlgMenuThread)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMENUTHREAD_H__91847F00_3471_11D3_822E_00A024A917F6__INCLUDED_)
