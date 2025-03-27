cmake_minimum_required(VERSION 3.10)

project(AttitudeIndicator)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define asset directory
set(ASSET_DIR "${CMAKE_SOURCE_DIR}/assets/")
add_definitions(-DASSET_DIR="${ASSET_DIR}")

# Explicitly set output directories for all configurations
foreach(CONFIG_TYPE DEBUG RELEASE RELWITHDEBINFO MINSIZEREL)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG_TYPE} "${CMAKE_BINARY_DIR}/${CONFIG_TYPE}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIG_TYPE} "${CMAKE_BINARY_DIR}/${CONFIG_TYPE}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONFIG_TYPE} "${CMAKE_BINARY_DIR}/${CONFIG_TYPE}")
endforeach()

# Compiler flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

# Windows-specific linker settings
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:WinMainCRTStartup")

# Automatically disable console in Release mode
option(SHOW_CONSOLE "Show console window" OFF)

if(SHOW_CONSOLE)
    add_definitions(-DSHOW_CONSOLE)
else()
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:libcmt.lib")
endif()

# Ensure using static runtime library for MSVC
if(WIN32 AND MSVC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:LIBCMT")
endif()

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/src
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

# Add GLAD source file
add_library(glad STATIC ${CMAKE_SOURCE_DIR}/external/glad/src/glad.c)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES} ${IMGUI_SRC})

# Link against libraries
target_link_libraries(${PROJECT_NAME} glad OpenGL::GL ${GLFW_LIB})

# Platform-specific compiler options
if (WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE /wd4996)
endif()

# Replace the existing ASSET_DIR definition with:
set(ORIGINAL_ASSET_DIR "${CMAKE_SOURCE_DIR}/assets/")
set(ASSET_DIR "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets/")
add_definitions(-DASSET_DIR="${ASSET_DIR}")

# Modify the custom command to ensure assets are copied first
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    # Ensure the destination assets directory exists
    COMMAND ${CMAKE_COMMAND} -E make_directory "${ASSET_DIR}"

    # Copy the original assets to the build directory
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${ORIGINAL_ASSET_DIR}" "${ASSET_DIR}"

    COMMENT "Copying assets to the output directory"
)
