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
// ddrawsty.cpp : implementation file
//

#include "stdafx.h"
#include "glviewhdr.h"
#include "ddrawsty.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DDrawStyle dialog


DDrawStyle::DDrawStyle(CWnd* pParent /*=NULL*/,CMyglView *pView,DTarget Target)
	: CDialog(DDrawStyle::IDD, pParent)
{
	applied = 0;
	m_pView = pView;
	m_target = Target;

	//{{AFX_DATA_INIT(DDrawStyle)
	m_scale1 = 0.1f;
	m_scale2 = 0.05f;
	m_xsteps = 8;
	m_ysteps = 4;
	m_style = _T("sphere");
	m_texture = FALSE;
	m_orient = FALSE;
	m_enable = FALSE;
	m_fill = FALSE;
	m_smooth = FALSE;
	m_point = FALSE;
	//}}AFX_DATA_INIT
}


void DDrawStyle::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	if (!pDX->m_bSaveAndValidate) {
	  m_pView->GetDrawStyle(*this);
	}

	//{{AFX_DATA_MAP(DDrawStyle)
	DDX_Text(pDX, IDC_RADIUS1, m_scale1);
	DDX_Text(pDX, IDC_RADIUS2, m_scale2);
	DDX_Text(pDX, IDC_STEPSX, m_xsteps);
	DDV_MinMaxInt(pDX, m_xsteps, 0, 64);
	DDX_Text(pDX, IDC_STEPSY, m_ysteps);
	DDX_CBString(pDX, IDC_STYLE, m_style);
	DDX_Check(pDX, IDC_TEXTURE, m_texture);
	DDX_Check(pDX, IDC_ORIENT, m_orient);
	DDX_Check(pDX, IDC_ENABLE, m_enable);
	DDX_Check(pDX, IDC_FILL, m_fill);
	DDX_Check(pDX, IDC_SMOOTH, m_smooth);
	DDX_Check(pDX, IDC_POINT, m_point);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DDrawStyle, CDialog)
	//{{AFX_MSG_MAP(DDrawStyle)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// DDrawStyle message handlers

void DDrawStyle::OnApply()
{
	if (!UpdateData(TRUE))
	{
		TRACE0("UpdateData failed during dialog apply\n");
		// the UpdateData routine will set focus to correct item
		return;
	}
	if (m_pView) m_pView->ApplyDrawStyle(*this);
}

void DDrawStyle::OnCancel()
{
	// TODO: Add extra cleanup here

	CDialog::OnCancel();
}

void DDrawStyle::OnOK()
{
	// TODO: Add extra validation here

	CDialog::OnOK();
}
