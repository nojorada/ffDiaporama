#ifndef __ImageFunction2D__
#define __ImageFunction2D__
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

#include "FunctionXD.h"
#include "ImageMath.h"
#include "PixelUtils.h"
class ImageFunction2D : public Function2D 
{

   public:
	static const int ZERO = 0;
	static const int CLAMP = 1;
	static const int WRAP = 2;
	
	protected:
   const int *pixels;
	int width;
	int height;
	int edgeAction;
	bool alpha;
	
   void _init()
   {
      edgeAction = ZERO;
	   alpha = false;
   }

	public:
 //   ImageFunction2D(const QImage &image) {
 //     _init();
	//	new (this) ImageFunction2D(image, false);
	//}
	//
	//ImageFunction2D(const QImage &image, bool alpha) {
 //     _init();
	//	new (this) ImageFunction2D(image, ZERO, alpha);
	//}
	//
	ImageFunction2D(const QImage &image, int edgeAction = ZERO, bool alpha = false) 
   {
      init((const int *)image.bits(), image.width(), image.height(), edgeAction, alpha);
		//init( getRGB( image, 0, 0, image.getWidth(), image.getHeight(), null), image.getWidth(), image.getHeight(), edgeAction, alpha);
	}
	
	ImageFunction2D(const int *pixels, int width, int height, int edgeAction, bool alpha) 
   {
		init(pixels, width, height, edgeAction, alpha);
	}
	
 	//ImageFunction2D(QImage image) {
  //    _init();
		//new (this) ImageFunction2D( image, ZERO, false );
 	//}
	
	//ImageFunction2D(QImage image, int edgeAction, bool alpha) 
 //  {
 //		PixelGrabber pg = new PixelGrabber(image, 0, 0, -1, -1, null, 0, -1);
 //		try {
 //			pg.grabPixels();
 //		} catch (InterruptedException e) {
 //			throw new RuntimeException("interrupted waiting for pixels!");
 //		}
 //		if ((pg.status() & ImageObserver.ABORT) != 0) {
 //			throw new RuntimeException("image fetch aborted");
 //		}
 //		init((int[])pg.getPixels(), pg.getWidth(), pg.getHeight(), edgeAction, alpha);
 // 	}

	/**
	 * A convenience method for getting ARGB pixels from an image. This tries to avoid the performance
	 * penalty of BufferedImage.getRGB unmanaging the image.
	 */
	//int[] getRGB( BufferedImage image, int x, int y, int width, int height, int[] pixels ) {
	//	int type = image.getType();
	//	if ( type == BufferedImage.TYPE_INT_ARGB || type == BufferedImage.TYPE_INT_RGB )
	//		return (int [])image.getRaster().getDataElements( x, y, width, height, pixels );
	//	return image.getRGB( x, y, width, height, pixels, 0, width );
 //   }

	void init(const int *pixels, int width, int height, int edgeAction, bool alpha) {
		this->pixels = pixels;
		this->width = width;
		this->height = height;
		this->edgeAction = edgeAction;
		this->alpha = alpha;
	}
	
	float evaluate(float x, float y) {
		int ix = (int)x;
		int iy = (int)y;
		if (edgeAction == WRAP) {
			ix = ImageMath::mod(ix, width);
			iy = ImageMath::mod(iy, height);
		} else if (ix < 0 || iy < 0 || ix >= width || iy >= height) {
			if (edgeAction == ZERO)
				return 0;
			if (ix < 0)
				ix = 0;
			else if (ix >= width)
				ix = width-1;
			if (iy < 0)
				iy = 0;
			else if (iy >= height)
				iy = height-1;
		}
		return alpha ? ((pixels[iy*width+ix] >> 24) & 0xff) / 255.0f : PixelUtils::brightness(pixels[iy*width+ix]) / 255.0f;
	}
	
	void setEdgeAction(int edgeAction) {
		this->edgeAction = edgeAction;
	}

	int getEdgeAction() {
		return edgeAction;
	}

	int getWidth() {
		return width;
	}
	
	int getHeight() {
		return height;
	}
	
	const int *getPixels() {
		return pixels;
	}
};
#endif // ImageFunction2D


