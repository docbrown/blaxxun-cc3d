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
#ifndef _Quaternation_h_
#define _Quaternation_h_


class Matrix;
//
// Quaternation
//


class Quaternation  {                        /* Quaternation type */
public:
   	double x, y, z, w;

	operator double *() { return &x; }  // assume above fields are linear in memory

 	/* tolerance for quaternion operations	*/
    #define	Q_EPSILON   (1e-10)

   	void Set(double _x, double _y, double _z, double _w) { x=_x,y=_y,z=_z,w=_w; }
	
	// rotate given axis and angle 
	void SetAxisRotation(double ax,double ay, double az, double phi);
	void GetAxisRotation(Vector &axis, double &angle) const;
	
	void SetAxisRotation(const Vector &axis, double angle) { SetAxisRotation(axis.x,axis.y,axis.z,angle); }
	void SetAxisRotationCos(const Vector &axis, double cos_angle);
	
    void GetAxisRotation(float &ax, float  &ay, float &az, float &phi) const;

	void SetFromEuler(double yaw, double pitch, double roll);

	// create a quaternion from two vectors that rotates v1 to v2 about an axis perpendicular to both
	void Set(const Vector &v1, const Vector &v2);

	// set by a trackball rotation 
	BOOL SetTrackball(float p1x, float p1y, float p2x, float p2y,float trackballSize=0.8);

    // Spherical linear interpolation of unit quaternions.

	

	void InterpolateSpin(const Quaternation &a, const Quaternation &b,
					 double alpha, int spin=0) { InterpolateSphereSpin(alpha,a,b,*this,spin); }

	void Interpolate(const Quaternation &a, const Quaternation &b,
					 double alpha) 
	{ InterpolateSphereSpin(alpha,a,b,*this,0); }
	//	{ InterpolateSphere(a,b,alpha,*this); }

	void GetMatrix(Matrix &m) const { QuaternationToMatrix(*this, m); }
	void SetMatrix(const Matrix &m) { QuaternationFromMatrix(*this, m); }



	friend void InterpolateSphereSpin(double alpha, const Quaternation &a,
                        const Quaternation &b, Quaternation &q, int spin);
	friend void 
	InterpolateSphere(const Quaternation &startQuat, const Quaternation &endQuat, double t,Quaternation &destQuat);

	friend void QuaternationToMatrix(const Quaternation &q, Matrix &mat);
	friend void QuaternationFromMatrix(Quaternation &destQuat, const Matrix & srcMatrix);


	friend Quaternation RandomQuaternation(double x[3]);

	// normalize quaternion.
	friend Quaternation  Normalize( const Quaternation &srcQuat);
	friend void Mult(Quaternation &destQuat, const Quaternation &qLeft, const Quaternation &qRight);
	friend Quaternation operator* (const Quaternation& A, const Quaternation& B);


	friend Quaternation Conjugate(const Quaternation &srcQuat);
	friend Quaternation Invert(const Quaternation &srcQuat);
	friend Quaternation Exp(const Quaternation &srcQuat);
	friend Quaternation Log(const Quaternation  &srcQuat);
	friend Quaternation RandomQuaternation(long *argseed);

	/*  Generate uniform random unit Quaternation from uniform deviates.
	*  Each x[i] should vary between 0 and 1.
	*/
	Quaternation RandomQuaternation(double x[3]);


};

void RandomRotation(long *seed, Matrix &m);
void RandomRotation(double x[3], Matrix &m);


class QuaternationWithSpin : public Quaternation  {     

public:

	int spin;

	void SetAxisRotation(double ax,double ay, double az, double phi);

	void Interpolate(const Quaternation &a, const Quaternation &b,
					 double alpha) { InterpolateSphereSpin(alpha,a,b,*this,spin); }

};


#endif
