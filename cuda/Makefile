OUTPUT=vecAdd

CC_OPT=-std=c11
CC_CUDA=nvcc

#CC_LIB=-lm
#CC_OMP=-fopenmp
#CC_PTH=-pthread
CFLAGS=-arch=sm_50 -I/opt/cuda/include

.PHONY: all
all: $(OUTPUT)_zero $(OUTPUT)_seq $(OUTPUT)_malloc $(OUTPUT)

$(OUTPUT)_zero: $(OUTPUT)_zero.c
	gcc -o $(OUTPUT)_zero.o $(CC_OPT) $(OUTPUT)_zero.c  $(CC_LIB)

$(OUTPUT)_seq: $(OUTPUT)_seq.c
	gcc -o $(OUTPUT)_seq.o $(CC_OPT) $(OUTPUT)_seq.c  $(CC_LIB)

$(OUTPUT)_malloc: $(OUTPUT)_malloc.c
	gcc -o $(OUTPUT)_malloc.o $(CC_OPT) $(OUTPUT)_malloc.c  $(CC_LIB)

$(OUTPUT): $(OUTPUT).cu
	$(CC_CUDA) $(CFLAGS) -o $(OUTPUT).o

.PHONY: clean
clean:
	rm *.o
