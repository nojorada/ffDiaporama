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

#ifndef CBRUSHDEFINITION_H
#define CBRUSHDEFINITION_H

// Basic inclusions (common to all files)
#include "_GlobalDefines.h"

// Include some additional standard class
#include <QRectF>
#include <QPainter>
#include <QImage>
#include <QString>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

// Include some common various class
#include "cBaseBrushDefinition.h"
#include "cSpeedWave.h"
#include "Shape.h"
#include "cBaseMediaFile.h"
#include "cSoundBlockList.h"

//============================================
// Auto Framing
//============================================

class cAutoFramingDef 
{
   public:
      QString     ToolTip;
      int         GeometryType;
      bool        IsInternal;

      cAutoFramingDef() {}
      cAutoFramingDef(QString ToolTip, bool IsInternal, int GeometryType);
      cAutoFramingDef(const char *ToolTip, bool IsInternal,int GeometryType);
};
extern cAutoFramingDef AUTOFRAMINGDEF[NBR_AUTOFRAMING];
void   AutoFramingDefInit();

//*********************************************************************************************************************************************
// Base object for brush object
//*********************************************************************************************************************************************
class cCompositionObject;
class cBrushDefinition : public cBaseBrushDefinition, public xmlReadWrite 
{
   bool pushedImage;
   bool slidingBrush;
   // Image correction part
   struct brushParams {
      double X;             // X position (in %) relative to up/left corner
      double Y;             // Y position (in %) relative to up/left corner
      double XOffset;
      double YOffset;
      double ZoomFactor;    // Zoom factor (in %)
      double ImageRotation; // Image rotation (in °)
      int    Brightness;    // Brightness adjustment
      int    Contrast;      // Contrast adjustment
      double Gamma;         // Gamma adjustment
      int    Red;           // Red adjustment
      int    Green;         // Green adjustment
      int    Blue;          // Blue adjustment
      bool   LockGeometry;  // True if geometry is locked
      double AspectRatio;   // Aspect Ratio of image
      bool   FullFilling;   // Background image disk only : If true aspect ratio is not keep and image is deformed to fill the frame

      // filter-values
      double GaussBlurSharpenSigma,BlurSharpenRadius;       // Blur/Sharpen parameters
      int    TypeBlurSharpen,QuickBlurSharpenSigma;
      double Desat,Swirl,Implode;        // Filter parameters
      int    onOffFilter;                // On-Off filter = combination of Despeckle, Equalize, Gray and Negative;

      brushParams();
      void InitDefaultValues();
      bool hasFilter(OnOffFilter f) const { return (onOffFilter & f) != 0; }
      void setFilter(OnOffFilter f, bool set) { if (set) onOffFilter |= f; else onOffFilter &= ~f; }
      bool hasFilter();
      bool operator==(const brushParams &rhs);
      bool operator!=(const brushParams &rhs) { return !(*this == rhs); }
      bool isSlidingBrush(const brushParams &rhs);
      bool adjustValues(const brushParams &prev, double PctDone);
      bool adjustSlideValues(const brushParams &prev, double PctDone);
   };
   brushParams params;
   //QImage GetImageDiskBrushImage(QRectF Rect, bool PreviewMode, int64_t Position, cSoundBlockList *SoundTrackMontage, double PctDone, cBrushDefinition *PreviousBrush);
   //QImage GetImageDiskBrushVideo(QRectF Rect, bool PreviewMode, int64_t Position, cSoundBlockList *SoundTrackMontage, double PctDone, cBrushDefinition *PreviousBrush);
   //QImage GetImageDiskBrushGMaps(QRectF Rect, bool PreviewMode, int64_t Position, cSoundBlockList *SoundTrackMontage, double PctDone, cBrushDefinition *PreviousBrush);

   public:
      // Link to global objects
      cApplicationConfig *ApplicationConfig;
      cCompositionObject *CompositionObject;         // Link to parent (cCompositionObject)

      // Basic settings
      int TypeComposition; // Type of composition object (COMPOSITIONTYPE_BACKGROUND, COMPOSITIONTYPE_OBJECT, COMPOSITIONTYPE_SHOT)
      int ImageSpeedWave;  // Speed wave for this object during annimations

      // Embedded media object
      cBaseMediaFile *MediaObject;               // Embeded Media Object
      bool DeleteMediaObject;

      bool hasFilter();
            
