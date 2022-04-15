VERSION = 3.0.1

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

# The mac version of hdf5 does not use pkg-config
# we locate it manually
mac {
   message(Searching for hdf5 package on mac)
   system(which h5cc) {
   H5CC = $$system(which h5cc )
   H5DIR = $$system(dirname $$H5CC)/..
   message("HDF5 located in $$H5DIR")

   H5INC=$$system(dirname $$H5CC)/../include
   H5LIB=$$system(dirname $$H5CC)/../lib
   message($$H5LIB)
 
   INCLUDEPATH += $$H5INC
   LIBS        += -L$$H5LIB   }
   else {
     message("Cannot locate HDF5 package. Is it installed"?)
     message("Configuration of Qhfd failed")
   }

}
  
LIBS +=  -L../../../lib -lQatDataAnalysis -lQatGenericFunctions -L/usr/local/lib -lhdf5 -ldl


