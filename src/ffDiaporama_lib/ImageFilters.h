/* ======================================================================
    This file is part of ffDiaporama
    ffDiaporama is a tool to make diaporama as video
    Copyright (C) 2011-2014 Dominique Levray <domledom@laposte.net>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
   ====================================================================== */

#ifndef _IMAGEFILTERS_H
#define _IMAGEFILTERS_H

#include "BasicDefines.h"

//// Adapt From fmt_filters <http://ksquirrel.sourceforge.net/subprojects.php>
//void FltBrightness(QImage &Img,int32_t bn);
//void FltContrast(QImage &Img,int32_t contrast);
//void FltGamma(QImage &Img,double L);
//void FltColorize(QImage &Img,int32_t red,int32_t green,int32_t blue);
//
//// Adapt From QImageBlitz <http://qimageblitz.sourceforge.net/>
//void FltGrayscale(QImage &Img);
//void FltGrayscale(QImage &Img, double pctDone);
//void FltAutoContrast(QImage &Img);
//void FltBlur(QImage &Img,int radius);
//void FltSharpen(QImage &Img,int radius);
//void FltEdge(QImage &Img);
//void FltCharcoal(QImage &Img);
//void FltDespeckle(QImage &Img);
//void FltAntialias(QImage &Img);
//void FltGaussianBlur(QImage &Img,float radius,float sigma);
//void FltGaussianSharpen(QImage &Img,float radius,float sigma);
//void FltEqualize(QImage &Img);
//void FltEmboss(QImage &Img, float radius, float sigma);
//void FltOilPaint(QImage &Img, float radius=0.0);
//void FltDesaturate(QImage &Img, float desat);
//void FltSwirl(QImage &Img, float degrees);
//void FltImplode(QImage &Img, float amount);

class ffdFilter
{
   public:
      // Adapt From fmt_filters <http://ksquirrel.sourceforge.net/subprojects.php>
      static void FltBrightness(QImage &Img,int32_t bn);
      static void FltContrast(QImage &Img,int32_t contrast);
      static void FltGamma(QImage &Img,double L);
      static void FltColorize(QImage &Img,int32_t red,int32_t green,int32_t blue);

      // Adapt From QImageBlitz <http://qimageblitz.sourceforge.net/>
      static void FltGrayscale(QImage &Img);
      //static void FltGrayscale(QImage &Img, double pctDone);
      static void FltAutoContrast(QImage &Img);
      static void FltBlur(QImage &Img,int radius);
      static void FltSharpen(QImage &Img,int radius);
      static void FltEdge(QImage &Img);
      static void FltCharcoal(QImage &Img);
      static void FltDespeckle(QImage &Img);
      static void FltAntialias(QImage &Img);
      static void FltGaussianBlur(QImage &Img,float radius,float sigma);
      static void FltGaussianSharpen(QImage &Img,float radius,float sigma);
      static void FltEqualize(QImage &Img);
      static void FltEmboss(QImage &Img, float radius, float sigma);
      static void FltOilPaint(QImage &Img, float radius=0.0);
      static void FltDesaturate(QImage &Img, float desat);
      static void FltSwirl(QImage &Img, float degrees);
      static void FltImplode(QImage &Img, float amount);
};
#endif // _IMAGEFILTERS_H
