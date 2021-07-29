OUTPUT=cudaAdd

.PHONY: all
all: $(OUTPUT)

$(OUTPUT): $(OUTPUT).cu
	nvcc -o $(OUTPUT).o $(OUTPUT).cu

.PHONY: clean
clean:
	rm *.o
