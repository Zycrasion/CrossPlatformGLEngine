# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("CrossPlatformGLEngine")


include_directories(include)
include_directories(glfw-3.3.8/include)
include_directories(CrossPlatformGLEngine)
# Include sub-projects.
add_subdirectory ("lib")
add_subdirectory ("CrossPlatformGLEngine")
add_subdirectory ("tests")