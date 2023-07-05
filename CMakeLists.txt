cmake_minimum_required(VERSION 3.16)

set(APP_NAME mosquittoClient)
set(CMAKE_BUILD_TYPE Debug)
project(${APP_NAME} C CXX)

#####################################################
# Setting the dependency directories path
#####################################################
execute_process(COMMAND git rev-parse --show-toplevel
  OUTPUT_STRIP_TRAILING_WHITESPACE
  WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
  OUTPUT_VARIABLE GIT_ROOT)
message(STATUS "GIT_ROOT = ${GIT_ROOT}")

set(THIRD_PARTY_DIR "${GIT_ROOT}/3rdParty")
include("${THIRD_PARTY_DIR}/cmake/third_party_paths.cmake")
list(APPEND CMAKE_MODULE_PATH "${THIRD_PARTY_DIR}/cmake")



#####################################################
# Sources and headers
#####################################################
# Relative path to the project sources.
set (APP_SRC ".")  
# Note: The new child scope inherits all variable definitions from its parent scope.set(APP_SRC ".")
#       i.e. ${APP_SRC} is visible for all dependent child projects
message (STATUS "${APP_NAME}: Looking for sources in ${CMAKE_CURRENT_SOURCE_DIR}/${APP_SRC}")

#list the target source files
#file(GLOB TARGET_H "${CMAKE_SOURCE_DIR}/*.hpp" "${XML_SRC}/*.hpp")
#file(GLOB TARGET_CPP "${CMAKE_SOURCE_DIR}/*.cpp" "${XML_SRC}/*.cpp")
#set(APP_SRC ${TARGET_CPP} ${TARGET_H})
set(APP_SRC ${APP_SRC} 
    main.cpp
)

# build and link target executable
add_executable(${APP_NAME} ${APP_SRC})


find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(mosquitto REQUIRED)
target_link_libraries(${APP_NAME} mosquittopp)
target_link_libraries(${APP_NAME} mosquitto)
target_link_libraries(${APP_NAME} Threads::Threads)
target_link_libraries(${APP_NAME} OpenSSL::SSL OpenSSL::Crypto)