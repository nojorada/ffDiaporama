#include "qt_cuda.h"
#include <QTime>
#include <QSize>
#include <QPainter>
#include <qdebug.h>

bool use_cuda = false;
void freeHostMem(void *info)
{
   cudaFreeHost(info);
   qDebug() << "freeing hostmem";
}

cQtCuda *cuda=NULL;
cQtCuda *cQtCuda::getCuda()
{
   if( cuda == NULL )
      cuda = new cQtCuda();
   return cuda;
}

cQtCuda::cQtCuda()
{
   dev = 0;
   initialised = false;
}

bool cQtCuda::initCuda()
{
   if( initialised )
      return true;
   cudaDeviceReset();
   int dev = 0;
   cudaDeviceProp deviceProp;
   cudaGetDeviceProperties(&deviceProp, dev);
   /*NppGpuComputeCapability computeCap =*/ nppGetGpuComputeCapability();
   initialised = true;
   return true;
}

void cQtCuda::exitCuda()
{
   cudaDeviceReset();
   initialised = false;
}

bool cQtCuda::usableImageFormat(QImage::Format format)
{
   switch(format)
   {
      case QImage::Format_Invalid: return false;
      case QImage::Format_Mono: return false;
      case QImage::Format_MonoLSB: return false;
      case QImage::Format_Indexed8: return false;
      case QImage::Format_RGB32: return true;
      case QImage::Format_ARGB32: return true;
      case QImage::Format_ARGB32_Premultiplied: return true;
      case QImage::Format_RGB16: return false;
      case QImage::Format_ARGB8565_Premultiplied: return false;
      case QImage::Format_RGB666: return false;
      case QImage::Format_ARGB6666_Premultiplied: return false;
      case QImage::Format_RGB555: return false;
      case QImage::Format_ARGB8555_Premultiplied: return false;
      case QImage::Format_RGB888: return true;
      case QImage::Format_RGB444: return false;
      case QImage::Format_ARGB4444_Premultiplied: return false;
      case QImage::Format_RGBX8888: return false;
      case QImage::Format_RGBA8888: return true;
      case QImage::Format_RGBA8888_Premultiplied: return true;
   }
   return false;
}

int cQtCuda::bpp(QImage::Format format)
{
   switch(format)
   {
      case QImage::Format_Invalid: return 0;
      case QImage::Format_Mono: return 0;
      case QImage::Format_MonoLSB: return 0;
      case QImage::Format_Indexed8: return 0;
      case QImage::Format_RGB32: return 4;
      case QImage::Format_ARGB32: return 4;
      case QImage::Format_ARGB32_Premultiplied: return 4;
      case QImage::Format_RGB16: return 0;
      case QImage::Format_ARGB8565_Premultiplied: return 0;
      case QImage::Format_RGB666: return 0;
      case QImage::Format_ARGB6666_Premultiplied: return 0;
      case QImage::Format_RGB555: return 0;
      case QImage::Format_ARGB8555_Premultiplied: return 0;
      case QImage::Format_RGB888: return 3;
      case QImage::Format_RGB444: return 0;
      case QImage::Format_ARGB4444_Premultiplied: return 0;
      case QImage::Format_RGBX8888: return 0;
      case QImage::Format_RGBA8888: return 4;
      case QImage::Format_RGBA8888_Premultiplied: return 4;
   }
   return 0;
}

Npp8u *cQtCuda::deviceMalloc(QImage::Format format,int nWidthPixels, int nHeightPixels, int * pStepBytes)
{
   switch(format)
   {
      case QImage::Format_Invalid: return NULL;
      case QImage::Format_Mono: return NULL;
      case QImage::Format_MonoLSB: return NULL;
      case QImage::Format_Indexed8: return NULL;

      case QImage::Format_RGB32: 
      case QImage::Format_ARGB32: 
      case QImage::Format_ARGB32_Premultiplied: 
         return nppiMalloc_8u_C4(nWidthPixels, nHeightPixels, pStepBytes);

      case QImage::Format_RGB16: return NULL;
      case QImage::Format_ARGB8565_Premultiplied: return NULL;
      case QImage::Format_RGB666: return NULL;
      case QImage::Format_ARGB6666_Premultiplied: return NULL;
      case QImage::Format_RGB555: return NULL;
      case QImage::Format_ARGB8555_Premultiplied: return NULL;
      case QImage::Format_RGB888: 
         return nppiMalloc_8u_C3(nWidthPixels, nHeightPixels, pStepBytes);

      case QImage::Format_RGB444: return NULL;
      case QImage::Format_ARGB4444_Premultiplied: return NULL;
      case QImage::Format_RGBX8888: return NULL;

      case QImage::Format_RGBA8888: 
      case QImage::Format_RGBA8888_Premultiplied: 
         return nppiMalloc_8u_C4(nWidthPixels, nHeightPixels, pStepBytes);
   }
   return NULL;
}

NppStatus cQtCuda::nppiResizeSqrPixel_8u(QImage::Format format,const Npp8u* pSrc, NppiSize oSrcSize, 
      int nSrcStep, NppiRect oSrcROI, Npp8u *pDst, int nDstStep, NppiRect oDstROI, 
      double nXFactor, double nYFactor, double nXShift, double nYShift, int eInterpolation)
{
   switch(format)
   {
      case QImage::Format_RGB32: 
         //return nppiResizeSqrPixel_8u_AC4R(pSrc, oSrcSize, 
         //   nSrcStep,  oSrcROI, pDst, nDstStep, oDstROI, 
         //   nXFactor, nYFactor, nXShift, nYShift, eInterpolation);

      case QImage::Format_ARGB32: 
      case QImage::Format_ARGB32_Premultiplied: 
      case QImage::Format_RGBA8888: 
      case QImage::Format_RGBA8888_Premultiplied: 
         return nppiResizeSqrPixel_8u_C4R(pSrc, oSrcSize, 
            nSrcStep,  oSrcROI, pDst, nDstStep, oDstROI, 
            nXFactor, nYFactor, nXShift, nYShift, eInterpolation);

      case QImage::Format_RGB888: 
         return nppiResizeSqrPixel_8u_C3R(pSrc, oSrcSize, 
            nSrcStep,  oSrcROI, pDst, nDstStep, oDstROI, 
            nXFactor, nYFactor, nXShift, nYShift, eInterpolation);

   }
   return NPP_NOT_IMPLEMENTED_ERROR;
}

