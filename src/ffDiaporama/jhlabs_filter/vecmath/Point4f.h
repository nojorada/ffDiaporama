#ifndef __Point4f__
#define __Point4f__
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
class Point4f : Tuple4f {

	public:
    Point4f() {
		new (this) Point4f( 0, 0, 0, 0 );
	}
	
	Point4f( float x[4] ) {
		this->x = x[0];
		this->y = x[1];
		this->z = x[2];
		this->w = x[3];
	}

	Point4f( float x, float y, float z, float w ) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	Point4f( const Point4f &t ) {
		this->x = t.x;
		this->y = t.y;
		this->z = t.z;
		this->w = t.w;
	}

	Point4f( const Tuple4f &t ) {
		this->x = t.x;
		this->y = t.y;
		this->z = t.z;
		this->w = t.w;
	}

	float distanceL1( const Point4f &p ) {
		return qFabs(x-p.x) + qFabs(y-p.y) + qFabs(z-p.z) + qFabs(w-p.w);
	}

	float distanceSquared( const Point4f &p ) {
		float dx = x-p.x;
		float dy = y-p.y;
		float dz = z-p.z;
		float dw = w-p.w;
		return dx*dx+dy*dy+dz*dz+dw*dw;
	}

	float distance( const Point4f &p ) {
		float dx = x-p.x;
		float dy = y-p.y;
		float dz = z-p.z;
		float dw = w-p.w;
		return (float)qSqrt( dx*dx+dy*dy+dz*dz+dw*dw );
	}

};
#endif // __Point4f__
