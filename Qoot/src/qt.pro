VERSION = 3.0.1

isEmpty(PREFIX) {
  PREFIX=usr/local
}
  
isEmpty(QATLIBDIR) {
  QATLIBDIR=/$$PREFIX/lib
}

TEMPLATE = lib dll
TARGET = Qoot
DEPENDPATH += .
INCLUDEPATH += . .. ../../QatDataAnalysis ../../QatGenericFunctions /usr/include/root

DESTDIR=../../../lib
CONFIG += build_all release c++14

# Input
SOURCES +=            RootDriver.cpp

INSTALLS += target
target.path=/$$QATLIBDIR

mac {
  INCLUDEPATH += /usr/local/include
}
QMAKE_LDFLAGS += -fPIC  
QMAKE_CXXFLAGS += -fPIC  
LIBS += -L../../../lib -lQatDataAnalysis -lQatGenericFunctions  -lgsl -lgslcblas -lm -L/usr/local/lib  -L/usr/lib/root -lHist -lMatrix -lTree -lRIO -lNet -lMathCore -lCore -ldl


