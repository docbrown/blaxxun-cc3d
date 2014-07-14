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

#include "point.h"
#include "matrix.h"
#include "gutils.h"

#include "quaternation.h"


/*
 *      Given an axis and angle, compute Quaternation.
 */
void Quaternation::SetAxisRotation(double ax,double ay, double az, double phi)
{ double alen,f;

        // normalize vector
        alen = sqrt(sqr(ax)+sqr(ay)+sqr(az));
		
		/* if zero vector passed in, just return identity quaternion	*/
		
		if ( alen < Q_EPSILON )
    		{
    		x = 0;
    		y = 0;
    		z = 0;
    		// w = 1;
	        w = cos(0.5*phi); // hg 20.10.98

    		return;
    	}

        f = sin(0.5*phi) / alen;
        x = ax*f;
        y = ay*f;
        z = az*f;
        w = cos(0.5*phi);
}


void Quaternation::SetAxisRotationCos(const Vector &axis, double cos_angle) 
{ 
	// must clamp cos_angle to -1 ..1, otherwise acos returns NAN !!!!
	if (cos_angle < -1.0) cos_angle= -1.0;
	else if (cos_angle > 1.0) cos_angle= 1.0;
	SetAxisRotation(axis.x,axis.y,axis.z,acos(cos_angle)); 
}


#if 0
void
Quaternation::GetAxisRotation(Vector &axis, double &angle)
{
	double	half_angle;
	double	sin_half_angle;

	half_angle = acos(w);
	sin_half_angle = sin(half_angle);
	angle = half_angle * 2;

	if ( sin_half_angle < 1e-8 && sin_half_angle > -1e-8 )
		axis.Set(0, 0, 0);
	else
	{
		sin_half_angle = 1.0 / sin_half_angle;
		axis.Set(x * sin_half_angle, y * sin_half_angle, z * sin_half_angle);
	}
}
#endif


// sqr(x)+sqr(y) + sqr(z) + sqr(w) == 1
// w = cos(w*0.5), x = vx * sin(w*0.5), y = vy * sin(w*0.5), z = vz * sin(w*0.5)
void
Quaternation::GetAxisRotation(Vector &axis, double &angle) const 
{
#if 1
	// vecmat 1.1.2
	double sin_a2 = sqrt(x*x + y*y + z*z);  // |sin a/2|, w = cos a/2

	angle = (2.0*atan2(sin_a2, w)); // 0 <= angle <= PI , because 0 < sin_a2

	if (sin_a2 >= Q_EPSILON) {
		sin_a2 = 1.0/sin_a2;

		axis.Set(x*sin_a2,y*sin_a2,z*sin_a2);
	} else {
        axis.Set(0,0,1);
	}
	

#if 0
    double t = sqrt( (1.0-sqr(w)) / (sqr(x) + sqr(y) + sqr(z)));

    if (t > 1e-8) {
        axis.Set(x/t,y/t,z/t);
        axis.Normalize();
        angle = 2.0 * atan2(t,w); // wrong
    } else {
        axis.Set(0,0,1);
        angle = 0;
    }
#endif

#else
	 // was before 
	  axis.Set(x,y,z);
      axis.Normalize();
      angle = 2.0*acos(w);
#endif


}
void Quaternation::GetAxisRotation(float &ax, float  &ay, float &az, float &angle) const
{
#if 0
    double t = sqrt( (1.0-sqr(w)) / (sqr(x) + sqr(y) + sqr(z)));
    if (t > 1e-8) {
        ax = x/t,ay = y/t,az= z/t;
        // normalize vector
        double alen = sqrt(sqr(ax)+sqr(ay)+sqr(az));
        alen = 1.0/alen;
        ax*=alen,ay*=alen,az*=alen;
        angle = 2.0 * atan2(t,w); // somw how wrong
    } else {
        ax=ay=0,az=1;
        angle = 0;
    }
#endif

#if 0
      ax = x,ay = y,az= z;
      // normalize vector
      double alen = sqrt(sqr(x)+sqr(y)+sqr(z));
      if (alen > 1E-8) {
     
        alen = 1.0/alen;
        ax = x*alen,ay = y* alen,az=z*alen;
      }
      angle = 2.0*acos(w);
#endif 

	// 5.5.98
	float normalizeFactor = 1.0 / sqrt( x*x + y*y + 
      	    	    	      z*z + w*w );
/*
	if (normalizeFactor >0) {
		x*=normalizeFactor;
		y*=normalizeFactor;
		z*=normalizeFactor;
		w*=normalizeFactor;
	} 
*/
	// vecmat 1.1.2
	double sin_a2 = sqrt(x*x + y*y + z*z);  // |sin a/2|, w = cos a/2

	angle = (2.0*atan2(sin_a2, w)); // 0 <= angle <= PI , because 0 < sin_a2

	if (sin_a2 >= Q_EPSILON) {
		sin_a2 = 1.0/sin_a2;

		ax=x*sin_a2,ay=y*sin_a2,az=z*sin_a2;
	} else {
        ax=0,ay=0,az=1;
	}
//	TRACE("Angle 1 %g Angle 2 %g  axis %g %g %g \n",angle,2.0*acos(w),ax,ay,az);


}




