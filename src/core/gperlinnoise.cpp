
#include "stdafx.h"
// perlin's stuff 

#include "gutils.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "gperlinnoise.h"

#define random rand
//#ifdef WIN32
//    static double random()			{ return ((double)::rand()) / (double) RAND_MAX; }
//#else
//    static double random()			{ return ::drand48(); }
//#endif



#define FLOOR(x) ((int)(x) - ((x) < 0 && (x) != (int)(x)))
#define CEIL(x) ((int)(x) + ((x) > 0 && (x) != (int)(x)))
#define CLAMP(x,a,b) ((x) <= (a) ? (a) : ((x) >= (b) ? (b) : (x)))
#define LERP(t,x0,x1)  ((x0) + (t)*((x1)-(x0)))

#define PULSE(a,b,x) (step((a),(x)) - step((b),(x)))
#define boxstep(a,b,x) clamp(((x)-(a))/((b)-(a)),0,1)



gfloat bias(gfloat a, gfloat b)
{
	return pow(a, log(b) / log(0.5));
}

gfloat gain(gfloat a, gfloat b)
{
	gfloat p = log(1. - b) / log(0.5);

	if (a < .001)
		return 0.;
	else if (a > .999)
		return 1.;
	if (a < 0.5)
		return pow(2 * a, p) / 2;
	else
		return 1. - pow(2 * (1. - a), p) / 2;
}

gfloat
smoothstep(gfloat x,gfloat a, gfloat b)
{
    if (x < a)
        return 0;
    if (x >= b)
        return 1;
    x = (x - a)/(b - a); /* normalize to [0:1] */
    return (x*x * (3 - 2*x));
}


gfloat Noise(gfloat vec[], int len)
{
	switch (len) {
	case 0:
		return 0.;
	case 1:
		return Noise1(vec[0]);
	case 2:
		return Noise2(vec);
	default:
		return Noise3(vec);
	}
}

gfloat turbulence(gfloat *v, gfloat freq)
{
	gfloat t, vec[3];

	for (t = 0. ; freq >= 1. ; freq /= 2) {
		vec[0] = freq * v[0];
		vec[1] = freq * v[1];
		vec[2] = freq * v[2];
		t += fabs(Noise3(vec)) / freq;
	}
	return t;
}

/* noise functions over 1, 2, and 3 dimensions */

#define B 0x100
#define BM 0xff

#define N 0x1000
#define NP 12   /* 2^N */
#define NM 0xfff

static int p[B + B + 2];
static gfloat g3[B + B + 2][3];
static gfloat g2[B + B + 2][2];
static gfloat g1[B + B + 2];
static int start = 1;


#define s_curve(t) ( t * t * (3. - 2. * t) )

#define lerp(t, a, b) ( a + t * (b - a) )

#define setup(i,b0,b1,r0,r1)\
	t = vec[i] + N;\
	b0 = ((int)t) & BM;\
	b1 = (b0+1) & BM;\
	r0 = t - (int)t;\
	r1 = r0 - 1.;


gfloat Noise1(gfloat arg)
{
	int bx0, bx1;
	gfloat rx0, rx1, sx, t, u, v, vec[1];

	vec[0] = arg;
	if (start) {
		start = 0;
		initNoise();
	}

	setup(0, bx0,bx1, rx0,rx1);

	sx = s_curve(rx0);

	u = rx0 * g1[ p[ bx0 ] ];
	v = rx1 * g1[ p[ bx1 ] ];

	return lerp(sx, u, v);
}

gfloat Noise2(const gfloat vec[2])
{
	int bx0, bx1, by0, by1, b00, b10, b01, b11;
	gfloat rx0, rx1, ry0, ry1, *q, sx, sy, a, b, t, u, v;
	int i, j;

	if (start) {
		start = 0;
		initNoise();
	}

	setup(0, bx0,bx1, rx0,rx1);
	setup(1, by0,by1, ry0,ry1);

	i = p[ bx0 ];
	j = p[ bx1 ];

	b00 = p[ i + by0 ];
	b10 = p[ j + by0 ];
	b01 = p[ i + by1 ];
	b11 = p[ j + by1 ];

	sx = s_curve(rx0);
	sy = s_curve(ry0);

#define at2(rx,ry) ( rx * q[0] + ry * q[1] )

	q = g2[ b00 ] ; u = at2(rx0,ry0);
	q = g2[ b10 ] ; v = at2(rx1,ry0);
	a = lerp(sx, u, v);

	q = g2[ b01 ] ; u = at2(rx0,ry1);
	q = g2[ b11 ] ; v = at2(rx1,ry1);
	b = lerp(sx, u, v);

	return lerp(sy, a, b);
}

