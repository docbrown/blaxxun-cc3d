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
#ifndef _GUrlCache_H
#define _GUrlCache_H
/******************************************************************************
@doc

@module GUrlCache.h - GLView WWW caching |

Copyright (c) 1998	by  blaxxun interactive - Holger Grahn
All rights reserved

Purpose:

Classes:
<c GUrlCache>

Notes:

Changes:

$Revision: 1.10 $
$Log: gurlcache.h,v $
Revision 1.10  1999/07/06 16:55:11  tom
*** empty log message ***

Revision 1.1  1998/03/31 17:29:44  holger
empty message



Todo :

******************************************************************************/


class GReporter;
class GUrlCache;




//
// GFileInfo
// stores some file information

class GFileInfo 
{
public :
	time_t creationTime,lastAccessTime,lastWriteTime;
	DWORD  sizeLow,sizeHigh;

	// constructor 
	GFileInfo() : creationTime(0),lastAccessTime(0),lastWriteTime(0),sizeLow(0),sizeHigh(0) {}

	void setCreationTime(const time_t t) { creationTime = t; }
	void setLastAccessTime(const time_t t) { lastAccessTime = t; }
	void setLastWriteTime(const time_t t) { lastWriteTime = t; }

#ifdef _MFC 
	void setCreationTime(const CTime t) { creationTime = t.GetTime(); }
	void setLastAccessTime(const CTime t) { lastAccessTime = t.GetTime(); }
	void setLastWriteTime(const CTime t) { lastWriteTime = t.GetTime(); }
#endif


	// get the last modified time 
	time_t getLastModifiedTime() const {

		if ( lastWriteTime > creationTime ) 
				return lastWriteTime;
		else return creationTime;
	}


	// is the file to old  ?
	gbool toOld(long days); 
	gbool olderThen(time_t checkTime); 

};

//
// Sort file entrys
// 



class GFSortEntry {
public :
	time_t t;
	CString path;

	DWORD length;
	gbool isDir;

	GFSortEntry(const CString &path_,time_t t_,DWORD l,gbool isDir_=FALSE) : path(path_),t(t_),length(l),isDir(isDir_) { }
	~GFSortEntry() {} 

};


class GFileSorter {
public :
		GFileSorter() :fileSum(0),mustSort(FALSE) {}
		~GFileSorter();

		Array<GFSortEntry *> entries;
		gbool mustSort;

		// flush the file entries list
		void Flush();

		// add a new file entry (memory is taken )
		gbool Add(GFSortEntry *e); 

		// find an entry 
		GFSortEntry *Find(const char *path);

		// sort the entry from oldes to newest 
		gbool SortTime();
		// sort by name 
		gbool SortName();

		// add all files in directory to entries 
		gbool AddFiles(const char *directory,time_t &maxFileTime,BOOL &stop);

		// sum in bytes of all files added so far
		LONGLONG fileSum;		
	
		// delete upTo maxCnt oldest entries, 
		// or stop if maxBytes bytes have been freed up
		gbool DeleteOldest(GUrlCache &cache,int maxCnt,LONGLONG maxBytes,BOOL &stop);
};

typedef enum {
	GCACHE_ALWAYS,
	GCACHE_ONCE,
	GCACHE_DAYS,
	CCACHE_NEVER
} GCacheMode;


//
// GUrlCache
// caches url in local directorys

class GUrlCache {

public:

	// exclusive access
	CCriticalSection locker;	// 

	BOOL Lock() { return locker.Lock(); }
	BOOL Unlock() { return locker.Unlock(); }

	gbool readCacheEnabled;		//!< reading from cahce enabled 
	gbool readMediaCacheEnabled;	// Laurent - Jan.2000
 
	gbool writeCacheEnabled;	//!< write to cache enabled

	gbool writeMediaCacheEnabled; // Laurent - Jan.2000

	CString writeCacheDir;		//!< directory path for r/w cache
	gbool writeCacheDirValid;	//!< is the directory ok
	
	CString writeMediaCacheDir;		// Laurent - Jan.2000
	gbool writeMediaCacheDirValid;	// Laurent - Jan.2000
	
	CStringArray readCacheDirs;	//!< list of expanded RO directories
	CStringArray cdroms;		//!< list of cdrom drivers

	CStringArray umelCacheDirs; //!< list of expanded UMEL RO directories
	CString		umelHome;		//!< UMEL url home 

	GFileSorter modifiedList;	// list of urls with modified days 

	
	GCacheMode cacheMode;		//!< the caching mode

	CTime sessionTime;			//!< time for once per session verification

	CTime getRefreshTime();

	long expirationDays;		//!< after how many days to refresh

