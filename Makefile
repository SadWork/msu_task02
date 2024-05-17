CFLAGS ?= -Wall -O3

all: prog.efi

prog.efi: main.o root.o func.o integral.o
	gcc -m32 -o prog.efi main.o integral.o root.o func.o -lm

main.o: main.c root.h func.o integral.o
	gcc $(CFLAGS) -m32 -c -o main.o main.c

integral.o: integral.c integral.h
	gcc $(CFLAGS) -m32 -c -o integral.o integral.c

root.o: root.c root.h
	gcc $(CFLAGS) -m32 -c -o root.o root.c

func.o: func.asm
	nasm -f elf32 -o func.o func.asm

func.asm: compiler/function_to_asm.c .debug/spec.txt
	gcc $(CFLAGS) -o function_to_asm.efi compiler/function_to_asm.c -lm
	./function_to_asm.efi -i .debug/spec.txt > func.asm

clean:
	rm -f func.o segment.h integral.o root.o main.o prog.efi function_to_asm.efi func.asm
