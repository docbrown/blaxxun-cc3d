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
// pixform.cpp : implementation file
//										   

#include "stdafx.h"
#include "glviewhdr.h"

#ifdef _OGL

#include "pixform.h"
#include "morepfd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif
#define WM_INIT WM_USER + 1

/////////////////////////////////////////////////////////////////////////////
// CPixForm dialog

CPixForm::CPixForm(CWnd* pParent /*=NULL*/)
	: CDialog(CPixForm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPixForm)
	m_accumbits = 0;
	m_alphabits = 0;
	m_auxbuffers = 0;
	m_colorbits = 0;
	m_dbufferdontcare = FALSE;
	m_depthbits = 0;
	m_doublebuffer = FALSE;
	m_drawbitmap = FALSE;
	m_drawwindow = FALSE;
	m_stencilbits = 0;
	m_stereo = FALSE;
	m_stereodontcare = FALSE;
	m_supportgdi = FALSE;
	m_supportopengl = FALSE;
	m_layertype = -1;
	m_pixeltype = -1;
	m_index = 1;
	m_swapcopy = FALSE;
	m_swapexchange = FALSE;
	m_needpalette = FALSE;
	m_needsystempalette = FALSE;
	m_genericformat = FALSE;
	//}}AFX_DATA_INIT
}

