cmake_minimum_required(VERSION 3.28)

project(QatGenericFunctions VERSION ${Qat_VERSION} LANGUAGES CXX)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)

file( GLOB SOURCES src/*.cpp )
file( GLOB HEADERS QatGenericFunctions/*.h QatGenericFunctions/*.icc )

find_package(Eigen3 REQUIRED)

include(CMakePackageConfigHelpers)
set(QAT_FIND_DEPENDENCIES "find_dependency(Eigen3)")

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


configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/../cmake/QatPackageConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(EXPORT ${PROJECT_NAME}-export
    FILE "${PROJECT_NAME}Targets.cmake"
    NAMESPACE "Qat::"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)

configure_file (pkgconfig/QatGenericFunctions.pc.in pkgconfig/QatGenericFunctions.pc ) 

install (FILES ${CMAKE_BINARY_DIR}/QatGenericFunctions/pkgconfig/QatGenericFunctions.pc
   DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
   COMPONENT Development )




