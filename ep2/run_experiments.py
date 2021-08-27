# libraries
import os
#import sys
import pandas as pd
from time import sleep

# setup experiment
results_filename = "output.txt"
csv_filename = "./experiment_results.csv"

programs = ["./mandelbrot_seq.o",
            "./mandelbrot_pth.o", 
            "./mandelbrot_omp.o",
            "./mandelbrot_mpi.o",
            "./mandelbrot_mpi_pth.o",
            "./mandelbrot_mpi_omp.o"]
            
repetitions = 15
nthreads = [1, 4, 8, 16, 32, 64]
image = "-0.188 -0.012 0.554 0.754"
image_size = 4096

df = pd.DataFrame(data=None, columns=[
                  "program",
                  "repetitions",
                  "threads",
                  "image",
                  "image_size",
                  "time_sec",
                  "time_tolerance_sec",
                  ])

os.system("rm output.txt")


# build commmand header
command_header = "perf stat -r " + str(repetitions)

# build command lines
command_lines = []
for nthread in nthreads:
    nth = int(nthread / 2)

    for program in programs:
        r1 = ("seq" in program) and (nthread > 1)
        r2 = ("mpi.o" in program) and (nthread > 16)
        r3 = ("mpi_pth" in program) and (nthread == 1)
        r4 = ("mpi_omp" in program) and (nthread == 1)

        if r1 or r2 or r3 or r4:
            continue

        if nthread > 8:
            n_mpi = 8
            n_oth = nthread - 8
        else:
            n_mpi = int(nthread / 2)
            n_oth = int(nthread / 2)
        
        prg = program + " " + image + " " + str(image_size)
        
        # mount omp command line preparation
        omp_prep_simple = "export OMP_NUM_THREADS=" + str(nthread) + "|"
        omp_prep_dual   = "export OMP_NUM_THREADS=" + str(n_oth) + "|"

        # mount mpi_command
        mpi_command_simple = "mpiexec -np " + str(nthread)
        mpi_command_dual   = "mpiexec -np " + str(n_mpi)

        # mount pthread command
        pth_command_simple = str(nthread)
        pth_command_dual   = str(n_oth)
        
        if "seq" in program:
            command_line = command_header + " " + prg
        
        elif "mpi_pth" in program:
            command_line = command_header + " " + mpi_command_dual + " " + prg + " " + pth_command_dual
            
        elif "mpi_omp" in program:
            command_line = omp_prep_dual + command_header + " " + mpi_command_dual + " " + prg
        
        elif "mpi" in program:
            command_line = command_header + " " + mpi_command_simple + " " + prg
            
        elif "pth" in program:
            command_line = command_header + " " + prg + " " + pth_command_simple

        elif "omp" in program:
            command_line = omp_prep_simple + command_header + " " + prg

            
        command_lines.append(command_line)
        
        
        # write in pandas
        database_row = {
            "program": program,
            "repetitions": repetitions,
            "threads": nthread,
            "image": image,
            "image_size": image_size
            }
        df = df.append(database_row, ignore_index=True)


# execute all commmand lines
ind = 0
for command_line in command_lines:
    full_command_line = command_line + " >> " + results_filename + " 2>&1"
    print(full_command_line)

    os.system(full_command_line)
    figure_name = str(ind) + ".ppm"
    os.rename("output.ppm", figure_name)
    ind += 1
    sleep(1)


# decode output and store on pandas
ind = 0
f = open(results_filename, 'r')
lines = f.readlines()
for line in lines:
    if "elapsed" in line:
        l = line.split(sep = " ")
        l_short = [x for x in l if x]
        time_seconds = l_short[0]
        time_seconds = time_seconds.replace(",", ".")
        time_seconds = float(time_seconds)
        df.iloc[ind, 5] = time_seconds

        time_tolerance_sec = l_short[2]
        time_tolerance_sec = time_tolerance_sec.replace(",", ".")
        time_tolerance_sec = float(time_tolerance_sec)
        df.iloc[ind, 6] = time_tolerance_sec

        ind += 1


# store df as csv
df.to_csv(csv_filename, index = True, header=True, sep=",")
