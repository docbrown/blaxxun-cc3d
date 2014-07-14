//  -*- C++ -*-

/*
 * Copyright (C) 1996   Silicon Graphics, Inc.
 *
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |   $Revision: 1.1 $
 |
 |   Description:
 |	This contains the definition of the VbPList generic pointer
 |	list class. An VbPList is a list of (void *) pointers that
 |	allows easy insertion, removal, and other operations.
 |
 |	The VbIntList class allows lists of integers to be created.
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson, Gavin Bell
 |      (for the PC)    : Jim Kent, Daniel Woods
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef _VB_PLIST_
#define _VB_PLIST_

#include "vbbasic.h"

//////////////////////////////////////////////////////////////////////////////
//
//  Class: VbPList
//
//  List of generic (void *) pointers. VbPList allows random access,
//  insertion, and removal.
//
//////////////////////////////////////////////////////////////////////////////

class VbPList {
  public:

    // Constructor
    VbPList();

    // Constructor allocates enough room for the given number of pointers
    VbPList(int initSize);

    // Constructor for copying
    VbPList(const VbPList &pl);

    // Destructor
    ~VbPList();

    // Adds given pointer to end of list
    void	append(void * ptr)
	{ if (nPtrs + 1 > ptrsSize) expand(nPtrs + 1);
	  ptrs[nPtrs++] = ptr; }

    // Returns index of given pointer in list, or -1 if not found
    int		find(const void *ptr) const;

    // Inserts given pointer in list before pointer with given index
    void	insert(void *ptr, int addBefore);

    // Removes pointer with given index
    void	remove(int which);

    // Returns number of pointers in list
    int		getLength() const		{ return (int) nPtrs;	}

    // Removes all pointers after one with given index, inclusive
    // ??? should really be an error check in here
    void	truncate(int start)
	{ nPtrs = start; }

    // Copy a PList
    void	copy(const VbPList &pl);

    VbPList &	operator =(const VbPList &pl)	{ copy(pl); return *this; }

    // Returns pointer with given index
    void *&	operator [](int i) const
    	{ if (i >= nPtrs) grow(i); return ptrs[i]; }

    // equality tests
    int		operator ==(const VbPList &pl) const
	{ return pl.nPtrs == nPtrs ? compare(pl) : FALSE; }
    int		operator !=(const VbPList &pl) const
	{ return pl.nPtrs == nPtrs ? ! compare(pl) : TRUE; }

    void *	get(int i) const { return ptrs[i]; }
    void	set(int i, void *j) { ptrs[i] = j; }

  private:

    // NOTE: this must be called only if the number of elements in the two
    // lists is the same, otherwise badness could result
    int		compare(const VbPList &pl) const;

    void **	ptrs;		// The collection of pointers
    int		nPtrs;		// Number of pointers used
    int		ptrsSize;	// Number of pointers allocated

    // There are three(!) methods to expand the list.  grow() is used
    // when space is dynamically created, and needs to be initialized
    // to NULL:
    void	grow(int max) const;

    // setSize is used by grow and in other places internally where we
    // know that nothing needs to be initialized to NULL.
    void	setSize(int size)
	{ if (size > ptrsSize) expand(size); nPtrs = size; }

    // expand is the lowest level routine.  It just reallocates the
    // array and copies over the old values.
    void	expand(int size);
};

//////////////////////////////////////////////////////////////////////////////
//
//  Class: VbStringList
//
//  A list of strings.  This list is used to list classes associated with 
//  a specific error.
//
//////////////////////////////////////////////////////////////////////////////

class VbString;

class VbStringList : public VbPList {
  public:
    void	append(VbString *string)
	{ ((VbPList *) this)->append((void *) string); }

    int 	 find(VbString *string)
	{ return ((VbPList *) this)->find((void *) string); }

    void	insert(VbString *string, int addBefore)
	{ ((VbPList *) this)->insert((void *) string, addBefore); }

    VbString *&	operator [](int i) const
	{ return ( (VbString *&) ( (*(const VbPList *) this) [i] ) ); }
};


#endif /* _VB_PLIST_ */
