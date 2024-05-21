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

#include "cSpeedWave.h"
#include <QEasingCurve>
#include <QPainter>
#include <QImage>

#define PI2 (3.14159265/2)

double ComputePCT(int SpeedWave,double CurrentPCT) 
{
   CurrentPCT = qBound(0.0, CurrentPCT, 1.0);
   if( SpeedWave >= InQuad && SpeedWave < SPEEDWAVE_TYPE_NBR )
      return QEasingCurve( QEasingCurve::Type(SpeedWave-4) ).valueForProgress(CurrentPCT);

   switch (SpeedWave) 
   {
      case SPEEDWAVE_LINEAR           : return CurrentPCT;                                        break;
      case SPEEDWAVE_SINQUARTER       : return sin(PI2*CurrentPCT);                               break;
      case SPEEDWAVE_SINQUARTERx4     : return pow(sin(PI2*CurrentPCT),2)/pow(sin(PI2),2);        break;
      case SPEEDWAVE_POW2             : return pow(CurrentPCT,2);                                 break;
      case SPEEDWAVE_SQRT             : return sqrt(CurrentPCT);                                  break;
      case SPEEDWAVE_PROJECTDEFAULT   :
      default                         :
         std::cout << QString("Unrecognise SpeedWave in _SpeedWave::ComputePCT").toLocal8Bit().constData() << std::flush;
         exit(1);
   }
}

QString GetSpeedWaveName(int SpeedWave) 
{
   switch (SpeedWave) 
   {
      case SPEEDWAVE_PROJECTDEFAULT   : return QApplication::translate("Speed wave","Project default (%1)");          break;
      case SPEEDWAVE_LINEAR           : return QApplication::translate("Speed wave","Constant speed");                break;
      case SPEEDWAVE_SINQUARTER       : return QApplication::translate("Speed wave","Fast then slow");                break;
      case SPEEDWAVE_SINQUARTERx4     : return QApplication::translate("Speed wave","Slow then fast");                break;
      case SPEEDWAVE_POW2             : return QApplication::translate("Speed wave","Faster and faster");             break;
      case SPEEDWAVE_SQRT             : return QApplication::translate("Speed wave","Slower and slower");             break;
      case InQuad      :     return QApplication::translate("Speed wave","InQuad");          break;
      case OutQuad     :     return QApplication::translate("Speed wave","OutQuad");          break;
      case InOutQuad   :     return QApplication::translate("Speed wave","InOutQuad");          break;
      case OutInQuad   :     return QApplication::translate("Speed wave","OutInQuad");          break;
      case InCubic     :     return QApplication::translate("Speed wave","InCubic");          break;
      case OutCubic    :     return QApplication::translate("Speed wave","OutCubic");          break;
      case InOutCubic  :     return QApplication::translate("Speed wave","InOutCubic");          break;
      case OutInCubic  :     return QApplication::translate("Speed wave","OutInCubic");          break;
      case InQuart     :     return QApplication::translate("Speed wave","InQuart");          break;
      case OutQuart    :     return QApplication::translate("Speed wave","OutQuart");          break;
      case InOutQuart  :     return QApplication::translate("Speed wave","InOutQuart");          break;
      case OutInQuart  :     return QApplication::translate("Speed wave","OutInQuart");          break;
      case InQuint     :     return QApplication::translate("Speed wave","InQuint");          break;
      case OutQuint    :     return QApplication::translate("Speed wave","OutQuint");          break;
      case InOutQuint  :     return QApplication::translate("Speed wave","InOutQuint");          break;
      case OutInQuint  :     return QApplication::translate("Speed wave","OutInQuint");          break;
      case InSine      :     return QApplication::translate("Speed wave","InSine");          break;
      case OutSine     :     return QApplication::translate("Speed wave","OutSine");          break;
      case InOutSine   :     return QApplication::translate("Speed wave","InOutSine");          break;
      case OutInSine   :     return QApplication::translate("Speed wave","OutInSine");          break;
      case InExpo      :     return QApplication::translate("Speed wave","InExpo");          break;
      case OutExpo     :     return QApplication::translate("Speed wave","OutExpo");          break;
      case InOutExpo   :     return QApplication::translate("Speed wave","InOutExpo");          break;
      case OutInExpo   :     return QApplication::translate("Speed wave","OutInExpo");          break;
      case InCirc      :     return QApplication::translate("Speed wave","InCirc");          break;
      case OutCirc     :     return QApplication::translate("Speed wave","OutCirc");          break;
      case InOutCirc   :     return QApplication::translate("Speed wave","InOutCirc");          break;
      case OutInCirc   :     return QApplication::translate("Speed wave","OutInCirc");          break;
      case InElastic   :     return QApplication::translate("Speed wave","InElastic");          break;
      case OutElastic  :     return QApplication::translate("Speed wave","OutElastic");          break;
      case InOutElastic:     return QApplication::translate("Speed wave","InOutElastic");          break;
      case OutInElastic:     return QApplication::translate("Speed wave","OutInElastic");          break;
      case InBack      :     return QApplication::translate("Speed wave","InBack");          break;
      case OutBack     :     return QApplication::translate("Speed wave","OutBack");          break;
      case InOutBack   :     return QApplication::translate("Speed wave","InOutBack");          break;
      case OutInBack   :     return QApplication::translate("Speed wave","OutInBack");          break;
      case InBounce    :     return QApplication::translate("Speed wave","InBounce");          break;
      case OutBounce   :     return QApplication::translate("Speed wave","OutBounce");          break;
      case InOutBounce :     return QApplication::translate("Speed wave","InOutBounce");          break;
      case OutInBounce :     return QApplication::translate("Speed wave","OutInBounce");          break;
      default                         :
         std::cout << QString("Unrecognise SpeedWave in _SpeedWave::GetSpeedWaveName").toLocal8Bit().constData() << std::flush;
         exit(1);
   }
}

