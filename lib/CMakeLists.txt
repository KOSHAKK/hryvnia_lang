set(PROJECT_LIB_NAME hryvnia_lang_lib)

project(${PROJECT_LIB_NAME})

file(GLOB_RECURSE CPP_SRCS src/*.cpp)
file(GLOB_RECURSE HPP_SRCS src/*.hpp)

set(SRCS ${CPP_SRCS} ${HPP_SRCS})

add_library(${PROJECT_LIB_NAME} STATIC ${SRCS})
target_include_directories(${PROJECT_LIB_NAME} PUBLIC src)

file(GLOB_RECURSE TEST_CPP_SRCS test/*.cpp)
file(GLOB_RECURSE TEST_HPP_SRCS test/*.hpp)

set(TEST_SRCS ${TEST_CPP_SRCS} ${TEST_HPP_SRCS})

add_executable(hryvnia_lang_tests ${TEST_SRCS})
target_include_directories(hryvnia_lang_tests PRIVATE
    src/*.hpp
)

#gtest
target_link_libraries(hryvnia_lang_tests gtest gtest_main ${PROJECT_LIB_NAME})



#spdlog
add_subdirectory(spdlog)
target_link_libraries(hryvnia_lang_lib PUBLIC spdlog::spdlog)



# llvm
find_package(LLVM REQUIRED CONFIG)

llvm_map_components_to_libnames(LLVM_LIBS
    core
    support
    native
    orcjit
)


message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "LLVM dir: ${LLVM_DIR}")

target_include_directories(${PROJECT_LIB_NAME} PUBLIC ${LLVM_INCLUDE_DIRS})
target_link_libraries(${PROJECT_LIB_NAME} PUBLIC ${LLVM_LIBS})

