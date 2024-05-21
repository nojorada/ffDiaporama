#include "mainwindow.h"
#include <QApplication>
#include <QPainter>
#include <QElapsedTimer>
#include <QDebug>
#include <QObject>
#include <QPaintDevice>

#include "smmintrin.h"
#include "emmintrin.h"
#include <qimageblitz.h>
#include "ImageFilters.h"
#include "ocv.h"

#ifndef LONGLONG
typedef __int64 LONGLONG;
#endif
class autoTimer
{
   QString messageText;
   QElapsedTimer timer;
   public:
      autoTimer(const QString &s);
      ~autoTimer();
};

const char *nano2time(qint64 ticks, bool withMillis)
{
	static char clockBuffer[128];
	qint64 millis,nanos,mikros;
	qint64 lSeconds = ticks/1000000000ll;
	millis = ticks / 1000000LL % 1000i64;
   mikros = ticks / 1000LL % 1000i64;
   nanos = ticks % 1000i64;
   double d = (double)(ticks) / (double) 1000000000LL;
	long s = lSeconds % 60L;
	long m = lSeconds / 60L;
	long h = lSeconds / 3600L;
	m -= (h*60);
	if( withMillis )
		sprintf_s( clockBuffer,128,"%02d:%02d:%02d.%03lld.%03lld.%03lld (%f secs)",h,m,s,millis,mikros,nanos,d);
	else
		sprintf_s( clockBuffer,128,"%02d:%02d:%02d",h,m,s);
	return clockBuffer;
}

static qint64 lastElapsed = 0;
autoTimer::autoTimer(const QString &s)
{
   messageText = s;
   timer.start();
}

autoTimer::~autoTimer()
{
   lastElapsed = timer.nsecsElapsed();
   qDebug() << messageText << " " <<  nano2time(lastElapsed,true);
}


void Transition_Luma(QImage Luma,double PCT,QImage *ImageA,QImage *ImageB,QPainter *WorkingPainter,int DestImageWith,int DestImageHeight) 
{
   QImage Img = ImageB->copy();

   autoTimer *a = new autoTimer("std luma");
   // Apply PCTDone to luma mask
   quint32  limit    = quint8(PCT*double(0xff))+1;
   quint32 *LumaData = (quint32 *)Luma.bits();
   quint32 *ImgData1 = (quint32 *)ImageB->bits();
   quint32 *ImgData2 = (quint32 *)ImageA->bits();
   quint32 *ImgData = (quint32 *)Img.bits();
   int size = DestImageWith * DestImageHeight;
   for (int i = 0; i < size; i++) 
   {
      if (((*LumaData++)& 0xff)>limit) 
         *ImgData=*ImgData2;
      else
         *ImgData=*ImgData1;
      ImgData++;
      ImgData1++;
      ImgData2++;
   }
   delete a;

   // Draw transformed image
   WorkingPainter->drawImage(0,0,Img);
}

void Transition_LumaDestImage(QImage Luma,double PCT,QImage *ImageA,QImage *ImageB,QPainter *WorkingPainter,int DestImageWith,int DestImageHeight, QImage *destImage) 
{
    //QImage    Img = ImageB->copy();

   autoTimer *a = new autoTimer("std luma destimage");
    // Apply PCTDone to luma mask
    quint32  limit    = quint8(PCT*double(0xff))+1;
    quint32 *LumaData = (quint32 *)Luma.bits();
    quint32 *ImgData1 = (quint32 *)ImageB->bits();
    quint32 *ImgData2 = (quint32 *)ImageA->bits();
    quint32 *ImgData = (quint32 *)destImage->bits();
    int size = DestImageWith * DestImageHeight;
    for (int i = 0; i < size; i++) 
    {
        if (((*LumaData++)& 0xff)>limit) 
         *ImgData=*ImgData2;
         else
         *ImgData=*ImgData1;
        ImgData++;
        ImgData1++;
        ImgData2++;
    }
    delete a;

    // Draw transformed image
    //WorkingPainter->drawImage(0,0,Img);
}

