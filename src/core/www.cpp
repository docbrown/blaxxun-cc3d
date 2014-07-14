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

@module WWW.cpp - GLView WWW HTTP support 	|

Copyright (c) 1995-1998	by Holger Grahn
All rights reserved

Purpose:
GFile abstraction provides url/file abstraction with 
	HTTP,  caching unzip support 

Netscape DDE SUPPORT 

Classes:
GFile
GFileRef

Notes:
	using urlmon UrlDownloadToCacheFile
	implementaion based on Microsoft wininet dll uncommented

Changes:
	5.05.97 added ZLIB gunzip support
	01.02.98 HG gzip check, IE returns wogel.wav.gz ==> wogel_wav.wav !!!!!!!!!!!!!!!
	25.03.98 url cache support
	30.09.98 resolved Problem with VC 6.0 changed wininet header file

$Revision: 1.32 $
$Log: www.cpp,v $
Revision 1.32  1998/10/02 14:57:28  holger
empty message

Revision 1.29  1998/07/02 17:46:45  holger
empty message

Revision 1.28  1998/07/01 16:48:53  holger
empty message



Todo :
	WIN32 dependent 



******************************************************************************/

#include "stdafx.h"
#include <assert.h>
#include <string.h>
#include <time.h>

#define _WWW



#ifdef WIN32

#include <Process.h>
#include <Shellapi.h>										 
#include <ddeml.h>

#endif 


#include "gversion.h"
#include "gconfig.h"

#include "gtrace.h"
#include "gurlcache.h"


#include "www.h"


#include "greporter.h"

#ifndef _GLVIEW_DLL			
#include "resource.h"
#include "DWWW.h"
#endif

// use zlib to unzip 
#define GUSE_ZLIB

#ifdef GUSE_ZLIB
#include "gunzip.h"
#endif 



#ifdef _GSOCK
#include <afxsock.h>

static WSADATA	WWWwsadata;
static int WWWinit =0;
static int WWWnotAvailable =0;
#else

#if defined(_WIN32)
// need this for error constants
#include "winsock.h"
#endif 
#endif 

//INVALID_PORT_NUMBER INTERNET_INVALID_PORT_NUMBER

int WWWabort=0;
int WWWkeepConnection=0;

// flag if files from cache can be used
int WWWuseFilesFromCache = 1;

// mode flag how cache files are generated
int WWWCacheFileFmt=1; // long format


CString WWWemail;
CString WWWbrowserPath;


// CString  acceptTypes = "x-world/x-vrml;world/vrml;image/*;*/*";

 const char* acceptTypes[] = { "x-world/x-vrml", 
								"model/vrml", 
								"image/*",
								"application/x-gzip",
								"audio/*",
								"*/*" ,
								NULL
							 };




HWND GFile::hGlobalPostMsgWnd = NULL; // the  global window, to send  thread completition messages to 

BOOL GFile::useInetCache=TRUE;

#ifdef G_WININET
 // WINDOWS Internet functions parameters
CString GFile::userAgent= _PROGRAM "/" _VERSION " " _PROGRAM_TYPE;

DWORD GFile::accessType=  INTERNET_OPEN_TYPE_PRECONFIG; 

CString  GFile::proxyServer;	
CString  GFile::proxyByPass;	// new active x sdk

//INTERNET_PORT GFile::proxyPort=INTERNET_INVALID_PORT_NUMBER;	// no longer in active X SDK

DWORD GFile::internetOpenFlags=0;

CString GFile::ftpUserName;
CString GFile::ftpPassword;

#endif


#ifdef G_WININET
HINTERNET GFile::hSession=NULL; 
#endif

GFile *GFile::filesInUse = NULL; // static list of GFiles in uses 


DWORD GFile::lastReportedInternetError=0; // last error reported 
int GFile::lastReportedFileError=0; // last file error reported 
BOOL  GFile::dontReportErrors = FALSE;; // last error reported 

static CRITICAL_SECTION fileSection;  
static BOOL fileSectionInit = FALSE;

void LockFile() 
{
	// Initialize the critical section.
	if (!fileSectionInit) { 
		InitializeCriticalSection(&fileSection); 
		fileSectionInit = TRUE;
		//TRACE("init section\n");
	}
	//TRACE("Enter section\n");

    EnterCriticalSection(&fileSection); 
    // Access the shared resource.

}


// 
// use registry to find list of UMEL directories
//
gbool GetUmelPath(CString &path)
{
#ifdef WIN32

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


    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, cpath, 0, KEY_READ, &hkey) != ERROR_SUCCESS){
		return gfalse;
    }
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
 
	val.TrimRight();

	path = val; 			
	TRACE("Reg found UMEL :%s \n" ,(const char *) val);
	return (path.GetLength() >= 0);

#else

    return( gfalse );

#endif
}



void UnlockFile() 
{
	//TRACE("Leave section\n");
    // Release ownership of the critical section.
    LeaveCriticalSection(&fileSection);
}



// add m to files in use list 
void GFile::AddFileInUse(GFile *m)
{

	//TRACE("Add file in use %p %s \n",m,m->url);

	ASSERT(m->next == NULL);
	ASSERT(m->prev == NULL);

	
	LockFile();

    if (!filesInUse) filesInUse = m;
    else  {
      m->next = filesInUse->next;
      m->prev =  filesInUse;
      if (filesInUse->next)
        filesInUse->next->prev = m;
      filesInUse->next = m;
    }
	UnlockFile();
}

// remove m from files in use list 
void GFile::RemoveFileInUse(GFile *m)
{
	TRACE("remove  file in use %p %s \n",m, (const char * )m->url);
	ASSERT(m != NULL);


	LockFile();

    if (m == filesInUse) {  // must be first
        filesInUse= m->next;
        if (filesInUse) filesInUse->prev = NULL;
		m->next = m->prev = NULL;
    } else {
	  // not part of list if prev == NULL, otherwise must be first 
	  if (m->prev == NULL) { 
	  } else {	
		m->prev->next = m->next;
		if (m->next) m->next->prev = m->prev;
  		m->next = m->prev = NULL;
	  }
    }
	UnlockFile();
}

// kill all files in use 
void GFile::KillFilesInUse()
{
	LockFile();


    GFile *i= filesInUse;
	int cnt=0;

    
    while(i != NULL) 
    {
		// TRACE("kill file in use %p %s \n",i,i->url);
		GFile *n = i->next;
        i->Kill();
        i= n;
		cnt++;
    }
	UnlockFile();
	// TRACE("%d files  in use \n",cnt);
}

// kill all files in use using myWnd as notifaction target
void GFile::KillFilesInUse(HWND myWnd)
{
	LockFile();

    GFile *i= filesInUse;
	int cnt=0;
    
    while(i != NULL) 
    {
		GFile *n = i->next;
		if (i->hPostMsgWnd == myWnd) {
			TRACE("GFile::kill file in use for window %p %s \n",myWnd, (const char *)i->url);
			i->Kill();
			cnt++;
		}
        i= n;
    }
	UnlockFile();
	TRACE("%d GFile threads were/are in use for window \n",cnt);
}

int NumFilesInUse()
{
	LockFile();

    GFile *i= GFile::filesInUse;
	int cnt=0;
    
    while(i != NULL) 
    {
		GFile *n = i->next;
		cnt++;
        i= n;
    }
	UnlockFile();
	TRACE("%d GFile threads were/are in use \n",cnt);
	return cnt;
}

/*

  Netscape download notification 

*/


gbool GFile::OnUrlNotify(LPCTSTR url, LPCTSTR mimeType,time_t lastModified, LPCTSTR fileName, GLocalFileClass fileClass,long reason) 
{
	if (!(this->url == url)) 
		return FALSE;
	
	if (mimeType && *mimeType != 0)
		contentType = mimeType;
	this->lastModified = lastModified;
	this->lastChecked = CTime::GetCurrentTime();

	SetLocalFile(fileName,fileClass); // this will set the localFileFmt based on extension
	

	// check content type for localFileFmt
	if (contentType.GetLength()>0) 
	{
	   TRACE("MIME TYPE : %s\n",(const char *) contentType);

	   int type = ParseMimeType(contentType);
	   
	   // Netscape reports text/plain for png 
	   if (type == UNKNOWN_FMT || type == GZIP_FMT || type >= HTML_FMT) {
		    // TRACE("Unknown Mime Type %s type %d type from filename %d \n",contentType,type,localFileFmt);
			if (localFileFmt != UNKNOWN_FMT) type = localFileFmt; 
	   }
	   localFileFmt = type;

	}


    if (localFileFmt == UNKNOWN_FMT) 
		ProcessExtension(localFile);

	if (localFileFmt <= 0) {  // check file header if format still unknown
		FILE *f;
		f=fopen(localFileUnzipped,"rb");
		if (f) {
			char buf[120];
			if (fread(buf,1,40,f) == 40)
				localFileFmt=GetFileType(buf,40);
			fclose(f);
		}
	}



	threadRet = - reason;
	if (reason == 0) {
		urlLoaded = 1;

#ifdef G_CACHE

		// save file in cache 	
		if (!Flag(WWW_DONT_CACHE)) {
			theCache.SaveUrl(url,localFile,lastModified,localCacheFile);
		}

#endif

	}

#ifdef WIN32

	// send completition message to window
	threadBusy=2;

	if (hPostMsgWnd) {
	  ref(); 	// receiving window must do the unref 
      ::PostMessage(hPostMsgWnd,WM_READFILECOMPLETED, (WPARAM) threadRet, (LPARAM) this) ;
	} else 
	if (hGlobalPostMsgWnd) {
	  ref(); 	// receiving window must do the unref 
	  ::PostMessage(hGlobalPostMsgWnd,WM_READFILECOMPLETED, (WPARAM) threadRet, (LPARAM) this) ;
	}
	else 
	  threadBusy=0;

#else

    threadBusy=0;

#endif 

//	InterlockedDecrement(&GFile::threadsInUse); 

	return TRUE;
}


gbool GFile::OnUrlNotify(HWND myWnd, LPCTSTR url, LPCTSTR mimeType,time_t lastModified, LPCTSTR fileName, GLocalFileClass fileClass, long reason)
{
	//LockFile();

    GFile *i= filesInUse;
	int cnt=0;
    
    while(i != NULL) 
    {
		GFile *n = i->next;
		if (i->hPostMsgWnd == myWnd) {
		
			if (i->OnUrlNotify(url,mimeType,lastModified,fileName,fileClass,reason)) {
				ASSERT(cnt==0); // diagnostic debug assert only 
				cnt++;
				// break; hg 15.11.99 could be multiple requests ??
			}
		}
        i= n;
    }
	//UnlockFile();
	return cnt>0;


}


    

void GFile::ClearSessionError() 
{ 
	lastReportedInternetError = 0; 
	lastReportedFileError = 0; 
	dontReportErrors = FALSE; 
} 



// how many bytes to read in one request
#define BUF_SIZE 4096

                 	
int GFile::maxThreads=7;  
gbool GFile::useThreads=gtrue;

LONG GFile::threadsInUse=0;
 
CString GFile::cacheDirectory;
 
long GFile::cacheMaxSize=16 *1024*1024;
long GFile::cacheCurSize = 0;
	
long GFile::cacheDownloadSize=0;
 
 void GFile::DeleteCache(int options)
 {
 }



int GetURLFromLocalFile(const char * szLocal, char * szURL);
int AskNetscapeToOpenURL(const char * szURL);


// find WWW Browser 
// use registry to find shell open command for Netscape
//
int FindWWWBrowser(CString &browserPath)
{
#ifdef WIN32

	if (browserPath.GetLength() == 0) {
		CString key = "NetscapeMarkup\\shell\\open\\command";
		CString val;
		long lSize = _MAX_PATH;
		long ret =::RegQueryValue(HKEY_CLASSES_ROOT, key, val.GetBuffer(_MAX_PATH),&lSize); 
        val.ReleaseBuffer();
        if (ret == ERROR_SUCCESS && val.GetLength()>0)
		{
			int arg = val.Find("\"%1\"");

			if (arg>0) val=val.Left(arg);
			else {
				arg = val.Find("%1");
				if (arg>0) val=val.Left(arg);
			}
			
			val.TrimRight();

			browserPath = val; 			
			TRACE("Reg found Netscape :%s \n" ,(const char *) val);
		}
	}
	return (browserPath.GetLength() >= 0);

#else

    return 0;

#endif
}



// flag if loading in progress
int busy=0;
// busy check

int Busy(int msg) 
{
	if (busy>0) {
	  if (msg) {
	  	Reporter.Warning("Sorry, system is busy please wait");
	  }	
	  return(1);
	}
	return(0);
}


static const char* prefix_http = "http://";
static const char* prefix_file = "file://";

// GLView known file extensions / formats
struct { GFileFormat type; char *ext; } fileExtensions[]= 
{

HMF_FMT,"hmf",
GEO_FMT,"geo",
D3V_FMT,"3dv",
D3V_FMT,"3dz",
RAW_FMT,"raw",
OBJ_FMT,"obj",
STL_FMT,"stl",
DXF_FMT,"dxf",
POV_FMT,"pov",

BXM_FMT,"bxm",

VRML_FMT,"wrl",
VRML_FMT,"wgz",
VRML_FMT,"wrz",
VRML_FMT,"vrml",
VRML_FMT,"iv",

VRML_FMT,"bx3", // ??
VRML_FMT,"bxwrl", // ??

DIB_FMT,"dib",
DIB_FMT,"bmp",
RGB_FMT,"rgb",

GIF_FMT,"gif",

TGA_FMT,"tga",
TGA_FMT,"targa",

JPEG_FMT,"jpg",
JPEG_FMT,"jpeg",

PPM_FMT,"ppm",

PNG_FMT,"png",

PCX_FMT,"pcx",

AVI_FMT,"avi",
MPG_FMT,"mpg",
MPG_FMT,"mpeg",
MPG_FMT,"mpe",

MPG_FMT,"ivf", // indeo ?
MPG_FMT,"asx", 
MPG_FMT,"asf", 

MPG_FMT,"wma", 


//MPG_FMT,"rav", // indeo ?
//MPG_FMT,"rm",  // real ?


QT_FMT, "mov", 
QT_FMT, "qt", 

MPG_FMT,"mp1", 
MPG_FMT,"mp2", 
MPG_FMT,"mpv2", 
MPG_FMT,"mpv", 
MPS_FMT,"mp3", // SOUND
MPS_FMT,"mpa", // SOUND
MPG_FMT,"mp4", 

MPS_FMT,"lma", // SOUND ??
//MPS_FMT,"mp3", // SOUND

RMA_FMT,"mp3", // SOUND

RMA_FMT,"ra",  
RMA_FMT,"ram",  //audio/x-pn-realaudio
RMA_FMT,"rmm", 
RMA_FMT,"rav", // real ?
RMA_FMT,"rm",  // real ?
RMA_FMT,"rmp",  // real ?
RMA_FMT,"rm",  // real ?
RMA_FMT,"rv",  // real ?
RMA_FMT,"smil", // real ?
RMA_FMT,"smi", // real ?


#ifdef _DEBUG
// MPG_FMT,"mid", // SOUND, test AMOVIE 
// MPG_FMT,"rmi", // SOUND, test AMOVIE 
#endif

WAV_FMT,"wav",
AU_FMT,"au",
AU_FMT,"snd",

MIDI_FMT,"mid",
MIDI_FMT,"midi",
MIDI_FMT,"rmi", // ??? 

JS_FMT,"js",
JS_FMT,"vs", // vrml script

GZIP_FMT,"gz",

D3M_FMT,"3dm",
HTML_FMT,"htm",
HTML_FMT,"html",

UNKNOWN_FMT,NULL
};


// find pointer to extension into fileName
const char * GetExtp(const char *fileName)
{
    if (!fileName) return(NULL);

	const char *extp;
	extp = strrchr(fileName,'.');
	if (!extp) return(extp);

	const char *del = strrchr(fileName,'_');  // path is transformed into www.host.com_aa_bb_cc
	if (extp < del) return(NULL);
	
	del = strrchr(fileName,'/');
	if (extp < del) return(NULL);
	del = strrchr(fileName,'\\');
	if (extp < del) return(NULL);
	return(extp);
}

// check if data has a html signature
int IsHtmlData(const char *data) 
{
   if (!data) return(0);
   if (strprefix(data,"HTTP/")) return 1;
   if (data[0]== '<') {
   	if (strprefix(data,"<!")) return 1;
   	if (strprefix(data,"<HTML>")) return 1;
   	if (strprefix(data,"<TITLE>")) return 1;
   	if (strprefix(data,"<HEAD>")) return 1;
  }
  return(0);
}


// check if data has the gzip signature
int IsZippedData(const char *data) 
{
	if ((data[0]== ((char )0x1F) && data[1]== ((char) 0x8B))) return(1);
	return(0);
}


// check if data is a redirect HTML output 
int IsRedirectedUrl(const char *data,char* newUrl) 
{
  if (strstr(data,"document has moved") || strstr(data,"can be found")) {
 	char *p = strstr(data,"href=\"");
 	if (!p) p = strstr(data,"HREF=\"");
 	if (!p) p = strstr(data,"HRef=\"");
	if (p) {
	   p+=6;
	   char *p1 = strchr(p,'"');
	   if (p1) {
		  strncpy(newUrl,p,p1-p);
		  newUrl[p1-p]=0;
		  return(1);
	   }
	}
  }
  return(0);
}



// parse extenions and return a known filetype id
GFileFormat GetFileType(const char *fileName) 
{
	const char *extp;

	// get the extension 
	extp = GetExtp(fileName);

	if (!extp) return(UNKNOWN_FMT);

	CString ext(extp+1);
	ext.MakeLower();
	int i=0;
	if (ext == "gz") {
	}	
	while (fileExtensions[i].ext) {
		if (ext == fileExtensions[i].ext)
 		return(fileExtensions[i].type);
		i++;
	}
	return(UNKNOWN_FMT);
}

// get file type from file extension
GFileFormat GetFileTypeExtension(const char *extension) 
{

if (!extension) return(UNKNOWN_FMT);

CString ext(extension);
ext.MakeLower();
int i=0;
while (fileExtensions[i].ext) {
 if (ext == fileExtensions[i].ext)
 	return(fileExtensions[i].type);
 i++;
}
return(UNKNOWN_FMT);
}


// estimated file type from first data bytes of file
GFileFormat 
GetFileType( const char * data,int num) 
{
	/* place to hold the signiture string for a png file. */
	static unsigned char png_sig[8] = {137, 80, 78, 71, 13, 10, 26, 10};
	if (num>=8) {
	   if (memcmp((const unsigned char *)data, png_sig, 8)==0) return (PNG_FMT);
	}
	
	if ((data[0]== ((char )0x1F) && data[1]== ((char) 0x8B)))  // check gz sig
			return GZIP_FMT;
	
	if (strncmp(data,"#VRML",5) == 0) return VRML_FMT;
	if (strncmp(data,"GIF",3) == 0) return GIF_FMT;

	return(UNKNOWN_FMT);

}




// check if this is a zipped file

int IsZippedFile(const char *fileName) 
{
	const char *extp;
	
	extp = GetExtp(fileName);
	if (!extp) return(0);

	CString ext(extp+1);
	ext.MakeLower();

	if (ext == "z" || ext == "gz") return(1);
	if (ext == "wrz" || ext == "wgz") return(1);
	return(0);
}

// check if file fileName does exists
// 1 ok, 0 not found
int ExistsFile(const char *fileName)
{ int ret = 0;

#ifdef WIN32
	HANDLE f = CreateFile(fileName,GENERIC_READ,FILE_SHARE_READ,
    		NULL,	 // address of security descriptor,
    		OPEN_EXISTING, 
			FILE_FLAG_SEQUENTIAL_SCAN,NULL);	

	 if  (f == INVALID_HANDLE_VALUE) {
		 	int err = GetLastError();
			ret = 0;
	 } else {
		   CloseHandle(f);
		   ret = 1;
	 }
#else
	 FILE *f = fopen(fileName,"r");
	 if (f) {
		 ret = 1;
		 fclose(f);
	 } else 
		 ret =0;
#endif

  return(ret);
}

/*
BOOL GetFileInformationByHandle(
    HANDLE hFile, 	// handle of file 
    LPBY_HANDLE_FILE_INFORMATION lpFileInformation 	// address of structure 
   )

ypedef struct _BY_HANDLE_FILE_INFORMATION { // bhfi  
    DWORD    dwFileAttributes; 
    FILETIME ftCreationTime; 
    FILETIME ftLastAccessTime; 
    FILETIME ftLastWriteTime; 
    DWORD    dwVolumeSerialNumber; 
    DWORD    nFileSizeHigh; 
    DWORD    nFileSizeLow; 
    DWORD    nNumberOfLinks; 
    DWORD    nFileIndexHigh; 
    DWORD    nFileIndexLow; 
} 


WIN32_FIND_DATA structure describes a file found by the FindFirstFile or FindNextFile function. 
typedef struct _WIN32_FIND_DATA { // wfd  
    DWORD dwFileAttributes; 
    FILETIME ftCreationTime; 
    FILETIME ftLastAccessTime; 
    FILETIME ftLastWriteTime; 
    DWORD    nFileSizeHigh; 
    DWORD    nFileSizeLow; 
    DWORD    dwReserved0; 
    DWORD    dwReserved1; 
    TCHAR    cFileName[ MAX_PATH ]; 
    TCHAR    cAlternateFileName[ 14 ]; 
} WIN32_FIND_DATA; 
 

  HANDLE FindFirstFile(
    LPCTSTR lpFileName,	// pointer to name of file to search for  
    LPWIN32_FIND_DATA lpFindFileData 	// pointer to returned information 
   );

BOOL FindNextFile(
    HANDLE hFindFile,	// handle to search  
    LPWIN32_FIND_DATA lpFindFileData 	// pointer to structure for data on found file  
   );
BOOL FindClose(
    HANDLE hFindFile 	// file search handle 
   );

 */




/* 	
	Open zipfile as 
	stream
*/


extern "C" int G__cdecl _open_osfhandle(
        long osfhandle,
        int flags
        );

#ifdef WIN32

