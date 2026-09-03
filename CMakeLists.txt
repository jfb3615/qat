# === Preamble ===
cmake_minimum_required(VERSION 3.28)

set ( CMAKE_CXX_STANDARD 20 )
set ( CMAKE_BUILD_TYPE Release)
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
set(CMAKE_INSTALL_RPATH "$ORIGIN")
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

include(CMakePackageConfigHelpers)
if (QAT_BUILD_QAT3D)
  set(QAT_3D_FIND_DEPENDENCY "find_dependency(QatInventorWidgets)")
else()
  set(QAT_3D_FIND_DEPENDENCY "")
endif()

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/QatConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/QatConfig.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Qat"
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/QatConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/QatConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/QatConfigVersion.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Qat"
)



