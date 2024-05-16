CFLAGS ?= -O3

all: prog.efi

prog.efi: main.o root.o func.o segment.h
	gcc -m32 -o prog.efi main.o root.o func.o -lm

main.o: main.c func.o
	gcc $(CFLAGS) -m32 -c -o main.o main.c

root.o: root.c
	gcc $(CFLAGS) -m32 -c -o root.o root.c

func.o:
	gcc $(CFLAGS) -o function_to_asm.efi compiler/function_to_asm.c -lm
	./function_to_asm.efi -i .debug/spec.txt > func.asm
	nasm -f elf32 -o func.o func.asm

clean:
	rm -f func.o segment.h root.o main.o prog.efi function_to_asm.efi
