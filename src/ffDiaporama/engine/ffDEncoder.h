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

#ifndef FFDENCODER_H
#define FFDENCODER_H

// Basic inclusions (common to all files)
#include "_GlobalDefines.h"

// Specific inclusions
#include "_Diaporama.h"

#define SUPPORTED_COMBINATION       0
#define UNSUPPORTED_COMBINATION     1
#define INVALID_COMBINATION         2
#define VBRMINCOEF                  0.5
#define VBRMAXCOEF                  1.25

static int CheckEncoderCapabilities(VFORMAT_ID FormatId,AVCodecID VideoCodec,AVCodecID AudioCodec);

//************************************************

class ffD_Encoder : public QObject
{
   Q_OBJECT
   cDiaporama *Diaporama;
   bool IsOpen,InterleaveFrame;
   volatile bool Continue;
public:
   // Container parameters & buffers
   QString OutputFileName;
   int FromSlide,ToSlide;
   int64_t NbrFrame;                       // Number of frame to generate
   AVFormatContext *Container;

   // Video parameters & buffers
   AVRational VideoFrameRate;
   int        VideoBitrate;
   int        VideoCodecSubId;
   sIMAGEDEF  *ImageDef;

   qreal               LastVideoPts;
   qreal               IncreasingVideoPts;
   int64_t             VideoFrameNbr;
   AVStream            *VideoStream;
   qreal               dFPS;
   int                 InternalWidth,InternalHeight,ExtendV;
   AVFrame             *VideoFrame;
   struct SwsContext   *VideoFrameConverter;           // Converter from QImage to YUV image
   u_int8_t            *VideoEncodeBuffer;             // Buffer for encoded image
   int                 VideoEncodeBufferSize;          // Buffer for encoded image
   int64_t             VideoFrameBufSize;
   u_int8_t            *VideoFrameBuf;

   // Audio parameters & buffers
   int AudioChannels;
   int AudioBitrate;
   int AudioSampleRate;
   int AudioCodecSubId;

   qreal    LastAudioPts;
   qreal    IncreasingAudioPts;
   int64_t  AudioFrameNbr;
   AVStream *AudioStream;
   AVFrame  *AudioFrame;
   u_int8_t *AudioResamplerBuffer;          // Buffer for sampled audio
   int      AudioResamplerBufferSize;
   SwrContext *AudioResampler;

      // Progress display settings
      bool StopProcessWanted;              // True if user click on cancel or close during encoding process
      QTime StartTime;                      // Time the process start

      int64_t RenderedFrame;
      qreal   Position;
      int     Column,ColumnStart,AdjustedDuration;

      CompositionContextList PreparedTransitBrushList;
      CompositionContextList PreparedBrushList;

      QSemaphore preloadSlidesCounter;
      QFutureWatcher<void> ThreadPreload;
      QFutureWatcher<void> ThreadAssembly;
      QFutureWatcher<void> ThreadEncodeVideo;
      QFutureWatcher<void> ThreadEncodeAudio;

      ffD_Encoder();
      ~ffD_Encoder();

      bool OpenEncoder(QWidget *ParentWindow,cDiaporama *Diaporama,QString OutputFileName,int FromSlide,int ToSlide,
         bool EncodeVideo,int VideoCodecSubId,bool VBR,sIMAGEDEF *ImageDef,int ImageWidth,int ImageHeight,int ExtendV,int InternalWidth,int InternalHeight,AVRational PixelAspectRatio,int VideoBitrate,
         bool EncodeAudio,int AudioCodecSubId,int AudioChannels,int AudioBitrate,int AudioSampleRate,QString Language);
      bool DoEncode();
      void flush_encoders();
      void CloseEncoder();

   signals:
      void ImageReady(QImage img);

   private:
      QFutureWatcher<void> ThreadEncoding;
      QList<cDiaporamaObjectInfo *> renderedFrames;
      QSemaphore maxRenderedFrames;
      QSemaphore RenderedFrames;
      QMutex rfListMutex;
      int  getThreadFlags(AVCodecID ID);
      bool AddStream(AVStream **Stream,AVCodec **codec,const char *CodecName,AVMediaType Type);
      bool OpenVideoStream(sVideoCodecDef *VideoCodecDef,int VideoCodecSubId,bool VBR,AVRational VideoFrameRate,int ImageWidth,int ImageHeight,AVRational PixelAspectRatio,int VideoBitrate);
      bool OpenAudioStream(sAudioCodecDef *AudioCodecDef,int &AudioChannels,int &AudioBitrate,int &AudioSampleRate,QString Language);

      bool PrepareTAG(QString Language);
      QString AdjustMETA(QString Text);

      void Assembly(cDiaporamaObjectInfo *Frame,cDiaporamaObjectInfo *PreviousFrame,cSoundBlockList *RenderMusic,cSoundBlockList *ToEncodeMusic,volatile bool &Continue);
      void EncodeMusic(cDiaporamaObjectInfo *Frame,cSoundBlockList *RenderMusic,cSoundBlockList *ToEncodeMusic,volatile bool &Continue);
      void EncodeVideo(QImage *ImageList,volatile bool &Continue);
      void Encode(cDiaporamaObjectInfo *Frame, cDiaporamaObjectInfo *PreviousFrame, cSoundBlockList *RenderMusic, cSoundBlockList *ToEncodeMusic/*, QImage *Image*/);

      void initAudioResampler(const cSoundBlockList &RenderMusic, const cSoundBlockList &ToEncodeMusic);
public:
      bool produceFrames();
      void encodeFrames(cSoundBlockList *RenderMusic,cSoundBlockList *ToEncodeMusic);
};

#endif // FFDENCODER_H
