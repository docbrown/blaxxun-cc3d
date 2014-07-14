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
// dtexture.cpp : implementation file
//

#include "stdafx.h"
#include "glviewhdr.h"

#include "resource.h"

#include "dtexture.h"
#include "grender.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DTexture dialog

DTexture::DTexture(CWnd* pParent /*=NULL*/,CMyglView *pView /* = NULL */)
	: CDialog(DTexture::IDD, pParent)
{
	m_pView = pView;

	//{{AFX_DATA_INIT(DTexture)
	m_tfilter_max = "";
	m_tfilter_min = "";
	m_tfunction = "";
	m_repeat_t = FALSE;
	m_repeat_s = FALSE;
	m_gen_mipmaps = FALSE;
	m_rotate = "";
	m_scale = "";
	m_translate = "";
	m_white = FALSE;
	m_perspective_hint = FALSE;
	m_max_size = 0;
	m_tblend = _T("");
	//}}AFX_DATA_INIT
}

void DTexture::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	if (!pDX->m_bSaveAndValidate) {
	  m_pView->GetTexture(*this);
	}

	//{{AFX_DATA_MAP(DTexture)
	DDX_CBString(pDX, IDC_COMBO_TFILTER_MAX, m_tfilter_max);
	DDX_CBString(pDX, IDC_COMBO_TFILTER_MIN, m_tfilter_min);
	DDX_CBString(pDX, IDC_COMBO_TFUNCTION, m_tfunction);
	DDX_Check(pDX, IDC_CHECK_TREPEAT_T, m_repeat_t);
	DDX_Check(pDX, IDC_CHECK_TREPEAT_S, m_repeat_s);
	DDX_Check(pDX, IDC_TEXTURE_GEN_MIPMAPS, m_gen_mipmaps);
	DDX_Text(pDX, IDC_TEXTURE_ROTATE, m_rotate);
	DDX_Text(pDX, IDC_TEXTURE_SCALE, m_scale);
	DDX_Text(pDX, IDC_TEXTURE_TRANSLATE, m_translate);
	DDX_Check(pDX, IDC_TEXTURE_WHITE, m_white);
	DDX_Check(pDX, IDC_PERSPECTIVE_HINT, m_perspective_hint);
	DDX_Text(pDX, IDC_TEXTURE_MAX_SIZE, m_max_size);
	DDV_MinMaxInt(pDX, m_max_size, 0, 12000);
	DDX_CBString(pDX, IDC_COMBO_TBLEND, m_tblend);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(DTexture, CDialog)
	//{{AFX_MSG_MAP(DTexture)
	ON_BN_CLICKED(IDC_TEXTURE_RESET, OnClickedTextureReset)
	ON_BN_CLICKED(IDC_APPLY, OnClickedApply)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SCALE_SPIN, OnDeltaposScaleSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_ROTATEZ_SPIN, OnDeltaposRotatezSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_TRANSLATEX_SPIN, OnDeltaposTranslateXSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_TRANSLATEY_SPIN, OnDeltaposTranslateYSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DTexture message handlers

void DTexture::OnOK()
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void DTexture::OnCancel()
{
	CDialog::OnCancel();
}

void DTexture::OnClickedTextureReset()
{
		SetDlgItemText(IDC_TEXTURE_ROTATE,"0 0 0");
		SetDlgItemText(IDC_TEXTURE_SCALE,"1 1 1");
		SetDlgItemText(IDC_TEXTURE_TRANSLATE,"0 0 0");
    OnClickedApply();

}

void DTexture::OnClickedApply()
{
	if (!UpdateData(TRUE))
	{
		TRACE0("UpdateData failed during dialog apply\n");
		// the UpdateData routine will set focus to correct item
		return;
	}
	if (m_pView) m_pView->ApplyTexture(*this);
}


//    GetDlgItemText(IDC_TEXTURE_SCALE,buf,sizeof(buf));
//    sscanf(buf,"%f %f %f",&scale.x,&scale.y,&scale.z);


void DTexture::OnDeltaposScaleSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    char buf[120];
    Point val;
  
    GetDlgItemText(IDC_TEXTURE_SCALE,buf,sizeof(buf));
    sscanf(buf,"%f %f %f",&val.x,&val.y,&val.z);

    int delta = pNMUpDown->iDelta;
    if (delta <0) 
     val /= 0.5 * -delta ; // *0.5 * -delta;
    else if (delta >0) 
     val *= 0.5* delta;
    else return;

    sprintf(buf,"%f %f %f",val.x,val.y,val.z);
    SetDlgItemText(IDC_TEXTURE_SCALE,buf);

    *pResult = 0;
    OnClickedApply();
}

