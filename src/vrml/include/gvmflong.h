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
#ifndef  _GV_MF_LONG_
#define  _GV_MF_LONG_

#include <gvsubfield.h>

class GvMFLong : public GvMField {
    GV_MFIELD_HEADER(GvMFLong,long,1);

  public:
    long *values;

	operator long *() { return(values); }
    long& operator[] (int i) { return(values[i]);}

	static GvFieldClass getClassFieldType() { return (MFLONG); }
	GvFieldClass getFieldType() const { return (MFLONG); }
	GvFieldClass getSFFieldType() const { return (SFLONG); }

    void set1(int index, long value);
    const long  *get() { return values; }

	long get1(int i) const { if (i>=num) i = num-1; return(values[i]); }
    int set(long val);

	int io1Value(AStream &a, int index);
	int get1(GvString &value, int index);
	int set1(const char* value, int index);

	GvField* get1Field(int index);


    void Interpolate (long *result, const long *A, float ta, const long *B,float tb) { *result = (long) (*A * ta+ *B *tb); }

    // Interpolate cnt values result = A * ta + B * tb
    void Interpolate (long *result, const long *A, long ta, const long *B,long tb, int cnt) 
    { 
        while (cnt >0 ) {
            Interpolate(result,A,ta,B,tb);
            result ++; A++; B++; cnt--;
        }
    }

};

#endif /* _GV_MF_LONG_ */
