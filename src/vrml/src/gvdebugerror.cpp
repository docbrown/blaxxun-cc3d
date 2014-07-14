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

#include "greporter.h"

#include <stdarg.h>
#include <gvstring.h>
#include <gvdebugerror.h>

void
G__cdecl GvDebugError::post(const char *methodName, const char *formatString, ...)
{
    char	buf[10000];
    va_list	ap;

    va_start(ap, formatString);
#ifdef HAS_vsnprintf
    _vsnprintf(buf, sizeof(buf), formatString, ap);
#else
    vsprintf(buf, formatString, ap);
#endif
    va_end(ap);

    Reporter.Error("VRML error in %s(): %s\n", methodName, buf);

//    fprintf(stderr, "VRML error in %s(): %s\n", methodName, buf);
//    TRACE("VRML error in %s(): %s\n", methodName, buf);
}
