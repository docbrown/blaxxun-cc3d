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
// DAbout.cpp : implementation file
//

#include "stdafx.h"
#include "blaxxunCC3d.h"
#include "DAbout.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DAbout dialog


DAbout::DAbout(CWnd* pParent /*=NULL*/)
	: CDialog(DAbout::IDD, pParent)
{
	//{{AFX_DATA_INIT(DAbout)
	m_version = _T("");
	m_version2 = _T("");
	m_version3 = _T("");
	//}}AFX_DATA_INIT
}


void DAbout::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DAbout)
	DDX_Text(pDX, IDC_VERSION, m_version);
	DDX_Text(pDX, IDC_VERSION2, m_version2);
	DDX_Text(pDX, IDC_VERSION3, m_version3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DAbout, CDialog)
	//{{AFX_MSG_MAP(DAbout)
	ON_BN_CLICKED(IDC_UPDATE_CHECK, OnUpdateCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DAbout message handlers

void DAbout::OnUpdateCheck() 
{

	if (!UpdateData(TRUE))
	{
		TRACE0("UpdateData failed during dialog termination.\n");
		// the UpdateData routine will set focus to correct item
		return;
	}
	EndDialog(IDC_UPDATE_CHECK);

	
}
