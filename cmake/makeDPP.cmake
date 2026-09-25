set(DPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(DPP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(D_BUILD_VOICE_SUPPORT OFF CACHE BOOL "" FORCE)

include(FetchContent)

FetchContent_Declare(
    dpp
    GIT_REPOSITORY https://github.com/brainboxdotcc/DPP.git
    GIT_TAG        v10.1.6
)

FetchContent_MakeAvailable(dpp)