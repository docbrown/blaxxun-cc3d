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
#ifndef _Matrix_h_
#define _Matrix_h_

/******************************************************************************
@doc

@module Matrix.h - 4*4 Matrix class	|


Purpose:

Classes:
<c Matrix>
<c MatrixAndInverse>
<c DecomposedMatrix>

Notes:

Changes:

$Revision: 1.14 $
$Log: matrix.h,v $
Revision 1.14  1999/07/06 16:55:17  tom
..

Revision 1.13  1999/04/07 15:20:54  tom
empty message



Todo :

******************************************************************************/

// Forward-declare some other classes
class Point;
//class ostream;

class AStream;

//! a 4*4 float transformation matrix
//! many operations are designed to not use itermediate or stack return values for speed
class Matrix {

public:

	enum Contents {
		HAS_TRANSLATION = 1,		//!< matrix has translation part

		HAS_SCALE		= 2,		//!< a scale part 
		
		HAS_NEGATIVE_SCALE = 4,		//!< negative scale 
		HAS_ROTATION	= 8,		//!< any rotation
		
		HAS_PERSPECTIVE	= 16,		//!< perspective part
		
		HAS_NON_UNIFORM_SCALE = 32,	//!< non uniform scale 
	

		HAS_ANY_SCALE = 2+4+32,
		HAS_ANY_TRANSFORM	= 1+2+4+8+32,
		HAS_ANYTHING	= 1+2+4+8+16+32,

	};	
	
	union {
		//! the 4*4 matrix
		float x[4][4];

#ifdef _D3D
		//! the 4*4 D3DMATRIX matrix alias
		D3DMATRIX md3d;
#endif

	};

	unsigned char contents;

	// identity : contents = 0
	gbool IsIdentity() const { return contents == 0; }
	gbool HasScale() const { return (contents & HAS_SCALE) !=0; }
	gbool HasRotation() const { return (contents & HAS_ROTATION) !=0; }
	gbool Has(int what) const { return (contents & what) !=0; }


	//! constructor, do nothing !
    Matrix() {  contents = HAS_ANYTHING; }

	//! constructor, with matrix 
    Matrix(const Matrix &m);


	//! Acess row
	float*  operator[] (int row) { return(x[row]);}
	//! Acess row
	const float*  operator[] (int row) const { return(x[row]);}

    // Assignment operators
    Matrix& operator+= (const Matrix& A);	//!< add-to
    Matrix& operator-= (const Matrix& A);	//!< subtract-from
    Matrix& operator*= (const Matrix& A);	//!< multiply by matrix

    Matrix& MultiplyLeft(const Matrix& A);	//!< multiply by matrix, A is left, this is right
    Matrix& MultiplyRight(const Matrix& A);	//!< multiply by matrix, A is right, this is left

	//! translation matrix 
	Matrix& TranslationMatrix(const Point& Location);
	Matrix& ScaleByLeft(const Point& scale);
    
	//! set to A*B, neither A or B is this !!!!!!
	Matrix& Multiply(const Matrix& A,const Matrix& B);	
    
	Matrix& operator*= (float A);		//!< scale by scalar

#ifdef _D3D
    //! return pointer as D3D matrix, same memory layout
	operator D3DMATRIX&() { return * ((D3DMATRIX *) &x[0][0]); } 
#endif


    // Fundamental operations
    void Invert();					//!< Invert the matrix
    void Transpose();				//!< Transpose the matrix
    
    friend Matrix Invert(const Matrix& M);		//!< Invert a given matrix
    friend Matrix Transpose(const Matrix& M);	//!< Transpose a given matrix

    void SetToInverse(const Matrix& M);	//!< set this to the inverse of a given matrix
	
	//! set to identiy matrix 
	Matrix& Identity();
	
	// *= assigment

	Matrix& Scale(float X, float Y, float Z);
	Matrix& Scale(const Point& scale);
	
	Matrix& Translate(const Point& Location);
	Matrix& Translate(float X, float Y, float Z);
	
	Matrix& Rotate(const Point& Rotation);
	Matrix& Rotate(float X, float Y, float Z);

    // Create various types of matrix.
    friend Matrix IdentityMatrix();
    friend Matrix ZeroMatrix();
    friend Matrix TranslationMatrix(const Point& Location);
	friend Matrix TranslationMatrix(float X, float Y, float Z);

