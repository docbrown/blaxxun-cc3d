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

@module GURlCache.cpp - GLView URL Cache 	|

Copyright (c) 1998	blaxxun interactive, Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:

Changes:

$Revision: 1.28 $
$Log: gurlcache.cpp,v $
Revision 1.28  1999/07/06 16:55:08  tom
*** empty log message ***

Revision 1.15  1998/07/09 15:47:00  kristof
*** empty log message ***

Revision 1.14  1998/07/01 16:48:41  holger
empty message

Revision 1.1  1998/03/31 17:29:44  holger
empty message



Todo :
	WIN32+MFC dependent 



******************************************************************************/

#include "stdafx.h"
#include <assert.h>
#include <string.h>
#include <time.h>



#ifdef WIN32

#include <Process.h>

#else

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#endif 


#include "gversion.h"
#include "gtrace.h"
#include "greporter.h"

#include "gurlcache.h"


// should be a global some where 
static const char* prefix_http = "http://";


//
// GFileInfo
// stores some file information



// Identify valid Media Library mirror sites and initialize variable. Laurent - Jan.2000

static const char *mirror1 = "http://www.web3d.org/WorkingGroups/media/";
static const char *mirror2 = "http://www.livinglinks.com/UniversalMedia/";
static const char *mirror3 = "http://www.officetowers.com/UniversalMedia/";
static const char *mirror4 = "http://www.sc.ehu.es/ccwgamoa/UniversalMedia/";

// is the file to old 
gbool GFileInfo::toOld(long days) 
{
		
		CTime current = CTime::GetCurrentTime(); 
		// CTimeSpan duration( days, 0, 0, 0 );
		// current -= days * 24 * 60 * 60;

        current = current.GetTime() - days * 24 * 60 * 60;

		CTime old (creationTime);		

#ifdef _DEBUG_X
		CString s = current.Format( "%A, %B %d, %Y" );
		CString t = old.Format( "%A, %B %d, %Y" );


		TRACE(" current %s prev %s \n",(const char *)s,(const char *)t);
#endif



		return ( old < current);
}


	



//
// GUrlCache
// caches url in local directorys

// the global urlcache
GUrlCache theCache;

GUrlCache::GUrlCache() 
{ 
	cacheMode = GCACHE_ONCE; 
	
	sessionTime = CTime::GetCurrentTime();

#ifdef _DEBUG
	time_t t= sessionTime.GetTime();
	TRACE("GUrlCache:: sessionTime %s \n",ctime(&t));
#endif 

	readCacheEnabled = TRUE; 
	readMediaCacheEnabled = TRUE; // Laurent - Jan.2000
	writeCacheEnabled = TRUE; 
	writeMediaCacheEnabled = TRUE; // Laurent - Jan.2000
	writeCacheDirValid = FALSE;
	writeMediaCacheDirValid = FALSE; // Laurent - Jan.2000

	expirationDays = 7; // check after 1 week 
	minSpaceFree = 5*1024*1024; // 5 MB 
	spaceUsed = 0; // we don't know 

	maxSpaceUsed = 50*1024*1024; // 55 MB


	lastCacheFlushTime=0;
	
	needCacheCleaning = FALSE;
	lastDiskSpaceCheck = 0;
  
	stop = FALSE;
	cleanerBusy=FALSE;

}

GUrlCache::~GUrlCache()
{	// notify thread to stop 
	if (cleanerBusy) { TRACE("~GUrlCache() cleaner thread busy \n"); }
	stop = TRUE;
	Lock();
	Unlock();
}

// get a time for the current cache mode setting 
CTime GUrlCache::getRefreshTime()
{
	switch (cacheMode) {
	case GCACHE_ONCE : return sessionTime;
	case GCACHE_ALWAYS: return CTime::GetCurrentTime();
	case GCACHE_DAYS : 	{	
			CTime current = CTime::GetCurrentTime(); 

			//CTimeSpan duration( expirationDays, 0, 0, 0 );
			//current -=duration;

            current = current.GetTime() - expirationDays * 24 * 60 * 60;
 
			return current;
			}

	default :
		return (0);
	}
}


// set the write cache directory 
gbool GUrlCache::SetWriteCacheDirectory(const char *directory)
{
	writeCacheDir = directory;

	writeCacheDir.TrimLeft();
	writeCacheDir.TrimRight();

	if (writeCacheDir.IsEmpty()) {
		writeCacheEnabled = FALSE;
		writeCacheDirValid = FALSE;
		return FALSE;
	}


	if (strlen(directory) <=3) {
		writeCacheEnabled = FALSE;
		writeCacheDirValid = FALSE;
		return FALSE;
	}

#ifdef _WIN32

	CString sysDir;

	UINT s = ::GetSystemDirectory(sysDir.GetBuffer(MAX_PATH+1),MAX_PATH); 
	sysDir.ReleaseBuffer();
	if (s>0 && striprefix(writeCacheDir,sysDir)) {
		writeCacheEnabled = FALSE;
		writeCacheDirValid = FALSE;
		return FALSE;
	}

#endif

	if (!CreateDirectory(writeCacheDir)) {
		writeCacheEnabled = FALSE;
		writeCacheDirValid = FALSE;
		return FALSE;
	}

#ifdef _WIN32
	if (writeCacheDir[1]== ':')
		writeCacheDrive = writeCacheDir.Left(3);
	else 
		writeCacheDrive = writeCacheDir;

#endif
			
 	writeCacheDirValid = TRUE;


	return TRUE;

}



