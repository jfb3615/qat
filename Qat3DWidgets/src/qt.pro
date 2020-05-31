VERSION = 2.0.0

isEmpty(PREFIX) {
  PREFIX=/usr/local
}
  
isEmpty(QATLIBDIR) {
  QATLIBDIR=$$PREFIX/lib
}

TEMPLATE = lib dll
TARGET = Qat3DWidgets
DEPENDPATH += .
INCLUDEPATH += . .. 
DESTDIR=../../../lib
FORMS=QatViewerWidget.ui 
CONFIG += qt release c++11
QT     += widgets 3dcore 3drender 3dinput 3dextras

# Input
HEADERS += ../Qat3DWidgets/QatViewerWidget.h 


SOURCES += QatViewerWidget.cpp 

target.path=$$QATLIBDIR
INSTALLS += target
                       
headers.path=$$PREFIX/include/Qat3DWidgets
headers.files= ../Qat3DWidgets/*.h 
INSTALLS += headers 

pc.path  = $$QATLIBDIR/pkgconfig
mac {
  pc.files = ../pkgconfig/mac/*.pc
}
linux {
  pc.files = ../pkgconfig/linux/*.pc
}
  
INSTALLS += pc




