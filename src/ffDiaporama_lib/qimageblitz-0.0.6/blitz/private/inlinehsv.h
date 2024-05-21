#ifndef __PIXIE_INLINEHSV_H
#define __PIXIE_INLINEHSV_H

/*
 Copyright (C) 2005, 2007 Daniel M. Duley <daniel.duley@verizon.net>

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include <QColor>

/**
 * HSV to RGB conversion is required often for some algorithms. This
 * class inlines, (at least with gcc), most of the code required for this
 * conversion in order to increase performance in loops. The idea is you
 * create an instance of this class outside your main loop then call
 * convertRGB2HSV() and convertHSV2RGB when you need it.
 *
 * @author Daniel M. Duley
 * @short Inline HSV conversion
 */
class InlineHSV
{
public:
    InlineHSV(){;}
    /**
     * Converts the given RGB value to HSV. The hue(), saturation(), and
     * value() methods will be valid after calling this. The red, green,
     * and blue items are also set the the supplied values.
     */
    inline void convertRGB2HSV(unsigned int rgb);
    inline void convertRGB2HSV(int red, int green, int blue);
    /**
     * Converts the given HSV value to RGB. The red(), green(), and
     * blue() methods will be valid after calling this. The hue,
     * saturation, and value methods are also set to the supplied values.
     */
    inline void convertHSV2RGB(int hue, int saturation, int value);
    /**
     * Converts the current hue, saturation, and value settings to RGB.
     */
    inline void convertHSV2RGB();

    inline void setHSV(int hue, int saturation, int value)
        {h = hue; s = saturation; v = value;}
    inline void setHue(int hue){h = hue;}
    inline void setSaturation(int saturation){s = saturation;}
    inline void setValue(int value){v = value;}
    inline int hue(){return(h);}
    inline int saturation(){return(s);}
    inline int value(){return(v);}

    inline void setRGB(int red, int green, int blue)
        {r = red; g = green; b=blue;}
    inline void setRed(int red){r = red;}
    inline void setGreen(int green){g = green;}
    inline void setBlue(int blue){b = blue;}
    inline int red(){return(r);}
    inline int green(){return(g);}
    inline int blue(){return(b);}
private:
    int h, s, v;
    int r, g, b;
    int max, whatmax, min, delta;
    unsigned int f, p, q, t;
};

inline void InlineHSV::convertRGB2HSV(unsigned int pixel)
{
    convertRGB2HSV(qRed(pixel), qGreen(pixel), qBlue(pixel));
}

#define noQT_LIKE
inline void InlineHSV::convertRGB2HSV(int red, int green, int blue)
{
    r = red; g = green; b = blue;
    h = 0;
    #ifdef QT_LIKE
#define Q_MAX_3(a, b, c) ( ( a > b && a > c) ? a : (b > c ? b : c) )
#define Q_MIN_3(a, b, c) ( ( a < b && a < c) ? a : (b < c ? b : c) )


    const qreal _r = red   / qreal(USHRT_MAX);
    const qreal _g = green / qreal(USHRT_MAX);
    const qreal _b = blue  / qreal(USHRT_MAX);
    const qreal max = Q_MAX_3(_r, _g, _b);
    const qreal min = Q_MIN_3(_r, _g, _b);
    const qreal delta = max - min;
    v = qRound(max * USHRT_MAX);
    if (qFuzzyIsNull(delta)) {
        // achromatic case, hue is undefined
        h = USHRT_MAX;
        s = 0;
    } else {
        // chromatic case
        qreal hue = 0;
        s = qRound((delta / max) * USHRT_MAX);
        if (qFuzzyCompare(r, max)) {
            hue = ((_g - _b) /delta);
        } else if (qFuzzyCompare(_g, max)) {
            hue = (qreal(2.0) + (_b - _r) / delta);
        } else if (qFuzzyCompare(b, max)) {
            hue = (qreal(4.0) + (_r - _g) / delta);
        } else {
            Q_ASSERT_X(false, "QColor::toHsv", "internal error");
        }
        hue *= qreal(60.0);
        if (hue < qreal(0.0))
            hue += qreal(360.0);
        h = qRound(hue * 100);
    }
    #else
    max = r;                               // maximum RGB component
    whatmax = 0;                            // r=>0, g=>1, b=>2
    if(g > max) { max = g; whatmax = 1; }
    if(b > max) { max = b; whatmax = 2; }
    min = r;                               // find minimum value
    if(g < min) min = g;
    if(b < min) min = b;
    delta = max-min;
    v = max;                                   // calc value
    s = max ? (510*delta+max)/(2*max) : 0;
    if(s == 0)
        h = -1;                                // undefined hue
    else{
        switch(whatmax){
        case 0:                             // red is max component
            h = (g >= b) ? (120*(g-b)+delta)/(2*delta) :
                (120*(g-b+delta)+delta)/(2*delta) + 300;
            break;
        case 1:                             // green is max component
            h = (b > r) ? h = 120 + (120*(b-r)+delta)/(2*delta) :
                60 + (120*(b-r+delta)+delta)/(2*delta);
            break;
        case 2:                             // blue is max component
            h = (r > g) ? h = 240 + (120*(r-g)+delta)/(2*delta) :
                h = 180 + (120*(r-g+delta)+delta)/(2*delta);
            break;
        }
    }
    #endif
}

