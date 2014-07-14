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
#ifndef _WWW_H
#define _WWW_H
/******************************************************************************
@doc

@module WWW.h - GLView WWW access + file/extension handling|

Copyright (c) 1995	by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GFile>

Notes:

Changes:

Todo :

******************************************************************************/

// don't use wininet calls 
#undef G_WININET

#ifdef WIN32
// use IE URLCACHE API's
#define G_URLCACHE
#endif


#if defined(G_WININET) || defined(G_URLCACHE)
#include <wininet.h>
#endif

// we need BrowserObserver + IBindStatusCallback
#ifdef _COM

#include "urlmon.h"
#include "glviewvrml.h"

#endif 



// flag if loading in progress
extern int busy;
// busy check , return 1 if system busy 
int Busy(int msg=0);

// Abort loading of current load 
int WWWAbortLoad();

// keep connection for loading inlines
extern int WWWkeepConnection;

// flag if files from cache can be used
extern int WWWuseFilesFromCache;
// mode flag how cache files are generated
extern int WWWCacheFileFmt;

extern CString WWWemail;
extern CString WWWbrowserPath;


void WWWCloseConnection();

// Delete a url from cache
int WWWDeleteUrlFromCache(const char *url);


enum GFileState
{
	S_NOT_LOADED,	// there was no try to load the url
	S_NO_URL,		// url/file is empty string
	S_BAD_URL,		// url/file does not exist
	S_NO_DATA,		//  url/file is empty
	S_BAD_DATA,		// url/file contains bad data or cannot be handled
	S_ALL_BAD,		// all urls are bad 
	S_ABORTED,		// url loading has been aborted
	S_LOADING,		// loading of the url is in progress
	S_LOADED,		// url has been loaded and is waiting for processing
	S_PROCESSING,   // data is beeing processed
	S_PROCESSED		// url has been loaded and processed by application
};







//@enum GFileFormats | enumerations for different 3d file formats
typedef enum 
{

UNKNOWN_FMT,//@emem unknown
HMF_FMT,	//@emem HMF
GEO_FMT,	//@emem  GEO e.g. 3dr
D3V_FMT,	//@emem  3DV native GLView format 
RAW_FMT,	//@emem  RAW triangle data
OBJ_FMT,	//@emem  Wavefront obj format
STL_FMT,	//@emem  STL triangle format ascii
DXF_FMT,	//@emem  DXF
POV_FMT,	//@emem  POV 2.0 Ray-tracer	
BXM_FMT,	// bx Media stream
VRML_FMT,	//@emem  VRML 1.0 ascii format

DIB_FMT,
RGB_FMT,
JPEG_FMT,
GIF_FMT,
TGA_FMT,

PPM_FMT,
PCX_FMT,
PNG_FMT,

AVI_FMT,
MPG_FMT,
QT_FMT,	// quicktime 

WAV_FMT,
MIDI_FMT,
AU_FMT,
MPS_FMT,	// mpeg sound 

RMA_FMT,	// real media architecture format

JS_FMT,		// javascript 

GZIP_FMT,

D3M_FMT,
HTML_FMT,

} GFileFormat;

// RMA RTSP protocl

#define G_RMA_PROTOCOL _T("rtsp://")
#define G_RMA_PROTOCOL_PNM _T("pnm://")


#define FIRST_IMAGE_FMT DIB_FMT
#define LAST_IMAGE_FMT PNG_FMT

#define FIRST_3D_FMT HMF_FMT
#define LAST_3D_FMT VRML_FMT

//#define G_URN_UMEL _T("urn:web3d:umel:")
#define G_URN_UMEL _T("urn:web3d:media:")

// 
// use registry to find list of UMEL directories
//
gbool GetUmelPath(CString &path);


// parse extions and return a known filetype id
GFileFormat GetFileType(const char *fileName);

// estimated file type from first data bytes of file
GFileFormat GetFileType( const char * data,int num); 

// get file type from file extension
GFileFormat GetFileTypeExtension(const char *extension); 


// check if file fileName does exists
// 1 ok, 0 not found
int ExistsFile(const char *fileName);


// check if this is a zipped file
int IsZippedFile(const char *fileName);

