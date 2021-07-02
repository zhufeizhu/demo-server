CMAKE_MINIMUM_REQUIRED(VERSION 3.5.0)

project(Server)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

aux_source_directory(./ SERVER_SRC_LIST)

aux_source_directory(../base/ THREADPOLL_SRC_LIST)

add_executable(Server ${SERVER_SRC_LIST} ${THREADPOLL_SRC_LIST})

target_include_directories(Server PUBLIC
    ../
)

target_link_libraries(Server PUBLIC
    pthread
)