// unzip file to a temporary one
FILE * GZipOpen(const char * fileName, const char *mode)
{	CString cmd;
	int ret = 0;
	int unzipRet = 0;

     // add filename quoted
    cmd = "gzip.exe -dc \"";  
    cmd += fileName;
    cmd += "\"";


	CString directory;
	HANDLE hRead;
	HANDLE hWrite;

	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	ret = CreatePipe(&hRead,&hWrite,&sa, 32*1024); 

	GetCurrentDirectory(_MAX_PATH,directory.GetBuffer(_MAX_PATH));
	directory.ReleaseBuffer();
	unzipRet = -1;


	PROCESS_INFORMATION pinf;
	STARTUPINFO pstart;
		memset(&pstart,0,sizeof(pstart));
		memset(&pinf,0,sizeof(pinf));
		pstart.cb = sizeof(pstart);

		pstart.dwFlags |= STARTF_USESTDHANDLES;
		pstart.hStdOutput = hWrite;

		if (CreateProcess(
			NULL,cmd.GetBuffer(_MAX_PATH),
			NULL,NULL,
    		TRUE,	// handle inheritance flag 
    		NORMAL_PRIORITY_CLASS,	// creation flags 
    		NULL,	// pointer to new environment block 
    		(const char *)directory,	// pointer to current directory name 
    		&pstart,	// pointer to STARTUPINFO 
			&pinf
   			)) 
		{

		// put in internal file table 
		int h = _open_osfhandle((long) hRead,0);

		// associate stream
		return(fdopen(h,"r"));

		}
		return (NULL);

		
		
#if 0
		{

   		unzipRet = WaitForSingleObjectEx(
    		pinf.hProcess,	// handle of object to wait for 
    		INFINITE,	// time-out interval in milliseconds 
    		FALSE 	// return to execute I/O completion routine if TRUE  
	    );	

    GetExitCodeProcess(
    	pinf.hProcess,
    	(LPDWORD)  &unzipRet 	// address to receive termination status 
   	);	
	
	if (!CloseHandle(pinf.hProcess))  {	// handle of object to close  
   		TRACE("Can´t close handle to process\n");;
    } 
	}
    
	if (unzipRet != 0)
    {
   		Reporter.Error("Can´t execute '%s'.",(const char *) cmd);
    }
 return(ret);

#endif

}

#endif



/*
int UnzipToTmpFile(const char *fileName, CString &tmpFile)
{
   const char *name;

   if( ( name = _tempnam( "c:\\tmp", "glv" ) ) != NULL )
      tmpFile = tmp;
   else
      return(0);

}

*/

#if 0

// doesn´t work for non console application, problem of
// stdin / out handles
extern "C" int G__cdecl _set_osfhnd(int,long);


FILE * GZipOpen(const char * fileName, const char *mode)
{
 	CString cmd;
 	// add filename quoted
 	cmd = "gzip.exe -dc \"";
 	cmd += fileName;

 	cmd += "\"";
	
	HANDLE hHandle=	 GetStdHandle(STD_INPUT_HANDLE);

	// low level open 
	//FILE *_fdopen( int handle, const char *mode );


	int ret=0;
	HANDLE f = CreateFile("tmp.out",GENERIC_WRITE,FILE_SHARE_WRITE,
    		NULL,	 // address of security descriptor,
    		CREATE_ALWAYS, 
			FILE_FLAG_SEQUENTIAL_SCAN,NULL);	

		 if  (f ==INVALID_HANDLE_VALUE) {
		 	int err = GetLastError();
			ret = 0;
		 } else {
		   //CloseHandle(f);
		   ret = 1;
		 }

	hHandle = f;

	_set_osfhnd(1,(long) hHandle);
	
	ret = SetStdHandle(STD_OUTPUT_HANDLE,hHandle);

	//BOOL ret = SetStdHandle(STD_OUTPUT_HANDLE,HANDLE hHandle);


	
	TRACE("Zipped fopen %s \n",(const char *) cmd);

													// mode 
    FILE *infile = _popen((const char *) cmd, "rt");  /* use "w" for zwrite */

	if (!infile) 
	    Reporter.Error("Can´t open pipe %s (%s) ",(const char *) cmd,mode);

	return infile;
}

#endif

#ifdef WIN32


FILE * fopenZ(const char * fileName, const char *mode)
{
    int isZipped = IsZippedFile(fileName);

	if (isZipped)
	   return GZipOpen(fileName,mode);
	else  {
		FILE * f= fopen(fileName,mode);
		if (!f) return(f);
		// try if this is a gzipped file
		int c1,c2;
		c1 = fgetc(f);
		c2 = fgetc(f);
		fseek(f,0,SEEK_SET);
		if (c1==0x1F && c2==0x8B) {
			fclose(f);
			int fmt;
			char newName[_MAX_PATH];
			int ret;

#ifdef GUSE_ZLIB
			char fileNameUnzipped[_MAX_PATH];
			strcpy(fileNameUnzipped,fileName); 
			strcat(fileNameUnzipped,".uz"); 

			// create a temp cache file name 
			CString tmp;
			UrlGetLocalFileName(fileName,tmp.GetBuffer(_MAX_PATH));
			tmp.ReleaseBuffer();

			if (GFile::cacheDirectory.GetLength()>0) {
				int l = GFile::cacheDirectory.GetLength();

				if (strncmp(fileName,GFile::cacheDirectory,l) == 0) {
					tmp = fileNameUnzipped; // its already a temp file 
				}
				else {
					if (GFile::cacheDirectory[l-1] != '\\')
						GFile::cacheDirectory += '\\';

					tmp =	GFile::cacheDirectory + tmp;
				}
			}
			strcpy(fileNameUnzipped,tmp); 

			ret = gz_file_uncompress(fileName,fileNameUnzipped,TRUE);

			FILE * f= fopen(fileNameUnzipped,mode);
			return(f);


#else 


			CString newZipped;
			newZipped=fileName;
			newZipped += ".gz";

			ret = rename(fileName,newZipped);
			if (ret !=0 ) {
				Reporter.Error("Cant rename %s to %s for unzip \n",fileName,(const char *) newZipped);							
				return(NULL);
			}

			ret= UnZipFile(newZipped,newName,fmt,TRUE,TRUE);
			FILE * f= fopen(newName,mode);
			return(f);

#endif
			

			// return GZipOpen(fileName,mode);
		}
		else return(f);
	}
}
#endif


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
			  BOOL keepSourceFile)
{ 

 const char *fileName = fileNameArg;


 int ret = 0;
 int unzipRet = 0;

 strcpy(fileNameUnzipped,fileName); 


 	

 // pointer to extension in fileNameUnzipped
 char *extp;
 extp = (char *) GetExtp(fileNameUnzipped);

 CString ext;

 if (extp == NULL) {
	 extp = &fileNameUnzipped[strlen(fileNameUnzipped)];
	 ext = "";
 }	
 else 
   ext = extp+1;
 
 ext.MakeLower();

 int isZipped = 0;

 if (ext == "z" || ext == "gz") {
 	isZipped = 1;
	*extp = 0;	// cut string 
 }
 else if (ext.GetLength() == 3) {
	if ((ext == "wgz") || (ext == "wrz")) {
		strcpy(extp,".wrl");	// append to filenameUnzipped 

#ifndef GUSE_ZLIB
		CString newZipped;
		newZipped=fileNameUnzipped;
		newZipped += ".gz";
		ret = rename(fileName,newZipped);
		if (ret !=0 ) return(-1);
		fileName = newZipped;
#endif 
		isZipped = 1;
	}
 	else { 
 		//if (ext[2] == 'z')  
	   isZipped =0;
    }	
 }
 ret = 0;


	BOOL verifyIfZipped = !isZipped;

	if (verifyIfZipped) {  // check file if zipped 

		FILE *f;
		f=fopen(fileName,"rb");

		if (f) {
			char buf[120];

			if (fread(buf,1,20,f) == 20) {
				if (strncmp(buf,"#VRML",5) == 0) {
					fmt = VRML_FMT;
				}
				else if ((buf[0]== ((char )0x1F) && buf[1]== ((char) 0x8B))) { // check gz sig
					isZipped = 1;
				}
			}

			fclose(f);
		}
	}




    if (isZipped)  { 	// do unzip

 	if (ExistsFile(fileName)) {

#ifdef GUSE_ZLIB

	if (useTempFile) {
		// create a cache file name 
		CString tmp;
		UrlGetLocalFileName(fileNameUnzipped,tmp.GetBuffer(_MAX_PATH));
		tmp.ReleaseBuffer();

		if (GFile::cacheDirectory.GetLength()>0) {
			int l = GFile::cacheDirectory.GetLength();

			if (strncmp(fileNameUnzipped,GFile::cacheDirectory,l) == 0) {
				tmp = fileNameUnzipped; // its already a temp file 
			}
			else {
				if (GFile::cacheDirectory[l-1] != '\\')
					GFile::cacheDirectory += '\\';

				tmp =	GFile::cacheDirectory + tmp;
			}
		}
		strcpy(fileNameUnzipped,tmp); 
	}

	ret = gz_file_uncompress(fileName,fileNameUnzipped,TRUE);

	if (ret >=0) {
		// delete source files 
		if (!keepSourceFile) 
			unlink(fileName);
	}
#else 

	// use gzip command 
	// can`t pass outputfile to gzip
	CString cmd;     
	// add filename quoted
    cmd = "gzip.exe -dqf \"";  
    cmd += fileName;
    cmd += "\"";

	fmt = GetFileType(fileNameUnzipped); 
   	
   		Reporter.Status("Executing '%s'",cmd);
		busy++;

#ifdef WIN32
	CString directory;
	
  	GetCurrentDirectory(_MAX_PATH,directory.GetBuffer(_MAX_PATH));
	directory.ReleaseBuffer();
	unzipRet = -1;


	PROCESS_INFORMATION pinf;
	STARTUPINFO pstart;
		memset(&pstart,0,sizeof(pstart));
		memset(&pinf,0,sizeof(pinf));
		pstart.cb = sizeof(pstart);
		 //pstart.hStdOutput
		 // pstart.dwFlags |= STARTF_USESTDHANDLES;
		if (CreateProcess(
		NULL,cmd.GetBuffer(_MAX_PATH),
		NULL,NULL,

    	FALSE,	// handle inheritance flag 
    	DETACHED_PROCESS,	// creation flags 
    	NULL,	// pointer to new environment block 
    	(const char *)directory,	// pointer to current directory name 
    	&pstart,	// pointer to STARTUPINFO 
		&pinf
   		)) {

   	unzipRet = WaitForSingleObjectEx(
    	pinf.hProcess,	// handle of object to wait for 
    	INFINITE,	// time-out interval in milliseconds 
    	FALSE 	// return to execute I/O completion routine if TRUE  
    );	

    GetExitCodeProcess(
    	pinf.hProcess,
    	(LPDWORD)  &unzipRet 	// address to receive termination status 
   	);	
	
	if (!CloseHandle(pinf.hProcess))  {	// handle of object to close  
   		TRACE("Can´t close handle to process\n");;
    } 
	}
    
	if (unzipRet != 0)
    {
   		Reporter.Error("Can´t execute '%s'.",(const char *) cmd);
		ret = -1;
    }


#else  //  !WIN32
		//ret = WinExec((const char *)cmd,SW_SHOWNORMAL);
		//ret = _system(cmd);
		unzipRet =  _spawnlp(_P_WAIT, "gzip.exe","gzip.exe", "-dqf", fileName,NULL); // fileNameUnzipped,NULL);
		//int pid =  _spawnlp(_P_DETACH, "gzip.exe","gzip.exe", "-dqf", fileName,NULL); // fileNameUnzipped,NULL);
		//_cwait(&ret,pid,_WAIT_CHILD);
#endif

		busy--;
		Reporter.Status("");
#endif // ZLIB 

	
	if (unzipRet !=0) ret = -1;

	}  //ExistsFile(fileName)
	else {
        //  zipped file not present, already unzipped ?
    }

	//if (ret != 0) ret=-1; 
	//else 
	//ret = 0;
 };

 return(ret);
}



//
// GFileRef
//



GFileState 
GFileRef::GetStateFromLoader()
{  int ret;

    if (loader->Busy()) return(S_LOADING);

//	if (loader->isThread) {
		if (loader->isThread) TRACE("thread no longer  busy \n");
		loader->isThread = 0;
	
	    ret = loader->threadRet; // get return code from thread

		if (ret == ERROR_REDIRECTED_URL) {
			if (loader->url == loader->redirectUrl) {
					if (loader->reporter) loader->reporter->Error("Redirected url %s matches current !",(const char*) loader ->url);
					return(S_BAD_URL);
			} else {
				  loader->Set(loader->redirectUrl);
				  ret=0;

			}
		} 
		if (ret >=0) {
			// to do:ret = loader->ProcessLoad(ret);  // do rest of processing (urlLoaded == 1) 

	    	ret = loader->Process();  // do rest of processing (urlLoaded == 1) 
			
			localFile = loader->localFileUnzipped;
			localFileFmt = loader->localFileFmt;
			localFileClass = loader->localFileClass;

			home=loader->GetHomeUrl();

			if (ret == ERROR_THREAD_STARTED)
				return(S_LOADING);
	    }
		if (ret<0) {
			Release();
			if (ret == ERROR_BAD_URL)
				return S_BAD_URL;
			else return S_BAD_DATA;
		}
		else
			return (S_LOADED);
//	}
//	else return (S_LOADED);
}


// get local file name 
const char *GFileRef::GetLocalFile() const 
{
	if (loader) return (loader->localFileUnzipped);
	return (localFile);
}

// get file type
int GFileRef::GetLocalFileFmt() const 
{
	if (loader) return (loader->localFileFmt);
	return (localFileFmt);
}

// get absolute home url/filename 
const char *GFileRef::GetHomeUrl() const 
{
	if (loader) return (loader->GetHomeUrl());
	return (home);
}

// get absolute url/filename 
const char *GFileRef::GetUrl() const 
{
	if (loader) return (loader->GetUrl());
	return (NULL);
}


// start a load of the url
GFileState 
GFileRef::Load(const char *urlHome, // the home url 
				const char *url,		// the relative url/file
				const char *refedByUrl,	// who has referenced this url
				const char *refedByType, // what type of referee
				int wwwFlags,		// wwwFlags 
				HWND hPostMsgWnd,	// window to notify on completition
				int currentUrlI,		// the current urli for MF-Urls´s
				CTime checkTime,	 // URL cache check time	
				GReporter *reporter, // the error reporter
				GFile *rootLoader
				)	

{
	if (url == NULL || url[0] == 0) {
		state = S_NO_URL;
		return (state);
	}
	// too many threads busy
	if (GFile::useThreads && (GFile::threadsInUse > GFile::maxThreads)) {
	   		state = S_NOT_LOADED;
			// what if local ???
	   		return(state);
    }
/*
	// test for Netscape case 
	if (NumFilesInUse() >= 10) {
	   		state = S_NOT_LOADED;
			// what if local ???
	   		return(state);
	}
*/

	urlI = currentUrlI;
	ASSERT(loader == NULL);
	
	Release();


	loader = new GFile();
	loader->ref();
	loader->SetFlags(wwwFlags);



	if (rootLoader) { // copy basic from root loader  settings 
		if (rootLoader->reporter)
			loader->SetReporter(rootLoader->reporter);
		
#ifdef _COM 
		loader->client = rootLoader->client;
		loader->statusCallback = rootLoader->statusCallback;
		loader->hObserver = rootLoader->hObserver;

#endif

		loader->hPostMsgWnd = rootLoader->hPostMsgWnd;		
		loader->hNetscapeMsgWnd = rootLoader->hNetscapeMsgWnd;	
		loader->ifModifiedSince = rootLoader->ifModifiedSince; // use refresh time from parent last modified

	}

	CTime refreshTime = 0;


#ifdef G_CACHE

	refreshTime = theCache.getRefreshTime();

#endif

	if (checkTime > refreshTime) refreshTime = checkTime;


	// keep inlines consistent with cache settings
	// not done for textures sound's etc 
	if (streq(refedByType,"Inline") || streq(refedByType,"EXTERNPROTO")) {
		
		if (refreshTime > loader->ifModifiedSince) 
			loader->ifModifiedSince = refreshTime;
	}
	else {
		if (checkTime >0) { // refresh textures, if a check time is given 
			if (checkTime > loader->ifModifiedSince) 
				loader->ifModifiedSince = checkTime;
		}
	}

	if (reporter)
		loader->SetReporter(reporter);
	   
	loader->SetHome(urlHome);
	   
	loader->Set(url,1);
	loader->SetRefedFrom(refedByUrl,refedByType);
	   
    loader->useThread = GFile::useThreads;

	// check out if url is already beeing downloaded 
	GFile *reqInProgress = NULL;
	if (loader->url.GetLength()>0)  // its a internet URL 
	{
	   LockFile();

	   GFile * p=GFile::filesInUse;
	   while (p!= NULL) {
		   if (p->url == loader->url) {
			   reqInProgress = p;
			   break;
		   }
		   p = p->next;
	   }
	   if (reqInProgress) reqInProgress->ref();
	   UnlockFile();

	}

	if (reqInProgress) { // reuse existing request 
		loader->unref();
		// increment reference count  done above
		loader = reqInProgress;
		TRACE("Request already in progress : %s \n",(const char *)loader->GetUrl());
		localFile = loader->localFileUnzipped;
		localFileFmt = loader->localFileFmt;
		localFileClass = loader->localFileClass;
		home=loader->GetHomeUrl();
		state = GetStateFromLoader();
		return(state);
	}

	ASSERT(loader != NULL);

	if (loader == NULL) {
	   	state = S_NOT_LOADED;
	   	return(state);
	}


	// window to notify on completition
	loader->hPostMsgWnd = hPostMsgWnd;
	
	  // url process (loading / unzipping  / unknow extension dispatch) or url thread starting
	int ret = loader->Process();
	
	if (ret == ERROR_THREAD_STARTED) {
		home=loader->GetHomeUrl();
		state = S_LOADING;
		
		// store in reqList(loader);
		GFile::AddFileInUse(loader);

	} else
	{
		localFile = loader->localFileUnzipped;
		localFileFmt = loader->localFileFmt;
		localFileClass = loader->localFileClass;
		home=loader->GetHomeUrl();

		if (ret<0) {
			Release();
			state = S_BAD_DATA;
		}
		else
			state = S_LOADED;
	}

	return(state);
}


// setup loader without loading 
GFileState 
GFileRef::SetupUrl(const char *urlHome, // the home url 
				const char *url,		// the relative url/file
				const char *refedByUrl,	// who has referenced this url
				const char *refedByType, // what type of referee
				int wwwFlags,		// wwwFlags 
				HWND hPostMsgWnd,	// window to notify on completition
				int currentUrlI,		// the current urli for MF-Urls´s
				GReporter *reporter, // the error reporter
				GFile *rootLoader,
				CString &extraArg	// the # argument
				)	

{
	extraArg = "";

	if (url == NULL || url[0] == 0) {
		state = S_NO_URL;
		return (state);
	}

	urlI = currentUrlI;
	
	// ASSERT(loader == NULL);
	
	Release();


	loader = new GFile();
	loader->ref();
	loader->SetFlags(wwwFlags);

	if (rootLoader) { // copy basic settings 
		if (rootLoader->reporter)
			loader->SetReporter(rootLoader->reporter);
#ifdef _COM

		loader->client = rootLoader->client;
		loader->statusCallback = rootLoader->statusCallback;
		loader->hObserver = rootLoader->hObserver;

#endif

		loader->hPostMsgWnd = rootLoader->hPostMsgWnd;		
		loader->hNetscapeMsgWnd = rootLoader->hNetscapeMsgWnd;	

	}
	if (reporter)
		loader->SetReporter(reporter);
	   
	loader->SetHome(urlHome);
	   
	loader->Set(url,1);
	loader->SetRefedFrom(refedByUrl,refedByType);
	   
    loader->useThread = GFile::useThreads;
	
	extraArg = loader->urlExtraArgs; // get the # argument


	// check out if url is beeing downloaded 
	GFile *reqInProgress = NULL;
	if (loader->url.GetLength()>0)  // its a internet URL 
	{
	   LockFile();

	   GFile * p=GFile::filesInUse;
	   while (p!= NULL) {
		   if (p->url == loader->url) {
			   reqInProgress = p;
			   break;
		   }
		   p = p->next;
	   }
	   if (reqInProgress) 
		   reqInProgress->ref();
	   UnlockFile();

	}

	if (reqInProgress) { // reuse existing request 
		loader->unref();
		// increment reference count  done above
		loader = reqInProgress;
		TRACE("Request already in progress : %s  ref = %d \n",(const char *)loader->GetUrl(),loader->refCnt);
		localFile = loader->localFileUnzipped;
		localFileFmt = loader->localFileFmt;
		localFileClass = loader->localFileClass;
		home=loader->GetHomeUrl();
		state = GetStateFromLoader();
		return(state);
	}

	ASSERT(loader != NULL);

	if (loader == NULL) {
	   	state = S_NOT_LOADED;
	   	return(state);
	}


	// window to notify on completition
	loader->hPostMsgWnd = hPostMsgWnd;
	
	localFile = loader->localFileUnzipped;
	localFileFmt = loader->localFileFmt;
	home=loader->GetHomeUrl();

	state = S_NOT_LOADED;

	return(state);
}



//
//  GFile 
//

// can another thread be started ?
gbool GFile::CanUseThread()
{
 	return (useThreads && (threadsInUse < maxThreads));
}

RTIMP(GFile,0,RTRoot);

//
// GFile 
//                 
GFile::GFile()
{
  refCnt=0;
  next = prev = NULL;


  reporter = GReporter::current;

  wwwFlags = WWW_REPORT_ERRORS;
  ifModifiedSince = 0;

  localFileUnzippedDelete=FALSE;   


  size = 0;
  loadingStartTime=0;
  loadingTime = 0;
  urlLoaded = 1;
  localFileLoaded = 0;
  localFileFmt = -1;
  localFileClass = GLFC_UNKNOWN;

  sceneId = 0;
  HTTPStatus = 0;

  contentLength = 0;
  isMultipart = 0;
  partNumber = 0;

  //data = NULL;

  hPostMsgWnd=NULL;
  hNetscapeMsgWnd = NULL;

#ifdef _COM
 
  client = NULL;
  statusCallback = NULL; 
  m_pbinding = NULL;
  m_pstm = NULL;

#endif

  useThread=0;
  isThread = 0;
#ifdef WIN32
  thread=NULL;
#endif
  threadBusy = 0;
  threadRet = 0;
  threadStatus = 0;
 
  stop = 0;
  lastModified = 0;
  cacheLastModified = 0;
  lastChecked = 0;


#ifdef G_WININET

  // WININET handles 
  hInternetConnection = NULL;
  hInternetFile = NULL;

#endif
}

void GFile::SetReporter(GReporter *reporter)
{
	this->reporter = reporter;
	if (this->reporter == NULL)
	  this->reporter = GReporter::current;

}



void GFile::unref() {
  //refCnt--;
  InterlockedDecrement(&refCnt);
  
  //TRACE("gfile::unref %s  refcnt now = %d\n",GetUrl(),refCnt);

  if (refCnt<=0)
  		delete this;
  else if (Busy() && refCnt == 1 && stop == 0) {  // thread proc has one reference, no other references, so abort
		 Kill(); // new 
  		 // stop = 1;
  }		
}

void GFile::unrefNoDelete() 
{ 
   InterlockedDecrement(&refCnt);
}


void GFile::ref() 
{
  //refCnt++;
  // TRACE("gfile::ref %s  refcnt  = %d\n",GetUrl(),refCnt);

  InterlockedIncrement(&refCnt);
}