/*	
	UnZipFile - unzip file to a uncompressed on

	IN fileNameArg : zipped input file name

	OUT fileNameUnzipped : output fileName in same directory / name without gz suffix

	IN int in fmt : format of file 

	IN  useTempFile : use temp file 

	IN  keepSourceFile : FLAG keep uncompressed sourceFile 

	TO DO: allow unzip of local files to cache, eg. readonly directory on CD etc. 

 */

int UnZipFile(const char *fileNameArg, 
			  char *fileNameUnzipped, 
			  int &fmt,
			  BOOL useTempFile,
			  BOOL keepSourceFile);




#define URN_PROTOCOL "urn:"
#define URN_GLVIEW "urn:inet:glview.com"

#define URL_PROTOCOL "url:"



#define MIME_VRML "x-world/x-vrml"
#define MIME_GLVIEW "x-world/x-glview"
#define MIME_VRML1 "model/vrml"
#define MIME_HTML "text/html"

#define MIME_BXM "application/x-bxmedia"

#define MIME_GIF "image/gif"
#define MIME_JPEG "image/jpeg"
#define MIME_RGB "image/x-rgb"
#define MIME_BMP "image/x-MS-bmp"
#define MIME_PPM "image/x-portable-pixmap"

#define MIME_AVI "video/x-msvideo"
#define MIME_MPEG "video/mpeg"

#define MIME_WAV_1 "audio/x-wav"
#define MIME_WAV "audio/wav"
#define MIME_AU "audio/basic"

// ?? Midi
#define MIME_MIDI "audio/midi"

#define MIME_JAVASCRIPT "application/x-javascript"

#define MIME_RMA "audio/x-pn-realaudio"


//multipart/x-mixed-replace; boundary=Th1sRand0mStr1ng.
#define MIME_MULTI_REP "multipart/x-mixed-replace"  
//; boundary=Th1sRand0mStr1ng.

#define ERROR_BASE   -100000

#define ERROR_THREAD_STARTED (ERROR_BASE+0)

#define ERROR_HTML_DATA (ERROR_BASE+1)
#define ERROR_HTML_DATA_REPORTED (ERROR_BASE+2)
#define ERROR_REDIRECTED_URL (ERROR_BASE+2)
#define ERROR_CONNECTION_ERROR (ERROR_BASE+3)
#define ERROR_TRANSMISSION_ABORTED (ERROR_BASE+4)
#define ERROR_TRANSMISSION_ERROR (ERROR_BASE+5)
#define ERROR_UNKNOWN_PROTOCOL (ERROR_BASE+6)
#define ERROR_UNKNOWN_MIME_TYPE (ERROR_BASE+7)
#define ERROR_BAD_URL (ERROR_BASE+8)

class CString;

int CombinePath(const char *home,const char *path,CString &out);
int CreateAbsolutePath(const char *home,const char *path,CString &newPath);
int CreateRelativePathname(const char *src, const char *dest,char *result);


// flags to control Process Operation 
enum WWW_FLAG {
	WWW_REPORT_ERRORS = 1,	// Report any errors encountered 
	WWW_RELOAD = 2,			// Reload the url even if in cache 
	WWW_REFRESH = 4,		// check if file must be refreshed 
	WWW_DONT_CACHE = 8,		// don't write url to GLView cache 
	WWW_EXECUTE_UNKNOWN_FMTS = 16	// pass unknown formats to Windows Shell Execute
};



class GReporter;


// describes the class of a localFile
//
typedef enum {
	GLFC_UNKNOWN,			// unknown 
	GLFC_LOCAL_FILE,		// local file on computer
	GLFC_TMP_FILE,			// temporary file 
	GLFC_CC3D_CACHE_FILE,	// its a CC3D cache file
	GLFC_IE_CACHE_FILE,		// its a Windows UrlCache  cache file
	GLFC_NS_CACHE_FILE		// its a Netscape cache file
} GLocalFileClass;




//
// GFile

// handling URL / local filenames + zipped files

class GFile : public RTRoot

#ifdef _COM
	// callback interface for some Win-Url functions 
	, public IBindStatusCallback 
