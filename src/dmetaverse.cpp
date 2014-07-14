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
// DMetaVerse.cpp : implementation file
//

#include "stdafx.h"
#include "glview.h"
#include "DMetaVerse.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DMetaVerse dialog


DMetaVerse::DMetaVerse(CWnd* pParent /*=NULL*/)
	: CDialog(DMetaVerse::IDD, pParent)
{
	//{{AFX_DATA_INIT(DMetaVerse)
	m_level = 0;
	m_max = _T("");
	m_min = _T("");
	m_path = _T("");
	m_tiles = _T("");
	//}}AFX_DATA_INIT
}


void DMetaVerse::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DMetaVerse)
	DDX_Text(pDX, IDC_LEVEL, m_level);
	DDX_Text(pDX, IDC_MAX, m_max);
	DDX_Text(pDX, IDC_MIN, m_min);
	DDX_Text(pDX, IDC_PATH, m_path);
	DDX_Text(pDX, IDC_TILES, m_tiles);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DMetaVerse, CDialog)
	//{{AFX_MSG_MAP(DMetaVerse)
	ON_BN_CLICKED(ID_GEN_FILE, OnGenFileStructure)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DMetaVerse message handlers

void DMetaVerse::OnGenFileStructure() 
{
	// TODO: Add your control notification handler code here
	
}

void DMetaVerse::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void DMetaVerse::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
