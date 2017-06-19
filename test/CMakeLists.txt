PROJECT(cpp_test)

# SET up files
SET (${PROJECT_NAME}_headers  ../src/unit_test.h
                              ../src/catch_test.h
                              ../3rdparty/catch/single_include/catch.hpp)
SET (${PROJECT_NAME}_sources  program.cpp
                              logger_tests.cpp
                              util_methods_tests.cpp
                              assert_tests.cpp
                              unit_test_base_tests.cpp
                              test_suite_base_tests.cpp)

# create binaries
# ---------------
ADD_EXECUTABLE (${PROJECT_NAME} ${${PROJECT_NAME}_headers} ${${PROJECT_NAME}_sources})

# Creates folder tests and adds target project
SET_PROPERTY(TARGET ${PROJECT_NAME} PROPERTY FOLDER tests)

# include directories
# -------------------
INCLUDE_DIRECTORIES(../src)
INCLUDE_DIRECTORIES(../3rdparty/catch/single_include)
