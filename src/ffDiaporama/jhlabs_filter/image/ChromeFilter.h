#ifndef __ChromeFilter__
#define __ChromeFilter__
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

#include "LightFilter.h"
#include "TransferFilter.h"

/**
 * A filter which simulates chrome.
 */
class ChromeTaransferFilter : public TransferFilter
{
   private:
   float amount;
	float exposure;
   public:
      ChromeTaransferFilter(float a, float e) : TransferFilter() { amount = a; exposure = e; }
	protected:
      float transferFunction( float v ) 
      {
	   	v += amount * (float)qSin( v * 2 * ImageMath::PI );
		   return 1 - (float)exp(-v * exposure);
	}
};
class ChromeFilter : public LightFilter 
{
	private:
   float amount;
	float exposure;

	/**
	 * Set the amount of effect.
	 * @param amount the amount
     * @min-value 0
     * @max-value 1
     * @see #getAmount
	 */
	public:
      ChromeFilter() : LightFilter()
      {
         amount = 0.5f;
	      exposure = 1.0f;
      }

    void setAmount(float amount) {
		this->amount = amount;
	}

	/**
	 * Get the amount of chrome.
	 * @return the amount
     * @see #setAmount
	 */
	float getAmount() {
		return amount;
	}

	/**
	 * Set the exppsure of the effect.
	 * @param exposure the exposure
     * @min-value 0
     * @max-value 1
     * @see #getExposure
	 */
	void setExposure(float exposure) {
		this->exposure = exposure;
	}
	
	/**
	 * Get the exppsure of the effect.
	 * @return the exposure
     * @see #setExposure
	 */
	float getExposure() {
		return exposure;
	}

    QImage *filter( const QImage &src, QImage *dst ) 
    {
      setColorSource( LightFilter::COLORS_CONSTANT ); 
		dst = LightFilter::filter( src, dst );
      ChromeTaransferFilter tf(amount, exposure);
		//TransferFilter tf = new TransferFilter() 
  //    {
		//	protected:
  //        float transferFunction( float v ) 
  //        {
		//		v += amount * (float)qSin( v * 2 * ImageMath::PI );
		//		return 1 - (float)exp(-v * exposure);
		//	}
		//};
      return tf.filter( *dst, dst );
    }

	//public String toString() {
	//	return "Effects/Chrome...";
	//}
};
#endif // __ChromeFilter__


