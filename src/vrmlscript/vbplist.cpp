/*
 * Copyright (C) 1996   Silicon Graphics, Inc.
 *
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |   $Revision: 1.2 $
 |
 |   Classes:
 |	VbPList
 |
 |   Author(s)		: Nick Thompson, Paul S. Strauss
 |      (For the PC)    : Jim Kent, Daniel Woods
 |
 |   Notes:
 |	VbPLists are stored as dynamically-allocated arrays.
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include "stdafx.h"

#include "vbplist.h"

#define DEFAULT_INITIAL_SIZE	4

//
// Constructor
//

VbPList::VbPList()
{
    ptrs  = NULL;
    nPtrs = ptrsSize = 0;

    setSize(0);
}

//
// Constructor allocates enough room for the given number of pointers.
//

VbPList::VbPList(int initSize)
{
    ptrs  = NULL;
    nPtrs = ptrsSize = 0;

    setSize(initSize);		// Makes enough room for initSize entries
    setSize(0);			// Sets nPtrs back to 0
}

//
// Initialize one PList from another
//

VbPList::VbPList(const VbPList &pl)
{
    int i;

    nPtrs = pl.nPtrs;
    ptrsSize = pl.ptrsSize;
    ptrs = new void *[ptrsSize];

    for (i = 0; i < nPtrs; i++)
	ptrs[i] = pl.ptrs[i];
}

//
// Destructor
//

VbPList::~VbPList()
{
    if (ptrs != NULL)
	delete [] ptrs;
}

//
// Returns index of given pointer in list, or -1 if not found
//

int
VbPList::find(const void *ptr) const
{
    int i;

    for (i = 0; i < nPtrs; i++)
	if (ptrs[i] == ptr)
	    return(i);

    return -1;	// Not found
}

//
// Inserts given pointer in list before pointer with given index
//

void
VbPList::insert(void *ptr, int addBefore)
{
    int	i;

    // If addBefore is off the end of the list, grow the list (and
    // initialize any new elements to NULL)
    if (addBefore > nPtrs) grow(addBefore);

    // Make room for one more
    setSize(nPtrs + 1);

    // Move pointers forward to make room
    for (i = nPtrs - 1; i > addBefore; --i)
	ptrs[i] = ptrs[i - 1];

    // Insert the new one
    ptrs[addBefore] = ptr;
}

//
// Removes pointer with given index
//

void
VbPList::remove(int which)
{
    int i;

    // Move all pointers after the ith one backward
    for (i = which; i < nPtrs - 1; i++)
	ptrs[i] = ptrs[i + 1];

    // Shrink the list
    setSize(nPtrs - 1);
}

//
// Copies a pointer list
//

void
VbPList::copy(const VbPList &pl)
{
    int i;

    setSize(pl.nPtrs);

    for (i = 0; i < nPtrs; i++)
	ptrs[i] = pl.ptrs[i];
}

//
// Makes the list big enough for "newSize" elements, and initializes
// any newly created elements to NULL.
//

void
VbPList::grow(int max) const
{
    int newSize = max+1;
    int oldSize = nPtrs;

    // Get around the const thing:
    VbPList *me = (VbPList *)this;

    me->setSize(newSize);
    
    for (int i = oldSize; i < newSize; i++) {
		me->ptrs[i] = NULL;
    }
}


//
// Changes size of list to be new size. If the new size is larger than
// the old one, it allocates more space.
//

void
VbPList::expand(int size)
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

// NOTE: this must be called only if the number of elements in the two
// lists is the same, otherwise badness could result

int
VbPList::compare(const VbPList &pl) const
{
    int i;
    for (i = 0; i < nPtrs; i++)
	if ((*this)[i] != pl[i])
	    return FALSE;

    return TRUE;
}

/* COMMENTED OUT FOR PC
VbVec3fList::~VbVec3fList()
{
    int		i;

    for(i = 0; i < getLength(); i++)
	delete (*this)[i];
}
   END COMMENTED OUT FOR PC */
