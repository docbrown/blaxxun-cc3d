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
#if !defined(AFX_DPROP_H__620D0B81_0719_11D1_942A_0020AFE05CC8__INCLUDED_)
#define AFX_DPROP_H__620D0B81_0719_11D1_942A_0020AFE05CC8__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DProp.h : header file
//


#define CGLViewCtrlCtrl CMyglView

class CMyglView;

/////////////////////////////////////////////////////////////////////////////
// DPropSheet

class DPropSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(DPropSheet)

// Construction
public:
	DPropSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	DPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DPropSheet)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~DPropSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(DPropSheet)
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// DPropD3D dialog

class DPropD3D : public CPropertyPage
{
	DECLARE_DYNCREATE(DPropD3D)

// Construction
public:
	DPropD3D(CGLViewCtrlCtrl *ctrl=NULL);
	~DPropD3D();
	
	BOOL OnInitDialog(); 

	
	int m_driverIndex;
	int m_modeIndex;

	CGLViewCtrlCtrl *m_ctrl;

// Dialog Data
	//{{AFX_DATA(DPropD3D)
	enum { IDD = IDD_PROP_D3D };
	CComboBox	m_modeC;
	CComboBox	m_driverC;
	BOOL	m_dither;
	BOOL	m_tfilter;
	CString	m_info;
	BOOL	m_use_videomem;
	BOOL	m_antiAlias;
	BOOL	m_tmipmap;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(DPropD3D)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(DPropD3D)
	afx_msg void OnSelchangeComboDriver();
	afx_msg void OnSelchangeComboMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// DPropGeneral dialog

class DPropGeneral : public CPropertyPage
{
	DECLARE_DYNCREATE(DPropGeneral)

// Construction
public:
	DPropGeneral(CGLViewCtrlCtrl *ctrl=NULL);
	~DPropGeneral();

	BOOL OnInitDialog(); 

	CGLViewCtrlCtrl *m_ctrl;

	int m_uiModeIndex;


// Dialog Data
	//{{AFX_DATA(DPropGeneral)
	enum { IDD = IDD_PROP_GENERAL };
	CComboBox	m_menueModeC;
	CString	m_menueMode;
	BOOL	m_verbose;
	BOOL	m_fullSpeed;
	BOOL	m_navPanelOn;
	BOOL	m_exactCollision;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(DPropGeneral)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(DPropGeneral)
	afx_msg void OnRegisterVrml();
	afx_msg void OnSelchangeMenueMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// DAskHardware dialog

class DAskHardware : public CDialog
{
// Construction
public:
	DAskHardware(CWnd* pParent = NULL);   // standard constructor
	
	CGLViewCtrlCtrl *m_ctrl;
	BOOL m_useHardware;
	BOOL OnInitDialog(); 

// Dialog Data
	//{{AFX_DATA(DAskHardware)
	enum { IDD = IDD_ASKHARDWARE };
	BOOL	m_dontAskAgain;
	CString	m_info;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DAskHardware)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DAskHardware)
	virtual void OnOK();
	afx_msg void OnNo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// DPropSound dialog

class DPropSound : public CPropertyPage
{
// Construction
public:
	DPropSound(CGLViewCtrlCtrl *ctrl = NULL);   // standard constructor
	
	BOOL OnInitDialog(); 
	CGLViewCtrlCtrl *m_ctrl;

	int m_qualityI;

// Dialog Data
	//{{AFX_DATA(DPropSound)
	enum { IDD = IDD_PROP_SOUND };
	CComboBox	m_qualityC;
	CString	m_quality;
	CString	m_info;
	int		m_num_sounds;
	BOOL	m_use_rsx;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DPropSound)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DPropSound)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// DPropOpenGL dialog

class DPropOpenGL : public CPropertyPage
{
	DECLARE_DYNCREATE(DPropOpenGL)

// Construction
public:
	DPropOpenGL(CGLViewCtrlCtrl *ctrl=NULL);
	~DPropOpenGL();

	BOOL OnInitDialog(); 
	CGLViewCtrlCtrl *m_ctrl;

	int m_nurbsModeIndex;

// Dialog Data
	//{{AFX_DATA(DPropOpenGL)
	enum { IDD = IDD_PROP_OPENGL };
	CComboBox	m_nurbsModeC;
	BOOL	m_dither;
	CString	m_info;
	CString	m_info1;
	BOOL	m_tfilter;
	BOOL	m_rsoftware;
	BOOL	m_tmipmap;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(DPropOpenGL)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(DPropOpenGL)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// DPropCache dialog

class DPropCache : public CPropertyPage
{
	DECLARE_DYNCREATE(DPropCache)

// Construction
public:
	DPropCache(CGLViewCtrlCtrl *ctrl=NULL);
	~DPropCache();

	BOOL OnInitDialog(); 
	CGLViewCtrlCtrl *m_ctrl;

// Dialog Data
	//{{AFX_DATA(DPropCache)
	enum { IDD = IDD_PROP_CACHE };
	BOOL	m_useCache;
	CString	m_cacheDirectory;
	CString	m_roCacheDirectorys;
	int		m_verifyDays;
	BOOL	m_cacheRead;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(DPropCache)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(DPropCache)
	afx_msg void OnEmptyCache();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// DPropCacheSettings dialog

class DPropCacheSettings : public CPropertyPage
{
	DECLARE_DYNCREATE(DPropCacheSettings)

// Construction
public:
	DPropCacheSettings(CGLViewCtrlCtrl *ctrl=NULL);
	~DPropCacheSettings();

	BOOL OnInitDialog(); 
	CGLViewCtrlCtrl *m_ctrl;

// Dialog Data
	//{{AFX_DATA(DPropCacheSettings)
	enum { IDD = IDD_PROP_CACHE_SETTINGS };
	int		m_cacheVerify;
	DWORD	m_maxSpace;
	DWORD	m_minSpaceFree;
	int		m_verifyDays;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(DPropCacheSettings)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(DPropCacheSettings)
	afx_msg void OnCacheClean();
	afx_msg void OnCacheDelete();
	afx_msg void OnDeltaposMinSpaceFreeSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposMaxSpaceSpin(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// DPropD3D2 dialog

class DPropD3D2 : public CPropertyPage
{
	DECLARE_DYNCREATE(DPropD3D2)

// Construction
public:
	DPropD3D2(CGLViewCtrlCtrl *ctrl=NULL);
	~DPropD3D2();

	BOOL OnInitDialog(); 

	CGLViewCtrlCtrl *m_ctrl;

	BOOL m_isKatmaiProcessor;
	int m_nurbsModeIndex; // selected index 


// Dialog Data
	//{{AFX_DATA(DPropD3D2)
	enum { IDD = IDD_PROP_D3D_2 };
	CComboBox	m_nurbsModeC;
	CButton	m_useKatmaiC;
	CString	m_info;
	BOOL	m_useKamtai;
	BOOL	m_softwareLighting;
	float	m_targetFps;
	float	m_tessellationFactor;
	BOOL	m_useD3dCulling;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(DPropD3D2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(DPropD3D2)
	afx_msg void OnDeltaposTargetFpsSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposTessFactorSpin(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DPROP_H__620D0B81_0719_11D1_942A_0020AFE05CC8__INCLUDED_)
