cmake_minimum_required(VERSION 3.16)
project(Tetris LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

FetchContent_Declare(
    SDL
    GIT_REPOSITORY https://github.com/libsdl-org/SDL
    GIT_TAG "release-3.4.0"
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/SDL
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(SDL)

FetchContent_Declare(
    ImGui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG "v1.92.6"
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/ImGui
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(ImGui)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*.cpp" "src/*.h")

add_executable(${PROJECT_NAME} ${SRC_FILES})

source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${SRC_FILES})

# Include directories
target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_SOURCE_DIR}/external/ImGui
    ${CMAKE_SOURCE_DIR}/external/ImGui/backends
)

# Link the SDL3 library.
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

set_property(DIRECTORY ${CMAKE_CURRENT_SRC_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})