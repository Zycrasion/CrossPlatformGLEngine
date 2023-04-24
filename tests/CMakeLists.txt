link_libraries(CrossPlatformGLEngine)

if (UNIX)
add_compile_options("-fpermissive")
endif (UNIX)

include_directories(${CMAKE_ROOT}/src)


add_executable(imgui_test "imgui_test.cpp")
add_executable(triangle_test "triangle_test.cpp")
add_executable(triangle_test_ecs "triangle_test_ecs.cpp")
add_executable(multi_window_test "multi_window_test.cpp")
add_executable(triangle_texture_test "triangle_texture_test.cpp")
add_executable(massive_gpu_instancing_test "massive_gpu_instancing_test.cpp")

execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/tests/res
                                                           ${CMAKE_BINARY_DIR}/tests/res)