void Transition_LumaX(QImage Luma,double PCT,QImage *ImageA,QImage *ImageB,QPainter *WorkingPainter,int DestImageWith,int DestImageHeight) 
{
   QImage *tmpImg = 0;//
   QPaintDevice *pDev = WorkingPainter->device();
   QImage *destImage = dynamic_cast< QImage* >( pDev );
   quint32 *ImgData;
   if( destImage && destImage->size() == ImageA->size() && destImage->format() == ImageA->format() )
   {
      WorkingPainter->end();      
      ImgData = (quint32 *)destImage->bits();
   }
   else
   {
      tmpImg = new QImage(*ImageB);
      ImgData = (quint32 *)tmpImg->bits();
   }
   autoTimer *a = new autoTimer("std lumax");
   // Apply PCTDone to luma mask
   quint32  limit    = quint8(PCT*double(0xff))+1;
   quint32 *LumaData = (quint32 *)Luma.bits();
   quint32 *ImgData1 = (quint32 *)ImageB->bits();
   quint32 *ImgData2 = (quint32 *)ImageA->bits();
   //quint32 *ImgData = (quint32 *)destImage->bits();
   int size = DestImageWith * DestImageHeight;
   for (int i = 0; i < size; i++) 
   {
      if (((*LumaData++)& 0xff)>limit) 
         *ImgData=*ImgData2;
      else
         *ImgData=*ImgData1;
      ImgData++;
      ImgData1++;
      ImgData2++;
   }
   delete a;

   if( tmpImg )
   {
      // Draw transformed image
      WorkingPainter->drawImage(0,0,*tmpImg);
      delete tmpImg;
   }
}

void Transition_LumaSSE(QImage Luma,double PCT,QImage *ImageA,QImage *ImageB,QPainter *WorkingPainter,int DestImageWith,int DestImageHeight, QImage *destImage) 
{
   //QImage    Img = ImageB->copy();

   autoTimer *a = new autoTimer("sse luma");
   // Apply PCTDone to luma mask
   quint32  limit    = quint8(PCT*double(0xff))+1;
   quint32 *LumaData = (quint32 *)Luma.bits();
   quint32 *ImgData1  = (quint32 *)ImageB->bits();
   quint32 *ImgData2 = (quint32 *)ImageA->bits();
   quint32 *ImgData  = (quint32 *)destImage->bits();
   int size = DestImageWith * DestImageHeight;
   size /= 4;
   __m128i mmLimit;
   mmLimit = _mm_set1_epi32(limit);
   __m128i mmMask;
   mmMask = _mm_set_epi32(255, 255, 255, 255);
   for (int i = 0; i < size; i++) 
   {
      // take luma-Bits
      __m128i mmLuma = _mm_load_si128((const __m128i *)LumaData);
      // mask out rgb, leaving alpha
      mmLuma = _mm_and_si128(mmMask, mmLuma);
      // compare with limits
      mmLuma = _mm_cmpgt_epi32(mmLuma, mmLimit);
      // load image-data (4 pixels in onnce)
      __m128i mm0 = _mm_load_si128((const __m128i *)ImgData1);
      __m128i mm1 = _mm_load_si128((const __m128i *)ImgData2);
      __m128i result = _mm_castps_si128(_mm_blendv_ps(_mm_castsi128_ps(mm0), _mm_castsi128_ps(mm1), _mm_castsi128_ps(mmLuma)));
      _mm_store_si128((__m128i *)ImgData, result);
      //if (((*LumaData++)& 0xff)>limit) *ImgData=*ImgData2;
      ImgData += 4;
      ImgData1 += 4;
      ImgData2 += 4;
      LumaData += 4;
   }
   delete a;

   // Draw transformed image
   //WorkingPainter->drawImage(0,0,Img);
}
void lumaTest()
{
   #define PWIDTH 1920
   #define PHEIGHT 1080
   QImage imga, imgb, imgLuma;
   imga.load("z:/Bilder/Tansania-Sansibar/DCIM/Flowers/P1090073.JPG");
   imgb.load("z:/Bilder/Tansania-Sansibar/DCIM/Flowers/P1090064.JPG");
   imgLuma.load("f:/ffDiaporama_x64/luma/Box/BoxA1.png");
   imga = imga.scaled(PWIDTH, PHEIGHT).convertToFormat(QImage::Format_ARGB32_Premultiplied);
   imgb = imgb.scaled(PWIDTH, PHEIGHT).convertToFormat(QImage::Format_ARGB32_Premultiplied);
   imgLuma = imgLuma.scaled(PWIDTH, PHEIGHT).convertToFormat(QImage::Format_ARGB32_Premultiplied);
   
   QImage destImage(PWIDTH,PHEIGHT, QImage::Format_ARGB32_Premultiplied);
   QPainter p(&destImage);
   autoTimer *a = new autoTimer("std luma");
   Transition_Luma(imgLuma,90,&imga,&imgb,&p, PWIDTH,PHEIGHT); 
   p.end();
   delete a;
   //destImage.save("lumatest.jpg");

   a = new autoTimer("std luma destimage");
   Transition_LumaDestImage(imgLuma,90,&imga,&imgb,NULL/*&p*/, PWIDTH,PHEIGHT, &destImage); 
   //p.end();
   delete a;
   destImage.save("lumatest.jpg");

   QImage destImageSSE(PWIDTH,PHEIGHT, QImage::Format_ARGB32_Premultiplied);
   //QPainter pSSE(&destImageSSE);
   a = new autoTimer("sse luma");
   Transition_LumaSSE(imgLuma,90,&imga,&imgb,/*&pSSE*/NULL, PWIDTH,PHEIGHT, &destImage); 
   //pSSE.end();
   delete a;
   destImageSSE.save("lumatestSE.jpg");

   QPainter p2(&destImage);
   a = new autoTimer("std lumax");
   Transition_LumaX(imgLuma,90,&imga,&imgb,&p2, PWIDTH,PHEIGHT); 
   if( p2.isActive() )
      p2.end();
   delete a;

}

