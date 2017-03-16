# global properties
GET_PROPERTY(LOCAL_COMPILE_FLAGS GLOBAL PROPERTY APP_COMPILE_FLAGS_GLOBAL)
GET_PROPERTY(LOCAL_LINK_FLAGS GLOBAL PROPERTY APP_LINK_FLAGS_GLOBAL)

# functions
function(create_exe exeName)
    add_executable(${exeName} ${APP_SourceDir}/tools/${exeName}.cpp)

    set_target_properties(${exeName} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    )

    target_link_libraries(${exeName}
        ${CMAKE_THREAD_LIBS_INIT}
        ${CMAKE_DL_LIBS}
        minorseq
        ${ZLIB_LIBRARIES}
        ${HTSLIB_LIBRARIES}
        ${PacBioBAM_LIBRARIES}
        ${pbcopper_LIBRARIES}
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
    )

    install(TARGETS ${exeName} RUNTIME DESTINATION bin)
    set_target_properties(${exeName} PROPERTIES COMPILE_FLAGS ${LOCAL_COMPILE_FLAGS})
    if (LOCAL_LINK_FLAGS)
        set_target_properties(${exeName} PROPERTIES LINK_FLAGS ${LOCAL_LINK_FLAGS})
    endif()
endfunction()

# get all header files for IDE support
file(GLOB_RECURSE APP_HEADER "${APP_IncludeDir}/*.h")
file(GLOB_RECURSE APP_HIDDEN_HEADER "*.h")
# get sources for src/
file(GLOB APP_CPP        "*.cpp")

include_directories(
    ${APP_IncludeDir}
    ${CMAKE_BINARY_DIR}/generated
    ${Boost_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS}
    ${HTSLIB_INCLUDE_DIRS}
    ${PacBioBAM_INCLUDE_DIRS}
    ${pbcopper_INCLUDE_DIRS}
)

# set_target_properties(apparatus PROPERTIES COMPILE_FLAGS ${LOCAL_COMPILE_FLAGS})
# if (LOCAL_LINK_FLAGS)
#     set_target_properties(apparatus PROPERTIES LINK_FLAGS ${LOCAL_LINK_FLAGS})
# endif()

create_exe(extract_subreads)
create_exe(mafft_pileup)
create_exe(juliet_pileup)