gbool GUrlCache::SetWriteMediaCacheDirectory(const char *mediaDirectory) // Laurent - Jan.2000
{
	writeMediaCacheDir = mediaDirectory;

	writeMediaCacheDir.TrimLeft();
	writeMediaCacheDir.TrimRight();

	if (writeMediaCacheDir.IsEmpty()) {
		writeMediaCacheEnabled = FALSE;
		writeMediaCacheDirValid = FALSE;
		return FALSE;
	}


	if (strlen(mediaDirectory) <=3) {
		writeMediaCacheEnabled = FALSE;
		writeMediaCacheDirValid = FALSE;
		return FALSE;
	}

	CString sysDir;

	UINT s = ::GetSystemDirectory(sysDir.GetBuffer(MAX_PATH+1),MAX_PATH); 
	sysDir.ReleaseBuffer();
	if (s>0 && strprefix(writeMediaCacheDir,sysDir)) {
		writeMediaCacheEnabled = FALSE;
		writeMediaCacheDirValid = FALSE;
		return FALSE;
	}




	if (!CreateDirectory(writeMediaCacheDir)) {
		writeMediaCacheEnabled = FALSE;
		writeMediaCacheDirValid = FALSE;
		return FALSE;
	}

	if (writeMediaCacheDir[1]== ':')
		writeMediaCacheDrive = writeMediaCacheDir.Left(3);
	else 
		writeMediaCacheDrive = writeMediaCacheDir;

			
 	writeMediaCacheDirValid = TRUE;


	return TRUE;

}

// amount of diskspace of drive of cache dir
LONGLONG GUrlCache::GetFreeDiskSpace()
{
#ifdef _WIN32
	LONGLONG result = 0;
    DWORD sectorsPerCluster,bytesPerSector, numberOfFreeClusters, totalNumberOfClusters;

	if (::GetDiskFreeSpace( writeCacheDrive, &sectorsPerCluster, &bytesPerSector, &numberOfFreeClusters, &totalNumberOfClusters)) {
		//LARGE_INTEGER		numberOfFreeClusters
		LONGLONG clusterSize = sectorsPerCluster * bytesPerSector;
		
		result = (LONGLONG)numberOfFreeClusters*clusterSize;
	}
	return result;

#else

    return( -1 );

#endif

}


gbool GUrlCache::AddCacheDirectory(const char *dir,CStringArray &readCacheDirs)
{
	CString directory;
	directory = dir;

	directory.TrimRight();
	directory.TrimLeft();

	if (directory.IsEmpty()) return FALSE;

	// 
	if (strprefix(directory,"cdrom") ||  strprefix(directory,"CDROM")) { // replace cd rom with found cdroms 
		int i=0;
		
		const char *x = directory;
		x+=strlen("cdrom");

		// get cdrom id 
		if (*x >= '0' &&  *x <'9') {
			i = (*x - '0');
			x++;
		}
		// skip the :
		if (*x == ':') x++;


		
		if (i<cdroms.GetSize()) {
			CString dir = cdroms.GetAt(i);
			// strip of prefix
			

			if (*x == '\\') x++;
			else if (*x == '/') x++;

			directory = dir  + CString(x);


		}
		else return FALSE; // no cdrom 
	}

	// filter out duplicates 
	for (int i=0; i<readCacheDirs.GetSize();i++) {
		if (readCacheDirs.GetAt(i) == directory)
			return FALSE;
	}
 

	if (ExistsDirectory(directory)) {
		readCacheDirs.Add(directory);
		return TRUE;
	}
	return FALSE;

}


// parse a set of urls 
gbool GUrlCache::SetReadCacheDirectoryPath(const char *dirs)
{

#ifdef _WIN32

	// UINT GetDriveType(directory)
	// GetDiskFreeSpace
	//GetLogicalDrives(VOID)

	// get cdroms 
	
	char drives [_MAX_PATH*4+1];


	DWORD size=GetLogicalDriveStrings(sizeof(drives),drives);

	const char *p=drives;
	const char *pend = p + min((int)size,(int)sizeof(drives));
	
	int ncdroms = 0;
	cdroms.RemoveAll();

	while(p<pend) {
		int l = strlen(p);
		UINT type = GetDriveType(p);

		if (type == DRIVE_CDROM) {
			ncdroms ++;
			cdroms.Add(p);
		}			
		p+=l+1;
	}

 


    // loop over the path 	
	
	p= dirs;
	const char *pstart= dirs;
	
	readCacheDirs.RemoveAll();

	while (*p != 0) {

		char c = *p++;
		
		if ( (c == ';')) { // found separator
			CString dir(pstart,p-pstart-1);

			AddCacheDirectory(dir,readCacheDirs);
			pstart = p;

		}
	}

	if (p>pstart) {
		CString dir(pstart,p-pstart);
		AddCacheDirectory(dir,readCacheDirs);
		pstart = p;
	}
	
	return readCacheDirs.GetSize()>0;

#else

    return( gfalse );

#endif

}