      // Video specific part
      double SoundVolume;                // Volume of soundtrack
      bool   Deinterlace;                // Add a YADIF filter to deinterlace video (on/off)

      // Google maps specific part
      struct sMarker 
      {
         QString MarkerColor;
         QString TextColor;
         QString LineColor;
         enum MARKERVISIBILITY {MARKERHIDE,MARKERSHADE,MARKERSHOW,MARKERTABLE} Visibility;
         enum MARKERSIZE {SMALL,MEDIUM,LARGE,VERYLARGE};
      };
      QList<sMarker> Markers;

      explicit cBrushDefinition(cCompositionObject *CompositionObject, cApplicationConfig *TheApplicationConfig);
      ~cBrushDefinition();
                   
      virtual void InitDefaultValues();
                   
      void *GetDiaporamaObject();
      void *GetDiaporama();
        
      void CopyFromBrushDefinition(cBrushDefinition *BrushToCopy);
      void AddShotPartToXML(QDomElement *Element);
      void AddShotPartToXML(ExXmlStreamWriter &xmlStream);
      void SaveToXML(QDomElement *ParentElement,QString ElementName,QString PathForRelativPath,bool ForceAbsolutPath,cReplaceObjectList *ReplaceList,QList<qlonglong> *ResKeyList,bool IsModel);
      void SaveToXMLex(ExXmlStreamWriter &xmlStream, QString ElementName, QString PathForRelativPath, bool ForceAbsolutPath, cReplaceObjectList *ReplaceList, QList<qlonglong> *ResKeyList, bool IsModel);
      bool LoadFromXML(QDomElement *ParentElement,QString ElementName,QString PathForRelativPath,QStringList *AliasList,bool *ModifyFlag,TResKeyList *ResKeyList,bool DuplicateRes);
      virtual QBrush *GetBrush(QSizeF Size,bool PreviewMode,int Position,cSoundBlockList *SoundTrackMontage,double PctDone,cBrushDefinition *PreviousBrush, QRectF *srcRect = NULL);
      virtual QImage GetImage(QSizeF Size,bool PreviewMode,int Position,cSoundBlockList *SoundTrackMontage,double PctDone,cBrushDefinition *PreviousBrush);
      virtual QRectF GetImageRect(QSizeF Size,bool PreviewMode,int Position,cSoundBlockList *SoundTrackMontage,double PctDone,cBrushDefinition *PreviousBrush);

      int GetImageType();

      // Image correction part
      QImage ApplyFilter(QImage Image);
      //QImage ApplyFilters(QImage NewRenderImage,double TheBrightness,double TheContrast,double TheGamma,double TheRed,double TheGreen,double TheBlue,double TheDesat,bool ProgressifOnOffFilter,cBrushDefinition *PreviousBrush,double PctDone);
      void ApplyFilters(QImage &NewRenderImage,double TheBrightness,double TheContrast,double TheGamma,double TheRed,double TheGreen,double TheBlue,double TheDesat,bool ProgressifOnOffFilter,cBrushDefinition *PreviousBrush,double PctDone);


      // Utility functions used to draw thumbnails of image
      QImage *ImageToWorkspace(QImage *SrcImage,int WantedSize,qreal &maxw,qreal &maxh,qreal &minw,qreal &minh/*,qreal &x1,qreal &x2,qreal &x3,qreal &x4,qreal &y1,qreal &y2,qreal &y3,qreal &y4*/);
      void   ApplyMaskToImageToWorkspace(QImage *SrcImage,QRectF CurSelRect,int BackgroundForm,int AutoFramingStyle=-1);
      void   ApplyMaskToImageToWorkspace(QImage *SrcImage,int AutoFramingStyle,int BackgroundForm,int WantedSize,qreal maxw,qreal maxh,qreal minw,qreal minh,qreal X,qreal Y,qreal ZoomFactor,qreal AspectRatio,qreal ProjectGeometry);

