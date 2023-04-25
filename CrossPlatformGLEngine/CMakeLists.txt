# CMakeList.txt : CMake project for CrossPlatformGLEngine, include source and define
# project specific logic here.
#

# Add source to this project's executable.
file(GLOB CrossPlatformGLEngine_SRC CMAKE_CONFIGURE_DEPENDS  "imgui/*.h" "imgui/*.cpp" "*.hpp" "*.cpp" )
add_library (CrossPlatformGLEngine ${CrossPlatformGLEngine_SRC})
target_link_libraries(CrossPlatformGLEngine glfw)
target_link_libraries(CrossPlatformGLEngine glad)
target_link_libraries(CrossPlatformGLEngine glm)


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CrossPlatformGLEngine PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
