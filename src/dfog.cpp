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
// dfog.cpp : implementation file
//

#include "stdafx.h"
#include "glviewhdr.h"

#include "dfog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dfog dialog


Dfog::Dfog(CWnd* pParent /*=NULL*/,CMyglView *pView /* NULL */)
	: CDialog(Dfog::IDD, pParent)
{
	applied = 0;
	m_pView = pView;
	//{{AFX_DATA_INIT(Dfog)
	m_type = _T("");
	m_density = 0.0f;
	m_end = 0.0f;
	m_start = 0.0f;
	m_use_background = FALSE;
	m_enable = FALSE;
	m_color = _T("");
	m_quality = FALSE;
	//}}AFX_DATA_INIT
}


void Dfog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	if (!pDX->m_bSaveAndValidate) {
	  m_pView->GetFog(*this);
	}

	//{{AFX_DATA_MAP(Dfog)
	DDX_CBString(pDX, IDC_COMBO_TYPE, m_type);
	DDX_Text(pDX, IDC_DENSITY, m_density);
	DDX_Text(pDX, IDC_END, m_end);
	DDX_Text(pDX, IDC_START, m_start);
	DDX_Check(pDX, IDC_USE_BACKGROUND, m_use_background);
	DDX_Check(pDX, IDC_FOG_ENABLE, m_enable);
	DDX_Text(pDX, IDC_COLOR, m_color);
	DDX_Check(pDX, IDC_QUALITY, m_quality);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Dfog, CDialog)
	//{{AFX_MSG_MAP(Dfog)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_BN_CLICKED(IDC_EDIT_COLOR, OnEditColor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dfog message handlers

void Dfog::OnApply()
{
	if (!UpdateData(TRUE))
	{
		TRACE0("UpdateData failed during dialog apply\n");
		// the UpdateData routine will set focus to correct item
		return;
	}
	if (m_pView) m_pView->ApplyFog(*this);
}

void Dfog::OnEditColor()
{
	CString cs;
	Point c;
	int len=180;
	int focus =IDC_COLOR;

	GetDlgItemText(focus,cs.GetBuffer(len+1),len);

	CstringToPoint(cs,c);
	if (m_pView->EditColor("Color",c)) {
		CstringFromPoint(cs,c);
		SetDlgItemText(focus,cs);
	}
}

void Dfog::OnCancel()
{
	CDialog::OnCancel();
}

void Dfog::OnOK()
{
	CDialog::OnOK();
}
