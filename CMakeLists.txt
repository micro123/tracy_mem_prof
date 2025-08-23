cmake_minimum_required(VERSION 3.20)

project(TracyMemProf CXX)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_C_VISIBILITY_PRESET hidden)

add_library(
    ${PROJECT_NAME}
    MODULE
    src/main.cpp
    tracy/TracyClient.cpp
)

target_compile_definitions(
    ${PROJECT_NAME} PRIVATE
    TRACY_ENABLE
    TRACY_NO_CALLSTACK_INLINES
    TRACY_CALLSTACK=10
    TRACY_NO_SAMPLING
    TRACY_ON_DEMAND
    TRACY_LIBUNWIND_BACKTRACE
    # TRACY_DEBUGINFOD
)

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    dl
)

target_include_directories(
    ${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/tracy
)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    PREFIX ""
)
