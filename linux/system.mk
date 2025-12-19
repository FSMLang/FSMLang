LEX = flex -I 
YACC = bison -d -y -v #--debug -d
#CC = gcc -g -ggdb -DFSMLANG_DEVELOP #-Wall -Werror 
CC = gcc
DIFF = diff --strip-trailing-cr --ignore-space-change $(DIFF_FLAGS)
override CFLAGS+=-DLINUX

