CC=gcc

2: prova2.c #util.h utf.h #-ggdb
	$(CC) prova2.c -o $@

.PHONY: clean
clean:
	rm -f *.o