void QuaternationWithSpin::SetAxisRotation(double ax,double ay, double az, double phi)
{ double alen,f;

        alen = sqrt(sqr(ax)+sqr(ay)+sqr(az));
        f = sin(phi/2.0) / alen;
        x = ax*f;
        y = ay*f;
        z = az*f;
        w = cos(phi/2.0);
		spin = (int) (phi / TWOPI);
}


#if 0
        double alpha;                   /* interpolation parameter (0 to 1) */
        Quaternation *a, *b;            /* start and end unit Quaternations */
        Quaternation *q;                        /* output interpolated Quaternation */
        int spin;                       /* number of extra spin rotations */
#endif

// float EPS from GUTILS.H
#define EPSILON EPS

/* Graphic Gems III */
void InterpolateSphereSpin(double alpha, const Quaternation &a,
                        const Quaternation &b, Quaternation &q, int spin)

{
        double beta;                    /* complementary interp parameter */
        double theta;                   /* angle between A and B */
        double sin_t, cos_t;            /* sine, cosine of theta */
        double phi;                     /* theta plus spins */
        int bflip;                      /* use negation of B? */

        /* cosine theta = dot product of A and B */
        cos_t = a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;

        /* if B is on opposite hemisphere from A, use -B instead */
        if (cos_t < 0.0) {
                cos_t = -cos_t;
                bflip = TRUE;
        } else
                bflip = FALSE;

        /* if B is (within precision limits) the same as A,
         * just linear interpolate between A and B.
         * Can't do spins, since we don't know what direction to spin.
         */
        if (1.0 - cos_t < EPSILON) {
                beta = 1.0 - alpha;
        } else {                                /* normal case */
                theta = acos(cos_t);
                phi = theta + spin * PI;
                sin_t = sin(theta);
                beta = sin(theta - alpha*phi) / sin_t;
                alpha = sin(alpha*phi) / sin_t;

 
        }

        if (bflip)
                alpha = -alpha;

        /* interpolate */
        q.x = beta*a.x + alpha*b.x;
        q.y = beta*a.y + alpha*b.y;
        q.z = beta*a.z + alpha*b.z;
        q.w = beta*a.w + alpha*b.w;
}


/* see also Watt, Advanced Animation and rendering techniques Pg. 362 */
/* urot.c */
/* Generates a uniform random rotation */
/* Ken Shoemake, September 1991 */

/* typedef struct {float x, y, z, w;} Quat; */
enum QuatPart {X, Y, Z, W, QuatLen, V=0};

/* * * * * *  Utility for Quaternation conversion * * * * * */

/** QuaternationToMatrix
 *  Construct rotation matrix from Quaternation (unit or not).
 *  Assumes matrix is used to multiply row vector on the right:
 *  vnew = vold mat.  Works correctly for right-handed coordinate system
 *  and right-handed rotations. For column vectors or for left-handed
 *  coordinate systems, transpose the matrix.
 */
void QuaternationToMatrix(const Quaternation &q, Matrix &mat)
{
        double norm = q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w;
        double s = (norm > 0.0) ? 2.0/norm : 0.0;
        double xs = q.x*s,              ys = q.y*s,             zs = q.z*s;
        double wx = q.w*xs,     wy = q.w*ys,    wz = q.w*zs,
               xx = q.x*xs,    xy = q.x*ys,    xz = q.x*zs,
               yy = q.y*ys,    yz = q.y*zs,    zz = q.z*zs;

        mat[X][X] = 1.0 - (yy + zz); mat[X][Y] = xy + wz; mat[X][Z] = xz - wy;
        mat[Y][X] = xy - wz; mat[Y][Y] = 1.0 - (xx + zz); mat[Y][Z] = yz + wx;
        mat[Z][X] = xz + wy; mat[Z][Y] = yz - wx; mat[Z][Z] = 1.0 - (xx + yy);
		mat[0][3] = mat[1][3]= mat[2][3] = 0.0; mat[3][3] = 1.0; 
		mat[3][0] = mat[3][1]= mat[3][2] = 0.0; 
} /* QuarternationToMatrix */

/*****************************************************************************
 * 
   QuaternationFromMatrix: Convert 4x4 row-major rotation matrix to unit quaternion
 *
 *****************************************************************************/

void 
QuaternationFromMatrix(Quaternation &destQuat, const Matrix & srcMatrix)
{
    double  	trace, s;
    int     	i, j, k;
    static int 	next[3] = {Y, Z, X};


	trace = srcMatrix[X][X] + srcMatrix[Y][Y]+ srcMatrix[Z][Z];

	if (trace > 0.0)
    {
		s = sqrt(trace + 1.0);
		destQuat.w = s * 0.5;
		s = 0.5 / s;
    
		destQuat.x = (srcMatrix[Y][Z] - srcMatrix[Z][Y]) * s;
		destQuat.y = (srcMatrix[Z][X] - srcMatrix[X][Z]) * s;
		destQuat.z = (srcMatrix[X][Y] - srcMatrix[Y][X]) * s;
    } 

	else 
    {
		i = X;
		if (srcMatrix[Y][Y] > srcMatrix[X][X])
			i = Y;
		if (srcMatrix[Z][Z] > srcMatrix[i][i])
			i = Z;
		
		j = next[i];  
		k = next[j];
    
		s = sqrt( (srcMatrix[i][i] - (srcMatrix[j][j]+srcMatrix[k][k])) + 1.0 );
		destQuat[i] = s * 0.5;
    
		s = 0.5 / s;
    
		destQuat.w = (srcMatrix[j][k] - srcMatrix[k][j]) * s;
		destQuat[j] = (srcMatrix[i][j] + srcMatrix[j][i]) * s;
		destQuat[k] = (srcMatrix[i][k] + srcMatrix[k][i]) * s;
    }

}   /* QuaternationFromMatrix */



