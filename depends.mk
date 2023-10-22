%.d: %.c
	@set -e; rm -f $@; \
	$(CC) -M -MG $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : Makefile ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

%.i: %.c
	$(CC) -E $(CFLAGS) $< > $@

ifneq ($(MAKECMDGOALS),clean)
-include $(SRC:.c=.d)
endif

