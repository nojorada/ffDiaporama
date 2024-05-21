#ifndef __ConvolveFilter__
#define __ConvolveFilter__
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
class Kernel
{
   float *matrix;
   int rows, cols;
   public:
      Kernel()
      {
         matrix = 0;
         rows = 0;
         cols = 0;
      }
      Kernel(int nRows, int nCols)
      {
         rows = nRows; 
         cols = nCols;
         matrix = new float[rows*cols];
      }
      Kernel(int nRows, int nCols, float *srcMatrix)
      {
         rows = nRows; 
         cols = nCols;
         matrix = new float[rows*cols];
         for(int i = 0; i < nRows; i++ )
            for(int j = 0; j < nCols; j++ )
               matrix[i*cols+j] = *srcMatrix++;
      }
      ~Kernel()
      {
         delete matrix;         
      }
      //void setData(int nRows, int nCols, float *srcMatrix)
      void setData(int nCols, int nRows, float *srcMatrix)
      {
         delete matrix;
         matrix = 0;
         rows = nRows; 
         cols = nCols;
         matrix = new float[rows*cols];
         for(int i = 0; i < nRows; i++ )
            for(int j = 0; j < nCols; j++ )
               matrix[i*cols+j] = *srcMatrix++;
      }

      int getHeight() const { return rows; }
      int getWidth() const { return cols; }
      float *getKernelData( float *out = NULL ) const
      {
         if( out )
         {
            for(int i = 0; i < rows; i++ )
               for(int j = 0; j < cols; j++ )
                  out[i*cols+j] = matrix[i*cols+j];
         }
         return matrix;
      }
};

/**
 * A filter which applies a convolution kernel to an image.
 * @author Jerry Huxtable
 */
class ConvolveFilter : public AbstractBufferedImageOp
{
   public:	
    /**
     * Treat pixels off the edge as zero.
     */
	static const int ZERO_EDGES = 0;

    /**
     * Clamp pixels off the edge to the nearest edge.
     */
	static const int CLAMP_EDGES = 1;

    /**
     * Wrap pixels off the edge to the opposite edge.
     */
	static const int WRAP_EDGES = 2;

    /**
     * The convolution kernel.
     */
	Kernel kernel;

    /**
     * Whether to convolve alpha.
     */
	bool alpha;

    /**
     * Whether to promultiply the alpha before convolving.
     */
	bool premultiplyAlpha;

    /**
     * What do do at the image edges.
     */
	int edgeAction;
   void init()
   {
      //kernel = 0;
      alpha = true;
      premultiplyAlpha = true;
      edgeAction = CLAMP_EDGES;
   }

   public:
	/**
	 * Construct a filter with a null kernel. This is only useful if you're going to change the kernel later on.
	 */
	ConvolveFilter() : kernel(3,3)
   {
      init();
      //kernel = new Kernel(3,3);
		//this(new float[9]);
	}

	/**
	 * Construct a filter with the given 3x3 kernel.
	 * @param matrix an array of 9 floats containing the kernel
	 */
	ConvolveFilter(float matrix[9]) 
   {
      init();
		//this(new Kernel(3, 3, matrix));
      kernel.setData(3,3, matrix);
	}
	
	/**
	 * Construct a filter with the given kernel.
	 * @param rows	the number of rows in the kernel
	 * @param cols	the number of columns in the kernel
	 * @param matrix	an array of rows*cols floats containing the kernel
	 */
	ConvolveFilter(int rows, int cols, float *matrix) 
   {
      init();
      kernel.setData(rows,cols, matrix);
		//this(new Kernel(cols, rows, matrix));
	}
	
   ~ConvolveFilter()
   {
      //delete kernel;
   }
	/**
	 * Construct a filter with the given 3x3 kernel.
	 * @param kernel the convolution kernel
	 */
	//ConvolveFilter(Kernel kernel) 
 //  {
 //     init();
	//	this->kernel = kernel;	
	//}

 //   /**
 //    * Set the convolution kernel.
 //    * @param kernel the kernel
 //    * @see #getKernel
 //    */
	//void setKernel(Kernel kernel) {
	//	this->kernel = kernel;
	//}

 //   /**
 //    * Get the convolution kernel.
 //    * @return the kernel
 //    * @see #setKernel
 //    */
	//Kernel getKernel() {
	//	return kernel;
	//}

    /**
     * Set the action to perfomr for pixels off the image edges.
     * @param edgeAction the action
     * @see #getEdgeAction
     */
	void setEdgeAction(int edgeAction) {
		this->edgeAction = edgeAction;
	}

    /**
     * Get the action to perfomr for pixels off the image edges.
     * @return the action
     * @see #setEdgeAction
     */
	int getEdgeAction() {
		return edgeAction;
	}

    /**
     * Set whether to convolve the alpha channel.
     * @param useAlpha true to convolve the alpha
     * @see #getUseAlpha
     */
	void setUseAlpha( bool useAlpha ) {
		this->alpha = useAlpha;
	}