/* * * * * *  How to do it using gaussians * * * * * */

/** Qt_RandomG
 *  Generate uniform random unit Quaternation from random seed.
 */
Quaternation RandomQuaternation(long *argseed)
{
/*  This algorithm generates a gaussian deviate for each coordinate, so
 *  the total effect is to generate a symmetric 4-D gaussian distribution,
 *  by separability. Projecting onto the surface of the hypersphere gives
 *  a uniform distribution.
 */
        Quaternation q;
        /* uurand generates doubles uniformly distributed between Ð1 and +1 */
        /* This linear congruential generator is inline to exploit signed ints */
        register long seed = *argseed;
		#define uurand() ((seed = (seed+1)*69069)/2147483648.0)
        register double x = uurand(), y = uurand();
        register double z = uurand(), w = uurand();
        register double s1, s2;
        double num1, num2, root1, root2, r;
        while ((s1 = x*x+y*y) > 1.0) {x = uurand(); y = uurand();}
        while ((s2 = z*z+w*w) > 1.0) {z = uurand(); w = uurand();}
        /* Now the point (x,y) is uniformly distributed in the unit disk */
        /* So is the point (z,w), independently */
        num1 = -2*log(s1); num2 = -2*log(s2);
        /* Now x*sqrt(num1/s1) is gaussian distributed, using polar method */
        /* Similarly for y, z, and w, and all are independent */
        r = num1 + num2;        /* Sum of squares of four gaussians */
        root1 = sqrt((num1/s1)/r); root2 = sqrt((num2/s2)/r);
        /* Normalizing onto unit sphere gives uniform unit Quaternation */
        q.x = x*root1; q.y = y*root1; q.z = z*root2; q.w = w*root2;
        *argseed = seed;
#undef uurand
        return (q);
} /* Qt_RandomG */

/** M3_RandomRotG
 *  Generate uniform random rotation matrix from random seed.
 */
void RandomRotation(long *seed, Matrix &m)
{
        QuaternationToMatrix(RandomQuaternation(seed), m);
} /* M3_RandomRotG */


/* * * * * *  How to do it using subgroup algorithm * * * * * */

/** Qt_Random
 *  Generate uniform random unit Quaternation from uniform deviates.
 *  Each x[i] should vary between 0 and 1.
 */
Quaternation RandomQuaternation(double x[3])
{
/*  The subgroup algorithm can be condensed to this efficient form.
 *  Use rotations around z as a subgroup, with coset representatives
 *  the rotations pointing the z axis in different directions.
 */

        Quaternation q;
        register double r1 = sqrt(1.0 - x[0]), r2 = sqrt(x[0]);
        register double t1 = TWOPI*x[1], t2 = TWOPI*x[2];
        register double c1 = cos(t1), s1 = sin(t1);
        register double c2 = cos(t2), s2 = sin(t2);
        q.x = s1*r1; q.y = c1*r1; q.z = s2*r2; q.w = c2*r2;
        return (q);
} /* Qt_Random */

/** RandomRotation
 *  Generate uniform random rotation matrix from uniform deviates.
 */
void RandomRotation(double x[3], Matrix &m)
{
        QuaternationToMatrix(RandomQuaternation(x), m);
} /* M3_RandomRot */

/* End of urot.c */




/*****************************************************************************
 * Mult: Compute quaternion product destQuat = qLeft * qRight.
 *  	    destQuat can be same as either qLeft or qRight or both.
 *****************************************************************************/
void 
Mult(Quaternation &destQuat, const Quaternation &qLeft, const Quaternation &qRight)
{

Quaternation  tempDest;


tempDest.w = qLeft.w*qRight.w - qLeft.x*qRight.x - 
    	      qLeft.y*qRight.y - qLeft.z*qRight.z;

tempDest.x = qLeft.w*qRight.x + qLeft.x*qRight.w + 
    	      qLeft.y*qRight.z - qLeft.z*qRight.y;

tempDest.y = qLeft.w*qRight.y + qLeft.y*qRight.w + 
    	      qLeft.z*qRight.x - qLeft.x*qRight.z;

tempDest.z = qLeft.w*qRight.z + qLeft.z*qRight.w + 
    	      qLeft.x*qRight.y - qLeft.y*qRight.x;

destQuat = tempDest;

}   /* Mult	*/


