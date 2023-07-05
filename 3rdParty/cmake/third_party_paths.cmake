# Setting the top level directory
if (NOT GIT_ROOT)
    execute_process(COMMAND git rev-parse --show-toplevel
    OUTPUT_STRIP_TRAILING_WHITESPACE
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
    OUTPUT_VARIABLE GIT_ROOT)
    message(STATUS "GIT_ROOT = ${GIT_ROOT}")
endif()

set(MOSQUITTO_PATH  "${GIT_ROOT}/3rdParty/mosquitto")
set(OPENSSL_PATH    "${GIT_ROOT}/3rdParty/openssl")
# Set this and let cmake do the rest of the work. See Cmake FindOpenSSL
set(OPENSSL_ROOT_DIR            "${OPENSSL_PATH}/openssl-1.1.1k_x86-linux-gnu")



# CMAKE_PREFIX_PATH will be searched first by functions like find_package()

list(APPEND CMAKE_PREFIX_PATH   "${MOSQUITTO_PATH}/lib/x86_64")
list(APPEND CMAKE_PREFIX_PATH   "${MOSQUITTO_PATH}/include")

