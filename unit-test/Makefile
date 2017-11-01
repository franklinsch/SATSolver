CC=gcc
CFLAGS=-iquote../ -g -Wall -Wextra -Wpedantic $(flags)

UPDEPS=$(wildcard ../*.h)
SRCS=$(wildcard *.c)
TST=$(SRCS:.c=.tst)

%.o: %.c $(UPDEPS)
	$(CC) $(CFLAGS) -c $< -o $@


%.tst : %.c ../%.o
	$(CC) $(CFLAGS) $^ -o $@


test: $(TST) run-tests
	./run-tests


.PHONY: all
all: $(TST)

.PHONY: clean
clean:
	rm -f *.tst ../*.o
