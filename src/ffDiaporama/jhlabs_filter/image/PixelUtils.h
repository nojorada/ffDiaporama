#ifndef __PixelUtils__
#define __PixelUtils__
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


#include <QtMath>
/**
 * Some more useful math functions for image processing.
 * These are becoming obsolete as we move to Java2D. Use MiscComposite instead.
 */
class PixelUtils 
{
   public:
   enum PixlOps {
      eREPLACE = 0,
      eNORMAL = 1,
      eMIN = 2,
      eMAX = 3,
      eADD = 4,
      eSUBTRACT = 5,
      eDIFFERENCE = 6,
      eMULTIPLY = 7,
      eHUE = 8,
      eSATURATION = 9,
      eVALUE = 10,
      eCOLOR = 11,
      eSCREEN = 12,
      eAVERAGE = 13,
      eOVERLAY = 14,
      eCLEAR = 15,
      eEXCHANGE = 16,
      eDISSOLVE = 17,
      eDST_IN = 18,
      eALPHA = 19,
      eALPHA_TO_GRAY = 20
   };
	//static const int REPLACE = 0;
	//static const int NORMAL = 1;
	//static const int MIN = 2;
	//static const int MAX = 3;
	//static const int ADD = 4;
	//static const int SUBTRACT = 5;
	//static const int DIFFERENCE = 6;
	//static const int MULTIPLY = 7;
	//static const int HUE = 8;
	//static const int SATURATION = 9;
	//static const int VALUE = 10;
	//static const int COLOR = 11;
	//static const int SCREEN = 12;
	//static const int AVERAGE = 13;
	//static const int OVERLAY = 14;
	//static const int CLEAR = 15;
	//static const int EXCHANGE = 16;
	//static const int DISSOLVE = 17;
	//static const int DST_IN = 18;
	//static const int ALPHA = 19;
	//static const int ALPHA_TO_GRAY = 20;

	private:
    //static Random randomGenerator = new Random();

	/**
	 * Clamp a value to the range 0..255
	 */
	public:
    static int clamp(int c) 
    {
		if (c < 0)
			return 0;
		if (c > 255)
			return 255;
		return c;
	}

	static int interpolate(int v1, int v2, float f) {
		return clamp((int)(v1+f*(v2-v1)));
	}
	
	static int brightness(int rgb) {
		int r = (rgb >> 16) & 0xff;
		int g = (rgb >> 8) & 0xff;
		int b = rgb & 0xff;
		return (r+g+b)/3;
	}
	
	static bool nearColors(int rgb1, int rgb2, int tolerance) {
		int r1 = (rgb1 >> 16) & 0xff;
		int g1 = (rgb1 >> 8) & 0xff;
		int b1 = rgb1 & 0xff;
		int r2 = (rgb2 >> 16) & 0xff;
		int g2 = (rgb2 >> 8) & 0xff;
		int b2 = rgb2 & 0xff;
		return qFabs(r1-r2) <= tolerance && qFabs(g1-g2) <= tolerance && qFabs(b1-b2) <= tolerance;
	}
	
	private:
     static float hsb1[3];// = new float[3];//FIXME-not thread safe
	  static float hsb2[3];// = new float[3];//FIXME-not thread safe
	
	// Return rgb1 painted onto rgb2
	public:
   static int combinePixels(int rgb1, int rgb2, int op) {
		return combinePixels(rgb1, rgb2, op, 0xff);
	}
	
	static int combinePixels(int rgb1, int rgb2, int op, int extraAlpha, int channelMask) {
		return (rgb2 & ~channelMask) | combinePixels(rgb1 & channelMask, rgb2, op, extraAlpha);
	}
	
