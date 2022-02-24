CMAKE_MINIMUM_REQUIRED(VERSION 3.5.0)

project(server)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_definitions(-g)

aux_source_directory(./ SERVER_SRC_LIST)

add_executable(server ${SERVER_SRC_LIST} ${THREADPOLL_SRC_LIST})

target_include_directories(server PUBLIC
    .
)

target_link_libraries(server PUBLIC
    pthread
)