NppStatus cQtCuda::nppiCopyConstBorder_8u(QImage::Format format, const Npp8u * pSrc,   int nSrcStep, NppiSize oSrcSizeROI,
                                           Npp8u * pDst,   int nDstStep, NppiSize oDstSizeROI,
                                     int nTopBorderHeight, int nLeftBorderWidth,
                                     const Npp8u aValue[])
{
   switch(format)
   {
      case QImage::Format_RGB32: 
      case QImage::Format_ARGB32: 
      case QImage::Format_ARGB32_Premultiplied: 
      case QImage::Format_RGBA8888: 
      case QImage::Format_RGBA8888_Premultiplied: 
         return nppiCopyConstBorder_8u_C4R (pSrc, nSrcStep, oSrcSizeROI, 
                  pDst, nDstStep, oDstSizeROI, 
                  nTopBorderHeight, nLeftBorderWidth, aValue);

      case QImage::Format_RGB888: 
         return nppiCopyConstBorder_8u_C3R (pSrc, nSrcStep, oSrcSizeROI, 
                  pDst, nDstStep, oDstSizeROI, 
                  nTopBorderHeight, nLeftBorderWidth, aValue);

   }
   return NPP_NOT_IMPLEMENTED_ERROR;
}

NppStatus cQtCuda::nppiRotate_8u(QImage::Format format, const Npp8u *pSrc, NppiSize oSrcSize, int nSrcStep, NppiRect oSrcROI, 
                        Npp8u * pDst, int nDstStep, NppiRect oDstROI,
                        double nAngle, double nShiftX, double nShiftY, int eInterpolation)
{
   switch(format)
   {
      case QImage::Format_RGB32: 
      case QImage::Format_ARGB32: 
      case QImage::Format_ARGB32_Premultiplied: 
      case QImage::Format_RGBA8888: 
      case QImage::Format_RGBA8888_Premultiplied: 
         return nppiRotate_8u_C4R (pSrc, oSrcSize, nSrcStep, oSrcROI, 
         pDst, nDstStep, oDstROI, 
         nAngle, nShiftX, nShiftY, eInterpolation);

      case QImage::Format_RGB888: 
         return nppiRotate_8u_C3R (pSrc, oSrcSize, nSrcStep, oSrcROI, 
         pDst, nDstStep, oDstROI, 
         nAngle, nShiftX, nShiftY, eInterpolation);
   }
   return NPP_NOT_IMPLEMENTED_ERROR;
}


Npp8u *cQtCuda::image2device(const QImage & image, int *Pitch)
{
   Npp8u *pImageCUDA = deviceMalloc(image.format(),image.width(), image.height(), Pitch);
   if( !pImageCUDA )
      return NULL;
   // copy image to device
   cudaError_t cuda_err = cudaMemcpy2D(pImageCUDA, *Pitch, image.bits(), image.bytesPerLine(),
                                    image.bytesPerLine(), image.height(), cudaMemcpyHostToDevice);
   if( cuda_err < 0 )
   {
      nppiFree(pImageCUDA);
      pImageCUDA = NULL;
   }
   return pImageCUDA;
}

QImage cQtCuda::scaled(const QImage & image, const QSize & size, Qt::AspectRatioMode aspectRatioMode , Qt::TransformationMode transformMode ) 
{
   return scaled(image,size.width(), size.height(),aspectRatioMode,transformMode);
}

#define noTIMIMG
QImage cQtCuda::scaled(const QImage & image, int width, int height, Qt::AspectRatioMode aspectRatioMode , Qt::TransformationMode transformMode ) 
{

   QImage::Format format = image.format();
   if( !usableImageFormat(format) || width < 5 || height < 5 )
      return image.scaled(width,height,aspectRatioMode,transformMode);
#ifdef TIMIMG
   QTime t,t2;
#endif
   unsigned int nImageWidth  = image.width();
   unsigned int nImageHeight = image.height();
   //unsigned int nSrcPitch    = image.bytesPerLine();
   //unsigned int nByteCount   = image.byteCount();
   //const unsigned char *pSrcData   = image.bits();
#ifdef TIMIMG
   t.start();
   t2.start();
#endif
   // reserve device-memory for src-image
   int nSrcPitchCUDA;
   //Npp8u *pSrcImageCUDA = nppiMalloc_8u_C4(nImageWidth, nImageHeight, &nSrcPitchCUDA);
   //Npp8u *pSrcImageCUDA = deviceMalloc(format,nImageWidth, nImageHeight, &nSrcPitchCUDA);
   Npp8u *pSrcImageCUDA = image2device(image,&nSrcPitchCUDA);
#ifdef TIMIMG
   qDebug() << "nppimalloc takes " << t.elapsed() << " mSec";
#endif
   if( !pSrcImageCUDA )
      return image.scaled(width,height,aspectRatioMode,transformMode);

   // calc resize-rect
   //NppStatus nppiGetResizeRect (NppiRect oSrcROI, NppiRect  pDstRect, double nXFactor, double nYFactor, double nXShift, double nYShift, int eInterpolation)
   NppiRect oSrcROI;
   oSrcROI.height = nImageHeight;
   oSrcROI.width = nImageWidth;
   oSrcROI.x = 0;
   oSrcROI.y = 0;
   NppiRect DstRect;

   QSize s(width,height);
   QSize newSize = image.size();
   newSize.scale(s, aspectRatioMode);
   newSize.rwidth() = qMax(newSize.width(), 1);
   newSize.rheight() = qMax(newSize.height(), 1);
   if (newSize == image.size())
        return image;

   double nXFactor = (double)newSize.width()/nImageWidth;
   double nYFactor = (double)newSize.height()/nImageHeight;
   double nXShift = 0;
   double nYShift = 0;
   // interpolation
   //NPPI_INTER_NN
   //NPPI_INTER_LINEAR
   //NPPI_INTER_CUBIC
   //NPPI_INTER_CUBIC2P_BSPLINE
   //NPPI_INTER_CUBIC2P_CATMULLROM
   //NPPI_INTER_CUBIC2P_B05C03
   //NPPI_INTER_SUPER
   //NPPI_INTER_LANCZOS
   int eInterpolation = NPPI_INTER_SUPER;
   if( transformMode != Qt::SmoothTransformation ) 
      eInterpolation = NPPI_INTER_NN;
      
#ifdef TIMIMG
   t.restart();
#endif
   /*NppStatus grrStatus = */nppiGetResizeRect(oSrcROI, &DstRect, nXFactor, nYFactor, nXShift, nYShift, eInterpolation);
#ifdef TIMIMG
   qDebug() << "getResizeRect takes " << t.elapsed() << " mSec + " << t2.elapsed();
#endif

   // reserve device-memory for dst-image
   int nDstPitchCUDA;
#ifdef TIMIMG
   t.restart();
#endif
   Npp8u *pDstImageCUDA = image2device(image,&nDstPitchCUDA);
#ifdef TIMIMG
   qDebug() << "nppimalloc takes " << t.elapsed() << " mSec";
#endif
   if( !pDstImageCUDA )
      return image.scaled(width,height,aspectRatioMode,transformMode);
   
#ifdef TIMIMG
   t.restart();
#endif
#ifdef TIMIMG
   qDebug() << "image to device " << t.elapsed() << " mSec + " << t2.elapsed();

   t.restart();
#endif
   //NppStatus nppiResizeSqrPixel_8u_C4R (const Npp8u  pSrc, NppiSize oSrcSize, int
   //   nSrcStep, NppiRect oSrcROI, Npp8u  pDst, int nDstStep, NppiRect oDstROI, double
   //   nXFactor, double nYFactor, double nXShift, double nYShift, int eInterpolation)
   // do the resizing
   NppiSize oSrcSize;
   oSrcSize.width = nImageWidth;
   oSrcSize.height = nImageHeight;
   /*NppStatus nppStatus = */nppiResizeSqrPixel_8u(image.format(),pSrcImageCUDA, oSrcSize, nSrcPitchCUDA, 
      oSrcROI, pDstImageCUDA, nDstPitchCUDA, DstRect, 
      nXFactor, nYFactor, nXShift, nYShift, eInterpolation);
#ifdef TIMIMG
   qDebug() << "image resize " << t.elapsed() << " mSec";
#endif
   // create a destination-image
   uchar *bitMem=0;
   cudaMallocHost(&bitMem,nDstPitchCUDA*DstRect.height);
   QImage image2(bitMem,DstRect.width, DstRect.height,image.format(),freeHostMem,bitMem); 
   //QImage image2(DstRect.width, DstRect.height,image.format()); 

#ifdef TIMIMG
   t.restart();
#endif
   // copy data from device into image
   //extern __host__ cudaError_t CUDARTAPI cudaMemcpy2D(void *dst, size_t dpitch, const void *src, size_t spitch, size_t width, size_t height, enum cudaMemcpyKind kind);
   /*cuda_err = */cudaMemcpy2D(image2.bits(), image2.bytesPerLine(), pDstImageCUDA, nDstPitchCUDA,
                                    image2.bytesPerLine(), DstRect.height, cudaMemcpyDeviceToHost);
#ifdef TIMIMG
   qDebug() << "image to host " << t.elapsed() << " mSec + " << t2.elapsed();
#endif

   // free cuda-mem
   nppiFree(pSrcImageCUDA);
   nppiFree(pDstImageCUDA);
#ifdef TIMIMG
   qDebug() << "resize total " << t2.elapsed() << " mSec";
   qDebug() << " ";
#endif

   return image2;
}

