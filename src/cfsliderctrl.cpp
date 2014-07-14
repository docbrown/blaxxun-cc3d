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
// CFSliderCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "CFSliderCtrl.h"


/////////////////////////////////////////////////////////////////////////////
// CFSliderCtrl
// a sliderctrl connected to a float edit field


//	DECLARE_DYNAMIC(CFSliderCtrl)


	CFSliderCtrl::CFSliderCtrl(int idEdit,float *val) 
	{ r0 = 0.0; r1 = 1.0; m_idEdit = idEdit; m_val = val; additive=0; }

	void CFSliderCtrl::Init(int idEdit,float *val,float _r0,float _r1) 
		{  m_idEdit = idEdit; m_val = val; 
		   SetRange(_r0,_r1);
		   SetPos(*m_val);
		}



	void CFSliderCtrl::SetRange(float _r0, float _r1) {
					r0 = _r0; r1 = _r1;
					CSliderCtrl::SetRange(0,1000);
					CSliderCtrl::SetTicFreq(50);
			}

	float  CFSliderCtrl::GetPos() { return UnMap(CSliderCtrl::GetPos()); }

	void CFSliderCtrl::SetPos(float r) { CSliderCtrl::SetPos(Map(r)); }
	

	// set edit to new slider position 
	void CFSliderCtrl::OnScroll(CWnd* pDlgWnd) {
		float r = GetPos();
		
		if (m_val)  {
		   if (*m_val == r) return;	// nothing changed
		   *m_val = r;
 	 	   if (m_idEdit > 0) {
 	 	      DDX_Text(&CDataExchange(pDlgWnd, FALSE),m_idEdit, *m_val);
		   }
		}
	};
	
	// set slider & edit to new value
	void CFSliderCtrl::Set(CWnd* pDlgWnd, float r) {
		SetPos(r);
 	 	if (m_idEdit > 0) {
		   if (m_val) *m_val = r;
 	 	   DDX_Text(&CDataExchange(pDlgWnd, FALSE),m_idEdit, *m_val);
		}
	}
	
	// set position from edit 
	void CFSliderCtrl::SetFromEdit(CWnd* pDlgWnd) {
 	 	if (m_idEdit > 0) {
		   float r;
 	 	   DDX_Text(&CDataExchange(pDlgWnd, TRUE),m_idEdit, r);
   		   if (m_val) *m_val = r;
		   SetPos(r);
		}
	}
