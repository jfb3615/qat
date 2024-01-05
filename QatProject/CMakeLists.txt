cmake_minimum_required(VERSION 3.5.0)

project(QatProject VERSION ${Qat_VERSION} LANGUAGES CXX)
include (GNUInstallDirs)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file( GLOB SOURCES *.cpp )
file( GLOB FORMS   *.ui *.qrc)
file( GLOB HEADERS *.h *.icc )
file( GLOB SCRIPTS testprogram qat-function)
file( GLOB TEMPLATES TEMPLATES/*)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt6 COMPONENTS Widgets PrintSupport Svg REQUIRED)
find_package(Eigen3 REQUIRED)



add_executable( qat-project  ${HEADERS} ${SOURCES} ${FORMS} )
target_link_libraries( qat-project PUBLIC Qt6::Widgets )
target_include_directories( qat-project PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )



install( TARGETS qat-project
   EXPORT ${PROJECT_NAME}-export
   RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
   COMPONENT Runtime )



install( FILES ${SCRIPTS}
   DESTINATION ${CMAKE_INSTALL_BINDIR}
   COMPONENT Runtime )

install( FILES ${TEMPLATES}
   DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/qat/templates
   COMPONENT Runtime )



