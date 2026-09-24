cmake_minimum_required(VERSION 3.28)

project(ButterflyBot 
    DESCRIPTION "The successor of Bread Bot!"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


file(GLOB_RECURSE PROJECT_SOURCES
     CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

include(cmake/FindDPP.cmake)
find_package(DPP REQUIRED)


add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE dpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

#if(MSVC)
#    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
#else()
#    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -O2)
#endif()