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
// ===================================================================
// matrix.cpp
// 20.10.96     denom = v.x * M.x[0][3] + v.y * M.x[1][3] + v.z * M.x[2][3] + M.x[3][3];

// ===================================================================
#include "stdafx.h"
//#include <math.h>
//#include <iostream.h>
//#include <iomanip.h>
#include "gutils.h"
//#include "point.h"
//#include "matrix.h"
//#include "astream.h"


Matrix::Matrix(const Matrix &m) 
{
	memcpy(&x[0][0],&m.x[0][0],sizeof(x[0][0]) * 16);
	contents = m.contents;
}

  // Graphic Gems I Pg. 770
  // Post concatenation

void Matrix::PostTranslate(float tx,float ty,float tz) 
  {
	for(int i=0; i<4; i++) {
		x[i][0] += x[i][3]*tx;
		x[i][1] += x[i][3]*ty;
		x[i][2] += x[i][3]*tz;
	}
	contents |= HAS_TRANSLATION;
  }

void Matrix::PostScale(float sx,float sy,float sz) 
{
	for(int i=0; i<4; i++) {
		x[i][0] *= sx;
		x[i][1] *= sy;
		x[i][2] *= sz;
	}
	contents |= HAS_SCALE;

	if ((sx <=0) || (sy <=0) || (sz <=0))
		contents |= HAS_NEGATIVE_SCALE;

	if ((sx != sy) || (sy != sz))
		contents |= HAS_NON_UNIFORM_SCALE;

}


static inline double MINOR(const Matrix& M,
                    int r0, int r1, int r2, int c0, int c1, int c2)
{
  return M[r0][c0] * (M[r1][c1] * M[r2][c2] - M[r2][c1] * M[r1][c2]) -
	 M[r0][c1] * (M[r1][c0] * M[r2][c2] - M[r2][c0] * M[r1][c2]) +
	 M[r0][c2] * (M[r1][c0] * M[r2][c1] - M[r2][c0] * M[r1][c1]);
}



Matrix Adjoint(const Matrix& m) 
{
  Matrix A;
  
  A[0][0] =  MINOR(m, 1, 2, 3, 1, 2, 3);
  A[0][1] = -MINOR(m, 0, 2, 3, 1, 2, 3);
  A[0][2] =  MINOR(m, 0, 1, 3, 1, 2, 3);
  A[0][3] = -MINOR(m, 0, 1, 2, 1, 2, 3);
  A[1][0] = -MINOR(m, 1, 2, 3, 0, 2, 3);
  A[1][1] =  MINOR(m, 0, 2, 3, 0, 2, 3);
  A[1][2] = -MINOR(m, 0, 1, 3, 0, 2, 3);
  A[1][3] =  MINOR(m, 0, 1, 2, 0, 2, 3);
  A[2][0] =  MINOR(m, 1, 2, 3, 0, 1, 3);
  A[2][1] = -MINOR(m, 0, 2, 3, 0, 1, 3);
  A[2][2] =  MINOR(m, 0, 1, 3, 0, 1, 3);
  A[2][3] = -MINOR(m, 0, 1, 2, 0, 1, 3);
  A[3][0] = -MINOR(m, 1, 2, 3, 0, 1, 2);
  A[3][1] =  MINOR(m, 0, 2, 3, 0, 1, 2);
  A[3][2] = -MINOR(m, 0, 1, 3, 0, 1, 2);
  A[3][3] =  MINOR(m, 0, 1, 2, 0, 1, 2);

  A.contents = m.contents; // ?? what to return
  return A;
}

void 
Adjoint(const Matrix& m,Matrix &A) 
{
  
  A[0][0] =  MINOR(m, 1, 2, 3, 1, 2, 3);
  A[0][1] = -MINOR(m, 0, 2, 3, 1, 2, 3);
  A[0][2] =  MINOR(m, 0, 1, 3, 1, 2, 3);
  A[0][3] = -MINOR(m, 0, 1, 2, 1, 2, 3);
  A[1][0] = -MINOR(m, 1, 2, 3, 0, 2, 3);
  A[1][1] =  MINOR(m, 0, 2, 3, 0, 2, 3);
  A[1][2] = -MINOR(m, 0, 1, 3, 0, 2, 3);
  A[1][3] =  MINOR(m, 0, 1, 2, 0, 2, 3);
  A[2][0] =  MINOR(m, 1, 2, 3, 0, 1, 3);
  A[2][1] = -MINOR(m, 0, 2, 3, 0, 1, 3);
  A[2][2] =  MINOR(m, 0, 1, 3, 0, 1, 3);
  A[2][3] = -MINOR(m, 0, 1, 2, 0, 1, 3);
  A[3][0] = -MINOR(m, 1, 2, 3, 0, 1, 2);
  A[3][1] =  MINOR(m, 0, 2, 3, 0, 1, 2);
  A[3][2] = -MINOR(m, 0, 1, 3, 0, 1, 2);
  A[3][3] =  MINOR(m, 0, 1, 2, 0, 1, 2);

  A.contents = m.contents; // ?? what to return

}



double Determinant(const Matrix& m)
{
/*		5.5.97 
  return 
	m[0][0] * MINOR(m, 1, 2, 3, 1, 2, 3) -
	 m[0][1] * MINOR(m, 1, 2, 3, 0, 2, 3) +
	 m[0][2] * MINOR(m, 1, 2, 3, 0, 1, 3) -
	 m[0][3] * MINOR(m, 1, 2, 3, 0, 1, 2);
*/
  double ret = 0.0;
  if (m[0][0] != 0.0f) ret = m[0][0] * MINOR(m, 1, 2, 3, 1, 2, 3);
  if (m[0][1] != 0.0f) ret -= m[0][1] * MINOR(m, 1, 2, 3, 0, 2, 3);
  if (m[0][2] != 0.0f) ret += m[0][2] * MINOR(m, 1, 2, 3, 0, 1, 3);
  if (m[0][3] != 0.0f) ret -= m[0][3] * MINOR(m, 1, 2, 3, 0, 1, 2);
  return ret;

}
#ifdef _D3D
gbool InvertSimple( D3DMATRIX& q, const D3DMATRIX& a )
{
	/* already checked for persp part !
    if( fabs(a._44 - 1.0f) > .001f)
        return gfalse;
    if( fabs(a._14) > .001f || fabs(a._24) > .001f || fabs(a._34) > .001f )
        return gfalse;
*/

    gfloat fDetInv = 1.0f / ( a._11 * ( a._22 * a._33 - a._23 * a._32 ) -
                             a._12 * ( a._21 * a._33 - a._23 * a._31 ) +
                             a._13 * ( a._21 * a._32 - a._22 * a._31 ) );

    q._11 =  fDetInv * ( a._22 * a._33 - a._23 * a._32 );
    q._12 = -fDetInv * ( a._12 * a._33 - a._13 * a._32 );
    q._13 =  fDetInv * ( a._12 * a._23 - a._13 * a._22 );
    q._14 = 0.0f;

    q._21 = -fDetInv * ( a._21 * a._33 - a._23 * a._31 );
    q._22 =  fDetInv * ( a._11 * a._33 - a._13 * a._31 );
    q._23 = -fDetInv * ( a._11 * a._23 - a._13 * a._21 );
    q._24 = 0.0f;

    q._31 =  fDetInv * ( a._21 * a._32 - a._22 * a._31 );
    q._32 = -fDetInv * ( a._11 * a._32 - a._12 * a._31 );
    q._33 =  fDetInv * ( a._11 * a._22 - a._12 * a._21 );
    q._34 = 0.0f;

    q._41 = -( a._41 * q._11 + a._42 * q._21 + a._43 * q._31 );
    q._42 = -( a._41 * q._12 + a._42 * q._22 + a._43 * q._32 );
    q._43 = -( a._41 * q._13 + a._42 * q._23 + a._43 * q._33 );
    q._44 = 1.0f;

    return gtrue;
}
#endif

