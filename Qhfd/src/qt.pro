VERSION = 3.0.3

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

QMAKE_CXX  = $$system(pkg-config --variable=prefix ompi-cxx)/bin/mpicxx
QMAKE_LINK = $$system(pkg-config --variable=prefix ompi-cxx)/bin/mpicxx

DESTDIR=../../../lib
CONFIG += build_all release c++11

# Input
SOURCES +=            *.cpp

INSTALLS += target
target.path=/$$QATLIBDIR

LIBS +=  -L../../../lib -lQatDataAnalysis -lQatGenericFunctions -L/usr/local/lib -ldl
            
# The mac version of hdf5 does not use pkg-config
# we locate it manually
mac {
   message(Searching for hdf5 package on mac)
   system(which h5cc) {
   H5CC = $$system(which h5cc )
   H5DIR = $$system(dirname $$H5CC)/..
   H5CLN = $$clean_path($$H5DIR)
   message("HDF5 installation located under $$H5CLN")

   H5INC=$$H5CLN/include
   H5LIB=$$H5CLN/lib
 
   INCLUDEPATH += $$H5INC
   LIBS        += -L$$H5LIB   }
   else {
     message("Cannot locate HDF5 package. Is it installed"?)
     message("Configuration of Qhfd failed")
   }

}
else {
  LIBS           += $$system(pkg-config -libs   hdf5-openmpi) 
  QMAKE_CXXFLAGS += $$system(pkg-config -cflags hdf5-openmpi) 
}




