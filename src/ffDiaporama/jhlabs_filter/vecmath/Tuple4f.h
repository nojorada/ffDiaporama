#ifndef __Tuple4f__
#define __Tuple4f__
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
class Tuple4f 
{
	public:
    float x, y, z, w;

	Tuple4f() {
		new (this) Tuple4f( 0, 0, 0, 0 );
	}
   virtual ~Tuple4f() {}
	
	Tuple4f( float x[4] ) {
		this->x = x[0];
		this->y = x[1];
		this->z = x[2];
		this->w = x[2];
	}

	Tuple4f( float x, float y, float z, float w ) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	Tuple4f( const Tuple4f &t ) {
		this->x = t.x;
		this->y = t.y;
		this->z = t.z;
		this->w = t.w;
	}

	void absolute() {
		x = qFabs(x);
		y = qFabs(y);
		z = qFabs(z);
		w = qFabs(w);
	}

	virtual void absolute( const Tuple4f &t ) {
		x = qFabs(t.x);
		y = qFabs(t.y);
		z = qFabs(t.z);
		w = qFabs(t.w);
	}

	virtual void clamp( float min, float max ) {
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
		if ( w < min )
			w = min;
		else if ( w > max )
			w = max;
	}

	virtual void set( float x, float y, float z, float w ) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	virtual void set( float x[4] ) {
		this->x = x[0];
		this->y = x[1];
		this->z = x[2];
		this->w = x[2];
	}

	virtual void set( const Tuple4f &t ) {
		x = t.x;
		y = t.y;
		z = t.z;
		w = t.w;
	}

	virtual void set( Tuple4f &t ) {
		x = t.x;
		y = t.y;
		z = t.z;
		w = t.w;
	}


	virtual void get( Tuple4f &t ) {
		t.x = x;
		t.y = y;
		t.z = z;
		t.w = w;
	}

	virtual void get( float *t ) {
		t[0] = x;
		t[1] = y;
		t[2] = z;
		t[3] = w;
	}

	void negate() {
		x = -x;
		y = -y;
		z = -z;
		w = -w;
	}

	void negate( const Tuple4f &t ) {
		x = -t.x;
		y = -t.y;
		z = -t.z;
		w = -t.w;
	}

	void interpolate( const Tuple4f &t, float alpha ) {
		float a = 1-alpha;
		x = a*x + alpha*t.x;
		y = a*y + alpha*t.y;
		z = a*z + alpha*t.z;
		w = a*w + alpha*t.w;
	}

	void scale( float s ) {
		x *= s;
		y *= s;
		z *= s;
		w *= s;
	}

	void add( const Tuple4f &t ) {
		x += t.x;
		y += t.y;
		z += t.z;
		w += t.w;
	}

	void add( const Tuple4f &t1, const Tuple4f &t2 ) {
		x = t1.x+t2.x;
		y = t1.y+t2.y;
		z = t1.z+t2.z;
		w = t1.w+t2.w;
	}

	void sub( const Tuple4f &t ) {
		x -= t.x;
		y -= t.y;
		z -= t.z;
		w -= t.w;
	}

	void sub( const Tuple4f &t1, const Tuple4f &t2 ) {
		x = t1.x-t2.x;
		y = t1.y-t2.y;
		z = t1.z-t2.z;
		w = t1.w-t2.w;
	}

	//String toString() {
	//	return "["+x+", "+y+", "+z+", "+w+"]";
	//}
	
};
#endif // __Tuple4f__
