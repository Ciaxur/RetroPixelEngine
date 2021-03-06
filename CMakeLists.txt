cmake_minimum_required(VERSION 3.15)
project(retroApp)

# C++ Standard Version
set(CMAKE_CXX_STANDARD 17)

# Get Build Type
if(NOT CMAKE_BUILD_TYPE)            # Default = Release
    set(CMAKE_BUILD_TYPE Release)
endif()

# Congiure C++ Flags (GNU Only)
IF(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-Wall -Wextra")    # General C-Flags
    set(CMAKE_CXX_FLAGS_DEBUG "-g")         # Debug Compilation
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")      # Best Optimization
ENDIF(CMAKE_COMPILER_IS_GNUCXX)


# Find SDL2 and OpenGL
IF (WIN32)
    # Include the SDL2 Directory
    include_directories(dependencies/SDL2/include)
    set(SDL2_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/dependencies/SDL2")

    # Support both 32 and 64 bit builds
    if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
        set(SDL2_LIBS "${CMAKE_CURRENT_LIST_DIR}/dependencies/SDL2/lib/x64/SDL2.lib;${CMAKE_CURRENT_LIST_DIR}/dependencies/SDL2/lib/x64/SDL2main.lib")
    else ()
        set(SDL2_LIBS "${CMAKE_CURRENT_LIST_DIR}/dependencies/SDL2/lib/x86/SDL2.lib;${CMAKE_CURRENT_LIST_DIR}/dependencies/SDL2/lib/x86/SDL2main.lib")
    endif ()
    string(STRIP "${SDL2_LIBS}" SDL2_LIBS)

ELSE()
    set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
    find_package(SDL2 REQUIRED COMPONENTS main)
    find_package(SDL2_ttf REQUIRED)
    
    include_directories(${SDL2_INCLUDE_DIRS} ${SDL2main_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${CMAKE_BINARY_DIR})
ENDIF()

# Add spdlog as Dependency
add_subdirectory(dependencies/spdlog)
set(spdlog_DIR dependencies/spdlog)


add_executable(retroApp 
    src/main.cc 
    include/RetroPixelEngine.h      src/RetroPixelEngine.cc
    include/HelperFunctions.hpp
    )

target_link_libraries(retroApp ${SDL2_LIBS} ${SDL2_TTF_LIBRARIES} ${OPENGL_LIBRARIES} spdlog)