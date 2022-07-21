GCC = clang 

.PHONY: muldef
muldef:
	${GCC} -c -o muldef.o muldef.c
	${GCC} -c -o p1.o p1.c
	${GCC} -Xlinker --allow-multiple-definition -o muldef.out muldef.o p1.o
	./muldef.out

.PHONY: ld
ld:
	${GCC} -O0 -m32 -fno-pic -c -o main.o main.c
	${GCC} -O0 -m32 -fno-pic -c -o swap.o swap.c 
	${GCC} -m32 -nostdlib -no-pie -o main.out main.o swap.o

.PHONY: llvmir
llvmir:
	${GCC} swap.c -S -emit-llvm -fno-discard-value-names -c
	${GCC} main.c -S -emit-llvm -fno-discard-value-names -c

.PHONY: llvm 
llvm:
	llc swap.ll -o swap.s
	llc main.ll -o main.s