void test()
{
   const int NUMVALS = 256;
   quint32 src1[NUMVALS];
   quint32 src2[NUMVALS];
   quint32 dst[NUMVALS];
   for(int i = 0; i < NUMVALS; i++ )
   {
      src1[i] = 1;
      src2[i] = 2;
      dst[i] = 0;
   }
   __m128 a, b, res;
   __m128 mask;
   mask.m128_u32[0] = 0x80000000;
   mask.m128_u32[1] = 0;
   mask.m128_u32[2] = 0;
   mask.m128_u32[3] = 0x80000000;
   quint32 *pSrc1 = src1;
   quint32 *pSrc2 = src2;
   quint32 *pDst = dst;
   for(int i = 0; i < NUMVALS/4; i++ )
   {
      a = _mm_load_ps((float*)pSrc1);
      b = _mm_load_ps((float*)pSrc2);
      res = _mm_blendv_ps( a, b, mask );
      _mm_store_ps((float*)pDst,res);
      pSrc1 += 4;
      pSrc2 += 4;
      pDst += 4;
   }
}

void blitz_test();
void sseTest();
#ifndef WIN64
void mmxTest();
#endif
void sseConvolveTest();
extern QIMAGEBLITZ_EXPORT quint64 cvcalls;
int main(int argc, char **argv)
{
   //test();
   //lumaTest();
   blitz_test();
   qDebug() << "cvcalls " << cvcalls;
   //mmxTest();
   //sseTest();
   sseConvolveTest();
   return 0;
    QApplication app(argc, argv);

    QStringList args(app.arguments());
    if(args.count() > 1){
        for(int i=1; i < args.count(); ++i){
            BlitzMainWindow *mw = new BlitzMainWindow;
            mw->openFile(args.at(i));
            mw->resize(640, 480);
            mw->show();
        }
    }
    else{
        BlitzMainWindow *mw = new BlitzMainWindow;
        mw->resize(640, 480);
        mw->show();
    }
    return(app.exec());
}

