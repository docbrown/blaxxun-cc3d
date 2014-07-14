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
#ifndef _STACK_H
#define _STACK_H

// Stack class based on Array

template<class T>
class Stack : public Array<T>
{
public:
	Stack() : Array<T>()	{};
	Stack(int initSize) : Array<T>(initSize)	{ SetLength(0); };

	int GetDepth() { return Length(); }
	void SetDepth(int newDepth) { SetLength(newDepth); }
	
	int IsEmpty() { return Length()==0; }

	// Push a new Element
	void Push(T NewTop) { Append(NewTop); }

	// get Top Element
	T Top() { return(p[Length()-1]); }
	
	// Pop a Element
	void PopTop() { SetLength(Length()-1); }

	T Pop() { T top =Top(); SetLength(Length()-1); return(top); }


	// operations from array which should be ok
	// Flush, =, Append ,io
};

#endif