void
Matrix::Invert()
{
	// identity matrix, is the inverse for itself
	if (contents ==0)
		return;

	Matrix m;
	
	if ((contents & (HAS_PERSPECTIVE /*| HAS_NON_UNIFORM_SCALE*/)) == 0) {

#ifdef _D3D
		if (InvertSimple(m.md3d,md3d)) {
			this->md3d= m.md3d;
			return;
		}
#endif

	}

	// assert(!isSingular());
   float det =  (1.0/ Determinant(*this));
   
	Adjoint(*this,m);
/*
	if (det == 1.0f) { 
		*this=m; return;
	}
*/
	for (int i = 0; i < 4; i++)
	for (int j = 0; j < 4; j++)
	    x[i][j] = m[i][j]*det;
    
	contents = contents; // ?? what to return

}

// set this to the inverse of a given matrix
// avoid unecessarry copies 
void Matrix::SetToInverse(const Matrix& m)	
{
	// identity matrix, is the inverse for itself
	if (m.contents ==0) {
		Identity();
		return;
	}


	if ((m.contents & (HAS_PERSPECTIVE /*| HAS_NON_UNIFORM_SCALE*/)) == 0) {

#ifdef _D3D
		if (InvertSimple(md3d,m.md3d)) {
			return;
		}
#endif

	}
	
	// assert(!isSingular());
	float det =  (1.0/ Determinant(m));
   
	Adjoint(m,*this);

	if (det != 1.0f)  
		for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			x[i][j] *= det;
    
	contents = m.contents; // ?? what to return

}



//bool IsSingular(const Matrix& m) const
//{
//  return IsZero(Determinant(m));
//}



#if 0

// Invert the matrix using Gaussian elimination.  Not efficient,
// nor does it check for singular matrices.
void
Matrix::Invert()
{
    int i, j, k;
    Matrix Out = IdentityMatrix();

    for (i = 0; i < 4; i++) {
	if (x[i][i] != 1.0) {
	    float divby = 1.0f / x[i][i];
	    for (j = 0; j < 4; j++) {
			Out.x[i][j] *= divby;
			x[i][j] *= divby;
	    }
	}
	for (j = 0; j < 4; j++) {
	    if (j != i) {
		if (x[j][i] != 0.0) {
		    float mulby = x[j][i];
		      for (k = 0; k < 4; k++) {
				x[j][k] -= mulby * x[i][k];
				Out.x[j][k] -= mulby * Out.x[i][k];
		      }
		}
	    }
	}
    }
    *this = Out;
}
#endif 


// Invert the given matrix using the above inversion routine.
Matrix
Invert(const Matrix& M)
{
    Matrix InvertMe = M;
    InvertMe.Invert();
    return InvertMe;
}

// Transpose the matrix.
void
Matrix::Transpose()
{
    for (int i = 0; i < 4; i++)
	for (int j = i; j < 4; j++)
	    if (i != j) {
			float temp = x[i][j];
			x[i][j] = x[j][i];
			x[j][i] = temp;
	    }
}

// Transpose the given matrix using the transpose routine above.
Matrix
Transpose(const Matrix& M)
{
    Matrix TransposeMe = M;
    TransposeMe.Transpose();
    return TransposeMe;
}

// Construct an identity matrix.
Matrix
IdentityMatrix()
{
    Matrix M;

	const float one = 1.0f;
	const float zero = 0.0f;
    // help compiler ?
    M.x[0][0] = one;  M.x[1][0] =		M.x[2][0] =		M.x[3][0] =
    M.x[0][1] = zero; M.x[1][1] = one;  M.x[2][1] =		M.x[3][1] =
    M.x[0][2] =       M.x[1][2] = zero;	M.x[2][2] = one;  M.x[3][2] =
    M.x[0][3] =       M.x[1][3] =       M.x[2][3] = zero; M.x[3][3] =one;
/*
    for (int i = 0; i < 4; i++)
	for (int j = 0; j < 4; j++)
	    M.x[i][j] = (i == j) ? 1.0 : 0.0;
*/
	M.contents = 0;
    return M;
}

Matrix&
Matrix::Identity()
{
	const float one = 1.0f;
	const float zero = 0.0f;
    // help compiler ?
    x[0][0] = one;  x[1][0] =		x[2][0] =		x[3][0] =
    x[0][1] = zero; x[1][1] = one;  x[2][1] =		x[3][1] =
    x[0][2] =       x[1][2] = zero;	x[2][2] = one;  x[3][2] =
    x[0][3] =       x[1][3] =       x[2][3] = zero; x[3][3] = one;

/*
    for (int i = 0; i < 4; i++)
	for (int j = 0; j < 4; j++)
	    x[i][j] = (i == j) ? 1.0 : 0.0;
*/
	contents = 0;
    return *this;
}

// Construct a zero matrix.
Matrix
ZeroMatrix()
{
    Matrix M;
    for (int i = 0; i < 4; i++)
	for (int j = 0; j < 4; j++)
	    M.x[i][j] = 0;

	M.contents = Matrix::HAS_SCALE;

    return M;
}

