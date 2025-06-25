cmake_minimum_required(VERSION 3.5.0)

project(QatInventorWidgets VERSION ${Qat_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file( GLOB SOURCES src/*.cpp )
file( GLOB FORMS   src/*.ui )
file( GLOB HEADERS QatInventorWidgets/*.h QatInventorWidgets/*.icc )



set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt${QT_VERSION} COMPONENTS Widgets PrintSupport Svg REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(Coin REQUIRED)
find_package(SoQt REQUIRED)


add_library( QatInventorWidgets SHARED ${HEADERS} ${SOURCES} ${FORMS} )
target_link_libraries( QatInventorWidgets PUBLIC Eigen3::Eigen QatPlotWidgets QatPlotting QatDataAnalysis QatGenericFunctions Qt${QT_VERSION}::Widgets Qt${QT_VERSION}::PrintSupport Qt::Svg Coin::Coin SoQt::SoQt)
target_include_directories( QatInventorWidgets PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )

set_target_properties( QatInventorWidgets PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )


install(TARGETS QatInventorWidgets
    EXPORT ${PROJECT_NAME}-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT          Runtime
            NAMELINK_COMPONENT Development   # Requires CMake 3.12
)

install( FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/QatInventorWidgets
   COMPONENT Development )




configure_file (pkgconfig/QatInventorWidgets.pc.in pkgconfig/QatInventorWidgets.pc ) 

install (FILES  ${CMAKE_BINARY_DIR}/QatInventorWidgets/pkgconfig/QatInventorWidgets.pc
   DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
   COMPONENT Development )

