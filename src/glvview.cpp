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
/*******************************************************************************
@doc

@module GLVView.cpp - GLView CMyglView MFC View Class implementation	|

Copyright (c) 1995 - 1998 by Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:
all menu and dialog popup functions are implemented here

Changes:

Todo :

******************************************************************************/

#include "stdafx.h"
#include "gversion.h"

#include "glviewhdr.h"
#include "MainFrm.h"

#ifdef _OGL
#include "COpenGL.h"
#include "pixform.h"
#include "pfdinfo.h"
#endif

#ifdef _D3D
#include "DD3DOptions.h"
#endif

#include "mmsystem.h"

#include "gcamera.h"

#include "grender.h"

#include "glrenderbitmap.h"
#include "glutils.h"

#include "gkatmai.h"


//#ifdef _DEBUG
//#define _MIRO
//#endif




#include "gutils.h"
#include "gclass.h"
#include "gshell.h"
#include "gshellop.h"
#include "gshelli.h"
#include "gshellio.h"
#include "gfont.h"
#include "ggeom.h"
#include "greporter.h"

#ifdef _DXF
#include "gdxfin.h"
#endif

#include "ronny.h"


#include "gmodel.h"

#define DRAG_DEFORMER_MODE (JUMP_TO_MODE +1)

#define LOOKAT_OBJECT_MODE JUMP_TO_MODE


// dialogs
// ???
#include "resource.h"

#include "dmateria.h"
#include "dtexture.h"
#include "dbackgro.h"
#include "dfog.h"
#include "doptions.h"

#include "dstyle.h"
#include "ddrawsty.h"

#include "dobjnew.h"

#include "dtime.h"
#include "dcamera.h"


// dialog property pages
#include "dstylep.h"

#include "DConsole.h"


// Deformers
#include "gsurface.h"
#include "gdeform.h"
#include "ddefsph.h"


#include "DTransform.h"
#include "DProp.h"

//#ifdef _AVI
//#include "avi\avisup.h"
//#endif

// WWW
#include "www.h"
#include "DWWW.h"
#include "gurlcache.h"

#ifdef _MIRO

#include "gmiro.h"

#endif





// from Gv
#include <gvnodes.h>

#include <gvscene.h>
#include <gvtraverse.h>
#include <gvconnection.h>

#include "grayselect.h"

#include "giotraversal.h"
#include "gdisplaytree.h"
#include "goptimizetraversal.h"
#include "gnurbs.h"


// TreeEditor
#include "DTreeEditor.h"

#include "DRenderFile.h"

// EventHandlers
#include "gevent.h"
#include "gedit.h"


#include "gsound.h"

#include <time.h>



#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define ORDER_INFO "\nfor ordering information please see file order.txt and the About Box"

// Error message

int __cdecl Error (TCHAR *message, ...) 
{ // see also mfc\src\dumpout.cpp
	va_list args;
	va_start(args, message);

	int nBuf;
	TCHAR szBuffer[4096];

	nBuf = _vstprintf(szBuffer, message, args);
	ASSERT(nBuf < sizeof(szBuffer));

//	afxDump << szBuffer;

	va_end(args);
	int ret=AfxMessageBox(szBuffer,MB_OK);
	TRACE0(szBuffer);
	return(ret);

}

// Report status messages to statusbar
class CMyglViewReporter : public GReporter
{


public : 
	CMyglView *m_ctrl;

	CMyglViewReporter(CMyglView *ctrl) : m_ctrl(ctrl) {}


	virtual int Status1(const char * message);
	virtual int Trace1(const char * message);
	virtual int Warning1(const char * message);
	virtual int Error1(const char * message);

};


int 
CMyglViewReporter::Status1(const char * message)
{

  if (!TryLock()) return -1;

  //CFrameWnd *pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);
  // new 10.11.95 like in Mainframe

  CMainFrame *pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
  pFrame->SetMessageText(message);
  
  pFrame->m_wndStatusBar.UpdateWindow();

  //pFrame->UpdateWindow();
  UnLock();

  return(0);
}

int 
CMyglViewReporter::Trace1(const char * message)
{
	// if (ignoreWarnings) return -1;

	if (!TryLock()) return -1;

	//Lock();

	if (m_ctrl) {
		// protected !!!!!!! AFX_MANAGE_STATE(m_ctrl->m_pModuleState); // XX
		if (m_ctrl->dialogs.m_dConsole) {
			m_ctrl->dialogs.m_dConsole->AddText(message); 
			if (!m_ctrl->dialogs.m_dConsole->m_hide) m_ctrl->dialogs.m_dConsole->ShowWindow(SW_SHOWNA);
		}
	}

	UnLock();
	return 0;
}

int 
CMyglViewReporter::Warning1(const char * message)
{	
	if (ignoreWarnings) return -1;

	if (!TryLock()) return -1;

	if (m_ctrl) {
		m_ctrl->Message(message,PROGRESS_MESSAGE);

		// protected !!!!!!! AFX_MANAGE_STATE(m_ctrl->m_pModuleState); // XX
		if (m_ctrl->dialogs.m_dConsole) {
			m_ctrl->dialogs.m_dConsole->AddText(message); 
			if (!m_ctrl->dialogs.m_dConsole->m_hide) m_ctrl->dialogs.m_dConsole->ShowWindow(SW_SHOWNA);
		}
		else {
			 GReporter::Warning1(message);
		}
	}
	UnLock();

	return 0;
}


int 
CMyglViewReporter::Error1(const char * message)
{
	Status1(message);
	if (m_ctrl) {
		m_ctrl->Message(message,PROGRESS_MESSAGE);

		// protected !!!!!!! AFX_MANAGE_STATE(m_ctrl->m_pModuleState); // XX
		if (m_ctrl->dialogs.m_dConsole) {
			m_ctrl->dialogs.m_dConsole->AddText(message); 
			if (!m_ctrl->dialogs.m_dConsole->m_hide) m_ctrl->dialogs.m_dConsole->ShowWindow(SW_SHOWNA);
		}

		else GReporter::Error1(message);
	}
	return 0;
}



/////////////////////////////////////////////////////////////////////////////
// CMyglView
IMPLEMENT_DYNCREATE(CMyglView, CView)

BEGIN_MESSAGE_MAP(CMyglView, CView)
	//{{AFX_MSG_MAP(CMyglView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_PIXFMT_CHOOSE, OnPixfmtChoose)
	ON_COMMAND(ID_PIXFMT_DESCRIBE, OnPixfmtDescribe)
	ON_COMMAND(ID_PIXFMT_ENUM, OnPixfmtEnum)
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_CAMERA_DOLLY_XY, OnCameraDollyXy)
	ON_COMMAND(ID_CAMERA_ORBIT, OnCameraOrbit)
	ON_COMMAND(ID_CAMERA_PAN, OnCameraPan)
	ON_COMMAND(ID_CAMERA_RESET, OnCameraReset)
	ON_COMMAND(ID_CAMERA_WALK, OnCameraWalk)
	ON_COMMAND(ID_CAMERA_ZOOM, OnCameraZoom)
	ON_COMMAND(ID_RENDER_WIREFRAME, OnRenderWireframe)
	ON_COMMAND(ID_RENDER_SOLID, OnRenderSolid)
	ON_COMMAND(ID_RENDER_FLAT, OnRenderFlat)
	ON_COMMAND(ID_RENDER_GOURAUD, OnRenderGouraud)
	ON_COMMAND(ID_CAMERA_ROLL, OnCameraRoll)
	ON_COMMAND(ID_CAMERA_SPINAUTO, OnCameraSpinauto)
	ON_WM_TIMER()
	ON_COMMAND(ID_FILE_READ, OnFileRead)
	ON_COMMAND(ID_RENDER_VERTICES, OnRenderVertices)
	ON_COMMAND(ID_RENDER_LIGHTED, OnRenderLighted)
	ON_COMMAND(ID_RENDER_NORMALS, OnRenderNormals)
	ON_COMMAND(ID_RENDER_TEXTURED, OnRenderTextured)
	ON_COMMAND(ID_RENDER_OPTIONS_BACKFACECULL, OnRenderOptionsBackfacecull)
	ON_COMMAND(ID_RENDER_OPTIONS_2SIDEDLIGHTING, OnRenderOptions2sidedlighting)
	ON_COMMAND(ID_RENDER_OPTIONS_LOCALVIEWER, OnRenderOptionsLocalviewer)
	ON_COMMAND(ID_RENDER_OPTIONS_ANTIALIASLINES, OnRenderOptionsAntialiaslines)
	ON_COMMAND(ID_RENDER_OPTIONS_LOADTEXTURE, OnRenderOptionsLoadtexture)
	ON_COMMAND(ID_RENDER_COLORED, OnRenderColored)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_DOLLY_XY, OnUpdateCameraDollyXy)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_ORBIT, OnUpdateCameraOrbit)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_PAN, OnUpdateCameraPan)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_ZOOM, OnUpdateCameraZoom)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_ROLL, OnUpdateCameraRoll)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_WALK, OnUpdateCameraWalk)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_SPINAUTO, OnUpdateCameraSpinauto)
	ON_UPDATE_COMMAND_UI(ID_RENDER_WIREFRAME, OnUpdateRenderWireframe)
	ON_UPDATE_COMMAND_UI(ID_RENDER_VERTICES, OnUpdateRenderVertices)
	ON_UPDATE_COMMAND_UI(ID_RENDER_SOLID, OnUpdateRenderSolid)
	ON_UPDATE_COMMAND_UI(ID_RENDER_FLAT, OnUpdateRenderFlat)
	ON_UPDATE_COMMAND_UI(ID_RENDER_GOURAUD, OnUpdateRenderGouraud)
	ON_UPDATE_COMMAND_UI(ID_RENDER_LIGHTED, OnUpdateRenderLighted)
	ON_UPDATE_COMMAND_UI(ID_RENDER_COLORED, OnUpdateRenderColored)
	ON_UPDATE_COMMAND_UI(ID_RENDER_TEXTURED, OnUpdateRenderTextured)
	ON_UPDATE_COMMAND_UI(ID_RENDER_NORMALS, OnUpdateRenderNormals)
	ON_UPDATE_COMMAND_UI(ID_RENDER_OPTIONS_BACKFACECULL, OnUpdateRenderOptionsBackfacecull)
	ON_UPDATE_COMMAND_UI(ID_RENDER_OPTIONS_2SIDEDLIGHTING, OnUpdateRenderOptions2sidedlighting)
	ON_UPDATE_COMMAND_UI(ID_RENDER_OPTIONS_LOCALVIEWER, OnUpdateRenderOptionsLocalviewer)
	ON_UPDATE_COMMAND_UI(ID_RENDER_OPTIONS_ANTIALIASLINES, OnUpdateRenderOptionsAntialiaslines)
	ON_COMMAND(ID_MATERIAL_LOAD, OnMaterialLoad)
	ON_COMMAND(ID_MATERIAL_EDIT, OnMaterialEdit)
	ON_COMMAND(ID_MATERIAL_BACKGROUND, OnMaterialBackground)
	ON_COMMAND(ID_RENDER_OPTIONS_REVCULLING, OnRenderOptionsRevCulling)
	ON_UPDATE_COMMAND_UI(ID_RENDER_OPTIONS_REVCULLING, OnUpdateRenderOptionsRevCulling)
	ON_COMMAND(ID_MATERIAL_TEXTURE_OPTIONS, OnMaterialTextureOptions)
	ON_COMMAND(ID_MATERIAL_BACKGROUND_OPTIONS, OnMaterialBackgroundOptions)
	ON_COMMAND(ID_DEFORM_SPHERE, OnDeformSphere)
	ON_COMMAND(ID_TOOLS_DEFORM_DISABLE, OnToolsDeformDisable)
	ON_COMMAND(ID_MATERIAL_FOG, OnFog)
	ON_COMMAND(ID_MATERIAL_PARAMETER, OnParameter)
	ON_COMMAND(ID_OPTIONS_CONFIG, OnOptionsConfig)
	ON_COMMAND(ID_RENDER_STYLE, OnRenderStyle)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_SAVEAS, OnFileSaveas)
	ON_COMMAND(ID_CAMERA_AUTOSPIN, OnCameraAutospin)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_AUTOSPIN, OnUpdateCameraAutospin)
	ON_COMMAND(ID_OBJECT_FLIPNORMALS, OnObjectFlipnormals)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_FLIPNORMALS, OnUpdateObjectFlipnormals)
	ON_COMMAND(ID_TOOLS_BUILD_SHELL, OnToolsBuildShell)
	ON_WM_PALETTECHANGED()
	ON_WM_QUERYNEWPALETTE()
	ON_COMMAND(ID_TOOLS_TRIANGULATE, OnToolsTriangulate)
	ON_COMMAND(ID_FILE_NEWFONTLOGO, OnFileNewfontlogo)
	ON_COMMAND(ID_TOOLS_INFO, OnToolsObjectInfo)
	ON_COMMAND(ID_RENDER_TIME, OnRenderTime)
	ON_UPDATE_COMMAND_UI(ID_RENDER_TIME, OnUpdateRenderTime)
	ON_COMMAND(ID_TOOLS_MORPH, OnToolsMorph)
	ON_COMMAND(ID_RENDER_BOXES, OnRenderBoxes)
	ON_UPDATE_COMMAND_UI(ID_RENDER_BOXES, OnUpdateRenderBoxes)
	ON_COMMAND(ID_WWW_OPENURL, OnWwwOpenUrl)
	ON_UPDATE_COMMAND_UI(ID_WWW_OPENURL, OnUpdateWwwOpenUrl)
	ON_UPDATE_COMMAND_UI(ID_WWW_ABORTLOAD, OnUpdateWwwAbortLoad)
	ON_COMMAND(ID_WWW_ABORTLOAD, OnWwwAbortLoad)
	ON_COMMAND(ID_WWW_OPTIONS, OnWwwOptions)
	ON_WM_ENTERIDLE()
	ON_COMMAND(ID_CAMERA_VIEWPOINTS, OnCameraViewpoints)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_VIEWPOINTS, OnUpdateCameraViewpoints)
	ON_WM_CLOSE()
	ON_COMMAND(ID_WWW_LOADALLINLINES, OnWwwLoadAllInlines)
	ON_UPDATE_COMMAND_UI(ID_WWW_LOADALLINLINES, OnUpdateWwwLoadAllInlines)
	ON_COMMAND(ID_CAMERA_FLY, OnCameraFly)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_FLY, OnUpdateCameraFly)
	ON_COMMAND(ID_RENDER_TRANSPARENCY, OnRenderTransparency)
	ON_UPDATE_COMMAND_UI(ID_RENDER_TRANSPARENCY, OnUpdateRenderTransparency)
	ON_COMMAND(ID_RENDER_NONLIGHTED, OnRenderNonLighted)
	ON_UPDATE_COMMAND_UI(ID_RENDER_NONLIGHTED, OnUpdateRenderNonLighted)
	ON_COMMAND(ID_WWW_RELOAD, OnWwwReload)
	ON_COMMAND(ID_RENDER_ANIMATE, OnRenderAnimate)
	ON_UPDATE_COMMAND_UI(ID_RENDER_ANIMATE, OnUpdateRenderAnimate)
	ON_COMMAND(ID_TOOLS_TREE_EDITOR, OnToolsTreeEditor)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_TREE_EDITOR, OnUpdateToolsTreeEditor)
	ON_COMMAND(IDD_CAMERA_RELATIVE, OnCameraRelative)
	ON_UPDATE_COMMAND_UI(IDD_CAMERA_RELATIVE, OnUpdateCameraRelative)
	ON_COMMAND(ID_FILE_RENDER, OnFileRender)
	ON_UPDATE_COMMAND_UI(ID_FILE_RENDER, OnUpdateFileRender)
	ON_COMMAND(ID_OPTIONS_GL_INFO, OnOptionsGlInfo)
	ON_COMMAND(ID_MATERIAL_BACKGROUND_IMAGE, OnMaterialBackgroundImage)
	ON_COMMAND(ID_FILE_RENDER_TO_FILE, OnFileRenderToFile)
	ON_COMMAND(ID_MATERIAL_BACKGROUND_RESET, OnMaterialBackgroundReset)
	ON_UPDATE_COMMAND_UI(ID_MATERIAL_BACKGROUND_RESET, OnUpdateMaterialBackgroundReset)
	ON_COMMAND(ID_OPTIONS_DIRECT3D, OnOptionsDirect3d)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_DIRECT3D, OnUpdateOptionsDirect3d)
	ON_COMMAND(ID_OPTIONS_FULLSCREEN, OnOptionsFullscreen)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_FULLSCREEN, OnUpdateOptionsFullscreen)
	ON_WM_MOVE()
	ON_WM_ACTIVATE()
	ON_WM_ACTIVATEAPP()
	ON_COMMAND(ID_CAMERA_ANIMATE_VIEWPOINTS, OnCameraAnimateViewpoints)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_ANIMATE_VIEWPOINTS, OnUpdateCameraAnimateViewpoints)
	ON_COMMAND(ID_CAMERA_VIEWPOINT_NEXT, OnCameraViewpointNext)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_VIEWPOINT_NEXT, OnUpdateCameraViewpointNext)
	ON_COMMAND(ID_CAMERA_VIEWPOINT_PREV, OnCameraViewpointPrev)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_VIEWPOINT_PREV, OnUpdateCameraViewpointPrev)
	ON_COMMAND(ID_CAMERA_SEEKTO, OnCameraSeekToLast)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_SEEKTO, OnUpdateCameraSeekToLast)
	ON_COMMAND(ID_TOOLS_TRANSFORM, OnToolsTransform)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_TRANSFORM, OnUpdateToolsTransform)
	ON_COMMAND(ID_CAMERA_NONE, OnCameraNone)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_NONE, OnUpdateCameraNone)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_TRIANGULATE, OnUpdateToolsTriangulate)
	ON_COMMAND(ID_CAMERA_HEADLIGHT, OnCameraHeadLight)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_HEADLIGHT, OnUpdateCameraHeadLight)
	ON_UPDATE_COMMAND_UI(ID_DEFORM_SPHERE, OnUpdateDeformSphere)
	ON_COMMAND(ID_CAMERA_FOLLOW_OBJECT, OnCameraFollowObject)
	ON_COMMAND(ID_CAMERA_COLLISION, OnCameraCollision)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_COLLISION, OnUpdateCameraCollision)
	ON_COMMAND(ID_TOOLS_OPTIMIZE, OnToolsOptimize)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_OPTIMIZE, OnUpdateToolsOptimize)
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_CAMERA_JUMPTO, OnCameraJumpto)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_JUMPTO, OnUpdateCameraJumpto)
	ON_COMMAND(ID_RENDER_SOUND, OnRenderSound)
	ON_UPDATE_COMMAND_UI(ID_RENDER_SOUND, OnUpdateRenderSound)
	ON_COMMAND(ID_OPTIONS_SHOWAVATAR, OnOptionsShowAvatar)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_SHOWAVATAR, OnUpdateOptionsShowAvatar)
	ON_COMMAND(ID_CAMERA_GRAVITY, OnCameraGravity)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_GRAVITY, OnUpdateCameraGravity)
	ON_WM_MOUSEACTIVATE()
	ON_COMMAND(ID_TOOLS_METAVERSE, OnToolsMetaverse)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_METAVERSE, OnUpdateToolsMetaverse)
	ON_COMMAND(ID_TOOLS_LIBRARY, OnToolsLibrary)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_LIBRARY, OnUpdateToolsLibrary)
	ON_COMMAND(ID_OPTIONS_PREFERENCES, OnOptionsPreferences)
	ON_WM_PAINT()
	ON_MESSAGE(WM_READFILECOMPLETED, OnReadFileCompleted) 
	ON_MESSAGE(WM_DIALOG_CLOSED, OnDialogClosed) 
	ON_MESSAGE(WM_SIZING, OnSizing) 
	ON_MESSAGE(WM_MOVING, OnMoving) 
	ON_MESSAGE(WM_CAPTURECHANGED, OnCaptureChanged) 
	ON_COMMAND(ID_TOOLS_VRML2_CON, OnToolsVrml2Convert)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_VRML2_CON, OnUpdateToolsVrml2Convert)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_VIEWPOINT_0,ID_VIEWPOINT_19,OnCameraViewpoint)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEWPOINT_0,ID_VIEWPOINT_19,OnUpdateCameraViewpoint)

	ON_COMMAND_RANGE(ID_DRIVER_0,ID_DRIVER_9,OnOptionsDriver)
	ON_UPDATE_COMMAND_UI_RANGE(ID_DRIVER_0,ID_DRIVER_9,OnUpdateOptionsDriver)

END_MESSAGE_MAP()


// Utilities

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
static BOOL GetRegKey(HKEY hkey,LPCTSTR lpszKey, CString &value)
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

