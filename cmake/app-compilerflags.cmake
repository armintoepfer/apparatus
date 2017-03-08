
include(CheckCXXCompilerFlag)

# shared CXX flags for all source code & tests
set(APP_FLAGS "-std=c++1y -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable")

# static linking
IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(APP_LINK_FLAGS "${APP_LINK_FLAGS} -static-libgcc -static-libstdc++")
ENDIF()

# NOTE: quash clang warnings w/ Boost
check_cxx_compiler_flag("-Wno-unused-local-typedefs" HAS_NO_UNUSED_LOCAL_TYPEDEFS)
if(HAS_NO_UNUSED_LOCAL_TYPEDEFS)
    set(APP_FLAGS "${APP_FLAGS} -Wno-unused-local-typedefs")
endif()

# Cannot use this until pappam complies
# if (CMAKE_COMPILER_IS_GNUCXX)
#     set(APP_FLAGS "${APP_FLAGS} -Werror=suggest-override")
# endif()

# Coverage settings
if (APP_inc_coverage)
    set(APP_FLAGS "${APP_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

# Extra testing that will lead to longer compilation times!
if (SANITIZE)
    # AddressSanitizer is a fast memory error detector
    set(APP_FLAGS "${APP_FLAGS} -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls")

    # Clang Thread Safety Analysis is a C++ language extension which warns about
    # potential race conditions in code.
    set(APP_FLAGS "${APP_FLAGS} -Wthread-safety")

    # ThreadSanitizer is a tool that detects data races
    set(APP_FLAGS "${APP_FLAGS} -fsanitize=thread")

    # MemorySanitizer is a detector of uninitialized reads.
    set(APP_FLAGS "${APP_FLAGS} -fsanitize=memory")

    # UndefinedBehaviorSanitizer is a fast undefined behavior detector.
    set(APP_FLAGS "${APP_FLAGS} -fsanitize=undefined")
endif()

if (JULIET_INHOUSE_PERFORMANCE)
    set(APP_FLAGS "${APP_FLAGS} -DJULIET_INHOUSE_PERFORMANCE")
endif()

# shared CXX flags for src & tests
SET_PROPERTY(GLOBAL PROPERTY APP_COMPILE_FLAGS_GLOBAL ${APP_FLAGS})
SET_PROPERTY(GLOBAL PROPERTY APP_LINK_FLAGS_GLOBAL ${APP_LINK_FLAGS})