QImage cQtCuda::scaledToHeight(const QImage & image, int height, Qt::TransformationMode mode ) 
{
   return scaled(image,-1, height,Qt::KeepAspectRatio,mode);
}

QImage cQtCuda::scaledToWidth(const QImage & image, int width, Qt::TransformationMode mode ) 
{
   return scaled(image,width, -1,Qt::KeepAspectRatio,mode);
}


typedef unsigned            u_int32_t;
typedef unsigned char       u_int8_t;
void cQtCuda::Transition_Luma(QImage Luma,double PCT,QImage *ImageA,QImage *ImageB,QPainter *WorkingPainter,int DestImageWith,int DestImageHeight) 
{
    QImage    Img      =ImageB->copy();

    // Apply PCTDone to luma mask
    u_int32_t  limit    =u_int8_t(PCT*double(0xff))+1;
    u_int32_t *LumaData=(u_int32_t *)Luma.bits();
    u_int32_t *ImgData =(u_int32_t *)Img.bits();
    u_int32_t *ImgData2=(u_int32_t *)ImageA->bits();
    //u_int32_t mask = 0xff;
    int size = DestImageWith*DestImageHeight;
    for (int i=0;i<size;i++) {
        if (((*LumaData++)& 0xff)>limit) *ImgData=*ImgData2;
        ImgData++;
        ImgData2++;
    }

    // Draw transformed image
    WorkingPainter->drawImage(0,0,Img);
}

QImage cQtCuda::expandCanvas(const QImage & image, int newWidth, int newHeight)
{

   if( newWidth < image.width() || newHeight < image.height() )
      return image;

   int nSrcPitchCUDA;
   Npp8u *pSrcImageCUDA = deviceMalloc(image.format(),image.width(), image.height(), &nSrcPitchCUDA);
   // copy image to device
   cudaError_t cuda_err = cudaMemcpy2D(pSrcImageCUDA, nSrcPitchCUDA, image.bits(), image.bytesPerLine(),
                                    image.bytesPerLine(), image.height(), cudaMemcpyHostToDevice);


   int nDstPitchCUDA;
   Npp8u *pDstImageCUDA = deviceMalloc(image.format(),newWidth, newHeight, &nDstPitchCUDA);

//• NppStatus nppiCopyConstBorder_8u_C4R (const Npp8u pSrc, int nSrcStep, NppiSize oSrcSize-
//ROI, Npp8u pDst, int nDstStep, NppiSize oDstSizeROI, int nTopBorderHeight, int nLeftBorder-
//Width, const Npp8u aValue[4])
   NppiSize oSrcSizeROI;
   oSrcSizeROI.width = image.width();
   oSrcSizeROI.height = image.height();
   NppiSize oDstSizeROI;
   oDstSizeROI.width = newWidth;
   oDstSizeROI.height = newHeight;
   int nTopBorderHeight = (newHeight-image.height())/2;
   int nLeftBorderWidth = (newWidth-image.width())/2;
   Npp8u aValue[4] = { 0,0,0,0 };
   /*NppStatus stat = */nppiCopyConstBorder_8u_C4R (pSrcImageCUDA, nSrcPitchCUDA, oSrcSizeROI, 
      pDstImageCUDA, nDstPitchCUDA, oDstSizeROI, 
      nTopBorderHeight, nLeftBorderWidth, aValue);

   // create a destination-image
   uchar *bitMem=0;
   cudaMallocHost(&bitMem,nDstPitchCUDA*newHeight);
   QImage image2(bitMem,newWidth, newHeight,image.format(),freeHostMem,bitMem); 
   // copy data from device into image
   cuda_err = cudaMemcpy2D(image2.bits(), image2.bytesPerLine(), pDstImageCUDA, nDstPitchCUDA,
                                    image2.bytesPerLine(), newHeight, cudaMemcpyDeviceToHost);
   // free cuda-mem
   nppiFree(pSrcImageCUDA);
   nppiFree(pDstImageCUDA);
   return image2;
}

