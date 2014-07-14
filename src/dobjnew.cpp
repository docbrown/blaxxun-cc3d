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
// dobjnew.cpp : implementation file
//

#include "stdafx.h"
#include "glviewhdr.h"

#include "dobjnew.h"
// ???
#include "resource.h"



#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DObjNew dialog


DObjNew::DObjNew(CWnd* pParent /*=NULL*/,CMyglView *pView /* = NULL */)
	: CDialog(DObjNew::IDD, pParent)
{
	m_pView = pView;
	//{{AFX_DATA_INIT(DObjNew)
	m_stepsx = 32;
	m_stepsy = 16;
	m_type = _T("ronny");
	m_umax = 1.0f;
	m_umin = 0.0f;
	m_vmax = 1.0f;
	m_vmin = 0.0f;
	m_h = 2.0f;
	m_r = 1.0f;
	m_height = 2.0f;
	m_append = FALSE;
	//}}AFX_DATA_INIT
}


void DObjNew::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	if (!pDX->m_bSaveAndValidate) {
	   m_pView->GetObject(*this);
	}
	//{{AFX_DATA_MAP(DObjNew)
	DDX_Text(pDX, IDC_STEPSX, m_stepsx);
	DDV_MinMaxInt(pDX, m_stepsx, 1, 5000);
	DDX_Text(pDX, IDC_STEPSY, m_stepsy);
	DDV_MinMaxInt(pDX, m_stepsy, 1, 5000);
	DDX_CBString(pDX, IDC_TYPE, m_type);
	DDV_MaxChars(pDX, m_type, 60);
	DDX_Text(pDX, IDC_UMAX, m_umax);
	DDX_Text(pDX, IDC_UMIN, m_umin);
	DDX_Text(pDX, IDC_VMAX, m_vmax);
	DDX_Text(pDX, IDC_VMIN, m_vmin);
	DDX_Text(pDX, IDC_RADIUS, m_r);
	DDX_Text(pDX, IDC_HEIGHT, m_height);
	DDX_Check(pDX, IDC_APPEND_SCENE, m_append);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate) {
	   m_pView->ApplyObject(*this);
	}
}


BEGIN_MESSAGE_MAP(DObjNew, CDialog)
	//{{AFX_MSG_MAP(DObjNew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// DObjNew message handlers

void DObjNew::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void DObjNew::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
/////////////////////////////////////////////////////////////////////////////
// DNewFontLogo dialog


DNewFontLogo::DNewFontLogo(CWnd* pParent /*=NULL*/,CMyglView *pView /* = NULL */)
	: CDialog(DNewFontLogo::IDD, pParent)
{
	m_pView = pView;
	//m_lf.lfHeight = 0;
    
    //strcpy(m_lf.lfFaceName,"Arial");
	 
	 // Let's fill out a LOGFONT structure so that we can have an initial font.
	    m_lf.lfHeight=40;
	    m_lf.lfWidth=0;
	    m_lf.lfEscapement=m_lf.lfOrientation=0;
	    m_lf.lfWeight=FW_NORMAL;
	    m_lf.lfItalic=0;
	    m_lf.lfUnderline=0;
	    m_lf.lfStrikeOut=0;
	    m_lf.lfCharSet=ANSI_CHARSET;
	    m_lf.lfOutPrecision=OUT_TT_ONLY_PRECIS;
	    m_lf.lfClipPrecision=CLIP_DEFAULT_PRECIS;
	    m_lf.lfQuality=DEFAULT_QUALITY;
	    m_lf.lfPitchAndFamily=FF_DONTCARE;
	    lstrcpy(m_lf.lfFaceName,"Arial");


	//{{AFX_DATA_INIT(DNewFontLogo)
	m_extrude = TRUE;
	m_extrusion_factor = 1.0f;
	m_text = _T("GLView");
	m_top = TRUE;
	m_font_descr = _T("");
	m_bottom = TRUE;
	m_parameters = FALSE;
	m_invert_factor = 2.0f;
	m_invert = FALSE;
	m_align = -1;
	m_outline = FALSE;
	//}}AFX_DATA_INIT

}


void DNewFontLogo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	if (!pDX->m_bSaveAndValidate) {
    	m_font_descr = m_lf.lfFaceName;
	}

	
	//{{AFX_DATA_MAP(DNewFontLogo)
	DDX_Check(pDX, IDC_EXTRUDE, m_extrude);
	DDX_Text(pDX, IDC_EXTRUSION_FACTOR, m_extrusion_factor);
	DDX_Text(pDX, IDC_TEXT, m_text);
	DDX_Check(pDX, IDC_TOP, m_top);
	DDX_Text(pDX, IDC_FONT_DESCR, m_font_descr);
	DDX_Check(pDX, IDC_BOTTOM, m_bottom);
	DDX_Check(pDX, IDC_PARAMETERS, m_parameters);
	DDX_Text(pDX, IDD_INVERT_FACTOR, m_invert_factor);
	DDX_Check(pDX, IDC_Invert, m_invert);
	DDX_Radio(pDX, IDC_ALIGN_LEFT, m_align);
	DDX_Check(pDX, IDC_OUTLINE, m_outline);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DNewFontLogo, CDialog)
	//{{AFX_MSG_MAP(DNewFontLogo)
	ON_BN_CLICKED(IDC_FONT, OnFont)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// Choose a font using the standard font dialog
int DNewFontLogo::ChooseFont()
{
   if (m_lf.lfHeight == 0) {
   		// get current font description
   		CFont* pFont = m_pView->GetFont();

   		if (pFont != NULL)
	   		pFont->GetObject(sizeof(LOGFONT), &m_lf);
   		else
	   		::GetObject(GetStockObject(SYSTEM_FONT), sizeof(LOGFONT), &m_lf);
   	}
	
	CFontDialog dlg(&m_lf, CF_TTONLY | CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT);

	if (dlg.DoModal() == IDOK)
	{

	 	m_font_descr = m_lf.lfFaceName;
	 	return(1);
	}
	else return(0);
}


/////////////////////////////////////////////////////////////////////////////
// DNewFontLogo message handlers

void DNewFontLogo::OnFont() 
{
	if (ChooseFont())
	{
   	   SetDlgItemText(IDC_FONT_DESCR,m_font_descr);
	}
}
