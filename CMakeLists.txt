cmake_minimum_required(VERSION 3.10)
project(Annotater)

find_package(OpenCV REQUIRED)

include_directories(/usr/local/include/opencv4)

add_executable(Annotater main.cpp)
target_compile_options(Annotater PUBLIC -Wall -Wextra -pedantic -Wshadow -Wreturn-type -fsanitize=undefined)
target_compile_features(Annotater PUBLIC cxx_std_17)

target_include_directories(Annotater PUBLIC ${OPENCV_INCLUDE_DIRS})
target_link_libraries(Annotater PUBLIC ${OpenCV_LIBS})
target_link_libraries(Annotater PUBLIC -fsanitize=undefined)
