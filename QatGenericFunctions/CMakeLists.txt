cmake_minimum_required(VERSION 3.5.0)

project(QatGenericFunctions VERSION ${Qat_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)

file( GLOB SOURCES src/*.cpp )
file( GLOB HEADERS QatGenericFunctions/*.h QatGenericFunctions/*.icc )

find_package(Eigen3 REQUIRED)

add_library( QatGenericFunctions SHARED ${HEADERS} ${SOURCES} )
target_link_libraries( QatGenericFunctions PUBLIC Eigen3::Eigen )
target_include_directories( QatGenericFunctions PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )

set_target_properties( QatGenericFunctions PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )


install(TARGETS QatGenericFunctions
    EXPORT ${PROJECT_NAME}-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT          Runtime
            NAMELINK_COMPONENT Development   # Requires CMake 3.12
)

install( FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/QatGenericFunctions
   COMPONENT Development )

configure_file (pkgconfig/QatGenericFunctions.pc.in pkgconfig/QatGenericFunctions.pc ) 

install (FILES ${CMAKE_BINARY_DIR}/QatGenericFunctions/pkgconfig/QatGenericFunctions.pc
   DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
   COMPONENT Development )