GFile::~GFile()
{
    // TRACE("~gfile::ref %s  refcnt  = %d\n",GetUrl(),refCnt);
	RemoveFileInUse(this);
	Kill();
#ifdef _COM
    hObserver.p = NULL;
#endif
}

int GFile::Flush() 
{
// delete temporary unzipped
  return(0);
}

#ifdef G_WININET

// thread worker proc 
// AFX_THREADPROC


UINT G__cdecl GFileWriteUrlToFileWorker( LPVOID pParam )
{
  GFile *me = (GFile *)  pParam;

  if (!me) return(0);

  me->ref(); //  keep me alive

  // done in GFile::Process
  // InterlockedIncrement(&GFile::threadsInUse); // ++;

  me->threadBusy=1;

   TRACE("Thread (%d)Begin ( %s\n\t %s ( \n",GFile::threadsInUse,(const char *) me->url,(const char *) me->localFile);	
  
  		me->threadRet = me->WriteUrlToFile();

 		me->urlLoaded = 1;

		 	if (me->threadRet>=0) {
				me->size = me->threadRet;
				if (!me->useInetCache) me->WriteUrlDescriptionFile();
			    // to do: check for http errors / redirection
			}

  
   InterlockedDecrement(&GFile::threadsInUse); 

   TRACE("Thread END %d %s\n\t %s END)  %d \n",me->threadRet,(const char *) me->url,(const char *) me->localFile,me->threadRet);	

   me->thread = NULL;


   // refcnt == 1 nobody interested in thread any more, 1 ref from me
   // send completition message to window
   me->threadBusy=2;

   if (me->hPostMsgWnd && me->refCnt >1) {
	  me->ref(); 	// receiving window must do the unref 
      ::PostMessage(me->hPostMsgWnd,WM_READFILECOMPLETED, (WPARAM) me->threadRet, (LPARAM) me) ;
   } else 
   if (me->hGlobalPostMsgWnd && me->refCnt >1)  {
	  me->ref(); 	// receiving window must do the unref 
	  ::PostMessage(me->hGlobalPostMsgWnd,WM_READFILECOMPLETED, (WPARAM) me->threadRet, (LPARAM) me) ;
   }
   else 
     me->threadBusy=0;

   me->unref(); 

   return(1);
}
#endif

/*
HRESULT URLDownloadToCacheFile( 
    LPUNKNOWN            lpUnkcaller,
    LPCWSTR             szURL,
    LPWSTR              szFileName,
    DWORD               dwBufLength,
    DWORD               dwReserved,
    IBindStatusCallback *pBSC
);
*/


#ifdef G_URLMON
// needed functions from urlmon.dll (urlmon.h)
 
// static function variables for lib functions (global)
URLDownloadToCacheFileAProc URLDownloadToCacheFileH = NULL;
HlinkSimpleNavigateToStringProc HlinkSimpleNavigateToStringH = NULL;
CoGetClassObjectFromURLProc CoGetClassObjectFromURLH = NULL;


// http://www.microsoft.com/msdn/sdk/inetsdk/help/itt/monikers/functions/URLDownloadToCacheFile.htm

HINSTANCE urlmonLib = NULL;

/*
InitializeUrlmonLib
initialize the IE specific urlmon.dll and find the entry points we are interested in
Goal : don't request the dll during startup, only if need 

*/
BOOL InitializeUrlmonLib() 
{
	// HLinkSimpleNav == urlmon.dll
	if (urlmonLib == NULL) { // load the urlmon library 
		urlmonLib = AfxLoadLibrary("urlmon.dll"); // LoadLibrary
		if (urlmonLib == NULL) {
			// Reporter.Error("Can't load library 'urlmon.dll'");
			return FALSE;
		}
		else {
			URLDownloadToCacheFileH =  ( URLDownloadToCacheFileAProc )GetProcAddress(urlmonLib,"URLDownloadToCacheFileA");
			HlinkSimpleNavigateToStringH =  ( HlinkSimpleNavigateToStringProc )GetProcAddress(urlmonLib,"HlinkSimpleNavigateToString");
			CoGetClassObjectFromURLH =  ( CoGetClassObjectFromURLProc )GetProcAddress(urlmonLib,"CoGetClassObjectFromURL");
			if (URLDownloadToCacheFileH == ( URLDownloadToCacheFileAProc ) NULL) {
				// Reporter.Error("Can't find function in library 'urlmon.dll'");
				return FALSE;
			}
		}
	}
	return TRUE;
}

// not really needed , windows will clean it up 
void TermUrlmonLib() 
{
}
#else
#define URLDownloadToCacheFileH URLDownloadToCacheFile
#endif


// find url in IE-WININET-cache
// and update some status variables
gbool GFile::FindInternetCacheFile(CString &localFile)
{
	  gbool ret = FALSE;

#ifdef G_URLCACHE

#ifndef MAX_CACHE_ENTRY_INFO_SIZE	
		INTERNET_CACHE_ENTRY_INFO buf;
        INTERNET_CACHE_ENTRY_INFO *entry = &buf;
		DWORD len = sizeof(INTERNET_CACHE_ENTRY_INFO);
		DWORD maxlen = sizeof(INTERNET_CACHE_ENTRY_INFO);
#else
	  // no longer defined in VC 6.0 ?

	  char buf[MAX_CACHE_ENTRY_INFO_SIZE];
      INTERNET_CACHE_ENTRY_INFO *entry = (INTERNET_CACHE_ENTRY_INFO *)buf;
	  DWORD len = MAX_CACHE_ENTRY_INFO_SIZE;
	  DWORD maxlen = MAX_CACHE_ENTRY_INFO_SIZE;
#endif

      
      HANDLE hdl;
                    
      if ((hdl = FindFirstUrlCacheEntry (url /*NULL*/, entry, &len)) != NULL)
      {
           do
           {
			   if (entry->lpszSourceUrlName != NULL && (url == entry->lpszSourceUrlName))
			   if ( (entry->lpszLocalFileName != NULL) )
               {
                   localFile = entry->lpszLocalFileName ;
				   ret = TRUE;
				   size = entry->dwSizeLow;
					//size = entry->LastModifiedDate;
				   lastModified =  CTime(entry->LastModifiedTime); // GMT !!!!!!!!!!
				   expires =  CTime(entry->ExpireTime); // GMT !!!!!!!!!!
#ifdef _DEBUG
				   TRACE(" URL %s \n",entry->lpszSourceUrlName);
				   time_t t= lastModified.GetTime();
				   TRACE(" lastModified %s \n",ctime(&t));
				   t = expires.GetTime();
				   TRACE(" expires %s \n",ctime(&t));
#endif					
				   if (entry->lpHeaderInfo != NULL) {
					   // getHeaderEntry("Content-Type: ",contentType);
					   // getHeaderEntry("Content-Encoding: ",contentEncoding);
/*
lpHeaderInfo	0x0153ef60 "HTTP/1.1 200 OK
ETag: "5890a-24e5-34ec0129"
Content-Length: 9445
Keep-Alive: timeout=15, max=95
Content-Type: x-world/x-vrml
Content-Encoding: x-gzip
*/
				   }

                   break;
               }
               len = maxlen;
           }
           while (FindNextUrlCacheEntry ( hdl, entry, &len) );
		   FindCloseUrlCache(hdl);

      }
#endif

	  return ret;
}

#ifdef WIN32

// get WIN32 error message
void GetErrorMessage(DWORD err, CString &msg)
{

		LPVOID lpMsgBuf=NULL;

		::FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			err,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL 
		);

		if (lpMsgBuf) msg = (LPCTSTR) lpMsgBuf;

		// Free the buffer.
		if (lpMsgBuf) LocalFree( lpMsgBuf );
}
#endif


#ifdef G_URLCACHE

// AFX_THREADPROC

UINT G__cdecl GFileWriteUrlToCacheFileWorker( LPVOID pParam )
{
	GFile *me = (GFile *)  pParam;
	if (!me) { 
		InterlockedDecrement(&GFile::threadsInUse); 

		return(0);
	}

#ifdef G_URLMON
	if (URLDownloadToCacheFileH == NULL)  { 
		me->unref(); // extra ref, from thread starter 
		InterlockedDecrement(&GFile::threadsInUse); 
		return 0;
	}

#endif

	// done in GFile::Process
	// InterlockedIncrement(&GFile::threadsInUse); // ++;
	
	// already done from  thread starter  me->ref(); // keep me alive 

	me->threadBusy=1;
	CString fileName;
	CString tmp;

    TRACE("Thread (%d)Begin ( %s\n\t %s ( \n",GFile::threadsInUse,(const char *) me->url,(const char *) me->localFile);	
	if (me->url.GetLength() == 0) {
		ASSERT(0);
	}


	{

	
	HRESULT res;
	// URLOSTRM_GETNEWESTVERSION 
	// URLOSTRM_USECACHEDCOPY 
	// URLOSTRM_USECACHEDCOPY_ONLY 



    if ( FAILED(res=URLDownloadToCacheFileH(
        me->client, // GetControllingUnknown(),                // control's IUnknown
        me->url,                         // URL to download
        fileName.GetBuffer(MAX_PATH),	// buffer to fill with name
        MAX_PATH,                               // buffer length
        0,                                      // reserved
        (me->statusCallback) ? me->statusCallback : me)) )            // if NULL no status information
    {
		fileName.ReleaseBuffer();
		
		CString msg;

		GetErrorMessage(res, msg);

		TRACE("Can't download '%s' %s (%x)",(const char *) me->url,(const char *)msg,res);

		// check if it can be  found using cache API's
		if (me->FindInternetCacheFile(fileName)) {
			me->localFile = fileName;
			me->localFileClass = GLFC_IE_CACHE_FILE;
			me->localFileUnzipped = fileName;
			me->urlLoaded = 1;
			me->threadRet = 0;
			// save file in cache 	
#ifdef G_CACHE
			if (!me->Flag(WWW_DONT_CACHE))
				theCache.SaveUrl(me->url,me->localFile,me->lastModified.GetTime(),me->localCacheFile);

#endif
		
		} else {
			if (me->Flag(WWW_REPORT_ERRORS) && me->reporter) 
				me->reporter->Error("Can't download '%s' : %s (%x)",(const char *) me->url,(const char *)msg,res);
			me->threadRet = -1;
		}
    }
	else {
		fileName.ReleaseBuffer();
		me->localFile = fileName;
		me->localFileClass = GLFC_IE_CACHE_FILE;
		me->localFileUnzipped = fileName;
		me->urlLoaded = 1;
		me->threadRet = 0;
		
		// anyway, go to the cache file functions and update lastModified date
#ifdef G_CACHE
		if (theCache.readCacheEnabled && me->FindInternetCacheFile(fileName)) {
			
			// TRACE("Last Modified %s  %d \n",(const char *) me->contentType,me->lastModified.GetTime());
		}		
#endif

		// save file in cache 	
#ifdef G_CACHE
		if (!me->Flag(WWW_DONT_CACHE))
			theCache.SaveUrl(me->url,me->localFile,me->lastModified.GetTime(),me->localCacheFile);
#endif 
		me->lastChecked = CTime::GetCurrentTime();

	}

#if 0
	// temp try for IE 4.01 gz file download problem  800C0007 data not available
	if (me->threadRet <0) {
		
		fileName = me->localFile;

		if ( FAILED(res=URLDownloadToFile(
			me->client, // GetControllingUnknown(),                // control's IUnknown
			me->url,                         // URL to download
			fileName,
			0,                                      // reserved
			(me->statusCallback) ? me->statusCallback : me)) )            // if NULL no status information
		{
			TRACE("Can't download '%s' (%x)",(const char *) me->url,res);
		    
			if (me->Flag(WWW_REPORT_ERRORS) && me->reporter) me->reporter->Error("Can't download '%s' (%x)",(const char *) me->url,res);
		
			me->threadRet = -1;
		
		}
		else {
			me->localFile = fileName;
			me->localFileUnzipped = fileName;
			me->urlLoaded = 1;
			me->threadRet = 0;
		}
	}
#endif

	}

	InterlockedDecrement(&GFile::threadsInUse); 

	TRACE("Thread END %d %s\n\t %s END)  %d \n",me->threadRet,(const char *) me->url,(const char *) me->localFile,me->threadRet);	

	// tmp.Format("Thread END %d %s\n\t %s )  %x \n",me->threadRet,(const char *) me->url,(const char *) me->localFile,res);	
	// ::OutputDebugString(tmp);

	me->thread = NULL;


	// refcnt == 1 nobody interested in thread any more, 1 ref from me
	// send completition message to window
	me->threadBusy=2;

	if (me->hPostMsgWnd && me->refCnt >1) {
	  me->ref(); 	// receiving window must do the unref 
      ::PostMessage(me->hPostMsgWnd,WM_READFILECOMPLETED, (WPARAM) me->threadRet, (LPARAM) me) ;
	} else 
	if (me->hGlobalPostMsgWnd && me->refCnt >1) {
	  me->ref(); 	// receiving window must do the unref 
	  ::PostMessage(me->hGlobalPostMsgWnd,WM_READFILECOMPLETED, (WPARAM) me->threadRet, (LPARAM) me) ;
	}
	else 
      me->threadBusy=0;

	me->unref(); 

	return(1);
}

#endif G_URLCACHE




// set url part of this to new url
// and set local file part to cache directory filename
int GFile::SetUrl(const char *path) 
{

		if (strprefix(path,URL_PROTOCOL)) 
			path += strlen(URL_PROTOCOL);

		// strip of arguments 
		const char *s=strrchr(path,'#');
		if (s) {
		  urlExtraArgs = &s[1];
		  url = CString(path,s-path);
		} else {
		  url = path;
		  urlExtraArgs = "";
		}

		// get home path 
		s=strrchr(url,'\\'); // for file:  URL´s 
		const char *s1=strrchr(url,'/');
		if (s1>s ) s = s1;
 		if (s) 
  		  urlHome =CString(url,s-url+1);
		else urlHome = "";

		// hints to not cache 05.07.99 hg 
		if (strstr(url,"/no_cache/")) {
			wwwFlags |= WWW_DONT_CACHE;
		}
		if (strstr(url,"/cgi-bin/")) {
			wwwFlags |= WWW_DONT_CACHE;
		}


		// create a cache file name 
		CString tmp;

		UrlGetLocalFileName(url,tmp.GetBuffer(url.GetLength()));
		tmp.ReleaseBuffer();

		// create cache directory if not existing 
		if (cacheDirectory.GetLength()>0) {
		  int l = cacheDirectory.GetLength();

#ifdef WIN32
		  if (cacheDirectory[l-1] != '\\')
			  cacheDirectory += '\\';
		  {
			SECURITY_ATTRIBUTES sa;
			sa.nLength = sizeof(sa);
			sa.lpSecurityDescriptor = NULL;
			sa.bInheritHandle = FALSE;
			CreateDirectory(cacheDirectory,&sa);
		  }
#endif
		  
		  tmp = cacheDirectory + tmp;

		  	
		}
		// set the local file 
		SetLocalFile(tmp,GLFC_CC3D_CACHE_FILE);
		urlLoaded = 0; // Default : url not loaded to a disk file
		return(1);
}


// set local file name part to new filename path
int GFile::SetLocalFile(const char *path,GLocalFileClass fileClass) 
{
		
	   // strip of arguments 
		const char *s=strrchr(path,'#');
		if (s) {
		  urlExtraArgs = &s[1];
		  localFile = CString(path,s-path);
		} else {
		  localFile = path;
		  // urlExtraArgs = ""; // might be set from url ???
		}
	


		s=strrchr(localFile,'\\');
		const char *s1=strrchr(localFile,'/');
		if (s1>s ) s = s1; 
 		if (s) 
  		  localFileHome =CString(localFile,s-localFile+1);
		else localFileHome = "";

		localFileUnzipped = localFile;

		localFileLoaded = 0;
		localFileFmt = GetFileType(localFile);
		localFileClass = fileClass;

		localCacheFile.Empty();

		return(1);
}

/*
 in case of FATAL media error
 delete cached files in order to force refresh
 on next visit
 return code :
	0 no deletion was done
	1 local file deleted
	2  " may try a reload 
*/

int GFile::DeleteCachedLocalFile(GLocalFileClass localFileClass,const char *fileName)
{
#ifdef WIN32

	switch (localFileClass) {
	case GLFC_UNKNOWN:			// unknown 
	case GLFC_LOCAL_FILE:		// local file on computer
			return 0;

	case GLFC_NS_CACHE_FILE:		// its a Netscape cache file
			{
			char    szDrive[8] ;
			char    szDir[_MAX_PATH] ;
			char    szFile[_MAX_PATH] ;
			char    szExt[_MAX_PATH] ;
			int     i = 0 ;

			_splitpath(fileName, szDrive, szDir, szFile, szExt) ;
/*
			// file was copied to cache, delete the CC3D copy
			if (localCacheFile != localFile  && !localCacheFile.IsEmpty()) {
				 ::DeleteFile(localCacheFile); 
				 localCacheFile.Empty();
			}
*/
			// mozilla cache file ?
			if (szFile[0] != 'M') 
				return 0;
			if (strlen(szFile) != 8) 
				return 0;
			// /cache in dir ?
			// fat.db in directory 

			}
			// fall through 
	case GLFC_TMP_FILE:			// temporary file 
	case GLFC_CC3D_CACHE_FILE:	// its a CC3D cache file
	case GLFC_IE_CACHE_FILE:		// its a Windows UrlCache  cache file
		{
			BOOL ret;
			TRACE("Removing file : %s \n",fileName);
			//ASSERT(0); // diagnostic only
			GLocalFileClass oldFileClass = localFileClass;
/*
			// file was copied to cache, delete the CC3D copy as well
			if (localCacheFile != localFile && !localCacheFile.IsEmpty()) {
				 ret =::DeleteFile(localCacheFile); 
				 localCacheFile.Empty();
			}
*/
			ret =::DeleteFile(fileName); 
			localFileClass = GLFC_UNKNOWN;
			//localFileLoaded = 0;
			if (ret) {
				if (oldFileClass == GLFC_CC3D_CACHE_FILE) return 2; // try a reload
				return 1;
			}
			return 0;
		}
	default : return 0;
	}

#else

    return 0;

#endif

}

int GFile::DeleteCachedLocalFile()
{
#ifdef WIN32

	const char *fileName = localFile;

	switch (localFileClass) {
	case GLFC_UNKNOWN:			// unknown 
	case GLFC_LOCAL_FILE:		// local file on computer
			return 0;

	case GLFC_NS_CACHE_FILE:		// its a Netscape cache file
			{
			char    szDrive[8] ;
			char    szDir[_MAX_PATH] ;
			char    szFile[_MAX_PATH] ;
			char    szExt[_MAX_PATH] ;
			int     i = 0 ;

			_splitpath(fileName, szDrive, szDir, szFile, szExt) ;

			// file was copied to cache, delete the CC3D copy
			if (localCacheFile != localFile  && !localCacheFile.IsEmpty()) {
				 ::DeleteFile(localCacheFile); 
				 localCacheFile.Empty();
			}

			// mozilla cache file ?
			if (szFile[0] != 'M') 
				return 0;
			if (strlen(szFile) != 8) 
				return 0;
			// /cache in dir ?
			// fat.db in directory 

			}
			// fall through 
	case GLFC_TMP_FILE:			// temporary file 
	case GLFC_CC3D_CACHE_FILE:	// its a CC3D cache file
	case GLFC_IE_CACHE_FILE:		// its a Windows UrlCache  cache file
		{
			BOOL ret;
			TRACE("Removing file : %s \n",fileName);
			//ASSERT(0); // diagnostic only
			GLocalFileClass oldFileClass = localFileClass;

			// file was copied to cache, delete the CC3D copy as well
			if (localCacheFile != localFile && !localCacheFile.IsEmpty()) {
				 ret =::DeleteFile(localCacheFile); 
				 localCacheFile.Empty();
			}

			ret =::DeleteFile(fileName); 
			localFileClass = GLFC_UNKNOWN;
			localFileLoaded = 0;
			if (ret) {
				if (oldFileClass == GLFC_CC3D_CACHE_FILE) return 2; // try a reload
				return 1;
			}
			return 0;
		}
	default : return 0;
	}

#else

    return 0;

#endif
}




/*
  Combine a absolute and a relative path

  Could be combinations like
  home : \ \  \ path using / / /
*/
int CombinePath(const char *home,const char *path,CString &out)
{

 if (path[0] == '.') {
 if (strprefix(path,"../")) {
	const char *pstart= home;
	const char *pend= &pstart[strlen(pstart)-1];

	if (pend > pstart) {
		if ((*pend == '/') || (*pend == '\\')) pend--;

		// strip filename part 
		while ((pend>pstart) && (! ((*pend == '/') || (*pend == '\\'))) ) { pend --; }

        CString newHome(pstart,(pend-pstart)+1);
		return (CombinePath(newHome, &path[3],out));
	}
 }
 else if (strprefix(path,"./")) {
		return (CombinePath(home, &path[2],out));
 }
 if (strprefix(path,"..\\")) {
	const char *pstart= home;
	const char *pend= &pstart[strlen(pstart)-1];

	if (pend > pstart) {
		if ((*pend == '/') || (*pend == '\\')) pend--;

		// strip filename part 
		while ((pend>pstart) && (! ((*pend == '/') || (*pend == '\\'))) ) { pend --; }

        CString newHome(pstart,(pend-pstart)+1);
		return (CombinePath(newHome, &path[3],out));
	}
 }
 else if (strprefix(path,".\\")) {
		return (CombinePath(home, &path[2],out));
 }

 }

 // simply combine
 out = home;
 out += path;
 return(1);
}


// create absolute path for path if path is a relative filename (./ ../ .\ ..\ etc


int CreateAbsolutePath(const char *home,const char *path,CString &newPath)
{
  int isRelPath;

  isRelPath = !((strchr(path,':') || path[0]=='/' || path[0] == '\\'));
  
  // if relative path, resolve to home 
  if ( (path[0]=='~') || (path[0]=='.') || isRelPath) {
  	int ofs = 0;
	if (strcmp(path,"./") == 0) ofs = 2;
	else 
	
	if (strcmp(path,".\\") == 0) ofs = 2;

	CombinePath(home, &path[ofs],newPath);

  }
  else newPath = path;

  return(1);
}



/*

create a pathname relative to src,

A:/B/C/D   A:/B/C/E   ==  ../E


*/
int CreateRelativePathname(const char *src, const char *dest,char *result)
{
 const char *s = src;
 const char *d = dest;
 while (*s == *d) { s++; d++; }
 // different at start 
 if (s == src) { strcpy(result,dest); return(0); }
 if (*s == 0)   // A:/B  A:/B/C ==> C
 { 
     if (*d == '/' || *d == '\\') d++; // skip starting separator
     strcpy(result,d); return(1); 
 }
 
 //A:/B/C  A:/B/D ==> ../C
 if (! (strchr(s,'/') || strchr(s,'\\')) ) {
        strcpy(result,"../");
        if (*d == '/' || *d == '\\') d++; // skip starting separator
        strcat(result,d);
        return(1);

 }
 else {
     strcpy(result,dest); return(0); 
 }
}




