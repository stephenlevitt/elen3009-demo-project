# DO NOT MODIFY THIS FILE! - THIS WILL BE USED TO BUILD YOUR PROJECT

# ------------------------------------------------------------------------------
# Parts of this CMakeLists.txt file come from these sources:
# https://github.com/itsYakub/RaylibGameTemplate and
# https://github.com/robloach/raylib-cpp
# ------------------------------------------------------------------------------

# =========================== Specify Minimum Tool Versions ============================

cmake_minimum_required(VERSION 4.3.4) # the currently stable version of CMake is required
project(elen3009-project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
    # Check for minimum MinGW version
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "16.1.0")
            message(FATAL_ERROR "MinGW version 16.1.0 or higher is required on Windows.")
        endif()
    endif()
endif()

# Put all generated executables and docs alongside this CMakeLists.txt file
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# ====================== Download and Build Dependencies ======================

include(FetchContent)

# raylib
find_package(raylib QUIET)
if (NOT raylib_FOUND)
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 6.0
        GIT_SHALLOW TRUE # only clone latest commit, restricted to branch names and tags
    )
    FetchContent_MakeAvailable(raylib)
endif()

# raylib-cpp
find_package(raylib_cpp QUIET)
if (NOT raylib_cpp_FOUND)
    FetchContent_Declare(
        raylib_cpp
        GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
        GIT_TAG v6.0.2
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(raylib_cpp)
endif()

# doctest (modified doctest.h compatible with raylib)
# see issue: https://github.com/raysan5/raylib/issues/1217
find_package(doctest QUIET)
if (NOT doctest_FOUND)
    set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE) # suppress deprecation warning
    FetchContent_Declare(
        doctest
        GIT_REPOSITORY https://github.com/stephenlevitt/doctest-with-raylib.git
        GIT_TAG v1.0
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(doctest)
    set(CMAKE_WARN_DEPRECATED ON CACHE BOOL "" FORCE)
endif()

# =========================== Select Source Files for Compilation ============================

set(MAIN_CPP "main.cpp") # the cpp file that runs the game and contains the entry point main() function
set(SRC_PATH "${CMAKE_SOURCE_DIR}/game-source-code") # path to game source code folder

# CONFIGURE_DEPENDS is used to make sure that the globbing is re-run when a new source or test file is added
file(GLOB GAME_SRC CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/game-source-code/*.cpp)
file(GLOB TEST_SRC_FILES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/test-source-code/*.cpp)
set(TESTS_SRC ${GAME_SRC} ${TEST_SRC_FILES})
# remove MAIN_CPP from the list of test source files: doctest provides its own main function
list(REMOVE_ITEM TESTS_SRC "${CMAKE_SOURCE_DIR}/game-source-code/${MAIN_CPP}")

# ==================================== Setup Targets =========================================

# Game executable target
set(GAME_EXE "game") # name of the game executable
add_executable(${GAME_EXE} ${GAME_SRC}) # shows the console output
# To prevent the console output from appearing when the game is run, replace the above line with this one:
# add_executable(${GAME_EXE} WIN32 ${GAME_SRC})
target_include_directories(${GAME_EXE} PRIVATE ${SRC_PATH})

#  Test executable target
set(TESTS_EXE "tests") # name of the test executable
add_executable(${TESTS_EXE} ${TESTS_SRC})
target_include_directories(${TESTS_EXE} PRIVATE ${SRC_PATH})
target_include_directories(${TESTS_EXE} PRIVATE "${doctest_SOURCE_DIR}/doctest") # include doctest header

# ================================= Linker Settings ==========================================

if (WIN32)
    # Statically link libraries on Windows
    # (Source: https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html)
    target_link_options(${GAME_EXE} PRIVATE -static)
    target_link_libraries(${GAME_EXE} PRIVATE raylib_cpp raylib)
    target_link_options(${TESTS_EXE} PRIVATE -static)
    target_link_libraries(${TESTS_EXE} PRIVATE raylib_cpp raylib)
elseif (LINUX)
    message("Producing builds for Linux")
    message("NB: your releases have to build and run on Windows using the toolchain given in Lab 0!")

    # Linux: Telling the linker to statically link the libgcc and libstdc++ to the project.
    # (Source: https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html)
    target_link_options(${GAME_EXE} PRIVATE -static-libgcc -static-libstdc++)
    target_link_libraries(${GAME_EXE} PRIVATE raylib_cpp raylib)
    target_link_options(${TESTS_EXE} PRIVATE -static-libgcc -static-libstdc++)
    target_link_libraries(${TESTS_EXE} PRIVATE raylib_cpp raylib)
elseif(APPLE)
    message("Producing builds for macOS")
    message("NB: your releases have to build and run on Windows using the toolchain given in Lab 0!")

    # Include paths for IntelliSense and compiling
    target_include_directories(${GAME_EXE} PRIVATE ${raylib_SOURCE_DIR}/src ${raylib_cpp_SOURCE_DIR}/include)
    target_include_directories(${TESTS_EXE} PRIVATE ${raylib_SOURCE_DIR}/src ${raylib_cpp_SOURCE_DIR}/include)
    # Link the necessary macOS frameworks
    target_link_libraries(${GAME_EXE} PRIVATE
        "-framework IOKit"  # Access hardware devices and drivers from your apps and services.
        "-framework Cocoa"  # Native object-oriented application programming interface.
        "-framework OpenGL" # Cross-language, cross-platform application programming interface for rendering 2D and 3D vector graphics.
        raylib_cpp
        raylib
    )
    target_link_libraries(${TESTS_EXE} PRIVATE
        "-framework IOKit"
        "-framework Cocoa"
        "-framework OpenGL"
        raylib_cpp
        raylib
    )
else()
    message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}. Only Windows, Linux, and macOS are supported.")
endif()

# ====================================== Doxygen ==========================================

# Extract Doxygen documentation from the game source code
# Documentation is placed in a folder called "doxygen" alongside CMakeLists.txt.
# Open the index.html file in a browser to view the documentation.
# NB: Doxygen is not a dependency of the project, it is assumed that it is already installed on your system
find_package(Doxygen QUIET)

if (DOXYGEN_FOUND)
    # Doxygen options: see https://www.doxygen.nl/manual/config.html
    set(DOXYGEN_GENERATE_TREEVIEW YES)
    # Place doxygen output alongside this CMakeLists.txt file (HTML will be under doxygen/html)
    set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/doxygen")
    # Comment out the following line if you want warnings for undocumented code
    set(DOXYGEN_WARN_IF_UNDOCUMENTED NO)
    # Doxygen docs will be produced as part of the 'all' target
    doxygen_add_docs(doxygen_docs
        "${SRC_PATH}"
        ALL
        COMMENT "Generate HTML documentation")
endif()


# ======================================== CTest ============================================

include(CTest)
enable_testing()
if (doctest_SOURCE_DIR)
    include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)
endif()
# automatically add doctest tests to CTest; specify WORKING_DIRECTORY to ensure that relative paths are correct for CTest
doctest_discover_tests(${TESTS_EXE} WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})


