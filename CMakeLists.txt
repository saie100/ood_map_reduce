cmake_minimum_required(VERSION 3.14)
project(ood_map_reduce)

# GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.1
)

FetchContent_MakeAvailable(googletest)
enable_testing()

include(GoogleTest)
include_directories(src/headers/)
# Create unit test executable
add_executable(unit_test tests/unit_test.cpp src/Executive.cpp src/FileManager.cpp)
# Create main executable
add_executable(main main.cpp src/Executive.cpp src/FileManager.cpp)


#target_link_libraries(unit_test GTest::gtest) # Custom main function
target_link_libraries(unit_test GTest::gtest_main) #Google's main function implementation
gtest_discover_tests(unit_test)