// parse a set of urls 
gbool GUrlCache::SetUmelDirectoryPath(const char *dirs)
{

	const char *p;


    // loop over the path 	

	p= dirs;
	const char *pstart= dirs;
	
	umelCacheDirs.RemoveAll();

	while (*p != 0) {

		char c = *p++;
		
		if ( (c == ';')) { // found separator
			CString dir(pstart,p-pstart-1);
			if (strprefix(pstart,prefix_http)) {
				umelHome = dir;
			} else
				AddCacheDirectory(dir,umelCacheDirs);
			pstart = p;

		}
	}

	if (p>pstart) {
		CString dir(pstart,p-pstart);
		if (strprefix(pstart,prefix_http)) {
				umelHome = dir;
		} else
			AddCacheDirectory(dir,umelCacheDirs);
		pstart = p;
	}
	
	return umelCacheDirs.GetSize()>0;
}




// combine a directory and a path and create all directory inbetween 
gbool GUrlCache::CombineCreateDirectory(const char *start, const char * relPath, CString &absFilePath) 
{

	CString directory;

	directory = start;
	
	const char *p= relPath;

	// create start directory 
	if (!CreateDirectory(directory)) return FALSE;

	if ( (*p == '/') || (*p == '\\')) { // found separator
		p++;
	}

	while (*p != 0) {

		char c = *p++;
		
		directory += c; // append char 

		if ( (c == '/') || (c == '\\')) { // found separator
			if (!CreateDirectory(directory)) {
				return FALSE;
			}
		}


	}
	
	absFilePath = directory;

	return TRUE;

}



// create/check the directory
gbool GUrlCache::CreateDirectory(CString &directory ) 
{
	  int l = directory.GetLength();
	  
	  if (l == 0) return FALSE;

	  if ( ! ((directory[l-1] == '\\') ||  (directory[l-1] == '/')) )
		  directory += '\\';
	  
	  {
#ifdef _WIN32
		  DWORD err;

		  SECURITY_ATTRIBUTES sa;
		  sa.nLength = sizeof(sa);
		  sa.lpSecurityDescriptor = NULL;
		  sa.bInheritHandle = FALSE;
		  gbool ret = ::CreateDirectory(directory,&sa);
		  if (!ret) {
			err = GetLastError();
			if (err == 	ERROR_ALREADY_EXISTS ) 
				ret = TRUE;
			else {
				TRACE("CreateDirectory failed with %d \n",err);

				if (err == ERROR_PATH_NOT_FOUND) {
				// something inbetween not ok
				}	
			}	
		    return ret;
		  }	
#else

          int ret = mkdir( directory, 755 );
          if (ret < 0 )
          {
              if( errno == EEXIST )
              {
                  ret = 0;
              }
              else
              {
                  TRACE("CreateDirectory failed with %s\n", strerror( errno ));

              }
          }
 
          return( ret == 0 );
#endif
	  }
	  return 0;

}

gbool GUrlCache::ExistsDirectory(CString &directory ) 
{
#ifdef _WIN32
	
	int l = directory.GetLength();
	  
	if (l == 0) return FALSE;

	if ( ! ((directory[l-1] == '\\') ||  (directory[l-1] == '/')) )
	  directory += '\\';

	BOOL ret = FALSE;
	DWORD err;

	CString tmp(directory);
	tmp +="info.txt";


	HANDLE f = CreateFile(tmp,GENERIC_READ,FILE_SHARE_READ,
    		NULL,	 // address of security descriptor,
    		OPEN_EXISTING, 
			FILE_FLAG_SEQUENTIAL_SCAN,NULL);	

	 if  (f == INVALID_HANDLE_VALUE) {
			ret = FALSE;
	 } else {
		   CloseHandle(f);
		   ret = TRUE;
	 }

	  if (!ret) {
			err = GetLastError();
			TRACE("ExistDirectory failed with %d \n",err);
			if (err == 	ERROR_ALREADY_EXISTS || err == ERROR_FILE_NOT_FOUND) 
				ret = TRUE;
			else if (err == ERROR_PATH_NOT_FOUND) {
				// something inbetween not ok
				ret = FALSE;

			}				
	  }	
	  return ret;

#else

      struct stat buf;

      int rc = stat( directory, &buf );
      if( !rc )
      {
          if( S_ISDIR( buf.st_mode ))
          {
              return( gtrue );
          }
      }
      return( gfalse );

#endif

}


// transform url to a relative file path
gbool GUrlCache::UrlToFilePath(const char *url,CString &relFilePath)
{

	char *FileName = relFilePath.GetBuffer(strlen(url));
	BOOL afterQuestion = FALSE;


  		if (strprefix(url,prefix_http))
  			strcpy(FileName,&url[strlen(prefix_http)]);
  		else strcpy(FileName,url);
  		char *ext;
  		// extra arguments to CGI ???
		ext =strrchr(FileName,'?'); 
		if (ext) { 
			// *ext = 0; 
			// return FALSE; // no cgi-bin
		}
  		
		// strip of argument 
		ext =strrchr(FileName,'#'); if (ext) *ext = 0;

		//test16.wrl_;>_@=& 
		
		ext = FileName;
		while (*ext) {
		  switch(*ext) {
		  case '/':
				 if (afterQuestion) *ext ='_'; 
				 // else keep separator case '/':
#ifdef WIN32
				 *ext = '\\'; // convert directory separators
#endif
				 break;
		  case '?':	 
			afterQuestion = TRUE;
		  case '\\':
		  case ':':	 
		  case ';':	 
		  case ',':	 
		  case '>':	 
		  case '<':	 
		  case '|':	
		  case '~':	
		  case '*':	
				 *ext ='_'; 
				 break;
		  default :
		      break;
		  }
		  ext ++;

		}

	relFilePath.ReleaseBuffer();		

	TRACE("Local filename %s \n",(const char *)relFilePath);
	return TRUE;
}


