OUTPUT=cudaAdd3

#CC_OPT=-std=c11

#CC_LIB=-lm
#CC_OMP=-fopenmp
#CC_PTH=-pthread
#CFLAGS=-arch=sm_50

.PHONY: all
all: $(OUTPUT)

$(OUTPUT): $(OUTPUT).cu
	nvcc -o $(OUTPUT).o $(OUTPUT).cu

.PHONY: clean
clean:
	rm *.o
