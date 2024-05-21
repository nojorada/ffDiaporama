call qt_version.bat
set PATH=%MQT%;%PATH%
call e:\"Microsoft Visual Studio 10.0"\VC\vcvarsall.bat amd64
%MQT%\qmake -tp vc -spec win32-msvc2010 -r 
pause