	static int combinePixels(int rgb1, int rgb2, int op, int extraAlpha) {
		if (op == eREPLACE)
			return rgb1;
		int a1 = (rgb1 >> 24) & 0xff;
		int r1 = (rgb1 >> 16) & 0xff;
		int g1 = (rgb1 >> 8) & 0xff;
		int b1 = rgb1 & 0xff;
		int a2 = (rgb2 >> 24) & 0xff;
		int r2 = (rgb2 >> 16) & 0xff;
		int g2 = (rgb2 >> 8) & 0xff;
		int b2 = rgb2 & 0xff;

		switch (op) {
		case eNORMAL:
			break;
		case eMIN:
			r1 = qMin(r1, r2);
			g1 = qMin(g1, g2);
			b1 = qMin(b1, b2);
			break;
		case eMAX:
			r1 = qMax(r1, r2);
			g1 = qMax(g1, g2);
			b1 = qMax(b1, b2);
			break;
		case eADD:
			r1 = clamp(r1+r2);
			g1 = clamp(g1+g2);
			b1 = clamp(b1+b2);
			break;
		case eSUBTRACT:
			r1 = clamp(r2-r1);
			g1 = clamp(g2-g1);
			b1 = clamp(b2-b1);
			break;
		case eDIFFERENCE:
			r1 = clamp(qFabs(r1-r2));
			g1 = clamp(qFabs(g1-g2));
			b1 = clamp(qFabs(b1-b2));
			break;
		case eMULTIPLY:
			r1 = clamp(r1*r2/255);
			g1 = clamp(g1*g2/255);
			b1 = clamp(b1*b2/255);
			break;
		case eDISSOLVE:
			if ((rand()/*randomGenerator.nextInt()*/ & 0xff) <= a1) {
				r1 = r2;
				g1 = g2;
				b1 = b2;
			}
			break;
		case eAVERAGE:
			r1 = (r1+r2)/2;
			g1 = (g1+g2)/2;
			b1 = (b1+b2)/2;
			break;
		case eHUE:
		case eSATURATION:
		case eVALUE:
		case eCOLOR:
			// todo Color.RGBtoHSB(r1, g1, b1, hsb1);
			// todo Color.RGBtoHSB(r2, g2, b2, hsb2);
			switch (op) {
			case eHUE:
				hsb2[0] = hsb1[0];
				break;
			case eSATURATION:
				hsb2[1] = hsb1[1];
				break;
			case eVALUE:
				hsb2[2] = hsb1[2];
				break;
			case eCOLOR:
				hsb2[0] = hsb1[0];
				hsb2[1] = hsb1[1];
				break;
			}
			// todo rgb1 = Color.HSBtoRGB(hsb2[0], hsb2[1], hsb2[2]);
			r1 = (rgb1 >> 16) & 0xff;
			g1 = (rgb1 >> 8) & 0xff;
			b1 = rgb1 & 0xff;
			break;
		case eSCREEN:
			r1 = 255 - ((255 - r1) * (255 - r2)) / 255;
			g1 = 255 - ((255 - g1) * (255 - g2)) / 255;
			b1 = 255 - ((255 - b1) * (255 - b2)) / 255;
			break;
		case eOVERLAY:
			int m, s;
			s = 255 - ((255 - r1) * (255 - r2)) / 255;
			m = r1 * r2 / 255;
			r1 = (s * r1 + m * (255 - r1)) / 255;
			s = 255 - ((255 - g1) * (255 - g2)) / 255;
			m = g1 * g2 / 255;
			g1 = (s * g1 + m * (255 - g1)) / 255;
			s = 255 - ((255 - b1) * (255 - b2)) / 255;
			m = b1 * b2 / 255;
			b1 = (s * b1 + m * (255 - b1)) / 255;
			break;
		case eCLEAR:
			r1 = g1 = b1 = 0xff;
			break;
		case eDST_IN:
			r1 = clamp((r2*a1)/255);
			g1 = clamp((g2*a1)/255);
			b1 = clamp((b2*a1)/255);
			a1 = clamp((a2*a1)/255);
			return (a1 << 24) | (r1 << 16) | (g1 << 8) | b1;
		case eALPHA:
			a1 = a1*a2/255;
			return (a1 << 24) | (r2 << 16) | (g2 << 8) | b2;
		case eALPHA_TO_GRAY:
			int na = 255-a1;
			return (a1 << 24) | (na << 16) | (na << 8) | na;
		}
		if (extraAlpha != 0xff || a1 != 0xff) {
			a1 = a1*extraAlpha/255;
			int a3 = (255-a1)*a2/255;
			r1 = clamp((r1*a1+r2*a3)/255);
			g1 = clamp((g1*a1+g2*a3)/255);
			b1 = clamp((b1*a1+b2*a3)/255);
			a1 = clamp(a1+a3);
		}
		return (a1 << 24) | (r1 << 16) | (g1 << 8) | b1;
	}

};
#endif // __PixelUtils__
