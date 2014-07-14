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


/******************************************************************************
@doc

@module GNurbsUtil.cpp - NURBS utilities|

Copyright (c) 1999 by blaxxun interactive
All rights reserved

Purpose:

  
Notes:
Code is mostly based on the Piegl Nurbs book.

Changes:

Todo :


******************************************************************************/


#include "stdafx.h"
#include "gnurbsutil.h"


/* 
//////////////////////////////////////////////////////////////	
//															//	
//															//	
//				S O M E    F U N C T I O N S				//
//															//
//////////////////////////////////////////////////////////////
 
// get the error reporting object 
GReporter* getReporter(GTraversal &state) 
{
	GView * view = state.getBrowser(); 
	ASSERT(view != NULL);
	if (!view) return GReporter::current; // return NULL;
	return view->GetReporter();
}

*/



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



// fitting 
//
// Knot removal algorithm 
// [Piegl] page 185 
//
static 
int RemoveKnot(int n, int p, float *U, float *Px, float *Py, float *Pz,
			   float u, int r, int s, int num, float SCALE)
{  
	int i, j, ii, jj, k; 
	int m, ord; 
	int fout, last, first; 
	int t, off; 

	float alfi, alfj; 
	float *tempx, *tempy, *tempz; 
	float distx, disty, distz, dist; 

	int remflag; 

	tempx = new float[n+1]; 
	tempy = new float[n+1];
	tempz = new float[n+1];

	m   = n + p + 1; 
	ord = p + 1; 

	fout  = (2*r - s - p)/2; 
	last  = r-s ; 
	first = r-p; 

	for (t=0; t<num; t++)
	{
		off      = first - 1; 
		tempx[0] = Px[off]; 
		tempy[0] = Py[off]; 
		tempz[0] = Pz[off]; 

		tempx[last+1-off] = Px[last+1]; 
		tempy[last+1-off] = Py[last+1];
		tempz[last+1-off] = Pz[last+1];

		i  = first; j  = last; 
		ii = 1; 	jj = last - off; 

		remflag = 0; 
		while (j-i>t)
		{
			alfi      = (u-U[i])/(U[i+ord+t]-U[i]); 
			alfj      = (u-U[j-t])/(U[j+ord]-U[j-t]); 
			tempx[ii] = (Px[i] - (1.0-alfi)*tempx[ii-1])/alfi; 
			tempy[ii] = (Py[i] - (1.0-alfi)*tempy[ii-1])/alfi;
			tempz[ii] = (Pz[i] - (1.0-alfi)*tempz[ii-1])/alfi;

			tempx[jj] = (Px[j] - alfj*tempx[jj+1])/(1.0-alfj); 
			tempy[jj] = (Py[j] - alfj*tempy[jj+1])/(1.0-alfj);
			tempz[jj] = (Pz[j] - alfj*tempz[jj+1])/(1.0-alfj);

			i = i+1; ii = ii+1; 
			j = j-1; jj = jj-1; 
		}
		if (j-i<t)
		{
			distx = (tempx[ii-1] - tempx[jj+1]);
			disty = (tempy[ii-1] - tempy[jj+1]);
			distz = (tempz[ii-1] - tempz[jj+1]);
			dist  = sqrt(distx*distx + disty*disty + distz*distz); 
			if (dist<SCALE) 
			{
				remflag = 1; 
			}
		}
		else
		{
			alfi  = (u-U[i])/(U[i+ord+t]-U[i]); 
			distx = (Px[i] - (alfi*tempx[ii+t+1]+(1.0-alfi)*tempx[ii-1]));
			disty = (Py[i] - (alfi*tempy[ii+t+1]+(1.0-alfi)*tempy[ii-1]));
			distz = (Pz[i] - (alfi*tempz[ii+t+1]+(1.0-alfi)*tempz[ii-1]));
			dist  = sqrt(distx*distx + disty*disty + distz*distz); 
			if (dist<SCALE) 
			{
				remflag = 1; 
			}
		}
		if ( remflag == 0 ) 
		{
			break; 
		}
		else
		{ 
			i = first; j = last; 
			while (j-i>t)
			{
				Px[i] = tempx[i-off]; 
				Py[i] = tempy[i-off];
				Pz[i] = tempz[i-off];

				Px[j] = tempx[j-off]; 
				Py[j] = tempy[j-off];
				Pz[j] = tempz[j-off];

				i = i+1; j = j-1; 
			}
		}

		first = first-1; 
	}
	
	if (t==0) {
		delete tempx;
		delete tempy;
		delete tempz;
		return (t);
	}
	
	for (k=r+1; k<=m; k++)
	{
		U[k-t] = U[k]; 
	}
	j = fout; i=j; 
	for (k=1; k<t; k++)
	{
		if ((k%2) == 1)
		{
			i = i+1;
		}
		else
		{
			j = j-1; 
		}
	}
	for (k=i+1; k<=n; k++)
	{
		Px[j] = Px[k]; 
		Py[j] = Py[k];
		Pz[j] = Pz[k];
		j = j+1;
	}
	delete tempx;
	delete tempy;
	delete tempz;

	return (t); 
}
//
// LU Decomposition 
// [Pres] page 38 
//

