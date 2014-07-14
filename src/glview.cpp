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

@module GLView.cpp - GLView MFC application class|

Copyright (c) 1995	by Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:
   	app.OnIdle message is passes to active view 

Changes:

Todo :

******************************************************************************/

// GLView.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "glviewhdr.h"

#include "gversion.h"

#include "resource.h"
//#include "glview.h"

#include "mainfrm.h"
//#include "glvdoc.h"
//#include "glvview.h"

#include "greporter.h"


// GvLib Database
#include <gvdb.h>

// CoCreate
#include <objbase.h>


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMyglApp

BEGIN_MESSAGE_MAP(CMyglApp, CWinApp)
	//{{AFX_MSG_MAP(CMyglApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_COMMAND(ID_OPTIONS_REGISTER_FILETYPES, OnRegisterFiletypes)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyglApp construction

CMyglApp::CMyglApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMyglApp object

CMyglApp NEAR theApp;

static BOOL SetRegKey(LPCTSTR lpszKey, LPCTSTR lpszValue)
{
	if (::RegSetValue(HKEY_CLASSES_ROOT, lpszKey, REG_SZ,
		  lpszValue, lstrlen(lpszValue)) != ERROR_SUCCESS)
	{
		TRACE1("Warning: registration database update failed for key '%s'.\n",
			lpszKey);
		return FALSE;
	}
	return TRUE;
}

static BOOL SetRegKey(HKEY hkey,LPCTSTR lpszKey, LPCTSTR lpszValue)
{
	if (::RegSetValueEx(hkey, lpszKey, 0, REG_SZ,
		  (const BYTE*) lpszValue, lstrlen(lpszValue)*sizeof(TCHAR)) != ERROR_SUCCESS)
	{
		TRACE1("Warning: registration database update failed for key '%s'.\n",
			lpszKey);
		return FALSE;
	}
	return TRUE;
}

static BOOL SetRegKey(HKEY hkey,LPCTSTR lpszSubKey,LPCTSTR lpszKey, LPCTSTR lpszValue)
{
	HKEY hSubKey = NULL;

	if (::RegCreateKey(hkey, lpszSubKey, &hSubKey) == ERROR_SUCCESS) {

		if (::RegSetValueEx(hSubKey, lpszKey, 0, REG_SZ,
		  (const BYTE*) lpszValue, lstrlen(lpszValue)*sizeof(TCHAR)) != ERROR_SUCCESS)
		{
			TRACE1("Warning: registration database update failed for key '%s'.\n", 	lpszKey);
			if (hSubKey != NULL) ::RegCloseKey(hSubKey);
			return FALSE;
		}
		if (hSubKey != NULL) ::RegCloseKey(hSubKey);
		return TRUE;
    } 
	else return FALSE;
}


#ifdef _COM

// ATL Global Module only instance
CAtlGlobalModule _Module;

// ATL GLobal Object Map
BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()

//	OBJECT_ENTRY(CLSID_ObjectTwo, CObjectTwo)

#endif


/////////////////////////////////////////////////////////////////////////////
// CMyglApp initialization

static CString strFileTypeId;

static HRESULT m_coResult;


#ifdef _KATMAI

extern BOOL KatmaiInit();

#endif

