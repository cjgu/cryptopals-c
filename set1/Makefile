
utils.o: utils.c utils.h
	clang -c utils.c -o utils.o

hextob64: hextob64.c utils.o
	clang utils.o hextob64.c -o hextob64

all: hextob64

clean: 
	rm *.o hextob64
