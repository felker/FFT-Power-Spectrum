cmake_minimum_required(VERSION 3.0)

project(FFT-Spectrum)
set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3 -g -march=native -fopenmp -std=c++11 -lm")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3 -g -march=native -std=c++11 -lm")
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}/cmake_modules/")

find_package(SFML 2.5 REQUIRED audio graphics window system)

set(LIBRARY sfml-audio sfml-graphics sfml-window sfml-system)
add_subdirectory(src)
target_include_directories(${PROJECT_NAME} PUBLIC include)
add_subdirectory(bin)
add_subdirectory(tests)

add_custom_target(copy-runtime-files ALL COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data ${CMAKE_BINARY_DIR}/data)


include(CMakePrintHelpers)
cmake_print_variables(SFML_FOUND SFML_LIBRARIES SFML_INCLUDE_DIRS)
