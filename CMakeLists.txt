cmake_minimum_required(VERSION 3.10)

project(AttitudeIndicator)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Option to control console visibility
option(SHOW_CONSOLE "Show console window" OFF)

# Always set Windows entry point
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:WinMainCRTStartup")

# Conditionally add SHOW_CONSOLE definition
if(SHOW_CONSOLE)
    add_definitions(-DSHOW_CONSOLE)
else()
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:libcmt.lib")
endif()

# Option to enable/disable NDEBUG
option(ENABLE_NDEBUG "Disable assertions and enable release optimizations" ON)

# Add -DNDEBUG only if ENABLE_NDEBUG is ON
if (ENABLE_NDEBUG)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")
endif()

# Ensure using static runtime library for MSVC
if(WIN32 AND MSVC)
    # Link against specific libraries to avoid default library conflicts
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:LIBCMT")
endif()

# Include src directory
include_directories(${CMAKE_SOURCE_DIR}/src)

# Include external directories
include_directories(
    ${CMAKE_SOURCE_DIR}/external/glad/include
    ${CMAKE_SOURCE_DIR}/external/imgui
    ${CMAKE_SOURCE_DIR}/external/
)

# IMGUI source files
set(IMGUI_SRC
    ${CMAKE_SOURCE_DIR}/external/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/backends/imgui_impl_glfw.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/backends/imgui_impl_opengl3.cpp
)

# Find OpenGL
find_package(OpenGL REQUIRED)
message(STATUS "OpenGL found.")

# GLFW Configuration
if(WIN32)
    set(GLFW_ROOT "${CMAKE_SOURCE_DIR}/external/glfw-3.4")
    include_directories("${GLFW_ROOT}/include")
    set(GLFW_LIB_DIR "${GLFW_ROOT}/lib-vc2022")
    set(GLFW_LIB "${GLFW_LIB_DIR}/glfw3_mt.lib")
endif()

link_directories("${GLFW_LIB_DIR}")

# Find all .cpp files in src/ and its subdirectories
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Define asset directory as an absolute path
set(ASSET_DIR "${CMAKE_SOURCE_DIR}/assets/")
add_definitions(-DASSET_DIR="${ASSET_DIR}")

# Add GLAD source file
add_library(glad STATIC ${CMAKE_SOURCE_DIR}/external/glad/src/glad.c)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES} ${SCRIPT_SOURCES} ${IMGUI_SRC})

# Link against libraries
target_link_libraries(${PROJECT_NAME} glad OpenGL::GL ${GLFW_LIB})

# Platform-specific compiler options
if (WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE /wd4996)
endif()
