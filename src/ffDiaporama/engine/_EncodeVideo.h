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

#ifndef _ENCODEVIDEO_H
#define _ENCODEVIDEO_H

// Basic inclusions (common to all files)
#include "_GlobalDefines.h"

// Specific inclusions
#include "_Diaporama.h"
#include <QSharedPointer>
#include <QElapsedTimer>

#define SUPPORTED_COMBINATION       0
#define UNSUPPORTED_COMBINATION     1
#define INVALID_COMBINATION         2
#define VBRMINCOEF                  0.5
#define VBRMAXCOEF                  1.25

int CheckEncoderCapabilities(VFORMAT_ID FormatId,AVCodecID VideoCodec,AVCodecID AudioCodec);

typedef struct OutputStream
{
   AVStream            *stream;
   AVCodecContext      *cc;
   AVFrame             *frame;
   qreal               LastPts;
   qreal               IncreasingPts;
   OutputStream() { stream = NULL; cc = NULL; frame = NULL; LastPts = 0; IncreasingPts = 0; }
} OutputStream;

#define GET(varType,name) varType get##name(){ return name;}
#define GET_SET(varType,name) varType get##name() { return name;} void set##name(varType value) { name=value;}
//************************************************
class cEncodeVideo : public QObject
{
   Q_OBJECT
      OutputStream ostVideo;
      OutputStream ostAudio;

    cDiaporama          *Diaporama;
    bool                IsOpen,InterleaveFrame;
    QMutex  encoderMutex;

    // Container parameters & buffers
    QString             OutputFileName;
    int                 FromSlide,ToSlide;
    int64_t             NbrFrame;                       // Number of frame to generate
    AVFormatContext     *Container;
    bool enableFaststart;

    // Video parameters & buffers
    AVRational          VideoFrameRate;
    int                 VideoBitrate;
    bool                useCRF;
    int                 iCRF;
    int                 VideoCodecSubId;
    sIMAGEDEF           *ImageDef;

    int64_t             VideoFrameNbr;

    qreal               dFPS;
    int                 InternalWidth,InternalHeight,ExtendV;
    int                 iRenderWidth, iRenderHeight;
    bool                bDSR;
    struct SwsContext   *VideoFrameConverter;           // Converter from QImage to YUV image
    struct SwsContext   *VideoYUVConverter;             // Converter from YUV to YUV image
    u_int8_t            *VideoEncodeBuffer;             // Buffer for encoded image
    int64_t             VideoFrameBufSize;
    u_int8_t            *VideoFrameBuf;

    // Audio parameters & buffers
    int                 AudioChannels;
    int                 AudioBitrate;
    int                 AudioSampleRate;
    int                 AudioCodecSubId;

    int64_t             AudioFrameNbr;
    u_int8_t            *AudioResamplerBuffer;          // Buffer for sampled audio
    int                 AudioResamplerBufferSize;
    SwrContext          *AudioResampler;

    // Progress display settings
    bool                StopProcessWanted;              // True if user click on cancel or close during encoding process
    QTime               StartTime;                      // Time the process start
	 QElapsedTimer		previewTimer;
	 qint64 lastPreview;
    bool showPreview;
    bool renderOnly;
    bool yuvPassThroughSignal;

    volatile int64_t RenderedFrame;
    volatile int64_t EncodedFrames;
    qreal Position;
    int Column,ColumnStart,AdjustedDuration;

    CompositionContextList PreparedTransitBrushList;
    CompositionContextList PreparedBrushList;

    QSemaphore preloadSlidesCounter;
    QFutureWatcher<void> ThreadPreload;
    QFutureWatcher<void> ThreadAssembly;
    QFutureWatcher<void> ThreadEncodeVideo;
    QFutureWatcher<void> ThreadEncodeAudio;

public:
   cEncodeVideo();
    ~cEncodeVideo();

