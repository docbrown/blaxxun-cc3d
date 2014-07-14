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
// pixform.h : header file
//
#ifdef _OGL

#include "COpenGL.h"
/////////////////////////////////////////////////////////////////////////////
// CPixForm dialog

class CPixForm : public CDialog
{
// Construction
public:
	CPixForm(CWnd* pParent = NULL);	// standard constructor

	int m_nID;
	void SetID(int nID) {m_nID = nID;};

	GOpenglDeviceWGL *m_glptr;

	void SetGLptr(GOpenglDeviceWGL *opengl) {m_glptr = opengl;};

// Dialog Data
	//{{AFX_DATA(CPixForm)
	enum { IDD = IDD_CHOOSEPF };
	int		m_accumbits;
	int		m_alphabits;
	int		m_auxbuffers;
	int		m_colorbits;
	BOOL	m_dbufferdontcare;
	int		m_depthbits;
	BOOL	m_doublebuffer;
	BOOL	m_drawbitmap;
	BOOL	m_drawwindow;
	int		m_stencilbits;
	BOOL	m_stereo;
	BOOL	m_stereodontcare;
	BOOL	m_supportgdi;
	BOOL	m_supportopengl;
	int		m_layertype;
	int		m_pixeltype;
	int		m_index;
	BOOL	m_swapcopy;
	BOOL	m_swapexchange;
	BOOL	m_needpalette;
	BOOL	m_needsystempalette;
	BOOL	m_genericformat;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CPixForm)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickedMorepfd();
	afx_msg void OnClickedLastPfd();
	afx_msg void OnClickedNextPfd();
	afx_msg void OnClickedEndPfd();
	afx_msg void OnClickedFirstPfd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
  int m_nNextID;
  void UpdateDlg(PIXELFORMATDESCRIPTOR *pfd, int idx);

};

#endif
