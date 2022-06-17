NAME := $(shell basename $(PWD))
export MODULE := M1
all: $(NAME)-64 $(NAME)-32


test:
	./$(NAME)-64 test
	
include ../Makefile
