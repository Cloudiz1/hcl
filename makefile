.PHONY: run clean
all: full 

full:
	clear
	@mkdir -p build
	@for f in $$(find $(dir) -name "*.c"); do \
		out=build/$$(basename $$f .c); \
		gcc $$f -I include/ -Wall -Werror -o $$out || exit 1; \
		./$$out || exit 1; \
		echo; \
	done

dir = tests/
run:
	gcc $(dir)/$(f).c -I include/ -Wall -Werror -Wpedantic -o build/$(f)
	./build/$(f)

expand:
	gcc $(dir)/$(f).c -I include/ -E -P -o build/$(f)_expanded.c

clean:
	rm -r build/*