	DWORD minSpaceFree;			//!< leave at minimum this amount of diskspace free
	LONGLONG spaceUsed;			//!< estimate of how much space is beeing used by cache files 
	DWORD maxSpaceUsed;			//!< how much bytes we should maximal used 

	// some stats
	DWORD bytesWrittenToCache;
	DWORD bytesReadFromCache;

	time_t lastCacheFlushTime;
	time_t checkInterval;
	
	CString writeCacheDrive;		// directory driver path for r/w cache for checking diskspace
	CString writeMediaCacheDrive;	// Laurent - Jan.2000

	BOOL cleanerBusy;				// CheckCache is currently running
	BOOL stop;
	
	gbool needCacheCleaning;			// flag if cache cleaning needed
	DWORD lastDiskSpaceCheck;		// time last diskspace check was called 	
	

	GUrlCache();
	~GUrlCache();

	//! set the write cache directory
	gbool SetWriteCacheDirectory(const char *dir);

	gbool SetWriteMediaCacheDirectory(const char *dir); // Laurent - Jan.2000

	//! add a single directory to list of directory
	gbool AddCacheDirectory(const char *dir,CStringArray &readCacheDirs);

	//! set list of directories,  a:\xx;c:\yyy;CDROM:
	gbool SetReadCacheDirectoryPath(const char *dirs);
	
	//! clear all directorys
	void ClearReadCacheDirectorys() { readCacheDirs.RemoveAll(); }

	//! set UMEL list of directories,  a:\xx;c:\yyy;CDROM:
	gbool SetUmelDirectoryPath(const char *dirs);
	
	//! clear all directory
	void ClearUmelDirectorys() { umelCacheDirs.RemoveAll(); }


	//! create/check the directory
	//! also adds trailing \ 
	gbool CreateDirectory(CString &directory); 
	
	//! existes the directory ?
	gbool ExistsDirectory(CString &directory); 

	//! amount of diskspace of drive of cache dir
	LONGLONG GetFreeDiskSpace();


	//! transform url to a relative file path
	gbool UrlToFilePath(const char *url,CString &relFilePath);
	
	//! combine directory + relFilePath to fullpathname
	gbool Combine(const CString &directory,const CString relFilePath,CString &absFilePath);

	//! combine a directory and a path and create all directory inbetween 
	gbool CombineCreateDirectory(const char *directoy, const char * relPath,CString &absFilePath); 


	//! exists the file ?
	gbool Exists(const char * absFilePath,GFileInfo *info);

	//! is the file expired 
	gbool Expired(const char * absFilePath,GFileInfo &info);

	// copy the file from src to dest
	// if creationTime > 0 set the lastModified  & creationDate to time 
	gbool CopyFile(const CString &srcFilePath,const CString &destFilePath,gbool checkDir,time_t creationTime);


	//! save downloaded url to cache, return cache file name , true if copy was sucessfull
	gbool SaveUrl(const char *url,const CString &srcFilePath,time_t creationTime,CString &cacheFileName);

	gbool SetUrlModified(const char *url,time_t lastModifiedTime);
	gbool GetUrlModified(const char *url,time_t  &lastModifiedTime);

	// check cache for already  downloaded url 
	// result TRUE : srcFilePath contains local file, info file info
	// if checkTime, cache creation time must be >= checkTime 
	//
	gbool GetUrl(const char *url,time_t checkTime, CString &srcFilePath,GFileInfo &info);

#if 0
	// check cache for already  downloaded url 
	gbool GetUrl(const char *url,CString &srcFilePath);

	// check cache for already  downloaded url 
	gbool GetUrl(const char *url, time_t checkTime, CString &srcFilePath);
#endif

	// recursively remove a file branch 
	gbool RemoveFiles(const char *directory, time_t olderThan);

	// remove empty directory
	gbool RemoveDirectory(const char *directory); 

	// remove file
	gbool RemoveFile(const char *fileName); 

	// remove oldest files 
	gbool RemoveOldestFile(const char *directory, time_t olderThan,DWORD cacheLimit);
	
	// query cache status 
	// variables must be 0 at entry
	gbool GetCacheStats(const char *directory, time_t &oldest,DWORD &spaceInUse, int &filesInUse);

	// check if cache needs cleaning
	gbool NeedCacheCleaning(DWORD theTime);

	// check the cache to clean up stuff
	gbool CheckCache();


	// check UMEL directories from UMEL urn
	// result TRUE : srcFilePath contains local file, info file info
	//

	gbool GetUmelUrl(const char *url, CString &srcFilePath,GFileInfo &info, gbool &isFile);

};

// global :
// the global urlcache
extern GUrlCache theCache;

// AFX_THREADPROC
// cache cleaner thread function
UINT G__cdecl CUrlCacheCleanerThread (LPVOID pParam);




#endif // _GUrlCache_H
