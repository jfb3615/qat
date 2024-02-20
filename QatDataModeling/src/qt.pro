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
TARGET = QatDataModeling
DEPENDPATH += .
INCLUDEPATH += . .. ../../QatGenericFunctions ../../QatDataAnalysis ../Minuit2

DESTDIR=../../../lib
CONFIG += build_all release c++17


# Input
HEADERS +=            ../QatDataModeling/*.h 


SOURCES +=            *.cpp ../Minuit2/src/*.cxx

target.path=/$$QATLIBDIR
INSTALLS += target

headers.path=/$$PREFIX/include/QatDataModeling
headers.files= ../QatDataModeling/*h ../QatDataModeling/*.icc
INSTALLS += headers



QMAKE_SUBSTITUTES += ../qmakeconfig/QatDataModeling.pc.in
pc.path  = $$QATPCDIR
pc.files = ../qmakeconfig/*.pc

INSTALLS += pc

CONFIG += link_pkgconfig
PKGCONFIG += eigen3


LIBS += -L$(DESTDIR) -lQatDataAnalysis -lQatGenericFunctions 


