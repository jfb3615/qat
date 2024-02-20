VERSION=3.0.3

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
TARGET = QatDataAnalysis
DEPENDPATH += .
INCLUDEPATH += . .. ../../QatGenericFunctions

DESTDIR=../../../lib
CONFIG += build_all release c++17


QMAKE_CXXFLAGS += -DQATLIBDIR=$$QATLIBDIR
# Input
HEADERS +=            ../QatDataAnalysis/*.h ../QatDataAnalysis/*.icc

SOURCES +=            *.cpp

target.path=$$QATLIBDIR
INSTALLS += target

headers.path=/$$PREFIX/include/QatDataAnalysis
headers.files= ../QatDataAnalysis/*h ../QatDataAnalysis/*.icc
INSTALLS += headers
QMAKE_SUBSTITUTES += ../qmakeconfig/QatDataAnalysis.pc.in
pc.path  = $$QATPCDIR
pc.files = ../qmakeconfig/*.pc

INSTALLS += pc

LIBS += -L$(DESTDIR) -lQatGenericFunctions -ldl


