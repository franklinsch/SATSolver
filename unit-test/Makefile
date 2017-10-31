CC=gcc
CFLAGS=-iquote../ -Wall -Wextra -Wpedantic $(flags)

DEPS = $(wildcard *.h)
SRCS=$(wildcard *.c)
UPDEPS=$(wildcard ../*c)
OBJS=$(wildcard ../*.o)
TST=$(SRCS:.c=.tst)

%.o: %.c $(UPDEPS)
	$(CC) $(CFLAGS) -c $< -o $@


%.tst : %.c ../%.o
	$(CC) -g $(CFLAGS) $^ -o $@


test: $(TST)
	./run-tests


.PHONY: all
all: $(TST)

.PHONY: clean
clean:
	rm -f *.tst
