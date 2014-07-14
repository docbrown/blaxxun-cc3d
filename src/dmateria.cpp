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
// dmateria.cpp : implementation file
//

#include "stdafx.h"
#include "glviewhdr.h"

#include "dmateria.h"

#include <stdio.h>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DMaterial dialog

DMaterial::DMaterial(CWnd* pParent /*=NULL*/,CMyglView *pView /* = NULL*/)
	: CDialog(DMaterial::IDD, pParent)
{
	m_pView = pView;
	//{{AFX_DATA_INIT(DMaterial)
	m_ambient = "";
	m_diffuse = "";
	m_emission = "";
	m_specular = "";
	m_shininess = 0;
	m_alpha = 0;
	//}}AFX_DATA_INIT
	
	focus=IDC_EDIT_DIFFUSE;
}


void DMaterial::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	if (!pDX->m_bSaveAndValidate) {
	   CstringFromPoint(m_ambient,ambient);
	   CstringFromPoint(m_diffuse,diffuse);
	   CstringFromPoint(m_emission,emission);
	   CstringFromPoint(m_specular,specular);
	}

	//{{AFX_DATA_MAP(DMaterial)
	DDX_Text(pDX, IDC_EDIT_AMBIENT, m_ambient);
	DDX_Text(pDX, IDC_EDIT_DIFFUSE, m_diffuse);
	DDX_Text(pDX, IDC_EDIT_EMISSION, m_emission);
	DDX_Text(pDX, IDC_EDIT_SPECULAR, m_specular);
	DDX_Text(pDX, IDC_EDIT_SHININESS, m_shininess);
	DDV_MinMaxFloat(pDX, m_shininess, 0., 128.);
	DDX_Text(pDX, IDC_EDIT_ALPHA, m_alpha);
	DDV_MinMaxFloat(pDX, m_alpha, 0., 1.);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate) {
	   CstringToPoint(m_ambient,ambient);
	   CstringToPoint(m_diffuse,diffuse);
	   CstringToPoint(m_emission,emission);
	   CstringToPoint(m_specular,specular);
	}
}

BEGIN_MESSAGE_MAP(DMaterial, CDialog)
	//{{AFX_MSG_MAP(DMaterial)
	ON_BN_CLICKED(IDC_MATERIAL_COLOR, OnClickedMaterialColor)
	ON_EN_SETFOCUS(IDC_EDIT_AMBIENT, OnSetfocusEditAmbient)
	ON_EN_SETFOCUS(IDC_EDIT_DIFFUSE, OnSetfocusEditDiffuse)
	ON_EN_SETFOCUS(IDC_EDIT_EMISSION, OnSetfocusEditEmission)
	ON_BN_CLICKED(IDC_MATERIAL_APPLY, OnClickedMaterialApply)
	ON_EN_SETFOCUS(IDC_EDIT_SPECULAR, OnSetfocusEditSpecular)
	ON_BN_CLICKED(IDC_MATERIAL_LOAD, OnClickedMaterialLoad)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DMaterial message handlers

void DMaterial::OnOK()
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void DMaterial::OnCancel()
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void DMaterial::OnClickedMaterialColor()
{
   COLORREF clr=RGB(0,0,0);
   
   Point c;
	CString cs;
	
	int len=180;
	GetDlgItemText(focus,cs.GetBuffer(len+1),len);

   CstringToPoint(cs,c);
	
	c *= 255.0;
	clr = RGB(int(c.x),int(c.y), int (c.z));

	CColorDialog dlgColor(clr);
	dlgColor.m_cc.Flags |= CC_FULLOPEN; // enable custom color section

	if (dlgColor.DoModal() == IDOK)
	{
		clr = dlgColor.GetColor();
		c.x = GetRValue(clr) / 255.0;
     	c.y = GetGValue(clr) / 255.0;
		c.z = GetBValue(clr) / 255.0;
	}
   CstringFromPoint(cs,c);
	SetDlgItemText(focus,cs);
   OnClickedMaterialApply();
}

void DMaterial::OnSetfocusEditAmbient()
{
	focus=IDC_EDIT_AMBIENT;
}

void DMaterial::OnSetfocusEditDiffuse()
{
	focus=IDC_EDIT_DIFFUSE;
}

void DMaterial::OnSetfocusEditSpecular()
{
	focus=IDC_EDIT_SPECULAR;
	
}

void DMaterial::OnSetfocusEditEmission()
{
	focus=IDC_EDIT_EMISSION;
}

void DMaterial::OnClickedMaterialApply()
{
	if (m_pView) {	 
		if (UpdateData(TRUE)) 
		{
	   	   m_pView->ApplyMaterial(*this);
	    }
	}
}


void DMaterial::OnClickedMaterialLoad()
{
	if (m_pView) {
	   m_pView->LoadMaterial();
	   m_pView->GetMaterial(*this);
	   UpdateData(FALSE);	// View state ==> to dialog
	}
}
