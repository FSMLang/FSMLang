CFLAGS += -I ../../../src -DARCH=$(ARCH)

SRC=$(shell find -L . -type f -name "*.c" -printf "%P ")

OBJ=$(SRC:.c=.o)

clean:
	-rm -f $(OBJ) test

test: $(OBJ)
	gcc -o $@ $(OBJ)

runtest: test
	./test