QImage cQtCuda::expandCanvasCopyScale(const QImage & image, int newWidth, int newHeight, int posX, int posY, int w, int h, int scaledw, int scaledh, Qt::TransformationMode transformMode)
{
   if( newWidth < image.width() || newHeight < image.height() )
      return image;

   // alloc device-mem for image
   int nSrcPitchCUDA;
   Npp8u *pSrcImageCUDA = deviceMalloc(image.format(),image.width(), image.height(), &nSrcPitchCUDA);

   // copy image to device
   cudaError_t cuda_err = cudaMemcpy2D(pSrcImageCUDA, nSrcPitchCUDA, image.bits(), image.bytesPerLine(),
                                    image.bytesPerLine(), image.height(), cudaMemcpyHostToDevice);

   // expand canvas
   int nDstPitchCUDA;
   Npp8u *pDstImageCUDA = deviceMalloc(image.format(),newWidth, newHeight, &nDstPitchCUDA);

//• NppStatus nppiCopyConstBorder_8u_C4R (const Npp8u pSrc, int nSrcStep, NppiSize oSrcSize-
//ROI, Npp8u pDst, int nDstStep, NppiSize oDstSizeROI, int nTopBorderHeight, int nLeftBorder-
//Width, const Npp8u aValue[4])
   NppiSize oSrcSizeROI;
   oSrcSizeROI.width = image.width();
   oSrcSizeROI.height = image.height();
   NppiSize oDstSizeROI;
   oDstSizeROI.width = newWidth;
   oDstSizeROI.height = newHeight;
   int nTopBorderHeight = (newHeight-image.height())/2;
   int nLeftBorderWidth = (newWidth-image.width())/2;
   Npp8u aValue[4] = { 0,0,0,0};
   /*NppStatus stat = */nppiCopyConstBorder_8u_C4R (pSrcImageCUDA, nSrcPitchCUDA, oSrcSizeROI, 
      pDstImageCUDA, nDstPitchCUDA, oDstSizeROI, 
      nTopBorderHeight, nLeftBorderWidth, aValue);

   NppiRect oSrcROI;
   oSrcROI.x = 0;
   oSrcROI.y = 0;
   oSrcROI.width = w;
   oSrcROI.height = h;
   NppiRect DstRect;

   // reserve device-memory for dst-image
   int nDstPitchCUDA_2;
   Npp8u *pDstImageCUDA_2 = deviceMalloc(image.format(),scaledw, scaledh, &nDstPitchCUDA_2);
   //Npp8u *pDstImageCUDA_2 = deviceMalloc(image.format(),5000, 5000, &nDstPitchCUDA_2);

   int eInterpolation = NPPI_INTER_SUPER;
   if( transformMode != Qt::SmoothTransformation ) 
      eInterpolation = NPPI_INTER_NN;

   // calculate scale-factor
   double nXFactor = (double)scaledw/w;
   double nYFactor = (double)scaledh/h;
   double nXShift = 0;
   double nYShift = 0;

   // calc resize-rect
   NppStatus grrStatus = nppiGetResizeRect(oSrcROI, &DstRect, nXFactor, nYFactor, nXShift, nYShift, eInterpolation);
   NppiSize oSrcSize;
   oSrcSize.width = newWidth;
   oSrcSize.height = newHeight;

   // do resizing
   // calc offset to first pixel
   long offset;
   offset = posY * nDstPitchCUDA + posX * 4;
   /*NppStatus nppStatus = */nppiResizeSqrPixel_8u_C4R (pDstImageCUDA+offset, oSrcSize, nDstPitchCUDA, 
      oSrcROI, pDstImageCUDA_2, nDstPitchCUDA_2, DstRect, 
      nXFactor, nYFactor, nXShift, nYShift, eInterpolation);

   // create a destination-image
   uchar *bitMem=0;
   cudaMallocHost(&bitMem,nDstPitchCUDA_2*scaledh);
   //cudaMallocHost(&bitMem,nDstPitchCUDA_2*5000);
   QImage image2(bitMem,scaledw, scaledh,image.format(),freeHostMem,bitMem); 
   //QImage image2(bitMem,5000, 5000,image.format(),freeHostMem,bitMem); 
   // copy data from device into image
   cuda_err = cudaMemcpy2D(image2.bits(), image2.bytesPerLine(), pDstImageCUDA_2, nDstPitchCUDA_2,
                                    image2.bytesPerLine(), scaledh, cudaMemcpyDeviceToHost);
   // free cuda-mem
   nppiFree(pSrcImageCUDA);
   nppiFree(pDstImageCUDA);
   nppiFree(pDstImageCUDA_2);
   return image2;
}


#define noFULLDEBUGMESSAGE
#define SHOW_ERROR(a,b) \
if(a) \
{\
   qDebug() << "expandCanvasRotateCopyScale " << image.width() <<"x"<< image.height() << "format " << image.format();\
   qDebug() << "    " << newWidth << "x" << newHeight << " rot = " << rotationFactor;    \
   qDebug() << "    " << posX << " " << posY << " " << w << "x" << h;\
   qDebug() << "    " << scaledw << "x" << scaledh;\
   qDebug() << b << " " << a;\
}

#define SHOW_CUDAERROR(a,b) \
if(a)\
{\
   qDebug() << b << " " << a;\
}