// Construct a translation matrix given the location to translate to.
Matrix
TranslationMatrix(const Point& Location)
{
    Matrix M; M.Identity();
    M.x[3][0] = Location.x;
    M.x[3][1] = Location.y;
    M.x[3][2] = Location.z;
	M.contents = Matrix::HAS_TRANSLATION;
    return M;
}

Matrix&
Matrix::TranslationMatrix(const Point& Location)
{
    //Identity();
	const float one = 1.0f;
	const float zero = 0.0f;
    // help compiler ?
    x[0][0] = one;  x[1][0] =		x[2][0] =	/*	x[3][0] = */
    x[0][1] = zero; x[1][1] = one;  x[2][1] =	/*	x[3][1] = */
    x[0][2] =       x[1][2] = zero;	x[2][2] = one; /* x[3][2] = */
    x[0][3] =       x[1][3] =       x[2][3] = zero; x[3][3] = one;

    x[3][0] = Location.x;
    x[3][1] = Location.y;
    x[3][2] = Location.z;
	contents = HAS_TRANSLATION;
    return *this;
}


Matrix
TranslationMatrix(float X, float Y, float Z)
{
    Matrix M; M.Identity();
    M.x[3][0] = X;
    M.x[3][1] = Y;
    M.x[3][2] = Z;
	M.contents = Matrix::HAS_TRANSLATION;
    return M;
}

// Construct a rotation matrix.  Rotates Angle radians about the
// X axis.
Matrix
RotationXMatrix(float Angle)
{
    Matrix M; M.Identity();
    float Cosine = cos(Angle);
    float Sine = sin(Angle);
    M.x[1][1] = Cosine;
    M.x[2][1] = -Sine;
    M.x[1][2] = Sine;
    M.x[2][2] = Cosine;
	M.contents = Matrix::HAS_ROTATION;
    return M;
}

// Construct a rotation matrix.  Rotates Angle radians about the
// Y axis.
Matrix
RotationYMatrix(float Angle)
{
    Matrix M; M.Identity();
    float Cosine = cos(Angle);
    float Sine = sin(Angle);
    M.x[0][0] = Cosine;
    M.x[2][0] = Sine;
    M.x[0][2] = -Sine;
    M.x[2][2] = Cosine;
	M.contents = Matrix::HAS_ROTATION;
    return M;
}

// Construct a rotation matrix.  Rotates Angle radians about the
// Z axis.
Matrix
RotationZMatrix(float Angle)
{
    Matrix M; M.Identity();
    float Cosine = cos(Angle);
    float Sine = sin(Angle);
    M.x[0][0] = Cosine;
    M.x[1][0] = -Sine;
    M.x[0][1] = Sine;
    M.x[1][1] = Cosine;
	M.contents = Matrix::HAS_ROTATION;
    return M;
}

// can be optimized
Matrix 
RotationMatrix(const Point& Rotation)
{
   Matrix M = RotationXMatrix(Rotation.x);
   if (Rotation.y != 0.0) M *= RotationYMatrix(Rotation.y);
   if (Rotation.z != 0.0) M *= RotationZMatrix(Rotation.z);
   M.contents = Matrix::HAS_ROTATION;
   return M;
}


Matrix 
RotationMatrix (float X, float Y, float Z)
{
   Matrix M = RotationXMatrix(X);
   if (Y != 0.0) M *= RotationYMatrix(Y);
   if (Z != 0.0) M *= RotationZMatrix(Z);
   M.contents = Matrix::HAS_ROTATION;
   return M;
}

// Construct a yaw-pitch-roll rotation matrix.	Rotate Yaw
// radians about the XY axis, rotate Pitch radians in the
// plane defined by the Yaw rotation, and rotate Roll radians
// about the axis defined by the previous two angles.
Matrix
RotationYPRMatrix(float Yaw, float Pitch, float Roll)
{
    Matrix M;
    float ch = cos(Yaw);
    float sh = sin(Yaw);
    float cp = cos(Pitch);
    float sp = sin(Pitch);
    float cr = cos(Roll);
    float sr = sin(Roll);

    M.x[0][0] = ch*cr + sh*sp*sr;
    M.x[1][0] = -ch*sr + sh*sp*cr;
    M.x[2][0] = sh*cp;
    M.x[0][1] = sr*cp;
    M.x[1][1] = cr*cp;
    M.x[2][1] = -sp;
    M.x[0][2] = -sh*cr - ch*sp*sr;
    M.x[1][2] = sr*sh + ch*sp*cr;
    M.x[2][2] = ch*cp;
    for (int i = 0; i < 4; i++)
		M.x[3][i] = M.x[i][3] = 0;
    M.x[3][3] = 1;
	M.contents = Matrix::HAS_ROTATION;

    return M;
}

// Construct a rotation of a given angle about a given axis.
// Derived from Eric Haines's SPD (Standard Procedural
// Database).
Matrix
RotationAxisMatrix(const Point& axis_, float angle)
{
    
	if (angle == 0.0) {
		return IdentityMatrix();
	}
	Point axis = axis_;
	axis.Normalize();

	Matrix M;
    double cosine = cos(angle);
    double sine = sin(angle);
    double one_minus_cosine = 1 - cosine;

    M.x[0][0] = axis.x * axis.x + (1.0 - axis.x * axis.x) * cosine;
    M.x[0][1] = axis.x * axis.y * one_minus_cosine + axis.z * sine;
    M.x[0][2] = axis.x * axis.z * one_minus_cosine - axis.y * sine;
    M.x[0][3] = 0;

    M.x[1][0] = axis.x * axis.y * one_minus_cosine - axis.z * sine;
    M.x[1][1] = axis.y * axis.y + (1.0 - axis.y * axis.y) * cosine;
    M.x[1][2] = axis.y * axis.z * one_minus_cosine + axis.x * sine;
    M.x[1][3] = 0;

    M.x[2][0] = axis.x * axis.z * one_minus_cosine + axis.y * sine;
    M.x[2][1] = axis.y * axis.z * one_minus_cosine - axis.x * sine;
    M.x[2][2] = axis.z * axis.z + (1.0 - axis.z * axis.z) * cosine;
    M.x[2][3] = 0;

    M.x[3][0] = 0;
    M.x[3][1] = 0;
    M.x[3][2] = 0;
    M.x[3][3] = 1;

	M.contents = Matrix::HAS_ROTATION;

    return M;
}

// create a rotation maxtrix relative to AxisPt about vector AxisDir with angle