    friend Matrix RotationXMatrix(float Angle);
    friend Matrix RotationYMatrix(float Angle);
    friend Matrix RotationZMatrix(float Angle);
    friend Matrix RotationMatrix(const Point& Rotation);
	friend Matrix RotationMatrix (float X, float Y, float Z);

    friend Matrix RotationYPRMatrix(float Yaw, float Pitch, float Roll);
    friend Matrix RotationAxisMatrix(const Point& axis, float Angle);
	friend Matrix RotationAxisMatrix(const Point &AxisPt, const Point& AxisDir, float angle);

    friend Matrix ScaleMatrix(float X, float Y, float Z);
    friend Matrix ScaleMatrix(const Point & Scale);
    
    friend Matrix GenRotation(const Point& x, const Point& y, const Point& z);
	
	//! gen a Rotation, so that vector v1 is mapped onto v2
    //! v1,v2 need to be normalized
	friend Matrix GenRotationMap(const Vector v1,const Vector v2);

    friend Matrix ViewMatrix(const Point& LookAt, const Point& Viewer, const Point& Up);
    friend Matrix QuadricMatrix(float a, float b, float c, float d, float e,
				float f, float g, float h, float j, float k);
    friend Matrix MirrorX();
    friend Matrix MirrorY();
    friend Matrix MirrorZ();
    friend Matrix RotationOnly(const Matrix& x);

    // Binary operators
    friend Matrix operator+ (const Matrix& A, const Matrix& B);
    friend Matrix operator- (const Matrix& A, const Matrix& B);
    friend Matrix operator* (const Matrix& A, float B);
    friend Matrix operator* (const Matrix& A, const Matrix& B);
    
	friend void Multiply (const Matrix& A, const Matrix& B,Matrix &ret);

    friend class Point operator* (const Matrix& M, const Point& v);
    friend class Point RotateOnly(const Matrix& M, const Point& v);
	friend class Vector operator* (const Matrix& M, const Vector&v); 

	// get a scale factor to transform values 
	float ScaleFactor() const ;

    int Decompose(Point &scale,Point &shear, Point &rotate, Point &translate) const;
	int Compose(const Point &scale, const Point &shear,const Point &rotate,const Point &translate);
	
	void PostTranslate(float tx,float ty,float tz); 	
	void PostScale(float sx,float sy,float sz); 


    /*
    friend class PointArray  PointArray& operator* (const Matrix& M, const PointArray& A);
    friend class PointArray PointArray& operator*= (const Matrix& M);
    */

    // Overloaded output operator.
    //friend ostream& operator<< (ostream& s, const Matrix& M);
	int io(AStream &a);	// input/output with AStreams
};

//! inverse is stored along with matrix
class MatrixAndInverse {

public:

	Matrix m;
	Matrix inverse;
	gbool isIdentity;

	MatrixAndInverse() { m.Identity();  inverse.Identity(); isIdentity=gtrue; }
	
    const Matrix& Inverse() const { return(inverse); }
    const Matrix& M() const { return(m); }
	
    gbool IsIdentity() const  { return(isIdentity); }
//	operator const Matrix &  { return m;}

	//! compute/update the inverse of the current matrix
	void ComputeInverse() { inverse = m; inverse.Invert(); }
	void Set(const Matrix &M) { m=M; ComputeInverse(); isIdentity=gfalse; }
	void Append(const Matrix &M) { m *= M; ComputeInverse(); isIdentity=gfalse; }
	void SetIdentity() { m.Identity();  inverse.Identity(); isIdentity=gtrue; } 

};

//! Decomposed Matrix
//! compound a certain class of matrix as
//! composed out of simple transformations
//
class DecomposedMatrix {
public:
	Point scale,shear,rotate,translate;

	DecomposedMatrix() {
				scale.Set(1.0,1.0,1.0);
				shear.Set();
				rotate.Set();
				translate.Set();
				
			};

	DecomposedMatrix(const Matrix m) {
		m.Decompose(scale,shear,rotate,translate);
		}
	
	int Compose(Matrix &m) {
		return(m.Compose(scale,shear,rotate,translate));
		};
	
	operator Matrix() { Matrix m; Compose(m); return(m); }

};

#endif