//#define TINY 1.e-20
#define TINY 1.e-10
static 
void ludcmp (float **a, int n, int np, int *indx, float d) 
{
	int      i, imax, j ,k;
	float   aamax, dum, sum; 
	float *vv;
	vv = new float[n+1];

	d = 1.;
	for (i=0; i<=n; i++)
	{   
		aamax = 0;
		for (j=0; j<=n; j++)
		{	
			if (fabs(a[i][j]) > aamax) aamax = fabs(a[i][j]); 
		}
		if (aamax == 0.) 
		{ 
			TRACE (" Singular matrix in ludcmp ");
			delete vv;
			return;
		}
		vv[i] = 1./aamax;
	}
	for (j=0; j<=n; j++)
	{   
		for (i=0; i<=j-1; i++)
		{	
			sum = a[i][j]; 
			for (k=0; k<=i-1; k++)
			{	
				sum = sum - a[i][k]*a[k][j];
			}
			a[i][j] = sum;
		}
		aamax = 0.;
		for (i=j; i<=n; i++)
		{	
			sum = a[i][j];
			for (k=0; k<=j-1; k++)
			{	
				sum = sum - a[i][k]*a[k][j];
			}
			a[i][j] = sum;
			dum = vv[i]*fabs(sum);
			if (dum >= aamax)
			{	
				imax = i;
				aamax = dum;
			}
		}
		if (j != imax)
		{	
			for ( k=0; k<=n; k++)
			{	
				dum        = a[imax][k];
				a[imax][k] = a[j][k];
				a[j][k]    = dum; 
			}
			d = -d;
			vv[imax] = vv[j];
		}
		indx[j] = imax;
		if ( a[j][j] == 0.) a[j][j] = TINY; 
		if ( j != n) 
		{	
			dum = 1./ a[j][j];
			for (i=j+1; i<=n; i++)
			{	
				a[i][j] = a[i][j]*dum;
			}
		}
	}	
	delete vv;
}
//
// Backward Substitution
// [Pres] page 39 
// 
static 
void lubksb (float **a, int n, int np, int *indx, float *b)
{
	int i, j, ii, ll;
	float sum;

	ii = -999; 
	for (i=0; i<=n; i++)
	{	
		ll    = indx[i];
		sum   = b[ll];
		b[ll] = b[i];
		if (ii != -999) 
		{	
			for (j=ii; j<=i-1; j++)
			{	 
				sum = sum - a[i][j]*b[j];
			}
		}
		else if ( sum != 0.) 
		{	
			ii = i;
		}
		b[i] = sum;
	}
	for (i=n; i>=0; i--)
	{	
		sum = b[i];
		for (j=i+1; j<=n; j++)
		{	
			sum=sum-a[i][j]*b[j];
		}
		b[i] = sum/a[i][i];
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
//
// setup A matrix 
// 
const
void SetupMatrixA (int N, int pdim, float *uk, float *u, float **A)
{
	int i, j;
	int span; 
	float nf[4];

	for (i=0; i<=N; i++)
	{ 
		for (j=0; j<=N; j++)
		{  
			A[i][j] = 0.;
		}
	}
	for (i=0; i<=N; i++)
	{	
		span = FindSpan(N, pdim, uk[i], u); 
		BasisFuns(span, uk[i], pdim, u, nf); 
		for (j=0; j<4; j++)
		{ 
			A[i][span-pdim+j] = nf[j];
		}
	}
}
//
//
//


#if 1
//
// Compute a 3D Nurbscurve fitting a set of sample points
//
void ComputeNurbsCurve ( // input 
					   int numValues, 
					   const float *key, 
					   const float * keyValue,
					   float SCALE,
					   
					   // output
					   int &dimension,
					   int &order,
					   
					   FloatArray &knot,
					   PointArray &ctrlPts
					   //FloatArray &weight
					   )
{

	
    int      N, m, pdim;
	
	// float   SCALE; 
    int      i, j, ntemp;

	float  *t, *x, *y, *z; 
	float  *uk, *u; 
	float  *Px, *Py, *Pz;	 
	
	float   d;
	int     *indx;
	float	*rhs;
	float	**A;
	int	    irem, nrem, rflag; 

	///
	/// get input data
	/// 

	N    = numValues - 1; // dimension of datapoints -1

	pdim = 3; 
	
	order = pdim+1;

	m    = N + pdim + 1; // dimension of knotvector -1

	t = new float[N+1]; 
	x = new float[N+1];
	y = new float[N+1];
	z = new float[N+1];

	for (i=0; i<=N; i++)
	{	
		t[i]=key[i];
		x[i]=keyValue[i*3];
		y[i]=keyValue[i*3+1];
		z[i]=keyValue[i*3+2];
	}
	
	Px   = new float[N+1];
	Py   = new float[N+1];
	Pz   = new float[N+1];
	uk   = new float[N+1]; 
	u    = new float[N+10];
	rhs  = new float[N+1];
	indx = new int[N+1];

	  
	A = (float **) malloc ((N+1)*sizeof (float *)); 
	for (i=0; i<=N; i++)
	{	
		A[i] = (float *) malloc ((N+1)*sizeof (float)); 
	}		 

	ComputeUkVector (N, t, uk); 

	ComputeKnotVector (N, pdim, m, uk, u); 

	SetupMatrixA (N, pdim, uk, u, A); 

	//
	// calculate LU decomposition  
	// page [Press] page 38 
	//
	ludcmp (A, N, N, indx, d);
	
	for (j=0; j<=N; j++)
	{	rhs[j] = x[j];
	}
	lubksb (A, N, N, indx, rhs);
	for (j=0; j<=N; j++)
	{	Px[j] = rhs[j];
	}
	
	for (j=0; j<=N; j++)
	{	rhs[j] = y[j];
	}
	lubksb (A, N, N, indx, rhs);
	for (j=0; j<=N; j++)
	{	Py[j] = rhs[j];
	}
	
	for (j=0; j<=N; j++)
	{	rhs[j] = z[j];
	}
	lubksb (A, N, N, indx, rhs);
	for (j=0; j<=N; j++)
	{	Pz[j] = rhs[j];
	}

	//
	// Now test the Schweinetrick 
	//
    irem = 0; 
    nrem = 0; 

	if (SCALE >0.0f)  // remove control point & corresponding knots if within tolerance
	{
		rflag = 0; 
		i = pdim+1; 	
		while (rflag == 0) 
		{   
			if (irem == 0) i = i+1;
			if (i<N)
			{  
				irem = 0; 
				irem = RemoveKnot(N, pdim, u, Px, Py, Pz,
								  u[i], i, 1, 1, SCALE);
				if ( irem == 1 ) 
				{ 		  
					nrem++;	m--; N--;
				} 
			}
			else 
			{ 
				rflag = 1; 
			} 
		}
	}
	// WriteOutput (ntemp, N, m, u, Px, Py, Pz); 
	dimension = N+1;

	TRACE ("... # points processed/removed/kept %d %d %d \n", ntemp, ntemp-N-1, N+1 );

	knot.Set(m+1, u);
	ctrlPts.SetLength(dimension);

	for (i=0; i<m; i++) {
			ctrlPts[i].Set(Px[i],Py[i],Pz[i]);
	}

#ifdef CHECK 
	float  iv, upar;
	float  Cx, Cy, Cz; 
	iv = 1. / (float)(100);
	for (i=0; i<100; i++)
	{ upar = iv * (float)i + iv/2.; 
	  Cx = CurvePoint (N+2, pdim, u, Px, upar);
	  Cy = CurvePoint (N+2, pdim, u, Py, upar);
	  Cz = CurvePoint (N+2, pdim, u, Pz, upar);
	}	

#endif 


	delete t;
	delete x;
	delete y;
	delete z;

	delete Px;
	delete Py;
	delete Pz;
	delete uk;
	delete u;
	delete rhs;
	delete indx;

	  
	for (i=0; i<numValues; i++)
	{	
		free(A[i]);
	}		 
	free(A);


}

#endif


//////////////////////////////////////////////////////////////


	// store computed curve point 
	void GGridTess::Output(GGridPoint &p) {
		TRACE("ouput (%g %g) (%d %d) \n ", 
				p.p.x,p.p.y,p.iu,p.iv);

		curveP.Append(p);
	}

	void GGridTess::BeginContour() {
		curveS.Append(curveP.Length());
	}
	void GGridTess::EndContour() {
		curveS.Append(curveP.Length());
	}

	//int NumContours() { return curveS.Length() / 2; } ;

	void GGridTess::GetContour(int ci,PointArray & pts) 
	{
		int ofs=curveS[ci*2];
		int ofs1=curveS[ci*2+1];
		
		ASSERT((ci*2+1) < curveS.Length());

		pts.SetLength(ofs1-ofs);
		for (int i=ofs;i<ofs1;i++) {
			pts[i-ofs] = curveP[i].p;
		}

	}

	/*
	void SplitLine(const GGridPoint &p0, const GGridPoint &p1, float t, 
		GGridPoint &p)
	{
		p.t = t;
		Interpolate(p0.p,1.0-t, p1.p,t, p.p);
		MapToGridPoint(p);

	}
	*/

	
	void GGridTess::SubdivideLine(GGridPoint &p0, GGridPoint &p1) 
	{

		TRACE("P 0  (%g %g) (%d %d) P1  (%g %g) (%d %d)  \n", 
				p0.p.x,p0.p.y,p0.iu,p0.iv,
				p1.p.x,p1.p.y,p1.iu,p1.iv);


		if (InSameGrid(p0,p1)) { // do no subdivide further 
			Output(p1);
			return;
		}
		// grid delta 
		int diu = p1.iu - p0.iu;
		int div = p1.iv - p0.iv;

		int adiv = abs(div);
		int adiu = abs(diu);
		
		if ( (adiu <=1) && adiv <=1) {
			// p1 falling exactly on grid boundary ?
			if (p1.gridStatus !=0) {
				Output(p1);
				return; 
			}

			if (p0.gridStatus !=0) 
				return;
		}
		// subdivide
		
		GGridPoint p;
		float t=0.5;
		
		if (adiv > 0 && adiv > adiu) 
		{
			// try to divide at v grid line 
			float v;

			v = p0.iv;
			if (adiv == 1) v+=div; 
			else v+= div / 2;

			// remap 
			v = pmin.y + v/toGridScale.y;

			t = (v - p0.p.y) / (p1.p.y-p0.p.y);
			if (t>EPS && t< (1.0f-EPS)) {
				TRACE("Mapped to a v grid point %g\n",v);
			}
			else 
				t= 0.5;


		} else if (adiu > 0) {
			// try to divide at u grid line 
			float u;

			u = p0.iu;
			if (adiu == 1) u+=diu; 
			else u+= diu / 2;

			// remap 
			u = pmin.x + u/toGridScale.x;

			t = (u - p0.p.x) / (p1.p.x-p0.p.x);
			if (t>EPS && t< (1.0f-EPS)) {
				TRACE("Mapped to a u grid point %g\n",u);
			}
			else 
				t= 0.5;

		}

		// split
		SplitLine(p0,p1, t, p);

		// recursive 
		SubdivideLine(p0,p);
		SubdivideLine(p,p1);

	}

	// griddify a polyline 

	void GGridTess::Polyline(int np, const Point *p) 
	{
		GGridPoint p0,p1;
		p0.t=0;
		p0.p=p[0];
		MapToGridPoint(p0);
		bool p01 = true;

		Output(p0);

		for (int i=1; i < np; i++) {
			if (p01) { 
				p1.t=i;
				p1.p=p[i];
				MapToGridPoint(p1);
				SubdivideLine(p0, p1); 
				p01 = false;
			}
			else { 
				p0.t=i;
				p0.p=p[i];
				MapToGridPoint(p0);
				SubdivideLine(p1, p0); 
				p01 = true;
			}
		}
	}

	// nurbs curve stuff

	
	
	void GGridTess::NurbsCurve(int n, int p, const float *U, const float *P, const float *w,int steps) 
	{
		SetNurbsCurve(n,p,U,P,w);

		GGridPoint p0,p1;

		p0.t=U[p];
		EvalNurbsPoint(p0.t,p0.p);
		MapToGridPoint(p0);

		p1.t=U[n];

		teps = (p1.t - p0.t) / steps;

		EvalNurbsPoint(p1.t,p1.p);
		MapToGridPoint(p1);

		Output(p0);

		SubdivideCurve(p0,p1);

	}

	
	void GGridTess::SubdivideCurve(GGridPoint &p0, GGridPoint &p1) 
	{

		TRACE("P 0  (%g %g) (%d %d) P1  (%g %g) (%d %d)  \n", 
				p0.p.x,p0.p.y,p0.iu,p0.iv,
				p1.p.x,p1.p.y,p1.iu,p1.iv);


		float dt= p1.t - p0.t;


		if (InSameGrid(p0,p1) && (dt <= teps)) { // do no subdivide further 
			Output(p1);
			return;
		}
		// grid delta 
		int diu = p1.iu - p0.iu;
		int div = p1.iv - p0.iv;

		int adiv = abs(div);
		int adiu = abs(diu);
		
		if ( (dt <= (2.0*teps)) && (adiu <=1) && adiv <=1) {
			// p1 falling exactly on grid boundary ?
			if (p1.gridStatus !=0) {
				Output(p1);
				return; 
			}

			if (p0.gridStatus !=0) 
				return;

			// subdivide linear to speed up recursion
			if (dt <= (2.0*teps)) {
				SubdivideLine(p0,p1);
				return;
			}

		}
		// subdivide
		
		GGridPoint p;

		// split half
		float t= p0.t + (p1.t-p0.t) * 0.5;
		p.t = t;

		EvalNurbsPoint(t,p.p);
		MapToGridPoint(p);


#if 0		
		if (adiv > 0 && adiv > adiu) 
		{
			// try to divide at v grid line 
			float v;

			v = p0.iv;
			if (adiv == 1) v+=div; 
			else v+= div / 2;

			// remap 
			v = pmin.y + v/toGridScale.y;

			t = (v - p0.p.y) / (p1.p.y-p0.p.y);
			if (t>EPS && t< (1.0f-EPS)) {
				TRACE("Mapped to a v grid point %g\n",v);
			}
			else 
				t= 0.5;


		} else if (adiu > 0) {
			// try to divide at u grid line 
			float u;

			u = p0.iu;
			if (adiu == 1) u+=diu; 
			else u+= diu / 2;

			// remap 
			u = pmin.x + u/toGridScale.x;

			t = (u - p0.p.x) / (p1.p.x-p0.p.x);
			if (t>EPS && t< (1.0f-EPS)) {
				TRACE("Mapped to a u grid point %g\n",u);
			}
			else 
				t= 0.5;

		}


		// split
		SplitLine(p0,p1, t, p);
#endif 


		// recursive 
		SubdivideCurve(p0,p);
		SubdivideCurve(p,p1);

	}