// is the file expired 
gbool GUrlCache::Expired(const char * absFilePath,GFileInfo &info)
{
	return info.toOld(expirationDays);
}



// exists the file ?
gbool GUrlCache::Exists(const char *fileName,GFileInfo *info)
{ gbool ret = FALSE;

#ifdef WIN32
	HANDLE f = CreateFile(fileName,GENERIC_READ,FILE_SHARE_READ,
    		NULL,	 // address of security descriptor,
    		OPEN_EXISTING, 
			FILE_FLAG_SEQUENTIAL_SCAN,NULL);	

	 if  (f == INVALID_HANDLE_VALUE) {
		 	int err = GetLastError();
			ret = FALSE;
	 } else {
		 
		 if (info != NULL) {
			FILETIME creationTime,lastAccessTime,lastWriteTime;
		// VOID GetSystemTimeAsFileTime( LPFILETIME lpSystemTimeAsFileTime 
 


		 //DWORD GetTimeZoneInformation( LPTIME_ZONE_INFORMATION lpTimeZoneInformation);
 

			if (GetFileTime(f, &creationTime,&lastAccessTime,&lastWriteTime)) {
				 info->setCreationTime(creationTime);
				 info->setLastAccessTime(lastAccessTime);
				 info->setLastWriteTime(lastWriteTime);
			}
			info->sizeLow =GetFileSize(f,&info->sizeHigh);
		 }	
 

		 CloseHandle(f);
  	     ret = TRUE;
	 }
#else
	 FILE *f = fopen(fileName,"r");
	 if (f) {
		 ret = TRUE;
		 fclose(f);
	 } else 
		 ret =FALSE;
#endif

  return(ret);
}


// combine directory + relFilePath to fullpathname
gbool GUrlCache::Combine(const CString &directory,const CString relFilePath,CString &absFilePath)
{
	if (directory.GetLength() == 0) return FALSE;
	absFilePath = directory+relFilePath;
	return TRUE;
}

#ifdef _WIN32

// convert a time to a file Time (from AfxTimeToFileTime)
gbool TimeToFileTime(const CTime& time, LPFILETIME pFileTime)
{
	SYSTEMTIME sysTime;
	sysTime.wYear = (WORD)time.GetYear();
	sysTime.wMonth = (WORD)time.GetMonth();
	sysTime.wDay = (WORD)time.GetDay();
	sysTime.wHour = (WORD)time.GetHour();
	sysTime.wMinute = (WORD)time.GetMinute();
	sysTime.wSecond = (WORD)time.GetSecond();
	sysTime.wMilliseconds = 0;

	// convert system time to local file time
	FILETIME localTime;
	if (!SystemTimeToFileTime((LPSYSTEMTIME)&sysTime, &localTime))
		return FALSE;

	// convert local file time to UTC file time
	if (!LocalFileTimeToFileTime(&localTime, pFileTime))
		return FALSE;
	return TRUE;
}

#endif


// copy the file from src to dest
// if creationTime > 0 set the lastModified  & creationDate to time 

gbool GUrlCache::CopyFile(const CString &srcFilePath,const CString &destFilePath,gbool checkDir,time_t creationTime)
{
	DWORD err;
	CString dir;
	BOOL ret=FALSE;

	Lock();
	do {
	if (checkDir) {
		// get home path 
		const char *path=destFilePath;
		const char *s;

		s=strrchr(path,'\\'); // for file:  URL´s 
		const char *s1=strrchr(path,'/');
		if (s1>s ) s = s1;
 		if (s) 
  		  dir =CString(path,s-path+1);
		else { 
			dir = "";
			break;
		}
		if (!CreateDirectory(dir)) {
			err = GetLastError();
			TRACE("CreateDirectory failed with %d \n",err);
		}

	}

	if (::CopyFile(srcFilePath,destFilePath,FALSE))	{
		ret = TRUE;
		if (creationTime>0) { // set the creation & modifed date 

			// CopyFile seems to take date of IE/NS Cache File 

			CTime t(creationTime);
			FILETIME ft;
			FILETIME mft;
			if ( TimeToFileTime(t, &ft)) {

			HANDLE f = CreateFile(destFilePath,GENERIC_READ|GENERIC_WRITE ,FILE_SHARE_READ,
    			NULL,	 // address of security descriptor,
    			OPEN_EXISTING, 
				FILE_ATTRIBUTE_NORMAL,NULL);	

			if  (f == INVALID_HANDLE_VALUE) {
			 	err = GetLastError();
				ret = FALSE;
			} else {

				GetSystemTimeAsFileTime(&mft); // set to current time 
 
				if (!::SetFileTime(f,/* CreationTime */ &ft,&mft,&mft)) { 
				 	err = GetLastError();
					TRACE("SetFileTime  failed with %d \n",err);
					ASSERT(0);
					ret = FALSE;
				}
			    CloseHandle(f);

			}
			}
		}

	}
	else {
		ret = FALSE;
	
	}
	} while (0);

	Unlock();	

	if (!ret) {
		err = GetLastError();
		LPVOID lpMsgBuf=NULL;

		::FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL 
		);


 

		TRACE("Copy file %s %s failed with %d (%s) \n",(const char *) srcFilePath,(const char *) destFilePath,err,(const char *) lpMsgBuf);
		// Free the buffer.
		if (lpMsgBuf) LocalFree( lpMsgBuf );

		// ERROR_FILE_NOT_FOUND 

		//Local filename www.colonycity.com/cgi-bin/games/vrmltime.pl 
		//Copy file C:\WINDOWS\Temporary Internet Files\Content.IE5\MLPJ2DHX\vrmltime[1].wrl G:\cc3d\0cache\www.colonycity.com/cgi-bin/games/vrmltime.pl failed with 2 

		ASSERT(0);
	}
	return ret;
 
}; 