static BOOL GetRegKey(HKEY hkey,LPCTSTR lpszKey, int &value)
{
	CString v;
	if (GetRegKey(hkey,lpszKey,v)) {

		int ret= sscanf((const char *)v,"%d",&value);
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



// GetTimeTick
float GetTime() { return (float) clock()  / (float) CLOCKS_PER_SEC; }


int CstringToPoint(const CString &s,Point &p)
{
  return (sscanf(s,"%f %f %f",&p.x,&p.y,&p.z));
}

int CstringFromPoint(CString &s,const Point &p)
{
  return(sprintf(s.GetBuffer(180),"%g %g %g",p.x,p.y,p.z));
}

int CstringFromColor(CString &s,const Point &p)
{
  return(sprintf(s.GetBuffer(180),"%1.2g %1.2g %1.2g",p.x,p.y,p.z));
}


CTranslator::CTranslator() : m_section(_T("CC3D"))
{
	m_pApp = AfxGetApp();
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

#ifdef _DEBUG_0000
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
	if (value.GetLength() == 0) return FALSE;
	Replace(value,"\\t","\t",value);
	TRACE(" Lookup '%s' = '%s' \n",(const char *) key,(const char *) value);
	// and store in hash table
	m_hashTable[key] = value;


	return TRUE;
}


#if 0

LPCTSTR Translate(const char* id, LPCTSTR string)
{
		TRACE("%s=%s\n",id,string);
		return string;

}
#endif




// translate a message 
BOOL CMyglView::Translate(LPCTSTR key,LPCTSTR valueDefault, CString& value) {
	return m_translator.Translate(key,valueDefault,value);
}

BOOL CMyglView::Translate(LPCTSTR valueDefault, CString& value) 
{
	return m_translator.Translate(valueDefault,valueDefault,value);
}

BOOL CMyglView::TranslateMessageString(UINT nID, CString& value)
{
	char buf[60];
	buf[0]='s',
	_itoa(nID,&buf[1],10); CString valueDefault;
	if (m_translator.Lookup(buf,value)) return TRUE;
	//CWnd::GetMessageString(nID, valueDefault);
	return m_translator.Translate(buf,valueDefault,value);
}
//#define TRANS(a,b) Translate(#a,b)

#if 0
// Translate a CMenu
void CMyglView::Translate(CMenu &m) 
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

#endif


static int check_level=0;
static HWND  event_checker_hWnd=0;

int  CheckForInterrupt()
{
	MSG msg;


#if 0
	if (check_level>0) {  // recursive entry
	return(1);
	}
	check_level++;
	/* transparently process messages, while inside a update cycle */
	while (PeekMessage (&msg, /* NULL*/ event_checker_hWnd, 0, 0, PM_NOREMOVE) &&
	   GetMessage (&msg, NULL, 0, 0)) {
	   TranslateMessage (&msg);
	   DispatchMessage (&msg);
	   if (do_abort_update >0) break;
	}
	check_level--;
	if (do_abort_update && (check_level == 0)) {
		return(1);
		do_abort_update=0;
	}

#else
	while (1) {
		if (PeekMessage (&msg, event_checker_hWnd, 0, 0, PM_NOREMOVE)) {
		/* We only want to stop if it is a message we care about */
		switch (msg.message) {
		case WM_MOUSEMOVE:
			/* We don't care about this message while updating  */
			GetMessage (&msg, event_checker_hWnd, 0, 0);
			return(0); // ??
			break;
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_KEYDOWN:
			/* Now to avoid confusing other routines and since
			   If am sure they really don't want this processed
			   by the rest of the program */
	   // HG ?????????????		GetMessage (&msg, event_checker_hWnd, 0, 0);
			return(1);
		default:
			return(0);
		}
		}
		else  /* no message */
		{
			return(0);
		   }
   }
#endif
}

/////////////////////////////////////////////////////////////////////////////
// DDialogControl construction/destruction

DDialogControl::DDialogControl()
{
   // member dialogs : set NULL
	m_dMaterial = NULL;
	m_dTexture = NULL;
	m_dParameter = NULL;
	m_dBackground = NULL;
	m_dFog = NULL;
	m_dDrawStyle = NULL;

	m_dObjectNew = NULL;
	m_dNewFontLogo = NULL;
	m_dTime = NULL;
	m_dMorph = NULL;
    m_dDeformSphere = NULL;

	m_dOpenUrl = NULL;
	m_dViewpoint = NULL;
	m_dTreeEditor = NULL;

	m_dTransform = NULL;
	m_dConsole = NULL;

	m_dLibrary = NULL;

}

DDialogControl::~DDialogControl()
{
}

void DDialogControl::CloseAllDialogs() 
{

#define DEL(X) if (X) delete X; X = NULL;											
// for modeless !!!!!!!!!!!!!
#define DESTROY(X) if (X) { X->DestroyWindow(); delete X; X = NULL; }
#define HIDE(X) if (X) X->ShowWindow(SW_HIDE);
	
	// member dialogs : delete // Hide
	DEL(m_dMaterial);
	DEL(m_dTexture);
	DEL(m_dParameter);
	DEL(m_dBackground);
	DEL(m_dFog);
	DEL(m_dDrawStyle);

	//modal HIDE(m_dObjectNew);
	//modal HIDE(m_dNewFontLogo);
	
	//HIDE(m_dTime);
	DESTROY(m_dTime);
	DESTROY(m_dMorph);
    DEL(m_dDeformSphere);

	//DEL(m_dOpenUrl);
	DESTROY(m_dViewpoint);
	DESTROY(m_dTreeEditor);
	
	DESTROY(m_dTransform);
	HIDE(m_dConsole);
	DESTROY(m_dLibrary);

#undef DEL
#undef HIDE
}

void DDialogControl::DestroyAllDialogs() 
{

#define DEL(X) if (X) delete X; X = NULL;
// for modeless !!!!!!!!!!!!!
#define DESTROY(X) if (X) { X->DestroyWindow(); delete X; X = NULL; }

	// member dialogs : delete
	DESTROY(m_dMaterial);
	
    DESTROY(m_dTexture);
	
    DESTROY(m_dParameter);
	DEL(m_dBackground);
	DESTROY(m_dFog);
	DEL(m_dDrawStyle);

	DEL(m_dObjectNew);
	DEL(m_dNewFontLogo);
	
	DESTROY(m_dTime);
	DESTROY(m_dMorph);
	
    DESTROY(m_dDeformSphere);
	
    DESTROY(m_dOpenUrl);
	DESTROY(m_dViewpoint);
	DESTROY(m_dTreeEditor);
	DESTROY(m_dTransform);
	DESTROY(m_dConsole);
	DESTROY(m_dLibrary);

#undef DEL
#undef DESTROY
}


// update modeless dialogs

void DDialogControl::UpdateAllDialogs() 
{
 if (m_dTime) {
   m_dTime->UpdateData(FALSE);
 }
 if (m_dViewpoint) {
   m_dViewpoint->UpdateData(FALSE);
 }
 if (m_dTreeEditor) {
   m_dTreeEditor->UpdateData(FALSE);
 }
 if (m_dLibrary) {
   m_dLibrary->UpdateData(FALSE);
 }

}

void DDialogControl::UpdateTimeDialog() 
{
 if (m_dTime) {
    m_dTime->UpdateData(FALSE);
 }
}


/////////////////////////////////////////////////////////////////////////////
// CMyglView construction/destruction


static int frameCnt = 0;
static int idleCnt = 0;
static int moveCnt = 0;
static int mouseMoveCnt = 0;

CMyglView::CMyglView()
{
	m_minimized = FALSE;

	expired = 0;


#ifdef _OGL_XX
    m_glptr = NULL;
#endif

	
	mainLoader = NULL;

	lastMessageType = 0;
	lastCursorMode = 0;
	hCurrentCursor = NULL;

	doPrintTime=1;
	LastFrameTime = 0.0;
	MinFrameTime; 1.0 / 25.0;
	ComplexTime = 5.0; // sec

	LastTime = GetTime();

	doAutoAnimate=0;
	AutoAnimateTimeOut = 3.0; // sec
	Animating = 0;
	isComplex = 0;
    m_updateColors = TRUE;
    m_updateColors = FALSE;
	m_colorsOk = TRUE;
   

	m_savedReporter = NULL;
	m_reporter = NULL;

	m_cameraSpeed = 1.0;
    MOVE_TIMER_ID=123;

	m_displayFrameRate = FALSE;
	m_frames = 0;
	m_startTime= timeGetTime(); 

	m_moveAbs=TRUE;
    m_sensorTest=TRUE;
	m_verbose = TRUE;
	m_fullSpeed = TRUE;

	lbutton = 0;
	TimerId = 0;
	lastWasTimer = 0;

	Mode = NULL_MODE;
	fly_state = 0;
	ResetMove();
	SetCameraMode(ORBIT_MODE);
	
	SetCameraRelativeMode(0);
}

CMyglView::~CMyglView()
{

#ifdef _OGL_XX
  delete m_glptr;
#endif

  if (m_reporter) {
	   GReporter::SetCurrent(m_savedReporter); // to do: for MDI wrong
	   delete m_reporter;
	   m_reporter= NULL;
	   m_savedReporter = NULL;
  }
}
/////////////////////////////////////////////////////////////////////////////
//
void CMyglView::Message(const char *message,int messageType)
{
  
   if (view->GetFullScreen()) return;  // ignore in fullscreen mode 

  //CFrameWnd *pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);
  CMainFrame *pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
  ASSERT(pFrame);

  lastMessage = message;
  lastMessageType = messageType;

  pFrame->SetMessageText(message);
  //  pFrame->UpdateWindow();
  	//pFrame->m_wndStatusBar.SendMessage(WM_IDLEUPDATECMDUI);
  pFrame->m_wndStatusBar.UpdateWindow();


}

// send a message to the console
void CMyglView::Print(const char *message)
{
	GReporter *reporter = GetReporter();
	if (reporter) reporter->Trace1(message);
}

/////////////////////////////////////////////////////////////////////////////
// get the reporter object 
GReporter *CMyglView::GetReporter()
{
	if (m_reporter == NULL)
		return GReporter::current; // return global reporter object (Reporter)
	return m_reporter;
}



/////////////////////////////////////////////////////////////////////////////
//
CMyglView *CMyglView::GetView()
{
  CFrameWnd *pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);
  CView *pView = pFrame->GetActiveView();
  if (!pView)
	return (NULL);
  if (!pView->IsKindOf(RUNTIME_CLASS(CMyglView)))
	return (NULL);
  return (CMyglView *)pView;
}



bool CMyglView::GetProfile( LPCTSTR lpszSection, LPCTSTR lpszEntry, int &val)
{
	int x=-1;
	x=AfxGetApp()->GetProfileInt(lpszSection, lpszEntry,x);
	if (x == -1) return false;
	val = x;
	return true;
}

bool CMyglView::SetProfile( LPCTSTR lpszSection, LPCTSTR lpszEntry, int val)
{
	AfxGetApp()->WriteProfileInt(lpszSection, lpszEntry,val);
	return true;
}

bool CMyglView::GetProfile( LPCTSTR lpszSection, LPCTSTR lpszEntry, CString &val ) 
{
	val=AfxGetApp()->GetProfileString(lpszSection, lpszEntry,val);
	return val.GetLength()>0;
}

bool CMyglView::SetProfile( LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR val)
{
	AfxGetApp()->WriteProfileString(lpszSection, lpszEntry,val);
	return true;
}


// registry helpers
// get value
BOOL CMyglView::GetProfile(LPCTSTR lpszKey, CString &value)
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

BOOL CMyglView::GetProfile(LPCTSTR lpszKey, int &value)
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

BOOL CMyglView::SetProfile(LPCTSTR lpszKey, LPCTSTR value)
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

BOOL CMyglView::SetProfile(LPCTSTR lpszKey, int value)
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





// Overidden to change ActiveView in CMyglApp
void CMyglView::OnActivateView( BOOL bActivate, CView* pActivateView, CView* pDeactiveView )
{
   CView::OnActivateView(bActivate,pActivateView,pDeactiveView);
   if (bActivate)
	   GetApp()->SetActiveView(this);
}

// set the cursor mode depending on action 
void CMyglView::SetCursorMode(int cursorMode)
{
	lastCursorMode=cursorMode;

	HCURSOR hCursor;

	// Load a cursor resource that was originally created using
	// the Graphics Editor and assigned the i.d. IDC_MYCURSOR.
	//hCursor = AfxGetApp()->LoadCursor(IDC_SIZEALL);
    //hCursor = AfxGetApp()->LoadOEMCursor(OCR_SIZEALL);
	switch (lastCursorMode) {	
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

   HCURSOR hOldCursor;

   hOldCursor = SetCursor(hCursor);

   hCurrentCursor = hCursor;

}


/////////////////////////////////////////////////////////////////////////////
static int firstDraw = 1;
static CString initialModel;


// CMyglView drawing
void CMyglView::OnDraw(CDC* pDC)
{
 // UNUSED pDC;
  // CMyglDoc* pDoc = GetDocument();
  RECT rc;

#ifdef _OGL
  HGLRC hglrc = view->device->wglGetCurrentContext();
  if (!hglrc) return;
#endif

  {

	if (firstDraw) {
		firstDraw = 0;
		if (initialModel.GetLength()>0) {
			ReadModel(initialModel);
			// will send a redraw
			return;
		}
	}

#ifdef _OGL_XXX
	if (!m_glptr->IsSurfaceInitialized())
	{
		view->Initialize(GetSafeHwnd());
		m_glptr->SetSurfaceInitialized();
	}
#endif

// xxxxx	GetClientRect(&rc);

	{
		float	fftime;

		isComplex = isComplex || view->updateIsComplex;	 

		if (isComplex) {
		   // xx BeginWaitCursor();
		}
		
		view->DrawScene(rc);

		frameCnt++;


		if (isComplex) {
		//xxx   EndWaitCursor();
		   //Message("");
		}

		// xxxx m_endTime= timeGetTime();	// time last rendering cycle stopped

		if (m_displayFrameRate) {
		m_frames ++;

		if ( (m_frames % 6 ) == 0) {
			CString msg;
			msg.Format("Updates per sec %.2f (%.2f) \n",  1000.0f /   ((float) (m_endTime-m_startTime) /  (float )m_frames), 1000.0f / (float) view->lastFrameTime);
			m_frames = 0;
			m_startTime= timeGetTime(); 
			Message(msg,PROGRESS_MESSAGE);	
		}
		}


		fftime = LastFrameTime = LastTime = (float)(view->lastFrameTime) / 1000.0f;

		isComplex = LastTime > ComplexTime; // may have loaded includes

		if (doPrintTime && !busy && !m_displayFrameRate) {
			CString stime;
		    if (view->time.enabled) 
			  sprintf (stime.GetBuffer(80), "T = %.2f,Time: %.2f sec, (FPS=%.4f)", view->time.t,fftime,1.0/fftime);
			else 
			  sprintf (stime.GetBuffer(80), "Time: %.2f sec, (FPS=%.3f)", fftime,1.0/fftime);

			Message(stime );
			//	TRACE("Time:%.2f", fftime);
		}

		if (view->lastFrameLoadCnt >0) {
			if (view->lastFrameLoadErrorCnt == 0) {
			   Reporter.Status("%s, %d inlines/textures loaded",view->file->GetUrl(),view->lastFrameLoadCnt);
			}
			// else assume a errormessage is status
		}

		if (view->firstUpdate) {
		   view->firstUpdate = 0;
		   
		   if (view->SceneHasTextures()) {
		      if (view->initialTextureMode >0) {
			     do {
				 if ((view->initialTextureMode == 1) && (view->GetTexturing() == 0))
				     if (AfxMessageBox("Load textures for scene now ?",MB_YESNO) != IDYES) break;
				 if (view->GetTexturing() == 0) {
				     view->SetTexturing(1);
					 //Reporter.Status("Loading textures for %s",(view->file) ?view->file->GetUrl():"scene");
					 Redraw();
				 }
				 } while(0);
			  }
		   }
		}
	}
  }
}

/////////////////////////////////////////////////////////////////////////////
// CMyglView diagnostics

#ifdef _DEBUG
void CMyglView::AssertValid() const
{
	CView::AssertValid();
}

void CMyglView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMyglDoc* CMyglView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyglDoc)));
	return (CMyglDoc*) m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyglView message handlers

//
afx_msg LRESULT CMyglView::OnDialogClosed(WPARAM wParam, LPARAM lParam)
{  int ret=-1;

	CDialog *d = (CDialog *)lParam;
	
	if (!d) return(TRUE);

	if ( ((CWnd *)d) == dialogs.m_dTransform) 
    { dialogs.m_dTransform->DestroyWindow(); delete dialogs.m_dTransform; dialogs.m_dTransform = NULL; }
	else
	if (d == dialogs.m_dViewpoint) 
    { dialogs.m_dViewpoint->DestroyWindow(); delete dialogs.m_dViewpoint; dialogs.m_dViewpoint = NULL; }
	else
	if (d == dialogs.m_dTreeEditor) 
    { dialogs.m_dTreeEditor->DestroyWindow(); delete dialogs.m_dTreeEditor; dialogs.m_dTreeEditor = NULL; }
	else if (d == dialogs.m_dLibrary) 
    { dialogs.m_dLibrary->DestroyWindow(); delete dialogs.m_dLibrary; dialogs.m_dLibrary = NULL; }
	else if (d == dialogs.m_dTime) 
    { dialogs.m_dTime->DestroyWindow(); delete dialogs.m_dTime; dialogs.m_dTime = NULL; }


	if (d == dialogs.m_dOpenUrl) {
	}


	return(TRUE);

}
   


// set a default pixel fromat, copied from pixform.cpp
int CMyglView::SetDefaultPixelFormat(int DoubleBuffer,int depthBits, int Rgb)
{
	int nNewIdx=0;

#ifdef _OGL_XXX
	DWORD dwFlags = (DWORD)0;

//	  CView *cview = CMyglView::GetView();

	  memset(&m_glptr->m_PixFmtDesc, 0, sizeof(PIXELFORMATDESCRIPTOR));
	  dwFlags = 0;
	  dwFlags |=  PFD_DRAW_TO_WINDOW;
	  dwFlags |=  PFD_SUPPORT_OPENGL;
	  dwFlags |= (DoubleBuffer)    ? PFD_DOUBLEBUFFER	   : 0;

	  m_glptr->m_PixFmtDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	  m_glptr->m_PixFmtDesc.nVersion = 1;
	  m_glptr->m_PixFmtDesc.dwFlags = dwFlags;
	  m_glptr->m_PixFmtDesc.iPixelType = (Rgb) ? PFD_TYPE_RGBA : PFD_TYPE_COLORINDEX;
	  m_glptr->m_PixFmtDesc.cDepthBits = depthBits;



#if 0
	  dwFlags |= (m_drawbitmap)  ? PFD_DRAW_TO_BITMAP		 : 0;
	  dwFlags |= (m_drawwindow)  ? PFD_DRAW_TO_WINDOW		 : 0;
	  dwFlags |= (m_supportgdi)  ? PFD_SUPPORT_GDI		 : 0;
	  dwFlags |= (m_supportopengl)	 ? PFD_SUPPORT_OPENGL		 : 0;
	  dwFlags |= (m_doublebuffer)	 ? PFD_DOUBLEBUFFER 	 : 0;
	  dwFlags |= (m_dbufferdontcare) ? PFD_DOUBLEBUFFER_DONTCARE : 0;
	  dwFlags |= (m_stereo) 	 ? PFD_STEREO			 : 0;
	  dwFlags |= (m_stereodontcare)  ? PFD_STEREO_DONTCARE		 : 0;

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
#endif

	  nNewIdx = m_glptr->ChoosePixelFormat(m_glptr->m_hdc, &m_glptr->m_PixFmtDesc);

	  if ((m_glptr->IsNativeIndex(m_glptr->m_hdc, nNewIdx)) || (m_glptr->IsDeviceIndex(m_glptr->m_hdc, nNewIdx))) {
		// HG Release old GLRC
   //		m_glptr->ReleaseGLRC(cview->m_hWnd); // m_glptr->m_hdc);

		if (m_glptr->GetGLRC(m_glptr->m_hdc)) {
			m_glptr->SetSurfaceNotInitialized();
		} else {
		  // HG
		  MessageBox("Sorry, Pixel Format can not be changed", NULL, MB_OK | MB_ICONEXCLAMATION);
		}
	  }
	  else
		MessageBox("Unsupported Pixel Format", NULL, MB_OK | MB_ICONEXCLAMATION);
#endif
	 return(nNewIdx);
}

void CMyglView::OnInitialUpdate()
{
	// called also after a new document has been created !!!!!!!!!!!!!!!!!!!!!! 15.09.96
	static reallyFirstTime = 1;

	//HKEY hKeyRoot=NULL;
	CString val;


	// initialize cursors 
	hCurrentCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	BOOL initOk = FALSE;


	if (reallyFirstTime) {
		
		//::RegOpenKey(HKEY_CURRENT_USER, USER_KEY, &hKeyRoot);


		// Report handler
		m_savedReporter = GReporter::current;
		m_reporter = new CMyglViewReporter(this);

		GReporter::SetCurrent(m_reporter);
		if (!dialogs.m_dConsole) {
			dialogs.m_dConsole = new DConsole(this);
			dialogs.m_dConsole->Create(DConsole::IDD);
		}

    int depthBits=16;
	int rsoftware=0;

#ifdef _OGL

#if 0
	// to do>
	HGLRC hglrc = m_glptr->wglGetCurrentContext();

	int v;
	if (GetProfile("OpenGL","zdepth",v) && v>0)
			depthBits=v;
	if (GetProfile("OpenGL","software",v) && v>0)
			rsoftware=v;


  	if (hglrc)
  	{
		if (!m_glptr->IsSurfaceInitialized())
		{
			view->Initialize(GetSafeHwnd());
			m_glptr->SetSurfaceInitialized();
		}
	}
#endif
	HWND hwnd = GetSafeHwnd();
	HDC hdc = ::GetDC(hwnd);

	if (view->Initialize(hwnd,hdc )<0) {
		TRACE("Error during initialization \n");

		return; //  FALSE;
	}
	initOk = gtrue;


#else
	// driver choosed

	CString driver;	// preferenced Driver
	BOOL  bOnlySystemMemory = TRUE; // FALSE; VIDEOMEM automatic for HW
 	BOOL  bOnlyEmulation = FALSE;

	CString mode;

	GDriverHints driverHints;

	driverHints.m_useHW	= TRUE;

	if (GetProfile("Direct3D","driver", val)) {
		driver = val;
	}

	if (GetProfile("Direct3D","onlySystemMemory", bOnlySystemMemory)) {
		driverHints.m_onlySystemMemory = bOnlySystemMemory;
	}

	if (GetProfile("Direct3D","bOnlyEmulation", bOnlyEmulation)) {
		driverHints.m_onlyEmulation = bOnlyEmulation;
		if (bOnlyEmulation)
			driverHints.m_useHW	= FALSE;
	}


	if (GetProfile("Direct3D","fullscreenMode", mode)) {
		driverHints.m_fullScreenMode = mode;
	}

	driverHints.m_driver = driver;

	int ret = view->Initialize(GetSafeHwnd(),NULL,&driverHints);

	if (ret<0) {
		Reporter.Error("Can´t initialize graphics, terminating.");
		// exit 
		exit(-1);
			// OnAppExit();			
	}
	initOk = gtrue;
	// full screen mode 
	if (mode.GetLength()>0) 
		view->device->SetDefaultFullscreenMode(mode);


#endif
/*
	// get preferences
	view->maxNumSounds =  AfxGetApp()-> GetProfileInt("Sound","numSounds",8);
	//int cpuBudget = AfxGetApp()-> GetProfileInt("Sound","RsxCpuBudget",0);
	view->useRSXSound =  AfxGetApp()-> GetProfileInt("Sound","useRsx",0)>0;
	view->soundQuality = AfxGetApp()-> GetProfileInt("Sound","quality",GS_QUALITY_MAX);
	view->soundDoppler =  AfxGetApp()-> GetProfileInt("Sound","doppler",0)>0;
*/

	// new 28.04.99
	theCache.writeCacheEnabled = TRUE;
	theCache.SetWriteCacheDirectory(GFile::cacheDirectory); 
	theCache.SetReadCacheDirectoryPath("cdrom:\\cache"); // set as default 
	theCache.lastDiskSpaceCheck = timeGetTime() +1000l*90l; //xx ; start check after 90 sec 

	// get other settings 
	//if (hKeyRoot != NULL) 
	{
		int v;

		if (1) { // caching flags 
			v=theCache.cacheMode;
			if (GetProfile("Cache","verifyMode",v)) {
				if (v >= GCACHE_ALWAYS && v <= CCACHE_NEVER)
					theCache.cacheMode = (GCacheMode) v;
			}
			v=theCache.readCacheEnabled;
			if (GetProfile("Cache","readCache",v)) {
				theCache.readCacheEnabled = v>0;
			}
			
			v=theCache.writeCacheEnabled;
		
			if (GetProfile("Cache","writeCache", v)) {
				theCache.writeCacheEnabled = v>0;
			}
			val="";
			val = GFile::cacheDirectory;
			if (GetProfile("cache","directory", val)) {
				if (val.GetLength()>0) {
					GFile::cacheDirectory = val;
					TRACE("Cache directory is : %s \n",(const char *)val);
					theCache.SetWriteCacheDirectory(val); 
				}

			} else { // store initial value 
				val = GFile::cacheDirectory;
				SetProfile("cache","directory", val );
			}			



            val="";
			if (GetProfile("Cache","readonlyDirectorys", val)) {
					theCache.SetReadCacheDirectoryPath(val);
			}
			v=theCache.expirationDays;
			if (GetProfile("Cache","verifyDays", v)) {
					if (v>=0) theCache.expirationDays = v;
			}
			v=0;
			if (GetProfile("Cache","minDiskSpaceFreeKB", v) && (v>=1024)) {
					theCache.minSpaceFree = v*1024;
			}
			v=0;
			if (GetProfile("Cache","maxDiskSpaceUsedKB", v) && (v>=1024)) {
					theCache.maxSpaceUsed = v*1024;
			}
		}

		if (initOk) {
#ifdef _D3D		
		if (GetProfile("Direct3D","dither",v))
			view->SetDithering(v>0);

		if (GetProfile("Direct3D","textureFiltering",v))
			view->SetTextureFiltering(v);

		if (GetProfile("Direct3D","textureMipMap",v))
			view->SetTextureMipMap(v);

		// done in GModel.cpp at init time
		//if (GetProfile("Direct3D.useSoftwareLighting"),v))
		//	view->renderState.enableOwnLighting = (v>0); 

		if (GetProfile("Direct3D","antiAlias",v))
			view->SetAntiAliasing(v>0); 
#endif
		if (GetProfile("General","exactCollision",v))
			view->useRapid=v>0;


#ifdef _OGL
		if (GetProfile("OpenGL","dither",v))
			view->SetDithering(v);

		if (GetProfile("OpenGL","textureFiltering",v))
			view->SetTextureFiltering(v);

		if (GetProfile("OpenGL","textureMipMap",v))
			view->SetTextureMipMap(v);
		view->renderState.nurbsMode = GGL_NURBS_GLU;
		if (GetProfile("OpenGL","nurbsMode",v) && v>=0)
			view->renderState.nurbsMode=(GGlNurbsMode)v;

#endif
		}
		if (GetProfile("Direct3D","askHardware",v))
			m_askHardware = v;

		//xx if (GetProfile("UI","mode",v))
		//xx	m_uiMode = (UIMode) v;
		
		// if m_navPanelOn has already been deactivated by the property
		// settings, the flag may not be overridden by the registry value
		if (GetProfile("General","panel", v)) {
			//view->m_navPanelOn_Reg = v;
			//view->m_navPanelOn = view->m_navPanelOn_Reg & view->m_navPanelOn_Prop & view->m_navPanelOn_NavInf;
			//xx view->setNavigationPanelProp(m_navPanelOn_Embed && v);
		}

		//if (GetProfile("General","speedFac", v))
			;//	m_cameraSpeedFac = v;
		//else m_cameraSpeedFac = 10;

		if (GetProfile("General","fullSpeed", v))
			m_fullSpeed = v;

		if (GetProfile("General","verbose", v)) {
			m_verbose = v;
			if (m_reporter) m_reporter->verbose = v;

			if (m_verbose) {
 				view->inlineWwwFlags |= WWW_REPORT_ERRORS;
 			}
 			else 
 				view->inlineWwwFlags &= ~WWW_REPORT_ERRORS;
		}

		if (GetProfile("Nurbs","targetFps", v) && (v>0))
			view->targetFrameRate = v;

		if (GetProfile("Nurbs","tessellationMode", v) && (v>=0))
			view->nurbsTessellationMode = v;

		if (GetProfile("Nurbs","tessellationScale", v) && (v>=0))
			view->nurbsTessellationScale = (float) v/1000.0f;

		if (GetProfile("Avatar","url", val)) {
			view->myAvatarURL.set(val);
		}

		if (GetProfile("Avatar","name", val)) {
				view->myAvatarName.set(val);
		}
	

		//if (GetProfile("Navigation","anchorSingleClick",v))
		//		m_anchorSingleClick = v;

		if (GetProfile("Sound","enableSound",v))
			view->useSound = !!v;

		if (GetProfile("Sound","numSounds",v))
			view->maxNumSounds = v;
		
		if (GetProfile("Sound","useRsx",v))
			view->useRSXSound =  v != 0;

		if (GetProfile("Sound","quality",v))
			view->soundQuality = v;

		if (GetProfile("Sound","doppler",v))
			view->soundDoppler = v != 0;

		//::RegCloseKey(hKeyRoot);
	}

	reallyFirstTime=0;
	}
}

int CMyglView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// initialize cursors 
	hCurrentCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);

	for(int i=0; i<= ANCHOR_CURSOR;i++) cursors[i] = hCurrentCursor;
	cursors[STANDARD_LOADING_CURSOR] = AfxGetApp()->LoadStandardCursor(IDC_APPSTARTING);
	//cursors[NAVIGATION_CURSOR] = AfxGetApp()->LoadStandardCursor(IDC_SIZE);
	
	cursors[MOVING_CURSOR] = AfxGetApp()->LoadStandardCursor(IDC_CROSS);
	cursors[SENSOR_CURSOR] = AfxGetApp()->LoadStandardCursor(IDC_UPARROW);
	cursors[ANCHOR_CURSOR] = AfxGetApp()->LoadStandardCursor(IDC_SIZEALL);
	cursors[COLLISION_CURSOR] = AfxGetApp()->LoadStandardCursor(IDC_NO);
	cursors[ZOOM_CURSOR] = AfxGetApp()->LoadStandardCursor(IDC_SIZEWE);

	//


	HWND hwnd = GetSafeHwnd();
	HDC hdc = ::GetDC(hwnd);

#ifdef _OGL_XXX
	m_glptr = new COpenGL(hdc);
#endif

	view = new GView(); // to do pass ptr to m_glptr
	view->ref();

	view->SetView(this);	// back pointer
	view->SetViewController(this);	// back pointer


	m_controller = new GEditController(view);

	CMyglApp* app = (CMyglApp*) AfxGetApp();
	
	// Read settings from ini file 
	ReadProfileSetting();

   int depthBits;


   if (AfxGetApp()->GetProfileInt("GLView","Zbuffer16",0)) {
      depthBits = 16;
   } else
      depthBits = 32;


	
#ifdef _OGL
	if (!app->m_ChoosePixelFormat)
		SetDefaultPixelFormat(1,depthBits);
#endif
	
	// notification window for completed internet requests
	GFile::hGlobalPostMsgWnd = hwnd;

	return 0;
}


/////////////////////////////////////////////////////////////////////////////
//
BOOL CMyglView::PreCreateWindow(CREATESTRUCT& cs)
{
  //the view windows style bits must include WS_CLIPSIBLINGS and
  //WS_CLIPCHILDREN so that the wgl functions will work. This is
  //
  // hg xxx xcs.style = cs.style | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
 // Register New Class for Private DC

 // cs.lpszClass = AfxRegisterWndClass(CS_OWNDC|CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW);

  return CView::PreCreateWindow(cs);
}

void CMyglView::OnClose() 
{
   	if (dialogs.m_dOpenUrl) {
		 dialogs.m_dOpenUrl->SaveHistory(); // to late 
	}
	
	CView::OnClose();
}


/////////////////////////////////////////////////////////////////////////////
// CMyglView WM_DESTROY

void CMyglView::OnDestroy()
{

	if (mainLoader) {
		mainLoader->Kill();
	    mainLoader->unref();
		mainLoader = NULL;
	}
	

	// Write settings to ini file 
	WriteProfileSetting();

	HWND hwnd = GetSafeHwnd();

	// notification window for completed internet requests
	if (GFile::hGlobalPostMsgWnd == hwnd) GFile::hGlobalPostMsgWnd = NULL;

	
	GFile::KillFilesInUse(hwnd);

	GFile::ResetSession();

	dialogs.DestroyAllDialogs();

	if (view) {
		view->Term();
		// no longer interested in messages back 
		// back pointer not valid
		view->SetView(NULL);	// back pointer
		view->SetViewController(NULL);	// back pointer

		ASSERT(view->refCnt == 1);
		view->unref();
		//delete view;
	}
	view = 0;

#ifdef _OGL_XX
	//
	//Release the current GL rendering context and device context
	//
	m_glptr->ReleaseGLRC(hwnd);
#endif


	CView::OnDestroy();

}
/////////////////////////////////////////////////////////////////////////////
// Palette 
void CMyglView::OnPaletteChanged(CWnd* pFocusWnd) 
{
	//CView::OnPaletteChanged(pFocusWnd);
	
    // See if the change was caused by us and ignore it if not.
    if (pFocusWnd != this) {
        OnQueryNewPalette();
    }	

}

BOOL CMyglView::OnQueryNewPalette() 
{
    // We are going active or the system palette has changed,
    // so realize our palette.
#ifdef _OGL
/*
	CPalette* pPal = CPalette::FromHandle(m_glptr->m_hPalette); //GetPalette() ;
    if (pPal) {   	
        CDC* pDC = GetDC();
        pDC->SelectPalette(pPal, FALSE);
        UINT u = pDC->RealizePalette();
		m_colorsOk = TRUE;

     	// colors are looking wrong anyway, if window lost focus
       	if (u != 0) {
            // Some colors changed, so we need to do a repaint.
            //InvalidateRect(NULL, TRUE); // Repaint the lot.
		    if (m_updateColors) pDC->UpdateColors(); // pixel on pixel basis
			m_colorsOk = FALSE;
			SetAnimateOff();  // because colormap wrong
        }
        ReleaseDC(pDC);

    }
*/

	view->device->SetPalette();

    return TRUE; // Windows ignores this.	
#endif
#ifdef _D3D
	view->device->SetPalette();
    return TRUE;
#endif

	
//	return CView::OnQueryNewPalette();
}

