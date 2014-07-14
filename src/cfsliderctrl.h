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
#ifndef _CFSLIDER_CTRL_
#define _CFSLIDER_CTRL_
/////////////////////////////////////////////////////////////////////////////
// CFSliderCtrl
// a sliderctrl connected to a float edit field


class CFSliderCtrl : public CSliderCtrl 
{
//	DECLARE_DYNAMIC(CFSliderCtrl)

public : 

	float r0,r1;
	
	int m_idEdit;
	float *m_val;
	int additive;

	CFSliderCtrl(int idEdit = -1,float *val = NULL);
	void Init(int idEdit = -1,float *val = NULL,float _r0=0.0,float _r1=1.0); 


	// map position form float to int 
	int Map( float r)  {return(1000.0 * (r - r0)  / (r1-r0));}
	float UnMap(int r) { return (r0 + (r)/1000.0 * (r1-r0)); }

	void SetRange(float _r0 = 0.0, float _r1 = 1.0);
	float  GetPos();

	void SetPos(float r);
	

	// set edit to new slider position 
	void OnScroll(CWnd* pDlgWnd);
	
	// set slider & edit to new value
	void Set(CWnd* pDlgWnd, float r);
	
	// set position from edit 
	void SetFromEdit(CWnd* pDlgWnd);
};

#endif