#define delta(a,b) { double d1 = a; double d2 = b; qDebug() << "delta: " << (d2/d1)*100 << "%  d1 fps = " << 1e+9/d1 <<" d2 fps " << 1e+9/d2; }
void blitz_test()
{
   QImage imga, imgb;
   QImage destImage;
   imga.load("z:/Bilder/Tansania-Sansibar/DCIM/Flowers/P1090073.JPG");
   imgb.load("z:/Bilder/Tansania-Sansibar/DCIM/Flowers/P1090064.JPG");
   imga = imga.convertToFormat(QImage::Format_ARGB32_Premultiplied);
   imgb = imgb.convertToFormat(QImage::Format_ARGB32_Premultiplied);

   qint64 elFFD, elBlitz;
   autoTimer *a = new autoTimer("ffd grayscale");
   /*destImage = */ffd_filter::grayscale(imga);
   delete a;
   elFFD = lastElapsed;
   a = new autoTimer("blitz grayscale");
   /*destImage = */Blitz::grayscale(imgb);
   delete a;
   elBlitz = lastElapsed;
   delta(elFFD, elBlitz);

   a = new autoTimer("opencv gray");
   cv::Mat src2 = ASM::QImageToCvMat(imgb, false);
   cv::cvtColor(src2, src2, CV_RGB2GRAY);
   delete a;
   elFFD = lastElapsed;
   delta(elBlitz, elFFD);


   //swirl
   a = new autoTimer("ffd swirl");
   /*destImage = */ffd_filter::swirl(imga);
   delete a;
   elFFD = lastElapsed;
   a = new autoTimer("blitz swirl");
   destImage = Blitz::swirl(imgb);
   delete a;
   elBlitz = lastElapsed;
   delta(elFFD, elBlitz);

   //oilPaint
   a = new autoTimer("ffd oilPaint");
   /*destImage = */ffd_filter::oilPaint(imga, 2.0);
   delete a;
   elFFD = lastElapsed;
   a = new autoTimer("blitz oilPaint");
   destImage = Blitz::oilPaint(imgb, 2.0);
   delete a;
   elBlitz = lastElapsed;
   delta(elFFD, elBlitz);

   a = new autoTimer("ffd blur");
   destImage = ffd_filter::blur(imga);
   delete a;
   elFFD = lastElapsed;
   a = new autoTimer("blitz blur");
   destImage = Blitz::blur(imgb);
   delete a;
   elBlitz = lastElapsed;
   delta(elFFD, elBlitz);

   a = new autoTimer("ffd sharpen");
   destImage = ffd_filter::sharpen(imga);
   delete a;
   elFFD = lastElapsed;
   a = new autoTimer("blitz sharpen");
   destImage = Blitz::sharpen(imgb);
   delete a;
   elBlitz = lastElapsed;
   delta(elFFD, elBlitz);


   a = new autoTimer("ffd edge");
   /*destImage = */ffd_filter::edge(imga);
   delete a;
   elFFD = lastElapsed;
   a = new autoTimer("blitz edge");
   destImage = Blitz::edge(imgb);
   delete a;
   elBlitz = lastElapsed;
   delta(elFFD, elBlitz);

   a = new autoTimer("ffd charcoal");
   /*destImage = */ffd_filter::charcoal(imga);
   delete a;
   elFFD = lastElapsed;
   a = new autoTimer("blitz charcoal");
   destImage = Blitz::charcoal(imgb);
   delete a;
   elBlitz = lastElapsed;
   delta(elFFD, elBlitz);

   // despeckle  
   a = new autoTimer("ffd despeckle");
   /*destImage = */ffd_filter::despeckle(imga);
   delete a;
   elFFD = lastElapsed;
   a = new autoTimer("blitz despeckle");
   destImage = Blitz::despeckle(imgb);
   delete a;
   elBlitz = lastElapsed;
   delta(elFFD, elBlitz);

   //gaussianBlur
   a = new autoTimer("ffd gaussianBlur");
   /*destImage = */ffd_filter::gaussianBlur(imga);
   delete a;
   elFFD = lastElapsed;
   a = new autoTimer("blitz gaussianBlur");
   destImage = Blitz::gaussianBlur(imgb);
   delete a;
   elBlitz = lastElapsed;
   delta(elFFD, elBlitz);

   //emboss
   a = new autoTimer("ffd emboss");
   /*destImage = */ffd_filter::emboss(imga);
   delete a;
   elFFD = lastElapsed;
   a = new autoTimer("blitz emboss");
   destImage = Blitz::emboss(imgb);
   delete a;
   elBlitz = lastElapsed;
   delta(elFFD, elBlitz);

   //desaturate
   a = new autoTimer("ffd desaturate");
   /*destImage = */ffd_filter::desaturate(imga);
   delete a;
   elFFD = lastElapsed;
   a = new autoTimer("blitz desaturate");
   destImage = Blitz::desaturate(imgb);
   delete a;
   elBlitz = lastElapsed;
   delta(elFFD, elBlitz);

   a = new autoTimer("opencv blur");
   cv::Mat src = ASM::QImageToCvMat(imga, false);
   GaussianBlur( src, src, cv::Size(3,3), 0, 0, cv::BORDER_DEFAULT );
   delete a;

   //a = new autoTimer("opencv gray");
   //cv::Mat src2 = ASM::QImageToCvMat(imgb, false);
   //cvtColor(src2, src2, CV_RGB2GRAY);
   //delete a;

}

