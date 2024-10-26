cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20 CACHE INTERNAL "The C++ Standard this Project relies on")
set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE BOOL "Require a specific C++ Standard")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib" CACHE STRING "Unified Output Directory for Archives")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib" CACHE STRING "Unified Output Directory for Libraries")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin" CACHE STRING "Unified Output Directory for Executables")

project(CircleCollisionSandbox LANGUAGES CXX)

set(CNL_DIR "${CMAKE_CURRENT_LIST_DIR}/extern/cnl")
find_package(Cnl REQUIRED
        CONFIG REQUIRED
        PATHS "${CNL_DIR}"
        COMPONENTS Cnl)

set(SDL_DIR "${CMAKE_CURRENT_LIST_DIR}/extern/sdl")
find_package(SDL3 REQUIRED
        CONFIG REQUIRED
        PATHS "${SDL_DIR}"
        COMPONENTS SDL3-static)

set(TARGET_NAME ${CMAKE_PROJECT_NAME})

set(TARGET_SRC "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
file(GLOB_RECURSE TARGET_INC "${CMAKE_CURRENT_SOURCE_DIR}/include/*")

add_executable(${TARGET_NAME})
target_sources(${TARGET_NAME} PRIVATE ${TARGET_SRC})
target_sources(${TARGET_NAME} PRIVATE ${TARGET_INC})
target_include_directories(${TARGET_NAME} PRIVATE include)

set_target_properties(${TARGET_NAME} PROPERTIES
        LINKER_LANGUAGE CXX
        CXX_STANDARD ${CMAKE_CXX_STANDARD}
        CXX_STANDARD_REQUIRED ${CMAKE_CXX_STANDARD_REQUIRED})

target_link_libraries(${TARGET_NAME} PRIVATE Cnl::Cnl)
target_link_libraries(${TARGET_NAME} PRIVATE SDL3::SDL3-static)
