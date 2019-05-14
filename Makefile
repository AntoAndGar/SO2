CC=gcc

1: prova.c util.h utf.h
	$(CC) $^ -o $@

.PHONY: clean
clean:
	rm -f *.o