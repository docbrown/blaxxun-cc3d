
#include <nu_types.h>
#include <nuar_exact.h>
#include <nuge_normalize.h>
#include <nugr_basics.h>
#include <nugr_split.h>
#include <nugr_regularize.h>
#include <nugr_triangulate.h>
#include <nugr_io.h>

#if 0
void test_int( void )
{
  unsigned long a[4] = { 0xa89bffff, 0xef12aaaa, 0xcccc5555, 0x123411ff};
  unsigned long b[3] = { 0xa7897777, 0xffff6666, 0x88884444 };
  unsigned long c[5];
  int nc;
  
  NUAR_IntAdd( 4, a, 3, b, &nc, c );  
}

void test_int2( void )
{
  unsigned long a[10];
  unsigned long b[10];
  unsigned long c[20];
  int result,na,nb,nc;
  double f;

  NUAR_DoubleToInt( 914.0, 10, a, &na );
  NUAR_DoubleToInt( 84.0, 10, b, &nb );
  
  NUAR_IntSub( na, a, nb, b, &nc, c );
  
  f = NUAR_IntToDouble( nc, c );
  printf( "f = %f\n", f );

  NUAR_IntSub( nb, b, na, a, &nc, c );
  
  f = NUAR_IntToDouble( nc, c );
  printf( "f = %f\n", f );
}

void test_int3( void )
{
  unsigned long a[10];
  unsigned long b[10];
  unsigned long q[10];
  unsigned long r[10];
  int result,na,nb,nq,nr;
  double f;

  NUAR_DoubleToInt( 3142.0*(double)0x100000000LL, 10, a, &na );
  NUAR_DoubleToInt( 47.0*(double)0x100000000LL, 10, b, &nb );
  
//  NUAR_IntDiv( na, a, nb, b, &nq, q, &nr, r );
  NUAR_IntDiv( nb, b, na, a, &nq, q, &nr, r );
  
  f = NUAR_IntToDouble( nq, q );
  printf( "q = %f\n", f );

  f = NUAR_IntToDouble( nr, r );
  printf( "r = %f\n", f );
}


void test_rat( void )
{
  NUPrecRational a(924.0), ra, b(84.0), one(1.0), c;
  double f,n,d;
  
  c = a * b;
  f = NUAR_IntToDouble( c.m->m_na, c.m->m_a );
  printf( "f = %f\n", f );

  c = a + b;
  f = NUAR_IntToDouble( c.m->m_na, c.m->m_a );
  printf( "f = %f\n", f );

  ra = one / a;
  c = ra + b;
  n = NUAR_IntToDouble( c.m->m_na, c.m->m_a );
  d = NUAR_IntToDouble( c.m->m_nb, c.m->m_b );
  printf( "%f / %f\n", n, d );

}

void test_rat2( void )
{
  NUPrecRational x1(10.0), y1(10.0), x2(20.0), y2(20.0), y(15.0), c;
  double n,d;
  
  c = x1 + (y-y1)*(x2-x1)/(y2-y1);
  c.Dump(&n,&d);
  printf( "%f/%f\n", n, d );
}
 
void testGraph2( void )
{
  NUGraphEdge *NE;
  NUIndex nNE;
  NUIndex nE = 6, nP = 8, i;
  
  NUPlanarPoint P[8] = {
    {0.,10.}, {6.,10.},    // P0,P1
    {3.,7.},               // P2
    {0.,4.}, {6.,4.},      // P3,P4
    {3.,5.},               // P5
    {1.,2.}, {5.,2.}       // P6,P7
  };

  NUGraphVertice V[8];
  NUGraphEdge E[6] = {
    { {&V[0], &V[2]}, {&E[0], &E[2]}, {0, 0} },   // E0
    { {&V[1], &V[2]}, {&E[1], &E[0]}, {0, 0} },   // E1
    { {&V[2], &V[3]}, {&E[3], &E[2]}, {0, 0} },   // E2
    { {&V[2], &V[4]}, {&E[1], &E[3]}, {0, 0} },   // E3
    { {&V[5], &V[6]}, {&E[5], &E[4]}, {0, 0} },   // E4
    { {&V[5], &V[7]}, {&E[4], &E[5]}, {0, 0} }    // E5
  };
  
  NUGraphVertice dummiV[8] = {
    { &P[0], &E[0] },
    { &P[1], &E[1] },
    { &P[2], &E[0] },
    { &P[3], &E[2] },
    { &P[4], &E[3] },
    { &P[5], &E[4] },
    { &P[6], &E[4] },
    { &P[7], &E[5] }
  };

  for( i = 0; i < 8; i++ )
    V[i] = dummiV[i];


  NUGE_RegularizeGraph( nE, E, nP, V, &nNE, &NE );
}


