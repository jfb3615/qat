VERSION = 3.0.1

isEmpty(PREFIX) {
  PREFIX=usr/local
}
  
isEmpty(QATLIBDIR) {
  QATLIBDIR=/$$PREFIX/lib
}

TEMPLATE = lib dll
TARGET = QatPlotting
DEPENDPATH += .
INCLUDEPATH += . .. ../../QatDataAnalysis ../../QatGenericFunctions 

QT += widgets 

DESTDIR=../../../lib
CONFIG += build_all release c++17

# Input
HEADERS += ../QatPlotting/AbsPlotter.h \
           ../QatPlotting/LinToLog.h \
           ../QatPlotting/PlotStream.h \
           ../QatPlotting/WPlotStream.h \
           ../QatPlotting/Plotable.h \
           ../QatPlotting/PlotHist1D.h \
           ../QatPlotting/PlotHist1DProperties.h \
           ../QatPlotting/PlotHist2D.h \
           ../QatPlotting/PlotHist2DProperties.h \
           ../QatPlotting/PlotFunction1DProperties.h \
           ../QatPlotting/PlotFunction1D.h \
           ../QatPlotting/PlotProfileProperties.h \
           ../QatPlotting/PlotProfile.h \
           ../QatPlotting/PlotErrorEllipseProperties.h \
           ../QatPlotting/PlotErrorEllipse.h \
           ../QatPlotting/PlotOrbit.h \
           ../QatPlotting/PlotOrbitProperties.h \
           ../QatPlotting/PlotPointProperties.h \
           ../QatPlotting/PlotPoint.h \
           ../QatPlotting/PlotResidual1D.h \
           ../QatPlotting/PlotResidual1DProperties.h \
           ../QatPlotting/RealArg.h \
           ../QatPlotting/PlotRectProperties.h \
           ../QatPlotting/PlotRect.h \
           ../QatPlotting/PlotShadeBox.h \
           ../QatPlotting/PlotText.h \
           ../QatPlotting/PlotKey.h \
           ../QatPlotting/PlotMeasure.h \
           ../QatPlotting/PlotMeasureProperties.h \
           ../QatPlotting/PlotBand1D.h \
           ../QatPlotting/PlotBand1DProperties.h \
           ../QatPlotting/VisFunction.h \
           ../QatPlotting/PlotWave1D.h \
           ../QatPlotting/PlotWave1DProperties.h \
           ../QatPlotting/PlotFunction1D.icc \
           ../QatPlotting/PRectF.h

SOURCES += AbsPlotter.cpp \
           Plotable.cpp \
           PlotHist1D.cpp \
           PlotHist2D.cpp \
           PlotFunction1D.cpp  \
           PlotOrbit.cpp \
           PlotProfile.cpp \
           PlotErrorEllipse.cpp \
           PlotPoint.cpp \
           PlotResidual1D.cpp \
           PlotShadeBox.cpp \
           RealArg.cpp \
           PlotRect.cpp \
           PlotText.cpp \
           PlotKey.cpp \
           PlotMeasure.cpp \
           PlotBand1D.cpp \
           VisFunction.cpp \
           PlotWave1D.cpp 

INSTALLS += target
target.path=/$$QATLIBDIR

headers.path=/$$PREFIX/include/QatPlotting
headers.files= ../QatPlotting/*h ../QatPlotting/*.icc
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


