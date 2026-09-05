cmake_minimum_required(VERSION 4.2)
project(JSBSimUnityWrapper LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Directory di output
set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/unity)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})

# Costruisce JSBSim come libreria condivisa insieme al wrapper
set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
add_subdirectory(jsbsim)

# Il wrapper C, compilato insieme
add_library(JSBSimUnity SHARED
    src/jsbsim_c_api.h
    src/jsbsim_c_api.cpp
)

target_include_directories(JSBSimUnity PRIVATE
    jsbsim/src
)

target_link_libraries(JSBSimUnity PRIVATE
    libJSBSim
)

# Su Windows, esporta correttamente i simboli
if (WIN32)
    set_target_properties(JSBSimUnity PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS OFF
    )
endif()