#endif

#if 0
void triangulate_test( void )
{
/*
  NUPlanarPoint Chain1[3] = { {0.,1.}, {0.,0.}, {1.,0.} };
  int nChain1 = sizeof(Chain1)/sizeof(NUPlanarPoint);
  NUPlanarPoint Chain2[3] = { {0.,1.}, {1.,1.}, {1.,0.}  };
  int nChain2 = sizeof(Chain2)/sizeof(NUPlanarPoint);
*/
/*
  NUPlanarPoint Chain1[] = { {1.,1.}, {0.,0.7}, {0.,0.3}, {1.,0.} };
  int nChain1 = sizeof(Chain1)/sizeof(NUPlanarPoint);
  NUPlanarPoint Chain2[] = { {1.,1.}, {1.,0.} };
  int nChain2 = sizeof(Chain2)/sizeof(NUPlanarPoint);
*/
/*
  NUPlanarPoint Chain1[] = { {0.,1.}, {-1.,0.7}, {-2.,0.7},{-1.,0.3}, {0.,0.} };
  int nChain1 = sizeof(Chain1)/sizeof(NUPlanarPoint);
  NUPlanarPoint Chain2[] = { {0.,1.}, {1.,0.7}, {1.,0.3}, {0.,0.} };
  int nChain2 = sizeof(Chain2)/sizeof(NUPlanarPoint);
*/

  NUPlanarPoint Chain1[] = { {0.,1.}, {-1.,0.7}, {-1.,0.3}, {0.,0.} };
  int nChain1 = sizeof(Chain1)/sizeof(NUPlanarPoint);
  NUPlanarPoint Chain2[] ={{0.,1.},{1.,0.7},{2.,0.7},{3.,0.7},{1.,0.3},{0.,0.}};
  int nChain2 = sizeof(Chain2)/sizeof(NUPlanarPoint);

  double minx,maxx,miny,maxy,dx,dy,scale;
  graph_edge_list E;
  graph_vertex_list2 V1,V2;
  triangle_list T;
  FILE *log;
 
  log = fopen( "triangle.ll", "w" );
  if( log == NULL ) exit(-1);

  minx = miny = 0.0;
  maxx = maxy = 1.0;
  
  NUGE_UpdateBoundingBoxWithPlanarPoints( 
         nChain1, Chain1, &minx, &maxx, &miny, &maxy );
  NUGE_UpdateBoundingBoxWithPlanarPoints( 
         nChain2, Chain2, &minx, &maxx, &miny, &maxy );
  dx = maxx - minx;
  dy = maxy - miny;
  scale = dx > dy ? dx : dy;
  minx -= scale;
  miny -= scale;

  NUGR_SetDumpTransformation( minx, miny, scale, scale );

  NUGR_ChainToGraph( nChain1, Chain1, minx, miny, scale, scale, 0, 1,
                     &E, &V1 );
  NUGR_DumpVertices( V1.head );
  NUGR_DumpEdges( E.head );

  NUGR_ChainToGraph( nChain2, Chain2, minx, miny, scale, scale, 0, 1,
                     &E, &V2 );
  NUGR_DumpVertices( V2.head );
  NUGR_DumpEdges( E.head );


  NUGR_TriangulateMonotonePolygon( &V1, &V2, &E, &T );
}
#endif



