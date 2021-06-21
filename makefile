obj = main.o foo.o
all: 1

1: $(obj)
	$(CC) -m32 -g $(obj) -o $@ -lm
%.o: %.c
	$(CC) -c -g -m32 $< -o $@ -lm

main.o: main.c header.h


%.o: %.s
	nasm -g -f elf32 $< -o $@

clean: FORCE
	rm -f *.o

FORCE: