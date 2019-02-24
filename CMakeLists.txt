# CMakeList.txt : CMake project for GOLAnimator, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add OpenCV
find_package(OpenCV REQUIRED)

# Add source to this project's executable.
add_executable (GOLAnimator "GOLAnimator.cpp" "GOLAnimator.h")
target_include_directories(GOLAnimator PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(GOLAnimator ${OpenCV_LIBRARIES})


# TODO: Add tests and install targets if needed.
