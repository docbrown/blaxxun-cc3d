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
// dstyle.cpp : implementation file
//

#include "stdafx.h"
#include "glviewhdr.h"

#include "dstyle.h"
#include "ddrawsty.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DStyle dialog


DStyle::DStyle(CWnd* pParent /*=NULL*/,CMyglView *pView /* NULL */)
	: CDialog(DStyle::IDD, pParent)
{
	m_pView = pView;
 	applied = 0;

	//{{AFX_DATA_INIT(DStyle)
	m_edges_colored = FALSE;
	m_edges_lighted = FALSE;
	m_edges_matcolor = FALSE;
	m_edges_smooth = FALSE;
	m_edges_textured = FALSE;
	m_edges_visible = FALSE;
	m_edge_color = _T("");
	//}}AFX_DATA_INIT
}


void DStyle::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	if (!pDX->m_bSaveAndValidate) {
	  m_pView->GetStyle(*this);
	}

	//{{AFX_DATA_MAP(DStyle)
	DDX_Check(pDX, IDC_EDGES_COLORED, m_edges_colored);
	DDX_Check(pDX, IDC_EDGES_LIGHTED, m_edges_lighted);
	DDX_Check(pDX, IDC_EDGES_MATCOLOR, m_edges_matcolor);
	DDX_Check(pDX, IDC_EDGES_SMOOTH, m_edges_smooth);
	DDX_Check(pDX, IDC_EDGES_TEXTURED, m_edges_textured);
	DDX_Check(pDX, IDC_EDGES_VISIBLE, m_edges_visible);
	DDX_Text(pDX, IDC_EDGE_COLOR, m_edge_color);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DStyle, CDialog)
	//{{AFX_MSG_MAP(DStyle)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_BN_CLICKED(IDC_EDGE_DRAWSTYLE, OnEdgeDrawstyle)
	ON_BN_CLICKED(IDC_EDGE_COLOR_EDIT, OnEdgeEditColor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// DStyle message handlers

void DStyle::OnCancel()
{
	// TODO: Add extra cleanup here

	CDialog::OnCancel();
}

void DStyle::OnOK()
{
	// TODO: Add extra validation here

	CDialog::OnOK();
}

void DStyle::OnApply()
{
	if (!UpdateData(TRUE))
	{
		TRACE0("UpdateData failed during dialog apply\n");
		// the UpdateData routine will set focus to correct item
		return;
	}
	if (m_pView) m_pView->ApplyStyle(*this);

}

void DStyle::OnEdgeDrawstyle() 
{
	DDrawStyle d(NULL,m_pView,DEdges);
	d.DoModal();
}

void DStyle::OnEdgeEditColor() 
{
	// TODO: Add your control notification handler code here
	
}