gfloat Noise3(const gfloat vec[3])
{
	int bx0, bx1, by0, by1, bz0, bz1, b00, b10, b01, b11;
	gfloat rx0, rx1, ry0, ry1, rz0, rz1, *q, sy, sz, a, b, c, d, t, u, v;
	int i, j;

	if (start) {
		start = 0;
		initNoise();
	}

	setup(0, bx0,bx1, rx0,rx1);
	setup(1, by0,by1, ry0,ry1);
	setup(2, bz0,bz1, rz0,rz1);

	i = p[ bx0 ];
	j = p[ bx1 ];

	b00 = p[ i + by0 ];
	b10 = p[ j + by0 ];
	b01 = p[ i + by1 ];
	b11 = p[ j + by1 ];

	t  = s_curve(rx0);
	sy = s_curve(ry0);
	sz = s_curve(rz0);

#define at3(rx,ry,rz) ( rx * q[0] + ry * q[1] + rz * q[2] )

	q = g3[ b00 + bz0 ] ; u = at3(rx0,ry0,rz0);
	q = g3[ b10 + bz0 ] ; v = at3(rx1,ry0,rz0);
	a = lerp(t, u, v);

	q = g3[ b01 + bz0 ] ; u = at3(rx0,ry1,rz0);
	q = g3[ b11 + bz0 ] ; v = at3(rx1,ry1,rz0);
	b = lerp(t, u, v);

	c = lerp(sy, a, b);

	q = g3[ b00 + bz1 ] ; u = at3(rx0,ry0,rz1);
	q = g3[ b10 + bz1 ] ; v = at3(rx1,ry0,rz1);
	a = lerp(t, u, v);

	q = g3[ b01 + bz1 ] ; u = at3(rx0,ry1,rz1);
	q = g3[ b11 + bz1 ] ; v = at3(rx1,ry1,rz1);
	b = lerp(t, u, v);

	d = lerp(sy, a, b);

	return lerp(sz, c, d);
}

static void normalize2(gfloat v[2])
{
	gfloat s;

	s = sqrt(v[0] * v[0] + v[1] * v[1]);
	v[0] = v[0] / s;
	v[1] = v[1] / s;
}

static void normalize3(gfloat v[3])
{
	gfloat s;

	s = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	v[0] = v[0] / s;
	v[1] = v[1] / s;
	v[2] = v[2] / s;
}

void initNoiseRandom()
{
	int i, j, k;

	for (i = 0 ; i < B ; i++) {
		p[i] = i;

		g1[i] = (gfloat)((random() % (B + B)) - B) / B;

		for (j = 0 ; j < 2 ; j++)
			g2[i][j] = (gfloat)((random() % (B + B)) - B) / B;
		
		normalize2(g2[i]);

		for (j = 0 ; j < 3 ; j++)
			g3[i][j] = (gfloat)((random() % (B + B)) - B) / B;
		normalize3(g3[i]);
	}

	while (--i) {
		k = p[i];
		p[i] = p[j = random() % B];
		p[j] = k;
	}

	for (i = 0 ; i < B + 2 ; i++) {
		p[B + i] = p[i];
		g1[B + i] = g1[i];
		for (j = 0 ; j < 2 ; j++)
			g2[B + i][j] = g2[i][j];
		for (j = 0 ; j < 3 ; j++)
			g3[B + i][j] = g3[i][j];
	}
}

// Musgrave, init by NoiseTable
#include "gnoisetable.i"

void
initNoise()
{

	initNoiseRandom();

#if 0
	int i;

        for (i= 0; i < B+B+2; ++i)
          {
              p[i]= p_precomputed[i];
              //g[i][0]= g_precomputed[i][0];
              //g[i][1]= g_precomputed[i][1];
              //g[i][2]= g_precomputed[i][2];
              g1[i]= g_precomputed[i][0];
              g2[i]= g_precomputed[i][1];
              g3[i]= g_precomputed[i][2];
          }
#endif
}