int GFile::Set(const char *path,int isLocal,int queryForUrl) 
{
  int ret=-1;
  int isRelPath;

  // unset variables
  protocol = "";
  size = -1;
  arguments = "";
  title = "";

  contentType = "";
  contentEncoding = "";
  
  urlExtraArgs = ""; // new 

  // new 05.11.98 hg skip whitespace at start
  while(isspace(*path)) 
	  path++;

  // ????? 
  // is path not absolute ??????? 
  // isLocal means comes from WWWInline

  isRelPath = !((strchr(path,':') || path[0]=='/' || path[0] == '\\'));
  
  // if relative path, resolve to home 
  if ( (path[0]=='~') || (path[0]=='.') || isRelPath) {
  	int ofs = 0;
	if (strcmp(path,"./") == 0) ofs = 2;
	else 
	
	if (strcmp(path,".\\") == 0) ofs = 2;

  	if (urlHome.GetLength() > 0) {
		CString newPath;
		CombinePath(urlHome, &path[ofs],newPath);
		ret = SetUrl(newPath);
 	    return(ret);

	}
  	else if (localFileHome.GetLength() > 0) {
		CString newPath;
		CombinePath(localFileHome, &path[ofs],newPath);
		ret = Set(newPath);
		return(ret);

	}
  }

  if ((path[0]=='/') && (urlHome.GetLength() > 0)) {
  	char *p= strstr(urlHome,"//");
	if (p) {
	   p = strchr(p+2,'/');
	   if (p) {
	     	ret = Set(CString(urlHome,p-urlHome) + path);
			return(ret);
	   }
	}
  }

  if (IsLocalFile(path)) { // file::// ?
	url = "";
	urlHome = "";
	urlLoaded = 0;
	
	CString newPath(path+strlen(prefix_file)); // strip of file://

#ifdef _WIN32

	if (strnicmp(path, "file:///",8) == 0) { // netscape stupidity
		newPath = newPath.Mid(1); 
        int i = newPath.Find('|');   // netscape file://c|
        if (i>=0) newPath.SetAt(i ,':');
	}
#endif
	
	if (queryForUrl) { // ask netscape for url for this file 
		CString temp;

	    ret = GetURLFromLocalFile(newPath, temp.GetBuffer(2048));
	    temp.ReleaseBuffer();

	    if (ret == 1) {
	        TRACE("Got url from local file :%s\n",(const char *) temp);
	        SetUrl(temp);
            urlLoaded = 1; // don´t need to load, done by Netscape
	    }
	}
  	ret = SetLocalFile(newPath,GLFC_LOCAL_FILE);
	return(ret);
  }
  else if (IsUrlFile(path)) {
	ret = SetUrl(path);
	return(ret);
  }


  {
	url = "";
	urlHome = "";
	urlLoaded = 0;
	// && extension != url
	if (queryForUrl) {
	CString temp;
	ret = GetURLFromLocalFile(path, temp.GetBuffer(2048));
	temp.ReleaseBuffer();
	if (ret == 1) {
	  TRACE("Got url from local file :%s\n",(const char *) temp);
	  SetUrl(temp);
      urlLoaded = 1;

	  if (!useInetCache) WriteUrlDescriptionFile(); // temp hack, force a write of a url file for reference
	  // otherwise it will be only written on a load by us
	}
	}

  	ret = SetLocalFile(path,GLFC_LOCAL_FILE);
  }
  return(ret);
}

// set home directory / url, use before passing a relative url to set
int GFile::SetHome(const char *path,int isLocal)
{

	CString realPath(path);
	int i;

	// TBD: check proper URL parsing

	// strip of cgi-bin stuff
	i=realPath.Find('?');
	if (i >= 0 ) realPath = realPath.Left(i);

	// strip of # stuff
	i=realPath.ReverseFind('#');
	if (i >= 0 ) realPath = realPath.Left(i);

	// strip off file argument 
	i=realPath.ReverseFind('\\');
	int i1=realPath.ReverseFind('/');
	if (i1>=i) i = i1; 
	if (i >= 0 ) realPath = realPath.Left(i+1);

    if (strprefix(realPath,prefix_file)) { // file://
		if (strnicmp(realPath, "file:///",8) == 0) { // netscape stupidity
			realPath = realPath.Mid(8); 
			int i = realPath.Find('|');   // netscape file://c|
			if (i>=0) realPath.SetAt(i ,':');
		} else {
			realPath = realPath.Mid(strlen(prefix_file)); 
		}

  		urlHome = "";
		localFileHome = realPath;
		return(1);
    }

	if (IsUrlFile(path)) {
		urlHome = realPath;
		localFileHome = "";
		return(1);
	}
	else  {
  		urlHome = "";
		localFileHome = realPath;
		return(1);
	}
}



// check if this is a zipped file
int GFile::ProcessExtension(const char *fileName) 
{
const char *extp;

localFileUnzipped = fileName;

extp = GetExtp(fileName);

extension = "";

if (!extp) return(0);

CString ext(extp+1);
ext.MakeLower();

if (ext == "z" || ext == "gz") 	{
	fileIsZipped=1;

	CString localFileUnzippedTmp = CString(fileName,extp-fileName);
	
	// recheck local file type 
	localFileFmt = GetFileType(localFileUnzippedTmp); 

	if (localFileFmt == UNKNOWN_FMT) {
		// got only gz, bad
		if (url.GetLength()>0) {
			localFileFmt = GetFileType(url); 
		}
	}
	else 
		if (localFileFmt != VRML_FMT) { // we can do streaming unzip 
			localFileUnzipped  = localFileUnzippedTmp;
		}

} 
else if ((ext == "wgz") || (ext == "wrz")) {
		fileIsZipped=1;
#if 0
		// now streaming unzip 
		if (url.GetLength()>0) {   // 4.05.97
			// will be renamed to wrl.gz for unzip 
			localFile = localFileUnzipped.Left(extp-fileName)+".wrl.gz";
		}
		localFileUnzipped = localFileUnzipped.Left(extp-fileName)+".wrl";
#endif 
}
else {
   fileIsZipped = 0;
}

extension = ext;
return(0);
}

// read a url from a url description file
int ReadUrlFile(GFile &gfile,const char *filename)
{	int ret = -1;
	char buf[_MAX_PATH];
	int buf_size=_MAX_PATH;

	FILE *f=fopen(filename,"r");
	if (!f) return(-1);
	
	while(fgets(buf,buf_size,f)) {
		char *p=buf;
		//while(isspace(*p)) p++;
		char *tok=strtok(buf," \t\n");
		char *val;
        if (!tok) continue; // 15.04.96


		if (streq(tok,"url")) {
			val = strtok(NULL,"\t\n");
			gfile.Set(val);
			ret = 1;
		}
		else if (streq(tok,"file")) {
			val = strtok(NULL,"\t\n");
		 	// 1.04.96 use filename only if existing
         if	(gfile.url.GetLength()>0) {
            if  (ExistsFile(val)) { 
			      gfile.SetLocalFile(val,GLFC_LOCAL_FILE);
               gfile.urlLoaded = 1;
            }
         } else {
            gfile.SetLocalFile(val,GLFC_LOCAL_FILE);
         }
			ret = 1;
		}
		else if (streq(tok,"title")) {
			val = strtok(NULL,"\t\n");
//			gfile.SetTitle(val);
			gfile.title = val;
		}
		else if (streq(tok,"contentType")) {
			val = strtok(NULL,"\t\n");
			gfile.contentType = val;
		}

		else if (streq(tok,"size")) {
			val = strtok(NULL,"\t\n");
			gfile.size = atoi(val);
		}
		else if (streq(tok,"time")) {
			val = strtok(NULL,"\t\n");
		}
		else if (streq(tok,"lastModified")) {
			val = strtok(NULL,"\t\n");
			gfile.lastModified = atoi(val);
		}
		else if (streq(tok,"expires")) {
			val = strtok(NULL,"\t\n");
			gfile.expires = atoi(val);
		}
		else if (streq(tok,"end")) break;
		else if (streq(tok,";") || streq(tok,"#")) ;
		else TRACE("Unknown line (%s) in url file  : %s \n",filename,buf);
	}
	fclose(f);
	return(ret);
}

// read a url from a url description file
int ReadUrlFile(const char *filename,CString &url)
{	int ret = -1;
	char buf[_MAX_PATH];
	int buf_size=_MAX_PATH;

	FILE *f=fopen(filename,"r");
	if (!f) return(-1);
	
	while(fgets(buf,buf_size,f)) {
		char *p=buf;
		//while(isspace(*p)) p++;
		char *tok=strtok(buf," \t\n");
        if (!tok) continue; // 15.04.96

		char *val;
		if (streq(tok,"url")) {
			val = strtok(NULL,"\t\n");
			url = val;
			ret = 1;
         break; // 01.04.96
		}
		else if (streq(tok,"end")) break;
		else if (streq(tok,";") || streq(tok,"#")) ;
		else TRACE("Unknown line (%s) : %s \n",filename,buf);
	}
	fclose(f);
	return(ret);
}



const char * GetExecuteErrorMsg(int code) 
{
const char *msg = "";

#ifdef _WIN32

switch (code) 
{
case ERROR_FILE_NOT_FOUND	: msg="The specified file was not found."; break;
case ERROR_PATH_NOT_FOUND	: msg="The specified path was not found."; break;
case ERROR_BAD_FORMAT	: msg="The .EXE file is invalid (non-Win32 .EXE or error in .EXE image)."; break;
case SE_ERR_ASSOCINCOMPLETE	: msg="The filename association is incomplete or invalid."; break;
case SE_ERR_DDEBUSY	: msg="The DDE transaction could not be completed because other DDE transactions were being processed."; break;
case SE_ERR_DDEFAIL	: msg="The DDE transaction failed."; break;
case SE_ERR_DDETIMEOUT	: msg="The DDE transaction could not be completed because the request timed out."; break;
case SE_ERR_NOASSOC	: msg="There is no application associated with the given filename extension."; break;
case SE_ERR_SHARE	: msg="A sharing violation occurred."; break;
default:  break;
}

#endif

return(msg);
}


// verify that a the url of the cache file localFile matches the required URL
int VerifyCacheUrl(const char *localFile,const char *url)
{ CString urlFile(localFile);
  CString localFileUrl;
  
  urlFile += ".vrl"; // url
  
  if (ReadUrlFile(urlFile,localFileUrl) == 1) {
  	  TRACE("Requested url %s file in cache %s url %s \n",url,localFile,(const char*) localFileUrl);
  	  if (localFileUrl == url) return(1);
  }
  return(0);
}

// touch the cache vrl file 
int TouchCacheUrl(const char *localFile)
{ 
  int ret = 0;
  CString urlFile(localFile);
  
  urlFile += ".vrl"; // url
  FILE *f;
  f = fopen(urlFile,"r+");

  if (f) { fclose(f); ret = 1; }
  
  return(ret);
}

/* 
process a load request of this file
*/
int GFile::Process()
{  int ret;
   int callBrowser; // pass url / localFileUnzipped to browser

#ifdef _COM

   USES_CONVERSION;

#endif

   // hg 21.07.99 for savety set module state, if call results from COM/CALL/ BeginThread use AfxGetApp
#ifdef _AFXDLL
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif



Retry:
	ret = 0;
    callBrowser = 0;

	if (url.GetLength()==0) {
	
		if (localFile.GetLength()==0) { //xx
			if (reporter) reporter->Error("Empty url given" );
			ret = ERROR_BAD_URL;
			return ret;

	   }	
	   
	   ProcessExtension(localFile);

	   if  ((extension == "url") || (extension == "vrl")) {
   	   	  ret=ReadFromUrlDescriptionFile(localFile);
   		  if (ret<0) {
			if (reporter) reporter->Error("Can´t read url from url file %s",(const char *)localFile);
		  	return ret;
		  }
		  ret = 0;
	   }
	}

	if (url.GetLength()>0) { // we have an URL 

		 TRACE("Proces url %s \n",(const char *)url);

		 ProcessExtension(localFile);

		 const char *p=strchr(url,':');
		 
		 // get the protocol
		 if (p) {
			 protocol = CString(url, p-url);

             if (protocol == "urn") {
				 if (strprefix(url,G_URN_UMEL)) {
					 
					 CString path;
					 gbool isFile = gfalse;
					 GFileInfo info;
					 info.creationTime = 0;


#ifdef G_CACHE
					 if (theCache.GetUmelUrl(((const char *)url)+strlen(G_URN_UMEL),path,info,isFile)) {
						 if (isFile) {
			                 SetLocalFile(path,GLFC_LOCAL_FILE);
							 url = "";  
							 urlHome = "";
		                     urlLoaded = 1;
							 goto Retry;

						 }
						 else {
							 if (theCache.umelHome.GetLength()>0) {
								 urlLoaded = 0;

								 SetHome(theCache.umelHome);
								 Set(path);
								 goto Retry;

							 }

						 }

					 } 

#endif 

					 if (reporter && reporter->verbose) 
						 reporter->Warning("Universial Media not installed or media not present %s",(const char *)path);

				 }

			 } else


             if (protocol == "file") { // file///C:|myfile ....
                 p++; // :

                 while (*p == '/') p++;
                 
                 CString newFile(p);

                 int i = newFile.Find('|');
                 if (i>=0) newFile.SetAt(i ,':');

                 SetLocalFile(newFile,GLFC_LOCAL_FILE);
				 url = "";  // new 
				 urlHome = "";
                 
                 urlLoaded = 1;
				 goto Retry; // bad, new 

             }    
			 else if (!IsProtocolOk(protocol)) {
			    ret = ERROR_UNKNOWN_PROTOCOL;
				callBrowser = 1;
			 }

         }


		 if (extension == "html" || extension == "htm" || (ret == ERROR_UNKNOWN_PROTOCOL) ) {
			 localFile = url;
			 localFileUnzipped = url;
			 localFileFmt = HTML_FMT;
			 callBrowser = 1;
		 } else {

		 if (!urlLoaded) { 
		 // check our cache 	
			GFileInfo info;
			info.creationTime = 0;

#ifdef G_CACHE
			if ((!AnyFlag(WWW_REFRESH|WWW_RELOAD)) && theCache.GetUrl(url,ifModifiedSince.GetTime(),localFile,info)) {
				localFileClass = GLFC_CC3D_CACHE_FILE;

				localFileUnzipped = localFile;
				urlLoaded = 1;
				threadRet = 0;
				lastModified = info.creationTime; // info.getLastModifiedTime(); 
				cacheLastModified = info.creationTime;
				lastChecked = info.getLastModifiedTime(); // last time written 
			} else 
			// older cache stuff 
			if (!useInetCache && WWWuseFilesFromCache && !Flag(WWW_RELOAD) && (
		 		(ExistsFile(localFile) /* && VerifyCacheUrl(localFile,url)*/
				) 
		 		|| (ExistsFile(localFileUnzipped) /* && VerifyCacheUrl(localFile,url) */ ) ))
			{
		 		 //UpdateFromUrlDescriptionFile();
				TouchCacheUrl(localFile);
				Reporter.Status("Loading cached url %s",(const char *)url);
		 		urlLoaded = 1;
	 		} else
#endif 
            {	// get data
    			TRACE("Retreiving url %s  %s \n",(const char *) url,(const char *)localFileUnzipped);
				
				cacheLastModified = info.creationTime; // keep cache file entry time, to compare later 

#ifdef _COM
			
			// pass request to observer
			if (hObserver) {
				
				if (FAILED(hObserver->LoadUrlToFile((BSTR) T2COLE(((const char *)url))))) {
					if (reporter) reporter->Error("Cant' setup observed download request");
				}
				else {
					threadBusy=1;
					ret = ERROR_THREAD_STARTED; // thread
					// InterlockedIncrement(&GFile::threadsInUse); 

					return(ret);
				}
			}

#endif

#ifdef _WIN32

			// pass request to netscape message window 
			if (hNetscapeMsgWnd != NULL) {
				//::SendMessage(hNetscapeMsgWnd, WM_USER + 502, (WPARAM) (const char*)url,(LPARAM) NULL);
				// hg new 
				::PostMessage(hNetscapeMsgWnd, WM_USER + 502, (WPARAM) (const char*)url,(LPARAM) NULL);
				threadBusy=1;
				ret = ERROR_THREAD_STARTED; // thread
				// InterlockedIncrement(&GFile::threadsInUse); 
				return(ret);
			}

			if (useThread) {
  				threadBusy = 1;
				isThread = 1;

				InterlockedIncrement(&GFile::threadsInUse); 
				ref(); // extra ref, released by thread

				// start the thread 

  				thread = AfxBeginThread(GFileWriteUrlToCacheFileWorker, this,THREAD_PRIORITY_NORMAL,64*1024);	 	 	
#ifdef G_WININET
				// Wininet version 
  				//thread = AfxBeginThread(GFileWriteUrlToFileWorker, this,THREAD_PRIORITY_NORMAL,64*1024);	 	 	
#endif

				ret = ERROR_THREAD_STARTED; // thread

				return(ret);

	 	 	} else {

	 	 	  ret = WriteUrlToFile();

			}

#else

            ret = ERROR_BAD_URL;
#endif

			if (ret == ERROR_REDIRECTED_URL) {
				if (url == redirectUrl) {
					if (reporter) reporter->Error("Redirected url %s matches current !",(const char*) url);
				} else {
				  Set(redirectUrl);
				  goto Retry;
				}
			}
		 	if (ret<0) return(ret);

			size = ret;
			if (!useInetCache) WriteUrlDescriptionFile();

		 	urlLoaded = 1;
			// to do: check for http errors / redirection
		 }
		 }  // urlLoaded
		 }
	}  // url.GetLength()>=0

	int isLocal = (url.GetLength() == 0) || (protocol == "file");

	if (localFileFmt != VRML_FMT) { // can do streaming unzip on VRML read 
		ret = UnZipFile(localFile,localFileUnzipped.GetBuffer(_MAX_PATH), localFileFmt, 1,1); // hg isLocal, isLocal );
		localFileUnzipped.ReleaseBuffer();
	}
    
    TRACE("Proces  %s \n",(const char *)localFileUnzipped);


	// disable for savety reasons
#if 0	
	{
	 const char *prefix = "cmd:";
	 int prefixLen = strlen( prefix );
 	 if (strncmp( localFile, prefix,prefixLen) == 0) 
 	 {
	   	Reporter.Status("Executing '%s'",localFile);
		ret = WinExec((const char *)localFile + prefixLen,SW_SHOWNORMAL);
	   	Reporter.Status("Executing '%s' (%d)",localFile,ret);
		localFileUnzipped = "";
		ret = 0;
		return(ret);
		
	 }
 	}
	{
	 const char *prefix = "system:";
	 int prefixLen = strlen( prefix );
 	 if (strncmp( localFile, prefix,prefixLen) == 0) 
 	 {
	   	Reporter.Status("Executing command'%s'",localFile);
		ret = system((const char *)localFile + prefixLen);
	   	Reporter.Status("Executing command '%s' (%d)",localFile,ret);
		localFileUnzipped = "";
		ret = 0;
		return(ret);
		
	 }
 	}
#endif

	// check content type for localFileFmt
	if (contentType.GetLength()>0) 
	{
	   TRACE("MIME TYPE : %s\n",(const char *) contentType);

	   int type = ParseMimeType(contentType);
	   // Netscape reports text/plain for png 
	   if (type == UNKNOWN_FMT || type == GZIP_FMT || type >= HTML_FMT) {
		    //TRACE("Unknown Mime Type %s type %d type from filename %d \n",contentType,type,localFileFmt);
			if (localFileFmt != UNKNOWN_FMT) type = localFileFmt; 
	   }

	   localFileFmt = type;

	}

	if (localFileFmt <= 0) {  // check file header if format still unknown
		FILE *f;
		f=fopen(localFileUnzipped,"rb");
		if (f) {
			char buf[120];
			if (fread(buf,1,40,f) == 40)
				localFileFmt=GetFileType(buf,40);
/*
			if (strncmp(buf,"#VRML",5) == 0) {
				localFileFmt = VRML_FMT;
			}
*/
			fclose(f);
		}
	}



	if (localFileFmt == UNKNOWN_FMT && (url.GetLength()>0)) {
	   callBrowser = 1;
	}

    if (!urlLoaded && callBrowser && url.GetLength()>0) {
	   		localFile = url;
			localFileUnzipped = url;
	 }

#ifdef _WIN32
															  
	if (callBrowser || (localFileFmt == HTML_FMT)) {  // call browser

		if (Flag(WWW_EXECUTE_UNKNOWN_FMTS)) {
			
	    if (AskNetscapeToOpenURL(localFileUnzipped)==1) {
			Reporter.Status("%s passed to Netscape ",(const char* ) localFileUnzipped);
			ret = 0;

		} else  {
	    	CString cmd;
            cmd =  WWWbrowserPath;
           
            if (cmd.GetLength() == 0) {
                FindWWWBrowser(cmd);
            }


		    if (cmd.GetLength() == 0) {
			    if (reporter) reporter->Error("Please configure the full path to your WWW Browser\nin WWW->Options->Config");
			    ret = -1;
		    }
		    else {
	    	CString directory;

			// quote path to exe 
			if (cmd.Find("\"") <0) {
				cmd = "\"" + cmd;
				cmd += "\"";         
			}
         
	    
            // got a strange crash here
	    	//cmd += " \""+localFileUnzipped+"\"";

            cmd += " \"";
            cmd += localFileUnzipped;
            cmd += "\"";         


			Reporter.Status("%s ...",(const char* ) cmd);

  	    	GetCurrentDirectory(_MAX_PATH,directory.GetBuffer(_MAX_PATH));
	    	directory.ReleaseBuffer();


			PROCESS_INFORMATION pinf;
			STARTUPINFO pstart;
			memset(&pstart,0,sizeof(pstart));
			memset(&pinf,0,sizeof(pinf));
			pstart.cb = sizeof(pstart);
		
			if (CreateProcess(
				NULL,cmd.GetBuffer(_MAX_PATH),
				NULL,NULL,
    			FALSE,	// handle inheritance flag 
    			0,	// creation flags 
    			NULL,	// pointer to new environment block 
    			(const char *)directory,	// pointer to current directory name 
    			&pstart,	// pointer to STARTUPINFO 
				&pinf
   			)) {
				if (!CloseHandle(
        				pinf.hProcess 	// handle of object to close  
   					)) TRACE("Can´t close handle to process\n");;

 		   		ret = 0;
   			} else {
   				if (reporter) reporter->Error("Can´t execute '%s'.",(const char *) cmd);
				ret=-1;
   			}	
			}
        }
		localFileUnzipped = "";

		} // 
		//else // ERROR_UNCEXPEDTED_FILE_TYPE

	} // call browser
	else 
	if (localFileFmt <= 0 && Flag(WWW_EXECUTE_UNKNOWN_FMTS)) // unknown format 
	{
		CString directory;
   		GetCurrentDirectory(_MAX_PATH,directory.GetBuffer(_MAX_PATH));
		directory.ReleaseBuffer();

	   	Reporter.Status("Loading '%s'",localFile);
		HINSTANCE  inst;
     // HINSTANCE FindExecutable(localFileUnzipped,directroy,program.GetBuffer(_MAX_PATH));

		inst = ShellExecute(
    		AfxGetMainWnd()->GetDesktopWindow()->m_hWnd/* hwnd*/,	// handle to parent window
    		"open", 	// pointer to string that specifies operation to perform
    		localFileUnzipped,	
    		NULL,	// pointer to string that specifies executable-file parameters 
    		directory,	// pointer to string that specifies default directory
    		0 	// whether file is shown when opened
    	);
	   	Reporter.Status("Loading '%s' Status = %d",localFile,inst);
		//ERROR_FILE_NOT_FOUND	
	    if  (((int)inst) <= HINSTANCE_ERROR) {
			Reporter.Error("Can't execute '%s', %s (%d)",(const char*)localFileUnzipped,GetExecuteErrorMsg((int) inst),(int)inst);
			ret = -1;
		} else {
		  ret = 0;
		}
		localFileUnzipped = "";
	}

	if (localFileUnzipped.GetLength()>0) {
	    // verify file exists
		
		HANDLE f = CreateFile( localFileUnzipped,GENERIC_READ,FILE_SHARE_READ,
    		NULL,	 // address of security descriptor,
    		OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN,NULL);	
		 
		 if  (f ==INVALID_HANDLE_VALUE) {
		 	int err = GetLastError();
			// Reporter.Error("File '%s' does not exists (%d)",(const char*)localFileUnzipped,err);
#ifndef _GLVIEW_DLL			
			busy ++;
			refCnt ++;

			if (refedType.GetLength() >0 ) {
				
			}
			else {

			DWWWRetry d;
			
			if (url.GetLength()>0) d.m_url = url;
			else d.m_url = localFile;
			if (d.DoModal() == IDOK) {
				Set(d.m_url);
				refCnt--;
				busy--;
				goto Retry;
			}
			}
			refCnt--;
			busy--;
#endif

			ret = -1;
		 } else {
		   long size = GetFileSize(f,NULL);
		   CloseHandle(f);
		 }
    }	

#endif

	return(ret);
}


