
BIN  := assembler
SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)

CFLAGS := -g
LDLIBS :=

$(BIN): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDLIBS)

%.o : %.c $(wildcard *.h)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf *.o $(BIN) tmp *.plist *.txt valgrind.out main.out

svec.o: svec.c svec.h
	gcc $(CFLAGS) -c -o $@ $<
	
map.o: map.c map.h
	gcc $(CFLAGS) -c -o $@ $<

.PHONY: clean test
