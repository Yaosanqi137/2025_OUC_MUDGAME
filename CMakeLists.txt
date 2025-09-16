cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)

set(NAME BoxingGame)

project(2025_OUC_MUDGAME)

include(FetchContent)
FetchContent_Declare(
        toml11
        GIT_REPOSITORY https://github.com/ToruNiina/toml11.git
        GIT_TAG        v4.4.0
)
FetchContent_Declare(
        ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
        GIT_TAG        v6.1.9
)
FetchContent_MakeAvailable(toml11 ftxui)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY = ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

file(GLOB_RECURSE files "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")

add_executable(${NAME} ${files})

target_link_libraries(${NAME} PRIVATE toml11::toml11 ftxui::screen ftxui::dom ftxui::component)