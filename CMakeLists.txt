cmake_minimum_required(VERSION 3.14)
project(color_curve
	VERSION 1.0
	LANGUAGES CXX)

add_subdirectory(color_curve_lib)
add_executable(${PROJECT_NAME} "app/main.cpp")
target_link_libraries(${PROJECT_NAME} PUBLIC color_curve_lib)

