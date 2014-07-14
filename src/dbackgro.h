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
// dbackgro.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DBackground dialog
class CMyglView;  	// forward
class DBackground : public CDialog
{
public:
   int applied;
   
   CMyglView *m_pView;
   int focus;		// focus for color edit field

// Construction
public:
	DBackground(CWnd* pParent = NULL,CMyglView *pView=NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(DBackground)
	enum { IDD = IDD_BACKGROUND_OPTIONS };
	CString	m_type;
	CString	m_color1;
	CString	m_color2;
	CString	m_color3;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(DBackground)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClickedEditColor();
	afx_msg void OnSetfocusColor1();
	afx_msg void OnSetfocusColor2();
	afx_msg void OnClickedApply();
	afx_msg void OnSetfocusColor3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
