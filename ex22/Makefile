EXEC = ex22
OBJS = ex22.o ex22_main.o
CFLAGS = -std=gnu99 -Wall -g
vpath += ../common/include

.PHONY: all

all: $(EXEC)

$(EXEC): $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<


clean:
	$(RM) $(OBJS) $(EXEC)


doc: $(OBJS)
	@doxygen

dis-clean:
	@rm -fr doc

