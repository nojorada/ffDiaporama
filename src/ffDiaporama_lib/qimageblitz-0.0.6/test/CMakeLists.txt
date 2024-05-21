project(blitztest)

if (WIN32)
 set(CMAKE_DEBUG_POSTFIX "d")
endif (WIN32)

include_directories(
${CMAKE_SOURCE_DIR}
${blitztest_BINARY_DIR}
${blitz_lib_SOURCE_DIR}
${QT_INCLUDE_DIR}
${QT_QTCORE_INCLUDE_DIR}
${QT_QTGUI_INCLUDE_DIR}
)

set(blitztest_SRCS main.cpp mainwindow.cpp scaledialog.cpp)

qt4_automoc(${blitztest_SRCS}) 

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy) 

add_executable(blitztest ${blitztest_SRCS})

target_link_libraries(blitztest ${QT_QTCORE_LIBRARY}
${QT_QTGUI_LIBRARY} qimageblitz)

install(TARGETS blitztest DESTINATION ${BIN_INSTALL_DIR})

