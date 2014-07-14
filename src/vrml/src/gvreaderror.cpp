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
#include "stdafx.h"
#include <stdarg.h>
#include <gvstring.h>
#include <gvinput.h>
#include <gvreaderror.h>

#include "greporter.h"


void
G__cdecl GvReadError::post(GvInput *in,GvReadErrorCode errorCode, const char *formatString, ...)
{
  
	// report only one error per line 
	if (in->lastErrorLineNum == in->lineNum) return;
	
	in->errorCode = errorCode;

	if (!in->reportErrors) return;
	
	char	buf[10000];
	int cnt;

    va_list	ap;

    va_start(ap, formatString);

#ifdef HAS_vsnprintf
    cnt = _vsnprintf(buf,sizeof(buf), formatString, ap); // win32 
#else
   cnt = vsprintf(buf, formatString, ap);
#endif

    va_end(ap);

	if (cnt <=0) {
		ASSERT(0);
		return;
	}


    GvString locstr;
    in->getLocationString(locstr);

	GReporter *reporter = in->getReporter();

	if (reporter)
	if (in->src != NULL)
	    reporter->Error("VRML syntax error: %s in %s:\n%s", locstr.getString(), in->src, buf );
	else reporter->Error("VRML syntax error: %s\n%s",locstr.getString(), buf );

    //fprintf(stderr, "VRML read error: %s\n%s\n", buf, locstr.getString());
    //TRACE("VRML read error: %s\n%s\n", buf, locstr.getString());
	
	in->lastErrorLineNum = in->lineNum;
}
