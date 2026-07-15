all: columnar-transposition poly-aphabetic
	
columnar-transposition: columnar-transposition.c 
	cc columnar-transposition.c -O3 -Wall -Wextra -o columnar-transposition

poly-aphabetic: poly-aphabetic.c
	cc poly-aphabetic.c -O3 -Wall -Wextra -o poly-aphabetic

clean:
.PHONY: clean
