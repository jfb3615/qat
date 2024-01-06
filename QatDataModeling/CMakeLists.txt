cmake_minimum_required(VERSION 3.5.0)

project(QatDataModeling VERSION ${Qat_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file( GLOB SOURCES src/*.cpp Minuit2/src/*.cxx)
file( GLOB HEADERS QatDataModeling/*.h QatDataModeling/*.icc )

find_package(Eigen3 REQUIRED)

add_library( QatDataModeling SHARED ${HEADERS} ${SOURCES} )
target_link_libraries( QatDataModeling PUBLIC Eigen3::Eigen QatDataAnalysis QatGenericFunctions )
target_include_directories( QatDataModeling PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> PRIVATE Minuit2)

set_target_properties( QatDataModeling PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )


install(TARGETS QatDataModeling
    EXPORT ${PROJECT_NAME}-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT          Runtime
            NAMELINK_COMPONENT Development   # Requires CMake 3.12
)

install( FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/QatDataModeling
   COMPONENT Development )


configure_file (pkgconfig/QatDataModeling.pc.in pkgconfig/QatDataModeling.pc ) 

install (FILES  ${CMAKE_BINARY_DIR}/QatDataModeling/pkgconfig/QatDataModeling.pc
   DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
   COMPONENT Development )


