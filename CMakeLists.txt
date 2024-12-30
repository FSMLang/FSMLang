cmake_minimum_required(VERSION 3.1...3.26)

set(MY_PROJ_VER "2" CACHE STRING "Set desired project version")

message("Selected project version ${MY_PROJ_VER}")

if (${MY_PROJ_VER} EQUAL "1")
project(fsmlang VERSION 1.0 LANGUAGES C CXX)
endif()

if (${MY_PROJ_VER} EQUAL "2")
project(fsmlang VERSION 2.0 LANGUAGES C CXX)
endif()

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

find_package(FLEX 2.6 REQUIRED)
find_package(BISON 3.7 REQUIRED)

BISON_TARGET(FSM_PARSER ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c
             DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/y.tab.h COMPILE_FLAGS -Wno-conflicts-sr)

if (${MY_PROJ_VER} EQUAL "1")
FLEX_TARGET(FSM_LEXER ${CMAKE_CURRENT_SOURCE_DIR}/src/lexer.l  ${CMAKE_CURRENT_BINARY_DIR}/lexer.c)
endif()

if (${MY_PROJ_VER} EQUAL "2")
FLEX_TARGET(FSM_LEXER ${CMAKE_CURRENT_SOURCE_DIR}/src/lexer_v2.l  ${CMAKE_CURRENT_BINARY_DIR}/lexer_v2.c)
endif()


ADD_FLEX_BISON_DEPENDENCY(FSM_LEXER FSM_PARSER)

file(GLOB FSM_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

add_executable(fsm ${BISON_FSM_PARSER_OUTPUTS} ${FLEX_FSM_LEXER_OUTPUTS} ${FSM_SOURCES})

target_include_directories(fsm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(fsm PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

if (${MY_PROJ_VER} EQUAL "1")

    if(WIN32)
        target_compile_definitions(fsm PUBLIC CYGWIN MINGW MSDOS FSM_DEBUG _XOPEN_SOURCE=700 YY_NO_UNPUT YY_NO_INPUT)
    endif()

    if(UNIX AND NOT APPLE)
        target_compile_definitions(fsm PUBLIC LINUX FSM_DEBUG _XOPEN_SOURCE=700 YY_NO_INPUT YY_NO_UNPUT)
    endif()

    target_compile_options(fsm PRIVATE -Wall -Wextra -pedantic -Werror)

endif()

if (${MY_PROJ_VER} EQUAL "2")

    if(WIN32)
        target_compile_definitions(fsm PUBLIC CYGWIN MINGW MSDOS FSM_DEBUG _XOPEN_SOURCE=700 YY_NO_UNPUT YY_NO_INPUT)
    endif()

    if(UNIX AND NOT APPLE)
        target_compile_definitions(fsm PUBLIC LINUX FSM_DEBUG _XOPEN_SOURCE=700 YY_NO_UNPUT)
    endif()

    target_compile_options(fsm PRIVATE -Wall -Wextra -pedantic -Werror -DSUPPORT_INCLUDE_FILES)

endif()

