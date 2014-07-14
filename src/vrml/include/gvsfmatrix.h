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
#ifndef  _GV_SF_MATRIX_
#define  _GV_SF_MATRIX_

// HG
#include <gvnode.h>

#include <gvsubfield.h>

class GvSFMatrix : public GvSField {
  
  public:
  
   
	Matrix value;
  
    GV_SFIELD_HEADER(GvSFMatrix);

    GvSFMatrix() {
					value.Identity();
                 }

    GvSFMatrix(Matrix &m) : value(m) { }

	static GvFieldClass getClassFieldType() { return (SFMATRIX); }
	GvFieldClass	 getFieldType() const { return (SFMATRIX); }


    const float* getValue() const  { return &value[0][0];}

    void get(Matrix &m) {
			m = value;
	}

    Matrix &get() {
			return value;
	}

    void set(const Matrix &m) {
			value = m;
            setDefault(FALSE); OnChanged();
	}

	int ioValue(AStream &a); 
	
    int get(GvString &value);
	int set(const char* value);

	float get1(int index) { 
		return value[index /4] [index %4]; 
	}
	void set1(int index, float v)	{ 
			value[index /4] [index %4] = v; 
			value.contents = Matrix::HAS_ANYTHING;
			setDefault(FALSE); OnChanged(); 
	}

    
    const GvSFMatrix& operator= (const GvSFMatrix &f) 
    { 
			value = f.value;
            setDefault(FALSE); OnChanged();

            return(f); 
    }

    GvBool operator==(const GvSFMatrix &f) const { 
			for (int i=0; i<4; i++) 
			for (int j=0; j<4; j++) 
				if (value[i][j] != f.value[i][j]) return(FALSE);
            return(TRUE); 
    }

};

#endif /* _GV_SF_MATRIX_ */
