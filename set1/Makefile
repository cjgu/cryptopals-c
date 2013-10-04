
utils.o: utils.c utils.h
	clang -g -c utils.c -o utils.o

hextob64: hextob64.c utils.o
	clang -g utils.o hextob64.c -o hextob64

xor: xor.c utils.o
	clang -g utils.o xor.c -o xor

search-single-char-xor: search-single-char-xor.c utils.o
	clang -g utils.o search-single-char-xor.c -o search-single-char-xor

find-single-char-xor-string: find-single-char-xor-string.c utils.o
	clang -g utils.o find-single-char-xor-string.c -o find-single-char-xor-string

encrypt-xor: encrypt-xor.c utils.o
	clang -g utils.o encrypt-xor.c -o encrypt-xor

break-repeating-key-xor: break-repeating-key-xor.c utils.o
	clang -g utils.o break-repeating-key-xor.c -o break-repeating-key-xor

tests: utils.o tests.c
	clang -g utils.o tests.c -lcmocka -o tests

all: hextob64 xor search-single-char-xor find-single-char-xor-string encrypt-xor \
	break-repeating-key-xor

clean:
	rm *.o hextob64 xor