/////////////////////////////////////////////////////////////////////////////
//
void CMyglView::OnPixfmtChoose()
{
	if (!view) return;
#ifdef _OGL
	CPixForm PixForm;
	PixForm.SetID(ID_PIXFMT_CHOOSE);
	PixForm.SetGLptr(view->device);
	PixForm.DoModal();
#endif

//	SetProfile("GLView","Mode",PixForm.m_index); // not ok test
}

/////////////////////////////////////////////////////////////////////////////
//
void CMyglView::OnPixfmtDescribe()
{
	if (!view) return;

#ifdef _OGL
	if (view->device->wglGetCurrentContext())
	{
	  CPFDInfo pfdInfo;
	  pfdInfo.SetGLptr(view->device);
	  pfdInfo.DoModal();
	}
	else MessageBox( "No current rendering context", NULL, MB_ICONEXCLAMATION | MB_OK);
#endif
}

/////////////////////////////////////////////////////////////////////////////
//
void CMyglView::OnPixfmtEnum()
{
	if (!view) return;
#ifdef _OGL
	CPixForm PixForm;
	PixForm.SetID(ID_PIXFMT_ENUM);
	PixForm.SetGLptr(view->device);
	PixForm.DoModal();
#endif
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CMyglView::OnEraseBkgnd(CDC* pDC)
{
#ifdef _OGL
   // Erase if no gl-context initialized
  HGLRC hglrc = view->device->wglGetCurrentContext();

  if (hglrc && ! firstDraw) return (FALSE);
  else
  return CView::OnEraseBkgnd(pDC);
#endif

#ifdef _D3D
	if (view && view->device->bInitialized)
		return(FALSE);
	else 
	return CView::OnEraseBkgnd(pDC);
#endif
}


// Resize the view
void CMyglView::OnSize(UINT nType, int cx, int cy)
{	// not called on minimize
    if (nType == SIZE_MINIMIZED) {
		TRACE("Minimized \n");
	}
    else if (nType == SIZE_RESTORED) {
		TRACE("Restored  \n");
	}

	CView::OnSize(nType, cx, cy);
	view->OnSize(nType,cx,cy);
}

void CMyglView::OnMove(int x, int y) 
{  // not called if Mainframe Window move 
	CView::OnMove(x, y);
	view->OnMove(x,y);
}



// Mark view for redraw, because of attribute / seettings have changed
void CMyglView::Redraw()
{
   Invalidate(FALSE);
}


void CMyglView::OnLButtonDblClk(UINT nFlags, CPoint point)
{

	view->SetTheTime(); // set global VRML eventTime

	if (view->eventHandler) { // send event to current eventHandler
		Point screen;
		view->Pixel2Screen(point,screen);
		int ret = view->eventHandler->OnLButtonDblClk(nFlags,screen);
		if (ret == GEventHandler::EV_POP) view->PopEventHandler()->unref();
		if ((view->needUpdate >0) || view->getRedraw()) Redraw();
		return;
	}

	view->EndDegrade();

    if (view->Select(point.x,point.y,nFlags,GView::JUMP_WWWAnchor)) {
 	  // camera selections is already done	
	  if (view->executeNode) {
	  	if (RTISA(view->executeNode,GvWWWAnchor)) {
			GvWWWAnchor * jump = (GvWWWAnchor *) view->executeNode;
			
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

			jump = NULL; // string will be destroyed !
			view->executeNode = NULL;

			// to do : add reason
			ReadUrl(absoluteUrl);

		} // WWWAnchor
	  	else if (RTISA(view->executeNode,Gv2Anchor)) {
			Gv2Anchor * jump = (Gv2Anchor *) view->executeNode;
			
            TRACE("VRML 2 Anchor picked \n");
            if (jump->url.Length()>0) {
            CString url(jump->url[0]);	// url to execute

#if 0            
			// Add MAP options to URL
			if (jump->map == (int)GvWWWAnchor::POINT) {
				char buf[120];
				Point *p=&view->lastHit->p;
				sprintf(buf,"?%f,%f,%f",p->x,p->y,p->z);
				url += buf;
			}
			// Map objectname
#endif

			
			CString absoluteUrl;
			CreateAbsolutePath(jump->getHomeUrl(),url,absoluteUrl);

			jump = NULL; // string will be destroyed !
			view->executeNode = NULL;

			// to do : add reason
			ReadUrl(absoluteUrl);
            }
		} // Anchor

	  }
	}

//	CView::OnLButtonDblClk(nFlags, point);
}



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
            {DOLLY_XY_MODE,"dolly"},
			{ORBIT_MODE,"orbit"},
			{ORBIT_MODE,"examine"},
			{PAN_MODE,"pan"},
			{ZOOM_MODE,"zoom"},
			{ROLL_MODE,"roll"},
			
            {WALK_MODE,"walk"},
			{WALK_MODE,"plane"},
			{WALK_MODE,"plane"},
			
            // {FLY_MODE,"fly"},

			{WALK_2CLICK_MODE,"walk2"},
			{ZOOM_LENS_MODE,"lens"},

			// {JUMP_TO_MODE,"lookat"},
			// {JUMP_TO_MODE,"jump"},
/*
			{LOOKAT_OBJECT_MODE,"lookat"},
			{LOOKAT_OBJECT_MODE,"point"},

			{TRANSLATE_MODE,"translate"},
			{SCALE_MODE,"scale"},
			{ROTATE_MODE,"rotate"},

			
			{DRAG_DEFORMER_MODE,"deform"},
			{DRAG_VALUE_MODE,"drag value"},
*/
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


int CMyglView::SetCameraMode(int newMode)
{
  m_currentCameraMode = newMode;
  m_lButtonMode = newMode;
  return(m_currentCameraMode);
}

int CMyglView::GetCameraMode() 
{
   return(m_currentCameraMode);
}

int CMyglView::SetCameraMode(const char *mode)
{
    int newMode = MoveModeLookup(mode);
	if (newMode<0) { // try lower case 
		CString m(mode);
		m.MakeLower();
		newMode = MoveModeLookup(m);
	}
	

    if (newMode>=0) return SetCameraMode(newMode);
	else { 
		TRACE("Bad camera mode %s \n",mode); 
		return(0);
	}
}


const char * CMyglView::GetCameraModeString(int mode)
{
    if (mode<0) mode = m_currentCameraMode;
    return(MoveModeLookup(mode));
}

// get the current camera navigation mode by string
const char* CMyglView::GetCameraModeString()
{
    return(MoveModeLookup(m_currentCameraMode));
}



int CMyglView::SetMode(int newMode)
{
//  m_currentMode = newMode;
  m_lButtonMode = newMode;
  return(m_currentMode);
}

int CMyglView::GetMode() 
{
   return(m_lButtonMode);
}


void CMyglView::ResetMove()
{
	m_currentMode = NULL_MODE;
	m_lButtonMode = ORBIT_MODE;
	//m_mButtonMode = ZOOM_MODE;
	//m_rButtonMode = WALK_MODE;
	//m_lButtonDClickMode = SELECT_MODE;
	m_moveTimerId = 0;
	m_moveRepeat = 1;
	m_moveRepeatStartTime = 0.3; // 0.5;
	m_moveRepeatCycleTime = 0.02f; // 0.1; 0.05
	fly_state = 0;
}



void CMyglView::OnLButtonDown(UINT nFlags, CPoint point)
{

	view->SetTheTime(); // set global VRML eventTime

	frameCnt = 0;
	idleCnt = 0;
	moveCnt = 0;
	mouseMoveCnt = 0;

	if (m_moveTimerId) { // kill the auto move timer 
	   KillTimer(m_moveTimerId);
	   m_moveTimerId = 0;
	}
	m_firstPoint = point;
	m_firstFlags = nFlags;
	m_firstTime = GetTime();


	// if event handler ?
	if (view->eventHandler) { // send event to current eventHandler
		// degrade ??
		Point screen;
		view->Pixel2Screen(point,screen);
		int ret = view->eventHandler->OnLButtonDown(nFlags,screen);
		if (ret == GEventHandler::EV_POP) view->PopEventHandler()->unref();
		SetCapture();
		if ((view->needUpdate >0) || view->getRedraw()) Redraw();
		return;
	}

	// Seek to Object if pressed with ALT 
	if (nFlags & 0x20) {
        if (view->Select(point.x,point.y,nFlags,GView::JUMP_OBJECT)) {
			// camera changed
			Message("Jumping to object");
			Redraw();
		}

		return;
	}


	m_currentMode = m_lButtonMode;

	if (m_moveRepeat) { 
//		float cycle = max(LastFrameTime,m_moveRepeatCycleTime);
		float cycle = m_moveRepeatCycleTime;
//		m_moveTimerId = SetTimer(2, int (cycle * 1000), NULL);
		m_moveTimerId = SetTimer(2, int (cycle * 1000), NULL);
	}


	lastWasTimer = 0;
	lbutton=1;
	lbuttonP= point;
	buttonFlags = nFlags;


	Mode = m_currentMode; // mode is now the current (viewer) mode


	if (TimerId !=0 ) {
	   KillTimer(TimerId);
	   TimerId = 0;
	}

	// disable the auto animate feature 
	if (Animating) SetAnimateOff();

    if (!view->GetFullScreen()) {
	switch (Mode) {
	case NULL_MODE: Message("Hold and drag to activate sensors",NAVIGATION_MESSAGE); break;
	case ORBIT_MODE: Message("Hold and drag to orbit around the object, SHIFT Rolls, CONTROL zooms",NAVIGATION_MESSAGE); break;
	case WALK_MODE: Message("Hold and drag horizontal to look left/right, vertical to walk into or outof scene, SHIFT pans, CONTROL translates in xy",NAVIGATION_MESSAGE); break;
	case FLY_MODE: Message("Move the mouse horizontal to look left/right, vertical to walk into or outof scene, SHIFT pans, CONTROL translates in xy",NAVIGATION_MESSAGE); break;
	case ZOOM_MODE: Message("Hold and drag horizontal to change zoom scale",NAVIGATION_MESSAGE); break;
	case ROLL_MODE: Message("Hold and drag horizontal to change up direction",NAVIGATION_MESSAGE); break;
	case JUMP_TO_MODE: Message("Click objects to seek to, SHIFT follows",NAVIGATION_MESSAGE); break;
	default : 	
			Message("Drag with the mouse",NAVIGATION_MESSAGE);
	break;
	}
	}

	if (Mode == LOOKAT_OBJECT_MODE) {
		if (nFlags & MK_SHIFT) {
			if (view->Select(point.x,point.y,nFlags,GView::FOLLOW_OBJECT)) {
				Mode = NULL_MODE;
				m_currentMode = NULL_MODE;
			}

		} else {

			if (view->Select(point.x,point.y,nFlags,GView::JUMP_OBJECT)) {
				// camera changed
				Mode = NULL_MODE;
				m_currentMode = NULL_MODE;
				//SetMode(GetCameraMode());
			}
		}
		return;	
	}

	// capture the mouse event outside the window 
	SetCapture();


	if (Mode == NULL_MODE) {
        if (view->Select(point.x,point.y,nFlags,GView::DRAG_SENSORS)) {
			if ((view->needUpdate >0) || view->getRedraw()) Redraw();
		}
		return;
   }


	// check for sensor hit, even if a camera walk mode 
   if (m_sensorTest && (Mode>= DOLLY_XY_MODE && Mode <= ZOOM_LENS_MODE)) {
        if (view->Select(point.x,point.y,nFlags,GView::DRAG_SENSORS)) {

			if (view->eventHandler != NULL) { // sensor handler pushed 
				Mode = NULL_MODE;
				if ((view->needUpdate >0) || view->getRedraw()) Redraw();
				return;
			}
		}
   }
 
   //if (Mode == ZOOM_MODE) SetCursor(cursors[ZOOM_CURSOR]);
   //else SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZE)); // CROSS

//	SetCursor(AfxGetApp()->LoadCursor(IDC_PENCIL));


	if (Mode == FLY_MODE) { 
		fly_state ++;
		if (fly_state == 1) { 
			SetCursorMode(MOVING_CURSOR);
			// SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
			view->BeginDegrade();

		} else  {
			SetCursorMode(NAVIGATION_CURSOR);
			fly_state = 0;
			Mode = NULL_MODE;
			// cleanup on lbutton Up
		}
	}
	else { 
		view->BeginDegrade();
	    if (Mode == ZOOM_MODE) SetCursorMode(ZOOM_CURSOR);
		else SetCursorMode(MOVING_CURSOR);
	}

}

void CMyglView::OnLButtonUp(UINT nFlags, CPoint point)
{
	view->SetTheTime();

	if (lastMessageType == NAVIGATION_MESSAGE) 	Message("");

	if (view->eventHandler) { // send event to current eventHandler
		Point screen;
		view->Pixel2Screen(point,screen);
		int ret = view->eventHandler->OnLButtonUp(nFlags,screen);
		if (ret == GEventHandler::EV_POP) view->PopEventHandler()->unref();

		SetCursorMode(STANDARD_CURSOR);

		ReleaseCapture();
		if ((view->needUpdate >0) || view->getRedraw()) Redraw();

		return;
	}

	if (Mode == FLY_MODE && fly_state == 1) {  // terminate on next click 
		return;
	}
	
	if (m_moveTimerId) {
	   KillTimer(m_moveTimerId);
	   m_moveTimerId = 0;
	}



	view->EndDegrade();

	SetCursorMode(STANDARD_CURSOR);
	
	lbutton=0;
	Mode = NULL_MODE;

	// update camera dialog
	if (dialogs.m_dViewpoint) {
   		dialogs.m_dViewpoint->UpdateData(FALSE);
 	}
	m_lastFlags = nFlags;
	m_lastPoint = point;
	m_lastTime = GetTime();

#ifdef _DEBUG

	float t = m_lastTime - m_firstTime;

	CString msg;
	msg.Format(" Frames #%d time = %f fps = %f Idle %d Move %d Mouse move = %d",frameCnt,t,  1.0f / (t / (float) frameCnt),
		idleCnt,moveCnt,mouseMoveCnt);
	Message(msg);
#endif

	ReleaseCapture();

}

#define OFS2FAC(ofs) (((ofs)<0.0) ? 1.0 / (-(ofs)+1.0) : (ofs)+1.0)
#define SQ1(F) (((F) < 0.0) ? - ((F)*(F)) : (F)*(F))

#define SQ(F) ((F))


CPoint operator+(CPoint a,CPoint b) { return CPoint(a.x+b.x,a.y+b.y);}
CPoint operator-(CPoint a,CPoint b) { return CPoint(a.x-b.x,a.y-b.y);}

void CMyglView::OnMouseMove(UINT nFlags, CPoint point)
{
	int ret;

	view->SetTheTime(); // set global VRML eventTime

	mouseMoveCnt++;

	if (view->eventHandler) { // send event to current eventHandler
		Point screen;
		view->Pixel2Screen(point,screen);
		// select to find if sensor still of geometry
		ret=view->Select(point.x,point.y,nFlags,GView::INFO);
		ret = view->eventHandler->OnMouseMove(nFlags,screen);
		if (ret == GEventHandler::EV_POP) view->PopEventHandler()->unref();
		if ((view->needUpdate >0) || view->getRedraw()) Redraw();
		return;
	}

#if 0
	if (lastWasTimer) {
	  	lbuttonP = m_lastPoint; // reset lButtonP because incremented by timer move
	  	lbuttonP=point;
	  	lastWasTimer = 0;
	  // m_firstPoint =  m_lastPoint; // test
		// Reset move distance
	  	m_firstPoint = point;
		m_firstFlags = nFlags;
		m_firstTime = GetTime();

		m_lastFlags = nFlags;
		m_lastPoint = point;
		m_lastTime = GetTime();
		return; // no update now 

	}
#endif
	m_lastFlags = nFlags;
	m_lastPoint = point;
	m_lastTime = GetTime();
	
	if (Mode == NULL_MODE) {	// Default mouse move action,  display WWW anchors 
	   CPoint lastDelta = point - lbuttonP;

	   if ((abs(lastDelta.x) + abs(lastDelta.y)) >=2) {
	      ret= view->Select(point.x,point.y,nFlags,GView::INFO_WWWAnchor);
		  lbuttonP = point;
		  if (ret & SELECT_ANCHOR_BIT)
			  SetCursorMode(ANCHOR_CURSOR);
		  else if (ret & SELECT_SENSOR_BIT)
			  SetCursorMode(SENSOR_CURSOR);
		  else
			SetCursorMode(NAVIGATION_CURSOR);
	   }

	} else {
	   //XX DoMove(nFlags,point);
	}
}

/*
   Repeat last move on time event 
   
*/

#define INTEPS 3


// execute move action, on time event 
BOOL CMyglView::DoMoveTimer()
{
	Point delta;


	delta.x = (m_lastPoint.x-m_firstPoint.x);
	delta.y = -(m_lastPoint.y-m_firstPoint.y);
	delta.z = 0;
	if (delta.x >= INTEPS) delta.x -= INTEPS;
	else if (delta.x <= -INTEPS) delta.x += INTEPS;
	else delta.x = 0;

	if (delta.y >= INTEPS) delta.y -= INTEPS;
	else if (delta.y <= -INTEPS) delta.y += INTEPS;
	else delta.y = 0;
	
	if ((abs((int)delta.x) + (abs((int)delta.y))) <= 0) return FALSE;



	delta.x *= 0.1;
	delta.y *= 0.1;

/*

	if (fabs(delta.x) > fabs(delta.y)) {
			if (fabs(delta.x) < INTEPS) delta.x = 0;
	} else {
			if (fabs(delta.y) < INTEPS) delta.y = 0;
	}

    
	//xxx if ((abs((int)delta.x) + (abs((int)delta.y))) <=7) return;

	delta.x *= 0.1;
	delta.y *= 0.1;
*/

	BOOL ret = DoDeltaMove(m_lastFlags, delta);
		
	lastWasTimer = 1;



#if 0
   if ((GetTime() - m_lastTime ) > m_moveRepeatStartTime) {
//		DoMove(m_lastFlags,lbuttonP+lastDelta);
		CPoint delta;
		delta = (m_lastPoint-m_firstPoint);
		//delta.x *= 0.25;
		//delta.y *= 0.25;
		delta.x *=0.5;
		delta.y *=0.5;

		DoMove(m_lastFlags,lbuttonP + delta);
		
		lastWasTimer = 1;
   }
#endif 
   return ret;
}


// do the mouse move, based on current mouse position in point 
BOOL CMyglView::DoMove(UINT nFlags, CPoint point)
{
	Point delta;	
	if (Mode > NULL_MODE) {

	   return DoMoveTimer();
	
	   // delta check with origin 
	   delta.x = (point.x-m_firstPoint.x);
	   delta.y = -(point.y-m_firstPoint.y);
	   delta.z = 0;


	   if ((abs((int)delta.x) + (abs((int)delta.y))) <=7) return FALSE;

	   delta.x =  (point.x - lbuttonP.x);
	   delta.y = -(point.y - lbuttonP.y);
	   delta.z = 0;
       
//	   if ((abs((int)delta.x) <=2) && (abs((int)delta.y) <=2)) return;


	   DoDeltaMove(nFlags,delta);


	   lbuttonP = point; // incremental movement because camera functions are incremental
	}
	return FALSE;
}

// do an incremental delta change of parameters
BOOL CMyglView::DoDeltaMove(UINT nFlags, Point &delta)
{
	if (Mode > NULL_MODE) {
	   BOOL groundTest=FALSE;

	   moveCnt++;

	   float ftime = (float) view->lastFrameTime / 1000.0f;
	   //float wscale = PI*0.5*0.5*0.5;

	   float wscale = PI*ftime;

	   float step = view->viewStepSpeed * ftime;

	   //float stepx= view->GetCamera().width;
	   
	   float stepx= step;
	   float stepy= step;


	   float x, y,z,dxpix,dypix;

//	   lastDelta = point - lbuttonP;

	   dxpix = x =  delta.x;
	   dypix = y =	delta.y;

#if 0
	   float INTEPS = 4.0-0.0;	// was 2.0 min move epsilon
	   if (fabs(y) > fabs(x)) {
			if (fabs(x) < INTEPS) x = 0;
	   } else {
			if (fabs(y) < INTEPS) y = 0;
	   }
#endif


/*
   too fast 
	   x = SQ1(x); // 16.11.96
	   y = SQ1(y);

*/
	   x *= 2.0;
	   y *= 2.0;



	   x *= 0.005;
	   y *= 0.005;

	// 4.11.95
	   //float scale = LastFrameTime;
	   //if (scale <0.05) scale = 0.05;
	   //x *= 0.005*scale;
	   //y *= 0.005*scale;

	   Point oldPosition = view->GetCamera().position;

	   switch (Mode) {
	   case DOLLY_XY_MODE :
			x =  delta.x / 1.0f; 
			y =  delta.y / 1.0f; 

		  if (nFlags & MK_SHIFT) {
		    view->GetCamera().DollyMax(m_cameraSpeed *  - SQ(x) *stepx,0.0, m_cameraSpeed * SQ(y) * step,!GetCameraRelativeMode());
		  } else {
		    view->GetCamera().DollyMax(m_cameraSpeed *  - SQ(x) * stepx, m_cameraSpeed * - SQ(y) * stepy,0.0,!GetCameraRelativeMode());
		  }
		  break;
	   
      case ORBIT_MODE :
		  if (nFlags & MK_CONTROL) {		// ZOOM
		  		float f=x*0.1;
		   		x*=-1.0;
		   		if (x<-0.5) x = -0.5;else if (x>0.5) x = 0.5;
		   		view->GetCamera().Zoom(OFS2FAC(x));
/*		  } else  if (nFlags & MK_SHIFT) {	 // ROLL 
			   view->GetCamera().RollMax(wscale*SQ(x));
*/
		  } else {
#if 0
           // try relative 
           GCamera &c = view->GetCamera();

           Point oldTarget(c.target);
           c += (- c.target + c.worldCenter);
           c.OrbitMax(-wscale*SQ(x),wscale*SQ(y),0 && !GetCameraRelativeMode());
           c += ( oldTarget - c.worldCenter);
#endif

			if (nFlags & MK_SHIFT) 
				view->GetCamera().OrbitCenterMax(-wscale*SQ(x),wscale*SQ(y),0 && !GetCameraRelativeMode());
		    else view->GetCamera().OrbitMax(-wscale*SQ(x),wscale*SQ(y),0 && !GetCameraRelativeMode());
		  }
		  break;

	   case PAN_MODE :
		  if (nFlags & MK_SHIFT) {
			  // fall through ZOOM 
		  }
		  else { 
			view->GetCamera().PanMax(0.5*wscale*-SQ(x),0.5*wscale*SQ(y),!GetCameraRelativeMode());
			break;
		  }	

	   case ZOOM_MODE :
		   { float f=x*0.1;
		   x*=-1.0;
		   if (x<-0.5) x = -0.5;else if (x>0.5) x = 0.5;
		   view->GetCamera().Zoom(OFS2FAC(x));

		   if (0) { 
				y*=-1.0;
				if (y<-0.5) y = -0.5;else if (y>0.5) y = 0.5;
				view->GetCamera().ZoomDistance(OFS2FAC(y));
		   }

		   }
		  break;
	   case ZOOM_LENS_MODE :
		   { float f=x*0.1;
		   x*=-1.0;
		   if (x<-0.5) x = -0.5;else if (x>0.5) x = 0.5;
		   view->GetCamera().ZoomDistance(OFS2FAC(x));
		   }
		  break;
	   case ROLL_MODE :
		   view->GetCamera().RollMax(wscale*SQ(x));
		   break;

	   case WALK_MODE :

	   case FLY_MODE :
		  if (nFlags & MK_CONTROL) { // move left-right // up down 
			x*= 0.25;
			y*= 0.25;

            x = limit(x,-0.5f,0.5f);
            y = limit(y,-0.5f,0.5f);

			x =  delta.x / 1.0f; 
			y =  delta.y / 1.0f; 

		    view->GetCamera().Dolly(m_cameraSpeed *  - SQ(x) * stepx, m_cameraSpeed * SQ(y) * stepy,
                                0.0,  !GetCameraRelativeMode());
		  } else 
		  if (nFlags & MK_SHIFT) {
		  		view->GetCamera().PanMax(0.5f * -wscale*SQ(x),0.5*wscale*SQ(y),!GetCameraRelativeMode()); // new 0.5 new 9.11.
		  } else {
			// x:pan x + y :dolly z
			view->GetCamera().PanMax(0.5f * -wscale*SQ(x),0.0,!GetCameraRelativeMode()); 
			// was 0.25 without SQ
			// y = limit(y,-0.5f,0.5f);

			//view->GetCamera().Dolly(0.0,0.0, m_cameraSpeed * SQ(y) * 0.5 * view->GetCamera().TargetDistance(),!GetCameraRelativeMode());
		    y =  delta.y / 1.0f; 
			view->GetCamera().Dolly(0.0,0.0, m_cameraSpeed * SQ(y) * step ,!GetCameraRelativeMode());

		    if (Mode == WALK_MODE) groundTest=TRUE;
		  }

		  break;

	   case DRAG_DEFORMER_MODE:

		  if (view->deformer)  {
			Point ppix,pworld;
			ppix.Set(dxpix,dypix,0.0);

			view->GetCamera().Pixel2WorldDelta(ppix,pworld);
			if (nFlags & MK_SHIFT)	pworld.Set(pworld.x,pworld.z,pworld.y);
			view->deformer->Translate(pworld);
			Redraw();
		  }

		  break;


//	   default :
//		  break;

	   }

	
	   if (view->GetCamera().Changed()) {

			// check for collision detection 

			if (view->collisionDetection) {
				Point newPosition = view->GetCamera().position;
				Point p,n;

				if (view->CollisionDetect(oldPosition,newPosition,p,n)) {
					view->GetCamera() += (p-newPosition);
					Message("Collision !",COLLISION_MESSAGE);
					SetCursorMode(COLLISION_CURSOR);
				}
				else {
					if (lastMessageType == COLLISION_MESSAGE) 
						Message("");
					if (lastCursorMode == COLLISION_CURSOR)
						SetCursorMode(NAVIGATION_CURSOR);
				}
			}
			if (groundTest && view->stayOnGround) {
				Point newPosition,hitPosition,hitNormal;
				if (view->GroundDetect(view->GetCamera().position,
					newPosition,hitPosition,hitNormal)  >0) {
					view->GetCamera() += (newPosition-view->GetCamera().position);
				}

			}
			// OnDraw(NULL); // Redraw();
			Redraw();
			return TRUE;

	   }
	   else return FALSE;
	}
	else return FALSE;
}


void CMyglView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	float wstep = PI / 18.0; // step for orbit 
	float dstep = 0.1;	// step for dolly 

	int shift = GetAsyncKeyState(VK_SHIFT);  // also VK_LSHIFT RSHIFT
	int control = GetAsyncKeyState(VK_CONTROL);
	int alt = GetAsyncKeyState(VK_MENU);

	float dist = view->GetCamera().width;

	int walk= 0;
	int orbit =0;
	int pan = 0;

	if (control) {
		  pan = 1;
	} else
   
	if (GetCameraMode() == WALK_MODE) {
      if (shift) orbit = 1;
      else walk = 1;
	} 
	else {
      if (!shift) orbit = 1;
      else walk = 1;
	} 


   
	switch (nChar) {
	case VK_ESCAPE : 
			   if (view->GetFullScreen()) {
					view->SetFullScreen(0);
			   }
			   //if (view->currentTraversal) view->currentTraversal->stop ++;
			   else if (view->AnimatingCamera()) {
				   view->AnimateCameraStop();
			   }
	           else if (view->time.enabled) OnRenderAnimate(); // switch of animation
               else if (view->GetTexturing()) {
                     OnRenderTextured();
               }
			   else {
					WWWAbortLoad();
					return;
			   }
			break;
	case VK_SPACE: 		// SPACEBAR 
				OnCameraViewpoints();

			break;
	case VK_NEXT: 		// 	22	PAGE DOWN key 
				OnCameraViewpointNext();
				break;
	case VK_PRIOR: 		// PAGE UP key 
				OnCameraViewpointPrev();
				break;

    case (UINT)'+' :
			if (orbit) 
		    	view->GetCamera().Zoom(1.0/1.14);
			else 
//		    view->GetCamera().Dolly(0.0,0.0, -dstep*nRepCnt * view->GetCamera().TargetDistance());
      		view->GetCamera().Dolly(0.0,dstep*nRepCnt * dist,0.0); 

			break;
    case (UINT)'-' :

			if (orbit) 
		    	view->GetCamera().Zoom(1.14);
			else 
				//view->GetCamera().Dolly(0.0,0.0, dstep*nRepCnt * view->GetCamera().TargetDistance());
      			view->GetCamera().Dolly(0.0,-dstep*nRepCnt * dist,0.0); 
			break;
	case VK_END: 		// 	23	END key 
	case VK_HOME: 		// 	24	HOME key 
                OnCameraJumpto();
			break;
	case VK_LEFT: 		// 	25	LEFT ARROW key 
			if (walk) 
		        view->GetCamera().Dolly(-dstep*nRepCnt * dist,0.0,0.0); 
			 else if (pan)
			    view->GetCamera().Pan(-wstep*nRepCnt,0.0); 
			 else 
			   view->GetCamera().Orbit(wstep*nRepCnt,0.0); 
			break;
	case VK_UP: 		// 	26	UP ARROW key 
			if (walk) 
              view->GetCamera().Dolly(0.0,0.0, dstep*nRepCnt * view->GetCamera().TargetDistance());
			else if (pan)
			    view->GetCamera().Pan(0.0,wstep*nRepCnt); 
			else 
			    view->GetCamera().Orbit(0.0,wstep*nRepCnt); 
			break;
	case VK_RIGHT: 		// 	27	RIGHT ARROW key 
			 if (walk) 
		        view->GetCamera().Dolly(dstep*nRepCnt * dist,0.0,0.0); 
			 else if (pan)
			    view->GetCamera().Pan(wstep*nRepCnt,0.0); 
			 else 
			    view->GetCamera().Orbit(-wstep*nRepCnt,0.0); 
			    break;
	case VK_DOWN: 		// 	28	DOWN ARROW key 
			 if (walk) 
		        //view->GetCamera().Dolly(0.0,-dstep*nRepCnt * dist,0.0); 
              view->GetCamera().Dolly(0.0,0.0, -dstep*nRepCnt * view->GetCamera().TargetDistance());
			 else if (pan)
			    view->GetCamera().Pan(0.0,-wstep*nRepCnt); 
			 else 
			    view->GetCamera().Orbit(0.0,-wstep*nRepCnt); 
			 break;

    case (UINT)'A' :
                OnRenderAnimate();
                break;

    case (UINT)'B' :
				view->BindBackground(NULL);
				Redraw();
                break;

    case (UINT)'C' :
                OnCameraCollision();
                break;

    case (UINT)'=' :
    case (UINT)'D' :
    case VK_F7 :
				m_displayFrameRate = !m_displayFrameRate;
				Message(CString("Frame Rate display  is now ") + ((m_displayFrameRate) ? "On" : "Off") );
				break;

    case VK_F5 :
    case (UINT)'f' :
    case (UINT)'F' :
                OnOptionsFullscreen();
                break;

    case (UINT)'K' :


#ifdef _KATMAI			
				KatmaiEnable(!useKatmai); 
				Message(CString("Katmai code is now ") + ((useKatmai) ? "On" : "Off") );
				break;
#endif



    case (UINT)'u' :
    case (UINT)'U' :
                view->viewCulling = !view->viewCulling;
				Message(CString("View frustrum culling  is now ") + ((view->viewCulling) ? "On" : "Off") );
                break;

    case (UINT)'s' : // secondary driver 
    case (UINT)'S' :
#ifdef _D3D
				if (view->device->NumDevices>1) {
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

    case (UINT)'T' :
				OnRenderTextured();
				break;
		

    case (UINT)'G' :
				OnCameraGravity();
				break;

    case (UINT)'Z' : {

#ifdef _GSTATS
				// status 
				Reporter.Trace("Total Primitives %d Vertices %d Faces %d",GShell::primsProcessed,GShell::verticesProcessed,GShell::facesProcessed);
				Reporter.Trace("Triangles passed to D3D %d  single primitive max #vertices %d max # %d triangles drawn %d" , GShell::trianglesDrawn,GShell::maxPrimVertexCnt,GShell::maxPrimTriCnt);
				float f=(float)(view->lastFrameTime) / 1000.0f;
				if (f>0.0f)
				Reporter.Trace("Last frame time : %f = FPS %f trispersecond %f \n",f,1.0/f,1.0/ (f/(double) GShell::trianglesDrawn));
#endif

				}					
		break;


    case (UINT)'j' :
    case (UINT)'J' :
                OnCameraJumpto();
                break;

    case (UINT)'m' :
    case (UINT)'M' :
                OnCameraDollyXy();
                break;

    case (UINT)'P' :
                OnCameraPan();
                break;

    case (UINT)'w' :
    case (UINT)'W' :
                OnCameraWalk();
                break;

    case (UINT)'o' :
    case (UINT)'O' :
                OnCameraOrbit();
                break;
    case (UINT)'x' :
    case (UINT)'X' :
				view->SetBspMode(!view->GetBspMode());
				Redraw();
				Message("BSP Mode toggled ");

                break;
	case (UINT)'y' :
    case (UINT)'Y' :
				view->SetTextureFiltering(!view->GetTextureFiltering());
				Redraw();
				Message("Texture filtering toggled ");
                break;

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


	default :
		break;
	}
	if (view->GetCamera().changed) {
		Redraw();
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CMyglView::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (TRUE) {		  // CreatePopup
		// make sure window is active
		GetParentFrame()->ActivateFrame();

		m_rlastPoint = point;
		m_rlastFlags = nFlags;

		ClientToScreen(&point);
		CMenu menu;
		if (menu.LoadMenu(IDR_MAINFRAME_POPUP))
		{
			CMenu* pPopup = menu.GetSubMenu(0);
			ASSERT(pPopup != NULL);

			// Append viewpoints to menu 
			{	
				GvSceneInfo *sceneInfo = view->GetSceneInfo();
				CMenu* v = pPopup->GetSubMenu(2); // this must be the viewpoint menu
				if (v && sceneInfo) {
					int n = sceneInfo->cameras.Length();
					if (n>20) n = 20;
					
					v->DeleteMenu( 0, MF_BYPOSITION ); // delete the dummy entry 
  
					for (int i=0; i<n; i++)  {
						GvNode *n = sceneInfo->cameras[i];
						CString s = n->getName().getString();

						if (RTISA(n,GvViewpoint)) {	 // 4.07.97
							s = ((GvViewpoint *) n)->description.get();
							// ignore internal viewpoints in display
							if (s.GetLength() == 0) continue;
                        } 
						
						if (s.GetLength() >50) s = s.Left(50); // truncate 
						v->AppendMenu(MF_STRING, ID_VIEWPOINT_0+i,s);
					}
				}

			} // viewpoints



			CMenu menu1;
			if (menu1.LoadMenu(IDR_MAINFRAME)) {
			
			CMenu* subMenu = menu1.GetSubMenu(3);
			pPopup->AppendMenu(MF_POPUP,(UINT) subMenu->GetSafeHmenu(), "Camera");
			subMenu = menu1.GetSubMenu(4);
			if (subMenu) pPopup->AppendMenu(MF_POPUP,(UINT) subMenu->GetSafeHmenu(), "Render");
			subMenu = menu1.GetSubMenu(6);
			if (subMenu) pPopup->AppendMenu(MF_POPUP,(UINT) subMenu->GetSafeHmenu(), "Tools");
			}

#ifdef _D3D
			pPopup->AppendMenu(MF_STRING, ID_OPTIONS_FULLSCREEN,"Fullscreen");

			// Append drivers to menu 
			{	
				GD3Device *d = view->device;
				if (d) {
				for (int i=0; i<d->NumDrivers; i++) 
					pPopup->AppendMenu(MF_STRING, ID_DRIVER_0+i,d->Driver[i].Name);
				}

			}
#endif  


			pPopup->TrackPopupMenu(TPM_RIGHTBUTTON | TPM_LEFTALIGN,
								   point.x, point.y,
								   AfxGetMainWnd()); // route commands through main window
			return;

		}
	}
	
	if (Animating) SetAnimateOff();
	view->Select(point.x,point.y,nFlags,GView::INFO);
	CView::OnRButtonDown(nFlags, point);
}


void CMyglView::OnRButtonUp(UINT nFlags, CPoint point)
{
	CView::OnRButtonUp(nFlags, point);
}


void CMyglView::OnCameraSeekToLast() 
{
   if (view->Select(m_rlastPoint.x,m_rlastPoint.y,m_rlastFlags,GView::JUMP_OBJECT)) {
		Message("Jumping to object");
		Redraw();
   }
}

void CMyglView::OnUpdateCameraSeekToLast(CCmdUI* pCmdUI) 
{
	
}

void CMyglView::OnCameraFollowObject() 
{
   if (view->Select(m_rlastPoint.x,m_rlastPoint.y,m_rlastFlags,GView::FOLLOW_OBJECT)) {
		Redraw();
   }
}


////////////////////////////////////////////////////////////////////////////
void CMyglView::OnCameraReset()
{
	if (view->AnimatingCamera()) {
		view->AnimateCameraStop();
    }

	if (Animating) SetAnimateOff();

	view->CameraReset();
	UpdateViewpointDialog();
	Redraw();
}

void CMyglView::OnCameraDollyXy()
{
	SetCameraMode(DOLLY_XY_MODE);
}

void CMyglView::OnCameraNone() 
{
	SetCameraMode(NULL_MODE);
}



void CMyglView::OnCameraOrbit()
{
	SetCameraMode(ORBIT_MODE);
}

void CMyglView::OnCameraPan()
{
	SetCameraMode(PAN_MODE);
}


void CMyglView::OnCameraWalk()
{
	SetCameraMode(WALK_MODE);
}

void CMyglView::OnCameraZoom()
{
	SetCameraMode(ZOOM_MODE);
}

void CMyglView::OnCameraRoll()
{
	SetCameraMode(ROLL_MODE);

}
void CMyglView::OnCameraFly() 
{
	SetCameraMode(FLY_MODE);
}

void CMyglView::OnCameraRelative() 
{
	SetCameraRelativeMode(GetCameraRelativeMode() == 0 ? 1 : 0);
}


void CMyglView::OnUpdateCameraRelative(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetCameraRelativeMode());
}


// menu button check handlers for camera menu

void CMyglView::OnUpdateCameraDollyXy(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(GetCameraMode() == DOLLY_XY_MODE);
}

void CMyglView::OnUpdateCameraNone(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetCameraMode() == NULL_MODE);
}

