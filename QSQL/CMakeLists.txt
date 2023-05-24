# Set up the project.
cmake_minimum_required( VERSION 3.10 )
set  (CMAKE_CXX_STANDARD 17)
project( "QSQLite" VERSION 3.0.3 LANGUAGES CXX )
set    ( CMAKE_BUILD_TYPE RelWithDebInfo )

# Add the project's modules directory to CMAKE_MODULE_PATH:
list( INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_SOURCE_DIR}/modules )

# Use the GNU install directory names.
include( GNUInstallDirs )

# Project's dependencies.SQLite::SQLite3

#----------------------------------------------------------------------------
#
set(OUTPUT bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${OUTPUT})

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp  )

add_library(QSQLite SHARED ${sources} ${headers})

find_package (SQLite3 REQUIRED)

target_link_libraries(QSQLite PRIVATE SQLite::SQLite3 QatDataAnalysis QatGenericFunctions )
#target_include_directories(QSQLite PUBLIC ${ROOT_INCLUDE_DIR} 
#   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
#   $<INSTALL_INTERFACE:include> )

install( TARGETS QSQLite
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Runtime
   NAMELINK_SKIP )

