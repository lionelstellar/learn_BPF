CLANG = clang

INCLUDE_PATH += -I/home/jiangyikun/linux/tools/lib/bpf

LIBRARY_PATH = -L/usr/local/lib64
BPFSO = -lbpf

.PHONY: clean 

clean:
	rm -f fetch bpf_fs/create bpf_fs/update

fetch: 	map_pinning_fetch.c 
	$(CLANG) -o fetch -lelf $(INCLUDE_PATH) $(LIBRARY_PATH) $(BPFSO) $?

create: bpf_fs/map_create.c
	$(CLANG) -o bpf_fs/create -lelf $(INCLUDE_PATH) $(LIBRARY_PATH) $(BPFSO) $?

update: bpf_fs/map_update.c
	$(CLANG) -o bpf_fs/update -lelf $(INCLUDE_PATH) $(LIBRARY_PATH) $(BPFSO) $?

build: fetch create update

.DEFAULT_GOAL := build