void test_graph2( void )
{
  NUPlanarPoint Contour1[4] = { {0.,0.}, {1.,0.}, {1.,1.}, {0.,1.} };
  NUPlanarPoint Contour2[4] = { {0.4,0.4}, {0.6,0.4}, {0.6,0.6}, {0.4,0.6} };
  int nContour1 = sizeof(Contour1)/sizeof(NUPlanarPoint);
  int nContour2 = sizeof(Contour2)/sizeof(NUPlanarPoint);
  double minx,maxx,miny,maxy,dx,dy,scale;
  graph_edge_list E;
  graph_vertex_list V;
  FILE *log;
  NUPrecRational farleft;
  triangle_list T;
 
  log = fopen( "graphtest.ll", "w" );
  if( log == NULL ) exit(-1);
 
  minx = miny = 0.0;
  maxx = maxy = 1.0;
  
  NUGE_UpdateBoundingBoxWithPlanarPoints( 
         nContour1, Contour1, &minx, &maxx, &miny, &maxy );
  NUGE_UpdateBoundingBoxWithPlanarPoints( 
         nContour2, Contour2, &minx, &maxx, &miny, &maxy );
  dx = maxx - minx;
  dy = maxy - miny;
  scale = dx > dy ? dx : dy;
  minx -= scale;
  miny -= scale;

  NUGR_SetDumpTransformation( minx, miny, scale, scale );

  NUGR_PolygonToGraph( nContour1, Contour1, minx, miny, scale, scale, 0, 1,
                       &E, &V );
  NUGR_DumpEdges( E.head );

  NUGR_PolygonToGraph( nContour2, Contour2, minx, miny, scale, scale, 0, 1,
                       &E, &V );
  NUGR_DumpEdges( E.head );

  NUGR_OrientEdges( E.head );

  printf("Before Regularization:\n" );
  printf("======================\n" );
  NUGR_DumpEdges( E.head );
  NUGR_DrawEdges( log, E.head, minx, miny, scale, scale );

  NUGR_Regularize( &E, &V );

  printf("After Regularization:\n" );
  printf("======================\n" );
  NUGR_DumpEdges( E.head );
  NUGR_DrawEdges( log, E.head, minx, miny, scale, scale );

  NUGR_Triangulate( &E, &V, farleft, &T );

  printf("After Triangulation:\n" );
  printf("======================\n" );
  NUGR_DrawTriangles( log, &T, minx, miny, scale, scale );

  fclose(log);
}







/* ----------------------------------------------------------------------
  Test Cutting graph into horizontal strips
------------------------------------------------------------------------ */

