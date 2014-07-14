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
#include <gvplist.h>

#define DEFAULT_INITIAL_SIZE	1
// hg was 4 

GvPList::GvPList(int size)
{
    ptrs  = NULL;
    nPtrs = ptrsSize = 0;

    setSize(size);
}

GvPList::~GvPList()
{
    if (ptrs != NULL)
		delete [] ptrs;
}

int
GvPList::find(const void *ptr) const
{
    int i;

    for (i = 0; i < nPtrs; i++)
	if (ptrs[i] == ptr)
	    return(i);

    return -1;
}


void
GvPList::remove(int which)
{
    int i;

    for (i = which; i < nPtrs - 1; i++)
		ptrs[i] = ptrs[i + 1];

    setSize(nPtrs - 1);
}

void	
GvPList::remove(void * ptr)
{
	int which = find(ptr);
	if (which>=0) remove(which);
}


void
GvPList::insert(int index,void * ptr)
{
   	 if (index<0) index = 0;
	 if (index>= nPtrs) index = nPtrs;

     setSize(nPtrs + 1);
	 
	 for(int i=nPtrs-2; i>= index; i--) {
		 ptrs[i+1] = ptrs[i];
	 }
	 ptrs[index] =ptr;
}

void
GvPList::expand(int size)
{
    void	**newPtrs;
    int	i;

    if (ptrsSize == 0)
		ptrsSize = DEFAULT_INITIAL_SIZE;

    while (size > ptrsSize) {
		ptrsSize *= 2;
    }

    newPtrs = new (void *[ptrsSize]);

    if (ptrs != NULL) {
		for (i = 0; i < nPtrs; i++)
			newPtrs[i] = ptrs[i];
		delete [] ptrs;
    }

    ptrs = newPtrs;
}