Matrix
RotationAxisMatrix(const Point &AxisPt, const Point& AxisDir, float angle)
{
    Matrix M;
	M = TranslationMatrix(-AxisPt);
	M *= RotationAxisMatrix(AxisDir,angle);
	M *= TranslationMatrix(AxisPt);
	return(M);
}

// Construct a scale matrix given the X, Y, and Z parameters
// to scale by.  To scale uniformly, let X==Y==Z.
Matrix
ScaleMatrix(float X, float Y, float Z)
{
    Matrix M; M.Identity();

    M.x[0][0] = X;
    M.x[1][1] = Y;
    M.x[2][2] = Z;

	M.contents = Matrix::HAS_SCALE;

	if ((X <=0) || (Y <=0) || (Z <=0))
		M.contents |= Matrix::HAS_NEGATIVE_SCALE;

	if ((X != Y) || (Y != Z))
		M.contents |= Matrix::HAS_NON_UNIFORM_SCALE;

    return M;
}

Matrix 
ScaleMatrix(const Point& Scale)
{
  return(ScaleMatrix(Scale.x,Scale.y,Scale.z));
}

// Construct a rotation matrix that makes the x, y, z axes
// correspond to normalized the vectors given.
Matrix
GenRotation(const Point& x, const Point& y, const Point& z)
{
    Matrix M; 
    M.x[0][0] = x.x;  M.x[0][1] = y.x;  M.x[0][2] = z.x;  M.x[0][3] = 0;
    M.x[1][0] = x.y;  M.x[1][1] = y.y;  M.x[1][2] = z.y;  M.x[1][3] = 0;
    M.x[2][0] = x.z;  M.x[2][1] = y.z;  M.x[2][2] = z.z;  M.x[2][3] = 0;
    M.x[3][0] = 0;	  M.x[3][1] = 0;    M.x[3][2] = 0;    M.x[3][3] = 1;

	M.contents = Matrix::HAS_ROTATION;

    return M;
}

// gen a Rotation, so that vector v1 is mapped onto v2
// v1,v2 need to be normalized

Matrix
GenRotationMap(const Vector	v1,const Vector v2)
{
  Matrix M;
  Point v = CrossProd(v1,v2);
  double cosw = DotProd(v1,v2);
  double sinw = v.Len();
  v *= 1.0/sinw;	// normalize v
  return RotationAxisMatrix(v, atan2(cosw,sinw));
}



// Construct a view matrix to rotate and translate the world
// given that the viewer is located at Viewer, looking at
// LookAt, and the up vector is UpL.  The transformation
// changes things around so the viewer is at the origin and
// looking down the -Z axis.
// GGems I pg 516 / 778
Matrix
ViewMatrix(const Point& LookAt, const Point& Viewer, const Point& UpL)
{
    Matrix M; M.Identity();
    Point U, V, N;
    Point Up = Normalize(UpL);

//    N = Normalize(Viewer - LookAt);
    N = Normalize(LookAt - Viewer);	 // hg 15.09.96

	//  V = Normalize(Up - N * DotProd(Up, N));

    V = Up; /////////////
   
    V -= N * DotProd(V, N);
    V.Normalize();


    U = CrossProd(V, N);
	U.Normalize(); // hg 15.11.96

//	U *= -1.0; // test flip u this would change handedness !

    M.x[0][0] = U.x;
    M.x[1][0] = U.y;
    M.x[2][0] = U.z;
    M.x[0][1] = V.x;
    M.x[1][1] = V.y;
    M.x[2][1] = V.z;
    M.x[0][2] = N.x;
    M.x[1][2] = N.y;
    M.x[2][2] = N.z;

    M.x[3][0] = -DotProd(U, Viewer);
    M.x[3][1] = -DotProd(V, Viewer);
    M.x[3][2] = -DotProd(N, Viewer);

	M.contents = Matrix::HAS_TRANSLATION| Matrix::HAS_ROTATION;

    return M;
}

// Construct a quadric matrix.	After Foley et al. pp. 528-529.
Matrix
QuadricMatrix(float a, float b, float c, float d, float e,
	      float f, float g, float h, float j, float k)
{
    Matrix M;

    M.x[0][0] = a;  M.x[0][1] = d;  M.x[0][2] = f;  M.x[0][3] = g;
    M.x[1][0] = d;  M.x[1][1] = b;  M.x[1][2] = e;  M.x[1][3] = h;
    M.x[2][0] = f;  M.x[2][1] = e;  M.x[2][2] = c;  M.x[2][3] = j;
    M.x[3][0] = g;  M.x[3][1] = h;  M.x[3][2] = j;  M.x[3][3] = k;

	M.contents = Matrix::HAS_ANYTHING;

    return M;
}

// Construct various "mirror" matrices, which flip coordinate
// signs in the various axes specified.
Matrix
MirrorX()
{
    Matrix M; M.Identity();
    M.x[0][0] = -1;
	M.contents = Matrix::HAS_ANY_SCALE;
    return M;
}

Matrix
MirrorY()
{
    Matrix M; M.Identity();
    M.x[1][1] = -1;
	M.contents = Matrix::HAS_ANY_SCALE;
    return M;
}

Matrix
MirrorZ()
{
    Matrix M; M.Identity();
    M.x[2][2] = -1;
	M.contents = Matrix::HAS_ANY_SCALE;
    return M;
}

Matrix
// Zero translation part
RotationOnly(const Matrix& x)
{
    Matrix M = x;
    M.x[3][0] = M.x[3][1] = M.x[3][2] = 0;
	M.contents = x.contents & ~Matrix::HAS_TRANSLATION;
    return M;
}


// Basic Transformation as incremental assignment operators

Matrix& Matrix::Scale(float X, float Y, float Z)
{
	*this *= ScaleMatrix(X,Y,Z);
	return(*this);
}

Matrix& Matrix::Scale(const Point &scale)
{
	*this *= ScaleMatrix(scale);
	return(*this);
}

Matrix& Matrix::Translate(const Point& Location)
{
	*this *= TranslationMatrix(Location);
	return(*this);
}

Matrix& Matrix::Translate(float X, float Y, float Z)
{
	*this *= ::TranslationMatrix(X,Y,Z);
	return(*this);
}


Matrix& Matrix::Rotate(const Point& Rotation)
{
	*this *= RotationMatrix(Rotation);
	return(*this);
}

Matrix& Matrix::Rotate(float X, float Y, float Z)
{
	*this *= RotationMatrix(X,Y,Z);
	return(*this);
}


