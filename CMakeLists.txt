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

# Create main executable
add_executable(main src/main.cpp src/Executive.cpp src/FileManager.cpp src/Map.cpp src/Reduce.cpp src/Sort.cpp src/Workflow.cpp)

# Create unit test executables
add_executable(FileManagerTest tests/FileManagerTest.cpp src/FileManager.cpp)
add_executable(ExecutiveTest tests/ExecutiveTest.cpp src/Executive.cpp src/FileManager.cpp)
add_executable(MapTest tests/MapTest.cpp src/FileManager.cpp src/Map.cpp)


#target_link_libraries(RunAllTests GTest::gtest) # Custom main function
target_link_libraries(FileManagerTest GTest::gtest_main) #Google's main function implementation
target_link_libraries(ExecutiveTest GTest::gtest_main) #Google's main function implementation
target_link_libraries(MapTest GTest::gtest_main) #Google's main function implementation

#gtest_discover_tests(unit_test)
