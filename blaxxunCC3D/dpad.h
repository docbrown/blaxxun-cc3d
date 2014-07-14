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
#if !defined(AFX_DPAD_H__BFB38200_EBDC_11D1_AF19_0000B45A5973__INCLUDED_)
#define AFX_DPAD_H__BFB38200_EBDC_11D1_AF19_0000B45A5973__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// dpad.h : header file
//

class GView;

/////////////////////////////////////////////////////////////////////////////
// DPadAvatarView dialog

class DPadAvatarView : public CPropertyPage
{
	DECLARE_DYNCREATE(DPadAvatarView)

// Construction
public:
	DPadAvatarView();
	~DPadAvatarView();

	GView *view;

// Dialog Data
	//{{AFX_DATA(DPadAvatarView)
	enum { IDD = IDD_PROP_AVATAR_VIEW };
	CListBox	m_modeList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(DPadAvatarView)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(DPadAvatarView)
	afx_msg void OnSelchangeModeList();
	afx_msg void OnReset();
	afx_msg void OnDblclkModeList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// DPadViewpoints dialog

class DPadViewpoints : public CPropertyPage
{
// Construction
public:
	DPadViewpoints(); 

	GView *view;

// Dialog Data
	//{{AFX_DATA(DPadViewpoints)
	enum { IDD = IDD_PROP_VIEWPOINTS };
	CListBox	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DPadViewpoints)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DPadViewpoints)
	afx_msg void OnDblclkList();
	afx_msg void OnSelchangeList();
	afx_msg void OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTour();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// DPadNavigationMode dialog

class DPadNavigationMode : public CPropertyPage
{
	DECLARE_DYNCREATE(DPadNavigationMode)

// Construction
public:
	DPadNavigationMode();
	~DPadNavigationMode();

	GView *view;

// Dialog Data
	//{{AFX_DATA(DPadNavigationMode)
	enum { IDD = IDD_PROP_NAVIGATION_MODE };
	CListBox	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(DPadNavigationMode)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(DPadNavigationMode)
	afx_msg void OnSelchangeList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DPAD_H__BFB38200_EBDC_11D1_AF19_0000B45A5973__INCLUDED_)
