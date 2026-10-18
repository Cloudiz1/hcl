.PHONY: run clean
all: run

run:
	gcc tests/main.c -I include/ -o build/tests
	./build/tests

expand:
	gcc tests/main.c -I include/ -E -P -o build/expanded.c

clean:
	rm build/*
