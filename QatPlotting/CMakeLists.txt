cmake_minimum_required(VERSION 3.5.0)

project(QatPlotting VERSION ${Qat_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)
file( GLOB SOURCES src/*.cpp )
file( GLOB HEADERS QatPlotting/*.h QatPlotting/*.icc )



set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt${QT_VERSION} COMPONENTS Widgets REQUIRED)
find_package(Eigen3 REQUIRED)



add_library( QatPlotting SHARED ${HEADERS} ${SOURCES} )
target_link_libraries( QatPlotting PUBLIC Eigen3::Eigen QatDataAnalysis QatGenericFunctions Qt${QT_VERSION}::Widgets )
target_include_directories( QatPlotting PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )

set_target_properties( QatPlotting PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )


install(TARGETS QatPlotting
    EXPORT ${PROJECT_NAME}-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT          Runtime
            NAMELINK_COMPONENT Development   # Requires CMake 3.12
)

install( FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/QatPlotting
   COMPONENT Development )


configure_file (pkgconfig/QatPlotting.pc.in pkgconfig/QatPlotting.pc ) 

install (FILES  ${CMAKE_BINARY_DIR}/QatPlotting/pkgconfig/QatPlotting.pc
   DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
   COMPONENT Development )

