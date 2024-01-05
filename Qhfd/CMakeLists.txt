cmake_minimum_required(VERSION 3.5.0)

project(Qhfd VERSION ${Qat_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file( GLOB SOURCES src/*.cpp )
file( GLOB HEADERS Qhfd/*.h Qhfd/*.icc )

find_package(Eigen3 REQUIRED)
find_package( HDF5 REQUIRED COMPONENTS CXX )

add_library( Qhfd SHARED ${HEADERS} ${SOURCES} )
target_link_libraries( Qhfd PUBLIC Eigen3::Eigen QatDataAnalysis QatGenericFunctions )
target_include_directories( Qhfd PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> PUBLIC ${HDF5_CXX_INCLUDE_DIRS})

set_target_properties( Qhfd PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )


install(TARGETS Qhfd
    EXPORT ${PROJECT_NAME}-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT          Runtime
            NAMELINK_COMPONENT Development   # Requires CMake 3.12
)

install( FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Qhfd
   COMPONENT Development )



