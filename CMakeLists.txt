cmake_minimum_required(VERSION 3.14)
project(icpc_book)

set(CMAKE_CXX_STANDARD 17)


### Quick Start Source: https://google.github.io/googletest/quickstart-cmake.html

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(generic_segment_tree
        tests/xor_segment_tree.cc
        tests/max_segment_tree.cc)

target_link_libraries(generic_segment_tree GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(generic_segment_tree)
###
