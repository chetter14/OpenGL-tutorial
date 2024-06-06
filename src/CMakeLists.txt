cmake_minimum_required(VERSION 3.4...3.28 FATAL_ERROR)

add_executable(Test-example main.cpp triangle.cpp stb_image_impl.cpp "${OpenGL-tutorial_SOURCE_DIR}/glad/src/glad.c")

add_dependencies(Test-example glfw)

target_include_directories(Test-example PRIVATE "${GLFW_SOURCE_DIR}/include" "${OpenGL-tutorial_SOURCE_DIR}/glad/include" "${OpenGL-tutorial_SOURCE_DIR}")

target_link_libraries(Test-example PRIVATE "${OpenGL-tutorial_SOURCE_DIR}/lib/OpenGL32.Lib")

if (MSVC AND CMAKE_GENERATOR MATCHES "Visual Studio")
	target_link_libraries(Test-example PRIVATE "$<$<CONFIG:DEBUG>:${GLFW_BINARY_DIR}/src/Debug/glfw3.lib>" "$<$<CONFIG:RELEASE>:${GLFW_BINARY_DIR}/src/Release/glfw3.lib>")
endif ()