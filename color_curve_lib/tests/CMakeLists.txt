cmake_minimum_required(VERSION 3.14)
set(PROJECT_NAME ${PROJECT_NAME}_tests)
project(${PROJECT_NAME}
	VERSION 1.0
	LANGUAGES CXX)

Include(FetchContent)

FetchContent_Declare(
	Catch2
	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.0.1
	)

FetchContent_MakeAvailable(Catch2)

add_executable(${PROJECT_NAME} tests.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE color_curve_lib)
cmake_path(GET CMAKE_CURRENT_LIST_DIR PARENT_PATH PARENT_DIR)
target_include_directories(${PROJECT_NAME} PRIVATE ${PARENT_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2WithMain)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