    bool OpenEncoder(QWidget *ParentWindow,cDiaporama *Diaporama,QString OutputFileName,int FromSlide,int ToSlide,
             bool EncodeVideo,int VideoCodecSubId,bool VBR,sIMAGEDEF *ImageDef,int ImageWidth,int ImageHeight,int ExtendV,int InternalWidth,int InternalHeight,AVRational PixelAspectRatio,int VideoBitrate,
             bool useCRF, int iCRF,
             bool EncodeAudio,int AudioCodecSubId,int AudioChannels,int AudioBitrate,int AudioSampleRate,QString Language);
    bool DoEncode();
    void CloseEncoder();
    bool hasVideoStream() { return ostVideo.stream != NULL; }
    bool hasAudioStream() { return ostAudio.stream != NULL; }

    void enablePreview(bool bEnable) { showPreview = bEnable; }
    void setRenderOnly(bool bEnable) { renderOnly = bEnable; }

    void setStopProcessWanted() { StopProcessWanted = true; }
    bool isStopProcessWanted() { return StopProcessWanted == true;  }
    void setRenderOnly() { renderOnly = true; }
    bool isRenderOnly() { return renderOnly; }
    int getVideoCodecSubId() { return VideoCodecSubId; }
    sIMAGEDEF *getImageDef() { return ImageDef; }

    QTime getStartTime() { return StartTime; }
    GET_SET(int, FromSlide)
    GET_SET(int, ToSlide)
    bool isEnableFaststart() { return enableFaststart; }
    void setEnableFaststart(bool enable) { enableFaststart = enable; }

    GET(int64_t, EncodedFrames);
    GET(int64_t, RenderedFrame)
    GET(int64_t, NbrFrame);
    GET(int, AdjustedDuration)
    GET(qreal, dFPS)
    GET(int, Column)
    GET(int, ColumnStart)
    GET(qreal, Position)
    GET_SET(int, AudioBitrate);
    GET_SET(int, AudioSampleRate);
    GET_SET(int, AudioCodecSubId);
    signals:
      void ImageReady(QImage img);
      void YUVPassThrough();
private:

    int  getThreadFlags(AVCodecID ID);
    bool AddStream(AVStream **Stream,const AVCodec **codec,const char *CodecName,AVMediaType Type);
    bool OpenVideoStream(sVideoCodecDef *VideoCodecDef,int VideoCodecSubId,bool VBR,AVRational VideoFrameRate,int ImageWidth,int ImageHeight,AVRational PixelAspectRatio,int VideoBitrate);
    bool OpenAudioStream(sAudioCodecDef *AudioCodecDef,int &AudioChannels,int &AudioBitrate,int &AudioSampleRate,QString Language);
    void flush_encoders();

    bool PrepareTAG(QString Language);
    QString AdjustMETA(QString Text);

    bool encode(AVCodecContext *ctx, AVFrame *frame, AVPacket *pkt);
    void EncodeMusic(cDiaporamaObjectInfo *Frame,cSoundBlockList *RenderMusic,cSoundBlockList *ToEncodeMusic,volatile bool *Continue);
    void EncodeVideo(const QImage *ImageList,volatile bool *Continue);
    void EncodeVideoFrame(AVFrame *frame, volatile bool *Continue);

    typedef struct _sframe
    {
      QSharedPointer<cDiaporamaObjectInfo> Frame;
      QSharedPointer<cDiaporamaObjectInfo> PreviousFrame;
      _sframe(QSharedPointer<cDiaporamaObjectInfo> f, QSharedPointer<cDiaporamaObjectInfo> pf) 
      {
         Frame = f;
         PreviousFrame = pf;
      }
    } sFrames;

    QList<sFrames> frameList;
    QSemaphore      frameListFree, frameListFilled;
    QMutex          frameListMutex;
    cSoundBlockList ToEncodeMusic, RenderMusic;
    QFutureWatcher<void> ThreadEncode;
    bool allDone;
    void Assembly(QSharedPointer<cDiaporamaObjectInfo> Frame,QSharedPointer<cDiaporamaObjectInfo> PreviousFrame,volatile bool *Continue);
    void Encoding(volatile bool *Continue);
    bool testEncode();
};

#endif // _ENCODEVIDEO_H