// save downloaded url to cache
gbool GUrlCache::SaveUrl(const char *url,const CString &srcFilePath,time_t creationTime,CString &cacheFileName)
{
	CString relFilePath,absFilePath;
	GFileInfo info;
	CString selectedCacheDirectory;

	selectedCacheDirectory = writeCacheDir;
	int urlStrip= 0;
	
	if (writeMediaCacheEnabled && writeMediaCacheDirValid) {

		// Check if file to cache is from recognized Media Library mirror site.
		// If so, cache it in Media Library directory instead of regular cache directory


		if (strprefix(url,mirror1)) {
			selectedCacheDirectory = writeMediaCacheDir;
			urlStrip=strlen(mirror1);
		} else
		if (strprefix(url,mirror2)) {
			selectedCacheDirectory = writeMediaCacheDir;
			urlStrip=strlen(mirror2);
		} else
		if (strprefix(url,mirror3)) {
			selectedCacheDirectory = writeMediaCacheDir;
			urlStrip=strlen(mirror3);
		} else
		if (strprefix(url,mirror4)) {
			selectedCacheDirectory = writeMediaCacheDir;
			urlStrip=strlen(mirror4);
		} 


	}

	if ((urlStrip == 0) && (!writeCacheEnabled) )
		return FALSE;

	if (!UrlToFilePath(url+urlStrip,relFilePath)) {
		return FALSE;
	}

	if  (!CombineCreateDirectory(selectedCacheDirectory,relFilePath,absFilePath)){
		return FALSE;
	}				// end Laurent - Jan.2000


	cacheFileName = absFilePath;

	if (!CopyFile(srcFilePath,absFilePath,FALSE,creationTime)) {
		DWORD lastErr = GetLastError();

		if ( (lastErr ==ERROR_NOT_ENOUGH_MEMORY || lastErr == ERROR_OUTOFMEMORY )
			&& (!cleanerBusy) ) {
				TRACE("GurlCache::Was out of disk space  try cleaning \n");
				CheckCache();	// try to make space 

				if (!CopyFile(srcFilePath,absFilePath,FALSE,creationTime))  // retry 
					return FALSE;
		} else 
			return FALSE;
	}

	if (creationTime>0)			// store the URL lastModified date for once per session comparison
		SetUrlModified(url,creationTime);

	if (Exists(absFilePath,&info)) {
		bytesWrittenToCache += info.sizeLow;
		return TRUE;
	}
	else 
		return FALSE;
}

// check cache for already  downloaded url 
// result TRUE : srcFilePath contains local file, info file info
// if checkTime, cache creation time must be >= checkTime 
//
gbool GUrlCache::GetUrl(const char *url,time_t checkTime, CString &srcFilePath,GFileInfo &info)
{
	CString relFilePath,absFilePath;
	
	BOOL expiredCheck = TRUE;

	if ((!readCacheEnabled) && (!readMediaCacheEnabled)) // Laurent - Jan.2000
		return FALSE;

	if (!UrlToFilePath(url,relFilePath)) {
		return FALSE;
	}

	if (!Combine(writeCacheDir,relFilePath,absFilePath)) {
		return FALSE;
	}

	gbool exists= Exists(absFilePath,&info);

	// hints to not cache 
	if (strstr(url,"/no_cache/")) {
		return FALSE;
	}
	if (strstr(url,"/cgi-bin/")) {
		return FALSE;
	}


	if (!exists) {

		// check readonly cache 
	
		for (int i=0; i<readCacheDirs.GetSize();i++) {
			CString dir = readCacheDirs.GetAt(i);

	   		if (Combine(dir,relFilePath,absFilePath) && (Exists(absFilePath,&info))) {
				srcFilePath = absFilePath;

				if (checkTime >0 && info.getLastModifiedTime() <checkTime) // refresg
					return FALSE;

				// no expired check if in RO-Cache !

				return TRUE;
			}

		}

		return FALSE;
	} 

	bytesReadFromCache += info.sizeLow;

#ifdef _DEBUG
	time_t t= checkTime;
	TRACE("URL %s cache checkTime  %s \n",url,ctime(&t));
	t = info.creationTime;
	TRACE("cache creationTime  %s \n",ctime(&t));
	t = info.lastWriteTime;
	TRACE("cache lastWriteTime  %s \n",ctime(&t));
#endif


	if (checkTime >0 && /*info.creationTime*/ info.getLastModifiedTime()  <checkTime)
		return FALSE;

/*  // day check alreay in checktime
	if (expiredCheck && Expired(absFilePath,info)) {
		return FALSE;
	}
*/

	srcFilePath = absFilePath;
	return TRUE;
}