    /**
     * Get whether to convolve the alpha channel.
     * @return true to convolve the alpha
     * @see #setUseAlpha
     */
	bool getUseAlpha() {
		return alpha;
	}

    /**
     * Set whether to premultiply the alpha channel.
     * @param premultiplyAlpha true to premultiply the alpha
     * @see #getPremultiplyAlpha
     */
	void setPremultiplyAlpha( bool premultiplyAlpha ) {
		this->premultiplyAlpha = premultiplyAlpha;
	}

    /**
     * Get whether to premultiply the alpha channel.
     * @return true to premultiply the alpha
     * @see #setPremultiplyAlpha
     */
	bool getPremultiplyAlpha() {
		return premultiplyAlpha;
	}

    QImage *filter( const QImage &src, QImage *dst ) 
    {
        int width = src.width();
        int height = src.height();

        if ( dst == 0 )
         //dst = new QImage(src.width(), src.height(), src.format());
         dst = createCompatibleDestImage(src);

        const int *inPixels = (const int*)src.bits();//new int[width*height];
        int *outPixels = (int *)dst->bits();//new int[width*height];
        //getRGB( src, 0, 0, width, height, inPixels );

   //     if ( premultiplyAlpha )
			//ImageMath.premultiply( inPixels, 0, inPixels.length );
   		convolve(kernel, inPixels, outPixels, width, height, alpha, edgeAction);
   //     if ( premultiplyAlpha )
			//ImageMath.unpremultiply( outPixels, 0, outPixels.length );

        //setRGB( dst, 0, 0, width, height, outPixels );
        return dst;
    }

    //public BufferedImage createCompatibleDestImage(BufferedImage src, ColorModel dstCM) {
    //    if ( dstCM == null )
    //        dstCM = src.getColorModel();
    //    return new BufferedImage(dstCM, dstCM.createCompatibleWritableRaster(src.getWidth(), src.getHeight()), dstCM.isAlphaPremultiplied(), null);
    //}
    
    QRect getBounds2D( const QImage &src ) {
        return QRect(0, 0, src.width(), src.height());
    }
    
    //QPoint getPoint2D( QPoint srcPt, QPoint dstPt ) {
    //    if ( dstPt == null )
    //        dstPt = new Point2D.Double();
    //    dstPt.setLocation( srcPt.getX(), srcPt.getY() );
    //    return dstPt;
    //}

    //public RenderingHints getRenderingHints() {
    //    return null;
    //}

    /**
     * Convolve a block of pixels.
     * @param kernel the kernel
     * @param inPixels the input pixels
     * @param outPixels the output pixels
     * @param width the width
     * @param height the height
     * @param edgeAction what to do at the edges
     */
	static void convolve(const Kernel &kernel, const int *inPixels, int *outPixels, int width, int height, int edgeAction) 
   {
		convolve(kernel, inPixels, outPixels, width, height, true, edgeAction);
	}
	
    /**
     * Convolve a block of pixels.
     * @param kernel the kernel
     * @param inPixels the input pixels
     * @param outPixels the output pixels
     * @param width the width
     * @param height the height
     * @param alpha include alpha channel
     * @param edgeAction what to do at the edges
     */
	static void convolve(const Kernel &kernel, const int *inPixels, int *outPixels, int width, int height, bool alpha, int edgeAction) {
		if (kernel.getHeight() == 1)
			convolveH(kernel, inPixels, outPixels, width, height, alpha, edgeAction);
		else if (kernel.getWidth() == 1)
			convolveV(kernel, inPixels, outPixels, width, height, alpha, edgeAction);
		else
			convolveHV(kernel, inPixels, outPixels, width, height, alpha, edgeAction);
	}
	