QImage cQtCuda::expandCanvasRotateCopyScale_old(const QImage & image, int newWidth, int newHeight, double rotationFactor, int posX, int posY, int w, int h, int scaledw, int scaledh, Qt::TransformationMode transformMode )
{
//#ifdef FULLDEBUGMESSAGE
   qDebug() << "expandCanvasRotateCopyScale ";
   qDebug() << "image is " << image.width() <<"x"<< image.height() << " format " << image.format();
   qDebug() << "  expand to " << newWidth << "x" << newHeight;
   qDebug() << "  rotate = " << rotationFactor;    
   qDebug() << "  take from pos " << posX << " " << posY << " " << w << "x" << h;
   qDebug() << "  scale to " << scaledw << "x" << scaledh;
//#endif 
   if( newWidth < image.width() || newHeight < image.height() ) // is that ok?
      return image;

   // check that there is really somthing to do 
   if( image.width() == scaledw &&
       image.height() == scaledh &&
       rotationFactor == 0.0 &&
       posX == (newWidth-scaledw)/2 &&
       posY == (newHeight-scaledh)/2 )
      return image;

   QImage::Format format = image.format();

   // source-image to device
   int nSrcPitchCUDA;
   Npp8u *pSrcImageCUDA = image2device(image, &nSrcPitchCUDA);

   // expand canvas
   int nDstPitchCUDA;
   Npp8u *pDstImageCUDA = deviceMalloc(format,newWidth, newHeight, &nDstPitchCUDA);
#ifdef FULLDEBUGMESSAGE
   qDebug() << "pDstImageCUDA " << pDstImageCUDA;
#endif 

//• NppStatus nppiCopyConstBorder_8u_C4R (const Npp8u pSrc, int nSrcStep, NppiSize oSrcSize-
//ROI, Npp8u pDst, int nDstStep, NppiSize oDstSizeROI, int nTopBorderHeight, int nLeftBorder-
//Width, const Npp8u aValue[4])
   NppiSize oSrcSizeROI = { image.width(), image.height() };
   NppiSize oDstSizeROI = { newWidth, newHeight };
   int nTopBorderHeight = (newHeight-image.height())/2;
   int nLeftBorderWidth = (newWidth-image.width())/2;
   Npp8u aValue[4] = { 0,0,0,0 };
   NppStatus stat = nppiCopyConstBorder_8u(format,pSrcImageCUDA, nSrcPitchCUDA, oSrcSizeROI, 
      pDstImageCUDA, nDstPitchCUDA, oDstSizeROI, 
      nTopBorderHeight, nLeftBorderWidth, aValue);
#ifdef FULLDEBUGMESSAGE
   qDebug() << "stat " << stat;
#endif 
SHOW_ERROR(stat,"stat")


   // rotation
   if( rotationFactor != 0.0 )
   {
      NppiRect oSrcROI;
      oSrcROI.x = 0;
      oSrcROI.y = 0;
      oSrcROI.width = newWidth;
      oSrcROI.height = newHeight;
      double aBoundingBox[2][2];
      double nAngle = rotationFactor*-1;
      double nShiftX = 0.0;
      double nShiftY = 0.0;
      //NppStatus nppiGetRotateBound (NppiRect oSrcROI, double aBoundingBox[2][2],double nAngle, double nShiftX, double nShiftY)
      NppStatus roStatus = nppiGetRotateBound (oSrcROI, aBoundingBox,nAngle, nShiftX, nShiftY);
#ifdef FULLDEBUGMESSAGE
   qDebug() << "roStatus " << roStatus;
#endif 
SHOW_ERROR(roStatus,"roStatus")
      int nw = abs(aBoundingBox[0][0]) + abs(aBoundingBox[1][0]);
      int nh = abs(aBoundingBox[0][1]) + abs(aBoundingBox[1][1]);
      nShiftX = aBoundingBox[0][0]*-1.0-(nw-newWidth)/2;
      nShiftY = aBoundingBox[0][1]*-1.0-(nh-newHeight)/2;
      //roStatus = nppiGetRotateBound (oSrcROI, aBoundingBox,nAngle, nShiftX, nShiftY);
      int nDstPitchCUDARot;
      Npp8u *pDstImageCUDARot = deviceMalloc(format,nw+10, nh+10, &nDstPitchCUDARot);
#ifdef FULLDEBUGMESSAGE
   qDebug() << "pDstImageCUDARot " << pDstImageCUDARot;
#endif 
      Npp8u aValue[4] = {0,0,0,0};
      Npp8u aValue3[3] = {0,0,0};
      NppiSize oSizeROIset = {nw+10, nh+10};
      NppStatus setStatus;
      if(format == QImage::Format_RGB888)
         setStatus = nppiSet_8u_C3R (aValue, pDstImageCUDARot, nDstPitchCUDARot,oSizeROIset);
      else
         setStatus = nppiSet_8u_C4R (aValue, pDstImageCUDARot, nDstPitchCUDARot,oSizeROIset);
#ifdef FULLDEBUGMESSAGE
   qDebug() << "setStatus " << setStatus;
#endif 
SHOW_ERROR(setStatus,"setStatus")
      //NppStatus nppiRotate_8u_C4R (const Npp8u  pSrc, NppiSize oSrcSize, int nSrcStep,
      //      NppiRect oSrcROI, Npp8u  pDst, int nDstStep, NppiRect oDstROI, double nAngle,
      //      double nShiftX, double nShiftY, int eInterpolation)
      NppiSize oSrcSize;
      oSrcSize.width = newWidth;
      oSrcSize.height = newHeight;
      NppiRect oDstROI;
      oDstROI.x = 0;
      oDstROI.y = 0;
      oDstROI.width = newWidth;//nw;
      oDstROI.height = newHeight;//nh;
      int eInterpolation = NPPI_INTER_SUPER;
      //if( transformMode != Qt::SmoothTransformation ) 
         eInterpolation = NPPI_INTER_NN;
      NppStatus rotStatus;
      if(format == QImage::Format_RGB888)
         rotStatus = nppiRotate_8u_C3R (pDstImageCUDA, oSrcSize, nDstPitchCUDA,oSrcROI, 
            pDstImageCUDARot, nDstPitchCUDARot, oDstROI, 
            nAngle, nShiftX, nShiftY, eInterpolation);
      else
         rotStatus = nppiRotate_8u_C4R (pDstImageCUDA, oSrcSize, nDstPitchCUDA,oSrcROI, 
            pDstImageCUDARot, nDstPitchCUDARot, oDstROI, 
            nAngle, nShiftX, nShiftY, eInterpolation);
#ifdef FULLDEBUGMESSAGE
   qDebug() << "rotStatus " << rotStatus;
#endif 
SHOW_ERROR(rotStatus,"rotStatus")
      nppiFree(pDstImageCUDA);
      pDstImageCUDA = pDstImageCUDARot;
      nDstPitchCUDA = nDstPitchCUDARot;
   }
   // rotation end


   NppiRect oSrcROI = { 0, 0, w, h };
   NppiRect DstRect;

   // reserve device-memory for dst-image
   int nDstPitchCUDA_2;
   Npp8u *pDstImageCUDA_2 = deviceMalloc(format,scaledw+5, scaledh+5, &nDstPitchCUDA_2);
#ifdef FULLDEBUGMESSAGE
   qDebug() << "pDstImageCUDA_2 " << pDstImageCUDA_2;
#endif 

   int eInterpolation = NPPI_INTER_SUPER;
   //if( transformMode != Qt::SmoothTransformation ) 
   //   eInterpolation = NPPI_INTER_NN;

   // calculate scale-factor
   double nXFactor = (double)scaledw/w;
   double nYFactor = (double)scaledh/h;
   double nXShift = 0.0;
   double nYShift = 0.0;

   // calc resize-rect
   NppStatus grrStatus = nppiGetResizeRect(oSrcROI, &DstRect, nXFactor, nYFactor, nXShift, nYShift, eInterpolation);
#ifdef FULLDEBUGMESSAGE
   qDebug() << "grrStatus " << grrStatus;
#endif 
SHOW_ERROR(grrStatus,"grrStatus")
   //if( posX < 0 )
   //   DstRect.x += abs(posX);
   //if( posY < 0 )
   //   DstRect.y += abs(posY);

   NppiSize oSrcSize = { newWidth, newHeight };

   // do resizing
   // calc offset to first pixel
   long offset = 0;
   if( posY > 0 )
      offset = posY * nDstPitchCUDA;
   if( posX > 0 )
       offset += (posX * 4);
   NppStatus nppStatus = nppiResizeSqrPixel_8u(format,pDstImageCUDA+offset, oSrcSize, nDstPitchCUDA, 
      oSrcROI, pDstImageCUDA_2, nDstPitchCUDA_2, DstRect, 
      nXFactor, nYFactor, nXShift, nYShift, eInterpolation);

#ifdef FULLDEBUGMESSAGE
   if( nppStatus )
      qDebug() << "nppStatus " << nppStatus;
#endif 
SHOW_ERROR(nppStatus,"nppStatus")
   // create a destination-image
   //uchar *bitMem=0;
   //cudaMallocHost(&bitMem,nDstPitchCUDA_2*scaledh);
   //cudaMallocHost(&bitMem,nDstPitchCUDA_2*5000);
   //QImage image2(bitMem,scaledw, scaledh,image.format(),freeHostMem,bitMem); 
   QImage image2(scaledw, scaledh,image.format()); 
   cudaError_t cuda_err = cudaMemcpy2D(image2.bits(), image2.bytesPerLine(), pDstImageCUDA_2, nDstPitchCUDA_2,
                                    image2.bytesPerLine(), scaledh, cudaMemcpyDeviceToHost);
#ifdef FULLDEBUGMESSAGE
   qDebug() << "cuda_err " << cuda_err;
#endif 
   // free cuda-mem
   nppiFree(pSrcImageCUDA);
   nppiFree(pDstImageCUDA);
   nppiFree(pDstImageCUDA_2);
   return image2;
}

