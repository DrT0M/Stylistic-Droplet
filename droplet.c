//	Stylistic Droplet

//	Draw a stylistic droplet shape profile using varying curvature
//	Curvature is the change in curve direction over a small distance travelled (e.g. angle per unit length in rad/m)
//	Start at the origin with unit curvature and direction
//	Change direction at each step with the same amount to complete course

//Compile:
//	cc droplet.c -o ~/droplet -lm

//Run with the defaults -90 10 64 (hanging by a thread):
//	~/droplet > droplet.scad

//Run with specific 1, 2, or 3 parameters:
//	~/droplet 0 46.5 64	//start drip
//	~/droplet 0 52 64	//mid drip
//	~/droplet 0 999 64	//close to spherical
//	~/droplet 0 43 74	//vase color("sandybrown")

//  tab stop @ 8
/*****************************************************************************/
//	Stylistic Droplet is licensed under
//	MIT License
//
//	Copyright (c) 2025 DrT0M
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//	
//	The above copyright notice and this permission notice shall be included in all
//	copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//	SOFTWARE.
//
/*****************************************************************************/
#include <stdio.h>		//	fprintf
#include <stdlib.h>		//	atoi atof calloc
#include <math.h>		//	M_PI sin cos
const	double	PI = M_PI;
const	double	segs = 360/5;		//		segments to draw complete circle of unit curvature
const	double	step = 2*PI/segs;	//		small distance travelled per segment
const	double	k0 = 1.;		//		initial curvature
	double	d0 = -90;		//	argv[1]	initial direction (degree: -90 due south; 0 due east)
	double	i0 = 10.;		//	argv[2] iterations to reduce unit curvature to zero
	int	iter = 64;		//	argv[3] draw this number of segments (max iterations)

/*****************************************************************************/
int main(int argc, char*argv[])
{
	if( argc > 1 )	d0   = atof( argv[1] );
	if( argc > 2 )	i0   = atof( argv[2] );
	if( argc > 3 )	iter = atoi( argv[3] );

	double*k = (double*)calloc( 1+iter, sizeof(double) );	//curvature
	double*a = (double*)calloc( 1+iter, sizeof(double) );	//change of direction (angle)
	double*A = (double*)calloc( 1+iter, sizeof(double) );	A[0] = d0*PI/180.;
	double*Z = (double*)calloc( 1+iter, sizeof(double) );	Z[0] = 0;
	double*Y = (double*)calloc( 1+iter, sizeof(double) );	Y[0] = 0;

	for( int i = 0 ; i < iter ; i++ )
	{
		k[i] = k0 + (0-k0)*i/(i0-0);		//curvature: initially k0 reduced to zero (straight ahead) over i0 iterations, and beyond (turning the other direction)
		a[i] = k[i] * step;			//change in angle
		A[ i+1 ] = A[i] + a[i];			//angle
		Y[ i+1 ] = Y[i] + step*cos( A[i] );
		Z[ i+1 ] = Z[i] + step*sin( A[i] );
	}

	/*****************************************************************************/
	FILE*F = stdout;
	(void)fprintf(F,"//%s %g %g %d\n", "droplet", d0, i0, iter);
	(void)fprintf(F,"rotate_extrude($fn=60)\n");
	(void)fprintf(F,"polygon(\n[[0,%g]\n",Z[0]);
	double	Z_tip = Z[0];
	for( int i = 1 ; i <= iter ; i++ )
	{
	    if( Y[i] >= 0.0 )
	    {
		(void)fprintf(F,",[%g\t,%g\t]\n", Y[i], Z[i]);
		Z_tip = Z[i];
	    }
	    else
		break;	
	}
	(void)fprintf(F,",[0,%g]\n", Z_tip );
	(void)fprintf(F,"]);\n");
	(void)fclose( F );

	/*****************************************************************************/
	(void)	free(Y);
	(void)	free(Z);
	(void)	free(A);
	(void)	free(a);
	(void)	free(k);
}