BOOL CMyglApp::InitInstance()
{

#ifdef _KATMAI
	if (!KatmaiInit()) {
#if 0
		::MessageBox(NULL, "This version requires an Intel Pentium III processor \n Program will be terminated"  
			, _PROGRAM " System Message" , MB_OK | MB_ICONINFORMATION | MB_SYSTEMMODAL | MB_TOPMOST);

		//AfxMessageBox("This version requires an Intel Katmai processor \n Program will be terminated ");
		return FALSE;
#endif

	}
#endif

 	/*
	// Initialize COM Library (for RSX)
	*/
	m_coResult = CoInitialize(NULL);

#ifdef _COM
	// Initialize the ATL Module
	_Module.Init(ObjectMap,m_hInstance);

#endif


	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
	m_ChoosePixelFormat = 0;
	m_ActiveView = 0;


#ifndef _NSPLUGIN
	Enable3dControls();	// VC 2.0
#endif

	//SetDialogBkColor();	     // set dialog background color to gray
	LoadStdProfileSettings();  // Load standard INI file options (including MRU)


	GReporter::SetCurrent(new GReporter);

    // make sure Gv VRML  database is init !!!
    GvDB::init();

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.
   
   CDocTemplate *pTemplate;

   AddDocTemplate(pTemplate=new CSingleDocTemplate(IDR_MAINFRAME,
			RUNTIME_CLASS(CMyglDoc),
			RUNTIME_CLASS(CMainFrame),     // main SDI frame window
			RUNTIME_CLASS(CMyglView)));

	// command line processing here
	{
	CString fileArg;
	
	// there is a utility function 
	if (m_lpCmdLine[0] != '\0')
	{
		char *p,*p1;

		p=m_lpCmdLine;
		while (*p) {

		 while (isspace(*p)) p++;
		 
		 if ((*p == '-') || (*p == '/')) { // Option start
		 p++;
		 switch (*p++) {
		 case 'X' :
		 case 'x' :
				m_ChoosePixelFormat = 1;
				break;
		 default :
			Reporter.Error("Unknown commandline option '%c' in %s",*p,m_lpCmdLine);
			break;
		 }
		}
		else {	// file argument
#if 0
			if (fileArg.GetLength()>0) {
				Reporter.Error("too many commandline arguments in %s",m_lpCmdLine);
 				while (!isspace(*p)) { if (!*p) break;p++;}
			}		 
		 	else while (!isspace(*p)) {
		   		if (!*p) break;
		   		fileArg += *p;
		   		p++;
		 	}
#else
			if (*p == '"') {
			    p++;
				while (*p != '"') {
		   			if (!*p) break;
		   			fileArg += *p;
		   			p++;
		 		}
				if (*p == '"') p++;
			    while (isspace(*p)) p++;
				break;
				// to do: rest of commandline
			}
			else 
			   fileArg = p;
			break;
#endif

			}
		
		}
	}

	if (fileArg.GetLength() > 0)
	 	OpenDocumentFile(fileArg);	// directly load a file
	else 
	  // create a new (empty) document
	  OnFileNew();
	}


	RegisterShellFileTypes();

	// register all extensions
	{
	CString strFilterExt,strFileTypeName; //strFileTypeId
	if (pTemplate->GetDocString(strFileTypeId,
	   CDocTemplate::regFileTypeId) && !strFileTypeId.IsEmpty())
		{
			// enough info to register it
			if (!pTemplate->GetDocString(strFileTypeName,
			   CDocTemplate::regFileTypeName))
				strFileTypeName = strFileTypeId;    // use id name

	}

	} // for all extensions
	

	// enable file manager drag/drop and DDE Execute open
	m_pMainWnd->DragAcceptFiles();

	EnableShellOpen();
	// VC 2.0
	// The main window has been initialized, so show and update it.
	//pMainFrame->ShowWindow(m_nCmdShow);
	//pMainFrame->UpdateWindow();

#ifdef _EVALUATION
    // increment runcount 
    int runCount = GetProfileInt("GLView","RunCount",0);
    runCount++;
    WriteProfileInt("GLView","RunCount",runCount);
#endif

    int version = GetProfileInt("GLView","Version",0);
    if (version == 0 || version<_VERSION_INT) {
        version = _VERSION_INT;
        WriteProfileInt("GLView","Version",version);
// Date  WriteProfileInt("GLView","Version",version);
        // increment run-count
    }
	return TRUE;
}


// return app returncode 
int CMyglApp::ExitInstance()
{

#ifdef _COM
	_Module.RevokeClassObjects(); // Revoke class factories for ATL
	_Module.Term();				  // cleanup ATL GLobal Module
#endif

    if(m_coResult == S_OK){
    	/*
    	// Tell the COM Library we are all done
    	*/
	    CoUninitialize();
    } /* if */

#ifdef _DEBUG
    GvDB::term();  // need to free data only if memory debugging
#endif

    return(CWinApp::ExitInstance());

}

// 
BOOL CMyglApp::OnDDECommand( LPTSTR lpszCommand )
{
 	Reporter.Status("DDE command %s ",lpszCommand);
	return (CWinApp::OnDDECommand(lpszCommand));
}


// Idle Processing
BOOL CMyglApp::OnIdle(LONG lcount)
{
	CWnd* pMainWnd = m_pMainWnd;

	if (pMainWnd != NULL && pMainWnd->m_hWnd != NULL && pMainWnd->IsWindowVisible() && !pMainWnd->IsIconic())
	{
		if (lcount==0) {
		   CWinApp::OnIdle(lcount); // this does an WM_UPDATEIDLE ...
		}
		if (m_ActiveView) return(m_ActiveView->OnIdle(lcount));
	} 
	
	return FALSE;

		
#if 0
  if (!CWinApp::OnIdle(lcount)) {
		// if lcount == 0, there was a message before
		// pass up message to view
		if (m_ActiveView) return(m_ActiveView->OnIdle(lcount));

  }
  return TRUE; // send more messages
#endif

}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_key;
	CString	m_customer;
	CString	m_company;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//{{AFX_MSG(CAboutDlg)
	afx_msg void OnThanks();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_key = _T("");
	m_customer = _T("");
	m_company = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