void sseConvolveTest()
{
   #define LOOPCOUNT 50000
   #define OUTERLOOPCOUNT 20000
   //quint32 src[] = { qRgba(1,10,20,255),qRgba(20,40,60,128),qRgba(3,3,3,3),qRgba(4,4,4,4),5,6,7,8 };
   quint32 *srcarr = new quint32[LOOPCOUNT];// = { qRgba(1,10,20,255),qRgba(20,40,60,128),qRgba(3,3,3,3),qRgba(4,4,4,4),5,6,7,8 };
   for( int i = 0; i < LOOPCOUNT; i++ )
   {
      srcarr[i] = qRgba(i%255,i%255,i%255,i%255);
   }
   float width = 0.5;
   float rgba[4] = { 0,0,0,0 }; // order: B G R A
   quint32 *src = srcarr;

   __m128 mmsum = _mm_setzero_ps();
   __m128i mm7 = _mm_setzero_si128();

   autoTimer *a = new autoTimer("sse convolve");
   //__m128i mm0 = _mm_cvtsi32_si128(*src); // mm0 is 0 0 0 0 0 0 0 0 0 0 0 0 A1 R1 G1 B1
   //mm0 = _mm_cvtepu8_epi32(mm0);
   //__m128 mm2 = _mm_cvtepi32_ps(mm0);
   //__m128 mm1 = _mm_load_ps1(&width);
   //mm1 = _mm_mul_ps(mm1,mm2);
   //mmsum = _mm_add_ps(mmsum,mm1);

   __m128i mm0;
   __m128 mm1,mm2,mm3;
   for(int x = 0; x < OUTERLOOPCOUNT; x++ )
   {
      src = srcarr;
   for(int i = 0; i < LOOPCOUNT; i++ )
   {
      mm0 = _mm_cvtsi32_si128(*src); // load rgba mm0 is 0 0 0 0 0 0 0 0 0 0 0 0 A1 R1 G1 B1
      mm0 = _mm_cvtepu8_epi32(mm0);  // expand 8bit to 32bit mm0 is 0 0 0 A1 0 0 0 R1 0 0 0 G1 0 0 0 B1
      mm2 = _mm_cvtepi32_ps(mm0);    // convert to floats
      mm1 = _mm_load_ps1(&width);    // get width 4 times into mm1
      mm3 = _mm_mul_ps(mm1,mm2);     // multiply
      mmsum = _mm_add_ps(mmsum,mm3); // add to accumulator
      ++src;
   }
   }
   delete a;
   _mm_storeu_ps(rgba,mmsum);
   qDebug() << rgba[0] << " " << rgba[1] << " " << rgba[2];
   qDebug() << "lastElapsed " << lastElapsed;


   src = srcarr;
   #define MCONVOLVE_ACC(weight, pixel) \
    r+=((weight))*(qRed((pixel))); g+=((weight))*(qGreen((pixel))); \
    b+=((weight))*(qBlue((pixel)));
   float r=0,g=0,b=0;
   a = new autoTimer("std convolve");
   for(int x = 0; x < OUTERLOOPCOUNT; x++ )
   {
   src = srcarr;
   for(int i = 0; i < LOOPCOUNT; i++ )
   {
      //MCONVOLVE_ACC(width,*src);
      r+=((width))*(qRed((*src))); 
      g+=((width))*(qGreen((*src))); 
      b+=((width))*(qBlue((*src)));
      ++src;
   }
   }
   delete a;
   rgba[0] = r;
   rgba[1] = g;
   rgba[2] = b;
   rgba[3] = 0;
   qDebug() << r << " " << g << " " << b;
   qDebug() << "lastElapsed " << lastElapsed;

   delete srcarr;
}

