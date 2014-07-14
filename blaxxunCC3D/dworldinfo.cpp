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
// DWorldInfo.cpp : implementation file
//

#include "stdafx.h"
#include "blaxxunCC3d.h"
#include "DWorldInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DWorldInfo dialog


DWorldInfo::DWorldInfo(CWnd* pParent /*=NULL*/)
	: CDialog(DWorldInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(DWorldInfo)
	m_info = _T("");
	m_title = _T("");
	m_url = _T("");
	m_vrml2 = FALSE;
	//}}AFX_DATA_INIT
}


void DWorldInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DWorldInfo)
	DDX_Text(pDX, IDC_INFO, m_info);
	DDX_Text(pDX, IDC_TITLE, m_title);
	DDX_Text(pDX, IDC_URL, m_url);
	DDX_Check(pDX, IDC_VRML2, m_vrml2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DWorldInfo, CDialog)
	//{{AFX_MSG_MAP(DWorldInfo)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DWorldInfo message handlers
