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
// dstyle.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DStyle dialog

class DStyle : public CDialog
{
public:
   int applied;

   CMyglView *m_pView;

// Construction
public:
	DStyle(CWnd* pParent = NULL,CMyglView *pView=NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DStyle)
	enum { IDD = IDD_STYLE };
	BOOL	m_edges_colored;
	BOOL	m_edges_lighted;
	BOOL	m_edges_matcolor;
	BOOL	m_edges_smooth;
	BOOL	m_edges_textured;
	BOOL	m_edges_visible;
	CString	m_edge_color;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DStyle)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DStyle)
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnApply();
	afx_msg void OnEdgeDrawstyle();
	afx_msg void OnEdgeEditColor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
