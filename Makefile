CC = gcc
INCLUDE = -Iinclude
CCFLAGS = -g -O2 -Wall -Wextra -Werror


SRC = $(shell find ./src -name "*.c")
OBJ = $(patsubst ./src/%.c, ./obj/%.o, $(SRC))
LIB_TARGET = AES128.a

.phony: all
all: lib tests

.phony: dirs
dirs:
	@mkdir -p obj lib bin

.phony: lib
lib: dirs $(OBJ)
	@echo [AR] ./lib/$(LIB_TARGET)
	@ar -crs ./lib/$(LIB_TARGET) $(OBJ)

obj/%.o: src/%.c
	@echo [CC] $@
	@$(CC) -c $< -o $@ $(CCFLAGS) $(INCLUDE) $(LDFLAGS)

.phony: tests
tests: ./lib/$(LIB_TARGET)
	@echo [CC] bin/tests
	@$(CC) $(CCFLAGS) tests/tests.c lib/$(LIB_TARGET) -o bin/tests $(INCLUDE)

.phony: clean
clean:
	@rm -r obj

.phony: clean_all
clean_all:
	@rm -r obj lib bin