cudaImage cQtCuda::image2device(const QImage & image)
{
   cudaImage cudaImage;
   cudaImage.pMem = deviceMalloc(image.format(),image.width(), image.height(), &cudaImage.pitch);
   cudaError_t cuda_err = cudaMemcpy2D(cudaImage.pMem, cudaImage.pitch, image.bits(), image.bytesPerLine(),
                                    image.bytesPerLine(), image.height(), cudaMemcpyHostToDevice);
   if( cuda_err < 0 )
   {
      nppiFree(cudaImage.pMem);
      cudaImage.pMem = NULL;
   }
   cudaImage.format = image.format();
   cudaImage.width = image.width();
   cudaImage.height = image.height();
   return cudaImage;
}

QImage cQtCuda::expandCanvasRotateCopyScale(const QImage & image, int newWidth, int newHeight, double rotationFactor, int posX, int posY, int w, int h, int scaledw, int scaledh, Qt::TransformationMode transformMode )
{
//#ifdef FULLDEBUGMESSAGE
   qDebug() << "expandCanvasRotateCopyScale ";
   qDebug() << "image is " << image.width() <<"x"<< image.height() << " format " << image.format();
   qDebug() << "  expand to " << newWidth << "x" << newHeight;
   qDebug() << "  rotate = " << rotationFactor;    
   qDebug() << "  take from pos " << posX << " " << posY << " " << w << "x" << h;
   qDebug() << "  scale to " << scaledw << "x" << scaledh;
//#endif 
   if( newWidth < image.width() || newHeight < image.height() ) // is that ok?
      return image;

   // check that there is really somthing to do 
   if( image.width() == scaledw &&
       image.height() == scaledh &&
       rotationFactor == 0.0 &&
       posX == (newWidth-scaledw)/2 &&
       posY == (newHeight-scaledh)/2 )
      return image;

   QImage::Format format = image.format();

   cudaImage srcImage = image2device(image);
   toARGB32_Premultiplied(srcImage);
   cudaImage expanded = expandCanvas(srcImage,newWidth,newHeight);

   // rotation
   if( rotationFactor != 0.0 )
   {
      // todo
      cudaImage rotated = rotate(expanded, rotationFactor);
      nppiFree(expanded.pMem);
      expanded = rotated;
   }
   // rotation end

   cudaImage taken = take(expanded,posX, posY, w, h);
   cudaImage scaled = scale(taken,scaledw, scaledh, transformMode);

   // create a destination-image
   //uchar *bitMem=0;
   //cudaMallocHost(&bitMem,nDstPitchCUDA_2*scaledh);
   //cudaMallocHost(&bitMem,nDstPitchCUDA_2*5000);
   //QImage image2(bitMem,scaledw, scaledh,image.format(),freeHostMem,bitMem); 
   QImage image2 = fromcudaImage(scaled);

#ifdef FULLDEBUGMESSAGE
   qDebug() << "cuda_err " << cuda_err;
#endif 
   // free cuda-mem
   nppiFree(scaled.pMem);
   nppiFree(taken.pMem);
   nppiFree(expanded.pMem);
   nppiFree(srcImage.pMem);
   return image2;
}

//cudaImage cQtCuda::image2cudaImage(const QImage & image)
//{
//   cudaImage cudaImage;
//   cudaImage.pMem = deviceMalloc(image.format(),image.width(), image.height(), &cudaImage.pitch);
//   cudaError_t cuda_err = cudaMemcpy2D(cudaImage.pMem, cudaImage.pitch, image.bits(), image.bytesPerLine(),
//                                    image.bytesPerLine(), image.height(), cudaMemcpyHostToDevice);
//   if( cuda_err < 0 )
//   {
//      nppiFree(cudaImage.pMem);
//      cudaImage.pMem = NULL;
//   }
//   cudaImage.format = image.format();
//   cudaImage.width = image.width();
//   cudaImage.height = image.height();
//   return cudaImage;
//}

