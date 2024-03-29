SHELL := /bin/bash

# GCC = g++
# CPP = cpp
# AS = as
# AR = ar
# OBJDUMP = objdump

GCC = clang++ # see https://bugs.llvm.org/show_bug.cgi?id=52265
CPP = clang-cpp
AS = llvm-as
AR = llvm-ar
OBJDUMP = llvm-objdump

CPP_FLAGS = -g\
	-O0\
	-pipe\
	-std=c++2a\
	-pthread\
	-fdiagnostics-color=always

.PHONY: prepare
prepare:
	bear -- make build

.PHONY: lint
lint:
	@clang-format src/**/*.{h,cpp} -i

.PHONY: clean
clean:
	@-rm -rf src/**/*.{o,s,i,out,a,ll}
	@-rm -rf src/*.{o,s,i,out,a,ll}
	@-rm -rf *.{o,s,i,out,a,ll}

# NOT USED BEGIN

# 预处理
# 对每一个.i文件，声明它依赖对应的.cpp文件，然后使用预处理命令cpp生成它。$< 代表第一个依赖文件，$@ 会对每一个目标依次执行

# 对每一个.cpp文件替换.cpp为.i，得到cppfiles
cppfiles = $(patsubst %.cpp,%.i,$(wildcard src/dsal/*.cpp src/leetcode/*.cpp src/example/*.cpp src/*.cpp))
$(cppfiles): %.i: %.cpp
	${CPP} $< -o $@

# cpp伪依赖统筹所有.i文件
.PHONY: cpp
cpp: $(cppfiles)

asmfiles = $(subst .i,.s,$(cppfiles))

# 编译
$(asmfiles): %.s: %.i
	${GCC} -S $< -o $@

cc1: $(asmfiles)

objfiles = $(subst .i,.o,$(cppfiles))

# 汇编
# $(objfiles): %.o: %.s
# 	${AS} $< -o $@

# 静态库
ar:
	ar rs cppd.a src/example/semaphore.o

# NOT USED END

$(objfiles): %.o: %.cpp
	${GCC} ${CPP_FLAGS} -c $< -o $@

as: $(objfiles)

# 链接
.PHONY: build
build: $(objfiles)
	${GCC} ${CPP_FLAGS} -v -Xlinker --warn-common -o main.out $(objfiles)
	@echo -e '\n\033[1;32m Built main.out. \033[0m'

.PHONY: start
start: build
	./main.out
