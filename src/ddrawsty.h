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
// ddrawsty.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DDrawStyle dialog

enum DTarget { DVertices,DEdges,DFaces,DNormals,DBBox };


class DDrawStyle : public CDialog
{
public:
   int applied;

   CMyglView *m_pView;
   DTarget m_target;

// Construction
public:
	DDrawStyle(CWnd* pParent = NULL,
		CMyglView *pView=NULL,
		DTarget Target = DVertices);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DDrawStyle)
	enum { IDD = IDD_DRAW_STYLE };
	float	m_scale1;
	float	m_scale2;
	int		m_xsteps;
	int		m_ysteps;
	CString	m_style;
	BOOL	m_texture;
	BOOL	m_orient;
	BOOL	m_enable;
	BOOL	m_fill;
	BOOL	m_smooth;
	BOOL	m_point;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DDrawStyle)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DDrawStyle)
	afx_msg void OnApply();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
