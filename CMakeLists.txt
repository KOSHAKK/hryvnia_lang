cmake_minimum_required(VERSION 3.31)
project(hryvnia_lang)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

add_subdirectory(lib)

file(GLOB_RECURSE CPP_SRCS src/*.cpp)
file(GLOB_RECURSE HPP_SRCS src/*.hpp)

set(SRCS ${CPP_SRCS} ${HPP_SRCS})

add_executable(${PROJECT_NAME} ${SRCS})
target_include_directories(${PROJECT_NAME} PUBLIC lib/src/hryvnia_lang)
target_link_libraries(${PROJECT_NAME} PUBLIC hryvnia_lang_lib)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
