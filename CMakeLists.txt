CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

PROJECT (cpp_test)
SET(VERSION_MAJOR                "1")
SET(VERSION_MINOR                "0")
SET(VERSION_PATCH                "0")
SET(VERSION                      "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
SET(CMAKE_CXX_STANDARD           14)
SET(CMAKE_CXX_STANDARD_REQUIRED  ON)

IF (UNIX)
   LIST(APPEND CMAKE_CXX_FLAGS "-std=c++14 ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")
ENDIF(UNIX)


# Turn on the ability to create folders to organize projects (.vcproj)
# It creates &quot;CMakePredefinedTargets&quot; folder by default and adds CMake
# defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

ADD_SUBDIRECTORY(test)

# Add tests
ENABLE_TESTING()
ADD_TEST(NAME unit_test COMMAND cpp_test)
