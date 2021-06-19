# libraries
#import sys
import pandas as pd

# initialization
path = "./results"
programs = ["/mandelbrot_seq", "/mandelbrot_pth", "/mandelbrot_omp"]
nthreads = ["/1", "/2", "/4", "/6", "/8", "/16", "/32"]
images = ["/elephant.log", "/full.log", "/seahorse.log", "/triple_spiral.log"]

df = pd.DataFrame(data=None, columns=[
                  "program",
                  "threads",
                  "image",
                  "image_size",
                  "time_sec",
                  "time_tolerance_sec"
                  ])

    
# loops
"""
program = programs[2]
nthread = nthreads[0]
image = images[0]
"""
for program in programs:
    for nthread in nthreads:
        if (program == programs[0] and nthread != nthreads[0]):
            break
        
        for image in images:

            if program == programs[0]:
                filename = path + program + image
            else:
                filename = path + program + nthread + image
            
            
            f = open(filename, 'r')
            lines = f.readlines()
            for line in lines:
                l = line.split(sep = " ")
                
                #image_size = 0
                #number_of_threads = 1
            
                if "Performance" in l:
                    #break
                    image_size = l[10]
                    if program != programs[1]:
                        image_size = image_size[:-1]
                    image_size = int(image_size)
                    continue
            
                if "elapsed" in l:
                    l_short = [x for x in l if x]
                    time_seconds = l_short[0]
                    time_seconds = time_seconds.replace(",", ".")
                    time_seconds = float(time_seconds)
                    
                    time_tolerance = l_short[2]
                    time_tolerance = time_tolerance.replace(",", ".")
                    time_tolerance = float(time_tolerance)
                
                    # write in pandas
                    database_row = {
                        "program": program[1:],
                        "threads": nthread[1:],
                        "image": image[1:],
                        "image_size": image_size,
                        "time_sec": time_seconds,
                        "time_tolerance_sec": time_tolerance
                        }
                    
                    df = df.append(database_row, ignore_index=True)
                    
# store df as csv
df.to_csv("experiment_results.csv", index = False, header=True, sep=",")
