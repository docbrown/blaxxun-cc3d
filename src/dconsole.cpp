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
// DConsole.cpp : implementation file
//

#include "stdafx.h"

#include "resource.h"

#include "glviewhdr.h"


#include "DConsole.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DConsole dialog


DConsole::DConsole(CWnd* pParent /*=NULL*/)
	: CDialog(DConsole::IDD, pParent)
{
	m_hide = FALSE;
	//{{AFX_DATA_INIT(DConsole)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DConsole::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DConsole)
	DDX_Control(pDX, IDC_TEXT, m_text);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DConsole, CDialog)
	//{{AFX_MSG_MAP(DConsole)
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	ON_BN_CLICKED(IDC_COPY, OnCopy)
	ON_EN_ERRSPACE(IDC_TEXT, OnErrspaceText)
	ON_BN_CLICKED(IDHIDE, OnHide)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DConsole message handlers
void DConsole::AddText(const TCHAR *text) 
{
	CString newText;

	const TCHAR *s=text;

	while (*s) { // replace \n by \r\n
		TCHAR *e = strchr(s,'\n');
	
		if (e) {
			newText += CString(s,e-s);
			s = e+1;
			newText += "\r\n";
		} else {
			newText += s;
			break;
		}
	}
	newText += "\r\n";
	int l=	m_text.GetWindowTextLength();
	m_text.SetSel( l+1, -1); 
	m_text.ReplaceSel(newText); 
	l=	m_text.GetWindowTextLength();
	m_text.SetSel( l+1, -1,FALSE); 
}

void DConsole::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void DConsole::OnClear() 
{
	m_text.SetSel(0,-1,TRUE); 
	m_text.ReplaceSel(""); 
	m_text.Clear();
}

void DConsole::OnCopy() 
{
	int l=	m_text.GetWindowTextLength();
	m_text.SetSel(0,-1,TRUE); 
	m_text.Copy();
	m_text.SetSel( l+1, -1); 
}

void DConsole::OnErrspaceText() 
{
	// TODO: Add your control notification handler code here
	OnClear();
	
}

void DConsole::OnHide() 
{
	// TODO: Add your control notification handler code here
	m_hide = TRUE;
	CDialog::OnOK();
	
}
