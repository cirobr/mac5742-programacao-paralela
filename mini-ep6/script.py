import sys
from subprocess import run

import numpy as np

# get program arguments
prog = [sys.argv[1]]
N = int(sys.argv[2])

# creates ordered sequence
seq_ord = np.arange(100)

success = np.empty(shape=(0,), dtype=bool)
for i in range(N):
    
    # creates random sequence
    seq = np.copy(seq_ord)
    np.random.shuffle(seq)
    args = [str(int) for int in seq]
    print("sequence:", i+1)
    
    # script execution
    result = run(prog + args, capture_output=True, text=True)
    res = result.stdout[:-2]
    res = np.fromstring(res, dtype=int, sep=' ')
    
    # check if result is correct
    success = np.append(success, np.all(np.equal(res, seq_ord)))
        
if np.mean(success) >= 0.9:
    print("ok")
else:
    print("not ok")