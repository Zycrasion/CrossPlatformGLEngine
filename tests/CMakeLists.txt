link_libraries(CrossPlatformGLEngine)

if (UNIX)
add_compile_options("-fpermissive")
endif (UNIX)

add_executable(triangle_test "triangle_test.cpp")
add_executable(triangle_test_ecs "triangle_test_ecs.cpp")
add_executable(multi_window_test "multi_window_test.cpp")
add_executable(massive_gpu_instancing_test "massive_gpu_instancing_test.cpp")