void test_cut_horizontal( void )
{
  NUPlanarPoint Domain[] = {
    {0.0, 0.0}, {1.0, 0.0},
    {1.0, 1.0}, {0.0, 1.0}
  };
  int nDomain = sizeof(Domain)/sizeof(NUPlanarPoint);
/*
  NUPlanarPoint Contour1[] = {
    {-0.05, -0.05}, {1.05, -0.05},
    {-0.05, 1.05}, {1.05, 1.05}
  };
*/

  NUPlanarPoint Contour1[] = {
    {-0.05, -0.05}, {-0.05, 1.05},
    {1.05, -0.05}, {1.05, 1.05}
  };


  int nContour1 = sizeof(Contour1)/sizeof(NUPlanarPoint);
  int nE = nContour1;
  double minx,maxx,miny,maxy,dx,dy,scale,y1,y2,far_x1,far_x2,min_x,max_x;
  graph_edge_list E;
  graph_vertex_list V;
  int nCuts = 5, i, j;
  cut_info Cuts[nCuts];
  NUPrecRational Y1,Y2,FARX1,FARX2,MINX,MAXX,DeltaY;
  FILE *log;
  
  log = fopen( "graphtest.ll", "w" );
  if( log == NULL ) exit(-1);
  
  minx = miny = 0.0;
  maxx = maxy = 1.0;
  
  NUGE_UpdateBoundingBoxWithPlanarPoints( 
         nContour1, Contour1, &minx, &maxx, &miny, &maxy );
  dx = maxx - minx;
  dy = maxy - miny;
  scale = dx > dy ? dx : dy;
  minx -= scale;
  miny -= scale;
  NUGR_SetDumpTransformation( minx, miny, scale, scale );

  NUGE_NormalizePlanarPoints( nDomain, Domain, minx, scale, miny, scale );
//  NUGE_NormalizePlanarPoints( nContour1, Contour1, minx, scale, miny, scale );
 
  NUGR_PolygonToGraph( nContour1, Contour1, minx, miny, scale, scale, 0, 1,
                       &E, &V );
    
  NUGR_OrientEdges( E.head );
  printf( "Original\n" );
  NUGR_DumpEdges( E.head );
  NUGR_DrawEdges( log, E.head, minx, miny, scale, scale );

  y1 = Domain[0].y;
  y2 = Domain[2].y;
  min_x = 0.5;
  max_x = 2.5;
  far_x1 = 0.0;
  far_x2 = 3.0;
  y1 *= (double)0x10000000000000LL;   /* 2^52 */
  y2 *= (double)0x10000000000000LL;   /* 2^52 */
  min_x *= (double)0x10000000000000LL;   /* 2^52 */
  max_x *= (double)0x10000000000000LL;   /* 2^52 */
  far_x1 *= (double)0x10000000000000LL;   /* 2^52 */
  far_x2 *= (double)0x10000000000000LL;   /* 2^52 */

  Y1 = NUPrecRational(y1);
  Y2 = NUPrecRational(y2);
  MINX = NUPrecRational(min_x);
  MAXX = NUPrecRational(max_x);
  FARX1 = NUPrecRational(far_x1);
  FARX2 = NUPrecRational(far_x2);
  DeltaY = (Y2-Y1)/NUPrecRational(nCuts-2);
  
  Cuts[1].val = Y1;
  for( i = 2; i < nCuts-1; i++ )
  {
    Cuts[i].val = Y1 + NUPrecRational(i-1)*DeltaY;
  }
  Cuts[nCuts-1].val = Y2;

  double f;
  for( i = 1; i < nCuts; i++ )
  {
    f = Cuts[i].val.Dump();
    f /= (double)0x10000000000000LL;
    f = f*scale+miny;
    printf( "val = %f\n", f );
  }

  NUGR_DumpVertices( V.head );
/*
  NUGR_Regularize( &E, &V );
  NUGR_DumpVertices( V.head );
*/
  NUGR_IntersectWithHorizontals( &E, &V, DeltaY, nCuts, Cuts );


  printf( "Strip %d\n======\n", i );
  for( j = nCuts-1; j >= 1; j-- )
  {
    NUGR_DumpEdges( Cuts[j].E.head );
  }

  for( i = nCuts-1; i > 0; i-- )
  {
    NUGR_DivideHorizontalStrips( &Cuts[i], &Cuts[i-1], nE,
                         MINX, MAXX, FARX1, FARX2 );

    printf( "After Processing Strip %d\n============\n", i );
    for( j = nCuts-1; j >= 1; j-- )
    {
      NUGR_DumpEdges( Cuts[j].E.head );
    }
  }

  printf( "Result:\n======\n" );
  for( j = nCuts-1; j >= 0; j-- )
  {
    NUGR_DumpEdges( Cuts[j].E.head );
    NUGR_DrawEdges( log, Cuts[j].E.head, minx, miny, scale, scale );
  }

  fclose(log);
}
/* ----------------------------------------------------------------------
  Test Cutting graph into vertical strips
------------------------------------------------------------------------ */

