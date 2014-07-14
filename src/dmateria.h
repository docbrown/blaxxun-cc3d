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
// dmateria.h : header file
// Dialog for changing Material
// Colors ambient diffuse, specular , emissive
// alpha, shininess
//

/////////////////////////////////////////////////////////////////////////////
// DMaterial dialog
class CMyglView;

class DMaterial : public CDialog
{
public:
	CMyglView *m_pView;

// Construction
public:
	DMaterial(CWnd* pParent = NULL,CMyglView *pView = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(DMaterial)
	enum { IDD = IDD_MATERIAL };
	CString	m_ambient;
	CString	m_diffuse;
	CString	m_emission;
	CString	m_specular;
	float	m_shininess;
	float	m_alpha;
	//}}AFX_DATA

   int focus;
   Point ambient,diffuse,emission,specular;

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(DMaterial)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClickedMaterialColor();
	afx_msg void OnSetfocusEditAmbient();
	afx_msg void OnSetfocusEditDiffuse();
	afx_msg void OnSetfocusEditEmission();
	afx_msg void OnClickedMaterialApply();
	afx_msg void OnSetfocusEditSpecular();
	afx_msg void OnClickedMaterialLoad();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