Quaternation operator* (const Quaternation& qLeft, const Quaternation& qRight)
{
  Quaternation  tempDest;

  tempDest.w = qLeft.w*qRight.w - qLeft.x*qRight.x - 
    	      qLeft.y*qRight.y - qLeft.z*qRight.z;

  tempDest.x = qLeft.w*qRight.x + qLeft.x*qRight.w + 
    	      qLeft.y*qRight.z - qLeft.z*qRight.y;

  tempDest.y = qLeft.w*qRight.y + qLeft.y*qRight.w + 
    	      qLeft.z*qRight.x - qLeft.x*qRight.z;

  tempDest.z = qLeft.w*qRight.z + qLeft.z*qRight.w + 
    	      qLeft.x*qRight.y - qLeft.y*qRight.x;

  return(tempDest);

}


/*****************************************************************************
 * normalize quaternion.
 *****************************************************************************/
Quaternation  
Normalize( const Quaternation &srcQuat)
{
    Quaternation destQuat;
    double normalizeFactor;


	normalizeFactor = 1.0 / sqrt( srcQuat.x*srcQuat.x + srcQuat.y*srcQuat.y + 
      	    	    	      srcQuat.z*srcQuat.z + srcQuat.w*srcQuat.w );

	destQuat.x = srcQuat.x * normalizeFactor;
	destQuat.y = srcQuat.y * normalizeFactor;
	destQuat.z = srcQuat.z * normalizeFactor;
	destQuat.w = srcQuat.w * normalizeFactor;

	return(destQuat);
}   /* Normalize  */


/*****************************************************************************
 * Conjugate - conjugate quaternion.  src == dest ok.
 *****************************************************************************/
Quaternation
Conjugate(const Quaternation &srcQuat)
{
    Quaternation destQuat;

	destQuat.x = -srcQuat.x;
	destQuat.y = -srcQuat.y;
	destQuat.z = -srcQuat.z;
	destQuat.w =  srcQuat.w;
	return(destQuat);

}   /* Conjugate  */


/*****************************************************************************
 * Invert: Invert quaternion; That is, form its multiplicative inverse.
 *  	    	src == dest ok.
 *****************************************************************************/
Quaternation 
Invert(const Quaternation &srcQuat)
    

{
    Quaternation  destQuat;
    double srcQuatNorm;

	srcQuatNorm = 1.0 / (srcQuat.x*srcQuat.x + srcQuat.y*srcQuat.y + 
    	    	     srcQuat.z*srcQuat.z + srcQuat.w*srcQuat.w);

	destQuat.x = -srcQuat.x * srcQuatNorm;
	destQuat.y = -srcQuat.y * srcQuatNorm;
	destQuat.z = -srcQuat.z * srcQuatNorm;
	destQuat.w =  srcQuat.w * srcQuatNorm;
	return(destQuat);

}   /* invert	*/


/*****************************************************************************
 * exp: Exponentiate quaternion, assuming scalar part 0.  src == dest ok.
 *****************************************************************************/

Quaternation
Exp(const Quaternation &srcQuat)
{
  Quaternation destQuat;

   double theta, scale;


	theta = sqrt(srcQuat.x*srcQuat.x + srcQuat.y*srcQuat.y + 
    	     srcQuat.z*srcQuat.z);

	if (theta > Q_EPSILON)
		scale = sin(theta)/theta;
	else
		scale = 1.0;

	destQuat.x = scale*srcQuat.x;
	destQuat.y = scale*srcQuat.y;
	destQuat.z = scale*srcQuat.z;
	destQuat.w = cos(theta);
	return(destQuat);

}   /* exp	*/



/*****************************************************************************
 * Log: Take the natural logarithm of unit quaternion.  src == dest ok.
 *****************************************************************************/
Quaternation  
Log(const Quaternation  &srcQuat)
{
    Quaternation  destQuat;
    double theta, scale;


	scale = sqrt(srcQuat.x*srcQuat.x + srcQuat.y*srcQuat.y + 
    	    	    	    	     srcQuat.z*srcQuat.z);
	theta = atan2(scale, srcQuat.w);

	if (scale > 0.0)
		scale = theta/scale;

	destQuat.x = scale * srcQuat.x;
	destQuat.y = scale * srcQuat.y;
	destQuat.z = scale * srcQuat.z;
	destQuat.w = 0.0;
	return(destQuat);

}    /* Log	*/




/*****************************************************************************
 *
    Interpolate: Spherical linear interpolation of unit quaternions.
 
   	As t goes from 0 to 1, destQuat goes from startQ to endQuat.
       This routine should always return a point along the shorter
   	of the two paths between the two.  That is why the vector may be
   	negated in the end.
   	
   	src == dest should be ok, although that doesn't seem to make much
   	sense here.
 *
 *****************************************************************************/
void 
InterpolateSphere(const Quaternation &startQuat, const Quaternation &endQuat, double t,Quaternation &destQuat)

