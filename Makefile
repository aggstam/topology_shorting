FILE = test_file
OUTPUT = output
THREADS = 4

all:
	$(info Executing normal code...)
	gcc -o topology_shorting topology_shorting.c
	./topology_shorting $(FILE) $(OUTPUT)

parallel:
	$(info Executing parallel code...)
	gcc -o topology_shorting_parallel topology_shorting_parallel.c
	./topology_shorting_parallel $(THREADS) $(FILE) $(OUTPUT)

clean:
	rm -f topology_shorting topology_shorting_parallel

.PHONY: all parallel clean
