cmake_minimum_required(VERSION 3.5.0)
project("BSOD Trojan" VERSION 1.0 LANGUAGES CXX)

# Add icon for windows (lol it won't compile on other platforms anyway)
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    enable_language("RC")
    set (WIN32_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/resources/app.rc)
endif()

include(CTest)
enable_testing()

# Compile all files inside the src directory
file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(bsod ${SOURCES} ${WIN32_RESOURCES})
# Diffrent executable names for debug and release builds
set(CMAKE_DEBUG_POSTFIX _debug)
set(CMAKE_RELEASE_POSTFIX _release)
set_target_properties(bsod PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
set_target_properties(bsod PROPERTIES RELEASE_POSTFIX ${CMAKE_RELEASE_POSTFIX})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic-errors \
-Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion")

# Hide console window in release builds
string(APPEND CMAKE_CXX_FLAGS_RELEASE " -mwindows")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