{
    Quaternation  startQ;  	    	    	/* temp copy of startQuat	*/
    double  omega, cosOmega, sinOmega;
    double  startScale, endScale;


	startQ = startQuat;

	cosOmega = startQ.x*endQuat.x + startQ.y*endQuat.y + 
    	   startQ.z*endQuat.z + startQ.w*endQuat.w;

	/* If the above dot product is negative, it would be better to
	*  go between the negative of the initial and the final, so that
	*  we take the shorter path.  
	*/
	if ( cosOmega < 0.0 ) 
    {
    	cosOmega *= -1;
	    startQ.x *= -1;
	    startQ.y *= -1;
	    startQ.z*= -1;
	    startQ.w *= -1;
    }

	if ( (1.0 + cosOmega) > Q_EPSILON ) 
    {
		/* usual case */
		if ( (1.0 - cosOmega) > Q_EPSILON ) 
		{
			/* usual case */
			omega = acos(cosOmega);
			sinOmega = sin(omega);
			startScale = sin((1.0 - t)*omega) / sinOmega;
			endScale = sin(t*omega) / sinOmega;
   		} 
		else 
		{
			/* ends very close */
			startScale = 1.0 - t;
			endScale = t;
		}
		destQuat.x = startScale*startQ.x + endScale*endQuat.x;
		destQuat.y = startScale*startQ.y + endScale*endQuat.y;
		destQuat.z = startScale*startQ.z + endScale*endQuat.z;
		destQuat.w = startScale*startQ.w + endScale*endQuat.w;
    } 
	else 
    {
    /* ends nearly opposite */
    destQuat.x = -startQ.y;  
    destQuat.y =  startQ.x;
    destQuat.z = -startQ.w;  
    destQuat.w =  startQ.z;
    
    startScale = sin((0.5 - t) * PI);
    endScale = sin(t * PI);
	destQuat.x = startScale*startQ.x + endScale*destQuat.x;
	destQuat.y = startScale*startQ.y + endScale*destQuat.y;
	destQuat.z = startScale*startQ.z + endScale*destQuat.z;
	destQuat.w = startScale*startQ.w + endScale*destQuat.w;
 }

}   /* Interpolate	*/


/*****************************************************************************
 *
   - create a quaternion from two vectors that rotates
    	    	     	v1 to v2 about an axis perpendicular to both
 *
 *****************************************************************************/
void Quaternation::Set(const Vector &v1, const Vector &v2 )
{
Vector u1, u2 ;
Vector axis ;					 /* axis of rotation */
double theta ;				     /* angle of rotation about axis */
double theta_complement ;
double crossProductMagnitude ;

/*
** Normalize both vectors and take cross product to get rotation axis. 
*/

u1 = Normalize(v1) ;
u2 = Normalize(v2) ;

axis=CrossProd(u1,u2);

/*
** | u1 X u2 | = |u1||u2|sin(theta)
**
** Since u1 and u2 are normalized, 
**
**  theta = arcsin(|axis|)
*/
crossProductMagnitude = sqrt( DotProd ( axis, axis ) ) ;


/*
** Occasionally, even though the vectors are normalized, the magnitude will
** be calculated to be slightly greater than one.  If this happens, just
** set it to 1 or asin() will barf.
*/
if( crossProductMagnitude > 1.0 )
   crossProductMagnitude = 1.0 ;

#if 0
		// garbage ?? 
		// new 6.05.98
		//axis.Normalize();
		theta = abs(atan2(crossProductMagnitude, DotProd(u1,u2)));
		// hgxxx theta = (atan2(DotProd(u1,u2),crossProductMagnitude));
	    SetAxisRotation(axis.x, axis.y, axis.z, theta ) ;

#else 
/*
** Take arcsin of magnitude of rotation axis to compute rotation angle.
** Since crossProductMagnitude=[0,1], we will have theta=[0,pi/2].
*/
theta = asin( crossProductMagnitude ) ;
theta_complement = PI - theta ;

/*
// hg 
theta =acos(DotProd(u1,u2)); // -1 .. 1 0 .. PI
theta_complement = PI - theta ;
axis.Normalize();

TRACE("Theta 1 %g Theta 2 %g, Dot %g Cross %g \n",asin( crossProductMagnitude ) ,acos(DotProd(u1,u2)),DotProd(u1,u2),crossProductMagnitude);

*/

/*
** If cos(theta) < 0, use complement of theta as rotation angle.
*/

if(DotProd(u1, u2) < 0.0 )
   {
   theta = theta_complement ;
   theta_complement = PI - theta ;
   }

/* if angle is 0, just return identity quaternion   */
if( theta < Q_EPSILON )
   {
   x = 0.0 ;
   y = 0.0 ;
   z = 0.0 ;
   w = 1.0 ;
   }
else
   {
   if( theta_complement < Q_EPSILON )
      {
      /*
      ** The two vectors are opposed.  Find some arbitrary axis vector.
      ** First try cross product with x-axis if u1 not parallel to x-axis.
      */
      if( (u1.y*u1.y + u1.z*u1.z) >= Q_EPSILON )
	 {
	 	axis.x = 0.0 ;
	 	axis.y = u1.z ;
	 	axis.z = -u1.y ;
	 }
      else
	 {
	 	/*
	 	** u1 is parallel to to x-axis.  Use z-axis as axis of rotation.
	 	*/
	 	axis.x = axis.y = 0.0 ;
	 	axis.z = 1.0 ;
	 }
      }

   //axis.Normalize();
   SetAxisRotation(axis.x, axis.y, axis.z, theta ) ;
   // *this = Normalize( *this);

}

#endif
}	/* q_from_two_vecs */


/*****************************************************************************
 *  
    SetFromEuler - converts 3 euler angles (in radians) to a quaternion
     
	angles are in radians;  Assumes roll is rotation about X, pitch
	is rotation about Y, yaw is about Z.  Assumes order of 
	yaw, pitch, roll applied as follows:
	    
	    p' = roll( pitch( yaw(p) ) )

    	See comments for q_euler_to_col_matrix for more on this.
 *
 *****************************************************************************/

