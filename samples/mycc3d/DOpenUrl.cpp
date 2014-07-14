// DOpenUrl.cpp : implementation file
//

#include "stdafx.h"
#include "MyCC3D.h"
#include "DOpenUrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DOpenUrl dialog


DOpenUrl::DOpenUrl(CWnd* pParent /*=NULL*/)
	: CDialog(DOpenUrl::IDD, pParent)
{
	//{{AFX_DATA_INIT(DOpenUrl)
	m_url = _T("");
	//}}AFX_DATA_INIT
}


void DOpenUrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DOpenUrl)
	DDX_Text(pDX, IDC_URL, m_url);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DOpenUrl, CDialog)
	//{{AFX_MSG_MAP(DOpenUrl)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DOpenUrl message handlers
