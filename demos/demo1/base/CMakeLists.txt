cmake_minimum_required(VERSION 3.0.0)

project(ThreadPoolTest)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

aux_source_directory(. BASE_SRC_LIST)
aux_source_directory(./test TEST_SRC_LIST)

add_executable(ThreadPoolTest ${BASE_SRC_LIST} ${TEST_SRC_LIST})

target_include_directories(ThreadPoolTest PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(ThreadPoolTest PUBLIC gtest_main gtest pthread)