void
Quaternation::SetFromEuler(double yaw, double pitch, double roll)


{
    double  cosYaw, sinYaw, cosPitch, sinPitch, cosRoll, sinRoll;
    double  half_roll, half_pitch, half_yaw;


/* put angles into radians and divide by two, since all angles in formula
 *  are (angle/2)
 */

half_yaw = yaw / 2.0;
half_pitch = pitch / 2.0;
half_roll = roll / 2.0;

cosYaw = cos(half_yaw);
sinYaw = sin(half_yaw);

cosPitch = cos(half_pitch);
sinPitch = sin(half_pitch);

cosRoll = cos(half_roll);
sinRoll = sin(half_roll);


x = sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw;
y = cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw;
z = cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw;

w = cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw;

}	

/*
 *      Implementation of a virtual trackball.
 *      Implemented by Gavin Bell, lots of ideas from Thant Tessman and
 *              the August '88 issue of Siggraph's "Computer Graphics," pp. 121-129.
 *
 */

/*
 * This size should really be based on the distance from the center of
 * rotation to the point on the object underneath the mouse.  That
 * point would then track the mouse as closely as possible.  This is a
 * simple example, though, so that is left as an Exercise for the
 * Programmer.
 */
#define TRACKBALLSIZE  (0.8)

#ifndef M_SQRT1_2
#define M_SQRT1_2 (1.0/sqrt(2.0))
#endif

#ifndef M_SQRT2
#define M_SQRT2 sqrt(2.0)
#endif


/*
 * Project an x,y pair onto a sphere of radius r OR a hyperbolic sheet
 * if we are away from the center of the sphere.
 */
static float
tb_project_to_sphere(float r, float x, float y)
{
        float d, t, z;

        d = sqrt(x*x + y*y);
        if (d < r*M_SQRT1_2)    /* Inside sphere */
        z = sqrt(r*r - d*d);
        else
        {                       /* On hyperbola */
                t = r / M_SQRT2;
                z = t*t / d;
        }
        return z;
}



/*
 * Ok, simulate a track-ball.  Project the points onto the virtual
 * trackball, then figure out the axis of rotation, which is the cross
 * product of P1 P2 and O P1 (O is the center of the ball, 0,0,0)
 * Note:  This is a deformed trackball-- is a trackball in the center,
 * but is deformed into a hyperbolic sheet of rotation away from the
 * center.  This particular function was chosen after trying out
 * several variations.
 *
 * It is assumed that the arguments to this routine are in the range
 * (-1.0 ... 1.0)
 */
BOOL
Quaternation::SetTrackball(float p1x, float p1y, float p2x, float p2y,float trackballSize)
{
        Point a;		/* Axis of rotation */
        float phi;      /* how much to rotate about axis */
        Point p1, p2, d;
        float t;

        if (p1x == p2x && p1y == p2y)
        {
                Set(0,0,0,1.0); /* Zero rotation */
                return FALSE;
        }

/*
 * First, figure out z-coordinates for projection of P1 and P2 to
 * deformed sphere
 */
        p1.Set(p1x,p1y,tb_project_to_sphere(trackballSize,p1x,p1y));
        p2.Set(p2x,p2y,tb_project_to_sphere(trackballSize,p2x,p2y));

/*
 *      Now, we want the cross product of P1 and P2
 */
        a = CrossProd(p2,p1);

/*
 *      Figure out how much to rotate around that axis.
 */
        d = p1-p2;
        t = (d.Length()) / (2.0*trackballSize);
        /*
         * Avoid problems with out-of-control values...
         */
        if (t > 1.0) t = 1.0;
        if (t < -1.0) t = -1.0;
        phi = 2.0 * asin(t);

		SetAxisRotation(a,phi);
		return(TRUE);
}



#if 0


/*****************************************************************************
 * 
    qp_to_matrix: Convert quaternion to 3x4 PPHIGS rotation matrix.
   	    	    Quaternion need not be unit magnitude.
 
    notes:
    	- a call to this function took about 20-50us on Sun4's as of 7/92
 *
 *****************************************************************************/
void 
qp_to_matrix(destMatrix, srcQuat)

    Q_MatrixType	destMatrix;
    Quaternation  	srcQuat;

{
    double s,xs,ys,zs,wx,wy,wz,xx,xy,xz,yy,yz,zz;

/* For unit srcQuat, just set s = 2.0; or set xs = srcQuat.x + 
 *   srcQuat.x, etc. 
 */
s = 2.0 / (srcQuat.x*srcQuat.x + srcQuat.y*srcQuat.y + 
	   srcQuat.z*srcQuat.z + srcQuat.w*srcQuat.w);

xs = srcQuat.x * s;   ys = srcQuat.y * s;   zs = srcQuat.z * s;
wx = srcQuat.w * xs;  wy = srcQuat.w * ys;  wz = srcQuat.w * zs;
xx = srcQuat.x * xs;  xy = srcQuat.x * ys;  xz = srcQuat.x * zs;
yy = srcQuat.y * ys;  yz = srcQuat.y * zs;  zz = srcQuat.z * zs;

/* set up 4x4 destMatrixrix */
destMatrix[X][X] = 1.0 - (yy + zz);
destMatrix[X][Y] = xy - wz;
destMatrix[X][Z] = xz + wy;
destMatrix[X].w = 0.0;

destMatrix[Y][X] = xy + wz;
destMatrix[Y][Y] = 1.0 - (xx + zz);
destMatrix[Y][Z] = yz - wx;
destMatrix[Y].w = 0.0;

destMatrix[Z][X] = xz - wy;
destMatrix[Z][Y] = yz + wx;
destMatrix[Z][Z] = 1.0 - (xx + yy);
destMatrix[Z].w = 0.0;

}   /* qp_to_matrix */


