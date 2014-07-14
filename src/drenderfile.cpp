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
// DRenderFile.cpp : implementation file
//

#include "stdafx.h"
#include "glviewhdr.h"

#include "DRenderFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DRenderFile dialog


DRenderFile::DRenderFile(CWnd* pParent /*=NULL*/)
	: CDialog(DRenderFile::IDD, pParent)
{
	renderMin = 4;
    renderMax = 2048;
    //{{AFX_DATA_INIT(DRenderFile)
	m_width = 640;
	m_fileName = _T("");
	m_height = 480;
	//}}AFX_DATA_INIT
}


void DRenderFile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DRenderFile)
	DDX_Control(pDX, IDC_HEIGHT_SPIN, m_heightSpin);
	DDX_Control(pDX, IDC_WIDTH_SPIN, m_widthSpin);
	DDX_Text(pDX, IDC_WIDTH, m_width);
	DDV_MinMaxInt(pDX, m_width, 4, 2048);
	DDX_Text(pDX, IDC_FILE, m_fileName);
	DDX_Text(pDX, IDC_HEIGHT, m_height);
	DDV_MinMaxInt(pDX, m_height, 4, 2048);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DRenderFile, CDialog)
	//{{AFX_MSG_MAP(DRenderFile)
	ON_EN_UPDATE(IDC_HEIGHT, OnUpdateHeight)
	ON_EN_UPDATE(IDC_WIDTH, OnUpdateWidth)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DRenderFile message handlers

BOOL DRenderFile::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Set the buddy control.
	m_widthSpin.SetBuddy(GetDlgItem(IDC_WIDTH));
	// Set the range.
	m_widthSpin.SetRange(renderMin,renderMax);
	m_heightSpin.SetBuddy(GetDlgItem(IDC_HEIGHT));
	// Set the range.
	m_heightSpin.SetRange(renderMin,renderMax);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DRenderFile::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void DRenderFile::OnUpdateHeight() 
{
	// TODO: Add your control notification handler code here
	
}

void DRenderFile::OnUpdateWidth() 
{
	// TODO: Add your control notification handler code here
	
}
