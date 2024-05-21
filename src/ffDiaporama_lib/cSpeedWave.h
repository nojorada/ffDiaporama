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

#ifndef _SPEEDWAVE_H
#define _SPEEDWAVE_H

#define SPEEDWAVE_DISABLE        -2
#define SPEEDWAVE_PROJECTDEFAULT -1

#include "BasicDefines.h"

enum SPEEDWAVE_TYPE  
{
   SPEEDWAVE_LINEAR,
   SPEEDWAVE_SINQUARTER, // same as easeOutSine
   SPEEDWAVE_SINQUARTERx4,
   SPEEDWAVE_POW2,
   SPEEDWAVE_SQRT,
   // QT EasyCurves
   InQuad, OutQuad, InOutQuad, OutInQuad,
   InCubic, OutCubic, InOutCubic, OutInCubic,
   InQuart, OutQuart, InOutQuart, OutInQuart,
   InQuint, OutQuint, InOutQuint, OutInQuint,
   InSine, OutSine, InOutSine, OutInSine,
   InExpo, OutExpo, InOutExpo, OutInExpo,
   InCirc, OutCirc, InOutCirc, OutInCirc,
   InElastic, OutElastic, InOutElastic, OutInElastic,
   InBack, OutBack, InOutBack, OutInBack,
   InBounce, OutBounce, InOutBounce, OutInBounce,

   SPEEDWAVE_TYPE_NBR                  // Last of the list
};

double  ComputePCT(int SpeedWave,double CurrentPCT);
QString GetSpeedWaveName(int SpeedWave);
double animValue(double start, double end, double currentPCT);
QRectF animValue(QRectF start, QRectF end, double currentPCT);
double animDiffValue(double start, double end, double currentPCT);
int mapSpeedwave(int);
void speedwaves2pics();
#endif // _SPEEDWAVE_H
