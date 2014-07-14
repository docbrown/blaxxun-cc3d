/*

	Notes:
	Code is based on the Piegl Nurbs book.
	The NURBS Book, Les Piegl and Wayne Tiller, Springer 1997 

*/


#define GNURBS_MAX_ORDER 16



// 
// find span - see the NURBS Book
// page 68 
// 
int FindSpan (int n, int p, float u, const float *U)
{   

	int low, high, mid; 
	if (u >= U[n+1]) // savety >= check 
		return (n);
	low = p; 
	high = n+1; 
	mid = (low + high)/2; 
	while (u < U[mid] || u >= U[mid+1]) 
	{	if ( u < U[mid]) 
		{ high = mid; 
		}
		else 
		{ low = mid; 
		}
		mid = (low + high)/2;		
	}
	
	return (mid); 

}



// 
// get Basis functions 
// page 70
// 

void BasisFuns ( int i, float u, int p, const float *U, float *Nf ) 
{

	float left[GNURBS_MAX_ORDER];  // could use alloca 
	float right[GNURBS_MAX_ORDER]; 
	float saved, temp; 

	Nf[0]  = 1.;
	for (int j=1; j<=p; j++)
	{
		left[j] = u-U[i+1-j];
		right[j] = U[i+j] -u; 
		saved = 0.0; 
		for (int r=0; r<j; r++)
		{ 
			temp = Nf[r]/(right[r+1]+left[j-r]);
			Nf[r] = saved+right[r+1]*temp; 
			saved = left[j-r]*temp;
		}
		Nf[j] = saved; 
	} 
}



// 
// get point on curve 
// [Piegl] page 82
// 

//
//
//called by the NurbsCurve
//
//Compute point on rational B-spline curve
//Input: n,p,U,P,w, u
//n: number of Points
//Output: Cx, Cy, Cz in outPoint

void CurvePoint (int n, int p, const float *U, const float *P, const float *w, const float u, Point &outPoint)
{
	int    span; 

	float Nf[GNURBS_MAX_ORDER]; 
	float Cw, Cwx, Cwy, Cwz; 

	span = FindSpan (n, p, u, U);  
	BasisFuns (span, u, p, U, Nf);  
	

	P+= (span-p)*3;
	if (w)  w += span-p;

	Cwx = Nf[0] * P[0];
	Cwy = Nf[0] * P[1];
	Cwz = Nf[0] * P[2];

	if (w) Cw  = Nf[0]  * w[0];

	P+=3;

	for (int i=1; i<=p; i++,P+=3)
	{
		Cwx +=  Nf[i] * P[0];
		Cwy +=  Nf[i] * P[1];
		Cwz +=  Nf[i] * P[2];
		if (w) Cw  += Nf[i]  * w[i];
		//TRACE("CV%i: %f %f %f\n", i, P[0], P[1], P[2]);
	}

	if (w) {
		Cw = 1.0f/Cw;
		outPoint.x = Cwx*Cw;
		outPoint.y = Cwy*Cw;
		outPoint.z = Cwz*Cw;
	} else {
		outPoint.x = Cwx;
		outPoint.y = Cwy;
		outPoint.z = Cwz;
	}

	//TRACE("CurvePoint: fraction %f  x y z: %f %f %f \n", u, outPoint.x, outPoint.y, outPoint.z); 

}


void CurvePoint2D (int n, int p, const float *U, const float *P, const float *w, const float u, Point &outPoint)
{
	int    span; 

	float Nf[GNURBS_MAX_ORDER]; 
	float Cw, Cwx, Cwy; 

	span = FindSpan (n, p, u, U);  
	BasisFuns (span, u, p, U, Nf);  
	

	P+= (span-p)*2;
	if (w)  w += span-p;

	Cwx = Nf[0] * P[0];
	Cwy = Nf[0] * P[1];

	if (w) Cw  = Nf[0]  * w[0];

	P+=2;

	for (int i=1; i<=p; i++,P+=2)
	{
		Cwx +=  Nf[i] * P[0];
		Cwy +=  Nf[i] * P[1];
		if (w) Cw  += Nf[i]  * w[i];
		//TRACE("CV%i: %f %f %f\n", i, P[0], P[1], P[2]);
	}

	if (w) {
		Cw = 1.0f/Cw;
		outPoint.x = Cwx*Cw;
		outPoint.y = Cwy*Cw;
	} else {
		outPoint.x = Cwx;
		outPoint.y = Cwy;
	}

	//TRACE("CurvePoint: fraction %f  x y z: %f %f %f \n", u, outPoint.x, outPoint.y, outPoint.z); 

}

// evaluate point on Nurbs Surface 
// version with weight

gbool SurfacePoint(
				  float u, float v, int uOrder, int vOrder, int uDimension, int vDimension,
				  const float *uKnot, const float * vKnot,
				  const Point *P,const float *W,
				  float *dest // result
				  )
{
	
	float Nu[GNURBS_MAX_ORDER]; 
	float Nv[GNURBS_MAX_ORDER]; 

	int pu = uOrder-1;
	int pv = vOrder-1;


	// skip if src not in knot parametric range
	if (u<uKnot[pu]) return false;
	if (u>uKnot[uDimension]) return false;

	if (v<vKnot[pv]) return false;
	if (v>vKnot[vDimension]) return false;

	// find knot vector span and evaluate Bspline Basis functions
	int spanu=FindSpan(uDimension,pu,u,uKnot);
	BasisFuns (spanu, u, pu, uKnot, Nu);

	int spanv=FindSpan(vDimension,pv,v,vKnot);
	BasisFuns (spanv, v, pv, vKnot, Nv);

	Point result;
	result.Zero();

	float resultw = 0.0f;



	int vi;

	int pvi=(spanv-pv)*uDimension;

	for (vi=0; vi<=pv; vi++, pvi+=uDimension)
	{
		float w= Nv[vi];

		for (int ui=0; ui<=pu; ui++)
		{
			int pui = spanu-pu+ui;
			result += w * Nu[ui] * P[pvi+pui];
			if(W) resultw += w * Nu[ui] * W[pvi+pui];
		}

	}


	// TRACE("%f %f %f \n",result.x,result.y,result.z);
	if (W) result *= (1.0/resultw);

	*dest++ = result.x;
	*dest++ = result.y;
	*dest++ = result.z;

	return gtrue;


}


//
// Compute the uk's 
// [Piegl] page 364 
//
void ComputeUkVector(int N, float *t, float* uk)
{
	float d;
	int i; 

	d = t[N] - t[0];
	uk[0] = 0.; 
	for (i=1; i<N; i++)
	{
		uk[i] = t[i]/d;
	}
	uk[N] = 1.;
}
//
// calculate the knotvector  
// page 365 
//
void ComputeKnotVector(int N, int pdim, int m, float *uk, float *u)
{
	int i, j; 

	for (j=0; j<=pdim; j++) 
	{ 
		u[j] = 0.; 
	} 

	for (j=1; j<=N-pdim; j++) 
	{	
		u[j+pdim] = 0.; 
		for (i=j; i<=j+pdim-1; i++)
		{
			u[j+pdim] = u[j+pdim] + uk[i];
		}
		u[j+pdim] = u[j+pdim]/(float) pdim;  
 	}	

	for (j=(m-pdim); j<=m; j++) 
	{ 
		u[j] = 1.;
	}
}


