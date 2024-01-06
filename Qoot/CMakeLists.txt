# Set up the project.
cmake_minimum_required( VERSION 3.10 )
set  (CMAKE_CXX_STANDARD 17)
project( "QOOT" VERSION ${Qat_VERSION} LANGUAGES CXX )
set    ( CMAKE_BUILD_TYPE RelWithDebInfo )

# Add the project's modules directory to CMAKE_MODULE_PATH:
list( INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_SOURCE_DIR}/modules )

# Use the GNU install directory names.
include( GNUInstallDirs )

# Project's dependencies.
find_package( ROOT REQUIRED )
#----------------------------------------------------------------------------
#
set(OUTPUT bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${OUTPUT})

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp  )

add_library(Qoot SHARED ${sources} ${headers})


target_link_libraries(Qoot PUBLIC ${ROOT_LIBRARIES} QatDataAnalysis QatGenericFunctions )
target_include_directories(Qoot PUBLIC ${ROOT_INCLUDE_DIR} 
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )


set_target_properties( Qoot PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )

install( TARGETS Qoot
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Runtime
   NAMELINK_SKIP )