int mapSpeedwave(int speedwave)
{
   if( speedwave <= SPEEDWAVE_SQRT )
      return speedwave;
   return SPEEDWAVE_LINEAR;
}

double animValue(double start, double end, double currentPCT)
{
   //if( currentPCT > 1.0 )
   //   qDebug() << "anim Value is " <<  currentPCT*100.0;     
   //currentPCT = qBound(0.0, currentPCT, 1.0);
   return start + (end-start) * currentPCT;
}

QRectF animValue(QRectF start, QRectF end, double currentPCT)
{
   //if( currentPCT > 1.0 )
   //   qDebug() << "anim Value is " <<  currentPCT*100.0;     

   return QRectF( animDiffValue(start.x(), end.x(), currentPCT), animDiffValue(start.y(), end.y(), currentPCT),
                  animDiffValue(start.width(), end.width(), currentPCT), animDiffValue(start.height(), end.height(), currentPCT) );
}

double animDiffValue(double start, double end, double currentPCT)
{
   //currentPCT = qBound(0.0, currentPCT, 1.0);
   if( start != end )
      return animValue(start, end, currentPCT);
   return end;
}


// draw every speedwave into an png and save it
void speedwaves2pics()
{
   QImage img(128, 132, QImage::Format_ARGB32);

   double y;
   double vfp; // value for progress
   QPoint p1, p2;
   int minY = 100;
   int maxY = 1;
   double minVFP = 100.;
   double maxVFP = -1.;
   for (int wave = 0; wave < /*SPEEDWAVE_TYPE::*/SPEEDWAVE_TYPE_NBR; wave++)
   {
      img.fill(Qt::lightGray);
      QPainter p(&img);
      p.drawLine(14, 0, 14, 132);
      p.drawLine(0, 118, 132, 118);
      QPen penRed(Qt::red);
      penRed.setStyle(Qt::DotLine);
      p.setPen(penRed);
      p.drawLine(15, 18, 114, 18);
      p.drawLine(114, 18, 114, 118);
      QPen pen(Qt::blue);
      p.setPen(pen);
      for (int i = 0; i <= 100; i++)
      {
         y = i / 100.0;
         vfp = ComputePCT(wave, y);
         y = 118 - vfp*100.0;
         p2.setX(i+14);
         p2.setY(y);
         if (y < minY)
            minY = y;
         if (y > maxY)
            maxY = y;
         if (vfp < minVFP)
            minVFP = vfp;
         if (vfp > maxVFP)
            maxVFP = vfp;
         if (i == 0)
            img.setPixel(i+14, y, Qt::black);
         else
            p.drawLine(p1, p2);
         p1 = p2;
      }
      QString name = QString("sw%1").arg(wave, 3, 10, QChar('0'));
      if (wave >= 5)
         name = GetSpeedWaveName(wave);
      QString s = QString("f:/zw/") + name + ".png";
      img.save(s);
   }
   QEasingCurve ec(QEasingCurve::OutElastic);
//   qDebug() << "amp " << ec.amplitude();
//   qDebug() << "minY = " << minY << " maxY = " << maxY << " minVFP = " << minVFP << " maxVFP = " << maxVFP;
}
