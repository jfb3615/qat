# === Preamble ===
cmake_minimum_required(VERSION 3.10)

set ( CMAKE_CXX_STANDARD 20 )

# === Project's settings ===
include( ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Qat-version.cmake )
project( "Qat" VERSION ${Qat_VERSION} LANGUAGES CXX )
message (STATUS "Building library version ${Qat_VERSION} " )
# Make the 'cmake' module directory visible to CMake.
list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake )

option (QAT_USE_QT6 "Use QT6 for Qat components" ON )
if (QAT_USE_QT6)
  message (STATUS "Use QT6" )
  set (QT_VERSION 6)
else()
  message (STATUS "Use QT5" )
  set (QT_VERSION 5)
endif()

option (QAT_BUILD_QAT3D "Build the QAT 3D component" ON) 


# Use the GNU install directory names.
include( GNUInstallDirs )
set(CMAKE_CXX_FLAGS "-DQATBINDIR=${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR} -DQATSHAREDIR=${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATAROOTDIR} -DQATMANDIR=${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_MANDIR} -DQATLIBDIR=${CMAKE_INSTALL_FULL_LIBDIR}")


add_subdirectory(QatGenericFunctions)
add_subdirectory(QatDataAnalysis)
add_subdirectory(QatDataModeling)
add_subdirectory(QatPlotting)
add_subdirectory(QatPlotWidgets)
add_subdirectory(Qhfd)
add_subdirectory(QatProject)
if (QAT_BUILD_QAT3D)
  message (STATUS "Building QatInventorWidgets" )
  add_subdirectory(QatInventorWidgets)
endif()



