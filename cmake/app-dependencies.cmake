
# External libraries
# Get static libraries
SET(CMAKE_FIND_LIBRARY_SUFFIXES .a ${CMAKE_FIND_LIBRARY_SUFFIXES})

# Boost
find_package(Boost COMPONENTS system filesystem REQUIRED)

SET(MS_build_bin OFF CACHE STRING "" FORCE)
SET(MS_build_tests OFF CACHE STRING "" FORCE)
add_subdirectory(${APP_ThirdPartyDir}/minorseq external/minorseq/build)

# Threads
if (NOT Threads)
    find_package(Threads REQUIRED)
endif()