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
// DD3DOptions.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DD3DOptions dialog

class GView;


class DD3DOptions : public CDialog
{
// Construction
public:
	DD3DOptions(CWnd* pParent,GView *view);   // standard constructor

	int m_driverIndex;
	int m_modeIndex;
	int m_textureModeIndex;
	int	m_deviceIndex;

	GView *m_view;


// Dialog Data
	//{{AFX_DATA(DD3DOptions)
	enum { IDD = IDD_D3DOPTIONS };
	CComboBox	m_dxLevelC;
	CComboBox	m_deviceC;
	CComboBox	m_driverC;
	CComboBox	m_modeC;
	CComboBox	m_textureModeC;
	BOOL	m_highlights;
	BOOL	m_dither;
	BOOL	m_alphaStipple;
	CString	m_driver;
	BOOL	m_tfilter;
	CString	m_info;
	BOOL	m_use_videomem;
	CString	m_dxLevel;
	BOOL	m_software;
	BOOL	m_anitalias;
	BOOL	m_useD3DCulling;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{   AFX_VIRTUAL(DD3DOptions)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}  AFX_VIRTUAL

// Implementation
protected:


	// Generated message map functions
	//{{AFX_MSG(DD3DOptions)
	virtual void OnOK();
	afx_msg void OnSelchangeDriver();
	afx_msg void OnSelchangeMode();
	afx_msg void OnSelchangeTextureMode();
	afx_msg void OnSelchangeDevice();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
