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
/******************************************************************************
@doc

@module blaxxunCC3D.cpp - Implementation of CGLViewCtrlApp and DLL registration..|

Copyright (c) 1997 - 1999 blaxxun interactive - Holger Grahn	
All rights reserved

Purpose:

Classes:

Notes:

Changes:
06.06.98 HG Ini file location in install directory (same as ocx)
17.09.98 HG One time Katmai initialization
10.02.99 HG added GvDB:term to clean up static allocated data to aid in Memory Debugging


$Revision: 1.24 $
$Log: blaxxuncc3d.cpp,v $
Revision 1.24  1999/07/06 17:02:16  tom
*** empty log message ***

Revision 1.23  1999/03/10 10:14:33  tom
*** empty log message ***



Todo :

******************************************************************************/


#include "stdafx.h"

#include "blaxxunCC3D.h"

#include "GVersion.h"

#include "WWW.h"


#include "GReporter.h"
#include "GTrace.h"

#include "GRender.h"
#include <GvDB.h>
#include "GModel.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CGLViewCtrlApp NEAR theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0x4b6e3010, 0x6e45, 0x11d0, { 0x93, 0x9, 0, 0x20, 0xaf, 0xe0, 0x5c, 0xc8 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


#ifdef _COM

// ATL Global Module only instance - declared in stdafx_atl.h
CAtlGlobalModule _Module;

// ATL GLobal Object Map
BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()


//	OBJECT_ENTRY(CLSID_ObjectTwo, CObjectTwo)
#endif

#ifdef _KATMAI

extern BOOL KatmaiInit();

#endif


//_PNH _set_new_handler( _PNH );

static _PNH cc3d_old_new_handler = (_PNH) NULL;
static int cc3d_old_new_mode=0;

int __cdecl cc3d_new_handler(size_t s)
{
	MessageBox(NULL,"Fatal Error : out of memory \nPlease terminate program " ,_PROGRAM,MB_OK);

	TRACE("Fatal Error: out of memory during alloaction of %d bytes\n",s);
	
	if (cc3d_old_new_handler) return cc3d_old_new_handler(s);
	ASSERT(0); // go into debugger
	return 0;
}



////////////////////////////////////////////////////////////////////////////
// CGLViewCtrlApp::InitInstance - DLL initialization

BOOL CGLViewCtrlApp::InitInstance()
{
	TRACE("CGLViewCtrlApp::InitInstance() \n");

	cc3d_old_new_handler=_set_new_handler(cc3d_new_handler); 
	cc3d_old_new_mode=_set_new_mode(1); // for malloc too 



#ifdef _KATMAI
	if (!KatmaiInit()) {
		// AfxMessageBox("This version requires an Intel Katmai processor \n Program will be terminated ");

		//MessageBox(NULL,"This version requires an Intel Katmai processor \n Program will be terminated " ,_PROGRAM,MB_OK);

		//return FALSE;
	}
	else { TRACE("CGLViewCtrlApp::Running on Intel Katmai processor \n"); }
#endif


	BOOL bInit = COleControlModule::InitInstance();
	
	// Initialize the ATL Module
	_Module.Init(ObjectMap,m_hInstance);

#if 0 // only for EXE's
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
#endif
	VERIFY(SUCCEEDED(_Module.RegisterServer(TRUE)));// ATL Classes
	//VERIFY(SUCCEEDED(_Module.RegisterTypeLib("glviewvrml.tlb")));// type lib


	if (bInit)
	{
		// TODO: Add your own module initialization code here.

		// set the default error reporting object
		GReporter::SetCurrent(new GReporter());
		GReporter::SetDefault(GReporter::current);

#ifdef _EAI_TRACE
		// for tracing EAI calls 
		if (GTrace::current == NULL) {
			FILE *f = fopen("\\temp\\glviewlog.txt","w");
			GTrace::SetCurrent(new GTrace(f));
		} 
#endif

		
		EAI_TRACE("CGLViewCtrlApp::InitInstance()\n");
		EAI_FLUSH();

		// make sure Gv VRML  database is init 
		GvDB::init();

		// set the cache directory 
		CString tmp;
		CString installDir;

		if (1) { // use local install directory 
			GetModuleFileName(AfxGetInstanceHandle( ), tmp.GetBuffer(_MAX_PATH),_MAX_PATH);
			tmp.ReleaseBuffer();
			int iBackslash = tmp.ReverseFind('\\');
			if (iBackslash != -1) 	tmp = tmp.Left(iBackslash+1);
			installDir = tmp;

		} else {
			GetTempPath(_MAX_PATH,tmp.GetBuffer(_MAX_PATH));
			tmp.ReleaseBuffer();
		}
		if (tmp.GetLength()>0) {
		  int l = tmp.GetLength();
		  if (tmp[l-1] != '\\')
			  tmp += '\\';
		}
		tmp += "cache";
		GFile::cacheDirectory = tmp;
		TRACE("Cache directory is : %s \n",(const char *)tmp);
		
		// set the new ini-file name (stored in the ccpro directory, name is cc3d.ini)	
		tmp = installDir + "cc3d.ini";
		if (m_pszProfileName)  free( (void*)m_pszProfileName );
		m_pszProfileName = _tcsdup( tmp );

		TRACE("Ini file is : %s \n",(const char *)tmp);

		if (m_pszAppName)  free( (void*)m_pszAppName );
		m_pszAppName = _tcsdup(_PROGRAM);


	}

	return bInit;
}


