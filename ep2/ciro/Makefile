OUTPUT=mandelbrot

IMAGE=.ppm

CC_OPT=-std=c11
CC_LIB=-lm

CC_OMP=-fopenmp
CC_PTH=-pthread

.PHONY: all
all: $(OUTPUT)_omp $(OUTPUT)_pth $(OUTPUT)_seq $(OUTPUT)_mpi $(OUTPUT)_mpi_pth $(OUTPUT)_mpi_omp

$(OUTPUT)_omp: $(OUTPUT)_omp.c
	gcc -o $(OUTPUT)_omp.o $(CC_OPT) $(CC_LIB) $(CC_OMP) $(OUTPUT)_omp.c  $(CC_LIB)

$(OUTPUT)_pth: $(OUTPUT)_pth.c
	gcc -o $(OUTPUT)_pth.o $(CC_OPT) $(CC_PTH) $(OUTPUT)_pth.c $(CC_LIB)

$(OUTPUT)_seq: $(OUTPUT)_seq.c
	gcc -o $(OUTPUT)_seq.o $(CC_OPT) $(OUTPUT)_seq.c $(CC_LIB)
	
$(OUTPUT)_mpi: $(OUTPUT)_mpi.c
	mpicc -o $(OUTPUT)_mpi.o $(CC_OPT) $(OUTPUT)_mpi.c $(CC_LIB)

$(OUTPUT)_mpi_pth: $(OUTPUT)_mpi_pth.c
	mpicc -o $(OUTPUT)_mpi_pth.o $(CC_OPT) $(CC_PTH) $(OUTPUT)_mpi_pth.c $(CC_LIB)

$(OUTPUT)_mpi_omp: $(OUTPUT)_mpi_omp.c
	mpicc -o $(OUTPUT)_mpi_omp.o $(CC_OPT) $(CC_OMP) $(OUTPUT)_mpi_omp.c $(CC_LIB)

.PHONY: clean
clean:
	rm *.o *$(IMAGE)