#ifndef _EVALUATION
#ifndef _SNI
	if (!pDX->m_bSaveAndValidate) {
		m_key = AfxGetApp()-> GetProfileString("GLView","Key","");
		m_company = AfxGetApp()-> GetProfileString("GLView","Company","");
		m_customer = AfxGetApp()-> GetProfileString("GLView","Customer","");
	}

	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_KEY, m_key);
	DDX_Text(pDX, IDC_CUSTOMER, m_customer);
	DDX_Text(pDX, IDC_COMPANY, m_company);
	//}}AFX_DATA_MAP
#endif
#endif
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_BN_CLICKED(IDC_THANKS, OnThanks)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CMyglApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CMYglApp commands


void CMyglApp::OnAppExit() 
{
#ifdef _EVALUATION
	// AfxMessageBox("Thank you for trying GLView\nfor ordering information please see the file order.txt");
#endif
	CWinApp::OnAppExit();
}

static void  MyAfxGetModuleShortFileName(HINSTANCE hInst, CString& strShortName)
{
#ifdef _MAC
	::GetModuleFileName(hInst, strShortName.GetBuffer(_MAX_PATH), _MAX_PATH);
	strShortName.ReleaseBuffer();
#else
	TCHAR szLongPathName[_MAX_PATH];
	::GetModuleFileName(hInst, szLongPathName, _MAX_PATH);
	if (::GetShortPathName(szLongPathName,
		strShortName.GetBuffer(_MAX_PATH), _MAX_PATH) == 0)
	{
		// rare failure case (especially on not-so-modern file systems)
		strShortName = szLongPathName;
	}
	strShortName.ReleaseBuffer();
#endif
}


