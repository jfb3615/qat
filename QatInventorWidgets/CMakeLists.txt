cmake_minimum_required(VERSION 3.28)

project(QatInventorWidgets VERSION ${Qat_VERSION} LANGUAGES CXX)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)

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

include(CMakePackageConfigHelpers)
set(QAT_FIND_DEPENDENCIES "find_dependency(Eigen3)\nfind_dependency(Qt${QT_VERSION} COMPONENTS Widgets PrintSupport Svg)\nfind_dependency(Coin)\nfind_dependency(SoQt)\nfind_dependency(QatDataAnalysis)\nfind_dependency(QatGenericFunctions)\nfind_dependency(QatPlotting)\nfind_dependency(QatPlotWidgets)")


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


configure_file (pkgconfig/QatInventorWidgets.pc.in pkgconfig/QatInventorWidgets.pc ) 

install (FILES  ${CMAKE_BINARY_DIR}/QatInventorWidgets/pkgconfig/QatInventorWidgets.pc
   DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
   COMPONENT Development )