#endif
{
public:
	RT(GFile);
	LONG  refCnt; // how many references to this GFile exist 

	// increment reference counter
	void ref();
	// decrement reference counter
	void unref();
	void unrefNoDelete();

    GFile *next;    // Double link to form list of all GFiles in in use
    GFile *prev;


	GReporter *reporter; // error reporter assosicated with GFile
	void SetReporter(GReporter *reporter); // set the reporter 

	static GFile *filesInUse; // static list of GFiles in uses 

	// add m to  files in use list 
	static void AddFileInUse(GFile *m);
	
	// remove m from files in use list 
	static void RemoveFileInUse(GFile *m);

	// kill all files in use 
	static void KillFilesInUse();

	// kill all files in use using myWnd as notifaction target
	static void KillFilesInUse(HWND myWnd);

	// notify waiting file structures  by a Nescape notify url 
	static gbool  OnUrlNotify(HWND myWnd, LPCTSTR url, LPCTSTR mimeType,time_t lastModified, 
		LPCTSTR fileName, GLocalFileClass fileClass, long reason); 


	//
	// static void NotifyUrl(LPCTSTR url, LPCTSTR mimeType, LPCTSTR fileName, long reason);


	// the absolute url to be retreived, empty if a local file 
	CString url;

	// the protocol of the file 
	CString protocol;
	
	// check if we can handle the protocol (without : )
	virtual int IsProtocolOk(const char *protocl);

	// url has been loaded to local file
	gbool urlLoaded;

	CString urlHome;			// home path for url
	CString urlExtraArgs;		// info after # or ? 

	CString localFileUnzipped;  // the local filename after unzipping
	gbool localFileIsUnzipped;   

	gbool localFileUnzippedDelete;   

	CString extension;
	
	CString localFile;			//!< the downloaded file, but may be zipped
	gbool localFileLoaded;

	CString localFileHome;		// home path for local file

	CString localCacheFile;		//!< name of cc3d cache file if copied to cache 


	int localFileFmt;			//!< the file fmt as detected from extension or mime type
	
	GLocalFileClass localFileClass; //! the class of the local file

	int wwwFlags;				// WWW flags

//	time_t refreshTime;			// refresh url if cached version is older

	// set a flag 
	void SetFlags(int flags) { wwwFlags = flags; }
	
	// test flag 
	gbool Flag(int bit) { return ((wwwFlags & bit) == bit); }
	gbool AnyFlag(int bit) { return ((wwwFlags & bit) != 0); }


	gbool fileIsZipped;
	
	float loadingStartTime;
	float loadingTime;
	
	long size;					// file/url size in bytes
	//char *data;


	// set file to any object like url, local file ... ~ 
	// queryUrl = 1 means try to get URL from Netscape
	int Set(const char *path,int isLocal=0,int queryForUrl=0); 

	//! set home directory / url, use before passing a relative url to set
	int SetHome(const char *path,int isLocal=0); 

	int SetUrl(const char *path); 
	
	// set the local file part
	int SetLocalFile(const char *path,GLocalFileClass fileClass); 

	/*!
	in case of FATAL media error
	delete cached files in order to force refresh
	on next visit
	return code :
		0 no deletion was done
		1 local file deleted
		2  " may try a reload 
	*/

	int DeleteCachedLocalFile();
	//!static variant
	static int DeleteCachedLocalFile(GLocalFileClass localFileClass,const char *fileName);


	//! get url / localFile
	const char * GetUrl();

	//! get full url / localFile including # location
	CString GetFullUrl();

	//! get home url / local home directory
	const char * GetHomeUrl();

	CString arguments;
	CString title;

	// referenced from the following source document
	CString refedFrom;
	
	// the type of refence e.g. WWWAnchor, Texture2, WWWInline
	CString refedType;

	// set reference information 
	void SetRefedFrom(const char *RefedFrom, const char * RefedType) 
	{
	   refedFrom = RefedFrom;
	   refedType = RefedType;
	}


	GFile();

	virtual ~GFile();

protected:
	
	GFile& operator=(const GFile &file);

public:

	// get file type from extenion 
	int ProcessExtension(const char *fileName); 

	// process unzipping of zip-files
	int ProcessZip();


	// process url loading / url description file loading and unzipping
	// start thread if useThread = 1
	int Process();

	// process loading, after url has been retreived by thread
	int ProcessLoad(int threadRet);


	// url has been redirected 
	int ProcessRedirectedUrl(const char*newUrl);

	// got Netscape URL notification 
	gbool OnUrlNotify(LPCTSTR url, LPCTSTR mimeType, time_t lastModified, LPCTSTR fileName,GLocalFileClass fileClass, long reason); 


	// retreive url data from url and write to local file
	int WriteUrlToFile(int writeDescription=0);

	// get GFile information from .url file
	int ReadFromUrlDescriptionFile(const char *urlFile);

	// write a URL file, describing the current URL
	int WriteUrlDescriptionFile();

	int Flush(); 


	// the scene this GFile or task is belonging too
	int sceneId;

	// Internet stuff 

	CString redirectUrl;	// a url resulting from redirection
	
	// Header information for HTTP Url
	int HTTPStatus;

	// header fields 
	DWORD contentLength;
	CString contentType;
	CString contentEncoding;

	CTime lastModified;			// the date the URL was last modified (Server Date)
	CTime expires;

	CTime ifModifiedSince;		// the check date for reloading (Client Date)

	CTime cacheLastModified;	// the lastModified date of the cache file (Server Date) (File == CreationDate)
	CTime lastChecked;			// the date the URL was last checked (Client Date)

	int isMultipart;	
	
	CString partSeparator;
	CString partHeader;
	int partNumber;
	
	int ParseMimeType(const char *content);

	// check content, after HTTP session has been started
	// to decide to load url, or pass it to netscape
	virtual int IsContentOk(const char *content);

	// find url in IE-WININET-cache
	// and update some status variables
	gbool FindInternetCacheFile(CString &localFile);



	static HWND hGlobalPostMsgWnd; // the  global window, to send  thread completition messages to 
	
	HWND hPostMsgWnd; // the local window, if any to send  thread completition messages to 


	// Netscape case using window messages
	HWND hNetscapeMsgWnd; // Netscape URL -Loading window 

	#ifdef _COM
	// Netscape case using browser observer // CCpro
	// handle of observer 
	CComPtr<BrowserObserver> hObserver;

	// URLDownloadToCacheFile case 
	LPUNKNOWN client; // the container for URLDownloadToCacheFile
	IBindStatusCallback *statusCallback; //

	#endif

	static BOOL useInetCache; // USE WININET-Cache 

	// remove temporary files e.g. from unzip/download 
	static void RemoveTemporaryFiles();

	// misc global for accessing the WIN internet functions 
#ifdef G_WININET

	static CString userAgent;

	
    static DWORD  accessType;	
	// Internet Open parameters
    static CString  proxyServer;
	static CString proxyByPass;	
    
	// static INTERNET_PORT proxyPort;	
    static DWORD internetOpenFlags;	


	
	static CString ftpUserName;
	static CString ftpPassword;

	static HINTERNET hSession; // Internet session handle
#endif

	static DWORD lastReportedInternetError; // last error reported 
	static int lastReportedFileError; // last file error reported 
	static BOOL  dontReportErrors; // last error reported 
    
	static void ClearSessionError();

    // set settings off internet session
    static int InternetOpen();



	static int maxThreads;
	static gbool useThreads;
	static LONG threadsInUse;
	
	// can another thread be started ?
 	static gbool CanUseThread();

	
	gbool useThread;		// use a thread for loading
	gbool isThread;		// thread has been used for loading
#ifdef WIN32	
	CWinThread *thread;  // Thread returned from AfxBeginThread
#endif

	LONG threadBusy;		// thread is currently busy, loading not completed

	int threadRet;		// return value of thread function
	
	// is the file thread busy loading 
	int Busy() { return(threadBusy); }
	
	int threadStatus;

  	// GLView cache diredtory 
	static CString cacheDirectory;

	static long cacheMaxSize;
	static long cacheCurSize;
	
	static long cacheDownloadSize;
	
	static void DeleteCache(int options);



	// reset internet session
	static void ResetSession();

    // get InternetOpen accessType as index for dialog
    static int GetAccessTypeIndex(); 

    // Sets the Access Type using an Index
    static void SetAccessTypeIndex(int index); 

#ifdef G_WININET
	
	// WININET handle for connection
	HINTERNET hInternetConnection;

	// WININET handle for file
	HINTERNET hInternetFile;;

	BOOL ReportInternetError(HANDLE h, LPCTSTR strFunctionName);

	long LoadUrlHTTP(const char *m_strServer, int port, const char *m_strPath,char **data);
	long LoadUrlFTP(const char *m_strServer, int port, const char *m_strPath,char **data);
	
	long LoadUrlOther(const char *url,char **data);

	int InternetOpenUrl(const char * url,char **data); 

#endif

	// Kill session
	int stop; // stop flag for thread 
	
	// KILL url download 
	virtual int Kill();


#ifdef _COM
  public:

    IBinding*			m_pbinding;
    IStream*			m_pstm;

#ifdef _AFXDLL
	AFX_MODULE_STATE*	m_pModuleState;
#endif

    // IUnknown methods
    STDMETHOD(QueryInterface) (REFIID riid,void ** ppv);
    STDMETHOD_(ULONG, AddRef) ();
    STDMETHOD_(ULONG, Release) ();

    // IBindStatusCallback methods
    STDMETHOD(OnStartBinding)(DWORD grfBSCOption, IBinding* pbinding);
    STDMETHOD(GetPriority)(LONG* pnPriority);
    STDMETHOD(OnLowResource)(DWORD dwReserved);
	STDMETHOD(OnProgress)( 
            /* [in] */ ULONG ulProgress,
            /* [in] */ ULONG ulProgressMax,
            /* [in] */ ULONG ulStatusCode,
            /* [in] */ LPCWSTR szStatusText);
    STDMETHOD(OnStopBinding)(HRESULT hrResult, LPCWSTR szError);
    STDMETHOD(GetBindInfo)(DWORD* pgrfBINDF, BINDINFO* pbindinfo);
    STDMETHOD(OnDataAvailable)(DWORD grfBSCF, DWORD dwSize, FORMATETC *pfmtetc,
                        STGMEDIUM* pstgmed);
    STDMETHOD(OnObjectAvailable)(REFIID riid, IUnknown* punk);

#endif



};


