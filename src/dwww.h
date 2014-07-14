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
// dwww.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DOpenUrl dialog
class CMyglView;

class DOpenUrl : public CDialog
{
// Construction
public:
	DOpenUrl(CWnd* pParent = NULL,CMyglView *view=NULL);   // standard constructor
	CMyglView *m_pView;

// Dialog Data
	//{{AFX_DATA(DOpenUrl)
	enum { IDD = IDD_OPEN_URL };
	CListBox	m_history;
	CString	m_status;
	CString	m_url;
	int		m_historySel;
	BOOL	m_add_files;
	//}}AFX_DATA

	int ClearHistory();
	int AddToHistory(const char * string,int mode=0);
	int AddFileToHistory(const char * string,int mode=0);
	void SaveHistory(); 
	void RestoreHistory(); 


	int SetUrl(const char * string,int mode=0);
	int SetStatus(const char * string,int mode=0);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DOpenUrl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DOpenUrl)
	afx_msg void OnSelChangeHistory();
	afx_msg void OnDblclkHistory();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeUrl();
	afx_msg void OnClear();
	afx_msg void OnSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// DWWWOptions dialog

class DWWWOptions : public CDialog
{
// Construction
public:
	DWWWOptions(CWnd* pParent = NULL,CMyglView * pView=NULL);   // standard constructor
	CMyglView * m_pView;

// Dialog Data
	//{{AFX_DATA(DWWWOptions)
	enum { IDD = IDD_WWW_OPTIONS };
	BOOL	m_useCamera;
	BOOL	m_useSpotLights;
	BOOL	m_useSceneLights;
	BOOL	m_headLight;
	CString	m_cacheDirectory;
	BOOL	m_keepHostConnection;
	BOOL	m_useTextures;
	BOOL	m_useShapeHints;
	BOOL	m_useBackground;
	BOOL	m_useMaterials;
	BOOL	m_useSensors;
	int		m_maxInlineDepth;
	BOOL	m_incrementalUpdates;
	CString	m_email;
	int		m_inline;
	BOOL	m_useFilesFromCache;
	float	m_lightScale;
	CString	m_browserPath;
	BOOL	m_auto_play;
	int		m_texture_loading;
	CString	m_ambientLight;
	BOOL	m_enableCollision;
	CString	m_avatar;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DWWWOptions)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DWWWOptions)
	virtual void OnOK();
	afx_msg void OnSave();
	afx_msg void OnInlineAll();
	afx_msg void OnInlineIfNeeded();
	afx_msg void OnInlineNever();
	afx_msg void OnBrowseBrowser();
	afx_msg void OnBrowseCache();
	afx_msg void OnInternetConfig();
	afx_msg void OnBrowseAvatar();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// DWWWRetry dialog

class DWWWRetry : public CDialog
{
// Construction
public:
	DWWWRetry(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DWWWRetry)
	enum { IDD = IDD_WWW_RETRY };
	CString	m_url;
	CString	m_status;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DWWWRetry)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DWWWRetry)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnIgnoreAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// DWWWConfig dialog

class DWWWConfig : public CDialog
{
// Construction
public:
	DWWWConfig(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DWWWConfig)
	enum { IDD = IDD_WWW_CONFIG };
	CString	m_proxy;
	CString	m_proxy_bypass;
	int		m_access_type;
	int		m_max_threads;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DWWWConfig)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DWWWConfig)
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
