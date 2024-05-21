#ifndef __Function1D__
#define __Function1D__
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

class Function1D 
{
   public:
	virtual float evaluate(float v) = 0;
};

class Function2D 
{
	public:
      virtual ~Function2D() {}
      virtual float evaluate(float x, float y) = 0;
};

class Function3D 
{
	public:
      virtual float evaluate(float x, float y, float z) = 0;
};

class BinaryFunction 
{
	public:
      bool isBlack(int rgb);
} ;

class CompositeFunction1D : public Function1D 
{
	Function1D *f1, *f2;
	
	public:
   CompositeFunction1D(Function1D *f1, Function1D *f2) 
   {
		this->f1 = f1;
		this->f2 = f2;
	}
	
	float evaluate(float v) {
		return f1->evaluate(f2->evaluate(v));
	}
};
 
 class CompoundFunction2D : public Function2D // abstract!!
 {
 	protected:
      Function2D *basis;
	
	public:
      CompoundFunction2D(Function2D *basis) { this->basis = basis; }
	   void setBasis(Function2D *basis) { this->basis = basis; }
	   Function2D *getBasis() { return basis; }
};
 
class BlackFunction : public BinaryFunction 
{
	public:
   bool isBlack(int rgb) 
   {
		return rgb == 0xff000000;
	}
};

class FractalSumFunction : public CompoundFunction2D 
{
	float octaves;
	
	public:
   FractalSumFunction(Function2D *basis) : CompoundFunction2D(basis){ octaves = 1.0f; }
	
   float evaluate(float x, float y) 
   {
		float t = 0.0f;

		for (float f = 1.0f; f <= octaves; f *= 2)
			t += basis->evaluate(f * x, f * y) / f;
		return t;
	}
};

#endif // __Function1D__

