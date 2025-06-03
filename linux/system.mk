LEX = flex -I 
YACC = bison -d -y -v #--debug -d
#CC = gcc -g -ggdb -Werror -DFSMLANG_DEVELOP #-Wall
CC = gcc
DIFF = diff --strip-trailing-cr --ignore-space-change
override CFLAGS+=-DLINUX

