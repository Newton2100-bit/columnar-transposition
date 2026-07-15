all: a.out
a.out: columnar-transposition.c 
	cc columnar-transposition.c -O3 -Wall -Wextra

clean:
	rm ./test/input.txt
	rm ./test/test-file.txt

.PHONY: clean
