VERSION = 2.0.0

isEmpty(PREFIX) {
  PREFIX=usr/local
}
  
isEmpty(QATLIBDIR) {
  QATLIBDIR=/$$PREFIX/lib
}

TEMPLATE = lib dll
TARGET = QHDF5
DEPENDPATH += .
INCLUDEPATH += . .. ../../QatDataAnalysis ../../QatGenericFunctions

QMAKE_CXX  = mpicxx
QMAKE_LINK = mpicxx

DESTDIR=../../../lib
CONFIG += build_all release c++11


linux {
  CONFIG += link_pkgconfig
  PKGCONFIG += hdf5-openmpi
}

# Input
SOURCES +=            *.cpp

INSTALLS += target
target.path=/$$QATLIBDIR

mac {
  INCLUDEPATH += /usr/local/include
}
  
LIBS += -L../../../lib -lQatDataAnalysis -lQatGenericFunctions -L/usr/local/lib -lhdf5 -ldl