gbool GUrlCache::SetUrlModified(const char *url,time_t lastModifiedTime)
{
	GFSortEntry *e = NULL;

	if ( (e=modifiedList.Find(url)) != NULL) {
		if (lastModifiedTime>e->t) e->t = lastModifiedTime;
		return TRUE;
	}

	e = new GFSortEntry(url,lastModifiedTime,0);
	if(!e) return FALSE;
	modifiedList.Add(e);
	return TRUE;
}

gbool GUrlCache::GetUrlModified(const char *url,time_t  &lastModifiedTime)
{
	GFSortEntry *e = NULL;

	if ( (e=modifiedList.Find(url)) != NULL) {
		lastModifiedTime=e->t;
		return TRUE;
	}
	return FALSE;
}


GFileSorter::~GFileSorter()
{
	Flush();
}


gbool GFileSorter::Add(GFSortEntry *e) 
{	if (!e) return FALSE; 
	entries.Append(e); 
	mustSort = TRUE;
	return TRUE; 
}

void GFileSorter::Flush()
{
	for (int i= 0; i<entries.Length(); i++) {
		GFSortEntry *e = entries[i];
		entries[i]=NULL;
		delete e;

	}
	entries.SetLength(0);
	fileSum = 0;
	mustSort = FALSE;
}

// or stop if maxBytes bytes have been freed up
gbool GFileSorter::DeleteOldest(GUrlCache &cache,int maxCnt,LONGLONG maxBytes,BOOL &stop)
{
	LONGLONG curBytes = 0;
	int maxDirEntry=0;
	int i;

	if (maxCnt <=0) maxCnt = entries.Length();
	else 
	maxCnt = min (maxCnt,entries.Length());

	// pass 1 delete files until we have reached maxBytes free 
	for(i=0; i<maxCnt; i++) 
	{
		GFSortEntry *e = entries[i];

		if (stop) break;


		if (e->isDir) {
			maxDirEntry = i;
		}
		else 
		if (cache.RemoveFile(e->path)) {
			curBytes += e->length;

			if (maxBytes>0 && curBytes>=maxBytes)
				break;
		}
	}
	// pass two, delete directory entries 
	for(i=0; i<maxCnt; i++) 
	{
		GFSortEntry *e = entries[i];

		if (stop) break;


		if (e->isDir) {
			cache.RemoveDirectory(e->path); // try to remove dir 
		}
		else 
			if (i>maxDirEntry)  { break; } // delete following dir entrys 
	}
	
	// update sum 
	fileSum -=curBytes;

	return TRUE;

}


/*
 * CompareModes
 */
static int
G__cdecl CompareGFSortEntry(const void* element1, const void* element2) {
    GFSortEntry *e1, *e2;

    e1 = *((GFSortEntry**)element1);
    e2 = *((GFSortEntry**)element2);

	time_t l1 = e1->t;
	time_t l2 = e2->t;

	if (l1<l2) return -1;
	if (l1>l2) return 1;

	return 0;

}

static int
G__cdecl CompareGFSortEntryName(const void* element1, const void* element2) {
    GFSortEntry *e1, *e2;

    e1 = *((GFSortEntry**)element1);
    e2 = *((GFSortEntry**)element2);

	return e1->path.Compare(e2->path);

}

gbool GFileSorter::SortTime()
{
	qsort((void *)&entries[0], (size_t)entries.Length(), sizeof(GFSortEntry*),CompareGFSortEntry);
	mustSort = FALSE;
	return TRUE;
}

gbool GFileSorter::SortName()
{
	qsort((void *)&entries[0], (size_t)entries.Length(), sizeof(GFSortEntry*),CompareGFSortEntryName);
	mustSort = FALSE;
	return TRUE;
}

// find an entry 
GFSortEntry *GFileSorter::Find(const char *path)
{
	if (mustSort) 
		SortName();
	int l=0;
	int r=entries.Length()-1;

	// binary search  terminate if interval below 1
	while ((l+1) < r) {

		int i = (l+r) / 2;
		int res = entries[i]->path.Compare(path);

		if (res<0) {
			r = i;
			continue;
		}
		else if (res>0) {
			l = i;
			continue;
		}
		else {
			return entries[i];
		}
	}
	return NULL;

}

