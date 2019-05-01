# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

cmake_minimum_required(VERSION 3.5)

include (cmake/HunterGate.cmake)
HunterGate(
        URL "https://github.com/ruslo/hunter/archive/v0.23.154.tar.gz"
        SHA1 "d720f3775d4b896dab91f8249de04956e31804dd"
        LOCAL
)

project(BoostOutcome LANGUAGES CXX VERSION 2.0.0.0)

hunter_add_package(Boost COMPONENTS exception system)
find_package(Boost REQUIRED COMPONENTS exception system)

add_library(outcome INTERFACE)
add_library(Boost::outcome ALIAS outcome)

target_include_directories(
        outcome
        INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_link_libraries(outcome INTERFACE
  Boost::boost
  Boost::exception
  Boost::system
)

#
# installation
#
include(GNUInstallDirs)

set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")

set(config_install_dir "lib/cmake/${PROJECT_NAME}")
set(include_install_dir "include")
set(pkgconfig_install_dir "lib/pkgconfig")

set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(pkg_config "${generated_dir}/${PROJECT_NAME}.pc")
set(targets_export_name "${PROJECT_NAME}Targets")
set(namespace "Boost::")

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${version_config}" COMPATIBILITY SameMajorVersion
)

configure_file("cmake/boost_outcome.pc.in" "${pkg_config}" @ONLY)

configure_package_config_file(
        "cmake/Config.cmake.in"
        "${project_config}"
        INSTALL_DESTINATION "${config_install_dir}"
)

INSTALL(TARGETS outcome
        EXPORT "${targets_export_name}"
        INCLUDES DESTINATION "${include_install_dir}")

INSTALL(DIRECTORY include/boost DESTINATION include)

install(
        FILES "${project_config}" "${version_config}"
        DESTINATION "${config_install_dir}"
)

install(
        FILES "${pkg_config}"
        DESTINATION "${pkgconfig_install_dir}"
)

install(
        EXPORT "${targets_export_name}"
        NAMESPACE "${namespace}"
        DESTINATION "${config_install_dir}"
)