//
// GFileRef		manages loading of an url / file 
//

class GFileRef
{
public:
	GFileState state;
	GFile *loader;		//!< url loader if processing
	int urlI;			//!< the current loaded url for MFUrls

	CString home;		//!< the url home
	CString localFile;	//!< file name of local file (could be a cache file=
	GLocalFileClass localFileClass; //<! the class of the local file	
	
	int localFileFmt;	//!< file  format 

	CString baseUrl;	//!< the base URL for resolving relative url's
	
	CTime checkTime;	//!< URL cache check time for children	



public:

	GFileRef() : loader(NULL),urlI(0),state(S_NOT_LOADED),localFileClass(GLFC_UNKNOWN),localFileFmt(-1) {}
	~GFileRef() { Release(); }

	//! get current state of processing 
	GFileState GetState() {
			if (loader) state = GetStateFromLoader();
			return(state);
	};

	//! is the state ok i.e. S_PROCESSED
	gbool Ok() const { return (state == S_PROCESSED); }

	//! set the base url 
	void SetBaseUrl(const char *url) {
		baseUrl = url;
	}

	//! get the base url 
	const char * GetBaseUrl() const  {
		return baseUrl;
	}
	
	gbool HasBaseUrl() const {
		return baseUrl.GetLength()>0;
	}


