#ifndef __Point3f__
#define __Point3f__
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

#include "Tuple3f.h"
/**
 * Vector math package, converted to look similar to javax.vecmath.
 */
class Point3f : Tuple3f 
{

	public:
    Point3f() {
		new (this) Point3f( 0, 0, 0 );
	}
	
	Point3f( float x[3] ) {
		this->x = x[0];
		this->y = x[1];
		this->z = x[2];
	}

	Point3f( float x, float y, float z ) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Point3f( const Point3f &t ) {
		this->x = t.x;
		this->y = t.y;
		this->z = t.z;
	}

	Point3f( const Tuple3f &t ) {
		this->x = t.x;
		this->y = t.y;
		this->z = t.z;
	}

	float distanceL1( const Point3f &p ) {
		return qFabs(x-p.x) + qFabs(y-p.y) + qFabs(z-p.z);
	}

	float distanceSquared( const Point3f &p ) {
		float dx = x-p.x;
		float dy = y-p.y;
		float dz = z-p.z;
		return dx*dx+dy*dy+dz*dz;
	}

	float distance( const Point3f &p ) {
		float dx = x-p.x;
		float dy = y-p.y;
		float dz = z-p.z;
		return (float)qSqrt( dx*dx+dy*dy+dz*dz );
	}

};
#endif // __Point3f__
