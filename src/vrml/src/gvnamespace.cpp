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

@module GvNameSpace.h - VRML 2.0 NameSpaces implementation|

Copyright (c) 1996	by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GvNameSpace>

Notes:

Changes:

$Revision: 1.4 $
$Log: gvnamespace.cpp,v $
Revision 1.4  1999/07/06 16:46:03  tom
*** empty log message ***

Revision 1.3  1999/05/10 13:55:02  tom
*** empty log message ***



Todo :

******************************************************************************/

#include "stdafx.h"
#include <gvnamespace.h>
#include <gvproto.h>


GvNameSpace::GvNameSpace(): parsedProtos(0),next(NULL),nodeNameGenCounter(0)
{ 
}
GvNameSpace::~GvNameSpace()
{ 
}

// names are mapped to an identical string, so name.getString() is a unique key

GvBool
GvNameSpace::addNode(const GvName &name, GvNode *node)
{
	// its up to context to keep node alive !
    return  nodes.enter((u_long) name.getString(), (void *) node);
}

GvBool
GvNameSpace::removeNode(const GvName &name)
{
    return  nodes.remove((u_long) name.getString());
}

GvBool
GvNameSpace::removeNode(const GvName &name, GvNode *node)
{
	void *current;
    if (nodes.find((u_long) name.getString(),  current))
		if ((GvNode *) current == node) 
		  return nodes.remove((u_long) name.getString());
	return FALSE;
}


GvNode *
GvNameSpace::findNode(const GvName &name) const
{
    void	*node;

    if (nodes.find((u_long) name.getString(), node))
	    return (GvNode *) node;
    return NULL;
}

// RETURNS true, and in name a new unique node name, currently not in use
GvBool GvNameSpace::generateNodeName(const char *prefix,GvName &name) 
{
    char buf[80];

    while(1) {        
        nodeNameGenCounter++;
        sprintf(buf,"%s_%d",prefix,nodeNameGenCounter);
        {
            GvName newName(buf);
            if (findNode(newName) == NULL) {
                name = newName;
                return(TRUE);
            }
        }
    }
    return(FALSE);
}


// RETURNS true, and in name a new unique node name, currently not in use
GvBool generateNodeName(GvDict &dict,const char *prefix,GvName &name)
{
    char buf[80];
    int nodeNameGenCounter = 0;

    while(1) {        
        nodeNameGenCounter++;
        sprintf(buf,"%s_%d",prefix,nodeNameGenCounter);
        {
            GvName newName(buf);
            void	*node;

            if (!dict.find((u_long) newName.getString(), node)) {
                name = newName;
                return(TRUE);
            }
        }
    }
    return(FALSE);
}



GvBool
GvNameSpace::addProto(const GvName &name, GvProto *node)
{
	parsedProtos.append(node);
    return  protos.enter((u_long) name.getString(), (void *) node);
}

GvProto *
GvNameSpace::findProto(const GvName &name,GvBool recursive) const
{
    void	*node;

    if (protos.find((u_long) name.getString(), node))
	    return (GvProto *) node;
    
    // search all nested name spaces
    if (next && recursive) return(next->findProto(name,recursive));

    return NULL;
}

