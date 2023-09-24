# CMakeList.txt : CMake project for GOLAnimator, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
project("GOLAnimator")

# Add OpenCV
find_package(OpenCV REQUIRED)

# Add source to this project's executable.
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_executable (GOLAnimator 
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/GOLAnimator.cpp
    ${SRC_DIR}/GOLAnimator.h)

target_include_directories(GOLAnimator PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(GOLAnimator ${OpenCV_LIBRARIES})


# TODO: Add tests and install targets if needed.
