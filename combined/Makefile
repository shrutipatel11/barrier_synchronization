MPICC = mpicc
CC = gcc

OMPFLAGS = -fopenmp
OMPLIBS = -lgomp

CFLAGS = -g -std=gnu99 -I. -Wall $(OMPFLAGS)
LDLIBS = $(OMPLIBS) -lm
INCLUDES = /usr/include/mpich

all: combined

SRCS := $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

combined: $(OBJS)
	$(MPICC) -o $@ $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS)

%.o: %.c
	$(CC) -c -v $(CFLAGS) -I$(INCLUDES) $(OMPFLAGS) $< -o $@

clean:
	rm -rf *.o *.dSYM combined