/*****************************************************************************
 * 
    q_to_row_matrix: Convert quaternion to 4x4 row-major rotation matrix.
  	    	    Quaternion need not be unit magnitude.
 *
 *****************************************************************************/
void 
q_to_row_matrix(destMatrix, srcQuat)

    q_matrix_type   destMatrix;
    Quaternation  	    srcQuat;

{
    double  s;
    double  xs, ys, zs,
    	    wx, wy, wz,
	    xx, xy, xz,
	    yy, yz, zz;


/* 
 * For unit srcQuat, just set s = 2.0; or set xs = srcQuat[X] + 
 *   srcQuat.x, etc. 
 */

s = 2.0 / (srcQuat.x*srcQuat.x + srcQuat.y*srcQuat.y + 
    	   srcQuat.z*srcQuat.z + srcQuat.w*srcQuat.w);

xs = srcQuat.x * s;   ys = srcQuat.y * s;   zs = srcQuat.z * s;
wx = srcQuat.w * xs;  wy = srcQuat.w * ys;  wz = srcQuat.w * zs;
xx = srcQuat.x * xs;  xy = srcQuat.x * ys;  xz = srcQuat.x * zs;
yy = srcQuat.y * ys;  yz = srcQuat.y * zs;  zz = srcQuat.z * zs;

/* set up 4x4 destMatrixrix */
destMatrix[X][X] = 1.0 - (yy + zz);
destMatrix[X][Y] = xy + wz;
destMatrix[X][Z] = xz - wy;
destMatrix[X].w = 0.0;

destMatrix[Y][X] = xy - wz;
destMatrix[Y][Y] = 1.0 - (xx + zz);
destMatrix[Y][Z] = yz + wx;
destMatrix[Y].w = 0.0;

destMatrix[Z][X] = xz + wy;
destMatrix[Z][Y] = yz - wx;
destMatrix[Z][Z] = 1.0 - (xx + yy);
destMatrix[Z].w = 0.0;

destMatrix.w[X] = 0.0;
destMatrix.w[Y] = 0.0;
destMatrix.w[Z] = 0.0;
destMatrix.w.w = 1.0;

}   /* q_to_row_matrix */



/*****************************************************************************
 * q_to_col_matrix: Convert quaternion to 4x4 column-major rotation matrix.
 *  	    	    Quaternion need not be unit magnitude.
 *****************************************************************************/
void 
q_to_col_matrix(destMatrix, srcQuat)
    
    q_matrix_type   destMatrix;
    Quaternation  	    srcQuat;

{
    double s,xs,ys,zs,wx,wy,wz,xx,xy,xz,yy,yz,zz;


/* For unit srcQuat, just set s = 2.0; or set xs = srcQuat[X] + 
 *  srcQuat.x, etc. 
 *****************************************************************************/
s = 2.0 / (srcQuat.x*srcQuat.x + srcQuat.y*srcQuat.y + 
    	   srcQuat.z*srcQuat.z + srcQuat.w*srcQuat.w);

xs = srcQuat.x * s;   ys = srcQuat.y * s;   zs = srcQuat.z * s;
wx = srcQuat.w * xs;  wy = srcQuat.w * ys;  wz = srcQuat.w * zs;
xx = srcQuat.x * xs;  xy = srcQuat.x * ys;  xz = srcQuat.x * zs;
yy = srcQuat.y * ys;  yz = srcQuat.y * zs;  zz = srcQuat.z * zs;

/* set up 4x4 destMatrixrix */
destMatrix[X][X] = 1.0 - (yy + zz);
destMatrix[X][Y] = xy - wz;
destMatrix[X][Z] = xz + wy;
destMatrix[X].w = 0.0;

destMatrix[Y][X] = xy + wz;
destMatrix[Y][Y] = 1.0 - (xx + zz);
destMatrix[Y][Z] = yz - wx;
destMatrix[Y].w = 0.0;

destMatrix[Z][X] = xz - wy;
destMatrix[Z][Y] = yz + wx;
destMatrix[Z][Z] = 1.0 - (xx + yy);
destMatrix[Z].w = 0.0;

destMatrix.w[X] = 0.0;
destMatrix.w[Y] = 0.0;
destMatrix.w[Z] = 0.0;
destMatrix.w.w = 1.0;

}   /* q_to_col_matrix */



/*****************************************************************************
 *
   qp_from_matrix- Convert 3x4 PPHIGS rotation matrix to unit quaternion.
 *
 *****************************************************************************/

