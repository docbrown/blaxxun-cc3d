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
#ifndef _GREPORTER_H
#define _GREPORTER_H

//@class reports warning / status / error messages 
class GReporter
{

public :
	static GReporter *current;	//  current global reporter
	static void *currentData;

	static GReporter *defaultReporter;	// default one 
	static void *defaultData;

	static void SetCurrent(GReporter * newReporter,void * Data = NULL) 
		{ current = newReporter; currentData = Data; }

	static void SetDefault(GReporter * newReporter,void * Data = NULL) 
		{ defaultReporter = newReporter; defaultData = Data; }

	
	BOOL ignoreWarnings;
	BOOL ignoreErrors;
	BOOL ingoreTrace;
	BOOL verbose;

	int errorCnt;
	int warningCnt;

	int doAbort;

	// critial section to serializes error messages 
	CRITICAL_SECTION reporterSection;  
    BOOL reporterSectionInit;
	int locked;

	void Lock(); 
	BOOL TryLock(); 

	void UnLock();


	// clear error counter
	void Clear() { errorCnt = warningCnt = 0; doAbort = 0; }

	void setIngore(BOOL warnings=FALSE,BOOL errors=FALSE) {
			ignoreWarnings = warnings; ignoreErrors = errors;
	}


	GReporter() {   ignoreWarnings = FALSE; ignoreErrors = FALSE; 
					ingoreTrace = FALSE;
					errorCnt = warningCnt = 0; doAbort = 0;
					reporterSectionInit = FALSE; 
					verbose = FALSE;
					locked = 0;
				}

	// low level virtual methdods for reporting 

	virtual int Warning1(const char * message);
	virtual int Status1(const char * message);
	virtual int Error1(const char * message);

	virtual int Trace1(const char * message);

	// high level like printf 
    int G__cdecl Warning(const char *formatString, ...);
    int G__cdecl Status(const char *formatString, ...);
    int G__cdecl Error(const char *formatString, ...);
    int G__cdecl Trace(const char *formatString, ...);

};

// the global reporter object
#define Reporter (*GReporter::current) 


#endif
		   
