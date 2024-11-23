#Bad, all three of these are supposed to be applications from C:\cygwin\bin but there is no checking to 
#make sure that that's what we are using.
LEX = flex -I 
YACC = bison -d -y -v #--debug -d
#CC = gcc -g -ggdb -Werror #-Wall
CC = gcc
DIFF = diff --strip-trailing-cr --ignore-space-change
override CFLAGS+=-DLINUX

