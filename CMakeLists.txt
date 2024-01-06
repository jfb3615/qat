# === Preamble ===
cmake_minimum_required(VERSION 3.16...3.26)

# === Project's settings ===
include( ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Qat-version.cmake )
project( "Qat" VERSION ${Qat_VERSION} LANGUAGES CXX )
message (STATUS "Building library version ${Qat_VERSION} " )
# Make the 'cmake' module directory visible to CMake.
list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake )


# Use the GNU install directory names.
include( GNUInstallDirs )
set(CMAKE_CXX_FLAGS "-DQATBINDIR=${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR} -DQATSHAREDIR=${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATAROOTDIR} -DQATMANDIR=${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_MANDIR} ")
add_subdirectory(QatGenericFunctions)
add_subdirectory(QatDataAnalysis)
add_subdirectory(QatDataModeling)
add_subdirectory(QatPlotting)
add_subdirectory(QatPlotWidgets)
add_subdirectory(Qhfd)
add_subdirectory(QatProject)
add_subdirectory(QatInventorWidgets)



