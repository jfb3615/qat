VERSION = 3.0.3


isEmpty(PREFIX) {
  PREFIX=usr/local
}
  
isEmpty(QATLIBDIR) {
  QATLIBDIR=/$$PREFIX/lib
}

isEmpty(QATPCDIR) {
  QATPCDIR=/$$PREFIX/share/pkgconfig
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

headers.path=/$$PREFIX/include/QatGenericFunctions
headers.files= ../QatGenericFunctions/*h ../QatGenericFunctions/*.icc
INSTALLS += headers


QMAKE_SUBSTITUTES += ../qmakeconfig/QatGenericFunctions.pc.in
pc.path  = $$QATPCDIR
pc.files = ../qmakeconfig/*.pc

INSTALLS += pc

CONFIG += link_pkgconfig
PKGCONFIG += eigen3


