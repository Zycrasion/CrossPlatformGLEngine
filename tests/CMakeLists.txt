link_libraries(CrossPlatformGLEngine)

if (UNIX)
add_compile_options("-fpermissive")
endif (UNIX)

add_executable(triangle_test "triangle_test.cpp")