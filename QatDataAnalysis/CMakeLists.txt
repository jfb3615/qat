cmake_minimum_required(VERSION 3.5.0)

project(QatDataAnalysis VERSION ${Qat_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file( GLOB SOURCES src/*.cpp )
file( GLOB HEADERS QatDataAnalysis/*.h QatDataAnalysis/*.icc )

find_package(Eigen3 REQUIRED)

add_library( QatDataAnalysis SHARED ${HEADERS} ${SOURCES} )
target_link_libraries( QatDataAnalysis PUBLIC Eigen3::Eigen QatGenericFunctions )
target_include_directories( QatDataAnalysis PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )

set_target_properties( QatDataAnalysis PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )


install(TARGETS QatDataAnalysis
    EXPORT ${PROJECT_NAME}-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT          Runtime
            NAMELINK_COMPONENT Development   # Requires CMake 3.12
)

install( FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/QatDataAnalysis
   COMPONENT Development )


configure_file (pkgconfig/QatDataAnalysis.pc.in pkgconfig/QatDataAnalysis.pc ) 


install (FILES  ${CMAKE_BINARY_DIR}/QatDataAnalysis/pkgconfig/QatDataAnalysis.pc
   DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
   COMPONENT Development )

