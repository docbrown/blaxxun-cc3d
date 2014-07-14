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
// dwww.cpp : implementation file
//

#include "stdafx.h"
#include "glviewhdr.h"

#include "resource.h"

#include "www.h"

#include "dwww.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DOpenUrl dialog


DOpenUrl::DOpenUrl(CWnd* pParent /*=NULL*/,CMyglView *view)
	: CDialog(DOpenUrl::IDD, pParent)
{
	m_pView = view;
	//{{AFX_DATA_INIT(DOpenUrl)
	m_status = _T("");
	m_url = _T("");
	m_historySel = -1;
	m_add_files = FALSE;
	//}}AFX_DATA_INIT
}


void DOpenUrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DOpenUrl)
	DDX_Control(pDX, IDC_HISTORY, m_history);
	DDX_Text(pDX, IDC_STATUS, m_status);
	DDX_Text(pDX, IDC_URL, m_url);
	DDX_LBIndex(pDX, IDC_HISTORY, m_historySel);
	DDX_Check(pDX, IDC_ADD_FILES, m_add_files);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DOpenUrl, CDialog)
	//{{AFX_MSG_MAP(DOpenUrl)
	ON_LBN_SELCHANGE(IDC_HISTORY, OnSelChangeHistory)
	ON_LBN_DBLCLK(IDC_HISTORY, OnDblclkHistory)
	ON_EN_CHANGE(IDC_URL, OnChangeUrl)
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// DOpenUrl message handlers

int DOpenUrl::ClearHistory()
{
  m_history.ResetContent();
  return(0);
}

static const char *section = "History";
// to do: don´t add duplicates ?
int DOpenUrl::AddToHistory(const char * string,int mode)
{
	int ret = m_history.AddString(string);
    if (ret>=0) {
    	char  entry[60];
    	sprintf(entry,"history%d",ret);
    	AfxGetApp()->WriteProfileString( section, entry, string);

  		sprintf(entry,"history%d",ret+1);
  		AfxGetApp()->WriteProfileString( section, entry, "");

	}
	return(ret);
}
int DOpenUrl::AddFileToHistory(const char * string,int mode)
{
	if (!m_add_files) return(0);
	int ret = m_history.AddString(string);
    if (ret>=0) {
    	char  entry[60];
    	sprintf(entry,"history%d",ret);
    	AfxGetApp()->WriteProfileString( section, entry, string);
  		
  		sprintf(entry,"history%d",ret+1);
  		AfxGetApp()->WriteProfileString( section, entry, "");
	}
	return(ret);
}

int DOpenUrl::SetUrl(const char * string,int mode)
{
	m_url = string;
    m_historySel = -1;
	CDataExchange DX(this, FALSE);
	
	DDX_Text(&DX, IDC_URL, m_url);
    m_history.SetCurSel(m_historySel);

	//DDX_LBIndex(&DX, IDC_HISTORY, m_historySel);

	return(0);
}

int DOpenUrl::SetStatus(const char * string,int mode)
{
	m_status = string;
	CDataExchange DX(this, FALSE);
	DDX_Text(&DX, IDC_STATUS, m_status);
	return(0);
}

const int maxHistoryCnt = 1000;

void DOpenUrl::SaveHistory() 
{
  CString value;
  char  entry[60];
  int i;
  for (i=0; i<m_history.GetCount(); i++) {
   m_history.GetText(i,value);
   sprintf(entry,"history%d",i);
   if (!AfxGetApp()->WriteProfileString( section, entry, value))
  	 	break;
  }

  sprintf(entry,"history%d",i);
  AfxGetApp()->WriteProfileString( section, entry, "");

  AfxGetApp()-> WriteProfileInt(section,"historyCnt",i);
  AfxGetApp()-> WriteProfileInt(section,"historyAddFiles",m_add_files);
}

void DOpenUrl::RestoreHistory() 
{ const char *section = "History";
  CString value;
  char  entry[60];
  int i,cnt;

  m_add_files = AfxGetApp()-> GetProfileInt(section,"historyAddFiles",TRUE);

 // cnt = AfxGetApp()->GetProfileInt(section,"historyCnt",0);
  cnt = 1000;
  for (i=0; i<cnt; i++) {
   if (i>maxHistoryCnt) break;
   sprintf(entry,"history%d",i);
   
   value = AfxGetApp()->GetProfileString( section, entry);
   if (value.GetLength() == 0) 	break;
   m_history.AddString(value);
  }
}


