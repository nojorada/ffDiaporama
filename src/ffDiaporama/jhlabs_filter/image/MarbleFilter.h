#ifndef __MarbleFilter__
#define __MarbleFilter__

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
#include "TransformFilter.h"
#include "PixelUtils.h"
/**
 * This filter applies a marbling effect to an image, displacing pixels by random amounts.
 */
class MarbleFilter : public TransformFilter 
{
	private:
      float sinTable[256], cosTable[256];
	   float xScale;
	   float yScale;
	   float amount;
	   float turbulence;
	
	public:
   MarbleFilter() 
   {
		setEdgeAction(TransformFilter::CLAMP);
	   xScale = 30;
	   yScale = 30;
	   amount = 0.5;
	   turbulence = 0.5;
	}
	
	/**
     * Set the X scale of the effect.
     * @param xScale the scale.
     * @see #getXScale
     */
	void setXScale(float xScale) {
		this->xScale = xScale;
	}

	/**
     * Get the X scale of the effect.
     * @return the scale.
     * @see #setXScale
     */
	float getXScale() {
		return xScale;
	}

	/**
     * Set the Y scale of the effect.
     * @param yScale the scale.
     * @see #getYScale
     */
	void setYScale(float yScale) {
		this->yScale = yScale;
	}

	/**
     * Get the Y scale of the effect.
     * @return the scale.
     * @see #setYScale
     */
	float getYScale() {
		return yScale;
	}

	/**
	 * Set the amount of effect.
	 * @param amount the amount
     * @min-value 0
     * @max-value 1
     * @see #getAmount
	 */
	void setAmount(float amount) {
		this->amount = amount;
	}

	/**
	 * Get the amount of effect.
	 * @return the amount
     * @see #setAmount
	 */
	float getAmount() {
		return amount;
	}

	/**
     * Specifies the turbulence of the effect.
     * @param turbulence the turbulence of the effect.
     * @min-value 0
     * @max-value 1
     * @see #getTurbulence
     */
	void setTurbulence(float turbulence) {
		this->turbulence = turbulence;
	}

	/**
     * Returns the turbulence of the effect.
     * @return the turbulence of the effect.
     * @see #setTurbulence
     */
	float getTurbulence() {
		return turbulence;
	}

	private:
    void initialize() 
    {
		for (int i = 0; i < 256; i++) 
      {
			float angle = ImageMath::TWO_PI*i/256.0*turbulence;
			sinTable[i] = (float)(-yScale*sin(angle));
			cosTable[i] = (float)(yScale*cos(angle));
		}
	}

	int displacementMap(int x, int y) 
   {
		return PixelUtils::clamp((int)(127 * (1+Noise::noise2(x / xScale, y / xScale))));
	}
	
	protected:
    void transformInverse(int x, int y, float out[2]) {
		int displacement = displacementMap(x, y);
		out[0] = x + sinTable[displacement];
		out[1] = y + cosTable[displacement];
	}

    public:
     QImage *filter(const QImage &src, QImage *dst ) 
     {
		initialize();
		return TransformFilter::filter( src, dst );
	}

	//public String toString() {
	//	return "Distort/Marble...";
	//}
};
#endif // __MarbleFilter__