/* 
 *=====================================================================*
 * --------------------------------------------------------------------*
 * ease-in/ease-out                                                    *
 * --------------------------------------------------------------------*
 * By Dr. Richard E. Parent, The Ohio State University                 *
 * (parent@cis.ohio-state.edu)                                         *
 * --------------------------------------------------------------------*
 * using parabolic blending at the end points                          *
 * first leg has constant acceleration from 0 to v during time 0 to t1 *
 * second leg has constant velocity of v during time t1 to t2          *
 * third leg has constant deceleration from v to 0 during time t2 to 1 *
 * these are integrated to get the 'distance' traveled at any time     *
 * --------------------------------------------------------------------*
 */
gfloat ease(gfloat t,gfloat t1,gfloat t2)
{
  gfloat  ans,s,a,b,c,nt,rt;
  gfloat  v,a1,a2;

  v = 2/(1+t2-t1);  /* constant velocity attained */
  a1 = v/t1;        /* acceleration of first leg */
  a2 = -v/(1-t2);   /* deceleration of last leg */

  if (t<t1) {
    rt = 0.5*a1*t*t;       /* pos = 1/2 * acc * t*t */
  }
  else if (t<t2) {
    a = 0.5*a1*t1*t1;      /* distance from first leg */
    b = v*(t-t1);            /* distance = vel * time  of second leg */
    rt = a + b;
  }
  else {
    a = 0.5*a1*t1*t1;      /* distance from first leg */
    b = v*(t2-t1);           /* distance from second leg */
    c = ((v + v + (t-t2)*a2)/2) * (t-t2);  /* distance = ave vel. * time */
    rt = a + b + c;
  }
  return(rt);                                  
}


/*
 * The QAEB procedural ray tracing routine.
 *
 * Fractal functions module.
 *
 * This is mostly bunch of C code for terrain models that appears in the book 
 * "Textures and Modeling: A Procedural Approach," Ebert et al, Academic Press,
 * 1998.
 *
 * Copyright 1998 F. Kenton Musgrave
 * All rights reserved
 */

static gfloat   exponent_array[MAX_OCTAVES];
static int	first=TRUE;
static gfloat lastLacunarity = 0.0;
static gfloat lastH = 0.0;

static void setOctaves(gfloat H, gfloat lacunarity)
{
	int		i;

	/* precompute and store spectral weights */
	if ( first || lastLacunarity != lacunarity || H!=lastH) {
		/* compute weight for each frequency */
		for (i=0; i<MAX_OCTAVES; i++) 
			exponent_array[i] = pow( lacunarity, -i*H );
		
		first = FALSE;
		lastLacunarity = lacunarity;
		lastH=H;
	}
}

/* 
 * Ridged multifractal - FKM 4/94
 *
 * Some good parameter values to start with:
 *
 *     H:            0.9
 *     offset:       1
 *     gain:         2
 */
gfloat 
RidgedMultifractal( Point point, gfloat H, gfloat lacunarity,
		    gfloat octaves, gfloat offset, gfloat threshold )
{
	gfloat  	result, frequency, signal, weight;
	int             i;
	
	setOctaves(H,lacunarity);

	/* get first octave */
	signal = Noise2( point );
	/* get absolute value of signal (this creates the ridges) */
	if ( signal < 0.0 ) signal = -signal;
	/* invert and translate (note that "offset" should be ~= 1.0) */
	signal = offset - signal;
	/* square the signal, to increase "sharpness" of ridges */
	signal *= signal;
	/* assign initial values */
	result = signal;
	weight = 1.0;

	for( i=1; weight>0.001 && i<octaves; i++ ) {
		point.x *= lacunarity;
		point.y *= lacunarity;
		point.z *= lacunarity;

		/* weight successive contributions by previous signal */
		weight = signal * threshold;
		if ( weight > 1.0 ) weight = 1.0;
		if ( weight < 0.0 ) weight = 0.0;
		signal = Noise2( point );
		if ( signal < 0.0 ) signal = -signal;
		signal = offset - signal;
		signal *= signal;
		/* weight the contribution */
		signal *= weight;
		result += signal * exponent_array[i];
	}

	return( result );

} /* RidgedMultifractal() */