void CMyglView::OnUpdateCameraOrbit(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(GetCameraMode() == ORBIT_MODE);
}

void CMyglView::OnUpdateCameraPan(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(GetCameraMode() == PAN_MODE);
}

void CMyglView::OnUpdateCameraZoom(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(GetCameraMode() == ZOOM_MODE);
}

void CMyglView::OnUpdateCameraRoll(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(GetCameraMode() == ROLL_MODE);
}

void CMyglView::OnUpdateCameraWalk(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(GetCameraMode() == WALK_MODE);
}

void CMyglView::OnUpdateCameraFly(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetCameraMode() == FLY_MODE);
}

void CMyglView::OnUpdateCameraSpinauto(CCmdUI* pCmdUI)
{
//	pCmdUI->SetCheck(TimerId !=0);
	pCmdUI->SetCheck(Animating);
}

void CMyglView::OnCameraAutospin()
{
	doAutoAnimate = ! doAutoAnimate;
	if (!doAutoAnimate && Animating) SetAnimating(0);

}

void CMyglView::OnUpdateCameraAutospin(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(doAutoAnimate);
}

// **************************************************************
// Render Menu
//
void SetRenderChecks()
{
 // not needed, done by mfc on_update_command_ui
}

void CMyglView::OnRenderWireframe()
{
	 view->SetRenderMode(GRenderWireframe);
	 Redraw();
	 SetRenderChecks();
}

void CMyglView::OnRenderVertices()
{
	 view->SetRenderMode(GRenderVertices);
	 Redraw();
	 SetRenderChecks();
}

void CMyglView::OnRenderBoxes() 
{
	 view->SetRenderMode(GRenderBoundingBoxes);
	 Redraw();
	 SetRenderChecks();
}


void CMyglView::OnRenderSolid()
{
	 view->SetRenderMode(GRenderSolid);
	 Redraw();
	 SetRenderChecks();
}

void CMyglView::OnRenderFlat()
{
	 view->SetRenderMode(GRenderFlat);
	 Redraw();
	 SetRenderChecks();
}

void CMyglView::OnRenderNonLighted() 
{
	 view->SetRenderMode(GRenderNonLighted);
	 Redraw();
	 SetRenderChecks();
	
}



void CMyglView::OnRenderGouraud()
{
	 view->SetRenderMode(GRenderGouraud);
	 Redraw();
	 SetRenderChecks();
}

void CMyglView::OnRenderLighted()
{  int mode;
   mode = view->ToggleLightMode();
   Redraw();
   SetRenderChecks();
//	 CMenu *m=GetMenu();
//	if (m) m->CheckMenuItem(ID_RENDER_LIGHTED, mode ? MF_CHECKED : MF_UNCHECKED);
}

void CMyglView::OnRenderNormals()
{
   view->ToggleNormalMode();
   Redraw();
   SetRenderChecks();
}

void CMyglView::OnRenderColored()
{
   view->ToggleLocalColors();
   Redraw();
   SetRenderChecks();
}


void CMyglView::OnRenderTextured()
{
   view->ToggleTexturing();
   Redraw();
   SetRenderChecks();
}

void CMyglView::OnRenderTransparency() 
{
   view->ToggleTransparency();
   Redraw();
   SetRenderChecks();
}




void CMyglView::OnRenderOptionsBackfacecull()
{
   view->ToggleBackfaceCulling();
   Redraw();
   SetRenderChecks();
}

void CMyglView::OnRenderOptionsRevCulling()
{
  view->ToggleFrontFaceCW();
  Redraw();
}


void CMyglView::OnRenderOptions2sidedlighting()
{
   view->ToggleTwoSidedLighting();
   Redraw();
   SetRenderChecks();
}

void CMyglView::OnRenderOptionsLocalviewer()
{
   view->ToggleLocalViewer();
   Redraw();
   SetRenderChecks();
}

void CMyglView::OnRenderOptionsAntialiaslines()
{
   view->ToggleLineAntialiasing();
   Redraw();
   SetRenderChecks();
}

// ****************************************************************************
void CMyglView::OnCameraSpinauto()
{
//	TimerId = SetTimer(1, 100, NULL);
	SetAnimating(!Animating);
	if (Animating) AnimationStartTime = GetTime();

}

void CMyglView::OnTimer(UINT nIDEvent)
{
	if (nIDEvent == m_moveTimerId) {
		if (!DoMoveTimer())
			OnIdle(1);
	} else 
	if (nIDEvent == TimerId) {
	}
	else
	  CView::OnTimer(nIDEvent);
}

// Called from idle processing (CMyglView::OnIdle)
void CMyglView::OnAnimateIdle()
{
	  view->GetCamera().Orbit(PI*0.1,PI*0.03);
	  Redraw();
}
// ****************************************************************************
// Read a File,

extern int WWWabort; // to do in GFile


    // check for expiration
int CMyglView::Expired() 
{
    if (!expired) return(0);
	AfxMessageBox("Sorry, this demo version is expired"  ORDER_INFO);
    return(1);
}




//read model
int CMyglView::ReadModel(const char *FileName) 
{ int ret=-1;

  if (firstDraw) {
    //if (Expired()) return(-1);
  	initialModel = FileName;	// window not visible at this time, redo load in OnDraw on firstDraw
	return(1);
  }

  if (Busy(1)) return(-1);
  
  CloseAllDialogs();

  if (Expired()) return(-1);

  CString msg  = "Reading file ";
  msg += FileName; 
  msg += " ...";
  
  Message(msg, PROGRESS_MESSAGE);

  BeginWaitCursor();


   // to do:kill all current loading threads

   if (mainLoader) 
   {
  		mainLoader->unref();
		mainLoader = NULL;
   }
   
   WWWabort = 0;
   mainLoader = new GFile;
   mainLoader->ClearSessionError(); 
   mainLoader->SetFlags(WWW_REPORT_ERRORS|WWW_EXECUTE_UNKNOWN_FMTS);


   mainLoader->useThread = GFile::useThreads;
   mainLoader->ref();

   
   // window to notify on completition
   mainLoader->hPostMsgWnd = GetSafeHwnd();

   mainLoader->localFileHome = view->file->localFileHome;
   mainLoader->Set(FileName,0,1);

   ret = mainLoader->Process();
   EndWaitCursor();

   if (ret == ERROR_THREAD_STARTED) // quit , notification on read file completed 
   {
   }
   else  {
   if (ret >=0) {
        BeginWaitCursor();

		AddUrlToHistory(FileName,1);
		if (mainLoader->localFileUnzipped.GetLength()>0) {
			ret = view->ReadModel(mainLoader);
			if (ret>=0) {
				SetAnimateOff();
				Redraw();
				//Message("File "+FullPath);
				CMyglDoc* pDoc = GetDocument();
				pDoc->SetPathName(FileName,TRUE);
				isComplex = 1; 
			 }

		} else ret=0;
        EndWaitCursor();

   }
   // delete mainLoader;
   mainLoader->unref();

   mainLoader =NULL;
   }

   UpdateAllDialogs();

   if (lastMessageType == PROGRESS_MESSAGE) Message("");


   return(ret);
}
	
//write model
int CMyglView::WriteModel(const char *FileName)
{
  return(-1);
}



// ****************************************************************************
// Read a File,
// normal MFC Doc structure is not used
// because of flexibility and SDI application structure
//
void CMyglView::OnFileRead()
{
//	CString    fileName;
	CString    fileExt;
	CString    FullPath;
	CString    Name;
	CString    Path;
	static fileCnt = 0;
	static dxfFileCnt = 0;

    if (Busy(1)) return;


#ifdef _EVALUATION
#if 0
	if ((fileCnt > 10) && ((fileCnt % 6) == 0)) {
		AfxMessageBox("This is an unregistered Evaluation Version" ORDER_INFO);
	}
#endif

#endif

    // check for expiration

    if (Expired()) return;


	CFileDialog dlgFile(TRUE	// open
	,NULL
	,NULL,
	OFN_FILEMUSTEXIST, // HG | OFN_HIDEREADONLY,
#ifdef _GLVIEW_LE
	"all 3D-Files|*.3dv"
#else
	"all 3D-Files|*.3dv;*.geo;*.raw;*.obj"
#endif

#ifdef _GV
	";*.wrl;*.wrz;*.gz;*.url;*.vrl"
#endif

#ifdef _MIRO
   	";*.mir"
#endif

#ifdef _DXF
	";*.dxf"
#endif
	";" IMAGE_EXTENSIONS
	"|"
	"3DV-Files (*.3dv)|*.3dv|"
	#ifdef _DXF
	"DXF-Files (*.dxf)|*.dxf|"
	#endif
	#ifdef _GV
	"VRML 1.0/2.0 Files (*.wrl)|*.wrl;*.vrml;*.wrl.gz;*.wrz;*.iv|"
	#endif
    #ifdef _MIRO
   	"Miro-Files (*.mir)|*.mir|"
    #endif
	#ifdef _HMF
	"HMF-Files (*.hmf)|*.hmf|"
	#endif
	#ifdef _GEO
	"GEO-Files (*.geo)|*.geo|"
	#endif
	#ifdef _RAW
	"RAW-Files (*.raw)|*.raw|"
	#endif
	#ifdef _OBJ
	"Wavefront Files (*.obj)|*.obj|"
	#endif
	"WWW Url References (*.vrl,*.url)|*.vrl;*.url|"
	"Image-Files (" IMAGE_EXTENSIONS ")|" IMAGE_EXTENSIONS "|"
	"All Files (*.*)|*.*|"
	"|",NULL);

	dlgFile.m_ofn.lpstrTitle = "Read 3D Object from File";

	BOOL bRet = dlgFile.DoModal() == IDOK ? TRUE : FALSE;

	if (bRet== TRUE) {
		int res=0;
		fileExt = dlgFile.GetFileExt();
		fileExt.MakeLower();
		FullPath=dlgFile.GetPathName();
		Name=FullPath.Right(FullPath.GetLength()-FullPath.ReverseFind('\\')-1);
		Path=FullPath.Left(FullPath.ReverseFind('\\'));

#ifdef _EVALUATION
//	if (fileExt == "obj") {
//		AfxMessageBox("This fileformat is not supported in this unregistered Evaluation Version" ORDER_INFO);
//		return;
//	}
/*
	if (((fileExt == "obj") || (fileExt == "dxf")) && (dxfFileCnt >= 3))  {
		AfxMessageBox("You can only read 3 DXF/OBJ files in this unregistered Evaluation Version" ORDER_INFO);
		return;
	}
*/
#endif
	fileCnt ++;

#ifdef _MIRO
				if (fileExt == "mir"){
                   GShell *s= NULL;     
                   res = ReadMiroFile(FullPath,s); 
                   if (s)  {
                       CloseAllDialogs();
	                   SetCameraMode(ORBIT_MODE);
	                   SetCameraRelativeMode(1);

                       view->SetBackfaceCulling(0);
                       res= view->SetShell(s);
	                   view->SetTexturing(0);
    	               if (s->vc.Length() || s->fc.Length()) view->SetLocalColors(1);

                       Redraw();
                   } else if (res<0) {
                       Reporter.Error("Bad file format for Miro file %s ",(const char*) FullPath);
                   } 
				} else
#endif

		res = ReadModel(FullPath);

#if 0

			BeginWaitCursor();

				// read image file as textured polygons
				if ((fileExt == "dib") || (fileExt == "bmp") ||	(fileExt == "rgb")) {
					int format = 0;
					if (fileExt == "rgb") format = IMAGE_RGB_FORMAT;
					res= view->ReadTexturedPolygon(FullPath,format);
				}
#ifdef _AVI
				else if (fileExt == "avi") {
				  // Play a avi File
					res= PlayAviFile(NULL/*pWinApp->m_hInstance*/, FullPath);
				}
#endif
#ifdef _GEO
				else if (fileExt == "geo") {
				  res= view->ReadShell(FullPath,GEO_FMT);
				}
#endif
#ifdef _OBJ
				else if (fileExt == "obj") {
				  res= view->ReadShell(FullPath,OBJ_FMT);
				  dxfFileCnt ++;
				}
#endif
#ifdef _RAW
				else if (fileExt == "raw") {
				  res= view->ReadShell(FullPath,RAW_FMT);
				}
#endif
#ifdef _HMF
				else if (fileExt == "hmf"){
				  res= view->ReadShell(FullPath,HMF_FMT);
				}
#endif
#ifdef _DXF
				else if (fileExt == "dxf"){
				  res= view->ReadModel(FullPath,DXF_FMT);
				  dxfFileCnt ++;
				}
#endif
#ifdef _GV
				else if ((fileExt == "wrl") || (fileExt == "vrml")){
				  res= view->ReadModel(FullPath,VRML_FMT);
				}
#endif
				else if ((fileExt == "wav")){
				  res= view->ReadModel(FullPath,WAV_FMT);
				}
				else if ((fileExt == "au")){
				  res= view->ReadModel(FullPath,AU_FMT);
				}
				else if ((fileExt == "mid")){
				  res= view->ReadModel(FullPath,MIDI_FMT);
				}
				else {
				  res= view->ReadShell(FullPath,D3V_FMT);
				}

				SetAnimateOff();
				Redraw();
				EndWaitCursor();
				 if (!res) {

				 }
				 Message("File "+FullPath);
				 CMyglDoc* pDoc = GetDocument();
				 pDoc->SetPathName(FullPath,TRUE);
				 isComplex = 1; // assume it is complex to get Rendering message
#endif
	}
}

int FileCheck(GShell *shell) 
{
static int checkCnt = 0;

	if (!shell) {
		AfxMessageBox("Current object is empty, nothing to save.");
		return(0);
	}

	checkCnt ++;

#ifdef _EVALUATION
	if (shell->GetFaceCount() > 10000) {
		AfxMessageBox("Save File function limited to 10000 faces in this unregistered Evaluation Version" ORDER_INFO);
		return(0);
	}
	if (checkCnt > 5) {
		AfxMessageBox("Save File function limited to 5 times only in this unregistered Evaluation Version" ORDER_INFO);
		return(0);
	}
#endif 

	return(1);
}