// Add corresponding elements of the two matrices.
Matrix&
Matrix::operator+= (const Matrix& A)
{
    for (int i = 0; i < 4; i++)
	for (int j = 0; j < 4; j++)
	    x[i][j] += A.x[i][j];

	contents |= A.contents;
    return *this;
}

// Subtract corresponding elements of the matrices.
Matrix&
Matrix::operator-= (const Matrix& A)
{
    for (int i = 0; i < 4; i++)
	for (int j = 0; j < 4; j++)
	    x[i][j] -= A.x[i][j];

	contents |= A.contents;
    return *this;
}

// Scale each element of the matrix by A.
Matrix&
Matrix::operator*= (float A)
{
    for (int i = 0; i < 4; i++)
	for (int j = 0; j < 4; j++)
	    x[i][j] *= A;

	contents |= HAS_ANY_SCALE;
    return *this;
}

#ifdef _KATMAI

#include "gkatmai.h"

typedef float m44[4][4] ;

Matrix&
Matrix::operator*= (const Matrix& A) 
{
	//m44 *b= (m44*) &A.x;
	// MatrixMult_SIMDasm( this->md3d,A.md3d,this->md3d);
	//KatmaiMatrixMult_Int( x,*((float* [4][4])&A.x[0][0]),x );
	// KatmaiMatrixMult_Int( x,*b,x );
	(*KMatrixMult)(x,*((m44*) &A.x),x);
	contents |= A.contents;
    return *this;
}
Matrix& 
Matrix::MultiplyLeft(const Matrix& A)
{
	(*KMatrixMult)(*((m44*) &A.x),x,x);
	contents |= A.contents;
    return *this;
}

Matrix& 
Matrix::MultiplyRight(const Matrix& A)
{
	(*KMatrixMult)(x,*((m44*) &A.x),x);
	contents |= A.contents;
    return *this;
}


Matrix
operator* (const Matrix& A, const Matrix& B)
{
    Matrix ret;
	// MatrixMult_SIMDasm( A.md3d,B.md3d,ret.md3d );
	// KatmaiMatrixMult_Int( *((m44*) &A.x), *((m44*) &B.x),ret.x );

	(*KMatrixMult)(*((m44*) &A.x),*((m44*) &B.x),ret.x);

	ret.contents = A.contents | B.contents;
    return ret;
}

// set to A*B, neither A or B is this
Matrix& Matrix::Multiply(const Matrix& A,const Matrix& B)
{
	(*KMatrixMult)(*((m44*) &A.x),*((m44*) &B.x),x);
	contents = A.contents | B.contents;
    return *this;

}

void Multiply(const Matrix& A, const Matrix& B, Matrix &ret)
{
	(*KMatrixMult)(*((m44*) &A.x),*((m44*) &B.x),ret.x);
	ret.contents = A.contents | B.contents;

}



#else

// Multiply two matrices.
Matrix&
Matrix::operator*= (const Matrix& A)
{
    Matrix ret = *this;

    for (int i = 0; i < 4; i++)
	for (int j = 0; j < 4; j++) {
	    float subt = 0;
	    for (int k = 0; k < 4; k++)
			subt += ret.x[i][k] * A.x[k][j];
	    x[i][j] = subt;
	}
	contents |= A.contents;    
	return *this;
}
Matrix& 
Matrix::MultiplyLeft(const Matrix& A)
{
	Matrix B(*this);
#if 1
	// total unroll 
        x[0][0] = A.x[0][0] * B.x[0][0] + A.x[0][1] * B.x[1][0] + A.x[0][2] * B.x[2][0] + A.x[0][3] * B.x[3][0] ;
        x[0][1] = A.x[0][0] * B.x[0][1] + A.x[0][1] * B.x[1][1] + A.x[0][2] * B.x[2][1] + A.x[0][3] * B.x[3][1] ;
        x[0][2] = A.x[0][0] * B.x[0][2] + A.x[0][1] * B.x[1][2] + A.x[0][2] * B.x[2][2] + A.x[0][3] * B.x[3][2] ;
        x[0][3] = A.x[0][0] * B.x[0][3] + A.x[0][1] * B.x[1][3] + A.x[0][2] * B.x[2][3] + A.x[0][3] * B.x[3][3] ;

        x[1][0] = A.x[1][0] * B.x[0][0] + A.x[1][1] * B.x[1][0] + A.x[1][2] * B.x[2][0] + A.x[1][3] * B.x[3][0] ;
        x[1][1] = A.x[1][0] * B.x[0][1] + A.x[1][1] * B.x[1][1] + A.x[1][2] * B.x[2][1] + A.x[1][3] * B.x[3][1] ;
        x[1][2] = A.x[1][0] * B.x[0][2] + A.x[1][1] * B.x[1][2] + A.x[1][2] * B.x[2][2] + A.x[1][3] * B.x[3][2] ;
        x[1][3] = A.x[1][0] * B.x[0][3] + A.x[1][1] * B.x[1][3] + A.x[1][2] * B.x[2][3] + A.x[1][3] * B.x[3][3] ;

        x[2][0] = A.x[2][0] * B.x[0][0] + A.x[2][1] * B.x[1][0] + A.x[2][2] * B.x[2][0] + A.x[2][3] * B.x[3][0] ;
        x[2][1] = A.x[2][0] * B.x[0][1] + A.x[2][1] * B.x[1][1] + A.x[2][2] * B.x[2][1] + A.x[2][3] * B.x[3][1] ;
        x[2][2] = A.x[2][0] * B.x[0][2] + A.x[2][1] * B.x[1][2] + A.x[2][2] * B.x[2][2] + A.x[2][3] * B.x[3][2] ;
        x[2][3] = A.x[2][0] * B.x[0][3] + A.x[2][1] * B.x[1][3] + A.x[2][2] * B.x[2][3] + A.x[2][3] * B.x[3][3] ;

        x[3][0] = A.x[3][0] * B.x[0][0] + A.x[3][1] * B.x[1][0] + A.x[3][2] * B.x[2][0] + A.x[3][3] * B.x[3][0] ;
        x[3][1] = A.x[3][0] * B.x[0][1] + A.x[3][1] * B.x[1][1] + A.x[3][2] * B.x[2][1] + A.x[3][3] * B.x[3][1] ;
        x[3][2] = A.x[3][0] * B.x[0][2] + A.x[3][1] * B.x[1][2] + A.x[3][2] * B.x[2][2] + A.x[3][3] * B.x[3][2] ;
        x[3][3] = A.x[3][0] * B.x[0][3] + A.x[3][1] * B.x[1][3] + A.x[3][2] * B.x[2][3] + A.x[3][3] * B.x[3][3] ;

#else


    for (int i = 0; i < 4; i++)	{
        x[i][0] = A.x[i][0] * B.x[0][0] + A.x[i][1] * B.x[1][0] + A.x[i][2] * B.x[2][0] + A.x[i][3] * B.x[3][0] ;
        x[i][1] = A.x[i][0] * B.x[0][1] + A.x[i][1] * B.x[1][1] + A.x[i][2] * B.x[2][1] + A.x[i][3] * B.x[3][1] ;
        x[i][2] = A.x[i][0] * B.x[0][2] + A.x[i][1] * B.x[1][2] + A.x[i][2] * B.x[2][2] + A.x[i][3] * B.x[3][2] ;
        x[i][3] = A.x[i][0] * B.x[0][3] + A.x[i][1] * B.x[1][3] + A.x[i][2] * B.x[2][3] + A.x[i][3] * B.x[3][3] ;
	}
#endif
	contents |= A.contents;
    return *this;
}