// url has been redirected 
int GFile::ProcessRedirectedUrl(const char* newUrl)
{	int ret;

	if (url == newUrl) {
		if (reporter) reporter->Error("Redirected url %s matches current !",(const char*) newUrl);
		return(-1);
	} else {
		  Set(newUrl);
		  ret = Process();
	}
	return(ret);
}


int GFile::ProcessLoad(int threadRet)
{  int ret;
   int callBrowser; // pass url / localFileUnzipped to browser


	ret = threadRet;
    callBrowser = 0;
	callBrowser =  (ret == ERROR_UNKNOWN_PROTOCOL) || (ret ==  ERROR_UNKNOWN_MIME_TYPE)
				  || (ret ==  ERROR_HTML_DATA);

	if (callBrowser) ret = 0;



		if (ret == ERROR_REDIRECTED_URL) 
		{
			if (url == redirectUrl) {
				Reporter.Error("Redirected url %s matches current !",(const char*) url);
			} else {
			  Set(redirectUrl);
			  return(Process());
			}
		}
		
		switch (ret) {
		case  ERROR_CONNECTION_ERROR:
		case  ERROR_TRANSMISSION_ABORTED:
		case  ERROR_TRANSMISSION_ERROR:
		case  ERROR_BAD_URL:
		case -1:
				return(ret);
		default:
		      break;
		}

	
	if (ret <0) return(ret);

	if (urlLoaded  && localFileFmt != VRML_FMT) { // can do streaming unzip on VRML read
		int isLocal = (url.GetLength() == 0) || (protocol == "file");
		ret =UnZipFile(localFile,localFileUnzipped.GetBuffer(_MAX_PATH), localFileFmt,1,1); //  isLocal, isLocal);
		localFileUnzipped.ReleaseBuffer();
    
	}
   	TRACE("Proces  %s  format %d \n",(const char *)localFileUnzipped,localFileFmt);


	// disable for savety reasons
#if 0	
	{
	 const char *prefix = "cmd:";
	 int prefixLen = strlen( prefix );
 	 if (strncmp( localFile, prefix,prefixLen) == 0) 
 	 {
	   	Reporter.Status("Executing '%s'",localFile);
		ret = WinExec((const char *)localFile + prefixLen,SW_SHOWNORMAL);
	   	Reporter.Status("Executing '%s' (%d)",localFile,ret);
		localFileUnzipped = "";
		ret = 0;
		return(ret);
		
	 }
 	}
	{
	 const char *prefix = "system:";
	 int prefixLen = strlen( prefix );
 	 if (strncmp( localFile, prefix,prefixLen) == 0) 
 	 {
	   	Reporter.Status("Executing command'%s'",localFile);
		ret = system((const char *)localFile + prefixLen);
	   	Reporter.Status("Executing command '%s' (%d)",localFile,ret);
		localFileUnzipped = "";
		ret = 0;
		return(ret);
		
	 }
 	}
#endif

	// check content type for localFileFmt
	if (contentType.GetLength()>0) {
	   TRACE("MIME TYPE : %s\n",(const char *) contentType);

	   int type = ParseMimeType(contentType);

	   // Netscape reports text/plain for png 
	   if (type == UNKNOWN_FMT || type == GZIP_FMT || type >= HTML_FMT) {
		    //TRACE("Unknown Mime Type %s type %d type from filename %d \n",contentType,type,localFileFmt);
			if (localFileFmt != UNKNOWN_FMT) type = localFileFmt; 
	   }

	   localFileFmt = type;
	}

	if (localFileFmt <=0) {  // check file header if format still unknown
		FILE *f;
		f=fopen(localFileUnzipped,"r");
		if (f) {
			char buf[120];
			if (fread(buf,1,40,f) == 40)
				localFileFmt=GetFileType(buf,40);
		fclose(f);
		}
	}


	if (localFileFmt == UNKNOWN_FMT && (url.GetLength()>0)) {
	   callBrowser = 1;
	}

    if (/*!urlLoaded &&*/ callBrowser && url.GetLength()>0) {
	   		localFile = url;
			localFileUnzipped = url;
	 }


#ifdef _WIN32
															  
	if (callBrowser || (localFileFmt == HTML_FMT)) {  // call browser
		if (Flag(WWW_EXECUTE_UNKNOWN_FMTS)) {


		TRACE("Calling browser with %s\n",(const char*)localFileUnzipped);
	    
	    if (AskNetscapeToOpenURL(localFileUnzipped)==1) {
			Reporter.Status("%s passed to Netscape ",(const char* ) localFileUnzipped);
			ret = 0;

		} else 
		if (WWWbrowserPath.GetLength() == 0) {
			if (reporter) reporter->Error("Please configure the full path to your WWW Browser\nin WWW->Options->Config");
			ret = -1;
		}
		else {
	    	CString directory;
	    	CString cmd;
	    
	    	cmd =  WWWbrowserPath;
	    	cmd += " "+localFileUnzipped;


			if (reporter) reporter->Status("%s ...",(const char* ) cmd);

  	    	GetCurrentDirectory(_MAX_PATH,directory.GetBuffer(_MAX_PATH));
	    	directory.ReleaseBuffer();


			PROCESS_INFORMATION pinf;
			STARTUPINFO pstart;
			memset(&pstart,0,sizeof(pstart));
			memset(&pinf,0,sizeof(pinf));
			pstart.cb = sizeof(pstart);
		
			if (CreateProcess(
				NULL,cmd.GetBuffer(_MAX_PATH),
				NULL,NULL,
    			FALSE,	// handle inheritance flag 
    			0,	// creation flags 
    			NULL,	// pointer to new environment block 
    			(const char *)directory,	// pointer to current directory name 
    			&pstart,	// pointer to STARTUPINFO 
				&pinf
   			)) {
				if (!CloseHandle(
        				pinf.hProcess 	// handle of object to close  
   					)) TRACE("Can´t close handle to process\n");;

 		   		ret = 0;
   			} else {
   				Reporter.Error("Can´t execute WWW Browser '%s'.",(const char *) cmd);
				ret=-1;
   			}	
			}
			localFileUnzipped = "";
		}
	} // call browser
	else 
	if ((localFileFmt <= 0 && Flag(WWW_EXECUTE_UNKNOWN_FMTS))) // unknown format 
	{
		CString directory;
   		GetCurrentDirectory(_MAX_PATH,directory.GetBuffer(_MAX_PATH));
		directory.ReleaseBuffer();


	   	Reporter.Status("Loading '%s'",localFile);
		HINSTANCE  inst;
     // HINSTANCE FindExecutable(localFileUnzipped,directroy,program.GetBuffer(_MAX_PATH));

		inst = ShellExecute(
    		AfxGetMainWnd()->GetDesktopWindow()->m_hWnd/* hwnd*/,	// handle to parent window
    		"open", 	// pointer to string that specifies operation to perform
    		localFileUnzipped,	
    		NULL,	// pointer to string that specifies executable-file parameters 
    		directory,	// pointer to string that specifies default directory
    		0 	// whether file is shown when opened
    	);
	   	if (reporter) reporter->Status("Loading '%s' Status = %d",localFile,inst);
		//ERROR_FILE_NOT_FOUND	
	    if  (((int)inst) <= HINSTANCE_ERROR) {
			if (reporter) reporter->Error("Windows Can't execute '%s', %s (%d)",(const char*)localFileUnzipped,GetExecuteErrorMsg((int) inst),(int)inst);
			ret = -1;
		} else {
		  ret = 0;
		}

		localFileUnzipped = "";
	}

	if (localFileUnzipped.GetLength()>0) {
	    // verify file exists
		
		HANDLE f = CreateFile( localFileUnzipped,GENERIC_READ,FILE_SHARE_READ,
    		NULL,	 // address of security descriptor,
    		OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN,NULL);	
		 
		 if  (f == INVALID_HANDLE_VALUE) {
		 	int err = GetLastError();
#ifdef _GLVIEW_DLL
			if (reporter) reporter->Error("File '%s' does not exists (%d)",(const char*)localFileUnzipped,err);
#else
			
			busy ++;

			DWWWRetry d;
			
			if (url.GetLength()>0) d.m_url = url;
			else d.m_url = localFile;
			if (d.DoModal() == IDOK) {
				Set(d.m_url);
				busy--;
				return(Process());
			}
			busy--;
#endif

			ret = -1;
		 } else {
		   long size = GetFileSize(f,NULL);
		   // check for GZIP ???
		   CloseHandle(f);
		 }
    }	
#endif

	return(ret);
}



// is url a www file
// if there is a : colon
int IsUrlFile(const char* url) 
{

 const char *p=strchr(url,':');
 if (!p) return(0);
 int pofs = p-url;
 if (pofs >2) {
	 const char *p1=strchr(url,'\\');
	 if (!p1 || p1 > p) return(1);
 }
 return(0);

#if 0
 int httpLen = strlen( proto_http );
 if ( strncmp( proto_http, url, httpLen) ) return 0;
 return(1);
#endif

}

// is url a local file url : file://
int IsLocalFile(const char* url) 
{
	int localLen = strlen( prefix_file );
	if ( strncmp( prefix_file, url, localLen) ) return 0;
	return(1);
}


// get a cache fileName from url
// IN	url - the url
// OUT  FileName - the generated filename 

int UrlGetLocalFileName(const char* url,char *FileName) 
{
 //int httpLen = strlen( prefix_http );
 //if ( strncmp( prefix_http, url, httpLen) ) return 0;
 
 if (WWWCacheFileFmt==1) { // long format  www.sgi.com_aa_bb.wrl
  		if (strprefix(url,prefix_http))
  			strcpy(FileName,&url[strlen(prefix_http)]);
  		else strcpy(FileName,url);
  		char *ext;
  		// extra arguments to CGI ???
 
// 		ext =strrchr(FileName,'?'); if (ext) *ext = 0; // 20.01.97
  		ext =strrchr(FileName,'#'); if (ext) *ext = 0;

		//test16.wrl_;>_@=& 
		
		ext = FileName;
		while (*ext) {
		  switch(*ext) {
		  case '/':
		  case '\\':
		  case ':':	 
		  case ';':	 
		  case '>':	 
		  case '<':	 
		  case '?':	 
		  case '|':	
		  case '*':	
				 *ext ='_'; break;
		  default :
		      break;
		  }
		  ext ++;

		}
		
		if (strlen(FileName)>250) {
		 strcpy(FileName,&FileName[strlen(FileName)-250]);
		}
 
 } else {
 	// use only base filename
 	const char *s=strrchr(url,'/');
 	if (s) {
  		strcpy(FileName,s+1);
  		char *ext;
  		// extra arguments to CGI ???
 		//ext =strrchr(FileName,'?'); if (ext) *ext = 0;
  		ext =strrchr(FileName,'#'); if (ext) *ext = 0;
  	return(1);
 	}
 }
 return(0);
}

int UrlGetLocalFileNameShort(const char* url,char *FileName) 
{
 int httpLen = strlen( prefix_http );
 //??if ( strncmp( http, url, httpLen) ) return 0;
 
 {
 	// use only base filename
 	const char *s=strrchr(url,'/');
 	if (s) {
  		strcpy(FileName,s+1);
  		char *ext;
  		// extra arguments to CGI ???
 		ext =strrchr(FileName,'?'); if (ext) *ext = 0;
  		ext =strrchr(FileName,'#'); if (ext) *ext = 0;
  	return(1);
 	}
 }
 return(0);
}

#ifndef _WWW
// Abort loading of current load 
int WWWAbortLoad() {
	WWWabort ++;
	return(1);
}

#endif


//////////////////////////////////////////////////////////////////////////
#ifdef _WWW

#ifdef _GSOCK

int WWWInit()
{	int ret;
	ret = AfxSocketInit( &WWWwsadata );
	if (ret == 0) {
		WWWnotAvailable =1;
	}
	WWWinit=1;
	return(ret);
}

class WWWSocket : public CSocket
{
public:
 enum Status {
 		NOT_INITIALIZED,
 		CONNECTED,
		READING,
		WRITING,
		READY,
		IN_ERROR
 };
 
 Status status;
 int lastError;

 int nPort;
 CString machine;

 CString lastMachine;
 CString lastIP;
 int  reuseConnection;

 
 WWWSocket() { 
 		nPort = 80; 
		status = NOT_INITIALIZED;
		lastError=WSAGetLastError();
		reuseConnection = 0;
 		}

 int Open(const char * url,char **data);
// int Close();


 // overwrites of CSocket
 virtual void OnReceive( int nErrorCode );
 virtual void OnSend( int nErrorCode );
 virtual void OnConnect( int nErrorCode );
 virtual void OnClose( int nErrorCode );

 virtual BOOL OnMessagePending( );

 int GetFile(const char *file,char **data);

};

static WWWSocket *currentWWWSocket= NULL;

// Abort loading of current load 
int WWWAbortLoad() {
	WWWabort ++;
	if (currentWWWSocket && currentWWWSocket->IsBlocking()) {
		Reporter.Status("Cancelling Network operation ...");
		currentWWWSocket->CancelBlockingCall();
	}
	return(1);
}

int WWWTerm()
{
	if (currentWWWSocket && currentWWWSocket->IsBlocking()) {
		Reporter.Status("Cancelling Network operation ...");
		currentWWWSocket->CancelBlockingCall();
		// system probly nested
	}
	else 
	if (!busy && currentWWWSocket) {
		delete currentWWWSocket;
		currentWWWSocket = 0;
	}
	return(0);
	//WSA done by MFC at end 
}


void WWWCloseConnection()
{
	if (currentWWWSocket) {
		currentWWWSocket->Close();
		currentWWWSocket->status = WWWSocket::NOT_INITIALIZED;
	}

}


int WWWSocket::Open(const char * url,char **data) 
{
	int ret=-1;
 	if (!WWWinit) {
	  if (!WWWInit()) {
	  	Reporter.Error("Sorry, no internet access available");
		return(-1);
	  }
	} 
	reuseConnection = WWWkeepConnection;
	lastError=WSAGetLastError();
	WWWabort = 0;

	if (WWWnotAvailable) return(-1);

	const char* s;
	const char* file;
	size_t size;
	int machineLen; /* length of machine name */

	const char* http = "http://";
	int httpLen = strlen( http );
	if ( !url ) return -1;
	if ( strncmp( http, url, httpLen) ) return -1;

	/* get the machine name */
	for (s = url + strlen(http) + 1; *s && (*s != ':') && (*s != '/'); s++ )

	machineLen = s - url +1; // HG +1 
	machineLen -= httpLen;

	machine = CString(url + httpLen, machineLen );

	/* get port number */
	if ( *s == ':' ) {
		nPort = atoi( ++s );
		while( *s && (*s != '/') )
			s++;
	} else 
	nPort = 80;


	/* get the file name */
	if ( *s == '/' )
		file = s;
	else
		file = "/";

	 // use existing connenction if machine & port ok

	 if (machine == lastMachine &&
	    reuseConnection && (m_hSocket != INVALID_SOCKET) && (status == CONNECTED)) {
	   		busy ++;
	   		ret = GetFile(file,data);
			busy--;
			return(ret);

	 } else {
	   //if (reuseConnection) 
	   if (m_hSocket != INVALID_SOCKET) Close(); /// close socket !!!!!!!!!!
	   lastMachine = machine;

	 }

 	//Create(nPort,SOCK_STREAM); //,machine);
	//Close();
	if (m_hSocket == INVALID_SOCKET) // not created
		Create();
	lastError=WSAGetLastError();
	
	Reporter.Status("Connecting to %s for `%s'...",(const char *)machine,file);
	busy ++;
 	
 	if (Connect(machine,nPort)) {
		Reporter.Status("Connected to %s for '%s' ...",(const char *)machine,file);
		status = CONNECTED;
		ret = GetFile(file,data);
	}
	else { 
		lastError=WSAGetLastError();
		Reporter.Error("Can't connect to %s (%d) for '%s'",(const char *)machine,lastError,file);
		status = IN_ERROR;
	}
	
	if (!reuseConnection) Close(); /// close socket !!!!!!!!!!

	busy--;

	return(ret);
}

#define bzero(ptr,size) memset(ptr,0,size)
#define bcopy(src,dest,size) memcpy(dest,src,size)


int WWWSocket::GetFile(const char *file,char **data)
{

char buf[BUF_SIZE];
char* results = 0;
/*size_t*/ int size,bytes;
char getString[2048]; /* BUG: use malloc, but freeing after error is a problem */

*data = NULL;

/* send GET message to http */
sprintf(getString,"GET %s\n", file);
if (Send(getString, strlen(getString),0) != strlen(getString)) {
	lastError=WSAGetLastError();
	Reporter.Error("Can get document %s on %s (%d)",file,(const char *)machine,lastError);
	return -1;
}

status = READING;
/* get data */
size = 0;
results = NULL;
busy++;

while( (bytes = Receive(buf,BUF_SIZE,0)) > 0) {

	int newSize = size + bytes;
	results = (char *) realloc( results, newSize+1 );
	assert( results );
	if (results == NULL) break;
	
	bcopy( buf, results + size, bytes );
	size += bytes;
	
	results[size]=0;  // for html data as string

	Reporter.Status("Receiving '%s' (%dk) ...",file,size / 1024);
	if (WWWabort) break;
	//WSAESHUTDOWN 
}
busy--;

lastError=WSAGetLastError();
if (lastError!=0) status = IN_ERROR;
else status = CONNECTED;

//Reporter.Error("Can get document %s on %s (%d)",file,(const char *)machine,lastError);
if (WWWabort) {
	Reporter.Status("Transfer of '%s'  aborted after (%dk) ",file,size / 1024);
	if (results) free(results);
	results = NULL;
	size = -1;
} else 
  Reporter.Status("Received '%s' (%dk) ",file,size / 1024);

*data = results;
return size;
}



void WWWSocket::OnConnect( int nErrorCode ) 
{
   if (nErrorCode == 0) {
   	//	Reporter.Status("Connected");
   }
   CSocket::OnConnect(nErrorCode);
}

void WWWSocket::OnClose( int nErrorCode ) 
{
   if (nErrorCode == 0) {
   		//Reporter.Status("Connection closed");
   }
   CSocket::OnClose(nErrorCode);
}

void WWWSocket::OnReceive( int nErrorCode ) 
{
   if (nErrorCode == 0) {
  // 		Reporter.Status("Receiving");
   }
   CSocket::OnReceive(nErrorCode);
}

void WWWSocket::OnSend( int nErrorCode ) 
{
   if (nErrorCode == 0) {
//   		Reporter.Status("Sending");
   }
   CSocket::OnSend(nErrorCode);
}

BOOL WWWSocket::OnMessagePending()
{
	MSG msg;
	
	if (!((status == CONNECTED) || (status == READING))) return(FALSE);

	if (::PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE) &&
	   ::GetMessage (&msg, NULL, 0, 0)) {
//	   ::TranslateMessage (&msg);
	   ::DispatchMessage (&msg);
		return(TRUE);

	}
/*
	if (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_REMOVE))
	{
		::DispatchMessage(&msg);
		return FALSE;	// usually return TRUE, but OnIdle usually causes WM_PAINTs
	}
	return FALSE;

 	return(CSocket::OnMessagePending()); 
*/
	return FALSE;
}
#else


// Abort loading of current load 
int WWWAbortLoad() 
{											  
	WWWabort ++;
	// call kill on each gfile in use 
	GFile::KillFilesInUse();
	return(1);
}


#endif



const char *
GFile::GetHomeUrl()
{
   if (!urlHome.IsEmpty()) return (urlHome);
   return(localFileHome);

}


const char *
GFile::GetUrl()
{
   if (!url.IsEmpty()) return (url);
   return(localFile);

}


// get full url / localFile including # location

CString GFile::GetFullUrl()
{
	CString s(GetUrl());

	if (!urlExtraArgs.IsEmpty()) {
		s += '#';
		s+= urlExtraArgs;
	}	
	return s;
}