/* 
 * Stats-by-Position multifractal - FKM 4/94
 *
 * Some good parameter values to start with:
 *
 *     H:            0.25
 *     offset:       .7
 *     gain:         1
 */
gfloat 
HybridMultifractal( Point point, gfloat H, gfloat lacunarity, 
		    gfloat octaves, gfloat offset, gfloat gain )
{
	gfloat		frequency, value, signal, weight, remainder;
	int		i;

	setOctaves(H,lacunarity);

	/* get first octave of function; later octaves are weighted */
	value = Noise2( point ) + offset;
	weight = gain * value;
	point.x *= lacunarity;
	point.y *= lacunarity;
	point.z *= lacunarity;

	/* inner loop of spectral construction, where the fractal is built */
	for (i=1; weight>0.001 && i<octaves; i++) {
		/* prevent divergence */
		if ( weight > 1.0 )  weight = 1.0;

		/* get next higher frequency */
		signal = ( Noise2( point ) + offset ) * exponent_array[i];
		/* add it in, weighted by previous freq's local value */
		value += weight * signal;
		/* update the (monotonically decreasing) weighting value */
		weight *= gain * signal;

		point.x *= lacunarity;
		point.y *= lacunarity;
		point.z *= lacunarity;
	} /* for */

	/* take care of remainder in "octaves" */
	remainder = octaves - (int)octaves;
	if ( remainder )
		/* "i" and spatial freq. are preset in loop above */
		value += remainder * Noise2( point ) * exponent_array[i];

	return( value );

} /* HybridMultifractal() */



/*
 * Heterogeneous procedural terrain fucntion: stats by altitude method.
 * Evaluated at "point"; returns value stored in "value".
 * 
 * Parameters:
 * 	"H" is the fractal increment
 * 	"lacunarity" is the gap between successive frequencies
 * 	"octaves" is the number of frequencies in the fBm
 * 	"offset" raises the terrain from ‘sea level’
 */
gfloat
Hetero_Terrain( Point point,
		gfloat H, gfloat lacunarity, gfloat octaves, gfloat offset )
{
	gfloat		value, increment, frequency, remainder;
	int			i;

	setOctaves(H,lacunarity);

	/* first unscaled octave of function; later octaves are scaled */
	value = offset + Noise2( point );
	point.x *= lacunarity;
	point.y *= lacunarity;
	point.z *= lacunarity;

	/* inner loop of spectral construction, where the fractal is built */
	for (i=1; i<octaves; i++) {
		/* obtain displaced noise value */
		increment = Noise2( point ) + offset;
		/* scale amplitude appropriately for this frequency */
		increment *= exponent_array[i];
		/* scale increment by current ‘altitude’ of fucntion */
		increment *= value;
		/* add increment to "value" */
		value += increment;
		point.x *= lacunarity;
		point.y *= lacunarity;
		point.z *= lacunarity;
	} /* for */

	/* take care of remainder in "octaves" */
	remainder = octaves - (int)octaves;
	if ( remainder ) {
		/* "i" and spatial freq. are preset in loop above */
		/* note that the main loop code is made shorter here */
		increment = (Noise2( point ) + offset) * exponent_array[i];
		value += remainder * increment * value;
	}

	return( value );

} /* Hetero_Terrain() */



/*
 * Procedural multifractal evaluated at "point"; 
 * returns value stored in "value".
 * 
 * Parameters:
 * 	H" determines the highest fractal dimension
 * 	"lacunarity" is gap between successive frequencies
 * 	"octaves" is the number of frequencies in the fBm
 * 	"scale" scales the basis function & determines multifractality
 */
gfloat
multifractal( Point point,
	      gfloat H, gfloat lacunarity, gfloat octaves, gfloat scale )
{
	gfloat		value, frequency, remainder;
	int		i;

	setOctaves(H,lacunarity);

	value = 1.0;		/* initialize ""value"" */

	/* inner loop of spectral construction, where the fractal is built */
	for (i=0; i<octaves; i++) {
		value *= exponent_array[i] * Noise3( point ) + 1.0;
		point.x *= lacunarity;
		point.y *= lacunarity;
		point.z *= lacunarity;
	} /* for */

	/* take care of remainder in "octaves" */
	remainder = octaves - (int)octaves;
	if ( remainder )
		/* "i" and spatial freq. are preset in loop above */
		value += remainder * exponent_array[i]
			 * Noise3( point ) + 1.0;

	return value;

} /* multifractal() */


