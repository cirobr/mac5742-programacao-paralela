OUTPUT=bs_omp

CC=gcc
CC_OPT=-std=c11
CC_LIBS=-lm         # <math.h>

CC_OMP=-fopenmp

.PHONY: all
all: $(OUTPUT)

$(OUTPUT): $(OUTPUT).c
	$(CC) -o $(OUTPUT).o $(CC_OPT) $(CC_OMP) $(OUTPUT).c  $(CC_LIBS)

.PHONY: clean
clean:
	rm $(OUTPUT) $(OUTPUT).o
