###
# project
###
set(PROJECT cpp_reflection_tests)
project(${PROJECT} CXX)


###
# compilation options
###
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")


###
# includes
###
include_directories(${PROJECT_SOURCE_DIR}/includes
                    ${CPP_REFLECTION_INCLUDES}
                    ${GTEST_INCLUDES})


###
# sources
###
set(DIRS "sources" "sources/**")
foreach(dir ${DIRS})
  # get directory sources
  file(GLOB s_${dir} "${dir}/*.cpp")
  # set sources
  set(SOURCES ${SOURCES} ${s_${dir}})
endforeach()


###
# executable
###
add_executable(${PROJECT} ${SOURCES})
target_link_libraries(${PROJECT} cpp_reflection gtest pthread)
set_target_properties(${PROJECT}
                      PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/target")
