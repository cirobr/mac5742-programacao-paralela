#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "mpi.h"

double c_x_min;
double c_x_max;
double c_y_min;
double c_y_max;

double pixel_width;
double pixel_height;

int iteration_max = 200;

int image_size;
int NUM_THREADS;
unsigned char **image_buffer;

int i_x_max;
int i_y_max;
int image_buffer_size;

int gradient_size = 16;
int colors[17][3] = {
                        {66, 30, 15},
                        {25, 7, 26},
                        {9, 1, 47},
                        {4, 4, 73},
                        {0, 7, 100},
                        {12, 44, 138},
                        {24, 82, 177},
                        {57, 125, 209},
                        {134, 181, 229},
                        {211, 236, 248},
                        {241, 233, 191},
                        {248, 201, 95},
                        {255, 170, 0},
                        {204, 128, 0},
                        {153, 87, 0},
                        {106, 52, 3},
                        {16, 16, 16},
                    };

void allocate_image_buffer(){
    int rgb_size = 3;
    image_buffer = (unsigned char **) malloc(sizeof(unsigned char *) * image_buffer_size);

    for(int i = 0; i < image_buffer_size; i++){
        image_buffer[i] = (unsigned char *) malloc(sizeof(unsigned char) * rgb_size);
    };
};

void free_image_buffer() {
    for(int i = 0; i < image_buffer_size; i++){
        free(image_buffer[i]);
    };

    free(image_buffer);
}

void init(int argc, char *argv[]){
    if(argc < 6){
        printf("usage: ./mandelbrot_pth c_x_min c_x_max c_y_min c_y_max image_size\n");
        printf("examples with image_size = 11500:\n");
        printf("    Full Picture:         ./mandelbrot_pth -2.5 1.5 -2.0 2.0 11500\n");
        printf("    Seahorse Valley:      ./mandelbrot_pth -0.8 -0.7 0.05 0.15 11500\n");
        printf("    Elephant Valley:      ./mandelbrot_pth 0.175 0.375 -0.1 0.1 11500\n");
        printf("    Triple Spiral Valley: ./mandelbrot_pth -0.188 -0.012 0.554 0.754 11500\n");
        exit(0);
    }
    else{
        sscanf(argv[1], "%lf", &c_x_min);
        sscanf(argv[2], "%lf", &c_x_max);
        sscanf(argv[3], "%lf", &c_y_min);
        sscanf(argv[4], "%lf", &c_y_max);
        sscanf(argv[5], "%d", &image_size);
        sscanf(argv[6], "%d", &NUM_THREADS);

        i_x_max           = image_size;
        i_y_max           = image_size;
        image_buffer_size = image_size * image_size;
        
        pixel_width       = (c_x_max - c_x_min) / i_x_max;
        pixel_height      = (c_y_max - c_y_min) / i_y_max;
    };
};

void update_rgb_buffer(int iteration, int x, int y){
    int color;

    if(iteration == iteration_max){
        image_buffer[(i_y_max * y) + x][0] = colors[gradient_size][0];
        image_buffer[(i_y_max * y) + x][1] = colors[gradient_size][1];
        image_buffer[(i_y_max * y) + x][2] = colors[gradient_size][2];
    }
    else{
        color = iteration % gradient_size;

        image_buffer[(i_y_max * y) + x][0] = colors[color][0];
        image_buffer[(i_y_max * y) + x][1] = colors[color][1];
        image_buffer[(i_y_max * y) + x][2] = colors[color][2];
    };
};

void write_to_file(){
    FILE * file;
    char * filename               = "output.ppm";
    char * comment                = "# ";

    int max_color_component_value = 255;

    file = fopen(filename,"wb");

    fprintf(file, "P6\n %s\n %d\n %d\n %d\n", comment,
            i_x_max, i_y_max, max_color_component_value);

    for(int i = 0; i < image_buffer_size; i++){
        fwrite(image_buffer[i], 1 , 3, file);
    };

    fclose(file);
};

// struct Thread
struct Thread_Data {
	int i_x_init;
	int i_x_max;
	int i_y_init;
	int i_y_max;
};

