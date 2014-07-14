// MyCC3D.h : main header file for the MYCC3D application
//

#if !defined(AFX_MYCC3D_H__F44EA525_9D59_11D1_AF19_444553540000__INCLUDED_)
#define AFX_MYCC3D_H__F44EA525_9D59_11D1_AF19_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMyCC3DApp:
// See MyCC3D.cpp for the implementation of this class
//

class CMyCC3DApp : public CWinApp
{
public:
	CMyCC3DApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyCC3DApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMyCC3DApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCC3D_H__F44EA525_9D59_11D1_AF19_444553540000__INCLUDED_)
