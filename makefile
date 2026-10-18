run:
	gcc tests/main.c -I tests/__internal_testing.h -I include/ -o build/tests
	./build/tests

clean:
	rm build/*
