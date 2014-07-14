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
// DRenderFile.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DRenderFile dialog

class DRenderFile : public CDialog
{
// Construction
public:
	DRenderFile(CWnd* pParent = NULL);   // standard constructor
    int renderMin;
    int renderMax;

// Dialog Data
	//{{AFX_DATA(DRenderFile)
	enum { IDD = IDD_RENDER_FILE };
	CSpinButtonCtrl	m_heightSpin;
	CSpinButtonCtrl	m_widthSpin;
	int		m_width;
	CString	m_fileName;
	int		m_height;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DRenderFile)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DRenderFile)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnUpdateHeight();
	afx_msg void OnUpdateWidth();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
