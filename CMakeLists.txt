#############################################
# CMake build script for the APPARATUS library
#############################################

cmake_policy(SET CMP0048 NEW)
project(APPARATUS VERSION 1.0.0 LANGUAGES CXX C)
cmake_minimum_required(VERSION 3.2)

set(ROOT_PROJECT_NAME ${PROJECT_NAME} CACHE STRING "root project name")

# Build type
IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: Debug Release Profile RelWithDebInfo ReleaseWithAssert" FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

# Build-time options
option(APP_build_bin    "Build binaries." ON)
option(APP_build_tests  "Build APPARATUS's unit tests." ON)
option(APP_inc_coverage "Include APPARATUS's coverage script." OFF)
option(APP_use_ccache   "Build APPARATUS using ccache, if available." ON)

# Main project paths
set(APP_RootDir       ${APPARATUS_SOURCE_DIR})
set(APP_IncludeDir    ${APP_RootDir}/include)
set(APP_SourceDir     ${APP_RootDir}/src)
set(APP_SwigDir       ${APP_RootDir}/swig)
set(APP_TestsDir      ${APP_RootDir}/tests)
set(APP_ThirdPartyDir ${APP_RootDir}/third-party)

# Project configuration
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake ${CMAKE_MODULE_PATH})

# Fixed order, do not sort or shuffle
include(app-ccache)
include(app-releasewithassert)
include(app-dependencies)
include(app-compilerflags)
include(app-gitsha1)

# Build library
add_subdirectory(${APP_SourceDir})