	// get the current state from loader
	GFileState GetStateFromLoader();

	int DeleteCachedLocalFile() 
	{
		if (loader) return loader->DeleteCachedLocalFile();
		return GFile::DeleteCachedLocalFile(localFileClass, localFile);
		//return 0;
	}

	//! loader no longer needed, release it
	void Release() {
		if (loader) loader->unref();
		loader = NULL;
	};
	
	//! set state to processed
	void SetProcessed() {
		Release();
		state = S_PROCESSED;
	}

	//! set state to "no url available"
	void SetNoUrl() {
		Release();
		state = S_NO_URL;
	}

	//! set state to "unexpected / bad url data"
	void SetBadData() {
		Release();
		state = S_BAD_DATA;
	}

	//! reset set state to "not loaded, start loading if needed"
	void SetNotLoaded() {
		Release();
		urlI = 0; // reset to first url element
		state = S_NOT_LOADED;
	}

	//! get local file name 
	const char *GetLocalFile() const;

	//! get file type
	int GetLocalFileFmt() const;

	//! get absolute home url/filename 
	const char *GetHomeUrl() const;

	//! get absolute  url/filename (only if loader is present
	const char *GetUrl() const;


	//! start loader operation 
	//! and return the state 
	GFileState Load(const char *home, // the home url 
				const char *url,		// the relative url/file
				const char *refedByUrl,	// who has referenced this url
				const char *refedByType, // what type of referee
				int wwwFlags,		// wwwFlags 
				HWND hPostMsgWnd,	// window to notify on completition
				int currentUrlI=0,		// the current url Index for MF-Urls´s
				CTime checkTime=0,	 // URL cache check time	
				GReporter *reporter = NULL, // the error reporter
				GFile *rootLoader = NULL // GFile to get status callbacks from 
				);	

