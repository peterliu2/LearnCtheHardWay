
EXEC = ex20
CFLAGS = -std=c99 -Wall -g
OBJS = ex20.o

.PHONY: all

all: $(EXEC)


$(EXEC): $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJS) $(EXEC)

doc:
	@doxygen

checkpatch:
	/usr/src/linux-source-4.2.0/linux-source-4.2.0/scripts/checkpatch.pl --file ex20.c dbg.h > checkpatch_log

astyle:
	astyle -r --style=kr --indent=spaces=4 --indent-switches --suffix=none "*.c" && astyle -r --style=kr --indent=spaces=4 --indent-switches --suffix=none "*.h"

cocci:
	make $(EXEC) coccicheck MODE=report

sparse:
	make C=2 $(EXEC)