void *thread_task(void * data_t) {
    int i_x;
    int i_y;
    int iteration;    
    double c_x;
    double c_y;
    double z_x;
    double z_y;
    double z_x_squared;
    double z_y_squared;
    double escape_radius_squared = 4;
    struct Thread_Data * data = (struct Thread_Data *) data_t;
    int i_x_init = data->i_x_init;
    int i_x_max = data->i_x_max;
    int i_y_init = data->i_y_init;
    int i_y_max = data->i_y_max;



    for(i_y = i_y_init; i_y < i_y_max; i_y++){
    c_y = c_y_min + i_y * pixel_height;

    if(fabs(c_y) < pixel_height / 2){
        c_y = 0.0;
    };

        for(i_x = i_x_init; i_x < i_x_max; i_x++){
            c_x         = c_x_min + i_x * pixel_width;

            z_x         = 0.0;
            z_y         = 0.0;

            z_x_squared = 0.0;
            z_y_squared = 0.0;

            for(iteration = 0;
                iteration < iteration_max && \
                ((z_x_squared + z_y_squared) < escape_radius_squared);
                iteration++){
                z_y         = 2 * z_x * z_y + c_y;
                z_x         = z_x_squared - z_y_squared + c_x;

                z_x_squared = z_x * z_x;
                z_y_squared = z_y * z_y;
            };

            update_rgb_buffer(iteration, i_x, i_y);

        }
    }
	pthread_exit(NULL); 
}

void compute_mandelbrot(){
    pthread_t thread[NUM_THREADS];
    pthread_attr_t attr;
    struct Thread_Data data[NUM_THREADS];
    int error_code;
    long t;
    
    pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    
    // inicialização do intervalo que cada thread processa
	int blk = image_size/NUM_THREADS;
	int offset = 0;
	for(int i = 0; i < NUM_THREADS; i++) {
		data[i].i_x_init = 0;
		data[i].i_x_max = image_size;
		data[i].i_y_init = offset;
        data[i].i_y_max = offset+blk;
        offset += blk;
	    }


    data[NUM_THREADS-1].i_y_max += image_size%NUM_THREADS;
    // Create Threads
	for(t = 0; t < NUM_THREADS; t++){

		error_code = pthread_create(&thread[t],
									&attr,
									thread_task,
									(void *) &data[t]);
		if (error_code) {
			printf("ERROR; return code from pthread_create() is %d\n",error_code);
			exit(-1);
			};
    }

    pthread_attr_destroy(&attr);

	// Join thread
	for(t=0; t < NUM_THREADS; t++ ) {
		error_code = pthread_join(thread[t], NULL);
	 	if (error_code) {
	 		printf("ERROR; return code from pthread_join() is %d\n",error_code);
	 	};
     
	 };
	};

int main(int argc, char *argv[]){
    int my_rank;
    int p;
    int dest = 0; 
    int tag = 50; 
    int source;
    MPI_Status status; 

    MPI_Init(&argc,&argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p); 
   
    init(argc, argv);

    int x0 = my_rank*image_size/p;
    int xFim = (my_rank+1)*image_size/p;
    int y0 = 0;
    int yFim = image_size;

    int bufferSize = (xFim - x0) * (yFim - y0)*3;

    allocate_image_buffer();
    compute_mandelbrot(x0, xFim, y0, yFim);

    
    if (my_rank == 0) {
        int* recebido = malloc(bufferSize*sizeof(int));
        for (source = 1; source < p; source++) { 
            MPI_Recv(recebido, bufferSize, MPI_INT, source, tag, MPI_COMM_WORLD, &status); 
            int xIntervalo = xFim - x0;
            int t = bufferSize/3;

            for (int canal = 0; canal < 3; canal++) {
                for (int i = 0; i<t; i++) {
                    int x = source*xIntervalo + i % xIntervalo;
                    int y = i / xIntervalo;

                    image_buffer[(i_y_max * y) + x][canal] = recebido[i + canal*t];
                }
            }
        } 

        free(recebido);
    }
    else {
        int* resultado = malloc(bufferSize*sizeof(int));
        int i = 0;

        for (int y = y0; y<yFim; y++) {
            for (int x = x0; x<xFim; x++) {
                resultado[i] = image_buffer[(i_y_max * y) + x][0];
                i++;
            }
        }

        for (int y = y0; y<yFim; y++) {
            for (int x = x0; x<xFim; x++) {
                resultado[i] = image_buffer[(i_y_max * y) + x][1];
                i++;
            }
        }

        for (int y = y0; y<yFim; y++) {
            for (int x = x0; x<xFim; x++) {
                resultado[i] = image_buffer[(i_y_max * y) + x][2];
                i++;
            }
        }

        MPI_Send(resultado, bufferSize, MPI_INT, dest, tag, MPI_COMM_WORLD);

        free(resultado);
    }

    if (my_rank == 0) {
        write_to_file();
    }

    free_image_buffer();
    
    MPI_Finalize();

    return 0;
};