void CMyglView::OnFileSaveas()
{
	CString    fileExt;
	CString    FullPath;
	CString    Name;
	CString    Path;

    if (Expired()) return;


//#ifdef _EVALUATION
#if 0
	 {
		AfxMessageBox("Save File function not supported in this unregistered Evaluation Version" ORDER_INFO);
		return;
	}
#else

	// 29.12.95 changed to wrl default
	CFileDialog dlgFile(FALSE,"*.wrl",NULL,OFN_HIDEREADONLY,
#ifdef _VRML
	"VRML-Files (*.wrl;*.vrml)|*.wrl|"
#endif
	#ifdef _3DV
	"3DV-Files (*.3dv)|*.3dv|"
	#endif
//	"3DW-Files (*.3dw)|*.3dw|"
	#ifdef _HMF
	"HMF-Files (*.hmf)|*.hmf|"
	#endif
	#ifdef _POV
	"POV-Files (*.pov)|*.pov|"
	#endif
	#ifdef _RAW
	"RAW-Files (*.raw)|*.raw|"
	#endif
	#ifdef _GEO
	"GEO-Files (*.geo)|*.geo|"
	#endif
	#ifdef _STL
	"STL-Files (*.stl)|*.stl||"
	#endif
	,NULL);

	dlgFile.m_ofn.lpstrTitle = "Write object to 3D file";

	BOOL bRet = dlgFile.DoModal() == IDOK ? TRUE : FALSE;

	if (bRet== TRUE) {
		int res=0;
		fileExt = dlgFile.GetFileExt();
		fileExt.MakeLower();
		FullPath=dlgFile.GetPathName();

		Message("Writing file "+FullPath + "...");

			BeginWaitCursor();
				// write
				if ((fileExt == "dib") || (fileExt == "bmp")) {
                   DRenderFile d;
                   d.m_fileName = FullPath;

                   if (d.DoModal() == IDOK) {
                        res= RenderOnDib(view,d.m_fileName,d.m_width,d.m_height,24);
                    }


					res = -1;
				}
#ifdef _GEO
				else if (fileExt == "geo") {
                   
                   GShell *s = view->theShell;
                   GShell *snew = NULL;

                   if (s == NULL)       // compute a new for world 
                       s = snew = view->ComputeShell();
				  if (s && FileCheck(s))
   				      res=WriteShellGeo(s,FullPath);
                  if (snew) delete snew;


				} 
				else if (fileExt == "geon") {
                   GShell *s = view->theShell;
                   GShell *snew = NULL;

                   if (s == NULL)       // compute a new for world 
                       s = snew = view->ComputeShell();
				  if (s && FileCheck(s))
   				      res=WriteShellGeo(s,FullPath);
                  if (snew) delete snew;

				} 
#endif
#ifdef _STL
				else if (fileExt == "stl") {
                   GShell *s = view->theShell;
                   GShell *snew = NULL;

                   if (s == NULL)       // compute a new for world 
                       s = snew = view->ComputeShell();
				  if (s && FileCheck(s))
   				      res=WriteShellStl(s,FullPath);
                  if (snew) delete snew;
				} 
#endif
#ifdef _HMF
				else if (fileExt == "hmf") {
				  if ((view->theShell == NULL)) OnToolsBuildShell();
				  if (FileCheck(view->theShell))
				      res=WriteShellHmf(view->theShell,FullPath);
				}
#endif
#ifdef _VRML
				else if ((fileExt == "wrl")|| (fileExt == "vrml")) {

					if (view->theShell) {
                      // Convert shell to full VRML scene including current options
					  // changed to a scene node 

                      GvNode * node = view->CreateVrmlScene();
                      if (node) {
                          CloseAllDialogs();

                          view->theShell = NULL;
                          view->SetNode(node,0);
                      }
                  }    
				  
                  if (view->theWorld || view->GetScene()) {

					gbool vrml2 = view->IsVrml2();

				  	GioTraversal traversal(FullPath,"w");
					traversal.vrml2 = vrml2;
					traversal.format = VRML_FMT;
					traversal.writeInlines = 0;
					traversal.minimizeSize = AfxGetApp()-> GetProfileInt("GLView","minimizeFiles",0);
					traversal.astream->minimizeSize = traversal.minimizeSize; 


					traversal.DoFileHeader();
					traversal.astream->comment("written by " _PROGRAM _PROGRAM_TYPE " " _VERSION " " URL_HOME);
					traversal.astream->nl();
					
					
					InitializeGvAttributes(traversal);
					traversal.InitializeAttributes();

					view->Traverse(traversal);
				  
				  } else {
				  	if ((view->theShell == NULL)) OnToolsBuildShell();
				  	if (FileCheck(view->theShell))
				      res=WriteShellVrml(view->theShell,FullPath,TRUE);
				  }
				} 
#endif
#ifdef _RAW
				else if (fileExt == "raw") {
                   GShell *s = view->theShell;
                   GShell *snew = NULL;

                   if (s == NULL)       // compute a new for world 
                       s = snew = view->ComputeShell();
				  if (s && FileCheck(s))
				  	    res=WriteShellRaw(s,FullPath);
                  if (snew) delete snew;

              } 
#endif
#ifdef _POV
				else if (fileExt == "pov") {
                   GShell *s = view->theShell;
                   GShell *snew = NULL;

                   if (s == NULL)       // compute a new for world 
                       s = snew = view->ComputeShell();
				  if (s && FileCheck(s))
				  	    res=WriteShellPov(s,FullPath);
                  if (snew) delete snew;


				} 
#endif
				else if (fileExt == "3dw") {
#ifndef _EVALUATION
				  if (FileCheck(view->theShell))
				  if (view->theWorld)
				      res=view->theWorld->Write(FullPath);
				  else 
#endif
				  res = -1;
				} 
#ifdef _3DV
				else if (fileExt == "3dv") {
                   
                   GShell *s = view->theShell;
                   GShell *snew = NULL;

                   if (s == NULL)       // compute a new for world 
                       s = snew = view->ComputeShell();
				  if (s && FileCheck(s))
				  	    res=WriteShell3DV(s,FullPath);
                  if (snew) delete snew;

				}
#endif
			else  {
				Reporter.Error("%s not supported in this version",fileExt);
				res = -1;
			}				
			// Redraw();
			EndWaitCursor();
			if (!res) {
			}
			if (res>= 0) {
				Message("File "+FullPath);
				CMyglDoc* pDoc = GetDocument();
				// pDoc->SetPathName(FullPath,TRUE);
			}
	}
#endif
}


// ****************************************************************************

void CMyglView::OnRenderOptionsLoadtexture()
{
		CString		path;
        CString     current;

        current = view->GetTexture();

		if (current.ReverseFind('\\') > 0) {
			path=current.Left(current.ReverseFind('\\'));
		}


		CFileDialog f(TRUE,NULL,current,OFN_FILEMUSTEXIST,
		    "Image-Files (" IMAGE_EXTENSIONS ")|" IMAGE_EXTENSIONS "|"
		    "RGB-Files (*.rgb)|*.rgb||",NULL);

	    f.m_ofn.lpstrTitle = "Read Texture from image file";

		if (path.GetLength()>0) f.m_ofn.lpstrInitialDir = path;

		if (f.DoModal() == IDOK) {
			current = f.GetPathName();
	        Message("Reading texture "+current + "...",PROGRESS_MESSAGE);
		    
            BeginWaitCursor();
		
		    int fmt = 0;
	    	CString fileExt = f.GetFileExt();
    		fileExt.MakeLower();
			fmt = GetFileTypeExtension(fileExt);

			int ret = view->EditTexture(current,fmt);
			if (ret <0) {
				Reporter.Error("Error reading texture file %s",(const char *)current);
			}


		    // enable texturing
		    view->SetTexturing(1);

		    Redraw();
		
		    EndWaitCursor();
		    Message("Texture "+current);
            Redraw();
		}
    
    
}

void CMyglView::OnMaterialBackgroundImage() 
{
		CString		path;
        CString     current;

        current = view->GetBackgroundImage();

		if (current.ReverseFind('\\') > 0) {
			path=current.Left(current.ReverseFind('\\'));
		}


		CFileDialog f(TRUE,NULL,current,OFN_FILEMUSTEXIST,
		    "Image-Files (" IMAGE_EXTENSIONS ")|" IMAGE_EXTENSIONS "|"
		    "RGB-Files (*.rgb)|*.rgb||",NULL);
    
        f.m_ofn.lpstrTitle = "Read Background from image file";
		if (path.GetLength()>0) f.m_ofn.lpstrInitialDir = path;

		if (f.DoModal() == IDOK) {
			current = f.GetPathName();
            view->EditBackgroundImage(current);
            Redraw();
		    Message("Background "+current);

		}
}

void CMyglView::OnMaterialBackgroundReset() 
{
    view->EditBackgroundImage("");
    Redraw();

    
}

void CMyglView::OnUpdateMaterialBackgroundReset(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}



// ****************************************************************************
// menu button check handlers

void CMyglView::OnUpdateRenderWireframe(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(view->GetRenderMode() == GRenderWireframe);
}

void CMyglView::OnUpdateRenderVertices(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(view->GetRenderMode() == GRenderVertices);
}

void CMyglView::OnUpdateRenderBoxes(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(view->GetRenderMode() == GRenderBoundingBoxes);
}

void CMyglView::OnUpdateRenderSolid(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(view->GetRenderMode() == GRenderSolid);
}

void CMyglView::OnUpdateRenderFlat(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(view->GetRenderMode() == GRenderFlat);
}

void CMyglView::OnUpdateRenderNonLighted(CCmdUI* pCmdUI)
{ 
	pCmdUI->SetCheck(view->GetRenderMode() == GRenderNonLighted);
}


void CMyglView::OnUpdateRenderGouraud(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(view->GetRenderMode() == GRenderGouraud);
}

void CMyglView::OnUpdateRenderLighted(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(view->GetLightMode());
}

void CMyglView::OnUpdateRenderColored(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(view->GetLocalColors());
}

void CMyglView::OnUpdateRenderTextured(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(view->GetTexturing());
}


void CMyglView::OnUpdateRenderTransparency(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(view->GetTransparency());
}


void CMyglView::OnUpdateRenderNormals(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(view->GetNormalMode());
}

void CMyglView::OnUpdateRenderOptionsBackfacecull(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(view->GetBackfaceCulling());
}

void CMyglView::OnUpdateRenderOptionsRevCulling(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(view->GetFrontFaceCW());
}

void CMyglView::OnUpdateRenderOptions2sidedlighting(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(view->GetTwoSidedLighting());
}

void CMyglView::OnUpdateRenderOptionsLocalviewer(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(view->GetLocalViewer());
}

void CMyglView::OnUpdateRenderOptionsAntialiaslines(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(view->GetLineAntialiasing());
}

// ****************************************************************************


CString MaterialFileName;

// Material load file dialogbox + loading
int CMyglView::LoadMaterial()
{
	CString    FullPath;
	CString    fileExt;
//	fileName = MaterialFileName;

	CFileDialog dlgFile(TRUE,".3dm",MaterialFileName,
	OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
	"Material-Files (*.3dm)|*.3dm||",NULL);

	dlgFile.m_ofn.lpstrTitle = "Read Material from ASCII File";

	BOOL bRet = dlgFile.DoModal() == IDOK ? TRUE : FALSE;

	if (bRet== TRUE) {
				fileExt = dlgFile.GetFileExt();
				fileExt.MakeLower();
				FullPath=dlgFile.GetPathName();

				BeginWaitCursor();

					view->ReadMaterial(0,FullPath);

				Redraw();
				EndWaitCursor();
				MaterialFileName= FullPath;
	}
	return(bRet);
}

void CMyglView::OnMaterialLoad()
{
  LoadMaterial();
}

// ****************************************************************************

// Apply Material state to view
void CMyglView::ApplyMaterial(DMaterial &d)
{
   view->SetMaterial(d.ambient, d.diffuse,d.specular,d.emission,d.m_shininess,d.m_alpha);
   Redraw();
}

// Get Material state from	view
void CMyglView::GetMaterial(DMaterial &d)
{
	view->GetMaterial(d.ambient, d.diffuse,d.specular,d.emission,d.m_shininess,d.m_alpha);
}

void CMyglView::OnMaterialEdit()
{
   if (dialogs.m_dMaterial == NULL) {
    	dialogs.m_dMaterial = new DMaterial(this,this);
    	GetMaterial(*dialogs.m_dMaterial);
		dialogs.m_dMaterial->Create(DMaterial::IDD);
	}
    else {
	    GetMaterial(*dialogs.m_dMaterial);
        dialogs.m_dMaterial->UpdateData(FALSE);	// View state ==> to dialog
    }
	dialogs.m_dMaterial->ShowWindow(SW_SHOWNORMAL);


#if 0
   
    DMaterial d(NULL,this);
	GetMaterial(d);


	BOOL bRet = d.DoModal() == IDOK ? TRUE : FALSE;

	if (bRet) {
	   ApplyMaterial(d);
	} else {
		// to do: if already applied, undo ?
	}
#endif
}

// ****************************************************************************
// edit a single color using standard dialog

int CMyglView::EditColor(const char *prompt, Point &color)
{
	COLORREF clr=RGB(0,0,0);

	Point c = color * 255.0;

	clr = RGB(int(c.x),int(c.y), int (c.z));

	CColorDialog dlgColor(clr);

	dlgColor.m_cc.Flags |= CC_FULLOPEN; // enable custom color section

	// dlgColor.SetWindowText(prompt); Wrong

	if (dlgColor.DoModal() == IDOK)
	{
		clr = dlgColor.GetColor();
		c.Set(GetRValue(clr) / 255.0,GetGValue(clr) / 255.0,GetBValue(clr) / 255.0);
		color = c;
		//Redraw();
		return(1);
	}
	return(0);
}



// Edit Background color using color dialog
void CMyglView::OnMaterialBackground()
{	Point c;

	view->GetBackgroundColor(c);
	if (EditColor("Edit Background Color",c)) {
		view->SetBackgroundColor(c);
		Redraw();
	}
}

// ****************************************************************************
// Texture Options

// Apply Dialogsetting to view
void CMyglView::ApplyTexture(DTexture &d)
{
	  view->renderState.textureParameters.SetMinFilter(d.m_tfilter_min);
	  view->renderState.textureParameters.SetMagFilter(d.m_tfilter_max);
	  view->renderState.textureParameters.SetBlend(d.m_tblend);
	  view->renderState.textureParameters.SetWrap(d.m_repeat_t,d.m_repeat_s);
	  view->renderState.textureParameters.gen_mipmaps=d.m_gen_mipmaps;
	  view->renderState.textureParameters.set_diffuse_white=d.m_white ;

	  CstringToPoint(d.m_rotate,view->renderState.textureTransform3.rotate);
	  CstringToPoint(d.m_scale,view->renderState.textureTransform3.scale);
	  CstringToPoint(d.m_translate,view->renderState.textureTransform3.translate);

	  view->renderState.SetTextureParameters();

	  //?????xxxxxxxxxxxxxxxxx removed view->renderState.SetTextureTransform(view->renderState.textureTransform3);

#ifdef _OGL
	  view->renderState.textureGeneration.Set(d.m_tfunction);
	  view->renderState.textureGeneration.glSet();
#endif

	  view->renderState.SetPerspectiveCorrectionHint(d.m_perspective_hint ? GL_NICEST : GL_FASTEST);
	  view->renderState.textureParameters.SetMaxSize(d.m_max_size,d.m_max_size);

#ifdef _D3D
	  // redo exbufs, because of texture matrix
	  view->renderState.ReleaseExBufs();
#endif

#ifdef _OGL
	  // mipmaps changed,
	  view->renderState.ReleaseTextures();
#endif



   Redraw();
}

// copy view setting into dialog Texture
void CMyglView::GetTexture(DTexture &d)
{
	  gbool repeat_t,repeat_s;

	  d.m_tfilter_min = view->renderState.textureParameters.GetMinFilter();
	  d.m_tfilter_max = view->renderState.textureParameters.GetMagFilter();
	  d.m_tblend = view->renderState.textureParameters.GetBlend();
	  view->renderState.textureParameters.GetWrap(repeat_s,repeat_t);
	  d.m_repeat_s = repeat_s;
	  d.m_repeat_t = repeat_t;
	  d.m_gen_mipmaps= view->renderState.textureParameters.gen_mipmaps;
	  d.m_white = view->renderState.textureParameters.set_diffuse_white;

	  CstringFromPoint(d.m_rotate,view->renderState.textureTransform3.rotate);
	  CstringFromPoint(d.m_scale,view->renderState.textureTransform3.scale);
	  CstringFromPoint(d.m_translate,view->renderState.textureTransform3.translate);

#ifdef _OGL
	  d.m_tfunction = view->renderState.textureGeneration.parameter;
#endif

      d.m_perspective_hint = (view->renderState.perspectiveCorrectionHint == GL_NICEST);
	  view->renderState.textureParameters.GetMaxSize(d.m_max_size,d.m_max_size);

}

void CMyglView::OnMaterialTextureOptions()
{
   if (dialogs.m_dTexture == NULL) {
    	dialogs.m_dTexture = new DTexture(this,this);
		dialogs.m_dTexture->Create(DTexture::IDD);
	}
	dialogs.m_dTexture->ShowWindow(SW_SHOWNORMAL);
}

// ****************************************************************************
// Parameter
// Apply Dialogsettings to view
void CMyglView::ApplyParameter(DParameter &d)
{
  if (!view->parameter) view->parameter = new GParameter;

#ifdef _OGL
  view->renderState.textureGeneration.SetOff();
  view->renderState.textureGeneration.glSet();
#endif

  view->parameter->changed = 1;
  view->parameter->SetFunction(d.m_function);
  view->parameter->SetSource(d.m_source);
  view->parameter->SetMapping(d.m_mapping);
  view->parameter->enabled=d.m_enable;
  view->parameter->normalize=d.m_normalize ;
  view->parameter->showSurface=d.m_showSurface;
  view->parameter->showMappedObject= d.m_showMappedObject;

  DecomposedMatrix dm;
  CstringToPoint(d.m_rotate,dm.rotate);dm.rotate *= Deg2Rad;
  CstringToPoint(d.m_scale,dm.scale);
  CstringToPoint(d.m_translate,dm.translate);
  Matrix m;
  dm.Compose(m);
  view->parameter->SetMatrix(m);

  // translate rotate scale

   Redraw();
}

// copy view setting into dialog Texture
void CMyglView::GetParameter(DParameter &d)
{
  if (!view->parameter) view->parameter = new GParameter;
  if (!view->parameter) return;
  d.m_function = view->parameter->Function;
  d.m_source = view->parameter->Source;
  d.m_mapping = view->parameter->Mapping;
  d.m_enable = view->parameter->enabled;
  d.m_normalize = view->parameter->normalize;
  d.m_showSurface = view->parameter->showSurface;
  d.m_showMappedObject = view->parameter->showMappedObject;
  DecomposedMatrix m(view->parameter->m.m);;
  m.rotate *= Rad2Deg;
  CstringFromPoint(d.m_rotate,m.rotate);
  CstringFromPoint(d.m_scale,m.scale);
  CstringFromPoint(d.m_translate,m.translate);
  // translate rotate scale
}



void CMyglView::OnParameter()
{

   if (dialogs.m_dParameter == NULL) {
    	dialogs.m_dParameter = new DParameter(this,this);
		dialogs.m_dParameter->Create(DParameter::IDD);
	}


	dialogs.m_dParameter->ShowWindow(SW_SHOWNORMAL);

}

// ****************************************************************************
// Fog

// Apply Dialogsetting to view
void CMyglView::ApplyFog(Dfog &d)
{
	  view->renderState.fog.enable = d.m_enable;
	  view->renderState.fog.SetMode(d.m_type);
	  view->renderState.fog.density = d.m_density;

	  view->renderState.fog.start = d.m_start;
	  view->renderState.fog.end = d.m_end;
	  view->renderState.fog.useBackgroundColor = d.m_use_background;
#ifdef _OGL
	  view->renderState.fog.SetQuality(d.m_quality);
#endif
	  Point c;
	  CstringToPoint(d.m_color,c);
	  view->renderState.fog.color.Set(c);

	  view->renderState.SetFog();

   Redraw();
}

// copy view setting into dialog Texture
void CMyglView::GetFog(Dfog &d)
{
	  d.m_enable = view->renderState.fog.enable;
	  d.m_type = view->renderState.fog.GetMode();
	  d.m_density = view->renderState.fog.density;
	  d.m_start = view->renderState.fog.start;
	  d.m_end = view->renderState.fog.end;
	  d.m_use_background = view->renderState.fog.useBackgroundColor;
#ifdef _OGL
	  d.m_quality = view->renderState.fog.GetQuality();
#endif
	  Point c;
	  view->renderState.fog.color.Get(c);
	  CstringFromPoint(d.m_color,c);

}


void CMyglView::OnFog()
{

   if (dialogs.m_dFog == NULL) {
    	dialogs.m_dFog = new Dfog(this,this);
		dialogs.m_dFog->Create(Dfog::IDD);
	}


	dialogs.m_dFog->ShowWindow(SW_SHOWNORMAL);

}

// ****************************************************************************
// Background Options

// Apply Dialogsetting to view
void CMyglView::ApplyBackground(DBackground &d)
{
	Redraw();
}

// copy view setting into dialog
void CMyglView::GetBackground(DBackground &d)
{	Point c;

	view->GetBackgroundColor(c);
	CstringFromPoint(d.m_color1,c);
}

void CMyglView::OnMaterialBackgroundOptions()
{
	DBackground d(NULL,this);


	BOOL bRet = d.DoModal() == IDOK ? TRUE : FALSE;

	if (bRet) {
	   ApplyBackground(d);
	} else {
		// to do: if already applied, undo ?
	}

}

// ****************************************************************************

void CMyglView::Apply(DDeformSphere &d)
{

  if (view->deformer && d.m_drag) {
		SetMode(DRAG_DEFORMER_MODE);
  }

  Redraw();
}

void CMyglView::Get(DDeformSphere &d)
{

}


void CMyglView::OnDeformSphere()
{
	SetAnimateOff();

	if (view->deformer == NULL)
	   view->deformer = new GSphereDeform(Point(5.0,0.0,0.0),3.0);

   if (dialogs.m_dDeformSphere == NULL) {
    	dialogs.m_dDeformSphere = new DDeformSphere(this,this,(GSphereDeform *)view->deformer);
		dialogs.m_dDeformSphere->Create(DDeformSphere::IDD);
	}

   dialogs.m_dDeformSphere->SetDeform((GSphereDeform *) view->deformer);

   dialogs.m_dDeformSphere->ShowWindow(SW_SHOWNORMAL);
}

void CMyglView::OnUpdateDeformSphere(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(view->theShell != NULL);	
	
}

void CMyglView::OnToolsDeformDisable()
{
   if (dialogs.m_dDeformSphere) { 
	   dialogs.m_dDeformSphere->DestroyWindow(); 
	  delete dialogs.m_dDeformSphere; dialogs.m_dDeformSphere = NULL; 
   }
   if (view->deformer) {  
		 delete view->deformer;
		 view->deformer = NULL;
   }
}


// ****************************************************************************
// Config


void CMyglView::OnOptionsConfig()
{
	SetAnimateOff();

	DOptions d(NULL /*,this*/);
	d.m_print_time = doPrintTime;
	
	d.m_degrade = view->degradeDuringMove;
	d.m_degrade_style = GRenderModeLookup(view->degradeRenderMode);


	d.m_build_tristrips = view->GetComputePrimitiveList();
	d.m_gl_cache = view->useGlCache;
	d.m_update_colors = m_updateColors;
	d.m_draw_frontbuffer = view->renderState.GetBufferMode() == 1;

    d.m_zoom2 = view->renderState.GetBufferMode() == 2;

#ifdef _OGL    
	d.m_gl_extensions = view->renderState.useVertexArrayEXT;
#endif

	d.m_auto_spin = AfxGetApp()->GetProfileInt("GLView","AutoSpin",0); // changed default to off 
	d.m_win_x = AfxGetApp()->GetProfileInt("GLView","WinSizeX",0);
	d.m_win_y = AfxGetApp()->GetProfileInt("GLView","WinSizeY",0);

    d.m_zbuffer_16= AfxGetApp()->GetProfileInt("GLView","Zbuffer16",0);
	d.m_num_sounds =  AfxGetApp()-> GetProfileInt("Sound","NumSounds",6);
	int cpuBudget = AfxGetApp()-> GetProfileInt("Sound","CpuBudget",0);
	
	d.m_use_rsx =  AfxGetApp()-> GetProfileInt("Sound","UseRsx",0);
	d.m_use_rsx =  AfxGetApp()-> GetProfileInt("Sound","UseRsx",0);
	d.m_minimize_files = AfxGetApp()-> GetProfileInt("GLView","minimizeFiles",0);



   
    CMainFrame *pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
    d.m_save_window_size = pFrame->m_saveWindowSize = AfxGetApp()-> GetProfileInt("GLView","saveWindowSize",1);
   



	SetAnimateOff();

	BOOL bRet = d.DoModal() == IDOK ? TRUE : FALSE;

	if (bRet) {
		doPrintTime = d.m_print_time;
		view->degradeDuringMove = d.m_degrade;
		view->degradeRenderMode = (GRenderMode) GRenderModeLookup(d.m_degrade_style);
		if (view->degradeRenderMode <0) view->degradeRenderMode = GRenderWireframe;

	  	view->SetUseGlCache(d.m_gl_cache);
		
		m_updateColors=d.m_update_colors;
		
		view->renderState.SetBufferMode(d.m_draw_frontbuffer ? 1 : (d.m_zoom2 ? 2: 0));

#ifdef _OGL		
		view->renderState.useVertexArrayEXT=d.m_gl_extensions ;
#endif

	   view->SetComputePrimitiveList(d.m_build_tristrips);
      
      pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
      pFrame->m_saveWindowSize = d.m_save_window_size;

	    AfxGetApp()->WriteProfileInt("GLView","AutoSpin",d.m_auto_spin);
	    AfxGetApp()->WriteProfileInt("GLView","UpdateColors",m_updateColors);
	    AfxGetApp()->WriteProfileInt("GLView","WinSizeX",d.m_win_x);
	    AfxGetApp()->WriteProfileInt("GLView","WinSizeY",d.m_win_y);
	    AfxGetApp()->WriteProfileInt("GLView","Zbuffer16",d.m_zbuffer_16);
       
        SetProfile("GLView","saveWindowSize",pFrame->m_saveWindowSize);
	    
		view->maxNumSounds= d.m_num_sounds;
		view->useRSXSound= d.m_use_rsx;

		SetProfile("Sound","numSounds",view->maxNumSounds);
 	    //int cpuBudget = SetProfile("Sound","RsxCpuBudget",0);
		SetProfile("Sound","useRsx",view->useRSXSound);
		SetProfile("Sound","quality",view->soundQuality);
		SetProfile("Sound","doppler",view->soundDoppler);
		SetProfile("GLView","minimizeFiles",d.m_minimize_files);


	} else {
		// to do: if already applied, undo ?
	}
}
// ****************************************************************************
// DrawStyle

// Apply Dialogsetting to view
void CMyglView::ApplyStyle(DStyle &d)
{	Point c;
	// edges
	view->renderState.edges.colored = d.m_edges_colored;
	view->renderState.edges.lighted = d.m_edges_lighted ;
	view->renderState.edges.useMaterialColor = d.m_edges_matcolor;
	view->renderState.edges.smooth = d.m_edges_smooth;
	view->renderState.edges.textured = d.m_edges_textured;
	view->renderState.edges.visible = d.m_edges_visible;

	CstringToPoint(d.m_edge_color,c);
	view->renderState.edgeColor.Set(c);

	d.applied = 1;
	Redraw();
}

// copy view setting into dialog
void CMyglView::GetStyle(DStyle &d)
{	Point c;
	// edges
	d.m_edges_colored = view->renderState.edges.colored;
	d.m_edges_lighted = view->renderState.edges.lighted;
	d.m_edges_matcolor = view->renderState.edges.useMaterialColor;
	d.m_edges_smooth = view->renderState.edges.smooth;
	d.m_edges_textured = view->renderState.edges.textured;
	d.m_edges_visible = view->renderState.edges.visible;

	view->renderState.edgeColor.Get(c);
	CstringFromPoint(d.m_edge_color,c);
}

// Apply Dialogsetting to view
void CMyglView::ApplyStyle(DStylePage &d)
{	Point c;

	RenderFlags *p=NULL;

	CstringToPoint(d.m_color,c);

	switch (d.m_target) {
	case DVertices:
		p = &view->renderState.vertices;
		view->renderState.vertexColor.Set(c);
		break;

	case DEdges:
		p = &view->renderState.edges;
		view->renderState.edgeColor.Set(c);
		break;

	case DFaces:
		p = &view->renderState.faces;
		view->renderState.faceColor.Set(c);
		break;

	case DNormals:
		p = &view->renderState.normals;
		view->renderState.normalColor.Set(c);
		break;
	case DBBox:
		p = &view->renderState.bboxes;
		view->renderState.bboxColor.Set(c);
		break;
	default :
		TRACE("Unknown drawstyle %d\n",d.m_target);

	}
	if (p == NULL) return;

	p->colored = d.m_colored;
	p->lighted = d.m_lighted ;
	p->useMaterialColor = d.m_matcolor;
	p->textured = d.m_textured;
	p->smooth = d.m_smooth;
	p->visible = d.m_visible;

	view->InvalidateModelCache();

	d.applied = 1;
	Redraw();
}

// copy view setting into dialog
void CMyglView::GetStyle(DStylePage &d)
{	Point c;
	RenderFlags *p=NULL;


	switch (d.m_target) {
	case DVertices:
		p = &view->renderState.vertices;
		view->renderState.vertexColor.Get(c);
		break;

	case DEdges:
		p = &view->renderState.edges;
		view->renderState.edgeColor.Get(c);
		break;

	case DFaces:
		p = &view->renderState.faces;
		view->renderState.faceColor.Get(c);
		break;

	case DNormals:
		p = &view->renderState.normals;
		view->renderState.normalColor.Get(c);
		break;
	case DBBox:
		p = &view->renderState.bboxes;
		view->renderState.bboxColor.Get(c);
		break;
	default :
		TRACE("Unknown drawstyle %d\n",d.m_target);

	}
	if (p == NULL) return;


	d.m_colored = p->colored;
	d.m_lighted = p->lighted;
	d.m_matcolor = p->useMaterialColor;
	d.m_smooth = p->smooth;
	d.m_textured = p->textured;
	d.m_visible = p->visible;

	CstringFromPoint(d.m_color,c);
}


