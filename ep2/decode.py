# libraries
import pandas as pd

results_filename = "./output.txt"
csv_filename = "./experiment_results.csv"

# read pandas database
df = pd.read_csv(csv_filename, index_col = 0, sep = ",")

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
