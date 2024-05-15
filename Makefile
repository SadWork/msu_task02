all: prog.efi

prog: main.o root.o func.o segment.h
	gcc -m32 -o prog.efi main.o root.o func.o -lm

main.o: main.c func.o
	gcc -m32 -c -o main.o main.c -lm

root.o: root.c root.c
	gcc -m32 -c -o root.o root.c -lm

func.o:
	gcc -o function_to_asm.efi compiler/function_to_asm.c -lm
	./function_to_asm.efi -i .debug/spec.txt > func.asm
	nasm -f elf32 -o func.o func.asm

clean:
	rm func.o segment.o segment.c root.o main.o prog.efi function_to_asm.efi
	