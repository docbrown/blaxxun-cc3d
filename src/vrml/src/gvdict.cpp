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
#include <gvdict.h>

struct GvDictListThing {
    GvPList	*keyList;
    GvPList	*valueList;
};

GvDict::GvDict( int entries )
{
    tableSize=entries;
    buckets=new GvDictEntry *[tableSize];
    for (int i = 0; i < tableSize; i++)
	buckets[i] = NULL;
}

GvDict::~GvDict()
{
    clear();
    delete [] buckets;
}

void
GvDict::clear()
{
    int		i;
    GvDictEntry	*entry, *nextEntry;

    for (i = 0; i < tableSize; i++) {

	for (entry = buckets[i]; entry != NULL; entry = nextEntry) {
	    nextEntry = entry->next;
	    delete entry;
	}
	buckets[i] = NULL;
    }
}

GvBool
GvDict::enter(u_long key, void *value)
{
    GvDictEntry		*&entry = findEntry(key);
    if (entry == NULL) {
	    entry = new GvDictEntry(key, value);
	    entry->next = NULL;
	    return TRUE;
    }
    else {
	    entry->value = value;
	    return FALSE;
    }
}

GvBool
GvDict::find(u_long key, void *&value) const
{
    GvDictEntry		*&entry = findEntry(key);

    if (entry == NULL) {
	value = NULL;
	return FALSE;
    }
    else {
	value = entry->value;
	return TRUE;
    }
}

GvDictEntry *&
GvDict::findEntry(u_long key) const
{
    GvDictEntry		**entry;

    entry = &buckets[key % tableSize];

    while (*entry != NULL) {
	if ((*entry)->key == key)
	    break;
	entry = &(*entry)->next;
    }
    return *entry;
}

GvBool
GvDict::remove(u_long key)
{
    GvDictEntry		*&entry = findEntry(key);
    GvDictEntry		*tmp;

    if (entry == NULL)
	    return FALSE;
    else {
	    tmp = entry;
	    entry = entry->next;
	    delete tmp;
	    return TRUE;
    }
}