void sseTest()
{
   QColor c(qRgba(1,2,3,4)); 
   QColor cGray = qGray(qRgba(255,255,255,255));
   qDebug() << cGray.red() << " " << cGray.green() << " " << cGray.blue() << " " << cGray.alpha();
   
   quint32 src[] = { qRgba(10,20,30,255),qRgba(30,40,50,128),qRgba(3,3,3,3),qRgba(4,4,4,4),5,6,7,8 };
   quint16 packedmult[] = {5, 16, 11, 1, 5, 16, 11, 1};
   quint16 alphamask[] = {0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0xFFFF};
   quint16 noalphamask[] = {0xFFFF, 0xFFFF, 0xFFFF, 0x0000,0xFFFF, 0xFFFF, 0xFFFF, 0x0000};
   quint64 rmask[] = {0xFFFF, 0xFFFF};
   quint64 pm = 5ll << 48 | 16ll << 32 | 11 << 16 | 1;
   quint64 dpm[] = { 5ll << 48 | 16ll << 32 | 11 << 16 | 1, 5ll << 48 | 16ll << 32 | 11 << 16 | 1 };
   __m128i mm7 = _mm_setzero_si128();
   __m128i mm4 = _mm_loadu_si128((__m128i const*)&dpm);
   mm4 = _mm_unpacklo_epi8(mm7, mm4);
   mm4 = _mm_setr_epi16(5, 16, 11, 1, 5, 16, 11, 1);
   __m128i mm5 = _mm_loadu_si128((__m128i const*)alphamask);
   __m128i mm6 = _mm_loadu_si128((__m128i const*)noalphamask);
   __m128i mm3 = _mm_loadu_si128((__m128i const*)rmask);

   // load 2 pixels
   __m128i mm0 = _mm_loadl_epi64((__m128i const*)src); // mm0 is 0 0 0 0 0 0 0 0 A2 R2 G2 B2 A1 R1 G1 B1
   // extend to 16 bit values                          
   mm0 = _mm_unpacklo_epi8(mm0, mm7);                  // mm0 is now A2 R2 G2 B2 A1 R1 G1 B1 (16 bit values)
   // shuffle "pshufw $0xE4, %%mm0, %%mm2\n\t" // copy for alpha later
   __m128i mm2 = _mm_shuffle_epi32(mm0,0xE4); 
   //mm2 = _mm_shufflelo_epi16(mm2,0xE4); 
   // mask out alpha 
   mm0 = _mm_and_si128(mm0,mm6);
   // multiply with packedmult
   mm0 = _mm_madd_epi16(mm4, mm0);

   __m128i mm1 = _mm_shuffle_epi32(mm0,0xE4); 
   //mm1 = _mm_shufflehi_epi16(mm1,0xE4); 
   //"pshufw $0xE4, %%mm0, %%mm1\n\t"

   //mm1 = _mm_unpacklo_epi16(mm1,mm7); 
   //mm1 = _mm_unpackhi_epi16(mm1,mm7); 
   //mm1 = _mm_srli_epi16(mm1,4);
   mm1 = _mm_shufflehi_epi16(mm1,_MM_SHUFFLE2(1,0));
   mm1 = _mm_shufflelo_epi16(mm1,_MM_SHUFFLE2(1,0));
   mm1 = _mm_and_si128(mm1,mm3);
   //"punpckhwd %%mm7, %%mm1\n\t" // sum R
   mm0 = _mm_add_epi16(mm1,mm0); //"paddw %%mm1, %%mm0\n\t"
   mm0 = _mm_srli_epi16(mm0,5);  //"psrlw $0x05, %%mm0\n\t" // divide by 32

   mm0 = _mm_shufflehi_epi16(mm0, 0);                   //"pshufw $0x00, %%mm0, %%mm0\n\t" // copy to all pixels
   mm0 = _mm_shufflelo_epi16(mm0,0);
   mm0 = _mm_and_si128(mm0,mm6);                  //"pand %%mm6, %%mm0\n\t" // reset original alpha
   mm2 = _mm_and_si128(mm5, mm2);                  //"pand %%mm5, %%mm2\n\t"
   mm0 = _mm_or_si128(mm2,mm0); //"por %%mm2, %%mm0\n\t"
   mm0 = _mm_packus_epi16(mm0,mm0);
   *(quint64*)src = mm0.m128i_u64[0];
   QColor cc(*(QRgb*)src);
   qDebug() << cc.red() << " " << cc.green() << " " << cc.blue() << " " << cc.alpha();

   //*src = _mm_cvtsi128_si64(mm0);
                     //"packuswb %%mm0, %%mm0\n\t" // pack and write
                     //"movd %%mm0, (%0)\n\t"
/*
            while(data != end){
                __asm__ __volatile__
                    ("movd (%0), %%mm0\n\t" // load pixel
                     "punpcklbw %%mm7, %%mm0\n\t" // upgrade to quad
                     "pshufw $0xE4, %%mm0, %%mm2\n\t" // copy for alpha later
                     "pand %%mm6, %%mm0\n\t" // zero alpha so we can use pmaddwd

                     "pmaddwd %%mm4, %%mm0\n\t" // 2 multiplies and sum BG, RA
                     "pshufw $0xE4, %%mm0, %%mm1\n\t"
                     "punpckhwd %%mm7, %%mm1\n\t" // sum R
                     "paddw %%mm1, %%mm0\n\t"
                     "psrlw $0x05, %%mm0\n\t" // divide by 32

                     "pshufw $0x00, %%mm0, %%mm0\n\t" // copy to all pixels
                     "pand %%mm6, %%mm0\n\t" // reset original alpha
                     "pand %%mm5, %%mm2\n\t"
                     "por %%mm2, %%mm0\n\t"

                     "packuswb %%mm0, %%mm0\n\t" // pack and write
                     "movd %%mm0, (%0)\n\t"
                     : : "r"(data));
                ++data;
            }
*/
}


