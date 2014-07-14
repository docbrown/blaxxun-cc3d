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

@module GLView.h - GLView MFC application	|

Copyright (c) 1995	by Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:

Changes:

Todo :

******************************************************************************/
#ifndef _GLView_h_
#define _GLView_h_

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMyglApp:
// See mygl.cpp for the implementation of this class
//

// Forward
class CMyglView;

class CMyglApp : public CWinApp
{
public:
	CMyglApp();

	int m_ChoosePixelFormat;

	CMyglView* m_ActiveView;

	void SetActiveView(CMyglView *view) { m_ActiveView = view; }
	CMyglView* GetActiveView() { return(m_ActiveView); }

// Overrides

	virtual BOOL InitInstance();

    virtual int ExitInstance();

	virtual BOOL OnDDECommand( LPTSTR lpszCommand);

	virtual BOOL OnIdle(LONG lcount); // Idle Processing

// Implementation

	//{{AFX_MSG(CMyglApp)
	afx_msg void OnAppAbout();
	afx_msg void OnAppExit();
	afx_msg void OnRegisterFiletypes();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// DRegisterFiletypes dialog

class DRegisterFiletypes : public CDialog
{
// Construction
public:
	DRegisterFiletypes(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DRegisterFiletypes)
	enum { IDD = IDD_REGISTER_FILETYPES };
	CString	m_types;
	BOOL	m_register_netscape;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DRegisterFiletypes)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DRegisterFiletypes)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif
