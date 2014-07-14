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
/******************************************************************************
@doc

@module GvDB.cpp - VRML Scengraph library init / term|

Copyright (c) 1996 - 1998 by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GvDB>

  
Notes:

Changes:

$Revision: $
$Log: $

Todo :

******************************************************************************/


#include <gvdb.h>

#include "gclass.h"

#include <gvelement.h>

#include <gvinput.h>
#include <gvreaderror.h>
#include <gvnode.h>

#include <gvseparator.h>

const char *GvDB::versionString = "GLView VRML Parser 2.0";


void
GvDB::init()
{
	GStackableAttribute::InitClass();
    GvNode::init();
}




void
GvDB::term()
{
    GvNode::term();


#ifdef _DEBUG
	// for debugging clean up static class information stuff
	GvElement::termClass();
	GClassFreeAttributeClasses();
	GvNameEntry::termClass();
#endif



    // problem, static Nodes  ::  destructors will access name
    //termClass on all built in nodes to free fieldData
    //
}



GvBool
GvDB::read(GvInput *in, GvNode *&node)
{
    GvBool ret;
#if 0
	GvSeparator *sep=NULL;		// allow multi 
	int cnt=0;
	ret = TRUE;
	GvNode *newNode;
	
	for(;ret;) {
	
	    ret = GvNode::read(in, newNode);
	
		if (!ret) break;
		
		if (!newNode) {      // HG 26.01.96
			GvBool	gotChar;
			char c;
			if ((gotChar = in->get(c))) 
				GvReadError::post(in, "Unexpected extra characters ('%c') ", c);
			if (gotChar) in->putBack(c);
			break;
		}

		
	
		cnt++;
		if (cnt > 1) {
		  if (sep == NULL) {
		     sep = new GvSeparator();
			 if (node) {		// add first
		     	sep->addChild(node);
			 }

		  }
		  if (newNode != NULL) {
		    	sep->addChild(newNode);
		  }
		}
		else node = newNode;

	    if (!in->skipWhiteSpace())  break;
	
		if (in->eof()) break;
		


	}

	if (sep) {
		node = sep;
		ret = TRUE;
	}
	else if (node) {
		ret = TRUE;
	}
#else

    ret = GvNode::read(in, node);

    if (ret && node == NULL && ! in->eof()) {
		char	c;
		in->get(c);
		GvReadError::post(in,GV_RE_SYNTAX_ERROR, "Extra characters ('%c') found in input", c);
		ret = FALSE;
    }
#endif

    return ret;
}