void DOpenUrl::OnSelChangeHistory() 
{
	// TODO: Add your control notification handler code here
	m_historySel = m_history.GetCurSel();
	if (m_historySel >=0) {
	   m_history.GetText(m_historySel,m_url);
	   DDX_Text(&CDataExchange(this, FALSE),IDC_URL, m_url);
	}	
}

void DOpenUrl::OnDblclkHistory() 
{
	// TODO: Add your control notification handler code here
	OnSelChangeHistory();
	if (m_historySel >=0) 
	   OnOK();
}

void DOpenUrl::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
	if (m_pView) {
		int ret = m_pView->ReadUrl(m_url);
	}
}

void DOpenUrl::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL DOpenUrl::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DOpenUrl::OnChangeUrl() 
{
	if (m_historySel >=0) {
	   m_history.SetCurSel(-1);
	   m_historySel = -1;
	}
	
}
/////////////////////////////////////////////////////////////////////////////
// DWWWOptions dialog


DWWWOptions::DWWWOptions(CWnd* pParent /*=NULL*/,CMyglView *pView)
	: CDialog(DWWWOptions::IDD, pParent)
{
	m_pView = pView;
	//{{AFX_DATA_INIT(DWWWOptions)
	m_useCamera = FALSE;
	m_useSpotLights = FALSE;
	m_useSceneLights = FALSE;
	m_headLight = TRUE;
	m_cacheDirectory = _T("");
	m_keepHostConnection = FALSE;
	m_useTextures = FALSE;
	m_useShapeHints = FALSE;
	m_useBackground = TRUE;
	m_useMaterials = TRUE;
	m_useSensors = TRUE;
	m_maxInlineDepth = 0;
	m_incrementalUpdates = FALSE;
	m_email = _T("");
	m_inline = -1;
	m_useFilesFromCache = FALSE;
	m_lightScale = 1.0f;
	m_browserPath = _T("");
	m_auto_play = FALSE;
	m_texture_loading = -1;
	m_ambientLight = _T("");
	m_enableCollision = FALSE;
	m_avatar = _T("");
	//}}AFX_DATA_INIT
}


void DWWWOptions::DoDataExchange(CDataExchange* pDX)
{
	if (!pDX->m_bSaveAndValidate)
		if (m_pView) m_pView->Get(*this);

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DWWWOptions)
	DDX_Check(pDX, IDC_USE_CAMERA, m_useCamera);
	DDX_Check(pDX, IDC_SCENE_SPOT_LIGTHS, m_useSpotLights);
	DDX_Check(pDX, IDC_SCENE_LIGTHS, m_useSceneLights);
	DDX_Check(pDX, IDC_SCENE_HEAD_LIGHT, m_headLight);
	DDX_Text(pDX, IDC_CACHE_DIRECTORY, m_cacheDirectory);
	DDX_Check(pDX, IDC_KEEP_CONNECTION, m_keepHostConnection);
	DDX_Check(pDX, IDC_USE_TEXTURES, m_useTextures);
	DDX_Check(pDX, IDC_USE_SHAPE_HINTS, m_useShapeHints);
	DDX_Check(pDX, IDC_USE_BACKGROUND, m_useBackground);
	DDX_Check(pDX, IDC_USE_MATERIALS, m_useMaterials);
	DDX_Check(pDX, IDC_USE_SENSORS, m_useSensors);
	DDX_Text(pDX, IDC_MAX_INLINE, m_maxInlineDepth);
	DDV_MinMaxInt(pDX, m_maxInlineDepth, 0, 60);
	DDX_Check(pDX, IDC_INCREMENTAL_UPDATES, m_incrementalUpdates);
	DDX_Text(pDX, IDC_EMAIL, m_email);
	DDV_MaxChars(pDX, m_email, 256);
	DDX_Radio(pDX, IDC_INLINE_NEVER, m_inline);
	DDX_Check(pDX, IDC_USE_CACHE, m_useFilesFromCache);
	DDX_Text(pDX, IDC_LIGHT_SCALE, m_lightScale);
	DDX_Text(pDX, IDC_BROWSER_PATH, m_browserPath);
	DDX_Check(pDX, IDC_AUTO_PLAY, m_auto_play);
	DDX_Radio(pDX, IDC_TEXTURE_NEVER, m_texture_loading);
	DDX_Text(pDX, IDC_LIGHT_AMBIENT, m_ambientLight);
	DDX_Check(pDX, IDC_ENABLE_COLLISION, m_enableCollision);
	DDX_Text(pDX, IDC_AVATAR, m_avatar);
	//}}AFX_DATA_MAP


	if (pDX->m_bSaveAndValidate)
		if (m_pView) m_pView->Apply(*this);

}


