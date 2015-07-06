###
# config
###
cmake_minimum_required(VERSION 2.8.1)
cmake_policy(SET CMP0042 NEW)

###
# verbose make
###
# set(CMAKE_VERBOSE_MAKEFILE TRUE)


###
# project
###
set(PROJECT cpp_reflection)
project(${PROJECT} CXX)


###
# compilation options
###
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")


###
# variables
###
set(DEPS_FOLDER ${PROJECT_SOURCE_DIR}/deps)
set(GTEST_INCLUDES ${DEPS_FOLDER}/gtest/include)
set(GTEST_LIBS ${DEPS_FOLDER}/gtest/lib)
set(CPP_REFLECTION_INCLUDES ${PROJECT_SOURCE_DIR}/includes)


###
# includes
###
include_directories(${CPP_REFLECTION_INCLUDES})


###
# link
###
link_directories(${GTEST_LIBS})


###
# sources
###
set(DIRS "sources")
foreach(dir ${DIRS})
  # get directory sources
  file(GLOB s_${dir} "${dir}/*.cpp")
  # set sources
  set(SOURCES ${SOURCES} ${s_${dir}})
endforeach()


###
# executable
###
add_library(${PROJECT} SHARED ${SOURCES})
set_target_properties(${PROJECT}
                      PROPERTIES
                      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/target")

add_executable(${PROJECT}_example example/main.cpp)
target_link_libraries(${PROJECT}_example ${PROJECT})
set_target_properties(${PROJECT}_example
                      PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/target")


###
# tests
###
IF (BUILD_TESTING)
    add_subdirectory(tests)
ENDIF(BUILD_TESTING)
