#ifndef __TwirlFilter__
#define __TwirlFilter__
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
 * A Filter which distorts an image by twisting it from the centre out.
 * The twisting is centred at the centre of the image and extends out to the smallest of
 * the width and height. Pixels outside this radius are unaffected.
 */
class TwirlFilter :public TransformFilter {
	
	private:
   qreal angle;
	qreal centreX;
	qreal centreY;
	qreal radius;

	qreal radius2;
	qreal icentreX;
	qreal icentreY;

	/**
	 * Construct a TwirlFilter with no distortion.
	 */
	public:
    TwirlFilter() {
      angle = 0;
      centreX = 0.5f;
      centreY = 0.5f;
      radius = 100;

      radius2 = 0;
		setEdgeAction( CLAMP );
	}

	/**
	 * Set the angle of twirl in radians. 0 means no distortion.
	 * @param angle the angle of twirl. This is the angle by which pixels at the nearest edge of the image will move.
     * @see #getAngle
	 */
	void setAngle(qreal angle) {
		this->angle = angle;
	}
	
	/**
	 * Get the angle of twist.
	 * @return the angle in radians.
     * @see #setAngle
	 */
	qreal getAngle() {
		return angle;
	}
	
	/**
	 * Set the centre of the effect in the X direction as a proportion of the image size.
	 * @param centreX the center
     * @see #getCentreX
	 */
	void setCentreX( qreal centreX ) {
		this->centreX = centreX;
	}

	/**
	 * Get the centre of the effect in the X direction as a proportion of the image size.
	 * @return the center
     * @see #setCentreX
	 */
	qreal getCentreX() {
		return centreX;
	}
	
	/**
	 * Set the centre of the effect in the Y direction as a proportion of the image size.
	 * @param centreY the center
     * @see #getCentreY
	 */
	void setCentreY( qreal centreY ) {
		this->centreY = centreY;
	}

	/**
	 * Get the centre of the effect in the Y direction as a proportion of the image size.
	 * @return the center
     * @see #setCentreY
	 */
	qreal getCentreY() {
		return centreY;
	}
	
	/**
	 * Set the centre of the effect as a proportion of the image size.
	 * @param centre the center
     * @see #getCentre
	 */
	void setCentre( QPointF centre ) {
		this->centreX = (qreal)centre.x();
		this->centreY = (qreal)centre.y();
	}

	/**
	 * Get the centre of the effect as a proportion of the image size.
	 * @return the center
     * @see #setCentre
	 */
	QPointF getCentre() {
		return QPointF( centreX, centreY );
	}
	
	/**
	 * Set the radius of the effect.
	 * @param radius the radius
     * @min-value 0
     * @see #getRadius
	 */
	void setRadius(qreal radius) {
		this->radius = radius;
	}

	/**
	 * Get the radius of the effect.
	 * @return the radius
     * @see #setRadius
	 */
	qreal getRadius() {
		return radius;
	}

    QImage *filter( const QImage &src, QImage *dst ) {
		icentreX = src.width() * centreX;
		icentreY = src.height() * centreY;
		if ( radius == 0 )
			radius = qMin(icentreX, icentreY);
		radius2 = radius*radius;
		return TransformFilter::filter( src, dst );
	}
	
	protected:
    void transformInverse(int x, int y, qreal out[2]) {
		qreal dx = x-icentreX;
		qreal dy = y-icentreY;
		qreal distance = dx*dx + dy*dy;
		if (distance > radius2) {
			out[0] = x;
			out[1] = y;
		} else {
			distance = (qreal)qSqrt(distance);
			qreal a = (qreal)qAtan2(dy, dx) + angle * (radius-distance) / radius;
			out[0] = icentreX + distance*(qreal)qCos(a);
			out[1] = icentreY + distance*(qreal)qSin(a);
		}
	}

	//String toString() {
	//	return "Distort/Twirl...";
	//}

};
#endif // __TwirlFilter__