void CMyglView::OnRenderStyle()
{

	// Property sheet for Vertices, Edges, Faces, Normals

	CPropertySheet dlgPropertySheet(IDS_DRAWING_STYLE);
	DStylePage stylePageVertex(IDS_VERTEX,this,DVertices);
	DStylePage stylePageEdge(IDS_EDGE,this,DEdges);
	DStylePage stylePageFace(IDS_FACE,this,DFaces);
	DStylePage stylePageNormal(IDS_NORMAL,this,DNormals);
	DStylePage stylePageBBox(IDS_BBOX,this,DBBox);

	dlgPropertySheet.AddPage(&stylePageVertex);
	dlgPropertySheet.AddPage(&stylePageEdge);
	dlgPropertySheet.AddPage(&stylePageFace);
	dlgPropertySheet.AddPage(&stylePageNormal);
	dlgPropertySheet.AddPage(&stylePageBBox);

	if (dlgPropertySheet.DoModal() == IDOK)
	{
/*
		m_pShapeSelected->m_shapecolor = colorPage.m_nColor;
		m_pShapeSelected->m_shapestyle	= (SHAPE_STYLE)stylePage.m_nShapeStyle;
		GetDocument()->SetModifiedFlag();
		GetDocument()->UpdateAllViews(NULL);
*/
	}


//	DStyle d(NULL, this);
//	BOOL bRet = d.DoModal() == IDOK ? TRUE : FALSE;
}

void CMyglView::ApplyDrawStyle(DDrawStyle &d)
{
#ifdef _OGL
	GExtendedDrawStyle **p=NULL;
	switch (d.m_target) {
	case DVertices:
			p = &view->renderState.vertexDrawStyle; break;
	case DEdges:
			p = &view->renderState.edgeDrawStyle; break;
			break;
	case DFaces:
		//	p = &view->renderState.faceDrawStyle; break;
	case DNormals:
		p = &view->renderState.normalDrawStyle; 	break;
	
	case DBBox:
		p = &view->renderState.bboxDrawStyle; 		break;
	
	default :
		TRACE("Unknown drawststyle %d\n",d.m_target);
	}
	if (p == NULL) return;
	if (*p == NULL) *p = new GExtendedDrawStyle();

	if (p && *p) {
	GExtendedDrawStyle *s= *p;

	s->SetScale(d.m_scale1,d.m_scale2);
	s->SetSteps(d.m_xsteps,d.m_ysteps);
	s->SetDrawStyle(d.m_style);
	s->SetTextured(d.m_texture);
	s->orient = d.m_orient;
	s->enabled = d.m_enable;

	s->SetSmooth(d.m_smooth);
	//s->SetLighted(d.m_smooth);
	s->drawStyle = (d.m_fill) ? GLU_FILL : ( (!d.m_point) ? GLU_LINE : GLU_POINT);
	s->SetChanged();
	view->InvalidateModelCache();
	}

	Redraw();
#endif

}

// copy view setting into dialog
void CMyglView::GetDrawStyle(DDrawStyle &d)
{
#ifdef _OGL

    GExtendedDrawStyle **p=NULL;
	switch (d.m_target) {
	case DVertices:
			p = &view->renderState.vertexDrawStyle; break;
	case DEdges:
			p = &view->renderState.edgeDrawStyle; break;
	case DFaces:
		//	p = &view->renderState.faceDrawStyle; break;
	case DNormals:
		p = &view->renderState.normalDrawStyle; 	break;
	case DBBox:
		p = &view->renderState.bboxDrawStyle; 	break;
	}
	if (p && *p) {
	GExtendedDrawStyle *s= *p;
	d.m_scale1 = s->scale1;;
	d.m_scale2 = s->scale2;
	d.m_xsteps = s->xsteps;
	d.m_ysteps = s->ysteps;
	d.m_style =  s->GetDrawStyle();
	d.m_texture = s->textured;
	d.m_orient = s->orient;
	d.m_enable = s->enabled;

	// lighted =?
	d.m_smooth = s->smooth;

	d.m_fill = (s->drawStyle == GLU_FILL);
	d.m_point = (s->drawStyle == GLU_POINT);

	} else {
	d.m_fill=1;
	d.m_smooth=1;
	switch (d.m_target) {
	case DVertices:
			d.m_style = "sphere";   break;
	case DEdges:
			d.m_style = "cylinder";     break;
	case DNormals:
			d.m_style = "cone";     break;
	}
	}
#endif
}

// Object New dialog
//static	DObjNew dObjectNew();

void CMyglView::OnFileNew()
{
    if (Expired()) return;

	SetAnimateOff();

    if (dialogs.m_dObjectNew == NULL) dialogs.m_dObjectNew = new DObjNew(this,this);
	if (dialogs.m_dObjectNew->DoModal() == IDOK) { }
}


void CMyglView::OnEnterIdle(UINT nWhy, CWnd* pWho) 
{
	CView::OnEnterIdle(nWhy, pWho);
	
	// TODO: Add your message handler code here
}



BOOL CMyglView::OnIdle(LONG lcount)
{	// to do : if visible & !iconized ...

    int ret = TRUE;
	int redraw=FALSE;

	idleCnt ++;

	// check for a camera animation
    if (view->AnimatingCamera()) {
        if (view->AnimateCameraStep()==2) {
             if (dialogs.m_dViewpoint) {
                dialogs.m_dViewpoint->UpdateData(FALSE);
            }
        };
        redraw = TRUE;
        ret = TRUE;
    }
	else 
	if (view->AnimatingPhysicalCamera())
		redraw = TRUE;


	// check animations 
	if (view->time.Enabled() && (!view->time.Paused())) {
		
		// step the time 
		if (view->playRealtime) {
		   view->time.Step(LastFrameTime);  // *sgn(dt)
	 	} else 
	 	  view->time.Step();

		if (dialogs.m_dTime) dialogs.m_dTime->UpdateT(); // update time display

		// trigger any VRML 2.0 time sensors
		// has a sensor triggered and changed some field values ??
		if (view->TriggerTimeSensors() && view->getRedraw() /* GvConnection::getTriggeredConnectionsCnt()>0*/ ) {
			redraw = TRUE;
			ret = TRUE;
		}
		else {
			// if there are VRML 1.0 GLView  animation extensions
			if (view->sceneInfo && view->sceneInfo->IsAnimated())
				redraw = TRUE;
		}

	} else {
		if (!redraw) ret = FALSE;
	}

	// check for any other changes
//	if (redraw || (view->needUpdate >0) || GvConnection::getTriggeredConnectionsCnt()>0) {
//		redraw = TRUE;
//	}
	if (redraw || (view->needUpdate >0) || view->getRedraw()) {
		redraw = TRUE;
		//view->getRedraw() = FALSE; // new 12.04.97
	}

	//check for replaceWorld loadURL requests 
	view->CheckForSceneChange();

#if 0
	if (Mode > NULL_MODE) {
	   if (redraw) Redraw();
	   if (DoMoveTimer()); // we are moving, call the move action 
			return;
	}
#endif

	
	view->CheckTasks();

	// default GLView spin animation feature
	if (Animating) {
		OnAnimateIdle();
		redraw = TRUE;
#if 0
		#ifdef _EVALUATION
		if ((GetTime()-AnimationStartTime) > 40.0) {
		  AfxMessageBox("This is an unregistered Evaluation Version" ORDER_INFO);
		  AnimationStartTime = GetTime();
		}
		#endif
#endif

	} else {
	if (doAutoAnimate) { // should auto animation be enabled ?
		if (lcount <3) {
			LastTime = GetTime();
		}
		else
		if (m_colorsOk && !isComplex 
			&& (GetMode() != WALK_MODE)
			&& (GetTime()-LastTime) >= (AutoAnimateTimeOut)) {  
			// goto AutoAnimate after a While if color map ok & scene not to complex
			// Go into auto animation
			Animating = 1;
			OnAnimateIdle();
			LastTime = GetTime();
			AnimationStartTime = LastTime;
			redraw = TRUE;
		}
	} // doAutoAnimate
 }

 // trigger repaint
 if (redraw)  OnDraw(NULL); // Redraw();
 return(ret);

}


void CMyglView::OnObjectFlipnormals()
{
	if (view->theShell) {
	   view->theShell->FlipNormals();
	   Redraw();
	}
	else Reporter.Warning("Flip Normals only supported for single object scenes \n");
}

void CMyglView::OnUpdateObjectFlipnormals(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(view->theShell != NULL);
	if (view->theShell) pCmdUI->SetCheck(view->theShell->NormalsFlipped());
}

// ****************************************************************************
// Object New

GMesh * EvalSurfaceToMesh(GSurface *s,float umin,float umax,float vmin,float vmax,float usteps,float vsteps)
{ float du = (umax - umin) / (float) usteps;;
  float dv = (vmax - vmin) / (float) vsteps;

  // close surface
  usteps++;
  vsteps++;

  PointArray p(usteps*vsteps);
  Point *pp = p.DataRW();

  PointArray n(usteps*vsteps);
  Point *pn = n.DataRW();
  int nok=0;

  Point arg;
  
  int ui,vi;
  float u,v;
  arg.z = 0.0;

  for (v=vmin,vi=0; vi < vsteps; v+=dv,vi++) {
	arg .y = v;
  	for (u=umin,ui=0; ui < usteps; u+=du,ui++) {
		arg.x = u;
		s->Eval(arg, *pp);	 		// compute surface value
		pp++;
		nok=s->EvalNormal(arg, *pn);		// compute surface normal
		pn++;
	}
  }		
  GMesh *obj = new GMesh(usteps,vsteps,p);
  
  obj->SetVertexParametersNative();
  

  if (nok) {
   obj->Update();
   obj->SetVN(n);
  }
  
  return(obj);

}

// Apply Dialogsetting to view
void CMyglView::ApplyObject(DObjNew &d)
{
GShell *obj = 0;	// the new object
GvNode *node = 0;    // """

float x=0.0, y=0.0, z=0.0;

float r = d.m_r;
float h = d.m_height;

if (d.m_type == "ronny") { obj= new_ronny(); }
else if (d.m_type == "cube") {		
	obj = new_box3d(0,x,y,z,r,r,r);
} else if (d.m_type == "cube with parameters") {
		obj = NewBox(x,y,z,r,r,r);
} else if (d.m_type == "cylinder") { 
		obj = NewCylinder(x,y,z,r,h,d.m_stepsx);
} else if (d.m_type == "cylindery") { 
		obj = NewCylinderVrml(x,y,z,r,h,d.m_stepsx);
} else if (d.m_type == "coney") { 
		obj = NewConeVrml(x,y,z,r,h,d.m_stepsx);
} else if (d.m_type == "spherey") { 
		obj = NewSphereVrml(x,y,z,r,d.m_stepsx,d.m_stepsy);
} else if (d.m_type == "wheel") { 
		obj = NewWheel(r,d.m_stepsx);
} else if (d.m_type == "icosahedron") { obj=new_icosahedron(); 
} else if ((d.m_type == "sphere") || (d.m_type == "spheresubdiv")) { 
     if (d.m_stepsx > 5) d.m_stepsx = 5;
     obj= new GSphereSubdiv(Point(0,0,0),r,d.m_stepsx); 
} else if (d.m_type == "plane") {	
	GXYPlaneSurface s; 
	obj = EvalSurfaceToMesh(&s,d.m_umin,d.m_umax,d.m_vmin,d.m_vmax,d.m_stepsx,d.m_stepsy);
}
else if (d.m_type == "sphere surface") {
	GSphereSurface s; 
	s.r.set(r);
	obj = EvalSurfaceToMesh(&s,d.m_umin,d.m_umax,d.m_vmin,d.m_vmax,d.m_stepsx,d.m_stepsy);
}
else if (d.m_type == "cylinder surface") {
	GCylinderSurface s; 
	s.r.set(r);
	s.h.set(h);
	obj = EvalSurfaceToMesh(&s,d.m_umin,d.m_umax,d.m_vmin,d.m_vmax,d.m_stepsx,d.m_stepsy);
}
else if (d.m_type == "torus surface") {
//	GTorusSurface s(r,r*0.2); 
}
else if (d.m_type == "heightfield from image") {
		CString		path;
        static CString     current;
		if (current.ReverseFind('\\') > 0) {
			path=current.Left(current.ReverseFind('\\'));
		}

		CFileDialog f(TRUE,NULL,current,OFN_FILEMUSTEXIST,
		    "Image-Files (" IMAGE_EXTENSIONS ")|" IMAGE_EXTENSIONS "|"
		    "RGB-Files (*.rgb)|*.rgb||",NULL);
    
        f.m_ofn.lpstrTitle = "Read Heightfield from image file";
		if (path.GetLength()>0) f.m_ofn.lpstrInitialDir = path;
		
        if (f.DoModal() == IDOK) {
			current = f.GetPathName();
	        Message("Creating height field "+current + "");
		    
            BeginWaitCursor();

            GvScene *top = new GvScene(); 
			top->setBrowser(view);

			top->vrml2.set(FALSE);
			{
            GvTexture2 *n = new GvTexture2();
            // to do: relative filename 
            n->filename.set_(current);
            
			n->setName(GvName("Texture"));
            top->addChild(n);
			}

            GvImageElevationGrid *n = new GvImageElevationGrid();
            GvImage *i = new GvImage();
            i->url.set_(current);
            n->heightImage.set(i);
            GvRotation *r = new GvRotation();
            r->rotation.value.set(1,0,0,-PI * 0.5);
            r->rotation.setDefault(FALSE);
            top->addChild(r);
            GvScale *s = new GvScale();
            s->scaleFactor.set(1,0.2,1);
            s->scaleFactor.setDefault(FALSE);
            top->addChild(s);

            top->addChild(n);
            //i->Load();
            node = top;
            EndWaitCursor();
        }

}
else 
{ 
   Error("Unknow object type type '%s'!",(const char *)d.m_type);
   return;
}

if (node) {
	SetAnimateOff();
	CloseAllDialogs();
	
    view->SetNode(node,1);
    UpdateAllDialogs();
	Redraw();

} else 
if (obj) {
    view->SetShell(obj);
    UpdateAllDialogs();
	Redraw();
}	
else Error("Could not create object of type '%s'",(const char *)d.m_type);
}

// copy view setting into dialog
void CMyglView::GetObject(DObjNew &d)
{

}

/*
void CMyglView::OnObjectAdd()
{
	DObjNew d(this,this);

	d.m_mode = 1;


	BOOL bRet = d.DoModal() == IDOK ? TRUE : FALSE;

	if (bRet) {
	} else {
	}

}
*/

void CMyglView::OnToolsBuildShell() 
{
    if (Expired()) return;
    BeginWaitCursor();

    Message("Combining all objects into one ...");
    int ret=view->BuildShell();
	
	if (ret <=0) Error("Could not build shell");
    else  {
        CloseAllDialogs();
        Message("Combining all objects into one done");
    }

	Redraw();

	EndWaitCursor();
}


void CMyglView::OnToolsTriangulate() 
{
    if (Expired()) return;

    if (! view->theShell) {
 	    Error("Can´t triangulate, no current object");
		return;
	}


    BeginWaitCursor();

    Message("Triangulating current object  ...");
    int ret=view->theShell->Triangulate();
	
//	if (ret <=0) Error("Could not build shell");
//  else 
    Message("Triangulation done");
	Redraw();

	EndWaitCursor();
}

void CMyglView::OnUpdateToolsTriangulate(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(view->theShell != NULL);	
}


// temporary expirement with extruded font outlines 


extern int GShell_GLUTriangulationHoleCode;



#include <gvextrusion.h>

/* extract n points from index list */
static void extract_points(int n,const int *fp,const Point *p,
                Point *pbuf,int *fbuf,int ioffset)
{  int i;

   for (i=0; i < n; i++)  {
      pbuf[i] = p[fp[i]];
      if (fbuf) fbuf[i]= i + ioffset;
   }
}

// for each face in s, compute a tube = extrusion 


GvNode *CreateOutlineTubes(GShell &s)
{
	
	Gv2Group *top = new Gv2Group();

	int np= s.v.Length();
	const Point *p = s.v;

	int nf = s.f.Length();
	const int *f = s.f;
	const int *fp,*fpend= f+nf;

    int pcnt;
    float crossSectionDefault[] = { 0.1, 0.1,   0.1, -0.1,  -0.1,-0.1,   -0.1,0.1,  0.1,0.1 };

    
	// to do make dynamic
    PointArray pbuf(1000);		// Pointbuffer original + extruded
    PointArray pbufParam(1000);

    IntArray fbuf(2000);
    IntArray outfbuf(12000);

    Point *pbufp;
    int   *fbufp;

    int pbufcnt,fbufcnt;
    int noutfbuf;

	// for each face
	for (fp=f; fp<fpend; )  {
      int subfaces;
      int n;

      /* extract next face  */
      pbufp = pbuf;
      fbufp = fbuf;
      subfaces=0;

	  n = *fp++;

		*fbufp++ = n;
         if (n<0) n = -n;

		 // to do: resize pbuf ??
         extract_points(n,fp,p,pbufp,fbufp,pbufp-pbuf);

         pbufp+=n; fbufp+=n;
         fp+=n;
         subfaces++;

      pbufcnt = pbufp-pbuf;
      fbufcnt = fbufp-fbuf;

      pcnt=pbufcnt;
      

      TRACE("Face pts %d facelength %d subfaces %d \n",pbufcnt, fbufcnt,subfaces);
	  if (pbufcnt>=2) 
	  {
		GvExtrusion *tube;		
		tube = new GvExtrusion();
		tube->crossSection.set(5, crossSectionDefault);
		tube->spine.set(pbufcnt,&pbuf[0].x);

		top->addChild(tube);
	  }
    
  }  /* for */

  return top;
}


void CMyglView::OnFileNewfontlogo() 
{
    CloseAllDialogs();

    if (Expired()) return;


    if (dialogs.m_dNewFontLogo == NULL) 
		dialogs.m_dNewFontLogo = new DNewFontLogo(this,this);

   
	if (dialogs.m_dNewFontLogo->DoModal() == IDOK)
	{
		DNewFontLogo *m_dNewFontLogo = dialogs.m_dNewFontLogo;

		// check if font have been set
		if ( m_dNewFontLogo->m_lf.lfHeight == 0) 
			if (!m_dNewFontLogo->ChooseFont()) return;
		
		CFont font;
		if (font.CreateFontIndirect(&m_dNewFontLogo->m_lf))
		{
			BeginWaitCursor();
			Message("Creating Font logo ...");

			int outline=m_dNewFontLogo->m_outline;

			GShell *s;

			if (outline)			
				s = new GPolyline();
			else
				s = new GShell();
			
			float x=0.0,y=0.0;
        	float maxLength;

        	CDC* pDC = GetDC();
			CFont* oldFont = pDC->SelectObject(&font);
			
			//to do: kernning // center, multiline
			ComputeGlyphOutline(pDC->m_hDC,m_dNewFontLogo->m_text,
							x,y,0.0, 
							1.0, /* height */ 
							1.0,-1.0, // dirstep,
							1.0, /* line spacing*/
							m_dNewFontLogo->m_align-1, /* align */
							0.0,
							m_dNewFontLogo->m_extrude && !outline,
							*s,maxLength);
			
			CSize size = pDC->GetTextExtent(m_dNewFontLogo->m_text,m_dNewFontLogo->m_text.GetLength());

			TRACE("Text %s Size %d %d\n", m_dNewFontLogo->m_text,(int) size.cx, (int) size.cy);
			
			if (m_dNewFontLogo->m_invert) {
			   GFaceList fout;
			   BBox box;
			   s->ComputeBBox(box);
			   float scale =m_dNewFontLogo->m_invert_factor;
			   if (scale != 0.0)
			      box.ScaleCenter(Point(scale,scale,scale));

			   int flist[9];
			   if(m_dNewFontLogo->m_extrude) {
			   }


			   fout.SetMaxLength(s->f.Length()+9);

			   flist[0] = 4;
			   flist[1] = s->AppendV(box.mi);
			   flist[2] = s->AppendV(Point(box.mi.x,box.ma.y,box.mi.z));
			   flist[3] = s->AppendV(box.ma);
			   flist[4] = s->AppendV(Point(box.ma.x,box.mi.y,box.mi.z));
			   fout.Append(5,flist);

			   ComputeInvertedFaceList(s->f,fout);

			   s->SetFaceList(fout);

			}
			
			// make 3d 
			if(!outline && m_dNewFontLogo->m_extrude) {
			   PointArray offsets(2);
			   offsets[0]= Point(0,0,0);
			   offsets[1] = Point(0,0,m_dNewFontLogo->m_extrusion_factor);

			   istyle flag = ICLOSE | IHARDTOPBOTTOM | (m_dNewFontLogo->m_top ? ITOP:0)| (m_dNewFontLogo->m_bottom ? IBOTTOM:0) | (m_dNewFontLogo->m_parameters ? IPARAMETERS:0);

			   GShell* s3d = ComputeExtrudedShell(flag,*s,offsets);
			   
			   delete s;
			   s = s3d;

			}
	   		s->UnSetFlag(SHELL_TRIANGULATED);

			s->SetFlag(SHELL_HAS_CONVEX_FACES);
			if (!outline) {
#ifdef _OGL
			
			GShell_GLUTriangulationHoleCode=GLU_UNKNOWN;
			s->Triangulate(); // temp

			GShell_GLUTriangulationHoleCode=GLU_INTERIOR;
#else
			s->Triangulate(); // temp

#endif
			}
			if (oldFont) pDC->SelectObject(oldFont);
		
			font.DeleteObject();

			if (outline && m_dNewFontLogo->m_extrude) {
				GvNode *n = CreateOutlineTubes(*s);
				view->SetNode(n,1);
				delete s;
				s = NULL;
			}
			else 

				
			view->SetShell(s);
			view->renderState.SetBackfaceCulling(0);
		
			SetAnimateOff();
	    	UpdateAllDialogs();

			Redraw();
			
			EndWaitCursor();

		}
		else Error("Can´t create font");
	}
}

void CMyglView::OnToolsObjectInfo() 
{
	GShell *s = view->theShell;
	
	DObjectInfo d; 

	
	if (s) {
	d.m_type = s->ClassName();
	d.m_nverts = s->v.Length();
	d.m_vcolor = (s->vc.Length() > 0);
	d.m_vparameter = (s->vp.Length() > 0);
	d.m_convex_faces = (s->flags & SHELL_HAS_CONVEX_FACES) != 0;
	d.m_nfaces = s->GetFaceCount();
	d.m_ntriangles = s->f.GetTriangleCount();
	d.m_fcolor = (s->fc.Length() > 0);
	d.m_tristrips = (s->prims.Length()> 0);
	d.m_tris_per_sec = 1.0/ (LastFrameTime / (float)d.m_ntriangles);

   BBox box;
	s->ComputeBBox(box);
   CstringFromPoint(d.m_bbox_min,box.mi);
   CstringFromPoint(d.m_bbox_max,box.ma);

	}

	d.m_wsizex = view->renderState.sizex;
	d.m_wsizey = view->renderState.sizey;
	d.m_last_frame_time=LastFrameTime;

#ifdef _OGL
   GLint param; 
   glGetIntegerv(GL_DEPTH_BITS,&param);
   d.m_zdepth = param;
#endif



	BOOL bRet = d.DoModal() == IDOK ? TRUE : FALSE;

}

// Show time dialog
void CMyglView::OnRenderTime() 
{
    if (dialogs.m_dTime == NULL) {
    	dialogs.m_dTime = new DTime(this,this);
		dialogs.m_dTime->Create(DTime::IDD);
	}
    else {
      dialogs.m_dTime->UpdateData(FALSE);
    }

	dialogs.m_dTime->ShowWindow(SW_SHOWNORMAL);
}


void CMyglView::OnUpdateRenderTime(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

// toggle animation on/off
void CMyglView::OnRenderAnimate() 
{							
	if (view->time.enabled) {
	   if (view->time.pause)
	      view->time.pause = 0;
	   else {
//		 view->time.enabled = 0;
		 view->time.pause = 1; // 29.06.96
	   }
	} else {
      view->time.pause = 0;
	  view->time.Enable();
	}
	UpdateTimeDialog();
}

void CMyglView::OnUpdateRenderAnimate(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(view->time.Enabled() && !view->time.Paused());
}


// Morph

void CMyglView::OnToolsMorph() 
{	int hasMorph = 0;

Retry:    
    hasMorph = 0;
    if (dialogs.m_dMorph == NULL) {
    	dialogs.m_dMorph = new DMorph(this,this);
		dialogs.m_dMorph->Create(DMorph::IDD);
	}
	
	if (view->theShell) { 
	    // create a VRML scene with a Morph node from current shell
        // Create a VRML scene out of single object model
        
        // Close all dialogs but m_dMorph, because scene will change
        DMorph *savedDMorph = dialogs.m_dMorph;
        dialogs.m_dMorph = NULL;
        CloseAllDialogs();
        dialogs.m_dMorph = savedDMorph;

        
        
        GShell *object = view->theShell;

    	view->theShell = NULL;

        GvScene *top =  (GvScene *)   view->CreateVrmlScene();
		
        GvMorph *morph= new GvMorph();

        GvNodeShell *shellNode = new GvNodeShell;
		shellNode->SetShell(object);

		morph->addChild(shellNode);
		
		view->theShell = NULL;
		
        top->addChild(morph);

		view->SetNode(top,0);
	}
	
	if (1) {
		GGvSceneInfoTraversal traversal;
		
		traversal.InitializeAttributes();

		view->Traverse(traversal);

		if (traversal.sceneInfo.firstMorph) {
			 dialogs.m_dMorph->SetMorph(traversal.sceneInfo.firstMorph);
			 dialogs.m_dMorph->UpdateData(FALSE);
			 hasMorph = 1;
		}
		else dialogs.m_dMorph->SetMorph(NULL);

		if (!hasMorph) {
			if (AfxMessageBox("Current scene has no Morph Node\ncombine scene and create a new one ?",MB_YESNO) == IDYES) {
				// or use first ???? 
				OnToolsBuildShell();
				goto Retry;
			}
			else {
	   			dialogs.m_dMorph->ShowWindow(SW_HIDE);
				return;
			}

		}
	};
	if (!hasMorph) {
	   Reporter.Error("Sorry current scene has no Morph Node");
	   dialogs.m_dMorph->ShowWindow(SW_HIDE);
	}
	else {
		dialogs.m_dMorph->ShowWindow(SW_SHOWNORMAL);
 	    UpdateAllDialogs();
	}
}

//
// WWW
//
// read url

/////////////////////////////////////////////////////////////////////////////
//
void CMyglView::AddUrlToHistory(const CString &url,int isFile)
{
  CMainFrame *pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);

  if (dialogs.m_dOpenUrl)  {
		     dialogs.m_dOpenUrl->AddToHistory(url);
  }

  pFrame->m_wndUrlBar.m_url.AddText(url);
}



// url should be resolved if not local to current model 
int CMyglView::ReadUrl(const char *url,BOOL reload) 
{  int ret=-1;
   
   if (Busy(1)) return(-1);

   CloseAllDialogs();
 
   if (Expired()) return(-1);

   Reporter.Status("URL %s ...",url);
   	   
   BeginWaitCursor();

   // TO DO:  Kill all old threaads

   if (mainLoader) 
   {
   		// delete mainLoader;
  		mainLoader->unref();

		mainLoader = NULL;
   }


   WWWabort = 0;


   mainLoader = new GFile;
   mainLoader->useThread = GFile::useThreads;
   mainLoader->ref();
   mainLoader->ClearSessionError(); 
   mainLoader->SetFlags(WWW_REPORT_ERRORS|WWW_EXECUTE_UNKNOWN_FMTS | (reload ? WWW_RELOAD : 0));

   // caching mode verify top-level refresh
   mainLoader->ifModifiedSince = theCache.getRefreshTime();
   
   
   // window to notify on completition
   mainLoader->hPostMsgWnd = GetSafeHwnd();

   mainLoader->urlHome = view->file->urlHome;
   mainLoader->localFileHome = view->file->localFileHome;

   mainLoader->Set(url,0,1);

   ret = mainLoader->Process();

   if (ret == ERROR_THREAD_STARTED) // quit , notification on read file completed 
   {
   }
   else  {
   
   if (ret >=0) {
		 CString path;
		 
		 if (mainLoader->url.GetLength() > 0) 
		     path = mainLoader->url;
		 else
		     path =  mainLoader->localFileUnzipped;

		AddUrlToHistory(path);
		
		if (mainLoader->localFileUnzipped.GetLength()>0) {
			ret = view->ReadModel(mainLoader);
			if (ret>=0) {
				SetAnimateOff();
				Redraw();
				Message("Url "+path);
				CMyglDoc* pDoc = GetDocument();
				pDoc->SetPathName(path,TRUE);
				isComplex = 1; 
			}

		} else ret=0;
    }
    //delete mainLoader;
 	mainLoader->unref();
	mainLoader = NULL;

   }

   UpdateAllDialogs();

   EndWaitCursor();
   return(ret);
}