int GFile::WriteUrlDescriptionFile()
{
#if 0
		if (1) {	// write a .url file to retreive full path

            if (strprefix(url,"file://")) return(0); // HG 13.05 don´t write entry for file:/// url´s 

	 		CString urlFile(localFile);
	 		urlFile += ".vrl"; // .url 
	 		FILE *f;
			f = fopen(urlFile,"w");
			if (f) {
				fprintf(f,"url %s\n",(const char *) url);
				fprintf(f,"file %s\n", (const char *)localFile);
				if (size>0) fprintf(f,"size %d\n",size);   
				
				//CTime t = CTime:GetCurrentTime();
				char tmptime[128], tmpdate[128];
				_strtime( tmptime ); _strdate( tmpdate );
				fprintf(f,"time %s %s\n",tmptime,tmpdate);

				if (contentType.GetLength()>0) fprintf(f,"contentType %s\n",(const char *) contentType); // 1.04.96

				if (refedFrom.GetLength()>0) fprintf(f,"refedFrom %s\n",(const char *) refedFrom);
				if (refedType.GetLength()>0) fprintf(f,"refedType %s\n",(const char *) refedType);
				if (lastModified.GetTime() >0) fprintf(f,"lastModified %ld\n",lastModified.GetTime());
				if (expires.GetTime() >0) fprintf(f,"expires %ld\n",expires.GetTime());
    
				// date
				fclose(f);
				return(1);
			}
			else if (reporter) reporter->Error("Can´t write url description file : %s ",(const char *) urlFile);
		}

#endif 

   return(0);
}

// get GFile information from .url file
int GFile::ReadFromUrlDescriptionFile(const char *urlFile)
{
  // read a url from a url description file
  CString urlFileCopy(urlFile);
  return(ReadUrlFile(*this,urlFileCopy));
}


#if 0
// retreive url data and write to local file
int GFile::WriteUrlToFile(int writeDescription)
{ int ret;
	ret = WWWWriteUrlToFile(url,localFile);
	if (ret<0) return(ret);
	size = ret;
	if (writeDescription) WriteUrlDescriptionFile();
	urlLoaded = 1;
	return(ret);
}

#endif

int GFile::WriteUrlToFile(int writeDescription) 
{
  int ret = -1;

#ifdef _WIN32

  char * data = NULL;
  size_t size = 0;

#ifdef G_WININET  
 if (1) {					 
	 ret = InternetOpenUrl(url,&data); 
 } 
#endif


#ifdef _GSOCK 
 else {

     if (!currentWWWSocket) { currentWWWSocket = new WWWSocket; }

	 ret = currentWWWSocket->Open(url,&data); 
 }
#endif

 if (ret>=0) {

	size_t size = ret;
	if ((ret == 0) || (data == NULL)) {
		//ret =  ERROR_HTML_NO_DATA;
	    ret = -1;
	} else {


	if (IsHtmlData(data)) 
	{
		ret =  ERROR_HTML_DATA;
		if (IsRedirectedUrl(data,redirectUrl.GetBuffer(_MAX_PATH))) {
			ret =  ERROR_REDIRECTED_URL;

		}
		redirectUrl.ReleaseBuffer();

		if (ret !=  ERROR_REDIRECTED_URL && size < 500) {
			CString errString;
			errString = "HTML Result for ";
			errString += url;
			errString += "\n";
			errString += data;

			AfxMessageBox(errString, MB_ICONEXCLAMATION | MB_OK) ;
			//Reporter.Error("HTML result for %s \n%s",url,data);
			ret =  ERROR_HTML_DATA_REPORTED;
		}
	}
	else {

		if (IsZippedData(data)) {
			if (!IsZippedFile(localFile)) {
				localFile += ".gz";
			}
		}	

	// write data 
	const char *file = localFile;
		
	FILE *f;
	ret = -1;
	f = fopen(file,"wb");
	//EAI_TRACE("Writing cache file %s  ok &d\n",(const char *)file,f !=NULL);

	if (f) {
		if (fwrite(data,size,1,f) == 1) {
		   ret = size;
		   cacheDownloadSize += size;

		}	else {
			Reporter.Error("Failed to write cache file : %s ",(const char *) file);
		}
		fclose(f);
		if (writeDescription) {	// write a .url file to retreive full path
	 		CString urlFile(file);
	 		urlFile += ".vrl"; // url
	 		FILE *f;
			f = fopen(urlFile,"w");
			if (f) {
				fprintf(f,"url %s\n",(const char *) url);
				fprintf(f,"file %s\n", file);
				
				if (contentType.GetLength()>0) fprintf(f,"ContentType %s\n",(const char *) contentType);
				fprintf(f,"size %d\n",size);   
				
				//CTime t = CTime:GetCurrentTime();
				char tmptime[128], tmpdate[128];
				_strtime( tmptime ); _strdate( tmpdate );
				fprintf(f,"time %s %s\n",tmptime,tmpdate);
    
				// date
			};
			fclose(f);
	}
	} //if f
	else Reporter.Error("Can´t write cache file : %s ",(const char *) file);
	}
   }
}
if (data) free(data);

#endif

return(ret);	
}

#endif


/* Global variable */

static DWORD dwInstID=0L;

#ifdef _WIN32

HDDEDATA CALLBACK DdeCallback(

    UINT  uType,	// transaction type
    UINT  uFmt,	// clipboard data format
    HCONV  hconv,	// handle of the conversation
    HSZ  hsz1,	// handle of a string
    HSZ  hsz2,	// handle of a string
    HDDEDATA  hdata,	// handle of a global memory object
    DWORD  dwData1,	// transaction-specific data
    DWORD  dwData2 	// transaction-specific data
   ) 
{

   return(0);
}

#endif

#ifdef  Linux

int GetURLFromLocalFile(const char * szLocal,  char * szURL)
{
    return(-1);
}

int AskNetscapeToOpenURL(const char * szURL)
{
    //to do: ie
    return -1;
}
#endif

#ifdef  _GLVIEW_IE

// retreive the url for a local file
// not needed for IE

int GetURLFromLocalFile(const char * szLocal,  char * szURL)
{
	return(-1);
}

int AskNetscapeToOpenURL(const char * szURL)
{
	//to do: ie 
	return -1;
}

#else

#ifdef _WIN32

// retreive the url for a local file
// Netscape Helper App version: use DDE to get URL
int GetURLFromLocalFile(const char * szLocal,  char * szURL)
{
    
    int ret= -1;

    HSZ hszFindItem;
    HSZ hszSearchTopic;
    HSZ hszApp;

    WORD          wDDEMLError;

    dwInstID = 0L ;


    if (DdeInitialize(&dwInstID, DdeCallback,
                      APPCMD_CLIENTONLY,
                      0L))
    {
        MessageBox(NULL, "Can´t initialize DDE", NULL, MB_OK) ;
        return(-1); ;
    }

    hszApp = DdeCreateStringHandle(dwInstID,
                                   (LPSTR)"NETSCAPE",
                                   CP_WINANSI );

    hszSearchTopic = DdeCreateStringHandle(dwInstID,
                                           (LPSTR)"WWW_QueryURLFile",
                                           CP_WINANSI);

    hszFindItem = DdeCreateStringHandle(dwInstID,
                                        (LPSTR)szLocal,
                                         CP_WINANSI );

    
    CONVCONTEXT ccInfo;

	memset(&ccInfo,0,sizeof(ccInfo));
	ccInfo.cb = sizeof(ccInfo);


    ccInfo.iCodePage = CP_WINANSI;
    HCONV hConv = DdeConnect(dwInstID, hszApp, hszSearchTopic, &ccInfo) ;

    if(!hConv)
    {
        /* If the conversation couldn't be started, let the caller know why. */
        wDDEMLError = DdeGetLastError(dwInstID);
    }
    else
    {
        DWORD        dwLength;

        HDDEDATA hddResult = DdeClientTransaction(NULL, //(VOID FAR *)NULL,
                                 (DWORD)0,
                                 hConv,
                                 hszFindItem,
                                 CF_TEXT,
                                 XTYP_REQUEST,
                                 100000,
                                 (LPDWORD)NULL) ;

        if (hddResult > 0)
        {
            LPSTR lpszOurData = (LPSTR) DdeAccessData(hddResult
				,(LPDWORD)&dwLength);

            if (lpszOurData)
            {
                if (lstrlen(lpszOurData) > 0)
                {
                    lstrcpy(szURL, lpszOurData+1) ;
                    szURL[lstrlen(szURL)-1] = '\0' ;
					ret = 1;
                }
            }

            DdeUnaccessData(hddResult);
            DdeFreeDataHandle(hddResult);
        }
        else
        {
            lstrcpy(szURL, "") ;
			ret = 0;
        }

    }

    DdeFreeStringHandle(dwInstID , hszFindItem );
    DdeFreeStringHandle(dwInstID , hszSearchTopic );
    DdeFreeStringHandle(dwInstID , hszApp );

    if (hConv)
    {
        BOOL bStatus = DdeDisconnect(hConv) ;

        if(!bStatus)
        {
            /* If the conversation couldn't be ended, let the caller know why. */
            wDDEMLError = DdeGetLastError(dwInstID);
        }
    }
	
	return(ret);
}


// ask the WEB brower to show the indicated URL
int AskNetscapeToOpenURL(const char * szURL)
{
	int ret = -1;

    WORD          wDDEMLError;
    char          szStringSpace[_MAX_PATH*2] ;

    HSZ hszFindItem;
    HSZ hszSearchTopic;
    HSZ hszApp;

// file:///C|/HTML/DDEAPI.HTM

// If this is not a URL, create a URL from the local filespec.
/// This code is DOS specific and Netscape specific!
    const char *del = strchr(szURL,':');

	if (del &&  (del - szURL) <=2) del = NULL; /* a: b:  mailto: */


    if (del || (strnicmp(szURL, "file:///", 8) == 0)
     	    || (strnicmp(szURL, "http://",  7) == 0))
    {
        szStringSpace[0] = '"';
        lstrcpy(&szStringSpace[1], szURL) ;
        lstrcat(szStringSpace, "\"") ;
    }
    // Must be a local file!
    else
    {
        char    szDrive[8] ;
        char    szDir[_MAX_PATH] ;
        char    szFile[_MAX_PATH] ;
        char    szExt[_MAX_PATH] ;
        int     i = 0 ;

        _splitpath(szURL, szDrive, szDir, szFile, szExt) ;

        lstrcpy(szStringSpace, "file:///") ;

        szDrive[1] = '|' ;
        szDrive[2] = 0 ;
        lstrcat(szStringSpace, szDrive) ;

        while(szDir[i] != '\0')
        {
            if (szDir[i] == '\\')
                szDir[i] = '/' ;
            i++ ;
        }

        lstrcat(szStringSpace, szDir) ;
        lstrcat(szStringSpace, szFile) ;
        lstrcat(szStringSpace, szExt) ;

    }


// Add the rest of the arguments
    lstrcat(szStringSpace, ",,0xFFFFFFFF,0x0") ;

    dwInstID = 0L ;

	// (PFNCALLBACK)MakeProcInstance((FARPROC)DDEClientCallback,ghInst),
    if(DdeInitialize(&dwInstID,
                     DdeCallback,
                     APPCMD_CLIENTONLY,
                     0L))
    {
        MessageBox(NULL, "Can´t initialize DDE", NULL, MB_OK) ;
        return(-1);
    }

    hszApp = DdeCreateStringHandle(dwInstID,
                                   (LPSTR)"NETSCAPE",
                                   CP_WINANSI );

    hszSearchTopic = DdeCreateStringHandle(dwInstID,
                                           (LPSTR)"WWW_OpenURL",
                                           CP_WINANSI);

    hszFindItem = DdeCreateStringHandle(dwInstID,
                                        (LPSTR)szStringSpace,
                                         CP_WINANSI );

    CONVCONTEXT ccInfo;
	memset(&ccInfo,0,sizeof(ccInfo));
	ccInfo.cb = sizeof(ccInfo);

    ccInfo.iCodePage = CP_WINANSI;

    HCONV hConv = DdeConnect(dwInstID, hszApp, hszSearchTopic, &ccInfo) ;

    if(!hConv)
    {
        /* If the conversation couldn't be started, let the caller know why. */
        wDDEMLError = DdeGetLastError(dwInstID);
    }
    else
    {
        HDDEDATA hddResult = DdeClientTransaction(NULL, //(VOID FAR *)NULL,
                                 (DWORD)0,
                                 hConv,
                                 hszFindItem,
                                 CF_TEXT,
                                 XTYP_REQUEST,
                                 100000,
                                 (LPDWORD)NULL) ;

        if (hddResult > 0)
        {
            //DdeFreeDataHandle(hddResult); // Need to do this?????
			ret=1;
        }
    }

    DdeFreeStringHandle(dwInstID , hszFindItem );
    DdeFreeStringHandle(dwInstID , hszSearchTopic );
    DdeFreeStringHandle(dwInstID , hszApp );

    if (hConv)
    {
        BOOL bStatus = DdeDisconnect(hConv) ;

        if(!bStatus)
        {
            /* If the conversation couldn't be ended, let the caller know why.
*/
            wDDEMLError = DdeGetLastError(dwInstID);
        }
    }
	return ret;
}
#endif
#endif


#if 0
int NetscapeInit(HWND hwndClientDDE)
{
static BOOL fInInitiate = FALSE;
    char *szApplication="NETSCAPE";
    char *szTopic="WWW_OpenURL";
	
	ATOM atomApplication;
	ATOM atomTopic;

    
    atomApplication = *szApplication == 0 ?
        NULL : GlobalAddAtom((LPSTR) szApplication);
    atomTopic = *szTopic == 0 ?
        NULL : GlobalAddAtom((LPSTR) szTopic);

    fInInitiate = TRUE;
    SendMessage((HWND) -1,        /* broadcasts message          */
        WM_DDE_INITIATE,          /* initiates conversation      */
        (WPARAM) hwndClientDDE,   /* handle of client DDE window */
        MAKELONG(atomApplication, /* application-name atom       */
            atomTopic));          /* topic-name atom             */
    fInInitiate = FALSE;
    if (atomApplication != NULL)
        GlobalDeleteAtom(atomApplication);
    if (atomTopic != NULL)
        GlobalDeleteAtom(atomTopic);
}
#endif



//
// Helper function
//
BOOL Succeeded(HANDLE h, LPCTSTR strFunctionName) ;

//
// Closes the Internet session so InternetOpen will be called on next GetPage.
//
void GFile::ResetSession()
{
   TRACE0("Resetting Session\r\n") ;

#ifdef G_WININET
   if (hSession != NULL)
   {
      ::InternetCloseHandle(hSession);
      hSession = NULL ;
   }   
#endif
}


struct internet_error_ {
		int code;
		const char *message;
		} internetErrors[] = 

		{	  

#ifdef _WIN32

  { ERROR_INVALID_PARAMETER ,"Invalid Parameter"  }


 // from winsock.h VSC. 2.2

  ,{ WSAEINTR                ,"EINTR"              }
  ,{ WSAEBADF                ,"EBADF"              }
  ,{ WSAEACCES               ,"EACCES"             }
  ,{ WSAEFAULT               ,"EFAULT"             }
  ,{ WSAEINVAL               ,"EINVAL"             }
  ,{ WSAEMFILE               ,"EMFILE"             }
  ,{ WSAEWOULDBLOCK          ,"EWOULDBLOCK"        }
  ,{ WSAEINPROGRESS          ,"EINPROGRESS"        }
  ,{ WSAEALREADY             ,"EALREADY"           }
  ,{ WSAENOTSOCK             ,"ENOTSOCK"           }
  ,{ WSAEDESTADDRREQ         ,"EDESTADDRREQ"       }
  ,{ WSAEMSGSIZE             ,"EMSGSIZE"           }
  ,{ WSAEPROTOTYPE           ,"EPROTOTYPE"         }
  ,{ WSAENOPROTOOPT          ,"ENOPROTOOPT"        }
  ,{ WSAEPROTONOSUPPORT      ,"EPROTONOSUPPORT"    }
  ,{ WSAESOCKTNOSUPPORT      ,"ESOCKTNOSUPPORT"    }
  ,{ WSAEOPNOTSUPP           ,"EOPNOTSUPP"         }
  ,{ WSAEPFNOSUPPORT         ,"EPFNOSUPPORT"       }
  ,{ WSAEAFNOSUPPORT         ,"EAFNOSUPPORT"       }
  ,{ WSAEADDRINUSE           ,"EADDRINUSE"         }
  ,{ WSAEADDRNOTAVAIL        ,"EADDRNOTAVAIL"      }
  ,{ WSAENETDOWN             ,"ENETDOWN"           }
  ,{ WSAENETUNREACH          ,"ENETUNREACH"        }
  ,{ WSAENETRESET            ,"ENETRESET"          }
  ,{ WSAECONNABORTED         ,"ECONNABORTED"       }
  ,{ WSAECONNRESET           ,"ECONNRESET"         }
  ,{ WSAENOBUFS              ,"ENOBUFS"            }
  ,{ WSAEISCONN              ,"EISCONN"            }
  ,{ WSAENOTCONN             ,"ENOTCONN"           }
  ,{ WSAESHUTDOWN            ,"ESHUTDOWN"          }
  ,{ WSAETOOMANYREFS         ,"ETOOMANYREFS"       }
  ,{ WSAETIMEDOUT            ,"Timeout"          }
  ,{ WSAECONNREFUSED         ,"Connection refused"       }
  ,{ WSAELOOP                ,"ELOOP"              }
  ,{ WSAENAMETOOLONG         ,"Name to long"     }
  ,{ WSAEHOSTDOWN            ,"Host down"          }
  ,{ WSAEHOSTUNREACH         ,"Host unreachable"    }
  ,{ WSAENOTEMPTY            ,"ENOTEMPTY"          }
  ,{ WSAEPROCLIM             ,"EPROCLIM"           }
  ,{ WSAEUSERS               ,"EUSERS"             }
  ,{ WSAEDQUOT               ,"EDQUOT"             }
  ,{ WSAESTALE               ,"ESTALE"             }
  ,{ WSAEREMOTE              ,"EREMOTE"            }
							                          
  ,{ WSAEDISCON              ,"EDISCON"            }
							                          
/*							                          
 * Extended Windows Sockets errtended Windows Sockets tant definitions
 */							                          
  ,{ WSASYSNOTREADY          ,"SYSNOTREADY"        }
  ,{ WSAVERNOTSUPPORTED      ,"VERNOTSUPPORTED"    }
  ,{ WSANOTINITIALISED       ,"NOTINITIALISED"     }
  ,{ WSAHOST_NOT_FOUND       ,"HOST_NOT_FOUND"     }
  ,{ WSATRY_AGAIN            ,"TRY_AGAIN"          }
							                          
/* Non recoverable errors, FORMn recoverable errors, FFUSED, NOTIMP */
  ,{ WSANO_RECOVERY          ,"NO_RECOVERY"        }
  							                          
/* Valid name, no data record olid name, no data recorsted type */
  ,{ WSANO_DATA              ,"NO_DATA"            }
/* no address, look for MX reco address, look for MX  */
  ,{ WSANO_ADDRESS           ,"WSWSANO_ADDRESS"       }


, { ERROR_INTERNET_OUT_OF_HANDLES           ,  "INTERNET_OUT_OF_HANDLES"}
, { ERROR_INTERNET_TIMEOUT                  ,  "INTERNET_TIMEOUT"}
, { ERROR_INTERNET_EXTENDED_ERROR           ,  "INTERNET_EXTENDED_ERROR"}
, { ERROR_INTERNET_INTERNAL_ERROR           ,  "INTERNET_INTERNAL_ERROR"}
, { ERROR_INTERNET_INVALID_URL              ,  "Invalid URL"}
, { ERROR_INTERNET_UNRECOGNIZED_SCHEME      ,  "INTERNET_UNRECOGNIZED_SCHEME"}
, { ERROR_INTERNET_NAME_NOT_RESOLVED        ,  "Name not resolved"}
, { ERROR_INTERNET_PROTOCOL_NOT_FOUND       ,  "INTERNET_PROTOCOL_NOT_FOUND"}
, { ERROR_INTERNET_INVALID_OPTION           ,  "INTERNET_INVALID_OPTION"}
, { ERROR_INTERNET_BAD_OPTION_LENGTH        ,  "INTERNET_BAD_OPTION_LENGTH"}
, { ERROR_INTERNET_OPTION_NOT_SETTABLE      ,  "INTERNET_OPTION_NOT_SETTABLE"}
, { ERROR_INTERNET_SHUTDOWN                 ,  "INTERNET_SHUTDOWN"}
, { ERROR_INTERNET_INCORRECT_USER_NAME      ,  "INTERNET_INCORRECT_USER_NAME"}
, { ERROR_INTERNET_INCORRECT_PASSWORD       ,  "INTERNET_INCORRECT_PASSWORD"}
, { ERROR_INTERNET_LOGIN_FAILURE            ,  "Login failure"}
, { ERROR_INTERNET_INVALID_OPERATION        ,  "INTERNET_INVALID_OPERATION"}
, { ERROR_INTERNET_OPERATION_CANCELLED      ,  "INTERNET_OPERATION_CANCELLED"}
, { ERROR_INTERNET_INCORRECT_HANDLE_TYPE    ,  "INTERNET_INCORRECT_HANDLE_TYPE"}
, { ERROR_INTERNET_INCORRECT_HANDLE_STATE   ,  "INTERNET_INCORRECT_HANDLE_STATE"}
, { ERROR_INTERNET_NOT_PROXY_REQUEST        ,  "INTERNET_NOT_PROXY_REQUEST"}
, { ERROR_INTERNET_REGISTRY_VALUE_NOT_FOUND ,  "INTERNET_REGISTRY_VALUE_NOT_FOUND"}
, { ERROR_INTERNET_BAD_REGISTRY_PARAMETER   ,  "INTERNET_BAD_REGISTRY_PARAMETER"}
, { ERROR_INTERNET_NO_DIRECT_ACCESS         ,  "INTERNET_NO_DIRECT_ACCESS"}
, { ERROR_INTERNET_NO_CONTEXT               ,  "INTERNET_NO_CONTEXT"}             
, { ERROR_INTERNET_NO_CALLBACK              ,  "INTERNET_NO_CALLBACK"}            
, { ERROR_INTERNET_REQUEST_PENDING          ,  "INTERNET_REQUEST_PENDING"}        
, { ERROR_INTERNET_INCORRECT_FORMAT         ,  "INTERNET_INCORRECT_FORMAT"}       
, { ERROR_INTERNET_ITEM_NOT_FOUND           ,  "INTERNET_ITEM_NOT_FOUND"}         
, { ERROR_INTERNET_CANNOT_CONNECT           ,  "INTERNET_CANNOT_CONNECT"}         
, { ERROR_INTERNET_CONNECTION_ABORTED       ,  "INTERNET_CONNECTION_ABORTED"}     
, { ERROR_INTERNET_CONNECTION_RESET         ,  "INTERNET_CONNECTION_RESET"}       
, { ERROR_INTERNET_FORCE_RETRY              ,  "INTERNET_FORCE_RETRY"}            
, { ERROR_INTERNET_INVALID_PROXY_REQUEST    ,  "INTERNET_INVALID_PROXY_REQUEST"}  
, { ERROR_INTERNET_NEED_UI                  ,  "INTERNET_NEED_UI"}                
											                                
, { ERROR_INTERNET_HANDLE_EXISTS            ,  "INTERNET_HANDLE_EXISTS"}          
, { ERROR_INTERNET_SEC_CERT_DATE_INVALID    ,  "INTERNET_SEC_CERT_DATE_INVALID"}  
, { ERROR_INTERNET_SEC_CERT_CN_INVALID      ,  "INTERNET_SEC_CERT_CN_INVALID"}    
, { ERROR_INTERNET_HTTP_TO_HTTPS_ON_REDIR   ,  "INTERNET_HTTP_TO_HTTPS_ON_REDIR"} 
, { ERROR_INTERNET_HTTPS_TO_HTTP_ON_REDIR   ,  "INTERNET_HTTPS_TO_HTTP_ON_REDIR"} 
, { ERROR_INTERNET_MIXED_SECURITY           ,  "INTERNET_MIXED_SECURITY"}         
, { ERROR_INTERNET_CHG_POST_IS_NON_SECURE   ,  "INTERNET_CHG_POST_IS_NON_SECURE"} 
, { ERROR_INTERNET_POST_IS_NON_SECURE       ,  "INTERNET_POST_IS_NON_SECURE"}     
, { ERROR_INTERNET_CLIENT_AUTH_CERT_NEEDED  ,  "INTERNET_CLIENT_AUTH_CERT_NEEDED"}
, { ERROR_INTERNET_INVALID_CA               ,  "INTERNET_INVALID_CA"}             
, { ERROR_INTERNET_CLIENT_AUTH_NOT_SETUP    ,  "INTERNET_CLIENT_AUTH_NOT_SETUP"}  
, { ERROR_INTERNET_ASYNC_THREAD_FAILED      ,  "INTERNET_ASYNC_THREAD_FAILED"}    
, { ERROR_INTERNET_REDIRECT_SCHEME_CHANGE   ,  "INTERNET_REDIRECT_SCHEME_CHANGE"} 


// FTP API errors								 ors                               
, { ERROR_FTP_TRANSFER_IN_PROGRESS          ,  "FTP_TRANSFER_IN_PROGRESS"}
//??, { ERROR_FTP_CONNECTED                     ,  "FTP_CONNECTED"}
, { ERROR_FTP_DROPPED                       ,  "FTP_DROPPED"}
		// gopher API errors					 her API errors                    
, { ERROR_GOPHER_PROTOCOL_ERROR             ,  "GOPHER_PROTOCOL_ERROR"}
, { ERROR_GOPHER_NOT_FILE                   ,  "GOPHER_NOT_FILE"}
, { ERROR_GOPHER_DATA_ERROR                 ,  "GOPHER_DATA_ERROR"}
, { ERROR_GOPHER_END_OF_DATA                ,  "GOPHER_END_OF_DATA"}
, { ERROR_GOPHER_INVALID_LOCATOR            ,  "GOPHER_INVALID_LOCATOR"}
, { ERROR_GOPHER_INCORRECT_LOCATOR_TYPE     ,  "GOPHER_INCORRECT_LOCATOR_TYPE"}
, { ERROR_GOPHER_NOT_GOPHER_PLUS            ,  "GOPHER_NOT_GOPHER_PLUS"}
, { ERROR_GOPHER_ATTRIBUTE_NOT_FOUND        ,  "GOPHER_ATTRIBUTE_NOT_FOUND"}
, { ERROR_GOPHER_UNKNOWN_LOCATOR            ,  "GOPHER_UNKNOWN_LOCATOR"}
	// HTTP API errors						 P API errors                      

, { ERROR_HTTP_HEADER_NOT_FOUND             ,  "HTTP_HEADER_NOT_FOUND"}           
, { ERROR_HTTP_DOWNLEVEL_SERVER             ,  "HTTP_DOWNLEVEL_SERVER"}           
, { ERROR_HTTP_INVALID_SERVER_RESPONSE      ,  "HTTP_INVALID_SERVER_RESPONSE"}    
, { ERROR_HTTP_INVALID_HEADER               ,  "HTTP_INVALID_HEADER"}             
, { ERROR_HTTP_INVALID_QUERY_REQUEST        ,  "HTTP_INVALID_QUERY_REQUEST"}      
, { ERROR_HTTP_HEADER_ALREADY_EXISTS        ,  "HTTP_HEADER_ALREADY_EXISTS"}      
, { ERROR_HTTP_REDIRECT_FAILED              ,  "HTTP_REDIRECT_FAILED"}            
, { ERROR_HTTP_NOT_REDIRECTED               ,  "HTTP_NOT_REDIRECTED"}             
											                                
, { ERROR_INTERNET_SECURITY_CHANNEL_ERROR   ,  "INTERNET_SECURITY_CHANNEL_ERROR"} 
, { ERROR_INTERNET_UNABLE_TO_CACHE_FILE     ,  "INTERNET_UNABLE_TO_CACHE_FILE"}   
, { ERROR_INTERNET_TCPIP_NOT_INSTALLED      ,  "INTERNET_TCPIP_NOT_INSTALLED"}   
,

#endif

{ 0,NULL }

};

