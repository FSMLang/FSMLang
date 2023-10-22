VPATH=../../../src

ifdef OUTPUT_DIR
include $(OUTPUT_DIR)/system.mk
endif

TARGET ?= runtest

override CFLAGS += -I../ -I../../../src -DARCH=$(ARCH)

SRC+=$(shell find -L . -type f -name "*.c" -printf "%P ")

OBJ=$(SRC:.c=.o)

all: $(TARGET)

clean:
	-@rm -f $(OBJ) test test.out test.result *.d

test: $(OBJ)
	gcc -o $@ $(OBJ)

runtest: test
	./test > test.out
	rm test.out

diff_canonical: test
	./test > test.out
	diff test.out test.canonical > test.result
	rm test.result test.out