// add files to list 
gbool GFileSorter::AddFiles(const char *directory,time_t &maxFileTime,BOOL &stop)
{

    CFileFind finder;

	CString dir = directory;

    // add separator 
	int l = dir.GetLength();
	if (l == 0) return FALSE;
    if ( !((dir[l-1] == '\\') || (dir[l-1] == '/'))) dir += '\\';

	dir += "*.*";
	
	CString path;

	CTime creationTime((time_t)0);
	CTime accessTime((time_t)0);
	CTime writeTime((time_t)0);

    // setup the find structure 
	BOOL bWorking = finder.FindFile(dir);

    
	while (bWorking)  { // for all entrys 
		if (stop) break;

        bWorking = finder.FindNextFile();

        path = finder.GetFilePath();
		
		creationTime = (time_t)0;
		accessTime = (time_t)0;
		writeTime = (time_t)0;

		BOOL ret=finder.GetCreationTime(creationTime);
		finder.GetLastAccessTime(accessTime);
		finder.GetLastWriteTime(writeTime);

		time_t t = creationTime.GetTime();

		if (writeTime.GetTime()>0) t = max(t,writeTime.GetTime()); // HG wg Kristof
		
		if (accessTime.GetTime()>0) t = max(t,accessTime.GetTime());

		if (finder.IsDots( )) {	// ignore . ..

		} else 	if (finder.IsDirectory( )) { // recursively step down 
			
			t=0; // we want to delete empty directories new 20.10.98
			AddFiles(path,t,stop);

			DWORD length = 0;
			
			// to do get date of latest 
			TRACE("D %s c %ld a %ld w %ld size %ld \n",(const char *) path, creationTime.GetTime(),accessTime.GetTime(),writeTime.GetTime(),length);

			// time is the max of the child time +1
			GFSortEntry *e = new GFSortEntry(path,t+1,length,gtrue);

			if (!e) break;
			if (!Add(e)) break;
			
			if (t>maxFileTime) maxFileTime = t;


		}
		else { 
			
			DWORD length = finder.GetLength(); // get length 64
			fileSum += length;

			TRACE("F %s c %ld a %ld w %ld size %ld \n",(const char *) path, creationTime.GetTime(),accessTime.GetTime(),writeTime.GetTime(),length);
			GFSortEntry *e = new GFSortEntry(path,t,length);

			if (t>maxFileTime) maxFileTime = t;

			if (!e) break;
			if (!Add(e)) break;
		}


    }
	finder.Close();

	//TRACE("%ld bytes \n",(long)fileSum);

	return TRUE;
} 




// recursively remove a file branch 
gbool GUrlCache::RemoveFiles(const char *directory,time_t olderThan)
{

    CFileFind finder;

	CString dir = directory;

    // add separator 
	int l = dir.GetLength();
	if (l == 0) return FALSE;
    if ( !((dir[l-1] == '\\') || (dir[l-1] == '/'))) dir += '\\';

	dir += "*.*";
	
	CString path;

	CTime creationTime((time_t)0);
	CTime accessTime((time_t)0);
	CTime writeTime((time_t)0);

    // setup the find structure 
	BOOL bWorking = finder.FindFile(dir);

	LONGLONG fileSum=0;
    
	while (bWorking)  { // for all entrys 
		if (stop) break;

        bWorking = finder.FindNextFile();

        path = finder.GetFilePath();
		
		creationTime = (time_t)0;
		accessTime = (time_t)0;
		writeTime = (time_t)0;

		BOOL ret=finder.GetCreationTime(creationTime);
		finder.GetLastAccessTime(accessTime);
		finder.GetLastWriteTime(writeTime);
		time_t t = creationTime.GetTime();
		
		// if (accessTime.GetTime()>0) t = max(t,accessTime.GetTime());


		if (finder.IsDots( )) {	// ignore . ..

		} else 	if (finder.IsDirectory( )) { // recursively step down 
			RemoveFiles(path,olderThan);
			RemoveDirectory(path);
		}
		else { 
			
			DWORD length = finder.GetLength();

			TRACE("F %s c %ld a %ld w %ld size %ld \n",(const char *) path, creationTime.GetTime(),accessTime.GetTime(),writeTime.GetTime(),length);
			if (olderThan >0 ) {

				if (t < olderThan) {
					if (RemoveFile(path))
						fileSum += length;
				}


			} else { 
				if (RemoveFile(path))
					fileSum += length;
			}

		}


    }
	finder.Close();

	TRACE("%ld bytes deleted \n",(long) fileSum);

	return TRUE;
} 

// remove  directory
gbool GUrlCache::RemoveDirectory(const char *directory) 
{
	TRACE("Removing directory : %s \n",directory);
	return ::RemoveDirectory(directory); 
	// return TRUE;

}; 
 

// remove  directory
gbool GUrlCache::RemoveFile(const char *fileName)
{

	TRACE("Removing file : %s \n",fileName);
	return ::DeleteFile(fileName); 
	//return TRUE;
} 

gbool GUrlCache::GetCacheStats(const char *directory, time_t &oldest,DWORD &spaceInUse, int &filesInUse)
{

    CFileFind finder;

	CString dir = directory;

    // add separator 
	int l = dir.GetLength();
	if (l == 0) return FALSE;
    if ( !((dir[l-1] == '\\') || (dir[l-1] == '/'))) dir += '\\';

	dir += "*.*";
	
	CString path;

	CTime creationTime((time_t)0);
	CTime accessTime((time_t)0);
	CTime writeTime((time_t)0);

    // setup the find structure 
	BOOL bWorking = finder.FindFile(dir);

    
	while (bWorking)  { // for all entrys 
		if (stop) break;

        bWorking = finder.FindNextFile();

        path = finder.GetFilePath();
		
		creationTime = (time_t)0;
		accessTime = (time_t)0;
		writeTime = (time_t)0;

		BOOL ret=finder.GetCreationTime(creationTime);
		finder.GetLastAccessTime(accessTime);
		finder.GetLastWriteTime(writeTime);
		time_t t = creationTime.GetTime();
		
		if (accessTime.GetTime()>0) t = max(t,accessTime.GetTime());


		if (finder.IsDots( )) {	// ignore . ..

		} else 	if (finder.IsDirectory( )) { // recursively step down 
			GetCacheStats(path,oldest,spaceInUse,filesInUse);
		}
		else { 
			
			DWORD length = finder.GetLength();

			TRACE("F %s c %ld a %ld w %ld size %ld \n",(const char *) path, creationTime.GetTime(),accessTime.GetTime(),writeTime.GetTime(),length);
			oldest = min(oldest,t);
			filesInUse++;
			spaceInUse += length;

		}


    }
	finder.Close();


	return TRUE;
} 



