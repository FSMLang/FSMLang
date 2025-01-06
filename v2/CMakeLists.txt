cmake_minimum_required(VERSION 3.1...3.26)

project(fsmlang VERSION 2.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

find_package(FLEX 2.6 REQUIRED)
find_package(BISON 3.7 REQUIRED)

BISON_TARGET(FSM_PARSER ${CMAKE_CURRENT_SOURCE_DIR}/../src/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c
             DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/y.tab.h COMPILE_FLAGS -Wno-conflicts-sr)

FLEX_TARGET(FSM_LEXER ${CMAKE_CURRENT_SOURCE_DIR}/../src/lexer_v2.l  ${CMAKE_CURRENT_BINARY_DIR}/lexer_v2.c)

ADD_FLEX_BISON_DEPENDENCY(FSM_LEXER FSM_PARSER)

file(GLOB FSM_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../src/*.c)

add_executable(fsmv2 ${BISON_FSM_PARSER_OUTPUTS} ${FLEX_FSM_LEXER_OUTPUTS} ${FSM_SOURCES} )

set_target_properties(
	fsmv2
	PROPERTIES
		OUTPUT_NAME fsm
)

target_include_directories(fsmv2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../src)
target_include_directories(fsmv2 PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

if(WIN32)
    target_compile_definitions(fsmv2 PUBLIC CYGWIN MINGW MSDOS FSM_DEBUG _XOPEN_SOURCE=700 YY_NO_UNPUT)
endif()

if(UNIX AND NOT APPLE)
    target_compile_definitions(fsmv2 PUBLIC LINUX FSM_DEBUG _XOPEN_SOURCE=700 YY_NO_UNPUT)
endif()

target_compile_options(fsmv2 PRIVATE -Wall -Wextra -pedantic -Werror -DSUPPORT_INCLUDE_FILES)

