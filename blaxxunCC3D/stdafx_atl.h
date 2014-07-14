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
#if !defined(_stdafx_atl_h)
#define _stdafx_atl_h
/******************************************************************************
@doc

@module stdafx_atl.h - blaxxunCC3D OCX Control ATL standard header file|

Copyright (c) 1997 - 1999 by blaxxun interactive - Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:


Changes:

$Revision: $
$Log: $


Todo :

******************************************************************************/


#ifdef _COM

#define _WIN32_WINNT 0x0400
#define _ATL_APARTMENT_THREADED

#ifdef _DEBUG
// trace all query interface calls
// #define _ATL_DEBUG_QI
#endif

#include <atlbase.h>


// see ATL1.1 examples MFCATL
//

// We can implement the MFC/ATL lock count interaction in two different ways
// (you may comment/uncomment the one you want to try)

// ATL can blindly delegate all the ATL Lock()/Unlock() calls to MFC
/*
class CAtlGlobalModule : public CComModule
{
public:
	LONG Lock()
	{
		AfxOleLockApp();
		return 0;
	}
	LONG Unlock()
	{
		AfxOleUnlockApp();
		return 0;
	}
};
*/

// ATL can increment MFC's lock count the first time and decrement MFC's lock
// count when its lock count reaches zero.

class CAtlGlobalModule : public CComModule
{
public:
	LONG Lock()
	{
		if (GetLockCount()==0)
			AfxOleLockApp();
		return CComModule::Lock();
	}
	LONG Unlock()
	{
		LONG l = CComModule::Unlock();
		if (GetLockCount() == 0)
			AfxOleUnlockApp();
		return l;
	}
};

extern CAtlGlobalModule _Module;

#include <atlcom.h>
// ATL 1.1 #include <atlutil.h>


#if (_ATL_VER >= 0x0300)

#undef END_COM_MAP
// we have BIIIGGG problems with ATL 3.0 changes in VC 6.0
// END_COM_MAP in ATL 3.0 defines IUnknown members as abstract !!???

#ifdef _ATL_DEBUG_QI
#define END_COM_MAP()   {NULL, 0, 0}};\
	return &_entries[1];}
#else
#define END_COM_MAP()   {NULL, 0, 0}};\
	return _entries;}
#endif // _ATL_DEBUG_QI

#endif


#endif // _COM


#endif // !defined(_stdafx_atl_h)