cudaImage cQtCuda::expandCanvas(const cudaImage& src, int newWidth, int newHeight)
{
   cudaImage cudaImage;
   cudaImage.pMem = deviceMalloc(src.format,newWidth, newHeight, &cudaImage.pitch);
   cudaImage.format = src.format;
   cudaImage.width = newWidth;
   cudaImage.height = newHeight;

//• NppStatus nppiCopyConstBorder_8u_C4R (const Npp8u pSrc, int nSrcStep, NppiSize oSrcSize-
//ROI, Npp8u pDst, int nDstStep, NppiSize oDstSizeROI, int nTopBorderHeight, int nLeftBorder-
//Width, const Npp8u aValue[4])
   NppiSize oSrcSizeROI = { src.width, src.height };
   NppiSize oDstSizeROI = { newWidth, newHeight };
   int nTopBorderHeight = (newHeight-src.height)/2;
   int nLeftBorderWidth = (newWidth-src.width)/2;
   Npp8u aValue[4] = { 0,0,0,0 };
   NppStatus stat = nppiCopyConstBorder_8u(src.format,src.pMem, src.pitch, oSrcSizeROI, 
      cudaImage.pMem, cudaImage.pitch, oDstSizeROI, 
      nTopBorderHeight, nLeftBorderWidth, aValue);
   SHOW_CUDAERROR(stat,"cQtCuda::expandCanvas")
#ifdef FULLDEBUGMESSAGE
   qDebug() << "stat " << stat;
#endif 
//SHOW_ERROR(stat,"stat")
   return cudaImage;
}

cudaImage cQtCuda::rotate(const cudaImage& src, double rotationFactor)
{
   cudaImage result;
   if( rotationFactor != 0.0 )
   {
      cudaImage dstImage;
      NppiRect oSrcROI;
      oSrcROI.x = 0;
      oSrcROI.y = 0;
      oSrcROI.width = src.width;
      oSrcROI.height = src.height;
      double aBoundingBox[2][2];
      double nAngle = rotationFactor*-1;
      double nShiftX = 0.0;
      double nShiftY = 0.0;
      NppStatus roStatus = nppiGetRotateBound (oSrcROI, aBoundingBox,nAngle, nShiftX, nShiftY);
#ifdef FULLDEBUGMESSAGE
   qDebug() << "roStatus " << roStatus;
#endif 
//SHOW_ERROR(roStatus,"roStatus")
      int nw = abs(aBoundingBox[0][0]) + abs(aBoundingBox[1][0]);
      int nh = abs(aBoundingBox[0][1]) + abs(aBoundingBox[1][1]);
      nShiftX = aBoundingBox[0][0]*-1.0;//-(nw-src.width)/2;
      nShiftY = aBoundingBox[0][1]*-1.0;//-(nh-src.height)/2;
      //roStatus = nppiGetRotateBound (oSrcROI, aBoundingBox,nAngle, nShiftX, nShiftY);
      dstImage.pMem = deviceMalloc(src.format,nw, nh, &dstImage.pitch);
      dstImage.format = src.format;
      dstImage.width = nw;
      dstImage.height = nh;
      //int nDstPitchCUDARot;
      //Npp8u *pDstImageCUDARot = deviceMalloc(src.format,nw+10, nh+10, &nDstPitchCUDARot);
#ifdef FULLDEBUGMESSAGE
   qDebug() << "pDstImageCUDARot " << pDstImageCUDARot;
#endif 
      Npp8u aValue[4] = {0,0,0,0};
      Npp8u aValue3[3] = {0,0,0};
      //Npp8u aValue[4] = {0,0,0,0};
      //Npp8u aValue3[3] = {0,0,0};
      NppiSize oSizeROIset = {nw, nh};
      NppStatus setStatus;
      if(src.format == QImage::Format_RGB888)
         setStatus = nppiSet_8u_C3R (aValue, dstImage.pMem, dstImage.pitch,oSizeROIset);
      else
         setStatus = nppiSet_8u_C4R (aValue, dstImage.pMem, dstImage.pitch,oSizeROIset);
#ifdef FULLDEBUGMESSAGE
   qDebug() << "setStatus " << setStatus;
#endif 
   SHOW_CUDAERROR(setStatus,"cQtCuda::rotate set")
//SHOW_ERROR(setStatus,"setStatus")
      //NppStatus nppiRotate_8u_C4R (const Npp8u  pSrc, NppiSize oSrcSize, int nSrcStep,
      //      NppiRect oSrcROI, Npp8u  pDst, int nDstStep, NppiRect oDstROI, double nAngle,
      //      double nShiftX, double nShiftY, int eInterpolation)
      NppiSize oSrcSize;
      oSrcSize.width = src.width;
      oSrcSize.height = src.height;
      NppiRect oDstROI;
      oDstROI.x = 0;
      oDstROI.y = 0;
      oDstROI.width = nw;
      oDstROI.height = nh;
      int eInterpolation = NPPI_INTER_SUPER;
      //if( transformMode != Qt::SmoothTransformation ) 
         eInterpolation = NPPI_INTER_NN;
      NppStatus rotStatus;
      if(src.format == QImage::Format_RGB888)
         rotStatus = nppiRotate_8u_C3R (src.pMem, oSrcSize, src.pitch,oSrcROI, 
            dstImage.pMem, dstImage.pitch, oDstROI, 
            nAngle, nShiftX, nShiftY, eInterpolation);
      else
         rotStatus = nppiRotate_8u_C4R (src.pMem, oSrcSize, src.pitch,oSrcROI, 
            dstImage.pMem, dstImage.pitch, oDstROI, 
            nAngle, nShiftX, nShiftY, eInterpolation);
   SHOW_CUDAERROR(rotStatus,"cQtCuda::rotate rotate")
#ifdef FULLDEBUGMESSAGE
   qDebug() << "rotStatus " << rotStatus;
#endif 
   result = take(dstImage,(nw-src.width)/2,(nh-src.height)/2,src.width,src.height);
   nppiFree(dstImage.pMem);
//SHOW_ERROR(rotStatus,"rotStatus")
      //nppiFree(pDstImageCUDA);
      //pDstImageCUDA = pDstImageCUDARot;
      //nDstPitchCUDA = nDstPitchCUDARot;
   }
   return result;
}