// ??? InternetErrorDl

const char *InternetErrorMessage(int code) 
{
	for(int i=0; internetErrors[i].message != NULL;i++) 
    	if (internetErrors[i].code == code) return internetErrors[i].message;
     return("unknown");
}


#ifdef G_WININET


//
//
//
BOOL GFile::ReportInternetError(HANDLE h, LPCTSTR strFunctionName)
{
   if (h == NULL)
   {
      DWORD dwError = GetLastError() ;
      // what is error 84
      TRACE("%s *** Error = %s (%u) ***\r\n",strFunctionName,InternetErrorMessage(dwError), dwError) ;
	  DWORD iError,bufferLength=1023;

	  CString buffer;
	  BOOL iErr = InternetGetLastResponseInfo(&iError,buffer.GetBuffer(1024),&bufferLength);
	  buffer.ReleaseBuffer(); 
	  if (dwError == lastReportedInternetError) {
		  // ignore message if already reported 
	  } else {
		  if (Flag(WWW_REPORT_ERRORS)) {

			  lastReportedInternetError = dwError; 
			  if (reporter) reporter->Error("%s returns error %s (%u) %s\nfor %s", strFunctionName,InternetErrorMessage(dwError), dwError,(LPCTSTR) buffer,(const char*) url);
		  }
	  }	
      return FALSE;
   }
   else
   {
      //TRACE("%s\r\n", strFunctionName) ;
      return TRUE ;
   }

}


BOOL Succeeded(HANDLE h, LPCTSTR strFunctionName)
{
   if (h == NULL)
   {
      DWORD dwError = GetLastError() ;
      TRACE("%s *** Error = %u***\r\n",strFunctionName, InternetErrorMessage(dwError),dwError) ;
	  DWORD iError,bufferLength=1023;
	  CString buffer;
	  BOOL iErr = InternetGetLastResponseInfo(&iError,buffer.GetBuffer(1024),&bufferLength);
	  buffer.ReleaseBuffer(); 

      //CString errString ;
	  Reporter.Error("%s returns error %s (%u) %s", 
	  strFunctionName,InternetErrorMessage(dwError),dwError,(LPCTSTR) buffer) ;
	  //AfxMessageBox(errString, MB_ICONEXCLAMATION | MB_OK) ;
      return FALSE;
   }
   else
   {
      //TRACE("%s\r\n", strFunctionName) ;
      return TRUE ;
   }

}

BOOL ReportError(DWORD dwError, LPCTSTR strFunctionName)
{
      TRACE("%s *** Error = %u***\r\n",strFunctionName, dwError) ;
	  DWORD iError,bufferLength=1023;
	  CString buffer;
	  BOOL iErr = InternetGetLastResponseInfo(&iError,buffer.GetBuffer(1024),&bufferLength);
	  buffer.ReleaseBuffer(); 

      //CString errString ;

      Reporter.Error("%s returns error %u %s", strFunctionName, dwError,(LPCTSTR) buffer) ;
      //AfxMessageBox(errString, MB_ICONEXCLAMATION | MB_OK) ;
      return FALSE;
}

#endif


int GFile::Kill() 
{
	stop = 1;			// set the stop flag
	reporter = NULL;	//  don't report any errors
	hPostMsgWnd = NULL; // no more notification messages 

#ifdef _COM
	// BindStatuscallback 
	if(m_pbinding != NULL ) 
	{
		// is this safe from other thread !??? 
		TRACE(" GFile::killing  binding from other thread ");
		// m_pbinding->Abort();

	}
#endif 

#ifdef G_WININET
	// for WININET calls 
	if (hInternetFile) {
    	//VERIFY(::InternetCloseHandle(hInternetFile)); 
    	::InternetCloseHandle(hInternetFile); 
		hInternetFile = NULL;
	}

  	if (hInternetConnection) {
      //VERIFY(::InternetCloseHandle(hInternetConnection)) ;
      ::InternetCloseHandle(hInternetConnection);
	  hInternetConnection = NULL;
	  return(1);
   	} 
#endif

	// TO DO : netscape case : pass kill notification to stream 
	return(0);
}



#ifdef G_WININET
// wininet related HTTP/FTP access 
//
// Load a HTTP url 
//

// old sweeper interface
BOOL QueryInfo(
    HINTERNET hHttpRequest,
    WORD   what,
    LPVOID lpvBuffer,
    LPDWORD lpvBufferLength
   ) 
{  DWORD index = 0;

   return ::HttpQueryInfo(hHttpRequest,what,lpvBuffer,lpvBufferLength,&index);
}
//HTTP_QUERY_IF_MODIFIED_SINCE
//HTTP_QUERY_EXPIRES                      
//HTTP_QUERY_LAST_MODIFIED  

// get InternetOpen accessType as index for dialog
int GFile::GetAccessTypeIndex() 
{
   int index = 0 ;
   switch (accessType)
   {
   case PRE_CONFIG_INTERNET_ACCESS:
      index = 0 ;
      break;

   case CERN_PROXY_INTERNET_ACCESS:
      index = 1 ;
      break ;

   case GATEWAY_INTERNET_ACCESS:
      index = 2 ;
      break ;

   case LOCAL_INTERNET_ACCESS:
      index = 3 ;
      break ;
   default:
      ASSERT(0) ;
   }
   return index ;
}


// Sets the Access Type using an Index
void GFile::SetAccessTypeIndex(int index) 
{
   // This function is tied to the dialog box...
   switch(index)
   {
   case 0:
      accessType = PRE_CONFIG_INTERNET_ACCESS ;
      break;
   case 1:
      accessType = CERN_PROXY_INTERNET_ACCESS ;
      break;
   case 2:
      accessType = GATEWAY_INTERNET_ACCESS ;
      break;
   case 3:
      accessType = LOCAL_INTERNET_ACCESS ;
      break;
   default:
      ASSERT(0) ;
   }
   //ResetSession() ;
}




// set settings off internet session
int GFile::InternetOpen()
{
    if (hSession != NULL) {
        InternetCloseHandle(hSession);
        hSession = NULL;
    }   


    if (hSession == NULL)
    {
      //
      // Initialize the Internet Functions.
      //
      TRACE("Starting Session (Access = %i) (Proxy = %s)\r\n",
            GetAccessTypeIndex(), (LPCTSTR)proxyServer) ;
      
      hSession = ::InternetOpen(userAgent, accessType,proxyServer,proxyByPass /* proxyPort*/, internetOpenFlags);

      if (!Succeeded(hSession, "InternetOpen"))
      {
         return 0;
      }
      return(1);
   }
   return(0);
   
}

long GFile::LoadUrlHTTP(const char *m_strServer, int port, const char *m_strPath,char **data)
{
    long uiResult = THREAD_BAD;
	
	*data = NULL;
	char buf[BUF_SIZE];
	char* results = NULL;
	/*size_t*/ int size=0,bytes=0;
	*data = NULL;

	int multiPartBegin; // 1 seeking for header


    if (hSession == NULL)
    {
      //
      // Initialize the Internet Functions.
      //
      TRACE("Starting Session (Access = %i) (Proxy = %s)\r\n",
            GetAccessTypeIndex(), (LPCTSTR)proxyServer) ;
      
      hSession = ::InternetOpen(userAgent, accessType,proxyServer,proxyByPass /* proxyPort*/, internetOpenFlags);

      if (!ReportInternetError(hSession, "InternetOpen"))
      {
         // Send message to UI that we finished.
//         ::PostMessage(m_hPostMsgWnd,WM_READFILECOMPLETED, NULL, (LPARAM)THREAD_BAD) ;

         return THREAD_BAD;
      }
   }

   if (reporter) reporter->Status("Connecting to %s ...",(const char *)m_strServer);

   hInternetConnection = ::InternetConnect(hSession,m_strServer,port /*INVALID_PORT_NUMBER*/,
                                          "" /* username */,  "" /* password */,
                                          INTERNET_SERVICE_HTTP,
                                          0,
                                          0) ;

                                          
   if (!WWWabort && !stop && ReportInternetError(hInternetConnection, "InternetConnect"))
   {

	  if (reporter) reporter->Status("Connecting to %s for `%s'...",(const char *)m_strServer,m_strPath);


      CString hdr;

	 
      hInternetFile = ::HttpOpenRequest(hInternetConnection,
                                              "GET",
                                              m_strPath,
                                              HTTP_VERSION,
                                              refedFrom, // NULL, // refedFrom,
                                              acceptTypes, // NULL,	/*  LPCTSTR FAR * lplpszAcceptTypes, */
                                              INTERNET_FLAG_EXISTING_CONNECT | (useInetCache ? 0 :  INTERNET_FLAG_DONT_CACHE)
											  | (Flag(WWW_RELOAD) ? INTERNET_FLAG_RELOAD : 0), 
                                              0) ;

      if (ReportInternetError(hInternetFile, "HttpOpenRequest"))
      {
	     hdr = "User-Agent:"+userAgent+"\r\n";
		 
		 BOOL hAddRequest = ::HttpAddRequestHeaders( hInternetFile, (const char *) hdr,-1l,HTTP_ADDREQ_FLAG_REPLACE|HTTP_ADDREQ_FLAG_ADD) ;
		 // can only handle one request ??
		 //hdr = "Accept:" + acceptTypes + "\r\n";
/*
		if (refedFrom.GetLength()>0) { 
			hdr="Referer:";
			hdr+=refedFrom;
			hdr+="\r\n";
		}
*/


         BOOL bSendRequest = ::HttpSendRequest(hInternetFile, NULL, /*"",*/ 0, NULL, 0);
         
         if (ReportInternetError((HINTERNET)bSendRequest, "HttpSendRequest"))
         {

            // Get size of file.
            char bufQuery[128] ;
            DWORD dwFileSize ;
			DWORD status;
			int moved = 0;
			
			/* get data */
			size = 0;
			results = NULL;


            DWORD dwLengthBufQuery = sizeof (bufQuery);
            BOOL bQuery = QueryInfo(hInternetFile,HTTP_QUERY_CONTENT_LENGTH,bufQuery,&dwLengthBufQuery) ;
	  		Reporter.Status("Retreiving 'http://%s%s'...",(const char *)m_strServer,m_strPath);

            if (bQuery) {
               // The Query was successful
               TRACE("HttpQueryInfo FileSize is %s.\r\n", bufQuery) ;
               dwFileSize = (DWORD)atol(bufQuery) ;
			   contentLength = dwFileSize;
            }
            else
            {
               // The Query failed
			   contentLength = 0;
			   DWORD dwError = GetLastError() ;
			   if (dwError != ERROR_HTTP_HEADER_NOT_FOUND)
			      // 1.04.96 ReportError(dwError,"HttpQueryInfo");

               TRACE("\tQueryInfo Failed.\r\n") ;
               dwFileSize = 5*1024 ;
            }

			dwLengthBufQuery = sizeof (bufQuery);
            bQuery = QueryInfo(hInternetFile,HTTP_QUERY_STATUS_CODE,bufQuery,&dwLengthBufQuery) ;
            if (bQuery) {
               TRACE("HttpQueryInfo Status Code  is %s.\r\n", bufQuery) ;
               status = (DWORD)atol(bufQuery) ;
  			   
  			   HTTPStatus = status;  

			   if ((status >=  HTTP_STATUS_MOVED) && (status <= HTTP_STATUS_REDIRECT_METHOD)) {
					// redirected 
					moved = 1;
			   }
			   else if (status != HTTP_STATUS_OK) {
			      		dwLengthBufQuery = sizeof (bufQuery);
						bufQuery[0]=0;
            			bQuery = QueryInfo(hInternetFile,HTTP_QUERY_STATUS_TEXT,bufQuery,&dwLengthBufQuery) ;

						size = ERROR_BAD_URL;
					    if (Flag(WWW_REPORT_ERRORS)) {
						if (lastReportedInternetError != ERROR_BAD_URL) {
							lastReportedInternetError = ERROR_BAD_URL;
							Reporter.Error("HTTP Server reports\n %s (%d)\nfor http://%s%s",bufQuery,status,(const char *)m_strServer,m_strPath);
						
						}		
						}
			   }
            }
            else
            {
			   status = 0;
  			   HTTPStatus = status;  
            }

            dwLengthBufQuery = sizeof (bufQuery);
            bQuery = QueryInfo(hInternetFile,HTTP_QUERY_CONTENT_TYPE,bufQuery,&dwLengthBufQuery) ;
            
            if (bQuery)
            {
               TRACE("HttpQueryInfo ContentType %s.\r\n", bufQuery) ;
			   contentType = bufQuery;
#if 0
               //not ready
			   //multipart/x-mixed-replace; boundary=Th1sRand0mStr1ng.
				if (strprefix(contentType,MIME_MULTI_REP)) {
					  const char *p = strstr(contentType,"boundary=");
					  if (p) {
					     partSeparator = CString(p);  // add CR LF ???
						 isMultipart=1;
						 multiPartBegin = 1;
					  }
				}
#endif


            }
            else
            {
			   DWORD dwError = GetLastError() ;
			   //if (dwError != ERROR_HTTP_HEADER_NOT_FOUND)    ReportError(dwError,"HttpQueryInfo");
               TRACE("\tQueryType failed. \r\n") ;
			   contentType = "";
			   isMultipart = 0;
            }

            dwLengthBufQuery = sizeof (bufQuery);
            bQuery = QueryInfo(hInternetFile,HTTP_QUERY_CONTENT_TRANSFER_ENCODING,bufQuery,&dwLengthBufQuery) ;
            if (bQuery)
            {
				contentEncoding = bufQuery;
            }
            else
            {
			   DWORD dwError = GetLastError() ;
			   // if (dwError != ERROR_HTTP_HEADER_NOT_FOUND)  ReportError(dwError,"HttpQueryInfo");
            }

			SYSTEMTIME	time;
            dwLengthBufQuery = sizeof (time);
            bQuery = QueryInfo(hInternetFile,HTTP_QUERY_LAST_MODIFIED | HTTP_QUERY_FLAG_SYSTEMTIME ,&time,&dwLengthBufQuery) ;
            if (bQuery)
            {
				lastModified = time;
			}
            dwLengthBufQuery = sizeof (time);
            bQuery = QueryInfo(hInternetFile,HTTP_QUERY_EXPIRES | HTTP_QUERY_FLAG_SYSTEMTIME ,&time,&dwLengthBufQuery) ;
            if (bQuery)
            {
				expires = time;
			}



#if 1		
			{
            char bufQuery1[2048];
            dwLengthBufQuery = sizeof (bufQuery1);
			//strcpy(bufQuery1,field);
			bufQuery1[0]=0;

            bQuery = QueryInfo(hInternetFile,HTTP_QUERY_RAW_HEADERS,bufQuery1,&dwLengthBufQuery) ;
            if (bQuery)
            {
			  TRACE0(bufQuery1);
			  TRACE("\n");
            }
            else
            {
			   DWORD dwError = GetLastError() ;
			   //if (dwError != ERROR_HTTP_HEADER_NOT_FOUND) ReportError(dwError,"HttpQueryInfo");
               TRACE("\tQueryRawHeaders failed. \r\n") ;
            }
            }  

#endif
            DWORD dwBytesRead;
            if (size <0) {
			  // error 
			} else 
			if (!moved && (contentType.GetLength() >0) && !IsContentOk(contentType))  {
			    size =  ERROR_UNKNOWN_MIME_TYPE;
			    if (Flag(WWW_REPORT_ERRORS)) {
					if (lastReportedInternetError != ERROR_UNKNOWN_MIME_TYPE) {
						lastReportedInternetError = ERROR_UNKNOWN_MIME_TYPE;
						if (reporter) reporter->Error("Unknown Mime-type '%s'\nfor http://%s%s",(const char *)contentType,(const char *)m_strServer,m_strPath);
					}		
				}
			} else {
			while(1) {
            	BOOL bRead = ::InternetReadFile(hInternetFile,buf,BUF_SIZE,&dwBytesRead);	
            	
            	if (bRead && dwBytesRead == 0) {
				    //EOF 
					break;
				}
				
				if (WWWabort || stop ) break;


            	if (ReportInternetError((HINTERNET)bRead, "InternetReadFile"))
            	{
//              	TRACE("\tBytes Read is %d\r\n", dwBytesRead) ;
					int newSize = size + dwBytesRead;
					results = (char *) realloc( results, newSize+1 );
					assert( results );
					if (results == NULL) break;

	
					memcpy(results + size, buf, dwBytesRead);
					
					size += dwBytesRead;
	
					results[size]=0;  // for html data as string

					if (isMultipart) {
					   char * separator = strstr(results,partSeparator);
					   if (separator) {
					   		if (multiPartBegin) {
#if 0
--Th1sRand0mStr1ng
Content-type: x-world/x-vrml

#VRML V1.0 ascii

#endif

							   int partHeaderLen = separator - results;
							   partHeader = CString(results, separator-results);

							   multiPartBegin = 0;
							   // remove header from data
							   memcpy(results,separator,size-partHeaderLen);
							   size-= partHeaderLen;
		   					   results[size]=0;  
							   // check mime type of header



							} else {
							   multiPartBegin = 1;
   							   size =  separator - results;
							   // to do save rest	  
							   // part finished 
							   break;

							}
					   }
					}  // multipart

					if (dwFileSize >0) 
					if (reporter) reporter->Status("Receiving  %d%% 'http://%s%s' (%dk of %dk) ...", (int) (100.0 *size/dwFileSize),(const char *)m_strServer,m_strPath,size / 1024, dwFileSize / 1024);
					else if (reporter) reporter->Status("Receiving 'http://%s%s' (%dk of ?) ...",(const char *)m_strServer,m_strPath,size / 1024);


               		uiResult = THREAD_GOOD;
            	} // InternetReadFile
				else {
                    // error
                    stop = 1;
                    break; // 13.04.96
				}
			} // while reading
			
            if (WWWabort || stop) {
				if (reporter) reporter->Status("Transfer aborted 'http://%s%s' after %dk of %dk",(const char *)m_strServer,m_strPath,size / 1024,dwFileSize / 1024);
				if (results) free(results);
				results = NULL;
				size = -1;
				size = ERROR_TRANSMISSION_ABORTED;
			} else 
	  		    if (reporter) reporter->Status("Received 'http://%s%s' (%dk)",(const char *)m_strServer,m_strPath,size / 1024);
			
			} 


         } // HttpSendRequest

         if (hInternetFile) ::InternetCloseHandle(hInternetFile); 
		 hInternetFile = NULL;

      } // HttpOpenRequest

      if (hInternetConnection) ::InternetCloseHandle(hInternetConnection);
	  hInternetConnection = NULL;
   } // InternetConnect 

   *data = results;
   return(size);

}