inline void InlineHSV::convertHSV2RGB(int hue, int saturation, int value)
{
    h = hue; s = saturation; v = value;
    convertHSV2RGB();
}

inline void InlineHSV::convertHSV2RGB()
{
#ifdef QT_LIKE
   if (s == 0 || h == USHRT_MAX) {
         // achromatic case
         r = g = b = v;
         return;
   }

   // chromatic case
   const qreal _h = h == 36000 ? 0 : h / 6000.;
   const qreal _s = s / qreal(USHRT_MAX);
   const qreal _v = v / qreal(USHRT_MAX);
   const int i = int(_h);
   const qreal f = _h - i;
   const qreal p = _v * (qreal(1.0) - _s);

   if (i & 1) {
         const qreal q = _v * (qreal(1.0) - (_s * f));

         switch (i) {
         case 1:
            r = qRound(q * USHRT_MAX);
            g = qRound(_v * USHRT_MAX);
            b = qRound(p * USHRT_MAX);
            break;
         case 3:
            r = qRound(p * USHRT_MAX);
            g = qRound(q * USHRT_MAX);
            b = qRound(_v * USHRT_MAX);
            break;
         case 5:
            r = qRound(_v * USHRT_MAX);
            g = qRound(p * USHRT_MAX);
            b = qRound(q * USHRT_MAX);
            break;
         }
   } 
   else 
   {
         const qreal t = _v * (qreal(1.0) - (_s * (qreal(1.0) - f)));

         switch (i) {
         case 0:
            r = qRound(_v * USHRT_MAX);
            g = qRound(t * USHRT_MAX);
            b = qRound(p * USHRT_MAX);
            break;
         case 2:
            r = qRound(p * USHRT_MAX);
            g = qRound(_v * USHRT_MAX);
            b = qRound(t * USHRT_MAX);
            break;
         case 4:
            r = qRound(t * USHRT_MAX);
            g = qRound(p * USHRT_MAX);
            b = qRound(_v * USHRT_MAX);
            break;
         }
      }
#else
    if(h < -1 || s > 255 || v > 255)
        return;

    r = g = b = v;
    if(s > 0 && h != -1){
        if(h >= 360) h %= 360;
        f = h%60;
        h /= 60;
        p = (2*v*(255-s)+255)/510;
        if(h&1){
            q = (2*v*(15300-s*f)+15300)/30600;
            switch( h ) {
                case 1: r=q; g=v, b=p; break;
                case 3: r=p; g=q, b=v; break;
                case 5: r=v; g=p, b=q; break;
            }
        } else {
            t = (2*v*(15300-(s*(60-f)))+15300)/30600;
            switch( h ) {
                case 0: r=v; g=t, b=p; break;
                case 2: r=p; g=v, b=t; break;
                case 4: r=t; g=p, b=v; break;
            }
        }
    }
#endif
}

#endif