void 
qp_from_matrix(destQuat, srcMatrix)
    
    Quaternation  	 destQuat;
    Q_MatrixType srcMatrix;

{
    
    double  	trace, s;
    int     	i, j, k;
    static int 	next[3] = {Y, Z, X};

trace = srcMatrix[X][X] + srcMatrix[Y][Y] + srcMatrix[Z][Z];

if (trace > 0.0) 
    {
    s = sqrt(trace + 1.0);
    destQuat.w = s * 0.5;
    s = 0.5 / s;
    
    destQuat.x = (srcMatrix[Z][Y] - srcMatrix[Y][Z]) * s;
    destQuat.y = (srcMatrix.x[Z] - srcMatrix[Z].x) * s;
    destQuat.z = (srcMatrix[Y].x - srcMatrix.x[Y]) * s;
    } 
else 
    {
    i = X;
    if (srcMatrix[Y][Y] > srcMatrix[X][X])
	i = Y;
    if (srcMatrix[Z][Z] > srcMatrix[i][i])
	i = Z;
    
    j = next[i];  
    k = next[j];
    
    s = sqrt( (srcMatrix[i][i] - (srcMatrix[j][j]+srcMatrix[k][k])) + 1.0 );
    
    destQuat[i] = s * 0.5;
    
    s = 0.5 / s;
    
    destQuat.w = (srcMatrix[k][j] - srcMatrix[j][k]) * s;
    destQuat[j] = (srcMatrix[j][i] + srcMatrix[i][j]) * s;
    destQuat[k] = (srcMatrix[k][i] + srcMatrix[i][k]) * s;
    
    }


}   /* qp_from_matrix */



/*****************************************************************************
 * q_from_col_matrix- Convert 4x4 column-major rotation matrix 
 *  	    	    	to unit quaternion.
 *****************************************************************************/

void 
q_from_col_matrix(destQuat, srcMatrix)
    
    Quaternation  	    destQuat;
    q_matrix_type   srcMatrix;

{
    double  	trace, s;
    int     	i, j, k;
    static int 	next[3] = {Y, Z, X};


trace = srcMatrix[X][X] + srcMatrix[Y][Y] + srcMatrix[Z][Z];

if (trace > 0.0) 
    {
    s = sqrt(trace + 1.0);
    
    destQuat.w = s * 0.5;
    
    s = 0.5 / s;
    
    destQuat[X] = (srcMatrix[Z][Y] - srcMatrix[Y][Z]) * s;
    destQuat.y = (srcMatrix.x[Z] - srcMatrix[Z][X]) * s;
    destQuat.z = (srcMatrix[Y][X] - srcMatrix[X][Y]) * s;
    } 

else 
    {
    i = X;
    if (srcMatrix[Y][Y] > srcMatrix[X][X])
	i = Y;
    if (srcMatrix[Z][Z] > srcMatrix[i][i])
	i = Z;
    j = next[i];  
    k = next[j];
    
    s = sqrt( (srcMatrix[i][i] - (srcMatrix[j][j]+srcMatrix[k][k])) + 1.0 );
    destQuat[i] = s * 0.5;
    s = 0.5 / s;
    
    destQuat.w = (srcMatrix[k][j] - srcMatrix[j][k]) * s;
    destQuat[j] = (srcMatrix[j][i] + srcMatrix[i][j]) * s;
    destQuat[k] = (srcMatrix[k][i] + srcMatrix[i][k]) * s;
    }

}   /* q_from_col_matrix */



/*****************************************************************************
 * 
   q_from_row_matrix: Convert 4x4 row-major rotation matrix to unit quaternion
 *
 *****************************************************************************/

void 
q_from_row_matrix(destQuat, srcMatrix)
    Quaternation  	    destQuat;
    q_matrix_type   srcMatrix;
{
    double  	trace, s;
    int     	i, j, k;
    static int 	next[3] = {Y, Z, X};


trace = srcMatrix[X][X] + srcMatrix[Y][Y]+ srcMatrix[Z][Z];

if (trace > 0.0)
    {
    s = sqrt(trace + 1.0);
    destQuat.w = s * 0.5;
    s = 0.5 / s;
    
    destQuat.x = (srcMatrix[Y][Z] - srcMatrix[Z][Y]) * s;
    destQuat.y = (srcMatrix[Z][X] - srcMatrix[X][Z]) * s;
    destQuat.z = (srcMatrix[X][Y] - srcMatrix[Y][X]) * s;
    } 

else 
    {
    i = X;
    if (srcMatrix[Y][Y] > srcMatrix[X][X])
	i = Y;
    if (srcMatrix[Z][Z] > srcMatrix[i][i])
	i = Z;
    j = next[i];  
    k = next[j];
    
    s = sqrt( (srcMatrix[i][i] - (srcMatrix[j][j]+srcMatrix[k][k])) + 1.0 );
    destQuat[i] = s * 0.5;
    
    s = 0.5 / s;
    
    destQuat.w = (srcMatrix[j][k] - srcMatrix[k][j]) * s;
    destQuat[j] = (srcMatrix[i][j] + srcMatrix[j][i]) * s;
    destQuat[k] = (srcMatrix[i][k] + srcMatrix[k][i]) * s;
    }

}   /* q_from_row_matrix */




#endif