BEGIN_MESSAGE_MAP(DWWWOptions, CDialog)
	//{{AFX_MSG_MAP(DWWWOptions)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_INLINE_ALL, OnInlineAll)
	ON_BN_CLICKED(IDC_INLINE_IF_NEEDED, OnInlineIfNeeded)
	ON_BN_CLICKED(IDC_INLINE_NEVER, OnInlineNever)
	ON_BN_CLICKED(IDC_BROWSE_BROWSER, OnBrowseBrowser)
	ON_BN_CLICKED(IDC_BROWSE_CACHE, OnBrowseCache)
	ON_BN_CLICKED(IDC_WWW_CONFIG, OnInternetConfig)
	ON_BN_CLICKED(IDC_BROWSE_AVATAR, OnBrowseAvatar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// DWWWOptions message handlers

void DWWWOptions::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void DWWWOptions::OnSave() 
{
	// TODO: Add your control notification handler code here
	if (m_pView) {	 
		if (UpdateData(TRUE)) 
		{
	   	   m_pView->Apply(*this);
		   m_pView->WriteProfileSetting();
	    }
	}

	
}

void DWWWOptions::OnInlineAll() 
{
	// TODO: Add your control notification handler code here
	
}

void DWWWOptions::OnInlineIfNeeded() 
{
	// TODO: Add your control notification handler code here
	
}

void DWWWOptions::OnInlineNever() 
{
	// TODO: Add your control notification handler code here
	
}
/////////////////////////////////////////////////////////////////////////////
// DWWWRetry dialog


DWWWRetry::DWWWRetry(CWnd* pParent /*=NULL*/)
	: CDialog(DWWWRetry::IDD, pParent)
{
	//{{AFX_DATA_INIT(DWWWRetry)
	m_url = _T("");
	m_status = _T("");
	//}}AFX_DATA_INIT
}


void DWWWRetry::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DWWWRetry)
	DDX_Text(pDX, IDC_URL, m_url);
	DDX_Text(pDX, IDC_STATUS, m_status);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DWWWRetry, CDialog)
	//{{AFX_MSG_MAP(DWWWRetry)
	ON_BN_CLICKED(IDC_IGNORE_ALL, OnIgnoreAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// DWWWRetry message handlers

void DWWWRetry::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void DWWWRetry::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void DOpenUrl::OnClear() 
{
	ClearHistory();	
}

void DOpenUrl::OnSave() 
{
	SaveHistory();	
}

void DWWWRetry::OnIgnoreAll() 
{
	// TODO: Add your control notification handler code here
	
}

void DWWWOptions::OnBrowseBrowser() 
{
	CDataExchange dxs(this, TRUE);
	CDataExchange dxr(this, FALSE);

	DDX_Text(&dxs, IDC_BROWSER_PATH, m_browserPath);

	if (1) {
		CString		path;
		if (m_browserPath.ReverseFind('\\') > 0) {
			path=m_browserPath.Left(m_browserPath.ReverseFind('\\'));
		}

		CFileDialog f(TRUE,NULL,m_browserPath,0,"Executables|*.exe||");
		f.m_ofn.lpstrTitle = "Path to WWW Browser";
		if (path.GetLength()>0) f.m_ofn.lpstrInitialDir = path;

		if (f.DoModal() == IDOK) {
			m_browserPath = f.GetPathName();
		    CDataExchange dxr(this, FALSE);
			DDX_Text(&dxr, IDC_BROWSER_PATH, m_browserPath);
		}
	}
}

void DWWWOptions::OnBrowseCache() 
{
	CDataExchange dxs(this, TRUE);
	CDataExchange dxr(this, FALSE);

	DDX_Text(&dxs, IDC_CACHE_DIRECTORY, m_cacheDirectory);

	CString		path=m_cacheDirectory;
	

	CFileDialog f(TRUE,NULL,NULL,0,"All files|*.*||");
	f.m_ofn.lpstrTitle = "Path for GLView WWW cache directory";
	
	if (path.GetLength()>0) f.m_ofn.lpstrInitialDir = path;
	if (f.DoModal() == IDOK) {
	
			m_cacheDirectory = f.GetPathName();

			if (m_cacheDirectory.ReverseFind('\\') > 0) {
				m_cacheDirectory=m_cacheDirectory.Left(m_cacheDirectory.ReverseFind('\\'));
			}


		    CDataExchange dxr(this, FALSE);
			DDX_Text(&dxr, IDC_CACHE_DIRECTORY,m_cacheDirectory);


	}
}
/////////////////////////////////////////////////////////////////////////////
// DWWWConfig dialog


DWWWConfig::DWWWConfig(CWnd* pParent /*=NULL*/)
	: CDialog(DWWWConfig::IDD, pParent)
{
	//{{AFX_DATA_INIT(DWWWConfig)
	m_proxy = _T("");
	m_proxy_bypass = _T("");
	m_access_type = -1;
	m_max_threads = 5;
	//}}AFX_DATA_INIT
}

//
// Returns an Index which represents the AccessType
//


void DWWWConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DWWWConfig)
	DDX_Text(pDX, IDC_PROXY, m_proxy);
	DDX_Text(pDX, IDC_PROXY_BYPASS, m_proxy_bypass);
	DDX_Text(pDX, IDC_MAX_THREAD, m_max_threads);
	DDX_Radio(pDX, IDC_RADIO1, m_access_type);
	//}}AFX_DATA_MAP
}




