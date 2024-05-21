#ifndef __AbstractBufferedImageOp__
#define __AbstractBufferedImageOp__
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


/**
 * A convenience class which implements those methods of BufferedImageOp which are rarely changed.
 */
 #include <QImage>
class AbstractBufferedImageOp /*implements BufferedImageOp, Cloneable */
{
   public:
   QImage *createCompatibleDestImage(const QImage &src/*, ColorModel dstCM*/) 
   {
      return new QImage(src.width(), src.height(), src.format());
   }

   QRect getBounds2D( QImage src ) 
   {
      return QRect(src.rect());
   }

   QPointF *getPoint2D( QPointF srcPt, QPointF *dstPt ) 
   {
      if ( dstPt == 0 )
         dstPt = new QPointF();
      *dstPt = srcPt;
      return dstPt;
   }

    //RenderingHints getRenderingHints() {
    //    return NULL;
    //}

	/**
	 * A convenience method for getting ARGB pixels from an image. This tries to avoid the performance
	 * penalty of BufferedImage.getRGB unmanaging the image.
     * @param image   a BufferedImage object
     * @param x       the left edge of the pixel block
     * @param y       the right edge of the pixel block
     * @param width   the width of the pixel arry
     * @param height  the height of the pixel arry
     * @param pixels  the array to hold the returned pixels. May be null.
     * @return the pixels
     * @see #setRGB
     */
	//int[] getRGB( BufferedImage image, int x, int y, int width, int height, int[] pixels ) 
 //  {
	//	int type = image.getType();
	//	if ( type == BufferedImage.TYPE_INT_ARGB || type == BufferedImage.TYPE_INT_RGB )
	//		return (int [])image.getRaster().getDataElements( x, y, width, height, pixels );
	//	return image.getRGB( x, y, width, height, pixels, 0, width );
 //   }

	/**
	 * A convenience method for setting ARGB pixels in an image. This tries to avoid the performance
	 * penalty of BufferedImage.setRGB unmanaging the image.
     * @param image   a BufferedImage object
     * @param x       the left edge of the pixel block
     * @param y       the right edge of the pixel block
     * @param width   the width of the pixel arry
     * @param height  the height of the pixel arry
     * @param pixels  the array of pixels to set
     * @see #getRGB
	 */
	//void setRGB( BufferedImage image, int x, int y, int width, int height, int[] pixels ) 
 //  {
	//	int type = image.getType();
	//	if ( type == BufferedImage.TYPE_INT_ARGB || type == BufferedImage.TYPE_INT_RGB )
	//		image.getRaster().setDataElements( x, y, width, height, pixels );
	//	else
	//		image.setRGB( x, y, width, height, pixels, 0, width );
 //   }

	//public Object clone() {
	//	try {
	//		return super.clone();
	//	}
	//	catch ( CloneNotSupportedException e ) {
	//		return null;
	//	}
	//}
};
#endif //__AbstractBufferedImageOp__
