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
// dstylep.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DStylePage dialog

class DStylePage : public CPropertyPage
{
	DECLARE_DYNCREATE(DStylePage)

public:
   int applied;

    CMyglView *m_pView;		// Pointer to view
    DTarget m_target;

// Construction
public:
	DStylePage(UINT nIDCaption=0,CMyglView *pView=NULL,
		DTarget Target = DVertices);
	~DStylePage();

// Dialog Data
	//{{AFX_DATA(DStylePage)
	enum { IDD = IDD_STYLE_PAGE };
	BOOL	m_visible;
	BOOL	m_textured;
	BOOL	m_smooth;
	BOOL	m_matcolor;
	BOOL	m_lighted;
	BOOL	m_colored;
	CString	m_color;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(DStylePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	void OnButtonClicked(UINT /*nCmdID*/);	

	void OnCancel();
	BOOL OnKillActive();	// Update data !
	void OnOK();
	BOOL OnSetActive();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(DStylePage)
	afx_msg void OnColorEdit();
	afx_msg void OnDrawstyle();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