BEGIN_MESSAGE_MAP(DWWWConfig, CDialog)
	//{{AFX_MSG_MAP(DWWWConfig)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// DWWWConfig message handlers

void DWWWConfig::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void DWWWConfig::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void DWWWOptions::OnInternetConfig() 
{
	DWWWConfig d;
#ifdef G_WININET        
    d.m_proxy = GFile::proxyServer;
    d.m_proxy_bypass = GFile::proxyByPass;
    d.m_access_type=GFile::GetAccessTypeIndex();
#endif
	d.m_max_threads = GFile::maxThreads;


    if (d.DoModal() == IDOK) {
		GFile::maxThreads=max(1,d.m_max_threads);

#ifdef G_WININET        

        GFile::proxyServer = d.m_proxy;
        
		GFile::proxyByPass = d.m_proxy_bypass;
        
		GFile::SetAccessTypeIndex(d.m_access_type);
        if (GFile::hSession != NULL)
           GFile::ResetSession();
#endif

    }

}

void DWWWOptions::OnBrowseAvatar() 
{
	// TODO: Add your control notification handler code here
	CDataExchange dxs(this, TRUE);
	CDataExchange dxr(this, FALSE);

	DDX_Text(&dxs, IDC_AVATAR, m_avatar);

	if (1) {
		CString		path;
		if (m_avatar.ReverseFind('\\') > 0) {
			path=m_browserPath.Left(m_avatar.ReverseFind('\\'));
		}

		CFileDialog f(TRUE,NULL,m_avatar,0,"Avatar Vrml files|*.wrl||");

		f.m_ofn.lpstrTitle = "Path to WWW Browser";
		if (path.GetLength()>0) f.m_ofn.lpstrInitialDir = path;

		if (f.DoModal() == IDOK) {
			m_avatar = f.GetPathName();
		    CDataExchange dxr(this, FALSE);
			DDX_Text(&dxr, IDC_AVATAR, m_avatar);
		}
	}
	
}