	/**
	 * Convolve with a 2D kernel.
     * @param kernel the kernel
     * @param inPixels the input pixels
     * @param outPixels the output pixels
     * @param width the width
     * @param height the height
     * @param alpha include alpha channel
     * @param edgeAction what to do at the edges
	 */
	static void convolveHV(const Kernel &kernel, const int *inPixels, int *outPixels, int width, int height, bool alpha, int edgeAction) {
		int index = 0;
		float *matrix = kernel.getKernelData( 0 );
		int rows = kernel.getHeight();
		int cols = kernel.getWidth();
		int rows2 = rows/2;
		int cols2 = cols/2;

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				float r = 0, g = 0, b = 0, a = 0;

				for (int row = -rows2; row <= rows2; row++) {
					int iy = y+row;
					int ioffset;
					if (0 <= iy && iy < height)
						ioffset = iy*width;
					else if ( edgeAction == CLAMP_EDGES )
						ioffset = y*width;
					else if ( edgeAction == WRAP_EDGES )
						ioffset = ((iy+height) % height) * width;
					else
						continue;
					int moffset = cols*(row+rows2)+cols2;
					for (int col = -cols2; col <= cols2; col++) {
						float f = matrix[moffset+col];

						if (f != 0) {
							int ix = x+col;
							if (!(0 <= ix && ix < width)) {
								if ( edgeAction == CLAMP_EDGES )
									ix = x;
								else if ( edgeAction == WRAP_EDGES )
									ix = (x+width) % width;
								else
									continue;
							}
							int rgb = inPixels[ioffset+ix];
							a += f * ((rgb >> 24) & 0xff);
							r += f * ((rgb >> 16) & 0xff);
							g += f * ((rgb >> 8) & 0xff);
							b += f * (rgb & 0xff);
						}
					}
				}
				int ia = alpha ? PixelUtils::clamp((int)(a+0.5)) : 0xff;
				int ir = PixelUtils::clamp((int)(r+0.5));
				int ig = PixelUtils::clamp((int)(g+0.5));
				int ib = PixelUtils::clamp((int)(b+0.5));
				outPixels[index++] = (ia << 24) | (ir << 16) | (ig << 8) | ib;
			}
		}
	}

	/**
	 * Convolve with a kernel consisting of one row.
     * @param kernel the kernel
     * @param inPixels the input pixels
     * @param outPixels the output pixels
     * @param width the width
     * @param height the height
     * @param alpha include alpha channel
     * @param edgeAction what to do at the edges
	 */
	static void convolveH(const Kernel &kernel, const int *inPixels, int *outPixels, int width, int height, bool alpha, int edgeAction) {
		int index = 0;
		float *matrix = kernel.getKernelData( 0 );
		int cols = kernel.getWidth();
		int cols2 = cols/2;

		for (int y = 0; y < height; y++) {
			int ioffset = y*width;
			for (int x = 0; x < width; x++) {
				float r = 0, g = 0, b = 0, a = 0;
				int moffset = cols2;
				for (int col = -cols2; col <= cols2; col++) {
					float f = matrix[moffset+col];

					if (f != 0) {
						int ix = x+col;
						if ( ix < 0 ) {
							if ( edgeAction == CLAMP_EDGES )
								ix = 0;
							else if ( edgeAction == WRAP_EDGES )
								ix = (x+width) % width;
						} else if ( ix >= width) {
							if ( edgeAction == CLAMP_EDGES )
								ix = width-1;
							else if ( edgeAction == WRAP_EDGES )
								ix = (x+width) % width;
						}
						int rgb = inPixels[ioffset+ix];
						a += f * ((rgb >> 24) & 0xff);
						r += f * ((rgb >> 16) & 0xff);
						g += f * ((rgb >> 8) & 0xff);
						b += f * (rgb & 0xff);
					}
				}
				int ia = alpha ? PixelUtils::clamp((int)(a+0.5)) : 0xff;
				int ir = PixelUtils::clamp((int)(r+0.5));
				int ig = PixelUtils::clamp((int)(g+0.5));
				int ib = PixelUtils::clamp((int)(b+0.5));
				outPixels[index++] = (ia << 24) | (ir << 16) | (ig << 8) | ib;
			}
		}
	}

	/**
	 * Convolve with a kernel consisting of one column.
     * @param kernel the kernel
     * @param inPixels the input pixels
     * @param outPixels the output pixels
     * @param width the width
     * @param height the height
     * @param alpha include alpha channel
     * @param edgeAction what to do at the edges
	 */
	static void convolveV(const Kernel &kernel, const int *inPixels, int *outPixels, int width, int height, bool alpha, int edgeAction) {
		int index = 0;
		float *matrix = kernel.getKernelData(0);
		int rows = kernel.getHeight();
		int rows2 = rows/2;

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				float r = 0, g = 0, b = 0, a = 0;

				for (int row = -rows2; row <= rows2; row++) {
					int iy = y+row;
					int ioffset;
					if ( iy < 0 ) {
						if ( edgeAction == CLAMP_EDGES )
							ioffset = 0;
						else if ( edgeAction == WRAP_EDGES )
							ioffset = ((y+height) % height)*width;
						else
							ioffset = iy*width;
					} else if ( iy >= height) {
						if ( edgeAction == CLAMP_EDGES )
							ioffset = (height-1)*width;
						else if ( edgeAction == WRAP_EDGES )
							ioffset = ((y+height) % height)*width;
						else
							ioffset = iy*width;
					} else
						ioffset = iy*width;

					float f = matrix[row+rows2];

					if (f != 0) {
						int rgb = inPixels[ioffset+x];
						a += f * ((rgb >> 24) & 0xff);
						r += f * ((rgb >> 16) & 0xff);
						g += f * ((rgb >> 8) & 0xff);
						b += f * (rgb & 0xff);
					}
				}
				int ia = alpha ? PixelUtils::clamp((int)(a+0.5)) : 0xff;
				int ir = PixelUtils::clamp((int)(r+0.5));
				int ig = PixelUtils::clamp((int)(g+0.5));
				int ib = PixelUtils::clamp((int)(b+0.5));
				outPixels[index++] = (ia << 24) | (ir << 16) | (ig << 8) | ib;
			}
		}
	}

	//public String toString() {
	//	return "Blur/Convolve...";
	//}
};

#endif // __ConvolveFilter__

