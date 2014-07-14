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
#ifndef _GCameraUtil_h_
#define _GCameraUtil_h_



class GCamera;
class GvNode;
class Matrix;




void ConvertCameraToOrientation(const Point &position, const Point &target, const Point &up, 
							Vector &axis, float &angle);

//
// GCameraValue
// keeps basic camera setting, ready for interpolation

class GCameraValue {
public :
    Quaternation orientation;
    Point position;
    float fieldOfView;

    void Interpolate(const GCameraValue &a,const GCameraValue &b, float alpha);

    // set camera value by camera settings in c
    void Set(GCamera &c);
    int Set(GvNode *n);
    int Set(GvNode *n,const Matrix *cameraTransform);

};

// set the camera from GCameraValuse c
void SetCamera(GCamera *camera,const GCameraValue &c,const Matrix *cameraTransform=NULL);



class GCameraInterpolator
{
public:
    GCameraValue a;
    GCameraValue b;
    float alpha;
    GCameraValue current;
	
	double tstart,tend;

    void Reset() { alpha = 0; current = a; }

    void Interpolate(float newAlpha) { 
        alpha = newAlpha;
        current.Interpolate(a,b,alpha);  
    }

};


inline float  Interpolate (const float A, float ta, const float B,float tb)
{ return(A*ta+B*tb);
}

#if 0
class GCameraOp
{
public:
    // begin initialization 
    virtual void Init(GCamera *c) {};
    // do operation for time t  [0..1]
    virtual void Evaluate(GCamera *c,float t) = 0;
};

class GCameraOrbit : public GCameraOp
{
public:
    float wx,wy;
    GCameraOrbit(float Wx, float Wy);
    void Evaluate(GCamera *c,float t) { c->Orbit(wx*t,wy*t); }
};
// Orbit Pan Roll move
// position orientation fov

class GCameraCommand
{
    float duration;
    Array <GCameraOp*> ops;
    void Evaluate(GCamera *c,float t);

};

#endif



#endif