void DTexture::OnDeltaposRotatezSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

    char buf[120];
    Point val;
  
    GetDlgItemText(IDC_TEXTURE_ROTATE,buf,sizeof(buf));
    sscanf(buf,"%f %f %f",&val.x,&val.y,&val.z);

    int delta = pNMUpDown->iDelta;
    if (delta != 0) 
     val.z += 2.5 * delta;
    else return;

    sprintf(buf,"%f %f %f",val.x,val.y,val.z);
    SetDlgItemText(IDC_TEXTURE_ROTATE,buf);

    *pResult = 0;
    OnClickedApply();
}


void DTexture::OnDeltaposTranslateXSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    char buf[120];
    Point val;
  
    GetDlgItemText(IDC_TEXTURE_TRANSLATE,buf,sizeof(buf));
    sscanf(buf,"%f %f %f",&val.x,&val.y,&val.z);

    int delta = pNMUpDown->iDelta;
    if (delta != 0) 
       val.x += 0.05 * delta;
    else return;

    sprintf(buf,"%f %f %f",val.x,val.y,val.z);
    SetDlgItemText(IDC_TEXTURE_TRANSLATE,buf);

    *pResult = 0;
    OnClickedApply();

}

void DTexture::OnDeltaposTranslateYSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    char buf[120];
    Point val;
  
    GetDlgItemText(IDC_TEXTURE_TRANSLATE,buf,sizeof(buf));
    sscanf(buf,"%f %f %f",&val.x,&val.y,&val.z);

    int delta = pNMUpDown->iDelta;
    if (delta != 0) 
       val.y += 0.05 * delta;
    else return;

    sprintf(buf,"%f %f %f",val.x,val.y,val.z);
    SetDlgItemText(IDC_TEXTURE_TRANSLATE,buf);

    *pResult = 0;
    OnClickedApply();
}



/////////////////////////////////////////////////////////////////////////////
// DParameter dialog


DParameter::DParameter(CWnd* pParent /*=NULL*/,CMyglView *pView /* = NULL */)
	: CDialog(DParameter::IDD, pParent)
{
	m_pView = pView;
	//{{AFX_DATA_INIT(DParameter)
	m_function = _T("");
	m_source = _T("");
	m_normalize = FALSE;
	m_mapping = _T("");
	m_showSurface = FALSE;
	m_rotate = _T("");
	m_scale = _T("");
	m_translate = _T("");
	m_enable = FALSE;
	m_showMappedObject = FALSE;
	//}}AFX_DATA_INIT
}


void DParameter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	if (!pDX->m_bSaveAndValidate) {
	  m_pView->GetParameter(*this);
	}

	//{{AFX_DATA_MAP(DParameter)
	DDX_CBString(pDX, IDC_COMBO_PFUNCTION, m_function);
	DDX_CBString(pDX, IDC_COMBO_PSOURCE, m_source);
	DDX_Check(pDX, IDC_CHECK_NORMALIZE, m_normalize);
	DDX_CBString(pDX, IDC_COMBO_PMAPPING, m_mapping);
	DDX_Check(pDX, IDC_CHECK_MAPPING_VISIBLE, m_showSurface);
	DDX_Text(pDX, IDC_MAPPING_ROTATE, m_rotate);
	DDX_Text(pDX, IDC_MAPPING_SCALE, m_scale);
	DDX_Text(pDX, IDC_MAPPING_TRANSLATE, m_translate);
	DDX_Check(pDX, IDC_PARAMETER_ENABLE, m_enable);
	DDX_Check(pDX, IDC_CHECK_MAPPED_OBJECT_VISIBLE, m_showMappedObject);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DParameter, CDialog)
	//{{AFX_MSG_MAP(DParameter)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_BN_CLICKED(IDC_MAPPING_RESET, OnMappingReset)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// DParameter message handlers

void DParameter::OnApply() 
{
	if (!UpdateData(TRUE))
	{
		TRACE0("UpdateData failed during dialog apply\n");
		// the UpdateData routine will set focus to correct item
		return;
	}
	if (m_pView) m_pView->ApplyParameter(*this);
}

void DParameter::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void DParameter::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
	if (m_pView) m_pView->ApplyParameter(*this); //17.06.96


}

void DParameter::OnMappingReset() 
{
	// TODO: Add your control notification handler code here
		SetDlgItemText(IDC_MAPPING_ROTATE,"0 0 0");
		SetDlgItemText(IDC_MAPPING_SCALE,"1 1 1");
		SetDlgItemText(IDC_MAPPING_TRANSLATE,"0 0 0");
	
}


