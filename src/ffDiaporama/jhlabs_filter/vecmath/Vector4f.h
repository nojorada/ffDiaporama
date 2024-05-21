#ifndef __Vector4f__
#define __Vector4f__
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

#include "Tuple4f.h"

/**
 * Vector math package, converted to look similar to javax.vecmath.
 */
class Vector4f : Tuple4f 
{

	public:
    Vector4f() {
		new (this)Vector4f( 0, 0, 0, 0 );
	}
	
	Vector4f( float x[4] ) {
		this->x = x[0];
		this->y = x[1];
		this->z = x[2];
		this->w = x[2];
	}

	Vector4f( float x, float y, float z, float w ) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	Vector4f( const Vector4f &t ) {
		x = t.x;
		y = t.y;
		z = t.z;
		w = t.w;
	}

	Vector4f( const Tuple4f &t ) {
		x = t.x;
		y = t.y;
		z = t.z;
		w = t.w;
	}

	float dot( const Vector4f &v ) {
		return v.x * x + v.y * y + v.z * z + v.w * w;
	}

	float length() const {
		return (float)qSqrt( x*x+y*y+z*z+w*w );
	}

	void normalize() {
		float d = 1.0f/( x*x+y*y+z*z+w*w );
		x *= d;
		y *= d;
		z *= d;
		w *= d;
	}

};
#endif // __Vector4f__