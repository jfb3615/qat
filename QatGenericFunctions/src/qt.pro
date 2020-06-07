VERSION = 3.0.1


isEmpty(PREFIX) {
  PREFIX=/usr/local
}
  
isEmpty(QATLIBDIR) {
  QATLIBDIR=$$PREFIX/lib
}

TEMPLATE = lib dll
TARGET = QatGenericFunctions
DEPENDPATH += .
INCLUDEPATH += . .. 

DESTDIR=../../../lib
CONFIG += build_all release c++17

# Input
HEADERS +=            ../QatGenericFunctions/*.h ../QatGenericFunctions/*.icc

SOURCES +=            *.cpp
 
target.path=$$QATLIBDIR
INSTALLS += target

headers.path=$$PREFIX/include/QatGenericFunctions
headers.files= ../QatGenericFunctions/*h ../QatGenericFunctions/*.icc
INSTALLS += headers

pc.path  = $$QATLIBDIR/pkgconfig
mac {
  pc.files = ../pkgconfig/mac/*.pc
}
linux {
  pc.files = ../pkgconfig/linux/*.pc
}
INSTALLS += pc

CONFIG += link_pkgconfig
PKGCONFIG += eigen3