void test_cut_vertical( void )
{
  NUPlanarPoint Domain[] = {
    {0.0, 0.0}, {1.0, 0.0},
    {1.0, 1.0}, {0.0, 1.0}
  };
  int nDomain = sizeof(Domain)/sizeof(NUPlanarPoint);
/*
  NUPlanarPoint Contour1[] = {
    {-0.05, -0.05}, {1.05, -0.05},
    {-0.05, 1.05}, {1.05, 1.05}
  };
*/
  NUPlanarPoint Contour1[] = {
    {-0.05, -0.05}, {-0.05, 1.05},
    {1.05, -0.05}, {1.05, 1.05}
  };


  int nContour1 = sizeof(Contour1)/sizeof(NUPlanarPoint);
  int nE = nContour1;
  double minx,maxx,miny,maxy,dx,dy,scale,x1,x2,far_y1,far_y2,min_y,max_y;
  graph_edge_list E;
  graph_vertex_list V;
  int nCuts = 5, i, j;
  cut_info Cuts[nCuts];
  NUPrecRational X1,X2,FARY1,FARY2,MINY,MAXY,DeltaX;
  FILE *log;
  
  log = fopen( "graphtest.ll", "w" );
  if( log == NULL ) exit(-1);
  
  minx = -0.05;
  miny = 0.0;
  maxx = 1.05;
  maxy = 1.0;
  
  NUGE_UpdateBoundingBoxWithPlanarPoints( 
         nContour1, Contour1, &minx, &maxx, &miny, &maxy );
  dx = maxx - minx;
  dy = maxy - miny;
  scale = dx > dy ? dx : dy;
  minx -= scale;
  miny -= scale;

  NUGR_SetDumpTransformation( minx, miny, scale, scale );

  NUGE_NormalizePlanarPoints( nDomain, Domain, minx, scale, miny, scale );
 
  NUGR_PolygonToGraph( nContour1, Contour1, minx, miny, scale, scale, 0, 1,
                       &E, &V );

  NUGR_OrientEdges( E.head );

  printf( "Original\n" );
  NUGR_DumpEdges( E.head );
  NUGR_DrawEdges( log, E.head, minx, miny, scale, scale );

  x1 = Domain[0].x;
  x2 = Domain[2].x;
  min_y = 0.5;
  max_y = 2.5;
  far_y1 = 0.0;
  far_y2 = 3.0;
  x1 *= (double)0x10000000000000LL;   /* 2^52 */
  x2 *= (double)0x10000000000000LL;   /* 2^52 */
  min_y *= (double)0x10000000000000LL;   /* 2^52 */
  max_y *= (double)0x10000000000000LL;   /* 2^52 */
  far_y1 *= (double)0x10000000000000LL;   /* 2^52 */
  far_y2 *= (double)0x10000000000000LL;   /* 2^52 */

  X1 = NUPrecRational(x1);
  X2 = NUPrecRational(x2);
  MINY = NUPrecRational(min_y);
  MAXY = NUPrecRational(max_y);
  FARY1 = NUPrecRational(far_y1);
  FARY2 = NUPrecRational(far_y2);
  DeltaX = (X2-X1)/NUPrecRational(nCuts-2);
  
  Cuts[1].val = X1;
  for( i = 2; i < nCuts-1; i++ )
  {
    Cuts[i].val = X1 + NUPrecRational(i-1)*DeltaX;
  }
  Cuts[nCuts-1].val = X2;

  double f;
  for( i = 1; i < nCuts; i++ )
  {
    f = Cuts[i].val.Dump();
    f /= (double)0x10000000000000LL;
    f = f*scale+minx;
    printf( "val = %f\n", f );
  }
/*
  NUGR_DumpVertices( V.head );
  NUGR_Regularize( &E, &V );
*/
  NUGR_DumpVertices( V.head );

  NUGR_IntersectWithVerticals( &E, &V, DeltaX, nCuts, Cuts );

  printf( "Strip %d\n======\n", i );
  for( j = nCuts-1; j >= 1; j-- )
  {
    NUGR_DumpVertices( Cuts[j].V.head );
    NUGR_DumpEdges( Cuts[j].E.head );
  }

  for( i = nCuts-1; i > 0; i-- )
  {
    NUGR_DivideVerticalStrips( &Cuts[i], &Cuts[i-1], nE,
                               MINY, MAXY, FARY1, FARY2 );

    printf( "After Processing Strip %d\n============\n", i );
    for( j = nCuts-1; j >= 1; j-- )
    {
      NUGR_DumpEdges( Cuts[j].E.head );
    }
  }

  printf( "Result:\n======\n" );
  for( j = nCuts-1; j >= 0; j-- )
  {
    NUGR_DumpEdges( Cuts[j].E.head );
    NUGR_DrawEdges( log, Cuts[j].E.head, minx, miny, scale, scale );
  }

  fclose(log);
}


int main()
{
  NUST_InitializePoolAlloc();
//  test_graph2();
//  triangulate_test();
//  test_cut_horizontal();
  test_cut_vertical();
  
  return(0);
}


