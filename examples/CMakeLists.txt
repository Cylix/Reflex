###
# compilation options
###
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")


###
# includes
###
include_directories(${PROJECT_SOURCE_DIR}/includes
                    ${REFLEX_INCLUDES})


###
# reflection without instance
###
add_executable(reflection_without_instance reflection_without_instance.cpp)
target_link_libraries(reflection_without_instance reflex)
set_target_properties(reflection_without_instance
                      PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/target")


###
# reflection with instance
###
add_executable(reflection_with_instance reflection_with_instance.cpp)
target_link_libraries(reflection_with_instance reflex)
set_target_properties(reflection_with_instance
                    PROPERTIES
                    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/target")


###
# reflection on non member function
###
add_executable(reflection_non_member_function reflection_non_member_function.cpp)
target_link_libraries(reflection_non_member_function reflex)
set_target_properties(reflection_non_member_function
                      PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/target")
