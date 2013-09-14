CFLAGS=-std=c++11 
CFLAGS+=-stdlib=libc++
CC=clang++
#flags for test.c
cc=clang
DEBUG=-g
#warnings
WARNINGS=-Wall
WARNINGS=-Weverything
OPT= -O0 -O1 -O2 -O3 -O4
ONUM=1
ASM=-S
BOOST=-D__BOOST__

test: test.cpp aaron.hpp
	$(CC) $(CFLAGS) $< -o $@ $(DEBUG) $(word $(ONUM), $(OPT))

clean:
	rm test