#ifndef WIN64
void mmxTest()
{
   QColor c(qRgba(1,2,3,4)); 
   QColor cGray = qGray(qRgba(1,2,3,4));
   qDebug() << cGray.red() << " " << cGray.green() << " " << cGray.blue() << " " << cGray.alpha();
   typedef struct { quint16 d[4]; } Pack4;
   quint32 src[] = { qRgba(1,1,1,1),qRgba(2,2,2,2),qRgba(3,3,3,3),qRgba(4,4,4,4),5,6,7,8 };
   Pack4 packedmult = {{5, 16, 11, 1}};
   quint64 pm = 5ll << 48 | 16ll << 32 | 11 << 16 | 1;
   //Pack4 alphamask[] = {0x0000, 0x0000, 0x0000, 0xFFFF};
   //Pack4 noalphamask[] = {0xFFFF, 0xFFFF, 0xFFFF, 0x0000};
   //quint16 packedmult[] = {5, 16, 11, 1, 5, 16, 11, 1};
   quint16 alphamask[] = {0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0xFFFF};
   quint16 noalphamask[] = {0xFFFF, 0xFFFF, 0xFFFF, 0x0000,0xFFFF, 0xFFFF, 0xFFFF, 0x0000};
   QRgb qmul = qRgba(11,16,5,1);
   __m64 mm7 = _mm_setzero_si64();
   __m128i mm3 = _mm_loadu_si128((__m128i const*)&packedmult);
   __m64 mm4 = _mm_movepi64_pi64(mm3);
   mm4 = _mm_set_pi16(1,11,16,5);
   mm4 = _mm_cvtsi32_si64(qmul);
   mm4 = _mm_unpacklo_pi8 (mm4, mm7);
   mm3 = _mm_loadu_si128((__m128i const*)alphamask);
   __m64 mm5 = _mm_movepi64_pi64(mm3);
   mm3 = _mm_loadu_si128((__m128i const*)noalphamask);
   __m64 mm6 = _mm_movepi64_pi64(mm3);

   // load 2 pixels
   __m64 mm0 = _mm_cvtsi32_si64(src[1]);//_mm_loadl_epi64((__m128i const*)src); // mm0 is 0 0 0 0 A1 R1 G1 B1
   // extend to 16 bit values                          
   mm0 = _mm_unpacklo_pi8 (mm0, mm7);                  // mm0 is now A1 R1 G1 B1 (16 bit values)
   // shuffle "pshufw $0xE4, %%mm0, %%mm2\n\t" // copy for alpha later
   __m64 mm2 = _m_pshufw (mm0,0xE4); 
   //mm2 = _mm_shufflelo_epi16(mm2,0xE4); 
   // mask out alpha ???
   mm0 = _m_pand(mm0,mm6);
   // multiply with packedmult
   mm0 = _m_pmaddwd (mm4, mm0);

   __m64 mm1 = _m_pshufw(mm0,0xE4); 
   //mm1 = _mm_shufflehi_epi16(mm1,0xE4); 
   //"pshufw $0xE4, %%mm0, %%mm1\n\t"

   mm1 = _m_punpckhwd (mm1,mm7); //"punpckhwd %%mm7, %%mm1\n\t" // sum R
   mm0 = _m_paddw (mm1,mm0); //"paddw %%mm1, %%mm0\n\t"
   mm0 = _m_psrlwi (mm0,5);  //"psrlw $0x05, %%mm0\n\t" // divide by 32

   mm0 = _m_pshufw(mm0,0);                   //"pshufw $0x00, %%mm0, %%mm0\n\t" // copy to all pixels
   //mm0 = _mm_shufflelo_epi16(mm0,0);
   mm0 = _m_pand(mm0,mm6);                  //"pand %%mm6, %%mm0\n\t" // reset original alpha
   mm2 = _m_pand(mm5, mm2);                  //"pand %%mm5, %%mm2\n\t"
   mm0 = _m_por (mm2,mm0); //"por %%mm2, %%mm0\n\t"
   mm0 = _m_packuswb (mm0,mm0);
                     //"packuswb %%mm0, %%mm0\n\t" // pack and write
                     //"movd %%mm0, (%0)\n\t"

   _m_empty();

/*
            while(data != end){
                __asm__ __volatile__
                    ("movd (%0), %%mm0\n\t" // load pixel
                     "punpcklbw %%mm7, %%mm0\n\t" // upgrade to quad
                     "pshufw $0xE4, %%mm0, %%mm2\n\t" // copy for alpha later
                     "pand %%mm6, %%mm0\n\t" // zero alpha so we can use pmaddwd

                     "pmaddwd %%mm4, %%mm0\n\t" // 2 multiplies and sum BG, RA
                     "pshufw $0xE4, %%mm0, %%mm1\n\t"
                     "punpckhwd %%mm7, %%mm1\n\t" // sum R
                     "paddw %%mm1, %%mm0\n\t"
                     "psrlw $0x05, %%mm0\n\t" // divide by 32

                     "pshufw $0x00, %%mm0, %%mm0\n\t" // copy to all pixels
                     "pand %%mm6, %%mm0\n\t" // reset original alpha
                     "pand %%mm5, %%mm2\n\t"
                     "por %%mm2, %%mm0\n\t"

                     "packuswb %%mm0, %%mm0\n\t" // pack and write
                     "movd %%mm0, (%0)\n\t"
                     : : "r"(data));
                ++data;
            }
*/
}
#endif