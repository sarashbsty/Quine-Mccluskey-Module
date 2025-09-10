cmake_minimum_required(VERSION 3.15)
project(Quine-Mccluskey LANGUAGES C)
set(CMAKE_C_COMPILER gcc)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Include headers globally
include_directories(include)

file(GLOB_RECURSE ALL_SOURCES
    "source/*.c"
    "helper_source/*.c"
)

add_executable(quine ${ALL_SOURCES} ${ALL_HEADERS})

set_target_properties(quine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)
