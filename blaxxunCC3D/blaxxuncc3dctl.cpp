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

@module blaxxunCC3DCtl.cpp - Implementation of the CGLViewCtrlCtrl OLE control class.|

Copyright (c) 1997 - 1999 blaxxun interactive 	
All rights reserved

Purpose:
	blaxxunCC3D core OCX control class
	mime type registration
	user interface + navigation

Classes:
<c CGLViewCtrlCtrl>

Notes:
	update version string in GVersion.h on major changes / IDL changes

Changes:
	22.10.97 hg InitializeUrlmonLib - dynamic loading of IE urlmon.dll
	23.10.97    Rainers changes
	27.10.97 HG timer back to 25, misc
	??
	03.11.97 HG setObserver loadUrlFromFile
	10.01.98 HG new sound stuff 
	1999/07/07 thilo thread model for NS dialogs

   $Revision: 1.117 $
   $Log: blaxxuncc3dctl.cpp,v $
   Revision 1.117  1999/07/07 15:20:51  thilo
   *** empty log message ***

   Revision 1.116  1999/07/06 17:02:19  tom
   *** empty log message ***

   Revision 1.88  1998/10/30 13:40:44  tom
   shift-taste

   Revision 1.87  1998/10/29 18:07:42  holger
   empty message

   Revision 1.83  1998/10/07 13:38:49  tom
   *** empty log message ***

   Revision 1.82  1998/09/29 11:55:02  holger
   empty message

   Revision 1.81  1998/09/28 15:21:18  tom
   support of embed and param tags for navpanel

   Revision 1.80  1998/09/22 16:45:49  holger
   empty message

   Revision 1.79  1998/09/21 16:44:59  tom
   *** empty log message ***

   Revision 1.77  1998/08/14 14:18:57  tom
   onsize fuer navpanel geaendert

   Revision 1.76  1998/08/13 14:45:16  tom
   *** empty log message ***

   Revision 1.75  1998/08/12 14:33:57  kristof
   use sendmessage instead of post for netscape load requests

   Revision 1.74  1998/08/12 13:17:13  tom
   slide-buttons im navpanel eingefuegt

   Revision 1.72  1998/08/11 09:43:15  tom
   Erweiterung des Navigationspanels.

   Revision 1.71  1998/07/30 16:32:53  thilo
   *** empty log message ***

   Revision 1.70  1998/07/30 12:37:01  tom
   erste versuche mit navpanel

   Revision 1.69  1998/07/17 14:43:36  kristof
   *** empty log message ***

   Revision 1.68  1998/07/14 17:25:02  kristof
   wiatcursor removed
   return parameter for getobject rayhit changed

   Revision 1.67  1998/07/13 16:48:48  kristof
   lower performance if not foreground
   OnEvent added

   Revision 1.66  1998/07/09 15:45:21  kristof
   *** empty log message ***

   Revision 1.65  1998/07/06 17:42:14  holger
   empty message

   Revision 1.49  1998/04/20 11:41:37  bernd
   kick out fast timer ticks

   Revision 1.47  1998/04/15 15:53:57  holger
   *** empty log message ***

   Revision 1.22  1997/11/24 13:59:43  rainer
   navigation corrections, merge with Holger.


  Todo :

******************************************************************************/



#include "stdafx.h"
#include "blaxxunCC3D.h"
#include "blaxxunCC3dCtl.h"
#include "blaxxunCC3dPpg.h"

#include "RenderThread.h"

#include "blaxxunCC3DInterface.h"

// register mime types 
#define REG_MIME

// #include "Urlmon.h"
#include "hlink.h"

#include "mmsystem.h"

// Dialogs
#include "DlgMenuThread.h"

#include "DAbout.h"
#include "DWorldInfo.h"
#include "DProp.h"
#include "DPad.h"


#include "DConsole.h"

#include "GVersion.h"

#include <malloc.h>

#include <objsafe.h>
#include "helpers.h"

#ifdef _OGL
#include "COpenGL.h"
#endif

#include "GReporter.h"

#include "GTrace.h"

#include "GUrlCache.h"

#include "GRender.h"

#include "RenderThread.h"


#include <GvScene.h>
#include <GvDB.h>
#include <GvConnection.h>
#include <GvTraverse.h>
#include <GvWWWAnchor.h>
#include <GvWWWInline.h>
#include <GvPerspectiveCamera.h>
#include <GvInfo.h>
#include <GvProto.h>
#include <GvSensors.h>

#include "GPanel.h"
#include "GModel.h"
#include "GRaySelect.h"

#include "GSound.h"
#include "GRSXSound.h"
#include "GDSSound.h"



// if io wished 
#include "GIoTraversal.h"

// EventHandlers
#include "GSound.h"

// EventHandlers
#include "GEvent.h"
#include "GCameraUtil.h"

#include "GKatmai.h"

#include <time.h>

#include "dispids.h"
#include <idispids.h> // READYSTATE 


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Helpers for netscape 


class CModalThread : public CWinThread
{
	DECLARE_DYNCREATE(CModalThread)
	CModalThread() { m_exit = FALSE; m_dialog = NULL; }
	virtual BOOL InitInstance();
	void Exit() { m_exit = TRUE; }
	void SetDialog(CDialog* dialog) { m_dialog = dialog; }
	void SetFinFlag(BOOL* ff) { m_ff = ff; }
	UINT GetDlgRet() { return m_rc; }
	BOOL		m_exit;
	CDialog*	m_dialog;
	UINT		m_rc;
	BOOL*		m_ff;
};

IMPLEMENT_DYNCREATE(CModalThread, CWinThread)
BOOL CModalThread::InitInstance()
{
	m_rc = m_dialog->DoModal();
	*m_ff = TRUE;
	while ( !m_exit ) { Sleep(200); }
	return FALSE;
}

void CGLViewCtrlCtrl::DoThreadModalUpdateMenu(CMenu* menu) 
{
	// thilo 15.07.99: zusätzlicher code damit die checkmarks
	// im popup-menü funktionieren. vor anzeige des menüs wird
	// die menüstruktur durchlaufen und für jedes item der ent-
	// sprechende command-update handler aufgeufen.

	CCmdUI state;
	state.m_pMenu = menu;
	state.m_nIndexMax	= menu->GetMenuItemCount();

	for ( UINT j=0; j< state.m_nIndexMax; j++ ) 
	{
		CMenu* sub = menu->GetSubMenu(j);
		if ( sub != NULL )	{ DoThreadModalUpdateMenu(sub);	}

		state.m_nIndex	= j;
		state.m_nID		= menu->GetMenuItemID(j);
		state.DoUpdate(this,true);
	}

}

UINT CGLViewCtrlCtrl::DoThreadModal(CMenu* menu, POINT* menuPos, CWnd* parent)
{
	#ifndef _NS_MODAL_THREAD
		menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON, menuPos->x, menuPos->y, parent);
		return 0;
	#endif

	if ( !(view->observerFlags & GOBSERVE_WM_ANCHOR) ) // netscape
	{
		menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON, menuPos->x, menuPos->y, parent);
		return 0;
	}

	DoThreadModalUpdateMenu(menu);
	CDlgMenuThread dlg(NULL, menu, menuPos, parent);
	DoThreadModal( (CDialog*)&dlg, 30000, TRUE );

	return 0;
}

UINT CGLViewCtrlCtrl::DoThreadModal(CDialog* dialog, UINT timeout, BOOL menu)
{
	#ifndef _NS_MODAL_THREAD
		return dialog->DoModal();
	#endif

    if ( !(view->observerFlags & GOBSERVE_WM_ANCHOR) ) // netscape
	{
		return dialog->DoModal();
	}

	if ( m_pt != NULL ) 
	{
		if ( m_pd != NULL )
			 m_pd->EndDialog(IDCANCEL);
		else
  			 m_pt->PostThreadMessage(WM_QUIT, NULL, 0);

		while ( !m_threadFinished ) Sleep(200);
		m_pt = NULL;
		m_pd = NULL;
	}

	m_pd = dialog;
	m_threadFinished = FALSE;
	
	m_pt = AfxBeginThread ( RUNTIME_CLASS(CModalThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED );
	((CModalThread*)m_pt)->SetFinFlag(&m_threadFinished);
	((CModalThread*)m_pt)->SetDialog(dialog);
	((CModalThread*)m_pt)->ResumeThread();
	
	UINT count = 0;
	CString msg;
	BOOL prg = ( view && !view->GetFullScreen() )? TRUE : FALSE;
   	
	while ( !m_threadFinished  )
	{ 
		Sleep(300);
		count += 300;

		//AfxGetMainWnd()->UpdateWindow();

		if ( count == timeout )
		{
			if ( menu )
				::PostMessage(HWND_BROADCAST, WM_CANCELMODE, 0, 0);
			else
				m_pd->EndDialog(IDCANCEL);
		}

        wsprintf( msg.GetBuffer(_MAX_PATH), _T("%lu"), (timeout-count)/1000 );
		if ( prg ) ::SendMessage(view->observerWnd, WM_USER + 500, (WPARAM) (const char*)msg , 0);
		msg.ReleaseBuffer();
	}

	msg = "";
	if ( prg ) ::SendMessage(view->observerWnd, WM_USER + 500, (WPARAM) (const char*)msg , 0);
	
	if ( m_pt != NULL )
	{
		UINT rc = ((CModalThread*)m_pt)->GetDlgRet();
		((CModalThread*)m_pt)->Exit();
		m_pt = NULL;
		return rc;
	}
	
	return IDCANCEL;
}


//
// class ViewLock
// helper to automatically lock view in function block 
//
//
class ViewLock {
public :
	GView *m_view;

	ViewLock(GView *view) : m_view(view)
	{
		view->Lock(); 
	}
	~ViewLock() 
	{
		m_view->Unlock(); 
	}

};

//
// CCtrlReporter
// Report status messages to statusbar
// errors / warnings / traces to conole

class CCtrlReporter : public GReporter
{
public : 

	CGLViewCtrlCtrl *m_ctrl;

	CCtrlReporter(CGLViewCtrlCtrl *ctrl) : m_ctrl(ctrl) {}
	CString m_msg; // message buffer
	CString m_print;

	virtual int Status1(const char * message);
	virtual int Warning1(const char * message);
	virtual int Error1(const char * message);
	virtual int Trace1(const char * message);

};


int 
CCtrlReporter::Status1(const char * message)
{
	m_msg = message;
    ::PostMessage(m_ctrl->GetSafeHwnd(),WM_STATUSMESSAGE, (WPARAM) (const char *) m_msg, (LPARAM) PROGRESS_MESSAGE) ;
	return 0;


	if (!TryLock()) return -1;
		m_ctrl->Message(message,PROGRESS_MESSAGE);
	UnLock();
	return 0;
}

int 
CCtrlReporter::Warning1(const char * message)
{	
	if (ignoreWarnings) return -1;

	if (verbose) Status1(message);

	if (m_print.GetLength()>0) m_print += "\n";
	m_print += message;
    ::PostMessage(m_ctrl->GetSafeHwnd(),WM_CONSOLE_MESSAGE, (WPARAM) (!verbose), (LPARAM) PROGRESS_MESSAGE) ;
	return 0;

	if (!TryLock()) return -1;

	if (m_ctrl) {
		if (verbose) m_ctrl->Message(message,PROGRESS_MESSAGE);

		// protected !!!!!!! AFX_MANAGE_STATE(m_ctrl->m_pModuleState); // XX
		if (m_ctrl->m_dConsole) {
			m_ctrl->m_dConsole->AddText(message); 
			if (verbose) if (!m_ctrl->m_dConsole->m_hide) m_ctrl->m_dConsole->ShowWindow(SW_SHOWNOACTIVATE); //SW_SHOWNA);
		}
		else {


		CString caption;
		caption = _PROGRAM;
		//caption += " ";
		//if (view) capation += view->GetUrl();


		//m_ctrl->MessageBox(message, caption,MB_OK);

		if (::MessageBox(m_ctrl->GetSafeHwnd(),message,caption,MB_OKCANCEL | MB_ICONEXCLAMATION) != IDOK) {
		   ignoreWarnings= TRUE;
		}
		}
	}

	UnLock();

	return 0;
}

int 
CCtrlReporter::Trace1(const char * message)
{
	// if (ignoreWarnings) return -1;
	if (m_print.GetLength()>0) m_print += "\n";
	m_print += message;
    ::PostMessage(m_ctrl->GetSafeHwnd(),WM_CONSOLE_MESSAGE, (WPARAM) (const char *) NULL, (LPARAM) PROGRESS_MESSAGE) ;
	return 0;

	if (!TryLock()) return -1;

	//Lock();

	if (m_ctrl) {
		// protected !!!!!!! AFX_MANAGE_STATE(m_ctrl->m_pModuleState); // XX
		if (m_ctrl->m_dConsole) {
			m_ctrl->m_dConsole->AddText(message); 
			if (!m_ctrl->m_dConsole->m_hide) m_ctrl->m_dConsole->ShowWindow(SW_SHOWNA);
		}
	}

	UnLock();
	return 0;
}


int 
CCtrlReporter::Error1(const char * message)
{
	if (ignoreErrors) return -1;
	// Status1(message); already called from GReporter.error
	if (m_print.GetLength()>0) m_print += "\n";
	m_print += message;
    ::PostMessage(m_ctrl->GetSafeHwnd(),WM_CONSOLE_MESSAGE, (WPARAM) (const char *) NULL, (LPARAM) PROGRESS_MESSAGE) ;
	return 0;

	if (!TryLock()) return -1;

	// Lock();

	if (m_ctrl) {
		m_ctrl->Message(message,PROGRESS_MESSAGE);
		//GReporter::Error1(message);
		// changed to MessageBox on Wnd

		// protected !!!!!!! AFX_MANAGE_STATE(m_ctrl->m_pModuleState); // XX
		if (m_ctrl->m_dConsole) {
			m_ctrl->m_dConsole->AddText(message); 
			if (!m_ctrl->m_dConsole->m_hide) m_ctrl->m_dConsole->ShowWindow(SW_SHOWNA);
		}
		else {
			CString caption;
			caption = _PROGRAM;

			_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();

			//m_ctrl->MessageBox(message, caption,MB_OK);
			if (::MessageBox(m_ctrl->GetSafeHwnd(),message,caption,MB_OKCANCEL | MB_ICONEXCLAMATION) != IDOK) {
				ignoreErrors= TRUE;
			}
		}

	}

	UnLock();

	return 0;
}




// Notes
// Keyboard Interface 
// Error Reporting :  ThrowError
//TN038: MFC/OLE IUnknown Implementation
//TN039: MFC/OLE Automation Implementation

//
// CTranslator
// translates messages 
//


CTranslator::CTranslator() : m_section(_T("CC3D"))
{
	m_pApp = AfxGetApp();
#ifdef _DEBUG
	//out.open("c:/cc3d/translate.log");
#endif

}

CTranslator::~CTranslator()
{
	CString key;					
	CString entry;	
    POSITION runPos = m_hashTable.GetStartPosition ();	// get start position for iteration
    
	while ( runPos != NULL )
    {
		m_hashTable.GetNextAssoc ( runPos, key, entry );
		m_hashTable.SetAt ( key, NULL );
	}

	m_hashTable.RemoveAll();
}

BOOL Replace(const CString &val,LPCTSTR key,LPCTSTR replace,CString &ret) {
	int arg = val.Find(key);

	if (arg>=0) {	ret=val.Left(arg)+replace+val.Mid(arg+strlen(key));
		return TRUE; }
	else {
		ret = val;
		return FALSE;
	}
}			

// translate unique string into language denpendent string, entryDefault is the default english string
BOOL CTranslator::Translate(LPCTSTR key,LPCTSTR entryDefault, CString& value) 
{
	// lookup hashtable ?
	if (m_hashTable.Lookup( key, value )) {
		return TRUE;
	}

	// get from profile 
	value  = m_pApp->GetProfileString(m_section, key,"");
	if (value.GetLength() == 0) value = entryDefault; 
	else Replace(value,"\\t","\t",value);
	TRACE(" translate '%s' '%s' = '%s' \n",(const char *) key,(const char *) entryDefault,(const char *) value);
	// and store in hash table
	m_hashTable[key] = value;

#ifdef _DEBUG
	/*
	char debugBuf[256];
	sprintf(debugBuf, "'%s' '%s' = '%s' \n",(const char *) key,(const char *) entryDefault,(const char *) value);
	out << debugBuf;*/
	// write all existings keys to  file 
	CString tmp;
	Replace(value,"\t","\\t",tmp);
	m_pApp->WriteProfileString(m_section, key, tmp);
#endif

	return TRUE;
}



BOOL CTranslator::Lookup(LPCTSTR key,CString& value) 
{
	// lookup hashtable ?
	if (m_hashTable.Lookup( key, value )) {
		return TRUE;
	}

	// get from profile 
	value  = m_pApp->GetProfileString(m_section, key,"");
	if (value.GetLength() == 0) 
		return FALSE;
	Replace(value,"\\t","\t",value);
	TRACE(" Lookup '%s' = '%s' \n",(const char *) key,(const char *) value);
	// and store in hash table
	m_hashTable[key] = value;


	return TRUE;
}



// translate a message 
BOOL CGLViewCtrlCtrl::Translate(LPCTSTR key,LPCTSTR valueDefault, CString& value) {
	return m_translator.Translate(key,valueDefault,value);
}

BOOL CGLViewCtrlCtrl::Translate(LPCTSTR valueDefault, CString& value) 
{
	return m_translator.Translate(valueDefault,valueDefault,value);
}

BOOL CGLViewCtrlCtrl::TranslateMessageString(UINT nID, CString& value)
{
	char buf[60];
	buf[0]='s',
	_itoa(nID,&buf[1],10); CString valueDefault;
//	if (m_translator.Lookup(buf,value)) return TRUE;
	//verhindert print in translate! wird doch in translate sowieso gemacht

	COleControl::GetMessageString(nID, valueDefault);
	return m_translator.Translate(buf,valueDefault,value);
}
//#define TRANS(a,b) Translate(#a,b)


// Translate a CMenu
void CGLViewCtrlCtrl::Translate(CMenu &m) 
{

	UINT cnt = m.GetMenuItemCount();
	CString k,l;
	//MENUITEMINFO mi;

	for (UINT i=0; i<cnt;i++) {
		k = "";
		l= "";
		int id= m.GetMenuItemID(i);
		if (id == -1) { // sub menu;
			if (m.GetMenuString(i,k,MF_BYPOSITION)>0) { 
				if (Translate(k,l)) {
					m.ModifyMenu(i,MF_BYPOSITION | MF_STRING,id,l);
				}
			}
			CMenu *tmp = m.GetSubMenu(i);

			if (tmp != NULL)
				Translate(*tmp);
		}
		if (id >0) { // not a separator 
			char buf[60];
			buf[0]='m',
			_itoa(id,&buf[1],10);
			if (m.GetMenuString(i,k,MF_BYPOSITION)>0) { 
				if (Translate(buf, k, l)) {
					m.ModifyMenu(i,MF_BYPOSITION | MF_STRING,id,l);
				}
			}
		}

	}
}



// Utilities

// GetTime in seconds 
float GetTime() { return (float) clock()  / (float) CLOCKS_PER_SEC; }


// Trace refcounter for debugging 
#define TRACEREF() TRACE("Ref %d %s %d \n",m_dwRef,__FILE__,__LINE__);


IMPLEMENT_DYNCREATE(CGLViewCtrlCtrl, COleControl)

/////////////////////////////////////////////////////////////////////////////
// Special WM_PAINT message handler that includes the HDC
// MFC ctlcore.cpp
#define ON_WM_PAINT_SPECIAL() \
	{ WM_PAINT, 0, 0, 0, AfxSig_vD, \
		(AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(CDC*))&OnPaint },


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CGLViewCtrlCtrl, COleControl)
	//{{AFX_MSG_MAP(CGLViewCtrlCtrl)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_ACTIVATE()
	ON_COMMAND(ID_CAMERA_NONE, OnCameraNone)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_NONE, OnUpdateCameraNone)
	ON_COMMAND(ID_CAMERA_ORBIT, OnCameraOrbit)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_ORBIT, OnUpdateCameraOrbit)
	ON_COMMAND(ID_CAMERA_PAN, OnCameraPan)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_PAN, OnUpdateCameraPan)
	ON_COMMAND(ID_CAMERA_ROLL, OnCameraRoll)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_ROLL, OnUpdateCameraRoll)
	ON_COMMAND(ID_CAMERA_DOLLY_XY, OnCameraDollyXy)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_DOLLY_XY, OnUpdateCameraDollyXy)
	ON_COMMAND(ID_CAMERA_FLY, OnCameraFly)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_FLY, OnUpdateCameraFly)
	ON_COMMAND(ID_CAMERA_ANIMATE_VIEWPOINTS, OnCameraAnimateViewpoints)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_ANIMATE_VIEWPOINTS, OnUpdateCameraAnimateViewpoints)
	ON_COMMAND(ID_CAMERA_RESET, OnCameraReset)
	ON_COMMAND(ID_CAMERA_SEEKTO, OnCameraSeekto)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_SEEKTO, OnUpdateCameraSeekto)
	ON_COMMAND(ID_CAMERA_VIEWPOINT_NEXT, OnCameraViewpointNext)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_VIEWPOINT_NEXT, OnUpdateCameraViewpointNext)
	ON_COMMAND(ID_CAMERA_VIEWPOINT_PREV, OnCameraViewpointPrev)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_VIEWPOINT_PREV, OnUpdateCameraViewpointPrev)
	ON_COMMAND(ID_CAMERA_WALK, OnCameraWalk)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_WALK, OnUpdateCameraWalk)
	ON_COMMAND(ID_CAMERA_ZOOM, OnCameraZoom)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_ZOOM, OnUpdateCameraZoom)
	ON_COMMAND(ID_RENDER_FLAT, OnRenderFlat)
	ON_UPDATE_COMMAND_UI(ID_RENDER_FLAT, OnUpdateRenderFlat)
	ON_COMMAND(ID_RENDER_GOURAUD, OnRenderGouraud)
	ON_UPDATE_COMMAND_UI(ID_RENDER_GOURAUD, OnUpdateRenderGouraud)
	ON_COMMAND(ID_RENDER_TEXTURED, OnRenderTextured)
	ON_UPDATE_COMMAND_UI(ID_RENDER_TEXTURED, OnUpdateRenderTextured)
	ON_COMMAND(ID_RENDER_WIREFRAME, OnRenderWireframe)
	ON_UPDATE_COMMAND_UI(ID_RENDER_WIREFRAME, OnUpdateRenderWireframe)
	ON_COMMAND(ID_POPUP_HEADLIGHTON, OnCameraHeadlight)
	ON_UPDATE_COMMAND_UI(ID_POPUP_HEADLIGHTON, OnUpdateCameraHeadlight)
	ON_COMMAND(ID_RENDER_NONLIGHTED, OnRenderNonlighted)
	ON_UPDATE_COMMAND_UI(ID_RENDER_NONLIGHTED, OnUpdateRenderNonlighted)
	ON_COMMAND(ID_RENDER_VERTICES, OnRenderVertices)
	ON_UPDATE_COMMAND_UI(ID_RENDER_VERTICES, OnUpdateRenderVertices)
	ON_COMMAND(IDD_CAMERA_RELATIVE, OnCameraRelative)
	ON_UPDATE_COMMAND_UI(IDD_CAMERA_RELATIVE, OnUpdateCameraRelative)
	ON_COMMAND(ID_CAMERA_FOLLOW_OBJECT, OnCameraFollowObject)
	ON_COMMAND(ID_CAMERA_COLLISION, OnCameraCollision)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_COLLISION, OnUpdateCameraCollision)
	ON_WM_ACTIVATEAPP()
	ON_WM_DROPFILES()
	ON_WM_ENABLE()
	ON_WM_KILLFOCUS()
	ON_WM_PALETTECHANGED()
	ON_WM_PALETTEISCHANGING()
	ON_WM_QUERYNEWPALETTE()
	ON_WM_WINDOWPOSCHANGED()
	ON_COMMAND(ID_POPUP_HELP_MANUAL, OnPopupHelpManual)
	ON_COMMAND(ID_POPUP_HELP_VISIT, OnPopupHelpVisit)
	ON_COMMAND(ID_POPUP_HELP_ABOUT, OnPopupHelpAbout)
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_CAMERA_JUMP, OnCameraJump)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_JUMP, OnUpdateCameraJump)
	ON_COMMAND(ID_SETTINGS_STOP_LOADING, OnSettingsStopLoading)
	ON_UPDATE_COMMAND_UI(ID_SETTINGS_STOP_LOADING, OnUpdateSettingsStopLoading)
	ON_COMMAND(ID_SETTINGS_RELOAD, OnSettingsReload)
	ON_UPDATE_COMMAND_UI(ID_SETTINGS_RELOAD, OnUpdateSettingsReload)
	ON_WM_SHOWWINDOW()
	ON_COMMAND(ID_SETTINGS_SOUND, OnSettingsSound)
	ON_UPDATE_COMMAND_UI(ID_SETTINGS_SOUND, OnUpdateSettingsSound)
	ON_COMMAND(ID_POPUP_HELP_WORLDINFO, OnPopupHelpWorldinfo)
	ON_COMMAND(ID_POPUP_SETTINGS_STAYONGROUND, OnSettingsStayOnGround)
	ON_UPDATE_COMMAND_UI(ID_POPUP_SETTINGS_STAYONGROUND, OnUpdateSettingsStayOnGround)
	ON_COMMAND(ID_RENDER_DITHER, OnRenderDither)
	ON_UPDATE_COMMAND_UI(ID_RENDER_DITHER, OnUpdateRenderDither)
	ON_COMMAND(ID_RENDER_TEXTURE_SMOOTH, OnRenderTextureSmooth)
	ON_UPDATE_COMMAND_UI(ID_RENDER_TEXTURE_SMOOTH, OnUpdateRenderTextureSmooth)
	ON_COMMAND(ID_SETTINGS_PREFERENCES, OnSettingsPreferences)
	ON_WM_MOUSEACTIVATE()
	ON_COMMAND(ID_POPUP_HELP_CHECKVERSION, OnPopupHelpCheckversion)
	ON_COMMAND(ID_CAMERA_ROTATE, OnCameraRotate)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_ROTATE, OnUpdateCameraRotate)
	ON_COMMAND(ID_POPUP_SPEED_VERYSLOW, OnPopupSpeedVeryslow)
	ON_UPDATE_COMMAND_UI(ID_POPUP_SPEED_VERYSLOW, OnUpdatePopupSpeedVeryslow)
	ON_COMMAND(ID_POPUP_SPEED_SLOW, OnPopupSpeedSlow)
	ON_UPDATE_COMMAND_UI(ID_POPUP_SPEED_SLOW, OnUpdatePopupSpeedSlow)
	ON_COMMAND(ID_POPUP_SPEED_MEDIUM, OnPopupSpeedMedium)
	ON_UPDATE_COMMAND_UI(ID_POPUP_SPEED_MEDIUM, OnUpdatePopupSpeedMedium)
	ON_COMMAND(ID_POPUP_SPEED_FAST, OnPopupSpeedFast)
	ON_UPDATE_COMMAND_UI(ID_POPUP_SPEED_FAST, OnUpdatePopupSpeedFast)
	ON_COMMAND(ID_POPUP_SPEED_VERYFAST, OnPopupSpeedVeryfast)
	ON_UPDATE_COMMAND_UI(ID_POPUP_SPEED_VERYFAST, OnUpdatePopupSpeedVeryfast)
	ON_COMMAND(ID_CAMERA_ZOOMOUT, OnCameraZoomout)
	ON_COMMAND(ID_CAMERA_STRAIGHTEN_UP, OnCameraStraightenUp)
	ON_WM_KEYUP()
	ON_WM_DEVMODECHANGE()
	ON_WM_CHAR()
	ON_COMMAND(ID_GO_BACK, OnGoBack)
	ON_UPDATE_COMMAND_UI(ID_GO_BACK, OnUpdateGoBack)
	ON_COMMAND(ID_CAMERA_GRAVITY, OnCameraGravity)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_GRAVITY, OnUpdateCameraGravity)
	ON_COMMAND(ID_SETTINGS_CONSOLE, OnSettingsConsole)
	ON_UPDATE_COMMAND_UI(ID_SETTINGS_CONSOLE, OnUpdateSettingsConsole)
	ON_WM_ENTERIDLE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_POPUP_3RPERSON, OnView3rdPerson)
	ON_UPDATE_COMMAND_UI(ID_POPUP_3RPERSON, OnUpdateView3rdPerson)
	ON_COMMAND(ID_POPUP_PAD, OnViewPad)
	ON_UPDATE_COMMAND_UI(ID_POPUP_PAD, OnUpdateViewPad)
	ON_COMMAND(ID_POPUP_PANEL, OnPopupPanel)
	ON_UPDATE_COMMAND_UI(ID_POPUP_PANEL, OnUpdatePopupPanel)
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MBUTTONDBLCLK()
	ON_WM_INITMENU()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP

	ON_COMMAND_RANGE(ID_VIEWPOINT_0,ID_VIEWPOINT_19,OnCameraViewpoint)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEWPOINT_0,ID_VIEWPOINT_19,OnUpdateCameraViewpoint)

	ON_COMMAND_RANGE(ID_USER_MENU_START,ID_USER_MENU_END,OnUserMenu)
	
	ON_COMMAND_RANGE(ID_DRIVER_0,ID_DRIVER_9,OnOptionsDriver)
	ON_UPDATE_COMMAND_UI_RANGE(ID_DRIVER_0,ID_DRIVER_9,OnUpdateOptionsDriver)

	ON_MESSAGE(WM_READFILECOMPLETED, OnReadFileCompleted) 

	ON_MESSAGE(WM_HARDWARECHECK, OnHardwareCheck) 
	ON_MESSAGE(WM_STATUSMESSAGE, OnStatusMessage) 
	ON_MESSAGE(WM_CONSOLE_MESSAGE, OnConsoleMessage) 
	
	ON_MESSAGE(WM_SIZING, OnSizing) 
	ON_MESSAGE(WM_MOVING, OnMoving)
	ON_MESSAGE(WM_CAPTURECHANGED, OnCaptureChanged) 
	

	ON_WM_PAINT()
	ON_OLEVERB(AFX_IDS_VERB_EDIT, OnEdit)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)

END_MESSAGE_MAP()

/*
#if _MFC_VER >= 0x0421 
	ON_WM_SETTINGCHANGE()
#endif
*/


// HG 	ON_WM_CLOSE()

/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CGLViewCtrlCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CGLViewCtrlCtrl)
	DISP_PROPERTY_EX(CGLViewCtrlCtrl, "url", GetUrl, SetUrl, VT_BSTR)
	DISP_PROPERTY_EX(CGLViewCtrlCtrl, "renderMode", GetRenderMode, SetRenderMode, VT_BSTR)
	DISP_PROPERTY_EX(CGLViewCtrlCtrl, "navigationMode", GetNavigationMode, SetNavigationMode, VT_BSTR)
	DISP_PROPERTY_EX(CGLViewCtrlCtrl, "headlight", GetHeadlight, SetHeadlight, VT_I2)
	DISP_PROPERTY_EX(CGLViewCtrlCtrl, "walkSpeed", GetWalkSpeed, SetWalkSpeed, VT_R4)
	DISP_PROPERTY_EX(CGLViewCtrlCtrl, "animateAllViewpoints", GetAnimateAllViewpoints, SetAnimateAllViewpoints, VT_BOOL)
	DISP_PROPERTY_EX(CGLViewCtrlCtrl, "viewpoint", GetViewpoint, SetViewpoint, VT_BSTR)
	DISP_PROPERTY_EX(CGLViewCtrlCtrl, "description", GetDescription, SetDescription, VT_BSTR)
	DISP_PROPERTY_EX(CGLViewCtrlCtrl, "world", GetWorld, SetWorld, VT_BSTR)
	DISP_PROPERTY_EX(CGLViewCtrlCtrl, "collisionDetection", GetCollisionDetection, SetCollisionDetection, VT_BOOL)
	DISP_PROPERTY_EX(CGLViewCtrlCtrl, "avatarHeight", GetAvatarHeight, SetAvatarHeight, VT_R4)
	DISP_PROPERTY_EX(CGLViewCtrlCtrl, "collisionDistance", GetCollisionDistance, SetCollisionDistance, VT_R4)
	DISP_PROPERTY_EX(CGLViewCtrlCtrl, "visibilityLimit", GetVisibilityLimit, SetVisibilityLimit, VT_R4)
	DISP_PROPERTY_EX(CGLViewCtrlCtrl, "gravity", GetGravity, SetGravity, VT_BOOL)
	DISP_FUNCTION(CGLViewCtrlCtrl, "OnNextViewpoint", OnNextViewpoint, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CGLViewCtrlCtrl, "OnPrevViewpoint", OnPrevViewpoint, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CGLViewCtrlCtrl, "getName", getName, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CGLViewCtrlCtrl, "getVersion", getVersion, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CGLViewCtrlCtrl, "getCurrentSpeed", getCurrentSpeed, VT_R4, VTS_NONE)
	DISP_FUNCTION(CGLViewCtrlCtrl, "getCurrentFrameRate", getCurrentFrameRate, VT_R4, VTS_NONE)
	DISP_FUNCTION(CGLViewCtrlCtrl, "getWorldURL", getWorldURL, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CGLViewCtrlCtrl, "replaceWorld", replaceWorld, VT_EMPTY, VTS_UNKNOWN)
	DISP_FUNCTION(CGLViewCtrlCtrl, "loadURL", loadURL, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CGLViewCtrlCtrl, "setDescription", setDescription, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CGLViewCtrlCtrl, "createVrmlFromString", createVrmlFromString, VT_UNKNOWN, VTS_BSTR)
	DISP_FUNCTION(CGLViewCtrlCtrl, "createVrmlFromURL", createVrmlFromURL, VT_EMPTY, VTS_BSTR VTS_DISPATCH VTS_BSTR)
	DISP_FUNCTION(CGLViewCtrlCtrl, "getNode", getNode, VT_UNKNOWN, VTS_BSTR)
	DISP_FUNCTION(CGLViewCtrlCtrl, "addRoute", addRoute, VT_EMPTY, VTS_UNKNOWN VTS_BSTR VTS_UNKNOWN VTS_BSTR)
	DISP_FUNCTION(CGLViewCtrlCtrl, "deleteRoute", deleteRoute, VT_EMPTY, VTS_UNKNOWN VTS_BSTR VTS_UNKNOWN VTS_BSTR)
	DISP_FUNCTION(CGLViewCtrlCtrl, "createNode", createNode, VT_UNKNOWN, VTS_BSTR)
	DISP_FUNCTION(CGLViewCtrlCtrl, "addNode", addNode, VT_EMPTY, VTS_UNKNOWN)
	DISP_FUNCTION(CGLViewCtrlCtrl, "getWorld", getWorld, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION(CGLViewCtrlCtrl, "beginUpdate", beginUpdate, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CGLViewCtrlCtrl, "endUpdate", endUpdate, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CGLViewCtrlCtrl, "setTimerInterval", setTimerInterval, VT_I4, VTS_I4)
	DISP_FUNCTION(CGLViewCtrlCtrl, "isSceneLoaded", isSceneLoaded, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CGLViewCtrlCtrl, "setNodeName", setNodeName, VT_EMPTY, VTS_UNKNOWN VTS_BSTR)
	DISP_FUNCTION(CGLViewCtrlCtrl, "getNodeName", getNodeName, VT_BSTR, VTS_UNKNOWN)
	DISP_FUNCTION(CGLViewCtrlCtrl, "getEventOut", getEventOut, VT_UNKNOWN, VTS_BSTR)
	DISP_FUNCTION(CGLViewCtrlCtrl, "getEventIn", getEventIn, VT_UNKNOWN, VTS_BSTR)
	DISP_FUNCTION(CGLViewCtrlCtrl, "getControlPathname", getControlPathname, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CGLViewCtrlCtrl, "saveWorld", saveWorld, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CGLViewCtrlCtrl, "saveViewpoint", saveViewpoint, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CGLViewCtrlCtrl, "setFullscreen", setFullscreen, VT_BOOL, VTS_BOOL)
	DISP_FUNCTION(CGLViewCtrlCtrl, "removeNode", removeNode, VT_BOOL, VTS_UNKNOWN)
	DISP_FUNCTION(CGLViewCtrlCtrl, "setObserver", setObserver, VT_BOOL, VTS_UNKNOWN VTS_I4)
	DISP_FUNCTION(CGLViewCtrlCtrl, "loadURLfromFile", loadURLfromFile, VT_BOOL, VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CGLViewCtrlCtrl, "onUrlNotify", onUrlNotify, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4)
	DISP_FUNCTION(CGLViewCtrlCtrl, "setObserverWnd", setObserverWnd, VT_BOOL, VTS_I4)
	DISP_FUNCTION(CGLViewCtrlCtrl, "getUiMode", getUiMode, VT_I4, VTS_NONE)
	DISP_FUNCTION(CGLViewCtrlCtrl, "setUiMode", setUiMode, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CGLViewCtrlCtrl, "openPreferences", openPreferences, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CGLViewCtrlCtrl, "playSound", playSound, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CGLViewCtrlCtrl, "loadURLfromFile2", loadURLfromFile2, VT_BOOL, VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CGLViewCtrlCtrl, "onUrlNotify2", onUrlNotify2, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_BSTR VTS_I4)
	DISP_FUNCTION(CGLViewCtrlCtrl, "print", printMessage, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CGLViewCtrlCtrl, "setViewpointByValue", setViewpointByValue, VT_EMPTY, VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_I4)
	DISP_FUNCTION(CGLViewCtrlCtrl, "getViewpointByValue", getViewpointByValue, VT_EMPTY, VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_I4)
	DISP_FUNCTION(CGLViewCtrlCtrl, "getNodeBoundingBox", getNodeBoundingBox, VT_BOOL, VTS_UNKNOWN VTS_I4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4)
	DISP_FUNCTION(CGLViewCtrlCtrl, "setUrl2", setUrl2, VT_EMPTY, VTS_BSTR VTS_I4 VTS_I4)
	DISP_FUNCTION(CGLViewCtrlCtrl, "getInterfaceVersion", getInterfaceVersion, VT_I4, VTS_NONE)
	DISP_FUNCTION(CGLViewCtrlCtrl, "setThirdPersonView", setThirdPersonView, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CGLViewCtrlCtrl, "getThirdPersonView", getThirdPersonView, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CGLViewCtrlCtrl, "computeRayHit", computeRayHit, VT_UNKNOWN, VTS_UNKNOWN VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_I4)
	DISP_FUNCTION(CGLViewCtrlCtrl, "setMyAvatarNode", setMyAvatarNode, VT_EMPTY, VTS_UNKNOWN)
	DISP_FUNCTION(CGLViewCtrlCtrl, "getMyAvatarNode", getMyAvatarNode, VT_UNKNOWN, VTS_NONE)
	DISP_FUNCTION(CGLViewCtrlCtrl, "setMyAvatarURL", setMyAvatarURL, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CGLViewCtrlCtrl, "getMyAvatarURL", getMyAvatarURL, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CGLViewCtrlCtrl, "setViewpointFollow", setViewpointFollow, VT_BOOL, VTS_UNKNOWN VTS_R4 VTS_R4 VTS_R4 VTS_I4)
	DISP_FUNCTION(CGLViewCtrlCtrl, "setNodeEventIn", setNodeEventIn, VT_BOOL, VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CGLViewCtrlCtrl, "getNodeEventOut", getNodeEventOut, VT_BSTR, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CGLViewCtrlCtrl, "getObject", getObject, VT_UNKNOWN, VTS_I4)
	DISP_FUNCTION(CGLViewCtrlCtrl, "setNavigationPanel", setNavigationPanel, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CGLViewCtrlCtrl, "getNavigationPanel", getNavigationPanel, VT_BOOL, VTS_NONE)
	DISP_STOCKPROP_READYSTATE()
	//}}AFX_DISPATCH_MAP
//	DISP_PROPERTY_EX_ID(CGLViewCtrlCtrl, "url", DISPID_URL,GetUrl, SetUrl, VT_BSTR)
//	DISP_PROPERTY_EX_ID(CGLViewCtrlCtrl, "Media",DISPID_MEDIA, GetMedia, SetMedia, VT_BSTR)
//	DISP_PROPERTY_EX_ID(CGLViewCtrlCtrl, "Cfformat",DISPID_CFFORMAT, GetCfformat, SetCfformat, VT_BSTR)
	DISP_FUNCTION_ID(CGLViewCtrlCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)

END_DISPATCH_MAP()

// aboutbox ?? 

/////////////////////////////////////////////////////////////////////////////
// CGLViewCtrlCtrl interface map

BEGIN_INTERFACE_MAP(CGLViewCtrlCtrl,COleControl)
//    INTERFACE_PART(CGLViewCtrlCtrl, IID_IPersistMoniker, PersistMoniker)
//    INTERFACE_PART(CGLViewCtrlCtrl, IID_IOleCommandTarget, CmdTargetObj)
END_INTERFACE_MAP()
  


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CGLViewCtrlCtrl, COleControl)
	//{{AFX_EVENT_MAP(CGLViewCtrlCtrl)
	EVENT_CUSTOM("OnSceneChanged", FireOnSceneChanged, VTS_BSTR)
	EVENT_CUSTOM("OnEvent", FireOnEvent, VTS_BSTR  VTS_BSTR  VTS_R8)
	EVENT_STOCK_READYSTATECHANGE()
	EVENT_CUSTOM_ID("OnProgress",DISPID_PROGRESS, FireOnProgress, VTS_I4)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()

//	EVENT_CUSTOM("Progress", FireProgress, VTS_I4)


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CGLViewCtrlCtrl, 1)
	PROPPAGEID(CGLViewCtrlPropPage::guid)
END_PROPPAGEIDS(CGLViewCtrlCtrl)

//	PROPPAGEID(CGLViewCtrlPropPage::guid)
//	PROPPAGEID(CRenderingPropPage::guid)
//	PROPPAGEID(CNavigationPropPage::guid)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CGLViewCtrlCtrl, _PROGRAM "." _PROGRAM ".1",
	0x4b6e3013, 0x6e45, 0x11d0, 0x93, 0x9, 0, 0x20, 0xaf, 0xe0, 0x5c, 0xc8)

//IMPLEMENT_OLECREATE_EX(CGLViewCtrlCtrl, "GLVIEWCTRL.GLViewCtrlCtrl.1",
//	0x4b6e3013, 0x6e45, 0x11d0, 0x93, 0x9, 0, 0x20, 0xaf, 0xe0, 0x5c, 0xc8)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CGLViewCtrlCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DGLViewCtrl =
		{ 0x4b6e3011, 0x6e45, 0x11d0, { 0x93, 0x9, 0, 0x20, 0xaf, 0xe0, 0x5c, 0xc8 } };
const IID BASED_CODE IID_DGLViewCtrlEvents =
		{ 0x4b6e3012, 0x6e45, 0x11d0, { 0x93, 0x9, 0, 0x20, 0xaf, 0xe0, 0x5c, 0xc8 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information see help : OLEMISC

static const DWORD BASED_CODE _dwGLViewCtrlOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |	//SetClientSite called 
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE 	// SetExtent
	| OLEMISC_ACTSLIKEBUTTON  //  mk:@ivt:pdobj/native/activex/src/ctrlgde_15.htm
	| OLEMISC_ACTSLIKELABEL;
//	| OLEMISC_SIMPLEFRAME;



IMPLEMENT_OLECTLTYPE(CGLViewCtrlCtrl, IDS_GLVIEWCTRL, _dwGLViewCtrlOleMisc)


// Registry helpers
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

// HKEY_CURRENT_USER key to store settings 
#define USER_KEY _T("Software\\" _COMPANY "\\" _PROGRAM)

static BOOL SetRegKey(HKEY hkey,LPCTSTR lpszKey, int value)
{
	CString x;
	x.Format("%d",value);
	return SetRegKey(hkey,lpszKey,x);
}



// Registry helpers
static BOOL GetRegKey(HKEY hkey, LPCTSTR lpszKey, CString &value)
{
	DWORD lSize = _MAX_PATH;
	DWORD dType= REG_SZ;
	long ret = ::RegQueryValueEx(hkey, lpszKey,0, &dType, (BYTE*) (value.GetBuffer(_MAX_PATH)),&lSize);

    value.ReleaseBuffer();

    if (ret == ERROR_SUCCESS && value.GetLength()>0)
	{
		return TRUE;
	}
	else return FALSE;
}


//additionaly the reg is opened and closed
static BOOL GetRegKey(HKEY hkey, LPCTSTR lpSubKey, LPCTSTR lpszKey, CString &value)
{

	DWORD lSize = _MAX_PATH;
	DWORD dType= REG_SZ;
	HKEY hkResult; //the result of the opening

    if (RegOpenKeyEx(hkey, lpSubKey, 0, KEY_READ, &hkResult) != ERROR_SUCCESS)
	{
		return gfalse;
    }
	
	long ret = ::RegQueryValueEx(hkResult, lpszKey,0, &dType, (BYTE*) (value.GetBuffer(_MAX_PATH)),&lSize);

    value.ReleaseBuffer();

    RegCloseKey(hkResult);

    if (ret == ERROR_SUCCESS && value.GetLength()>0)
	{
		return TRUE;
	}
	else return FALSE;


/*
	
	const char* cpath;
	const char* cname;
	DWORD type;
	DWORD size;
	HKEY hkey;
	
	CString val;

    //cpath = "WEB3D\\PROTOCOLS\\urn\\web3d\\media";
    cpath = "SOFTWARE\\WEB3D\\PROTOCOLS\\urn\\web3d\\media";
    cname = "default";
   
	//HKEY_LOCAL_MACHINE\SOFTWARE\WEB3D\PROTOCOLS\urn\web3d\media


    if (RegQueryValueEx(hkey, cname, NULL, &type, NULL, &size) != ERROR_SUCCESS){
       RegCloseKey(hkey);
       return gfalse;
    }
    if (RegQueryValueEx(hkey, cname, NULL, &type,(unsigned char *)val.GetBuffer(size+1), &size) != ERROR_SUCCESS){
		val.ReleaseBuffer();
        RegCloseKey(hkey);
       return gfalse;
    }
    val.ReleaseBuffer();
    RegCloseKey(hkey);
	*/
}

static BOOL GetRegKey(HKEY hkey,LPCTSTR lpszKey, int &value)
{
	CString v;
	if (GetRegKey(hkey,lpszKey,v)) {

		int ret= sscanf((const char *)v,"%d",&value);
		return ret == 1;
	} else return FALSE;
}

static BOOL GetRegKey(HKEY hkey,LPCTSTR lpszKey, float &value)
{
	CString v;
	if (GetRegKey(hkey,lpszKey,v)) {

		int ret= sscanf((const char *)v,"%f",&value);
		return ret == 1;
	} else return FALSE;
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

static BOOL SetRegKeySimple(LPCTSTR lpszKey, LPCTSTR lpszValue, HKEY hKey = HKEY_CLASSES_ROOT)
{
	if (::RegSetValue(hKey, lpszKey, REG_SZ,
		  lpszValue, lstrlen(lpszValue)) != ERROR_SUCCESS)
	{
		TRACE1("Warning: registration database update failed for key '%s'.\n",
			lpszKey);
		return FALSE;
	}
	return TRUE;
}
static BOOL DelRegKeySimple(LPCTSTR lpszKey, LPCTSTR lpszValue, HKEY hKey = HKEY_CLASSES_ROOT)
{	CString value;
	LONG lSize = _MAX_PATH;
	DWORD dType= REG_SZ;
	long ret = ::RegQueryValue(hKey, lpszKey,(value.GetBuffer(_MAX_PATH)),&lSize);

    value.ReleaseBuffer();

    if (ret == ERROR_SUCCESS && value == lpszValue)
	{
		if (::RegOpenKey(hKey, lpszKey, &hKey) == ERROR_SUCCESS) {		
			::RegDeleteValue(hKey, NULL);
			::RegCloseKey(hKey);		
		} 
		return TRUE;
	}
	return FALSE;
}
static BOOL DelRegKey(HKEY hKey,LPCTSTR lpszKey, LPCTSTR lpszValue)
{	CString value;
	DWORD lSize = _MAX_PATH;
	DWORD dType= REG_SZ;
	long ret = ::RegQueryValueEx(hKey, lpszKey,0, &dType, (BYTE*) (value.GetBuffer(_MAX_PATH)),&lSize);

    value.ReleaseBuffer();

    if (ret == ERROR_SUCCESS && value == lpszValue)
	{
		::RegDeleteValue(hKey, lpszKey);		
		return TRUE;
	}
	return FALSE;
}
static BOOL DelRegKey(HKEY hKey,LPCTSTR lpszKey)
{	
	::RegDeleteValue(hKey, lpszKey);		
	return TRUE;
}

// Under Win32, a reg key may not be deleted unless it is empty.
// Thus, to delete a tree,  one must recursively enumerate and
// delete all of the sub-keys.

static LONG RecursiveRegDeleteKey(HKEY hParentKey, LPCTSTR szKeyName)
{
	DWORD   dwIndex = 0L;
	TCHAR   szSubKeyName[256];
	HKEY    hCurrentKey;
	DWORD   dwResult;

	if ((dwResult = ::RegOpenKey(hParentKey, szKeyName, &hCurrentKey)) ==
		ERROR_SUCCESS)
	{
		// Remove all subkeys of the key to delete
		while ((dwResult = RegEnumKey(hCurrentKey, 0, szSubKeyName, 255)) ==
			ERROR_SUCCESS)
		{
			if ((dwResult = RecursiveRegDeleteKey(hCurrentKey,
				szSubKeyName)) != ERROR_SUCCESS)
				break;
		}

		// If all went well, we should now be able to delete the requested key
		if ((dwResult == ERROR_NO_MORE_ITEMS) || (dwResult == ERROR_BADKEY))
		
		{
			dwResult = ::RegDeleteKey(hParentKey, szKeyName);
		}
	}

	::RegCloseKey(hCurrentKey);
	return dwResult;
}


#define GUID_CCH    39  // Characters in string form of guid, including '\0'
// see also MFC/src/CTLREG.cpp
// see indetsdk/specs/register.htm
// inetsdk\samples\basectrl\framewrk\util.cpp
// see KB mimetype.exe sample 

BOOL RegisterMimeTypes(CLSID &clsid,LPCTSTR lpszProgID,BOOL registerVrml,BOOL registerExt) 
{
	USES_CONVERSION;

	BOOL bSuccess = FALSE;

	TCHAR szKey[_MAX_PATH*2];


	// Format class ID as a string
	OLECHAR szClassID[GUID_CCH];
	int cchGuid = ::StringFromGUID2(clsid, szClassID, GUID_CCH);
	LPCTSTR lpszClassID = OLE2CT(szClassID);

	ASSERT(cchGuid == GUID_CCH);    // Did StringFromGUID2 work?
	if (cchGuid != GUID_CCH)
		return FALSE;


	{ // add additional Control and Enable FullPage keys 
	HKEY hkeyClassID = NULL;

	wsprintf(szKey, _T("CLSID\\%s"), lpszClassID);
	if (::RegCreateKey(HKEY_CLASSES_ROOT, szKey, &hkeyClassID) != ERROR_SUCCESS) goto Error;


	HKEY hkeyTmp = NULL;
	wsprintf(szKey, _T("Control")); 
	if (::RegCreateKey(hkeyClassID, szKey, &hkeyTmp) == ERROR_SUCCESS)  ::RegCloseKey(hkeyTmp);

	wsprintf(szKey, _T("EnableFullPage\\.wrl"));
	if (::RegCreateKey(hkeyClassID, szKey, &hkeyTmp) == ERROR_SUCCESS) 	::RegCloseKey(hkeyTmp);

	wsprintf(szKey, _T("EnableFullPage\\.vrml"));
	if (::RegCreateKey(hkeyClassID, szKey, &hkeyTmp) == ERROR_SUCCESS) 	::RegCloseKey(hkeyTmp);

	wsprintf(szKey, _T("EnableFullPage\\.gz"));
	if (::RegCreateKey(hkeyClassID, szKey, &hkeyTmp) == ERROR_SUCCESS) 	::RegCloseKey(hkeyTmp);

	wsprintf(szKey, _T("EnableFullPage\\.wrz"));
	if (::RegCreateKey(hkeyClassID, szKey, &hkeyTmp) == ERROR_SUCCESS) 	::RegCloseKey(hkeyTmp);

	wsprintf(szKey, _T("EnableFullPage\\.bxwrl"));
	if (::RegCreateKey(hkeyClassID, szKey, &hkeyTmp) == ERROR_SUCCESS) 	::RegCloseKey(hkeyTmp);

	wsprintf(szKey, _T("EnableFullPage\\.bx3"));
	if (::RegCreateKey(hkeyClassID, szKey, &hkeyTmp) == ERROR_SUCCESS) 	::RegCloseKey(hkeyTmp);

/*
	// found this for ActiveMovieControl, this enables the Netscape Plugin instead of control !!!!
	wsprintf(szKey, _T("EnablePlugin\\.gz"));
	if (::RegCreateKey(hkeyClassID, szKey, &hkeyTmp) == ERROR_SUCCESS) 	::RegCloseKey(hkeyTmp);
*/

    
	::RegCloseKey(hkeyClassID);
	
	
	}

	//SOFTWARE\\Microsoft\\Internet Explorer\\EmbedExtnToClsidMappings	
	if (registerVrml) {
	HKEY hkey = NULL;
	TCHAR szValue[_MAX_PATH];

	wsprintf(szKey, _T("SOFTWARE\\Microsoft\\Internet Explorer\\EmbedExtnToClsidMappings"));
	if (::RegCreateKey(HKEY_LOCAL_MACHINE, szKey, &hkey) == ERROR_SUCCESS) {
		wsprintf(szValue,_T("clsid:%s"),&lpszClassID[1]);
		szValue[lstrlen(szValue)-1]=0;

		SetRegKeySimple(_T(".wrl"),		szValue ,hkey); 
		SetRegKeySimple(_T(".vrml"),	szValue ,hkey); 
		SetRegKeySimple(_T(".wrz"),		szValue ,hkey); 
		SetRegKeySimple(_T(".bxwrl"),	szValue ,hkey); 
		SetRegKeySimple(_T(".bx3"),	szValue ,hkey); 
		// SetRegKeySimple(_T(".wrl.gz"),	szValue ,hkey ); // won't work 
		// SetRegKeySimple(_T(".wav.gz"),	szValue ,hkey ); // test

		if (hkey != NULL) ::RegCloseKey(hkey);
	}

#if 1
	 // remove cosmo plugin compatibility stuff !
	// IE will anyway rescan netscape plugin directory 

	 wsprintf(szKey, _T("SOFTWARE\\Microsoft\\Internet Explorer\\Plugins\\MIME\\x-world/x-vrml"));
	 RecursiveRegDeleteKey(HKEY_LOCAL_MACHINE,szKey);
	 wsprintf(szKey, _T("SOFTWARE\\Microsoft\\Internet Explorer\\Plugins\\MIME\\model/vrml"));
	 RecursiveRegDeleteKey(HKEY_LOCAL_MACHINE,szKey);
	 wsprintf(szKey, _T("SOFTWARE\\Microsoft\\Internet Explorer\\Plugins\\Extension\\.cgi"));
	 RecursiveRegDeleteKey(HKEY_LOCAL_MACHINE,szKey);
	 wsprintf(szKey, _T("SOFTWARE\\Microsoft\\Internet Explorer\\Plugins\\Extension\\.wrl"));
	 RecursiveRegDeleteKey(HKEY_LOCAL_MACHINE,szKey);
	 wsprintf(szKey, _T("SOFTWARE\\Microsoft\\Internet Explorer\\Plugins\\Extension\\.wrz"));
	 RecursiveRegDeleteKey(HKEY_LOCAL_MACHINE,szKey);
	 wsprintf(szKey, _T("SOFTWARE\\Microsoft\\Internet Explorer\\Plugins\\Extension\\.vrml"));
	 RecursiveRegDeleteKey(HKEY_LOCAL_MACHINE,szKey);
	 wsprintf(szKey, _T("SOFTWARE\\Microsoft\\Internet Explorer\\Plugins\\Extension\\.gz"));
	 RecursiveRegDeleteKey(HKEY_LOCAL_MACHINE,szKey);
#endif

	}

	if (!registerVrml) { // check if model/vrml exists, if not anyway register mime types
		CString val;
		if (GetRegKey(HKEY_CLASSES_ROOT,_T("MIME\\Database\\Content Type\\model/vrml"), _T("CLSID"),val)) {
			//if already VRML ?
		}
		else {
			registerVrml = TRUE;

		}
	}

	// define MIME Types 
	if (registerVrml) 
	{
	HKEY hkey = NULL;
	wsprintf(szKey, _T("MIME\\Database\\Content Type\\x-world/x-vrml"));
	if (::RegCreateKey(HKEY_CLASSES_ROOT, szKey, &hkey) == ERROR_SUCCESS) {
		SetRegKey(hkey, _T("CLSID"),		lpszClassID ); 
		SetRegKey(hkey, _T("Extension"),	_T(".wrl")); 
		if (hkey != NULL) ::RegCloseKey(hkey);
		hkey = NULL;
	}
	wsprintf(szKey, _T("MIME\\Database\\Content Type\\model/vrml"));
	if (::RegCreateKey(HKEY_CLASSES_ROOT, szKey, &hkey) == ERROR_SUCCESS) {
		SetRegKey(hkey, _T("CLSID"),		lpszClassID); 
		SetRegKey(hkey, _T("Extension"),	_T(".wrl")); 
		if (hkey != NULL) ::RegCloseKey(hkey);
		hkey = NULL;
	}
	// hg new 23.02.98
	wsprintf(szKey, _T("MIME\\Database\\Content Type\\application/x-gzip-compressed"));
	if (::RegCreateKey(HKEY_CLASSES_ROOT, szKey, &hkey) == ERROR_SUCCESS) {
		// SetRegKey(hkey, _T("CLSID"),		lpszClassID); 
		SetRegKey(hkey, _T("Extension"),	_T(".gz")); 
		if (hkey != NULL) ::RegCloseKey(hkey);
		hkey = NULL;
	}
/*
	// hg new 23.02.98
	wsprintf(szKey, _T("MIME\\Database\\Content Type\\application/x-gzip"));
	if (::RegCreateKey(HKEY_CLASSES_ROOT, szKey, &hkey) == ERROR_SUCCESS) {
		// SetRegKey(hkey, _T("CLSID"),		lpszClassID); 
		SetRegKey(hkey, _T("Extension"),	_T(".gz")); 
		if (hkey != NULL) ::RegCloseKey(hkey);
		hkey = NULL;
	}
*/
	}

	// mime: application/x-blaxxuncc3d
	{
	HKEY hkey = NULL;
	wsprintf(szKey, _T("MIME\\Database\\Content Type\\application/x-%s"),_PROGRAM);
	if (::RegCreateKey(HKEY_CLASSES_ROOT, szKey, &hkey) == ERROR_SUCCESS) {
		SetRegKey(hkey, _T("CLSID"),		lpszClassID); 
		SetRegKey(hkey, _T("Extension"),	_T(".wrl")); 
		if (hkey != NULL) ::RegCloseKey(hkey);
		hkey = NULL;
	}
	}

	if (registerVrml) { // associate extension with control and set mime - type 
		if (registerExt) SetRegKeySimple(_T(".wrl"),lpszProgID);
		SetRegKey(HKEY_CLASSES_ROOT, _T(".wrl"),_T("Content Type"),	_T("model/vrml")); 

		SetRegKeySimple(_T(".bxwrl"),lpszProgID);
		SetRegKey(HKEY_CLASSES_ROOT, _T(".bxwrl"),_T("Content Type"),	_T("model/vrml")); 

		//if (registerExt) SetRegKeySimple(_T(".wrl.gz"),lpszProgID);
		//SetRegKey(HKEY_CLASSES_ROOT, _T(".wrl.gz"), _T("Content Type"),	_T("model/vrml")); 

		// test 
		//if (registerExt) SetRegKeySimple(_T(".wav.gz"),lpszProgID);
		//SetRegKey(HKEY_CLASSES_ROOT, _T(".wav.gz"), _T("Content Type"),	_T("audio/wav")); 

		if (registerExt) SetRegKeySimple(_T(".wrz"),lpszProgID);
		SetRegKey(HKEY_CLASSES_ROOT, _T(".wrz"), _T("Content Type"),	_T("model/vrml")); 
	
		if (registerExt) SetRegKeySimple(_T(".vrml"),lpszProgID);
	
		SetRegKey(HKEY_CLASSES_ROOT, _T(".vrml"),_T("Content Type"),	_T("model/vrml")); 

		// stupid, but we need to register for gz to get wrl.gz 
		// TO DO : only for IE 3.01
		/* removed 20.10.98, IE 3.0 obsolete ?
		if (registerExt) 	SetRegKeySimple(_T(".gz"),lpszProgID);
		*/

		SetRegKey(HKEY_CLASSES_ROOT, _T(".gz"),_T("Content Type"),	_T("application/x-gzip")); 
		//SetRegKey(HKEY_CLASSES_ROOT, _T(".gz"),_T("Content Type"),	_T("application/x-gzip-compressed"));  // hg 23.02.99
		// IE 4.01 adds a PROTOCOLS/ entry

	}
	
	// GLView file format extensions
	// no SetRegKeySimple(_T(".vrl"),lpszProgID);


	// get path TO IE-Explorer 
	{
	CString val;
	CString ie;
	CString ns;
	CString html;

	{	// path to IE 
		long lSize;
		lSize = _MAX_PATH;
		long ret =::RegQueryValue(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\IEXPLORE.EXE"), val.GetBuffer(_MAX_PATH),&lSize); 
        val.ReleaseBuffer();
        if (ret == ERROR_SUCCESS && val.GetLength()>0)
		{
			int arg = val.Find(';');
			if (arg>0) val=val.Left(arg);
			ie = val;
		}
		
		// path to NS
		{
			lSize = _MAX_PATH;
			ret =::RegQueryValue(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Netscape.exe"), val.GetBuffer(_MAX_PATH),&lSize); 
			val.ReleaseBuffer();
			if (ret == ERROR_SUCCESS && val.GetLength()>0)
			{
				int arg = val.Find(';');
				if (arg>0) val=val.Left(arg);
				ns = val;
			}
		}

	}

	// get html browser 
	{
		CString val;
		long lSize;
		lSize = _MAX_PATH;
		long ret =::RegQueryValue(HKEY_CLASSES_ROOT,_T(".htm"), val.GetBuffer(_MAX_PATH),&lSize); 
        val.ReleaseBuffer();
        if (ret == ERROR_SUCCESS && val.GetLength()>0)
		{
			CString key =  val + _T("\\shell\\open\\command");
			lSize = _MAX_PATH;
			ret =::RegQueryValue(HKEY_CLASSES_ROOT,key, val.GetBuffer(_MAX_PATH),&lSize); 
            val.ReleaseBuffer();
			// "C:\PROGRA~1\INTERN~1\iexplore.exe" -nohome
			if (ret == ERROR_SUCCESS && val.GetLength()>0) {

				int arg = val.Find('\"');
		//		if (arg>0) 
		//			val=val.Left(arg);
				html = val;
			}
		}
	}
	
	val = ns;
	if (val.GetLength() == 0) val = ie;

    html.MakeUpper();

	if (ie.GetLength()>0 && html.Find("IEXPLORE")>=0) {
		val = ie;
	}

	// associate progId shell open with iexplore // compare also with CLASSES_ROOT htmlfile 
	// if registerExt 
    if (val.GetLength()>0) 
	{
		// for 
		CString path;
		if (val[0] != '\"') path  = "\"" + val + "\"";
		else path = val;

		path += " \"%1\"";	// append argument 
		wsprintf(szKey, _T("%s\\shell\\open\\command"),(const TCHAR *)lpszProgID);
		SetRegKeySimple(szKey,path);

		wsprintf(szKey, _T("%s\\DefaultIcon"),(const TCHAR *)lpszProgID);
		SetRegKeySimple(szKey,val+",1");
	}
	}

	// get IE-classpath  & remove  axWorldView && npcosmo 
	if (registerVrml) {
		CString val;
		CString newVal;
		CString oldVal;

		DWORD lSize = _MAX_PATH*10;
		DWORD dType= REG_SZ;

		HKEY key=NULL;

		if (::RegOpenKey(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\Microsoft\\Java VM"),&key) == ERROR_SUCCESS)
        {

		long ret =::RegQueryValueEx(key,_T("Classpath"), 0, &dType,  (BYTE*) ( val.GetBuffer(lSize)),&lSize); 

        val.ReleaseBuffer();

        if (ret == ERROR_SUCCESS && val.GetLength()>0)
		{   // axWorldView.zip  //npcosmop.zip // npcosmop21.zip
			CString entry;
			
			oldVal = val;
		
			BOOL done=FALSE;
			while (!done) { // for each ; separate classpath entry
				int arg = val.Find(';');
				if (arg>0) { entry=val.Left(arg); val=val.Mid(arg+1); }
				else { done = TRUE; entry = val; }
				
				CString entryLow(entry);
				entryLow.MakeLower();

				if ((entryLow.Find("axworldview") >=0)  //axWorldView
					|| (entryLow.Find("npcosmop") >=0)
					|| (entryLow.Find("msvrml") >=0)
					|| (entryLow.Find("corteai") >=0) //corteai.zip
				)
				{
					// remove from classpath
				}
				else {
					if (newVal.GetLength()>>0) newVal += ";";
					newVal += entry;
				}
			}

			if (oldVal != newVal) {
				SetRegKey(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\Microsoft\\Java VM"),_T("Classpath"),newVal); 
				SetRegKey(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\Microsoft\\Java VM"),_T("Classpath.CC3Dbackup"),oldVal); 
			}
		}
		::RegCloseKey(key);

		}
	}

	
	
	// create version independent entry 
	{
		CString pid;
		pid =  lpszProgID;

			int arg = pid.ReverseFind('.');
			if (arg>0) pid=pid.Left(arg);

			wsprintf(szKey, _T("%s"),(const TCHAR *) pid);
			CString desc;
			
			desc.LoadString(IDS_GLVIEWCTRL);

			SetRegKeySimple(szKey,desc);

			wsprintf(szKey, _T("%s\\CLSID"),(const TCHAR *) pid);
			SetRegKeySimple(szKey,lpszClassID);

			wsprintf(szKey, _T("%s\\CurVer"),(const TCHAR *)pid);
			SetRegKeySimple(szKey,lpszProgID);
	}


	// if wished :
//	SetRegKeySimple(_T(".3dv"),lpszProgID);
//	SetRegKeySimple(_T(".iv"),lpszProgID);

/*
	RegisterMediaType
	RegisterClipboardFormat
	RegisterMediaTypeClass
	Registers media types strings.
	HRESULT RegisterMediaTypes (
    UINT  ctypes,	//Number of media type strings in rgszTypes 
    LPTSTR *rgszTypes,	//Pointer to array of media types to be registered
    CLIPFORMAT *rgcfTypes	//Pointer to array of 32-bit values corresponding to rgszType array values
   );
*/
/*
	{ 
	LPCTSTR types[2] = { 	_T("model/vrml"), _T("x-world/x-vrml") };
	CLIPFORMAT cfTypes[2];

	RegisterMediaTypes(2,types,cfTypes);
	}

*/

Error:

	return bSuccess;

}

BOOL UnregisterMimeTypes(CLSID &clsid,LPCTSTR lpszProgID,BOOL registerVrml,BOOL registerExt) 
{
	USES_CONVERSION;

	BOOL bSuccess = FALSE;

	TCHAR szKey[_MAX_PATH*2];


	// Format class ID as a string
	OLECHAR szClassID[GUID_CCH];
	int cchGuid = ::StringFromGUID2(clsid, szClassID, GUID_CCH);
	LPCTSTR lpszClassID = OLE2CT(szClassID);

	ASSERT(cchGuid == GUID_CCH);    // Did StringFromGUID2 work?
	if (cchGuid != GUID_CCH)
		return FALSE;


	{ // add additional Control and Enable FullPage keys 
	HKEY hkeyClassID = NULL;

	wsprintf(szKey, _T("CLSID\\%s"), lpszClassID);
	RecursiveRegDeleteKey(HKEY_CLASSES_ROOT, szKey);
	}

	//SOFTWARE\\Microsoft\\Internet Explorer\\EmbedExtnToClsidMappings	
	if (registerVrml) {
	HKEY hkey = NULL;
	TCHAR szValue[_MAX_PATH];

	wsprintf(szKey, _T("SOFTWARE\\Microsoft\\Internet Explorer\\EmbedExtnToClsidMappings"));
	if (::RegOpenKey(HKEY_LOCAL_MACHINE, szKey, &hkey) == ERROR_SUCCESS) {
		wsprintf(szValue,_T("clsid:%s"),&lpszClassID[1]);
		szValue[lstrlen(szValue)-1]=0;

		DelRegKeySimple(_T(".wrl"),		szValue ,hkey); 
		DelRegKeySimple(_T(".vrml"),	szValue ,hkey); 
		DelRegKeySimple(_T(".wrz"),		szValue ,hkey); 
		DelRegKeySimple(_T(".bxwrl"),	szValue ,hkey); 
		DelRegKeySimple(_T(".wrl.gz"),	szValue ,hkey ); // won't work 
		DelRegKeySimple(_T(".wav.gz"),	szValue ,hkey ); // test

		if (hkey != NULL) ::RegCloseKey(hkey);
	}


	}

	if (!registerVrml) { // check if model/vrml exists, if not anyway register mime types
		CString val;
		if (GetRegKey(HKEY_CLASSES_ROOT,_T("MIME\\Database\\Content Type\\model/vrml"), _T("CLSID"),val)) {
			//if already VRML ?
		}
		else {
			registerVrml = TRUE;

		}
	}

	// define MIME Types 
	if (registerVrml) 
	{
	HKEY hkey = NULL;
	wsprintf(szKey, _T("MIME\\Database\\Content Type\\x-world/x-vrml"));
	if (::RegOpenKey(HKEY_CLASSES_ROOT, szKey, &hkey) == ERROR_SUCCESS) {
		DelRegKey(hkey, _T("CLSID"),		lpszClassID ); 
		//SetRegKey(hkey, _T("Extension"),	_T(".wrl")); 
		if (hkey != NULL) ::RegCloseKey(hkey);
		hkey = NULL;
	}
	wsprintf(szKey, _T("MIME\\Database\\Content Type\\model/vrml"));
	if (::RegOpenKey(HKEY_CLASSES_ROOT, szKey, &hkey) == ERROR_SUCCESS) {
		DelRegKey(hkey, _T("CLSID"),		lpszClassID); 
		//SetRegKey(hkey, _T("Extension"),	_T(".wrl")); 
		if (hkey != NULL) ::RegCloseKey(hkey);
		hkey = NULL;
	}
	}

	// mime: application/x-blaxxuncc3d
	{
	HKEY hkey = NULL;
	wsprintf(szKey, _T("MIME\\Database\\Content Type\\application/x-%s"),_PROGRAM);
	if (::RegOpenKey(HKEY_CLASSES_ROOT, szKey, &hkey) == ERROR_SUCCESS) {
		DelRegKey(hkey, _T("CLSID"),		lpszClassID); 
		DelRegKey(hkey, _T("Extension"),	_T(".wrl")); 
		if (hkey != NULL) ::RegCloseKey(hkey);
		hkey = NULL;
	}
	RecursiveRegDeleteKey(HKEY_CLASSES_ROOT, szKey); // anyway delete whole key, because non standard
	}


	if (registerVrml) { // associate extension with control and set mime - type 
		if (registerExt) DelRegKeySimple(_T(".wrl"),lpszProgID);
		//SetRegKey(HKEY_CLASSES_ROOT, _T(".wrl"),_T("Content Type"),	_T("model/vrml")); 

		DelRegKeySimple(_T(".bxwrl"),lpszProgID);
		//DelRegKey(HKEY_CLASSES_ROOT, _T(".bxwrl"),_T("Content Type"),	_T("model/vrml")); 

		if (registerExt) DelRegKeySimple(_T(".wrl.gz"),lpszProgID);
		//SetRegKey(HKEY_CLASSES_ROOT, _T(".wrl.gz"), _T("Content Type"),	_T("model/vrml")); 

		// test 
		if (registerExt) DelRegKeySimple(_T(".wav.gz"),lpszProgID);
		//SetRegKey(HKEY_CLASSES_ROOT, _T(".wav.gz"), _T("Content Type"),	_T("audio/wav")); 

		if (registerExt) DelRegKeySimple(_T(".wrz"),lpszProgID);
		//SetRegKey(HKEY_CLASSES_ROOT, _T(".wrz"), _T("Content Type"),	_T("model/vrml")); 
	
		if (registerExt) DelRegKeySimple(_T(".vrml"),lpszProgID);
	
		// SetRegKey(HKEY_CLASSES_ROOT, _T(".vrml"),_T("Content Type"),	_T("model/vrml")); 

		// stupid, but we need to register for gz to get wrl.gz 
		if (registerExt) 	DelRegKeySimple(_T(".gz"),lpszProgID);

		//SetRegKey(HKEY_CLASSES_ROOT, _T(".gz"),_T("Content Type"),	_T("application/x-gzip")); 
		//SetRegKey(HKEY_CLASSES_ROOT, _T(".gz"),_T("Content Type"),	_T("application/x-gzip-compressed"));  // hg 23.02.99
		// IE 4.01 adds a PROTOCOLS/ entry

	}
	
	
	// create version independent entry 
	{
		CString pid;
		pid =  lpszProgID;

			int arg = pid.ReverseFind('.');
			if (arg>0) pid=pid.Left(arg);

			wsprintf(szKey, _T("%s"),(const TCHAR *) pid);
			RecursiveRegDeleteKey(HKEY_CLASSES_ROOT, szKey);
			RecursiveRegDeleteKey(HKEY_CLASSES_ROOT, lpszProgID);

	}




//Error:

	return bSuccess;

}


#if 0


// to be removed, now implemented at the GFile level 

class CBindStatusCallback : public IBindStatusCallback 
{

  public:
    // IUnknown methods
    STDMETHODIMP    QueryInterface(REFIID riid,void ** ppv);
    STDMETHODIMP_(ULONG)    AddRef()    { return m_cRef++; }
    STDMETHODIMP_(ULONG)    Release()   { if (--m_cRef == 0) { /* delete this;  return 0; */ } return m_cRef; }

    // IBindStatusCallback methods
    STDMETHODIMP    OnStartBinding(DWORD grfBSCOption, IBinding* pbinding);
    STDMETHODIMP    GetPriority(LONG* pnPriority);
    STDMETHODIMP    OnLowResource(DWORD dwReserved);
	STDMETHOD(OnProgress)( 
            /* [in] */ ULONG ulProgress,
            /* [in] */ ULONG ulProgressMax,
            /* [in] */ ULONG ulStatusCode,
            /* [in] */ LPCWSTR szStatusText);
    STDMETHODIMP    OnStopBinding(HRESULT hrResult, LPCWSTR szError);
    STDMETHODIMP    GetBindInfo(DWORD* pgrfBINDF, BINDINFO* pbindinfo);
    STDMETHODIMP    OnDataAvailable(DWORD grfBSCF, DWORD dwSize, FORMATETC *pfmtetc,
                        STGMEDIUM* pstgmed);
    STDMETHODIMP    OnObjectAvailable(REFIID riid, IUnknown* punk);

    // constructors/destructors
    CBindStatusCallback(CGLViewCtrlCtrl* pView);
    ~CBindStatusCallback();

    // data members
	DWORD				m_cRef;
    IBinding*			m_pbinding;
    IStream*			m_pstm;
    DWORD				m_cbOld;
	CGLViewCtrlCtrl*	m_pView;

	// saw this in mfc\src\oleasmon.cpp
#ifdef _AFXDLL
	AFX_MODULE_STATE*	m_pModuleState;
#endif


};



CBindStatusCallback::CBindStatusCallback(CGLViewCtrlCtrl* pView)
{
    m_pbinding = NULL;
    m_pstm = NULL;
    m_cRef = 1;
    m_cbOld = 0;
	m_pView = pView;
#ifdef _AFXDLL
	m_pModuleState = AfxGetModuleState();
	ASSERT(m_pModuleState != NULL);
#endif

}  

CBindStatusCallback::~CBindStatusCallback()
{
    if (m_pstm)
        m_pstm->Release();
    if (m_pbinding)
        m_pbinding->Release();
}  

STDMETHODIMP CBindStatusCallback::QueryInterface(REFIID riid, void** ppv)
{ 
    *ppv = NULL;
    
    if (riid==IID_IUnknown || riid==IID_IBindStatusCallback)
        {
        *ppv = this;
        AddRef();
        return S_OK;
        }
    return E_NOINTERFACE;
}  

STDMETHODIMP CBindStatusCallback::OnStartBinding(DWORD grfBSCOption, IBinding* pbinding)
{
	if(NULL != pbinding)
	{
		pbinding->AddRef();
		m_pbinding = pbinding;
	}

	return(NOERROR);
} 

STDMETHODIMP CBindStatusCallback::OnStopBinding(HRESULT hrStatus, LPCWSTR pszError) 
{
	// clear the progress bar
//	m_pFormView->m_ctlProgress.SetPos(0);

	if(NULL != m_pbinding)
	{
		m_pbinding->Release();
		m_pbinding = NULL;
	}

	return(NOERROR);
}

STDMETHODIMP CBindStatusCallback::GetPriority(LONG* pnPriority) 
{
	 return(NOERROR);
}  

STDMETHODIMP CBindStatusCallback::OnLowResource(DWORD dwReserved)
{
 	 return(NOERROR);
}  

STDMETHODIMP CBindStatusCallback::GetBindInfo(DWORD* pgrfBINDF, BINDINFO* pbindInfo)
{
	//if (reload) *pgrf |= BINDFBINDF_GETNEWESTVERSION; 
/*
		*pgrfBINDF = BINDF_ASYNCHRONOUS | BINDF_ASYNCSTORAGE |
			BINDF_GETNEWESTVERSION | BINDF_NOWRITECACHE;
		pbindInfo->cbSize = sizeof(BINDINFO);
		pbindInfo->szExtraInfo = NULL;
		memset(&pbindInfo->stgmedData, 0, sizeof(STGMEDIUM));
		pbindInfo->grfBindInfoF = 0;
		pbindInfo->dwBindVerb = BINDVERB_GET;
		pbindInfo->szCustomVerb = NULL;
		return S_OK;

*/
	 return (NOERROR);

}

STDMETHODIMP CBindStatusCallback::OnDataAvailable(DWORD grfBSCF, 
		DWORD dwSize, FORMATETC* pfmtetc, STGMEDIUM* pstgmed) 
{  // CBindstatusCallback see ATLCTL.h
	return(NOERROR);
}

STDMETHODIMP CBindStatusCallback::OnObjectAvailable(REFIID riid, IUnknown* punk) 
{
	return(NOERROR);
}  
//HRESULT m_pBinding.Abort(void); 
// IWindowForBindingUI
STDMETHODIMP CBindStatusCallback::OnProgress(ULONG ulProgress,
		ULONG ulProgressMax, ULONG ulStatusCode, LPCWSTR pwzStatusText) 
{
	USES_CONVERSION;

	if (pwzStatusText == NULL) return NOERROR;

	AFX_MANAGE_STATE(m_pModuleState);

	LPCTSTR message = W2CT(pwzStatusText);
	LPCTSTR message1= "";
	switch (ulStatusCode) {		
    case BINDSTATUS_FINDINGRESOURCE : message1= "Finding "; break;
    case BINDSTATUS_CONNECTING : message1= "Connecting "; break;
    case BINDSTATUS_SENDINGREQUEST : message1= "Requesting "; break;
    case BINDSTATUS_REDIRECTING : message1= "Redirecting "; break;
    case BINDSTATUS_USINGCACHEDCOPY : message1= "Cached "; break;
    case BINDSTATUS_BEGINDOWNLOADDATA : message1= "Begin "; break;
    case BINDSTATUS_DOWNLOADINGDATA : message1= ""; break;
    case BINDSTATUS_ENDDOWNLOADDATA : message1= "End "; break;
	default: break;
	}


	TRACE("ulProgress = %u\t ulProgressMax = %u ulStatusCode = %u pwzStatusText = %s\n", 
			ulProgress, ulProgressMax, ulStatusCode, message);

	if (ulProgressMax==0) ulProgressMax=1;
	
	CString msg;
	sprintf(msg.GetBuffer(200+strlen(message)),"%s%d%% %s", message1, (int) ((ulProgress *100) / ulProgressMax),message);
	msg.ReleaseBuffer();

	m_pView->Message(msg,PROGRESS_MESSAGE);

	// set the range for the progress control

	// I am dividing the progress values by two here because they are ULONG 
	// and the control takes ints.  This way we dont get negative numbers in the
	// case of progress values larger that INT_MAX

//	if( BINDSTATUS_BEGINDOWNLOADDATA == ulStatusCode )
//		m_pFormView->m_ctlProgress.SetRange( 0, ulProgressMax / 2 );

//	if( 0 != ulProgressMax )
//		m_pFormView->m_ctlProgress.SetPos(ulProgress /2 );

 	return(NOERROR);
}

#endif



/////////////////////////////////////////////////////////////////////////////
// CGLViewCtrlCtrl::CGLViewCtrlCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CGLViewCtrlCtrl

BOOL CGLViewCtrlCtrl::CGLViewCtrlCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegInsertable | afxRegApartmentThreading to afxRegInsertable.

	if (bRegister) {
		BOOL retval = AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_GLVIEWCTRL,
			IDB_GLVIEWCTRL,
			afxRegInsertable | afxRegApartmentThreading,
			_dwGLViewCtrlOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);

#ifdef REG_MIME
		int regMime=1;
		int regExt=1; // testt 

		// query option from install program, if we should register mimetypes 
		HKEY key=NULL;

		if (::RegOpenKey(HKEY_CLASSES_ROOT,m_lpszProgID,&key) ==
			ERROR_SUCCESS) {
			if (GetRegKey(key,_T("RegisterMimeTypes"),regMime)) {
				SetRegKey(key,_T("RegisterMimeTypes"),1); // reset for next time
			}
			if (GetRegKey(key,_T("RegisterExtensions"),regExt)) {
				SetRegKey(key,_T("RegisterExtensions"),1); // reset for next time put in back 23.02.98 HG
			}
			::RegCloseKey(key);
		}
		if (retval) {
#ifdef _DEBUG_XXX
				CString msg;
				msg.Format("Registering mimetypes %s  RegisterMimeTypes = %d RegisterExtensions = %d",m_lpszProgID,regMime,regExt);
				AfxMessageBox(msg);
#endif
			
			RegisterMimeTypes(m_clsid,m_lpszProgID,regMime,regExt);

		}
#endif


		// mark as safe for scripting--failure OK
		HRESULT hr = CreateComponentCategory(CATID_SafeForScripting, 
			L"Controls that are safely scriptable");

		if (SUCCEEDED(hr))
			// only register if category exists
			RegisterCLSIDInCategory(m_clsid, CATID_SafeForScripting);
			// don't care if this call fails

		// mark as safe for data initialization
		hr = CreateComponentCategory(CATID_SafeForInitializing, 
			L"Controls safely initializable from persistent data");

		if (SUCCEEDED(hr))
			// only register if category exists
			RegisterCLSIDInCategory(m_clsid, CATID_SafeForInitializing);
			// don't care if this call fails

		return retval;

	}
	else {
		// delete preferences 
		RecursiveRegDeleteKey(HKEY_CURRENT_USER, USER_KEY);
		if (AfxOleUnregisterClass(m_clsid, m_lpszProgID)) {
			UnregisterMimeTypes(m_clsid,m_lpszProgID,TRUE,TRUE);
			return TRUE;
		} else return FALSE; 
	}
}


// #define  _HAS_OSVERSIONINFOEX


// get system info 
BOOL GetSystemVersionDisplay(CString &s,CString &version)
{

#ifdef _HAS_OSVERSIONINFOEX
	// W2000 coded , supported in VC 6.0 

   OSVERSIONINFOEX osvi;
   BOOL bOsVersionInfoEx;

   // Try calling GetVersionEx using the OSVERSIONINFOEX structure,
   // which is supported on Windows 2000.
   //
   // If that fails, try using the OSVERSIONINFO structure.

   ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
   osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

   if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
   {
      // If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.

      osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
      if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
         return FALSE;
   }

#else
	// VC 50. safe
	OSVERSIONINFO osvi;
    BOOL bOsVersionInfoEx=FALSE;

    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));

    osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
    if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
         return FALSE;

#endif


   switch (osvi.dwPlatformId)
   {
      case VER_PLATFORM_WIN32_NT:

      // Test for the product.

         if ( osvi.dwMajorVersion <= 4 )
            s = ("Microsoft Windows NT ");

         if ( osvi.dwMajorVersion == 5 )
            s = ("Microsoft Windows 2000 ");

      // Test for workstation versus server.
#ifdef VER_NT_WORKSTATION
         if( bOsVersionInfoEx )
         {
            if ( osvi.wProductType == VER_NT_WORKSTATION )
               s+=  ( "Professional " );

            if ( osvi.wProductType == VER_NT_SERVER )
               s+= ( "Server " );
         }
         else
#endif
         {
            HKEY hKey;
            char szProductType[80];
            DWORD dwBufLen;

            RegOpenKeyEx( HKEY_LOCAL_MACHINE,
               "SYSTEM\\CurrentControlSet\\Control\\ProductOptions",
               0, KEY_QUERY_VALUE, &hKey );
            RegQueryValueEx( hKey, "ProductType", NULL, NULL,
               (LPBYTE) szProductType, &dwBufLen);
            RegCloseKey( hKey );
            if ( lstrcmpi( "WINNT", szProductType) == 0 )
               s+= ( "Workstation " );
            if ( lstrcmpi( "SERVERNT", szProductType) == 0 )
               s+= ( "Server " );
         }

      // Display version, service pack (if any), and build number.

         version.Format ("version %d.%d %s (Build %d)\n",
            osvi.dwMajorVersion,
            osvi.dwMinorVersion,
            osvi.szCSDVersion,
            osvi.dwBuildNumber & 0xFFFF);


         break;

      case VER_PLATFORM_WIN32_WINDOWS:

         if ((osvi.dwMajorVersion > 4) || 
            ((osvi.dwMajorVersion == 4) && (osvi.dwMinorVersion > 0)))
         {
             s = ("Microsoft Windows 98 ");
         } 
         else s =("Microsoft Windows 95 ");
         version.Format ("version %d.%d %s (Build %d)\n",
            osvi.dwMajorVersion,
            osvi.dwMinorVersion,
            osvi.szCSDVersion,
            osvi.dwBuildNumber & 0xFFFF);

         break;

      case VER_PLATFORM_WIN32s:

         s = ("Microsoft Win32s ");
         version.Format ("version %d.%d %s (Build %d)\n",
            osvi.dwMajorVersion,
            osvi.dwMinorVersion,
            osvi.szCSDVersion,
            osvi.dwBuildNumber & 0xFFFF);

         break;
   }
   return TRUE; 
}


/////////////////////////////////////////////////////////////////////////////
// CGLViewCtrlCtrl::CGLViewCtrlCtrl - Constructor

CGLViewCtrlCtrl::CGLViewCtrlCtrl() 
{
	InitializeIIDs(&IID_DGLViewCtrl, &IID_DGLViewCtrlEvents);

	TRACE("CGLViewCtrlCtrl::CGLViewCtrlCtrl() %p\n",this);

	m_initialThirdPersonView = FALSE;


	m_driverHints = new GDriverHints();



	///////////////////navpanel\\\\\\\\\\\\\\\\\\\\\\\
	//the panel is created in GView according to the settings
	//in the property-page (registry)
	//m_PanelEnabled = TRUE;

	m_navPanelDrag = FALSE;
	InsideSphere = FALSE;

	m_NavSlideStatePanel = FALSE;
	m_NavUPStatePanel = FALSE;		
	m_NavDOWNStatePanel = FALSE;	
	m_NavLEFTStatePanel = FALSE;		
	m_NavRIGHTStatePanel = FALSE;

	m_navPanelOn_Embed = TRUE;
	m_forceHwProblemCheck = TRUE;
	m_fullscreenOnStartup = FALSE;
	m_hideCursor = FALSE;

	m_lReadyState = READYSTATE_LOADING;
	// TODO: Call InternalSetReadyState when the readystate changes.

	// Initialize your control's instance data here.
	m_uiMode = UI_EXPERT;

	m_displayFrameRate = FALSE;
	m_displayFrameStats = FALSE;
	m_frames = 0;
	m_checkFrames = 0;

	m_startTime= timeGetTime(); 
	m_endTime= timeGetTime(); 

	m_handleWrlIntern=TRUE; // handle wrl anchors internal

	m_timerRunning = FALSE;
	m_NavTimerRunning = FALSE;
	m_NavCollision.LastCollision = FALSE;
	m_NavFollowObject = FALSE;
	// interval for animation timer


//	m_timerInterval = 25;
	m_timerInterval = 10;

	// no navigation running
	m_NavRunning = 0;
	// no navigation interpolator running
	m_NavIPActive = 0;
	
    m_idleCounter = 0;

	m_timerCnt				  = 0;
	m_NavTimerCnt			  = 0;
	m_NavTimerCnt_1			  = 0;
	m_NavCursorBlkCount		  = 0;
	m_NavCursorShow			  = TRUE;
	m_NavCursorModeOld		  = -2;
	m_NavCamera.FixPointValid = FALSE;
	m_NavCoordDspl			  = FALSE;
	m_NavNotActive			  = FALSE;
	m_NavMotion				  = FALSE;
//	m_NavTriggerRotation	  = FALSE; 
//	m_NavTriggerSlide		  = FALSE; 
//	m_NavTrigger			  = FALSE; 
	m_NavIsWithinRect		  = FALSE;
	m_NavFocus				  = FALSE;

	m_NavOldSlideState			= FALSE;				
	m_NavCNTLState				= FALSE;				
	m_NavSlideState				= FALSE;				
	m_NavSpeedState				= FALSE;
	m_NavFollowState			= FALSE;	
	m_NavRotState				= FALSE;	
	m_NavPointState				= FALSE;
	m_NavUPState				= FALSE;
	m_NavDOWNState				= FALSE;
	m_NavLEFTState				= FALSE;
	m_NavRIGHTState				= FALSE;
	m_NavLBUTState				= FALSE;	

	m_collisionState			= FALSE; 


//	m_tcount = 0;

//m_timerTimeSensorCycle = 2;
	m_timerTimeSensorCycle = 1; // 17.08.97

	updateLock = 0;
	m_mustRedraw = FALSE;
	
	m_renderThread = NULL;
	m_useRenderThread = TRUE; // use render thread
	m_useRenderThread = FALSE; // don't render thread

	InitializeCriticalSection(&m_sceneSection); 


	view = NULL;
	m_minimized = FALSE;
	m_menuActive = FALSE;

	expired = FALSE;

	lastMessageType=0;
	lastMessage="";
    m_messageUrl = "";
	m_messageTargetFrame = "";

	lastCursorMode = 0;
	hCurrentCursor = NULL;

	m_initialized = FALSE;
	m_enableInteraction = TRUE;

	m_pDC = NULL;

	m_fullSpeed = FALSE; 
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	if (si.dwProcessorType >=PROCESSOR_INTEL_PENTIUM)
//#ifdef _KATMAI
		m_fullSpeed = TRUE; // should depend on system & HW 
//#endif

	m_verbose = FALSE; 

	m_askHardware = TRUE; 
	m_saveDriverSettings = FALSE;

	m_dAskHardware = NULL;
	m_dConsole = NULL;
	m_dPad = NULL;


	m_reporter = NULL;

#ifdef _OGL_XX
	m_glptr = NULL;
#endif

	mainLoader = NULL;

	m_historyIndex = -1;

	m_wasDrag = FALSE;
	m_dragEps = 2;
	m_currentAnchor=NULL;
	m_currentTSensor = FALSE;


	m_currentCameraMode	= NULL_MODE;
	m_cameraSpeedFac = 10;
	// these are computed in NavStart
	m_cameraSpeed = 0;
	m_cameraSpeedRot = 0;


    MOVE_TIMER_ID=123;

    m_sensorTest=TRUE;
	m_anchorSingleClick = TRUE;
	
	m_pt				= NULL;
	m_pd				= NULL;
	m_threadFinished	= TRUE;

	ResetMove();
}


// get the reporter object 
GReporter *CGLViewCtrlCtrl::GetReporter()
{
	if (m_reporter == NULL)
		return GReporter::current; // return global reporter object (Reporter)
	return m_reporter;
}


/////////////////////////////////////////////////////////////////////////////
// CGLViewCtrlCtrl::~CGLViewCtrlCtrl - Destructor

CGLViewCtrlCtrl::~CGLViewCtrlCtrl()
{
	TRACE("CGLViewCtrlCtrl::~CGLViewCtrlCtrl() %p\n",this);
	TRACEREF();
	
	// TODO: Cleanup your control's instance data here.
	if ( m_pt != NULL ) 
	{
		if ( m_pd != NULL )
			 m_pd->EndDialog(IDCANCEL);
		else
  			 m_pt->PostThreadMessage(WM_QUIT, NULL, 0);

		while ( !m_threadFinished ) Sleep(200);
		m_pt = NULL;
		m_pd = NULL;
	}

	if (mainLoader)	 // should already happened in OnDestroy  kill main URL loader
    {
   		mainLoader->Kill();
   		mainLoader->unref();
		mainLoader = NULL;
	}
	
	
	if (m_reporter) {
		if (GReporter::current == m_reporter)
			GReporter::SetCurrent(GReporter::defaultReporter,GReporter::defaultReporter);
		delete m_reporter;
		m_reporter = NULL;
	}
	if (m_driverHints) delete m_driverHints;

	DeleteCriticalSection(&m_sceneSection); 


}



//
// OnStatusMessage -  Print a status line message
//
LRESULT CGLViewCtrlCtrl::OnStatusMessage(WPARAM wParam, LPARAM lParam)
{
	CString msg((const char*) wParam);
	Message(msg,lParam);
	return 0;

}

// write actual console buffer to console
LRESULT CGLViewCtrlCtrl::OnConsoleMessage(WPARAM wParam, LPARAM lParam)
{	BOOL popup = !(BOOL) wParam;
	if (m_dConsole && m_reporter) {
		//HWND fg=::GetForegroundWindow();
		CWnd * fg  = GetForegroundWindow();

		if (popup && !m_dConsole->m_hide) 
			m_dConsole->ShowWindow(SW_SHOWNOACTIVATE); // SW_SHOWNA);
		if (m_reporter->m_print.GetLength()>0) 
			m_dConsole->AddText(m_reporter->m_print); 
		m_reporter->m_print = "";
/*
		if (::GetForegroundWindow() != fg) { // Thilo : reset foreground 
			::SetForegroundWindow(fg);
		}
*/
/* hg 10.06.99 disabled because of Peter's Problems  ??
		if (::GetForegroundWindow() != fg) { // Thilo : reset foreground 
			::SetForegroundWindow(fg);
		}
*/
	}
	return 0;

}


void CGLViewCtrlCtrl::Message(const char *message,int messageType)
{
	

	AFX_MANAGE_STATE(m_pModuleState); // XX
	
	if (!view) return;  // ignore 
	if (view->GetFullScreen()) return;  // ignore in fullscreen mode 


	lastMessage = message;
	lastMessageType = messageType;

#ifdef _DEBUG
	if (lastMessage.GetLength() == 0) {
		TRACE("xx \n");
	}

#endif
  
	USES_CONVERSION;

	// check if observer wants the message 
	if (view->observerFlags & GOBSERVE_MESSAGE) {
		view->observer->OnStatusMessage((BSTR) T2COLE(lastMessage),lastMessageType);
		return;
	}
	if (view->observerFlags & GOBSERVE_WM_MESSAGE) {
		// hg ::SendMessage(view->observerWnd, WM_USER + 500, (WPARAM) (const char*)lastMessage,lastMessageType);
		// hg new 
		::PostMessage(view->observerWnd, WM_USER + 500, (WPARAM) (const char*)lastMessage,lastMessageType);
		return;
	}



	if (m_pInPlaceFrame == NULL) return; 

	// notify container of new status text
 	m_pInPlaceFrame->SetStatusText(T2COLE(message));

}

// send a message to the console
void CGLViewCtrlCtrl::Print(const char *message)
{
	GReporter *reporter = GetReporter();
	if (reporter) reporter->Trace1(message);
}

// see COleControl::GetMessageString
void CGLViewCtrlCtrl::GetMessageString(UINT nID, CString& rMessage) const
{	// need to get ride of constness. translator caches strings 
	((CGLViewCtrlCtrl *)this)->TranslateMessageString(nID,rMessage);
/*	return;	
	// load appropriate string
	LPTSTR lpsz = rMessage.GetBuffer(255);
	if (AfxLoadString(nID, lpsz) != 0)
	{
		// first newline terminates actual string
		lpsz = _tcschr(lpsz, '\n');
		if (lpsz != NULL)
			*lpsz = '\0';
	}
	else
	{
		// not found
		TRACE1("Warning: no message line prompt for ID 0x%04X.\n", nID);
	}
	rMessage.ReleaseBuffer();
*/
}


// set state, if user currently is in collision during movement
void CGLViewCtrlCtrl::SetCollisionState(BOOL newState)
{
	if (m_collisionState != newState) {
		m_collisionState = newState;
		view->SetColliding(!!newState);

		if (m_collisionState) {

			 SetCursorMode(NAV_COLLISION);

		}
		else  {
			// SetCursorMode(NAV_COLLISION);

		}
	}
}


// set the cursor mode depending on action 
void CGLViewCtrlCtrl::SetCursorMode(int cursorMode)
{
//HCURSOR		hCursor;
	HCURSOR		hOldCursor;

	lastCursorMode = cursorMode;
	//TRACE("CGLViewCtrlCtrl::SetCursorMode.................%i\n", cursorMode);

	if (m_hideCursor)
		cursorMode = NAV_NO_CURSOR;

	if (cursorMode >= 0)
	{
		hOldCursor = SetCursor(NavCursors[cursorMode]);
		hCurrentCursor = NavCursors[cursorMode]; 
	}


/*
	lastCursorMode=cursorMode;

	switch (lastCursorMode) {	
	HCURSOR hCursor;
	case STANDARD_CURSOR:
		if (mainLoader)
		hCursor = cursors[STANDARD_LOADING_CURSOR];
		else hCursor = cursors[cursorMode];
		
		break;
	case NAVIGATION_CURSOR:
		if (mainLoader)
		hCursor = cursors[STANDARD_LOADING_CURSOR];
		else hCursor = cursors[cursorMode];
		
		break;
	default :
		hCursor = cursors[cursorMode];
		break;
 };
*/  


}

#if 0

// set a default pixel fromat
int CGLViewCtrlCtrl::SetDefaultPixelFormat(BOOL DoubleBuffer,int depthBits, BOOL softwareRendering, BOOL Rgb)
{
	int nNewIdx=0;

#ifdef _OGL
	  DWORD dwFlags = (DWORD)0;

	  DWORD type = (::GetObjectType(m_glptr->m_hdc));

#ifdef _DEBUG
	  if (type != OBJ_DC) {
		  CString x;
		  x.Format("DC type = %d ",type);
		  AfxMessageBox(x);
	  }
#endif

	  memset(&m_glptr->m_PixFmtDesc, 0, sizeof(PIXELFORMATDESCRIPTOR));
	  dwFlags = 0;
	  if (type == OBJ_DC) dwFlags |=  PFD_DRAW_TO_WINDOW;
	  dwFlags |=  PFD_SUPPORT_OPENGL;
	  if (type == OBJ_DC) dwFlags |= (DoubleBuffer) ? PFD_DOUBLEBUFFER : 0;
	  
	  if (softwareRendering) { 
		  dwFlags |= PFD_GENERIC_FORMAT | PFD_SUPPORT_GDI;
	  } 	
	  m_glptr->m_PixFmtDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	  m_glptr->m_PixFmtDesc.nVersion = 1;
	  m_glptr->m_PixFmtDesc.dwFlags = dwFlags;
	  m_glptr->m_PixFmtDesc.iPixelType = (Rgb) ? PFD_TYPE_RGBA : PFD_TYPE_COLORINDEX;
	  m_glptr->m_PixFmtDesc.cDepthBits = depthBits;

	  nNewIdx = m_glptr->ChoosePixelFormat(m_glptr->m_hdc, &m_glptr->m_PixFmtDesc);

	  if ((m_glptr->IsNativeIndex(m_glptr->m_hdc, nNewIdx)) || (m_glptr->IsDeviceIndex(m_glptr->m_hdc, nNewIdx))) {

		if (m_glptr->GetGLRC(m_glptr->m_hdc)) {
			m_glptr->SetSurfaceNotInitialized();
		} else {
		  MessageBox("Sorry, OpenGL Pixel Format can not be changed", NULL, MB_OK | MB_ICONEXCLAMATION);
		  nNewIdx = -1;
		}
	  }
	  else {
		 MessageBox("Unsupported OpenGL Pixel Format", NULL, MB_OK | MB_ICONEXCLAMATION);
		 nNewIdx = -1;
	  }	
#endif
	 return(nNewIdx);
}

#endif

// mark view for redraw 
void CGLViewCtrlCtrl::Redraw()
{
	if (m_useRenderThread) {
		m_mustRedraw = TRUE;
	} 
	else 
	{
		// short cut 
		InvalidateRect(NULL, FALSE);
		//InvalidateControl(); // does more 
	}
}

// get the onlySystemMemory Direct 3D setting flag 
BOOL GetSysMemFlag()
{
	HKEY hKeyRoot=NULL;
	CString val;
	
	BOOL bOnlySystemMemory = TRUE; // change default to TRUE, HW uses video automatically


	::RegOpenKey(HKEY_CURRENT_USER, USER_KEY, &hKeyRoot);

	if (hKeyRoot && GetRegKey(hKeyRoot,_T("Direct3D.onlySystemMemory"), bOnlySystemMemory)) {
	}

	if (hKeyRoot) ::RegCloseKey(hKeyRoot);

	return bOnlySystemMemory;

}



BOOL CGLViewCtrlCtrl::Initialize(HDC hDC)
{

	TRACE("CGLViewCtrlCtrl::Initialize() %p\n",this);
	BOOL initOk = FALSE;
	int v;
	float vf;

	if (m_reporter == NULL) { // create reporter object 
		m_reporter = new CCtrlReporter(this);
		m_reporter->verbose = m_verbose;
	}

	GetClientRect(&m_clientRect);

	HKEY hKeyRoot=NULL;
	CString val;

	::RegOpenKey(HKEY_CURRENT_USER, USER_KEY, &hKeyRoot);

	// initialize cursors 
	hCurrentCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
/*
	for(int i=0; i<= ANCHOR_CURSOR;i++) cursors[i] = hCurrentCursor;
	cursors[STANDARD_LOADING_CURSOR] = AfxGetApp()->LoadStandardCursor(IDC_APPSTARTING);
	//cursors[NAVIGATION_CURSOR] = AfxGetApp()->LoadStandardCursor(IDC_SIZE);
	
   	cursors[MOVING_CURSOR] = AfxGetApp()->LoadStandardCursor(IDC_CROSS);
	cursors[SENSOR_CURSOR] = AfxGetApp()->LoadStandardCursor(IDC_UPARROW);
	cursors[ANCHOR_CURSOR] = AfxGetApp()->LoadStandardCursor(IDC_SIZEALL);
	cursors[COLLISION_CURSOR] = AfxGetApp()->LoadStandardCursor(IDC_NO);
	cursors[ZOOM_CURSOR] = AfxGetApp()->LoadStandardCursor(IDC_SIZEWE);
*/
   	// NavCursors[NAV_NONE]			= AfxGetApp()->LoadCursor(IDC_CURSOR22);
   	// hg 29.03.99 use same cursor as in IE ! 
	NavCursors[NAV_NONE]			= AfxGetApp()->LoadStandardCursor(IDC_ARROW);

   	NavCursors[NAV_WALK]			= AfxGetApp()->LoadCursor(IDC_CURSOR1);
   	NavCursors[NAV_SLIDE]			= AfxGetApp()->LoadCursor(IDC_CURSOR2);
   	NavCursors[NAV_ROTATE]			= AfxGetApp()->LoadCursor(IDC_CURSOR3);
   	NavCursors[NAV_EXAMINE]			= AfxGetApp()->LoadCursor(IDC_CURSOR4);
   	NavCursors[NAV_FLY]				= AfxGetApp()->LoadCursor(IDC_CURSOR5);
   	NavCursors[NAV_WALK_ROT]		= AfxGetApp()->LoadCursor(IDC_CURSOR17);
   	NavCursors[NAV_WALK_SLIDE]		= AfxGetApp()->LoadCursor(IDC_CURSOR18);
   	NavCursors[NAV_WALK_ROT_SLIDE]	= AfxGetApp()->LoadCursor(IDC_CURSOR19);
   	NavCursors[NAV_SLIDE_ROT]		= AfxGetApp()->LoadCursor(IDC_CURSOR16);
   	NavCursors[NAV_ROT_SLIDE]		= AfxGetApp()->LoadCursor(IDC_CURSOR13);
   	NavCursors[NAV_EXA_SLIDE]		= AfxGetApp()->LoadCursor(IDC_CURSOR14);
   	NavCursors[NAV_JUMP_TO]			= AfxGetApp()->LoadCursor(IDC_CURSOR20);
   	NavCursors[NAV_FOLLOW]			= AfxGetApp()->LoadCursor(IDC_CURSOR21);
   	
	NavCursors[NAV_ANCHOR]			= AfxGetApp()->LoadCursor(IDC_CURSOR11); // hand
   	
	// NavCursors[NAV_NONE_SENSOR]		= AfxGetApp()->LoadCursor(IDC_CURSOR15);
   	NavCursors[NAV_NONE_SENSOR]		= AfxGetApp()->LoadCursor(IDC_CURSOR11); // point hand test hg 16.01.00

   	NavCursors[NAV_WALK_SENSOR]		= AfxGetApp()->LoadCursor(IDC_CURSOR6);
   	NavCursors[NAV_SLIDE_SENSOR]	= AfxGetApp()->LoadCursor(IDC_CURSOR7);
   	NavCursors[NAV_ROTATE_SENSOR]	= AfxGetApp()->LoadCursor(IDC_CURSOR8);
   	NavCursors[NAV_EXAMINE_SENSOR]	= AfxGetApp()->LoadCursor(IDC_CURSOR9);
   	NavCursors[NAV_FLY_SENSOR]		= AfxGetApp()->LoadCursor(IDC_CURSOR10);
   	
	// NavCursors[NAV_ANCHOR_SENSOR]	= AfxGetApp()->LoadCursor(IDC_CURSOR12);
   	NavCursors[NAV_ANCHOR_SENSOR]		= AfxGetApp()->LoadCursor(IDC_CURSOR11); // point hand test hg 16.01.00

   	NavCursors[NAV_PANEL_CURSOR]	= AfxGetApp()->LoadCursor(IDC_CURSOR23);
   	NavCursors[NAV_PAN]				= AfxGetApp()->LoadCursor(IDC_CURSOR24);
   	NavCursors[NAV_NO_CURSOR]		= AfxGetApp()->LoadCursor(IDC_CURSOR25);
	NavCursors[NAV_COLLISION]		= AfxGetApp()->LoadStandardCursor(IDC_NO);
	
	// try to load IE urlmon.lib
#ifdef G_URLMON
	InitializeUrlmonLib();
#endif
    //
    int depthBits=16;
	int rsoftware=0;
/*
#ifdef _OGL
	if (!m_glptr) m_glptr = new COpenGL(hDC); //(;pdc->m_hDC);

	if (GetRegKey(hKeyRoot,_T("OpenGL.zdepth"),v) && v>0)
			depthBits=v;
	if (GetRegKey(hKeyRoot,_T("OpenGL.software"),v) && v>0)
			rsoftware=v;


#endif
*/
	if (!view) 
    {
		view = new GView(); // create the view  object 
		view->ref();
		//if (view) view->m_navPanelOn = view->m_navPanelOn_Prop = m_PanelEnabled;			

	}
	view->SetView(this);	// back pointer
	view->SetViewController(this);	// back pointer

	view->degradeDuringMove = 0;

	view->SetTheTime(); // set global VRML eventTime
	
	// Read settings from ini file 
	//ReadProfileSetting();

	
#ifdef _OGL_XXX
	if (SetDefaultPixelFormat(1,depthBits,rsoftware)<=0) {
	   return (FALSE);
	}
#endif
	
	m_initialized = TRUE;


#ifdef _OGL
	if (view->Initialize(GetSafeHwnd(),hDC)<0) {
		TRACE("Error during initialization \n");

		return FALSE;
	}
/* 
	if (!m_glptr->IsSurfaceInitialized())
	{
		view->Initialize(GetSafeHwnd());
		m_glptr->SetSurfaceInitialized();
	}
*/
	initOk = TRUE;
#else
	CString driver;	// preferenced Driver, default "Ramp Driver"
	BOOL  bOnlySystemMemory = TRUE; // FALSE; VIDEOMEM automatic for HW
 	BOOL  bOnlyEmulation = FALSE;

	CString mode;

	if (hKeyRoot && GetRegKey(hKeyRoot,_T("Direct3D.askHardware"),v))
		m_askHardware = v;

	int driverOk=1;
	if (hKeyRoot && GetRegKey(hKeyRoot,_T("Direct3D.driverOk"),v))
		driverOk = v;	

	if (hKeyRoot && GetRegKey(hKeyRoot,_T("Direct3D.driver"), val)) {

		if (val == "Direct3D HAL" && (m_askHardware)) {
			m_askHardware = 1; // ??
		}
		if (val.GetLength()>0) driver = val;

	}

	if (hKeyRoot && GetRegKey(hKeyRoot,_T("Direct3D.onlySystemMemory"), bOnlySystemMemory)) {
		m_driverHints->m_onlySystemMemory = bOnlySystemMemory;
	}

	if (hKeyRoot && GetRegKey(hKeyRoot,_T("Direct3D.bOnlyEmulation"), bOnlyEmulation)) {
		m_driverHints->m_onlyEmulation = bOnlyEmulation;
	}


	if (hKeyRoot && GetRegKey(hKeyRoot,_T("Direct3D.fullscreenMode"), mode)) {
		if (m_driverHints->m_fullScreenMode.GetLength() == 0 ) // check order ??
			m_driverHints->m_fullScreenMode = mode;
	}
	
	SetProfile(_T("Direct3D.driverOk"),0);

	if (!driverOk && driver == "Direct3D HAL" && m_forceHwProblemCheck) {
		
		DAskHardwareProblem d(this);

		int ret=DoThreadModal(&d);
		
		if (ret == ID_SAFE_MODE)  {	
			m_driverHints->m_onlyEmulation= bOnlyEmulation = TRUE;
			m_driverHints->m_onlySystemMemory = bOnlySystemMemory = TRUE;
			driver = "";
			DelRegKey(hKeyRoot,	_T("Direct3D.driver")); 
			DelRegKey(hKeyRoot,	_T("Direct3D.textureFiltering")); 
			DelRegKey(hKeyRoot,	_T("Direct3D.textureMipMap")); 
			DelRegKey(hKeyRoot,	_T("Direct3D.antiAlias")); 
		}
		/*
		else if (ret == ID_USEHWINDX3)  	
		{
			SetProfile(_T("Direct3D.dxlevel"),3);
			bOnlyEmulation = FALSE;
			DelRegKey(hKeyRoot,	_T("Direct3D.textureMipMap")); 
			DelRegKey(hKeyRoot,	_T("Direct3D.antiAlias")); 
		}*/

	}
	// initialize the view
	m_driverHints->m_driver = driver;
	if (m_fullscreenOnStartup) {
		m_driverHints->m_useFullscreen = TRUE;
		m_driverHints->m_fullScreenMode = m_fullscreenModeOnStartup;
	}

	int ret = view->Initialize(GetSafeHwnd(),hDC,m_driverHints);

	if (ret<0) {
		TRACE("D3D : Problem during initialization \n ");
		initOk = FALSE;
		m_askHardware = TRUE;
		m_saveDriverSettings = FALSE;
		if (hKeyRoot) {	// reset settings
			SetRegKey(hKeyRoot,_T("Direct3D.askHardware"), m_askHardware );
			DelRegKey(hKeyRoot,	_T("Direct3D.driver")); 	
			DelRegKey(hKeyRoot,	_T("Direct3D.textureFiltering")); 
			DelRegKey(hKeyRoot,	_T("Direct3D.textureMipMap")); 
			DelRegKey(hKeyRoot,	_T("Direct3D.antiAlias")); 
		}

	} 
	else { 
		if (!view->device->IsHardwareDriver())
				SetProfile(_T("Direct3D.driverOk"),1);
		initOk = TRUE;
	}
	
	// full screen mode 
	if (mode.GetLength()>0) 
		view->device->SetDefaultFullscreenMode(mode);

	if (m_fullscreenModeOnStartup)//set in propexchange
		if (!view->device->SetDefaultFullscreenMode(m_fullscreenModeOnStartup)) {
			//AfxMessageBox("Warning : bad fullscreen mode "+m_fullscreenModeOnStartup);
			TRACE("Warning : bad fullscreen mode "+m_fullscreenModeOnStartup);
		}			;

		if (m_fullscreenOnStartup) {//set in propexchange
			setFullscreen(TRUE);
			m_NavFocus = TRUE;
			SetFocus(); // 22.01.2000 ??
		}

#endif

	theCache.writeCacheEnabled = TRUE;
	theCache.writeMediaCacheEnabled = TRUE; // Laurent - Jan.2000
	theCache.SetWriteCacheDirectory(GFile::cacheDirectory); 
	//hg theCache.SetWriteMediaCacheDirectory(GFile::cacheMediaDirectory);  // Laurent - Jan.2000
	//theCache.SetWriteMediaCacheDirectory(GFile::cacheDirectory+"\\UniversalMedia");  // hg ? 

	theCache.SetReadCacheDirectoryPath("cdrom:\\cache"); // set as default 
	theCache.lastDiskSpaceCheck = timeGetTime() +1000l*90l; //xx ; start check after 90 sec 

	// UMEL - NOW KNOWN AND REFERRED TO AS MEDIA IN NEW CODE PORTIONS
	CString umelPath;
	if (GetUmelPath(umelPath)) {
		theCache.SetUmelDirectoryPath(umelPath); 
	}


	// get other settings 
	if (hKeyRoot != NULL) {

		if (1) { // caching flags 
			if (GetRegKey(hKeyRoot,_T("Cache.verifyMode"),v)) {
				if (v >= GCACHE_ALWAYS && v <= CCACHE_NEVER)
					theCache.cacheMode = (GCacheMode) v;
			}
			if (GetRegKey(hKeyRoot,_T("Cache.readCache"),v)) {
				theCache.readCacheEnabled = v>0;
			}
			
			if (GetRegKey(hKeyRoot,_T("Cache.readMediaCache"),v)) {
				theCache.readMediaCacheEnabled = v>0;
			}

			if (GetRegKey(hKeyRoot,_T("Cache.writeCache"), v)) {
				theCache.writeCacheEnabled = v>0;
			}

			if (GetRegKey(hKeyRoot,_T("Cache.writeMediaCache"), v)) { // Laurent - Jan.2000
				theCache.writeMediaCacheEnabled = v>0;
			}

			val="";
			if (GetRegKey(hKeyRoot,_T("cache.directory"), val)) {
				if (val.GetLength()>0) {
					GFile::cacheDirectory = val;
					TRACE("Cache directory is : %s \n",(const char *)val);
					theCache.SetWriteCacheDirectory(val); 
				}

			} else { // store initial value 
				val = GFile::cacheDirectory;
				SetRegKey(hKeyRoot,_T("cache.directory"), val );
			}			



            val="";
			if (GetRegKey(hKeyRoot,_T("cache.mediaDirectory"), val)) { // Laurent - Jan.2000
				if (val.GetLength()>0) {
					//GFile::cacheMediaDirectory = val;
					TRACE(" Universal Media cache directory is : %s \n",(const char *)val);
					theCache.SetWriteMediaCacheDirectory(val); 
				}

			} else { // store initial value 
			
				//SetRegKey(hKeyRoot,_T("cache.mediaDirectory"), val );
				theCache.SetWriteMediaCacheDirectory(GFile::cacheDirectory+"UniversalMedia\\");  // hg ? 
		}



          	val="";
			if (GetRegKey(hKeyRoot,_T("Cache.readonlyDirectorys"), val)) {
					theCache.SetReadCacheDirectoryPath(val);
			}
			v=0;

			if (GetRegKey(hKeyRoot,_T("Cache.mediaDirectory"), val)) { // Laurent - Jan.2000
					theCache.SetWriteMediaCacheDirectory(val);
			}
			v=0;

			if (GetRegKey(hKeyRoot,_T("Cache.verifyDays"), v)) {
					if (v>=0) theCache.expirationDays = v;
			}
			v=0;
			if (GetRegKey(hKeyRoot,_T("Cache.minDiskSpaceFreeKB"), v) && (v>=1024)) {
					theCache.minSpaceFree = v*1024;
			}
			v=0;
			if (GetRegKey(hKeyRoot,_T("Cache.maxDiskSpaceUsedKB"), v) && (v>=1024)) {
					theCache.maxSpaceUsed = v*1024;
			}
		}

		if (initOk) {
#ifdef _D3D		
		if (GetRegKey(hKeyRoot,_T("Direct3D.dither"),v))
			view->SetDithering(v>0);

		if (GetRegKey(hKeyRoot,_T("Direct3D.textureFiltering"),v))
			view->SetTextureFiltering(v);

		if (GetRegKey(hKeyRoot,_T("Direct3D.textureMipMap"),v))
			view->SetTextureMipMap(v);

		// done in GModel.cpp at init time
		//if (GetRegKey(hKeyRoot,_T("Direct3D.useSoftwareLighting"),v))
		//	view->renderState.enableOwnLighting = (v>0); 

		if (GetRegKey(hKeyRoot,_T("Direct3D.antiAlias"),v))
			view->SetAntiAliasing(v>0); 
#endif
		if (GetRegKey(hKeyRoot,_T("General.exactCollision"),v))
			view->useRapid=v>0;


#ifdef _OGL
		if (GetRegKey(hKeyRoot,_T("OpenGL.dither"),v))
			view->SetDithering(!!v);

		if (GetRegKey(hKeyRoot,_T("OpenGL.textureFiltering"),v))
			view->SetTextureFiltering(!!v);

		if (GetRegKey(hKeyRoot,_T("OpenGL.textureMipMap"),v))
			view->SetTextureMipMap(v);

#endif
		}

		if (GetRegKey(hKeyRoot,_T("UI.mode"),v))
			m_uiMode = (UIMode) v;
		
		// if m_navPanelOn has already been deactivated by the property
		// settings, the flag may not be overridden by the registry value
		if (GetRegKey(hKeyRoot,_T("General.panel"), v)) {
			//view->m_navPanelOn_Reg = v;
			//view->m_navPanelOn = view->m_navPanelOn_Reg & view->m_navPanelOn_Prop & view->m_navPanelOn_NavInf;
			view->setNavigationPanelProp(m_navPanelOn_Embed && v);
		}

		if (GetRegKey(hKeyRoot,_T("General.speedFac"), v))
			m_cameraSpeedFac = v;
		//else m_cameraSpeedFac = 10;

		if (GetRegKey(hKeyRoot,_T("General.fullSpeed"), v))
			m_fullSpeed = v;

		if (GetRegKey(hKeyRoot,_T("General.verbose"), v)) {
			m_verbose = v;
			if (m_reporter) m_reporter->verbose = v;

			if (m_verbose) {
 				view->inlineWwwFlags |= WWW_REPORT_ERRORS;
 			}
 			else 
 				view->inlineWwwFlags &= ~WWW_REPORT_ERRORS;
		}

		if (GetRegKey(hKeyRoot,_T("Nurbs.targetFps"), v) && (v>0))
			view->SetTargetFrameRate(v);

		if (GetRegKey(hKeyRoot,_T("Nurbs.tessellationMode"), v) && (v>=0))
			view->nurbsTessellationMode = v;
		
		if (GetRegKey(hKeyRoot,_T("Nurbs.tessellationScale"), vf) && (v>=0.001))
			view->nurbsTessellationScale = v;

		if (GetRegKey(hKeyRoot,_T("Performance.visbilityFactor"), vf) && (vf>=0.01)) // 4.3
			view->lodScale = vf;

		if (GetRegKey(hKeyRoot,_T("Avatar.url"), val)) {
			view->myAvatarURL.set(val);
		}

		if (GetRegKey(hKeyRoot,_T("Avatar.name"), val)) {
				view->myAvatarName.set(val);
		}
	

		if (GetRegKey(hKeyRoot,_T("Navigation.anchorSingleClick"),v))
			m_anchorSingleClick = v;

		if (GetRegKey(hKeyRoot,_T("Sound.enableSound"),v))
			view->useSound = !!v;

		if (GetRegKey(hKeyRoot,_T("Sound.numSounds"),v))
			view->maxNumSounds = v;
		
		if (GetRegKey(hKeyRoot,_T("Sound.useRsx"),v))
			view->useRSXSound =  v != 0;

		if (GetRegKey(hKeyRoot,_T("Sound.quality"),v))
			view->soundQuality = v;

		if (GetRegKey(hKeyRoot,_T("Sound.doppler"),v))
			view->soundDoppler = v != 0;

		::RegCloseKey(hKeyRoot);
	}
	else {
			// no registry settings yet, store at lease cache directory
			HKEY hKeyRoot;
			// SAVE settings to registry 
			
			if (::RegCreateKey(HKEY_CURRENT_USER, USER_KEY, &hKeyRoot) == ERROR_SUCCESS) {

				CString val;
				val = GFile::cacheDirectory;
				SetRegKey(hKeyRoot,_T("cache.directory"), val );
				::RegCloseKey(hKeyRoot);

			}
	}		



//--------------------------------------------

	m_cameraSpeed = view->viewStepSpeed ;		// 1 / 16
	view->enableStayOnGround = TRUE;

//------------------------------------------------

	if (view->m_navPanelOn)
		view->NavigationPanelCreate(0);

    StartTimer();
	InternalSetReadyState(READYSTATE_INTERACTIVE);
	HWND fg=::GetForegroundWindow();
	m_dConsole = new DConsole(this);
	m_dConsole->Create(DConsole::IDD,this); // hg this new 11.0.5.98
	//m_dConsole->ShowWindow(SW_SHOWNA);
	if (::GetForegroundWindow() != fg) { // Thilo : reset foreground 
			::SetForegroundWindow(fg);
	}

#ifdef _D3D
	BOOL startDriverWasOk = TRUE;

	if (driver.GetLength() >0)
		startDriverWasOk = (driver == view->device->ThisDriver.Name);

	if (startDriverWasOk && m_askHardware  /* && !view->device->IsHardwareDriver()*/ 
		&& view->device->HasHardwareDriver(D3DAPP_ONLYWINDOW)) 
	{

		m_dAskHardware = new DAskHardware(this);
		m_dAskHardware->m_dontAskAgain = !m_askHardware;
		if ( (view->device->WindowsDisplay.bpp <15) || (view->device->WindowsDisplay.bpp >16)) {
			m_dAskHardware->m_info = "32000 Color Display setting recommended ";
		}
		m_dAskHardware->m_info += view->device->deviceId.szDescription;

		m_dAskHardware->Create(DAskHardware::IDD);
		m_dAskHardware->ShowWindow(SW_SHOWNORMAL);
	}
#endif

	if (m_useRenderThread) { // use rendering thread
		if (!StartRenderThread()) 
			m_useRenderThread = FALSE;
	}
	
	return TRUE;
}

// save D3D driver setting to registry
void CGLViewCtrlCtrl::SaveDriverSettings()
{
	if (!view) return;

#ifdef _D3D
	HKEY hKeyRoot;

    // SAVE settings to registry 
			
	if (::RegCreateKey(HKEY_CURRENT_USER, USER_KEY, &hKeyRoot) == ERROR_SUCCESS) {
			CString val;
			val = view->device->Driver[view->device->CurrDriver].Name;
			SetRegKey(hKeyRoot,_T("Direct3D.driver"), val);
			SetRegKey(hKeyRoot,_T("Direct3D.askHardware"), m_askHardware );
			::RegCloseKey(hKeyRoot);
	}

#endif

	m_saveDriverSettings = FALSE; // turn off flag 

}

//
// OnHardwareCheck -   Hardware check dialog done
//
LRESULT CGLViewCtrlCtrl::OnHardwareCheck(WPARAM wParam, LPARAM lParam)
{
	if (!m_dAskHardware) return 0;

	m_askHardware = !m_dAskHardware->m_dontAskAgain;
	BOOL useHardware = m_dAskHardware->m_useHardware;
	m_dAskHardware->DestroyWindow(); delete m_dAskHardware; m_dAskHardware = NULL; 

#ifdef _D3D

	if (useHardware) {

		if (view->device->IsHardwareDriver()) {
			
			if (!m_askHardware) m_saveDriverSettings = TRUE;

		} else {
			//view->NavigationPanelDestroy();
			if (view->device->ChangeToHardwareDriver(D3DAPP_ONLYWINDOW)) {
				//if (view->m_navPanelOn) view->NavigationPanelCreate(0);

				SetProfile(_T("Direct3D.driverOk"),0);				
				m_checkFrames=0; // redo checking 

				Redraw();

				if (!m_askHardware) 
					m_saveDriverSettings = TRUE;
			}
		}
	} else {

		if (view->device->IsHardwareDriver()) {
			view->device->bOnlySystemMemory = GetSysMemFlag();
			if (!view->device->ChangeDriver(view->device->SoftwareDriver,D3DAPP_ONLYWINDOW)) {
					view->ReportD3DError();
			}
			else  {
				m_saveDriverSettings = TRUE; // save driver anyway
				Redraw();
			}		

		}
		if (!m_askHardware) 
			m_saveDriverSettings = TRUE;
	}

	// SaveDriverSettings(); // done at exit if needed

#endif
	return 0;
}

void CGLViewCtrlCtrl::CloseAllDialogs() 
{
	if (m_dAskHardware) { m_dAskHardware->DestroyWindow(); delete m_dAskHardware; m_dAskHardware = NULL; }
	if (m_dConsole)	
		if ( (m_dConsole->GetSafeHwnd() != NULL) && m_dConsole->IsWindowVisible( )) 
			m_dConsole->ShowWindow(SW_HIDE);
	
	if (m_dPad) { m_dPad->DestroyWindow(); delete m_dPad; m_dPad = NULL; }

}

// implemented OnPaint directly, 
// instead of OnDraw which does more things in COleControl::OnDraw
void CGLViewCtrlCtrl::OnPaint(/* CDC* pDC */)
{

	BOOL bWait= FALSE;
	GFPSetControl(); // mask FP exceptions 

	if (!m_initialized) // not initialized yet, do it now !
    {
		HWND hwnd = GetSafeHwnd();
		if (!m_pDC) m_pDC = GetDC(); 

		if (!Initialize(m_pDC->m_hDC)) 
			return;
		//ReleaseDC(m_pDC); at the end released
		
		if (m_initialUrl.GetLength()>0)	// what we have to loade
			SetUrl(m_initialUrl);
		else InternalSetReadyState(READYSTATE_COMPLETE);
		
		if (m_initialAvatarUrl.GetLength()>0) {	// 
			setMyAvatarURL(m_initialAvatarUrl);
		}
		if (m_initialThirdPersonView)
			setThirdPersonView(m_initialThirdPersonView);

	}

	// validate window for OnPaint()
	ValidateRect(NULL);

	if (m_useRenderThread) {
		m_mustRedraw = TRUE;
		return;
	} 


	if (!m_initialized || view == NULL) {
		// still in wrong state ?
		ASSERT(0);
		return;
	}
	if (view->Locked()) { 
		// still locked, in wrong state ?
		//ASSERT(0); // diagnostic only 
		// this happens sometimes at exit, do we get a Paint while in some state ?
		return; 
	}


#ifdef _D3D
	// reset position on screen, necessary for Direct3D because OnMove not called
	view->OnMove(0,0); 

#endif
//	 TRACE("\nOnDraw() %p\n",this);


	if (view->currentTraversal != NULL) 
    {
		TRACE("OnDraw: Should not happen\n");
		ASSERT(0);
		return; // recursive entry ==> bad idea
	}

	if (view->updateIsComplex) 
    {
		bWait = TRUE;
		BeginWaitCursor();
	}
	view->Lock(); 


		// draw it
		view->DrawScene(m_clientRect);


	// enable texture mapping after first update 
	if (view->firstUpdate) 
    {
	   view->firstUpdate = 0;
 	   if (view->initialTextureMode >0) 
       {
	     if (view->GetTexturing() == 0) 
         {
		     view->SetTexturing(1);
			 Redraw();
		 }
	  }
	}
	if (bWait) EndWaitCursor();

	m_endTime= timeGetTime();	// time last rendering cycle stopped

	view->Unlock();


	if (m_displayFrameRate) {
		m_frames ++;

		if ( m_frames == 10) {
			CString msg;
			if (m_displayFrameStats) {
				
				//msg.Format("%.2f fps  (%.0d ms) Triangles #%i Triangles/s #%f", ( 10000.0f /  (float) (m_endTime-m_startTime)  ), view->lastFrameTime, view->frameStats.tris, 10000.f*(float)view->frameStats.tris/(float) (m_endTime-m_startTime));

				int tris=0,prims=0,times=0;
				
				for(int i=0; i<NUM_LAST_FRAME_TIMES;i++) { // compute average
					times+=view->lastFrameTimes[i];
					tris+=view->lastFrameTris[i];
					prims+=view->lastFramePrims[i];
				}

				float dt = 0.001 * times / (float) NUM_LAST_FRAME_TIMES;

				float tr = tris / (float) NUM_LAST_FRAME_TIMES;
				float pr = prims / (float) NUM_LAST_FRAME_TIMES;
				if (dt>0.0f)
				  msg.Format("%.2f fps  (%.0d ms) Triangles #%d Triangles/s #%d Primitives #%d", ( 10000.0f /  (float) (m_endTime-m_startTime)  ), view->lastFrameTime, (int)tr,(int)( tr/dt),(int)pr);



			} else
				msg.Format("%.2f frames per sec  (%.0d ms) \n",  ( 10000.0f /  (float) (m_endTime-m_startTime)  ), view->lastFrameTime);

			m_frames = 0;
			m_startTime= m_endTime; 
			Message(msg,PROGRESS_MESSAGE);	
		}
	}

#ifdef _D3D		
		// set flag that driver seems working ok 
		//one frame is already rendered in the startup logo!!!!!!!!
		if (m_checkFrames == 1) {
			if (view->device->IsHardwareDriver())
				SetProfile(_T("Direct3D.driverOk"),1);
		}
#endif

		
	m_checkFrames++;



#if 1
	if (m_nFlags &  WF_MODALLOOP ) { // new 07.07.99 hg 
		TRACE("In modal loop \n");
	}
	//	if (m_NavFocus) 
	/* if fast rendering, post a WM_TIMER message, under WINDOWS 95 timer resolution is limited  */
	if  (m_fullSpeed && !m_menuActive && (updateLock == 0)  && (view->lastFrameTime < 70) && (mainLoader == NULL) /* && (	m_lReadyState == READYSTATE_COMPLETE) */) {
		MSG msg;
		// DWORD GetQueueStatus
        if (m_NavTimerRunning  || !PeekMessage(&msg,NULL,0,0,PM_NOREMOVE)) { // check if any message is pending
			PostMessage(WM_TIMER,0,0); // call me again asap
			::Sleep(0);
		} else {
			// TRACE("Fast render : message pending %d %x \n",msg.message,msg.message);

		}
	
	}
#endif

}


/////////////////////////////////////////////////////////////////////////////
// CGLViewCtrlCtrl::OnDraw - Drawing function

void CGLViewCtrlCtrl::OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{

}


/////////////////////////////////////////////////////////////////////////////
// CGLViewCtrlCtrl::DoPropExchange - Persistence support

void CGLViewCtrlCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);


//	PX_DataPath(pPX, _T("WORLD"), m_world);


	// TODO: Call PX_ functions for each persistent custom property.
	CString url,media,dash, fullscreen,fullscreenMode,avatarUrl,avatarMode;
	ULONG	timerInterval=0;

	if (pPX->IsLoading()) {
		PX_String(pPX, "URL", url);
		PX_String(pPX, "Media", media);
		PX_String(pPX, "VRML-DASHBOARD", dash);
		
		PX_String(pPX, "FULLSCREEN", fullscreen); // 4.3
		
		PX_String(pPX, "FULLSCREEN-MODE", fullscreenMode); // 4.3
	
		PX_ULong(pPX,  "TIMER-INTERVAL", timerInterval, 0); //4.3

		PX_String(pPX, "AVATAR-URL", avatarUrl);  //4.3
		PX_String(pPX, "AVATAR-DISPLAY", avatarMode);  //4.3

		//
		PX_Bool(pPX, "FORCE-HW", m_driverHints->m_useHW);  //4.3
		PX_Bool(pPX, "FORCE-RGB", m_driverHints->m_useRgb);  //4.3
		PX_Bool(pPX, "HW-PROBLEM-CHECK", m_forceHwProblemCheck);  //4.3
		PX_Bool(pPX, "HIDE-CURSOR", m_hideCursor);  //4.3

		// PX_String(pPX, "BACKGROUND-COLOR", backgroundColor); // 4.3
		// PX_String(pPX, "BACKGROUND-IMAGE", backgroundImage); // 4.3

	
		if ( (dash.Compare("0")==0) || (dash.CompareNoCase("false")==0)) {
			if (view)
				view->setNavigationPanelProp(FALSE);
			m_navPanelOn_Embed = FALSE;			
		}


		if ( (fullscreen.Compare("1")==0) || (fullscreen.CompareNoCase("true")==0)) 
		{
			//can not set mode here because view may not exist
			m_fullscreenOnStartup = TRUE;
			// m_hideCursor = TRUE;
		}

		if (fullscreenMode.GetLength()>0) {
			m_fullscreenModeOnStartup = fullscreenMode;
		}

		if (url.GetLength() == 0) 
			PX_String(pPX, "DATA", url);

		if (url.GetLength() == 0) 
			PX_String(pPX, "WORLD", url);
		
		// see KB mimetype example (used from EMBED etc. ) 
		if (url.GetLength() == 0) 
			PX_String(pPX, "SRC", url);


		if (url.GetLength()>0) {
			if (view == NULL) {
				m_initialUrl = url;
			} 
			else SetUrl(url);
		}  else {

		}
		if (avatarUrl.GetLength()>0) {
			setMyAvatarURL(avatarUrl);
		}

		if (timerInterval>0) {
			if (m_timerRunning)
				setTimerInterval(timerInterval);
			else m_timerInterval = timerInterval;

		}

		if ( (avatarMode.Compare("1")==0) || (avatarMode.CompareNoCase("true")==0)) 
		{
			setThirdPersonView(TRUE);
		}


	} else 
    {
		if (view == NULL)
			url = m_initialUrl;
		else
			url = view->GetUrl();

		PX_String(pPX, "URL", url);
	}
    if (pPX->GetVersion() >= MAKELONG(0, 2)) 
    {
	}
   

}


/////////////////////////////////////////////////////////////////////////////
// CGLViewCtrlCtrl::OnResetState - Reset control to default state

void CGLViewCtrlCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange
	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CGLViewCtrlCtrl::AboutBox - Display an "About" box to the user

void CGLViewCtrlCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_GLVIEWCTRL);
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	//dlgAbout.DoModal();
	DoThreadModal(&dlgAbout);

}


/////////////////////////////////////////////////////////////////////////////
// CGLViewCtrlCtrl message handlers

BOOL CGLViewCtrlCtrl::OnSetExtent(LPSIZEL lpSizeL) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return COleControl::OnSetExtent(lpSizeL);
}

BOOL CGLViewCtrlCtrl::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style = cs.style | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
/*	MFC adds Parent DC for OCX wnd ??
	// hg test 07/07/99

	if (cs.style |= CS_PARENTDC) {
		cs.style &= ~CS_PARENTDC; 
	}
	// cs.style |= CS_OWNDC;
*/

	if (cs.lpszClass == NULL) 
    {   // see wordpad for more detailed example 
		// Register New Class for Private DC
		// hg test 07/07/99 // alreday class in MFC OCX case ??
		// cs.lpszClass = AfxRegisterWndClass(CS_OWNDC|CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW);
	}
	
	return COleControl::PreCreateWindow(cs);
}
//
// Navigation
//


/*
typedef struct {
	int i;
	char *s;
	} IS;
*/

 const IS MoveModes[] = {
			{-1,"<Invalid>"},
            {SELECT_MODE,"select"},
            {NULL_MODE,"none"}, 
            {DOLLY_XY_MODE,"slide"},
            {DOLLY_XY_MODE,"dolly"},
			{ORBIT_MODE,"examine"},
			{ORBIT_MODE,"orbit"},
			{ROTATE_MODE,"rotate"},
			{PAN_MODE,"pan"},
			{ZOOM_MODE,"zoom"},
			{ROLL_MODE,"roll"},
			
            {WALK_MODE,"walk"},
			{WALK_MODE,"plane"},
			
            {FLY_MODE,"fly"},
            {FLY_MODE,"fly_plane"},

			{WALK_2CLICK_MODE,"walk2"},
			{ZOOM_LENS_MODE,"lens"},
			{JUMP_TO_MODE,"lookat"},
			{JUMP_TO_MODE,"jump"},

			{0,NULL},
			};

const char * MoveModeLookup(int v)
{ for (int i=0; MoveModes[i].s != NULL; i++)
      if (MoveModes[i].i == v) return(MoveModes[i].s);
  return 0;
}

int MoveModeLookup(const char *v)
{ for (int i=0; MoveModes[i].s != NULL; i++)
      if (streq(MoveModes[i].s,v)) return(MoveModes[i].i);
  return -1;
}



// set the camera navigation mode 
int CGLViewCtrlCtrl::SetCameraMode(int newMode)
{
	m_currentCameraMode = newMode;
	m_lButtonMode = newMode;
	
	SetCollisionState(FALSE);
	// reset follow mode if scene or camera mode changes
	NavResetFollowObject();


/*	if (newMode == NULL_MODE) { // no panel allowed 
		if (view->m_navPanelOn) {
			view->NavigationPanelDestroy();
		}
		view->m_navPanelOn = view->m_navPanelOn_NavInf = FALSE;
	}
	else {
		//show panel 
		if ((view->m_navPanelOn_Prop && view->m_navPanelOn_Reg) 
			&& !view->panel) 
		{
			view->NavigationPanelCreate(0);
			view->m_navPanelOn_NavInf = TRUE;
		}
	}*/
	if (newMode == NULL_MODE) { // no panel allowed 
		if (view->m_navPanelOn) {
			view->NavigationPanelDestroy();
		}
		view->setNavigationPanelNavInf(FALSE);
	}
	else {
		//show panel 
		view->setNavigationPanelNavInf(TRUE);
		if (view->m_navPanelOn && !view->panel)
			view->NavigationPanelCreate(0);
	}

	return(m_currentCameraMode);
}

// get the camera navigation mode 
int CGLViewCtrlCtrl::GetCameraMode() 
{
   return(m_currentCameraMode);
}

// set the camera navigation mode by string 
int CGLViewCtrlCtrl::SetCameraMode(const char *mode_)
{

	CString mode(mode_);

    int newMode = MoveModeLookup(mode);

	if (newMode<0) { // try lower case 
		mode.MakeLower();
		newMode = MoveModeLookup(mode);
	}

    int i = mode.Find("_restricted");
	if (i>0) {
		view->allowAnyNavigation = gfalse;
		mode = mode.Mid(0,i-1);
		newMode = MoveModeLookup(mode);
	}
	


	if (streq(mode,"any")) {
		view->stayOnGround = TRUE;
		newMode = WALK_MODE;
		view->allowAnyNavigation = gtrue;
	}
	if (streq(mode,"fly")) { // VRML FLY mode 
		view->stayOnGround = FALSE;
		newMode = WALK_MODE;
	}
	else if (newMode == WALK_MODE) {
		view->stayOnGround = TRUE;
	}

	
    if (newMode>=0) return SetCameraMode(newMode);
	else { 
		TRACE("Bad camera mode %s \n",mode); 
		return(0);
	}
}

// get the camera navigation mode by string for mode 
const char* CGLViewCtrlCtrl::GetCameraModeString(int mode)
{
    if (mode<0) mode = m_currentCameraMode;
    return(MoveModeLookup(mode));
}

// get the current camera navigation mode by string
const char* CGLViewCtrlCtrl::GetCameraModeString()
{
    return(MoveModeLookup(m_currentCameraMode));
}


// reset the navigation parameters 
void CGLViewCtrlCtrl::ResetMove()
{
	m_currentMode = NULL_MODE;
	m_lButtonMode = WALK_MODE;
	m_moveTimerId = 0;
	m_moveRepeat = TRUE;
	m_moveRepeatStartTime = 0.3;   // in secondes 
	m_moveRepeatCycleTime = 0.02f; 
	m_sensorTest = TRUE;
	m_cameraRelative = TRUE;
	fly_state = 0;

}

// Helpers

// convert an offset to a factor  
#define OFS2FAC(ofs) (((ofs)<0.0) ? 1.0 / (-(ofs)+1.0) : (ofs)+1.0)

// square 
#define SQ1(F) (((F) < 0.0) ? - ((F)*(F)) : (F)*(F))

#define SQ(F) ((F))





CPoint operator+(CPoint a,CPoint b) { return CPoint(a.x+b.x,a.y+b.y);}
CPoint operator-(CPoint a,CPoint b) { return CPoint(a.x-b.x,a.y-b.y);}


void CGLViewCtrlCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{	// not called in IE 3.0 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	if (view) {
		ViewLock viewLock(view); 
		view->SetTheTime(); // set global VRML eventTime
		if (view->triggerKeyboardEvent(GEV_keypress,"keypress",nChar,nFlags))
			return;
	}

	
	COleControl::OnChar(nChar, nRepCnt, nFlags);
}

 
void CGLViewCtrlCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{


	if (!m_initialized) 
	{
		COleControl::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}
	if (view) {
		ViewLock viewLock(view); 
		view->SetTheTime(); // set global VRML eventTime
		if (view->triggerKeyboardEvent(GEV_keydown,"keydown",nChar,nFlags))
			return;
	}


	m_NavNotActive = FALSE;


	// if no repeat key
	if (!(nFlags & 0x4000)) 
	// check status of all navigation keys
		NavCheckKeyState(FALSE);
	   
	switch (nChar) {
	// **** ASCII Keys now moved to OnChar( ) 

	case VK_ESCAPE : 
			if (view->GetFullScreen()) 
				view->SetFullScreen(0);
			else if (view->AnimatingCamera()) 
				view->AnimateCameraStop();
			// if any navigation interpolator active then stop
			else if (m_NavIPActive)
				m_NavIPActive = 0;
			else if (m_NavFollowObject)
			{
				NavResetFollowObject();
			}			
			else
				OnCameraReset();
			return;
			   
			break;
	case VK_BACK :
				NavStart();
			break;
	case VK_SPACE : // ==> Follow in checkKeyState 
			/* if (m_NavFollowObject)
			{
				NavResetFollowObject();
			}
			*/
			break;
 
	case VK_ADD:
	case VK_NEXT: 		// 	22	PAGE DOWN key 
				if (view->allowAnyNavigation) OnCameraViewpointNext();
				break;

	case VK_SUBTRACT:
	case VK_PRIOR: 		// PAGE UP key 
				if (view->allowAnyNavigation) OnCameraViewpointPrev();
				break;


	case VK_END: 		// 	23	END key 

			if (view->allowAnyNavigation) 
				view->BindViewpoint(NULL); // unbind viewpoint, (camera is free from animation)

			if ( (view->allowAnyNavigation) && view->AnimatingCamera()) 
				view->AnimateCameraStop();
			// if any navigation interpolator active then stop
			else if (m_NavIPActive)
				m_NavIPActive = 0;
			else if (m_NavFollowObject)
			{
				NavResetFollowObject();
			}			

			break;

	case VK_HOME: 		// 	24	HOME key 
			// rebind current viewpoint
			if (view->allowAnyNavigation) 
		    if (view->viewpointIndex >=0 &&  view->viewpointIndex < view->sceneInfo->cameras.Length()) 
		        view->SetCamera(view->sceneInfo->cameras[view->viewpointIndex],&view->sceneInfo->cameraTransforms[view->viewpointIndex],0);

			break;
	

	case VK_LEFT: 		// 	25	LEFT ARROW key 
	case VK_UP: 		// 	26	UP ARROW key 
	case VK_RIGHT: 		// 	27	RIGHT ARROW key 
	case VK_DOWN: 		// 	28	DOWN ARROW key 
			// prevent key repeat
			if (view->allowAnyNavigation) 
			if (!(m_NavRunning & 0x2))
				NavStart();
			break;

//	case VK_NUMPAD8:
//			view->GetCamera().PanY(0.01,!GetCameraRelativeMode()); 
//			break;

	case VK_NUMPAD2:	// Test !!! let camera look along y-axis
			if (!view->allowAnyNavigation) break;

			view->GetCamera().target = view->GetCamera().position;
			view->GetCamera().target.y += m_NavTargetDist;

              //view->GetCamera().Dolly(0.0,0.0, -step*nRepCnt);
			  break;

	case VK_NUMPAD4:	// Test !!! let camera look along x-axis
			if (!view->allowAnyNavigation) break;
			view->GetCamera().target = view->GetCamera().position;
			view->GetCamera().target.x += m_NavTargetDist;
			 //   view->GetCamera().Pan(wstep*nRepCnt,0.0); 
			  break;

	case VK_NUMPAD6:	// Test !!! let camera look along z-axis
			if (!view->allowAnyNavigation) break;
			view->GetCamera().target = view->GetCamera().position;
			view->GetCamera().target.z += m_NavTargetDist;
		//	    view->GetCamera().Pan(-wstep*nRepCnt,0.0); 
			  break;
#if 0 // hg
	case VK_NUMPAD7:
    case (UINT)'7' :  // left forward 
		//	  view->GetCamera().Pan(wstep*nRepCnt,0.0); 
         //     view->GetCamera().Dolly(0.0,0.0, step*nRepCnt);
			  break;

	case VK_NUMPAD9:
    case (UINT)'9' :  // right forward
		//	  view->GetCamera().Pan(-wstep*nRepCnt,0.0); 
         //     view->GetCamera().Dolly(0.0,0.0, step*nRepCnt);
			  break;
    
	case VK_NUMPAD1:
    case (UINT)'1' :  // left back 
		//	  view->GetCamera().Pan(-3.0*wstep*nRepCnt,0.0); 
        //      view->GetCamera().Dolly(0.0,0.0, -step*nRepCnt);
			  break;

	case VK_NUMPAD3:
    case (UINT)'3' :  // right back 
		//	  view->GetCamera().Pan(3.0*wstep*nRepCnt,0.0); 
          //    view->GetCamera().Dolly(0.0,0.0, -step*nRepCnt);
			  break;

#endif

    case VK_F9 :
				OnSettingsPreferences();
                break;
    case VK_F5 :
				if (m_uiMode <=	UI_SIMPLE) break;
				view->ToggleFullScreen();
				m_hideCursor = FALSE;
				Redraw();
                break;
    
	case VK_F6 :
				if (m_uiMode <=	UI_SIMPLE) break;

#ifdef _D3D
			if (view->device->NumDevices>1) { // secondary driver 
				int oldDriver = view->device->CurrDevice;
				int newDriver = oldDriver +1;
				if (newDriver >= view->device->NumDevices)
					newDriver = 0;
				if (!view->device->ChangeDevice(newDriver,(newDriver == view->device->PrimaryDevice) ? D3DAPP_ONLYWINDOW:0)) {
						view->ReportD3DError();
					}
				}
#endif
				break;
    case VK_F7 :
				m_displayFrameRate = !m_displayFrameRate;
				m_displayFrameStats = FALSE;
				Message(CString("Frame Rate display  is now ") + ((m_displayFrameRate) ? "On" : "Off") );
				if (!m_displayFrameRate ) m_frames = 0;
				break;
    case VK_F8 :
			{
				m_displayFrameStats = !m_displayFrameStats;
				m_displayFrameRate =  m_displayFrameStats;
				Message(CString("Frame Stats display  is now ") + ((m_displayFrameStats) ? "On" : "Off") );
				if (!m_displayFrameRate ) m_frames = 0;
			}
				break;
	case VK_F12 :
				if (m_uiMode <=	UI_SIMPLE) break;
				view->ToggleFullScreen();
				m_hideCursor = FALSE;
				Redraw();
                break;

	// problem in IE 3.0 OnChar is *not* called 
    case (UINT)'?' : 
				OnSettingsPreferences();
                break;

    case (UINT)'=' :
			m_displayFrameRate = !m_displayFrameRate;
			Message(CString("Frame Rate display  is now ") + ((m_displayFrameRate) ? "On" : "Off") );
            if (!m_displayFrameRate ) m_frames = 0;
			break;


    case (UINT)'+' :
    case (UINT)'*' :
			if (!view->allowAnyNavigation) break;
			m_cameraSpeed = view->viewStepSpeed * 1.5;
			break;

    case (UINT) '-' :
    case (UINT) '/' :
			if (!view->allowAnyNavigation) break;
			m_cameraSpeed = view->viewStepSpeed / 1.5;
			if (m_cameraSpeed <= 0.0001) m_cameraSpeed = 0.0001;
			break;

	// third person view test : 
    case (UINT)'1' : view->followDolly.setz(view->followDolly.z() - 0.2); view->UpdatePhysicalCamera(gtrue); Redraw(); break;
    case (UINT)'2' : view->followDolly.setz(view->followDolly.z() + 0.2); view->UpdatePhysicalCamera(gtrue); Redraw(); break;
    case (UINT)'3' : view->followDolly.sety(view->followDolly.y() - 0.2); view->UpdatePhysicalCamera(gtrue); Redraw(); break;
    case (UINT)'4' : view->followDolly.sety(view->followDolly.y() + 0.2); view->UpdatePhysicalCamera(gtrue); Redraw(); break;

    case (UINT)'5' : view->followPan.sety(view->followPan.y() - 0.2); view->UpdatePhysicalCamera(gtrue); Redraw(); break;
    case (UINT)'6' : view->followPan.sety(view->followPan.y() + 0.2); view->UpdatePhysicalCamera(gtrue); Redraw(); break;
    case (UINT)'7' : view->followPan.setx(view->followPan.x() - 0.2); view->UpdatePhysicalCamera(gtrue); Redraw(); break;
    case (UINT)'8' : view->followPan.setx(view->followPan.x() + 0.2); view->UpdatePhysicalCamera(gtrue); Redraw(); break;

    case (UINT)'9' : view->followOrbit.setx(view->followOrbit.x() - 0.2); view->UpdatePhysicalCamera(gtrue); Redraw(); break;
    case (UINT)'0' : view->followOrbit.setx(view->followOrbit.x() + 0.2); view->UpdatePhysicalCamera(gtrue); Redraw(); break;
    case (UINT)'§' : view->followOrbit.sety(view->followOrbit.y() - 0.2); view->UpdatePhysicalCamera(gtrue); Redraw(); break;
    case (UINT)')' : view->followOrbit.sety(view->followOrbit.y() + 0.2); view->UpdatePhysicalCamera(gtrue); Redraw(); break;

    case (UINT)'A' : {
				if (m_uiMode <=	UI_SIMPLE) break;
				bool mode = !view->showAvatar;
				view->ShowAvatar(mode);
				view->SetThirdPersonView(mode);
				Message(CString("Avatar display is now ") + ((view->IsAvatarShown()) ? "On" : "Off") );
				Redraw();
				}
                break;
	// end third person test 

    case (UINT)'B' :
				if (!view->allowAnyNavigation) break;
				if (m_uiMode <=	UI_SIMPLE) break;
				view->BindBackground(NULL);
				Redraw();
                break;

    //case (UINT)'c' :
    case (UINT)'C' :	// Toggle collision mode 
				if (!view->allowAnyNavigation) break;
				if (m_uiMode <=	UI_SIMPLE) 
					break;
//                if (m_NavCNTLState && m_NavSpeedState)
					OnCameraCollision();
                break;
    case (UINT)'D' : // Coordinate display on/off
			if (!view->allowAnyNavigation) break;
				if (m_uiMode <=	UI_SIMPLE) 
					break;
                if (m_NavCNTLState && m_NavSpeedState)
					if (m_NavCoordDspl)
						m_NavCoordDspl = FALSE;
					else
						m_NavCoordDspl = TRUE;
				break;
			  
    case (UINT)'F' :
				if (!view->allowAnyNavigation) break;
				if (m_uiMode <=	UI_SIMPLE) break;
                if (m_NavCNTLState && m_NavSpeedState)
				{	
						Message(CString("Viewer mode is now FLY"));
						OnCameraFly();
				}
                break;

    //case (UINT)'g' :
    case (UINT)'G' :
				if (!view->allowAnyNavigation) break;
				if (m_uiMode <=	UI_SIMPLE) break;
				OnSettingsStayOnGround();
				break;

    //case (UINT)'j' :
    case (UINT)'J' :
				if (!view->allowAnyNavigation) break;
			//	if (m_uiMode <=	UI_SIMPLE) break;
            //   OnCameraJump();
			//	Message(CString("Viewer mode is now ") + GetCameraModeString(GetCameraMode()));
                break;
//#ifdef _DEBUG
			case (UINT)'M' :
				{
				MEMORYSTATUS stat;
				GReporter *reporter = GetReporter();

				GlobalMemoryStatus (&stat);

				SYSTEM_INFO si;
				GetSystemInfo(&si);

				#define DIV 1024
				// #define DIV 1

				char *divisor = "K";

				// char *divisor = "";

				char buf[512];
				// Handle the width of the field in which to print numbers this way to
				// make changes easier. The asterisk in the print format specifier
				// "%*ld" takes an int from the argument list, and uses it to pad and
				// right-justify the number being formatted.
				#define WIDTH 7

				  TRACE ("The MemoryStatus structure is %ld bytes long.\n", stat.dwLength);
				  TRACE ("It should be %d.\n", sizeof (stat));
				  TRACE ("%ld percent of memory is in use.\n",
						  stat.dwMemoryLoad);

				  sprintf(buf,"Memory Status : %ld used",stat.dwMemoryLoad);	
				  if (reporter) reporter->Trace(buf);

				  TRACE ("There are %*ld total %sbytes of physical memory.\n",
						  WIDTH, stat.dwTotalPhys/DIV, divisor);
				  TRACE ("There are %*ld free %sbytes of physical memory.\n",
						  WIDTH, stat.dwAvailPhys/DIV, divisor);

				  sprintf(buf,"%*ld free %sB/%*ld total %sB physical memory",WIDTH, stat.dwAvailPhys/DIV, divisor, WIDTH, stat.dwTotalPhys/DIV, divisor);	
				  if (reporter) reporter->Trace(buf);


				  TRACE ("There are %*ld total %sbytes of paging file.\n",
						  WIDTH, stat.dwTotalPageFile/DIV, divisor);
				  TRACE ("There are %*ld free %sbytes of paging file.\n",
						  WIDTH, stat.dwAvailPageFile/DIV, divisor);
				  
				  sprintf(buf,"%*ld free %sB  / %*ld total %sBs paging file ",WIDTH, stat.dwAvailPageFile/DIV, divisor,
						WIDTH, stat.dwTotalPageFile/DIV, divisor);
				  if (reporter) reporter->Trace(buf);

				  TRACE ("There are %*ld total %sbytes of virtual memory.\n",
						  WIDTH, stat.dwTotalVirtual/DIV, divisor);
				  TRACE ("There are %*ld free %sbytes of virtual memory.\n",
						  WIDTH, stat.dwAvailVirtual/DIV, divisor);

				  sprintf(buf,"%*ld free %sB / %*ld total %sbytes of virtual memory", WIDTH, stat.dwAvailVirtual/DIV, divisor,WIDTH, stat.dwTotalVirtual/DIV, divisor);
				  if (reporter) reporter->Trace(buf);


				}				
				break;

//#endif
    case (UINT)'S' :
				if (!view->allowAnyNavigation) break;
				if (m_uiMode <=	UI_SIMPLE) 
					break;
				// toggle variable to prevent key repeat
                if (m_NavCNTLState && m_NavSpeedState)
				{	
					Message(CString("Viewer mode is now SLIDE"));
					OnCameraDollyXy();
				}
				break;

//    case (UINT)',' : GoBack(); break;//tv: auskommentiert auf request hin
//    case (UINT)'.' : GoForward(); break;
    case (UINT)'P' :
				if (!view->allowAnyNavigation) break;
				if (m_uiMode <=	UI_SIMPLE) break;
                if (m_NavCNTLState && m_NavSpeedState)
				{	
					Message(CString("Viewer mode is now PAN"));
					OnCameraPan();
				}
				break;

    case (UINT)'W' :
				if (!view->allowAnyNavigation) break;
				if (m_uiMode <=	UI_SIMPLE) break;
                if (m_NavCNTLState && m_NavSpeedState)
				{	
					Message(CString("Viewer mode is now WALK"));
					OnCameraWalk();
				}
				break;

    case (UINT)'O' :
    case (UINT)'E' :
				if (!view->allowAnyNavigation) break;
				if (m_uiMode <=	UI_SIMPLE) break;
                if (m_NavCNTLState && m_NavSpeedState)
				{	
					Message(CString("Viewer mode is now EXAMINE"));
					OnCameraOrbit();
				}
                break;
    

    case (UINT)'X' :
				if (!view->allowAnyNavigation) break;
				if (m_uiMode <=	UI_SIMPLE) break;
                if (m_NavCNTLState && m_NavSpeedState) {
					view->SetBspMode(!view->GetBspMode());
					Redraw();
					Message("BSP Mode toggled ");
				}
                break;
    case (UINT)'Y' :
				if (m_uiMode <=	UI_SIMPLE) break;
				view->SetTextureFiltering(!view->GetTextureFiltering());
				Redraw();
				Message("Texture filtering toggled ");
                break;
    case (UINT)'Z' :
				if (m_uiMode <=	UI_SIMPLE) break;
				OnViewPad();
                break;

    case (UINT)'R' :
				if (!view->allowAnyNavigation) break;
				if (m_uiMode <=	UI_SIMPLE) break;
                if (m_NavCNTLState && m_NavSpeedState)
				{	
					Message(CString("Viewer mode is now ROTATE"));
					OnCameraRotate();
				}
                break;
    case (UINT)'U' : // Straighten Up 
				if (!view->allowAnyNavigation) break;
				if (m_uiMode <=	UI_SIMPLE) 	break;
				{
	                NavStraightenUp();
				}
                break;
    case (UINT)'K' :

#ifdef _KATMAI			
				KatmaiEnable(!useKatmai); 
				Message(CString("Katmai code is now ") + ((useKatmai) ? "On" : "Off") );
				Redraw();

#endif
				break;
    case (UINT)'L' :  // save world in memory for debugging cache dir /blaxxuncc3d.wrl
				if (m_uiMode <=	UI_SIMPLE) 	break;
				{
					CString path;
					path = GFile::cacheDirectory;
					int l = path.GetLength();
					if (path[l-1] != '\\')
						path += '\\';
					path += _PROGRAM;
					path += ".wrl";
					Message(CString("Saving world to ")+path,PROGRESS_MESSAGE);
					saveWorld(path);
					Message(CString("World saved to ")+path,PROGRESS_MESSAGE);
				}
                break;

	default :
		COleControl::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
		break;
	}
}



void CGLViewCtrlCtrl::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{

	if (!m_initialized) 
	{
		COleControl::OnKeyUp(nChar, nRepCnt, nFlags);
		return;
	}

	if (view) {
		ViewLock viewLock(view); 
		view->SetTheTime(); // set global VRML eventTime
		if (view->triggerKeyboardEvent(GEV_keyup,"keyup",nChar,nFlags))
			return;
	}

	m_NavNotActive = FALSE;

	// check status of all navigation keys
	NavCheckKeyState(FALSE);
	
/*
	switch (nChar) 
			{
			case VK_LEFT: 		// 	25	LEFT ARROW key 
					m_NavLEFTState = FALSE;
					NavCheckCursor();
					break;
			case VK_UP: 		// 	26	UP ARROW key 
					m_NavUPState = FALSE;
					NavCheckCursor();
					break;
			case VK_RIGHT: 		// 	27	RIGHT ARROW key 
					m_NavRIGHTState = FALSE;
					NavCheckCursor();
					break;
			case VK_DOWN: 		// 	28	DOWN ARROW key 
					m_NavDOWNState = FALSE;
					NavCheckCursor();
					break;
			default: 
					break;
			}
*/


	COleControl::OnKeyUp(nChar, nRepCnt, nFlags);
}


// check if its an MFC Window , // new 12.06.99 HG 
LRESULT MySendMessage( HWND hWnd,
    UINT Msg,
    WPARAM wParam,
    LPARAM lParam)
{
	DWORD dwResult; 

	CWnd *observerCwnd=CWnd::FromHandlePermanent(hWnd);		
	if (observerCwnd) 
		return observerCwnd->SendMessage(Msg, wParam, lParam);
	//else return ::SendMessage(hWnd, Msg, wParam, lParam);
	else return ::SendMessageTimeout(hWnd, Msg, wParam, lParam,SMTO_ABORTIFHUNG,100,&dwResult); // not in win32s


}



/*

    // Retrieve the file.  If it is in the cache and up to date it will just return the cache 
    // if it is a file: URL it will just return the local file without making a cache copy, 
    // otherwise it will download the file and return the cached file
    if ( FAILED(URLDownloadToCacheFile(
        GetControllingUnknown(),                // control's IUnknown
        m_cstrFileName,                         // URL to download
        m_cstrCacheFileName.GetBuffer(MAX_PATH),// buffer to fill with name
        MAX_PATH,                               // buffer length
        0,                                      // reserved
        NULL )) )                               // no status information
    {
        AfxMessageBox("Cannot download file");
        m_cstrCacheFileName = "Not Found";
        return;
    }
*/

   
/*
HRESULT HlinkSimpleNavigateToString(
    LPCWSTR szTarget,	//String to be resolved into target's moniker
    LPCWSTR szLocation,	//Optional string representing location within target
    LPCWSTR szTargetFrameName,	//Optional string naming the target frame
    IUnknown * punk,	//IUnknown pointer to the initiating document or object
    IBindCtx * pbc,	//Bind context object
    IBindStatusCallback * pbsc,	//Bind status callback 
    DWORD grfHLNF,	//Navigation flags
    DWORD dwReserved	//Reserved for future use
   );	
 
*/
// SetMoniker
// GetMoniker 
HRESULT NavigateToUrl(LPUNKNOWN pUnk,const char *url, const char *location=NULL, const char *targetFrame=NULL,	DWORD flags = 0)
{
		HRESULT result;
		USES_CONVERSION;

#ifdef G_URLMON
		if (HlinkSimpleNavigateToStringH == NULL) {
			return S_FALSE;
		}
		result = HlinkSimpleNavigateToStringH(
			(url == NULL) ? NULL : T2COLE(url),
			(location == NULL) ? NULL :T2COLE(location) ,
			(targetFrame == NULL) ? NULL :T2COLE(targetFrame) ,
			pUnk, // if  NULL new Explorer window will be started 
			NULL,
			NULL,
			flags,
			0);
#else
		result = HlinkSimpleNavigateToString(
			(url == NULL) ? NULL : T2COLE(url),
			(location == NULL) ? NULL :T2COLE(location) ,
			(targetFrame == NULL) ? NULL :T2COLE(targetFrame) ,
			pUnk, // if  NULL new Explorer window will be started 
			NULL,
			NULL,
			flags,
			0);
#endif
		return(result);
}


// Navigate to url 
HRESULT CGLViewCtrlCtrl::NavigateToUrl(const char *url, const char *location, const char *targetFrame,DWORD flags)
{

//    CWaitCursor wait;
    USES_CONVERSION;   

    CString msg;


    EAI_TRACE("Navigating to %s \n",url);
    EAI_FLUSH();
    TRACE("Navigating to %s \n",url);

    if (!strncmp(url, "OnEvent", 7)) // call vbscript/javascript
    {
        const char *urlp = &url[7];

		CString s, 
		par1 = "", par2 = "";

		while(isspace(*urlp)) urlp++;

        s = urlp;
	
		int i1=-1;

		if (*urlp == '(')
			i1 = 0;
        int i2 = s.Find(',');
        int i3 = s.ReverseFind(')');


        if (i1 >= 0 && i1 < i2)
            par1 = s.Mid(i1+1, i2 - i1 -1);
		else 
            if (i1 >= 0 && i3>0)
				par1 = s.Mid(i1+1, i3 - i1 -1);

        if (i2 > 0 && i2 < i3)
            par2 = s.Mid(i2+1, i3 - i2 -1);
		par1.TrimLeft(); 
		par1.TrimRight();
		par2.TrimLeft(); 
		par2.TrimRight();
	    
        double t = view->getTime();

        if (view->observerFlags & GOBSERVE_WM_ANCHOR) // netscape
        {
            m_messageUrl.Format("javascript:OnEvent('%s','%s',%.16g)", par1, par2, t);		
            m_messageTargetFrame = targetFrame;        
	        MySendMessage(view->observerWnd, WM_USER + 501, (WPARAM) (const char*)m_messageUrl, (LPARAM) (const char*)m_messageTargetFrame);
	        //::PostMessage(view->observerWnd, WM_USER + 501, (WPARAM) (const char*)m_messageUrl, (LPARAM) (const char*)m_messageTargetFrame);
        }
        else // IE
        {
            FireOnEvent(par1, par2, t);
        }
		return S_OK;
    }

	if (targetFrame == NULL) {
		Translate(_T("Jumping to"),msg);
		msg += " ";
		msg += url;
		Message(msg,PROGRESS_MESSAGE);
	}


	// directly load VRML url, if same frame and no observer  
	if (m_handleWrlIntern && targetFrame == NULL && !(view->observerFlags & GOBSERVE_ANCHOR)) {
		// set up file for url parsing 
		GFile *f= new GFile();
		f->ref();
		
		f->Set(url);
		f->ProcessExtension(f->localFile);

		if (f->localFileFmt == VRML_FMT) {
			f->unref();
		    TRACE("WRL Intern Navigating to %s \n",url);
			ReadUrl(url,NULL);
			return S_OK; 
		};

		f->unref();
	}

   // check if observer wants OnLoadUrl
	if (view->observerFlags & GOBSERVE_ANCHOR) 
    {
		HRESULT res=view->observer->OnLoadUrl((url == NULL) ? NULL : (BSTR) T2COLE(url),
			(targetFrame == NULL) ? NULL : (BSTR) T2COLE(targetFrame));
		if (res == S_OK) return res;
	}
	if (view->observerFlags & GOBSERVE_WM_ANCHOR) 
    {
        m_messageUrl = url;		
        m_messageTargetFrame = targetFrame;    

		MySendMessage(view->observerWnd, WM_USER + 501, (WPARAM) (const char*)m_messageUrl, (LPARAM) (const char*)m_messageTargetFrame);

		//::PostMessage(view->observerWnd, WM_USER + 501, (WPARAM) (const char*)m_messageUrl, (LPARAM) (const char*)m_messageTargetFrame);
		return S_OK;
	}

	// default use HLINK DLL 
   LPUNKNOWN pUnk = GetControllingUnknown();

   return ::NavigateToUrl(pUnk,url,location,targetFrame,flags);
}

/*

   load URL directly from all information of Anchor node / browser loadUrl
*/

BOOL CGLViewCtrlCtrl::loadURLfromFields(const char *description,GvMFString *urls, GvMFString *parameter,const char *homeUrl)
{
    USES_CONVERSION;   

    TRACE("loadURLfromFields  %s\n",description );
	CString absoluteUrl;
	int i;

    if (urls->Length()>0 && !strncmp(urls->get1(0), "OnEvent", 7)) // call vbscript/javascript
    {
        absoluteUrl = urls->get1(0);

        const char *target   = NULL;
		const char *location = NULL;

		if (parameter && parameter->Length()>0) 
        {
			const char *targetPrefix = "target=";
			const int targetPrefixLen = strlen(targetPrefix);

			int i;
			for (i=0;i< parameter->Length(); i++) {
				// check for target option 
				const char *opt = parameter->get1(i).getString();
				if (strncmp(opt, targetPrefix,targetPrefixLen) == 0) {
					target = &opt[targetPrefixLen];
				}
			}
		}
    	return NavigateToUrl(absoluteUrl,location,target) == S_OK;
    }

	if (view->observerFlags & GOBSERVE_ANCHOR) 
    {

		int numUrls=0;
		int numParams=0;

		BSTR *urlsB  = NULL;
		BSTR *paramsB  = NULL;
		BSTR descriptionB = (description == NULL) ? NULL : (BSTR) T2COLE(description);
		
		urlsB = new BSTR[urls->Length()];

		for (i=0; i<urls->Length(); i++) {
			CString url(urls->get1(i));	// url to execute
			
			if (url[0]  == '#') { 
				if (view->SetCamera(& ((const char *) url)[1])) return TRUE;
			}

			CreateAbsolutePath(homeUrl,url,absoluteUrl);
			
			urlsB[i] = absoluteUrl.AllocSysString();				
			numUrls ++;
		}		

		if (parameter) {
			paramsB = new BSTR[parameter->Length()];

			for (i=0; i<parameter->Length(); i++) {
				CString p(parameter->get1(i));	// url to execute
				paramsB[i] = p.AllocSysString();				
				numParams++;
			}		
		}

		HRESULT res=view->observer->OnLoadAnchor(descriptionB,numUrls,urlsB,numParams,paramsB);
		
		for (i=0; i<numUrls; i++) ::SysFreeString(urlsB[i]);
		delete urlsB;
		for (i=0; i<numParams; i++) ::SysFreeString(paramsB[i]);
		delete paramsB;


		if (res == S_OK) return TRUE;
	}            
	
	// standard case 

	if (urls->Length()>0) {
            
			CString url(urls->get1(0));	// url to execute

	
			if (url.GetLength() == 0) { // bad url
				return FALSE;
			}	


			if (url[0]  == '#') { // GOTO viewpoint 
				if ( view->SetCamera(& ((const char *) url)[1]))
				return TRUE;
			}
			


			CreateAbsolutePath(homeUrl,url,absoluteUrl);

			const char *target = NULL;
			const char *location = NULL;

			if (parameter && parameter->Length()>0) {
				const char *targetPrefix = "target=";
				const int targetPrefixLen = strlen(targetPrefix);

				int i;
				for (i=0;i< parameter->Length(); i++) {
					// check for target option 
					const char *opt = parameter->get1(i).getString();
					if (strncmp(opt, targetPrefix,targetPrefixLen) == 0) {
						target = &opt[targetPrefixLen];
					}
				}
			}

			//ReadUrl(absoluteUrl);
			return NavigateToUrl(absoluteUrl,location,target) == S_OK;

            }
			else return FALSE;
}


BOOL CGLViewCtrlCtrl::ExecuteAnchorNode(GvNode *node)
{
	#ifdef _G_VRML1

	  	if (RTISA(node,GvWWWAnchor)) {
			GvWWWAnchor * jump = (GvWWWAnchor *) node;
			
			CString url(jump->name);	// url to execute

			// Add MAP options to URL
			if (jump->map == (int)GvWWWAnchor::POINT) {
				char buf[120];
				Point *p=&view->lastHit->p;
				sprintf(buf,"?%f,%f,%f",p->x,p->y,p->z);
				url += buf;
			}
			// Map objectname
			CString absoluteUrl;
			CreateAbsolutePath(jump->getHomeUrl(),url,absoluteUrl);

			const char *location = NULL;
			const char *target = jump->target;

			if (!target || strlen(target) == 0) target = NULL;

			jump = NULL; // string will be destroyed !
			view->executeNode = NULL;

			// ReadUrl(absoluteUrl);
			NavigateToUrl(absoluteUrl,location,target);

		} // WWWAnchor
	  	else 
#endif _G_VRML1			

			if (RTISA(view->executeNode,Gv2Anchor)) {
			Gv2Anchor * jump = (Gv2Anchor *) view->executeNode;
			
            TRACE("VRML 2 Anchor picked \n");
#if 1
			jump->ref();
		
			BOOL ret = loadURLfromFields(jump->description.get(),&jump->url,&jump->parameter,jump->getHomeUrl());

			jump->unref();
			jump = NULL; 
			view->executeNode = NULL;
			return ret;
#else
            if (jump->url.Length()>0) {
            CString url(jump->url[0]);	// url to execute

		
			CString absoluteUrl;
			CreateAbsolutePath(jump->getHomeUrl(),url,absoluteUrl);

			const char *target = NULL;
			const char *location = NULL;

			if (jump->parameter.Length()>0) {
				const char *targetPrefix = "target=";
				const int targetPrefixLen = strlen(targetPrefix);

				int i;
				for (i=0;i< jump->parameter.Length(); i++) {
					// check for target option 
					const char *opt = jump->parameter[i].getString();
					if (strncmp(opt, targetPrefix,targetPrefixLen) == 0) {
						target = &opt[targetPrefixLen];
					}
				}
			}

			jump = NULL; // string will be destroyed !
			view->executeNode = NULL;

			//ReadUrl(absoluteUrl);
			NavigateToUrl(absoluteUrl,location,target);

            }
#endif
		} // Anchor
		else return FALSE;

	return TRUE;
}


void CGLViewCtrlCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (!m_initialized) return;
	TRACE("CGLViewCtrlCtrl::OnLButtonDblClk() %p\n",this);

	ViewLock viewLock(view); 
	view->SetTheTime(); // set global VRML eventTime

	if (view->triggerMouseEvent(GEV_dblclick, "dblclick",point.x,point.y,nFlags))
		return;


	if (view->eventHandler) { // send event to current eventHandler
		Point screen;
		view->Pixel2Screen(point,screen);
		int ret = view->eventHandler->OnLButtonDblClk(nFlags,screen);
		if (ret == GEventHandler::EV_POP) view->PopEventHandler()->unref();
		if ((view->needUpdate >0) || view->getRedraw()) Redraw();
		return;
	}

	view->EndDegrade();

    // ???   lastMessage = ""; // clear last message to detect messages sent by scripts etc.
    if (view->Select(point.x,point.y,nFlags,GView::JUMP_WWWAnchor)) {
 	  // camera selection is already done	at this stage 
	  if (view->executeNode) {
			ExecuteAnchorNode((GvNode*)view->executeNode);
	  }
	}
}




void CGLViewCtrlCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (!m_initialized) return;
	
	SetCapture(); 
	
	ViewLock viewLock(view); 
	view->SetTheTime(); // set global VRML eventTime


//tv->
	int result = -1;
	CPoint tmpPoint(point);
	//ScreenToClient(&tmpPoint);

	if (view->panel)
	if (view->panel->Inside(point))
		result = view->panel->OnMouseAction(point, PANEL_LBUTTONDOWN); 

	switch (result) {
	case	PANEL_SPHERE0:
		m_NavUPStatePanel = TRUE;
		break;
	case	PANEL_SPHERE1:
		m_NavUPStatePanel = m_NavRIGHTStatePanel = TRUE;
		break;		
	case PANEL_SPHERE2:
		m_NavRIGHTStatePanel = TRUE;
		break;
	case PANEL_SPHERE3:
		m_NavRIGHTStatePanel = m_NavDOWNStatePanel = TRUE;
		break;
	case PANEL_SPHERE4:
		m_NavDOWNStatePanel = TRUE;
		break;
	case PANEL_SPHERE5:
		m_NavDOWNStatePanel = m_NavLEFTStatePanel = TRUE;
		break;
	case PANEL_SPHERE6:
		m_NavLEFTStatePanel = TRUE;
		break;
	case PANEL_SPHERE7:
		m_NavLEFTStatePanel = m_NavUPStatePanel = TRUE;
		break;
	case PANEL_SLIDE_RIGHT:
		m_NavSlideStatePanel = m_NavRIGHTStatePanel = TRUE;
		break;
	case PANEL_SLIDE_LEFT:
		m_NavSlideStatePanel = m_NavLEFTStatePanel = TRUE;
		break;

	case PANEL_HELP:
		view->panel->GetHelpButPos(&point);
		//OnRButtonDown(2, tmpPoint);
		OnRButtonUp(2, tmpPoint);
		break;
	case PANEL_CLOSE:
		if (view->panel->Minimized()) view->panel->maximize();
		else view->panel->minimize();
		Redraw();
		break;
	case PANEL_VIEW_RESET:
		OnCameraReset(); 
		break;
	case PANEL_VIEW_PREV:
		OnCameraViewpointPrev(); 
		break;
	case PANEL_VIEW_NEXT:
		OnCameraViewpointNext(); 
	default:
		break;
	}

	if (result >= PANEL_SPHERE0 && result <= PANEL_SLIDE_LEFT) {
		if (!(m_NavRunning & 0x2))
			NavStart();
		NavCheckKeyState();
	}
	
	if (result >= PANEL_SPHERE0 && result <= PANEL_SPHERE7) {
		m_navPanelDrag = TRUE;
		m_NavNotActive = FALSE;
		m_firstFlags = nFlags;
		m_firstTime = GetTime();
		m_wasDrag = FALSE; 
		view->BeginDegrade();
		m_NavLBUTState = TRUE;
	}

//<-tv
	


	if (!view->panel || (view->panel && !view->panel->Inside(point)))
	{//do the normal work
	int			ret;



	if (!m_timerRunning) 
		    StartTimer();

	m_NavNotActive = FALSE;
	if (view->panel) view->panel->disable();
		m_firstPoint = point;
		m_firstFlags = nFlags;
		m_firstTime = GetTime();
		m_wasDrag = FALSE; 
		
		m_currentTSensor = FALSE;
		m_currentTDragSensor = FALSE;

		// set the keyboard focus
		SetFocus();  // we now have also the OnMouseActivate method ? doesn't work with passport 

		
		if (view->triggerMouseEvent(GEV_mousedown,"mousedown",point.x,point.y,nFlags)) // new 29.05.999
			return;



		// if event handler ?
		if (view->eventHandler) { // send event to current eventHandler
			// degrade ??
			Point screen;
			view->Pixel2Screen(point,screen);
			int ret = view->eventHandler->OnLButtonDown(nFlags,screen);
			if (ret == GEventHandler::EV_POP) 
				view->PopEventHandler()->unref();
			if ((view->needUpdate >0) || view->getRedraw()) 
				Redraw();
			return;
		}



	
		if (GetCameraMode() == NULL_MODE) 
			{ // NONE navigation mode,  process sensors 
			if (view->Select(point.x,point.y,nFlags,GView::DRAG_SENSORS)) 
				{
				if (view->executeNode) 
					{
					m_currentAnchor = (GvNode*) view->executeNode;
					m_currentAnchor->ref();
					}
				if ((view->needUpdate >0) || view->getRedraw()) 
					Redraw();
				}
			return;
			}

		
		// check for navigation keys
		// dont activate sensors if navigation key is pressed

	m_NavCNTLState = nFlags & MK_CONTROL; // HG
	m_NavSpeedState  = nFlags & MK_SHIFT; // HG 

	// do we want navigation 
	BOOL forcedNav = (m_NavPointState || m_NavCNTLState || m_NavFollowState );


//	if ( (view->panel && !view->panel->Active() && !view->panel->Inside(point))
	
	// check for sensor hit, even if a camera walk mode 
	if (m_sensorTest && !(forcedNav || m_NavSlideState || m_NavRotState))
			{
				// ?? lastMessage = ""; // clear last message to detect messages sent by scripts etc.

				if (ret = view->Select(point.x,point.y,nFlags, GView::DRAG_SENSORS)) 
					{
					if (m_anchorSingleClick && view->executeNode) 
						{
						m_currentAnchor = (GvNode*) view->executeNode;
						m_currentAnchor->ref();

					}
					// touch sensor

					if (ret & SELECT_SENSOR_BIT)
						m_currentTSensor = TRUE;

					if (ret & SELECT_DRAG_SENSOR_BIT)
						m_currentTDragSensor = TRUE;

					// there was a sensors or anchor 
					if (view->eventHandler != NULL) 
						{ // sensor handler installed 
						if ((view->needUpdate >0) || view->getRedraw()) 
							Redraw();

						// dont allow Nav drag if drag sensor
						if (m_currentTDragSensor)
							return;	// no navigation because of sensor
						}
					}
				}


	view->BeginDegrade();

	m_NavLBUTState = TRUE;

	// check status of all navigation keys
	NavCheckKeyState();

	// start navigation
	NavStart();

	}

}


void CGLViewCtrlCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (!m_initialized) return;
	ViewLock viewLock(view); 
	view->SetTheTime(); // set global VRML eventTime

	
	if (view->panel)
	{
		if (view->panel->Inside(point))
			view->panel->OnMouseAction(point, PANEL_LBUTTONUP); 
		view->panel->enable();
		m_navPanelDrag = FALSE;
		m_NavUPStatePanel = m_NavDOWNStatePanel = m_NavLEFTStatePanel
			= m_NavRIGHTStatePanel = m_NavSlideStatePanel = FALSE;

	}

	// check status of all navigation keys
	NavCheckKeyState();

	m_NavLBUTState = FALSE;


	if (lastMessageType == NAVIGATION_MESSAGE) 	
		Message("",NAVIGATION_MESSAGE);

	m_currentTSensor = FALSE;
	m_currentTDragSensor = FALSE;

	if (view->triggerMouseEvent(GEV_mouseup,"mouseup",point.x,point.y,nFlags | MK_LBUTTON))
			return;


	if (view->eventHandler) 
		{ // send event to current eventHandler
		Point screen;
		view->Pixel2Screen(point,screen);
		
		int ret = view->eventHandler->OnLButtonUp(nFlags,screen);

		if (ret == GEventHandler::EV_POP && view->eventHandler) 
			view->PopEventHandler()->unref();

/* hg anyway start an anchor 
//		SetCursorMode(STANDARD_CURSOR);
		lastCursorMode = -1;
		ReleaseCapture();
		if ((view->needUpdate >0) || view->getRedraw()) Redraw();
		// 
		return;
*/	
	}


	if (m_currentAnchor) {
		if (!m_wasDrag) {	// no trembling
			m_NavFollowObject = FALSE;
			m_NavCursorShow = TRUE;
			ExecuteAnchorNode(m_currentAnchor);
		}
		if ( m_currentAnchor) m_currentAnchor->unref();
		m_currentAnchor = NULL;
	}


	// we don´t need timers 
	if (m_timerRunning && updateLock >0) 
		StopTimer();

//	if(!m_NavRunning)
//		SetCursorMode(STANDARD_CURSOR);


	view->EndDegrade();
	
	lastCursorMode = -1;

	ReleaseCapture();

}
void CGLViewCtrlCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!m_initialized) return;

	ViewLock viewLock(view); 
	view->SetTheTime(); // set global VRML eventTime
	
	// TRACE("OnMouseMove 	time = %.16g \n", view->getTime());


	// wenn schon navigiert wird, evtl richtung wechseln
	//wenn keine navigation, mouse over
	//tv->
	int result = -1;
	int panelActionFlag;
	CPoint tmpPoint(point);

	if (view->PanelOk())
	if (view->panel->Inside(point))
	{
		if (lastMessageType >= HIT_MESSAGE && lastMessageType <= COLLISION_MESSAGE) 	
			Message("");

		SetCursorMode(NAV_PANEL_CURSOR);
		if (m_navPanelDrag && view->panel->InsideSphere(point))
			view->panel->enable();

		if (nFlags & MK_LBUTTON)
			panelActionFlag = PANEL_MOUSEMOVE_DOWN;
		else
			panelActionFlag = PANEL_MOUSEMOVE_OVER;
		
		result = view->panel->OnMouseAction(point, panelActionFlag); 
		m_NavRIGHTStatePanel = m_NavDOWNStatePanel =m_NavLEFTStatePanel = 
		m_NavUPStatePanel = m_NavSlideStatePanel = FALSE;	
		if (nFlags)
			switch (result) {
			case -1:
				//no hit
				//dragged outside the window
				//m_NavRIGHTStatePanel = m_NavDOWNState =m_NavLEFTState = 
				//m_NavUPState = m_NavSlideState = FALSE;
				break;
			case	PANEL_SPHERE0:
				m_NavUPStatePanel = TRUE;
				break;
			case	PANEL_SPHERE1:
				m_NavUPStatePanel = m_NavRIGHTStatePanel = TRUE;
				break;		
			case PANEL_SPHERE2:
				m_NavRIGHTStatePanel = TRUE;
				break;
			case PANEL_SPHERE3:
				m_NavRIGHTStatePanel = m_NavDOWNStatePanel = TRUE;
				break;
			case PANEL_SPHERE4:
				m_NavDOWNStatePanel = TRUE;
				break;
			case PANEL_SPHERE5:
				m_NavDOWNStatePanel = m_NavLEFTStatePanel = TRUE;
				break;
			case PANEL_SPHERE6:
				m_NavLEFTStatePanel = TRUE;
				break;
			case PANEL_SPHERE7:
				m_NavLEFTStatePanel = m_NavUPStatePanel = TRUE;
				break;
			case PANEL_SLIDE_RIGHT:
				m_NavSlideStatePanel = m_NavRIGHTStatePanel = TRUE;
				break;
			case PANEL_SLIDE_LEFT:
				m_NavSlideStatePanel = m_NavLEFTStatePanel = TRUE;
				break;
				
			default:
				break;
			}//switch
		NavCheckKeyState(FALSE);
	}

	if (result>= PANEL_SPHERE0 && result<=PANEL_SPHERE7) {
		InsideSphere = TRUE;
		if (!(m_NavRunning & 0x2))
			NavStart();
	}
	else
		InsideSphere = FALSE;

	if (view->panel && m_navPanelDrag && (nFlags & MK_LBUTTON) && result == -1)
		 view->panel->disable();//tv


	//  <-tv

	
	if (!view->panel || 
		(view->panel && !view->panel->Inside(point) && !m_navPanelDrag) ||
		(view->panel && m_navPanelDrag && (nFlags & MK_LBUTTON) && result == -1)
		)
		//if no panel
		//if not over panel
		//if navpaneldrag and sphere not hit

	{//do normal work
	int		ret=0;

	

	

	m_prevLastPoint = m_lastPoint;
	m_lastPoint = point;

	m_NavMousePos = point; // hg

	CPoint lastDelta = m_lastPoint - m_firstPoint;

	NavCheckKeyState(); // hg 



	if ((abs(lastDelta.x) + abs(lastDelta.y)) >= m_dragEps)
	{
		if (!m_wasDrag)
		{
			m_wasDrag = TRUE; 
			//NavCheckKeyState();
		}
	}

	if (view->triggerMouseEvent(GEV_mousemove, "mousemove",point.x,point.y,nFlags))
			return;
	m_NavNotActive = FALSE;


	if (view->eventHandler) 
		{ // send event to current eventHandler
		Point screen;
		// if on touch sensor and not on a drag one then stop event handler
		//if (view->eventHandler->sensorType == TOUCH_SENSOR)
		if (m_currentTSensor && ! m_currentTDragSensor)
			{
			if (m_wasDrag)
				{
				m_currentTSensor = FALSE;
				m_currentTDragSensor = FALSE;
				view->eventHandler->unref();
				view->eventHandler = NULL;
				// final events ???
				}
			}
		else
			{

			view->Pixel2Screen(point,screen);
			
			// select to find if sensor still of geometry
			ret=view->Select(point.x,point.y,nFlags,GView::INFO);

			ret = view->eventHandler->OnMouseMove(nFlags,screen);
			if (ret == GEventHandler::EV_POP) 
				view->PopEventHandler()->unref();
			if ((view->needUpdate >0) || view->getRedraw()) 
				Redraw();
			return;
			}
	}

	// update current position 
	m_lastFlags = nFlags;
	m_lastTime = GetTime();
   


	//CString tmp;
	//tmp.Format("%d %d (%d) ",point.x,point.y,m_currentMode);
	//Message(tmp);
/*	

			CString			fname;
			CString			fext;
			char			str[256];

			sprintf(str,"updateLock: %d ", updateLock);
			fext = str;
			fname = fext; 
			Message(fname);
*/
	if (updateLock == 0 && m_initialized) {
		if (!m_timerRunning) 	StartTimer();
	}

	// do we want navigation 
	BOOL forcedNav = (m_NavPointState || m_NavCNTLState || m_NavFollowState );

//	if (m_currentMode == NULL_MODE) 
	if (!(forcedNav || m_NavSlideState || m_NavRotState || m_NavRunning))
		if (m_NavIsWithinRect && m_NavFocus)

		{	// Default mouse move action,  display WWW anchors 
//		CPoint lastDelta = m_lastPoint - m_prevLastPoint;
//		if ((abs(lastDelta.x)  + abs(lastDelta.y)) >=1) 
			{
//			lastMessage = ""; // clear last message to detect messages sent by scripts etc.

//		    ret=view->Select(point.x,point.y,nFlags,GView::INFO_WWWAnchor);

			NavCheckCursor();

/*
		  if (ret & SELECT_ANCHOR_BIT)
			  SetCursorMode(ANCHOR_CURSOR);
		  else if (ret & SELECT_SENSOR_BIT)
			  SetCursorMode(SENSOR_CURSOR);
		  else
			SetCursorMode(NAVIGATION_CURSOR);
*/
			// get window rectangle 	
//			GetClientRect( &client_rect);

/*		
			{
			CString			fname;
			CString			fext;
			char			str[256];

			sprintf(str,"x: %d ", point.x);
			fext = str;
			fname = fext; 

			sprintf(str,"y: %d ", point.y);
			fext = str;
			fname += fext; 
			
			sprintf(str,"   r: %d ", client_rect.right);
			fext = str;
			fname += fext; 

			sprintf(str,"b: %d ", client_rect.bottom);
			fext = str;
			fname += fext; 
			Message(fname);
			}
*/

/*
	if (point.x > 0 && point.y > 0 && point.x < client_rect.right && point.y < client_rect.bottom)
				{
				if (ret & SELECT_ANCHOR_BIT)
					mode = NAV_ANCHOR;
				else
					mode = GetCameraMode();
				
				if (ret & SELECT_SENSOR_BIT)
					mode += NAV_NONE_SENSOR;

				SetCursorMode(mode);
				}
*/
/* HG ?? 
		  if ((view->needUpdate >0) || view->getRedraw()) Redraw();
*/ 


		}
		}
	} 
/* hg don't understand this 
	else
		if (m_NavIsWithinRect && !m_NavFocus)
			if (GetCursor() != hCurrentCursor)
			{
				SetCursor(hCurrentCursor);
			}
*/	
	// hg 6.4.99 to many mouse moves are locking updates
#if 1
	/* if fast rendering, post a WM_TIMER message, under WINDOWS 95 timer resolution is limited  */
	if  (m_fullSpeed && !m_menuActive && (updateLock == 0)  
		&& (view->lastFrameTime < 70) && (mainLoader == NULL) 
		&& (m_timerInterval <= 50)
		/* && (	m_lReadyState == READYSTATE_COMPLETE) */) {
		MSG msg;
		// DWORD GetQueueStatus
        if (m_NavTimerRunning  || !PeekMessage(&msg,NULL,0,0,PM_NOREMOVE)) { // check if any message is pending
			PostMessage(WM_TIMER,0,0); // call me again asap
		} else {
			// TRACE("Fast render mouse move : message pending %d (%x)\n",msg.message,msg.message);
		}
	
	}
#endif

}



// max 64 menus with 64 entries
#define MENU_MAX 64
#define ID_MENUE(MENU,CHOICE) (ID_USER_MENU_START + (MENU) * MENU_MAX  + CHOICE) 


// user menu function triggered
// get menue sensor / index from id and trigger sensor 

void CGLViewCtrlCtrl::OnUserMenu(UINT id) 
{
	int i= id - ID_USER_MENU_START; 
	int menuI = i / MENU_MAX;
	int index = i % MENU_MAX;

	ViewLock viewLock(view); 

	view->SetTheTime(); // set global VRML eventTime

	GvSceneInfo *s = view->GetSceneInfo(); 
	if (menuI>=0 && menuI< s->menueSensors.getNum() && index >=0 ) {
		GvMenuSensor *m = (GvMenuSensor *) (GvNode*) s->menueSensors[menuI];
		if (m && m->enabled && index < m->choices.getNum()) {
			m->choice.set(m->choices[index]);
		}
	}

}



void CGLViewCtrlCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{

	if (view->triggerMouseEvent(GEV_mousedown, "mousedown",point.x,point.y,nFlags))
		return;

	//COleControl::OnRButtonDown(nFlags, point);
}

void CGLViewCtrlCtrl::OnSize(UINT nType, int cx, int cy) 
{
//  checks
	TRACE("CGLViewCtrlCtrl::OnSize %i %i\n", cx, cy); 
	RECT r;
	GetUpdateRect(&r);
	TRACE("OnSize %d update rect %d t %d r %d b %d \n",nType, r.left,r.top,r.right,r.bottom);
	GetWindowRect(&r);
	TRACE("OnSize %d window rect %d t %d r %d b %d \n",nType, r.left,r.top,r.right,r.bottom);
/*	
	if (view->GetFullScreen())
	{
#ifdef _D3D
		TRACE("ignore CGLViewCtrlCtrl::OnSize! We are in FULLSCREEN!\n");
		RECT clientRect;
		::GetClientRect(view->device->hwndFullScreenWnd, &clientRect);
		TRACE("hwndFullScreenWnd %d %d %d %d \n",clientRect.left,clientRect.top,clientRect.right,clientRect.bottom);
		cx = clientRect.right;
		cy = clientRect.bottom;
#endif


	//	return;
	}
*/
	CWnd::OnSize(nType, cx, cy);
	//COleControl::OnSize(nType, cx, cy);	

    if ( !m_initialized ) return;

	if ((m_clientRect.Width() ==  cx) && (m_clientRect.Height() == cy)) {

		GetClientRect(&m_clientRect);
		TRACE("OnSize same size  Rect l %d t %d r %d b %d \n",m_clientRect.left,m_clientRect.top,m_clientRect.right,m_clientRect.bottom);

		#ifdef _D3D
			// reset position on screen, necessary for Direct3D because OnMove not called
			if (view) view->OnMove(0,0); 
		#endif

		return;
	}

	GetClientRect(&m_clientRect);
	TRACE("OnSize %d Rect l %d t %d r %d b %d \n",nType, m_clientRect.left,m_clientRect.top,m_clientRect.right,m_clientRect.bottom);
	TRACEREF();


	
	if ( view ) // || !GetAsyncKeyState(VK_LBUTTON)) 
    {
		ViewLock viewLock(view); // for now 
		// propagate to renderthread 
		
		view->OnSize(nType,cx,cy);
		// m_resized = FALSE;

		#ifdef _D3D
			// reset position on screen, necessary for Direct3D because OnMove not called
			view->OnMove(0,0); 
		#endif


	}   
	else {
	   // m_resized = TRUE;
	   // m_sizeType = nType;
	   // m_sizeCx = cx;
	   // m_sizeCy = cy;
		TRACE("ignoring SIZE ");
	}
}

void CGLViewCtrlCtrl::OnMove(int x, int y) 
{
    COleControl::OnMove(x, y);
	GetClientRect(&m_clientRect);
	TRACE("CGLViewCtrlCtrl::OnMove Rect l %d t %d r %d b %d \n",m_clientRect.left,m_clientRect.top,m_clientRect.right,m_clientRect.bottom);

	if (m_initialized)
    {
		ViewLock viewLock(view); // for now 
	    view->OnMove(0,0);
    }
}



void CGLViewCtrlCtrl::OnSetClientSite() 
{
//	RecreateControlWindow(); // from time cntrl
	TRACE("CGLViewCtrlCtrl::OnSetClientSite()  %p\n",this);
	
	COleControl::OnSetClientSite();
	// if client site NULL, control disabled / no longer visibe ?
	if (m_pClientSite == NULL) 
    {
		TRACE("OnSetClientSite:: now null\n"); 
	}		
}


int CGLViewCtrlCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	EAI_TRACE("CGLViewCtrlCtrl::OnCreate() %p\n",this);
	EAI_FLUSH();

	TRACE("CGLViewCtrlCtrl::OnCreate() %p THread Id is %X \n",this,GetCurrentThreadId());
	TRACEREF();

	// lpCreateStruct->cy-=20;

	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_initialized) 
    {
		HWND hwnd = GetSafeHwnd();
		if (!m_pDC) m_pDC = GetDC(); 
		if (!Initialize(m_pDC->m_hDC)) return -1;

		// start loading 
		if (m_initialUrl.GetLength()>0)
			SetUrl(m_initialUrl);
		else InternalSetReadyState(READYSTATE_COMPLETE);

		if (m_initialAvatarUrl.GetLength()>0) {	// 
			setMyAvatarURL(m_initialAvatarUrl);
		}
		if (m_initialThirdPersonView)
			setThirdPersonView(m_initialThirdPersonView);


	}

#if 0
	// HG 19.03.99 *********************, check also on MouseActivate
	// this causes CC3D to lock in memory in IE 4.0
	// I do not know why this is here, think was KB base workaround for IE 3.0 ?????
	OnActivateInPlace (TRUE, NULL); // == UI-Activate the control

#endif

	// TODO: Add your specialized creation code here
	// could get initial window size from createStruct

	TRACEREF();

	LPUNKNOWN pCC3D = GetInterface(&IID_IUnknown); // (does not AddRef)
    ::SetProp(GetSafeHwnd(), "CC3D_LPUNK", (HANDLE)pCC3D);

	TRACEREF();


	GetClientRect(&m_clientRect);

	return 0;
}

// see http://www.microsoft.com/kb/articles/q152/3/91.htm
void CGLViewCtrlCtrl::OnClose(DWORD dwSaveOption) 
{
	// send termination message 
	if (view && view->observerFlags & GOBSERVE_ANCHOR)
		HRESULT res=view->observer->OnLoadUrl(NULL, NULL);

	// called if IE-Explorer File->Closed called 
	TRACE("CGLViewCtrlCtrl::OnClose() %p\n",this);
	TRACEREF();
	COleControl::OnClose(dwSaveOption);
}

// kill all download activities for this window 
void CGLViewCtrlCtrl::KillDownloads() 
{
	// kill all download activities for this window 
	if (GetSafeHwnd()) 
    {
		GFile::KillFilesInUse(GetSafeHwnd());
		// pass request to netscape message window 
		if (view && view->observerFlags & GOBSERVE_WM_URLLOADING) {
			MySendMessage(view->observerWnd, WM_USER + 503, (WPARAM) NULL,(LPARAM) NULL);
		}
	}
}


void CGLViewCtrlCtrl::OnDestroy() 
{

	EAI_TRACE("CGLViewCtrlCtrl::OnDestroy() %p\n",this);
	EAI_FLUSH();

	TRACE("CGLViewCtrlCtrl::OnDestroy() %p\n",this);
	TRACEREF();

	if (m_useRenderThread) 
		StopRenderThread();

	if (m_saveDriverSettings)  // save driver settings
		SaveDriverSettings();

	CloseAllDialogs();
	
	// destroy console
	if (m_dConsole) { 
		m_dConsole->DestroyWindow(); delete m_dConsole; m_dConsole = NULL; 
	}

	// called very late if internet explorer is beeing released, not when the HTML page changes 
	// also after close 

	StopTimer();


	if (mainLoader)	 // kill main URL loader
    {
   		mainLoader->Kill();
   		mainLoader->unref();
		mainLoader = NULL;
	}

	KillDownloads();


	// TO DO: kill all observer connections to Java

    // release the DC 
	if (m_pDC) ReleaseDC(m_pDC);
	m_pDC = NULL;



	COleControl::OnDestroy();
	
	// TODO: Add your message handler code here

	if (view) 
    {
		view->Term();
		// back pointer not valid
		view->SetView(NULL);	// back pointer
		view->SetViewController(NULL);	// back pointer

		ASSERT(view->refCnt == 1); // other wise some bad guy keeps a pointer ????
		view->unref(); // should go to delete view
	}
	view = NULL;
	m_initialized = FALSE;


#ifdef _OGL_XX
	if (m_glptr) delete m_glptr;
	m_glptr = NULL;
#endif
	TRACEREF();
}



BOOL CGLViewCtrlCtrl::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default

#ifdef _D3D
	if (m_initialized && view && view->device->bInitialized 	&& (view->currentTraversal == NULL)) 
		return(FALSE);
	else 
		return COleControl::OnEraseBkgnd(pDC);
#else
	if (!m_initialized)
		return COleControl::OnEraseBkgnd(pDC);
#endif

	return TRUE; // don´t erase 

	//return COleControl::OnEraseBkgnd(pDC);
}




// do animation 
BOOL CGLViewCtrlCtrl::OnIdle(LONG lcount)
{	
    if ( !m_initialized ) 
		return false;

	// TRACE("CGLViewCtrlCtrl::OnIdle\n");

	float LastFrameTime = 0.1; //XX

    gbool ret = FALSE;
	gbool redraw=FALSE;

	view->Lock();

	// check for a camera animation
    if (view->AnimatingCamera()) {
        if (view->AnimateCameraStep()==2) {
        };
        redraw = TRUE;
        ret = TRUE;
    }
	else 
	if (view->AnimatingPhysicalCamera()) // the 3rd person follow mode 
		redraw = TRUE;

	if (updateLock == 0) {

	// check animations 
	if (view->time.Enabled() && (!view->time.Paused())) {
		
		// step the time 
		if (view->playRealtime) {
		   view->time.Step(LastFrameTime);  
	 	} else 
	 	  view->time.Step();

		m_timerCnt ++;

		if (m_timerCnt == m_timerTimeSensorCycle) {  // do less timer sensor work 
			m_timerCnt = 0;

			// update sceneInfo to keep track of TimeSensors new 15.08.97
			// viewpoint popup -menu ?
			if (view->doUpdateSceneInfo) 
				view->UpdateSceneInfo();

			// trigger any VRML 2.0 time sensors
			// has a sensor triggered and changed some field values ??
			if (view->TriggerTimeSensors() && view->getRedraw() /* GvConnection::getTriggeredConnectionsCnt()>0*/) {
				redraw = TRUE;
				ret = TRUE;
			}
			else {
				// if there are VRML 1.0 GLView animation extensions
				if (view->sceneInfo && view->sceneInfo->IsAnimated())
					redraw = TRUE;
			}
		}

	}

	// check for any other changes
	if (redraw || (view->needUpdate >0) || view->getRedraw() /* GvConnection::getTriggeredConnectionsCnt()>0*/) {
		redraw = TRUE;
	}
	
	view->CheckTasks();
	} // updateLock == 0


	// trigger repaint
	if (redraw || view->GetFullScreen()) {
		// TRACE("Redraw from OnIdle");
		Redraw();
	}
    ret = FALSE;

	// check if we need to run the cache cleanup thread 
	if (theCache.writeCacheEnabled && !theCache.cleanerBusy && theCache.NeedCacheCleaning(m_startTime)) {
		Message(Translate(_T("Cleaning cache ...")),PROGRESS_MESSAGE);
		theCache.cleanerBusy = TRUE;
		CWinThread * t = AfxBeginThread(CUrlCacheCleanerThread, &theCache,THREAD_PRIORITY_NORMAL,64*1024);	 	 	
	}

	view->Unlock();

	if ( lcount )
        return(FALSE);        
    else
        return(ret);

}

const short ID_TIMER = 1;
const short NAV_TIMER = 2;

void CGLViewCtrlCtrl::OnTimer(UINT nIDEvent) 
{
	if (!m_initialized) 	return;
	
	if (m_useRenderThread) return;

	//if (view->Locked()) return;
	
	OnDoTimer(nIDEvent);
}


void CGLViewCtrlCtrl::OnDoTimer(UINT nIDEvent) 
{
	//int		count;
	//BOOL	focus;
	
	if (!view) {
		TRACE("CGLViewCtrlCtrl::OnDoTimer but no view \n");
		return ;
	}
	// TRACE("CGLViewCtrlCtrl::OnDoTimer\n");
	GFPSetControl(); // maks FP exceptions 

/*
 	// check navigation keys
	if (!m_NavNotActive)
		mode = NavCheckKeyState();
*/
//	m_tcount++;
	// if navigation timer then call navigation
	if (m_NavTimerRunning && !(view->AnimatingCamera()))
		NavGo();

 	// check navigation keys and set cursor shapes
	// if there is no pop up menu
/*
	if (!m_NavNotActive)
		NavCheckKeyState();
*/

	BOOL isFast= (view->lastFrameTime < 80); // new logic hg.

	//isFast = FALSE; // old path 

	if (GetFocus() != this)
	{ 
		if (m_NavFocus)
		{		
			m_NavFocus = FALSE;
		//	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
			m_NavCursorModeOld = -2;
			m_NavNotActive = TRUE;
			//SetCursor(hCurrentCursor);
			TRACE("Timer:lost  Focus \n");
		}
	}
	else
	{ 
		if (!m_NavFocus)
		{		
			m_NavFocus = TRUE;
			TRACE("Timer:Now Focus \n");
//			m_NavNotActive = FALSE;
		}
	}

#if 1
	if (!m_useRenderThread) {
	DWORD theTime= timeGetTime();
    if ( m_fullSpeed )
	{
       // HG 20.08 if ( ((DWORD)(theTime - m_endTime)) < 10) return;
    
    }
    else
    {
		// time last rendering cycle stopped
		if ( ((DWORD)(theTime - m_endTime)) <= 15)
        {
			// leave 15 msec time frame before checking time sensors
			return;
		}
	}
    }

#endif


	// check cursor state and change if necessary
	if (!m_NavNotActive && m_NavFocus)
	{
		m_NavTimerCnt++;
		if (m_NavTimerCnt >= 3)
		{	
			m_NavTimerCnt = 0;
			NavCheckCursor();
		}
	}

	// if in follow object mode then let cursor blink
	if (m_NavFollowObject)
	{
		if (!view->followObject) // follow object no longer active
			NavResetFollowObject();
		else 
		{
		if (m_NavIsWithinRect && !m_NavNotActive && m_NavFocus)
			{
			m_NavCursorBlkCount++;
			if (m_NavCursorBlkCount >= 7)
				{
				m_NavCursorBlkCount = 0;
				if (m_NavCursorShow)
					{
					SetCursorMode(NAV_FOLLOW);
					//ShowCursor(FALSE);
					m_NavCursorShow = FALSE;
					}
				else
					{
					SetCursorMode(m_NavCursorModeOld);
					//ShowCursor(TRUE);
					m_NavCursorShow = TRUE;
					}
				}
		}		
		else
			// reset blinking
			if (!m_NavCursorShow)
			{
				NavResetCursorBlinking();
			}
		}
	}



//else  // trigger timeSensors, check for redraw bit 
	//if (m_NavNotActive || !(m_NavTimerRunning || m_NavIsWithinRect) || !m_NavFocus)
	//if (m_NavNotActive || !m_NavTimerRunning || !m_NavFocus)
#if 0
	if (m_useRenderThread) {	
		m_mustRedraw |=  view->getRedraw();  // (((int)TlsGetValue(TLS_REDRAW)) !=0);
		//check for replaceWorld loadURL requests 
		view->CheckForSceneChange();
		return;
	}
#endif

    /* check foreground window */
    bool isForeground = false;
    {
        CWnd * top = GetTopLevelParent();
        CWnd * fg  = GetForegroundWindow();
        if (fg)
            fg = fg->GetTopLevelParent();
	    if (fg && top && fg->GetSafeHwnd() == fg->GetSafeHwnd())
        {
            isForeground = true;
        }
    }

	if ( /* m_NavNotActive || hg */ (!m_NavFocus || isFast) && isForeground ) // new isFast
	{
		m_NavTimerCnt_1++;
        m_idleCounter = 0;
		if (m_NavTimerCnt_1 >= 5 || isFast )
		//if (m_NavTimerCnt_1 >= 1)
		{	
			m_NavTimerCnt_1 = 0;
			OnIdle(1);
			 // hg Sleep(10);
		}
	}
	else
    {
        if ( !isForeground )
        {
			m_idleCounter +=1;
			if (m_idleCounter >= 10)
            {
				OnIdle(1);
				m_idleCounter = 0;
            }
        
			Sleep(0);

        }
        else if (!m_useRenderThread &&  view && view->observer )
        {
			m_idleCounter +=2;
			if ( isFast ||(m_idleCounter < 10))
				OnIdle(1);
			else
				m_idleCounter = 0;
        
			Sleep(0);

        }
        else { // hg fullspeed 
           OnIdle(1);
		}
    }

	//check for replaceWorld loadURL requests 
	view->CheckForSceneChange();

}																



/////////////////////////////////////////////////////////////////////////////
// CGLViewCtrlCtrl::StartTimer - Start the timer used for animation

void CGLViewCtrlCtrl::StartTimer()
{
	if (m_useRenderThread) return;
	if (!m_initialized) return;

	if ( SetTimer(ID_TIMER, m_timerInterval, NULL))
	    m_timerRunning = TRUE;
}


// ReStart Timer if temporary disabled
void CGLViewCtrlCtrl::RestartTimer() 
{
	if (/* updateLock == 0 && */ m_initialized) {
		if (!m_timerRunning) 	
			StartTimer();
	}
}



/////////////////////////////////////////////////////////////////////////////
// CGLViewCtrlCtrl::StopTimer - Stop the timer.

void CGLViewCtrlCtrl::StopTimer()
{
	if (m_timerRunning) 
		KillTimer(ID_TIMER);
	m_timerRunning = FALSE;
}


void CGLViewCtrlCtrl::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{	BOOL mActive;

	COleControl::OnActivate(nState, pWndOther, bMinimized); // new 12.06.99
	
	TRACE("CGLViewCtrlCtrl::OnActivate(%d, %p, %d) \n",nState, pWndOther,bMinimized);

	if (nState == WA_INACTIVE || bMinimized) 
    {
		mActive = FALSE;
	}		
	else mActive = TRUE;
/*
	if (!mActive) {
		AfxMessageBox("deactivated",MB_OK,0);
	}
	else 
		AfxMessageBox("activated",MB_OK,0);
*/
}


// get the current URL (DISPATCH property)
BSTR CGLViewCtrlCtrl::GetUrl() 
{
	CString strResult;
	if (view) strResult = view->GetUrl();
	else strResult = m_initialUrl;
	return strResult.AllocSysString();
}

// set the new URL (DISPATCH property)
void CGLViewCtrlCtrl::SetUrl(LPCTSTR lpszNewValue) 
{
	CString newUrl(lpszNewValue);

	if (!view) {  // not initialized yet, store the url
		m_initialUrl = newUrl;
		return;
	}
	TRACEREF();

	
	//set the *crash in url* flag
	HKEY hKeyRoot=NULL;

	::RegOpenKey(HKEY_CURRENT_USER, USER_KEY, &hKeyRoot);
	SetProfile(_T("Direct3D.crashInUrl"), lpszNewValue);
	if (hKeyRoot) ::RegCloseKey(hKeyRoot);



	// reenable warnings/error messages if disabled 
	m_reporter->Clear();
	m_reporter->setIngore(FALSE,FALSE);
	// LPOLECLIENTSITE = GetClientSite();


	// get the container url for resolving if url is relative 
	IMoniker *pMoniker=NULL;
	HRESULT result;
	LPOLESTR container=NULL;
	LPOLESTR object=NULL;
	LPOLESTR objectRel=NULL;
	CString home;


	if (m_pClientSite) {
	
	if (m_pClientSite->GetMoniker(OLEGETMONIKER_TEMPFORUSER,OLEWHICHMK_CONTAINER,&pMoniker) == S_OK)
	{
		if (result = pMoniker->GetDisplayName(NULL,	NULL,&container) == S_OK) {
			home = container;
		}
		pMoniker->Release();
    }


/*
	// these are always NULL 
	if (m_pClientSite->GetMoniker(OLEGETMONIKER_TEMPFORUSER,OLEWHICHMK_OBJFULL,&pMoniker) == S_OK)
	{
		result = pMoniker->GetDisplayName(NULL,	NULL,&object);
		pMoniker->Release();
    }

	CString home1;
	home1 = object;

	if (m_pClientSite->GetMoniker(OLEGETMONIKER_TEMPFORUSER,OLEWHICHMK_OBJREL,&pMoniker) == S_OK)
	{
		result = pMoniker->GetDisplayName(NULL,	NULL,&objectRel);
		pMoniker->Release();
    }

	CString home2;
	home2 = objectRel;

	if (object) ::SysFreeString(object);
	if (objectRel) ::SysFreeString(objectRel);
*/

	}

	if (container) CoTaskMemFree(container);

	// go and read it now 
	ReadUrl(newUrl,home);
	TRACEREF();

}


// go back in url history stack 
BOOL CGLViewCtrlCtrl::GoBack()
{
	if (m_historyIndex <= 0) return FALSE;
	m_historyIndex --;

	CString url(m_history[m_historyIndex]);
	m_historyIndex--;

	// SetUrl(url);
	NavigateToUrl(url);

	return TRUE;

}

// go forward in url history stack 
BOOL CGLViewCtrlCtrl::GoForward()
{
	if (m_historyIndex >= (m_history.Length()-1)) return FALSE;
	m_historyIndex ++;

	CString url(m_history[m_historyIndex]);

	m_historyIndex --; // will be pushed again by AddUrlToHistory

	// SetUrl(url);
	NavigateToUrl(url);

	return TRUE;

}

// store url in histroy 
void CGLViewCtrlCtrl::AddUrlToHistory(const char *url,const char *description, BOOL isFile)
{
	CString s(url);
	BOOL traceUrl= FALSE;

#ifdef _DEBUG
	traceUrl= TRUE;
#endif
	HKEY hKeyRoot=NULL;
	CString val;
	


	::RegOpenKey(HKEY_CURRENT_USER, USER_KEY, &hKeyRoot);

	if (hKeyRoot && GetRegKey(hKeyRoot,_T("General.traceUrl"), traceUrl)) {

	}

	if (hKeyRoot) ::RegCloseKey(hKeyRoot);

	if (traceUrl) {
	// trace all URL's in HTML ready format
	CString dir = view->GetInstallDirectory();
	FILE *f;
	CString name;
	name = dir +  "history.htm";

	f= fopen(name,"a+");
	if (f) {
		fprintf(f,"<a href=\"%s\" target=world>%s</a><p>\n",url,url);
		fclose(f);
	}
	name = dir +  "history.inc";
	f= fopen(name,"a+");
	if (f) {
		fprintf(f,",\"%s\"\n",url);
		fclose(f);
	}
	} // traceUrl

	
	m_historyIndex ++;

	int l=m_history.Length();

	if (m_historyIndex>=0 && m_historyIndex < l)
		m_history[m_historyIndex] = url;
	else 
	{	
		if (l > 128) { // flush it 
			m_history.Flush();
		}
		m_history.Append(s);
		m_historyIndex = m_history.Length()-1;
	}
	
}

/*
  Read the url / setup download for the URL cc3d should handle

*/
int CGLViewCtrlCtrl::ReadUrl(const char *url,const char *homeUrl,gbool reload,time_t lastModified) 
{  int ret=-1;
   
   //Reporter.Status("URL %s ...",url);

   CString msg;

   USES_CONVERSION;

   
   Translate(_T("Loading"),msg);

   msg += "\"";
   msg += url;
   msg+="\" ...";

   Message(msg,PROGRESS_MESSAGE);
   TRACEREF();
   	   
   BeginWaitCursor();

   if (mainLoader) // kill current loader 
   {
		mainLoader->Kill();
		mainLoader->unref();
		mainLoader = NULL;
   }

   KillDownloads();


   mainLoader = new GFile;
   mainLoader->useThread = GFile::useThreads;
   mainLoader->ref();
   mainLoader->ClearSessionError();  // reset error flags 

   gbool errors= !(view && view->observerFlags & GOBSERVE_URLERRORS);

   mainLoader->SetFlags( (errors ? WWW_REPORT_ERRORS:0)  /*|WWW_EXECUTE_UNKNOWN_FMTS*/ 
				| (reload ? WWW_RELOAD : 0));

   // caching mode verify top-level refresh
   mainLoader->ifModifiedSince = theCache.getRefreshTime();
   
   if (lastModified >0) mainLoader->ifModifiedSince = lastModified;


   mainLoader->SetReporter(GetReporter()); // set the error/progress reporting object
 
   // window to notify on completition
   mainLoader->hPostMsgWnd = GetSafeHwnd();

   // check if observer wants OnSceneChanged
   if (view->observerFlags & GOBSERVE_URLLOADING) {
		mainLoader->hObserver = view->observer;
   }
   if (view->observerFlags & GOBSERVE_WM_URLLOADING) {
	   mainLoader->hNetscapeMsgWnd = view->observerWnd;
   }
  // should work, but get exception somewhere in HTMLvw
   // mainLoader->client =  (LPUNKNOWN)GetInterface(&IID_IUnknown); // GetClientSite(); //GetControllingUnknown();
   // to be removed if (!m_scb) m_scb = new CBindStatusCallback(this);
   // to be removed mainLoader->statusCallback = m_scb;

 
   if (homeUrl) {
	   mainLoader->SetHome(homeUrl);
	   mainLoader->SetRefedFrom(homeUrl,"Load");

   } else {	
	  mainLoader->urlHome = view->file->urlHome;
	  mainLoader->localFileHome = view->file->localFileHome;
   }

   mainLoader->Set(url,0,0);

   // process url 
   ret = mainLoader->Process();

   if (ret == ERROR_THREAD_STARTED) // download started, quit for now , notification on read file completed 
   {
		InternalSetReadyState(READYSTATE_LOADING);
   }
   else  {

	  // we should have some local file name 	
	  if (ret >=0) {
		 CString path(mainLoader->GetUrl());
		 ViewLock viewLock(view); 
  
		 AddUrlToHistory(mainLoader->GetFullUrl());
		 
	
		if (mainLoader->localFileUnzipped.GetLength()>0) {
			ret = view->ReadModel(mainLoader);
			if (ret>=0) {
				SetAnimateOff();
				Redraw();
				Message("Url "+path);
				FireOnSceneChanged(path);
				// check if observer wants OnSceneChanged
				if (view->observerFlags & GOBSERVE_MESSAGE) {
					view->observer->OnSceneChanged((BSTR) T2COLE((const char*)path));
				}
			}

		} else ret=0;
	}

	if (ret <0) {
		// check if observer wants OnUrlError
		if (view && view->observerFlags & GOBSERVE_URLERRORS) {
			HRESULT res=view->observer->OnUrlError((BSTR) T2COLE((const char*) mainLoader->GetUrl()), NULL,ret);
		}

	}
 	mainLoader->unref();
	mainLoader = NULL;
	InternalSetReadyState(READYSTATE_COMPLETE);
   }


   EndWaitCursor();
   TRACEREF();
   return(ret);
}


//
// OnReadFileCompleted -   GFile Thread sends a user defined WM_READFILECOMPLETED
//                         message to this window, when the thread has finished.
//
afx_msg LRESULT 
CGLViewCtrlCtrl::OnReadFileCompleted(WPARAM wParam, LPARAM lParam)
{  int ret=-1;
   CString msg;

   USES_CONVERSION;
   
   GFile *f= (GFile *) lParam;
   
   if (!f) {	// the GFile structure is passed in lParam

#ifdef _DEBUG
   		Reporter.Error("OnReadFileCompleted : Bad File notification");
#endif 

		return(TRUE);

   }
   if (f->refCnt<=0) { // reference counter should never be 0
	   TRACE("CGLViewCtrlCtrl::bad file notification \n") ;
	   return(TRUE);
   }	

   TRACE("CGLViewCtrlCtrl::OnReadFileCompleted Ret = %d (Refs = %d)  %s\n",f->threadRet, f->refCnt, (const char *) f->url) ;
   
   EAI_TRACE("CGLViewCtrlCtrl::OnReadFileCompleted Ret = %d (Refs = %d)  %s\n",f->threadRet, f->refCnt, (const char *) f->url) ;
   EAI_FLUSH();



   f->threadBusy = 0; // acknowledge termination from thread proc
   ret = f->threadRet; // get return code from thread
   
   ViewLock viewLock(view); 

   if (ret<0) {
   } // error	
   else {
		Translate(_T("Loaded"),msg);
		msg += " ";
		msg += f->url;

		Message(msg,PROGRESS_MESSAGE);
   }	


   // it is the mail url loader task 
   if (f == mainLoader)	{

	  BeginWaitCursor();
	  	
	  	ASSERT(!mainLoader->Busy());

		mainLoader->isThread = 0;
	
	    ret = mainLoader->threadRet; // get return code from thread

		if (1) {
      	 
      	    ret = mainLoader->ProcessLoad(ret);

   	  		if (ret == ERROR_THREAD_STARTED) // quit , notification on read file completed 
   	  		{
   	  		}
   	  		else  {
   
   			if (ret >=0) {
				CString path(mainLoader->GetUrl());
	   
				AddUrlToHistory(mainLoader->GetFullUrl());
		  	
		
			if (mainLoader->localFileUnzipped.GetLength()>0) {
				
				Translate(_T("Reading"),msg);
				msg += " ";
				msg += path;
				
			     	Message(msg);
             
				ret = view->ReadModel(mainLoader);
				if (ret>=0) {
					SetAnimateOff();
					Redraw();
					Message("Url "+path);
					FireOnSceneChanged(path);
					// check if observer wants OnSceneChanged
					if (view->observerFlags & GOBSERVE_MESSAGE) {
						view->observer->OnSceneChanged((BSTR) T2COLE((const char *) path));
					}
				} else { // url error
				}

			} else ret=0;
   		} // ret >= 0
		}
	   
    }

   if (ret != ERROR_THREAD_STARTED)
   {
   		mainLoader->unref();
		mainLoader = NULL;
   }

   EndWaitCursor();
   SetCursorMode(lastCursorMode);

   if (ret <0) {	
		// check if observer wants OnUrlError
		if (view && view->observerFlags & GOBSERVE_URLERRORS) {
			HRESULT res=view->observer->OnUrlError((BSTR) T2COLE((const char*) f->GetUrl()), NULL,ret);
		}	
	}

   // do the unref from PostMessage 
   f->unref();

   return TRUE ;

   } // main loader  
   


   // check any createVrmlFromUrl nodes pending
   //
   GvScene * scene =  view->GetScene();
   BOOL createVrmlFromUrlFound = FALSE;

   if (scene && scene->createVrmlFromUrls.getNum()>0) {

	// for resolving relative urls 
	GLoadInlinesTraversal traversal;
	traversal.view = view;
	traversal.resolveRelativeUrls = gtrue;

	scene->ref(); // keep it alive 
	for (int i=0; i<scene->createVrmlFromUrls.getNum(); i++) {
		Gv2CreateVrmlFromUrl *src = (Gv2CreateVrmlFromUrl *) scene->createVrmlFromUrls.get1(i);
		src->ref();


		// check & process load request 
		src->Load(traversal);

		// get current status 
		GFileState status = src->loader.GetState();

		switch (status) {
		case S_NOT_LOADED :	 // to many threads, wait 
		case S_LOADING :
		case S_PROCESSING :
			break; // continue to work on this 

		default : 		
			{ // no longer loading 
			TRACE("OnReadFileCompleted :: CreateVrmlFromUrl: %s done %d \n",src->GetUrl(),status);
			EAI_TRACE("OnReadFileCompleted :: CreateVrmlFromUrl: %s done %d \n",src->GetUrl(),status);


			// remove request from list 
			scene->removeCreateVrmlFromUrl(src); // will unref it !
			createVrmlFromUrlFound = TRUE;

			// traverse and instace proto's trigger extern proto load 
			
			traversal.traversalOrder = view->traversalInlineLoadingOrder;
			traversal.doViewCulling= FALSE;
			traversal.loadProtos = GTraversal::LOAD_ALL;
			traversal.loadInlines = GTraversal::DONT_LOAD;
			traversal.loadSounds = GTraversal::DONT_LOAD;
  
			traversal.maxInlineLevel = view->maxInlineDepth;
			traversal.InitializeAttributes();

			traversal.SetCamera(view->globalCamera);
			traversal.SetPhysicalCamera(view->GetPhysicalCamera());

			traversal.traverseAll = 1;
			traversal.traverseTransformHierarchyOnly=1;

			src->Traverse(traversal);


			src->SendEvent();

			// TO DO: / BUG, need to resolve relative inlines in CreateVrmlFromUrl !

			i--; // continue check others 
			}
		}
		src->unref();

	} // for i
	scene->unref(); // keep it alive 


   }	

   if (createVrmlFromUrlFound)   
   {
	  view->UpdateSceneInfo(); // probably sensor stuff added 
	  // addChildren will set redraw bit !
   } else
   if (f->refedType == "EXTERNPROTO") { // an EXTERNPROTO file was loaded 
	  view->UpdateSceneInfo(); // scene may need protos if even not in visible traverse scene graph
   }
   // to think batch updateSceneInfo calls until next redraw 


   // redraw, so that inline gets loaded during rendering traversal 
   if (f->refCnt > 1)  // non obsolete thread
         view->needUpdate ++;

   if (ret<0) {
		// check if observer wants OnUrlError
		if (view && view->observerFlags & GOBSERVE_URLERRORS) {
			HRESULT res=view->observer->OnUrlError((BSTR) T2COLE((const char*) f->GetUrl()),
				NULL,ret);
		}
   } // error	

   // do the unref from PostMessage 
   f->unref();

   // load a bit more if no any other pending download , only in IE enviroment
   if ( (GFile::threadsInUse == 0) && 
	   !((view->observerFlags & GOBSERVE_URLLOADING) || 
	   (view->observerFlags & GOBSERVE_WM_URLLOADING))) 
   {
	   view->triggerUrlLoading = TRUE;

	   if (!m_enableInteraction) { // IE has minimized use 
			view->triggerUrlLoading = FALSE;
			view->LoadAllInlines(1,1);

	   }
   }


   return TRUE;
}





void CGLViewCtrlCtrl::OnCameraNone() 
{
	SetCameraMode(NULL_MODE);
}

void CGLViewCtrlCtrl::OnUpdateCameraNone(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetCameraMode() == NULL_MODE);
}

void CGLViewCtrlCtrl::OnCameraOrbit() 
{
	SetCameraMode(ORBIT_MODE);
	SetCursorMode(NAV_EXAMINE);
	Message("Examine: Keys R : move in/out , J : jump to, space : Follow",PROGRESS_MESSAGE);

}

void CGLViewCtrlCtrl::OnUpdateCameraOrbit(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetCameraMode() == ORBIT_MODE);
}

void CGLViewCtrlCtrl::OnCameraPan() 
{
	SetCameraMode(PAN_MODE);
	SetCursorMode(NAV_PAN);

}

void CGLViewCtrlCtrl::OnUpdateCameraPan(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetCameraMode() == PAN_MODE);
}

void CGLViewCtrlCtrl::OnCameraJump() 
{
	SetCameraMode(JUMP_TO_MODE);
}

void CGLViewCtrlCtrl::OnUpdateCameraJump(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetCameraMode() == JUMP_TO_MODE);
}


void CGLViewCtrlCtrl::OnCameraRoll() 
{
	SetCameraMode(ROLL_MODE);
}

void CGLViewCtrlCtrl::OnUpdateCameraRoll(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetCameraMode() == ROLL_MODE);
}

void CGLViewCtrlCtrl::OnCameraDollyXy() 
{
	SetCameraMode(DOLLY_XY_MODE);
	SetCursorMode(NAV_SLIDE);
}

void CGLViewCtrlCtrl::OnUpdateCameraDollyXy(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetCameraMode() == DOLLY_XY_MODE);
}

void CGLViewCtrlCtrl::OnCameraFly() 
{
	SetCameraMode(FLY_MODE);
	SetCursorMode(NAV_FLY);
}

void CGLViewCtrlCtrl::OnUpdateCameraFly(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetCameraMode() == FLY_MODE);
}

void CGLViewCtrlCtrl::OnCameraRotate() 
{
	SetCameraMode(ROTATE_MODE);
	SetCursorMode(NAV_ROTATE);
	NavStraightenUp();
	
}

void CGLViewCtrlCtrl::OnUpdateCameraRotate(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetCameraMode() == ROTATE_MODE);
	
}





void CGLViewCtrlCtrl::OnCameraAnimateViewpoints() 
{
	if (view->AnimatingCamera()) {
		view->AnimateCameraStop();
    }
	else view->AnimateAllCameras();
}

void CGLViewCtrlCtrl::OnUpdateCameraAnimateViewpoints(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(view->AnimatingCamera());
}

void CGLViewCtrlCtrl::OnCameraReset() 
{
	if (view->AnimatingCamera()) 
		view->AnimateCameraStop();
	
	// try to reset to entry viewpoint
	if (!view->SetViewpoint(0,1))
	// no viewpoint defined then zoom out
		view->CameraReset();
	
	Redraw();
}


void CGLViewCtrlCtrl::OnCameraSeekto() 
{
   // ?? lastMessage = ""; // clear last message to detect messages sent by scripts etc.
   if (view->Select(m_rlastPoint.x,m_rlastPoint.y,m_rlastFlags,GView::JUMP_OBJECT)) {
		Message("Jumping to object");
		Redraw();
   }
}

void CGLViewCtrlCtrl::OnUpdateCameraSeekto(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CGLViewCtrlCtrl::OnCameraFollowObject() 
{
   if (view->Select(m_rlastPoint.x,m_rlastPoint.y,m_rlastFlags,GView::FOLLOW_OBJECT)) {
		Redraw();
   }
}


void CGLViewCtrlCtrl::OnCameraRelative() 
{
	SetCameraRelativeMode(GetCameraRelativeMode() == 0 ? 1 : 0);
}

void CGLViewCtrlCtrl::OnUpdateCameraRelative(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetCameraRelativeMode());
}

void CGLViewCtrlCtrl::OnCameraHeadlight() 
{
	view->SetHeadLight(!view->GetHeadLight());
	Redraw();
}

void CGLViewCtrlCtrl::OnUpdateCameraHeadlight(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(view->GetHeadLight());
}

void CGLViewCtrlCtrl::OnCameraCollision() 
{
	if (!view) return;
	view->collisionDetection = !  view->collisionDetection;
	Message(CString("Collision detection is now ") + ((view->collisionDetection) ? "On" : "Off") );

}

void CGLViewCtrlCtrl::OnUpdateCameraCollision(CCmdUI* pCmdUI) 
{
	if (!view) return;
	pCmdUI->SetCheck(view->collisionDetection);	
}


void CGLViewCtrlCtrl::OnCameraZoomout() 
{
	view->CameraReset();
	
	Redraw();

}


void CGLViewCtrlCtrl::OnSettingsStayOnGround() 
{
	if (!view) return;
	view->stayOnGround = !view->stayOnGround;
	Message(CString("Stay on ground is now ") + ((view->stayOnGround) ? "On" : "Off") );
}

void CGLViewCtrlCtrl::OnUpdateSettingsStayOnGround(CCmdUI* pCmdUI) 
{
	if (!view) return;
	pCmdUI->SetCheck(view->stayOnGround);
}


void CGLViewCtrlCtrl::OnCameraGravity() 
{
	OnSettingsStayOnGround();	
}

void CGLViewCtrlCtrl::OnUpdateCameraGravity(CCmdUI* pCmdUI) 
{
	if (!view) return;
	pCmdUI->SetCheck(view->stayOnGround);
	
}

void CGLViewCtrlCtrl::OnCameraStraightenUp() 
{
	NavStraightenUp();
}


void CGLViewCtrlCtrl::OnCameraViewpointNext() 
{
    view->SetNextViewpoint(view->cameraAnimate);
	Redraw();
}

void CGLViewCtrlCtrl::OnUpdateCameraViewpointNext(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CGLViewCtrlCtrl::OnCameraViewpointPrev() 
{
	view->SetPrevViewpoint(view->cameraAnimate);
	Redraw();
}

void CGLViewCtrlCtrl::OnUpdateCameraViewpointPrev(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}


void CGLViewCtrlCtrl::OnCameraViewpoint(UINT id) 
{
	int i= id - ID_VIEWPOINT_0; 
    view->SetViewpoint(i,view->cameraAnimate);
	if (view->camera->Changed()) Redraw();
	
	if (view->cameraAnimate && !m_timerRunning) 
		StartTimer();

}


void CGLViewCtrlCtrl::OnUpdateCameraViewpoint(CCmdUI* pCmdUI) 
{
	int i= pCmdUI->m_nID - ID_VIEWPOINT_0; 
	int index;
	view->GetViewpointIndex(index);
	pCmdUI->SetCheck(i == index );
}



void CGLViewCtrlCtrl::OnCameraWalk() 
{
	SetCameraMode(WALK_MODE);
	SetCursorMode(NAV_WALK);
	NavStraightenUp();
	Message("Walking: Keys S : Slide, R : Rotate, J : jump to, space : Follow",PROGRESS_MESSAGE);

}

void CGLViewCtrlCtrl::OnUpdateCameraWalk(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetCameraMode() == WALK_MODE);
}

void CGLViewCtrlCtrl::OnCameraZoom() 
{
	SetCameraMode(ZOOM_MODE);
}

void CGLViewCtrlCtrl::OnUpdateCameraZoom(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetCameraMode() == ZOOM_MODE);
}

//
// Rendering switches 
//


void CGLViewCtrlCtrl::OnRenderVertices() 
{
	 view->SetRenderMode(GRenderVertices);
	 Redraw();
}

void CGLViewCtrlCtrl::OnUpdateRenderVertices(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(view->GetRenderMode() == GRenderVertices);
}


void CGLViewCtrlCtrl::OnRenderWireframe() 
{
	 view->SetRenderMode(GRenderWireframe);
	 Redraw();
}

void CGLViewCtrlCtrl::OnUpdateRenderWireframe(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(view->GetRenderMode() == GRenderWireframe);
}


void CGLViewCtrlCtrl::OnRenderFlat() 
{
	 view->SetRenderMode(GRenderFlat);
	 Redraw();
}

void CGLViewCtrlCtrl::OnUpdateRenderFlat(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(view->GetRenderMode() == GRenderFlat);
}

void CGLViewCtrlCtrl::OnRenderGouraud() 
{
	 view->SetRenderMode(GRenderGouraud);
	 Redraw();
}

void CGLViewCtrlCtrl::OnUpdateRenderGouraud(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(view->GetRenderMode() == GRenderGouraud);
}


void CGLViewCtrlCtrl::OnRenderNonlighted() 
{
   view->SetRenderMode(GRenderNonLighted);
   Redraw();
}

void CGLViewCtrlCtrl::OnUpdateRenderNonlighted(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(view->GetRenderMode() == GRenderNonLighted);
}

//
//  Options 
//


void CGLViewCtrlCtrl::OnOptionsDriver(UINT id) 
{
	int i= id - ID_DRIVER_0; 
#ifdef _D3D

	view->device->bOnlySystemMemory = GetSysMemFlag();
	if (view->device->bInitialized) {

	// clean that up 

	if (view->device->ChangeDriver(i,0)) {
		if (view->device->IsHardwareDriver()) {

			CString sceneUrl;
			if (view) sceneUrl = view->GetUrl();
			else sceneUrl = m_initialUrl;

			SetProfile(_T("Direct3D.driverOk"),0);				
			SetProfile(_T("Direct3D.driverOkUrl"),sceneUrl);				
			m_checkFrames=0; // redo checking 
		}
		else SetProfile(_T("Direct3D.driverOk"),1);				

	} else {
		 // couldn´t handle, fallback to software 			
		if (view->device->CurrDriver != view->device->SoftwareDriver)		
        if (!view->device->ChangeDriver(view->device->SoftwareDriver,NULL)) {
				view->ReportD3DError();
		} else {
				view->ReportD3DError();
		}
	}
	Redraw();
	}

#endif

}

void CGLViewCtrlCtrl::OnUpdateOptionsDriver(CCmdUI* pCmdUI) 
{
#ifdef _D3D
	int i= pCmdUI->m_nID - ID_DRIVER_0; 
	int index=view->device->CurrDriver;
	pCmdUI->SetCheck(i == index );

	if (!view->device->Driver[i].bCanDoWindow)
		pCmdUI->Enable(FALSE);


#endif
}




//
//  OCX Browser Interfaces 
//




GvNode * GetNode(LPUNKNOWN node)
{

   Node *result=NULL;
   node->QueryInterface(IID_Node, (void **)&result);
   if (!result) return NULL;

   result->Release();

   // is this ok ? 
   return (GvNode *) result;
}

void CGLViewCtrlCtrl::OnRenderTextured() 
{

#ifdef _DEBUG_TEST_CODE 
   LPUNKNOWN node = createNode("Sphere");

   Node *result;
   node->QueryInterface(IID_Node, (void **)&result);

   BSTR res=NULL;
   result->getType(&res);
  
   CString s(res);
   ::SysFreeString(res);
   
   TRACE("node  type is %s \n",(const char *) s);


   Field *radius = NULL;


   CComBSTR radiusS("radius");

   result->getField( radiusS, &radius);

   if (radius) { 
		int ret;
		radius->getType(&ret);
	    
		EventInSFFloat *value = NULL;

		radius->QueryInterface(IID_EventInSFFloat, (void **)&value);

		if (value) {
			value->setValue(2.0);
			value->Release();
		}


		radius->Release();
	}	

   result->Release();

   node->Release();
   {
	    //const char *t ="DEF scene Transform { children [ Sphere {} ] } ";
        CString t;
		t = "DEF AvatarList Group { }\n";
		t += "DEF Position ProximitySensor  { size 1e30 1e30 1e30 }\n";
		t += "DEF Transporter Viewpoint { }";

	    LPUNKNOWN node = createVrmlFromString(t);

		if (node) replaceWorld(node);

		//Node *result;
		//node->QueryInterface(IID_Node, (void **)&result);
		
   }

#endif 

   view->ToggleTexturing();
   Redraw();
}

void CGLViewCtrlCtrl::OnUpdateRenderTextured(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(view->GetTexturing());
}



void CGLViewCtrlCtrl::OnNextViewpoint() 
{
	if (!view) return;
    int ret = view->SetNextViewpoint(1);
}

void CGLViewCtrlCtrl::OnPrevViewpoint() 
{
	// TODO: Add your dispatch handler code here
	if (!view) return;
    int ret = view->SetPrevViewpoint(1);
}

BSTR CGLViewCtrlCtrl::GetRenderMode() 
{
	CString strResult;
	
	strResult = GRenderModeLookup(view->GetRenderMode());

	return strResult.AllocSysString();
}

void CGLViewCtrlCtrl::SetRenderMode(LPCTSTR lpszNewValue) 
{
  	CString tmp(lpszNewValue);
	tmp.MakeLower();
    GRenderMode mode = (GRenderMode) GRenderModeLookup(tmp);
	view->SetRenderMode(mode);

	Redraw();
	
	//SetModifiedFlag();
}


// Browser EAI interface

// return browser name: GLView
BSTR CGLViewCtrlCtrl::getName() 
{
	CString strResult;

	strResult = _PROGRAM;

	return strResult.AllocSysString();
}

// return browser version: 3.03
BSTR CGLViewCtrlCtrl::getVersion() 
{
	CString strResult;

	strResult = _VERSION;
#ifdef _D3D
	strResult +=" Direct3D";
#endif
#ifdef _OGL
	strResult +=" OpenGL";
#endif

	strResult +=" ";
	strResult += __DATE__ ;

#ifdef _DEBUG
	strResult += " DEBUG";
#endif

	return strResult.AllocSysString();
}

float CGLViewCtrlCtrl::getCurrentSpeed() 
{
	if (!view) return 0.0;
	return view->viewStepSpeedLocal;
}

float CGLViewCtrlCtrl::getCurrentFrameRate() 
{
	if (!view) return 0.0;
	return view->getCurrentFrameRate();
}

// get URL of current scene 
BSTR CGLViewCtrlCtrl::getWorldURL() 
{
	EAI_CALL("getWorldURL")

	CString strResult;

	if (view) strResult = view->GetUrl();
	else strResult = m_initialUrl;

	return strResult.AllocSysString();
}


// replace world by node
// node probably is topLevel scene node created by createVrmlFromString
// otherwise a new scene containing node is created
// note : no bounding is currently done !
// no naming is done 
void CGLViewCtrlCtrl::replaceWorld(LPUNKNOWN node) 
{
	EAI_CALL("replaceWorld")

	if (!view) return;

	// get Node interface 
	Node *result=NULL;
	node->QueryInterface(IID_Node, (void **)&result);
	if (!result) return;

	// get the pointer to native GLView node 
	GvNode *n = NULL;
	result->getNative((long *) &n);

	if (n) {
		view->SetNode(n);
		// check if observer wants OnSceneChanged
		if (view->observerFlags & GOBSERVE_MESSAGE) {
				view->observer->OnSceneChanged((BSTR) NULL);
		}

		// to do: bind initial nodes ??? 
		if (updateLock == 0) Redraw();
	}
	result->Release();

}

// load the url given, parameter could be "target=frameName"
// EAI url and parameter are MF´s !!
void CGLViewCtrlCtrl::loadURL(LPCTSTR url, LPCTSTR parameter) 
{
	EAI_CALL("loadURL")

	if (!view) {
		m_initialUrl = url;
		return;
	}
	if (url[0] == '#')  {
		view->SetCamera(&url[1]);
		return;
	}

	CString absoluteUrl;
	CreateAbsolutePath(view->GetScene()->GetHomeUrl(),url,absoluteUrl);

	const char *target = NULL;
	const char *location = NULL;

	if (strlen(parameter)>0) {
			const char *targetPrefix = "target=";
			const int targetPrefixLen = strlen(targetPrefix);

			// check for target option 
			const char *opt = parameter;
			if (strncmp(opt, targetPrefix,targetPrefixLen) == 0) {
						target = &opt[targetPrefixLen];
			}
	}

	NavigateToUrl(absoluteUrl,location,target);

}

void CGLViewCtrlCtrl::setDescription(LPCTSTR description) 
{
	// could be title ? 
	Message(description);
}

// create Vrml from String
// returns Scene Pseudo Node containings nodes
LPUNKNOWN CGLViewCtrlCtrl::createVrmlFromString(LPCTSTR vrmlSyntax) 
{
	EAI_CALL("createVrmlFromString")

	if (!view)  {
		Reporter.Error("View not initialized\n");
		return NULL;
	}
    
	if (!view) return FALSE;

	GvScene * scene =  view->GetScene();
    GvScene* newScene = NULL;
    if (scene == NULL)  {  // no scene yet 
        scene = new GvScene();

        if (!scene) return NULL;
		scene->setBrowser(view);

        scene->ref();
        newScene = scene->createVrmlFromString((char *)vrmlSyntax,m_reporter);
        scene->unref(); scene = NULL;

    } 	// create a new scene from string 
	else newScene = scene->createVrmlFromString((char *)vrmlSyntax,m_reporter);

	if (!newScene) return NULL;

	IUnknown* result=NULL;
	newScene->QueryInterface(IID_IUnknown, (void **)&result);
	return result;

}

// create vrml from URL 

// EAI url is MFString, no waz  
void CGLViewCtrlCtrl::createVrmlFromURL(LPCTSTR url, LPDISPATCH nodeDP, LPCTSTR event) 
{

	EAI_CALL("createVrmlFromURL")

//	CWaitCursor wait;
	CString msg;

	if (!view) return;

	GvScene * scene =  view->GetScene();
	if (!scene) return;

/*
	Translate(_T("Loading"),msg);

    msg += "\"";
    msg += url;
    msg+="\" ...";


	Message(msg,PROGRESS_MESSAGE);
	EAI_TRACE(msg);

*/


	// setup createVrmlFromUrl pseudo node 
	Gv2CreateVrmlFromUrl *vrmlFromUrl = new Gv2CreateVrmlFromUrl();
	vrmlFromUrl->setBrowser(view);
	vrmlFromUrl->ref();

	vrmlFromUrl->url.set_(url);

	if (nodeDP) {
		// get Node interface 
		Node *nodeI=NULL;
		nodeDP->QueryInterface(IID_Node, (void **)&nodeI);
    
		// get the pointer to native GLView node 
		GvNode *node = NULL;
		if (nodeI) {
			nodeI->getNative((long *) &node);
			nodeI->Release();
		}
		if (node) { // set node and get eventIn Index 
			 vrmlFromUrl->node.set(node);
			 vrmlFromUrl->nodeEvent.set(node->getFieldData()->getEventInIndex(node,event));
		}
	}

	// for resolving relative urls 
	GLoadInlinesTraversal traversal;
	traversal.resolveRelativeUrls = gtrue;

	traversal.view = view;
	
	// process load request 
	vrmlFromUrl->Load(traversal);
	
	// get status 
	GFileState status = vrmlFromUrl->loader.GetState();

	switch (status) {
	case S_NOT_LOADED :	 // to many threads, wait 
	case S_LOADING :
		 // will be processed when URL ready 
		TRACE("CreateVrmlFromUrl: %s started %d \n",(const char *) url,status);
		scene->addCreateVrmlFromUrl(vrmlFromUrl); 
		break; 

	default : 		
		// data loaded or error, send event 
		TRACE("CreateVrmlFromUrl: %s done %d \n",(const char *) url,status);
		// status == S_PROCESSED if all went well, anyway send NULL children event even if error
		vrmlFromUrl->SendEvent();
		view->UpdateSceneInfo(); // probably sensor stuff added 
		break;

	}

	vrmlFromUrl->unref();
	vrmlFromUrl = NULL;
}

// get node of current scene by name
// nodes added from createVrmlFromString, createVrmlFromUrl are not available !
LPUNKNOWN CGLViewCtrlCtrl::getNode(LPCTSTR name) 
{
	EAI_CALL("getNode")

	if (!view) return NULL;
	GvScene * scene =  view->GetScene();
	if (!scene) return NULL;

	// find the node
	GvNode *node = scene->getNode(name);

	// insert Scene Group for Passport if not loading yet 
	if (node == NULL && (strcmp(name,"Scene") == 0) && mainLoader == NULL) {
		ViewLock viewLock(view);
		GvNode *n = new Gv2Group();
		n->setName("Scene");
		scene->addNode(n);
		node = n;
	}

	if (!node) return NULL;

	IUnknown* result=NULL;
	node->QueryInterface(IID_IUnknown, (void **)&result);

	return result;
}

// add a ROUTE
void CGLViewCtrlCtrl::addRoute(LPUNKNOWN fromNodeU, LPCTSTR fromEventOut, LPUNKNOWN toNodeU, LPCTSTR toEventIn) 
{
	EAI_CALL("addRoute")

	if (!view) return;
	ViewLock viewLock(view);

	GvNode *fromNode=NULL;
	GvNode *toNode=NULL;

	// get nodes 
	if (fromNodeU) { // get Node interface 
		Node *nodeI=NULL;
		fromNodeU->QueryInterface(IID_Node, (void **)&nodeI);
		// get the pointer to native GLView node 
		if (nodeI) {
			nodeI->getNative((long *) &fromNode);
			nodeI->Release();
		}
	}
	if (toNodeU) { 	// get Node interface 
		Node *nodeI=NULL;
		toNodeU->QueryInterface(IID_Node, (void **)&nodeI);
		// get the pointer to native GLView node 
		if (nodeI) {
			nodeI->getNative((long *) &toNode);
			nodeI->Release();
		}
	}

	if (!fromNode || !toNode) return; //ERROR

	view->addRoute(fromNode,fromEventOut,toNode,toEventIn);

}

// delete a ROUTE
void CGLViewCtrlCtrl::deleteRoute(LPUNKNOWN fromNodeU, LPCTSTR fromEventOut, LPUNKNOWN toNodeU, LPCTSTR toEventIn) 
{
	EAI_CALL("deleteRoute")

	if (!view) return;
	
	ViewLock viewLock(view);


	GvNode *fromNode=NULL;
	GvNode *toNode=NULL;

	// get nodes 
	if (fromNodeU) { // get Node interface 
		Node *nodeI=NULL;
		fromNodeU->QueryInterface(IID_Node, (void **)&nodeI);
		// get the pointer to native GLView node 
		if (nodeI) {
			nodeI->getNative((long *) &fromNode);
			nodeI->Release();
		}
	}
	if (toNodeU) { 	// get Node interface 
		Node *nodeI=NULL;
		toNodeU->QueryInterface(IID_Node, (void **)&nodeI);
		// get the pointer to native GLView node 
		if (nodeI) {
			nodeI->getNative((long *) &toNode);
			nodeI->Release();
		}
	}

	if (!fromNode || !toNode) return; //ERROR

	view->deleteRoute(fromNode,fromEventOut,toNode,toEventIn);


}

// create a new node of type nodeClass (could be name of PROTO/EXTERNPROTO)
LPUNKNOWN CGLViewCtrlCtrl::createNode(LPCTSTR nodeclass) 
{
	EAI_CALL("createNode")

	ViewLock viewLock(view); 

	const char * name = nodeclass;
	GvScene * scene =  view->GetScene();
    GvNode *node=NULL; 
	
	if (scene == NULL) 
        node = GvNode::createInstanceFromName(name,1);
    else 
	    node = scene->createNodeInstance(name);
 
	if (node == NULL) {
		return NULL;
	}
	else { 
		node->setBrowser(view);

		IUnknown* result=NULL;
		node->QueryInterface(IID_IUnknown, (void **)&result);
		return result;
	}
}


// add a new toplevel node to scene
void CGLViewCtrlCtrl::addNode(LPUNKNOWN node) 
{
	EAI_CALL("addNode")

    if (node == NULL) return;
	
	ViewLock viewLock(view); 

	// get Node interface 
	Node *result=NULL;
	node->QueryInterface(IID_Node, (void **)&result);
	if (!result) return;
    
	// get the pointer to native GLView node 
	GvNode *n = NULL;
	result->getNative((long *) &n);

	if (n && view) {
		GvScene * s = view->GetScene();
		if (s) { 
			s->addNode(n);
		    view->doUpdateSceneInfo++;
        } else {
			// no scene yet, create one 
			s = new GvScene();
			s->setBrowser(view);
			s->addNode(n);
			view->SetScene(s);
		}
	}
	result->Release();
}

// remove a toplevel node from scene
BOOL CGLViewCtrlCtrl::removeNode(LPUNKNOWN node) 
{
	EAI_CALL("removeNode")

	// get Node interface 
	BOOL ret = FALSE;

    if (node == NULL) return ret;
    ASSERT(view);
	if (view == NULL) return ret;

	ViewLock viewLock(view); 

	Node *result=NULL;

	node->QueryInterface(IID_Node, (void **)&result);

	if (!result) return FALSE;

	// get the pointer to native GLView node 
	GvNode *n = NULL;
	result->getNative((long *) &n);

	if (n && view ) {
	    if (view->GetScene()->removeNode(n)) {
			view->doUpdateSceneInfo++;
		}

	}
	result->Release();
	return ret;
}


DWORD CGLViewCtrlCtrl::GetControlFlags() 
{
	// TODO: Add your specialized code here and/or call the base class
	//pointerInactive
	return COleControl::GetControlFlags(); // 
}

BOOL CGLViewCtrlCtrl::OnSetObjectRects(LPCRECT lpRectPos, LPCRECT lpRectClip) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return COleControl::OnSetObjectRects(lpRectPos, lpRectClip);
}

void CGLViewCtrlCtrl::OnActivateApp(BOOL bActive, HTASK hTask) 
{
	TRACE("CGLViewCtrlCtrl::OnActivateApp %d %p \n",bActive, hTask);

	COleControl::OnActivateApp(bActive, hTask);
	
	// TODO: Add your message handler code here
	
}

void CGLViewCtrlCtrl::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: Add your message handler code here and/or call default
	TRACE("CGLViewCtrlCtrl::On Drop Files .... ");
	
	COleControl::OnDropFiles(hDropInfo);
}


void CGLViewCtrlCtrl::OnEnable(BOOL bEnable) 
{
	TRACE("CGLViewCtrlCtrl::OnEnable(%d) \n",bEnable);

	COleControl::OnEnable(bEnable);
	
	// TODO: Add your message handler code here
}

void CGLViewCtrlCtrl::OnEnterIdle(UINT nWhy, CWnd* pWho) 
{
	TRACE("CGLViewCtrlCtrl::OnEnterIdle(%d %p) \n",nWhy,pWho);

	COleControl::OnEnterIdle(nWhy, pWho);
	
	// TODO: Add your message handler code here
	
}

void CGLViewCtrlCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	COleControl::OnSetFocus(pOldWnd);
	
	TRACE("CGLViewCtrlCtrl::OnSetFocus(%p) \n",pOldWnd);
	// TODO: Add your message handler code here

	m_NavFocus = TRUE;
	
}


void CGLViewCtrlCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	TRACE("CGLViewCtrlCtrl::OnKillFocus(%p) \n",pNewWnd);

	// got focus 
	if (pNewWnd != this)
	{
		// hg temp test SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		m_NavCursorModeOld = -2;
		m_NavFocus = FALSE;
		m_NavNotActive = TRUE;
	}
	else {
		m_NavNotActive = FALSE;
		m_NavFocus = TRUE;
	}


	COleControl::OnKillFocus(pNewWnd); 
	
	// TODO: Add your message handler code here
	
}

void CGLViewCtrlCtrl::OnPaletteChanged(CWnd* pFocusWnd) 
{
	TRACE("CGLViewCtrlCtrl::OnPaletteChanged  %d \n",pFocusWnd != this);
	//	COleControl::OnPaletteChanged(pFocusWnd);
    // See if the change was caused by us and ignore it if not.
    if (pFocusWnd != this) {
        //OnQueryNewPalette();
    }	
}

void CGLViewCtrlCtrl::OnPaletteIsChanging(CWnd* pRealizeWnd) 
{
	TRACE("CGLViewCtrlCtrl::OnPaletteIsChanging \n");
//	COleControl::OnPaletteIsChanging(pRealizeWnd);
	
	// TODO: Add your message handler code here
	
}

BOOL CGLViewCtrlCtrl::OnQueryNewPalette() 
{
	TRACE("Query on New Palette \n");
	if (view) {
#ifdef _OGL
/* 
	CPalette* pPal = CPalette::FromHandle(m_glptr->m_hPalette); //GetPalette() ;
    if (pPal) {   	
        CDC* pDC = GetDC();
        pDC->SelectPalette(pPal, FALSE);
        UINT u = pDC->RealizePalette();
        ReleaseDC(pDC);
    }
*/

	view->device->SetPalette();
	return TRUE;
#endif
#ifdef _D3D
	view->device->SetPalette();
	return TRUE;
#endif
	}
	return COleControl::OnQueryNewPalette();
}

void CGLViewCtrlCtrl::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	
	COleControl::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	
}

/*
STDMETHODIMP COleControl::XOleObject::SetMoniker(DWORD, LPMONIKER)
	OLEWHICHMK_CONTAINER    = 1, 
    OLEWHICHMK_OBJREL       = 2, 
    OLEWHICHMK_OBJFULL 
STDMETHODIMP COleControl::XOleObject::GetMoniker(DWORD, DWORD, LPMONIKER*)
{
	return E_NOTIMPL;
}

STDMETHODIMP COleControl::XOleObject::InitFromData(LPDATAOBJECT, BOOL, DWORD)
{
	return E_NOTIMPL;
}

STDMETHODIMP COleControl::XOleObject::GetClipboardData(DWORD, LPDATAOBJECT*)
{
  IMoniker *pMoniker;
  HRESULT result;
  LPOLESTR url=NULL;

  if (pOleClientSite->GetMoniker(OLEGETMONIKER_TEMPFORUSER,OLEWHICHMK_CONTAINER,&pMoniker) == S_OK)
  {
	result = pMoniker->GetDisplayName(NULL,	// IBindCtx *pbc Pointer to bind context to be used
				NULL,	// IMoniker *pmkToLeft,Pointer to moniker to the left in the composite
				LPOLESTR *ppszDisplayName	//Indirect pointer to the display name
   );	
	pMoniker->Release();
  }


HRESULT pMoniker->GetDisplayName(
    IBindCtx *pbc,	//Pointer to bind context to be used
    IMoniker *pmkToLeft,	//Pointer to moniker to the left in the composite
    LPOLESTR *ppszDisplayName	//Indirect pointer to the display name
   );	
 


*/

#if 1

// IPersistMoniker

STDMETHODIMP_(ULONG) CGLViewCtrlCtrl::XPersistMoniker::AddRef()
{
	METHOD_PROLOGUE_EX_(CGLViewCtrlCtrl, PersistMoniker)
	return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CGLViewCtrlCtrl::XPersistMoniker::Release()
{
	METHOD_PROLOGUE_EX_(CGLViewCtrlCtrl, PersistMoniker)
	return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP CGLViewCtrlCtrl::XPersistMoniker::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE_EX_(CGLViewCtrlCtrl, PersistMoniker)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}



STDMETHODIMP CGLViewCtrlCtrl::XPersistMoniker::GetClassID(CLSID * clsid)
{
	return E_NOTIMPL;
}


STDMETHODIMP CGLViewCtrlCtrl::XPersistMoniker::IsDirty(void)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGLViewCtrlCtrl::XPersistMoniker::Load(BOOL fFullyAvailable, 
		IMoniker* pmkSrc,  IBindCtx* pbc,  DWORD grfMode)
{

	HRESULT result;
	LPOLESTR container=NULL;

	result = pmkSrc->GetDisplayName(NULL,	NULL,&container);

	CString home;
	home = container;
	if (container) ::SysFreeString(container);

	
	
	return E_NOTIMPL;
}

STDMETHODIMP CGLViewCtrlCtrl::XPersistMoniker::Save(IMoniker* pmkDst,  IBindCtx* pbc,  BOOL fRemember)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGLViewCtrlCtrl::XPersistMoniker::SaveCompleted(IMoniker* pmkNew, IBindCtx* pbc)
{
	return E_NOTIMPL;
}

	
STDMETHODIMP CGLViewCtrlCtrl::XPersistMoniker::GetCurMoniker(IMoniker** ppmkCur)
{
	return E_NOTIMPL;

#if 0
	LPCTSTR types[] = { TEXT("x-world/x-vrml"), TEXT("model/vrml") };
	CLIPFORMAT cfTypes[2];
	RegisterMediaTypes(2,types,cfTypes);
#endif
}

#endif

#if 0

urlmon.h

STDAPI RegisterMediaTypes(UINT ctypes, const LPCSTR* rgszTypes, CLIPFORMAT* rgcfTypes);            
STDAPI FindMediaType(LPCSTR rgszTypes, CLIPFORMAT* rgcfTypes);                                       
STDAPI CreateFormatEnumerator( UINT cfmtetc, FORMATETC* rgfmtetc, IEnumFORMATETC** ppenumfmtetc); 
STDAPI RegisterFormatEnumerator(LPBC pBC, IEnumFORMATETC *pEFetc, DWORD reserved);          
STDAPI RevokeFormatEnumerator(LPBC pBC, IEnumFORMATETC *pEFetc);                            
STDAPI RegisterMediaTypeClass(LPBC pBC,UINT ctypes, const LPCSTR* rgszTypes, CLSID *rgclsID, DWORD reserved);    
STDAPI FindMediaTypeClass(LPBC pBC, LPCSTR szType, CLSID *pclsID, DWORD reserved);                          
STDAPI UrlMkSetSessionOption(DWORD dwOption, LPVOID pBuffer, DWORD dwBufferLength, DWORD dwReserved);       

yield
Registers a mapping of media types to CLSIDs to override the default mapping when binding objects in the specified bind context.
HRESULT RegisterMediaTypeClass(
    LPBC pbc,	//Pointer to the bind context
    UINT ctypes,	//Number of media type strings in rgszTypes
    LPCSTR  *rgszTypes,	//Pointer to array of media types to be registered
    CLSID  *rgclsID,	//Pointer to array of CLSIDs corresponding to rgszTypes
    DWORD  dwReserved	//Reserved for future use; must be zero
   );	
 
Parameters
pbc
[in] Pointer to the IBindCtx interface for the bind context in which the media types are to be registered.
ctypes
[in] Number of media type strings in the rgszTypes array. This parameter cannot be zero.
rgszTypes
[in] Pointer to an array of strings identifying the media types to be registered. None of these strings can be NULL.
rgclsID
[in] Pointer to an array of CLSIDs corresponding to the media type strings in the rgszTypes array.
dwReserved
[in] Reserved for future use; must be zero.
 
Return Values
S_OK 
The operation was successful.
E_INVALIDARG
One or more parameters are invalid.
 
Remarks
This function is primarily used by moniker clients (for example, web browsers) to override the default registry mapping between file types and CLSIDs. This function creates an object that is registered on the bind context and is used by monikers when choosing the class to instantiate as a result of an IMoniker::BindToObject operation.
See Also
RegisterMediaTypes 


STDAPI CreateURLMoniker(LPMONIKER pMkCtx, LPCWSTR szURL, LPMONIKER FAR * ppmk);             
STDAPI GetClassURL(LPCWSTR szURL, CLSID *pClsID);                                           
STDAPI CreateAsyncBindCtx(DWORD reserved, IBindStatusCallback *pBSCb,                       
                                IEnumFORMATETC *pEFetc, IBindCtx **ppBC);                   
STDAPI MkParseDisplayNameEx(IBindCtx *pbc, LPCWSTR szDisplayName, ULONG *pchEaten,          
                                LPMONIKER *ppmk);                                           
STDAPI RegisterBindStatusCallback(LPBC pBC, IBindStatusCallback *pBSCb,                     
                                IBindStatusCallback**  ppBSCBPrev, DWORD dwReserved);       
STDAPI RevokeBindStatusCallback(LPBC pBC, IBindStatusCallback *pBSCb);                      
STDAPI GetClassFileOrMime(LPBC pBC, LPCWSTR szFilename, LPVOID pBuffer, DWORD cbSize, LPCWSTR szMime, DWORD dwReserved, CLSID *pclsid); 
STDAPI IsValidURL(LPBC pBC, LPCWSTR szURL, DWORD dwReserved);                               
STDAPI CoGetClassObjectFromURL( REFCLSID rCLASSID,
            LPCWSTR szCODE, DWORD dwFileVersionMS, 
            DWORD dwFileVersionLS, LPCWSTR szTYPE,
            LPBINDCTX pBindCtx, DWORD dwClsContext,
            LPVOID pvReserved, REFIID riid, LPVOID * ppv);
 

#endif


BSTR CGLViewCtrlCtrl::GetNavigationMode() 
{
	CString strResult;
	strResult = GetCameraModeString(GetCameraMode());
    strResult.MakeUpper();
	return strResult.AllocSysString();
}


void CGLViewCtrlCtrl::SetNavigationMode(LPCTSTR lpszNewValue) 
{
	CString tmp = lpszNewValue;
    tmp.MakeLower();
    SetCameraMode(tmp);
}

short CGLViewCtrlCtrl::GetHeadlight() 
{
	if (!view) return 1;
	return view->GetHeadLight();
}

void CGLViewCtrlCtrl::SetHeadlight(short nNewValue) 
{
	if (view) 
		view->SetHeadLight (nNewValue ? TRUE : FALSE);
}

float CGLViewCtrlCtrl::GetWalkSpeed() 
{
	if (view) return view->viewStepSpeedLocal;
	else return 0.0f;
}

void CGLViewCtrlCtrl::SetWalkSpeed(float newValue) 
{
	if (!view || newValue <0) return;
	view->viewStepSpeedLocal=newValue;
	view->UpdateGlobalNavigationValues();
}

// get top level scene node of node type scene
LPDISPATCH CGLViewCtrlCtrl::getWorld() 
{
	if (!view) return NULL;
	GvScene *scene =  view->GetScene();
	if (!scene) return NULL;
	IDispatch *result=NULL;
	scene->QueryInterface(IID_IDispatch, (void **)&result);
	return result;
}

// lock update and animation activity
void CGLViewCtrlCtrl::beginUpdate() 
{
	if ( updateLock == 0 && m_initialized
		&& (m_currentMode == NULL_MODE)	) 
	{
		if (m_useRenderThread) {
			m_renderThread->PauseThread();
		} else  StopTimer();
	}
	updateLock ++;
	//view->Lock();

	if (view) {
		view->SetTheTime(); // set global VRML eventTime
	}
	// TRACE("beginUpdate ( %d",updateLock);
}

// unlock update and animation activity
void CGLViewCtrlCtrl::endUpdate() 
{
	updateLock--;
	ASSERT(updateLock >=0);
    if ( updateLock < 0 ) 
        updateLock = 0;
	//else view->Unlock();

    if (updateLock == 0 && m_initialized) {
		if (m_useRenderThread) {
			m_renderThread->UnpauseThread();
		} else {  
			if (!m_timerRunning) 	StartTimer();
			// OnIdle(1); // check routes / timeSensors, done in next timer // hg test 3/3/98
			OnTimer(0);
		}
	}
	// TRACE("endUpdate ) %d",updateLock);
}

// experiment with rendering in thread 
void CGLViewCtrlCtrl::LockScene()
{
    EnterCriticalSection(&m_sceneSection); 
}

void CGLViewCtrlCtrl::UnlockScene()
{
    LeaveCriticalSection(&m_sceneSection); 

}

UINT CGLViewCtrlCtrl::RenderWorker()
{

	BOOL drawIt;
	BOOL bWait;
	CString msg;


	m_currentState = RUN_STATE;

	CWinThread* thread =AfxGetThread();
	
	//TlsSetValue(TLS_BROWSER,view);
	//TlsSetValue(TLS_REDRAW,0);
	
	//xxxxx GvNode::setBrowser(view);
	view->clearRedraw();

	

	while (m_requestedState != STOP_STATE) 
	{

	LockScene();
	
	// OnTimer(0);
	OnIdle(1);

	//UnlockScene();
	
	//LockScene();

	drawIt = m_mustRedraw || view->getRedraw(); // (((int)TlsGetValue(TLS_REDRAW)) !=0);

	m_mustRedraw = FALSE;

	if (drawIt)	{
	#ifdef _D3D
		// reset position on screen, necessary for Direct3D because OnMove not called
		view->OnMove(0,0); 
	#endif
    TRACE("\nRenderOnDraw() %p\n",this);

	bWait = FALSE;

	if (view->updateIsComplex) 
    {
		bWait = TRUE;
		BeginWaitCursor();
	}


	// draw 
	view->DrawScene(m_clientRect);

	// enable texture mapping after first update 
	if (view->firstUpdate) 
    {
	   view->firstUpdate = 0;
 	   if (view->initialTextureMode >0) 
       {
	     if (view->GetTexturing() == 0) 
         {
		     view->SetTexturing(1);
			 Redraw();
		 }
	  }
	}
	if (bWait) EndWaitCursor();

	m_endTime= timeGetTime();	// time last rendering cycle stopped

	if (m_displayFrameRate) {
		m_frames ++;

		if ( m_frames == 10) {
			if (m_displayFrameStats) {
				
				//msg.Format("%.2f fps  (%.0d ms) Triangles #%i Triangles/s #%f", ( 10000.0f /  (float) (m_endTime-m_startTime)  ), view->lastFrameTime, view->frameStats.tris, 10000.f*(float)view->frameStats.tris/(float) (m_endTime-m_startTime));

				int tris=0,prims=0,times=0;
				
				for(int i=0; i<NUM_LAST_FRAME_TIMES;i++) { // compute average
					times+=view->lastFrameTimes[i];
					tris+=view->lastFrameTris[i];
					prims+=view->lastFramePrims[i];
				}

				float dt = 0.001 * times / (float) NUM_LAST_FRAME_TIMES;

				float tr = tris / (float) NUM_LAST_FRAME_TIMES;
				float pr = prims / (float) NUM_LAST_FRAME_TIMES;
				if (dt>0.0f)
				  msg.Format("%.2f fps  (%.0d ms) Triangles #%f Triangles/s #%f Primitives #%f", ( 10000.0f /  (float) (m_endTime-m_startTime)  ), view->lastFrameTime, tr, tr/dt,pr);



			}
			else
				msg.Format("%.2f frames per sec  (%.0d ms) \n",  ( 10000.0f /  (float) (m_endTime-m_startTime)  ), view->lastFrameTime);
		
			m_frames = 0;
			m_startTime= timeGetTime(); 
			//Message(msg,PROGRESS_MESSAGE);	
	      ::PostMessage(GetSafeHwnd(),WM_STATUSMESSAGE, (WPARAM) (const char *) msg, (LPARAM) PROGRESS_MESSAGE) ;

	}

	}
	
	} // drawIt

	UnlockScene();

	} // (requestedState != STOP_STATE)

	m_currentState = STOP_STATE;
	return 0;

}

// AFX_THREADPROC

UINT __cdecl CGLViewCtrlCtrlRenderWorker (LPVOID pParam )
{
  CGLViewCtrlCtrl *me = (CGLViewCtrlCtrl *)  pParam;
  return me->RenderWorker();
}

//mk:@ivt:pdwbase/native/sdk/win32/sys/src/prothred_18.htm
BOOL CGLViewCtrlCtrl::StartRenderThread()
{

	CRenderThread *pThread = new CRenderThread(this);
	if (pThread == NULL)
		return FALSE;

	ASSERT_VALID(pThread);

	// Create Thread in a suspended state so we can set the Priority 
	// before it starts getting away from us
	if (!pThread->CreateThread(CREATE_SUSPENDED))
	{
		delete pThread;
		return FALSE;
	}

	// since everything is successful, add the thread to our list
	m_renderThread = pThread;
	TRACE("render thread started %p \n",m_renderThread);

	// If you want to make the sample more sprightly, set the thread priority here 
	// a little higher. It has been set at idle priority to keep from bogging down 
	// other apps that may also be running.
	// VERIFY(pThread->SetThreadPriority(THREAD_PRIORITY_IDLE));
	// Now the thread can run wild
	pThread->ResumeThread();


	return TRUE;
	
/*

	m_requestedState = RUN_STATE;

	CWinThread * m_renderThread = AfxBeginThread(CGLViewCtrlCtrlRenderWorker, this,THREAD_PRIORITY_NORMAL,64*1024);	 	 	

	return m_renderThread != NULL;
*/

}

void CGLViewCtrlCtrl::StopRenderThread()
{
	TRACE("Stopping render thread %dp \n",m_renderThread);
	if (m_renderThread) {
		m_renderThread->KillThread();
		m_renderThread = NULL;
	}
	m_useRenderThread = FALSE;
	m_requestedState = STOP_STATE;
}


BOOL CGLViewCtrlCtrl::GetAnimateAllViewpoints() 
{
	if (!view) return FALSE;
	return (view->AnimatingCamera());
}


// enable/disable animation through all viewpoints
void CGLViewCtrlCtrl::SetAnimateAllViewpoints(BOOL bNewValue) 
{
	if (!view) return;
							
	if (bNewValue) {
	    view->AnimateAllCameras();
		if (!m_timerRunning) StartTimer();

	} else {
		if (view->AnimatingCamera()) 
			view->AnimateCameraStop();
    }
}

// get the name of current viewpoint 
BSTR CGLViewCtrlCtrl::GetViewpoint() 
{
	CString strResult;
	if (view) 
		strResult = view->GetViewpointName();

	return strResult.AllocSysString();
}

// set current viewpoint to viewpoint named lpszNewValue
void CGLViewCtrlCtrl::SetViewpoint(LPCTSTR lpszNewValue) 
{
	if (!view) return;
	view->SetCamera(lpszNewValue);
	if (!m_timerRunning) StartTimer();
}

BSTR CGLViewCtrlCtrl::GetDescription() 
{
	CString strResult;
	// TODO: Add your property handler here

	return strResult.AllocSysString();
}


void CGLViewCtrlCtrl::SetDescription(LPCTSTR lpszNewValue) 
{
	// TODO: Add your property handler here
	Message(lpszNewValue);
}

/*
void CGlViewDataPathProperty::OnStartBinding()
{
}
void CGlViewDataPathProperty::OnStopBinding( HRESULT hresult, LPCTSTR szError )
{
}
void CGlViewDataPathProperty::OnDataAvailable( DWORD dwsize, DWORD bscfFlag )
{
}
*/


//OLE_DATAPATH
BSTR CGLViewCtrlCtrl::GetWorld() 
{
	CString strResult;
	// TODO: Replace "VAR" with the name of a member variable
	//       whose type is derived from CDataPathProperty.

//	 strResult = m_world.GetPath();

	return strResult.AllocSysString();
}

void CGLViewCtrlCtrl::SetWorld(LPCTSTR lpszNewValue) 
{
	TRACE("CGLViewCtrlCtrl::SetWorld(...) %p\n",this);

	// TODO: Replace "VAR" with the name of a member variable
	//       whose type is derived from CDataPathProperty.

//	Load(lpszNewValue, m_world);

	SetModifiedFlag();
}
//IDataObject
BOOL CGLViewCtrlCtrl::OnSetData(LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium,
	BOOL bRelease)
{
	TRACE("CGLViewCtrlCtrl::OnSetData(...) %p\n",this);

	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(lpFormatEtc, sizeof(FORMATETC), FALSE));
	ASSERT(AfxIsValidAddress(lpStgMedium, sizeof(STGMEDIUM)));

	return COleControl::OnSetData(lpFormatEtc,lpStgMedium,bRelease);
/*
	// default implementation supports propset format
	BOOL bSuccess = FALSE;
	CLSID fmtid;
	if (_AfxOleMatchPropsetClipFormat(lpFormatEtc->cfFormat, &fmtid))
	{
		bSuccess = SetPropsetData(lpFormatEtc, lpStgMedium, fmtid);

		if (bSuccess && bRelease)
			ReleaseStgMedium(lpStgMedium);
	}

	return bSuccess;

*/
}


//void CMyDataPathProperty::OnDataAvailable(DWORD dwSize, DWORD grfBSCF)
	
BOOL CGLViewCtrlCtrl::OnRenderFileData(LPFORMATETC lpFormatEtc, CFile* pFile) 
{
	// TODO: Add your specialized code here and/or call the base class
	TRACE("CGLViewCtrlCtrl::OnRenderFileData(...) %p\n",this);
	
	return COleControl::OnRenderFileData(lpFormatEtc, pFile);
}

BOOL CGLViewCtrlCtrl::OnRenderData(LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium) 
{
	// TODO: Add your specialized code here and/or call the base class
	TRACE("CGLViewCtrlCtrl::OnRenderData(...) %p\n",this);
	
	return COleControl::OnRenderData(lpFormatEtc, lpStgMedium);
}

BOOL CGLViewCtrlCtrl::OnRenderGlobalData(LPFORMATETC lpFormatEtc, HGLOBAL* phGlobal) 
{
	// TODO: Add your specialized code here and/or call the base class
	TRACE("CGLViewCtrlCtrl::OnRenderGlobalData(...) %p\n",this);
	
	return COleControl::OnRenderGlobalData(lpFormatEtc, phGlobal);

}
//OLE Controls OLE Controls: Advanced Topics
BOOL CGLViewCtrlCtrl::PreTranslateMessage(MSG* pMsg) 


{
	// http://www.microsoft.com/kb/articles/q168/7/77.htm
	switch (pMsg->message)
      {
         case WM_KEYDOWN:
         case WM_KEYUP:
            switch (pMsg->wParam)
            {
               case VK_UP:
               case VK_DOWN:
               case VK_LEFT:
               case VK_RIGHT:
               case VK_HOME:
               case VK_END:
                  SendMessage (pMsg->message, pMsg->wParam, pMsg->lParam);
                  // Windowless controls won't be able to call SendMessage.
                  // Instead, just respond to the message here.
                  return TRUE;
            }
            break;
      }

	//called from : TranslateAccelerator
	//COleControl::XOleInPlaceActiveObject::TranslateAccelerator(LPMSG lpmsg)
	if (pMsg->message == WM_KEYDOWN) {
		OnKeyDown(pMsg->wParam,LOWORD(pMsg->lParam),HIWORD(pMsg->lParam));
		// RETURN FALSE if not handled 
		return TRUE;
	}
	else if (pMsg->message == WM_KEYUP) {
		OnKeyUp(pMsg->wParam,LOWORD(pMsg->lParam),HIWORD(pMsg->lParam));
		// RETURN FALSE if not handled 
		return TRUE;
	}
/*
	else if (pMsg->message == WM_CHAR) {
		OnChar(pMsg->wParam,LOWORD(pMsg->lParam),HIWORD(pMsg->lParam));
		// RETURN FALSE if not handled 
		return TRUE;
	}
*/
	else if (pMsg->message == WM_SIZING) {
		// OnKeyDown(nChar,
		return TRUE;
	}
	else				  
	if (pMsg->message == WM_KEYDOWN) {
		TRACE("Key Down %d %d \n",(int) pMsg->wParam, (int) pMsg->lParam);
	}
	return COleControl::PreTranslateMessage(pMsg);
}



void CGLViewCtrlCtrl::SetInitialDataFormats()
{
	COleControl::SetInitialDataFormats();

}


long CGLViewCtrlCtrl::setTimerInterval(long interval) 
{
	//  dispatch handler for setTimerInterval
	long ret = m_timerInterval;

	StopTimer();

	if (interval >0) 
		m_timerInterval = interval;

	
	if (m_initialized && updateLock == 0) 
		StartTimer();
	

	return ret;
}

BOOL CGLViewCtrlCtrl::isSceneLoaded() 
{
	if (!m_initialized) return FALSE;
	return (mainLoader == NULL);

}

void CGLViewCtrlCtrl::setNodeName(LPUNKNOWN nodeU, LPCTSTR name) 
{
	EAI_CALL("setNodeName")

	if (!view) return;
	GvScene * scene =  view->GetScene();
	if (!scene) return;

	GvNode *fromNode=NULL;

	// get nodes 
	if (nodeU) { // get Node interface 
		Node *nodeI=NULL;
		nodeU->QueryInterface(IID_Node, (void **)&nodeI);
		// get the pointer to native GLView node 
		if (nodeI) {
			nodeI->getNative((long *) &fromNode);
			nodeI->Release();
		}
	}
	if (fromNode)
		scene->setNodeName(fromNode,name);


}

BSTR CGLViewCtrlCtrl::getNodeName(LPUNKNOWN nodeU) 
{
	CString strResult;
	// TODO: Add your dispatch handler code here
	GvNode *fromNode=NULL;

	// get nodes 
	if (nodeU) { // get Node interface 
		Node *nodeI=NULL;
		nodeU->QueryInterface(IID_Node, (void **)&nodeI);
		// get the pointer to native GLView node 
		if (nodeI) {
			nodeI->getNative((long *) &fromNode);
			nodeI->Release();
		}
	}
	if (fromNode)
		strResult = fromNode->getName().getString();

	
	return strResult.AllocSysString();
}

// get collision detection flag 
BOOL CGLViewCtrlCtrl::GetCollisionDetection() 
{
	if (!view) return 1;
	return view->collisionDetection;

}

// set collision detection flag 
void CGLViewCtrlCtrl::SetCollisionDetection(BOOL bNewValue) 
{
	if (!view) return;
		view->collisionDetection = bNewValue!=0;
}

// search for eventOut on browser 
// can be used to set callback on SFString worldUrl_changed
LPUNKNOWN CGLViewCtrlCtrl::getEventOut(LPCTSTR eventOutName) 
{
	if (!view)  {
		Reporter.Error("View not initialized\n");
		return NULL;
	}
	EventOut *result=NULL;

	CComBSTR name(eventOutName);

	HRESULT res = view->getEventOut(name,&result);

	return(result);
}

// search for eventIn on browser 

LPUNKNOWN CGLViewCtrlCtrl::getEventIn(LPCTSTR eventInName) 
{
	if (!view) {
		Reporter.Error("View not initialized\n");
		return NULL;
	}
	EventIn *result=NULL;

	CComBSTR name(eventInName);

	HRESULT res = view->getEventIn(name,&result);

	return(result);
}

// return the full path to the control
// can be used to find support files 
BSTR CGLViewCtrlCtrl::getControlPathname() 
{
	CString strResult;
	GetModuleFileName(AfxGetInstanceHandle(), strResult.GetBuffer(_MAX_PATH),_MAX_PATH);
	strResult.ReleaseBuffer();
	return strResult.AllocSysString();
}

// show manual 
void CGLViewCtrlCtrl::OnPopupHelpManual() 
{
	CString url;

#if 0
	// get directory name of program 
	CString strPathName;
	GetModuleFileName(AfxGetInstanceHandle( ), strPathName.GetBuffer(_MAX_PATH),_MAX_PATH);
	strPathName.ReleaseBuffer();
	int iBackslash = strPathName.ReverseFind('\\');
	if (iBackslash != -1) 	strPathName = strPathName.Left(iBackslash+1);

	url = "file://"+strPathName+"overview.html";
#else
    // url = URL_HOME "/products/client/cc3d/index.html";
	url = URL_HOME "/products/contact/3d/";
#endif

	Message("Displaying "+url);

//	::NavigateToUrl(NULL,url);
	NavigateToUrl(url,NULL,"_blank",HLNF_OPENINNEWWINDOW);
	
}

// show manual 
void CGLViewCtrlCtrl::OnPopupHelpVisit() 
{
	CString url;
	url = URL_HOME;

	Message("Visiting "+url);

//	::NavigateToUrl(NULL,url);
	NavigateToUrl(url,NULL,"_blank",HLNF_OPENINNEWWINDOW);
	
}


void CGLViewCtrlCtrl::OnPopupHelpCheckversion() 
{
	CString queryString;
	

	//queryString = URL_PRODUCT "/versionCheck.htm?";
	//queryString += _PROGRAM;

    // queryString= URL_HOME "/cgi-bin/products/CC3Dcheck.pl?";
	// new for 4.0
    queryString= URL_HOME "/cgi-bin/products/contactcheck.pl?";

	queryString += "version=";
	queryString += _VERSION;

#ifdef _D3D
	queryString +="&lib=Direct3D";
#endif
#ifdef _OGL
	queryString +="&lib=OpenGL";
#endif

	queryString += "&Date=";
	CString tmp = __DATE__;
#if 0
		ext = tmp.GetBFileName;
		while (*ext) {
		  switch(*ext) {
		  case ' ':	
				 *ext ='_'; 
				 break;
		  default :
		      break;
		  }
		  ext ++;

		}

	relFilePath.ReleaseBuffer();		
#endif


	queryString += tmp ;

	NavigateToUrl(queryString,NULL,"_blank",HLNF_OPENINNEWWINDOW);
	
}

void CGLViewCtrlCtrl::OnPopupHelpAbout() 
{
	
	//DAbout dlgAbout(this);
	DAbout dlgAbout(NULL);

	CString strResult;
	
	strResult = "Version ";
	
	strResult += _VERSION;


#ifdef _D3D
	strResult +=" Direct3D";
#endif
#ifdef _OGL
	strResult +=" OpenGL";
#endif

	strResult +=" ";
	strResult += __DATE__ ;


#ifdef _DEBUG
	strResult += " DEBUG";
#endif

#ifdef _KATMAI		
	if (useKatmai) {
		strResult += " Intel Katmai";
	}

#endif
	CString osName,osVersion;

	GetSystemVersionDisplay(osName,osVersion);

	dlgAbout.m_version = strResult;
	dlgAbout.m_version2 = osName + " " + osVersion;
	dlgAbout.m_version3 = "Installed in : " + view->GetInstallDirectory();
	BOOL checkVersion;

	//StopTimer();
	m_menuActive = TRUE;
	AFX_MANAGE_STATE(_afxModuleAddrThis);
		//checkVersion = dlgAbout.DoModal() == IDC_UPDATE_CHECK;
		checkVersion = DoThreadModal((CDialog*)&dlgAbout) == IDC_UPDATE_CHECK;
	m_menuActive = FALSE;

	//RestartTimer();

	if (checkVersion) 
		OnPopupHelpCheckversion();

}

BOOL CGLViewCtrlCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	//TRACE("OnSetCursor(%p %d %d  \n",pWnd, nHitTest, message);
	if (message == 0) // menue mode 
		return (COleControl::OnSetCursor(pWnd, nHitTest, message));

/*
	if (pWnd != this)
		{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		m_NavNotActive = TRUE;
		}
	else
		m_NavNotActive = FALSE;
*/
//	else
//		return CDialog::OnSetCursor(pWnd, nHitTest, message);


	//if (!COleControl::OnSetCursor(pWnd, nHitTest, message))
//	SetCursor(hCurrentCursor);
	return(TRUE);
}

// WM_MOVING
afx_msg LRESULT CGLViewCtrlCtrl::OnMoving(WPARAM wParam, LPARAM lParam)
{
	TRACE("OnMoving(%d %d) \n",wParam,lParam);
	return(TRUE);
}

//WM_SIZING
afx_msg LRESULT CGLViewCtrlCtrl::OnSizing(WPARAM wParam, LPARAM lParam)
{
	TRACE("OnSizing(%d %d) \n",wParam,lParam);
	return(TRUE);
}

//WM_CAPTURECHANGED
afx_msg LRESULT CGLViewCtrlCtrl::OnCaptureChanged(WPARAM wParam, LPARAM lParam)
{

	//hwndNewCapture = (HWND) lParam;    // handle of window to gain mouse capture 
	TRACE("Capture changed (%d %d) \n",wParam,lParam);

	m_NavLBUTState = FALSE; // HG 

	// to do: kill event handler 
	// set Cursor 
//	if (!m_NavRunning)
//		SetCursorMode(STANDARD_CURSOR);


	// kill current Anchor 
	if (m_currentAnchor) {
		m_currentAnchor->unref();
		m_currentAnchor = NULL;
    }

	m_currentTSensor = FALSE;
	m_currentTDragSensor = FALSE;

	if (view && view->eventHandler)  // new 
	{ 
		 view->PopEventHandler()->unref();
	}
	return(TRUE);
}



void CGLViewCtrlCtrl::OnMnemonic(LPMSG pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	TRACE("OnMnemonic\n");

	COleControl::OnMnemonic(pMsg);
}

void CGLViewCtrlCtrl::OnKeyDownEvent(USHORT nChar, USHORT nShiftState) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	COleControl::OnKeyDownEvent(nChar, nShiftState);
}

// Non-in-place activation overrides 


// called when HTML page redisplays

HRESULT CGLViewCtrlCtrl::OnOpen(BOOL bTryInPlace, LPMSG pMsg)
{
	HRESULT res;

	TRACE("CGLViewCtrlCtrl::OnOpen(%d ) %p\n",bTryInPlace,this);

	res = COleControl::OnOpen(bTryInPlace,pMsg);

	LPUNKNOWN pCC3D = GetInterface(&IID_IUnknown); // (does not AddRef)
    ::SetProp(GetSafeHwnd(), "CC3D_LPUNK", (HANDLE)pCC3D);

	return(res);
}

// called when HTML page changes 
HRESULT CGLViewCtrlCtrl::OnHide()
{
	HRESULT res;

	TRACE("CGLViewCtrlCtrl::OnHide() %p\n",this);

	// send termination message  // 13.12.99 HG
	if (view && view->observerFlags & GOBSERVE_ANCHOR)
		res=view->observer->OnLoadUrl(NULL, NULL);


    ::RemoveProp(GetSafeHwnd(), "CC3D_LPUNK");

	if (m_useRenderThread) 
		StopRenderThread();

	if (m_saveDriverSettings)   // save driver settings
		SaveDriverSettings();
	
	if (m_checkFrames < 10) { // normal exit, but not many redraw's set driveOk flag anyway
		SetProfile(_T("Direct3D.driverOk"),1);				
	}

	
	CloseAllDialogs();

	// destroy console
	if (m_dConsole) { // 13.12.99 hg
		m_dConsole->DestroyWindow(); delete m_dConsole; m_dConsole = NULL; 
	}


	res = COleControl::OnHide();
	EAI_TRACE("CGLViewCtrlCtrl::OnHide() %p\n",this);
	EAI_FLUSH();

	StopTimer();

	if (mainLoader)	 // kill main URL loader
    {
   		mainLoader->Kill();
   		mainLoader->unref();
		mainLoader = NULL;
	}

    KillDownloads();


	// release View   ?? 
	m_initialized = FALSE;

	if (view) 
    {
		view->Term();
		// back pointer not valid
		view->SetView(NULL);	// back pointer
		view->SetViewController(NULL);	// back pointer
		//ASSERT(view->refCnt == 1); // other wise some bad guy keeps a pointer ????
		// problem VRMLSCRIPT browser variable keeps a count 
		view->unref(); // should go to delete view
		view = NULL;
	}


	return(res);
}

// GetOuterWindow();
// pWnd->SetParent(NULL);


void CGLViewCtrlCtrl::OnSettingsStopLoading() 
{
	if (mainLoader)	 // kill main URL loader
    {
   		mainLoader->Kill();
   		mainLoader->unref();
		mainLoader = NULL;
	}


    KillDownloads();

	// what about createVrmlFromUrls ????
}

void CGLViewCtrlCtrl::OnUpdateSettingsStopLoading(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( (GFile::filesInUse != NULL) || (mainLoader!= NULL));
}

void CGLViewCtrlCtrl::OnSettingsReload() 
{
	if (!m_initialized) return ;
	if (view->file->url.GetLength()>0) {
	   ReadUrl(view->file->url,view->file->urlHome,TRUE);
	} else 
	if (view->file->localFile.GetLength()>0) {
	   ReadUrl(view->file->localFile,view->file->localFileHome);
	}
}

void CGLViewCtrlCtrl::OnUpdateSettingsReload(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CGLViewCtrlCtrl::OnAmbientPropertyChange(DISPID dispid) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	COleControl::OnAmbientPropertyChange(dispid);
}

BOOL CGLViewCtrlCtrl::OnEdit(LPMSG lpMsg, HWND hWndParent, LPCRECT lpRect) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return COleControl::OnEdit(lpMsg, hWndParent, lpRect);
}

void CGLViewCtrlCtrl::OnGetControlInfo(LPCONTROLINFO pControlInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	//see Help "Appendices / Netw Interface for Controls / Keyboard Interface"
	COleControl::OnGetControlInfo(pControlInfo);
	
	pControlInfo->dwFlags |= CTRLINFO_EATS_ESCAPE;
				//	Accelerator
	//HACCEL hAccel;
	//LoadAccelTable CWinApp
	CWinApp *a= AfxGetApp();

	pControlInfo->hAccel= ::LoadAccelerators(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR));
}

//OLE Controls: Advanced Topics

HMENU CGLViewCtrlCtrl::OnGetInPlaceMenu() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return COleControl::OnGetInPlaceMenu();
}

BOOL CGLViewCtrlCtrl::OnGetNaturalExtent(DWORD dwAspect, LONG lindex, DVTARGETDEVICE* ptd, HDC hicTargetDev, DVEXTENTINFO* pExtentInfo, LPSIZEL psizel) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return COleControl::OnGetNaturalExtent(dwAspect, lindex, ptd, hicTargetDev, pExtentInfo, psizel);
}

void CGLViewCtrlCtrl::Serialize(CArchive& ar) 
{
	if (ar.IsStoring())
	{	// storing code
	}
	else
	{	// loading code
	}
}
/*
void CGLViewCtrlCtrl::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	COleControl::OnClose();
}
*/

void CGLViewCtrlCtrl::OnShowWindow(BOOL bShow, UINT nStatus) 
{	// called on start and after close 

	TRACE("CGLViewCtrlCtrl::OnShowWindow(%d %d) %p\n",bShow,nStatus,this);

	COleControl::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here

	
}

void CGLViewCtrlCtrl::OnSettingsSound() 
{
	 view->SetUseSound(!view->GetUseSound());
}

void CGLViewCtrlCtrl::OnUpdateSettingsSound(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(view->GetUseSound());
}

LRESULT CGLViewCtrlCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{

	if (FALSE && m_useRenderThread) {

		LRESULT ret;
	    TRACE("Window Proc  %d \n",message);

		LockScene();
		    TRACE("Window Proc after lock %d \n",message);

			ret=  COleControl::WindowProc(message, wParam, lParam);
			
			if (view->getRedraw()) { // get thread local redraw flag
				m_mustRedraw = TRUE;
				view->clearRedraw();
			}


		UnlockScene();
		return ret;
	}

#if 0
	// some hardcoded message for speed 
	switch (message) {
	case  WM_KEYDOWN :
		TRACE("Keydown %d %d \n", (int) wParam,(int) lParam);
		OnKeyDown(wParam, LOWORD(lParam), HIWORD(lParam));
		return TRUE;
	case  WM_KEYUP :
		TRACE("Keyup %d %d \n", (int) wParam,(int) lParam);
		OnKeyUp(wParam, LOWORD(lParam), HIWORD(lParam));
		return TRUE;
	case WM_LBUTTONDOWN: 
		{ CPoint point((DWORD)lParam); OnLButtonDown(wParam, point); return TRUE; }
	case WM_LBUTTONUP:
		{ CPoint point((DWORD)lParam); OnLButtonUp(wParam, point); return TRUE; }
	case WM_MOUSEMOVE:
		{ CPoint point((DWORD)lParam); OnMouseMove(wParam, point); return TRUE; }

	case WM_PAINT:
		OnPaint(); return TRUE;

	case WM_TIMER:
		OnTimer(wParam); return TRUE;

	case WM_SETCURSOR:
		return OnSetCursor(NULL /*CWnd::FromHandle((HWND)wParam)*/,(short)LOWORD(lParam), HIWORD(lParam));

	default :
		return COleControl::WindowProc(message, wParam, lParam);
	}
#endif
	return COleControl::WindowProc(message, wParam, lParam);

}

void CGLViewCtrlCtrl::OnPopupHelpWorldinfo() 
{
	if (!view) return;

	DWorldInfo d;

	d.m_url= view->GetUrl();

	if (view->GetScene()) {
		d.m_vrml2 = view->GetScene()->vrml2.get();
	
		GvSceneInfo *s = view->GetSceneInfo();
		GvWorldInfo	*w= s->firstWorldInfo;
		if (w) {
			d.m_title = w->title.get();
			for (int i=0; i< w->_info.getNum();i++) {
				d.m_info += w->_info.get1(i).getString();
				d.m_info += "\r\n";
			}
		}
	}


	StopTimer();
	AFX_MANAGE_STATE(_afxModuleAddrThis);
		//d.DoModal();
		DoThreadModal((CDialog*)&d);
	RestartTimer();
}

BOOL CGLViewCtrlCtrl::saveWorld(LPCTSTR fileNameRel) 
{
	// dispatch handler code 
	if (!view) return FALSE;

	char fileName[_MAX_PATH+1];

	if (_fullpath( fileName , fileNameRel,_MAX_PATH ) == NULL) {
		return FALSE;
	}

	if (!fileName) return FALSE;
	
	if (strlen(fileName) <=4) {
		return FALSE;
	}


    char    szDrive[8] ;
    char    szDir[_MAX_PATH] ;
    char    szFile[_MAX_PATH] ;
    char    szExt[_MAX_PATH] ;
    int     i = 0 ;

    _splitpath(fileName, szDrive, szDir, szFile, szExt) ;

	if (strlen(szFile) == 0 || !strieq(szExt,".wrl")) 
		return FALSE;

	if (strlen(szDir) <=2 || strieq(szDir,"\\windows\\")) {
		return FALSE;
	}


	CString sysDir;

	UINT s = ::GetSystemDirectory(sysDir.GetBuffer(MAX_PATH+1),MAX_PATH); 
	sysDir.ReleaseBuffer();
	if (s>0 && strstr(szDir,sysDir)) {
		return FALSE;
	}

	GetReporter()->Warning("Saving world to %s ",fileName);

	gbool vrml2 = view->IsVrml2();

	GioTraversal traversal(fileName,"w");
	traversal.vrml2 = vrml2;
	traversal.format = VRML_FMT;
	traversal.writeInlines = 0;
	if (traversal.astream->f == NULL ) {
		GetReporter()->Error("Save world, can't save to %s ",fileName);
		return FALSE;
	}
	else {
		traversal.DoFileHeader();
		traversal.astream->comment("written by " _PROGRAM _PROGRAM_TYPE " " _VERSION);
		traversal.astream->nl();
	
	
		InitializeGvAttributes(traversal);
		traversal.InitializeAttributes();

		view->Traverse(traversal);

		return TRUE;
	}
}

void CGLViewCtrlCtrl::saveViewpoint(LPCTSTR viewpointName) 
{
	if (!view) return;

	if (viewpointName == NULL || strlen(viewpointName) == 0)
		view->SaveCamera((const char *)NULL);
	else view->SaveCamera(viewpointName);
	
}

// toggle dithering flag
void CGLViewCtrlCtrl::OnRenderDither() 
{
	gbool v = ! view->GetDithering();

	view->SetDithering(v);
	Redraw();

	
}

void CGLViewCtrlCtrl::OnUpdateRenderDither(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( view->GetDithering() !=0);
}

// toggle texture smooth filtering flag
void CGLViewCtrlCtrl::OnRenderTextureSmooth() 
{
	int v=view->GetTextureFiltering();

	if (v == 0) v = 1;
	else v = 0;

	view->SetTextureFiltering(v);
	Redraw();
}

void CGLViewCtrlCtrl::OnUpdateRenderTextureSmooth(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(view->GetTextureFiltering() !=0);
}

// registry helpers
// get value
BOOL CGLViewCtrlCtrl::GetProfile(LPCTSTR lpszKey, CString &value)
{
	HKEY hKeyRoot=NULL;
	BOOL bRet = FALSE;


	::RegOpenKey(HKEY_CURRENT_USER, USER_KEY, &hKeyRoot);

	if (hKeyRoot && GetRegKey(hKeyRoot,lpszKey, value)) {
		bRet = TRUE;
	}

	if (hKeyRoot) ::RegCloseKey(hKeyRoot);
	return bRet;

}

BOOL CGLViewCtrlCtrl::GetProfile(LPCTSTR lpszKey, int &value)
{
	HKEY hKeyRoot=NULL;
	BOOL bRet = FALSE;


	::RegOpenKey(HKEY_CURRENT_USER, USER_KEY, &hKeyRoot);

	if (hKeyRoot && GetRegKey(hKeyRoot,lpszKey, value)) {
		bRet = TRUE;
	}

	if (hKeyRoot) ::RegCloseKey(hKeyRoot);
	return bRet;

}

// set value

BOOL CGLViewCtrlCtrl::SetProfile(LPCTSTR lpszKey, LPCTSTR value)
{
	HKEY hKeyRoot=NULL;
	BOOL bRet = FALSE;


	::RegOpenKey(HKEY_CURRENT_USER, USER_KEY, &hKeyRoot);

	if (hKeyRoot && SetRegKey(hKeyRoot,lpszKey, value)) {
		bRet = TRUE;
	}

	if (hKeyRoot) ::RegCloseKey(hKeyRoot);
	return bRet;

}

BOOL CGLViewCtrlCtrl::SetProfile(LPCTSTR lpszKey, int value)
{
	HKEY hKeyRoot=NULL;
	BOOL bRet = FALSE;


	::RegOpenKey(HKEY_CURRENT_USER, USER_KEY, &hKeyRoot);

	if (hKeyRoot && SetRegKey(hKeyRoot,lpszKey, value)) {
		bRet = TRUE;
	}

	if (hKeyRoot) ::RegCloseKey(hKeyRoot);
	return bRet;

}





void CGLViewCtrlCtrl::OnSettingsPreferences() 
{ 
	//DPropSheet dialog(Translate(_T("Preferences")),this);
	DPropSheet dialog(Translate(_T("Preferences")),NULL);

	DPropGeneral *pGeneral=new DPropGeneral(this); //

	pGeneral->m_verbose  = m_verbose;
	pGeneral->m_fullSpeed = m_fullSpeed;
	pGeneral->m_exactCollision = view->useRapid;




	HKEY hKeyRoot=NULL;
	BOOL PanelOn;

	::RegOpenKey(HKEY_CURRENT_USER, USER_KEY, &hKeyRoot);

	if (hKeyRoot && GetRegKey(hKeyRoot,_T("General.panel"), PanelOn)) {
		pGeneral->m_navPanelOn = PanelOn;
	} else 
		pGeneral->m_navPanelOn = TRUE;// set as default 


	DPropSound *pSound=new DPropSound(this); 

	DPropCache *pCache=new DPropCache(this); 
	DPropCacheSettings *pCacheSettings=new DPropCacheSettings(this); 

	dialog.m_psh.dwFlags |= PSH_NOAPPLYNOW;


	pSound->m_num_sounds = view->maxNumSounds;
	pSound->m_use_rsx = view->useRSXSound;
	pSound->m_qualityI = view->soundQuality;
	// pSound->m_info;


    pCache->m_useCache = theCache.writeCacheEnabled;
    pCache->m_useMediaCache = theCache.writeMediaCacheEnabled; // Laurent - Jan.2000
    //pCache->m_cacheRead = theCache.readCacheEnabled;
	pCache->m_cacheDirectory = theCache.writeCacheDir;
	pCache->m_mediaCacheDirectory = theCache.writeMediaCacheDir; // Laurent - Jan.2000

	pCacheSettings->m_cacheVerify = (int)theCache.cacheMode;
    pCacheSettings->m_verifyDays = theCache.expirationDays;

    pCacheSettings->m_maxSpace = theCache.maxSpaceUsed / (1024*1024);
    pCacheSettings->m_minSpaceFree = theCache.minSpaceFree / (1024*1024);

	// get from registry 
	{
	CString val;

	if (hKeyRoot && GetRegKey(hKeyRoot,_T("Cache.readonlyDirectorys"), val)) {
		pCache->m_roCacheDirectorys = val;
	} else {
		pCache->m_roCacheDirectorys = "cdrom:\\cache"; // set as default 
	}

#if 0
	// Laurent - Jan. 2000
	val = "";

	if (hKeyRoot && GetRegKey(hKeyRoot,_T("Cache.mediaDirectory"), val)) {
		pCache->m_mediaCacheDirectory = val;
	} else  {
/*
		CString winDir;

		UINT s = ::GetWindowsDirectory(winDir.GetBuffer(MAX_PATH+1),MAX_PATH); 
		winDir.ReleaseBuffer();
		winDir += "\\media\\UniversalMedia";

		pCache->m_mediaCacheDirectory = winDir; // set as default 
*/
	}
#endif

	} 

	
	BOOL bOnlySystemMemory = FALSE;

#ifdef _D3D
	// Direct 3D Options
	DPropD3D *pD3D=new DPropD3D(this);

	pD3D->m_dither = view->GetDithering();
	pD3D->m_tfilter = view->GetTextureFiltering()>0;
	pD3D->m_tmipmap = view->GetTextureMipMap()>0;
	pD3D->m_antiAlias = view->GetAntiAliasing();
	
	pD3D->m_driverIndex = view->device->CurrDriver;
	pD3D->m_modeIndex = view->device->CurrMode;

	{

	if (hKeyRoot && GetRegKey(hKeyRoot,_T("Direct3D.onlySystemMemory"), bOnlySystemMemory)) {
	}

	if (hKeyRoot && GetRegKey(hKeyRoot,_T("Direct3D.dxlevel"), pD3D->m_dxLevel)) {

	}

	}
	bOnlySystemMemory = GetSysMemFlag();
	pD3D->m_use_videomem = ! bOnlySystemMemory;



	dialog.AddPage(pD3D);

#endif

#ifdef _OGL
	// Direct 3D Options
	DPropOpenGL *pOGL=new DPropOpenGL(this);

	pOGL->m_dither = view->GetDithering();
	pOGL->m_tfilter = view->GetTextureFiltering()>0;
	pOGL->m_tmipmap = view->GetTextureMipMap()>0;

	pOGL->m_info = "GL ";

    pOGL->m_info += (const char *) glGetString(GL_VERSION);
	pOGL->m_info += " ";
    pOGL->m_info  += (const char *) glGetString(GL_RENDERER);
	pOGL->m_info += " - ";
    pOGL->m_info  += (const char *) glGetString(GL_VENDOR);
    GLint param; 
    glGetIntegerv(GL_DEPTH_BITS,&param);

	pOGL->m_info1.Format("Z-Depth :%d Extensions :",(int)param);

    pOGL->m_info1 += (const char *) glGetString(GL_EXTENSIONS);
	
	int v=0;
	if (GetRegKey(hKeyRoot,_T("OpenGL.software"), v))
		pOGL->m_rsoftware=v;

	dialog.AddPage(pOGL);

#endif

	DPropD3D2 *pD3D2=new DPropD3D2(this);

#ifdef _KATMAI
	pD3D2->m_useKamtai = useKatmai;
	pD3D2->m_isKatmaiProcessor = isKatmaiProcessor;
#endif

	pD3D2->m_targetFps=view->targetFrameRate;
	pD3D2->m_tessellationFactor=view->nurbsTessellationScale;
	pD3D2->m_nurbsModeIndex=view->nurbsTessellationMode;

#ifdef _D3D
	pD3D2->m_softwareLighting = view->renderState.enableOwnLighting;
	if (pD3D2->m_softwareLighting && !view->renderState.doOwnLighting) {
		pD3D2->m_info = _T("Software lighting disabled !");
	}
	pD3D2->m_useD3dCulling = view->renderState.enableD3DCulling;
#endif

	dialog.AddPage(pD3D2);

	DPropPerformance *pDPerf=new DPropPerformance(this);

	if (pDPerf) {

		pDPerf->m_useTextures = view->useTextures;
		pDPerf->m_useSounds = view->useSound;
		pDPerf->m_useMovies = view->useMovies;
		pDPerf->m_visbilityFactor = view->lodScale;
		pDPerf->m_textureSizeIndex = view->maxUserTextureSize;

		// use movies
		dialog.AddPage(pDPerf);
	}

	dialog.AddPage(pSound);

	pGeneral->m_uiModeIndex = m_uiMode;

	dialog.AddPage(pGeneral);

	if (pCache)
		dialog.AddPage(pCache);

	if (pCacheSettings)
		dialog.AddPage(pCacheSettings);

	if (hKeyRoot) ::RegCloseKey(hKeyRoot);
	hKeyRoot=NULL;

	AFX_MANAGE_STATE(_afxModuleAddrThis);
	
	// if ( dialog.DoModal() == IDOK) 

	if ( (DoThreadModal( (CDialog*)&dialog ) == IDOK) && (view !=NULL) )
	{
			m_uiMode = (UIMode) pGeneral->m_uiModeIndex;
			m_verbose = pGeneral->m_verbose ;
			if (m_reporter) m_reporter->verbose = m_verbose;
			if (m_verbose) {
 				view->inlineWwwFlags |= WWW_REPORT_ERRORS;
 			}
 			else 
 				view->inlineWwwFlags &= ~WWW_REPORT_ERRORS;


			m_fullSpeed= pGeneral->m_fullSpeed;
			PanelOn = pGeneral->m_navPanelOn;
			view->useRapid= !! pGeneral->m_exactCollision;

		#ifdef _KATMAI
			if (pD3D2->m_useKamtai != useKatmai) {
				KatmaiEnable(pD3D2->m_useKamtai);
			}
		#endif

			view->SetTargetFrameRate(pD3D2->m_targetFps);
			view->nurbsTessellationScale=pD3D2->m_tessellationFactor;
			view->nurbsTessellationMode=pD3D2->m_nurbsModeIndex;

			if (pDPerf) {

				view->SetUseTextures(pDPerf->m_useTextures);
				view->SetUseSound(pDPerf->m_useSounds);
				view->SetUseMovies(pDPerf->m_useMovies);
				view->SetLodScale(pDPerf->m_visbilityFactor);
				view->SetMaxUserTextureSize(pDPerf->m_textureSizeIndex);
			}

#ifdef _D3D
			view->renderState.enableD3DCulling= !! pD3D2->m_useD3dCulling;

			if (pD3D2->m_softwareLighting != (BOOL)view->renderState.enableOwnLighting) {
				view->renderState.enableOwnLighting=!!pD3D2->m_softwareLighting;
				view->renderState.ReleaseMaterials(); 
				view->renderState.ReleaseTextures(); 
				view->renderState.ReleaseLights(); 
				view->renderState.ReleaseExBufs(); 
				view->boundBackgroundChanged = 1;
			}
	
			view->SetDithering(!!pD3D->m_dither);
			view->SetTextureFiltering(!!pD3D->m_tfilter);
			view->SetTextureMipMap(pD3D->m_tmipmap);
			view->SetAntiAliasing(!!pD3D->m_antiAlias);

			bOnlySystemMemory = !pD3D->m_use_videomem;
		
			// full screen mode 
			view->device->SetDefaultFullscreenMode(pD3D->m_modeIndex);
			//BOOL view->device->SetDefaultFullscreenMode(w,h,bpp);

			// driver change ??
			if ((view->device->CurrDriver != pD3D->m_driverIndex) || 
				( (view->device->bOnlySystemMemory != bOnlySystemMemory) && !view->device->Driver[view->device->CurrDriver].bIsHardware)) 
			{
				view->device->bOnlySystemMemory = bOnlySystemMemory;
				view->NavigationPanelDestroy();
				if (view->device->ChangeDriver(pD3D->m_driverIndex,D3DAPP_ONLYWINDOW)) {	
					if (view->device->IsHardwareDriver()) {
						SetProfile(_T("Direct3D.driverOk"),0);				
						m_checkFrames=0; // redo checking 
					}
					
					if (view->m_navPanelOn)
						view->NavigationPanelCreate(0);
					view->SetTextureFiltering(!!pD3D->m_tfilter);
					view->SetTextureMipMap(pD3D->m_tmipmap);
					view->SetAntiAliasing(!!pD3D->m_antiAlias);
				}
			}
			Redraw();
#endif
#ifdef _OGL
			view->SetDithering(!!pOGL->m_dither);
			view->SetTextureFiltering(pOGL->m_tfilter);
			view->SetTextureMipMap(pOGL->m_tmipmap);

			Redraw();
#endif

			HKEY hKeyRoot;
			// SAVE settings to registry 
			
			if (::RegCreateKey(HKEY_CURRENT_USER, USER_KEY, &hKeyRoot) == ERROR_SUCCESS) {

				CString val;
				val = GFile::cacheDirectory;
				SetRegKey(hKeyRoot,_T("cache.directory"), val );

				//val = GFile::cacheMediaDirectory; // Laurent - Jan.2000
				//SetRegKey(hKeyRoot,_T("cache.mediaDirectory"), val );

				// Set Media Library Registry key as per recommended practice. Laurent - Jan.2000
				SetRegKey(HKEY_LOCAL_MACHINE,_T("Software\\WEB3D\\PROTOCOLS\\urn\\web3d\\media\\"),_T("Default"),val);


				SetRegKey(hKeyRoot,_T("UI.mode"), (int) m_uiMode );

				SetRegKey(hKeyRoot,_T("General.panel"), (int) PanelOn);				
				SetRegKey(hKeyRoot,_T("General.fullSpeed"), (int) m_fullSpeed);
				SetRegKey(hKeyRoot,_T("General.verbose"), (int) m_verbose);
				SetRegKey(hKeyRoot,_T("General.exactCollision"), (int) 	view->useRapid);


				SetRegKey(hKeyRoot,_T("Performance.useTextures"), (int)view->useTextures);
				SetRegKey(hKeyRoot,_T("Performance.useMovies"), (int)view->useMovies);
				SetRegKey(hKeyRoot,_T("Sound.enableSound"), (int)view->useSound);
				
				SetRegKey(hKeyRoot,_T("Performance.maxTextureSize"), (int)view->maxUserTextureSize);
				SetRegKey(hKeyRoot,_T("Performance.visbilityFactor"), view->lodScale);


				SetRegKey(hKeyRoot,_T("Nurbs.targetFps"), view->targetFrameRate);
				SetRegKey(hKeyRoot,_T("Nurbs.tessellationMode"), view->nurbsTessellationMode);


#ifdef _D3D
				val = view->device->Driver[view->device->CurrDriver].Name;
				m_saveDriverSettings = TRUE; // save driver name only on "sucessfull" exit 

				//SetRegKey(hKeyRoot,_T("Direct3D.driver"), val);

				bOnlySystemMemory = !pD3D->m_use_videomem;


				val = "";

				if (view->device->GetDefaultFullscreenMode(val))
					SetRegKey(hKeyRoot,_T("Direct3D.fullscreenMode"),val);

				SetRegKey(hKeyRoot,_T("Direct3D.dither"), view->GetDithering());
				SetRegKey(hKeyRoot,_T("Direct3D.textureFiltering"), view->GetTextureFiltering() );
				SetRegKey(hKeyRoot,_T("Direct3D.textureMipMap"), view->GetTextureMipMap() );
				SetRegKey(hKeyRoot,_T("Direct3D.antiAlias"), view->GetAntiAliasing() );
				
				//SetRegKey(hKeyRoot,_T("Direct3D.alphaStipple"),view->renderState.alphaStipple);

				SetRegKey(hKeyRoot,_T("Direct3D.askHardware"), m_askHardware );
				SetRegKey(hKeyRoot,_T("Direct3D.onlySystemMemory"), bOnlySystemMemory);
				SetRegKey(hKeyRoot,_T("Direct3D.dxlevel"), pD3D->m_dxLevel);
				
				SetRegKey(hKeyRoot,_T("Direct3D.useSoftwareLighting"), view->renderState.enableOwnLighting);
				SetRegKey(hKeyRoot,_T("Direct3D.useD3DCulling"), view->renderState.enableD3DCulling);


#endif

#ifdef _OGL
				SetRegKey(hKeyRoot,_T("OpenGL.textureFiltering"), view->GetTextureFiltering() );
				SetRegKey(hKeyRoot,_T("OpenGL.textureMipMap"), view->GetTextureMipMap() );
				SetRegKey(hKeyRoot,_T("OpenGL.dither"), view->GetDithering());
				//SetRegKey(hKeyRoot,_T("OpenGL.zdepth"), software);
				SetRegKey(hKeyRoot,_T("OpenGL.software"), pOGL->m_rsoftware);
#endif

				SetRegKey(hKeyRoot,_T("Navigation.anchorSingleClick"), m_anchorSingleClick);

				gbool soundChanged = ( ((int)view->useRSXSound) != pSound->m_use_rsx) || 
									(view->soundQuality != pSound->m_qualityI);

				if (soundChanged) {
					view->ReleaseSound();
					Redraw();
				}
				view->maxNumSounds = pSound->m_num_sounds;
				view->useRSXSound = pSound->m_use_rsx !=0 ;
				view->soundQuality = pSound->m_qualityI;

				if (view->soundDevice) {
					view->soundDevice->SetQuality(view->soundQuality);
	 			    view->soundDevice->SetNumActiveSounds(view->maxNumSounds);
	 			    view->soundDevice->PrioritizeSounds();
				}

				SetRegKey(hKeyRoot,_T("Sound.numSounds"), view->maxNumSounds);
				SetRegKey(hKeyRoot,_T("Sound.useRsx"), view->useRSXSound);
				SetRegKey(hKeyRoot,_T("Sound.quality"), view->soundQuality);
				SetRegKey(hKeyRoot,_T("Sound.doppler"), view->soundDoppler);


				if (pCache) {

					theCache.writeCacheEnabled = !!pCache->m_useCache;
					theCache.writeMediaCacheEnabled = !!pCache->m_useMediaCache; // Laurent - Jan.2000
					//theCache.readCacheEnabled = pCache->m_cacheRead;
					theCache.readCacheEnabled = theCache.writeCacheEnabled;
					theCache.readMediaCacheEnabled = theCache.writeMediaCacheEnabled; // Laurent - Jan.2000

					//theCache.expirationDays = pCache->m_verifyDays;
					if (pCacheSettings->m_cacheVerify >= GCACHE_ALWAYS
						&& pCacheSettings->m_cacheVerify <= CCACHE_NEVER)
					theCache.cacheMode = (GCacheMode)pCacheSettings->m_cacheVerify;
					theCache.expirationDays= pCacheSettings->m_verifyDays;
					DWORD old_maxSpaceUsed = theCache.maxSpaceUsed;
					DWORD old_minSpaceFree = theCache.minSpaceFree;

					theCache.maxSpaceUsed = pCacheSettings->m_maxSpace * (1024*1024);
					theCache.minSpaceFree = pCacheSettings->m_minSpaceFree * (1024*1024);
					// settings lowered ?? 
					if (theCache.maxSpaceUsed < old_maxSpaceUsed) theCache.needCacheCleaning = TRUE;
					if (theCache.minSpaceFree > old_minSpaceFree) theCache.needCacheCleaning = TRUE;

					SetRegKey(hKeyRoot,_T("Cache.verifyMode"), theCache.cacheMode);

					SetRegKey(hKeyRoot,_T("Cache.writeCache"), theCache.writeCacheEnabled);
					SetRegKey(hKeyRoot,_T("Cache.writeMediaCache"), theCache.writeMediaCacheEnabled); // Laurent - Jan.2000

					SetRegKey(hKeyRoot,_T("Cache.readCache"), theCache.readCacheEnabled);
					SetRegKey(hKeyRoot,_T("Cache.readMediaCache"), theCache.readMediaCacheEnabled); // Laurent - Jan.2000

					SetRegKey(hKeyRoot,_T("Cache.mediaDirectory"), pCache->m_mediaCacheDirectory); // Laurent - Jan.2000

					// Set Media Library Registry key as per recommended practice. Laurent - Jan.2000
					SetRegKey(HKEY_LOCAL_MACHINE,_T("Software\\WEB3D\\PROTOCOLS\\urn\\web3d\\media\\"),_T("Default"),pCache->m_mediaCacheDirectory);


					SetRegKey(hKeyRoot,_T("Cache.readonlyDirectorys"), pCache->m_roCacheDirectorys);
					SetRegKey(hKeyRoot,_T("Cache.verifyDays"), theCache.expirationDays);

					SetRegKey(hKeyRoot,_T("Cache.minDiskSpaceFreeKB"), theCache.minSpaceFree/1024);
					SetRegKey(hKeyRoot,_T("Cache.maxDiskSpaceUsedKB"), theCache.maxSpaceUsed/1024);

					
					BOOL ret = theCache.SetWriteCacheDirectory(pCache->m_cacheDirectory);
					if ((theCache.writeCacheDir.GetLength()>0) &&  (!theCache.writeCacheDirValid || !ret)) {
						AfxMessageBox("Warning : bad caching directory "+pCache->m_cacheDirectory);
					}
					else SetRegKey(hKeyRoot,_T("Cache.directory"), pCache->m_cacheDirectory);

					theCache.SetReadCacheDirectoryPath(pCache->m_roCacheDirectorys);

					BOOL ret2 = theCache.SetWriteMediaCacheDirectory(pCache->m_mediaCacheDirectory); // Laurent - Jan. 2000
					if ((theCache.writeMediaCacheDir.GetLength()>0) &&  (!theCache.writeMediaCacheDirValid || !ret2)) {
						AfxMessageBox("Warning : bad Universal Media caching directory "+pCache->m_mediaCacheDirectory);
					}

					else SetRegKey(hKeyRoot,_T("Cache.mediaDirectory"), pCache->m_mediaCacheDirectory);

					theCache.SetUmelDirectoryPath(pCache->m_mediaCacheDirectory);  // end Laurent - Jan. 2000
				}

				::RegCloseKey(hKeyRoot);
			}

	}
}

float CGLViewCtrlCtrl::GetAvatarHeight() 
{
	if (!view) return 0;
	return view->heightOverGround;
}

void CGLViewCtrlCtrl::SetAvatarHeight(float newValue) 
{
	if (!view) return; 
	view->heightOverGround	= view->heightOverGroundLocal=newValue;
}

float CGLViewCtrlCtrl::GetCollisionDistance() 
{
	if (!view) return 0;

	return view->collisionDistance;
}


void CGLViewCtrlCtrl::SetCollisionDistance(float newValue) 
{
	if (!view) return; 
	view->collisionDistance = view->collisionDistanceLocal = newValue;
}

float CGLViewCtrlCtrl::GetVisibilityLimit() 
{
	if (!view) return 0; 
	// TODO: Add your property handler here
	return view->camera->zfar;
}

void CGLViewCtrlCtrl::SetVisibilityLimit(float newValue) 
{
	if (newValue <= 0.0f ) return; // BAD 
	if (!view) return; 
	view->visibilityLimit = view->visibilityLimitLocal = newValue;
	view->camera->zfar = newValue;
}


BOOL CGLViewCtrlCtrl::setFullscreen(BOOL mode) 
{
	if (!view) {
		m_fullscreenOnStartup = mode;
		return mode;
	}

	if (view->GetFullScreen() != mode) {
		view->SetFullScreen(mode);
	}
	return view->GetFullScreen();
}


BOOL CGLViewCtrlCtrl::GetGravity() 
{
	if (!view) return FALSE;
	return view->stayOnGround;
}

void CGLViewCtrlCtrl::SetGravity(BOOL bNewValue) 
{
	view->stayOnGround = bNewValue!=0;
	// SetModifiedFlag();
}


// http://www.microsoft.com/kb/articles/q167/9/56.htm


ULONG FAR EXPORT CGLViewCtrlCtrl::XCmdTargetObj::AddRef()
{
   METHOD_PROLOGUE(CGLViewCtrlCtrl, CmdTargetObj)
   return pThis->ExternalAddRef();
}

ULONG FAR EXPORT CGLViewCtrlCtrl::XCmdTargetObj::Release()
{
   METHOD_PROLOGUE(CGLViewCtrlCtrl, CmdTargetObj)
   return pThis->ExternalRelease();
}

HRESULT FAR EXPORT CGLViewCtrlCtrl::XCmdTargetObj::QueryInterface(
   REFIID iid, void FAR* FAR* ppvObj)
{
   METHOD_PROLOGUE(CGLViewCtrlCtrl, CmdTargetObj)
   return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

//IOleCommandTarget::QueryStatus

STDMETHODIMP CGLViewCtrlCtrl::XCmdTargetObj::QueryStatus(
   const GUID* pguidCmdGroup, ULONG cCmds, OLECMD rgCmds[],
   OLECMDTEXT* pcmdtext)
{
   METHOD_PROLOGUE(CGLViewCtrlCtrl, CmdTargetObj)
   //... add YOUR own code here.
   TRACE("OLECMDID  QueryStatus%d \n",rgCmds[0]);

   return S_OK;
}

//IOleCommandTarget::Exec
STDMETHODIMP CGLViewCtrlCtrl::XCmdTargetObj::Exec(
   const GUID* pguidCmdGroup, DWORD nCmdID, DWORD nCmdExecOpt,
   VARIANTARG* pvarargIn, VARIANTARG* pvarargOut)
{
   METHOD_PROLOGUE(CGLViewCtrlCtrl, CmdTargetObj)

   TRACE("OLECMDID %d \n",nCmdID);
   
   switch (nCmdID) {
   case OLECMDID_STOP:
      {
      // ... STOP button is clicked, add YOUR own code here.
	   if (pThis->m_initialized) {

			pThis->OnSettingsStopLoading(); // KILL download
			pThis->view->ReleaseSound();
			pThis->StopTimer();
			//view->time.Disable();
       }
	   break;


      }
	case OLECMDID_REFRESH : 
	{
      // ::MessageBox(NULL, "Refresh","CGLViewCtrlCtrl", MB_OK);
		break;
	}	
	case 36 : //OLECMDID_ENABLE_INTERACTION 
		//Sent by the container to tell the object to either pause or resume any multimedia (audio or animation) in the document. Takes a VT_BOOL parameter, which is TRUE if the multimedia should be resumed or FALSE if it should be paused. The Internet Explorer 4.0 browser uses this command to inform the object of when it is minimized or completely covered by another window so that the object can pause playing of multimedia information. 
		TRACE("Enabled interaction %d \n",pvarargIn->boolVal);

		if (pThis->m_initialized && pvarargIn != NULL) {
			pThis->m_enableInteraction=pvarargIn->boolVal;
			if (!pThis->m_enableInteraction) pThis->StopTimer();
			else pThis->StartTimer();
		}

		break;

	case 37 : //OLECMDID_ONUNLOAD 
		// Sent by the browser before a new navigation is initiated or the browser is being closed. This provides the opportunity for a document to query the user if any unsaved data should be saved. Returning S_OK allows the navigation to close. Returning S_FALSE prevents the operation. This command is sent only once. Because navigation is asynchronous, it is important to remember that the user can make changes in a page or document after this command is received but before the new page is displayed. 
	   if (pThis->m_initialized) {

			pThis->OnSettingsStopLoading(); // KILL download
			pThis->view->ReleaseSound();
			pThis->StopTimer();
			//view->time.Disable();
       }

		break;

	default :
	   break;
   }

   return S_OK;
}

// see OnActivateInPlace
// PRB: MFC ActiveX Control in IE Doesn't Detect Keystrokes
// Article ID: Q168777

 


int CGLViewCtrlCtrl::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{

	TRACE("CGLViewCtrlCtrl::OnMouseActivate %d %d \n",nHitTest, message);

	if (view && !view->GetFullScreen()) // hg to check 
		OnActivateInPlace (TRUE, NULL); // == UI-Activate the control	

	return COleControl::OnMouseActivate(pDesktopWnd, nHitTest, message);
}



BOOL CGLViewCtrlCtrl::setObserver(LPUNKNOWN observerUnk, long flags) 
{

	if (!view) return FALSE;

	CComQIPtr<BrowserObserver,&IID_BrowserObserver> tmp(observerUnk);

	if (tmp) {  // new observer

		if (view->observer != NULL && view->observer != tmp) 
			return FALSE; // kristof want : only one observer

		view->observer = tmp;
		view->observerFlags = flags |  (view->observerFlags & ~GOBSERVER_OBSERVE_MASK);
		view->reportErrors = FALSE; // tmp
		GFile *mainLoader = view->file;

		if (mainLoader) {
			// check if observer wants OnSceneChanged
			if (view->observerFlags & GOBSERVE_URLLOADING) {
				mainLoader->hObserver = view->observer;
			}
			if (view->observerFlags & GOBSERVE_WM_URLLOADING) {
				mainLoader->hNetscapeMsgWnd = view->observerWnd;
			}
		}
		return TRUE;
	}
	else {
		if (view->observerFlags & GOBSERVE_URLLOADING) {
			// release all observer pointers around 
			if (mainLoader)	mainLoader->hObserver.Release();
			if (view->file) view->file->hObserver.Release();
			if (mainLoader)	 // kill main URL loader
			{
   				mainLoader->Kill();
   				mainLoader->unref();
				mainLoader = NULL;
			}

			// kill all download activities for this window 
			if (GetSafeHwnd()) {

				KillDownloads();

				GFile *i= GFile::filesInUse;
    
				while(i != NULL) 
				{
					GFile *n = i->next;
					if (i->hPostMsgWnd == GetSafeHwnd()) {
						i->hObserver.Release();
					}
					i= n;
				}
			}

		}
		view->observer.Release();
		view->observerFlags = 0 | (view->observerFlags & ~GOBSERVER_OBSERVE_MASK);
		view->reportErrors = TRUE; // tmp
		return  (observerUnk == NULL);
	}
}

BOOL CGLViewCtrlCtrl::setObserverWnd(long  hWnd) 
{
    if (hWnd == 0)
    {
	    if (view && view->observerFlags & GOBSERVE_ANCHOR)
		    view->observer->OnLoadUrl(NULL, NULL);
    }
	if (!m_initialized) {
		HWND hwnd = GetSafeHwnd();
		if (!m_pDC) m_pDC = GetDC(); 
		if (!Initialize(m_pDC->m_hDC)) return FALSE;
	}

	view->observerWnd = (HWND) hWnd;
	if (hWnd != NULL)
		view->observerFlags = (GOBSERVE_WM_MESSAGE | GOBSERVE_WM_ANCHOR | GOBSERVE_WM_URLLOADING) | (view->observerFlags & ~GOBSERVER_WM_MASK);
	else view->observerFlags=  0 | (view->observerFlags & ~GOBSERVER_WM_MASK);

	GFile *mainLoader = view->file;

	if (mainLoader) {
		// check if observer wants OnSceneChanged
		if (view->observerFlags & GOBSERVE_URLLOADING) {
			mainLoader->hObserver = view->observer;
		}
		if (view->observerFlags & GOBSERVE_WM_URLLOADING) {
			mainLoader->hNetscapeMsgWnd = view->observerWnd;
		}
	}

	return TRUE;
}




// Netscape functions 
// load the top level scene as URL already download to the file fileName 
BOOL CGLViewCtrlCtrl::loadURLfromFile2(LPCTSTR url, LPCTSTR mimeType, long lastModified, long size, LPCTSTR fileName) 
{

   BOOL ret = FALSE;

   USES_CONVERSION;

	   
   CString msg;
//   CWaitCursor wait;
   
   TRACE("loadURLfromFile %s %s %s \n",url,mimeType,fileName);

   if (!fileName) return FALSE;

	if (!m_initialized) {
		HWND hwnd = GetSafeHwnd();
		if (!m_pDC) m_pDC = GetDC(); 
		if (!Initialize(m_pDC->m_hDC)) return FALSE;
	}

   ViewLock viewLock(view); 
   msg = "Loading \"";
   msg += url;
   msg+="\" ...";

   Message(msg,PROGRESS_MESSAGE);
   	   
   // TO DO:  Kill all old threads
   if (mainLoader) // kill current loader 
   {
   		mainLoader->Kill();
  		mainLoader->unref();
		mainLoader = NULL;
   }



   mainLoader = new GFile;
   mainLoader->useThread = GFile::useThreads;
   mainLoader->ref();
   mainLoader->ClearSessionError(); 

   mainLoader->SetFlags(WWW_REPORT_ERRORS);
   mainLoader->SetReporter(GetReporter());

   mainLoader->lastModified = lastModified;
   mainLoader->size = size;
 
   // window to notify on completition
   mainLoader->hPostMsgWnd = GetSafeHwnd();
   
  // check if observer wants OnSceneChanged
   if (view->observerFlags & GOBSERVE_URLLOADING) {
		mainLoader->hObserver = view->observer;
   }
   if (view->observerFlags & GOBSERVE_WM_URLLOADING) {
	   mainLoader->hNetscapeMsgWnd = view->observerWnd;
   }
 
  if (url) 
	   mainLoader->Set(url,0,0);

   if (mimeType) {
	   mainLoader->contentType = mimeType;
   }	

   GLocalFileClass fileClass = GLFC_LOCAL_FILE;

   if (1) fileClass = GLFC_NS_CACHE_FILE; // netscape cache file 

   mainLoader->urlLoaded = 1;	
   // set the local filename 
   mainLoader->SetLocalFile(fileName,fileClass);

   mainLoader->ProcessExtension(mainLoader->localFile);

   TRACE("File Formt Is %d \n", mainLoader->localFileFmt);
   

   if (mainLoader->url.GetLength()>0) {
		GFileInfo info;
		info.creationTime = 0;
		CString cacheFile;

		// get the cache file time stamp if any 
		if ((!mainLoader->AnyFlag(WWW_REFRESH|WWW_RELOAD)) && theCache.GetUrl(mainLoader->url,mainLoader->ifModifiedSince.GetTime(),cacheFile,info)) {
			mainLoader->cacheLastModified = info.creationTime;
		}

		// save file to cache 
		if (!mainLoader->Flag(WWW_DONT_CACHE) ) {
			theCache.SaveUrl(mainLoader->url,mainLoader->localFile,lastModified,mainLoader->localCacheFile);
		}
   }


   if (mainLoader->localFileFmt == GZIP_FMT || mainLoader->localFileFmt == UNKNOWN_FMT) {
	   TRACE("Unexpected Fileformat %d \n",mainLoader->localFileFmt);
	   mainLoader->localFileFmt = VRML_FMT; // assume VRML
   }


   CString path(mainLoader->GetUrl());
		 
  
   AddUrlToHistory(mainLoader->GetFullUrl());
		
		if (mainLoader->localFileUnzipped.GetLength()>0) {
			ret = view->ReadModel(mainLoader);
			if (ret>=0) {
				SetAnimateOff();
				Redraw();
				Message("Url "+path);
				FireOnSceneChanged(path);
				// check if observer wants OnSceneChanged
				if (view->observerFlags & GOBSERVE_MESSAGE) {
					view->observer->OnSceneChanged((BSTR) T2COLE((const char*)path));
				}

				ret = TRUE;
			}

		};
	
	mainLoader->unref();
	mainLoader = NULL;
	InternalSetReadyState(READYSTATE_COMPLETE);

   return(ret);

}

BOOL CGLViewCtrlCtrl::loadURLfromFile(LPCTSTR url, LPCTSTR mimeType, LPCTSTR fileName) 
{
	return loadURLfromFile2(url,mimeType,0,0,fileName);

}


void CGLViewCtrlCtrl::onUrlNotify(LPCTSTR url, LPCTSTR mimeType, LPCTSTR fileName, long reason) 
{
    TRACE("onUrlNotify url %s, %s %d \n",url,fileName,reason);
	
	GLocalFileClass fileClass = GLFC_LOCAL_FILE;

	if (1) fileClass = GLFC_NS_CACHE_FILE; // netscape cache file 
    
	// check if it is our top level loader ?                                                               <<< neu
    if (mainLoader) mainLoader->OnUrlNotify(url,mimeType,0,fileName,fileClass,reason);  

	// scan through all waiting files
	// and pass the notify request 
	GFile::OnUrlNotify(GetSafeHwnd(), url, mimeType, 0, fileName, fileClass, reason); 

}

void CGLViewCtrlCtrl::onUrlNotify2(LPCTSTR url, LPCTSTR mimeType, long lastModified, long size, LPCTSTR fileName, long reason) 
{
    TRACE("onUrlNotify2 url %s, %s %d %d \n",url,fileName,lastModified,reason);

	GLocalFileClass fileClass = GLFC_LOCAL_FILE;

	if (1) fileClass = GLFC_NS_CACHE_FILE; // netscape cache file 
	
	// check if it is our top level loader ?                                                               <<< neu
    if (mainLoader) mainLoader->OnUrlNotify(url,mimeType,lastModified,fileName,fileClass,reason);  

	// scan through all waiting files
	// and pass the notify request 
	GFile::OnUrlNotify(GetSafeHwnd(), url, mimeType, lastModified, fileName,fileClass, reason); 

}



long CGLViewCtrlCtrl::getUiMode() 
{
	return m_uiMode;
}

void CGLViewCtrlCtrl::setUiMode(long newMode) 
{
	if (newMode>=UI_NONE && newMode<=UI_EXPERT) {
		m_uiMode = (UIMode) newMode;
	}
}

void CGLViewCtrlCtrl::openPreferences(long flags) 
{
   if (view)
		OnSettingsPreferences();
}

void CGLViewCtrlCtrl::playSound(LPCTSTR soundFilename) 
{
	if (view)  {
		ViewLock viewLock(view); 
		view->PlaySoundFile(soundFilename);
	}
}




void CGLViewCtrlCtrl::OnPopupSpeedVeryslow() 
{
	//m_cameraSpeed = 0.0100;	
	m_cameraSpeed = view->viewStepSpeed / 5.0;	
	m_cameraSpeedFac = 2;
	
	HKEY hKeyRoot;
	// SAVE settings to registry 
				
	if (::RegCreateKey(HKEY_CURRENT_USER, USER_KEY, &hKeyRoot) == ERROR_SUCCESS) {
		SetRegKey(hKeyRoot,_T("General.speedFac"),  m_cameraSpeedFac);
		::RegCloseKey(hKeyRoot);
	}

}

void CGLViewCtrlCtrl::OnUpdatePopupSpeedVeryslow(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_cameraSpeedFac == 2);
}

void CGLViewCtrlCtrl::OnPopupSpeedSlow() 
{
	//m_cameraSpeed = 0.0300;	
	m_cameraSpeed = view->viewStepSpeed / 2.0;	
	m_cameraSpeedFac = 5;	

	HKEY hKeyRoot;
	// SAVE settings to registry 
				
	if (::RegCreateKey(HKEY_CURRENT_USER, USER_KEY, &hKeyRoot) == ERROR_SUCCESS) {
		SetRegKey(hKeyRoot,_T("General.speedFac"),  m_cameraSpeedFac);
		::RegCloseKey(hKeyRoot);
	}

}

void CGLViewCtrlCtrl::OnUpdatePopupSpeedSlow(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_cameraSpeedFac == 5);
}

void CGLViewCtrlCtrl::OnPopupSpeedMedium() 
{
	//m_cameraSpeed = 0.0625;	
	m_cameraSpeed = view->viewStepSpeed ;	
	m_cameraSpeedFac = 10;
	
	HKEY hKeyRoot;
	// SAVE settings to registry 
				
	if (::RegCreateKey(HKEY_CURRENT_USER, USER_KEY, &hKeyRoot) == ERROR_SUCCESS) {
		SetRegKey(hKeyRoot,_T("General.speedFac"),  m_cameraSpeedFac);
		::RegCloseKey(hKeyRoot);
	}

}

void CGLViewCtrlCtrl::OnUpdatePopupSpeedMedium(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_cameraSpeedFac == 10);
}

void CGLViewCtrlCtrl::OnPopupSpeedFast() 
{
	//m_cameraSpeed = 0.1200;	
	m_cameraSpeed = view->viewStepSpeed * 2.0;	
	m_cameraSpeedFac = 20;	

	HKEY hKeyRoot;
	// SAVE settings to registry 
				
	if (::RegCreateKey(HKEY_CURRENT_USER, USER_KEY, &hKeyRoot) == ERROR_SUCCESS) {
		SetRegKey(hKeyRoot,_T("General.speedFac"),  m_cameraSpeedFac);
		::RegCloseKey(hKeyRoot);
	}
}

void CGLViewCtrlCtrl::OnUpdatePopupSpeedFast(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_cameraSpeedFac == 20);
}

void CGLViewCtrlCtrl::OnPopupSpeedVeryfast() 
{
	//m_cameraSpeed = 0.3500;	
	m_cameraSpeed = view->viewStepSpeed * 5;	
	m_cameraSpeedFac = 60;	
	
	HKEY hKeyRoot;
	// SAVE settings to registry 
				
	if (::RegCreateKey(HKEY_CURRENT_USER, USER_KEY, &hKeyRoot) == ERROR_SUCCESS) {
		SetRegKey(hKeyRoot,_T("General.speedFac"),  m_cameraSpeedFac);
		::RegCloseKey(hKeyRoot);
	}

}

void CGLViewCtrlCtrl::OnUpdatePopupSpeedVeryfast(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_cameraSpeedFac == 60);
}





void CGLViewCtrlCtrl::OnDevModeChange(LPTSTR lpDeviceName) 
{
	COleControl::OnDevModeChange(lpDeviceName);
	TRACE("CGLViewCtrlCtrl::OnDevModeChange(%s)\n",lpDeviceName);
	

#ifdef _D3D
	view->NavigationPanelDestroy();

	if (view->device->ChangeDriver(view->device->CurrDriver,D3DAPP_ONLYWINDOW)) {
		if (view->m_navPanelOn)
			view->NavigationPanelCreate(0);
		Redraw();
	}

#endif
	
}

BOOL CGLViewCtrlCtrl::OnProperties(LPMSG lpMsg, HWND hWndParent, LPCRECT lpRect) 
{
	// TODO: Add your specialized code here and/or call the base class
	TRACE("CGLViewCtrlCtrl::OnProperties \n");
	
	return COleControl::OnProperties(lpMsg, hWndParent, lpRect);
}


void CGLViewCtrlCtrl::OnGoBack() 
{
	GoBack();
}

void CGLViewCtrlCtrl::OnUpdateGoBack(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_historyIndex>0);	
}


void CGLViewCtrlCtrl::OnSettingsConsole() 
{
	if (m_dConsole) {
		m_dConsole->m_hide = FALSE;
		m_dConsole->ShowWindow(SW_SHOWNOACTIVATE); // SW_SHOWNA
	}
}


void CGLViewCtrlCtrl::OnUpdateSettingsConsole(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_dConsole!=NULL);	
}


void CGLViewCtrlCtrl::OnView3rdPerson() 
{	bool mode = !view->thirdPersonView;

	view->SetThirdPersonView(mode);
	view->ShowAvatar(mode);

	if (view->showAvatar) Message("Use 12 34 56 78 90 Keys to change viewing perspective",PROGRESS_MESSAGE);
}

void CGLViewCtrlCtrl::OnUpdateView3rdPerson(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(view->thirdPersonView);	
}

void CGLViewCtrlCtrl::OnViewPad() 
{
	// TODO: Add your command handler code here
	if (m_dPad) { m_dPad->DestroyWindow(); delete m_dPad; m_dPad = NULL; }
	else 
	if (m_dPad == NULL) {
		m_dPad = 	new CPropertySheet(_T("CC3D control pad"),this);
		DPadViewpoints *pVp =new DPadViewpoints(); 
		DPadAvatarView *pAv =new DPadAvatarView(); 
		pVp->view =view;
		m_dPad->AddPage(pVp);
		m_dPad->AddPage(pAv);
		pAv->view =view;
		DWORD flags = WS_SYSMENU | WS_POPUP | WS_CAPTION | DS_MODALFRAME |  WS_VISIBLE;
   
		if (m_dPad->Create(this,flags, WS_EX_DLGMODALFRAME |  WS_EX_TOOLWINDOW)) {
			m_dPad->ShowWindow(SW_SHOWNORMAL);	
		}
	
	} else {
		m_dPad->ShowWindow(SW_SHOWNORMAL);	
	}

	
}

void CGLViewCtrlCtrl::OnUpdateViewPad(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_dPad != NULL);
	
}



void CGLViewCtrlCtrl::printMessage(LPCTSTR message) 
{
	GetReporter()->Trace(message);
}
/*
   set viepoint to a new value
   mode : bit 0    1: animated transistion 0 : not animated
		  bit 1    1: set absolute viewpoint (unbind any bound viewpoint)
		  bit 2	   1: values are a relative move distance (left/right, up/down, in/out, look left/right, look up/down, roll,)
		  bit 3    1: check for collision, 0 not
		  bit 4    1: check for ground, 0 not
		  

*/
void CGLViewCtrlCtrl::setViewpointByValue_(float positionX, float positionY, float positionZ, float orientationX, float orientationY, float orientationZ, float orientationAngle, long mode) 
{
	setViewpointByValue(positionX,positionY,positionZ, orientationX, orientationY,orientationZ,orientationAngle,mode); 

}

// protected 
void CGLViewCtrlCtrl::setViewpointByValue(float positionX, float positionY, float positionZ, float orientationX, float orientationY, float orientationZ, float orientationAngle, long mode) 
{
	if (!view) return;

	gbool animated = (mode & 1) !=0;
	gbool global = (mode & 2) !=0;
	gbool relative = (mode & 4) !=0;
	gbool cd = (mode & 8) !=0;
	gbool grav = (mode & 16) !=0;

	GCamera *camera;
	GCameraValue v;

	camera = &view->GetCamera();

	if (global || animated)
		view->BindViewpoint(NULL);

	Point oldPosition = view->GetCamera().position;
	Point newPosition(oldPosition);

	if (relative) {
		if (animated) {
			if (!view->cameraInterpolator) {
				view->cameraInterpolator = new GCameraInterpolator();
			}
			view->cameraInterpolator->a.Set(*camera); 
		}
		
		camera->Dolly(positionX,positionY,positionZ);
		camera->Pan(orientationX,orientationY);
		camera->Roll(orientationZ);
		
		newPosition = camera->position;

		if (animated) {
			view->cameraInterpolator->b.Set(*camera); 
			view->cameraInterpolator->Reset();
		}

	}
	else {
		v.position.Set(positionX,positionY,positionZ);
		v.orientation.SetAxisRotation(orientationX,orientationY,orientationZ,orientationAngle);
		v.fieldOfView = camera->GetFovY();

		newPosition = v.position;

		if (animated) {
			if (!view->cameraInterpolator) {
				view->cameraInterpolator = new GCameraInterpolator();
			}
			view->cameraInterpolator->a.Set(*camera); 
			view->cameraInterpolator->b = v; 
			view->cameraInterpolator->Reset();
		}
		else ::SetCamera(camera,v);
	}

	if (cd) {
		Point p,n;
		Point newPosition = view->GetCamera().position;
		// p = schnittpunkt - distance to object, n = normalenvektor
		if (view->CollisionDetect(oldPosition,newPosition,p,n)) 
		{
			view->GetCamera() += (p-newPosition);
			// could optiomize, no redraw if camera didn't change 
			// Message("Collision !",COLLISION_MESSAGE);
			SetCollisionState(TRUE);
			if (animated && view->cameraInterpolator) {
				view->cameraInterpolator->b.position = camera->position; 
			}

		}
		else 
			SetCollisionState(FALSE);

	} else 
	  SetCollisionState(FALSE);

	if (grav) 
		NavCheckStayOnGround();

	view->setRedraw();
	//if (camera->Changed()) 	Redraw();
}

/*
  get the current viewpoint data
  mode 0 : local viewpoint
  mode 1 : global viewpoint
  mode 2 : third person mode viewpoint

*/
void CGLViewCtrlCtrl::getViewpointByValue(float FAR* positionX, float FAR* positionY, float FAR* positionZ, float FAR* orientationX, float FAR* orientationY, float FAR* orientationZ, float FAR* orientationAngle, long mode) 
{
	if (!view) return;

	Vector axis;
	float angle;
	GCamera *camera = &view->GetCamera();

	if (mode == 1)
		camera = &view->GetGlobalCamera();
	else if (mode == 2)
		camera = view->GetPhysicalCamera();

	ConvertCameraToOrientation(camera->position, camera->target,camera->up,axis, angle);
	*positionX = camera->position.x,	*positionY = camera->position.y, *positionZ = camera->position.z;
	*orientationX = axis.x,	*orientationY = axis.y, *orientationZ = axis.z;
	*orientationAngle = angle;


}
/*
	compute bounding box of specified node,
	if node is NULL boundingbox of scene is computed
	mode 0 computing bounding box using bbox es of Group nodes if specified
	mode 1 ignore bbox es of Group nodes if specified

  Note : this function called on a transform node, will include the  effect of the transform on the bbox
  Returns FALSE if a bbox can't not computed for that node / scene graph 


*/
BOOL CGLViewCtrlCtrl::getNodeBoundingBox(LPUNKNOWN node, long mode, float FAR* xmin, float FAR* ymin, float FAR* zmin, float FAR* xmax, float FAR* ymax, float FAR* zmax) 
{
	if (!view) return FALSE;

	BBox bbox;
	Node *result=NULL;
	GvNode *n = NULL;
	int ret;
	  
    if (node != NULL) {
		// get Node interface 
		node->QueryInterface(IID_Node, (void **)&result);
		if (!result) return FALSE;
		// get the pointer to native GLView node 
		result->getNative((long *) &n);
		result->Release();
	}



	  GGvBBoxTraversal traversal;
	  traversal.useBBoxes = !(mode &1);

      traversal.InitializeAttributes();
      //traversal.Set(new GTimeStack(time)); // set current time
	  traversal.SetCamera(view->globalCamera);
	  traversal.SetPhysicalCamera(view->GetPhysicalCamera());

	  if (n == NULL)
		ret = view->Traverse(traversal);
	  else ret = n->Traverse(traversal);

      bbox = traversal.bbox;

	  *xmin = bbox.mi.x;
	  *ymin = bbox.mi.y;
	  *zmin = bbox.mi.z;

	  *xmax = bbox.ma.x;
	  *ymax = bbox.ma.y;
	  *zmax = bbox.ma.z;
	  
	  return(!bbox.IsEmpty());
}

/*

	load a new top level scene from url
	lastModified if > 0 specifies a date/time in time_t (GMT) format where url has last been modified
	if mode = 1 a refresh is forced

*/

void CGLViewCtrlCtrl::setUrl2(LPCTSTR url, long lastModified, long mode) 
{
	if (!view) {  // not initialized yet, store the url
		m_initialUrl = url;
		return;
	}

	gbool refresh = (mode & 1) !=0;

	ReadUrl(url,NULL,refresh,(time_t) lastModified); 

}

/*
	returns the blaxxuncc3d.odl interface version
	this allows runtime verification checks of the interfaces.

*/

long CGLViewCtrlCtrl::getInterfaceVersion() 
{

	return CURRENT_VERSION;
}

/*
	disable/enabled third person display mode
*/

void CGLViewCtrlCtrl::setThirdPersonView(BOOL mode) 
{
	if (!view) { 
		m_initialThirdPersonView = mode;
		return;
	}

	view->ShowAvatar(!!mode);
	view->SetThirdPersonView(!!mode);
	Redraw();

}

/*
	return true if thirdPerson view mode is active 
*/
BOOL CGLViewCtrlCtrl::getThirdPersonView() 
{
	if (!view) return FALSE;
	return view->thirdPersonView;
}

/*

  compute a ray intersection
	start - end two positions defining the ray
	startNode - the scene graph node to start computation (positions are local to this node)
	if startNode is NULL, the current scene is used for intersection testing
	
	if the result is TRUE rayHitinfo will point to a HitInfo node whithe the computed intersection information


*/

LPUNKNOWN CGLViewCtrlCtrl::computeRayHit(LPUNKNOWN startNode, float startx, float starty, float startz, float endx, float endy, float endz, long mode) 
{
	if (!view) return NULL;

	GvNode *fromNode=NULL;
    LPUNKNOWN rayHitInfo = NULL;

	// get nodes 
	if (startNode) { // get Node interface 
		Node *nodeI=NULL;
		startNode->QueryInterface(IID_Node, (void **)&nodeI);
		// get the pointer to native GLView node 
		if (nodeI) {
			nodeI->getNative((long *) &fromNode);
			nodeI->Release();
		}
	}
	Point p1(startx,starty,startz),p2(endx,endy,endz);

	GvRayHitInfo *info=NULL;

	info = new GvRayHitInfo();
	if (!info ) return NULL;

	// trigger selection
	if (info->OnTrigger(view,p1,p2,fromNode)) {
		info->QueryInterface(IID_IUnknown, (void **)&rayHitInfo); // and return node 
		return rayHitInfo;
	}
	else {
		delete info;

		return rayHitInfo;
	}
}

/*
	set the third person avatar node 
*/
void CGLViewCtrlCtrl::setMyAvatarNode(LPUNKNOWN node) 
{
	if (!view) return;

	GvNode *fromNode=NULL;

	// get nodes 
	if (node) { // get Node interface 
		Node *nodeI=NULL;
		node->QueryInterface(IID_Node, (void **)&nodeI);
		// get the pointer to native GLView node 
		if (nodeI) {
			nodeI->getNative((long *) &fromNode);
			nodeI->Release();
		}
	}

	view->myAvatar.set(fromNode);
	if (view->thirdPersonView) { // currently active, reload new 
		view->BindAvatar(fromNode);
	}

}

/*
	return the current third person avatar node
	or NULL if currently no avatar set or loaded from URL
*/


LPUNKNOWN CGLViewCtrlCtrl::getMyAvatarNode() 
{
	if (!view) return NULL;

	GvNode  *node= view->myAvatar.get();
	if (!node) return NULL;

	IUnknown* result=NULL;
	node->QueryInterface(IID_IUnknown, (void **)&result);

	return result;
}

/*
	set the Avatar Url for third person mode 
*/

void CGLViewCtrlCtrl::setMyAvatarURL(LPCTSTR url) 
{
	if (!view)  {
		m_initialAvatarUrl = url;
		return;
	}

	view->myAvatarURL.set(url);
	view->set_myAvatarURL(&view->myAvatarURL);

}

/*
	return the currently set avatar URL
*/
BSTR CGLViewCtrlCtrl::getMyAvatarURL() 
{

	CString strResult;
	if (!view) 
		strResult = m_initialAvatarUrl;
	else 
		strResult = view->myAvatarURL.get();

	return strResult.AllocSysString();
}

/*
	enable viewpoint follow mode,
	node is a scene graph node where upper level transforms are animated, eg an avatar
	ref sets a reference point in the local coordinate system of node, for tracking changes
	if node is NULL follow mode is turned off
*/

BOOL CGLViewCtrlCtrl::setViewpointFollow(LPUNKNOWN node, float refX, float refY, float refZ, long mode) 
{
	if (!view) return FALSE;

	Point p(refX,refY,refZ);
	if (node == NULL) {
		view->SetFollowObjectOff();
		return FALSE;
	}

	GvNode *fromNode=NULL;

	Node *nodeI=NULL;
	node->QueryInterface(IID_Node, (void **)&nodeI);
	// get the pointer to native GLView node 
	if (nodeI) {
		nodeI->getNative((long *) &fromNode);
		nodeI->Release();
		
	}

	return view->SetFollowObject(fromNode,p);

}

/*
	set value of eventIn named eventInName of node named nodeName
	TRUE on success
*/

BOOL CGLViewCtrlCtrl::setNodeEventIn(LPCTSTR nodeName, LPCTSTR eventInName, LPCTSTR value) 
{
	if (!view) return FALSE;

	GvScene * scene =  view->GetScene();
	if (!scene) return FALSE;
	// find the node
	GvNode *node = scene->getNode(nodeName);
	if (!node) return FALSE;
	EventIn *event=NULL;
	BOOL ret = FALSE;
	CComBSTR name(eventInName);

	// find eventOut
	if (node->getEventIn(name,&event) == S_OK && (event != NULL)) {
		if (updateLock == 0)
			view->SetTheTime(); // set global VRML eventTime

		// set as string 
		CComBSTR val(value);
		ret = (event->setValueFromString(val) == S_OK);
		event->Release();
		if (updateLock == 0)
			if ((view->needUpdate >0) || view->getRedraw()) Redraw();

	}
	return ret;

}

/*
	get value of eventOut named eventOutName of node named nodeName
	TRUE on success
*/

BSTR CGLViewCtrlCtrl::getNodeEventOut(LPCTSTR nodeName, LPCTSTR eventOutName)//,  FAR* value) 
{
/*
	if (!view) return FALSE;
	GvScene * scene =  view->GetScene();
	if (!scene) return FALSE;
	// find the node
	GvNode *node = scene->getNode(nodeName);
	if (!node) return FALSE;
	EventOut *event=NULL;
	BOOL ret = FALSE;
	CComBSTR name(eventOutName);

	// find eventOut
	if (node->getEventOut(name,&event) == S_OK && (event != NULL)) {
		// get as string 
		ret = (event->toString(value) == S_OK);
		event->Release();
	}
	*/
	
	if (!view) return NULL;

	GvScene * scene =  view->GetScene();
	if (!scene) return NULL;
	// find the node
	GvNode *node = scene->getNode(nodeName);
	if (!node) return NULL;
	EventOut *event=NULL;
	BSTR value = NULL;
	BSTR ret = NULL;
	CComBSTR name(eventOutName);

	// find eventOut
	if (node->getEventOut(name,&event) == S_OK && (event != NULL)) {
		// get as string 
		if (event->toString(&value) == S_OK)
			ret = value;
		else 
			ret = NULL;

		event->Release();
	}
	return ret;
}

/*
	return handle on some predefined internal browser objects
	
	 SoundInterface and DIRECTDRAW object s created, 
	 if sound device is not yet initilized


*/

LPUNKNOWN CGLViewCtrlCtrl::getObject(long objectId) 
{
	LPUNKNOWN ret = NULL;
	
	if (view) 
	switch(objectId) {


	case	BROWSER_OBJECT_DIRECTDRAW:
			ret = view->GetDirectDraw(); break;

#ifdef _D3D

	case	BROWSER_OBJECT_DIRECT3D:
				ret = view->device->lpD3D; break;

	case	BROWSER_OBJECT_DIRECT3DDEVICE:
				ret = view->device->lpD3DDevice; break;

	case	BROWSER_OBJECT_DIRECT3DVIEWPORT:
				ret = view->device->lpD3DViewport; break;

	case	BROWSER_OBJECT_DD_FRONTBUFFER:
			ret = view->device->lpFrontBuffer; break;

	case	BROWSER_OBJECT_DD_BACKBUFFER:
			ret = view->device->lpBackBuffer; break;

	case	BROWSER_OBJECT_DD_ZBUFFER:
			ret = view->device->lpZBuffer; break;
#endif


	case	BROWSER_OBJECT_DIRECTSOUND: {
				GSoundDevice *s = view->GetSoundDevice();
				if (!s) break;
				if (RTISA(s,GDSSoundDevice)) {
					ret = ((GDSSoundDevice *) s)->m_lpDS;
				}

			};
			break;
	case	BROWSER_OBJECT_DIRECTSOUND3DLISTENER:
			{
				GSoundDevice *s = view->GetSoundDevice();
				if (!s) break;
				if (RTISA(s,GDSSoundDevice)) {
					ret = ((GDSSoundDevice *) s)->m_lpDL;
				}

			};
			break;


	case	BROWSER_OBJECT_RSX:
			{
				GSoundDevice *s = view->GetSoundDevice();
				if (!s) break;
				if (RTISA(s,GRSXSoundDevice)) {
					ret = ((GRSXSoundDevice *) s)->m_lpRSX;
				}

			};
			break;
	case	BROWSER_OBJECT_RSX_LISTENER:
			{
				GSoundDevice *s = view->GetSoundDevice();
				if (!s) break;
				if (RTISA(s,GRSXSoundDevice)) {
					ret = ((GRSXSoundDevice *) s)->m_lpDL;
				}

			};
			break;
	default : break;
	}


	if (ret) {
		ret->AddRef();
	}

	return ret;
}

void CGLViewCtrlCtrl::OnPopupPanel() 
{
	if (view->PanelOk())
	{
		view->NavigationPanelDestroy();
		view->setNavigationPanelProp(FALSE);
	}
	else {

		if (view->allowAnyNavigation)
		{
			view->NavigationPanelCreate(1);
			view->setNavigationPanelProp(TRUE);
		}
	}
}

void CGLViewCtrlCtrl::OnUpdatePopupPanel(CCmdUI* pCmdUI) 
{
	if (view) {
		if (view->PanelOk()) {
			pCmdUI->SetCheck(TRUE);
			pCmdUI->Enable(view->panel->Ok());
		} else {
			pCmdUI->SetCheck(FALSE);
			pCmdUI->Enable(view->allowAnyNavigation); // & registry ??
		}
	}
}



void CGLViewCtrlCtrl::setNavigationPanel(BOOL mode) 
{
	m_navPanelOn_Embed = mode;
	if (!view) return;

	view->setNavigationPanelProp(!!mode);
	if (view && !view->m_navPanelOn && view->panel)
		view->NavigationPanelDestroy();
}	

BOOL CGLViewCtrlCtrl::getNavigationPanel() 
{
	if (!view) return m_navPanelOn_Embed;
	return view->m_navPanelOn;
}

void CGLViewCtrlCtrl::OnRButtonUp(UINT nFlags, CPoint point) 
{
	if (!m_initialized) return;

	if (view) {
		ViewLock viewLock(view); 
		view->SetTheTime(); // set global VRML eventTime
		if (view->triggerMouseEvent(GEV_mouseup, "mouseup",point.x,point.y,nFlags | MK_RBUTTON))
			return;
	}
	if (view->GetFullScreen()) 
		return;  // ignore in fullscreen mode 

	m_rlastPoint = point;
	m_rlastFlags = nFlags;

	ClientToScreen(&point);

	// make sure window is active
	
	//GetParentFrame()->ActivateFrame();

	if (m_uiMode == UI_NONE) return;


	CMenu menu;
		if (menu.LoadMenu(IDR_MAINFRAME_POPUP))
		{
			int menueId =0;

			int vpSubMenu = 0; // indices of entrys 
			int viewerMenu = 2;

			if (m_uiMode == UI_SIMPLE) { 
				menueId=2; 
				vpSubMenu = 0;
				viewerMenu = 1;				
			}



			CMenu* pPopup = menu.GetSubMenu(menueId);

			CString s;

			ASSERT(pPopup != NULL);

			if (!view->allowAnyNavigation) 
			{ // navigation info type = NONE & not any, remove all UI

				if (viewerMenu>=0)  pPopup->DeleteMenu(viewerMenu, MF_BYPOSITION);
				if (vpSubMenu>=0) pPopup->DeleteMenu(vpSubMenu, MF_BYPOSITION);

				vpSubMenu = -1;
				viewerMenu = -1;
			}				

			
			// Translate it 
			Translate(*pPopup);

			// Append viewpoints to menu 
			if (vpSubMenu >=0) {	
				GvSceneInfo *sceneInfo = view->GetSceneInfo();

				CMenu* v = pPopup->GetSubMenu(vpSubMenu); // this must be the viewpoint menu
				if (v && sceneInfo) {
					int n = sceneInfo->cameras.Length();
					if (n>25) n = 25;
					
					v->DeleteMenu( 7, MF_BYPOSITION ); // delete the dummy entry 
  
					for (int i=0; i<n; i++)  {
						GvNode *n = sceneInfo->cameras[i];
						s = n->getName().getString();

						if (RTISA(n,GvViewpoint)) {	 
							if (n->refCnt <=1) 	continue;
							s = ((GvViewpoint *) n)->description.get();
							
							// ignore internal viewpoints in display
							if (s.GetLength() == 0) continue;
							s = Translate(s);
                        } 
						
						if (s.GetLength() >50) s = s.Left(50); // truncate 

						v->AppendMenu(MF_STRING, ID_VIEWPOINT_0+i,s);
					}
				}

			}

			// build custom menus 
			{
				GvSceneInfo *s = view->GetSceneInfo(); 
				int cnt = min(MENU_MAX,s->menueSensors.getNum());

				BOOL needSep = TRUE;
				if (cnt >0)

				for(int i=0; i< cnt; i++) {
					int j;

					GvMenuSensor *m = (GvMenuSensor *) (GvNode*) s->menueSensors[i];
					
					CMenu* v = pPopup->GetSubMenu(vpSubMenu); // this must be the viewpoint menu

					if (!m || !m->enabled) continue;
					int numEntries = min(m->choices.getNum(),m->descriptions.getNum());
					
					numEntries = min(numEntries,MENU_MAX);

					CString where = m->position.get();
					
					CMenu pop;
					pop.CreateMenu( );
					
					for (j=0; j<numEntries;j++) {
						if (m->choices[j] == -1) 
							pop.AppendMenu(MF_SEPARATOR,ID_MENUE(i,j),m->descriptions[j]);
						else pop.AppendMenu(MF_STRING,ID_MENUE(i,j),Translate(m->descriptions[j]));
					}
					
					if (where == "TOP") {
						pPopup->InsertMenu(0, MF_BYPOSITION |MF_SEPARATOR ,NULL,"");
						pPopup->InsertMenu(0, MF_BYPOSITION |MF_POPUP | MF_STRING,(int) (HMENU) pop,m->title);
					}
					else {
						if (needSep) {
							pPopup->AppendMenu(MF_SEPARATOR,NULL,"");
							needSep = FALSE;
						}

						pPopup->AppendMenu(MF_POPUP | MF_STRING,(int) (HMENU) pop,Translate(m->title));
					}

					pop.Detach();
				}
			} 

			if (m_historyIndex>0 && !view->observer) {
				Translate(_T("&Back"),s);
 				pPopup->AppendMenu(MF_STRING, ID_GO_BACK, s);
			}

#ifdef _D3D
			// Append drivers to menu 
			if (m_uiMode != UI_SIMPLE)
			{	
				GD3Device *d = view->device;
				pPopup->AppendMenu(MF_SEPARATOR, 0,"");
				for (int i=0; i<d->NumDrivers; i++) {
					s = d->Driver[i].NameUi;
					//if (d->Driver[i].bCanDoWindow) 
					{ // if not fitting windows mode checked in OnUpdateOptionsDriver(CCmdUI* pCmdUI)
						s=Translate(s);
						pPopup->AppendMenu(MF_STRING , ID_DRIVER_0+i,s);
					}
				}

			}
#endif  
		SetCursorMode(NAV_NONE); // HG 

		// HG new test 
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));

		m_NavNotActive = TRUE;
		m_NavCursorModeOld = -2;
		m_menuActive = TRUE;
		//AfxLockTempMaps();

		{
			// HG test, try to set prev module state // see
			
			// thilo 15.07.99: folgende zeile führt zum crash in netscape
			// verstehe auch nicht, was die hier bewirken soll, deshalb auskommentiert.
			// AFX_MANAGE_STATE(AfxGetThreadState()->m_pPrevModuleState);

			// VERIFY(AfxSetModuleState(AfxGetThreadState()->m_pPrevModuleState) == &afxModuleState);
			// CHandleMap* pMap = afxMapHWND(TRUE); //create map if not exist
			// ASSERT(pMap != NULL);
			// CWnd* pWnd = (CWnd*)pMap->FromHandle(hWnd);

			//pPopup->TrackPopupMenu(TPM_RIGHTBUTTON | TPM_LEFTALIGN, point.x, point.y,   this); 
			DoThreadModal(pPopup, &point, this);

		}
		//AfxUnlockTempMaps();
		pPopup->DestroyMenu(); // according to doc called also in destructor
		CMenu::DeleteTempMap(); // clean up temp CMenus 
		m_menuActive = FALSE;

		}

		else COleControl::OnRButtonUp(nFlags, point);
}

void CGLViewCtrlCtrl::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	if (view) {
		ViewLock viewLock(view); 
		view->SetTheTime(); // set global VRML eventTime
		if (view->triggerMouseEvent(GEV_dblclick, "dblclick",point.x,point.y,nFlags))
			return;
	}
	
	COleControl::OnRButtonDblClk(nFlags, point);
}

void CGLViewCtrlCtrl::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
/*

#if _MFC_VER >= 0x0421 
	COleControl::OnSettingChange(uFlags, lpszSection);
#endif

*/
	
	// TODO: Add your message handler code here
	
}

void CGLViewCtrlCtrl::OnMButtonDown(UINT nFlags, CPoint point) 
{
	if (view) {
		ViewLock viewLock(view); 
		view->SetTheTime(); // set global VRML eventTime
		if (view->triggerMouseEvent(GEV_mousedown, "mousedown",point.x,point.y,nFlags))
			return;
	}
	
	COleControl::OnMButtonDown(nFlags, point);
}

void CGLViewCtrlCtrl::OnMButtonUp(UINT nFlags, CPoint point) 
{
	if (view) {
		ViewLock viewLock(view); 
		view->SetTheTime(); // set global VRML eventTime
		if (view->triggerMouseEvent(GEV_mouseup, "mouseup",point.x,point.y,nFlags | MK_MBUTTON))
			return;
	}
	
	COleControl::OnMButtonUp(nFlags, point);
}

void CGLViewCtrlCtrl::OnMButtonDblClk(UINT nFlags, CPoint point) 
{
	if (view) {
		ViewLock viewLock(view); 
		view->SetTheTime(); // set global VRML eventTime
		if (view->triggerMouseEvent(GEV_dblclick, "dblclick",point.x,point.y,nFlags))
			return;
	}

	COleControl::OnMButtonDblClk(nFlags, point);
}

void CGLViewCtrlCtrl::OnInitMenu(CMenu* pMenu) 
{

	TRACE("CGLViewCtrlCtrl::OnInitMenu() %p\n",this);	
	COleControl::OnInitMenu(pMenu);
	
	// TODO: Add your message handler code here

}

void CGLViewCtrlCtrl::OnFinalRelease() 
{
	// TODO: Add your specialized code here and/or call the base class
	TRACE("CGLViewCtrlCtrl::OnFinalRelease() %p\n",this);	
	
	COleControl::OnFinalRelease();
}

void CGLViewCtrlCtrl::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	TRACE("CGLViewCtrlCtrl::OnContextMenu() %p\n",this);	
}