long GFile::LoadUrlFTP(const char *m_strServer, int port, const char *m_strPath,char **data)
{
    long uiResult = THREAD_BAD;
	
	*data = NULL;
	char buf[BUF_SIZE];
	char* results = NULL;
	int size=0,bytes=0;
	*data = NULL;


    if (hSession == NULL)
    {
      //
      // Initialize the Internet Functions.
      //
      TRACE("Starting Session (Access = %i) (Proxy = %s)\r\n",
            GetAccessTypeIndex(), (LPCTSTR)proxyServer) ;
      
      hSession = ::InternetOpen(userAgent, accessType,proxyServer,proxyByPass/*,proxyPort*/,internetOpenFlags) ;

      if (!ReportInternetError(hSession, "InternetOpen"))
      {
         return THREAD_BAD;
      }
   }

   if (reporter) reporter->Status("Connecting to %s for `%s'...",(const char *)m_strServer,m_strPath);
   hInternetConnection = ::InternetConnect(hSession,m_strServer,port,
                                          ftpUserName,  ftpPassword,
                                          INTERNET_SERVICE_FTP,
                                          0,
                                          0) ;


                                          
   if (!WWWabort && !stop && ReportInternetError(hInternetConnection, "InternetConnect"))
   {

	  if (reporter) reporter->Status("Connected to %s for `%s'...",(const char *)m_strServer,m_strPath);

	  hInternetFile = FtpOpenFile( hInternetConnection, m_strPath, GENERIC_READ, FTP_TRANSFER_TYPE_BINARY,  0 /*dwContext*/	);

      if (ReportInternetError(hInternetFile, "FtpOpenFile"))
      {
         
            // Get size of file.
            char bufQuery[128] ;
            DWORD dwFileSize=0 ;
            DWORD dwLengthBufQuery = sizeof (bufQuery);

			/* get data */
			size = 0;
			results = NULL;


            DWORD dwBytesRead;
            

			while(1) {
            	BOOL bRead = ::InternetReadFile(hInternetFile,buf,BUF_SIZE,&dwBytesRead);	
            	if (bRead && dwBytesRead == 0) {
				    //EOF 
					break;
				}
				
				if (WWWabort || stop ) break;


            	if (ReportInternetError((HINTERNET)bRead, "InternetReadFile"))
            	{
//               		TRACE("\tBytes Read is %d\r\n", dwBytesRead) ;
					int newSize = size + dwBytesRead;
					results = (char *) realloc( results, newSize+1 );
					assert( results );
					if (results == NULL) break;

	
					memcpy(results + size, buf, dwBytesRead);
					
					size += dwBytesRead;
	
					results[size]=0;  // for html data as string

					Reporter.Status("Receiving 'http://%s%s' (%dk) ...",(const char *)m_strServer,m_strPath,size / 1024);


                    uiResult = THREAD_GOOD;
            	} // InternetReadFile
				else {
                    stop = 1;
                    break; // 18.04.96

				}
			}
			//Reporter.Error("Can get document %s on %s (%d)",file,(const char *)machine,lastError);
			if (WWWabort ||stop) {
				Reporter.Status("Transfer aborted 'http://%s%s' after %dk of %dk",(const char *)m_strServer,m_strPath,size / 1024,dwFileSize / 1024);
				if (results) free(results);
				results = NULL;
				size = -1;
				size = ERROR_TRANSMISSION_ABORTED;
			} else 
	  			if (reporter) reporter->Status("Received 'ftp://%s%s' (%dk)",(const char *)m_strServer,m_strPath,size / 1024);

         VERIFY(::InternetCloseHandle(hInternetFile)); 
		 hInternetFile = NULL;

      } // HttpOpenRequest

      VERIFY(::InternetCloseHandle(hInternetConnection)) ;
	  hInternetConnection = NULL;
   } // InternetConnect 

   *data = results;
   return(size);
}



long GFile::LoadUrlOther(const char *url,char **data)
{
	return(ERROR_UNKNOWN_PROTOCOL);
}

#endif

	
// check if we can handle the protocol (without : )
int GFile::IsProtocolOk(const char *protocol)
{
  if (streq(protocol,"http")) return(1);
  if (streq(protocol,"https")) return(1);
  if (streq(protocol,"ftp")) return(1);
  return(0);
}


// parset mime type and convert to internal format code
int GFile::ParseMimeType(const char *content)
{
   if (strprefix(content,"text/")) return(HTML_FMT);

   if (streq(content,MIME_VRML)) return(VRML_FMT);
   if (streq(content,MIME_VRML1)) return(VRML_FMT);

   if (streq(content,MIME_GLVIEW)) return(VRML_FMT); // ??? 
   if (streq(content,"application/x-blaxxunCC3D")) return(VRML_FMT);
   
   if (streq(content,MIME_BXM)) return(BXM_FMT); // ??? 
   
   if (streq(content,MIME_GIF)) return(GIF_FMT);
   if (streq(content,MIME_JPEG)) return(JPEG_FMT);

   if (streq(content,"image/pjpeg")) return(JPEG_FMT); // progressive JPEG 

   if (streq(content,MIME_RGB)) return(RGB_FMT);
   if (streq(content,MIME_BMP)) return(DIB_FMT);
   
   if (streq(content,"image/x-bitmap")) return(DIB_FMT); // xbm ???
   if (streq(content,"image/bmp")) return(DIB_FMT);

   if (streq(content,"image/x-portable-pixmap")) return(PPM_FMT);

   if (streq(content,"image/x-png")) return(PNG_FMT);
   if (streq(content,"image/png")) return(PNG_FMT);

   if (streq(content,MIME_AVI)) return(AVI_FMT);
   if (streq(content,MIME_MPEG)) return(MPG_FMT);

   if (streq(content,MIME_WAV)) return(WAV_FMT);
   if (streq(content,MIME_WAV_1)) return(WAV_FMT);
   
   if (streq(content,MIME_AU)) return(AU_FMT);

   if (streq(content,MIME_MIDI)) return(MIDI_FMT); // ?? 
   if (streq(content,"audio/x-midi")) return(MIDI_FMT); // ??

   if (streq(content,MIME_JAVASCRIPT)) return(JS_FMT);
   if (streq(content,"application/x-vrmlscript")) return(JS_FMT);
   if (streq(content,"application/x-ecmascript")) return(JS_FMT);



   if (streq(content,"application/x-gzip")) return(GZIP_FMT);
   if (streq(content,"application/x-gzip-compressed")) return(GZIP_FMT);

   // query mime database 


   if (streq(content,"")) {
       // guess from extension

   }
   return(UNKNOWN_FMT);

}

// check contentType for detecting filetype 
int GFile::IsContentOk(const char *content)
{
  
  int type;
  if (streq(content,"*/*")) { // a bad server 	4.10.96 
	  type = localFileFmt;
  } else {			

    // get file type from content 
	type = ParseMimeType(content);

	if (type == UNKNOWN_FMT || type == GZIP_FMT || type >= HTML_FMT) {
			if (localFileFmt != UNKNOWN_FMT) type = localFileFmt; 
	}
  }		
  return(type > UNKNOWN_FMT && (type !=HTML_FMT));
}


#ifdef G_WININET

/*
  load the data for the internet url
*/

int GFile::InternetOpenUrl(const char * url,char **data) 
{
	int ret=-1;
	CString machine;
	int nPort;
	const char* s;
	const char* file;
	size_t size;
	int machineLen; /* length of machine name */

	if ( !url ) return -1;


	s=strchr(url,':');
	if (!s) s = url;
	
	protocol = CString(url, s-url);
	
	{
		/* get the machine name */
		s=strchr(url,':');
		if (s) s +=3; else s = url;
		const char *sstart = s;
		machineLen=0;
		
		for (; *s && (*s != ':') && (*s != '/'); s++ )
			machineLen = s - sstart+1;; 

		machine = CString(sstart, machineLen );

		/* get port number */
		if ( *s == ':' ) {
			nPort = atoi( ++s );
			while( *s && (*s != '/') )
			s++;
		} else 
	  		nPort = 0;


		/* get the file name */
		if ( *s == '/' )
			file = s;
		else
			file = "/";

		if (protocol == "http") {
			if (nPort == 0) nPort = INTERNET_DEFAULT_HTTP_PORT ;  // 80
   			return LoadUrlHTTP(machine,nPort,file,data);
		} else 	if (protocol == "https") {
			if (nPort == 0) nPort = INTERNET_DEFAULT_HTTPS_PORT ;  // 443
   			return LoadUrlHTTP(machine,nPort,file,data);
		} else if (protocol == "ftp") {
			if (nPort == 0) nPort = INTERNET_DEFAULT_FTP_PORT ;  
   			return LoadUrlFTP(machine,nPort,file,data);
		} else 
   			return LoadUrlOther(url,data);
	}
}

#endif




#ifdef _COM 


STDMETHODIMP_(ULONG)    GFile::AddRef()
{
	ref();
	return refCnt;
}
STDMETHODIMP_(ULONG)    GFile::Release()
{
	unref();
	return refCnt;
}

STDMETHODIMP GFile::QueryInterface(REFIID riid, void** ppv)
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

STDMETHODIMP GFile::OnStartBinding(DWORD grfBSCOption, IBinding* pbinding)
{
	if (pbinding != NULL)
	{
		ASSERT(m_pbinding == NULL);

		pbinding->AddRef();
		m_pbinding = pbinding;

		// HRESULT m_pbinding->GetBindResult(
		// CLSID *pclsidProtocol,//Pointer to the CLSID that identifies the protocol usedDWORD *pdwBindResult,//Pointer to bind resultLPWSTR *pszBindResult,//Pointer to protocol-specific bind resultDWORD dwReserved //Reserved for future use; must be NULL);
	}

	return(NOERROR);
} 

//                                                                           
// WinINet and protocol specific errors are mapped to one of the following   
// error which are returned in IBSC::OnStopBinding                           
//                                                                           
// from urlmon.h


struct binding_error_ {
		HRESULT code;
		const char *message;
		} bindingErrors[] = 

{	  

{ INET_E_INVALID_URL		, "Invalid url" },
{ INET_E_NO_SESSION   	, "No Session" },
{ INET_E_CANNOT_CONNECT	, "Cannot connect" },
{ INET_E_RESOURCE_NOT_FOUND	, "Resource not found" },
{ INET_E_OBJECT_NOT_FOUND		, "Object not found" },
{ INET_E_DATA_NOT_AVAILABLE	, "Data not available" },
{ INET_E_DOWNLOAD_FAILURE		, "Download failure" },
{ INET_E_AUTHENTICATION_REQUIRED , "Authentication required" },
{ INET_E_NO_VALID_MEDIA, "No valid media" },
{ INET_E_CONNECTION_TIMEOUT, "Connection timeout" },
{ INET_E_INVALID_REQUEST , "Invalid request" },
{ INET_E_UNKNOWN_PROTOCOL, "Unknown protocol" },
{ INET_E_SECURITY_PROBLEM, "Security Problem" },
{ INET_E_CANNOT_LOAD_DATA, "Cannot load data" },
{ INET_E_CANNOT_INSTANTIATE_OBJECT, "Cannot instantiate object" },
{ 0,NULL }

};


const char *BindingErrorMessage(HRESULT code) 
{
	for(int i=0; bindingErrors[i].message != NULL;i++) 
    	if (bindingErrors[i].code == code) return bindingErrors[i].message;
    return("unknown");
}



STDMETHODIMP GFile::OnStopBinding(HRESULT hrStatus, LPCWSTR pszError) 
{
	USES_CONVERSION;


	if (m_pbinding != NULL)
	{
		m_pbinding->Release();
		m_pbinding = NULL;
	}

	if (pszError != NULL) {
		LPCTSTR message = W2CT(pszError);
		TRACE( "OnStopBinding:Download %s\n Error:%s (%x)",(const char *) url,message,hrStatus);
		if (Flag(WWW_REPORT_ERRORS) && reporter) reporter->Error( "Download %s\n Error:%s (%x)",(const char *) url,message,hrStatus);
	}
	else if (hrStatus != S_OK) {
		
		const char *msg= BindingErrorMessage(hrStatus);

		TRACE( "OnStopBinding:Download %s\n Error:%s  (%x)",(const char *) url,msg,hrStatus);
		if (Flag(WWW_REPORT_ERRORS) && reporter) reporter->Error( "Download %s\n Error:%s (%x)",(const char *) url,msg,hrStatus);

	}

	return(NOERROR);
}


STDMETHODIMP GFile::GetPriority(LONG* pnPriority) 
{
	 return(NOERROR);
}  

STDMETHODIMP GFile::OnLowResource(DWORD dwReserved)
{
 	 return(NOERROR);
}  

STDMETHODIMP GFile::GetBindInfo(DWORD* pgrfBINDF, BINDINFO* pbindInfo)
{

	// doesnt get called ???
	if (Flag(WWW_RELOAD)) *pgrfBINDF |= BINDF_GETNEWESTVERSION; 

	// doesnt get called ???
	// beginning with IE 5.01 it gets called !!!!!!!!!!!!!
	// and the code below caused a E_FAIL ???????????????????????????????????

	return (NOERROR);


	*pgrfBINDF |= BINDF_ASYNCHRONOUS;


/*
		*pgrfBINDF = BINDF_ASYNCHRONOUS | BINDF_ASYNCSTORAGE |
			BINDF_GETNEWESTVERSION | BINDF_NOWRITECACHE;
*/
		pbindInfo->cbSize = sizeof(BINDINFO);
		pbindInfo->szExtraInfo = NULL;
		memset(&pbindInfo->stgmedData, 0, sizeof(STGMEDIUM));
		pbindInfo->grfBindInfoF = 0;
		pbindInfo->dwBindVerb = BINDVERB_GET;
		pbindInfo->szCustomVerb = NULL;
		return S_OK;

	 return (NOERROR);

}

STDMETHODIMP GFile::OnDataAvailable(DWORD grfBSCF, 
		DWORD dwSize, FORMATETC* pfmtetc, STGMEDIUM* pstgmed) 
{  // CBindstatusCallback see ATLCTL.h
	return(NOERROR);
}


STDMETHODIMP GFile::OnObjectAvailable(REFIID riid, IUnknown* punk) 
{
	return(NOERROR);
}  

//HRESULT m_pBinding.Abort(void); 
// IWindowForBindingUI

STDMETHODIMP GFile::OnProgress(ULONG ulProgress,
		ULONG ulProgressMax, ULONG ulStatusCode, LPCWSTR pwzStatusText) 
{
	USES_CONVERSION;

	if (pwzStatusText == NULL) return NOERROR;

//	AFX_MANAGE_STATE(m_pModuleState);

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

	case BINDSTATUS_MIMETYPEAVAILABLE : 
						contentType = message;
						return NOERROR;						
						break;
	case BINDSTATUS_CACHEFILENAMEAVAILABLE :
						message1 = "FileName"; 
						TRACE("Cache File %s \n", message);
						return NOERROR;
						break;
	default: 
		TRACE("Unknown Bindstatus callback %d \n", message);
		return NOERROR;
	}

	if (ulStatusCode != BINDSTATUS_DOWNLOADINGDATA) {
		TRACE("ulProgress = %u\t ulProgressMax = %u ulStatusCode = %u pwzStatusText = %s\n", 
		ulProgress, ulProgressMax, ulStatusCode, message);
	}
	if (ulProgressMax==0) ulProgressMax=1;
	
	if (reporter) reporter->Status("%s%d%% %s", message1, (int) ((ulProgress *100) / ulProgressMax),message);


	// set the range for the progress control

	// I am dividing the progress values by two here because they are ULONG 
	// and the control takes ints.  This way we dont get negative numbers in the
	// case of progress values larger that INT_MAX

//	if( BINDSTATUS_BEGINDOWNLOADDATA == ulStatusCode )
//		m_pFormView->m_ctlProgress.SetRange( 0, ulProgressMax / 2 );

//	if( 0 != ulProgressMax )
//		m_pFormView->m_ctlProgress.SetPos(ulProgress /2 );
	if (refCnt <= 1 || stop) 
		return E_ABORT; // one ref hold by us, to do: may be 2 because of statusCallback.addRef()
 	else return(NOERROR);
}


#endif

// stolen from Mozilla
static int
mime_decode_base64_token (const char *in, char *out)
{
  /* reads 4, writes 0-3.  Returns bytes written.
	 (Writes less than 3 only at EOF.) */
  int j;
  int eq_count = 0;
  unsigned long num = 0;

  for (j = 0; j < 4; j++)
	{
	  unsigned char c = 0;
	  if (in[j] >= 'A' && in[j] <= 'Z')		 c = in[j] - 'A';
	  else if (in[j] >= 'a' && in[j] <= 'z') c = in[j] - ('a' - 26);
	  else if (in[j] >= '0' && in[j] <= '9') c = in[j] - ('0' - 52);
	  else if (in[j] == '+')				 c = 62;
	  else if (in[j] == '/')				 c = 63;
	  else if (in[j] == '=')				 c = 0, eq_count++;
	  else
		ASSERT(0);
	  num = (num << 6) | c;
	}

  *out++ = (char) (num >> 16);
  *out++ = (char) ((num >> 8) & 0xFF);
  *out++ = (char) (num & 0xFF);

  if (eq_count == 0)
	return 3;				/* No "=" padding means 4 bytes mapped to 3. */
  else if (eq_count == 1)
	return 2;				/* "xxx=" means 3 bytes mapped to 2. */
  else if (eq_count == 2)
	return 1;				/* "xx==" means 2 bytes mapped to 1. */
  else
	{						/* "x===" can't happen, because "x" would then */
	  ASSERT(0);			/* be encoding only 6 bits, not the min of 8. */
	  return 1;
	}
}


static int
mime_decode_base64_buffer (char *buffer, int length)
{
  /* Warning, we are overwriting the buffer which was passed in.
	 This is ok, because decoding these formats will never result
	 in larger data than the input, only smaller. */

  const char *in  = buffer;
  char *out = (char *) buffer;
  char token [4];
  int i;
  i = 0;


  while (length > 0)
	{
	  while (i < 4 && length > 0)
		{
		  if ((*in >= 'A' && *in <= 'Z') ||
			  (*in >= 'a' && *in <= 'z') ||
			  (*in >= '0' && *in <= '9') ||
			  *in == '+' || *in == '/' || *in == '=')
			  token [i++] = *in;
		  in++;
		  length--;
		}

	  if (i < 4)
	  {
		  /* Didn't get enough for a complete token. */
		  //XP_MEMCPY (data->token, token, i);
		  //data->token_size = i;
		  length = 0;
		  break;
	  }
	  i = 0;

	  int n = mime_decode_base64_token (token, out);

	  /* Advance "out" by the number of bytes just written to it. */
	  out += n;
  }

  return  (out - buffer);
}


#if 0
int
mime_encode_base64_buffer (MimeEncoderData *data,
						   const char *buffer, int32 size)
{
  int status = 0;
  const unsigned char *in = (unsigned char *) buffer;
  const unsigned char *end = in + size;
  char out_buffer[80];
  char *out = out_buffer;
  uint32 i = 0, n = 0;
  uint32 off;

  if (size == 0)
	return 0;
  else if (size < 0)
	{
	  XP_ASSERT(0);
	  return -1;
	}


  /* If this input buffer is too small, wait until next time. */
  if (size < (3 - data->in_buffer_count))
	{
	  XP_ASSERT(size < 3 && size > 0);
	  data->in_buffer[data->in_buffer_count++] = buffer[0];
	  if (size > 1)
		data->in_buffer[data->in_buffer_count++] = buffer[1];
	  XP_ASSERT(data->in_buffer_count < 3);
	  return 0;
	}


  /* If there are bytes that were put back last time, take them now.
   */
  i = 0;
  if (data->in_buffer_count > 0) n = data->in_buffer[0];
  if (data->in_buffer_count > 1) n = (n << 8) + data->in_buffer[1];
  i = data->in_buffer_count;
  data->in_buffer_count = 0;

  /* If this buffer is not a multiple of three, put one or two bytes back.
   */
  off = ((size + i) % 3);
  if (off)
	{
	  data->in_buffer[0] = buffer [size - off];
	  if (off > 1)
		data->in_buffer [1] = buffer [size - off + 1];
	  data->in_buffer_count = off;
	  size -= off;
	  XP_ASSERT (! ((size + i) % 3));
	  end = (unsigned char *) (buffer + size);
	}

  /* Populate the out_buffer with base64 data, one line at a time.
   */
  while (in < end)
	{
	  int32 j;

	  while (i < 3)
		{
		  n = (n << 8) | *in++;
		  i++;
		}
	  i = 0;

	  for (j = 18; j >= 0; j -= 6)
		{
		  unsigned int k = (n >> j) & 0x3F;
		  if (k < 26)       *out++ = k      + 'A';
		  else if (k < 52)  *out++ = k - 26 + 'a';
		  else if (k < 62)  *out++ = k - 52 + '0';
		  else if (k == 62) *out++ = '+';
		  else if (k == 63) *out++ = '/';
		  else abort ();
		}

	  data->current_column += 4;
	  if (data->current_column >= 72)
		{
		  /* Do a linebreak before column 76.  Flush out the line buffer. */
		  data->current_column = 0;
		  *out++ = '\015';
		  *out++ = '\012';
		  status = data->write_buffer (out_buffer, (out - out_buffer),
									   data->closure);
		  out = out_buffer;
		  if (status < 0) return status;
		}
	}

  /* Write out the unwritten portion of the last line buffer. */
  if (out > out_buffer)
	{
	  status = data->write_buffer (out_buffer, (out - out_buffer),
								   data->closure);
	  if (status < 0) return status;
	}

  return 0;
}


ProcessDataUrl() 
{

	CString encoding;
	int len+0;

	int l=strlen("data:"); 
	
	if (strnicmp(url,"data:",l) !=0) {
			TRACE("bad data: url \n");
			return S_BAD_URL;
	}
	url+=l;

	char *del = strchr(url,',');

	if (!del) {
			TRACE("bad data: url \n");
			return S_BAD_URL;
	}

	*del = 0;
	
	char *semi= strchr(url,';');
	if (semi) {
		contentType = CString(url, semi - url);
		encoding = CString(semi+1, del-(semi+1)); 
	}

	*del = ',';

	url = del+1;
	TRACE("data: url contentType %s encoding + %s \n",(const char *) contentType,(const char *) encoding);


	if (encoding == "base64")
	 len = mime_decode_base64_buffer(url,outBuffer);
	else 
	{
		TRACE("bad data: url \n");
		return S_BAD_URL;

	}

}

#endif
