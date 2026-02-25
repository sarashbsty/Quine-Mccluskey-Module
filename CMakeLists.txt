cmake_minimum_required(VERSION 3.15)
project(Quine-Mccluskey LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Include headers globally
include_directories(include)

file(GLOB_RECURSE ALL_SOURCES
    "source/*.c"
    "utils/*.c"
	"cjson/*.c"
)

add_executable(quine ${ALL_SOURCES} ${ALL_HEADERS})

if (EMSCRIPTEN)
    target_link_options(quine PRIVATE
        -sMODULARIZE=1
        -sEXPORT_NAME=QMModule
        "-sEXPORTED_FUNCTIONS=['_qm_run','_free']"
        "-sEXPORTED_RUNTIME_METHODS=['stringToNewUTF8','UTF8ToString']"
    )
endif()

set_target_properties(quine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)
