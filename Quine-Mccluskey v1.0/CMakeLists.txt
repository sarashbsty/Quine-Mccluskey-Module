cmake_minimum_required(VERSION 3.15)
project(Quine-Mccluskey LANGUAGES CXX)
set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include headers globally
include_directories(include)

file(GLOB_RECURSE ALL_SOURCES
    "source/*.cpp"
    "helper_source/*.cpp"
)

add_executable(quine ${ALL_SOURCES} ${ALL_HEADERS})

set_target_properties(quine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)
