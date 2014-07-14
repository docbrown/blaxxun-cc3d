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
#ifndef _GV_PLIST_
#define _GV_PLIST_

#include <gvbasic.h>

class GvPList {
  public:
    
    GvPList(int size=0);
   
    ~GvPList();

    void	append(void * ptr)
		{ 	if (nPtrs + 1 > ptrsSize) expand(nPtrs + 1);
	  		ptrs[nPtrs++] = ptr; 
		}


    int		find(const void *ptr) const;
    
    void	remove(int which);
    void	remove(void * ptr);

	void 	insert(int index,void * ptr); 

    inline int getLength() const	{ return (int) nPtrs;	}
    
    void	truncate(int start) { nPtrs = start; }
    
    inline void* operator [](int i) const	{ return ptrs[i]; }

	// HG 
    inline void* &	operator [](int i) { return ptrs[i]; }

    void	setSize(int size)
			{ if (size > ptrsSize) expand(size); nPtrs = size; }

  private:
    void **	ptrs;
    int		nPtrs;
    int		ptrsSize;		
    void	expand(int size);
};

#endif /* _GV_PLIST_ */
