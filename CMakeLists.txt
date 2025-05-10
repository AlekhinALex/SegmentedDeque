cmake_minimum_required(VERSION 3.10)
project(lab3)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "inc/*.hpp")
file(GLOB IMPLEMENTATIONS "impl/*.tpp")
file(GLOB TEST_HEADERS "tests/*.hpp")
file(GLOB TEST_SOURCE "tests/*.cpp")


find_package(GTest REQUIRED)
include(GoogleTest)

include_directories(${GTEST_INCLUDE_DIRS} inc impl)


add_executable(tests
    ${TEST_SOURCE}
    ${HEADERS}
    ${IMPLEMENTATIONS}
)

target_link_libraries(tests GTest::GTest GTest::Main pthread)
