/* 
 * This code is licensed under the Web3D-blaxxun Community Source License, 
 * provided in distribution file LICENSE.TXT and available online at 
 * http://www.web3D.org/TaskGroups/x3d/blaxxun/Web3D-blaxxunCommunitySourceAgreement.html 
 * and may only be used for non-commercial use as specified in that license. 
 * 
 * THE WEB3D CONSORTIUM AND BLAXXUN DO NOT MAKE AND HEREBY DISCLAIM ANY EXPRESS 
 * OR IMPLIED WARRANTIES RELATING TO THIS CODE, INCLUDING BUT NOT LIMITED TO, 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR 
 * PURPOSE, OR ANY WARRANTIES THAT MIGHT ARISE FROM A COURSE OF DEALING, USAGE 
 * OR TRADE PRACTICE.  THE COMMUNITY SOURCE CODE IS PROVIDED UNDER THIS 
 * AGREEMENT "AS IS," WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, 
 * INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COMMUNITY SOURCE CODE ARE 
 * FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR 
 * NON-INFRINGING OR IN ANY WAY CONSTITUTE THE COMPLETE PRODUCT MARKETED UNDER 
 * THE NAMES GIVEN SAID CODE. 
 */ 

/* LIBGUL - Geometry Utility Library
 * Copyright (c) 1999 by Norbert Irmer and Blaxxun Interactive 
 * All rights reserved 
 */ 
 
#ifndef GUGE_NORMALIZE_H
#define GUGE_NORMALIZE_H