void CMyglApp::OnRegisterFiletypes() 
{

  DRegisterFiletypes d;
  TCHAR szKey[_MAX_PATH*2];
  
  if (d.DoModal() == IDOK) {
     const char *p= d.m_types;
	 const char *p1;

	 // for each extension
	 while(*p != 0) {
	 	p1=p;
	 	while( (*p1!=0) && (*p1 != ',')) p1 ++;

		CString ext(p,p1-p);
		ext.TrimLeft(); ext.TrimRight();
		if (ext.GetLength()>0) {
		  ext = "."+ext;
		  (void)SetRegKey(ext, strFileTypeId);

		}
		if (*p1 == 0) break;
		p=p1+1;

	 }
	// define MIME Types 
	{
	HKEY hkey = NULL;
	wsprintf(szKey, _T("MIME\\Database\\Content Type\\x-world/x-vrml"));
	if (::RegCreateKey(HKEY_CLASSES_ROOT, szKey, &hkey) == ERROR_SUCCESS) {
		//SetRegKey(hkey, _T("CLSID"),		lpszClassID ); 
		SetRegKey(hkey, _T("Extension"),	_T(".wrl")); 
		if (hkey != NULL) ::RegCloseKey(hkey);
	}
	wsprintf(szKey, _T("MIME\\Database\\Content Type\\model/vrml"));
	if (::RegCreateKey(HKEY_CLASSES_ROOT, szKey, &hkey) == ERROR_SUCCESS) {
		//SetRegKey(hkey, _T("CLSID"),		lpszClassID); 
		SetRegKey(hkey, _T("Extension"),	_T(".wrl")); 
		if (hkey != NULL) ::RegCloseKey(hkey);
	}
	}

	// associate extension with control and set mime - type 
	SetRegKey(HKEY_CLASSES_ROOT, _T(".wrl"),_T("Content Type"),	_T("model/vrml")); 
	SetRegKey(HKEY_CLASSES_ROOT, _T(".wrl.gz"), _T("Content Type"),	_T("model/vrml")); 
	SetRegKey(HKEY_CLASSES_ROOT, _T(".wrz"), _T("Content Type"),	_T("model/vrml")); 
	SetRegKey(HKEY_CLASSES_ROOT, _T(".vrml"),_T("Content Type"),	_T("model/vrml")); 

	// stupid, but we need to register for gz to get wrl.gz 
	SetRegKey(_T(".gz"),strFileTypeId);
	SetRegKey(HKEY_CLASSES_ROOT, _T(".gz"),_T("Content Type"),	_T("application/x-gzip")); 
	



     if (d.m_register_netscape) {   // register GLView with Netscape
        const char * key = "Software\\Netscape\\Netscape Navigator\\Viewers";
        const char * key1 = "Software\\Netscape\\Netscape Navigator\\Suffixes";
        CString val("GLView.exe");
    
	    MyAfxGetModuleShortFileName(AfxGetInstanceHandle(), val);

        HKEY hKey;
//        long ret= ::RegOpenKey(HKEY_CURRENT_USER, key,&hKey);
        long ret= ::RegOpenKeyEx(HKEY_CURRENT_USER, key,0,KEY_ALL_ACCESS,&hKey);
        if (ret == ERROR_SUCCESS) {
            ret =::RegSetValueEx(hKey, "x-world/x-vrml",NULL,REG_SZ,(unsigned char*) (const char *)val,val.GetLength()+1);
            ret =::RegSetValueEx(hKey, "x-world/x-glview",NULL,REG_SZ,(unsigned char*) (const char *)val,val.GetLength()+1);
            ret =::RegSetValueEx(hKey, "model/vrml",NULL,REG_SZ,(unsigned char*) (const char *)val,val.GetLength()+1);

            // need to create a type
            int i=0;
            CString typeKey;
            int vrmlOk=0;
            int glviewOk=0;

            while (i<64) {
                
                DWORD lSize = 255;
                DWORD type = REG_SZ;
                typeKey.Format("TYPE%d",i);
                ret =::RegQueryValueEx(hKey, typeKey,NULL,&type,(unsigned char*) val.GetBuffer(255),&lSize);
                val.ReleaseBuffer();
                if (ret == ERROR_SUCCESS) {
                    // type already there ???
                    if (val == "x-world/x-vrml") vrmlOk=1;
                    if (val == "x-world/x-glview") glviewOk=1;
                } else {
                   // at end of types, add types if needed  
                   if (!vrmlOk) {
                        typeKey.Format("TYPE%d",i);
                        val = "x-world/x-vrml";
                        ret =::RegSetValueEx(hKey, typeKey,NULL,REG_SZ,(unsigned char*) (const char *)val,val.GetLength()+1);
                        i++;
                        typeKey.Format("TYPE%d",i);
                        val = "model/vrml";
                        ret =::RegSetValueEx(hKey, typeKey,NULL,REG_SZ,(unsigned char*) (const char *)val,val.GetLength()+1);
                        i++;
                   }
                   if (!glviewOk) {
                        typeKey.Format("TYPE%d",i);
                        val = "x-world/x-glview";
                        ret =::RegSetValueEx(hKey, typeKey,NULL,REG_SZ,(unsigned char*) (const char *)val,val.GetLength()+1);
                        i++;
                   }
                   break;

                }
                i ++;

            }
           ::RegCloseKey(hKey);
                       
        }

        ret= ::RegOpenKey(HKEY_CURRENT_USER, key1,&hKey);
        if (ret == ERROR_SUCCESS) {
            val = "wrl,vrml,wrl.gz,wrz";
            
            ret =::RegSetValueEx(hKey, "x-world/x-vrml",NULL,REG_SZ,(unsigned char*) (const char *)val,val.GetLength()+1);
            val = "wrl,vrml,wrl.gz,wrz,3dv";
            ret =::RegSetValueEx(hKey, "x-world/x-glview",NULL,REG_SZ,(unsigned char*) (const char *)val,val.GetLength()+1);
           ::RegCloseKey(hKey);
                       
        }

     }

	 

  }
}
/////////////////////////////////////////////////////////////////////////////
// DRegisterFiletypes dialog


DRegisterFiletypes::DRegisterFiletypes(CWnd* pParent /*=NULL*/)
	: CDialog(DRegisterFiletypes::IDD, pParent)
{
	//{{AFX_DATA_INIT(DRegisterFiletypes)
	m_types = _T("");
	m_register_netscape = TRUE;
	//}}AFX_DATA_INIT

	m_types = "3DV,GEO,RAW" 
#ifdef _DXF
			",DXF" 
#endif
			",RGB" // DIB,BMP,JPEG,GIF,TGA"

			",WRL,WRZ,WRL.GZ,VRML,URL,VRL";
}


void DRegisterFiletypes::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DRegisterFiletypes)
	DDX_Text(pDX, IDC_FILETYPES, m_types);
	DDX_Check(pDX, IDC_REGISTER_NETSCAPE, m_register_netscape);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DRegisterFiletypes, CDialog)
	//{{AFX_MSG_MAP(DRegisterFiletypes)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// DRegisterFiletypes message handlers

void DRegisterFiletypes::OnOK() 
{
	CDialog::OnOK();
}

void CAboutDlg::OnThanks() 
{
 CThanksTo d;
 
 d.DoModal();
	
}


// declaration of global UID´s from blaxxunVRML.idl
#include <initguid.h>

#include "gsound.h"

#ifdef G_RSX
#include "rsx.h"
#endif


#ifdef _COM
#include "..\blaxxuncc3d\blaxxunVRML.h"
#define IID_DEFINED
#include "..\blaxxuncc3d\blaxxunVRML_i.c"
#endif 