////////////////////////////////////////////////////////////////////////////
// CGLViewCtrlApp::ExitInstance - DLL termination

int CGLViewCtrlApp::ExitInstance()
{
	TRACE("CGLViewCtrlApp::ExitInstance()\n");

#ifdef _DEBUG

	GView::termClass();
	GvDB::term();

#endif 



	// TODO: Add your own module termination code here.
	EAI_TRACE("CGLViewCtrlApp::ExitInstance()\n");
	EAI_FLUSH();

	if (GReporter::defaultReporter) {
		delete GReporter::defaultReporter;
		GReporter::SetDefault(NULL);
	}

	// MFC's class factories registration is automatically revoked by MFC itself
	_Module.RevokeClassObjects(); // Revoke class factories for ATL
	_Module.Term();				  // cleanup ATL GLobal Module

	_set_new_handler(cc3d_old_new_handler); 
	_set_new_mode(cc3d_old_new_mode); // for malloc too 


	return COleControlModule::ExitInstance();
}


/*
  5.5.97 added this 2 calls, during explicit library link 
  where are these defined ?? 

*/
#if 0
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
//	AFX_MANAGE_STATE(_AfxGetOleModuleState());
	AFX_MANAGE_STATE(_afxModuleAddrThis);
	return AfxDllGetClassObject(rclsid, riid, ppv);
}

STDAPI DllCanUnloadNow(void)
{
//	AFX_MANAGE_STATE(_AfxGetOleModuleState());
	AFX_MANAGE_STATE(_afxModuleAddrThis);
	return AfxDllCanUnloadNow();
}

#endif

// declaration of global UID´s from GLViewVrml.idl

#include <initguid.h>

// RSX 
#include "GSound.h"

#ifdef G_RSX
#include "rsx.h"
#endif



#ifdef _COM
#include "blaxxunVRML.h"
#define IID_DEFINED
#include "blaxxunVRML_i.c"
#endif 



/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), LIBID_blaxxunVRMLLib, "blaxxunVRML.tlb")) {
//#ifdef _DEBUG
		MessageBox(NULL,"Can´t register blaxxunVRML.tlb type library\nEAI may not work",_PROGRAM,MB_OK);
	//	return ResultFromScode(SELFREG_E_TYPELIB);
//#endif
	}



	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}


/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!AfxOleUnregisterTypeLib(LIBID_blaxxunVRMLLib)) {
//#ifdef _DEBUG
		MessageBox(NULL,"Can´t unregister blaxxunVRML.tlb type library",_PROGRAM,MB_OK);
	//	return ResultFromScode(SELFREG_E_TYPELIB);
//#endif
	}

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}




