#  Find mosquitto includes and libraries
#  If found, these are variables and targets are defined
#
#  mosquitto_INCLUDE_DIRS - where to find mosquitto.h, etc.
#  mosquitto_LIBRARIES    - List of libraries when using libmosquitto.
#  mosquitto_FOUND        - True if libmosquitto found.
#  mosquitto              - imported target for the lib
#
#  See follwoing for doc: 
#     https://subscription.packtpub.com/book/programming/9781801070058/9/ch09lvl1sec57/writing-your-own-find-modules
#     https://gitlab.kitware.com/cmake/community/-/wikis/doc/tutorials/How-To-Find-Libraries#writing-find-modules

if (NOT mosquitto_INCLUDE_DIR)
  find_path(mosquitto_INCLUDE_DIR mosquitto.h)
endif()

if (NOT mosquitto_LIBRARY)
  find_library(
    mosquitto_LIBRARY
    NAMES mosquitto)
endif()

if (NOT mosquittopp_INCLUDE_DIR)
  find_path(mosquittopp_INCLUDE_DIR mosquittopp.h)
endif()

if (NOT mosquittopp_LIBRARY)
  find_library(
    mosquittopp_LIBRARY
    NAMES mosquittopp)
endif()

# Conventionally, the target cmake should find/link openssl instead of here.
if(NOT OPENSSL_FOUND)
  find_package(OpenSSL)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  mosquitto DEFAULT_MSG
  mosquitto_LIBRARY mosquitto_INCLUDE_DIR)

set(mosquitto_INCLUDE_DIRS ${mosquitto_INCLUDE_DIR} ${mosquittopp_INCLUDE_DIR} )
set(mosquitto_LIBRARIES ${mosquitto_LIBRARY} ${mosquittopp_LIBRARY})
message(STATUS "mosquitto include dirs: ${mosquitto_INCLUDE_DIRS}")
message(STATUS "mosquitto libraries:    ${mosquitto_LIBRARIES}")

# create IMPORTED targets, so libraries can link against this
add_library(mosquitto UNKNOWN IMPORTED)
set_target_properties(mosquitto PROPERTIES
  IMPORTED_LOCATION ${mosquitto_LIBRARY}
  INTERFACE_INCLUDE_DIRECTORIES ${mosquitto_INCLUDE_DIR}
)

add_library(mosquittopp UNKNOWN IMPORTED)
set_target_properties(mosquittopp PROPERTIES
  IMPORTED_LOCATION ${mosquittopp_LIBRARY}
  INTERFACE_INCLUDE_DIRECTORIES ${mosquittopp_INCLUDE_DIR}
)

mark_as_advanced(mosquitto_INCLUDE_DIR mosquitto_LIBRARY mosquittopp_INCLUDE_DIR mosquittopp_LIBRARY)