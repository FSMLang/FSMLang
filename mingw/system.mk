#Bad, all three of these are supposed to be applications from C:\cygwin\bin but there is no checking to 
#make sure that that's what we are using.
LEX = flex -I
YACC = bison -d -y
CC = i686-w64-mingw32-gcc.exe
DIFF = diff -b

LDFLAGS += -L /usr/lib