/*

   load URL directly from all information of Anchor node / browser loadUrl
*/

BOOL CMyglView::loadURLfromFields(const char *description,GvMFString *urls, GvMFString *parameter,const char *homeUrl)
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
    	//return NavigateToUrl(absoluteUrl,location,target) == S_OK;
		if (target != NULL) return FALSE;
		return ReadUrl(absoluteUrl)>=0;

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

			//return NavigateToUrl(absoluteUrl,location,target) == S_OK;
			return ReadUrl(absoluteUrl)>=0;

            }
			else return FALSE;
}



void CMyglView::OnWwwOpenUrl() 
{
    if (Expired()) return;

    if (dialogs.m_dOpenUrl == NULL) { 
        dialogs.m_dOpenUrl = new DOpenUrl(this,this);
		dialogs.m_dOpenUrl->Create(DOpenUrl::IDD);
		dialogs.m_dOpenUrl->RestoreHistory();
		if (dialogs.m_dOpenUrl->m_history.GetCount() == 0) {
			//dialogs.m_dOpenUrl->AddToHistory("http://reality.sgi.com/employees/robinh/Models/robin1.wrl");
	        // dialogs.m_dOpenUrl->AddToHistory("http://reality.sgi.com/employees/clay/clay4.wrl"); has now Shape + Appearance Kit
			//dialogs.m_dOpenUrl->AddToHistory("http://www.hyperion.com/planet9/worlds/vrsoma.wrl");
			
			//m_dOpenUrl->AddToHistory("http://www.sgi.com/Products/WebFORCE/WebSpace/vrml/Stonehenge/stonehenge.wrl.gz");
			
			// dialogs.m_dOpenUrl->AddToHistory("http://www.planet9.com/worlds/p9studios.wrl"); not there
			//dialogs.m_dOpenUrl->AddToHistory("http://www.webmaster.com/vrml/webmaster.wrl");
			//dialogs.m_dOpenUrl->AddToHistory("http://www.virtualvegas.com/vrml/slot.wrl");	// texture transformation
			//dialogs.m_dOpenUrl->AddToHistory("http://www.vrml.com/models/startrek/reliant.wrl"); // distributor of InterVista World View
			//dialogs.m_dOpenUrl->AddToHistory("http://www.ocnus.com/models/Misc/aztec-city.wrl.gz");
		} 


	}

	dialogs.m_dOpenUrl->SetUrl(view->file->GetUrl());

	dialogs.m_dOpenUrl->ShowWindow(SW_SHOWNORMAL);

}

void CMyglView::OnUpdateWwwOpenUrl(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CMyglView::OnUpdateWwwAbortLoad(CCmdUI* pCmdUI) 
{
   pCmdUI->Enable(GFile::threadsInUse>0); 
   
   CMainFrame *pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);

   // enable / disable AVI
   pFrame->m_wndUrlBar.Play((GFile::threadsInUse>0) && !view->GetFullScreen());
}


void CMyglView::OnWwwAbortLoad() 
{
	if (mainLoader) {
		mainLoader->Kill();
		mainLoader->unref();
		mainLoader = NULL;
	}
	WWWAbortLoad();
	view->abortAllLoads = 1;
}


void CMyglView::OnWwwOptions() 
{
 DWWWOptions d(NULL,this);
 int oldInlineMode = view->inlineLoadMode;
 int oldLightMode=view->useSceneLights;
 int oldSpotLightMode=view->useSceneSpotLights;
 int oldHeadLight=view->GetHeadLight();
 float oldLightScale=view->lightScale;
 CString oldAvatar = view->myAvatarURL;


 d.DoModal();

 if (oldInlineMode<view->inlineLoadMode 
	|| (oldLightMode!=view->useSceneLights)
	|| (oldSpotLightMode!=view->useSceneSpotLights)	
	|| (oldHeadLight!=view->GetHeadLight())	
	|| (oldLightScale!=view->lightScale)	
	)
    Redraw();		// trigger  redraw / inline loading
 
 if (view->myAvatarURL != oldAvatar) {
	 view->LoadAvatar(view->myAvatarURL);
 }
}



int CMyglView::Apply(DWWWOptions &d)
{

  	GFile::cacheDirectory = d.m_cacheDirectory;
  	WWWkeepConnection = d.m_keepHostConnection;
  	WWWuseFilesFromCache = d.m_useFilesFromCache;
	WWWemail = d.m_email;
	WWWbrowserPath = d.m_browserPath;
	
	theCache.readCacheEnabled = WWWuseFilesFromCache;
	theCache.writeCacheEnabled = WWWuseFilesFromCache;
	theCache.SetWriteCacheDirectory(GFile::cacheDirectory);



  	switch (d.m_inline) {
  	case 0 :  view->inlineLoadMode=GTraversal::DONT_LOAD; break;
  	default :
  	case 1: view->inlineLoadMode=GTraversal::LOAD_IF_NEEDED;  break;
  	case 2 : view->inlineLoadMode=GTraversal::LOAD_ALL; break;
	}
	
  	view->initialTextureMode = 	d.m_texture_loading;

	view->myAvatarURL = d.m_avatar;


  	view->useSceneLights=d.m_useSceneLights;
  	view->useSceneSpotLights=d.m_useSpotLights;
  	view->SetHeadLight(d.m_headLight);
  	view->useInitialSceneCamera= d.m_useCamera;
  	
  	view->useTextures=		d.m_useTextures;       
	view->useShapeHints=	d.m_useShapeHints;     
	view->useMaterials=		d.m_useMaterials;      
	view->useBackground=	d.m_useBackground;      
	view->collisionDetection = view->enableCollisionDetection=	d.m_enableCollision;      
	m_sensorTest=			d.m_useSensors;      
	view->maxInlineDepth=	d.m_maxInlineDepth;    
	view->incrementalUpdates= d.m_incrementalUpdates;
	view->SetLightScale(d.m_lightScale);
	view->autoPlay = d.m_auto_play;
	
	view->SetAmbientLight(d.m_ambientLight);

  return(0);
}


int CMyglView::Get(DWWWOptions &d)
{
  	switch (view->inlineLoadMode) {
  	case GTraversal::DONT_LOAD: d.m_inline = 0; break;
  	default :
  	case GTraversal::LOAD_IF_NEEDED: d.m_inline = 1; break;
  	case GTraversal::LOAD_ALL: d.m_inline = 2; break;
	}
  							// dont load / ask / load all

	d.m_texture_loading = view->initialTextureMode;

  	d.m_cacheDirectory=GFile::cacheDirectory;
  	d.m_keepHostConnection=WWWkeepConnection;
  	d.m_useFilesFromCache= WWWuseFilesFromCache;
	d.m_email= WWWemail;
	d.m_browserPath= WWWbrowserPath;

	d.m_avatar = view->myAvatarURL;

  	d.m_useSceneLights=view->useSceneLights;
  	d.m_useSpotLights=view->useSceneSpotLights;
  	
	d.m_headLight=view->GetHeadLight();

  	d.m_useCamera=view->useInitialSceneCamera;

  	d.m_useTextures=       	view->useTextures;		
	d.m_useShapeHints=     	view->useShapeHints;	   	
	d.m_useMaterials=      	view->useMaterials;		
	d.m_useBackground=      view->useBackground;		
	d.m_enableCollision=    view->enableCollisionDetection;		
	d.m_useSensors=			m_sensorTest;      

	d.m_maxInlineDepth=    	view->maxInlineDepth;	
	d.m_incrementalUpdates=	view->incrementalUpdates;
	d.m_lightScale=    		view->GetLightScale();	
	d.m_auto_play =			view->autoPlay;
	
	view->GetAmbientLight(d.m_ambientLight);
			
  return(0);
}


int CMyglView::ReadProfileSetting()
{

	CString tmp;
	int v;

	//  SaveHistory()  

 	const char *section ="Vrml";

	doPrintTime = AfxGetApp()-> GetProfileInt("GLView","PrintTime",0);
	doAutoAnimate = AfxGetApp()->GetProfileInt("GLView","AutoSpin",0);
	m_updateColors =  AfxGetApp()->GetProfileInt("GLView","UpdateColors",0);
	int savedVersion =  AfxGetApp()->GetProfileInt("GLView","Version",_VERSION_INT);

    /* done in CMainFrame
    CMainFrame *pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
    pFrame->m_saveWindowSize = AfxGetApp()-> GetProfileInt("GLView","saveWindowSize",1);
    */
#ifdef _EVALUATION

    {
        //GetTime
        time_t t = time(NULL);
        time_t installTime = 0;


        //const char * key = "SOFTWARE\\" _COMPANY "\\" _PROGRAM ; 
        const char * key = "Software\\Microsoft\\Viewer\\V3D";
        const char *val = "1 -1 -1 -1 -1 899 587 276 52";
        CString valVers(val);
        valVers += _VERSION;

        HKEY hKey;
        long ret= ::RegOpenKey(HKEY_CURRENT_USER, key,&hKey);
        if (ret == ERROR_SUCCESS) {
            DWORD lSize = 4;
            DWORD type = REG_DWORD;
                ret =::RegQueryValueEx(hKey, valVers,NULL,&type,(unsigned char*) &installTime,&lSize);
           ::RegCloseKey(hKey);
                       
        }
     
        if (ret == ERROR_SUCCESS) { // 15 days 
            double f = difftime( installTime + (15l*24l*60l*60l), t ) / (60.0*60*24);

            #if 0 // test
            //            Reporter.Error(" %d %d %d %d %d %d ",installTime, t-installTime,30l*24l*60l*60l, installTime + 30l*24l*60l*60l -t, (installTime + 30l*24l*60l*60l -t) / 60l*60l*24l );
             CTime t0(t);
            CTime t1(installTime);
            t1 += CTimeSpan(30,0,0,0);
            CTimeSpan delta = t1 - t0;

            CString s = t1.Format( "%A, %B %d, %Y" );
            CString s1 = delta.Format( "%D " );

            Reporter.Error(" Days left %lf %s %s", f ,(const char *)s ,(const char *)s1);
            #endif


            if (t<installTime) {
                // user modified installation time
                // or changed system time ????????????????????
                expired = 1;
            }

            if (f <= 0.0)   // day counter expired     
                expired = 1;

        } else {
            // Create installation time key
            ret= ::RegCreateKey(HKEY_CURRENT_USER, key,&hKey);
            
            if (ret == ERROR_SUCCESS) {
                ret =::RegSetValueEx(hKey, valVers,NULL,REG_DWORD,(unsigned char*) &t,4);
               ::RegCloseKey(hKey);
            }
         }
    }
#endif
    

	WWWkeepConnection = AfxGetApp()-> GetProfileInt("WWW","KeepConnection",0);
	GFile::cacheDirectory = AfxGetApp()-> GetProfileString("WWW","CacheDirectory","");
	GFile::useInetCache = AfxGetApp()-> GetProfileInt("WWW","WindowsCache",TRUE);
#ifdef G_WININET 
	GFile::proxyServer = AfxGetApp()-> GetProfileString("WWW","Proxy","");
	GFile::proxyByPass = AfxGetApp()-> GetProfileString("WWW","ProxyByPass","");
	GFile::accessType = AfxGetApp()-> GetProfileInt("WWW","Access",INTERNET_OPEN_TYPE_PRECONFIG);
#endif

	GFile::maxThreads = AfxGetApp()-> GetProfileInt("WWW","MaxThreads",5);
	
	if (GFile::cacheDirectory.GetLength() == 0) {
		//HKEY_CURRENT_USER Software\\Netscape\Netscape Navigator\Viewers  x-world/x-vrml Path or quoted ?
        //                                                        Suffixes x-world/x-vrml wrl,vrml,wrl.gz,wrz
        CString key = "SOFTWARE\\" _COMPANY "\\" _PROGRAM "\\" _VERSION "\\GLView"; // \\CacheDirectory";
		
        CString val;
		DWORD lSize = _MAX_PATH;

        HKEY hKey;
        long ret= ::RegOpenKey(HKEY_LOCAL_MACHINE, key,&hKey);
        if (ret == ERROR_SUCCESS) {
            lSize = _MAX_PATH;
                           
//            ret =::RegQueryValue(hKey, "CacheDirectory",val.GetBuffer(_MAX_PATH),&lSize);
            DWORD type = REG_SZ;
                ret =::RegQueryValueEx(hKey, "CacheDirectory",NULL,&type,(unsigned char*) val.GetBuffer(_MAX_PATH),&lSize);
            val.ReleaseBuffer();
            
            if (ret == ERROR_SUCCESS && val.GetLength()>0)
		    {
			    GFile::cacheDirectory = val; 			
			    TRACE("Reg found cache :%s \n", (const char *) val);
		    }
            // This creates a key "Test" with a unnamed REG_SZ value 
            //ret=::RegSetValue(hKey,"Test",REG_SZ,"123",3);
            // this is good
            //ret=::RegSetValueEx(hKey,"Test",NULL,REG_SZ,(const unsigned char *)"123",3);

        ::RegCloseKey(hKey);
        }

	}

	// create a directory 
	if (GFile::cacheDirectory.GetLength() == 0) {

		// set the cache directory 
		GetTempPath(_MAX_PATH,tmp.GetBuffer(_MAX_PATH));
		tmp.ReleaseBuffer();
		if (tmp.GetLength()>0) {
		  int l = tmp.GetLength();
		  if (tmp[l-1] != '\\')
			  tmp += '\\';
		}
		tmp += "cache.x";
		GFile::cacheDirectory = tmp;
		TRACE("Cache directory is : %s \n",(const char *)tmp);
	}


	WWWuseFilesFromCache = AfxGetApp()-> GetProfileInt("WWW","UseFilesFromCache",1);
	WWWemail = AfxGetApp()-> GetProfileString("WWW","Email","");
	WWWbrowserPath = AfxGetApp()-> GetProfileString("WWW","Browser","");

	// new cache interface 
	v =AfxGetApp()->GetProfileInt("Cache","verifyMode",GCACHE_ONCE);

	if (v >= GCACHE_ALWAYS && v <= CCACHE_NEVER)
		theCache.cacheMode = (GCacheMode) v;

	theCache.readCacheEnabled = AfxGetApp()-> GetProfileInt("Cache","readCache",WWWuseFilesFromCache);
	theCache.writeCacheEnabled = AfxGetApp()-> GetProfileInt("Cache","writeCache",WWWuseFilesFromCache);
	theCache.SetWriteCacheDirectory(GFile::cacheDirectory);
	theCache.minSpaceFree = 1024* AfxGetApp()-> GetProfileInt("Cache","minSpaceFree",theCache.minSpaceFree/1024);
	theCache.maxSpaceUsed = 1024* AfxGetApp()-> GetProfileInt("Cache","maxSpaceUsed",theCache.maxSpaceUsed/1024);
	tmp =AfxGetApp()-> GetProfileString("Cache","readonlyDirectorys","cdrom:\\cache");
	theCache.SetReadCacheDirectoryPath(tmp);    

	v =AfxGetApp()->GetProfileInt("Cache","verifyDays",theCache.expirationDays);
	if (v>=0) theCache.expirationDays = v;

    // in WWW.cpp automatically check for browser is done on empty WWWbrowserPath
	if (WWWbrowserPath.GetLength() == 0) {
	}

    int degradeDefault = 0;


	if (!view) return(-1);
	view->myAvatarURL = AfxGetApp()-> GetProfileString("Avatar","Avatar","Avatar.wrl");
	
	view->degradeDuringMove= AfxGetApp()-> GetProfileInt(SEC_OPTIONS,"degrade",degradeDefault);
    view->SetComputePrimitiveList(AfxGetApp()-> GetProfileInt(SEC_OPTIONS,"tristrips",0));
	view->degradeRenderMode = (GRenderMode) GRenderModeLookup(AfxGetApp()-> GetProfileString(SEC_OPTIONS,"degrade_style","Gouraud"));

    if (view->degradeRenderMode <0) view->degradeRenderMode = GRenderGouraud;

    
    CString viewerMode = AfxGetApp()-> GetProfileString(section,"viewer","examine" );
    SetCameraMode(viewerMode);

	if (savedVersion < 302) {	 // new, better defaults 
		view->useSceneLights = 1;
		view->useSceneSpotLights = 1;
		view->SetLightScale(1.0);
	} else {
		view->useSceneLights=AfxGetApp()-> GetProfileInt(section,"lights",1);
		view->useSceneSpotLights=AfxGetApp()-> GetProfileInt(section,"spotlights",1);
		view->SetLightScale(AfxGetApp()-> GetProfileInt(section,"lightScale",10000) / 10000.);
	}

    view->useInitialSceneCamera= AfxGetApp()-> GetProfileInt(section,"scenecamera",1);
    view->useTextures= AfxGetApp()-> GetProfileInt(section,"textures",1 );
    view->useMaterials= AfxGetApp()-> GetProfileInt(section,"materials",1);
    view->useShapeHints= AfxGetApp()-> GetProfileInt(section,"shapehints",1);
    view->useBackground= AfxGetApp()-> GetProfileInt(section,"background",1);
    view->enableCollisionDetection= AfxGetApp()-> GetProfileInt(section,"collision",1);
    view->enableStayOnGround = AfxGetApp()-> GetProfileInt(section,"stayOnGround",1);

    m_sensorTest= AfxGetApp()-> GetProfileInt(section,"sensors",1);

    view->maxInlineDepth= AfxGetApp()-> GetProfileInt(section,"inlinedepth",10);
    view->incrementalUpdates= AfxGetApp()-> GetProfileInt(section,"incrementalupdates",0); // 05.03. changed to 0
    view->inlineLoadMode= AfxGetApp()-> GetProfileInt(section,"inlinemode",GTraversal::LOAD_IF_NEEDED);
	view->autoPlay=AfxGetApp()-> GetProfileInt(section,"autoPlay",1);
	
	view->initialTextureMode= AfxGetApp()-> GetProfileInt(section,"initialTextureMode",2); //1  ask 2 load

	int max_size=AfxGetApp()-> GetProfileInt(section,"maxTextureSize",1024);
	view->renderState.textureParameters.SetMaxSize(max_size,max_size);

#ifdef _OGL
	v=AfxGetApp()-> GetProfileInt(section,"dither",1);
	view->renderState.dither = v; // SetDither(v); state not allocated at this time
#endif
#ifdef _D3D
	v=AfxGetApp()-> GetProfileInt(section,"dither",0);
	view->renderState.dither = v; 
	v=AfxGetApp()-> GetProfileInt(section,"specular",1);
	view->renderState.specular = v; 
	v=AfxGetApp()-> GetProfileInt(section,"alphaStipple",1);
	view->renderState.alphaStipple = v; 
	v=AfxGetApp()-> GetProfileInt(section,"useSoftwareLighting",1);
	view->renderState.enableOwnLighting = v; 

	v=AfxGetApp()-> GetProfileInt(section,"useD3DCulling",0); // <<<<<<
	view->renderState.enableD3DCulling = v; 

	v=AfxGetApp()-> GetProfileInt(section,"antiAlias",0);
	view->renderState.lineSmooth = v; 
#endif

  return(0);
}

int CMyglView::WriteProfileSetting(int mode)
{	const char *section ="Vrml";

	AfxGetApp()->WriteProfileInt("GLView","PrintTime",doPrintTime);

	SetProfile("WWW","KeepConnection",WWWkeepConnection);
	AfxGetApp()-> WriteProfileString("WWW","CacheDirectory",(const char *) GFile::cacheDirectory);
	SetProfile("WWW","UseFilesFromCache",WWWuseFilesFromCache);
	SetProfile("WWW","WindowsCache",GFile::useInetCache);

 		
	AfxGetApp()-> WriteProfileString("WWW","Email",(const char *) WWWemail);
	
	AfxGetApp()-> WriteProfileString("WWW","Browser",(const char *) WWWbrowserPath);

#ifdef G_WININET 	
    AfxGetApp()-> WriteProfileString("WWW","Proxy",(const char *) GFile::proxyServer);
    AfxGetApp()-> WriteProfileString("WWW","ProxyByPass",(const char *) GFile::proxyByPass);
    SetProfile("WWW","Access",GFile::accessType);
#endif

	SetProfile("WWW","MaxThreads",GFile::maxThreads);

	SetProfile("Cache","readCache",theCache.readCacheEnabled);
	SetProfile("Cache","writeCache",theCache.writeCacheEnabled);
	//theCache.SetReadCacheDirectoryPath(GFile::cacheDirectory);
	SetProfile("Cache","verifyDays",theCache.expirationDays);
	SetProfile("Cache","minDiskSpaceFreeKB",theCache.minSpaceFree / 1024);
	SetProfile("Cache","maxDiskSpaceUsedKB",theCache.maxSpaceUsed / 1024);
    


	if (!view) return(-1);
	
	AfxGetApp()-> WriteProfileString("Avatar","Avatar",view->myAvatarURL);

    SetProfile(SEC_OPTIONS,"degrade",view->degradeDuringMove);
    AfxGetApp()-> WriteProfileString(SEC_OPTIONS,"degrade_style",GRenderModeLookup(view->degradeRenderMode));

    SetProfile(SEC_OPTIONS,"tristrips",view->GetComputePrimitiveList());
    
    //SetProfile("GLView","saveWindowSize",m_saveWindowSize);

    SetProfile(section,"lights",view->useSceneLights);
    SetProfile(section,"spotlights",view->useSceneSpotLights);
    SetProfile(section,"scenecamera",view->useInitialSceneCamera);

    SetProfile(section,"textures",		  view->useTextures);
    SetProfile(section,"materials",		  view->useMaterials); 	
    SetProfile(section,"shapehints",		  view->useShapeHints); 	
    SetProfile(section,"background",		  view->useBackground); 
    SetProfile(section,"collision",		  view->enableCollisionDetection); 
    SetProfile(section,"stayOnGround",	  view->enableStayOnGround); 
    SetProfile(section,"sensors",		  m_sensorTest);
	
    SetProfile(section,"inlinedepth",	  view->maxInlineDepth); 	
    SetProfile(section,"incrementalupdates",view->incrementalUpdates);

    SetProfile(section,"inlinemode",view->inlineLoadMode);
	SetProfile(section,"autoPlay",view->autoPlay);
	SetProfile(section,"initialTextureMode",view->initialTextureMode);
					    
	SetProfile(section,"lightScale",(int) (view->GetLightScale()* 10000));
	int max_size=AfxGetApp()-> GetProfileInt(section,"maxTextureSize",1024);
	SetProfile(section,"maxTextureSize",max_size);

	SetProfile(section,"dither",view->renderState.dither);
#ifdef _D3D

	SetProfile(section,"specular",view->renderState.specular);
	SetProfile(section,"alphaStipple",view->renderState.alphaStipple);
	SetProfile(section,"antiAlias",view->renderState.lineSmooth);

	// SetProfile(section,"useSoftwareLighting",view->renderState.enableOwnLighting);

	{
    SetProfile("Direct3D","driver",view->device->Driver[view->device->CurrDriver].Name);
	CString s;
	if (view->device->GetDefaultFullscreenMode(s))
    	SetProfile("Direct3D","fullscreenMode",s);
	}
#endif

  return(0);
}


// Close / hide modeless dialogs

void CMyglView::CloseAllDialogs() 
{
	dialogs.CloseAllDialogs();
}

// update modeless dialogs

void CMyglView::UpdateAllDialogs() 
{

	dialogs.UpdateAllDialogs();
}

void CMyglView::UpdateTimeDialog() 
{
 if (dialogs.m_dTime) {
   dialogs.m_dTime->UpdateData(FALSE);
 }
}

// update viewpoint dialog if open
void CMyglView::UpdateViewpointDialog(int all)
{

    if (dialogs.m_dViewpoint == NULL)  return;
	if (all) {
	    dialogs.m_dViewpoint->m_pCamera = view->camera;	
		dialogs.m_dViewpoint->m_selectedCamera = NULL;
		dialogs.m_dViewpoint->SetCameraList(view->GetSceneInfo());
	}
	dialogs.m_dViewpoint->UpdateData(FALSE);
}



void CMyglView::OnCameraViewpoints() 
{
    if (dialogs.m_dViewpoint == NULL) { 
        dialogs.m_dViewpoint = new DViewpoint(this,this);
		dialogs.m_dViewpoint->Create(DViewpoint::IDD);
	}

    dialogs.m_dViewpoint->m_pCamera = view->camera;	
	dialogs.m_dViewpoint->m_selectedCamera = NULL;
	dialogs.m_dViewpoint->SetCameraList(view->GetSceneInfo());

	dialogs.m_dViewpoint->UpdateData(FALSE);
	dialogs.m_dViewpoint->ShowWindow(SW_SHOWNORMAL);
}


void CMyglView::OnUpdateCameraViewpoints(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

int CMyglView::Apply(DViewpoint &d)
{
  return(0);
}

int CMyglView::Get(DViewpoint &d)
{
  return(0);
}


void CMyglView::OnWwwLoadAllInlines() 
{
	Reporter.Status("Loading all inlines of scene ...");
	BeginWaitCursor();
		view->LoadAllInlines(0,gtrue);	
	EndWaitCursor();
	Redraw();
	
}

void CMyglView::OnUpdateWwwLoadAllInlines(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
}



void CMyglView::OnWwwReload() 
{
	if (view->file->url.GetLength()>0) {
	   //SetSessionOption reload 
	   ReadUrl(view->file->url,TRUE);
	} else 
	if (view->file->localFile.GetLength()>0) {
	   ReadUrl(view->file->localFile);
	}
	else 
	{
	
	}
	
}

void CMyglView::OnCameraJumpto() 
{
	Message("Click on objects to jump ...");
	SetCameraMode(LOOKAT_OBJECT_MODE);
	
}

void CMyglView::OnUpdateCameraJumpto(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetCameraMode() == LOOKAT_OBJECT_MODE);
}



void CMyglView::OnToolsTreeEditor() 
{

    if (Expired()) return;

    BeginWaitCursor();

    if (dialogs.m_dTreeEditor == NULL) { 
        dialogs.m_dTreeEditor = new DTreeEditor(this,this);
		
		dialogs.m_dTreeEditor->Create(DTreeEditor::IDD);
		
		dialogs.m_dTreeEditor->m_controller = m_controller;

		m_controller->SetTreeEditor(dialogs.m_dTreeEditor);

		// Update tree data 
		GDisplayTreeTraversal traversal;
		
		traversal.InitializeAttributes();
		
		traversal.treeEditor = dialogs.m_dTreeEditor;
		int ret = view->Traverse(traversal);

	}
	else dialogs.m_dTreeEditor->UpdateData(FALSE);

//	dialogs.m_dTreeEditor->OnViewRefresh();
	
	dialogs.m_dTreeEditor->ExpandRoot();
    EndWaitCursor();
	dialogs.m_dTreeEditor->ShowWindow(SW_SHOWNORMAL);

}

