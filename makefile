# GCC = riscv64-linux-gnu-g++-10
# CPP = riscv64-linux-gnu-cpp
# AS = riscv64-linux-gnu-as
# AR = riscv64-linux-gnu-ar
# OBJDUMP = riscv64-linux-gnu-objdump

GCC = g++
CPP = cpp
AS = as
AR = ar
OBJDUMP = objdump

# GCC = clang++ # see https://bugs.llvm.org/show_bug.cgi?id=52265
# CPP = clang-cpp
# AS = llvm-as
# AR = llvm-ar
# OBJDUMP = llvm-objdump

CPP_FLAGS = -g\
	-O0\
	-pipe\
	-std=c++2a\
	-fdiagnostics-color=always

.PHONY: prepare
prepare:
	bear -- make build

.PHONY: clean
clean:
	@-rm -rf src/**/*.{s,i,o,out,a,ll}
	@-rm -rf src/*.{s,i,o,out,a,ll}
	@-rm -rf *.{s,i,o,out,a,ll}

# 对每一个.cpp文件替换.cpp为.i，得到cppfiles
cppfiles = $(patsubst %.cpp,%.i,$(wildcard src/**/*.cpp src/*.cpp))

# 对每一个.i文件，声明它依赖对应的.cpp文件，然后使用预处理命令cpp生成它。$< 代表第一个依赖文件，$@ 会对每一个目标依次执行
$(cppfiles): %.i: %.cpp
	${CPP} $< -o $@

# cpp伪依赖统筹所有.i文件
.PHONY: cpp
cpp: $(cppfiles)

asmfiles = $(subst .i,.s,$(cppfiles))

# 编译
$(asmfiles): %.s: %.i
	${GCC} -S ${CPP_FLAGS} $< -o $@

cc1: $(asmfiles)

objfiles = $(subst .i,.o,$(cppfiles))

# 汇编
$(objfiles): %.o: %.s
	${AS} $< -o $@

as: $(objfiles)

# 链接
.PHONY: build
build: $(objfiles)
	${GCC} -fuse-ld=lld -v -Xlinker --warn-common -o main.out $(objfiles)
	@echo -e '\n\033[1;32m Built main.out. \033[0m'

.PHONY: start
start: build
	./main.out

# 静态库
ar:
	ar rs cppd.a src/example/semaphore.o
