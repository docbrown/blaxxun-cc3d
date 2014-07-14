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

Array.cpp

Copyright (c) 1995	by Holger Grahn
All rights reserved

Purpose:

Template based Array class
Designed to easily interchange with C array notation
and "by value" elements
Derived class NumericArray supports common arithmetic operators


Classes:
Array<T>
	NumericArray<T>
		IntArray
		FloatArray


Notes:

Changes:

Todo :
Automatic Expansion + Expansion factor
Implementation change :
	use malloc, realloc, free instead of new & delete ?
******************************************************************************/
#include "stdafx.h"

//#include "astream.h"
//#include "array.h"


// Implementation for some member function of IntArray & FloatArray
//
int IntArray::io(AStream &a)
{ int i;

  int  newelement;

  if (a.BeginList()) {

   if (a.isOutput()) {
     for(i=0;i<np;i++) {
	    if (i>0) a.ListSep();
	    if ((i % 16) == 15) a.nl();
	    //p[i].io(a);
	    a.io(p[i]);
     }
   } else {
     np=0;
     /* read while ( until closing paren */
     while (a.Skip() != a.list_close) {
	   if (a.eof) break;
	   //newelement.io(a);
	   a.io(newelement);
	   if (np==maxp)  
	      SetMaxLength((np+1024) + (np/2)); // make array bigger 10 + 50 %
	   Append(newelement);
	   // to do : listsep
     }
  }

  a.EndList();
  }
  return (a.stat);
}

int FloatArray::io(AStream &a)
{ int i;

  float  newelement;

  if (a.BeginList()) {
   if (a.isOutput()) {
     for(i=0;i<np;i++) {
	    if (i>0) a.ListSep();
	    if ((i % 16) == 15) a.nl();
	    //p[i].io(a);
	    a.io(p[i]);
     }
   } else {
     np=0;
     /* read while ( until closing paren */
     while (a.Skip() != a.block_close) {
	   if (a.eof) break;
	   //newelement.io(a);
	   a.io(newelement);
	   if (np==maxp)  SetMaxLength((np+1024) + (np/2)); // make array bigger
	   Append(newelement);
	   // to do : listsep
     }
  }
  a.EndList();
  }
  return (a.stat);
}