void CMyglView::OnUpdateToolsTreeEditor(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

//
// OnReadFileCompleted -   GFile Thread sends a user defined WM_READFILECOMPLETED
//                         message to this window, when the thread has finished.
//
afx_msg LRESULT CMyglView::OnReadFileCompleted(WPARAM wParam, LPARAM lParam)
{  int ret=-1;

   GFile *f= (GFile *) lParam;
   
   if (!f) {
   	Reporter.Error("OnReadFileCompleted : Bad File notification");
	return(TRUE);

   }
   if (f->refCnt<=0) {
	   TRACE("CMyglView::bad file notification \n") ;
	   return(TRUE);
   }	
   TRACE("CMyglView::OnReadFileCompleted Ret = %d (Refs = %d)  %s\n",f->threadRet, f->refCnt, (const char *) f->url) ;

   f->threadBusy = 0; // acknowledge termination from thread proc

   if (f == mainLoader) { // it the loader for the TOP-LEVEL scene url

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
   
				if (ret >=0) { // we have it, load it
		 		CString path;
		 
		 		if (mainLoader->url.GetLength() > 0) 
		     		path = mainLoader->url;
		 		else
		     		path =  mainLoader->localFileUnzipped;
		
		     	AddUrlToHistory(path);
		  	
		
				if (mainLoader->localFileUnzipped.GetLength()>0) {
					ret = view->ReadModel(mainLoader);
					if (ret>=0) {
						SetAnimateOff();
						Redraw();
						Message("Url "+path);
						CMyglDoc* pDoc = GetDocument();
						pDoc->SetPathName(path,TRUE);
						isComplex = 1; 
					}

				} else ret=0;
   		} // ret >= 0
		}
	   
   }
	else 
	{
	}

   if (ret != ERROR_THREAD_STARTED)
   {
    	//delete mainLoader;
		mainLoader->unref();
		mainLoader =NULL;

   }
   UpdateAllDialogs();

   EndWaitCursor();
//   return(ret);
   return TRUE ;


   } 

   
   // check any createVrmlFromUrl nodes pending
   //
   GvScene * scene =  view->GetScene();
   BOOL createVrmlFromUrlFound = FALSE;

   if (scene && scene->createVrmlFromUrls.getNum()>0) {

		// for resolving relative urls 
		GLoadInlinesTraversal traversal;
		traversal.view = view;

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
				// instance PROTO's 
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

				// send the event
				src->SendEvent();
				// remove request from list 
				scene->removeCreateVrmlFromUrl(src); // will unref it !
				createVrmlFromUrlFound = TRUE;
				i--; // continue check others 
				}
			}
			src->unref();

		} // for i


   } // createVrmlFromUrl	

   if (createVrmlFromUrlFound)   
   {
	  view->UpdateSceneInfo(); // there may be sensor stuff added 
	  // addChildren will set redraw bit !
   }
   else  {
   
     // it's a URL propably from Inline/ImageTexture Sound 
     // to think : call callback 0n Node
	 
	   if (f->refCnt > 0)  // non obsolete threand
         view->needUpdate ++; // do a refresh, need inlines will be loaded 

	// f->unref(); done by WWWInline/Texture 2

   }

   // load a bit more 
   if (GFile::threadsInUse == 0) 
	   view->triggerUrlLoading = TRUE;

   return TRUE ;
}


// OpenGl Version : render scene to image 
void CMyglView::OnFileRenderToFile() 
{

        CString		path;
        static CString current;

        if (Expired()) return;


		if (current.ReverseFind('\\') > 0) {
			path=current.Left(current.ReverseFind('\\'));
		}

		CFileDialog f(FALSE,NULL,current,0,
            "BMP-Files (*.bmp;*.dib)| *.bmp;*.dib||",NULL);

	    f.m_ofn.lpstrTitle = "Render scene to image file";

		if (path.GetLength()>0) f.m_ofn.lpstrInitialDir = path;

		if (f.DoModal() == IDOK) {
			current = f.GetPathName();
	        Message("Reading texture "+current + "...",PROGRESS_MESSAGE);
		    
            BeginWaitCursor();
		
		    int fmt = 0;
	    	CString fileExt = f.GetFileExt();
    		fileExt.MakeLower();
			fmt = GetFileTypeExtension(fileExt);


            DRenderFile d;
            
			d.m_fileName = current;
            if (d.DoModal() == IDOK) {
                RenderOnDib(view,d.m_fileName,d.m_width,d.m_height,24);
            }
		}
}


void CMyglView::OnFileRender() 
{
  if (Expired()) return;

   DRenderFile d;
   if (d.DoModal() == IDOK) {
       RenderOnDib(view,d.m_fileName,d.m_width,d.m_height,24);
   }
   
}

void CMyglView::OnUpdateFileRender(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CMyglView::OnOptionsGlInfo() 
{
#ifdef _OGL
    DInfoOpengl d;
  
    d.m_version = (const char *) glGetString(GL_VERSION);
    d.m_renderer = (const char *) glGetString(GL_RENDERER);
    d.m_vendor = (const char *) glGetString(GL_VENDOR);
    d.m_extensions = (const char *) glGetString(GL_EXTENSIONS);
	d.DoModal();
#endif
}






void CMyglView::OnOptionsDirect3d() 
{

#ifdef _D3D
    DD3DOptions d(this,view);

	BOOL bOnlySystemMemory = AfxGetApp()->GetProfileInt("DirectX","OnlySystemMemory",0);

	d.m_deviceIndex = view->device->CurrDevice;

    d.m_dither = view->renderState.dither;
    d.m_highlights = view->renderState.specular;
    d.m_alphaStipple = view->renderState.alphaStipple;
	d.m_driverIndex = view->device->CurrDriver;
	d.m_modeIndex = view->device->CurrMode;
    d.m_driver = view->device->Driver[view->device->CurrDriver].Name;
	d.m_textureModeIndex = view->device->CurrTextureFormat;
	d.m_tfilter = view->GetTextureFiltering()>0;
	d.m_use_videomem = ! bOnlySystemMemory;
	d.m_use_videomem = ! bOnlySystemMemory;
	d.m_software = view->renderState.enableOwnLighting;
	d.m_useD3DCulling = view->renderState.enableD3DCulling;
	d.m_anitalias = view->renderState.lineSmooth;


	DWORD mem = view->device->TotalVideoMemory() / 1024;
	DWORD free = view->device->FreeVideoMemory() / 1024;
	BOOL vidBuffer = view->device->bBackBufferInVideo;
		
	d.m_info.Format("Video Mem : %dk free:%dk %s Soft %d",mem,free,(vidBuffer ? "using video mem" : "using system mem"),(int) view->renderState.doOwnLighting);


    if (d.DoModal() == IDOK) 
    {
		int updateState=0;
		bOnlySystemMemory = !d.m_use_videomem;

	    SetProfile("Direct3D","onlySystemMemory",bOnlySystemMemory);
        
		if (d.m_dither != view->renderState.dither) {
            view->renderState.dither = d.m_dither;
			updateState = 1;
        }
        if (d.m_highlights != view->renderState.specular)  {
            view->renderState.specular = d.m_highlights;
			updateState = 1;
        }
        if (d.m_alphaStipple != view->renderState.alphaStipple)  {
            view->renderState.alphaStipple = d.m_alphaStipple;
			updateState = 1;
        }

		if (d.m_anitalias != view->renderState.lineSmooth) {
			view->renderState.lineSmooth= d.m_anitalias;
			updateState = 1;

		}

		if (d.m_software != view->renderState.enableOwnLighting) {
			view->renderState.enableOwnLighting = d.m_software;
			Reporter.Warning("Resize or change device to HAL / RGB for software lighting change ");
			updateState = 1;
		}
		if (d.m_useD3DCulling != view->renderState.enableD3DCulling) {
			view->renderState.enableD3DCulling = d.m_useD3DCulling;
			//Reporter.Warning("Resize or change device to HAL / RGB for software lighting change ");
			updateState = 1;
		}

		if (d.m_deviceIndex != view->device->CurrDevice) {
				int oldDriver = view->device->CurrDevice;
                if (!view->device->ChangeDevice(d.m_deviceIndex,(d.m_deviceIndex == 0) ?D3DAPP_ONLYWINDOW:0)) {
                    view->ReportD3DError();
				}
		
		} else {

		if (updateState) view->renderState.SetState();

		// full screen mode 
		view->device->CurrMode = d.m_modeIndex;

/*
		if (d.m_driverIndex != d3dapp->CurrDriver) {
				int oldDriver = d3dapp->CurrDriver;
                if (!D3DAppChangeDriver(d.m_driverIndex,NULL)) {
                    ReportD3DAppError();
				}

		}
*/
		if (d.m_driverIndex != view->device->CurrDriver) {
				int oldDriver = view->device->CurrDriver;
                if (!view->device->ChangeDriver(d.m_driverIndex,NULL)) {
                    view->ReportD3DError();
				}

		}
		else { 
#if 0			
		// ignore
		if (d.m_textureModeIndex >=0 &&  (d.m_textureModeIndex != view->device->CurrTextureFormat)) {
           if (d.m_textureModeIndex < view->device->NumTextureFormats) {
				if (!view->device->ChangeTextureFormat(d.m_textureModeIndex)) {
					Reporter.Warning("Could not change texture format");				
				}
				view->ReleaseTextures();
		   } else Reporter.Warning("This mode not available using %s driver",view->device->Driver[view->device->CurrDriver].Name);
		}
#endif

		}
		} // device changed 
	    view->SetTextureFiltering(d.m_tfilter);

		Redraw();

    }
#endif
}

void CMyglView::OnUpdateOptionsDirect3d(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CMyglView::OnOptionsFullscreen() 
{
	if (!view->GetFullScreen()) {
		CloseAllDialogs();
	}
	view->ToggleFullScreen();
    Redraw();

}


void CMyglView::OnUpdateOptionsFullscreen(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(view->GetFullScreen());
}


// these functions are not called from view, only called from  mainframe

void CMyglView::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CView::OnActivate(nState, pWndOther, bMinimized);
	view->OnActivate();
}

void CMyglView::OnActivateApp(BOOL bActive, HTASK hTask) 
{
	CView::OnActivateApp(bActive, hTask);
	// active = bActive;
	// TODO: Add your message handler code here
	
}

void CMyglView::OnCameraAnimateViewpoints() 
{
	if (view->AnimatingCamera()) {
		view->AnimateCameraStop();
		UpdateViewpointDialog();
    }
	else view->AnimateAllCameras();
	
}

void CMyglView::OnUpdateCameraAnimateViewpoints(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(view->AnimatingCamera());

}

void CMyglView::OnCameraViewpointNext() 
{
    view->SetNextViewpoint(view->cameraAnimate);
	UpdateViewpointDialog();	
	Redraw();
}

void CMyglView::OnUpdateCameraViewpointNext(CCmdUI* pCmdUI) 
{
	
}

void CMyglView::OnCameraViewpointPrev() 
{
	view->SetPrevViewpoint(view->cameraAnimate);
	UpdateViewpointDialog();	
	Redraw();
	
}

void CMyglView::OnUpdateCameraViewpointPrev(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
}


void CMyglView::OnToolsTransform() 
{

   if (dialogs.m_dTransform == NULL) {
    	dialogs.m_dTransform = new DTransformSheet(_T("Transform"),this);
		dialogs.m_dTransform->AddPage(new DTransformPosition(this));
		dialogs.m_dTransform->AddPage(new DTransformScale(this));
		dialogs.m_dTransform->AddPage(new DTransformRotation(this));
		dialogs.m_dTransform->AddPage(new DTransformMode(this));
//xxx 		dialogs.m_dTransform->AddPage(new DTransformSelect(this));
		dialogs.m_dTransform->AddPage(new DTransformNewObject(this));
		dialogs.m_dTransform->Create(this);

	}
    else {
		if (dialogs.m_dTransform->m_hWnd == NULL) dialogs.m_dTransform->Create(this);
        else dialogs.m_dTransform->UpdateData(FALSE);	// View state ==> to dialog
		dialogs.m_dTransform->ShowWindow(SW_SHOWNORMAL);
    }
}

void CMyglView::OnUpdateToolsTransform(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(view->GetScene() != NULL);	
}



void CMyglView::OnCameraHeadLight() 
{
	view->SetHeadLight(!view->GetHeadLight());
	Message(CString("Headlight is now ") + ((view->GetHeadLight()) ? "On" : "Off") );
	Redraw();
}

void CMyglView::OnUpdateCameraHeadLight(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(view->GetHeadLight());
}


void CMyglView::OnCameraCollision() 
{
	view->collisionDetection = !  view->collisionDetection;
	Message(CString("Collision detection is now ") + ((view->collisionDetection) ? "On" : "Off") );

}

void CMyglView::OnUpdateCameraCollision(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(view->collisionDetection);	
}

void CMyglView::OnCameraGravity() 
{
	view->stayOnGround = !view->stayOnGround;
	Message(CString("Gravity is now ") + ((view->stayOnGround) ? "On" : "Off") );

}

void CMyglView::OnUpdateCameraGravity(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(view->stayOnGround);
}



void CMyglView::OnToolsOptimize() 
{

	CWaitCursor wait;

	CloseAllDialogs();
	if (view->theShell) {
        // Convert shell to full VRML scene including current options
	  // changed to a scene node 
        GvNode * node = view->CreateVrmlScene(gtrue);
        if (node) {
            view->theShell = NULL;
             view->SetNode(node,0);
        }
		return;
   }    

	GvScene *n =view->GetScene();
	if (!n) return;

		GOptimizeTraversal traversal;
   		traversal.InitializeAttributes();
  		traversal.view = view;

		InitializeGvAttributes(traversal);

		
		GetOptimizerSettings(traversal);
		traversal.vrml2= gtrue;

		if (traversal.bx3d) {
			n->protos.deleteAll();
		} else {
			traversal.keepPrimitives=gtrue;
		}

		if (traversal.ignoreRoutes) {
			n->routes.deleteAll();
		}

		
		Reporter.Status("Optimizing Tree ...");

		traversal.Do(n->children);

		n->children.deleteAll();

		traversal.GetResult(n->children);
		
		n->vrml2.set(TRUE); // will do an VRML 2 conversion
		
		view->UpdateSceneInfo();
		
		Reporter.Status("Optimizing Tree done");

		Redraw();
	
}

void CMyglView::OnUpdateToolsOptimize(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(view->GetScene() != NULL);	
}


void CMyglView::OnToolsVrml2Convert() 
{
	CWaitCursor wait;

	CloseAllDialogs();
	if (view->theShell) {
        // Convert shell to full VRML scene including current options
	  // changed to a scene node 
        GvNode * node = view->CreateVrmlScene(gtrue);
        if (node) {
            view->theShell = NULL;
             view->SetNode(node,0);
        }
		return;
   }    

	GvScene *n =view->GetScene();
	if (!n) return;

		GOptimizeTraversal traversal;
   		traversal.InitializeAttributes();
  		traversal.view = view;

		InitializeGvAttributes(traversal);

		
		GetOptimizerSettings(traversal);
		traversal.vrml2= gtrue;

		if (traversal.bx3d) {
			n->protos.deleteAll();
		} else {
			traversal.keepPrimitives=gtrue;
		}

		if (traversal.ignoreRoutes) {
			n->routes.deleteAll();
		}

		
		Reporter.Status("Optimizing Tree ...");

		traversal.Do(n->children);

		n->children.deleteAll();

		traversal.GetResult(n->children);
		
		n->vrml2.set(TRUE); // will do an VRML 2 conversion
		
		view->UpdateSceneInfo();
		
		Reporter.Status("Optimizing Tree done");

		Redraw();
	
	
}

void CMyglView::OnUpdateToolsVrml2Convert(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}



BOOL CMyglView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	
	///// xxxx if (!CView::OnSetCursor(pWnd, nHitTest, message)) 
	SetCursor(hCurrentCursor);
	return (TRUE);
}


// WM_MOVING
afx_msg LRESULT CMyglView::OnMoving(WPARAM wParam, LPARAM lParam)
{
	return(TRUE);
}

//WM_SIZING
afx_msg LRESULT CMyglView::OnSizing(WPARAM wParam, LPARAM lParam)
{
	return(TRUE);
}

//WM_CAPTURECHANGED
afx_msg LRESULT CMyglView::OnCaptureChanged(WPARAM wParam, LPARAM lParam)
{

	//hwndNewCapture = (HWND) lParam;    // handle of window to gain mouse capture 
	TRACE("Capture changed \n");
	// to do: kill event handler 
	// set Cursor 
	if (Mode != NULL_MODE)
		SetCursorMode(STANDARD_CURSOR);

	Mode = NULL_MODE;

	return(TRUE);
}
// Stop navigation action, e.g. because new viewpoint was bound 
void CMyglView::OnStopNavigation() 
{
	// set Cursor 
	if (m_currentMode != NULL_MODE)
		SetCursorMode(STANDARD_CURSOR);
	m_currentMode = NULL_MODE;

}


// disable/enable sound 
void CMyglView::OnRenderSound() 
{
	 view->SetUseSound(!view->GetUseSound());
}

void CMyglView::OnUpdateRenderSound(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(view->GetUseSound());
}

void CMyglView::OnOptionsShowAvatar() 
{
	view->ShowAvatar(!view->IsAvatarShown());
	view->SetThirdPersonView(view->IsAvatarShown());
}

void CMyglView::OnUpdateOptionsShowAvatar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(view->IsAvatarShown());
}


void CMyglView::OnOptionsDriver(UINT id) 
{
	int i= id - ID_DRIVER_0; 
#ifdef _D3D
	if (view->device->ChangeDriver(i,0)) {
		Redraw();
	}
#endif
	
}

void CMyglView::OnUpdateOptionsDriver(CCmdUI* pCmdUI) 
{
#ifdef _D3D
	int i= pCmdUI->m_nID - ID_DRIVER_0; 
	int index=view->device->CurrDriver;
	pCmdUI->SetCheck(i == index );
#endif
	
}


void CMyglView::OnCameraViewpoint(UINT id) 
{
	int i= id - ID_VIEWPOINT_0; 
    view->SetViewpoint(i,view->cameraAnimate);
	if (view->camera->Changed()) Redraw();

}


void CMyglView::OnUpdateCameraViewpoint(CCmdUI* pCmdUI) 
{
	int i= pCmdUI->m_nID - ID_VIEWPOINT_0; 
	int index;
	view->GetViewpointIndex(index);
	pCmdUI->SetCheck(i == index );
}


int CMyglView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CView::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void CMyglView::OnToolsMetaverse() 
{
	// TODO: Add your command handler code here
	
}

void CMyglView::OnUpdateToolsMetaverse(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CMyglView::OnToolsLibrary() 
{
	// TODO: Add your command handler code here

    if (dialogs.m_dLibrary == NULL) { 
        dialogs.m_dLibrary = new DTreeEditor(this,this);
		dialogs.m_dLibrary->viewCompact = TRUE;
		
		dialogs.m_dLibrary->Create(DTreeEditor::IDD);
		
		dialogs.m_dLibrary->m_controller = m_controller;

		// Update tree data 
		GDisplayTreeTraversal traversal;
		
		traversal.InitializeAttributes();
		
		traversal.treeEditor = dialogs.m_dLibrary;
		GvNode* lib= view->GetLibrary();
		if (lib)
			int ret = lib->Traverse(traversal);

	}
	else dialogs.m_dLibrary->UpdateData(FALSE);

	
}

void CMyglView::OnUpdateToolsLibrary(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CMyglView::OnOptionsPreferences() 
{   // CPropertySheet PROPSHEETHEADER 
	DPropSheet dialog(Translate(_T("Preferences")),this);
	DPropGeneral *pGeneral=new DPropGeneral(this); //

	pGeneral->m_verbose  = m_verbose;
	pGeneral->m_fullSpeed = m_fullSpeed;
	pGeneral->m_exactCollision = view->useRapid;




	HKEY hKeyRoot=NULL;
	BOOL PanelOn;

	//::RegOpenKey(HKEY_CURRENT_USER, USER_KEY, &hKeyRoot);

	if (hKeyRoot && GetProfile("General","panel", PanelOn)) {
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
    //pCache->m_cacheRead = theCache.readCacheEnabled;
	pCache->m_cacheDirectory = theCache.writeCacheDir;

	pCacheSettings->m_cacheVerify = (int)theCache.cacheMode;
    pCacheSettings->m_verifyDays = theCache.expirationDays;

    pCacheSettings->m_maxSpace = theCache.maxSpaceUsed / (1024*1024);
    pCacheSettings->m_minSpaceFree = theCache.minSpaceFree / (1024*1024);

	// get from registry 
	{
	CString val;

	if (hKeyRoot && GetProfile("Cache","readonlyDirectorys", val)) {
		pCache->m_roCacheDirectorys = val;
	} else 
		pCache->m_roCacheDirectorys = "cdrom:\\cache"; // set as default 


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

	if (GetProfile("Direct3D","onlySystemMemory", bOnlySystemMemory)) {
	}


	}
	//xxbOnlySystemMemory = GetSysMemFlag();
	pD3D->m_use_videomem = ! bOnlySystemMemory;



	dialog.AddPage(pD3D);

#endif

#ifdef _OGL
	// Direct 3D Options
	DPropOpenGL *pOGL=new DPropOpenGL(this);

	pOGL->m_dither = view->GetDithering();
	pOGL->m_tfilter = view->GetTextureFiltering()>0;
	pOGL->m_tmipmap = view->GetTextureMipMap()>0;
	pOGL->m_nurbsModeIndex = view->renderState.nurbsMode;

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
	if (GetProfile("OpenGL","software",v))
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


	dialog.AddPage(pSound);

	// removed pGeneral->m_cache=GFile::cacheDirectory;
	
	//pGeneral->m_uiModeIndex = m_uiMode;

	dialog.AddPage(pGeneral);

	if (pCache)
		dialog.AddPage(pCache);

	if (pCacheSettings)
		dialog.AddPage(pCacheSettings);

	if (hKeyRoot) ::RegCloseKey(hKeyRoot);
	hKeyRoot=NULL;

	if (dialog.DoModal() == IDOK) {

			// removed GFile::cacheDirectory = pGeneral->m_cache;
			//m_uiMode = (UIMode) pGeneral->m_uiModeIndex;
			m_verbose = pGeneral->m_verbose ;
			if (m_reporter) m_reporter->verbose = m_verbose;

			m_fullSpeed= pGeneral->m_fullSpeed;
			PanelOn = pGeneral->m_navPanelOn;
			view->useRapid= !! pGeneral->m_exactCollision;

		#ifdef _KATMAI
			if (pD3D2->m_useKamtai != useKatmai) {
				KatmaiEnable(pD3D2->m_useKamtai);
			}
		#endif

			view->targetFrameRate= pD3D2->m_targetFps;
			view->nurbsTessellationScale=pD3D2->m_tessellationFactor;
			view->nurbsTessellationMode=pD3D2->m_nurbsModeIndex;

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
				if (view->device->ChangeDriver(pD3D->m_driverIndex,0)) {	
					//XX if (view->m_navPanelOn)
					//XX	view->NavigationPanelCreate(0);
					view->SetTextureFiltering(!!pD3D->m_tfilter);
					view->SetTextureMipMap(pD3D->m_tmipmap);
					view->SetAntiAliasing(!!pD3D->m_antiAlias);
				}
			}
			Redraw();
#endif
#ifdef _OGL
			view->SetDithering(pOGL->m_dither);
			view->SetTextureFiltering(pOGL->m_tfilter);
			view->SetTextureMipMap(pOGL->m_tmipmap);
			view->renderState.nurbsMode = (GGlNurbsMode) pOGL->m_nurbsModeIndex;
			SetProfile("OpenGL","nurbsMode",view->renderState.nurbsMode);
			Redraw();
#endif

			//HKEY hKeyRoot;
			// SAVE settings to registry 
			
			//if (::RegCreateKey(HKEY_CURRENT_USER, USER_KEY, &hKeyRoot) == ERROR_SUCCESS) 
			{

				CString val;
				val = GFile::cacheDirectory;
				SetProfile("cache","directory", val );

				//xxSetProfile("UI","mode", (int) m_uiMode );

				//xx SetProfile("General","panel", (int) PanelOn);				
				//xx SetProfile("General","fullSpeed", (int) m_fullSpeed);
				SetProfile("General","verbose", (int) m_verbose);
				SetProfile("General","exactCollision", (int) 	view->useRapid);

				SetProfile("Nurbs","targetFps", view->targetFrameRate);
				SetProfile("Nurbs","tessellationMode", view->nurbsTessellationMode);
				if (view->nurbsTessellationMode == GNURBS_STATIC_TESSELATION)
					SetProfile("Nurbs","tessellationScale", view->nurbsTessellationScale*1000);


#ifdef _D3D
				val = view->device->Driver[view->device->CurrDriver].Name;
				SetProfile("Direct3D","driver", val);

				bOnlySystemMemory = !pD3D->m_use_videomem;


				val = "";

				if (view->device->GetDefaultFullscreenMode(val))
					SetProfile("Direct3D","fullscreenMode",val);

				SetProfile("Direct3D","dither", view->GetDithering());
				SetProfile("Direct3D","textureFiltering", view->GetTextureFiltering() );
				SetProfile("Direct3D","textureMipMap", view->GetTextureMipMap() );
				SetProfile("Direct3D","antiAlias", view->GetAntiAliasing() );
				
				//SetProfile("Direct3D","alphaStipple"),view->renderState.alphaStipple);
				//SetProfile("Direct3D","antiAlias",view->renderState.lineSmooth);

				SetProfile("Direct3D","askHardware", m_askHardware );
				SetProfile("Direct3D","onlySystemMemory", bOnlySystemMemory);
				
				SetProfile("Direct3D","useSoftwareLighting", view->renderState.enableOwnLighting);
				SetProfile("Direct3D","useD3DCulling", view->renderState.enableD3DCulling);

#endif

#ifdef _OGL
				SetProfile("OpenGL","textureFiltering", view->GetTextureFiltering() );
				SetProfile("OpenGL","textureMipMap", view->GetTextureMipMap() );
				SetProfile("OpenGL","dither", view->GetDithering());
				//SetProfile("OpenGL","zdepth", software);
				SetProfile("OpenGL","software", pOGL->m_rsoftware);
#endif

				//xxSetProfile("Navigation.anchorSingleClick"), m_anchorSingleClick);

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

				SetProfile("Sound","numSounds", view->maxNumSounds);
				SetProfile("Sound","useRsx", view->useRSXSound);
				SetProfile("Sound","quality", view->soundQuality);
				SetProfile("Sound","doppler", view->soundDoppler);


				if (pCache) {

					theCache.writeCacheEnabled = !!pCache->m_useCache;
					//theCache.readCacheEnabled = pCache->m_cacheRead;
					theCache.readCacheEnabled = theCache.writeCacheEnabled;
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

					SetProfile("Cache","verifyMode", theCache.cacheMode);

					SetProfile("Cache","writeCache", theCache.writeCacheEnabled);
					SetProfile("Cache","readCache", theCache.readCacheEnabled);

					SetProfile("Cache","readonlyDirectorys", pCache->m_roCacheDirectorys);
					SetProfile("Cache","verifyDays", theCache.expirationDays);

					SetProfile("Cache","minDiskSpaceFreeKB", theCache.minSpaceFree/1024);
					SetProfile("Cache","maxDiskSpaceUsedKB", theCache.maxSpaceUsed/1024);

					
					BOOL ret = theCache.SetWriteCacheDirectory(pCache->m_cacheDirectory);
					if ((theCache.writeCacheDir.GetLength()>0) &&  (!theCache.writeCacheDirValid || !ret)) {
						AfxMessageBox("Warning : bad caching directory "+pCache->m_cacheDirectory);
					}
					else SetProfile("Cache","directory", pCache->m_cacheDirectory);

					theCache.SetReadCacheDirectoryPath(pCache->m_roCacheDirectorys);


				}

				//::RegCloseKey(hKeyRoot);
			}

	}
}

