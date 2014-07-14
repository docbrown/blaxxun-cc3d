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
#if !defined(AFX_DCONSOLE_H__2B2E4FB2_6718_11D1_9481_0020AFE05CC8__INCLUDED_)
#define AFX_DCONSOLE_H__2B2E4FB2_6718_11D1_9481_0020AFE05CC8__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DConsole.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DConsole dialog

class DConsole : public CDialog
{
// Construction
public:
	DConsole(CWnd* pParent = NULL);   // standard constructor

	void AddText(const TCHAR *text); 

// Dialog Data
	//{{AFX_DATA(DConsole)
	enum { IDD = IDD_CONSOLE };
	CButton	m_BtnClose;
	CButton	m_BtnHide;
	CButton	m_BtnCopy;
	CButton	m_BtnClear;
	CEdit	m_EdtText;
	//}}AFX_DATA
	
	BOOL m_hide; // IF TRUE :  don't pop up again the box 

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DConsole)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DConsole)
	virtual void OnOK();
	afx_msg void OnClear();
	afx_msg void OnCopy();
	afx_msg void OnHide();
	virtual BOOL OnInitDialog();
	afx_msg void OnErrspaceText();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


    // Needed for resizing:
    BOOL m_DistancesInited;
    int m_Dist_EdtText_Btns;
    int m_Dist_EdtText_RightBorder;
    int m_Dist_LeftBorder_BtnCopy; 
    int m_Dist_BtnCopy_BtnClear;
    int m_Dist_BtnHide_BtnClose;
    int m_Dist_BtnClose_RightBorder;
    int m_Dist_Btns_BottomBorder;

    void InitDistances();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DCONSOLE_H__2B2E4FB2_6718_11D1_9481_0020AFE05CC8__INCLUDED_)
