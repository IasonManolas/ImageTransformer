cmake_minimum_required(VERSION 3.14)
project(color_curve_lib)

file(GLOB LIB_SOURCES src/color_curve_lib.cpp)
file(GLOB LIB_HEADERS include/color_curve_lib.hpp)
add_library(${PROJECT_NAME} ${LIB_SOURCES} ${LIB_HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC include/)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

include(FetchContent)
find_package(OpenCV REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC ${OpenCV_LIBS})
target_include_directories(${PROJECT_NAME} PUBLIC ${OpenCV_INCLUDE_DIRS})

find_package(TBB REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE TBB::tbb)


include(CTest)
if(BUILD_TESTING)
        add_subdirectory(tests)
endif()
