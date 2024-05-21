#ifndef __Tuple3f__
#define __Tuple3f__
/*
Copyright 2006 Jerry Huxtable

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <QtMath>
/**
 * Vector math package, converted to look similar to javax.vecmath.
 */
class Tuple3f 
{
	public:
    float x, y, z;

	Tuple3f() {
		new (this) Tuple3f( 0, 0, 0 );
	}
	
	Tuple3f( float x[3] ) {
		this->x = x[0];
		this->y = x[1];
		this->z = x[2];
	}

	Tuple3f( float x, float y, float z ) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Tuple3f( const Tuple3f &t ) {
		this->x = t.x;
		this->y = t.y;
		this->z = t.z;
	}

	void absolute() {
		x = qFabs(x);
		y = qFabs(y);
		z = qFabs(z);
	}

	void absolute( const Tuple3f &t ) {
		x = qFabs(t.x);
		y = qFabs(t.y);
		z = qFabs(t.z);
	}

	void clamp( float min, float max ) {
		if ( x < min )
			x = min;
		else if ( x > max )
			x = max;
		if ( y < min )
			y = min;
		else if ( y > max )
			y = max;
		if ( z < min )
			z = min;
		else if ( z > max )
			z = max;
	}

	void set( float x, float y, float z ) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	void set( float x[3] ) {
		this->x = x[0];
		this->y = x[1];
		this->z = x[2];
	}

	void set( const Tuple3f &t ) {
		x = t.x;
		y = t.y;
		z = t.z;
	}

	void get( Tuple3f &t ) {
		t.x = x;
		t.y = y;
		t.z = z;
	}

	void get( float t[3] ) {
		t[0] = x;
		t[1] = y;
		t[2] = z;
	}

	void negate() {
		x = -x;
		y = -y;
		z = -z;
	}

	void negate( Tuple3f &t ) {
		x = -t.x;
		y = -t.y;
		z = -t.z;
	}

	void interpolate( const Tuple3f &t, float alpha ) {
		float a = 1-alpha;
		x = a*x + alpha*t.x;
		y = a*y + alpha*t.y;
		z = a*z + alpha*t.z;
	}

	void scale( float s ) {
		x *= s;
		y *= s;
		z *= s;
	}

	void add( const Tuple3f &t ) {
		x += t.x;
		y += t.y;
		z += t.z;
	}

	void add( const Tuple3f &t1, const Tuple3f &t2 ) {
		x = t1.x+t2.x;
		y = t1.y+t2.y;
		z = t1.z+t2.z;
	}

	void sub( const Tuple3f &t ) {
		x -= t.x;
		y -= t.y;
		z -= t.z;
	}

	void sub( const Tuple3f &t1, const Tuple3f &t2 ) {
		x = t1.x-t2.x;
		y = t1.y-t2.y;
		z = t1.z-t2.z;
	}

	void scaleAdd( float s, const Tuple3f &t ) {
		x += s*t.x;
		y += s*t.y;
		z += s*t.z;
	}
	
	void scaleAdd( float s, const Tuple3f &t1, const Tuple3f &t2 ) {
		x = s*t1.x + t2.x;
		y = s*t1.y + t2.y;
		z = s*t1.z + t2.z;
	}
	
	//String toString() {
	//	return "["+x+", "+y+", "+z+"]";
	//}
	
};
#endif // Tuple3f

