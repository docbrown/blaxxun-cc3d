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
// dtexture.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DTexture dialog

class CMyglView;	// forward

class DTexture : public CDialog
{
public:
	CMyglView *m_pView;

// Construction
public:
	DTexture(CWnd* pParent = NULL,CMyglView *pView=NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(DTexture)
	enum { IDD = IDD_TEXTURE };
	CString	m_tfilter_max;
	CString	m_tfilter_min;
	CString	m_tfunction;
	BOOL	m_repeat_t;
	BOOL	m_repeat_s;
	BOOL	m_gen_mipmaps;
	CString	m_rotate;
	CString	m_scale;
	CString	m_translate;
	BOOL	m_white;
	BOOL	m_perspective_hint;
	int		m_max_size;
	CString	m_tblend;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(DTexture)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClickedTextureReset();
	afx_msg void OnClickedApply();
	afx_msg void OnDeltaposScaleSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposRotatezSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposTranslateXSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposTranslateYSpin(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// DParameter dialog

class DParameter : public CDialog
{
	CMyglView *m_pView;

// Construction
public:
	DParameter(CWnd* pParent = NULL,CMyglView *pView=NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DParameter)
	enum { IDD = IDD_PARAMETER };
	CString	m_function;
	CString	m_source;
	BOOL	m_normalize;
	CString	m_mapping;
	BOOL	m_showSurface;
	CString	m_rotate;
	CString	m_scale;
	CString	m_translate;
	BOOL	m_enable;
	BOOL	m_showMappedObject;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DParameter)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DParameter)
	afx_msg void OnApply();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnMappingReset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