Matrix& 
Matrix::MultiplyRight(const Matrix& B)
{
	Matrix A(*this);
#if 1
	// total unroll 
        x[0][0] = A.x[0][0] * B.x[0][0] + A.x[0][1] * B.x[1][0] + A.x[0][2] * B.x[2][0] + A.x[0][3] * B.x[3][0] ;
        x[0][1] = A.x[0][0] * B.x[0][1] + A.x[0][1] * B.x[1][1] + A.x[0][2] * B.x[2][1] + A.x[0][3] * B.x[3][1] ;
        x[0][2] = A.x[0][0] * B.x[0][2] + A.x[0][1] * B.x[1][2] + A.x[0][2] * B.x[2][2] + A.x[0][3] * B.x[3][2] ;
        x[0][3] = A.x[0][0] * B.x[0][3] + A.x[0][1] * B.x[1][3] + A.x[0][2] * B.x[2][3] + A.x[0][3] * B.x[3][3] ;

        x[1][0] = A.x[1][0] * B.x[0][0] + A.x[1][1] * B.x[1][0] + A.x[1][2] * B.x[2][0] + A.x[1][3] * B.x[3][0] ;
        x[1][1] = A.x[1][0] * B.x[0][1] + A.x[1][1] * B.x[1][1] + A.x[1][2] * B.x[2][1] + A.x[1][3] * B.x[3][1] ;
        x[1][2] = A.x[1][0] * B.x[0][2] + A.x[1][1] * B.x[1][2] + A.x[1][2] * B.x[2][2] + A.x[1][3] * B.x[3][2] ;
        x[1][3] = A.x[1][0] * B.x[0][3] + A.x[1][1] * B.x[1][3] + A.x[1][2] * B.x[2][3] + A.x[1][3] * B.x[3][3] ;

        x[2][0] = A.x[2][0] * B.x[0][0] + A.x[2][1] * B.x[1][0] + A.x[2][2] * B.x[2][0] + A.x[2][3] * B.x[3][0] ;
        x[2][1] = A.x[2][0] * B.x[0][1] + A.x[2][1] * B.x[1][1] + A.x[2][2] * B.x[2][1] + A.x[2][3] * B.x[3][1] ;
        x[2][2] = A.x[2][0] * B.x[0][2] + A.x[2][1] * B.x[1][2] + A.x[2][2] * B.x[2][2] + A.x[2][3] * B.x[3][2] ;
        x[2][3] = A.x[2][0] * B.x[0][3] + A.x[2][1] * B.x[1][3] + A.x[2][2] * B.x[2][3] + A.x[2][3] * B.x[3][3] ;

        x[3][0] = A.x[3][0] * B.x[0][0] + A.x[3][1] * B.x[1][0] + A.x[3][2] * B.x[2][0] + A.x[3][3] * B.x[3][0] ;
        x[3][1] = A.x[3][0] * B.x[0][1] + A.x[3][1] * B.x[1][1] + A.x[3][2] * B.x[2][1] + A.x[3][3] * B.x[3][1] ;
        x[3][2] = A.x[3][0] * B.x[0][2] + A.x[3][1] * B.x[1][2] + A.x[3][2] * B.x[2][2] + A.x[3][3] * B.x[3][2] ;
        x[3][3] = A.x[3][0] * B.x[0][3] + A.x[3][1] * B.x[1][3] + A.x[3][2] * B.x[2][3] + A.x[3][3] * B.x[3][3] ;

#else


    for (int i = 0; i < 4; i++)	{
        x[i][0] = A.x[i][0] * B.x[0][0] + A.x[i][1] * B.x[1][0] + A.x[i][2] * B.x[2][0] + A.x[i][3] * B.x[3][0] ;
        x[i][1] = A.x[i][0] * B.x[0][1] + A.x[i][1] * B.x[1][1] + A.x[i][2] * B.x[2][1] + A.x[i][3] * B.x[3][1] ;
        x[i][2] = A.x[i][0] * B.x[0][2] + A.x[i][1] * B.x[1][2] + A.x[i][2] * B.x[2][2] + A.x[i][3] * B.x[3][2] ;
        x[i][3] = A.x[i][0] * B.x[0][3] + A.x[i][1] * B.x[1][3] + A.x[i][2] * B.x[2][3] + A.x[i][3] * B.x[3][3] ;
	}
#endif
	contents |= A.contents;
    return *this;
}


Matrix
operator* (const Matrix& A, const Matrix& B)
{
    Matrix ret;
#if 1
    for (int i = 0; i < 4; i++)	{
        ret.x[i][0] = A.x[i][0] * B.x[0][0] + A.x[i][1] * B.x[1][0] + A.x[i][2] * B.x[2][0] + A.x[i][3] * B.x[3][0] ;
        ret.x[i][1] = A.x[i][0] * B.x[0][1] + A.x[i][1] * B.x[1][1] + A.x[i][2] * B.x[2][1] + A.x[i][3] * B.x[3][1] ;
        ret.x[i][2] = A.x[i][0] * B.x[0][2] + A.x[i][1] * B.x[1][2] + A.x[i][2] * B.x[2][2] + A.x[i][3] * B.x[3][2] ;
        ret.x[i][3] = A.x[i][0] * B.x[0][3] + A.x[i][1] * B.x[1][3] + A.x[i][2] * B.x[2][3] + A.x[i][3] * B.x[3][3] ;
	}
#else 

    for (int i = 0; i < 4; i++)
	for (int j = 0; j < 4; j++) {
	    //float subt = 0;
	    //for (int k = 0; k < 4; k++)   subt += A.x[i][k] * B.x[k][j];
	    //ret.x[i][j] = subt;

        ret.x[i][j] = A.x[i][0] * B.x[0][j] + A.x[i][1] * B.x[1][j] + A.x[i][2] * B.x[2][j] + A.x[i][3] * B.x[3][j] ;
	}
#endif

	ret.contents = A.contents | B.contents;
    return ret;
}