      // Utility functions used to define default framing
      int  GetCurrentFramingStyle(qreal ProjectGeometry);
      bool CalcWorkspace(qreal &dmax,qreal &maxw,qreal &maxh,qreal &minw,qreal &minh/*,qreal &x1,qreal &x2,qreal &x3,qreal &x4,qreal &y1,qreal &y2,qreal &y3,qreal &y4*/);
      void ApplyAutoFraming(int AutoAdjust,qreal ProjectGeometry);
      void s_AdjustWTop(qreal ProjectGeometry);
      void s_AdjustWMidle(qreal ProjectGeometry);
      void s_AdjustWBottom(qreal ProjectGeometry);
      void s_AdjustHLeft(qreal ProjectGeometry);
      void s_AdjustHMidle(qreal ProjectGeometry);
      void s_AdjustHRight(qreal ProjectGeometry);
      void s_AdjustWH();
      void s_AdjustMinWTop(qreal ProjectGeometry);
      void s_AdjustMinWMidle(qreal ProjectGeometry);
      void s_AdjustMinWBottom(qreal ProjectGeometry);
      void s_AdjustMinHLeft(qreal ProjectGeometry);
      void s_AdjustMinHMidle(qreal ProjectGeometry);
      void s_AdjustMinHRight(qreal ProjectGeometry);
      void s_AdjustMinWH();

      QImage GetImageDiskBrush(QSizeF Size, bool PreviewMode, int64_t Position, cSoundBlockList *SoundTrackMontage, double PctDone, cBrushDefinition *PreviousBrush, QRectF *srcRect = NULL);
      QImage GetRealImageDiskBrush(QSizeF Size, bool PreviewMode, int64_t Position, cSoundBlockList *SoundTrackMontage, double PctDone, cBrushDefinition *PreviousBrush, QRectF *srcRect = NULL);
      void pushBack(const QImage &img, bool PreviewMode);
      bool isFromPushback(bool /*PreviewMode*/) { return /*PreviewMode ? */pushedImage /*: pushedRenderImage*/; }
      int GetHeightForWidth(int WantedWith,QRectF Rect);
      int GetWidthForHeight(int WantedHeight,QRectF Rect);

      void GetRealLocation(void **Location,void **RealLocation);
      void DrawMarker(QPainter *Painter,QPoint Position,int MarkerNum,sMarker::MARKERVISIBILITY Visibility,QSize MarkerSize,cBrushDefinition::sMarker::MARKERSIZE Size,bool DisplayType=false);
      void ComputeMarkerSize(void *Location,QSize MapImageSize);
      void AddMarkerToImage(QImage *DestImage);

      bool isVideo();
      bool isImageFile();
      bool isVectorImageFile();
      bool isGMapsMap();
      bool isStatic(cBrushDefinition* prevBrush);
      bool isSlidingBrush(cBrushDefinition* prevBrush);
      bool canCacheFilteredImages(double TheBrightness, double TheContrast, double TheGamma, double TheRed, double TheGreen, double TheBlue, double TheDesat, cBrushDefinition* prevBrush);
      void removeFromCache();



      // Image correction part
      #define GETSET(type, name) \
         type name() const { return params.name; } \
         void set##name(type val) { if( params.name != val ) removeFromCache(); params.name = val; }
      GETSET(double,X);
      GETSET(double,Y);
      GETSET(double,ZoomFactor);
      GETSET(double,ImageRotation);
      GETSET(int,Brightness);
      GETSET(int,Contrast);
      GETSET(double,Gamma);
      GETSET(int,Red);
      GETSET(int,Green);
      GETSET(int,Blue);
      GETSET(bool,LockGeometry);
      GETSET(double,AspectRatio);
      GETSET(bool,FullFilling);

      bool assignChanges(const cBrushDefinition& rhs, const cBrushDefinition& savedBrush);

      // filter-values
      GETSET(double,GaussBlurSharpenSigma);
      GETSET(double,BlurSharpenRadius);
      GETSET(int,TypeBlurSharpen);
      GETSET(int,QuickBlurSharpenSigma);

      GETSET(double,Desat);
      GETSET(double,Swirl);
      GETSET(double,Implode);
      int  OnOffFilters() const     { return params.onOffFilter; }
      void setOnOffFilters(int i)   { if( i != params.onOffFilter ) removeFromCache(); params.onOffFilter = i; }
      bool hasFilter(OnOffFilter f) { return params.hasFilter(f); }
      void setFilter(OnOffFilter f, bool set) { int oldFilter = params.onOffFilter; params.setFilter(f,set); if (oldFilter != params.onOffFilter) removeFromCache(); }

      QRectF appliedRect(double size);
#undef GETSET
   void preLoad(bool bPreview);
};

#endif // CBRUSHDEFINITION_H
