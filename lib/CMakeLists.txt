project(hryvnia_lang_lib)


file(GLOB_RECURSE CPP_SRCS src/*.cpp)
file(GLOB_RECURSE HPP_SRCS src/*.hpp)

set(SRCS ${CPP_SRCS} ${HPP_SRCS})

add_library(hryvnia_lang_lib STATIC ${SRCS})
target_include_directories(hryvnia_lang_lib PUBLIC src)

file(GLOB_RECURSE TEST_CPP_SRCS test/*.cpp)
file(GLOB_RECURSE TEST_HPP_SRCS test/*.hpp)

set(TEST_SRCS ${TEST_CPP_SRCS} ${TEST_HPP_SRCS})

add_executable(hryvnia_lang_tests ${TEST_SRCS})
target_include_directories(hryvnia_lang_tests PRIVATE
    src/*.hpp
)
target_link_libraries(hryvnia_lang_tests gtest gtest_main hryvnia_lang_lib)

# llvm
find_package(LLVM REQUIRED CONFIG)

llvm_map_components_to_libnames(LLVM_LIBS core support)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "LLVM include dirs: ${LLVM_INCLUDE_DIRS}")
message(STATUS "LLVM libraries: ${LLVM_LIBRARIES}")

target_include_directories(hryvnia_lang_lib PUBLIC ${LLVM_INCLUDE_DIRS})
target_link_libraries(hryvnia_lang_lib PUBLIC ${LLVM_LIBS})
