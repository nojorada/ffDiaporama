#ifndef __KaleidoscopeFilter__
#define __KaleidoscopeFilter__
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

/**
 * A Filter which produces the effect of looking into a kaleidoscope.
 */
class KaleidoscopeFilter : public TransformFilter 
{

	private:
      float angle;
      float angle2;
      float centreX;
      float centreY;
      int sides;
      float radius;

      float icentreX;
      float icentreY;

	/**
	 * Construct a KaleidoscopeFilter with no distortion.
	 */
	public:
    KaleidoscopeFilter() 
    {
		setEdgeAction( CLAMP );
      angle = 0;
      angle2 = 0;
      centreX = 0.5f;
      centreY = 0.5f;
      sides = 3;
      radius = 0;
	}

	/**
	 * Set the number of sides of the kaleidoscope.
	 * @param sides the number of sides
     * @min-value 2
     * @see #getSides
	 */
	void setSides(int sides) {
		this->sides = sides;
	}

	/**
	 * Get the number of sides of the kaleidoscope.
	 * @return the number of sides
     * @see #setSides
	 */
	int getSides() {
		return sides;
	}

	/**
     * Set the angle of the kaleidoscope.
     * @param angle the angle of the kaleidoscope.
     * @angle
     * @see #getAngle
     */
	void setAngle(float angle) {
		this->angle = angle;
	}
	
	/**
     * Get the angle of the kaleidoscope.
     * @return the angle of the kaleidoscope.
     * @see #setAngle
     */
	float getAngle() {
		return angle;
	}
	
	/**
     * Set the secondary angle of the kaleidoscope.
     * @param angle2 the angle
     * @angle
     * @see #getAngle2
     */
	void setAngle2(float angle2) {
		this->angle2 = angle2;
	}
	
	/**
     * Get the secondary angle of the kaleidoscope.
     * @return the angle
     * @see #setAngle2
     */
	float getAngle2() {
		return angle2;
	}
	
	/**
	 * Set the centre of the effect in the X direction as a proportion of the image size.
	 * @param centreX the center
     * @see #getCentreX
	 */
	void setCentreX( float centreX ) {
		this->centreX = centreX;
	}

	/**
	 * Get the centre of the effect in the X direction as a proportion of the image size.
	 * @return the center
     * @see #setCentreX
	 */
	float getCentreX() {
		return centreX;
	}
	
	/**
	 * Set the centre of the effect in the Y direction as a proportion of the image size.
	 * @param centreY the center
     * @see #getCentreY
	 */
	void setCentreY( float centreY ) {
		this->centreY = centreY;
	}

	/**
	 * Get the centre of the effect in the Y direction as a proportion of the image size.
	 * @return the center
     * @see #setCentreY
	 */
	float getCentreY() {
		return centreY;
	}
	
	/**
	 * Set the centre of the effect as a proportion of the image size.
	 * @param centre the center
     * @see #getCentre
	 */
	void setCentre( QPoint centre ) {
		this->centreX = (float)centre.x();
		this->centreY = (float)centre.y();
	}

	/**
	 * Get the centre of the effect as a proportion of the image size.
	 * @return the center
     * @see #setCentre
	 */
	QPoint getCentre() {
		return QPoint( centreX, centreY );
	}
	
	/**
	 * Set the radius of the effect.
	 * @param radius the radius
     * @min-value 0
     * @see #getRadius
	 */
	void setRadius( float radius ) {
		this->radius = radius;
	}

	/**
	 * Get the radius of the effect.
	 * @return the radius
     * @see #setRadius
	 */
	float getRadius() {
		return radius;
	}
	
    QImage *filter( const QImage &src, QImage *dst ) 
    {
		icentreX = src.width() * centreX;
		icentreY = src.height() * centreY;
		return TransformFilter::filter( src, dst );
	}
	
	protected:
    void transformInverse(int x, int y, float out[2]) 
    {
		double dx = x-icentreX;
		double dy = y-icentreY;
		double r = qSqrt( dx*dx + dy*dy );
		double theta = qAtan2( dy, dx ) - angle - angle2;
		theta = ImageMath::triangle( (float)( theta/ImageMath::PI*sides*.5 ) );
		if ( radius != 0 ) {
			double c = qCos(theta);
			double radiusc = radius/c;
			r = radiusc * ImageMath::triangle( (float)(r/radiusc) );
		}
		theta += angle;

		out[0] = (float)(icentreX + r*qCos(theta));
		out[1] = (float)(icentreY + r*qSin(theta));
	}

	//public String toString() {
	//	return "Distort/Kaleidoscope...";
	//}

};
#endif // __KaleidoscopeFilter__

