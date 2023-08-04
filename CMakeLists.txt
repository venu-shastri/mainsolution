#######################################################
### Modern C++  examples              ###
#######################################################
# Project information
cmake_minimum_required(VERSION 3.14)
project(MainSolution
        VERSION 1.0.0
        DESCRIPTION "Modern C++ Examples"
        HOMEPAGE_URL "https://github.com/venu-shastri/mainsolution.git"
        )
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_SUPPRESS_REGENERATION true)


#######################################################
### CMake Functions                                 ###
#######################################################
# Put ./cmake directory in our include paths
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
# Include our custom cmake functions
include(cmake/functions.cmake)
# Include FetchContent scripts
include(FetchContent)




# What to build
option(BUILD_EXAMPLES "Build examples" ON)

# Find package we need for threads
# Some systems don't require this package at all
# but will still give us errors trying to link
# to a target that doesn't exist
find_package(Threads)
find_package(Execution)
if (NOT Threads_FOUND)
    add_library(Threads INTERFACE)
    add_library(Threads::Threads ALIAS Threads)
endif()


#######################################################
###  examples                           ###
#######################################################
if (BUILD_EXAMPLES)
    message("Configuring examples")
    add_subdirectory(examples)
endif ()

#######################################################
### Final messages if everything is OK              ###
#######################################################
message(STATUS "🥳 Project configured successfully")
message(STATUS "❤️ Would you like to show some love by starring the repo?")
message(STATUS "🟡 Visit https://github.com/venu-shastri/mainsolution.git ⭐️")
message(STATUS "🙏 Thanks! It means a lot to me!")
