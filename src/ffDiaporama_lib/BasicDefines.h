/* ======================================================================
    This file is part of ffDiaporama
    ffDiaporama is a tool to make diaporama as video
    Copyright (C) 2015-2019 Gerhard Kokerbeck <ffDiaporama@sbk2day.com>
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
   ======================================================================
    THIS FILE MUST ABSOLUTELY BE REFERENCED AT FIRST IN ALL .h FILES OF
    THE PROJECT
   ====================================================================== */


#ifndef FFD_BASICDEFINES_H
#define FFD_BASICDEFINES_H

// Remove unwanted warnings when using MSVC
#ifndef _DEPRECATION_DISABLE                /* One time only */
    #define _DEPRECATION_DISABLE            /* Disable deprecation true */
    #if (_MSC_VER >= 1400)                  /* Check version */
        #pragma warning(disable: 4996)      /* Disable deprecation */
        #pragma warning(disable: 4005)      /* Disable warning on macro redefinition */
        #pragma warning(disable: 4100)      /* Disable warning on macro redefinition */
    #endif                                  /* #if defined(NMEA_WIN) && (_MSC_VER >= 1400) */
#endif                                      /* #ifndef _DEPRECATION_DISABLE */

//============================================
// Activate standard stdint macro
//============================================

#ifdef _STDINT_H
    #undef _STDINT_H            // Remove previous inclusion (if exist)
#endif

#define __STDC_CONSTANT_MACROS  // Activate macro for stdint
#include <stdint.h>             // Include stdint with macro activated
#include <stdlib.h>
#include <iostream>

//============================================
// Specific for MSVC
//============================================

#ifdef _MSC_VER
    #define snprintf    _snprintf_s
    #define AVRCAST                     // MSC doesn't allow CAST in struct constant definition
#else
    #define AVRCAST (AVRational)        // mingw need CAST in struct constant definition
    #include <unistd.h>
#endif

#ifdef Q_OS_LINUX
    #define u_int64_t uint64_t
    #define u_int32_t uint32_t
    #define u_int16_t uint16_t
    #define u_int8_t uint8_t
#endif

//============================================
// Minimum QT inclusions needed by all files
//============================================
#include <QtCore>
#include <QApplication>
#include <QtDebug>
#include <QList>
#include <QString>
#include <QStringList>
#include <QDomElement>

#if QT_VERSION >= 0x050000
    #include <QtConcurrent>
#endif

// some macros for keeping prototypes for some QT-Functions in sync
#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
#define QT_IMAGESIZE_IN_BYTES sizeInBytes
#define FILECREATIONDATE birthTime
#else
#define QT_IMAGESIZE_IN_BYTES byteCount
#define FILECREATIONDATE created
#endif

#if (QT_VERSION >= QT_VERSION_CHECK(5, 11, 0))
#define QT_FONT_WIDTH horizontalAdvance
#else
#define QT_FONT_WIDTH width
#endif

#if (QT_VERSION >= QT_VERSION_CHECK(5, 13, 0))
#define QT_SWAP swapItemsAt
#define QT_QSORT std::sort
#else
#define QT_SWAP swap
#define QT_QSORT qSort
#endif

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
#define QT_STABLE_SORT std::stable_sort
#define QT_SETTEXTOLOR setForeground
#define QT_CONCURRENT_RUN(obj,func) QtConcurrent::run(func,obj)
#define QT_CONCURRENT_RUN_1(obj,func,arg1) QtConcurrent::run(func,obj,arg1)
#define QT_CONCURRENT_RUN_2(obj,func,arg1,arg2) QtConcurrent::run(func,obj,arg1,arg2)
#define QT_CONCURRENT_RUN_3(obj,func,arg1,arg2,arg3) QtConcurrent::run(func,obj,arg1,arg2,arg3)
#define QT_CONCURRENT_RUN_4(obj,func,arg1,arg2,arg3,arg4) QtConcurrent::run(func,obj,arg1,arg2,arg3,arg4)
#define QT_CONCURRENT_RUN_5(obj,func,arg1,arg2,arg3,arg4,arg5) QtConcurrent::run(func,obj,arg1,arg2,arg3,arg4,arg5)
#else
#define QT_STABLE_SORT qStableSort
#define QT_SETTEXTOLOR setTextColor
#define QT_CONCURRENT_RUN(obj,func) QtConcurrent::run(obj,func)
#define QT_CONCURRENT_RUN_1(obj,func,arg1) QtConcurrent::run(obj,func,arg1)
#define QT_CONCURRENT_RUN_2(obj,func,arg1,arg2) QtConcurrent::run(obj,func,arg1,arg2)
#define QT_CONCURRENT_RUN_3(obj,func,arg1,arg2,arg3) QtConcurrent::run(obj,func,arg1,arg2,arg3)
#define QT_CONCURRENT_RUN_4(obj,func,arg1,arg2,arg3,arg4) QtConcurrent::run(obj,func,arg1,arg2,arg3,arg4)
#define QT_CONCURRENT_RUN_5(obj,func,arg1,arg2,arg3,arg4,arg5) QtConcurrent::run(obj,func,arg1,arg2,arg3,arg4,arg5)
#endif

