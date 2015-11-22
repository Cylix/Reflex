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
# variables
###
set(DEPS_FOLDER ${PROJECT_SOURCE_DIR}/deps)
set(GTEST_INCLUDES ${DEPS_FOLDER}/gtest/include)
set(GTEST_LIBS ${DEPS_FOLDER}/gtest/lib)
set(REFLEX_INCLUDES ${PROJECT_SOURCE_DIR}/includes)


###
# installation directives
###
install (DIRECTORY ${REFLEX_INCLUDES}/ DESTINATION include)


###
# tests
###
IF (BUILD_TESTING)
    add_subdirectory(tests)
ENDIF(BUILD_TESTING)


###
# examples
###
IF (BUILD_EXAMPLES)
    add_subdirectory(examples)
ENDIF(BUILD_EXAMPLES)