// set to A*B, neither A or B is this
Matrix& Matrix::Multiply(const Matrix& A,const Matrix& B)
{
    for (int i = 0; i < 4; i++)	{
        x[i][0] = A.x[i][0] * B.x[0][0] + A.x[i][1] * B.x[1][0] + A.x[i][2] * B.x[2][0] + A.x[i][3] * B.x[3][0] ;
        x[i][1] = A.x[i][0] * B.x[0][1] + A.x[i][1] * B.x[1][1] + A.x[i][2] * B.x[2][1] + A.x[i][3] * B.x[3][1] ;
        x[i][2] = A.x[i][0] * B.x[0][2] + A.x[i][1] * B.x[1][2] + A.x[i][2] * B.x[2][2] + A.x[i][3] * B.x[3][2] ;
        x[i][3] = A.x[i][0] * B.x[0][3] + A.x[i][1] * B.x[1][3] + A.x[i][2] * B.x[2][3] + A.x[i][3] * B.x[3][3] ;
	}
	contents = A.contents | B.contents;
    return *this;
}

void Multiply(const Matrix& A, const Matrix& B, Matrix &ret)
{
    for (int i = 0; i < 4; i++)	{
        ret.x[i][0] = A.x[i][0] * B.x[0][0] + A.x[i][1] * B.x[1][0] + A.x[i][2] * B.x[2][0] + A.x[i][3] * B.x[3][0] ;
        ret.x[i][1] = A.x[i][0] * B.x[0][1] + A.x[i][1] * B.x[1][1] + A.x[i][2] * B.x[2][1] + A.x[i][3] * B.x[3][1] ;
        ret.x[i][2] = A.x[i][0] * B.x[0][2] + A.x[i][1] * B.x[1][2] + A.x[i][2] * B.x[2][2] + A.x[i][3] * B.x[3][2] ;
        ret.x[i][3] = A.x[i][0] * B.x[0][3] + A.x[i][1] * B.x[1][3] + A.x[i][2] * B.x[2][3] + A.x[i][3] * B.x[3][3] ;
	}
	ret.contents = A.contents | B.contents;
}


#endif

Matrix& 
Matrix::ScaleByLeft(const Point& scale)
{
        x[0][0] = scale.x * x[0][0];
        x[0][1] = scale.x * x[0][1]; 
        x[0][2] = scale.x * x[0][2];
        x[0][3] = scale.x * x[0][3];

        x[1][0] = scale.y * x[1][0];
        x[1][1] = scale.y * x[1][1];
        x[1][2] = scale.y * x[1][2];
        x[1][3] = scale.y * x[1][3];

        x[2][0] =  scale.z * x[2][0];
        x[2][1] =  scale.z * x[2][1];
        x[2][2] =  scale.z * x[2][2];
        x[2][3] =  scale.z * x[2][3];
	
	if ((scale.x <=0) || (scale.y <=0) || (scale.z <=0))
		contents |= HAS_NEGATIVE_SCALE | HAS_SCALE;

	if ((scale.x != scale.y) || (scale.y != scale.z))
		contents |= HAS_NON_UNIFORM_SCALE | HAS_SCALE;
	else if (scale.x == 1.0) {
		// no scale 
		ASSERT(scale.y == 1.0);
		ASSERT(scale.z == 1.0);
	}
	else contents |= HAS_SCALE;

	return *this;

}

// Add corresponding elements of the matrices.
Matrix
operator+ (const Matrix& A, const Matrix& B)
{
    Matrix ret;

    for (int i = 0; i < 4; i++)
	for (int j = 0; j < 4; j++)
	    ret.x[i][j] = A.x[i][j] + B.x[i][j];

	ret.contents = Matrix::HAS_ANYTHING;

    return ret;
}

// Subtract corresponding elements of the matrices.
Matrix
operator- (const Matrix& A, const Matrix& B)
{
    Matrix ret;

    for (int i = 0; i < 4; i++)
	for (int j = 0; j < 4; j++)
	    ret.x[i][j] = A.x[i][j] - B.x[i][j];

	ret.contents = Matrix::HAS_ANYTHING;
    return ret;
}



// Transform a point by a matrix.
Point
operator* (const Matrix& M, const Point& v)
{
    Point ret;
    float denom;

    ret.x = v.x * M.x[0][0] + v.y * M.x[1][0] + v.z * M.x[2][0] + M.x[3][0];
    ret.y = v.x * M.x[0][1] + v.y * M.x[1][1] + v.z * M.x[2][1] + M.x[3][1];
    ret.z = v.x * M.x[0][2] + v.y * M.x[1][2] + v.z * M.x[2][2] + M.x[3][2];
    denom = v.x * M.x[0][3] + v.y * M.x[1][3] + v.z * M.x[2][3] + M.x[3][3];

	//ASSERT(denom != 0.0f);
	if (denom == 0.0f) 
		return ret;

    if (denom != 1.0)
		ret *= (1.0f / denom);

    return ret;
}

// Apply the rotation portion of a matrix to a vector.
Point
RotateOnly(const Matrix& M, const Point& v)
{
    Point ret;
    float denom;

    ret.x = v.x * M.x[0][0] + v.y * M.x[1][0] + v.z * M.x[2][0];
    ret.y = v.x * M.x[0][1] + v.y * M.x[1][1] + v.z * M.x[2][1];
    ret.z = v.x * M.x[0][2] + v.y * M.x[1][2] + v.z * M.x[2][2];
    denom = v.x * M.x[0][3] + v.y * M.x[1][3] + v.z * M.x[2][3] +  M.x[3][3];

	// ASSERT(denom != 0.0f);
	if (denom == 0.0f) 
		return ret;

    if (denom != 1.0)
		ret *= (1.0f / denom);

    return ret;
}