//====================================================================
// For windows, windows.h and winbase.h must be included after QtCore
//====================================================================
#ifdef Q_OS_WIN
    #include <windows.h>
    #include <winbase.h>

    // These type are undefined under Windows
    typedef unsigned char       u_int8_t;
    typedef unsigned short      u_int16_t;
    typedef unsigned            u_int32_t;
    typedef unsigned long long  u_int64_t;
#endif

//====================================================================
// Math
//====================================================================

#include <cmath>
#define _USE_MATH_DEFINES   // for msvc
#ifndef M_PI
    // Some cmath doesn't define it,try from the older source
    #include "math.h"
#endif

//====================================================================
// Internal log defines and functions
//====================================================================

// Log level for message
#define LOGMSG_DEBUGTRACE                   1
#define LOGMSG_INFORMATION                  2
#define LOGMSG_WARNING                      3
#define LOGMSG_CRITICAL                     4

const QEvent::Type BaseAppEvent = QEvent::Type(2000);   // The custom event will be send to EventReceiver (if EventReceiver not null)
#define EVENT_GeneralLogChanged             1           // General internal event code to display log message

extern int          LogMsgLevel;                        // Level from wich debug message was print to stdout
extern QStringList  EventList;                          // Internal event queue
extern QObject      *EventReceiver;                     // Windows wich receive event

void    PostEvent(int EventType,QString EventParam = QLatin1String(""));
void    ToLog(int MessageType,QString Message,QString Source = QLatin1String("internal"),bool AddBreak=true);
#define TOLOG(a,b) { if( a >= LogMsgLevel ) { ToLog(a,b); } }

//====================================================================
// Standard project geometry definition
//====================================================================

enum ffd_GEOMETRY {
    GEOMETRY_4_3,
    GEOMETRY_16_9,
    GEOMETRY_40_17,
    GEOMETRY_THUMBNAIL,
    GEOMETRY_SQUARE,
    GEOMETRY_NONE
};

//============================================
// Media object types
//============================================

enum OBJECTTYPE {
    OBJECTTYPE_UNMANAGED,
    OBJECTTYPE_MANAGED,
    OBJECTTYPE_FOLDER,
    OBJECTTYPE_FFDFILE,
    OBJECTTYPE_IMAGEFILE,
    OBJECTTYPE_VIDEOFILE,
    OBJECTTYPE_MUSICFILE,
    OBJECTTYPE_THUMBNAIL,
    OBJECTTYPE_IMAGEVECTOR,
    OBJECTTYPE_IMAGECLIPBOARD,
    OBJECTTYPE_GMAPSMAP
};

#define PREVIEWMAXHEIGHT    720         // Max height for preview image
//#define PREVIEWMAXHEIGHT    1440         // Max height for preview image

//============================================
// Utility functions
//============================================

double GetDoubleValue(QDomElement CorrectElement, QString Name);                                        // Load a double value from an XML element
double GetDoubleValue(QString sValue);                                                                 // Load a double value from a string
bool CalcWorkspace(qreal ImgWidth, qreal ImgHeight, double ImageRotation, qreal dmax, qreal *maxw, qreal *maxh, qreal *minw, qreal *minh);
#define deleteList(l) while(!l.isEmpty() ) delete l.takeLast()

// timing
#ifndef LONGLONG
//typedef __int64 LONGLONG;
typedef long long LONGLONG;
#endif
const char *PC2time(LONGLONG ticks, bool withMillis);
LONGLONG curPCounter();

class autoTimer
{
   QString messageText;
   QElapsedTimer timer;
   public:
      autoTimer(const QString &s);
      ~autoTimer();
};

#ifdef _DEBUG
#define AUTOTIMER(a,b) autoTimer a(b)
#else
#define AUTOTIMER(a,b) 
#endif

#endif // _BASICDEFINES_H
