# 
# needs a lot of reworking: DEFINES += QT_NO_CAST_FROM_ASCII QT_NO_CAST_TO_ASCII QT_NO_CAST_FROM_BYTEARRAY QT_NO_URL_CAST_FROM_STRING
# needs some reworking:DEFINES += QT_USE_QSTRINGBUILDER
QMAKE_DEFAULT_INCDIRS = "\\"
win32 {
   contains(QMAKE_HOST.arch,x86_64) { 
      win32:QMAKE_LFLAGS_WINDOWS+=/MACHINE:X64
      win32:QMAKE_LFLAGS_WINDOWS_DLL+=/MACHINE:X64
   }
   else {
      win32:QMAKE_LFLAGS_WINDOWS+=/MACHINE:X86
      win32:QMAKE_LFLAGS_WINDOWS_DLL+=/MACHINE:X86 
   }
   QMAKE_LFLAGS_DEBUG+=/MANIFEST:NO
   QMAKE_CXXFLAGS += /MP
   CONFIG -= flat

   # OPENCV
   #INCLUDEPATH += d:/zw/opencv/opencv/build/include
   #LIBS += -Ld:/zw/opencv/opencv/build/x64/vc10/lib
   #LIBS += -lopencv_world300 -lopencv_ts300
   #DEFINES += WITH_OPENCV

   # ImageMagick
   #INCLUDEPATH += f:\src\ImageMagick-6.9.0\Magick++\lib 
   #INCLUDEPATH += f:\src\ImageMagick-6.9.0
   #LIBS += -Lf:\src\ImageMagick-6.9.0\VisualMagick\lib
   #CONFIG(debug, debug|release) {
   #   LIBS += -lCORE_DB_magick_ -lCORE_DB_Magick++_
   #} else {
   #   LIBS += -lCORE_RL_magick_ -lCORE_RL_Magick++_
   #} 
}

top_srcdir=$$PWD
message("pri top_srcdir is "$$top_srcdir)
CONFIG+=silent
#CONFIG(debug, debug|release) {
#   DESTDIR=/home/norbert/bin/ffDiaporama
#}
#CONFIG(relase, debug|release) {
#   DESTDIR=/home/norbert/bin/ffDiaporama
#}
#message ("DESTDIR IS "$$(DESTDIR))
