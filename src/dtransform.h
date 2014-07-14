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
// DTransform.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DTransformPosition dialog
/*
   PropertyPage dialogs
   for translate, scale, rotate transformation

*/


class CMyglView;


class GView;
class GMoveTransformHandler;
class GTransformPathHandler;

class DTransformPosition : public CPropertyPage
{
	DECLARE_DYNCREATE(DTransformPosition)

// Construction
public:
	DTransformPosition(CMyglView *pView= NULL, UINT nIDTemplate=DTransformPosition::IDD, UINT nIDCaption = 0);
	~DTransformPosition();


	// pointer to current installed handler 
	//GMoveTransformHandler 
	GTransformPathHandler 	*m_handler;


	// Pointer to view 
	CMyglView *m_pView;

	// get the GView of the view 
	GView *GetView();

	int GetAxes() { return ((m_x?1:0) | ((m_y ? 1:0) <<1) | ((m_z ? 1:0) <<2)); }

	// update value in dialog 
	virtual void SetValue(const char *value);

	// send value in edit field to handler 
	virtual void ApplyValue();


	afx_msg void OnControlClicked(UINT ctrl); 



// Dialog Data
	//{{AFX_DATA(DTransformPosition)
	enum { IDD = IDD_TRANSFORM_POSITION };
	CEdit	m_valueCtrl;
	CString	m_value;
	int		m_type;
	BOOL	m_x;
	BOOL	m_y;
	BOOL	m_z;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(DTransformPosition)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	virtual BOOL OnApply();
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(DTransformPosition)
	afx_msg void OnTypeScreen();
	afx_msg void OnClose();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillfocusValue();
	afx_msg void OnChangeValue();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// DTransformScale

class DTransformScale : public 	DTransformPosition //	CPropertyPage
{
	DECLARE_DYNAMIC(DTransformScale)

	// Pointer to view 
	//CMyglView *m_pView;

// Construction
public:
	DTransformScale(CMyglView *pView= NULL);

// Attributes
public:

// Operations
public:

// Dialog Data
	//{{AFX_DATA(DTransformScale)
	enum { IDD = IDD_TRANSFORM_SCALE };
	BOOL	m_uniform;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DTransformScale)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~DTransformScale();

	// Generated message map functions
protected:
	//{{AFX_MSG(DTransformScale)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// DTransformRotation dialog

class DTransformRotation : public DTransformPosition // CPropertyPage
{
	DECLARE_DYNCREATE(DTransformRotation)

// Construction
public:
	DTransformRotation(CMyglView *pView= NULL);
	~DTransformRotation();

// Dialog Data
	//{{AFX_DATA(DTransformRotation)
	enum { IDD = IDD_TRANSFORM_ROTATION };
	BOOL	m_trackball;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(DTransformRotation)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(DTransformRotation)
	afx_msg void OnTrackball();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// DTransformDialog dialog

class DTransformDialog : public CDialog
{
// Construction
public:
	DTransformDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DTransformDialog)
	enum { IDD = IDD_TRANSFORM };
	CTabCtrl	m_tab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DTransformDialog)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DTransformDialog)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTab();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// DTransformSheet

class DTransformSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(DTransformSheet)

// Construction
public:
	DTransformSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	DTransformSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DTransformSheet)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~DTransformSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(DTransformSheet)
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// DTransformNewObject dialog

class DTransformNewObject : public DTransformPosition // CPropertyPage
{
	DECLARE_DYNCREATE(DTransformNewObject)

// Construction
public:
	DTransformNewObject(CMyglView *pView= NULL);
	~DTransformNewObject();

	// add object types from library to combo box
	void FillObjectTypes(); 


// Dialog Data
	//{{AFX_DATA(DTransformNewObject)
	enum { IDD = IDD_TRANSFORM_NEW_OBJECT };
	CComboBox	m_object_type_ctrl;
	BOOL	m_share;
	CString	m_object_type;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(DTransformNewObject)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(DTransformNewObject)
	afx_msg void OnSelchangeObjectType();
	afx_msg void OnShare();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// DTransformMode dialog

class DTransformMode : public CPropertyPage
{
	DECLARE_DYNCREATE(DTransformMode)

// Construction
public:
	DTransformMode(CMyglView *pView= NULL, UINT nIDTemplate=DTransformMode::IDD, UINT nIDCaption = 0);
	~DTransformMode();

	// Pointer to view 
	CMyglView *m_pView;

	// get the GView of the view 
	GView *GetView();

// Dialog Data
	//{{AFX_DATA(DTransformMode)
	enum { IDD = IDD_TRANSFORM_MODE };
	CString	m_value;
	BOOL	m_deep;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(DTransformMode)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(DTransformMode)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// DTransformSelect dialog

class DTransformSelect : public CPropertyPage
{
	DECLARE_DYNCREATE(DTransformSelect)

// Construction
public:
	DTransformSelect(CMyglView *pView= NULL,UINT nIDTemplate=DTransformSelect::IDD, UINT nIDCaption = 0);
	~DTransformSelect();

	// Pointer to view 
	CMyglView *m_pView;

	// get the GView of the view 
	GView *GetView();

	// pointer to handler 
	GTransformPathHandler 	*m_handler;



// Dialog Data
	//{{AFX_DATA(DTransformSelect)
	enum { IDD = IDD_TRANSFORM_SELECT };
	CEdit	m_infoC;
	CListCtrl	m_listC;
	CComboBox	m_valueC;
	BOOL	m_deep;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(DTransformSelect)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(DTransformSelect)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