/*
 * Procedural fBm evaluated at "point"; returns value stored in "value".
 * 
 * Parameters:
 * 	"H" is the fractal increment
 * 	"lacunarity" is the gap between successive frequencies
 * 	"octaves" is the number of frequencies in the fBm
 */
gfloat
fBm( Point point, gfloat H, gfloat lacunarity, gfloat octaves )
{
	gfloat		value, frequency, remainder;
	int		i;

	setOctaves(H,lacunarity);

	value = 0.0;		/* initialize "value" */

	/* inner loop of spectral construction, where the fractal is built */
	for (i=0; i<octaves; i++) {
		value += Noise3( point ) * exponent_array[i];
		point.x *= lacunarity;
		point.y *= lacunarity;
		point.z *= lacunarity;
	} /* for */

	/* take care of remainder in "octaves" */
	remainder = octaves - (int)octaves;
	if ( remainder )
		/* "i" and spatial freq. are preset in loop above */
		value += remainder * Noise3( point ) * exponent_array[i];

	return( value );

} /* fBm() */


/*
 * Procedural "turbulence" function: fBm constructed from abs( Noise() )
 * 
 * Parameters:
 * 	"H" is the fractal increment
 * 	"lacunarity" is the gap between successive frequencies
 * 	"octaves" is the number of frequencies in the fBm
 */
gfloat
turbulence( Point point, gfloat H, gfloat lacunarity, int octaves )
{
	gfloat		temp, value, frequency, remainder;
	int		i;

	setOctaves(H,lacunarity);

	value = 0.0;		/* initialize "value" */

	/* inner loop of spectral construction, where the fractal is built */
	for (i=0; i<octaves; i++) {
		temp = Noise3( point ) * exponent_array[i];
		if ( temp < 0.0 )
			temp = -temp;
		value += temp;
		point.x *= lacunarity;
		point.y *= lacunarity;
		point.z *= lacunarity;
	} /* for */

	/* take care of remainder in "octaves" */
	remainder = octaves - (int)octaves;
	if ( remainder ) {
		/* "i" and spatial freq. are preset in loop above */
		temp = remainder * Noise3( point ) * exponent_array[i];
		if ( temp < 0.0 )
			temp = -temp;
		value +=  temp;
	}

	return( value );

} /* turbulence() */




//==================
// spline.c
/* Coefficients of basis matrix. */
#define CR00     -0.5
#define CR01      1.5
#define CR02     -1.5
#define CR03      0.5
#define CR10      1.0
#define CR11     -2.5
#define CR12      2.0
#define CR13     -0.5
#define CR20     -0.5
#define CR21      0.0
#define CR22      0.5
#define CR23      0.0
#define CR30      0.0
#define CR31      1.0
#define CR32      0.0
#define CR33      0.0

gfloat
spline(gfloat x, int nknots, const gfloat *knot)
{
    int span;
    int nspans = nknots - 3;
    gfloat c0, c1, c2, c3;	/* coefficients of the cubic.*/

    if (nspans < 1) {  /* illegal */
        fprintf(stderr, "Spline has too few knots.\n");
        return 0;
    }

    /* Find the appropriate 4-point span of the spline. */
    x = CLAMP(x, 0, 1) * nspans;
    span = (int) x;
    if (span >= nknots - 3)
        span = nknots - 3;
    x -= span;
    knot += span;

    /* Evaluate the span cubic at x using Horner's rule. */
    c3 = CR00*knot[0] + CR01*knot[1]
       + CR02*knot[2] + CR03*knot[3];
    c2 = CR10*knot[0] + CR11*knot[1]
       + CR12*knot[2] + CR13*knot[3];
    c1 = CR20*knot[0] + CR21*knot[1]
       + CR22*knot[2] + CR23*knot[3];
    c0 = CR30*knot[0] + CR31*knot[1]
       + CR32*knot[2] + CR33*knot[3];

    return ((c3*x + c2)*x + c1)*x + c0;
}
