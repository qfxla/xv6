cmake_minimum_required(VERSION 3.23)
project(xv6)

set(CMAKE_CXX_STANDARD 11)
#file(GLOB SOURCE_FILES "*.h" "*.cpp" "*.c" "/user/*.c")
file(GLOB SRC_FILES
        "main.cpp"
        "${PROJECT_SOURCE_DIR}/user/*.c"
        "${PROJECT_SOURCE_DIR}/user/*.h"
        "${PROJECT_SOURCE_DIR}/kernel/*.c"
        "${PROJECT_SOURCE_DIR}/kernel/*.h"
        "${PROJECT_SOURCE_DIR}/test/*.c")

add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES})


#add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})
