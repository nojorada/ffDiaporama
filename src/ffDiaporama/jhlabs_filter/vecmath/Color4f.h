#ifndef __Color4f__
#define __Color4f__
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
#include <QColor>
/**
 * Vector math package, converted to look similar to javax.vecmath.
 */
class Color4f : public Tuple4f 
{

	public:
    Color4f() :Tuple4f() {
		new (this) Color4f( 0, 0, 0, 0 );
	}
   virtual ~Color4f(){}
	
	Color4f( float x[4] ) : Tuple4f(x){}
	//	this->x = x[0];
	//	this->y = x[1];
	//	this->z = x[2];
	//	this->w = x[3];
	//}

	Color4f( float x, float y, float z, float w ) : Tuple4f(x,y,z,w) {}

	Color4f( const Color4f &t ) :Tuple4f(t) {}
	//	this->x = t.x;
	//	this->y = t.y;
	//	this->z = t.z;
	//	this->w = t.w;
	//}

	//Color4f( const Tuple4f &t ) {
	//	this->x = t.x;
	//	this->y = t.y;
	//	this->z = t.z;
	//	this->w = t.w;
	//}

   Color4f(const QColor &c)
   {
      x = c.redF();
      y = c.greenF();
      z = c.blueF();
      w = c.alphaF();
      //c.getRgbF(&x,&y,&z,&w);
   }
   void set( float x, float y, float z, float w )  { Tuple4f::set(x,y,z,w); }
   void set(const Color4f &c) { Tuple4f::set(c); }
//   void set(Color4f c) { Tuple4f::set(c); }
   void set(const QColor &c)
   {
      x = c.redF();
      y = c.greenF();
      z = c.blueF();
      w = c.alphaF();
      //c.getRgbF(&x,&y,&z,&w);
   }

   Color4f &operator=(int rgb)
   {
      QColor c(rgb);
      x = c.redF();
      y = c.greenF();
      z = c.blueF();
      w = c.alphaF();
      //c.getRgbF(&x,&y,&z,&w);
      return *this;
   }

   operator int() const
   {
      return QColor::fromRgbF(x, y, z, w).rgba();
      //return (int(w) << 24) | (int(x) << 16) | (int(y) << 8) | int(z);
   }
   Color4f &operator=(const Color4f &c)
   {
      set(c.x, c.y, c.z, c.w);
      return *this;
   }
   //Color4f &operator=(Color4f c)
   //{
   //   set(c.x, c.y, c.z, c.w);
   //   return *this;
   //}
   //void set(float r, float g, float b, float alpha)
   //{
   //   Tuple4f::set(r,g,b,alpha);
   ////   x = r;
   ////   y = g;
   ////   z = b; 
   ////   w = alpha;
   //}
  // void set(const Color4f &t)
  // {
		//this->x = t.x;
		//this->y = t.y;
		//this->z = t.z;
		//this->w = t.w;
  // }
	//Color4f( Color c ) {
	//	set( c );
	//}

	//void set( Color c ) {
	//	set( c.getRGBComponents( null ) );
	//}

	//Color get() {
	//	return new Color( x, y, z, w );
	//}
};
#endif // __Color4f__
