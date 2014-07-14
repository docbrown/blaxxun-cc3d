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
// pfdinfo.cpp : implementation file
//

#include "stdafx.h"
#include "glviewhdr.h"

#ifdef _OGL
#include "pfdinfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPFDInfo dialog

CPFDInfo::CPFDInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CPFDInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPFDInfo)
	m_curpfd = 0;
	m_devfmt = FALSE;
	m_genfmt = FALSE;
	m_maxpfd = 0;
	//}}AFX_DATA_INIT
}

void CPFDInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPFDInfo)
	DDX_Text(pDX, IDC_CURPFD, m_curpfd);
	DDX_Check(pDX, IDC_DEVFMT, m_devfmt);
	DDX_Check(pDX, IDC_GENFMT, m_genfmt);
	DDX_Text(pDX, IDC_MAXPFD, m_maxpfd);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPFDInfo, CDialog)
	//{{AFX_MSG_MAP(CPFDInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPFDInfo message handlers

BOOL CPFDInfo::OnInitDialog()
{
	CDialog::OnInitDialog();
	HDC hdc = m_glptr->m_hdc;
    
	CenterWindow(NULL);
    CheckRadioButton( IDC_GENFMT, IDC_GENFMT, IDC_GENFMT);
	if (m_maxpfd > 24)
      CheckRadioButton( IDC_DEVFMT, IDC_DEVFMT, IDC_DEVFMT);

	m_maxpfd = m_glptr->m_maxpfd;
	SetDlgItemInt(IDC_MAXPFD, m_maxpfd);
	m_curpfd = m_glptr->GetCurPFDIndex();
	SetDlgItemInt(IDC_CURPFD, m_curpfd);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

#endif
