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
#ifndef _GUnzip_h
#define _GUnzip_h

/* ===========================================================================
 * Uncompress input to output then close both files.
 */
//int  gz_uncompress( gzFile in, FILE   *out,BOOL reportErrors);

/* ===========================================================================
 * Uncompress the given file and remove the original.
 * ret <0 error, Error Message reported if reportErrors 
 * ret >=0 ok,
 */
int gz_file_uncompress(char *file,BOOL reportErrors);


/* ===========================================================================
 * Uncompress the given infile to outfile 
 * ret <0 error, Error Message reported if reportErrors 
 * ret >=0 ok,
 */
int gz_file_uncompress(const char *infile,const char *outfile,BOOL reportErrors);

#endif // _GUnzip_h

