cmake_minimum_required(VERSION 3.10.0)

project(QHDF5 VERSION ${Qat_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file( GLOB SOURCES src/*.cpp )
file( GLOB HEADERS Qhfd/*.h Qhfd/*.icc )

find_package(Eigen3 REQUIRED)
find_package( HDF5 REQUIRED COMPONENTS CXX )

add_library( QHDF5 SHARED ${HEADERS} ${SOURCES} )
target_link_libraries( QHDF5 PUBLIC Eigen3::Eigen QatDataAnalysis QatGenericFunctions hdf5::hdf5_cpp HDF5::HDF5 )
target_include_directories( QHDF5 PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> PUBLIC ${HDF5_CXX_INCLUDE_DIRS})

set_target_properties( QHDF5 PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )


install(TARGETS QHDF5
    EXPORT ${PROJECT_NAME}-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT          Runtime
            NAMELINK_COMPONENT Development   # Requires CMake 3.12
)



