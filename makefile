CPP_FLAGS = -g 		\
	-pipe 					\
	-lpthread 			\
	-std=c++2a 			\
	-Werror 				\
	-ftemplate-backtrace-limit=0 \
	-finline-functions \
	-funswitch-loops \
	--verbose

.PHONY: prepare
prepare:
	bear -- make build

.PHONY: clean
clean:
	@-rm -f *.o
	@-rm -f *.asm
	@-rm -f *.out
	@-rm -f *.S

.PHONY: build
build: clean 
	g++ -c src/example/*.cpp src/main.cpp ${CPP_FLAGS}
	g++ src/example/*.cpp src/main.cpp -o main.out

.PHONY: start
start:
	@echo
	@./main.out

.PHONY: debug
debug:
	@gdb main.out
