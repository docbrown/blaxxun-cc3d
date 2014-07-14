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
#ifndef _GTRACE_H
#define _GTRACE_H

//@class reports warning / status / error messages 
class GTrace
{

public :
	static GTrace *current;

	static BOOL doTrace;
	

	static void SetCurrent(GTrace * newTrace) 
		{ current = newTrace;}

	FILE *logFile;


	GTrace(FILE *f) { logFile = f; doTrace = logFile != NULL; }
	~GTrace() { if (logFile != NULL) fclose(logFile); }


	int Warning1(const char * message);
    int G__cdecl Warning(const char *formatString, ...);
	void Flush() { if (logFile) fflush(logFile); }

};

#define Tracer (*GTrace::current) 


class GEaiTrace
{
public :
	static int eaiLevel;

	GEaiTrace(const char *msg="EAI Call") {
		eaiLevel++;
		Tracer.Warning1(msg);
		Tracer.Warning1("(");
		if (eaiLevel >1) {
			Tracer.Warning1("******* nested ");
			Tracer.Flush();
		}

	}

	~GEaiTrace() {
		Tracer.Warning1(")\n");
		eaiLevel--;

	}
};
// do trace 
//#define _EAI_TRACE

#ifdef _EAI_TRACE
#define EAI_CALL(name) GEaiTrace tempTrace(name);
#define EAI_TRACE Tracer.Warning 
#define EAI_FLUSH() Tracer.Flush()
#else
#define EAI_CALL(name)
// allow a dummy call EAI_TRACE(A,B,C)
#define EAI_TRACE	1 ? 0 : printf

//Tracer.Warning 

#define EAI_FLUSH()
#endif

#endif
		   