// Transform a Plane by a matrix.
Plane
operator* (const Matrix& M, const Plane& v)
{
    Plane ret;
	
    ret.n.x = v.n.x * M.x[0][0] + v.n.y * M.x[1][0] + v.n.z * M.x[2][0] + v.d * M.x[3][0];
    ret.n.y = v.n.x * M.x[0][1] + v.n.y * M.x[1][1] + v.n.z * M.x[2][1] + v.d * M.x[3][1];
    ret.n.z = v.n.x * M.x[0][2] + v.n.y * M.x[1][2] + v.n.z * M.x[2][2] + v.d * M.x[3][2];
    ret.d   = v.n.x * M.x[0][3] + v.n.y * M.x[1][3] + v.n.z * M.x[2][3] + v.d * M.x[3][3];

    return ret;
}


// get a scale factor to transform values 
float Matrix::ScaleFactor() const  {
	Point p(1,1,1);
	p = RotateOnly(*this,p);
	return (p.Length() / sqrt(3.0f));
}


#if 0
Vector 
operator* (const Matrix& M, const Vector &v)
{
  return(RotateOnly(M,v));
}
#endif

// Scale each element of the matrix by B.
Matrix
operator* (const Matrix& A, float B)
{
    Matrix ret;

    for (int i = 0; i < 4; i++)
	for (int j = 0; j < 4; j++)
	    ret.x[i][j] = A.x[i][j] * B;

    return ret;
}

// input output using astream
int Matrix::io(AStream &a)
{
  a.Sep('(');
    for (int i = 0; i < 4; i++)
	{
	  a.Sep('(');
		for (int j = 0; j < 4; j++) a.io(x[i][j]);
      a.Sep(')');
	}
  a.Sep(')');
  return a.stat;
}


// From hquat.c

/*
  unmatrix : Spencer W. Thomas, University of Michigan
  Graphic Gems II, Page 600 + 320
*/
#define XY x
#define XZ y
#define YZ z

int Matrix::Decompose(Point &scale,Point &shear, Point &rotate, Point &translate) const
{ register int i;
  Matrix locmat(*this);
  Point row[3];
  Point cross12;

  /* assume
     det of upper 3*3 matrix non zero
     no perspective part
  */
  /* normalize */

  translate.x = locmat.x[3][0]; locmat.x[3][0]= 0;
  translate.y = locmat.x[3][1]; locmat.x[3][1]= 0;
  translate.z = locmat.x[3][2]; locmat.x[3][2]= 0;

  /* now get scale and shear */
  for(i=0; i< 3; i++) { row[i].Set(locmat.x[i][0],locmat.x[i][1],locmat.x[i][2]); }

  /* Compute x scale factor and normalize first row */
  scale.x = row[0].Len();
  row[0].Normalize();

  shear.XY = DotProd(row[0],row[1]);
  
  row[1]+=row[0] * -shear.XY;

  /* Compute y scale factor and normalize second row */
  scale.y = row[1].Len();
  row[1].Normalize();

  if (scale.y != 0.0) shear.XY /= scale.y;

  /* compute xz and yz shears, orthogonalize 3rd row */
  shear.XZ = DotProd(row[0],row[2]);
  row[2]+=row[0]* -shear.XZ;

  shear.YZ = DotProd(row[1],row[2]);
  row[2]+=row[1]*-shear.YZ;

  scale.z = row[2].Len();
  row[2].Normalize();

  if (scale.z != 0.0) shear.XZ /= scale.z;
  if (scale.z != 0.0) shear.YZ /= scale.z;

  /* at this point the matrix in rows is orthonormal.
     Check for a coordinate system flip. if the determinat is -1
     then negate the matrix and the scaling factors.
  */
  cross12 = CrossProd(row[1],row[2]);

  if (DotProd(row[0],cross12) < 0)
     for(i=0; i <3;i++) {
        row[i]*= -1.0;
        scale[i] *= -1.0;
     }

  /* rotations */
  rotate.y = asin(-row[0].z);

  if (cos(rotate.y) != 0) {
        rotate.x = atan2(row[1].z,row[2].z);
        rotate.z = atan2(row[0].y,row[0].x);
  } else {
        rotate.x = atan2(row[1].x,row[1].y);
        rotate.z = 0;
  }
//  rotate.x = RAD2DEG(rotate.x);
//  rotate.y = RAD2DEG(rotate.y);
//  rotate.z = RAD2DEG(rotate.z);

#if 0
  if (1) {
      printf("\nScale "); VECPRINT(*scale);
      printf("\nShear "); VECPRINT(*shear);
      printf("\nRotate "); VECPRINT(*rotate);
      printf("\nTrans  "); VECPRINT(*translate);
  }
#endif

  return(1);
}

#if 0
int compute_shear_matrix(char *what,double amount,tmatrix mat)
{
   HC_Compute_Identity_Matrix(mat);
   if (streq(what,"xy"))
      mat [1][0] =  amount;
   else if (streq(what,"xz"))
      mat [2][0] =  amount;
   else if (streq(what,"yz"))
      mat [2][1] =  amount;
   return (1);
}

 #endif

int Matrix::Compose(
        const Point &scale, const Point &shear, 
        const Point &rotate,const Point &translate)
{ *this=ScaleMatrix(scale);
  *this*=RotationMatrix(rotate);
  *this*=TranslationMatrix(translate);
  return(1);
}


// Overloaded << for C++-style output.
/*
ostream&
operator<< (ostream& s, const Matrix& M)
{
    for (int i = 0; i < 4; i++) {
	for (int j = 0; j < 4; j++)
	    s << setprecision(2) << setw(10) << M.x[i][j];
	s << '\n';
    }
    return s;
}
*/

// Rotate a direction vector...
Point
PlaneRotate(const Matrix& tform, const Point& p)
{
    // I sure hope that matrix is invertible...
    Matrix use = Transpose(Invert(tform));

    return RotateOnly(use, p);
}

#if 0

// Transform
// don't know who will destroy N ?????

PointArray& operator* (const Matrix& M, const PointArray& A)
{ PointArray N(A.length());

  register const Point *src;
  register Point *dest;
  /* src = A.p;
  dest = N.p; */
  src = A.data();
  dest = N.data();
  register int cnt = A.length();
  while (cnt --) {
     *dest++ = M * *src++;
  }
  return N;
}

#if 0
PointArray& PointArray::operator*= (const Matrix& M)
{ register Point *src;
  src = p;
  register int cnt = np;
  while (cnt --) {
     *src= M * *src;
     src++;
  }
  return(*this);
}
#endif
#endif


PointArray& PointArray::Mult(const Matrix& M)
{ register Point *src;
  src = p;
  register int cnt = np;
  while (cnt --) {
     *src= M * *src;
     src++;
  }
  return(*this);
}


