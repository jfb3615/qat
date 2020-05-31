VERSION = 2.0.0

isEmpty(PREFIX) {
  PREFIX=/usr/local
}
  
isEmpty(QATLIBDIR) {
  QATLIBDIR=$$PREFIX/lib
}

TEMPLATE = lib dll
TARGET = QatPlotWidgets
DEPENDPATH += .
INCLUDEPATH += . .. ../../QatPlotting ../../QatDataAnalysis ../../QatGenericFunctions
DESTDIR=../../../lib
FORMS=LabelEditorDialog.ui PVControl.ui PlotDirDialog.ui ParameterDialog.ui MultipleViewWindow.ui MultipleViewWidget.ui CharSelWidget.ui
CONFIG += qt release c++17
QT     += widgets svg printsupport

# Input
HEADERS += ../QatPlotWidgets/CustomRangeDivider.h \
           ../QatPlotWidgets/RangeDivision.h   \
           ../QatPlotWidgets/AbsRangeDivider.h \
           ../QatPlotWidgets/LinearRangeDivider.h \
           ../QatPlotWidgets/LogRangeDivider.h \
           ../QatPlotWidgets/PlotView.h \
           ../QatPlotWidgets/LabelEditorDialog.h \
           ../QatPlotWidgets/PVControl.h \
           ../QatPlotWidgets/PlotHist1DDialog.h \
           ../QatPlotWidgets/PlotHist2DDialog.h \
           ../QatPlotWidgets/PlotDirDialog.h \
           ../QatPlotWidgets/PlotFunction1DDialog.h \
           ../QatPlotWidgets/PlotOrbitDialog.h \
           ../QatPlotWidgets/ParameterDialog.h \
           ../QatPlotWidgets/LinearSubdivider.h \
           ../QatPlotWidgets/LogSubdivider.h \
           ../QatPlotWidgets/MultipleViewWindow.h \
           ../QatPlotWidgets/MultipleViewWidget.h \
           ../QatPlotWidgets/CharSelWidget.h

SOURCES += CustomRangeDivider.cpp \
           RangeDivision.cpp \
           AbsRangeDivider.cpp \
           LinearRangeDivider.cpp \
           LogRangeDivider.cpp \
           PlotView.cpp \
           LabelEditorDialog.cpp \
           PVControl.cpp \
           PlotHist1DDialog.cpp \
           PlotHist2DDialog.cpp \
           PlotDirDialog.cpp \
           PlotFunction1DDialog.cpp \
           PlotOrbitDialog.cpp \
           ParameterDialog.cpp \
           LinearSubdivider.cpp \
           LogSubdivider.cpp \
           MultipleViewWindow.cpp \
           MultipleViewWidget.cpp \
           CharSelWidget.cpp

target.path=$$QATLIBDIR
INSTALLS += target
                       
headers.path=$$PREFIX/include/QatPlotWidgets
headers.files= ../QatPlotWidgets/*.h ../QatPlotWidgets/*.icc ui_PlotDirDialog.h ui_LabelEditorDialog.h ui_PVControl.h ui_ParameterDialog.h
INSTALLS += headers 

pc.path  = $$QATLIBDIR/pkgconfig
mac {
  pc.files = ../pkgconfig/mac/*.pc
}
linux {
  pc.files = ../pkgconfig/linux/*.pc
}
INSTALLS += pc

RESOURCES = PlotWidgets.qrc

LIBS += -L$(DESTDIR) -lQatPlotting -lQatGenericFunctions