// check the cache to clean up stuff
gbool GUrlCache::CheckCache() 
{
	gbool ret=FALSE;


	int maxCnt=0;
	LONGLONG maxBytes=0;

	GFileSorter sorter;

	TRACE("Check cache start \n");

	if (!writeCacheDirValid) return FALSE;
	cleanerBusy=TRUE;

	Lock();
	
	while (1) {
	time_t maxFileTime = 0;
	sorter.AddFiles(writeCacheDir,maxFileTime,stop);


	spaceUsed = sorter.fileSum;
	// how much byptes should be freed
	LONGLONG diskSpace = GetFreeDiskSpace();

	if (maxSpaceUsed >0 && spaceUsed >maxSpaceUsed)
		maxBytes = spaceUsed-maxSpaceUsed;

	if (minSpaceFree>0 && (diskSpace < minSpaceFree)) {

		maxBytes=max(maxBytes, (minSpaceFree-diskSpace));

	}
	TRACE("Cache cleaner Space used by cache files : %d k, Free Diskspace %d k, space to clean : %dk \n",(long)(spaceUsed / 1024),(long)(diskSpace/1024),(long)(maxBytes / 1024));
	if (maxBytes >0) { // delete a bit more 
		maxBytes += max(maxBytes/8, LONGLONG(maxSpaceUsed / 8) ) ; // 12.05 %
	}
	if (maxBytes <= (100*1024)) { // to few bytes to free exit 
		spaceUsed = sorter.fileSum;
		bytesWrittenToCache = 0;

		sorter.Flush();
		break;
	}


	if (stop) break;

	sorter.SortTime();

	if (stop) break;
	TRACE("Cache cleaner trying to deleting: : %dk \n",long(maxBytes / 1024));



	ret = sorter.DeleteOldest(*this,maxCnt,maxBytes,stop);

	if (stop) break;

	spaceUsed = sorter.fileSum;
	bytesWrittenToCache = 0; // clear for next time 

	TRACE("Cache cleaner Space used by cache files : %ld k \n",(long) (spaceUsed / 1024));

	sorter.Flush();

	break;
	}
	needCacheCleaning = FALSE;
	lastCacheFlushTime = ::time(NULL);
	
	Unlock();
	cleanerBusy=FALSE;
	TRACE("Check cache end \n");

	return ret;


}

gbool GUrlCache::NeedCacheCleaning(DWORD theTime)
{
	// too much space used ? 

	if ( (spaceUsed+bytesWrittenToCache) >maxSpaceUsed)  
		needCacheCleaning=TRUE;
	else if (spaceUsed <= 0) { // we don't know yet, written more then 5 % , was 2 % ?
		if ( (bytesWrittenToCache >= (maxSpaceUsed/20)) /* && (bytesWrittenToCache> (200*1024)) */ )
			needCacheCleaning = TRUE;
	}		

	if (needCacheCleaning) return TRUE;
	
	if (((long)theTime-(long)lastDiskSpaceCheck) > 1000l*120l) { // two minutes 
		// monitor free disk space 
		LONGLONG diskSpace = GetFreeDiskSpace();

		if (minSpaceFree>0 && (diskSpace < minSpaceFree)) {
			needCacheCleaning=TRUE;
		}
		lastDiskSpaceCheck = theTime;
		TRACE("Diskspace free = %ld KB \n", (long) (diskSpace / 1024));
	}
	return needCacheCleaning;

}


// AFX_THREADPROC
// cache cleaner thread function

UINT G__cdecl CUrlCacheCleanerThread (LPVOID pParam )
{
  GUrlCache *me = (GUrlCache *)  pParam;
  TRACE("Starting cleaner Thread \n");
  return me->CheckCache();
}



// UMEL support

// check UMEL directories from UMEL urn
// result TRUE : srcFilePath contains local file, info file info
//
/*
gbool GUrlCache::UmelUrnToFilePath(const char *url, CString &relFilePath)) 
{
	if (striprefix(url,
}
*/

gbool GUrlCache::GetUmelUrl(const char *url, CString &srcFilePath,GFileInfo &info, gbool &isFile)
{
	CString relFilePath,absFilePath;

	// get relative path

	//if (striprefix(url,G_URN_UMEL))

	relFilePath=url;
	srcFilePath = relFilePath;

	// check UMEL writeMediaCacheDir directory

	if (writeMediaCacheDirValid) {
   		if (Combine(writeMediaCacheDir,relFilePath,absFilePath) && (Exists(absFilePath,&info))) {
			srcFilePath = absFilePath;
			isFile = TRUE;
			return TRUE;
		}
	}

		// check readonly  UMEL directory
	
		for (int i=0; i<umelCacheDirs.GetSize();i++) {
			const CString &dir = umelCacheDirs.ElementAt(i);

	   		if (Combine(dir,relFilePath,absFilePath) && (Exists(absFilePath,&info))) {
				srcFilePath = absFilePath;
				isFile = TRUE;

				return TRUE;
			}

		}

		return FALSE;

}





