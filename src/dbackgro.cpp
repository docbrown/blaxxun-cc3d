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
// dbackgro.cpp : implementation file
//

#include "stdafx.h"
#include "glviewhdr.h"

#include "dbackgro.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DBackground dialog

DBackground::DBackground(CWnd* pParent /*=NULL*/,CMyglView *pView /* NULL */)
	: CDialog(DBackground::IDD, pParent)
{
	applied = 0;
	m_pView = pView;
	//{{AFX_DATA_INIT(DBackground)
	m_type = "";
	m_color1 = "";
	m_color2 = "";
	m_color3 = "";
	//}}AFX_DATA_INIT

	focus=IDC_COLOR1;
}

void DBackground::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DBackground)
	DDX_CBString(pDX, IDC_TYPE, m_type);
	DDX_Text(pDX, IDC_COLOR1, m_color1);
	DDX_Text(pDX, IDC_COLOR2, m_color2);
	DDX_Text(pDX, IDC_COLOR3, m_color3);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(DBackground, CDialog)
	//{{AFX_MSG_MAP(DBackground)
	ON_BN_CLICKED(IDC_EDIT_COLOR, OnClickedEditColor)
	ON_EN_SETFOCUS(IDC_COLOR1, OnSetfocusColor1)
	ON_EN_SETFOCUS(IDC_COLOR2, OnSetfocusColor2)
	ON_BN_CLICKED(IDC_APPLY, OnClickedApply)
	ON_EN_SETFOCUS(IDC_COLOR3, OnSetfocusColor3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DBackground message handlers

void DBackground::OnOK()
{
	// TODO: Add extra validation here

	CDialog::OnOK();
}

void DBackground::OnCancel()
{
	// TODO: Add extra cleanup here

	CDialog::OnCancel();
}

void DBackground::OnClickedEditColor()
{
	CString cs;
	Point c;
	int len=180;

	GetDlgItemText(focus,cs.GetBuffer(len+1),len);

	CstringToPoint(cs,c);
	if (m_pView->EditColor("Color",c)) {
	    CstringFromPoint(cs,c);
		SetDlgItemText(focus,cs);
	}
}

void DBackground::OnSetfocusColor1()
{
	focus = IDC_COLOR1;
}

void DBackground::OnSetfocusColor2()
{
	focus = IDC_COLOR2;
}

void DBackground::OnClickedApply()
{
	if (!UpdateData(TRUE))
	{
		TRACE0("UpdateData failed during dialog apply\n");
		// the UpdateData routine will set focus to correct item
		return;
	}
	if (m_pView) m_pView->ApplyBackground(*this);
}

void DBackground::OnSetfocusColor3()
{
	focus = IDC_COLOR3;
}