	//! setup loader without loading , in order to get url 
	GFileState 
	      SetupUrl(const char *urlHome, // the home url 
				const char *url,		// the relative url/file
				const char *refedByUrl,	// who has referenced this url
				const char *refedByType, // what type of referee
				int wwwFlags,		// wwwFlags 
				HWND hPostMsgWnd,	// window to notify on completition
				int currentUrlI,		// the current urli for MF-Urls´s
				GReporter *reporter, // the error reporter

				GFile *rootLoader,
				
				CString &extraArg	// retzrn : the # argument
				
				);	


};


//
// Message to post to a window when thread is done.
//
#define WM_READFILECOMPLETED (WM_USER + 100)

// load request message for netscape plugin-window
#ifndef  WM_NETSCAPE_LOAD_URL
#define WM_NETSCAPE_LOAD_URL WM_USER + 5001
#endif


//   Return values for GetWebPageWorkerThread
const int THREAD_GOOD = 0 ;
const int THREAD_BAD = -1 ;


// is url a www file
int IsUrlFile(const char* url); 
// is url a local file
int IsLocalFile(const char* url); 

int UrlGetLocalFileName(const char* url,char *FileName); 

int WWWWriteUrlToFile(const char *url,const char *file,int writeDescription=0); 



#ifdef WIN32
// needed functions from urlmon.dll (urlmon.h)

typedef HRESULT (*URLDownloadToCacheFileAProc)(LPUNKNOWN,LPCSTR,LPTSTR,DWORD,DWORD,LPBINDSTATUSCALLBACK);                  

typedef HRESULT (*HlinkSimpleNavigateToStringProc)(                                                                        
    /* [in] */ LPCWSTR szTarget,         // required - target document - null if local jump w/in doc       
    /* [in] */ LPCWSTR szLocation,       // optional, for navigation into middle of a doc                  
    /* [in] */ LPCWSTR szTargetFrameName,// optional, for targeting frame-sets                             
    /* [in] */ IUnknown *pUnk,           // required - we'll search this for other necessary interfaces    
    /* [in] */ IBindCtx *pbc,            // optional. caller may register an IBSC in this                  
    /* [in] */ IBindStatusCallback *,                                                                      
    /* [in] */ DWORD grfHLNF,            // flags                                                          
    /* [in] */ DWORD dwReserved          // for future use, must be NULL                                   
);         

typedef HRESULT (*CoGetClassObjectFromURLProc)( REFCLSID rCLASSID,
            LPCWSTR szCODE, DWORD dwFileVersionMS, 
            DWORD dwFileVersionLS, LPCWSTR szTYPE,
            LPBINDCTX pBindCtx, DWORD dwClsContext,
            LPVOID pvReserved, REFIID riid, LPVOID * ppv);
 
// static function variables for lib functions 
extern URLDownloadToCacheFileAProc URLDownloadToCacheFileH;
extern HlinkSimpleNavigateToStringProc HlinkSimpleNavigateToStringH;
extern CoGetClassObjectFromURLProc CoGetClassObjectFromURLH;


extern HINSTANCE urlmonLib;

/*
InitializeUrlmonLib
initialize the IE specific urlmon.dll and find the entry points we are interested in
Goal : don't request the dll during startup, only if need 

*/
BOOL InitializeUrlmonLib();
void TermUrlmonLib(); 

#endif



#endif
