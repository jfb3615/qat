# qat
The QAT libraries contain software for function manipulation, data collection,  data modeling, and plotting. These are intended for students and practitioners  of the physical sciences.

# To build qat libraries

* cd qat
* mkdir build
* cd build
* cmake ..
* make
* sudo make install

# prerequisites

* cmake
*Qt libraries.  Either Qt5 or Qt6 is possible.
* Coin (for 3D graphics)
* SoQt.  It may be built for Qt5 or Qt6.
* eigen (for vector and matrix operations)
* gsl   (the gnu scientific library)


# configuration

Qt6 is used by default.  To configure for Qt5, add the flag -DQAT_USE_QT6=OFF to the cmake step. 

