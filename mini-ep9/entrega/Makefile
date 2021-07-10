OUTPUT=threadpool

CC=gcc
CC_OPT=-std=c11

#CC_LIBS=-lm         # <math.h>
CC_PTH=-pthread     #<pthread.h>

.PHONY: all
all: $(OUTPUT)

$(OUTPUT): $(OUTPUT).c
	$(CC) -o $(OUTPUT).o $(CC_OPT) $(CC_PTH) $(OUTPUT).c $(CC_LIBS)

.PHONY: clean
clean:
	rm $(OUTPUT).o
