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
// dstring.cpp : implementation file
//

#include "stdafx.h"
#include "glviewhdr.h"
#include "dstring.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DString dialog


DString::DString(const char *prompt,CString &value,CWnd* pParent /*=NULL*/) : 
   m_prompt(prompt), m_value(value) , m_valueRef(value),  CDialog(DString::IDD, pParent)
{
	//{{AFX_DATA_INIT(DString)
	//m_prompt = _T("");
	//m_value = _T("");
	//}}AFX_DATA_INIT
}


void DString::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DString)
	DDX_Text(pDX, IDC_PROMPT, m_prompt);
	DDX_Text(pDX, IDC_VALUE, m_value);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
		m_valueRef = m_value;
}


BEGIN_MESSAGE_MAP(DString, CDialog)
	//{{AFX_MSG_MAP(DString)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// DString message handlers

void DString::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void DString::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
