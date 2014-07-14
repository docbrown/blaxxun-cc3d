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
#ifndef _GV_DICT_
#define _GV_DICT_

#include <gvbasic.h>
#include <gvstring.h>
#include <gvplist.h>

class GvDictEntry {
  private:
    u_long		key;
    void *		value;
    GvDictEntry *	next;
    GvDictEntry(u_long k, void *v)	{ key = k; value = v; };

friend class GvDict;
};

class GvDict {
  public:
    GvDict( int entries = 251 );
    ~GvDict();
    void	clear();
    GvBool	enter(u_long key, void *value);
    GvBool	find(u_long key, void *&value) const;
    GvBool	remove(u_long key);

  private:
    int			tableSize;
    GvDictEntry *	*buckets;
    GvDictEntry *&	findEntry(u_long key) const;
};

#endif /* _GV_DICT_ */
