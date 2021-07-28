OUTPUT=vecAdd

CC_OPT=-std=c11

#CC_LIB=-lm
#CC_OMP=-fopenmp
#CC_PTH=-pthread
#CFLAGS=-arch=sm_50

.PHONY: all
all: $(OUTPUT)_zero $(OUTPUT)_seq $(OUTPUT)_malloc $(OUTPUT)_simple $(OUTPUT)

$(OUTPUT)_zero: $(OUTPUT)_zero.c
	gcc -o $(OUTPUT)_zero.o $(CC_OPT) $(OUTPUT)_zero.c

$(OUTPUT)_seq: $(OUTPUT)_seq.c
	gcc -o $(OUTPUT)_seq.o $(CC_OPT) $(OUTPUT)_seq.c

$(OUTPUT)_malloc: $(OUTPUT)_malloc.c
	gcc -o $(OUTPUT)_malloc.o $(CC_OPT) $(OUTPUT)_malloc.c

$(OUTPUT)_simple: $(OUTPUT)_simple.cu
	nvcc -o $(OUTPUT)_simple.o $(OUTPUT)_simple.cu

$(OUTPUT): $(OUTPUT).cu
	nvcc -o $(OUTPUT).o $(OUTPUT).cu

.PHONY: clean
clean:
	rm *.o
