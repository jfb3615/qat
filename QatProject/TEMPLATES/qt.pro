TEMPLATE = app
TARGET = <app>
DESTDIR = ../local/bin
DEPENDPATH += .
INCLUDEPATH += . .. 

mac {
  CONFIG -= app_bundle
}


CONFIG += qt debug c++20

# Input
SOURCES += *.cpp

QMAKE_DEL_FILE=rm -rf
QMAKE_DISTCLEAN += ../local

mac {
PKG_CONFIG_PATH += $$[QT_HOST_PREFIX]/opt/qt/libexec/lib/pkgconfig:$$[QT_INSTALL_LIBS]/pkgconfig
  PKG_CONFIG = PKG_CONFIG_PATH=$$PKG_CONFIG_PATH pkg-config
}

CONFIG    += link_pkgconfig

#              Dependency diagram
#==============================================
#                                             |
#       QatInventorWidgets                    |
#          /          \                       |
#         /             Coin                  |
#  QatPlotWidgets                             |
#    \                                        |
#    QatPlotting        QatDataModeling       |
#   /  \               /                      |
# Qt    QatDataAnalysis                       |
#        \                                    |
#         QatGenericFunctions                 |
#         \       \                           |
#          eigen  gsl                         |
#=============================================      

# This package depends on and inherits
# dependencies of (change as needed):

PKGCONFIG += QatPlotWidgets