cudaImage cQtCuda::take(const cudaImage& src, int posX, int posY, int w, int h)
{
   cudaImage dstImage;
   dstImage.pMem = deviceMalloc(src.format,w, h, &dstImage.pitch);
   dstImage.format = src.format;
   dstImage.width = w;
   dstImage.height = h;

      Npp8u aValue[4] = {0,0,0,0};
      Npp8u aValue3[3] = {0,0,0};
      NppiSize oSizeROIset = {w, h};
      NppStatus setStatus;
      if(src.format == QImage::Format_RGB888)
         setStatus = nppiSet_8u_C3R (aValue, dstImage.pMem, dstImage.pitch,oSizeROIset);
      else
         setStatus = nppiSet_8u_C4R (aValue, dstImage.pMem, dstImage.pitch,oSizeROIset);

   Npp8u *pSrc = src.pMem;
   Npp8u *pDst = dstImage.pMem;
   int dw = w;
   int dh = h;
   if( posX < 0 )
   {
      dw -= posX*-1;
      pDst += posX*-1*bpp(src.format);
      posX = 0;
   }
   if( dw > src.width )
      dw = src.width;//-posX;
   if( posY < 0 )
   {
      dh -= posY*-1;
      pDst += posY*-1*dstImage.pitch;
      posY = 0;
   }
   if( /*posY + */dh > h )
      dh = h;//-posY;
   pSrc += posX*bpp(src.format)+posY*src.pitch;
   cudaError_t cuda_err = cudaMemcpy2D(pDst, dstImage.pitch, pSrc, src.pitch,
                                    dw*bpp(src.format), dh, cudaMemcpyDeviceToDevice);
   SHOW_CUDAERROR(cuda_err,"cQtCuda::take")
   return dstImage;
}

cudaImage cQtCuda::scale(const cudaImage& src, int scaledw, int scaledh, Qt::TransformationMode transformMode)
{
   NppiRect oSrcROI = { 0, 0, src.width, src.height };
   NppiRect DstRect;

   // reserve device-memory for dst-image
   cudaImage dstImage;
   dstImage.pMem = deviceMalloc(src.format,scaledw, scaledh, &dstImage.pitch);
   dstImage.format = src.format;
   dstImage.width = scaledw;
   dstImage.height = scaledh;

   //int eInterpolation = NPPI_INTER_SUPER;
   ////if( transformMode != Qt::SmoothTransformation ) 
   //   eInterpolation = NPPI_INTER_NN;
   int eInterpolation = NPPI_INTER_NN; // other modes didn't work!!!!!

   // calculate scale-factor
   double nXFactor = (double)scaledw/src.width;
   double nYFactor = (double)scaledh/src.height;
   double nXShift = 0.5;
   double nYShift = 0.5;

   // calc resize-rect
   NppStatus grrStatus = nppiGetResizeRect(oSrcROI, &DstRect, nXFactor, nYFactor, nXShift, nYShift, eInterpolation);

   //if( posX < 0 )
   //   DstRect.x += abs(posX);
   //if( posY < 0 )
   //   DstRect.y += abs(posY);

   NppiSize oSrcSize = { src.width, src.height };

   // do resizing
   // calc offset to first pixel
   long offset = 0;
   //if( posY > 0 )
   //   offset = posY * nDstPitchCUDA;
   //if( posX > 0 )
   //    offset += (posX * 4);
   //NppStatus nppiResizeSqrPixel_8u(QImage::Format,const Npp8u *pSrc, NppiSize oSrcSize, 
   //      int nSrcStep, NppiRect oSrcROI, Npp8u *pDst, int nDstStep, NppiRect oDstROI, 
   //      double nXFactor, double nYFactor, double nXShift, double nYShift, int eInterpolation);
   NppStatus nppStatus = nppiResizeSqrPixel_8u(src.format,src.pMem, oSrcSize, src.pitch, 
      oSrcROI, dstImage.pMem, dstImage.pitch, DstRect, 
      nXFactor, nYFactor, nXShift, nYShift, eInterpolation);
   SHOW_CUDAERROR(nppStatus,"cQtCuda::scale")
   if( nppStatus )
   {
      qDebug() << "error";
   }
   return dstImage;
}

bool cQtCuda::toARGB32_Premultiplied(cudaImage& src)
{
   cudaImage img;
   bool bRet = false;
   if( src.format != QImage::Format_RGBA8888_Premultiplied )
   {
      switch(bpp(src.format))
      { 
         case 3:
         {
            cudaImage alphaImage;
            NppiSize oSizeROI = { src.width, src.height};
            int aDstOrder[4] = { 0,1,2,3 }; 
            alphaImage.pMem = deviceMalloc(QImage::Format_RGBA8888_Premultiplied,src.width, src.height, &alphaImage.pitch);
            NppStatus status = nppiSwapChannels_8u_C3C4R (src.pMem, src.pitch, alphaImage.pMem, alphaImage.pitch, oSizeROI, aDstOrder, 255);
            nppiFree(src.pMem);
            alphaImage.format = QImage::Format_RGBA8888_Premultiplied;
            alphaImage.width = src.width;
            alphaImage.height = src.height;
            src = alphaImage;
            bRet = true;
         }
         break;

         case 4:
         {
            Npp8u aConstants[4] = { 0,0,0,255 };
            NppiSize oSizeROI = { src.width, src.height};
            int aDstOrder[4] = { 0,1,3,2 }; 
            NppStatus status = nppiOrC_8u_C4IR (aConstants, src.pMem, src.pitch, oSizeROI);
            //status = nppiSwapChannels_8u_C4IR(img.pMem, img.pitch, oSizeROI, aDstOrder);
            src.format = QImage::Format_ARGB32_Premultiplied;
            bRet = true;
         }
         break;
      }
   }
   else
      bRet = true;
   return bRet;
}

QImage cQtCuda::fromcudaImage(const cudaImage& src)
{
   QImage image(src.width, src.height, src.format); 
   cudaError_t cuda_err = cudaMemcpy2D(image.bits(), image.bytesPerLine(), src.pMem, src.pitch,
                                    image.bytesPerLine(), image.height(), cudaMemcpyDeviceToHost);
   return image;
}

/*
cudaImage::cudaImage()
{
   pMem = NULL;
   pitch = 0;
   width = 0;
   height = 0;
   error = NPP_NO_ERROR;
}

cudaImage cudaImage::expandCanvas(int width, int height)
{
   return cudaImage();
}

cudaImage cudaImage::rotate()
{
   return cudaImage();
}

cudaImage cudaImage::take()
{
   return cudaImage();
}

cudaImage cudaImage::scale()
{
   return cudaImage();
}
*/
