/* ======================================================================
    This file is part of ffDiaporama
    ffDiaporama is a tools to make diaporama as video
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

//#include "BasicDefines.h"
#include <QImage>

// Adapt From fmt_filters <http://ksquirrel.sourceforge.net/subprojects.php>
void FltBrightness(QImage &Img,qint32 bn);
void FltContrast(QImage &Img,qint32 contrast);
void FltGamma(QImage &Img,double L);
void FltColorize(QImage &Img,qint32 red,qint32 green,qint32 blue);

// Adapt From QImageBlitz <http://qimageblitz.sourceforge.net/>
void FltGrayscale(QImage &Img);
void FltGrayscale(QImage &Img, double pctDone);
void FltAutoContrast(QImage &Img);
void FltBlur(QImage &Img,int radius);
void FltSharpen(QImage &Img,int radius);
void FltEdge(QImage &Img);
void FltCharcoal(QImage &Img);
void FltDespeckle(QImage &Img);
void FltAntialias(QImage &Img);
void FltGaussianBlur(QImage &Img,float radius,float sigma);
void FltGaussianSharpen(QImage &Img,float radius,float sigma);
void FltEqualize(QImage &Img);
void FltEmboss(QImage &Img, float radius, float sigma);
void FltOilPaint(QImage &Img, float radius=0.0);
void FltDesaturate(QImage &Img, float desat);
void FltSwirl(QImage &Img, float degrees);
void FltImplode(QImage &Img, float amount);

class ffd_filter
{
   public:
   static bool grayscale(QImage &img, bool reduceDepth=false) { FltGrayscale(img); return true; }
   static QImage blur(QImage &img, int radius=3) { FltBlur(img, radius); return img; }
   static QImage sharpen(QImage &img, int radius=3) { FltSharpen(img,radius); return img; }
   static QImage edge(QImage &img) { FltEdge(img); return img; }
   static QImage charcoal(QImage &img) { FltCharcoal(img); return img; }
   static QImage& despeckle(QImage &img) { FltDespeckle(img); return img; }
   static QImage antialias(QImage &img) { FltAntialias(img); return img; }
   static QImage gaussianBlur(QImage &img, float radius=0.0, float sigma=1.0) { FltGaussianBlur(img, radius, sigma); return img; }
   static QImage gaussianSharpen(QImage &img, float radius=0.0, float sigma=1.0/*, EffectQuality quality=High*/) { FltGaussianSharpen(img, radius, sigma); return img; }
   static bool equalize(QImage &img) { FltEqualize(img); return true; }
   static QImage emboss(QImage &img, float radius=0.0, float sigma=1.0/*,EffectQuality quality=High*/) { FltEmboss(img, radius, sigma); return img; }
   static QImage oilPaint(QImage &img, float radius=0.0/*,EffectQuality quality=High*/) { FltOilPaint(img, radius); return img; }
   static QImage& desaturate(QImage &img, float desat = 0.5) { FltDesaturate(img, desat); return img; }
   static QImage swirl(QImage &img, float degrees=60.0) { FltSwirl(img, degrees); return img; }
   static QImage implode(QImage &img, float amount=0.3) { FltImplode(img, amount); return img; }
};
#endif // _IMAGEFILTERS_H
