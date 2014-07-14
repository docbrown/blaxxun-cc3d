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
// morepfd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMorePFD dialog

class CMorePFD : public CDialog
{
// Construction
public:
	CMorePFD(CWnd* pParent = NULL);	// standard constructor

	LPPIXELFORMATDESCRIPTOR m_ppfd;

// Dialog Data
	//{{AFX_DATA(CMorePFD)
	enum { IDD = IDD_MOREPFD };
	int		m_accumalphabits;
	int		m_accumbluebits;
	int		m_accumgreenbits;
	int		m_accumredbits;
	int		m_alphabits;
	int		m_alphashift;
	int		m_bluebits;
	int		m_blueshift;
	DWORD	m_damagemask;
	int		m_greenbits;
	int		m_greenshift;
	DWORD	m_layermask;
	int		m_redbits;
	int		m_redshift;
	DWORD	m_visiblemask;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CMorePFD)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
