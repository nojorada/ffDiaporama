#ifndef __PointFilter__
#define __PointFilter__
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

#include "AbstractBufferedImageOp.h"
/**
 * An abstract superclass for point filters. The interface is the same as the old RGBImageFilter.
 */
class PointFilter : public AbstractBufferedImageOp 
{

	protected:
    bool canFilterIndexColorModel;

    public:
    PointFilter() 
    {
      canFilterIndexColorModel = false;
    }
    QImage *filter( const QImage &src, QImage *dst ) 
    {
       int width = src.width();
       int height = src.height();

       if ( dst == 0 )
          dst = createCompatibleDestImage( src );

       setDimensions( width, height);

       int *inPixels = new int[width];
       for ( int y = 0; y < height; y++ ) 
       {
          memcpy(inPixels, src.scanLine(y),src.bytesPerLine());
          for ( int x = 0; x < width; x++ )
             inPixels[x] = filterRGB( x, y, inPixels[x] );
          memcpy(dst->scanLine(y),inPixels, src.bytesPerLine());
       }
       delete [] inPixels;

       return dst;
    }

	void setDimensions(int width, int height) {	}

	virtual int filterRGB(int x, int y, int rgb) = 0;
};
#endif // __PointFilter__
