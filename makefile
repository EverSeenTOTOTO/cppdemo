MAIN = main
ENTRY = src/$(MAIN).cpp

CPP_FLAGS = -g 		\
	-pipe 					\
	-ldl 						\
	-lpthread 			\
	-lssl 					\
	-lcrypto 				\
	-std=c++2a 			\
	-Werror 				\
	-ftemplate-backtrace-limit=0 \
	-fext-numeric-literals \
	-finline-functions \
	-funswitch-loops

clean:
	@-rm -f *.o
	@-rm -f *.out

build: clean
	@g++ $(ENTRY) -o $(MAIN).out ${CPP_FLAGS}

start: build
	./$(MAIN).out

debug: build
	@gdb ${MAIN}.out
