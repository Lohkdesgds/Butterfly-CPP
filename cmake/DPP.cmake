set(DPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(DPP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(D_BUILD_VOICE_SUPPORT OFF CACHE BOOL "" FORCE)

include(FetchContent)

if(NOT TARGET std::filesystem)
    add_library(std::filesystem INTERFACE IMPORTED)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
        set_property(TARGET std::filesystem APPEND PROPERTY INTERFACE_LINK_LIBRARIES "stdc++fs")
    endif()
endif()

FetchContent_Declare(
    dpp
    GIT_REPOSITORY https://github.com/brainboxdotcc/DPP.git
    GIT_TAG        v10.1.6
)

FetchContent_MakeAvailable(dpp)