namespace guge {

using gul::Ptr;
using gul::point;
using gul::point2;
using gul::hpoint;
using gul::hpoint2;

/*-----------------------------------------------------------------------
  Update an existing bounding box with additional points
-----------------------------------------------------------------------*/
template< class T >
inline void UpdateBoundingBoxH( const int nP, Ptr< hpoint<T> > &Pw,
                                T& minx, T& maxx,
                                T& miny, T& maxy,
                                T& minz, T& maxz )
{
  T x,y,z,w;  
  int i;

  for( i = 0; i < nP; i++ )
  {
    w = Pw[i].w;
    
    x = Pw[i].x / w;
    if( x < minx ) minx = x;
    else if( x > maxx ) maxx = x;
    
    y = Pw[i].y / w;
    if( y < miny ) miny = y;
    else if( y > maxy ) maxy = y;
    
    z = Pw[i].z / w;
    if( z < minz ) minz = z;
    else if( z > maxz ) maxz = z;
  }  
}
#ifdef _MSC_VER

template

inline void UpdateBoundingBoxH( const int nP, Ptr< hpoint<float> > &Pw,

                                float& minx, float& maxx,

                                float& miny, float& maxy,

                                float& minz, float& maxz );

template

inline void UpdateBoundingBoxH( const int nP, Ptr< hpoint<double> > &Pw,

                                double& minx, double& maxx,

                                double& miny, double& maxy,

                                double& minz, double& maxz );

#endif



/*
template< class T >
inline void UpdateBoundingBox( Ptr< hpoint<T> > &Pw, 
                               point<T> &minP, point<T> &maxP )
{                    
  UpdateBoundingBox( Pw.nElems(), Pw, minP.x, maxP.x, minP.y, maxP.y, 
                     minP.z, maxP.z );                 
}                                              
*/

template< class T >
inline void UpdateBoundingBoxH( const int nP, Ptr< hpoint2<T> > &Pw,
                                T& minx, T& maxx,
                                T& miny, T& maxy )
{
  T x,y,w;  
  int i;

  for( i = 0; i < nP; i++ )
  {
    w = Pw[i].w;
    
    x = Pw[i].x / w;
    if( x < minx ) minx = x;
    else if( x > maxx ) maxx = x;
    
    y = Pw[i].y / w;
    if( y < miny ) miny = y;
    else if( y > maxy ) maxy = y;
  }  
}
#ifdef _MSC_VER

template

inline void UpdateBoundingBoxH( const int nP, Ptr< hpoint2<float> > &Pw,

                                float& minx, float& maxx,

                                float& miny, float& maxy );

template

inline void UpdateBoundingBoxH( const int nP, Ptr< hpoint2<double> > &Pw,

                                double& minx, double& maxx,

                                double& miny, double& maxy );

#endif



/*
template< class T >
inline void UpdateBoundingBox( Ptr< hpoint2<T> > &Pw, 
                               point2<T> &minP, point2<T> &maxP )
{                    
  UpdateBoundingBox( Pw.nElems(), Pw, minP.x, maxP.x, minP.y, maxP.y );
}                                              
*/




template< class T >
inline void UpdateBoundingBoxE( const int nP, Ptr< point<T> > &P,
                                T& minx, T& maxx,
                                T& miny, T& maxy,
                                T& minz, T& maxz )
{                    
  for( int i = 0; i < nP; i++ )
  {
    if( P[i].x < minx ) minx = P[i].x;
    else if( P[i].x > maxx ) maxx = P[i].x;
    
    if( P[i].y < miny ) miny = P[i].y;
    else if( P[i].y > maxy ) maxy = P[i].y;

    if( P[i].z < minz ) minz = P[i].z;
    else if( P[i].z > maxz ) maxz = P[i].z;
  }                      
}                                              
/*
template< class T >
inline void UpdateBoundingBox( Ptr< point<T> > &P, point<T> &minP, point<T> &maxP )
{                    
  UpdateBoundingBox( P.nElems(), P, minP.x, maxP.x, minP.y, maxP.y, minP.z, maxP.z );                 
}                                              
*/
template< class T >                      
inline void UpdateBoundingBoxE( const int nP, Ptr< point2<T> > &P,
                                T& minx, T& maxx,
                                T& miny, T& maxy  )
{
  for( int i = 0; i < nP; i++ )
  {
    if( P[i].x < minx ) minx = P[i].x;
    else if( P[i].x > maxx ) maxx = P[i].x;
    
    if( P[i].y < miny ) miny = P[i].y;
    else if( P[i].y > maxy ) maxy = P[i].y;
  }
}
/*
template< class T >
inline void UpdateBoundingBox( Ptr< point2<T> > &P, point2<T> &minP, point2<T> &maxP)
{                    
  UpdateBoundingBox( P.nElems(), P, minP.x, maxP.x, minP.y, maxP.y );                 
}                                              
*/
/*-------------------------------------------------------------------------
  Calculate the bounding box of a set of points
------------------------------------------------------------------------*/

template< class T >
inline void CalcBoundingBoxE( const int nP, Ptr< point<T> > &P,
                              T& minx, T& maxx,
                              T& miny, T& maxy,
                              T& minz, T& maxz )
{
  minx = maxx = P[0].x;
  miny = maxy = P[0].y;
  minz = maxz = P[0].z;
 
  UpdateBoundingBoxE( nP, P, minx, maxx, miny, maxy, minz, maxz );
}
                                            
template< class T >
inline void CalcBoundingBoxE( const int nP, Ptr< point2<T> > &P,
                              T& minx, T& maxx,
                              T& miny, T& maxy  )
{
  minx = maxx = P[0].x;
  miny = maxy = P[0].y;
 
  UpdateBoundingBoxE( nP, P, minx, maxx, miny, maxy );
}

/*-------------------------------------------------------------------------
  Calculate the bounding box of a set of homogeneous points (after they
  are projected into Euclidian space)
------------------------------------------------------------------------*/
template< class T >
inline void CalcBoundingBoxH( const int nP, Ptr< hpoint<T> > &Pw,
                              T& minx, T& maxx,
                              T& miny, T& maxy,
                              T& minz, T& maxz )
{
  T w = Pw[0].w;

  minx = maxx = Pw[0].x / w;
  miny = maxy = Pw[0].y / w;
  minz = maxz = Pw[0].z / w;
 
  UpdateBoundingBoxH( nP, Pw, minx, maxx, miny, maxy, minz, maxz );
}

template< class T >
inline void CalcBoundingBoxH( const int nP, Ptr< hpoint2<T> > &Pw,
                              T& minx, T& maxx,
                              T& miny, T& maxy )
{
  T w = Pw[0].w;

  minx = maxx = Pw[0].x / w;
  miny = maxy = Pw[0].y / w;
 
  UpdateBoundingBoxH( nP, Pw, minx, maxx, miny, maxy );
}



template< class HP, class EP >
inline void CalcBoundingBoxVerts( Ptr< HP > &Pw, EP &minP, EP &maxP );

template< class HP, class EP >
inline void UpdateBoundingBoxVerts( Ptr< HP > &Pw, EP &minP, EP &maxP );

/* -------------- found no other way than this for VC++ :(((( ----------------------- */

template<>
inline void CalcBoundingBoxVerts( Ptr< point2<float> > &P, 
                                  point2<float> &minP, point2<float> &maxP )
{                    
  CalcBoundingBoxE( P.nElems(), P, minP.x, maxP.x, minP.y, maxP.y );
}
template<>
inline void CalcBoundingBoxVerts( Ptr< point2<double> > &P, 
                                  point2<double> &minP, point2<double> &maxP )
{                    
  CalcBoundingBoxE( P.nElems(), P, minP.x, maxP.x, minP.y, maxP.y );
}
template<>
inline void CalcBoundingBoxVerts( Ptr< hpoint2<float> > &Pw, 
                                  point2<float> &minP, point2<float> &maxP )
{                    
  CalcBoundingBoxH( Pw.nElems(), Pw, minP.x, maxP.x, minP.y, maxP.y );
}
template<>
inline void CalcBoundingBoxVerts( Ptr< hpoint2<double> > &Pw, 
                                  point2<double> &minP, point2<double> &maxP )
{                    
  CalcBoundingBoxH( Pw.nElems(), Pw, minP.x, maxP.x, minP.y, maxP.y );
}
template<>
inline void CalcBoundingBoxVerts( Ptr< point<float> > &P, point<float> &minP, point<float> &maxP )
{                    
  CalcBoundingBoxE( P.nElems(), P, minP.x, maxP.x, minP.y, maxP.y, minP.z, maxP.z );                 
}  
template<>
inline void CalcBoundingBoxVerts( Ptr< point<double> > &P, point<double> &minP, point<double> &maxP )
{                    
  CalcBoundingBoxE( P.nElems(), P, minP.x, maxP.x, minP.y, maxP.y, minP.z, maxP.z );                 
} 
template<>
inline void CalcBoundingBoxVerts( Ptr< hpoint<float> > &Pw, point<float> &minP, point<float> &maxP )
{                    
  CalcBoundingBoxH( Pw.nElems(), Pw, minP.x, maxP.x, minP.y, maxP.y, minP.z, maxP.z );                 
}  
template<>
inline void CalcBoundingBoxVerts( Ptr< hpoint<double> > &Pw, point<double> &minP, point<double> &maxP )
{                    
  CalcBoundingBoxH( Pw.nElems(), Pw, minP.x, maxP.x, minP.y, maxP.y, minP.z, maxP.z );                 
} 


template<>
inline void UpdateBoundingBoxVerts( Ptr< point2<float> > &P, 
                                  point2<float> &minP, point2<float> &maxP )
{                    
  UpdateBoundingBoxE( P.nElems(), P, minP.x, maxP.x, minP.y, maxP.y );
}
template<>
inline void UpdateBoundingBoxVerts( Ptr< point2<double> > &P, 
                                  point2<double> &minP, point2<double> &maxP )
{                    
  UpdateBoundingBoxE( P.nElems(), P, minP.x, maxP.x, minP.y, maxP.y );
}
template<>
inline void UpdateBoundingBoxVerts( Ptr< hpoint2<float> > &Pw, 
                                  point2<float> &minP, point2<float> &maxP )
{                    
  UpdateBoundingBoxH( Pw.nElems(), Pw, minP.x, maxP.x, minP.y, maxP.y );
}
template<>
inline void UpdateBoundingBoxVerts( Ptr< hpoint2<double> > &Pw, 
                                  point2<double> &minP, point2<double> &maxP )
{                    
  UpdateBoundingBoxH( Pw.nElems(), Pw, minP.x, maxP.x, minP.y, maxP.y );
}
template<>
inline void UpdateBoundingBoxVerts( Ptr< point<float> > &P, point<float> &minP, point<float> &maxP )
{                    
  UpdateBoundingBoxE( P.nElems(), P, minP.x, maxP.x, minP.y, maxP.y, minP.z, maxP.z );                 
}  
template<>
inline void UpdateBoundingBoxVerts( Ptr< point<double> > &P, point<double> &minP, point<double> &maxP )
{                    
  UpdateBoundingBoxE( P.nElems(), P, minP.x, maxP.x, minP.y, maxP.y, minP.z, maxP.z );                 
} 
template<>
inline void UpdateBoundingBoxVerts( Ptr< hpoint<float> > &Pw, point<float> &minP, point<float> &maxP )
{                    
  UpdateBoundingBoxH( Pw.nElems(), Pw, minP.x, maxP.x, minP.y, maxP.y, minP.z, maxP.z );                 
}  
template<>
inline void UpdateBoundingBoxVerts( Ptr< hpoint<double> > &Pw, point<double> &minP, point<double> &maxP )
{                    
  UpdateBoundingBoxH( Pw.nElems(), Pw, minP.x, maxP.x, minP.y, maxP.y, minP.z, maxP.z );                 
} 



/*-----------------------------------------------------------------------
  translate and scale a set of points
------------------------------------------------------------------------*/
template< class T >
inline void NormalizePointsE( const int nP, Ptr< point<T> > P,
                              const T minx, const T scalex,
                              const T miny, const T scaley,
                              const T minz, const T scalez ) 
{
  for( int i = 0; i < nP; i++ )
  {
    P[i].x = (P[i].x - minx) / (scalex);
    P[i].y = (P[i].y - miny) / (scaley);
    P[i].z = (P[i].z - minz) / (scalez);
  }
}
template< class T >
inline void NormalizePointsE( const int nP, Ptr< point2<T> > P,
                              const T minx, const T scalex,
                              const T miny, const T scaley ) 
{
  for( int i = 0; i < nP; i++ )
  {
    P[i].x = (P[i].x - minx) / (scalex);
    P[i].y = (P[i].y - miny) / (scaley);
  }
}
/*-----------------------------------------------------------------------
  translate and scale a set of points, given in their homogeneous
  representation
------------------------------------------------------------------------*/
template< class T >
inline void NormalizePointsH( const int nP, Ptr< hpoint<T> > Pw,
                              const T minx, const T scalex,
                              const T miny, const T scaley,
                              const T minz, const T scalez ) 
{
  int i;
  T w;
  for( i = 0; i < nP; i++ )
  {
    w = Pw[i].w;
    Pw[i].x = (Pw[i].x - w*minx) / (scalex);
    Pw[i].y = (Pw[i].y - w*miny) / (scaley);
    Pw[i].z = (Pw[i].z - w*minz) / (scalez);
  }
}

/*-----------------------------------------------------------------------
  re-translate and re-scale a set of points
------------------------------------------------------------------------*/
template< class T >
inline void DeNormalizePointsE( const int nP, Ptr< point<T> > P,
                                const T minx, const T scalex,
                                const T miny, const T scaley,
                                const T minz, const T scalez ) 
{
  for( int i = 0; i < nP; i++ )
  {
    P[i].x = P[i].x * (scalex) + minx;
    P[i].y = P[i].y * (scaley) + miny;
    P[i].z = P[i].z * (scalez) + minz;    
  }
}
template< class T >
inline void DeNormalizePointsE( const int nP, Ptr< point2<T> > P,
                                const T minx, const T scalex,
                                const T miny, const T scaley ) 
{
  for( int i = 0; i < nP; i++ )
  {
    P[i].x = P[i].x * (scalex) + minx;
    P[i].y = P[i].y * (scaley) + miny;
  }
}
/*-----------------------------------------------------------------------
  re-translate and re-scale a set of points, given in their homogeneous
  representation
------------------------------------------------------------------------*/
template< class T >
inline void DeNormalizePointsH( const int nP, Ptr< hpoint<T> > Pw,
                                const T minx, const T scalex,
                                const T miny, const T scaley,
                                const T minz, const T scalez ) 
{
  int i;
  T w;
  for( i = 0; i < nP; i++ )
  {
    w = Pw[i].w;
    Pw[i].x = Pw[i].x * (scalex) + w * minx;
    Pw[i].y = Pw[i].y * (scaley) + w * miny;
    Pw[i].z = Pw[i].z * (scalez) + w * minz;    
  }
}

template< class T1, class T2 >
void set( gul::bounding_box<T1> &box, 
          const gul::point<T2> &minP, const gul::point<T2> &maxP ) 
{
  box.x1 = (T1)minP.x;
  box.x2 = (T1)maxP.x;
  box.y1 = (T1)minP.y;
  box.y2 = (T1)maxP.y;
  box.z1 = (T1)minP.z;
  box.z2 = (T1)maxP.z;
}

template< class T1, class T2 >
void set( gul::bounding_box<T1> &box, 
          const gul::point2<T2> &minP, const gul::point2<T2>& maxP) 
{
  box.x1 = (T1)minP.x;
  box.x2 = (T1)maxP.x;
  box.y1 = (T1)minP.y;
  box.y2 = (T1)maxP.y;
  box.z1 = 0;
  box.z2 = 0;  
}

}
                       
#endif






