win32 {
   # OPENCV
   INCLUDEPATH += d:/zw/opencv3/opencv/build/include
   DEFINES += WITH_OPENCV

   contains(QMAKE_HOST.arch,x86_64) {
   	CONFIG(debug, debug|release)  {
	   	OBJECTS_DIR_BASE = g:/obj/blitz/debug
   	}
	   CONFIG(release, debug|release) {
   		OBJECTS_DIR_BASE = g:/obj/blitz/release
      }
      LIBS += -Ld:/zw/opencv3/opencv/build/x64/vc11/lib
   }
   else {
   	CONFIG(debug, debug|release)  {
	   	OBJECTS_DIR_BASE = g:/obj/blitz_x86/debug
   	}
	   CONFIG(release, debug|release) {
   		OBJECTS_DIR_BASE = g:/obj/blitz_x86/release
      }
      LIBS += -Ld:/zw/opencv3/opencv/build/x86/vc11/lib
   }
   LIBS += -lopencv_world300 -lopencv_ts300
	CONFIG(debug, debug|release)  {
		OBJECTS_DIR = $${OBJECTS_DIR_BASE}\\$${TARGET}
		UI_DIR  = $${OBJECTS_DIR_BASE}\\$${TARGET}\\ui
		MOC_DIR = $${OBJECTS_DIR_BASE}\\$${TARGET}\\moc
		RCC_DIR = $${OBJECTS_DIR_BASE}\\$${TARGET}\\moc
		OBJMOC  = $${OBJECTS_DIR_BASE}\\$${TARGET}\\mocobj
	}
	CONFIG(release, debug|release) {
		OBJECTS_DIR = $${OBJECTS_DIR_BASE}\\$${TARGET}
		UI_DIR  = $${OBJECTS_DIR_BASE}\\$${TARGET}\\ui
		MOC_DIR = $${OBJECTS_DIR_BASE}\\$${TARGET}\\moc
		RCC_DIR = $${OBJECTS_DIR_BASE}\\$${TARGET}\\moc
		OBJMOC  = $${OBJECTS_DIR_BASE}\\$${TARGET}\\mocobj
	}
   DESTDIR=f:/blitz
   contains(QMAKE_HOST.arch,x86_64) {
      DESTDIR=f:/blitz_x64
   }
   LIBS += -L$${DESTDIR}

   !debug_and_release|build_pass {
	    CONFIG(debug, debug|release) {
          TARGET = $$member(TARGET, 0)d
          !isEmpty(QMAKE_TARGET_DESCRIPTION){
				   QMAKE_TARGET_DESCRIPTION = $$member(QMAKE_TARGET_DESCRIPTION,0) (DEBUG-Version)
          }
	   }
   }
   QMAKE_LFLAGS_RELEASE += /OPT:NOREF

}
