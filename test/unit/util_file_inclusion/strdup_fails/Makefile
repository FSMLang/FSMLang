#here, it is only necessary to list source files that are not local to the current directory.
SRC=util_file_inclusion.c

LIB_SRC=fsm_utils.c   \
        lexer_debug.c \
        list.c


$(LIB_SRC:.c=.o): override CFLAGS+=-DLEX_DEBUG

TARGET=diff_canonical
GRAB_STDERR=2>&1

include ../../unitTest.mk