void CPixForm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPixForm)
	DDX_Text(pDX, IDC_accumbits, m_accumbits);
	DDV_MinMaxInt(pDX, m_accumbits, 0, 64);
	DDX_Text(pDX, IDC_ALPHABITS, m_alphabits);
	DDV_MinMaxInt(pDX, m_alphabits, 0, 32);
	DDX_Text(pDX, IDC_CAUXBUFFERS, m_auxbuffers);
	DDV_MinMaxInt(pDX, m_auxbuffers, 0, 32);
	DDX_Text(pDX, IDC_COLORBITS, m_colorbits);
	DDV_MinMaxInt(pDX, m_colorbits, 0, 32);
	DDX_Check(pDX, IDC_DBUFFERDONTCARE, m_dbufferdontcare);
	DDX_Text(pDX, IDC_DEPTHBITS, m_depthbits);
	DDV_MinMaxInt(pDX, m_depthbits, 0, 32);
	DDX_Check(pDX, IDC_DOUBLEBUFFER, m_doublebuffer);
	DDX_Check(pDX, IDC_DRWBITMAP, m_drawbitmap);
	DDX_Check(pDX, IDC_DRWWINDOW, m_drawwindow);
	DDX_Text(pDX, IDC_STENCILBITS, m_stencilbits);
	DDV_MinMaxInt(pDX, m_stencilbits, 0, 32);
	DDX_Check(pDX, IDC_STEREO, m_stereo);
	DDX_Check(pDX, IDC_STEREODONTCARE, m_stereodontcare);
	DDX_Check(pDX, IDC_SUPPORTGDI, m_supportgdi);
	DDX_Check(pDX, IDC_SUPPORTOPENGL, m_supportopengl);
	DDX_Radio(pDX, IDC_MAINPLANE, m_layertype);
	DDX_Radio(pDX, IDC_TYPERGBA, m_pixeltype);
	DDX_Text(pDX, IDC_INDEX, m_index);
	DDV_MinMaxInt(pDX, m_index, 1, 24);
	DDX_Check(pDX, IDC_SWAP_COPY, m_swapcopy);
	DDX_Check(pDX, IDC_SWAP_EXCHANGE, m_swapexchange);
	DDX_Check(pDX, IDC_NEED_PALETTE, m_needpalette);
	DDX_Check(pDX, IDC_NEED_SYSTEM_PALETTE, m_needsystempalette);
	DDX_Check(pDX, IDC_GENERIC_FORMAT, m_genericformat);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPixForm, CDialog)
	//{{AFX_MSG_MAP(CPixForm)
	ON_BN_CLICKED(IDC_MOREPFD, OnClickedMorepfd)
	ON_BN_CLICKED(IDC_LAST_PFD, OnClickedLastPfd)
	ON_BN_CLICKED(IDC_NEXT_PFD, OnClickedNextPfd)
	ON_BN_CLICKED(IDC_END_PFD, OnClickedEndPfd)
	ON_BN_CLICKED(IDC_FIRST_PFD, OnClickedFirstPfd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPixForm message handlers

void CPixForm::OnOK()
{
	int nNewIdx;
	DWORD dwFlags = (DWORD)0;
    CView *cview = CMyglView::GetView();

	if (m_nID == ID_PIXFMT_CHOOSE)
	{
      memset(&m_glptr->m_PixFmtDesc, 0, sizeof(PIXELFORMATDESCRIPTOR));

	  UpdateData();

	  dwFlags |= (m_drawbitmap)      ? PFD_DRAW_TO_BITMAP        : 0;
	  dwFlags |= (m_drawwindow)      ? PFD_DRAW_TO_WINDOW        : 0;
	  dwFlags |= (m_supportgdi)      ? PFD_SUPPORT_GDI           : 0;
	  dwFlags |= (m_supportopengl)   ? PFD_SUPPORT_OPENGL        : 0;
	  dwFlags |= (m_doublebuffer)    ? PFD_DOUBLEBUFFER          : 0;
	  dwFlags |= (m_dbufferdontcare) ? PFD_DOUBLEBUFFER_DONTCARE : 0;
	  dwFlags |= (m_stereo)          ? PFD_STEREO                : 0;
	  dwFlags |= (m_stereodontcare)  ? PFD_STEREO_DONTCARE       : 0;
	  dwFlags |= (m_genericformat)   ? PFD_GENERIC_FORMAT        : 0;
	  dwFlags |= (m_needpalette)     ? PFD_NEED_PALETTE          : 0;
      dwFlags |= (m_needsystempalette) ? PFD_NEED_SYSTEM_PALETTE : 0;
	  dwFlags |= (m_swapexchange)    ? PFD_SWAP_EXCHANGE      : 0;
	  dwFlags |= (m_swapcopy)        ? PFD_SWAP_COPY      : 0;

	  m_glptr->m_PixFmtDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	  m_glptr->m_PixFmtDesc.nVersion = 1;
	  m_glptr->m_PixFmtDesc.dwFlags = dwFlags;
	  m_glptr->m_PixFmtDesc.iPixelType = (m_pixeltype == 0) ? PFD_TYPE_RGBA : PFD_TYPE_COLORINDEX;
	  m_glptr->m_PixFmtDesc.cColorBits = m_colorbits;
	  m_glptr->m_PixFmtDesc.cAlphaBits = m_alphabits;
	  m_glptr->m_PixFmtDesc.cAccumBits = m_accumbits;
	  m_glptr->m_PixFmtDesc.cDepthBits = m_depthbits;
	  m_glptr->m_PixFmtDesc.cStencilBits = m_stencilbits;
	  m_glptr->m_PixFmtDesc.cAuxBuffers = m_auxbuffers;

	  nNewIdx = m_glptr->ChoosePixelFormat(m_glptr->m_hdc, &m_glptr->m_PixFmtDesc);

	  if ((m_glptr->IsNativeIndex(m_glptr->m_hdc, nNewIdx)) || (m_glptr->IsDeviceIndex(m_glptr->m_hdc, nNewIdx))) {
		// HG Release old GLRC
		m_glptr->ReleaseGLRC(); // cview->m_hWnd); // m_glptr->m_hdc);

	    if (m_glptr->GetGLRC(m_glptr->m_hdc)) {
			m_glptr->SetSurfaceNotInitialized();
		} else {
		  // HG	
	      MessageBox("Sorry, Pixel Format can not be changed", NULL, MB_OK | MB_ICONEXCLAMATION);
		}
	  }
	  else					         
	    MessageBox("Unsupported Pixel Format", NULL, MB_OK | MB_ICONEXCLAMATION);

	  cview->CWnd::InvalidateRect(NULL, TRUE);
	}
	CDialog::OnOK();
}
	 
/////////////////////////////////////////////////////////////////////////////
// OnInitDialog
BOOL CPixForm::OnInitDialog()
{		
	CDialog::OnInitDialog();
	m_nNextID = 1;
    CWnd *cwndMorePFD = GetDlgItem(IDC_MOREPFD);
	CWnd *cwndNext = GetDlgItem(IDC_NEXT_PFD);
	CWnd *cwndLast = GetDlgItem(IDC_LAST_PFD);
	CWnd *cwndEnd = GetDlgItem(IDC_END_PFD);
	CWnd *cwndFirst = GetDlgItem(IDC_FIRST_PFD);
	HGLRC hglrc = m_glptr->wglGetCurrentContext();
	int nCurIdx = (m_nID == ID_PIXFMT_CHOOSE && m_glptr->m_hglrc) ? 
	               m_glptr->GetPixelFormat(m_glptr->m_hdc) : m_nNextID++;

	CenterWindow(NULL);

	SetDlgItemInt(IDC_INDEX, nCurIdx, TRUE);

    //if there is currently a valid rendering context or we are enumerating
	if (hglrc || m_nID == ID_PIXFMT_ENUM)
	{
	  if (nCurIdx > 0)
	  {
	    //if there is a valid pixel format associated with the window
		//then update controls in dialog
	    if (m_glptr->DescribePixelFormat(m_glptr->m_hdc, 
	        nCurIdx, sizeof(PIXELFORMATDESCRIPTOR), &m_glptr->m_PixFmtDesc))
		{
		  UpdateDlg(&m_glptr->m_PixFmtDesc, nCurIdx);
    	  cwndMorePFD->EnableWindow(TRUE);
		  //if we are enumerating then enable the controls
		  //for browsing the pixel formats
	      if (m_nID == ID_PIXFMT_ENUM)
	      {
  	        cwndNext ->EnableWindow(TRUE);
	        cwndLast ->EnableWindow(TRUE);
  	        cwndEnd  ->EnableWindow(TRUE);
	        cwndFirst->EnableWindow(TRUE);
	      }
		}

	  }
	}
	//a pixel format has not been setup for the window so
	//simply set some defaults
	else
	{
      CheckRadioButton( IDC_OVERLAYPLANE, IDC_UNDERLAYPLANE, IDC_MAINPLANE);
      CheckRadioButton( IDC_TYPERGBA, IDC_TYPECOLORINDEX, IDC_TYPERGBA);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

/////////////////////////////////////////////////////////////////////////////
// OnClickedMorepfd
void CPixForm::OnClickedMorepfd()
{
	CMorePFD MorePFD;
	PIXELFORMATDESCRIPTOR pfd;

	HGLRC hglrc = m_glptr->m_hglrc;
	HDC   hdc   = m_glptr->m_hdc;
	int nCurIdx = (m_nID == ID_PIXFMT_CHOOSE && hdc) ? m_glptr->GetPixelFormat(hdc) : m_nNextID;

	if (hglrc || m_nID == ID_PIXFMT_ENUM)
	{
	  if (nCurIdx > 0)
	  {
	    if (m_glptr->DescribePixelFormat(hdc, nCurIdx, sizeof(PIXELFORMATDESCRIPTOR), &pfd))
		{
	      MorePFD.m_ppfd = (LPPIXELFORMATDESCRIPTOR)&pfd;
	      MorePFD.DoModal();
		}
	  }
	}
}


/////////////////////////////////////////////////////////////////////////////
// OnClickedLastPfd
void CPixForm::OnClickedLastPfd()
{
  GOpenglDeviceWGL gl;

  PIXELFORMATDESCRIPTOR pfd;
  HDC   hdc   = m_glptr->m_hdc;
  int nID = (m_nNextID > 1) ?  --m_nNextID : 1;

  if (m_glptr->DescribePixelFormat(hdc, nID, sizeof(PIXELFORMATDESCRIPTOR), &pfd))
    UpdateDlg(&pfd, nID);

}

/////////////////////////////////////////////////////////////////////////////
// OnClickedNextPfd
void CPixForm::OnClickedNextPfd()
{
  GOpenglDeviceWGL gl;
  PIXELFORMATDESCRIPTOR pfd;
  HDC   hdc   = m_glptr->m_hdc;
  int nMaxPFD = m_glptr->GetMaxPFIndex(hdc);
  int nID = (m_nNextID < nMaxPFD) ?  ++m_nNextID : nMaxPFD;

  if (m_glptr->DescribePixelFormat(hdc, nID, sizeof(PIXELFORMATDESCRIPTOR), &pfd))
    UpdateDlg(&pfd, nID);
}

/////////////////////////////////////////////////////////////////////////////
// OnClickedEndPfd
void CPixForm::OnClickedEndPfd()
{
  GOpenglDeviceWGL gl;
  PIXELFORMATDESCRIPTOR pfd;
  HDC   hdc   = m_glptr->m_hdc;
  int nID = m_nNextID = 24;

  if (m_glptr->DescribePixelFormat(hdc, nID, sizeof(PIXELFORMATDESCRIPTOR), &pfd))
    UpdateDlg(&pfd, nID);
}

/////////////////////////////////////////////////////////////////////////////
// OnClickedFirstPfd
void CPixForm::OnClickedFirstPfd()
{
  GOpenglDeviceWGL gl;
  PIXELFORMATDESCRIPTOR pfd;
  HDC   hdc   = m_glptr->m_hdc;
  int nID = 1;
  m_nNextID = nID;
  

  if (m_glptr->DescribePixelFormat(hdc, nID, sizeof(PIXELFORMATDESCRIPTOR), &pfd))
    UpdateDlg(&pfd, nID);

}

/////////////////////////////////////////////////////////////////////////////
// UpdateDlg
void CPixForm::UpdateDlg(PIXELFORMATDESCRIPTOR *pfd, int idx)
{
  int i, j, nCheck;

  for (i = PFD_DOUBLEBUFFER, j = IDC_DOUBLEBUFFER; i <= PFD_SUPPORT_OPENGL; i <<= 1, j++)
  {
     nCheck = (pfd->dwFlags & i) ? 1 : 0;
	  CheckDlgButton(j, nCheck);
  }

  nCheck = (pfd->dwFlags & PFD_DOUBLEBUFFER_DONTCARE) ? 1 : 0;
  CheckDlgButton(IDC_DBUFFERDONTCARE, nCheck); 
    		
  nCheck = (pfd->dwFlags & PFD_STEREO_DONTCARE) ? 1 : 0;
  CheckDlgButton(IDC_STEREODONTCARE, 1); 
  
  CheckDlgButton(IDC_GENERIC_FORMAT, (pfd->dwFlags & PFD_GENERIC_FORMAT) ? 1 : 0); 
  CheckDlgButton(IDC_NEED_PALETTE, (pfd->dwFlags & PFD_NEED_PALETTE) ? 1 : 0); 
  CheckDlgButton(IDC_NEED_SYSTEM_PALETTE, (pfd->dwFlags & PFD_NEED_SYSTEM_PALETTE) ? 1 : 0); 
  CheckDlgButton(IDC_SWAP_EXCHANGE, (pfd->dwFlags & PFD_SWAP_EXCHANGE) ? 1 : 0); 
  CheckDlgButton(IDC_SWAP_COPY, (pfd->dwFlags & PFD_SWAP_COPY) ? 1 : 0); 

  SetDlgItemInt(IDC_COLORBITS, pfd->cColorBits, TRUE);
  SetDlgItemInt(IDC_ALPHABITS, pfd->cAlphaBits, TRUE);
  SetDlgItemInt(IDC_accumbits, pfd->cAccumBits, TRUE);
  SetDlgItemInt(IDC_DEPTHBITS, pfd->cDepthBits, TRUE);
  SetDlgItemInt(IDC_STENCILBITS, pfd->cStencilBits, TRUE);
  SetDlgItemInt(IDC_CAUXBUFFERS, pfd->cAuxBuffers, TRUE);
  SetDlgItemInt(IDC_INDEX, idx, TRUE);

  for (i = PFD_UNDERLAY_PLANE, j = IDC_UNDERLAYPLANE; i <= PFD_MAIN_PLANE; i++, j--)
  {
    if (pfd->iLayerType == i)
      CheckRadioButton( IDC_MAINPLANE, IDC_UNDERLAYPLANE, j);
  }

  for (i = PFD_TYPE_RGBA, j = IDC_TYPERGBA; i <= PFD_TYPE_COLORINDEX; i++, j++)
  {
    if (pfd->iPixelType == i)
      CheckRadioButton( IDC_TYPERGBA, IDC_TYPECOLORINDEX, j);
  }

}

#endif
