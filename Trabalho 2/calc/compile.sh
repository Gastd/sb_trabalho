#! /bin/bash

nasm -f elf -F dwarf -g -o $1.o $1.asm
ld -m elf_i386 -o $1.elf $1.o
# gdbgui $1.elf
