cmake_minimum_required(VERSION 3.5)

project(qr-tcp-tunnel VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_BUILD_TYPE_UPPERCASE MATCHES "(RELEASE|RELWITHDEBINFO|MINSIZEREL)")
    set(RELEASE_MODE ON)
endif()

option(BUILD_APP "Build application" TRUE)
option(BUILD_CLI "Build CLI tool" TRUE)
option(BUILD_CLI_STATIC "Build static linked CLI " FALSE)

if(NOT RELEASE_MODE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror -Wno-comment -Wno-reorder -Werror=return-type -Woverloaded-virtual -Wno-unused-parameter")
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror -Wno-comment -Wno-reorder -Werror=return-type -Woverloaded-virtual")
else()
endif()

# read version
file(READ "CHANGELOG.md" VERSION_FILE_CONTENT)
string(REGEX REPLACE "^# ([0-9.]+).*$" "\\1" APP_VERSION ${VERSION_FILE_CONTENT})
message(STATUS "App version: " ${APP_VERSION})

if(BUILD_APP)
    add_subdirectory(app)
endif()

if(BUILD_CLI)
    add_subdirectory(cli)
endif()
