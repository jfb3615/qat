cmake_minimum_required(VERSION 3.5.0)

project(QatPlotWidgets VERSION ${Qat_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file( GLOB SOURCES src/*.cpp src/*.qrc )
file( GLOB FORMS   src/*.ui )
file( GLOB HEADERS QatPlotWidgets/*.h QatPlotWidgets/*.icc )

qt_add_resources (SOURCES *.qrc)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt${QT_VERSION} COMPONENTS Widgets PrintSupport Svg REQUIRED)
find_package(Eigen3 REQUIRED)



add_library( QatPlotWidgets SHARED ${HEADERS} ${SOURCES} ${FORMS} )
target_link_libraries( QatPlotWidgets PUBLIC Eigen3::Eigen QatDataAnalysis QatGenericFunctions QatPlotting Qt${QT_VERSION}::Widgets Qt${QT_VERSION}::PrintSupport Qt::Svg)
target_include_directories( QatPlotWidgets PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )

set_target_properties( QatPlotWidgets PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )


install(TARGETS QatPlotWidgets
    EXPORT ${PROJECT_NAME}-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT          Runtime
            NAMELINK_COMPONENT Development   # Requires CMake 3.12
)

install( FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/QatPlotWidgets
   COMPONENT Development )



configure_file (pkgconfig/QatPlotWidgets.pc.in pkgconfig/QatPlotWidgets.pc ) 


install (FILES  ${CMAKE_BINARY_DIR}/QatPlotWidgets/pkgconfig/QatPlotWidgets.pc
   DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
   COMPONENT Development )

