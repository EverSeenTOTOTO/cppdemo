MAIN = main

CPP_FLAGS = -g 		\
	-pipe 					\
	-lpthread 			\
	-std=c++2a 			\
	-Werror 				\
	-ftemplate-backtrace-limit=0 \
	-fext-numeric-literals \
	-finline-functions \
	-funswitch-loops \
	--verbose

C_FLAGS = -g \
  -pipe \
	-lpthread \
	-Werror \
	--verbose

.PHONY: clean
clean:
	@-rm -f *.o
	@-rm -f *.asm
	@-rm -f *.out
	@-rm -f *.S

.PHONY: c
c: clean
	gcc src/c/${MAIN}.c -o ${MAIN}.out ${C_FLAGS}
	objdump -S $(MAIN).out > $(MAIN).asm
	echo
	./$(MAIN).out

.PHONY: cpp
cpp: clean
	g++ src/${MAIN}.cpp -o ${MAIN}.out ${CPP_FLAGS}
	./$(MAIN).out

.PHONY: start
start:
	./$(MAIN).out

.PHONY: debug
debug:
	@gdb ${MAIN}.out
