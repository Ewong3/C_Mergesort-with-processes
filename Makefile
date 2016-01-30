all: Mergesort.c Mergesort.h
	gcc -g Mergesort.c -o Mergesort

clean:
	$(RM) Mergesort
