override CFLAGS += -Wall -Wpedantic -Wextra -DUNIT_TEST


VPATH=../../../src

ifdef OUTPUT_DIR
include $(OUTPUT_DIR)/system.mk
VPATH=$(OUTPUT_DIR)/../src
endif

TARGET ?= runtest

override CFLAGS += -I../ -I$(VPATH) -DARCH=$(ARCH)

SRC+=$(shell find -L . -type f -name "*.c" -printf "%P ")

OBJ=$(SRC:.c=.o)

all: $(TARGET)

ifdef LIB_SRC
LIB_OBJ=$(LIB_SRC:.c=.o)
LIB_NAME=fsm
ADD_LIB=-Wl,--allow-multiple-definition -L. -l$(LIB_NAME) 

$(LIB_NAME): $(LIB_OBJ)
	@$(AR) rcs $(addprefix lib, $@).a $(LIB_OBJ)

endif

clean:
	-@rm -f $(OBJ) $(LIB_OBJ) $(addprefix lib, $(LIB_NAME)).a test test.out test.result *.d > /dev/null 2>&1

test: $(OBJ) $(LIB_NAME)
	@gcc -o $@ $(OBJ) $(ADD_LIB)

runtest: test
	@./test > test.out $(GRAB_STDERR)
	-@rm -f test.out
	-@rm -f $(OBJ) test test.out test.result *.d

diff_canonical: test
	@./test > test.out $(GRAB_STDERR)
	@diff test.out test.canonical > test.result
	-@rm -f $(OBJ) test test.out test.result *.d > /dev/null 2>&1
	-@rm -f $(LIB_OBJ) $(addprefix lib, $(LIB_NAME)).a > /dev/null 2>&1

ifdef OUTPUT_DIR
include $(OUTPUT_DIR)/../depends.mk
else
include ../../../depends.mk
endif

