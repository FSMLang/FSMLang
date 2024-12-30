#
#  Configuration to support FSMLang version 2
#
LEXER_FEATURES=SUPPORT_INCLUDE_FILES
LEXER_SRC=lexer_v2.c
override CFLAGS+=-DSUPPORT_INCLUDE_FILES
