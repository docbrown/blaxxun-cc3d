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
/******************************************************************************
@doc

@module DObjNew.cpp - object creation dialogs|

Copyright (c) 1995	by Holger Grahn
All rights reserved

Purpose:

Classes:
<c DObjNew>
<c DNewFontLogo>

Notes:

Changes:

Todo :

******************************************************************************/

// dobjnew.h : header file

//

/////////////////////////////////////////////////////////////////////////////
//@class DObjNew dialog

class DObjNew : public CDialog
{
public:
	CMyglView *m_pView;
	int mode;	// 0 = replace, 1 = add

// Construction
public:
	DObjNew(CWnd* pParent = NULL,CMyglView *pView=NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DObjNew)
	enum { IDD = IDD_NEW_OBJECT };
	int		m_stepsx;
	int		m_stepsy;
	CString	m_type;
	float	m_umax;
	float	m_umin;
	float	m_vmax;
	float	m_vmin;
	float	m_h;
	float	m_r;
	float	m_height;
	BOOL	m_append;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DObjNew)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DObjNew)
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
//@class DNewFontLogo dialog

class DNewFontLogo : public CDialog
{
public:
   //@cmember view for this dialog
	CMyglView *m_pView;

   //@cmember logical font description
   LOGFONT m_lf;

	// Choose a font using the standard font dialog
	int ChooseFont();


// Construction
public:
	DNewFontLogo(CWnd* pParent = NULL,CMyglView *pView=NULL);   // standard constructor


// Dialog Data
	//{{AFX_DATA(DNewFontLogo)
	enum { IDD = IDD_FONT_LOGO };
	BOOL	m_extrude;
	float	m_extrusion_factor;
	CString	m_text;
	BOOL	m_top;
	CString	m_font_descr;
	BOOL	m_bottom;
	BOOL	m_parameters;
	float	m_invert_factor;
	BOOL	m_invert;
	int		m_align;
	BOOL	m_outline;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DNewFontLogo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DNewFontLogo)
	afx_msg void OnFont();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
