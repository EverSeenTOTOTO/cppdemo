MAIN = main

CPP_FLAGS = -g 		\
	-pipe 					\
	-lpthread 			\
	-std=c++2a 			\
	-Werror 				\
	-ftemplate-backtrace-limit=0 \
	-finline-functions \
	-funswitch-loops \
	--verbose

.PHONY: clean
clean:
	@-rm -f *.o
	@-rm -f *.asm
	@-rm -f *.out
	@-rm -f *.S

.PHONY: build
build: clean
	bear -- g++ src/${MAIN}.cpp -o ${MAIN}.out ${CPP_FLAGS}
	objdump -S $(MAIN).out > $(MAIN).asm
	echo
	./$(MAIN).out

.PHONY: debug
debug:
	@gdb ${MAIN}.out
