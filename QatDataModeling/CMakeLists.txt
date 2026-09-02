cmake_minimum_required(VERSION 3.28)

project(QatDataModeling VERSION ${Qat_VERSION} LANGUAGES CXX)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)

file( GLOB SOURCES src/*.cpp Minuit2/src/*.cxx)
file( GLOB HEADERS QatDataModeling/*.h QatDataModeling/*.icc )

find_package(Eigen3 REQUIRED)

include(CMakePackageConfigHelpers)
set(QAT_FIND_DEPENDENCIES "find_dependency(Eigen3)\nfind_dependency(QatDataAnalysis)\nfind_dependency(QatGenericFunctions)")

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


configure_file (pkgconfig/QatDataModeling.pc.in pkgconfig/QatDataModeling.pc ) 

install (FILES  ${CMAKE_BINARY_DIR}/QatDataModeling/pkgconfig/QatDataModeling.pc
   DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
   COMPONENT Development )


