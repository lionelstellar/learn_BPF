CLANG = clang

INCLUDE_PATH += -I/home/jiangyikun/linux/tools/lib/bpf

LIBRARY_PATH = -L/usr/local/lib64
BPFSO = -lbpf

.PHONY: clean 

clean:
	rm -f main

# manipulate: map_manipulate.c 
# 	$(CLANG) -o map_manipulate.o -lelf $(INCLUDE_PATH) $(LIBRARY_PATH) $(BPFSO) $?

main: map_manipulate.c main.c
	$(CLANG) -o main -lelf $(INCLUDE_PATH) $(LIBRARY_PATH) $(BPFSO) $?


build: main

.DEFAULT_GOAL := build
